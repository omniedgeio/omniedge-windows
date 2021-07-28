#include "api.h"
#include <QDesktopServices>

API::API(QObject *parent) : QObject(parent)
{
    this->networkManager = new QNetworkAccessManager(this);
    this->networkManager->setNetworkAccessible(QNetworkAccessManager::Accessible);

    this->baseURL = "http://192.168.0.165:1111/api";
}

void API::getAuthSession() {
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(this->baseURL + "/auth/login/session"));
    QNetworkReply* reply = this->networkManager->get(networkRequest);
    qDebug() << "API: Getting Auth Session...";

    connect(reply, &QNetworkReply::finished, [=](){
        ResponseStatus status = this->getResponseStatus(reply);

        if(status == ResponseStatus::Success){
            QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
            AuthSession auth;
            auth.uuid = responseDoc["data"].toObject()["uuid"].toString();
            auth.expiredAt = responseDoc["data"].toObject()["expired_at"].toVariant().toDateTime();
            this->userAuthSession = auth;
            QDesktopServices::openUrl(QUrl(responseDoc["data"].toObject()["auth_url"].toString()));

            connect(&authSessionWebSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), [=](QAbstractSocket::SocketError error){
                qDebug() << "API: Error when connecting to websocket" + authSessionWebSocket.errorString();
            });
            connect(&authSessionWebSocket, &QWebSocket::connected, [](){
                qDebug() << "API: Connected to websocket";
            });
            connect(&authSessionWebSocket, &QWebSocket::disconnected,[this](){
                this->userAuthSession = AuthSession{};
                qDebug() << "API: Disconencted from websocket";
            });
            connect(&authSessionWebSocket, &QWebSocket::textMessageReceived, [this](QString message){
                QJsonDocument responseDoc = QJsonDocument::fromJson(message.toUtf8());
                this->currentToken = responseDoc["token"].toString();
                qDebug() << "Token : " << this->currentToken;
                this->getUserInfo();
                this->registerDevice();
                emit token(Token{ this->currentToken });
            });
            QUrl wsUrl = QUrl(this->baseURL + "/auth/login/session/" + auth.uuid);
            wsUrl.setScheme(QString("ws"));
            authSessionWebSocket.open(wsUrl);
            qDebug() << "API: Connecting to websocket" << wsUrl;
        } else {
            emit error(status, reply->errorString());
        }
        qDebug() << "API: DONE Get Auth Session " << (status == ResponseStatus::Success);
        reply->deleteLater();
    });
}

void API::registerDevice() {
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(this->baseURL + "/devices/register"));
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    networkRequest.setRawHeader("Authorization", "Bearer " + this->currentToken.toUtf8());

    QByteArray hardwareUUID = QSysInfo::machineUniqueId();
    QString deviceName = QSysInfo::machineHostName();
    QString os = QSysInfo::prettyProductName();
    QJsonObject obj;
    obj.insert("os", QString(os));
    obj.insert("name", QString(deviceName));
    obj.insert("hardware_uuid", QString(hardwareUUID));

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    qDebug() << data;
    QNetworkReply* reply = this->networkManager->post(networkRequest, data);
    qDebug() << "API: Registering device..."<< networkRequest.url();
    connect(reply, &QNetworkReply::finished, [=](){
        ResponseStatus status = this->getResponseStatus(reply);
        if (status == ResponseStatus::Success) {
            QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
            this->currentDeviceUUID = responseDoc["data"].toObject()["uuid"].toString();
        } else {
            qDebug() << reply->errorString() << reply->readAll();
        }
        qDebug() << "API: Done register device"<< (status == ResponseStatus::Success);
        reply->deleteLater();
    });
}

void API::getUserInfo() {
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(this->baseURL + "/user/profile"));
    networkRequest.setRawHeader("Authorization", "Bearer " + this->currentToken.toUtf8());
    QNetworkReply* reply = this->networkManager->get(networkRequest);
    qDebug() << "API: Getting user info...";

    connect(reply, &QNetworkReply::finished, [=](){
        ResponseStatus status = this->getResponseStatus(reply);

        if(status == ResponseStatus::Success){
            QJsonObject data = QJsonDocument::fromJson(reply->readAll())["data"].toObject();
            this->userProfile.uuid = data["uuid"].toString();
            this->userProfile.name = data["name"].toString();
            this->userProfile.email = data["email"].toString();
            emit profile(this->userProfile);
        } else {
            emit error(status, reply->errorString());
        }

        qDebug() << "API: DONE Get User Info " << (status == ResponseStatus::Success);
        reply->deleteLater();
    });
}

void API::getVirtualNetworks(){
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(this->baseURL + "/virtual-networks"));
    networkRequest.setRawHeader("Authorization", "Bearer " + this->currentToken.toUtf8());
    QNetworkReply* reply = this->networkManager->get(networkRequest);

    qDebug() << "API: Getting virtual networks...";
    connect(reply, &QNetworkReply::finished, [=](){
        ResponseStatus status = this->getResponseStatus(reply);

        if(status == ResponseStatus::Success){
            QJsonArray data = QJsonDocument::fromJson(reply->readAll())["data"].toArray();
            for(QJsonValue res_vn :data){
                VirtualNetwork vn;
                vn.uuid = res_vn.toObject()["uuid"].toString();
                vn.name = res_vn.toObject()["name"].toString();
                vn.ipRange = res_vn.toObject()["ip_range"].toString();

                for(QJsonValue res_dev: res_vn.toObject()["devices"].toArray()){
                    Device dev;
                    dev.uuid = res_dev.toObject()["uuid"].toString();
                    dev.name = res_dev.toObject()["name"].toString();
                    dev.virtualIP = res_dev.toObject()["virtual_ip"].toString();
                    vn.devices.append(dev);
                }
                this->userVirtualNetworks.append(vn);
            }
            emit virtualNetworks(this->userVirtualNetworks);
        } else {
            emit error(status, reply->errorString());
        }

        qDebug() << "API: DONE Get virtual networks " << (status == ResponseStatus::Success);
        reply->deleteLater();
    });
}

void API::joinVirtualNetwork(QString uuid) {
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(this->baseURL + "/virtual_networks/" + uuid + "/devices/" + this->currentDeviceUUID + "/join"));
    networkRequest.setRawHeader("Authorization", "Bearer " + this->currentToken.toUtf8());
    QJsonDocument data;
    QNetworkReply* reply = this->networkManager->post(networkRequest, data.toJson());
    qDebug() << "API: Joining virtual network...";

    connect(reply, &QNetworkReply::finished, [=](){
        ResponseStatus status = this->getResponseStatus(reply);
        if(status == ResponseStatus::Success){
            QJsonObject data = QJsonDocument::fromJson(reply->readAll())["data"].toObject();
            this->userConnectInfo.communityName = data["community_name"].toString();
            this->userConnectInfo.secretKey = data["secret_key"].toString();
            this->userConnectInfo.virtualIP = data["virtual_ip"].toString();
            this->userConnectInfo.subnetMask = data["subnet_mask"].toString();
            this->userConnectInfo.host = data["server"].toObject()["host"].toString();

            emit connectInfo(this->userConnectInfo);
        } else {
            emit error(status, reply->errorString());
        }

        qDebug() << "API: DONE Get virtual networks " << (status == ResponseStatus::Success);
        reply->deleteLater();
    });
}

ResponseStatus API::getResponseStatus(QNetworkReply* reply){
    if (reply->error() != QNetworkReply::NoError) {
        return ResponseStatus::UnknownError;
    }

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (!statusCode.isValid()) {
        return ResponseStatus::UnknownError;
    }

    QString code = statusCode.toString();

    if (code == "400") {
        return ResponseStatus::AccessDenied;
    } else if (code == "500") {
        return ResponseStatus::InternalFailure;
    } else if (code == "403") {
        return ResponseStatus::InvalidToken;
    } else if (code == "401") {
        return ResponseStatus::Unauthorized;
    } else if (code == "404") {
        return ResponseStatus::MalformedQueryString;
    } else if (code == "503") {
        return ResponseStatus::ServiceUnavailable;
    } else if (code == "200") {
        return ResponseStatus::Success;
    }

    return ResponseStatus::UnknownError;
}
