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
	ft_lstadd_back(&stats->times, ft_lstnew(time_ptr));
}

void	free_stats(t_stats *stats)
{
	if (stats->times)
		ft_lstclear(&stats->times, free);
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