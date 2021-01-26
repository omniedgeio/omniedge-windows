#include "myoauthhttpserverreplyhandler.h"
#include <QtNetwork/qtnetwork-config.h>
#include <QNetworkReply>
#include <QFile>
MyOAuthHttpServerReplyHandler::MyOAuthHttpServerReplyHandler(QObject *parent):QOAuthHttpServerReplyHandler(QHostAddress::Any, 0, parent)
{
}

MyOAuthHttpServerReplyHandler::MyOAuthHttpServerReplyHandler(quint16 port, QObject *parent) :QOAuthHttpServerReplyHandler(QHostAddress::Any, port, parent)
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
}

//MyOAuthHttpServerReplyHandler::MyOAuthHttpServerReplyHandler(const QHostAddress &address,
//                                                           quint16 port, QObject *parent) :
//    QOAuthOobReplyHandler(parent),
//    d_ptr(new QOAuthHttpServerReplyHandlerPrivate(this))
//{
//    listen(address, port);
//}

QString MyOAuthHttpServerReplyHandler::callback() const
{
    const QUrl url(QString::fromLatin1("http://localhost:8080/"));
    return url.toString(QUrl::EncodeDelimiters);
}
