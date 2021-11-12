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

#include "mcc_generated_files/mcc.h"
#include <math.h>
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"
#include <string.h>
#include <util/delay.h>
#include "i2c_simple_master.h"


#define R1 10000.0F
#define C1 1.009249522E-03F
#define C2 2.378405444E-04F
#define C3 2.019202697E-07F

#define MCP9809_ADDR				0x18 
#define MCP9808_REG_TA				0x05

volatile uint8_t rf_interrupt = false;

void floatToByteArray(float f, uint8_t* temp_p ) {
    uint8_t i;
    for (i = 0; i < 4; i++) {
        temp_p[i] = ((uint8_t*)&f)[i];
    }
}

float getTemperature()
{
    float logR2, R2;
    uint16_t adc_result;
    adc_result = ADC0_GetConversion(ADC_MUXPOS_AIN7_gc);
    /* Shift result by 6 because adc samples are accumulated */
    adc_result = adc_result >> 6;
    
    /* Convert the raw ADC value to celsius */
    R2 = ((R1 * 1023.0) / (float)adc_result) - (1.0 * R1);
    logR2 = log(R2);
    return ((1.0 / (C1 + C2*logR2 + C3*logR2*logR2*logR2)) - 273.15);
}



int32_t SENSORS_getTempValue (void)
{
    uint32_t temperature;
    int8_t sign=0;
    uint8_t lowByte, highByte;
    
    temperature = i2c_read2ByteRegister(MCP9809_ADDR, MCP9808_REG_TA);
    sign = (temperature & 0x1000) ? 1 : (-1);
    temperature &= 0xFFF;
    lowByte = (uint8_t)temperature;
    highByte= (uint8_t)(temperature >> 8);
    if(sign)
    {
    temperature = 25600 - (highByte * 1600 + (lowByte * 100) / 16);
    }
    else
    {
    temperature = 25600 - (highByte * 1600 + (lowByte * 100) / 16);
    }
    return temperature;
}

/*
    Main application
*/

void nrf24_ISR_Handler();
nRF24L01 *setup_rf(void);

int main(void)
{   
    /* Initializes MCU, drivers and middleware */
    SYSTEM_Initialize();
    static uint8_t buffer[8], i=0, on;
    uint8_t to_address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
    nRF24L01 *rf = setup_rf();
    uint8_t data;
    float tempc, amb_temp_f;
    int16_t amb_temp;
    nRF24L01_read_register(rf, CONFIG, &data, 1);
    printf("CONFIG: %x\n", data);
    nRF24L01_read_register(rf, EN_AA, &data, 1);
    printf("EN_AA: %x\n", data);
    nRF24L01_read_register(rf, EN_RXADDR, &data, 1);
    printf("EN_RXADDR: %x\n", data);
    nRF24L01_read_register(rf, SETUP_AW, &data, 1);
    printf("SETUP_AW: %x\n", data);
    nRF24L01_read_register(rf, SETUP_RETR, &data, 1);
    printf("SETUP_RETR: %x\n", data);
    nRF24L01_read_register(rf, RF_CH, &data, 1);
    printf("RF_CH: %x\n", data);
    nRF24L01_read_register(rf, RF_SETUP, &data, 1);
    printf("RF_SETUP: %x\n", data);
    nRF24L01_read_register(rf, STATUS, &data, 1);
    printf("STATUS: %x\n", data);
    nRF24L01_read_register(rf, FIFO_STATUS, &data, 1);
    printf("FIFO_STATUS: %x\n", data);
    nRF24L01_read_register(rf, FEATURE, &data, 1);
    printf("FEATURE: %x\n", data);
    PORTD_INT_NRF24_SetInterruptHandler(&nrf24_ISR_Handler);
    /* Replace with your application code */
    while (1){
        floatToByteArray(getTemperature(), buffer);
        tempc = getTemperature();
        amb_temp = SENSORS_getTempValue();
        amb_temp_f = amb_temp / 1000.00;
        floatToByteArray(amb_temp_f, buffer+4);
        //for(i=0; i<sizeof(buffer); i++)
       // {
            //USART2_Write(buffer[i]);
        //}
        printf("Temp: %.2f\n", tempc);
        
        if (rf_interrupt) {
            rf_interrupt = false;
            int success = nRF24L01_transmit_success(rf);
            if (success != 0)
                nRF24L01_flush_transmit_message(rf);
        }


            on = !on;
            nRF24L01Message msg;
            memcpy(msg.data, buffer, 8);
            //else memcpy(msg.data, "OFF", 4);
            msg.length = 32;
            printf("Ambient: %i", amb_temp);
            nRF24L01_transmit(rf, to_address, &msg);
            _delay_ms(500);
        
    }
    
}

nRF24L01 *setup_rf(void) {
    nRF24L01 *rf = nRF24L01_init();
    rf->ss.port = 0;
    rf->ss.pin = 0;
    rf->ce.port = 0;
    rf->ce.pin = 0;
    rf->sck.port = 0;
    rf->sck.pin = 0;
    rf->mosi.port = 0;
    rf->mosi.pin = 0;
    rf->miso.port = 0;
    rf->miso.pin = 0;
    // interrupt on falling edge of INT0 (PD2)
    nRF24L01_begin(rf);
    return rf;
}


void nrf24_ISR_Handler()
{
    rf_interrupt = true;
    RED_LED_Toggle();
}
/**
    End of File
*/