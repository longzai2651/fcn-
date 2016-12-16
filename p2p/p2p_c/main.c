#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/ioctl.h>

#include <arpa/inet.h>

#include "../p2p.h"

static int udp_recv(int sock, void* buf, int len, struct sockaddr_in* addr, int timeout)
{
    fd_set fds;
    struct timeval tv;
    socklen_t slen = sizeof(struct sockaddr_in);
	int ret ;

    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    FD_ZERO(&fds);
    FD_SET(sock, &fds);

    ret = select(sock + 1, &fds, NULL, NULL, &tv);
    if (!FD_ISSET(sock, &fds))
		return 0;

	ret = recvfrom(sock, buf, len, 0, (struct sockaddr *)addr, &slen);
    return ret;
}

int main(int argc, char** argv)
{
	struct sockaddr_in addr = {0};
    int ret, id, sock = -1, dst = 0;
	nat_echo_req req;
	nat_echo_ack ack;
	char buf[64];

	// ip sid [dst_sid]

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	addr.sin_family = AF_INET;

	id = atoi(argv[2]);
	if (argc > 3)
		dst = atoi(argv[3]);

	while (1) {
reg_id:
		/* 1. 注册ＩＤ */
		req.type = P2P_C_REG;
		req.id = id;

		addr.sin_port = htons(6000);
		addr.sin_addr.s_addr = inet_addr(argv[1]);

		sendto(sock, &req, sizeof(req), 0, (struct sockaddr *)&addr, sizeof(addr));
		ret = udp_recv(sock, &ack, sizeof(ack), NULL, 1000);
		if (ret <= 0) continue;

		printf("1. %d %x:%d %x\n", ack.sid, ntohl(ack.ip), ntohs(ack.port), ack.type);

        if (dst > 0){
			/* 2. 请求p2p连接 */
			req.type = P2P_C_CONN;
			req.id = id;
			req.data = dst;	//target_id
			sendto(sock, &req, sizeof(req), 0, (struct sockaddr *)&addr, sizeof(addr));
        }

		/* 3. 接受到ＰＥＥＲ方　端口ＩＰ信息 */
		ret = udp_recv(sock, &ack, sizeof(ack), NULL, 1000);
		if (ret <= 0)
			continue;

		printf("2. %d %x:%d %x\n", ack.sid, ntohl(ack.ip), ntohs(ack.port), ack.type);

        /* 4. 依次向对方PORT, PORT+1, PORT+3 发出ＵＤＰ数据包　*/
        req.type = P2P_C_HELLO;
		req.data = 0;

        addr.sin_addr.s_addr = ack.ip;
        addr.sin_port = htons(ntohs(ack.port));

		sendto(sock, &req, sizeof(req), 0, (struct sockaddr *)&addr, sizeof(addr));

        addr.sin_port = htons(ntohs(ack.port)+1);
		sendto(sock, &req, sizeof(req), 0, (struct sockaddr *)&addr, sizeof(addr));

        addr.sin_port = htons(ntohs(ack.port)+3);
		sendto(sock, &req, sizeof(req), 0, (struct sockaddr *)&addr, sizeof(addr));

		/* 5. 接受ＵＤＰ穿透后的数据包 */
		ret = udp_recv(sock, buf, sizeof(buf), &addr, 2000);

        if (ret > 0 && *(int*)buf == P2P_C_HELLO) {

			printf("3. [%d] %d bytes [%x %d]\n", ntohs(addr.sin_port), ret, *(int*)buf, *(int*)(buf+4));

        	//ttl = 64;
			//setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
			sendto(sock, &req, sizeof(req), 0, (struct sockaddr *)&addr, sizeof(addr));
			break;
        }

		//new sock
        close(sock);
        sock = socket(AF_INET, SOCK_DGRAM, 0);
	}

    return 0;
}
