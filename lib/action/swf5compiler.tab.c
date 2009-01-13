/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse swf5parse
#define yylex   swf5lex
#define yyerror swf5error
#define yylval  swf5lval
#define yychar  swf5char
#define yydebug swf5debug
#define yynerrs swf5nerrs


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




/* Copy the first part of user declarations.  */
#line 5 "swf5compiler.y"


#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "compile.h"
#include "action.h"
#include "assembler.h"

#define YYPARSE_PARAM buffer

Buffer bf, bc;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 187 of yacc.c.  */
#line 407 "swf5compiler.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 420 "swf5compiler.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   5253

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  159
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  348
/* YYNRULES -- Number of states.  */
#define YYNSTATES  616

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   389

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   147,     2,     2,     2,   146,   139,     2,
     157,   158,   144,   142,   132,   143,   151,   145,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   136,   156,
     137,   133,   138,   135,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   152,     2,   153,   141,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   154,   140,   155,   148,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   134,   149,   150
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     8,    10,    13,    15,    17,
      19,    22,    25,    27,    31,    33,    36,    38,    40,    42,
      44,    46,    48,    50,    58,    61,    65,    67,    71,    79,
      85,    86,    88,    90,    98,    99,   102,   109,   114,   118,
     120,   122,   124,   126,   128,   130,   132,   134,   136,   138,
     140,   142,   144,   146,   148,   150,   152,   154,   156,   158,
     160,   162,   164,   166,   168,   170,   172,   174,   176,   178,
     180,   182,   184,   186,   188,   190,   192,   194,   196,   198,
     200,   202,   204,   206,   208,   210,   212,   214,   216,   218,
     220,   222,   224,   226,   228,   230,   232,   234,   236,   238,
     240,   242,   244,   246,   248,   250,   252,   254,   256,   258,
     260,   262,   264,   266,   268,   270,   272,   274,   275,   277,
     281,   283,   290,   292,   296,   301,   303,   305,   307,   309,
     310,   311,   317,   324,   335,   344,   354,   355,   357,   360,
     363,   364,   367,   370,   373,   375,   377,   382,   385,   390,
     396,   401,   409,   417,   425,   433,   441,   446,   453,   468,
     472,   481,   486,   493,   497,   501,   505,   509,   513,   517,
     522,   527,   532,   537,   542,   547,   552,   556,   561,   566,
     571,   576,   581,   588,   597,   602,   603,   605,   609,   615,
     622,   630,   634,   636,   640,   642,   644,   646,   648,   650,
     652,   654,   656,   658,   660,   662,   664,   666,   668,   670,
     672,   674,   678,   683,   685,   688,   691,   694,   698,   702,
     706,   710,   714,   718,   722,   726,   730,   734,   738,   742,
     746,   750,   754,   758,   762,   766,   770,   776,   780,   784,
     786,   789,   795,   799,   801,   805,   807,   809,   811,   813,
     816,   819,   823,   826,   828,   831,   833,   835,   837,   839,
     841,   845,   849,   851,   852,   858,   861,   863,   865,   867,
     870,   873,   877,   881,   883,   886,   887,   892,   894,   896,
     898,   900,   902,   904,   906,   910,   911,   915,   917,   920,
     922,   924,   926,   928,   930,   932,   934,   936,   938,   940,
     942,   944,   946,   948,   950,   952,   954,   956,   958,   960,
     962,   964,   966,   968,   970,   972,   974,   976,   978,   980,
     982,   984,   986,   988,   990,   992,   994,   996,   998,  1000,
    1002,  1004,  1006,  1008,  1010,  1012,  1014,  1016,  1018,  1020,
    1022,  1024,  1026,  1028,  1030,  1032,  1034,  1036,  1039
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     160,     0,    -1,    -1,   161,   162,    -1,    -1,   163,    -1,
     162,   163,    -1,   166,    -1,   179,    -1,   166,    -1,   164,
     166,    -1,   154,   155,    -1,   165,    -1,   154,   164,   155,
      -1,   156,    -1,   207,   156,    -1,   170,    -1,   185,    -1,
     187,    -1,   188,    -1,   173,    -1,   168,    -1,   167,    -1,
      20,   157,   202,   158,   154,   164,   155,    -1,    18,   156,
      -1,    18,   203,   156,    -1,   207,    -1,   169,   132,   207,
      -1,    12,   157,   202,   158,   166,     6,   166,    -1,    12,
     157,   202,   158,   166,    -1,    -1,   202,    -1,     7,    -1,
     172,   157,   202,   158,   154,   174,   155,    -1,    -1,   174,
     175,    -1,     8,   202,   136,   164,     3,   156,    -1,     8,
     202,   136,   164,    -1,     9,   136,   164,    -1,   109,    -1,
      16,    -1,    17,    -1,    23,    -1,    24,    -1,    25,    -1,
      26,    -1,    27,    -1,    28,    -1,    29,    -1,    30,    -1,
      31,    -1,    32,    -1,    33,    -1,    34,    -1,    35,    -1,
      36,    -1,    37,    -1,    38,    -1,    39,    -1,    40,    -1,
      41,    -1,    42,    -1,    43,    -1,    44,    -1,    45,    -1,
      46,    -1,    47,    -1,    48,    -1,    49,    -1,    50,    -1,
      51,    -1,    52,    -1,    53,    -1,    54,    -1,    55,    -1,
      56,    -1,    57,    -1,    58,    -1,    59,    -1,    60,    -1,
      61,    -1,    62,    -1,    63,    -1,    64,    -1,    65,    -1,
      66,    -1,    67,    -1,    68,    -1,    69,    -1,    70,    -1,
      71,    -1,    72,    -1,    73,    -1,    74,    -1,    75,    -1,
      76,    -1,    77,    -1,    78,    -1,    79,    -1,    80,    -1,
      81,    -1,    82,    -1,    83,    -1,    99,    -1,   100,    -1,
      84,    -1,    85,    -1,    86,    -1,    87,    -1,    88,    -1,
      89,    -1,    90,    -1,    91,    -1,    92,    -1,    93,    -1,
      94,    -1,    95,    -1,    -1,   176,    -1,   177,   132,   176,
      -1,     5,    -1,   178,   176,   157,   177,   158,   166,    -1,
     176,    -1,   202,   151,   176,    -1,   202,   152,   202,   153,
      -1,   192,    -1,   195,    -1,    13,    -1,    14,    -1,    -1,
      -1,   181,   157,   202,   158,   166,    -1,   182,   166,    13,
     157,   202,   158,    -1,    10,   157,   186,   156,   171,   156,
     186,   158,   183,   166,    -1,    10,   157,   176,    11,   180,
     158,   184,   166,    -1,    10,   157,    15,   176,    11,   180,
     158,   184,   166,    -1,    -1,   169,    -1,     4,   156,    -1,
       3,   156,    -1,    -1,   132,    93,    -1,   132,    92,    -1,
     132,   108,    -1,   104,    -1,   202,    -1,   109,   157,   193,
     158,    -1,    17,   109,    -1,    17,   200,   151,   109,    -1,
      17,   200,   152,   202,   153,    -1,    28,   157,   203,   158,
      -1,    32,   157,   202,   132,   202,   189,   158,    -1,    94,
     157,   202,   132,   202,   189,   158,    -1,    96,   157,   202,
     132,   190,   189,   158,    -1,    95,   157,   202,   132,   202,
     189,   158,    -1,    97,   157,   202,   132,   190,   189,   158,
      -1,    98,   157,   202,   158,    -1,    99,   157,   202,   132,
     202,   158,    -1,    99,   157,   202,   132,   202,   132,   202,
     132,   202,   132,   202,   132,   202,   158,    -1,   100,   157,
     158,    -1,    82,   157,   202,   132,   202,   132,   202,   158,
      -1,    83,   157,   202,   158,    -1,    33,   157,   108,   132,
     108,   158,    -1,    34,   157,   158,    -1,    35,   157,   158,
      -1,    36,   157,   158,    -1,    37,   157,   158,    -1,    39,
     157,   158,    -1,    38,   157,   158,    -1,   101,   157,   104,
     158,    -1,   101,   157,   108,   158,    -1,   101,   157,   202,
     158,    -1,   102,   157,   108,   158,    -1,   102,   157,   202,
     158,    -1,   109,   157,   193,   158,    -1,    22,   157,   202,
     158,    -1,    24,   157,   158,    -1,    23,   157,   202,   158,
      -1,    25,   157,   203,   158,    -1,    29,   157,   202,   158,
      -1,    30,   157,   202,   158,    -1,    31,   157,   202,   158,
      -1,    26,   157,   202,   132,   202,   158,    -1,    78,   157,
     202,   132,   202,   132,   202,   158,    -1,    56,   157,   203,
     158,    -1,    -1,   203,    -1,   193,   132,   203,    -1,   178,
     157,   177,   158,   166,    -1,   200,   151,   176,   157,   193,
     158,    -1,   200,   152,   202,   153,   157,   193,   158,    -1,
     176,   136,   203,    -1,   196,    -1,   197,   132,   196,    -1,
     118,    -1,   121,    -1,   120,    -1,   119,    -1,   122,    -1,
     123,    -1,   124,    -1,   134,    -1,   128,    -1,   129,    -1,
     130,    -1,   116,    -1,   117,    -1,   201,    -1,   192,    -1,
     195,    -1,   176,    -1,   200,   151,   176,    -1,   200,   152,
     202,   153,    -1,   204,    -1,   143,   202,    -1,   148,   202,
      -1,   147,   202,    -1,   202,   115,   202,    -1,   202,   114,
     202,    -1,   202,   144,   202,    -1,   202,   145,   202,    -1,
     202,   146,   202,    -1,   202,   142,   202,    -1,   202,   143,
     202,    -1,   202,   139,   202,    -1,   202,   140,   202,    -1,
     202,   141,   202,    -1,   202,   137,   202,    -1,   202,   138,
     202,    -1,   202,   111,   202,    -1,   202,   112,   202,    -1,
     202,   110,   202,    -1,   202,   113,   202,    -1,   202,   125,
     202,    -1,   202,   126,   202,    -1,   202,   127,   202,    -1,
     202,   135,   202,   136,   202,    -1,   201,   133,   203,    -1,
     202,    57,   200,    -1,   202,    -1,    16,   176,    -1,    16,
     176,   157,   193,   158,    -1,   152,   193,   153,    -1,   165,
      -1,   154,   197,   155,    -1,   192,    -1,   194,    -1,   195,
      -1,   200,    -1,   199,   201,    -1,   201,   199,    -1,   157,
     202,   158,    -1,   143,   104,    -1,   104,    -1,   143,   105,
      -1,   105,    -1,   106,    -1,   103,    -1,   108,    -1,   206,
      -1,   205,   132,   206,    -1,   176,   133,   203,    -1,   176,
      -1,    -1,    21,   154,   208,   209,   155,    -1,    15,   205,
      -1,   191,    -1,   192,    -1,   195,    -1,   199,   201,    -1,
     201,   199,    -1,   201,   133,   203,    -1,   201,   198,   202,
      -1,   214,    -1,   209,   214,    -1,    -1,    20,   211,   209,
      19,    -1,   108,    -1,   104,    -1,   105,    -1,   106,    -1,
     103,    -1,   107,    -1,   212,    -1,   213,   132,   212,    -1,
      -1,    43,   215,   213,    -1,   210,    -1,    44,   107,    -1,
      45,    -1,    18,    -1,    46,    -1,    47,    -1,    48,    -1,
      49,    -1,    50,    -1,    51,    -1,    52,    -1,    53,    -1,
      54,    -1,    55,    -1,    56,    -1,    57,    -1,    58,    -1,
      17,    -1,    16,    -1,    60,    -1,    59,    -1,    61,    -1,
      62,    -1,    63,    -1,    64,    -1,    65,    -1,    15,    -1,
      66,    -1,    67,    -1,    68,    -1,    69,    -1,    70,    -1,
      71,    -1,    72,    -1,    73,    -1,    74,    -1,    75,    -1,
      76,    -1,    77,    -1,    78,    -1,    29,    -1,    40,    -1,
      41,    -1,    42,    -1,    79,    -1,    80,    -1,    81,    -1,
      26,    -1,    82,    -1,    83,    -1,    28,    -1,    84,    -1,
      23,    -1,    85,    -1,    30,    -1,    31,    -1,    24,    -1,
      86,    -1,    87,    -1,    88,    -1,    89,   108,    -1,    90,
     108,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   152,   152,   152,   160,   164,   165,   169,   171,   176,
     179,   185,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   203,   214,   224,   236,   237,   241,   252,
     262,   263,   267,   272,   286,   289,   297,   302,   307,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
     388,   389,   390,   391,   392,   393,   394,   399,   402,   407,
     414,   419,   433,   438,   444,   449,   451,   455,   460,   466,
     471,   475,   488,   501,   536,   569,   603,   604,   610,   621,
     634,   636,   638,   640,   647,   654,   662,   670,   676,   683,
     690,   694,   701,   708,   715,   722,   729,   735,   742,   753,
     758,   766,   770,   780,   784,   788,   792,   796,   800,   804,
     810,   817,   823,   830,   839,   846,   850,   854,   858,   862,
     866,   870,   874,   879,   885,   894,   897,   902,   911,   924,
     932,   941,   948,   952,   958,   959,   960,   961,   962,   963,
     964,   965,   966,   967,   968,   972,   973,   991,  1009,  1010,
    1015,  1022,  1028,  1037,  1039,  1044,  1049,  1053,  1062,  1072,
    1077,  1082,  1087,  1092,  1097,  1102,  1107,  1112,  1117,  1122,
    1128,  1133,  1137,  1142,  1146,  1150,  1154,  1164,  1191,  1199,
    1201,  1207,  1213,  1218,  1223,  1231,  1233,  1235,  1237,  1239,
    1284,  1329,  1332,  1336,  1340,  1344,  1348,  1352,  1356,  1363,
    1365,  1371,  1377,  1385,  1384,  1389,  1392,  1394,  1398,  1402,
    1441,  1480,  1501,  1546,  1547,  1552,  1551,  1559,  1562,  1565,
    1568,  1571,  1573,  1580,  1581,  1585,  1585,  1591,  1593,  1600,
    1602,  1604,  1606,  1608,  1610,  1612,  1614,  1616,  1618,  1620,
    1622,  1624,  1626,  1628,  1630,  1632,  1634,  1636,  1638,  1640,
    1642,  1644,  1646,  1648,  1650,  1654,  1655,  1656,  1657,  1658,
    1659,  1660,  1661,  1662,  1663,  1664,  1665,  1666,  1667,  1668,
    1669,  1670,  1671,  1672,  1673,  1674,  1675,  1676,  1677,  1678,
    1679,  1680,  1681,  1682,  1683,  1684,  1685,  1688,  1692
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BREAK", "CONTINUE", "FUNCTION", "ELSE",
  "SWITCH", "CASE", "DEFAULT", "FOR", "IN", "IF", "WHILE", "DO", "VAR",
  "NEW", "DELETE", "RETURN", "END", "WITH", "ASM", "EVAL", "RANDOM",
  "GETTIMER", "LENGTH", "CONCAT", "SUBSTR", "TRACE", "INT", "ORD", "CHR",
  "GETURL", "GETURL1", "NEXTFRAME", "PREVFRAME", "PLAY", "STOP",
  "TOGGLEQUALITY", "STOPSOUNDS", "DUP", "SWAP", "POP", "PUSH",
  "SETREGISTER", "CALLFUNCTION", "CALLMETHOD", "AND", "OR", "XOR",
  "MODULO", "ADD", "LESSTHAN", "EQUALS", "INC", "DEC", "TYPEOF",
  "INSTANCEOF", "ENUMERATE", "INITOBJECT", "INITARRAY", "GETMEMBER",
  "SETMEMBER", "SHIFTLEFT", "SHIFTRIGHT", "SHIFTRIGHT2", "VAREQUALS",
  "OLDADD", "SUBTRACT", "MULTIPLY", "DIVIDE", "OLDEQUALS", "OLDLESSTHAN",
  "LOGICALAND", "LOGICALOR", "NOT", "STRINGEQ", "STRINGLENGTH",
  "SUBSTRING", "GETVARIABLE", "SETVARIABLE", "SETTARGETEXPRESSION",
  "DUPLICATEMOVIECLIP", "REMOVEMOVIECLIP", "STRINGLESSTHAN", "MBLENGTH",
  "MBSUBSTRING", "MBORD", "MBCHR", "BRANCHALWAYS", "BRANCHIFTRUE",
  "GETURL2", "POST", "GET", "LOADVARIABLES", "LOADMOVIE",
  "LOADVARIABLESNUM", "LOADMOVIENUM", "CALLFRAME", "STARTDRAG", "STOPDRAG",
  "GOTOFRAME", "SETTARGET", "NULLVAL", "INTEGER", "DOUBLE", "BOOLEAN",
  "REGISTER", "STRING", "IDENTIFIER", "\"==\"", "\"<=\"", "\">=\"",
  "\"!=\"", "\"&&\"", "\"||\"", "\"++\"", "\"--\"", "\"+=\"", "\"/=\"",
  "\"*=\"", "\"-=\"", "\"%=\"", "\"&=\"", "\"|=\"", "\"<<\"", "\">>\"",
  "\">>>\"", "\"<<=\"", "\">>=\"", "\">>>=\"", "NOELSE", "','", "'='",
  "\"^=\"", "'?'", "':'", "'<'", "'>'", "'&'", "'|'", "'^'", "'+'", "'-'",
  "'*'", "'/'", "'%'", "'!'", "'~'", "UMINUS", "POSTFIX", "'.'", "'['",
  "']'", "'{'", "'}'", "';'", "'('", "')'", "$accept", "program", "@1",
  "code", "anycode", "stmts", "emptybraces", "stmt", "with_stmt",
  "return_stmt", "assign_stmts", "if_stmt", "expr_opt", "switch_init",
  "switch_stmt", "switch_cases", "switch_case", "identifier",
  "formals_list", "function_init", "function_decl", "obj_ref",
  "while_init", "do_init", "for_init", "for_in_init", "iter_stmt",
  "assign_stmts_opt", "cont_stmt", "break_stmt", "urlmethod", "level",
  "void_function_call", "function_call", "expr_list", "anon_function_decl",
  "method_call", "objexpr", "objexpr_list", "assignop", "incdecop",
  "lvalue_expr", "lvalue", "expr", "expr_or_obj", "primary", "init_vars",
  "init_var", "assign_stmt", "@2", "opcode_list", "with", "@3",
  "push_item", "push_list", "opcode", "@4", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,    44,    61,   387,    63,    58,    60,    62,    38,
     124,    94,    43,    45,    42,    47,    37,    33,   126,   388,
     389,    46,    91,    93,   123,   125,    59,    40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   159,   161,   160,   160,   162,   162,   163,   163,   164,
     164,   165,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   167,   168,   168,   169,   169,   170,   170,
     171,   171,   172,   173,   174,   174,   175,   175,   175,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   177,   177,   177,
     178,   179,   180,   180,   180,   180,   180,   181,   182,   183,
     184,   185,   185,   185,   185,   185,   186,   186,   187,   188,
     189,   189,   189,   189,   190,   190,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   193,   193,   193,   194,   195,
     195,   196,   197,   197,   198,   198,   198,   198,   198,   198,
     198,   198,   198,   198,   198,   199,   199,   200,   200,   200,
     201,   201,   201,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   203,
     203,   203,   203,   203,   203,   204,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   204,   204,   204,   204,   205,
     205,   206,   206,   208,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   209,   209,   211,   210,   212,   212,   212,
     212,   212,   212,   213,   213,   215,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     0,     1,     2,     1,     1,     1,
       2,     2,     1,     3,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     7,     2,     3,     1,     3,     7,     5,
       0,     1,     1,     7,     0,     2,     6,     4,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     3,
       1,     6,     1,     3,     4,     1,     1,     1,     1,     0,
       0,     5,     6,    10,     8,     9,     0,     1,     2,     2,
       0,     2,     2,     2,     1,     1,     4,     2,     4,     5,
       4,     7,     7,     7,     7,     7,     4,     6,    14,     3,
       8,     4,     6,     3,     3,     3,     3,     3,     3,     4,
       4,     4,     4,     4,     4,     4,     3,     4,     4,     4,
       4,     4,     6,     8,     4,     0,     1,     3,     5,     6,
       7,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     4,     1,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     5,     3,     3,     1,
       2,     5,     3,     1,     3,     1,     1,     1,     1,     2,
       2,     3,     2,     1,     2,     1,     1,     1,     1,     1,
       3,     3,     1,     0,     5,     2,     1,     1,     1,     2,
       2,     3,     3,     1,     2,     0,     4,     1,     1,     1,
       1,     1,     1,     1,     3,     0,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     0,     1,     0,     0,   120,    32,     0,     0,
     127,   128,     0,    40,    41,     0,     0,     0,     0,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,     0,     0,   103,   104,     0,     0,    39,
     205,   206,     0,    14,     3,     5,    12,     7,    22,    21,
      16,     0,    20,   210,     0,     8,     0,     0,    17,    18,
      19,   266,   267,   268,     0,     0,   207,     0,   139,   138,
     136,     0,    41,    42,    43,    44,    45,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    75,
      97,   101,   102,   115,   116,   103,   104,    39,   262,   265,
     259,   147,   208,   209,     0,   207,    40,   257,   253,   255,
     256,   258,    39,     0,     0,     0,   185,     0,    24,     0,
     243,     0,   208,   246,   209,     0,   248,   207,   239,     0,
     213,     0,   263,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   185,    11,     0,     9,     6,     0,     0,
       0,     0,   269,     0,     0,   194,   197,   196,   195,   198,
     199,   200,   202,   203,   204,     0,   201,     0,   270,    15,
       0,   137,   210,     0,    26,     0,     0,     0,   185,     0,
       0,   240,   252,   254,   214,   216,   215,     0,   186,     0,
     192,     0,     0,   117,   249,     0,   250,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,   176,     0,     0,     0,     0,     0,     0,
       0,     0,   163,   164,   165,   166,   168,   167,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   159,   253,
     258,     0,   258,     0,     0,    13,    10,     0,   117,     0,
       0,   211,     0,   271,   272,   262,     0,     0,    30,     0,
     261,   260,     0,    39,     0,   185,     0,   242,     0,     0,
     244,   251,   118,     0,   237,   238,   231,   229,   230,   232,
     218,   217,   233,   234,   235,     0,   227,   228,   224,   225,
     226,   222,   223,   219,   220,   221,     0,   313,   305,   304,
     290,   275,   339,   343,   334,   337,   327,   341,   342,   328,
     329,   330,   285,     0,   289,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   307,   306,
     308,   309,   310,   311,   312,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   331,   332,
     333,   335,   336,   338,   340,   344,   345,   346,     0,     0,
       0,   287,   273,   175,   177,   178,     0,   150,   179,   180,
     181,     0,     0,   184,     0,     0,   161,     0,     0,     0,
       0,   156,     0,   169,   170,   171,   172,   173,   146,     0,
       0,     0,     0,   185,   212,     0,    27,   210,     0,   208,
     209,     0,     0,    31,    29,   174,   149,     0,   187,   191,
     193,     0,     0,     0,     0,     0,     0,   288,   347,   348,
     264,   274,     0,   140,     0,     0,     0,   140,   140,   253,
     140,   145,   140,     0,    34,     0,   131,     0,     0,   185,
       0,   130,     0,     0,   136,     0,   241,   119,   188,   236,
       0,     0,   281,   278,   279,   280,   282,   277,   283,   286,
     182,     0,     0,   162,     0,     0,     0,     0,     0,     0,
       0,   157,     0,   121,   132,   189,     0,   130,     0,   123,
       0,     0,    28,    23,   276,     0,   142,   141,   143,   151,
       0,     0,   152,   154,   153,   155,     0,     0,     0,    33,
      35,   190,     0,   134,   124,   129,   284,   183,   160,     0,
       0,     0,   135,     0,     0,     0,    38,   133,     0,    37,
       0,     0,     0,   139,     0,   158
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   104,   105,   225,   106,   226,   108,   109,
     251,   110,   492,   111,   112,   562,   590,   113,   363,   181,
     115,   488,   116,   117,   603,   568,   118,   253,   119,   120,
     552,   520,   121,   182,   267,   183,   184,   270,   271,   247,
     185,   186,   187,   188,   268,   190,   159,   160,   127,   300,
     450,   451,   505,   548,   549,   452,   506
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -496
static const yytype_int16 yypact[] =
{
      17,    30,   908,  -496,  -136,  -123,  -496,  -496,  -126,  -105,
    -496,  -496,  4178,  -496,  4003,  1907,  -101,  -120,   -88,   -80,
     -75,   -72,   -65,  -496,   -62,   -50,   -43,   -40,   -38,   -31,
     -30,   -28,   -27,   -25,   -24,   -21,  -496,  -496,  -496,  -496,
    -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,   -20,  -496,  -496,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,  -496,  -496,   -18,  -496,  -496,  -496,   -17,   -16,
    -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,
     -15,   -11,    -8,    -5,     5,     6,     7,     8,     9,    22,
    -496,  -496,  1165,  -496,   908,  -496,  -496,  -496,  -496,  -496,
    -496,    23,  -496,  -496,  4178,  -496,    24,  1627,  -496,  -496,
    -496,  -496,  -125,   -97,  4091,   -78,   -58,   -57,  -496,  -496,
    3137,  2190,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,    49,   -96,
    -496,   -71,  -496,  -496,   -64,  -496,  4178,  -496,  -496,  -496,
    -496,  -496,    27,  2326,  2190,  2190,  2051,  3002,  -496,  2190,
    -496,    37,  -496,  -496,  -496,  4091,   -78,   -49,  4896,   -47,
    -496,  2190,  -496,  2190,  2190,    -3,  2051,  2190,  2051,  2190,
    2190,  2190,  2190,    45,    40,    42,    47,    48,    51,    52,
    2051,  2190,  2190,  2190,  2190,  2190,  2190,  2190,  2190,  2190,
      53,  2462,  2598,  2051,  -496,  1319,  -496,  -496,  2190,    39,
    2190,   143,   -54,  4178,  2190,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,  -496,  -496,  -496,  2051,  -496,  2190,  -496,  -496,
    4178,    69,   174,    46,  -496,   331,  2051,  4178,  2051,  4265,
    2190,    50,  -496,  -496,   146,   146,   146,  -103,  -496,    76,
    -496,  -113,   576,  4178,   -54,  2051,  -496,  4091,  2190,  2190,
    2190,  2190,  2190,  2190,  2190,  2190,  2190,  2190,  2190,  2190,
    2190,  2190,  2190,  2190,  2190,  2190,  2190,  2190,  -496,   750,
    5163,   962,   999,  -496,    55,   688,    56,  2850,  3280,  3317,
    4304,    86,  -496,  -496,  -496,  -496,  -496,  -496,    58,  4341,
    4378,  3354,  4415,  4452,  4489,  4526,  3391,  4563,  -496,    61,
      62,  3428,    64,  3465,  -119,  -496,  -496,  3502,  4178,  3539,
      68,    70,  3761,  -496,  4896,    -9,  3234,  2190,  2190,  1627,
    -496,  -496,  -118,  -121,  3798,  2051,  2051,  -496,  2051,  4178,
    -496,  -496,  -496,  -117,  -496,   -78,  4955,   192,   192,  4955,
    4933,  4933,    26,    26,    26,  4600,   192,   192,    32,    32,
      32,   -33,   -33,   146,   146,   146,    75,  -496,  -496,  -496,
    -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,  -496,   123,  -496,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,
    -496,  -496,  -496,  -496,  -496,  -496,  -496,  -496,   124,   125,
    2869,  -496,  -496,  -496,  -496,  -496,  2190,  -496,  -496,  -496,
    -496,  2190,   127,  -496,  2190,  2190,  -496,  2190,  2190,  2734,
    2734,  -496,  2190,  -496,  -496,  -496,  -496,  -496,   -48,    77,
    -114,  1627,  2190,  2051,    80,  2190,  -496,    82,    83,    88,
      89,  3872,    94,  4896,   236,  -496,   -61,  -111,  -496,  -496,
    -496,  4178,  1627,  2190,  1627,  5163,    84,  -496,  -496,  -496,
    -496,  -496,  3576,  4637,    96,  4674,  4711,  4637,  4637,  -110,
     112,  4896,   112,   289,  -496,  1627,  -496,  3613,  -109,  2051,
      97,  -496,  4178,  2190,  3234,  1627,  -496,  -496,  -496,  4896,
    1473,  5087,  -496,  -496,  -496,  -496,  -496,  -496,  -496,   120,
    -496,   -14,    98,  -496,  2190,  2190,    99,   101,   102,   103,
    2190,  -496,    -4,  -496,  -496,  -496,  -107,  -496,  1627,  -496,
    3835,   104,  -496,  -496,  -496,    84,  -496,  -496,  -496,  -496,
    3650,  3687,  -496,  -496,  -496,  -496,  4748,  2190,   128,  -496,
    -496,  -496,  1627,  -496,  -496,  -496,  -496,  -496,  -496,  2190,
    4785,  1627,  -496,  1627,  4822,  1627,  1627,  -496,  2190,  1772,
    4859,   109,  2190,    -1,  3724,  -496
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -496,  -496,  -496,  -496,   162,  -495,    28,    44,  -496,  -496,
    -496,  -496,  -496,  -496,  -496,  -496,  -496,    20,   -66,     4,
    -496,  -211,  -496,  -496,  -496,  -289,  -496,  -254,  -496,  -496,
    -397,  -189,  -496,    -2,  -220,  -496,    14,   -77,  -496,  -496,
     141,    43,    91,   562,    38,  -496,  -496,    31,  -129,  -496,
    -218,  -496,  -496,  -290,  -496,  -440,  -496
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -213
static const yytype_int16 yytable[] =
{
     122,   254,   485,   334,   587,   588,   114,   -36,   -36,   540,
     511,  -148,   162,   356,   356,   501,   123,    -4,   501,   359,
     128,   356,  -144,   356,   277,   356,  -208,  -208,   163,   356,
       3,   130,   158,   129,   192,  -148,   257,  -148,   352,   478,
     495,   502,   360,   180,   525,   125,   107,   536,  -144,   565,
     357,   591,   131,   189,  -209,  -209,   191,   164,   100,   101,
     235,   236,   237,   238,   239,   240,   241,   100,   101,   193,
     242,   243,   244,   233,   234,   245,   246,   194,   576,   577,
     -39,   -39,   195,   277,   275,   196,   258,   259,   260,   277,
    -212,  -212,   197,   126,   578,   198,   529,  -207,  -207,   249,
     122,   511,   122,  -174,  -174,   165,   606,   199,   114,   298,
     609,   295,   296,   297,   200,   122,   123,   201,   123,   202,
     556,   557,   162,   558,   256,   559,   203,   204,   122,   205,
     206,   123,   207,   208,   229,   497,   209,   210,   163,   211,
     212,   213,   214,   124,   123,   125,   215,   125,   107,   216,
     252,   589,   217,   311,   -36,   303,   340,   284,   285,   286,
     125,   231,   218,   219,   220,   221,   222,   125,   293,   294,
     295,   296,   297,   125,   293,   294,   295,   296,   297,   223,
     228,   230,   256,   162,   258,   347,   261,   542,   543,   544,
     545,   546,   547,   126,   273,   126,   338,   269,   312,   163,
     313,   346,   348,   277,   180,   314,   315,   355,   126,   316,
     317,   328,   358,   455,   457,   232,   463,   486,   462,   473,
     474,   126,   476,   122,   180,   482,   180,   483,   125,   504,
     507,   524,   508,   509,   304,   514,   306,   529,   180,   123,
    -122,   531,   535,   124,   551,   124,  -125,  -126,   318,   277,
     534,   180,   575,   341,   553,   567,   579,   582,   124,   583,
     584,   585,   595,   528,   601,   613,   227,   248,   125,   336,
     345,   124,   480,   180,   530,   162,   274,   158,   592,   341,
     571,   522,   500,   343,   180,   596,   180,   541,   351,     0,
       0,   163,     0,   362,   350,     0,     0,     0,     0,     0,
       0,     0,     0,   180,     0,     0,     0,     0,     0,   566,
       0,     0,     0,   364,     0,     0,   126,   284,   285,   286,
     365,     0,     0,     0,     0,     0,     0,     0,   276,     0,
       0,   290,   291,   292,   293,   294,   295,   296,   297,     0,
       0,     0,     0,     0,   122,   489,   277,   122,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   362,     0,
     123,   490,     0,   123,     0,     0,   124,   487,   165,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   269,
       0,     0,     0,   180,   180,     0,   180,     0,   277,   125,
       0,     0,   125,   494,   498,     0,   499,     0,     0,   278,
     279,   280,   281,   282,   283,   254,     0,     0,     0,     0,
       0,     0,     0,     0,   284,   285,   286,     0,     0,     0,
       0,   560,     0,     0,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,     0,   126,     0,     0,
     126,   278,   279,   280,   281,   282,   283,   561,     0,     0,
       0,     0,     0,     0,     0,     0,   284,   285,   286,     0,
       0,     0,     0,     0,     0,     0,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,     0,   122,
       0,     0,     0,   489,     0,     0,     0,   124,     0,   349,
     124,     0,     0,     0,     0,   123,     0,     0,     0,   490,
     122,     0,   122,     0,     0,   487,     0,     0,     0,     0,
       0,   180,     0,     0,     0,     0,   123,     0,   123,     0,
       0,   537,     0,   122,   125,   526,     0,     0,     0,     0,
       0,     0,   122,   122,     0,     0,     0,     0,   122,   123,
       0,     0,     0,     0,     0,   125,   538,   125,   123,   123,
       0,     0,   569,     0,   123,     0,     0,   180,     0,     0,
       0,     0,     0,     0,     0,     0,   122,     0,   125,   563,
       0,     0,   126,     0,     0,     0,     0,   125,   125,   572,
       0,     0,   123,   125,   336,     0,     0,     0,     0,     0,
     122,     0,     0,   126,     0,   126,     0,     0,     0,   122,
       0,   122,     0,   122,   122,     0,   123,   122,     0,     0,
       0,   125,   593,     0,     0,   123,   126,   123,     0,   123,
     123,     0,   124,   123,     0,   126,   126,     0,     0,     0,
       0,   126,     0,   277,     0,   125,   602,     0,     0,     0,
       0,     0,     0,   124,   125,   124,   125,   607,   125,   125,
     336,     0,   125,   336,     0,     0,     0,     0,     0,   126,
       0,     0,     0,     0,     0,     0,   124,     0,     0,     0,
       0,     0,     0,     0,     0,   124,   124,     0,     0,     0,
       0,   124,     0,   126,     0,     0,   278,   279,   280,   281,
     282,   283,   126,   255,   126,     0,   126,   126,     0,     0,
     126,   284,   285,   286,     0,     0,     0,     0,     0,   124,
       0,   287,     0,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   124,   361,   264,   265,   266,     0,     0,
       0,   272,   124,     0,   124,   277,   124,   124,     0,     0,
     124,     0,     0,   299,     0,   301,   302,     0,     0,   305,
       0,   307,   308,   309,   310,     0,     0,     0,     0,     0,
       0,     0,     0,   319,   320,   321,   322,   323,   324,   325,
     326,   327,     0,   331,   333,     0,     0,     0,     0,     0,
     337,     0,   339,     0,     0,     0,   342,     0,   278,   279,
     280,   281,   282,   283,     0,     0,     0,   277,     0,   344,
       0,     0,     0,   284,   285,   286,     0,     0,     0,     0,
     456,     0,   354,   287,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,     0,     0,     0,     0,     0,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     278,   279,   280,   281,   282,   283,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   284,   285,   286,     0,     0,
       0,     0,     0,     0,     0,   287,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   386,   491,
     493,     4,     5,     6,     0,     7,     0,     0,     8,     0,
       9,    10,    11,    12,    13,    14,    15,     0,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,     0,     0,     0,     0,     0,     0,    99,   512,   277,
       0,     0,     0,   513,   100,   101,   515,   516,     0,   517,
     518,   521,   521,     0,   523,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   527,     0,     0,   491,     0,     0,
       0,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,     0,   102,     0,   103,   539,     0,     0,     0,     0,
       0,     0,   278,   279,   280,   281,   282,   283,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   284,   285,   286,
       0,     0,     0,     0,     0,   570,     0,   287,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   278,
     279,   280,   281,   282,   283,     0,   580,   581,     0,     0,
     453,     0,   586,     0,   284,   285,   286,     0,     0,     0,
       0,     0,     0,     0,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,     0,     0,     0,   600,
       0,     0,     0,     0,     0,     0,     0,   454,     0,     0,
       0,   604,     0,     0,     0,     0,     0,     0,     4,     5,
     610,     0,     7,     0,   614,     8,     0,     9,    10,    11,
      12,    13,    14,    15,     0,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,     0,     0,
       0,     0,     0,     0,    99,     0,     0,     0,     0,     0,
       0,   100,   101,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
     224,   103,     4,     5,     0,     0,     7,     0,     0,     8,
       0,     9,    10,    11,    12,    13,    14,    15,     0,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,     0,     0,     0,     0,     0,     0,    99,     0,
       0,     0,     0,     0,     0,   100,   101,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   102,   335,   103,     4,     5,     0,     0,
       7,     0,     0,     8,     0,     9,    10,    11,    12,    13,
      14,    15,     0,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,     0,     0,     0,     0,
       0,     0,    99,     0,     0,     0,     0,     0,     0,   100,
     101,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   102,   573,   103,
       4,     5,     0,     0,     7,     0,     0,     8,     0,     9,
      10,    11,    12,    13,    14,    15,     0,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
       0,     0,     0,     0,     0,     0,    99,     0,     0,     0,
       0,     0,     0,   100,   101,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   611,     5,     0,     0,     7,
       0,   102,     8,   103,     9,    10,    11,    12,    13,    14,
      15,     0,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,     0,     0,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,   100,   101,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     6,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   166,   132,     0,   102,     0,   103,    18,
      19,    20,    21,    22,    23,   137,    25,    26,    27,   141,
     142,   143,   144,   145,   146,   147,   148,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,   151,
     152,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,   153,   154,     0,     0,     0,   155,   156,     0,     0,
     167,   168,   169,   170,     0,   171,   172,     0,     0,     0,
       0,     0,     0,   100,   101,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     173,     0,     0,     0,   174,   175,     6,     0,     0,   176,
       0,   177,     0,   178,   179,     0,     0,   166,   132,     0,
       0,     0,     0,    18,    19,    20,    21,    22,    23,   137,
      25,    26,    27,   141,   142,   143,   144,   145,   146,   147,
     148,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,   151,   152,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,   153,   154,     0,     0,     0,
     155,   156,     0,     0,   167,   168,   169,   170,     0,   171,
     172,     0,     0,     0,     0,     0,     0,   100,   101,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   173,     6,     0,     0,   174,   175,
       0,     0,     0,   176,     0,   177,    13,   132,   179,     0,
       0,     0,    18,    19,    20,    21,    22,    23,   137,    25,
      26,    27,   141,   142,   143,   144,   145,   146,   147,   148,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,   151,   152,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,   153,   154,     0,     0,     0,   155,
     156,     0,     0,   167,   168,   169,   170,     0,   171,   172,
       0,     0,     0,     0,     0,     0,   100,   101,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     6,     0,   173,     0,     0,     0,   174,   175,     0,
       0,     0,    13,   132,     0,     0,     0,   179,    18,    19,
      20,    21,    22,    23,   137,    25,    26,    27,   141,   142,
     143,   144,   145,   146,   147,   148,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,   151,   152,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
     153,   154,     0,     0,     0,   155,   156,     0,     0,   167,
     262,   263,   170,     0,   171,   172,     0,     0,     0,     0,
       0,     0,   100,   101,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     6,     0,   173,
       0,     0,     0,   174,   175,     0,     0,     0,    13,   132,
       0,     0,     0,   179,    18,    19,    20,    21,    22,    23,
     137,    25,    26,    27,   141,   142,   143,   144,   145,   146,
     147,   148,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,   151,   152,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,   153,   154,     0,     0,
       0,   155,   156,     0,     0,   167,   329,   169,   170,     0,
     330,   172,     0,     0,     0,     0,     0,     0,   100,   101,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     6,     0,   173,     0,     0,     0,   174,
     175,     0,     0,     0,    13,   132,     0,     0,     0,   179,
      18,    19,    20,    21,    22,    23,   137,    25,    26,    27,
     141,   142,   143,   144,   145,   146,   147,   148,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
     151,   152,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,   153,   154,     0,     0,     0,   155,   156,     0,
       0,   167,   168,   169,   170,     0,   332,   172,     0,     0,
       0,     0,     0,     0,   100,   101,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     6,
       0,   173,     0,     0,     0,   174,   175,     0,     0,     0,
      13,   132,     0,     0,     0,   179,    18,    19,    20,    21,
      22,    23,   137,    25,    26,    27,   141,   142,   143,   144,
     145,   146,   147,   148,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,   151,   152,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,   153,   154,
       0,     0,     0,   155,   156,     0,     0,   167,   519,   169,
     170,     0,   171,   172,     0,     0,     0,     0,     0,     0,
     100,   101,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   173,     0,     0,
       0,   174,   175,     0,   387,   388,   389,   390,     0,   391,
       0,   179,   392,   393,     0,   394,     0,   395,   396,   397,
     398,     0,     0,     0,     0,     0,     0,   277,     0,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     278,   279,   280,   281,   282,   283,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   284,   285,   286,     0,     0,
       0,     0,     0,     0,     0,   287,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   458,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    13,   132,
       0,     0,     0,     0,   510,   133,   134,   135,   136,    23,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,   149,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
     150,    75,    76,    77,   151,   152,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,   153,   154,     0,     0,
       0,   155,   156,     0,     0,     0,     0,     0,     0,     0,
       0,   157,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   250,    13,    14,     0,     0,   224,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
       0,     0,     0,     0,     0,     0,    99,     0,     0,    12,
      13,    14,     0,   100,   101,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,   277,     0,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
     100,   101,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     278,   279,   280,   281,   282,   283,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   284,   285,   286,     0,     0,
       0,   277,     0,     0,     0,   287,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   278,   279,   280,
     281,   282,   283,     0,     0,     0,     0,     0,   459,     0,
       0,     0,   284,   285,   286,     0,     0,     0,   277,     0,
       0,     0,   287,     0,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   278,   279,   280,   281,   282,   283,
       0,     0,     0,     0,     0,   460,     0,     0,     0,   284,
     285,   286,     0,     0,     0,   277,     0,     0,     0,   287,
       0,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   278,   279,   280,   281,   282,   283,     0,     0,     0,
       0,     0,   466,     0,     0,     0,   284,   285,   286,     0,
       0,     0,   277,     0,     0,     0,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   278,   279,
     280,   281,   282,   283,     0,     0,     0,     0,     0,   471,
       0,     0,     0,   284,   285,   286,     0,     0,     0,   277,
       0,     0,     0,   287,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   278,   279,   280,   281,   282,
     283,     0,     0,     0,     0,     0,   475,     0,     0,     0,
     284,   285,   286,     0,     0,     0,   277,     0,     0,     0,
     287,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   278,   279,   280,   281,   282,   283,     0,     0,
       0,     0,     0,   477,     0,     0,     0,   284,   285,   286,
       0,     0,     0,   277,     0,     0,     0,   287,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   278,
     279,   280,   281,   282,   283,     0,     0,     0,     0,     0,
     479,     0,     0,     0,   284,   285,   286,     0,     0,     0,
     277,     0,     0,     0,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   278,   279,   280,   281,
     282,   283,     0,     0,     0,     0,     0,   481,     0,     0,
       0,   284,   285,   286,     0,     0,     0,   277,     0,     0,
       0,   287,     0,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   278,   279,   280,   281,   282,   283,     0,
       0,     0,     0,     0,   550,     0,     0,     0,   284,   285,
     286,     0,     0,     0,   277,     0,     0,     0,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     278,   279,   280,   281,   282,   283,     0,     0,     0,     0,
       0,   564,     0,     0,     0,   284,   285,   286,     0,     0,
       0,   277,     0,     0,     0,   287,     0,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   278,   279,   280,
     281,   282,   283,     0,     0,     0,     0,     0,   597,     0,
       0,     0,   284,   285,   286,     0,     0,     0,   277,     0,
       0,     0,   287,     0,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   278,   279,   280,   281,   282,   283,
       0,     0,     0,     0,     0,   598,     0,     0,     0,   284,
     285,   286,     0,     0,     0,   277,     0,     0,     0,   287,
       0,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   278,   279,   280,   281,   282,   283,     0,     0,     0,
       0,     0,   615,     0,     0,     0,   284,   285,   286,     0,
       0,     0,   277,     0,     0,     0,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   278,   279,
     280,   281,   282,   283,   484,     0,     0,     0,     0,     0,
       0,     0,     0,   284,   285,   286,     0,     0,     0,   277,
       0,     0,     0,   287,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   278,   279,   280,   281,   282,
     283,   496,     0,     0,     0,     0,     0,     0,     0,     0,
     284,   285,   286,     0,     0,     0,     0,     0,     0,     0,
     287,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   278,   279,   280,   281,   282,   283,   594,     0,
       0,     0,     0,     0,     0,     0,     0,   284,   285,   286,
       0,     0,     0,     0,     0,     0,     0,   287,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,    13,
     132,     0,     0,   532,   533,    18,    19,    20,    21,    22,
      23,   137,    25,    26,    27,   141,   142,   143,   144,   145,
     146,   147,   148,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,   151,   152,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,   153,   154,     0,
       0,     0,   155,   156,     0,     0,     0,    13,   132,     0,
       0,     0,   161,    18,    19,    20,    21,    22,    23,   137,
      25,    26,    27,   141,   142,   143,   144,   145,   146,   147,
     148,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,   151,   152,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,   153,   154,     0,     0,     0,
     155,   156,     0,     0,    13,   132,     0,     0,     0,     0,
     172,   133,   134,   135,   136,    23,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,   149,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,   150,    75,    76,    77,
     151,   152,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,   153,   154,     0,     0,     0,   155,   156,     0,
       0,    13,   132,     0,     0,     0,     0,   157,   133,   134,
     135,   136,    23,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,   149,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,   150,    75,    76,    77,   151,   152,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,   153,
     154,   277,     0,     0,   155,   156,     0,     0,     0,     0,
       0,     0,     0,     0,   353,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   278,   279,   280,   281,   282,   283,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   284,
     285,   286,     0,     0,     0,   277,   461,     0,     0,   287,
       0,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   278,   279,   280,   281,   282,   283,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   284,   285,   286,     0,
       0,     0,   277,   464,     0,     0,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   278,   279,
     280,   281,   282,   283,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   284,   285,   286,     0,     0,     0,   277,
     465,     0,     0,   287,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   278,   279,   280,   281,   282,
     283,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     284,   285,   286,     0,     0,     0,   277,   467,     0,     0,
     287,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   278,   279,   280,   281,   282,   283,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   284,   285,   286,
       0,     0,     0,   277,   468,     0,     0,   287,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   278,
     279,   280,   281,   282,   283,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   284,   285,   286,     0,     0,     0,
     277,   469,     0,     0,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   278,   279,   280,   281,
     282,   283,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   284,   285,   286,     0,     0,     0,   277,   470,     0,
       0,   287,     0,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   278,   279,   280,   281,   282,   283,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   284,   285,
     286,     0,     0,     0,   277,   472,     0,     0,   287,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     278,   279,   280,   281,   282,   283,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   284,   285,   286,     0,     0,
       0,   277,     0,     0,     0,   287,   503,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   278,   279,   280,
     281,   282,   283,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   284,   285,   286,     0,     0,     0,   277,   551,
       0,     0,   287,     0,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   278,   279,   280,   281,   282,   283,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   284,
     285,   286,     0,     0,     0,   277,   554,     0,     0,   287,
       0,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   278,   279,   280,   281,   282,   283,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   284,   285,   286,     0,
       0,     0,   277,   555,     0,     0,   287,     0,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   278,   279,
     280,   281,   282,   283,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   284,   285,   286,     0,     0,     0,   277,
     599,     0,     0,   287,     0,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   278,   279,   280,   281,   282,
     283,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     284,   285,   286,     0,     0,     0,   277,     0,     0,     0,
     287,   605,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   278,   279,   280,   281,   282,   283,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   284,   285,   286,
       0,     0,     0,   277,   608,     0,     0,   287,     0,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   278,
     279,   280,   281,   282,   283,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   284,   285,   286,     0,     0,     0,
     277,   612,     0,     0,   287,     0,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   278,   279,   280,   281,
     282,   283,   277,     0,     0,     0,     0,     0,     0,     0,
       0,   284,   285,   286,     0,     0,     0,     0,     0,     0,
       0,   287,     0,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   278,   279,   280,   281,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   284,   285,
     286,     0,     0,     0,     0,     0,   279,   280,     0,     0,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     284,   285,   286,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   387,   388,   389,   390,   574,   391,     0,     0,
     392,   393,     0,   394,     0,   395,   396,   397,   398,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   443,   444,   445,   446,   447,   448,   449,   387,   388,
     389,   390,     0,   391,     0,     0,   392,   393,     0,   394,
       0,   395,   396,   397,   398,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   443,   444,   445,
     446,   447,   448,   449
};

static const yytype_int16 yycheck[] =
{
       2,   130,    11,   223,     8,     9,     2,     8,     9,   504,
     450,   132,    14,   132,   132,   132,     2,     0,   132,   132,
     156,   132,   132,   132,    57,   132,   151,   152,    14,   132,
       0,   157,    12,   156,   154,   156,   132,   158,   258,   158,
     158,   158,   155,    15,   158,     2,     2,   158,   158,   158,
     153,   158,   157,    15,   151,   152,   157,    14,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   116,   117,   157,
     128,   129,   130,   151,   152,   133,   134,   157,    92,    93,
     151,   152,   157,    57,   133,   157,   157,   151,   152,    57,
     151,   152,   157,     2,   108,   157,   157,   151,   152,   156,
     102,   541,   104,   151,   152,    14,   601,   157,   104,   156,
     605,   144,   145,   146,   157,   117,   102,   157,   104,   157,
     517,   518,   124,   520,   133,   522,   157,   157,   130,   157,
     157,   117,   157,   157,   114,   355,   157,   157,   124,   157,
     157,   157,   157,     2,   130,   102,   157,   104,   104,   157,
     130,   155,   157,   108,   155,   158,    13,   125,   126,   127,
     117,   117,   157,   157,   157,   157,   157,   124,   142,   143,
     144,   145,   146,   130,   142,   143,   144,   145,   146,   157,
     157,   157,   133,   185,   157,    11,   166,   103,   104,   105,
     106,   107,   108,   102,   157,   104,   157,   177,   158,   185,
     158,   132,   156,    57,   176,   158,   158,   157,   117,   158,
     158,   158,   136,   158,   158,   124,   158,   346,   132,   158,
     158,   130,   158,   225,   196,   157,   198,   157,   185,   154,
     107,   154,   108,   108,   196,   108,   198,   157,   210,   225,
     158,   158,     6,   102,   132,   104,   158,   158,   210,    57,
     156,   223,   132,   233,   158,   158,   158,   158,   117,   158,
     158,   158,   158,   483,   136,   156,   104,   126,   225,   225,
     250,   130,   338,   245,   485,   277,   185,   257,   567,   259,
     534,   470,   359,   245,   256,   575,   258,   505,   257,    -1,
      -1,   277,    -1,   273,   256,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   275,    -1,    -1,    -1,    -1,    -1,   529,
      -1,    -1,    -1,   275,    -1,    -1,   225,   125,   126,   127,
     277,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,    -1,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,    -1,   346,   347,    57,   349,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   338,    -1,
     346,   347,    -1,   349,    -1,    -1,   225,   347,   277,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   359,
      -1,    -1,    -1,   355,   356,    -1,   358,    -1,    57,   346,
      -1,    -1,   349,   349,   356,    -1,   358,    -1,    -1,   110,
     111,   112,   113,   114,   115,   534,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,    -1,    -1,    -1,
      -1,   132,    -1,    -1,   135,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,   346,    -1,    -1,
     349,   110,   111,   112,   113,   114,   115,   158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,   481,
      -1,    -1,    -1,   485,    -1,    -1,    -1,   346,    -1,   158,
     349,    -1,    -1,    -1,    -1,   481,    -1,    -1,    -1,   485,
     502,    -1,   504,    -1,    -1,   485,    -1,    -1,    -1,    -1,
      -1,   483,    -1,    -1,    -1,    -1,   502,    -1,   504,    -1,
      -1,   501,    -1,   525,   481,   481,    -1,    -1,    -1,    -1,
      -1,    -1,   534,   535,    -1,    -1,    -1,    -1,   540,   525,
      -1,    -1,    -1,    -1,    -1,   502,   502,   504,   534,   535,
      -1,    -1,   532,    -1,   540,    -1,    -1,   529,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   568,    -1,   525,   525,
      -1,    -1,   481,    -1,    -1,    -1,    -1,   534,   535,   535,
      -1,    -1,   568,   540,   540,    -1,    -1,    -1,    -1,    -1,
     592,    -1,    -1,   502,    -1,   504,    -1,    -1,    -1,   601,
      -1,   603,    -1,   605,   606,    -1,   592,   609,    -1,    -1,
      -1,   568,   568,    -1,    -1,   601,   525,   603,    -1,   605,
     606,    -1,   481,   609,    -1,   534,   535,    -1,    -1,    -1,
      -1,   540,    -1,    57,    -1,   592,   592,    -1,    -1,    -1,
      -1,    -1,    -1,   502,   601,   504,   603,   603,   605,   606,
     606,    -1,   609,   609,    -1,    -1,    -1,    -1,    -1,   568,
      -1,    -1,    -1,    -1,    -1,    -1,   525,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   534,   535,    -1,    -1,    -1,
      -1,   540,    -1,   592,    -1,    -1,   110,   111,   112,   113,
     114,   115,   601,   131,   603,    -1,   605,   606,    -1,    -1,
     609,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,   568,
      -1,   135,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   592,   158,   173,   174,   175,    -1,    -1,
      -1,   179,   601,    -1,   603,    57,   605,   606,    -1,    -1,
     609,    -1,    -1,   191,    -1,   193,   194,    -1,    -1,   197,
      -1,   199,   200,   201,   202,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   211,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,    -1,    -1,    -1,    -1,    -1,
     228,    -1,   230,    -1,    -1,    -1,   234,    -1,   110,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    57,    -1,   247,
      -1,    -1,    -1,   125,   126,   127,    -1,    -1,    -1,    -1,
     132,    -1,   260,   135,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   347,
     348,     3,     4,     5,    -1,     7,    -1,    -1,    10,    -1,
      12,    13,    14,    15,    16,    17,    18,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,   109,   456,    57,
      -1,    -1,    -1,   461,   116,   117,   464,   465,    -1,   467,
     468,   469,   470,    -1,   472,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   482,    -1,    -1,   485,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   503,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,   533,    -1,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   110,
     111,   112,   113,   114,   115,    -1,   554,   555,    -1,    -1,
     158,    -1,   560,    -1,   125,   126,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   135,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,    -1,    -1,    -1,   587,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
      -1,   599,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
     608,    -1,     7,    -1,   612,    10,    -1,    12,    13,    14,
      15,    16,    17,    18,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
     155,   156,     3,     4,    -1,    -1,     7,    -1,    -1,    10,
      -1,    12,    13,    14,    15,    16,    17,    18,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,   155,   156,     3,     4,    -1,    -1,
       7,    -1,    -1,    10,    -1,    12,    13,    14,    15,    16,
      17,    18,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,   156,
       3,     4,    -1,    -1,     7,    -1,    -1,    10,    -1,    12,
      13,    14,    15,    16,    17,    18,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,    -1,    -1,     7,
      -1,   154,    10,   156,    12,    13,    14,    15,    16,    17,
      18,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    16,    17,    -1,   154,    -1,   156,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    -1,    -1,    -1,    99,   100,    -1,    -1,
     103,   104,   105,   106,    -1,   108,   109,    -1,    -1,    -1,
      -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,    -1,    -1,   147,   148,     5,    -1,    -1,   152,
      -1,   154,    -1,   156,   157,    -1,    -1,    16,    17,    -1,
      -1,    -1,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    -1,    -1,    -1,
      99,   100,    -1,    -1,   103,   104,   105,   106,    -1,   108,
     109,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,     5,    -1,    -1,   147,   148,
      -1,    -1,    -1,   152,    -1,   154,    16,    17,   157,    -1,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    -1,    -1,    -1,    99,
     100,    -1,    -1,   103,   104,   105,   106,    -1,   108,   109,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     5,    -1,   143,    -1,    -1,    -1,   147,   148,    -1,
      -1,    -1,    16,    17,    -1,    -1,    -1,   157,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    -1,    -1,    -1,    99,   100,    -1,    -1,   103,
     104,   105,   106,    -1,   108,   109,    -1,    -1,    -1,    -1,
      -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,    -1,   143,
      -1,    -1,    -1,   147,   148,    -1,    -1,    -1,    16,    17,
      -1,    -1,    -1,   157,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    -1,    -1,
      -1,    99,   100,    -1,    -1,   103,   104,   105,   106,    -1,
     108,   109,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     5,    -1,   143,    -1,    -1,    -1,   147,
     148,    -1,    -1,    -1,    16,    17,    -1,    -1,    -1,   157,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    -1,    -1,    -1,    99,   100,    -1,
      -1,   103,   104,   105,   106,    -1,   108,   109,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,
      -1,   143,    -1,    -1,    -1,   147,   148,    -1,    -1,    -1,
      16,    17,    -1,    -1,    -1,   157,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      -1,    -1,    -1,    99,   100,    -1,    -1,   103,   104,   105,
     106,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,
      -1,   147,   148,    -1,    15,    16,    17,    18,    -1,    20,
      -1,   157,    23,    24,    -1,    26,    -1,    28,    29,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,    17,
      -1,    -1,    -1,    -1,   155,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    -1,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    16,    17,    -1,    -1,   155,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    15,
      16,    17,    -1,   116,   117,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,    57,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,    -1,    -1,
      -1,    57,    -1,    -1,    -1,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   110,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,    -1,   125,   126,   127,    -1,    -1,    -1,    57,    -1,
      -1,    -1,   135,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   110,   111,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,   125,
     126,   127,    -1,    -1,    -1,    57,    -1,    -1,    -1,   135,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   110,   111,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,    -1,   125,   126,   127,    -1,
      -1,    -1,    57,    -1,    -1,    -1,   135,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   110,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,    -1,   125,   126,   127,    -1,    -1,    -1,    57,
      -1,    -1,    -1,   135,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   110,   111,   112,   113,   114,
     115,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,
     125,   126,   127,    -1,    -1,    -1,    57,    -1,    -1,    -1,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   110,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,    -1,   125,   126,   127,
      -1,    -1,    -1,    57,    -1,    -1,    -1,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   110,
     111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,    -1,   125,   126,   127,    -1,    -1,    -1,
      57,    -1,    -1,    -1,   135,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   110,   111,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
      -1,   125,   126,   127,    -1,    -1,    -1,    57,    -1,    -1,
      -1,   135,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   110,   111,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,   125,   126,
     127,    -1,    -1,    -1,    57,    -1,    -1,    -1,   135,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,    -1,   125,   126,   127,    -1,    -1,
      -1,    57,    -1,    -1,    -1,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   110,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,    -1,   125,   126,   127,    -1,    -1,    -1,    57,    -1,
      -1,    -1,   135,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   110,   111,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,   125,
     126,   127,    -1,    -1,    -1,    57,    -1,    -1,    -1,   135,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   110,   111,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,    -1,   125,   126,   127,    -1,
      -1,    -1,    57,    -1,    -1,    -1,   135,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   110,   111,
     112,   113,   114,   115,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,    -1,    -1,    -1,    57,
      -1,    -1,    -1,   135,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   110,   111,   112,   113,   114,
     115,   153,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   110,   111,   112,   113,   114,   115,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    16,
      17,    -1,    -1,   151,   152,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    16,    17,    -1,
      -1,    -1,   109,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    -1,    -1,    -1,
      99,   100,    -1,    -1,    16,    17,    -1,    -1,    -1,    -1,
     109,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    -1,    -1,    -1,    99,   100,    -1,
      -1,    16,    17,    -1,    -1,    -1,    -1,   109,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    57,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,    -1,    -1,    -1,    57,   132,    -1,    -1,   135,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   110,   111,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,    -1,
      -1,    -1,    57,   132,    -1,    -1,   135,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   110,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,    -1,    -1,    -1,    57,
     132,    -1,    -1,   135,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   110,   111,   112,   113,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,    -1,    -1,    -1,    57,   132,    -1,    -1,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   110,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
      -1,    -1,    -1,    57,   132,    -1,    -1,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   110,
     111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,    -1,    -1,    -1,
      57,   132,    -1,    -1,   135,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   110,   111,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,    -1,    -1,    -1,    57,   132,    -1,
      -1,   135,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   110,   111,   112,   113,   114,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,    -1,    -1,    -1,    57,   132,    -1,    -1,   135,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     110,   111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,    -1,    -1,
      -1,    57,    -1,    -1,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   110,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,    -1,    -1,    -1,    57,   132,
      -1,    -1,   135,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   110,   111,   112,   113,   114,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,    -1,    -1,    -1,    57,   132,    -1,    -1,   135,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   110,   111,   112,   113,   114,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,    -1,
      -1,    -1,    57,   132,    -1,    -1,   135,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   110,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,    -1,    -1,    -1,    57,
     132,    -1,    -1,   135,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   110,   111,   112,   113,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,    -1,    -1,    -1,    57,    -1,    -1,    -1,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   110,   111,   112,   113,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
      -1,    -1,    -1,    57,   132,    -1,    -1,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   110,
     111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,    -1,    -1,    -1,
      57,   132,    -1,    -1,   135,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   110,   111,   112,   113,
     114,   115,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   135,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,    -1,    -1,    -1,    -1,    -1,   111,   112,    -1,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     125,   126,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,    15,    16,    17,    18,    19,    20,    -1,    -1,
      23,    24,    -1,    26,    -1,    28,    29,    30,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    15,    16,
      17,    18,    -1,    20,    -1,    -1,    23,    24,    -1,    26,
      -1,    28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   160,   161,     0,     3,     4,     5,     7,    10,    12,
      13,    14,    15,    16,    17,    18,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   109,
     116,   117,   154,   156,   162,   163,   165,   166,   167,   168,
     170,   172,   173,   176,   178,   179,   181,   182,   185,   187,
     188,   191,   192,   195,   199,   200,   201,   207,   156,   156,
     157,   157,    17,    23,    24,    25,    26,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    56,
      78,    82,    83,    94,    95,    99,   100,   109,   176,   205,
     206,   109,   192,   195,   200,   201,    16,   103,   104,   105,
     106,   108,   109,   143,   147,   148,   152,   154,   156,   157,
     165,   178,   192,   194,   195,   199,   200,   201,   202,   203,
     204,   157,   154,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   155,   164,   166,   163,   157,   176,
     157,   166,   201,   151,   152,   118,   119,   120,   121,   122,
     123,   124,   128,   129,   130,   133,   134,   198,   199,   156,
      15,   169,   176,   186,   207,   202,   133,   132,   157,   151,
     152,   176,   104,   105,   202,   202,   202,   193,   203,   176,
     196,   197,   202,   157,   201,   133,   199,    57,   110,   111,
     112,   113,   114,   115,   125,   126,   127,   135,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   156,   202,
     208,   202,   202,   158,   203,   202,   203,   202,   202,   202,
     202,   108,   158,   158,   158,   158,   158,   158,   203,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   158,   104,
     108,   202,   108,   202,   193,   155,   166,   202,   157,   202,
      13,   176,   202,   203,   202,   176,   132,    11,   156,   158,
     203,   206,   193,   109,   202,   157,   132,   153,   136,   132,
     155,   158,   176,   177,   203,   200,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   202,   202,   202,   158,    15,    16,    17,
      18,    20,    23,    24,    26,    28,    29,    30,    31,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
     209,   210,   214,   158,   158,   158,   132,   158,   158,   158,
     158,   132,   132,   158,   132,   132,   158,   132,   132,   132,
     132,   158,   132,   158,   158,   158,   158,   158,   158,   158,
     177,   158,   157,   157,   153,    11,   207,   176,   180,   192,
     195,   202,   171,   202,   166,   158,   153,   193,   203,   203,
     196,   132,   158,   136,   154,   211,   215,   107,   108,   108,
     155,   214,   202,   202,   108,   202,   202,   202,   202,   104,
     190,   202,   190,   202,   154,   158,   166,   202,   193,   157,
     180,   158,   151,   152,   156,     6,   158,   176,   166,   202,
     164,   209,   103,   104,   105,   106,   107,   108,   212,   213,
     158,   132,   189,   158,   132,   132,   189,   189,   189,   189,
     132,   158,   174,   166,   158,   158,   193,   158,   184,   176,
     202,   186,   166,   155,    19,   132,    92,    93,   108,   158,
     202,   202,   158,   158,   158,   158,   202,     8,     9,   155,
     175,   158,   184,   166,   153,   158,   212,   158,   158,   132,
     202,   136,   166,   183,   202,   136,   164,   166,   132,   164,
     202,     3,   132,   156,   202,   158
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 152 "swf5compiler.y"
    { bf = newBuffer();
		bc = newBuffer();
	;}
    break;

  case 3:
#line 155 "swf5compiler.y"
    { Buffer b = newBuffer();
		  bufferWriteConstants(b);
		  bufferConcat(b, bf);
		  bufferConcat(b, bc);
		  *((Buffer *)buffer) = b; ;}
    break;

  case 4:
#line 160 "swf5compiler.y"
    { Buffer b = newBuffer(); *((Buffer *)buffer) = b; ;}
    break;

  case 7:
#line 170 "swf5compiler.y"
    { bufferConcat(bc, (yyvsp[(1) - (1)].action)); ;}
    break;

  case 8:
#line 172 "swf5compiler.y"
    { bufferConcat(bf, (yyvsp[(1) - (1)].action)); ;}
    break;

  case 9:
#line 177 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action); ;}
    break;

  case 10:
#line 180 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (2)].action);
		  bufferConcat((yyval.action), (yyvsp[(2) - (2)].action)); ;}
    break;

  case 11:
#line 185 "swf5compiler.y"
    { ;}
    break;

  case 12:
#line 189 "swf5compiler.y"
    { (yyval.action) = NULL; ;}
    break;

  case 13:
#line 190 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].action); ;}
    break;

  case 14:
#line 191 "swf5compiler.y"
    { (yyval.action) = NULL; ;}
    break;

  case 15:
#line 192 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (2)].action); ;}
    break;

  case 23:
#line 204 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferWriteOp((yyval.action), SWFACTION_WITH);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(6) - (7)].action)));
		  bufferConcat((yyval.action), (yyvsp[(6) - (7)].action)); ;}
    break;

  case 24:
#line 215 "swf5compiler.y"
    { int tmp = chkctx(CTX_FUNCTION);
		  if(tmp < 0)
			swf5error("return outside function");
		  (yyval.action) = newBuffer();
		  while(--tmp >= 0)
			bufferWriteOp((yyval.action), SWFACTION_POP);
		  bufferWriteNull((yyval.action));
		  bufferWriteOp((yyval.action), SWFACTION_RETURN); ;}
    break;

  case 25:
#line 225 "swf5compiler.y"
    { int tmp = chkctx(CTX_FUNCTION);
		  if(tmp < 0)
			swf5error("return outside function");
		  (yyval.action) = newBuffer();
		  while(--tmp >= 0)
			bufferWriteOp((yyval.action), SWFACTION_POP);
		  bufferConcat((yyval.action), (yyvsp[(2) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_RETURN); ;}
    break;

  case 27:
#line 237 "swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action)); ;}
    break;

  case 28:
#line 242 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferWriteOp((yyval.action), SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(7) - (7)].action))+5);
		  bufferConcat((yyval.action), (yyvsp[(7) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_BRANCHALWAYS);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(5) - (7)].action)));
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action)); ;}
    break;

  case 29:
#line 253 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (5)].action);
		  bufferWriteOp((yyval.action), SWFACTION_LOGICALNOT);
		  bufferWriteOp((yyval.action), SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(5) - (5)].action)));
		  bufferConcat((yyval.action), (yyvsp[(5) - (5)].action)); ;}
    break;

  case 30:
#line 262 "swf5compiler.y"
    { (yyval.action) = NULL; ;}
    break;

  case 31:
#line 263 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action); ;}
    break;

  case 32:
#line 268 "swf5compiler.y"
    { addctx(CTX_SWITCH); ;}
    break;

  case 33:
#line 274 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferResolveSwitch((yyval.action), &(yyvsp[(6) - (7)].switchcases));
		  bufferResolveJumps((yyval.action));
		  bufferWriteOp((yyval.action), SWFACTION_POP);
		  delctx(CTX_SWITCH);
 /* FIXME: continue in switch continues surrounding loop, if any */
	;}
    break;

  case 34:
#line 286 "swf5compiler.y"
    { (yyval.switchcases).count = 0;
		  (yyval.switchcases).list = 0; ;}
    break;

  case 35:
#line 290 "swf5compiler.y"
    { (yyval.switchcases) = (yyvsp[(1) - (2)].switchcases);
		  (yyval.switchcases).list = (struct switchcase*) realloc((yyval.switchcases).list, ((yyval.switchcases).count+1) * sizeof(struct switchcase));
		  (yyval.switchcases).list[(yyval.switchcases).count] = (yyvsp[(2) - (2)].switchcase);
		  (yyval.switchcases).count++; ;}
    break;

  case 36:
#line 298 "swf5compiler.y"
    { (yyval.switchcase).cond = (yyvsp[(2) - (6)].action);
		  (yyval.switchcase).action = (yyvsp[(4) - (6)].action);
		  (yyval.switchcase).isbreak = 1; ;}
    break;

  case 37:
#line 303 "swf5compiler.y"
    { (yyval.switchcase).cond = (yyvsp[(2) - (4)].action);
		  (yyval.switchcase).action = (yyvsp[(4) - (4)].action);
		  (yyval.switchcase).isbreak = 0; ;}
    break;

  case 38:
#line 308 "swf5compiler.y"
    { (yyval.switchcase).cond = NULL;
		  (yyval.switchcase).action = (yyvsp[(3) - (3)].action);
		  (yyval.switchcase).isbreak = 0; ;}
    break;

  case 40:
#line 318 "swf5compiler.y"
    { (yyval.str) = strdup("new"); ;}
    break;

  case 41:
#line 319 "swf5compiler.y"
    { (yyval.str) = strdup("delete"); ;}
    break;

  case 42:
#line 320 "swf5compiler.y"
    { (yyval.str) = strdup("random"); ;}
    break;

  case 43:
#line 321 "swf5compiler.y"
    { (yyval.str) = strdup("getTimer"); ;}
    break;

  case 44:
#line 322 "swf5compiler.y"
    { (yyval.str) = strdup("length"); ;}
    break;

  case 45:
#line 323 "swf5compiler.y"
    { (yyval.str) = strdup("concat"); ;}
    break;

  case 46:
#line 324 "swf5compiler.y"
    { (yyval.str) = strdup("substr"); ;}
    break;

  case 47:
#line 325 "swf5compiler.y"
    { (yyval.str) = strdup("trace"); ;}
    break;

  case 48:
#line 326 "swf5compiler.y"
    { (yyval.str) = strdup("int"); ;}
    break;

  case 49:
#line 327 "swf5compiler.y"
    { (yyval.str) = strdup("ord"); ;}
    break;

  case 50:
#line 328 "swf5compiler.y"
    { (yyval.str) = strdup("chr"); ;}
    break;

  case 51:
#line 329 "swf5compiler.y"
    { (yyval.str) = strdup("getURL"); ;}
    break;

  case 52:
#line 330 "swf5compiler.y"
    { (yyval.str) = strdup("getURL1"); ;}
    break;

  case 53:
#line 331 "swf5compiler.y"
    { (yyval.str) = strdup("nextFrame"); ;}
    break;

  case 54:
#line 332 "swf5compiler.y"
    { (yyval.str) = strdup("prevFrame"); ;}
    break;

  case 55:
#line 333 "swf5compiler.y"
    { (yyval.str) = strdup("play"); ;}
    break;

  case 56:
#line 334 "swf5compiler.y"
    { (yyval.str) = strdup("stop"); ;}
    break;

  case 57:
#line 335 "swf5compiler.y"
    { (yyval.str) = strdup("toggleQuality"); ;}
    break;

  case 58:
#line 336 "swf5compiler.y"
    { (yyval.str) = strdup("stopSounds"); ;}
    break;

  case 59:
#line 337 "swf5compiler.y"
    { (yyval.str) = strdup("dup"); ;}
    break;

  case 60:
#line 338 "swf5compiler.y"
    { (yyval.str) = strdup("swap"); ;}
    break;

  case 61:
#line 339 "swf5compiler.y"
    { (yyval.str) = strdup("pop"); ;}
    break;

  case 62:
#line 340 "swf5compiler.y"
    { (yyval.str) = strdup("push"); ;}
    break;

  case 63:
#line 341 "swf5compiler.y"
    { (yyval.str) = strdup("setRegister"); ;}
    break;

  case 64:
#line 342 "swf5compiler.y"
    { (yyval.str) = strdup("callFunction"); ;}
    break;

  case 65:
#line 343 "swf5compiler.y"
    { (yyval.str) = strdup("callMethod"); ;}
    break;

  case 66:
#line 344 "swf5compiler.y"
    { (yyval.str) = strdup("and"); ;}
    break;

  case 67:
#line 345 "swf5compiler.y"
    { (yyval.str) = strdup("or"); ;}
    break;

  case 68:
#line 346 "swf5compiler.y"
    { (yyval.str) = strdup("xor"); ;}
    break;

  case 69:
#line 347 "swf5compiler.y"
    { (yyval.str) = strdup("modulo"); ;}
    break;

  case 70:
#line 348 "swf5compiler.y"
    { (yyval.str) = strdup("add"); ;}
    break;

  case 71:
#line 349 "swf5compiler.y"
    { (yyval.str) = strdup("lessThan"); ;}
    break;

  case 72:
#line 350 "swf5compiler.y"
    { (yyval.str) = strdup("equals"); ;}
    break;

  case 73:
#line 351 "swf5compiler.y"
    { (yyval.str) = strdup("inc"); ;}
    break;

  case 74:
#line 352 "swf5compiler.y"
    { (yyval.str) = strdup("dec"); ;}
    break;

  case 75:
#line 353 "swf5compiler.y"
    { (yyval.str) = strdup("typeof"); ;}
    break;

  case 76:
#line 354 "swf5compiler.y"
    { (yyval.str) = strdup("instanceof"); ;}
    break;

  case 77:
#line 355 "swf5compiler.y"
    { (yyval.str) = strdup("enumerate"); ;}
    break;

  case 78:
#line 356 "swf5compiler.y"
    { (yyval.str) = strdup("initobject"); ;}
    break;

  case 79:
#line 357 "swf5compiler.y"
    { (yyval.str) = strdup("initarray"); ;}
    break;

  case 80:
#line 358 "swf5compiler.y"
    { (yyval.str) = strdup("getmember"); ;}
    break;

  case 81:
#line 359 "swf5compiler.y"
    { (yyval.str) = strdup("setmember"); ;}
    break;

  case 82:
#line 360 "swf5compiler.y"
    { (yyval.str) = strdup("shiftleft"); ;}
    break;

  case 83:
#line 361 "swf5compiler.y"
    { (yyval.str) = strdup("shiftright"); ;}
    break;

  case 84:
#line 362 "swf5compiler.y"
    { (yyval.str) = strdup("shiftright2"); ;}
    break;

  case 85:
#line 363 "swf5compiler.y"
    { (yyval.str) = strdup("varequals"); ;}
    break;

  case 86:
#line 364 "swf5compiler.y"
    { (yyval.str) = strdup("oldAdd"); ;}
    break;

  case 87:
#line 365 "swf5compiler.y"
    { (yyval.str) = strdup("subtract"); ;}
    break;

  case 88:
#line 366 "swf5compiler.y"
    { (yyval.str) = strdup("multiply"); ;}
    break;

  case 89:
#line 367 "swf5compiler.y"
    { (yyval.str) = strdup("divide"); ;}
    break;

  case 90:
#line 368 "swf5compiler.y"
    { (yyval.str) = strdup("oldequals"); ;}
    break;

  case 91:
#line 369 "swf5compiler.y"
    { (yyval.str) = strdup("oldlessthan"); ;}
    break;

  case 92:
#line 370 "swf5compiler.y"
    { (yyval.str) = strdup("logicaland"); ;}
    break;

  case 93:
#line 371 "swf5compiler.y"
    { (yyval.str) = strdup("logicalor"); ;}
    break;

  case 94:
#line 372 "swf5compiler.y"
    { (yyval.str) = strdup("not"); ;}
    break;

  case 95:
#line 373 "swf5compiler.y"
    { (yyval.str) = strdup("stringeq"); ;}
    break;

  case 96:
#line 374 "swf5compiler.y"
    { (yyval.str) = strdup("stringlength"); ;}
    break;

  case 97:
#line 375 "swf5compiler.y"
    { (yyval.str) = strdup("substring"); ;}
    break;

  case 98:
#line 376 "swf5compiler.y"
    { (yyval.str) = strdup("getvariable"); ;}
    break;

  case 99:
#line 377 "swf5compiler.y"
    { (yyval.str) = strdup("setvariable"); ;}
    break;

  case 100:
#line 378 "swf5compiler.y"
    { (yyval.str) = strdup("settargetexpression"); ;}
    break;

  case 101:
#line 379 "swf5compiler.y"
    { (yyval.str) = strdup("duplicatemovieclip"); ;}
    break;

  case 102:
#line 380 "swf5compiler.y"
    { (yyval.str) = strdup("removemovieclip"); ;}
    break;

  case 103:
#line 381 "swf5compiler.y"
    { (yyval.str) = strdup("startdrag"); ;}
    break;

  case 104:
#line 382 "swf5compiler.y"
    { (yyval.str) = strdup("stopdrag"); ;}
    break;

  case 105:
#line 383 "swf5compiler.y"
    { (yyval.str) = strdup("stringlessthan"); ;}
    break;

  case 106:
#line 384 "swf5compiler.y"
    { (yyval.str) = strdup("mblength"); ;}
    break;

  case 107:
#line 385 "swf5compiler.y"
    { (yyval.str) = strdup("mbsubstring"); ;}
    break;

  case 108:
#line 386 "swf5compiler.y"
    { (yyval.str) = strdup("mbord"); ;}
    break;

  case 109:
#line 387 "swf5compiler.y"
    { (yyval.str) = strdup("mbchr"); ;}
    break;

  case 110:
#line 388 "swf5compiler.y"
    { (yyval.str) = strdup("branchalways"); ;}
    break;

  case 111:
#line 389 "swf5compiler.y"
    { (yyval.str) = strdup("branchiftrue"); ;}
    break;

  case 112:
#line 390 "swf5compiler.y"
    { (yyval.str) = strdup("getURL2"); ;}
    break;

  case 113:
#line 391 "swf5compiler.y"
    { (yyval.str) = strdup("post"); ;}
    break;

  case 114:
#line 392 "swf5compiler.y"
    { (yyval.str) = strdup("get"); ;}
    break;

  case 115:
#line 393 "swf5compiler.y"
    { (yyval.str) = strdup("loadvariables"); ;}
    break;

  case 116:
#line 394 "swf5compiler.y"
    { (yyval.str) = strdup("loadMovie"); ;}
    break;

  case 117:
#line 399 "swf5compiler.y"
    { (yyval.exprlist).buffer = newBuffer();
		  (yyval.exprlist).count = 0; ;}
    break;

  case 118:
#line 403 "swf5compiler.y"
    { (yyval.exprlist).buffer = newBuffer();
		  bufferWriteHardString((yyval.exprlist).buffer, (byte*)(yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  (yyval.exprlist).count = 1; ;}
    break;

  case 119:
#line 408 "swf5compiler.y"
    { (yyval.exprlist) = (yyvsp[(1) - (3)].exprlist);
		  bufferWriteHardString((yyval.exprlist).buffer, (byte*)(yyvsp[(3) - (3)].str), strlen((yyvsp[(3) - (3)].str))+1);
		  ++(yyval.exprlist).count; ;}
    break;

  case 120:
#line 415 "swf5compiler.y"
    { addctx(CTX_FUNCTION); ;}
    break;

  case 121:
#line 420 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_DEFINEFUNCTION);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(2) - (6)].str)) +
				     bufferLength((yyvsp[(4) - (6)].exprlist).buffer) + 5);
		  bufferWriteHardString((yyval.action), (byte*) (yyvsp[(2) - (6)].str), strlen((yyvsp[(2) - (6)].str))+1);
		  bufferWriteS16((yyval.action), (yyvsp[(4) - (6)].exprlist).count);
		  bufferConcat((yyval.action), (yyvsp[(4) - (6)].exprlist).buffer);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(6) - (6)].action)));
		  bufferConcat((yyval.action), (yyvsp[(6) - (6)].action));
		  delctx(CTX_FUNCTION); ;}
    break;

  case 122:
#line 434 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  free((yyvsp[(1) - (1)].str)); ;}
    break;

  case 123:
#line 439 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferWriteString((yyval.action), (yyvsp[(3) - (3)].str), strlen((yyvsp[(3) - (3)].str))+1);
		  bufferWriteOp((yyval.action), SWFACTION_GETMEMBER);
		  free((yyvsp[(3) - (3)].str)); ;}
    break;

  case 124:
#line 445 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (4)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (4)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); ;}
    break;

  case 127:
#line 456 "swf5compiler.y"
    { addctx(CTX_LOOP); ;}
    break;

  case 128:
#line 461 "swf5compiler.y"
    { addctx(CTX_LOOP); ;}
    break;

  case 129:
#line 466 "swf5compiler.y"
    { addctx(CTX_LOOP); ;}
    break;

  case 130:
#line 471 "swf5compiler.y"
    { addctx(CTX_FOR_IN); ;}
    break;

  case 131:
#line 476 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (5)].action);
		  bufferWriteOp((yyval.action), SWFACTION_LOGICALNOT);
		  bufferWriteOp((yyval.action), SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(5) - (5)].action))+5);
		  bufferConcat((yyval.action), (yyvsp[(5) - (5)].action));
		  bufferWriteOp((yyval.action), SWFACTION_BRANCHALWAYS);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), -(bufferLength((yyval.action))+2));
		  bufferResolveJumps((yyval.action));
		  delctx(CTX_LOOP); ;}
    break;

  case 132:
#line 489 "swf5compiler.y"
    { if((yyvsp[(2) - (6)].action))
			{	(yyval.action) = (yyvsp[(2) - (6)].action);
		  		bufferConcat((yyval.action), (yyvsp[(5) - (6)].action));
			}
			else
				(yyval.action) = (yyvsp[(5) - (6)].action);
		  bufferWriteOp((yyval.action), SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), -(bufferLength((yyval.action))+2));
		  bufferResolveJumps((yyval.action));
		  delctx(CTX_LOOP); ;}
    break;

  case 133:
#line 502 "swf5compiler.y"
    {
		  if((yyvsp[(3) - (10)].action))
		    (yyval.action) = (yyvsp[(3) - (10)].action);
		  else
		    (yyval.action) = newBuffer();

		  if((yyvsp[(7) - (10)].action))
		  {
                    bufferWriteOp((yyval.action), SWFACTION_BRANCHALWAYS);
                    bufferWriteS16((yyval.action), 2);
                    bufferWriteS16((yyval.action), bufferLength((yyvsp[(7) - (10)].action)));
		  }
		  else
		    (yyvsp[(7) - (10)].action) = newBuffer();

		  if((yyvsp[(5) - (10)].action))
		  {
		    bufferConcat((yyvsp[(7) - (10)].action), (yyvsp[(5) - (10)].action));
                    bufferWriteOp((yyvsp[(7) - (10)].action), SWFACTION_LOGICALNOT);
                    bufferWriteOp((yyvsp[(7) - (10)].action), SWFACTION_BRANCHIFTRUE);
                    bufferWriteS16((yyvsp[(7) - (10)].action), 2);
                    bufferWriteS16((yyvsp[(7) - (10)].action), bufferLength((yyvsp[(10) - (10)].action))+5);
                  }

                  bufferConcat((yyvsp[(7) - (10)].action), (yyvsp[(10) - (10)].action));
                  bufferWriteOp((yyvsp[(7) - (10)].action), SWFACTION_BRANCHALWAYS);
                  bufferWriteS16((yyvsp[(7) - (10)].action), 2);
                  bufferWriteS16((yyvsp[(7) - (10)].action), -(bufferLength((yyvsp[(7) - (10)].action))+2));
                  bufferResolveJumps((yyvsp[(7) - (10)].action));

                  bufferConcat((yyval.action), (yyvsp[(7) - (10)].action));
				  delctx(CTX_LOOP);
                ;}
    break;

  case 134:
#line 537 "swf5compiler.y"
    { Buffer b2, b3;
		  int tmp;

		  (yyval.action) = (yyvsp[(5) - (8)].action);
		  bufferWriteOp((yyval.action), SWFACTION_ENUMERATE);	

		  b2 = newBuffer();
		  bufferWriteSetRegister(b2, 0);
		  bufferWriteOp(b2, SWFACTION_PUSHDATA);
		  bufferWriteS16(b2, 1);
		  bufferWriteU8(b2, 2);
		  bufferWriteOp(b2, SWFACTION_NEWEQUALS);
		  bufferWriteOp(b2, SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16(b2, 2);

		  b3 = newBuffer();
/* basically a lvalue could be used here rather than an ident !!! */
/* probably by using reg1 for the test rather than reg0 */
		  bufferWriteString(b3, (yyvsp[(3) - (8)].str), strlen((yyvsp[(3) - (8)].str))+1);
		  bufferWriteRegister(b3, 0);
		  bufferWriteOp(b3, SWFACTION_SETVARIABLE);
		  bufferConcat(b3, (yyvsp[(8) - (8)].action));
		  bufferWriteS16(b2, bufferLength(b3) + 5);
		  tmp = bufferLength(b2) + bufferLength(b3) + 5;
		  bufferConcat((yyval.action), b2);
		  bufferWriteOp(b3, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(b3, 2);
		  bufferWriteS16(b3, -tmp);
		  bufferResolveJumps(b3);
		  bufferConcat((yyval.action), b3);
		  delctx(CTX_FOR_IN); ;}
    break;

  case 135:
#line 570 "swf5compiler.y"
    { Buffer b2, b3;
		  int tmp;

		  (yyval.action) = (yyvsp[(6) - (9)].action);
		  bufferWriteOp((yyval.action), SWFACTION_ENUMERATE);	

		  b2 = newBuffer();
		  bufferWriteSetRegister(b2, 0);
		  bufferWriteOp(b2, SWFACTION_PUSHDATA);
		  bufferWriteS16(b2, 1);
		  bufferWriteU8(b2, 2);
		  bufferWriteOp(b2, SWFACTION_NEWEQUALS);
		  bufferWriteOp(b2, SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16(b2, 2);
		  // add size later

		  b3 = newBuffer();
		  bufferWriteString(b3, (yyvsp[(4) - (9)].str), strlen((yyvsp[(4) - (9)].str))+1);
		  bufferWriteRegister(b3, 0);
		  bufferWriteOp(b3, SWFACTION_VAREQUALS);
		  bufferConcat(b3, (yyvsp[(9) - (9)].action));
		  bufferWriteS16(b2, bufferLength(b3) + 5);
		  tmp = bufferLength(b2) + bufferLength(b3) + 5;
		  bufferConcat((yyval.action), b2);
		  bufferWriteOp(b3, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(b3, 2);
		  bufferWriteS16(b3, -tmp);
		  bufferResolveJumps(b3);
		  bufferConcat((yyval.action), b3);
		  delctx(CTX_FOR_IN); ;}
    break;

  case 136:
#line 603 "swf5compiler.y"
    { (yyval.action) = NULL; ;}
    break;

  case 138:
#line 611 "swf5compiler.y"
    { if(chkctx(CTX_CONTINUE) < 0)
			swf5error("continue outside loop");
		  (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_BRANCHALWAYS);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), MAGIC_CONTINUE_NUMBER); ;}
    break;

  case 139:
#line 622 "swf5compiler.y"
    { int tmp = chkctx(CTX_BREAK);
		  if(tmp < 0)
			swf5error("break outside switch / loop");
		  (yyval.action) = newBuffer();
		  if(tmp)	/* break out of a for .. in */
			bufferWriteOp((yyval.action), SWFACTION_POP);
		  bufferWriteOp((yyval.action), SWFACTION_BRANCHALWAYS);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), MAGIC_BREAK_NUMBER); ;}
    break;

  case 140:
#line 634 "swf5compiler.y"
    { (yyval.getURLMethod) = GETURL_METHOD_NOSEND; ;}
    break;

  case 141:
#line 636 "swf5compiler.y"
    { (yyval.getURLMethod) = GETURL_METHOD_GET; ;}
    break;

  case 142:
#line 638 "swf5compiler.y"
    { (yyval.getURLMethod) = GETURL_METHOD_POST; ;}
    break;

  case 143:
#line 640 "swf5compiler.y"
    { if(strcmp((yyvsp[(2) - (2)].str), "GET") == 0)
				    (yyval.getURLMethod) = GETURL_METHOD_GET;
				  else if(strcmp((yyvsp[(2) - (2)].str), "POST") == 0)
				    (yyval.getURLMethod) = GETURL_METHOD_POST; ;}
    break;

  case 144:
#line 648 "swf5compiler.y"
    { char *lvlstring = (char*) malloc(12*sizeof(char));
		  sprintf(lvlstring, "_level%d", (yyvsp[(1) - (1)].intVal));
		  (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), lvlstring, strlen(lvlstring)+1);
		  free(lvlstring); ;}
    break;

  case 145:
#line 655 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), "_level", 7);
		  bufferConcat((yyval.action), (yyvsp[(1) - (1)].action));
		  bufferWriteOp((yyval.action), SWFACTION_STRINGCONCAT); ;}
    break;

  case 146:
#line 663 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(3) - (4)].exprlist).count);
		  bufferWriteString((yyval.action), (yyvsp[(1) - (4)].str), strlen((yyvsp[(1) - (4)].str))+1);
		  bufferWriteOp((yyval.action), SWFACTION_CALLFUNCTION);
		  bufferWriteOp((yyval.action), SWFACTION_POP);
		  free((yyvsp[(1) - (4)].str)); ;}
    break;

  case 147:
#line 671 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(2) - (2)].str), strlen((yyvsp[(2) - (2)].str))+1);
		  free((yyvsp[(2) - (2)].str));
		  bufferWriteOp((yyval.action), SWFACTION_DELETE); ;}
    break;

  case 148:
#line 677 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (4)].action);
		  // bufferWriteOp($$, SWFACTION_GETVARIABLE);
		  bufferWriteString((yyval.action), (yyvsp[(4) - (4)].str), strlen((yyvsp[(4) - (4)].str))+1);
		  free((yyvsp[(4) - (4)].str));
		  bufferWriteOp((yyval.action), SWFACTION_DELETEVAR); ;}
    break;

  case 149:
#line 684 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (5)].action);
		  // bufferWriteOp($$, SWFACTION_GETVARIABLE);
		  bufferConcat((yyval.action), (yyvsp[(4) - (5)].action));
		  // bufferWriteOp($$, SWFACTION_GETVARIABLE);
		  bufferWriteOp((yyval.action), SWFACTION_DELETEVAR); ;}
    break;

  case 150:
#line 691 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_TRACE); ;}
    break;

  case 151:
#line 695 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), (yyvsp[(6) - (7)].getURLMethod)); ;}
    break;

  case 152:
#line 702 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0xc0+(yyvsp[(6) - (7)].getURLMethod)); ;}
    break;

  case 153:
#line 709 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0x80+(yyvsp[(6) - (7)].getURLMethod)); ;}
    break;

  case 154:
#line 716 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0x40+(yyvsp[(6) - (7)].getURLMethod)); ;}
    break;

  case 155:
#line 723 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (7)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_GETURL2);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), (yyvsp[(6) - (7)].getURLMethod)); ;}
    break;

  case 156:
#line 730 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_CALLFRAME);
		  bufferWriteS16((yyval.action), 0); ;}
    break;

  case 157:
#line 736 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), "0", 2); /* no constraint */
		  bufferConcat((yyval.action), (yyvsp[(5) - (6)].action));
		  bufferConcat((yyval.action), (yyvsp[(3) - (6)].action));
		  bufferWriteOp((yyval.action), SWFACTION_STARTDRAGMOVIE); ;}
    break;

  case 158:
#line 743 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferConcat((yyval.action), (yyvsp[(7) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(11) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(9) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(13) - (14)].action));
		  bufferWriteString((yyval.action), "1", 2); /* has constraint */
		  bufferConcat((yyval.action), (yyvsp[(5) - (14)].action));
		  bufferConcat((yyval.action), (yyvsp[(3) - (14)].action));
		  bufferWriteOp((yyval.action), SWFACTION_STARTDRAGMOVIE); ;}
    break;

  case 159:
#line 754 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_STOPDRAGMOVIE); ;}
    break;

  case 160:
#line 759 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (8)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (8)].action));
		  bufferConcat((yyval.action), (yyvsp[(7) - (8)].action));
		  bufferWriteInt((yyval.action), 16384); /* magic number */
		  bufferWriteOp((yyval.action), SWFACTION_ADD);
		  bufferWriteOp((yyval.action), SWFACTION_DUPLICATECLIP); ;}
    break;

  case 161:
#line 767 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_REMOVECLIP); ;}
    break;

  case 162:
#line 771 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GETURL);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (6)].str)) + strlen((yyvsp[(5) - (6)].str)) + 2);
		  bufferWriteHardString((yyval.action), (byte*)(yyvsp[(3) - (6)].str), strlen((yyvsp[(3) - (6)].str)));
		  bufferWriteU8((yyval.action), 0);
		  bufferWriteHardString((yyval.action), (byte*)(yyvsp[(5) - (6)].str), strlen((yyvsp[(5) - (6)].str)));
		  bufferWriteU8((yyval.action), 0); ;}
    break;

  case 163:
#line 781 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_NEXTFRAME); ;}
    break;

  case 164:
#line 785 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_PREVFRAME); ;}
    break;

  case 165:
#line 789 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_PLAY); ;}
    break;

  case 166:
#line 793 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_STOP); ;}
    break;

  case 167:
#line 797 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_STOPSOUNDS); ;}
    break;

  case 168:
#line 801 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_TOGGLEQUALITY); ;}
    break;

  case 169:
#line 805 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GOTOFRAME);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), (yyvsp[(3) - (4)].intVal)); ;}
    break;

  case 170:
#line 811 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GOTOLABEL);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (4)].str))+1);
		  bufferWriteHardString((yyval.action), (byte*)(yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  free((yyvsp[(3) - (4)].str)); ;}
    break;

  case 171:
#line 818 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_GOTOEXPRESSION);
		  bufferWriteS16((yyval.action), 1);
		  bufferWriteU8((yyval.action), 0); ;}
    break;

  case 172:
#line 824 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_SETTARGET);
		  bufferWriteS16((yyval.action), strlen((yyvsp[(3) - (4)].str))+1);
		  bufferWriteHardString((yyval.action), (byte*)(yyvsp[(3) - (4)].str), strlen((yyvsp[(3) - (4)].str))+1);
		  free((yyvsp[(3) - (4)].str)); ;}
    break;

  case 173:
#line 831 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_SETTARGETEXPRESSION); ;}
    break;

  case 174:
#line 840 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(3) - (4)].exprlist).count);
		  bufferWriteString((yyval.action), (yyvsp[(1) - (4)].str), strlen((yyvsp[(1) - (4)].str))+1);
		  bufferWriteOp((yyval.action), SWFACTION_CALLFUNCTION);
		  free((yyvsp[(1) - (4)].str)); ;}
    break;

  case 175:
#line 847 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE); ;}
    break;

  case 176:
#line 851 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_GETTIMER); ;}
    break;

  case 177:
#line 855 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_RANDOM); ;}
    break;

  case 178:
#line 859 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_STRINGLENGTH); ;}
    break;

  case 179:
#line 863 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_INT); ;}
    break;

  case 180:
#line 867 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_ORD); ;}
    break;

  case 181:
#line 871 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_CHR); ;}
    break;

  case 182:
#line 875 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (6)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (6)].action));
		  bufferWriteOp((yyval.action), SWFACTION_STRINGCONCAT); ;}
    break;

  case 183:
#line 880 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (8)].action);
		  bufferConcat((yyval.action), (yyvsp[(5) - (8)].action));
		  bufferConcat((yyval.action), (yyvsp[(7) - (8)].action));
		  bufferWriteOp((yyval.action), SWFACTION_SUBSTRING); ;}
    break;

  case 184:
#line 886 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (4)].action);
		  bufferWriteOp((yyval.action), SWFACTION_TYPEOF); ;}
    break;

  case 185:
#line 894 "swf5compiler.y"
    { (yyval.exprlist).buffer = newBuffer();
		  (yyval.exprlist).count = 0; ;}
    break;

  case 186:
#line 898 "swf5compiler.y"
    { (yyval.exprlist).buffer = (yyvsp[(1) - (1)].action);
		  (yyval.exprlist).count = 1; ;}
    break;

  case 187:
#line 903 "swf5compiler.y"
    { Buffer tmp = newBuffer();
		  bufferConcat(tmp, (yyvsp[(3) - (3)].action));
		  bufferConcat(tmp, (yyval.exprlist).buffer);
		  (yyval.exprlist).buffer = tmp;
		  ++(yyval.exprlist).count;  ;}
    break;

  case 188:
#line 912 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteOp((yyval.action), SWFACTION_DEFINEFUNCTION);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(3) - (5)].exprlist).buffer) + 5);
		  bufferWriteU8((yyval.action), 0); /* empty function name */
		  bufferWriteS16((yyval.action), (yyvsp[(3) - (5)].exprlist).count);
		  bufferConcat((yyval.action), (yyvsp[(3) - (5)].exprlist).buffer);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(5) - (5)].action)));
		  bufferConcat((yyval.action), (yyvsp[(5) - (5)].action));
		  delctx(CTX_FUNCTION); ;}
    break;

  case 189:
#line 925 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(5) - (6)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(5) - (6)].exprlist).count);
		  bufferConcat((yyval.action), (yyvsp[(1) - (6)].action));
		  bufferWriteString((yyval.action), (yyvsp[(3) - (6)].str), strlen((yyvsp[(3) - (6)].str))+1);
		  bufferWriteOp((yyval.action), SWFACTION_CALLMETHOD);
		  free((yyvsp[(3) - (6)].str)); ;}
    break;

  case 190:
#line 933 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(6) - (7)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(6) - (7)].exprlist).count);
		  bufferConcat((yyval.action), (yyvsp[(1) - (7)].action));
		  bufferConcat((yyval.action), (yyvsp[(3) - (7)].action));
		  bufferWriteOp((yyval.action), SWFACTION_CALLMETHOD); ;}
    break;

  case 191:
#line 942 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (3)].str), strlen((yyvsp[(1) - (3)].str))+1);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action)); ;}
    break;

  case 192:
#line 949 "swf5compiler.y"
    { (yyval.exprlist).buffer = (yyvsp[(1) - (1)].action);
		  (yyval.exprlist).count = 1; ;}
    break;

  case 193:
#line 953 "swf5compiler.y"
    { bufferConcat((yyval.exprlist).buffer, (yyvsp[(3) - (3)].action));
		  ++(yyval.exprlist).count;  ;}
    break;

  case 194:
#line 958 "swf5compiler.y"
    { (yyval.op) = SWFACTION_NEWADD; ;}
    break;

  case 195:
#line 959 "swf5compiler.y"
    { (yyval.op) = SWFACTION_SUBTRACT; ;}
    break;

  case 196:
#line 960 "swf5compiler.y"
    { (yyval.op) = SWFACTION_MULTIPLY; ;}
    break;

  case 197:
#line 961 "swf5compiler.y"
    { (yyval.op) = SWFACTION_DIVIDE; ;}
    break;

  case 198:
#line 962 "swf5compiler.y"
    { (yyval.op) = SWFACTION_MODULO; ;}
    break;

  case 199:
#line 963 "swf5compiler.y"
    { (yyval.op) = SWFACTION_BITWISEAND; ;}
    break;

  case 200:
#line 964 "swf5compiler.y"
    { (yyval.op) = SWFACTION_BITWISEOR; ;}
    break;

  case 201:
#line 965 "swf5compiler.y"
    { (yyval.op) = SWFACTION_BITWISEXOR; ;}
    break;

  case 202:
#line 966 "swf5compiler.y"
    { (yyval.op) = SWFACTION_SHIFTLEFT; ;}
    break;

  case 203:
#line 967 "swf5compiler.y"
    { (yyval.op) = SWFACTION_SHIFTRIGHT; ;}
    break;

  case 204:
#line 968 "swf5compiler.y"
    { (yyval.op) = SWFACTION_SHIFTRIGHT2; ;}
    break;

  case 205:
#line 972 "swf5compiler.y"
    { (yyval.op) = SWFACTION_INCREMENT; ;}
    break;

  case 206:
#line 973 "swf5compiler.y"
    { (yyval.op) = SWFACTION_DECREMENT; ;}
    break;

  case 207:
#line 992 "swf5compiler.y"
    { if((yyvsp[(1) - (1)].lval).obj)
		  {
		    (yyval.action) = (yyvsp[(1) - (1)].lval).obj;

		    if((yyvsp[(1) - (1)].lval).ident)
		      bufferConcat((yyval.action), (yyvsp[(1) - (1)].lval).ident);
		    else
		      bufferConcat((yyval.action), (yyvsp[(1) - (1)].lval).memexpr);

		    bufferWriteOp((yyval.action), SWFACTION_GETMEMBER);
		  }
		  else
		  {
		    (yyval.action) = (yyvsp[(1) - (1)].lval).ident;
		    bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE);
		  }
		;}
    break;

  case 210:
#line 1016 "swf5compiler.y"
    { (yyval.lval).ident = newBuffer();
		  bufferWriteString((yyval.lval).ident, (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  free((yyvsp[(1) - (1)].str));
		  (yyval.lval).obj = 0;
		  (yyval.lval).memexpr = 0; ;}
    break;

  case 211:
#line 1023 "swf5compiler.y"
    { (yyval.lval).obj = (yyvsp[(1) - (3)].action);
		  (yyval.lval).ident = newBuffer();
		  bufferWriteString((yyval.lval).ident, (yyvsp[(3) - (3)].str), strlen((yyvsp[(3) - (3)].str))+1);
		  (yyval.lval).memexpr = 0; ;}
    break;

  case 212:
#line 1029 "swf5compiler.y"
    { (yyval.lval).obj = (yyvsp[(1) - (4)].action);
		  (yyval.lval).memexpr = (yyvsp[(3) - (4)].action);
		  (yyval.lval).ident = 0; ;}
    break;

  case 214:
#line 1040 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteInt((yyvsp[(2) - (2)].action), -1);
		  bufferWriteOp((yyvsp[(2) - (2)].action), SWFACTION_MULTIPLY); ;}
    break;

  case 215:
#line 1045 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteInt((yyvsp[(2) - (2)].action), 0xffffffff);
		  bufferWriteOp((yyvsp[(2) - (2)].action), SWFACTION_BITWISEXOR); ;}
    break;

  case 216:
#line 1050 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action);
		  bufferWriteOp((yyvsp[(2) - (2)].action), SWFACTION_LOGICALNOT); ;}
    break;

  case 217:
#line 1054 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferWriteOp((yyval.action), SWFACTION_DUP);
		  bufferWriteOp((yyval.action), SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(3) - (3)].action))+1);
		  bufferWriteOp((yyval.action), SWFACTION_POP);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action)); ;}
    break;

  case 218:
#line 1063 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferWriteOp((yyval.action), SWFACTION_DUP);
		  bufferWriteOp((yyval.action), SWFACTION_LOGICALNOT);
		  bufferWriteOp((yyval.action), SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16((yyval.action), 2);
		  bufferWriteS16((yyval.action), bufferLength((yyvsp[(3) - (3)].action))+1);
		  bufferWriteOp((yyval.action), SWFACTION_POP);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action)); ;}
    break;

  case 219:
#line 1073 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_MULTIPLY); ;}
    break;

  case 220:
#line 1078 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_DIVIDE); ;}
    break;

  case 221:
#line 1083 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_MODULO); ;}
    break;

  case 222:
#line 1088 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_NEWADD); ;}
    break;

  case 223:
#line 1093 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_SUBTRACT); ;}
    break;

  case 224:
#line 1098 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_BITWISEAND); ;}
    break;

  case 225:
#line 1103 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_BITWISEOR); ;}
    break;

  case 226:
#line 1108 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_BITWISEXOR); ;}
    break;

  case 227:
#line 1113 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_NEWLESSTHAN); ;}
    break;

  case 228:
#line 1118 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(1) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_NEWLESSTHAN); ;}
    break;

  case 229:
#line 1123 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(3) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(1) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_NEWLESSTHAN);
		  bufferWriteOp((yyval.action), SWFACTION_LOGICALNOT); ;}
    break;

  case 230:
#line 1129 "swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_NEWLESSTHAN);
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_LOGICALNOT); ;}
    break;

  case 231:
#line 1134 "swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_NEWEQUALS); ;}
    break;

  case 232:
#line 1138 "swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_NEWEQUALS);
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_LOGICALNOT); ;}
    break;

  case 233:
#line 1143 "swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_SHIFTLEFT); ;}
    break;

  case 234:
#line 1147 "swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_SHIFTRIGHT); ;}
    break;

  case 235:
#line 1151 "swf5compiler.y"
    { bufferConcat((yyvsp[(1) - (3)].action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyvsp[(1) - (3)].action), SWFACTION_SHIFTRIGHT2); ;}
    break;

  case 236:
#line 1155 "swf5compiler.y"
    { bufferWriteOp((yyvsp[(1) - (5)].action), SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16((yyvsp[(1) - (5)].action), 2);
		  bufferWriteS16((yyvsp[(1) - (5)].action), bufferLength((yyvsp[(5) - (5)].action))+5);
		  bufferConcat((yyvsp[(1) - (5)].action), (yyvsp[(5) - (5)].action));
		  bufferWriteOp((yyvsp[(1) - (5)].action), SWFACTION_BRANCHALWAYS);
		  bufferWriteS16((yyvsp[(1) - (5)].action), 2);
		  bufferWriteS16((yyvsp[(1) - (5)].action), bufferLength((yyvsp[(3) - (5)].action)));
		  bufferConcat((yyvsp[(1) - (5)].action), (yyvsp[(3) - (5)].action)); ;}
    break;

  case 237:
#line 1165 "swf5compiler.y"
    { if((yyvsp[(1) - (3)].lval).obj) /* obj[memexpr] or obj.ident */
		  {
		    (yyval.action) = (yyvsp[(1) - (3)].lval).obj;

		    if((yyvsp[(1) - (3)].lval).ident)
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).ident);
		    else
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).memexpr);

		    bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		    bufferWriteSetRegister((yyval.action), 0);
		    bufferWriteOp((yyval.action), SWFACTION_SETMEMBER);
		    bufferWriteRegister((yyval.action), 0);
		  }
		  else /* just ident */
		  {
		    (yyval.action) = (yyvsp[(3) - (3)].action);
		    bufferWriteOp((yyval.action), SWFACTION_DUP);
		    bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).ident);
		    bufferWriteOp((yyval.action), SWFACTION_SWAP);
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
/* tricky case missing here: lvalue ASSIGN expr */
/* like in x = y += z; */
		;}
    break;

  case 238:
#line 1192 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_INSTANCEOF); ;}
    break;

  case 240:
#line 1202 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), 0);
		  bufferWriteString((yyval.action), (yyvsp[(2) - (2)].str), strlen((yyvsp[(2) - (2)].str))+1);
		  bufferWriteOp((yyval.action), SWFACTION_NEW); ;}
    break;

  case 241:
#line 1208 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(4) - (5)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(4) - (5)].exprlist).count);
		  bufferWriteString((yyval.action), (yyvsp[(2) - (5)].str), strlen((yyvsp[(2) - (5)].str))+1);
		  bufferWriteOp((yyval.action), SWFACTION_NEW); ;}
    break;

  case 242:
#line 1214 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(2) - (3)].exprlist).count);
		  bufferWriteOp((yyval.action), SWFACTION_INITARRAY); ;}
    break;

  case 243:
#line 1219 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), 0);
		  bufferWriteOp((yyval.action), SWFACTION_INITOBJECT); ;}
    break;

  case 244:
#line 1224 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].exprlist).buffer;
		  bufferWriteInt((yyval.action), (yyvsp[(2) - (3)].exprlist).count);
		  bufferWriteOp((yyval.action), SWFACTION_INITOBJECT); ;}
    break;

  case 249:
#line 1240 "swf5compiler.y"
    { if((yyvsp[(2) - (2)].lval).obj)
		  {
		    if((yyvsp[(2) - (2)].lval).ident)	// expr . identifier
		    {
		      (yyval.action) = (yyvsp[(2) - (2)].lval).obj;
		      bufferWriteOp((yyval.action), SWFACTION_DUP);	      /* a, a */
		      bufferWriteBuffer((yyval.action), (yyvsp[(2) - (2)].lval).ident);        /* a, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i, a */
		      bufferConcat((yyval.action), (yyvsp[(2) - (2)].lval).ident);             /* a, i, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER);
		      bufferWriteOp((yyval.action), (yyvsp[(1) - (2)].op));
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a.i = a.i+1 */
		      bufferWriteRegister((yyval.action), 0);	      /* a.i+1 */
		    }
		    else	// expr [ expr ]
		    {
		      (yyval.action) = (yyvsp[(2) - (2)].lval).memexpr;			      /* i */
		      bufferConcat((yyval.action), (yyvsp[(2) - (2)].lval).obj);		      /* i, a */
		      bufferWriteSetRegister((yyval.action), 0);	/* ($2.memexpr can use reg0) */
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i */
		      bufferWriteOp((yyval.action), SWFACTION_DUP);	      /* a, i, i */
		      bufferWriteRegister((yyval.action), 0);	      /* a, i, i, a */
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, i, a[i] */
		      bufferWriteOp((yyval.action), (yyvsp[(1) - (2)].op));		      /* a, i, a[i]+1 */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a[i] = a[i]+1 */
		      bufferWriteRegister((yyval.action), 0);	      /* a[i]+1 */
		    }
		  }
		  else	// identifier
		  {
		    (yyval.action) = newBuffer();
		    bufferWriteBuffer((yyval.action), (yyvsp[(2) - (2)].lval).ident);
		    bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE);
		    bufferWriteOp((yyval.action), (yyvsp[(1) - (2)].op));
		    bufferWriteOp((yyval.action), SWFACTION_DUP);
		    bufferConcat((yyval.action), (yyvsp[(2) - (2)].lval).ident);
		    bufferWriteOp((yyval.action), SWFACTION_SWAP);
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
		;}
    break;

  case 250:
#line 1285 "swf5compiler.y"
    { if((yyvsp[(1) - (2)].lval).obj)
		  {
		    if((yyvsp[(1) - (2)].lval).ident)
		    {
		      (yyval.action) = (yyvsp[(1) - (2)].lval).obj;	                      /* a */
		      bufferWriteOp((yyval.action), SWFACTION_DUP);	      /* a, a */
		      bufferWriteBuffer((yyval.action), (yyvsp[(1) - (2)].lval).ident);        /* a, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, a.i */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a.i, a */
		      bufferConcat((yyval.action), (yyvsp[(1) - (2)].lval).ident);             /* a.i, a, i */
		      bufferWriteRegister((yyval.action), 0);             /* a.i, a, i, a.i */
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (2)].op));		      /* a.i, a, i, a.i+1 */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER);
		    }
		    else
		    {
		      (yyval.action) = (yyvsp[(1) - (2)].lval).memexpr;
		      bufferConcat((yyval.action), (yyvsp[(1) - (2)].lval).obj);               /* i, a */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i */
		      bufferWriteOp((yyval.action), SWFACTION_DUP);	      /* a, i, i */
		      bufferWriteRegister((yyval.action), 0);             /* a, i, i, a */
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, i, a[i] */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (2)].op));		      /* a, i, a[i]+1 */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER);
		      bufferWriteRegister((yyval.action), 0);             /* a[i] */
		    }
		  }
		  else
		  {
		    (yyval.action) = newBuffer();
		    bufferWriteBuffer((yyval.action), (yyvsp[(1) - (2)].lval).ident);
		    bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE);
		    bufferWriteOp((yyval.action), SWFACTION_DUP);
		    bufferWriteOp((yyval.action), (yyvsp[(2) - (2)].op));
		    bufferConcat((yyval.action), (yyvsp[(1) - (2)].lval).ident);
		    bufferWriteOp((yyval.action), SWFACTION_SWAP);
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
		;}
    break;

  case 251:
#line 1330 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (3)].action); ;}
    break;

  case 252:
#line 1333 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), -(yyvsp[(2) - (2)].intVal)); ;}
    break;

  case 253:
#line 1337 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteInt((yyval.action), (yyvsp[(1) - (1)].intVal)); ;}
    break;

  case 254:
#line 1341 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteDouble((yyval.action), -(yyvsp[(2) - (2)].doubleVal)); ;}
    break;

  case 255:
#line 1345 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteDouble((yyval.action), (yyvsp[(1) - (1)].doubleVal)); ;}
    break;

  case 256:
#line 1349 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteBoolean((yyval.action), (yyvsp[(1) - (1)].intVal)); ;}
    break;

  case 257:
#line 1353 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteNull((yyval.action)); ;}
    break;

  case 258:
#line 1357 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  free((yyvsp[(1) - (1)].str)); ;}
    break;

  case 260:
#line 1366 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (3)].action);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action)); ;}
    break;

  case 261:
#line 1372 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (3)].str), strlen((yyvsp[(1) - (3)].str))+1);
		  bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		  bufferWriteOp((yyval.action), SWFACTION_VAREQUALS); ;}
    break;

  case 262:
#line 1378 "swf5compiler.y"
    { (yyval.action) = newBuffer();
		  bufferWriteString((yyval.action), (yyvsp[(1) - (1)].str), strlen((yyvsp[(1) - (1)].str))+1);
		  bufferWriteOp((yyval.action), SWFACTION_VAR); ;}
    break;

  case 263:
#line 1385 "swf5compiler.y"
    { asmBuffer = newBuffer(); ;}
    break;

  case 264:
#line 1387 "swf5compiler.y"
    { (yyval.action) = asmBuffer; ;}
    break;

  case 265:
#line 1390 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(2) - (2)].action); ;}
    break;

  case 267:
#line 1395 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action);
		  bufferWriteOp((yyval.action), SWFACTION_POP); ;}
    break;

  case 268:
#line 1399 "swf5compiler.y"
    { (yyval.action) = (yyvsp[(1) - (1)].action);
		  bufferWriteOp((yyval.action), SWFACTION_POP); ;}
    break;

  case 269:
#line 1403 "swf5compiler.y"
    { if((yyvsp[(2) - (2)].lval).obj)
		  {
		    if((yyvsp[(2) - (2)].lval).ident)
		    {
		      (yyval.action) = (yyvsp[(2) - (2)].lval).obj;		              /* a */
		      bufferWriteOp((yyval.action), SWFACTION_DUP);	      /* a, a */
		      bufferWriteBuffer((yyval.action), (yyvsp[(2) - (2)].lval).ident);	      /* a, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, a.i */
		      bufferWriteOp((yyval.action), (yyvsp[(1) - (2)].op));		      /* a, a.i+1 */
		      bufferConcat((yyval.action), (yyvsp[(2) - (2)].lval).ident);	      /* a, a.i+1, i */
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i, a.i+1 */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a.i = a.i+1 */
		    }
		    else
		    {
		      /* weird contortions so that $2.memexpr can use reg 0 */
		      (yyval.action) = (yyvsp[(2) - (2)].lval).memexpr;			      /* i */
		      bufferConcat((yyval.action), (yyvsp[(2) - (2)].lval).obj);		      /* i, a */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i */
		      bufferWriteOp((yyval.action), SWFACTION_DUP);	      /* a, i, i */
		      bufferWriteRegister((yyval.action), 0);	      /* a, i, i, a */
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, i, a[i] */
		      bufferWriteOp((yyval.action), (yyvsp[(1) - (2)].op));		      /* a, i, a[i]+1 */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a[i] = a[i]+1 */
		    }
		  }
		  else
		  {
		    (yyval.action) = (yyvsp[(2) - (2)].lval).ident;
		    bufferWriteOp((yyval.action), SWFACTION_DUP);
		    bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE);
		    bufferWriteOp((yyval.action), (yyvsp[(1) - (2)].op));
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
		;}
    break;

  case 270:
#line 1442 "swf5compiler.y"
    { if((yyvsp[(1) - (2)].lval).obj)
		  {
		    if((yyvsp[(1) - (2)].lval).ident)
		    {
		      (yyval.action) = (yyvsp[(1) - (2)].lval).obj;			      /* a */
		      bufferWriteOp((yyval.action), SWFACTION_DUP);       /* a, a */
		      bufferWriteBuffer((yyval.action), (yyvsp[(1) - (2)].lval).ident);	      /* a, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, a.i */
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (2)].op));                  /* a, a.i+1 */
		      bufferConcat((yyval.action), (yyvsp[(1) - (2)].lval).ident);             /* a, a.i+1, i */
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i, a.i+1 */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a.i = a.i+1 */
		    }
		    else
		    {
		      /* weird contortions so that $1.memexpr can use reg 0 */
		      (yyval.action) = (yyvsp[(1) - (2)].lval).memexpr;	/* i */
		      bufferConcat((yyval.action), (yyvsp[(1) - (2)].lval).obj);		      /* i, a */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i */
		      bufferWriteOp((yyval.action), SWFACTION_DUP);       /* a, i, i */
		      bufferWriteRegister((yyval.action), 0);             /* a, i, i, a */
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, i, a[i] */
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (2)].op));                  /* a, i, a[i]+1 */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a[i] = a[i]+1 */
		    }
		  }
		  else
		  {
		    (yyval.action) = (yyvsp[(1) - (2)].lval).ident;
		    bufferWriteOp((yyval.action), SWFACTION_DUP);	
		    bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE);
		    bufferWriteOp((yyval.action), (yyvsp[(2) - (2)].op));
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
		;}
    break;

  case 271:
#line 1481 "swf5compiler.y"
    { if((yyvsp[(1) - (3)].lval).obj)
		  {
		    (yyval.action) = (yyvsp[(1) - (3)].lval).obj;

		    if((yyvsp[(1) - (3)].lval).ident)
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).ident);
		    else
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).memexpr);

		    bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		    bufferWriteOp((yyval.action), SWFACTION_SETMEMBER);
		  }
		  else
		  {
		    (yyval.action) = (yyvsp[(1) - (3)].lval).ident;
		    bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
		;}
    break;

  case 272:
#line 1502 "swf5compiler.y"
    { if((yyvsp[(1) - (3)].lval).obj)
		  {
		    if((yyvsp[(1) - (3)].lval).ident)
		    {
		      (yyval.action) = (yyvsp[(1) - (3)].lval).obj;			      /* a */
		      bufferWriteOp((yyval.action), SWFACTION_DUP);	      /* a, a */
		      bufferWriteBuffer((yyval.action), (yyvsp[(1) - (3)].lval).ident);	      /* a, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, a.i */
		      bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));		      /* a, a.i, v */
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (3)].op));		      /* a, a.i+v */
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).ident);	      /* a, a.i+v, i */
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i, a.i+v */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a.i = a.i+v */
		    }
		    else
		    {
		      (yyval.action) = (yyvsp[(1) - (3)].lval).memexpr;			      /* i */
		      bufferConcat((yyval.action), (yyvsp[(1) - (3)].lval).obj);		      /* i, a */
		      bufferWriteSetRegister((yyval.action), 0);
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i */
		      bufferWriteOp((yyval.action), SWFACTION_DUP);	      /* a, i, i */
		      bufferWriteRegister((yyval.action), 0);	      /* a, i, i, a */
		      bufferWriteOp((yyval.action), SWFACTION_SWAP);      /* a, i, a, i */
		      bufferWriteOp((yyval.action), SWFACTION_GETMEMBER); /* a, i, a[i] */
		      bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));		      /* a, i, a[i], v */
		      bufferWriteOp((yyval.action), (yyvsp[(2) - (3)].op));		      /* a, i, a[i]+v */
		      bufferWriteOp((yyval.action), SWFACTION_SETMEMBER); /* a[i] = a[i]+v */
		    }
		  }
		  else
		  {
		    (yyval.action) = (yyvsp[(1) - (3)].lval).ident;
		    bufferWriteOp((yyval.action), SWFACTION_DUP);
		    bufferWriteOp((yyval.action), SWFACTION_GETVARIABLE);
		    bufferConcat((yyval.action), (yyvsp[(3) - (3)].action));
		    bufferWriteOp((yyval.action), (yyvsp[(2) - (3)].op));
		    bufferWriteOp((yyval.action), SWFACTION_SETVARIABLE);
		  }
		;}
    break;

  case 274:
#line 1547 "swf5compiler.y"
    { (yyval.len) = (yyvsp[(1) - (2)].len) + (yyvsp[(2) - (2)].len); ;}
    break;

  case 275:
#line 1552 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer,
						     SWFACTION_WITH); ;}
    break;

  case 276:
#line 1554 "swf5compiler.y"
    { (yyval.len) = (yyvsp[(2) - (4)].len) + (yyvsp[(3) - (4)].len);
				  bufferPatchLength(asmBuffer, (yyvsp[(3) - (4)].len)); ;}
    break;

  case 277:
#line 1559 "swf5compiler.y"
    { (yyval.len) = bufferWriteConstantString(asmBuffer,(byte*) (yyvsp[(1) - (1)].str),
								 strlen((yyvsp[(1) - (1)].str))+1); ;}
    break;

  case 278:
#line 1562 "swf5compiler.y"
    { bufferWriteU8(asmBuffer, PUSH_INT);
				  (yyval.len) = bufferWriteInt(asmBuffer, (yyvsp[(1) - (1)].intVal))+1; ;}
    break;

  case 279:
#line 1565 "swf5compiler.y"
    { bufferWriteU8(asmBuffer, PUSH_DOUBLE);
				  (yyval.len) = bufferWriteDouble(asmBuffer, (yyvsp[(1) - (1)].doubleVal))+1; ;}
    break;

  case 280:
#line 1568 "swf5compiler.y"
    { bufferWriteU8(asmBuffer, PUSH_BOOLEAN);
				  (yyval.len) = bufferWriteU8(asmBuffer, (yyvsp[(1) - (1)].intVal))+1; ;}
    break;

  case 281:
#line 1571 "swf5compiler.y"
    { (yyval.len) = bufferWriteU8(asmBuffer, PUSH_NULL); ;}
    break;

  case 282:
#line 1573 "swf5compiler.y"
    { bufferWriteU8(asmBuffer, PUSH_REGISTER);
				  (yyval.len) = bufferWriteU8(asmBuffer,
						     (char)atoi((yyvsp[(1) - (1)].str)))+1; ;}
    break;

  case 283:
#line 1580 "swf5compiler.y"
    { (yyval.len) = (yyvsp[(1) - (1)].len); ;}
    break;

  case 284:
#line 1581 "swf5compiler.y"
    { (yyval.len) += (yyvsp[(3) - (3)].len); ;}
    break;

  case 285:
#line 1585 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer,
						     SWFACTION_PUSHDATA);
				  (yyval.len) += bufferWriteS16(asmBuffer, 0); ;}
    break;

  case 286:
#line 1588 "swf5compiler.y"
    { (yyval.len) = (yyvsp[(2) - (3)].len) + (yyvsp[(3) - (3)].len);
				  bufferPatchLength(asmBuffer, (yyvsp[(3) - (3)].len)); ;}
    break;

  case 288:
#line 1594 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_SETREGISTER);
				  (yyval.len) += bufferWriteS16(asmBuffer, 1);
				  (yyval.len) += bufferWriteU8(asmBuffer,
						      (char)atoi((yyvsp[(2) - (2)].str))); ;}
    break;

  case 289:
#line 1600 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer,
						     SWFACTION_CALLFUNCTION); ;}
    break;

  case 290:
#line 1602 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer,
						     SWFACTION_RETURN); ;}
    break;

  case 291:
#line 1604 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_CALLMETHOD); ;}
    break;

  case 292:
#line 1606 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_BITWISEAND); ;}
    break;

  case 293:
#line 1608 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_BITWISEOR); ;}
    break;

  case 294:
#line 1610 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_BITWISEXOR); ;}
    break;

  case 295:
#line 1612 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_MODULO); ;}
    break;

  case 296:
#line 1614 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_NEWADD); ;}
    break;

  case 297:
#line 1616 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_NEWLESSTHAN); ;}
    break;

  case 298:
#line 1618 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_NEWEQUALS); ;}
    break;

  case 299:
#line 1620 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_INCREMENT); ;}
    break;

  case 300:
#line 1622 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_DECREMENT); ;}
    break;

  case 301:
#line 1624 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_TYPEOF); ;}
    break;

  case 302:
#line 1626 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_INSTANCEOF); ;}
    break;

  case 303:
#line 1628 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_ENUMERATE); ;}
    break;

  case 304:
#line 1630 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_DELETE); ;}
    break;

  case 305:
#line 1632 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_NEW); ;}
    break;

  case 306:
#line 1634 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_INITARRAY); ;}
    break;

  case 307:
#line 1636 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_INITOBJECT); ;}
    break;

  case 308:
#line 1638 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_GETMEMBER); ;}
    break;

  case 309:
#line 1640 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_SETMEMBER); ;}
    break;

  case 310:
#line 1642 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_SHIFTLEFT); ;}
    break;

  case 311:
#line 1644 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_SHIFTRIGHT); ;}
    break;

  case 312:
#line 1646 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_SHIFTRIGHT2); ;}
    break;

  case 313:
#line 1648 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_VAR); ;}
    break;

  case 314:
#line 1650 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, 
						     SWFACTION_VAREQUALS); ;}
    break;

  case 315:
#line 1654 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_ADD); ;}
    break;

  case 316:
#line 1655 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SUBTRACT); ;}
    break;

  case 317:
#line 1656 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MULTIPLY); ;}
    break;

  case 318:
#line 1657 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_DIVIDE); ;}
    break;

  case 319:
#line 1658 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_EQUAL); ;}
    break;

  case 320:
#line 1659 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LESSTHAN); ;}
    break;

  case 321:
#line 1660 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LOGICALAND); ;}
    break;

  case 322:
#line 1661 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LOGICALOR); ;}
    break;

  case 323:
#line 1662 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_LOGICALNOT); ;}
    break;

  case 324:
#line 1663 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGEQ); ;}
    break;

  case 325:
#line 1664 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGLENGTH); ;}
    break;

  case 326:
#line 1665 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SUBSTRING); ;}
    break;

  case 327:
#line 1666 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_INT); ;}
    break;

  case 328:
#line 1667 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_DUP); ;}
    break;

  case 329:
#line 1668 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SWAP); ;}
    break;

  case 330:
#line 1669 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_POP); ;}
    break;

  case 331:
#line 1670 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_GETVARIABLE); ;}
    break;

  case 332:
#line 1671 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SETVARIABLE); ;}
    break;

  case 333:
#line 1672 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_SETTARGETEXPRESSION); ;}
    break;

  case 334:
#line 1673 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGCONCAT); ;}
    break;

  case 335:
#line 1674 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_DUPLICATECLIP); ;}
    break;

  case 336:
#line 1675 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_REMOVECLIP); ;}
    break;

  case 337:
#line 1676 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_TRACE); ;}
    break;

  case 338:
#line 1677 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_STRINGCOMPARE); ;}
    break;

  case 339:
#line 1678 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_RANDOM); ;}
    break;

  case 340:
#line 1679 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBLENGTH); ;}
    break;

  case 341:
#line 1680 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_ORD); ;}
    break;

  case 342:
#line 1681 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_CHR); ;}
    break;

  case 343:
#line 1682 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_GETTIMER); ;}
    break;

  case 344:
#line 1683 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBSUBSTRING); ;}
    break;

  case 345:
#line 1684 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBORD); ;}
    break;

  case 346:
#line 1685 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_MBCHR); ;}
    break;

  case 347:
#line 1688 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_BRANCHALWAYS);
				  (yyval.len) += bufferWriteS16(asmBuffer, 2);
				  (yyval.len) += bufferBranchTarget(asmBuffer, (yyvsp[(2) - (2)].str)); ;}
    break;

  case 348:
#line 1692 "swf5compiler.y"
    { (yyval.len) = bufferWriteOp(asmBuffer, SWFACTION_BRANCHIFTRUE);
				  (yyval.len) += bufferWriteS16(asmBuffer, 2);
				  (yyval.len) += bufferBranchTarget(asmBuffer, (yyvsp[(2) - (2)].str)); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 5479 "swf5compiler.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 1697 "swf5compiler.y"



