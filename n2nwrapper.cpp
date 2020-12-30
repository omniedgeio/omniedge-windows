#include "n2nwrapper.h"
extern "C" {
#include "n2n/n2n.h"
}

N2NWrapper::N2NWrapper()
{

}

int N2NWrapper::startEdge(char *device_name, char *community_name,
                           char *encrypt_key, char *device_mac,
                           char *local_ip_address,
                           char *supernode_ip_address_port,
                           int *keep_on_running)
{
      tuntap_dev tuntap;
      n2n_edge_t *eee;
      n2n_edge_conf_t conf;
      int rv;
      QString masknet="255.255.255.0";
      /* Setup the configuration */
      edge_init_conf_defaults(&conf);
      conf.encrypt_key = encrypt_key;
      conf.transop_id = N2N_TRANSFORM_ID_TWOFISH;
      snprintf((char*)conf.community_name, sizeof(conf.community_name), "%s", community_name);
      edge_conf_add_supernode(&conf, supernode_ip_address_port);

      /* Validate configuration */
      if(edge_verify_conf(&conf) != 0)
        return(-1);

      /* Open the tuntap device */
      if(tuntap_open(&tuntap, device_name, "static",
             local_ip_address, masknet.toLatin1().data(),
             device_mac, DEFAULT_MTU) < 0)
        return(-2);

      /* Init edge */
      if((eee = edge_init(&tuntap, &conf, &rv)) == NULL)
        goto quick_edge_init_end;

      rv = run_edge_loop(eee, keep_on_running);
      edge_term(eee);

    quick_edge_init_end:
      tuntap_close(&tuntap);
      return(rv);

}
