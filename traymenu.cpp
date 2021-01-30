#include "traymenu.h"
#include <QAction>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>

TrayMenu::TrayMenu()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/images/AppIcon.png"));
    trayIcon->setVisible(true);
    trayIcon->showMessage("OmniEdge", "Connect without corcern", QSystemTrayIcon::Information, 3000);
    trayIcon->setToolTip("OmniEdge");
    createActions();
    createTrayIcon();
}

void TrayMenu::createActions()
{
    loginAction = new QAction(tr("Log in..."), this);
    connect(loginAction, &QAction::triggered, this, &QWidget::hide);
    logoutAction = new QAction(tr("Logout"), this);
    aboutAction = new QAction(tr("About..."), this);
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void TrayMenu::createTrayIcon()
{
    trayIconMenu = new QMenu(this);

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(loginAction);
    trayIconMenu->addAction(logoutAction);

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);
}

void TrayMenu::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Systray"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void TrayMenu::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        //iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1) % iconComboBox->count());
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}

void TrayMenu::showMessage()
{

}



