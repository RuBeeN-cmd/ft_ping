#ifndef ARGS_H
#define ARGS_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

typedef enum e_opt_category
{
	MISC_CAT,
	INFO_CAT
} t_opt_category;

typedef struct	s_opt
{
	char			short_flag;
	char			*long_flag;
	char			*description;
	int				has_arg;
	char			*arg_name;
	t_opt_category	category;
	int				can_interrupt;
}				t_opt;

#define COUNT_OPT	0
#define HELP_OPT	1
#define USAGE_OPT	2
#define VERSION_OPT 3
#define OPTS_NB		4

#define	OPTS { \
	((t_opt) {'c', "count", "stop after sending NUMBER packets", 1, "NUMBER", MISC_CAT, 0}), \
	((t_opt) {'?', "help", "give this help list", 0, NULL, INFO_CAT, 1}), \
	((t_opt) {0, "usage", "give a short usage message", 0, NULL, INFO_CAT, 1}), \
	((t_opt) {'V', "version", "print program version", 0, NULL, INFO_CAT, 1}), \
}

typedef struct	s_opts
{
	uint64_t	bitmap;
	uint32_t	count;
	char		**hosts;
}				t_opts;

int 	parse_args(int argc, char *argv[], t_opts *opts);

int		has_flag(uint64_t bitmap, uint32_t flag);
int		get_first_interrupt_flag(uint64_t bitmap);

void	invalid_option(char *program_name, char opt);
void	unrecognized_option(char *program_name, char *opt);
void	long_opt_arg_required(char *program_name, char *opt);
void	short_opt_arg_required(char *program_name, char opt);
void	help(t_opt opts[], size_t opts_nb);
void	version();
void	missing_host(char *program_name);
void	usage(t_opt opts[], size_t opts_nb);

#endif