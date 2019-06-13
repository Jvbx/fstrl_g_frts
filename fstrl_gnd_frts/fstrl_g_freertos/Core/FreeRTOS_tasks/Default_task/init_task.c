/*
 * SpiNand_wrkr.c
 *
 *  Created on: May 19, 2019
 *      Author: jvbx0
 */

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "dwt_stm32_delay.h"
#include "gd5f.h"
#include "lfs.h"
#include "LCD_1602A.h"
#include "usb_device.h"
#include "fatfs.h"
#include "init_task.h"

extern gd5f       spi_nand_gd5f;


/* USER CODE BEGIN Header_vStart_tsk_INIT_Periph */
/**
  * @brief  Function implementing the tsk_PERIF_Reset thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_vStart_tsk_INIT_Periph */
void vStart_tsk_INIT_Periph(const void * argument)
{
  osThreadSuspendAll();

/* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* init code for FATFS */
  MX_FATFS_Init();
  USB_PIN_CONNECT()


  /* USER CODE BEGIN StartDefaultTask */
  //инициализация функции микросекунных задержек
   DWT_Delay_Init();
  //инициализация дисплея
  LCD1602A_Init();
  LCD1602A_DisplayControl(ON, OFF, OFF);


   //инициализация spi-nand
   gd5f_init(&spi_nand_gd5f);
   gd5f_reset(&spi_nand_gd5f);
   gd5f_get_features(&spi_nand_gd5f);



   char outbuff[20] = {};

extern const struct lfs_config cfg;
lfs_t      lfs;
lfs_file_t file;

   volatile int32_t err = lfs_mount(&lfs, &cfg);
         if (err < 0) // если есть ошибка монтирования
         {
             // то произвести форматирование
             LCD1602A_SetCursorPos(0, 1);
             LCD1602A_PrintStr("Mount FAIL...");
             LCD1602A_SetCursorPos(1, 1);
             LCD1602A_PrintStr("Formatting...");
             err = lfs_format(&lfs, &cfg);
             err = lfs_mount(&lfs, &cfg);
         }
         LL_mDelay(1000);
         LCD1602A_SetCursorPos(0, 1);
         LCD1602A_PrintStr("                 ");
         LCD1602A_SetCursorPos(0, 1);
         LCD1602A_PrintStr("LFS Mount OK...");
         // прочитать текущее значение кол-ва загрузок
         uint32_t boot_count = 0;
         err = lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
         err = lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

         // обновить счетчик загрузок
         boot_count += 1;
         // перейти в начало файла
         err = lfs_file_rewind(&lfs, &file);
         // произвести запись
         err = lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

         // закрыть файл, окончательно производится дозапись на носитель
         err = lfs_file_close(&lfs, &file);

         // размонтировать файловую систему
         err = lfs_unmount(&lfs);

         // печать значения счетчика включений
         sprintf(outbuff, "boot_count: %ld", boot_count);
         LCD1602A_SetCursorPos(1, 1);
         LCD1602A_PrintStr("                 ");
         LCD1602A_SetCursorPos(1, 1);
         LCD1602A_PrintStr(outbuff);

         LL_mDelay(1000);
         LCD1602A_ClearScreen(); //Очистка дисплея от мусора
         LCD1602A_SetCursorPos(0, 1); //Установка курсора
         LCD1602A_PrintStr("Starting OS");
         LCD1602A_SetCursorPos(1, 0);
         for (uint8_t i = 0; i < 16; i++) {
             LCD1602A_PrintStr("*");
             LL_mDelay(40);
         }

         LCD1602A_ClearScreen();
         osThreadResumeAll();


  /* Infinite loop */
  for(;;)
  {
    osThreadTerminate(NULL);
  }
  /* USER CODE END StartDefaultTask */
}
