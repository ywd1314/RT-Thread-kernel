#ifndef __RT_DEF_H__
#define __RT_DEF_H__

#include "rtconfig.h"
/*
*****************************************************************************
*                               ��������
*****************************************************************************
*/

/*RT-Thread �������������ض���*/
typedef signed    char                rt_int8_t;
typedef signed    short               rt_int16_t;
typedef signed    long                rt_int32_t;
typedef unsigned  char                rt_uint8_t;
typedef unsigned  short               rt_uint16_t;
typedef unsigned  long                rt_uint32_t;
typedef int                           rt_bool_t;

/* 32bit CPU */
typedef long                          rt_base_t;
typedef unsigned  long                rt_ubase_t;

typedef rt_base_t                     rt_err_t;
typedef rt_uint32_t                   rt_time_t;
typedef rt_uint32_t                   rt_tick_t;
typedef rt_base_t                     rt_flag_t;
typedef rt_ubase_t                    rt_size_t;
typedef rt_ubase_t                    rt_dev_t;
typedef rt_base_t                     rt_off_t;


#define RT_TIMER_SKIP_LIST_LEVEL      1
#define RT_TICK_MAX                   0xffffffff
#define RT_TIMER_SKIP_LIST_MASK       0x3


/* �������������ض��� */
#define RT_TRUE                       1
#define RT_FALSE                      0

#ifdef __CC_ARM
#define rt_inline                     static __inline
#define ALIGN(n)                      __attribute__((aligned(n)))

#elif defined (__IAR_SYSTEMS_ICC__)
#define rt_inline                     static inline
#define ALIGN(n)                      PRAGMA(data_alignment=n)

#elif defined (__GNUC__)
#define rt_inline                     static __inline
#define ALIGN(n)                      __attribute__((aligned(n)))
#else
#error not suported tool chain
#endif


#define RT_ALIGN(size, align)        (((size) + (align) - 1) & ~((align) - 1))
#define RT_ALIGN_DOWN(size, align)   ((size) & ~((align) - 1))

#define RT_NULL                      (0)

/* ��������ڵ��������� */
struct rt_list_node
{
	struct rt_list_node *next;
	struct rt_list_node *prev;
};
typedef struct rt_list_node rt_list_t;


/* RT-Thread �������ض��� */
#define RT_EOK                 0       /**< There is no error */
#define RT_ERROR               1       /**< A generic error happens */
#define RT_ETIMEOUT            2       /**< Timed out */
#define RT_EFULL               3       /**< The resource is full */
#define RT_EEMPTY              4       /**< The resource is empty */
#define RT_ENOMEM              5       /**< No memory */
#define RT_ENOSYS              6       /**< No system */
#define RT_EBUSY               7       /**< Busy */
#define RT_EIO                 8       /**< IO error */
#define RT_EINTR               9       /**< Interrupted system call */
#define RT_EINVAL              10      /**< Invalid argument */

/*
 *�߳�״̬����
 */
#define RT_THREAD_INIT                 0x00
#define RT_THREAD_READY                0x01
#define RT_THREAD_SUSPEND              0x02
#define RT_THREAD_RUNNING              0x03
#define RT_THREAD_BLOCK                RT_THREAD_SUSPEND
#define RT_THREAD_CLOSE                0x04
#define RT_THREAD_STAT_MASK            0x0f

#define RT_THREAD_SIGNAL               0x10
#define RT_THREAD_SIGNAL_READY         (RT_THREAD_SIGNAL | RT_THREAD_READY)        
#define RT_THREAD_SIGNAL_SUSPEND       0x20
#define RT_THREAD_STAT_SIGNAL_MASK     0xf0

/* ��������ö�ٶ��� */
enum rt_object_class_type
{
	RT_Object_Class_Thread = 0,        /* �������߳� */
	RT_Object_Class_Semaphrore,        /* �������ź��� */
	RT_Object_Class_Mutex,             /* �����ǻ����� */
	RT_Object_Class_Event,             /* �������¼� */
	RT_Object_Class_Mailbox,           /* ���������� */
	RT_Object_Class_MessageQueue,      /* ��������Ϣ���� */
	RT_Object_Class_MemHeap,           /* �������ڴ�� */
	RT_Object_Class_MemPool,           /* �������ڴ�� */
	RT_Object_Class_Device,            /* �������豸 */
	RT_Object_Class_Timer,             /* �����Ƕ�ʱ�� */
	RT_Object_Class_Module,            /* ������ģ�� */
	RT_Object_Class_Unknown,           /* ����δ֪ */
	RT_Object_Class_Static = 0x80      /* �����Ǿ�̬���� */
};

/* �����������Ͷ��� */
struct rt_object
{
	char name[RT_NAME_MAX];            /* �ں˶�������� */
	rt_uint8_t type;                   /* �ں˶�������� */
	rt_uint8_t flag;                   /* �ں˶����״̬ */
	
	rt_list_t  list;                   /* �ں˶�����б�ڵ� */
};
typedef struct rt_object *rt_object_t; /* �ں˶������������ض��� */

/* �ں˶�����Ϣ�ṹ�嶨�� */
struct rt_object_information{
	enum rt_object_class_type type;    /* �������� */
	rt_list_t object_list;             /* �����б�ڵ�ͷ */
	rt_size_t object_size;             /* �����С */
};

/* ��������������±궨�壬���������Ĵ�С */
enum rt_object_info_type
{
	RT_Object_Info_Thread = 0,      /* �������߳� */
#ifdef RT_USING_SEMAPHORE
	RT_Object_Info_Semaphore,       /* �������ź��� */
#endif
#ifdef RT_USING_MUTEX
	RT_Object_Info_Mutex,           /* �����ǻ����� */
#endif
#ifdef RT_USING_EVENT
	RT_Object_Info_Event,           /* �������¼� */
#endif
#ifdef RT_USING_MAILBOX
	RT_Object_Info_Mailbox,         /* ���������� */
#endif
#ifdef RT_USING_MESSAGEQUEUE
	RT_Object_Info_MessageQueue,    /* ��������Ϣ���� */
#endif
#ifdef RT_USING_MEMHEAP
	RT_Object_Info_MemHeap,         /* �������ڴ�� */
#endif
#ifdef RT_USING_MEMPOOL
	RT_Object_Info_MemPool,         /* �������ڴ�� */
#endif
#ifdef RT_USING_DEVICE
	RT_Object_Info_Device,          /* �������豸 */
#endif
	RT_Object_Info_Timer,           /* �����Ƕ�ʱ�� */
#ifdef RT_USING_MODULE
	RT_Object_Info_Module,          /* ������ģ�� */
#endif
	RT_Object_Info_Unknown,         /* ����δ֪ */
};

/* ��ʱ��״̬�궨�� */
#define RT_TIMER_FLAG_DEACTIVATED    0x0  /* ��ʱ��û�м��� */
#define RT_TIMER_FLAG_ACTIVATED      0x1  /* ��ʱ���Ѿ����� */
#define RT_TIMER_FLAG_ONE_SHOT       0x0  /* ���ζ�ʱ */
#define RT_TIMER_FLAG_PERIODIC       0x2  /* ���ڶ�ʱ */

#define RT_TIMER_FLAG_HARD_TIMER     0x0  /* Ӳ����ʱ������ʱ���ص�������tick isr�е��� */
#define RT_TIMER_FLAG_SOFT_TIMER     0x4  /* �����ʱ������ʱ���ص������ڶ�ʱ���߳��е��� */

/* ��ʱ����������궨�� */
#define RT_TIMER_CTRL_SET_TIME       0x0  /* ���ö�ʱ����ʱʱ�� */
#define RT_TIMER_CTRL_GET_TIME       0x1  /* ��ȡ��ʱ����ʱʱ�� */
#define RT_TIMER_CTRL_SET_ONESHOT    0x2  /* �޸Ķ�ʱ��Ϊһ�ζ�ʱ */
#define RT_TIMER_CTRL_SET_PERIODIC   0x3  /* �޸Ķ�ʱ��Ϊ���ڶ�ʱ */


/* ��ʱ���ṹ�� */
struct rt_timer
{
	struct rt_object parent;                    /* ��rt_object �̳� */
	
	rt_list_t row[RT_TIMER_SKIP_LIST_LEVEL];    /* �ڵ� */
	void (*timeout_func)(void *parameter);      /* ��ʱ���� */
	void *parameter;                            /* ��ʱ�����β� */
	
	rt_tick_t init_tick;                        /* ��ʱ��ʵ����Ҫ��ʱ��ʱ�� */
	rt_tick_t timeout_tick;                     /* ��ʱ��ʵ�ʳ�ʱʱ��ϵͳ������ */
};
typedef struct rt_timer *rt_timer_t;


/* �߳̿��ƿ��������� */
struct rt_thread
{
	/* rt���� */
	char name[RT_NAME_MAX];        /* ��������� */
	rt_uint8_t type;               /* ��������� */
	rt_uint8_t flags;              /* �����״̬ */
	rt_list_t  list;               /* ������б�ڵ� */
	
	rt_list_t tlist;               /* �߳�����ڵ� */
	
	void *sp;                      /* �߳�ջָ�� */
	void *entry;                   /* �߳���ڵ�ַ */
	void *parameter;               /* �߳��β� */
	void *stack_addr;              /* �߳�ջ��ʼ��ַ */
	rt_uint32_t stack_size;        /* �߳�ջ��С����λΪ�ֽ� */
	
	rt_ubase_t  init_tick;         /* ��ʼʱ��Ƭ */
	rt_ubase_t  remaining_tick;    /* ʣ��ʱ��Ƭ */
	
	rt_uint8_t  current_priority;  /* ��ǰ���ȼ� */
	rt_uint8_t  init_priority;     /* ��ʼ���ȼ� */
	rt_uint32_t number_mask;       /* ��ǰ���ȼ����� */
	
	rt_err_t    error;             /* ������ */
	rt_uint8_t  stat;              /* �̵߳�״̬ */
	
	struct rt_timer thread_timer;  /* ���õ��̶߳�ʱ�� */
};
typedef struct rt_thread *rt_thread_t;


#endif/* __RT_DEF_H__ */
