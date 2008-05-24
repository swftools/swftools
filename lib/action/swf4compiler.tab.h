/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

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
#define BREAK 258
#define FOR 259
#define CONTINUE 260
#define IF 261
#define ELSE 262
#define DO 263
#define WHILE 264
#define THIS 265
#define EVAL 266
#define TIME 267
#define RANDOM 268
#define LENGTH 269
#define INT 270
#define CONCAT 271
#define DUPLICATECLIP 272
#define REMOVECLIP 273
#define TRACE 274
#define STARTDRAG 275
#define STOPDRAG 276
#define ORD 277
#define CHR 278
#define CALLFRAME 279
#define GETURL 280
#define GETURL1 281
#define LOADMOVIE 282
#define LOADVARIABLES 283
#define POSTURL 284
#define SUBSTR 285
#define GETPROPERTY 286
#define NEXTFRAME 287
#define PREVFRAME 288
#define PLAY 289
#define STOP 290
#define TOGGLEQUALITY 291
#define STOPSOUNDS 292
#define GOTOFRAME 293
#define GOTOANDPLAY 294
#define FRAMELOADED 295
#define SETTARGET 296
#define TELLTARGET 297
#define STRING 298
#define NUMBER 299
#define IDENTIFIER 300
#define PATH 301
#define GETURL_METHOD 302
#define EQ 303
#define LE 304
#define GE 305
#define NE 306
#define LAN 307
#define LOR 308
#define INC 309
#define DEC 310
#define IEQ 311
#define DEQ 312
#define MEQ 313
#define SEQ 314
#define STREQ 315
#define STRNE 316
#define STRCMP 317
#define PARENT 318
#define END 319
#define UMINUS 320
#define POSTFIX 321
#define NEGATE 322




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 17 "swf4compiler.y"
typedef union YYSTYPE {
  Buffer action;
  char *str;
  SWFActionFunction function;
  SWFGetUrl2Method getURLMethod;
} YYSTYPE;
/* Line 1240 of yacc.c.  */
#line 177 "swf4compiler.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE swf4lval;



