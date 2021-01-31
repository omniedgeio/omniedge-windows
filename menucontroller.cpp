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

    this->oauthThread = new QThread(this);
    this->oauth = new OAuth();
    connect(this, &MenuController::oAuthGrantSignal, this->oauth, &OAuth::grant);
    connect(this->oauth, &OAuth::granted, this, &MenuController::oAuthGranted);
    this->oauth->moveToThread(this->oauthThread);
    this->oauthThread->start();

    this->proxyThread = new QThread(this);
    this->proxy = new Proxy();
    connect(this, &MenuController::getUserInfoSignal, this->proxy, &Proxy::getUserInfo, Qt::QueuedConnection);
    connect(this, &MenuController::getVirtualNetworksSignal, this->proxy, &Proxy::getVirtualNetworks, Qt::QueuedConnection);
    connect(this, &MenuController::joinVirtualNetworkSignal, this->proxy, &Proxy::joinVirtualNetwork, Qt::QueuedConnection);
    connect(this->proxy, &Proxy::userInfo, this, &MenuController::userInfoReply);
    connect(this->proxy, &Proxy::superNodeInfo, this, &MenuController::superNodeInfoReply);
    connect(this->proxy, &Proxy::virtualNetworks, this, &MenuController::virtualNetworksReply);
    this->proxy->moveToThread(this->proxyThread);
    this->proxyThread->start();

    qRegisterMetaType<UserInfo>("UserInfo");
    qRegisterMetaType<SuperNodeInfo>("SuperNodeInfo");
    qRegisterMetaType<ResponseStatus>("ResponseStatus");
    qRegisterMetaType<QList<VirtualNetwork>>("QList<VirtualNetwork>");
}

void MenuController::login(){
    emit oAuthGrantSignal();
}

void MenuController::logout(){
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
}

void MenuController::disconnectSN(){
    keep_on_running = 0;
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
    emit showMessage("Login successfully", "Connect without concern");
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


