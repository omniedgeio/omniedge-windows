#include <QApplication>
#include <QCoreApplication>
#include <QSettings>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include "traymenu.h"
#include "runguard.h"
#include "syslog.h"
#include <QString>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    // load default language by system language
    QString locale = QLocale::system().name();
    qDebug() << locale;
    QTranslator translator;
    if (locale.startsWith("zh_CN")) {
        translator.load(QCoreApplication::applicationDirPath() + "/languages/zh_CN.qm");
    } else {
        translator.load(QCoreApplication::applicationDirPath() + "/languages/en_US.qm");
    }
    app.installTranslator(&translator);

    // single instance lock
    RunGuard guard(QStringLiteral("Omniedge_Desktop_Client"));
    if ( !guard.tryToRun() )
        return 0;

#ifdef QT_NO_DEBUG
    syslog hlog;
    hlog.installReleaseMsgHandler();
#endif

    // Setting for QSettings
    QCoreApplication::setOrganizationName("Omniedge");
    QCoreApplication::setOrganizationDomain("omniedge.io");
    QCoreApplication::setApplicationName("Omniedge Desktop Client");

    TrayMenu menu;

    return app.exec();
}
