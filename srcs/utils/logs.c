#include <logs.h>
#include <libft.h>

t_log_level *get_log_level() {
	static t_log_level current_log_level = LEVEL_INFO;
	return &current_log_level;
}

void set_log_level(t_log_level level) {
	t_log_level *current_log_level = get_log_level();
	*current_log_level = level;
}

static int *get_empty_prefix() {
	static int empty_prefix = 0;
	return &empty_prefix;
}

char *get_log_prefix(t_log_level level) {
	static char buff[128] = {};
	int 		empty_prefix = *get_empty_prefix();
	ft_bzero(buff, sizeof(buff));

	if (empty_prefix)
		return buff;
	int idx = 0;
	if (level == LEVEL_DEBUG) {
		idx += sprintf(buff + idx, "[%s", get_color(COLOR_PURPLE));
		idx += sprintf(buff + idx, DBG_PREFIX);
		idx += sprintf(buff + idx, "%s] ", get_color(COLOR_RESET));
	}
	else if (level == LEVEL_INFO) {
		idx += sprintf(buff + idx, "[%s", get_color(COLOR_GREEN));
		idx += sprintf(buff + idx, INFO_PREFIX);
		idx += sprintf(buff + idx, "%s] ", get_color(COLOR_RESET));
	}
	else if (level == LEVEL_WARNING) {
		idx += sprintf(buff + idx, "[%s", get_color(COLOR_ORANGE));
		idx += sprintf(buff + idx, WARN_PREFIX);
		idx += sprintf(buff + idx, "%s] ", get_color(COLOR_RESET));
	}
	else if (level == LEVEL_ERROR) {
		idx += sprintf(buff + idx, "[%s", get_color(COLOR_RED));
		idx += sprintf(buff + idx, ERR_PREFIX);
		idx += sprintf(buff + idx, "%s] ", get_color(COLOR_RESET));
	}
	return buff;
}

void	disable_prefix() {
	int *empty_prefix = get_empty_prefix();
	*empty_prefix = 1;
}