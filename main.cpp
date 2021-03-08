#include "traymenu.h"
#include <QApplication>
#include <QSettings>
#include <QQmlApplicationEngine>
#include <QtQml/qqml.h>
#include "menucontroller.h"
#include "syslog.h"
#include "runguard.h"
#include "maintenancetool.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    //single instance
    RunGuard guard( "Omniedge_Desktop_Client" );
    if ( !guard.tryToRun() )
        return 0;

#ifdef QT_NO_DEBUG
    syslog hlog;
    hlog.installReleaseMsgHandler();
#endif

    // Setting for QSetting
    QCoreApplication::setOrganizationName("Omniedge");
    QCoreApplication::setOrganizationDomain("omniedge.io");
    QCoreApplication::setApplicationName("Omniedge Desktop Client");

    TrayMenu menu;
//    qmlRegisterType<MaintenanceTool>("com.example.plugin.maintenancetool"
//                                 , 1, 0, "MaintenanceTool");

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return a.exec();
}
