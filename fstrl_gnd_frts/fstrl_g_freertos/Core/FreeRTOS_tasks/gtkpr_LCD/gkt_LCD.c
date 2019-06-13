/*
 * gtkpr_LCD.c
 *
 *  Created on: May 19, 2019
 *      Author: jvbx0
 */

#include "gkt_LCD.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "LCD_1602A.h"




/* Header_Start_gtkpr_LCD */
/**
* @brief Function implementing the gkt_LCD thread.
* @param argument: Not used
* @retval None
*/
/* Header_vStart_gkt_LCD */
void vStart_gkt_LCD(const void * argument)
{
//extern osMutexId mt_LCD_freeHandle;
 extern osMessageQId msgto_gkt_LCDHandle;
 lcd_msg msgbuf;



  /* Infinite loop */
  for(;;)
  {
      if (uxQueueMessagesWaiting(msgto_gkt_LCDHandle))
          {
            xQueueReceive(msgto_gkt_LCDHandle, &msgbuf, portMAX_DELAY);
            if (msgbuf.is_data)
               {
               LCD1602A_SetCursorPos(msgbuf.pos_x, msgbuf.pos_y);
               LCD1602A_PrintStr(msgbuf.msg);
               }
          }
      taskYIELD();
  }

}
