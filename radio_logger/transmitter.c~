#include "contiki.h"
#include "net/rime.h"
#include "net/packetbuf.h"
#include "dev/spi.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
//#include "powertrace.h"
#include <stdio.h>
#include <msp430.h> 
#include "dev/cc2420.h"
#include "dev/cc2420_const.h"
#include "settings_cc2420_interferer.h"
#include "dev/cc2420.h"
#include <string.h>

#define CHANNEL 26
struct message{
	long seqno;	
};

long count = 0;
	
/*---------------------------------------------------------------------------*/
PROCESS(gen_int, "packet stream transmission");
AUTOSTART_PROCESSES(&gen_int);
/*---------------------------------------------------------------------------*/

static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  _NOP();  
}

static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;

PROCESS_THREAD(gen_int, ev, data)
{
  static struct etimer et;
  
  cc2420_set_txpower(31);
  cc2420_set_channel(CHANNEL);
  
  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();
  
  broadcast_open(&broadcast, CHANNEL, &broadcast_call);
  //powertrace_start(CLOCK_SECOND*2);
  
  while(1) {
    leds_on(LEDS_GREEN);
    struct message msg;
    msg.seqno = count++;	
    packetbuf_clear();
    packetbuf_copyfrom(&msg, sizeof(struct message));
    //rimeaddr_t addr;
    //addr.u8[0] = 2;
    //addr.u8[1] = 0;
    //unicast_send(&uc, &addr);	
    //printf("Sending message %ld\n",msg.seqno);
    //if(!linkaddr_cmp(&addr, &linkaddr_node_addr)) {
      etimer_set(&et, CLOCK_SECOND/1000);
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
      broadcast_send(&broadcast);
    //}
    if(count >= 1000){
	 count = 0;
    }
   } 
    
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
