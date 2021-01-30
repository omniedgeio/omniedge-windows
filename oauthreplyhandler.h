#ifndef OAUTHREPLYHANDLER_H
#define OAUTHREPLYHANDLER_H

#include <QObject>
#include <QOAuthHttpServerReplyHandler>

/**
 * @brief The OAuthReplyHandler class
 *
 * Customize OAuthReplyHandler
 * To fix trailing slash on aws cognito
 */

class OAuthReplyHandler: public QOAuthHttpServerReplyHandler
{
public:
    explicit OAuthReplyHandler(quint16 port, QObject *parent = nullptr);

    QString callback() const override;
};

#endif // OAUTHREPLYHANDLER_H
