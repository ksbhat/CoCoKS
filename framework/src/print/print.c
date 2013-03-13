
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


#include "print.h"
#include "traverse.h"
#include "tree_basic.h"
#include "dbug.h"
#include "memory.h"
#include "globals.h"

/*
 * INFO structure
 */
struct INFO {
  bool firsterror;
};

struct TABSTACK {
  int  n_count; // number of tabs to be prefixed.
} tabstack = { 0 };

#define TABSTACK_MAXSIZE 100

#define INFO_FIRSTERROR(n) ((n)->firsterror)

#define PRINTNODE(msg)	\
    printf("\n");       \
    puttabs();          \
    printf("=>\t[[ %s ]]\t", msg);

#define PRINTTYPE(type)   \
    switch ( type )       \
    {                     \
      case T_int    : printf(" type:int "); break;   \
      case T_bool   : printf(" type:bool "); break;  \
      case T_float  : printf(" type:float "); break; \
      case T_void   : printf(" type:void "); break;  \
      case T_unknown: printf(" type:unknown detected! "); \
    }

#define PRINTEXTERN(value)   \
    switch ( value )       \
    {                     \
      case FALSE  : printf(" non-extern "); break;   \
      case TRUE   : printf(" extern "); break;  \
    }                     

#define PRINTEXPORT(value)   \
    switch ( value )       \
    {                     \
      case FALSE  : printf(" non-export "); break;   \
      case TRUE   : printf(" export "); break;  \
    } 

#define PRELOGUE  push();

#define EPILOGUE  pop();

void push()
{
  if ( TABSTACK_MAXSIZE > tabstack.n_count)
  {
    tabstack.n_count++;
  }
}

void pop()
{
  if ( 0 < tabstack.n_count)
  {
    tabstack.n_count--;
  }
}

void puttabs()
{
  int i;

  for(i=0; i<tabstack.n_count; i++)
  {
    printf("\t");
  }
  return ;
}

static info *MakeInfo()
{
  info *result;
  
  result = MEMmalloc(sizeof(info));

  INFO_FIRSTERROR(result) = FALSE;
  
  return result;
}


static info *FreeInfo( info *info)
{
  info = MEMfree( info);

  return info;
}

/** <!--******************************************************************-->
 *
 * @fn PRTinstr
 *
 * @brief Prints the node and its sons/attributes
 *
 * @param arg_node BinOp node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/

node *
PRTassign (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTassign");

  PRINTNODE("Assign");

  PRELOGUE

  if (ASSIGN_VAR( arg_node) != NULL) {
    ASSIGN_VAR( arg_node) = TRAVdo( ASSIGN_VAR( arg_node), arg_info);
    printf( " = ");
  }
  
  ASSIGN_EXPR( arg_node) = TRAVdo( ASSIGN_EXPR( arg_node), arg_info);
  
  printf( ";\n");
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}


/** <!--******************************************************************-->
 *
 * @fn PRTbinop
 *
 * @brief Prints the node and its sons/attributes
 *
 * @param arg_node BinOp node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/

node *
PRTbinop (node * arg_node, info * arg_info)
{
  char *tmp;

  DBUG_ENTER ("PRTbinop");

  PRINTNODE("BinOp");

  PRELOGUE

  printf( "( ");

  BINOP_LEFT( arg_node) = TRAVdo( BINOP_LEFT( arg_node), arg_info);

  switch (BINOP_OP( arg_node)) {
    case BO_add:
      tmp = "+";
      break;
    case BO_sub:
      tmp = "-";
      break;
    case BO_mul:
      tmp = "*";
      break;
    case BO_div:
      tmp = "/";
      break;
    case BO_mod:
      tmp = "%";
      break;
    case BO_lt:
      tmp = "<";
      break;
    case BO_le:
      tmp = "<=";
      break;
    case BO_gt:
      tmp = ">";
      break;
    case BO_ge:
      tmp = ">=";
      break;
    case BO_eq:
      tmp = "==";
      break;
    case BO_ne:
      tmp = "!=";
      break;
    case BO_or:
      tmp = "||";
      break;
    case BO_and:
      tmp = "&&";
      break;
    case BO_unknown:
      DBUG_ASSERT( 0, "unknown binop detected!");
  }

  printf( " %s ", tmp);

  BINOP_RIGHT( arg_node) = TRAVdo( BINOP_RIGHT( arg_node), arg_info);

  printf( ")");
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}


/** <!--******************************************************************-->
 *
 * @fn PRTfloat
 *
 * @brief Prints the node and its sons/attributes
 *
 * @param arg_node Float node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/

node *
PRTfloat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTfloat");

  PRELOGUE

  printf( "%f", FLOAT_VALUE( arg_node));
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}



/** <!--******************************************************************-->
 *
 * @fn PRTnum
 *
 * @brief Prints the node and its sons/attributes
 *
 * @param arg_node Num node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/

node *
PRTnum (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTnum");

  PRELOGUE

  printf( "%i", NUM_VALUE( arg_node));
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}


/** <!--******************************************************************-->
 *
 * @fn PRTboolean
 *
 * @brief Prints the node and its sons/attributes
 *
 * @param arg_node Boolean node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/

node *
PRTbool (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTbool");

  PRELOGUE

  if (BOOL_VALUE( arg_node)) {
    printf( "true");
  }
  else {
    printf( "false");
  }
  
  EPILOGUE
  
  DBUG_RETURN (arg_node);
}


/** <!--******************************************************************-->
 *
 * @fn PRTvar
 *
 * @brief Prints the node and its sons/attributes
 *
 * @param arg_node letrec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/

node *
PRTvar (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTvar");
  
  PRINTNODE("Var");

  PRELOGUE
  
  printf( "%s", VAR_NAME( arg_node));

  VAR_EXPRLIST(arg_node) = TRAVopt(VAR_EXPRLIST(arg_node), arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}


/** <!--******************************************************************-->
 *
 * @fn PRTerror
 *
 * @brief Prints the node and its sons/attributes
 *
 * @param arg_node letrec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/

node *
PRTerror (node * arg_node, info * arg_info)
{
  bool first_error;

  DBUG_ENTER ("PRTerror");

  PRELOGUE
  
  if (NODE_ERROR (arg_node) != NULL) {
    NODE_ERROR (arg_node) = TRAVdo (NODE_ERROR (arg_node), arg_info);
  }

  first_error = INFO_FIRSTERROR( arg_info);

  if( (global.outfile != NULL)
      && (ERROR_ANYPHASE( arg_node) == global.compiler_anyphase)) {

    if ( first_error) {
      printf ( "\n/******* BEGIN TREE CORRUPTION ********\n");
      INFO_FIRSTERROR( arg_info) = FALSE;
    }

    printf ( "%s\n", ERROR_MESSAGE( arg_node));

    if (ERROR_NEXT (arg_node) != NULL) {
      TRAVopt (ERROR_NEXT (arg_node), arg_info);
    }

    if ( first_error) {
      printf ( "********  END TREE CORRUPTION  *******/\n");
      INFO_FIRSTERROR( arg_info) = TRUE;
    }
  }
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}



node *PRTstatement(node * arg_node, info * arg_info)
{
  DBUG_ENTER("PRTstatement");
  
  PRINTNODE("Statement");

  PRELOGUE  

  STATEMENT_SET_STATEMENT(arg_node) =TRAVdo(STATEMENT_SET_STATEMENT(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}

node *PRTprogram (node *arg_node, info * arg_info)
{
	DBUG_ENTER("Program");

  PROGRAM_NEXT(arg_node) =TRAVopt(PROGRAM_NEXT(arg_node),arg_info);
  
	PRINTNODE("Program");

  PRELOGUE
  
	PROGRAM_CODEBLOCK(arg_node) =TRAVdo(PROGRAM_CODEBLOCK(arg_node),arg_info);
	
  EPILOGUE

  DBUG_RETURN (arg_node);
}

node *PRTvardec (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTvardec");

  PRINTNODE("VarDec");

  PRELOGUE
  
  PRINTTYPE(VARDEC_TYPE(arg_node));

  VARDEC_VAR(arg_node) =TRAVdo(VARDEC_VAR(arg_node),arg_info);

  VARDEC_EXPRLIST(arg_node) =TRAVopt(VARDEC_EXPRLIST(arg_node),arg_info);

  VARDEC_EXPR(arg_node) =TRAVopt(VARDEC_EXPR(arg_node),arg_info);
  
  EPILOGUE
  
  DBUG_RETURN (arg_node);
}
node *PRTparam (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTparam");

  PRINTNODE("Param");

  PRELOGUE
  
  PRINTTYPE(PARAM_TYPE(arg_node));

  PARAM_VARNUMLIST(arg_node) =TRAVopt(PARAM_VARNUMLIST(arg_node),arg_info);

  PARAM_VAR(arg_node) =TRAVdo(PARAM_VAR(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}

node *PRTfundec (node *arg_node, info * arg_info)
{
	DBUG_ENTER("PRTfundec");

  PRINTNODE("FunDec");

  PRELOGUE
  
  PRINTEXTERN(FUNDEC_EXTERN(arg_node));
  
  FUNDEC_FUNHEADER(arg_node) =TRAVdo(FUNDEC_FUNHEADER(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTfundef (node *arg_node, info * arg_info)
{
	DBUG_ENTER("PRTfundef");

  PRINTNODE("FunDef");

  PRELOGUE
  
  PRINTEXPORT(FUNDEF_EXPORT(arg_node));
  
  FUNDEF_LOCALFUNDEF(arg_node) =TRAVdo(FUNDEF_LOCALFUNDEF(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTparamlist (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTparamlist");

  PARAMLIST_NEXT(arg_node) =TRAVopt(PARAMLIST_NEXT(arg_node),arg_info);

  PRINTNODE("ParamList");

  PRELOGUE
  
  PARAMLIST_PARAM(arg_node) =TRAVdo(PARAMLIST_PARAM(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTfunheader (node *arg_node, info * arg_info)
{
	DBUG_ENTER("PRTfunheader");

  PRINTNODE("FunHeader");

  PRELOGUE
  
  PRINTTYPE(FUNHEADER_TYPE(arg_node));
  
  FUNHEADER_VAR(arg_node) =TRAVdo(FUNHEADER_VAR(arg_node),arg_info);

  FUNHEADER_PARAMLIST(arg_node) =TRAVopt(FUNHEADER_PARAMLIST(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);

}
node *PRTlocalfundef (node *arg_node, info * arg_info)
{
	DBUG_ENTER("PRTlocalfundef");

  PRINTNODE("LocalFunDef");

  PRELOGUE
  
  LOCALFUNDEF_FUNHEADER(arg_node) =TRAVdo(LOCALFUNDEF_FUNHEADER(arg_node),arg_info);
  
  LOCALFUNDEF_FUNBODY(arg_node) =TRAVdo(LOCALFUNDEF_FUNBODY(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTfunbody (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTfunbody");

  PRINTNODE("FunBody");

  PRELOGUE
  
  FUNBODY_VARDECLIST(arg_node) =TRAVopt(FUNBODY_VARDECLIST(arg_node),arg_info);
  
  FUNBODY_LOCALFUNDEFLIST(arg_node) =TRAVopt(FUNBODY_LOCALFUNDEFLIST(arg_node),arg_info);

  FUNBODY_STATEMENTLIST(arg_node) =TRAVopt(FUNBODY_STATEMENTLIST(arg_node),arg_info);

  FUNBODY_RETURN(arg_node) =TRAVopt(FUNBODY_RETURN(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTlocalfundeflist (node *arg_node, info * arg_info)
{
	DBUG_ENTER("PRTlocalfundeflist");

  LOCALFUNDEFLIST_NEXT(arg_node) =TRAVopt(LOCALFUNDEFLIST_NEXT(arg_node),arg_info);

  PRINTNODE("LocalFunDefList");

  PRELOGUE
  
  LOCALFUNDEFLIST_LOCALFUNDEF(arg_node) =TRAVdo(LOCALFUNDEFLIST_LOCALFUNDEF(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTglobaldef (node *arg_node, info * arg_info)
{
	DBUG_ENTER ("PRTglobaldef");

	PRINTNODE("global def");

  PRELOGUE
  
  GLOBALDEF_SET_GLOBALDEF( arg_node) = TRAVdo( GLOBALDEF_SET_GLOBALDEF(arg_node), arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTusualdef (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTusualdef");

  PRINTNODE("UsualDef");

  PRELOGUE
  
  PRINTEXPORT(USUALDEF_EXPORT(arg_node));

  PRINTTYPE(USUALDEF_TYPE(arg_node));
  
  USUALDEF_VAR(arg_node) =TRAVdo(USUALDEF_VAR(arg_node),arg_info);
	
	USUALDEF_EXPR(arg_node) =TRAVopt(USUALDEF_EXPR(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTarraydef (node *arg_node, info * arg_info)
{
	DBUG_ENTER("PRTarraydef");

	PRINTNODE("ArrayDef");

  PRELOGUE
  
	PRINTEXPORT(ARRAYDEF_EXPORT(arg_node));

  PRINTTYPE(ARRAYDEF_TYPE(arg_node));

	ARRAYDEF_EXPRLIST(arg_node) = TRAVdo(ARRAYDEF_EXPRLIST(arg_node), arg_info);

	ARRAYDEF_VAR(arg_node) = TRAVdo(ARRAYDEF_VAR(arg_node), arg_info);
  
  EPILOGUE

	DBUG_RETURN (arg_node);
}
node *PRTglobaldec (node *arg_node, info * arg_info)
{
	DBUG_ENTER("PRTglobaldec");

  PRINTNODE("GlobalDec");

  PRELOGUE
  
  PRINTEXTERN(GLOBALDEC_EXTERN(arg_node));

  PRINTTYPE(GLOBALDEC_TYPE(arg_node));

  GLOBALDEC_VARNUMLIST(arg_node) = TRAVopt(GLOBALDEC_VARNUMLIST(arg_node), arg_info);

  GLOBALDEC_VAR(arg_node) = TRAVdo(GLOBALDEC_VAR(arg_node), arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTvarnumlist (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTvarnumlist");

  VARNUMLIST_NEXT(arg_node) = TRAVopt(VARNUMLIST_NEXT(arg_node), arg_info);

  PRINTNODE("VarNumList");

  PRELOGUE
  
  VARNUMLIST_VARORNUM(arg_node) = TRAVdo(VARNUMLIST_VARORNUM(arg_node), arg_info);

  EPILOGUE  

  DBUG_RETURN (arg_node);
}
node *PRTvardeclist (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTvardeclist");

  VARDECLIST_NEXT(arg_node) =TRAVopt(VARDECLIST_NEXT(arg_node),arg_info);

  PRINTNODE("VarDecList");

  PRELOGUE
  
  VARDECLIST_VARDEC(arg_node) =TRAVdo(VARDECLIST_VARDEC(arg_node),arg_info);

  EPILOGUE
  
  DBUG_RETURN (arg_node);
}
node *PRTreturn (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTreturn");

  PRINTNODE("Return");

  PRELOGUE
  
  RETURN_EXPR(arg_node) =TRAVdo(RETURN_EXPR(arg_node),arg_info);
  
  EPILOGUE
  
  DBUG_RETURN (arg_node);
}
node *PRTstatementlist (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTstatementlist");

  STATEMENTLIST_NEXT(arg_node) =TRAVopt(STATEMENTLIST_NEXT(arg_node),arg_info);

  PRINTNODE("StatementList");

  PRELOGUE
  
  STATEMENTLIST_STATEMENT(arg_node) =TRAVdo(STATEMENTLIST_STATEMENT(arg_node),arg_info);

  EPILOGUE
  
  //EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTdowhilestat (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTdowhilestat");

  PRINTNODE("DoWhileStat");

  PRELOGUE
  
  DOWHILESTAT_BLOCK(arg_node) =TRAVdo(DOWHILESTAT_BLOCK(arg_node),arg_info);

  DOWHILESTAT_EXPR(arg_node) =TRAVdo(DOWHILESTAT_EXPR(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTwhilestat (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTwhilestat");

  PRINTNODE("WhileStat");

  PRELOGUE
  
  WHILESTAT_EXPR(arg_node) =TRAVdo(WHILESTAT_EXPR(arg_node),arg_info);

  WHILESTAT_BLOCK(arg_node) =TRAVdo(WHILESTAT_BLOCK(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);

}
node *PRTforstat (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTforstat");

  PRINTNODE("ForStat");

  PRELOGUE
  
  FORSTAT_VARDEC(arg_node) =TRAVdo(FORSTAT_VARDEC(arg_node),arg_info);

  FORSTAT_EXPR1(arg_node) =TRAVdo(FORSTAT_EXPR1(arg_node),arg_info);

  FORSTAT_EXPR2(arg_node) =TRAVopt(FORSTAT_EXPR2(arg_node),arg_info);

  FORSTAT_BLOCK(arg_node) =TRAVdo(FORSTAT_BLOCK(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTfuncall (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTfuncall");

  PRINTNODE("FunCall");

  PRELOGUE
  
  FUNCALL_VAR(arg_node) =TRAVdo(FUNCALL_VAR(arg_node),arg_info);

  FUNCALL_EXPRLIST(arg_node) =TRAVopt(FUNCALL_EXPRLIST(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTexprlist (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTexprlist");

  PRINTNODE("ExprList");

  PRELOGUE
  
  EXPRLIST_EXPR(arg_node) =TRAVdo(EXPRLIST_EXPR(arg_node),arg_info);

  EPILOGUE

  EXPRLIST_NEXT(arg_node) =TRAVopt(EXPRLIST_NEXT(arg_node),arg_info);

  DBUG_RETURN (arg_node);
}
node *PRTifstat (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTifstat");

  PRINTNODE("IfStat");

  PRELOGUE
  
  IFSTAT_EXPR(arg_node) =TRAVdo(IFSTAT_EXPR(arg_node),arg_info);

  IFSTAT_BLOCK(arg_node) =TRAVdo(IFSTAT_BLOCK(arg_node),arg_info);

  IFSTAT_ELSEBLOCK(arg_node) =TRAVopt(IFSTAT_ELSEBLOCK(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTelseblock (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTelseblock");

  PRINTNODE("ElseBlock");

  PRELOGUE
  
  ELSEBLOCK_BLOCK(arg_node) =TRAVdo(ELSEBLOCK_BLOCK(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTmonop (node *arg_node, info * arg_info)
{
  char *tmp;

  DBUG_ENTER ("PRTmonop");

  PRINTNODE("MonOp");

  PRELOGUE
  
  printf( "( ");

  switch (MONOP_OP( arg_node)) {
    case MO_neg:
      tmp = "-";
      break;
    case MO_not:
      tmp = "!";
      break;
    case MO_unknown:
      DBUG_ASSERT( 0, "unknown monop detected!");
  }

  printf( " %s ", tmp);

  MONOP_RIGHT( arg_node) = TRAVdo( MONOP_RIGHT( arg_node), arg_info);

  printf( ")");
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTcast (node *arg_node, info * arg_info)
{
  DBUG_ENTER ("PRTcast");

  PRINTNODE("Cast");

  PRELOGUE
  
  printf( "( ");

  PRINTTYPE(CAST_TYPE( arg_node));

  printf( ")");

  CAST_EXPR( arg_node) = TRAVdo( CAST_EXPR( arg_node), arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTenclosedblock (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTenclosedblock");

  PRINTNODE("EnclosedBlock");

  PRELOGUE
  
  ENCLOSEDBLOCK_STATEMENTLIST(arg_node) =TRAVdo(ENCLOSEDBLOCK_STATEMENTLIST(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTvardeclistlocalfundef (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTvardeclistlocalfundef");

  PRINTNODE("VarDecListLocalFunDef");

  PRELOGUE
  
  VARDECLISTLOCALFUNDEF_VARDECLIST(arg_node) =TRAVopt(VARDECLISTLOCALFUNDEF_VARDECLIST(arg_node),arg_info);

  VARDECLISTLOCALFUNDEF_LOCALFUNDEF(arg_node) =TRAVopt(VARDECLISTLOCALFUNDEF_LOCALFUNDEF(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *PRTarrayinit (node *arg_node, info * arg_info)
{
  DBUG_ENTER("PRTarrayinit");

  PRINTNODE("ArrayInit");

  PRELOGUE
  
  ARRAYINIT_EXPRLIST(arg_node) =TRAVdo(ARRAYINIT_EXPRLIST(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}

/** <!-- ****************************************************************** -->
 * @brief Prints the given syntaxtree
 * 
 * @param syntaxtree a node structure
 * 
 * @return the unchanged nodestructure
 ******************************************************************************/

node 
*PRTdoPrint( node *syntaxtree)
{
  info *info;
  
  DBUG_ENTER("PRTdoPrint");

  DBUG_ASSERT( (syntaxtree!= NULL), "PRTdoPrint called with empty syntaxtree");

  printf( "\n\n------------------------------\n\n");

  info = MakeInfo();
  
  TRAVpush( TR_prt);

  syntaxtree = TRAVdo( syntaxtree, info);

  TRAVpop();

  info = FreeInfo(info);

  printf( "\n\n------------------------------\n\n");

  DBUG_RETURN( syntaxtree);
}

/**
 * @}
 */



