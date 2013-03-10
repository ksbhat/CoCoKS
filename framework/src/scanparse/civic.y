%{


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

#include "types.h"
#include "tree_basic.h"
#include "str.h"
#include "dbug.h"
#include "ctinfo.h"
#include "free.h"
#include "globals.h"

//#ifdef DEBUG
	#define DEBUG(msg) \
		printf("%s:\t%s\n", "DEBUG",msg);
/*#else
	#define DEBUG(msg) ;
#endif
*/

static node *parseresult = NULL;
extern int yylex();
static int yyerror( char *errname);

%}

%union {
 nodetype            nodetype;
 char               *id;
 int                 cint;
 float               cflt;
 binop               cbinop;
 monop				 cmonop;
 type				 ctype;
 node               *node;
}

%token BRACKET_L BRACKET_R COMMA SEMICOLON
%token LE LT GE GT EQ NE OR AND
//priority from low to high
%left OR
%left AND
%left EQ NE
%left LT LE GE GT
%left PLUS MINUS
%left STAR SLASH PERCENT

%token TRUEVAL FALSEVAL LET

%token <cint> NUM
%token <cflt> FLOAT
%token <id> ID
%token EXTERN EXPORT TYPE_VOID TYPE_INT TYPE_BOOL TYPE_FLOAT RETURN 
%token FL_BRACKET_L FL_BRACKET_R SQ_BRACKET_L SQ_BRACKET_R
%token FOR DO WHILE  
%nonassoc IF
%nonassoc ELSE
%token NOT
%type <node> intval floatval boolval constant 
//%type <node> assign varlet program
%type <node> program expr 
%type <node> codeblock globaldef setglobaldef usualdef initProgram
%type <node> unaryandcast exprfunctioncall exprsequence groupedexpression
%type <node> assign
%type <node> block statementlist statement funcall fundef funheader param
%type <node> paramsequence funbody vardeclist return vardec
%type <node> ifblock elseblock whileblock dowhileblock forblock forinitstat
%type <cbinop> binop
%type <cmonop> monOp
%type <ctype> type

%start initProgram 

%%

initProgram :program
	{
		parseresult=$1;
	}
	;

program:codeblock 
		{

			$$=TBmakeProgram($1,NULL);
			DEBUG("ACCEPT");
		}
		;

codeblock:globaldef
		{
			$$=$1;
		} 
		|
		fundef
		{
			$$=$1;
		}
		;

globaldef:setglobaldef
		{
			$$=TBmakeGlobaldef($1);
		}
		;
setglobaldef:usualdef
		{
			$$=$1;
		}
		;

usualdef:EXPORT type ID LET expr SEMICOLON
		{
			DEBUG("IN USUALDEF");
			node* var=TBmakeVar(STRcpy($3),NULL);
			DEBUG("BW IN USUALDEF");
			$$=TBmakeUsualdef(FALSE,$2,var,$5);
			DEBUG("AFTER IN USUALDEF");
		}
		;

fundef: EXPORT funheader FL_BRACKET_L funbody FL_BRACKET_R
	{
		$$=TBmakeFundef(TRUE,$2,$4);
	}
	|
	funheader FL_BRACKET_L funbody FL_BRACKET_R
	{
		$$=TBmakeFundef(FALSE,$1,$3);
	}
	;

funheader:  type ID BRACKET_L paramsequence BRACKET_R
	     {
		node* var=TBmakeVar(STRcpy($2),NULL);
		$$=TBmakeFunheader($1,var,$4);
	     } 
	     |
	     TYPE_VOID ID BRACKET_L paramsequence BRACKET_R
	     {
		node* var=TBmakeVar(STRcpy($2),NULL);
		$$=TBmakeFunheader(T_void,var,$4);
	     } 
	     |
	     TYPE_VOID ID BRACKET_L BRACKET_R
	     {
		node* var=TBmakeVar(STRcpy($2),NULL);
		$$=TBmakeFunheader(T_void,var, NULL);
	     } 
	     |
	     type ID BRACKET_L BRACKET_R
             {
                node* var=TBmakeVar(STRcpy($2),NULL);
                $$=TBmakeFunheader($1,var,NULL);
             }
		;

paramsequence : paramsequence COMMA param
                {
                        DEBUG("PARAM SEQUENCE");
                        $$=TBmakeParamlist($3,$1);
                }
              | param
                {
                        DEBUG("END oF PARAM SEQUENCE");
                        $$=TBmakeExprlist($1,NULL);
                }
                ;

param :type ID
		{
                	node* var=TBmakeVar(STRcpy($2),NULL);
			$$=TBmakeParam($1,NULL,var);
		}
		;


funbody : vardeclist statementlist return
	 {
		DEBUG("IN Funbody with return");
		$$=TBmakeFunbody($1,NULL,$2,$3);	
		DEBUG("after Funbody with return");
	 }	
	|
	  vardeclist statementlist
	{
		DEBUG("IN Funbody without return");
		$$=TBmakeFunbody($1,NULL,$2,NULL);
		DEBUG("after Funbody without return");
	}


return : RETURN expr SEMICOLON
	{
		$$=TBmakeReturn($2);
	}
	;

vardeclist:vardec 
		{
			$$=TBmakeVardeclist($1,NULL);
		}
		|
		vardeclist vardec 
		{
			$$=TBmakeVardeclist($2,$1);
		}
		;	


vardec	: type ID LET expr SEMICOLON
		{
			DEBUG("IN VARDEC With initial");
			node* var=TBmakeVar(STRcpy($2),NULL);
			$$= TBmakeVardec($1,NULL,var,$4);
		}
		|
	  type ID SEMICOLON
		{
			DEBUG("IN VARDEC");
			node* var=TBmakeVar(STRcpy($2),NULL);
			$$= TBmakeVardec($1,NULL,var,NULL);
		}
		;
	     

assign: ID LET expr SEMICOLON
		 {
			DEBUG("IN ASSIGN");
			node* varlet=TBmakeVarlet(STRcpy($1),NULL);
		 	$$ = TBmakeAssign(varlet,$3);
		 }
		;


block		:statement
		{
			DEBUG("IN BLOCK");
			$$=$1;
		}
		| FL_BRACKET_L statementlist FL_BRACKET_R
		{
			DEBUG("IN Enclosed BLOCK");
			$$=TBmakeEnclosedblock($2);
		}
		;
		
statementlist:statement 
		{
			DEBUG("IN STATEMENT OF STATEMENTLIST");
			$$=TBmakeStatementlist($1,NULL);
		}
		|
		statementlist statement
		{
			DEBUG("IN STATEMENTLIST OF STATEMENTLIST");
			$$=TBmakeStatementlist($2,$1);
		}
		;		

		
statement: assign
		{
			DEBUG("IN NONCONDITIONAL : ASSIGN");
			$$=TBmakeStatement($1);
		}
		|
		funcall
		{
			DEBUG("IN NONCONDITIONAL : FUNCALL");
			$$=TBmakeStatement($1);
		}
		|
		ifblock
		{
			DEBUG("IN NONCONDITIONAL : IFBlock");
			$$=TBmakeStatement($1);
		
		}
		|
		whileblock
		{
			DEBUG("IN NONCONDITIONAL : WHILEBlock");
			$$=TBmakeStatement($1);
		}
		|
		dowhileblock
		{
			DEBUG("IN NONCONDITIONAL : DO WHILE Block");
			$$=TBmakeStatement($1);
		}
		|
		forblock
		{
			DEBUG("IN NONCONDITIONAL : FOR Block");
			$$=TBmakeStatement($1);
		}
		;

ifblock:IF BRACKET_L expr BRACKET_R block elseblock
		{
			DEBUG("IN IFELSEBLOCK");
			$$=TBmakeIfstat($3,$5,$6);
		} 
		|
	IF BRACKET_L expr BRACKET_R block %prec IF
		{
			DEBUG("IN IFBLOCK");
			$$=TBmakeIfstat($3,$5,NULL);
		}
		;
elseblock: ELSE block
		{
			DEBUG("IN ELSE BLOCK");
			$$=TBmakeElseblock($2);
		}
		;

whileblock 	:	WHILE BRACKET_L expr BRACKET_R block 
		{
			DEBUG("IN WHILE BLOCK");
			$$=TBmakeWhilestat($3, $5);
		}
		;

dowhileblock	:	DO block WHILE BRACKET_L expr BRACKET_R SEMICOLON
		{
			DEBUG("IN DO WHILE BLOCK");
			$$=TBmakeDowhilestat($2, $5);
		}
		;	

forblock	:	FOR BRACKET_L forinitstat COMMA expr COMMA expr BRACKET_R block
		{
			DEBUG("IN FOR LOOP");
			$$=TBmakeForstat($3, $5, $7, $9);
		}
		|	FOR BRACKET_L forinitstat COMMA expr  BRACKET_R block
		{
			DEBUG("IN FOR LOOP WITHOUT STEP");
			$$=TBmakeForstat($3, $5, NULL, $7);
		}
		;


forinitstat	:	TYPE_INT ID LET expr
		{
			DEBUG("IN FOR VARDEC PART");
			node* var=TBmakeVar(STRcpy($2),NULL);
			$$=TBmakeVardec(T_int, NULL, var, $4);
		}
		;

funcall:ID BRACKET_L exprsequence BRACKET_R SEMICOLON//function call
                {
                        DEBUG("FUNCTION CALL");
                        node *var=TBmakeVar(STRcpy($1),NULL);
                        $$=TBmakeFuncall(var,$3);
                }       
                |ID BRACKET_L BRACKET_R SEMICOLON
                {
                        DEBUG("FUNCTION CALL WITHOUT PARAM");
			printf("the value %s",$1);
                        node *var1=TBmakeVar(STRcpy($1),NULL);
			DEBUG("AFTER CREATING VAR");
                        $$=TBmakeFuncall(var1,NULL);
			DEBUG("ABOUT TO EXIT");
                }
		;



expr: expr binop unaryandcast  
		{
			DEBUG("OTHER BINOP");
			$$ = $1;
		}
	|
	expr MINUS unaryandcast
		{
			DEBUG(" BINOP TESTMINUS");
			binop symbol=BO_sub;
			$$=TBmakeBinop(symbol,$1,$3);
			
		}
	| unaryandcast
		{
			DEBUG(" UNARY AND CAST");
			$$ = $1;
		}
		;
	 

unaryandcast : monOp exprfunctioncall 
		{
			DEBUG(" MONOP TESTUNARY");
			if($2==NULL)
			{
				printf("inside MONOP NULL");
				exit(1);	
			}
			$$ = TBmakeMonop($1,$2);
			DEBUG(" AFTER MONOP TESTUNARY");
		}
		| MINUS exprfunctioncall
		{
			monop symbol=MO_neg;
			DEBUG("MINUS TESTUNARY");
			$$ = TBmakeMonop(symbol,$2);
		}
		| BRACKET_L type BRACKET_R exprfunctioncall //casting expression
		{
			DEBUG("BRACKET TESTUNARY");
			$$ = TBmakeCast($2,$4);
		}
		| exprfunctioncall 
		{
			DEBUG("CONSTANT TESTUNARY");
			$$ = $1;
		}
		;


exprfunctioncall:ID BRACKET_L exprsequence BRACKET_R //function call
		{
			DEBUG("FUNCTION CALL");
			node *var=TBmakeVar(STRcpy($1),NULL);
			$$=TBmakeFuncall(var,$3);
		}       
		| ID BRACKET_L BRACKET_R
		{
			DEBUG("FUNCTION CALL");
			node *var=TBmakeVar(STRcpy($1),NULL);
			$$=TBmakeFuncall(var,NULL);
		}
		| groupedexpression
		{
			$$=$1;
		}
		;


exprsequence : expr COMMA exprsequence
		{
			DEBUG("EXPR SEQUENCE");
			$$=TBmakeExprlist($1,$3);
		}
	      | expr
		{
			DEBUG("END oF  EXPR SEQUENCE");
			$$=TBmakeExprlist($1,NULL);
		}
	
		;
groupedexpression: BRACKET_L expr BRACKET_R
		{
			$$=$2;
		}
		| ID
                {
                        DEBUG("ID CAST EXPRESSION");
                        $$ = TBmakeVar(STRcpy($1),NULL);

                }
                |  constant
                {
                        DEBUG("CONSTANT CAST EXPRESSION");
                        $$=$1;
                }
                ;

constant: floatval
          {
            $$ = $1;
          }
        | intval
          {
            $$ = $1;
          }
        | boolval
          {
            $$ = $1;
          }
        ;

floatval: FLOAT
           {
             $$ = TBmakeFloat( $1);
           }
         ;

intval: NUM
        {
          $$ = TBmakeNum( $1);
        }
      ;

boolval: TRUEVAL
         {
           $$ = TBmakeBool( TRUE);
         }
       | FALSEVAL
         {
           $$ = TBmakeBool( FALSE);
         }
       ;

binop: PLUS      { $$ = BO_add; }
     | STAR      { $$ = BO_mul; }
     | SLASH     { $$ = BO_div; }
     | PERCENT   { $$ = BO_mod; }
     | LE        { $$ = BO_le; }
     | LT        { $$ = BO_lt; }
     | GE        { $$ = BO_ge; }
     | GT        { $$ = BO_gt; }
     | EQ        { $$ = BO_eq; }
     | OR        { $$ = BO_or; }
     | AND       { $$ = BO_and; }
     ;
     
monOp: 	NOT		{$$	= MO_not;}


type: 	 TYPE_INT		{$$ = T_int;} 
	 |TYPE_BOOL 		{$$ = T_bool;}
	 |TYPE_FLOAT 	{$$ = T_float;}

	 
%%



static int yyerror( char *error)
{
  CTIabort( "line %d, col %d\nError parsing source code: %s\n", 
            global.line, global.col, error);

  return( 0);
}

node *YYparseTree( void)
{
  DBUG_ENTER("YYparseTree");

  yyparse();

  DBUG_RETURN( parseresult);
}


