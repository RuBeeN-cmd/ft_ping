#include <ft_ping.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	set_log_level(DEBUG);	

	t_context c = {0};
	int ret = init_context(&c, argc, argv);
	if (ret)
		return (ret == -1);

	init_stats(&c.stats);
	for (size_t i = 0; c.opts.hosts[i]; i++)
	{
		struct sockaddr_in dest_addr;
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = 0;
		if (inet_pton(AF_INET, c.opts.hosts[i], &dest_addr.sin_addr.s_addr) <= 0) {
			WARN("Failed to convert address: %s\n", c.opts.hosts[i]);
			continue ;
		}
		t_ping_packet p = {};
		if (create_ping_packet(dest_addr, &p)) {
			WARN("Failed to create packet for: %s\n", c.opts.hosts[i]);
			continue ;
		}
		INFO("PING %s (%s): %d data bytes\n", c.opts.hosts[i], inet_ntoa(dest_addr.sin_addr), PING_PACKET_DATA_SIZE);
		for (uint32_t j = 0; \
			(j < c.opts.count || !has_flag(c.opts.bitmap, COUNT_OPT) || !c.opts.count) && !is_early_exit(); \
			j++) {
			if (send_echo_request(c.send_socket, dest_addr, &p)) {
				WARN("Failed to send echo request for: %s\n", c.opts.hosts[i]);
				continue;
			}
			capture_response(&c.stats, &p, c.recv_socket);
			update_packet(&p);
			sleep(1);
		}
		calculate_metrics(&c.stats);
		INFO("--- %s ping statistics ---\n", c.opts.hosts[i]);
		INFO("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", c.stats.min_time, c.stats.avg_time, c.stats.max_time, c.stats.stddev_time);
	}
	close_context(&c);
	return (0);
}