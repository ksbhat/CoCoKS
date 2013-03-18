#ifndef _RENAME_H_
#define _RENAME_H_

#include "types.h"

extern node *RENAMEvar (node *node, info *info);
extern node *RENAMEforstat (node *node, info *info);
extern node *RENAMEenclosedblock(node *arg_node, info * arg_info);

#endif
