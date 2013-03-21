
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
#include "ctinfo.h"

/*
 * INFO structure
 */

static info *MakeInfo()
{
	info *result;

	result = MEMmalloc(sizeof(info));

	INFO_FIRSTERROR(result) = FALSE;

	INFO_ISGLOBAL(result) = FALSE;

	INFO_GSTATEMENTLISTHEAD(result)=NULL;

	INFO_GSTATEMENTLIST(result) = NULL;

	INFO_LSTATEMENTLIST(result) = NULL;

	INFO_GVARDECLIST(result) = NULL;

	INFO_LVARDECLIST(result) = NULL;

	INFO_PREVPARAMLIST(result) = NULL;

	INFO_FUNCTIONNAME(result)=NULL;

	INFO_VARIABLECHECK(result)=FALSE;

	INFO_VARLISTNAME(result)=NULL;

	INFO_HEADSCOPEDARRAY(result)=NULL;

	INFO_BUILDVARLIST(result)=NULL;

	INFO_SHADOWVARIABLELIST(result)=NULL;

	INFO_SHADOWVARIABLESCOPE(result)=NULL;

	INFO_APPENDSTATEMENT(result)=NULL;

	INFO_ISFUNCALLSTATEMENT(result)=FALSE;

	INFO_ENLCOSEDBLOCKCOUNT(result)=0;

	INFO_FORID(result)=NULL;

	INFO_PREVFORID(result)=NULL;

	return result;
}



static info *FreeInfo( info *info)
{
	info = MEMfree( info);

	return info;
}

// Frees single node
static void freeNode(node *arg_node)
{
	if(arg_node!=NULL)
	{
		MEMfree(arg_node);
	}
}

/*

void freevarlistname(struct varlistname *varlist)
{
	DBUG_ENTER("SEPfreevariablelistname");
	struct varlistname *prevlist;
	prevlist=varlist;

	while(varlist!=NULL)
	{
		MEMfree(varlist->id);
		varlist=varlist->next;
		MEMfree(prevlist);
		prevlist=varlist;
	}
	DBUG_VOID_RETURN;
}
 */

static void freevariablelist(struct variablelist *variable)
{
	DBUG_ENTER("SEPfreevariablelist");
	struct variablelist *prevlist;

	prevlist=variable;



	while(variable!=NULL)
	{

		freeNode(variable->varvardec);
		variable=variable->next;

		MEMfree(prevlist);

		prevlist=variable;
	}
	DBUG_VOID_RETURN;

}

// Used for arrays.
static void freescopedvariable(struct scopedvariable *scopedlist)
{
	DBUG_ENTER("SEPfreescopedvariable");
	struct scopedvariable *prevlist;
	prevlist=scopedlist;
	freevariablelist(scopedlist->head);
	MEMfree(prevlist);


	DBUG_VOID_RETURN;
}


char *SEPgenvarname(info *arg_info)
{
	DBUG_ENTER("SEPgetvarname");
	char szPattern[3] = "__";
	char  *name;
	static int counter = 0;

	name = STRcat(szPattern, STRitoa(counter));
	counter = (counter + 1) % NAME_COUNTER_LIMIT;
	DBUG_RETURN(name);
}
/*
bool ispotentialcollision(node *arg_node)
{

	DBUG_ENTER("SEPispotentialCollision");
	//TODO: later

	char *id=VAR_NAME(arg_node);
	if(STRlen(id)==4)
	{

		if(STRprefix("__",id))
		{
			if((id[2]>='0' && id[2]<='9') && (id[3]>='0' && id[3]<='9'))
			{
				printf("about to return true");
				DBUG_RETURN(TRUE);
			}
		}
	}
	DBUG_RETURN(FALSE);
}
*/

// Currently being used only for array (dimension expansion, etc) only.
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

// Check the local variables that overshadow prev.ly defined arrays
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

// Used to get the dimensions of the array from its declaration
// when we want to expand the array usage statement (like in funcall exprlist)
node* returnarraydec(node *var_node,info *arg_info)
{
	char *id,*comparedid;
	DBUG_ENTER("SEPreturnarray");

	struct scopedvariable *scoped_array;
	struct variablelist *variables;
	node* variable;

	id= VAR_NAME(var_node);
	scoped_array=INFO_HEADSCOPEDARRAY(arg_info);
	while(scoped_array!=NULL)
	{
		variables=scoped_array->head;

		while(variables!=NULL)
		{
			variable = variables->varvardec;
			comparedid=VAR_NAME(VARDEC_VAR(variable));
			if(STReq(id,comparedid))
			{
				DBUG_RETURN(variable);
			}
			variables=variables->next;
		}
		scoped_array=scoped_array->next;
	}

	DBUG_RETURN(NULL);
}



node *SEPprogram(node *arg_node, info * arg_info)
{
	node *program;
	struct scopedvariable *globalarray;
	////struct variablelist *backup;
	DBUG_ENTER("SEPprogram");

	program=arg_node;

	/*
	 * First part would be to find all the global variable and assign it to __init()
	 * to the global variable counter. This lookup is done ahead just to get idea
	 * of the variable those variable we need to create in the init function
	 *
	 */

	globalarray=MEMmalloc(sizeof(struct scopedvariable));

	INFO_VARIABLECHECK(arg_info)=TRUE;
	while(program != NULL)
	{
		if(NODE_TYPE (PROGRAM_CODEBLOCK(program))==N_vardeclistlocalfundef
				|| NODE_TYPE(PROGRAM_CODEBLOCK(program))==N_globaldec||NODE_TYPE(PROGRAM_CODEBLOCK(program))==N_globaldef)
		{

			PROGRAM_CODEBLOCK(program)=TRAVdo(PROGRAM_CODEBLOCK(program),arg_info);
		}
		else if(NODE_TYPE(PROGRAM_CODEBLOCK(program))==N_fundec)
		{
			globalarray->head=INFO_BUILDVARLIST(arg_info);
			INFO_BUILDVARLIST(arg_info)=NULL;
			PROGRAM_CODEBLOCK(program)=TRAVdo(PROGRAM_CODEBLOCK(program),arg_info);
			if(INFO_BUILDVARLIST(arg_info)!=NULL)
			{
				freevariablelist(INFO_BUILDVARLIST(arg_info));
			}
			INFO_BUILDVARLIST(arg_info)=globalarray->head;
		}

		/*
		 * The code below release the paramlist which will be a part
		 * of the localvariablelist before the next codeblock is analysed.
		 * For other function this will be cleared in the funbody.
		 */

		program=PROGRAM_NEXT(program);
	}

	INFO_VARIABLECHECK(arg_info)=FALSE;
	/*
	 * the below code create a scope for the global array elements and assign
	 * all the element found at that level.
	 */

	if(INFO_BUILDVARLIST(arg_info)!=NULL)
	{
		printf("Creating a INFOBUILD VARLIST");
		globalarray->head=INFO_BUILDVARLIST(arg_info);
		globalarray->next=INFO_HEADSCOPEDARRAY(arg_info);
		INFO_BUILDVARLIST(arg_info)=NULL;
		INFO_HEADSCOPEDARRAY(arg_info)=globalarray;
	}
	else
	{
		MEMfree(globalarray);
	}


	/*
	 *This is the place where the function body would get processed
	 */
	program=arg_node;
	while(program != NULL)
	{
		if( NODE_TYPE(PROGRAM_CODEBLOCK(program))==N_fundef || NODE_TYPE(PROGRAM_CODEBLOCK(program))==N_vardeclistlocalfundef)

		{

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

	DBUG_RETURN(arg_node);
}


/**************************************************************
 * This kicks in, when a global or definition is traversed.
 * If an initialization is detected, it is converted to
 * a declaration and an assignment is added to the
 * info structure.
 ***************************************************************/
node *SEPvardeclistlocalfundef (node *arg_node, info * arg_info)
{

	DBUG_ENTER("SEPvardeclistlocalfundef");

	//This will ensure that the global variables are
	//processed first before the functions

	if(INFO_VARIABLECHECK(arg_info))
	{
		// setting that now we are processing global declarations
		INFO_ISGLOBAL(arg_info) = TRUE;

		VARDECLISTLOCALFUNDEF_VARDECLIST(arg_node) =TRAVopt(VARDECLISTLOCALFUNDEF_VARDECLIST(arg_node),arg_info);

		INFO_ISGLOBAL(arg_info) = FALSE;
	}
	else
	{
		VARDECLISTLOCALFUNDEF_LOCALFUNDEF(arg_node) =TRAVopt(VARDECLISTLOCALFUNDEF_LOCALFUNDEF(arg_node),arg_info);
	}

	DBUG_RETURN (arg_node);
}

/**************************************************************
 * This kicks in, when a global definition is traversed.
 * If an initialization is detected, it is converted to
 * a declaration and an assignment is added to the
 * info structure.
 ***************************************************************/
node *SEPusualdef (node *arg_node, info * arg_info)
{

	node *assignment, *var, *source;
	node *statementlist,*final_statement;
	////struct varlistname *namecollision;

	DBUG_ENTER("SEPusualdef");

	USUALDEF_VAR(arg_node) =TRAVdo(USUALDEF_VAR(arg_node),arg_info);
	USUALDEF_EXPR(arg_node) =TRAVopt(USUALDEF_EXPR(arg_node),arg_info);

	if (NULL != USUALDEF_EXPR(arg_node))
	{
		source = USUALDEF_VAR(arg_node);
		var = COPYdoCopy(source);

		assignment = TBmakeAssign(var, USUALDEF_EXPR(arg_node) );
		USUALDEF_EXPR(arg_node) = NULL;
		final_statement=TBmakeStatement(assignment);

		statementlist = TBmakeStatementlist(final_statement, NULL);
		if(INFO_GSTATEMENTLIST(arg_info)!=NULL)
		{
			STATEMENTLIST_NEXT(INFO_GSTATEMENTLIST(arg_info))=statementlist;
		}
		INFO_GSTATEMENTLIST(arg_info) = statementlist;
		if(INFO_GSTATEMENTLISTHEAD(arg_info)==NULL)
		{
			INFO_GSTATEMENTLISTHEAD(arg_info)=statementlist;
		}
	}

	DBUG_RETURN (arg_node);
}


node *SEParraydef (node *arg_node, info * arg_info)
{
	node *tail,*expr_list,*new_expr_list;
	node *funvar,*funcall,*assignment_statement,*var,*vardec,*final_statement;
	struct varlistname;
	struct variablelist *list;

	DBUG_ENTER("SEParraydef");

	ARRAYDEF_EXPRLIST(arg_node) = TRAVdo(ARRAYDEF_EXPRLIST(arg_node), arg_info);

	ARRAYDEF_VAR(arg_node) = TRAVdo(ARRAYDEF_VAR(arg_node), arg_info);

	tail=NULL;
	expr_list=ARRAYDEF_EXPRLIST(arg_node);

	new_expr_list=COPYdoCopy(expr_list);
	//Pre-prepare a slot for the function call __allocate statement
	funvar = TBmakeVar(STRcpy("__allocate"), NULL);
	funcall = TBmakeFuncall(funvar, new_expr_list);

	var=COPYdoCopy(ARRAYDEF_VAR(arg_node));
	assignment_statement=TBmakeAssign(var,funcall);
	final_statement=TBmakeStatement(assignment_statement);

	tail=TBmakeStatementlist(final_statement,NULL);

	if(INFO_GSTATEMENTLIST(arg_info)!=NULL)
		STATEMENTLIST_NEXT(INFO_GSTATEMENTLIST(arg_info))=tail;

	INFO_GSTATEMENTLIST(arg_info)=tail;

	if(INFO_GSTATEMENTLISTHEAD(arg_info)==NULL)
		INFO_GSTATEMENTLISTHEAD(arg_info)=INFO_GSTATEMENTLIST(arg_info);
	/*
	 * The below code will create a list of array variable in the global scope.
	 * This will be used when we have to change the function call.
	 */
	var=COPYdoCopy(ARRAYDEF_VAR(arg_node));
	new_expr_list=COPYdoCopy(expr_list);

	vardec=TBmakeVardec(T_int,new_expr_list,var,NULL);
	list=MEMmalloc(sizeof(struct variablelist));
	printf("BUILDING INFO LIST");
	list->varvardec=vardec;
	list->next=INFO_BUILDVARLIST(arg_info);
	INFO_BUILDVARLIST(arg_info)=list;

	DBUG_RETURN (arg_node);
}


node *SEPvardec(node *arg_node,info *arg_info)
{
	node *expr_list, *new_expr_list, *funvar, *funcall, *assignment_statement;
	node *statement_list,*expr,*tail,*var,*vardec,*final_statement;
	////struct varlistname *namecollision;
	struct variablelist *list,*shadowingvariable;

	DBUG_ENTER("SEPvardec");

	VARDEC_EXPRLIST(arg_node)=TRAVopt(VARDEC_EXPRLIST(arg_node),arg_info);

	expr_list=VARDEC_EXPRLIST(arg_node);

	//For Array declaration
	tail=NULL;
	if(expr_list!=NULL)
	{

		new_expr_list=COPYdoCopy(expr_list);
		//Pre-prepare a slot for the function call __allocate statement
		funvar = TBmakeVar(STRcpy("__allocate"), NULL);
		funcall = TBmakeFuncall(funvar, new_expr_list);
		var=COPYdoCopy(VARDEC_VAR(arg_node));
		assignment_statement=TBmakeAssign(var,funcall);
		final_statement=TBmakeStatement(assignment_statement);
		tail=TBmakeStatementlist(final_statement,NULL);

		//the below code is used for adding array elements to the current list
		var=COPYdoCopy(VARDEC_VAR(arg_node));
		new_expr_list=COPYdoCopy(expr_list);
		vardec=TBmakeVardec(T_int,new_expr_list,var,NULL);
		list=MEMmalloc(sizeof(struct variablelist));
		printf("BUILDING INFO LIST");
		list->varvardec=vardec;
		list->next=INFO_BUILDVARLIST(arg_info);
		INFO_BUILDVARLIST(arg_info)=list;
	}

	expr= VARDEC_EXPR(arg_node);

	if(expr != NULL)
	{
		VARDEC_EXPR(arg_node)=NULL;
		var=COPYdoCopy(VARDEC_VAR(arg_node));
		assignment_statement=TBmakeAssign(var,expr);
		final_statement= TBmakeStatement(assignment_statement);

		if(!INFO_ISGLOBAL(arg_info))
		{
			statement_list=TBmakeStatementlist(final_statement,INFO_LSTATEMENTLIST(arg_info));
			if(tail!=NULL)
			{
				STATEMENTLIST_NEXT(statement_list)=tail;
				if(INFO_LSTATEMENTLIST(arg_info)!=NULL)
				{
					STATEMENTLIST_NEXT(INFO_LSTATEMENTLIST(arg_info))=tail;
				}
				INFO_LSTATEMENTLIST(arg_info)=tail;
				if(INFO_LSTATEMENTLISTHEAD(arg_info)==NULL)
					INFO_LSTATEMENTLISTHEAD(arg_info)=statement_list;
			}

			else
			{
				statement_list=TBmakeStatementlist(final_statement,NULL);

				if(INFO_LSTATEMENTLIST(arg_info)!=NULL)
					STATEMENTLIST_NEXT(INFO_LSTATEMENTLIST(arg_info))=statement_list;

				INFO_LSTATEMENTLIST(arg_info)=statement_list;

				if(INFO_LSTATEMENTLISTHEAD(arg_info)==NULL)
					INFO_LSTATEMENTLISTHEAD(arg_info)=INFO_LSTATEMENTLIST(arg_info);

				/*
				 * The below code will create a list of
				 * local non array variable which shadow array variable
				 */

				if(checkVariableExist(var,arg_info))
				{
					printf("\n after the call to checkvariable");
					shadowingvariable=MEMmalloc(sizeof(struct variablelist));
					shadowingvariable->varvardec=COPYdoCopy(arg_node);
					shadowingvariable->next=INFO_SHADOWVARIABLELIST(arg_info);
					INFO_SHADOWVARIABLELIST(arg_info)=shadowingvariable;
				}
			}
		}
		else
		{

			if(tail!=NULL)
			{
				CTIerror("Operation performed on line: %d \n reason:assignment is not permitted in global arrays",NODE_LINE(arg_node));
			}
			else
			{
				statement_list=TBmakeStatementlist(final_statement,NULL);

				if(INFO_GSTATEMENTLIST(arg_info)!=NULL)
					STATEMENTLIST_NEXT(INFO_GSTATEMENTLIST(arg_info))=statement_list;

				INFO_GSTATEMENTLIST(arg_info)=statement_list;

				if(INFO_GSTATEMENTLISTHEAD(arg_info)==NULL)
					INFO_GSTATEMENTLISTHEAD(arg_info)=INFO_GSTATEMENTLIST(arg_info);
			}
		}
	}
	else
	{
		if(!INFO_ISGLOBAL(arg_info))
		{

			if(tail!=NULL)
			{

				if(INFO_LSTATEMENTLIST(arg_info)!=NULL)
				{
					STATEMENTLIST_NEXT(INFO_LSTATEMENTLIST(arg_info))=tail;
				}
				INFO_LSTATEMENTLIST(arg_info)=tail;

				if(INFO_LSTATEMENTLISTHEAD(arg_info)==NULL)
					INFO_LSTATEMENTLISTHEAD(arg_info)=INFO_LSTATEMENTLIST(arg_info);
			}
			else
			{
				/*
				 * The below code will create a list of
				 * local non array variable which shadow array variable.
				 */
				var=VARDEC_VAR(arg_node);
				if(checkVariableExist(var,arg_info))
				{
					shadowingvariable=MEMmalloc(sizeof(struct variablelist));
					shadowingvariable->varvardec=COPYdoCopy(arg_node);
					shadowingvariable->next=INFO_SHADOWVARIABLELIST(arg_info);
					INFO_SHADOWVARIABLELIST(arg_info)=shadowingvariable;
				}
			}
		}
		else
		{
			if(tail!=NULL)
			{
				if(INFO_GSTATEMENTLIST(arg_info)!=NULL)
					STATEMENTLIST_NEXT(INFO_GSTATEMENTLIST(arg_info))=tail;

				INFO_GSTATEMENTLIST(arg_info)=tail;

				if(INFO_GSTATEMENTLISTHEAD(arg_info)==NULL)
					INFO_GSTATEMENTLISTHEAD(arg_info)=INFO_GSTATEMENTLIST(arg_info);
			}
		}
	}
	VARDEC_VAR(arg_node)=TRAVdo(VARDEC_VAR(arg_node), arg_info);

	DBUG_RETURN (arg_node);
}

node *SEPparamlist (node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEPparamlist");
	node *param,*varnumlist,*newParamlist,*varornum,*tail,*newParam,*var;
	node *prevparamlist;
	struct variablelist *shadowingvariable;

	PARAMLIST_PARAM(arg_node) =TRAVdo(PARAMLIST_PARAM(arg_node),arg_info);

	//// PARAMLIST_NEXT(arg_node) = TRAVopt(PARAMLIST_NEXT(arg_node), arg_info);

	param=PARAMLIST_PARAM(arg_node);
	var=PARAM_VAR(param);
	if(checkVariableExist(var,arg_info))
	{
		shadowingvariable=MEMmalloc(sizeof(struct variablelist));
		shadowingvariable->varvardec=COPYdoCopy(var);
		shadowingvariable->next=INFO_SHADOWVARIABLELIST(arg_info);
		INFO_SHADOWVARIABLELIST(arg_info)=shadowingvariable;
	}
	// If the param has array dimension specification:
	if(PARAM_VARNUMLIST(param) != NULL)
	{

		varnumlist = PARAM_VARNUMLIST(param);


		newParamlist=NULL;

		while(varnumlist!=NULL)
		{

			varornum=VARNUMLIST_VARORNUM(varnumlist);
			if(NODE_TYPE(varornum)==N_var)
			{

				var=COPYdoCopy(varornum);

				newParam=TBmakeParam(T_int,NULL,var);

				if(checkVariableExist(var,arg_info)==TRUE)
				{

					shadowingvariable=MEMmalloc(sizeof(struct variablelist));
					shadowingvariable->varvardec=COPYdoCopy(var);
					shadowingvariable->next=INFO_SHADOWVARIABLELIST(arg_info);
					INFO_SHADOWVARIABLELIST(arg_info)=shadowingvariable;
				}

				if(newParamlist==NULL)
				{
					// first time we enter the loop, set the tail.
					newParamlist=TBmakeParamlist(newParam,newParamlist);
					tail = newParamlist;

				}
				else
				{
					newParamlist=TBmakeParamlist(newParam,NULL);
					PARAMLIST_NEXT(prevparamlist)=newParamlist;
					printf("\nafter paramlist");
				}
				prevparamlist=newParamlist;
			}

			varnumlist=VARNUMLIST_NEXT(varnumlist);
		}

		//eg int a, int [m,n]b, int c => c->b->n->m->a. the below code does this insertion
		PARAMLIST_NEXT(prevparamlist)=PARAMLIST_NEXT(arg_node); // m->a
		PARAMLIST_NEXT(arg_node)=tail; // b->n
	}

	INFO_PREVPARAMLIST(arg_info)=arg_node;

	PARAMLIST_NEXT(arg_node) = TRAVopt(PARAMLIST_NEXT(arg_node), arg_info);

	DBUG_RETURN (arg_node);
}


/*******************************************************************
 Hooks into funbody traversal to work on vardeclist transformations
 ********************************************************************/
node *SEPfunbody (node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEPfunbody");
	struct scopedvariable *functionscopearray,*functionscopedshadow;

	bool createscopearray,createscopeshadow;

	createscopearray=FALSE;
	createscopeshadow=FALSE;

	FUNBODY_VARDECLIST(arg_node) =TRAVopt(FUNBODY_VARDECLIST(arg_node),arg_info);

	printf("\n FINISHED TRAVERSING VARDECLIST in function body");

	/*
	 * Do not include the compiler defined variables in this list
	 */

	if(INFO_BUILDVARLIST(arg_info)!=NULL)
	{
		printf("\n creating a localdef");
		functionscopearray=MEMmalloc(sizeof(struct scopedvariable));
		functionscopearray->head=INFO_BUILDVARLIST(arg_info);
		functionscopearray->next=INFO_HEADSCOPEDARRAY(arg_info);
		INFO_HEADSCOPEDARRAY(arg_info)=functionscopearray;
		/*
		 * Memory released will be done when the scope variable is released.
		 */
		INFO_BUILDVARLIST(arg_info)=NULL;
		createscopearray=TRUE;
		printf("\n done with a localdef");
	}


	if(INFO_SHADOWVARIABLELIST(arg_info)!=NULL)
	{
		printf("\n creating a shadow variable");
		functionscopedshadow=MEMmalloc(sizeof(struct scopedvariable));
		functionscopedshadow->head=INFO_SHADOWVARIABLELIST(arg_info);
		functionscopedshadow->next=INFO_SHADOWVARIABLESCOPE(arg_info);
		INFO_SHADOWVARIABLESCOPE(arg_info)=functionscopedshadow;
		/*
		 * Memory released will be done when the scope variable is released.
		 */
		INFO_SHADOWVARIABLELIST(arg_info)=NULL;
		createscopeshadow=TRUE;
		printf("\n done with a shadow variable");
	}


	FUNBODY_STATEMENTLIST(arg_node) = TRAVopt(FUNBODY_STATEMENTLIST(arg_node), arg_info);
	//NO statement case
	if(INFO_LSTATEMENTLISTHEAD(arg_info) != NULL)
	{
		FUNBODY_STATEMENTLIST(arg_node)=INFO_LSTATEMENTLISTHEAD(arg_info);
		INFO_LSTATEMENTLISTHEAD(arg_info)=NULL;
		INFO_LSTATEMENTLIST(arg_info) = NULL;
	}
	printf("\n after adding stmt");


	// prepend the newly formed local vardeclist to the funbody's vardeclist.
	if(INFO_LVARDECLIST(arg_info)!=NULL)
	{
		VARDECLIST_NEXT(INFO_LVARDECLIST(arg_info)) = FUNBODY_VARDECLIST(arg_node);
		FUNBODY_VARDECLIST(arg_node)=INFO_LVARDECLIST(arg_info);
	}
	INFO_LVARDECLIST(arg_info) = NULL;

	FUNBODY_LOCALFUNDEFLIST(arg_node) = TRAVopt(FUNBODY_LOCALFUNDEFLIST(arg_node), arg_info);

	if(createscopearray)
	{
		printf("\n inside scoped array");
		functionscopearray=INFO_HEADSCOPEDARRAY(arg_info);
		INFO_HEADSCOPEDARRAY(arg_info)=INFO_HEADSCOPEDARRAY(arg_info)->next;
		freescopedvariable(functionscopearray);
		printf("\n After scoped array");
		functionscopearray=NULL;
	}

	if(createscopeshadow)
	{
		printf("\n inside shadow array");
		functionscopedshadow=INFO_SHADOWVARIABLESCOPE(arg_info);
		INFO_SHADOWVARIABLESCOPE(arg_info)=INFO_SHADOWVARIABLESCOPE(arg_info)->next;
		freescopedvariable(functionscopedshadow);

		functionscopedshadow=NULL;
		printf("\n removing shadow");
	}

	DBUG_RETURN (arg_node);
}


node *SEPenclosedblock(node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEPstatementlist");
	INFO_ENLCOSEDBLOCKCOUNT(arg_info)++;
	ENCLOSEDBLOCK_STATEMENTLIST(arg_node)=TRAVdo(ENCLOSEDBLOCK_STATEMENTLIST(arg_node),arg_info);
	INFO_ENLCOSEDBLOCKCOUNT(arg_info)--;
	DBUG_RETURN(arg_node);
}

node *SEPstatementlist (node *arg_node, info * arg_info)
{
	node *statementlist;
	DBUG_ENTER("SEPstatementlist");

	STATEMENTLIST_STATEMENT(arg_node) = TRAVdo(STATEMENTLIST_STATEMENT(arg_node), arg_info);

	/*
	 * the following code will append assignment statement in for loops
	 */

	if(INFO_APPENDSTATEMENT(arg_info)!=NULL)
	{
		printf("\ninside append statement");
		statementlist=TBmakeStatementlist(INFO_APPENDSTATEMENT(arg_info),STATEMENTLIST_NEXT(arg_node));
		STATEMENTLIST_NEXT(arg_node)=statementlist;
		INFO_APPENDSTATEMENT(arg_info)=NULL;
	}
	else
		statementlist=arg_node;

	STATEMENTLIST_NEXT(arg_node) = TRAVopt(STATEMENTLIST_NEXT(arg_node), arg_info);

	if(NULL == STATEMENTLIST_NEXT(arg_node) && INFO_ENLCOSEDBLOCKCOUNT(arg_info)==0)
	{
		STATEMENTLIST_NEXT(arg_node) = INFO_LSTATEMENTLISTHEAD(arg_info);
		INFO_LSTATEMENTLIST(arg_info) = NULL;
		INFO_LSTATEMENTLISTHEAD(arg_info)=NULL;
	}

	DBUG_RETURN (arg_node);
}

/*
node *SEPstatement(node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEPStatement");
	if(NODE_TYPE (STATEMENT_SET_STATEMENT(arg_node)) == N_forstat)
	{
		TRAVpush(TR_rename);
		STATEMENT_SET_STATEMENT(arg_node) =TRAVdo(STATEMENT_SET_STATEMENT(arg_node),arg_info);
		TRAVpop();
	}
	else
		STATEMENT_SET_STATEMENT(arg_node) =TRAVdo(STATEMENT_SET_STATEMENT(arg_node),arg_info);
	DBUG_RETURN(arg_node);
}
 */

node *SEPforstat (node *arg_node,info *arg_info)
{
	node *forinit,*varnode,*varnodecopy,*vardec,*vardeccopy,*vardeclist,*expr,*assign,*statement;
	////node *statementlist;
	char *id;
	char *parentid,*parentprevid;
	DBUG_ENTER("SEPforstat");

	/*
	 *  the block will have to be traversed first so that the order of
	 * statement assignment is maintained in case of nested for loops
	 *
	 */
	forinit=FORSTAT_VARDEC(arg_node);
	id=SEPgenvarname(arg_info);
	parentid=INFO_FORID(arg_info);
	parentprevid=INFO_PREVFORID(arg_info);
	INFO_PREVFORID(arg_info)=VAR_NAME(VARDEC_VAR(forinit));
	INFO_FORID(arg_info)=id;
	FORSTAT_BLOCK(arg_node) = TRAVdo(FORSTAT_BLOCK(arg_node), arg_info);


	TRAVpush(TR_rename);
	//this is for renaming
	FORSTAT_BLOCK(arg_node) = TRAVdo(FORSTAT_BLOCK(arg_node), arg_info);

	//FORSTAT_VARDEC(arg_node)=TRAVdo(FORSTAT_VARDEC(arg_node),arg_info);
	//if(VARDEC_EXPR(forinit)==NULL)
	//printf("\nEXPR NULL");
	expr=COPYdoCopy(VARDEC_EXPR(forinit));
	expr=TRAVopt(expr,arg_info);

	varnode=TBmakeVar(id,NULL);
	vardec=TBmakeVardec(T_int,NULL,varnode,NULL);
	vardeclist=TBmakeVardeclist(vardec,INFO_LVARDECLIST(arg_info));
	INFO_LVARDECLIST(arg_info)=vardeclist;

	vardeccopy=COPYdoCopy(vardec);

	FORSTAT_VARDEC(arg_node)=vardeccopy;

	varnodecopy=COPYdoCopy(varnode);

	assign=TBmakeAssign(varnodecopy,expr);
	statement=TBmakeStatement(assign);
	INFO_APPENDSTATEMENT(arg_info)=statement;

	FORSTAT_EXPR1(arg_node)=TRAVdo(FORSTAT_EXPR1(arg_node),arg_info);
	FORSTAT_EXPR2(arg_node)=TRAVopt(FORSTAT_EXPR2(arg_node),arg_info);

	TRAVpop();

	INFO_PREVFORID(arg_info)=parentprevid;
	INFO_FORID(arg_info)=parentid;
	freeNode(forinit);
	DBUG_RETURN (arg_node);
}


node *SEPfuncall (node *arg_node, info * arg_info)
{
	node *exprlist,*expr,*arraydec,*vardecexprlist,*vardecexpr,*tail,*newexpr;

	node *prevexprlist=NULL,*newexprlist,*prevvardeclist;

	bool runmade=FALSE;

	DBUG_ENTER("SEPfuncall");

	exprlist=FUNCALL_EXPRLIST(arg_node);


	tail=NULL;

	while(exprlist!=NULL)
	{
		expr=EXPRLIST_EXPR(exprlist);

		if((NODE_TYPE (expr) == N_var))
		{
			printf("\nXXX %s",VAR_NAME(expr));

			arraydec=returnarraydec(expr,arg_info);
			if(VAR_EXPRLIST(expr)==NULL)
			{
				printf ("\nFor array %s expr list is NULL",VAR_NAME(expr));
			}
			if(arraydec!=NULL && VAR_EXPRLIST(expr)==NULL)
			{
				printf("\nName %s",VAR_NAME(VARDEC_VAR(arraydec)));

				if(INFO_SHADOWVARIABLESCOPE(arg_info)==NULL || !checkshadow(VARDEC_VAR(arraydec),arg_info))
				{
					vardecexprlist=VARDEC_EXPRLIST(arraydec);

					int i=0;
					prevvardeclist=NULL;
					while(vardecexprlist!=NULL)
					{
						vardecexpr=EXPRLIST_EXPR(vardecexprlist);
						newexpr=COPYdoCopy(vardecexpr);
						newexprlist=TBmakeExprlist(newexpr,NULL);
						if(runmade==FALSE)
						{
							printf("In runmade i:%d",++i);
							tail=newexprlist;
						}
						else if(prevvardeclist==NULL)
						{
							printf("In else if i:%d",++i);
							EXPRLIST_NEXT(prevexprlist)=newexprlist;
						}
						else
						{
							printf("In else i:%d",++i);
							EXPRLIST_NEXT(prevvardeclist)=newexprlist;
						}
						prevvardeclist=newexprlist;
						vardecexprlist=EXPRLIST_NEXT(vardecexprlist);
						runmade=TRUE;
					}

					EXPRLIST_NEXT(newexprlist)=exprlist;
				}
			}

		}

		runmade=TRUE;
		prevexprlist=exprlist;
		exprlist=EXPRLIST_NEXT(exprlist);
	}
	if(tail!=NULL)
	{
		FUNCALL_EXPRLIST(arg_node)=tail;
	}
	if(FUNCALL_EXPRLIST(arg_node)!=NULL)
		PRTdoPrint(FUNCALL_EXPRLIST(arg_node));

	DBUG_RETURN (arg_node);
}

/******************************************************************************
   Function that adds __init() to the syntax tree
   NOTE: syntax tree is modified by this function 
 *******************************************************************************/

node *SEPintroduceInit(node *syntaxtree, info *info)
{
	node *var, *header, *funbody, *init;
	node *vardeclistfundef, *program;
	if ( NULL == syntaxtree || NULL == INFO_GSTATEMENTLIST(info))
	{
		return syntaxtree;
	}

	var     = TBmakeVar(STRcpy("__init"), NULL);
	header  = TBmakeFunheader(T_void, var, NULL);
	funbody = TBmakeFunbody(NULL, NULL, INFO_GSTATEMENTLISTHEAD(info), NULL);
	PRTdoPrint(INFO_GSTATEMENTLISTHEAD(info));
	init = TBmakeLocalfundef(header, funbody);
	INFO_GSTATEMENTLIST(info)=NULL;
	INFO_GSTATEMENTLISTHEAD(info)=NULL;
	vardeclistfundef = TBmakeVardeclistlocalfundef(NULL, init);
	program = TBmakeProgram(vardeclistfundef, syntaxtree);
	return program;
}

node
*SEPdoSeparateDeclInit( node *syntaxtree)
{
	info *info;
	node* newsyntaxtree;

	DBUG_ENTER("SEPdoSeparateDeclInit");

	DBUG_ASSERT( (syntaxtree!= NULL), "SEPdoSeparateDeclInit called with empty syntaxtree");

	printf( "\n\n------------------------------\n\n");

	info = MakeInfo();

	TRAVpush( TR_sep);

	syntaxtree = TRAVdo( syntaxtree, info);

	TRAVpop();

	newsyntaxtree = SEPintroduceInit(syntaxtree, info);

	info = FreeInfo(info);

	printf( "\n\n------------------------------\n\n");

	DBUG_RETURN( newsyntaxtree);
}

/**
 * @}
 */



