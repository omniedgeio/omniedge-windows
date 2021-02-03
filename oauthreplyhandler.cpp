#include "oauthreplyhandler.h"
#include <QString>
#include <QFile>

OAuthReplyHandler::OAuthReplyHandler(quint16 port, QObject *parent) :
    QOAuthHttpServerReplyHandler(QHostAddress::Any, port, parent)
{
    QByteArray val;
    QFile file;
    file.setFileName(":/success.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        val = file.readAll();
        file.close();
    }
    this->setCallbackText(val);

    this->customPort = port;
    listen(QHostAddress("localhost"), port);
}

QString OAuthReplyHandler::callback() const
{
    const QUrl url(QString::fromLatin1("http://localhost:" + QString::number(this->customPort).toLatin1() + "/"));
    return url.toString(QUrl::EncodeDelimiters);
}
