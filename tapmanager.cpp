#include "tapmanager.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QNetworkInterface>
#include <QOperatingSystemVersion>
#include <QProcess>
#include <QSysInfo>

TapManager::TapManager(QObject *parent) : QObject(parent) {
  QNetworkInterface interface;
  QStringList interfaceList;
  QList<QNetworkInterface> IpList = interface.allInterfaces();
  for (int i = 0; i < IpList.size(); i++)
    interfaceList.append(IpList.at(i).humanReadableName());

  QString appPath = QCoreApplication::applicationDirPath();
  QString batPath = appPath + "/tap-windows/add_tap_device.bat";
  QFileInfo check_file(batPath);
  if (check_file.exists() && check_file.isFile()) {
    qDebug() << "TapManager: " << batPath << " exists.";
    if (!interfaceList.contains("OmniEdge", Qt::CaseSensitive)) {
      QProcess::execute(batPath);
    } else
      qDebug() << "OmniEdge tap device exist";

  } else {
    qDebug() << "TapManager: " << batPath << " not exists.";
  }
}

void TapManager::addTap() {
  QString appPath = QCoreApplication::applicationDirPath();
  QString batPath = appPath + "/tap-windows/add_tap_device.bat";
  QProcess::execute(batPath);
}
