#include <args.h>
#include <libft.h>
#include <stdio.h>

int	match_long_opt(t_opt opts[], size_t opts_nb, char *arg)
{
	size_t	arg_len = ft_strlen(arg);
	for (size_t i = 0; i < opts_nb; i++)
	{
		if (opts[i].long_flag && !ft_strncmp(opts[i].long_flag, arg, arg_len))
			return (i);
	}
	return (-1);
}

int	match_short_opt(t_opt opts[], size_t opts_nb, char arg)
{
	for (size_t i = 0; i < opts_nb; i++)
	{
		if (arg == opts[i].short_flag)
			return (i);
	}
	return (-1);
}

int parse_args(int argc, char *argv[])
{
	t_opt	opts[OPTS_NB] = OPTS;
	size_t	opts_nb = OPTS_NB;
	t_opts	parsed_opts = { 0 };
	for (size_t i = 1; i < (size_t) argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			char *arg = argv[i] + 2;
			int	opt_index = match_long_opt(opts, opts_nb, arg);
			if (opt_index == -1)
			{
				// Invalid flag
				fprintf(stderr, "Invalid flag: %s\n", arg);
				return (1);
			}
			else
			{
				parsed_opts.bitmap ^= 1 << opt_index;
			}
			// long opt
			i++;
		}
		else if (argv[i][0] == '-' && argv[i][1])
		{
			size_t	arg_len = ft_strlen(argv[i]);
			for (size_t j = 1; j < arg_len; j++)
			{
				char arg = argv[i][j];
				int	opt_index = match_short_opt(opts, opts_nb, arg);
				if (opt_index == -1)
				{
					// Invalid flag
					fprintf(stderr, "Invalid flag: %c\n", arg);
					return (1);
				}
				else
				{
					parsed_opts.bitmap ^= 1 << opt_index;
				}
			}
		}
		else
		{
			char *arg = argv[i];
			(void) arg;
			// arg
		}
	}
	return (0);
}


