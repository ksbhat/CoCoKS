/*
 * infostruct.h
 *
 *  Created on: Mar 18, 2013
 *      Author: sharan
 */

#ifndef INFOSTRUCT_H_
#define INFOSTRUCT_H_


struct varlistname{
	char* id;
	char* previd;
	struct varlistname *next;
};

struct variablelist {
	node *varvardec;
	struct variablelist *next;
};

struct scopedvariable {
	struct variablelist *head;
	struct scopedvariable *next;
};

typedef struct INFO {
	bool firsterror;
	bool isglobal;
	bool variablecheck;
	bool isFunctionCallStmt;
	int enclosedblockedcount;
	node *gstatementlisthead;
	node *gstatementlist;
	node *lstatementlisthead;
	node *lstatementlist;
	node *functionname;
	node *gvardeclist;
	node *lvardeclist;
	node *prevparamlist;
	node *appendstatement;
	struct varlistname *forid;
	struct varlistname *head_varlistname;
	struct scopedvariable *head_scopedarray;
	struct scopedvariable *shadowed_variable;
	struct variablelist *build_varlist;
	struct variablelist *build_shadowlist;
}info;
#define INFO_FIRSTERROR(n) ((n)->firsterror)
#define INFO_VARIABLECHECK(n) ((n)->variablecheck)
#define INFO_ISGLOBAL(n) ((n)->isglobal)
#define INFO_GSTATEMENTLIST(n) ((n)->gstatementlist)
#define INFO_GSTATEMENTLISTHEAD(n) ((n)->gstatementlisthead)
#define INFO_LSTATEMENTLIST(n) ((n)->lstatementlist)
#define INFO_LSTATEMENTLISTHEAD(n) ((n)->lstatementlisthead)
#define INFO_GVARDECLIST(n)    ((n)->gvardeclist)
#define INFO_LVARDECLIST(n)    ((n)->lvardeclist)
#define INFO_PREVPARAMLIST(n)   ((n)->prevparamlist)
#define INFO_FUNCTIONNAME(n) ((n)->functionname)
#define INFO_INITPROGRAM(n) ((n)->initprogram)
#define INFO_VARLISTNAME(n) ((n)->head_varlistname)
#define INFO_HEADSCOPEDARRAY(n) ((n)->head_scopedarray)
#define INFO_BUILDVARLIST(n) ((n)->build_varlist)
#define INFO_SHADOWVARIABLESCOPE(n) ((n)->shadowed_variable)
#define INFO_SHADOWVARIABLELIST(n) ((n)->build_shadowlist)
#define INFO_APPENDSTATEMENT(n)	((n)->appendstatement)
#define INFO_ISFUNCALLSTATEMENT(n) ((n)->isFunctionCallStmt)
#define INFO_FORID(n) ((n)->forid)
#define INFO_ENLCOSEDBLOCKCOUNT(n) ((n)->enclosedblockedcount)

#define NAME_COUNTER_LIMIT 100




#endif /* INFOSTRUCT_H_ */
