
/**
 * @file print.h
 *
 * Functions to print node structures
 * 
 */

#ifndef _SAC_SEP_NODE_H_
#define _SAC_SEP_NODE_H_

#include "types.h"

extern node *SEPinstrs (node * arg_node, info * arg_info);
extern node *SEPassign (node * arg_node, info * arg_info);
extern node *SEPvar (node * arg_node, info * arg_info);
extern node *SEPvarlet (node * arg_node, info * arg_info);
extern node *SEPbinop (node * arg_node, info * arg_info);
extern node *SEPfloat (node * arg_node, info * arg_info);
extern node *SEPnum (node * arg_node, info * arg_info);
extern node *SEPbool (node * arg_node, info * arg_info);
extern node *SEPerror (node * arg_node, info * arg_info);
extern node *SEPstatement(node * arg_node, info * arg_info);
extern node *SEPprogram (node *arg_node, info * arg_info);
extern node *SEPvardec (node *arg_node, info * arg_info);
extern node *SEPparam (node *arg_node, info * arg_info);
extern node *SEPfundec (node *arg_node, info * arg_info);
extern node *SEPfundef (node *arg_node, info * arg_info);
extern node *SEPparamlist (node *arg_node, info * arg_info);
extern node *SEPfunheader (node *arg_node, info * arg_info);
extern node *SEPlocalfundef (node *arg_node, info * arg_info);
extern node *SEPfunbody (node *arg_node, info * arg_info);
extern node *SEPlocalfundeflist (node *arg_node, info * arg_info);
extern node *SEPglobaldef (node *arg_node, info * arg_info);
extern node *SEPusualdef (node *arg_node, info * arg_info);
extern node *SEParraydef (node *arg_node, info * arg_info);
extern node *SEPglobaldec (node *arg_node, info * arg_info);
extern node *SEPvarnumlist (node *arg_node, info * arg_info);
extern node *SEPvardeclist (node *arg_node, info * arg_info);
extern node *SEPreturn (node *arg_node, info * arg_info);
extern node *SEPstatementlist (node *arg_node, info * arg_info);
extern node *SEPdowhilestat (node *arg_node, info * arg_info);
extern node *SEPwhilestat (node *arg_node, info * arg_info);
extern node *SEPforstat (node *arg_node, info * arg_info);
extern node *SEPfuncall (node *arg_node, info * arg_info);
extern node *SEPexprlist (node *arg_node, info * arg_info);
extern node *SEPifstat (node *arg_node, info * arg_info);
extern node *SEPelseblock (node *arg_node, info * arg_info);
extern node *SEPmonop (node *arg_node, info * arg_info);
extern node *SEPcast (node *arg_node, info * arg_info);
extern node *SEPenclosedblock (node *arg_node, info * arg_info);
extern node *SEPvardeclistlocalfundef(node *arg_node, info * arg_info);
extern node *SEParrayinit(node *arg_node, info * arg_info);


extern node *SEPdoSeparateDeclInit( node *syntaxtree);

#endif /* _SAC_SEP_NODE_H_ */
