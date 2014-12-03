#include "contiki.h"
#include "net/rime/rime.h"
#include "net/packetbuf.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include <stdio.h>
#include <stdlib.h>
#include <msp430.h> 
#include "settings_cc2420_interferer.h"
#include "dev/cc2420.h"
#include "powertrace.h"
#include <string.h>

#define PACKETBUF_SIZE 100
#define CHANNEL 26
uint16_t power = 3;
	
/*---------------------------------------------------------------------------*/
PROCESS(gen_int, "packet stream transmission");
AUTOSTART_PROCESSES(&gen_int);
/*---------------------------------------------------------------------------*/

int random(int min, int max)
{
 return min+abs(random_rand()%(max-min+1));
}

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
  
  cc2420_set_channel(CHANNEL);
  
  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();
  
  broadcast_open(&broadcast, CHANNEL, &broadcast_call);
  
  powertrace_start(CLOCK_SECOND/10000);
 while(1) {
    cc2420_set_txpower(power);	
    packetbuf_clear();
    leds_on(LEDS_GREEN);
    etimer_set(&et, CLOCK_SECOND/50);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    packetbuf_copyfrom ("kXq97cRSBQPIz9wMWDgY",20);
    broadcast_send(&broadcast);
    //printf("broadcast message sent\n");
    power+=1;
    if(power>31){
    power=3;
    } 
   } 
    
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
