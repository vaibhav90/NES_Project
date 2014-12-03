#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <random.h>
#include "contiki.h"
#include "dev/settings_cc2420_interferer.h"
#include "dev/cc2420.h"
#include "dev/watchdog.h"


// Variables definition
#define INTERFERED_CHANNEL 24	// Interfered channel
#define CARRIER_TYPE 1			// Carrier type: 0 = unmodulated, !0 = modulated
#define EMULATION_TYPE 0		// 1 = random, 0 = linear


// More randomness in the output power
int random_power(int min, int max)
{
 return min+abs(random_rand()%(max-min+1));
}

// Simple periodic pattern (like the one from microwave ovens)
void periodic_jammer(int period_on, int period_off)
{
 CC2420_SPI_ENABLE();
 set_jammer(CARRIER_TYPE);
 while(1){	
	// ON cycle
	#if EMULATION_TYPE	
		power_jammer(random_power(0,31));
	#else
		power_jammer(CC2420_TXPOWER_MAX);
	#endif
	clock_delay(period_on);
	// OFF cycle
	power_jammer(CC2420_TXPOWER_MIN);
	clock_delay(period_off);
 }
 CC2420_SPI_DISABLE();
}


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
 cc2420_set_channel(INTERFERED_CHANNEL);
 printf("HandyMote: interfering with periodic interference\n"); 
 
 // Interfering with a continuous pattern 
 periodic_jammer(9943, 10246); 

 PROCESS_WAIT_EVENT();  
 PROCESS_END();
}
