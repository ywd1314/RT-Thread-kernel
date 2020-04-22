/*
***********************************************************
*                     main()函数
***********************************************************
*/
#include "rtthread.h"
#include "rthw.h"
#include "ARMCM3.h"


ALIGN(RT_ALIGN_SIZE)
/* 定义线程栈 */
rt_uint8_t rt_flag1_thread_stack[512];
rt_uint8_t rt_flag2_thread_stack[512];
rt_uint8_t rt_flag3_thread_stack[512];

/* 定义线程控制块 */
struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;
struct rt_thread rt_flag3_thread;

rt_uint8_t flag1 = 0;
rt_uint8_t flag2 = 0;
rt_uint8_t flag3 = 0;

extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

/* 软件延时 */
void delay(uint32_t count)
{
	for(; count!=0; count--);
}

/* 线程1 */
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

/* 线程2 */
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

/* 线程3 */
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

/* SysTick_Handler()中断服务函数 */
void SysTick_Handler(void)
{
	/* 进入中断 */
	rt_interrupt_enter();
	/* 时基更新 */
	rt_tick_increase();
	
	/* 离开中断 */
	rt_interrupt_leave();
}

int main(void)
{
	/* 硬件初始化 */
	/* 将硬件相关的初始化放在这里，如果是软件仿真，则没有相关初始化代码 */
	
	/* 关中断 */
	rt_hw_interrupt_disable();
	
	/* SysTick中断频率设置 */
	SysTick_Config( SystemCoreClock / RT_TICK_PER_SECOND );  //RT_TICK_PER_SECOND设为100-->10ms
	
	/* 系统定时器列表初始化 */
	rt_system_timer_init();
	
	/* 调度器初始化 */
	rt_system_scheduler_init();
	
	/* 初始化空闲线程 */
	rt_thread_idle_init();
	
	/* 初始化进程 */
	rt_thread_init( &rt_flag1_thread,
					"flag1",
					flag1_thread_entry,
					RT_NULL,
					&rt_flag1_thread_stack[0],
					sizeof(rt_flag1_thread_stack),
					2,
					4);
	/* 将线程插入就绪列表中 */
	rt_thread_startup(&rt_flag1_thread);
	
	/* 初始化进程 */
	rt_thread_init( &rt_flag2_thread,
					"flag2",
					flag2_thread_entry,
					RT_NULL,
					&rt_flag2_thread_stack[0],
					sizeof(rt_flag2_thread_stack),
					3,
					2);
	/* 将线程插入就绪列表中 */
	rt_thread_startup(&rt_flag2_thread);
					
	/* 初始化进程 */
	rt_thread_init( &rt_flag3_thread,
					"flag3",
					flag3_thread_entry,
					RT_NULL,
					&rt_flag3_thread_stack[0],
					sizeof(rt_flag3_thread_stack),
					3,
					3);
	/* 将线程插入就绪列表中 */
	rt_thread_startup(&rt_flag3_thread);
	
	/* 启动系统调度器 */
	rt_system_scheduler_start();
	for(;;)
	{
		/*无操作*/
	}
}
