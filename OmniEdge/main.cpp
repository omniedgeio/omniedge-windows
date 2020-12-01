#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "edgenode.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    EdgeNode edge;
    edge.EdgeNodeRun();
    return app.exec();
}
