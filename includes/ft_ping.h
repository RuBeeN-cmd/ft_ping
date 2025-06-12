#ifndef FT_PING_H
#define FT_PING_H

#define PROGRAM_VERSION "1.0.0"
#define	PROGRAM_NAME	"ft_ping"

#include <packet.h>
#include <arpa/inet.h>



typedef struct	s_context
{
	uint64_t			flags;
	char				**hosts;
	int					socket_fd;
	struct sockaddr_in	src_addr;
	struct sockaddr_in	crt_dest_addr;
	t_ping_packet		last_packet;
}				t_context;




uint8_t send_echo_request(int socket, struct sockaddr_in dest, t_ping_packet packet);
int		create_socket();

#endif