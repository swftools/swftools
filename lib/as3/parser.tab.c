
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
#line 401 "parser.y"

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
#line 852 "parser.y"

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
#line 995 "parser.y"

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
#line 1169 "parser.y"

    static int xml_level = 0;


/* Line 274 of skeleton.m4  */
#line 1837 "parser.y"

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
#line 2012 "parser.y"

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
#line 2171 "parser.y"

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
#define YYFINAL  162
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3432

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  137
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  136
/* YYNRULES -- Number of rules.  */
#define YYNRULES  358
/* YYNRULES -- Number of states.  */
#define YYNSTATES  615

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
    1069,  1073,  1074,  1077,  1080,  1085,  1090,  1093,  1098
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     138,     0,    -1,   139,    -1,    -1,   140,    -1,   141,    -1,
     140,   141,    -1,   189,    -1,   201,    -1,   199,    -1,   235,
      -1,   210,    -1,   150,    -1,   152,   128,   139,   135,    -1,
     101,    -1,    -1,   143,    -1,   144,    -1,   143,   144,    -1,
     201,    -1,   199,    -1,   235,    -1,   210,    -1,   150,    -1,
     152,   128,   142,   135,    -1,   126,   151,   262,   127,    -1,
     101,    -1,   146,    -1,    -1,   146,   148,    -1,   148,    -1,
     271,    -1,   192,    -1,   164,    -1,   165,    -1,   166,    -1,
     167,    -1,   174,    -1,   160,    -1,   186,    -1,   182,    -1,
     258,    -1,   272,    -1,   270,    -1,   128,   146,   135,    -1,
     128,   135,    -1,   101,    -1,   147,    -1,   154,    -1,   168,
      -1,   169,    -1,   254,    -1,   184,    -1,   152,   128,   146,
     135,    -1,   148,   101,    -1,   148,    -1,   147,    -1,    -1,
       3,    90,     3,    -1,   104,   262,    -1,    -1,    45,   155,
      -1,    31,   155,    -1,   156,    -1,   155,   102,   156,    -1,
       3,   245,   153,    -1,    -1,   158,   149,    -1,    -1,    65,
     157,    -1,    64,   124,   256,   136,   157,   159,    -1,    -1,
     154,    -1,   258,    -1,    45,     3,   245,    -1,     3,    -1,
      10,   124,    -1,    10,    49,   124,    -1,   163,   161,   101,
     256,   101,   258,   136,   157,    -1,   163,   162,    68,   256,
     136,   157,    -1,    11,   124,   256,   136,   157,    -1,    12,
     157,    11,   124,   256,   136,    -1,    66,    -1,    66,     3,
      -1,    29,    -1,    29,     3,    -1,    -1,   171,    -1,   173,
      -1,   171,   173,    -1,   172,    -1,   171,   172,    -1,    33,
     262,   106,   145,    -1,    61,   106,   145,    -1,    -1,    13,
     124,   175,   262,   136,   128,   170,   135,    -1,    -1,    32,
     124,     3,   245,   136,   177,   128,   145,   135,    -1,    -1,
      26,   128,   179,   145,   135,    -1,   176,    -1,   180,   176,
      -1,   180,    -1,   180,   178,    -1,   178,    -1,    -1,    51,
     128,   183,   145,   135,   181,    -1,    36,   256,    -1,    36,
      -1,    38,   124,   256,   136,    -1,   185,   149,    -1,     3,
      -1,    16,    -1,    15,    -1,    28,    -1,   188,   129,   187,
      -1,   187,    -1,    -1,    16,   188,   128,   190,   142,   135,
      -1,    -1,    16,   128,   191,   142,   135,    -1,    40,     3,
      -1,    40,   241,    -1,    40,   188,   129,   117,    -1,    -1,
     194,    -1,   195,    -1,   194,   195,    -1,    19,    -1,    20,
      -1,    17,    -1,    37,    -1,    46,    -1,    48,    -1,    47,
      -1,    24,    -1,    22,    -1,     3,    -1,    -1,    53,   242,
      -1,    -1,    53,   243,    -1,    -1,    14,   243,    -1,    -1,
     193,    30,     3,   196,   198,   128,   200,   203,   135,    -1,
      -1,   193,    43,     3,   197,   128,   202,   206,   135,    -1,
      -1,   204,    -1,   205,    -1,   204,   205,    -1,   101,    -1,
     152,   128,   203,   135,    -1,   210,    -1,   235,    -1,   126,
     151,   262,   127,    -1,   147,    -1,    -1,   207,    -1,   208,
      -1,   207,   208,    -1,   101,    -1,    45,     3,    -1,   193,
      25,   234,     3,   124,   231,   136,   245,    -1,    45,    -1,
      31,    -1,    -1,   193,   209,   211,   212,    -1,   213,    -1,
     212,   102,   213,    -1,     3,   245,   153,    -1,    -1,   104,
     262,    -1,     7,    -1,     8,    -1,     9,    -1,     4,    -1,
      55,    -1,    54,    -1,    44,    -1,    27,    -1,    28,    -1,
     228,    -1,   111,    -1,   112,    -1,    -1,    -1,   128,   262,
     221,   135,    -1,    -1,   128,   262,   223,   135,    -1,    -1,
     224,   220,    -1,   224,     4,    -1,   224,   112,    -1,   228,
     224,    -1,   225,   228,   224,    -1,     3,    -1,   222,    -1,
      -1,   229,    -1,   217,   226,   227,   116,   219,   112,    -1,
     217,   226,   227,   218,   224,   111,   116,   226,   219,   112,
      -1,   217,   226,   227,   218,   224,   225,   111,   116,   226,
     219,   112,    -1,   230,    -1,   229,   230,    -1,   222,    -1,
     222,   104,     4,    -1,   222,   104,   222,    -1,     3,   104,
     222,    -1,     3,   104,     4,    -1,    -1,   232,    -1,    94,
     233,    -1,   232,   102,    94,   233,    -1,   232,   102,   233,
      -1,   233,    -1,     3,   106,   244,   214,    -1,     3,   214,
      -1,    50,    -1,    34,    -1,    -1,    -1,   193,    25,   234,
       3,   124,   231,   136,   245,   128,   236,   145,   135,    -1,
       3,    -1,    -1,    -1,    25,   237,   124,   231,   136,   245,
     128,   239,   145,   135,    -1,   187,    -1,   188,   129,   187,
      -1,   241,    -1,   240,    -1,   242,    -1,   243,   102,   242,
      -1,   242,    -1,   117,    -1,    35,    -1,   106,   244,    -1,
      -1,    -1,   124,   247,   136,    -1,    -1,   248,    -1,   249,
      -1,   255,    -1,   248,   102,    -1,   249,   255,    -1,    -1,
      23,   262,   250,   246,    -1,   262,   124,   247,   136,    -1,
      52,   124,   247,   136,    -1,    63,   262,    -1,    41,    -1,
      41,   256,    -1,   262,    -1,   257,    -1,   262,    -1,   257,
     102,   262,    -1,   262,    -1,   258,   102,   262,    -1,    -1,
     261,    -1,     3,    -1,     4,    -1,     7,    -1,     8,    -1,
       9,    -1,   260,   106,   255,    -1,   261,   102,   260,   106,
     255,    -1,   238,    -1,   266,    -1,   251,    -1,   253,    -1,
     252,    -1,   268,    -1,   215,    -1,   216,    -1,     5,    -1,
      18,    -1,   126,   247,   127,    -1,    70,   259,   135,    -1,
     262,   111,   262,    -1,   262,   112,   262,    -1,   262,    75,
     262,    -1,   262,    76,   262,    -1,   262,    71,   262,    -1,
     262,    72,   262,    -1,   262,    74,   262,    -1,   262,    73,
     262,    -1,   262,    88,   262,    -1,   262,    89,   262,    -1,
     120,   262,    -1,   119,   262,    -1,   262,   109,   262,    -1,
     262,   108,   262,    -1,   262,   107,   262,    -1,   262,    97,
     262,    -1,   262,    96,   262,    -1,   262,    95,   262,    -1,
     262,   116,   262,    -1,   262,   118,   262,    -1,   262,   115,
     262,    -1,   262,   114,   262,    -1,   262,   117,   262,    -1,
     262,    68,   262,    -1,   262,    69,   262,    -1,   262,    39,
     262,    -1,   262,    67,   262,    -1,    42,   262,    -1,    35,
     262,    -1,    35,    -1,   124,   257,   136,    -1,   114,   262,
      -1,   262,   126,   262,   127,    -1,   262,    80,   262,    -1,
     262,    79,   262,    -1,   262,    86,   262,    -1,   262,    85,
     262,    -1,   262,    87,   262,    -1,   262,    78,   262,    -1,
     262,    77,   262,    -1,   262,    84,   262,    -1,   262,    81,
     262,    -1,   262,    82,   262,    -1,   262,    83,   262,    -1,
     262,   104,   262,    -1,   262,   105,   262,   106,   262,    -1,
     262,    92,    -1,   262,    91,    -1,    92,   262,    -1,    91,
     262,    -1,    52,   129,     3,    -1,   130,     3,    -1,    -1,
     262,   129,   124,   263,   262,   136,    -1,     3,    -1,   117,
      -1,   187,    -1,   117,    -1,   262,    90,   262,    -1,   262,
     129,   264,    90,   265,    -1,   262,    93,   265,    -1,   262,
      93,   264,    90,   265,    -1,   262,   129,   126,   262,   127,
      -1,   262,   129,   130,   265,    -1,   262,   129,   130,   264,
      90,   265,    -1,   262,    93,   130,   265,    -1,   262,    93,
     130,   264,    90,   265,    -1,   262,   129,   130,   126,   262,
     127,    -1,   262,    93,   130,   126,   262,   127,    -1,   262,
     129,   265,    -1,    -1,   267,     3,    -1,    15,     3,    -1,
      15,     3,   104,     3,    -1,    15,     3,   104,     4,    -1,
     193,   269,    -1,    62,    15,   104,   262,    -1,    21,    15,
     242,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   292,   292,   294,   294,   295,   296,   298,   299,   300,
     301,   302,   303,   304,   305,   307,   307,   308,   309,   311,
     312,   313,   314,   315,   316,   317,   318,   320,   321,   323,
     326,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   346,   347,   348,   349,
     350,   351,   352,   353,   365,   366,   370,   381,   389,   416,
     417,   419,   420,   422,   423,   425,   486,   486,   490,   491,
     494,   510,   511,   512,   517,   521,   526,   527,   529,   549,
     593,   609,   621,   624,   627,   630,   634,   635,   636,   637,
     638,   639,   641,   652,   655,   655,   686,   686,   711,   711,
     727,   728,   729,   730,   738,   747,   747,   796,   801,   812,
     822,   839,   840,   841,   842,   844,   845,   847,   847,   849,
     849,   873,   881,   895,   911,   912,   913,   914,   921,   922,
     923,   924,   925,   926,   927,   928,   929,   930,   934,   935,
     937,   938,   940,   941,   945,   943,   951,   949,   958,   959,
     960,   961,   962,   963,   964,   965,   966,   968,   974,   975,
     976,   977,   978,   979,   982,  1055,  1055,  1057,  1057,  1059,
    1060,  1062,  1146,  1147,  1155,  1156,  1159,  1160,  1161,  1162,
    1163,  1164,  1165,  1173,  1184,  1185,  1186,  1188,  1188,  1191,
    1191,  1194,  1195,  1198,  1203,  1206,  1209,  1212,  1215,  1219,
    1222,  1226,  1230,  1235,  1241,  1244,  1247,  1250,  1255,  1258,
    1261,  1271,  1275,  1281,  1287,  1295,  1300,  1306,  1314,  1322,
    1323,  1324,  1327,  1326,  1343,  1344,  1346,  1345,  1369,  1389,
    1404,  1405,  1407,  1408,  1410,  1411,  1412,  1421,  1422,  1426,
    1427,  1429,  1430,  1431,  1433,  1437,  1438,  1443,  1444,  1491,
    1541,  1562,  1584,  1587,  1594,  1597,  1600,  1603,  1606,  1609,
    1614,  1615,  1617,  1618,  1619,  1620,  1621,  1623,  1629,  1638,
    1639,  1640,  1641,  1642,  1643,  1645,  1649,  1654,  1672,  1683,
    1693,  1702,  1703,  1704,  1705,  1706,  1707,  1708,  1709,  1710,
    1711,  1712,  1713,  1714,  1715,  1716,  1717,  1718,  1719,  1720,
    1721,  1722,  1723,  1724,  1725,  1726,  1727,  1728,  1729,  1730,
    1731,  1732,  1733,  1734,  1735,  1736,  1737,  1738,  1739,  1740,
    1741,  1742,  1743,  1744,  1745,  1746,  1747,  1749,  1750,  1751,
    1752,  1754,  1769,  1777,  1777,  1831,  1832,  1834,  1835,  1876,
    1879,  1882,  1889,  1892,  1903,  1911,  1915,  1922,  1926,  1936,
    1947,  2144,  2145,  2180,  2187,  2194,  2201,  2223,  2230
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
  "PACKAGE_INITCODE", "EMBED_START", "CONDITIONAL_COMPILATION",
  "MAYBEEXPRESSION", "VARIABLE_DECLARATION", "VARIABLE_LIST",
  "ONE_VARIABLE", "IF_CODEBLOCK", "$@1", "MAYBEELSE", "IF", "FOR_INIT",
  "FOR_IN_INIT", "FOR_START", "FOR", "FOR_IN", "WHILE", "DO_WHILE",
  "BREAK", "CONTINUE", "MAYBE_CASE_LIST", "CASE_LIST", "CASE", "DEFAULT",
  "SWITCH", "$@2", "CATCH", "$@3", "FINALLY", "$@4", "CATCH_LIST",
  "CATCH_FINALLY_LIST", "TRY", "$@5", "THROW", "WITH_HEAD", "WITH",
  "X_IDENTIFIER", "PACKAGE", "PACKAGE_DECLARATION", "$@6", "$@7", "IMPORT",
  "MAYBE_MODIFIERS", "MODIFIER_LIST", "MODIFIER", "EXTENDS",
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
  "MEMBER", "NOTHING", "VAR_READ", "NAMESPACE_ID", "NAMESPACE_DECLARATION",
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
     141,   141,   141,   141,   141,   142,   142,   143,   143,   144,
     144,   144,   144,   144,   144,   144,   144,   145,   145,   146,
     146,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   148,   148,   148,   148,
     148,   148,   148,   148,   149,   149,   150,   151,   152,   153,
     153,   154,   154,   155,   155,   156,   158,   157,   159,   159,
     160,   161,   161,   161,   162,   162,   163,   163,   164,   165,
     166,   167,   168,   168,   169,   169,   170,   170,   170,   170,
     171,   171,   172,   173,   175,   174,   177,   176,   179,   178,
     180,   180,   181,   181,   181,   183,   182,   184,   184,   185,
     186,   187,   187,   187,   187,   188,   188,   190,   189,   191,
     189,   192,   192,   192,   193,   193,   194,   194,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   196,   196,
     197,   197,   198,   198,   200,   199,   202,   201,   203,   203,
     204,   204,   205,   205,   205,   205,   205,   205,   206,   206,
     207,   207,   208,   208,   208,   209,   209,   211,   210,   212,
     212,   213,   214,   214,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   216,   217,   218,   219,   221,   220,   223,
     222,   224,   224,   224,   224,   225,   225,   226,   226,   227,
     227,   228,   228,   228,   229,   229,   230,   230,   230,   230,
     230,   231,   231,   231,   231,   232,   232,   233,   233,   234,
     234,   234,   236,   235,   237,   237,   239,   238,   240,   241,
     242,   242,   243,   243,   244,   244,   244,   245,   245,   246,
     246,   247,   247,   247,   248,   249,   248,   250,   251,   252,
     252,   253,   254,   254,   255,   256,   257,   257,   258,   258,
     259,   259,   260,   260,   260,   260,   260,   261,   261,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   263,   262,   264,   264,   265,   265,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     266,   267,   268,   269,   269,   269,   270,   271,   272
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
       3,     0,     2,     2,     4,     4,     2,     4,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     124,   137,   177,   277,   174,   175,   176,     0,     0,    66,
       0,     0,   130,   278,   128,   129,     0,   136,   351,   135,
     225,   181,   182,   310,   131,     0,     0,   351,   180,   132,
     134,   133,     0,     0,   179,   178,     0,   351,     0,   260,
     351,   351,    14,   184,   351,   351,   351,   351,   241,   124,
       0,     0,     2,   124,     5,    56,    12,     0,    38,    71,
      33,    34,    35,    36,    37,    40,   124,    39,     7,    32,
       0,   125,   126,     9,     8,    11,   275,   276,     0,   183,
      10,   269,   271,   273,   272,    41,   258,   270,     0,   274,
      43,    31,    42,     0,     0,    76,   351,     0,   124,    94,
     111,   113,   112,   114,   119,   116,     0,     0,   247,   224,
       0,   309,   351,   121,     0,   122,   308,   105,   241,     0,
       0,   251,   351,   262,   263,   264,   265,   266,     0,     0,
     261,   330,   329,   312,   292,   291,     0,   256,     0,   242,
     243,   244,   254,    84,     0,   108,   252,     0,    82,    46,
      45,   124,    47,    30,     0,    48,    49,    50,    52,     0,
      51,   332,     1,     6,   124,    75,     0,    72,     0,     0,
      73,    55,   110,     0,   221,     0,   166,     0,   165,   167,
     356,   137,   127,   197,   351,   198,   199,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   328,   327,     0,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     241,   351,     0,   352,    58,    77,     0,   255,     0,    67,
     351,   124,   117,     0,   228,     0,   231,   230,   358,   239,
     211,     0,     0,    28,     0,   331,   351,     0,   280,   351,
       0,   351,   311,   279,   245,   246,    85,   238,    62,    63,
     107,   253,    61,    83,    44,    29,   124,     0,   238,   351,
     351,    54,   353,   220,   219,     0,   138,   140,     0,   189,
       0,   206,     0,   200,   204,   259,   306,   307,   304,   305,
     285,   286,   288,   287,   283,   284,   320,   319,   315,   314,
     322,   323,   324,   321,   317,   316,   318,   289,   290,   339,
     111,   338,     0,   337,     0,   341,   298,   297,   296,   325,
       0,   295,   294,   293,   281,   282,   302,   301,   299,   303,
     300,     0,     0,   333,   351,     0,     0,   350,    66,   351,
       0,    26,    57,     0,   124,    17,    23,     0,    20,    19,
      22,    21,   124,   115,     0,   241,   248,   172,     0,     0,
     212,   216,   109,   123,   229,     0,    27,   250,   357,    66,
     267,     0,   257,     0,    60,     0,   124,    13,    60,     0,
       0,     0,     0,     0,   142,     0,     0,   238,   168,   169,
       0,     0,     0,   185,   186,   191,   205,   351,     0,   346,
       0,   351,   249,   313,   351,     0,   351,     0,   344,     0,
      80,     0,     0,   351,   120,    18,   124,     0,     0,   351,
       0,   218,   213,   238,     0,     0,    68,   351,   236,   235,
     234,   237,   351,    65,    64,    53,   351,    66,   354,   355,
     211,   139,     0,     0,   232,   141,   146,    60,     0,   190,
     210,   209,   207,   208,     0,     0,     0,     0,   338,   342,
     326,     0,   343,     0,     0,   340,    81,    86,     0,     0,
     118,   240,   173,   172,     0,     0,   215,     0,     0,   100,
     104,   102,   106,    66,    70,   268,    59,     0,    79,     0,
     143,   144,     0,   124,   171,   170,   201,   193,   184,   194,
     351,   192,     0,   191,   349,   347,   334,   348,   345,   351,
       0,     0,    87,    90,    88,    25,    24,   217,   226,   214,
      98,     0,   101,   103,    69,    66,   238,   124,   233,     0,
     162,     0,     0,   124,   160,     0,   187,   184,   191,   195,
       0,    28,    95,    91,    89,    28,    28,   238,    78,     0,
     152,    57,   157,     0,     0,     0,   124,   150,   154,   155,
     163,   221,   147,   161,   186,     0,     0,   196,    28,    93,
       0,     0,     0,   222,   351,   124,   145,   151,     0,     0,
     188,   186,    92,   227,    99,    96,    28,     0,     0,     0,
     202,     0,     0,     0,   156,   153,   211,   203,    28,   223,
       0,     0,   238,    97,   164
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    51,    52,    53,    54,   353,   354,   355,   375,   376,
     152,   153,   172,   356,   423,   154,   443,   155,   272,   269,
      97,    98,   494,    58,   168,   169,    59,    60,    61,    62,
      63,   156,   157,   521,   522,   523,   524,    64,   240,   489,
     602,   490,   556,   491,   492,    65,   253,   158,    66,    67,
     323,   245,    68,   362,   241,    69,   159,    71,    72,   394,
     396,   453,   358,   537,   359,   503,   565,   566,   567,   542,
     543,   544,   179,   360,   288,   398,   399,   431,    76,    77,
      78,   405,   464,   511,   575,   185,   400,   465,   512,   186,
     292,    79,   293,   294,   369,   370,   371,   285,   361,   596,
     110,    81,   555,   246,   247,   440,   455,   441,   384,   366,
     138,   139,   140,   249,    82,    83,    84,   160,   141,   236,
     237,    85,   128,   129,   130,    86,   414,   324,   325,    87,
      88,    89,   180,    90,    91,    92
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -499
static const yytype_int16 yypact[] =
{
     431,   -19,  -499,  -499,  -499,  -499,  -499,   -22,   -35,  -499,
     -17,    34,  -499,  -499,  -499,  -499,    81,  -499,  2103,  -499,
      75,  -499,  -499,  2156,  -499,   -15,   344,  2103,  -499,  -499,
    -499,  -499,   -11,    -3,  -499,  -499,   109,  2103,     6,   319,
    2103,  2103,  -499,  -499,  2103,  2103,  2103,  2103,  2022,   719,
     142,   151,  -499,   590,  -499,  -499,  -499,    30,  -499,  1969,
    -499,  -499,  -499,  -499,  -499,  -499,  1879,  -499,  -499,  -499,
     305,   366,  -499,  -499,  -499,  -499,  -499,  -499,    18,  -499,
    -499,  -499,  -499,  -499,  -499,    69,  2925,  -499,   199,  -499,
    -499,  -499,  -499,   218,   100,  -499,  2103,   222,  1879,  -499,
    -499,  -499,  -499,  -499,  -499,  -499,    13,   377,   -46,  -499,
     111,   708,  2103,   105,   125,  -499,   708,  -499,  2022,   236,
     136,   708,  2103,  -499,  -499,  -499,  -499,  -499,   121,   153,
     159,   708,   708,   436,   708,   708,   -33,  2862,   135,   163,
    2022,  -499,  2862,   269,   270,  2103,  2103,   270,   271,  -499,
    -499,   848,  -499,  -499,   147,  -499,  -499,  -499,  -499,   267,
    -499,  -499,  -499,  -499,  1106,  -499,   286,  -499,   190,   225,
      69,   193,  -499,   294,    86,   296,  -499,   298,  -499,  -499,
    -499,  -499,  -499,  -499,  2103,  -499,    20,  2103,  2103,  2103,
    2103,  2103,  2103,  2103,  2103,  2103,  2103,  2103,  2103,  2103,
    2103,  2103,  2103,  2103,  2103,  2103,  2103,  2103,  2103,  2103,
    2103,  2103,  -499,  -499,   141,  2103,  2103,  2103,  2103,  2103,
    2103,  2103,  2103,  2103,  2103,  2103,  2103,  2103,  2103,  2103,
    2022,  2103,    23,  -499,  -499,  -499,   166,   204,   186,  -499,
    2103,  1235,  -499,   377,   187,   189,  -499,  -499,  -499,   191,
      21,   188,    57,  1751,   195,  -499,  2103,   197,  -499,  2103,
     319,  2103,  -499,  -499,  -499,  -499,  -499,   219,   239,  -499,
    -499,  -499,   239,  -499,  -499,  -499,  1879,   210,   219,  2103,
    2103,  -499,   242,  -499,  -499,   346,   299,   317,   348,  2862,
     272,   274,    19,    20,  -499,  2862,  1098,   582,   582,   582,
    3303,  3303,  3303,  3303,  1098,  1098,  2862,  2862,  2862,  2862,
    2862,  2862,  2862,  2862,  2862,  2862,  2862,  2988,  3051,  -499,
    -499,   281,   248,  -499,   283,  -499,   700,   700,   700,  2862,
    2358,  3114,  3177,  3240,  1098,  1098,   436,   436,   708,   708,
     708,   243,  2421,  -499,  2103,   289,   285,  -499,  -499,  2103,
    2216,  -499,   254,   247,  1364,  -499,  -499,   263,  -499,  -499,
    -499,  -499,  1235,  -499,   377,  2022,  -499,    46,   394,   262,
     306,  -499,  -499,  -499,   280,   265,  1751,  -499,  2862,  -499,
    -499,   310,  2862,    83,   314,   270,   977,  -499,    -7,   318,
     284,   157,   297,   377,   408,   377,   295,   219,   327,  -499,
     302,    15,    42,  -499,  -499,  -499,  -499,  2103,   342,  -499,
     293,  2103,  -499,  -499,  2103,  2484,  2103,   343,  -499,   293,
    -499,   309,   328,  2103,  -499,  -499,  1235,   322,   324,  2103,
      83,  -499,  -499,   219,    28,    38,   396,  2103,  -499,  -499,
    -499,  -499,  2103,  -499,  -499,  -499,  2103,  -499,  -499,  -499,
      21,  -499,   377,   334,  -499,   361,  -499,   314,   348,  -499,
    -499,  -499,  -499,  -499,   352,    26,  2547,   293,  -499,  -499,
    2862,  2287,  -499,  2610,   293,  -499,  -499,    44,  2673,   330,
    -499,  -499,  2862,   363,   353,   394,  -499,   356,   350,  -499,
    -499,    38,  -499,  -499,  -499,  -499,  2862,   -28,  -499,   336,
     361,  -499,   377,   233,  -499,  -499,  -499,  -499,   354,  -499,
    2103,  -499,   365,  -499,  -499,  -499,  -499,  -499,  -499,  2103,
     374,   355,    44,  -499,  -499,  -499,  -499,  -499,  -499,  -499,
    -499,   484,  -499,  -499,  -499,  -499,   219,  1493,  -499,   485,
    -499,   464,   357,   266,  -499,    18,  2862,   375,  -499,   110,
    2736,  1751,  -499,  -499,  -499,  1751,  1751,   219,  -499,   369,
    -499,   254,  -499,   371,    50,   367,  1622,  -499,  -499,  -499,
    -499,    86,  -499,  -499,  -499,   370,    18,   110,  1751,  -499,
     373,   376,   364,  -499,  2103,  1493,  -499,  -499,   501,   397,
    -499,  -499,  -499,  -499,  -499,  -499,  1751,  2799,   378,   382,
    -499,   398,   384,   379,  -499,  -499,    21,  -499,  1751,  -499,
     380,   383,   219,  -499,  -499
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -499,  -499,   351,  -499,   466,  -322,  -499,   167,  -212,   -31,
       1,   -57,   419,    59,   -41,     2,    67,   471,   381,   146,
    -334,  -499,  -499,  -499,  -499,  -499,  -499,  -499,  -499,  -499,
    -499,  -499,  -499,  -499,  -499,    11,    12,  -499,  -499,    45,
    -499,    53,  -499,  -499,  -499,  -499,  -499,  -499,  -499,  -499,
      -6,    56,  -499,  -499,  -499,  -499,     0,  -499,   475,  -499,
    -499,  -499,    63,  -499,    66,  -499,   -50,  -499,   -18,  -499,
    -499,    31,  -499,     3,  -499,  -499,    89,    73,  -499,  -499,
    -499,  -499,  -498,  -499,  -499,  -170,  -499,  -461,  -499,  -497,
    -499,  -422,  -499,   278,  -433,  -499,  -346,   -13,     4,  -499,
    -499,  -499,  -499,  -499,   537,   -95,   112,   143,  -270,  -499,
    -105,  -499,  -499,  -499,  -499,  -499,  -499,  -499,  -130,   -54,
     528,   -52,  -499,   316,  -499,   -12,  -499,    16,  -190,  -499,
    -499,  -499,  -499,  -499,  -499,  -499
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -352
static const yytype_int16 yytable[] =
{
      70,    55,    57,    75,    80,   105,   108,   170,   388,   171,
     265,   111,   248,   254,   420,   116,   291,   499,   151,   460,
     105,   183,   432,   290,   367,   121,   320,    94,   131,   132,
     507,   367,   133,   134,   135,   137,   142,   100,   101,   102,
     427,   171,   347,   513,   211,   436,   462,   214,   574,   101,
     102,   103,   549,    70,    55,    57,    75,    80,   251,    56,
     100,   -74,   103,    73,   487,   173,    74,   106,   257,   261,
     488,    93,   101,   102,   187,   174,   589,   519,   109,   591,
     231,   176,   114,   232,   137,   103,   100,   577,   486,    96,
     548,   270,   271,   601,   275,   178,   107,   442,   101,   102,
     137,   244,    95,   262,   479,   520,   142,    99,   535,   112,
     137,   103,    56,   498,   507,   368,    73,   117,   438,    74,
     283,   118,   485,   291,   120,   341,   119,   457,   142,   380,
     122,   403,   409,   137,   137,   404,   284,   508,   509,   529,
     321,   242,   243,   184,   320,   161,   184,   343,   184,   344,
     429,   162,   430,   345,   510,   418,   101,   102,   164,   534,
     448,   449,   104,   484,    70,    55,    57,    75,    80,   103,
     184,   187,   289,   610,   373,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     439,   558,   233,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   142,   342,
     469,   234,   509,    56,   235,   389,   390,    73,   350,   475,
      74,   461,   463,   238,  -111,   250,   181,   363,   510,   255,
     256,    70,    55,   357,   378,   386,   374,   142,   346,   382,
      12,   320,    14,    15,   252,    17,   258,    19,   321,   259,
     428,   260,   263,   101,   102,   264,   559,   137,   137,   181,
      24,   322,   266,   267,   273,   276,   103,   515,   539,    29,
      30,    31,   173,    12,   518,    14,    15,   582,    17,   278,
      19,   279,   320,   280,   281,   421,   100,   282,   451,   286,
     454,   287,   348,    24,   101,   102,   261,   495,   101,   102,
     349,   539,    29,    30,    31,   365,  -116,   103,   364,   275,
     173,   103,   123,   124,   372,   383,   125,   126,   127,   275,
     174,   377,   415,   379,   540,   175,   176,   137,   408,   579,
     142,   385,   614,   580,   581,   387,   391,   113,   177,   392,
     178,   397,   393,   142,    70,    55,   357,   454,   374,   101,
     102,   417,    70,    55,   357,   321,   592,   540,  -158,   181,
     395,  -336,   103,   410,   407,   419,   401,   244,   402,   412,
     100,  -241,   424,    12,   603,    14,    15,   244,    17,   244,
      19,   426,   101,   102,   497,   466,   611,   367,   433,   470,
     435,  -159,   471,    24,   473,   103,   321,   538,   434,  -115,
     468,   478,    29,    30,    31,   416,   437,   482,   442,   446,
     447,   450,   452,   456,   244,   142,    70,    55,   357,   458,
     496,    -3,   467,   474,     1,     2,     3,   459,     4,     5,
       6,     7,     8,     9,    10,   476,   244,    11,    12,    13,
      14,    15,    16,    17,    18,    19,   477,   480,    21,    22,
     481,   493,   501,   502,   506,   526,    23,   429,    24,    25,
     545,    26,   536,    27,   531,    28,   547,    29,    30,    31,
     551,   528,    32,    33,   530,    34,    35,   557,   570,   571,
     552,   576,   572,    36,    37,    38,   244,   583,   546,   585,
     595,    39,   586,   541,   599,   590,   606,   550,   593,   600,
     607,   594,   608,   605,   609,   277,   612,   239,   613,   163,
     584,   425,    40,    41,   504,   268,   211,   212,   213,   214,
     167,   444,    42,   553,   554,   598,   532,   564,   562,   563,
     568,   569,    43,   541,   533,    44,   182,   505,   587,   142,
      45,    46,   227,   228,   229,    47,   527,    48,   588,    49,
     230,    50,   231,   115,   500,   232,   564,   562,   563,   568,
     569,   406,   597,   483,   573,   136,   381,     0,     0,     0,
       0,     0,     0,     0,     0,   564,   562,   563,   568,   569,
      -4,     0,     0,     1,     2,     3,     0,     4,     5,     6,
       7,     8,     9,    10,     0,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,     0,     0,    21,    22,     0,
       0,   188,     0,     0,     0,    23,     0,    24,    25,     0,
      26,     0,    27,     0,    28,     0,    29,    30,    31,     0,
       0,    32,    33,     0,    34,    35,     0,     0,     0,  -352,
    -352,  -352,    36,    37,    38,     0,     0,   196,   197,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   211,   212,   213,   214,     0,   215,   216,   217,
       0,    40,    41,     0,     0,     0,     0,     0,     0,     0,
       0,    42,     0,   223,   224,     0,   225,   226,   227,   228,
     229,    43,     0,     0,    44,     0,   230,     0,   231,    45,
      46,   232,     0,     0,    47,     0,    48,     0,    49,     0,
      50,     0,     1,     2,     3,    -4,     4,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    12,    13,    14,    15,
      16,    17,    18,    19,    20,     0,    21,    22,   143,     0,
     144,     0,     0,     0,    23,   145,    24,    25,     0,    26,
     146,    27,     0,    28,   147,    29,    30,    31,     0,     0,
      32,    33,     0,    34,    35,     0,     0,     0,     0,     0,
       0,    36,    37,    38,     0,   148,     0,     0,     0,    39,
     211,   212,   213,   214,     0,     0,     0,     0,   211,   212,
     213,   214,     0,     0,     0,     0,     0,     0,     0,     0,
      40,    41,     0,     0,   225,   226,   227,   228,   229,     0,
     149,     0,     0,     0,   230,     0,   231,     0,     0,   232,
      43,     0,   230,    44,   231,     0,     0,   232,    45,    46,
       0,     0,     0,    47,     0,    48,     0,    49,     0,    50,
       0,     1,     2,     3,   150,     4,     5,     6,     7,     8,
       9,    10,     0,     0,     0,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     0,    21,    22,   143,     0,   144,
       0,     0,     0,    23,   145,    24,    25,     0,    26,   146,
      27,     0,    28,   147,    29,    30,    31,     0,     0,    32,
      33,     0,    34,    35,     0,     0,     0,     0,     0,     0,
      36,    37,    38,     0,   148,     0,     0,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      41,     0,     0,     0,     0,     0,     0,     0,     0,   149,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43,
       0,     0,    44,     0,     0,     0,     0,    45,    46,     0,
       0,     0,    47,     0,    48,     0,    49,     0,    50,     0,
       1,     2,     3,   274,     4,     5,     6,     7,     8,     9,
      10,     0,     0,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,     0,    21,    22,   143,     0,   144,     0,
       0,     0,    23,   145,    24,    25,     0,    26,   146,    27,
       0,    28,   147,    29,    30,    31,     0,     0,    32,    33,
       0,    34,    35,     0,     0,     0,     0,     0,     0,    36,
      37,    38,     0,   148,     0,     0,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    40,    41,
       0,     0,     0,     0,     0,     0,     0,     0,   149,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
       0,    44,     0,     0,     0,     0,    45,    46,     0,     0,
       0,    47,     0,    48,     0,    49,     0,    50,     0,     1,
       2,     3,   445,     4,     5,     6,     7,     8,     9,    10,
       0,     0,    11,    12,    13,    14,    15,    16,    17,    18,
      19,     0,     0,    21,    22,     0,     0,  -352,     0,     0,
       0,    23,     0,    24,    25,     0,    26,     0,    27,     0,
      28,     0,    29,    30,    31,     0,     0,    32,    33,     0,
      34,    35,     0,     0,     0,     0,     0,     0,    36,    37,
      38,     0,     0,  -352,  -352,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   211,   212,
     213,   214,     0,   215,   216,   217,     0,    40,    41,     0,
       0,     0,     0,     0,     0,     0,     0,    42,     0,  -352,
    -352,     0,   225,   226,   227,   228,   229,    43,     0,     0,
      44,     0,   230,     0,   231,    45,    46,   232,     0,     0,
      47,     0,    48,     0,    49,     0,    50,     0,     1,     2,
       3,    -3,     4,     5,     6,     7,     8,     9,    10,     0,
       0,     0,    12,    13,    14,    15,    16,    17,    18,    19,
       0,     0,    21,    22,     0,     0,     0,     0,     0,     0,
      23,     0,    24,    25,     0,    26,     0,    27,     0,    28,
       0,    29,    30,    31,     0,     0,    32,    33,     0,    34,
      35,     0,     0,     0,     0,     0,     0,    36,    37,    38,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    40,    41,     0,     0,
       0,     0,     0,     0,     0,     0,   351,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,     0,     0,    44,
       0,     0,     0,     0,    45,    46,     0,     0,     0,    47,
       0,   352,     0,    49,     0,    50,     0,     1,     2,     3,
     -15,     4,     5,     6,     7,     8,     9,    10,     0,     0,
       0,    12,    13,    14,    15,    16,    17,    18,    19,     0,
       0,    21,    22,     0,     0,     0,     0,     0,     0,    23,
       0,    24,    25,     0,    26,     0,    27,     0,    28,     0,
      29,    30,    31,     0,     0,    32,    33,     0,    34,    35,
       0,     0,     0,     0,     0,     0,    36,    37,    38,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    40,    41,     0,     0,     0,
       0,     0,     0,     0,     0,   351,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,     0,     0,    44,     0,
       0,     0,     0,    45,    46,     0,     0,     0,    47,     0,
     352,     0,    49,     0,    50,     0,     1,     2,     3,   -16,
       4,     5,     6,     7,     8,     9,    10,     0,     0,     0,
      12,    13,    14,    15,    16,    17,    18,    19,     0,     0,
      21,    22,     0,     0,     0,     0,     0,     0,    23,     0,
      24,    25,     0,    26,     0,    27,     0,    28,     0,    29,
      30,    31,     0,     0,    32,    33,     0,    34,    35,     0,
       0,     0,     0,     0,     0,    36,    37,    38,     0,     0,
       0,     0,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    40,    41,     0,     0,     0,     0,
       0,     0,     0,     0,   560,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,     0,    44,     0,     0,
       0,     0,    45,    46,     0,     0,     0,    47,     0,   561,
       0,    49,     0,    50,     0,     1,     2,     3,  -148,     4,
       5,     6,     7,     8,     9,    10,     0,     0,     0,    12,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    21,
      22,     0,     0,     0,     0,     0,     0,    23,     0,    24,
      25,     0,    26,     0,    27,     0,    28,     0,    29,    30,
      31,     0,     0,    32,    33,     0,    34,    35,     0,     0,
       0,     0,     0,     0,    36,    37,    38,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,    41,     0,     0,     0,     0,     0,
       0,     0,     0,   560,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    43,     0,     0,    44,     0,     0,     0,
       0,    45,    46,     0,     0,     0,    47,     0,   561,     0,
      49,     0,    50,     0,     1,     2,     3,  -149,     4,     5,
       6,     7,     8,     9,    10,     0,  -124,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,     0,    21,    22,
     143,     0,   144,     0,     0,     0,    23,   145,    24,    25,
       0,    26,   146,    27,     0,    28,   147,    29,    30,    31,
       0,     0,    32,    33,     0,    34,    35,     0,     0,     0,
       0,     0,     0,    36,    37,    38,     0,   148,     0,     0,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    40,    41,     0,     0,     0,     0,     0,     0,
       0,     0,   149,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    43,     0,     0,    44,     0,     0,     0,     0,
      45,    46,     0,     0,     0,    47,     0,    48,     0,    49,
       0,    50,     1,     2,     3,     0,     4,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    12,    13,    14,    15,
      16,    17,    18,    19,    20,     0,    21,    22,   143,     0,
     144,     0,     0,     0,    23,   145,    24,    25,     0,    26,
     146,    27,     0,    28,   147,    29,    30,    31,     0,     0,
      32,    33,     0,    34,    35,     0,     0,     0,     0,     0,
       0,    36,    37,    38,     0,   148,     0,     0,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      40,    41,   165,     2,     3,     0,     4,     5,     6,     0,
     149,     0,     0,     0,     0,     0,     0,    13,     0,     0,
      43,     0,    18,    44,    20,     0,    21,    22,    45,    46,
     144,     0,     0,    47,    23,    48,     0,    49,     0,    50,
       0,    27,     0,    28,   166,     0,     0,     0,     0,     0,
       0,    33,     0,    34,    35,  -351,     2,     3,     0,     4,
       5,     6,    37,     0,     0,     0,     0,     0,     0,    39,
      13,     0,     0,     0,     0,    18,     0,    20,     0,    21,
      22,     0,     0,     0,     0,     0,     0,    23,     0,     0,
      40,    41,     0,     0,    27,     0,    28,     0,     0,     0,
       0,     0,     0,     0,    33,     0,    34,    35,     0,     0,
      43,     0,     0,    44,     0,    37,     0,     0,    45,    46,
       0,     0,    39,    47,     0,    48,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,     2,     3,     0,
       4,     5,     6,    40,    41,     0,     0,     0,     0,     0,
       0,    13,     0,     0,     0,     0,    18,     0,    20,     0,
      21,    22,     0,    43,     0,     0,    44,     0,    23,     0,
       0,    45,    46,     0,     0,    27,    47,    28,    48,     0,
       0,     0,    50,     0,     0,    33,     0,    34,    35,     0,
       2,     3,     0,     4,     5,     6,    37,     0,     0,     0,
       0,     0,     0,    39,    13,     0,     0,     0,     0,    18,
       0,    20,     0,    21,    22,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    40,    41,     0,     0,     0,     0,
      28,     0,     0,     0,     0,     0,     0,     0,    33,     0,
      34,    35,     0,     0,    43,     0,     0,    44,     0,     0,
       0,     0,    45,    46,     0,     0,    39,    47,     0,    48,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    40,    41,     0,
       0,     0,     0,     0,     0,   188,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,     0,    48,   189,   190,   191,    50,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
       0,   215,   216,   217,     0,     0,     0,     0,     0,     0,
     218,   219,     0,   220,   221,   222,   188,   223,   224,     0,
     225,   226,   227,   228,   229,     0,     0,     0,     0,     0,
     230,     0,   231,     0,     0,   232,     0,     0,     0,     0,
       0,     0,   422,     0,   189,   190,   191,     0,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,     0,   215,   216,   217,     0,     0,     0,     0,     0,
       0,   218,   219,     0,   220,   221,   222,   188,   223,   224,
       0,   225,   226,   227,   228,   229,     0,     0,     0,     0,
       0,   230,     0,   231,     0,     0,   232,     0,     0,     0,
       0,     0,     0,   516,     0,   189,   190,   191,     0,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,     0,   215,   216,   217,     0,     0,     0,     0,
     188,     0,   218,   219,   411,   220,   221,   222,     0,   223,
     224,     0,   225,   226,   227,   228,   229,     0,     0,     0,
       0,     0,   230,     0,   231,     0,     0,   232,   189,   190,
     191,     0,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,     0,   215,   216,   217,     0,
       0,     0,     0,   188,     0,   218,   219,     0,   220,   221,
     222,     0,   223,   224,     0,   225,   226,   227,   228,   229,
       0,     0,     0,     0,     0,   230,     0,   231,   413,     0,
     232,   189,   190,   191,     0,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,     0,   215,
     216,   217,     0,     0,     0,     0,   188,     0,   218,   219,
       0,   220,   221,   222,     0,   223,   224,     0,   225,   226,
     227,   228,   229,     0,     0,     0,     0,     0,   230,     0,
     231,   472,     0,   232,   189,   190,   191,     0,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,     0,   215,   216,   217,     0,     0,     0,     0,   188,
       0,   218,   219,     0,   220,   221,   222,     0,   223,   224,
       0,   225,   226,   227,   228,   229,     0,     0,     0,     0,
       0,   230,     0,   231,   514,     0,   232,   189,   190,   191,
       0,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,     0,   215,   216,   217,     0,     0,
       0,     0,   188,     0,   218,   219,     0,   220,   221,   222,
       0,   223,   224,     0,   225,   226,   227,   228,   229,     0,
       0,     0,     0,     0,   230,     0,   231,   517,     0,   232,
     189,   190,   191,     0,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,     0,   215,   216,
     217,     0,     0,     0,     0,   188,     0,   218,   219,     0,
     220,   221,   222,     0,   223,   224,     0,   225,   226,   227,
     228,   229,     0,     0,     0,     0,     0,   230,     0,   231,
     525,     0,   232,   189,   190,   191,     0,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
       0,   215,   216,   217,     0,     0,     0,     0,   188,     0,
     218,   219,   578,   220,   221,   222,     0,   223,   224,     0,
     225,   226,   227,   228,   229,     0,     0,     0,     0,     0,
     230,     0,   231,     0,     0,   232,   189,   190,   191,     0,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,     0,   215,   216,   217,     0,     0,     0,
       0,   188,     0,   218,   219,     0,   220,   221,   222,     0,
     223,   224,     0,   225,   226,   227,   228,   229,     0,     0,
       0,     0,     0,   230,     0,   231,   604,     0,   232,   189,
     190,   191,     0,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,     0,   215,   216,   217,
       0,     0,     0,     0,   188,     0,   218,   219,     0,   220,
     221,   222,     0,   223,   224,     0,   225,   226,   227,   228,
     229,     0,     0,     0,     0,     0,   230,     0,   231,     0,
       0,   232,   189,   190,   191,     0,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,     0,
     215,   216,   217,     0,     0,     0,     0,   188,     0,   218,
     219,     0,   220,   221,   222,     0,     0,   224,     0,   225,
     226,   227,   228,   229,     0,     0,     0,     0,     0,   230,
       0,   231,     0,     0,   232,   189,   190,   191,     0,   192,
     193,   194,   195,   196,   197,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   210,   211,   212,
     213,   214,     0,   215,   216,   217,     0,     0,     0,     0,
     188,     0,     0,     0,     0,   220,   221,   222,     0,   223,
     224,     0,   225,   226,   227,   228,   229,     0,     0,     0,
       0,     0,   230,     0,   231,     0,     0,   232,   189,   190,
     191,     0,   192,   193,   194,   195,   196,   197,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   211,   212,   213,   214,     0,   215,   216,   217,     0,
       0,     0,     0,   188,     0,     0,     0,     0,   220,   221,
     222,     0,   223,   224,     0,   225,   226,   227,   228,   229,
       0,     0,     0,     0,     0,   230,     0,   231,     0,     0,
     232,   189,   190,   191,     0,   192,   193,   194,   195,   196,
     197,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   211,   212,   213,   214,     0,   215,
     216,   217,     0,     0,     0,     0,   188,     0,     0,     0,
       0,     0,   221,   222,     0,   223,   224,     0,   225,   226,
     227,   228,   229,     0,     0,     0,     0,     0,   230,     0,
     231,     0,     0,   232,   189,   190,   191,     0,   192,   193,
     194,   195,   196,   197,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   211,   212,   213,
     214,     0,   215,   216,   217,     0,     0,     0,     0,   188,
       0,     0,     0,     0,     0,     0,   222,     0,   223,   224,
       0,   225,   226,   227,   228,   229,     0,     0,     0,     0,
       0,   230,     0,   231,     0,     0,   232,   189,   190,   191,
       0,   192,   193,   194,   195,   196,   197,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     211,   212,   213,   214,     0,   215,   216,   217,     0,     0,
       0,     0,   188,     0,     0,     0,     0,     0,     0,  -352,
       0,   223,   224,     0,   225,   226,   227,   228,   229,     0,
       0,     0,     0,     0,   230,     0,   231,     0,     0,   232,
     189,   190,   191,     0,  -352,  -352,  -352,  -352,   196,   197,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   211,   212,   213,   214,     0,   215,   216,
     217,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   223,   224,     0,   225,   226,   227,
     228,   229,     0,     0,     0,     0,     0,   230,     0,   231,
       0,     0,   232
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,     0,     0,    11,    18,    59,   278,    66,
     140,    23,   107,   118,   348,    27,   186,   450,    49,     4,
      26,     3,   368,     3,     3,    37,     3,    49,    40,    41,
       4,     3,    44,    45,    46,    47,    48,     3,    15,    16,
     362,    98,   232,   465,    90,   379,     4,    93,   545,    15,
      16,    28,   513,    53,    53,    53,    53,    53,   112,     0,
       3,    68,    28,     0,    26,    15,     0,    11,   122,   102,
      32,    90,    15,    16,   102,    25,   574,    33,     3,   576,
     126,    31,    26,   129,    96,    28,     3,   548,   434,   124,
     512,   145,   146,   591,   151,    45,    15,   104,    15,    16,
     112,   107,   124,   136,   426,    61,   118,   124,   136,   124,
     122,    28,    53,   447,     4,    94,    53,   128,    35,    53,
      34,   124,    94,   293,    15,   230,   129,   397,   140,   259,
     124,   112,   322,   145,   146,   116,    50,   111,   112,   485,
     117,   128,   129,   128,     3,     3,   128,   124,   128,   126,
     104,     0,   106,   130,   128,   345,    15,    16,   128,   493,
       3,     4,   128,   433,   164,   164,   164,   164,   164,    28,
     128,   102,   184,   606,   117,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     117,   535,     3,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     410,     3,   112,   164,   124,   279,   280,   164,   240,   419,
     164,   401,   402,    11,   129,   124,     3,   243,   128,     3,
     104,   241,   241,   241,   256,   276,   252,   259,   232,   261,
      17,     3,    19,    20,   129,    22,   135,    24,   117,   106,
     365,   102,   127,    15,    16,   102,   536,   279,   280,     3,
      37,   130,     3,     3,     3,   128,    28,   467,    45,    46,
      47,    48,    15,    17,   474,    19,    20,   557,    22,     3,
      24,   101,     3,    68,   101,   349,     3,     3,   393,     3,
     395,     3,   136,    37,    15,    16,   102,   437,    15,    16,
     124,    45,    46,    47,    48,   124,   129,    28,   129,   376,
      15,    28,     3,     4,   136,   106,     7,     8,     9,   386,
      25,   136,   344,   136,   101,    30,    31,   349,   322,   551,
     352,   102,   612,   555,   556,   135,   104,     3,    43,     3,
      45,     3,    53,   365,   354,   354,   354,   452,   364,    15,
      16,   345,   362,   362,   362,   117,   578,   101,   135,     3,
      53,    90,    28,    90,   126,    90,   104,   383,   104,   136,
       3,   127,   135,    17,   596,    19,    20,   393,    22,   395,
      24,   128,    15,    16,   446,   407,   608,     3,   136,   411,
     135,   135,   414,    37,   416,    28,   117,   502,   102,   129,
     117,   423,    46,    47,    48,   126,   106,   429,   104,   101,
     136,   124,    14,   128,   430,   437,   426,   426,   426,   102,
     442,     0,    90,    90,     3,     4,     5,   135,     7,     8,
       9,    10,    11,    12,    13,   136,   452,    16,    17,    18,
      19,    20,    21,    22,    23,    24,   128,   135,    27,    28,
     136,    65,   128,   102,   112,   135,    35,   104,    37,    38,
     116,    40,   136,    42,   124,    44,   111,    46,    47,    48,
     106,   128,    51,    52,   128,    54,    55,     3,     3,    25,
     135,   116,   135,    62,    63,    64,   502,   128,   510,   128,
     136,    70,   135,   503,     3,   135,   124,   519,   135,   112,
     112,   135,   128,   135,   135,   164,   136,    98,   135,    53,
     561,   354,    91,    92,   457,   144,    90,    91,    92,    93,
      59,   385,   101,   522,   522,   585,   491,   537,   537,   537,
     537,   537,   111,   543,   491,   114,    71,   458,   566,   561,
     119,   120,   116,   117,   118,   124,   483,   126,   571,   128,
     124,   130,   126,    26,   452,   129,   566,   566,   566,   566,
     566,   293,   584,   430,   543,    47,   260,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   585,   585,   585,   585,   585,
       0,    -1,    -1,     3,     4,     5,    -1,     7,     8,     9,
      10,    11,    12,    13,    -1,    -1,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    27,    28,    -1,
      -1,    39,    -1,    -1,    -1,    35,    -1,    37,    38,    -1,
      40,    -1,    42,    -1,    44,    -1,    46,    47,    48,    -1,
      -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,    67,
      68,    69,    62,    63,    64,    -1,    -1,    75,    76,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    91,    92,    93,    -1,    95,    96,    97,
      -1,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,    -1,   111,   112,    -1,   114,   115,   116,   117,
     118,   111,    -1,    -1,   114,    -1,   124,    -1,   126,   119,
     120,   129,    -1,    -1,   124,    -1,   126,    -1,   128,    -1,
     130,    -1,     3,     4,     5,   135,     7,     8,     9,    10,
      11,    12,    13,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    29,    -1,
      31,    -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,
      41,    42,    -1,    44,    45,    46,    47,    48,    -1,    -1,
      51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    -1,    -1,    -1,    70,
      90,    91,    92,    93,    -1,    -1,    -1,    -1,    90,    91,
      92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    92,    -1,    -1,   114,   115,   116,   117,   118,    -1,
     101,    -1,    -1,    -1,   124,    -1,   126,    -1,    -1,   129,
     111,    -1,   124,   114,   126,    -1,    -1,   129,   119,   120,
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
      23,    24,    25,    -1,    27,    28,    29,    -1,    31,    -1,
      -1,    -1,    35,    36,    37,    38,    -1,    40,    41,    42,
      -1,    44,    45,    46,    47,    48,    -1,    -1,    51,    52,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    -1,    66,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,   119,   120,    -1,    -1,
      -1,   124,    -1,   126,    -1,   128,    -1,   130,    -1,     3,
       4,     5,   135,     7,     8,     9,    10,    11,    12,    13,
      -1,    -1,    16,    17,    18,    19,    20,    21,    22,    23,
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
     126,    -1,   128,    -1,   130,    -1,     3,     4,     5,   135,
       7,     8,     9,    10,    11,    12,    13,    -1,    -1,    -1,
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
      -1,   128,    -1,   130,    -1,     3,     4,     5,   135,     7,
       8,     9,    10,    11,    12,    13,    -1,    -1,    -1,    17,
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
     128,    -1,   130,    -1,     3,     4,     5,   135,     7,     8,
       9,    10,    11,    12,    13,    -1,    15,    -1,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    27,    28,
      29,    -1,    31,    -1,    -1,    -1,    35,    36,    37,    38,
      -1,    40,    41,    42,    -1,    44,    45,    46,    47,    48,
      -1,    -1,    51,    52,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    -1,    66,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
     119,   120,    -1,    -1,    -1,   124,    -1,   126,    -1,   128,
      -1,   130,     3,     4,     5,    -1,     7,     8,     9,    10,
      11,    12,    13,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    27,    28,    29,    -1,
      31,    -1,    -1,    -1,    35,    36,    37,    38,    -1,    40,
      41,    42,    -1,    44,    45,    46,    47,    48,    -1,    -1,
      51,    52,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    -1,    66,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    92,     3,     4,     5,    -1,     7,     8,     9,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    18,    -1,    -1,
     111,    -1,    23,   114,    25,    -1,    27,    28,   119,   120,
      31,    -1,    -1,   124,    35,   126,    -1,   128,    -1,   130,
      -1,    42,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    54,    55,     3,     4,     5,    -1,     7,
       8,     9,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      18,    -1,    -1,    -1,    -1,    23,    -1,    25,    -1,    27,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    -1,
      91,    92,    -1,    -1,    42,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    52,    -1,    54,    55,    -1,    -1,
     111,    -1,    -1,   114,    -1,    63,    -1,    -1,   119,   120,
      -1,    -1,    70,   124,    -1,   126,    -1,    -1,    -1,   130,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,     5,    -1,
       7,     8,     9,    91,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    25,    -1,
      27,    28,    -1,   111,    -1,    -1,   114,    -1,    35,    -1,
      -1,   119,   120,    -1,    -1,    42,   124,    44,   126,    -1,
      -1,    -1,   130,    -1,    -1,    52,    -1,    54,    55,    -1,
       4,     5,    -1,     7,     8,     9,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    18,    -1,    -1,    -1,    -1,    23,
      -1,    25,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    92,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,
      54,    55,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,    -1,   119,   120,    -1,    -1,    70,   124,    -1,   126,
      -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,    67,    68,    69,   130,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   105,    -1,   107,   108,   109,    39,   111,   112,    -1,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,   136,    -1,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    -1,    95,    96,    97,    -1,    -1,    -1,    -1,    -1,
      -1,   104,   105,    -1,   107,   108,   109,    39,   111,   112,
      -1,   114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,   136,    -1,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    -1,    -1,    -1,
      39,    -1,   104,   105,   106,   107,   108,   109,    -1,   111,
     112,    -1,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,    -1,   129,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    -1,    95,    96,    97,    -1,
      -1,    -1,    -1,    39,    -1,   104,   105,    -1,   107,   108,
     109,    -1,   111,   112,    -1,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,   127,    -1,
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
      -1,   124,    -1,   126,   127,    -1,   129,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    -1,    95,    96,    97,    -1,    -1,
      -1,    -1,    39,    -1,   104,   105,    -1,   107,   108,   109,
      -1,   111,   112,    -1,   114,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,   127,    -1,   129,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    -1,    95,    96,
      97,    -1,    -1,    -1,    -1,    39,    -1,   104,   105,    -1,
     107,   108,   109,    -1,   111,   112,    -1,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
     127,    -1,   129,    67,    68,    69,    -1,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      -1,    95,    96,    97,    -1,    -1,    -1,    -1,    39,    -1,
     104,   105,   106,   107,   108,   109,    -1,   111,   112,    -1,
     114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,
     124,    -1,   126,    -1,    -1,   129,    67,    68,    69,    -1,
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
     118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,
      -1,   129,    67,    68,    69,    -1,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    -1,
      95,    96,    97,    -1,    -1,    -1,    -1,    39,    -1,   104,
     105,    -1,   107,   108,   109,    -1,    -1,   112,    -1,   114,
     115,   116,   117,   118,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,    -1,   129,    67,    68,    69,    -1,    71,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    -1,    95,    96,    97,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,   107,   108,   109,    -1,   111,
     112,    -1,   114,   115,   116,   117,   118,    -1,    -1,    -1,
      -1,    -1,   124,    -1,   126,    -1,    -1,   129,    67,    68,
      69,    -1,    71,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    91,    92,    93,    -1,    95,    96,    97,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,   107,   108,
     109,    -1,   111,   112,    -1,   114,   115,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,    -1,
     129,    67,    68,    69,    -1,    71,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    91,    92,    93,    -1,    95,
      96,    97,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,   108,   109,    -1,   111,   112,    -1,   114,   115,
     116,   117,   118,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,    -1,   129,    67,    68,    69,    -1,    71,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,
      93,    -1,    95,    96,    97,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,   111,   112,
      -1,   114,   115,   116,   117,   118,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,    -1,    -1,   129,    67,    68,    69,
      -1,    71,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    91,    92,    93,    -1,    95,    96,    97,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,   111,   112,    -1,   114,   115,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,   124,    -1,   126,    -1,    -1,   129,
      67,    68,    69,    -1,    71,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    91,    92,    93,    -1,    95,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   111,   112,    -1,   114,   115,   116,
     117,   118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,    -1,   129
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
     130,   138,   139,   140,   141,   147,   150,   152,   160,   163,
     164,   165,   166,   167,   174,   182,   185,   186,   189,   192,
     193,   194,   195,   199,   201,   210,   215,   216,   217,   228,
     235,   238,   251,   252,   253,   258,   262,   266,   267,   268,
     270,   271,   272,    90,    49,   124,   124,   157,   158,   124,
       3,    15,    16,    28,   128,   187,   188,    15,   262,     3,
     237,   262,   124,     3,   188,   241,   262,   128,   124,   129,
      15,   262,   124,     3,     4,     7,     8,     9,   259,   260,
     261,   262,   262,   262,   262,   262,   257,   262,   247,   248,
     249,   255,   262,    29,    31,    36,    41,    45,    66,   101,
     135,   146,   147,   148,   152,   154,   168,   169,   184,   193,
     254,     3,     0,   141,   128,     3,    45,   154,   161,   162,
     258,   148,   149,    15,    25,    30,    31,    43,    45,   209,
     269,     3,   195,     3,   128,   222,   226,   102,    39,    67,
      68,    69,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    95,    96,    97,   104,   105,
     107,   108,   109,   111,   112,   114,   115,   116,   117,   118,
     124,   126,   129,     3,     3,   124,   256,   257,    11,   149,
     175,   191,   128,   129,   187,   188,   240,   241,   242,   250,
     124,   256,   129,   183,   247,     3,   104,   256,   135,   106,
     102,   102,   136,   127,   102,   255,     3,     3,   155,   156,
     256,   256,   155,     3,   135,   148,   128,   139,     3,   101,
      68,   101,     3,    34,    50,   234,     3,     3,   211,   262,
       3,   222,   227,   229,   230,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
       3,   117,   130,   187,   264,   265,   262,   262,   262,   262,
     262,   262,   262,   262,   262,   262,   262,   262,   262,   262,
     262,   247,   262,   124,   126,   130,   264,   265,   136,   124,
     262,   101,   126,   142,   143,   144,   150,   152,   199,   201,
     210,   235,   190,   187,   129,   124,   246,     3,    94,   231,
     232,   233,   136,   117,   187,   145,   146,   136,   262,   136,
     255,   260,   262,   106,   245,   102,   146,   135,   245,   256,
     256,   104,     3,    53,   196,    53,   197,     3,   212,   213,
     223,   104,   104,   112,   116,   218,   230,   126,   264,   265,
      90,   106,   136,   127,   263,   262,   126,   264,   265,    90,
     157,   256,   136,   151,   135,   144,   128,   142,   247,   104,
     106,   214,   233,   136,   102,   135,   157,   106,    35,   117,
     242,   244,   104,   153,   156,   135,   101,   136,     3,     4,
     124,   242,    14,   198,   242,   243,   128,   245,   102,   135,
       4,   222,     4,   222,   219,   224,   262,    90,   117,   265,
     262,   262,   127,   262,    90,   265,   136,   128,   262,   142,
     135,   136,   262,   244,   245,    94,   233,    26,    32,   176,
     178,   180,   181,    65,   159,   255,   262,   258,   157,   231,
     243,   128,   102,   202,   153,   213,   112,     4,   111,   112,
     128,   220,   225,   228,   127,   265,   136,   127,   265,    33,
      61,   170,   171,   172,   173,   127,   135,   214,   128,   233,
     128,   124,   176,   178,   157,   136,   136,   200,   242,    45,
     101,   193,   206,   207,   208,   116,   262,   111,   228,   224,
     262,   106,   135,   172,   173,   239,   179,     3,   157,   245,
     101,   126,   147,   152,   193,   203,   204,   205,   210,   235,
       3,    25,   135,   208,   226,   221,   116,   224,   106,   145,
     145,   145,   245,   128,   151,   128,   135,   205,   234,   219,
     135,   226,   145,   135,   135,   136,   236,   262,   203,     3,
     112,   219,   177,   145,   127,   135,   124,   112,   128,   135,
     231,   145,   136,   135,   245
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
#line 304 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 24:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 316 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 25:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 317 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(2) - (4)].number_int);PASS1 as3_warning("embed command ignored");}
    }
    break;


  
    case 27:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 320 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 28:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 321 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 29:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 323 "parser.y"
    {
    (yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));
}
    }
    break;


  
    case 30:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 326 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 44:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 342 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (3)].code);}
    }
    break;


  
    case 45:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 343 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 46:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 346 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 53:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 353 "parser.y"
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
#line 365 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (2)].code);}
    }
    break;


  
    case 55:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 366 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 56:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 370 "parser.y"
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
#line 381 "parser.y"
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
#line 389 "parser.y"
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
#line 416 "parser.y"
    {(yyval.node)=(yyvsp[(2) - (2)].node);}
    }
    break;


  
    case 60:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 417 "parser.y"
    {(yyval.node)=mkdummynode();}
    }
    break;


  
    case 61:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 419 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 62:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 420 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 63:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 422 "parser.y"
    {(yyval.code) = (yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 64:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 423 "parser.y"
    {(yyval.code) = code_append((yyvsp[(1) - (3)].code), (yyvsp[(3) - (3)].code));}
    }
    break;


  
    case 65:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 426 "parser.y"
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
#line 486 "parser.y"
    {PASS12 new_state();}
    }
    break;


  
    case 67:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 486 "parser.y"
    {
    (yyval.code) = var_block((yyvsp[(2) - (2)].code), state->vars);
    PASS12 old_state();
}
    }
    break;


  
    case 68:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 490 "parser.y"
    {(yyval.code) = code_new();}
    }
    break;


  
    case 69:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 491 "parser.y"
    {(yyval.code)=(yyvsp[(2) - (2)].code);}
    }
    break;


  
    case 70:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 494 "parser.y"
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
#line 510 "parser.y"
    {(yyval.code)=code_new();}
    }
    break;


  
    case 74:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 517 "parser.y"
    {
    PASS1 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable(state->method, (yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1,0);
    PASS2 (yyval.id)=(yyvsp[(2) - (3)].id);new_variable(state->method, (yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].classinfo),1,0);
}
    }
    break;


  
    case 75:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 521 "parser.y"
    {
    PASS12
    (yyval.id)=(yyvsp[(1) - (1)].id);
}
    }
    break;


  
    case 76:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 526 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (2)].id);(yyval.for_start).each=0;}
    }
    break;


  
    case 77:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 527 "parser.y"
    {PASS12 new_state();(yyval.for_start).name=(yyvsp[(1) - (3)].id);(yyval.for_start).each=1;}
    }
    break;


  
    case 78:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 529 "parser.y"
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
#line 549 "parser.y"
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
#line 593 "parser.y"
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
#line 609 "parser.y"
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
#line 621 "parser.y"
    {
    (yyval.code) = abc___break__(0, "");
}
    }
    break;


  
    case 83:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 624 "parser.y"
    {
    (yyval.code) = abc___break__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 84:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 627 "parser.y"
    {
    (yyval.code) = abc___continue__(0, "");
}
    }
    break;


  
    case 85:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 630 "parser.y"
    {
    (yyval.code) = abc___continue__(0, (yyvsp[(2) - (2)].id));
}
    }
    break;


  
    case 86:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 634 "parser.y"
    {(yyval.code)=0;}
    }
    break;


  
    case 87:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 635 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 88:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 636 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 89:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 637 "parser.y"
    {(yyval.code)=code_append((yyvsp[(1) - (2)].code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 90:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 638 "parser.y"
    {(yyval.code)=(yyvsp[(1) - (1)].code);}
    }
    break;


  
    case 91:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 639 "parser.y"
    {(yyval.code)=code_append((yyval.code),(yyvsp[(2) - (2)].code));}
    }
    break;


  
    case 92:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 641 "parser.y"
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
#line 652 "parser.y"
    {
    (yyval.code) = (yyvsp[(3) - (3)].code);
}
    }
    break;


  
    case 94:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 655 "parser.y"
    {PASS12 new_state();state->switch_var=alloc_local();}
    }
    break;


  
    case 95:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 655 "parser.y"
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
#line 686 "parser.y"
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
#line 691 "parser.y"
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
#line 711 "parser.y"
    {PASS12 new_state();state->exception_name=0;}
    }
    break;


  
    case 99:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 711 "parser.y"
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
#line 727 "parser.y"
    {(yyval.catch_list).l=list_new();(yyval.catch_list).finally=0;list_append((yyval.catch_list).l,(yyvsp[(1) - (1)].exception));}
    }
    break;


  
    case 101:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 728 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (2)].catch_list);list_append((yyval.catch_list).l,(yyvsp[(2) - (2)].exception));}
    }
    break;


  
    case 102:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 729 "parser.y"
    {(yyval.catch_list)=(yyvsp[(1) - (1)].catch_list);}
    }
    break;


  
    case 103:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 730 "parser.y"
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
#line 738 "parser.y"
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
#line 747 "parser.y"
    {PASS12 new_state();
                 state->method->has_exceptions=1;
                 state->method->late_binding=1;//for invariant scope_code
                }
    }
    break;


  
    case 106:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 750 "parser.y"
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
#line 796 "parser.y"
    {
    (yyval.code)=(yyvsp[(2) - (2)].value).c;
    (yyval.code)=abc_throw((yyval.code));
}
    }
    break;


  
    case 108:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 801 "parser.y"
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
#line 812 "parser.y"
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
#line 822 "parser.y"
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
#line 840 "parser.y"
    {PASS12 (yyval.id)="package";}
    }
    break;


  
    case 113:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 841 "parser.y"
    {PASS12 (yyval.id)="namespace";}
    }
    break;


  
    case 114:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 842 "parser.y"
    {PASS12 (yyval.id)="NaN";}
    }
    break;


  
    case 115:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 844 "parser.y"
    {PASS12 (yyval.id) = concat3((yyvsp[(1) - (3)].id),".",(yyvsp[(3) - (3)].id));free((yyvsp[(1) - (3)].id));(yyvsp[(1) - (3)].id)=0;}
    }
    break;


  
    case 116:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 845 "parser.y"
    {PASS12 (yyval.id)=strdup((yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 117:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 847 "parser.y"
    {PASS12 startpackage((yyvsp[(2) - (3)].id));free((yyvsp[(2) - (3)].id));(yyvsp[(2) - (3)].id)=0;}
    }
    break;


  
    case 118:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 848 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 119:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 849 "parser.y"
    {PASS12 startpackage("");}
    }
    break;


  
    case 120:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 850 "parser.y"
    {PASS12 endpackage();(yyval.code)=0;}
    }
    break;


  
    case 121:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 873 "parser.y"
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
#line 881 "parser.y"
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
#line 895 "parser.y"
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
#line 911 "parser.y"
    {PASS12 (yyval.flags).flags=0;(yyval.flags).ns=0;}
    }
    break;


  
    case 125:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 912 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 126:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 913 "parser.y"
    {PASS12 (yyval.flags)=(yyvsp[(1) - (1)].flags);}
    }
    break;


  
    case 127:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 914 "parser.y"
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
#line 921 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PUBLIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 129:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 922 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PRIVATE;(yyval.flags).ns=0;}
    }
    break;


  
    case 130:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 923 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PROTECTED;(yyval.flags).ns=0;}
    }
    break;


  
    case 131:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 924 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_STATIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 132:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 925 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_DYNAMIC;(yyval.flags).ns=0;}
    }
    break;


  
    case 133:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 926 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_FINAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 134:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 927 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_OVERRIDE;(yyval.flags).ns=0;}
    }
    break;


  
    case 135:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 928 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NATIVE;(yyval.flags).ns=0;}
    }
    break;


  
    case 136:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 929 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_PACKAGEINTERNAL;(yyval.flags).ns=0;}
    }
    break;


  
    case 137:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 930 "parser.y"
    {PASS12 (yyval.flags).flags=FLAG_NAMESPACE;
                               (yyval.flags).ns=(yyvsp[(1) - (1)].id);
                       }
    }
    break;


  
    case 138:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 934 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 139:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 935 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 140:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 937 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 141:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 938 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 142:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 940 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();}
    }
    break;


  
    case 143:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 941 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(2) - (2)].classinfo_list);}
    }
    break;


  
    case 144:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 945 "parser.y"
    {PASS12 startclass(&(yyvsp[(1) - (6)].flags),(yyvsp[(3) - (6)].id),(yyvsp[(4) - (6)].classinfo),(yyvsp[(5) - (6)].classinfo_list));}
    }
    break;


  
    case 145:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 947 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 146:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 951 "parser.y"
    {PASS12 (yyvsp[(1) - (5)].flags).flags|=FLAG_INTERFACE;
                                          startclass(&(yyvsp[(1) - (5)].flags),(yyvsp[(3) - (5)].id),0,(yyvsp[(4) - (5)].classinfo_list));}
    }
    break;


  
    case 147:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 954 "parser.y"
    {PASS12 endclass();(yyval.code)=0;}
    }
    break;


  
    case 153:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 963 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(1) - (4)].number_int);}
    }
    break;


  
    case 156:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 966 "parser.y"
    {PASS_ALWAYS as3_pass=(yyvsp[(2) - (4)].number_int);PASS1 as3_warning("embed command ignored");}
    }
    break;


  
    case 157:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 968 "parser.y"
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
#line 979 "parser.y"
    {
    syntaxerror("variable declarations not allowed in interfaces");
}
    }
    break;


  
    case 164:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 982 "parser.y"
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
#line 1057 "parser.y"
    {PASS12 setslotstate(&(yyvsp[(1) - (2)].flags),(yyvsp[(2) - (2)].token));}
    }
    break;


  
    case 168:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1057 "parser.y"
    {PASS12 (yyval.code)=(yyvsp[(4) - (4)].code);setslotstate(0, 0);}
    }
    break;


  
    case 169:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1059 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 170:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1060 "parser.y"
    {PASS12 (yyval.code)=0;}
    }
    break;


  
    case 171:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1063 "parser.y"
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
#line 1146 "parser.y"
    {(yyval.constant)=0;}
    }
    break;


  
    case 173:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1147 "parser.y"
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
#line 1155 "parser.y"
    {(yyval.constant) = constant_new_int((yyvsp[(1) - (1)].number_int));}
    }
    break;


  
    case 175:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1156 "parser.y"
    {
    (yyval.constant) = constant_new_uint((yyvsp[(1) - (1)].number_uint));
}
    }
    break;


  
    case 176:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1159 "parser.y"
    {(yyval.constant) = constant_new_float((yyvsp[(1) - (1)].number_float));}
    }
    break;


  
    case 177:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1160 "parser.y"
    {(yyval.constant) = constant_new_string2((yyvsp[(1) - (1)].str).str,(yyvsp[(1) - (1)].str).len);free((char*)(yyvsp[(1) - (1)].str).str);}
    }
    break;


  
    case 178:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1161 "parser.y"
    {(yyval.constant) = constant_new_true((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 179:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1162 "parser.y"
    {(yyval.constant) = constant_new_false((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 180:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1163 "parser.y"
    {(yyval.constant) = constant_new_null((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 181:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1164 "parser.y"
    {(yyval.constant) = constant_new_undefined((yyvsp[(1) - (1)].token));}
    }
    break;


  
    case 182:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1165 "parser.y"
    {(yyval.constant) = constant_new_float(__builtin_nan(""));}
    }
    break;


  
    case 183:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1173 "parser.y"
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
#line 1184 "parser.y"
    {PASS_ALWAYS if(!xml_level++) tokenizer_begin_xml();}
    }
    break;


  
    case 185:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1185 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xmltext();}
    }
    break;


  
    case 186:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1186 "parser.y"
    {PASS_ALWAYS if(!--xml_level) tokenizer_end_xml(); else tokenizer_begin_xmltext();}
    }
    break;


  
    case 187:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1188 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xmltext();}
    }
    break;


  
    case 188:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1188 "parser.y"
    {
    (yyval.node) = (yyvsp[(2) - (4)].node);
}
    }
    break;


  
    case 189:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1191 "parser.y"
    {PASS_ALWAYS tokenizer_begin_xml();}
    }
    break;


  
    case 190:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1191 "parser.y"
    {
    (yyval.node) = (yyvsp[(2) - (4)].node);
}
    }
    break;


  
    case 191:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1194 "parser.y"
    {(yyval.node)=mkstringnode("");}
    }
    break;


  
    case 192:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1195 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node),(yyvsp[(2) - (2)].node));
}
    }
    break;


  
    case 193:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1198 "parser.y"
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
#line 1203 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node), mkstringnode(">"));
}
    }
    break;


  
    case 195:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1206 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node),(yyvsp[(2) - (2)].node));
}
    }
    break;


  
    case 196:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1209 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (3)].node), mkaddnode((yyvsp[(2) - (3)].node),(yyvsp[(3) - (3)].node)));
}
    }
    break;


  
    case 197:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1212 "parser.y"
    {
    (yyval.node) = mkstringnode((yyvsp[(1) - (1)].id));
}
    }
    break;


  
    case 198:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1215 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 199:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1219 "parser.y"
    {
    (yyval.node) = mkstringnode("");
}
    }
    break;


  
    case 200:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1222 "parser.y"
    {
    (yyval.node) = mkaddnode(mkstringnode(" "),(yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 201:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1226 "parser.y"
    {
    //$$ = allocprintf("<%s%s/>", $2, $3, $5, $8);
    (yyval.node) = mkaddnode(mkaddnode(mkaddnode(mkstringnode("<"),(yyvsp[(2) - (6)].node)),(yyvsp[(3) - (6)].node)),mkstringnode("/>"));
}
    }
    break;


  
    case 202:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1230 "parser.y"
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
#line 1235 "parser.y"
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
#line 1241 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 205:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1244 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (2)].node), mkaddnode(mkstringnode(" "),(yyvsp[(2) - (2)].node)));
}
    }
    break;


  
    case 206:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1247 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 207:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1250 "parser.y"
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
#line 1255 "parser.y"
    {
    (yyval.node) = mkaddnode((yyvsp[(1) - (3)].node), mkaddnode(mkstringnode("=\""), mkaddnode((yyvsp[(3) - (3)].node), mkstringnode("\""))));
}
    }
    break;


  
    case 209:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1258 "parser.y"
    {
    (yyval.node) = mkaddnode(mkaddnode(mkstringnode(concat2((yyvsp[(1) - (3)].id),"=\"")), (yyvsp[(3) - (3)].node)), mkstringnode("\""));
}
    }
    break;


  
    case 210:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1261 "parser.y"
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
#line 1271 "parser.y"
    {
    PASS12
    memset(&(yyval.params),0,sizeof((yyval.params)));
}
    }
    break;


  
    case 212:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1275 "parser.y"
    {
    PASS12
    (yyval.params)=(yyvsp[(1) - (1)].params);
}
    }
    break;


  
    case 213:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1281 "parser.y"
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
#line 1287 "parser.y"
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
#line 1295 "parser.y"
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
#line 1300 "parser.y"
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
#line 1306 "parser.y"
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
#line 1314 "parser.y"
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
#line 1324 "parser.y"
    {PASS12 (yyval.token)=0;}
    }
    break;


  
    case 222:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1327 "parser.y"
    {PASS12 startfunction(&(yyvsp[(1) - (9)].flags),(yyvsp[(3) - (9)].token),(yyvsp[(4) - (9)].id),&(yyvsp[(6) - (9)].params),(yyvsp[(8) - (9)].classinfo));}
    }
    break;


  
    case 223:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1328 "parser.y"
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
#line 1344 "parser.y"
    {PASS12 (yyval.id)=0;}
    }
    break;


  
    case 226:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1346 "parser.y"
    {PASS12 innerfunction((yyvsp[(2) - (7)].id),&(yyvsp[(4) - (7)].params),(yyvsp[(6) - (7)].classinfo));}
    }
    break;


  
    case 227:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1347 "parser.y"
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
#line 1369 "parser.y"
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
#line 1389 "parser.y"
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
#line 1407 "parser.y"
    {PASS12 (yyval.classinfo_list)=list_new();list_append((yyval.classinfo_list), (yyvsp[(1) - (1)].classinfo));}
    }
    break;


  
    case 233:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1408 "parser.y"
    {PASS12 (yyval.classinfo_list)=(yyvsp[(1) - (3)].classinfo_list);list_append((yyval.classinfo_list),(yyvsp[(3) - (3)].classinfo));}
    }
    break;


  
    case 234:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1410 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(1) - (1)].classinfo);}
    }
    break;


  
    case 235:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1411 "parser.y"
    {PASS12 (yyval.classinfo)=TYPE_ANY;}
    }
    break;


  
    case 236:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1412 "parser.y"
    {PASS12 (yyval.classinfo)=TYPE_VOID;}
    }
    break;


  
    case 237:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1421 "parser.y"
    {PASS12 (yyval.classinfo)=(yyvsp[(2) - (2)].classinfo);}
    }
    break;


  
    case 238:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1422 "parser.y"
    {PASS12 (yyval.classinfo)=0;}
    }
    break;


  
    case 239:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1426 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 240:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1427 "parser.y"
    {(yyval.value_list)=(yyvsp[(2) - (3)].value_list);}
    }
    break;


  
    case 241:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1429 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 244:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1433 "parser.y"
    {(yyval.value_list).number=1;
                                                  (yyval.value_list).cc = (yyvsp[(1) - (1)].value).c;
                                                 }
    }
    break;


  
    case 245:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1437 "parser.y"
    {(yyval.value_list) = (yyvsp[(1) - (2)].value_list);}
    }
    break;


  
    case 246:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1438 "parser.y"
    {
                                                  (yyval.value_list).number= (yyvsp[(1) - (2)].value_list).number+1;
                                                  (yyval.value_list).cc = code_append((yyvsp[(1) - (2)].value_list).cc, (yyvsp[(2) - (2)].value).c);
                                                  }
    }
    break;


  
    case 248:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1444 "parser.y"
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
#line 1491 "parser.y"
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
#line 1541 "parser.y"
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
#line 1562 "parser.y"
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
#line 1584 "parser.y"
    {
    (yyval.code) = abc_returnvoid(0);
}
    }
    break;


  
    case 253:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1587 "parser.y"
    {
    (yyval.code) = (yyvsp[(2) - (2)].value).c;
    (yyval.code) = abc_returnvalue((yyval.code));
}
    }
    break;


  
    case 254:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1594 "parser.y"
    {
    (yyval.value) = node_read((yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 255:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1597 "parser.y"
    {
    (yyval.value) = node_read((yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 256:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1600 "parser.y"
    {
    (yyval.node) = mkmultinode(&node_comma, (yyvsp[(1) - (1)].node));
}
    }
    break;


  
    case 257:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1603 "parser.y"
    {
    (yyval.node) = multinode_extend((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));
}
    }
    break;


  
    case 258:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1606 "parser.y"
    { 
    (yyval.code) = node_exec((yyvsp[(1) - (1)].node)); 
}
    }
    break;


  
    case 259:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1609 "parser.y"
    { 
    (yyval.code) = (yyvsp[(1) - (3)].code);
    (yyval.code) = code_append((yyval.code), node_exec((yyvsp[(3) - (3)].node))); 
}
    }
    break;


  
    case 260:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1614 "parser.y"
    {(yyval.value_list).cc=0;(yyval.value_list).number=0;}
    }
    break;


  
    case 261:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1615 "parser.y"
    {(yyval.value_list)=(yyvsp[(1) - (1)].value_list);}
    }
    break;


  
    case 262:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1617 "parser.y"
    {(yyval.code)=abc_pushstring(0,(yyvsp[(1) - (1)].id));}
    }
    break;


  
    case 263:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1618 "parser.y"
    {(yyval.code)=abc_pushstring2(0,&(yyvsp[(1) - (1)].str));}
    }
    break;


  
    case 264:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1619 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 265:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1620 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 266:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1621 "parser.y"
    {syntaxerror("dictionary keys must be strings");}
    }
    break;


  
    case 267:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1623 "parser.y"
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
#line 1629 "parser.y"
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
#line 1638 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 270:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1639 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 271:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1640 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 272:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1641 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 273:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1642 "parser.y"
    {(yyval.node) = mkcodenode((yyvsp[(1) - (1)].value));}
    }
    break;


  
    case 274:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1643 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node);}
    }
    break;


  
    case 275:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1645 "parser.y"
    { 
    (yyval.node) = mkconstnode((yyvsp[(1) - (1)].constant));
}
    }
    break;


  
    case 276:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1649 "parser.y"
    {
    (yyval.node) = (yyvsp[(1) - (1)].node);
}
    }
    break;


  
    case 277:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1654 "parser.y"
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
#line 1672 "parser.y"
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
#line 1683 "parser.y"
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
#line 1693 "parser.y"
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
#line 1702 "parser.y"
    {(yyval.node) = mknode2(&node_lt,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 282:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1703 "parser.y"
    {(yyval.node) = mknode2(&node_gt,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 283:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1704 "parser.y"
    {(yyval.node) = mknode2(&node_le,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 284:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1705 "parser.y"
    {(yyval.node) = mknode2(&node_ge,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 285:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1706 "parser.y"
    {(yyval.node) = mknode2(&node_eqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 286:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1707 "parser.y"
    {(yyval.node) = mknode2(&node_eqeqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 287:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1708 "parser.y"
    {(yyval.node) = mknode2(&node_noteqeq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 288:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1709 "parser.y"
    {(yyval.node) = mknode2(&node_noteq,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 289:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1710 "parser.y"
    {(yyval.node) = mknode2(&node_oror,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 290:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1711 "parser.y"
    {(yyval.node) = mknode2(&node_andand,(yyvsp[(1) - (3)].node),(yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 291:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1712 "parser.y"
    {(yyval.node) = mknode1(&node_not, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 292:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1713 "parser.y"
    {(yyval.node) = mknode1(&node_bitnot, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 293:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1714 "parser.y"
    {(yyval.node) = mknode2(&node_bitand, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 294:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1715 "parser.y"
    {(yyval.node) = mknode2(&node_bitxor, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 295:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1716 "parser.y"
    {(yyval.node) = mknode2(&node_bitor, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 296:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1717 "parser.y"
    {(yyval.node) = mknode2(&node_shr, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 297:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1718 "parser.y"
    {(yyval.node) = mknode2(&node_ushr, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 298:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1719 "parser.y"
    {(yyval.node) = mknode2(&node_shl, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 299:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1720 "parser.y"
    {(yyval.node) = mknode2(&node_div, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 300:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1721 "parser.y"
    {(yyval.node) = mknode2(&node_mod, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 301:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1722 "parser.y"
    {(yyval.node) = mknode2(&node_plus, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 302:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1723 "parser.y"
    {(yyval.node) = mknode2(&node_minus, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 303:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1724 "parser.y"
    {(yyval.node) = mknode2(&node_multiply, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 304:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1725 "parser.y"
    {(yyval.node) = mknode2(&node_in, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 305:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1726 "parser.y"
    {(yyval.node) = mknode2(&node_as, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 306:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1727 "parser.y"
    {(yyval.node) = mknode2(&node_instanceof, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 307:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1728 "parser.y"
    {(yyval.node) = mknode2(&node_is, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 308:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1729 "parser.y"
    {(yyval.node) = mknode1(&node_typeof, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 309:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1730 "parser.y"
    {(yyval.node) = mknode1(&node_void, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 310:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1731 "parser.y"
    { (yyval.node) = mkconstnode(constant_new_undefined());}
    }
    break;


  
    case 311:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1732 "parser.y"
    { (yyval.node)=(yyvsp[(2) - (3)].node);}
    }
    break;


  
    case 312:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1733 "parser.y"
    {(yyval.node) = mknode1(&node_neg, (yyvsp[(2) - (2)].node));}
    }
    break;


  
    case 313:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1734 "parser.y"
    {(yyval.node) = mknode2(&node_arraylookup, (yyvsp[(1) - (4)].node),(yyvsp[(3) - (4)].node));}
    }
    break;


  
    case 314:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1735 "parser.y"
    {(yyval.node) = mknode2(&node_muleq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 315:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1736 "parser.y"
    {(yyval.node) = mknode2(&node_modeq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 316:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1737 "parser.y"
    {(yyval.node) = mknode2(&node_shleq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 317:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1738 "parser.y"
    {(yyval.node) = mknode2(&node_shreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 318:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1739 "parser.y"
    {(yyval.node) = mknode2(&node_ushreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 319:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1740 "parser.y"
    { (yyval.node) = mknode2(&node_diveq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 320:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1741 "parser.y"
    { (yyval.node) = mknode2(&node_bitoreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 321:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1742 "parser.y"
    { (yyval.node) = mknode2(&node_bitxoreq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 322:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1743 "parser.y"
    { (yyval.node) = mknode2(&node_bitandeq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 323:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1744 "parser.y"
    { (yyval.node) = mknode2(&node_pluseq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 324:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1745 "parser.y"
    { (yyval.node) = mknode2(&node_minuseq, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 325:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1746 "parser.y"
    { (yyval.node) = mknode2(&node_assign, (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node));}
    }
    break;


  
    case 326:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1747 "parser.y"
    { (yyval.node) = mknode3(&node_tenary, (yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node));}
    }
    break;


  
    case 327:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1749 "parser.y"
    { (yyval.node) = mknode1(&node_rplusplus, (yyvsp[(1) - (2)].node));}
    }
    break;


  
    case 328:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1750 "parser.y"
    { (yyval.node) = mknode1(&node_rminusminus, (yyvsp[(1) - (2)].node));}
    }
    break;


  
    case 329:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1751 "parser.y"
    {(yyval.node) = mknode1(&node_lplusplus, (yyvsp[(2) - (2)].node)); }
    }
    break;


  
    case 330:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1752 "parser.y"
    {(yyval.node) = mknode1(&node_lminusminus, (yyvsp[(2) - (2)].node)); }
    }
    break;


  
    case 331:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1755 "parser.y"
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
#line 1769 "parser.y"
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
#line 1777 "parser.y"
    {PASS12 new_state();state->xmlfilter=1;}
    }
    break;


  
    case 334:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1777 "parser.y"
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
#line 1831 "parser.y"
    {(yyval.id)=(yyvsp[(1) - (1)].id);}
    }
    break;


  
    case 336:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1832 "parser.y"
    {(yyval.id)="*";}
    }
    break;


  
    case 338:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1835 "parser.y"
    {(yyval.id)="*";}
    }
    break;


  
    case 339:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1876 "parser.y"
    {
}
    }
    break;


  
    case 340:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1879 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].id), (yyvsp[(5) - (5)].id), 0, 0);
}
    }
    break;


  
    case 341:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1882 "parser.y"
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
#line 1889 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].id), (yyvsp[(5) - (5)].id), 1, 0);
}
    }
    break;


  
    case 343:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1892 "parser.y"
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
#line 1903 "parser.y"
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
#line 1911 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (6)].node), (yyvsp[(4) - (6)].id), (yyvsp[(6) - (6)].id), 0, 1);
}
    }
    break;


  
    case 346:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1915 "parser.y"
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
#line 1922 "parser.y"
    {
    (yyval.node) = get_descendants((yyvsp[(1) - (6)].node), (yyvsp[(4) - (6)].id), (yyvsp[(6) - (6)].id), 1, 1);
}
    }
    break;


  
    case 348:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 1926 "parser.y"
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
#line 1936 "parser.y"
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
#line 1947 "parser.y"
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


  
    case 352:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2145 "parser.y"
    {
    PASS1
    char*name = (yyvsp[(2) - (2)].id);
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
    char*name = (yyvsp[(2) - (2)].id);
    (yyval.node) = resolve_identifier(name);
}
    }
    break;


  
    case 353:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2180 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (2)].id);
    n->url = (yyvsp[(2) - (2)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 354:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2187 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].id);
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 355:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2194 "parser.y"
    {
    PASS12
    NEW(namespace_decl_t,n);
    n->name = (yyvsp[(2) - (4)].id);
    n->url = (yyvsp[(4) - (4)].str).str;
    (yyval.namespace_decl)=n;
}
    }
    break;


  
    case 356:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2201 "parser.y"
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


  
    case 357:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2224 "parser.y"
    {
    (yyval.code) = 0;
    (yyval.code) = code_append((yyval.code), node_read((yyvsp[(4) - (4)].node)).c);
    (yyval.code) = abc_dxnslate((yyval.code));
}
    }
    break;


  
    case 358:
    if(as3_pass==2) {

/* Line 1464 of skeleton.m4  */
#line 2230 "parser.y"
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
#line 7330 "parser.tab.c"
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



