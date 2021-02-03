#include "oauthreplyhandler.h"
#include <QString>
#include <QtNetworkAuth/QOAuthHttpServerReplyHandler>

OAuthReplyHandler::OAuthReplyHandler(quint16 port, QObject *parent) :
    QOAuthHttpServerReplyHandler(QHostAddress::Any, port, parent)
{
    this->custom_port = port;
            listen(QHostAddress::LocalHost, port);
}

QString OAuthReplyHandler::callback() const
{
    const QUrl url(QString::fromLatin1("http://localhost:" + QString::number(this->port()).toLatin1() + "/"));
    return url.toString(QUrl::EncodeDelimiters);
}
