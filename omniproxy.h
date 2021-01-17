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
//#include <QtNetwork/QNetworkInterface>
#include <QHostAddress>
#include <QUrl>

class OmniProxy : public QObject
{
    Q_OBJECT

    enum EndPoint{
        PostJoinNetwork = 0,
        GetDevicesList = 1,
        GetVirtualNetworkSecret = 2
    };

    typedef struct{
         QString name;
         QString virtualIP;
         QString instanceID;
         QString summary;
         QString description;
    }Device;

public:
    explicit OmniProxy(QObject *parent = nullptr);
    ~OmniProxy();

public slots:
    void handleFinished(QNetworkReply *networkReply);
    void handleReadyRead(QNetworkReply *networkReply);

    void setToken(QString token);
    void joinVirtualNetwork();
    void getDeviceList();
    void getVirtualNetworkKey();



signals:

private:

    // Local
    QString token;
    QString instanceID;
    QString deviceName;
    QString description;
    QString deviceLanIp;
    QString deviceMacAddr;

    // Fetch from API
    QString virtualIP;
    QString encryptMethod;
    QString secretKey;
    QString communityName;

    QList<Device> devices;
    QNetworkAccessManager *networkManager;
    QHash<QNetworkReply*, EndPoint> endPoints;
    QString apiUrl = "https://ae4ffa2f-d6a5-42bc-878a-05ab291b9ab1.mock.pstmn.io";


    //QString getInternalIP();
    void generatePubKey();

};

#endif // OMNIPROXY_H
