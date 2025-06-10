#include <args.h>

int main(int argc, char *argv[]) {
	if (parse_args(argc, argv))
	{
		return (1);
	}
	return (0);
}