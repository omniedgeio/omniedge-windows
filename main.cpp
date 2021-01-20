#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QIcon>
#include "omniproxy.h"
#include "googleoauth.h"
#include "syslog.h"
#include "n2nworkerwrapper.h"
extern "C" {
#include "n2n/n2n.h"
}
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QCoreApplication::setOrganizationName("Omniedge");
    QCoreApplication::setApplicationName("Omniedge-Desktop-Client");

    QGuiApplication app(argc, argv);
    //app.setWindowIcon(QIcon(":/images/logo.jpeg"));//设置窗口左上角图标
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    QString community_name = "omniedge";
    QString encrypt_key    = "66YRd88kyYdhzk";

    N2NWorkerWrapper* n2n = new N2NWorkerWrapper();
    GoogleOAuth* oauth = new GoogleOAuth();
    OmniProxy* proxy = new OmniProxy();
    QObject* root = engine.rootObjects().first();
    QObject *pObjConnect = root->findChild<QObject*>("item_connect");

    if(pObjConnect)
    {
        QObject::connect(
                    pObjConnect,
                    SIGNAL(connectSN(QString, QString)),
                    n2n,
                    SLOT(startEdge(QString, QString)));
        QObject::connect(
                    pObjConnect,
                    SIGNAL(disconnectSN()),
                    n2n,
                    SLOT(stopEdge()));
        QObject::connect(
                    pObjConnect,
                    SIGNAL(oauth()),
                    oauth,
                    SLOT(grant()));
    }


    #ifdef QT_NO_DEBUGW
        syslog hlog;
        hlog.installReleaseMsgHandler();
    #endif
    return app.exec();
}
