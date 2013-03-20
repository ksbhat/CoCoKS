/*
 * stack_lib.c
 *
 *  Created on: Mar 15, 2013
 *      Author: sharan
 */
#include "stack_lib.h"

void push(node *nodeelement)
{
	struct stack *prev;
	if (top==NULL)
	{
		top=MEMmalloc(sizeof(struct stack));
		top->next=NULL;
	}
	else
	{
		prev=top;
		top=MEMmalloc(sizeof(struct stack));
		top->next=prev;
	}
	top->element=nodeelement;


}


node* pop()
{
	struct node* return_node;
	if(top!=NULL)
	{
		return_node=top->element;
		top=top->next;
	}
	return return_node;
}




