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
#include "gkt_LCD.h"
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
osThreadId tsk_INIT_PeriphHandle;
uint32_t tsk_INIT_PeriphBuffer[ 256 ];
osStaticThreadDef_t tsk_INIT_PeriphControlBlock;
osThreadId tsk_SPINand_ioHandle;
uint32_t tsk_SPINand_ioBuffer[ 1024 ];
osStaticThreadDef_t tsk_SPINand_ioControlBlock;
osThreadId tsk_LED1_blinkHandle;
uint32_t tsk_LED1_blinkBuffer[ 256 ];
osStaticThreadDef_t tsk_LED1_blinkControlBlock;
osThreadId tsk_LED2_blinkHandle;
uint32_t tsk_LED2_blinkBuffer[ 256 ];
osStaticThreadDef_t tsk_LED2_blinkControlBlock;
osThreadId gkt_LCDHandle;
uint32_t gkt_LCDBuffer[ 256 ];
osStaticThreadDef_t gkt_LCDControlBlock;
osMessageQId msgto_tsk_SpiNand_ioHandle;
uint8_t msgto_SpiNand_wrkrBuffer[ 16 * sizeof( uint8_t ) ];
osStaticMessageQDef_t msgto_SpiNand_wrkrControlBlock;
osMessageQId msgto_gkt_LCDHandle;
uint8_t msgto_gkt_LCDBuffer[ 16 * sizeof( lcd_msg ) ];
osStaticMessageQDef_t msgto_gkt_LCDControlBlock;
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

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void vStart_tsk_INIT_Periph(const void * argument);
void vStart_tsk_SPINand_io(const void * argument);
void vStart_tsk_LEDx_blink(const void * argument);
void vStart_gkt_LCD(const void * argument);
void tmTestTimer_clbck(const void * argument);

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
  /* definition and creation of msgto_tsk_SpiNand_io */
  osMessageQStaticDef(msgto_tsk_SpiNand_io, 16, uint8_t, msgto_SpiNand_wrkrBuffer, &msgto_SpiNand_wrkrControlBlock);
  msgto_tsk_SpiNand_ioHandle = osMessageCreate(osMessageQ(msgto_tsk_SpiNand_io), NULL);

  /* definition and creation of msgto_gkt_LCD */
  osMessageQStaticDef(msgto_gkt_LCD, 16, lcd_msg, msgto_gkt_LCDBuffer, &msgto_gkt_LCDControlBlock);
  msgto_gkt_LCDHandle = osMessageCreate(osMessageQ(msgto_gkt_LCD), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of tsk_INIT_Periph */
  osThreadStaticDef(tsk_INIT_Periph, vStart_tsk_INIT_Periph, osPriorityAboveNormal, 0, 256, tsk_INIT_PeriphBuffer, &tsk_INIT_PeriphControlBlock);
  tsk_INIT_PeriphHandle = osThreadCreate(osThread(tsk_INIT_Periph), NULL);

  /* definition and creation of tsk_SPINand_io */
  osThreadStaticDef(tsk_SPINand_io, vStart_tsk_SPINand_io, osPriorityNormal, 0, 1024, tsk_SPINand_ioBuffer, &tsk_SPINand_ioControlBlock);
  tsk_SPINand_ioHandle = osThreadCreate(osThread(tsk_SPINand_io), NULL);

  /* definition and creation of tsk_LED1_blink */
  osThreadStaticDef(tsk_LED1_blink, vStart_tsk_LEDx_blink, osPriorityNormal, 0, 256, tsk_LED1_blinkBuffer, &tsk_LED1_blinkControlBlock);
  tsk_LED1_blinkHandle = osThreadCreate(osThread(tsk_LED1_blink), NULL);

  /* definition and creation of tsk_LED2_blink */
  osThreadStaticDef(tsk_LED2_blink, vStart_tsk_LEDx_blink, osPriorityNormal, 0, 256, tsk_LED2_blinkBuffer, &tsk_LED2_blinkControlBlock);
  tsk_LED2_blinkHandle = osThreadCreate(osThread(tsk_LED2_blink), (void*) 1);

  /* definition and creation of gkt_LCD */
  osThreadStaticDef(gkt_LCD, vStart_gkt_LCD, osPriorityNormal, 0, 256, gkt_LCDBuffer, &gkt_LCDControlBlock);
  gkt_LCDHandle = osThreadCreate(osThread(gkt_LCD), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_vStart_tsk_INIT_Periph */
/**
  * @brief  Function implementing the tsk_INIT_Periph thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_vStart_tsk_INIT_Periph */
__weak void vStart_tsk_INIT_Periph(const void * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* init code for FATFS */
  MX_FATFS_Init();

  /* USER CODE BEGIN vStart_tsk_INIT_Periph */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END vStart_tsk_INIT_Periph */
}

/* USER CODE BEGIN Header_vStart_tsk_SPINand_io */
/**
* @brief Function implementing the tsk_SPINand_io thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vStart_tsk_SPINand_io */
__weak void vStart_tsk_SPINand_io(const void * argument)
{
  /* USER CODE BEGIN vStart_tsk_SPINand_io */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END vStart_tsk_SPINand_io */
}

/* USER CODE BEGIN Header_vStart_tsk_LEDx_blink */
/**
* @brief Function implementing the tsk_LED1_blink thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vStart_tsk_LEDx_blink */
__weak void vStart_tsk_LEDx_blink(const void * argument)
{
  /* USER CODE BEGIN vStart_tsk_LEDx_blink */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END vStart_tsk_LEDx_blink */
}

/* USER CODE BEGIN Header_vStart_gkt_LCD */
/**
* @brief Function implementing the gkt_LCD thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vStart_gkt_LCD */
__weak void vStart_gkt_LCD(const void * argument)
{
  /* USER CODE BEGIN vStart_gkt_LCD */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END vStart_gkt_LCD */
}

/* tmTestTimer_clbck function */
void tmTestTimer_clbck(const void * argument)
{
  /* USER CODE BEGIN tmTestTimer_clbck */

  /* USER CODE END tmTestTimer_clbck */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
