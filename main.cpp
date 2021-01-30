#include "traymenu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(edge);
    QApplication a(argc, argv);

    TrayMenu menu;
    return a.exec();
}
