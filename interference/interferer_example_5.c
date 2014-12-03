#include <stdio.h>
#include "contiki.h"
#include "settings_cc2420_interferer.h"
#include "dev/cc2420.h"
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
 cc2420_set_txpower(CC2420_TXPOWER_MAX);  
 printf("interfering with a specific shape signal\n"); 
 
 // Starting the carrier
 CC2420_SPI_ENABLE();
 SPI_SET_MODULATED(0x050C);
 CC2420_SPI_DISABLE();
 
 while(1) {		 
	interfere();	
	clock_delay(60000);
	stop();
	clock_delay(60000);
	interfere();	
	clock_delay(20000);
	stop();
	clock_delay(60000);	
	interfere();	
	clock_delay(60000);
	stop();
	clock_delay(20000);
	clock_delay(60000);
 }     
 
 PROCESS_WAIT_EVENT(); 
 PROCESS_END();
}
