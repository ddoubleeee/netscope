#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/if_arp.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "arp.h"
#include "parse_error.h"

ssize_t arp_parse(const uint8_t *buffer, size_t buffer_len, ARPInfo *result) {
	if (buffer_len < sizeof(struct arphdr)) {return PARSE_ERR_TRUNCATED_HEADER;}
	struct arphdr header;
	memcpy(&header, buffer, sizeof(struct arphdr));
	size_t header_var_size = 2 * (header.ar_hln + header.ar_pln);
	if (buffer_len - sizeof(struct arphdr) < header_var_size) {return PARSE_ERR_TRUNCATED_HEADER;}

	result->hardware_type = ntohs(header.ar_hrd);
	result->proto_type = ntohs(header.ar_pro);
	result->hardware_len = header.ar_hln;
	result->proto_len = header.ar_pln;
	result->opcode = ntohs(header.ar_op);

	result->sender_hardware_addr = buffer + sizeof(struct arphdr);
	result->sender_proto_addr = result->sender_hardware_addr + result->hardware_len;
	result->target_hardware_addr = result->sender_proto_addr + result->proto_len;
	result->target_proto_addr = result->target_hardware_addr + result->hardware_len;

	return sizeof(struct arphdr) + header_var_size;
}

void arp_print(const ARPInfo *info) {
	/* IPv4 - might needa extend later? */
	char sender_ip[INET_ADDRSTRLEN];
	char target_ip[INET_ADDRSTRLEN];
	if (info->proto_type == ETH_P_IP && info->proto_len == sizeof(struct in_addr)) {
		inet_ntop(AF_INET, info->sender_proto_addr, sender_ip, INET_ADDRSTRLEN);
		inet_ntop(AF_INET, info->target_proto_addr, target_ip, INET_ADDRSTRLEN);
	}
	printf("====== ARP Header ===============\n");
	printf("hard t:  %u\n", info->hardware_type);
	printf("proto t: 0x%x\n", info->proto_type);
	printf("opcode:  %u\n", info->opcode);
	printf("send hw: ");
	for (size_t i = 0; i < info->hardware_len; ++i) {
		printf("%x ", info->sender_hardware_addr[i]);
	}
	printf("\n");
	printf("send pt: %s\n", sender_ip);
	printf("targ hw: ");
	for (size_t i = 0; i < info->hardware_len; ++i) {
		printf("%x ", info->target_hardware_addr[i]);
	}
	printf("\n");
	printf("targ pt: %s\n", target_ip);
}