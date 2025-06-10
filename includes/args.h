#ifndef ARGS_H
#define ARGS_H

#include <stdint.h>
#include <stddef.h>

typedef struct	s_opt
{
	char	short_flag;
	char	*long_flag;
	char	*description;
}				t_opt;

#define OPTS_NB		1
#define VERSION_OPT 1

#define	OPTS { \
	((t_opt) {'V', NULL, "Show version and exit"}) \
}

typedef struct	s_opts
{
	uint64_t	bitmap;
}				t_opts;

int parse_args(int argc, char *argv[]);


#endif