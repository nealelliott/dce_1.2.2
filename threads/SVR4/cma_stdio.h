/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: cma_stdio.h,v $
 * Revision 1.1.4.2  1996/02/18  23:09:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:33:23  marty]
 *
 * Revision 1.1.4.1  1995/12/07  21:56:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:37  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:52:49  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:49:49  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/14  21:31:53  ohara
 * 	moved again
 * 	[1994/04/14  20:49:36  ohara]
 * 
 * Revision 1.1.2.1  1994/04/01  20:14:51  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  18:30:46  jd]
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

#include <dce/cma.h>

#include <stdio.h>
#include <limits.h>
#include <nl_types.h>

#include <stdarg.h>

/*
 * MACROS
 */

/*
 * C Run-time Library "Standard I/O" Routine Wrappers
 */
#if !_CMA_REENTRANT_CLIB_

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

#  define pclose	cma_pclose 
#  define popen		cma_popen 
#  define tempnam	cma_tempnam 
#  define ttyslot	cma_ttyslot

# undef	getc
# define	getc	cma_fgetc

# undef	getchar
# define getchar()	cma_fgetc (stdin)

# undef putc
# define putc		cma_fputc

# undef	putchar
# define putchar(c)	cma_fputc (c, stdout)

#define catopen        cma_catopen
#define catgets        cma_catgets
#define catclose       cma_catclose

# define	system	cma_system
# define 	popen	cma_popen
#endif	/* !_CMA_REENTRANT_CLIB_ */

#endif






