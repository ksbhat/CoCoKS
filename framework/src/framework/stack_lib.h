#ifndef _SAC_STACKLIB_H_
#define _SAC_STACKLIB_H_
#include "globals.h"
	struct stack
	{
		node *element;
		struct stack *next;
	};

	struct stack *top=NULL;
    extern void push(node *pushelement);
    extern node *pop();

#endif
