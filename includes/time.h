#ifndef TIME_H
#define TIME_H

#include <sys/time.h>

double			get_elapsed_ms(struct timeval start, struct timeval end);

#endif