#ifndef _SAC_STACKLIB_H_
#define _SAC_STACKLIB_H_
#include "globals.h"

typedef struct nodeelement
{	
	node *nodeelement;
	node **next;
}NODEELEMENT;

typedef struct stack
{
    NODEELEMENT *element;
}STACK;

extern bool push(STACK *stack, node *pushelement);
extern node *pop(STACK *stack);
extern bool isempty(STACK *stack);

#endif
