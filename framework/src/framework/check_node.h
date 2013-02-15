
/**
 * @file check_node.h
 *
 * Functions to free node structures
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: check_node.h.xsl 15657 2007-11-13 13:57:30Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

#ifndef _SAC_CHECK_NODE_H_
#define _SAC_CHECK_NODE_H_

#include "types.h"

extern node *CHKMpostfun (node * arg_node, info * arg_info);

extern node *CHKMassign (node * arg_node, info * arg_info);
extern node *CHKMbinop (node * arg_node, info * arg_info);
extern node *CHKMbool (node * arg_node, info * arg_info);
extern node *CHKMcast (node * arg_node, info * arg_info);
extern node *CHKMconst (node * arg_node, info * arg_info);
extern node *CHKMdo (node * arg_node, info * arg_info);
extern node *CHKMdowhilestat (node * arg_node, info * arg_info);
extern node *CHKMelse (node * arg_node, info * arg_info);
extern node *CHKMelseblock (node * arg_node, info * arg_info);
extern node *CHKMenclosedblock (node * arg_node, info * arg_info);
extern node *CHKMenclosedexpr (node * arg_node, info * arg_info);
extern node *CHKMequals (node * arg_node, info * arg_info);
extern node *CHKMerror (node * arg_node, info * arg_info);
extern node *CHKMexport (node * arg_node, info * arg_info);
extern node *CHKMexprlist (node * arg_node, info * arg_info);
extern node *CHKMextern (node * arg_node, info * arg_info);
extern node *CHKMfloat (node * arg_node, info * arg_info);
extern node *CHKMfor (node * arg_node, info * arg_info);
extern node *CHKMfordec (node * arg_node, info * arg_info);
extern node *CHKMforstat (node * arg_node, info * arg_info);
extern node *CHKMforvarinit (node * arg_node, info * arg_info);
extern node *CHKMfunbody (node * arg_node, info * arg_info);
extern node *CHKMfuncall (node * arg_node, info * arg_info);
extern node *CHKMfundec (node * arg_node, info * arg_info);
extern node *CHKMfundef (node * arg_node, info * arg_info);
extern node *CHKMfunheader (node * arg_node, info * arg_info);
extern node *CHKMglobaldec (node * arg_node, info * arg_info);
extern node *CHKMglobaldef (node * arg_node, info * arg_info);
extern node *CHKMifkeyword (node * arg_node, info * arg_info);
extern node *CHKMifstat (node * arg_node, info * arg_info);
extern node *CHKMkey_bool (node * arg_node, info * arg_info);
extern node *CHKMkey_float (node * arg_node, info * arg_info);
extern node *CHKMkey_int (node * arg_node, info * arg_info);
extern node *CHKMkey_void (node * arg_node, info * arg_info);
extern node *CHKMl_bracket (node * arg_node, info * arg_info);
extern node *CHKMl_fl_bracket (node * arg_node, info * arg_info);
extern node *CHKMmonop (node * arg_node, info * arg_info);
extern node *CHKMnum (node * arg_node, info * arg_info);
extern node *CHKMparam (node * arg_node, info * arg_info);
extern node *CHKMparamlist (node * arg_node, info * arg_info);
extern node *CHKMprogram (node * arg_node, info * arg_info);
extern node *CHKMr_bracket (node * arg_node, info * arg_info);
extern node *CHKMr_fl_bracket (node * arg_node, info * arg_info);
extern node *CHKMrettype (node * arg_node, info * arg_info);
extern node *CHKMreturn (node * arg_node, info * arg_info);
extern node *CHKMreturnkeyword (node * arg_node, info * arg_info);
extern node *CHKMstatement (node * arg_node, info * arg_info);
extern node *CHKMstatementlist (node * arg_node, info * arg_info);
extern node *CHKMtype (node * arg_node, info * arg_info);
extern node *CHKMvar (node * arg_node, info * arg_info);
extern node *CHKMvardec (node * arg_node, info * arg_info);
extern node *CHKMvardeclist (node * arg_node, info * arg_info);
extern node *CHKMvarlet (node * arg_node, info * arg_info);
extern node *CHKMwhile (node * arg_node, info * arg_info);
extern node *CHKMwhilestat (node * arg_node, info * arg_info);

#endif /* _SAC_CHECK_NODE_H_ */
