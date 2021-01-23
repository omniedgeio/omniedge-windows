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
#include <QQmlApplicationEngine>
#include "googleoauth.h"

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
    explicit OmniProxy(QQmlApplicationEngine *engine);
    virtual ~OmniProxy();
    bool checkToken();

    QVariantList virtualNetworkList;
    QVariantMap userInfo;

public slots:
    void getVirtualNetworks();
    QVariantMap joinVirtualNetwork(QString virtualNetworkID);
    QVariantMap getUserInfo();

signals:
    void isLogin(bool status);
    void passMsg(QString title,QString msg);
private:

    QQmlApplicationEngine* qmlEngine;
    // Local
    QString instanceID;
    QString deviceName;
    QString description;
    QString deviceLanIp;
    QString deviceMacAddr;

    // Fetch settings from oauth.json
    QString idToken;
    QString clientId;
    QString cognitoUri;
    QString clientSecret;
    QString apiEndpoint;
    QString graphqlEndpoint;

    QNetworkAccessManager *networkManager;

    bool refreshToken();
    void generatePubKey();
    QString getInternalIP();
    QVariantMap graphqlQuery(QString query, QVariantMap variables);

    GoogleOAuth* oauth;
};

#endif // OMNIPROXY_H
