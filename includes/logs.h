#ifndef LOGS_H
#define LOGS_H

#include <color.h>
#include <stdio.h>

typedef enum e_log_level {
	DEBUG,
	INFO,
	WARNING,
	ERROR
} t_log_level;

#define DBG_PREFIX	"["PURPLE"DBG"RESET"]"
#define INFO_PREFIX	"["GREEN"INF"RESET"]"
#define WARN_PREFIX	"["YELLOW"WRN"RESET"]"
#define ERR_PREFIX	"["RED"ERR"RESET"]"

#define DBG_PREFIX_NOCOLOR	"[DBG]"
#define INFO_PREFIX_NOCOLOR	"[INF]"
#define WARN_PREFIX_NOCOLOR	"[WRN]"
#define ERR_PREFIX_NOCOLOR	"[ERR]"

#define LOG(_level_, _format_, ...) do { \
	if (_level_ >= *get_log_level()) { \
		switch (_level_) \
		{ \
			case DEBUG: \
				printf("%s" _format_, *get_log_prefix(DEBUG), ##__VA_ARGS__); \
				break; \
			case INFO: \
				printf("%s" _format_, *get_log_prefix(INFO), ##__VA_ARGS__); \
				break; \
			case WARNING: \
				fprintf(stderr, "%s" _format_, *get_log_prefix(WARNING), ##__VA_ARGS__); \
				break; \
			case ERROR: \
				fprintf(stderr, "%s" _format_, *get_log_prefix(ERROR), ##__VA_ARGS__); \
				break; \
			default: \
				break; \
		} \
	} \
} while(0)

#define DBG(_format_, ...) LOG(DEBUG, _format_, ##__VA_ARGS__)
#define INFO(_format_, ...) LOG(INFO, _format_, ##__VA_ARGS__)
#define WARN(_format_, ...) LOG(WARNING, _format_, ##__VA_ARGS__)
#define ERR(_format_, ...) LOG(ERROR, _format_, ##__VA_ARGS__)

t_log_level *get_log_level();
void		set_log_level(t_log_level level);

char 	**get_log_prefix(t_log_level level);
void	disable_ansi_colors();
void	disable_prefix();

#endif