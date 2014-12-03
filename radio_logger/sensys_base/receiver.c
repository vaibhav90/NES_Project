#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "contiki.h"
#include "net/rime.h"
#include "dev/leds.h"
#include "dev/cc2420.h"	

struct message{
	long seqno;
	int noise;
	int rssi;
	int lqi;
	int snr;
	int prr;
};

PROCESS(receiver, " base...");
AUTOSTART_PROCESSES(&receiver);


static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
      leds_on(LEDS_GREEN);
	 struct message mesg;
	 memcpy(&mesg, packetbuf_dataptr(), sizeof(mesg)); 	 
	 int r = (cc2420_last_rssi - 45);	 
	 int n = (cc2420_rssi() - 45);
	 int l = (cc2420_last_correlation);
	 int s = r - n;	 
	 if(s<0) s=0;
	 printf("BASE:%ld,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",mesg.seqno,mesg.noise,mesg.rssi,mesg.lqi,mesg.snr,mesg.prr,r,n,l,s);
	 leds_off(LEDS_GREEN); 
}


static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;

PROCESS_THREAD(receiver, ev, data)
{
 PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
 PROCESS_BEGIN();
 
 // Initial configurations  on CC2420 and resetting the timer
 leds_off(LEDS_ALL);
 cc2420_set_txpower(31);
 cc2420_set_channel(26);
 
  broadcast_open(&broadcast, 26, &broadcast_call);
 
  // Ready to receive...
 leds_on(LEDS_BLUE); 
 
 while(1) {
	PROCESS_WAIT_EVENT();		
 }        
 PROCESS_END();
}
