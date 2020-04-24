/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_stdio.h $Revision: 1.1.6.1 $ $Date: 1996/05/10 13:28:31 $
 */
/*
 * HISTORY
 * $Log: cma_stdio.h,v $
 * Revision 1.1.6.1  1996/05/10  13:28:31  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:51 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:48 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  18:23  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  15:08:34  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:23  root  1.1.8.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:14:08  hopkins]
 *
 * 	Inserted "const" where required, based on a comparison with /usr/include/stdio.h
 * 	on HP_UX and OSF/1.
 * 	[1993/08/17  22:02:19  rhw]
 *
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:17:54  cjd]
 *
 * 	Initial 486 port.
 * 	[1993/05/11  21:44:29  jd]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:50:48  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:35:13  hopkins]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:35:34  mgm  1.1.1.11]
 *
 * Revision 1.1.2.2  1994/06/09  13:34:43  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:35  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/01  20:13:46  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:03:12  jd]
 * 
 * Revision 1.2.12.3  1993/08/17  22:44:09  rhw
 * 	Inserted "const" where required, based on a comparison with /usr/include/stdio.h
 * 	on HP_UX and OSF/1.
 * 	[1993/08/17  22:02:19  rhw]
 * 
 * Revision 1.2.12.2  1993/05/24  20:50:27  cjd
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:17:54  cjd]
 * 
 * Revision 1.2.10.2  1993/05/12  13:49:07  jd
 * 	Initial 486 port.
 * 	[1993/05/11  21:44:29  jd]
 * 
 * Revision 1.2.5.6  1993/02/04  17:13:14  sommerfeld
 * 	[OT 7129] Fix typo in compare against _CMA__SVR4
 * 	[1993/02/04  16:59:11  sommerfeld]
 * 
 * Revision 1.2.5.5  1993/02/01  22:19:30  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:32:24  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:21:53  hinman]
 * 
 * Revision 9.5.1.13  93/01/08  11:25:58  devsrc
 * 	[raj] Fix copyright  notice
 * 
 * Revision 9.5.1.12  92/12/17  13:55:45  raj
 * 	[raj] New naming + parallelized reentrant library changes.
 * 
 * Revision 9.5.6.2  92/11/06  11:15:09  raj
 * 	[raj] Changes for new naming scheme with reentrant libraries.
 * 
 * Revision 9.5.1.11  92/11/24  08:56:01  blurie
 * 	Fix up merge error before setlinebuf():
 * 
 * 		#  if !defined(__hpux) && (!(_CMA_UNIX_TYPE == _CMA__SVR4))
 * 
 * 	should be:
 * 
 * 		#  if !defined(__hpux) && (_CMA_UNIX_TYPE != _CMA__SVR4)
 * 	[92/11/24  08:51:02  blurie]
 * 
 * Revision 9.5.1.10  92/10/01  13:09:40  raj
 * 	[raj] Fix 1.0.2 merge inconsitencies
 * 
 * Revision 9.5.5.2  92/10/01  13:08:32  raj
 * 	[raj] Fix 1.0.2 merge inconsitencies
 * 
 * Revision 9.12.1.2  92/09/30  10:36:42  hinman
 * 	Auto checkin of OSF rev 1.2.5.3
 * 
 * Revision 1.2.5.3  1992/09/29  20:15:46  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:54:42  sekhar]
 * 
 * Revision 1.2.5.2  1992/09/03  14:40:40  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:06:34  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:22:46  bolinger
 * 	Bring forward 1.0.1 revision 1.2.3.2 (fix for OT defect 4668).
 * 	[1992/08/10  16:23:37  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:57:00  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:56:44  bolinger]
 * 
 * Revision 1.2.3.2  1992/07/15  22:51:36  bolinger
 * 	Fix OT defect 4668:  for OSF/1 only, add new cma_system()
 * 	wrapper and extend existing cma_popen() wrapper so that
 * 	process-virtual itimer is disabled across the internal
 * 	fork() that popen() and system() each do.
 * 	[1992/07/15  22:42:28  bolinger]
 * 
 * Revision 1.2  1992/01/19  22:14:47  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
 */

/*
 *  OSF DCE Version 1.0 
 */
/*
 *  Copyright (c) 1990, 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and  copied
 *  only  in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any  other
 *  copies  thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is  hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without  notice
 *  and  should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	Concert Multhithread (tm) Architecture
 *
 *  ABSTRACT:
 *
 *	Header file for example jacket routines which make the selected
 *	C run-time library routines thread reentrant.  These jackets are
 *	transparent to the calling code, so that, when a reentrant version
 *	of the library becomes available, they can be dispensed with without
 *	requiring changes to the calling code.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	29 June 1990
 *
 *  MODIFICATION HISTORY:
 * 
 *	001	Paul Curtin	26 October 1990
 *		Added if's to scanf functions, dependent upon (v)ariable
 * 		versions.
 *	002	Paul Curtin	19 November 1990
 *		Conditionally removed protos on vms for; cma_pclose, cma_popen,
 *		cma_setbuffer, cma_setlinebuf, cma_tempnam, and cma_ttyslot
 *	003	Paul Curtin	21 November 1990
 *		Rearranged conditional exclusion
 *	004	Paul Curtin	31 January 1991
 *		Removed conditional types for cma prototypes, to match `man'
 * 		pgs.
 *	005	Paul Curtin	25 February 1991
 * 		Conditionalized *scanf* defines, because the wrapper 
 *		routines are dependent upon v*scanf versions that are 
 *		not currently available.
 *	006	Paul Curtin	23 April 1991
 *		Changed the parameter name template to filespec for cma_mktemp.
 *	007	Dave Butenhof	27 August 1991
 *		Fix v*printf functions so they'll work.
 *	008	Dave Butenhof	04 September 1991
 *		Include stdarg.h or varargs.h to define the va_list type used
 *		in the vprintf functions.
 *	009	Dave Butenhof	18 September 1991
 *		Integrate Apollo CMA5 reverse drop: fix putchar() macro to
 *		output to stdout rather than stdin.
 *	010	Dave Butenhof	19 September 1991
 *		Integrate HPUX CMA5 reverse drop: exclude wrappers if
 *		_POSIX_REENTRANT_FUNCTIONS, and exclude several wrappers
 *		(for unimplemented functions) anyway for HPUX.
 *	011	Dave Butenhof	01 October 1991
 *		Fix cma_ungetc() prototype to match ANSI C (c is "int", not
 *		"char").
 *	012	Dave Butenhof	03 October 1991
 *		Exclude wrappers if _CMA_THREAD_SYNC_IO_ is defined.
 *	013	Dave Butenhof	19 November 1991
 *		Remove direct check for _POSIX_REENTRANT_FUNCTIONS; rely on
 *		_CMA_REENTRANT_CLIB_, since we control that. OSF/1 defines
 *		_POSIX_REENTRANT_FUNCTIONS, but OSF DCE reference port isn't
 *		using libc_r.a.
 */


#ifndef CMA_STDIO
#define CMA_STDIO

/*
 *  INCLUDE FILES
 */

# include <dce/cma.h>
#include <stdio.h>
#include <limits.h>
#include <nl_types.h>

# include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * MACROS
 */

/*
 * C Run-time Library "Standard I/O" Routine Wrappers
 */
#if !_CMA_REENTRANT_CLIB_
# if !defined(_CMA_NOWRAPPERS_) && _CMA_UNIPROCESSOR_
# define ctermid	cma_ctermid 
# define cuserid	cma_cuserid 
# define fclose		cma_fclose 
# define fflush		cma_fflush 
# define fdopen		cma_fdopen 
# define fgetc		cma_fgetc 
# define fgets		cma_fgets 
# define fopen		cma_fopen 
# define fprintf	cma_fprintf 
# define fputc		cma_fputc 
# define fputs		cma_fputs 
# define fread		cma_fread 
# define freopen	cma_freopen 
# define fseek		cma_fseek 
# define ftell		cma_ftell 
# define fwrite		cma_fwrite 
# define gets		cma_gets 
# define getw		cma_getw 
# define isatty		cma_isatty 
# define mktemp		cma_mktemp 
# define printf		cma_printf 
# define puts		cma_puts 
# define putw		cma_putw 
# define rewind		cma_rewind 
# define setbuf		cma_setbuf 
# define setvbuf	cma_setvbuf 
# define sprintf	cma_sprintf 
# define tmpfile	cma_tmpfile 
# define tmpnam		cma_tmpnam 
# define ttyname	cma_ttyname 
# define vfprintf	cma_vfprintf
# define vprintf	cma_vprintf
# define vsprintf	cma_vsprintf

# if _CMA_VSSCANF_
#  define scanf		cma_scanf 
#  define fscanf	cma_fscanf 
#  define sscanf	cma_sscanf 
# endif

# define pclose	cma_pclose 
# define popen		cma_popen 
# define tempnam	cma_tempnam 
# define ttyslot	cma_ttyslot

# undef	getc
# define	getc	cma_fgetc

# undef	getchar
# define getchar()	cma_fgetc (stdin)

# undef putc
# define putc		cma_fputc

# undef	putchar
# define putchar(c)	cma_fputc (c, stdout)
#endif  /*!(_CMA_NOWRAPPERS_) && _CMA_UNIPROCESSOR_*/
#endif	/* !_CMA_REENTRANT_CLIB_ */

/*
 * Other functions that need wrappers, at least for now.
 */


/*
 * INTERFACES
 */
#if !_CMA_REENTRANT_CLIB_
extern char *cma_ctermid (char *);
extern char *cma_cuserid (char *);
extern int  cma_fclose (FILE *);
extern int  cma_fflush (FILE *);
extern FILE *cma_fdopen (int, const char *);
extern int  cma_fgetc (FILE *);
extern char *cma_fgets (char *, int,FILE *);
extern FILE *cma_fopen (const char *,const char	*);
extern int  cma_fprintf (FILE *,const char *,...);
extern int  cma_fputc (int, FILE *);
extern int  cma_fputs (const char *,FILE *);
extern int  cma_fread (void *, size_t, size_t ,FILE *);
extern FILE *cma_freopen (const char *, const char *, FILE *);
#if _CMA_VSSCANF_
extern int  cma_fscanf (FILE *,	const char *,...);
#endif
extern int  cma_fseek (FILE *, long, int );
extern long cma_ftell (FILE *);
extern int  cma_fwrite (const void *,size_t ,size_t ,FILE *);
extern char *cma_gets (char *);
extern int  cma_getw (FILE *);
extern int  cma_isatty (int); /* none on HP800 */
extern char *cma_mktemp (char *); /* none on HP800 */
extern int  cma_pclose (FILE *);
extern FILE *cma_popen (const char *,const char *);
extern int  cma_printf (const char *,...);
extern int  cma_puts (const char *);
extern int  cma_putw (int ,FILE	*);
extern void cma_rewind (FILE *);
#if _CMA_VSSCANF_
extern int  cma_scanf (const char*,...);
#endif
extern void cma_setbuf (FILE*,char *);
extern int  cma_setvbuf (FILE *,char *,int, size_t);

extern int cma_sprintf (char *, const char *,...);

#if _CMA_VSSCANF_
extern int  cma_sscanf (const char *,const char *,...);
#endif
extern char *cma_tempnam (const char *,const char *);
extern FILE *cma_tmpfile (void);
extern char *cma_tmpnam (char *);
extern char *cma_ttyname (int); /* None on HP800 */
extern int  cma_ttyslot (void); /* None on HP800 */
extern char cma_ungetc (int ,FILE *);
extern int  cma_vfprintf (FILE *,const char *,__va_list);
extern int  cma_vprintf (const char *,__va_list);
extern int  cma_vsprintf (char *,const char *,__va_list);

#endif /* _CMA_REENTRANT_CLIB_ */

extern nl_catd cma_catopen (const char *,int );
extern char *cma_catgets (nl_catd ,int ,int , const char *);
extern int  cma_catclose (nl_catd);

extern int  cma_system (const char *);

#ifdef __cplusplus
}
#endif

#endif
