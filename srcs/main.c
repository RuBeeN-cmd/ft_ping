#include <args.h>
#include <packet.h>
#include <ft_ping.h>
#include <unistd.h>
#include <stdlib.h>


// static void print_opts(t_opts opts) {
// 	printf("Bitmap: %lu\n", opts.bitmap);
// 	for (size_t i = 0; opts.hosts[i]; i++)
// 		printf("Host %zu: %s\n", i + 1, opts.hosts[i]);
// }

int main(int argc, char *argv[]) {
	t_opts opts = {0};
	if (parse_args(argc, argv, &opts))
		return (1);
	
	int socket_fd = create_socket();
	if (socket_fd < 0)
		return (1);
		
	for (size_t i = 0; opts.hosts[i]; i++)
	{
		struct sockaddr_in dest_addr;
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = 0;
		if (inet_pton(AF_INET, opts.hosts[i], &dest_addr.sin_addr.s_addr) <= 0) {
			perror("inet_pton");
			return (1);
		}
		t_ping_packet packet = {};
		create_packet(dest_addr, &packet);
		if (send_echo_request(socket_fd, dest_addr, packet))
			return (1);
	}
	close(socket_fd);
	free(opts.hosts);
	return (0);
}