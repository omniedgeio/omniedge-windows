#include "traymenu.h"
#include <QMessageBox>
#include <QApplication>
#include <QMenu>
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(edge);
    QApplication a(argc, argv);

    QMenu* menu = new QMenu();
    QAction *action1;
    action1= new QAction("action1", NULL);
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon(":/images/AppIcon.png"));
    menu->addAction(action1);
    trayIcon->setContextMenu( menu);
    trayIcon->setVisible(true);
    trayIcon->showMessage("Test Message", "Text", QSystemTrayIcon::Information, 1000);
    return a.exec();
}
