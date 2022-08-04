#include "traymenu.h"
#include "api.h"
#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include "menucontroller.h"
#include "syslog.h"
#include "runguard.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    //load default language by system language
    QString locale = QLocale::system().name();
    qDebug() << locale;
    QTranslator translator;
    if(locale.compare("zh_CN") ==0)
    {
        translator.load(qApp->applicationDirPath()+QString("/languages/zh_CN.qm"));
    }
    else
    {
        translator.load(qApp->applicationDirPath()+QString("/languages/en_US.qm"));
    }
    app.installTranslator(&translator);


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

    return app.exec();
}
