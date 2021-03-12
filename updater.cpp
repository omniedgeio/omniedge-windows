#include "updater.h"
#include <QUrlQuery>

Updater::Updater(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

Updater::~Updater()
{
    delete manager;
}

void Updater::checkForUpdates(int flag)
{

    if(flag == ForceNotification)
    {
        notifyUpdates = true;
    }else if(flag == NoNotification)
    {
        notifyUpdates = false;
    }
    QNetworkRequest appUpdateCheckReq;
    appUpdateCheckReq.setUrl(QUrl("https://raw.githubusercontent.com/omniedgeio/omniedge-windows-update/main/check_version.xml"));
    manager->get(appUpdateCheckReq);
}
void Updater::showUpdateNotificationDialog()
{
    INFO(tr("There is a new verision available (") + latestVersion + ")");
    if(notifyUpdates)
    {
        //Show update message
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Update available"));
        msgBox.setIcon(QMessageBox::Information);
        QPushButton changelogBtn(tr("Changelog"));
#ifdef Q_OS_WIN
        msgBox.addButton(QMessageBox::Yes);
        msgBox.addButton(QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setText(tr("There's a new version of Omniedge available. Do you want to download it?"));
#endif
        msgBox.addButton(&changelogBtn, QMessageBox::HelpRole);
        changelogBtn.disconnect(); //Make sure changelog button dosen't close the dialog
        connect(&changelogBtn, SIGNAL(clicked()), this, SLOT(showChangelog()));
        connect(this, SIGNAL(updateDialogsRejected()), &msgBox, SLOT(reject()));

        int selection = msgBox.exec();
        if(selection == QMessageBox::Yes)
        {
            DownloadUpdateDialog dialog;
            dialog.startDownload(latestVersion);
            dialog.exec();
        }
    }
}

void Updater::rejectNotificationDialogs()
{
    Q_EMIT updateDialogsRejected();
}

void Updater::showChangelog()
{
//    ChangelogDialog changelog;
//    connect(this, SIGNAL(updateDialogsRejected()), &changelog, SLOT(reject()));
//    changelog.exec();
}

void Updater::progressUpdate(int)
{
    Q_EMIT updateProgessRange(0, numPluginsUpdating * 4);
}


void Updater::replyFinished(QNetworkReply *reply)
{
    QString replyText = reply->readAll();
    if(reply->error() != QNetworkReply::NoError)
    {
        //Parse servers response
        QDomDocument doc("error");
        if (!doc.setContent(replyText)) {
            //No XML to parse, user is probably disconnected
            return;
        }else
        {
            QDomElement docElem = doc.documentElement();
            QDomElement message = docElem.firstChildElement("message");
            if(!message.text().isEmpty())
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Failed to check for updates"));
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(tr("Failed to check for updates.\nError was: ") + message.text());
                msgBox.exec();
            }
        }
    }else
    {
        //This is check_version.xml
        QDomDocument doc("reply");
        if (!doc.setContent(replyText)) {
            return;
        }
        QDomElement docElem = doc.documentElement();
        QDomElement versionElem = docElem.firstChildElement("current_version");
        QDomElement outdatedElem = docElem.firstChildElement("outdated");
        latestVersion = versionElem.text();
        bool outdated = QVariant(outdatedElem.text()).toBool();
        if(outdated && notifyUpdates)
        {
            Q_EMIT newVersionAvailable(latestVersion);
            showUpdateNotificationDialog();
        }
        Q_EMIT versionNumberRecieved(latestVersion, outdated);
    }
}

