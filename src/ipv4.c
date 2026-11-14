#include "ipv4.h"

ssize_t ipv4_parse(const uint8_t *data, size_t data_len, IPv4Info *result) {
	struct iphdr header;
	if (data_len < sizeof(struct iphdr)) {return -1;}
	memcpy(&header, data, sizeof(struct iphdr));

	result->version = header.version;
	result->ihl = header.ihl;
	result->tos = header.tos;
	result->total_len = ntohs(header.tot_len);
	result->id = ntohs(header.id);

	uint8_t flags_byte = ntohs(header.frag_off) >> 13;
	IPv4Flags flags;
	flags.reserved = flags_byte & 0x04;
	flags.df = flags_byte & 0x02;
	flags.mf = flags_byte & 0x01;

	result->flags = flags;
	result->offset = ntohs(header.frag_off) & 0x1FFF;
	result->ttl = header.ttl;
	result->protocol = header.protocol;
	result->checksum = ntohs(header.check);
	result->src_addr = header.saddr;
	result->dest_addr = header.daddr;

	result->options_len = header.ihl * 4 - sizeof(struct iphdr);
	memcpy(result->options, data + sizeof(struct iphdr), result->options_len);

	return sizeof(struct iphdr) + result->options_len;
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
	printf("options: ");
	for (size_t i = 0; i < info->options_len; ++i) {
		printf("%02x ", info->options[i]);
	}
	printf("\n");

}