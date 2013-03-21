#include <stdio.h>
#define UNKOWN 0
#define INT 1
#define FLOAT 2
#define BOOL 4


typedef enum {arithmeticoperation, logicaloperation, relationaloperation} operationclassify;

typedef enum{binaryoperation,monoperation} operationtype;

typedef enum {inputtype,outputtype}parametertype;

struct operationmetadata {
type inputtype;
type outputtype;
}listoperation[3];


typedef struct binoperation 
{
  type left;
  type right;
  binop operation;
}BINOPERATION;

typedef struct monoperation 
{
  type input;
  monop operation;
}MONOPERATION;

//input parameter from the user.
typedef struct operationonparameter
{
  operationtype optype;
  union {
    struct binoperation *binaryop; //this is the reference structure for binary operations
    struct monoperation *monop;  // this is the reference structure for mono operations
  }operation;
  char **errorstring;
} OPERATIONONPARAMETER;


char *parametervalue[]={"input","output"};

char* printExpectedError(parametertype param,type input)
{
    char *errorstring,*oldstring;
    if(param==inputtype)
    {
      errorstring=STRcat("Expect ",parametervalue[0]);
    } 
    else
    {
      errorstring=STRcat("Expect ",parametervalue[1]);
    }
    oldstring=errorstring;
    errorstring=STRcat(errorstring," to be");
    MEMfree(oldstring);
    if(input==T_int)
    {
      errorstring=STRcat(errorstring," Integer");
    }
    else if(input==T_float)
    {
      errorstring=STRcat(errorstring," Float"); 
    }
    else
    {
      errorstring=STRcat(errorstring," Bool");  
    }
    return errorstring;
}
 

char* printMismatchError(parametertype param,type input)
{
    char *errorstring,*oldstring;
    if(param==inputtype)
    {
      errorstring=STRcat("Mismatch ",parametervalue[0]);
    } 
    
    oldstring=errorstring;
    errorstring=STRcat(errorstring," Expect both to be");
    MEMfree(oldstring);
    if(input==T_int)
    {
      errorstring=STRcat(errorstring," Integer");
    }
    else if(input==T_float)
    {
      errorstring=STRcat(errorstring," Float"); 
    }
    else
    {
      errorstring=STRcat(errorstring," Bool");  
    }
    return errorstring;
} 

void init_arithmetic_operation()
{
  listoperation[arithmeticoperation].inputtype=INT|FLOAT;
  listoperation[arithmeticoperation].outputtype=INT|FLOAT;
}



void init_logical_operation()
{
  listoperation[logicaloperation].inputtype=INT|FLOAT;
  listoperation[logicaloperation].outputtype=BOOL; 
}



void init_relation_operation()
{
  listoperation[relationaloperation].inputtype=BOOL;
  listoperation[relationaloperation].outputtype=BOOL;  
}



void init_operation()
{
	
	init_arithmetic_operation();

  init_logical_operation();

  init_relation_operation();
} 



bool parameterverification(type input,operationclassify oc,parametertype param)
{
  int result;

  if(param==inputtype)
  {  
      result=input & listoperation[oc].inputtype;
  }
  else
  {
    result=input & listoperation[oc].outputtype;
  } 
  if(result==input)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}


int classifyoperation(struct operationonparameter *opparameter)
{
  
  operationtype optype=opparameter->optype;
  if(optype==binaryoperation)
  {
      binop bop=opparameter->operation.binaryop->operation;

      if(bop==BO_add|| bop== BO_sub|| bop==BO_mul || bop==BO_div ||bop== BO_mod)
      {
        return arithmeticoperation;
      }
      else if(bop== BO_lt||bop==BO_le||bop==BO_gt|| bop==BO_ge||bop==BO_eq||bop==BO_ne)
      {
        return logicaloperation;
      }
      else 
      {
        return relationaloperation;
      }

  }
  else
  {
    monop mop=opparameter->operation.monop->operation;
    if(mop==MO_neg)
    {
      return arithmeticoperation;
    }
    else
      return logicaloperation;
  }
}

// Checks the types of the operands for the specified operation.
// TRUE if valid.
bool checkoperation(struct operationonparameter *opparameter)
{
    struct binoperation *binop;
    struct monoperation *monop;
    bool flag;
    operationclassify oc;
    char *result;
    oc=classifyoperation(opparameter);
    operationtype optype=opparameter->optype;
    if(optype==monoperation)
    {
        monop=opparameter->operation.monop;
        
        flag=parameterverification(monop->input,oc,inputtype);
        if(!flag)
        {
          result=printExpectedError(monop->input,inputtype);
         opparameter->errorstring=&result;
         return FALSE;
        }
        
        return TRUE;
    }
    else
    {
        binop=opparameter->operation.binaryop;
        if ( binop->left != binop->right)
        {
          result=printMismatchError(binop->left,inputtype);
          opparameter->errorstring=&result;
        }
        flag=parameterverification(binop->left,oc,inputtype);
        if(!flag)
        {
          result=printExpectedError(binop->left,inputtype);
          opparameter->errorstring=&result;
          return FALSE;
        }
        else 
         {
            flag=parameterverification(binop->right,oc,inputtype);
            if(!flag)
            {
              result=printExpectedError(binop->right, inputtype);
              opparameter->errorstring=&result;
              return FALSE;
            }   
         } 
         return TRUE;
    }
} 

