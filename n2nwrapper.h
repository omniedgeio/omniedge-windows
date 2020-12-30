#ifndef N2NWRAPPER_H
#define N2NWRAPPER_H

#include <QObject>

class N2NWrapper
{
public:
    N2NWrapper();
    int startEdge(char *device_name, char *community_name,
                               char *encrypt_key, char *device_mac,
                               char *local_ip_address,
                               char *supernode_ip_address_port,
                               int *keep_on_running);
};

#endif // N2NWRAPPER_H
