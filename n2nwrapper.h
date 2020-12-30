#ifndef N2NWRAPPER_H
#define N2NWRAPPER_H

#include <QObject>
#include <QString>
#include <QThread>

extern "C" {
#include "n2n/n2n_transforms.h"
}
#define SUPERNODE_ADDR QString("52.80.139.238:7787")

class N2NWrapper : public QObject
{
    Q_OBJECT

public:
    explicit N2NWrapper();
    virtual ~N2NWrapper();

signals:
    void configError();
    void wintapError();
    void connected();
    void disconnected();

public slots:
    void setVirtualIp(QString virtual_ip, QString encryption);
    void startEdge(QString community_name, QString encrypt_key);

private:
    QThread* n2nThread;
    QString virtual_ip;
    n2n_transform_t transop_id;
};

#endif // N2NWRAPPER_H
