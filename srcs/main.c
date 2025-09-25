#include <ft_ping.h>
#include <unistd.h>

int	get_dest_addr(struct sockaddr_in *dest_addr, char addr_str[])
{
	dest_addr->sin_family = AF_INET;
	dest_addr->sin_port = 0;
	if (inet_pton(AF_INET, addr_str, &dest_addr->sin_addr.s_addr) <= 0) {
		return (1);
	}
	return (0);
}

int main(int argc, char *argv[]) {
	t_context c = {0};
	int ret = init_context(&c, argc, argv);
	if (ret)
		return (ret == -1);
	for (size_t i = 0; c.opts.hosts[i]; i++)
	{
		struct sockaddr_in dest_addr;
		if (get_dest_addr(&dest_addr, c.opts.hosts[i])) {
			WARN("Failed to convert address: %s\n", c.opts.hosts[i]);
			continue ;
		}

		t_ping_packet p = {};
		if (create_ping_packet(dest_addr, &p, c.opts.ttl)) {
			WARN("Failed to create packet for: %s\n", c.opts.hosts[i]);
			continue ;
		}
		INFO("PING %s (%s): %d data bytes\n", c.opts.hosts[i], inet_ntoa(dest_addr.sin_addr), PING_PACKET_DATA_SIZE);
		while ((c.stats.sent < c.opts.count || !has_flag(c.opts.bitmap, COUNT_OPT) || !c.opts.count) && !is_early_exit()) {
			if (send_echo_request(c.send_socket, dest_addr, &p)) {
				WARN("Failed to send echo request for: %s\n", c.opts.hosts[i]);
				continue;
			}
			c.stats.sent++;
			capture_response(&c.stats, &p, c.recv_socket);
			update_packet(&p);
			sleep(1);
		}
		calculate_metrics(&c.stats);
		INFO("--- %s ping statistics ---\n", c.opts.hosts[i]);
		INFO("%d packets transmitted, %d packets received, %.1f%% packet loss\n", c.stats.sent, c.stats.received, c.stats.sent ? ((c.stats.sent - c.stats.received) * 100.0 / c.stats.sent) : 0);
		if (c.stats.received)
			INFO("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", c.stats.min_time, c.stats.avg_time, c.stats.max_time, c.stats.stddev_time);
	}
	close_context(&c);
	return (0);
}