#include <packet.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <libft.h>
#include <ft_ping.h>

void	dbg_packet(t_packet *packet)
{
	char buffer[1024] = {};
	char *crt = buffer; 
	char *color = "\033[0;32m";
	for (int i = 0; (long unsigned) i < sizeof(t_packet); i++)
	{
		if ((unsigned long)i >= sizeof(struct iphdr))
			color = "\033[0;34m";
		if ((unsigned long)i >= sizeof(struct iphdr) + sizeof(struct icmphdr))
			color = "\033[0;35m";
		crt += sprintf(crt, "%s%02X\033[0m ", color, *((uint8_t *) packet + i));
		if (i % 16 == 15) {
			sprintf(crt, "\n");
			DBG("%s", buffer);
			ft_memset(buffer, 0, crt - buffer);
			crt = buffer;
		} else if (i % 8 == 7) {
			crt += sprintf(crt, " ");
		}
	}
	sprintf(crt, "\n");
}

/**
 * @brief https://datatracker.ietf.org/doc/html/rfc1071#section-4.1
 */
uint16_t	 calculate_checksum(void *buffer, size_t length)
{
	uint32_t	sum = 0;
	uint16_t	*ptr = (uint16_t *) buffer;

	while( length > 1 )
	{
		sum += ntohs(*ptr++);
		length -= 2;
	}
	if( length > 0 )
		sum += *(uint8_t *) ptr;
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);
	return (htons(~sum));
}

int	init_ip_header(struct iphdr *ip_header, size_t packet_size, in_addr_t dest_addr)
{
	ip_header->ihl = sizeof(struct iphdr) / 4; 
	ip_header->version = 4;
	ip_header->tos = IPTOS_CLASS_DEFAULT;
	ip_header->tot_len = htons(packet_size);
	ip_header->id = htons(getpid());
	ip_header->frag_off = 0;
	ip_header->ttl = 64;
	ip_header->protocol = IPPROTO_ICMP;
	ip_header->check = 0;

	struct in_addr	src_addr;

	if (get_source_ip(&src_addr)) {
		return (1);
	}

	ip_header->saddr = src_addr.s_addr;
	ip_header->daddr = dest_addr;

	ip_header->check = calculate_checksum(ip_header, sizeof(struct iphdr));
	return (0);
}

void	init_icmp_header(struct icmphdr *icmp_header, uint16_t sequence)
{
	icmp_header->type = ICMP_ECHO;
	icmp_header->code = 0;
	icmp_header->checksum = 0;
	icmp_header->un.echo.id = htons(getpid() & 0xFFFF);
	icmp_header->un.echo.sequence = sequence;
}

void	init_icmp_data(char *data)
{
	ft_bzero(data, PING_PACKET_DATA_SIZE);
	struct timeval tv;
	if (gettimeofday(&tv, NULL) == -1)
	{
		// TODO
		return ;
	}
	ft_memcpy(data, &tv.tv_sec, sizeof(tv.tv_sec));
	for (uint32_t i = sizeof(tv.tv_sec); i < PING_PACKET_DATA_SIZE; i++)
	{
		data[i] = i;
	}
}

int	create_packet(struct sockaddr_in dest_addr, t_packet *packet)
{

	if (init_ip_header(&packet->ip_header, sizeof(packet->ip_header)
 		+ sizeof(packet->icmp_header) + PING_PACKET_DATA_SIZE, dest_addr.sin_addr.s_addr))
	{
		return (1);
	}
	init_icmp_header(&packet->icmp_header, 0);
	init_icmp_data(packet->data);
	packet->icmp_header.checksum = calculate_checksum(&packet->icmp_header, sizeof(packet->icmp_header) + PING_PACKET_DATA_SIZE);
	return (0);
}

int	create_ping_packet(struct sockaddr_in dest_addr, t_ping_packet *p)
{
	if (create_packet(dest_addr, &p->packet))
		return (1);
	ft_bzero(&p->send_timestamp, sizeof(p->send_timestamp));
	return (0);
}

void	update_packet(t_ping_packet *p)
{
	init_icmp_header(&p->packet.icmp_header, p->packet.icmp_header.un.echo.sequence + 1);
	p->packet.icmp_header.checksum = calculate_checksum(&p->packet.icmp_header, sizeof(p->packet.icmp_header) + PING_PACKET_DATA_SIZE);
}