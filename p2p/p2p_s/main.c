#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/ioctl.h>

#include <arpa/inet.h>
#include "../p2p.h"

typedef struct {
	unsigned short port;
	unsigned int ip;
    int sid;
    int sock;

    struct sockaddr_in addr;
} peer_info;

peer_info peer_list[32];

static peer_info* sid_2_peer(int sid)
{
	int i;
    for (i=0; i<32; i++) {
        if (peer_list[i].sid == sid)
			return peer_list + i;
    }
    return NULL;
}

static int send_peer_info(peer_info* dst, unsigned short t, int flag, peer_info* info)
{
	socklen_t len = sizeof(struct sockaddr_in);

	nat_echo_ack nat_echo;

    nat_echo.ip = info->ip;
    nat_echo.port = info->port;
    nat_echo.sid = info->sid;
    nat_echo.type = t;
    nat_echo.flag = flag;

	return sendto(dst->sock, &nat_echo, sizeof(nat_echo), 0,
				(struct sockaddr*)&dst->addr, len);
}

int main(int argc, char** argv)
{
	int i, opt = 1;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	char buf[1024];
	struct sockaddr_in l_addr = {0};
	peer_info *peer;

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

	//listen 6000 echo server
    l_addr.sin_family = AF_INET;
    l_addr.sin_addr.s_addr = INADDR_ANY;
    if (argc > 1)
		l_addr.sin_port = htons(atoi(argv[1]));
	else
		l_addr.sin_port = htons(6000);

    /* Bind */
    if (bind(sock, (struct sockaddr *)&l_addr, sizeof(struct sockaddr)) < 0) {
        close(sock);
        return -1;
    }

    //ioctl(sock, FIONBIO, &opt);

	memset(peer_list, 0, sizeof(peer_list));

    while (1) {
		struct sockaddr_in addr = {0};
		socklen_t len = sizeof(addr);
        int n;

		n = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&addr, &len);

        printf("udp recv %d\n", n);
		if (n == sizeof(nat_echo_req)){
			nat_echo_req * req = (nat_echo_req*)buf;
			if (req->type == P2P_C_REG) {
				//record it
				peer = sid_2_peer(req->id);
                if (!peer) {
                    for (i=0; i<32; i++) {
                        if (!peer_list[i].sid) {
                            peer = peer_list + i;
                            break;
                        }
                    }
                }

                if (peer) {
                    peer->sid = req->id;
                    peer->ip = addr.sin_addr.s_addr;
                    peer->port = addr.sin_port;
					peer->addr = addr;
					peer->sock = sock;

					send_peer_info(peer, P2P_S_INFO, 0, peer);
                }

			} else if (req->type == P2P_C_QUERY) {
				peer_info * peer_info;
				peer = sid_2_peer(req->id);
				peer_info = sid_2_peer(req->data);
				if (peer) {
					printf("query:%x ok\n", req->id);
					send_peer_info(peer, P2P_S_INFO, 0, peer_info);
                }
			} else if (req->type == P2P_C_CONN) {
				peer_info * dst_peer;
				peer = sid_2_peer(req->id);
				dst_peer = sid_2_peer(req->data);
				if (dst_peer && peer) {
					send_peer_info(dst_peer, P2P_C_CONN, req->flag, peer);
					send_peer_info(peer, P2P_C_CONN, req->flag, dst_peer);
				}
			}
		}

    }

    return 0;
}
