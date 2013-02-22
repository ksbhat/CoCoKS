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

#define DEBUG(msg) \
	printf("%s:l:%d,c:%d\t%s\n", "DEBUG",global.line, global.col,msg);

#define FILTER(token) \
  global.col += yyleng;  \
  return( token);

#define INC_LINECNT() \
  global.line += 1;   \
  global.col = 0;     \
  yyless(1);     

#define LINEBUF_SIZE 256
#define YY_NO_UNPUT

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
*/

%}

%p 6000 /* set positions to 6000 */
%e 2000 /* set nodes to 1100 */
%option noyywrap
%x COMMENT

%%

"//.*$"			   { 
				DEBUG("SINGLE COMMENT START");
				DEBUG(yytext);
 				/*single line comment ; no need to send it to the parser */ ; } 

"/*"			   { 
				DEBUG("COMMENT START");
				DEBUG(yytext);
				BEGIN(COMMENT);   
			   }

<COMMENT>\n		   { 
				DEBUG("COMMENT Line");
				DEBUG(yytext);
				
				INC_LINECNT();    }

<COMMENT>"*/"		   {  
				DEBUG("COMMENT end");
				DEBUG(yytext);
				
				BEGIN(INITIAL); 
			    }

<COMMENT>.*		   {
				DEBUG("COMMENT line");
				DEBUG(yytext);
				
				 global.col += yyleng;
			}
"("                     { 
				DEBUG("Simple Bracket START");
				DEBUG(yytext);
				
				FILTER( BRACKET_L); 
			}
")"                     { 	
				DEBUG("Simple bracket end");
				DEBUG(yytext);
				
				FILTER( BRACKET_R); 
			}
","                     { 
				DEBUG("COmma START");
				DEBUG(yytext);
				
				FILTER( COMMA); 
			}

";"                     { 	
				DEBUG("semicolon START");
				DEBUG(yytext);
				
				FILTER( SEMICOLON); 
			}

"-"                     { 	
				DEBUG("hyphen START");
				DEBUG(yytext);
				
				FILTER( MINUS); 
			}

"+"                     { 
				DEBUG("plus START");
				DEBUG(yytext);
				
				FILTER( PLUS);  
			}

"*"                     { 	
				DEBUG("multiply START");
				DEBUG(yytext);
				
				FILTER( STAR);
			} 

"/"                     { 
				DEBUG("divide START");
				DEBUG(yytext);
				
				FILTER( SLASH);  
			} 

"%"                     {	
				DEBUG("mod START");
				DEBUG(yytext);
				
				 FILTER( PERCENT); 
			} 

"<="                    { 	
				DEBUG("lE START");
				DEBUG(yytext);
				
				FILTER( LE);
			}

"<"                     { 
				DEBUG("less START");
				DEBUG(yytext);
				
				FILTER( LT);    
			} 

">="                    { 	
				DEBUG("Ge START");
				DEBUG(yytext);
				
				FILTER( GE);    
			}

">"                     { 
				DEBUG("greater START");
				DEBUG(yytext);
				
				FILTER( GT);    
			}

"=="                    { 	
				DEBUG("equals START");
				DEBUG(yytext);
				
				FILTER( EQ);    
			}

"!="                    { 
				DEBUG("not equals START");
				DEBUG(yytext);
				
				FILTER( NE);
		        }

"&&"                    { 
				DEBUG("and START");
				DEBUG(yytext);
				
				FILTER( AND);    
			}

"||"                    { 
				DEBUG("or START");
				DEBUG(yytext);
				
				FILTER( OR);    

			}

"="                     { 
				DEBUG("assign START");
				DEBUG(yytext);
				
				FILTER( LET);    
			}

"true"                  { 
				DEBUG("true START");
				DEBUG(yytext);
				
				FILTER( TRUEVAL); 
			}

"false"                 { 
				DEBUG("false START");
				DEBUG(yytext);
				
				FILTER( FALSEVAL); 
			}

"extern"		{ 
				DEBUG("extern START");
				DEBUG(yytext);
				
				FILTER( EXTERN ); 
			}

"export"		{ 
				DEBUG("export START");
				DEBUG(yytext);
				
				FILTER( EXPORT ); 
			}

"void"	  	  	{ 
				DEBUG("void START");
				DEBUG(yytext);
				
				FILTER( TYPE_VOID ); 
			}

"int"			{ 
				DEBUG("int START");
				DEBUG(yytext);
				
				FILTER( TYPE_INT ); 
			}

"bool"			{ 
				DEBUG("bool START");
				DEBUG(yytext);
				
				FILTER( TYPE_BOOL ); 
			}

"float"			{
				DEBUG("float START");
				DEBUG(yytext);
				
				FILTER( TYPE_FLOAT ); 
			}

"return"		{ 
				DEBUG("return START");
				DEBUG(yytext);
				
				FILTER( RETURN ); 
			}

"{"			{ 
				DEBUG("{ START");
				DEBUG(yytext);
				
				FILTER( FL_BRACKET_L  ); 
			}

"}"			{ 
				DEBUG("} START");
				DEBUG(yytext);
				
				FILTER( FL_BRACKET_R  ); 
			}

"["			{ 
				DEBUG("[ START");
				DEBUG(yytext);
				
				FILTER( SQ_BRACKET_L ); 
			}

"]"			{ 
				DEBUG("] START");
				DEBUG(yytext);
				
				FILTER( SQ_BRACKET_R ); 
			}

"if"			{
				DEBUG("if START");
				DEBUG(yytext);
				
				 FILTER( IF ); 
			}

"do"			{
				DEBUG("do START");
				DEBUG(yytext);
				
				 FILTER( DO );
			}

"while"			{
				DEBUG("while START");
				DEBUG(yytext);
				
				 FILTER( WHILE ); 
			}

"for"			{ 
				DEBUG("for START");
				DEBUG(yytext);
				
				FILTER( FOR ); 
			}

[A-Za-z][A-Za-z0-9_]*   { 
				DEBUG("line code START");
				DEBUG(yytext);
				
				yylval.id = STRcpy(yytext);
                             	FILTER( ID);
                        }

[0-9]+                  { 
				DEBUG("list number START");
				DEBUG(yytext);
				
				yylval.cint=atoi(yytext);
                             	FILTER( NUM);
                        }

[0-9]+(\.[0-9]+)*       { 
				DEBUG("list float START");
				DEBUG(yytext);
				
				yylval.cint=atoi(yytext);
                             	FILTER( NUM);
			}

\n.*                    { 
				DEBUG("new linecount START");
				DEBUG(yytext);
				
				global.line += 1;
                             	global.col = 0;
                             	yyless(1);
                        }

[ \t]+                   { 
				DEBUG("tab START");
				DEBUG(yytext);
				
				global.col += yyleng;
                        } 
