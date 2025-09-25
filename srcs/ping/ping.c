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

int	capture_response(t_stats *stats, t_ping_packet *p, int recv_socket)
{
	struct sockaddr_in from_addr = {};
	socklen_t addr_size = sizeof(from_addr);

	char buffer[1024] = {};
	ssize_t captured_bytes = recvfrom(recv_socket, buffer, sizeof(buffer), 0, 
									  (struct sockaddr *) &from_addr, &addr_size);
	if (captured_bytes == -1)
		return (1);
	struct timeval send_time;
	gettimeofday(&send_time, NULL);
	
	long seconds = send_time.tv_sec - p->send_timestamp.tv_sec;
	long micros = send_time.tv_usec - p->send_timestamp.tv_usec;
	if (micros < 0) {
		seconds--;
		micros += 1000000;
	}
	add_time(stats, seconds * 1000 + micros / 1000.0);
	INFO("%zd bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n" \
		, captured_bytes - sizeof(struct iphdr), inet_ntoa(from_addr.sin_addr), \
		p->packet.icmp_header.un.echo.sequence, p->packet.ip_header.ttl, \
		seconds * 1000 + micros / 1000.0);
	return (0);
}