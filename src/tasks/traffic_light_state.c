/*
 * traffic_light_statee.c
 *
 *  Created on: Mar 9, 2021
 *      Author: oliverlewis
 */

#include <stdio.h>
#include <math.h>

#include "traffic_light_state.h"
#include "../tls_utils/middleware.h"
#include "../tls_utils/tls_utilities.h"

#define AMBER_DURATION 3000
#define MAX_TIMER_LENGTH 20000 //15 seconds.
#define MIN_TIMER_LENGTH 3000

xTimerHandle xTimer_red = 0;
xTimerHandle xTimer_amber = 0;
xTimerHandle xTimer_green = 0;

BaseType_t x_red_timer_length = 5000;
BaseType_t x_green_timer_length = 5000;

uint8_t traffic_light_state;

/**
 * Task for managing traffic lights.
 */
void Traffic_Light_State_Task( void *pvParameters )
{
	printf("The Traffic Light State Task has been started.\n");

	extern xQueueHandle xQueue_traffic_flow;
	extern xQueueHandle xQueue_traffic_light;

	/* Create the traffic light timers. */
	xTimer_red = xTimerCreate( "RedLightTimer", pdMS_TO_TICKS(x_red_timer_length), pdFALSE, 0, RedLightTimerCallback);
	xTimer_amber = xTimerCreate( "AmberLightTimer", pdMS_TO_TICKS(AMBER_DURATION), pdFALSE, 0, AmberLightTimerCallback);
	xTimer_green = xTimerCreate( "GreenLightTimer", pdMS_TO_TICKS(x_green_timer_length), pdFALSE, 0, GreenLightTimerCallback);

	// Initialize the green light to start.
	traffic_light_state = light_green;
	xTimerStart(xTimer_green, pdMS_TO_TICKS(500) );

	uint8_t traffic_flow = 0;

	// Updates the traffic light state every 1 second.
	for (;;)
	{
		// Get the traffic flow.
		if ( xQueuePeek( xQueue_traffic_flow, &traffic_flow, pdMS_TO_TICKS(150) ) )
		{
			// Update the traffic light durations
			prvUpdateLightDurations( traffic_flow );
		}

		// Add the traffic light to the queue.
		if ( xQueueSend( xQueue_traffic_light, &traffic_light_state, pdMS_TO_TICKS(0) ) )
		{
			vTaskDelay( pdMS_TO_TICKS(1000) );
		}
		else
		{
			vTaskDelay( pdMS_TO_TICKS(50) );
		}

	}
}

void prvUpdateLightDurations ( uint8_t traffic_flow )
{
	x_green_timer_length = (BaseType_t)( MAX_TIMER_LENGTH * ((float)traffic_flow / 255.0));
	x_red_timer_length = MAX_TIMER_LENGTH - x_green_timer_length;

	if ( x_green_timer_length < MIN_TIMER_LENGTH )
		x_green_timer_length = MIN_TIMER_LENGTH;
	if ( x_red_timer_length < MIN_TIMER_LENGTH )
		x_red_timer_length = MIN_TIMER_LENGTH;
}

void RedLightTimerCallback( TimerHandle_t xTimer )
{
	traffic_light_state = light_green;
	xTimerChangePeriod( xTimer_green, pdMS_TO_TICKS(x_green_timer_length), pdMS_TO_TICKS(500) );
}

void AmberLightTimerCallback( TimerHandle_t xTimer )
{
	traffic_light_state = light_red;
	xTimerChangePeriod( xTimer_red, pdMS_TO_TICKS(x_red_timer_length), pdMS_TO_TICKS(500) );
}

void GreenLightTimerCallback( TimerHandle_t xTimer )
{
	traffic_light_state = light_amber;
	xTimerStart( xTimer_amber, pdMS_TO_TICKS(500) );
}
