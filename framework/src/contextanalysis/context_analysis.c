
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


#include "separate_decl_init.h"
#include "traverse.h"
#include "tree_basic.h"
#include "copy.h"
#include "dbug.h"
#include "memory.h"
#include "globals.h"
#include "str.h"
#include "stack_lib.h"
#include "print.h"
#include "infostruct.h"
#include "lookup_table.h"
#include "mytypes.h"
/*
 * INFO structure
 */

 struct variablelist {
	node *varvardec;
	struct variablelist *next;
};

struct scopedvariable {
	lut_t *hash_table;
	struct scopedvariable *next;
};

typedef enum {
	variablename,fuctionname,globaldec,globalarraydec
}VARTYPE;

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
	type *returntype;
	node *var;
	node *paramlist;
};


// type of the values in the hash_tables.
struct idinfo {
	VARTYPE vartype;
	node *varnode;
	union {
		struct variableinfo *varinfo;
		struct functioninfo *funinfo;
		struct globalvariable *globalvarinfo;
		struct globalArrayvariable *globalarrayinfo;
	}info;
};


struct info
{
	bool firsterror;
	node *programhead;
	bool variablerun;
	node *programstart;
	lut_t *buildhashlist;
	char* functioname;
	struct scopedvariable *scopedlist;
	type result_type;
}

#define INFO_FIRSTERROR(n) ((n)->firsterror)
#define INFO_PROGRAMHEAD(n) ((n)->programhead)
#define INFO_VARIABLERUN(n) ((n)->variablerun)
#define INFO_PROGRAMSTART(n) ((n)->programstart)
#define INFO_FUNCTIONNAME(n) ((n)->functioname)
#define INFO_BUILDHASHTABLE(n) ((n)->buildhashlist)
#define INFO_HEADSCOPEDARRAY(n) ((n)->scopedlist)



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
	return result;
}

//List error messages


inline void printRedeclarationError(int currentLine,char *varname,int prevLine)
{
	CTIerror("Operation performed on line: %d \n reason:
				redeclaration of variable matches with var %s declared on %d",currentLine,varname,prevLine);
}

inline void CAdeclarationnotfound(int linenumber,char *varname)
{
	CTIerror("Operation performed on line: %d \n reason:
				   Declaration of the variable %s is missing",linenumber,varname);
}

static info *FreeInfo( info *info)
{
	info = MEMfree( info);

	return info;
}

void freeNode(node *arg_node)
{
	if(arg_node!=NULL)
	{
		MEMfree(arg_node);
	}
}



void freehashtable(lut_t *hash_table)
{
	DBUG_ENTER("CAfreehashtable");
	LUTremoveLut(hash_table);
	DBUG_VOID_RETURN;
}

void freescopedvariable(struct scopedvariable *scopedlist)
{
	DBUG_ENTER("SEPfreescopedvariable");
	struct scopedvariable *prevlist;
	prevlist=scopedlist;
	freehashtable(scopedlist->head);
	MEMfree(prevlist);
	DBUG_VOID_RETURN;
}



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



node *CAprogram(node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEPprogram");
	node *tail,*codeblock,*program;
	struct scopedvariable *globalarray;
	struct variablelist *backup;

	if(INFO_PROGRAMSTART(arg_info)==NULL)
	{
		INFO_PROGRAMSTART(arg_info)=arg_node;
		INFO_BUILDHASHTABLE(arg_info)=LUTgenerateLut();
	}
	
	PROGRAM_NEXT(arg_info)=TRAVopt(PROGRAM_NEXT(arg_info),arg_info);

	

	INFO_VARIABLERUN(arg_info)=true;

	PROGRAM_CODEBLOCK(arg_node)=TRAVdo(PROGRAM_CODEBLOCK(arg_node),arg_info);

	if(INFO_PROGRAMSTART(arg_node)==arg_node)
	{
		if(INFO_BUILDVARLIST!=NULL)
		{
			globalarray=MEMmalloc(sizeof(struct scopedvariable));
			globalarray->head=INFO_BUILDVARLIST(arg_info);
			globalarray->next=NULL;
			INFO_HEADSCOPEDARRAY(arg_info)=globalarray;
			INFO_BUILDHASHTABLE(arg_info)=NULL;
		}

		INFO_VARIABLERUN(arg_info)=FALSE;

		//Funbody can start processing the contents in the body
		program=arg_node;

		while(program!=NULL)
		{
			if(NODE_TYPE(PROGRAM_CODEBLOCK(program))==N_fundef || NODE_TYPE(program)==N_vardeclistlocalfundef)
			{	
				PROGRAM_CODEBLOCK(program)=TRAVdo(PROGRAM_CODEBLOCK(program),arg_info);
			}
			program=PROGRAM_NEXT(program);	
		}

	}


	/*
	 * this code will the global scope variable
	 */

	if(INFO_HEADSCOPEDARRAY(arg_info)!=NULL)
	{
		freescopedvariable(INFO_HEADSCOPEDARRAY(arg_info));
	}

	if(INFO_VARLISTNAME(arg_info)!=NULL)
	{
		freevarlistname(INFO_VARLISTNAME(arg_info));
	}


	DBUG_RETURN(arg_node);
}

node *CAvardeclistlocalfundef(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAvardeclistlocalfundef");
	if(INFO_VARIABLERUN==true)
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
	DBUG_ENTER("CAfundec");

	struct functioninfo *funinfo;
	struct idinfo *iddetails,*previddetails;
	char *funname;

 	if(INFO_VARIABLERUN(arg_info))
 	{	
 		lut_t hash_table= INFO_BUILDHASHTABLE(arg_info);	

		if(hash_table!=NULL)
		{
			funinfo=MEMmalloc(sizeof(struct functioninfo));

			//Memory used only for the structure rest reuses the memory from existing memory. Do not release it.
			funinfo->returntype=FUNHEADER_TYPE(arg_node);
			funinfo->paramlist=FUNHEADER_PARAMLIST(arg_node);
		
		
			iddetails=MEMmalloc(sizeof(struct idinfo));
			iddetails->vartype=functioname;
			iddetails->varnode=FUNHEADER_VAR(arg_node);
			iddetails->info.funinfo=funinfo;

			funname=VAR_NAME(FUNHEADER_VAR(arg_node));
			if((previddetails=LUTsearchInLutS(hash_table,funname))==NULL)
				hash_table=LUTinsertIntoLutS(hash_table,funname,iddetails);
			else
			{
				printRedeclarationError(NODE_LINE(arg_node),funname,NODE_LINE(previddetails->varnode));
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
	
		LOCALFUNDEF_FUNBODY (arg_node)=TRAVdo(LOCALFUNDEF_FUNBODY (arg_node),arg_info);
	}
	DBUG_RETURN(arg_node);
}


node *CAusualdef(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAusualdef");
	struct globalvariable *varinfo;
	struct idinfo *id,*previddetails;

	lut_t hash_table=INFO_BUILDHASHTABLE(arg_info);
	if(hash_table!=NULL)
	{
		varinfo=MEMmalloc(sizeof(struct globalvariable));
		varinfo->usual_def=arg_node;
		

		idinfo=MEMmalloc(sizeof(struct idinfo));
		id->VARTYPE=globaldec;
		id->varnode=USUALDEF_VAR(arg_node);
		id->info.globalvarinfo=varinfo;
		
		if((previddetails=LUTsearchInLutS(hash_table,VAR_NAME(id->varnode)))==NULL)
				hash_table=LUTinsertIntoLutS(hash_table,VAR_NAME(id->varnode),iddetails);
		else
		{
			printRedeclarationError(NODE_LINE(arg_node),funname,NODE_LINE(previddetails->varnode));
		}

	}
	DBUG_RETURN(arg_node);
}


node *CAarraydef(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAarraydef");
	struct globalArrayvariable *varinfo;
	struct idinfo *id,*previddetails;

	lut_t hash_table=INFO_BUILDHASHTABLE(arg_info);
	if(hash_table!=NULL)
	{
		varinfo=MEMmalloc(sizeof(struct globalArrayvariable));

		// Free ArrayDec
		varinfo->arraydef=arg_node;
		varinfo->exprlist=ARRAYDEF_EXPRLIST(arg_node);

		idinfo=MEMmalloc(sizeof(struct idinfo));
		id->VARTYPE=globalarraydec;
		id->varnode=ARRAYDEF_VAR(arg_node);
		id->info.variableinfo=varinfo;
		
		if((previddetails=LUTsearchInLutS(hash_table,VAR_NAME(id->varnode)))==NULL)
				hash_table=LUTinsertIntoLutS(hash_table,VAR_NAME(id->varnode),iddetails);
		else
		{
			printRedeclarationError(NODE_LINE(arg_node),funname,NODE_LINE(previddetails->varnode));
		}

	}
	DBUG_RETURN(arg_node);
}

node *CAvardec(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAvardec");
	struct variableinfo *varinfo;
	struct idinfo *id,*previddetails;

	lut_t hash_table=INFO_BUILDHASHTABLE(arg_info);
	if(hash_table!=NULL)
	{
		varinfo=MEMmalloc(sizeof(struct variableinfo));

		// Free ArrayDec
		varinfo->vardec=vardec;
		varinfo->exprlist=VARDEC_EXPRLIST(arg_node);

		idinfo=MEMmalloc(sizeof(struct idinfo));
		id->VARTYPE=variablename;
		id->varnode=VARDEC_VAR(arg_node);
		id->info.variableinfo=varinfo;
		
		if((previddetails=LUTsearchInLutS(hash_table,VAR_NAME(id->varnode)))==NULL)
				hash_table=LUTinsertIntoLutS(hash_table,VAR_NAME(id->varnode),iddetails);
		else
		{
			printRedeclarationError(NODE_LINE(arg_node),funname,NODE_LINE(previddetails->varnode));
		}

	}
	DBUG_RETURN(arg_node);

}


node *CAfunbody(node *arg_node,info *arg_info)
{
	struct scopedvariable *scopevariablelist;
	struct idinfo *varinfo;
	node *paramlist;
	node *param;
	lut_t hash_table;
	struct functioninfo *funinfo;
	struct variableinfo *varinfo;
	char *paramname;
	bool ishashadded=FALSE;
	struct idinfo *currentscopeinfo;

	hash_table=LUTgenerateLut();
	if(INFO_FUNCTIONNAME(arg_info)!=NULL)
	{
		scopevariablelist=INFO_HEADSCOPEDARRAY(arg_info);
		varinfo=(struct idinfo*) LUTsearchInLutS(scopevariablelist->head,INFO_FUNCTIONNAME(arg_info))
		if(info!=NULL)
		{
			funinfo=varinfo->info.functioninfo;
			paramlist=funinfo->paramlist;
			
			while(paramlist!=NULL)
			{
				param=PARAMLIST_PARAM(paramlist);
				node *vardec=TBmakeVardec(PARAM_TYPE(param),PARAM_VARORNUMLIST(param),PARAM_VAR(PARAM),NULL);
				varinfo=MEMmalloc(sizeof(struct variableinfo));
				varinfo->vardec=vardec;
				varinfo->exprlist=PARAM_VARORNUMLIST(param);
				paramname=VAR_NAME(PARAM_VAR(PARAM));
				LUTinsertIntoLutS(hash_table,paramname,varinfo);				
				paramlist=PARAMLIST_NEXT(paramlist);
			}
		}	
	}
	
	INFO_VARIABLERUN(arg_info)=TRUE;
	INFO_BUILDHASHTABLE(arg_info)=hash_table;
	FUNBODY_VARDECLIST(arg_node)=TRAVopt(FUNBODY_VARDECLIST(arg_node),arg_info);

	FUNBODY_LOCALFUNDEFLIST(arg_node)=TRAVopt(FUNBODY_LOCALFUNDEFLIST(arg_node),arg_info);
	
	//Creating a scope for this function prefixing it to the head of the list.
	if(INFO_BUILDHASHTABLE(arg_info)!=NULL)
	{
		scopedvariablelist =MEMmalloc(struct scopedvariable);
		scopedvariablelist->hash_table=INFO_BUILDHASHTABLE(arg_info);
		scopedvariablelist->next=INFO_HEADSCOPEDARRAY(arg_info);
		INFO_HEADSCOPEDARRAY(arg_info)=scopedvariablelist;
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
		currentscopeinfo=scopelist;
		INFO_HEADSCOPEDARRAY(arg_info)=scopedvariablelist->next;
		freescopedvariable(scopevariable);
	}

	DBUG_RETURN(arg_node);
}


node *CAstatementlist(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAstatementlist");
	STATEMENTLIST_NEXT(arg_node)=TRAVopt(STATEMENTLIST_NEXT(arg_node),arg_info);
	STATEMENTLIST_STATEMENT(arg_node)=TRAVopt(STATEMENTLIST_STATMENT(arg_node),arg_info);
	DBUG_RETURN(arg_node);

}



node *CAassignstatement(node *arg_node, info *arg_node)
{
	DBUG_ENTER("CAassignstatement");
	DBUG_RETURN(arg_node);
}


node *CAvar(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAvar");
	char *varname;
	lut_t hash_table;
	struct scopedvariable *scopedhashtable;
	struct idinfo *varinfo;
	bool flag=FALSE;

	scopedhashtable=INFO_HEADSCOPEDARRAY(arg_info);
	while(scopedhashtable!=NULL)
	{
		hash_table=scopedhashtable->head;
		varinfo=LUTsearchInLutS(hash_table,VAR_NAME(arg_node));
		if(varinfo!=NULL)
		{
			VARTYPE type=varinfo->type;
			if(type==functioname)
			{
				VAR_DECL(arg_node)=varinfo->info.varinfo->vardec; //Getting the vardec from the value stored in the hash table
			}
			else if (type==variablename)
			{
				VAR_DECL(arg_node)=varinfo->info.funinfo->funheader;
			}
			else if (type==globalArrayvariable)
			{
				VAR_DECL(arg_node)=varinfo->info.globalarrayinfo->arraydef;
			}
			else
			{
				VAR_DECL(arg_node)=varinfo->info.globalvariable->usual_def;
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

	DBUG_RETURN(arg_node);

}


node
*CAdoContextAnalysis( node *syntaxtree)
{
	info *info;
	node* newsyntaxtree;

	DBUG_ENTER("CAdoContextAnalysis");

	DBUG_ASSERT( (syntaxtree!= NULL), "SEPdoSeparateDeclInit called with empty syntaxtree");

	printf( "\n\n------------------------------\n\n");

	info = MakeInfo();

	TRAVpush( TR_sep);

	syntaxtree = TRAVdo( syntaxtree, info);

	TRAVpop();
	info = FreeInfo(info);

	printf( "\n\n------------------------------\n\n");

	DBUG_RETURN( newsyntaxtree);
}

