#include "menucontroller.h"

MenuController::MenuController(QObject *parent) : QObject(parent)
{
    this->n2nThread = new QThread(this);
    this->n2nWorker = new N2NWorker(&this->keep_on_running);
    connect(this, &MenuController::n2nConnectSignal, this->n2nWorker, &N2NWorker::connect);
    connect(this->n2nWorker, &N2NWorker::error, this, &MenuController::n2nError);
    connect(this->n2nWorker, &N2NWorker::connected, this, &MenuController::n2nConnected);
    connect(this->n2nWorker, &N2NWorker::disconnected, this, &MenuController::n2nDisconnected);
    this->n2nWorker->moveToThread(this->n2nThread);
    this->n2nThread->start();

    this->tapThread = new QThread(this);
    this->tapManager = new TapManager();
    connect(this, &MenuController::addTapInterfaceSignal, this->tapManager, &TapManager::addTap);
    this->tapManager->moveToThread(this->tapThread);
    this->tapThread->start();

    connect(&this->api, &API::token, [=](Token token){
        QSettings settings;
        settings.setValue(SETTINGS_ID_TOKEN, token.token);
    });
    connect(&this->api, &API::profile, [=](Profile profile){
        userEmail = profile.email;
        emit updateEmail(userEmail);
    });
    connect(&this->api, &API::virtualNetworks, [=](QList<VirtualNetwork> virtualNetwork){
        this->virtualNetworks = virtualNetwork;
        emit oauthloginStatus(true);
        emit updateStatus(tr("Status: Logged in..."));
    });
    connect(&this->api, &API::connectInfo, [=](ConnectInfo connectInfo){
         emit n2nConnectSignal(
                 connectInfo.host,connectInfo.communityName,connectInfo.secretKey,connectInfo.subnetMask,connectInfo.virtualIP);
         emit updateStatus(tr("Status: Connecting..."));
         myVirtualIP = connectInfo.virtualIP;
    });
    connect(&this->api, &API::error, [=](ResponseStatus status, QString errorString){
        qDebug()<<"[ERROR] "<<errorString;
    });
}

void MenuController::login(){
    this->api.getAuthSession();
    emit updateStatus(tr("Status:Logged out"));
}

void MenuController::logout(){
    if (keep_on_running) {
        keep_on_running = 0;
        emit updateStatus(tr("Status: Disconnecting..."));
    }
    QSettings settings;
    settings.clear();
    emit oauthloginStatus(false);
}
void MenuController::joinVirtualNetworkManual(QString uuid)
{
    this->api.joinVirtualNetwork(uuid);
}
void MenuController::getUserIdToken(QString token){
    this->api.currentToken = token;
    this->api.getUserInfo();
    //this->api.getRefreshToken();
    this->api.registerDevice();
    this->api.getVirtualNetworks();
}

void MenuController::disconnectSN(){
    keep_on_running = 0;
    emit updateStatus(tr("Status: Disconnecting..."));
}

void MenuController::n2nError(N2NWorkerError error)
{
    if (error == N2NWorkerError::WINTAP){
        emit addTapInterfaceSignal();
        emit showMessage(tr("Driver error"), tr("Installing tap adapter... Please retry after 15 seconds."));
    }
}


void MenuController::userInfoReply(
        ResponseStatus status,
        UserInfo info
        ){
    if(status == ResponseStatus::Success){
        //this->userInfo = info;
        //emit getVirtualNetworksSignal();
    } else {
        emit oauthloginStatus(false);
    }
}

void MenuController::virtualNetworksReply(
        ResponseStatus status,
        QList<VirtualNetwork> virtualNetworks){
    if(status == ResponseStatus::Success){
        //this->virtualNetworks = virtualNetworks;
        //emit joinVirtualNetworkSignal(virtualNetworks.at(0).id);
    } else {
        emit oauthloginStatus(false);
    }
}

//void MenuController::superNodeInfoReply(ResponseStatus status, SuperNodeInfo info)
//{
//    if(status == ResponseStatus::Success){
//        //this->supernodes[info.virtualNetworkID] = info;
//    }
//    emit oauthloginStatus(true);
//}


