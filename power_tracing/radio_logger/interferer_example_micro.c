//Modulated Carrier, Linear Emulation and Periodic Interference Generation 
#include <stdio.h>
#include <math.h>
#include "net/rime/rime.h"
#include "net/packetbuf.h"
#include <stdlib.h>
#include "random.h"
#include <string.h>
#include <msp430.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "dev/settings_cc2420_interferer.h"
#include "dev/cc2420.h"
#include "dev/watchdog.h"
#include "powertrace.h"

uint16_t power = 3;

// Main Process
PROCESS(interferer_example, "Simple jammer");
AUTOSTART_PROCESSES(&interferer_example);

PROCESS_THREAD(interferer_example, ev, data)
{
 PROCESS_EXITHANDLER()  
 PROCESS_BEGIN();
 
 static struct etimer et;
 //powertrace_start(CLOCK_SECOND * 2);

 // Initial configurations on CC2420: channel and tx power
 watchdog_stop();
 cc2420_set_txpower(power);
 cc2420_set_channel(26);
 //printf("interfering with periodic interference\n"); 
 // Continuous Interference
 
 CC2420_SPI_ENABLE();
 
 //SPI_SET_UNMODULATED(0x1800,0x0100,0x0508,0x0004);
 SPI_SET_MODULATED(0x050C);
 
 powertrace_start(CLOCK_SECOND/10000);

 while(1){	
 
 for(power=3; power<32; power+=4){
 SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (power & 0x1f));
 etimer_set(&et, CLOCK_SECOND/1000);
 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
 //printf ("transmit power = %d\n",power);
 }
 
 for(power=27; power>3; power-=4){
 SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (power & 0x1f));
 etimer_set(&et, CLOCK_SECOND/1000);
 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
 //printf ("transmit power = %d\n",power);
 }
 
 for(power=3; power<32; power+=2){
 SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (0 & 0x1f));
 etimer_set(&et, CLOCK_SECOND/1000);
 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
 //printf ("transmit power = %d\n",power);
 }
 
} 
 CC2420_SPI_DISABLE();
 void powertrace_stop(void); 
 PROCESS_WAIT_EVENT();  
 PROCESS_END();
}
 
 
 
 
