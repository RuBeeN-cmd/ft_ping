#include <args.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ft_ping.h>

uint8_t send_echo_request(int socket, struct sockaddr_in dest, t_ping_packet *p)
{
	ssize_t	send_ret = 0;

	send_ret = sendto(socket, &(p->packet), sizeof(p->packet), 0, (struct sockaddr *) &dest, sizeof(dest));
	if (send_ret == -1)
		return (1);
	struct timeval send_time;
	gettimeofday(&send_time, NULL);
	p->send_timestamp = send_time;
	return (0);
}

int get_source_ip(struct in_addr *src_addr)
{
	struct sockaddr_in	addr;
	socklen_t			addr_len = sizeof(addr);
	int					sock;
	
	struct sockaddr_in	remote;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sock < 0) {
        return (1);
    }
    
    remote.sin_family = AF_INET;
    remote.sin_port = htons(53);
    inet_pton(AF_INET, "8.8.8.8", &remote.sin_addr);

    if (connect(sock, (struct sockaddr *) &remote, sizeof(remote)) < 0) {
        close(sock);
        return (1);
    }

	if (getsockname(sock, (struct sockaddr *) &addr, &addr_len) < 0)
	{
        close(sock);
        return (1);
    }
	*src_addr = addr.sin_addr;
	close(sock);
	return (0);
}
