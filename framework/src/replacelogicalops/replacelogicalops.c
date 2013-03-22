
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
#include "dbug.h"
#include "memory.h"
#include "globals.h"
#include "str.h"
#include "print.h"
#include "mytypes.h"
#include "replacelogicalops.h"
 	
/*
 * INFO structure
 */
 
struct INFO
{
	bool firsterror;

	node *replacement_var;
	node *replacement_expr;
	node *replaced_statementlist, *new_vardeclist;
};

#define INFO_FIRSTERROR(n) ((n)->firsterror)
#define INFO_REPL_VAR(n)	((n)->replacement_var)
#define INFO_REPL_EXPR(n)	((n)->replacement_expr)
#define INFO_REPL_STMTS(n)  ((n)->replaced_statementlist)
#define INFO_VARDECLIST(n)	((n)->new_vardeclist)
#define NAME_COUNTER_LIMIT 50000 // less than max int though.

static info *MakeInfo()
{
	info *result;

	result = MEMmalloc(sizeof(info));

	INFO_FIRSTERROR(result) = FALSE;

	INFO_REPL_VAR(result) = NULL;

	INFO_REPL_EXPR(result) = NULL;

	INFO_REPL_STMTS(result) = NULL;

	INFO_VARDECLIST(result) = NULL;

	return result;
}

static info *FreeInfo( info *info)
{
	info = MEMfree( info);

	return info;
}

char *CONgenvarname()
{
	DBUG_ENTER("CONgetvarname");
	char szPattern[3] = "__$";
	char  *name;
	static int counter = 0;

	name=STRitoa(counter);
	name = STRcat(szPattern, name);
	
	counter = (counter + 1) % NAME_COUNTER_LIMIT;
	
	DBUG_RETURN(name);
}

void CONreplaceOr(node *boolvar, node *arg_node, info *arg_info)
{
	DBUG_ENTER("CONreplaceOr");

	node *leftexpr, *rightexpr;
    node *newstatement, *innerstatement;
    node *assigntrue, *assignfalse;

    leftexpr = BINOP_LEFT(arg_node);
    rightexpr = BINOP_RIGHT(arg_node);
    
    assigntrue = TBmakeAssign(boolvar, TBmakeBool(TRUE));
    assignfalse = TBmakeAssign(boolvar, TBmakeBool(FALSE));

    innerstatement = TBmakeIfstat(rightexpr, assigntrue, assignfalse); // if (right) __$1 = true else __$1 = false;
    newstatement = TBmakeIfstat(leftexpr, assigntrue, innerstatement); // if (left) __$1 = true else { innerstatement};

    // prepend to whatever is present previously.
    INFO_REPL_STMTS(arg_info) = TBmakeStatementlist(newstatement, INFO_REPL_STMTS(arg_info)); 

	DBUG_VOID_RETURN;
}

void CONreplaceAnd(node *boolvar, node *arg_node, info *arg_info)
{
	DBUG_ENTER("CONreplaceAnd");

	node *leftexpr, *rightexpr;
    node *newstatement, *innerstatement;
    node *assigntrue, *assignfalse;
    leftexpr = BINOP_LEFT(arg_node);
    rightexpr = BINOP_RIGHT(arg_node);

    assigntrue = TBmakeAssign(boolvar, TBmakeBool(TRUE));
    assignfalse = TBmakeAssign(boolvar, TBmakeBool(FALSE));

    innerstatement = TBmakeIfstat(rightexpr, assigntrue, NULL); // if (right) { if (left) {__$1 = true } }
    newstatement = TBmakeIfstat(leftexpr, innerstatement, assignfalse); // if (left) { innerstatement} else { __$1= false;}

    // prepend to whatever is present previously.
    INFO_REPL_STMTS(arg_info) = TBmakeStatementlist(newstatement, INFO_REPL_STMTS(arg_info)); 

	DBUG_VOID_RETURN;
}


node *CONusualdef(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAusualdef");
	
	//other contents of usualdef cannot have expressions. (export, type and var). So traversal is not required.
	USUALDEF_EXPR(arg_node) = TRAVdo(USUALDEF_EXPR(arg_node), arg_info);

	if (INFO_REPL_VAR(arg_info) != NULL)
	{
		// replace the current expression with the variable

		USUALDEF_EXPR(arg_node) = INFO_REPL_VAR(arg_info);

		INFO_REPL_VAR(arg_info) = NULL;
	}

	DBUG_RETURN(arg_node);
}

node *CONvardec(node *arg_node,info *arg_info)
{
	DBUG_ENTER("CAvardec");

	VARDEC_EXPRLIST(arg_node) = TRAVopt(VARDEC_EXPRLIST(arg_node), arg_info);

	VARDEC_VAR(arg_node) = TRAVdo(VARDEC_VAR(arg_node), arg_info);

	VARDEC_EXPR(arg_node) = TRAVopt(VARDEC_EXPR(arg_node), arg_info);

	if (VARDEC_EXPR(arg_node) != NULL)
	{
		if (INFO_REPL_VAR(arg_info) != NULL)
		{
			// replace the current expression with the variable

			VARDEC_EXPR(arg_node) = INFO_REPL_VAR(arg_info);

			INFO_REPL_VAR(arg_info) = NULL;
		}
	}

	DBUG_RETURN(arg_node);
}

node *CONifstat(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CONifstatement");

	IFSTAT_EXPR(arg_node) = TRAVdo(IFSTAT_EXPR(arg_node), arg_info);

	if (INFO_REPL_VAR(arg_info) != NULL)
	{
		// replace the current expression with the variable

		IFSTAT_EXPR(arg_node) = INFO_REPL_VAR(arg_info);

		INFO_REPL_VAR(arg_info) = NULL;
	}

	IFSTAT_BLOCK(arg_node) = TRAVopt(IFSTAT_BLOCK(arg_node), arg_info);

	IFSTAT_ELSEBLOCK(arg_node) = TRAVopt(IFSTAT_ELSEBLOCK(arg_node), arg_info);	

	DBUG_RETURN(arg_node);
}

node *CONwhilestat(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAwhilestatement");

	WHILESTAT_EXPR(arg_node) = TRAVdo(WHILESTAT_EXPR(arg_node), arg_info);

	if (INFO_REPL_VAR(arg_info) != NULL)
	{
		// replace the current expression with the variable

		WHILESTAT_EXPR(arg_node) = INFO_REPL_VAR(arg_info);

		INFO_REPL_VAR(arg_info) = NULL;
	}

	WHILESTAT_BLOCK(arg_node) = TRAVdo(WHILESTAT_BLOCK(arg_node), arg_info);	

	DBUG_RETURN(arg_node);
}

node *CONdowhilestat(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAdowhilestatement");

	DOWHILESTAT_EXPR(arg_node) = TRAVdo(DOWHILESTAT_EXPR(arg_node), arg_info);

	if (INFO_REPL_VAR(arg_info) != NULL)
	{
		// replace the current expression with the variable

		DOWHILESTAT_EXPR(arg_node) = INFO_REPL_VAR(arg_info);

		INFO_REPL_VAR(arg_info) = NULL;
	}

	DOWHILESTAT_BLOCK(arg_node) = TRAVdo(DOWHILESTAT_BLOCK(arg_node), arg_info);	

	DBUG_RETURN(arg_node);
}

node *CONforstat(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAforstatement");

	FORSTAT_VARDEC(arg_node) = TRAVdo(FORSTAT_VARDEC(arg_node), arg_info);

	FORSTAT_EXPR1(arg_node) = TRAVdo(FORSTAT_EXPR1(arg_node), arg_info);

	if (INFO_REPL_VAR(arg_info) != NULL)
	{
		// replace the current expression with the variable

		FORSTAT_EXPR1(arg_node) = INFO_REPL_VAR(arg_info);

		INFO_REPL_VAR(arg_info) = NULL;
	}

	FORSTAT_EXPR2(arg_node) = TRAVopt(FORSTAT_EXPR2(arg_node), arg_info);	

	if (FORSTAT_EXPR2(arg_node) != NULL)
	{
		if (INFO_REPL_VAR(arg_info) != NULL)
		{
			// replace the current expression with the variable

			FORSTAT_EXPR1(arg_node) = INFO_REPL_VAR(arg_info);

			INFO_REPL_VAR(arg_info) = NULL;
		}
	}

	FORSTAT_BLOCK(arg_node) = TRAVdo(FORSTAT_BLOCK(arg_node), arg_info);

	DBUG_RETURN(arg_node);
}


node *CONassign(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CONassign");

	ASSIGN_EXPR(arg_node) = TRAVdo(ASSIGN_EXPR(arg_node), arg_info);

	if (INFO_REPL_VAR(arg_info) != NULL)
	{
		// replace the current expression with the variable

		ASSIGN_EXPR(arg_node) = INFO_REPL_VAR(arg_info);

		INFO_REPL_VAR(arg_info) = NULL;
	}

	ASSIGN_VAR(arg_node) = TRAVdo(ASSIGN_VAR(arg_node), arg_info);

	DBUG_RETURN(arg_node);
}

node *CONstatementlist(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CONstatementlist");

	node *nextstats, *newstats;

	STATEMENTLIST_NEXT(arg_node)=TRAVopt(STATEMENTLIST_NEXT(arg_node),arg_info);
	STATEMENTLIST_STATEMENT(arg_node)=TRAVdo(STATEMENTLIST_STATEMENT(arg_node),arg_info);

	// We are at the point where, maybe the underlying statement has seen one or more replacements of 
	// expression(s) with the newly created boolean variables. Now, we can insert the corresponding newly
	// added "if statements".

	//We know we have reversed list of statements... ie., return statement preceeds the first statement.

	if (INFO_REPL_STMTS(arg_info) != NULL)
	{ 
		// Inserting the new statements to the original statementslist.

		nextstats = STATEMENTLIST_NEXT(arg_node);
		STATEMENTLIST_NEXT(arg_node) = INFO_REPL_STMTS(arg_info); // attaching the prev side to the new list.

		newstats = INFO_REPL_STMTS(arg_info);
		while(STATEMENTLIST_NEXT(newstats) != NULL)
		{
			newstats = STATEMENTLIST_NEXT(newstats);
		}
		STATEMENTLIST_NEXT(newstats) = nextstats; // attaching the new stats to the remaining set of statements.

		INFO_REPL_STMTS(arg_info) = NULL;	
	}

	DBUG_RETURN(arg_node);

}

node *CONcast(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CONcast");

	CAST_EXPR(arg_node) = TRAVdo(CAST_EXPR(arg_node), arg_info);

	// Cast expr can have a boolean expression, maybe to convert to int or such.
	if (INFO_REPL_VAR(arg_info) != NULL)
	{
		// replace the current expression with the variable

		CAST_EXPR(arg_node) = INFO_REPL_VAR(arg_info);

		INFO_REPL_VAR(arg_info) = NULL;
	}

	DBUG_RETURN(arg_node);
}

node *CONexprlist(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CAexprlist");

	EXPRLIST_EXPR(arg_node) = TRAVdo(EXPRLIST_EXPR(arg_node), arg_info);

	if (INFO_REPL_VAR(arg_info) != NULL)
	{
		// replace the current expression with the variable
		EXPRLIST_EXPR(arg_node) = INFO_REPL_VAR(arg_info);

		INFO_REPL_VAR(arg_info) = NULL;
	}

	EXPRLIST_NEXT(arg_node) = TRAVopt(EXPRLIST_NEXT(arg_node), arg_info);

	// Thus every expr in the exprlist will check if var is null or not in the info, and 
	// then performs the replacement if necessary.

	DBUG_RETURN(arg_node);
}

node *CONvardeclist(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CONvardeclist");

	node *nextdecls, *newdecls;

	VARDECLIST_NEXT(arg_node) = TRAVopt(VARDECLIST_NEXT(arg_node), arg_info);

	VARDECLIST_VARDEC(arg_node) = TRAVdo(VARDECLIST_VARDEC(arg_node), arg_info);

	if (INFO_VARDECLIST(arg_info) != NULL)
	{
		// Inserting the new list to the original vardeclist.

		nextdecls = VARDECLIST_NEXT(arg_node);
		VARDECLIST_NEXT(arg_node) = INFO_VARDECLIST(arg_info); // attaching the prev side to the new list.

		newdecls = INFO_VARDECLIST(arg_info);
		while(VARDECLIST_NEXT(newdecls) != NULL)
		{
			newdecls = VARDECLIST_NEXT(newdecls);
		}
		VARDECLIST_NEXT(newdecls) = nextdecls; // attaching the new decls to the remaining set of decls.

		INFO_VARDECLIST(arg_info) = NULL;	
	}

	DBUG_RETURN(arg_node);
}

node *CONbinop(node *arg_node, info *arg_info)
{
	DBUG_ENTER("CONbinop");

	BINOP_LEFT(arg_node) = TRAVdo(BINOP_LEFT(arg_node), arg_info);

	if (INFO_REPL_VAR(arg_info) != NULL)
	{
		// replace the current left expression with the variable
		BINOP_LEFT(arg_node) = INFO_REPL_VAR(arg_info);

		INFO_REPL_VAR(arg_info) = NULL;
	}

	BINOP_RIGHT(arg_node) = TRAVdo(BINOP_RIGHT(arg_node), arg_info);

	if (INFO_REPL_VAR(arg_info) != NULL)
	{
		// replace the current right expression with the variable
		BINOP_RIGHT(arg_node) = INFO_REPL_VAR(arg_info);

		INFO_REPL_VAR(arg_info) = NULL;
	}

	node *boolvar = TBmakeVar(CONgenvarname(), NULL);
    if (BO_and == BINOP_OP(arg_node))
    {
    	CONreplaceAnd(boolvar, arg_node, arg_info);
    }
    else if (BO_or == BINOP_OP(arg_node))
    {
    	CONreplaceOr(boolvar, arg_node, arg_info);
    }

    node *vardec = TBmakeVardec(T_bool, NULL, boolvar, NULL);
    INFO_VARDECLIST(arg_info) = TBmakeVardeclist(vardec, INFO_VARDECLIST(arg_info));

    INFO_REPL_VAR(arg_info) = boolvar;

    // The next guy (the parent) in the traversal, ensures to replace the 
    // current expression with the new variable.
    // We are doing this by hooking onto everywhere where expr can occur.

	DBUG_RETURN(arg_node);	
}



node
*CONdoReplaceAndnOr( node *syntaxtree)
{
	info *info;

	DBUG_ENTER("CONdoReplaceAndnOr");

	DBUG_ASSERT( (syntaxtree!= NULL), "CONdoReplaceAndnOr called with empty syntaxtree");

	printf( "\n\n------------------------------\n\n");

	info = MakeInfo();

	TRAVpush( TR_con);

	syntaxtree = TRAVdo( syntaxtree, info);

	TRAVpop();
	info = FreeInfo(info);

	printf( "\n\n------------------------------\n\n");

	DBUG_RETURN( syntaxtree);
}

