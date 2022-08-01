#include "traymenu.h"
#include <QAction>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>
#include "api.h"

TrayMenu::TrayMenu()
{
    aboutDlg = new AboutDialog();
    trayIcon = new QSystemTrayIcon(QIcon(":/images/AppIcon.png"));
    trayIcon->setVisible(true);
    //trayIcon->showMessage("OmniEdge", "Connect without concern", QSystemTrayIcon::Information, 1000);
    trayIcon->setToolTip("OmniEdge");
    updater = new Updater();
    controller = new MenuController();
    connect(this, SIGNAL(loginSignal(QString)), controller, SLOT(getUserIdToken(QString)));
    connect(controller, &MenuController::showMessage, this, &TrayMenu::showMessage);
    connect(controller, &MenuController::updateStatus, this, &TrayMenu::updateStatus);
    connect(controller, &MenuController::updateEmail, this, &TrayMenu::updateEmail);
    connect(controller, &MenuController::oauthloginStatus, this, &TrayMenu::createMenu);
    connect(controller, &MenuController::n2nConnected, this, &TrayMenu::connected);
    connect(controller, &MenuController::n2nDisconnected, this, &TrayMenu::disconnected);
    connect(controller, SIGNAL(showMessage(QString,QString)), this, SLOT(showMessage(QString,QString)));
    /* Create actions */
    statusAction = new QAction(tr("Starting application..."), this);

    statusSeperator = new QAction();
    statusSeperator->setSeparator(true);

    disconnectAction = new QAction(tr("Disconnect"), this);
    connect(disconnectAction, &QAction::triggered, controller, &MenuController::disconnectSN);

    devicesMenu = new QMenu(tr("Virtual Networks"));

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
    connect(aboutAction, &QAction::triggered, this, &TrayMenu::showAboutDialog);

    updateAction = new QAction(tr("Check update"), this);
    connect(updateAction, &QAction::triggered, updater, &Updater::checkForUpdates);

    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(statusAction);
    statusAction->setVisible(false);

    trayIconMenu->addAction(statusSeperator);

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
    trayIconMenu->addAction(updateAction);
    trayIconMenu->addAction(quitAction);

    QSettings settings;
    if(settings.contains(SETTINGS_ID_TOKEN)){
        statusAction->setText(tr("Starting application..."));
        statusAction->setVisible(true);
        emit loginSignal(settings.value(SETTINGS_ID_TOKEN).toString());
    }

    trayIcon->setContextMenu(trayIconMenu);
}

TrayMenu::~TrayMenu()
{

}

void TrayMenu::createMenu(bool loginStatus)
{
    QMenu* submenu;
    //qDebug() << "Tray menu: Login status : " << loginStatus;
    if(loginStatus){
        statusAction->setVisible(true);
        statusSeperator->setVisible(true);

        disconnectAction->setVisible(true);
        disconnectAction->setEnabled(false);
        connectionSeperator->setVisible(true);

        dashboardAction->setVisible(true);
        for(int i = 0;i<this->controller->virtualNetworks.length();i++ )
        {
            submenu = devicesMenu->addMenu(this->controller->virtualNetworks.at(i).name);
            QAction* subAction = submenu->addAction(tr("Join"));
            submenu->setDefaultAction(subAction);
            subAction->setIconText(tr("Join"));
            subAction->setCheckable(true);
            submenu->addSeparator();
            connect(subAction,  &QAction::triggered, [=]() {
                this->controller->joinVirtualNetworkManual(this->controller->virtualNetworks.at(i).uuid);
            });
            for(int j =0;j<this->controller->virtualNetworks.at(i).devices.length();j++)
                submenu->addAction(this->controller->virtualNetworks.at(i).devices.at(j).name);
            trayIconMenu->insertMenu(connectionSeperator, devicesMenu);
        }

        logoutAction->setText(tr("Logout as ")+ this->controller->userEmail);
        logoutAction->setVisible(true);

        loginAction->setVisible(false);
        showMessage(tr("Login successfully"), tr("Bring intranet on the internet."));
    } else {
        statusAction->setVisible(false);
        statusSeperator->setVisible(false);

        disconnectAction->setVisible(false);
        disconnectAction->setEnabled(false);
        connectionSeperator->setVisible(false);

        dashboardAction->setVisible(false);
        devicesMenu->clear();
        trayIconMenu->removeAction(devicesMenu->menuAction());

        logoutAction->setText(tr("Logout as ") + this->controller->userEmail);
        logoutAction->setVisible(false);

        loginAction->setVisible(true);
        showMessage(tr("Logout successfully"), tr("Looking forward to see you again"));
    }
}


void TrayMenu::dashboard()
{
    QString link = "https://omniedge.io/dashboard";
    QDesktopServices::openUrl(QUrl(link));
}

void TrayMenu::showAboutDialog()
{
    aboutDlg->open();
}

void TrayMenu::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    Q_UNUSED(reason);
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
void TrayMenu::updateEmail(QString mail)
{
    logoutAction->setText(tr("Logout as ") + mail);
}

void TrayMenu::connected()
{
    disconnectAction->setEnabled(true);
    statusAction->setText(tr("My Address: ") + this->controller->myVirtualIP);
}

void TrayMenu::disconnected()
{
    disconnectAction->setEnabled(false);
    statusAction->setText(tr("Status: Offline"));
}
