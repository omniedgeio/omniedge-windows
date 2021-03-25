#ifndef DOWNLOADUPDATEDIALOG_H
#define DOWNLOADUPDATEDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>
#include <QFile>
#include <log.h>
#include <QDir>
#include <QProcess>
#include <QMessageBox>
#include <QCoreApplication>
//#include "arch.h"

namespace Ui {
    class DownloadUpdateDialog;
}

class DownloadUpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadUpdateDialog(QWidget *parent = 0);
    ~DownloadUpdateDialog();
    void setupUi();
    void startDownload(QString version);
    void displayErrorMessage(QString error);
private:
    Ui::DownloadUpdateDialog *ui;
    QNetworkAccessManager* netManager;
    QFile* tmpFile;
    int id_connect,id_login, id_cd_folder, id_cd_version, id_get, id_close;

private Q_SLOTS:
    void updateDataTransferProgress(qint64 done, qint64 total);
    void downloadFinished(QNetworkReply *reply);
    void launchInstaller();
    void handleSslErrors(QNetworkReply* reply, const QList<QSslError> & errors);
};

#endif // DOWNLOADUPDATEDIALOG_H
