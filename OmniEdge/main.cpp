#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include "googleoauth.h"
#include "edge.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/logo.jpeg"));//设置窗口左上角图标
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    GoogleOAuth hGoogleOAuth;
    QObject* root = engine.rootObjects().first();
    QObject *pObjGoogleOAuth = root->findChild<QObject*>("item_auth");
    if(pObjGoogleOAuth)
    {
        QObject::connect(pObjGoogleOAuth,SIGNAL(googleAuthClicked()),&hGoogleOAuth,SLOT(grant()));
    }
    Edge hEdge;
    QObject *pObjConnect = root->findChild<QObject*>("item_connect");
    if(pObjConnect)
    {
        QObject::connect(pObjConnect,SIGNAL(connetToSN(QString,QString,QString,QString,QString)),&hEdge,SLOT(connectSuperNode(QString,QString,QString,QString,QString)));
        QObject::connect(&hEdge,SIGNAL(replyConnectStatus(int)),pObjConnect,SIGNAL(statusOfConnect(int)));

    }
    // Test edge.exe
    /*Edge* edge = new Edge();
    edge->connectSuperNode(
                "52.80.139.238",
                "7787",
                "omniedge",
                "66YRd88kyYdhzk",
                "192.168.8.8"
                );*/

    return app.exec();
}
