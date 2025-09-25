#include <logs.h>

t_log_level *get_log_level() {
	static t_log_level current_log_level = INFO;
	return &current_log_level;
}

void set_log_level(t_log_level level) {
	t_log_level *current_log_level = get_log_level();
	*current_log_level = level;
}

char **get_log_prefix(t_log_level level) {
	static char *dbg_prefix = DBG_PREFIX" ";
	static char *info_prefix = INFO_PREFIX" ";
	static char *warn_prefix = WARN_PREFIX" ";
	static char *err_prefix = ERR_PREFIX" ";
	if (level == DEBUG)
		return &dbg_prefix;
	else if (level == INFO)
		return &info_prefix;
	else if (level == WARNING)
		return &warn_prefix;
	else if (level == ERROR)
		return &err_prefix; 
	return NULL;
}

void	disable_ansi_colors() {
	char **dbg_prefix = get_log_prefix(DEBUG);
	char **info_prefix = get_log_prefix(INFO);
	char **warn_prefix = get_log_prefix(WARNING);
	char **err_prefix = get_log_prefix(ERROR);
	*dbg_prefix = DBG_PREFIX_NOCOLOR" ";
	*info_prefix = INFO_PREFIX_NOCOLOR" ";
	*warn_prefix = WARN_PREFIX_NOCOLOR" ";
	*err_prefix = ERR_PREFIX_NOCOLOR" ";
}

void	disable_prefix() {
	char **dbg_prefix = get_log_prefix(DEBUG);
	char **info_prefix = get_log_prefix(INFO);
	char **warn_prefix = get_log_prefix(WARNING);
	char **err_prefix = get_log_prefix(ERROR);
	*dbg_prefix = "";
	*info_prefix = "";
	*warn_prefix = "";
	*err_prefix = "";
}