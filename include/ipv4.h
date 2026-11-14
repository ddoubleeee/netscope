#ifndef NETSCOPE_IPV4_H
#define NETSCOPE_IPV4_H

#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>


typedef struct IPv4Flags {
	uint8_t reserved;
	uint8_t df;
	uint8_t mf;
} IPv4Flags;


typedef struct IPv4Info {
	uint8_t version;
	uint8_t ihl;
	uint8_t tos;
	uint16_t total_len;
	uint16_t id;
	IPv4Flags flags;
	uint16_t offset;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t checksum;
	uint32_t src_addr;
	uint32_t dest_addr;
	uint8_t options[40];
	size_t options_len;

} IPv4Info;

ssize_t ipv4_parse(const uint8_t *data, size_t data_len, IPv4Info *result);
void ipv4_print(const IPv4Info *info);


#endif