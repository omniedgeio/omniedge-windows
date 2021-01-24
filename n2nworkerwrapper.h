#ifndef N2NWORKERWRAPPER_H
#define N2NWORKERWRAPPER_H

#include <QObject>
#include <QThread>

#include "n2nworker.h"

class N2NWorkerWrapper : public QObject
{
    Q_OBJECT

public:
    N2NWorkerWrapper();

signals:
    void configError();
    void wintapError();
    void connected();
    void disconnected();
    void startEdgeSignal(QString community_name, QString encrypt_key);
    void setVirtualIpSignal(QString virtual_ip, QString encryption);

public:
    void setVirtualIp(QString virtual_ip, QString encryption);
    void startEdge(QString community_name, QString encrypt_key);
    void stopEdge();

private:
    N2NWorker* n2nWorker;
    QThread* n2nThread;
    int keep_on_running = 1;
};

#endif // N2NWORKERWRAPPER_H
