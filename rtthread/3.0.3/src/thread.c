#include "rtthread.h"
#include "rthw.h"

/* �߳���صĺ���������ļ��ж��� */
extern rt_uint8_t *rt_hw_stack_init(void *tentry,
							 void *parameter,
							 rt_uint8_t *stack_addr);
extern void rt_object_init(struct rt_object *object,
					enum rt_object_class_type type,
					const char *name);
					
extern struct rt_thread *rt_current_thread;
extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];
extern rt_uint32_t rt_thread_ready_priority_group;
					
void rt_thread_timeout(void *parameter);

rt_err_t rt_thread_init(struct rt_thread *thread,
						const char *name,
						void (*entry)(void *parameter),
						void *parameter,
						void *stack_start,
						rt_uint32_t stack_size,
						rt_uint8_t  priority,
						rt_uint32_t tick)
{
	/* �̶߳����ʼ�� */
	/* �߳̽ṹ�忪ͷ���ֵ�4����Ա����rt_object_t��Ա */
	rt_object_init((rt_object_t)thread, RT_Object_Class_Thread, name);
	
	rt_list_init(&(thread->tlist));
	
	thread->entry = (void *)entry;
	thread->parameter = parameter;
	
	thread->stack_addr = stack_start;
	thread->stack_size = stack_size;
	
	/* ��ʼ���߳�ջ���������߳�ջָ�� */
	thread->sp = 
	(void *)rt_hw_stack_init( thread->entry,
							  thread->parameter,
	(void *)((char *)thread->stack_addr + thread->stack_size - 4)
	);
	
	thread->init_priority = priority;
	thread->current_priority = priority;
	thread->number_mask = 0;
	
	/* �������״̬ */
	thread->error = RT_EOK;
	thread->stat = RT_THREAD_INIT;
	
	/* ʱ��Ƭ��� */
	thread->init_tick = tick;
	thread->remaining_tick = tick;
	
	/* ��ʼ���̶߳�ʱ�� */
	rt_timer_init(  &(thread->thread_timer),
					thread->name,
					rt_thread_timeout,
					thread,
					0,
					RT_TIMER_FLAG_ONE_SHOT);
	
	return RT_EOK;
}

/**
 *�ú������ڹ���ָ�����߳�
 *
 *@param thread Ҫ��������߳�
 *
 *@return ����״̬��RT_EOK on OK,-RT_ERROR on error
 *
 *@note �����������߳��������ڵ��øú����󣬱������rt_schedule()����ϵͳ����
 *
 */
rt_err_t rt_thread_suspend(rt_thread_t thread)
{
	register rt_base_t temp;
	
	/* ֻ�о������̲߳��ܺñ����𣬷����˳������ش����� */
	if((thread->stat & RT_THREAD_STAT_MASK) != RT_THREAD_READY)
	{
		return -RT_ERROR;
	}
	
	/* ���ж� */
	temp = rt_hw_interrupt_disable();
	
	/* �ı��߳�״̬ */
	thread->stat = RT_THREAD_SUSPEND;
	/* ���̴߳Ӿ����б���ɾ�� */
	rt_schedule_remove_thread(thread);
	
	/* ֹͣ�̶߳�ʱ�� */
	rt_timer_stop(&(thread->thread_timer));
	
	/* ���ж� */
	rt_hw_interrupt_enable(temp);
	
	return RT_EOK;
}


/**
 *�ú������õ�ǰ�߳�����һ��ʱ�䣬��λΪtick
 *
 *@param tick ����ʱ�䣬��λΪtick
 *
 *@return RT_EOK
 */
rt_err_t rt_thread_sleep(rt_tick_t tick)
{
	register rt_base_t temp;
	struct rt_thread *thread;
	
	/* ���ж� */
	temp = rt_hw_interrupt_disable();
	
	/* ��ȡ��ǰ�̵߳��߳̿��ƿ� */
	thread = rt_current_thread;
	
	/* �����߳� */
	rt_thread_suspend(thread);
	
	/* �����̳߳�ʱʱ�� */
	rt_timer_control(&(thread->thread_timer), RT_TIMER_CTRL_SET_TIME, &tick);
	
	/* ������ʱ�� */
	rt_timer_start(&(thread->thread_timer));
	
	/* ���ж� */
	rt_hw_interrupt_enable(temp);
	
	/* ִ��ϵͳ���� */
	rt_schedule();
	
	return RT_EOK;
}


/* ������ʱ���� */
rt_err_t rt_thread_delay(rt_tick_t tick)
{
	return rt_thread_sleep(tick);
}


/**
 *�ú������ڻָ�һ���̣߳�Ȼ����ŵ������б�
 *
 *@param thread ��Ҫ���ָ����߳�
 *
 *@return ����״̬��RT_EOK on OK, -RT_ERROR on error
 */
rt_err_t rt_thread_resume(rt_thread_t thread)
{
	register rt_base_t temp;
	
	/* ��Ҫ�ָ����̱߳���Ϊ����״̬�����򷵻ش����� */
	if((thread->stat & RT_THREAD_STAT_MASK) != RT_THREAD_SUSPEND)
	{
		return -RT_ERROR;
	}
	
	/* ���ж� */
	temp = rt_hw_interrupt_disable();
	
	/* �ӹ�������Ƴ� */
	rt_list_remove(&(thread->tlist));
	
	/* ���ж� */
	rt_hw_interrupt_enable(temp);
	
	/* ��������б� */
	rt_schedule_insert_thread(thread);
	
	return RT_EOK;
}

rt_thread_t rt_thread_self(void)
{
	return rt_current_thread;
}

/**
 *����һ���̲߳�����ŵ�ϵͳ�ľ����б���
 *
 *@param thread ���������߳�
 *
 *@return ����״̬��RT_EOK on OK, -RT_ERROR on error
 */
rt_err_t rt_thread_startup(rt_thread_t thread)
{
	/* ���õ�ǰ���ȼ�Ϊ��ʼ���ȼ� */
	thread->current_priority = thread->init_priority;
	thread->number_mask = 1UL << thread->current_priority;
	
	/* �ı��̵߳�״̬Ϊ����״̬ */
	thread->stat = RT_THREAD_SUSPEND;
	/* Ȼ��ָ��߳� */
	rt_thread_resume(thread);
	
	if(rt_thread_self() != RT_NULL)
	{
		/* ϵͳ���� */
		rt_schedule();
	}
	
	return RT_EOK;
}

/**
 *�̳߳�ʱ����
 *���߳���ʱ���ڻ��ߵȴ�����Դ���û��߳�ʱʱ���ú����ᱻ����
 *
 *@param parameter ��ʱ�������β�
 */
void rt_thread_timeout(void *parameter)
{
	struct rt_thread *thread;
	
	thread = (struct rt_thread *)parameter;
	
	/* ���ô�����Ϊ��ʱ */
	thread->error = -RT_ETIMEOUT;
	
	/* ���̴߳ӹ����б��лָ� */
	rt_list_remove(&(thread->tlist));
	
	/* ���̲߳�������б� */
	rt_schedule_insert_thread(thread);
	
	/* ϵͳ���� */
	rt_schedule();
}

/**
 *
 *�ú�����ʹ��ǰ�߳��ó���������������ѡ��������ȼ����߳�ִ��
 *
 *��ǰ�̻߳��Ǵ��ھ���̬
 *
 *@return RT_EOK
 */
rt_err_t rt_thread_yield(void)
{
	register rt_base_t level;
	struct rt_thread *thread;
	
	/* ���ж� */
	level = rt_hw_interrupt_disable();
	
	/* ��ȡ��ǰ�̵߳��߳̿��ƿ� */
	thread = rt_current_thread;
	
	/* ����̴߳��ھ���̬����ͬһ�����ȼ��²�ֹһ���߳� */
	if((thread->stat & RT_THREAD_STAT_MASK) == RT_THREAD_READY && thread->tlist.next != thread->tlist.prev)
	{
		/* ��ʱ��Ƭ������̴߳Ӿ����б����Ƴ� */
		rt_list_remove(&(thread->tlist));
		
		/* ���̲߳�������ȼ��µ�����β�� */
		rt_list_insert_before(&(rt_thread_priority_table[thread->current_priority]), &(thread->tlist));
		
		/* ���ж� */
		rt_hw_interrupt_enable(level);
		
		/* ִ�е��� */
		rt_schedule();
		
		return RT_EOK;
	}
	
	/* ���ж� */
	rt_hw_interrupt_enable(level);
	
	return RT_EOK;
}