/**
 *  Driver developed in HERMES project
 */
/**
 *  @addtogroup  RM3100
 *  @brief       Hardware drivers to communicate with sensors via I2C.
 *  @{
 *      @file       rm3100.c
 *      @brief      An I2C-based driver for PNI RM3100 Evaluation Board.
 *      @details    Change History:
 *                  ************************************************************************
 *                  | VERSION      | DATE	    | AUTHORS	  | DESCRIPTION            |
 *                  |:------------:|:--------------:|:------------|:-----------------------|
 *                  | 1.0          |    8/9/2014    | MR          | First Release          |
 *                  \n\n
 */
#include "i2c.h"
#include "rm3100.h"
#include <plib.h>

/** @details Information of the ASIC configurations */
struct config {
    unsigned int cycle_count;
    float sample_rate;
    float max_data_rate;
    float gain;
};
/* Default Values */
struct config rm = {
    .cycle_count   = 200,
    .sample_rate   = 37,
    .max_data_rate = 440,
    .gain          = 75
};

/**
 *  @brief  Inicializes the Ev. Board to do Single Measurents (On Request).
 *  in main cycle is necessary to use requestSingleMeasurement() and
 *  monotoring DRDY, or STATUS register to know when data is ready to be read.
 *  Sets Cycle Count to 200 cycles/s.
 *  @param[in]  none
 *  @return     none
 */
void RM3100_init_SM_Operation ( void ) {

    setCycleCount ( 200 );
    continuousModeConfig ( CMM_ALL_AXIS_ON | CMM_OFF );
}
/**
 *  @brief  Inicializes the Ev. Board in Continuous Measurents Mode.
 *  Must monotoring DRDY, or STATUS register to know when data is ready to be read.
 *  Sets Cycle Count to 200 cycles/s.
 *  Sets data rate to 300 readings/s.
 *  @param[in]  none
 *  @return     none
 */
void RM3100_init_CMM_Operation ( void ) {

    setCycleCount ( 200 );
    continuousModeConfig ( CMM_ALL_AXIS_ON | DRDY_WHEN_ALL_AXIS_MEASURED | CM_START );
    setCMMdatarate(CMM_UPDATERATE_300);
}


/**
 *  @brief  request current gain value - Only depends off cycle count value
 *  @param[in]  none
 *  @return     calculated gain (sensibility)
 */
float getRM3100Gain (void){ return rm.gain; }
/**
 *  @brief  request current sample rate
 *  @param[in]  none
 *  @return     actual sample rate
 */
float getRM3100SampleRate (void){ return rm.sample_rate; }
/**
 *  @brief  request max data rate possible - Only depends off cycle count value
 *  @param[in]  none
 *  @return     max data rate
 */
float getRM3100MaxDataRate (void){ return rm.max_data_rate; }
/**
 *  @brief  request cycle count value
 *  @param[in]  none
 *  @return     actual cycle count value
 */
unsigned int getRM3100CycleCount (void){ return rm.cycle_count; }

/**
 *  @brief  Sets cycle count and updates gain and max_data_rate values
 *  @param[in]  desire value PNI recomends values between 30 and 400 Hz
 *  @return     0 if successful, 1 otherwise.
 */
BOOL setCycleCount ( unsigned int value ) {

    BYTE to_reg[6];

    if (value > 65535 || value<0)
        return TRUE;
    else{

        if (value > 400)
            value=400;
        else if (value < 30)
            value=30;

        to_reg[0] = value>>8;
        to_reg[1] = value;
        to_reg[2] = to_reg[0];
        to_reg[3] = to_reg[1];
        to_reg[4] = to_reg[0];
        to_reg[5] = to_reg[1];

        if (i2c_write(RM3100_ADDRESS_00, CCX_MSB_REG, 6,to_reg))
            return TRUE;
        else{
            rm.cycle_count   = value;
            rm.gain          = (float) rm.cycle_count * 0.37 + 1;

            float x = (float) 0.000011*value + 0.000075;
            rm.max_data_rate = 1 / x;
        }
    }
    return FALSE;
}
/**
 *  @brief  Sets data rate in Continuous Measurement Mode.
 *  Fails if desire datarate is higher than the max data rate recommended by PNI.
 *  @param[in]  Data rate configuration BYTE
 *  @return     0 if successful, 1 otherwise.
 */
BOOL setCMMdatarate ( BYTE conf ) {

    if (conf < CMM_UPDATERATE_600 || conf > CMM_UPDATERATE_0_075)
        return TRUE;
    else{
        BYTE *ptr;
        float temp = 1000;

        switch (conf){
            case CMM_UPDATERATE_600: temp=600;
                                    break;
            case CMM_UPDATERATE_300: temp=300;
                                    break;
            case CMM_UPDATERATE_150: temp=150;
                                    break;
            case CMM_UPDATERATE_75 : temp=75;
                                    break;
            case CMM_UPDATERATE_37 : temp=37;
                                    break;
            case CMM_UPDATERATE_18 : temp=18;
                                    break;
            case CMM_UPDATERATE_9  : temp=9;
                                    break;
            case CMM_UPDATERATE_4_5: temp=4.5;
                                    break;
            case CMM_UPDATERATE_2_3: temp=2.3;
                                    break;
            case CMM_UPDATERATE_1_2: temp=1.2;
                                    break;
            case CMM_UPDATERATE_0_6: temp=0.6;
                                    break;
            case CMM_UPDATERATE_0_3: temp=0.3;
                                    break;
            case CMM_UPDATERATE_0_15: temp=0.15;
                                    break;
            case CMM_UPDATERATE_0_075: temp=0.075;
                                    break;
        }
        if (temp > rm.max_data_rate)
            return TRUE;

        ptr = &conf ;
        if (i2c_write(RM3100_ADDRESS_00, CMM_TMRC_REG, 1, ptr))
            return TRUE;
        else{
            rm.sample_rate = temp;
        }
    }
    return FALSE;
}
/**
 *  @brief  Continuous Measurement Mode (CMM) Register Configuration.
 *  @param[in]  CMM configuration BYTE
 *  @return     0 if successful, 1 otherwise.
 */
BOOL continuousModeConfig ( BYTE conf ) {

    BYTE *ptr;

    ptr = &conf ;
    if (i2c_write(RM3100_ADDRESS_00, CMM_REG, 1, ptr))
        return TRUE;

    return FALSE;
}
/**
 *  @brief      Request Single Measurement to PNI ASIC.
 *  @param[in]  none
 *  @return     0 if successful, 1 otherwise.
 */
BOOL requestSingleMeasurement ( void ) {

    BYTE to_reg = SM_ALL_AXIS;
    BYTE *ptr;

    ptr = &to_reg;
    if (i2c_write(RM3100_ADDRESS_00, POLL_REG, 1, ptr))
        return TRUE;

    return FALSE;
}
/**
 *  @brief      Get data ready Status
 *  @param[in]  none
 *  @return     1 if data ready, 0 otherwise.
 */
BOOL getDataReadyStatus ( void ) {

    BYTE data[1];

    i2c_read(RM3100_ADDRESS_00, STATUS_REG, 1, data);

    return (data[0] & STATUS_MASK);
}
/**
 *  @brief      Self test to Ev Board.
 *  @param[in]  none
 *  @return     1 if all axis OK, 0 otherwise.
 */
BOOL getRM3100Status ( void ) {

    BYTE data[1];
    BYTE to_reg = STE_ON | BW_11 | BP_11;
    BYTE *ptr;

    ptr = &to_reg;
    if (i2c_write(RM3100_ADDRESS_00, BIST_REG, 1, ptr))
        return FALSE;

    if (requestSingleMeasurement ())
        return FALSE;

    while(!getDataReadyStatus());
    
    if (i2c_read(RM3100_ADDRESS_00, BIST_REG, 1, data))
        return FALSE;

    if (data[0] & STE_ON){
        to_reg = STE_OFF;
        ptr = &to_reg;
        if (i2c_write(RM3100_ADDRESS_00, BIST_REG, 1, ptr))
            return FALSE;

        return (data[0] & BIST_MASK);
    }
    return FALSE;
}
/**
 *  @brief      Request RM3100 Ev. Board Revision
 *  @param[in]  none
 *  @return     Revision value.
 */
BYTE getRM3100revision ( void ) {

    BYTE data[1];

    i2c_read(RM3100_ADDRESS_00, REVID_REG, 1, data);

    return data[0];
}

/**
 *  @brief      Read the raw magnetic values of all 3 axis.
 *  @param[in]  none
 *  @return     x,y,z 32 bits raw_data of sensor_xyz type
 */
sensor_xyz ReadRM3100Raw ( void ) {
    
    sensor_xyz raw;
    BYTE data[9] ={0};
    long reading = 0;

    i2c_read(RM3100_ADDRESS_00, MX, 9, data);

    reading  = *data <<8;
    reading |= *(data+1);
    reading  = reading <<8;
    reading |= *(data+2);
    reading  = reading <<8;
    raw.x    = reading >>8;

    reading  = *(data+3) <<8;
    reading |= *(data+4);
    reading  = reading <<8;
    reading |= *(data+5);
    reading  = reading <<8;
    raw.y    = reading >>8;

    reading  = *(data+6) <<8;
    reading |= *(data+7);
    reading  = reading <<8;
    reading |= *(data+8);
    reading  = reading <<8;
    raw.z    = reading >>8;

    return raw;
}



//    i2c_read(RM3100_ADDRESS_00, MX, 3, data);
//    reading  = *data <<8;
//    reading |= *(data+1);
//    reading  = reading <<8;
//    reading |= *(data+2);
//    reading  = reading <<8;
//    raw.x   = reading >>8;
//
//    data[4]=0;
//    i2c_read(RM3100_ADDRESS_00, MY, 3, data);
//    reading  = *data <<8;
//    reading |= *(data+1);
//    reading  = reading <<8;
//    reading |= *(data+2);
//    reading  = reading <<8;
//    raw.y   = reading >>8;
//
//    data[4]=0;
//    i2c_read(RM3100_ADDRESS_00, MZ, 3, data);
//    reading  = *data <<8;
//    reading |= *(data+1);
//    reading  = reading <<8;
//    reading |= *(data+2);
//    reading  = reading <<8;
//    raw.z   = reading >>8;
