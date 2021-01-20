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
    ~OmniProxy();

public slots:
    QList<VirtualNetwork> getVirtualNetworks();
    QVariantMap joinVirtualNetwork(QString virtualNetworkID);

signals:

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

};

#endif // OMNIPROXY_H
