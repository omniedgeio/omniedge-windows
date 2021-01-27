#ifndef MENUFLOW_H
#define MENUFLOW_H

#include <QObject>
#include <QSettings>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QThread>
#include "oauth.h"
#include "omniproxy.h"
#include "n2nworkerwrapper.h"

class MenuFlow : public QObject
{
    Q_OBJECT
public:
    explicit MenuFlow(QQmlApplicationEngine* engine);
    virtual ~MenuFlow();
    bool checkToken();
    void getUserInfo();
    void getVirtualNetworks();
    void joinVirtualNetwork();

signals:
    void showMessage(QString title, QString message);
    void loginStatus(bool status);


public slots:
    void authenticate();
    void logout();
    void connectSN();
    void disconnectSN();

private:
    OmniProxy* proxy;
    OAuth* oauth;
    N2NWorkerWrapper* n2n;
    QQmlApplicationEngine* qmlEngine;
    QVariantList virtualNetworks;
    QThread m_menuflow;
    QString virtualNetworksID;
    struct Credential{
        QString virtualIP;
        QString superNodeAddr;
        QString communityName;
        QString communityPassword;
    } credential;
};

#endif // MENUFLOW_H
