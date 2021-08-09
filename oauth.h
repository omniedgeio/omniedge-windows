#ifndef OAUTH_H
#define OAUTH_H

#include <QString>
#include <QObject>
#include <QtNetworkAuth>
#include <QAbstractOAuthReplyHandler>
#include "oauthreplyhandler.h"

const QString SETTINGS_ID_TOKEN = "IdToken";

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
