#ifndef OAUTH_H
#define OAUTH_H

#include <QObject>
#include <QtNetworkAuth>
#include "oauthreplyhandler.h"

class OAuth : public QObject
{
    Q_OBJECT
public:
    explicit OAuth(QObject *parent = nullptr);
    virtual ~OAuth();

public slots:
    void grant();
    void tokenReceived(const QVariantMap &data);

signals:
    void granted();

private:
    OAuthReplyHandler* replyHandler;
    QOAuth2AuthorizationCodeFlow* oAuthFlow;
};

#endif // OAUTH_H
