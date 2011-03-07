
/* A Bison parser, made by GNU Bison 2.4.2.  */

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
#define YYBISON_VERSION "2.4.2"

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
/* Line 223 of skeleton.m4  */
#line 48 "parser.y"
tokenunion
{

/* Line 223 of skeleton.m4  */
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



/* Line 223 of skeleton.m4  */
#line 287 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 273 of skeleton.m4  */
#line 277 "parser.y"


#define PASS1 }} if(as3_pass == 1) {{
#define PASS1END }} if(as3_pass == 2) {{
#define PASS2 }} if(as3_pass == 2) {{
#define PASS12 }} if(as3_pass == 1 || as3_pass == 2) {{
#define PASS12END }} if(as3_pass == 2) {{
#define PASS_ALWAYS }} {{



/* Line 273 of skeleton.m4  */
#line 311 "parser.tab.c"
/* Unqualified %code blocks.  */

/* Line 274 of skeleton.m4  */
#line 403 "parser.y"

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
#line 854 "parser.y"

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
#line 999 "parser.y"

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
#line 1173 "parser.y"

    static int xml_level = 0;


/* Line 274 of skeleton.m4  */
#line 1841 "parser.y"

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
#line 2016 "parser.y"

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
#line 2175 "parser.y"

    void add_active_url(const char*url)
    {
        NEW(namespace_t,n);
        n->name = url;
        list_append(state->active_namespace_urls, n);
    }



/* Line 274 of skeleton.m4  */
#line 609 "parser.tab.c"

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
#define YYFINAL  158
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3127

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  137
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  134
/* YYNRULES -- Number of rules.  */
#define YYNRULES  350
/* YYNRULES -- Number of states.  */
#define YYNSTATES  593

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   366

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   120,     2,     2,     2,   118,   109,     2,
     124,   136,   117,   115,   102,   114,   129,   116,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   106,   101,
     111,   104,   112,   105,   130,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   126,     2,   127,   108,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   128,   107,   135,   119,     2,     2,     2,
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
      95,    96,    97,    98,    99,   100,   103,   110,   113,   121,
     122,   123,   125,   131,   132,   133,   134
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     8,    10,    13,    15,    17,
      19,    21,    23,    25,    27,    28,    30,    32,    35,    37,
      39,    41,    43,    45,    50,    52,    54,    55,    58,    60,
      62,    64,    66,    68,    70,    72,    74,    76,    78,    80,
      82,    84,    86,    90,    93,    95,    97,    99,   101,   103,
     105,   107,   110,   112,   114,   115,   118,   119,   122,   125,
     127,   131,   135,   136,   139,   140,   143,   150,   151,   153,
     155,   159,   161,   164,   168,   177,   184,   190,   197,   199,
     202,   204,   207,   208,   210,   212,   215,   217,   220,   225,
     229,   230,   239,   240,   250,   251,   257,   259,   262,   264,
     267,   269,   270,   277,   280,   282,   287,   290,   292,   294,
     296,   298,   302,   304,   305,   312,   313,   319,   322,   325,
     330,   331,   333,   335,   338,   340,   342,   344,   346,   348,
     350,   352,   354,   356,   357,   360,   361,   364,   365,   368,
     369,   379,   380,   389,   390,   392,   394,   397,   399,   401,
     403,   408,   410,   411,   413,   415,   418,   420,   423,   432,
     434,   436,   437,   442,   444,   448,   452,   453,   456,   458,
     460,   462,   464,   466,   468,   470,   472,   474,   476,   478,
     480,   481,   482,   487,   488,   493,   494,   497,   500,   503,
     506,   510,   512,   514,   515,   517,   524,   535,   547,   549,
     552,   554,   558,   562,   566,   570,   571,   573,   576,   581,
     585,   587,   592,   595,   597,   599,   600,   601,   614,   616,
     617,   618,   629,   631,   635,   637,   639,   641,   645,   647,
     649,   651,   654,   655,   656,   660,   661,   663,   665,   667,
     670,   673,   674,   679,   684,   689,   692,   694,   697,   699,
     701,   703,   707,   709,   713,   714,   716,   718,   720,   722,
     724,   726,   730,   736,   738,   740,   742,   744,   746,   748,
     750,   752,   754,   756,   760,   764,   768,   772,   776,   780,
     784,   788,   792,   796,   800,   804,   807,   810,   814,   818,
     822,   826,   830,   834,   838,   842,   846,   850,   854,   858,
     862,   866,   870,   873,   876,   878,   882,   885,   890,   894,
     898,   902,   906,   910,   914,   918,   922,   926,   930,   934,
     938,   944,   947,   950,   953,   956,   960,   963,   964,   971,
     973,   975,   977,   981,   987,   991,   997,  1003,  1008,  1015,
    1020,  1027,  1034,  1041,  1045,  1047,  1050,  1055,  1060,  1063,
    1068
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     138,     0,    -1,   139,    -1,    -1,   140,    -1,   141,    -1,
     140,   141,    -1,   188,    -1,   200,    -1,   198,    -1,   234,
      -1,   209,    -1,   150,    -1,   101,    -1,    -1,   143,    -1,
     144,    -1,   143,   144,    -1,   200,    -1,   198,    -1,   234,
      -1,   209,    -1,   150,    -1,   126,   151,   261,   127,    -1,
     101,    -1,   146,    -1,    -1,   146,   148,    -1,   148,    -1,
     269,    -1,   191,    -1,   163,    -1,   164,    -1,   165,    -1,
     166,    -1,   173,    -1,   159,    -1,   185,    -1,   181,    -1,
     257,    -1,   270,    -1,   268,    -1,   128,   146,   135,    -1,
     128,   135,    -1,   101,    -1,   147,    -1,   153,    -1,   167,
      -1,   168,    -1,   253,    -1,   183,    -1,   148,   101,    -1,
     148,    -1,   147,    -1,    -1,   104,   261,    -1,    -1,    45,
     154,    -1,    31,   154,    -1,   155,    -1,   154,   102,   155,
      -1,     3,   244,   152,    -1,    -1,   157,   149,    -1,    -1,
      65,   156,    -1,    64,   124,   255,   136,   156,   158,    -1,
      -1,   153,    -1,   257,    -1,    45,     3,   244,    -1,     3,
      -1,    10,   124,    -1,    10,    49,   124,    -1,   162,   160,
     101,   255,   101,   257,   136,   156,    -1,   162,   161,    68,
     255,   136,   156,    -1,    11,   124,   255,   136,   156,    -1,
      12,   156,    11,   124,   255,   136,    -1,    66,    -1,    66,
       3,    -1,    29,    -1,    29,     3,    -1,    -1,   170,    -1,
     172,    -1,   170,   172,    -1,   171,    -1,   170,   171,    -1,
      33,   261,   106,   145,    -1,    61,   106,   145,    -1,    -1,
      13,   124,   174,   261,   136,   128,   169,   135,    -1,    -1,
      32,   124,     3,   244,   136,   176,   128,   145,   135,    -1,
      -1,    26,   128,   178,   145,   135,    -1,   175,    -1,   179,
     175,    -1,   179,    -1,   179,   177,    -1,   177,    -1,    -1,
      51,   128,   182,   145,   135,   180,    -1,    36,   255,    -1,
      36,    -1,    38,   124,   255,   136,    -1,   184,   149,    -1,
       3,    -1,    16,    -1,    15,    -1,    28,    -1,   187,   129,
     186,    -1,   186,    -1,    -1,    16,   187,   128,   189,   142,
     135,    -1,    -1,    16,   128,   190,   142,   135,    -1,    40,
       3,    -1,    40,   240,    -1,    40,   187,   129,   117,    -1,
      -1,   193,    -1,   194,    -1,   193,   194,    -1,    19,    -1,
      20,    -1,    17,    -1,    37,    -1,    46,    -1,    48,    -1,
      47,    -1,    24,    -1,    22,    -1,    -1,    53,   241,    -1,
      -1,    53,   242,    -1,    -1,    14,   242,    -1,    -1,   192,
      30,     3,   195,   197,   128,   199,   202,   135,    -1,    -1,
     192,    43,     3,   196,   128,   201,   205,   135,    -1,    -1,
     203,    -1,   204,    -1,   203,   204,    -1,   101,    -1,   209,
      -1,   234,    -1,   126,   151,   261,   127,    -1,   147,    -1,
      -1,   206,    -1,   207,    -1,   206,   207,    -1,   101,    -1,
      45,     3,    -1,   192,    25,   233,     3,   124,   230,   136,
     244,    -1,    45,    -1,    31,    -1,    -1,   192,   208,   210,
     211,    -1,   212,    -1,   211,   102,   212,    -1,     3,   244,
     152,    -1,    -1,   104,   261,    -1,     7,    -1,     8,    -1,
       9,    -1,     4,    -1,    55,    -1,    54,    -1,    44,    -1,
      27,    -1,    28,    -1,   227,    -1,   111,    -1,   112,    -1,
      -1,    -1,   128,   261,   220,   135,    -1,    -1,   128,   261,
     222,   135,    -1,    -1,   223,   219,    -1,   223,     4,    -1,
     223,   112,    -1,   227,   223,    -1,   224,   227,   223,    -1,
       3,    -1,   221,    -1,    -1,   228,    -1,   216,   225,   226,
     116,   218,   112,    -1,   216,   225,   226,   217,   223,   111,
     116,   225,   218,   112,    -1,   216,   225,   226,   217,   223,
     224,   111,   116,   225,   218,   112,    -1,   229,    -1,   228,
     229,    -1,   221,    -1,   221,   104,     4,    -1,   221,   104,
     221,    -1,     3,   104,   221,    -1,     3,   104,     4,    -1,
      -1,   231,    -1,    94,   232,    -1,   231,   102,    94,   232,
      -1,   231,   102,   232,    -1,   232,    -1,     3,   106,   243,
     213,    -1,     3,   213,    -1,    50,    -1,    34,    -1,    -1,
      -1,   192,    25,   233,     3,   124,   230,   136,   244,   128,
     235,   145,   135,    -1,     3,    -1,    -1,    -1,    25,   236,
     124,   230,   136,   244,   128,   238,   145,   135,    -1,   186,
      -1,   187,   129,   186,    -1,   240,    -1,   239,    -1,   241,
      -1,   242,   102,   241,    -1,   241,    -1,   117,    -1,    35,
      -1,   106,   243,    -1,    -1,    -1,   124,   246,   136,    -1,
      -1,   247,    -1,   248,    -1,   254,    -1,   247,   102,    -1,
     248,   254,    -1,    -1,    23,   261,   249,   245,    -1,   261,
     124,   246,   136,    -1,    52,   124,   246,   136,    -1,    63,
     261,    -1,    41,    -1,    41,   255,    -1,   261,    -1,   256,
      -1,   261,    -1,   256,   102,   261,    -1,   261,    -1,   257,
     102,   261,    -1,    -1,   260,    -1,     3,    -1,     4,    -1,
       7,    -1,     8,    -1,     9,    -1,   259,   106,   254,    -1,
     260,   102,   259,   106,   254,    -1,   237,    -1,   265,    -1,
     250,    -1,   252,    -1,   251,    -1,   266,    -1,   214,    -1,
     215,    -1,     5,    -1,    18,    -1,   126,   246,   127,    -1,
      70,   258,   135,    -1,   261,   111,   261,    -1,   261,   112,
     261,    -1,   261,    75,   261,    -1,   261,    76,   261,    -1,
     261,    71,   261,    -1,   261,    72,   261,    -1,   261,    74,
     261,    -1,   261,    73,   261,    -1,   261,    88,   261,    -1,
     261,    89,   261,    -1,   120,   261,    -1,   119,   261,    -1,
     261,   109,   261,    -1,   261,   108,   261,    -1,   261,   107,
     261,    -1,   261,    97,   261,    -1,   261,    96,   261,    -1,
     261,    95,   261,    -1,   261,   116,   261,    -1,   261,   118,
     261,    -1,   261,   115,   261,    -1,   261,   114,   261,    -1,
     261,   117,   261,    -1,   261,    68,   261,    -1,   261,    69,
     261,    -1,   261,    39,   261,    -1,   261,    67,   261,    -1,
      42,   261,    -1,    35,   261,    -1,    35,    -1,   124,   256,
     136,    -1,   114,   261,    -1,   261,   126,   261,   127,    -1,
     261,    80,   261,    -1,   261,    79,   261,    -1,   261,    86,
     261,    -1,   261,    85,   261,    -1,   261,    87,   261,    -1,
     261,    78,   261,    -1,   261,    77,   261,    -1,   261,    84,
     261,    -1,   261,    81,   261,    -1,   261,    82,   261,    -1,
     261,    83,   261,    -1,   261,   104,   261,    -1,   261,   105,
     261,   106,   261,    -1,   261,    92,    -1,   261,    91,    -1,
      92,   261,    -1,    91,   261,    -1,    52,   129,     3,    -1,
     130,     3,    -1,    -1,   261,   129,   124,   262,   261,   136,
      -1,   117,    -1,   186,    -1,   117,    -1,   261,    90,   261,
      -1,   261,   129,   263,    90,   264,    -1,   261,    93,   264,
      -1,   261,    93,   263,    90,   264,    -1,   261,   129,   126,
     261,   127,    -1,   261,   129,   130,   264,    -1,   261,   129,
     130,   263,    90,   264,    -1,   261,    93,   130,   264,    -1,
     261,    93,   130,   263,    90,   264,    -1,   261,   129,   130,
     126,   261,   127,    -1,   261,    93,   130,   126,   261,   127,
      -1,   261,   129,   264,    -1,     3,    -1,    15,     3,    -1,
      15,     3,   104,     3,    -1,    15,     3,   104,     4,    -1,
     192,   267,    -1,    62,    15,   104,   261,    -1,    21,    15,
     241,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   292,   292,   294,   294,   295,   296,   298,   299,   300,
     301,   302,   303,   305,   307,   307,   308,   309,   311,   312,
     313,   314,   315,   317,   318,   320,   321,   323,   326,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   346,   347,   348,   349,   350,   351,
     352,   367,   368,   372,   383,   418,   419,   421,   422,   424,
     425,   427,   488,   488,   492,   493,   496,   512,   513,   514,
     519,   523,   528,   529,   531,   551,   595,   611,   623,   626,
     629,   632,   636,   637,   638,   639,   640,   641,   643,   654,
     657,   657,   688,   688,   713,   713,   729,   730,   731,   732,
     740,   749,   749,   798,   803,   814,   824,   841,   842,   843,
     844,   846,   847,   849,   849,   851,   851,   875,   883,   897,
     913,   914,   915,   916,   923,   924,   925,   926,   927,   928,
     929,   930,   931,   938,   939,   941,   942,   944,   945,   949,
     947,   955,   953,   962,   963,   964,   965,   966,   968,   969,
     970,   972,   978,   979,   980,   981,   982,   983,   986,  1059,
    1059,  1061,  1061,  1063,  1064,  1066,  1150,  1151,  1159,  1160,
    1163,  1164,  1165,  1166,  1167,  1168,  1169,  1177,  1188,  1189,
    1190,  1192,  1192,  1195,  1195,  1198,  1199,  1202,  1207,  1210,
    1213,  1216,  1219,  1223,  1226,  1230,  1234,  1239,  1245,  1248,
    1251,  1254,  1259,  1262,  1265,  1275,  1279,  1285,  1291,  1299,
    1304,  1310,  1318,  1326,  1327,  1328,  1331,  1330,  1347,  1348,
    1350,  1349,  1373,  1393,  1408,  1409,  1411,  1412,  1414,  1415,
    1416,  1425,  1426,  1430,  1431,  1433,  1434,  1435,  1437,  1441,
    1442,  1447,  1448,  1495,  1545,  1566,  1588,  1591,  1598,  1601,
    1604,  1607,  1610,  1613,  1618,  1619,  1621,  1622,  1623,  1624,
    1625,  1627,  1633,  1642,  1643,  1644,  1645,  1646,  1647,  1649,
    1653,  1658,  1676,  1687,  1697,  1706,  1707,  1708,  1709,  1710,
    1711,  1712,  1713,  1714,  1715,  1716,  1717,  1718,  1719,  1720,
    1721,  1722,  1723,  1724,  1725,  1726,  1727,  1728,  1729,  1730,
    1731,  1732,  1733,  1734,  1735,  1736,  1737,  1738,  1739,  1740,
    1741,  1742,  1743,  1744,  1745,  1746,  1747,  1748,  1749,  1750,
    1751,  1753,  1754,  1755,  1756,  1758,  1773,  1781,  1781,  1836,
    1838,  1839,  1880,  1883,  1886,  1893,  1896,  1907,  1915,  1919,
    1926,  1930,  1940,  1951,  2149,  2184,  2191,  2198,  2205,  2227,
    2234
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
  "\"...\"", "\"<<\"", "\">>>\"", "\">>\"", "prec_none", "prec_var_read",
  "below_semicolon", "';'", "','", "below_assignment", "'='", "'?'", "':'",
  "'|'", "'^'", "'&'", "below_lt", "'<'", "'>'", "below_minus", "'-'",
  "'+'", "'/'", "'*'", "'%'", "'~'", "'!'", "minusminus_prefix",
  "plusplus_prefix", "below_curly", "'('", "new2", "'['", "']'", "'{'",
  "'.'", "'@'", "below_identifier", "above_identifier", "below_else",
  "above_function", "'}'", "')'", "$accept", "PROGRAM",
  "MAYBE_PROGRAM_CODE_LIST", "PROGRAM_CODE_LIST", "PROGRAM_CODE",
  "MAYBE_INPACKAGE_CODE_LIST", "INPACKAGE_CODE_LIST", "INPACKAGE_CODE",
  "MAYBECODE", "CODE", "CODE_STATEMENT", "CODEPIECE", "CODEBLOCK",
  "PACKAGE_INITCODE", "EMBED_START", "MAYBEEXPRESSION",
  "VARIABLE_DECLARATION", "VARIABLE_LIST", "ONE_VARIABLE", "IF_CODEBLOCK",
  "$@1", "MAYBEELSE", "IF", "FOR_INIT", "FOR_IN_INIT", "FOR_START", "FOR",
  "FOR_IN", "WHILE", "DO_WHILE", "BREAK", "CONTINUE", "MAYBE_CASE_LIST",
  "CASE_LIST", "CASE", "DEFAULT", "SWITCH", "$@2", "CATCH", "$@3",
  "FINALLY", "$@4", "CATCH_LIST", "CATCH_FINALLY_LIST", "TRY", "$@5",
  "THROW", "WITH_HEAD", "WITH", "X_IDENTIFIER", "PACKAGE",
  "PACKAGE_DECLARATION", "$@6", "$@7", "IMPORT", "MAYBE_MODIFIERS",
  "MODIFIER_LIST", "MODIFIER", "EXTENDS", "EXTENDS_LIST",
  "IMPLEMENTS_LIST", "CLASS_DECLARATION", "$@8", "INTERFACE_DECLARATION",
  "$@9", "MAYBE_CLASS_BODY", "CLASS_BODY", "CLASS_BODY_ITEM",
  "MAYBE_INTERFACE_BODY", "INTERFACE_BODY", "IDECLARATION", "VARCONST",
  "SLOT_DECLARATION", "$@10", "SLOT_LIST", "ONE_SLOT", "MAYBECONSTANT",
  "CONSTANT", "XML", "OPEN", "CLOSE", "CLOSE2", "XMLEXPR1", "$@11",
  "XMLEXPR2", "$@12", "XMLTEXT", "XML2", "XML_ID_OR_EXPR",
  "MAYBE_XMLATTRIBUTES", "XMLNODE", "XMLATTRIBUTES", "XMLATTRIBUTE",
  "MAYBE_PARAM_LIST", "PARAM_LIST", "PARAM", "GETSET",
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
     355,    59,    44,   356,    61,    63,    58,   124,    94,    38,
     357,    60,    62,   358,    45,    43,    47,    42,    37,   126,
      33,   359,   360,   361,    40,   362,    91,    93,   123,    46,
      64,   363,   364,   365,   366,   125,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   137,   138,   139,   139,   140,   140,   141,   141,   141,
     141,   141,   141,   141,   142,   142,   143,   143,   144,   144,
     144,   144,   144,   144,   144,   145,   145,   146,   146,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   148,   148,   148,   148,   148,   148,
     148,   149,   149,   150,   151,   152,   152,   153,   153,   154,
     154,   155,   157,   156,   158,   158,   159,   160,   160,   160,
     161,   161,   162,   162,   163,   164,   165,   166,   167,   167,
     168,   168,   169,   169,   169,   169,   170,   170,   171,   172,
     174,   173,   176,   175,   178,   177,   179,   179,   180,   180,
     180,   182,   181,   183,   183,   184,   185,   186,   186,   186,
     186,   187,   187,   189,   188,   190,   188,   191,   191,   191,
     192,   192,   193,   193,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   195,   195,   196,   196,   197,   197,   199,
     198,   201,   200,   202,   202,   203,   203,   204,   204,   204,
     204,   204,   205,   205,   206,   206,   207,   207,   207,   208,
     208,   210,   209,   211,   211,   212,   213,   213,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   215,   216,   217,
     218,   220,   219,   222,   221,   223,   223,   223,   223,   224,
     224,   225,   225,   226,   226,   227,   227,   227,   228,   228,
     229,   229,   229,   229,   229,   230,   230,   230,   230,   231,
     231,   232,   232,   233,   233,   233,   235,   234,   236,   236,
     238,   237,   239,   240,   241,   241,   242,   242,   243,   243,
     243,   244,   244,   245,   245,   246,   246,   246,   247,   248,
     247,   249,   250,   251,   251,   252,   253,   253,   254,   255,
     256,   256,   257,   257,   258,   258,   259,   259,   259,   259,
     259,   260,   260,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   262,   261,   263,
     264,   264,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   265,   266,   267,   267,   267,   268,   269,
     270
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     2,     1,     1,
       1,     1,     1,     4,     1,     1,     0,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     0,     2,     0,     2,     2,     1,
       3,     3,     0,     2,     0,     2,     6,     0,     1,     1,
       3,     1,     2,     3,     8,     6,     5,     6,     1,     2,
       1,     2,     0,     1,     1,     2,     1,     2,     4,     3,
       0,     8,     0,     9,     0,     5,     1,     2,     1,     2,
       1,     0,     6,     2,     1,     4,     2,     1,     1,     1,
       1,     3,     1,     0,     6,     0,     5,     2,     2,     4,
       0,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     2,     0,     2,     0,     2,     0,
       9,     0,     8,     0,     1,     1,     2,     1,     1,     1,
       4,     1,     0,     1,     1,     2,     1,     2,     8,     1,
       1,     0,     4,     1,     3,     3,     0,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     0,     4,     0,     4,     0,     2,     2,     2,     2,
       3,     1,     1,     0,     1,     6,    10,    11,     1,     2,
       1,     3,     3,     3,     3,     0,     1,     2,     4,     3,
       1,     4,     2,     1,     1,     0,     0,    12,     1,     0,
       0,    10,     1,     3,     1,     1,     1,     3,     1,     1,
       1,     2,     0,     0,     3,     0,     1,     1,     1,     2,
       2,     0,     4,     4,     4,     2,     1,     2,     1,     1,
       1,     3,     1,     3,     0,     1,     1,     1,     1,     1,
       1,     3,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     1,     3,     2,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       5,     2,     2,     2,     2,     3,     2,     0,     6,     1,
       1,     1,     3,     5,     3,     5,     5,     4,     6,     4,
       6,     6,     6,     3,     1,     2,     4,     4,     2,     4,
       3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     120,   344,   171,   271,   168,   169,   170,     0,     0,    62,
       0,     0,   126,   272,   124,   125,     0,   132,     0,   131,
     219,   175,   176,   304,   127,     0,     0,     0,   174,   128,
     130,   129,     0,     0,   173,   172,     0,     0,     0,   254,
       0,     0,    13,   178,     0,     0,     0,     0,   235,   120,
       0,     0,     2,   120,     5,    53,    12,    36,    67,    31,
      32,    33,    34,    35,    38,   120,    37,     7,    30,     0,
     121,   122,     9,     8,    11,   269,   270,     0,   177,    10,
     263,   265,   267,   266,    39,   252,   264,   268,    41,    29,
      40,     0,    72,     0,     0,   120,    90,   107,   109,   108,
     110,   115,   112,     0,     0,   241,   218,     0,   303,     0,
     117,     0,   118,   302,   101,   235,     0,     0,   245,     0,
     256,   257,   258,   259,   260,     0,     0,   255,   324,   323,
     306,   286,   285,     0,   250,     0,   236,   237,   238,   248,
      80,     0,   104,   246,     0,    78,    44,    43,   120,    45,
      28,    46,    47,    48,    50,     0,    49,   326,     1,     6,
     344,     0,    68,     0,     0,    69,    52,   106,     0,   215,
       0,   160,     0,   159,   161,   348,   123,   191,     0,   192,
     193,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,   321,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   235,     0,     0,    73,     0,   249,
       0,    63,     0,   120,   113,     0,   222,     0,   225,   224,
     350,   233,   205,     0,     0,    26,     0,   325,     0,     0,
     274,     0,     0,     0,   305,   273,   239,   240,    81,   232,
      58,    59,   103,   247,    57,    79,    42,    27,   232,     0,
       0,    51,   345,   214,   213,     0,   133,   135,     0,   183,
       0,   200,     0,   194,   198,   253,   300,   301,   298,   299,
     279,   280,   282,   281,   277,   278,   314,   313,   309,   308,
     316,   317,   318,   315,   311,   310,   312,   283,   284,   332,
     331,     0,   330,     0,   334,   292,   291,   290,   319,     0,
     289,   288,   287,   275,   276,   296,   295,   293,   297,   294,
       0,     0,   327,     0,     0,     0,   343,    62,     0,     0,
      24,    54,     0,   120,    16,    22,    19,    18,    21,    20,
     120,   111,     0,   235,   242,   166,     0,     0,   206,   210,
     105,   119,   223,     0,    25,   244,   349,    62,   261,     0,
     251,     0,    56,     0,    56,     0,     0,     0,     0,     0,
     137,     0,     0,   232,   162,   163,     0,     0,     0,   179,
     180,   185,   199,     0,     0,   339,     0,     0,   243,   307,
       0,     0,     0,     0,   337,     0,    76,     0,     0,     0,
     116,    17,     0,     0,     0,     0,   212,   207,   232,     0,
       0,    64,     0,   230,   229,   228,   231,     0,    61,    60,
       0,    62,   346,   347,   205,   134,     0,     0,   226,   136,
     141,    56,     0,   184,   204,   203,   201,   202,     0,     0,
       0,     0,   331,   335,   320,     0,   336,     0,     0,   333,
      77,    82,     0,   114,   234,   167,   166,     0,     0,   209,
       0,     0,    96,   100,    98,   102,    62,    66,   262,    55,
       0,    75,     0,   138,   139,     0,   120,   165,   164,   195,
     187,   178,   188,     0,   186,     0,   185,   342,   340,   328,
     341,   338,     0,     0,     0,    83,    86,    84,    23,   211,
     220,   208,    94,     0,    97,    99,    65,    62,   232,   120,
     227,     0,   156,     0,     0,   120,   154,     0,   181,   178,
     185,   189,     0,    26,    91,    87,    85,    26,    26,   232,
      74,     0,   147,    54,   151,     0,     0,   120,   145,   148,
     149,   157,   215,   142,   155,   180,     0,     0,   190,    26,
      89,     0,     0,     0,   216,     0,   140,   146,     0,     0,
     182,   180,    88,   221,    95,    92,    26,     0,     0,   196,
       0,     0,     0,   150,   205,   197,    26,   217,     0,     0,
     232,    93,   158
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    51,    52,    53,    54,   342,   343,   344,   363,   364,
     149,   150,   167,   345,   409,   428,   151,   264,   261,    94,
      95,   477,    57,   163,   164,    58,    59,    60,    61,    62,
     152,   153,   504,   505,   506,   507,    63,   232,   472,   581,
     473,   538,   474,   475,    64,   245,   154,    65,    66,   312,
     237,    67,   350,   233,    68,   155,    70,    71,   380,   382,
     437,   346,   519,   347,   486,   546,   547,   548,   524,   525,
     526,   174,   348,   278,   384,   385,   416,    75,    76,    77,
     391,   448,   494,   556,   179,   386,   449,   495,   180,   282,
      78,   283,   284,   357,   358,   359,   275,   349,   576,   107,
      80,   537,   238,   239,   425,   439,   426,   372,   354,   135,
     136,   137,   241,    81,    82,    83,   156,   138,   228,   229,
      84,   125,   126,   127,    85,   400,   313,   314,    86,    87,
     175,    88,    89,    90
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -484
static const yytype_int16 yypact[] =
{
    1323,  -484,  -484,  -484,  -484,  -484,  -484,   -13,   -71,  -484,
     -53,    62,  -484,  -484,  -484,  -484,    73,  -484,  1851,  -484,
     110,  -484,  -484,   407,  -484,     9,   229,  1851,  -484,  -484,
    -484,  -484,   -22,    16,  -484,  -484,   115,  1851,    33,   390,
    1851,  1851,  -484,  -484,  1851,  1851,  1851,  1851,  1851,   549,
     159,   216,  -484,  1452,  -484,  -484,  -484,  -484,  1798,  -484,
    -484,  -484,  -484,  -484,  -484,  1708,  -484,  -484,  -484,   375,
     419,  -484,  -484,  -484,  -484,  -484,  -484,    10,  -484,  -484,
    -484,  -484,  -484,  -484,   117,  2620,  -484,  -484,  -484,  -484,
    -484,    93,  -484,  1851,   209,  1708,  -484,  -484,  -484,  -484,
    -484,  -484,  -484,   -65,   277,   147,  -484,   100,   -24,  1851,
     113,   119,  -484,   -24,  -484,  1851,   225,   135,   -24,  1851,
    -484,  -484,  -484,  -484,  -484,   111,   148,   151,   -24,   -24,
     208,   -24,   -24,   -50,  2557,   134,   160,  1851,  -484,  2557,
     260,   261,  1851,  1851,   261,   263,  -484,  -484,   678,  -484,
    -484,  -484,  -484,  -484,  -484,   256,  -484,  -484,  -484,  -484,
     207,   275,  -484,   180,   219,   117,   188,  -484,   291,    39,
     292,  -484,   293,  -484,  -484,  -484,  -484,  -484,  1851,  -484,
      13,  1851,  1851,  1851,  1851,  1851,  1851,  1851,  1851,  1851,
    1851,  1851,  1851,  1851,  1851,  1851,  1851,  1851,  1851,  1851,
    1851,  1851,  1851,  1851,  1851,  1851,  -484,  -484,    22,  1851,
    1851,  1851,  1851,  1851,  1851,  1851,  1851,  1851,  1851,  1851,
    1851,  1851,  1851,  1851,  1851,  1851,    18,  -484,   166,   202,
     182,  -484,  1851,   807,  -484,   277,   178,   181,  -484,  -484,
    -484,   192,    15,   183,    44,  1580,   185,  -484,  1851,   186,
    -484,  1851,   390,  1851,  -484,  -484,  -484,  -484,  -484,   211,
     226,  -484,  -484,  -484,   226,  -484,  -484,  -484,   211,  1851,
    1851,  -484,   227,  -484,  -484,   326,   280,   285,   338,  2557,
     245,   246,    37,    13,  -484,  2557,   928,   799,   799,   799,
    2998,  2998,  2998,  2998,   928,   928,  2557,  2557,  2557,  2557,
    2557,  2557,  2557,  2557,  2557,  2557,  2557,  2683,  2746,  -484,
     258,   210,  -484,   262,  -484,   354,   354,   354,  2557,  2053,
    2809,  2872,  2935,   928,   928,   208,   208,   -24,   -24,   -24,
     218,  2116,  -484,  1851,   240,   265,  -484,  -484,  1851,  1911,
    -484,   232,   221,   936,  -484,  -484,  -484,  -484,  -484,  -484,
     807,  -484,   277,  1851,  -484,    50,   359,   228,   270,  -484,
    -484,  -484,   234,   230,  1580,  -484,  2557,  -484,  -484,   267,
    2557,   101,   272,   261,   -17,   273,   247,   143,   257,   277,
     366,   277,   264,   211,   284,  -484,   254,    27,    31,  -484,
    -484,  -484,  -484,  1851,   305,  -484,   244,  1851,  -484,  -484,
    1851,  2179,  1851,   312,  -484,   244,  -484,   271,   289,  1851,
    -484,  -484,   268,   283,  1851,   101,  -484,  -484,   211,    21,
     102,   343,  1851,  -484,  -484,  -484,  -484,  1851,  -484,  -484,
    1851,  -484,  -484,  -484,    15,  -484,   277,   294,  -484,   311,
    -484,   272,   338,  -484,  -484,  -484,  -484,  -484,   314,    99,
    2242,   244,  -484,  -484,  2557,  1982,  -484,  2305,   244,  -484,
    -484,    51,  2368,  -484,  -484,  2557,   319,   299,   359,  -484,
     300,   307,  -484,  -484,   102,  -484,  -484,  -484,  -484,  2557,
     -41,  -484,   297,   311,  -484,   277,   266,  -484,  -484,  -484,
    -484,   313,  -484,  1851,  -484,   329,  -484,  -484,  -484,  -484,
    -484,  -484,  1851,   331,   315,    51,  -484,  -484,  -484,  -484,
    -484,  -484,  -484,   445,  -484,  -484,  -484,  -484,   211,  1065,
    -484,   446,  -484,   427,   318,   323,  -484,    10,  2557,   339,
    -484,   103,  2431,  1580,  -484,  -484,  -484,  1580,  1580,   211,
    -484,   332,  -484,   232,  -484,   106,   322,  1194,  -484,  -484,
    -484,  -484,    39,  -484,  -484,  -484,   328,    10,   103,  1580,
    -484,   341,   344,   337,  -484,  1851,  -484,  -484,   451,   352,
    -484,  -484,  -484,  -484,  -484,  -484,  1580,  2494,   350,  -484,
     369,   357,   347,  -484,    15,  -484,  1580,  -484,   353,   351,
     211,  -484,  -484
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -484,  -484,  -484,  -484,   435,   140,  -484,   149,  -268,   444,
       3,   -55,   399,    41,   -48,    55,   439,   360,   127,  -288,
    -484,  -484,  -484,  -484,  -484,  -484,  -484,  -484,  -484,  -484,
    -484,  -484,  -484,  -484,    -3,    -2,  -484,  -484,    30,  -484,
      32,  -484,  -484,  -484,  -484,  -484,  -484,  -484,  -484,     6,
      59,  -484,  -484,  -484,  -484,     1,  -484,   437,  -484,  -484,
    -484,    43,  -484,    45,  -484,  -484,  -484,   -42,  -484,  -484,
     -16,  -484,     2,  -484,  -484,    66,    46,  -484,  -484,  -484,
    -484,  -479,  -484,  -484,  -165,  -484,  -416,  -484,  -483,  -484,
    -387,  -484,   231,  -395,  -484,  -336,   -39,     4,  -484,  -484,
    -484,  -484,  -484,   484,   -97,    75,   104,  -260,  -484,  -104,
    -484,  -484,  -484,  -484,  -484,  -484,  -484,  -125,   -61,   468,
     -52,  -484,   276,  -484,   -18,  -484,  -212,  -184,  -484,  -484,
    -484,  -484,  -484,  -484
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -330
static const yytype_int16 yytable[] =
{
     105,    69,    74,    55,    79,   108,   165,   240,   374,   113,
     166,   246,   257,   177,   335,   281,   280,   102,   355,   118,
     417,    97,   128,   129,   355,    97,   130,   131,   132,   134,
     139,   444,   102,    98,    99,   446,    91,    98,    99,   482,
     166,    56,   336,    72,   555,    73,   100,    97,   243,   406,
     100,   -70,   253,    93,    69,    74,    55,    79,   249,    98,
      99,   181,   496,   234,   235,    97,   205,   206,   207,   208,
     103,    96,   100,   273,   571,   134,   569,    98,    99,   421,
     531,   262,   263,   469,   502,   111,   254,   427,   104,   274,
     100,   134,   580,   267,    56,   517,    72,   139,    73,   394,
     224,   134,   225,   490,    97,   226,   114,   490,   530,   356,
     236,    92,   503,   106,   558,   468,    98,    99,   281,   139,
     330,   168,   403,   441,   134,   134,   368,   395,   470,   100,
     117,   169,   511,   109,   471,   310,   423,   171,   178,   310,
     115,   178,   332,   481,   333,   116,   432,   433,   334,   389,
     404,   173,   311,   390,   414,   178,   415,   119,   467,   178,
     279,   361,   157,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   516,   588,
     101,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   139,   331,   375,   376,
     491,   492,   453,    97,   339,   492,   158,   227,   424,   181,
     230,   459,   445,   447,   242,    98,    99,   493,   247,   540,
     366,   493,   110,   139,    69,   370,    55,   205,   100,   248,
     208,   351,  -107,    97,    98,    99,   250,    97,   244,   413,
     362,   134,   134,   252,   251,    98,    99,   100,   541,    98,
      99,   255,   256,   258,   259,   560,   265,   498,   100,   561,
     562,   168,   100,   225,   501,   -71,   226,   407,   268,   563,
      97,   269,   435,    12,   438,    14,    15,   270,    17,   271,
      19,   572,    98,    99,   272,   276,   277,   478,   205,   206,
     207,   208,   337,    24,   253,   100,   338,  -112,   582,   267,
     352,   521,    29,    30,    31,   401,   353,   371,   589,   360,
     134,   365,   367,   139,   221,   222,   223,   310,   373,   378,
     592,   377,   224,   379,   225,   139,   393,   226,   381,   438,
      12,   383,    14,    15,    69,    17,    55,    19,  -329,   387,
     388,    69,   396,    55,   398,   405,   410,   310,   362,  -235,
      24,   452,   355,  -111,   418,   420,   402,   522,   521,    29,
      30,    31,   419,   422,   430,   450,   427,   236,   480,   454,
     436,   434,   455,   431,   457,   236,   442,   236,   520,   443,
     168,   462,   440,   120,   121,   451,   465,   122,   123,   124,
     169,  -152,   458,   463,   139,   170,   171,   460,   476,   479,
       1,     2,     3,   485,     4,     5,     6,   461,   172,   464,
     173,   236,   484,   414,   522,    13,   489,   510,   512,   527,
      18,   513,    20,   518,    21,    22,    12,   533,    14,    15,
     529,    17,   236,    19,   205,   206,   207,   208,   539,   551,
     534,    28,   552,   553,   578,   557,    24,   566,  -153,    33,
     564,    34,    35,   570,   579,    29,    30,    31,   219,   220,
     221,   222,   223,   575,   584,   528,   573,    39,   224,   574,
     225,   585,   587,   226,   532,   586,   591,   523,   159,   590,
     412,   236,   411,   148,   231,   565,   487,   162,    40,    41,
     429,   260,   535,   536,   514,   567,   515,   176,   488,   554,
     112,   483,   509,   568,   392,   133,     0,     0,     0,   466,
     545,   549,   544,   550,     0,   139,   523,     0,   369,     0,
       0,    47,     0,    48,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   577,   545,   549,
     544,   550,     1,     2,     3,     0,     4,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    12,    13,    14,    15,
      16,    17,    18,    19,    20,     0,    21,    22,   140,     0,
     141,     0,     0,     0,    23,   142,    24,    25,     0,    26,
     143,    27,     0,    28,   144,    29,    30,    31,     0,     0,
      32,    33,     0,    34,    35,     0,     0,     0,     0,     0,
       0,    36,    37,    38,     0,   145,     0,     0,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      40,    41,     0,     0,     0,     0,     0,     0,     0,     0,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,     0,     0,    44,     0,     0,     0,     0,    45,    46,
       0,     0,     0,    47,     0,    48,     0,    49,     0,    50,
       0,     1,     2,     3,   147,     4,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     0,    21,    22,   140,     0,   141,
       0,     0,     0,    23,   142,    24,    25,     0,    26,   143,
      27,     0,    28,   144,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,    38,     0,   145,     0,     0,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      41,     0,     0,     0,     0,     0,     0,     0,     0,   146,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,    44,     0,     0,     0,     0,    45,    46,     0,
       0,     0,    47,     0,    48,     0,    49,     0,    50,     0,
       1,     2,     3,   266,     4,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    12,    13,    14,    15,    16,    17,
      18,    19,     0,     0,    21,    22,     0,     0,   182,     0,
       0,     0,    23,     0,    24,    25,     0,    26,     0,    27,
       0,    28,     0,    29,    30,    31,     0,     0,    32,    33,
       0,    34,    35,     0,     0,     0,  -330,  -330,  -330,    36,
      37,    38,     0,     0,   190,   191,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   205,
     206,   207,   208,     0,   209,   210,   211,     0,    40,    41,
       0,     0,     0,     0,     0,     0,     0,     0,   340,     0,
     217,   218,     0,   219,   220,   221,   222,   223,    43,     0,
       0,    44,     0,   224,     0,   225,    45,    46,   226,     0,
       0,    47,     0,   341,     0,    49,     0,    50,     0,     1,
       2,     3,   -14,     4,     5,     6,     7,     8,     9,    10,
       0,     0,     0,    12,    13,    14,    15,    16,    17,    18,
      19,     0,     0,    21,    22,     0,     0,  -330,     0,     0,
       0,    23,     0,    24,    25,     0,    26,     0,    27,     0,
      28,     0,    29,    30,    31,     0,     0,    32,    33,     0,
      34,    35,     0,     0,     0,     0,     0,     0,    36,    37,
      38,     0,     0,  -330,  -330,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   205,   206,
     207,   208,     0,   209,   210,   211,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,     0,   340,     0,  -330,
    -330,     0,   219,   220,   221,   222,   223,    43,     0,     0,
      44,     0,   224,     0,   225,    45,    46,   226,     0,     0,
      47,     0,   341,     0,    49,     0,    50,     0,     1,     2,
       3,   -15,     4,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    12,    13,    14,    15,    16,    17,    18,    19,
       0,     0,    21,    22,     0,     0,     0,     0,     0,     0,
      23,     0,    24,    25,     0,    26,     0,    27,     0,    28,
       0,    29,    30,    31,     0,     0,    32,    33,     0,    34,
      35,     0,     0,     0,     0,     0,     0,    36,    37,    38,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    40,    41,     0,     0,
       0,     0,     0,     0,     0,     0,   542,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,    44,
       0,     0,     0,     0,    45,    46,     0,     0,     0,    47,
       0,   543,     0,    49,     0,    50,     0,     1,     2,     3,
    -143,     4,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    12,    13,    14,    15,    16,    17,    18,    19,     0,
       0,    21,    22,     0,     0,     0,     0,     0,     0,    23,
       0,    24,    25,     0,    26,     0,    27,     0,    28,     0,
      29,    30,    31,     0,     0,    32,    33,     0,    34,    35,
       0,     0,     0,     0,     0,     0,    36,    37,    38,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    40,    41,     0,     0,     0,
       0,     0,     0,     0,     0,   542,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,    44,     0,
       0,     0,     0,    45,    46,     0,     0,     0,    47,     0,
     543,     0,    49,    -3,    50,     0,     1,     2,     3,  -144,
       4,     5,     6,     7,     8,     9,    10,     0,     0,    11,
      12,    13,    14,    15,    16,    17,    18,    19,     0,     0,
      21,    22,     0,     0,     0,     0,     0,     0,    23,     0,
      24,    25,     0,    26,     0,    27,     0,    28,     0,    29,
      30,    31,     0,     0,    32,    33,     0,    34,    35,     0,
       0,     0,     0,     0,     0,    36,    37,    38,     0,     0,
       0,     0,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    40,    41,     0,     0,     0,     0,
       0,     0,     0,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,    44,     0,     0,
       0,     0,    45,    46,     0,     0,     0,    47,     0,    48,
       0,    49,    -4,    50,     0,     1,     2,     3,     0,     4,
       5,     6,     7,     8,     9,    10,     0,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    21,
      22,     0,     0,     0,     0,     0,     0,    23,     0,    24,
      25,     0,    26,     0,    27,     0,    28,     0,    29,    30,
      31,     0,     0,    32,    33,     0,    34,    35,     0,     0,
       0,     0,     0,     0,    36,    37,    38,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,    41,     0,     0,     0,     0,     0,
       0,     0,     0,    42,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,    44,     0,     0,     0,
       0,    45,    46,     0,     0,     0,    47,     0,    48,     0,
      49,     0,    50,     1,     2,     3,     0,     4,     5,     6,
       7,     8,     9,    10,     0,  -120,     0,    12,    13,    14,
      15,    16,    17,    18,    19,    20,     0,    21,    22,   140,
       0,   141,     0,     0,     0,    23,   142,    24,    25,     0,
      26,   143,    27,     0,    28,   144,    29,    30,    31,     0,
       0,    32,    33,     0,    34,    35,     0,     0,     0,     0,
       0,     0,    36,    37,    38,     0,   145,     0,     0,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    40,    41,     0,     0,     0,     0,     0,     0,     0,
       0,   146,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,     0,     0,    44,     0,     0,     0,     0,    45,
      46,     0,     0,     0,    47,     0,    48,     0,    49,     0,
      50,     1,     2,     3,     0,     4,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     0,    21,    22,   140,     0,   141,
       0,     0,     0,    23,   142,    24,    25,     0,    26,   143,
      27,     0,    28,   144,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,    38,     0,   145,     0,     0,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      41,   160,     2,     3,     0,     4,     5,     6,     0,   146,
       0,     0,     0,     0,     0,     0,    13,     0,     0,    43,
       0,    18,    44,    20,     0,    21,    22,    45,    46,   141,
       0,     0,    47,    23,    48,     0,    49,     0,    50,     0,
      27,     0,    28,   161,     0,     0,     0,     0,     0,     0,
      33,     0,    34,    35,     1,     2,     3,     0,     4,     5,
       6,    37,     0,     0,     0,     0,     0,     0,    39,    13,
       0,     0,     0,     0,    18,     0,    20,     0,    21,    22,
       0,     0,     0,     0,     0,     0,    23,     0,     0,    40,
      41,     0,     0,    27,     0,    28,     0,     0,     0,     0,
       0,     0,     0,    33,     0,    34,    35,     0,     0,    43,
       0,     0,    44,     0,    37,     0,     0,    45,    46,     0,
       0,    39,    47,     0,    48,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,    41,     0,     0,     0,     0,     0,     0,
     182,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,     0,     0,    44,     0,     0,     0,     0,
      45,    46,     0,     0,     0,    47,     0,    48,   183,   184,
     185,    50,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,     0,   209,   210,   211,     0,
       0,     0,     0,     0,     0,   212,   213,     0,   214,   215,
     216,   182,   217,   218,     0,   219,   220,   221,   222,   223,
       0,     0,     0,     0,     0,   224,     0,   225,     0,     0,
     226,     0,     0,     0,     0,     0,     0,   408,     0,   183,
     184,   185,     0,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,     0,   209,   210,   211,
       0,     0,     0,     0,     0,     0,   212,   213,     0,   214,
     215,   216,   182,   217,   218,     0,   219,   220,   221,   222,
     223,     0,     0,     0,     0,     0,   224,     0,   225,     0,
       0,   226,     0,     0,     0,     0,     0,     0,   499,     0,
     183,   184,   185,     0,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,     0,   209,   210,
     211,     0,     0,     0,     0,   182,     0,   212,   213,   397,
     214,   215,   216,     0,   217,   218,     0,   219,   220,   221,
     222,   223,     0,     0,     0,     0,     0,   224,     0,   225,
       0,     0,   226,   183,   184,   185,     0,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
       0,   209,   210,   211,     0,     0,     0,     0,   182,     0,
     212,   213,     0,   214,   215,   216,     0,   217,   218,     0,
     219,   220,   221,   222,   223,     0,     0,     0,     0,     0,
     224,     0,   225,   399,     0,   226,   183,   184,   185,     0,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,     0,   209,   210,   211,     0,     0,     0,
       0,   182,     0,   212,   213,     0,   214,   215,   216,     0,
     217,   218,     0,   219,   220,   221,   222,   223,     0,     0,
       0,     0,     0,   224,     0,   225,   456,     0,   226,   183,
     184,   185,     0,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,     0,   209,   210,   211,
       0,     0,     0,     0,   182,     0,   212,   213,     0,   214,
     215,   216,     0,   217,   218,     0,   219,   220,   221,   222,
     223,     0,     0,     0,     0,     0,   224,     0,   225,   497,
       0,   226,   183,   184,   185,     0,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,     0,
     209,   210,   211,     0,     0,     0,     0,   182,     0,   212,
     213,     0,   214,   215,   216,     0,   217,   218,     0,   219,
     220,   221,   222,   223,     0,     0,     0,     0,     0,   224,
       0,   225,   500,     0,   226,   183,   184,   185,     0,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,     0,   209,   210,   211,     0,     0,     0,     0,
     182,     0,   212,   213,     0,   214,   215,   216,     0,   217,
     218,     0,   219,   220,   221,   222,   223,     0,     0,     0,
       0,     0,   224,     0,   225,   508,     0,   226,   183,   184,
     185,     0,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,     0,   209,   210,   211,     0,
       0,     0,     0,   182,     0,   212,   213,   559,   214,   215,
     216,     0,   217,   218,     0,   219,   220,   221,   222,   223,
       0,     0,     0,     0,     0,   224,     0,   225,     0,     0,
     226,   183,   184,   185,     0,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,     0,   209,
     210,   211,     0,     0,     0,     0,   182,     0,   212,   213,
       0,   214,   215,   216,     0,   217,   218,     0,   219,   220,
     221,   222,   223,     0,     0,     0,     0,     0,   224,     0,
     225,   583,     0,   226,   183,   184,   185,     0,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,     0,   209,   210,   211,     0,     0,     0,     0,   182,
       0,   212,   213,     0,   214,   215,   216,     0,   217,   218,
       0,   219,   220,   221,   222,   223,     0,     0,     0,     0,
       0,   224,     0,   225,     0,     0,   226,   183,   184,   185,
       0,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,     0,   209,   210,   211,     0,     0,
       0,     0,   182,     0,   212,   213,     0,   214,   215,   216,
       0,     0,   218,     0,   219,   220,   221,   222,   223,     0,
       0,     0,     0,     0,   224,     0,   225,     0,     0,   226,
     183,   184,   185,     0,   186,   187,   188,   189,   190,   191,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   204,   205,   206,   207,   208,     0,   209,   210,
     211,     0,     0,     0,     0,   182,     0,     0,     0,     0,
     214,   215,   216,     0,   217,   218,     0,   219,   220,   221,
     222,   223,     0,     0,     0,     0,     0,   224,     0,   225,
       0,     0,   226,   183,   184,   185,     0,   186,   187,   188,
     189,   190,   191,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   205,   206,   207,   208,
       0,   209,   210,   211,     0,     0,     0,     0,   182,     0,
       0,     0,     0,   214,   215,   216,     0,   217,   218,     0,
     219,   220,   221,   222,   223,     0,     0,     0,     0,     0,
     224,     0,   225,     0,     0,   226,   183,   184,   185,     0,
     186,   187,   188,   189,   190,   191,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   205,
     206,   207,   208,     0,   209,   210,   211,     0,     0,     0,
       0,   182,     0,     0,     0,     0,     0,   215,   216,     0,
     217,   218,     0,   219,   220,   221,   222,   223,     0,     0,
       0,     0,     0,   224,     0,   225,     0,     0,   226,   183,
     184,   185,     0,   186,   187,   188,   189,   190,   191,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,     0,   209,   210,   211,
       0,     0,     0,     0,   182,     0,     0,     0,     0,     0,
       0,   216,     0,   217,   218,     0,   219,   220,   221,   222,
     223,     0,     0,     0,     0,     0,   224,     0,   225,     0,
       0,   226,   183,   184,   185,     0,   186,   187,   188,   189,
     190,   191,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   205,   206,   207,   208,     0,
     209,   210,   211,     0,     0,     0,     0,   182,     0,     0,
       0,     0,     0,     0,  -330,     0,   217,   218,     0,   219,
     220,   221,   222,   223,     0,     0,     0,     0,     0,   224,
       0,   225,     0,     0,   226,   183,   184,   185,     0,  -330,
    -330,  -330,  -330,   190,   191,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   205,   206,
     207,   208,     0,   209,   210,   211,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   217,
     218,     0,   219,   220,   221,   222,   223,     0,     0,     0,
       0,     0,   224,     0,   225,     0,     0,   226
};

static const yytype_int16 yycheck[] =
{
      18,     0,     0,     0,     0,    23,    58,   104,   268,    27,
      65,   115,   137,     3,   226,   180,     3,    11,     3,    37,
     356,     3,    40,    41,     3,     3,    44,    45,    46,    47,
      48,     4,    26,    15,    16,     4,    49,    15,    16,   434,
      95,     0,   226,     0,   527,     0,    28,     3,   109,   337,
      28,    68,   102,   124,    53,    53,    53,    53,   119,    15,
      16,   102,   449,   128,   129,     3,    90,    91,    92,    93,
      11,   124,    28,    34,   557,    93,   555,    15,    16,   367,
     496,   142,   143,   419,    33,    26,   136,   104,    15,    50,
      28,   109,   571,   148,    53,   136,    53,   115,    53,   311,
     124,   119,   126,     4,     3,   129,   128,     4,   495,    94,
     104,   124,    61,     3,   530,    94,    15,    16,   283,   137,
     224,    15,   334,   383,   142,   143,   251,   311,    26,    28,
      15,    25,   468,   124,    32,   117,    35,    31,   128,   117,
     124,   128,   124,   431,   126,   129,     3,     4,   130,   112,
     334,    45,   130,   116,   104,   128,   106,   124,   418,   128,
     178,   117,     3,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   476,   584,
     128,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   269,   270,
     111,   112,   396,     3,   232,   112,     0,   124,   117,   102,
      11,   405,   387,   388,   124,    15,    16,   128,     3,   517,
     248,   128,     3,   251,   233,   253,   233,    90,    28,   104,
      93,   235,   129,     3,    15,    16,   135,     3,   129,   353,
     244,   269,   270,   102,   106,    15,    16,    28,   518,    15,
      16,   127,   102,     3,     3,   533,     3,   451,    28,   537,
     538,    15,    28,   126,   458,    68,   129,   338,     3,   539,
       3,   101,   379,    17,   381,    19,    20,    68,    22,   101,
      24,   559,    15,    16,     3,     3,     3,   422,    90,    91,
      92,    93,   136,    37,   102,    28,   124,   129,   576,   364,
     129,    45,    46,    47,    48,   333,   124,   106,   586,   136,
     338,   136,   136,   341,   116,   117,   118,   117,   102,     3,
     590,   104,   124,    53,   126,   353,   126,   129,    53,   436,
      17,     3,    19,    20,   343,    22,   343,    24,    90,   104,
     104,   350,    90,   350,   136,    90,   135,   117,   352,   127,
      37,   117,     3,   129,   136,   135,   126,   101,    45,    46,
      47,    48,   102,   106,   101,   393,   104,   371,   430,   397,
      14,   124,   400,   136,   402,   379,   102,   381,   485,   135,
      15,   409,   128,     3,     4,    90,   414,     7,     8,     9,
      25,   135,    90,   135,   422,    30,    31,   136,    65,   427,
       3,     4,     5,   102,     7,     8,     9,   128,    43,   136,
      45,   415,   128,   104,   101,    18,   112,   128,   128,   116,
      23,   124,    25,   136,    27,    28,    17,   106,    19,    20,
     111,    22,   436,    24,    90,    91,    92,    93,     3,     3,
     135,    44,    25,   135,     3,   116,    37,   135,   135,    52,
     128,    54,    55,   135,   112,    46,    47,    48,   114,   115,
     116,   117,   118,   136,   124,   493,   135,    70,   124,   135,
     126,   112,   135,   129,   502,   128,   135,   486,    53,   136,
     350,   485,   343,    49,    95,   543,   441,    58,    91,    92,
     373,   141,   505,   505,   474,   547,   474,    70,   442,   525,
      26,   436,   466,   552,   283,    47,    -1,    -1,    -1,   415,
     519,   519,   519,   519,    -1,   543,   525,    -1,   252,    -1,
      -1,   124,    -1,   126,    -1,    -1,    -1,   130,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   565,   547,   547,
     547,   547,     3,     4,     5,    -1,     7,     8,     9,    10,
      11,    12,    13,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    29,    -1,
      31,    -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,
      41,    42,    -1,    44,    45,    46,    47,    48,    -1,    -1,
      51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,   119,   120,
      -1,    -1,    -1,   124,    -1,   126,    -1,   128,    -1,   130,
      -1,     3,     4,     5,   135,     7,     8,     9,    10,    11,
      12,    13,    -1,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,    41,
      42,    -1,    44,    45,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,   119,   120,    -1,
      -1,    -1,   124,    -1,   126,    -1,   128,    -1,   130,    -1,
       3,     4,     5,   135,     7,     8,     9,    10,    11,    12,
      13,    -1,    -1,    -1,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    27,    28,    -1,    -1,    39,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    40,    -1,    42,
      -1,    44,    -1,    46,    47,    48,    -1,    -1,    51,    52,
      -1,    54,    55,    -1,    -1,    -1,    67,    68,    69,    62,
      63,    64,    -1,    -1,    75,    76,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    -1,    95,    96,    97,    -1,    91,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     111,   112,    -1,   114,   115,   116,   117,   118,   111,    -1,
      -1,   114,    -1,   124,    -1,   126,   119,   120,   129,    -1,
      -1,   124,    -1,   126,    -1,   128,    -1,   130,    -1,     3,
       4,     5,   135,     7,     8,     9,    10,    11,    12,    13,
      -1,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    27,    28,    -1,    -1,    39,    -1,    -1,
      -1,    35,    -1,    37,    38,    -1,    40,    -1,    42,    -1,
      44,    -1,    46,    47,    48,    -1,    -1,    51,    52,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    -1,    -1,    75,    76,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    91,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,   111,
     112,    -1,   114,   115,   116,   117,   118,   111,    -1,    -1,
     114,    -1,   124,    -1,   126,   119,   120,   129,    -1,    -1,
     124,    -1,   126,    -1,   128,    -1,   130,    -1,     3,     4,
       5,   135,     7,     8,     9,    10,    11,    12,    13,    -1,
      -1,    -1,    17,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    -1,    37,    38,    -1,    40,    -1,    42,    -1,    44,
      -1,    46,    47,    48,    -1,    -1,    51,    52,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      -1,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,   119,   120,    -1,    -1,    -1,   124,
      -1,   126,    -1,   128,    -1,   130,    -1,     3,     4,     5,
     135,     7,     8,     9,    10,    11,    12,    13,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      -1,    37,    38,    -1,    40,    -1,    42,    -1,    44,    -1,
      46,    47,    48,    -1,    -1,    51,    52,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,    -1,   119,   120,    -1,    -1,    -1,   124,    -1,
     126,    -1,   128,     0,   130,    -1,     3,     4,     5,   135,
       7,     8,     9,    10,    11,    12,    13,    -1,    -1,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,
      37,    38,    -1,    40,    -1,    42,    -1,    44,    -1,    46,
      47,    48,    -1,    -1,    51,    52,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,   119,   120,    -1,    -1,    -1,   124,    -1,   126,
      -1,   128,     0,   130,    -1,     3,     4,     5,    -1,     7,
       8,     9,    10,    11,    12,    13,    -1,    -1,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,
      38,    -1,    40,    -1,    42,    -1,    44,    -1,    46,    47,
      48,    -1,    -1,    51,    52,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    -1,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
      -1,   119,   120,    -1,    -1,    -1,   124,    -1,   126,    -1,
     128,    -1,   130,     3,     4,     5,    -1,     7,     8,     9,
      10,    11,    12,    13,    -1,    15,    -1,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    27,    28,    29,
      -1,    31,    -1,    -1,    -1,    35,    36,    37,    38,    -1,
      40,    41,    42,    -1,    44,    45,    46,    47,    48,    -1,
      -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    -1,    66,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,   119,
     120,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,    -1,
     130,     3,     4,     5,    -1,     7,     8,     9,    10,    11,
      12,    13,    -1,    -1,    -1,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    27,    28,    29,    -1,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,    41,
      42,    -1,    44,    45,    46,    47,    48,    -1,    -1,    51,
      52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    -1,    66,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,
      92,     3,     4,     5,    -1,     7,     8,     9,    -1,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,   111,
      -1,    23,   114,    25,    -1,    27,    28,   119,   120,    31,
      -1,    -1,   124,    35,   126,    -1,   128,    -1,   130,    -1,
      42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      52,    -1,    54,    55,     3,     4,     5,    -1,     7,     8,
       9,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    18,
      -1,    -1,    -1,    -1,    23,    -1,    25,    -1,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,    91,
      92,    -1,    -1,    42,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    54,    55,    -1,    -1,   111,
      -1,    -1,   114,    -1,    63,    -1,    -1,   119,   120,    -1,
      -1,    70,   124,    -1,   126,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
     119,   120,    -1,    -1,    -1,   124,    -1,   126,    67,    68,
      69,   130,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    -1,    95,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,   104,   105,    -1,   107,   108,
     109,    39,   111,   112,    -1,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    -1,    -1,    -1,    -1,    -1,   104,   105,    -1,   107,
     108,   109,    39,   111,   112,    -1,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,   136,    -1,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    -1,    95,    96,
      97,    -1,    -1,    -1,    -1,    39,    -1,   104,   105,   106,
     107,   108,   109,    -1,   111,   112,    -1,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,    -1,   129,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    -1,    -1,    -1,    39,    -1,
     104,   105,    -1,   107,   108,   109,    -1,   111,   112,    -1,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,   127,    -1,   129,    67,    68,    69,    -1,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    -1,    95,    96,    97,    -1,    -1,    -1,
      -1,    39,    -1,   104,   105,    -1,   107,   108,   109,    -1,
     111,   112,    -1,   114,   115,   116,   117,   118,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,   127,    -1,   129,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    -1,    -1,    -1,    39,    -1,   104,   105,    -1,   107,
     108,   109,    -1,   111,   112,    -1,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,
      -1,   129,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    -1,
      95,    96,    97,    -1,    -1,    -1,    -1,    39,    -1,   104,
     105,    -1,   107,   108,   109,    -1,   111,   112,    -1,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,   127,    -1,   129,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    -1,    -1,    -1,
      39,    -1,   104,   105,    -1,   107,   108,   109,    -1,   111,
     112,    -1,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,   127,    -1,   129,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    -1,    95,    96,    97,    -1,
      -1,    -1,    -1,    39,    -1,   104,   105,   106,   107,   108,
     109,    -1,   111,   112,    -1,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,    -1,
     129,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    -1,    -1,    -1,    39,    -1,   104,   105,
      -1,   107,   108,   109,    -1,   111,   112,    -1,   114,   115,
     116,   117,   118,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,   127,    -1,   129,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    -1,    95,    96,    97,    -1,    -1,    -1,    -1,    39,
      -1,   104,   105,    -1,   107,   108,   109,    -1,   111,   112,
      -1,   114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,    -1,    -1,   129,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    -1,    95,    96,    97,    -1,    -1,
      -1,    -1,    39,    -1,   104,   105,    -1,   107,   108,   109,
      -1,    -1,   112,    -1,   114,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,    -1,   129,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    -1,    95,    96,
      97,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
     107,   108,   109,    -1,   111,   112,    -1,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,    -1,   129,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,   107,   108,   109,    -1,   111,   112,    -1,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,    -1,    -1,   129,    67,    68,    69,    -1,
      71,    72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      91,    92,    93,    -1,    95,    96,    97,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    -1,   108,   109,    -1,
     111,   112,    -1,   114,   115,   116,   117,   118,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,    -1,   129,    67,
      68,    69,    -1,    71,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,   111,   112,    -1,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,
      -1,   129,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    91,    92,    93,    -1,
      95,    96,    97,    -1,    -1,    -1,    -1,    39,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,   111,   112,    -1,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,    -1,   129,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,
     112,    -1,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,    -1,   129
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     4,     5,     7,     8,     9,    10,    11,    12,
      13,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    27,    28,    35,    37,    38,    40,    42,    44,    46,
      47,    48,    51,    52,    54,    55,    62,    63,    64,    70,
      91,    92,   101,   111,   114,   119,   120,   124,   126,   128,
     130,   138,   139,   140,   141,   147,   150,   159,   162,   163,
     164,   165,   166,   173,   181,   184,   185,   188,   191,   192,
     193,   194,   198,   200,   209,   214,   215,   216,   227,   234,
     237,   250,   251,   252,   257,   261,   265,   266,   268,   269,
     270,    49,   124,   124,   156,   157,   124,     3,    15,    16,
      28,   128,   186,   187,    15,   261,     3,   236,   261,   124,
       3,   187,   240,   261,   128,   124,   129,    15,   261,   124,
       3,     4,     7,     8,     9,   258,   259,   260,   261,   261,
     261,   261,   261,   256,   261,   246,   247,   248,   254,   261,
      29,    31,    36,    41,    45,    66,   101,   135,   146,   147,
     148,   153,   167,   168,   183,   192,   253,     3,     0,   141,
       3,    45,   153,   160,   161,   257,   148,   149,    15,    25,
      30,    31,    43,    45,   208,   267,   194,     3,   128,   221,
     225,   102,    39,    67,    68,    69,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    95,
      96,    97,   104,   105,   107,   108,   109,   111,   112,   114,
     115,   116,   117,   118,   124,   126,   129,   124,   255,   256,
      11,   149,   174,   190,   128,   129,   186,   187,   239,   240,
     241,   249,   124,   255,   129,   182,   246,     3,   104,   255,
     135,   106,   102,   102,   136,   127,   102,   254,     3,     3,
     154,   155,   255,   255,   154,     3,   135,   148,     3,   101,
      68,   101,     3,    34,    50,   233,     3,     3,   210,   261,
       3,   221,   226,   228,   229,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     117,   130,   186,   263,   264,   261,   261,   261,   261,   261,
     261,   261,   261,   261,   261,   261,   261,   261,   261,   261,
     246,   261,   124,   126,   130,   263,   264,   136,   124,   261,
     101,   126,   142,   143,   144,   150,   198,   200,   209,   234,
     189,   186,   129,   124,   245,     3,    94,   230,   231,   232,
     136,   117,   186,   145,   146,   136,   261,   136,   254,   259,
     261,   106,   244,   102,   244,   255,   255,   104,     3,    53,
     195,    53,   196,     3,   211,   212,   222,   104,   104,   112,
     116,   217,   229,   126,   263,   264,    90,   106,   136,   127,
     262,   261,   126,   263,   264,    90,   156,   255,   136,   151,
     135,   144,   142,   246,   104,   106,   213,   232,   136,   102,
     135,   156,   106,    35,   117,   241,   243,   104,   152,   155,
     101,   136,     3,     4,   124,   241,    14,   197,   241,   242,
     128,   244,   102,   135,     4,   221,     4,   221,   218,   223,
     261,    90,   117,   264,   261,   261,   127,   261,    90,   264,
     136,   128,   261,   135,   136,   261,   243,   244,    94,   232,
      26,    32,   175,   177,   179,   180,    65,   158,   254,   261,
     257,   156,   230,   242,   128,   102,   201,   152,   212,   112,
       4,   111,   112,   128,   219,   224,   227,   127,   264,   136,
     127,   264,    33,    61,   169,   170,   171,   172,   127,   213,
     128,   232,   128,   124,   175,   177,   156,   136,   136,   199,
     241,    45,   101,   192,   205,   206,   207,   116,   261,   111,
     227,   223,   261,   106,   135,   171,   172,   238,   178,     3,
     156,   244,   101,   126,   147,   192,   202,   203,   204,   209,
     234,     3,    25,   135,   207,   225,   220,   116,   223,   106,
     145,   145,   145,   244,   128,   151,   135,   204,   233,   218,
     135,   225,   145,   135,   135,   136,   235,   261,     3,   112,
     218,   176,   145,   127,   124,   112,   128,   135,   230,   145,
     136,   135,   244
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
        
    case 23:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 317 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(2) - (4)].number_int);PASS1 as3_warning("embed command ignored");}
    }
    break;


  
    case 25:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 320 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 26:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 321 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 27:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 323 "parser.y"
    {
    (yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));
}
    }
    break;


  
    case 28:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 326 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 42:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 342 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (3)].code);}
    }
    break;


  
    case 43:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 343 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 44:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 346 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 51:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 367 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (2)].code);}
    }
    break;


  
    case 52:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 368 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 53:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 372 "parser.y"
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


  
    case 54:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 383 "parser.y"
    {
    PASS_ALWAYS
    (yyval.number_int) = as3_pass;
    as3_pass=0;
}
    }
    break;


  
    case 55:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 418 "parser.y"
    {(yyval.node)=(yyvsp[(2) - (2)].node);}
    }
    break;


  
    case 56:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 419 "parser.y"
    {(yyval.node)=mkdummynode();}
    }
    break;


  
    case 57:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 421 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 58:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 422 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 59:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 424 "parser.y"
    {(yyval.code) = (yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 60:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 425 "parser.y"
    {(yyval.code) = code_append((yyvsp[(1) - (3)].code), (yyvsp[(3) - (3)].code));}
    }
    break;


  
    case 61:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 428 "parser.y"
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


  
    case 62:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 488 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 63:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 488 "parser.y"
    {
    (yyval.code) = var_block((yyvsp[(2) - (2)].code), state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 64:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 492 "parser.y"
    {(yyval.code) = code_new();}
    }
    break;


  
    case 65:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 493 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 66:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 496 "parser.y"
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


  
    case 67:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 512 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 70:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 519 "parser.y"
    {
    PASS1 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable(state->method, (yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1,0);
    PASS2 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable(state->method, (yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1,0);
}
    }
    break;


  
    case 71:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 523 "parser.y"
    {
    PASS12
    (yyval.id)=(yyvsp[(1) - (1)].id);
}
    }
    break;


  
    case 72:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 528 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (2)].id);(yyval.for_start).each=0;}
    }
    break;


  
    case 73:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 529 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (3)].id);(yyval.for_start).each=1;}
    }
    break;


  
    case 74:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 531 "parser.y"
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


  
    case 75:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 551 "parser.y"
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


  
    case 76:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 595 "parser.y"
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


  
    case 77:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 611 "parser.y"
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


  
    case 78:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 623 "parser.y"
    {
    (yyval.code) = abc___break__(0, "");
}
    }
    break;


  
    case 79:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 626 "parser.y"
    {
    (yyval.code) = abc___break__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 80:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 629 "parser.y"
    {
    (yyval.code) = abc___continue__(0, "");
}
    }
    break;


  
    case 81:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 632 "parser.y"
    {
    (yyval.code) = abc___continue__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 82:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 636 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 83:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 637 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 84:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 638 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 85:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 639 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 86:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 640 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 87:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 641 "parser.y"
    {(yyval.code)=code_append((yyval.code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 88:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 643 "parser.y"
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


  
    case 89:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 654 "parser.y"
    {
    (yyval.code) = (yyvsp[(3) - (3)].code);
}
    }
    break;


  
    case 90:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 657 "parser.y"
    {PASS12 new_state();state->switch_var=alloc_local();}
    }
    break;


  
    case 91:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 657 "parser.y"
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


  
    case 92:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 688 "parser.y"
    {PASS12 new_state();
                                                      state->exception_name=(yyvsp[(3) - (5)].id);
                                               PASS1 new_variable(state->method, (yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].classinfo), 0, 0);
                                               PASS2 new_variable(state->method, (yyvsp[(3) - (5)].id), (yyvsp[(4) - (5)].classinfo), 0, 0);
                                              }
    }
    break;


  
    case 93:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 693 "parser.y"
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


  
    case 94:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 713 "parser.y"
    {PASS12 new_state();state->exception_name=0;}
    }
    break;


  
    case 95:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 713 "parser.y"
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


  
    case 96:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 729 "parser.y"
    {(yyval.catch_list).l=list_new();(yyval.catch_list).finally=0;list_append((yyval.catch_list).l,(yyvsp[(1) - (1)].exception));}
    }
    break;


  
    case 97:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 730 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (2)].catch_list);list_append((yyval.catch_list).l,(yyvsp[(2) - (2)].exception));}
    }
    break;


  
    case 98:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 731 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (1)].catch_list);}
    }
    break;


  
    case 99:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 732 "parser.y"
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


  
    case 100:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 740 "parser.y"
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


  
    case 101:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 749 "parser.y"
    {PASS12 new_state();
                 state->method->has_exceptions=1;
                 state->method->late_binding=1;//for invariant scope_code
                }
    }
    break;


  
    case 102:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 752 "parser.y"
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


  
    case 103:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 798 "parser.y"
    {
    (yyval.code)=(yyvsp[(2) - (2)].value).c;
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 104:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 803 "parser.y"
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


  
    case 105:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 814 "parser.y"
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


  
    case 106:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 824 "parser.y"
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


  
    case 108:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 842 "parser.y"
    {PASS12 (yyval.id)="package";}
    }
    break;


  
    case 109:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 843 "parser.y"
    {PASS12 (yyval.id)="namespace";}
    }
    break;


  
    case 110:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 844 "parser.y"
    {PASS12 (yyval.id)="NaN";}
    }
    break;


  
    case 111:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 846 "parser.y"
    {PASS12 (yyval.id) = concat3((yyvsp[(1) - (3)].id),".",(yyvsp[(3) - (3)].id));free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;}
    }
    break;


  
    case 112:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 847 "parser.y"
    {PASS12 (yyval.id)=strdup((yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 113:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 849 "parser.y"
    {PASS12 startpackage((yyvsp[(2) - (3)].id));free((yyvsp[(2) - (3)].id));(yyvsp[(2) - (3)].id)=0;}
    }
    break;


  
    case 114:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 850 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 115:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 851 "parser.y"
    {PASS12 startpackage("");}
    }
    break;


  
    case 116:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 852 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 117:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 875 "parser.y"
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


  
    case 118:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 883 "parser.y"
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


  
    case 119:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 897 "parser.y"
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


  
    case 120:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 913 "parser.y"
    {PASS12 (yyval.flags).flags=0;(yyval.flags).ns=0;}
    }
    break;


  
    case 121:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 914 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 122:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 915 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 123:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 916 "parser.y"
    {
    PASS12 
    (yyval.flags).flags=(yyvsp[(1) - (2)].flags).flags|(yyvsp[(2) - (2)].flags).flags;
    if((yyvsp[(1) - (2)].flags).ns && (yyvsp[(2) - (2)].flags).ns) syntaxerror("only one namespace allowed in one declaration");
    (yyval.flags).ns=(yyvsp[(1) - (2)].flags).ns?(yyvsp[(1) - (2)].flags).ns:(yyvsp[(2) - (2)].flags).ns;

}
    }
    break;


  
    case 124:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 923 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PUBLIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 125:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 924 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PRIVATE;(yyval.flags).ns=0;}
    }
    break;


  
    case 126:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 925 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PROTECTED;(yyval.flags).ns=0;}
    }
    break;


  
    case 127:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 926 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_STATIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 128:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 927 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_DYNAMIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 129:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 928 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_FINAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 130:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 929 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_OVERRIDE;(yyval.flags).ns=0;}
    }
    break;


  
    case 131:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 930 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NATIVE;(yyval.flags).ns=0;}
    }
    break;


  
    case 132:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 931 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PACKAGEINTERNAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 133:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 938 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 134:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 939 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 135:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 941 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 136:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 942 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 137:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 944 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 138:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 945 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 139:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 949 "parser.y"
    {PASS12 startclass(&(yyvsp[(1) - (6)].flags),(yyvsp[(3) - (6)].id),(yyvsp[(4) - (6)].classinfo),(yyvsp[(5) - (6)].classinfo_list));}
    }
    break;


  
    case 140:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 951 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 141:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 955 "parser.y"
    {PASS12 (yyvsp[(1) - (5)].flags).flags|=FLAG_INTERFACE;
                                          startclass(&(yyvsp[(1) - (5)].flags),(yyvsp[(3) - (5)].id),0,(yyvsp[(4) - (5)].classinfo_list));}
    }
    break;


  
    case 142:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 958 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 150:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 970 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(2) - (4)].number_int);PASS1 as3_warning("embed command ignored");}
    }
    break;


  
    case 151:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 972 "parser.y"
    {
    code_t*c = state->cls->static_init->header;
    c = code_append(c, (yyvsp[(1) - (1)].code));  
    state->cls->static_init->header = c;
}
    }
    break;


  
    case 157:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 983 "parser.y"
    {
    syntaxerror("variable declarations not allowed in interfaces");
}
    }
    break;


  
    case 158:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 986 "parser.y"
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


  
    case 161:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1061 "parser.y"
    {PASS12 setslotstate(&(yyvsp[(1) - (2)].flags),(yyvsp[(2) - (2)].token));}
    }
    break;


  
    case 162:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1061 "parser.y"
    {PASS12 (yyval.code)=(yyvsp[(4) - (4)].code);setslotstate(0, 0);}
    }
    break;


  
    case 163:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1063 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 164:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1064 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 165:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1067 "parser.y"
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


  
    case 166:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1150 "parser.y"
    {(yyval.constant)=0;}
    }
    break;


  
    case 167:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1151 "parser.y"
    {
  (yyval.constant) = malloc(sizeof(constant_t));
  *(yyval.constant) = node_eval((yyvsp[(2) - (2)].node));
  if((yyval.constant)->type == CONSTANT_UNKNOWN) {
    syntaxerror("can't evaluate default parameter value (needs to be a compile-time constant)");
  }
}
    }
    break;


  
    case 168:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1159 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_int));}
    }
    break;


  
    case 169:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1160 "parser.y"
    {
    (yyval.constant) = constant_new_uint((yyvsp[(1) - (1)].number_uint));
}
    }
    break;


  
    case 170:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1163 "parser.y"
    {(yyval.constant) = constant_new_float((yyvsp[(1) - (1)].number_float));}
    }
    break;


  
    case 171:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1164 "parser.y"
    {(yyval.constant) = constant_new_string2((yyvsp[(1) - (1)].str).str,(yyvsp[(1) - (1)].str).len);free((char*)(yyvsp[(1) - (1)].str).str);}
    }
    break;


  
    case 172:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1165 "parser.y"
    {(yyval.constant) = constant_new_true((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 173:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1166 "parser.y"
    {(yyval.constant) = constant_new_false((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 174:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1167 "parser.y"
    {(yyval.constant) = constant_new_null((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 175:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1168 "parser.y"
    {(yyval.constant) = constant_new_undefined((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 176:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1169 "parser.y"
    {(yyval.constant) = constant_new_float(__builtin_nan(""));}
    }
    break;


  
    case 177:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1177 "parser.y"
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


  
    case 178:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1188 "parser.y"
    {PASS_ALWAYS if(!xml_level++) tokenizer_begin_xml();}
    }
    break;


  
    case 179:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1189 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xmltext();}
    }
    break;


  
    case 180:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1190 "parser.y"
    {PASS_ALWAYS if(!--xml_level) tokenizer_end_xml(); else tokenizer_begin_xmltext();}
    }
    break;


  
    case 181:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1192 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xmltext();}
    }
    break;


  
    case 182:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1192 "parser.y"
    {
    (yyval.node) = (yyvsp[(2) - (4)].node);
}
    }
    break;


  
    case 183:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1195 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xml();}
    }
    break;


  
    case 184:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1195 "parser.y"
    {
    (yyval.node) = (yyvsp[(2) - (4)].node);
}
    }
    break;


  
    case 185:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1198 "parser.y"
    {(yyval.node)=mkstringnode("");}
    }
    break;


  
    case 186:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1199 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node),(yyvsp[(2) - (2)].node));
}
    }
    break;


  
    case 187:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1202 "parser.y"
    {
    char* str = string_cstr(&(yyvsp[(2) - (2)].str));
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node),mkstringnode(str));
    free(str);
}
    }
    break;


  
    case 188:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1207 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node), mkstringnode(">"));
}
    }
    break;


  
    case 189:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1210 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node),(yyvsp[(2) - (2)].node));
}
    }
    break;


  
    case 190:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1213 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (3)].node), mkaddnode((yyvsp[(2) - (3)].node),(yyvsp[(3) - (3)].node)));
}
    }
    break;


  
    case 191:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1216 "parser.y"
    {
    (yyval.node) = mkstringnode((yyvsp[(1) - (1)].id));
}
    }
    break;


  
    case 192:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1219 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 193:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1223 "parser.y"
    {
    (yyval.node) = mkstringnode("");
}
    }
    break;


  
    case 194:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1226 "parser.y"
    {
    (yyval.node) = mkaddnode(mkstringnode(" "),(yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 195:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1230 "parser.y"
    {
    //$$ = allocprintf("<%s%s/>", $2, $3, $5, $8);
    (yyval.node) = mkaddnode(mkaddnode(mkaddnode(mkstringnode("<"),(yyvsp[(2) - (6)].node)),(yyvsp[(3) - (6)].node)),mkstringnode("/>"));
}
    }
    break;


  
    case 196:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1234 "parser.y"
    {
    //$$ = allocprintf("<%s%s>%s</%s>", $2, $3, $5, $8);
    (yyval.node) = mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(
         mkstringnode("<"),(yyvsp[(2) - (10)].node)),(yyvsp[(3) - (10)].node)),mkstringnode(">")),(yyvsp[(5) - (10)].node)),mkstringnode("</")),(yyvsp[(8) - (10)].node)),mkstringnode(">"));
}
    }
    break;


  
    case 197:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1239 "parser.y"
    {
    //$$ = allocprintf("<%s%s>%s%s</%s>", $2, $3, $5, $6, $9);
    (yyval.node) = mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(mkaddnode(
         mkstringnode("<"),(yyvsp[(2) - (11)].node)),(yyvsp[(3) - (11)].node)),mkstringnode(">")),(yyvsp[(5) - (11)].node)),(yyvsp[(6) - (11)].node)),mkstringnode("</")),(yyvsp[(9) - (11)].node)),mkstringnode(">"));
}
    }
    break;


  
    case 198:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1245 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 199:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1248 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node), mkaddnode(mkstringnode(" "),(yyvsp[(2) - (2)].node)));
}
    }
    break;


  
    case 200:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1251 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 201:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1254 "parser.y"
    {
    char* str = string_cstr(&(yyvsp[(3) - (3)].str));
    (yyval.node) = mkaddnode((yyvsp[(1) - (3)].node), mkstringnode(concat2("=",str)));
    free(str);
}
    }
    break;


  
    case 202:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1259 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (3)].node), mkaddnode(mkstringnode("=\""), mkaddnode((yyvsp[(3) - (3)].node), mkstringnode("\""))));
}
    }
    break;


  
    case 203:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1262 "parser.y"
    {
    (yyval.node) = mkaddnode(mkaddnode(mkstringnode(concat2((yyvsp[(1) - (3)].id),"=\"")), (yyvsp[(3) - (3)].node)), mkstringnode("\""));
}
    }
    break;


  
    case 204:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1265 "parser.y"
    {
    char* str = string_cstr(&(yyvsp[(3) - (3)].str));
    (yyval.node)=mkstringnode(allocprintf("%s=%s", (yyvsp[(1) - (3)].id),str));
    free(str);
    free((yyvsp[(1) - (3)].id));free((char*)(yyvsp[(3) - (3)].str).str);
}
    }
    break;


  
    case 205:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1275 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
}
    }
    break;


  
    case 206:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1279 "parser.y"
    {
    PASS12
    (yyval.params)=(yyvsp[(1) - (1)].params);
}
    }
    break;


  
    case 207:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1285 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(2) - (2)].param));
}
    }
    break;


  
    case 208:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1291 "parser.y"
    {
    PASS12
    (yyval.params) =(yyvsp[(1) - (4)].params);
    (yyval.params).varargs=1;
    list_append((yyval.params).list, (yyvsp[(4) - (4)].param));
}
    }
    break;


  
    case 209:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1299 "parser.y"
    {
    PASS12
    (yyval.params) = (yyvsp[(1) - (3)].params);
    list_append((yyval.params).list, (yyvsp[(3) - (3)].param));
}
    }
    break;


  
    case 210:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1304 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
    list_append((yyval.params).list, (yyvsp[(1) - (1)].param));
}
    }
    break;


  
    case 211:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1310 "parser.y"
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


  
    case 212:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1318 "parser.y"
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


  
    case 215:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1328 "parser.y"
    {PASS12 (yyval.token)=0;}
    }
    break;


  
    case 216:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1331 "parser.y"
    {PASS12 startfunction(&(yyvsp[(1) - (9)].flags),(yyvsp[(3) - (9)].token),(yyvsp[(4) - (9)].id),&(yyvsp[(6) - (9)].params),(yyvsp[(8) - (9)].classinfo));}
    }
    break;


  
    case 217:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1332 "parser.y"
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


  
    case 219:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1348 "parser.y"
    {PASS12 (yyval.id)=0;}
    }
    break;


  
    case 220:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1350 "parser.y"
    {PASS12 innerfunction((yyvsp[(2) - (7)].id),&(yyvsp[(4) - (7)].params),(yyvsp[(6) - (7)].classinfo));}
    }
    break;


  
    case 221:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1351 "parser.y"
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


  
    case 222:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1373 "parser.y"
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


  
    case 223:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1393 "parser.y"
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


  
    case 226:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1411 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();list_append((yyval.classinfo_list), (yyvsp[(1) - (1)].classinfo));}
    }
    break;


  
    case 227:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1412 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(1) - (3)].classinfo_list);list_append((yyval.classinfo_list),(yyvsp[(3) - (3)].classinfo));}
    }
    break;


  
    case 228:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1414 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(1) - (1)].classinfo);}
    }
    break;


  
    case 229:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1415 "parser.y"
    {PASS12 (yyval.classinfo)=TYPE_ANY;}
    }
    break;


  
    case 230:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1416 "parser.y"
    {PASS12 (yyval.classinfo)=TYPE_VOID;}
    }
    break;


  
    case 231:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1425 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 232:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1426 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 233:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1430 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 234:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1431 "parser.y"
    {(yyval.value_list)=(yyvsp[(2) - (3)].value_list);}
    }
    break;


  
    case 235:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1433 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 238:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1437 "parser.y"
    {(yyval.value_list).number=1;
                                                  (yyval.value_list).cc = (yyvsp[(1) - (1)].value).c;
                                                 }
    }
    break;


  
    case 239:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1441 "parser.y"
    {(yyval.value_list) = (yyvsp[(1) - (2)].value_list);}
    }
    break;


  
    case 240:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1442 "parser.y"
    {
                                                  (yyval.value_list).number= (yyvsp[(1) - (2)].value_list).number+1;
                                                  (yyval.value_list).cc = code_append((yyvsp[(1) - (2)].value_list).cc, (yyvsp[(2) - (2)].value).c);
                                                  }
    }
    break;


  
    case 242:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1448 "parser.y"
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


  
    case 243:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1495 "parser.y"
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


  
    case 244:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1545 "parser.y"
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


  
    case 245:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1566 "parser.y"
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


  
    case 246:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1588 "parser.y"
    {
    (yyval.code) = abc_returnvoid(0);
}
    }
    break;


  
    case 247:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1591 "parser.y"
    {
    (yyval.code) = (yyvsp[(2) - (2)].value).c;
    (yyval.code) = abc_returnvalue((yyval.code));
}
    }
    break;


  
    case 248:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1598 "parser.y"
    {
    (yyval.value) = node_read((yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 249:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1601 "parser.y"
    {
    (yyval.value) = node_read((yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 250:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1604 "parser.y"
    {
    (yyval.node) = mkmultinode(&node_comma, (yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 251:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1607 "parser.y"
    {
    (yyval.node) = multinode_extend((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
}
    }
    break;


  
    case 252:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1610 "parser.y"
    { 
    (yyval.code) = node_exec((yyvsp[(1) - (1)].node)); 
}
    }
    break;


  
    case 253:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1613 "parser.y"
    { 
    (yyval.code) = (yyvsp[(1) - (3)].code);
    (yyval.code) = code_append((yyval.code), node_exec((yyvsp[(3) - (3)].node))); 
}
    }
    break;


  
    case 254:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1618 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 255:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1619 "parser.y"
    {(yyval.value_list)=(yyvsp[(1) - (1)].value_list);}
    }
    break;


  
    case 256:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1621 "parser.y"
    {(yyval.code)=abc_pushstring(0,(yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 257:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1622 "parser.y"
    {(yyval.code)=abc_pushstring2(0,&(yyvsp[(1) - (1)].str));}
    }
    break;


  
    case 258:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1623 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 259:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1624 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 260:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1625 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 261:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1627 "parser.y"
    {
    (yyval.value_list).cc = 0;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(1) - (3)].code));
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (3)].value).c);
    (yyval.value_list).number = 2;
}
    }
    break;


  
    case 262:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1633 "parser.y"
    {
    (yyval.value_list).cc = (yyvsp[(1) - (5)].value_list).cc;
    (yyval.value_list).number = (yyvsp[(1) - (5)].value_list).number+2;
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(3) - (5)].code));
    (yyval.value_list).cc = code_append((yyval.value_list).cc, (yyvsp[(5) - (5)].value).c);
}
    }
    break;


  
    case 263:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1642 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 264:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1643 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 265:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1644 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 266:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1645 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 267:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1646 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 268:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1647 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    }
    break;


  
    case 269:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1649 "parser.y"
    { 
    (yyval.node) = mkconstnode((yyvsp[(1) - (1)].constant));
}
    }
    break;


  
    case 270:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1653 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 271:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1658 "parser.y"
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


  
    case 272:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1676 "parser.y"
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


  
    case 273:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1687 "parser.y"
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


  
    case 274:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1697 "parser.y"
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


  
    case 275:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1706 "parser.y"
    {(yyval.node) = mknode2(&node_lt,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 276:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1707 "parser.y"
    {(yyval.node) = mknode2(&node_gt,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 277:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1708 "parser.y"
    {(yyval.node) = mknode2(&node_le,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 278:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1709 "parser.y"
    {(yyval.node) = mknode2(&node_ge,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 279:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1710 "parser.y"
    {(yyval.node) = mknode2(&node_eqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 280:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1711 "parser.y"
    {(yyval.node) = mknode2(&node_eqeqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 281:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1712 "parser.y"
    {(yyval.node) = mknode2(&node_noteqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 282:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1713 "parser.y"
    {(yyval.node) = mknode2(&node_noteq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 283:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1714 "parser.y"
    {(yyval.node) = mknode2(&node_oror,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 284:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1715 "parser.y"
    {(yyval.node) = mknode2(&node_andand,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 285:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1716 "parser.y"
    {(yyval.node) = mknode1(&node_not, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 286:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1717 "parser.y"
    {(yyval.node) = mknode1(&node_bitnot, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 287:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1718 "parser.y"
    {(yyval.node) = mknode2(&node_bitand, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 288:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1719 "parser.y"
    {(yyval.node) = mknode2(&node_bitxor, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 289:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1720 "parser.y"
    {(yyval.node) = mknode2(&node_bitor, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 290:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1721 "parser.y"
    {(yyval.node) = mknode2(&node_shr, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 291:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1722 "parser.y"
    {(yyval.node) = mknode2(&node_ushr, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 292:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1723 "parser.y"
    {(yyval.node) = mknode2(&node_shl, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 293:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1724 "parser.y"
    {(yyval.node) = mknode2(&node_div, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 294:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1725 "parser.y"
    {(yyval.node) = mknode2(&node_mod, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 295:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1726 "parser.y"
    {(yyval.node) = mknode2(&node_plus, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 296:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1727 "parser.y"
    {(yyval.node) = mknode2(&node_minus, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 297:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1728 "parser.y"
    {(yyval.node) = mknode2(&node_multiply, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 298:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1729 "parser.y"
    {(yyval.node) = mknode2(&node_in, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 299:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1730 "parser.y"
    {(yyval.node) = mknode2(&node_as, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 300:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1731 "parser.y"
    {(yyval.node) = mknode2(&node_instanceof, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 301:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1732 "parser.y"
    {(yyval.node) = mknode2(&node_is, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 302:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1733 "parser.y"
    {(yyval.node) = mknode1(&node_typeof, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 303:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1734 "parser.y"
    {(yyval.node) = mknode1(&node_void, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 304:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1735 "parser.y"
    { (yyval.node) = mkconstnode(constant_new_undefined());}
    }
    break;


  
    case 305:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1736 "parser.y"
    { (yyval.node)=(yyvsp[(2) - (3)].node);}
    }
    break;


  
    case 306:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1737 "parser.y"
    {(yyval.node) = mknode1(&node_neg, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 307:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1738 "parser.y"
    {(yyval.node) = mknode2(&node_arraylookup, (yyvsp[(1) - (4)].node),(yyvsp[(3) - (4)].node));}
    }
    break;


  
    case 308:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1739 "parser.y"
    {(yyval.node) = mknode2(&node_muleq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 309:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1740 "parser.y"
    {(yyval.node) = mknode2(&node_modeq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 310:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1741 "parser.y"
    {(yyval.node) = mknode2(&node_shleq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 311:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1742 "parser.y"
    {(yyval.node) = mknode2(&node_shreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 312:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1743 "parser.y"
    {(yyval.node) = mknode2(&node_ushreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 313:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1744 "parser.y"
    { (yyval.node) = mknode2(&node_diveq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 314:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1745 "parser.y"
    { (yyval.node) = mknode2(&node_bitoreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 315:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1746 "parser.y"
    { (yyval.node) = mknode2(&node_bitxoreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 316:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1747 "parser.y"
    { (yyval.node) = mknode2(&node_bitandeq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 317:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1748 "parser.y"
    { (yyval.node) = mknode2(&node_pluseq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 318:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1749 "parser.y"
    { (yyval.node) = mknode2(&node_minuseq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 319:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1750 "parser.y"
    { (yyval.node) = mknode2(&node_assign, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 320:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1751 "parser.y"
    { (yyval.node) = mknode3(&node_tenary, (yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node));}
    }
    break;


  
    case 321:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1753 "parser.y"
    { (yyval.node) = mknode1(&node_rplusplus, (yyvsp[(1) - (2)].node));}
    }
    break;


  
    case 322:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1754 "parser.y"
    { (yyval.node) = mknode1(&node_rminusminus, (yyvsp[(1) - (2)].node));}
    }
    break;


  
    case 323:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1755 "parser.y"
    {(yyval.node) = mknode1(&node_lplusplus, (yyvsp[(2) - (2)].node)); }
    }
    break;


  
    case 324:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1756 "parser.y"
    {(yyval.node) = mknode1(&node_lminusminus, (yyvsp[(2) - (2)].node)); }
    }
    break;


  
    case 325:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1759 "parser.y"
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


  
    case 326:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1773 "parser.y"
    {
    typedcode_t v;
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(2) - (2)].id)};
    v.c = abc_getlex2(0, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 327:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1781 "parser.y"
    {PASS12 new_state();state->xmlfilter=1;}
    }
    break;


  
    case 328:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1781 "parser.y"
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


  
    case 329:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1836 "parser.y"
    {(yyval.id)="*";}
    }
    break;


  
    case 331:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1839 "parser.y"
    {(yyval.id)="*";}
    }
    break;


  
    case 332:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1880 "parser.y"
    {
}
    }
    break;


  
    case 333:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1883 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].id), (yyvsp[(5) - (5)].id), 0, 0);
}
    }
    break;


  
    case 334:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1886 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (3)].node));
    multiname_t m = {MULTINAME, 0, &nopackage_namespace_set, (yyvsp[(3) - (3)].id)};
    v.c = abc_getdescendants2(v.c, &m);
    v.t = TYPE_XMLLIST;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 335:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1893 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].id), (yyvsp[(5) - (5)].id), 1, 0);
}
    }
    break;


  
    case 336:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1896 "parser.y"
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


  
    case 337:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1907 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (4)].node));
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(4) - (4)].id)};
    v.c = abc_getproperty2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 338:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1915 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (6)].node), (yyvsp[(4) - (6)].id), (yyvsp[(6) - (6)].id), 0, 1);
}
    }
    break;


  
    case 339:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1919 "parser.y"
    {
    typedcode_t v = node_read((yyvsp[(1) - (4)].node));
    multiname_t m = {MULTINAMEA, 0, &nopackage_namespace_set, (yyvsp[(4) - (4)].id)};
    v.c = abc_getdescendants2(v.c, &m);
    v.t = TYPE_STRING;
    (yyval.node) = mkcodenode(v);
}
    }
    break;


  
    case 340:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1926 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (6)].node), (yyvsp[(4) - (6)].id), (yyvsp[(6) - (6)].id), 1, 1);
}
    }
    break;


  
    case 341:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1930 "parser.y"
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


  
    case 342:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1940 "parser.y"
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


  
    case 343:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1951 "parser.y"
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


  
    case 344:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2149 "parser.y"
    {
    PASS1
    char*name = (yyvsp[(1) - (1)].id);
    /* Queue unresolved identifiers for checking against the parent
       function's variables.
       We consider everything which is not a local variable "unresolved".
       This encompasses class names, members of the surrounding class
       etc. which is *correct* because local variables of the parent function
       would shadow those.
       */

    if(!find_variable(state, name)) {
        unknown_variable(name);
        /* let the compiler know that it might want to check the current directory/package
           for this identifier- maybe there's a file $1.as defining $1. */
        as3_schedule_class_noerror(state->package, name);
    }
   
    (yyval.node) = 0;
    PASS2
    char*name = (yyvsp[(1) - (1)].id);
    (yyval.node) = resolve_identifier(name);
}
    }
    break;


  
    case 345:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2184 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (2)].id);
    n->url = (yyvsp[(2) - (2)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 346:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2191 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 347:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2198 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].str).str;
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 348:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2205 "parser.y"
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


  
    case 349:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2228 "parser.y"
    {
    (yyval.code) = 0;
    (yyval.code) = code_append((yyval.code), node_read((yyvsp[(4) - (4)].node)).c);
    (yyval.code) = abc_dxnslate((yyval.code));
}
    }
    break;


  
    case 350:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2234 "parser.y"
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
#line 7164 "parser.tab.c"
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



