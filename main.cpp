#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QIcon>
#include "googleoauth.h"
#include "syslog.h"
#include "n2nworkerwrapper.h"
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

    N2NWorkerWrapper* n2n = new N2NWorkerWrapper();

    GoogleOAuth hGoogleOAuth;
    QObject* root = engine.rootObjects().first();
    QObject *pObjGoogleOAuth = root->findChild<QObject*>("item_auth");

    if(pObjGoogleOAuth)
    {
        QObject::connect(
                    pObjGoogleOAuth,
                    SIGNAL(googleAuthClicked(QString, QString)),
                    n2n,
                    SLOT(startEdge(QString, QString)));        // Disabled google oauth
        // QObject::connect(pObjGoogleOAuth,SIGNAL(googleAuthClicked()),&hGoogleOAuth,SLOT(grant()));
    }

    QObject *pObjConnectForm = root->findChild<QObject*>("item_connect");
    if(pObjConnectForm)
    {
        QObject::connect(
                    pObjConnectForm,
                    SIGNAL(connetToSN()),
                    n2n,
                    SLOT(stopEdge()));
    }

    #ifdef QT_NO_DEBUGW
        syslog hlog;
        hlog.installReleaseMsgHandler();
    #endif
    return app.exec();
}
