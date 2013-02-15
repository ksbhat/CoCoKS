/****************************************************
 Author: Koustubha Bhat
 UvA ID: 10513892
 Date  : 10-Feb-2013
 ****************************************************/

#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "str.h"

#define PREFIX_LITERAL "__"

node *PREFIXVARvar (node * arg_node, info * arg_info)
{
	VAR_NAME( arg_node) = STRcat(PREFIX_LITERAL, VAR_NAME( arg_node));
	return ( arg_node );
}

node *PREFIXVARvarlet (node * arg_node, info * arg_info)
{
	VARLET_NAME( arg_node) = STRcat(PREFIX_LITERAL, VARLET_NAME( arg_node));
	return ( arg_node );
}

node *PREFIXdoPrefix( node *syntaxtree)
{

  TRAVpush( TR_prefixvar);

  syntaxtree = TRAVdo( syntaxtree, NULL);

  TRAVpop();

  return( syntaxtree);
}
