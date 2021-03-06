//Incremental Interference
#include <stdio.h>
#include <stdlib.h>
#include "contiki.h"
#include "powertrace.h"
#include "settings_cc2420_interferer.h"
#include "dev/cc2420.h"
#include "dev/watchdog.h"

#define SWITCHING_TIME (CLOCK_SECOND*1)	// Time after which increasing the power

int current_power = CC2420_TXPOWER_MAX;


// Main Process
PROCESS(interferer_incremental, "Simple jammer");
AUTOSTART_PROCESSES(&interferer_incremental);
PROCESS_THREAD(interferer_incremental, ev, data)
{
 PROCESS_EXITHANDLER()  
 PROCESS_BEGIN();
 
 // Initial configurations on CC2420: channel and tx power
 cc2420_set_channel(26);
 watchdog_stop();
 printf("interfering with incremental power\n");
  
 static struct etimer et; 
 powertrace_start(CLOCK_SECOND/10000);
 while(1) {		
	 etimer_set(&et, SWITCHING_TIME);
	 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	 
	 current_power++;
	 //printf("Power %d\n",current_power);
	 
	 // Interferer logic (incremental transmission power and turning off)
	 if(current_power == (CC2420_TXPOWER_MAX + 1)){
		CC2420_SPI_ENABLE();
		SPI_SET_MODULATED(0x050C);
		CC2420_SPI_DISABLE();
		current_power = CC2420_TXPOWER_MIN-1;
	 }
	 else if(current_power == CC2420_TXPOWER_MIN){
		CC2420_SPI_ENABLE();
		SPI_SET_MODULATED(0x050C);
		SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (CC2420_TXPOWER_MIN & 0x1f));
		CC2420_SPI_DISABLE();	
	 }	 
	 else if((current_power > CC2420_TXPOWER_MIN) && (current_power <= CC2420_TXPOWER_MAX)){
		CC2420_SPI_ENABLE();
		SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (current_power & 0x1f));
		CC2420_SPI_DISABLE();	
	 } 
 }     
 
 PROCESS_WAIT_EVENT(); 
 PROCESS_END();
}
