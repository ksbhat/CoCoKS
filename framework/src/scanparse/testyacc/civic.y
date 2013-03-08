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
 node               *node;
}

%token BRACKET_L BRACKET_R COMMA SEMICOLON
%token MINUS PLUS STAR SLASH PERCENT LE LT GE GT EQ NE OR AND
%token TRUEVAL FALSEVAL LET

%token <cint> NUM
%token <cflt> FLOAT
%token <id> ID
%token EXTERN EXPORT TYPE_VOID TYPE_INT TYPE_BOOL TYPE_FLOAT RETURN 
%token FL_BRACKET_L FL_BRAKCET_R SQ_BRACKET_L SQ_BRACKET_R
%token IF FOR DO WHILE
%token NOT
%type <node> intval floatval boolval constant expr
%type <node> assign varlet 
%type <node> MonOp type
%type <cbinop> binop

%start expr

%%


assign: varlet LET expr SEMICOLON
        {
          $$ = TBmakeAssign( $1, $3);
        }
        ;

varlet: ID
        {
          $$ = TBmakeVarlet( STRcpy( $1));
        }
        ;


expr: testUnary

testUnary : MonOp castExpression
		| BRACKET_L type BRACKET_R castExpression
		| castExpression
		;
       
castExpression : ID
		|  constant
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
     | MINUS     { $$ = BO_sub; }
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
     
MonOp: MINUS	{$$ = MO_neg;}
	  |	NOT		{$$	= MO_not;}

type:  TYPE_VOID		{$$ = T_void;}
	 |TYPE_INT		{$$ = T_int;} 
	 |TYPE_BOOL 		{$$ = T_bool;}
	 |TYPE_FLOAT	 	{$$ = T_float;}

	 
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

int main(int argc, char **argv)
{
        FILE *pFile;

        if ( NULL == (pFile = fopen("./input.txt", "r" )) )
        {
                printf("Error opening the file\n");
        }
        yyin = pFile;

        //while ( yylex() );
	yyparse();
        printf("Done\n");
        return 0;
}

