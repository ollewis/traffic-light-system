/*
 * traffic_flow.c
 *
 *  Created on: Mar 10, 2021
 *      Author: oliverlewis
 */

#include <stdio.h>
#include "traffic_flow.h"
#include "../tls_utils/middleware.h"

/**
 * Reads the value of the potentiometer and sends the traffic flow value to
 * other tasks.
 */
void Traffic_Flow_Adjustment_Task( void *pvParameters )
{
	printf("The Traffic Flow Adjustment Task has started.\n");

	extern xQueueHandle xQueue_traffic_flow;

	uint8_t traffic_flow = 0;

	for(;;)
	{
		// Get potentiometer value from the ADC.
		traffic_flow = xGetTrafficFlow();

		// Write it to the queue.
		xQueueOverwrite( xQueue_traffic_flow, &traffic_flow );
	}

}


