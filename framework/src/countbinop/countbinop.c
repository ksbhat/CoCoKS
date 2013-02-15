/****************************************************
 Author: Koustubha Bhat
 UvA ID: 10513892
 Date  : 11-Feb-2013
 ****************************************************/

#include "types.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"
#include "memory.h"
#include "globals.h"

struct INFO
{
  bool firsterror;
  int num_add;
  int num_sub;
  int num_mul;
  int num_div;
  int num_mod;
};

#define INFO_FIRSTERROR(n) ((n)->firsterror)

static info *MakeInfo()
{
  info *result;

  result = MEMmalloc(sizeof(info));

  INFO_FIRSTERROR(result) = FALSE;

  result->num_add = 0;
  result->num_sub = 0;
  result->num_mul = 0;
  result->num_div = 0;
  result->num_mod = 0;
  
  return result;
}

static info *FreeInfo( info *info)
{
  info = MEMfree( info);

  return info;
}

node *CNTBINOPbinop (node * arg_node, info * arg_info)
{
   BINOP_LEFT( arg_node) = TRAVdo( BINOP_LEFT( arg_node), arg_info);

  switch (BINOP_OP( arg_node)) 
  {
    case BO_add:
      (arg_info->num_add)++; 
      break;
    case BO_sub:
      arg_info->num_sub++; 
      break;
    case BO_mul:
      arg_info->num_mul++; 
      break;
    case BO_div:
      (arg_info->num_div)++; 
      break;
    case BO_mod:
      (arg_info->num_mod)++; 
      break;
    default: ;
      break;
  }

   BINOP_RIGHT( arg_node) = TRAVdo( BINOP_RIGHT( arg_node), arg_info);
  
   return ( arg_node );
}

node *CNTBINOPdoCount( node *syntaxtree)
{
  info *info;

  DBUG_ENTER("CNTBINOPdoCount");

  printf( "\n\n------------------------------\n\n");

  info = MakeInfo();

  TRAVpush( TR_cntbinop);

  syntaxtree = TRAVdo( syntaxtree, info);

  TRAVpop();

  printf("Number of binary operators found respectively:\n");
  printf("+ :%3d\n- :%3d\n* :%3d\n/ :%3d\n%% :%3d\n", info->num_add, info->num_sub, info->num_mul, info->num_div, info->num_mod);

  FreeInfo(info);
 
  return( syntaxtree);
}
