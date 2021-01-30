#ifndef TRAYMENU_H
#define TRAYMENU_H

#include <QSystemTrayIcon>

#ifndef QT_NO_SYSTEMTRAYICON

#include <QDialog>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

//! [0]
class TrayMenu : public QDialog
{
    Q_OBJECT

public:
    TrayMenu();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage();

private:
    void createActions();
    void createTrayIcon();


    QAction *loginAction;
    QAction *logoutAction;
    QAction *connectAction;
    QAction *disconnectAction;
    QAction *aboutAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // QT_NO_SYSTEMTRAYICON

#endif
