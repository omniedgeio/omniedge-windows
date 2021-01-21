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
#include <QThread>
#include <QList>
#include "googleoauth.h"

struct Device {
    QString id;
    QString name;
    QString virtualIP;
    QString description;
};

struct VirtualNetwork {
    QString id;
    QString ipPrefix;
    QString communityName;
    QList<Device> devices;
};

const QString LIST_VIRTUAL_NETWORKS_QUERY = " \
query { \
  listVirtualNetworks { \
    items { \
      id \
      ipPrefix \
      communityName \
      devices { \
        items { \
          id \
          name \
          virtualIP \
          description \
        } \
      } \
    } \
  } \
}";



class OmniProxy : public QObject
{
    Q_OBJECT

public:
    explicit OmniProxy(QObject *parent = nullptr);
    virtual ~OmniProxy();
    void checkToken();

public slots:
    QList<VirtualNetwork> getVirtualNetworks();
    QVariantMap joinVirtualNetwork(QString virtualNetworkID);


signals:
    void isLogin(bool status);
    void passMsg(QString title,QString msg);
    void updateDevices(QList<Device> devices);
private:

    // Local
    QString instanceID;
    QString deviceName;
    QString description;
    QString deviceLanIp;
    QString deviceMacAddr;

    // Fetch settings from oauth.json
    QString idToken;
    QString clientId;
    QString tokenUri;
    QString clientSecret;
    QString apiEndpoint;
    QString graphqlEndpoint;

    QNetworkAccessManager *networkManager;

    void refreshToken();
    QString getInternalIP();
    QVariantMap graphqlQuery(QString query, QVariantMap variables);

    GoogleOAuth* oauth;
};

#endif // OMNIPROXY_H
