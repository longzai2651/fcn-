#ifndef NAT_P2P_H_INCLUDED
#define NAT_P2P_H_INCLUDED

typedef struct {
    unsigned short type;
    unsigned short port;
	int sid;
	int ip;
	int flag;
} nat_echo_ack;

#define P2P_S_INFO		0x1235
#define P2P_S_CONN		0x1236

#define P2P_C_REG		0xabcd
#define P2P_C_QUERY		0xabc0
#define P2P_C_CONN		0xabc1
#define P2P_C_HELLO		0xabc2

typedef struct {
	int type;
    int id;
    int flag;
    int data;
} nat_echo_req;

#endif // NAT_P2P_H_INCLUDED
