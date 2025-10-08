#include <args.h>

int	has_flag(uint64_t bitmap, uint32_t flag)
{
	return ((bitmap & (1 << flag)));
}

int	get_first_interrupt_flag(uint64_t bitmap)
{
	t_opt	opts[OPTS_NB] = OPTS;
	size_t	opts_nb = OPTS_NB;

	for (size_t i = 0; i < opts_nb; i++)
	{
		if (opts[i].can_interrupt && has_flag(bitmap, i))
			return (i);
	}
	return (-1);
}