#ifndef PACKET_H
#define PACKET_H

#include <netinet/ip_icmp.h>
#include <netinet/ip.h>

#define PING_PACKET_DATA_SIZE	56
#define	IP_VERSION				4

typedef struct	s_ping_packet
{
	struct iphdr	ip_header;
	struct icmphdr	icmp_header;
	char			data[PING_PACKET_DATA_SIZE];
}				t_ping_packet;

int		create_packet(struct sockaddr_in dest_addr, t_ping_packet *packet, uint16_t sequence);
void	dbg_packet(t_ping_packet *packet);

#endif