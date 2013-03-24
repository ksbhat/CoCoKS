
/**
 * @file print.c
 *
 * Functions needed by print traversal.
 *
 */

/**
 * @defgroup print Print Functions.
 *
 * Functions needed by print traversal.
 *
 * @{
 */



#include "traverse.h"
#include "tree_basic.h"
#include "copy.h"
#include "dbug.h"
#include "memory.h"
#include "globals.h"
#include "str.h"
#include "print.h"
#include "compute_structure.c"
#include "lookup_table.h"
#include "mytypes.h"
#include "stack_lib.h"
#include "ctinfo.h"
#include "context_analysis.h"

/*
 * INFO structure
 */


struct INFO
{
	bool firsterror;
	node *programhead;
	bool variablerun;
	node *programstart;
	lut_t *buildhashlist;
	char* functioname;
	struct scopedvariable *scopedlist;
	type result_type;
};

#define INFO_FIRSTERROR(n) ((n)->firsterror)
#define INFO_PROGRAMHEAD(n) ((n)->programhead)
#define INFO_VARIABLERUN(n) ((n)->variablerun)
#define INFO_PROGRAMSTART(n) ((n)->programstart)
#define INFO_FUNCTIONNAME(n) ((n)->functioname)
#define INFO_BUILDHASHTABLE(n) ((n)->buildhashlist)
#define INFO_HEADSCOPEDARRAY(n) ((n)->scopedlist)
#define INFO_RESULTTYPE(n) ((n)->result_type)

static info *MakeInfo()
{
	info *result;

	result = MEMmalloc(sizeof(info));

	INFO_FIRSTERROR(result) = FALSE;

	INFO_PROGRAMHEAD(result)=NULL;

	INFO_VARIABLERUN(result)=FALSE;

	INFO_PROGRAMSTART(result)=NULL;

	INFO_FUNCTIONNAME(result)=NULL;	

	INFO_BUILDHASHTABLE(result)=NULL;	

	INFO_HEADSCOPEDARRAY(result)=NULL;

	INFO_RESULTTYPE(result)= T_unknown;

	return result;
}

//List error messages

void CAredeclarationerror(int currentLine,char *varname,int prevLine)
{
	CTIerror("Operation performed on line: %d \n reason:\tredeclaration of variable matches with var %s declared on %d",currentLine,varname,prevLine);
}

void CAdeclarationnotfound(int linenumber,char *varname)
{
	CTIerror("Operation performed on line: %d \n reason:\tDeclaration of the variable %s is missing",linenumber,varname);
}

void CAtypemismatcherror(int linenumber,char *errstring)
{
	CTIerror("Operation performed on line: %d \n reason:\ttype mismatch: %s ",linenumber, errstring);
}

void CAmoreparamsthanrequirederror(int linenumber)
{
	CTIerror("Operation performed on line: %d \n reason:\tMore arguments found than required: %s ",linenumber);
}


void CAtoofewargumentserror(int linenumber)
{
	CTIerror("Operation performed on line: %d \n reason:\tToo few arguments specified: %s ",linenumber);
}

void CAarraydimensionmismatcherror(int linenumber, int expectedcount, int observedcount)
{
	CTIerror("Operation performed on line: %d \n reason:\tArray dimension mismatch. Expected count: %d ; Observed count: %d",linenumber, expectedcount, observedcount);
}

void CAarrayindicesmustbeinterror(int linenumber)
{
	CTIerror("Operation performed on line: %d \n reason:\tArray indexing must be of type, int.",linenumber);
}

void CAarrayindexingerror(int linenumber)
{
	CTIerror("Operation performed on line: %d \n reason:\tNo., of indices used for indexing array doesnt match array dimension.",linenumber);
}


static info *FreeInfo( info *info)
{
	info = MEMfree( info);

	return info;
}

static void freehashtable(lut_t *hash_table)
{
	DBUG_ENTER("CAfreehashtable");
	LUTremoveLut(hash_table);
	DBUG_VOID_RETURN;
}

static void freescopedvariable(struct scopedvariable *scopedlist)
{
	DBUG_ENTER("CAfreescopedvariable");
	struct scopedvariable *prevlist;
	prevlist=scopedlist;
	freehashtable(scopedlist->hash_table);
	MEMfree(prevlist);
	DBUG_VOID_RETURN;
}

static void freestacklist(struct stacklist *stacklist)
{
	DBUG_ENTER("freestacklist");
	struct stacklist *curr;
	while(NULL != stacklist)
	{
		curr = stacklist;
		stacklist = stacklist->next;

		MEMfree(curr);
	}
	DBUG_VOID_RETURN;
}

/*
char *SEPgenvarname(info *arg_info)
{
	DBUG_ENTER("SEPgetvarname");
	char szPattern[3] = "__";
	char  *name,*clearname;
	static int counter = 0;
	struct varlistname *varseq;
	bool flag;

	while (TRUE)
	{
		flag=FALSE;
		if(counter<10)
		{
			clearname=STRitoa(counter);
			name=STRcat("0",clearname);
			MEMfree(clearname);
			clearname=name;
		}
		name = STRcat(szPattern, name);
		MEMfree(clearname);
		counter = (counter + 1) % NAME_COUNTER_LIMIT;
		if(INFO_VARLISTNAME(arg_info)!=NULL)
		{

			varseq=INFO_VARLISTNAME(arg_info);
			while(varseq!=NULL)
			{

				if(STReq(varseq->id,name))
				{

					flag=TRUE;
				}
				varseq=varseq->next;
			}
			if(!flag)
			{
				DBUG_RETURN(name);
			}
		}
		else
			break;
	}
	DBUG_RETURN(name);
}
 */

/*
bool checkVariableExist(node *var_node,info *arg_info)
{
	DBUG_ENTER("SEPCheckvariableexist");
	char *id,*comparedid;
	struct scopedvariable *scoped_array;
	struct variablelist *variables;
	node* variable;

	id= VAR_NAME(var_node);

	printf("\nInside checkvariable with ID:%s",id);

	scoped_array=INFO_HEADSCOPEDARRAY(arg_info);

	while(scoped_array!=NULL)
	{

		variables=scoped_array->head;

		while(variables!=NULL)
		{
			variable = variables->varvardec;

			comparedid=VAR_NAME(VARDEC_VAR(variable));
			printf("\nInside checkvariable with ID:%s",comparedid);
			if(STReq(id,comparedid))
			{
				printf("\nFOUND MATCHING VARIABLES");
				DBUG_RETURN(TRUE);
			}
			variables=variables->next;
		}
		scoped_array=scoped_array->next;
	}
	DBUG_RETURN(FALSE);
}

bool checkshadow(node *var_node,info *arg_info)
{
	DBUG_ENTER("SEPcheckshadow");
	char *id,*comparedid;
	struct scopedvariable *shadow_variable;
	struct variablelist *variables;
	node* variable;

	id= VAR_NAME(var_node);



	shadow_variable=INFO_SHADOWVARIABLESCOPE(arg_info);
	while(shadow_variable!=NULL)
	{
		variables=shadow_variable->head;

		while(variables!=NULL)
		{

			variable = variables->varvardec;

			comparedid=VAR_NAME(VARDEC_VAR(variable));
			printf("\ncompareid:%s",comparedid);
			if(STReq(id,comparedid))
			{
				printf("inside return loop");
				DBUG_RETURN(TRUE);
			}
			variables=variables->next;
		}

		shadow_variable=shadow_variable->next;
	}

	DBUG_RETURN(FALSE);
}
 */

node *CAprogram(node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEPprogram");
	node *program;
	struct scopedvariable *globalarray;
	//struct variablelist *backup;

	if(INFO_PROGRAMSTART(arg_info)==NULL)
	{
		printf("I am inside the first loop");
		INFO_PROGRAMSTART(arg_info)=arg_node;
		INFO_BUILDHASHTABLE(arg_info)=LUTgenerateLut();
		INFO_VARIABLERUN(arg_info)=TRUE;
	}

	PROGRAM_NEXT(arg_node)=TRAVopt(PROGRAM_NEXT(arg_node),arg_info);

	PROGRAM_CODEBLOCK(arg_node)=TRAVdo(PROGRAM_CODEBLOCK(arg_node),arg_info);

	if(INFO_PROGRAMSTART(arg_info)==arg_node)
	{

		if(INFO_BUILDHASHTABLE(arg_info)!=NULL)
		{
			globalarray=MEMmalloc(sizeof(struct scopedvariable));
			globalarray->hash_table=INFO_BUILDHASHTABLE(arg_info);
			globalarray->next=NULL;
			INFO_HEADSCOPEDARRAY(arg_info)=globalarray;
			printf("\n Created hashtable at the global scope");

		}
		else
			printf("\n Value INFO HASH TABLE is null");

		INFO_BUILDHASHTABLE(arg_info)=NULL;

		INFO_VARIABLERUN(arg_info)=FALSE;

		//Funbody can start processing the contents in the body
		program=arg_node;
		int i=0;
		while(program!=NULL)
		{

			if(NODE_TYPE(PROGRAM_CODEBLOCK(program))==N_fundef || NODE_TYPE(PROGRAM_CODEBLOCK(program))==N_vardeclistlocalfundef)
			{	
				printf("\nAbout to get through the body of the function");
				PROGRAM_CODEBLOCK(program)=TRAVdo(PROGRAM_CODEBLOCK(program),arg_info);
			}
			program=PROGRAM_NEXT(program);	
		}

		/*
		 * this code will the global scope variable
		 */

		if(INFO_HEADSCOPEDARRAY(arg_info)!=NULL)
		{
			freescopedvariable(INFO_HEADSCOPEDARRAY(arg_info));
		}

	}

	DBUG_RETURN(arg_node);
}

node *CAvardeclistlocalfundef(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAvardeclistlocalfundef");
	if(INFO_VARIABLERUN(arg_info)==TRUE)
	{
		VARDECLISTLOCALFUNDEF_VARDECLIST(arg_node)=TRAVopt(VARDECLISTLOCALFUNDEF_VARDECLIST(arg_node),arg_info);
	}

	VARDECLISTLOCALFUNDEF_LOCALFUNDEF(arg_node)=TRAVopt(VARDECLISTLOCALFUNDEF_LOCALFUNDEF(arg_node),arg_info);

	DBUG_RETURN(arg_node);
}

node *CAfundec(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAfundec");
	FUNDEC_FUNHEADER(arg_node)=TRAVdo(FUNDEC_FUNHEADER(arg_node),arg_info);
	DBUG_RETURN(arg_node);
}


node *CAfunheader(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAfunheader");

	struct functioninfo *funinfo;
	struct idinfo *iddetails,**previddetails;
	char *funname;



	if(INFO_VARIABLERUN(arg_info))
	{
		lut_t *hash_table= INFO_BUILDHASHTABLE(arg_info);


		if(hash_table!=NULL)
		{

			funinfo=MEMmalloc(sizeof(struct functioninfo));

			//Memory used only for the structure rest reuses the memory from existing memory. Do not release it.
			funinfo->funheader=arg_node;
			funinfo->returntype=FUNHEADER_TYPE(arg_node);
			funinfo->paramlist=FUNHEADER_PARAMLIST(arg_node);


			iddetails=MEMmalloc(sizeof(struct idinfo));
			iddetails->vartype=functionname;
			iddetails->varnode=FUNHEADER_VAR(arg_node);
			iddetails->info.funinfo=funinfo;

			funname=VAR_NAME(FUNHEADER_VAR(arg_node));
			previddetails=(struct idinfo **)LUTsearchInLutS(hash_table,funname);

			if(previddetails==NULL)
			{
				hash_table=LUTinsertIntoLutS(hash_table,funname,iddetails);
			}
			else
			{
				CAredeclarationerror(NODE_LINE(arg_node),funname,NODE_LINE((*previddetails)->varnode));
			}
		}
	}
	else
	{
		INFO_FUNCTIONNAME(arg_info)=VAR_NAME(FUNHEADER_VAR(arg_node));
	}	

	DBUG_RETURN(arg_node);
}

node *CAfundef (node *arg_node, info * arg_info)
{
	DBUG_ENTER("CAfundef");

	FUNDEF_LOCALFUNDEF (arg_node)=TRAVdo(FUNDEF_LOCALFUNDEF (arg_node),arg_info);

	DBUG_RETURN(arg_node);

}


node *CAlocalfundef(node *arg_node, info * arg_info)
{
	DBUG_ENTER("CAlocalfundef");

	LOCALFUNDEF_FUNHEADER(arg_node)=TRAVdo(LOCALFUNDEF_FUNHEADER(arg_node),arg_info);
	if(!INFO_VARIABLERUN(arg_info))
	{
		printf("\ninside funbody");
		LOCALFUNDEF_FUNBODY (arg_node)=TRAVdo(LOCALFUNDEF_FUNBODY (arg_node),arg_info);
	}
	DBUG_RETURN(arg_node);
}


node *CAusualdef(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAusualdef");
	struct globalvariable *varinfo;
	struct idinfo *id,**previddetails;

	lut_t *hash_table=INFO_BUILDHASHTABLE(arg_info);
	if(hash_table!=NULL)
	{
		varinfo=MEMmalloc(sizeof(struct globalvariable));
		varinfo->usual_def=arg_node;


		id=MEMmalloc(sizeof(struct idinfo));
		id->vartype=globaldec;
		id->varnode=USUALDEF_VAR(arg_node);
		id->info.globalvarinfo=varinfo;
		previddetails=(struct idinfo **)LUTsearchInLutS(hash_table,VAR_NAME(id->varnode));


		if(previddetails==NULL)
			hash_table=LUTinsertIntoLutS(hash_table,VAR_NAME(id->varnode),id);
		else
		{
			CAredeclarationerror(NODE_LINE(arg_node),VAR_NAME(id->varnode),NODE_LINE((*previddetails)->varnode));
		}

	}
	DBUG_RETURN(arg_node);
}


node *CAarraydef(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAarraydef");
	struct globalArrayVariable *varinfo;
	struct idinfo *id,**previddetails;

	lut_t *hash_table=INFO_BUILDHASHTABLE(arg_info);
	if(hash_table!=NULL)
	{
		varinfo=MEMmalloc(sizeof(struct globalArrayVariable));

		// Free ArrayDec
		varinfo->array_def=arg_node;
		varinfo->exprlist=ARRAYDEF_EXPRLIST(arg_node);

		id=MEMmalloc(sizeof(struct idinfo));
		id->vartype=globalarraydec;
		id->varnode=ARRAYDEF_VAR(arg_node);
		id->info.globalarrayinfo=varinfo;
		printf("\n before the search Table");
		previddetails=(struct idinfo **)LUTsearchInLutS(hash_table,VAR_NAME(id->varnode));

		if(previddetails==NULL)
		{
			printf("\n new insert");
			hash_table=LUTinsertIntoLutS(hash_table,VAR_NAME(id->varnode),id);
		}
		else
		{
			printf("\n Already exists");
			CAredeclarationerror(NODE_LINE(arg_node),VAR_NAME(id->varnode),NODE_LINE((*previddetails)->varnode));
		}

	}
	DBUG_RETURN(arg_node);
}

node *CAvardec(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAvardec");
	struct variableinfo *varinfo;
	struct idinfo *id;
	struct idinfo **previddetails;
	node *exprlist;

	lut_t *hash_table=INFO_BUILDHASHTABLE(arg_info);
	exprlist=VARDEC_EXPRLIST(arg_node);
	while(exprlist!=NULL)
	{
		INFO_RESULTTYPE(arg_info)= T_unknown;
		EXPRLIST_EXPR(exprlist)=TRAVdo(EXPRLIST_EXPR(exprlist),arg_info);
		if(INFO_RESULTTYPE(arg_info)!=T_int)
		{
			CAarrayindicesmustbeinterror(NODE_LINE(arg_node));
		}
		exprlist=EXPRLIST_NEXT(exprlist);
	}

	if(hash_table!=NULL)
	{
		varinfo=MEMmalloc(sizeof(struct variableinfo));

		// Free ArrayDec
		varinfo->vardec=arg_node;
		varinfo->exprlist=VARDEC_EXPRLIST(arg_node);

		id=MEMmalloc(sizeof(struct idinfo));
		id->vartype=variablename;
		id->varnode=VARDEC_VAR(arg_node);
		id->info.varinfo=varinfo;
		previddetails=(struct idinfo **)LUTsearchInLutS(hash_table,VAR_NAME(id->varnode));
		if(previddetails==NULL)
			hash_table=LUTinsertIntoLutS(hash_table,VAR_NAME(id->varnode),id);
		else
		{

			CAredeclarationerror(NODE_LINE(arg_node),VAR_NAME(id->varnode),NODE_LINE((*previddetails)->varnode));
		}

	}
	DBUG_RETURN(arg_node);

}

node *CAfunbody(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAfunbody");
	struct scopedvariable *scopevariablelist;
	struct idinfo *idinfo;
	node *paramlist;
	node *param;
	lut_t *hash_table;
	struct functioninfo *funinfo;
	struct idinfo *varinfo,**prevvarinfo;
	struct paraminformation *paraminfo;
	char *paramname;
	bool ishashadded=FALSE;
	struct scopedvariable *currentscopeinfo;
	printf("\nI am inside funbody");
	hash_table=LUTgenerateLut();
	if(INFO_FUNCTIONNAME(arg_info)!=NULL)
	{
		scopevariablelist=INFO_HEADSCOPEDARRAY(arg_info);
		prevvarinfo=(struct idinfo**) LUTsearchInLutS(scopevariablelist->hash_table,INFO_FUNCTIONNAME(arg_info));

		if(prevvarinfo!=NULL)
		{
			varinfo=(*prevvarinfo);
			funinfo=varinfo->info.funinfo;
			paramlist=funinfo->paramlist;

			while(paramlist!=NULL)
			{
				param=PARAMLIST_PARAM(paramlist);
				paraminfo=MEMmalloc(sizeof(struct paraminformation));
				paraminfo->param=param;
				paraminfo->varnumlist=PARAM_VARNUMLIST(param);
				paramname=VAR_NAME(PARAM_VAR(param));

				idinfo=MEMmalloc(sizeof(struct idinfo));
				idinfo->vartype=parameter;
				idinfo->varnode=PARAM_VAR(param);
				idinfo->info.paraminfo=paraminfo;
				LUTinsertIntoLutS(hash_table,paramname,idinfo);				
				paramlist=PARAMLIST_NEXT(paramlist);
			}
		}	
	}

	INFO_VARIABLERUN(arg_info)=TRUE;
	if(!LUTisEmptyLut(hash_table))
	{
		INFO_BUILDHASHTABLE(arg_info)=hash_table;
	}
	else
		INFO_BUILDHASHTABLE(arg_info)=NULL;

	FUNBODY_VARDECLIST(arg_node)=TRAVopt(FUNBODY_VARDECLIST(arg_node),arg_info);

	FUNBODY_LOCALFUNDEFLIST(arg_node)=TRAVopt(FUNBODY_LOCALFUNDEFLIST(arg_node),arg_info);

	//Creating a scope for this function prefixing it to the head of the list.
	if(INFO_BUILDHASHTABLE(arg_info)!=NULL)
	{
		printf("\n creating a hash table at local scope");
		scopevariablelist =MEMmalloc(sizeof(struct scopedvariable));
		scopevariablelist->hash_table=INFO_BUILDHASHTABLE(arg_info);
		scopevariablelist->next=INFO_HEADSCOPEDARRAY(arg_info);
		printf("\n Created hashtable at the global scope");
		INFO_HEADSCOPEDARRAY(arg_info)=scopevariablelist;
		INFO_BUILDHASHTABLE(arg_info)=NULL;
		ishashadded=TRUE;

	}

	//Second pass starts here
	INFO_VARIABLERUN(arg_info)=FALSE; 

	FUNBODY_LOCALFUNDEFLIST(arg_node)=TRAVopt(FUNBODY_LOCALFUNDEFLIST(arg_node),arg_info);
	FUNBODY_STATEMENTLIST(arg_node)=TRAVopt(FUNBODY_STATEMENTLIST(arg_node),arg_info);

	//Removing the head element from the scopevariable as we are leaving from the current scope
	if(ishashadded)
	{
		currentscopeinfo=INFO_HEADSCOPEDARRAY(arg_info);
		INFO_HEADSCOPEDARRAY(arg_info)=scopevariablelist->next;
		freescopedvariable(currentscopeinfo);
	}

	DBUG_RETURN(arg_node);
}

node *CAstatementlist(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAstatementlist");
	STATEMENTLIST_NEXT(arg_node)=TRAVopt(STATEMENTLIST_NEXT(arg_node),arg_info);
	STATEMENTLIST_STATEMENT(arg_node)=TRAVdo(STATEMENTLIST_STATEMENT(arg_node),arg_info);
	DBUG_RETURN(arg_node);

}

node *CAifstat(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAifstatement");
	type exprtype;

	IFSTAT_EXPR(arg_node) = TRAVdo(IFSTAT_EXPR(arg_node), arg_info);

	exprtype = INFO_RESULTTYPE(arg_info);

	IFSTAT_BLOCK(arg_node) = TRAVopt(IFSTAT_BLOCK(arg_node), arg_info);

	IFSTAT_ELSEBLOCK(arg_node) = TRAVopt(IFSTAT_ELSEBLOCK(arg_node), arg_info);	

	if ( exprtype != T_bool)
	{
		CAtypemismatcherror(NODE_LINE(arg_node), "Expected bool expression type.");
	}

	DBUG_RETURN(arg_node);
}

node *CAwhilestat(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAwhilestatement");
	type exprtype;

	WHILESTAT_EXPR(arg_node) = TRAVdo(WHILESTAT_EXPR(arg_node), arg_info);

	exprtype = INFO_RESULTTYPE(arg_info);

	WHILESTAT_BLOCK(arg_node) = TRAVdo(WHILESTAT_BLOCK(arg_node), arg_info);	

	if ( exprtype != T_bool)
	{
		CAtypemismatcherror(NODE_LINE(arg_node), "Expected bool expression type.");
	}

	DBUG_RETURN(arg_node);
}

node *CAdowhilestat(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAdowhilestatement");
	type exprtype;

	DOWHILESTAT_EXPR(arg_node) = TRAVdo(DOWHILESTAT_EXPR(arg_node), arg_info);

	exprtype = INFO_RESULTTYPE(arg_info);

	DOWHILESTAT_BLOCK(arg_node) = TRAVdo(DOWHILESTAT_BLOCK(arg_node), arg_info);	

	if ( exprtype != T_bool)
	{
		CAtypemismatcherror(NODE_LINE(arg_node), "Expected bool expression type.");
	}

	DBUG_RETURN(arg_node);
}

node *CAforstat(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAforstatement");
	type expr1type, expr2type = T_int, vartype;

	FORSTAT_VARDEC(arg_node) = TRAVdo(FORSTAT_VARDEC(arg_node), arg_info);

	vartype = INFO_RESULTTYPE(arg_info);

	FORSTAT_EXPR1(arg_node) = TRAVdo(FORSTAT_EXPR1(arg_node), arg_info);

	expr1type = INFO_RESULTTYPE(arg_info);

	FORSTAT_EXPR2(arg_node) = TRAVopt(FORSTAT_EXPR2(arg_node), arg_info);	

	FORSTAT_BLOCK(arg_node) = TRAVdo(FORSTAT_BLOCK(arg_node), arg_info);

	if (FORSTAT_EXPR2(arg_node) != NULL)
	{
		expr2type = INFO_RESULTTYPE(arg_info);
	}

	if ( T_int != vartype || T_int != expr1type || T_int != expr2type )
	{
		CAtypemismatcherror(NODE_LINE(arg_node), "Expected int expression type.");
	}

	INFO_RESULTTYPE(arg_info) = T_unknown;

	DBUG_RETURN(arg_node);
}


node *CAassign(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAassignstatement");
	type exprtype, vartype;
	INFO_RESULTTYPE(arg_info)=T_unknown;
	ASSIGN_EXPR(arg_node) = TRAVdo(ASSIGN_EXPR(arg_node), arg_info);

	exprtype = INFO_RESULTTYPE(arg_info);

	ASSIGN_VAR(arg_node) = TRAVdo(ASSIGN_VAR(arg_node), arg_info);

	vartype = INFO_RESULTTYPE(arg_info);

	if ( exprtype != vartype)
	{
		CAtypemismatcherror(NODE_LINE(arg_node), "Variable type doesnt match the expression result type.");
	}

	DBUG_RETURN(arg_node);
}

node *CAcast(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAcast");
	type newtype;
	//char errorstring[100];

	INFO_RESULTTYPE(arg_info)=T_unknown;
	CAST_EXPR(arg_node) = TRAVdo(CAST_EXPR(arg_node), arg_info);

	//exprtype = INFO_RESULTTYPE(arg_info);

	//CAST_TYPE(arg_node) = TRAVdo(CAST_TYPE(arg_node), arg_info);

	newtype = INFO_RESULTTYPE(arg_info);

	/*
	switch( newtype)
	{
		case T_int:	if (exprtype ==  T_bool)
					{
						errorstring = STRcat("Invalid cast: Cannot cast to ", "bool");
					}
					break;

		case T_bool: if (exprtype ==  T_int)
					 {
						errorstring = STRcat("Invalid cast: Cannot cast to ", "bool");
					 }
					break;

			break;
		case T_float:

			break;
		default:

			break;
	}
	 */

	INFO_RESULTTYPE(arg_info) = newtype;

	DBUG_RETURN(arg_node);
}

node *CAfuncall(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAfuncall");
	type rettype;
	node *exprlist, *expr, *funheader, *paramlist, *param;
	struct stacklist *stackparamlist, *topstackparamlist;
	struct stacklist *stackexprlist, *topstackexprlist;
	int paramcount = 0;
	int exprcount = 0,dimension_count=0;
	char *errorstring;
	node *varnumlist,*arrindexlist;
	INFO_RESULTTYPE(arg_info)=T_unknown;
	FUNCALL_VAR(arg_node) = TRAVdo(FUNCALL_VAR(arg_node), arg_info);

	rettype = INFO_RESULTTYPE(arg_info);

	exprlist=FUNCALL_EXPRLIST(arg_node);

	// ---> Type checking
	funheader = FUNCALL_VAR(VAR_DECL(arg_node));

	paramlist = FUNHEADER_PARAMLIST(funheader);

	stackparamlist = NULL; 
	topstackparamlist = NULL;
	topstackexprlist = NULL;

	while(paramlist != NULL)
	{
		// original list traversal
		param = PARAMLIST_PARAM(paramlist);
		paramlist=PARAMLIST_NEXT(paramlist);

		// building reverse list, so that we would be able to ignore the array dimension variables if any.

		// pop the dimension variables if its an array type.
		varnumlist = PARAM_VARNUMLIST(param);
		dimension_count=0;
		while ( NULL != varnumlist)
		{
			// popping off the dimension variables
			topstackparamlist = topstackparamlist->next;
			varnumlist = VARNUMLIST_NEXT(varnumlist);
			dimension_count++;
		}

		// add the param to the list.
		stackparamlist =MEMmalloc(sizeof(struct stacklist));
		stackparamlist->next = topstackparamlist;
		stackparamlist->dimension_count = dimension_count;
		stackparamlist->paramtype = PARAM_TYPE(param);
		topstackparamlist = stackparamlist;

		paramcount++;
	}

	while (exprlist!=NULL)
	{
		expr=EXPRLIST_EXPR(exprlist);

		EXPRLIST_EXPR(expr) = TRAVdo(EXPRLIST_EXPR(expr), arg_info);
		//exprtype = INFO_RESULTTYPE(arg_info);

		arrindexlist = NULL;

		if( NODE_TYPE(expr) == N_var)
		{
			arrindexlist = VARDEC_EXPRLIST(expr);

			dimension_count=0;

			while ( NULL != arrindexlist)
			{
				// popping off the dimension variables
				topstackexprlist = topstackexprlist->next;
				arrindexlist = EXPRLIST_NEXT(arrindexlist);
				dimension_count++;
			}
		}

		// add the param to the list.
		stackexprlist =MEMmalloc(sizeof(struct stacklist));
		stackexprlist->next = topstackexprlist;
		stackexprlist->dimension_count = dimension_count;
		stackexprlist->paramtype = PARAM_TYPE(param);
		topstackexprlist = stackexprlist;

		exprcount++; // counting the total number of args in funcall (including expansions).
	}

	INFO_RESULTTYPE(arg_info) = rettype; 

	//type checking starts.
	if (paramcount < exprcount)
	{
		CAmoreparamsthanrequirederror(NODE_LINE(arg_node));
		INFO_RESULTTYPE(arg_info) = T_unknown;
	}
	else if ( paramcount > exprcount)
	{
		CAtoofewargumentserror(NODE_LINE(arg_node));
		INFO_RESULTTYPE(arg_info) = T_unknown;
	}
	else
	{
		stackparamlist = topstackparamlist;
		stackexprlist = topstackexprlist;

		// rest of the type checking:
		while(NULL!=stackparamlist)
		{
			if ( stackparamlist->paramtype != stackexprlist->paramtype)
			{
				if(stackparamlist->paramtype == T_int)
				{
					errorstring = "Expect argument type:int.";
				}
				else if (stackparamlist->paramtype == T_float)
				{
					errorstring = "Expect argument type:float.";
				}
				else if (stackparamlist->paramtype == T_bool)
				{
					errorstring = "Expect argument type:bool.";
				}
				CAtypemismatcherror(NODE_LINE(arg_node), errorstring);
				INFO_RESULTTYPE(arg_info) = T_unknown;
			}
			if ( stackparamlist->dimension_count != stackexprlist->dimension_count)
			{
				CAarraydimensionmismatcherror(NODE_LINE(arg_node), stackparamlist->dimension_count, stackexprlist->dimension_count);
				INFO_RESULTTYPE(arg_info) = T_unknown;
			}
		}

	}

	if (NULL != topstackparamlist)
	{
		freestacklist(topstackparamlist);
	}
	if (NULL != topstackexprlist)
	{
		freestacklist(topstackexprlist);
	}

	//--> Array dimension reduction
	/*
	 * Steps: Check for __allocate function call
	 * and replace the exprlist of it, with a binop expr that modifies m, n to m * n
	 */

	node *newexpr, *leftsize, *rightsize;

	char *funname = VAR_NAME(FUNHEADER_VAR(funheader));
	exprlist = FUNCALL_EXPRLIST(arg_node);

	if (STReq("__allocate", funname))
	{
		// replace m, n to m * n
		expr=EXPRLIST_EXPR(exprlist);
		leftsize = expr;
		while (exprlist!=NULL)
		{
			rightsize = EXPRLIST_NEXT(exprlist);
			newexpr = TBmakeBinop(BO_mul, leftsize, rightsize);
			leftsize = newexpr;			
		}

		FUNCALL_EXPRLIST(arg_node) = TBmakeExprlist(newexpr, NULL);
	}

	DBUG_RETURN(arg_node);
}

node *CAmonop(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAmonop");

	type righttype;

	OPERATIONONPARAMETER op_on_parameter;
	MONOPERATION	mon_op;
	INFO_RESULTTYPE(arg_info)=T_unknown;

	MONOP_RIGHT(arg_node) = TRAVdo(MONOP_RIGHT(arg_node), arg_info);

	righttype = INFO_RESULTTYPE(arg_info);

	// Get the kind of operation to evaluate whether the type of operands is valid or not.
	mon_op.input = righttype;
	mon_op.operation = MONOP_OP(arg_node);

	op_on_parameter.optype = monoperation;
	op_on_parameter.operation.monop = &mon_op;

	if (FALSE == checkoperation(&op_on_parameter))
	{
		CAtypemismatcherror(NODE_LINE(arg_node), *(op_on_parameter.errorstring));
		INFO_RESULTTYPE(arg_info) = T_unknown; // We dont know the type!
	}

	DBUG_RETURN(arg_node);	
}

node *CAbinop(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAbinop");

	type lefttype, righttype;

	OPERATIONONPARAMETER op_on_parameter;
	BINOPERATION	bin_op;
	INFO_RESULTTYPE(arg_info)=T_unknown;

	BINOP_LEFT(arg_node) = TRAVdo(BINOP_LEFT(arg_node), arg_info);

	lefttype = INFO_RESULTTYPE(arg_info);

	BINOP_RIGHT(arg_node) = TRAVdo(BINOP_RIGHT(arg_node), arg_info);

	righttype = INFO_RESULTTYPE(arg_info);

	// Get the kind of operation to evaluate whether the type of operands is valid or not.
	bin_op.left = lefttype;
	bin_op.right = righttype;
	bin_op.operation = BINOP_OP(arg_node);

	op_on_parameter.optype = binaryoperation;
	op_on_parameter.operation.binaryop = &bin_op;

	if (FALSE == checkoperation(&op_on_parameter))
	{
		CAtypemismatcherror(NODE_LINE(arg_node), *op_on_parameter.errorstring);
		INFO_RESULTTYPE(arg_info) = T_unknown; // We dont know the type!
	}

	DBUG_RETURN(arg_node);	
}

node *CAarrayinit(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAarrayinit");
	node *expr, *exprlist;
	INFO_RESULTTYPE(arg_info)=T_unknown;
	if (NULL != ARRAYINIT_EXPRLIST(arg_node))
	{
		exprlist  = ARRAYINIT_EXPRLIST(arg_node);
		while( exprlist != NULL)
		{
			expr = EXPRLIST_EXPR(exprlist);
			EXPRLIST_EXPR(expr) = TRAVdo(EXPRLIST_EXPR(expr), arg_info);
			if (T_int != INFO_RESULTTYPE(arg_info))
			{
				// error
				CAarrayindicesmustbeinterror(NODE_LINE(arg_node));
			}
		}
	}

	DBUG_RETURN(arg_node);
}

node *CAvar(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAvar");
	lut_t *hash_table;
	struct scopedvariable *scopedhashtable;
	struct idinfo *varinfo,**prevvarinfo;
	bool flag=FALSE,firsttimeindicator=FALSE;
	node *sizeindex,*exprindex,*sizeexpr,*currexpr;
	node *varexpr, *varexprlist, *exprlist, *sizelist, *newexpr;
	int exprlistcount = 0, expectedcount=0;
	VARTYPE type;

	/*****************************************
	 !!General guideline for every element processing!!

	 TRAV[do|opt]() on the constituent nodes.
	 PROCESS...
	 SET INFO_RESULTTYPE(arg_info) at the end before returning!
	 ******************************************/

	/******TYPE info COLLECTION*****************/
	if (NULL != VAR_EXPRLIST(arg_node))
	{
		exprlistcount = 0;
		varexprlist  = VARDEC_EXPRLIST(arg_node);
		while( varexprlist != NULL)
		{
			varexpr = EXPRLIST_EXPR(varexprlist);
			EXPRLIST_EXPR(varexpr) = TRAVdo(EXPRLIST_EXPR(varexpr), arg_info);
			if (T_int != INFO_RESULTTYPE(arg_info))
			{
				// error
				CAarrayindicesmustbeinterror(NODE_LINE(arg_node));
			}
			exprlistcount++;
		}
	}

	scopedhashtable=INFO_HEADSCOPEDARRAY(arg_info);

	while(scopedhashtable!=NULL)
	{
		hash_table=scopedhashtable->hash_table;
		printf("\n Searching for %s",VAR_NAME(arg_node));
		prevvarinfo=(struct idinfo**)LUTsearchInLutS(hash_table,VAR_NAME(arg_node));

		if(prevvarinfo!=NULL)
		{
			varinfo=*prevvarinfo;
			type=varinfo->vartype;


			if(type==variablename)
			{
				VAR_DECL(arg_node)=varinfo->info.varinfo->vardec; //Getting the vardec from the value stored in the hash table
				INFO_RESULTTYPE(arg_info) = VARDEC_TYPE(VAR_DECL(arg_node));
			}
			else if (type==functionname)
			{	
				VAR_DECL(arg_node)=varinfo->info.funinfo->funheader;
				INFO_RESULTTYPE(arg_info) = FUNHEADER_TYPE(VAR_DECL(arg_node));
			}
			else if (type==globalarraydec)
			{
				VAR_DECL(arg_node)=varinfo->info.globalarrayinfo->array_def;
				INFO_RESULTTYPE(arg_info) = ARRAYDEF_TYPE(VAR_DECL(arg_node));
			}
			else if(type==globaldec)
			{
				VAR_DECL(arg_node)=varinfo->info.globalvarinfo->usual_def;
				INFO_RESULTTYPE(arg_info) = USUALDEF_TYPE(VAR_DECL(arg_node));
			}
			else if(type==parameter)
			{
				VAR_DECL(arg_node)=varinfo->info.paraminfo->param;
				INFO_RESULTTYPE(arg_info) = PARAM_TYPE(VAR_DECL(arg_node));
			}
			//No longer require the name of this variable.
			MEMfree(VAR_NAME(arg_node));
			flag=TRUE;
			break;
		}
		scopedhashtable=scopedhashtable->next;
	}

	if(!flag)
	{
		CAdeclarationnotfound(NODE_LINE(arg_node),VAR_NAME(arg_node));
	}

	if(prevvarinfo != NULL && type!= functionname && flag)
	{
		sizelist = VARDEC_EXPRLIST(VAR_DECL(arg_node));
		expectedcount = 0;
		while(NULL != sizelist)
		{
			expectedcount++;
			sizelist = EXPRLIST_NEXT(sizelist);
		}

		if (expectedcount != exprlistcount)
		{
			CAarrayindexingerror(NODE_LINE(arg_node));
		}

		/*********Array dimension reduction*******************/

		exprlist = VAR_EXPRLIST(arg_node);

		if (NULL != exprlist)
		{
			sizelist = VARDEC_EXPRLIST(VAR_DECL(arg_node));

			STACK *sizestack = MEMmalloc(sizeof(STACK));
			STACK *exprstack = MEMmalloc(sizeof(STACK));

			// Populate the sizestack by traversing the sizelist. But skip the first size.
			sizelist = EXPRLIST_NEXT(sizelist);
			while(NULL != sizelist)
			{
				push(sizestack, EXPRLIST_EXPR(sizelist) );
				sizelist = EXPRLIST_NEXT(sizelist);
			}

			//Populate the exprstack by traversing the exprlist.
			while(NULL != exprlist ) // We shouldnt push the last of the index as no mul is required for that.
			{
				push(exprstack, EXPRLIST_EXPR(exprlist));
				exprlist = EXPRLIST_NEXT(exprlist);
			}

			newexpr = pop(exprstack);  // this is our l in i, j, k, l indices.
			if (NULL == newexpr)
			{
				// We shouldnt be here ever.
			}

			// build newexpr something like: (p) * k + (p*o) * j + (p*o*n) * i
			firsttimeindicator = TRUE;
			while(FALSE == (isempty(sizestack)) )
			{
				sizeindex = pop(sizestack);


				//get size guy (o)
				sizeindex = pop(sizestack);

				if (TRUE == firsttimeindicator)
				{
					sizeexpr = sizeindex;
					firsttimeindicator = FALSE;
				}
				else
				{
					sizeexpr = TBmakeBinop(BO_mul, sizeexpr, sizeindex); // will compute (p*o)*n
				}

				exprindex = pop(exprstack); // get the index, like k
				currexpr = TBmakeBinop(BO_mul, sizeexpr, exprindex); // muliply with its multiplier.

				// add the two
				newexpr =  TBmakeBinop(BO_add, newexpr, currexpr);
			}

			FUNCALL_EXPRLIST(arg_node) = TBmakeExprlist(newexpr, NULL);

			//Free the stacks
			MEMfree(sizestack);
			MEMfree(exprstack);
		}


		/*---------------------------------------------------*/

		//Mark the type of the variable in the info structure, now that we know about it.
		INFO_RESULTTYPE(arg_info) = VARDEC_TYPE(VAR_DECL(arg_node));
	}
	DBUG_RETURN(arg_node);
}

node *CAnum(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAnum");
	INFO_RESULTTYPE(arg_info)=T_int;
	printf("\ninside INT");
	DBUG_RETURN(arg_node);
}

node *CAfloat(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAfloat");
	INFO_RESULTTYPE(arg_info)=T_float;
	printf("\ninside FLOAT");
	DBUG_RETURN(arg_node);
}

node *CAbool(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAbool");
	printf("\ninside INT");
	INFO_RESULTTYPE(arg_info)=T_bool;
	DBUG_RETURN(arg_node);
}

node
*CAdoContextAnalysis( node *syntaxtree)
{
	info *info;

	DBUG_ENTER("CAdoContextAnalysis");

	DBUG_ASSERT( (syntaxtree!= NULL), "SEPdoSeparateDeclInit called with empty syntaxtree");

	printf( "\n\n------------------------------\n\n");

	info = MakeInfo();

	TRAVpush( TR_ca);

	syntaxtree = TRAVdo( syntaxtree, info);

	TRAVpop();
	info = FreeInfo(info);

	printf( "\n\n------------------------------\n\n");

	DBUG_RETURN( syntaxtree);
}

