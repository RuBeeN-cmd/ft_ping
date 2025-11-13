#include <args.h>
#include <libft.h>
#include <ft_ping.h>

static void	try_help()
{
	ERR("Try '%s --help' or '%s --usage' for more information.\n", PROGRAM_NAME, PROGRAM_NAME);
}

void	invalid_value(char *program_name, char value[])
{
	ERR("%s: invalid value (`%s' near `%s')\n", program_name, value, value);
	try_help();
}

void	invalid_option(char *program_name, char opt)
{
	ERR("%s: invalid option -- '%c'\n", program_name, opt);
	try_help();
}

void	unrecognized_option(char *program_name, char *opt)
{
	ERR("%s: unrecognized option '%s'\n", program_name, opt);
	try_help();
}

void	long_opt_arg_required(char *program_name, char *opt)
{
	ERR("%s: option '%s' requires an argument\n", program_name, opt);
	try_help();
}

void	short_opt_arg_required(char *program_name, char opt)
{
	ERR("%s: option requires an argument -- '%c'\n", program_name, opt);
	try_help();
}

void	missing_host(char *program_name)
{
	ERR("%s: missing host operand\n", program_name);
	try_help();
}

static void	print_options(t_opt opts[], size_t opts_nb)
{
	int	description_padding = 25;
	for (size_t i = 0; i < opts_nb; i++)
	{
		if (opts[i].short_flag)
			INFO(" -%c", opts[i].short_flag);
		else
			INFO("   ");
		if (opts[i].short_flag && opts[i].long_flag)
			printf(",");
		else
			printf(" ");
		if (opts[i].long_flag)
		{
			if (opts[i].arg_name)
				printf(" --%s=%s%*s", opts[i].long_flag, opts[i].arg_name, description_padding - (int) ft_strlen(opts[i].long_flag) - (int) ft_strlen(opts[i].arg_name) - 4, "");
			else
				printf(" --%s%*s", opts[i].long_flag, description_padding - (int) ft_strlen(opts[i].long_flag) - 3, "");
		}
		else
			printf("%*s", description_padding, "");
		printf("%s\n", opts[i].description);
	}
}

void	help(t_opt opts[], size_t opts_nb)
{
	INFO("Usage: %s [OPTION...] HOST ...\n", PROGRAM_NAME);
	INFO("Send ICMP ECHO_REQUEST packets to network hosts.\n");
	INFO("\n");
	print_options(opts, opts_nb);
	INFO("\n");
	INFO("Mandatory or optional arguments to long options are also mandatory or optional\n");
	INFO("for any corresponding short options.\n");
	INFO("\n");
	INFO("The whole program is available only to superuser.\n");
}

void	version()
{
	INFO("%s version %s\n", PROGRAM_NAME, PROGRAM_VERSION);
}

void	usage(t_opt opts[], size_t opts_nb)
{
	char	short_opt[256] = { 0 };
	char	short_opt_with_val[256] = { 0 };
	char	long_opt[256] = { 0 };
	char	*crt_short_opt = short_opt;
	char	*crt_short_opt_with_val = short_opt_with_val;
	char	*crt_long_opt = long_opt;
	crt_short_opt += sprintf(crt_short_opt, " [-");
	for (size_t i = 0; i < opts_nb; i++)
	{
		if (opts[i].short_flag) {
			if (opts[i].arg_name) {
				crt_short_opt_with_val += sprintf(crt_short_opt_with_val, " [-%c %s]", opts[i].short_flag, opts[i].arg_name);
			} else {
				crt_short_opt += sprintf(crt_short_opt, "%c", opts[i].short_flag);
			}
		}
		if (opts[i].long_flag) {
			if (opts[i].arg_name) {
				crt_long_opt += sprintf(crt_long_opt, " [--%s=%s]", opts[i].long_flag, opts[i].arg_name);
			} else {
				crt_long_opt += sprintf(crt_long_opt, " [--%s]", opts[i].long_flag);
			}
		}
	}
	sprintf(crt_short_opt, "]");
	INFO("Usage: %s%s%s%s HOST ...\n", PROGRAM_NAME, short_opt, short_opt_with_val, long_opt);
}