#include "omniproxy.h"


OmniProxy::OmniProxy(QObject *parent) : QObject(parent)
{
    // create network manager
    this->networkManager = new QNetworkAccessManager(this);
    this->networkManager->setNetworkAccessible(QNetworkAccessManager::Accessible);

    connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleFinished(QNetworkReply*)));

    deviceName = QSysInfo::machineHostName();
    description = QSysInfo::prettyProductName();
    qDebug()<<"[deviceName:]"<<deviceName<<"[description:]"<<description;
}

OmniProxy::~OmniProxy()
{
    delete this->networkManager;
}

QString OmniProxy::getInternalIP()
{
    //获取本地IP片地址,
    QList<QHostAddress> addList = QNetworkInterface::allAddresses();

    foreach(QHostAddress address,addList)
    {
        //排除IPV6，排除回环地址
        if(address.protocol() == QAbstractSocket::IPv4Protocol
                && address != QHostAddress(QHostAddress::LocalHost))
        {
            //输出，转换为字符串格式
            qDebug() << address.toString();
            return address.toString();
        }
    }
    return nullptr;
}
void OmniProxy::joinToVirtualNetwork(){

    QString baseUrl = "https://ae4ffa2f-d6a5-42bc-878a-05ab291b9ab1.mock.pstmn.io/join";
    QUrl url(baseUrl);

    QJsonObject json;
    json.insert("name", "deviceName");// required, 用户设置的设备名称
    json.insert("summary", "NULL");//optional, 设备的备注
    json.insert("description", "description");// optional，设备的描述
    json.insert("instance_id", "NULL");// required, 设备的唯一标示
    json.insert("internal_ip", "0.0.0.0");// optional, 设备的网卡ip

    QJsonDocument document;
    document.setObject(json);
    QByteArray dataArray = document.toJson(QJsonDocument::Compact);

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setUrl(url);

    this->networkManager->post(request, dataArray);
}

void OmniProxy::getDeviceList(){

    QString baseUrl = "https://ae4ffa2f-d6a5-42bc-878a-05ab291b9ab1.mock.pstmn.io/devices";
    QUrl url(baseUrl);

    QNetworkRequest request;
    request.setUrl(url);

    this->networkManager->get(request);
}

void OmniProxy::getVirtualNetworkKey(){

    QString baseUrl = "http://omniedge.io/user/devices/{instance_id}/virtualNetworks/{virtual_network_id}";
    QUrl url(baseUrl);

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader( QNetworkRequest::ContentTypeHeader, " Authorization: Bearer user_token" );

    this->networkManager->get(request);
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
            qDebug() << reply;
            // get the root object
//            QJsonDocument itemDoc = QJsonDocument::fromJson(reply.toUtf8());
//            QJsonObject rootObject = itemDoc.object();

//            // parse virtual_ip & encryptMethod from json
//            vNetReply.tapIP = rootObject.value("virtual_ip").toString();
//            vNetReply.encryptMethod = rootObject.value("encrypt_method").toString();
//            qDebug() << "tapIP:" << vNetReply.tapIP;

            QJsonParseError jsonError;
            QJsonDocument devicesJson = QJsonDocument::fromJson(reply.toUtf8(),&jsonError);

            if (jsonError.error != QJsonParseError::NoError){
                qDebug() << jsonError.errorString();
            }
            QList<QVariant> list = devicesJson.toVariant().toList();
            for(int i=0;i<list.count();i++){
                QMap<QString, QVariant> map = list[i].toMap();
                if(map.contains("twofish")){
                    qDebug() <<i << "joinToVirtualNetwork" <<"tapIP:" << map["virtual_ip"].toString();
                }
                else{
                    qDebug() <<i<<"getDeviceList"<< map["virtual_ip"].toString();
                }
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
        qDebug() << "errorString: " << networkReply->errorString();
    }

    networkReply->manager()->deleteLater();
}
