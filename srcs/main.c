#include <time.h>
#include <ft_ping.h>
#include <unistd.h>
#include <netdb.h>

void	print_header_line(uint8_t is_verbose, char hostname[], char ip[], uint16_t packet_id, int packet_data_size) {
	if (is_verbose) {
		INFO("PING %s%s%s (%s%s%s): %s%d%s data bytes, id %s0x%04x%s = %s%d%s\n",
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
	for (size_t i = 0; c.opts.hosts[i]; i++)
	{
		t_target target = {0};
		if (init_target(&target, c.opts.hosts[i], &c.opts)) {
			continue;
		}
		dbg_packet(&target.packet.packet);
		print_header_line(has_flag(c.opts.bitmap, VERBOSE_OPT), target.arg, inet_ntoa(target.addr.sin_addr), ntohs(target.packet.packet.icmp_header.un.echo.id), PING_PACKET_DATA_SIZE);
		while (!has_flag(c.opts.bitmap, COUNT_OPT) || target.stats.sent < c.opts.count || !c.opts.count) {
			DBG("Sending packet\n");
			if (send_echo_request(c.send_socket, target.addr, &target.packet)) {
				WARN("Failed to send echo request for: %s\n", target.arg);
				break;
			}
			target.stats.sent++;


			if (!c.timed_out && has_flag(c.opts.bitmap, TIMEOUT_OPT) && is_timed_out(c.opts.timeout, c.start_time)) {
				c.timed_out = 1;
				break;
			}
			if (c.force_quit)
				break;


			DBG("Waiting for response...\n");
			capture_response(&target, c.recv_socket, &c.opts);
			update_packet(&target.packet);
			if (!has_flag(c.opts.bitmap, FLOOD_OPT) && c.opts.preload == 0)
				sleep(1);
			
			if (c.opts.preload)
				c.opts.preload--;

			if (c.timed_out)
				break;
			if (is_early_exit()) {
				c.force_quit = 1;
				break;
			}
		}
		show_stats(&target.stats, target.arg);
		free_stats(&target.stats);
	}
	close_context(&c);
	return (0);
}