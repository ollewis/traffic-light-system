/*
 * traffic_generator.c
 *
 *  Created on: Mar 10, 2021
 *      Author: oliverlewis
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "traffic_generator.h"
#include "../tls_utils/middleware.h"

static uint8_t prvTrafficGenerator ( uint8_t traffic_rate );

/**
 * Task for managing the traffic flow.
 */
void Traffic_Flow_Generator_Task( void *pvParameters)
{
	printf("The Traffic Flow Generator Task has been started.\n");

	extern xQueueHandle xQueue_traffic_flow;
	extern xQueueHandle xQueue_generated_traffic;

	uint8_t uc_traffic_rate = 0;
	uint8_t uc_generated_traffic = 0;

	for(;;)
	{

		// Generate traffic.
		if ( xQueuePeek( xQueue_traffic_flow, &uc_traffic_rate, pdMS_TO_TICKS(150) ) )
		{
			uc_generated_traffic = prvTrafficGenerator( uc_traffic_rate );
		}
		else
		{
			uc_generated_traffic = 0;
		}

		// Add the traffic to the queue.
		if ( xQueueSend( xQueue_generated_traffic, &uc_generated_traffic, pdMS_TO_TICKS(0) ) )
		{
			vTaskDelay( pdMS_TO_TICKS(500) );//
		}
		else
		{
			vTaskDelay( pdMS_TO_TICKS(50) );
		}

	}
}

uint8_t prvTrafficGenerator ( uint8_t traffic_rate )
{
	if (traffic_rate < 30) // Helps make traffic more consistent at a low rate.
		traffic_rate = 30;

	return (rand() % UINT8_MAX) > (UINT8_MAX - traffic_rate);
}

