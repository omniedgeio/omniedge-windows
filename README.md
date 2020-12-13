# omniedge-windows

```
Example: ADDING_SUPERNODE<>0<>XXX.XXX.XXX.XXX:XXXX
<> will be the seperator.

STATUS CODE                Parameters
---------------------------------------------------------
[edge.c]
Set option
ENCRYPT_KEY                encrypt_key
ETHERNET_MULTICAST_ENABLED
MULTIPLE_SUBNET_SUPPLIED
TOO_MANY_SUPERNODE
UNKNOWN_OPTION             option_key

Config file
CONFIG_FILE_NOT_FOUND      path
SKIPPING_UNRECOGNIZED_LINE

Daemonize
PARENT_PROCESS_EXITING
DAEMONIZING_ERROR          error_number
ERROR_MOVING_DIRECTORY_/

Term handler
LEAVING
SHUTDOWN

Main
STARTING_N2N                      version build_date
DYNAMIC_IP_ENABLED
IP_MODE                           mode[static|dynamic]
COMMUNITY_AND_ENCRYPTION_KEY_SAME
FAILED
STARTED

[edge_utils.c]
edge_init
INVALID_CONFIGURATION
ALLOCATE_MEMORY_FAILED
LZO_COMPRESSION_ERR
SUPERNODE                     index ip
TRANSOP_INIT_FAILED
EDGE_DISABLED_ENCRYPTION
SOCKET_SETUP_FAILED
BAD_SUPERNODE_PARAMS          addr host port
RESOLVE_SUPERNODE_IPV4_FAILED host
RESOLVE_SUPENODE_HOST_FAILED  HOST
WRONG_SUPERNODE_PARAMS

check_join_multicast_group
BIND_LOCAL_MULTICAST_GROUP_FAILED group port error_number
JOIN_MULTICAST_GROUP_SUCCESS      group port

update_supernode_reg
SUPERNODE_NOT_RESPONDING retry_supernode_ip

readFromTAPSocket
TAP_READ len errno strerror

readFromIPSocket
UNKNOWN_PACKET_TYPE      msg_type
UNKNOWN_COMMUNITY_PACKET

send_register
SEND_REGISTER     sock_to_cstr(sockbuf, remote_peer)

send_register_ack
SEND_REGISTER_ACK sock_to_cstr(sockbuf, remote_peer)

edge_conf_add_supernode
ADDING_SUPERNODE           index ip port

[wintap.c]
initWin32
INITIALISE_WINSOCK_2_X_FAILED

iterate_win_network_adapters
READ_REGISTRY_FAILED err

open_wintap
REQUIRED_TAPINSTALL_EXE
TAP_DEVICE_MISSING           devname
GET_MAC_ADDR_FROM_TAP_FAILED name ifName
OPEN_DEVICE                  {uid} tap_ip tap_name mtu mac_address
ENABLE_TAP_FAILED
SET_DEVICE_IP_FAILED         ifName cmd

tuntap_read
TUNTAP_ERROR last_err

```