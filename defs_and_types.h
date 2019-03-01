/*	Author: ebramkw
	Typedef and definitions	*/

/*---------------------------------------------------------------------------*/
#define SINK_NODE_ID 52739
#define SOURCE_NODE_ID 25351
/*---------------------------------------------------------------------------*/
// #define SENDING_RATE RTIMER_SECOND          // 1 HZ
#define SENDING_RATE RTIMER_SECOND / 10  // 10 HZ
// #define SENDING_RATE RTIMER_SECOND / 100 // 100 HZ
/*---------------------------------------------------------------------------*/
// #define PACKET_SIZE 12  // size in Bytes
#define PACKET_SIZE 32  // size in Bytes
// #define PACKET_SIZE 64  // size in Bytes
/*---------------------------------------------------------------------------*/
#define TIMESTAMP_NUMBERS (PACKET_SIZE / 4) - 2
typedef struct {
  unsigned long dst_id;
  unsigned long timestamp[TIMESTAMP_NUMBERS];
  unsigned long seq;
} data_packet_struct;
/*---------------------------------------------------------------------------*/