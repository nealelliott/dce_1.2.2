/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xx_lib_r.c,v $
 * Revision 1.1.15.2  1996/02/17  23:29:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:13  marty]
 *
 * Revision 1.1.15.1  1995/12/11  22:51:59  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/05  19:35 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1995/12/11  22:23:57  root]
 * 
 * 	Initial King Kong branch
 * 	[1993/07/09  16:44:40  root]
 * 
 * Revision 1.1.13.1  1994/02/23  21:44:15  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:43:03  pellis]
 * 
 * 	Merged Hp_dce1_0_1 changes
 * 	[1993/04/16  15:03:43  sekhar]
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  19:21  bissen
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.1.7  1993/03/11  22:17:04  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.2  1993/02/05  15:30:00  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:50:40  cjd]
 * 
 * Revision 1.1  1992/01/19  14:33:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   FILE_NAME: libxx_.c
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS: common
 *              xx_error
 *              xx_log
 *              xx_severe
 *              xx_warning
 *
 *   ORIGINS: 27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1991
 *   All Rights Reserved
 *
 *
 *   FOR_TEST: Any
 *
 *   USAGE:
 *
 *      #include <xx_lib.h>
 *
 *      int xx_error( string )
 *      char *string;
 *
 *      int xx_log( string )
 *      char *string;
 *
 *      int xx_severe( string )
 *      char *string;
 *
 *      int xx_warning( string )
 *      char *string;
 *
 *      where:
 *         string      is the message to be included in
 *                     the journal file.
 *
 *   DESCRIPTION:
 *      This is the C interface to the logging tools used
 *      by the DCE system tests.
 *
 *   HISTORY: 
 *      11/20/90   Ken Walker     Initial code
 *      12/05/90   Ken Walker     Flush stderr and stdout after printing,
 *                                check for getenv returning NULL
 *       1/18/91   Ken Walker     Made library thread-safe
 */

#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <dce/dce.h>

#ifndef XX_NO_THREADS
#include <dce/cma.h>
#include <dce/cma_ux.h>
#include <dce/cmalib_crtlx.h>
#include <pthread.h>
#endif

#include <systest.h>

#ifndef NULL
#define	NULL		((void *)0)
#endif

#ifdef XX_LIB_TIME
#define	PREFIX_LEN	19
#else
#define PREFIX_LEN	10
#endif

/********************************************************
 * common (internal use only)
 *   In this implementation, all the commands do the
 *   same thing except their names are different.  All
 *   the externalized commands in the interface call
 *   this internal function to do the actual work.
 * Input parameters:
 *   command	Name of the command the user called.
 *   userString	Text string the user passed in to be
 *		included in the log message.
 * Output parameters:
 *   None.
 * Returns:
 *   0 if successful, 1 otherwise.
 ********************************************************/
int common( command, userString )
char *	command;
char *	userString;
{
    int		i;			/* Loop counter */
    char	err_buf[255];		/* Error message */
    static int	journal = -1;		/* Test's journal file */
    char *	journalName;		/* Name of test's journal file */
    char	msg_buf[255];		/* Message to be added to journal */
	int     stdo = 1;
	int     stde = 2;
    int rc;
    static int firstTime = TRUE;
    long time_l;
    char* tp;
    char* timeStr;

    
    /* add the hr:min:sec to the prefix */
#ifdef XX_LIB_TIME
    time( &time_l );
    tp = ctime( &time_l );
    timeStr = &tp[11];
    tp[19] = ':';
    tp[20] = '\0';
    strcpy( msg_buf, timeStr );

    /* Build prefix of the form "-name----:" */
    strcat( msg_buf, "-" );
#else 
    /* Build prefix of the form "-name----:" */
    strcpy( msg_buf, "-" );
#endif
    strcat( msg_buf, command );
    for ( i = strlen(msg_buf); i < PREFIX_LEN-1; i++ )
	*(msg_buf+i) = '-';
    *(msg_buf+PREFIX_LEN-1) = ':';
    *(msg_buf+PREFIX_LEN) = ' ';
    *(msg_buf+PREFIX_LEN+1) = '\0';

    /* Append user's message to end of prefix */
    strcat( msg_buf, userString );
	strcat( msg_buf, "\n" );

    /* Print to stdo */
    rc = write( stdo, msg_buf, strlen( msg_buf ) );
    if( rc == -1 )
    {    sprintf( err_buf,
		    "xx_%s: Error %d when writing to stdout\n",
						    command, errno );
	 write( stde, err_buf, strlen( err_buf ) );
    }
    /* fflush( stdo ); */

    /* open the journal file, if one is defined */
    if( firstTime )
    { journalName = (char *)getenv( "JOURNAL" );
      if ( (journalName != NULL) && (*journalName != '\0') ) {
	journal = open( journalName, O_WRONLY | O_CREAT | O_APPEND, 0666 );
	if ( journal == -1 ) {
	    sprintf( err_buf,
		    "xx_%s: Error %d when opening journal file '%s'\n",
		    command, errno, journalName );
	    write( stde, err_buf, strlen( err_buf ) );
	    /* fflush( stde ); */
	    return( 1 );
	} 
      }
      firstTime = FALSE;
   }

   if( journal != -1 )
   {    if ( write( journal, msg_buf, strlen( msg_buf ) ) == -1 ) {
	    sprintf( err_buf,
		    "xx_%s: Error %d when writing to journal file '%s'\n",
		    command, errno, journalName );
	    write( stde, err_buf, strlen( err_buf ) );
	    /* fflush( stde ); */
	    return( 1 );
	}

    }

    return( 0 );
}

/********************************************************
 * xx_* (externalized interfaces)
 *   These are the actual functions called by a user from
 *   a test written in C.  They all have the same syntax.
 * Input parameters:
 *   userString	Text string the user passed in to be
 *		included in the log message.
 * Output parameters:
 *   None.
 * Returns:
 *   0 if successful, 1 otherwise.
 ********************************************************/
/********************************************************
 * xx_error
 ********************************************************/
int xx_error( userString )
char *	userString;
{
    return( common("error", userString) );
}


/********************************************************
 * xx_log
 ********************************************************/
int xx_log( userString )
char *	userString;
{
    return( common("log", userString) );
}


/********************************************************
 * xx_severe
 ********************************************************/
int xx_severe( userString )
char *	userString;
{
    return( common("severe", userString) );
}


/********************************************************
 * xx_warning
 ********************************************************/
int xx_warning( userString )
char *	userString;
{
    return( common("warning", userString) );
}
