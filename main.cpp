#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include "googleoauth.h"
#include "syslog.h"
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
    QString device_name="laptop";
    QString community_name="omniedge";
    QString encrypt_key="66YRd88kyYdhzk";
    QString device_mac="fe80::fce2:8d0a:921:47df";
    QString local_ip_address="10.254.1.2";
    QString supernode_ip_address_port="52.80.139.238:7787";
    int keep_on_running=1;
    quick_edge_init(device_name.toLatin1().data(), community_name.toLatin1().data(),
                    encrypt_key.toLatin1().data(),device_mac.toLatin1().data(),
                    local_ip_address.toLatin1().data(),supernode_ip_address_port.toLatin1().data(),
                    &keep_on_running);
    GoogleOAuth hGoogleOAuth;
    QObject* root = engine.rootObjects().first();
    QObject *pObjGoogleOAuth = root->findChild<QObject*>("item_auth");
    if(pObjGoogleOAuth)
    {
        // Disabled google oauth
        // QObject::connect(pObjGoogleOAuth,SIGNAL(googleAuthClicked()),&hGoogleOAuth,SLOT(grant()));
    }

    #ifdef QT_NO_DEBUGW
        syslog hlog;
        hlog.installReleaseMsgHandler();
    #endif
    return app.exec();
}
