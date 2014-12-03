#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "dev/cc2420.h"
#include "dev/leds.h"
#include <stdio.h>
#include <string.h>
#include "contiki.h"
#if defined(__AVR__)
#include <avr/io.h>
#endif
#include "dev/leds.h"
#include "dev/spi.h"
#include "cc2420.h"
#include "cc2420_const.h"
#include "net/packetbuf.h"
#include "net/rime/rimestats.h"
#include "net/netstack.h"
#include "contiki-conf.h"
#include "dev/leds.h"
#include "dev/radio.h"
#include "dev/watchdog.h"
#include "lib/random.h"
#include "net/mac/mac-sequence.h"
#include "net/mac/contikimac/contikimac.h"
#include "net/netstack.h"
#include "net/rime/rime.h"
#include "sys/compower.h"
#include "sys/pt.h"
#include "sys/rtimer.h"

struct message{
	long seqno;	
};

long count = 0;

/*---------------------------------------------------------------------------*/
PROCESS(example_broadcast_process, "Broadcast example");
AUTOSTART_PROCESSES(&example_broadcast_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  printf("broadcast message received from %d.%d: '%s'\n",
         from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_broadcast_process, ev, data)
{
  static struct etimer et;
  uint16_t cca; 

  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
  leds_off(LEDS_ALL);
  cc2420_set_txpower(15);
  cc2420_set_channel(26);
  PROCESS_BEGIN();

  broadcast_open(&broadcast, 26, &broadcast_call);

  while(1) {

    /* Delay 2-4 seconds */
    etimer_set(&et, CLOCK_SECOND*4);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
    struct message msg;
    msg.seqno = count++;
    packetbuf_clear();
    packetbuf_copyfrom(&msg, sizeof(struct message));
    //packetbuf_copyfrom("Hello", 6);
    broadcast_send(&broadcast);
    printf("Sending message %ld\n",msg.seqno);
    cca = NETSTACK_RADIO.channel_clear();
    printf("cca:%d\n",cca);
    if(count >= 100){
	 count = 0;
  }
}
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
