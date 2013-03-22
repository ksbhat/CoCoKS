#ifndef _CIVCC_REPLACELOGICALOPS_H_
#define _CIVCC_REPLACELOGICALOPS_H_

#include "types.h"

extern node *CONusualdef(node *arg_node,info *arg_info);
extern node *CONvardec(node *arg_node,info *arg_info);
extern node *CONifstat(node *arg_node, info *arg_info);
extern node *CONwhilestat(node *arg_node, info *arg_info);
extern node *CONdowhilestat(node *arg_node, info *arg_info);
extern node *CONforstat(node *arg_node, info *arg_info);
extern node *CONassign(node *arg_node, info *arg_info);
extern node *CONstatementlist(node *arg_node, info *arg_info);
extern node *CONcast(node *arg_node, info *arg_info);
extern node *CONexprlist(node *arg_node, info *arg_info);
extern node *CONvardeclist(node *arg_node, info *arg_info);
extern node *CONbinop(node *arg_node, info *arg_info);

#endif 