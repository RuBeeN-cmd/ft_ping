#ifndef ARGS_H
#define ARGS_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

typedef struct	s_opt
{
	char	short_flag;
	char	*long_flag;
	char	*description;
	int		has_arg;
	char	*arg_name;
}				t_opt;

#define COUNT_OPT	0
#define HELP_OPT	1
#define VERSION_OPT 2
#define OPTS_NB		3

#define	OPTS { \
	((t_opt) {'c', "count", "stop after sending NUMBER packets", 1, "NUMBER"}), \
	((t_opt) {'?', "help", "give this help list", 0, NULL}), \
	((t_opt) {'V', NULL, "print program version", 0, NULL}), \
}

typedef struct	s_opts
{
	uint64_t	bitmap;
	uint32_t	count;
	char		**hosts;
}				t_opts;

int 	parse_args(int argc, char *argv[], t_opts *opts);

int		has_flag(uint64_t bitmap, uint32_t flag);

void	invalid_option(char *program_name, char opt);
void	unrecognized_option(char *program_name, char *opt);
void	long_opt_arg_required(char *program_name, char *opt);
void	short_opt_arg_required(char *program_name, char opt);
void	help(t_opt opts[], size_t opts_nb);

#endif