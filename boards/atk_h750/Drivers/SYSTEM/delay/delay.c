/**
 ****************************************************************************************************
 * @file        delay.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-12
 * @brief       ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���(֧��ucosii)
 *              �ṩdelay_init��ʼ�������� delay_us��delay_ms����ʱ����
 * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200312
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"

static uint16_t  g_fac_us = 0;      /* us��ʱ������ */


/* ���SYS_SUPPORT_OS������,˵��Ҫ֧��OS��(������UCOS) */
#if SYS_SUPPORT_OS

/* ���ӹ���ͷ�ļ� ( ucos��Ҫ�õ�) */
#include "includes.h"

/* ����g_fac_ms����, ��ʾms��ʱ�ı�����, ����ÿ�����ĵ�ms��, (����ʹ��os��ʱ��,��Ҫ�õ�) */
static uint16_t g_fac_ms = 0;


/*
 *  ��delay_us/delay_ms��Ҫ֧��OS��ʱ����Ҫ������OS��صĺ궨��ͺ�����֧��
 *  ������3���궨��:
 *      delay_osrunning    :���ڱ�ʾOS��ǰ�Ƿ���������,�Ծ����Ƿ����ʹ����غ���
 *      delay_ostickspersec:���ڱ�ʾOS�趨��ʱ�ӽ���,delay_init�����������������ʼ��systick
 *      delay_osintnesting :���ڱ�ʾOS�ж�Ƕ�׼���,��Ϊ�ж����治���Ե���,delay_msʹ�øò����������������
 *  Ȼ����3������:
 *      delay_osschedlock  :��������OS�������,��ֹ����
 *      delay_osschedunlock:���ڽ���OS�������,���¿�������
 *      delay_ostimedly    :����OS��ʱ,���������������.
 *
 *  �����̽���UCOSII��UCOSIII��֧��,����OS,�����вο�����ֲ
 */

/* ֧��UCOSII */
#ifdef  OS_CRITICAL_METHOD                      /* OS_CRITICAL_METHOD������,˵��Ҫ֧��UCOSII */
#define delay_osrunning     OSRunning           /* OS�Ƿ����б��,0,������;1,������ */
#define delay_ostickspersec OS_TICKS_PER_SEC    /* OSʱ�ӽ���,��ÿ����ȴ��� */
#define delay_osintnesting  OSIntNesting        /* �ж�Ƕ�׼���,���ж�Ƕ�״��� */
#endif

/* ֧��UCOSIII */
#ifdef  CPU_CFG_CRITICAL_METHOD                 /* CPU_CFG_CRITICAL_METHOD������,˵��Ҫ֧��UCOSIII */
#define delay_osrunning     OSRunning           /* OS�Ƿ����б��,0,������;1,������ */
#define delay_ostickspersec OSCfg_TickRate_Hz   /* OSʱ�ӽ���,��ÿ����ȴ��� */
#define delay_osintnesting  OSIntNestingCtr     /* �ж�Ƕ�׼���,���ж�Ƕ�״��� */
#endif


/**
 * @brief       us����ʱʱ,�ر��������(��ֹ���us���ӳ�)
 * @param       ��
 * @retval      ��
 */
static void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD  /* ʹ��UCOSIII */
    OS_ERR err;
    OSSchedLock(&err);          /* UCOSIII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ */
#else                           /* ����UCOSII */
    OSSchedLock();              /* UCOSII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ */
#endif
}

/**
 * @brief       us����ʱʱ,�ָ��������
 * @param       ��
 * @retval      ��
 */
static void delay_osschedunlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD  /* ʹ��UCOSIII */
    OS_ERR err;
    OSSchedUnlock(&err);        /* UCOSIII�ķ�ʽ,�ָ����� */
#else                           /* ����UCOSII */
    OSSchedUnlock();            /* UCOSII�ķ�ʽ,�ָ����� */
#endif
}

/**
 * @brief       us����ʱʱ,�ָ��������
 * @param       ticks: ��ʱ�Ľ�����
 * @retval      ��
 */
static void delay_ostimedly(uint32_t ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
    OS_ERR err;
    OSTimeDly(ticks, OS_OPT_TIME_PERIODIC, &err);   /* UCOSIII��ʱ��������ģʽ */
#else
    OSTimeDly(ticks);   /* UCOSII��ʱ */
#endif
}

/**
 * @brief       systick�жϷ�����,ʹ��OSʱ�õ�
 * @param       ticks: ��ʱ�Ľ�����
 * @retval      ��
 */
void SysTick_Handler(void)
{
    if (delay_osrunning == 1)   /* OS��ʼ����,��ִ�������ĵ��ȴ��� */
    {
        OSIntEnter();           /* �����ж� */
        OSTimeTick();           /* ����ucos��ʱ�ӷ������ */
        OSIntExit();            /* ���������л����ж� */
    }
}
#endif

/**
 * @brief       ��ʼ���ӳٺ���
 * @param       sysclk: ϵͳʱ��Ƶ��, ��CPUƵ��(rcc_c_ck), 480Mhz
 * @retval      ��
 */
void delay_init(uint16_t sysclk)
{
#if SYS_SUPPORT_OS              /* �����Ҫ֧��OS. */
    uint32_t reload;
#endif
    SysTick->CTRL |= 1 << 2;    /* SYSTICKʹ���ں�ʱ��Դ,ͬCPUͬƵ�� */
    g_fac_us = sysclk;          /* �����Ƿ�ʹ��OS,g_fac_us����Ҫʹ�� */
#if SYS_SUPPORT_OS              /* �����Ҫ֧��OS. */
    reload = sysclk;            /* ÿ���ӵļ������� ��λΪM */
    reload *= 1000000 / delay_ostickspersec;/* ����delay_ostickspersec�趨���ʱ��
                                             * reloadΪ24λ�Ĵ���,���ֵ:16777216,��480M��,Լ��0.035s����
                                             */
    g_fac_ms = 1000 / delay_ostickspersec;  /* ����OS������ʱ�����ٵ�λ */
    SysTick->CTRL |= 1 << 1;    /* ����SYSTICK�ж� */
    SysTick->LOAD = reload;     /* ÿ1/delay_ostickspersec���ж�һ�� */
    SysTick->CTRL |= 1 << 0;    /* ����SYSTICK */
#endif
}


#if SYS_SUPPORT_OS  /* �����Ҫ֧��OS, �����´��� */

/**
 * @brief       ��ʱnus
 * @param       nus: Ҫ��ʱ��us��.
 * @note        nusȡֵ��Χ: 0~8947848(���ֵ��2^32 / g_fac_us @g_fac_us = 480)
 * @retval      ��
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload;
    reload = SysTick->LOAD;     /* LOAD��ֵ */
    ticks = nus * g_fac_us;     /* ��Ҫ�Ľ����� */
    delay_osschedlock();        /* ��ֹOS���ȣ���ֹ���us��ʱ */
    told = SysTick->VAL;        /* �ս���ʱ�ļ�����ֵ */

    while (1)
    {
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;    /* ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����. */
            }
            else
            {
                tcnt += reload - tnow + told;
            }

            told = tnow;

            if (tcnt >= ticks) break;   /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�. */
        }
    };

    delay_osschedunlock();              /* �ָ�OS���� */
}

/**
 * @brief       ��ʱnms
 * @param       nms: Ҫ��ʱ��ms�� (0< nms <= 65535)
 * @retval      ��
 */
void delay_ms(uint16_t nms)
{
    if (delay_osrunning && delay_osintnesting == 0) /* ���OS�Ѿ�������,���Ҳ������ж�����(�ж����治���������) */
    {
        if (nms >= g_fac_ms)                /* ��ʱ��ʱ�����OS������ʱ������ */
        {
            delay_ostimedly(nms / g_fac_ms);/* OS��ʱ */
        }

        nms %= g_fac_ms;                    /* OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ */
    }

    delay_us((uint32_t)(nms * 1000));            /* ��ͨ��ʽ��ʱ */
}

#else  /* ��ʹ��OSʱ, �����´��� */

/**
 * @brief       ��ʱnus
 * @param       nus: Ҫ��ʱ��us��.
 * @note        ע��: nus��ֵ,��Ҫ����34952us(���ֵ��2^24 / g_fac_us @g_fac_us = 480)
 * @retval      ��
 */
void delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = nus * g_fac_us; /* ʱ����� */
    SysTick->VAL = 0x00;            /* ��ռ����� */
    SysTick->CTRL |= 1 << 0 ;       /* ��ʼ���� */

    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); /* CTRL.ENABLEλ����Ϊ1, ���ȴ�ʱ�䵽�� */

    SysTick->CTRL &= ~(1 << 0) ;    /* �ر�SYSTICK */
    SysTick->VAL = 0X00;            /* ��ռ����� */
}

/**
 * @brief       ��ʱnms
 * @param       nms: Ҫ��ʱ��ms�� (0< nms <= 65535)
 * @retval      ��
 */
void delay_ms(uint16_t nms)
{
    uint32_t repeat = nms / 30;     /*  ������30,�ǿ��ǵ������г�ƵӦ��,
                                     *  ����500Mhz��ʱ��, delay_us���ֻ����ʱ33554us������
                                     */
    uint32_t remain = nms % 30;

    while (repeat)
    {
        delay_us(30 * 1000);        /* ����delay_us ʵ�� 1000ms ��ʱ */
        repeat--;
    }

    if (remain)
    {
        delay_us(remain * 1000);    /* ����delay_us, ��β����ʱ(remain ms)������ */
    }
}

#endif



































