/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     BRACKET_L = 258,
     BRACKET_R = 259,
     COMMA = 260,
     SEMICOLON = 261,
     MINUS = 262,
     PLUS = 263,
     STAR = 264,
     SLASH = 265,
     PERCENT = 266,
     LE = 267,
     LT = 268,
     GE = 269,
     GT = 270,
     EQ = 271,
     NE = 272,
     OR = 273,
     AND = 274,
     TRUEVAL = 275,
     FALSEVAL = 276,
     LET = 277,
     NUM = 278,
     FLOAT = 279,
     ID = 280,
     EXTERN = 281,
     EXPORT = 282,
     TYPE_VOID = 283,
     TYPE_INT = 284,
     TYPE_BOOL = 285,
     TYPE_FLOAT = 286,
     RETURN = 287,
     FL_BRACKET_L = 288,
     FL_BRACKET_R = 289,
     SQ_BRACKET_L = 290,
     SQ_BRACKET_R = 291,
     IF = 292,
     DO = 293,
     WHILE = 294,
     FOR = 295
   };
#endif
/* Tokens.  */
#define BRACKET_L 258
#define BRACKET_R 259
#define COMMA 260
#define SEMICOLON 261
#define MINUS 262
#define PLUS 263
#define STAR 264
#define SLASH 65
#define PERCENT 266
#define LE 267
#define LT 268
#define GE 269
#define GT 270
#define EQ 271
#define NE 272
#define OR 273
#define AND 274
#define TRUEVAL 275
#define FALSEVAL 276
#define LET 277
#define NUM 278
#define FLOAT 279
#define ID 280
#define EXTERN 281
#define EXPORT 282
#define TYPE_VOID 283
#define TYPE_INT 284
#define TYPE_BOOL 285
#define TYPE_FLOAT 286
#define RETURN 287
#define FL_BRACKET_L 288
#define FL_BRACKET_R 289
#define SQ_BRACKET_L 290
#define SQ_BRACKET_R 291
#define IF 292
#define DO 293
#define WHILE 294
#define FOR 295





#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 23 "src/scanparse/civic.y"

 nodetype            nodetype;
 char               *id;
 int                 cint;
 float               cflt;
 binop               cbinop;
 type		     datatype;
 node               *node;



/* Line 2068 of yacc.c  */
#line 111 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


