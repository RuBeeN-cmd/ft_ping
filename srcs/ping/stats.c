#include <math.h>
#include <ft_ping.h>

void	init_stats(t_stats *stats)
{
	stats->sent = 0;
	stats->received = 0;
	stats->lost = 0;
	stats->min_time = -1;
	stats->avg_time = 0;
	stats->max_time = -1;
	stats->stddev_time = 0;
	stats->times = NULL;
}

void	add_time(t_stats *stats, double time)
{
	if (stats->min_time == -1 || time < stats->min_time)
		stats->min_time = time;
	if (stats->max_time == -1 || time > stats->max_time)
		stats->max_time = time;
	double *time_ptr = malloc(sizeof(double));
	if (!time_ptr)
		return ;
	*time_ptr = time;
	stats->received++;
	ft_lstadd_back(&stats->times, ft_lstnew(time_ptr));
}

void	free_stats(t_stats *stats)
{
	if (stats->times) {
		DBG("Freeing stats times list\n");
		ft_lstclear(&stats->times, free);
	}
}

void	calculate_average(t_stats *stats)
{
	if (!stats->times)
		return ;
	double sum = 0;
	size_t count = 0;
	t_list *current = stats->times;
	while (current)
	{
		sum += *(double *)current->content;
		count++;
		current = current->next;
	}
	if (count > 0)
		stats->avg_time = sum / count;
}

void	calculate_stddev(t_stats *stats)
{
	if (!stats->times || stats->avg_time == 0)
		return ;
	double sum = 0;
	size_t count = 0;
	t_list *current = stats->times;
	while (current)
	{
		double diff = *(double *)current->content - stats->avg_time;
		sum += diff * diff;
		count++;
		current = current->next;
	}
	if (count > 1)
		stats->stddev_time = sqrt(sum / (count - 1));
}

void	calculate_metrics(t_stats *stats)
{
	calculate_average(stats);
	calculate_stddev(stats);
}

void	show_stats(t_stats *stats, char *host)
{
	INFO("--- %s%s%s ping statistics ---\n", get_color(COLOR_PURPLE), host, get_color(COLOR_RESET));
	double	percent_loss = stats->sent ? ((stats->sent - stats->received) * 100.0 / stats->sent) : 0;
	char	*percent_loss_color = percent_loss == 0 ? get_color(COLOR_GREEN) : (percent_loss < 100 ? get_color(COLOR_ORANGE) : get_color(COLOR_RED));
	INFO("%s%d%s packets transmitted, %s%d%s packets received, %s%d%%%s packet loss\n",
		get_color(COLOR_BLUE), stats->sent, get_color(COLOR_RESET),
		percent_loss_color, stats->received, get_color(COLOR_RESET),
		percent_loss_color, (int) percent_loss, get_color(COLOR_RESET));
	if (stats->received) {
		INFO("round-trip %smin%s/%savg%s/%smax%s/%sstddev%s = %s%.3f%s/%s%.3f%s/%s%.3f%s/%s%.3f%s ms\n",
			get_color(COLOR_GREEN), get_color(COLOR_RESET),
			get_color(COLOR_ORANGE), get_color(COLOR_RESET),
			get_color(COLOR_RED), get_color(COLOR_RESET),
			get_color(COLOR_BLUE), get_color(COLOR_RESET),
			get_color(COLOR_GREEN), stats->min_time, get_color(COLOR_RESET),
			get_color(COLOR_ORANGE), stats->avg_time, get_color(COLOR_RESET),
			get_color(COLOR_RED), stats->max_time, get_color(COLOR_RESET),
			get_color(COLOR_BLUE), stats->stddev_time, get_color(COLOR_RESET));
	}
}