#include "littledivelog.h"
#include "../jsonrequest.h"

LittleDiveLog::LittleDiveLog(QObject *parent) : QObject(parent)
{
}

bool LittleDiveLog::isLoggedIn()
{
    return m_refresh_token.length() > 0;
}

void LittleDiveLog::login(QString email, QString password)
{
    JsonRequest* req = new JsonRequest();
    req->url = "https://dive.littledev.nl/api/auth/refresh-token";
    req->method = RequestMethod::POST;

    QJsonObject data;
    data["email"] = email;
    data["password"] = password;

    req->data.setObject(data);

    connect(req, &JsonRequest::complete, this, [=](JsonResponse resp) {
        if(resp.parseError.error != QJsonParseError::NoError) {
            emit error("Invalid response from server");
        } else {
            auto obj = resp.data.object();
            if(obj.contains("error")) {
                emit error(obj["error"].toString());
            } else {
                m_refresh_token = obj["jwt"].toString();
                emit loggedIn();
            }
        }

        delete req;
    });

    req->send();
}

void LittleDiveLog::get_access_token(std::function<void()> callback)
{
    raw_request(
        RequestMethod::GET,
        "access-token",
        TokenType::REFRESH,
        NULL,
        [=](JsonResponse resp) {
            auto obj = resp.data.object();
            if(resp.statuscode == 401) {
                emit error("Invalid refresh token");
                m_access_token.clear();
                m_refresh_token.clear();
                emit loggedOut();
            } else if(!obj.contains("jwt")) {
                throw std::runtime_error("Expected webservice to return jwt");
            } else {
                m_access_token = obj["jwt"].toString();
                callback();
            }

        }
    );
}

void LittleDiveLog::raw_request(RequestMethod method, QString path, TokenType tokenType, QJsonObject *data, std::function<void(JsonResponse)> callback)
{
    JsonRequest* req = new JsonRequest();
    req->url = "https://dive.littledev.nl/api/" + path;
    req->method = method;

    switch(tokenType) {
        case TokenType::ACCESS:
            req->jwt = m_access_token;
        break;
        case TokenType::REFRESH:
            req->jwt = m_refresh_token;
        break;
    }

    if(data != NULL) {
        req->data.setObject(*data);
    }

    connect(req, &JsonRequest::complete, this, [=](JsonResponse resp) {
        if(resp.parseError.error != QJsonParseError::NoError) {
            emit error(resp.parseError.errorString());
        } else {
            callback(resp);
        }
        delete req;
    });

    req->send();
}

void LittleDiveLog::request(RequestMethod method, QString path, QJsonObject *data, std::function<void(JsonResponse)> callback)
{
    if(!isLoggedIn()) {
        emit error("Cannot execute request. Not yet logged in.");
        return;
    }
    if(m_access_token.isEmpty()) {
        get_access_token(
            [=]() {
                request(method, path, data, callback);
            }
        );
    }
}

void LittleDiveLog::logout() {
    if(!isLoggedIn()) {
        emit error("Cannot logout, while not yet logged in.");
        return;
    }

    JsonRequest* req = new JsonRequest();
    req->url = "https://dive.littledev.nl/api/auth/refresh-token/" + m_refresh_token;
    req->method = RequestMethod::DELETE;

    connect(req, &JsonRequest::complete, this, [=](JsonResponse resp) {

    });

    req->send();
}
