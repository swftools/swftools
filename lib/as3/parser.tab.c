
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "./skeleton.m4"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         a3_parse
#define yylex           a3_lex
#define yyerror         a3_error
#define yylval          a3_lval
#define yychar          a3_char
#define yydebug         a3_debug
#define yynerrs         a3_nerrs


/* Copy the first part of user declarations.  */

/* Line 198 of skeleton.m4  */
#line 23 "parser.y"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "abc.h"
#include "pool.h"
#include "files.h"
#include "common.h"
#include "tokenizer.h"
#include "registry.h"
#include "code.h"
#include "opcodes.h"
#include "compiler.h"
#include "expr.h"
#include "initcode.h"
#include "parser_help.h"

extern int a3_lex();



/* Line 198 of skeleton.m4  */
#line 103 "parser.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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
     below_semicolon = 354,
     below_assignment = 355,
     below_lt = 356,
     below_minus = 357,
     minusminus_prefix = 358,
     plusplus_prefix = 359,
     below_curly = 360,
     new2 = 361,
     above_identifier = 362,
     below_else = 363,
     above_function = 364
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union 
/* Line 231 of skeleton.m4  */
#line 48 "parser.y"
tokenunion
{

/* Line 231 of skeleton.m4  */
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



/* Line 231 of skeleton.m4  */
#line 285 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 273 of skeleton.m4  */
#line 275 "parser.y"


#define PASS1 }} if(as3_pass == 1) {{
#define PASS1END }} if(as3_pass == 2) {{
#define PASS2 }} if(as3_pass == 2) {{
#define PASS12 }} if(as3_pass == 1 || as3_pass == 2) {{
#define PASS12END }} if(as3_pass == 2) {{
#define PASS_ALWAYS }} {{



/* Line 273 of skeleton.m4  */
#line 309 "parser.tab.c"
/* Unqualified %code blocks.  */

/* Line 274 of skeleton.m4  */
#line 399 "parser.y"

    char is_subtype_of(classinfo_t*type, classinfo_t*supertype)
    {
        return 1; // FIXME
    }
    char do_init_variable(char*name)
    {
        if(!state->method->no_variable_scoping)
            return 0;
        if(!state->new_vars)
            return 1;
        return 1;
    }


/* Line 274 of skeleton.m4  */
#line 849 "parser.y"

    static void state_has_imports()
    {
        state->wildcard_imports = list_clone(state->wildcard_imports);
        state->imports = dict_clone(state->imports);
        state->has_own_imports = 1;
    }
    static void import_toplevel(const char*package)
    {
        char* s = strdup(package);
        while(1) {
            dict_put(state->import_toplevel_packages, s, 0);
            char*x = strrchr(s, '.');
            if(!x)
                break;
            *x = 0;
        }
        free(s);
    }


/* Line 274 of skeleton.m4  */
#line 992 "parser.y"

    static int slotstate_varconst = 0;
    static modifiers_t*slotstate_flags = 0;
    static void setslotstate(modifiers_t* flags, int varconst)
    {
        slotstate_varconst = varconst;
        slotstate_flags = flags;
        if(state->cls) {
            if(flags) {
                if(flags->flags&FLAG_STATIC) {
                    state->method = state->cls->static_init;
                } else {
                    state->method = state->cls->init;
                }
            } else {
                // reset to "default" state (all in class code is static by default) */
                state->method = state->cls->static_init;
            }
        } else {
            as3_assert(state->method);
        }
    }
    static trait_t* add_abc_slot(modifiers_t* modifiers, const char*name, multiname_t*m, code_t***c)
    {
        int flags = modifiers->flags;
        namespace_t ns = modifiers2access(modifiers);

        /* slot name */
        multiname_t mname = {QNAME, &ns, 0, name};
      
        trait_list_t**traits;
        code_t**code=0;
        if(!state->cls) {
            // global variable
            if(!global->init) global->init = abc_initscript(global->file);
            ns.name = state->package;
            traits = &global->init->traits;
            code = &global->init->method->body->code;
        } else if(flags&FLAG_STATIC) {
            // static variable
            traits = &state->cls->abc->static_traits;
            code = &state->cls->static_init->header;
        } else {
            // instance variable
            traits = &state->cls->abc->traits;
            code = &state->cls->init->header;
            
            if(ns.access == ACCESS_PROTECTED) {
                ns.name = concat3(state->cls->info->package,":",state->cls->info->name);
            }
        }
        if(c)
            *c = code;
        if(m) 
            *m = *multiname_clone(&mname);
            
        return trait_new_member(traits, 0, multiname_clone(&mname), 0);
    }


/* Line 274 of skeleton.m4  */
#line 1166 "parser.y"

    static int xml_level = 0;


/* Line 274 of skeleton.m4  */
#line 1833 "parser.y"

    node_t* resolve_identifier(const char*name);
    node_t* get_descendants(node_t*e,const char*ns,const char*subnode,char multi, char attr)
    {
	typedcode_t v = node_read(e);
	typedcode_t w;

	multiname_t m = {0,0,0,subnode};
	namespace_t zero = {ZERONAMESPACE,"*"};
	if(!strcmp(ns,"*")) {
	    m.ns = &zero;
	    m.type = attr?QNAMEA:QNAME;
	} else {
	    typedcode_t w = node_read(resolve_identifier(ns));
	    if(!TYPE_IS_NAMESPACE(w.t)) {
		as3_softwarning("%s might not be a namespace", ns);
	    }
	    v.c = code_append(v.c, w.c);
	    v.c = converttype(v.c, w.t, TYPE_NAMESPACE);
	    m.type = attr?RTQNAMEA:RTQNAME;
	}

	if(!multi) {
	    v.c = abc_getproperty2(v.c, &m);
	} else {
	    v.c = abc_getdescendants2(v.c, &m);
	}

	if(TYPE_IS_XML(v.t)) {
	    v.t = TYPE_XMLLIST;
	} else {
	    v.c = abc_coerce_a(v.c);
	    v.t = TYPE_ANY;
	}
	return mkcodenode(v);
    }


/* Line 274 of skeleton.m4  */
#line 2008 "parser.y"

    node_t* var_read(variable_t*v)
    {
        typedcode_t o;
	o.c = abc_getlocal(0, v->index);
	o.t = v->type;
	return mkcodenode(o);
    }

    node_t* resolve_identifier(const char*name)
    {
        typedcode_t o;
        o.t = 0;
        o.c = 0;

        slotinfo_t*a = 0;
        memberinfo_t*f = 0;

        variable_t*v;
        /* look at variables */
        if((v = find_variable(state, name))) {
            // name is a local variable
	    return var_read(v);
        }
        if((v = find_slot(state->method, name))) {
            o.c = abc_getscopeobject(o.c, 1);
            o.c = abc_getslot(o.c, v->index);
            o.t = v->type;
            return mkcodenode(o);
        }

        int i_am_static = state->method->is_static;

        if(!state->method->inner && !state->xmlfilter && state->cls)
	{
	    /* look at current class' members */
	    if((f = findmember_nsset(state->cls->info, name, 1, i_am_static)))
	    {
		// name is a member or attribute in this class
		int var_is_static = (f->flags&FLAG_STATIC);

		if(f->kind == INFOTYPE_VAR && (f->flags&FLAG_CONST)) {
		    /* if the variable is a constant (and we know what is evaluates to), we
		       can just use the value itself */
		    varinfo_t*v = (varinfo_t*)f;
		    if(v->value) {
			return mkconstnode(v->value);
		    }
		}
	       
		if(var_is_static >= i_am_static) {
		    if(f->kind == INFOTYPE_METHOD) {
			o.t = TYPE_FUNCTION(f);
		    } else {
			o.t = f->type;
		    }

		    if(var_is_static && !i_am_static) {
		    /* access to a static member from a non-static location.
		       do this via findpropstrict:
		       there doesn't seem to be any non-lookup way to access
		       static properties of a class */
			state->method->late_binding = 1;
			o.t = f->type;
			namespace_t ns = {f->access, f->package};
			multiname_t m = {QNAME, &ns, 0, name};
			o.c = abc_findpropstrict2(o.c, &m);
			o.c = abc_getproperty2(o.c, &m);
			return mkcodenode(o);
		    } else if(f->slot>0) {
			o.c = abc_getlocal_0(o.c);
			o.c = abc_getslot(o.c, f->slot);
			return mkcodenode(o);
		    } else {
			MEMBER_MULTINAME(m, f, name);
			o.c = abc_getlocal_0(o.c);
			o.c = abc_getproperty2(o.c, &m);
			return mkcodenode(o);
		    }
		}
	    } 
	    /* special case: it's allowed to access non-static constants
	       from a static context */
	    if(i_am_static && (f=findmember_nsset(state->cls->info, name, 1, 0))) {
		if(f->kind == INFOTYPE_VAR && (f->flags&FLAG_CONST)) {
		    varinfo_t*v = (varinfo_t*)f;
		    if(v->value) {
			return mkconstnode(v->value);
		    }
		}
	    }
	}
        
        /* look at actual classes, in the current package and imported */
        if(!state->xmlfilter && (a = find_class(name))) {
	    registry_use(a);
            if(state->cls && state->cls->info == (classinfo_t*)a && i_am_static) {
                o.c = abc_getlocal_0(0);
                o.t = TYPE_CLASS((classinfo_t*)a);
            } else {
                o = push_class(a);
            }
            return mkcodenode(o);
        }

        /* look through package prefixes */
        if(!state->xmlfilter && 
           (dict_contains(state->import_toplevel_packages, name) || 
            registry_ispackage(name))) {
            o.c = abc___pushpackage__(o.c, (char*)name);
            o.t = 0;
            return mkcodenode(o); //?
        }

        /* unknown object, let the avm2 resolve it */
        if(1) {
            if(!state->method->inner && !state->xmlfilter) {
                /* we really should make inner functions aware of the class context */
                as3_warning("Couldn't resolve '%s', doing late binding", name);
            }
            state->method->late_binding = 1;
                    
            multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, name};

            o.t = 0;
            o.c = abc_findpropstrict2(o.c, &m);
            o.c = abc_getproperty2(o.c, &m);
            return mkcodenode(o);
        }
    }


/* Line 274 of skeleton.m4  */
#line 2165 "parser.y"

    void add_active_url(const char*url)
    {
        NEW(namespace_t,n);
        n->name = url;
        list_append(state->active_namespace_urls, n);
    }



/* Line 274 of skeleton.m4  */
#line 607 "parser.tab.c"

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
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  161
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3382

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  135
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  135
/* YYNRULES -- Number of rules.  */
#define YYNRULES  357
/* YYNRULES -- Number of states.  */
#define YYNSTATES  612

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   364

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   119,     2,     2,     2,   117,   108,     2,
     123,   134,   116,   114,   101,   113,   128,   115,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   105,   100,
     110,   103,   111,   104,   129,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   125,     2,   126,   107,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   127,   106,   133,   118,     2,     2,     2,
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
      95,    96,    97,    98,    99,   102,   109,   112,   120,   121,
     122,   124,   130,   131,   132
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     8,    10,    13,    15,    17,
      19,    21,    23,    25,    30,    32,    33,    35,    37,    40,
      42,    44,    46,    48,    50,    55,    60,    62,    64,    65,
      68,    70,    72,    74,    76,    78,    80,    82,    84,    86,
      88,    90,    92,    94,    96,   100,   103,   105,   107,   109,
     111,   113,   115,   117,   122,   125,   127,   129,   130,   134,
     137,   138,   141,   144,   146,   150,   154,   155,   158,   159,
     162,   169,   170,   172,   174,   178,   180,   183,   187,   196,
     203,   209,   216,   218,   221,   223,   226,   227,   229,   231,
     234,   236,   239,   244,   248,   249,   258,   259,   269,   270,
     276,   278,   281,   283,   286,   288,   289,   296,   299,   301,
     306,   309,   311,   313,   315,   317,   321,   323,   324,   331,
     332,   338,   341,   344,   349,   350,   352,   354,   357,   359,
     361,   363,   365,   367,   369,   371,   373,   375,   377,   378,
     381,   382,   385,   386,   389,   390,   400,   401,   410,   411,
     413,   415,   418,   420,   425,   427,   429,   434,   436,   437,
     439,   441,   444,   446,   449,   458,   460,   462,   463,   468,
     470,   474,   478,   479,   482,   484,   486,   488,   490,   492,
     494,   496,   498,   500,   502,   504,   506,   507,   508,   513,
     514,   519,   520,   523,   526,   529,   532,   536,   538,   540,
     541,   543,   550,   561,   573,   575,   578,   580,   584,   588,
     592,   596,   597,   599,   602,   607,   611,   613,   618,   621,
     623,   625,   626,   627,   640,   642,   643,   644,   655,   657,
     661,   663,   665,   667,   671,   673,   675,   677,   680,   681,
     682,   686,   687,   689,   691,   693,   696,   699,   700,   705,
     710,   715,   718,   720,   723,   725,   727,   729,   733,   735,
     739,   740,   742,   744,   746,   748,   750,   752,   756,   762,
     764,   766,   768,   770,   772,   774,   776,   778,   780,   782,
     786,   790,   794,   798,   802,   806,   810,   814,   818,   822,
     826,   830,   833,   836,   840,   844,   848,   852,   856,   860,
     864,   868,   872,   876,   880,   884,   888,   892,   896,   899,
     902,   904,   908,   911,   916,   920,   924,   928,   932,   936,
     940,   944,   948,   952,   956,   960,   964,   970,   973,   976,
     979,   982,   986,   989,   990,   997,   999,  1001,  1003,  1005,
    1009,  1015,  1019,  1025,  1031,  1036,  1043,  1048,  1055,  1062,
    1069,  1073,  1075,  1078,  1083,  1088,  1091,  1096
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     136,     0,    -1,   137,    -1,    -1,   138,    -1,   139,    -1,
     138,   139,    -1,   187,    -1,   199,    -1,   197,    -1,   233,
      -1,   208,    -1,   148,    -1,   150,   127,   137,   133,    -1,
     100,    -1,    -1,   141,    -1,   142,    -1,   141,   142,    -1,
     199,    -1,   197,    -1,   233,    -1,   208,    -1,   148,    -1,
     150,   127,   140,   133,    -1,   125,   149,   260,   126,    -1,
     100,    -1,   144,    -1,    -1,   144,   146,    -1,   146,    -1,
     268,    -1,   190,    -1,   162,    -1,   163,    -1,   164,    -1,
     165,    -1,   172,    -1,   158,    -1,   184,    -1,   180,    -1,
     256,    -1,   269,    -1,   267,    -1,   127,   144,   133,    -1,
     127,   133,    -1,   100,    -1,   145,    -1,   152,    -1,   166,
      -1,   167,    -1,   252,    -1,   182,    -1,   150,   127,   144,
     133,    -1,   146,   100,    -1,   146,    -1,   145,    -1,    -1,
       3,    90,     3,    -1,   103,   260,    -1,    -1,    45,   153,
      -1,    31,   153,    -1,   154,    -1,   153,   101,   154,    -1,
       3,   243,   151,    -1,    -1,   156,   147,    -1,    -1,    65,
     155,    -1,    64,   123,   254,   134,   155,   157,    -1,    -1,
     152,    -1,   256,    -1,    45,     3,   243,    -1,     3,    -1,
      10,   123,    -1,    10,    49,   123,    -1,   161,   159,   100,
     254,   100,   256,   134,   155,    -1,   161,   160,    68,   254,
     134,   155,    -1,    11,   123,   254,   134,   155,    -1,    12,
     155,    11,   123,   254,   134,    -1,    66,    -1,    66,     3,
      -1,    29,    -1,    29,     3,    -1,    -1,   169,    -1,   171,
      -1,   169,   171,    -1,   170,    -1,   169,   170,    -1,    33,
     260,   105,   143,    -1,    61,   105,   143,    -1,    -1,    13,
     123,   173,   260,   134,   127,   168,   133,    -1,    -1,    32,
     123,     3,   243,   134,   175,   127,   143,   133,    -1,    -1,
      26,   127,   177,   143,   133,    -1,   174,    -1,   178,   174,
      -1,   178,    -1,   178,   176,    -1,   176,    -1,    -1,    51,
     127,   181,   143,   133,   179,    -1,    36,   254,    -1,    36,
      -1,    38,   123,   254,   134,    -1,   183,   147,    -1,     3,
      -1,    16,    -1,    15,    -1,    28,    -1,   186,   128,   185,
      -1,   185,    -1,    -1,    16,   186,   127,   188,   140,   133,
      -1,    -1,    16,   127,   189,   140,   133,    -1,    40,     3,
      -1,    40,   239,    -1,    40,   186,   128,   116,    -1,    -1,
     192,    -1,   193,    -1,   192,   193,    -1,    19,    -1,    20,
      -1,    17,    -1,    37,    -1,    46,    -1,    48,    -1,    47,
      -1,    24,    -1,    22,    -1,     3,    -1,    -1,    53,   240,
      -1,    -1,    53,   241,    -1,    -1,    14,   241,    -1,    -1,
     191,    30,     3,   194,   196,   127,   198,   201,   133,    -1,
      -1,   191,    43,     3,   195,   127,   200,   204,   133,    -1,
      -1,   202,    -1,   203,    -1,   202,   203,    -1,   100,    -1,
     150,   127,   201,   133,    -1,   208,    -1,   233,    -1,   125,
     149,   260,   126,    -1,   145,    -1,    -1,   205,    -1,   206,
      -1,   205,   206,    -1,   100,    -1,    45,     3,    -1,   191,
      25,   232,     3,   123,   229,   134,   243,    -1,    45,    -1,
      31,    -1,    -1,   191,   207,   209,   210,    -1,   211,    -1,
     210,   101,   211,    -1,     3,   243,   151,    -1,    -1,   103,
     260,    -1,     7,    -1,     8,    -1,     9,    -1,     4,    -1,
      55,    -1,    54,    -1,    44,    -1,    27,    -1,    28,    -1,
     226,    -1,   110,    -1,   111,    -1,    -1,    -1,   127,   260,
     219,   133,    -1,    -1,   127,   260,   221,   133,    -1,    -1,
     222,   218,    -1,   222,     4,    -1,   222,   111,    -1,   226,
     222,    -1,   223,   226,   222,    -1,     3,    -1,   220,    -1,
      -1,   227,    -1,   215,   224,   225,   115,   217,   111,    -1,
     215,   224,   225,   216,   222,   110,   115,   224,   217,   111,
      -1,   215,   224,   225,   216,   222,   223,   110,   115,   224,
     217,   111,    -1,   228,    -1,   227,   228,    -1,   220,    -1,
     220,   103,     4,    -1,   220,   103,   220,    -1,     3,   103,
     220,    -1,     3,   103,     4,    -1,    -1,   230,    -1,    94,
     231,    -1,   230,   101,    94,   231,    -1,   230,   101,   231,
      -1,   231,    -1,     3,   105,   242,   212,    -1,     3,   212,
      -1,    50,    -1,    34,    -1,    -1,    -1,   191,    25,   232,
       3,   123,   229,   134,   243,   127,   234,   143,   133,    -1,
       3,    -1,    -1,    -1,    25,   235,   123,   229,   134,   243,
     127,   237,   143,   133,    -1,   185,    -1,   186,   128,   185,
      -1,   239,    -1,   238,    -1,   240,    -1,   241,   101,   240,
      -1,   240,    -1,   116,    -1,    35,    -1,   105,   242,    -1,
      -1,    -1,   123,   245,   134,    -1,    -1,   246,    -1,   247,
      -1,   253,    -1,   246,   101,    -1,   247,   253,    -1,    -1,
      23,   260,   248,   244,    -1,   260,   123,   245,   134,    -1,
      52,   123,   245,   134,    -1,    63,   260,    -1,    41,    -1,
      41,   254,    -1,   260,    -1,   255,    -1,   260,    -1,   255,
     101,   260,    -1,   260,    -1,   256,   101,   260,    -1,    -1,
     259,    -1,     3,    -1,     4,    -1,     7,    -1,     8,    -1,
       9,    -1,   258,   105,   253,    -1,   259,   101,   258,   105,
     253,    -1,   236,    -1,   264,    -1,   249,    -1,   251,    -1,
     250,    -1,   265,    -1,   213,    -1,   214,    -1,     5,    -1,
      18,    -1,   125,   245,   126,    -1,    70,   257,   133,    -1,
     260,   110,   260,    -1,   260,   111,   260,    -1,   260,    75,
     260,    -1,   260,    76,   260,    -1,   260,    71,   260,    -1,
     260,    72,   260,    -1,   260,    74,   260,    -1,   260,    73,
     260,    -1,   260,    88,   260,    -1,   260,    89,   260,    -1,
     119,   260,    -1,   118,   260,    -1,   260,   108,   260,    -1,
     260,   107,   260,    -1,   260,   106,   260,    -1,   260,    97,
     260,    -1,   260,    96,   260,    -1,   260,    95,   260,    -1,
     260,   115,   260,    -1,   260,   117,   260,    -1,   260,   114,
     260,    -1,   260,   113,   260,    -1,   260,   116,   260,    -1,
     260,    68,   260,    -1,   260,    69,   260,    -1,   260,    39,
     260,    -1,   260,    67,   260,    -1,    42,   260,    -1,    35,
     260,    -1,    35,    -1,   123,   255,   134,    -1,   113,   260,
      -1,   260,   125,   260,   126,    -1,   260,    80,   260,    -1,
     260,    79,   260,    -1,   260,    86,   260,    -1,   260,    85,
     260,    -1,   260,    87,   260,    -1,   260,    78,   260,    -1,
     260,    77,   260,    -1,   260,    84,   260,    -1,   260,    81,
     260,    -1,   260,    82,   260,    -1,   260,    83,   260,    -1,
     260,   103,   260,    -1,   260,   104,   260,   105,   260,    -1,
     260,    92,    -1,   260,    91,    -1,    92,   260,    -1,    91,
     260,    -1,    52,   128,     3,    -1,   129,     3,    -1,    -1,
     260,   128,   123,   261,   260,   134,    -1,     3,    -1,   116,
      -1,   185,    -1,   116,    -1,   260,    90,   260,    -1,   260,
     128,   262,    90,   263,    -1,   260,    93,   263,    -1,   260,
      93,   262,    90,   263,    -1,   260,   128,   125,   260,   126,
      -1,   260,   128,   129,   263,    -1,   260,   128,   129,   262,
      90,   263,    -1,   260,    93,   129,   263,    -1,   260,    93,
     129,   262,    90,   263,    -1,   260,   128,   129,   125,   260,
     126,    -1,   260,    93,   129,   125,   260,   126,    -1,   260,
     128,   263,    -1,     3,    -1,    15,     3,    -1,    15,     3,
     103,     3,    -1,    15,     3,   103,     4,    -1,   191,   266,
      -1,    62,    15,   103,   260,    -1,    21,    15,   240,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   290,   290,   292,   292,   293,   294,   296,   297,   298,
     299,   300,   301,   302,   303,   305,   305,   306,   307,   309,
     310,   311,   312,   313,   314,   315,   316,   318,   319,   321,
     324,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   344,   345,   346,   347,
     348,   349,   350,   351,   363,   364,   368,   379,   387,   414,
     415,   417,   418,   420,   421,   423,   484,   484,   488,   489,
     492,   508,   509,   510,   515,   519,   524,   525,   527,   547,
     591,   607,   619,   622,   625,   628,   632,   633,   634,   635,
     636,   637,   639,   650,   653,   653,   684,   684,   709,   709,
     725,   726,   727,   728,   736,   745,   745,   794,   798,   809,
     819,   836,   837,   838,   839,   841,   842,   844,   844,   846,
     846,   870,   878,   892,   908,   909,   910,   911,   918,   919,
     920,   921,   922,   923,   924,   925,   926,   927,   931,   932,
     934,   935,   937,   938,   942,   940,   948,   946,   955,   956,
     957,   958,   959,   960,   961,   962,   963,   965,   971,   972,
     973,   974,   975,   976,   979,  1052,  1052,  1054,  1054,  1056,
    1057,  1059,  1143,  1144,  1152,  1153,  1156,  1157,  1158,  1159,
    1160,  1161,  1162,  1170,  1181,  1182,  1183,  1185,  1185,  1188,
    1188,  1191,  1192,  1195,  1200,  1203,  1206,  1209,  1212,  1216,
    1219,  1223,  1227,  1232,  1238,  1241,  1244,  1247,  1252,  1255,
    1258,  1268,  1272,  1278,  1284,  1292,  1297,  1303,  1311,  1319,
    1320,  1321,  1324,  1323,  1340,  1341,  1343,  1342,  1366,  1386,
    1401,  1402,  1404,  1405,  1407,  1408,  1409,  1418,  1419,  1423,
    1424,  1426,  1427,  1428,  1430,  1434,  1435,  1440,  1441,  1488,
    1538,  1559,  1581,  1584,  1591,  1594,  1597,  1600,  1603,  1606,
    1611,  1612,  1614,  1615,  1616,  1617,  1618,  1620,  1626,  1635,
    1636,  1637,  1638,  1639,  1640,  1642,  1646,  1651,  1669,  1680,
    1690,  1699,  1700,  1701,  1702,  1703,  1704,  1705,  1706,  1707,
    1708,  1709,  1710,  1711,  1712,  1713,  1714,  1715,  1716,  1717,
    1718,  1719,  1720,  1721,  1722,  1723,  1724,  1725,  1726,  1727,
    1728,  1729,  1730,  1731,  1732,  1733,  1734,  1735,  1736,  1737,
    1738,  1739,  1740,  1741,  1742,  1743,  1744,  1746,  1747,  1748,
    1749,  1751,  1766,  1774,  1774,  1828,  1829,  1830,  1831,  1872,
    1875,  1878,  1885,  1888,  1899,  1907,  1911,  1918,  1922,  1932,
    1943,  2140,  2174,  2181,  2188,  2195,  2217,  2224
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_IDENTIFIER", "T_STRING", "T_REGEXP",
  "T_EMPTY", "T_INT", "T_UINT", "T_FLOAT", "\"for\"", "\"while\"",
  "\"do\"", "\"switch\"", "\"implements\"", "\"namespace\"", "\"package\"",
  "\"protected\"", "\"arguments\"", "\"public\"", "\"private\"", "\"use\"",
  "\"internal\"", "\"new\"", "\"native\"", "\"function\"", "\"finally\"",
  "\"undefined\"", "\"NaN\"", "\"continue\"", "\"class\"", "\"const\"",
  "\"catch\"", "\"case\"", "\"set\"", "\"void\"", "\"throw\"",
  "\"static\"", "\"with\"", "\"instanceof\"", "\"import\"", "\"return\"",
  "\"typeof\"", "\"interface\"", "\"null\"", "\"var\"", "\"dynamic\"",
  "\"override\"", "\"final\"", "\"each\"", "\"get\"", "\"try\"",
  "\"super\"", "\"extends\"", "\"false\"", "\"true\"", "\"Boolean\"",
  "\"uint\"", "\"int\"", "\"Number\"", "\"String\"", "\"default\"",
  "\"default xml\"", "\"delete\"", "\"if\"", "\"else\"", "\"break\"",
  "\"is\"", "\"in\"", "\"as\"", "\"{ (dictionary)\"", "\"==\"", "\"===\"",
  "\"!=\"", "\"!==\"", "\"<=\"", "\">=\"", "\"|=\"", "\"/=\"", "\"%=\"",
  "\"*=\"", "\"&=\"", "\"+=\"", "\"-=\"", "\"^=\"", "\">>=\"", "\"<<=\"",
  "\">>>=\"", "\"||\"", "\"&&\"", "\"::\"", "\"--\"", "\"++\"", "\"..\"",
  "\"...\"", "\"<<\"", "\">>>\"", "\">>\"", "prec_none", "below_semicolon",
  "';'", "','", "below_assignment", "'='", "'?'", "':'", "'|'", "'^'",
  "'&'", "below_lt", "'<'", "'>'", "below_minus", "'-'", "'+'", "'/'",
  "'*'", "'%'", "'~'", "'!'", "minusminus_prefix", "plusplus_prefix",
  "below_curly", "'('", "new2", "'['", "']'", "'{'", "'.'", "'@'",
  "above_identifier", "below_else", "above_function", "'}'", "')'",
  "$accept", "PROGRAM", "MAYBE_PROGRAM_CODE_LIST", "PROGRAM_CODE_LIST",
  "PROGRAM_CODE", "MAYBE_INPACKAGE_CODE_LIST", "INPACKAGE_CODE_LIST",
  "INPACKAGE_CODE", "MAYBECODE", "CODE", "CODE_STATEMENT", "CODEPIECE",
  "CODEBLOCK", "PACKAGE_INITCODE", "EMBED_START",
  "CONDITIONAL_COMPILATION", "MAYBEEXPRESSION", "VARIABLE_DECLARATION",
  "VARIABLE_LIST", "ONE_VARIABLE", "IF_CODEBLOCK", "$@1", "MAYBEELSE",
  "IF", "FOR_INIT", "FOR_IN_INIT", "FOR_START", "FOR", "FOR_IN", "WHILE",
  "DO_WHILE", "BREAK", "CONTINUE", "MAYBE_CASE_LIST", "CASE_LIST", "CASE",
  "DEFAULT", "SWITCH", "$@2", "CATCH", "$@3", "FINALLY", "$@4",
  "CATCH_LIST", "CATCH_FINALLY_LIST", "TRY", "$@5", "THROW", "WITH_HEAD",
  "WITH", "X_IDENTIFIER", "PACKAGE", "PACKAGE_DECLARATION", "$@6", "$@7",
  "IMPORT", "MAYBE_MODIFIERS", "MODIFIER_LIST", "MODIFIER", "EXTENDS",
  "EXTENDS_LIST", "IMPLEMENTS_LIST", "CLASS_DECLARATION", "$@8",
  "INTERFACE_DECLARATION", "$@9", "MAYBE_CLASS_BODY", "CLASS_BODY",
  "CLASS_BODY_ITEM", "MAYBE_INTERFACE_BODY", "INTERFACE_BODY",
  "IDECLARATION", "VARCONST", "SLOT_DECLARATION", "$@10", "SLOT_LIST",
  "ONE_SLOT", "MAYBECONSTANT", "CONSTANT", "XML", "OPEN", "CLOSE",
  "CLOSE2", "XMLEXPR1", "$@11", "XMLEXPR2", "$@12", "XMLTEXT", "XML2",
  "XML_ID_OR_EXPR", "MAYBE_XMLATTRIBUTES", "XMLNODE", "XMLATTRIBUTES",
  "XMLATTRIBUTE", "MAYBE_PARAM_LIST", "PARAM_LIST", "PARAM", "GETSET",
  "FUNCTION_DECLARATION", "$@13", "MAYBE_IDENTIFIER", "INNERFUNCTION",
  "$@14", "CLASS", "PACKAGEANDCLASS", "CLASS_SPEC", "CLASS_SPEC_LIST",
  "TYPE", "MAYBETYPE", "MAYBE_PARAM_VALUES", "MAYBE_EXPRESSION_LIST",
  "EXPRESSION_LIST", "EXPRESSION_LIST_AND_COMMA", "XX", "NEW",
  "FUNCTIONCALL", "DELETE", "RETURN", "NONCOMMAEXPRESSION", "EXPRESSION",
  "COMMA_EXPRESSION", "VOIDEXPRESSION", "MAYBE_DICT_EXPRPAIR_LIST",
  "DICTLH", "DICT_EXPRPAIR_LIST", "E", "$@15", "ID_OR_NS", "SUBNODE",
  "MEMBER", "VAR_READ", "NAMESPACE_ID", "NAMESPACE_DECLARATION",
  "DEFAULT_NAMESPACE", "USE_NAMESPACE", 0
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
      59,    44,   355,    61,    63,    58,   124,    94,    38,   356,
      60,    62,   357,    45,    43,    47,    42,    37,   126,    33,
     358,   359,   360,    40,   361,    91,    93,   123,    46,    64,
     362,   363,   364,   125,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   135,   136,   137,   137,   138,   138,   139,   139,   139,
     139,   139,   139,   139,   139,   140,   140,   141,   141,   142,
     142,   142,   142,   142,   142,   142,   142,   143,   143,   144,
     144,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   146,   146,   146,   146,
     146,   146,   146,   146,   147,   147,   148,   149,   150,   151,
     151,   152,   152,   153,   153,   154,   156,   155,   157,   157,
     158,   159,   159,   159,   160,   160,   161,   161,   162,   163,
     164,   165,   166,   166,   167,   167,   168,   168,   168,   168,
     169,   169,   170,   171,   173,   172,   175,   174,   177,   176,
     178,   178,   179,   179,   179,   181,   180,   182,   182,   183,
     184,   185,   185,   185,   185,   186,   186,   188,   187,   189,
     187,   190,   190,   190,   191,   191,   192,   192,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   194,   194,
     195,   195,   196,   196,   198,   197,   200,   199,   201,   201,
     202,   202,   203,   203,   203,   203,   203,   203,   204,   204,
     205,   205,   206,   206,   206,   207,   207,   209,   208,   210,
     210,   211,   212,   212,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   214,   215,   216,   217,   219,   218,   221,
     220,   222,   222,   222,   222,   223,   223,   224,   224,   225,
     225,   226,   226,   226,   227,   227,   228,   228,   228,   228,
     228,   229,   229,   229,   229,   230,   230,   231,   231,   232,
     232,   232,   234,   233,   235,   235,   237,   236,   238,   239,
     240,   240,   241,   241,   242,   242,   242,   243,   243,   244,
     244,   245,   245,   245,   246,   247,   246,   248,   249,   250,
     250,   251,   252,   252,   253,   254,   255,   255,   256,   256,
     257,   257,   258,   258,   258,   258,   258,   259,   259,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   261,   260,   262,   262,   263,   263,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     264,   265,   266,   266,   266,   267,   268,   269
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     4,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     1,     4,     4,     1,     1,     0,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     4,     2,     1,     1,     0,     3,     2,
       0,     2,     2,     1,     3,     3,     0,     2,     0,     2,
       6,     0,     1,     1,     3,     1,     2,     3,     8,     6,
       5,     6,     1,     2,     1,     2,     0,     1,     1,     2,
       1,     2,     4,     3,     0,     8,     0,     9,     0,     5,
       1,     2,     1,     2,     1,     0,     6,     2,     1,     4,
       2,     1,     1,     1,     1,     3,     1,     0,     6,     0,
       5,     2,     2,     4,     0,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     2,
       0,     2,     0,     2,     0,     9,     0,     8,     0,     1,
       1,     2,     1,     4,     1,     1,     4,     1,     0,     1,
       1,     2,     1,     2,     8,     1,     1,     0,     4,     1,
       3,     3,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     0,     4,     0,
       4,     0,     2,     2,     2,     2,     3,     1,     1,     0,
       1,     6,    10,    11,     1,     2,     1,     3,     3,     3,
       3,     0,     1,     2,     4,     3,     1,     4,     2,     1,
       1,     0,     0,    12,     1,     0,     0,    10,     1,     3,
       1,     1,     1,     3,     1,     1,     1,     2,     0,     0,
       3,     0,     1,     1,     1,     2,     2,     0,     4,     4,
       4,     2,     1,     2,     1,     1,     1,     3,     1,     3,
       0,     1,     1,     1,     1,     1,     1,     3,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       1,     3,     2,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     5,     2,     2,     2,
       2,     3,     2,     0,     6,     1,     1,     1,     1,     3,
       5,     3,     5,     5,     4,     6,     4,     6,     6,     6,
       3,     1,     2,     4,     4,     2,     4,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     124,   351,   177,   277,   174,   175,   176,     0,     0,    66,
       0,     0,   130,   278,   128,   129,     0,   136,     0,   135,
     225,   181,   182,   310,   131,     0,     0,     0,   180,   132,
     134,   133,     0,     0,   179,   178,     0,     0,     0,   260,
       0,     0,    14,   184,     0,     0,     0,     0,   241,   124,
       0,     0,     2,   124,     5,    56,    12,     0,    38,    71,
      33,    34,    35,    36,    37,    40,   124,    39,     7,    32,
       0,   125,   126,     9,     8,    11,   275,   276,     0,   183,
      10,   269,   271,   273,   272,    41,   258,   270,   274,    43,
      31,    42,     0,    76,     0,     0,   124,    94,   111,   113,
     112,   114,   119,   116,     0,     0,   351,   247,   224,     0,
     309,     0,   121,     0,   122,   308,   105,   241,     0,     0,
     251,     0,   262,   263,   264,   265,   266,     0,     0,   261,
     330,   329,   312,   292,   291,     0,   256,     0,   242,   243,
     244,   254,    84,     0,   108,   252,     0,    82,    46,    45,
     124,    47,    30,     0,    48,    49,    50,    52,     0,    51,
     332,     1,     6,   124,   351,     0,    72,     0,     0,    73,
      55,   110,     0,   221,     0,   166,     0,   165,   167,   355,
     137,   127,   197,     0,   198,   199,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   328,   327,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   241,
       0,     0,    77,     0,   255,     0,    67,     0,   124,   117,
       0,   228,     0,   231,   230,   357,   239,   211,     0,     0,
      28,     0,   331,     0,     0,   280,     0,     0,     0,   311,
     279,   245,   246,    85,   238,    62,    63,   107,   253,    61,
      83,    44,    29,   124,     0,   238,     0,     0,    54,   352,
     220,   219,     0,   138,   140,     0,   189,     0,   206,     0,
     200,   204,   259,   306,   307,   304,   305,   285,   286,   288,
     287,   283,   284,   320,   319,   315,   314,   322,   323,   324,
     321,   317,   316,   318,   289,   290,   339,   111,   338,     0,
     337,     0,   341,   298,   297,   296,   325,     0,   295,   294,
     293,   281,   282,   302,   301,   299,   303,   300,     0,     0,
     333,     0,     0,     0,   350,    66,     0,     0,    26,    57,
       0,   124,    17,    23,     0,    20,    19,    22,    21,   124,
     115,     0,   241,   248,   172,     0,     0,   212,   216,   109,
     123,   229,     0,    27,   250,   356,    66,   267,     0,   257,
       0,    60,     0,   124,    13,    60,     0,     0,     0,     0,
       0,   142,     0,     0,   238,   168,   169,     0,     0,     0,
     185,   186,   191,   205,     0,     0,   346,     0,     0,   249,
     313,     0,     0,     0,     0,   344,     0,    80,     0,     0,
       0,   120,    18,   124,     0,     0,     0,     0,   218,   213,
     238,     0,     0,    68,     0,   236,   235,   234,   237,     0,
      65,    64,    53,     0,    66,   353,   354,   211,   139,     0,
       0,   232,   141,   146,    60,     0,   190,   210,   209,   207,
     208,     0,     0,     0,     0,   338,   342,   326,     0,   343,
       0,     0,   340,    81,    86,     0,     0,   118,   240,   173,
     172,     0,     0,   215,     0,     0,   100,   104,   102,   106,
      66,    70,   268,    59,     0,    79,     0,   143,   144,     0,
     124,   171,   170,   201,   193,   184,   194,     0,   192,     0,
     191,   349,   347,   334,   348,   345,     0,     0,     0,    87,
      90,    88,    25,    24,   217,   226,   214,    98,     0,   101,
     103,    69,    66,   238,   124,   233,     0,   162,     0,     0,
     124,   160,     0,   187,   184,   191,   195,     0,    28,    95,
      91,    89,    28,    28,   238,    78,     0,   152,    57,   157,
       0,     0,     0,   124,   150,   154,   155,   163,   221,   147,
     161,   186,     0,     0,   196,    28,    93,     0,     0,     0,
     222,     0,   124,   145,   151,     0,     0,   188,   186,    92,
     227,    99,    96,    28,     0,     0,     0,   202,     0,     0,
       0,   156,   153,   211,   203,    28,   223,     0,     0,   238,
      97,   164
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    51,    52,    53,    54,   350,   351,   352,   372,   373,
     151,   152,   171,   353,   420,   153,   440,   154,   269,   266,
      95,    96,   491,    58,   167,   168,    59,    60,    61,    62,
      63,   155,   156,   518,   519,   520,   521,    64,   237,   486,
     599,   487,   553,   488,   489,    65,   250,   157,    66,    67,
     320,   242,    68,   359,   238,    69,   158,    71,    72,   391,
     393,   450,   355,   534,   356,   500,   562,   563,   564,   539,
     540,   541,   178,   357,   285,   395,   396,   428,    76,    77,
      78,   402,   461,   508,   572,   184,   397,   462,   509,   185,
     289,    79,   290,   291,   366,   367,   368,   282,   358,   593,
     109,    81,   552,   243,   244,   437,   452,   438,   381,   363,
     137,   138,   139,   246,    82,    83,    84,   159,   140,   233,
     234,    85,   127,   128,   129,    86,   411,   321,   322,    87,
      88,   179,    89,    90,    91
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -508
static const yytype_int16 yypact[] =
{
    1734,   770,  -508,  -508,  -508,  -508,  -508,   -31,   -43,  -508,
     -35,    74,  -508,  -508,  -508,  -508,    54,  -508,  2187,  -508,
     113,  -508,  -508,   318,  -508,    -6,   277,  2187,  -508,  -508,
    -508,  -508,    -1,   -44,  -508,  -508,   105,  2187,    29,   441,
    2187,  2187,  -508,  -508,  2187,  2187,  2187,  2187,  2187,   714,
     133,   137,  -508,   587,  -508,  -508,  -508,    24,  -508,  2115,
    -508,  -508,  -508,  -508,  -508,  -508,  1988,  -508,  -508,  -508,
     352,   525,  -508,  -508,  -508,  -508,  -508,  -508,    14,  -508,
    -508,  -508,  -508,  -508,  -508,    52,  2944,  -508,  -508,  -508,
    -508,  -508,    36,  -508,  2187,   145,  1988,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,   -20,   423,  -508,   144,  -508,    39,
     388,  2187,    40,    41,  -508,   388,  -508,  2187,   169,    70,
     388,  2187,  -508,  -508,  -508,  -508,  -508,    87,    94,   123,
     388,   388,   822,   388,   388,   -71,  2882,   102,   129,  2187,
    -508,  2882,   232,   239,  2187,  2187,   239,   242,  -508,  -508,
     841,  -508,  -508,   120,  -508,  -508,  -508,  -508,   234,  -508,
    -508,  -508,  -508,  1095,   183,   254,  -508,   159,   192,    52,
     166,  -508,   264,    33,   270,  -508,   271,  -508,  -508,  -508,
    -508,  -508,  -508,  2187,  -508,    17,  2187,  2187,  2187,  2187,
    2187,  2187,  2187,  2187,  2187,  2187,  2187,  2187,  2187,  2187,
    2187,  2187,  2187,  2187,  2187,  2187,  2187,  2187,  2187,  2187,
    2187,  -508,  -508,   132,  2187,  2187,  2187,  2187,  2187,  2187,
    2187,  2187,  2187,  2187,  2187,  2187,  2187,  2187,  2187,  2187,
    2187,   127,  -508,   143,   177,   158,  -508,  2187,  1222,  -508,
     423,   162,   163,  -508,  -508,  -508,   171,    46,   164,   341,
    1861,   165,  -508,  2187,   167,  -508,  2187,   441,  2187,  -508,
    -508,  -508,  -508,  -508,   195,   203,  -508,  -508,  -508,   203,
    -508,  -508,  -508,  1988,   174,   195,  2187,  2187,  -508,   205,
    -508,  -508,   306,   262,   263,   314,  2882,   217,   225,    -5,
      17,  -508,  2882,  1087,   579,   579,   579,   392,   392,   392,
     392,  1087,  1087,  2882,  2882,  2882,  2882,  2882,  2882,  2882,
    2882,  2882,  2882,  2882,  3006,  3068,  -508,  -508,   240,   255,
    -508,   243,  -508,   706,   706,   706,  2882,  2386,  3130,  3192,
    3254,  1087,  1087,   822,   822,   388,   388,   388,   198,  2448,
    -508,  2187,   260,   245,  -508,  -508,  2187,  2246,  -508,   213,
     207,  1349,  -508,  -508,   215,  -508,  -508,  -508,  -508,  1222,
    -508,   423,  2187,  -508,    -7,   344,   214,   248,  -508,  -508,
    -508,   227,   230,  1861,  -508,  2882,  -508,  -508,   259,  2882,
     303,   272,   239,   968,  -508,   -41,   266,   244,   111,   258,
     423,   354,   423,   257,   195,   285,  -508,   261,    19,    22,
    -508,  -508,  -508,  -508,  2187,   297,  -508,   414,  2187,  -508,
    -508,  2187,  2510,  2187,   301,  -508,   414,  -508,   268,   280,
    2187,  -508,  -508,  1222,   278,   279,  2187,   303,  -508,  -508,
     195,    56,    71,   347,  2187,  -508,  -508,  -508,  -508,  2187,
    -508,  -508,  -508,  2187,  -508,  -508,  -508,    46,  -508,   423,
     288,  -508,   315,  -508,   272,   314,  -508,  -508,  -508,  -508,
    -508,   307,    12,  2572,   414,  -508,  -508,  2882,  2316,  -508,
    2634,   414,  -508,  -508,     9,  2696,   295,  -508,  -508,  2882,
     329,   294,   344,  -508,   308,   310,  -508,  -508,    71,  -508,
    -508,  -508,  -508,  2882,   -33,  -508,   302,   315,  -508,   423,
      28,  -508,  -508,  -508,  -508,   322,  -508,  2187,  -508,   336,
    -508,  -508,  -508,  -508,  -508,  -508,  2187,   348,   319,     9,
    -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,   451,  -508,
    -508,  -508,  -508,   195,  1476,  -508,   452,  -508,   431,   325,
     265,  -508,    14,  2882,   355,  -508,    18,  2758,  1861,  -508,
    -508,  -508,  1861,  1861,   195,  -508,   345,  -508,   213,  -508,
     349,   375,   338,  1603,  -508,  -508,  -508,  -508,    33,  -508,
    -508,  -508,   342,    14,    18,  1861,  -508,   353,   357,   343,
    -508,  2187,  1476,  -508,  -508,   488,   381,  -508,  -508,  -508,
    -508,  -508,  -508,  1861,  2820,   360,   371,  -508,   386,   372,
     365,  -508,  -508,    46,  -508,  1861,  -508,   367,   377,   195,
    -508,  -508
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -508,  -508,   356,  -508,   465,  -319,  -508,   170,   -79,   -37,
       1,   -59,   426,    58,   -34,     2,    69,   466,   384,   147,
    -332,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
    -508,  -508,  -508,  -508,  -508,    20,    31,  -508,  -508,    43,
    -508,    44,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -508,
      13,    67,  -508,  -508,  -508,  -508,     0,  -508,   462,  -508,
    -508,  -508,    60,  -508,    66,  -508,   -39,  -508,   -22,  -508,
    -508,     8,  -508,     3,  -508,  -508,    96,    72,  -508,  -508,
    -508,  -508,  -507,  -508,  -508,  -166,  -508,  -459,  -508,  -501,
    -508,  -424,  -508,   267,  -433,  -508,  -344,   -15,     4,  -508,
    -508,  -508,  -508,  -508,   528,   -95,   106,   131,  -175,  -508,
    -108,  -508,  -508,  -508,  -508,  -508,  -508,  -508,  -131,   -50,
     509,   -54,  -508,   304,  -508,   -12,  -508,  -188,  -185,  -508,
    -508,  -508,  -508,  -508,  -508
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -337
static const yytype_int16 yytable[] =
{
      70,    55,    57,    75,    80,   169,   107,   170,   262,   251,
     245,   110,   150,   417,   496,   115,   504,   182,    92,   288,
     287,   429,   504,   457,   103,   120,   459,   -74,   130,   131,
     258,   180,   132,   133,   134,   136,   141,   170,   510,   103,
     424,   571,   516,   343,   433,    12,   344,    14,    15,   364,
      17,   546,    19,    70,    55,    57,    75,    80,    56,   364,
      73,   248,   439,   259,   586,    24,    74,   280,   186,   105,
     517,   254,   588,   536,    29,    30,    31,    98,   104,   117,
      94,   598,   136,   281,   118,   545,   574,   483,    97,    99,
     100,   272,    93,   113,   267,   268,   426,   484,   427,   136,
     385,   532,   101,   485,   476,   141,   400,   239,   240,   136,
     401,    56,   495,    73,   445,   446,   108,   111,   241,    74,
     119,   338,   505,   506,   288,   377,   116,   141,   537,   506,
     317,   405,   136,   136,   406,   317,   160,   161,   526,   507,
     365,   183,    99,   100,   183,   507,   183,    99,   100,   183,
     482,   163,   121,   186,   414,   101,   235,   415,   531,   232,
     101,  -158,   247,    70,    55,    57,    75,    80,  -111,   249,
     607,   286,   252,   253,   292,   293,   294,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   256,
     555,   102,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   141,   339,   454,
     255,    56,   466,    73,   257,   347,   386,   387,   260,    74,
     261,   472,   458,   460,   210,   263,   383,   213,    70,    55,
     354,   375,   264,   318,   141,   270,   379,   273,   318,   172,
     340,   -75,   341,   360,   425,   481,   342,   275,   317,   276,
     277,   319,   371,   317,   136,   136,   278,   279,   180,   230,
      99,   100,   231,   283,   284,    99,   100,   345,   258,   512,
     112,   346,    12,   101,    14,    15,   515,    17,   101,    19,
    -116,   361,    99,   100,   362,   448,   418,   451,   369,   374,
     380,   376,    24,   492,   382,   101,    98,   384,   388,   389,
     536,    29,    30,    31,   272,   390,   392,   394,    99,   100,
     398,   106,     2,     3,   272,     4,     5,     6,   399,   412,
    -336,   101,   409,   407,   136,   416,    13,   141,   435,  -241,
     421,    18,   423,    20,    98,    21,    22,   364,   430,   431,
     141,    70,    55,   354,   451,  -115,    99,   100,   556,    70,
      55,   354,    28,   432,   434,   537,   443,   172,   449,   101,
      33,   318,    34,    35,   371,   439,   318,   173,   444,   579,
     404,   447,   174,   175,   453,   413,   455,   464,    39,   494,
     172,   471,   463,   241,   456,   176,   467,   177,  -159,   468,
     173,   470,   473,   241,   535,   241,   175,   474,   475,    40,
      41,   477,   490,   478,   479,   498,   499,    98,   503,   436,
     177,   525,   141,    70,    55,   354,    98,   493,   523,    99,
     100,   187,   426,   528,   611,   527,   533,   542,    99,   100,
     241,    47,   101,    48,   122,   123,   544,    50,   124,   125,
     126,   101,   549,   548,   554,   567,   568,   370,   569,   188,
     189,   190,   241,  -337,  -337,  -337,  -337,   195,   196,   576,
     573,   583,   580,   577,   578,   587,   582,   592,   210,   211,
     212,   213,   210,   211,   212,   213,   590,   214,   215,   216,
     591,   596,   597,   602,   603,   543,   589,   604,   606,   605,
     538,   609,   222,   223,   547,   224,   225,   226,   227,   228,
     610,   229,   241,   230,   600,   229,   231,   230,   162,   274,
     231,   422,   236,   501,   581,   166,   608,   265,   180,   441,
     465,   529,   530,   181,   561,   559,   560,   565,   566,   550,
     538,   584,    12,   595,    14,    15,   141,    17,   570,    19,
     551,   502,   524,   585,   114,   497,   135,   403,   480,     0,
       0,   378,    24,   561,   559,   560,   565,   566,     0,   594,
       0,    29,    30,    31,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   559,   560,   565,   566,    -4,     0,     0,
       1,     2,     3,     0,     4,     5,     6,     7,     8,     9,
      10,     0,     0,    11,    12,    13,    14,    15,    16,    17,
      18,    19,     0,     0,    21,    22,     0,     0,   187,     0,
       0,     0,    23,     0,    24,    25,     0,    26,     0,    27,
       0,    28,     0,    29,    30,    31,     0,     0,    32,    33,
       0,    34,    35,     0,     0,     0,  -337,  -337,  -337,    36,
      37,    38,     0,     0,   195,   196,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   210,
     211,   212,   213,     0,   214,   215,   216,     0,    40,    41,
       0,     0,     0,     0,     0,     0,     0,    42,     0,   222,
     223,     0,   224,   225,   226,   227,   228,    43,     0,     0,
      44,     0,   229,     0,   230,    45,    46,   231,     0,     0,
      47,     0,    48,     0,    49,     0,    50,     1,     2,     3,
      -4,     4,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
       0,    21,    22,   142,     0,   143,     0,     0,     0,    23,
     144,    24,    25,     0,    26,   145,    27,     0,    28,   146,
      29,    30,    31,     0,     0,    32,    33,     0,    34,    35,
       0,     0,     0,  -137,     0,     0,    36,    37,    38,     0,
     147,     0,     0,     0,    39,  -137,     0,  -137,     0,  -137,
    -137,     0,  -137,     0,  -137,  -137,   210,   211,   212,   213,
    -137,  -137,     0,     0,     0,    40,    41,  -137,     0,     0,
       0,     0,     0,  -137,   148,  -137,  -137,  -137,  -137,   224,
     225,   226,   227,   228,    43,     0,     0,    44,     0,   229,
       0,   230,    45,    46,   231,     0,     0,    47,     0,    48,
       0,    49,     0,    50,     1,     2,     3,   149,     4,     5,
       6,     7,     8,     9,    10,     0,     0,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,     0,    21,    22,
     142,     0,   143,     0,     0,     0,    23,   144,    24,    25,
       0,    26,   145,    27,     0,    28,   146,    29,    30,    31,
       0,     0,    32,    33,     0,    34,    35,     0,     0,     0,
       0,     0,     0,    36,    37,    38,     0,   147,     0,     0,
       0,    39,   210,   211,   212,   213,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,    41,     0,     0,     0,   226,   227,   228,
       0,   148,     0,     0,     0,   229,     0,   230,     0,     0,
     231,    43,     0,     0,    44,     0,     0,     0,     0,    45,
      46,     0,     0,     0,    47,     0,    48,     0,    49,     0,
      50,     1,     2,     3,   271,     4,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     0,    21,    22,   142,     0,   143,
       0,     0,     0,    23,   144,    24,    25,     0,    26,   145,
      27,     0,    28,   146,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,    38,     0,   147,     0,     0,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      41,     0,     0,     0,     0,     0,     0,     0,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
       0,    44,     0,     0,     0,     0,    45,    46,     0,     0,
       0,    47,     0,    48,     0,    49,     0,    50,     1,     2,
       3,   442,     4,     5,     6,     7,     8,     9,    10,     0,
       0,    11,    12,    13,    14,    15,    16,    17,    18,    19,
       0,     0,    21,    22,     0,     0,  -337,     0,     0,     0,
      23,     0,    24,    25,     0,    26,     0,    27,     0,    28,
       0,    29,    30,    31,     0,     0,    32,    33,     0,    34,
      35,     0,     0,     0,     0,     0,     0,    36,    37,    38,
       0,     0,  -337,  -337,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   210,   211,   212,
     213,     0,   214,   215,   216,     0,    40,    41,     0,     0,
       0,     0,     0,     0,     0,    42,     0,  -337,  -337,     0,
     224,   225,   226,   227,   228,    43,     0,     0,    44,     0,
     229,     0,   230,    45,    46,   231,     0,     0,    47,     0,
      48,     0,    49,     0,    50,     1,     2,     3,    -3,     4,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    12,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    21,
      22,     0,     0,     0,     0,     0,     0,    23,     0,    24,
      25,     0,    26,     0,    27,     0,    28,     0,    29,    30,
      31,     0,     0,    32,    33,     0,    34,    35,     0,     0,
       0,     0,     0,     0,    36,    37,    38,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,    41,     0,     0,     0,     0,     0,
       0,     0,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,     0,     0,    44,     0,     0,     0,     0,
      45,    46,     0,     0,     0,    47,     0,   349,     0,    49,
       0,    50,     1,     2,     3,   -15,     4,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    12,    13,    14,    15,
      16,    17,    18,    19,     0,     0,    21,    22,     0,     0,
       0,     0,     0,     0,    23,     0,    24,    25,     0,    26,
       0,    27,     0,    28,     0,    29,    30,    31,     0,     0,
      32,    33,     0,    34,    35,     0,     0,     0,     0,     0,
       0,    36,    37,    38,     0,     0,     0,     0,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      40,    41,     0,     0,     0,     0,     0,     0,     0,   348,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,    44,     0,     0,     0,     0,    45,    46,     0,
       0,     0,    47,     0,   349,     0,    49,     0,    50,     1,
       2,     3,   -16,     4,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    12,    13,    14,    15,    16,    17,    18,
      19,     0,     0,    21,    22,     0,     0,     0,     0,     0,
       0,    23,     0,    24,    25,     0,    26,     0,    27,     0,
      28,     0,    29,    30,    31,     0,     0,    32,    33,     0,
      34,    35,     0,     0,     0,     0,     0,     0,    36,    37,
      38,     0,     0,     0,     0,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,   557,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,    44,
       0,     0,     0,     0,    45,    46,     0,     0,     0,    47,
       0,   558,     0,    49,     0,    50,     1,     2,     3,  -148,
       4,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      12,    13,    14,    15,    16,    17,    18,    19,     0,     0,
      21,    22,     0,     0,     0,     0,     0,     0,    23,     0,
      24,    25,     0,    26,     0,    27,     0,    28,     0,    29,
      30,    31,     0,     0,    32,    33,     0,    34,    35,     0,
       0,     0,     0,     0,     0,    36,    37,    38,     0,     0,
       0,     0,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    40,    41,     0,     0,     0,     0,
       0,     0,     0,   557,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,    44,     0,     0,     0,
       0,    45,    46,     0,     0,     0,    47,     0,   558,     0,
      49,     0,    50,     0,    -3,     0,  -149,     1,     2,     3,
       0,     4,     5,     6,     7,     8,     9,    10,     0,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,     0,
       0,    21,    22,     0,     0,     0,     0,     0,     0,    23,
       0,    24,    25,     0,    26,     0,    27,     0,    28,     0,
      29,    30,    31,     0,     0,    32,    33,     0,    34,    35,
       0,     0,     0,     0,     0,     0,    36,    37,    38,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    40,    41,     0,     0,     0,
       0,     0,     0,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,    44,     0,     0,
       0,     0,    45,    46,     0,     0,     0,    47,     0,    48,
       0,    49,     0,    50,     1,     2,     3,     0,     4,     5,
       6,     7,     8,     9,    10,     0,  -124,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,     0,    21,    22,
     142,     0,   143,     0,     0,     0,    23,   144,    24,    25,
       0,    26,   145,    27,     0,    28,   146,    29,    30,    31,
       0,     0,    32,    33,     0,    34,    35,     0,     0,     0,
       0,     0,     0,    36,    37,    38,     0,   147,     0,     0,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,    41,     0,     0,     0,     0,     0,     0,
       0,   148,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,     0,     0,    44,     0,     0,     0,     0,    45,
      46,     0,     0,     0,    47,     0,    48,     0,    49,     0,
      50,     1,     2,     3,     0,     4,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     0,    21,    22,   142,     0,   143,
       0,     0,     0,    23,   144,    24,    25,     0,    26,   145,
      27,     0,    28,   146,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,    38,     0,   147,     0,     0,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      41,     0,     0,     0,     0,     0,     0,     0,   148,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
       0,    44,     0,     0,     0,     0,    45,    46,     0,     0,
       0,    47,     0,    48,     0,    49,     0,    50,   164,     2,
       3,     0,     4,     5,     6,     0,     0,     0,     0,     0,
       0,     0,     0,    13,     0,     0,     0,     0,    18,     0,
      20,     0,    21,    22,     0,     0,   143,     0,     0,     0,
      23,     0,     0,     0,     0,     0,     0,    27,     0,    28,
     165,     0,     0,     0,     0,     0,     0,    33,     0,    34,
      35,     0,     0,     0,     0,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
     106,     2,     3,     0,     4,     5,     6,     0,     0,     0,
       0,     0,     0,     0,     0,    13,    40,    41,     0,     0,
      18,     0,    20,     0,    21,    22,     0,     0,     0,     0,
       0,     0,    23,     0,     0,    43,     0,     0,    44,    27,
       0,    28,     0,    45,    46,     0,     0,     0,    47,    33,
      48,    34,    35,     0,    50,     0,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    40,    41,
       0,     0,     0,     0,     0,   187,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    43,     0,     0,
      44,     0,     0,     0,     0,    45,    46,     0,     0,     0,
      47,     0,    48,   188,   189,   190,    50,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
       0,   214,   215,   216,     0,     0,     0,     0,     0,   217,
     218,     0,   219,   220,   221,   187,   222,   223,     0,   224,
     225,   226,   227,   228,     0,     0,     0,     0,     0,   229,
       0,   230,     0,     0,   231,     0,     0,     0,     0,     0,
     419,     0,     0,   188,   189,   190,     0,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
       0,   214,   215,   216,     0,     0,     0,     0,     0,   217,
     218,     0,   219,   220,   221,   187,   222,   223,     0,   224,
     225,   226,   227,   228,     0,     0,     0,     0,     0,   229,
       0,   230,     0,     0,   231,     0,     0,     0,     0,     0,
     513,     0,     0,   188,   189,   190,     0,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
       0,   214,   215,   216,     0,     0,     0,   187,     0,   217,
     218,   408,   219,   220,   221,     0,   222,   223,     0,   224,
     225,   226,   227,   228,     0,     0,     0,     0,     0,   229,
       0,   230,     0,     0,   231,   188,   189,   190,     0,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,     0,   214,   215,   216,     0,     0,     0,   187,
       0,   217,   218,     0,   219,   220,   221,     0,   222,   223,
       0,   224,   225,   226,   227,   228,     0,     0,     0,     0,
       0,   229,     0,   230,   410,     0,   231,   188,   189,   190,
       0,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,     0,   214,   215,   216,     0,     0,
       0,   187,     0,   217,   218,     0,   219,   220,   221,     0,
     222,   223,     0,   224,   225,   226,   227,   228,     0,     0,
       0,     0,     0,   229,     0,   230,   469,     0,   231,   188,
     189,   190,     0,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,     0,   214,   215,   216,
       0,     0,     0,   187,     0,   217,   218,     0,   219,   220,
     221,     0,   222,   223,     0,   224,   225,   226,   227,   228,
       0,     0,     0,     0,     0,   229,     0,   230,   511,     0,
     231,   188,   189,   190,     0,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,     0,   214,
     215,   216,     0,     0,     0,   187,     0,   217,   218,     0,
     219,   220,   221,     0,   222,   223,     0,   224,   225,   226,
     227,   228,     0,     0,     0,     0,     0,   229,     0,   230,
     514,     0,   231,   188,   189,   190,     0,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
       0,   214,   215,   216,     0,     0,     0,   187,     0,   217,
     218,     0,   219,   220,   221,     0,   222,   223,     0,   224,
     225,   226,   227,   228,     0,     0,     0,     0,     0,   229,
       0,   230,   522,     0,   231,   188,   189,   190,     0,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,     0,   214,   215,   216,     0,     0,     0,   187,
       0,   217,   218,   575,   219,   220,   221,     0,   222,   223,
       0,   224,   225,   226,   227,   228,     0,     0,     0,     0,
       0,   229,     0,   230,     0,     0,   231,   188,   189,   190,
       0,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,     0,   214,   215,   216,     0,     0,
       0,   187,     0,   217,   218,     0,   219,   220,   221,     0,
     222,   223,     0,   224,   225,   226,   227,   228,     0,     0,
       0,     0,     0,   229,     0,   230,   601,     0,   231,   188,
     189,   190,     0,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,     0,   214,   215,   216,
       0,     0,     0,   187,     0,   217,   218,     0,   219,   220,
     221,     0,   222,   223,     0,   224,   225,   226,   227,   228,
       0,     0,     0,     0,     0,   229,     0,   230,     0,     0,
     231,   188,   189,   190,     0,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,     0,   214,
     215,   216,     0,     0,     0,   187,     0,   217,   218,     0,
     219,   220,   221,     0,     0,   223,     0,   224,   225,   226,
     227,   228,     0,     0,     0,     0,     0,   229,     0,   230,
       0,     0,   231,   188,   189,   190,     0,   191,   192,   193,
     194,   195,   196,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   209,   210,   211,   212,   213,
       0,   214,   215,   216,     0,     0,     0,   187,     0,     0,
       0,     0,   219,   220,   221,     0,   222,   223,     0,   224,
     225,   226,   227,   228,     0,     0,     0,     0,     0,   229,
       0,   230,     0,     0,   231,   188,   189,   190,     0,   191,
     192,   193,   194,   195,   196,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   210,   211,
     212,   213,     0,   214,   215,   216,     0,     0,     0,   187,
       0,     0,     0,     0,   219,   220,   221,     0,   222,   223,
       0,   224,   225,   226,   227,   228,     0,     0,     0,     0,
       0,   229,     0,   230,     0,     0,   231,   188,   189,   190,
       0,   191,   192,   193,   194,   195,   196,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     210,   211,   212,   213,     0,   214,   215,   216,     0,     0,
       0,   187,     0,     0,     0,     0,     0,   220,   221,     0,
     222,   223,     0,   224,   225,   226,   227,   228,     0,     0,
       0,     0,     0,   229,     0,   230,     0,     0,   231,   188,
     189,   190,     0,   191,   192,   193,   194,   195,   196,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   210,   211,   212,   213,     0,   214,   215,   216,
       0,     0,     0,   187,     0,     0,     0,     0,     0,     0,
     221,     0,   222,   223,     0,   224,   225,   226,   227,   228,
       0,     0,     0,     0,     0,   229,     0,   230,     0,     0,
     231,   188,   189,   190,     0,   191,   192,   193,   194,   195,
     196,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   210,   211,   212,   213,     0,   214,
     215,   216,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -337,     0,   222,   223,     0,   224,   225,   226,
     227,   228,     0,     0,     0,     0,     0,   229,     0,   230,
       0,     0,   231
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,     0,     0,    59,    18,    66,   139,   117,
     105,    23,    49,   345,   447,    27,     4,     3,    49,   185,
       3,   365,     4,     4,    11,    37,     4,    68,    40,    41,
     101,     3,    44,    45,    46,    47,    48,    96,   462,    26,
     359,   542,    33,   231,   376,    17,   231,    19,    20,     3,
      22,   510,    24,    53,    53,    53,    53,    53,     0,     3,
       0,   111,   103,   134,   571,    37,     0,    34,   101,    15,
      61,   121,   573,    45,    46,    47,    48,     3,    11,   123,
     123,   588,    94,    50,   128,   509,   545,   431,   123,    15,
      16,   150,   123,    26,   144,   145,   103,    26,   105,   111,
     275,   134,    28,    32,   423,   117,   111,   127,   128,   121,
     115,    53,   444,    53,     3,     4,     3,   123,   105,    53,
      15,   229,   110,   111,   290,   256,   127,   139,   100,   111,
       3,   319,   144,   145,   319,     3,     3,     0,   482,   127,
      94,   127,    15,    16,   127,   127,   127,    15,    16,   127,
      94,   127,   123,   101,   342,    28,    11,   342,   490,   123,
      28,   133,   123,   163,   163,   163,   163,   163,   128,   128,
     603,   183,     3,   103,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   105,
     532,   127,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   394,
     133,   163,   407,   163,   101,   237,   276,   277,   126,   163,
     101,   416,   398,   399,    90,     3,   273,    93,   238,   238,
     238,   253,     3,   116,   256,     3,   258,   127,   116,    15,
     123,    68,   125,   240,   362,   430,   129,     3,     3,   100,
      68,   129,   249,     3,   276,   277,   100,     3,     3,   125,
      15,    16,   128,     3,     3,    15,    16,   134,   101,   464,
       3,   123,    17,    28,    19,    20,   471,    22,    28,    24,
     128,   128,    15,    16,   123,   390,   346,   392,   134,   134,
     105,   134,    37,   434,   101,    28,     3,   133,   103,     3,
      45,    46,    47,    48,   373,    53,    53,     3,    15,    16,
     103,     3,     4,     5,   383,     7,     8,     9,   103,   341,
      90,    28,   134,    90,   346,    90,    18,   349,    35,   126,
     133,    23,   127,    25,     3,    27,    28,     3,   134,   101,
     362,   351,   351,   351,   449,   128,    15,    16,   533,   359,
     359,   359,    44,   133,   105,   100,   100,    15,    14,    28,
      52,   116,    54,    55,   361,   103,   116,    25,   134,   554,
     125,   123,    30,    31,   127,   125,   101,    90,    70,   443,
      15,    90,   404,   380,   133,    43,   408,    45,   133,   411,
      25,   413,   134,   390,   499,   392,    31,   127,   420,    91,
      92,   133,    65,   134,   426,   127,   101,     3,   111,   116,
      45,   127,   434,   423,   423,   423,     3,   439,   133,    15,
      16,    39,   103,   123,   609,   127,   134,   115,    15,    16,
     427,   123,    28,   125,     3,     4,   110,   129,     7,     8,
       9,    28,   133,   105,     3,     3,    25,   116,   133,    67,
      68,    69,   449,    71,    72,    73,    74,    75,    76,   548,
     115,   133,   127,   552,   553,   133,   127,   134,    90,    91,
      92,    93,    90,    91,    92,    93,   133,    95,    96,    97,
     133,     3,   111,   133,   123,   507,   575,   111,   133,   127,
     500,   134,   110,   111,   516,   113,   114,   115,   116,   117,
     133,   123,   499,   125,   593,   123,   128,   125,    53,   163,
     128,   351,    96,   454,   558,    59,   605,   143,     3,   382,
     116,   488,   488,    71,   534,   534,   534,   534,   534,   519,
     540,   563,    17,   582,    19,    20,   558,    22,   540,    24,
     519,   455,   480,   568,    26,   449,    47,   290,   427,    -1,
      -1,   257,    37,   563,   563,   563,   563,   563,    -1,   581,
      -1,    46,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   582,   582,   582,   582,   582,     0,    -1,    -1,
       3,     4,     5,    -1,     7,     8,     9,    10,    11,    12,
      13,    -1,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    27,    28,    -1,    -1,    39,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    40,    -1,    42,
      -1,    44,    -1,    46,    47,    48,    -1,    -1,    51,    52,
      -1,    54,    55,    -1,    -1,    -1,    67,    68,    69,    62,
      63,    64,    -1,    -1,    75,    76,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    -1,    95,    96,    97,    -1,    91,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,   110,
     111,    -1,   113,   114,   115,   116,   117,   110,    -1,    -1,
     113,    -1,   123,    -1,   125,   118,   119,   128,    -1,    -1,
     123,    -1,   125,    -1,   127,    -1,   129,     3,     4,     5,
     133,     7,     8,     9,    10,    11,    12,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    27,    28,    29,    -1,    31,    -1,    -1,    -1,    35,
      36,    37,    38,    -1,    40,    41,    42,    -1,    44,    45,
      46,    47,    48,    -1,    -1,    51,    52,    -1,    54,    55,
      -1,    -1,    -1,     3,    -1,    -1,    62,    63,    64,    -1,
      66,    -1,    -1,    -1,    70,    15,    -1,    17,    -1,    19,
      20,    -1,    22,    -1,    24,    25,    90,    91,    92,    93,
      30,    31,    -1,    -1,    -1,    91,    92,    37,    -1,    -1,
      -1,    -1,    -1,    43,   100,    45,    46,    47,    48,   113,
     114,   115,   116,   117,   110,    -1,    -1,   113,    -1,   123,
      -1,   125,   118,   119,   128,    -1,    -1,   123,    -1,   125,
      -1,   127,    -1,   129,     3,     4,     5,   133,     7,     8,
       9,    10,    11,    12,    13,    -1,    -1,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      29,    -1,    31,    -1,    -1,    -1,    35,    36,    37,    38,
      -1,    40,    41,    42,    -1,    44,    45,    46,    47,    48,
      -1,    -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    -1,    -1,
      -1,    70,    90,    91,    92,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    92,    -1,    -1,    -1,   115,   116,   117,
      -1,   100,    -1,    -1,    -1,   123,    -1,   125,    -1,    -1,
     128,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,    -1,
     129,     3,     4,     5,   133,     7,     8,     9,    10,    11,
      12,    13,    -1,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,    41,
      42,    -1,    44,    45,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,   123,    -1,   125,    -1,   127,    -1,   129,     3,     4,
       5,   133,     7,     8,     9,    10,    11,    12,    13,    -1,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    27,    28,    -1,    -1,    39,    -1,    -1,    -1,
      35,    -1,    37,    38,    -1,    40,    -1,    42,    -1,    44,
      -1,    46,    47,    48,    -1,    -1,    51,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    -1,    75,    76,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    -1,    95,    96,    97,    -1,    91,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,    -1,   110,   111,    -1,
     113,   114,   115,   116,   117,   110,    -1,    -1,   113,    -1,
     123,    -1,   125,   118,   119,   128,    -1,    -1,   123,    -1,
     125,    -1,   127,    -1,   129,     3,     4,     5,   133,     7,
       8,     9,    10,    11,    12,    13,    -1,    -1,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,
      38,    -1,    40,    -1,    42,    -1,    44,    -1,    46,    47,
      48,    -1,    -1,    51,    52,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,
     118,   119,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,
      -1,   129,     3,     4,     5,   133,     7,     8,     9,    10,
      11,    12,    13,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    27,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,    40,
      -1,    42,    -1,    44,    -1,    46,    47,    48,    -1,    -1,
      51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,    -1,   118,   119,    -1,
      -1,    -1,   123,    -1,   125,    -1,   127,    -1,   129,     3,
       4,     5,   133,     7,     8,     9,    10,    11,    12,    13,
      -1,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    37,    38,    -1,    40,    -1,    42,    -1,
      44,    -1,    46,    47,    48,    -1,    -1,    51,    52,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,   123,
      -1,   125,    -1,   127,    -1,   129,     3,     4,     5,   133,
       7,     8,     9,    10,    11,    12,    13,    -1,    -1,    -1,
      17,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,    -1,    40,    -1,    42,    -1,    44,    -1,    46,
      47,    48,    -1,    -1,    51,    52,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
      -1,   118,   119,    -1,    -1,    -1,   123,    -1,   125,    -1,
     127,    -1,   129,    -1,     0,    -1,   133,     3,     4,     5,
      -1,     7,     8,     9,    10,    11,    12,    13,    -1,    -1,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,    40,    -1,    42,    -1,    44,    -1,
      46,    47,    48,    -1,    -1,    51,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,    -1,   118,   119,    -1,    -1,    -1,   123,    -1,   125,
      -1,   127,    -1,   129,     3,     4,     5,    -1,     7,     8,
       9,    10,    11,    12,    13,    -1,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      29,    -1,    31,    -1,    -1,    -1,    35,    36,    37,    38,
      -1,    40,    41,    42,    -1,    44,    45,    46,    47,    48,
      -1,    -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,    -1,   118,
     119,    -1,    -1,    -1,   123,    -1,   125,    -1,   127,    -1,
     129,     3,     4,     5,    -1,     7,     8,     9,    10,    11,
      12,    13,    -1,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,    41,
      42,    -1,    44,    45,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,
      -1,   123,    -1,   125,    -1,   127,    -1,   129,     3,     4,
       5,    -1,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    -1,    23,    -1,
      25,    -1,    27,    28,    -1,    -1,    31,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    44,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,     8,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    18,    91,    92,    -1,    -1,
      23,    -1,    25,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    -1,   110,    -1,    -1,   113,    42,
      -1,    44,    -1,   118,   119,    -1,    -1,    -1,   123,    52,
     125,    54,    55,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    92,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,    -1,   118,   119,    -1,    -1,    -1,
     123,    -1,   125,    67,    68,    69,   129,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,   103,
     104,    -1,   106,   107,   108,    39,   110,   111,    -1,   113,
     114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,   103,
     104,    -1,   106,   107,   108,    39,   110,   111,    -1,   113,
     114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    -1,    -1,    39,    -1,   103,
     104,   105,   106,   107,   108,    -1,   110,   111,    -1,   113,
     114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,    -1,    -1,   128,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    -1,    -1,    39,
      -1,   103,   104,    -1,   106,   107,   108,    -1,   110,   111,
      -1,   113,   114,   115,   116,   117,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,   126,    -1,   128,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    -1,    95,    96,    97,    -1,    -1,
      -1,    39,    -1,   103,   104,    -1,   106,   107,   108,    -1,
     110,   111,    -1,   113,   114,   115,   116,   117,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,   126,    -1,   128,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    -1,    -1,    39,    -1,   103,   104,    -1,   106,   107,
     108,    -1,   110,   111,    -1,   113,   114,   115,   116,   117,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,   126,    -1,
     128,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    -1,    -1,    39,    -1,   103,   104,    -1,
     106,   107,   108,    -1,   110,   111,    -1,   113,   114,   115,
     116,   117,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
     126,    -1,   128,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    -1,    -1,    39,    -1,   103,
     104,    -1,   106,   107,   108,    -1,   110,   111,    -1,   113,
     114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,   126,    -1,   128,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    -1,    -1,    39,
      -1,   103,   104,   105,   106,   107,   108,    -1,   110,   111,
      -1,   113,   114,   115,   116,   117,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,    -1,    -1,   128,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    -1,    95,    96,    97,    -1,    -1,
      -1,    39,    -1,   103,   104,    -1,   106,   107,   108,    -1,
     110,   111,    -1,   113,   114,   115,   116,   117,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,   126,    -1,   128,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    -1,    -1,    39,    -1,   103,   104,    -1,   106,   107,
     108,    -1,   110,   111,    -1,   113,   114,   115,   116,   117,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,    -1,
     128,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    -1,    -1,    39,    -1,   103,   104,    -1,
     106,   107,   108,    -1,    -1,   111,    -1,   113,   114,   115,
     116,   117,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
      -1,    -1,   128,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,   106,   107,   108,    -1,   110,   111,    -1,   113,
     114,   115,   116,   117,    -1,    -1,    -1,    -1,    -1,   123,
      -1,   125,    -1,    -1,   128,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,   106,   107,   108,    -1,   110,   111,
      -1,   113,   114,   115,   116,   117,    -1,    -1,    -1,    -1,
      -1,   123,    -1,   125,    -1,    -1,   128,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    -1,    95,    96,    97,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,   107,   108,    -1,
     110,   111,    -1,   113,   114,   115,   116,   117,    -1,    -1,
      -1,    -1,    -1,   123,    -1,   125,    -1,    -1,   128,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,   110,   111,    -1,   113,   114,   115,   116,   117,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,    -1,
     128,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,   110,   111,    -1,   113,   114,   115,
     116,   117,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,
      -1,    -1,   128
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     4,     5,     7,     8,     9,    10,    11,    12,
      13,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    27,    28,    35,    37,    38,    40,    42,    44,    46,
      47,    48,    51,    52,    54,    55,    62,    63,    64,    70,
      91,    92,   100,   110,   113,   118,   119,   123,   125,   127,
     129,   136,   137,   138,   139,   145,   148,   150,   158,   161,
     162,   163,   164,   165,   172,   180,   183,   184,   187,   190,
     191,   192,   193,   197,   199,   208,   213,   214,   215,   226,
     233,   236,   249,   250,   251,   256,   260,   264,   265,   267,
     268,   269,    49,   123,   123,   155,   156,   123,     3,    15,
      16,    28,   127,   185,   186,    15,     3,   260,     3,   235,
     260,   123,     3,   186,   239,   260,   127,   123,   128,    15,
     260,   123,     3,     4,     7,     8,     9,   257,   258,   259,
     260,   260,   260,   260,   260,   255,   260,   245,   246,   247,
     253,   260,    29,    31,    36,    41,    45,    66,   100,   133,
     144,   145,   146,   150,   152,   166,   167,   182,   191,   252,
       3,     0,   139,   127,     3,    45,   152,   159,   160,   256,
     146,   147,    15,    25,    30,    31,    43,    45,   207,   266,
       3,   193,     3,   127,   220,   224,   101,    39,    67,    68,
      69,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    95,    96,    97,   103,   104,   106,
     107,   108,   110,   111,   113,   114,   115,   116,   117,   123,
     125,   128,   123,   254,   255,    11,   147,   173,   189,   127,
     128,   185,   186,   238,   239,   240,   248,   123,   254,   128,
     181,   245,     3,   103,   254,   133,   105,   101,   101,   134,
     126,   101,   253,     3,     3,   153,   154,   254,   254,   153,
       3,   133,   146,   127,   137,     3,   100,    68,   100,     3,
      34,    50,   232,     3,     3,   209,   260,     3,   220,   225,
     227,   228,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,     3,   116,   129,
     185,   262,   263,   260,   260,   260,   260,   260,   260,   260,
     260,   260,   260,   260,   260,   260,   260,   260,   245,   260,
     123,   125,   129,   262,   263,   134,   123,   260,   100,   125,
     140,   141,   142,   148,   150,   197,   199,   208,   233,   188,
     185,   128,   123,   244,     3,    94,   229,   230,   231,   134,
     116,   185,   143,   144,   134,   260,   134,   253,   258,   260,
     105,   243,   101,   144,   133,   243,   254,   254,   103,     3,
      53,   194,    53,   195,     3,   210,   211,   221,   103,   103,
     111,   115,   216,   228,   125,   262,   263,    90,   105,   134,
     126,   261,   260,   125,   262,   263,    90,   155,   254,   134,
     149,   133,   142,   127,   140,   245,   103,   105,   212,   231,
     134,   101,   133,   155,   105,    35,   116,   240,   242,   103,
     151,   154,   133,   100,   134,     3,     4,   123,   240,    14,
     196,   240,   241,   127,   243,   101,   133,     4,   220,     4,
     220,   217,   222,   260,    90,   116,   263,   260,   260,   126,
     260,    90,   263,   134,   127,   260,   140,   133,   134,   260,
     242,   243,    94,   231,    26,    32,   174,   176,   178,   179,
      65,   157,   253,   260,   256,   155,   229,   241,   127,   101,
     200,   151,   211,   111,     4,   110,   111,   127,   218,   223,
     226,   126,   263,   134,   126,   263,    33,    61,   168,   169,
     170,   171,   126,   133,   212,   127,   231,   127,   123,   174,
     176,   155,   134,   134,   198,   240,    45,   100,   191,   204,
     205,   206,   115,   260,   110,   226,   222,   260,   105,   133,
     170,   171,   237,   177,     3,   155,   243,   100,   125,   145,
     150,   191,   201,   202,   203,   208,   233,     3,    25,   133,
     206,   224,   219,   115,   222,   105,   143,   143,   143,   243,
     127,   149,   127,   133,   203,   232,   217,   133,   224,   143,
     133,   133,   134,   234,   260,   201,     3,   111,   217,   175,
     143,   126,   133,   123,   111,   127,   133,   229,   143,   134,
     133,   243
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
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
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
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
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


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

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
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
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

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
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
        
    case 13:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 302 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 24:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 314 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 25:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 315 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(2) - (4)].number_int);PASS1 as3_warning("embed command ignored");}
    }
    break;


  
    case 27:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 318 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 28:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 319 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 29:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 321 "parser.y"
    {
    (yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));
}
    }
    break;


  
    case 30:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 324 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 44:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 340 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (3)].code);}
    }
    break;


  
    case 45:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 341 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 46:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 344 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 53:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 351 "parser.y"
    {
    PASS_ALWAYS 
    if(as3_pass) {
        (yyval.code) = (yyvsp[(3) - (4)].code);
    } else {
        (yyval.code) = 0;
    }
    as3_pass=(yyvsp[(1) - (4)].number_int);
}
    }
    break;


  
    case 54:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 363 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (2)].code);}
    }
    break;


  
    case 55:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 364 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 56:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 368 "parser.y"
    {
    if((yyvsp[(1) - (1)].code)) {
        if(!global->init) 
            global->init = abc_initscript(global->file);
        code_t**cc = &global->init->method->body->code;
        *cc = code_append(*cc, (yyvsp[(1) - (1)].code));
    }
}
    }
    break;


  
    case 57:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 379 "parser.y"
    {
    PASS_ALWAYS
    (yyval.number_int) = as3_pass;
    as3_pass=0;
}
    }
    break;


  
    case 58:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 387 "parser.y"
    {
    PASS12
    (yyval.number_int)=as3_pass;
    char*key = concat3((yyvsp[(1) - (3)].id),"::",(yyvsp[(3) - (3)].id));
    if(!definitions || !dict_contains(definitions, key)) {
        as3_pass=0;
    }
    free(key);
}
    }
    break;


  
    case 59:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 414 "parser.y"
    {(yyval.node)=(yyvsp[(2) - (2)].node);}
    }
    break;


  
    case 60:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 415 "parser.y"
    {(yyval.node)=mkdummynode();}
    }
    break;


  
    case 61:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 417 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 62:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 418 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 63:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 420 "parser.y"
    {(yyval.code) = (yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 64:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 421 "parser.y"
    {(yyval.code) = code_append((yyvsp[(1) - (3)].code), (yyvsp[(3) - (3)].code));}
    }
    break;


  
    case 65:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 424 "parser.y"
    {
PASS12
    if(variable_exists((yyvsp[(1) - (3)].id))) 
        syntaxerror("Variable %s already defined", (yyvsp[(1) - (3)].id));
PASS1
    new_variable(state->method, (yyvsp[(1) - (3)].id), (yyvsp[(2) - (3)].classinfo), 1, 0);
PASS2
   
    char slot = 0;
    int index = 0;
    variable_t*v = 0;
    if(state->method->uses_slots) {
        v = find_slot(state->method, (yyvsp[(1) - (3)].id));
        if(v && !v->init) {
            // this variable is stored in a slot
            v->init = 1;
            v->type = (yyvsp[(2) - (3)].classinfo);
            slot = 1;
        }
    }
    if(!v) {
        v = new_variable2(state->method, (yyvsp[(1) - (3)].id), (yyvsp[(2) - (3)].classinfo), 1, 0);
    }

    (yyval.code) = slot?abc_getscopeobject(0, 1):0;
    
    typedcode_t val = node_read((yyvsp[(3) - (3)].node));
    if(!is_subtype_of(val.t, (yyvsp[(2) - (3)].classinfo))) {
        syntaxerror("Can't convert %s to %s", val.t->name, (yyvsp[(2) - (3)].classinfo)->name);
    }
    if((yyvsp[(2) - (3)].classinfo)) {
        if(val.c->prev || val.c->opcode != OPCODE_PUSHUNDEFINED) {
            (yyval.code) = code_append((yyval.code), val.c);
            (yyval.code) = converttype((yyval.code), val.t, (yyvsp[(2) - (3)].classinfo));
        } else {
            code_free(val.c);
            (yyval.code) = defaultvalue((yyval.code), (yyvsp[(2) - (3)].classinfo));
        }
    } else {
        if(val.c->prev || val.c->opcode != OPCODE_PUSHUNDEFINED) {
            (yyval.code) = code_append((yyval.code), val.c);
            (yyval.code) = abc_coerce_a((yyval.code));
        } else {
            // don't do anything
            code_free(val.c);
            code_free((yyval.code));
            (yyval.code) = 0;
            break;
        }
    }
    if(slot) {
        (yyval.code) = abc_setslot((yyval.code), v->index);
    } else {
        (yyval.code) = abc_setlocal((yyval.code), v->index);
        v->init = do_init_variable((yyvsp[(1) - (3)].id));
    }
}
    }
    break;


  
    case 66:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 484 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 67:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 484 "parser.y"
    {
    (yyval.code) = var_block((yyvsp[(2) - (2)].code), state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 68:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 488 "parser.y"
    {(yyval.code) = code_new();}
    }
    break;


  
    case 69:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 489 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 70:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 492 "parser.y"
    {
    (yyval.code) = code_new();
    (yyval.code) = code_append((yyval.code), (yyvsp[(3) - (6)].value).c);
    code_t*myjmp,*myif = (yyval.code) = abc_iffalse((yyval.code), 0);
   
    (yyval.code) = code_append((yyval.code), (yyvsp[(5) - (6)].code));
    if((yyvsp[(6) - (6)].code)) {
        myjmp = (yyval.code) = abc_jump((yyval.code), 0);
    }
    myif->branch = (yyval.code) = abc_nop((yyval.code));
    if((yyvsp[(6) - (6)].code)) {
        (yyval.code) = code_append((yyval.code), (yyvsp[(6) - (6)].code));
        myjmp->branch = (yyval.code) = abc_nop((yyval.code));
    }
}
    }
    break;


  
    case 71:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 508 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 74:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 515 "parser.y"
    {
    PASS1 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable(state->method, (yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1,0);
    PASS2 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable(state->method, (yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1,0);
}
    }
    break;


  
    case 75:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 519 "parser.y"
    {
    PASS12
    (yyval.id)=(yyvsp[(1) - (1)].id);
}
    }
    break;


  
    case 76:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 524 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (2)].id);(yyval.for_start).each=0;}
    }
    break;


  
    case 77:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 525 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (3)].id);(yyval.for_start).each=1;}
    }
    break;


  
    case 78:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 527 "parser.y"
    {
    if((yyvsp[(1) - (8)].for_start).each) syntaxerror("invalid syntax: ; not allowed in for each statement");
    (yyval.code) = code_new();
    (yyval.code) = code_append((yyval.code), (yyvsp[(2) - (8)].code));
    code_t*loopstart = (yyval.code) = abc_label((yyval.code));
    (yyval.code) = code_append((yyval.code), (yyvsp[(4) - (8)].value).c);
    code_t*myif = (yyval.code) = abc_iffalse((yyval.code), 0);
    (yyval.code) = code_append((yyval.code), (yyvsp[(8) - (8)].code));
    code_t*cont = (yyval.code) = abc_nop((yyval.code));
    (yyval.code) = code_append((yyval.code), (yyvsp[(6) - (8)].code));
    (yyval.code) = abc_jump((yyval.code), loopstart);
    code_t*out = (yyval.code) = abc_nop((yyval.code));
    breakjumpsto((yyval.code), (yyvsp[(1) - (8)].for_start).name, out);
    continuejumpsto((yyval.code), (yyvsp[(1) - (8)].for_start).name, cont);
    myif->branch = out;

    (yyval.code) = var_block((yyval.code), state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 79:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 547 "parser.y"
    {
    node_t*n = resolve_identifier((yyvsp[(2) - (6)].id));
    typedcode_t w = node_write(n);
    
    int it = alloc_local();
    int array = alloc_local();

    (yyval.code) = code_new();
    (yyval.code) = code_append((yyval.code), (yyvsp[(4) - (6)].value).c);
    (yyval.code) = abc_coerce_a((yyval.code));
    (yyval.code) = abc_setlocal((yyval.code), array);
    (yyval.code) = abc_pushbyte((yyval.code), 0);
    (yyval.code) = abc_setlocal((yyval.code), it);

    code_t*loopstart = (yyval.code) = abc_label((yyval.code));
    
    (yyval.code) = abc_hasnext2((yyval.code), array, it);
    code_t*myif = (yyval.code) = abc_iffalse((yyval.code), 0);
    (yyval.code) = abc_getlocal((yyval.code), array);
    (yyval.code) = abc_getlocal((yyval.code), it);
    if(!(yyvsp[(1) - (6)].for_start).each)
        (yyval.code) = abc_nextname((yyval.code));
    else
        (yyval.code) = abc_nextvalue((yyval.code));

    (yyval.code) = converttype((yyval.code), 0, w.t);
    (yyval.code) = code_append((yyval.code), w.c);

    (yyval.code) = code_append((yyval.code), (yyvsp[(6) - (6)].code));
    (yyval.code) = abc_jump((yyval.code), loopstart);
    
    code_t*out = (yyval.code) = abc_nop((yyval.code));
    breakjumpsto((yyval.code), (yyvsp[(1) - (6)].for_start).name, out);
    continuejumpsto((yyval.code), (yyvsp[(1) - (6)].for_start).name, loopstart);
    
    myif->branch = out;

    (yyval.code) = abc_kill((yyval.code), it);
    (yyval.code) = abc_kill((yyval.code), array);

    (yyval.code) = var_block((yyval.code), state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 80:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 591 "parser.y"
    {

    (yyval.code) = code_new();

    code_t*myjmp = (yyval.code) = abc_jump((yyval.code), 0);
    code_t*loopstart = (yyval.code) = abc_label((yyval.code));
    (yyval.code) = code_append((yyval.code), (yyvsp[(5) - (5)].code));
    code_t*cont = (yyval.code) = abc_nop((yyval.code));
    myjmp->branch = cont;
    (yyval.code) = code_append((yyval.code), (yyvsp[(3) - (5)].value).c);
    (yyval.code) = abc_iftrue((yyval.code), loopstart);
    code_t*out = (yyval.code) = abc_nop((yyval.code));
    breakjumpsto((yyval.code), (yyvsp[(1) - (5)].id), out);
    continuejumpsto((yyval.code), (yyvsp[(1) - (5)].id), cont);
}
    }
    break;


  
    case 81:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 607 "parser.y"
    {
    (yyval.code) = code_new();
    code_t*loopstart = (yyval.code) = abc_label((yyval.code));
    (yyval.code) = code_append((yyval.code), (yyvsp[(2) - (6)].code));
    code_t*cont = (yyval.code) = abc_nop((yyval.code));
    (yyval.code) = code_append((yyval.code), (yyvsp[(5) - (6)].value).c);
    (yyval.code) = abc_iftrue((yyval.code), loopstart);
    code_t*out = (yyval.code) = abc_nop((yyval.code));
    breakjumpsto((yyval.code), (yyvsp[(1) - (6)].id), out);
    continuejumpsto((yyval.code), (yyvsp[(1) - (6)].id), cont);
}
    }
    break;


  
    case 82:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 619 "parser.y"
    {
    (yyval.code) = abc___break__(0, "");
}
    }
    break;


  
    case 83:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 622 "parser.y"
    {
    (yyval.code) = abc___break__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 84:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 625 "parser.y"
    {
    (yyval.code) = abc___continue__(0, "");
}
    }
    break;


  
    case 85:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 628 "parser.y"
    {
    (yyval.code) = abc___continue__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 86:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 632 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 87:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 633 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 88:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 634 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 89:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 635 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 90:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 636 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 91:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 637 "parser.y"
    {(yyval.code)=code_append((yyval.code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 92:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 639 "parser.y"
    {
    (yyval.code) = abc_getlocal(0, state->switch_var);
    (yyval.code) = code_append((yyval.code), node_read((yyvsp[(2) - (4)].node)).c);
    code_t*j = (yyval.code) = abc_ifne((yyval.code), 0);
    (yyval.code) = code_append((yyval.code), (yyvsp[(4) - (4)].code));
    if((yyval.code)->opcode != OPCODE___BREAK__) {
        (yyval.code) = abc___fallthrough__((yyval.code), "");
    }
    code_t*e = (yyval.code) = abc_nop((yyval.code));
    j->branch = e;
}
    }
    break;


  
    case 93:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 650 "parser.y"
    {
    (yyval.code) = (yyvsp[(3) - (3)].code);
}
    }
    break;


  
    case 94:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 653 "parser.y"
    {PASS12 new_state();state->switch_var=alloc_local();}
    }
    break;


  
    case 95:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 653 "parser.y"
    {
    (yyval.code) = node_read((yyvsp[(4) - (8)].node)).c;
    (yyval.code) = abc_setlocal((yyval.code), state->switch_var);
    (yyval.code) = code_append((yyval.code), (yyvsp[(7) - (8)].code));

    code_t*out = (yyval.code) = abc_kill((yyval.code), state->switch_var);
    breakjumpsto((yyval.code), (yyvsp[(1) - (8)].id), out);
    
    code_t*c = (yyval.code),*lastblock=0;
    while(c) {
        if(c->opcode == OPCODE_IFNE) {
            if(!c->next) syntaxerror("internal error in fallthrough handling");
            lastblock=c->next;
        } else if(c->opcode == OPCODE___FALLTHROUGH__) {
            if(lastblock) {
                c->opcode = OPCODE_JUMP;
                c->branch = lastblock;
            } else {
                /* fall through end of switch */
                c->opcode = OPCODE_NOP;
            }
        }
        c=c->prev;
    }
   
    (yyval.code) = var_block((yyval.code), state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 96:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 684 "parser.y"
    {PASS12 new_state();
                                                      state->exception_name=(yyvsp[(3) - (5)].id);
                                               PASS1 new_variable(state->method, (yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].classinfo), 0, 0);
                                               PASS2 new_variable(state->method, (yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].classinfo), 0, 0);
                                              }
    }
    break;


  
    case 97:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 689 "parser.y"
    {
    namespace_t name_ns = {ACCESS_PACKAGE, ""};
    multiname_t name = {QNAME, &name_ns, 0, (yyvsp[(3) - (9)].id)};
    
    NEW(abc_exception_t, e)
    e->exc_type = sig2mname((yyvsp[(4) - (9)].classinfo));
    e->var_name = multiname_clone(&name);
    (yyval.exception) = e;

    code_t*c = 0;
    int i = find_variable_safe(state, (yyvsp[(3) - (9)].id))->index;
    e->target = c = abc_nop(0);
    c = abc_setlocal(c, i);
    c = code_append(c, code_dup(state->method->scope_code));
    c = code_append(c, (yyvsp[(8) - (9)].code));
    c = abc_kill(c, i);

    c = var_block(c, state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 98:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 709 "parser.y"
    {PASS12 new_state();state->exception_name=0;}
    }
    break;


  
    case 99:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 709 "parser.y"
    {
    (yyvsp[(4) - (5)].code) = var_block((yyvsp[(4) - (5)].code), state->vars);
    if(!(yyvsp[(4) - (5)].code)) {
        (yyval.exception)=0;
    } else {
        NEW(abc_exception_t, e)
        e->exc_type = 0; //all exceptions
        e->var_name = 0; //no name
        e->target = 0;
        e->to = abc_nop(0);
        e->to = code_append(e->to, (yyvsp[(4) - (5)].code));
        (yyval.exception) = e;
    }
    PASS12 old_state();
}
    }
    break;


  
    case 100:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 725 "parser.y"
    {(yyval.catch_list).l=list_new();(yyval.catch_list).finally=0;list_append((yyval.catch_list).l,(yyvsp[(1) - (1)].exception));}
    }
    break;


  
    case 101:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 726 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (2)].catch_list);list_append((yyval.catch_list).l,(yyvsp[(2) - (2)].exception));}
    }
    break;


  
    case 102:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 727 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (1)].catch_list);}
    }
    break;


  
    case 103:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 728 "parser.y"
    {
    (yyval.catch_list) = (yyvsp[(1) - (2)].catch_list);
    (yyval.catch_list).finally = 0;
    if((yyvsp[(2) - (2)].exception)) {
        list_append((yyval.catch_list).l,(yyvsp[(2) - (2)].exception));
        (yyval.catch_list).finally = (yyvsp[(2) - (2)].exception)->to;(yyvsp[(2) - (2)].exception)->to=0;
    }
}
    }
    break;


  
    case 104:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 736 "parser.y"
    {
    (yyval.catch_list).l=list_new();
    (yyval.catch_list).finally = 0;
    if((yyvsp[(1) - (1)].exception)) {
        list_append((yyval.catch_list).l,(yyvsp[(1) - (1)].exception));
        (yyval.catch_list).finally = (yyvsp[(1) - (1)].exception)->to;(yyvsp[(1) - (1)].exception)->to=0;
    }
}
    }
    break;


  
    case 105:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 745 "parser.y"
    {PASS12 new_state();
                 state->method->has_exceptions=1;
                 state->method->late_binding=1;//for invariant scope_code
                }
    }
    break;


  
    case 106:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 748 "parser.y"
    {
    code_t*out = abc_nop(0);

    code_t*start = abc_nop(0);
    (yyval.code) = code_append(start, (yyvsp[(4) - (6)].code));
    if(!is_break_or_jump((yyvsp[(4) - (6)].code))) {
        (yyval.code) = abc_jump((yyval.code), out);
    }
    code_t*end = (yyval.code) = abc_nop((yyval.code));
  
    int tmp;
    if((yyvsp[(6) - (6)].catch_list).finally)
        tmp = alloc_local();
    
    abc_exception_list_t*l = (yyvsp[(6) - (6)].catch_list).l;
    int count=0;
    while(l) {
        abc_exception_t*e = l->abc_exception;
        if(e->var_name) {
            (yyval.code) = code_append((yyval.code), e->target);
            (yyval.code) = abc_jump((yyval.code), out);
        } else {
            as3_assert((ptroff_t)(yyvsp[(6) - (6)].catch_list).finally);
            // finally block
            e->target = (yyval.code) = abc_nop((yyval.code));
            (yyval.code) = code_append((yyval.code), code_dup(state->method->scope_code));
            (yyval.code) = abc___rethrow__((yyval.code));
        }
        
        e->from = start;
        e->to = end;

        l = l->next;
    }
    (yyval.code) = code_append((yyval.code), out);

    (yyval.code) = insert_finally((yyval.code), (yyvsp[(6) - (6)].catch_list).finally, tmp);
        
    list_concat(state->method->exceptions, (yyvsp[(6) - (6)].catch_list).l);
   
    (yyval.code) = var_block((yyval.code), state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 107:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 794 "parser.y"
    {
    (yyval.code)=(yyvsp[(2) - (2)].value).c;
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 108:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 798 "parser.y"
    {
    if(!state->exception_name)
        syntaxerror("re-throw only possible within a catch block");
    variable_t*v = find_variable(state, state->exception_name);
    (yyval.code)=code_new();
    (yyval.code)=abc_getlocal((yyval.code), v->index);
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 109:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 809 "parser.y"
    {
     new_state();
     if(state->method->has_exceptions) {
         int v = alloc_local();
         state->method->scope_code = abc_getlocal(state->method->scope_code, v);
         state->method->scope_code = abc_pushwith(state->method->scope_code);
         (yyval.value_list).number = v;
     }
     (yyval.value_list).cc = (yyvsp[(3) - (4)].value).c;
}
    }
    break;


  
    case 110:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 819 "parser.y"
    {
     /* remove getlocal;pushwith from scope code again */
     state->method->scope_code = code_cutlast(code_cutlast(state->method->scope_code));

     (yyval.code) = (yyvsp[(1) - (2)].value_list).cc;
     if(state->method->has_exceptions) {
         (yyval.code) = abc_dup((yyval.code));
         (yyval.code) = abc_setlocal((yyval.code), (yyvsp[(1) - (2)].value_list).number);
     }
     (yyval.code) = abc_pushwith((yyval.code));
     (yyval.code) = code_append((yyval.code), (yyvsp[(2) - (2)].code));
     (yyval.code) = abc_popscope((yyval.code));
     old_state();
}
    }
    break;


  
    case 112:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 837 "parser.y"
    {PASS12 (yyval.id)="package";}
    }
    break;


  
    case 113:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 838 "parser.y"
    {PASS12 (yyval.id)="namespace";}
    }
    break;


  
    case 114:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 839 "parser.y"
    {PASS12 (yyval.id)="NaN";}
    }
    break;


  
    case 115:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 841 "parser.y"
    {PASS12 (yyval.id) = concat3((yyvsp[(1) - (3)].id),".",(yyvsp[(3) - (3)].id));free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;}
    }
    break;


  
    case 116:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 842 "parser.y"
    {PASS12 (yyval.id)=strdup((yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 117:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 844 "parser.y"
    {PASS12 startpackage((yyvsp[(2) - (3)].id));free((yyvsp[(2) - (3)].id));(yyvsp[(2) - (3)].id)=0;}
    }
    break;


  
    case 118:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 845 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 119:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 846 "parser.y"
    {PASS12 startpackage("");}
    }
    break;


  
    case 120:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 847 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 121:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 870 "parser.y"
    {
       PASS12
       slotinfo_t*s = registry_find(state->package, (yyvsp[(2) - (2)].id));
       if(!s && as3_pass==1) {as3_schedule_class(state->package, (yyvsp[(2) - (2)].id));}
       state_has_imports();
       dict_put(state->imports, state->package, (yyvsp[(2) - (2)].id));
       (yyval.code)=0;
}
    }
    break;


  
    case 122:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 878 "parser.y"
    {
       PASS12
       slotinfo_t*s = registry_find((yyvsp[(2) - (2)].classinfo)->package, (yyvsp[(2) - (2)].classinfo)->name);
       if(!s && as3_pass==1) {
           as3_schedule_class((yyvsp[(2) - (2)].classinfo)->package, (yyvsp[(2) - (2)].classinfo)->name);
       }
       /*if(s && s->kind == INFOTYPE_VAR && TYPE_IS_NAMESPACE(s->type)) {
	    trie_put(active_namespaces, (unsigned char*)$2->name, 0);
       }*/
       state_has_imports();
       dict_put(state->imports, (yyvsp[(2) - (2)].classinfo)->name, (yyvsp[(2) - (2)].classinfo));
       import_toplevel((yyvsp[(2) - (2)].classinfo)->package);
       (yyval.code)=0;
}
    }
    break;


  
    case 123:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 892 "parser.y"
    {
       PASS12
       if(strncmp("flash.", (yyvsp[(2) - (4)].id), 6) && as3_pass==1) {
           as3_schedule_package((yyvsp[(2) - (4)].id));
       }

       NEW(import_t,i);
       i->package = (yyvsp[(2) - (4)].id);
       state_has_imports();
       list_append(state->wildcard_imports, i);
       import_toplevel(i->package);
       (yyval.code)=0;
}
    }
    break;


  
    case 124:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 908 "parser.y"
    {PASS12 (yyval.flags).flags=0;(yyval.flags).ns=0;}
    }
    break;


  
    case 125:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 909 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 126:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 910 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 127:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 911 "parser.y"
    {
    PASS12 
    (yyval.flags).flags=(yyvsp[(1) - (2)].flags).flags|(yyvsp[(2) - (2)].flags).flags;
    if((yyvsp[(1) - (2)].flags).ns && (yyvsp[(2) - (2)].flags).ns) syntaxerror("only one namespace allowed in one declaration");
    (yyval.flags).ns=(yyvsp[(1) - (2)].flags).ns?(yyvsp[(1) - (2)].flags).ns:(yyvsp[(2) - (2)].flags).ns;

}
    }
    break;


  
    case 128:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 918 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PUBLIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 129:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 919 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PRIVATE;(yyval.flags).ns=0;}
    }
    break;


  
    case 130:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 920 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PROTECTED;(yyval.flags).ns=0;}
    }
    break;


  
    case 131:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 921 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_STATIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 132:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 922 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_DYNAMIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 133:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 923 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_FINAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 134:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 924 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_OVERRIDE;(yyval.flags).ns=0;}
    }
    break;


  
    case 135:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 925 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NATIVE;(yyval.flags).ns=0;}
    }
    break;


  
    case 136:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 926 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PACKAGEINTERNAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 137:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 927 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NAMESPACE;
                               (yyval.flags).ns=(yyvsp[(1) - (1)].id);
                       }
    }
    break;


  
    case 138:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 931 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 139:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 932 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 140:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 934 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 141:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 935 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 142:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 937 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 143:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 938 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 144:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 942 "parser.y"
    {PASS12 startclass(&(yyvsp[(1) - (6)].flags),(yyvsp[(3) - (6)].id),(yyvsp[(4) - (6)].classinfo),(yyvsp[(5) - (6)].classinfo_list));}
    }
    break;


  
    case 145:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 944 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 146:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 948 "parser.y"
    {PASS12 (yyvsp[(1) - (5)].flags).flags|=FLAG_INTERFACE;
                                          startclass(&(yyvsp[(1) - (5)].flags),(yyvsp[(3) - (5)].id),0,(yyvsp[(4) - (5)].classinfo_list));}
    }
    break;


  
    case 147:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 951 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 153:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 960 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 156:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 963 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(2) - (4)].number_int);PASS1 as3_warning("embed command ignored");}
    }
    break;


  
    case 157:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 965 "parser.y"
    {
    code_t*c = state->cls->static_init->header;
    c = code_append(c, (yyvsp[(1) - (1)].code));  
    state->cls->static_init->header = c;
}
    }
    break;


  
    case 163:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 976 "parser.y"
    {
    syntaxerror("variable declarations not allowed in interfaces");
}
    }
    break;


  
    case 164:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 979 "parser.y"
    {
    PASS12
    (yyvsp[(1) - (8)].flags).flags |= FLAG_PUBLIC;
    if((yyvsp[(1) - (8)].flags).flags&(FLAG_PRIVATE|FLAG_PACKAGEINTERNAL|FLAG_PROTECTED)) {
        syntaxerror("invalid method modifiers: interface methods always need to be public");
    }
    startfunction(&(yyvsp[(1) - (8)].flags),(yyvsp[(3) - (8)].token),(yyvsp[(4) - (8)].id),&(yyvsp[(6) - (8)].params),(yyvsp[(8) - (8)].classinfo));
    endfunction(&(yyvsp[(1) - (8)].flags),(yyvsp[(3) - (8)].token),(yyvsp[(4) - (8)].id),&(yyvsp[(6) - (8)].params),(yyvsp[(8) - (8)].classinfo), 0);
    list_deep_free((yyvsp[(6) - (8)].params).list);
}
    }
    break;


  
    case 167:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1054 "parser.y"
    {PASS12 setslotstate(&(yyvsp[(1) - (2)].flags),(yyvsp[(2) - (2)].token));}
    }
    break;


  
    case 168:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1054 "parser.y"
    {PASS12 (yyval.code)=(yyvsp[(4) - (4)].code);setslotstate(0, 0);}
    }
    break;


  
    case 169:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1056 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 170:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1057 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 171:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1060 "parser.y"
    {
PASS12
    int flags = slotstate_flags->flags;
    namespace_t ns = modifiers2access(slotstate_flags);

    if(as3_pass == 1) {

        varinfo_t* info = 0;
        if(state->cls) {
            memberinfo_t*i = registry_findmember(state->cls->info, ns.name, (yyvsp[(1) - (3)].id), 1, slotstate_flags->flags&FLAG_STATIC);
            if(i) {
                check_override(i, flags);
            }
            info = varinfo_register_onclass(state->cls->info, ns.access, ns.name, (yyvsp[(1) - (3)].id), slotstate_flags->flags&FLAG_STATIC);
        } else {
            slotinfo_t*i = registry_find(state->package, (yyvsp[(1) - (3)].id));
            if(i) {
                syntaxerror("package %s already contains '%s'", state->package, (yyvsp[(1) - (3)].id));
            }
            if(ns.name && ns.name[0]) {
                syntaxerror("namespaces not allowed on package-level variables");
            }
            info = varinfo_register_global(ns.access, state->package, (yyvsp[(1) - (3)].id));
        }

        info->type = (yyvsp[(2) - (3)].classinfo);
        info->flags = flags;
        
        dict_put(global->token2info, (void*)(ptroff_t)as3_tokencount, info);
    }

    if(as3_pass == 2) {
        varinfo_t*info = dict_lookup(global->token2info, (void*)(ptroff_t)as3_tokencount);

        multiname_t mname;
        code_t**code;
        trait_t*t = add_abc_slot(slotstate_flags, (yyvsp[(1) - (3)].id), &mname, &code);

        if((yyvsp[(2) - (3)].classinfo)) {
            MULTINAME(m, (yyvsp[(2) - (3)].classinfo));
            t->type_name = multiname_clone(&m);
        }
        info->slot = t->slot_id;
        
        /* workaround for "VerifyError: Error #1053: Illegal override of ::test2 in C1" 
           FIXME: is there a way to use slots and still don't have conflicting overrides?
        */
        info->slot = t->slot_id = 0;
       
        constant_t cval = (yyvsp[(3) - (3)].node)->type->eval((yyvsp[(3) - (3)].node));
        if(cval.type!=CONSTANT_UNKNOWN) {
            /* compile time constant */
            t->value = malloc(sizeof(constant_t));
            memcpy(t->value, &cval, sizeof(constant_t));
            info->value = constant_clone(t->value);
        } else {
            typedcode_t v = node_read((yyvsp[(3) - (3)].node));
            /* initalization code (if needed) */
            code_t*c = 0;
            if(v.c && !is_pushundefined(v.c)) {
                c = abc_getlocal_0(c);
                c = code_append(c, v.c);
                c = converttype(c, v.t, (yyvsp[(2) - (3)].classinfo));
                if(!t->slot_id) {
                    c = abc_initproperty2(c, &mname);
                } else {
                    c = abc_setslot(c, t->slot_id);
                }
            }
            *code = code_append(*code, c);
        }

        if(slotstate_varconst==KW_CONST) {
            t->kind= TRAIT_CONST;
            info->flags |= FLAG_CONST;
        }
    }

    (yyval.code)=0;
}
    }
    break;


  
    case 172:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1143 "parser.y"
    {(yyval.constant)=0;}
    }
    break;


  
    case 173:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1144 "parser.y"
    {
  (yyval.constant) = malloc(sizeof(constant_t));
  *(yyval.constant) = node_eval((yyvsp[(2) - (2)].node));
  if((yyval.constant)->type == CONSTANT_UNKNOWN) {
    syntaxerror("can't evaluate default parameter value (needs to be a compile-time constant)");
  }
}
    }
    break;


  
    case 174:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1152 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_int));}
    }
    break;


  
    case 175:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1153 "parser.y"
    {
    (yyval.constant) = constant_new_uint((yyvsp[(1) - (1)].number_uint));
}
    }
    break;


  
    case 176:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1156 "parser.y"
    {(yyval.constant) = constant_new_float((yyvsp[(1) - (1)].number_float));}
    }
    break;


  
    case 177:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1157 "parser.y"
    {(yyval.constant) = constant_new_string2((yyvsp[(1) - (1)].str).str,(yyvsp[(1) - (1)].str).len);free((char*)(yyvsp[(1) - (1)].str).str);}
    }
    break;


  
    case 178:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1158 "parser.y"
    {(yyval.constant) = constant_new_true((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 179:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1159 "parser.y"
    {(yyval.constant) = constant_new_false((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 180:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1160 "parser.y"
    {(yyval.constant) = constant_new_null((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 181:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1161 "parser.y"
    {(yyval.constant) = constant_new_undefined((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 182:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1162 "parser.y"
    {(yyval.constant) = constant_new_float(__builtin_nan(""));}
    }
    break;


  
    case 183:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1170 "parser.y"
    {
   multiname_t m = {QNAME, &stdns, 0, "XML"};
   typedcode_t v;
   v.c = 0;
   v.c = abc_getlex2(v.c, &m);
   v.c = code_append(v.c, node_read((yyvsp[(1) - (1)].node)).c);
   v.c = abc_construct(v.c, 1);
   v.t = TYPE_XML;
   (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 184:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1181 "parser.y"
    {PASS_ALWAYS if(!xml_level++) tokenizer_begin_xml();}
    }
    break;


  
    case 185:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1182 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xmltext();}
    }
    break;


  
    case 186:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1183 "parser.y"
    {PASS_ALWAYS if(!--xml_level) tokenizer_end_xml(); else tokenizer_begin_xmltext();}
    }
    break;


  
    case 187:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1185 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xmltext();}
    }
    break;


  
    case 188:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1185 "parser.y"
    {
    (yyval.node) = (yyvsp[(2) - (4)].node);
}
    }
    break;


  
    case 189:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1188 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xml();}
    }
    break;


  
    case 190:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1188 "parser.y"
    {
    (yyval.node) = (yyvsp[(2) - (4)].node);
}
    }
    break;


  
    case 191:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1191 "parser.y"
    {(yyval.node)=mkstringnode("");}
    }
    break;


  
    case 192:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1192 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node),(yyvsp[(2) - (2)].node));
}
    }
    break;


  
    case 193:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1195 "parser.y"
    {
    char* str = string_cstr(&(yyvsp[(2) - (2)].str));
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node),mkstringnode(str));
    free(str);
}
    }
    break;


  
    case 194:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1200 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node), mkstringnode(">"));
}
    }
    break;


  
    case 195:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1203 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node),(yyvsp[(2) - (2)].node));
}
    }
    break;


  
    case 196:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1206 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (3)].node), mkaddnode((yyvsp[(2) - (3)].node),(yyvsp[(3) - (3)].node)));
}
    }
    break;


  
    case 197:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1209 "parser.y"
    {
    (yyval.node) = mkstringnode((yyvsp[(1) - (1)].id));
}
    }
    break;


  
    case 198:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1212 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 199:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1216 "parser.y"
    {
    (yyval.node) = mkstringnode("");
}
    }
    break;


  
    case 200:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1219 "parser.y"
    {
    (yyval.node) = mkaddnode(mkstringnode(" "),(yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 201:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1223 "parser.y"
    {
    //$$ = allocprintf("<%s%s/>", $2, $3, $5, $8);
    (yyval.node) = mkaddnode(mkaddnode(mkaddnode(mkstringnode("<"),(yyvsp[(2) - (6)].node)),(yyvsp[(3) - (6)].node)),mkstringnode("/>"));
}
    }
    break;


  
    case 202:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1227 "parser.y"
    {
    //$$ = allocprintf("<%s%s>%s</%s>", $2, $3, $5, $8);
    (yyval.node) = mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(
         mkstringnode("<"),(yyvsp[(2) - (10)].node)),(yyvsp[(3) - (10)].node)),mkstringnode(">")),(yyvsp[(5) - (10)].node)),mkstringnode("</")),(yyvsp[(8) - (10)].node)),mkstringnode(">"));
}
    }
    break;


  
    case 203:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1232 "parser.y"
    {
    //$$ = allocprintf("<%s%s>%s%s</%s>", $2, $3, $5, $6, $9);
    (yyval.node) = mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(
         mkstringnode("<"),(yyvsp[(2) - (11)].node)),(yyvsp[(3) - (11)].node)),mkstringnode(">")),(yyvsp[(5) - (11)].node)),(yyvsp[(6) - (11)].node)),mkstringnode("</")),(yyvsp[(9) - (11)].node)),mkstringnode(">"));
}
    }
    break;


  
    case 204:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1238 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 205:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1241 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node), mkaddnode(mkstringnode(" "),(yyvsp[(2) - (2)].node)));
}
    }
    break;


  
    case 206:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1244 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 207:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1247 "parser.y"
    {
    char* str = string_cstr(&(yyvsp[(3) - (3)].str));
    (yyval.node) = mkaddnode((yyvsp[(1) - (3)].node), mkstringnode(concat2("=",str)));
    free(str);
}
    }
    break;


  
    case 208:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1252 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (3)].node), mkaddnode(mkstringnode("=\""), mkaddnode((yyvsp[(3) - (3)].node), mkstringnode("\""))));
}
    }
    break;


  
    case 209:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1255 "parser.y"
    {
    (yyval.node) = mkaddnode(mkaddnode(mkstringnode(concat2((yyvsp[(1) - (3)].id),"=\"")), (yyvsp[(3) - (3)].node)), mkstringnode("\""));
}
    }
    break;


  
    case 210:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1258 "parser.y"
    {
    char* str = string_cstr(&(yyvsp[(3) - (3)].str));
    (yyval.node)=mkstringnode(allocprintf("%s=%s", (yyvsp[(1) - (3)].id),str));
    free(str);
    free((yyvsp[(1) - (3)].id));free((char*)(yyvsp[(3) - (3)].str).str);
}
    }
    break;


  
    case 211:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1268 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
}
    }
    break;


  
    case 212:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1272 "parser.y"
    {
    PASS12
    (yyval.params)=(yyvsp[(1) - (1)].params);
}
    }
    break;


  
    case 213:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1278 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(2) - (2)].param));
}
    }
    break;


  
    case 214:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1284 "parser.y"
    {
    PASS12
    (yyval.params) =(yyvsp[(1) - (4)].params);
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(4) - (4)].param));
}
    }
    break;


  
    case 215:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1292 "parser.y"
    {
    PASS12
    (yyval.params) = (yyvsp[(1) - (3)].params);
    list_append((yyval.params).list, (yyvsp[(3) - (3)].param));
}
    }
    break;


  
    case 216:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1297 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    list_append((yyval.params).list, (yyvsp[(1) - (1)].param));
}
    }
    break;


  
    case 217:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1303 "parser.y"
    {
     PASS12
     (yyval.param) = rfx_calloc(sizeof(param_t));
     (yyval.param)->name=(yyvsp[(1) - (4)].id);
     (yyval.param)->type = (yyvsp[(3) - (4)].classinfo);
     PASS2
     (yyval.param)->value = (yyvsp[(4) - (4)].constant);
}
    }
    break;


  
    case 218:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1311 "parser.y"
    {
     PASS12
     (yyval.param) = rfx_calloc(sizeof(param_t));
     (yyval.param)->name=(yyvsp[(1) - (2)].id);
     (yyval.param)->type = TYPE_ANY;
     PASS2
     (yyval.param)->value = (yyvsp[(2) - (2)].constant);
}
    }
    break;


  
    case 221:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1321 "parser.y"
    {PASS12 (yyval.token)=0;}
    }
    break;


  
    case 222:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1324 "parser.y"
    {PASS12 startfunction(&(yyvsp[(1) - (9)].flags),(yyvsp[(3) - (9)].token),(yyvsp[(4) - (9)].id),&(yyvsp[(6) - (9)].params),(yyvsp[(8) - (9)].classinfo));}
    }
    break;


  
    case 223:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1325 "parser.y"
    {
    PASS1 
    endfunction(&(yyvsp[(1) - (12)].flags),(yyvsp[(3) - (12)].token),(yyvsp[(4) - (12)].id),&(yyvsp[(6) - (12)].params),0,0);
    PASS2
    if(!state->method->info) syntaxerror("internal error");
    
    code_t*c = method_header(state->method);
    c = wrap_function(c, 0, (yyvsp[(11) - (12)].code));

    endfunction(&(yyvsp[(1) - (12)].flags),(yyvsp[(3) - (12)].token),(yyvsp[(4) - (12)].id),&(yyvsp[(6) - (12)].params),(yyvsp[(8) - (12)].classinfo),c);
    PASS12
    list_deep_free((yyvsp[(6) - (12)].params).list);
    (yyval.code)=0;
}
    }
    break;


  
    case 225:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1341 "parser.y"
    {PASS12 (yyval.id)=0;}
    }
    break;


  
    case 226:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1343 "parser.y"
    {PASS12 innerfunction((yyvsp[(2) - (7)].id),&(yyvsp[(4) - (7)].params),(yyvsp[(6) - (7)].classinfo));}
    }
    break;


  
    case 227:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1344 "parser.y"
    {
    PASS1
    endfunction(0,0,(yyvsp[(2) - (10)].id),&(yyvsp[(4) - (10)].params),0,0);
    PASS2
    methodinfo_t*f = state->method->info;
    if(!f || !f->kind) syntaxerror("internal error");
    
    code_t*c = method_header(state->method);
    c = wrap_function(c, 0, (yyvsp[(9) - (10)].code));

    int index = state->method->var_index;
    endfunction(0,0,(yyvsp[(2) - (10)].id),&(yyvsp[(4) - (10)].params),(yyvsp[(6) - (10)].classinfo),c);
    
    (yyval.value).c = abc_getlocal(0, index);
    (yyval.value).t = TYPE_FUNCTION(f);

    PASS12 list_deep_free((yyvsp[(4) - (10)].params).list);
}
    }
    break;


  
    case 228:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1366 "parser.y"
    {
    PASS1 NEW(unresolvedinfo_t,c);
          memset(c, 0, sizeof(*c));
          c->kind = INFOTYPE_UNRESOLVED;
          c->name = (yyvsp[(1) - (1)].id);
          c->package = get_package_from_name((yyvsp[(1) - (1)].id));
          if(!c->package) {
              c->nsset = get_current_imports();
              /* make the compiler look for this class in the current directory,
                 just in case: */
              as3_schedule_class_noerror(state->package, (yyvsp[(1) - (1)].id));
          }
          (yyval.classinfo) = (classinfo_t*)c;
    PASS2
    slotinfo_t*s = find_class((yyvsp[(1) - (1)].id));
    if(!s) syntaxerror("Could not find class/method %s (current package: %s)\n", (yyvsp[(1) - (1)].id), state->package);
    (yyval.classinfo) = (classinfo_t*)s;
    registry_use(s);
}
    }
    break;


  
    case 229:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1386 "parser.y"
    {
    PASS1 NEW(unresolvedinfo_t,c);
          memset(c, 0, sizeof(*c));
          c->kind = INFOTYPE_UNRESOLVED;
          c->package = (yyvsp[(1) - (3)].id);
          c->name = (yyvsp[(3) - (3)].id);
          (yyval.classinfo) = (classinfo_t*)c;
    PASS2
    slotinfo_t*s = registry_find((yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].id));
    if(!s) syntaxerror("Couldn't find class/method %s.%s\n", (yyvsp[(1) - (3)].id), (yyvsp[(3) - (3)].id));
    free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;
    (yyval.classinfo) = (classinfo_t*)s;
    registry_use(s);
}
    }
    break;


  
    case 232:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1404 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();list_append((yyval.classinfo_list), (yyvsp[(1) - (1)].classinfo));}
    }
    break;


  
    case 233:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1405 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(1) - (3)].classinfo_list);list_append((yyval.classinfo_list),(yyvsp[(3) - (3)].classinfo));}
    }
    break;


  
    case 234:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1407 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(1) - (1)].classinfo);}
    }
    break;


  
    case 235:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1408 "parser.y"
    {PASS12 (yyval.classinfo)=TYPE_ANY;}
    }
    break;


  
    case 236:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1409 "parser.y"
    {PASS12 (yyval.classinfo)=TYPE_VOID;}
    }
    break;


  
    case 237:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1418 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 238:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1419 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 239:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1423 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 240:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1424 "parser.y"
    {(yyval.value_list)=(yyvsp[(2) - (3)].value_list);}
    }
    break;


  
    case 241:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1426 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 244:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1430 "parser.y"
    {(yyval.value_list).number=1;
                                                  (yyval.value_list).cc = (yyvsp[(1) - (1)].value).c;
                                                 }
    }
    break;


  
    case 245:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1434 "parser.y"
    {(yyval.value_list) = (yyvsp[(1) - (2)].value_list);}
    }
    break;


  
    case 246:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1435 "parser.y"
    {
                                                  (yyval.value_list).number= (yyvsp[(1) - (2)].value_list).number+1;
                                                  (yyval.value_list).cc = code_append((yyvsp[(1) - (2)].value_list).cc, (yyvsp[(2) - (2)].value).c);
                                                  }
    }
    break;


  
    case 248:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1441 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(2) - (4)].node));
    (yyval.value).c = v.c;
    if((yyval.value).c->opcode == OPCODE_COERCE_A) (yyval.value).c = code_cutlast((yyval.value).c);
    
    code_t*paramcode = (yyvsp[(4) - (4)].value_list).cc;
    if((yyval.value).c->opcode == OPCODE_GETPROPERTY) {
        multiname_t*name = (yyval.value).c->data[0];(yyval.value).c->data[0]=0;
        (yyval.value).c = code_cutlast((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_constructprop2((yyval.value).c, name, (yyvsp[(4) - (4)].value_list).number);
        multiname_destroy(name);
    } else if(is_getlocal((yyval.value).c)) {
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_construct((yyval.value).c, (yyvsp[(4) - (4)].value_list).number);
    } else if(TYPE_IS_CLASS(v.t) && v.t->data) {
        code_free((yyval.value).c);
        classinfo_t*c = v.t->data;
        MULTINAME(m, c);
        (yyval.value).c = abc_findpropstrict2(0, &m);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_constructprop2((yyval.value).c, &m, (yyvsp[(4) - (4)].value_list).number);
    /*} else if($$.c->opcode == OPCODE_GETSLOT) {
        int slot = (int)(ptroff_t)$$.c->data[0];
        trait_t*t = traits_find_slotid(state->cls->abc->traits,slot);//FIXME
        multiname_t*name = t->name;
        $$.c = code_cutlast($$.c);
        $$.c = code_append($$.c, paramcode);
        $$.c = abc_constructprop2($$.c, name, $4.number);*/
    } else {
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_construct((yyval.value).c, (yyvsp[(4) - (4)].value_list).number);
    }
   
    (yyval.value).t = TYPE_ANY;
    if(TYPE_IS_CLASS(v.t) && v.t->data) {
        (yyval.value).t = v.t->data;
    } else {
        (yyval.value).c = abc_coerce_a((yyval.value).c);
        (yyval.value).t = TYPE_ANY;
    }
}
    }
    break;


  
    case 249:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1488 "parser.y"
    {
   
    typedcode_t v = node_read((yyvsp[(1) - (4)].node));
    (yyval.value).c = v.c;
    if((yyval.value).c->opcode == OPCODE_COERCE_A) {
        (yyval.value).c = code_cutlast((yyval.value).c);
    }
    code_t*paramcode = (yyvsp[(3) - (4)].value_list).cc;

    (yyval.value).t = TYPE_ANY;
    if((yyval.value).c->opcode == OPCODE_GETPROPERTY) {
        multiname_t*name = (yyval.value).c->data[0];(yyval.value).c->data[0]=0;
        (yyval.value).c = code_cutlast((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_callproperty2((yyval.value).c, name, (yyvsp[(3) - (4)].value_list).number);
        multiname_destroy(name);
/*    } else if($$.c->opcode == OPCODE_GETSLOT && $$.c->prev->opcode != OPCODE_GETSCOPEOBJECT) {
        int slot = (int)(ptroff_t)$$.c->data[0];
        trait_t*t = traits_find_slotid(state->cls->abc->traits,slot);
        if(t->kind!=TRAIT_METHOD) {
            //ok: flash allows to assign closures to members.
        }
        multiname_t*name = t->name;
        $$.c = code_cutlast($$.c);
        $$.c = code_append($$.c, paramcode);
        //$$.c = abc_callmethod($$.c, t->method, len); //#1051 illegal early access binding
        $$.c = abc_callproperty2($$.c, name, $3.number);*/
    } else if((yyval.value).c->opcode == OPCODE_GETSUPER) {
        multiname_t*name = (yyval.value).c->data[0];(yyval.value).c->data[0]=0;
        (yyval.value).c = code_cutlast((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_callsuper2((yyval.value).c, name, (yyvsp[(3) - (4)].value_list).number);
        multiname_destroy(name);
    } else {
        (yyval.value).c = abc_getglobalscope((yyval.value).c);
        (yyval.value).c = code_append((yyval.value).c, paramcode);
        (yyval.value).c = abc_call((yyval.value).c, (yyvsp[(3) - (4)].value_list).number);
    }
   
    if(TYPE_IS_FUNCTION(v.t) && v.t->data) {
        (yyval.value).t = ((methodinfo_t*)(v.t->data))->return_type;
    } else if(TYPE_IS_CLASS(v.t) && v.t->data) {
        // calling a class is like a typecast
        (yyval.value).t = (classinfo_t*)v.t->data;
    } else {
        (yyval.value).t = TYPE_ANY;
        (yyval.value).c = abc_coerce_a((yyval.value).c);
    }
}
    }
    break;


  
    case 250:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1538 "parser.y"
    {
    if(!state->cls) syntaxerror("super() not allowed outside of a class");
    if(!state->method) syntaxerror("super() not allowed outside of a function");
    if(!state->method->is_constructor) syntaxerror("super() not allowed outside of a constructor");

    (yyval.value).c = code_new();
    (yyval.value).c = abc_getlocal_0((yyval.value).c);

    (yyval.value).c = code_append((yyval.value).c, (yyvsp[(3) - (4)].value_list).cc);
    /*
    this is dependent on the control path, check this somewhere else
    if(state->method->has_super)
        syntaxerror("constructor may call super() only once");
    */
    state->method->has_super = 1;

    (yyval.value).c = abc_constructsuper((yyval.value).c, (yyvsp[(3) - (4)].value_list).number);
    (yyval.value).c = abc_pushundefined((yyval.value).c);
    (yyval.value).t = TYPE_ANY;
}
    }
    break;


  
    case 251:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1559 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(2) - (2)].node));
    (yyval.value).c = v.c;
    if((yyval.value).c->opcode == OPCODE_COERCE_A) {
        (yyval.value).c = code_cutlast((yyval.value).c);
    }
    multiname_t*name = 0;
    if((yyval.value).c->opcode == OPCODE_GETPROPERTY) {
        (yyval.value).c->opcode = OPCODE_DELETEPROPERTY;
    } else if((yyval.value).c->opcode == OPCODE_GETSLOT) {
        int slot = (int)(ptroff_t)(yyval.value).c->data[0];
        multiname_t*name = traits_find_slotid(state->cls->abc->traits,slot)->name;
        (yyval.value).c = code_cutlast((yyval.value).c);
        (yyval.value).c = abc_deleteproperty2((yyval.value).c, name);
    } else {
        (yyval.value).c = abc_getlocal_0((yyval.value).c);
        MULTINAME_LATE(m, v.t?v.t->access:ACCESS_PACKAGE, "");
        (yyval.value).c = abc_deleteproperty2((yyval.value).c, &m);
    }
    (yyval.value).t = TYPE_BOOLEAN;
}
    }
    break;


  
    case 252:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1581 "parser.y"
    {
    (yyval.code) = abc_returnvoid(0);
}
    }
    break;


  
    case 253:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1584 "parser.y"
    {
    (yyval.code) = (yyvsp[(2) - (2)].value).c;
    (yyval.code) = abc_returnvalue((yyval.code));
}
    }
    break;


  
    case 254:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1591 "parser.y"
    {
    (yyval.value) = node_read((yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 255:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1594 "parser.y"
    {
    (yyval.value) = node_read((yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 256:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1597 "parser.y"
    {
    (yyval.node) = mkmultinode(&node_comma, (yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 257:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1600 "parser.y"
    {
    (yyval.node) = multinode_extend((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
}
    }
    break;


  
    case 258:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1603 "parser.y"
    { 
    (yyval.code) = node_exec((yyvsp[(1) - (1)].node)); 
}
    }
    break;


  
    case 259:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1606 "parser.y"
    { 
    (yyval.code) = (yyvsp[(1) - (3)].code);
    (yyval.code) = code_append((yyval.code), node_exec((yyvsp[(3) - (3)].node))); 
}
    }
    break;


  
    case 260:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1611 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 261:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1612 "parser.y"
    {(yyval.value_list)=(yyvsp[(1) - (1)].value_list);}
    }
    break;


  
    case 262:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1614 "parser.y"
    {(yyval.code)=abc_pushstring(0,(yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 263:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1615 "parser.y"
    {(yyval.code)=abc_pushstring2(0,&(yyvsp[(1) - (1)].str));}
    }
    break;


  
    case 264:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1616 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 265:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1617 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 266:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1618 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 267:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1620 "parser.y"
    {
    (yyval.value_list).cc = 0;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(1) - (3)].code));
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (3)].value).c);
    (yyval.value_list).number = 2;
}
    }
    break;


  
    case 268:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1626 "parser.y"
    {
    (yyval.value_list).cc = (yyvsp[(1) - (5)].value_list).cc;
    (yyval.value_list).number = (yyvsp[(1) - (5)].value_list).number+2;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (5)].code));
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(5) - (5)].value).c);
}
    }
    break;


  
    case 269:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1635 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 270:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1636 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 271:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1637 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 272:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1638 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 273:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1639 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 274:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1640 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    }
    break;


  
    case 275:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1642 "parser.y"
    { 
    (yyval.node) = mkconstnode((yyvsp[(1) - (1)].constant));
}
    }
    break;


  
    case 276:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1646 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 277:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1651 "parser.y"
    {
    typedcode_t v;
    v.c = 0;
    multiname_t m = {QNAME, &stdns, 0, "RegExp"};
    if(!(yyvsp[(1) - (1)].regexp).options) {
        v.c = abc_getlex2(v.c, &m);
        v.c = abc_pushstring(v.c, (yyvsp[(1) - (1)].regexp).pattern);
        v.c = abc_construct(v.c, 1);
    } else {
        v.c = abc_getlex2(v.c, &m);
        v.c = abc_pushstring(v.c, (yyvsp[(1) - (1)].regexp).pattern);
        v.c = abc_pushstring(v.c, (yyvsp[(1) - (1)].regexp).options);
        v.c = abc_construct(v.c, 2);
    }
    v.t = TYPE_REGEXP;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 278:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1669 "parser.y"
    {
    PASS1
    state->method->need_arguments = 1;
    PASS2
    typedcode_t v;
    v.c = abc_getlocal(0, state->method->need_arguments);
    v.t = TYPE_ARRAY;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 279:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1680 "parser.y"
    {
    typedcode_t v;
    v.c = code_new();
    v.c = code_append(v.c, (yyvsp[(2) - (3)].value_list).cc);
    v.c = abc_newarray(v.c, (yyvsp[(2) - (3)].value_list).number);
    v.t = registry_getarrayclass();
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 280:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1690 "parser.y"
    {
    typedcode_t v;
    v.c = code_new();
    v.c = code_append(v.c, (yyvsp[(2) - (3)].value_list).cc);
    v.c = abc_newobject(v.c, (yyvsp[(2) - (3)].value_list).number/2);
    v.t = registry_getobjectclass();
    (yyval.node) =  mkcodenode(v);
}
    }
    break;


  
    case 281:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1699 "parser.y"
    {(yyval.node) = mknode2(&node_lt,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 282:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1700 "parser.y"
    {(yyval.node) = mknode2(&node_gt,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 283:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1701 "parser.y"
    {(yyval.node) = mknode2(&node_le,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 284:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1702 "parser.y"
    {(yyval.node) = mknode2(&node_ge,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 285:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1703 "parser.y"
    {(yyval.node) = mknode2(&node_eqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 286:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1704 "parser.y"
    {(yyval.node) = mknode2(&node_eqeqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 287:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1705 "parser.y"
    {(yyval.node) = mknode2(&node_noteqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 288:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1706 "parser.y"
    {(yyval.node) = mknode2(&node_noteq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 289:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1707 "parser.y"
    {(yyval.node) = mknode2(&node_oror,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 290:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1708 "parser.y"
    {(yyval.node) = mknode2(&node_andand,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 291:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1709 "parser.y"
    {(yyval.node) = mknode1(&node_not, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 292:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1710 "parser.y"
    {(yyval.node) = mknode1(&node_bitnot, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 293:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1711 "parser.y"
    {(yyval.node) = mknode2(&node_bitand, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 294:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1712 "parser.y"
    {(yyval.node) = mknode2(&node_bitxor, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 295:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1713 "parser.y"
    {(yyval.node) = mknode2(&node_bitor, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 296:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1714 "parser.y"
    {(yyval.node) = mknode2(&node_shr, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 297:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1715 "parser.y"
    {(yyval.node) = mknode2(&node_ushr, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 298:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1716 "parser.y"
    {(yyval.node) = mknode2(&node_shl, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 299:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1717 "parser.y"
    {(yyval.node) = mknode2(&node_div, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 300:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1718 "parser.y"
    {(yyval.node) = mknode2(&node_mod, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 301:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1719 "parser.y"
    {(yyval.node) = mknode2(&node_plus, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 302:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1720 "parser.y"
    {(yyval.node) = mknode2(&node_minus, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 303:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1721 "parser.y"
    {(yyval.node) = mknode2(&node_multiply, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 304:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1722 "parser.y"
    {(yyval.node) = mknode2(&node_in, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 305:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1723 "parser.y"
    {(yyval.node) = mknode2(&node_as, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 306:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1724 "parser.y"
    {(yyval.node) = mknode2(&node_instanceof, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 307:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1725 "parser.y"
    {(yyval.node) = mknode2(&node_is, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 308:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1726 "parser.y"
    {(yyval.node) = mknode1(&node_typeof, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 309:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1727 "parser.y"
    {(yyval.node) = mknode1(&node_void, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 310:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1728 "parser.y"
    { (yyval.node) = mkconstnode(constant_new_undefined());}
    }
    break;


  
    case 311:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1729 "parser.y"
    { (yyval.node)=(yyvsp[(2) - (3)].node);}
    }
    break;


  
    case 312:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1730 "parser.y"
    {(yyval.node) = mknode1(&node_neg, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 313:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1731 "parser.y"
    {(yyval.node) = mknode2(&node_arraylookup, (yyvsp[(1) - (4)].node),(yyvsp[(3) - (4)].node));}
    }
    break;


  
    case 314:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1732 "parser.y"
    {(yyval.node) = mknode2(&node_muleq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 315:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1733 "parser.y"
    {(yyval.node) = mknode2(&node_modeq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 316:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1734 "parser.y"
    {(yyval.node) = mknode2(&node_shleq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 317:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1735 "parser.y"
    {(yyval.node) = mknode2(&node_shreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 318:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1736 "parser.y"
    {(yyval.node) = mknode2(&node_ushreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 319:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1737 "parser.y"
    { (yyval.node) = mknode2(&node_diveq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 320:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1738 "parser.y"
    { (yyval.node) = mknode2(&node_bitoreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 321:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1739 "parser.y"
    { (yyval.node) = mknode2(&node_bitxoreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 322:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1740 "parser.y"
    { (yyval.node) = mknode2(&node_bitandeq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 323:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1741 "parser.y"
    { (yyval.node) = mknode2(&node_pluseq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 324:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1742 "parser.y"
    { (yyval.node) = mknode2(&node_minuseq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 325:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1743 "parser.y"
    { (yyval.node) = mknode2(&node_assign, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 326:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1744 "parser.y"
    { (yyval.node) = mknode3(&node_tenary, (yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node));}
    }
    break;


  
    case 327:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1746 "parser.y"
    { (yyval.node) = mknode1(&node_rplusplus, (yyvsp[(1) - (2)].node));}
    }
    break;


  
    case 328:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1747 "parser.y"
    { (yyval.node) = mknode1(&node_rminusminus, (yyvsp[(1) - (2)].node));}
    }
    break;


  
    case 329:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1748 "parser.y"
    {(yyval.node) = mknode1(&node_lplusplus, (yyvsp[(2) - (2)].node)); }
    }
    break;


  
    case 330:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1749 "parser.y"
    {(yyval.node) = mknode1(&node_lminusminus, (yyvsp[(2) - (2)].node)); }
    }
    break;


  
    case 331:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1752 "parser.y"
    { if(!state->cls->info)
                  syntaxerror("super keyword not allowed outside a class");
              classinfo_t*t = state->cls->info->superclass;
              if(!t) t = TYPE_OBJECT;
              memberinfo_t*f = findmember_nsset(t, (yyvsp[(3) - (3)].id), 1, 0);
              MEMBER_MULTINAME(m, f, (yyvsp[(3) - (3)].id));
              typedcode_t v;
              v.c = 0;
              v.c = abc_getlocal_0(v.c);
              v.c = abc_getsuper2(v.c, &m);
              v.t = slotinfo_gettype((slotinfo_t*)f);
              (yyval.node) = mkcodenode(v);
           }
    }
    break;


  
    case 332:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1766 "parser.y"
    {
    typedcode_t v;
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(2) - (2)].id)};
    v.c = abc_getlex2(0, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 333:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1774 "parser.y"
    {PASS12 new_state();state->xmlfilter=1;}
    }
    break;


  
    case 334:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1774 "parser.y"
    {
    PASS1 old_state();
    PASS2
    typedcode_t v = node_read((yyvsp[(1) - (6)].node));
    typedcode_t w = node_read((yyvsp[(5) - (6)].node));
    code_t*c = 0;
    int index = alloc_local();
    int result = alloc_local();
    int tmp = alloc_local();
    int xml = alloc_local();
    
    c = code_append(c, v.c);
    c = abc_checkfilter(c);
    c = abc_coerce_a(c); //hasnext2 converts to *
    c = abc_setlocal(c, xml);
    multiname_t m = {QNAME, &stdns, 0, "XMLList"};
    c = abc_getlex2(c, &m);
    c = abc_construct(c, 0);
    c = abc_setlocal(c, result);
    c = abc_pushbyte(c, 0);
    c = abc_setlocal(c, index);
    code_t*jmp = c = abc_jump(c, 0);
    code_t*loop = c = abc_label(c);
    c = abc_getlocal(c, xml);
    c = abc_getlocal(c, index);
    c = abc_nextvalue(c);
    c = abc_dup(c);
    c = abc_setlocal(c, tmp);
    c = abc_pushwith(c);
    c = code_append(c, w.c);
    c = abc_popscope(c);
    code_t*b = c = abc_iffalse(c, 0);
    c = abc_getlocal(c, result);
    c = abc_getlocal(c, index);
    c = abc_getlocal(c, tmp);
    multiname_t m2 = {MULTINAMEL, 0, &nopackage_namespace_set, 0};
    c = abc_setproperty2(c, &m2);
    c = b->branch = jmp->branch = abc_nop(c);
    c = abc_kill(c, tmp);
    c = abc_hasnext2(c, xml, index);
    c = abc_iftrue(c, loop);
    c = abc_getlocal(c, result);
    c = abc_kill(c, xml);
    c = abc_kill(c, result);
    c = abc_kill(c, index);
    
    c = var_block(c, state->vars);
    old_state();
    typedcode_t r;
    r.c = c;
    r.t = TYPE_XMLLIST;
    (yyval.node) = mkcodenode(r);
}
    }
    break;


  
    case 335:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1828 "parser.y"
    {(yyval.id)=(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 336:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1829 "parser.y"
    {(yyval.id)="*";}
    }
    break;


  
    case 338:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1831 "parser.y"
    {(yyval.id)="*";}
    }
    break;


  
    case 339:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1872 "parser.y"
    {
}
    }
    break;


  
    case 340:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1875 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].id), (yyvsp[(5) - (5)].id), 0, 0);
}
    }
    break;


  
    case 341:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1878 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (3)].node));
    multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(3) - (3)].id)};
    v.c = abc_getdescendants2(v.c, &m);
    v.t = TYPE_XMLLIST;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 342:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1885 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].id), (yyvsp[(5) - (5)].id), 1, 0);
}
    }
    break;


  
    case 343:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1888 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (5)].node));
    typedcode_t w = node_read((yyvsp[(4) - (5)].node));
    multiname_t m = {MULTINAMEL, 0, &nopackage_namespace_set, 0};
    v.c = code_append(v.c, w.c);
    v.c = converttype(w.c, w.t, TYPE_STRING);
    v.c = abc_getproperty2(v.c, &m);
    v.t = TYPE_XMLLIST;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 344:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1899 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (4)].node));
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(4) - (4)].id)};
    v.c = abc_getproperty2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 345:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1907 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (6)].node), (yyvsp[(4) - (6)].id), (yyvsp[(6) - (6)].id), 0, 1);
}
    }
    break;


  
    case 346:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1911 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (4)].node));
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(4) - (4)].id)};
    v.c = abc_getdescendants2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 347:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1918 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (6)].node), (yyvsp[(4) - (6)].id), (yyvsp[(6) - (6)].id), 1, 1);
}
    }
    break;


  
    case 348:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1922 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (6)].node));
    typedcode_t w = node_read((yyvsp[(5) - (6)].node));
    multiname_t m = {MULTINAMELA, 0, &nopackage_namespace_set, 0};
    v.c = code_append(v.c, w.c);
    v.c = converttype(w.c, w.t, TYPE_STRING);
    v.c = abc_getproperty2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 349:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1932 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (6)].node));
    typedcode_t w = node_read((yyvsp[(5) - (6)].node));
    multiname_t m = {MULTINAMELA, 0, &nopackage_namespace_set, 0};
    v.c = code_append(v.c, w.c);
    v.c = converttype(w.c, w.t, TYPE_STRING);
    v.c = abc_getdescendants2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 350:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1943 "parser.y"
    {
    typedcode_t v1 = node_read((yyvsp[(1) - (3)].node));
    (yyval.value).c = v1.c;
    classinfo_t*t = v1.t;
    char is_static = 0;
    if(TYPE_IS_CLASS(t) && t->data) {
        t = t->data;
        is_static = 1;
    }
    if(TYPE_IS_XML(t) && !findmember_nsset(t, (yyvsp[(3) - (3)].id), 1, is_static)) {
        multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(3) - (3)].id)};
        (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
        (yyval.value).c = abc_coerce_a((yyval.value).c);
        (yyval.value).t = TYPE_XMLLIST;
    } else if(t) {
        if(t->subtype==INFOTYPE_UNRESOLVED) {
            syntaxerror("syntaxerror: trying to resolve property '%s' on incomplete object '%s'", (yyvsp[(3) - (3)].id), t->name);
        }
        memberinfo_t*f = findmember_nsset(t, (yyvsp[(3) - (3)].id), 1, is_static);
        char noslot = 0;
        if(f && !is_static != !(f->flags&FLAG_STATIC))
           noslot=1;
        if(f && f->slot && !noslot) {
            (yyval.value).c = abc_getslot((yyval.value).c, f->slot);
        } else {
            if(!f) {
                if(!TYPE_IS_XMLLIST(t)) {
                    as3_softwarning("Access of undefined property '%s' in %s", (yyvsp[(3) - (3)].id), t->name);
                }
            }
            MEMBER_MULTINAME(m, f, (yyvsp[(3) - (3)].id));
            (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
        }
        /* determine type */
        (yyval.value).t = slotinfo_gettype((slotinfo_t*)f);
        if(!(yyval.value).t)
           (yyval.value).c = abc_coerce_a((yyval.value).c);
        
    } else if(v1.c && v1.c->opcode == OPCODE___PUSHPACKAGE__) {
        string_t*package = v1.c->data[0];
        char*package2 = concat3(package->str, ".", (yyvsp[(3) - (3)].id));

        slotinfo_t*a = registry_find(package->str, (yyvsp[(3) - (3)].id));
        if(a) {
            (yyval.value) = push_class(a);
        } else if(dict_contains(state->import_toplevel_packages, package2) ||
                  registry_ispackage(package2)) {
            (yyval.value).c = v1.c;
            (yyval.value).c->data[0] = string_new4(package2);
            (yyval.value).t = 0;
        } else {
            syntaxerror("couldn't resolve %s", package2);
        }
    } else {
        /* when resolving a property on an unknown type, we do know the
           name of the property (and don't seem to need the package), but
           we need to make avm2 try out all access modes */
        as3_softwarning("Resolving %s on unknown type", (yyvsp[(3) - (3)].id));
        multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(3) - (3)].id)};
        (yyval.value).c = abc_getproperty2((yyval.value).c, &m);
        (yyval.value).c = abc_coerce_a((yyval.value).c);
        (yyval.value).t = TYPE_ANY;
    }
}
    }
    break;


  
    case 351:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2140 "parser.y"
    {
    PASS1
    /* Queue unresolved identifiers for checking against the parent
       function's variables.
       We consider everything which is not a local variable "unresolved".
       This encompasses class names, members of the surrounding class
       etc. which is *correct* because local variables of the parent function
       would shadow those.
       */

    if(!find_variable(state, (yyvsp[(1) - (1)].id))) {
        unknown_variable((yyvsp[(1) - (1)].id));
        /* let the compiler know that it might want to check the current directory/package
           for this identifier- maybe there's a file $1.as defining $1. */
        as3_schedule_class_noerror(state->package, (yyvsp[(1) - (1)].id));
    }
   
    (yyval.node) = 0;
    PASS2

    (yyval.node) = resolve_identifier((yyvsp[(1) - (1)].id));
}
    }
    break;


  
    case 352:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2174 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (2)].id);
    n->url = (yyvsp[(2) - (2)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 353:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2181 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 354:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2188 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].str).str;
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 355:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2195 "parser.y"
    {
    PASS12
    dict_put(state->namespaces, (unsigned char*)(yyvsp[(2) - (2)].namespace_decl)->name, (void*)(yyvsp[(2) - (2)].namespace_decl)->url);

    namespace_t access = modifiers2access(&(yyvsp[(1) - (2)].flags));
    varinfo_t* var = varinfo_register_global(access.access, state->package, (yyvsp[(2) - (2)].namespace_decl)->name);
    var->type = TYPE_NAMESPACE;
    namespace_t ns;
    ns.access = ACCESS_NAMESPACE;
    ns.name = (yyvsp[(2) - (2)].namespace_decl)->url;
    var->value = constant_new_namespace(&ns);
      
    if(as3_pass==2) {
        MULTINAME(m, TYPE_NAMESPACE);
        trait_t*t = add_abc_slot(&(yyvsp[(1) - (2)].flags), (yyvsp[(2) - (2)].namespace_decl)->name, 0, 0);
        t->value = var->value;
        t->type_name = multiname_clone(&m);
    }

    (yyval.code)=0;
}
    }
    break;


  
    case 356:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2218 "parser.y"
    {
    (yyval.code) = 0;
    (yyval.code) = code_append((yyval.code), node_read((yyvsp[(4) - (4)].node)).c);
    (yyval.code) = abc_dxnslate((yyval.code));
}
    }
    break;


  
    case 357:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2224 "parser.y"
    {
    PASS12

    varinfo_t*s = (varinfo_t*)(yyvsp[(3) - (3)].classinfo);
    if(s->kind == INFOTYPE_UNRESOLVED) {
        s = (varinfo_t*)registry_resolve((slotinfo_t*)s);
        if(!s)
            syntaxerror("Couldn't resolve namespace %s", (yyvsp[(3) - (3)].classinfo)->name);
    }

    if(!s || s->kind != INFOTYPE_VAR)
        syntaxerror("%s.%s is not a public namespace (%d)", (yyvsp[(3) - (3)].classinfo)->package, (yyvsp[(3) - (3)].classinfo)->name, s?s->kind:-1);
    if(!s->value || !NS_TYPE(s->value->type))
        syntaxerror("%s.%s is not a namespace", (yyvsp[(3) - (3)].classinfo)->package, (yyvsp[(3) - (3)].classinfo)->name);

    const char*url = s->value->ns->name;
    dict_put(state->namespaces, (unsigned char*)(yyvsp[(3) - (3)].classinfo)->name, (void*)url);
    add_active_url(url);
    (yyval.code)=0;
}
    }
    break;




/* Line 1464 of skeleton.m4  */
#line 7315 "parser.tab.c"
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
      /* If just tried and failed to reuse lookahead token after an
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

  /* Else will try to reuse lookahead token after shifting the error
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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
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



