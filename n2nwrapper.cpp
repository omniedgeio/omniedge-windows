#include "n2nwrapper.h"
extern "C" {
#include "n2n/n2n.h"
}

N2NWrapper::N2NWrapper()
{
    n2nThread = new QThread();
    moveToThread(n2nThread);
    n2nThread->start();
}

void N2NWrapper::setVirtualIp(QString virtual_ip, QString encryption)
{
    this->virtual_ip = virtual_ip;
    if(encryption == "twofish"){
        this->transop_id = N2N_TRANSFORM_ID_TWOFISH;
    }else{
        this->transop_id = N2N_TRANSFORM_ID_NULL;
    }
}

void N2NWrapper::startEdge(QString community_name, QString encrypt_key)
{

      this->setVirtualIp("10.254.1.8", "twofish");
      tuntap_dev tuntap;
      n2n_edge_t *eee;
      n2n_edge_conf_t conf;
      QByteArray community = community_name.toLatin1();
      QByteArray secret = encrypt_key.toLatin1();
      int rv;
      int keep_on_running = 1;

      /* Setup the configuration */
      edge_init_conf_defaults(&conf);
      conf.encrypt_key = secret.data();
      conf.transop_id = N2N_TRANSFORM_ID_TWOFISH;
      snprintf((char*)conf.community_name, sizeof(conf.community_name), "%s", community.data());
      edge_conf_add_supernode(&conf, SUPERNODE_ADDR.toLatin1().data());

      /* Validate configuration */
      if(edge_verify_conf(&conf) != 0){
        emit configError();
      }

      /* Open the tuntap device */
      if(tuntap_open(&tuntap,
                     (char*) "",
                     "static",
                     this->virtual_ip.toLatin1().data(),
                     (char*) "255.255.255.0",
                     (char*) "",
                     DEFAULT_MTU) < 0){
          emit wintapError();
      }

      /* Init edge */
      if((eee = edge_init(&tuntap, &conf, &rv)) == NULL)
        goto quick_edge_init_end;

      emit connected();
      rv = run_edge_loop(eee, &keep_on_running);
      edge_term(eee);

      quick_edge_init_end:
        emit disconnected();
        tuntap_close(&tuntap);
}

N2NWrapper::~N2NWrapper()
{
    n2nThread->wait();
    n2nThread->quit();
}
