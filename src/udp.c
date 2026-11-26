#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <linux/udp.h>
#include "udp.h"
#include "parse_error.h"


ssize_t udp_parse(const uint8_t *buffer, size_t buffer_len, UDPInfo *result) {
	struct udphdr header;
	if (buffer_len < sizeof(struct udphdr)) {return PARSE_ERR_TRUNCATED_HEADER;}
	memcpy(&header, buffer, sizeof(struct udphdr));


	result->src_port = ntohs(header.source);
	result->dest_port = ntohs(header.dest);
	result->len = ntohs(header.len);
	if (result->len < sizeof(struct udphdr)) {return PARSE_ERR_INVALID_LENGTH;}
	if (result->len > buffer_len) {return PARSE_ERR_TRUNCATED_HEADER;}
	result->checksum = ntohs(header.check);

	return sizeof(struct udphdr);
}


void udp_print(const UDPInfo *info) {
	printf("===== UDP Header ==========\n");
	printf("src:      %u\n", info->src_port);
	printf("dst:      %u\n", info->dest_port);
	printf("len:      %u\n", info->len);
	printf("checksum: %x\n", info->checksum);
}
