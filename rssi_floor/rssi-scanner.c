#include "contiki.h"
#include "net/rime/rime.h"
#include "net/netstack.h"

#include "dev/leds.h"
#include "cc2420.h"
#include "cc2420_const.h"
#include "dev/spi.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/* This assumes that the CC2420 is always on and "stable" */
static void
set_frq(int c)
{
  int f;
  /* fine graied channel - can we even read other channels with CC2420 ? */
  f = c + 302 + 0x4000;

  CC2420_WRITE_REG(CC2420_FSCTRL, f);
  CC2420_STROBE(CC2420_SRXON);
}

static void
do_rssi(void)
{
  int channel;
  printf("RSSI:");
  for(channel = 0; channel <= 79; ++channel) {
    set_frq(channel + 55);
    printf("%d ", cc2420_rssi() + 55);
  }
  printf("\n");
}

/*---------------------------------------------------------------------------*/
PROCESS(scanner_process, "RSSI Scanner");
AUTOSTART_PROCESSES(&scanner_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(scanner_process, ev, data)
{
  PROCESS_BEGIN();
  /* switch mac layer off, and turn radio on */
  NETSTACK_MAC.off(0);
  cc2420_on();

  while(1) {
    do_rssi();
    PROCESS_PAUSE();
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
