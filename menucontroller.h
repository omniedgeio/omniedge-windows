#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QtDebug>
#include "oauth.h"
#include "n2nworker.h"
#include "tapmanager.h"
#include "api.h"

class MenuController : public QObject
{
    Q_OBJECT
public:
    explicit MenuController(QObject *parent = nullptr);

public slots:
    void n2nError(N2NWorkerError error);
    void oAuthGranted();
    void userInfoReply(ResponseStatus status, UserInfo info);
    void superNodeInfoReply(ResponseStatus status, SuperNodeInfo info);
    void virtualNetworksReply(ResponseStatus status, QList<VirtualNetwork> virtualNetworks);

    void login();
    void logout();
    void connectSN();
    void disconnectSN();

signals:
    void n2nConnectSignal(
        QString supernode_addr,
        QString community_name,
        QString secret_key,
        QString virtual_ip
    );
    void addTapInterfaceSignal();
    void oAuthGrantSignal();
    void getUserInfoSignal();
    void getVirtualNetworksSignal();
    void joinVirtualNetworkSignal(QString virtualNetworkID);

    void n2nConnected();
    void n2nDisconnected();
    void oauthloginStatus(bool status);
    void showMessage(QString title, QString msg);
    void updateStatus(QString statusMsg);

private:
    QThread* n2nThread;
    QThread* tapThread;
    N2NWorker* n2nWorker;
    TapManager* tapManager;
    int keep_on_running = 1;

    API api;

};

#endif // MENUCONTROLLER_H
