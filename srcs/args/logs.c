#include <args.h>
#include <libft.h>
#include <ft_ping.h>

static void	try_help()
{
	fprintf(stderr, "Try '%s --help' or '%s --usage' for more information.\n", PROGRAM_NAME, PROGRAM_NAME);
}

void	invalid_option(char *program_name, char opt)
{
	fprintf(stderr, "%s: invalid option -- '%c'\n", program_name, opt);
	try_help();
}

void	unrecognized_option(char *program_name, char *opt)
{
	fprintf(stderr, "%s: unrecognized option '%s'\n", program_name, opt);
	try_help();
}

void	long_opt_arg_required(char *program_name, char *opt)
{
	fprintf(stderr, "%s: option '%s' requires an argument\n", program_name, opt);
	try_help();
}

void	short_opt_arg_required(char *program_name, char opt)
{
	fprintf(stderr, "%s: option requires an argument -- '%c'\n", program_name, opt);
	try_help();
}

static void	print_options(t_opt opts[], size_t opts_nb)
{
	int	description_padding = 25;
	for (size_t i = 0; i < opts_nb; i++)
	{
		if (opts[i].short_flag)
			printf(" -%c", opts[i].short_flag);
		else
			printf("   ");
		if (opts[i].short_flag && opts[i].long_flag)
			printf(",");
		else
			printf(" ");
		if (opts[i].long_flag)
		{
			if (opts[i].has_arg)
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
	printf("Usage: %s [OPTION...] HOST ...\n\
Send ICMP ECHO_REQUEST packets to network hosts.\n\n", PROGRAM_NAME);
	print_options(opts, opts_nb);
	printf("\nMandatory or optional arguments to long options are also mandatory or optional\n\
for any corresponding short options.\n\n\
Options marked with (root only) are available only to superuser.\n");
}