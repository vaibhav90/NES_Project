#include "contiki.h"
#include "net/rime.h"
#include "net/packetbuf.h"
#include "net/netstack.h"
#include "dev/spi.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "powertrace.h"
#include <stdio.h>
#include <msp430.h> 
#include "dev/cc2420.h"
#include "dev/cc2420_const.h"
#include "settings_cc2420_interferer.h"
#include "dev/cc2420.h"
#include <string.h>

#define PACKETBUF_SIZE 100
#define CHANNEL 26

uint16_t power=3, i=0;
	
/*---------------------------------------------------------------------------*/
PROCESS(gen_int, "packet stream transmission");
AUTOSTART_PROCESSES(&gen_int);
/*---------------------------------------------------------------------------*/

static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  leds_on(LEDS_RED);
  clock_delay(500);
  leds_off(LEDS_RED);  
}

static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;

PROCESS_THREAD(gen_int, ev, data)
{
  static struct etimer et;
   /* switch mac layer off, and turn radio on */
  //NETSTACK_MAC.off(0);
  //cc2420_on();
  
  cc2420_set_txpower(power);
  cc2420_set_channel(CHANNEL);
  
  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();
  
  broadcast_open(&broadcast, CHANNEL, &broadcast_call);
  powertrace_start(CLOCK_SECOND*2);
  
  while(1) {
  
    for(power=0; power<32; power+=1){
    cc2420_set_txpower(power);
    packetbuf_clear();
    etimer_set(&et, CLOCK_SECOND/1000);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    packetbuf_copyfrom ("wJM0Vx5KiG6stwnzl6jDFkEGHQHprtgaPAUEi6hQxPTIr89ozxwJM0Vx5KiG6stwnzl6jDFkEGHQHprtgaPAUEi6hQxPTIr89ozx",100);
    broadcast_send(&broadcast);
    }
    
    for(power=30; power>0; power-=1){
    cc2420_set_txpower(power);
    packetbuf_clear();
    etimer_set(&et, CLOCK_SECOND/1000);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    packetbuf_copyfrom ("wJM0Vx5KiG6stwnzl6jDFkEGHQHprtgaPAUEi6hQxPTIr89ozxwJM0Vx5KiG6stwnzl6jDFkEGHQHprtgaPAUEi6hQxPTIr89ozx",100);
    broadcast_send(&broadcast);
    }
    
    for(i=0; i<15; i++){
    etimer_set(&et, CLOCK_SECOND/100);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
    }
       
    
   } 
    
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
