#ifndef MYOAUTHHTTPSERVERREPLYHANDLER_H
#define MYOAUTHHTTPSERVERREPLYHANDLER_H

#include <QOAuthHttpServerReplyHandler>

class MyOAuthHttpServerReplyHandler : public QOAuthHttpServerReplyHandler
{
public:
    explicit MyOAuthHttpServerReplyHandler( QObject *parent = nullptr);
    explicit MyOAuthHttpServerReplyHandler(quint16 port, QObject *parent = nullptr);
    explicit MyOAuthHttpServerReplyHandler(const QHostAddress &address, quint16 port,
                                            QObject *parent = nullptr);

    QString callback() const override;
};

#endif // MYOAUTHHTTPSERVERREPLYHANDLER_H
