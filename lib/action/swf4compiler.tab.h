
/* A Bison parser, made by GNU Bison 2.4.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
     BREAK = 258,
     FOR = 259,
     CONTINUE = 260,
     IF = 261,
     ELSE = 262,
     DO = 263,
     WHILE = 264,
     THIS = 265,
     EVAL = 266,
     TIME = 267,
     RANDOM = 268,
     LENGTH = 269,
     INT = 270,
     CONCAT = 271,
     DUPLICATECLIP = 272,
     REMOVECLIP = 273,
     TRACE = 274,
     STARTDRAG = 275,
     STOPDRAG = 276,
     ORD = 277,
     CHR = 278,
     CALLFRAME = 279,
     GETURL = 280,
     GETURL1 = 281,
     LOADMOVIE = 282,
     LOADVARIABLES = 283,
     POSTURL = 284,
     SUBSTR = 285,
     GETPROPERTY = 286,
     NEXTFRAME = 287,
     PREVFRAME = 288,
     PLAY = 289,
     STOP = 290,
     TOGGLEQUALITY = 291,
     STOPSOUNDS = 292,
     GOTOFRAME = 293,
     GOTOANDPLAY = 294,
     FRAMELOADED = 295,
     SETTARGET = 296,
     TELLTARGET = 297,
     STRING = 298,
     NUMBER = 299,
     IDENTIFIER = 300,
     PATH = 301,
     GETURL_METHOD = 302,
     EQ = 303,
     LE = 304,
     GE = 305,
     NE = 306,
     LAN = 307,
     LOR = 308,
     INC = 309,
     DEC = 310,
     IEQ = 311,
     DEQ = 312,
     MEQ = 313,
     SEQ = 314,
     STREQ = 315,
     STRNE = 316,
     STRCMP = 317,
     PARENT = 318,
     END = 319,
     UMINUS = 320,
     POSTFIX = 321,
     NEGATE = 322
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 17 "swf4compiler.y"

  Buffer action;
  char *str;
  SWFActionFunction function;
  SWFGetUrl2Method getURLMethod;



/* Line 1676 of yacc.c  */
#line 128 "swf4compiler.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE swf4lval;


