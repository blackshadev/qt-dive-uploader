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
    JsonRequest req;
    req.url = "https://dive.littledev.nl";
    req.path = "api/auth/refresh-token";
    req.method = RequestMethod::POST;

    req.data["email"] = email;
    req.data["password"] = password;

    connect(req, &QNetworkReply::completed, this, [=](JsonResponse resp) {
        if(resp.parseError.error != QJsonParseError::NoError) {
        } else {
            auto obj = resp.data.object();
            if(obj.contains("error")) {
                emit error(obj["error"].toString());
            } else {
                qInfo(resp.data.toJson());
            }
        }
    });

    req.send();

    connect(reply, &QNetworkReply::finished, this, [=]() {
        auto retrDat = reply->readAll();

        QJsonDocument readDoc = QJsonDocument::fromJson(retrDat);
        auto retrObj = readDoc.object();

        if(retrObj.contains("error")) {
            emit error(retrObj["error"].toString());
        }

        reply->deleteLater();
    });

}
