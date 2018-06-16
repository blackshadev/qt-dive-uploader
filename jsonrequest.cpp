#include "jsonrequest.h"

QString JsonResponse::errorString() {
    if(!m_error.isEmpty()) {
        return m_error;
    } else if(parseError.error != parseError.NoError) {
        return parseError.errorString();
    } else if(data.object().contains("error")) {
        return data.object()["error"].toString();
    }

    return "";
}

bool JsonResponse::hasError() {
    return !m_error.isEmpty() || parseError.error != parseError.NoError || statuscode != 200;
}

QSet<JsonRequest*>* JsonRequest::pendingRequests = NULL;
bool JsonRequest::isStopped = false;

void JsonRequest::abortAllPendingRequest() {
    if(JsonRequest::pendingRequests == NULL) {
        return;
    }

    for(JsonRequest* req : JsonRequest::pendingRequests->values()) {
        req->m_reply->abort();
    }

}

void JsonRequest::stop() {
    JsonRequest::isStopped = true;
    JsonRequest::abortAllPendingRequest();
    JsonRequest::cleanupStatics();
}

void JsonRequest::cleanupStatics() {
    if(JsonRequest::isStopped && JsonRequest::pendingRequests != NULL && JsonRequest::pendingRequests->count() == 0) {
        delete JsonRequest::pendingRequests;
        JsonRequest::pendingRequests = NULL;
    }
}

JsonRequest::JsonRequest(QObject *parent) : QObject(parent)
{
    if(JsonRequest::pendingRequests == NULL) {
        JsonRequest::pendingRequests = new QSet<JsonRequest*>();
    }
    m_state = RequestState::None;
    m_reply = NULL;
}

JsonRequest::~JsonRequest()
{
    if(m_reply != NULL) {
        m_reply->deleteLater();
        m_reply = NULL;
    }

    if(JsonRequest::isStopped) {
        JsonRequest::cleanupStatics();
    }
}

void JsonRequest::send()
{

    JsonRequest::pendingRequests->insert(this);

    if(m_state != RequestState::None) {
        throw std::runtime_error("Request already send");
    }
    m_state = RequestState::Sending;
    QByteArray verb;
    switch(method) {
        case RequestMethod::GET:
            verb = "GET";
        break;
        case RequestMethod::POST:
            verb = "POST";
        break;
        case RequestMethod::DELETE:
            verb = "DELETE";
        break;
        case RequestMethod::PUT:
            verb = "PUT";
        break;
        case RequestMethod::PATCH:
            verb = "PATCH";
        break;
    }

    QNetworkRequest req(QUrl::fromUserInput(url));
    req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    if(!jwt.isEmpty()) {
        QString authHeader = "Bearer " + jwt;
        req.setRawHeader("Authorization", authHeader.toLocal8Bit());
    }

    QByteArray jsonBytes;
    if(!data.isEmpty()) {
        jsonBytes = data.toJson();
    }

    m_reply = m_qnam.sendCustomRequest(req, verb, jsonBytes);
    connect(m_reply, &QNetworkReply::finished, this, &JsonRequest::read_reply);
    connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &JsonRequest::read_error);
}

void JsonRequest::read_reply()
{
    m_state = RequestState::Reading;
    auto replyData = m_reply->readAll();

    JsonResponse resp;
    resp.statuscode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    resp.data = QJsonDocument::fromJson(replyData, &resp.parseError);
    m_state = RequestState::Completed;
    do_done();
    emit complete(resp);
}

void JsonRequest::read_error()
{
    m_state = RequestState::Reading;
    auto replyData = m_reply->readAll();

    JsonResponse resp;
    QJsonParseError err;
    resp.data = QJsonDocument::fromJson(replyData, &err);
    if(err.error == QJsonParseError::NoError) {
        auto obj = resp.data.object();
        if(obj.contains("error")) {
            resp.m_error = obj["error"].toString();
        }
    }

    if(resp.m_error.isEmpty()) {
        resp.m_error = m_reply->errorString();

    }
    resp.statuscode = -1;
    qWarning(("Got network err: " + m_reply->errorString()).toLocal8Bit().data());
    do_done();
    emit complete(resp);
}

void JsonRequest::do_done()
{
    JsonRequest::pendingRequests->remove(this);
}
