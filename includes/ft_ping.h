#ifndef FT_PING_H
#define FT_PING_H

#define PROGRAM_VERSION "1.0.0"
#define	PROGRAM_NAME	"ft_ping"

#include <packet.h>
#include <arpa/inet.h>
#include <logs.h>
#include <args.h>
#include <signals.h>
#include <libft.h>

typedef struct	s_stats
{
	uint32_t	sent;
	uint32_t	received;
	uint32_t	lost;
	double		min_time;
	double		avg_time;
	double		max_time;
	double		stddev_time;
	t_list		*times;
}				t_stats;

typedef struct s_context
{
	t_opts			opts;
	int 			send_socket;
	int 			recv_socket;
	t_stats			stats;
} t_context;

// context
int		init_context(t_context *c, int argc, char *argv[]);
void	close_context(t_context *c);


uint8_t send_echo_request(int socket, struct sockaddr_in dest, t_ping_packet *packet);
int		get_source_ip(struct in_addr *src_addr);
int		capture_response(t_stats *stats, t_ping_packet *p, int socket);

// stats.c
void 	init_stats(t_stats *stats);
void	add_time(t_stats *stats, double time);
void	free_stats(t_stats *stats);
void	calculate_metrics(t_stats *stats);

#endif