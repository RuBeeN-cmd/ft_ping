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
		dbg_packet(&p.packet);
		print_header_line(has_flag(c.opts.bitmap, VERBOSE_OPT), c.opts.hosts[i], inet_ntoa(dest_addr.sin_addr), ntohs(p.packet.icmp_header.un.echo.id), PING_PACKET_DATA_SIZE);
		t_stats stats;
		init_stats(&stats);
		while ((stats.sent < c.opts.count || !has_flag(c.opts.bitmap, COUNT_OPT) || !c.opts.count)) {
			DBG("Sending packet\n");
			if (send_echo_request(c.send_socket, dest_addr, &p)) {
				WARN("Failed to send echo request for: %s\n", c.opts.hosts[i]);
				continue;
			}
			stats.sent++;
			if (is_early_exit())
				break;
			DBG("Waiting for response...\n");
			capture_response(&stats, &p, c.recv_socket);
			update_packet(&p);
			sleep(1);
			if (is_early_exit())
				break;
		}
		calculate_metrics(&stats);
		show_stats(&stats, c.opts.hosts[i]);
		free_stats(&stats);
	}
	close_context(&c);
	return (0);
}