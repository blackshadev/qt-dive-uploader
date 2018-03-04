#include "jsonrequest.h"

JsonRequest::JsonRequest(QObject *parent) : QObject(parent)
{
    m_state = RequestState::None;
    m_reply = NULL;
}

JsonRequest::~JsonRequest()
{
    if(m_reply != NULL) {
        m_reply->deleteLater();
        m_reply = NULL;
    }
}

void JsonRequest::send()
{
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

    QNetworkRequest req(QUrl::fromUserInput(url + "/" + path));
    req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

    QByteArray jsonBytes;
    if(!data.isEmpty()) {
        jsonBytes = data.toJson();
    }

    m_reply = m_qnam.sendCustomRequest(req, verb, jsonBytes);
    connect(m_reply, &QNetworkReply::finished, this, &JsonRequest::read_reply);
}

void JsonRequest::read_reply()
{
    m_state = RequestState::Reading;
    auto replyData = m_reply->readAll();

    JsonResponse resp;
    resp.statuscode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    resp.data.fromJson(replyData, &resp.parseError);
    m_state = RequestState::Completed;
    emit complete(resp);
}
