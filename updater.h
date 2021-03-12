#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDomDocument>
#include <QMessageBox>
#include <log.h>
#include <QSettings>
#include <downloadupdatedialog.h>
//#include <dialog/changelogdialog.h>
#include <QStringList>
#include <QProgressDialog>
#include <QPushButton>

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = 0);
    virtual ~Updater();
    void loadSettings();
    void checkForUpdates(int flag = 0);

private:
    QNetworkAccessManager *manager;
    bool notifyUpdates;
    QString latestVersion;
    int numPluginsUpdating;
    QMessageBox updateMsgBox, pluginUpdateMsgBox;
    //ChangelogDialog changelogDialog;

public:
    const static int NoNotification = 1;
    const static int ForceNotification = 2;
Q_SIGNALS:
    void newVersionAvailable(QString versionNumber);
    void versionNumberRecieved(QString versionNumber, bool outdated);
    void pluginsUpdated();
    void updateProgessRange(int, int);
    void updateDialogsRejected();

public Q_SLOTS:
    void showUpdateNotificationDialog();
    void rejectNotificationDialogs();
    void showChangelog();
    void progressUpdate(int);
    void replyFinished(QNetworkReply* reply);

};

#endif // UPDATER_H
