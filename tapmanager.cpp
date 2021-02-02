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

    QFileInfo check_file(QDir::currentPath() + "\\tap-windows\\add_tap_device.bat");
    if (check_file.exists() && check_file.isFile()) {
        qDebug() << "TapManager: " << QDir::currentPath() + "\\tap-windows\\add_tap_device.bat" << " exists.";
        if(!interfaceList.contains("OmniEdge",Qt::CaseSensitive))
        {
            QProcess process;
            process.start(QDir::currentPath() + "\\tap-windows\\add_tap_device.bat");
            process.waitForFinished();
        }
        else
            qDebug() << "OmniEdge tap device exist";

    } else {
        qDebug() << "TapManager: " << QDir::currentPath()  + "\\tap-windows\\add_tap_device.bat" << " not exists.";
    }
}

void TapManager::addTap()
{
    QProcess process;
    process.start(QDir::currentPath() + "\\tap-windows\\add_tap_device.bat");
    process.waitForFinished();
}


