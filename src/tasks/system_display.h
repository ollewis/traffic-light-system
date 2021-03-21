/*
 * system_display.h
 *
 *  Created on: Mar 10, 2021
 *      Author: oliverlewis
 */

#ifndef TASKS_SYSTEM_DISPLAY_H_
#define TASKS_SYSTEM_DISPLAY_H_

#include "stm32f4xx.h"
#include "../../FreeRTOS_Source/include/FreeRTOS.h"
#include "../../FreeRTOS_Source/include/timers.h"
#include "../../FreeRTOS_Source/include/task.h"
#include "../../FreeRTOS_Source/include/queue.h"

void System_Display_Task( void *pvParameters );

#endif /* TASKS_SYSTEM_DISPLAY_H_ */
