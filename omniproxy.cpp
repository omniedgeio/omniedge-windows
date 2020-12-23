#include "omniproxy.h"


OmniProxy::OmniProxy(QObject *parent) : QObject(parent)
{
    // create network manager
    this->networkManager = new QNetworkAccessManager(this);
    this->networkManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    connect(this->networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleFinished(QNetworkReply*)));
    deviceName = QSysInfo::machineHostName();
    description = QSysInfo::prettyProductName();
    qDebug()<<"deviceName"<<deviceName<<"description"<<description;
}

OmniProxy::~OmniProxy()
{
    delete this->networkManager;
}

void OmniProxy::JoinToVirtualNetwork(){
    // URL
    QString baseUrl = "http://omniedge.io/join";
    QUrl url(baseUrl);

    QJsonObject json;
    json.insert("name", "deviceName");// required, 用户设置的设备名称
    json.insert("summary", "NULL");//optional, 设备的备注
    json.insert("description", "description");// optional，设备的描述
    json.insert("instance_id", "");// required, 设备的唯一标示
    json.insert("internal_ip", "");// optional, 设备的网卡ip

    QJsonDocument document;
    document.setObject(json);
    QByteArray dataArray = document.toJson(QJsonDocument::Compact);

    // 构造请求
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setUrl(url);

    // 发送请求
    this->networkManager->post(request, dataArray);
}

void OmniProxy::GetDeviceList(){
    // URL
    QString baseUrl = "http://omniedge.io/devices";
    QUrl url(baseUrl);

    // 构造请求
    QNetworkRequest request;
    request.setUrl(url);

    // 发送请求
    QNetworkReply *reply = this->networkManager->get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(readDeviceList()));
}

void OmniProxy::GetVirtualNetworkKey(){
    // URL
    QString baseUrl = "http://omniedge.io/user/devices/{instance_id}/virtualNetworks/{virtual_network_id}";
    QUrl url(baseUrl);

    // 构造请求
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader( QNetworkRequest::ContentTypeHeader, " Authorization: Bearer user_token" );
    // 发送请求
    QNetworkReply *reply = this->networkManager->get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(readDeviceList()));
}

void OmniProxy::readDeviceList(QNetworkReply *networkReply){
    // free later
    networkReply->deleteLater();
    networkReply->readAll();
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
            networkReply->readAll();
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
