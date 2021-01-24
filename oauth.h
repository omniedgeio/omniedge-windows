#ifndef OAUTH_H
#define OAUTH_H

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

class OAuth : public QObject
{
    Q_OBJECT
public:
    explicit OAuth(QObject *parent = nullptr);
    virtual ~OAuth();

public slots:
    void grant();
    void clearToken();
signals:
   void granted();
private:
    QOAuth2AuthorizationCodeFlow* oauth;
};

#endif // OAUTH_H
