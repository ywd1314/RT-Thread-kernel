#include "rtdef.h"

rt_inline void rt_list_init(rt_list_t *l)
{
	l->next = l->prev = l;
}

/* ��˫�������ͷ�������һ���ڵ� */
rt_inline void rt_list_insert_after(rt_list_t *l, rt_list_t *n)
{
	l->next->prev = n;
	n->next = l->next;
	
	l->next = n;
	n->prev = l;
}

/* ��˫�������ͷǰ�����һ���ڵ� */
rt_inline void rt_list_insert_before(rt_list_t *l, rt_list_t *n)
{
	l->prev->next = n;
	n->prev = l->prev;
	
	l->prev = n;
	n->next = l;
}

/* ��˫��������ɾ��һ���ڵ� */
rt_inline void rt_list_remove(rt_list_t *n)
{
	n->next->prev = n->prev;
	n->prev->next = n->next;
	
	n->next = n->prev = n;
}

/**
 * @brief �ж��б��Ƿ�Ϊ��
 * @param l ��Ҫ�жϵ�list
 */
rt_inline int rt_list_isempty(const rt_list_t *l)
{
    return l->next == l;
}


