%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "types.h"
#include "node_basic.h"
#include "str.h"
#include "free.h"
#include "civic.tab.h"
#include "globals.h"
#include "ctinfo.h"

#ifdef DEBUG
	#define DEBUG(msg) \
		printf("%s:l:%d,c:%d\t%s\n", "DEBUG",global.line, global.col,msg);
#else
	#define DEBUG(msg) ;
#endif

#define FILTER(token) \
  global.col += yyleng;  \
  return( token);

#define INC_LINECNT() \
  global.line += 1;   \
  global.col = 0;     \
  yyless(1);     

#define LINEBUF_SIZE 256
#define YY_NO_UNPUT
/*
struct Global
{
  int line;
  int col;
} global;

*/
/*************************************
TOKENS declared in the parser
*************************************

EXTERN
EXPORT
VOID
INT
BOOL
FLOAT
RETURN
FL_BRACKET_L
FL_BRACKET_R
SQ_BRACKET_L
SQ_BRACKET_R
IF
DO
WHILE
FOR
NOT
*/

%}

%p 6000 /* set positions to 6000 */
%e 2000 /* set nodes to 1100 */
%option noyywrap
%x COMMENT

%%

"//"(.*)$		   { 
				DEBUG("SINGLE COMMENT ");
				DEBUG(yytext);
 				/*single line comment ; no need to send it to the parser */ ; } 

"/*"			   { 
				DEBUG("COMMENT START");
				DEBUG(yytext);
				BEGIN(COMMENT);   
			   }

<COMMENT>\n		   { 
				DEBUG("COMMENT NewLine");
				DEBUG(yytext);
				
				INC_LINECNT();    }

<COMMENT>.*"*/"		   {  
				DEBUG("COMMENT END");
				DEBUG(yytext);
				
				BEGIN(INITIAL); 
			    }

<COMMENT>.*		   {
				DEBUG("COMMENT line");
				DEBUG(yytext);
				
				 global.col += yyleng;
			}
"("                     { 
				DEBUG("Simple Bracket");
				DEBUG(yytext);
				
				FILTER( BRACKET_L); 
			}
")"                     { 	
				DEBUG("Simple bracket end");
				DEBUG(yytext);
				
				FILTER( BRACKET_R); 
			}
","                     { 
				DEBUG("Comma ");
				DEBUG(yytext);
				
				FILTER( COMMA); 
			}

";"                     { 	
				DEBUG("semicolon ");
				DEBUG(yytext);
				
				FILTER( SEMICOLON); 
			}

"-"                     { 	
				DEBUG("hyphen ");
				DEBUG(yytext);
				
				FILTER( MINUS); 
			}

"+"                     { 
				DEBUG("plus ");
				DEBUG(yytext);
				
				FILTER( PLUS);  
			}

"*"                     { 	
				DEBUG("multiply ");
				DEBUG(yytext);
				
				FILTER( STAR);
			} 

"/"                     { 
				DEBUG("divide ");
				DEBUG(yytext);
				
				FILTER( SLASH);  
			} 

"!"                     { 
				DEBUG("unary NOT ");
				DEBUG(yytext);
				
				FILTER( NOT);  
			} 

"%"                     {	
				DEBUG("mod ");
				DEBUG(yytext);
				
				 FILTER( PERCENT); 
			} 

"<="                    { 	
				DEBUG("LE ");
				DEBUG(yytext);
				
				FILTER( LE);
			}

"<"                     { 
				DEBUG("less ");
				DEBUG(yytext);
				
				FILTER( LT);    
			} 

">="                    { 	
				DEBUG("GE ");
				DEBUG(yytext);
				
				FILTER( GE);    
			}

">"                     { 
				DEBUG("greater ");
				DEBUG(yytext);
				
				FILTER( GT);    
			}

"=="                    { 	
				DEBUG("equals ");
				DEBUG(yytext);
				
				FILTER( EQ);    
			}

"!="                    { 
				DEBUG("not equals ");
				DEBUG(yytext);
				
				FILTER( NE);
		        }

"&&"                    { 
				DEBUG("and ");
				DEBUG(yytext);
				
				FILTER( AND);    
			}

"||"                    { 
				DEBUG("or ");
				DEBUG(yytext);
				
				FILTER( OR);    

			}

"="                     { 
				DEBUG("assign ");
				DEBUG(yytext);
				
				FILTER( LET);    
			}

"true"                  { 
				DEBUG("true ");
				DEBUG(yytext);
				
				FILTER( TRUEVAL); 
			}

"false"                 { 
				DEBUG("false ");
				DEBUG(yytext);
				
				FILTER( FALSEVAL); 
			}

"extern"		{ 
				DEBUG("extern ");
				DEBUG(yytext);
				
				FILTER( EXTERN ); 
			}

"export"		{ 
				DEBUG("export ");
				DEBUG(yytext);
				
				FILTER( EXPORT ); 
			}

"void"	  	  	{ 
				DEBUG("void ");
				DEBUG(yytext);
				
				FILTER( TYPE_VOID ); 
			}

"int"			{ 
				DEBUG("int ");
				DEBUG(yytext);
				
				FILTER( TYPE_INT ); 
			}

"bool"			{ 
				DEBUG("bool ");
				DEBUG(yytext);
				
				FILTER( TYPE_BOOL ); 
			}

"float"			{
				DEBUG("float ");
				DEBUG(yytext);
				
				FILTER( TYPE_FLOAT ); 
			}

"return"		{ 
				DEBUG("return ");
				DEBUG(yytext);
				
				FILTER( RETURN ); 
			}

"{"			{ 
				DEBUG("{ ");
				DEBUG(yytext);
				
				FILTER( FL_BRACKET_L  ); 
			}

"}"			{ 
				DEBUG("} ");
				DEBUG(yytext);
				
				FILTER( FL_BRACKET_R  ); 
			}

"["			{ 
				DEBUG("[ ");
				DEBUG(yytext);
				
				FILTER( SQ_BRACKET_L ); 
			}

"]"			{ 
				DEBUG("] ");
				DEBUG(yytext);
				
				FILTER( SQ_BRACKET_R ); 
			}

"if"			{
				DEBUG("if ");
				DEBUG(yytext);
				
				 FILTER( IF ); 
			}
"else"			{
				DEBUG("if ");
				DEBUG(yytext);
				
				 FILTER( IF ); 
			}


"do"			{
				DEBUG("do ");
				DEBUG(yytext);
				
				 FILTER( DO );
			}

"while"			{
				DEBUG("while ");
				DEBUG(yytext);
				
				 FILTER( WHILE ); 
			}

"for"			{ 
				DEBUG("for ");
				DEBUG(yytext);
				
				FILTER( FOR ); 
			}

[A-Za-z][A-Za-z0-9_]*   { 
				DEBUG("line code ");
				DEBUG(yytext);
				
				//yylval.id = STRcpy(yytext);
                             	FILTER( ID);
                        }

[0-9]+                  { 
				DEBUG("list number ");
				DEBUG(yytext);
				
				yylval.cint=atoi(yytext);
                             	FILTER( NUM);
                        }

[0-9]+(\.[0-9]+)*       { 
				DEBUG("list float ");
				DEBUG(yytext);
				
				yylval.cint=atoi(yytext);
                             	FILTER( NUM);
			}

\n.*                    { 
				DEBUG("new linecount ");
				DEBUG(yytext);
				
				global.line += 1;
                             	global.col = 0;
                             	yyless(1);
                        }

[ \t]+                   { 
				DEBUG("tab ");
				DEBUG(yytext);
				
				global.col += yyleng;
                        }
%%

