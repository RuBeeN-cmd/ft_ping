#ifndef SIGNALS_H
#define SIGNALS_H

int		init_signals();
void	set_early_exit(int value);
int		*get_early_exit();
int		is_early_exit();

#endif