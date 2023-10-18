/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       ������ ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� Mini Pro H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
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
    sys_stm32_clock_init(240, 2, 2, 4);     /* ����ʱ��, 480Mhz */
    delay_init(480);                        /* ��ʱ��ʼ�� */
    usart_init(120, 115200);                /* ���ڳ�ʼ��Ϊ115200 */
    mpu_memory_protection();                /* ������ش洢���� */
    led_init();                             /* ��ʼ��LED */
    key_init();                             /* ��ʼ������ */
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

