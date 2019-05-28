/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId tsk_PERIF_ResetHandle;
uint32_t tsk_PERIF_ResetBuffer[ 512 ];
osStaticThreadDef_t tsk_PERIF_ResetControlBlock;
osThreadId tsk_SPINand_wrkHandle;
uint32_t tskSpiNand_wrkrBuffer[ 1024 ];
osStaticThreadDef_t tskSpiNand_wrkrControlBlock;
osThreadId tsk_LED1_blinkHandle;
uint32_t tsk_LED1_blinkBuffer[ 256 ];
osStaticThreadDef_t tsk_LED1_blinkControlBlock;
osThreadId tsk_LED2_blinkHandle;
uint32_t tsk_LED2_blinkBuffer[ 256 ];
osStaticThreadDef_t tsk_LED2_blinkControlBlock;
osMessageQId msgto_SpiNand_wrkrHandle;
uint8_t msgto_SpiNand_wrkrBuffer[ 16 * sizeof( uint8_t ) ];
osStaticMessageQDef_t msgto_SpiNand_wrkrControlBlock;
osMessageQId msgto_LED_wrkrHandle;
osMessageQId msgto_LCD_wrkrHandle;
uint8_t msgto_LCD_wrkrBuffer[ 16 * sizeof( uint16_t ) ];
osStaticMessageQDef_t msgto_LCD_wrkrControlBlock;
osTimerId tmTestTimerHandle;
osStaticTimerDef_t tmTestTimerControlBlock;
osMutexId mt_SPI1_freeHandle;
osStaticMutexDef_t mt_SPI1_freeControlBlock;
osMutexId mt_SPI2_freeHandle;
osStaticMutexDef_t mt_SPI2_freeControlBlock;
osMutexId mt_LCD_freeHandle;
osStaticMutexDef_t mt_LCD_freeControlBlock;
osMutexId mt_UART1_freeHandle;
osStaticMutexDef_t mt_UART1_freeControlBlock;
osMutexId mt_UART2_freeHandle;
osStaticMutexDef_t mt_UART2_freeControlBlock;
osMutexId mt_NAND_FLASHHandle;
osStaticMutexDef_t mt_NAND_FLASHControlBlock;
osSemaphoreId sm_LCD_Bus_readyHandle;
osStaticSemaphoreDef_t sm_LCD_Bus_readyControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void vReset_Perifery_Device(void const * argument);
extern void vStart_SpiNand_wrkr(void const * argument);
extern void Start_tsk_LEDx_blink(void const * argument);
void tmTestTimer_clbck(void const * argument);

extern void MX_USB_DEVICE_Init(void);
extern void MX_FATFS_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 5 */
__weak void vApplicationMallocFailedHook(void)
{
   /* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created. It is also called by various parts of the
   demo application. If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
}
/* USER CODE END 5 */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer   = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of mt_SPI1_free */
  osMutexStaticDef(mt_SPI1_free, &mt_SPI1_freeControlBlock);
  mt_SPI1_freeHandle = osMutexCreate(osMutex(mt_SPI1_free));

  /* definition and creation of mt_SPI2_free */
  osMutexStaticDef(mt_SPI2_free, &mt_SPI2_freeControlBlock);
  mt_SPI2_freeHandle = osMutexCreate(osMutex(mt_SPI2_free));

  /* definition and creation of mt_LCD_free */
  osMutexStaticDef(mt_LCD_free, &mt_LCD_freeControlBlock);
  mt_LCD_freeHandle = osMutexCreate(osMutex(mt_LCD_free));

  /* definition and creation of mt_UART1_free */
  osMutexStaticDef(mt_UART1_free, &mt_UART1_freeControlBlock);
  mt_UART1_freeHandle = osMutexCreate(osMutex(mt_UART1_free));

  /* definition and creation of mt_UART2_free */
  osMutexStaticDef(mt_UART2_free, &mt_UART2_freeControlBlock);
  mt_UART2_freeHandle = osMutexCreate(osMutex(mt_UART2_free));

  /* definition and creation of mt_NAND_FLASH */
  osMutexStaticDef(mt_NAND_FLASH, &mt_NAND_FLASHControlBlock);
  mt_NAND_FLASHHandle = osMutexCreate(osMutex(mt_NAND_FLASH));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of sm_LCD_Bus_ready */
  osSemaphoreStaticDef(sm_LCD_Bus_ready, &sm_LCD_Bus_readyControlBlock);
  sm_LCD_Bus_readyHandle = osSemaphoreCreate(osSemaphore(sm_LCD_Bus_ready), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */

  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of tmTestTimer */
  osTimerStaticDef(tmTestTimer, tmTestTimer_clbck, &tmTestTimerControlBlock);
  tmTestTimerHandle = osTimerCreate(osTimer(tmTestTimer), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of msgto_SpiNand_wrkr */
  osMessageQStaticDef(msgto_SpiNand_wrkr, 16, uint8_t, msgto_SpiNand_wrkrBuffer, &msgto_SpiNand_wrkrControlBlock);
  msgto_SpiNand_wrkrHandle = osMessageCreate(osMessageQ(msgto_SpiNand_wrkr), NULL);

  /* definition and creation of msgto_LED_wrkr */
  osMessageQDef(msgto_LED_wrkr, 16, uint16_t);
  msgto_LED_wrkrHandle = osMessageCreate(osMessageQ(msgto_LED_wrkr), NULL);

  /* definition and creation of msgto_LCD_wrkr */
  osMessageQStaticDef(msgto_LCD_wrkr, 16, uint16_t, msgto_LCD_wrkrBuffer, &msgto_LCD_wrkrControlBlock);
  msgto_LCD_wrkrHandle = osMessageCreate(osMessageQ(msgto_LCD_wrkr), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of tsk_PERIF_Reset */
  osThreadStaticDef(tsk_PERIF_Reset, vReset_Perifery_Device, osPriorityAboveNormal, 0, 512, tsk_PERIF_ResetBuffer, &tsk_PERIF_ResetControlBlock);
  tsk_PERIF_ResetHandle = osThreadCreate(osThread(tsk_PERIF_Reset), NULL);

  /* definition and creation of tsk_SPINand_wrk */
  osThreadStaticDef(tsk_SPINand_wrk, vStart_SpiNand_wrkr, osPriorityNormal, 0, 1024, tskSpiNand_wrkrBuffer, &tskSpiNand_wrkrControlBlock);
  tsk_SPINand_wrkHandle = osThreadCreate(osThread(tsk_SPINand_wrk), NULL);

  /* definition and creation of tsk_LED1_blink */
  osThreadStaticDef(tsk_LED1_blink, Start_tsk_LEDx_blink, osPriorityNormal, 0, 256, tsk_LED1_blinkBuffer, &tsk_LED1_blinkControlBlock);
  tsk_LED1_blinkHandle = osThreadCreate(osThread(tsk_LED1_blink), NULL);

  /* definition and creation of tsk_LED2_blink */
  osThreadStaticDef(tsk_LED2_blink, Start_tsk_LEDx_blink, osPriorityNormal, 0, 256, tsk_LED2_blinkBuffer, &tsk_LED2_blinkControlBlock);
  tsk_LED2_blinkHandle = osThreadCreate(osThread(tsk_LED2_blink), (void*) 1);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_vReset_Perifery_Device */
/**
  * @brief  Function implementing the tsk_PERIF_Reset thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_vReset_Perifery_Device */
__weak void vReset_Perifery_Device(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* init code for FATFS */
  MX_FATFS_Init();

  /* USER CODE BEGIN vReset_Perifery_Device */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END vReset_Perifery_Device */
}

/* tmTestTimer_clbck function */
void tmTestTimer_clbck(void const * argument)
{
  /* USER CODE BEGIN tmTestTimer_clbck */

  /* USER CODE END tmTestTimer_clbck */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
