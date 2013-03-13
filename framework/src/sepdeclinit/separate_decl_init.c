
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
#include "dbug.h"
#include "memory.h"
#include "globals.h"

/*
 * INFO structure
 */
struct INFO {
  bool firsterror;
};

struct SEPTABSTACK {
  int  n_count; // number of tabs to be prefixed.
} septabstack = { 0 };

#define TABSTACK_MAXSIZE 100

#define INFO_FIRSTERROR(n) ((n)->firsterror)

#define PRINTNODE(msg)	\
    printf("\n");       \
    SEPputtabs();          \
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

#define PRELOGUE  SEPpush();

#define EPILOGUE  SEPpop();

void SEPpush()
{
  if ( TABSTACK_MAXSIZE > septabstack.n_count)
  {
    septabstack.n_count++;
  }
}

void SEPpop()
{
  if ( 0 < septabstack.n_count)
  {
    septabstack.n_count--;
  }
}

void SEPputtabs()
{
  int i;

  for(i=0; i<septabstack.n_count; i++)
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
 * @fn SEPinstr
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
SEPassign (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("SEPassign");

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
 * @fn SEPbinop
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
SEPbinop (node * arg_node, info * arg_info)
{
  char *tmp;

  DBUG_ENTER ("SEPbinop");

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
 * @fn SEPfloat
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
SEPfloat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("SEPfloat");

  PRELOGUE

  printf( "%f", FLOAT_VALUE( arg_node));
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}



/** <!--******************************************************************-->
 *
 * @fn SEPnum
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
SEPnum (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("SEPnum");

  PRELOGUE

  printf( "%i", NUM_VALUE( arg_node));
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}


/** <!--******************************************************************-->
 *
 * @fn SEPboolean
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
SEPbool (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("SEPbool");

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
 * @fn SEPvar
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
SEPvar (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("SEPvar");
  
  PRINTNODE("Var");

  PRELOGUE
  
  printf( "%s", VAR_NAME( arg_node));

  VAR_EXPRLIST(arg_node) = TRAVopt(VAR_EXPRLIST(arg_node), arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}


/** <!--******************************************************************-->
 *
 * @fn SEPerror
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
SEPerror (node * arg_node, info * arg_info)
{
  bool first_error;

  DBUG_ENTER ("SEPerror");

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



node *SEPstatement(node * arg_node, info * arg_info)
{
  DBUG_ENTER("SEPstatement");
  
  PRINTNODE("Statement");

  PRELOGUE  

  STATEMENT_SET_STATEMENT(arg_node) =TRAVdo(STATEMENT_SET_STATEMENT(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}

node *SEPprogram (node *arg_node, info * arg_info)
{
	DBUG_ENTER("Program");

  PROGRAM_NEXT(arg_node) =TRAVopt(PROGRAM_NEXT(arg_node),arg_info);
  
	PRINTNODE("Program");

  PRELOGUE
  
	PROGRAM_CODEBLOCK(arg_node) =TRAVdo(PROGRAM_CODEBLOCK(arg_node),arg_info);
	
  EPILOGUE

  DBUG_RETURN (arg_node);
}

node *SEPvardec (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPvardec");

  PRINTNODE("VarDec");

  PRELOGUE
  
  PRINTTYPE(VARDEC_TYPE(arg_node));

  VARDEC_VAR(arg_node) =TRAVdo(VARDEC_VAR(arg_node),arg_info);

  VARDEC_EXPRLIST(arg_node) =TRAVopt(VARDEC_EXPRLIST(arg_node),arg_info);

  VARDEC_EXPR(arg_node) =TRAVopt(VARDEC_EXPR(arg_node),arg_info);
  
  EPILOGUE
  
  DBUG_RETURN (arg_node);
}
node *SEPparam (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPparam");

  PRINTNODE("Param");

  PRELOGUE
  
  PRINTTYPE(PARAM_TYPE(arg_node));

  PARAM_VARNUMLIST(arg_node) =TRAVopt(PARAM_VARNUMLIST(arg_node),arg_info);

  PARAM_VAR(arg_node) =TRAVdo(PARAM_VAR(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}

node *SEPfundec (node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEPfundec");

  PRINTNODE("FunDec");

  PRELOGUE
  
  PRINTEXTERN(FUNDEC_EXTERN(arg_node));
  
  FUNDEC_FUNHEADER(arg_node) =TRAVdo(FUNDEC_FUNHEADER(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPfundef (node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEPfundef");

  PRINTNODE("FunDef");

  PRELOGUE
  
  PRINTEXPORT(FUNDEF_EXPORT(arg_node));
  
  FUNDEF_LOCALFUNDEF(arg_node) =TRAVdo(FUNDEF_LOCALFUNDEF(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPparamlist (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPparamlist");

  PARAMLIST_NEXT(arg_node) =TRAVopt(PARAMLIST_NEXT(arg_node),arg_info);

  PRINTNODE("ParamList");

  PRELOGUE
  
  PARAMLIST_PARAM(arg_node) =TRAVdo(PARAMLIST_PARAM(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPfunheader (node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEPfunheader");

  PRINTNODE("FunHeader");

  PRELOGUE
  
  PRINTTYPE(FUNHEADER_TYPE(arg_node));
  
  FUNHEADER_VAR(arg_node) =TRAVdo(FUNHEADER_VAR(arg_node),arg_info);

  FUNHEADER_PARAMLIST(arg_node) =TRAVopt(FUNHEADER_PARAMLIST(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);

}
node *SEPlocalfundef (node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEPlocalfundef");

  PRINTNODE("LocalFunDef");

  PRELOGUE
  
  LOCALFUNDEF_FUNHEADER(arg_node) =TRAVdo(LOCALFUNDEF_FUNHEADER(arg_node),arg_info);
  
  LOCALFUNDEF_FUNBODY(arg_node) =TRAVdo(LOCALFUNDEF_FUNBODY(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPfunbody (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPfunbody");

  PRINTNODE("FunBody");

  PRELOGUE
  
  FUNBODY_VARDECLIST(arg_node) =TRAVopt(FUNBODY_VARDECLIST(arg_node),arg_info);
  
  FUNBODY_LOCALFUNDEFLIST(arg_node) =TRAVopt(FUNBODY_LOCALFUNDEFLIST(arg_node),arg_info);

  FUNBODY_STATEMENTLIST(arg_node) =TRAVopt(FUNBODY_STATEMENTLIST(arg_node),arg_info);

  FUNBODY_RETURN(arg_node) =TRAVopt(FUNBODY_RETURN(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPlocalfundeflist (node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEPlocalfundeflist");

  LOCALFUNDEFLIST_NEXT(arg_node) =TRAVopt(LOCALFUNDEFLIST_NEXT(arg_node),arg_info);

  PRINTNODE("LocalFunDefList");

  PRELOGUE
  
  LOCALFUNDEFLIST_LOCALFUNDEF(arg_node) =TRAVdo(LOCALFUNDEFLIST_LOCALFUNDEF(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPglobaldef (node *arg_node, info * arg_info)
{
	DBUG_ENTER ("SEPglobaldef");

	PRINTNODE("global def");

  PRELOGUE
  
  GLOBALDEF_SET_GLOBALDEF( arg_node) = TRAVdo( GLOBALDEF_SET_GLOBALDEF(arg_node), arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPusualdef (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPusualdef");

  PRINTNODE("UsualDef");

  PRELOGUE
  
  PRINTEXPORT(USUALDEF_EXPORT(arg_node));

  PRINTTYPE(USUALDEF_TYPE(arg_node));
  
  USUALDEF_VAR(arg_node) =TRAVdo(USUALDEF_VAR(arg_node),arg_info);
	
	USUALDEF_EXPR(arg_node) =TRAVopt(USUALDEF_EXPR(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEParraydef (node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEParraydef");

	PRINTNODE("ArrayDef");

  PRELOGUE
  
	PRINTEXPORT(ARRAYDEF_EXPORT(arg_node));

  PRINTTYPE(ARRAYDEF_TYPE(arg_node));

	ARRAYDEF_EXPRLIST(arg_node) = TRAVdo(ARRAYDEF_EXPRLIST(arg_node), arg_info);

	ARRAYDEF_VAR(arg_node) = TRAVdo(ARRAYDEF_VAR(arg_node), arg_info);
  
  EPILOGUE

	DBUG_RETURN (arg_node);
}
node *SEPglobaldec (node *arg_node, info * arg_info)
{
	DBUG_ENTER("SEPglobaldec");

  PRINTNODE("GlobalDec");

  PRELOGUE
  
  PRINTEXTERN(GLOBALDEC_EXTERN(arg_node));

  PRINTTYPE(GLOBALDEC_TYPE(arg_node));

  GLOBALDEC_VARNUMLIST(arg_node) = TRAVopt(GLOBALDEC_VARNUMLIST(arg_node), arg_info);

  GLOBALDEC_VAR(arg_node) = TRAVdo(GLOBALDEC_VAR(arg_node), arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPvarnumlist (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPvarnumlist");

  VARNUMLIST_NEXT(arg_node) = TRAVopt(VARNUMLIST_NEXT(arg_node), arg_info);

  PRINTNODE("VarNumList");

  PRELOGUE
  
  VARNUMLIST_VARORNUM(arg_node) = TRAVdo(VARNUMLIST_VARORNUM(arg_node), arg_info);

  EPILOGUE  

  DBUG_RETURN (arg_node);
}
node *SEPvardeclist (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPvardeclist");

  VARDECLIST_NEXT(arg_node) =TRAVopt(VARDECLIST_NEXT(arg_node),arg_info);

  PRINTNODE("VarDecList");

  PRELOGUE
  
  VARDECLIST_VARDEC(arg_node) =TRAVdo(VARDECLIST_VARDEC(arg_node),arg_info);

  EPILOGUE
  
  DBUG_RETURN (arg_node);
}
node *SEPreturn (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPreturn");

  PRINTNODE("Return");

  PRELOGUE
  
  RETURN_EXPR(arg_node) =TRAVdo(RETURN_EXPR(arg_node),arg_info);
  
  EPILOGUE
  
  DBUG_RETURN (arg_node);
}
node *SEPstatementlist (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPstatementlist");

  STATEMENTLIST_NEXT(arg_node) =TRAVopt(STATEMENTLIST_NEXT(arg_node),arg_info);

  PRINTNODE("StatementList");

  PRELOGUE
  
  STATEMENTLIST_STATEMENT(arg_node) =TRAVdo(STATEMENTLIST_STATEMENT(arg_node),arg_info);

  EPILOGUE
  
  //EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPdowhilestat (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPdowhilestat");

  PRINTNODE("DoWhileStat");

  PRELOGUE
  
  DOWHILESTAT_BLOCK(arg_node) =TRAVdo(DOWHILESTAT_BLOCK(arg_node),arg_info);

  DOWHILESTAT_EXPR(arg_node) =TRAVdo(DOWHILESTAT_EXPR(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPwhilestat (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPwhilestat");

  PRINTNODE("WhileStat");

  PRELOGUE
  
  WHILESTAT_EXPR(arg_node) =TRAVdo(WHILESTAT_EXPR(arg_node),arg_info);

  WHILESTAT_BLOCK(arg_node) =TRAVdo(WHILESTAT_BLOCK(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);

}
node *SEPforstat (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPforstat");

  PRINTNODE("ForStat");

  PRELOGUE
  
  FORSTAT_VARDEC(arg_node) =TRAVdo(FORSTAT_VARDEC(arg_node),arg_info);

  FORSTAT_EXPR1(arg_node) =TRAVdo(FORSTAT_EXPR1(arg_node),arg_info);

  FORSTAT_EXPR2(arg_node) =TRAVopt(FORSTAT_EXPR2(arg_node),arg_info);

  FORSTAT_BLOCK(arg_node) =TRAVdo(FORSTAT_BLOCK(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPfuncall (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPfuncall");

  PRINTNODE("FunCall");

  PRELOGUE
  
  FUNCALL_VAR(arg_node) =TRAVdo(FUNCALL_VAR(arg_node),arg_info);

  FUNCALL_EXPRLIST(arg_node) =TRAVopt(FUNCALL_EXPRLIST(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPexprlist (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPexprlist");

  PRINTNODE("ExprList");

  PRELOGUE
  
  EXPRLIST_EXPR(arg_node) =TRAVdo(EXPRLIST_EXPR(arg_node),arg_info);

  EPILOGUE

  EXPRLIST_NEXT(arg_node) =TRAVopt(EXPRLIST_NEXT(arg_node),arg_info);

  DBUG_RETURN (arg_node);
}
node *SEPifstat (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPifstat");

  PRINTNODE("IfStat");

  PRELOGUE
  
  IFSTAT_EXPR(arg_node) =TRAVdo(IFSTAT_EXPR(arg_node),arg_info);

  IFSTAT_BLOCK(arg_node) =TRAVdo(IFSTAT_BLOCK(arg_node),arg_info);

  IFSTAT_ELSEBLOCK(arg_node) =TRAVopt(IFSTAT_ELSEBLOCK(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPelseblock (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPelseblock");

  PRINTNODE("ElseBlock");

  PRELOGUE
  
  ELSEBLOCK_BLOCK(arg_node) =TRAVdo(ELSEBLOCK_BLOCK(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPmonop (node *arg_node, info * arg_info)
{
  char *tmp;

  DBUG_ENTER ("SEPmonop");

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
node *SEPcast (node *arg_node, info * arg_info)
{
  DBUG_ENTER ("SEPcast");

  PRINTNODE("Cast");

  PRELOGUE
  
  printf( "( ");

  PRINTTYPE(CAST_TYPE( arg_node));

  printf( ")");

  CAST_EXPR( arg_node) = TRAVdo( CAST_EXPR( arg_node), arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPenclosedblock (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPenclosedblock");

  PRINTNODE("EnclosedBlock");

  PRELOGUE
  
  ENCLOSEDBLOCK_STATEMENTLIST(arg_node) =TRAVdo(ENCLOSEDBLOCK_STATEMENTLIST(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEPvardeclistlocalfundef (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPvardeclistlocalfundef");

  PRINTNODE("VarDecListLocalFunDef");

  PRELOGUE
  
  VARDECLISTLOCALFUNDEF_VARDECLIST(arg_node) =TRAVopt(VARDECLISTLOCALFUNDEF_VARDECLIST(arg_node),arg_info);

  VARDECLISTLOCALFUNDEF_LOCALFUNDEF(arg_node) =TRAVopt(VARDECLISTLOCALFUNDEF_LOCALFUNDEF(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}
node *SEParrayinit (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEParrayinit");

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
*SEPdoSeparateDeclInit( node *syntaxtree)
{
  info *info;
  
  DBUG_ENTER("SEPdoSeparateDeclInit");

  DBUG_ASSERT( (syntaxtree!= NULL), "SEPdoSeparateDeclInit called with empty syntaxtree");

  printf( "\n\n------------------------------\n\n");

  info = MakeInfo();
  
  TRAVpush( TR_sep);

  syntaxtree = TRAVdo( syntaxtree, info);

  TRAVpop();

  info = FreeInfo(info);

  printf( "\n\n------------------------------\n\n");

  DBUG_RETURN( syntaxtree);
}

/**
 * @}
 */



