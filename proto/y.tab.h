/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_MAIN = 258,
    T_PRINTF = 259,
    T_CONST = 260,
    T_VAR = 261,
    T_ADD = 262,
    T_SUB = 263,
    T_MUL = 264,
    T_DIV = 265,
    T_EQ = 266,
    T_POPEN = 267,
    T_PCLOSE = 268,
    T_AOPEN = 269,
    T_ACLOSE = 270,
    T_COPEN = 271,
    T_CCLOSE = 272,
    T_INT = 273,
    T_NAME = 274,
    T_SEP = 275,
    T_COMMA = 276
  };
#endif
/* Tokens.  */
#define T_MAIN 258
#define T_PRINTF 259
#define T_CONST 260
#define T_VAR 261
#define T_ADD 262
#define T_SUB 263
#define T_MUL 264
#define T_DIV 265
#define T_EQ 266
#define T_POPEN 267
#define T_PCLOSE 268
#define T_AOPEN 269
#define T_ACLOSE 270
#define T_COPEN 271
#define T_CCLOSE 272
#define T_INT 273
#define T_NAME 274
#define T_SEP 275
#define T_COMMA 276

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 27 "tokens.y" /* yacc.c:1909  */
int i; char * s

#line 99 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
