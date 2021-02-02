#ifndef TRAYMENU_H
#define TRAYMENU_H

#include <QSystemTrayIcon>
#include <QDialog>
#include <QAction>
#include <QMenu>
#include "menucontroller.h"
#include "aboutdialog.h"

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

signals:
    void checkLoginStatus();
    void loginSignal();
    void logoutSignal();
    void connectSNSignal();
    void disconnectSNSignal();

private:
    void createActions();
    void createMenu(bool loginStatus);

    QAction *loginAction;
    QAction *logoutAction;
    QAction *connectAction;
    QAction *disconnectAction;
    QAction *dashboardAction;
    QAction *aboutAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QMenu *devicesMenu;

    MenuController* controller;
    AboutDialog *aboutDlg;

};
#endif
