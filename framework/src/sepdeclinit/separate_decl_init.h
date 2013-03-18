
/**
 * @file print.h
 *
 * Functions to print node structures
 * 
 */

#ifndef _SAC_SEP_NODE_H_
#define _SAC_SEP_NODE_H_

#include "types.h"

extern node *SEPprogram(node *arg_node, info * arg_info);
extern node *SEPvardeclistlocalfundef (node *arg_node, info * arg_info);
extern node *SEPusualdef (node *arg_node, info * arg_info);
extern node *SEParraydef (node *arg_node, info * arg_info);
extern node *SEPvardec (node *arg_node, info * arg_info);
extern node *SEPparamlist (node *arg_node, info * arg_info);
extern node *SEPfunbody (node *arg_node, info * arg_info);
extern node *SEPstatementlist (node *arg_node, info * arg_info);
extern node *SEPfuncall (node *arg_node, info * arg_info);
extern node *SEPforstat (node *arg_node, info * arg_info);
extern node *SEPvar (node *arg_node,info *arg_info);
//extern node *SEPstatement(node *arg_node, info * arg_info);
extern node *SEPenclosedblock(node *arg_node, info * arg_info);

extern node *SEPdoSeparateDeclInit( node *syntaxtree);

#endif /* _SAC_SEP_NODE_H_ */
