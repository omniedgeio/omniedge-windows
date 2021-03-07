#include "maintenancetool.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

MaintenanceTool::MaintenanceTool(QObject *parent) :
  QObject(parent)
, m_state(MaintenanceTool::NotRunning)
, m_hasUpdate(false)
{
  //プロセスが開始時のシグナル
  connect(&m_process, SIGNAL(started()), this, SLOT(processStarted()));
  //プロセスが終了時のシグナル
  connect(&m_process, SIGNAL(finished(int, QProcess::ExitStatus))
          , this, SLOT(processFinished(int, QProcess::ExitStatus)));
  //プロセスがエラー終了時のシグナル
  connect(&m_process, SIGNAL(error(QProcess::ProcessError))
          , this, SLOT(processError(QProcess::ProcessError)));
}

//更新の確認開始
void MaintenanceTool::checkUpdate()
{
  startMaintenanceTool(MaintenanceTool::CheckUpdate);
}

void MaintenanceTool::startMaintenanceTool(StartMode mode)
{
  QString toolName;
  QString path;
  toolName = "maintenancetool.exe";
  path = QDir("C:\\Program Files (x86)\\OmniEdge").absoluteFilePath(toolName);
  qDebug() << path;

  QStringList args;
  if(mode == MaintenanceTool::CheckUpdate){
    if(m_process.state() == QProcess::NotRunning){
      setUpdateDetails(QString());
      setHasUpdate(false);
      //アップデート確認用のオプションを設定                            [1]
      args.append("--checkupdates");
      //実行
      m_process.start(path, args);
    }else{
      qDebug() << "Already started.";
    }
  }else{
    //更新用のオプションを設定                                   [2]
    args.append("--updater");
    //メンテツールの通常起動はプロセス管理しない
    QProcess::startDetached(path, args);
  }
}
//メンテツールの状態
MaintenanceTool::ProcessState MaintenanceTool::state() const
{
  return m_state;
}
void MaintenanceTool::setState(ProcessState state)
{
  if(m_state == state)  return;
  m_state = state;
  emit stateChanged(m_state);
}
//更新があるかの取得
bool MaintenanceTool::hasUpdate() const
{
  return m_hasUpdate;
}
void MaintenanceTool::setHasUpdate(bool hasUpdate)
{
  if(m_hasUpdate == hasUpdate)  return;
  m_hasUpdate = hasUpdate;
  emit hasUpdateChanged(m_hasUpdate);
}
//更新の詳細情報
QString MaintenanceTool::updateDetails() const
{
  return m_updateDetails;
}
void MaintenanceTool::setUpdateDetails(const QString &updateDetails)
{
  if(m_updateDetails == updateDetails) return;
  m_updateDetails = updateDetails;
  emit updateDetailsChanged(m_updateDetails);
}

//プロセスが開始
void MaintenanceTool::processStarted()
{
  setState(MaintenanceTool::Running);
}
//プロセスが終了
void MaintenanceTool::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
  qDebug() << "exitCode=" << exitCode << ", exitStatus=" << exitStatus;

  if(exitCode == 0){
    //アップデートあり

    //標準出力を取得する                                         [3]
    QByteArray stdOut = m_process.readAllStandardOutput();
    QString stdOutStr = QString::fromLocal8Bit(stdOut);
    qDebug() << "out>" << stdOutStr;
    //exitCode= 0 , exitStatus= 0
    //out> "[0] Warning: Could not create lock file 'C:/Program Files (x86)/HelloWorld/lockmyApp15021976.lock': アクセスが拒否されました。
    // (0x00000005)
    //<updates>
    //    <update version="0.3.0-1" name="The root component" size="175748"/>
    //</updates>
    //
    //"
    //err> ""

    //アップデート情報のタグ部分だけ抽出する                             [4]
    QString xmlStr;
    QStringList lines;
    bool enabled = false;
    lines = stdOutStr.split("\n");
    foreach (const QString &line, lines) {
      if(line.startsWith("<updates>")){
        enabled = true;
        xmlStr.append(line);
      }else if(line.endsWith("</updates>")){
        xmlStr.append(line);
        break;
      }else if(enabled){
        xmlStr.append(line);
      }else{
      }
    }
    //取得した情報などを設定
    if(xmlStr.length() > 0){
      setUpdateDetails(xmlStr);
      setHasUpdate(true);
    }
    //停止状態に変更
    setState(MaintenanceTool::NotRunning);

  }else if(exitCode == 1){
    //アップデートなし
    QByteArray stdErr = m_process.readAllStandardError();
    QString stdErrStr = QString::fromLocal8Bit(stdErr);
    qDebug() << "err>" << stdErrStr;

    //exitCode= 1 , exitStatus= 0
    //out> "[0] Warning: Could not create lock file 'C:/Program Files (x86)/HelloWorld/lockmyApp15021976.lock': アクセスが拒否されました。
    // (0x00000005)
    //"
    //err> "There are currently no updates available.
    //"

    //停止状態に変更
    setState(MaintenanceTool::NotRunning);
  }else{
    //停止状態に変更
    setState(MaintenanceTool::NotRunning);
  }
}
//プロセスがエラー
void MaintenanceTool::processError(QProcess::ProcessError error)
{
  qDebug() << error;
  setState(MaintenanceTool::NotRunning);
}
