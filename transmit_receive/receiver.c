#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include "contiki.h"
#include "net/rime.h"
#include "dev/leds.h"
#include "dev/cc2420.h"	

#define PACKETS_PER_SECOND 10
#define PACKETS_TIME (CLOCK_SECOND/PACKETS_PER_SECOND)
#define UPDATES_PER_SECOND 2
#define UPDATE_TIME (CLOCK_SECOND/UPDATES_PER_SECOND)

struct message{
	long seqno;
	int noise;
	int rssi;
	int lqi;
	int snr;
	int prr;
};

// Arrays to store the info
int noise[PACKETS_PER_SECOND/UPDATES_PER_SECOND] = {0};
int rssi[PACKETS_PER_SECOND/UPDATES_PER_SECOND] = {0};
int lqi[PACKETS_PER_SECOND/UPDATES_PER_SECOND] = {0};
int snr[PACKETS_PER_SECOND/UPDATES_PER_SECOND] = {0};

int received = 0;
long count = 0;

PROCESS(receiver, " base...");
AUTOSTART_PROCESSES(&receiver);


static void broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
      leds_on(LEDS_GREEN);
	 struct message mesg;
	 memcpy(&mesg, packetbuf_dataptr(), sizeof(mesg));
	 //rssi[mesg.seqno] = (cc2420_last_rssi - 45);	 
	 //noise[mesg.seqno] = (cc2420_rssi() - 45);
	 //lqi[mesg.seqno] = cc2420_last_correlation;
	 //snr[mesg.seqno] = rssi[mesg.seqno] - noise[mesg.seqno];	 
	 //if(snr[mesg.seqno]<0) snr[mesg.seqno]=0;
	 //received++;
	 //printf("R: %d, N: %d, L: %d, S: %d, seqno:%ld, received=%d\n",rssi[mesg.seqno],noise[mesg.seqno],lqi[mesg.seqno],snr[mesg.seqno],mesg.seqno,received);
	 printf("R: %d\n",mesg.seqno);
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
