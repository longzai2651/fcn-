#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/ioctl.h>

#include <arpa/inet.h>
#include <unistd.h>

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

static int test_p2p(int server_ip, int id, int connect_id, int flag)
{
	nat_echo_req req;
	nat_echo_ack ack;
	struct sockaddr_in addr = {0};
	char buf[64];
	int ret, sock;
	int ttl; /* max = 255 */

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);
	addr.sin_addr.s_addr = server_ip;
	/* 1. 注册ＩＤ */
	req.type = P2P_C_REG;
	req.id = id;

reg_id:

	sendto(sock, &req, sizeof(req), 0, (struct sockaddr *)&addr, sizeof(addr));
	ret = udp_recv(sock, &ack, sizeof(ack), NULL, 1000);
	if (ret <= 0) goto err_exit;

	printf("1. %d %x:%d %x\n", ack.sid, ntohl(ack.ip), ntohs(ack.port), ack.type);

	/* 2. 请求p2p连接 */
	if (connect_id > 0) {
		req.type = P2P_C_CONN;
		req.id = id;
		req.data = connect_id;
		req.flag = flag;

		sendto(sock, &req, sizeof(req), 0, (struct sockaddr *)&addr, sizeof(addr));
	}

	/* 3. 接受到ＰＥＥＲ方　端口ＩＰ信息 */
	ret = udp_recv(sock, &ack, sizeof(ack), NULL, 1000);
	if (ret <= 0) {
		if (!connect_id)
			goto reg_id;
		else
			goto err_exit;
	}

	/* 4. 向对方PORT发出ＵＤＰ数据包　*/
	req.type = P2P_C_HELLO;
	req.data = 0;

	addr.sin_addr.s_addr = ack.ip;
	/* ack.flag -> 0-3 */
	if (connect_id > 0)
		addr.sin_port = htons(ntohs(ack.port) + (ack.flag % 2));
	else
		addr.sin_port = htons(ntohs(ack.port) + (ack.flag / 2));

	if (!connect_id)
		usleep(100*1000);

	printf("2. %d %d -> %x:%d %x\n", ack.sid, ack.flag, ntohl(ack.ip), ntohs(addr.sin_port), ack.type);
	sendto(sock, &req, sizeof(req), 0, (struct sockaddr *)&addr, sizeof(addr));

	ret = udp_recv(sock, buf, sizeof(buf), &addr, 2000);
	if (ret <= 0) goto err_exit;

	printf("3. [%d] %d bytes  [%x %x]\n", ntohs(addr.sin_port), ret, *(int*)buf, *(int*)(buf+4));
	if (*(int*)buf == P2P_C_HELLO) {
		sendto(sock, &req, sizeof(req), 0, (struct sockaddr *)&addr, sizeof(addr));
		close(sock);
		return 0;
	}

err_exit:
	close(sock);
	return -1;
}

int main(int argc, char** argv)
{
	int i, id, dst = 0;
	int host = inet_addr(argv[1]);
	// ip sid [dst_sid]

	id = atoi(argv[2]);
	if (argc > 3)
		dst = atoi(argv[3]);

	if (dst > 0) {
		while(1)
		{
			for (i=0; i<4; i++) {
				if (test_p2p(host, id, dst, i) == 0) {
					return 0;
				}
			}
		}

	} else {
		while (test_p2p(host, id, dst, 0));
	}

    return 0;
}
