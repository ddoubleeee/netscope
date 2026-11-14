#include <netinet/in.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <unistd.h>
#include "ethernet.h"
#include "ipv4.h"


int main() {

	unsigned char buf[65575];
	int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (fd < 0) {perror("socket"); return EXIT_FAILURE;}
	ssize_t bytes_read = recv(fd, buf, sizeof(buf), 0);
	if (bytes_read < 0) {perror("recv"); close(fd); return EXIT_FAILURE;}
	unsigned char *current = buf;

	EthernetInfo ethernet_hdr;
	ssize_t parsed = ethernet_parse(current, bytes_read, &ethernet_hdr);
	ethernet_print(&ethernet_hdr);
	current += parsed;

	if (ethernet_hdr.ethertype == ETH_P_IP) {
		IPv4Info ipv4_hdr;
		ipv4_parse(current, bytes_read - parsed, &ipv4_hdr);
		ipv4_print(&ipv4_hdr);
	}
	return 0;
}