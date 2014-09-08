/**
 *  Driver developed in HERMES project - ESTG IPLEIRIA by Miguel Rasteiro
 *  2130904@my.ipleiria.pt
 */
/**
 *  @addtogroup  RM3100
 *  @brief       Hardware drivers to communicate with sensors via I2C.
 *  @{
 *      @file       rm3100.h
 *      @brief      An I2C-based driver for PNI RM3100 Evaluation Board.
 *      @details    Change History:
 *                  ************************************************************************
 *                  | VERSION      | DATE	    | AUTHORS	  | DESCRIPTION            |
 *                  |:------------:|:--------------:|:------------|:-----------------------|
 *                  | 1.0          |    8/9/2014    | MR          | First Release          |
 *                  \n\n
 */
#include <plib.h>

#ifndef RM3100_H
#define	RM3100_H

/** @details Registers of the Ev Board ASIC by PNI */
#define POLL_REG        0x00 /** POLLS A SINGLE MEASUREMENT */
#define CMM_REG         0x01 /** CONTINUOUS MEASUREMENT MODE REGISTER */
// CYCLE COUNT
#define CCX_MSB_REG	0x04 /** CYCLE COUNT REGISTERS */
#define CCX_LSB_REG	0x05
#define CCY_MSB_REG	0x06
#define CCY_LSB_REG	0x07
#define CCZ_MSB_REG	0x08
#define CCZ_LSB_REG	0x09
// SETS CONTINUOUS MEASUREMENT MODE DATA RATE
#define CMM_TMRC_REG    0x0B /** CONTINUOUS MEASUREMENT MODE DATA RATE*/
// MEASUREMENT RESULTS 3BYTES IN 2'S COMPLEMENT FORMAT
#define MX              0x24 /** MEASUREMENT RESULTS REGISTERS */
#define MY              0x27
#define MZ              0x2A
// HANDY REGISTERS
#define BIST_REG        0x33 /** SELF TEST REGISTER */
#define STATUS_REG      0x34 /** DATA READY REGISTER */
#define HSHAKE_REG      0x35
#define REVID_REG       0x36 /** EV BOARD REVISION REGISTER */
// POSSIBLE ADRESSES
#define RM3100_ADDRESS_00     0x20 /** address SA0 & SA1 pins  low (GND) */
#define RM3100_ADDRESS_11     0x23 /** address SA0 & SA1 pins high (VCC) */
#define RM3100_ADDRESS_01     0x21 /** address SA0 high (VCC) & SA1 pins low (GND) */
#define RM3100_ADDRESS_10     0x22 /** address SA0  low (GND) & SA1 pinshigh (VCC) */

/// Configurations for continuous measurement mode
#define CMM_OFF                       0x00
#define CM_START                      0x01
#define ALARM_BIT                     0x02
#define DRDY_WHEN_ALARM_AND_ALL_AXIS  0x00
#define DRDY_WHEN_ANY_AXIS_MEASURED   0x04
#define DRDY_WHEN_ALL_AXIS_MEASURED   0x08
#define DRDY_WHEN_ALARM               0x0C
#define CMM_ALL_AXIS_ON               0x70
#define CMM_X_AXIS                    0x10
#define CMM_Y_AXIS                    0x20
#define CMM_Z_AXIS                    0x40
#define LDM_BIT                       0x80
/// Possible data rate choices
#define CMM_UPDATERATE_600            0x92
#define CMM_UPDATERATE_300            0x93
#define CMM_UPDATERATE_150            0x94
#define CMM_UPDATERATE_75             0x95
#define CMM_UPDATERATE_37             0x96
#define CMM_UPDATERATE_18             0x97
#define CMM_UPDATERATE_9              0x98
#define CMM_UPDATERATE_4_5            0x99
#define CMM_UPDATERATE_2_3            0x9A
#define CMM_UPDATERATE_1_2            0x9B
#define CMM_UPDATERATE_0_6            0x9C
#define CMM_UPDATERATE_0_3            0x9D
#define CMM_UPDATERATE_0_15           0x9E
#define CMM_UPDATERATE_0_075          0x9F

/// Configurations for Self test
#define STE_ON    0x80
#define STE_OFF   0x00
#define BW_00    0x00
#define BW_01    0x04
#define BW_10    0x08
#define BW_11    0x0C
#define BP_00    0x00
#define BP_01    0x01
#define BP_10    0x02
#define BP_11    0x03

#define SM_ALL_AXIS    0x70 /** Single measument mode */
#define STATUS_MASK    0x80 /** To get status of data ready */
#define BIST_MASK      0x70 /** To get status of the Ev Board */

/*************** VAR ****************/
/** @details Saves Raw data from sensors. */
typedef struct {
   long  x,
         y,/// x-axis data.
         z;/// y-axis data.
           /// z-axis data.
}sensor_xyz ;


/************ FUNCTIONS ************/
sensor_xyz  ReadRM3100Raw      ( void );
void RM3100_init_CMM_Operation ( void );
void RM3100_init_SM_Operation  ( void );

BOOL setCycleCount        ( unsigned int );
BOOL setCMMdatarate           ( BYTE );
BOOL continuousModeConfig     ( BYTE );
BOOL requestSingleMeasurement ( void );
BOOL getDataReadyStatus       ( void );
BYTE getRM3100revision        ( void );
BOOL getRM3100Status          ( void );

float        getRM3100Gain        ( void );
float        getRM3100SampleRate  ( void );
float        getRM3100MaxDataRate ( void );
unsigned int getRM3100CycleCount  ( void );

#endif	/* RM3100_H */

