#include "contiki.h"
#include "dev/leds.h"
#include <stdio.h>
#include "core/net/rime/rime.h"
#include "dev/serial-line.h"
#include "dev/uart1.h"
#include "node-id.h"
#include "defs_and_types.h"

/*---------------------------------------------------------------------------*/
// sender timer
static struct rtimer rt;
static struct pt pt;
/*---------------------------------------------------------------------------*/
static data_packet_struct received_packet;
static data_packet_struct data_packet;
unsigned long curr_timestamp;
/*---------------------------------------------------------------------------*/
PROCESS(cc2650_comm_process, "cc2650 communication process");
AUTOSTART_PROCESSES(&cc2650_comm_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  memcpy(&received_packet, packetbuf_dataptr(), sizeof(data_packet_struct));

  if(received_packet.dst_id == node_id && node_id == SINK_NODE_ID){
    leds_on(LEDS_RED);
    printf("Received packet with sequence number %lu and timestamp %3lu.%03lu\n", received_packet.seq, received_packet.timestamp[0] / 1000, received_packet.timestamp[0] % 1000);
    leds_off(LEDS_RED);
  }
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/
char sender_scheduler(struct rtimer *t, void *ptr) {
  static uint8_t i = 0;
  PT_BEGIN(&pt);
  while(1){
    leds_on(LEDS_RED);
    data_packet.seq++;
    curr_timestamp = (clock_time()*1e3) / CLOCK_SECOND;
    // memcpy(&data_packet.timestamp[0], &curr_timestamp, TIMESTAMP_NUMBERS * sizeof(unsigned long));
    for(i = 0; i < TIMESTAMP_NUMBERS; i++){
      data_packet.timestamp[i] = curr_timestamp;
    }

    printf("sending packet with sequence number %lu and timestamp %3lu.%03lu\n", data_packet.seq, curr_timestamp / 1000, curr_timestamp % 1000);

    packetbuf_copyfrom(&data_packet, (int)sizeof(data_packet_struct));
    broadcast_send(&broadcast);
    leds_off(LEDS_RED);

    rtimer_set(t, RTIMER_TIME(t) + SENDING_RATE, 1, (rtimer_callback_t)sender_scheduler, ptr);
    PT_YIELD(&pt);
  }
  PT_END(&pt);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(cc2650_comm_process, ev, data)
{
  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();

  broadcast_open(&broadcast, 129, &broadcast_call);

  // for serial port
  #if !WITH_UIP && !WITH_UIP6
  uart1_set_input(serial_line_input_byte);
  serial_line_init();
  #endif

  if(node_id == SOURCE_NODE_ID){ // source
    printf("CC2650 communication - I'm source\n");
    printf("Will be sending packet of size %d Bytes\n", (int)sizeof(data_packet_struct));

    // initialize data packet
    data_packet.dst_id = SINK_NODE_ID;
    data_packet.seq = 0;

    // Start sender in one millisecond.
    rtimer_set(&rt, RTIMER_NOW() + (RTIMER_SECOND / 1000), 1, (rtimer_callback_t)sender_scheduler, NULL);

  } else if(node_id == SINK_NODE_ID){ // sink
    printf("CC2650 communication - I'm sink\n");
    leds_on(LEDS_GREEN);
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/