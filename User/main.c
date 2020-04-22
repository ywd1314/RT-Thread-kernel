/*
***********************************************************
*                     main()����
***********************************************************
*/
#include "rtthread.h"
#include "rthw.h"
#include "ARMCM3.h"


ALIGN(RT_ALIGN_SIZE)
/* �����߳�ջ */
rt_uint8_t rt_flag1_thread_stack[512];
rt_uint8_t rt_flag2_thread_stack[512];
rt_uint8_t rt_flag3_thread_stack[512];

/* �����߳̿��ƿ� */
struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;
struct rt_thread rt_flag3_thread;

rt_uint8_t flag1 = 0;
rt_uint8_t flag2 = 0;
rt_uint8_t flag3 = 0;

extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

/* �����ʱ */
void delay(uint32_t count)
{
	for(; count!=0; count--);
}

/* �߳�1 */
void flag1_thread_entry(void *p_arg)
{
	for(;;)
	{
		flag1 = 1;
		rt_thread_delay(3);
		flag1 = 0;
		rt_thread_delay(3);
	}
}

/* �߳�2 */
void flag2_thread_entry(void *p_arg)
{
	for(;;)
	{
		flag2 = 1;
//		rt_thread_delay(2);
		delay(100);
		flag2 = 0;
//		rt_thread_delay(2);
		delay(100);
	}
}

/* �߳�3 */
void flag3_thread_entry(void *p_arg)
{
	for(;;)
	{
		flag3 = 1;
//		rt_thread_delay(3);
		delay(100);
		flag3 = 0;
//		rt_thread_delay(3);
		delay(100);
	}
}

/* SysTick_Handler()�жϷ����� */
void SysTick_Handler(void)
{
	/* �����ж� */
	rt_interrupt_enter();
	/* ʱ������ */
	rt_tick_increase();
	
	/* �뿪�ж� */
	rt_interrupt_leave();
}

int main(void)
{
	/* Ӳ����ʼ�� */
	/* ��Ӳ����صĳ�ʼ��������������������棬��û����س�ʼ������ */
	
	/* ���ж� */
	rt_hw_interrupt_disable();
	
	/* SysTick�ж�Ƶ������ */
	SysTick_Config( SystemCoreClock / RT_TICK_PER_SECOND );  //RT_TICK_PER_SECOND��Ϊ100-->10ms
	
	/* ϵͳ��ʱ���б��ʼ�� */
	rt_system_timer_init();
	
	/* ��������ʼ�� */
	rt_system_scheduler_init();
	
	/* ��ʼ�������߳� */
	rt_thread_idle_init();
	
	/* ��ʼ������ */
	rt_thread_init( &rt_flag1_thread,
					"flag1",
					flag1_thread_entry,
					RT_NULL,
					&rt_flag1_thread_stack[0],
					sizeof(rt_flag1_thread_stack),
					2,
					4);
	/* ���̲߳�������б��� */
	rt_thread_startup(&rt_flag1_thread);
	
	/* ��ʼ������ */
	rt_thread_init( &rt_flag2_thread,
					"flag2",
					flag2_thread_entry,
					RT_NULL,
					&rt_flag2_thread_stack[0],
					sizeof(rt_flag2_thread_stack),
					3,
					2);
	/* ���̲߳�������б��� */
	rt_thread_startup(&rt_flag2_thread);
					
	/* ��ʼ������ */
	rt_thread_init( &rt_flag3_thread,
					"flag3",
					flag3_thread_entry,
					RT_NULL,
					&rt_flag3_thread_stack[0],
					sizeof(rt_flag3_thread_stack),
					3,
					3);
	/* ���̲߳�������б��� */
	rt_thread_startup(&rt_flag3_thread);
	
	/* ����ϵͳ������ */
	rt_system_scheduler_start();
	for(;;)
	{
		/*�޲���*/
	}
}
