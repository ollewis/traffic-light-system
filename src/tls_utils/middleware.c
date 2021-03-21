/*
 * middleware.c
 *
 *  Created on: Mar 8, 2021
 *      Author: oliverlewis
 */
#include <stdio.h>
#include "middleware.h"

#include "stm32f4xx.h"
#include "../../FreeRTOS_Source/include/FreeRTOS.h"


/*-----------------------------------------------------------*/
// TRAFFIC FLOW via. ADC
/*-----------------------------------------------------------*/

/**
 * Returns the value of the potentiometer for traffic flow.
 *
 * Return values range from 0 - UINT8_MAX.
 **/
uint8_t max = UINT8_MAX;
uint8_t xGetTrafficFlow()
{
	ADC_SoftwareStartConv(ADC1);
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);
}

/*-----------------------------------------------------------*/
// TRAFFIC LIGHTS
/*-----------------------------------------------------------*/

/* Traffic Light Pins */
#define red		GPIO_Pin_0
#define amber	GPIO_Pin_1
#define green	GPIO_Pin_2

/* Turn off all traffic lights. */
void vResetTrafficLights ( void )
{
	GPIO_ResetBits( GPIOC, red | amber | green );
}

/* Set the green traffic light. */
void vSetGreen ( void )
{
	GPIO_ResetBits( GPIOC, red | amber );
	GPIO_SetBits( GPIOC, green );
}

/* Set the amber traffic light. */
void vSetAmber ( void )
{
	GPIO_ResetBits( GPIOC, red | green );
	GPIO_SetBits( GPIOC, amber );
}

/* Set the red traffic light. */
void vSetRed ( void )
{
	GPIO_ResetBits( GPIOC, amber | green );
	GPIO_SetBits( GPIOC, red );
}

/*-----------------------------------------------------------*/
// TRAFFIC LEDS
/*-----------------------------------------------------------*/

/* GPIO Pins for Traffic LEDs */
#define spc_data GPIO_Pin_6
#define spc_clock GPIO_Pin_7
#define spc_reset GPIO_Pin_8

/*
 * Set traffic LEDs
 *
 * @param ul_spc_data Bits 0-18 represent each of the 19 traffic LEDs. Bit 19 onwards does
 * not represent anything. If the bit is set then the corresponding LED will get turned on.
 **/
void vSetTraffic ( uint32_t ul_spc_data )
{
	// Clock has to start high.
	GPIO_SetBits(GPIOC, spc_clock);

	// Clear the traffic.
	prvClearTraffic();

	uint32_t ul_mask = 0x40000; // The mask starts at the 18th bit.

	uint8_t i;
	for(i = 0; i < 19; i++, ul_mask = ul_mask >> 1)
	{
		if ( ul_spc_data & ul_mask )
		{
			// Insert high.
			prvInsertHigh();

		}
		else
		{
			// Insert low.
			prvInsertLow();
		}
	}
}

/* Turns off all Traffic LEDs */
void prvClearTraffic ( void )
{
	GPIO_SetBits( GPIOC, spc_reset );
	GPIO_ResetBits( GPIOC, spc_reset );
	GPIO_SetBits( GPIOC, spc_reset );
}

/* Inserts a low value into the traffic flow. */
void prvInsertHigh ( void )
{
	GPIO_SetBits( GPIOC, spc_data );
	GPIO_ResetBits( GPIOC, spc_clock );
	GPIO_SetBits( GPIOC, spc_clock );
}

/* Inserts a low value into the traffic flow. */
void prvInsertLow ( void )
{
	GPIO_ResetBits( GPIOC, spc_data );
	GPIO_ResetBits( GPIOC, spc_clock );
	GPIO_SetBits( GPIOC, spc_clock );
}

