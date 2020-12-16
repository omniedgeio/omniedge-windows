#include "edge.h"
#include <QtDebug>
#include <QTimer>

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
    QString StandardOutput = edge_process->readAllStandardOutput();
    QString StandardError = edge_process->readAllStandardError();
    //qDebug() << StandardOutput;
    for(QString line : StandardOutput.split("\r\n")){
        QString command = line.split("<>")[0];
        QStringList params = line.replace(command + "<>", "").split("<>");
        if (command == "ADDING_SUPERNODE") {
            int index = params[0].toInt();
            QString ip = params[1];
            if(index >= this->supernodes.size()){
                this->supernodes.resize(index + 1);
            }
            this->supernodes[index] = ip;
        }
        else if (command == "ENCRYPT_KEY") {
            QString encrypt_key = params[0];
            if (this->password != encrypt_key) {
                this->kill();
                emit fatalError("Given password not same as n2n collected");
            }
        }
        else if (command == "STARTING_N2N") {
            QString version = params[0];
            QString build_date = params[1];
            emit info("Using n2n " + version + " built on " + build_date);
        }
        else if (command == "REQUIRED_TAPINSTALL_EXE") {
            this->kill();
            if(retry_count){
                emit fatalError("Tap needed, Please install it from openvpn website");
            } else {
                connectSuperNode(this->server_ip, this->port, this->community_name, this->password, this->tap_ip);
                retry_count++;
            }

        }
        else if (command == "IP_MODE") {
            this->ip_mode = params[0];
        }
        else if (command == "SET_DEVICE_IP_FAILED") {
            QString interface = params[0];
            QString error = params[1];
            this->kill();
            emit fatalError("Set device ip failed, please check for administrator permission.");
        }
        else if (command == "SUPERNODE") {
            int index = params[0].toInt();
            QString supernode = params[1];
            this->supernodes[index] = supernode;
            this->current_supernode = this->supernodes[index];
        }
        else if (command == "STARTED") {
            emit replyConnectStatus(ConnectStatus(Success));
            this->status = ConnectStatus(Success);
            this->ping("10.254.1.1");
            this->ping("10.254.1.2");
            this->ping("10.254.1.3");
            this->ping("10.254.1.4");
            this->ping("10.254.1.5");
            this->ping("10.254.1.6");
            this->ping("10.254.1.7");
            this->ping("10.254.1.8");
            this->ping("10.254.1.9");
        }
        qDebug() << command;
        qDebug() << params;
    }
}

void Edge::connectSuperNode(QString server_ip, QString port, QString community_name, QString password, QString tap_ip)
{
    this->server_ip = server_ip;
    this->port = port;
    this->community_name = community_name;
    this->password = password;
    this->tap_ip = tap_ip;
    QString command = " -l " + server_ip + ":" + port + " -c " + community_name + " -k " + password + " -a " + tap_ip;
    this->edge_process->start("edge.exe" + command);
    if(this->edge_process->state() == QProcess::NotRunning){
       emit fatalError("Edge not running for unexpected reason.");
    } else {
        this->status = ConnectStatus(Connecting);
        QTimer::singleShot(60 * 1000, this, SLOT(checkTimeout()));
    }
}

void Edge::kill()
{
    QProcess q;
    q.start("taskkill /im edge.exe /f");
    q.waitForFinished();
    this->edge_process->close();
    qDebug() << "FAILED";
    this->status = ConnectStatus(Fail);
    emit replyConnectStatus(ConnectStatus(Fail));
}

void Edge::checkTimeout()
{
    qDebug() << this->status;
    if(this->status == ConnectStatus(Connecting)){
        this->kill();
        emit replyConnectStatus(ConnectStatus(Timeout));
    }
}

void Edge::ping(QString ip){
    QProcess* ping_process = new QProcess(this);
    ping_process->start("ping " + ip + " -t");
    connect(ping_process, &QProcess::readyReadStandardOutput,
        [=](){
        QString output = ping_process->readAllStandardOutput();
        if(output.contains("<"))// Reply from 10.254.1.2: bytes=32 time<1ms TTL=128
        {
            int ms = output.remove(0,output.indexOf("<") + 1).split("ms ")[0].toInt();
            emit replyPingMs(ip, ms);
        }
        else if (output.contains("="))
        {
            output.remove(0,output.indexOf("=") + 1);
            output.remove(0,output.indexOf("=") + 1);
            int ms = output.split("ms ")[0].toInt();
            emit replyPingMs(ip, ms);
        }
        else
        {
            emit replyPingMs(ip, -1);
            //ping_process->kill();
        }
    });
}

Edge::~Edge()
{
    this->kill();
}
