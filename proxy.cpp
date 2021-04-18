#include "proxy.h"
#include "oauth.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

Proxy::Proxy(QObject *parent) : QObject(parent)
{
    this->networkManager = new QNetworkAccessManager(this);
    this->networkManager->setNetworkAccessible(QNetworkAccessManager::Accessible);

    this->instanceID = QSysInfo::machineUniqueId();
    this->deviceName = QSysInfo::machineHostName();
    this->description = QSysInfo::prettyProductName();

    qRegisterMetaType<UserInfo>("UserInfo");
    qRegisterMetaType<SuperNodeInfo>("SuperNodeInfo");
    qRegisterMetaType<ResponseStatus>("ResponseStatus");
    qRegisterMetaType<QList<VirtualNetwork>>("QList<VirtualNetwork>");

    // Get configurations
    QByteArray val;
    QFile file;
    file.setFileName(":/oauth.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        val = file.readAll();
        file.close();

        qDebug() << "Proxy: Opened configuration file.";

        QJsonDocument document = QJsonDocument::fromJson(val);
        QJsonObject object = document.object();
        QJsonObject settingsObject = object["dev"].toObject();

        clientId = settingsObject["client_id"].toString();
        clientSecret = settingsObject["client_secret"].toString();

        cognitoUri = settingsObject["cognito_uri"].toString();
        apiEndpoint = settingsObject["api_endpoint"].toString();
        graphqlEndpoint = settingsObject["graphql_endpoint"].toString();

        QSettings settings;
        if(settings.contains(SETTINGS_ID_TOKEN)){
            this->refreshToken();
        }
    } else {
        qDebug() << "Proxy: Cannot open configuration file.";
    }
}

bool Proxy::refreshToken()
{
    qDebug() << "Refreshing Token...";
    QSettings settings;

    if(!settings.contains(SETTINGS_REFRESH_TOKEN) ||
       !settings.contains(SETTINGS_EXPIRES_IN)){
        return false;
    }

    if(settings.value(SETTINGS_EXPIRES_IN).toInt() > QDateTime::currentSecsSinceEpoch()){
        idToken = settings.value(SETTINGS_ID_TOKEN).toString();
        accessToken = settings.value(SETTINGS_ACCESS_TOKEN).toString();
        return true;
    }

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
    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer,SIGNAL(timeout()),&connection_loop, SLOT(quit()));
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), &connection_loop, SLOT(quit()));
    reply = networkManager->post(networkRequest, params.query().toUtf8());
    timer.start(10000);
    connection_loop.exec();
    reply->deleteLater();
    timer.stop();

    ResponseStatus status = this->getResponseStatus(reply);

    if (status == ResponseStatus::Success) {
        QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
        QVariantMap responseObj = responseDoc.object().toVariantMap();
        settings.setValue(SETTINGS_ACCESS_TOKEN, responseObj["access_token"].toString());
        settings.setValue(SETTINGS_ID_TOKEN, responseObj["id_token"].toString());
        idToken = responseObj["id_token"].toString();
        accessToken = responseObj["access_token"].toString();
    }
    qDebug() << "DONE Refresh Token " << (status == ResponseStatus::Success);
    return true;
}

ResponseStatus Proxy::getResponseStatus(QNetworkReply* reply){
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

void Proxy::getUserInfo()
{
    if(!this->refreshToken()){
        emit userInfo(ResponseStatus::InvalidToken, UserInfo{});
        return;
    }
    qDebug() << "Getting User Info...";
    QNetworkReply* reply;
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(cognitoUri + "/oauth2/userInfo"));

    // Set access token
    networkRequest.setRawHeader("Authorization", "Bearer " + accessToken.toLocal8Bit());

    QEventLoop connection_loop;
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), &connection_loop, SLOT(quit()));
    reply = networkManager->get(networkRequest);
    connection_loop.exec();
    reply->deleteLater();

    UserInfo user;
    ResponseStatus status = this->getResponseStatus(reply);

    if (status == ResponseStatus::Success) {
        QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
        QVariantMap responseObj = responseDoc.object().toVariantMap();
        user.name = responseObj["name"].toString();
        user.email = responseObj["email"].toString();
        user.username = responseObj["sub"].toString();
    }

    qDebug() << "DONE Get User Info " << (status == ResponseStatus::Success);
    emit userInfo(status, user);
}

GraphqlResponse Proxy::graphqlQuery(GraphqlRequest request)
{
    QSettings settings;
    QNetworkReply* reply;
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(graphqlEndpoint));
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Set basic auth
    networkRequest.setRawHeader("Authorization", idToken.toLocal8Bit());
    QJsonObject obj;
    obj["query"] = request.query;

    if(!request.variables.isEmpty()) {
        obj["variables"] = QJsonObject::fromVariantMap(request.variables);
    }

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    QEventLoop connection_loop;
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), &connection_loop, SLOT(quit()));
    reply = networkManager->post(networkRequest, data);
    connection_loop.exec();
    reply->deleteLater();

    GraphqlResponse response;
    response.status = this->getResponseStatus(reply);

    if (response.status == ResponseStatus::Success) {
        QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
        QVariantMap responseObj = responseDoc.object().toVariantMap();
        response.data = responseObj;
    }

    return response;
}

void Proxy::getVirtualNetworks()
{
    if(!this->refreshToken()){
        emit virtualNetworks(ResponseStatus::InvalidToken, QList<VirtualNetwork>());
        return;
    }
    qDebug() << "Getting Virtual Network...";
    GraphqlRequest request;
    request.query = LIST_VIRTUAL_NETWORKS_QUERY;
    QList<VirtualNetwork> virtualNetworksList;
    //2021-04-09,before request,clear the qlist
    virtualNetworksList.clear();
    GraphqlResponse response = graphqlQuery(request);
    if (response.status == ResponseStatus::Success) {
        response.data = response.data["data"].toMap()["listVirtualNetworks"].toMap();
        if(response.data["items"].toList().length()==0)
        {
            qDebug() << "Virtual Network is Empty...";
            return;
        }
        for(QVariant virtualNetworkObj : response.data["items"].toList()){
            QVariantMap virtualNetworkObjMap = virtualNetworkObj.toMap();
            VirtualNetwork virtualNetwork;
            virtualNetwork.id = virtualNetworkObjMap["id"].toString();
            virtualNetwork.ipPrefix = virtualNetworkObjMap["ipPrefix"].toString();
            virtualNetwork.communityName = virtualNetworkObjMap["communityName"].toString();
            for(QVariant deviceObj : virtualNetworkObjMap["devices"].toMap()["items"].toList()){
                QVariantMap deviceObjMap = deviceObj.toMap();
                Device device;
                device.id = deviceObjMap["id"].toString();
                device.name = deviceObjMap["name"].toString();
                device.virtualIP = deviceObjMap["virtualIP"].toString();
                device.description = deviceObjMap["description"].toString();
                virtualNetwork.devices.push_back(device);
            }
            virtualNetworksList.push_back(virtualNetwork);
        }
    }

    std::sort(virtualNetworksList[0].devices.begin(),
          virtualNetworksList[0].devices.end(),
            [](const Device &a, const Device &b){
        QStringList x = QString(a.virtualIP).split(".");
        QStringList y = QString(b.virtualIP).split(".");
        for(int i = 0;i < x.length();i++){
            if(x[i].toInt() > y[i].toInt()){
                return false;
            }
        }
      return true;
    });
    qDebug() << "DONE Get Virtual Network " << (response.status == ResponseStatus::Success);
    emit virtualNetworks(response.status, virtualNetworksList);

}

void Proxy::joinVirtualNetwork(QString virtualNetworkID)
{
    if(!this->refreshToken()){
        emit superNodeInfo(ResponseStatus::InvalidToken, SuperNodeInfo{});
        return;
    }
    qDebug() << "Joining Virtual Network...";
    QSettings settings;
    QNetworkReply* reply;
    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(apiEndpoint + "/virtual-network/" + virtualNetworkID + "/join"));
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj.insert("name", deviceName);// required, 用户设置的设备名称
    obj.insert("userAgent", "WINDOWS");//optional, 设备的备注
    obj.insert("description", description);// optional，设备的描述
    obj.insert("publicKey","MIIBCgKCAQEApZw+zI9V6agCTczOauD5BdMlhDWdy3NtDwHF92OQHArvW/xfQJjA");//settings.value("publicKey").toString());
    obj.insert("instanceID", instanceID);// required, 设备的唯一标示
    obj.insert("virtualNetworkID", virtualNetworkID);// optional, 设备的网卡ip
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    networkRequest.setRawHeader("Authorization", idToken.toLocal8Bit());

    QEventLoop connection_loop;
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), &connection_loop, SLOT(quit()));
    reply = networkManager->post(networkRequest, data);
    connection_loop.exec();
    reply->deleteLater();

    ResponseStatus status = this->getResponseStatus(reply);
    SuperNodeInfo info;

    if (status == ResponseStatus::Success) {
        QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
        QVariantMap responseObj = responseDoc.object().toVariantMap();
        info.instanceID = responseObj["instaceID"].toString();
        info.virtualNetworkID = responseObj["virtualNetworkID"].toString();
        info.addr = responseObj["addr"].toString();
        info.virtualIP = responseObj["virtualIP"].toString();
        info.secretKey = responseObj["secretKey"].toString();
        info.communityName = responseObj["communityName"].toString();
        info.publicKey = responseObj["publicKey"].toString();
    }
    qDebug() << "DONE Joining Virtual Network " << (status == ResponseStatus::Success);
    emit superNodeInfo(status, info);
}
