/*
 * traffic_flow.h
 *
 *  Created on: Mar 10, 2021
 *      Author: oliverlewis
 */

#ifndef TASKS_TRAFFIC_FLOW_H_
#define TASKS_TRAFFIC_FLOW_H_

#include "stm32f4xx.h"
#include "../../FreeRTOS_Source/include/FreeRTOS.h"
#include "../../FreeRTOS_Source/include/timers.h"
#include "../../FreeRTOS_Source/include/task.h"
#include "../../FreeRTOS_Source/include/queue.h"

void Traffic_Flow_Adjustment_Task( void *pvParameters );

#endif /* TASKS_TRAFFIC_FLOW_H_ */
