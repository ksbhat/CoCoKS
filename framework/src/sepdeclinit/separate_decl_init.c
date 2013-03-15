    
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
#include "str.h"

/*
 * INFO structure
 */
struct INFO {
  bool firsterror;
  bool isglobal;

  node *gstatementlist;
  node *lstatementlist;

  node *gvardeclist;
  node *lvardeclist;
  node *prevparamlist;
  varbag *userdefined;
  varbag *compilerdefined;
};

typedef enum {
  USUALDEF, VARDEC,ARRAYDEF ;
} DEC_TYPE;

typedef struct VARBAG {
DEC_TYPE node_type;
node *var;
node *statements[2];
struct VARBAG *next; 
} varbag;

#define INFO_FIRSTERROR(n) ((n)->firsterror)
#define INFO_ISGLOBAL(n) ((n)->isglobal)
#define INFO_GSTATEMENTLIST(n) ((n)->gstatementlist)
#define INFO_LSTATEMENTLIST(n) ((n)->lstatementlist)
#define INFO_GVARDECLIST(n)    ((n)->gvardeclist)
#define INFO_LVARDECLIST(n)    ((n)->lvardeclist)
#define INFO_COMPILERDEFVARS(n)    ((n)->compilerdefined)
#define INFO_USERDEFVARS(n)   ((n)->userdefined)
#define INFO_PREVPARAMLIST(n)   ((n)->prevparamlist);
#define VARBAG_NEXT(n)        ((n)->next)
#define VARBAG_DECL(n)        ((n)->var)
#define VARBAG_STMTS(n)        ((n)->statements)
#define VARBAG_TYPE(n)        ((n)->node_type)

#define NAME_COUNTER_LIMIT  10000

static info *MakeInfo()
{
  info *result;
  
  result = MEMmalloc(sizeof(info));

  INFO_FIRSTERROR(result) = FALSE;

  INFO_ISGLOBAL(result) = FALSE;

  INFO_GSTATEMENTLIST(result) = NULL;

  INFO_LSTATEMENTLIST(result) = NULL;

  INFO_GVARDECLIST(result) = NULL;

  INFO_LVARDECLIST(result) = NULL;

  INFO_COMPILERDEFVARS(result) = NULL;

  INFO_USERDEFVARS(result) = NULL;

  INFO_PREVPARAMLIST(n) = NULL:

  return result;
}


static info *FreeInfo( info *info)
{
  info = MEMfree( info);

  return info;
}

/*
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
 *************************************************************************** /

node *SEParrayinit (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEParrayinit");

  PRINTNODE("ArrayInit");

  PRELOGUE
  
  ARRAYINIT_EXPRLIST(arg_node) =TRAVdo(ARRAYINIT_EXPRLIST(arg_node),arg_info);
  
  EPILOGUE

  DBUG_RETURN (arg_node);
}

*/

char *SEPgenvarname(info *arg_info)
{
  char szPattern[3] = "__";
  char  *name;
  static int counter = 0;

  name = STRcat(szPattern, STRitoa(counter));
  counter = (counter + 1) % NAME_COUNTER_LIMIT;

  return name;
}

bool SEPdetectnamecollision(node *arg_node)
{
   //TODO: later
  return FALSE;
}

extern node *SEPfuncall (node *arg_node, info * arg_info);
extern node *SEPparamlist (node *arg_node, info * arg_info);
extern node *SEPforstat (node *arg_node, info * arg_info);
extern node *SEParrayinit(node *arg_node, info * arg_info);

node *SEPparamlist (node *arg_node, info * arg_info)
{
  node *param;
  DBUG_ENTER("SEPparamlist");

  PARAMLIST_PARAM(arg_node) =TRAVdo(PARAMLIST_PARAM(arg_node),arg_info);

 // PARAMLIST_NEXT(arg_node) = TRAVopt(PARAMLIST_NEXT(arg_node), arg_info);
 
  param=PARAMLIST_PARAM(arg_node);


  // If the param has array dimension specification:
  if(PARAM_VARNUMLIST(param) != NULL)
  {
    varnumlist=PARAM_VARNUMLIST(param);

    varornum=VARNUMLIST_VARORNUM(varnumlist);
    newParamlist=NULL;
    while(varnumlist!=NULL)
    {
      var=VARORNUM_VAR(varornum);
      newParam=TBmakeParam(T_int,NULL,var);
      if(newParamlist==NULL)
      { 
        // first time we enter the loop, set the tail.
        newParamlist=TBmakeParamlist(newParam,newParamlist);
        tail = newParamlist;
      }
      else
      {
        newParamlist=TBmakeParamlist(newParam,newParamlist);
      }
      varnumlist=VARNUMLIST_NEXT(varornum);

    }
       //eg int a, int [m,n]b, int c => c->b->n->m->a. the below code does this insertion
       PARAMLIST_NEXT(tail)=PARAMLIST_NEXT(arg_node); // m->a
       PARAMLIST_NEXT(arg_node)=newParamlist; // b->n
    
  } 

  INFO_PREVPARAMLIST(arg_info)=arg_node;
  
  PARAMLIST_NEXT(arg_node) = TRAVopt(PARAMLIST_NEXT(arg_node), arg_info);

  DBUG_RETURN (arg_node); 
}
// returns a statementlist
node *SEParraydefinitionhandler(node *arg_node, info *arg_info)
{
  node *curr, *tail, *vardec, *var, *varCopyExpr, *varCopyAssign, *funcall;
  node *statement, *funvar, *yetanothervar, *source, *assignment, *arr_internal_assignment;
  node *vardeclist, *statementlist, *newexprlist, *funcallstat, *arrstatement, *arrstatementlist;

  DBUG_ENTER("SEParraydefinitionhandler");

    vardeclist = NULL;
    newexprlist = NULL;
    arrstatementlist = NULL;
    tail = NULL;

    // Processing the exprlist which is part of "array declaration" in the vardec.
    curr = VARDEC_EXPRLIST(arg_node);

    //Pre-prepare a slot for the function call __allocate statement
    funvar = TBmakeVar("__allocate", NULL);
    funcall = TBmakeFuncall(funvar, NULL);
    funcallstat = TBmakeStatement(funcall);

    while ( NULL != curr)
    {
      var = TBmakeVar(SEPgenvarname(), NULL)
      vardec = TBmakeVardec(T_int, NULL, var, NULL);
      vardeclist = TBmakeVardeclist(vardec, vardeclist);

      // Put the newly created variable into our compiler defined bag.
       compilerdefvar = MEMmalloc(sizeof(varbag));
       VARBAG_DECL(compilerdefvar) = VARDEC_VAR(arg_node);
       VARBAG_NEXT(compilerdefvar) = INFO_COMPILERDEFVARS(arg_info);
       INFO_COMPILERDEFVARS(arg_info) = compilerdefvar;

      // Add the corresponding expr into the newexprlist being formed
      varCopyExpr = MEMcopy(sizeof(node), var);
      newexprlist = TBmakeExprlist(varCopyExpr, newexprlist);
      

      // Create assignment statements for every internal variable created in this loop (the ones that occur in the function call)
      varCopyAssign = MEMcopy(sizeof(node), var);
      arrstatement = TBmakeAssign(varCopyAssign, EXPRLIST_EXPR(curr));

      //add this statement to our bag, corresponding to the variable used.
      VARBAG_STMTS(compilerdefvar)[0] = arrstatement;
      // Also add the pre-prepared function call statement to the bag.
      VARBAG_STMTS(funcallstat)[1] = funcallstat;  // this is going to be like a placeholder which gets filled in outside the loop.
      //// OBSERVATION (ABOVE): every var in the bag would refer to the same funcall statement and that is what we intend to do.

      if (NULL == arrstatementlist)
      {
        // Mark the tail: Its the first statement that we create in this while loop.
          arrstatementlist = TBmakeStatementlist(arrstatement, arrstatementlist);
          tail = arrstatementlist;
      }
      else
      {
        // This is not the first time we are entering the while loop if we are here.
        arrstatementlist = TBmakeStatementlist(arrstatement, arrstatementlist);
      }
      curr = EXPRLIST_NEXT(curr);
    }
    
    // Create a statement which is a funcall, passing in the exprlist formed above.
    FUNCALL_EXPRLIST(funcall) = newexprlist;
    
    // Now, we assign the __allocate() funcall to the newly created variable
    // like: <uservariable> = __allocate(__1, __2);
    yetanothervar = MEMcopy(sizeof(node), VARDEC_VAR(arg_node));
    arr_internal_assignment = TBmakeAssign(yetanothervar, EXPRLIST_EXPR(curr));
    arrstatement = TBmakeStatement(arr_internal_assignment);
    // Now collate the funcall statement and this newly added assignment statement into a new statementlist.
    arrstatementlist = TBmakeStatementlist(arrstatement, arrstatementlist);
    arrstatementlist = TBmakeStatementlist(funcallstat, arrstatementlist);

    DBUG_RETURN(arrstatementlist);
}

// Beware! Complex function that has taken more than 12 hours to code :)
node *SEPvardec(node *arg_node, info * arg_info)
{
  node *curr, *tail, *vardec, *var, *varCopyExpr, *varCopyAssign, *funcall;
  node *statement, *funvar, *yetanothervar, *source, *assignment, *arr_internal_assignment;
  node *vardeclist, *statementlist, *newexprlist, *funcallstat, *arrstatement, *arrstatementlist;

  varbag *namecollision, *compilerdefvar;

  DBUG_ENTER("SEPvardec");

  VARDEC_EXPR(arg_node) = TRAVopt(VARDEC_EXPR(arg_node), arg_info);  

  VARDEC_VAR(arg_node) = TRAVdo(VARDEC_VAR(arg_node), arg_info); 

  VARDEC_EXPRLIST(arg_node) = TRAVopt(VARDEC_EXPRLIST(arg_node), arg_info);   

    vardeclist = NULL;
    newexprlist = NULL;
    arrstatementlist = NULL;
    tail = NULL;

    if(ispotentialcollision(VARDEC_VAR(arg_node))
    {
      // NOTE: NEVER use VARBAG_STMTLIST(n) here!
       namecollision = MEMmalloc(sizeof(varbag));
       VARBAG_TYPE(namecollision)=VARDEC;
       VARBAG_DECL(namecollision) = VARDEC_VAR(arg_node); // pass the danger variable.
       VARBAG_NEXT(namecollision) = INFO_USERDEFVARS(arg_info);
       INFO_USERDEFVARS(arg_info) = namecollision;
    }

  // Array declaration being handled.
  if (NULL != VARDEC_EXPRLIST(arg_node))
  {
/*    // Processing the exprlist which is part of "array declaration".
    curr = VARDEC_EXPRLIST(arg_node);

    //Pre-prepare a slot for the function call __allocate statement
    funvar = TBmakeVar("__allocate", NULL);
    funcall = TBmakeFuncall(funvar, NULL);
    funcallstat = TBmakeStatement(funcall);

    while ( NULL != curr)
    {
      var = TBmakeVar(SEPgenvarname(), NULL)
      vardec = TBmakeVardec(T_int, NULL, var, NULL);
      vardeclist = TBmakeVardeclist(vardec, vardeclist);

      // Put the newly created variable into our compiler defined bag.
       compilerdefvar = MEMmalloc(sizeof(varbag));
       VARBAG_DECL(compilerdefvar) = VARDEC_VAR(arg_node);
       VARBAG_NEXT(compilerdefvar) = INFO_COMPILERDEFVARS(arg_info);
       INFO_COMPILERDEFVARS(arg_info) = compilerdefvar;

      // Add the corresponding expr into the newexprlist being formed
      varCopyExpr = MEMcopy(sizeof(node), var);
      newexprlist = TBmakeExprlist(varCopyExpr, newexprlist);
      

      // Create assignment statements for every internal variable created in this loop (the ones that occur in the function call)
      varCopyAssign = MEMcopy(sizeof(node), var);
      arrstatement = TBmakeAssign(varCopyAssign, EXPRLIST_EXPR(curr));

      //add this statement to our bag, corresponding to the variable used.
      VARBAG_STMTS(compilerdefvar)[0] = arrstatement;
      // Also add the pre-prepared function call statement to the bag.
      VARBAG_STMTS(funcallstat)[1] = funcallstat;  // this is going to be like a placeholder which gets filled in outside the loop.
      //// OBSERVATION (ABOVE): every var in the bag would refer to the same funcall statement and that is what we intend to do.

      if (NULL == arrstatementlist)
      {
        // Mark the tail: Its the first statement that we create in this while loop.
          arrstatementlist = TBmakeStatementlist(arrstatement, arrstatementlist);
          tail = arrstatementlist;
      }
      else
      {
        // This is not the first time we are entering the while loop if we are here.
        arrstatementlist = TBmakeStatementlist(arrstatement, arrstatementlist);
      }
      curr = EXPRLIST_NEXT(curr);
    }
    
    // Create a statement which is a funcall, passing in the exprlist formed above.
    FUNCALL_EXPRLIST(funcall) = newexprlist;
    
    // Now, we assign the __allocate() funcall to the newly created variable
    // like: <uservariable> = __allocate(__1, __2);
    yetanothervar = MEMcopy(sizeof(node), VARDEC_VAR(arg_node));
    arr_internal_assignment = TBmakeAssign(yetanothervar, EXPRLIST_EXPR(curr));
    arrstatement = TBmakeStatement(arr_internal_assignment);
    // Now collate the funcall statement and this newly added assignment statement into a new statementlist.
    arrstatementlist = TBmakeStatementlist(arrstatement, arrstatementlist);
    arrstatementlist = TBmakeStatementlist(funcallstat, arrstatementlist);
*/
    arrstatementlist = SEParraydefinitionhandler(arg_node, arg_info);
  }

  // NON-ARRAY variable declaration being handled.

  // ARRAY INIT should be handled in the next phase. Not here
  if ( NULL != VARDEC_EXPR(arg_node))
  {
    source = VARDEC_VAR(arg_node);
    var = MEMcopy(sizeof(node), source);

    assignment = TBmakeAssign(var, VARDEC_EXPR(arg_node) );
    VARDEC_EXPR(arg_node) = NULL;

    statement = TBmakeStatement(assignment);    

    if ( TRUE == INFO_ISGLOBAL(arg_info))
    { 
      if (NULL != tail)
      { 
        // Creating a list that holds the latest assignment and the ones formed
        // in the array declaration section.
        statementlist = TBmakeStatementlist(statement, arrstatementlist);  

        // Joining our newly formed array initialization statments to the 
        // recieved info structure (prefixing our list to the original)
        STATEMENTLIST_NEXT(tail) = INFO_GSTATEMENTLIST(arg_info);
        INFO_GVARDECLIST(arg_info)  = vardeclist;
      }
      else
      {
        // In case we dont have array side initializations, just create a statementlist 
        // with the assignment statement that we have.
        statementlist = TBmakeStatementlist(statement, INFO_GSTATEMENTLIST(arg_info));
      }

      //Update the info structure, with the final joined statement list. 
      //Pipe looks like: [[ GSTATMENTLIST(arg_info);<-- ASSIGNMENT||ARRAYLIST||origGSTATEMENTLIST(arg_info); ]]
      INFO_GSTATEMENTLIST(arg_info) = statementlist;

    }
    else // LSTATEMENT :)
    {
      if (NULL != tail)
      { 
        // Creating a list that holds the latest assignment and the ones formed
        // in the array declaration section.
        statementlist = TBmakeStatementlist(statement, arrstatementlist);  

        // Joining our newly formed array initialization statments to the 
        // recieved info structure (prefixing our list to the original)
        STATEMENTLIST_NEXT(tail) = INFO_LSTATEMENTLIST(arg_info);;
        INFO_LVARDECLIST(arg_info)  = vardeclist;
      }
      else
      {
        // In case we dont have array side initializations, just create a statementlist 
        // with the assignment statement that we have.
        statementlist = TBmakeStatementlist(statement, INFO_LSTATEMENTLIST(arg_info));
      }

      //Update the info structure, with the final joined statement list. 
      //Pipe looks like: [[ GSTATMENTLIST(arg_info);<-- ASSIGNMENT||ARRAYLIST||origGSTATEMENTLIST(arg_info); ]]
      INFO_LSTATEMENTLIST(arg_info) = statementlist;
    }
  } 
  else
  {
      if(NULL != tail)
      {
        if ( TRUE == INFO_ISGLOBAL(arg_info))
        {  
            STATEMENTLIST_NEXT(tail) = INFO_GSTATEMENTLIST(arg_info);
           INFO_GVARDECLIST(arg_info)  = vardeclist;
        }
        else
        {
            STATEMENTLIST_NEXT(tail) = INFO_LSTATEMENTLIST(arg_info);;
            INFO_LVARDECLIST(arg_info)  = vardeclist;
        }    
      } 
  }

  DBUG_RETURN (arg_node); 
}

node *SEPstatementlist (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPstatementlist");

  STATEMENTLIST_STATEMENT(arg_node) = TRAVdo(STATEMENTLIST_STATEMENT(arg_node), arg_info);  

  STATEMENTLIST_NEXT(arg_node) = TRAVopt(STATEMENTLIST_NEXT(arg_node), arg_info); 

  if(NULL == STATEMENTLIST_NEXT(arg_node))  
  {
    STATEMENTLIST_NEXT(arg_node) = INFO_LSTATEMENTLIST(arg_info);
  }

  DBUG_RETURN (arg_node); 
}

/*******************************************************************
 Hooks into funbody traversal to work on vardeclist transformations
********************************************************************/
node *SEPfunbody (node *arg_node, info * arg_info)
{
  DBUG_ENTER("SEPfunbody");

  FUNBODY_VARDECLIST(arg_node) =TRAVopt(FUNBODY_VARDECLIST(arg_node),arg_info);

  FUNBODY_STATEMENTLIST(arg_node) = TRAVopt(FUNBODY_STATEMENTLIST(arg_node), arg_info);

  INFO_LSTATEMENTLIST(arg_info) = NULL;

  // prepend the newly formed local vardeclist to the funbody's vardeclist.
  VARDECLIST_NEXT(INFO_LVARDECLIST(arg_info)) = FUNBODY_VARDECLIST(arg_node);

  INFO_LVARDECLIST(arg_info) = NULL;

  FUNBODY_LOCALFUNDEFLIST(arg_node) = TRAVopt(FUNBODY_LOCALFUNDEFLIST(arg_node), arg_info);

  DBUG_RETURN (arg_node); 
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
  
  // setting that now we are processing global declarations
  INFO_ISGLOBAL(arg_info) = TRUE; 

  VARDECLISTLOCALFUNDEF_VARDECLIST(arg_node) =TRAVopt(VARDECLISTLOCALFUNDEF_VARDECLIST(arg_node),arg_info);

  INFO_ISGLOBAL(arg_info) = FALSE;

  VARDECLISTLOCALFUNDEF_LOCALFUNDEF(arg_node) =TRAVopt(VARDECLISTLOCALFUNDEF_LOCALFUNDEF(arg_node),arg_info);

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
  node *statement, *statementlist, *namecollision;

  DBUG_ENTER("SEPusualdef");
  
  USUALDEF_VAR(arg_node) =TRAVdo(USUALDEF_VAR(arg_node),arg_info);
  USUALDEF_EXPR(arg_node) =TRAVopt(USUALDEF_EXPR(arg_node),arg_info); 
  if(ispotentialcollision(USUALDEF_VAR(arg_node))
    {
      // NOTE: NEVER use VARBAG_STMTLIST(n) here!
       namecollision = MEMmalloc(sizeof(varbag));
       VARBAG_TYPE(namecollision)=USUALDEF;
       VARBAG_DECL(namecollision) = USUALDEF_VAR(arg_node); // pass the danger variable.
       VARBAG_NEXT(namecollision) = INFO_USERDEFVARS(arg_info);
       INFO_USERDEFVARS(arg_info) = namecollision;
    } 
  
  if (NULL != USUALDEF_EXPR(arg_node))
  {
    source = USUALDEF_VAR(arg_node);
    var = MEMcopy(sizeof(node), source);

    assignment = TBmakeAssign(var, USUALDEF_EXPR(arg_node) );
    USUALDEF_EXPR(arg_node) = NULL;

    statement = TBmakeStatement(assignment);
    statementlist = TBmakeStatementlist(statement, INFO_GSTATEMENTLIST(arg_info));
    INFO_GSTATEMENTLIST(arg_info) = statementlist;
  }

  DBUG_RETURN (arg_node); 
}

node *SEParraydef (node *arg_node, info * arg_info)
{
  node *curr, *tail, *vardec, *var, *varCopyExpr, *varCopyAssign, *funcall;
  node *statement, *funvar, *yetanothervar, *source, *assignment, *arr_internal_assignment;
  node *vardeclist, *statementlist, *newexprlist, *funcallstat, *arrstatement, *arrstatementlist;

  DBUG_ENTER("SEParraydef");

  ARRAYDEF_EXPRLIST(arg_node) = TRAVdo(ARRAYDEF_EXPRLIST(arg_node), arg_info);  

  ARRAYDEF_VAR(arg_node) = TRAVdo(ARRAYDEF_VAR(arg_node), arg_info); 

    if(ispotentialcollision(ARRAYDEF_VAR(arg_node))
    {
      // NOTE: NEVER use VARBAG_STMTLIST(n) here!
       namecollision = MEMmalloc(sizeof(varbag));
       VARBAG_TYPE(namecollision)=ARRAYDEF;
       VARBAG_DECL(namecollision) = ARRAYDEF_VAR(arg_node); // pass the danger variable.
       VARBAG_NEXT(namecollision) = INFO_USERDEFVARS(arg_info);
       INFO_USERDEFVARS(arg_info) = namecollision;
    }
  
  arrstatementlist = SEParraydefinitionhandler(arg_node, arg_info);
  
  if(NULL != tail)
  {
    STATEMENTLIST_NEXT(tail) = INFO_GSTATEMENTLIST(arg_info);
    INFO_GVARDECLIST(arg_info)  = vardeclist;
  } 
  DBUG_RETURN (arg_node); 
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
 *************************************************************************** /
/******************************************************************************
   Function that adds __init() to the syntax tree
   NOTE: syntax tree is modified by this function 
*******************************************************************************/
void *SEPintroduceInit(node *syntaxtree, info *info)
{
  node *var, *header, *funbody, *init; 
  node *vardeclistfundef, *program;

  DBUG_ENTER("SEPintroduceInit");
  
  if ( NULL == syntaxtree || NULL == INFO_GSTATEMENTLIST(info))
  {
    return syntaxtree;
  }

  var     = TBmakeVar("__init", NULL);
  header  = TBmakeFunheader(T_void, var, NULL);
  funbody = TBmakeFunbody(NULL, NULL, INFO_GSTATEMENTLIST(info), NULL);
  init = TBmakeLocalfundef(header, funbody);
  
  vardeclistfundef = TBmakeVardeclistlocalfundef(NULL, init);
  program = TBmakeProgram(vardeclistfundef, syntaxtree);
  
  DBUG_RETURN (arg_node); 
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



