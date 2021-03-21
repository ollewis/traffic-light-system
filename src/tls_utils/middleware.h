/*
 * middleware.h
 *
 *  Created on: Mar 8, 2021
 *      Author: oliverlewis
 */

#ifndef MIDDLEWARE_H_
#define MIDDLEWARE_H_

#include <stdint.h>

#include "../Libraries/STM32F4xx_StdPeriph_Driver/inc/stm32f4xx_adc.h"

/*-----------------------------------------------------------*/
// TRAFFIC FLOW via. ADC
/*-----------------------------------------------------------*/

uint8_t xGetTrafficFlow();

/*-----------------------------------------------------------*/
// TRAFFIC LIGHTS
/*-----------------------------------------------------------*/

/* Reset all traffic lights to off. */
void vResetTrafficLights ( void );

/* Set the green traffic light. */
void vSetGreen ( void );

/* Set the amber traffic light. */
void vSetAmber ( void );

/* Set the red traffic light. */
void vSetRed ( void );

/*-----------------------------------------------------------*/
// TRAFFIC LEDS
/*-----------------------------------------------------------*/

/* Set traffic LEDs */
void vSetTraffic ( uint32_t );

/* Turns off all Traffic LEDs */
void prvClearTraffic ( void );

/* Inserts a high value into the traffic flow. */
void prvInsertHigh ( void );

/* Inserts a low value into the traffic flow. */
void prvInsertLow ( void );

#endif /* MIDDLEWARE_H_ */
