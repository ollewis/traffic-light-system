/*
 * traffic_light_statee.h
 *
 *  Created on: Mar 9, 2021
 *      Author: oliverlewis
 */

#ifndef TLS_STATE_H_
#define TLS_STATE_H_

#include "stm32f4xx.h"
#include "../../FreeRTOS_Source/include/FreeRTOS.h"
#include "../../FreeRTOS_Source/include/timers.h"
#include "../../FreeRTOS_Source/include/task.h"
#include "../../FreeRTOS_Source/include/queue.h"

void Traffic_Light_State_Task(void *pvParameters);

void prvUpdateLightDurations ( uint8_t traffic_flow );

void RedLightTimerCallback( TimerHandle_t xTimer );

void AmberLightTimerCallback( TimerHandle_t xTimer );

void GreenLightTimerCallback( TimerHandle_t xTimer );

#endif /* TLS_STATE_H_ */
