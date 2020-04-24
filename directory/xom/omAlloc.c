/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omAlloc.c,v $
 * Revision 1.1.81.2  1996/02/18  18:25:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:48  marty]
 *
 * Revision 1.1.81.1  1995/12/08  15:39:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:58  root]
 * 
 * Revision 1.1.79.2  1994/07/06  15:09:23  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:25:53  marrek]
 * 
 * Revision 1.1.79.1  1994/02/22  18:11:44  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:06:52  marrek]
 * 
 * Revision 1.1.77.1  1993/10/14  19:24:19  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:23:05  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  10:18:47  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  07:21:39  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:25:30  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:04  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:06:35  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:40:04  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:46:31  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:21:10  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:47:50  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omAlloc.c,v $ $Revision: 1.1.81.2 $ $Date: 1996/02/18 18:25:21 $";
#endif

/***************************************************************
*															   *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG	1991   *
*				 ALL RIGHTS	RESERVED						   *
*															   *
***************************************************************/

/**********************************************************************/
/*				 COPYRIGHT (C) BULL	S.A. 1990 1991 1992				  */
/*						  All Rights Reserved						  */
/**********************************************************************/
/*																	  */
/*	 * This	software is	furnished under	licence	and	may	be used	only  */
/*	 * in accordance with the terms	of that	licence	and	with the	  */
/*	 * inclusion of	the	above copyright	notice.						  */
/*	 * This	software may not be	provided or	otherwise made available  */
/*	 * to, or used by, any other person. No	title to or	ownership of  */
/*	 * the software	is hereby transferred.							  */
/*																	  */
/**********************************************************************/
/*	@(#)omAlloc.c	1.20 (Bull S.A)	6/26/92	*/

/*********************************************************
 **							**
 **	alloc.c	  --  Memory Allocation	Package	for	the	OM.	**
 **							**
 **	Author : J.Zrihen, DASD/SSD.			**
 *********************************************************/

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include	<stdio.h>
#include	<signal.h>
#include	<errno.h>
#include	<stdarg.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<xom.h>
#include	<xomip.h>

#define		OMINIT
#include	<alloc.h>
#include	<omAlloc.h>

/******************************************************************/
/*			  Global variable									  */
/******************************************************************/
FILE	*mmgt_logfile =		stderr;
OM_sint	mmgt_dbg = 0;

/******************************************************************/
/*			  Macros definition									  */
/******************************************************************/
#ifdef MMGT
#define	MMGT_ILLEGAL_OPT_COMBINATION(opt1, opt2)		 \
		  {											\
		  fprintf(mmgt_logfile,"Warning	: illegal options combination (	(%s) and (%s))\n", opt1, opt2);\
	  fflush (mmgt_logfile); \
		  }
#else
#define	MMGT_ILLEGAL_OPT_COMBINATION(opt1, opt2)
#endif

#ifdef MMGT
#define	MMGT_OPEN_TRACE_FILE_FAILED()				 \
		  {											\
	  extern int  errno;						\
		  fprintf(stderr,"Warning :	opening	MP trace file failed : %s\n", strerror(errno));\
		  }
#else
#define	MMGT_OPEN_TRACE_FILE_FAILED()
#endif

#ifdef MMGT
#define	MMGT_CLOSE_TRACE_FILE_FAILED()				  \
		  {											\
	  extern int  errno;						\
		  fprintf(stderr,"Warning :	closing	MP trace file failed : %s\n", strerror(errno));\
		  }
#else
#define	MMGT_CLOSE_TRACE_FILE_FAILED()
#endif


/**********************************************************************/
/*																	  */
/* FUNCTION:  mmgt_set_trace()										  */
/*																	  */
/* PURPOSE:	 set the Memory	Management trace options.				  */
/*																	  */
/* INPUT:  none.													  */
/*									  */
/* INPUT/OUTPUT: none							  */
/*									  */
/* OUTPUT:	none							  */
/*									  */
/* RETURN:		status : whether the function succeeded	or failed	  */
/*									  */
/* NOTE:	The	 environment variable "MMGT_TRACE_OPTIONS" is used to */
/*			set	up Memory trace	options.							  */
/*			The	syntax is :											  */
/*			MMGT_TRACE_OPTIONS=<option>[:<option>][@<file>]			  */
/*			The	possible value of <option> are :					  */
/*					 NONE :	none traces	will be	generated.			  */
/*					 ON	: all traces will be generated.	Some of	these */
/*			   traces can not be understood	by application*/
/*			   programmer. There are for BULL technical	  */
/*			   support.									  */
/*									  */
/*			<file> specified the trace file. Default file is stderr	  */
/*									  */
/*			Default	value of <option> is "NONE"						  */
/*									  */
/*			Example:												  */
/*			  MMGT_TRACE_OPTIONS=ALL@stderr; export	MMGT_TRACE_OPTIONS*/
/*									  */
/**********************************************************************/

OM_return_code	  mmgt_set_trace (void)

{
	char	*val, *s_val, *s_val_cpy; /* used to store value returned by "getenv" call */
	char	*tok;	   /* used to store	token value						*/
	char	*opt;	   /* used to store	the	string of the trace	options	*/
	OM_sint	cmp_res;	  /* used to store string comparison result	*/
	OM_sint	mmgt_trace_opt = MMGT_TRACE_NONE;	/* MMGT	trace options mask	*/

	/*-----------------------------------------------------------*/
	/* Get environment variable									 */
	/*-----------------------------------------------------------*/
	val	 = getenv("MMGT_TRACE_OPTIONS");

	/*-----------------------------------------------------------*/
	/* If environment variable "MMGT_TRACE_OPTIONS"	cannot be	 */
	/* found : there are no	trace set up.						 */
	/*-----------------------------------------------------------*/
	if (val	 !=	NULL) {
		/*---------------------------------------------------------*/
		/* store the MMGT_TRACE_OPTIONS	value					   */
		/*---------------------------------------------------------*/
		s_val =		(char *) malloc(strlen(val)	 + 1);
		if (s_val == NULL) {
			return(OM_TEMPORARY_ERROR);
		}

		/*---------------------------------------------------------*/
		/* store the pointer value in order	to free	memory		   */
		/*---------------------------------------------------------*/
		s_val_cpy =		s_val;
		strcpy(s_val, val);

		/*---------------------------------------------------------*/
		/* isolate trace options : skip	the	trace file			   */
		/*---------------------------------------------------------*/
		opt	 = strtok(val, "@");

		/*---------------------------------------------------------*/
		/* parse the trace options : options consistency is	checked*/
		/*---------------------------------------------------------*/
		tok	 = strtok(opt, ":");
		while (tok != NULL)	 {

			cmp_res	 = strcmp(tok, "NONE");
			if (cmp_res	 ==	0) {
				if (mmgt_trace_opt & MMGT_TRACE_ALL) {
					MMGT_ILLEGAL_OPT_COMBINATION("ALL",
						 "NONE");
					OM_FREE(s_val);
					return(OM_TEMPORARY_ERROR);
				}
				mmgt_trace_opt = MMGT_TRACE_NONE;
			}

			cmp_res	 = strcmp(tok, "ALL");
			if (cmp_res	 ==	0) {
				if (mmgt_trace_opt & MMGT_TRACE_NONE) {
					MMGT_ILLEGAL_OPT_COMBINATION("NONE",
						 "ALL");
					OM_FREE(s_val);
					return(OM_TEMPORARY_ERROR);
				}
				mmgt_trace_opt = MMGT_TRACE_ALL;
			}

			cmp_res	 = strcmp(tok, "ON");
			if (cmp_res	 ==	0) {
				if (mmgt_trace_opt & MMGT_TRACE_NONE) {
					MMGT_ILLEGAL_OPT_COMBINATION("NONE",
						 "ON");
					OM_FREE(s_val);
					return(OM_TEMPORARY_ERROR);
				}
				mmgt_trace_opt += MMGT_TRACE_ON;
			}


			tok	 = strtok(NULL,	":");

		} /* endwhile */

		/*---------------------------------------------------------*/
		/* get trace file										   */
		/*---------------------------------------------------------*/
		tok	 = strtok(s_val,	"@");
		tok	 = strtok(NULL,	"@");
		if (tok	 !=	NULL) {
			mmgt_logfile = fopen(tok, "a");
			if (mmgt_logfile ==		NULL) {
				mmgt_logfile = stderr;
				MMGT_OPEN_TRACE_FILE_FAILED();
			}
		}

		/*---------------------------------------------------------*/
		/* free	up memory										   */
		/*---------------------------------------------------------*/
		OM_FREE(s_val_cpy);

	} /* endif*/


	/*---------------------------------------------------------*/
	/* set MMGT	trace mask									   */
	/*---------------------------------------------------------*/
	mmgt_dbg =	mmgt_trace_opt;


	/*-----------------------------------------------------------*/
	/* Processing end :	success									 */
	/*-----------------------------------------------------------*/
	return(OM_SUCCESS);


	/*------------------------------------------------------*/
} /* end mmgt_set_trace */



/*------------------------------------------------------*/


/**********************************************************************/
/*																	  */
/* FUNCTION:	mmgt_calloc()								  */
/*																	  */
/* PURPOSE:	clear &	allocate a memory buffer			  */
/*									  */
/* INPUT:	size : the buffer size.					  */
/*									  */
/* INPUT/OUTPUT:	none						  */
/*									  */
/* OUTPUT:	none							  */
/*									  */
/* RETURN:	the	buffer pointer					  */
/*									  */
/* NOTE:	none							  */
/*									  */
/**********************************************************************/



char	*mmgt_xcalloc(
	char *file,
	OM_sint	line,
	OM_sint	n,
	OM_sint	size
)
{
	char	*pt;

#ifdef	THREADSAFE
	char	buf[12];
	pthread_t	thread_id;
#endif	/* THREADSAFE */

	if (!(n	 * size))
		return NULL;

	pt = (char *)calloc(n, size);

	if (mmgt_dbg) {
#ifdef	THREADSAFE
		thread_id =		pthread_self();
		sprintf	(buf, "0x%x", thread_id);
		fprintf(mmgt_logfile, "%s CALLOC  pt : %X, file	: %s, line : %d, size :	%d,	eob	: %x\n",
			 buf, (unsigned int)pt, file,
			line, size,		(unsigned int)pt + size);
#else  /* THREADSAFE */
		fprintf(mmgt_logfile, "NO_THREAD CALLOC	 pt	: %X, file : %s, line :	%d,	size : %d, eob : %x\n",
			 (unsigned int)pt, file,
			line, size,		(unsigned int)pt + size);
#endif	/* THREADSAFE */
		fflush (mmgt_logfile);
	} else if (	pt == NULL ) {
#ifdef	THREADSAFE
		thread_id =		pthread_self();
		sprintf	(buf, "0x%x", thread_id);
		fprintf(mmgt_logfile, "%s CALLOC  FAILED IN	file : %s, line	: %d, size : %d	(errno = %d)\n",
			 buf, file,
			line, size,		errno);
#else  /* THREADSAFE */
		fprintf(mmgt_logfile, "NO_THREAD CALLOC	 FAILED	IN file	: %s, line : %d, size :	%d (errno =	%d)\n",
			 file,
			line, size,		errno);
#endif	/* THREADSAFE */
		fflush (mmgt_logfile);
	}

	return(pt);

	/*-------------------------------------------------*/
} /* end *mmgt_calloc */


/*-------------------------------------------------*/

/**********************************************************************/
/*																	  */
/* FUNCTION:	mmgt_free()									  */
/*																	  */
/* PURPOSE:	free a memory buffer.						  */
/*									  */
/* INPUT:	buffer : the buffer	pointer				  */
/*									  */
/* INPUT/OUTPUT:	none						  */
/*									  */
/* OUTPUT:	none							  */
/*									  */
/* RETURN:	the	buffer pointer					  */
/*									  */
/* NOTE:	none							  */
/*									  */
/**********************************************************************/

void	mmgt_xfree (
	char *file,
	OM_sint	line,
	char *buffer
)
{

#ifdef	THREADSAFE
	char	buf[12];
	pthread_t	thread_id;
#endif	/* THREADSAFE */

	if (mmgt_dbg) {
#ifdef	THREADSAFE
		thread_id =		pthread_self();
		sprintf	(buf, "0x%x", thread_id);
		fprintf(mmgt_logfile, "%s FREE	  pt : %X, file	: %s, line : %d	\n",
			 buf,	(unsigned int)buffer,		file, line)	;
#else  /* THREADSAFE */
		fprintf(mmgt_logfile, "NO_THREAD FREE	 pt	: %X, file : %s, line :	%d \n",
			 (unsigned int)buffer, file, line) ;
#endif	/* THREADSAFE */
		fflush (mmgt_logfile);
	} else if (	buffer == NULL)	 {
#ifdef	THREADSAFE
		thread_id =		pthread_self();
		sprintf	(buf, "0x%x", thread_id);
		fprintf(mmgt_logfile, "%s FREE OF A	NULL POINTER IN	file : %s, line	: %d\n",
			 buf, file,		line);
#else  /* THREADSAFE */
		fprintf(mmgt_logfile, "NO_THREAD FREE OF A NULL	POINTER	IN file	: %s, line : %d\n",
			 file, line);
#endif	/* THREADSAFE */
		fflush (mmgt_logfile);
	}

	if ( buffer	 !=	NULL	) {
		OM_FREE(buffer);
	}

	/*-------------------------------------------------*/
} /* end mmgt_free */


/*-------------------------------------------------*/

/**********************************************************************/
/*																	  */
/* FUNCTION:	mmgt_malloc()								  */
/*																	  */
/* PURPOSE:	allocate a memory buffer.				  */
/*									  */
/* INPUT:	size : the buffer size.					  */
/*									  */
/* INPUT/OUTPUT:	none						  */
/*									  */
/* OUTPUT:	none							  */
/*									  */
/* RETURN:	the	buffer pointer					  */
/*									  */
/* NOTE:	none							  */
/*									  */
/**********************************************************************/

char	*mmgt_xmalloc(
	char *file,
	OM_sint	line,
	OM_sint	size
)
{
	char	*pt;

#ifdef	THREADSAFE
	char	buf[12];
	pthread_t	thread_id;
#endif	/* THREADSAFE */

	if (!size)
		return(	NULL);

	pt = (char *)malloc(size) ;

	if (mmgt_dbg) {
#ifdef	THREADSAFE
		thread_id =		pthread_self();
		sprintf	(buf, "0x%x", thread_id);
		fprintf(mmgt_logfile, "%s MALLOC  pt : %X, file	: %s, line : %d, size :	%d\n",
			 buf, (unsigned int)pt, file,		line, size);
#else  /* THREADSAFE */
		fprintf(mmgt_logfile, "NO_THREAD MALLOC	 pt	: %X, file : %s, line :	%d,	size : %d\n",
			 (unsigned int)pt, file, line, size);
#endif	/* THREADSAFE */
		fflush (mmgt_logfile);
	} else if (	pt == NULL ) {
#ifdef	THREADSAFE
		thread_id =		pthread_self();
		sprintf	(buf, "0x%x", thread_id);
		fprintf(mmgt_logfile, "%s MALLOC  FAILED IN	file : %s, line	: %d, size : %d	(errno = %d)\n",
			 buf, file,
			line, size,		errno);
#else  /* THREADSAFE */
		fprintf(mmgt_logfile, "NO_THREAD MALLOC	 FAILED	IN file	: %s, line : %d, size :	%d (errno =	%d)\n",
			 file,
			line, size,		errno);
#endif	/* THREADSAFE */
		fflush (mmgt_logfile);
	}

	return(pt) ;

	/*-------------------------------------------------*/
} /* end *mmgt_malloc */


/*-------------------------------------------------*/

/**********************************************************************/
/*																	  */
/* FUNCTION:	mmgt_realloc()								  */
/*																	  */
/* PURPOSE:	reallocate a memory	buffer.					  */
/*									  */
/* INPUT:	size : the buffer size.					  */
/*									  */
/* INPUT/OUTPUT:	none						  */
/*									  */
/* OUTPUT:	none							  */
/*									  */
/* RETURN:	the	buffer pointer					  */
/*									  */
/* NOTE:	none							  */
/*									  */
/**********************************************************************/


char	*mmgt_xrealloc (
	char *file,
	OM_sint	line,
	char *old,
	OM_sint	size
)
{
	char	*pt;

#ifdef	THREADSAFE
	char	buf[12];
	pthread_t	thread_id;
#endif	/* THREADSAFE */

	pt = (char *)realloc(old, size);

	if (mmgt_dbg) {
#ifdef	THREADSAFE
		thread_id =		pthread_self();
		sprintf	(buf, "0x%x", thread_id);
		fprintf(mmgt_logfile, "%s REALLOC pt : %X, file	: %s, line : %d, size :	%d,	old	: %X\n",
			 buf, (unsigned int)pt, file,
			line, size,		(unsigned int)old);
#else  /* THREADSAFE */
		fprintf(mmgt_logfile, "NO_THREAD REALLOC pt	: %X, file : %s, line :	%d,	size : %d, old : %X\n",
			 (unsigned int)pt, file,
			line, size,		(unsigned int)old);
#endif
		fflush (mmgt_logfile);
	} else if (	pt == NULL ) {
#ifdef	THREADSAFE
		thread_id =		pthread_self();
		sprintf	(buf, "0x%x", thread_id);
		fprintf(mmgt_logfile, "%s REALLOC  FAILED IN file :	%s,	line : %d, size	: %d (errno	= %d)\n",
			 buf, file,
			line, size,		errno);
#else  /* THREADSAFE */
		fprintf(mmgt_logfile, "NO_THREAD REALLOC  FAILED IN	file : %s, line	: %d, size : %d	(errno = %d)\n",
			 file,
			line, size,		errno);
#endif
		fflush (mmgt_logfile);
	}

	return(pt);

	/*-------------------------------------------------*/
} /* end *mmgt_realloc() */


/*-------------------------------------------------*/

/**
 **	omP_error -- Print error message and send a	signal to the process.
 **/
void
omP_error(
    const char *fmt,
    ...
)
{
    va_list ap;

	if (omP_trace_opt &		OMP_TRACE_CHECK_SYNTAX)	 {
        va_start(ap, fmt);
        (void) vfprintf(omP_trace_file, fmt, ap);
		(void) fprintf(omP_trace_file,"\n");
        va_end(ap);
	}
	OMP_FATAL_FAILURE ;
} /* end omP_error */


/**
 **	omP_xalloc_str -- Allocate and copy	the	specified string.
 **/
char	*
omP_xalloc_str(
	char *from,
	OM_sint	len)
{
	char	*to;

	to = OM_MALLOC(len + 1);
	if ( to != (char	*) NULL	) {
		memcpy(to, from, len);
		to[len] = '\0';
	} /* endif */
	return(	to);
} /* end omP_xalloc_str	*/
