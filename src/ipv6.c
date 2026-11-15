#include "ipv6.h"
#include "parse_error.h"
#include <arpa/inet.h>
#include <linux/ipv6.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define EXPECTED_VERSION 6


ssize_t ipv6_parse(const uint8_t *buffer, size_t buffer_len, IPv6Info *result) {
	struct ipv6hdr header;
	if (buffer_len < sizeof(struct ipv6hdr)) {return PARSE_ERR_TRUNCATED_HEADER;}
	memcpy(&header, buffer, sizeof(struct ipv6hdr));
	if (header.version != EXPECTED_VERSION) {return PARSE_ERR_INVALID_VERSION;}
	uint16_t payload_len = ntohs(header.payload_len);
	if (sizeof(struct ipv6hdr) + payload_len > buffer_len) {return PARSE_ERR_TRUNCATED_PAYLOAD;}


	result->version = header.version;
	/* 4 last bits of traffic class + 4 first bits of flow label */
	result->traffic_class = (header.priority << 4) | (header.flow_lbl[0] >> 4);
	/* discard priority bits*/
	result->flow_label[0] = header.flow_lbl[0] & 0x0F;
	result->flow_label[1] = header.flow_lbl[1];
	result->flow_label[2] = header.flow_lbl[2];
	result->payload_len = payload_len;
	result->next_header = header.nexthdr;
	result->hop_limit = header.hop_limit;
	result->src_addr = header.saddr;
	result->dest_addr = header.daddr;

	return sizeof(struct ipv6hdr);
}

void ipv6_print(const IPv6Info *info) {
	char src_ip[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET6, &info->src_addr, src_ip, INET6_ADDRSTRLEN);
	char dest_ip[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET6, &info->dest_addr, dest_ip, INET6_ADDRSTRLEN);

	printf("===== IPv6 Header ===============\n");
	printf("version:  %u\n", info->version);
	printf("traffic:  0x%02x\n", info->traffic_class);
	printf("flow lbl: 0x%01x%02x%02x\n", info->flow_label[0], info->flow_label[1], info->flow_label[2]);
	printf("pay len:  %u\n", info->payload_len);
	printf("next hdr: %u\n", info->next_header);
	printf("hop lim:  %u\n", info->hop_limit);
	printf("src:      %s\n", src_ip);
	printf("dest:     %s\n", dest_ip);
	printf("\n");

}