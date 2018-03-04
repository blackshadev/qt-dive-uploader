#include "littledivelog.h"
#include "../jsonrequest.h"

LittleDiveLog::LittleDiveLog(QObject *parent) : QObject(parent)
{
}

bool LittleDiveLog::isLoggedIn()
{
    return m_token.length() > 0;
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
                qInfo("Gotdata: " + resp.data.toJson());
            }
        }

        delete req;
    });

    req->send();

}
