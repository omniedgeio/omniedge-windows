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
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QtDebug>
#include <QQmlContext>
#include <memory>

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

enum class ResponseStatus {
  UnknownError,
  AccessDenied, // 400
  Unauthorized, // 401
  InternalFailure, // 500
  InvalidToken, // 403
  MalformedQueryString, // 404
  ServiceUnavailable, // 503
  Success, // 200
};

struct Response {
  QVariantMap data;
  ResponseStatus status;
};

class OmniProxy : public QObject
{
    Q_OBJECT

public:
    explicit OmniProxy();
    virtual ~OmniProxy();

    QVariantList virtualNetworkList;
    QVariantMap userInfo;
    Response refreshToken();
    Response getUserInfo();
    Response getVirtualNetworks();
    Response joinVirtualNetwork(QString virtualNetworkID);
    Response transformHttpReplyToResponse(QNetworkReply* reply);


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
    QString accessToken;
    QString clientId;
    QString cognitoUri;
    QString clientSecret;
    QString apiEndpoint;
    QString graphqlEndpoint;

    QNetworkAccessManager *networkManager;

    void generatePubKey();
    QString getInternalIP();
    Response graphqlQuery(QString query, QVariantMap variables);
};

#endif // OMNIPROXY_H
