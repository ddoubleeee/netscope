#include "ipv4.h"
#include "parse_error.h"
#include <linux/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>

#define EXPECTED_VERSION 4
#define MIN_HEADER_SIZE 20
#define MAX_HEADER_SIZE 60

ssize_t ipv4_parse(const uint8_t *data, size_t buffer_len, IPv4Info *result) {
	struct iphdr header;
	/* at least 20 bytes of fixed header */
	if (buffer_len < sizeof(struct iphdr)) {return PARSE_ERR_TRUNCATED_HEADER;}
	memcpy(&header, data, sizeof(struct iphdr));

	if (header.version != EXPECTED_VERSION) {return PARSE_ERR_INVALID_VERSION;}
	result->version = header.version;

	/* header size matches ihl */
	size_t header_len = header.ihl * 4;
	if (header_len < MIN_HEADER_SIZE || header_len > MAX_HEADER_SIZE) {return PARSE_ERR_INVALID_LENGTH;}
	/* full header with options is correct len */
	if (header_len > buffer_len) {return PARSE_ERR_TRUNCATED_HEADER;}
	result->ihl = header.ihl;

	result->tos = header.tos;

	uint16_t total_len = ntohs(header.tot_len);
	if (total_len < header_len) {return PARSE_ERR_INVALID_LENGTH;}
	if (total_len > buffer_len) {return PARSE_ERR_TRUNCATED_PAYLOAD;}
	result->total_len = total_len;

	result->id = ntohs(header.id);

	uint8_t flags_byte = ntohs(header.frag_off) >> 13;
	IPv4Flags flags;
	flags.reserved = (flags_byte & 0x04) != 0;
	flags.df = (flags_byte & 0x02) != 0;
	flags.mf = (flags_byte & 0x01) != 0;
	if (flags.reserved != 0) {return PARSE_ERR_RESERVED_BIT;}
	result->flags = flags;

	result->offset = ntohs(header.frag_off) & 0x1FFF;
	result->ttl = header.ttl;
	result->protocol = header.protocol;
	result->checksum = ntohs(header.check);
	result->src_addr = header.saddr;
	result->dest_addr = header.daddr;

	size_t options_len = header_len - sizeof(struct iphdr);
	memcpy(result->options, data + sizeof(struct iphdr), options_len);
	result->options_len = options_len;

	return header_len;
}

void ipv4_print(const IPv4Info *info) {
	char src_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET,	&info->src_addr, src_ip, INET_ADDRSTRLEN);
	char dst_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &info->dest_addr, dst_ip, INET_ADDRSTRLEN);

	printf("===== IPv4 Header ===============\n");
	printf("version:  %u\n", info->version);
	printf("ihl:      %u\n", info->ihl);
	printf("TOS:      %x\n", info->tos);
	printf("ttl len:  %u\n", info->total_len);
	printf("id:       %04x\n", info->id);
	printf("df/mf/re: %u/%u/%u\n", info->flags.df, info->flags.mf, info->flags.reserved);
	printf("frag off: %u\n", info->offset);
	printf("TTL:      %u\n", info->ttl);
	printf("protocol: %u\n", info->protocol);
	printf("checksum: %04x\n", info->checksum);
	printf("src:      %s\n", src_ip);
	printf("dest:     %s\n", dst_ip);
	printf("options:  ");
	if (info->options_len > 0) {
		for (size_t i = 0; i < info->options_len; ++i) {
			printf("%02x ", info->options[i]);
		}
	}
	else {
		printf("None");
	}
	printf("\n");

}