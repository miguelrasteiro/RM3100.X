/**
 *  Driver developed in HERMES project
 */
/**
 *  @addtogroup  I2C
 *  @brief       My I2C lib.
 *  @{
 *      @file       i2c.h
 *      @brief      An I2C lib for i2c operations.
 *      @details    Change History:
 *                  ************************************************************************
 *                  | VERSION      | DATE	    | AUTHORS	  | DESCRIPTION            |
 *                  |:------------:|:--------------:|:------------|:-----------------------|
 *                  | 1.0          |   25/8/2014    | MR          | First Release          |
 *                  \n\n
 */
#ifndef I2C_H
#define	I2C_H

#include <plib.h>
#include <peripheral/i2c.h>

//#define MPU_I2C I2C1

typedef enum
{
    MASTER,
    SLAVE
}i2cmode;

void i2c_init(I2C_MODULE i2cnum, i2cmode mode, BYTE address);
int i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data);
int i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data);

#endif	/* I2C_H */

