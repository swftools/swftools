/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     CONTINUE = 259,
     FUNCTION = 260,
     ELSE = 261,
     SWITCH = 262,
     CASE = 263,
     DEFAULT = 264,
     FOR = 265,
     IN = 266,
     IF = 267,
     WHILE = 268,
     DO = 269,
     VAR = 270,
     NEW = 271,
     DELETE = 272,
     RETURN = 273,
     END = 274,
     WITH = 275,
     ASM = 276,
     EVAL = 277,
     RANDOM = 278,
     GETTIMER = 279,
     LENGTH = 280,
     CONCAT = 281,
     SUBSTR = 282,
     TRACE = 283,
     INT = 284,
     ORD = 285,
     CHR = 286,
     GETURL = 287,
     GETURL1 = 288,
     NEXTFRAME = 289,
     PREVFRAME = 290,
     PLAY = 291,
     STOP = 292,
     TOGGLEQUALITY = 293,
     STOPSOUNDS = 294,
     DUP = 295,
     SWAP = 296,
     POP = 297,
     PUSH = 298,
     SETREGISTER = 299,
     CALLFUNCTION = 300,
     CALLMETHOD = 301,
     AND = 302,
     OR = 303,
     XOR = 304,
     MODULO = 305,
     ADD = 306,
     LESSTHAN = 307,
     EQUALS = 308,
     INC = 309,
     DEC = 310,
     TYPEOF = 311,
     INSTANCEOF = 312,
     ENUMERATE = 313,
     INITOBJECT = 314,
     INITARRAY = 315,
     GETMEMBER = 316,
     SETMEMBER = 317,
     SHIFTLEFT = 318,
     SHIFTRIGHT = 319,
     SHIFTRIGHT2 = 320,
     VAREQUALS = 321,
     OLDADD = 322,
     SUBTRACT = 323,
     MULTIPLY = 324,
     DIVIDE = 325,
     OLDEQUALS = 326,
     OLDLESSTHAN = 327,
     LOGICALAND = 328,
     LOGICALOR = 329,
     NOT = 330,
     STRINGEQ = 331,
     STRINGLENGTH = 332,
     SUBSTRING = 333,
     GETVARIABLE = 334,
     SETVARIABLE = 335,
     SETTARGETEXPRESSION = 336,
     DUPLICATEMOVIECLIP = 337,
     REMOVEMOVIECLIP = 338,
     STRINGLESSTHAN = 339,
     MBLENGTH = 340,
     MBSUBSTRING = 341,
     MBORD = 342,
     MBCHR = 343,
     BRANCHALWAYS = 344,
     BRANCHIFTRUE = 345,
     GETURL2 = 346,
     POST = 347,
     GET = 348,
     LOADVARIABLES = 349,
     LOADMOVIE = 350,
     LOADVARIABLESNUM = 351,
     LOADMOVIENUM = 352,
     CALLFRAME = 353,
     STARTDRAG = 354,
     STOPDRAG = 355,
     GOTOFRAME = 356,
     SETTARGET = 357,
     NULLVAL = 358,
     INTEGER = 359,
     DOUBLE = 360,
     BOOLEAN = 361,
     REGISTER = 362,
     STRING = 363,
     IDENTIFIER = 364,
     EQ = 365,
     LE = 366,
     GE = 367,
     NE = 368,
     LAN = 369,
     LOR = 370,
     INCR = 371,
     DECR = 372,
     IEQ = 373,
     DEQ = 374,
     MEQ = 375,
     SEQ = 376,
     REQ = 377,
     AEQ = 378,
     OEQ = 379,
     SHL = 380,
     SHR = 381,
     SHR2 = 382,
     SHLEQ = 383,
     SHREQ = 384,
     SHR2EQ = 385,
     NOELSE = 386,
     UMINUS = 388,
     POSTFIX = 389
   };
#endif
/* Tokens.  */
#define BREAK 258
#define CONTINUE 259
#define FUNCTION 260
#define ELSE 261
#define SWITCH 262
#define CASE 263
#define DEFAULT 264
#define FOR 265
#define IN 266
#define IF 267
#define WHILE 268
#define DO 269
#define VAR 270
#define NEW 271
#define DELETE 272
#define RETURN 273
#define END 274
#define WITH 275
#define ASM 276
#define EVAL 277
#define RANDOM 278
#define GETTIMER 279
#define LENGTH 280
#define CONCAT 281
#define SUBSTR 282
#define TRACE 283
#define INT 284
#define ORD 285
#define CHR 286
#define GETURL 287
#define GETURL1 288
#define NEXTFRAME 289
#define PREVFRAME 290
#define PLAY 291
#define STOP 292
#define TOGGLEQUALITY 293
#define STOPSOUNDS 294
#define DUP 295
#define SWAP 296
#define POP 297
#define PUSH 298
#define SETREGISTER 299
#define CALLFUNCTION 300
#define CALLMETHOD 301
#define AND 302
#define OR 303
#define XOR 304
#define MODULO 305
#define ADD 306
#define LESSTHAN 307
#define EQUALS 308
#define INC 309
#define DEC 310
#define TYPEOF 311
#define INSTANCEOF 312
#define ENUMERATE 313
#define INITOBJECT 314
#define INITARRAY 315
#define GETMEMBER 316
#define SETMEMBER 317
#define SHIFTLEFT 318
#define SHIFTRIGHT 319
#define SHIFTRIGHT2 320
#define VAREQUALS 321
#define OLDADD 322
#define SUBTRACT 323
#define MULTIPLY 324
#define DIVIDE 325
#define OLDEQUALS 326
#define OLDLESSTHAN 327
#define LOGICALAND 328
#define LOGICALOR 329
#define NOT 330
#define STRINGEQ 331
#define STRINGLENGTH 332
#define SUBSTRING 333
#define GETVARIABLE 334
#define SETVARIABLE 335
#define SETTARGETEXPRESSION 336
#define DUPLICATEMOVIECLIP 337
#define REMOVEMOVIECLIP 338
#define STRINGLESSTHAN 339
#define MBLENGTH 340
#define MBSUBSTRING 341
#define MBORD 342
#define MBCHR 343
#define BRANCHALWAYS 344
#define BRANCHIFTRUE 345
#define GETURL2 346
#define POST 347
#define GET 348
#define LOADVARIABLES 349
#define LOADMOVIE 350
#define LOADVARIABLESNUM 351
#define LOADMOVIENUM 352
#define CALLFRAME 353
#define STARTDRAG 354
#define STOPDRAG 355
#define GOTOFRAME 356
#define SETTARGET 357
#define NULLVAL 358
#define INTEGER 359
#define DOUBLE 360
#define BOOLEAN 361
#define REGISTER 362
#define STRING 363
#define IDENTIFIER 364
#define EQ 365
#define LE 366
#define GE 367
#define NE 368
#define LAN 369
#define LOR 370
#define INCR 371
#define DECR 372
#define IEQ 373
#define DEQ 374
#define MEQ 375
#define SEQ 376
#define REQ 377
#define AEQ 378
#define OEQ 379
#define SHL 380
#define SHR 381
#define SHR2 382
#define SHLEQ 383
#define SHREQ 384
#define SHR2EQ 385
#define NOELSE 386
#define UMINUS 388
#define POSTFIX 389




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 21 "swf5compiler.y"
{
  Buffer action;
  char *str;
  SWFGetUrl2Method getURLMethod;
  int op;
  int intVal;
  int len;
  double doubleVal;

  struct
  {
    Buffer buffer;
    int count;
  } exprlist;
  struct switchcase   switchcase;
  struct switchcases switchcases;
  struct
  {
	Buffer obj, ident, memexpr;
  } lval;
}
/* Line 1489 of yacc.c.  */
#line 337 "swf5compiler.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE swf5lval;

