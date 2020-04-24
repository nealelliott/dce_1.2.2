/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: sysdep.h,v $
 * Revision 1.2.13.2  1996/02/18  23:45:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:29  marty]
 *
 * Revision 1.2.13.1  1995/12/07  22:22:25  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/3  1995/11/22  01:53 UTC  psn
 * 	Fix for HP build.
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/2  1995/11/21  21:53 UTC  psn
 * 	Fix XIDL generated C++ stub suffixes for RIOS.
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/20  21:18 UTC  jrr
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/11/17  17:05 UTC  dat  /main/dat_xidl2/1]
 * 
 * 	oct 95 idl drop
 * 	[1995/10/23  00:44:34  bfc]
 * 	 *
 * 	may 95 idl drop
 * 	[1995/10/21  22:59:01  bfc]
 * 	 *
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:25:21  bfc]
 * 	 *
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/05  20:11 UTC  lmm
 * 	Merge fix for C++ compile line into Mothra
 * 
 * 	HP revision /main/HPDCE02/lmm_idlxx_fixes/1  1995/01/05  19:42 UTC  lmm
 * 	add +eh to CC command line for idl++ files
 * 
 * 	HP revision /main/HPDCE02/2  1994/11/01  23:56 UTC  lmm
 * 	Merging idl++ mods into Mothra
 * 
 * 	HP revision /main/HPDCE02/lmm_merge_idlxx/1  1994/11/01  23:52 UTC  lmm
 * 	Merging idl++ mods into Mothra
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:32 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 
 * 	HP revision /main/HPDCE01/3  1994/02/23  18:55  lmm
 * 	conditionalize location of cpp for HPUX 10.0
 * 
 * 	HP revision /main/HPDCE01/2  1994/01/24  22:22  tatsu_s
 * 	Submitted tatsu_s_ccase_fix_b0.
 * 	Allowed overriding of DEFAULT_*IDIR without defining *INCLUDE_*TEMPLATE.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/24  19:41  lmm
 * 	merge kk and hpdce01
 * 	[1995/12/07  21:13:14  root]
 * 
 * Revision 1.1.8.5  1993/11/19  19:10:37  sanders
 * 	Make version print "1.2 Lab Release"
 * 	Will fix this to be updated automatically before
 * 	final release
 * 	[1993/11/19  19:10:15  sanders]
 * 
 * Revision 1.1.8.4  1993/10/15  20:07:56  lmm
 * 	Merging OSF bug Fixes
 * 	[1993/10/15  20:07:19  lmm]
 * 
 * Revision 1.2.9.2  1993/10/05  21:38:39  ohara
 * 	the SVR4 version of ot number 8874
 * 	[1993/10/01  18:34:51  ohara]
 * 
 * Revision 1.2.9.1  1993/10/01  13:31:51  hinxman
 * 	OT 8874 - change design for spelling of #include generated from ACF include
 * 	statement
 * 	[1993/10/01  13:31:30  hinxman]
 * 
 * Revision 1.2.7.3  1993/07/07  20:04:03  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:34:58  ganni]
 * 
 * Revision 1.2.7.2  1993/06/24  20:09:14  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:38:00  hinman]
 * 
 * Revision 1.2.5.5  1993/01/07  22:17:25  hinxman
 * 	There are platforms on which IDL's mechanism for placement of stub object
 * 	files does not work.  There was already a mechanism in place in IDL to
 * 	issue the warning message:
 * 
 * 	    Object file for <filename> placed in current working directory
 * 
 * 	on such platforms; we merely needed to add _AIX as another such platform.
 * 	[1993/01/07  18:32:20  hinxman]
 * 
 * Revision 1.2.5.4  1993/01/03  22:11:51  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  14:38:32  bbelch]
 * 
 * Revision 1.2.5.3  1992/12/23  19:08:32  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:05:53  zeliff]
 * 
 * Revision 1.2.5.2  1992/09/29  20:41:24  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:35:21  weisman]
 * 
 * Revision 1.2.2.3  1992/04/14  12:38:46  mishkin
 * 	Don't #define OSF_LEX_YACC if APOLLO_LEX_YACC is defined (for HP OSF/1).
 * 	[1992/04/10  19:33:32  mishkin]
 * 
 * Revision 1.2.2.2  1992/03/24  14:14:00  harrow
 * 	Add AUTO_HEAP_STACK_THRESHOLD constant which determines
 * 	when to use heap instead of stack surrogates.  This allows
 * 	IDL to avoid overflowing the thread stack when large variables
 * 	would be allocated on the stack.
 * 
 * 	ANSI C prevents expected preprocessor symbol on VMS, so defined
 * 	VMS when __VMS is defined so the rest of IDL need not be modified.
 * 	[1992/03/16  22:05:34  harrow]
 * 
 * Revision 1.2  1992/01/19  22:13:26  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
 */

/*
 *  OSF DCE Version 1.0
 */

#ifndef sysdep_incl
#define sysdep_incl

/*
**
**  Copyright (c) 1989, 1990, 1992, 1993 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME
**
**      SYSDEP.H
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Operating system and compiler dependencies.
**
**  VERSION: DCE 1.0
**
*/

#if defined(__VMS) && !defined(VMS)
#define VMS
#define vms
#endif

/*
 *  exit status codes
 */
#ifdef VMS
#   define pgm_warning 0x10000000  /* 0 % 8 == warning */
#   define pgm_ok      0x00000001  /* 1 % 8 == success */
#   define pgm_error   0x10000002  /* 2 % 8 == error   */
#else
#   define pgm_ok      0
#   define pgm_warning 2
#   define pgm_error   3
#endif

/*
** Macro to test a system-specific status code for failure status.
*/
#ifdef VMS
#define ERROR_STATUS(s) (((s) & 1) != 1)
#else
#define ERROR_STATUS(s) ((s) != 0)
#endif

/*
** define HASDIRTREE if OS has foo/widget/bar file system.
** if HASDIRTREE, define BRANCHCHAR and BRANCHSTRING appropriately
** define HASPOPEN if system can do popen()
** define HASINODES if system has real inodes returned by stat()
*/

/* MSDOS */
#ifdef _MSDOS
#ifndef __STDC__
#define __STDC__ 1
#endif
#define BRANCHCHAR '\\'
#define BRANCHSTRING "\\"
#define HASPOPEN
#define HASDIRTREE
#define DEFAULT_IDIR     "."
#define DEFAULT_H_IDIR   "."
#define INCLUDE_TEMPLATE "#include <dce\\%s>\n"
#define USER_INCLUDE_TEMPLATE "#include \"%s\"\n"
#define USER_INCLUDE_H_TEMPLATE "#include \"%s.h\"\n"
#define MESSAGE_CATALOG_DIR "\\bin"
#define CD_IDIR "."
#define unlink _unlink
#define getcwd _getcwd
#define chdir _chdir
#define stat _stat
#define popen _popen
#define pclose _pclose
#define S_IFMT _S_IFMT
#define S_IFDIR _S_IFDIR
#define S_IFREG _S_IFREG
#define NO_TRY_CATCH_FINALLY
#ifdef TURBOC
#define stat(a,b) turboc_stat(a,b)
#endif
#endif


/* VAX VMS  */
#ifdef VMS
#define HASDIRTREE
#define BRANCHCHAR ']'
#define BRANCHSTRING "]"
/* VMS model is that system .IDL and .H files are in the same directory. */
#define DEFAULT_IDIR    "DCE:"
#define DEFAULT_H_IDIR  "DCE:"
#define INCLUDE_TEMPLATE "#include <dce/%s>\n"
#define USER_INCLUDE_TEMPLATE "#include <%s>\n"
#define USER_INCLUDE_H_TEMPLATE "#include <%s.h>\n"
#define CD_IDIR "[]"
#endif

#ifdef SNI_SVR4
# define DEFAULT_IDIR "/opt/include"
# define DEFAULT_H_IDIR "/opt/include"
# define INCLUDE_TEMPLATE "#include <dce/%s>\n"
# define USER_INCLUDE_TEMPLATE "#include <%s>\n"
#define USER_INCLUDE_H_TEMPLATE "#include <%s.h>\n"
#elif defined(SNI_SVR4V3)
# define DEFAULT_IDIR "/opt/dcelocal/share/include"
# define DEFAULT_H_IDIR "/opt/dcelocal/share/include"
# define INCLUDE_TEMPLATE "#include <dce/%s>\n"
# define USER_INCLUDE_TEMPLATE "#include <%s>\n"
#define USER_INCLUDE_H_TEMPLATE "#include <%s.h>\n"
#endif

#if defined(__OSF__) || defined(__OSF1__) || defined(__osf__) || defined(BSD) || defined(SYS5) || defined(ultrix) || defined(_AIX) || defined(__ultrix) || defined(_BSD) || defined(SNI_SVR4) || defined(SNI_UMIPS)
#define UNIX
#define HASDIRTREE
#define HASPOPEN
#define HASINODES
#define BRANCHCHAR '/'
#define BRANCHSTRING "/"
#define CD_IDIR "."
#endif

#ifndef CD_IDIR
Porting Message:  You must provide definitions for the symbols
    describing the directory structure available on your platform.  
#endif

/*
 * Default DCE include directory
 */
#ifndef DEFAULT_IDIR
# define DEFAULT_IDIR "/usr/include"
#endif
#ifndef DEFAULT_H_IDIR
# define DEFAULT_H_IDIR "/usr/include"
#endif
#ifndef INCLUDE_TEMPLATE
# define INCLUDE_TEMPLATE "#include <dce/%s>\n"
#endif
#ifndef USER_INCLUDE_TEMPLATE
# define USER_INCLUDE_TEMPLATE "#include <%s>\n"
#endif
#ifndef USER_INCLUDE_H_TEMPLATE
# define USER_INCLUDE_H_TEMPLATE "#include <%s.h>\n"
#endif

/*
 * Default DCE auto import path
 */
#ifndef AUTO_IMPORT_FILE
# define AUTO_IMPORT_FILE "dce/nbase.idl"
#endif

/*
** Default filetype names.
*/
#if defined(VMS) || defined(_MSDOS)
#define OBJ_FILETYPE ".OBJ"
#else
#define OBJ_FILETYPE ".o"
#endif

/*
** Default command to invoke C preprocessor.
*/
#ifdef UNIX
# define CPP_BY_DEFAULT
# ifdef apollo
#  define CPP "/usr/lib/cpp "
# elif defined(SNI_SVR4)
#  define CPP "/usr/ccs/lib/cpp"
# elif __hpux1000p
#  define CPP "/opt/langtools/lbin/cpp"
# else
#  define CPP "/lib/cpp "
# endif
#endif

#ifdef VMS
#ifdef __alpha
# define CPP "CC/NOWARN/STANDARD=VAX"
#else
# define CPP "CC/NOWARN"
# define CPP_DECC "CC/NOWARN/STANDARD=VAX"
#endif
#endif

#ifdef _MSDOS
# define CPP_BY_DEFAULT
#define CPP "cl /nologo /E "
#endif

/*
** Default suffixes for IDL-generated files.
*/
#if defined(UNIX) || defined(VMS) || defined(_MSDOS)
# define CSTUB_SUFFIX   "_cstub.c"
# define SSTUB_SUFFIX   "_sstub.c"
# define HEADER_SUFFIX  ".h"
# if defined(_AIX) || defined(__hpux)
#    define CSTUB_SUFFIX_CXX   "_cstub.C"
#    define SSTUB_SUFFIX_CXX   "_sstub.C"
# else
#    define CSTUB_SUFFIX_CXX   "_cstub.cxx"
#    define SSTUB_SUFFIX_CXX   "_sstub.cxx"
# endif
# define CAUX_SUFFIX    "_caux.c"
# define SAUX_SUFFIX    "_saux.c"
#endif

#if defined(_MSDOS)
# define CSTUB_SHORT_SUFFIX   "_c.c"
# define SSTUB_SHORT_SUFFIX   "_s.c"
# define CSTUB_SHORT_SUFFIX_CXX   "_c.cxx"
# define SSTUB_SHORT_SUFFIX_CXX   "_s.cxx"
# define CAUX_SHORT_SUFFIX    "_x.c"
# define SAUX_SHORT_SUFFIX    "_y.c"
#endif

#ifndef CSTUB_SUFFIX
Porting Message:  You must provide definitions for the files suffixes to
    be used on your platform.
#endif

/*
 * Template for IDL version text emitted as comment into generated files.
 */
#ifndef IDL_VERSION_TEXT 
# if defined(__hpux)
#   define _HP_INSTRUMENTED_IDL_ID
#  if !defined(_HP_DCE_VERSION_ID)
#   define _HP_DCE_VERSION_ID "HP DCE/9000 1.4 Release"
#  endif
#  if defined(_HP_DCE_BUILD_ID)
#   define _HP_DCE_BUILD_ID_IDL " " _HP_DCE_BUILD_ID
#  else
#   define _HP_DCE_BUILD_ID_IDL
#  endif
#  if defined(_HP_DCE_BUILD_DATE)
#   define _HP_DCE_BUILD_DATE_IDL " Date: " _HP_DCE_BUILD_DATE
#  else
#   define _HP_DCE_BUILD_DATE_IDL " Date: " __DATE__ " " __TIME__
#  endif
#  define IDL_VERSION_TEXT _HP_DCE_VERSION_ID _HP_INSTRUMENTED_IDL_ID \
          _HP_DCE_BUILD_ID_IDL _HP_DCE_BUILD_DATE_IDL
# else
#  define IDL_VERSION_TEXT "OSF DCE T1.2.0-09"
# endif /* defined(__hpux) */
#endif

#define IDL_VERSION_TEMPLATE "/* Generated by IDL compiler version %s */\n"

/*
** Default C compiler command and options.  CC_OPT_OBJECT is not defined for
** some unix platforms since "cc -c -o file.o file.c" does not work as expected.
*/
#ifdef UNIX
# if defined(vax) && defined(ultrix)
#  define CC_DEF_CMD "cc -c -Mg"
# elif defined(__hp9000s800)
#  define CC_DEF_CMD "c89 -D_HPUX_SOURCE -c"
# elif defined(SNI_SVR4)
#  define CC_DEF_CMD "/opt/bin/dce/dce_cc -c"
# elif defined(SNI_SVR4V3)
#  define CC_DEF_CMD "/opt/dcelocal/bin/cc -c"
# else
#  define CC_DEF_CMD "cc -c"
# endif
# if !(defined(vax) && defined(ultrix)) && !defined(apollo) && !defined(_AIX)
#  define CC_OPT_OBJECT "-o "
# endif
#endif

/*
** Default C++ compiler command and options.
*/
#ifdef UNIX
# if defined(_AIX)
#  define CXX_DEF_CMD "xlC -c"
# elif defined(__hpux)
#  define CXX_DEF_CMD "CC -D_HPUX_SOURCE -c"
# else
#  define CXX_DEF_CMD "cxx -c"
# endif
#endif

#ifdef VMS
# define CC_DEF_CMD "CC/G_FLOAT"
# define CC_OPT_OBJECT "/OBJECT="
# define CXX_DEF_CMD "CXX/G_FLOAT"
#endif

#ifdef _MSDOS
# define CC_DEF_CMD "cl /nologo /c "
# define CC_OPT_OBJECT "/Fo"
# define CXX_DEF_CMD "cl /nologo /c "
#endif

/*
** PASS_I_DIRS_TO_CC determines whether the list of import directories, with
** the system IDL directory replaced by the system H directory if present,
** gets passed as command option(s) to the C compiler when compiling stubs.
*/
#ifndef apollo
# define PASS_I_DIRS_TO_CC
#endif

/*
** Environment variables for IDL system include directories
** on supported platforms.
*/
#ifdef DUMPERS
# define NIDL_LIBRARY_EV "NIDL_LIBRARY"
#endif

/*
** Environment variables for IDL system include directories
** on NT.
*/
#if defined(_MSDOS)
#define DCE_INCLUDE_EV "DCELOC"
#define DCE_INCLUDE_DIR "\\DCE\\INCLUDE"
#endif

/*
** Maximum length of IDL identifiers.  Architecturally specified as 31, but
** on platforms whose C (or other) compilers have more stringent lengths,
** this value might have to be less.
*/
#define MAX_ID 31

/*
** Estimation of available stack size in a server stub.  Under DCE threads
** stack overflow by large amounts can result in indeterminant behavior.  If
** the estimated stack requirements for stack surrogates exceeds the value
** below, objects are allocated via malloc instead of on the stack.
*/
#define AUTO_HEAP_STACK_THRESHOLD 7000

/*
** Symbol for 'audible bell' character.  A workaround for the problem that
** some non-stdc compilers incorrectly map '\a' to 'a'.  Might need work
** on a non-stdc EBCDIC platform.
*/
#if defined(__STDC__)
#define AUDIBLE_BELL '\a'
#define AUDIBLE_BELL_CSTR "\\a"
#else
#define AUDIBLE_BELL '\007'
#define AUDIBLE_BELL_CSTR "\\007"
#endif

/*
** Data type of memory returned by malloc.  In ANSI standard compilers, this
** is a void *, but default to char * for others.
*/
#if defined(__STDC__) || defined(vaxc)
#define heap_mem void
#else
#define heap_mem char
#endif

/*
**  Maximum number of characters in a directory path name for a file.  Used
**  to allocate buffer space for manipulating the path name string.
*/
#ifndef PATH_MAX
# ifdef VMS
# define PATH_MAX  256
# else
# define PATH_MAX 1024
# endif
#endif

/*
** Define PROTO if the system compiler supports ANSI prototypes
*/
#ifndef PROTO
#if defined __STDC__ || defined VAXC || defined mips || defined _AIX || defined __alpha
#define PROTO
#endif
#endif

/*
** Define macros for NLS entry points used only in message.c
*/
#if defined(_AIX)
#       define NL_SPRINTF NLsprintf
#       define NL_VFPRINTF NLvfprintf
#else
#       define NL_SPRINTF sprintf
#       define NL_VFPRINTF vfprintf
#endif

/*
** Define symbols to reference variables of various YACC implementations
*/
#if defined(ultrix) || defined(__ultrix) || defined(VMS) || defined(_MSDOS)
#define ULTRIX_LEX_YACC
#endif

#if defined(__hp9000s300) || defined(__hp9000s800)
#define HPUX_LEX_YACC
#endif

#if defined(apollo)
#define APOLLO_LEX_YACC
#endif

#if (defined(__OSF__) || defined(__OSF1__)) && !defined(APOLLO_LEX_YACC)
#define OSF_LEX_YACC
#endif

#if defined(_AIX) || defined(__osf__)
#define AIX_LEX_YACC
#define AIX_YACC_VAR extern
#endif

#if defined(SNI_SVR4)
#define SVR4_LEX_YACC
#endif

#if defined(SNI_UMIPS)
#define UMIPS_LEX_YACC
#endif

/*
 * On SunOS 4.1 platforms, we define an additional constant to
 * distingquish changes between SunOS 4.0 and SunOS 4.1.
 */
#if defined(sun) && defined(sparc)
# define SUN_LEX_YACC
# if defined(__sunos_41__)      /* to distinguish 4.0 from 4.1 */
#  define SUN_41_LEX_YACC
# endif
#endif

/*
 * These tricks let us share lex/yacc macros across certain implementations,
 * even though they might be different.  YACC_VAR is used to handle the
 * situation where a lex/yacc variable is local or nonexistent data in one
 * implementation, but global in others.  YACC_INT is used to handle the
 * situation where a lex/yacc variable has data type short in one
 * implementation, but data type int in others.  AIX_YACC_VAR is to handle
 * yacc variables that only exist in AIX.
 */
#ifndef YACC_INT
#if defined (OSF_LEX_YACC)
#define YACC_VAR
#define YACC_INT int
#elif defined(ULTRIX_LEX_YACC) || (defined(SUN_LEX_YACC) && !defined(SUN_41_LEX_YACC))
#define YACC_VAR
#define YACC_INT short
#else /* SUN_41_LEX_YACC || AIX_LEX_YACC || APOLLO_LEX_YACC */
#define YACC_VAR extern
#define YACC_INT int
#endif
#endif


#if defined(OSF_LEX_YACC) || defined(ULTRIX_LEX_YACC) || defined(APOLLO_LEX_YACC) || defined(AIX_LEX_YACC) || defined(SUN_LEX_YACC) || defined(HPUX_LEX_YACC) || defined(SVR4_LEX_YACC) || defined(UMIPS_LEX_YACC)

/*
 * The constants below are defined by the output of LEX.  They are not
 * made available in an include file, and are therefore duplicated here.
 * Be very careful!! THESE DEFINITION MUST TRACK THOSE IN THE LEX GENERATED
 * MODULE!!!
 */

#ifndef YYLMAX
#if defined(SVR4_LEX_YACC)
#define YYLMAX 200
#else
#define YYLMAX 1024
#endif
#endif

#ifndef YYTYPE
#ifndef PROCESSING_LEX  /* Avoid doing if compiling lex-generated source */
#define YYTYPE int      /* Warning: if grammar shrinks, YYTYPE can be char! */
#endif
#endif

/*
 * The constants below are defined by the output of YACC.  They are not
 * made available in an include file, and are therefore duplicated here.
 * Be very careful!! THESE DEFINITION MUST TRACK THOSE IN THE YACC GENERATED
 * MODULE!!!
 */
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif

/*
 * Macro to declare all the external cells we are responsible for saving.
 * The non-extern struct declarations below are defined by the output of LEX
 * or YACC as indicated by the comment.  They are not made available in an
 * include file, and are therefore duplicated here.  Be very careful!!
 * THESE DEFINITION MUST TRACK THOSE IN THE LEX/YACC GENERATED MODULE!!!
 */
#define LEX_YACC_EXTERNS        \
extern int      yyprevious;     \
extern char *   yysptr;               \
extern char     yysbuf[];      \
extern int      yynerrs

/*
 * These are variables that are used by some lex or yacc's, but are
 * not current required.
 */
#ifdef notdef
struct yywork { YYTYPE verify, advance; };  /* lex */\
struct yysvf {                              /* lex */\
        struct yywork *yystoff;\
        struct yysvf *yyother;\
        int *yystops;};\
extern struct yysvf *yybgin;\
extern int          yychar;\
extern YACC_INT     yyerrflag;\
extern struct yysvf *yyestate;\
extern int          *yyfnd;\
extern FILE         *yyin;\
extern int          yyleng;\
extern int          yylineno;\
extern struct yysvf **yylsp;\
extern struct yysvf *yylstate;\
extern YYSTYPE      yylval;\
extern int          yymorfg;\
extern int          yynerrs;\
extern struct yysvf **yyolsp;\
extern int          yyprevious;\
YACC_VAR int        *yyps;\
YACC_VAR YYSTYPE    *yypv;\
AIX_YACC_VAR YYSTYPE    *yypvt;\
YACC_VAR YACC_INT   yys[ YYMAXDEPTH ];\
extern char         yysbuf[];\
extern char         *yysptr;\
YACC_VAR YACC_INT   yystate;\
extern int          yytchar;\
extern char         yytext[];\
YACC_VAR int        yytmp;\
extern YYSTYPE      yyv[];\
extern YYSTYPE      yyval
#endif


/*
** This is an opaque pointer to the real state, which is allocated by
** the NIDL_save_lex_yacc_state() routine.
*/
typedef char *lex_yacc_state_t;

extern lex_yacc_state_t NIDL_save_lex_yacc_state(
#ifdef PROTO
    void
#endif
);
extern void NIDL_restore_lex_yacc_state(
#ifdef PROTO
    lex_yacc_state_t state_ptr
#endif
);

#define LEX_YACC_STATE_BUFFER(name) lex_yacc_state_t name

#define SAVE_LEX_YACC_STATE(x)  ((x) = NIDL_save_lex_yacc_state())

#define RESTORE_LEX_YACC_STATE(x) NIDL_restore_lex_yacc_state(x)

#endif /* a known lex/yacc machine is defined */



#ifndef LEX_YACC_STATE_BUFFER
Porting Message:  Due to differences between implementations of the
      lex and yacc tools, different state variables must be saved
      in order to perform multiple parses within a single program
      exectuion.  Either enable one of the LEX_YACC sets above on this
      architecture or add an additional set of macros to save/restore
      the variables used by lex and yacc.  This is done via inspection
      of the generated lex/yacc output files for any non-automatic
      state variables.  You might also need to make additions to the
      file acf.h depending on your implementation of lex/yacc.  See
      the comments in acf.h for more information.
#endif
#endif

/*
** To avoid passing the VMS C compiler lines of more than 255 characters, we
** redirect fprintf through a special routine, use on other Digital platforms
** also for consistancy in testing of compiler output.
*/
#if defined(vms) || defined(ultrix) || defined(__osf__)
#define IDL_USE_OUTPUT_LINE
#ifdef fprintf
#undef fprintf
#endif
#define fprintf output_line

/*
** Functions exported by sysdep.c
*/
extern int output_line(
#ifdef PROTO
    FILE * fid,                 /* [in] File handle */
    char *format,               /* [in] Format string */
    ...                         /* [in] 0-N format arguments */
#endif
);

extern void flush_output_line(
#ifdef PROTO
    FILE * fid                  /* [in] File handle */
#endif
);
#endif

