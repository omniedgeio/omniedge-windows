#ifndef GOOGLEOAUTH_H
#define GOOGLEOAUTH_H

#include <QObject>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QSettings>

class GoogleOAuth : public QObject
{
    Q_OBJECT
public:
    explicit GoogleOAuth(QObject *parent = nullptr);
    virtual ~GoogleOAuth();

public slots:
    void grant();

private:
    QOAuth2AuthorizationCodeFlow* google;
    QSettings *reg;
};

#endif // GOOGLEOAUTH_H
