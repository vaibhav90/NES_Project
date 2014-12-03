#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <random.h>
#include "contiki.h"
#include "dev/settings_cc2420_interferer.h"
#include "dev/cc2420.h"
#include "dev/leds.h"
#include "dev/watchdog.h"

int random(int min, int max)
{
 return min+abs(random_rand()%(max-min+1));
}

/* ----------------------  Overclocking  ----------------- */
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

// Main Process
PROCESS(interferer_example, "Random Hopper");
AUTOSTART_PROCESSES(&interferer_example);

PROCESS_THREAD(interferer_example, ev, data)
{
 PROCESS_EXITHANDLER();  
 PROCESS_BEGIN();
 static struct etimer et;

 watchdog_stop();
 cc2420_set_txpower(CC2420_TXPOWER_MAX);
 cc2420_set_channel(26);
  
 CC2420_SPI_ENABLE();

 SPI_SET_MODULATED(0x050C);
 boost_cpu(); //
 while(1){	
 cc2420_set_channel(random(11,15));
 CC2420_SPI_ENABLE();
 SPI_SET_MODULATED(0x050C);
 CC2420_SPI_DISABLE();
 etimer_set(&et, CLOCK_SECOND/2);
 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
 }
 restore_cpu();
 CC2420_SPI_DISABLE();
 PROCESS_WAIT_EVENT();  
 PROCESS_END();
}
