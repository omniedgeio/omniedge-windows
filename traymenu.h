#ifndef TRAYMENU_H
#define TRAYMENU_H

#include <QSystemTrayIcon>
#include <QDialog>
#include <QAction>
#include <QMenu>
#include "menucontroller.h"
#include "aboutdialog.h"
#include "updater.h"

enum TrayMenuStatus {
    Unauthorized,
    Authorized,
    Connected,
};

class TrayMenu : public QDialog
{
    Q_OBJECT

public:
    explicit TrayMenu();
    virtual ~TrayMenu();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showAboutDialog();
    void dashboard();
    void showMessage(QString title, QString msg);
    void connected();
    void disconnected();
    void updateStatus(QString statusMsg);
    void updateEmail(QString mail);

signals:
    void loginSignal(QString idtoken);
    void logoutSignal();
    void connectSNSignal();
    void disconnectSNSignal();

private:
    void createActions();
    void createMenu(bool loginStatus);

    QMenu* submenu;
    QAction *statusAction;
    QAction *statusSeperator;

    QAction *virtualnetworksAction;
    QAction *disconnectAction;
    QMenu   *devicesMenu;
    QAction *connectionSeperator;

    QAction *dashboardAction;
    QAction *loginAction;
    QAction *logoutAction;
    QAction *webSeperator;

    QAction *aboutAction;
    QAction *updateAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    MenuController* controller;
    Updater* updater;
    AboutDialog *aboutDlg;
    QMap<QString, QAction*> deviceActionList;

};
#endif
