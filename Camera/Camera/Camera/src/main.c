
#include <string.h>
#include <asf.h>
#include "conf_board.h"
#include "conf_clock.h"
#include "ov7670.h"

/** Size of the receive buffer used by the PDCA, in bytes. */
#define BUFFER_SIZE         100

/** Max buffer number. */
#define MAX_BUF_NUM         1

/** All interrupt mask. */
#define ALL_INTERRUPT_MASK  0xffffffff

/** Timer counter frequency in Hz. */
#define TC_FREQ             10

#define STRING_EOL    "\r"
#define STRING_HEADER "-- USART Serial Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

#define PDCA_RX_CHANNEL  0
#define PDCA_TX_CHANNEL  1

/** Receive buffer. */
static uint8_t gs_puc_buffer[2][BUFFER_SIZE];

/** Next Receive buffer. */
static uint8_t gs_puc_nextbuffer[2][BUFFER_SIZE];

/** Current bytes in buffer. */
static uint32_t gs_ul_size_buffer = BUFFER_SIZE;

/** Current bytes in next buffer. */
static uint32_t gs_ul_size_nextbuffer = BUFFER_SIZE;

/** Buffer number in use. */
static uint8_t gs_uc_buf_num = 0;

/** Flag of one transfer end. */
static uint8_t g_uc_transend_flag = 0;

/** PDCA channel options. */
pdca_channel_config_t pdca_rx_options = {
	.addr = (void *)gs_puc_buffer, /* memory address */
	.pid = PDCA_PID_USART_RX, /* select peripheral - USART0 RX line.*/
	.size = BUFFER_SIZE, /* transfer counter */
	.r_addr = (void *)gs_puc_nextbuffer, /* next memory address */
	.r_size = BUFFER_SIZE, /* next transfer counter */
	.transfer_size = PDCA_MR_SIZE_BYTE, /* select size of the transfer */
	.etrig = false, /* disable event trigger*/
	.ring = false /* not use ring buffer*/
};
pdca_channel_config_t pdca_tx_options = {
	.addr = (void *)gs_puc_buffer, /* memory address */
	.pid = PDCA_PID_USART_TX, /* select peripheral - USART0 TX line.*/
	.size = 0, /* transfer counter */
	.r_addr = (void *)gs_puc_nextbuffer, /* next memory address */
	.r_size = 0, /* next transfer counter */
	.transfer_size = PDCA_MR_SIZE_BYTE, /* select size of the transfer */
	.etrig = false, /* disable event trigger*/
	.ring = false /* not use ring buffer*/
};


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
 *  Configure UART for debug message output.
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


/**
 * \brief Application entry point for usart_serial example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	status_code_t status, dutycycle1, dutycycle2;
	uint8_t addr, key;
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
	//init_twi();
	/* Configure UART for debug message output. */
	configure_console();
	tc_waveform_initialize();
	/* Output example information. */
	puts("\n\r\nCamera App\n\r");

	
	
	dutycycle1 = 50; //set to the middle
	dutycycle2 = 50;
	servo_move_xaxis(dutycycle1);
	servo_move_yaxis(dutycycle2);
	servo_stop_xaxis();
	servo_stop_yaxis();
	
	if (STATUS_OK == OV7670_init())
	{
		printf("Initialise success\n\r");
	}
	else
	{
		printf("Initialise fail\n\r");
	}
	
	
	while (1) {
		scanf("%c", (char *)&key);

		switch (key) 
		{
			case 'h':
				printf("Help\n\r----\n\nw,a,s,d - move camera\n\rp - take photo\n\r");
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
				break;
			
			case 'p':
				printf("Get Image!\n\r");
				LoadImageToBuffer();
				while ( STATUS_ERR_BUSY == GetImageIfAvailiable(0));
				printf("Done!\n\r");
				break;
		default:
			printf("Key 0x%02x pressed\n\r", key);
			break;
		}
	}
}