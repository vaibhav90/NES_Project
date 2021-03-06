#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <random.h>
#include "contiki.h"
#include "powertrace.h"
#include "settings_cc2420_interferer.h"
#include "dev/cc2420.h"
#include "dev/watchdog.h"


/* ----------------------  CPU BOOSTING  ----------------- */
uint16_t cpu1, cpu2;

void boost_cpu() // Temporarily boost CPU speed
{
 cpu1 = DCOCTL;
 cpu2 = BCSCTL1;
 DCOCTL = 0xff;
 BCSCTL1 |= 0x07;
}	

void restore_cpu() // Restore CPU speed
{
 DCOCTL = cpu1;
 BCSCTL1 = cpu2;
}
/* ------------------------------------------------------- */


// Main Process
PROCESS(interferer_example, "Simple jammer");
AUTOSTART_PROCESSES(&interferer_example);
PROCESS_THREAD(interferer_example, ev, data)
{
 PROCESS_EXITHANDLER()  
 PROCESS_BEGIN();

 // Initial configurations on CC2420: channel and tx power
 watchdog_stop();
 cc2420_set_txpower(CC2420_TXPOWER_MAX);
 cc2420_set_channel(26);
 printf("interfering with accurate timing\n"); 
 static struct etimer et; 
 
 // Boosting CPU
 boost_cpu();
 
 CC2420_SPI_ENABLE();
 SPI_SET_MODULATED(0x050C);
 powertrace_start(CLOCK_SECOND*4);
 volatile int a = 0;
 while(1){	
		SPI_SET_TXPOWER((0xa0ff & 0xffe0) | ((a*CC2420_TXPOWER_MAX) & 0x1f));
		etimer_set(&et, CLOCK_SECOND);
	     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		if(a){		
		}
		else{
			_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
			_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
			_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
			_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
			_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
			_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
			_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();
			_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();			
			
		}
		a = !a;
 }
 CC2420_SPI_DISABLE(); 
 
 // Recovering CPU
 restore_cpu();
 
 PROCESS_WAIT_EVENT();     
 PROCESS_END();
}
