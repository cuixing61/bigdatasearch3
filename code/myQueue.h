#include <stdlib.h>
#include <string.h>

#ifndef _MY_QEUE_H
#define _MY_QEUE_H

typedef struct _MY_QUEUE {
	void **data;
	int capacity, start, end;
}myQueue;

void queueInit(struct _MY_QUEUE *queue)//初始化 
{
	queue->capacity = 100;
	queue->start = queue->end = 0;
	queue->data = (void **)malloc(sizeof(void *) * queue->capacity);
}

void push(void *ptr, struct _MY_QUEUE *stack)//入队 
{
	if (stack->end == stack->capacity) {
		memmove(stack->data, stack->data + stack->start, sizeof(void *) * (stack->end - stack->start));
		stack->end -= stack->start;
		stack->start = 0;
		if (stack->end > stack->capacity * 0.8) {
			stack->capacity += 50;
			stack->data = (void**)realloc(stack->data, stack->capacity * sizeof(void *));
		}
	}
	stack->data[stack->end++] = ptr;
}

void *pop(struct _MY_QUEUE *stack)//出队 
{
	if (stack->start == stack->end)
		return NULL;
	return stack->data[stack->start++];
}

bool empty(struct _MY_QUEUE *stack)//判空 
{
	return stack->start == stack->end;
}

void queueClear(struct _MY_QUEUE *stack)
{
	free(stack->data);
}

#endif // !_MY_QEUE_H
