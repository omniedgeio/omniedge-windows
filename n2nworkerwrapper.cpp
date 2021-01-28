#include "n2nworkerwrapper.h"
#include <QtDebug>
N2NWorkerWrapper::N2NWorkerWrapper()
{
    n2nWorker = new N2NWorker(&keep_on_running);
    n2nThread = new QThread;

    connect(n2nWorker, &N2NWorker::configError, this, &N2NWorkerWrapper::configError);
    connect(n2nWorker, &N2NWorker::wintapError, this, &N2NWorkerWrapper::wintapError);
    connect(n2nWorker, &N2NWorker::wintapError, this, &N2NWorkerWrapper::wintapError);
    connect(n2nWorker, &N2NWorker::wintapError, this, &N2NWorkerWrapper::wintapError);
    connect(this, &N2NWorkerWrapper::startEdgeSignal, n2nWorker, &N2NWorker::startEdge);
    connect(this, &N2NWorkerWrapper::setVirtualIpSignal, n2nWorker, &N2NWorker::setVirtualIp);

    n2nWorker->moveToThread(n2nThread);
    n2nThread->start();
}

void N2NWorkerWrapper::setVirtualIp(QString virtual_ip, QString encryption)
{
    emit setVirtualIpSignal(virtual_ip, encryption);
}

void N2NWorkerWrapper::startEdge(QString addr, QString community_name, QString encrypt_key)
{
    qDebug("starting edge.");
    emit startEdgeSignal(addr, community_name, encrypt_key);
}

void N2NWorkerWrapper::stopEdge()
{
    qDebug("stopping edge.");
    emit stopEdgeSignal();
    keep_on_running = 0;
}
