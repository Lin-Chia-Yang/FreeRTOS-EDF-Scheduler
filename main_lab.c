/*
 * main_lab.c
 *
 *  Created on: Jun 7, 2023
 *      Author: rtos
 */

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>
#include <string.h>
#include "timers.h"
/* Standard demo includes. */
#include "partest.h"

#define TASK1_COMPUTATION_TIME 1
#define TASK1_PERIOD 4
#define TASK2_COMPUTATION_TIME 2
#define TASK2_PERIOD 5
#define TASK3_COMPUTATION_TIME 2
#define TASK3_PERIOD 10
/*
 * Called by main when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 1 in
 * main.c.
 */
TaskHandle_t xT1;
TaskHandle_t xT2;
TaskHandle_t xT3;
struct parameter{
    TickType_t deadline;
    TickType_t exectime;
};
void main_lab( void );
void vT1(void *pvParameters);
void vT2(void *pvParameters);
void vT3(void *pvParameters);
void printtask( void );
tskTCB tcb1;
tskTCB tcb2;
tskTCB tcb3;
void T1(void *pvParameters){
    TickType_t start;
    TickType_t rem;
    struct parameter *params = pvParameters;
    vTaskSetTCBdeadline(xT1, params->deadline);
    vTaskSetTCBexectime(xT1, params->exectime);
    vTaskSetTCBperiod(xT1, params->deadline);
    TaskStatus_t xTaskDetails1;
    vTaskGetInfo(xT1, &xTaskDetails1, pdTRUE, eInvalid);
    TickType_t deadline = xTaskDetails1.deadline;
    TickType_t exectime = xTaskDetails1.exectime;
    start = 0;
    while(1){
        while(xTaskDetails1.exectime > 0){
            vTaskGetInfo(xT1, &xTaskDetails1, pdTRUE, eInvalid);
        }
        TickType_t end = xTaskGetTickCount();
        start = start + (params->deadline);
        vTaskSetTCBdeadline(xT1, start + (params->deadline));
        vTaskSetTCBexectime(xT1, params->exectime);
        vTaskGetInfo(xT1, &xTaskDetails1, pdTRUE, eInvalid);
        rem = (start - end);
        vTaskDelay(rem);
    }
}
void T2(void *pvParameters){
    TickType_t start;
    TickType_t rem;
    struct parameter *params = pvParameters;
    vTaskSetTCBdeadline(xT2, params->deadline);
    vTaskSetTCBexectime(xT2, params->exectime);
    vTaskSetTCBperiod(xT2, params->deadline);
    TaskStatus_t xTaskDetails2;
    vTaskGetInfo(xT2, &xTaskDetails2, pdTRUE, eInvalid);
    start = 0;
    while(1){
        while( xTaskDetails2.exectime > 0){
            vTaskGetInfo(xT2, &xTaskDetails2, pdTRUE, eInvalid);
        }
        vTaskSetTCBexectime(xT2, params->exectime);
        vTaskGetInfo(xT2, &xTaskDetails2, pdTRUE, eInvalid);
        start = start + (params->deadline);
        vTaskSetTCBdeadline(xT2, start + (params->deadline));
        TickType_t end = xTaskGetTickCount();
        rem = (start - end);
        vTaskDelay(rem);
        printtask();
    }
}
void T3(void *pvParameters){
    TickType_t start;
    TickType_t rem;
    struct parameter *params = pvParameters;
    vTaskSetTCBdeadline(xT3, params->deadline);
    vTaskSetTCBexectime(xT3, params->exectime);
    vTaskSetTCBperiod(xT3, params->deadline);
    TaskStatus_t xTaskDetails3;
    vTaskGetInfo(xT3, &xTaskDetails3, pdTRUE, eInvalid);
    start = 0;
    while(1){
        while( xTaskDetails3.exectime > 0){
            vTaskGetInfo(xT3, &xTaskDetails3, pdTRUE, eInvalid);
        }
        TickType_t end = xTaskGetTickCount();
        start = start + (params->deadline);
        vTaskSetTCBdeadline(xT3, start + (params->deadline));
        vTaskSetTCBexectime(xT3, params->exectime);
        vTaskGetInfo(xT3, &xTaskDetails3, pdTRUE, eInvalid);
        rem = (start - end);
        vTaskDelay(rem);
//        printtask();
    }
}
void printtask( void ){
    taskENTER_CRITICAL();
    UBaseType_t i;
    if(log_idx > 0){
        for(i=0; i<log_idx; i++){
            printf("%d ", task_time[i]);
            if(task_status[i] == 0){
                printf("complete ");
            }
            else{
                printf("preempt ");
            }
            printf("%s ", previous_task[i]);
            printf("%s\n", next_task[i]);
        }
    }
    log_idx = 0;
    taskEXIT_CRITICAL();
}
void main_lab( void ){
//    printf("T1 c:%d p:%d\n", TASK1_COMPUTATION_TIME, TASK1_PERIOD);
//    printf("T2 c:%d p:%d\n", TASK2_COMPUTATION_TIME, TASK2_PERIOD);
//    printf("T3 c:%d p:%d\n", TASK3_COMPUTATION_TIME, TASK3_PERIOD);
    BaseType_t xReturnedT1;
    BaseType_t xReturnedT2;
    BaseType_t xReturnedT3;
    struct parameter p1;
    p1.deadline = TASK1_PERIOD;
    p1.exectime = TASK1_COMPUTATION_TIME;
    struct parameter p2;
    p2.deadline = TASK2_PERIOD;
    p2.exectime = TASK2_COMPUTATION_TIME;
    struct parameter p3;
    p3.deadline = TASK3_PERIOD;
    p3.exectime = TASK3_COMPUTATION_TIME;
    xReturnedT2 = xTaskCreate(T2, "T2", configMINIMAL_STACK_SIZE, (void*)&p2, 1, &xT2);
    xReturnedT3 = xTaskCreate(T3, "T3", configMINIMAL_STACK_SIZE, (void*)&p3, 1, &xT3);
    xReturnedT1 = xTaskCreate(T1, "T1", configMINIMAL_STACK_SIZE, (void*)&p1, 1, &xT1);
    vTaskSetTCBdeadline(xT1, TASK1_PERIOD);
    vTaskSetTCBdeadline(xT2, TASK2_PERIOD);
    vTaskSetTCBdeadline(xT3, TASK3_PERIOD);
    vTaskSetTCBcomptime(xT1, TASK1_COMPUTATION_TIME);
    vTaskSetTCBcomptime(xT2, TASK2_COMPUTATION_TIME);
    vTaskSetTCBcomptime(xT3, TASK3_COMPUTATION_TIME);
    vTaskStartScheduler();
    for(;;);
}

