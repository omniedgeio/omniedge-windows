#include "edge.h"
#include <QtDebug>

Edge::Edge(QObject *parent) : QObject(parent)
{
    edge_process = new QProcess(this);
    edge_process->setReadChannel(QProcess::StandardOutput);
    edge_process->setReadChannel(QProcess::StandardError);
    connect(edge_process, SIGNAL(readyReadStandardOutput()), this, SLOT(output()));
    connect(edge_process, SIGNAL(readyReadStandardError()), this, SLOT(output()));
}


void Edge::output(void)
{
    QByteArray StandardOutput = edge_process->readAllStandardOutput();
    QByteArray StandardError = edge_process->readAllStandardError();

    qDebug() << "edge: " << StandardOutput;
    qDebug() << "edge: " << StandardError;
    if(StandardOutput.indexOf("Edge Peer <<< ================ >>> Super Node")>=0)
    {
        qDebug() << "connect SN successfully.";
        replyConnectStatus(Success);
    }
}

void Edge::connectSuperNode(QString server_ip, QString port, QString community_name, QString password, QString internal_ip)
{
    QString command = " -l" + server_ip + ":" + port + " -c " + community_name + " -k " + password + " -a " + internal_ip ;
    this->edge_process->start("edge.exe" + command);
    if(this->edge_process->state() == QProcess::NotRunning){
        qDebug() << "edge.exe not running";
    }
}
