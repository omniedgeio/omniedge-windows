#ifndef MENUFLOW_H
#define MENUFLOW_H

#include <QObject>
#include <QSettings>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "oauth.h"
#include "omniproxy.h"
#include "n2nworkerwrapper.h"

class MenuFlow : public QObject
{
    Q_OBJECT
public:
    explicit MenuFlow(QQmlApplicationEngine* engine);
    bool checkToken();
    void getUserInfo();
    void getVirtualNetworks();

signals:
    void showMessage(QString title, QString message);
    void loginStatus(bool status);


public slots:
    void authenticate();
    void logout();
    void connectSN();
    void disconnectSN();
    //void joinVirtualNetwork(QString virtualNetworkID);

private:
    OmniProxy* proxy;
    OAuth* oauth;
    N2NWorkerWrapper* n2n;
    QQmlApplicationEngine* qmlEngine;
    QVariantList virtualNetworks;

};

#endif // MENUFLOW_H
