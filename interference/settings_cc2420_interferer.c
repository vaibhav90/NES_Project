#include "settings_cc2420_interferer.h"

/*---------------------------------------------------------------------------*/

// Reset the interferer back to normal mode (setting back the registers to their original value)
void reset_jammer(uint8_t carrier){
	if(carrier){
		SPI_SET_MODULATED(0x0500);
	}
	else{
		SPI_SET_UNMODULATED(0x0000,0x0000,0x0500,0x0010);
	}
	ENERGEST_OFF(ENERGEST_TYPE_TRANSMIT);
	ENERGEST_ON(ENERGEST_TYPE_LISTEN);  
}

/*---------------------------------------------------------------------------*/

// Starting the interferer (0 = unmodulated carrier, !0 = modulated carrier)
void set_jammer(uint8_t carrier){
	if(carrier){
		/* 
		 * The CC2420 has a built-in test pattern generator that can generate pseudo random sequence using the CRC generator. 
		 * This is enabled by setting MDMCTRL1.TX_MODE to 3 and issue a STXON command strobe. The modulated spectrum is then available on the RF pins. 
		 * The low byte of the CRC word is transmitted and the CRC is updated with 0xFF for each new byte. 
		 * The length of the transmitted data sequence is 65535 bits. The transmitted data-sequence is then: [synch header] [0x00, 0x78, 0xb8, 0x4b, 0x99, 0xc3, 0xe9, …]	
		*/
		SPI_SET_MODULATED(0x050C);
	}
	else{
		/* 
		 * An unmodulated carrier may be transmitted by setting MDMCTRL1.TX_MODE to 2, writing 0x1800 to the DACTST register and issue a STXON command strobe.
		 * The transmitter is then enabled while the transmitter I/Q DACs are overridden to static values. 
		 * An unmodulated carrier will then be available on the RF output pins.
		*/	
		SPI_SET_UNMODULATED(0x1800,0x0100,0x0508,0x0004);
	}
	ENERGEST_OFF(ENERGEST_TYPE_LISTEN);
	ENERGEST_ON(ENERGEST_TYPE_TRANSMIT);  
}

/*---------------------------------------------------------------------------*/

// Setting the transmission power to pow
void power_jammer(uint8_t pow){
	// 0xa0ff is the initial value of the CC2420_TXCTRL register measured by me
	SPI_SET_TXPOWER((0xa0ff & 0xffe0) | (pow & 0x1f));
}

/*---------------------------------------------------------------------------*/
