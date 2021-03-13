#ifndef N2NWORKER_H
#define N2NWORKER_H

#include <QObject>

enum N2NWorkerError {
    WINTAP,
    CONFIG,
    UNKNOWN
};

extern "C" {
#include "n2n/n2n.h"
#include "n2n/n2n_transforms.h"
}

class N2NWorker : public QObject
{
    Q_OBJECT

public:
    explicit N2NWorker(int* keep_on_running, QObject *parent = nullptr);

public slots:
    void connect(
        QString supernode_addr,
        QString community_name,
        QString secret_key,
        QString virtual_ip
    );

signals:
    void error(N2NWorkerError error);
    void connected();
    void disconnected();

private:
    int* keep_on_running;
};

#endif // N2NWORKER_H
