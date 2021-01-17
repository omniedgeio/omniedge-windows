#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QIcon>
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

    QGuiApplication app(argc, argv);
    //app.setWindowIcon(QIcon(":/images/logo.jpeg"));//设置窗口左上角图标
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QString idToken = "eyJraWQiOiJ5cHhPQ3pCMTVndTkweVB1WXpBUzZuRUJNK0hqQnBpWWFpZWxGSVdGeTRnPSIsImFsZyI6IlJTMjU2In0.eyJhdF9oYXNoI\
            joidDdjbFZ6dXNzUzV1dXpyd3hQcDJOZyIsInN1YiI6ImEyYjE5NjQ5LTRiZDQtNDMyZS05MDE3LWY3N2I1YWI2YWY5ZiIsImNvZ25pdG86Z3JvdXBzI\
            jpbInVzLWVhc3QtMl9aY2F1bFRzUFdfR29vZ2xlIl0sImVtYWlsX3ZlcmlmaWVkIjpmYWxzZSwiaXNzIjoiaHR0cHM6XC9cL2NvZ25pdG8taWRwLnVzL\
            WVhc3QtMi5hbWF6b25hd3MuY29tXC91cy1lYXN0LTJfWmNhdWxUc1BXIiwiY29nbml0bzp1c2VybmFtZSI6Imdvb2dsZV8xMTcwMjIxNDg1MDEwNzcxN\
            DU3NjYiLCJhdWQiOiI3Y3ZxdTVuNm4xYzhwaTMyNzY4NGE1Nms5NiIsImlkZW50aXRpZXMiOlt7InVzZXJJZCI6IjExNzAyMjE0ODUwMTA3NzE0NTc2N\
            iIsInByb3ZpZGVyTmFtZSI6Ikdvb2dsZSIsInByb3ZpZGVyVHlwZSI6Ikdvb2dsZSIsImlzc3VlciI6bnVsbCwicHJpbWFyeSI6InRydWUiLCJkYXRlQ\
            3JlYXRlZCI6IjE2MTA2MjMzOTY3OTAifV0sInRva2VuX3VzZSI6ImlkIiwiYXV0aF90aW1lIjoxNjEwNzg2Mjc0LCJleHAiOjE2MTA3ODk4NzQsImlhd\
            CI6MTYxMDc4NjI3NCwiZW1haWwiOiJzaGFuaHVhLmh1YW5nQGdtYWlsLmNvbSJ9.TXzS6CRNqpbQHpL4U-dejEvDAgPXXcprYyzI97lpHr0cuPLeEXwA3\
            xeCdtng-wWYgVk08vu4mBCqv1dq0WiWkGb25qdMeYTn5seuaUBfVBi7X2EPYbP70Qb2xCu-ZAaL1cRzn-DMJiyi4VlAGADwtwlW8KC0sA_Z_vsRSQ8Ws4\
            l81ff3EYWjcsWgUQu34Sg9YKDcwOa5lluINdYAPbuXZcUNovyYoOKOtSW_6nW6W7DvxxpCnHhtbsH1Xp0bvE2V87o8i8zTb-mWeB7PLaZ9rHfqE6fGGr\
            hW2KyhbbEG-P0ssqpj6Q4NQppZcMolTI0NYA2hkyPxXAE3UyfHmUTzkQ";
    OmniProxy *proxy = new OmniProxy();
    proxy->setToken(idToken);
    proxy->joinVirtualNetwork();
    //proxy->generatePubKey();

    QString community_name = "omniedge";
    QString encrypt_key    = "66YRd88kyYdhzk";

    N2NWorkerWrapper* n2n = new N2NWorkerWrapper();
    GoogleOAuth *oauth = new GoogleOAuth();
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
