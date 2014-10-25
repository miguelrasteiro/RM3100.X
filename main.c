#include <p32xxxx.h>
#include <plib.h>
#include <math.h>
#include "uart.h"
#include "hardware.h"
#include "rm3100.h"
#include "i2c.h"

#define PI          3.14159265358979

#define MAG_EXT_CAL 0                   /**< 1 - Using Calibrated Matrix; 0 - Default */

/*================================================================
                 G L O B A L   V A R I A B L E S
================================================================*/
// timer 1
UINT8 T1overflow = 1;
// external mpu int
//BYTE new_data=0;


#if !MAG_EXT_CAL                    /**< Identity matrix if not using external calibration data. */
    float mag_offsets [3] = {0, 0, 0};
    float mag_cal_matrix [3][3] = {{1, 0, 0 },
                                   {0, 1, 0 },
                                   {0, 0, 1 }};
#else
    float mag_offsets [3] = {   };                 /// from a calibration example
    float mag_cal_matrix [3][3] = {{   }, /// from a calibration example
                                   {   },
                                   {   }};
#endif

/*================================================================
             F U N C T I O N S   P R O T O T Y P E S
================================================================*/
// auxiliary
float get_time (void);
void  reset_timer (void);

/**
 * Timer 1 ISR
 *  Interrupt Priority Level = 2
 *  Vector 4
 */
void __ISR(TIMER_1_INT_VECTOR, ipl1) _Timer1Handler(void) {
    // Clear the interrupt flag
    mT1ClearIntFlag();
    // Protection to timer overflow
    T1overflow ++;
}

//    void __ISR(EXTERNAL_2_INT_VECTOR, ipl2) INT2Interrupt() {
//
//        mINT2ClearIntFlag();
//        new_data = 1;
//    }


/*================================================================
==================================================================
                   E N T R Y   P O I N T
==================================================================
================================================================*/
int main(void)
{
    BoardInit();                // PIC configurations + i2c,spi,uart,timers,interruptions inicializations

    int i = 0;
    i = getRM3100Status ();
    RM3100_init_SM_Operation ();
    //RM3100_init_CMM_Operation ();

    sensor_xyz raw;
    BYTE buf[64];
    float converted_x,converted_y,converted_z;
    float cycle_time,time_elapsed=0;
    float gain = getRM3100Gain ();

    float time_to_send=0;
    TRISAbits.TRISA2  = 0;	// set RA2 out

    while(1){

        cycle_time = get_time();

        if (cycle_time > 0.01 && !time_to_send){
            time_to_send = cycle_time;
            reset_timer();
            requestSingleMeasurement ();
        }

        if(getDataReadyStatus ()){
//        if(new_data){
            LATAbits.LATA2 = 1;
            time_elapsed = time_elapsed + time_to_send;

//            new_data=0;

            raw = ReadRM3100Raw ( );

            converted_x = (float) raw.x / gain ;
            converted_y = (float) raw.y / gain ;
            converted_z = (float) raw.z / gain ;

            sprintf(buf,"%.1f   %.1f   %.1f   %f\n",converted_x,converted_y,converted_z, time_to_send);
            SendDataBuffer(buf, strlen(buf));
            time_to_send=0;
            LATAbits.LATA2 = 0;
        }
    }
    return -1;
}


/** Temporization functions */
float get_time (void)
{
    unsigned int temptime;
    float elapsed_time;

    // Colocar em função
    temptime = ReadTimer1(); // Reads time elapsed
    elapsed_time = (float)((double) (temptime * T1overflow * 256 * 4) / PBCLK );// tempo = valortimer*overflow*prescaler*4/fosc ;

    return elapsed_time;
}

void reset_timer (void)
{
    WriteTimer1(0x00);  // Timer set to 0
    T1overflow = 1;
}
