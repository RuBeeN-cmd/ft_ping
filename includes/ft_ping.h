#ifndef FT_PING_H
#define FT_PING_H

#define PROGRAM_VERSION "1.0.0"
#define	PROGRAM_NAME	"ft_ping"

#include <packet.h>
#include <arpa/inet.h>
#include <logs.h>
#include <args.h>

typedef struct s_context
{
	t_opts	opts;
	int 	send_socket;
	int 	recv_socket;
} t_context;


// context
int		init_context(t_context *c, int argc, char *argv[]);
void	close_context(t_context *c);


uint8_t send_echo_request(int socket, struct sockaddr_in dest, t_ping_packet packet);
int		get_source_ip(struct in_addr *src_addr);

int		capture_response(t_ping_packet *packet_sent, int socket);

#endif