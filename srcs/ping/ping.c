#include <args.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <unistd.h>

#include <ft_ping.h>

uint8_t send_echo_request(int socket, struct sockaddr_in dest, t_ping_packet packet)
{
	ssize_t	send_ret = 0;
	
	printf("Sending Packet:\n");
	(void) socket;
	(void) dest;
	(void) packet;
	dbg_packet(&packet);

	printf("Socket: %d\n", socket);
	printf("Destination: %s\n", inet_ntoa(dest.sin_addr));
	printf("Packet size: %zu\n", sizeof(packet));
	send_ret = sendto(socket, &packet, sizeof(packet), 0, (struct sockaddr *) &dest, sizeof(dest));
	if (send_ret == -1)
		return (1);
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

int	capture_response(t_ping_packet *packet_sent, int recv_socket)
{
	(void) packet_sent;
	struct sockaddr_in from_addr = {};
	socklen_t addr_size = sizeof(from_addr);

	INFO("Receiving Packet...\n");
	DBG("Socket: %d\n", recv_socket);
	
	char buffer[1024] = {};
	ssize_t captured_bytes = recvfrom(recv_socket, buffer, sizeof(buffer), 0, 
									  (struct sockaddr *) &from_addr, &addr_size);
	if (captured_bytes == -1) {
		perror("recvfrom");
		return (1);
	}
	INFO("Packet Received !\n");
	DBG("Received %zd bytes from %s\n", captured_bytes, inet_ntoa(from_addr.sin_addr));
	dbg_packet((t_ping_packet *) &buffer);
	
	struct iphdr *ip_hdr = (struct iphdr *) buffer;
	struct icmphdr *icmp_hdr = (struct icmphdr *) (buffer + (ip_hdr->ihl * 4));
	DBG("ICMP Type: %d, Code: %d\n", icmp_hdr->type, icmp_hdr->code);
	return (0);
}