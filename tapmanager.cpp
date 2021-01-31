#include "tapmanager.h"
#include <QSysInfo>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QNetworkInterface>
#include <QOperatingSystemVersion>

TapManager::TapManager(QObject *parent) : QObject(parent)
{
    static bool is64bitHost = QSysInfo::currentCpuArchitecture().contains(QLatin1String("64"));
    if (is64bitHost) {
        this->tapPath = "amd64";
    } else {
        this->tapPath = "i386";
    }
    this->tapPath = QDir::currentPath() + "\\tap-windows\\" + this->tapPath;

    QFileInfo check_file(this->tapPath + "\\tapinstall.exe");
    if (check_file.exists() && check_file.isFile()) {
        qDebug() << "TapManager: " << this->tapPath + "\\tapinstall.exe" << " exists.";
    } else {
        qDebug() << "TapManager: " << this->tapPath + "\\tapinstall.exe" << " not exists.";
    }
}

void TapManager::addTap()
{
    QString infPath = "";
    if(QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows10) {
        infPath = "\\win10";
    }

    QFileInfo check_file(tapPath + infPath + "\\OemVista.inf");
    if (check_file.exists() && check_file.isFile()) {
        qDebug() << "TapManager: " << tapPath + infPath + "\\OemVista.inf" << " exists.";
    } else {
        qDebug() << "TapManager: " << tapPath + infPath + "\\OemVista.inf" << " not exists.";
    }

    QProcess process;
    process.start(this->tapPath + "\\tapinstall.exe",
                  QStringList() <<
                  "install" <<
                  tapPath + infPath + "\\OemVista.inf" <<
                  "TAP0901");
    process.waitForFinished();
    qDebug() << process.readAllStandardOutput();
}


