/**
 * \file
 *
 * \brief TC Capture Waveform Example for SAM.
 *
 * Copyright (c) 2011 - 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage TC Capture Waveform Example
 *
 * \section Purpose
 *
 * This example indicates how to use TC in capture mode to measure the pulse
 * frequency and count the total pulse number of an external signal injected
 * on TIOA pin.
 *
 * \section Requirements
 *
 * This package can be used with SAM evaluation kits.
 * It generates a waveform on TC TC_CHANNEL_WAVEFORM PIN_TC_WAVEFORM, and it
 * captures wave from TC_CHANNEL_CAPTURE PIN_TC_CAPTURE. Look at the
 * conf_board.h file to check these previous define.
 *
 * To measure the wavefrom on channel_waveform, connect PIN_TC_WAVEFORM to
 * PIN_TC_CAPTURE, and configure PIN_TC_WAVEFORM as output peripheral and
 * PIN_TC_CAPTURE as input peripheral.
 *
 * \section Descriptions
 *
 * This example shows how to configure TC in waveform and capture mode.
 * In capture mode, pulse signal (from PIN_TC_WAVEFORM) is set as an input to
 * PIN_TC_CAPTURE, and RA and RB will be loaded when programmed event occurs.
 * When TC interrupt happens, we could read RA and RB value for calculating
 * pulse frequency and increased pulse number. The current pulse frequency
 * and total pulse number are output on UART.
 *
 * The code can be roughly broken down as follows:
 * <ul>
 * <li>Select pre-defined waveform frequency and duty cycle to be generated.
 * <li>Configure TC_CHANNEL_WAVEFORM as waveform output.
 * <li>Configure TC_CHANNEL_CAPTURE as capture input.
 * <li>Configure capture Register A loaded when rising edge of TIOA occurs.
 * <li>Configure capture Register B loaded when falling edge of TIOA occurs.
 * <li>Configure an interrupt for TC and enable the RB load interrupt.
 * <li> 'c' starts capture.
 * <li> 's' stops capture, and dumps the information that has been captured.
 * </ul>
 *
 * \section Usage
 *
 * -# Compile the application.
 * -# Connect the UART port of the evaluation board to the computer and open
 * it in a terminal.
 *    - Settings: 115200 bauds, 8 bits, 1 stop bit, no parity, no flow control.
 * -# Download the program into the evaluation board and run it.
 * -# Upon startup, the application will output the following line on the UART:
 *    \code
 *     -- TC capture waveform example  xxx --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *    \endcode
 * -# Choose the item in the following menu to test.
 *    \code
 *     Menu :
 *     ------
 *       Output waveform property:
 *       0: Set Frequency =  178 Hz, Duty Cycle = 30%
 *       1: Set Frequency =  375 Hz, Duty Cycle = 50%
 *       2: Set Frequency =  800 Hz, Duty Cycle = 75%
 *       3: Set Frequency = 1000 Hz, Duty Cycle = 80%
 *       4: Set Frequency = 4000 Hz, Duty Cycle = 55%
 *       -------------------------------------------
 *       c: Capture waveform from TC(TC_PERIPHERAL) channel(TC_CHANNEL_CAPTURE)
 *       s: Stop capture and display captured informations
 *       h: Display menu
 *     ------
 *    \endcode
 *
 */

#include "asf.h"
#include "conf_board.h"
#include "conf_clock.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

#define STRING_EOL    "\r"
#define STRING_HEADER "--Servo control --\r\n" \
"-- "BOARD_NAME " --\r\n" \
"-- Compiled: "__DATE__ " "__TIME__ " --"STRING_EOL

//#define TC_CAPTURE_TIMER_SELECTION TC_CMR_TCCLKS_TIMER_CLOCK3

struct waveconfig_t {
	/** Internal clock signals selection. */
	uint32_t ul_intclock;
	/** Waveform frequency (in Hz). */
	uint16_t us_frequency;
	/** Duty cycle in percent (positive).*/
	uint16_t us_dutycycle;
};

#define  SERVO_FREQ		350
#define CLOCK_DIV	2

/** TC waveform configurations */
// static const struct waveconfig_t gc_waveconfig[] = {
// 	{TC_CMR_TCCLKS_TIMER_CLOCK2, SERVO_FREQ,  0},
// 	{TC_CMR_TCCLKS_TIMER_CLOCK2, SERVO_FREQ, 20},
// 	{TC_CMR_TCCLKS_TIMER_CLOCK2, SERVO_FREQ, 30},
// 	{TC_CMR_TCCLKS_TIMER_CLOCK2, SERVO_FREQ, 40},
// 	{TC_CMR_TCCLKS_TIMER_CLOCK2, SERVO_FREQ, 50},
// 	{TC_CMR_TCCLKS_TIMER_CLOCK2, SERVO_FREQ, 60},
// 	{TC_CMR_TCCLKS_TIMER_CLOCK2, SERVO_FREQ, 70},
// 	{TC_CMR_TCCLKS_TIMER_CLOCK2, SERVO_FREQ, 80},
// 	{TC_CMR_TCCLKS_TIMER_CLOCK2, SERVO_FREQ, 90}
// };

#if (SAM4L)
/* The first one is meaningless */
static const uint32_t divisors[5] = { 0, 2, 8, 32, 128};
#else
/* The last one is meaningless */
static const uint32_t divisors[5] = { 2, 8, 32, 128, 0};
#endif

/** Current wave configuration*/
static uint8_t gs_uc_configuration = 0;

/** Number of available wave configurations */
// const uint8_t gc_uc_nbconfig = sizeof(gc_waveconfig)
// 		/ sizeof(struct waveconfig_t);

/** Capture status*/
static uint32_t gs_ul_captured_pulses;
static uint32_t gs_ul_captured_ra;
static uint32_t gs_ul_captured_rb;

/**
 * \brief Display the user menu on the UART.
 */
// static void display_menu(void)
// {
// 	uint8_t i;
// 	puts("\n\rMenu :\n\r"
// 			"------\n\r"
// 			"  Output waveform property:\r");
// 	for (i = 0; i < gc_uc_nbconfig; i++) {
// 		printf("  %d: Set Frequency = %4u Hz, Duty Cycle = %2u%%\n\r", i,
// 				(unsigned int)gc_waveconfig[i].us_frequency,
// 				(unsigned int)gc_waveconfig[i].us_dutycycle);
// 	}
// 	printf("  -------------------------------------------\n\r"
// 			"  c: Capture waveform from TC%d channel %d\n\r"
// 			"  s: Stop capture and display captured informations \n\r"
// 			"  h: Display menu \n\r"
// 			//"------\n\r\r", TC_PERIPHERAL,TC_CHANNEL_CAPTURE
// 			);
// }

/**
 * \brief Configure TC TC_CHANNEL_WAVEFORM in waveform operating mode.
 */
static void tc_waveform_initialize(void)
{
	/* Configure PIO Pins for TC */
	ioport_set_pin_mode(PIN_TC_WAVEFORM, PIN_TC_WAVEFORM_MUX);
	/* Disable IO to enable peripheral mode) */
	ioport_disable_pin(PIN_TC_WAVEFORM);
	/* Configure the PMC to enable the TC module. */
	sysclk_enable_peripheral_clock(ID_TC_WAVEFORM);

	/* Init TC to waveform mode. */
	tc_init(TC, TC_CHANNEL_WAVEFORM,
			/* Waveform Clock Selection */
			TC_CMR_TCCLKS_TIMER_CLOCK2
			| TC_CMR_WAVE /* Waveform mode is enabled */
			| TC_CMR_ACPA_SET /* RA Compare Effect: set */
			| TC_CMR_ACPC_CLEAR /* RC Compare Effect: clear */
			| TC_CMR_CPCTRG /* UP mode with automatic trigger on RC Compare */
	);
	tc_start(TC, TC_CHANNEL_WAVEFORM);
	
	//configure Y Axis
	/* Configure PIO Pins for TC */
	ioport_set_pin_mode(PIN_TC_WAVEFORMYAXIS, PIN_TC_WAVEFORM_MUXYAXIS);
	/* Disable IO to enable peripheral mode) */
	ioport_disable_pin(PIN_TC_WAVEFORMYAXIS);
	/* Configure the PMC to enable the TC module. */
	sysclk_enable_peripheral_clock(ID_TC_WAVEFORM);

	/* Init TC to waveform mode. */
	tc_init(TC, TC_CHANNEL_WAVEFORMYAXIS,
	/* Waveform Clock Selection */
	TC_CMR_TCCLKS_TIMER_CLOCK2
	| TC_CMR_WAVE /* Waveform mode is enabled */
	| TC_CMR_ACPA_SET /* RA Compare Effect: set */
	| TC_CMR_ACPC_CLEAR /* RC Compare Effect: clear */
	| TC_CMR_CPCTRG /* UP mode with automatic trigger on RC Compare */
	);
	tc_start(TC, TC_CHANNEL_WAVEFORMYAXIS);
	
}


void servo_move_xaxis(uint32_t dutycycle)
{
	uint32_t ra, rc;
	/* Configure waveform frequency and duty cycle. */
	rc = (sysclk_get_peripheral_bus_hz(TC) /
			CLOCK_DIV /
			SERVO_FREQ);
	tc_write_rc(TC, TC_CHANNEL_WAVEFORM, rc);
	ra = (100 - dutycycle) * rc / 100;
	tc_write_ra(TC, TC_CHANNEL_WAVEFORM, ra);

	/* Enable TC TC_CHANNEL_WAVEFORM. */
	
	printf("Start waveform: Frequency = %d Hz,Duty Cycle = %2d%%\n\r",
			SERVO_FREQ,
			dutycycle);
}

void servo_move_yaxis(uint32_t dutycycle)
{
	uint32_t ra1, rc1;
	/* Configure waveform frequency and duty cycle. */
	rc1 = (sysclk_get_peripheral_bus_hz(TC) /
	CLOCK_DIV /
	SERVO_FREQ);
	tc_write_rc(TC, TC_CHANNEL_WAVEFORMYAXIS, rc1);
	ra1 = (100 - dutycycle) * rc1 / 100;
	tc_write_ra(TC, TC_CHANNEL_WAVEFORMYAXIS, ra1);

	/* Enable TC TC_CHANNEL_WAVEFORM. */
	
	printf("Start waveform: Frequency = %d Hz,Duty Cycle = %2d%%\n\r",
	SERVO_FREQ,
	dutycycle);
}

static inline void servo_stop_xaxis()
{
	servo_move_xaxis(0);
}

static inline void servo_stop_yaxis()
{
	servo_move_yaxis(0);
}
/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

#define SERVO_STEP	5

/**
 * \brief Application entry point for tc_capture_waveform example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint8_t key;
	uint16_t frequence, dutycycle1,dutycycle2;
   
	/* Initialize the SAM system */
	sysclk_init();
	board_init();

	/* Initialize the console uart */
	configure_console();

	/* Output example information */
	printf("-- 2D Servo control --\r\n");
	printf("-- %s\n\r", BOARD_NAME);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);



	/* Configure TC TC_CHANNEL_WAVEFORM as waveform operating mode */
	printf("Configure TC%d channel %d as waveform operating mode \n\r",
			TC_PERIPHERAL, TC_CHANNEL_WAVEFORM);
	tc_waveform_initialize();

	dutycycle1 = 50; //set to the middle
	dutycycle2 = 50;
	servo_move_xaxis(dutycycle1);
	servo_move_yaxis(dutycycle2);
	servo_stop_xaxis();
	servo_stop_yaxis();
	/* Display menu */
	//display_menu();

	while (1) {
		scanf("%c", (char *)&key);

		switch (key) {
		case 'h':
			//display_menu();
			break;
		case 'a':
			dutycycle1 += SERVO_STEP;
			if(dutycycle1 < 20)
			{
				dutycycle1 = 20;
			}
			else if (dutycycle1 >100)
			{
				dutycycle1 = 100;
			}
			
				servo_move_xaxis(dutycycle1);
				delay_ms(500);
				servo_stop_xaxis();
				//delay_ms(500);
			
			break;
		case 'd':
			dutycycle1 -= SERVO_STEP;
				if(dutycycle1 < 20)
				{
					dutycycle1 = 20;
				}
				else if (dutycycle1 >100)
				{
					dutycycle1 = 100;
				}
				servo_move_xaxis(dutycycle1);
				delay_ms(500);
				servo_stop_xaxis();
				//delay_ms(500);
		
			break;
		case 'w':
			dutycycle2 += SERVO_STEP;
				if(dutycycle2 < 20)
				{
					dutycycle2 = 20;
				}
				else if (dutycycle2 >100)
				{
					dutycycle2 = 100;
				}
				servo_move_yaxis(dutycycle2);
				delay_ms(500);
				servo_stop_yaxis();
				//delay_ms(500);
			
			break;
		case 's':
				dutycycle2 -= SERVO_STEP;
					if(dutycycle2 < 20)
					{
						dutycycle2 = 20;
					}
					else if (dutycycle2 >100)
					{
						dutycycle2 = 100;
					}
					servo_move_yaxis(dutycycle2);
					delay_ms(500);
					servo_stop_yaxis();
					//delay_ms(500);
			
			break;
// 		default:
// 			/* Set waveform configuration #n */
// 			if ((key >= '0') && (key <= ('0' + gc_uc_nbconfig - 1))) {
// 				if (!gs_ul_captured_pulses) {
// 					gs_uc_configuration = key - '0';
// 					servo_move_xaxis(dutycycle1);
// 					servo_move_yaxis(dutycycle2);
// 					delay_ms(500);
// 					servo_stop_xaxis();
// 					servo_stop_yaxis();
// 				} else {
// 					printf("Capturing ... , press 's' to stop capture first \r");
// 				}
// 			}
			//else printf("Key 0x%02x pressed\n\r", key);
//
			//break;
		}
	}
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
