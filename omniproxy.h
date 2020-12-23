#ifndef OMNIPROXY_H
#define OMNIPROXY_H

#include <QtCore>
#include <QObject>
#include <QString>
#include <QSysInfo>
#include <QtNetwork/QTcpSocket>
#include <QIODevice>
#include <QByteArray>
#include <QMetaObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>

class OmniProxy : public QObject
{
    Q_OBJECT
public:
    explicit OmniProxy(QObject *parent = nullptr);
    ~OmniProxy();
    void JoinToVirtualNetwork();
    void GetDeviceList();
    void GetVirtualNetworkKey();

public slots:
    void handleFinished(QNetworkReply *networkReply);
    void readDeviceList(QNetworkReply *networkReply);


private:
    QNetworkAccessManager *networkManager;
    QString deviceName,description;

};

#endif // OMNIPROXY_H
