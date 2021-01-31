#ifndef PROXY_H
#define PROXY_H

#include <QObject>
#include <QtNetwork>

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

struct UserInfo {
    QString name;
    QString email;
    QString username; // userID
};

struct SuperNodeInfo {
    QString instanceID;
    QString virtualNetworkID;
    QString addr;
    QString virtualIP;
    QString secretKey;
    QString communityName;
    QString publicKey;
};

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

struct GraphqlRequest {
    QString query;
    QVariantMap variables;
};

struct GraphqlResponse {
    ResponseStatus status;
    QVariantMap data;
};

class Proxy : public QObject
{
    Q_OBJECT
    Q_ENUM(ResponseStatus)
public:
    explicit Proxy(QObject *parent = nullptr);

public slots:
    void getUserInfo();
    bool refreshToken();
    void getVirtualNetworks();
    void joinVirtualNetwork(QString virtualNetworkID);

signals:
    void userInfo(ResponseStatus status, UserInfo info);
    void superNodeInfo(ResponseStatus status, SuperNodeInfo info);
    void virtualNetworks(ResponseStatus status, QList<VirtualNetwork> virtualNetworks);

private:
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
    QString getInternalIP();
    GraphqlResponse graphqlQuery(GraphqlRequest request);
    ResponseStatus getResponseStatus(QNetworkReply* reply);
};

#endif // PROXY_H
