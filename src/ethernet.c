#include "ethernet.h"

ssize_t ethernet_parse(const uint8_t *data, size_t data_len, EthernetInfo *result) {
	/* -1 for error */
	if (data_len < sizeof(struct ethhdr)) {return -1;}
	/* fit into header*/
	struct ethhdr header;
	memcpy(&header, data, sizeof(struct ethhdr));

	/* unpack to custom struct */
	memcpy(result->src, header.h_source, ETH_ALEN);
	memcpy(result->dest, header.h_dest, ETH_ALEN);
	result->ethertype = ntohs(header.h_proto);

	return sizeof(struct ethhdr);
}

void ethernet_print(const EthernetInfo *info) {
	printf("===== Ethernet Header ==================================\n");
	printf("dest: ");
	for (int i = 0; i < ETH_ALEN; ++i) {
		printf("%02x ", info->dest[i]);
	}
	printf("\n");

	printf("src:  ");
	for (int i = 0; i < ETH_ALEN; ++i) {
		printf("%02x ", info->src[i]);
	}
	printf("\n");
	printf("EtherType: 0x%04x (%s)\n", info->ethertype, info->ethertype == ETH_P_IP ? "IPv4" : "");
	printf("========================================================\n");

}