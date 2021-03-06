
/**
 * @file check_node.c
 *
 * Functions needed by chkm traversal.
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: check_node.c.xsl 15657 2007-11-13 13:57:30Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

/**
 * @defgroup check Touch all Tree Functions to catch every node, son and attribute
 *
 * Functions needed by free traversal.
 *
 * @{
 */


#include "check_node.h"
#include "check_attribs.h"
#include "tree_basic.h"
#include "traverse.h"
#include "dbug.h"

#define CHKMTRAV( node, info) (node != NULL) ? TRAVdo( node, info) : node
#define CHKMCOND( node, info)                                    \
    ? CHKMTRAV( node, info)                                      \
    : (node)



/*******************************************************************************
 *
 * @fn CHKMpostfun
 *
 * This is the postfun function of the CHKM Traversal  
 *
 ******************************************************************************/
node *
CHKMpostfun (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMpostfun");

  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMassign
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Assign node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMassign (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMassign");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  ASSIGN_LET (arg_node) = CHKMTRAV (ASSIGN_LET (arg_node), arg_info);
  ASSIGN_EXPR (arg_node) = CHKMTRAV (ASSIGN_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMbinop
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node BinOp node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMbinop (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMbinop");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  BINOP_LEFT (arg_node) = CHKMTRAV (BINOP_LEFT (arg_node), arg_info);
  BINOP_RIGHT (arg_node) = CHKMTRAV (BINOP_RIGHT (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMbool
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Bool node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMbool (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMbool");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMcast
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Cast node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMcast (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMcast");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  CAST_L_BRACKET (arg_node) = CHKMTRAV (CAST_L_BRACKET (arg_node), arg_info);
  CAST_TYPE (arg_node) = CHKMTRAV (CAST_TYPE (arg_node), arg_info);
  CAST_R_BRACKET (arg_node) = CHKMTRAV (CAST_R_BRACKET (arg_node), arg_info);
  CAST_EXPR (arg_node) = CHKMTRAV (CAST_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMconst
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Const node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMconst (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMconst");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  CONST_SET_CONST (arg_node) =
    CHKMTRAV (CONST_SET_CONST (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMdo
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Do node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMdo (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMdo");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMdowhilestat
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node DoWhileStat node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMdowhilestat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMdowhilestat");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DOWHILESTAT_DO (arg_node) = CHKMTRAV (DOWHILESTAT_DO (arg_node), arg_info);
  DOWHILESTAT_BLOCK (arg_node) =
    CHKMTRAV (DOWHILESTAT_BLOCK (arg_node), arg_info);
  DOWHILESTAT_WHILE (arg_node) =
    CHKMTRAV (DOWHILESTAT_WHILE (arg_node), arg_info);
  DOWHILESTAT_EXPR (arg_node) =
    CHKMTRAV (DOWHILESTAT_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMelse
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Else node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMelse (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMelse");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMelseblock
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node ElseBlock node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMelseblock (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMelseblock");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  ELSEBLOCK_ELSE (arg_node) = CHKMTRAV (ELSEBLOCK_ELSE (arg_node), arg_info);
  ELSEBLOCK_BLOCK (arg_node) =
    CHKMTRAV (ELSEBLOCK_BLOCK (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMenclosedblock
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node EnclosedBlock node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMenclosedblock (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMenclosedblock");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  ENCLOSEDBLOCK_L_FL_BRACKET (arg_node) =
    CHKMTRAV (ENCLOSEDBLOCK_L_FL_BRACKET (arg_node), arg_info);
  ENCLOSEDBLOCK_STATEMENTLIST (arg_node) =
    CHKMTRAV (ENCLOSEDBLOCK_STATEMENTLIST (arg_node), arg_info);
  ENCLOSEDBLOCK_R_FL_BRACKET (arg_node) =
    CHKMTRAV (ENCLOSEDBLOCK_R_FL_BRACKET (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMenclosedexpr
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node EnclosedExpr node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMenclosedexpr (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMenclosedexpr");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  ENCLOSEDEXPR_L_BRACKET (arg_node) =
    CHKMTRAV (ENCLOSEDEXPR_L_BRACKET (arg_node), arg_info);
  ENCLOSEDEXPR_EXPR (arg_node) =
    CHKMTRAV (ENCLOSEDEXPR_EXPR (arg_node), arg_info);
  ENCLOSEDEXPR_R_BRACKET (arg_node) =
    CHKMTRAV (ENCLOSEDEXPR_R_BRACKET (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMequals
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Equals node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMequals (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMequals");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMerror
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Error node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMerror (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMerror");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  ERROR_NEXT (arg_node) = CHKMTRAV (ERROR_NEXT (arg_node), arg_info);
  ERROR_MESSAGE (arg_node) =
    CHKMattribString (ERROR_MESSAGE (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMexport
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Export node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMexport (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMexport");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMexprlist
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node ExprList node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMexprlist (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMexprlist");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  EXPRLIST_NEXT (arg_node) = CHKMTRAV (EXPRLIST_NEXT (arg_node), arg_info);
  EXPRLIST_EXPR (arg_node) = CHKMTRAV (EXPRLIST_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMextern
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Extern node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMextern (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMextern");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfloat
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Float node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfloat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfloat");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfor
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node For node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfor (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfor");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfordec
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node ForDec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfordec (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfordec");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  FORDEC_TYPE (arg_node) = CHKMTRAV (FORDEC_TYPE (arg_node), arg_info);
  FORDEC_FORVARINIT (arg_node) =
    CHKMTRAV (FORDEC_FORVARINIT (arg_node), arg_info);
  FORDEC_EXPR (arg_node) = CHKMTRAV (FORDEC_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMforstat
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node ForStat node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMforstat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMforstat");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  FORSTAT_FOR (arg_node) = CHKMTRAV (FORSTAT_FOR (arg_node), arg_info);
  FORSTAT_FORDEC (arg_node) = CHKMTRAV (FORSTAT_FORDEC (arg_node), arg_info);
  FORSTAT_BLOCK (arg_node) = CHKMTRAV (FORSTAT_BLOCK (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMforvarinit
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node ForVarInit node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMforvarinit (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMforvarinit");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  FORVARINIT_VARLET (arg_node) =
    CHKMTRAV (FORVARINIT_VARLET (arg_node), arg_info);
  FORVARINIT_EQUALS (arg_node) =
    CHKMTRAV (FORVARINIT_EQUALS (arg_node), arg_info);
  FORVARINIT_EXPR (arg_node) =
    CHKMTRAV (FORVARINIT_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfunbody
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node FunBody node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfunbody (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfunbody");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfuncall
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node FunCall node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfuncall (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfuncall");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  FUNCALL_VAR (arg_node) = CHKMTRAV (FUNCALL_VAR (arg_node), arg_info);
  FUNCALL_EXPRLIST (arg_node) =
    CHKMTRAV (FUNCALL_EXPRLIST (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfundec
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node FunDec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfundec (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfundec");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  FUNDEC_EXTERN (arg_node) = CHKMTRAV (FUNDEC_EXTERN (arg_node), arg_info);
  FUNDEC_FUNHEADER (arg_node) =
    CHKMTRAV (FUNDEC_FUNHEADER (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfundef
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node FunDef node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfundef (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfundef");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  FUNDEF_EXPORT (arg_node) = CHKMTRAV (FUNDEF_EXPORT (arg_node), arg_info);
  FUNDEF_FUNHEADER (arg_node) =
    CHKMTRAV (FUNDEF_FUNHEADER (arg_node), arg_info);
  FUNDEF_FUNBODY (arg_node) = CHKMTRAV (FUNDEF_FUNBODY (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMfunheader
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node FunHeader node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMfunheader (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMfunheader");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  FUNHEADER_RETTYPE (arg_node) =
    CHKMTRAV (FUNHEADER_RETTYPE (arg_node), arg_info);
  FUNHEADER_VAR (arg_node) = CHKMTRAV (FUNHEADER_VAR (arg_node), arg_info);
  FUNHEADER_PARAMLIST (arg_node) =
    CHKMTRAV (FUNHEADER_PARAMLIST (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMglobaldec
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node GlobalDec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMglobaldec (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMglobaldec");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  GLOBALDEC_EXTERN (arg_node) =
    CHKMTRAV (GLOBALDEC_EXTERN (arg_node), arg_info);
  GLOBALDEC_TYPE (arg_node) = CHKMTRAV (GLOBALDEC_TYPE (arg_node), arg_info);
  GLOBALDEC_VAR (arg_node) = CHKMTRAV (GLOBALDEC_VAR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMglobaldef
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node GlobalDef node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMglobaldef (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMglobaldef");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  GLOBALDEF_EXPORT (arg_node) =
    CHKMTRAV (GLOBALDEF_EXPORT (arg_node), arg_info);
  GLOBALDEF_TYPE (arg_node) = CHKMTRAV (GLOBALDEF_TYPE (arg_node), arg_info);
  GLOBALDEF_VAR (arg_node) = CHKMTRAV (GLOBALDEF_VAR (arg_node), arg_info);
  GLOBALDEF_EXPR (arg_node) = CHKMTRAV (GLOBALDEF_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMifkeyword
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node IfKeyword node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMifkeyword (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMifkeyword");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMifstat
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node IfStat node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMifstat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMifstat");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  IFSTAT_IF (arg_node) = CHKMTRAV (IFSTAT_IF (arg_node), arg_info);
  IFSTAT_L_BRACKET (arg_node) =
    CHKMTRAV (IFSTAT_L_BRACKET (arg_node), arg_info);
  IFSTAT_EXPR (arg_node) = CHKMTRAV (IFSTAT_EXPR (arg_node), arg_info);
  IFSTAT_R_BRACKET (arg_node) =
    CHKMTRAV (IFSTAT_R_BRACKET (arg_node), arg_info);
  IFSTAT_BLOCK (arg_node) = CHKMTRAV (IFSTAT_BLOCK (arg_node), arg_info);
  IFSTAT_ELSEBLOCK (arg_node) =
    CHKMTRAV (IFSTAT_ELSEBLOCK (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMkey_bool
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Key_Bool node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMkey_bool (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMkey_bool");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMkey_float
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Key_Float node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMkey_float (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMkey_float");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMkey_int
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Key_Int node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMkey_int (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMkey_int");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMkey_void
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Key_Void node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMkey_void (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMkey_void");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMl_bracket
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node L_Bracket node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMl_bracket (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMl_bracket");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMl_fl_bracket
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node L_FL_Bracket node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMl_fl_bracket (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMl_fl_bracket");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMmonop
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node MonOp node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMmonop (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMmonop");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  MONOP_RIGHT (arg_node) = CHKMTRAV (MONOP_RIGHT (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMnum
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Num node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMnum (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMnum");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMparam
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Param node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMparam (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMparam");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  PARAM_TYPE (arg_node) = CHKMTRAV (PARAM_TYPE (arg_node), arg_info);
  PARAM_VAR (arg_node) = CHKMTRAV (PARAM_VAR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMparamlist
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node ParamList node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMparamlist (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMparamlist");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  PARAMLIST_NEXT (arg_node) = CHKMTRAV (PARAMLIST_NEXT (arg_node), arg_info);
  PARAMLIST_PARAM (arg_node) =
    CHKMTRAV (PARAMLIST_PARAM (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMprogram
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Program node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMprogram (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMprogram");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  PROGRAM_NEXT (arg_node) = CHKMTRAV (PROGRAM_NEXT (arg_node), arg_info);
  PROGRAM_CODEBLOCK (arg_node) =
    CHKMTRAV (PROGRAM_CODEBLOCK (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMr_bracket
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node R_Bracket node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMr_bracket (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMr_bracket");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMr_fl_bracket
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node R_FL_Bracket node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMr_fl_bracket (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMr_fl_bracket");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMrettype
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node RetType node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMrettype (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMrettype");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  RETTYPE_SET_RETTYPE (arg_node) =
    CHKMTRAV (RETTYPE_SET_RETTYPE (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMreturn
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Return node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMreturn (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMreturn");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  RETURN_RETURNKEYWORD (arg_node) =
    CHKMTRAV (RETURN_RETURNKEYWORD (arg_node), arg_info);
  RETURN_EXPR (arg_node) = CHKMTRAV (RETURN_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMreturnkeyword
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node ReturnKeyword node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMreturnkeyword (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMreturnkeyword");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMstatement
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Statement node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMstatement (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMstatement");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  STATEMENT_SET_STATEMENT (arg_node) =
    CHKMTRAV (STATEMENT_SET_STATEMENT (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMstatementlist
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node StatementList node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMstatementlist (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMstatementlist");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  STATEMENTLIST_NEXT (arg_node) =
    CHKMTRAV (STATEMENTLIST_NEXT (arg_node), arg_info);
  STATEMENTLIST_STATEMENT (arg_node) =
    CHKMTRAV (STATEMENTLIST_STATEMENT (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMtype
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Type node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMtype (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMtype");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  TYPE_SET_TYPE (arg_node) = CHKMTRAV (TYPE_SET_TYPE (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMvar
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node Var node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMvar (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMvar");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  VAR_NAME (arg_node) = CHKMattribString (VAR_NAME (arg_node), arg_info);
  VAR_DECL (arg_node) = CHKMattribLink (VAR_DECL (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMvardec
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node VarDec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMvardec (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMvardec");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  VARDEC_TYPE (arg_node) = CHKMTRAV (VARDEC_TYPE (arg_node), arg_info);
  VARDEC_VAR (arg_node) = CHKMTRAV (VARDEC_VAR (arg_node), arg_info);
  VARDEC_EXPR (arg_node) = CHKMTRAV (VARDEC_EXPR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMvardeclist
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node VarDecList node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMvardeclist (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMvardeclist");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  VARDECLIST_NEXT (arg_node) =
    CHKMTRAV (VARDECLIST_NEXT (arg_node), arg_info);
  VARDECLIST_VARDEC (arg_node) =
    CHKMTRAV (VARDECLIST_VARDEC (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMvarlet
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node VarLet node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMvarlet (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMvarlet");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  VARLET_NAME (arg_node) =
    CHKMattribString (VARLET_NAME (arg_node), arg_info);
  VARLET_DECL (arg_node) = CHKMattribLink (VARLET_DECL (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMwhile
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node While node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMwhile (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMwhile");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn CHKMwhilestat
 *
 * @brief Touched the node and its sons/attributes
 *
 * @param arg_node WhileStat node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
CHKMwhilestat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("CHKMwhilestat");
  NODE_ERROR (arg_node) = CHKMTRAV (NODE_ERROR (arg_node), arg_info);
  WHILESTAT_WHILE (arg_node) =
    CHKMTRAV (WHILESTAT_WHILE (arg_node), arg_info);
  WHILESTAT_EXPR (arg_node) = CHKMTRAV (WHILESTAT_EXPR (arg_node), arg_info);
  WHILESTAT_BLOCK (arg_node) =
    CHKMTRAV (WHILESTAT_BLOCK (arg_node), arg_info);
  DBUG_RETURN (arg_node);
}

/**
 * @}
 */
