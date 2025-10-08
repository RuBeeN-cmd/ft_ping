#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>
#include <bits/sigaction.h>
#include <stddef.h>

int		init_signals();
void	set_early_exit(int value);
int		*get_early_exit();
int		is_early_exit();

#endif