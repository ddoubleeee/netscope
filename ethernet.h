#ifndef NETSCOPE_ETHERNET_H
#define NETSCOPE_ETHERNET_H


#include <stdint.h>
#include <linux/if_ether.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

typedef struct EthernetInfo {
	uint8_t src[ETH_ALEN];
	uint8_t dest[ETH_ALEN];
	uint16_t ethertype;
} EthernetInfo;

ssize_t ethernet_parse(const uint8_t *data, size_t data_len, EthernetInfo *result);
void ethernet_print(const EthernetInfo *info);


#endif