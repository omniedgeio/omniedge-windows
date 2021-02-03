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
    connect(controller, &MenuController::showMessage, this, &TrayMenu::showMessage);
    connect(controller, &MenuController::updateStatus, this, &TrayMenu::updateStatus);
    connect(controller, &MenuController::oauthloginStatus, this, &TrayMenu::createMenu);
    connect(controller, &MenuController::n2nConnected, this, &TrayMenu::connected);
    connect(controller, &MenuController::n2nDisconnected, this, &TrayMenu::disconnected);

    /* Create actions */
    statusAction = new QAction(tr("Starting application..."), this);

    statusSeperator = new QAction();
    statusSeperator->setSeparator(true);

    connectAction = new QAction(tr("Connect"), this);
    connect(connectAction, &QAction::triggered, controller, &MenuController::connectSN);

    disconnectAction = new QAction(tr("Disconnect"), this);
    connect(disconnectAction, &QAction::triggered, controller, &MenuController::disconnectSN);

    devicesMenu = new QMenu("Network devices");

    connectionSeperator = new QAction();
    connectionSeperator->setSeparator(true);

    loginAction = new QAction(tr("Log in..."), this);
    connect(loginAction, &QAction::triggered, controller, &MenuController::login);

    logoutAction = new QAction(tr("Logout"), this);
    connect(logoutAction, &QAction::triggered, controller, &MenuController::logout);

    dashboardAction = new QAction(tr("Dashboard"), this);
    connect(dashboardAction, &QAction::triggered, this, &TrayMenu::dashboard);

    webSeperator = new QAction();
    webSeperator->setSeparator(true);

    aboutAction = new QAction(tr("About..."), this);
    connect(aboutAction, &QAction::triggered, this, &TrayMenu::aboutDialog);

    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(statusAction);
    statusAction->setVisible(false);

    trayIconMenu->addAction(statusSeperator);

    trayIconMenu->addAction(connectAction);
    connectAction->setVisible(false);

    trayIconMenu->addAction(disconnectAction);
    disconnectAction->setVisible(false);

    trayIconMenu->addAction(connectionSeperator);
    connectionSeperator->setVisible(false);

    trayIconMenu->addAction(dashboardAction);
    dashboardAction->setVisible(false);

    trayIconMenu->addAction(loginAction);

    trayIconMenu->addAction(logoutAction);
    logoutAction->setVisible(false);

    trayIconMenu->addAction(webSeperator);

    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(quitAction);

    QSettings settings;
    if(settings.contains(SETTINGS_ID_TOKEN)){
        statusAction->setText("Starting application...");
        statusAction->setVisible(true);
        emit checkLoginStatus();
    }

    trayIcon->setContextMenu(trayIconMenu);
}

TrayMenu::~TrayMenu()
{

}

void TrayMenu::createMenu(bool loginStatus)
{
    qDebug() << "Tray menu: Login status : " << loginStatus;
    if(loginStatus){
        statusAction->setText("My Address: " +
                              this->controller->supernodes[this->controller->virtualNetworks.at(0).id].virtualIP);
        statusAction->setVisible(true);
        statusSeperator->setVisible(true);

        connectAction->setVisible(true);
        disconnectAction->setVisible(true);
        disconnectAction->setEnabled(false);
        connectionSeperator->setVisible(true);

        dashboardAction->setVisible(true);

        for(Device device : this->controller->virtualNetworks.at(0).devices){
            devicesMenu->addAction(device.virtualIP + " " + device.name);
        }
        trayIconMenu->insertMenu(connectionSeperator, devicesMenu);

        logoutAction->setText(tr("Logout as " + this->controller->userInfo.email.toLocal8Bit()));
        logoutAction->setVisible(true);

        loginAction->setVisible(false);
        showMessage("Login successfully", "Connect without concern");
    } else {
        statusAction->setText("My Address: " +
                              this->controller->supernodes[this->controller->virtualNetworks.at(0).id].virtualIP);
        statusAction->setVisible(false);
        statusSeperator->setVisible(false);

        connectAction->setVisible(false);
        disconnectAction->setVisible(false);
        disconnectAction->setEnabled(false);
        connectionSeperator->setVisible(false);

        dashboardAction->setVisible(false);

        devicesMenu->clear();
        trayIconMenu->removeAction(devicesMenu->menuAction());

        logoutAction->setText(tr("Logout as " + this->controller->userInfo.email.toLocal8Bit()));
        logoutAction->setVisible(false);

        loginAction->setVisible(true);
        showMessage("Logout successfully", "Looking forward to see you again");
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

void TrayMenu::updateStatus(QString statusMsg)
{
    statusAction->setText(statusMsg);
    statusAction->setVisible(true);
    statusSeperator->setVisible(true);
}

void TrayMenu::connected()
{
    connectAction->setEnabled(false);
    disconnectAction->setEnabled(true);
    statusAction->setText("My Address: " +
                          this->controller->supernodes[this->controller->virtualNetworks.at(0).id].virtualIP);
}

void TrayMenu::disconnected()
{
    connectAction->setEnabled(true);
    disconnectAction->setEnabled(false);
    statusAction->setText("Status: Offline");
}
