/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "samsgram.y" /* yacc.c:337  */

#define YYDEBUG 1
/*
 *  @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: samsgram.y,v $
 * Revision 1.1.6.2  1996/03/09  23:32:16  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:47:21  marty]
 *
 * Revision 1.1.6.1  1995/12/08  20:55:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:37  root]
 * 
 * Revision 1.1.4.7  1994/09/30  07:56:09  rsalz
 * 	Implement DCE RFC 24.2 (OT CR 11929)
 * 	[1994/09/30  07:44:20  rsalz]
 * 
 * Revision 1.1.4.6  1994/06/10  20:53:25  devsrc
 * 	cr10872- fix copyright
 * 	[1994/06/10  16:38:11  devsrc]
 * 
 * Revision 1.1.4.5  1994/04/17  22:39:12  rsalz
 * 	Check number of % sequences for XPG4 (OT CR 10337)
 * 	[1994/04/17  22:38:49  rsalz]
 * 
 * Revision 1.1.4.4  1994/04/06  15:01:20  rsalz
 * 	Work-around strange malloc interaction (OT CR 10305).
 * 	[1994/04/06  14:54:13  rsalz]
 * 
 * Revision 1.1.4.3  1994/03/11  21:09:23  rsalz
 * 	Add -f flag.
 * 	Fix segfault (was freeing wrong pointer) (OT CR 10122).
 * 	[1994/03/11  14:14:20  rsalz]
 * 
 * Revision 1.1.4.2  1994/03/10  21:09:30  rsalz
 * 	Add collections and conditional text (OT CR 9697).
 * 	[1994/03/10  20:45:47  rsalz]
 * 
 * Revision 1.1.4.1  1993/12/14  21:47:35  rsalz
 * 	Remove #include <dce/dce_error.h>
 * 	[1993/12/14  21:47:27  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  17:41:59  rsalz
 * 	Initial release
 * 	[1993/08/16  17:41:13  rsalz]
 * 
 * $EndLog$
 */
#include <sams.h>


/*
**  This is defined in <dce/dce_error.h> but we want to build sams
**  before that header file is installed, so we just copy the constant.
*/
#define DCE_ERROR_STRING_SIZE 160 


/*
**  Build a msgfield_t from its parts.
*/
#define Makemsgfield_t(f, t, v, n)	\
	(f).Token = (t), (f).Value = (v), (f).Number = (n)
#define Dummymsgfield_t(f, t, v)	\
	(f).Token = (t), (f).Value = NULL, free((v)), (f).Number = NONUM


/*
**  A header field; comes at the start of the file.
*/
typedef struct headfield_s_t {
    int		Token;
    char	*Name;
    char	**Valptr;
    int		*Intptr;
    int		Seen;
} headfield_t;


/*
**  A record field; occurs within each message definition.
*/
typedef struct recfield_s_t {
    int		Token;
    char	*Name;
    int		Optional;
    int		Count;
} recfield_t;

/*
**  The fields we're collecting in the current record.  I don't like using
**  globals, but this is much more efficient than building up a linked
**  list and using that.
*/
static int		CountMsgFields;
static msgfield_t	MsgFields[20];

#line 175 "y.tab.c" /* yacc.c:337  */
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    tACTION = 258,
    tADMINISTRATOR = 259,
    tATTRIBUTES = 260,
    tBIT = 261,
    tCODE = 262,
    tCOLLECTION = 263,
    tCOMPONENT = 264,
    tDEFAULT = 265,
    tEND = 266,
    tENGINEER = 267,
    tEXPLANATION = 268,
    tHANDLE = 269,
    tID = 270,
    tNOT = 271,
    tNOTES = 272,
    tNUMBER = 273,
    tOPERATOR = 274,
    tPROGRAMMER = 275,
    tRESPONSE = 276,
    tSERVICEABILITY = 277,
    tSET = 278,
    tSEVERITY = 279,
    tSIZE = 280,
    tSTART = 281,
    tSUBCOMP = 282,
    tSYSTEM = 283,
    tTABLE = 284,
    tTABLES = 285,
    tTECHNOLOGY = 286,
    tTEXT = 287,
    tUSER = 288,
    tVALUE = 289,
    tVENDOR = 290
  };
#endif
/* Tokens.  */
#define tACTION 258
#define tADMINISTRATOR 259
#define tATTRIBUTES 260
#define tBIT 261
#define tCODE 262
#define tCOLLECTION 263
#define tCOMPONENT 264
#define tDEFAULT 265
#define tEND 266
#define tENGINEER 267
#define tEXPLANATION 268
#define tHANDLE 269
#define tID 270
#define tNOT 271
#define tNOTES 272
#define tNUMBER 273
#define tOPERATOR 274
#define tPROGRAMMER 275
#define tRESPONSE 276
#define tSERVICEABILITY 277
#define tSET 278
#define tSEVERITY 279
#define tSIZE 280
#define tSTART 281
#define tSUBCOMP 282
#define tSYSTEM 283
#define tTABLE 284
#define tTABLES 285
#define tTECHNOLOGY 286
#define tTEXT 287
#define tUSER 288
#define tVALUE 289
#define tVENDOR 290

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 106 "samsgram.y" /* yacc.c:352  */

    int			Number;
    char		*String;
    struct msgfield_s_t	Field;
    struct svcfield_s_t	Svc;

#line 295 "y.tab.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



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
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  22
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   145

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  51
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  108

#define YYUNDEFTOK  2
#define YYMAXUTOK   290

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      37,    38,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    36,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   126,   126,   127,   130,   136,   143,   152,   159,   164,
     165,   166,   171,   177,   180,   183,   186,   189,   192,   197,
     198,   201,   204,   205,   208,   216,   219,   224,   227,   230,
     233,   238,   242,   251,   254,   257,   262,   265,   268,   271,
     274,   278,   281,   284,   287,   290,   293,   296,   299,   302,
     308,   311
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "tACTION", "tADMINISTRATOR",
  "tATTRIBUTES", "tBIT", "tCODE", "tCOLLECTION", "tCOMPONENT", "tDEFAULT",
  "tEND", "tENGINEER", "tEXPLANATION", "tHANDLE", "tID", "tNOT", "tNOTES",
  "tNUMBER", "tOPERATOR", "tPROGRAMMER", "tRESPONSE", "tSERVICEABILITY",
  "tSET", "tSEVERITY", "tSIZE", "tSTART", "tSUBCOMP", "tSYSTEM", "tTABLE",
  "tTABLES", "tTECHNOLOGY", "tTEXT", "tUSER", "tVALUE", "tVENDOR", "'='",
  "'('", "')'", "$accept", "file", "svc", "l_svc", "e_svc", "headers",
  "l_head", "header", "records", "record", "o_bits", "bits", "l_field",
  "field", "tables", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,    61,    40,    41
};
# endif

#define YYPACT_NINF -18

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-18)))

#define YYTABLE_NINF -26

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      12,    12,   -17,    50,    32,    -5,    18,    21,    40,    51,
     -18,    27,   -18,    29,    33,   -18,   -18,    48,    36,   -18,
     -18,   -18,   -18,    30,    26,    -1,    -9,    -9,   -18,   -18,
     -18,   -18,   -18,   -18,    61,    53,    39,    58,    62,    92,
      36,    -9,   -18,   -18,    67,    64,   -18,   -18,    69,    72,
      70,    73,    75,    83,    85,    80,    81,    88,    91,    86,
      71,    95,    93,    98,    59,   -18,   100,   -18,   -18,   102,
     -18,    82,   -18,   -18,   -18,   106,   108,   -18,   -18,   111,
     113,   -18,   114,   115,   -18,   -18,   105,   -18,   116,   -18,
     -18,   -18,   -18,   -14,   -18,   -18,   109,   -18,   -18,   -18,
     117,    -2,   -18,   118,   -18,   -18,   120,   -18
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       8,     0,    11,     0,     0,    14,    27,     0,    15,    18,
      16,    17,     1,     0,     0,     0,     0,     2,    19,    10,
      13,    12,    28,    29,     0,     0,     0,     0,     0,     0,
      26,     3,    20,    30,     0,     0,    22,    24,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,     0,    23,    33,     0,
      41,    34,    36,    37,    38,     0,     0,    46,    42,     0,
       0,    40,     0,     0,    21,    32,     0,    43,     0,    44,
      45,    47,    50,     0,    48,    49,     0,    35,    51,    39,
       0,     0,     5,     0,     4,     6,     0,     7
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -18,   -18,   -18,   -18,    37,   -18,    44,   -18,   119,     3,
     -18,   112,   -18,    76,   -18
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     8,    26,   101,   102,     9,    10,    11,    27,    28,
      39,    18,    64,    65,    93
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      37,    98,   -25,   -25,   -25,    16,   -25,    38,    13,   104,
      19,   -25,   -25,     1,    24,    17,   -25,    25,   -25,   -25,
       2,     3,     4,   -25,    99,   100,   -25,   -25,     1,   -25,
      42,   -25,   -25,    20,   -25,     2,     3,     4,    16,    21,
      22,     5,    33,     6,    42,    12,     7,    30,    17,    -9,
      -9,    31,    34,    -9,    32,    29,     5,    14,     6,    35,
      36,     7,    48,    49,    50,    15,    51,    43,    44,    46,
      84,    52,    53,    23,    24,    45,    54,    25,    55,    56,
      47,    66,    67,    57,    68,    70,    58,    59,    71,    60,
      72,    61,    62,    69,    63,    48,    49,    50,    73,    51,
      74,    75,    76,    77,    52,    53,    78,    79,    80,    54,
      81,    55,    56,    83,    82,    86,    57,    87,    88,    58,
      59,    89,    60,    90,    61,    62,    91,    63,    92,    94,
      95,    96,   103,   106,    97,   107,   100,    40,   105,     0,
      85,     0,     0,     0,     0,    41
};

static const yytype_int8 yycheck[] =
{
       1,    15,     3,     4,     5,     6,     7,     8,    25,    11,
      15,    12,    13,     1,    23,    16,    17,    26,    19,    20,
       8,     9,    10,    24,    38,    27,    27,    28,     1,    30,
      27,    32,    33,    15,    35,     8,     9,    10,     6,    18,
       0,    29,     6,    31,    41,     1,    34,    18,    16,    22,
      23,    18,    16,    26,     6,    11,    29,     7,    31,    29,
      34,    34,     3,     4,     5,    15,     7,     6,    15,    11,
      11,    12,    13,    22,    23,    36,    17,    26,    19,    20,
      18,    14,    18,    24,    15,    15,    27,    28,    15,    30,
      15,    32,    33,    21,    35,     3,     4,     5,    15,     7,
      15,    21,    21,    15,    12,    13,    15,    21,    37,    17,
      15,    19,    20,    15,    21,    15,    24,    15,    36,    27,
      28,    15,    30,    15,    32,    33,    15,    35,    15,    15,
      15,    26,    15,    15,    18,    15,    27,    25,   101,    -1,
      64,    -1,    -1,    -1,    -1,    26
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     8,     9,    10,    29,    31,    34,    40,    44,
      45,    46,    45,    25,     7,    15,     6,    16,    50,    15,
      15,    18,     0,    22,    23,    26,    41,    47,    48,    45,
      18,    18,     6,     6,    16,    29,    34,     1,     8,    49,
      50,    47,    48,     6,    15,    36,    11,    18,     3,     4,
       5,     7,    12,    13,    17,    19,    20,    24,    27,    28,
      30,    32,    33,    35,    51,    52,    14,    18,    15,    21,
      15,    15,    15,    15,    15,    21,    21,    15,    15,    21,
      37,    15,    21,    15,    11,    52,    15,    15,    36,    15,
      15,    15,    15,    53,    15,    15,    26,    18,    15,    38,
      27,    42,    43,    15,    11,    43,    15,    15
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    39,    40,    40,    41,    42,    42,    43,    44,    45,
      45,    45,    46,    46,    46,    46,    46,    46,    46,    47,
      47,    48,    48,    48,    48,    49,    49,    50,    50,    50,
      50,    51,    51,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      53,    53
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     3,     8,     1,     2,     4,     1,     1,
       2,     2,     3,     3,     2,     2,     2,     2,     2,     1,
       2,     4,     3,     4,     3,     0,     1,     1,     2,     2,
       3,     1,     2,     2,     2,     4,     2,     2,     2,     4,
       2,     2,     2,     3,     3,     3,     2,     3,     3,     3,
       1,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
            else
              goto append;

          append:
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

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
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
# else /* defined YYSTACK_RELOCATE */
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
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 130 "samsgram.y" /* yacc.c:1652  */
    {
	    ServiceTable = (yyvsp[-5].String);
	    ServiceHandle = (yyvsp[-3].String);
	}
#line 1467 "y.tab.c" /* yacc.c:1652  */
    break;

  case 5:
#line 136 "samsgram.y" /* yacc.c:1652  */
    {
	    if (SvcCount >= SvcSize - 1) {
		SvcSize += REC_CHUNK;
		Service = realloc(Service, SvcSize * sizeof *Service);
	    }
	    Service[SvcCount++] = (yyvsp[0].Svc);
	}
#line 1479 "y.tab.c" /* yacc.c:1652  */
    break;

  case 6:
#line 143 "samsgram.y" /* yacc.c:1652  */
    {
	    if (SvcCount >= SvcSize - 1) {
		SvcSize += REC_CHUNK;
		Service = realloc(Service, SvcSize * sizeof *Service);
	    }
	    Service[SvcCount++] = (yyvsp[0].Svc);
	}
#line 1491 "y.tab.c" /* yacc.c:1652  */
    break;

  case 7:
#line 152 "samsgram.y" /* yacc.c:1652  */
    {
	    (yyval.Svc).Subcomp = (yyvsp[-2].String);
	    (yyval.Svc).sc_name = (yyvsp[-1].String);
	    (yyval.Svc).Code = (yyvsp[0].String);
	}
#line 1501 "y.tab.c" /* yacc.c:1652  */
    break;

  case 8:
#line 159 "samsgram.y" /* yacc.c:1652  */
    {
	    ValidateHeaders();
	}
#line 1509 "y.tab.c" /* yacc.c:1652  */
    break;

  case 11:
#line 166 "samsgram.y" /* yacc.c:1652  */
    {
	    YYABORT;
	}
#line 1517 "y.tab.c" /* yacc.c:1652  */
    break;

  case 12:
#line 171 "samsgram.y" /* yacc.c:1652  */
    {
	    char	buff[SMBUF];

	    sprintf(buff, "%d", (yyvsp[0].Number));
	    SetHeader(tCODE, strdup(buff), 0);
	}
#line 1528 "y.tab.c" /* yacc.c:1652  */
    break;

  case 13:
#line 177 "samsgram.y" /* yacc.c:1652  */
    {
	    SetHeader(tCOLLECTION, NULL, (yyvsp[0].Number));
	}
#line 1536 "y.tab.c" /* yacc.c:1652  */
    break;

  case 14:
#line 180 "samsgram.y" /* yacc.c:1652  */
    {
	    SetHeader(tCOMPONENT, (yyvsp[0].String), 0);
	}
#line 1544 "y.tab.c" /* yacc.c:1652  */
    break;

  case 15:
#line 183 "samsgram.y" /* yacc.c:1652  */
    {
	    SetHeader(tDEFAULT, NULL, (yyvsp[0].Number));
	}
#line 1552 "y.tab.c" /* yacc.c:1652  */
    break;

  case 16:
#line 186 "samsgram.y" /* yacc.c:1652  */
    {
	    SetHeader(tTECHNOLOGY, (yyvsp[0].String), 0);
	}
#line 1560 "y.tab.c" /* yacc.c:1652  */
    break;

  case 17:
#line 189 "samsgram.y" /* yacc.c:1652  */
    {
	    SetHeader(tVALUE, NULL, (yyvsp[0].Number));
	}
#line 1568 "y.tab.c" /* yacc.c:1652  */
    break;

  case 18:
#line 192 "samsgram.y" /* yacc.c:1652  */
    {
	    SetHeader(tTABLE, (yyvsp[0].String), 0);
	}
#line 1576 "y.tab.c" /* yacc.c:1652  */
    break;

  case 21:
#line 201 "samsgram.y" /* yacc.c:1652  */
    {
	    ValidateRecord((yyvsp[-2].Number));
	}
#line 1584 "y.tab.c" /* yacc.c:1652  */
    break;

  case 23:
#line 205 "samsgram.y" /* yacc.c:1652  */
    {
	    StoreChangeRecord((yyvsp[0].Number));
	}
#line 1592 "y.tab.c" /* yacc.c:1652  */
    break;

  case 24:
#line 208 "samsgram.y" /* yacc.c:1652  */
    {
	    if ((yyvsp[0].Number) <= 0 || (yyvsp[0].Number) > CODE_MASK / CollectionSize)
		yyerror("Bad collection number");
	    else
		StoreCollectionRecord((yyvsp[0].Number));
	}
#line 1603 "y.tab.c" /* yacc.c:1652  */
    break;

  case 25:
#line 216 "samsgram.y" /* yacc.c:1652  */
    {
	    (yyval.Number) = DefaultHeader;
	}
#line 1611 "y.tab.c" /* yacc.c:1652  */
    break;

  case 26:
#line 219 "samsgram.y" /* yacc.c:1652  */
    {
	    (yyval.Number) = (yyvsp[0].Number);
	}
#line 1619 "y.tab.c" /* yacc.c:1652  */
    break;

  case 27:
#line 224 "samsgram.y" /* yacc.c:1652  */
    {
	    (yyval.Number) = (yyvsp[0].Number);
	}
#line 1627 "y.tab.c" /* yacc.c:1652  */
    break;

  case 28:
#line 227 "samsgram.y" /* yacc.c:1652  */
    {
	    (yyval.Number) = 0;
	}
#line 1635 "y.tab.c" /* yacc.c:1652  */
    break;

  case 29:
#line 230 "samsgram.y" /* yacc.c:1652  */
    {
	    (yyval.Number) = (yyvsp[-1].Number) | (yyvsp[0].Number);
	}
#line 1643 "y.tab.c" /* yacc.c:1652  */
    break;

  case 30:
#line 233 "samsgram.y" /* yacc.c:1652  */
    {
	    (yyval.Number) = (yyvsp[-2].Number) & ~(yyvsp[0].Number);
	}
#line 1651 "y.tab.c" /* yacc.c:1652  */
    break;

  case 31:
#line 238 "samsgram.y" /* yacc.c:1652  */
    {
	    MsgFields[0] = (yyvsp[0].Field);
	    CountMsgFields = 1;
	}
#line 1660 "y.tab.c" /* yacc.c:1652  */
    break;

  case 32:
#line 242 "samsgram.y" /* yacc.c:1652  */
    {
	    if (CountMsgFields > COUNTOF(MsgFields) - 1)
		yyerror("Too many fields in record");
	    else
		MsgFields[CountMsgFields++] = (yyvsp[0].Field);
	}
#line 1671 "y.tab.c" /* yacc.c:1652  */
    break;

  case 33:
#line 251 "samsgram.y" /* yacc.c:1652  */
    {
	    Makemsgfield_t((yyval.Field), tACTION, (yyvsp[0].String), NONUM);
	}
#line 1679 "y.tab.c" /* yacc.c:1652  */
    break;

  case 34:
#line 254 "samsgram.y" /* yacc.c:1652  */
    {
	    Makemsgfield_t((yyval.Field), tCODE, (yyvsp[0].String), NONUM);
	}
#line 1687 "y.tab.c" /* yacc.c:1652  */
    break;

  case 35:
#line 257 "samsgram.y" /* yacc.c:1652  */
    {
	    if ((yyvsp[0].Number) == NONUM)
		yyerror("Using reserved message ID");
	    Makemsgfield_t((yyval.Field), tCODE, (yyvsp[-2].String), (yyvsp[0].Number));
	}
#line 1697 "y.tab.c" /* yacc.c:1652  */
    break;

  case 36:
#line 262 "samsgram.y" /* yacc.c:1652  */
    {
	    Makemsgfield_t((yyval.Field), tENGINEER, (yyvsp[0].String), NONUM);
	}
#line 1705 "y.tab.c" /* yacc.c:1652  */
    break;

  case 37:
#line 265 "samsgram.y" /* yacc.c:1652  */
    {
	    Makemsgfield_t((yyval.Field), tEXPLANATION, (yyvsp[0].String), NONUM);
	}
#line 1713 "y.tab.c" /* yacc.c:1652  */
    break;

  case 38:
#line 268 "samsgram.y" /* yacc.c:1652  */
    {
	    Makemsgfield_t((yyval.Field), tNOTES, (yyvsp[0].String), NONUM);
	}
#line 1721 "y.tab.c" /* yacc.c:1652  */
    break;

  case 39:
#line 271 "samsgram.y" /* yacc.c:1652  */
    {
	    Makemsgfield_t((yyval.Field), tTABLES, strdup(""), (yyvsp[-1].Number));
	}
#line 1729 "y.tab.c" /* yacc.c:1652  */
    break;

  case 40:
#line 274 "samsgram.y" /* yacc.c:1652  */
    {
	    CheckTextField((yyvsp[0].String));
	    Makemsgfield_t((yyval.Field), tTEXT, (yyvsp[0].String), NONUM);
	}
#line 1738 "y.tab.c" /* yacc.c:1652  */
    break;

  case 41:
#line 278 "samsgram.y" /* yacc.c:1652  */
    {
	    Makemsgfield_t((yyval.Field), tATTRIBUTES, (yyvsp[0].String), NONUM);
	}
#line 1746 "y.tab.c" /* yacc.c:1652  */
    break;

  case 42:
#line 281 "samsgram.y" /* yacc.c:1652  */
    {
	    Makemsgfield_t((yyval.Field), tSUBCOMP, (yyvsp[0].String), NONUM);
	}
#line 1754 "y.tab.c" /* yacc.c:1652  */
    break;

  case 43:
#line 284 "samsgram.y" /* yacc.c:1652  */
    {
	    Dummymsgfield_t((yyval.Field), tADMINISTRATOR, (yyvsp[0].String));
	}
#line 1762 "y.tab.c" /* yacc.c:1652  */
    break;

  case 44:
#line 287 "samsgram.y" /* yacc.c:1652  */
    {
	    Dummymsgfield_t((yyval.Field), tOPERATOR, (yyvsp[0].String));
	}
#line 1770 "y.tab.c" /* yacc.c:1652  */
    break;

  case 45:
#line 290 "samsgram.y" /* yacc.c:1652  */
    {
	    Dummymsgfield_t((yyval.Field), tPROGRAMMER, (yyvsp[0].String));
	}
#line 1778 "y.tab.c" /* yacc.c:1652  */
    break;

  case 46:
#line 293 "samsgram.y" /* yacc.c:1652  */
    {
	    Dummymsgfield_t((yyval.Field), tSEVERITY, (yyvsp[0].String));
	}
#line 1786 "y.tab.c" /* yacc.c:1652  */
    break;

  case 47:
#line 296 "samsgram.y" /* yacc.c:1652  */
    {
	    Dummymsgfield_t((yyval.Field), tSYSTEM, (yyvsp[0].String));
	}
#line 1794 "y.tab.c" /* yacc.c:1652  */
    break;

  case 48:
#line 299 "samsgram.y" /* yacc.c:1652  */
    {
	    Dummymsgfield_t((yyval.Field), tUSER, (yyvsp[0].String));
	}
#line 1802 "y.tab.c" /* yacc.c:1652  */
    break;

  case 49:
#line 302 "samsgram.y" /* yacc.c:1652  */
    {
	    Dummymsgfield_t((yyval.Field), tVENDOR, (yyvsp[-1].String));
	    free((yyvsp[0].String));
	}
#line 1811 "y.tab.c" /* yacc.c:1652  */
    break;

  case 50:
#line 308 "samsgram.y" /* yacc.c:1652  */
    {
	    (yyval.Number) = Table == NULL || strcmp((yyvsp[0].String), Table) == 0;
	}
#line 1819 "y.tab.c" /* yacc.c:1652  */
    break;

  case 51:
#line 311 "samsgram.y" /* yacc.c:1652  */
    {
	    (yyval.Number) = (yyvsp[-1].Number) || Table == NULL || strcmp((yyvsp[0].String), Table) == 0;
	}
#line 1827 "y.tab.c" /* yacc.c:1652  */
    break;


#line 1831 "y.tab.c" /* yacc.c:1652  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 315 "samsgram.y" /* yacc.c:1918  */



/*
**  These are the headers that must be at the start of the file.
*/
static headfield_t	Headers[] = {
    {	tCODE,		"code",		&CodeHeader,	NULL		},
    {	tCOLLECTION,	"collection",	NULL,		&CollectionSize	},
    {	tCOMPONENT,	"component",	&CompHeader,	NULL		},
    {	tDEFAULT,	"default",	NULL,		&DefaultHeader	},
    {	tTECHNOLOGY,	"technology",	&TechHeader,	NULL		},
    {	tVALUE,		"value",	NULL,		&ValueHeader	},
    {	tTABLE,		"table",	&TableHeader,	NULL		},
};

/*
**  These are the records for each message.
*/
static recfield_t	RecordFields[] = {
    {	tACTION,	"action",		FALSE,	0	},
    {	tATTRIBUTES,	"attributes",		TRUE,	0	},
    {	tCODE,		"code",			FALSE,	0	},
    {	tENGINEER,	"engineer",		TRUE,	0	},
    {	tEXPLANATION,	"explanation",		FALSE,	0	},
    {	tSUBCOMP,	"sub-component",	TRUE,	0	},
    {	tNOTES,		"notes",		TRUE,	0	},
    {	tTABLES,	"tables",		TRUE,	0	},
    {	tTEXT,		"text",			FALSE,	0	},
    {	tADMINISTRATOR,	"admin. response",	TRUE,	0	},
    {	tOPERATOR,	"operator response",	TRUE,	0	},
    {	tPROGRAMMER,	"programmer response",	TRUE,	0	},
    {	tSEVERITY,	"severity",		TRUE,	0	},
    {	tSYSTEM,	"system response",	TRUE,	0	},
    {	tUSER,		"user response",	TRUE,	0	},
};


/*
**  Make sure that each header field is legitimate.
*/
static void ValidateHeaders( void )
{
    static char		OPTIONAL[] = "$";
    headfield_t		*hp;
    long		l;
    char		*p;
    char		buff[SMBUF];

    /* Set defaults. */
    if (CodeHeader == NULL && CompHeader == NULL) {
	yyerror("Must have \"component\" or \"component code\" header");
	return;
    }
    if (CodeHeader != NULL && CompHeader != NULL) {
	yyerror("Cannot have both \"component\" and \"component code\" header");
	return;
    }

    /* Make sure code header is valid. */
    if (CodeHeader == NULL) {
	for (p = CompHeader; *p; p++)
	    if (strchr(RAD40ALPHABET, *p) == NULL) {
		(void)sprintf(buff,
			"Illegal character `%c' in \"component\"", *p);
		yyerror(buff);
	    }
	if (strlen(CompHeader) != 3)
	    yyerror("\"component\" header must have 3 characters");
	CodeHeader = OPTIONAL;
    }
    else {
	l = strtol(CodeHeader, &p, 0);
	if (l <= 0 || *p)
	    yyerror("Illegal value for \"component code\" header");
	if (l != (l & (TECH_MASK | ISV_COMP_MASK)))
	    yyerror("\"component code\" header value masked off");
	if ((l & TECH_MASK) != ISV1_TECH && (l & TECH_MASK) != ISV2_TECH)
	    yyerror("Illegal value for \"component code\" header");
	sprintf(buff, "%6.6lx", l >> ISV_COMP_SHIFT);
	CompHeader = strdup(buff);
	CodeHeader = strdup(buff);
    }

    if (TableHeader == NULL) {
	if (CodeHeader == OPTIONAL)
	    (void)sprintf(buff, "%s_msg_table", CompHeader);
	else
	    (void)sprintf(buff, "dce_%s_msg_table", CompHeader);
	TableHeader = strdup(buff);
    }


    /* Make sure they're all there. */
    for (hp = Headers; hp < ENDOF(Headers); hp++)
	if (hp->Valptr && *hp->Valptr == NULL)
	    y2error("Missing \"%s\" header", hp->Name);

    if (CodeHeader == OPTIONAL)
	CodeHeader = NULL;

    /* Make sure technology header is valid. */
    if (TechHeader && Technology(TechHeader) < 0)
	y2error("Ilegal value `%s' for \"technology\" header", TechHeader);
    if (CodeHeader != NULL && strcmp(TechHeader, "dce") != 0)
	yyerror("\"technology\" header must be \"dce\"");

    /* Make sure value header is valid. */
    l = CompHeader != NULL ? CODE_MASK : ISV_CODE_MASK;
    if (ValueHeader > l) {
	(void)sprintf(buff, "Value `%d' too big for \"value\" header",
		ValueHeader);
	yyerror(buff);
    }
}


/*
**  Store a header.
*/
static void SetHeader( int t, char *value, int Number)
{
    headfield_t		*hp;
    char		buff[SMBUF];

    for (hp = Headers; hp < ENDOF(Headers); hp++)
	if (hp->Token == t) {
	    if (hp->Seen)
		y2error("Duplicate \"%s\" header", hp->Name);
	    else if (value)
		*hp->Valptr = value;
	    else
		*hp->Intptr = Number;
	    hp->Seen = TRUE;
	    return;
	}
    (void)sprintf(buff, "Internal error -- unknown header %d", t);
    yyerror(buff);
}

/*
**  Add a "change counter" record to the in-core array.
*/
static void StoreChangeRecord( int i)
{
    record_t		*rp;

    /* Get more space if needed. */
    if (RecCount >= RecSize - 1) {
	RecSize += REC_CHUNK;
	Records = realloc(Records, RecSize * sizeof *Records);
    }

    /* Get record, fill in fields. */
    rp = &Records[RecCount++];
    rp->Flags = MTsetvalue;
    rp->Number = i;
    rp->Code = "";
}


/*
**  Add a "collection" record to the in-core array.
*/
static void StoreCollectionRecord( int	i)
{
    record_t		*rp;

    /* Get more space if needed. */
    if (RecCount >= RecSize - 1) {
	RecSize += REC_CHUNK;
	Records = realloc(Records, RecSize * sizeof *Records);
    }

    /* Get record, fill in fields. */
    rp = &Records[RecCount++];
    rp->Flags = MTcollect;
    rp->Number = i;
    rp->Code = "";
}


/*
**  Turn "foo <%s|file> 3\<5" into "foo \*Lfile\*O 3<5" for documentation text.
*/
static char *DocumentationFilter(char	*p)
{
    char	*save;
    char	*dest;
    int		i;

    /* Get worst-case amount of space. */
    for (i = strlen(p) + 1, dest = p; *dest; dest++)
	if (*dest == '<')
	    /* Add space for \*L and \*O */
	    i += 3 + 3;
    save = dest = malloc(i);

    while (*p) {
	if (*p != '<') {
	    if (*p == '\\' && p[1] == '<') {
		*dest++ = '<';
		p += 2;
	    }
	    else
		*dest++ = *p++;
	    continue;
	}

	while (*++p && *p != '|')
	    continue;
	if (*p != '|') {
	    yyerror("Missing | in <a|b> construct");
	    break;
	}
	*dest++ = '\\';
	*dest++ = '*';
	*dest++ = 'L';
	while (*++p && *p != '>')
	    *dest++ = *p;
	if (*p != '>') {
	    yyerror("Missing > in <a|b> construct");
	    break;
	}
	*dest++ = '\\';
	*dest++ = '*';
	*dest++ = 'O';
	p++;
    }
    *dest = '\0';

    return save;
}

/*
**  Turn "foo <%s|file> 3\<5" into "foo %s 3<5" for program text.
*/
static char *ProgramFilter(char	*p)
{
    char	*save;
    char	*dest;

    /* Get worst-case amount of output space. */
    dest = save = malloc(strlen(p) + 1);

    while (*p) {
	if (*p != '<') {
	    if (*p == '\\' && p[1] == '<') {
		*dest++ = '<';
		p += 2;
	    }
	    else
		*dest++ = *p++;
	    continue;
	}

	while (*++p && *p != '|')
	    *dest++ = *p;
	if (*p != '|') {
	    yyerror("Missing | in <a|b> construct");
	    break;
	}
	while (*++p && *p != '>')
	    continue;
	if (*p != '>') {
	    yyerror("Missing > in <a|b> construct");
	    break;
	}
	p++;
    }
    *dest = '\0';

    return save;
}


/*
**  Add a message record to the in-core array.
*/
static void StoreRecord(int Flags)
{
    static record_t	nullrec;
    record_t		*rp;
    msgfield_t		*f;
    int			i;
    char		buff[SMBUF];

    /* Get more space if needed. */
    if (RecCount >= RecSize - 1) {
	RecSize += REC_CHUNK;
	Records = realloc(Records, RecSize * sizeof *Records);
    }

    /* Get record, fill in known and default fields. */
    rp = &Records[RecCount++];
    *rp = nullrec;
    rp->Flags = Flags;
    rp->TableFlag = NONUM;

    /* Loop over fields; store each one in right place in record. */
    for (i = CountMsgFields, f = MsgFields; --i >= 0; f++)
	switch (f->Token) {
	default:
	    (void)sprintf(buff, "Internal error storing unknown type %d",
		    f->Token);
	    yyerror(buff);
	    break;
	/* Skip ignored fields. */
	case tADMINISTRATOR:
	case tOPERATOR:
	case tPROGRAMMER:
	case tSEVERITY:
	case tSYSTEM:
	case tUSER:
	case tVENDOR:
	    break;
	case tACTION:
	    rp->Action = f->Value;
	    break;
	case tATTRIBUTES:
	    rp->Attributes = f->Value;
	    break;
	case tCODE:
	    rp->Code = f->Value;
	    if (f->Number != NONUM) {
		rp->Set = TRUE;
		rp->Number = f->Number;
	    }
	    break;
	case tENGINEER:
	    break;
	case tEXPLANATION:
	    rp->Explanation = f->Value;
	    break;
	case tSUBCOMP:
	    rp->Subcomp = f->Value;
	    break;
	case tNOTES:
	    rp->Notes = f->Value;
	    break;
	case tTABLES:
	    rp->TableFlag = f->Number;
	    break;
	case tTEXT:
	    if (FilterText) {
		rp->DocText = DocumentationFilter(f->Value);
		rp->MesgText = ProgramFilter(f->Value);
	    }
	    else {
		rp->DocText = strdup(f->Value);
		rp->MesgText = strdup(f->Value);
	    }
	    free(f->Value);
	    break;
	}

    if ((rp->Flags & MTlongtext) == 0
     && strlen(rp->MesgText) > DCE_ERROR_STRING_SIZE)
	yyerror("Text is too long");
    if (rp->Attributes != NULL && rp->Subcomp == NULL)
	yyerror("Have \"attributes\" but not \"sub-component\" field");
    if (rp->Attributes == NULL && rp->Subcomp != NULL)
	yyerror("Have \"sub-component\" but not \"attributes\" field");

    /* If we got a "tables" field and the record could be in the
     * table, then only put it in the table if it is in the table
     * that we are generating. */
    if (rp->TableFlag == FALSE && (rp->Flags & MTintable) != 0)
	rp->Flags &= ~MTintable;
}


/*
**  Validate a record, making sure each field appears exactly once.
*/
static void ValidateRecord( int	Flags )
{
    recfield_t		*rp;
    msgfield_t		*f;
    int			i;
    char		*p;
    char		buff[SMBUF];
    int			bad;

    /* Reset counts. */
    for (rp = RecordFields; rp < ENDOF(RecordFields); rp++)
	rp->Count = 0;

    /* Make sure each field is a valid record. */
    for (bad = FALSE, i = CountMsgFields, f = MsgFields; --i >= 0; f++) {
	for (rp = RecordFields; rp < ENDOF(RecordFields); rp++)
	    if (f->Token == rp->Token) {
		if (rp->Count++ > 0) {
		    y2error("Duplicate \"%s\" field", rp->Name);
		    bad = TRUE;
		}
		break;
	    }
	/* Record not found in field list; this should not happen since
	 * yacc should not have recognized it! */
	if (rp == ENDOF(RecordFields)) {
	    (void)sprintf(buff, "Internal error -- unknown header %d",
		    f->Token);
	    yyerror(buff);
	    bad = TRUE;
	}
    }

    /* Make sure all fields were found. */
    for (rp = RecordFields; rp < ENDOF(RecordFields); rp++) {
	if (rp->Count != 0 || rp->Optional)
	    continue;
	if ((Flags & MTnodoc) != 0
	 && (rp->Token == tACTION || rp->Token == tEXPLANATION))
	    continue;
	y2error("Missing \"%s\" field", rp->Name);
	bad = TRUE;
    }

    if (!bad)
	StoreRecord(Flags);
}


static void CheckTextField(char	*text)
{
    static char		digits[] = "0123456789";
    char		buff[SMBUF];
    char		*p;
    char		*q;
    int			count;

    /* Don't require it if only one "%". */
    for (count = 0, p = text; *p; )
	if (*p++ == '%' && *p++ != '%')
	    count++;
    if (count < 2)
	return;

    /* Too many parameters for XPG4? */

    if (XPGCheck) {
	/* For all "%" make sure it is "%d$" where "d" is a digit. */
	for (count = 0, p = text; (p = strchr(p, '%')) != NULL; p++) {
	    q = strchr(digits, p[1]);
	    /*
	    if (q == NULL || q - digits > NL_ARGMAX || p[2] != '$') {
		yyerror("Illegal XPG/printf %d$ format.");
		break;
	    }
	   */
	}
    }
}
