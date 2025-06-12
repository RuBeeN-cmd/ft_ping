#include <args.h>

int	has_flag(uint64_t bitmap, uint32_t flag)
{
	return ((bitmap & (1 << flag)));
}