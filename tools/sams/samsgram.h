/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
#line 106 "samsgram.y" /* yacc.c:1921  */

    int			Number;
    char		*String;
    struct msgfield_s_t	Field;
    struct svcfield_s_t	Svc;

#line 135 "y.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
