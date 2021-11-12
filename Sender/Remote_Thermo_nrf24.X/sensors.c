#include "sensors.h"
#include "mcc_generated_files/mcc.h"
#include "i2c_simple_master.h"
#include <math.h>

#define MCP9809_ADDR				0x18 
#define MCP9808_REG_TA				0x05
#define MCP9808_REG_CONFIG          0x01
#define MCP9808_CONFIG_PWR_BIT      8

/* Some constants for steinhart-hart conversion */
#define R1 10000.0F
#define C1 1.009249522E-03F
#define C2 2.378405444E-04F
#define C3 2.019202697E-07F

/* MCP9808 sensor routine */ 
float SENSORS_getAmbientTemp(void)
{
    uint32_t temperature;
    uint8_t lowByte, highByte, sign;
    
    temperature = i2c_read2ByteRegister(MCP9809_ADDR, MCP9808_REG_TA);
    sign = (temperature & 0x1000) ? 1 : 0;
    temperature &= 0xFFF;
    
    lowByte = (uint8_t)temperature;
    highByte= (uint8_t)(temperature >> 8);
    
    if(sign)
    {
        /* Convert temp as per the datasheet but multiply with 100 as not 
         * to lose resolution */
        temperature = 25600 - (highByte * 1600 + (lowByte * 100) / 16);
    }
    else
    {
        /* Convert temp as per the datasheet but multiply with 100 as not 
         * to lose resolution */
        temperature = 25600 - (highByte * 1600 + (lowByte * 100) / 16);
    }
    
    // It could have been so good without floating points..
    return ((float)temperature)/1000.00;
}

/* 
    Basically ADC measurement and conversion of an NTC thermistor
    SLOW! UGLY! NASTY! Should have used a LUT, plenty of memory left
*/
float SENSORS_getPoolWaterTemp(void)
{
    float logR2, R2;
    uint16_t adc_result;
    
    adc_result = ADC0_GetConversion(ADC_MUXPOS_AIN7_gc);
    
    /* Shift result by 6 because adc samples are accumulated 
     See mcc configuration */
    adc_result = adc_result >> 6;
    
    /* Convert the raw ADC value to celsius */
    R2 = ((R1 * 1023.0) / (float)adc_result) - (1.0 * R1);
    logR2 = log(R2);
    
    return ((1.0 / (C1 + C2*logR2 + C3*logR2*logR2*logR2)) - 273.15);
}

/* Squeeze out some extra few hundred microamps :D */
void SENSORS_sleep(void)
{
    uint16_t config;
    config = i2c_read2ByteRegister(MCP9809_ADDR, MCP9808_REG_CONFIG);
    config &= ~(1<<MCP9808_CONFIG_PWR_BIT);
    i2c_write2ByteRegister(MCP9809_ADDR, MCP9808_REG_CONFIG, config);
}

void SENSORS_wakeUp(void)
{
    uint16_t config;
    config = i2c_read2ByteRegister(MCP9809_ADDR, MCP9808_REG_CONFIG);
    config |= (1<<MCP9808_CONFIG_PWR_BIT);
    i2c_write2ByteRegister(MCP9809_ADDR, MCP9808_REG_CONFIG, config);
}