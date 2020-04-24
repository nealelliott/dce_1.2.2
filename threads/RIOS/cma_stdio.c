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
 * HISTORY
 * $Log: cma_stdio.c,v $
 * Revision 1.1.6.1  1996/05/10  13:31:19  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/04/26  19:53 UTC  bissen
 * 	Merge single-threaded changes
 *
 * 	HP revision /main/DCE_1.2/bissen_122_work/1  1996/04/26  19:49 UTC  bissen
 * 	Fix wrappers that use cma_[un]import_fd
 * 	[1996/05/09  21:02:00  arvind]
 *
 * 	Move latest 1.0.1-based revision into 1.0.2.
 * 	[1992/08/28  20:42:41  bolinger]
 *
 * Revision 1.1.2.2  1994/06/09  13:35:41  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:14  devsrc]
 * 
 * 	Bring forward IBM integration changes to BL10 sources.
 * 	[1992/08/28  14:14:14  bolinger]
 * 
 * 	Bring forward 1.0.1 revision 1.2.4.2 (fix for OT defect 2413),
 * 	1.2.4.3 (fix for 1711), 1.2.4.4 (fix for 4668).
 * 	[1992/08/10  16:15:39  bolinger]
 * 
 * Revision 1.1.2.1  1994/04/01  20:14:18  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:53:00  jd]
 * 
 * Revision 1.2.16.5  1993/08/17  22:37:12  rhw
 * 	Added call to cma_unimport_fd in cma_popen.
 * 	[1993/08/17  22:06:36  rhw]
 * 
 * Revision 1.2.16.4  1993/08/05  18:38:07  rhw
 * 	set cma_sscanf to return whatever vsscanf returns.
 * 	[1993/08/05  18:05:31  rhw]
 * 
 * Revision 1.2.16.3  1993/06/24  21:03:53  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:45:22  hinman]
 * 
 * Revision 1.2.16.2  1993/05/24  20:50:18  cjd
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:17:36  cjd]
 * 
 * Revision 1.2.14.2  1993/05/11  22:02:29  jd
 * 	Initial 486 port.
 * 	[1993/05/11  21:44:15  jd]
 * 
 * Revision 1.2.6.7  1993/03/29  20:22:00  jd
 * 	Have cma_fdopen check to see if cma already knows about the file before
 * 	calling cma_import_fd. This will avoid an unnecessary warning from
 * 	cma__open_general before it makes exactly the same check and returns.
 * 	[1993/03/25  18:35:58  jd]
 * 
 * Revision 1.2.6.6  1993/02/05  22:59:12  jd
 * 	Conditionalized calls to cma_import_fd and cma_unimport_fd on
 * 	_CMA_THREAD_SYNC_IO_ not being set.
 * 	[1993/02/05  22:58:40  jd]
 * 
 * Revision 1.2.6.5  1993/02/01  22:19:18  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:32:09  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:21:36  hinman]
 * 
 * Revision 9.5.1.12  93/01/08  11:27:03  devsrc
 * 	[raj] Fix copyright  notice
 * 
 * Revision 9.5.1.11  92/12/18  16:27:55  blurie
 * 	Reentrant library problems.
 * 
 * 	cma_popen() should not cma_import_fd() anymore after the popen() with
 * 	the new reentrant libraries.
 * 	[92/12/18  16:27:16  blurie]
 * 
 * Revision 9.5.1.10  92/12/17  13:45:26  raj
 * 	[raj] New naming + parallelized reentrant library changes.
 * 
 * Revision 9.5.2.2  92/11/06  11:14:12  raj
 * 	[raj] Changes for new naming scheme with reentrant libraries.
 * 
 * Revision 9.5.1.9  92/11/24  08:55:47  blurie
 * 	Fix up merge error.  Before setlinebuf() was:
 * 
 * 		#if _CMA_OS_ != _CMA__VMS && _CMA_VENDOR_ != _CMA__HP && (_CMA_UNIX_TYPE == _CMA__SVR4)
 * 
 * 	It should be:
 * 
 * 		 #if _CMA_OS_ != _CMA__VMS && _CMA_VENDOR_ != _CMA__HP && (_CMA_UNIX_TYPE != _CMA__SVR4)
 * 	[92/11/24  08:50:05  blurie]
 * 
 * Revision 9.5.1.7  92/09/30  14:11:05  root
 * 	Acceptance of OSF rev 1.2.6.3
 * 
 * Revision 9.12.1.2  92/09/30  10:36:29  hinman
 * 	Auto checkin of OSF rev 1.2.6.3
 * 
 * Revision 1.2.6.3  1992/09/29  20:15:43  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:53:52  sekhar]
 * 
 * Revision 1.2.6.2  1992/09/03  14:40:33  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:06:28  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:56:09  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:55:46  bolinger]
 * 
 * Revision 1.2.4.4  1992/07/15  22:51:27  bolinger
 * 	Fix OT defect 4668:  for OSF/1 only, add new cma_system()
 * 	wrapper and extend existing cma_popen() wrapper so that
 * 	process-virtual itimer is disabled across the internal
 * 	fork() that popen() and system() each do.
 * 	[1992/07/15  22:44:25  bolinger]
 * 
 * Revision 1.2.4.3  1992/05/13  19:56:58  bolinger
 * 	Fix OT defect 1711:  fix wrapper signatures to match POSIX/ANSI/XPG3
 * 	specs.
 * 	[1992/05/12  14:02:01  bolinger]
 * 
 * Revision 1.2.4.2  1992/04/24  18:04:48  bolinger
 * 	Fix OT defect 2413: correct placement of _CMA_REENTRANT_LIBC_ #ifdef to
 * 	cover only those definitions that truly depend on absence of re-entrant
 * 	libc (several don't).  Fix OT defect 2991: include unistd.h to get
 * 	proper prototypes for cuserid(), ctermid().
 * 	[1992/04/24  17:14:19  bolinger]
 * 
 * Revision 1.2  1992/01/19  22:14:46  devrcs
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
 *	CMA services
 *
 *  ABSTRACT:
 *
 *	This module contains example jacket routines which make the selected
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
 *	001	Webb Scales	1 August 1990
 *		Fixed various pcc problems.
 *	002	Paul Curtin	26 October 1990
 *		Finished off def's of scanf, fscanf, and sscanf.  All
 *		are if'd out until (v)ariable versions are available.
 *	003	Paul Curtin	19 November 1990
 *		Conditionally removed definitions on vms for: cma_close,
 *		cma_popen, cma_setbuffer, cma_setlinebuf, cma_tempnam,
 *		and cma_ttyslot
 *	004	Paul Curtin	21 November 1990
 *		Rearranged conditional exclusion
 *	005	Webb Scales	30 May 1991
 *		Added calls to import and unimport fd's to make the stdio
 *		wrappers play better with the section 2 wrappers.
 *	006	Dave Butenhof	05 June 1991
 *		Integrate IBM-specific changes
 *	007	Paul Curtin	06 June 1991
 *		Made all wrappers self initializing.
 *	008	Dave Butenhof	12 June 1991
 *		Fix rewind() and setbuf() for AIX (they're void, not int).
 *	009	Dave Butenhof	27 August 1991
 *		Fix v*printf functions so they'll work.
 *	010	Dave Butenhof	17 September 1991
 *		Don't compile the module unless _CMA_UNIPROCESSOR_, to get
 *		around some ODE build environment problems.
 *	011	Dave Butenhof	18 September 1991
 *		Merge in Apollo & HP CMA5 reverse drops: add to some of
 *		the conditional expressions.
 *	012	Dave Butenhof	01 October 1991
 *		Fix cma_ungetc() prototype to match ANSI C (c is "int", not
 *		"char").
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>

#include <cma_init.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <nl_types.h>

# include <stdarg.h>
#include <cma_stdio.h>
#include <cma_ux.h>




/*
 * cma_stdio.h must not be included, directly or indirectly, by this file
 * (or compilation unit) as it contains definition which conflict with what
 * we want in this file!  So, cause an error if it has been included.
 */


/*
 * GLOBAL DATA
 */

/*
 * LOCAL DATA
 */

/*
 * LOCAL MACROS
 */

/*
 * INTERFACES
 */
extern int ttyslot (void);


#define cma_stdio_simple(type, func, arg_list)  \
    { \
    type	result;		\
    cma__int_init ();		\
    cma_lock_global ();		\
    result = func arg_list;	\
    cma_unlock_global ();	\
    return result;		\
    }

#define cma_stdio_void(func, arg_list)  \
    { \
    cma__int_init ();		\
    cma_lock_global ();		\
    func arg_list;		\
    cma_unlock_global ();	\
    return;			\
    }

#if !_CMA_REENTRANT_CLIB_		/* Don't compile unless needed! */


/*
 *  The ctermid subroutine generates the pathname of the con-
 *  trolling terminal for the current process, and stores it in
 *  a string.
 *
 *  If s is a NULL pointer, the string is stored in an internal
 *  static area, the contents of which are overwritten at the
 *  next call to ctermid, and the address of which is returned.
 *  Otherwise, s is assumed to point to a character array of at
 *  least L_ctermid elements.  The pathname is placed in this
 *  array and the value of s is returned.  The constant
 *  L_ctermid is defined in the <stdio.h> header file.
 *
 *  FIX-ME: This wrapper should be modified so that, if s is a 
 *	    NULL pointer, we create a PTC an use that.  Under
 *	    no circumstances should we be returning a pointer
 *	    to a piece of static storage, as this defeats half
 *	    the purpose of the reentrant wrapper.
 */
extern char *
cma_ctermid (
	char	*s)
    {
    cma_stdio_simple (char *, ctermid, (s));
    }


/*
 *  The cuserid subroutine generates a character-string
 *  representation of the login name of the owner of the current
 *  process.  If s is a NULL pointer, this representation is
 *  generated in an internal static area, the address of which
 *  is returned.  Otherwise, s is assumed to point to an array
 *  of at least L_cuserid characters; the representation is left
 *  in this array.  The constant L_cuserid is defined in the
 *  <stdio.h> header file.
 *
 *  FIX-ME: This wrapper should be modified so that, if s is a 
 *	    NULL pointer, we create a PTC an use that.  Under
 *	    no circumstances should we be returning a pointer
 *	    to a piece of static storage, as this defeats half
 *	    the purpose of the reentrant wrapper.
 */
extern char *
cma_cuserid (
	char	*s)
    {
    cma_stdio_simple (char *, cuserid, (s));
    }


/*
 *  The fclose routine causes any buffers for the named stream
 *  to be emptied, and the file to be closed.  Buffers allocated
 *  by the standard input/output system are freed.  The fclose
 *  routine is performed automatically upon calling exit(3).
 */
extern int
cma_fclose (
	FILE	*stream)
    {
    int		result;

    if (!cma__is_multithreaded()) return (fclose(stream));

    cma_lock_global ();
#if !_CMA_THREAD_SYNC_IO_
    cma_unimport_fd (fileno(stream));
#endif
    result = fclose(stream);
    cma_unlock_global ();

    return result;
    }


/*
 *  The fflush routine causes any buffered data for the named
 *  output stream to be written to that file.  The stream
 *  remains open.
 */
extern int
cma_fflush (
	FILE	*stream)
    {
    cma_stdio_simple (int, fflush, (stream));
    }


/*
 *  The fdopen routine associates a stream with a file descrip-
 *  tor.  File descriptors are obtained from open, dup, creat,
 *  or pipe(2), which open files but do not return pointers to a
 *  FILE structure stream. Streams are necessary input for many
 *  of the Section 3s library routines.  The type of stream must
 *  agree with the mode of the open file.
 */
extern FILE *
cma_fdopen (
	int	fildes,
	char	*type)
    {
    FILE        *result;

    if (!cma__is_multithreaded()) return (fdopen (fildes, type));

    cma_lock_global ();
    result = fdopen (fildes, type);
#if !_CMA_THREAD_SYNC_IO_
    if(!cma__is_open(fildes) && result) cma_import_fd (fileno(result));
#endif
    cma_unlock_global ();

    return result;
    }


/*
 *  The (f)getc function returns the next character from the named
 *  input stream.
 */
extern int
cma_fgetc (
	FILE	*stream)
    {
    /* 
     * Use the getc macro instead of the fgetc routine for efficiency.
     */
    cma_stdio_simple (int, getc, (stream));
    }


/*
 *  The fgets routine reads n-1 characters, or up to a newline
 *  character, whichever comes first, from the stream into the
 *  string s.  The last character read into s is followed by a
 *  null character.  The fgets routine returns its first argu-
 *  ment.
 */
extern char *
cma_fgets (
	char	*s,
	int	n,
	FILE	*stream)
    {
    cma_stdio_simple (char *, fgets, (s, n, stream));
    }


/*
 *  The fopen routine opens the file named by filename and asso-
 *  ciates a stream with it.  The fopen routine returns a
 *  pointer to the FILE structure associated with the stream.
 */
extern FILE *
cma_fopen (
	const char	*filename,
	const char	*type)
    {
    FILE 	*result;

    if (!cma__is_multithreaded()) return (fopen (filename, type));

    cma_lock_global ();
    result = fopen (filename, type);
#if !_CMA_THREAD_SYNC_IO_
    if (result)	cma_import_fd (fileno(result));
#endif
    cma_unlock_global ();

    return result;
    }


/*
 *  The fprintf subroutine places output on the named output 
 *  stream.  This function converts, formats, and prints its
 *  arguments after the first under control of the first argu-
 *  ment.  The first argument is a character string which con-
 *  tains two types of objects: plain characters, which are sim-
 *  ply copied to the output stream, and conversion specifica-
 *  tions.  Each of these cause conversion and printing of the
 *  next successive arg.
 */
extern int
cma_fprintf (
	FILE		*stream,
	const char	*format,
	...)
    {
    va_list	ap;
    int		result;


    cma__int_init();
    cma_lock_global ();
    va_start (ap, format);

    result = vfprintf (stream, format, ap);

    va_end (ap);

    cma_unlock_global ();

    return result;
    }


/*
 *  The (f)putc routine appends the character c to the named output
 *  stream.  It returns the character written.
 */
extern int
cma_fputc (
	int	c,
	FILE	*stream)
    {
    /* 
     * Use the putc macro instead of the fputc routine for efficiency.
     */
    cma_stdio_simple (int, putc, (c, stream));
    }


/*
 *  The fputs subroutine copies the null-terminated string s to
 *  the named output stream.
 */
extern int
cma_fputs (
	const char	*s,
	FILE		*stream)
    {
    cma_stdio_simple (int, fputs, (s, stream));
    }


/*
 *  The fread function reads into a block beginning at ptr,
 *  nitems of data of the type of *ptr from the named input
 *  stream.  It returns the number of items actually read.
 *
 *  If stream is stdin and the standard output is line buffered,
 *  then any partial output line will be flushed before any call
 *  to read(2) to satisfy the fread.
 */
extern size_t
cma_fread (
	void	*ptr,
	size_t	size,
	size_t	nitems,
	FILE	*stream)
    {
    cma_stdio_simple (int, fread, (ptr, size, nitems, stream));
    }


/*
 *  The freopen routine substitutes the named file in place of
 *  the open stream.  The original stream is closed, regardless
 *  of whether the open ultimately succeeds.  The freopen rou-
 *  tine returns a pointer to the FILE structure associated with
 *  stream.
 *
 *  The freopen routine is typically used to attach the preo-
 *  pened streams associated with stdin, stdout and stderr to
 *  other files.
 */
extern FILE *
cma_freopen (
	const char	*filename,
	const char	*type,
	FILE		*stream)
    {
    FILE 	*result;

    if (!cma__is_multithreaded()) return (freopen (filename, type, stream));

    cma_lock_global ();
#if !_CMA_THREAD_SYNC_IO_
    cma_unimport_fd (fileno(stream));
#endif
    result = freopen (filename, type, stream);
#if !_CMA_THREAD_SYNC_IO_
    if (result)	cma_import_fd (fileno(result));
#endif
    cma_unlock_global ();

    return result;
    }


#if _CMA_VSSCANF_
/*
 *  The fscanf subroutine reads from the named input
 *  stream.  This function reads characters, interprets them
 *  according to a format, and stores the results in its argu-
 *  ments.  Each expects, as arguments, a control string format
 *  described below, and a set of pointer arguments indicating
 *  where the converted input should be stored.
 */
extern int
cma_fscanf (
	FILE		*stream,
	const char	*format,
	...)
    {
    va_list     ap;
    int         result;

    cma__int_init();

    cma_lock_global ();
    va_start (ap, format);

    result = vfscanf (stream, format, ap);

    va_end (ap);

    cma_unlock_global ();

    return result;
    }
#endif


/*
 *  The fseek function sets the position of the next input or
 *  output operation on the stream.  The new position is at the
 *  signed distance offset bytes from the beginning, the current
 *  position, or the end of the file, according as ptrname has
 *  the value 0, 1, or 2.
 *
 *  The fseek function undoes any effects of ungetc(3s).
 */
extern int
cma_fseek (
	FILE	*stream,
	long	offset,
	int	ptrname)
    {
    cma_stdio_simple (int, fseek, (stream, offset, ptrname));
    }


/*
 *  The ftell function returns the current value of the offset
 *  relative to the beginning of the file associated with the
 *  named stream.  It is measured in bytes and is the only fool-
 *  proof way to obtain an offset for fseek.
 */
extern long
cma_ftell (
	FILE	*stream)
    {
    cma_stdio_simple (long, ftell, (stream));
    }


/*
 *  The fwrite function appends, at most, nitems of data of the
 *  type of *ptr beginning at ptr to the named output stream.
 *  It returns the number of items actually written.
 */
extern size_t
cma_fwrite (
	const void	*ptr,
	size_t		size,
	size_t		nitems,
	FILE		*stream)
    {
    cma_stdio_simple (int, fwrite, (ptr, size, nitems, stream));
    }


/*
 *  The gets routine reads a string into s from the standard
 *  input stream stdin.  The string is terminated by a newline
 *  character, which is replaced in s by a null character.  The
 *  gets routine returns its argument.
 */
extern char *
cma_gets (
	char	*s)
    {
    cma_stdio_simple (char *, gets, (s));
    }


/*
 *  The getw function returns the next word (in a 32-bit integer
 *  on a VAX-11 or MIPS machine) from the named input stream.
 *  It returns the constant EOF upon end of file or error, but
 *  since that is a good integer value, feof and ferror(3s)
 *  should be used to check the success of getw. The getw
 *  assumes no special alignment in the file.
 */
extern int
cma_getw (
	FILE	*stream)
    {
    cma_stdio_simple (int, getw, (stream));
    }


/*
 *  The isatty subroutine returns 1 if filedes is associated
 *  with a terminal device, 0 otherwise.
 */
extern int
cma_isatty (
	int	filedes)
    {
    cma_stdio_simple (int, isatty, (filedes));
    }


/*
 *  The mktemp subroutine replaces template by a unique file
 *  name, and returns the address of the template.  The template
 *  should look like a file name with six trailing X's, which
 *  will be replaced with the current process ID and a unique
 *  letter.
 */
extern char *
cma_mktemp (
	char	*template)
    {
    extern char *mktemp (char *template);


    cma_stdio_simple (char *, mktemp, (template));
    }


/*
 *  A stream opened by popen should be closed by pclose, which
 *  waits for the associated process to terminate and returns
 *  the exit status of the command.
 */
extern int
cma_pclose (
	FILE	*stream)
    {
    int		result;

    if (!cma__is_multithreaded()) return (pclose (stream));
    cma_lock_global ();
#if !_CMA_THREAD_SYNC_IO_
    cma_unimport_fd (fileno(stream));
#endif
    result = pclose (stream);
    cma_unlock_global ();

    return result;
    }


/*
 *  The printf subroutine places output on the standard output
 *  stream stdout.  This function converts, formats, and prints its
 *  arguments after the first under control of the first argu-
 *  ment.  The first argument is a character string which con-
 *  tains two types of objects: plain characters, which are sim-
 *  ply copied to the output stream, and conversion specifica-
 *  tions.  Each of these cause conversion and printing of the
 *  next successive arg.
 */
extern int
cma_printf (
	const char	*format,
	...)
    {
    va_list	ap;
    int		result;


    cma__int_init();
    cma_lock_global ();
    va_start (ap, format);


    result = vprintf (format, ap);

    va_end (ap);

    cma_unlock_global ();

    return result;
    }


/*
 *  The puts subroutine copies the null-terminated string s to
 *  the standard output stream stdout and appends a new line
 *  character.
 */
extern int
cma_puts (
	const char	*s)
    {
    cma_stdio_simple (int, puts, (s));
    }


/*
 *  The putw routine appends word (that is, int) w to the output
 *  stream.  It returns the word written.  The putw routine nei-
 *  ther assumes nor causes special alignment in the file.
 */
extern int
cma_putw (
	int	w,
	FILE	*stream)
    {
    cma_stdio_simple (int, putw, (w, stream));
    }


/*
 *  The rewind (stream) function is equivalent to fseek (stream, 0L, 0),
 *  except that no value is returned.
 */
extern void 
cma_rewind (
	FILE *stream)
    {
    cma_stdio_void (rewind, (stream));
    }


#if _CMA_VSSCANF_
/*
 *  The scanf subroutine reads from the standard input stream
 *  stdin.  This function reads characters, interprets them
 *  according to a format, and stores the results in its argu-
 *  ments.  Each expects, as arguments, a control string format
 *  described below, and a set of pointer arguments indicating
 *  where the converted input should be stored.
 */
extern int
cma_scanf (
	const char	*format,
	...)
    {
    va_list     ap;
    int         result;

    cma__int_init();

    cma_lock_global ();
    va_start (ap, format);

    result = vscanf (format, ap);

    va_end (ap);

    cma_unlock_global ();

    return result;
    }
#endif


/*
 *  The setbuf routine is used after a stream has been opened
 *  but before it is read or written.  The character array buf
 *  is used instead of an automatically allocated buffer.  If
 *  buf is the constant pointer NULL, input/output will be com-
 *  pletely unbuffered.  A manifest constant BUFSIZ tells how
 *  big an array is needed:
 *
 *	char buf[BUFSIZ];
 *
 */
extern void
cma_setbuf (
	FILE	*stream,
	char	*buf)
    {
    cma_stdio_void (setbuf, (stream, buf));
    }


/*
 *  The setbuffer routine, an alternate form of setbuf, is used
 *  after a stream has been opened but before it is read or
 *  written.  The character array buf whose size is determined
 *  by the size argument is used instead of an automatically
 *  allocated buffer.  If buf is the constant pointer NULL,
 *  input/output will be completely unbuffered.
 */
extern void
cma_setbuffer (
	FILE	*stream,
	char	*buf,
	int	size)
    {
    cma_stdio_void (setbuffer, (stream, buf, size));
    }


/*
 *  The setlinebuf routine is used to change stdout or stderr
 *  from block buffered or unbuffered to line buffered.  Unlike
 *  setbuf and setbuffer it can be used at any time that the
 *  file descriptor is active.
 */
extern void
cma_setlinebuf (
	FILE	*stream)
    {
    cma_stdio_void (setlinebuf, (stream));
    }


/*
 *  The setvbuf routine may be used after a stream has been
 *  opened but before it is read or written.  Type determines
 *  how stream will be buffered.  Legal values for type, defined
 *  in stdio.h are:
 *
 *	_IOFBF	    causes input/output to be fully buffered.
 *	_IOLBF	    causes output to be line buffered; the buffer
 *		    will be flushed when a new line is written,
 *		    the buffer is full, or input is requested.
 *	_IONBF	    causes input/output to be completely unbuffered.
 *
 *  If buf is not the NULL pointer, the array it points to will
 *  be used for buffering, instead of an automatically allocated
 *  buffer.  The size specifies the size of the buffer to be
 *  used.  The constant BUFSIZ in <stdio.h> is suggested as a
 *  good buffer size.  If input/output is unbuffered, buf and
 *  size are ignored.
 */
extern int
cma_setvbuf (
	FILE	*stream,
	char	*buf,
	int	type,
	size_t	size)
    {
    cma_stdio_simple (int, setvbuf, (stream, buf, type, size));
    }


/*
 *  The sprintf subroutine places output in the string s,
 *  followed  by the character `\0'.  This function converts,
 *  formats, and prints its arguments after the first under
 *  control of the first argument.  The first argument is a
 *  character string which contains two types of objects: plain
 *  characters, which are simply copied to the output stream, and
 *  conversion specifications.  Each of these cause conversion
 *  and printing of the next successive arg.
 */
extern int
cma_sprintf (
	char		*s,
	const char	*format,
	...)
    {
    va_list	ap;
    int		result;


    cma__int_init();
    cma_lock_global ();
    va_start (ap, format);

    result = vsprintf (s, format, ap);

    va_end (ap);

    cma_unlock_global ();

    return result;
    }


#if _CMA_VSSCANF_
/*
 *  The sscanf subroutine reads from the character
 *  string s.  This function reads characters, interprets them
 *  according to a format, and stores the results in its argu-
 *  ments.  Each expects, as arguments, a control string format
 *  described below, and a set of pointer arguments indicating
 *  where the converted input should be stored.
 */
extern int
cma_sscanf (
	const char	*s,
	const char	*format,
	...)
    {
    va_list     ap;
    int         result;


    cma__int_init();

    cma_lock_global ();
    va_start (ap, format);


    result = vsscanf (s, format, ap);

    va_end (ap);

    cma_unlock_global ();

    return result;

    }
#endif


/*
 *  This function generates file names that can safely be used
 *  for a temporary file.  The argument dir points to the path-name of
 *  the directory in which the file is to be created.  If dir is
 *  NULL or points to a string which is not a path-name for an
 *  appropriate directory, the path-name defined as P_tmpdir in
 *  the <stdio.h> header file is used.  If that path-name is not
 *  accessible, /tmp will be used as a last resort.  This entire
 *  sequence can be up-staged by providing an environment vari-
 *  able TMPDIR in the user's environment, whose value is a
 *  path-name for the desired temporary-file directory.
 *
 *  Many applications prefer their temporary files to have cer-
 *  tain favorite initial letter sequences in their names.  Use
 *  the pfx argument for this.  This argument may be NULL or
 *  point to a string of up to five characters to be used as the
 *  first few characters of the temporary-file name.
 *
 *  The tempnam subroutine uses malloc(3) to get space for the
 *  constructed file name, and returns a pointer to this area.
 *  Thus, any pointer value returned from tempnam may serve as
 *  an argument to free. For further information, see malloc(3).
 *  If tempnam cannot return the expected result for any reason,
 *  that is malloc failed, or none of the above mentioned
 *  attempts to find an appropriate directory was successful, a
 *  NULL pointer will be returned.
 */
extern char *
cma_tempnam (
	char	*dir, 
	char	*pfx)
    {
    cma_stdio_simple (char *, tempnam, (dir,  pfx));
    }


/*
 *  The tmpfile subroutine creates a temporary file and returns
 *  a corresponding FILE pointer.  The file will automatically
 *  be deleted when the process using it terminates.  The file
 *  is opened for update.
 */
extern FILE *
cma_tmpfile (void)
    {
    FILE	*result;

    if (!cma__is_multithreaded()) return (tmpfile ());
    cma_lock_global ();
    result = tmpfile ();
#if !_CMA_THREAD_SYNC_IO_
    if (result) cma_import_fd (fileno(result));
#endif
    cma_unlock_global ();

    return result;
    }


/*
 *  The tmpnam subroutine always generates a file name using the
 *  path-name defined as P_tmpdir in the <stdio.h> header file.
 *  If s is NULL, tmpnam leaves its result in an internal static
 *  area and returns a pointer to that area.  The next call to
 *  tmpnam will destroy the contents of the area.  If s is not
 *  NULL, it is assumed to be the address of an array of at
 *  least L_tmpnam bytes, where L_tmpnam is a constant defined
 *  in <stdio.h>; tmpnam places its result in that array and
 *  returns s.
 *
 *  FIX-ME: This wrapper should be modified so that, if s is a 
 *	    NULL pointer, we create a PTC an use that.  Under
 *	    no circumstances should we be returning a pointer
 *	    to a piece of static storage, as this defeats half
 *	    the purpose of the reentrant wrapper.
 */
extern char *
cma_tmpnam (
	char	*s)
    {
    cma_stdio_simple (char *, tmpnam, (s));
    }


/*
 *  The ttyname subroutine returns a pointer to the null-
 *  terminated path name of the terminal device associated with
 *  file descriptor filedes (this is a system file descriptor
 *  and has nothing to do with the standard I/O FILE typedef).
 */
extern char *
cma_ttyname (
	int	filedes)
    {
    extern char *ttyname (int);


    cma_stdio_simple (char *, ttyname, (filedes));
    }


/*
 *  The ttyslot subroutine returns the number of the entry in
 *  the ttys(5) file for the control terminal of the current
 *  process.
 */
extern int
cma_ttyslot (void)
    {
    cma_stdio_simple (int, ttyslot, ());
    }


/*
 *  The ungetc routine pushes the character c back on an input
 *  stream.  That character will be returned by the next getc
 *  call on that stream.  The ungetc routine returns c.
 *
 *  One character of pushback is guaranteed provided something
 *  has been read from the stream and the stream is actually
 *  buffered.  Attempts to push EOF are rejected.
 *
 *  The fseek(3s) routine erases all memory of pushed back char-
 *  acters.
 */
extern char
cma_ungetc(
	int	c,
	FILE	*stream)
    {
    cma_stdio_simple (char, ungetc, (c, stream));
    }


/*
 *  The vfprintf routine is the same as fprintf except that
 *  instead of being called with a variable number of arguments,
 *  it is called with an argument list as defined by varargs(3).
 */
extern int 
cma_vfprintf (
	FILE		*stream,
	const char	*format,
	va_list		ap)
    {
    int		result;


    cma__int_init();
    cma_lock_global ();
    result = vfprintf (stream, format, ap);
    cma_unlock_global ();

    return result;
    }


/*
 *  The vprintf routine is the same as printf except that
 *  instead of being called with a variable number of arguments,
 *  it is called with an argument list as defined by varargs(3).
 */
extern int 
cma_vprintf (
	const char	*format,
	va_list		ap)
    {
    int		result;


    cma__int_init();
    cma_lock_global ();
    result = vprintf (format, ap);
    cma_unlock_global ();

    return result;
    }


/*
 *  The vsprintf routine is the same as sprintf except that
 *  instead of being called with a variable number of arguments,
 *  it is called with an argument list as defined by varargs(3).
 */
extern int
cma_vsprintf (
	char		*s,
	const char	*format,
	va_list		ap)
    {
    int		result;


    cma__int_init();
    cma_lock_global ();
    result = vsprintf (s, format, ap);
    cma_unlock_global ();

    return result;
    }
#endif	/* if !_CMA_REENTRANT_CLIB_ */


extern nl_catd
cma_catopen(
	char    *name,
	int     oflag
        )
    {
    cma_stdio_simple (nl_catd, catopen, (name, oflag));
    }

extern char *
cma_catgets(
	nl_catd cat_descriptor,
        int     set_number,
        int     message_number,
        char    *string
        )
    {
    cma_stdio_simple (char *, catgets, 
                      (cat_descriptor, set_number, message_number, string));
    }


extern int
cma_catclose(
	nl_catd cat_descriptor)
    {
    cma_stdio_simple (int, catclose, (cat_descriptor));
    }



#if !_CMA_REENTRANT_CLIB_

/*
 * Popen(3) is now wrapped for two discrete reasons.  First,
 * if there is no re-entrant libc present, it's wrapped on
 * the same basis as other stdio functions.  However, on OSF/1
 * it's also wrapped in order to disable the virtual interval
 * timer, as explained for cma_system().
 *
 * The arguments to popen are pointers to null-terminated
 * strings containing respectively a shell command line and an
 * I/O mode, either "r" for reading or "w" for writing.  It
 * creates a pipe between the calling process and the command
 * to be executed.  The value returned is a stream pointer that
 * can be used (as appropriate) to write to the standard input
 */
extern FILE *
cma_popen (
	char	*command,
	char	*type)
    {
	FILE *rv;

# if	!_CMA_REENTRANT_CLIB_
    if (!cma__is_multithreaded()) return (popen(command, type));
    cma_lock_global ();
# endif
    rv = popen(command, type);
#if !_CMA_THREAD_SYNC_IO_
	if (rv) cma_import_fd (fileno(rv));
#endif
# if	!_CMA_REENTRANT_CLIB_
    cma_unlock_global ();
# endif
	return (rv);
    }

#endif	/* !_CMA_REENTRANT_CLIB_ */
