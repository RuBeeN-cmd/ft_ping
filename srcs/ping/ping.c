#include <args.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ft_ping.h>

uint8_t send_echo_request(int socket, struct sockaddr_in dest, t_ping_packet *p)
{
	ssize_t	send_ret = 0;

	send_ret = sendto(socket, &(p->packet), sizeof(p->packet), 0, (struct sockaddr *) &dest, sizeof(dest));
	if (send_ret == -1)
		return (1);
	struct timeval send_time;
	gettimeofday(&send_time, NULL);
	p->send_timestamp = send_time;
	return (0);
}

int get_source_ip(struct in_addr *src_addr)
{
	struct sockaddr_in	addr;
	socklen_t			addr_len = sizeof(addr);
	int					sock;
	
	struct sockaddr_in	remote;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sock < 0) {
        return (1);
    }
    
    remote.sin_family = AF_INET;
    remote.sin_port = htons(53);
    inet_pton(AF_INET, "8.8.8.8", &remote.sin_addr);

    if (connect(sock, (struct sockaddr *) &remote, sizeof(remote)) < 0) {
        close(sock);
        return (1);
    }

	if (getsockname(sock, (struct sockaddr *) &addr, &addr_len) < 0)
	{
        close(sock);
        return (1);
    }
	*src_addr = addr.sin_addr;
	close(sock);
	return (0);
}

static struct timeval calculate_time_diff(struct timeval *start, struct timeval *end)
{
	long sec = end->tv_sec - start->tv_sec;
	long micros = end->tv_usec - start->tv_usec;
	if (micros < 0) {
		sec--;
		micros += 1000000;
	}
	struct timeval result = {sec, micros};
	return result;
}

int	capture_response(t_stats *stats, t_ping_packet *p, int recv_socket)
{
	struct sockaddr_in from_addr = {};
	socklen_t addr_size = sizeof(from_addr);

	char buffer[1024] = {};
	ssize_t captured_bytes = recvfrom(recv_socket, buffer, sizeof(buffer), 0, 
									  (struct sockaddr *) &from_addr, &addr_size);
	if (captured_bytes == -1)
		return (1);
	struct timeval recv_time;
	gettimeofday(&recv_time, NULL);

	if (captured_bytes < (ssize_t)(sizeof(struct iphdr) + sizeof(struct icmphdr))) {
		DBG("Received packet is too small (%zd bytes)\n", captured_bytes);
		return (capture_response(stats, p, recv_socket));
	}

	if (((struct iphdr *)buffer)->protocol != IPPROTO_ICMP) {
		DBG("Received packet is not ICMP (protocol %d)\n", ((struct iphdr *)buffer)->protocol);
		return (capture_response(stats, p, recv_socket));
	}

	t_packet *recv_packet = (t_packet *)(buffer);
	if (recv_packet->icmp_header.type != ICMP_ECHOREPLY) {
		if (recv_packet->icmp_header.type == ICMP_TIME_EXCEEDED) {
			INFO("%s%zd%s bytes from %s%s%s: %sTime to live exceeded%s\n",
				get_color(COLOR_BLUE), captured_bytes - sizeof(struct iphdr), get_color(COLOR_RESET),
				get_color(COLOR_PURPLE), inet_ntoa(from_addr.sin_addr), get_color(COLOR_RESET),
				get_color(COLOR_RED), get_color(COLOR_RESET));
			return (0);
		}
		DBG("Received ICMP packet is not ECHOREPLY (type %d)\n", recv_packet->icmp_header.type);
		return (capture_response(stats, p, recv_socket));
	}
	if (recv_packet->icmp_header.un.echo.id != p->packet.icmp_header.un.echo.id
		|| recv_packet->icmp_header.un.echo.sequence != p->packet.icmp_header.un.echo.sequence) {
		DBG("Received ICMP packet ID does not match (got %d, expected %d)\n", ntohs(recv_packet->icmp_header.un.echo.id), getpid());
		return (capture_response(stats, p, recv_socket));
	}

	struct timeval time_diff = calculate_time_diff(&p->send_timestamp, &recv_time);
	add_time(stats, time_diff.tv_sec * 1000 + time_diff.tv_usec / 1000.0);
	INFO("%s%zd%s bytes from %s%s%s: icmp_seq=%s%d%s ttl=%s%d%s time=%s%.3f ms%s\n",
		get_color(COLOR_BLUE), captured_bytes - sizeof(struct iphdr), get_color(COLOR_RESET),
		get_color(COLOR_PURPLE), inet_ntoa(from_addr.sin_addr), get_color(COLOR_RESET),
		get_color(COLOR_BLUE), recv_packet->icmp_header.un.echo.sequence, get_color(COLOR_RESET),
		get_color(COLOR_YELLOW), recv_packet->ip_header.ttl, get_color(COLOR_RESET),
		get_color(COLOR_CYAN), time_diff.tv_sec * 1000 + time_diff.tv_usec / 1000.0,
		get_color(COLOR_RESET));
	return (0);
}