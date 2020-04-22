#include "rtdef.h"


void rt_hw_context_switch_to(rt_uint32_t to);
void PendSV_Handler(void);
void rt_hw_context_switch(rt_uint32_t from, rt_uint32_t to);
rt_base_t rt_hw_interrupt_disable(void);
void rt_hw_interrupt_enable(rt_base_t level);

