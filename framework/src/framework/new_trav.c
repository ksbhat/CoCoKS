
/**
 * @file eval.c
 *
 * Functions needed for compiling
 * 
 * THIS FILE HAS BEEN GENERATED USING 
 * $Id: free_node.c.xsl 14593 2006-01-31 17:09:55Z cg $.
 * DO NOT EDIT THIS FILE AS MIGHT BE CHANGED IN A LATER VERSION.
 *
 * ALL CHANGES MADE TO THIS FILE WILL BE OVERWRITTEN!
 *
 */

/**
 * @defgroup comp Compilation functions.
 *
 * Functions needed for compiling.
 *
 * @{
 */


#include "eval.h"
#include "traverse.h"
#include "dbug.h"


/** <!--******************************************************************-->
 *
 * @fn EVALassign
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Assign node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALassign (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALassign");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALbinop
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node BinOp node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALbinop (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALbinop");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALbool
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Bool node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALbool (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALbool");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALcast
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Cast node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALcast (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALcast");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALconst
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Const node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALconst (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALconst");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALdo
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Do node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALdo (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALdo");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALdowhilestat
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node DoWhileStat node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALdowhilestat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALdowhilestat");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALelse
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Else node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALelse (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALelse");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALelseblock
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node ElseBlock node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALelseblock (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALelseblock");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALenclosedblock
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node EnclosedBlock node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALenclosedblock (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALenclosedblock");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALenclosedexpr
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node EnclosedExpr node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALenclosedexpr (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALenclosedexpr");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALequals
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Equals node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALequals (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALequals");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALerror
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Error node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALerror (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALerror");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALexport
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Export node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALexport (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALexport");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALexprlist
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node ExprList node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALexprlist (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALexprlist");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALextern
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Extern node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALextern (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALextern");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfloat
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Float node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfloat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfloat");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfor
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node For node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfor (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfor");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfordec
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node ForDec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfordec (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfordec");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALforstat
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node ForStat node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALforstat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALforstat");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALforvarinit
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node ForVarInit node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALforvarinit (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALforvarinit");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfunbody
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node FunBody node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfunbody (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfunbody");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfuncall
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node FunCall node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfuncall (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfuncall");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfundec
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node FunDec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfundec (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfundec");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfundef
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node FunDef node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfundef (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfundef");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALfunheader
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node FunHeader node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALfunheader (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALfunheader");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALglobaldec
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node GlobalDec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALglobaldec (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALglobaldec");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALglobaldef
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node GlobalDef node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALglobaldef (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALglobaldef");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALifkeyword
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node IfKeyword node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALifkeyword (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALifkeyword");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALifstat
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node IfStat node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALifstat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALifstat");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALkey_bool
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Key_Bool node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALkey_bool (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALkey_bool");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALkey_float
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Key_Float node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALkey_float (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALkey_float");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALkey_int
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Key_Int node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALkey_int (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALkey_int");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALkey_void
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Key_Void node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALkey_void (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALkey_void");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALl_bracket
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node L_Bracket node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALl_bracket (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALl_bracket");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALl_fl_bracket
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node L_FL_Bracket node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALl_fl_bracket (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALl_fl_bracket");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALmonop
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node MonOp node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALmonop (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALmonop");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALnum
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Num node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALnum (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALnum");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALparam
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Param node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALparam (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALparam");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALparamlist
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node ParamList node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALparamlist (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALparamlist");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALprogram
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Program node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALprogram (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALprogram");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALr_bracket
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node R_Bracket node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALr_bracket (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALr_bracket");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALr_fl_bracket
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node R_FL_Bracket node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALr_fl_bracket (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALr_fl_bracket");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALrettype
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node RetType node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALrettype (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALrettype");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALreturn
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Return node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALreturn (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALreturn");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALreturnkeyword
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node ReturnKeyword node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALreturnkeyword (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALreturnkeyword");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALstatement
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Statement node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALstatement (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALstatement");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALstatementlist
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node StatementList node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALstatementlist (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALstatementlist");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALtype
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Type node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALtype (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALtype");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALvar
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node Var node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALvar (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALvar");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALvardec
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node VarDec node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALvardec (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALvardec");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALvardeclist
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node VarDecList node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALvardeclist (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALvardeclist");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALvarlet
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node VarLet node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALvarlet (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALvarlet");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALwhile
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node While node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALwhile (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALwhile");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/** <!--******************************************************************-->
 *
 * @fn EVALwhilestat
 *
 * @brief Frees the node and its sons/attributes
 *
 * @param arg_node WhileStat node to process
 * @param arg_info pointer to info structure
 *
 * @return processed node
 *
 ***************************************************************************/
node *
EVALwhilestat (node * arg_node, info * arg_info)
{
  DBUG_ENTER ("EVALwhilestat");
  arg_node = TRAVcont (arg_node, arg_info);
  DBUG_RETURN (arg_node);
}

/**
 * @}
 */