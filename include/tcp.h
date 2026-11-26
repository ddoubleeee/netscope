#include <stdint.h>
#include <sys/types.h>

enum {
    TCP_FIN = 1u << 0,
    TCP_SYN = 1u << 1,
    TCP_RST = 1u << 2,
    TCP_PSH = 1u << 3,
    TCP_ACK = 1u << 4,
    TCP_URG = 1u << 5,
    TCP_ECE = 1u << 6,
    TCP_CWR = 1u << 7
};

typedef struct TCPInfo {
	uint16_t src_port;
	uint16_t dest_port;
	uint32_t seq;
	uint32_t ack;
	uint8_t reserved;
	uint8_t acc_ecn;
	uint8_t flags;
	uint8_t data_offset;
	uint16_t window;
	uint16_t checksum;
	uint16_t urgent_ptr;
	const uint8_t *options;
	uint8_t options_len;


} TCPInfo;

ssize_t tcp_parse(const uint8_t *buffer, size_t buffer_len, TCPInfo *result);
void tcp_print(const TCPInfo *info);