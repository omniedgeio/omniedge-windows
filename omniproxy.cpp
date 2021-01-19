#include "omniproxy.h"
#include <memory>
using std::unique_ptr;
#include "openssl/bn.h"
#include "openssl/rsa.h"
#include "openssl/pem.h"
#include "openssl/bio.h"
#include "openssl/x509.h"
#include <cassert>
#define ASSERT assert

using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;
using EVP_KEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
using BIO_FILE_ptr = std::unique_ptr<BIO, decltype(&::BIO_free)>;
OmniProxy::OmniProxy(QObject *parent) : QObject(parent)
{
    // create network manager
    this->networkManager = new QNetworkAccessManager(this);
    this->networkManager->setNetworkAccessible(QNetworkAccessManager::Accessible);

    connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleFinished(QNetworkReply*)));

    instanceID = QSysInfo::machineUniqueId();
    deviceName = QSysInfo::machineHostName();
    description = QSysInfo::prettyProductName();

    reg = new QSettings("HKEY_CURRENT_USER\\Software\\OmniEdge",QSettings::NativeFormat);
    if (reg->value("id_token",QVariant()).toString()=="")
    {
       emit passMsg("OmniEdge","Please Login");
    }
    else
    {
       //qDebug()<<reg->value("id_token",QVariant()).toString();
       emit passMsg("OmniEdge",reg->value("id_token",QVariant()).toString());
    }

    this->generatePubKey();
}

OmniProxy::~OmniProxy()
{
    delete this->networkManager;
    delete reg;
}

void OmniProxy::generatePubKey()
{
    int rc;

        RSA_ptr rsa(RSA_new(), ::RSA_free);
        BN_ptr bn(BN_new(), ::BN_free);

        BIO_FILE_ptr pem1(BIO_new_file("rsa-public-1.pem", "w"), ::BIO_free);
        BIO_FILE_ptr pem2(BIO_new_file("rsa-public-2.pem", "w"), ::BIO_free);
        BIO_FILE_ptr pem3(BIO_new_file("rsa-private-1.pem", "w"), ::BIO_free);
        BIO_FILE_ptr pem4(BIO_new_file("rsa-private-2.pem", "w"), ::BIO_free);
        BIO_FILE_ptr pem5(BIO_new_file("rsa-private-3.pem", "w"), ::BIO_free);
        BIO_FILE_ptr der1(BIO_new_file("rsa-public.der", "w"), ::BIO_free);
        BIO_FILE_ptr der2(BIO_new_file("rsa-private.der", "w"), ::BIO_free);

        rc = BN_set_word(bn.get(), RSA_F4);
        ASSERT(rc == 1);

        // Generate key
        rc = RSA_generate_key_ex(rsa.get(), 2048, bn.get(), NULL);
        ASSERT(rc == 1);

        // Convert RSA to PKEY
        EVP_KEY_ptr pkey(EVP_PKEY_new(), ::EVP_PKEY_free);
        rc = EVP_PKEY_set1_RSA(pkey.get(), rsa.get());
        ASSERT(rc == 1);

        //////////

        // Write public key in ASN.1/DER
        rc = i2d_RSAPublicKey_bio(der1.get(), rsa.get());
        ASSERT(rc == 1);

        // Write public key in PKCS PEM
        rc = PEM_write_bio_RSAPublicKey(pem1.get(), rsa.get());
        ASSERT(rc == 1);

        // Write public key in Traditional PEM
        rc = PEM_write_bio_PUBKEY(pem2.get(), pkey.get());
        ASSERT(rc == 1);

        //////////

        // Write private key in ASN.1/DER
        rc = i2d_RSAPrivateKey_bio(der2.get(), rsa.get());
        ASSERT(rc == 1);

        // Write private key in PKCS PEM.
        rc = PEM_write_bio_PrivateKey(pem3.get(), pkey.get(), NULL, NULL, 0, NULL, NULL);
        ASSERT(rc == 1);

        // Write private key in PKCS PEM
        rc = PEM_write_bio_PKCS8PrivateKey(pem4.get(), pkey.get(), NULL, NULL, 0, NULL, NULL);
        ASSERT(rc == 1);

        // Write private key in Traditional PEM
        rc = PEM_write_bio_RSAPrivateKey(pem5.get(), rsa.get(), NULL, NULL, 0, NULL, NULL);
        ASSERT(rc == 1);
}
//QString OmniProxy::getInternalIP()
//{
//    //获取本地IP片地址,
//    QList<QHostAddress> addressList = QNetworkInterface::allAddresses();

//    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
//    {
//        if (interface.flags().testFlag(QNetworkInterface::IsUp) && !interface.flags().testFlag(QNetworkInterface::IsLoopBack))
//            foreach (QNetworkAddressEntry entry, interface.addressEntries())
//            {
//                if ( interface.hardwareAddress() != "00:00:00:00:00:00" &&
//                     entry.ip().toString().contains(".") &&
//                     !interface.humanReadableName().contains("VM")){
//                    deviceLanIp = entry.ip().toString();
//                    deviceMacAddr = interface.hardwareAddress();
//                }
//            }
//    }
//    return nullptr;
//}

void OmniProxy::setToken(QString token)
{
    this->token = token;
}

void OmniProxy::joinVirtualNetwork(){

    QUrl url(apiUrl + "/join");

    QJsonObject json;
    json.insert("name", deviceName);// required, 用户设置的设备名称
    json.insert("summary", "OMNIEDGE WINDOWS CLIENT");//optional, 设备的备注
    json.insert("description", description);// optional，设备的描述
    json.insert("instance_id", instanceID);// required, 设备的唯一标示
    json.insert("internal_ip", deviceLanIp);// optional, 设备的网卡ip

    QJsonDocument document;
    document.setObject(json);
    QByteArray dataArray = document.toJson(QJsonDocument::Compact);

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader( QNetworkRequest::ContentTypeHeader, " Authorization: Bearer " + token);

    QNetworkReply* reply = this->networkManager->post(request, dataArray);
    endPoints[reply] = EndPoint::PostJoinNetwork;
}

void OmniProxy::getDeviceList(){
    QUrl url(apiUrl + "/devices");

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader( QNetworkRequest::ContentTypeHeader, " Authorization: Bearer " + token);

    QNetworkReply* reply = this->networkManager->get(request);
    endPoints[reply] = EndPoint::GetDevicesList;
}

void OmniProxy::getVirtualNetworkKey(){

    QUrl url(apiUrl + "/user/devices/" + instanceID + "/virtualNetworks/{virtual_network_id}");

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader( QNetworkRequest::ContentTypeHeader, " Authorization: Bearer " + token);

    QNetworkReply* reply = this->networkManager->get(request);
    endPoints[reply] = EndPoint::GetVirtualNetworkSecret;
}

void OmniProxy::handleReadyRead(QNetworkReply *networkReply){
    // free later
    networkReply->deleteLater();
    qDebug() << networkReply->readAll();
    networkReply->manager()->deleteLater();
}

void OmniProxy::handleFinished(QNetworkReply *networkReply){
    // free later
    networkReply->deleteLater();

    // no error in request
    if (networkReply->error() == QNetworkReply::NoError)
    {
        // get HTTP status code
        qint32 httpStatusCode = networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        // 200
        if (httpStatusCode >= 200 && httpStatusCode < 300) // OK
        {
            QString reply = networkReply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply.toUtf8());
            QVariantMap replyMap = jsonResponse.object().toVariantMap();

            qDebug() << reply;

            switch(endPoints[networkReply]){
                case EndPoint::PostJoinNetwork:
                    virtualIP = replyMap["virtual_ip"].toString();
                    encryptMethod = replyMap["encrypt_method"].toString();
                    break;
                case EndPoint::GetVirtualNetworkSecret:
                    secretKey = replyMap["secret_key"].toString();
                    communityName = replyMap["community_name"].toString();
                    break;
                case EndPoint::GetDevicesList:
                    QVariant obj;
                    foreach(obj, jsonResponse.array().toVariantList()){
                        QVariantMap deviceMap = obj.toMap();
                        Device device;
                        device.name = deviceMap["name"].toString();
                        device.summary = deviceMap["summary"].toString();
                        device.virtualIP = deviceMap["virtual_ip"].toString();
                        device.instanceID = deviceMap["instace_id"].toString();
                        device.description = deviceMap["description"].toString();
                        devices.append(device);
                    }
                    break;
            }

        }
        else if (httpStatusCode >= 300 && httpStatusCode < 400) // 300 Redirect
        {
            // Get new url, can be relative
            QUrl relativeUrl = networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

            // url can be relative, we use the previous url to resolve it
            QUrl redirectUrl = networkReply->url().resolved(relativeUrl);

            // redirect to new url
            networkReply->manager()->get(QNetworkRequest(redirectUrl));

            // maintain manager
            return;
        }
        else if (httpStatusCode >= 400 && httpStatusCode < 500) // 400 Error
        {
            qDebug() << httpStatusCode << " Error!";
        }
        else if (httpStatusCode >= 500 && httpStatusCode < 600) // 500 Internal Server Error
        {
            qDebug() << httpStatusCode << " Error!";
        }
        else
        {
            qDebug() << "Status code invalid! " << httpStatusCode;
        }
    }
    else
    {
        qDebug() << "Error: " << networkReply->errorString();
    }

    networkReply->manager()->deleteLater();
}
