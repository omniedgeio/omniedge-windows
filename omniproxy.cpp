#include "omniproxy.h"
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QThread>

OmniProxy::OmniProxy(QObject *parent) : QObject(parent)
{
    // create network manager
    this->networkManager = new QNetworkAccessManager(this);
    this->networkManager->setNetworkAccessible(QNetworkAccessManager::Accessible);

    instanceID = QSysInfo::machineUniqueId();
    deviceName = QSysInfo::machineHostName();
    description = QSysInfo::prettyProductName();

    this->getInternalIP();

    // Get client id and client secret from oauth.json
    QFile file;
    QByteArray val;
    file.setFileName(":/oauth.json");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        val = file.readAll();
        file.close();
    }
    QJsonDocument document = QJsonDocument::fromJson(val);
    QJsonObject object = document.object();

    const auto settingsObject = object["dev"].toObject();
    tokenUri = settingsObject["token_uri"].toString();
    clientId = settingsObject["client_id"].toString();
    clientSecret = settingsObject["client_secret"].toString();
    graphqlEndpoint = settingsObject["graphql_endpoint"].toString();

    this->getVirtualNetworks();
}

OmniProxy::~OmniProxy()
{
    delete this->networkManager;
}

void OmniProxy::refreshToken(){
    QSettings settings;
    QNetworkReply* reply;
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(tokenUri));
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Set basic auth
    QString concatenated = clientId + ":" + clientSecret;
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    networkRequest.setRawHeader("Authorization", "Basic " + data);

    // Set post body
    QUrlQuery params;
    params.addQueryItem("grant_type", "refresh_token");
    params.addQueryItem("client_id", clientId);
    params.addQueryItem("refresh_token", settings.value("refreshToken").toString());

    QEventLoop connection_loop;
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), &connection_loop, SLOT(quit()));
    reply = networkManager->post(networkRequest, params.query().toUtf8());
    connection_loop.exec();
    reply->deleteLater();

    QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
    QVariantMap responseObj = responseDoc.object().toVariantMap();

    // Save to registry
    settings.setValue("accessToken", responseObj["access_token"].toString());
    settings.setValue("idToken", responseObj["id_token"].toString());
    idToken = responseObj["id_token"].toString();
}

QString OmniProxy::getInternalIP()
{
    //获取本地IP片地址,
    QList<QHostAddress> addressList = QNetworkInterface::allAddresses();

    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) && !interface.flags().testFlag(QNetworkInterface::IsLoopBack))
            foreach (QNetworkAddressEntry entry, interface.addressEntries())
            {
                if ( interface.hardwareAddress() != "00:00:00:00:00:00" &&
                     entry.ip().toString().contains(".") &&
                     !interface.humanReadableName().contains("VM")){
                    deviceLanIp = entry.ip().toString();
                    deviceMacAddr = interface.hardwareAddress();
                }
            }
    }
    return nullptr;
}

QVariantMap OmniProxy::graphqlQuery(QString query, QVariantMap variables){
    QSettings settings;
    QNetworkReply* reply;
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(graphqlEndpoint));
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    this->refreshToken();
    // Set basic auth
    networkRequest.setRawHeader("Authorization", idToken.toLocal8Bit());
    QJsonObject obj;
    obj["query"] = query;
    obj["variables"] = QJsonObject::fromVariantMap(variables);
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    QEventLoop connection_loop;
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), &connection_loop, SLOT(quit()));
    reply = networkManager->post(networkRequest, data);
    connection_loop.exec();
    reply->deleteLater();

    QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
    QVariantMap responseObj = responseDoc.object().toVariantMap();
    return responseObj;
}

QVariantMap OmniProxy::joinVirtualNetwork(QString virtualNetworkID){
    QSettings settings;
    QNetworkReply* reply;
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(apiEndpoint + "/virtual-network/" + virtualNetworkID + "/join"));
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj.insert("name", deviceName);// required, 用户设置的设备名称
    obj.insert("userAgent", "WINDOWS");//optional, 设备的备注
    obj.insert("description", description);// optional，设备的描述
    obj.insert("publicKey", settings.value("publicKey").toString());
    obj.insert("instance_id", instanceID);// required, 设备的唯一标示
    obj.insert("virtualNetworkID", virtualNetworkID);// optional, 设备的网卡ip
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    // Set token
    this->refreshToken();
    networkRequest.setRawHeader("Authorization", idToken.toLocal8Bit());

    QEventLoop connection_loop;
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), &connection_loop, SLOT(quit()));
    reply = networkManager->post(networkRequest, data);
    connection_loop.exec();
    reply->deleteLater();

    QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
    QVariantMap responseObj = responseDoc.object().toVariantMap();
    return responseObj;
}

