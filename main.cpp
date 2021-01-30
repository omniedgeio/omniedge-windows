#include "traymenu.h"
#include <QApplication>
#include "oauth.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Setting for QSetting
    QCoreApplication::setOrganizationName("Omniedge");
    QCoreApplication::setOrganizationDomain("omniedge.io");
    QCoreApplication::setApplicationName("Omniedge Desktop Client");

    TrayMenu menu;
    OAuth* oauth = new OAuth();
    oauth->grant();
    return a.exec();
}
