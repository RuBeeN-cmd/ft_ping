#ifndef ARGS_H
#define ARGS_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

typedef struct	s_opt
{
	char			short_flag;
	char			*long_flag;
	char			*description;
	char			*arg_name;
	int				can_interrupt;
}				t_opt;

#define COUNT_OPT		0
#define NUMERIC_OPT		1
#define TTL_OPT			2
#define VERBOSE_OPT 	3
#define TOS_OPT			4
#define TIMEOUT_OPT 	5
#define LINGER_OPT 		6
#define FLOOD_OPT 		7
#define PRELOAD_OPT 	8
#define LOG_OPT 		9
#define NO_COLOR_OPT	10
#define NO_PREFIX_OPT	11
#define HELP_OPT		12
#define USAGE_OPT		13
#define VERSION_OPT 	14
#define OPTS_NB			15

#define	OPTS { \
	((t_opt) {'c', "count", "stop after sending NUMBER packets", "NUMBER", 0}), \
	((t_opt) {'n', "numeric", "do not resolve host addresses", NULL, 0}), \
	((t_opt) {0, "ttl", "specify N as time-to-live", "N", 0}), \
	((t_opt) {'v', "verbose", "verbose output", NULL, 0}), \
	((t_opt) {'T', "tos", "set type of service (TOS) to NUM", "NUM", 0}), \
	((t_opt) {'w', "timeout", "stop after N seconds", "N", 0}), \
	((t_opt) {'W', "linger", "number of seconds to wait for response", "N", 0}), \
	((t_opt) {'f', "flood", "flood ping", NULL, 0}), \
	((t_opt) {'l', "preload", "send NUMBER packets as fast as possible before", "NUMBER", 0}), \
	((t_opt) {0, "log", "define the log level", "LEVEL", 0}), \
	((t_opt) {0, "no-color", "disable ANSI colors", NULL, 0}), \
	((t_opt) {0, "no-prefix", "disable logs prefix", NULL, 0}), \
	((t_opt) {'?', "help", "give this help list", NULL, 1}), \
	((t_opt) {0, "usage", "give a short usage message", NULL, 1}), \
	((t_opt) {'V', "version", "print program version", NULL, 1}), \
}

typedef struct	s_opts
{
	uint64_t	bitmap;
	uint32_t	count;
	uint8_t		ttl;
	uint32_t	timeout;
	uint32_t	linger;
	uint32_t	preload;
	char		**hosts;
	uint8_t		tos;
}				t_opts;

int 	parse_args(int argc, char *argv[], t_opts *opts);

int		has_flag(uint64_t bitmap, uint32_t flag);
int		get_first_interrupt_flag(uint64_t bitmap);

void	invalid_option(char *program_name, char opt);
void	invalid_value(char *program_name, char value[]);
void	unrecognized_option(char *program_name, char *opt);
void	long_opt_arg_required(char *program_name, char *opt);
void	short_opt_arg_required(char *program_name, char opt);
void	help(t_opt opts[], size_t opts_nb);
void	version();
void	missing_host(char *program_name);
void	usage(t_opt opts[], size_t opts_nb);

#endif