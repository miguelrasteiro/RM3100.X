#include <stdio.h>
#include "hardware.h"
#include "uart.h"
#include "i2c.h"

/** Configuration Bit settings
 * SYSCLK = 80 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
 * PBCLK = 80 MHz (SYSCLK / FPBDIV)
 * Primary Osc w/PLL (XT+,HS+,EC+PLL)
 * WDT OFF
 * Other options are don't care
 */
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = XT, FNOSC = PRIPLL, FPBDIV = DIV_1

//#pragma config WDTPS = PS16 // WDT timeout period = 16ms

void BoardInit(void)
{
    mJTAGPortEnable(0);                      		// disable JTAG

    /// PIN SETUP ///
    // setup I/O for RF module
//    RADIO_CS_TRIS     = 0;
//    RADIO_CS          = 1;				// deselect chip for now
//    RADIO_RESETn_TRIS = 0;
//    RADIO_RESETn      = 1;			// can be just tied high
//    RF_INT_TRIS       = 1;
//    SDI_TRIS          = 1;
//    SDO_TRIS          = 0;
//    SCK_TRIS          = 0;
//    SPI_SDO           = 0;
//    SPI_SCK           = 0;
//    RADIO_WAKE_TRIS   = 0;
//    RADIO_WAKE        = 1;				// can be just tied high

    // setup I/O for other stuff
    TRISAbits.TRISA2  = 0;	// set RA2 out for debug

    /// SPI 1 SETUP ////
//#ifdef HARDWARE_SPI
//    unsigned int pbFreq;
//    int SPI_Brg = -1;				// all 1s, 9 bits wide
//
//    SPI1CON = 0x00008120;				// SP1 setup: enable, CKE, Master, normal mode
//    pbFreq = (UINT32)FOSC / (1 << mOSCGetPBDIV() );	// Peripheral Bus Frequency = System Clock / PB Divider
//    while (pbFreq/(2*(++SPI_Brg+1)) > MAX_SPI_CLK_FREQ);	// SPI clock rate per PIC32 datasheet, section 23.3.7
//    SPI1BRG=SPI_Brg;					// set SPI baud rate
//#endif

    /// UART 1 SETUP ////
    UARTConfigure(UART_MODULE_ID, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART_MODULE_ID, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART_MODULE_ID, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART_MODULE_ID, GetPeripheralClock(), UARTBAUDRATE);
    UARTEnable(UART_MODULE_ID, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    /// TIMER 1 SETUP /////
    OpenTimer1(T1_ON | T1_PS_1_256, 0xFFFF);
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_1 | T1_INT_SUB_PRIOR_0);// Set up the core timer interrupt with a priority of 2 and zero sub-priority

    /// I2C 1 SETUP ////
     i2c_init(MPU_I2C, MASTER, 0); //Enable I2C channel

    /// EXTERNAL INTERRUPTIONS SETUP ////
    // ConfigINT2(EXT_INT_PRI_2 | RISING_EDGE_INT | EXT_INT_ENABLE); // EXT INT2
    // EXT INT 1
//    mINT1SetIntPriority(4);	/* Set the Interrupt Priority */
//    mINT1SetIntSubPriority(2);	/* Set Interrupt Subpriority Bits for INT1 */
//    mINT1SetEdgeMode(0);	/* Set INT1 to falling edge */
//    mINT1IntEnable(1);	/* Enable INT1 */

    INTEnableSystemMultiVectoredInt(); // Enable multi-vector interrupts

    // just to assure
//    RFIF = 0;
//    if( RF_INT_PIN == 0 )
//            RFIF = 1;
}
