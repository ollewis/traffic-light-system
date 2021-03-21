/*
 * system_display.c
 *
 *  Created on: Mar 10, 2021
 *      Author: oliverlewis
 */

#include <stdio.h>

#include "system_display.h"
#include "../tls_utils/tls_utilities.h"
#include "../tls_utils/middleware.h"

#define LOW8_MASK 0xff
#define HIGH11_MASK 0x7ff00

void System_Display_Task( void *pvParameters )
{
	printf("The System Display Task has started.\n");

	extern xQueueHandle xQueue_generated_traffic;
	extern xQueueHandle xQueue_traffic_light;

	uint32_t ul_spc_data = 0x00000000; // Represents the traffic.
	uint8_t uc_generated_traffic = 0; // 1 = car, 0 = no car
	uint32_t accumulate_mask = 0; // Used for shifting traffic on non-green lights.

	uint8_t uc_traffic_state; // represents the traffic light state
	uc_traffic_state = light_green; // start green.
	vSetGreen();

	/* Initialize the display */
	vResetTrafficLights();

	for(;;)
	{

		// Get generated traffic.
		if ( xQueueReceive( xQueue_generated_traffic, &uc_generated_traffic, pdMS_TO_TICKS(250) ) == pdTRUE )
		{
			/*
			 * Update traffic state.
			 *
			 * If the light is green then shift all bits at once.
			 *
			 * If the light is not green then accumulate the first 8 bits
			 * and shift the rest.
			 * */
			if ( uc_traffic_state == light_green )
			{
				ul_spc_data = (ul_spc_data << 1) + uc_generated_traffic;
			}
			else
			{
				// Accumulate bits from 7 to 0
				accumulate_mask = 0x80; //0b10000000
				for (int i = 0; i < 7; i++, accumulate_mask = accumulate_mask >> 1 )
				{
					if ( ul_spc_data & accumulate_mask )
					{
						// If current bit is 1 do nothing.
						continue;
					}
					else if ( ul_spc_data & (accumulate_mask >> 1) )
					{
						// If next bit is 1 'shift' it over.
						ul_spc_data = ul_spc_data + accumulate_mask;
						ul_spc_data = ul_spc_data - (accumulate_mask >> 1);
					}
				}

				// Set bit 0 if there is new traffic and its not full.
				if ( !(ul_spc_data & 0x1) )
				{
					ul_spc_data = ul_spc_data + uc_generated_traffic;
				}

				// Shift bits 8 - 18+
				accumulate_mask = (ul_spc_data & HIGH11_MASK) << 1; // Gets shifted bits 8 to 18

				// Put together
				ul_spc_data = (accumulate_mask) + (ul_spc_data & LOW8_MASK);
			}

			// Update the traffic LEDs.
			vSetTraffic( ul_spc_data );
		}


		vTaskDelay( pdMS_TO_TICKS(2000) );

		// Get and set the traffic light state.
		if ( xQueueReceive( xQueue_traffic_light, &uc_traffic_state, pdMS_TO_TICKS(250)) )
		{
			if ( uc_traffic_state == light_red )
			{
				vSetRed();
			}
			else if ( uc_traffic_state == light_amber )
			{
				vSetAmber();
			}
			else
			{
				vSetGreen();
			}
		}

	}
}
