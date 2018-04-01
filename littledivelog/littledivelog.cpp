#include "littledivelog.h"
#include "../jsonrequest.h"

LittleDiveLog::LittleDiveLog(QObject *parent) : QObject(parent)
{
}

LittleDiveLog::~LittleDiveLog()
{
    if(m_user_info != NULL) {
        delete m_user_info;
    }
}

bool LittleDiveLog::isLoggedIn()
{
    return !m_refresh_token.isEmpty();
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
                set_refresh_token(obj["jwt"].toString());
                emit loggedStateChanged(isLoggedIn());
                get_user_data();
            }
        }

        delete req;
    });

    req->send();
}

void LittleDiveLog::set_refresh_token(QString tok)
{
    m_refresh_token = tok;
    emit refreshTokenChanged(tok);
    if(!tok.isEmpty()) {
        get_user_data();
    }
}

QString LittleDiveLog::get_refresh_token()
{
    return m_refresh_token;
}

void LittleDiveLog::get_user_data()
{
    request(
        RequestMethod::GET,
        "/user/profile",
        NULL,
        [=](JsonResponse resp) {
            auto obj = resp.data.object();
            m_user_info = new UserInfo(this);

            auto dts = obj["inserted"].toString().left(19);
            auto dt = QDateTime::fromString(dts, "yyyy-MM-dd HH:mm:ss");

            m_user_info->m_user_id = obj["user_id"].toInt();
            m_user_info->m_name = obj["name"].toString();
            m_user_info->m_email = obj["email"].toString();
            m_user_info->m_inserted = dt;
            m_user_info->m_dive_count = obj["dive_count"].toInt();
            m_user_info->m_computer_count = obj["computer_count"].toInt();
            m_user_info->m_buddy_count = obj["buddy_count"].toInt();
            m_user_info->m_tag_count = obj["tag_count"].toInt();

            emit userInfoChanged(m_user_info);
        }
    );
}

void LittleDiveLog::get_access_token(std::function<void()> callback)
{
    raw_request(
        RequestMethod::GET,
        "/auth/access-token",
        TokenType::REFRESH,
        NULL,
        [=](JsonResponse resp) {
            auto obj = resp.data.object();
            if(resp.statuscode == 401 || resp.hasError()) {
                emit error("Invalid refresh token");
                m_access_token = QString();
                set_refresh_token(NULL);
                emit loggedStateChanged(isLoggedIn());
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
    req->url = "https://dive.littledev.nl/api" + path;
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

void LittleDiveLog::request(RequestMethod method, QString path, QJsonObject *data, std::function<void(JsonResponse)> callback, bool retry)
{
    if(!isLoggedIn()) {
        emit error("Cannot execute request. Not yet logged in.");
        return;
    }
    if(m_access_token.isNull()) {
        get_access_token(
            [=]() {
                // Perform actual request but no retries
                request(method, path, data, callback, false);
            }
        );
    } else {
        raw_request(method, path, TokenType::ACCESS, data, [=](JsonResponse resp) {
            if(retry == true && resp.statuscode == 401) {
                // 401, retry request after get_access_token
                m_access_token.clear();
                request(method, path, data, callback, false);
            } else {
                // Done, callback
                callback(resp);
            }
        });
    }
}

void LittleDiveLog::logout() {
    if(!isLoggedIn()) {
        emit error("Cannot logout, while not yet logged in.");
        return;
    }

    request(RequestMethod::DELETE, "auth/refresh-token/" + m_refresh_token, NULL, [=](JsonResponse resp) {
        set_refresh_token(NULL);
        emit loggedStateChanged(isLoggedIn());
        if(resp.statuscode != 200) {
            emit error(resp.errorString());
        }
    }, false);

}
