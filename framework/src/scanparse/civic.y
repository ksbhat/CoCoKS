/***************************************************
* Authors: 
* --------
* Koustubha Bhat	,	Sharan S. Santhanam
* UvA ID: 10513892		UvA ID:
* 
* Compiler Construction course - Feb/March 2013
* Universiteit van Amsterdam.
****************************************************/
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
 char                *id;
 int                 cint;
 float               cflt;
 binop               cbinop;
 monop               cmonop;
 type			     ctype;
 node                *node;
}

%token BRACKET_L BRACKET_R COMMA SEMICOLON
%token LE LT GE GT EQ NE OR AND NOT
%token TRUEVAL FALSEVAL LET
%token EXTERN EXPORT TYPE_VOID TYPE_INT TYPE_BOOL TYPE_FLOAT RETURN 
%token FL_BRACKET_L FL_BRACKET_R SQ_BRACKET_L SQ_BRACKET_R
%token FOR DO WHILE  
%token <cint> NUM
%token <cflt> FLOAT
%token <id> ID
%type <node> intval floatval boolval constant 
%type <node> program expr 
%type <node> codeblock globaldef setglobaldef usualdef initProgram
%type <node> unaryandcast exprfunctioncall exprsequence groupedexpression
%type <node> assign fundec globaldec exportfundef funbodyleft funbodyright
%type <node> block statementlist statement funcall localfundef funheader param
%type <node> paramsequence funbody localfundeflist vardeclist return vardec
%type <node> ifblock elseblock whileblock dowhileblock forblock forinitstat 
%type <node> arrayindexing varnumsequence arraydef arrayinit localvardeclistfundef
%type <cbinop> binop
%type <cmonop> monOp
%type <ctype> type

//priority from low to high
%left OR
%left AND
%left EQ NE
%left LT LE GE GT
%left PLUS MINUS
%left STAR SLASH PERCENT
%nonassoc IF
%nonassoc ELSE
%nonassoc CODEBLOCK
%nonassoc LCLFUNDEF
%start initProgram 

%%

initProgram	 	:	program
				{
					parseresult=$1;
					DEBUG("ACCEPT");
				}
				;

program			:	codeblock
				{
					$$=TBmakeProgram($1,NULL);
					DEBUG("IN PROGRAM : CODEBLOCK");
				}
				| program codeblock
				{	
					DEBUG("IN PROGRAM : CODEBLOCK LIST (program)");
					$$=TBmakeProgram($2, $1);
				}
				;

codeblock		:	globaldef
				{
					DEBUG("IN CODEBLOCK: globaldef");
					$$=$1;
				} 
				|
				globaldec
				{
					DEBUG("IN CODEBLOCK: globaldec");
					$$=$1;
				}
				|
				localvardeclistfundef	
				{
					DEBUG("IN CODEBLOCK: fundef");
					$$=$1;
				}
				|
				exportfundef
				{
					$$=$1;
				}
				|
				fundec
				{
					DEBUG("IN CODEBLOCK: fundec");
					$$=$1;
				}
				;

localvardeclistfundef		: vardec
				{
					node *vardeclist=TBmakeVardeclist($1,NULL);
					$$=TBmakeVardeclistlocalfundef(vardeclist,NULL );
				}
				|
				localfundef
				{
					$$=TBmakeVardeclistlocalfundef(NULL,$1 );
					
				}
				;

globaldef 		:	setglobaldef
				{
					DEBUG("IN GLOBALDEF: setglobaldef");
					$$=TBmakeGlobaldef($1);
				}
				;

setglobaldef 		:	usualdef
				{
					DEBUG("IN GLOBALDEF: usualdef");
					$$=$1;
				}
				|
				arraydef
				{
					DEBUG("IN GLOBALDEF: arraydef");
					$$=$1;
				}
				;

usualdef 		:	EXPORT type ID LET expr SEMICOLON
				{
					DEBUG("IN USUALDEF");
					node* var=TBmakeVar(STRcpy($3),NULL);
					$$=TBmakeUsualdef(TRUE,$2,var,$5);
				}
				|EXPORT type ID SEMICOLON
				{
					DEBUG("IN USUALDEF");
					node* var=TBmakeVar(STRcpy($3),NULL);
					$$=TBmakeUsualdef(TRUE,$2,var,NULL);
				}
				;
				;

arraydef 		:	EXPORT type SQ_BRACKET_L exprsequence SQ_BRACKET_R ID SEMICOLON
				{
					DEBUG("IN ARRAYDEF");
					node* var=TBmakeVar(STRcpy($6),NULL);
					$$=TBmakeArraydef(TRUE,$2,$4,var);
				}
				;

globaldec		:	EXTERN type ID SEMICOLON
				{
					DEBUG("IN GLOBALDEC");
			               	node* var=TBmakeVar(STRcpy($3), NULL);
					$$=TBmakeGlobaldec($2, TRUE, NULL, var);
				}
				| EXTERN type SQ_BRACKET_L varnumsequence SQ_BRACKET_R ID SEMICOLON
				{
					DEBUG("IN GLOBALDEC : ARRAYS");
			               	node* var=TBmakeVar(STRcpy($6), NULL);
					$$=TBmakeGlobaldec($2, TRUE, $4, var);
					
				}
				;


varnumsequence 		: 	varnumsequence COMMA ID
                		{
                        		DEBUG("PARAM SEQUENCE");
					node* var=TBmakeVar(STRcpy($3), NULL);
                  		      	$$=TBmakeVarnumlist(var,$1);
                		}
	            		|
	            		varnumsequence COMMA intval
                		{
                        		DEBUG("END oF PARAM SEQUENCE");
                  		      	$$=TBmakeVarnumlist($3,$1);
                		}
				| intval
				{
                        		DEBUG("END oF PARAM SEQUENCE");
                  		      	$$=TBmakeVarnumlist($1,NULL);
				}
				| ID
				{
					node* var=TBmakeVar(STRcpy($1), NULL);
                  		      	$$=TBmakeVarnumlist(var, NULL);
				}
                		;

exportfundef		: 	EXPORT localfundef //funheader FL_BRACKET_L funbody FL_BRACKET_R
				{
					DEBUG("IN EXPORT FUNDEF");
					$$=TBmakeFundef(TRUE, $2);
				}

localfundef 		: 	funheader funbody
				{
					DEBUG("IN FUNDEF with EXPORT");
					$$=TBmakeLocalfundef($1,$2);
				}
				;

localfundeflist		:	localfundeflist localfundef 
				{
					$$=TBmakeLocalfundeflist($2, $1);
				}
				| localfundef
				{
					$$=TBmakeLocalfundeflist($1, NULL);
				}
				;

fundec			:	EXTERN funheader SEMICOLON
				{
					DEBUG("IN FUNDEC");
					$$=TBmakeFundec(TRUE, $2);
				}
				;

funheader 		:  	type ID BRACKET_L paramsequence BRACKET_R
				{
					DEBUG("IN FUNHEADER");
					node* var=TBmakeVar(STRcpy($2),NULL);
					$$=TBmakeFunheader($1,var,$4);
				} 
				|
				TYPE_VOID ID BRACKET_L paramsequence BRACKET_R
				{
					DEBUG("IN FUNHEADER type: void");
					node* var=TBmakeVar(STRcpy($2),NULL);
					$$=TBmakeFunheader(T_void,var,$4);
				} 
				|
				TYPE_VOID ID BRACKET_L BRACKET_R
				{
					DEBUG("IN FUNHEADER type: void, without params");
					node* var=TBmakeVar(STRcpy($2),NULL);
					$$=TBmakeFunheader(T_void,var, NULL);
				} 
				|
	     			type ID BRACKET_L BRACKET_R
             			{
					DEBUG("IN FUNHEADER without params");
                			node* var=TBmakeVar(STRcpy($2),NULL);
                			$$=TBmakeFunheader($1,var,NULL);
             			}
				;

paramsequence 		: 	paramsequence COMMA param
                		{
                        		DEBUG("PARAM SEQUENCE");
                  		      	$$=TBmakeParamlist($3,$1);
                		}
	            		|
	            		param
                		{
                        		DEBUG("END oF PARAM SEQUENCE");
                        		$$=TBmakeParamlist($1,NULL);
                		}
                		;

param 			:	type ID
				{
			               	node* var=TBmakeVar(STRcpy($2),NULL);
					$$=TBmakeParam($1,NULL,var);
				}
				|
				type SQ_BRACKET_L varnumsequence SQ_BRACKET_R ID
				{
			               	node* var=TBmakeVar(STRcpy($5),NULL);
					$$=TBmakeParam($1,$3,var);
				}				
				;

funbody			:	funbodyleft funbodyright	
				{
					if ( FUNBODY_STATEMENTLIST($2) != NULL
						|| FUNBODY_RETURN($2) != NULL
						|| FUNBODY_VARDECLIST($1) != NULL
						|| FUNBODY_LOCALFUNDEFLIST($1) != NULL 
					   )
					{
						FUNBODY_STATEMENTLIST($1) = FUNBODY_STATEMENTLIST($2);
						FUNBODY_RETURN($1) = FUNBODY_RETURN($2);				
						$$=$1;
						MEMfree($2);
					}
					else
					{
						MEMfree($1);
						MEMfree($2);
						$$=NULL;
					}
				}
				;

funbodyleft		:	FL_BRACKET_L vardeclist localfundeflist
				{
					$$=TBmakeFunbody($2, $3, NULL, NULL);
				}
				| FL_BRACKET_L vardeclist 
				{
					$$=TBmakeFunbody($2, NULL, NULL, NULL);
				}
				;

funbodyright		: 	statementlist return FL_BRACKET_R
				{
					$$=TBmakeFunbody(NULL, NULL, $1, $2);
				}
				| statementlist FL_BRACKET_R
				{
					$$=TBmakeFunbody(NULL, NULL, $1, NULL);
				}
				| return FL_BRACKET_R
				{
					$$=TBmakeFunbody(NULL, NULL, NULL, $1);
				}
				| FL_BRACKET_R
				{
					$$=TBmakeFunbody(NULL, NULL, NULL, NULL);
				}
				;

return 			:	RETURN expr SEMICOLON
				{
					DEBUG("IN RETURN expr");
					$$=TBmakeReturn($2);
				}
				;

vardeclist 		:	vardeclist vardec
				{
					DEBUG("IN VARDECLIST");
					$$=TBmakeVardeclist($2,$1);
					DEBUG("AFTER VARDECLIST");
				}
				| 
				/* empty epsilon */ ////vardec
				{	//nothing
					DEBUG("IN VARDECLIST: EPSILON");
					////$$=TBmakeVardeclist($1, NULL);
					$$=NULL;
				}
				;	

vardec			: 	type ID LET expr SEMICOLON
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
				|
			 	type SQ_BRACKET_L exprsequence SQ_BRACKET_R ID LET expr SEMICOLON
				{
					DEBUG("IN VARDEC With initial, for arrays");
					node* var=TBmakeVar(STRcpy($5),NULL);
					$$= TBmakeVardec($1,$3,var,$7);
				}
				|
				type SQ_BRACKET_L exprsequence SQ_BRACKET_R ID SEMICOLON
				{
					DEBUG("IN VARDEC, for arrays");
					node* var=TBmakeVar(STRcpy($5),NULL);
					$$= TBmakeVardec($1,$3,var,NULL);
				}
				;
	     
assign 			: 	ID LET expr SEMICOLON
				{
					DEBUG("IN ASSIGN");
					node* var=TBmakeVar(STRcpy($1),NULL);
				 	$$ = TBmakeAssign(var,$3);
				}
			 	|
				ID SQ_BRACKET_L exprsequence SQ_BRACKET_R  LET expr SEMICOLON
				{
					DEBUG("IN ASSIGN: arrays");
					node* var=TBmakeVar(STRcpy($1),$3);
				 	$$ = TBmakeAssign(var,$6);
				}
				;

block			:	statement
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
		
statementlist	 	:	statement 
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

		
statement 		: 	assign
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

ifblock 		:	IF BRACKET_L expr BRACKET_R block elseblock
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
elseblock 		: 	ELSE block
				{
					DEBUG("IN ELSE BLOCK");
					$$=TBmakeElseblock($2);
				}
				;

whileblock 		:	WHILE BRACKET_L expr BRACKET_R block 
				{
					DEBUG("IN WHILE BLOCK");
					$$=TBmakeWhilestat($3, $5);
				}
				;

dowhileblock		:	DO block WHILE BRACKET_L expr BRACKET_R SEMICOLON
				{
					DEBUG("IN DO WHILE BLOCK");
					$$=TBmakeDowhilestat($2, $5);
				}
				;	

forblock		:	FOR BRACKET_L forinitstat COMMA expr COMMA expr BRACKET_R block
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

forinitstat		:	TYPE_INT ID LET expr
				{
					DEBUG("IN FOR VARDEC PART");
					node* var=TBmakeVar(STRcpy($2),NULL);
					$$=TBmakeVardec(T_int, NULL, var, $4);
				}
				;

funcall 		:	ID BRACKET_L exprsequence BRACKET_R SEMICOLON//function call
	          		{
	                        	DEBUG("FUNCTION CALL");
	                       		node *var=TBmakeVar(STRcpy($1),NULL);
	                      		$$=TBmakeFuncall(var,$3);
	                	}       
	               		|ID BRACKET_L BRACKET_R SEMICOLON
	                	{
	                        	DEBUG("FUNCTION CALL WITHOUT PARAM");
	                        	node *var1=TBmakeVar(STRcpy($1),NULL);
	                       		$$=TBmakeFuncall(var1,NULL);
	               		}
				;

expr 			:	expr binop unaryandcast  
				{
					DEBUG("OTHER BINOP");
					$$ = TBmakeBinop($2, $1, $3);
				}
				|
				expr MINUS unaryandcast
				{
					DEBUG(" BINOP TESTMINUS");
					binop symbol=BO_sub;
					$$=TBmakeBinop(symbol,$1,$3);
					
				}
				| 
				unaryandcast
				{
					DEBUG(" UNARY AND CAST");
					$$ = $1;
				}
				|
				arrayinit
				{ // this is the special array initialization square brackets.
				  // As per our thoughts, this can occur only in an assignment statement. So placing it here.
					DEBUG("ARRAY INIT SQ BRACKETS");
					$$=$1;
				}
				;
	 
unaryandcast 		: 	monOp exprfunctioncall 
				{
					DEBUG(" MONOP TESTUNARY");
					$$ = TBmakeMonop($1,$2);
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
				| arrayindexing
				{
					DEBUG("ARRAY INDEXING LEVEL");
					$$=$1;
				}
				;

exprfunctioncall	:	ID BRACKET_L exprsequence BRACKET_R //function call
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

arrayindexing		:	ID SQ_BRACKET_L exprsequence SQ_BRACKET_R
				{
					DEBUG("ARRAY INDEXING");
					$$=TBmakeVar($1, $3);
				}
				;

arrayinit		: 	SQ_BRACKET_L exprsequence SQ_BRACKET_R
				{
					DEBUG("IN ARRAY INIT SQ BRACKETS");
					$$=TBmakeArrayinit($2);
				}
				;

exprsequence 		:	expr COMMA exprsequence
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

groupedexpression	:	BRACKET_L expr BRACKET_R
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

constant 		:	floatval
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

floatval 		:	FLOAT
		            	{
		            		$$ = TBmakeFloat( $1);
		            	}
         			;

intval 			:	NUM
			        {
			          $$ = TBmakeNum( $1);
			        }
      				;

boolval 		:	TRUEVAL
			        {
			           $$ = TBmakeBool( TRUE);
			        }
       				| FALSEVAL
         			{
           				$$ = TBmakeBool( FALSE);
         			}
       				;

binop 			:	PLUS		{ $$ = BO_add; }
				| STAR      	{ $$ = BO_mul; }
				| SLASH         { $$ = BO_div; }
				| PERCENT       { $$ = BO_mod; }
				| NE            { $$ = BO_ne; }
				| LE            { $$ = BO_le; }
				| LT            { $$ = BO_lt; }
				| GE            { $$ = BO_ge; }
				| GT            { $$ = BO_gt; }
				| EQ            { $$ = BO_eq; }
				| OR            { $$ = BO_or; }
				| AND           { $$ = BO_and; }
				;

monOp 			: 	NOT 		{$$	= MO_not;}
				;

type 			: 	TYPE_INT	{$$ = T_int;} 
	 			|TYPE_BOOL 	{$$ = T_bool;}
	 			|TYPE_FLOAT 	{$$ = T_float;}
	 			;
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


