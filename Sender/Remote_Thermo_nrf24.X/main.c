#include "mcc_generated_files/mcc.h"
#include "nrf24l01.h"
#include "sensors.h"
#include <util/delay.h>
#include <string.h>

static void floatToByteArray(float f, uint8_t* temp_p );

volatile uint8_t rf_interrupt = false;

void nrf24_ISR_Handler();
nRF24L01 *setup_rf(void);

/*
    Main application
*/
int main(void)
{   
    /* Initializes MCU, drivers and middleware */
    SYSTEM_Initialize();
    static uint8_t payload[8];
    uint8_t to_address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
    nRF24L01 *rf = setup_rf();
    float poolWaterTempC, ambTempC;
    nRF24L01Message msg;
    
    /* Set ISR callback for pinchange interrupt */
    PORTD_INT_NRF24_SetInterruptHandler(&nrf24_ISR_Handler);
    
    while (1){
        
        poolWaterTempC = SENSORS_getPoolWaterTemp();
        floatToByteArray(poolWaterTempC, payload);
        
        ambTempC = SENSORS_getAmbientTemp();
        floatToByteArray(ambTempC, payload+4);
        
        if (rf_interrupt) 
        {
            rf_interrupt = false;
            int success = nRF24L01_transmit_success(rf);
            if (success != 0)
            {
                nRF24L01_flush_transmit_message(rf);
            }
        }

        
        memcpy(msg.data, payload, 8);
        msg.length = 32;
        nRF24L01_transmit(rf, to_address, &msg);
        _delay_ms(500);
    }  
}

nRF24L01 *setup_rf(void) {
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

static void floatToByteArray(float f, uint8_t* temp_p ) {
    uint8_t i;
    for (i = 0; i < 4; i++) {
        temp_p[i] = ((uint8_t*)&f)[i];
    }
}

/**
    End of File
*/