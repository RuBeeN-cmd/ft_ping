#include <args.h>
#include <libft.h>
#include <stdio.h>
#include <ft_ping.h>

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

int parse_args(int argc, char *argv[], t_opts *parsed_opts)
{
	t_opt	opts[OPTS_NB] = OPTS;
	size_t	opts_nb = OPTS_NB;
	parsed_opts->bitmap = 0;
	char	*hosts[1024];
	size_t	host_nb = 0;
	for (size_t i = 1; i < (size_t) argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			char	*arg = argv[i] + 2;
			char	*equal_sign = ft_strchr(arg, '=');
			char	*arg_value;
			if (equal_sign)
			{
				*equal_sign = 0;
				arg_value = equal_sign + 1;
			}
			int	opt_index = match_long_opt(opts, opts_nb, arg);
			if (opt_index == -1)
			{
				unrecognized_option(argv[0], argv[i]);
				return (-1);
			}
			if (opts[opt_index].has_arg)
			{
				if (!equal_sign)
				{
					if (i + 1 >= (size_t) argc)
					{
						long_opt_arg_required(argv[0], argv[i]);
						return (-1);
					}
					i++;
					arg_value = argv[i];
				}
				printf("%s value: %s\n", arg, arg_value);
			}
			if (opt_index == HELP_OPT)
			{
				help(opts, opts_nb);
				return (1);
			}
			else if (opt_index == VERSION_OPT)
			{
				printf("Version %s\n", PROGRAM_VERSION);
				return (1);
			}
			parsed_opts->bitmap ^= 1 << opt_index;
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
					invalid_option(argv[0], arg);
					return (-1);
				}

				char	*arg_value;
				if (opts[opt_index].has_arg)
				{
					if (i + 1 >= (size_t) argc)
					{
						short_opt_arg_required(argv[0], arg);
						return (-1);
					}
					i++;
					arg_value = argv[i];
					printf("%c value: %s\n", arg, arg_value);
				}




				if (opt_index == HELP_OPT)
				{
					help(opts, opts_nb);
					return (1);
				}
				else if (opt_index == VERSION_OPT)
				{
					printf("Version %s\n", PROGRAM_VERSION);
					return (1);
				}
				parsed_opts->bitmap ^= 1 << opt_index;
			}
		}
		else
		{
			char *arg = argv[i];
			hosts[host_nb++] = arg;
		}
	}
	parsed_opts->hosts = malloc((host_nb + 1) * sizeof(char *));
	if (!parsed_opts->hosts)
	{
		fprintf(stderr, "Memory allocation failed\n");
		return (-1);
	}
	ft_memcpy(parsed_opts->hosts, hosts, host_nb * sizeof(char *));
	parsed_opts->hosts[host_nb] = NULL;
	printf("Bitmap: %lu\n", parsed_opts->bitmap);
	return (0);
}
