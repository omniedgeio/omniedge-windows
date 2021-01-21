#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QIcon>
#include "omniproxy.h"
#include "googleoauth.h"
#include "syslog.h"
#include "n2nworkerwrapper.h"
#include "omniproxy.h"
extern "C" {
#include "n2n/n2n.h"
}
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QCoreApplication::setOrganizationName("Omniedge");
    QCoreApplication::setApplicationName("Omniedge-Desktop-Client");


    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    QObject* root = engine.rootObjects().first();
    QObject *pObjQml = root->findChild<QObject*>("item_qml");
    N2NWorkerWrapper* n2n = new N2NWorkerWrapper();
    GoogleOAuth* oauth = new GoogleOAuth();
    OmniProxy* proxy = new OmniProxy();

    if(pObjQml)
    {
        QObject::connect(pObjQml,SIGNAL(oauth()),oauth,SLOT(grant()));
        QObject::connect(pObjQml,SIGNAL(logout()),oauth,SLOT(clearToken()));
        QObject::connect(pObjQml,SIGNAL(connectSN(QString, QString)),n2n,SLOT(startEdge(QString, QString)));
        QObject::connect(pObjQml,SIGNAL(disconnectSN()),n2n,SLOT(stopEdge()));
        QObject::connect(n2n,SIGNAL(configError()),pObjQml,SIGNAL(configError()));
        QObject::connect(n2n,SIGNAL(wintapError()),pObjQml,SIGNAL(wintapError()));
        QObject::connect(proxy,SIGNAL(isLogin(bool)),pObjQml,SIGNAL(isLogin(bool)));
        //QObject::connect(proxy,SIGNAL(updateDevices(QVariantList)),pObjQml,SIGNAL(updateDevices(QVariantList)));
    }
    proxy->checkToken();
    #ifdef QT_NO_DEBUGW
        syslog hlog;
        hlog.installReleaseMsgHandler();
    #endif
    return app.exec();
}
