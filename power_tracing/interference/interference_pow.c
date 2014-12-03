//Modulated Carrier, Linear Emulation and Periodic Interference Generation 
#include "contiki.h"
#include "powertrace.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "net/rime/rime.h"
#include "dev/settings_cc2420_interferer.h"
#include "dev/cc2420.h"

PROCESS(interferer_example, "Simple jammer");
AUTOSTART_PROCESSES(&interferer_example);

PROCESS_THREAD(interferer_example, ev, data)
{
 PROCESS_EXITHANDLER()  
 PROCESS_BEGIN();
 cc2420_set_txpower(CC2420_TXPOWER_MAX);
 cc2420_set_channel(26);
 SPI_SET_MODULATED(0x050C);
 CC2420_SPI_ENABLE();
 powertrace_start(CLOCK_SECOND * 1);
 printf("going to loop now");
   
 	while(1){	

 		SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (CC2420_TXPOWER_MAX & 0x1f));
 		clock_delay(10500);
 		SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (CC2420_TXPOWER_MIN & 0x1f));
 		clock_delay(8000);
	   }
   
 CC2420_SPI_DISABLE();
 PROCESS_WAIT_EVENT();  
 void powertrace_stop(void); 
 PROCESS_END();
}


