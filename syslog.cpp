#include "syslog.h"
#include <stdio.h>
#include <stdlib.h>
#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include <QTime>
#include <QDir>

#define TIME qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
static FILE *q_fileStream = nullptr;
static int   g_logLevel   = 0;

enum TraceLevel
{
    TL_NULL = 0,
    TL_CRIT,
    TL_ERRO,
    TL_WARN,
    TL_INFO
};

static const int g_TraceLevel [] = { TL_INFO, TL_WARN, TL_ERRO, TL_CRIT, TL_NULL };

/*
 * Constructor of syslog class
 *
 * @Parameters:
 *    parent: default is nullptr
 *
 * @Retvals:
 *    N/A
 */
syslog::syslog(QObject *parent) : QObject(parent)
{

}

/*
 * Destructor of syslog class
 *
 * @Parameters:
 *    N/A
 *
 * @Retvals:
 *    N/A
 */
syslog::~syslog()
{

}

/*
 * Close the log file
 *
 * @Parameters:
 *    N/A
 *
 * @Retvals:
 *    N/A
 */
 void syslog::SoftWareShutDown()
{
    if (nullptr != q_fileStream)
    {
        qDebug("Close log file.");
        fclose (q_fileStream);
        q_fileStream = nullptr;
    }
}

 /*
  * Prints out debug messages, warnings, critical and fatal error messages
  *
  * @Parameters:
  *    type: enum describes the messages that can be sent to a message handler
  *    context: additional information about a log message
  *    msg: message information
  *
  * @Retvals:
  *    N/A
  */
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type);
    Q_UNUSED(context);
    QByteArray localMsg = msg.toLocal8Bit();
    if (g_logLevel && q_fileStream)
    {
        int iLevel = g_TraceLevel[type];
        if (g_logLevel < iLevel)
        {
            return;
        }
        fprintf(q_fileStream, "%s  %s \n", TIME, localMsg.constData());
        fflush(q_fileStream);
    }
}

/*
 * Installs a Qt message handler with log file
 *
 * @Parameters:
 *    strLogFile: log file
 *
 * @Retvals:
 *    N/A
 */
 void installMsgHandler(QString strLogFile)
{
    QByteArray byteArrayLogFile = strLogFile.toLatin1();

    if (nullptr != q_fileStream)
    {
        fclose (q_fileStream);
        q_fileStream = nullptr;
    }

    q_fileStream = fopen (byteArrayLogFile.constData(), "a+");
    if (nullptr != q_fileStream)
    {
        qDebug("Opened log file.");
        qInstallMessageHandler(myMessageOutput);

    }
    else
    {
        qDebug("Failed to open log file.");
    }
}

 /*
  * Installs a Qt message handler when released
  *
  * @Parameters:
  *    N/A
  *
  * @Retvals:
  *    N/A
  */
void syslog::installReleaseMsgHandler()
{
    QString strAppPath = QCoreApplication::applicationDirPath();
    QString strIniFile = strAppPath + "/RunInfo.ini";
    QString strLogFile;
    strLogFile =QString("%1/Omniedge.log").arg(strAppPath);
    QSettings pSettings(strIniFile, QSettings::IniFormat);
    g_logLevel = pSettings.value("TraceSet/TraceLevel", 4).toInt();
    installMsgHandler(strLogFile);
}
