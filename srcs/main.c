#include <ft_ping.h>

int main(int argc, char *argv[]) {
	set_log_level(DEBUG);	

	t_context c = {0};
	if (init_context(&c, argc, argv))
		return (1);

	for (size_t i = 0; c.opts.hosts[i]; i++)
	{
		struct sockaddr_in dest_addr;
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = 0;
		if (inet_pton(AF_INET, c.opts.hosts[i], &dest_addr.sin_addr.s_addr) <= 0) {
			WARN("Failed to convert address: %s\n", c.opts.hosts[i]);
			continue ;
		}
		t_ping_packet packet = {};
		if (create_packet(dest_addr, &packet)) {
			WARN("Failed to create packet for: %s\n", c.opts.hosts[i]);
			continue ;
		}
		if (send_echo_request(c.send_socket, dest_addr, packet)) {
			WARN("Failed to send echo request for: %s\n", c.opts.hosts[i]);
			continue;
		}
		capture_response(&packet, c.recv_socket);

	}
	close_context(&c);
	return (0);
}