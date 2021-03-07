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
    qmlRegisterType<MaintenanceTool>("com.example.plugin.maintenancetool"
                                   , 1, 0, "MaintenanceTool");

      QQmlApplicationEngine engine;
      engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    //create the updater with the application as parent -> will live long enough start the tool on exit
     /*   auto updater = new QtAutoUpdater::Updater::create("qtifw", {
            {"path", "C:/Program Files (x86)/OmniEdge/MaintenanceTool"} //.exe or .app is automatically added on the platform
        }, &a);

        QObject::connect(updater, &QtAutoUpdater::Updater::checkUpdatesDone, [updater](QtAutoUpdater::Updater::State state) {
            qDebug() << "Update result:" << state;
            if (state == QtAutoUpdater::Updater::State::NewUpdates) {
                //As soon as the application quits, the maintenancetool will be started in update mode
                qDebug() << "Update info:" << updater->updateInfo();
                updater->runUpdater();
            }
            //Quit the application
            qApp->quit();
        });

        //start the update check
        updater->checkForUpdates();*/

    return a.exec();
}
