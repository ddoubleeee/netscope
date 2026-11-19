#include "tcp.h"
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <linux/tcp.h>
#include "parse_error.h"


ssize_t tcp_parse(const uint8_t *buffer, size_t buffer_len, TCPInfo *result) {
	struct tcphdr header;
	if (buffer_len < sizeof(struct tcphdr)) {return PARSE_ERR_TRUNCATED_HEADER;}
	memcpy(&header, buffer, sizeof(struct tcphdr));

	result->src_port = ntohs(header.source);
	result->dest_port = ntohs(header.dest);
	result->seq = ntohl(header.seq);
	result->ack = ntohl(header.ack_seq);
	result->data_offset = header.doff;
	result->reserved = header.res1;
	result->acc_ecn = header.ae;
	result->flags =
		(header.cwr << 7) |
		(header.ece << 6) |
		(header.urg << 5) |
		(header.ack << 4) |
		(header.psh << 3) |
		(header.rst << 2) |
		(header.syn << 1) |
		header.fin;
	result->window = ntohs(header.window);
	result->checksum = ntohs(header.check);
	result->urgent_ptr = ntohs(header.urg_ptr);
	result->options_len = result->data_offset * 4 - sizeof(struct tcphdr);
	result->options = result->options_len > 0 ? buffer + sizeof(struct tcphdr) : NULL;

	return sizeof(struct tcphdr) + result->options_len;
}

void tcp_print(const TCPInfo *info) {
	printf("===== TCP Header ==========\n");
	printf("src:      %u\n", info->src_port);
	printf("dst:      %u\n", info->dest_port);
	printf("len:      %u\n", info->data_offset);
	printf("checksum: %02x\n", info->checksum);
}