#include "n2nworker.h"
#include <QDebug>

N2NWorker::N2NWorker(int* keep_on_running, QObject *parent) : QObject(parent)
{
    this->keep_on_running = keep_on_running;
}

void N2NWorker::connect(
        QString supernode_addr,
        QString community_name,
        QString secret_key,
        QString subnet,
        QString virtual_ip
)
{
    QByteArray ip = virtual_ip.toLatin1();
    QByteArray secret = secret_key.toLatin1();
    QByteArray addr = supernode_addr.toLatin1();
    QByteArray subnet_mask = subnet.toLatin1();
    QByteArray community = community_name.toLatin1();

    int rv;

    /* Setup the configuration */
    n2n_edge_t *eee;
    tuntap_dev tuntap;
    n2n_edge_conf_t conf;
    edge_init_conf_defaults(&conf);
    conf.encrypt_key = secret.data();
    conf.transop_id = N2N_TRANSFORM_ID_TWOFISH;
    snprintf((char*)conf.community_name, sizeof(conf.community_name), "%s", community.data());
    edge_conf_add_supernode(&conf, addr.data());
    qDebug() << "N2NWorker: Finish setup configuration.";

    /* Validate configuration */
    if(edge_verify_conf(&conf) != 0){
        qDebug() << "N2NWorker: Configuration error.";
        emit error(N2NWorkerError::CONFIG);
        return;
    }

    if(tuntap_open(&tuntap,
                   (char*) "",
                   "static",
                   ip.data(),
                   subnet_mask.data(),
                   (char*) "",
                   DEFAULT_MTU) < 0){
        emit error(N2NWorkerError::WINTAP);
        emit disconnected();
        return;
    }

    /* Init edge */
    if((eee = edge_init(&tuntap, &conf, &rv)) == NULL)
        goto quick_edge_init_end;

    emit connected();
    rv = run_edge_loop(eee, keep_on_running);
    edge_term(eee);

    quick_edge_init_end:
        emit disconnected();
        tuntap_close(&tuntap);
}
