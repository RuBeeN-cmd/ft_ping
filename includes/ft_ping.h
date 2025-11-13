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
#include <sys/time.h>

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
	struct timeval	start_time;
	int				force_quit;
	int				timed_out;
} t_context;

typedef struct s_target
{
	char				hostname[1024];
	char				*arg;
	struct sockaddr_in	addr;
	t_ping_packet		packet;
	t_stats				stats;
} t_target;

// context
int		init_context(t_context *c, int argc, char *argv[]);
void	close_context(t_context *c);


uint8_t send_echo_request(int socket, struct sockaddr_in dest, t_ping_packet *packet);
int		get_source_ip(struct in_addr *src_addr);
int		capture_response(t_target *target, int recv_socket, uint8_t verbose);

// stats.c
void 	init_stats(t_stats *stats);
void	add_time(t_stats *stats, double time);
void	free_stats(t_stats *stats);
void	calculate_metrics(t_stats *stats);
void	show_stats(t_stats *stats, char *host);

// reponse/logs.c
void	print_response_info(ssize_t captured_bytes, struct in_addr from_addr, char *hostname, t_packet *recv_packet, double time_ms);
void	print_packet_dump(t_packet *packet);
void	print_ttl_exceeded_info(t_packet *packet_sent, ssize_t captured_bytes, struct in_addr from_addr, uint8_t verbose);

// target.c
int		init_target(t_target *target, char *hostname, t_opts *opts);

#endif