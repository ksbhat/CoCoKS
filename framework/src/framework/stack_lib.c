/*
 * stack_lib.c
 *
 *  Created on: Mar 15, 2013
 *      Author: sharan
 */
#include "stack_lib.h"
#include "memory.h"

bool push(STACK *stack, node *nodeelement)
{
	//node **prev;
	NODEELEMENT *newelement;

	if (NULL == stack)
	{
		return FALSE;
	}
	if (NULL == nodeelement)
	{
		return FALSE;
	}
	/*
	else
	{
		if (isempty(stack))
		{
			new = MEMmalloc(sizeof(struct stack));
			new->next = NULL;
		}
		prev=stack->top;
		stack->top)=MEMmalloc(sizeof(struct stack));
		stack->element = nodeelement;
		stack->top->next=prev;
	}*/

	newelement = MEMmalloc(sizeof(NODEELEMENT));
	newelement->nodeelement = nodeelement;
	if(isempty(stack))
	{
		newelement->next = NULL;
	}
	else
	{
		newelement->next = &(stack->element->nodeelement); // point to the current top.
	}

	stack->element = newelement;
	return TRUE;
}

bool isempty(STACK *stack)
{
	if (NULL == stack)
	{
		return TRUE;
	}
	/*
	else if (NULL == stack->top)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	*/
	if (NULL == stack->element) // the top guy
	{
		return TRUE;
	}
	if (NULL == stack->element->nodeelement) // there is some top guy who is empty!
	{
		return TRUE;
	}
	return FALSE;
}

node* pop(STACK *stack)
{
	node* return_node = NULL; // initialize so that when stack is empty, we return NULL.
	node *prevelement;
    STACK *prevtop;

	if (isempty(stack))
	{
		return NULL; // an error condition.
	}
	/*if((*stack)!=NULL) // if not empty.
	{
		return_node=(*stack)->top->element;
		prevtop = (*stack); // current head.
		stack = &((*stack)->next);
		MEMfree(prevtop);
	}*/
	else
	{
		return_node = stack->element->nodeelement;
		prevelement = stack->element;

		stack->element = *(stack->element->next);
		MEMfree(prevelement);
	}
	return return_node;
}




