#include <logs.h>

t_log_level *get_log_level() {
	static t_log_level current_log_level = INFO;
	return &current_log_level;
}

void set_log_level(t_log_level level) {
	t_log_level *current_log_level = get_log_level();
	*current_log_level = level;
}


