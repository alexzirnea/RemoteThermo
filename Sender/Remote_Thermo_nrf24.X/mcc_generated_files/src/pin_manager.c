/**
  @Company
    Microchip Technology Inc.

  @Description
    This Source file provides APIs.
    Generation Information :
    Driver Version    :   1.0.0
*/
/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/


#include "../include/pin_manager.h"
static void (*PORTA_PA2_InterruptHandler)(void);
static void (*PORTC_CS_NRF24_InterruptHandler)(void);
static void (*PORTD_CE_NRF24_InterruptHandler)(void);
static void (*PORTA_PA4_InterruptHandler)(void);
static void (*PORTA_PA3_InterruptHandler)(void);
static void (*PORTD_INT_NRF24_InterruptHandler)(void);
static void (*PORTA_PA6_InterruptHandler)(void);
static void (*PORTA_PA5_InterruptHandler)(void);
static void (*PORTA_CS_WINC_InterruptHandler)(void);
static void (*PORTF_PF1_InterruptHandler)(void);
static void (*PORTF_PF0_InterruptHandler)(void);
static void (*PORTD_YLW_LED_InterruptHandler)(void);
static void (*PORTD_RED_LED_InterruptHandler)(void);
static void (*PORTD_BLU_LED_InterruptHandler)(void);
static void (*PORTD_GRN_LED_InterruptHandler)(void);

void PORT_Initialize(void);

void PIN_MANAGER_Initialize()
{
    PORT_Initialize();

    /* DIR Registers Initialization */
    PORTA.DIR = 0xD0;
    PORTB.DIR = 0x00;
    PORTC.DIR = 0x08;
    PORTD.DIR = 0x1F;
    PORTE.DIR = 0x00;
    PORTF.DIR = 0x01;

    /* OUT Registers Initialization */
    PORTA.OUT = 0x00;
    PORTB.OUT = 0x00;
    PORTC.OUT = 0x00;
    PORTD.OUT = 0x0F;
    PORTE.OUT = 0x00;
    PORTF.OUT = 0x00;

    /* PINxCTRL registers Initialization */
    PORTA.PIN0CTRL = 0x00;
    PORTA.PIN1CTRL = 0x00;
    PORTA.PIN2CTRL = 0x00;
    PORTA.PIN3CTRL = 0x00;
    PORTA.PIN4CTRL = 0x00;
    PORTA.PIN5CTRL = 0x00;
    PORTA.PIN6CTRL = 0x00;
    PORTA.PIN7CTRL = 0x00;
    PORTB.PIN0CTRL = 0x00;
    PORTB.PIN1CTRL = 0x00;
    PORTB.PIN2CTRL = 0x00;
    PORTB.PIN3CTRL = 0x00;
    PORTB.PIN4CTRL = 0x00;
    PORTB.PIN5CTRL = 0x00;
    PORTB.PIN6CTRL = 0x00;
    PORTB.PIN7CTRL = 0x00;
    PORTC.PIN0CTRL = 0x00;
    PORTC.PIN1CTRL = 0x00;
    PORTC.PIN2CTRL = 0x00;
    PORTC.PIN3CTRL = 0x00;
    PORTC.PIN4CTRL = 0x00;
    PORTC.PIN5CTRL = 0x00;
    PORTC.PIN6CTRL = 0x00;
    PORTC.PIN7CTRL = 0x00;
    PORTD.PIN0CTRL = 0x00;
    PORTD.PIN1CTRL = 0x00;
    PORTD.PIN2CTRL = 0x00;
    PORTD.PIN3CTRL = 0x00;
    PORTD.PIN4CTRL = 0x00;
    PORTD.PIN5CTRL = 0x00;
    PORTD.PIN6CTRL = 0x03;
    PORTD.PIN7CTRL = 0x00;
    PORTE.PIN0CTRL = 0x00;
    PORTE.PIN1CTRL = 0x00;
    PORTE.PIN2CTRL = 0x00;
    PORTE.PIN3CTRL = 0x00;
    PORTE.PIN4CTRL = 0x00;
    PORTE.PIN5CTRL = 0x00;
    PORTE.PIN6CTRL = 0x00;
    PORTE.PIN7CTRL = 0x00;
    PORTF.PIN0CTRL = 0x00;
    PORTF.PIN1CTRL = 0x00;
    PORTF.PIN2CTRL = 0x00;
    PORTF.PIN3CTRL = 0x00;
    PORTF.PIN4CTRL = 0x00;
    PORTF.PIN5CTRL = 0x00;
    PORTF.PIN6CTRL = 0x00;
    PORTF.PIN7CTRL = 0x00;

    /* PORTMUX Initialization */
    PORTMUX.CCLROUTEA = 0x00;
    PORTMUX.EVSYSROUTEA = 0x00;
    PORTMUX.TCAROUTEA = 0x00;
    PORTMUX.TCBROUTEA = 0x00;
    PORTMUX.TWISPIROUTEA = 0x00;
    PORTMUX.USARTROUTEA = 0x00;

    // register default ISC callback functions at runtime; use these methods to register a custom function
    PORTA_PA2_SetInterruptHandler(PORTA_PA2_DefaultInterruptHandler);
    PORTC_CS_NRF24_SetInterruptHandler(PORTC_CS_NRF24_DefaultInterruptHandler);
    PORTD_CE_NRF24_SetInterruptHandler(PORTD_CE_NRF24_DefaultInterruptHandler);
    PORTA_PA4_SetInterruptHandler(PORTA_PA4_DefaultInterruptHandler);
    PORTA_PA3_SetInterruptHandler(PORTA_PA3_DefaultInterruptHandler);
    PORTD_INT_NRF24_SetInterruptHandler(PORTD_INT_NRF24_DefaultInterruptHandler);
    PORTA_PA6_SetInterruptHandler(PORTA_PA6_DefaultInterruptHandler);
    PORTA_PA5_SetInterruptHandler(PORTA_PA5_DefaultInterruptHandler);
    PORTA_CS_WINC_SetInterruptHandler(PORTA_CS_WINC_DefaultInterruptHandler);
    PORTF_PF1_SetInterruptHandler(PORTF_PF1_DefaultInterruptHandler);
    PORTF_PF0_SetInterruptHandler(PORTF_PF0_DefaultInterruptHandler);
    PORTD_YLW_LED_SetInterruptHandler(PORTD_YLW_LED_DefaultInterruptHandler);
    PORTD_RED_LED_SetInterruptHandler(PORTD_RED_LED_DefaultInterruptHandler);
    PORTD_BLU_LED_SetInterruptHandler(PORTD_BLU_LED_DefaultInterruptHandler);
    PORTD_GRN_LED_SetInterruptHandler(PORTD_GRN_LED_DefaultInterruptHandler);
}

void PORT_Initialize(void)
{
    /* On AVR devices all peripherals are enable from power on reset, this
     * disables all peripherals to save power. Driver shall enable
     * peripheral if used */

    /* Set all pins to low power mode */
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTE + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTF + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTA + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTB + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTC + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
    for (uint8_t i = 0; i < 8; i++) {
        *((uint8_t *)&PORTD + 0x10 + i) |= 1 << PORT_PULLUPEN_bp;
    }
    
}

/**
  Allows selecting an interrupt handler for PORTA_PA2 at application runtime
*/
void PORTA_PA2_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_PA2_InterruptHandler = interruptHandler;
}

void PORTA_PA2_DefaultInterruptHandler(void)
{
    // add your PORTA_PA2 interrupt custom code
    // or set custom function using PORTA_PA2_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTC_CS_NRF24 at application runtime
*/
void PORTC_CS_NRF24_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTC_CS_NRF24_InterruptHandler = interruptHandler;
}

void PORTC_CS_NRF24_DefaultInterruptHandler(void)
{
    // add your PORTC_CS_NRF24 interrupt custom code
    // or set custom function using PORTC_CS_NRF24_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_CE_NRF24 at application runtime
*/
void PORTD_CE_NRF24_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_CE_NRF24_InterruptHandler = interruptHandler;
}

void PORTD_CE_NRF24_DefaultInterruptHandler(void)
{
    // add your PORTD_CE_NRF24 interrupt custom code
    // or set custom function using PORTD_CE_NRF24_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_PA4 at application runtime
*/
void PORTA_PA4_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_PA4_InterruptHandler = interruptHandler;
}

void PORTA_PA4_DefaultInterruptHandler(void)
{
    // add your PORTA_PA4 interrupt custom code
    // or set custom function using PORTA_PA4_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_PA3 at application runtime
*/
void PORTA_PA3_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_PA3_InterruptHandler = interruptHandler;
}

void PORTA_PA3_DefaultInterruptHandler(void)
{
    // add your PORTA_PA3 interrupt custom code
    // or set custom function using PORTA_PA3_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_INT_NRF24 at application runtime
*/
void PORTD_INT_NRF24_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_INT_NRF24_InterruptHandler = interruptHandler;
}

void PORTD_INT_NRF24_DefaultInterruptHandler(void)
{
    // add your PORTD_INT_NRF24 interrupt custom code
    // or set custom function using PORTD_INT_NRF24_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_PA6 at application runtime
*/
void PORTA_PA6_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_PA6_InterruptHandler = interruptHandler;
}

void PORTA_PA6_DefaultInterruptHandler(void)
{
    // add your PORTA_PA6 interrupt custom code
    // or set custom function using PORTA_PA6_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_PA5 at application runtime
*/
void PORTA_PA5_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_PA5_InterruptHandler = interruptHandler;
}

void PORTA_PA5_DefaultInterruptHandler(void)
{
    // add your PORTA_PA5 interrupt custom code
    // or set custom function using PORTA_PA5_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTA_CS_WINC at application runtime
*/
void PORTA_CS_WINC_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTA_CS_WINC_InterruptHandler = interruptHandler;
}

void PORTA_CS_WINC_DefaultInterruptHandler(void)
{
    // add your PORTA_CS_WINC interrupt custom code
    // or set custom function using PORTA_CS_WINC_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTF_PF1 at application runtime
*/
void PORTF_PF1_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTF_PF1_InterruptHandler = interruptHandler;
}

void PORTF_PF1_DefaultInterruptHandler(void)
{
    // add your PORTF_PF1 interrupt custom code
    // or set custom function using PORTF_PF1_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTF_PF0 at application runtime
*/
void PORTF_PF0_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTF_PF0_InterruptHandler = interruptHandler;
}

void PORTF_PF0_DefaultInterruptHandler(void)
{
    // add your PORTF_PF0 interrupt custom code
    // or set custom function using PORTF_PF0_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_YLW_LED at application runtime
*/
void PORTD_YLW_LED_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_YLW_LED_InterruptHandler = interruptHandler;
}

void PORTD_YLW_LED_DefaultInterruptHandler(void)
{
    // add your PORTD_YLW_LED interrupt custom code
    // or set custom function using PORTD_YLW_LED_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_RED_LED at application runtime
*/
void PORTD_RED_LED_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_RED_LED_InterruptHandler = interruptHandler;
}

void PORTD_RED_LED_DefaultInterruptHandler(void)
{
    // add your PORTD_RED_LED interrupt custom code
    // or set custom function using PORTD_RED_LED_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_BLU_LED at application runtime
*/
void PORTD_BLU_LED_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_BLU_LED_InterruptHandler = interruptHandler;
}

void PORTD_BLU_LED_DefaultInterruptHandler(void)
{
    // add your PORTD_BLU_LED interrupt custom code
    // or set custom function using PORTD_BLU_LED_SetInterruptHandler()
}
/**
  Allows selecting an interrupt handler for PORTD_GRN_LED at application runtime
*/
void PORTD_GRN_LED_SetInterruptHandler(void (* interruptHandler)(void)) 
{
    PORTD_GRN_LED_InterruptHandler = interruptHandler;
}

void PORTD_GRN_LED_DefaultInterruptHandler(void)
{
    // add your PORTD_GRN_LED interrupt custom code
    // or set custom function using PORTD_GRN_LED_SetInterruptHandler()
}
ISR(PORTD_PORT_vect)
{  
    // Call the interrupt handler for the callback registered at runtime
    if(VPORTD.INTFLAGS & PORT_INT4_bm)
    {
       PORTD_CE_NRF24_InterruptHandler();
    }
    if(VPORTD.INTFLAGS & PORT_INT6_bm)
    {
       PORTD_INT_NRF24_InterruptHandler();
    }
    if(VPORTD.INTFLAGS & PORT_INT1_bm)
    {
       PORTD_YLW_LED_InterruptHandler();
    }
    if(VPORTD.INTFLAGS & PORT_INT0_bm)
    {
       PORTD_RED_LED_InterruptHandler();
    }
    if(VPORTD.INTFLAGS & PORT_INT3_bm)
    {
       PORTD_BLU_LED_InterruptHandler();
    }
    if(VPORTD.INTFLAGS & PORT_INT2_bm)
    {
       PORTD_GRN_LED_InterruptHandler();
    }

    /* Clear interrupt flags */
    VPORTD.INTFLAGS = 0xff;
}
