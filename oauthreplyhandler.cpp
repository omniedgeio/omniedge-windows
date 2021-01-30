#include "oauthreplyhandler.h"
#include <QString>

OAuthReplyHandler::OAuthReplyHandler(quint16 port, QObject *parent) :
    QOAuthHttpServerReplyHandler(QHostAddress::Any, port, parent)
{}

QString OAuthReplyHandler::callback() const
{
    const QUrl url(QString::fromLatin1("http://localhost:" + QString::number(this->port()).toLatin1() + "/"));
    return url.toString(QUrl::EncodeDelimiters);
}
