//Modulated Carrier, Linear Emulation and Periodic Interference Generation 
#include <stdio.h>
#include <math.h>
#include "net/rime/rime.h"
#include "net/packetbuf.h"
#include "powertrace.h"
#include <stdlib.h>
#include <random.h>
#include <string.h>
#include <msp430.h>
#include "contiki.h"
#include "dev/settings_cc2420_interferer.h"
#include "dev/cc2420.h"
#include "dev/watchdog.h"

int random_power(int min, int max)
{
 return min+abs(random_rand()%(max-min+1));
}

// Main Process
PROCESS(interferer_example, "Simple jammer");
AUTOSTART_PROCESSES(&interferer_example);

PROCESS_THREAD(interferer_example, ev, data)
{

 static struct etimer et,et1;
 PROCESS_EXITHANDLER()  
 
 PROCESS_BEGIN();

 // Initial configurations on CC2420: channel and tx power
 watchdog_stop();
 cc2420_set_txpower(31);
 cc2420_set_channel(26);
 printf("interfering with periodic interference\n"); 
 // Continuous Interference
 cc2420_on();
 CC2420_SPI_ENABLE();
 
 //SPI_SET_UNMODULATED(0x1800,0x0100,0x0508,0x0004);
 SPI_SET_MODULATED(0x050C);
 
 powertrace_start(CLOCK_SECOND/10000);
 while(1){	
 etimer_set(&et, CLOCK_SECOND);
 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
 SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (31 & 0x1f));
 etimer_set(&et1, CLOCK_SECOND);
 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et1));
 SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (CC2420_TXPOWER_MIN & 0x1f));
 }
   
 CC2420_SPI_DISABLE();
 PROCESS_WAIT_EVENT();  
 PROCESS_END();
}
 
 
 
 
