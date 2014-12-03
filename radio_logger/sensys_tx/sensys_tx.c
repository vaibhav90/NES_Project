#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "contiki.h"
#include "net/rime/rime.h"
#include "dev/leds.h"
#include "dev/cc2420.h"

#define POWER 31
#define CHANNEL_BASE 22
#define CHANNEL_SENDER 26
#define RIME_SENDER 122
#define RIME_BASE 123

#define SENDER 104
#define RECEIVER 102
#define BASE 110

#define PACKETS_PER_SECOND 32
#define PACKETS_TIME (CLOCK_SECOND/PACKETS_PER_SECOND)

struct message{
	long seqno;	
};

// Timer
static struct ctimer timer1;
static void ctimer1_callback(void *ptr);

// Message counters
long count = 0;
int total_to_be_sent = 0;

// Unicast
static struct unicast_conn uc;


PROCESS(sensys_tx, "SenSys sender (TX)...");
AUTOSTART_PROCESSES(&sensys_tx);

// Timer timeout callback
static void ctimer1_callback(void *ptr)
{ 	
  leds_on(LEDS_GREEN);
  struct message msg;
  msg.seqno = count++;	
  packetbuf_clear();
  packetbuf_copyfrom(&msg, sizeof(struct message));
  linkaddr_t addr;
  addr.u8[0] = RECEIVER;
  addr.u8[1] = 0;
  unicast_send(&uc, &addr);	
  printf("Sending message %ld to %d.0\n",msg.seqno,RECEIVER);
  if(count >= total_to_be_sent){
	 count = 0;
  }
  else{
	ctimer_set(&timer1, PACKETS_TIME, ctimer1_callback, NULL);
  }
  leds_off(LEDS_GREEN);
}

// Handlers for unicast
static void recv_uc(struct unicast_conn *c, const linkaddr_t *from)
{ 
 if((from->u8[0] == RECEIVER) && (from->u8[1] == 0)){	 
 	 // Receive message
	 leds_on(LEDS_GREEN);
	 struct message mesg;
	 memcpy(&mesg, packetbuf_dataptr(), sizeof(mesg));
	 total_to_be_sent = mesg.seqno;
	 printf("Total to be sent: %d\n",total_to_be_sent);
	 leds_off(LEDS_GREEN); 	 
	 // Starting sending
	 //clock_delay(353*50);
	 ctimer_set(&timer1, 2*PACKETS_TIME, ctimer1_callback, NULL);
 }
 else{
	printf("ERROR: received an unexpected unicast from host (%d:%d)!\n",from->u8[0],from->u8[1]);  
 }
}
static const struct unicast_callbacks unicast_callbacks = {recv_uc};


PROCESS_THREAD(sensys_tx, ev, data)
{
 PROCESS_EXITHANDLER()
 PROCESS_BEGIN();
 
 // Initial configurations  on CC2420 and resetting the timer
 leds_off(LEDS_ALL);
 cc2420_set_txpower(POWER);
 cc2420_set_channel(CHANNEL_SENDER);
 unicast_open(&uc, RIME_SENDER, &unicast_callbacks); 
 ctimer_stop(&timer1);
  
 // Ready to send...
 leds_on(LEDS_BLUE); 
 
 while(1) {
	PROCESS_WAIT_EVENT();		
 }        
 PROCESS_END();
}
