#include <stdio.h>
#include "contiki.h"
#include "dev/cc2420.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "settings_cc2420_interferer.h"


// Global variables
uint8_t jammer_status = 0; 		// 0 = quiet, 1 = jamming
uint16_t channel = 11; 


// Main Process
PROCESS(interferer_example, "Simple jammer");
AUTOSTART_PROCESSES(&interferer_example);
PROCESS_THREAD(interferer_example, ev, data)
{
 PROCESS_EXITHANDLER()  
 PROCESS_BEGIN();

 // Initial configurations on CC2420: channel and tx power
 SENSORS_ACTIVATE(button_sensor);
 cc2420_set_txpower(CC2420_TXPOWER_MAX);
 cc2420_set_channel(channel);
 printf("activating/deactivating interference when user button is pressed\n"); 
 
 // Waiting for button events
 while(1){
	PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
	jammer_status = !jammer_status; //button is active low
		
	
	if(jammer_status){
		channel++;
		if(channel>26){
		channel=11;}
		cc2420_set_channel(channel);	
		leds_off(LEDS_GREEN);
		leds_on(LEDS_RED);
		CC2420_SPI_ENABLE();
		SPI_SET_MODULATED(0x050C);
		CC2420_SPI_DISABLE();
	}
	else{
		leds_off(LEDS_RED);
		leds_on(LEDS_GREEN);
		cc2420_set_channel(channel);		
		CC2420_SPI_ENABLE();
		SPI_SET_MODULATED(0x0500);
		CC2420_SPI_DISABLE();
	}
 }
 PROCESS_END();
}
