#ifndef LOGS_H
#define LOGS_H

#include <colors.h>
#include <stdio.h>

typedef enum e_log_level {
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_WARNING,
	LEVEL_ERROR
} t_log_level;

#define DBG_PREFIX	"DBG"
#define INFO_PREFIX	"INF"
#define WARN_PREFIX	"WRN"
#define ERR_PREFIX	"ERR"

#define LOG(_level_, _format_, ...) do { \
	if (_level_ >= *get_log_level()) { \
		switch (_level_) \
		{ \
			case LEVEL_DEBUG: \
				printf("%s" _format_, get_log_prefix(LEVEL_DEBUG), ##__VA_ARGS__); \
				break; \
			case LEVEL_INFO: \
				printf("%s" _format_, get_log_prefix(LEVEL_INFO), ##__VA_ARGS__); \
				break; \
			case LEVEL_WARNING: \
				fprintf(stderr, "%s" _format_, get_log_prefix(LEVEL_WARNING), ##__VA_ARGS__); \
				break; \
			case LEVEL_ERROR: \
				fprintf(stderr, "%s" _format_, get_log_prefix(LEVEL_ERROR), ##__VA_ARGS__); \
				break; \
			default: \
				break; \
		} \
	} \
} while(0)

#define DBG(_format_, ...) LOG(LEVEL_DEBUG, _format_, ##__VA_ARGS__)
#define INFO(_format_, ...) LOG(LEVEL_INFO, _format_, ##__VA_ARGS__)
#define WARN(_format_, ...) LOG(LEVEL_WARNING, _format_, ##__VA_ARGS__)
#define ERR(_format_, ...) LOG(LEVEL_ERROR, _format_, ##__VA_ARGS__)

t_log_level *get_log_level();
void		set_log_level(t_log_level level);

char 	*get_log_prefix(t_log_level level);
void	disable_prefix();

#endif