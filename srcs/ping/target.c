#include <ft_ping.h>
#include <netdb.h>

int hostname_to_ipv4_addr(char *hostname, in_addr_t *addr)
{
    struct hostent *host = NULL;

	host = gethostbyname(hostname);
    if (host == NULL) {
        return (1);
    }
    *addr = *(in_addr_t *)host->h_addr_list[0];
    return (0);
}

int	get_dest_addr(struct sockaddr_in *dest_addr, char addr_str[])
{
	dest_addr->sin_family = AF_INET;
	dest_addr->sin_port = 0;
	if (inet_pton(AF_INET, addr_str, &dest_addr->sin_addr.s_addr) <= 0
		&& hostname_to_ipv4_addr(addr_str, &dest_addr->sin_addr.s_addr)) {
		return (1);
	}
	return (0);
}

int	init_target(t_target *target, char *hostname, t_opts *opts) {
	target->arg = hostname;
	if (get_dest_addr(&target->addr, hostname)) {
		WARN("Failed to convert address: %s\n", hostname);
		return (1);
	}
	target->hostname[0] = 0;
	if (!has_flag(opts->bitmap, NUMERIC_OPT)) {
		getnameinfo((struct sockaddr *)&target->addr, sizeof(target->addr),
			target->hostname, sizeof(target->hostname), NULL, 0, 0);
	}

	if (create_ping_packet(target->addr, &target->packet, opts->ttl)) {
		WARN("Failed to create packet for: %s\n", hostname);
		return (1);
	}
	init_stats(&target->stats);
	return (0);
}