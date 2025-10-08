#include <signals.h>

void	set_early_exit(int value)
{
	*get_early_exit() = value;
}

int		*get_early_exit()
{
	static int early_exit = 0;
	return (&early_exit);
}

int		is_early_exit()
{
	return (*get_early_exit());
}

void	handle_sigint(int sig)
{
	(void)sig;
	set_early_exit(1);
}

int	init_signals()
{
	struct sigaction	sa = {};
	sa.sa_handler = handle_sigint;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (1);
	return (0);
}