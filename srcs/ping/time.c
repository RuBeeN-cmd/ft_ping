#include <time.h>

struct timeval get_elapsed_time(struct timeval *start, struct timeval *end)
{
	long sec = end->tv_sec - start->tv_sec;
	long micros = end->tv_usec - start->tv_usec;
	if (micros < 0) {
		sec--;
		micros += 1000000;
	}
	struct timeval result = {sec, micros};
	return result;
}

double timeval_to_ms(struct timeval tv)
{
	return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);
}

double get_elapsed_ms(struct timeval start, struct timeval end)
{
	return timeval_to_ms(get_elapsed_time(&start, &end));
}
