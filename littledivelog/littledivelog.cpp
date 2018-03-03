#include "littledivelog.h"

LittleDiveLog::LittleDiveLog(QObject *parent) : QObject(parent)
{
}

bool LittleDiveLog::isLoggedIn()
{
    return m_token.length() > 0;
}

void LittleDiveLog::login(QString email, QString password)
{
    auto url = QUrl("https://dive.littledev.nl/api/auth/refresh-token");
    auto req = QNetworkRequest(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

    QJsonObject data;
    data["email"] = email;
    data["password"] = password;
    QJsonDocument doc(data);
    QByteArray bytes = doc.toJson();

    auto reply = m_qnam.post(req, bytes);

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
