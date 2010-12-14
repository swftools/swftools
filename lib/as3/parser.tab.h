
/* A Bison parser, made by GNU Bison 2.4.2.  */

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
     T_IDENTIFIER = 258,
     T_STRING = 259,
     T_REGEXP = 260,
     T_EMPTY = 261,
     T_INT = 262,
     T_UINT = 263,
     T_FLOAT = 264,
     T_FOR = 265,
     T_WHILE = 266,
     T_DO = 267,
     T_SWITCH = 268,
     KW_IMPLEMENTS = 269,
     KW_NAMESPACE = 270,
     KW_PACKAGE = 271,
     KW_PROTECTED = 272,
     KW_ARGUMENTS = 273,
     KW_PUBLIC = 274,
     KW_PRIVATE = 275,
     KW_USE = 276,
     KW_INTERNAL = 277,
     KW_NEW = 278,
     KW_NATIVE = 279,
     KW_FUNCTION = 280,
     KW_FINALLY = 281,
     KW_UNDEFINED = 282,
     KW_NAN = 283,
     KW_CONTINUE = 284,
     KW_CLASS = 285,
     KW_CONST = 286,
     KW_CATCH = 287,
     KW_CASE = 288,
     KW_SET = 289,
     KW_VOID = 290,
     KW_THROW = 291,
     KW_STATIC = 292,
     KW_WITH = 293,
     KW_INSTANCEOF = 294,
     KW_IMPORT = 295,
     KW_RETURN = 296,
     KW_TYPEOF = 297,
     KW_INTERFACE = 298,
     KW_NULL = 299,
     KW_VAR = 300,
     KW_DYNAMIC = 301,
     KW_OVERRIDE = 302,
     KW_FINAL = 303,
     KW_EACH = 304,
     KW_GET = 305,
     KW_TRY = 306,
     KW_SUPER = 307,
     KW_EXTENDS = 308,
     KW_FALSE = 309,
     KW_TRUE = 310,
     KW_BOOLEAN = 311,
     KW_UINT = 312,
     KW_INT = 313,
     KW_NUMBER = 314,
     KW_STRING = 315,
     KW_DEFAULT = 316,
     KW_DEFAULT_XML = 317,
     KW_DELETE = 318,
     KW_IF = 319,
     KW_ELSE = 320,
     KW_BREAK = 321,
     KW_IS = 322,
     KW_IN = 323,
     KW_AS = 324,
     T_DICTSTART = 325,
     T_EQEQ = 326,
     T_EQEQEQ = 327,
     T_NE = 328,
     T_NEE = 329,
     T_LE = 330,
     T_GE = 331,
     T_ORBY = 332,
     T_DIVBY = 333,
     T_MODBY = 334,
     T_MULBY = 335,
     T_ANDBY = 336,
     T_PLUSBY = 337,
     T_MINUSBY = 338,
     T_XORBY = 339,
     T_SHRBY = 340,
     T_SHLBY = 341,
     T_USHRBY = 342,
     T_OROR = 343,
     T_ANDAND = 344,
     T_COLONCOLON = 345,
     T_MINUSMINUS = 346,
     T_PLUSPLUS = 347,
     T_DOTDOT = 348,
     T_DOTDOTDOT = 349,
     T_SHL = 350,
     T_USHR = 351,
     T_SHR = 352,
     prec_none = 353,
     prec_var_read = 354,
     below_semicolon = 355,
     below_assignment = 356,
     below_lt = 357,
     below_minus = 358,
     minusminus_prefix = 359,
     plusplus_prefix = 360,
     below_curly = 361,
     new2 = 362,
     below_identifier = 363,
     above_identifier = 364,
     below_else = 365,
     above_function = 366
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union 
/* Line 1685 of skeleton.m4  */
#line 48 "parser.y"
tokenunion
{

/* Line 1685 of skeleton.m4  */
#line 48 "parser.y"

    enum yytokentype token;

    classinfo_t*classinfo;
    classinfo_list_t*classinfo_list;
    slotinfo_t*slotinfo;
    slotinfo_list_t*slotinfo_list;

    int number_int;
    unsigned int number_uint;
    double number_float;
    code_t*code;
    typedcode_t value;
    //typedcode_list_t*value_list;
    codeandnumber_t value_list;
    param_t* param;
    params_t params;
    string_t str;
    char*id;
    constant_t*constant;
    for_start_t for_start;
    abc_exception_t *exception;
    regexp_t regexp;
    modifiers_t flags;
    namespace_decl_t* namespace_decl;
    node_t*node;
    struct {
        abc_exception_list_t *l;
        code_t*finally;
    } catch_list;



/* Line 1685 of skeleton.m4  */
#line 200 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE a3_lval;


