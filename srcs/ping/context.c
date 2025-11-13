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
	if (setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable)) < 0) {
		close(socket_fd);
		return (-1);
	}
	return (socket_fd);
}

static int	create_recv_socket(uint32_t timeout)
{
	int	socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (socket_fd < 0) {
		return (-1);
	}
	struct timeval tv;
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
		close(socket_fd);
		return (-1);
	}
	return (socket_fd);
}
		
int	init_context(t_context *c, int argc, char *argv[])
{
	c->force_quit = 0;
	c->timed_out = 0;
	int ret = parse_args(argc, argv, &c->opts);
	if (ret) {
		return (ret);
	}

	c->send_socket = create_send_socket();
	if (c->send_socket < 0) {
		ERR("Failed to create send socket\n");
		goto exit_3;
	}

	c->recv_socket = create_recv_socket(c->opts.linger);
	if (c->recv_socket < 0) {
		ERR("Failed to create receive socket\n");
		goto exit_2;
	}

	if (init_signals()) {
		ERR("Failed to initialize signals\n");
		goto exit_1;
	}

	if (gettimeofday(&c->start_time, NULL) < 0) {
		WARN("Failed to get start time, timeout option may not work\n");
	}

	return (0);

	exit_1:
	close(c->recv_socket);
	exit_2:
	close(c->send_socket);
	exit_3:
	free(c->opts.hosts);
	return (-1);
}

void	close_context(t_context *c)
{
	if (c->send_socket >= 0) {
		DBG("%sClosing send socket %s%d%s\n", \
			get_color(COLOR_RED), get_color(COLOR_ORANGE), c->send_socket, get_color(COLOR_RESET));
		close(c->send_socket);
	}
	if (c->recv_socket >= 0) {
		DBG("%sClosing send socket %s%d%s\n", \
			get_color(COLOR_RED), get_color(COLOR_ORANGE), c->recv_socket, get_color(COLOR_RESET));
		close(c->recv_socket);
	}
	DBG("%sFreeing host list%s\n", \
		get_color(COLOR_RED), get_color(COLOR_RESET));
	free(c->opts.hosts);
}