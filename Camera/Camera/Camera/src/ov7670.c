/*
 * ov7670.c
 *
 * Created: 31/01/2014 12:06:20
 *  Author: hl13g10
 */ 

#include "ov7670.h"
#include "asf.h"
const char default_settings[SETTINGS_LENGTH][2]=
{
	{OV_TSLB, 0x04},
	{OV_COM15, 0xd0},//RGB565 / RGB555
	{OV_COM7, 0x14},
	{OV_HREF, 0x80},
	{OV_HSTART, 0x16},
	{OV_HSTOP, 0x04},
	{OV_VSTRT, 0x02},
	{OV_VSTOP, 0x7b},//0x7a,
	{OV_VREF, 0x06},//0x0a,
	{OV_COM3, 0x00},
	{OV_COM14, 0x00},//
	{OV_SCALING_XSC, 0x00},
	{OV_SCALING_YSC, 0x00},
	{OV_SCALING_DCWCTR, 0x11},
	{OV_SCALING_PCLK_DIV, 0x00},//
	{0xa2, 0x02},
	{OV_CLKRC, 0x01},
	{OV_GAM1, 0x20},
	{OV_GAM2, 0x1c},
	{OV_GAM3, 0x28},
	{OV_GAM4, 0x3c},
	{OV_GAM5, 0x55},
	{OV_GAM6, 0x68},
	{OV_GAM7, 0x76},
	{OV_GAM8, 0x80},
	{OV_GAM9, 0x88},
	{OV_GAM10, 0x8f},
	{OV_GAM11, 0x96},
	{OV_GAM12, 0xa3},
	{OV_GAM13, 0xaf},
	{OV_GAM14, 0xc4},
	{OV_GAM15, 0xd7},
	{OV_GAM16, 0xe8},
	{OV_COM8, 0xe0},
	{OV_GAIN, 0x00},//AGC
	{OV_AECH, 0x00},
	{OV_COM4, 0x00},
	{OV_COM9, 0x20},//0x38, limit the max gain
	{OV_HIST6, 0x05},
	{OV_HIST12, 0x07},
	{OV_AEW, 0x75},
	{OV_AEB, 0x63},
	{OV_VPT, 0xA5},
	{OV_HIST0, 0x78},
	{OV_HIST1, 0x68},
	{OV_HIST2, 0x03},//0x0b,
	{OV_HIST7, 0xdf},//0xd8,
	{OV_HIST8, 0xdf},//0xd8,
	{OV_HIST9, 0xf0},
	{OV_HIST10, 0x90},
	{OV_HIST11, 0x94},
	{OV_COM8, 0xe5},
	{OV_COM5, 0x61},
	{OV_COM6, 0x4b},
	{0x16, 0x02},
	{OV_MVFP, 0x27},//0x37,
	{0x21, 0x02},
	{0x22, 0x91},
	{0x29, 0x07},
	{0x33, 0x0b},
	{0x35, 0x0b},
	{0x37, 0x1d},
	{0x38, 0x71},
	{OV_OFON, 0x2a},//
	{OV_COM12, 0x78},
	{0x4d, 0x40},
	{0x4e, 0x20},
	{OV_GFIX, 0x0c},///////////////////////
	{OV_DBLV, 0x60},//PLL
	{OV_REG74, 0x19},
	{0x8d, 0x4f},
	{0x8e, 0x00},
	{0x8f, 0x00},
	{0x90, 0x00},
	{0x91, 0x00},
	{OV_DM_LNL, 0x00},//0x19,//0x66
	{0x96, 0x00},
	{0x9a, 0x80},
	{0xb0, 0x84},
	{0xb1, 0x0c},
	{0xb2, 0x0e},
	{OV_THL_ST, 0x82},
	{0xb8, 0x0a},
	{OV_AWBC1, 0x14},
	{OV_AWBC2, 0xf0},
	{OV_AWBC3, 0x34},
	{OV_AWBC4, 0x58},
	{OV_AWBC5, 0x28},
	{OV_AWBC6, 0x3a},
	{0x59, 0x88},
	{0x5a, 0x88},
	{0x5b, 0x44},
	{0x5c, 0x67},
	{0x5d, 0x49},
	{0x5e, 0x0e},
	{OV_LCC3, 0x04},
	{OV_LCC4, 0x20},
	{OV_LCC5, 0x05},
	{OV_LCC6, 0x04},
	{OV_LCC7, 0x08},
	{OV_AWBCTR3, 0x0a},
	{OV_AWBCTR2, 0x55},
	{OV_AWBCTR1, 0x11},
	{OV_AWBCTR0, 0x9f},//0x9e for advance AWB
	{OV_GGAIN, 0x40},
	{OV_BLUE, 0x40},
	{OV_RED, 0x40},
	{OV_COM8, 0xe7},
	{OV_COM10, 0x02},//VSYNC negative
	{OV_MTX1, 0x80},
	{OV_MTX2, 0x80},
	{OV_MTX3, 0x00},
	{OV_MTX4, 0x22},
	{OV_MTX5, 0x5e},
	{OV_MTX6, 0x80},
	{OV_MTXS, 0x9e},
	{OV_COM16, 0x08},
	{OV_EDGE, 0x00},
	{OV_REG75, 0x05},
	{OV_REG76, 0xe1},
	{OV_DNSTH, 0x00},
	{OV_REG77, 0x01},
	{OV_COM13, 0xc2},	//0xc0,
	{OV_REG4B, 0x09},
	{OV_SATCTR, 0x60},
	{OV_COM16, 0x38},
	{OV_CONTRAS, 0x40},
	{0x34, 0x11},
	{OV_COM11, 0x02},//0x00,//0x02,
	{OV_HIST5, 0x89},//0x88,
	{0x96, 0x00},
	{0x97, 0x30},
	{0x98, 0x20},
	{0x99, 0x30},
	{0x9a, 0x84},
	{0x9b, 0x29},
	{0x9c, 0x03},
	{OV_BD50ST, 0x4c},
	{OV_BD60ST, 0x3f},
	{0x78, 0x04},
	{0x79, 0x01},//Some weird thing with reserved registers.
	{0xc8, 0xf0},
	{0x79, 0x0f},
	{0xc8, 0x00},
	{0x79, 0x10},
	{0xc8, 0x7e},
	{0x79, 0x0a},
	{0xc8, 0x80},
	{0x79, 0x0b},
	{0xc8, 0x01},
	{0x79, 0x0c},
	{0xc8, 0x0f},
	{0x79, 0x0d},
	{0xc8, 0x20},
	{0x79, 0x09},
	{0xc8, 0x80},
	{0x79, 0x02},
	{0xc8, 0xc0},
	{0x79, 0x03},
	{0xc8, 0x40},
	{0x79, 0x05},
	{0xc8, 0x30},
	{0x79, 0x26},
	{OV_COM2, 0x03},
	{OV_BRIGHT, 0x00},
	{OV_CONTRAS, 0x40},
	{OV_COM11, 0x42},//0x82,//0xc0,//0xc2,	//night mode
	
};


/**
 * \brief Initialize the TWIM module.
 *
 * \return STATUS_OK   if the initialization succeeds, error code otherwise.
 */
#define PATTERN_TEST_LENGTH (sizeof(write_data)/sizeof(uint8_t))
//! Array to store the test data for sending
uint8_t write_data[] = { 0 };
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
static status_code_t write_twi(unsigned char regID, unsigned char regDat)
{
	/* TWI chip address to communicate with */
	packet_tx.chip = OV7670_ADDR;
	/* TWI address/commands to issue to the other chip (node) */
	packet_tx.addr[0] = regID; //(VIRTUALMEM_ADDR >> 16) & 0xFF;
	packet_tx.addr[1] = (VIRTUALMEM_ADDR >> 8) & 0xFF;
	/* Length of the TWI data address segment (1-3 bytes) */
	packet_tx.addr_length = 1;
	/* Where to find the data to be written */
	write_data[0] = regDat;
	packet_tx.buffer = (void *) write_data;
	/* How many bytes do we want to write */
	packet_tx.length = PATTERN_TEST_LENGTH;
	//printf("Writing data to TARGET\r\n");
	/* Write data to TARGET */
	return twi_master_write(EXAMPLE_TWIM, &packet_tx);
}

static status_code_t wrOV7670Reg(unsigned char regID, unsigned char regDat)
{
	/*	I2C Traffic Generated:
	 *	S | OV_7670 + W | A | RegID | A | Data | A | P |
	 */
		twi_package_t tx;
		uint8_t data[2] = {1,2};
		/* TWI chip address to communicate with */
		tx.chip = 0x21;
		/* TWI address/commands to issue to the other chip (node) */
		tx.addr[0] = regID; 
		/* Length of the TWI data address segment (1-3 bytes) */
		tx.addr_length = 1;
		/* Where to find the data to be written */
		tx.buffer = (void *) &data;
		/* How many bytes do we want to write */
		tx.length = 1;
		//printf("Writing data to TARGET\r\n");
		/* Write data to TARGET */
		printf("Tx Packet:\n\r");
		printf("Addr: %02x \r\n", tx.chip);
		return twi_master_write(EXAMPLE_TWIM, &tx);

}

unsigned char OV7670_init(void)
{
	uint8_t temp;
	status_code_t status;
	printf("Inialising I2C\n\r");
	init_twi();
	printf("Inialising Camera\n\r");

	status = write_twi(OV_COM7,0x80);
	if(STATUS_OK != status)
		return STATUS_ERR_DENIED;
	delay_ms(10);
	
	for(temp = 0; temp < SETTINGS_LENGTH; temp++)
	{
		if( STATUS_OK != write_twi(default_settings[temp][0], default_settings[temp][1] ))
		{
			return STATUS_ERR_DENIED;
		}
		delay_ms(1);
	}
	// 	temp=0x80;
// 	if(STATUS_OK!=wrOV7670Reg(OV_COM7, temp)) //Reset Camera
// 	{
// 		return STATUS_ERR_DENIED;
// 	}
	return STATUS_OK;
}