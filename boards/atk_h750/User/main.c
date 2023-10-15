/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       触摸屏 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 Mini Pro H750开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/MPU/mpu.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TOUCH/touch.h"
#include "./BSP/TIMER/btim.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"
#include "lv_demo_stress.h"

int main(void)
{
    sys_stm32_clock_init(240, 2, 2, 4);     /* 设置时钟, 480Mhz */
    delay_init(480);                        /* 延时初始化 */
    usart_init(120, 115200);                /* 串口初始化为115200 */
    mpu_memory_protection();                /* 保护相关存储区域 */
    led_init();                             /* 初始化LED */
    key_init();                             /* 初始化按键 */
	btim_timx_int_init(10 - 1, 24000 - 1);
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	
//	lv_obj_t* switch_obj = lv_switch_create(lv_scr_act());
//	lv_obj_set_size(switch_obj, 120, 60);
//	lv_obj_align(switch_obj, LV_ALIGN_CENTER, 0, 0);

	lv_demo_stress();

	while(1)
	{
		delay_ms(5);
		lv_timer_handler();
	}	
}

