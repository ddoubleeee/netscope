#include <stdint.h>
#include <linux/if_ether.h>
#include <sys/types.h>

typedef struct ARPInfo {
	uint16_t hardware_type;
	uint16_t proto_type;
	uint8_t hardware_len;
	uint8_t proto_len;
	uint16_t opcode;
	/* for now, borrow buffer pointers, prolly needa improve ltr */
	const uint8_t *sender_hardware_addr;
	const uint8_t *sender_proto_addr;
	const uint8_t *target_hardware_addr;
	const uint8_t *target_proto_addr;
} ARPInfo;


ssize_t arp_parse(const uint8_t *buffer, size_t buffer_len, ARPInfo *result);
void arp_print(const ARPInfo *info);