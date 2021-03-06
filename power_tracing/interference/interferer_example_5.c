#include <stdio.h>
#include "contiki.h"
#include "settings_cc2420_interferer.h"
#include "dev/cc2420.h"
#include "powertrace.h"
#include "dev/watchdog.h"

void interfere() {
 CC2420_SPI_ENABLE();
 SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (CC2420_TXPOWER_MAX & 0x1f));
 CC2420_SPI_DISABLE();
}

void stop() {
 CC2420_SPI_ENABLE();
 SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (CC2420_TXPOWER_MIN & 0x1f));
 CC2420_SPI_DISABLE();
}


// Main Process
PROCESS(interferer_fixedpattern, "Simple jammer");
AUTOSTART_PROCESSES(&interferer_fixedpattern);
PROCESS_THREAD(interferer_fixedpattern, ev, data)
{
 PROCESS_EXITHANDLER()  
 PROCESS_BEGIN();

 // Initial configurations on CC2420: channel and tx power
 watchdog_stop();
 cc2420_set_channel(26); 
 static struct etimer et;  
 cc2420_set_txpower(CC2420_TXPOWER_MAX);  
 printf("interfering with a specific shape signal\n"); 
 
 // Starting the carrier
 CC2420_SPI_ENABLE();
 SPI_SET_MODULATED(0x050C);
 CC2420_SPI_DISABLE();
 powertrace_start(CLOCK_SECOND/10000);
 while(1) {		 
	interfere();	
	etimer_set(&et, CLOCK_SECOND*1.5);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	stop();
	etimer_set(&et, CLOCK_SECOND*1.5);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	interfere();	
	etimer_set(&et, CLOCK_SECOND*0.75);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	stop();
	etimer_set(&et, CLOCK_SECOND*1.5);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));	
	interfere();	
	etimer_set(&et, CLOCK_SECOND*1.5);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	stop();
	etimer_set(&et, CLOCK_SECOND*0.75);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	//clock_delay(60000);
 }     
 
 PROCESS_WAIT_EVENT(); 
 PROCESS_END();
}
