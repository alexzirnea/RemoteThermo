#include "mcc_generated_files/mcc.h"
#include "nrf24l01.h"
#include "sensors.h"
#include <util/delay.h>
#include <string.h>
#include <avr/sleep.h>

static void floatToByteArray(float f, uint8_t* temp_p );
static void nrf24_ISR_Handler(void);
static void goToSleep(void);
static void wakeUp(void);
static void process(void);

static nRF24L01 *setup_rf(void);

volatile uint8_t rf_interrupt = false;
uint8_t to_address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
nRF24L01Message msg;



nRF24L01 *rf;
/*
    Main application
*/
int main(void)
{   
    /* Initializes MCU, drivers and middleware */
    SYSTEM_Initialize();
   
    rf = setup_rf();
    
    RTC_SetPITIsrCallback(wakeUp);
    
    msg.length = 32;
    
    /* Set ISR callback for pinchange interrupt */
    //PORTD_INT_NRF24_SetInterruptHandler(&nrf24_ISR_Handler);
    
    while (1)
    {
        sleep_cpu();
    }  
}

static nRF24L01 *setup_rf(void)
{
    nRF24L01 *rf = nRF24L01_init();
    rf->channel = 0x64;

    nRF24L01_begin(rf);
    return rf;
}

void nrf24_ISR_Handler()
{
    rf_interrupt = true;
    RED_LED_Toggle();
}

static void floatToByteArray(float f, uint8_t* temp_p ) 
{
    uint8_t i;
    for (i = 0; i < 4; i++) {
        temp_p[i] = ((uint8_t*)&f)[i];
    }
}

static void goToSleep(void)
{
    nRF24L01_sleep(rf);
    SENSORS_sleep();
    GRN_LED_SetHigh();
}

static void wakeUp(void)
{
    GRN_LED_SetLow();
    SENSORS_wakeUp();
    nRF24L01_wakeUp(rf);
    process();
}

static void process(void)
{
    static uint8_t payload[8], txOk;
    float poolWaterTempC, ambTempC;
    
    poolWaterTempC = SENSORS_getPoolWaterTemp();
    floatToByteArray(poolWaterTempC, payload);
        
    ambTempC = SENSORS_getAmbientTemp();
    floatToByteArray(ambTempC, payload+4);
        
    if (rf_interrupt) 
    {
        rf_interrupt = false;
        txOk = nRF24L01_transmit_success(rf);
        if (!txOk)
        {
            nRF24L01_flush_transmit_message(rf);
        }   
    }

    memcpy(msg.data, payload, 8);
        
    nRF24L01_transmit(rf, to_address, &msg);   
    goToSleep();
}

/**
    End of File
*/