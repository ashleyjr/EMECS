
#include <string.h>
#include <asf.h>
#include "conf_board.h"
#include "conf_clock.h"

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
 * \brief Initialize the TWIM module.
 *
 * \return STATUS_OK   if the initialization succeeds, error code otherwise.
 */
#define PATTERN_TEST_LENGTH (sizeof(write_data)/sizeof(uint8_t))
//! Array to store the test data for sending
const uint8_t write_data[] = {
	'S', 'A', 'M', '4', 'L', ' ', 'T', 'W', 'I', 'M', ' ', 'M',
	'a', 's', 't', 'e', 'r', ' ', 'E', 'x', 'a', 'm', 'p', 'l', 'e'
};
//! Array to store the received test data
uint8_t read_data[PATTERN_TEST_LENGTH];
twi_package_t packet_tx, packet_rx;
uint32_t cpu_speed = 0;

static status_code_t init_twi(void)
{
	/* Set TWIM options */
	cpu_speed = sysclk_get_peripheral_bus_hz(EXAMPLE_TWIM);
	struct twim_config opts = {
		.twim_clk = cpu_speed,
		.speed = TWIM_MASTER_SPEED,
		.hsmode_speed = 0,
		.data_setup_cycles = 0,
		.hsmode_data_setup_cycles = 0,
		.smbus = false,
		.clock_slew_limit = 0,
		.clock_drive_strength_low = 0,
		.data_slew_limit = 0,
		.data_drive_strength_low = 0,
		.hs_clock_slew_limit = 0,
		.hs_clock_drive_strength_high = 0,
		.hs_clock_drive_strength_low = 0,
		.hs_data_slew_limit = 0,
		.hs_data_drive_strength_low = 0,
	};
	/* Initialize the TWIM Module */
	twim_set_callback(EXAMPLE_TWIM, 0, twim_default_callback, 1);

	return twim_set_config(EXAMPLE_TWIM, &opts);
}

/**
 * \brief Write the data pattern to the target.
 *
 * \return STATUS_OK   if all bytes were written, error code otherwise.
 */
static status_code_t write_test(void)
{
	/* TWI chip address to communicate with */
	packet_tx.chip = TARGET_ADDRESS;
	/* TWI address/commands to issue to the other chip (node) */
	packet_tx.addr[0] = (VIRTUALMEM_ADDR >> 16) & 0xFF;
	packet_tx.addr[1] = (VIRTUALMEM_ADDR >> 8) & 0xFF;
	/* Length of the TWI data address segment (1-3 bytes) */
	packet_tx.addr_length = TARGET_ADDR_LGT;
	/* Where to find the data to be written */
	packet_tx.buffer = (void *) write_data;
	/* How many bytes do we want to write */
	packet_tx.length = PATTERN_TEST_LENGTH;
	printf("Writing data to TARGET\r\n");
	/* Write data to TARGET */
	return twi_master_write(EXAMPLE_TWIM, &packet_tx);
}


/**
 * \brief Application entry point for usart_serial example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	status_code_t status;
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
	init_twi();
	/* Configure UART for debug message output. */
	configure_console();

	/* Output example information. */
	puts("Camera App\n\r");

	puts("Start Scan...");

	/* Perform Write Test */
	status = write_test();
	/* Check status of transfer */
	if (status == STATUS_OK) {
		printf("WRITE TEST:\tPASS\r\n");
		} else {
		printf("WRITE TEST:\tFAILED\r\n");
		while (1) {
			sleepmgr_enter_sleep();
		}
	}
	
	while (1) {
	}
}