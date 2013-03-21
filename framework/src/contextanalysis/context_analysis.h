#ifndef _CIVCC_CONTEXT_ANALYSIS_H_
#define _CIVCC_CONTEXT_ANALYSIS_H_

#include "types.h"
#include "lookup_table.h"

typedef enum {
	variablename,functionname,globaldec,globalarraydec,parameter
}VARTYPE;

struct stacklist {
	type paramtype;
	bool isarray;
	int dimension_count;
	struct stacklist *next;
};

struct scopedvariable {
	lut_t *hash_table;
	struct scopedvariable *next;
};

struct variableinfo {
	node *vardec;
	node *exprlist; // array size info.
};

struct globalvariable {
	node *usual_def;
};

struct globalArrayVariable {
	node *array_def;
	node *exprlist;
};

struct functioninfo {
	type returntype;
	node *funheader;
	node *paramlist;
};

struct paraminformation {
	type *type;
	node *param;
	node *varnumlist;
};

// type of the values in the hash_tables.
struct idinfo {
	VARTYPE vartype;
	node *varnode;
	union {
		struct variableinfo *varinfo;
		struct functioninfo *funinfo;
		struct globalvariable *globalvarinfo;
		struct globalArrayVariable *globalarrayinfo;
		struct paraminformation *paraminfo;
	}info;
};

extern node *CAprogram(node *arg_node, info * arg_info);
extern node *CAvardeclistlocalfundef(node *arg_node,info *arg_info);
extern node *CAfundec(node *arg_node,info *arg_info);
extern node *CAfunheader(node *arg_node,info *arg_info);
extern node *CAfundef (node *arg_node, info * arg_info);
extern node *CAlocalfundef(node *arg_node, info * arg_info);
extern node *CAusualdef(node *arg_node,info *arg_info);
extern node *CAarraydef(node *arg_node,info *arg_info);
extern node *CAvar(node *arg_node, info *arg_info);
extern node *CAarrayinit(node *arg_node, info *arg_info);
extern node *CAbinop(node *arg_node, info *arg_info);
extern node *CAmonop(node *arg_node, info *arg_info);
extern node *CAfuncall(node *arg_node, info *arg_info);
extern node *CAcast(node *arg_node, info *arg_info);
extern node *CAassign(node *arg_node, info *arg_info);
extern node *CAforstat(node *arg_node, info *arg_info);
extern node *CAdowhilestat(node *arg_node, info *arg_info);
extern node *CAwhilestat(node *arg_node, info *arg_info);
extern node *CAifstat(node *arg_node, info *arg_info);
extern node *CAstatementlist(node *arg_node, info *arg_info);
extern node *CAfunbody(node *arg_node,info *arg_info);
extern node *CAvardec(node *arg_node,info *arg_info);

#endif 