#include "edgenode.h"

EdgeNode::EdgeNode()
{

}

int EdgeNode::EdgeNodeRun()
{

      int     keep_on_running = 1;
      int     rc;
      int     i;
      n2n_edge_t eee; /* single instance for this program */
      edge_conf_t ec;


      ec.local_port = 0 /* any port */;
      ec.mgmt_port = N2N_EDGE_MGMT_PORT; /* 5644 by default */
      snprintf(ec.tuntap_dev_name, sizeof(ec.tuntap_dev_name), "edge0");
      snprintf(ec.ip_mode, sizeof(ec.ip_mode), "static");
      snprintf(ec.netmask, sizeof(ec.netmask), "255.255.255.0");
      ec.ip_addr[0] = '\0';
      ec.device_mac[0] = '\0';
      ec.mtu = DEFAULT_MTU;
      ec.got_s = 0;
      ec.encrypt_key = NULL;


      if(-1 == edge_init(&eee)) {
        //traceEvent(TRACE_ERROR, "Failed in edge_init");
        exit(1);
      }

      if(getenv("N2N_KEY")) {
        ec.encrypt_key = strdup(getenv("N2N_KEY"));
      }

    #ifdef WIN32
      ec.tuntap_dev_name[0] = '\0';
    #endif
      memset(&(eee.supernode), 0, sizeof(eee.supernode));
      eee.supernode.family = AF_INET;


      //rc = loadFromCLI(argc, argv, &ec, &eee);

//      if((rc < 0) || (eee.sn_num == 0))
//        help();

      //traceEvent(TRACE_NORMAL, "Starting n2n edge %s %s", n2n_sw_version, n2n_sw_buildDate);

      for (i=0; i<eee.sn_num; ++i)
        //traceEvent(TRACE_NORMAL, "supernode %u => %s\n", i, (eee.sn_ip_array[i]));

      supernode2addr(&(eee.supernode), eee.sn_ip_array[eee.sn_idx]);

      if(!((eee.community_name[0] != 0) &&
           (ec.ip_addr[0] != 0)
           ))
        {
          //help();
        }

      if((NULL == ec.encrypt_key) && (0 == strlen(eee.keyschedule)))
        {
          //traceEvent(TRACE_WARNING, "Encryption is disabled in edge.");

          eee.null_transop = 1;
        }

      if(0 == strcmp("dhcp", ec.ip_mode)) {
        //traceEvent(TRACE_NORMAL, "Dynamic IP address assignment enabled.");

        eee.dyn_ip_mode = 1;
      } else
        //traceEvent(TRACE_NORMAL, "ip_mode='%s'", ec.ip_mode);

      if(tuntap_open(&(eee.device), ec.tuntap_dev_name, ec.ip_mode, ec.ip_addr, ec.netmask, ec.device_mac, ec.mtu) < 0)
        return(-1);

      if(ec.local_port > 0)
        //traceEvent(TRACE_NORMAL, "Binding to local port %d", (signed int)ec.local_port);

      if(ec.encrypt_key) {
        if(edge_init_twofish(&eee, (uint8_t *)(ec.encrypt_key), strlen(ec.encrypt_key)) < 0) {
          fprintf(stderr, "Error: twofish setup failed.\n");
          return(-1);
        }
      } else if(strlen(eee.keyschedule) > 0) {
        if(edge_init_keyschedule(&eee) != 0) {
          fprintf(stderr, "Error: keyschedule setup failed.\n");
          return(-1);
        }
      }
      /* else run in NULL mode */

      eee.udp_sock = open_socket(ec.local_port, 1 /* bind ANY */);
      if(eee.udp_sock < 0) {
        //traceEvent(TRACE_ERROR, "Failed to bind main UDP port %u", (signed int)ec.local_port);
        return(-1);
      }

      eee.udp_mgmt_sock = open_socket(ec.mgmt_port, 0 /* bind LOOPBACK */);

      if(eee.udp_mgmt_sock < 0) {
        //traceEvent(TRACE_ERROR, "Failed to bind management UDP port %u", ec.mgmt_port);
        return(-1);
      }

      //traceEvent(TRACE_NORMAL, "edge started");

      update_supernode_reg(&eee, time(NULL));

      return run_edge_loop(&eee, &keep_on_running);

}
