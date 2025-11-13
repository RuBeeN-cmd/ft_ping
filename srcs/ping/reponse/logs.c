#include <ft_ping.h>

void	print_response_info(ssize_t captured_bytes, struct in_addr from_addr, char *hostname, t_packet *recv_packet, double time_ms) {
	if (hostname[0]) {
		INFO("%s%zd%s bytes from %s%s%s (%s%s%s): icmp_seq=%s%d%s ttl=%s%d%s time=%s%.3f ms%s\n",
			get_color(COLOR_BLUE), captured_bytes - sizeof(struct iphdr), get_color(COLOR_RESET),
			get_color(COLOR_RED), hostname, get_color(COLOR_RESET),
			get_color(COLOR_PURPLE), inet_ntoa(from_addr), get_color(COLOR_RESET),
			get_color(COLOR_BLUE), ntohs(recv_packet->icmp_header.un.echo.sequence), get_color(COLOR_RESET),
			get_color(COLOR_YELLOW), recv_packet->ip_header.ttl, get_color(COLOR_RESET),
			get_color(COLOR_CYAN), time_ms,
			get_color(COLOR_RESET));
	} else {
		INFO("%s%zd%s bytes from %s%s%s: icmp_seq=%s%d%s ttl=%s%d%s time=%s%.3f ms%s\n",
			get_color(COLOR_BLUE), captured_bytes - sizeof(struct iphdr), get_color(COLOR_RESET),
			get_color(COLOR_PURPLE), inet_ntoa(from_addr), get_color(COLOR_RESET),
			get_color(COLOR_BLUE), ntohs(recv_packet->icmp_header.un.echo.sequence), get_color(COLOR_RESET),
			get_color(COLOR_YELLOW), recv_packet->ip_header.ttl, get_color(COLOR_RESET),
			get_color(COLOR_CYAN), time_ms,
			get_color(COLOR_RESET));
	}
}

void	print_packet_dump(t_packet *packet) {
	INFO("%sIP HDR Dump:%s\n", get_color(COLOR_ORANGE), get_color(COLOR_RESET));
	INFO("%s", get_color(COLOR_GREEN));
	for (size_t i = 0; i < sizeof(struct iphdr); i++) {
		if (i % 2 == 0) {
			printf(" ");
		}
		printf("%02x", *((uint8_t *) &packet->ip_header + i));
	}
	printf("%s\n", get_color(COLOR_RESET));
	INFO("%sVr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data%s\n", get_color(COLOR_ORANGE), get_color(COLOR_RESET));
	char saddr[INET_ADDRSTRLEN] = {};
	char daddr[INET_ADDRSTRLEN] = {};
	inet_ntop(AF_INET, &packet->ip_header.saddr, saddr, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &packet->ip_header.daddr, daddr, INET_ADDRSTRLEN);
	INFO("%*d %*d  %02x %04x %04x %*d %04x  %02x  %02x %04x %s%s%s %s%s%s\n",
		2, packet->ip_header.version,
		2, packet->ip_header.ihl,
		packet->ip_header.tos,
		ntohs(packet->ip_header.tot_len),
		ntohs(packet->ip_header.id),
		3, (ntohs(packet->ip_header.frag_off) >> 13) & 0x7,
		ntohs(packet->ip_header.frag_off) & 0x1FF,
		packet->ip_header.ttl,
		packet->ip_header.protocol,
		ntohs(packet->ip_header.check),
		get_color(COLOR_PURPLE), saddr, get_color(COLOR_ORANGE),
		get_color(COLOR_PURPLE), daddr, get_color(COLOR_RESET)
	);
	INFO("%sICMP: type %d, code %d, size %s%lu%s, id %s0x%04x%s, seq %s0x%04x%s\n",
		get_color(COLOR_ORANGE),
		packet->icmp_header.type,
		packet->icmp_header.code,
		get_color(COLOR_BLUE), sizeof(packet->icmp_header) + sizeof(packet->data), get_color(COLOR_ORANGE),
		get_color(COLOR_GREEN), ntohs(packet->icmp_header.un.echo.id), get_color(COLOR_ORANGE),
		get_color(COLOR_GREEN), ntohs(packet->icmp_header.un.echo.sequence), get_color(COLOR_RESET)
	);
}


void	print_ttl_exceeded_info(t_packet *packet_sent, ssize_t captured_bytes, struct in_addr from_addr, uint8_t verbose) {
	INFO("%s%zd%s bytes from %s%s%s: %sTime to live exceeded%s\n",
		get_color(COLOR_BLUE), captured_bytes - sizeof(struct iphdr), get_color(COLOR_RESET),
		get_color(COLOR_PURPLE), inet_ntoa(from_addr), get_color(COLOR_RESET),
		get_color(COLOR_RED), get_color(COLOR_RESET));
	if (verbose) {
		print_packet_dump(packet_sent);
	}
}