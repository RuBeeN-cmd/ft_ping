#include <args.h>
#include <libft.h>
#include <stdio.h>
#include <ft_ping.h>

int64_t	str_to_u32(char str[])
{
	int64_t	value = 0;
	for (size_t i = 0; str[i]; i++)
	{
		if (!ft_isdigit(str[i]))
			return (-1);
		if (value > (value + str[i] - '0') * 10)
			return (-1);
		value *= 10;
		value += str[i] - '0';
	}
	return (value);
}


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

static int	assign_value(t_opts *parsed_opts, int opt_index, char *value)
{
	t_opt	opts[OPTS_NB] = OPTS;
	size_t	opts_nb = OPTS_NB;

	if (opt_index == COUNT_OPT) {
		int64_t	val = str_to_u32(value);
		if (val < 0) {
			return (-1);
		}
		parsed_opts->count = val;
	} else if (opt_index == LOG_OPT) {
		if (!ft_strncmp(value, "DEBUG", 6))
			set_log_level(LEVEL_DEBUG);
		else if (!ft_strncmp(value, "INFO", 5))
			set_log_level(LEVEL_INFO);
		else if (!ft_strncmp(value, "WARN", 5))
			set_log_level(LEVEL_WARNING);
		else if (!ft_strncmp(value, "ERROR", 6))
			set_log_level(LEVEL_ERROR);
		else
			return (-1);
	} else if (opt_index == NO_COLOR_OPT) {
		disable_colors();
	} else if (opt_index == NO_PREFIX_OPT) {
		disable_prefix();
	} else if (opt_index == HELP_OPT) {
		help(opts, opts_nb);
	} else if (opt_index == USAGE_OPT) {
		usage(opts, opts_nb);
	} else if (opt_index == VERSION_OPT) {
		version();
	} else if (opt_index == TTL_OPT) {
		int64_t	val = str_to_u32(value);
		if (val <= 0 || val > 255)
			return (-1);
		parsed_opts->ttl = val;
	} else if (opt_index == TIMEOUT_OPT) {
		int64_t	val = str_to_u32(value);
		if (val < 0 || val > 1024)
			return (-1);
		parsed_opts->timeout = val;
	} else if (opt_index == LINGER_OPT) {
		int64_t	val = str_to_u32(value);
		if (val < 0 || val > 1024)
			return (-1);
		parsed_opts->linger = val;
	}
	if (opts[opt_index].can_interrupt)
		return (1);
	return (0);
}

static int	parse_long_opt(t_opts *parsed_opts, char program_name[], char arg[], char *next_arg[])
{
	t_opt	opts[OPTS_NB] = OPTS;
	size_t	opts_nb = OPTS_NB;

	char	*opt = arg + 2;
	char	*equal_sign = ft_strchr(opt, '=');
	char	*opt_value = NULL;
	if (equal_sign) {
		*equal_sign = 0;
		opt_value = equal_sign + 1;
	}
	int	opt_index = match_long_opt(opts, opts_nb, opt);
	if (opt_index == -1) {
		unrecognized_option(program_name, arg);
		return (1);
	}
	if (opts[opt_index].arg_name) {
		if (!equal_sign) {
			if (!*next_arg) {
				long_opt_arg_required(program_name, arg);
				return (1);
			}
			opt_value = *next_arg;
			*next_arg = NULL;
		}
	}
	int ret = assign_value(parsed_opts, opt_index, opt_value);
	if (ret < 0) {
		invalid_value(program_name, opt_value);
		return (-1);
	} else if (ret) {
		return (1);
	}
	parsed_opts->bitmap ^= 1 << opt_index;
	return (0);
}

static int parse_short_opt(t_opts *parsed_opts, char program_name[], char arg, char *next_arg[])
{
	t_opt	opts[OPTS_NB] = OPTS;
	size_t	opts_nb = OPTS_NB;

	int	opt_index = match_short_opt(opts, opts_nb, arg);
	if (opt_index == -1) {
		invalid_option(program_name, arg);
		return (1);
	}
	char	*opt_value = NULL;
	if (opts[opt_index].arg_name) {
		if (!*next_arg) {
			short_opt_arg_required(program_name, arg);
			return (1);
		}
		opt_value = *next_arg;
		*next_arg = NULL;
	}
	int ret = assign_value(parsed_opts, opt_index, opt_value);
	if (ret < 0) {
		invalid_value(program_name, opt_value);
		return (-1);
	} else if (ret) {
		return (1);
	}
	parsed_opts->bitmap ^= 1 << opt_index;
	return (0);
}

int parse_args(int argc, char *argv[], t_opts *parsed_opts)
{
	parsed_opts->bitmap = 0;
	parsed_opts->ttl = 64;
	parsed_opts->linger = 1;
	parsed_opts->timeout = 0;
	parsed_opts->count = 0;
	char	*hosts[1024];
	size_t	host_nb = 0;

	char	*program_name = argv[0];
	for (size_t i = 1; i < (size_t) argc; i++)
	{
		if (!argv[i])
			continue ;
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			int ret = parse_long_opt(parsed_opts, program_name, argv[i], i + 1 <= (size_t) argc ? argv + i + 1 : NULL);
			if (ret)
				return (ret);
		}
		else if (argv[i][0] == '-' && argv[i][1])
		{
			size_t	arg_len = ft_strlen(argv[i]);
			for (size_t j = 1; j < arg_len; j++)
			{
				int ret = parse_short_opt(parsed_opts, program_name, argv[i][j], (i + 1 <= (size_t) argc ? argv + i + 1 : NULL));
				if (ret)
					return (ret);
			}
		} else {
			hosts[host_nb++] = argv[i];
		}
	}
	if (host_nb == 0) {
		missing_host(program_name);
		return (-1);
	}
	parsed_opts->hosts = malloc((host_nb + 1) * sizeof(char *));
	if (!parsed_opts->hosts) {
		fprintf(stderr, "Memory allocation failed\n");
		return (-1);
	}
	ft_memcpy(parsed_opts->hosts, hosts, host_nb * sizeof(char *));
	parsed_opts->hosts[host_nb] = NULL;
	return (0);
}