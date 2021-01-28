#ifndef N2NWORKER_H
#define N2NWORKER_H

#include <QObject>
#include <QString>
#include <QThread>

extern "C" {
#include "n2n/n2n.h"
#include "n2n/n2n_transforms.h"
}

class N2NWorker : public QObject
{
    Q_OBJECT

public:
    explicit N2NWorker(int* keep_on_running);
    virtual ~N2NWorker();
    void setVirtualIp(QString virtual_ip, QString encryption);
    void startEdge(QString addr, QString community_name, QString encrypt_key);
    void stopEdge();

signals:
    void configError();
    void wintapError();
    void connected();
    void disconnected();

private:
    QString virtual_ip;
    tuntap_dev tuntap;
    n2n_edge_t *eee;
    n2n_edge_conf_t conf;
    n2n_transform_t transop_id;
    int* keep_on_running;
};

#endif // N2NWORKER_H
