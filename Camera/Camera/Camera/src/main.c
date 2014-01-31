
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
	status_code_t status;
	uint8_t addr;
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
	//init_twi();
	/* Configure UART for debug message output. */
	configure_console();

	/* Output example information. */
	puts("\n\r\nCamera App\n\r");

	puts("Start Scan...\n\r");
	

	if (STATUS_OK == OV7670_init())
	{
		printf("Initialise success\n\r");
	}
	else
	{
		printf("Initialise fail\n\r");
	}
	
	printf("Get Image!\n\r");
	LoadImageToBuffer();
	while ( STATUS_ERR_BUSY == GetImageIfAvailiable(0));
	printf("Done!\n\r");
	while (1) {
	}
}