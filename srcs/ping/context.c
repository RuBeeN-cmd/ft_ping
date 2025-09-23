#include <ft_ping.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

static int	create_send_socket()
{
	int	socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (socket_fd < 0) {
		return (-1);
	}
	int enable = 1;
	if (setsockopt(socket_fd, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable)) < 0) {
		close(socket_fd);
		return (-1);
	}
	return (socket_fd);
}

static int	create_recv_socket()
{
	int	socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (socket_fd < 0) {
		return (-1);
	}
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
		close(socket_fd);
		return (-1);
	}
	return (socket_fd);
}
		
int	init_context(t_context *c, int argc, char *argv[])
{
	int ret = parse_args(argc, argv, &c->opts);
	if (ret) {
		return (ret);
	}

	c->send_socket = create_send_socket();
	if (c->send_socket < 0) {
		free(c->opts.hosts);
		ERR("Failed to create send socket\n");
		return (-1);
	}

	c->recv_socket = create_recv_socket();
	if (c->recv_socket < 0) {
		free(c->opts.hosts);
		close(c->send_socket);
		ERR("Failed to create receive socket\n");
		return (-1);
	}
	return (0);
}

void	close_context(t_context *c)
{
	if (c->send_socket >= 0) {
		DBG("Closing send socket %d\n", c->send_socket);
		close(c->send_socket);
	}
	if (c->recv_socket >= 0) {
		DBG("Closing receive socket %d\n", c->recv_socket);
		close(c->recv_socket);
	}
	DBG("Freeing host list\n");
	free(c->opts.hosts);
}