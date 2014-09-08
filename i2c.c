/**
 *  Driver developed in HERMES project
 */
/**
 *  @addtogroup  I2C
 *  @brief       My I2C lib.
 *  @{
 *      @file       i2c.c
 *      @brief      An I2C lib for i2c operations.
 *      @details    Change History:
 *                  ************************************************************************
 *                  | VERSION      | DATE	    | AUTHORS	  | DESCRIPTION            |
 *                  |:------------:|:--------------:|:------------|:-----------------------|
 *                  | 1.0          |   25/8/2014    | MR          | First Release          |
 *                  \n\n
 */
#include "i2c.h"
#include "hardware.h"
#include <peripheral/i2c.h>

/**
 *  @brief  Initialize i2c module:
 *  I2C clock is BRG
 *  If mode parameter is SLAVE, uses address to set slave address for the module
 *  Enable module
 *  @param[in]  I2C_MODULE i2cnum (use I2C1 otherwise is necessary to modify the functions)
 *  @param[in]  i2cmode mode (MASTER or SLAVE)
 *  @param[in]  BYTE address for SLAVE mode
 *  @return     none
 */
void i2c_init(I2C_MODULE i2cnum, i2cmode mode, BYTE address) {
	//enabling i2c module doesnt need changing port
	//direction/value etc, and is not a pin muxed peripheral
	I2CConfigure ( i2cnum, I2C_ENABLE_SLAVE_CLOCK_STRETCHING);
	I2CSetFrequency ( i2cnum, GetPeripheralClock(), BRG);

	if(mode == SLAVE)
	{
		//address mask is set to 0
		I2CSetSlaveAddress ( i2cnum, address&0x7f, 0, I2C_USE_7BIT_ADDRESS );
	}

	I2CEnable(i2cnum, TRUE);
}

/**
 *  @brief  Write to device using generic i2c protocol
 *  @param[in]  slave_addr - slave address
 *  @param[in]  reg_addr   - register address
 *  @param[in]  length     - number of bytes to write
 *  @param[in]  *data      - pointer for data to write
 *  @return     0 if sucessfull, 1 otherwise
 */
int i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data) {

	BYTE i;

	StartI2C1();								//Send the Start Bit
	IdleI2C1();									//Wait to complete
        if (MasterWriteI2C1(((slave_addr<<1)|(0x00))))
            return 1;
	IdleI2C1();
        if (MasterWriteI2C1(reg_addr))
            return 1;
	IdleI2C1();

	for(i=0;i<length;i++){
            if (MasterWriteI2C1(data[i]))
                return 1;
	}
        StopI2C1();								//Send the Stop condition
        IdleI2C1();								//Wait to complete

	return 0;
}

/**
 *  @brief  Write to device using generic i2c protocol
 *  @param[in]  slave_addr - slave address
 *  @param[in]  reg_addr   - register address
 *  @param[in]  length     - number of bytes to read
 *  @param[in]  *data      - pointer to where register data is to be transfered
 *  @return     0 if sucessfull, 1 otherwise
 */
int i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data) {

	BYTE i=2;

	StartI2C1();								//Send the Start Bit
	IdleI2C1();
        if (MasterWriteI2C1(((slave_addr<<1)|(0x00))))
            return 1;
	IdleI2C1();
        if (MasterWriteI2C1(reg_addr))
            return 1;
	IdleI2C1();
        StartI2C1();                        //Send the Start Bit
        IdleI2C1();                         //Wait to complete
        if (MasterWriteI2C1(((slave_addr<<1)|(0x01))))
            return 1;
	IdleI2C1();
	I2CReceiverEnable ( I2C1, TRUE);

	for(i=0;i<length;i++) {
            data[i] = MasterReadI2C1();
		if(i<(length-1)) {
                    I2CAcknowledgeByte(MPU_I2C, TRUE);
                    IdleI2C1();
		}
		else {
                    I2CAcknowledgeByte(MPU_I2C, FALSE);
                    IdleI2C1();
		}
	}
        StopI2C1();								//Send the Stop condition
        IdleI2C1();								//Wait to complete

	return 0;
}