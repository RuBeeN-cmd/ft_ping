#include <time.h>
#include <ft_ping.h>
#include <unistd.h>
#include <netdb.h>

int hostname_to_ipv4_addr(char *hostname, in_addr_t *addr)
{
    struct hostent *host = NULL;

	host = gethostbyname(hostname);
    if (host == NULL) {
        return (1);
    }
    *addr = *(in_addr_t *)host->h_addr_list[0];
    return (0);
}

int	get_dest_addr(struct sockaddr_in *dest_addr, char addr_str[])
{
	dest_addr->sin_family = AF_INET;
	dest_addr->sin_port = 0;
	if (inet_pton(AF_INET, addr_str, &dest_addr->sin_addr.s_addr) <= 0
		&& hostname_to_ipv4_addr(addr_str, &dest_addr->sin_addr.s_addr)) {
		return (1);
	}
	return (0);
}

void	print_header_line(uint8_t is_verbose, char hostname[], char ip[], uint16_t packet_id, int packet_data_size) {
	if (is_verbose) {
		INFO("PING %s%s%s (%s%s%s): %s%d%s data bytes, id %s0x%x%s = %s%d%s\n",
			get_color(COLOR_PURPLE), hostname, get_color(COLOR_RESET),
			get_color(COLOR_PURPLE), ip, get_color(COLOR_RESET),
			get_color(COLOR_BLUE), packet_data_size, get_color(COLOR_RESET),
			get_color(COLOR_GREEN), packet_id, get_color(COLOR_RESET),
			get_color(COLOR_GREEN), packet_id, get_color(COLOR_RESET));
	} else {
		INFO("PING %s%s%s (%s%s%s): %s%d%s data bytes\n",
			get_color(COLOR_PURPLE), hostname, get_color(COLOR_RESET),
			get_color(COLOR_PURPLE), ip, get_color(COLOR_RESET),
			get_color(COLOR_BLUE), packet_data_size, get_color(COLOR_RESET));
	}
}

int	is_timed_out(uint32_t timeout, struct timeval start_time)
{
	if (timeout == 0)
		return (0);
	struct timeval	current_time;
	if (gettimeofday(&current_time, NULL) < 0) {
		WARN("Failed to get current time, timeout option may not work\n");
		return (0);
	}
	double elapsed = get_elapsed_ms(start_time, current_time);
	if (elapsed >= timeout * 1000.0)
		return (1);
	return (0);
}


int main(int argc, char *argv[]) {
	t_context c = {0};
	int ret = init_context(&c, argc, argv);
	if (ret)
		return (ret == -1);
	int timed_out = 0;
	int	force_quit = 0;
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
		dbg_packet(&p.packet);
		print_header_line(has_flag(c.opts.bitmap, VERBOSE_OPT), c.opts.hosts[i], inet_ntoa(dest_addr.sin_addr), ntohs(p.packet.icmp_header.un.echo.id), PING_PACKET_DATA_SIZE);
		t_stats stats;
		init_stats(&stats);
		while (!has_flag(c.opts.bitmap, COUNT_OPT) || stats.sent < c.opts.count || !c.opts.count) {
			DBG("Sending packet\n");
			if (send_echo_request(c.send_socket, dest_addr, &p)) {
				WARN("Failed to send echo request for: %s\n", c.opts.hosts[i]);
				continue;
			}
			stats.sent++;


			if (!timed_out && has_flag(c.opts.bitmap, TIMEOUT_OPT) && is_timed_out(c.opts.timeout, c.start_time)) {
				timed_out = 1;
				break;
			}
			if (force_quit)
				break;


			DBG("Waiting for response...\n");
			capture_response(&stats, &p, c.recv_socket, has_flag(c.opts.bitmap, VERBOSE_OPT));
			update_packet(&p);
			sleep(1);


			if (timed_out)
				break;
			if (is_early_exit()) {
				force_quit = 1;
				break;
			}
		}
		calculate_metrics(&stats);
		show_stats(&stats, c.opts.hosts[i]);
		free_stats(&stats);
	}
	close_context(&c);
	return (0);
}