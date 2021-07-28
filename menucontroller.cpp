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
    });
    connect(&this->api, &API::profile, [=](Profile profile){
    });
    connect(&this->api, &API::virtualNetworks, [=](QList<VirtualNetwork> virtualNetwork){
    });
    connect(&this->api, &API::connectInfo, [=](ConnectInfo connectInfo){
    });
    connect(&this->api, &API::error, [=](ResponseStatus status, QString errorString){
    });
}

void MenuController::login(){
    this->api.getAuthSession();
    emit updateStatus("Status: Logging in...");
}

void MenuController::logout(){
    if (keep_on_running) {
        keep_on_running = 0;
        emit updateStatus("Status: Disconnecting...");
    }
    QSettings settings;
    settings.clear();
    emit oauthloginStatus(false);
}

void MenuController::connectSN(){
    SuperNodeInfo info = this->supernodes[this->virtualNetworks.at(0).id];
    emit n2nConnectSignal(
            info.addr,
            info.communityName,
            info.secretKey,
            info.virtualIP
         );
    emit updateStatus("Status: Connecting...");
}

void MenuController::disconnectSN(){
    keep_on_running = 0;
    emit updateStatus("Status: Disconnecting...");
}

void MenuController::n2nError(N2NWorkerError error)
{
    if (error == N2NWorkerError::WINTAP){
        emit addTapInterfaceSignal();
        emit showMessage("Driver error", "Installing tap adapter... Please retry after 15 seconds.");
    }
}

void MenuController::oAuthGranted()
{
    emit getUserInfoSignal();
}

void MenuController::userInfoReply(
        ResponseStatus status,
        UserInfo info
        ){
    if(status == ResponseStatus::Success){
        this->userInfo = info;
        emit getVirtualNetworksSignal();
    } else {
        emit oauthloginStatus(false);
    }
}

void MenuController::virtualNetworksReply(
        ResponseStatus status,
        QList<VirtualNetwork> virtualNetworks){
    if(status == ResponseStatus::Success){
        this->virtualNetworks = virtualNetworks;
        emit joinVirtualNetworkSignal(virtualNetworks.at(0).id);
    } else {
        emit oauthloginStatus(false);
    }
}

void MenuController::superNodeInfoReply(ResponseStatus status, SuperNodeInfo info)
{
    if(status == ResponseStatus::Success){
        this->supernodes[info.virtualNetworkID] = info;
    }
    emit oauthloginStatus(true);
}


