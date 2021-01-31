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
    QNetworkInterface interface;
    QStringList interfaceList;
    QList<QNetworkInterface> IpList = interface.allInterfaces();
    for (int i = 0; i < IpList.size(); i++)
        interfaceList.append(IpList.at(i).humanReadableName());

//    static bool is64bitHost = QSysInfo::currentCpuArchitecture().contains(QLatin1String("64"));
//    if (is64bitHost) {
//        this->tapPath = "amd64";
//    } else {
//        this->tapPath = "i386";
//    }
//    this->tapPath = QDir::currentPath() + "\\tap-windows\\" + this->tapPath;

    QFileInfo check_file(QDir::currentPath() + "\\tap-windows\\add_tap_device.bat");
    if (check_file.exists() && check_file.isFile()) {
        qDebug() << "TapManager: " << QDir::currentPath() + "\\tap-windows\\add_tap_device.bat" << " exists.";
        qDebug()<<interfaceList;
        if(!interfaceList.contains("OmniEdge",Qt::CaseSensitive))
        {
            QProcess process;
            process.start(QDir::currentPath() + "\\tap-windows\\add_tap_device.bat");
            process.waitForFinished();
            qDebug() << process.readAllStandardOutput();
        }
        else
            qDebug() << "OmniEdge tap device exist";

    } else {
        qDebug() << "TapManager: " << QDir::currentPath()  + "\\tap-windows\\add_tap_device.bat" << " not exists.";
    }
}

void TapManager::addTap()
{
//    QString infPath = "";
//    if(QOperatingSystemVersion::current() >= QOperatingSystemVersion::Windows10) {
//        infPath = "\\win10";
//    }

//    QFileInfo check_file(tapPath + infPath + "\\OemVista.inf");
//    if (check_file.exists() && check_file.isFile()) {
//        qDebug() << "TapManager: " << tapPath + infPath + "\\OemVista.inf" << " exists.";
//    } else {
//        qDebug() << "TapManager: " << tapPath + infPath + "\\OemVista.inf" << " not exists.";
//    }

//    QProcess process;
//    process.start(this->tapPath + "\\tapinstall.exe",
//                  QStringList() <<
//                  "install" <<
//                  tapPath + infPath + "\\OemVista.inf" <<
//                  "TAP0901");
//    process.waitForFinished();
//    qDebug() << process.readAllStandardOutput();
    QProcess process;
    process.start(QDir::currentPath() + "\\tap-windows\\add_tap_device.bat");
    process.waitForFinished();
    qDebug() << process.readAllStandardOutput();
}


