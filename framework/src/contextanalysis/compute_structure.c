#include <stdio.h>
#define UNKOWN 0
#define INT 1
#define FLOAT 2
#define BOOL 4


typedef {arithmeticoperation, logicaloperation, relationaloperation}opertionclassify;

typedef {binop,monop}operationtype;

typedef {inputtype,outputtype}parametertype;

struct operationmetadata {
type inputtype;
type outputtype;
}listoperation[3];


struct binoperation 
{
  type left;
  type right;
  type result;
  binop operation;
};

struct monoperation 
{
  type input;
  type result;
  monop operation;
};

//input parameter from the user.
struct operationonparameter
{
  operationtype optype;
  union {
    binoperation *binaryop; //this is the reference structure for binary operations
    monoperation *monop;  // this is the reference structure for mono operations
  }operation;
  char **errorstring;
};


char **parametervalue={"input","output"};


char* printExpectedError(parametertype param)
{
   char *errorstring,oldstring;
    if(param==inputtype)
    {
      errorstring=STRcat("Expect ",parametervalue[0]);
    } 
    else
    {
      errorstring=STRcat("Expect ",parametervalue[1]);
    }
    oldstring=errorstring;
    errorstring=STRcat(errorstring," to be Integer");
    
    return errorstring;
}


char* printBooleanExpectedError(parametertype param)
{
    char *errorstring,oldstring;
    if(param==inputtype)
    {
      errorstring=STRcat("Expect ",parametervalue[0]);
    } 
    else
    {
      errorstring=STRcat("Expect ",parametervalue[1]);
    }
    oldstring=errorstring;
    errorstring=STRcat(errorstring," to be Boolean");
    
    return errorstring;
}

char* printExpectedError(parametertype param,type input)
{
    char *errorstring,oldstring;
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
    if(input==T_int)
    {
      errorstring=STRcat(errorstring," Integer");
    }
    else if(input==T_float)
    {
      errorstring=STRcat(errorstring," T_Float"); 
    }
    else
    {
      errorstring=STRcat(errorstring," T_Bool");  
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



bool parameterverification(Type input,operationclassify oc,parametertype param)
{
  int result;
  int number;
  if(param==inputtype)
  {  
      result=input & listoperation[oc].inputparameter;
  }
  else
  {
    result=input & listoperation[oc].outputparameter;
  } 
  if(result==input)
  {
    return true;
  }
  else
  {
    return false;
  }
}


int classifyoperation(struct operationonparameter *opparameter)
{
  
  operation optype=opparameter->optype;
  if(optype==Monop)
  {
      binop bop=inputparameter->operation.binoperation;

      if(bop==BO_add|| bop== BO_sub|| bop==BO_mul || bop==BO_div ||bop== BO_mod)
      {
        return arithmeticoperation;
      }
      else if(bop== BO_lt||bop==BO_le||bop==BO_gt|| bop==BO_ge||bop==BO_eq||bop==BO_ne)
      {
        return logicaloperation;
      }
      else (bop==BO_or || bop== BO_and)
      {
        return relationaloperation;
      }

  }
  else
  {
    monop mop=inputparameter->operation.monop;
    if(mop==MO_minus)
    {
      return arithmeticoperation;
    }
    else
      return logicaloperation;
  }
}


int checkoperation(struct operationonparameter *opparameter)
{
    struct binoperation *binop;
    struct monooperation *monop;
    bool flag;
    opertionclassify oc;
    operation optype=opparameter->optype;
    oc=classifyoperation(opparameter);
    if(optype==Monop)
    {
        monop=opparameter->operation.monoperation;
        
        flag=parmeterverifiation(monop->input,oc,inputtype);
        if(!flag)
        {
         opparameter->errorstring=&(printExpectedError(inputtype,opparameter));
         return FALSE;
        }
        flag=parmeterverifiation(monop->result,oc,outputtype);
        if(!flag)
        {
         opparameter->errorstring=&(printExpectedError(outputtype,opparameter));
          return FALSE;
        } 
        return TRUE;
    }
    else
    {
        binop=opparameter->operation.binoperation;
        flag=parameterverification(binop->left,oc,inputtype);
        if(!flag)
        {
          opparameter->errorstring=&(printExpectedError(inputtype));
          return FALSE;
        }
        else 
         {
            flag=parameterverification(binop->rightoc,oc,inputtype);
            if(!flag)
            {
              opparameter->errorstring=&printExpectedError(inputtype,opparameter);
              return FALSE;
            }   
         } 
         flag=parameterverification(binop->result,oc,outputtype);
         if(!flag)
         {
          printExpectedErroropparameter->errorstring=&(outputtype,opparameter);
          return FALSE;
         }
         return TRUE;
    }
} 

