#ifndef API_H
#define API_H
//#include "proxy.h"
#include <QObject>
#include <QDateTime>
#include <QtNetwork>
#include <QtWebSockets/QtWebSockets>

//enum class ResponseStatus {
//  UnknownError,
//  Unauthorized, // 401
//  InternalServerError, // 500
//  NotFound, // 404
//  Success, // 200
//};

struct AuthSession {
    QString uuid;
    QDateTime expiredAt;
};

struct Token {
    QString token;
};

struct ConnectInfo {
    QString communityName;
    QString secretKey;
    QString virtualIP;
    QString subnetMask;
    QString host;
};

struct Profile {
    QString uuid;
    QString name;
    QString email;
};

struct Device {
    QString uuid;
    QString name;
    QString virtualIP;
    bool operator()(const Device & a, const Device & b) const{
        return a.virtualIP < b.virtualIP;
    }
};

struct VirtualNetwork {
    QString uuid;
    QString name;
    QString ipRange;
    QList<Device> devices;
};
struct UserInfo {
    QString name;
    QString email;
    QString username; // userID
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
class API : public QObject
{
    Q_OBJECT
public:
    explicit API(QObject *parent = nullptr);
    QString currentToken;
public slots:
    void getAuthSession(); // 开始登入， 会重定向至网页
    void registerDevice(); // 登入完成后，会自动 register device
    void getUserInfo();    // 登入成功后，会自动 get user info
    void getVirtualNetworks(); // 登入成功后，会自动 get virtual networks
    void joinVirtualNetwork(QString virtualNetworkUUID);

signals:
    void authSession(ResponseStatus status, AuthSession auth);
    void token(Token token);
    void deviceInfo();
    void profile(Profile profile);
    void virtualNetworks(QList<VirtualNetwork> virtualNetworks);
    void connectInfo(ConnectInfo info);
    void error(ResponseStatus status, QString errorString);

private:
    QString baseURL;
    QWebSocket authSessionWebSocket;


    QString currentDeviceUUID;

    Profile userProfile;
    AuthSession userAuthSession;
    ConnectInfo userConnectInfo;
    QList<VirtualNetwork> userVirtualNetworks;

    QNetworkAccessManager *networkManager;

    ResponseStatus getResponseStatus(QNetworkReply*);



};

#endif // API_H
