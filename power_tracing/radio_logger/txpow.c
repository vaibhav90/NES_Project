#include "contiki.h"
#include "net/rime/rime.h"
#include "net/packetbuf.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include <stdio.h>
#include <msp430.h> 
#include "settings_cc2420_interferer.h"
#include "dev/cc2420.h"
#include "powertrace.h"
#include <string.h>

#define CHANNEL 26
	
/*---------------------------------------------------------------------------*/
PROCESS(gen_int, "packet stream transmission");
AUTOSTART_PROCESSES(&gen_int);
/*---------------------------------------------------------------------------*/

/*static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  leds_on(LEDS_RED);
  clock_delay(500);
  leds_off(LEDS_RED);  
}
*/
//static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
//static struct broadcast_conn broadcast;

PROCESS_THREAD(gen_int, ev, data)
{
  static struct etimer et;
  
  cc2420_set_txpower(CC2420_TXPOWER_MAX);
  cc2420_set_channel(CHANNEL);
  SPI_SET_MODULATED(0x050C);
  CC2420_SPI_ENABLE();
//  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
  PROCESS_BEGIN();
 // broadcast_open(&broadcast, CHANNEL, &broadcast_call);
  powertrace_start(CLOCK_SECOND * 1);

  while(1) {
  //  packetbuf_clear();
  //  leds_on(LEDS_GREEN);
 //    etimer_set(&et, CLOCK_SECOND/2);
 //    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
     SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (CC2420_TXPOWER_MAX & 0x1f));
 	clock_delay(10500);
 	SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (CC2420_TXPOWER_MIN & 0x1f));
 	clock_delay(8000);
   // clock_delay(10500);
   // packetbuf_copyfrom ("wJM0Vx5KiG6stwnzl6jDFkEGHQHprtgaPAUEi6hQxPTIr89ozxwJM0Vx5KiG6stwnzl6jDFkEGHQHprtgaPAUEi6hQxPTIr89ozx",100);
   // broadcast_send(&broadcast);
    //printf("broadcast message sent\n");
   } 
  CC2420_SPI_DISABLE();
  void powertrace_stop(void);  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
