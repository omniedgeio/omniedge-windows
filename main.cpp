#include "traymenu.h"
#include <QApplication>
#include "menucontroller.h"
#include "syslog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef QT_NO_DEBUG
    syslog hlog;
    hlog.installReleaseMsgHandler();
#endif

    // Setting for QSetting
    QCoreApplication::setOrganizationName("Omniedge");
    QCoreApplication::setOrganizationDomain("omniedge.io");
    QCoreApplication::setApplicationName("Omniedge Desktop Client");

    TrayMenu menu;

    return a.exec();
}
