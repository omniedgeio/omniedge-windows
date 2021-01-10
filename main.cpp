#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include "googleoauth.h"
#include "syslog.h"
#include "omniproxy.h"
#include "myoauthhttpserverreplyhandler.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/logo.jpeg"));//设置窗口左上角图标
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    //OmniProxy hOmniProxy;
    //hOmniProxy.getDeviceList();
    //hOmniProxy.joinToVirtualNetwork();

    GoogleOAuth hGoogleOAuth(nullptr);
    QObject* root = engine.rootObjects().first();
    QObject *pObjGoogleOAuth = root->findChild<QObject*>("item_auth");
    if(pObjGoogleOAuth)
    {
        // Disabled google oauth
         QObject::connect(pObjGoogleOAuth,SIGNAL(googleOAuth()),&hGoogleOAuth,SLOT(grant()));
    }

    #ifdef QT_NO_DEBUGW
        syslog hlog;
        hlog.installReleaseMsgHandler();
    #endif
    return app.exec();
}
