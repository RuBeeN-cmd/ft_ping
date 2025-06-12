#include <args.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <sys/time.h>

#include <ft_ping.h>

// #define TIMEOUT_SEC 1

// int	set_socket_timeout(int socket_fd)
// {
// 	struct timeval tv_out;
// 	tv_out.tv_sec = TIMEOUT_SEC;
// 	tv_out.tv_usec = 0;
// 	if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out)) < 0)
// 		return -1;
// 	return 0;
// }

int	create_socket()
{
	int	socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (socket_fd < 0)
	{
		perror("socket");
		return (-1);
	}
	int disable = 1;
	if (setsockopt(socket_fd, IPPROTO_IP, IP_HDRINCL, &disable, sizeof(disable)) < 0) {
		return (-1);
	}
	return (socket_fd);
}

uint8_t send_echo_request(int socket, struct sockaddr_in dest, t_ping_packet packet)
{
	ssize_t	send_ret = 0;
	
	printf("Sending Packet:\n");
	(void) socket;
	(void) dest;
	(void) packet;
	dbg_packet(&packet);

	// char *coucou = "aa";
	printf("Socket: %d\n", socket);\
	printf("Destination: %s\n", inet_ntoa(dest.sin_addr));
	printf("Packet size: %zu\n", sizeof(packet));
	send_ret = sendto(socket, &packet, sizeof(packet), 0, (struct sockaddr *) &dest, sizeof(dest));
	// send_ret = sendto(socket, coucou, 2, 0, (struct sockaddr *) &dest, sizeof(dest));
	if (send_ret == -1)
		return (1);
	return (0);
}