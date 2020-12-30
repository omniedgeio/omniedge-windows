#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QIcon>
#include "googleoauth.h"
#include "syslog.h"
#include "n2nwrapper.h"
extern "C" {
#include "n2n/n2n.h"
}
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/logo.jpeg"));//设置窗口左上角图标
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    QString community_name = "omniedge";
    QString encrypt_key    = "66YRd88kyYdhzk";

    N2NWrapper* n2nwrapper = new N2NWrapper();
    //QThread* n2nThread = new QThread();
    //n2nwrapper->moveToThread(n2nThread);
    //n2nThread->start();

   // n2nwrapper->setVirtualIp("10.254.1.8", "twofish");
    //n2nwrapper->startEdge(community_name, encrypt_key);


    GoogleOAuth hGoogleOAuth;
    QObject* root = engine.rootObjects().first();
    QObject *pObjGoogleOAuth = root->findChild<QObject*>("item_auth");

    if(pObjGoogleOAuth)
    {
        QObject::connect(
                    pObjGoogleOAuth,
                    SIGNAL(googleAuthClicked(QString, QString)),
                    n2nwrapper,
                    SLOT(startEdge(QString, QString)));        // Disabled google oauth
        // QObject::connect(pObjGoogleOAuth,SIGNAL(googleAuthClicked()),&hGoogleOAuth,SLOT(grant()));
    }

    #ifdef QT_NO_DEBUGW
        syslog hlog;
        hlog.installReleaseMsgHandler();
    #endif
    return app.exec();
}
