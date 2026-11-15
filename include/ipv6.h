#ifndef NETSCOPE_IPV6_H
#define NETSCOPE_IPV6_H


#include <linux/in6.h>
#include <stdint.h>
#include <unistd.h>


typedef struct IPv6Info {
	uint8_t version;
	uint8_t traffic_class;
	uint8_t flow_label[3];
	uint16_t payload_len;
	uint8_t next_header;
	uint8_t hop_limit;
	struct in6_addr src_addr;
	struct in6_addr dest_addr;
} IPv6Info;


ssize_t ipv6_parse(const uint8_t *buffer, size_t buffer_len, IPv6Info *result);
void ipv6_print(const IPv6Info *info);

#endif