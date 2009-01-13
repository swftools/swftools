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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* If NAME_PREFIX is specified substitute the variables and functions
   names.  */
#define yyparse swf4parse
#define yylex   swf4lex
#define yyerror swf4error
#define yylval  swf4lval
#define yychar  swf4char
#define yydebug swf4debug
#define yynerrs swf4nerrs


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




/* Copy the first part of user declarations.  */
#line 5 "swf4compiler.y"


#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "compile.h"
#include "action.h"

#define YYPARSE_PARAM buffer



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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 17 "swf4compiler.y"
typedef union YYSTYPE {
  Buffer action;
  char *str;
  SWFActionFunction function;
  SWFGetUrl2Method getURLMethod;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 237 "swf4compiler.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 249 "swf4compiler.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
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
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  108
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1341

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  86
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  22
/* YYNRULES -- Number of rules. */
#define YYNRULES  137
/* YYNRULES -- Number of states. */
#define YYNSTATES  369

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   322

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    76,     2,     2,     2,     2,    71,     2,
      83,    84,    74,    72,    65,    73,    85,    75,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    68,    82,
      69,    66,    70,    67,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    80,     2,    81,     2,     2,     2,     2,
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
      77,    78,    79
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    12,    15,    19,    21,
      24,    26,    28,    30,    32,    34,    38,    39,    41,    44,
      55,    64,    74,    85,    94,   104,   112,   118,   119,   121,
     131,   137,   144,   154,   155,   157,   160,   163,   167,   172,
     177,   182,   187,   194,   203,   210,   217,   226,   233,   242,
     249,   264,   273,   277,   281,   285,   289,   293,   297,   302,
     307,   312,   317,   322,   327,   333,   339,   344,   348,   353,
     358,   363,   368,   373,   380,   389,   396,   399,   402,   404,
     408,   410,   413,   415,   417,   419,   423,   428,   431,   436,
     439,   442,   445,   449,   453,   457,   461,   465,   469,   473,
     477,   481,   485,   489,   493,   497,   501,   505,   509,   513,
     519,   521,   525,   527,   529,   531,   533,   535,   537,   539,
     541,   545,   547,   549,   552,   555,   560,   565,   569,   573,
     577,   581,   585,   591,   597,   603,   609,   615
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      87,     0,    -1,    88,    -1,    89,    -1,    88,    89,    -1,
      90,    -1,    80,    81,    -1,    80,    92,    81,    -1,    82,
      -1,   106,    82,    -1,    93,    -1,    95,    -1,    97,    -1,
      98,    -1,   106,    -1,    91,    65,   106,    -1,    -1,    90,
      -1,    92,    90,    -1,     6,    83,    40,    83,    44,    84,
      84,    90,     7,    90,    -1,     6,    83,    40,    83,    44,
      84,    84,    90,    -1,     6,    83,    76,    40,    83,    44,
      84,    84,    90,    -1,     6,    83,    40,    83,   107,    84,
      84,    90,     7,    90,    -1,     6,    83,    40,    83,   107,
      84,    84,    90,    -1,     6,    83,    76,    40,    83,   107,
      84,    84,    90,    -1,     6,    83,   107,    84,    90,     7,
      90,    -1,     6,    83,   107,    84,    90,    -1,    -1,   107,
      -1,     9,    83,    76,    40,    83,    44,    84,    84,    90,
      -1,     9,    83,   107,    84,    90,    -1,     8,    90,     9,
      83,   107,    84,    -1,     4,    83,    96,    82,    94,    82,
      96,    84,    90,    -1,    -1,    91,    -1,     5,    82,    -1,
       3,    82,    -1,    21,    83,    84,    -1,    24,    83,   103,
      84,    -1,    24,    83,    43,    84,    -1,    18,    83,   107,
      84,    -1,    19,    83,   107,    84,    -1,    25,    83,   107,
      65,   107,    84,    -1,    25,    83,   107,    65,   107,    65,
      47,    84,    -1,    26,    83,    43,    65,    43,    84,    -1,
      27,    83,   107,    65,   107,    84,    -1,    27,    83,   107,
      65,   107,    65,    47,    84,    -1,    28,    83,   107,    65,
     107,    84,    -1,    28,    83,   107,    65,   107,    65,    47,
      84,    -1,    20,    83,   107,    65,   107,    84,    -1,    20,
      83,   107,    65,   107,    65,   107,    65,   107,    65,   107,
      65,   107,    84,    -1,    17,    83,   107,    65,   107,    65,
     107,    84,    -1,    32,    83,    84,    -1,    33,    83,    84,
      -1,    34,    83,    84,    -1,    35,    83,    84,    -1,    36,
      83,    84,    -1,    37,    83,    84,    -1,    38,    83,    44,
      84,    -1,    38,    83,    43,    84,    -1,    38,    83,   107,
      84,    -1,    39,    83,   107,    84,    -1,    41,    83,    43,
      84,    -1,    41,    83,   107,    84,    -1,    42,    83,    43,
      84,    90,    -1,    42,    83,   107,    84,    90,    -1,    11,
      83,   107,    84,    -1,    12,    83,    84,    -1,    13,    83,
     107,    84,    -1,    14,    83,   107,    84,    -1,    15,    83,
     107,    84,    -1,    22,    83,   107,    84,    -1,    23,    83,
     107,    84,    -1,    16,    83,   107,    65,   107,    84,    -1,
      30,    83,   107,    65,   107,    65,   107,    84,    -1,    31,
      83,   107,    65,    43,    84,    -1,   105,    54,    -1,   105,
      55,    -1,   100,    -1,    83,   102,    84,    -1,    44,    -1,
      73,    44,    -1,    43,    -1,   103,    -1,   104,    -1,   104,
      85,    45,    -1,    54,   104,    85,    45,    -1,    54,   105,
      -1,    55,   104,    85,    45,    -1,    55,   105,    -1,    73,
     102,    -1,    76,   102,    -1,   105,    66,   102,    -1,   102,
      74,   102,    -1,   102,    75,   102,    -1,   102,    72,   102,
      -1,   102,    73,   102,    -1,   102,    71,   102,    -1,   102,
      69,   102,    -1,   102,    70,   102,    -1,   102,    49,   102,
      -1,   102,    50,   102,    -1,   102,    61,   102,    -1,   102,
      60,   102,    -1,   102,    62,   102,    -1,   102,    48,   102,
      -1,   102,    51,   102,    -1,   102,    52,   102,    -1,   102,
      53,   102,    -1,   102,    67,   102,    68,   102,    -1,    45,
      -1,   104,    68,    45,    -1,    10,    -1,    85,    -1,    75,
      -1,    63,    -1,    45,    -1,    46,    -1,   103,    -1,    43,
      -1,    83,   102,    84,    -1,   101,    -1,    99,    -1,    54,
     105,    -1,    55,   105,    -1,    54,   104,    85,    45,    -1,
      55,   104,    85,    45,    -1,   105,    66,   102,    -1,   105,
      58,   102,    -1,   105,    57,   102,    -1,   105,    56,   102,
      -1,   105,    59,   102,    -1,   104,    85,    45,    66,   102,
      -1,   104,    85,    45,    58,   102,    -1,   104,    85,    45,
      57,   102,    -1,   104,    85,    45,    56,   102,    -1,   104,
      85,    45,    59,   102,    -1,   102,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   144,   144,   150,   151,   156,   160,   161,   162,   163,
     164,   165,   166,   167,   171,   172,   176,   177,   178,   185,
     201,   217,   229,   243,   257,   267,   278,   288,   289,   313,
     328,   340,   348,   370,   371,   375,   383,   391,   395,   402,
     409,   413,   418,   425,   432,   441,   448,   455,   462,   470,
     477,   489,   498,   502,   506,   510,   514,   518,   522,   529,
     536,   542,   548,   555,   559,   573,   586,   590,   594,   598,
     602,   606,   610,   614,   619,   625,   634,   645,   659,   661,
     664,   669,   675,   680,   686,   691,   699,   714,   724,   739,
     749,   754,   758,   764,   769,   774,   779,   784,   789,   794,
     799,   805,   810,   815,   819,   823,   827,   832,   836,   840,
     852,   854,   861,   864,   867,   870,   873,   876,   881,   886,
     891,   895,   897,   899,   907,   915,   928,   941,   945,   952,
     959,   966,   973,   982,   995,  1008,  1021,  1036
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BREAK", "FOR", "CONTINUE", "IF", "ELSE", 
  "DO", "WHILE", "THIS", "EVAL", "TIME", "RANDOM", "LENGTH", "INT", 
  "CONCAT", "DUPLICATECLIP", "REMOVECLIP", "TRACE", "STARTDRAG", 
  "STOPDRAG", "ORD", "CHR", "CALLFRAME", "GETURL", "GETURL1", "LOADMOVIE", 
  "LOADVARIABLES", "POSTURL", "SUBSTR", "GETPROPERTY", "NEXTFRAME", 
  "PREVFRAME", "PLAY", "STOP", "TOGGLEQUALITY", "STOPSOUNDS", "GOTOFRAME", 
  "GOTOANDPLAY", "FRAMELOADED", "SETTARGET", "TELLTARGET", "STRING", 
  "NUMBER", "IDENTIFIER", "PATH", "GETURL_METHOD", "\"==\"", "\"<=\"", 
  "\">=\"", "\"!=\"", "\"&&\"", "\"||\"", "\"++\"", "\"--\"", "\"+=\"", 
  "\"/=\"", "\"*=\"", "\"-=\"", "\"===\"", "\"!==\"", "\"<=>\"", "\"..\"", 
  "\"end\"", "','", "'='", "'?'", "':'", "'<'", "'>'", "'&'", "'+'", 
  "'-'", "'*'", "'/'", "'!'", "UMINUS", "POSTFIX", "NEGATE", "'{'", "'}'", 
  "';'", "'('", "')'", "'.'", "$accept", "program", "elems", "elem", 
  "stmt", "assign_stmts", "statements", "if_stmt", "expr_opt", 
  "iter_stmt", "assign_stmts_opt", "cont_stmt", "break_stmt", 
  "void_function_call", "function_call", "pf_expr", "rhs_expr", 
  "variable", "sprite", "lhs_expr", "assign_stmt", "expr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,    44,    61,    63,    58,    60,
      62,    38,    43,    45,    42,    47,    33,   320,   321,   322,
     123,   125,    59,    40,    41,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    86,    87,    88,    88,    89,    90,    90,    90,    90,
      90,    90,    90,    90,    91,    91,    92,    92,    92,    93,
      93,    93,    93,    93,    93,    93,    93,    94,    94,    95,
      95,    95,    95,    96,    96,    97,    98,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   101,   101,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     103,   103,   104,   104,   104,   104,   104,   104,   105,   105,
     105,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   107
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1,     2,     1,     2,     3,     1,     2,
       1,     1,     1,     1,     1,     3,     0,     1,     2,    10,
       8,     9,    10,     8,     9,     7,     5,     0,     1,     9,
       5,     6,     9,     0,     1,     2,     2,     3,     4,     4,
       4,     4,     6,     8,     6,     6,     8,     6,     8,     6,
      14,     8,     3,     3,     3,     3,     3,     3,     4,     4,
       4,     4,     4,     4,     5,     5,     4,     3,     4,     4,
       4,     4,     4,     6,     8,     6,     2,     2,     1,     3,
       1,     2,     1,     1,     1,     3,     4,     2,     4,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     5,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     2,     2,     4,     4,     3,     3,     3,
       3,     3,     5,     5,     5,     5,     5,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,   112,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   119,   110,
     117,     0,     0,   115,   114,     0,     8,     0,   113,     0,
       2,     3,     5,    10,    11,    12,    13,   122,   121,   118,
       0,     0,     0,    36,    33,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     123,     0,   124,     6,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    82,    80,     0,     0,
       0,     0,     0,    78,     0,    83,    84,     0,     1,     4,
       0,     0,    76,    77,     0,     0,     0,     0,     0,     9,
      34,     0,    14,     0,     0,   137,     0,     0,     0,     0,
       0,     0,     0,     0,    37,     0,     0,     0,     0,     0,
       0,     0,    52,    53,    54,    55,    56,    57,    82,    80,
       0,     0,    82,     0,    82,     0,     0,     0,     7,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    87,     0,    89,    80,    90,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   120,     0,     0,   111,     0,
     130,   129,   128,   131,   127,     0,    27,     0,     0,     0,
       0,     0,     0,     0,    40,    41,     0,    39,    38,     0,
       0,     0,     0,    59,    58,    60,    61,    62,    63,     0,
       0,   125,   126,     0,    67,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,   105,   100,   101,   106,
     107,   108,   103,   102,   104,     0,    98,    99,    97,    95,
      96,    93,    94,    85,    92,     0,     0,     0,     0,     0,
      15,     0,    28,    80,     0,     0,    26,     0,     0,    30,
       0,     0,     0,     0,     0,     0,    64,    65,    66,    68,
      69,    70,     0,    71,    72,     0,     0,    86,    88,     0,
     135,   134,   133,   136,   132,    33,     0,     0,    80,     0,
       0,    31,     0,     0,     0,    49,     0,    42,    44,     0,
      45,     0,    47,     0,     0,     0,   109,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,     0,    73,
       0,    75,     0,    20,    23,     0,     0,     0,    51,     0,
      43,    46,    48,     0,    32,     0,     0,    21,    24,    29,
       0,    74,    19,    22,     0,     0,     0,     0,    50
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    39,    40,    41,    42,   120,    85,    43,   271,    44,
     121,    45,    46,    47,   103,    48,   125,   105,   106,   107,
      52,   126
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -70
static const short yypact[] =
{
     780,   -69,   -64,   -60,   -49,   780,   -28,   -70,   -22,   -14,
      -2,     3,     4,    11,    13,    17,    19,    20,    23,    25,
      26,    29,    30,    31,    33,    47,    54,    56,   -70,    16,
     -70,    32,    32,   -70,   -70,   614,   -70,   422,   -70,    72,
     780,   -70,   -70,   -70,   -70,   -70,   -70,   -70,   -70,   -70,
     -12,    65,     1,   -70,   108,   -70,   826,    80,   994,   422,
     422,   422,   422,    64,     5,   422,   118,   422,   422,    81,
      82,    86,    88,    89,   104,  1020,   422,  1076,  1102,   -11,
     -70,    -9,   -70,   -70,   -70,   697,   106,   109,   117,   119,
     120,   121,   123,   125,   127,   130,    98,   -70,    32,    32,
    1158,   422,   422,   -70,   516,   135,    -6,   141,   -70,   -70,
     145,   169,   -70,   -70,   422,   422,   422,   422,   422,   -70,
     151,   137,   -70,   139,   882,   613,   136,   140,   938,   142,
     160,   143,   144,   166,   -70,   148,   149,   170,   172,   174,
     175,   178,   -70,   -70,   -70,   -70,   -70,   -70,   -48,   150,
     161,   162,   -21,   163,   -13,   164,   199,   204,   -70,   -70,
     422,   167,   422,   422,   422,   422,   422,   422,   422,   422,
      -3,   -70,    -1,   -70,   -70,   -70,   -70,  1257,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,   422,   422,   -70,   205,   422,   -70,   128,
     613,   613,   613,   613,   613,   108,   422,  1184,   171,   780,
     422,   173,   780,   422,   -70,   -70,   422,   -70,   -70,   422,
     210,   422,   422,   -70,   -70,   -70,   -70,   -70,   -70,   780,
     780,   -70,   -70,   176,   -70,   177,   179,   180,   190,   182,
     188,   192,   193,   214,   228,   212,   -46,   124,   124,   -46,
     696,   696,   -46,   -46,   124,   107,   124,   124,   -65,    18,
      18,   -70,   -70,   -70,   613,   422,   422,   422,   422,   422,
     -70,   197,   -70,   191,   196,  1240,   274,   206,   244,   -70,
     224,   -63,   -51,   207,   -45,   -35,   -70,   -70,   -70,   -70,
     -70,   -70,   422,   -70,   -70,   422,   249,   -70,   -70,   422,
     613,   613,   613,   613,   613,   108,   211,   213,   215,   216,
     780,   -70,   217,   422,   422,   -70,   246,   -70,   -70,   247,
     -70,   251,   -70,   218,   231,   219,   613,   220,   780,   780,
     222,   223,   -70,   225,   229,   243,   230,   234,   235,   -70,
     422,   -70,   780,   305,   308,   780,   780,   780,   -70,   422,
     -70,   -70,   -70,   236,   -70,   780,   780,   -70,   -70,   -70,
     256,   -70,   -70,   -70,   422,   258,   422,   240,   -70
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
     -70,   -70,   -70,   285,    53,   -70,   -70,   -70,   -70,   -70,
      21,   -70,   -70,   -70,   -70,   -70,   346,     0,     6,    12,
     -53,   209
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -121
static const short yytable[] =
{
      49,   122,   314,   179,   180,    49,    50,   191,   192,   193,
     194,    50,    51,    53,   316,     7,   186,    51,  -119,    54,
     319,   315,    55,   188,   189,   190,   191,   192,   193,   194,
     321,    49,    49,   317,    56,    49,   223,    79,    81,   320,
      49,    50,     7,    80,    82,  -119,    50,    51,   135,   322,
      29,    30,    51,  -119,    49,    58,   110,   110,    57,   110,
      50,    59,   110,   227,   136,   110,    51,   110,    33,    60,
     137,   229,   108,   111,   156,    28,   157,    29,    30,   196,
      34,    61,   243,   119,   244,    49,    62,    63,    84,   127,
      38,    50,   193,   194,    64,    33,    65,    51,    49,    49,
      66,  -116,    67,    68,   170,   172,    69,    34,    70,    71,
     171,   173,    72,    73,    74,    37,    75,    38,     7,   112,
     113,   114,   115,   116,   117,     8,     9,    10,    11,    12,
      76,   118,    13,    14,    15,    16,    17,    77,   159,    78,
      18,    19,    20,    21,    22,    23,    24,    25,   134,    26,
      27,    28,   270,    29,    30,   178,   179,   180,   181,   182,
     183,   139,    31,    32,  -119,   142,   143,   184,   185,   186,
     144,    33,   145,   146,   187,   299,   188,   189,   190,   191,
     192,   193,   194,    34,   265,   266,   267,   268,   147,   160,
     198,    37,   161,    38,   269,   190,   191,   192,   193,   194,
     162,  -118,   163,   164,   165,    49,   166,   197,   167,    49,
     168,    50,    49,   169,   199,    50,   205,    51,    50,   206,
     209,    51,   207,   210,    51,   213,   212,   214,   215,    49,
      49,   216,   217,   218,   224,    50,    50,   219,   110,   220,
     221,    51,    51,   222,   231,   225,   226,   228,   230,   232,
     263,   234,   122,   283,   275,   292,   278,   295,   296,   297,
     288,   289,   276,   290,   291,   279,   293,   129,   130,   131,
     132,   133,   294,   298,   138,   306,   140,   141,  -120,   305,
     307,   310,   286,   287,   150,   151,   153,   155,   312,   313,
     311,   318,   325,   336,   337,   328,   340,   329,   338,   330,
     331,   333,   339,   341,   342,    49,   345,   346,   349,   347,
      49,    50,   355,   348,   350,   356,    50,    51,   351,   352,
     361,   364,    51,   366,   368,   109,   327,     0,    49,    49,
       0,     0,     0,     0,    50,    50,     0,     0,     0,     0,
      51,    51,    49,     0,     0,    49,    49,    49,    50,     0,
       0,    50,    50,    50,    51,    49,    49,    51,    51,    51,
       0,    50,    50,   332,     0,     0,     0,    51,    51,   233,
       0,   235,   236,   237,   238,   239,   240,   241,   242,     0,
       0,   343,   344,   104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   354,     0,     0,   357,   358,
     359,     0,     0,     0,     0,     0,     0,     0,   362,   363,
       0,     0,     0,     0,     0,   272,   274,     0,     0,   277,
       0,     0,   280,     0,     0,   281,     0,     0,   282,     0,
     284,   285,     7,    86,    87,    88,    89,    90,    91,     0,
       0,     0,     0,     0,    92,    93,   175,   176,   177,     0,
       0,     0,    94,    95,     0,     0,     0,     0,     0,     0,
     200,   201,   202,   203,   204,    96,    97,    29,    30,     0,
     176,     0,     0,     0,   176,     0,    98,    99,     0,     0,
       0,     0,     0,     0,   309,    33,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,    34,   101,     0,
       0,   323,     0,     0,   324,   102,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   334,   335,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,     0,     0,   264,     0,     0,     0,     0,     0,   353,
       0,     0,     0,     0,     0,     0,     0,     0,   360,     0,
       0,     0,     0,     0,   178,   179,   180,   181,   182,   183,
       0,     0,     0,   365,     0,   367,   184,   185,   186,     0,
       0,     0,     0,   187,     0,   188,   189,   190,   191,   192,
     193,   194,     0,     0,     0,     0,     0,     0,     0,     0,
     195,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   300,   301,   302,   303,   304,     0,     1,     2,     3,
       4,     0,     5,     6,     7,     0,     0,     0,     0,     0,
       0,     8,     9,    10,    11,    12,     0,     0,    13,    14,
      15,    16,    17,     0,     0,   326,    18,    19,    20,    21,
      22,    23,    24,    25,     0,    26,    27,    28,     0,    29,
      30,   178,   179,   180,   181,   182,   183,     0,    31,    32,
       0,     0,     0,   184,   185,   186,     0,    33,     0,     0,
     187,     0,   188,   189,   190,   191,   192,   193,   194,    34,
       0,     0,     0,     0,    35,    83,    36,    37,     0,    38,
       1,     2,     3,     4,     0,     5,     6,     7,     0,     0,
       0,     0,     0,     0,     8,     9,    10,    11,    12,     0,
       0,    13,    14,    15,    16,    17,     0,     0,     0,    18,
      19,    20,    21,    22,    23,    24,    25,     0,    26,    27,
      28,     0,    29,    30,   178,   179,   180,   181,     0,     0,
       0,    31,    32,     0,     0,     0,   184,   185,   186,     0,
      33,     0,     0,     0,     0,   188,   189,   190,   191,   192,
     193,   194,    34,     0,     0,     0,     0,    35,   158,    36,
      37,     0,    38,     1,     2,     3,     4,     0,     5,     6,
       7,     0,     0,     0,     0,     0,     0,     8,     9,    10,
      11,    12,     0,     0,    13,    14,    15,    16,    17,     0,
       0,     0,    18,    19,    20,    21,    22,    23,    24,    25,
       0,    26,    27,    28,     0,    29,    30,     0,     0,     0,
       0,     0,     0,     0,    31,    32,     7,    86,    87,    88,
      89,    90,    91,    33,     0,     0,     0,     0,    92,    93,
       0,     0,     0,     0,     0,    34,    94,    95,     0,     0,
      35,     0,    36,    37,     0,    38,   123,     0,     0,    96,
      97,    29,    30,     0,     0,     0,     0,     0,     0,     0,
      98,    99,     0,     0,     0,     0,     0,     0,     0,    33,
       0,     0,     7,    86,    87,    88,    89,    90,    91,   100,
       0,    34,   124,     0,    92,    93,     0,     0,     0,   102,
       0,    38,    94,    95,     0,     0,     0,     0,     0,     0,
       0,     0,   208,     0,     0,    96,    97,    29,    30,     0,
       0,     0,     0,     0,     0,     0,    98,    99,     0,     0,
       0,     0,     0,     0,     0,    33,     0,     0,     7,    86,
      87,    88,    89,    90,    91,   100,     0,    34,   101,     0,
      92,    93,     0,     0,     0,   102,     0,    38,    94,    95,
       0,     0,     0,     0,     0,     0,     0,     0,   211,     0,
       0,    96,    97,    29,    30,     0,     0,     0,     0,     0,
       0,     0,    98,    99,     0,     0,     0,     0,     0,     0,
       0,    33,     0,     0,     7,    86,    87,    88,    89,    90,
      91,   100,     0,    34,   101,     0,    92,    93,     0,     0,
       0,   102,     0,    38,    94,    95,     0,     0,     0,     0,
       7,    86,    87,    88,    89,    90,    91,    96,    97,    29,
      30,     0,    92,    93,     0,     0,     0,     0,    98,    99,
      94,    95,     0,     0,     0,     0,     0,    33,     0,     0,
       0,     0,     0,   148,   149,    29,    30,   100,     0,    34,
     128,     0,     0,     0,    98,    99,     0,   102,     0,    38,
       0,     0,     0,    33,     0,     0,     7,    86,    87,    88,
      89,    90,    91,   100,     0,    34,   101,     0,    92,    93,
       0,     0,     0,   102,     0,    38,    94,    95,     0,     0,
       0,     0,     7,    86,    87,    88,    89,    90,    91,   152,
      97,    29,    30,     0,    92,    93,     0,     0,     0,     0,
      98,    99,    94,    95,     0,     0,     0,     0,     0,    33,
       0,     0,     0,     0,     0,   154,    97,    29,    30,   100,
       0,    34,   101,     0,     0,     0,    98,    99,     0,   102,
       0,    38,     0,     0,     0,    33,     0,     0,     7,    86,
      87,    88,    89,    90,    91,   100,     0,    34,   101,     0,
      92,    93,     0,     0,     0,   102,     0,    38,    94,    95,
       0,     0,     0,     0,     7,    86,    87,    88,    89,    90,
      91,    96,   174,    29,    30,     0,    92,    93,     0,     0,
       0,     0,    98,    99,    94,    95,     0,     0,     0,     0,
       0,    33,     0,     0,     0,     0,     0,    96,   273,    29,
      30,   100,     0,    34,   101,     0,     0,     0,    98,    99,
       0,   102,     0,    38,     0,     0,     0,    33,     0,     0,
       7,    86,    87,    88,    89,    90,    91,   100,     0,    34,
     101,     0,    92,    93,     0,     0,     0,   102,     0,    38,
      94,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,   308,    29,    30,     0,     0,     0,
       0,     0,     0,     0,    98,    99,     0,     0,     0,     0,
       0,     0,     0,    33,     0,   178,   179,   180,   181,   182,
     183,     0,     0,   100,     0,    34,   101,   184,   185,   186,
       0,     0,     0,   102,   187,    38,   188,   189,   190,   191,
     192,   193,   194,     0,     0,     0,     0,     0,     0,     0,
       0,   245
};

static const short yycheck[] =
{
       0,    54,    65,    49,    50,     5,     0,    72,    73,    74,
      75,     5,     0,    82,    65,    10,    62,     5,    66,    83,
      65,    84,    82,    69,    70,    71,    72,    73,    74,    75,
      65,    31,    32,    84,    83,    35,    84,    31,    32,    84,
      40,    35,    10,    31,    32,    66,    40,    35,    43,    84,
      45,    46,    40,    66,    54,    83,    68,    68,     5,    68,
      54,    83,    68,    84,    64,    68,    54,    68,    63,    83,
      64,    84,     0,    85,    85,    43,    85,    45,    46,    85,
      75,    83,    85,    82,    85,    85,    83,    83,    35,     9,
      85,    85,    74,    75,    83,    63,    83,    85,    98,    99,
      83,    85,    83,    83,    98,    99,    83,    75,    83,    83,
      98,    99,    83,    83,    83,    83,    83,    85,    10,    54,
      55,    56,    57,    58,    59,    17,    18,    19,    20,    21,
      83,    66,    24,    25,    26,    27,    28,    83,    85,    83,
      32,    33,    34,    35,    36,    37,    38,    39,    84,    41,
      42,    43,   205,    45,    46,    48,    49,    50,    51,    52,
      53,    43,    54,    55,    66,    84,    84,    60,    61,    62,
      84,    63,    84,    84,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    75,    56,    57,    58,    59,    84,    83,
      45,    83,    83,    85,    66,    71,    72,    73,    74,    75,
      83,    66,    83,    83,    83,   205,    83,    66,    83,   209,
      83,   205,   212,    83,    45,   209,    65,   205,   212,    82,
      84,   209,    83,    83,   212,    65,    84,    84,    84,   229,
     230,    65,    84,    84,    84,   229,   230,    65,    68,    65,
      65,   229,   230,    65,    45,    84,    84,    84,    84,    45,
      45,    84,   305,    43,    83,    65,    83,    65,    65,    45,
      84,    84,   209,    84,    84,   212,    84,    58,    59,    60,
      61,    62,    84,    45,    65,    84,    67,    68,    66,    82,
      84,     7,   229,   230,    75,    76,    77,    78,    44,    65,
      84,    84,    43,    47,    47,    84,    65,    84,    47,    84,
      84,    84,    84,    84,    84,   305,    84,    84,    65,    84,
     310,   305,     7,    84,    84,     7,   310,   305,    84,    84,
      84,    65,   310,    65,    84,    40,   305,    -1,   328,   329,
      -1,    -1,    -1,    -1,   328,   329,    -1,    -1,    -1,    -1,
     328,   329,   342,    -1,    -1,   345,   346,   347,   342,    -1,
      -1,   345,   346,   347,   342,   355,   356,   345,   346,   347,
      -1,   355,   356,   310,    -1,    -1,    -1,   355,   356,   160,
      -1,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
      -1,   328,   329,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   342,    -1,    -1,   345,   346,
     347,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   355,   356,
      -1,    -1,    -1,    -1,    -1,   206,   207,    -1,    -1,   210,
      -1,    -1,   213,    -1,    -1,   216,    -1,    -1,   219,    -1,
     221,   222,    10,    11,    12,    13,    14,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    23,   100,   101,   102,    -1,
      -1,    -1,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,   116,   117,   118,    43,    44,    45,    46,    -1,
     124,    -1,    -1,    -1,   128,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,   275,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    -1,    75,    76,    -1,
      -1,   292,    -1,    -1,   295,    83,    -1,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   313,   314,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,   340,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   349,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      -1,    -1,    -1,   364,    -1,   366,    60,    61,    62,    -1,
      -1,    -1,    -1,    67,    -1,    69,    70,    71,    72,    73,
      74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   265,   266,   267,   268,   269,    -1,     3,     4,     5,
       6,    -1,     8,     9,    10,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    -1,    -1,    24,    25,
      26,    27,    28,    -1,    -1,   299,    32,    33,    34,    35,
      36,    37,    38,    39,    -1,    41,    42,    43,    -1,    45,
      46,    48,    49,    50,    51,    52,    53,    -1,    54,    55,
      -1,    -1,    -1,    60,    61,    62,    -1,    63,    -1,    -1,
      67,    -1,    69,    70,    71,    72,    73,    74,    75,    75,
      -1,    -1,    -1,    -1,    80,    81,    82,    83,    -1,    85,
       3,     4,     5,     6,    -1,     8,     9,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    19,    20,    21,    -1,
      -1,    24,    25,    26,    27,    28,    -1,    -1,    -1,    32,
      33,    34,    35,    36,    37,    38,    39,    -1,    41,    42,
      43,    -1,    45,    46,    48,    49,    50,    51,    -1,    -1,
      -1,    54,    55,    -1,    -1,    -1,    60,    61,    62,    -1,
      63,    -1,    -1,    -1,    -1,    69,    70,    71,    72,    73,
      74,    75,    75,    -1,    -1,    -1,    -1,    80,    81,    82,
      83,    -1,    85,     3,     4,     5,     6,    -1,     8,     9,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    19,
      20,    21,    -1,    -1,    24,    25,    26,    27,    28,    -1,
      -1,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
      -1,    41,    42,    43,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    10,    11,    12,    13,
      14,    15,    16,    63,    -1,    -1,    -1,    -1,    22,    23,
      -1,    -1,    -1,    -1,    -1,    75,    30,    31,    -1,    -1,
      80,    -1,    82,    83,    -1,    85,    40,    -1,    -1,    43,
      44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    73,
      -1,    75,    76,    -1,    22,    23,    -1,    -1,    -1,    83,
      -1,    85,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    43,    44,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    73,    -1,    75,    76,    -1,
      22,    23,    -1,    -1,    -1,    83,    -1,    85,    30,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    43,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    73,    -1,    75,    76,    -1,    22,    23,    -1,    -1,
      -1,    83,    -1,    85,    30,    31,    -1,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    43,    44,    45,
      46,    -1,    22,    23,    -1,    -1,    -1,    -1,    54,    55,
      30,    31,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    43,    44,    45,    46,    73,    -1,    75,
      76,    -1,    -1,    -1,    54,    55,    -1,    83,    -1,    85,
      -1,    -1,    -1,    63,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    73,    -1,    75,    76,    -1,    22,    23,
      -1,    -1,    -1,    83,    -1,    85,    30,    31,    -1,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    43,
      44,    45,    46,    -1,    22,    23,    -1,    -1,    -1,    -1,
      54,    55,    30,    31,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    43,    44,    45,    46,    73,
      -1,    75,    76,    -1,    -1,    -1,    54,    55,    -1,    83,
      -1,    85,    -1,    -1,    -1,    63,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    73,    -1,    75,    76,    -1,
      22,    23,    -1,    -1,    -1,    83,    -1,    85,    30,    31,
      -1,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    43,    44,    45,    46,    -1,    22,    23,    -1,    -1,
      -1,    -1,    54,    55,    30,    31,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    43,    44,    45,
      46,    73,    -1,    75,    76,    -1,    -1,    -1,    54,    55,
      -1,    83,    -1,    85,    -1,    -1,    -1,    63,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    73,    -1,    75,
      76,    -1,    22,    23,    -1,    -1,    -1,    83,    -1,    85,
      30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    43,    44,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    48,    49,    50,    51,    52,
      53,    -1,    -1,    73,    -1,    75,    76,    60,    61,    62,
      -1,    -1,    -1,    83,    67,    85,    69,    70,    71,    72,
      73,    74,    75,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     4,     5,     6,     8,     9,    10,    17,    18,
      19,    20,    21,    24,    25,    26,    27,    28,    32,    33,
      34,    35,    36,    37,    38,    39,    41,    42,    43,    45,
      46,    54,    55,    63,    75,    80,    82,    83,    85,    87,
      88,    89,    90,    93,    95,    97,    98,    99,   101,   103,
     104,   105,   106,    82,    83,    82,    83,    90,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,   104,
     105,   104,   105,    81,    90,    92,    11,    12,    13,    14,
      15,    16,    22,    23,    30,    31,    43,    44,    54,    55,
      73,    76,    83,   100,   102,   103,   104,   105,     0,    89,
      68,    85,    54,    55,    56,    57,    58,    59,    66,    82,
      91,    96,   106,    40,    76,   102,   107,     9,    76,   107,
     107,   107,   107,   107,    84,    43,   103,   104,   107,    43,
     107,   107,    84,    84,    84,    84,    84,    84,    43,    44,
     107,   107,    43,   107,    43,   107,    85,    85,    81,    90,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
     104,   105,   104,   105,    44,   102,   102,   102,    48,    49,
      50,    51,    52,    53,    60,    61,    62,    67,    69,    70,
      71,    72,    73,    74,    75,    84,    85,    66,    45,    45,
     102,   102,   102,   102,   102,    65,    82,    83,    40,    84,
      83,    40,    84,    65,    84,    84,    65,    84,    84,    65,
      65,    65,    65,    84,    84,    84,    84,    84,    84,    84,
      84,    45,    45,   107,    84,   107,   107,   107,   107,   107,
     107,   107,   107,    85,    85,    84,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,    45,   102,    56,    57,    58,    59,    66,
     106,    94,   107,    44,   107,    83,    90,   107,    83,    90,
     107,   107,   107,    43,   107,   107,    90,    90,    84,    84,
      84,    84,    65,    84,    84,    65,    65,    45,    45,    68,
     102,   102,   102,   102,   102,    82,    84,    84,    44,   107,
       7,    84,    44,    65,    65,    84,    65,    84,    84,    65,
      84,    65,    84,   107,   107,    43,   102,    96,    84,    84,
      84,    84,    90,    84,   107,   107,    47,    47,    47,    84,
      65,    84,    84,    90,    90,    84,    84,    84,    84,    65,
      84,    84,    84,   107,    90,     7,     7,    90,    90,    90,
     107,    84,    90,    90,    65,   107,    65,   107,    84
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


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
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
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
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
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
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

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
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
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

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
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
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
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

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
#line 145 "swf4compiler.y"
    { *((Buffer *)buffer) = yyvsp[0].action; ;}
    break;

  case 4:
#line 152 "swf4compiler.y"
    { bufferConcat(yyvsp[-1].action, yyvsp[0].action); ;}
    break;

  case 6:
#line 160 "swf4compiler.y"
    { yyval.action = NULL; ;}
    break;

  case 7:
#line 161 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action; ;}
    break;

  case 8:
#line 162 "swf4compiler.y"
    { yyval.action = NULL; ;}
    break;

  case 15:
#line 172 "swf4compiler.y"
    { bufferConcat(yyvsp[-2].action, yyvsp[0].action); ;}
    break;

  case 16:
#line 176 "swf4compiler.y"
    { yyval.action = NULL; ;}
    break;

  case 18:
#line 179 "swf4compiler.y"
    { bufferConcat(yyvsp[-1].action, yyvsp[0].action); ;}
    break;

  case 19:
#line 186 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_WAITFORFRAME);
		  bufferWriteS16(yyval.action, 3);
		  bufferWriteS16(yyval.action, atoi(yyvsp[-5].str));
		  free(yyvsp[-5].str);
		  bufferWriteU8(yyval.action, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, bufferLength(yyvsp[0].action)+5);
		  bufferConcat(yyval.action, yyvsp[0].action);			  /* ..here */
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, bufferLength(yyvsp[-2].action));
		  bufferConcat(yyval.action, yyvsp[-2].action); ;}
    break;

  case 20:
#line 202 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_WAITFORFRAME);
		  bufferWriteS16(yyval.action, 3);
		  bufferWriteS16(yyval.action, atoi(yyvsp[-3].str));
		  free(yyvsp[-3].str);
		  bufferWriteU8(yyval.action, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, 5);
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);	  /* ..here */
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, bufferLength(yyvsp[0].action));	  /* ..and then out */
		  bufferConcat(yyval.action, yyvsp[0].action); ;}
    break;

  case 21:
#line 218 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_WAITFORFRAME);
		  bufferWriteS16(yyval.action, 3);
		  bufferWriteS16(yyval.action, atoi(yyvsp[-3].str));
		  free(yyvsp[-3].str);
		  bufferWriteU8(yyval.action, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, bufferLength(yyvsp[0].action));
		  bufferConcat(yyval.action, yyvsp[0].action); ;}
    break;

  case 22:
#line 230 "swf4compiler.y"
    { yyval.action = yyvsp[-5].action;
		  bufferWriteU8(yyval.action, SWFACTION_WAITFORFRAMEEXPRESSION);
		  bufferWriteS16(yyval.action, 1);
		  bufferWriteU8(yyval.action, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, bufferLength(yyvsp[0].action)+5);
		  bufferConcat(yyval.action, yyvsp[0].action);			  /* ..here */
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, bufferLength(yyvsp[-2].action));
		  bufferConcat(yyval.action, yyvsp[-2].action); ;}
    break;

  case 23:
#line 244 "swf4compiler.y"
    { yyval.action = yyvsp[-3].action;
		  bufferWriteU8(yyval.action, SWFACTION_WAITFORFRAMEEXPRESSION);
		  bufferWriteS16(yyval.action, 1);
		  bufferWriteU8(yyval.action, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, 5);
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);	  /* ..here */
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, bufferLength(yyvsp[0].action));	  /* ..and then out */
		  bufferConcat(yyval.action, yyvsp[0].action); ;}
    break;

  case 24:
#line 258 "swf4compiler.y"
    { yyval.action = yyvsp[-3].action;
		  bufferWriteU8(yyval.action, SWFACTION_WAITFORFRAMEEXPRESSION);
		  bufferWriteS16(yyval.action, 1);
		  bufferWriteU8(yyval.action, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, bufferLength(yyvsp[0].action));
		  bufferConcat(yyval.action, yyvsp[0].action); ;}
    break;

  case 25:
#line 268 "swf4compiler.y"
    { bufferWriteU8(yyvsp[-4].action, SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16(yyvsp[-4].action, 2);
		  bufferWriteS16(yyvsp[-4].action, bufferLength(yyvsp[0].action)+5);
		  bufferConcat(yyvsp[-4].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-4].action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyvsp[-4].action, 2);
		  bufferWriteS16(yyvsp[-4].action, bufferLength(yyvsp[-2].action));
		  bufferConcat(yyvsp[-4].action, yyvsp[-2].action);
		  yyval.action = yyvsp[-4].action; ;}
    break;

  case 26:
#line 279 "swf4compiler.y"
    { bufferWriteU8(yyvsp[-2].action, SWFACTION_LOGICALNOT);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16(yyvsp[-2].action, 2);
		  bufferWriteS16(yyvsp[-2].action, bufferLength(yyvsp[0].action));
		  bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  yyval.action = yyvsp[-2].action; ;}
    break;

  case 27:
#line 288 "swf4compiler.y"
    { yyval.action = NULL; ;}
    break;

  case 28:
#line 289 "swf4compiler.y"
    { yyval.action = yyvsp[0].action; ;}
    break;

  case 29:
#line 314 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_WAITFORFRAME);
		  bufferWriteS16(yyval.action, 3);
		  bufferWriteS16(yyval.action, atoi(yyvsp[-3].str));
		  free(yyvsp[-3].str);
		  bufferWriteU8(yyval.action, 1);		/* if not loaded, jump to.. */
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, bufferLength(yyvsp[0].action)+5);
		  bufferConcat(yyval.action, yyvsp[0].action);				  /* ..here */
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, -(bufferLength(yyval.action)+2)); ;}
    break;

  case 30:
#line 329 "swf4compiler.y"
    { yyval.action = yyvsp[-2].action;
		  bufferWriteU8(yyval.action, SWFACTION_LOGICALNOT);
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, bufferLength(yyvsp[0].action)+5);
		  bufferConcat(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, -(bufferLength(yyval.action)+2));
		  bufferResolveJumps(yyval.action); ;}
    break;

  case 31:
#line 341 "swf4compiler.y"
    { yyval.action = yyvsp[-4].action;
		  bufferConcat(yyval.action, yyvsp[-1].action);
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, -(bufferLength(yyval.action)+2));
		  bufferResolveJumps(yyval.action); ;}
    break;

  case 32:
#line 349 "swf4compiler.y"
    { if (!yyvsp[-4].action)
                    yyvsp[-4].action = newBuffer();
                  else {
                    bufferWriteU8(yyvsp[-4].action, SWFACTION_LOGICALNOT);
                    bufferWriteU8(yyvsp[-4].action, SWFACTION_BRANCHIFTRUE);
                    bufferWriteS16(yyvsp[-4].action, 2);
                    bufferWriteS16(yyvsp[-4].action, bufferLength(yyvsp[0].action)+bufferLength(yyvsp[-2].action)+5);
                  }
                  bufferConcat(yyvsp[-4].action, yyvsp[0].action);
                  bufferConcat(yyvsp[-4].action, yyvsp[-2].action);
                  bufferWriteU8(yyvsp[-4].action, SWFACTION_BRANCHALWAYS);
                  bufferWriteS16(yyvsp[-4].action, 2);
                  bufferWriteS16(yyvsp[-4].action, -(bufferLength(yyvsp[-4].action)+2));
                  bufferResolveJumps(yyvsp[-4].action);
                  yyval.action = yyvsp[-6].action;
                  if(!yyval.action) yyval.action = newBuffer();
                  bufferConcat(yyval.action, yyvsp[-4].action);
                ;}
    break;

  case 33:
#line 370 "swf4compiler.y"
    { yyval.action = NULL; ;}
    break;

  case 35:
#line 376 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, MAGIC_CONTINUE_NUMBER); ;}
    break;

  case 36:
#line 384 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, MAGIC_BREAK_NUMBER); ;}
    break;

  case 37:
#line 392 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_STOPDRAGMOVIE); ;}
    break;

  case 38:
#line 396 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[-1].str, strlen(yyvsp[-1].str)+1);
		  bufferWriteU8(yyval.action, SWFACTION_CALLFRAME);
		  bufferWriteS16(yyval.action, 0);
		  free(yyvsp[-1].str); ;}
    break;

  case 39:
#line 403 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[-1].str, strlen(yyvsp[-1].str)+1);
		  bufferWriteU8(yyval.action, SWFACTION_CALLFRAME);
		  bufferWriteS16(yyval.action, 0);
		  free(yyvsp[-1].str); ;}
    break;

  case 40:
#line 410 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action;
		  bufferWriteU8(yyval.action, SWFACTION_REMOVECLIP); ;}
    break;

  case 41:
#line 414 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action;
		  bufferWriteU8(yyval.action, SWFACTION_TRACE); ;}
    break;

  case 42:
#line 419 "swf4compiler.y"
    { yyval.action = yyvsp[-3].action;
		  bufferConcat(yyval.action, yyvsp[-1].action);
		  bufferWriteU8(yyval.action, SWFACTION_GETURL2);
		  bufferWriteS16(yyval.action, 1);
		  bufferWriteU8(yyval.action, GETURL_METHOD_NOSEND); ;}
    break;

  case 43:
#line 426 "swf4compiler.y"
    { yyval.action = yyvsp[-5].action;
		  bufferConcat(yyval.action, yyvsp[-3].action);
		  bufferWriteU8(yyval.action, SWFACTION_GETURL2);
		  bufferWriteS16(yyval.action, 1);
		  bufferWriteU8(yyval.action, yyvsp[-1].getURLMethod); ;}
    break;

  case 44:
#line 433 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_GETURL);
		  bufferWriteS16(yyval.action, strlen(yyvsp[-3].str) + strlen(yyvsp[-1].str) + 2);
		  bufferWriteHardString(yyval.action, (byte*)yyvsp[-3].str, strlen(yyvsp[-3].str));
		  bufferWriteU8(yyval.action, 0);
		  bufferWriteHardString(yyval.action, (byte*)yyvsp[-1].str, strlen(yyvsp[-1].str));
		  bufferWriteU8(yyval.action, 0); ;}
    break;

  case 45:
#line 442 "swf4compiler.y"
    { yyval.action = yyvsp[-3].action;
		  bufferConcat(yyval.action, yyvsp[-1].action);
		  bufferWriteU8(yyval.action, SWFACTION_GETURL2);
		  bufferWriteS16(yyval.action, 1);
		  bufferWriteU8(yyval.action, GETURL_METHOD_NOSEND | GETURL_LOADMOVIE); ;}
    break;

  case 46:
#line 449 "swf4compiler.y"
    { yyval.action = yyvsp[-5].action;
		  bufferConcat(yyval.action, yyvsp[-3].action);
		  bufferWriteU8(yyval.action, SWFACTION_GETURL2);
		  bufferWriteS16(yyval.action, 1);
		  bufferWriteU8(yyval.action, yyvsp[-1].getURLMethod | GETURL_LOADMOVIE); ;}
    break;

  case 47:
#line 456 "swf4compiler.y"
    { yyval.action = yyvsp[-3].action;
		  bufferConcat(yyval.action, yyvsp[-1].action);
		  bufferWriteU8(yyval.action, SWFACTION_GETURL2);
		  bufferWriteS16(yyval.action, 1);
		  bufferWriteU8(yyval.action, GETURL_METHOD_NOSEND | GETURL_LOADVARIABLES); ;}
    break;

  case 48:
#line 463 "swf4compiler.y"
    { yyval.action = yyvsp[-5].action;
		  bufferConcat(yyval.action, yyvsp[-3].action);
		  bufferWriteU8(yyval.action, SWFACTION_GETURL2);
		  bufferWriteS16(yyval.action, 1);
		  bufferWriteU8(yyval.action, yyvsp[-1].getURLMethod | GETURL_LOADVARIABLES); ;}
    break;

  case 49:
#line 471 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, "0", 2); /* no constraint */
		  bufferConcat(yyval.action, yyvsp[-1].action);
		  bufferConcat(yyval.action, yyvsp[-3].action);
		  bufferWriteU8(yyval.action, SWFACTION_STARTDRAGMOVIE); ;}
    break;

  case 50:
#line 478 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferConcat(yyval.action, yyvsp[-7].action);
		  bufferConcat(yyval.action, yyvsp[-3].action);
		  bufferConcat(yyval.action, yyvsp[-5].action);
		  bufferConcat(yyval.action, yyvsp[-1].action);
		  bufferWriteString(yyval.action, "1", 2); /* has constraint */
		  bufferConcat(yyval.action, yyvsp[-9].action);
		  bufferConcat(yyval.action, yyvsp[-11].action);
		  bufferWriteU8(yyval.action, SWFACTION_STARTDRAGMOVIE); ;}
    break;

  case 51:
#line 490 "swf4compiler.y"
    { yyval.action = yyvsp[-5].action;
		  bufferConcat(yyval.action, yyvsp[-3].action);
		  bufferConcat(yyval.action, yyvsp[-1].action);
		  bufferWriteWTHITProperty(yyval.action);
		  bufferWriteU8(yyval.action, SWFACTION_ADD); /* see docs for explanation */
		  bufferWriteU8(yyval.action, SWFACTION_DUPLICATECLIP); ;}
    break;

  case 52:
#line 499 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_NEXTFRAME); ;}
    break;

  case 53:
#line 503 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_PREVFRAME); ;}
    break;

  case 54:
#line 507 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_PLAY); ;}
    break;

  case 55:
#line 511 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_STOP); ;}
    break;

  case 56:
#line 515 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_TOGGLEQUALITY); ;}
    break;

  case 57:
#line 519 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_STOPSOUNDS); ;}
    break;

  case 58:
#line 523 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_GOTOFRAME);
		  bufferWriteS16(yyval.action, 2);
		  bufferWriteS16(yyval.action, atoi(yyvsp[-1].str));
		  free(yyvsp[-1].str); ;}
    break;

  case 59:
#line 530 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_GOTOLABEL);
		  bufferWriteS16(yyval.action, strlen(yyvsp[-1].str)+1);
		  bufferWriteHardString(yyval.action, (byte*)yyvsp[-1].str, strlen(yyvsp[-1].str)+1);
		  free(yyvsp[-1].str); ;}
    break;

  case 60:
#line 537 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action;
		  bufferWriteU8(yyval.action, SWFACTION_GOTOEXPRESSION);
		  bufferWriteS16(yyval.action, 1);
		  bufferWriteU8(yyval.action, 0); ;}
    break;

  case 61:
#line 543 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action;
		  bufferWriteU8(yyval.action, SWFACTION_GOTOEXPRESSION);
		  bufferWriteS16(yyval.action, 1);
		  bufferWriteU8(yyval.action, 1); ;}
    break;

  case 62:
#line 549 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_SETTARGET);
		  bufferWriteS16(yyval.action, strlen(yyvsp[-1].str)+1);
		  bufferWriteHardString(yyval.action, (byte*)yyvsp[-1].str, strlen(yyvsp[-1].str)+1);
		  free(yyvsp[-1].str); ;}
    break;

  case 63:
#line 556 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action;
		  bufferWriteU8(yyval.action, SWFACTION_SETTARGETEXPRESSION); ;}
    break;

  case 64:
#line 560 "swf4compiler.y"
    { yyval.action = newBuffer();
			/* SetTarget(STRING) */
		  bufferWriteU8(yyval.action, SWFACTION_SETTARGET);
		  bufferWriteS16(yyval.action, strlen(yyvsp[-2].str)+1);
		  bufferWriteHardString(yyval.action, (byte*)yyvsp[-2].str, strlen(yyvsp[-2].str)+1);
			/* stmt */
		  bufferConcat(yyval.action, yyvsp[0].action);
			/* SetTarget('') */
		  bufferWriteU8(yyval.action, SWFACTION_SETTARGET);
		  bufferWriteS16(yyval.action, 1);
		  bufferWriteU8(yyval.action, 0);
		  free(yyvsp[-2].str); ;}
    break;

  case 65:
#line 574 "swf4compiler.y"
    { yyval.action = yyvsp[-2].action;
			/* SetTarget(expr) */
		  bufferWriteU8(yyval.action, SWFACTION_SETTARGETEXPRESSION); 
			/* stmt */
		  bufferConcat(yyval.action, yyvsp[0].action);
			/* SetTarget('') */
		  bufferWriteU8(yyval.action, SWFACTION_SETTARGET);
		  bufferWriteS16(yyval.action, 1);
		  bufferWriteU8(yyval.action, 0); ;}
    break;

  case 66:
#line 587 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action;
		  bufferWriteU8(yyval.action, SWFACTION_GETVARIABLE); ;}
    break;

  case 67:
#line 591 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteU8(yyval.action, SWFACTION_GETTIMER); ;}
    break;

  case 68:
#line 595 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action;
		  bufferWriteU8(yyval.action, SWFACTION_RANDOM); ;}
    break;

  case 69:
#line 599 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action;
		  bufferWriteU8(yyval.action, SWFACTION_STRINGLENGTH); ;}
    break;

  case 70:
#line 603 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action;
		  bufferWriteU8(yyval.action, SWFACTION_INT); ;}
    break;

  case 71:
#line 607 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action;
		  bufferWriteU8(yyval.action, SWFACTION_ORD); ;}
    break;

  case 72:
#line 611 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action;
		  bufferWriteU8(yyval.action, SWFACTION_CHR); ;}
    break;

  case 73:
#line 615 "swf4compiler.y"
    { yyval.action = yyvsp[-3].action;
		  bufferConcat(yyval.action, yyvsp[-1].action);
		  bufferWriteU8(yyval.action, SWFACTION_STRINGCONCAT); ;}
    break;

  case 74:
#line 620 "swf4compiler.y"
    { yyval.action = yyvsp[-5].action;
		  bufferConcat(yyval.action, yyvsp[-3].action);
		  bufferConcat(yyval.action, yyvsp[-1].action);
		  bufferWriteU8(yyval.action, SWFACTION_SUBSTRING); ;}
    break;

  case 75:
#line 626 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferConcat(yyval.action, yyvsp[-3].action);
		  bufferWriteGetProperty(yyval.action, yyvsp[-1].str);
		  bufferWriteU8(yyval.action, SWFACTION_GETPROPERTY);
		  free(yyvsp[-1].str); ;}
    break;

  case 76:
#line 635 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteBuffer(yyval.action, yyvsp[-1].action);
		  bufferWriteU8(yyval.action, SWFACTION_GETVARIABLE);
		  bufferWriteBuffer(yyval.action, yyvsp[-1].action);
		  bufferConcat(yyval.action, yyvsp[-1].action);
		  bufferWriteU8(yyval.action, SWFACTION_GETVARIABLE);
		  bufferWriteString(yyval.action, "1", 2);
		  bufferWriteU8(yyval.action, SWFACTION_ADD);
		  bufferWriteU8(yyval.action, SWFACTION_SETVARIABLE); ;}
    break;

  case 77:
#line 646 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteBuffer(yyval.action, yyvsp[-1].action);
		  bufferWriteU8(yyval.action, SWFACTION_GETVARIABLE);
		  bufferWriteBuffer(yyval.action, yyvsp[-1].action);
		  bufferConcat(yyval.action, yyvsp[-1].action);
		  bufferWriteU8(yyval.action, SWFACTION_GETVARIABLE);
		  bufferWriteString(yyval.action, "1", 2);
		  bufferWriteU8(yyval.action, SWFACTION_SUBTRACT);
		  bufferWriteU8(yyval.action, SWFACTION_SETVARIABLE); ;}
    break;

  case 79:
#line 662 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action; ;}
    break;

  case 80:
#line 665 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[0].str, strlen(yyvsp[0].str)+1);
		  free(yyvsp[0].str); ;}
    break;

  case 81:
#line 670 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, "-", 2);
		  bufferWriteString(yyval.action, yyvsp[0].str, strlen(yyvsp[0].str)+1);
		  free(yyvsp[0].str); ;}
    break;

  case 82:
#line 676 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[0].str, strlen(yyvsp[0].str)+1);
		  free(yyvsp[0].str); ;}
    break;

  case 83:
#line 681 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[0].str, strlen(yyvsp[0].str)+1);
		  bufferWriteU8(yyval.action, SWFACTION_GETVARIABLE);
		  free(yyvsp[0].str); ;}
    break;

  case 84:
#line 687 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[0].str, strlen(yyvsp[0].str)+1);
		  free(yyvsp[0].str); ;}
    break;

  case 85:
#line 692 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[-2].str, strlen(yyvsp[-2].str)+1);
		  bufferWriteGetProperty(yyval.action, yyvsp[0].str);
		  bufferWriteU8(yyval.action, SWFACTION_GETPROPERTY);
		  free(yyvsp[0].str);
		  free(yyvsp[-2].str); ;}
    break;

  case 86:
#line 700 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[-2].str, strlen(yyvsp[-2].str)+1);
		  bufferWriteGetProperty(yyval.action, yyvsp[0].str);
		  bufferWriteString(yyval.action, yyvsp[-2].str, strlen(yyvsp[-2].str)+1);
		  bufferWriteSetProperty(yyval.action, yyvsp[0].str);
		  bufferWriteString(yyval.action, yyvsp[-2].str, strlen(yyvsp[-2].str)+1);
		  bufferWriteGetProperty(yyval.action, yyvsp[0].str);
		  bufferWriteString(yyval.action, "1", 2);
		  bufferWriteU8(yyval.action, SWFACTION_ADD);
		  bufferWriteU8(yyval.action, SWFACTION_SETPROPERTY);
		  bufferWriteU8(yyval.action, SWFACTION_GETPROPERTY);
		  free(yyvsp[-2].str);
		  free(yyvsp[0].str); ;}
    break;

  case 87:
#line 715 "swf4compiler.y"
    { yyval.action = yyvsp[0].action;
		  bufferWriteU8(yyval.action, SWFACTION_DUP);
		  bufferWriteU8(yyval.action, SWFACTION_DUP);
		  bufferWriteU8(yyval.action, SWFACTION_GETVARIABLE);
		  bufferWriteString(yyval.action, "1", 2);
		  bufferWriteU8(yyval.action, SWFACTION_ADD);
		  bufferWriteU8(yyval.action, SWFACTION_SETVARIABLE);
		  bufferWriteU8(yyval.action, SWFACTION_GETVARIABLE); ;}
    break;

  case 88:
#line 725 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[-2].str, strlen(yyvsp[-2].str)+1);
		  bufferWriteGetProperty(yyval.action, yyvsp[0].str);
		  bufferWriteString(yyval.action, yyvsp[-2].str, strlen(yyvsp[-2].str)+1);
		  bufferWriteSetProperty(yyval.action, yyvsp[0].str);
		  bufferWriteString(yyval.action, yyvsp[-2].str, strlen(yyvsp[-2].str)+1);
		  bufferWriteGetProperty(yyval.action, yyvsp[0].str);
		  bufferWriteString(yyval.action, "1", 2);
		  bufferWriteU8(yyval.action, SWFACTION_ADD);
		  bufferWriteU8(yyval.action, SWFACTION_SETPROPERTY);
		  bufferWriteU8(yyval.action, SWFACTION_GETPROPERTY);
		  free(yyvsp[-2].str);
		  free(yyvsp[0].str); ;}
    break;

  case 89:
#line 740 "swf4compiler.y"
    { yyval.action = yyvsp[0].action;
		  bufferWriteU8(yyval.action, SWFACTION_DUP);
		  bufferWriteU8(yyval.action, SWFACTION_DUP);
		  bufferWriteU8(yyval.action, SWFACTION_GETVARIABLE);
		  bufferWriteString(yyval.action, "1", 2);
		  bufferWriteU8(yyval.action, SWFACTION_SUBTRACT);
		  bufferWriteU8(yyval.action, SWFACTION_SETVARIABLE);
		  bufferWriteU8(yyval.action, SWFACTION_GETVARIABLE); ;}
    break;

  case 90:
#line 750 "swf4compiler.y"
    { yyval.action = yyvsp[0].action;
		  bufferWriteString(yyvsp[0].action, "-1", 3);
		  bufferWriteU8(yyvsp[0].action, SWFACTION_MULTIPLY); ;}
    break;

  case 91:
#line 755 "swf4compiler.y"
    { yyval.action = yyvsp[0].action;
		  bufferWriteU8(yyvsp[0].action, SWFACTION_LOGICALNOT); ;}
    break;

  case 92:
#line 759 "swf4compiler.y"
    { yyval.action = yyvsp[-2].action;
		  bufferConcat(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_DUP);
		  bufferWriteU8(yyval.action, SWFACTION_SETVARIABLE); ;}
    break;

  case 93:
#line 765 "swf4compiler.y"
    { yyval.action = yyvsp[-2].action;
		  bufferConcat(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_MULTIPLY); ;}
    break;

  case 94:
#line 770 "swf4compiler.y"
    { yyval.action = yyvsp[-2].action;
		  bufferConcat(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_DIVIDE); ;}
    break;

  case 95:
#line 775 "swf4compiler.y"
    { yyval.action = yyvsp[-2].action;
		  bufferConcat(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_ADD); ;}
    break;

  case 96:
#line 780 "swf4compiler.y"
    { yyval.action = yyvsp[-2].action;
		  bufferConcat(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_SUBTRACT); ;}
    break;

  case 97:
#line 785 "swf4compiler.y"
    { yyval.action = yyvsp[-2].action;
		  bufferConcat(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_STRINGCONCAT); ;}
    break;

  case 98:
#line 790 "swf4compiler.y"
    { yyval.action = yyvsp[-2].action;
		  bufferConcat(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_LESSTHAN); ;}
    break;

  case 99:
#line 795 "swf4compiler.y"
    { yyval.action = yyvsp[0].action;
		  bufferConcat(yyval.action, yyvsp[-2].action);
		  bufferWriteU8(yyval.action, SWFACTION_LESSTHAN); ;}
    break;

  case 100:
#line 800 "swf4compiler.y"
    { yyval.action = yyvsp[0].action;
		  bufferConcat(yyval.action, yyvsp[-2].action);
		  bufferWriteU8(yyval.action, SWFACTION_LESSTHAN);
		  bufferWriteU8(yyval.action, SWFACTION_LOGICALNOT); ;}
    break;

  case 101:
#line 806 "swf4compiler.y"
    { bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_LESSTHAN);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_LOGICALNOT); ;}
    break;

  case 102:
#line 811 "swf4compiler.y"
    { bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_STRINGEQ);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_LOGICALNOT); ;}
    break;

  case 103:
#line 816 "swf4compiler.y"
    { bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_STRINGEQ); ;}
    break;

  case 104:
#line 820 "swf4compiler.y"
    { bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_STRINGCOMPARE); ;}
    break;

  case 105:
#line 824 "swf4compiler.y"
    { bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_EQUAL); ;}
    break;

  case 106:
#line 828 "swf4compiler.y"
    { bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_EQUAL);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_LOGICALNOT); ;}
    break;

  case 107:
#line 833 "swf4compiler.y"
    { bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_LOGICALAND); ;}
    break;

  case 108:
#line 837 "swf4compiler.y"
    { bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_LOGICALOR); ;}
    break;

  case 109:
#line 841 "swf4compiler.y"
    { bufferWriteU8(yyvsp[-4].action, SWFACTION_BRANCHIFTRUE);
		  bufferWriteS16(yyvsp[-4].action, 2);
		  bufferWriteS16(yyvsp[-4].action, bufferLength(yyvsp[0].action)+5);
		  bufferConcat(yyvsp[-4].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-4].action, SWFACTION_BRANCHALWAYS);
		  bufferWriteS16(yyvsp[-4].action, 2);
		  bufferWriteS16(yyvsp[-4].action, bufferLength(yyvsp[-2].action));
		  bufferConcat(yyvsp[-4].action, yyvsp[-2].action); ;}
    break;

  case 111:
#line 855 "swf4compiler.y"
    { yyval.str = yyvsp[-2].str;
		  yyval.str = stringConcat(yyval.str, strdup(":"));
		  yyval.str = stringConcat(yyval.str, yyvsp[0].str); ;}
    break;

  case 112:
#line 862 "swf4compiler.y"
    { yyval.str = strdup(""); ;}
    break;

  case 113:
#line 865 "swf4compiler.y"
    { yyval.str = strdup(""); ;}
    break;

  case 114:
#line 868 "swf4compiler.y"
    { yyval.str = strdup("/"); ;}
    break;

  case 115:
#line 871 "swf4compiler.y"
    { yyval.str = strdup(".."); ;}
    break;

  case 116:
#line 874 "swf4compiler.y"
    { yyval.str = yyvsp[0].str; ;}
    break;

  case 117:
#line 877 "swf4compiler.y"
    { yyval.str = yyvsp[0].str; ;}
    break;

  case 118:
#line 882 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[0].str, strlen(yyvsp[0].str)+1);
		  free(yyvsp[0].str); ;}
    break;

  case 119:
#line 887 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[0].str, strlen(yyvsp[0].str)+1);
		  free(yyvsp[0].str); ;}
    break;

  case 120:
#line 891 "swf4compiler.y"
    { yyval.action = yyvsp[-1].action; ;}
    break;

  case 123:
#line 900 "swf4compiler.y"
    { yyval.action = yyvsp[0].action;
		  bufferWriteBuffer(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_GETVARIABLE);
		  bufferWriteString(yyval.action, "1", 2);
		  bufferWriteU8(yyval.action, SWFACTION_ADD);
		  bufferWriteU8(yyval.action, SWFACTION_SETVARIABLE); ;}
    break;

  case 124:
#line 908 "swf4compiler.y"
    { yyval.action = yyvsp[0].action;
		  bufferWriteBuffer(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_GETVARIABLE);
		  bufferWriteString(yyval.action, "1", 2);
		  bufferWriteU8(yyval.action, SWFACTION_SUBTRACT);
		  bufferWriteU8(yyval.action, SWFACTION_SETVARIABLE); ;}
    break;

  case 125:
#line 916 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[-2].str, strlen(yyvsp[-2].str)+1);
		  bufferWriteGetProperty(yyval.action, yyvsp[0].str);
		  bufferWriteU8(yyval.action, SWFACTION_GETPROPERTY);
		  bufferWriteString(yyval.action, "1", 2);
		  bufferWriteU8(yyval.action, SWFACTION_ADD);
		  bufferWriteString(yyval.action, yyvsp[-2].str, strlen(yyvsp[-2].str)+1);
		  bufferWriteSetProperty(yyval.action, yyvsp[0].str);
		  bufferWriteU8(yyval.action, SWFACTION_SETPROPERTY);
		  free(yyvsp[-2].str);
		  free(yyvsp[0].str); ;}
    break;

  case 126:
#line 929 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[-2].str, strlen(yyvsp[-2].str)+1);
		  bufferWriteGetProperty(yyval.action, yyvsp[0].str);
		  bufferWriteU8(yyval.action, SWFACTION_GETPROPERTY);
		  bufferWriteString(yyval.action, "1", 2);
		  bufferWriteU8(yyval.action, SWFACTION_SUBTRACT);
		  bufferWriteString(yyval.action, yyvsp[-2].str, strlen(yyvsp[-2].str)+1);
		  bufferWriteSetProperty(yyval.action, yyvsp[0].str);
		  bufferWriteU8(yyval.action, SWFACTION_SETPROPERTY);
		  free(yyvsp[-2].str);
		  free(yyvsp[0].str); ;}
    break;

  case 127:
#line 942 "swf4compiler.y"
    { bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_SETVARIABLE); ;}
    break;

  case 128:
#line 946 "swf4compiler.y"
    { bufferWriteBuffer(yyvsp[-2].action, yyvsp[-2].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_GETVARIABLE);
		  bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_MULTIPLY);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_SETVARIABLE); ;}
    break;

  case 129:
#line 953 "swf4compiler.y"
    { bufferWriteBuffer(yyvsp[-2].action, yyvsp[-2].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_GETVARIABLE);
		  bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_DIVIDE);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_SETVARIABLE); ;}
    break;

  case 130:
#line 960 "swf4compiler.y"
    { bufferWriteBuffer(yyvsp[-2].action, yyvsp[-2].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_GETVARIABLE);
		  bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_ADD);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_SETVARIABLE); ;}
    break;

  case 131:
#line 967 "swf4compiler.y"
    { bufferWriteBuffer(yyvsp[-2].action, yyvsp[-2].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_GETVARIABLE);
		  bufferConcat(yyvsp[-2].action, yyvsp[0].action);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_SUBTRACT);
		  bufferWriteU8(yyvsp[-2].action, SWFACTION_SETVARIABLE); ;}
    break;

  case 132:
#line 974 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[-4].str, strlen(yyvsp[-4].str)+1);
		  bufferWriteSetProperty(yyval.action, yyvsp[-2].str);
		  bufferConcat(yyval.action,yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_SETPROPERTY);
		  free(yyvsp[-4].str);
		  free(yyvsp[-2].str); ;}
    break;

  case 133:
#line 983 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[-4].str, strlen(yyvsp[-4].str)+1);
		  bufferWriteSetProperty(yyval.action, yyvsp[-2].str);
		  bufferWriteString(yyval.action, yyvsp[-4].str, strlen(yyvsp[-4].str)+1);
		  bufferWriteGetProperty(yyval.action, yyvsp[-2].str);
		  bufferWriteU8(yyval.action, SWFACTION_GETPROPERTY);
		  bufferConcat(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_MULTIPLY);
		  bufferWriteU8(yyval.action, SWFACTION_SETPROPERTY);
		  free(yyvsp[-4].str);
		  free(yyvsp[-2].str); ;}
    break;

  case 134:
#line 996 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[-4].str, strlen(yyvsp[-4].str)+1);
		  bufferWriteSetProperty(yyval.action, yyvsp[-2].str);
		  bufferWriteString(yyval.action, yyvsp[-4].str, strlen(yyvsp[-4].str)+1);
		  bufferWriteGetProperty(yyval.action, yyvsp[-2].str);
		  bufferWriteU8(yyval.action, SWFACTION_GETPROPERTY);
		  bufferConcat(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_DIVIDE);
		  bufferWriteU8(yyval.action, SWFACTION_SETPROPERTY);
		  free(yyvsp[-4].str);
		  free(yyvsp[-2].str); ;}
    break;

  case 135:
#line 1009 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[-4].str, strlen(yyvsp[-4].str)+1);
		  bufferWriteSetProperty(yyval.action, yyvsp[-2].str);
		  bufferWriteString(yyval.action, yyvsp[-4].str, strlen(yyvsp[-4].str)+1);
		  bufferWriteGetProperty(yyval.action, yyvsp[-2].str);
		  bufferWriteU8(yyval.action, SWFACTION_GETPROPERTY);
		  bufferConcat(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_ADD);
		  bufferWriteU8(yyval.action, SWFACTION_SETPROPERTY);
		  free(yyvsp[-4].str);
		  free(yyvsp[-2].str); ;}
    break;

  case 136:
#line 1022 "swf4compiler.y"
    { yyval.action = newBuffer();
		  bufferWriteString(yyval.action, yyvsp[-4].str, strlen(yyvsp[-4].str)+1);
		  bufferWriteSetProperty(yyval.action, yyvsp[-2].str);
		  bufferWriteString(yyval.action, yyvsp[-4].str, strlen(yyvsp[-4].str)+1);
		  bufferWriteGetProperty(yyval.action, yyvsp[-2].str);
		  bufferWriteU8(yyval.action, SWFACTION_GETPROPERTY);
		  bufferConcat(yyval.action, yyvsp[0].action);
		  bufferWriteU8(yyval.action, SWFACTION_SUBTRACT);
		  bufferWriteU8(yyval.action, SWFACTION_SETPROPERTY);
		  free(yyvsp[-4].str);
		  free(yyvsp[-2].str); ;}
    break;


    }

/* Line 999 of yacc.c.  */
#line 2691 "swf4compiler.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


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
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
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

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


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
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 1039 "swf4compiler.y"


