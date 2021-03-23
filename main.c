/***************************************************************************
                       World Peace Industrial

	All rights are reserved. Reproduction in whole or in parts is
	prohibited without the prior written consent of the copyright owner.
----------------------------------------------------------------------------
	
	Module: main.c
	Project: Examples For FreeRTOS 
	Function:
	Version: 1.0
	Date: 2020/10/29
	Team: SH MCU team
	Team members: Deric Yeh, Abel Hu, Jay Wang, Ryan Pan
	Software Development Tool: Keil MDK for ARM v5.31
	Platform: RuoGe i.MXRT1060_EVB
	
	Change Logs:
	Date           Author       Notes
	2020-10-29     Abel         the first version
    
****************************************************************************/
/*******************************************************************************
 * Header Files
 ******************************************************************************/
 
/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Module includes. */
#include "module.h"
#include "egtask.h"
#include "egqueue.h"
#include "egsemaphore.h"
#include "egevent.h"
#include "egstimer.h"
#include "egnotify.h"

#include "app_key.h"
#include "app_pit.h"
/************************************************
*                MACRO Definition
*************************************************/
 

/************************************************
*                Type definition
*************************************************/
/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)

/************************************************
*                Constant 
*************************************************/



/************************************************
*                Global Variables
*************************************************/
 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle;

static StackType_t  Start_Task_Stack[128];
static StaticTask_t Start_Task_TCB;

volatile uint32_t CPU_RunTime=0;
/************************************************
*                Function Prototypes
*************************************************/
 
static void hello_task(void *pvParameters);
void vAppTaskCreate(void);


/*******************************************************************************
Function Name : vSysInit
Purpose	      : 		
Date          :
Parameters    : NONE
Modifies      : NONE
Return        : NONE
Notes         :
*******************************************************************************/
void vSysInit(void)
{
    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
	  
	  vKEYInit();
	  vPIT_Init();
}

/*******************************************************************************
Function Name : main
Purpose	      : 		
Date          :
Parameters    : NONE
Modifies      : NONE
Return        : NONE
Notes         :
*******************************************************************************/
int main(void)
{	
    vSysInit();
		
//	  while(1)
//		{
//			PRINTF("KEY1 %d\n",GPIO_PinRead(KEY1PORT,KEY1PIN));
//			vDelay_MS(1000);
//			PRINTF("KEY2 %d\n",GPIO_PinRead(KEY1PORT,KEY1PIN));
//		}

    BaseType_t xReturn=xTaskCreate(	(TaskFunction_t) vAppTaskCreate,
																		(char * ) "vAppTaskCreate",
																		(uint32_t) 128,
																		(void * ) NULL,
																		(UBaseType_t) 6,
																		(TaskHandle_t *) &AppTaskCreate_Handle );
		if(xReturn==pdPASS)
			vTaskStartScheduler();	
	
    while (1);
}


/*******************************************************************************
Function Name : vAppTaskCreate
Purpose	      : 		
Date          :
Parameters    : NONE
Modifies      : NONE
Return        : NONE
Notes         :
*******************************************************************************/
void vAppTaskCreate(void)
{
	  /*  进入临界区*/
		taskENTER_CRITICAL();

#if mdTASK	
	  vEtTaskCreatStatic();
	  vEtTaskCreat();
#endif
	
#if mdQUEUE	
	  vEqQUEUECreat();
    vEqTaskCreat();
#endif

#if mdBINARY_SEMAPHORE
    vEsSEMAPHORECreat();
    vEBSTaskCreat();
#endif
	
#if mdCOUNTING_SEMAPHORE
    vEsSEMAPHORECreat();
    vECSTaskCreat();
#endif	

#if mdMUTEX_SEMAPHORE
    vEsSEMAPHORECreat();
    vEMSTaskCreat();
#endif

#if mdEVENT_GROUP
    vEeEVENTCreat();
    vEVTaskCreat();
#endif

#if mdSTMER
    vSTimerCreat();
#endif

#if mdNOTIFY
    vEnQueueTaskCreat();
#endif

	  vTaskDelete(AppTaskCreate_Handle);
	  /*  退出临界区*/
		taskEXIT_CRITICAL();	
}

