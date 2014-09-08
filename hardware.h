/* 
 * File:   hardware.h
 * Author: MiguelRasteiro
 *
 * Created on 25 de Agosto de 2014, 17:04
 */

#ifndef HARDWARE_H
#define	HARDWARE_H

#define SYS_FREQ                (80000000L)                                       /**< CPU clock frequency */
#define FOSC                    (SYS_FREQ)                                        /**< CPU clock frequency */
#define	GetPeripheralClock()	(SYS_FREQ/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()	(SYS_FREQ)                         /**< Instructions frequency */
#define PBCLK                    SYS_FREQ/4
//#define Fsck        375000
#define BRG                     (400000)      /**< I2C frequency FastMode = 400kHz */
#define UARTBAUDRATE            (230400)
        // For Timers
#define ONE_SECOND              (FOSC/2)                  // 1s of PIC32 core timer ticks (== Hz)
#define MS_TO_CORE_TICKS(x)     ((UINT64)(x)*ONE_SECOND/1000)
#define uS_TO_CORE_TICKS(x)     ((UINT64)(x)*ONE_SECOND/1000000)
#define CT_TICKS_SINCE(tick)    (ReadCoreTimer() - (tick))	// number of core timer ticks since "tick"

#define TIMER_1_INT_VECTOR      (4)                                       /**< Interruption Vector For timer1 */
#define EXTERNAL_2_INT_VECTOR   (11)                                       /**< Interruption Vector For external interrupt 2*/

#define MPU_I2C                 (I2C1)

//Radio
#define BYTEPTR(x)              ((UINT8*)&(x))	// converts x to a UINT8* for bytewise access ala x[foo]

//#define HARDWARE_SPI			// vs. software bit-bang (slower)
//#define MAX_SPI_CLK_FREQ        (20000000L)             // Seems to match Table 5-5 of MRF24J20 datasheet

//// External
//    // Transceiver Configuration
//#define RFIF                IFS0bits.INT1IF             // interrupt input to PIC32
//#define RFIE                IEC0bits.INT1IE
//#define RF_INT_PIN          PORTEbits.RE8               // INT pin on RF module
//#define RF_INT_TRIS         TRISEbits.TRISE8
//
//#define RADIO_CS            LATAbits.LATA5
//#define RADIO_CS_TRIS       TRISAbits.TRISA5
//#define RADIO_RESETn        LATAbits.LATA4		// Not needed; leave floating
//#define RADIO_RESETn_TRIS   TRISAbits.TRISA4
//#define RADIO_WAKE          LATCbits.LATC14		// Not needed; leave floating
//#define RADIO_WAKE_TRIS     TRISCbits.TRISC14
//
//#define SPI_SDI             PORTCbits.RC4
//#define SDI_TRIS            TRISCbits.TRISC4
//#define SPI_SDO             LATDbits.LATD0
//#define SDO_TRIS            TRISDbits.TRISD0
//#define SPI_SCK             LATDbits.LATD10
//#define SCK_TRIS            TRISDbits.TRISD10
//
//#define SPEEDO              (LATFbits.LATF3)            // "speedometer" debug output pin


#endif	/* HARDWARE_H */

