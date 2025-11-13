#include <ft_ping.h>
#include <time.h>

int	check_response(ssize_t captured_bytes, char *buffer, uint16_t expected_id, uint16_t expected_seq) {
	if (captured_bytes < (ssize_t)(sizeof(struct iphdr) + sizeof(struct icmphdr))) {
		DBG("Received packet is too small (%zd bytes)\n", captured_bytes);
		return (0);
	}

	if (((struct iphdr *)buffer)->protocol != IPPROTO_ICMP) {
		DBG("Received packet is not ICMP (protocol %d)\n", ((struct iphdr *)buffer)->protocol);
		return (0);
	}

	t_packet *recv_packet = (t_packet *)(buffer);
	if (recv_packet->icmp_header.type != ICMP_ECHOREPLY && recv_packet->icmp_header.type != ICMP_TIME_EXCEEDED) {
		DBG("Received ICMP packet is not ECHOREPLY (type %d)\n", recv_packet->icmp_header.type);
		return (0);
	}
	if (recv_packet->icmp_header.type == ICMP_ECHOREPLY) {
		if (recv_packet->icmp_header.un.echo.id != expected_id) {
			DBG("Received ICMP packet ID does not match (got %d, expected %d)\n", ntohs(recv_packet->icmp_header.un.echo.id), expected_id);
			return (0);
		}
		if (recv_packet->icmp_header.un.echo.sequence != expected_seq) {
			DBG("Received ICMP packet sequence does not match (got %d, expected %d)\n", ntohs(recv_packet->icmp_header.un.echo.sequence), expected_seq);
			return (0);
		}
	}
	return (1);
}

int	capture_response(t_target *target, int recv_socket, uint8_t verbose)
{
	struct sockaddr_in from_addr = {};
	socklen_t addr_size = sizeof(from_addr);

	char buffer[1024] = {};
	ssize_t captured_bytes = recvfrom(recv_socket, buffer, sizeof(buffer), 0, 
									  (struct sockaddr *) &from_addr, &addr_size);
	if (captured_bytes == -1)
		return (1);
	DBG("Captured %zd bytes from %s\n", captured_bytes, inet_ntoa(from_addr.sin_addr));
	struct timeval recv_time;
	gettimeofday(&recv_time, NULL);

	if (!check_response(captured_bytes, buffer, target->packet.packet.icmp_header.un.echo.id, target->packet.packet.icmp_header.un.echo.sequence)) {
		return (capture_response(target, recv_socket, verbose));
	}

	t_packet *recv_packet = (t_packet *)(buffer);
	if (recv_packet->icmp_header.type == ICMP_TIME_EXCEEDED) {
		print_ttl_exceeded_info(&target->packet.packet, captured_bytes, from_addr.sin_addr, verbose);
	} else {
		double time_ms = get_elapsed_ms(target->packet.send_timestamp, recv_time);
		add_time(&target->stats, time_ms);
		print_response_info(captured_bytes, from_addr.sin_addr, target->hostname, recv_packet, time_ms);
	}
	return (0);
}