#include <stdint.h>
#include <sys/types.h>

typedef struct UDPInfo {
	uint16_t src_port;
	uint16_t dest_port;
	uint16_t len;
	uint16_t checksum;
} UDPInfo;

ssize_t udp_parse(const uint8_t *buffer, size_t buffer_len, UDPInfo *result);
void udp_print(const UDPInfo *info);
