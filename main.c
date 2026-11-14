#include "ethernet.h"

#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define IP

int main(int argc, const char *argv[]) {

	unsigned char buf[65575];
	int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (fd < 0) {perror("socket"); return EXIT_FAILURE;}
	ssize_t bytes_read = recv(fd, buf, sizeof(buf), 0);
	if (bytes_read < 0) {perror("recv"); close(fd); return EXIT_FAILURE;}
	unsigned char *current = buf;

	EthernetInfo ethernet_hdr;
	ssize_t parsed = ethernet_parse(current, sizeof(current), &ethernet_hdr);
	ethernet_print(&ethernet_hdr);
	current += parsed;
	/*
	switch (ethertype) {
		case ETH_P_IP: {
			struct iphdr ip4;
			memcpy(&ip4, current, sizeof(struct iphdr));

			char src_ip[INET_ADDRSTRLEN];
			inet_ntop(AF_INET,	&ip4.saddr, src_ip, INET_ADDRSTRLEN);
			char dst_ip[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &ip4.daddr, dst_ip, INET_ADDRSTRLEN);
			size_t options_len = ip4.ihl * 4 - sizeof(struct iphdr);

			printf("===== IPv4 Header ===============\n");
			printf("version:  %u\n", ip4.version);
			printf("ihl:      %u\n", ip4.ihl);
			printf("TOS:      %x\n", ip4.tos);
			printf("ttl len:  %u\n", ntohs(ip4.tot_len));
			printf("id:       %04x\n", ntohs(ip4.id));
			printf("flags:    %03b\n", ntohs(ip4.frag_off) >> 13);
			printf("frag off: %u\n", ntohs(ip4.frag_off) & 0x1FFF);
			printf("TTL:      %u\n", ip4.ttl);
			printf("protocol: %u\n", ip4.protocol);
			printf("checksum: %04x\n", ntohs(ip4.check));
			printf("src:      %s\n", src_ip);
			printf("dest:     %s\n", dst_ip);
			printf("options:  ");
			if (options_len == 0) {
				printf("None");
			}
			else {
				for (int i = 0; i < options_len; ++i) {
					printf("%x", *(current + sizeof(struct iphdr) + i));
				}
			}
			printf("\n===============================\n");

			current += ip4.ihl * 4;
			break;
		}
	}
*/
	return 0;
}