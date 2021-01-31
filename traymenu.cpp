#include "traymenu.h"
#include <QAction>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>

TrayMenu::TrayMenu()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/images/AppIcon.png"));
    trayIcon->setVisible(true);
    trayIcon->showMessage("OmniEdge", "Connect without corcern", QSystemTrayIcon::Information, 1000);
    trayIcon->setToolTip("OmniEdge");

    controller = new MenuController();
    connect(this, &TrayMenu::checkLoginStatus, controller, &MenuController::getUserInfoSignal);
    connect(controller, &MenuController::oauthloginStatus, this, &TrayMenu::createMenu);
    connect(controller, &MenuController::n2nConnected, this, &TrayMenu::connected);
    connect(controller, &MenuController::n2nDisconnected, this, &TrayMenu::disconnected);

    /* Create actions */
    connectAction = new QAction(tr("Connect"), this);
    connect(connectAction, &QAction::triggered, controller, &MenuController::connectSN);
    disconnectAction = new QAction(tr("Disconnect"), this);
    connect(disconnectAction, &QAction::triggered, controller, &MenuController::disconnectSN);

    loginAction = new QAction(tr("Log in..."), this);
    connect(loginAction, &QAction::triggered, controller, &MenuController::login);

    dashboardAction = new QAction(tr("Dashboard"), this);
    connect(dashboardAction, &QAction::triggered, this, &TrayMenu::dashboard);

    aboutAction = new QAction(tr("About..."), this);
    connect(aboutAction, &QAction::triggered, this, &TrayMenu::aboutDialog);

    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcut(QKeySequence("Shift+Q"));
    quitAction->setShortcutContext(Qt::ApplicationShortcut);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIconMenu = new QMenu(this);

    QSettings settings;
    if(settings.contains(SETTINGS_ID_TOKEN)){
        trayIconMenu->addAction("Status: Starting application...");
        emit checkLoginStatus();
    }

    trayIconMenu->addAction(loginAction);

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);
}

TrayMenu::~TrayMenu()
{

}

void TrayMenu::createMenu(bool loginStatus)
{
    delete trayIconMenu;
    trayIconMenu = new QMenu(this);
    qDebug() << "Tray menu: Login status : " << loginStatus;
    if(loginStatus){
        trayIconMenu->addAction("My Address: " + this->controller->supernodes[this->controller->virtualNetworks.at(0).id].virtualIP);
        trayIconMenu->addSeparator();
        trayIconMenu->addAction(connectAction);
        trayIconMenu->addAction(disconnectAction);
        disconnectAction->setEnabled(false);
        trayIconMenu->addSeparator();
        trayIconMenu->addAction(dashboardAction);

        devicesMenu= trayIconMenu->addMenu("Network Device");
        for(Device device : this->controller->virtualNetworks.at(0).devices){
            devicesMenu->addAction(device.virtualIP + " " + device.name);
        }
        trayIconMenu->addSeparator();
        logoutAction = new QAction(tr("Logout as " + this->controller->userInfo.email.toLocal8Bit()), this);
        connect(logoutAction, &QAction::triggered, controller, &MenuController::logout);
        trayIconMenu->addAction(logoutAction);
        trayIconMenu->addAction(aboutAction);
        trayIconMenu->addAction(quitAction);

        trayIcon->setContextMenu(trayIconMenu);
    } else {
        trayIconMenu->addAction(loginAction);

        trayIconMenu->addSeparator();
        trayIconMenu->addAction(aboutAction);
        trayIconMenu->addAction(quitAction);

        trayIcon->setContextMenu(trayIconMenu);
    }
}

void TrayMenu::dashboard()
{
    QString link = "https://www.omniedge.io";
    QDesktopServices::openUrl(QUrl(link));
}

void TrayMenu::aboutDialog()
{
    QDialog dialog;
    this->setWindowTitle("Custom Dialog");
    resize(400, 300);
}

void TrayMenu::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
 /*   switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }*/
}

void TrayMenu::showMessage(QString title,QString msg)
{
    trayIcon->showMessage(title, msg, QSystemTrayIcon::Information, 1000);
}

void TrayMenu::connected()
{
    connectAction->setEnabled(false);
    disconnectAction->setEnabled(true);
}

void TrayMenu::disconnected()
{
    connectAction->setEnabled(true);
    disconnectAction->setEnabled(false);
}
