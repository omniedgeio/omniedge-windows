#ifndef EDGENODE_H
#define EDGENODE_H

#include <QObject>
#include "n2n.h"
class EdgeNode
{
private:
    #define N2N_NETMASK_STR_SIZE    16 /* dotted decimal 12 numbers + 3 dots */
    #define N2N_MACNAMSIZ           18 /* AA:BB:CC:DD:EE:FF + NULL*/
    #define N2N_IF_MODE_SIZE        16 /* static | dhcp */

    /* *************************************************** */

    /** maximum length of command line arguments */
    #define MAX_CMDLINE_BUFFER_LENGTH    4096

    /** maximum length of a line in the configuration file */
    #define MAX_CONFFILE_LINE_LENGTH     1024

    /* ***************************************************** */

    typedef struct {
      int     local_port;
      int     mgmt_port;
      char    tuntap_dev_name[N2N_IFNAMSIZ];
      char    ip_mode[N2N_IF_MODE_SIZE];
      char    ip_addr[N2N_NETMASK_STR_SIZE];
      char    netmask[N2N_NETMASK_STR_SIZE];
      int     mtu;
      int     got_s;
      char    device_mac[N2N_MACNAMSIZ];
      char *  encrypt_key;
    #ifndef WIN32
      uid_t   userid;
      gid_t   groupid;
    #endif
    } edge_conf_t;
public:
    EdgeNode();
    int EdgeNodeRun();
};

#endif // EDGENODE_H
