#include "littledivelog.h"
#include "../http/jsonrequest.h"

LittleDiveLog::LittleDiveLog(QObject *parent) : QObject(parent)
{
    requests = new RequestContainer(parent);
}

LittleDiveLog::~LittleDiveLog()
{
    delete requests;
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
    auto req = requests->request();
    req->setURL("https://dive.littledev.nl/api/auth/refresh-token");
    req->setMethod(RequestMethod::POST);

    QJsonObject data;
    data["email"] = email;
    data["password"] = password;
    req->setBody(data);

    connect(req, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    connect(req, &AsyncRequest::finished, this, [=]() {
        auto resp = req->getResponse();

        auto jsonVal = resp->getBodyAsJSON();
        if (!jsonVal.isObject()) {
            return;
        }
        auto obj = jsonVal.toObject();

        if(obj.contains("error")) {
            emit error(obj["error"].toString());
        } else {
            set_refresh_token(obj["jwt"].toString());
            emit loggedStateChanged(isLoggedIn());
            fetch_user_data();
        }

    });

    req->send();
}

void LittleDiveLog::set_refresh_token(QString tok)
{
    m_refresh_token = tok;
    emit refreshTokenChanged(tok);
    emit loggedStateChanged(isLoggedIn());
    if(!tok.isEmpty()) {
        fetch_user_data();
    }
}

QString LittleDiveLog::get_refresh_token()
{
    return m_refresh_token;
}

void LittleDiveLog::fetch_user_data()
{
    request(
        RequestMethod::GET,
        "/user/profile",
        NULL,
        [=](HTTPResponse *resp) {

            auto obj = resp->getBodyAsJSON().toObject();
            if(m_user_info == NULL) {
                m_user_info = new UserInfo(this);
            }

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

            fetch_user_computers([=]() {
                emit userInfoChanged(m_user_info);
            });

        }
    );
}

bool LittleDiveLog::hasUserData()
{
    return m_user_info != NULL;
}

void LittleDiveLog::fetch_user_computers(std::function<void()> callback) {

    request(
        RequestMethod::GET,
        "/computer",
        NULL,
        [=](HTTPResponse *resp) {
            auto arr = resp->getBodyAsJSON().toArray();

            for(auto val : arr) {
                auto obj = val.toObject();
                m_user_info->addComputer(obj);
            }

            callback();
        }
    );

}

void LittleDiveLog::get_access_token(std::function<void()> callback, QObject* parent)
{
    raw_request(
        RequestMethod::GET,
        "/auth/access-token",
        RequestTokenType::REFRESH,
        NULL,
        [=](HTTPResponse *resp) {
            auto val = resp->getBodyAsJSON();

            if(resp->getStatusCode() == 401 || !val.isObject()) {
                emit error("Invalid refresh token");
                m_access_token = QString();
                set_refresh_token(NULL);
                emit loggedStateChanged(isLoggedIn());
                return;
            }

            auto obj = val.toObject();
            if(!obj.contains("jwt")) {
                throw std::runtime_error("Expected webservice to return jwt");
            } else {
                m_access_token = obj["jwt"].toString();
                callback();
            }

        },
        parent
    );
}

void LittleDiveLog::raw_request(
        RequestMethod method,
        QString path,
        RequestTokenType tokenType,
        QJsonObject *data,
        std::function<void(HTTPResponse *)> callback,
        QObject* parent
) {
    auto req = requests->request();
    req->setURL("https://dive.littledev.nl/api" + path);
    req->setMethod(method);
    if (data) {
        req->setBody(*data);
    }

    switch(tokenType) {
        case RequestTokenType::ACCESS:
            req->setHeader("Authorization", QString("Bearer ") + m_access_token);
        break;
        case RequestTokenType::REFRESH:
            req->setHeader("Authorization", QString("Bearer ") + m_refresh_token);
        break;
    }

    connect(req, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    connect(req, &AsyncRequest::finished, this, [=]() {
        callback(req->getResponse());
    });

    req->send();
}

void LittleDiveLog::request(
        RequestMethod method,
        QString path,
        QJsonObject *data,
        std::function<void(HTTPResponse *)> callback,
        bool retry,
        QObject* parent
) {
    if(!isLoggedIn()) {
        emit error("Cannot execute request. Not yet logged in.");
        return;
    }
    if(m_access_token.isNull()) {
        get_access_token(
            [=]() {
                // Perform actual request but no retries
                request(method, path, data, callback, false);
            },
            parent
        );
    } else {
        raw_request(method, path, RequestTokenType::ACCESS, data, [=](HTTPResponse *resp) {
            if(retry == true && resp->getStatusCode() == 401) {
                // 401, retry request after get_access_token
                m_access_token.clear();
                request(method, path, data, callback, false, parent);
            } else {
                // Done, callback
                callback(resp);
            }
        }, parent);
    }
}

void LittleDiveLog::logout() {
    if(!isLoggedIn()) {
        emit error("Cannot logout, while not yet logged in.");
        return;
    }

    raw_request(RequestMethod::DELETE, "/auth/refresh-token/", RequestTokenType::REFRESH, NULL, [=](HTTPResponse *resp) {
        set_refresh_token(NULL);
        m_access_token.clear();
        emit loggedStateChanged(isLoggedIn());
        if(resp->getStatusCode() != 200) {
            emit error(resp->errorString());
        }
    });

}
