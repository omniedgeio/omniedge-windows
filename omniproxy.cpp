#include "omniproxy.h"
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QtDebug>
#include <QQmlContext>
#include <memory>
/*
#include "openssl/bn.h"
#include "openssl/rsa.h"
#include "openssl/pem.h"
#include "openssl/bio.h"
#include "openssl/x509.h"
using std::unique_ptr;
using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;
using EVP_KEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
using BIO_FILE_ptr = std::unique_ptr<BIO, decltype(&::BIO_free)>;*/

OmniProxy::OmniProxy(QQmlApplicationEngine* engine)
{
    qmlEngine = engine;
    oauth = new GoogleOAuth();
    // create network manager
    this->networkManager = new QNetworkAccessManager(this);
    this->networkManager->setNetworkAccessible(QNetworkAccessManager::Accessible);

    instanceID = QSysInfo::machineUniqueId();
    deviceName = QSysInfo::machineHostName();
    description = QSysInfo::prettyProductName();
    QSettings settings;
    if(settings.value("publicKey").toString().isEmpty())
    {
        this->generatePubKey();
        QFile file("rsa-public-1.pem");
        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&file);
            QString pbk = file.readAll();
            settings.setValue("publicKey", pbk);
        }
    }

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
    cognitoUri = settingsObject["cognito_uri"].toString();
    clientId = settingsObject["client_id"].toString();
    clientSecret = settingsObject["client_secret"].toString();
    graphqlEndpoint = settingsObject["graphql_endpoint"].toString();
}

OmniProxy::~OmniProxy()
{
    delete this->networkManager;
}
void OmniProxy::generatePubKey()
{
    /*int rc;
    RSA_ptr rsa(RSA_new(), ::RSA_free);
    BN_ptr bn(BN_new(), ::BN_free);
    BIO_FILE_ptr pem1(BIO_new_file("rsa-public-1.pem", "w"), ::BIO_free);

    rc = BN_set_word(bn.get(), RSA_F4);
    if(rc != 1)
        qDebug()<<"generatePubKey BN_set_word err \n";

    // Generate key
    rc = RSA_generate_key_ex(rsa.get(), 2048, bn.get(), NULL);
    if(rc != 1)
        qDebug()<<"generatePubKey RSA_generate_key_ex err \n";

    // Convert RSA to PKEY
    EVP_KEY_ptr pkey(EVP_PKEY_new(), ::EVP_PKEY_free);
    rc = EVP_PKEY_set1_RSA(pkey.get(), rsa.get());
    if(rc != 1)
        qDebug()<<"generatePubKey EVP_PKEY_set1_RSA err \n";

    // Write public key in PKCS PEM
    rc = PEM_write_bio_RSAPublicKey(pem1.get(), rsa.get());
    if(rc != 1)
        qDebug()<<"generatePubKey PEM_write_bio_RSAPublicKey err \n";*/
}

bool OmniProxy::checkToken(){
    QSettings settings;
    if(!settings.value("idToken").toString().isEmpty()){
        getVirtualNetworks();
        getUserInfo();
        emit isLogin(true);
        return true;
    }
    else{
        emit isLogin(false);
        return false;
    }
}


bool OmniProxy::refreshToken(){
    QSettings settings;
    QNetworkReply* reply;
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(cognitoUri + "/oauth2/token"));
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

    if(reply->error() != QNetworkReply::NoError){
        emit isLogin(false);
        return false;
    }

    QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
    QVariantMap responseObj = responseDoc.object().toVariantMap();

    // Save to registry
    settings.setValue("accessToken", responseObj["access_token"].toString());
    settings.setValue("idToken", responseObj["id_token"].toString());
    idToken = responseObj["id_token"].toString();
    return true;
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

    if(!this->refreshToken()) {
        return QVariantMap();
    }
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
    if(!this->refreshToken()){
        return QVariantMap();
    }
    networkRequest.setRawHeader("Authorization", idToken.toLocal8Bit());

    QEventLoop connection_loop;
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), &connection_loop, SLOT(quit()));
    reply = networkManager->post(networkRequest, data);
    connection_loop.exec();
    reply->deleteLater();

    QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
    QVariantMap responseObj = responseDoc.object().toVariantMap();
    qDebug()<<"joinVirtualNetwork"<<responseObj;
    return responseObj;
}

QVariantMap OmniProxy::getUserInfo(){
    QSettings settings;
    QNetworkReply* reply;
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(cognitoUri + "/oauth2/userInfo"));

    if(!this->refreshToken()){
        return QVariantMap();
    }

    // Set access token
    networkRequest.setRawHeader("Authorization", "Bearer " + settings.value("accessToken").toString().toLocal8Bit());

    QEventLoop connection_loop;
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), &connection_loop, SLOT(quit()));
    reply = networkManager->get(networkRequest);
    connection_loop.exec();
    reply->deleteLater();

    QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
    QVariantMap responseObj = responseDoc.object().toVariantMap();

    /* Sample
        {
           "sub": "248289761001",
           "name": "Jane Doe",
           "given_name": "Jane",
           "family_name": "Doe",
           "preferred_username": "j.doe",
           "email": "janedoe@example.com"
        }
    */
    qDebug() << responseObj;
    qmlEngine->rootContext()->setContextProperty("user", responseObj);
    return responseObj;
}

void OmniProxy::getVirtualNetworks(){
    QVariantMap response = graphqlQuery(LIST_VIRTUAL_NETWORKS_QUERY, QVariantMap());
    if(response.contains("data")) {
        virtualNetworkList = response["data"].toMap()["listVirtualNetworks"].toMap()["items"].toList();
        qmlEngine->rootContext()->setContextProperty("vns", virtualNetworkList);
        //joinVirtualNetwork(vns.first().toMap().value("id").toString());
    }
}
