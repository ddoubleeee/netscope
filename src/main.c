#include <linux/ipv6.h>
#include <linux/udp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#include "ethernet.h"
#include "ipv4.h"
#include "ipv6.h"
#include "udp.h"
#include "arp.h"
#include "tcp.h"

int main() {

	unsigned char buf[65575];
	int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (fd < 0) {perror("socket"); return EXIT_FAILURE;}
	ssize_t bytes_read = recv(fd, buf, sizeof(buf), 0);
	if (bytes_read < 0) {perror("recv"); close(fd); return EXIT_FAILURE;}
	unsigned char *current = buf;
	size_t remaining = bytes_read;

	EthernetInfo ethernet_hdr;
	ssize_t parsed = ethernet_parse(current, bytes_read, &ethernet_hdr);
	if (parsed < 0) {
    	fprintf(stderr, "Parsing failed: %zd\n", parsed);
    	close(fd);
    	return EXIT_FAILURE;
    }
	ethernet_print(&ethernet_hdr);
	current += parsed;
	remaining -= parsed;

	if (ethernet_hdr.ethertype == ETH_P_IP) {
		IPv4Info ipv4_hdr;
		ssize_t parsed = ipv4_parse(current, remaining, &ipv4_hdr);
		if (parsed < 0) {return EXIT_FAILURE;}
		current += parsed;
		remaining -= parsed;
		ipv4_print(&ipv4_hdr);
		if (ipv4_hdr.protocol == IPPROTO_UDP) {
			UDPInfo udp_hdr;
			udp_parse(current, remaining, &udp_hdr);
			udp_print(&udp_hdr);
		}
		else if (ipv4_hdr.protocol == IPPROTO_TCP) {
			TCPInfo tcp_hdr;
			tcp_parse(current, remaining, &tcp_hdr);
			tcp_print(&tcp_hdr);
		}
	}
	else if (ethernet_hdr.ethertype == ETH_P_IPV6) {
		IPv6Info ipv6_hdr;
		ssize_t parsed = ipv6_parse(current, remaining, &ipv6_hdr);
		if (parsed < 0) {return EXIT_FAILURE;}
		current += parsed;
		remaining -= parsed;
		ipv6_print(&ipv6_hdr);
		if (ipv6_hdr.next_header == IPPROTO_UDP) {
			UDPInfo udp_hdr;
			udp_parse(current, remaining, &udp_hdr);
			udp_print(&udp_hdr);
		}
	}
	else if (ethernet_hdr.ethertype == ETH_P_ARP) {
		ARPInfo arp_hdr;
		arp_parse(current, remaining, &arp_hdr);
		arp_print(&arp_hdr);
	}
	return 0;
}