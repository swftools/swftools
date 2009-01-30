
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
     T_IDENTIFIER = 258,
     T_STRING = 259,
     T_REGEXP = 260,
     T_EMPTY = 261,
     T_INT = 262,
     T_UINT = 263,
     T_BYTE = 264,
     T_SHORT = 265,
     T_FLOAT = 266,
     T_FOR = 267,
     T_WHILE = 268,
     T_DO = 269,
     T_SWITCH = 270,
     KW_IMPLEMENTS = 271,
     KW_NAMESPACE = 272,
     KW_PACKAGE = 273,
     KW_PROTECTED = 274,
     KW_PUBLIC = 275,
     KW_PRIVATE = 276,
     KW_USE = 277,
     KW_INTERNAL = 278,
     KW_NEW = 279,
     KW_NATIVE = 280,
     KW_FUNCTION = 281,
     KW_FINALLY = 282,
     KW_UNDEFINED = 283,
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
     KW_DELETE = 317,
     KW_IF = 318,
     KW_ELSE = 319,
     KW_BREAK = 320,
     KW_IS = 321,
     KW_IN = 322,
     KW_AS = 323,
     T_EQEQ = 324,
     T_EQEQEQ = 325,
     T_NE = 326,
     T_NEE = 327,
     T_LE = 328,
     T_GE = 329,
     T_ORBY = 330,
     T_DIVBY = 331,
     T_MODBY = 332,
     T_MULBY = 333,
     T_PLUSBY = 334,
     T_MINUSBY = 335,
     T_SHRBY = 336,
     T_SHLBY = 337,
     T_USHRBY = 338,
     T_OROR = 339,
     T_ANDAND = 340,
     T_COLONCOLON = 341,
     T_MINUSMINUS = 342,
     T_PLUSPLUS = 343,
     T_DOTDOT = 344,
     T_DOTDOTDOT = 345,
     T_SHL = 346,
     T_USHR = 347,
     T_SHR = 348,
     prec_none = 349,
     below_semicolon = 350,
     below_assignment = 351,
     below_minus = 354,
     minusminus_prefix = 355,
     plusplus_prefix = 356,
     below_curly = 357,
     above_identifier = 358,
     below_else = 359,
     above_function = 360
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union 
/* Line 1685 of skeleton.m4  */
#line 41 "parser.y"
tokenunion
{

/* Line 1685 of skeleton.m4  */
#line 41 "parser.y"

    enum yytokentype token;
    int flags;

    classinfo_t*classinfo;
    classinfo_list_t*classinfo_list;

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
    struct {
        abc_exception_list_t *l;
        code_t*finally;
    } catch_list;



/* Line 1685 of skeleton.m4  */
#line 188 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE avm2_lval;


