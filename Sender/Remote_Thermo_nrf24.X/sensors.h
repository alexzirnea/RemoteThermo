/* 
 * File:   sensors.h
 * Author: Alexandru Zirnea
 *
 * Created on November 12, 2021, 8:22 PM
 */

#ifndef SENSORS_H
#define	SENSORS_H

#ifdef	__cplusplus
extern "C" {
#endif

float SENSORS_getAmbientTemp(void);
float SENSORS_getPoolWaterTemp(void);
void SENSORS_sleep(void);
void SENSORS_wakeUp(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SENSORS_H */

