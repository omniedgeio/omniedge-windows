#ifndef OAUTH_H
#define OAUTH_H

#include <QString>
#include <QObject>
#include <QtNetworkAuth>
#include <QAbstractOAuthReplyHandler>
#include "oauthreplyhandler.h"

const QString SETTINGS_ID_TOKEN = "IdToken";
const QString SETTINGS_EXPIRES_IN = "ExpiresIn";
const QString SETTINGS_ACCESS_TOKEN = "AccessToken";
const QString SETTINGS_REFRESH_TOKEN = "RefreshToken";

class OAuth : public QObject
{
    Q_OBJECT
public:
    explicit OAuth(QObject *parent = nullptr);
    virtual ~OAuth();

public slots:
    void grant();
    void tokensReceived(const QVariantMap &data);

signals:
    void granted();

private:
    OAuthReplyHandler* replyHandler;
    QOAuth2AuthorizationCodeFlow* oAuthFlow;
};

#endif // OAUTH_H
