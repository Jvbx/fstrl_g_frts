/*
 * gkt_LCD.h
 *
 *  Created on: May 19, 2019
 *      Author: jvbx0
 */

#ifndef __GKT_LCD_H_
#define __GKT_LCD_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "main.h"




typedef struct {
     uint8_t pos_x;
     uint8_t pos_y;
     uint8_t is_data;
     char    msg[40];
} lcd_msg;




#endif /* __GKT_LCD_H_ */
