#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QIcon>
#include <QQmlContext>
#include "syslog.h"
#include "menuflow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QCoreApplication::setOrganizationName("Omniedge");
    QCoreApplication::setApplicationName("Omniedge-Desktop-Client");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("loading", true);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    QObject* root = engine.rootObjects().first();
    QObject *pObjQml = root->findChild<QObject*>("item_qml");
    MenuFlow* menuFlow = new MenuFlow(&engine);

    if(pObjQml)
    {
        QObject::connect(pObjQml,SIGNAL(logout()),menuFlow,SLOT(logout()));
        QObject::connect(pObjQml,SIGNAL(login()),menuFlow,SLOT(authenticate()));
        QObject::connect(menuFlow,SIGNAL(loginStatus(bool)),pObjQml,SIGNAL(loginStatus(bool)));
        //QObject::connect(pObjQml,SIGNAL(connectSN(QString, QString)),n2n,SLOT(startEdge(QString, QString)));
        //QObject::connect(pObjQml,SIGNAL(disconnectSN()),n2n,SLOT(stopEdge()));
        //QObject::connect(n2n,SIGNAL(configError()),pObjQml,SIGNAL(configError()));
        //QObject::connect(n2n,SIGNAL(wintapError()),pObjQml,SIGNAL(wintapError()));
        if(menuFlow->checkToken()){
            QMetaObject::invokeMethod(menuFlow, &MenuFlow::getUserInfo);
            QMetaObject::invokeMethod(menuFlow, &MenuFlow::getVirtualNetworks);
        }
        engine.rootContext()->setContextProperty("loading", false);
    }

//    if(proxy->checkToken()){
//        proxy->vns = proxy->getVirtualNetworks();
//        qDebug() << proxy->vns <<"[VirtualNetworkid]"<<proxy->vns.first().toMap().value("id").toString();
//        engine.rootContext()->setContextProperty("vns", proxy->vns);
//        proxy->joinVirtualNetwork(proxy->vns.first().toMap().value("id").toString());
//    }

    #ifdef QT_NO_DEBUGW
        syslog hlog;
        hlog.installReleaseMsgHandler();
    #endif
    return app.exec();
}
