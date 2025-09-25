#include <colors.h>
#include <stddef.h>
#include <logs.h>

static char	**get_color_list() {
	static char *colors[ANSI_COLORS_COUNT] = ANSI_COLORS;
	return colors;
}

void	disable_colors() {
	char **colors = get_color_list();
	for (size_t i = 0; i < ANSI_COLORS_COUNT; i++) {
		colors[i] = "";
	}
}

void	enable_colors() {
	char **colors = get_color_list();
	char *default_colors[] = ANSI_COLORS;
	for (size_t i = 0; i < ANSI_COLORS_COUNT; i++) {
		colors[i] = default_colors[i];
	}
}

char *get_color(t_color color) {
	char **colors = get_color_list();
	if (color < 0 || color >= ANSI_COLORS_COUNT) {
		return NULL;
	}
	return colors[color];
}