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
#include <QtNetwork/QNetworkInterface>
#include <QHostAddress>
#include <QUrl>

class OmniProxy : public QObject
{
    Q_OBJECT

    typedef struct{
         QString tapIP;
         QString encryptMethod;
    }V_Net_Reply;

    typedef struct{
         QString instanceID;
         QString name;
         QString tapIP;
         QString summary;
         QString description;
    }Devices_Reply;

    typedef struct{
         QString communityName;
         QString secretKey;
    }SecretKey_Reply;

public:
    explicit OmniProxy(QObject *parent = nullptr);
    ~OmniProxy();

    void joinToVirtualNetwork();

public slots:
    void handleFinished(QNetworkReply *networkReply);
    void handleReadyRead(QNetworkReply *networkReply);
    void getDeviceList();
    void getVirtualNetworkKey();

signals:

private:
    QNetworkAccessManager *networkManager;
    QString deviceName,description;
    V_Net_Reply vNetReply;
    Devices_Reply devicesReply;
    SecretKey_Reply secretKeyReply;
    QString getInternalIP();

};

#endif // OMNIPROXY_H
