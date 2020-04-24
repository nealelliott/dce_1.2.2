/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
*/
/*
 * HISTORY
 * $Log: fmul.routines.c,v $
 * Revision 1.1.9.3  1996/02/17  23:27:07  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:40  marty]
 *
 * Revision 1.1.9.2  1995/12/11  22:00:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:59:25  root]
 * 
 * Revision 1.1.5.1  1994/10/24  19:22:06  rdl
 * 	Added threaded code.  Added improved debug mode.  Fixed minor bugs.
 * 	[1994/10/24  19:21:03  rdl]
 * 
 * Revision 1.1.2.2  1994/09/01  19:37:36  rdl
 * 	Changes to fix problems with RPC where multiple servers on one machine
 * 	[1994/09/01  19:36:22  rdl]
 * 
 * Revision 1.1.2.1  1994/08/24  19:45:02  rdl
 * 	Initial submission for dfs.fmul test.
 * 	[1994/08/24  19:44:35  rdl]
 * 
 * $EndLog$
 */

/* fmul.routines.c */

/* TODO: add chartowrite code */

#include <dce/cma_stdio.h>
#include <dce/cma_ux.h>

#include "fmul.h"
#include "fmul_common.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
/*
#include <sys/mode.h>
#include <sys/types.h>
*/

extern int debug;

int fd_locked=0;

long dolock(int request, filename_t filename, long start, long size, char chartowrite, long *rc_return, long *errno_return )
{
	int	i,rc;

	if (debug)
		puts("	entering dolock");
	/* open file */
	/* must be root to get S_ISGID to stick */
	if ((fd_locked=open( (char *)filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_ISGID)) == 0 )
	{
		if (debug)
			fprintf(stderr,"dolock: ");
		fprintf(stderr,"Cannot open %s.\n",filename);
		*rc_return=fd_locked;
		*errno_return=errno;
		return(-1);
	}

	/* move to position start in file */
	if ((rc=lseek( fd_locked, start, SEEK_SET )) < 0 )
	{
		if (debug)
			fprintf(stderr,"dolock: ");
		fprintf(stderr,"Cannot lseek %d bytes in %s.\n",start, filename);
		*rc_return=rc;
		*errno_return=errno;
		return(-1);
	}

	/* lock file */
	if ((rc=lockf( fd_locked, request, size )) != 0 )
	{
		if (debug)
			fprintf(stderr,"dolock: ");
		if (errno == EACCES)
			if (debug)
				fprintf(stderr,"Cannot lock %d bytes in %s already locked.\n",size, filename);
		else
			fprintf(stderr,"Cannot lock %d bytes in %s.\n",size, filename);
		*rc_return=rc;
		*errno_return=errno;
		return(-1);
	}
	if (debug)
		puts("	dolock ending");
	return(0);
}

idl_long_int l_lock(handle_t h, filename_t filename, idl_long_int start, idl_long_int size, idl_char chartowrite, idl_long_int *rc_return, idl_long_int *errno_return )
{
	if (debug)
		puts("function: l_lock");
	return(dolock(F_LOCK, filename, start, size, chartowrite, rc_return, errno_return));
}

idl_long_int l_tlock(handle_t h, filename_t filename, idl_long_int start, idl_long_int size, idl_char chartowrite, idl_long_int *rc_return, idl_long_int *errno_return )
{
	if (debug)
		puts("function: l_tlock");
	return(dolock(F_TLOCK, filename, start, size, chartowrite, rc_return, errno_return));
}

idl_long_int l_test( handle_t h, filename_t filename, idl_long_int start, idl_long_int size, idl_long_int *rc_return, idl_long_int *errno_return)
{
	int	i;
	int	fd;
	int	rc;

	if (debug)
		puts("function: l_test");
	/* open file */
	if ((fd=open( (char *)filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_ISGID)) == 0 )
	{
		if (debug)
			fprintf(stderr,"l_test: ");
		fprintf(stderr,"Cannot open %s.\n",filename);
		*rc_return=fd;
		*errno_return=errno;
		return(-1);
	}

	/* move to position start in file */
	if ((rc=lseek( fd, start, SEEK_SET )) < 0 )
	{
		if (debug)
			fprintf(stderr,"l_test: ");
		fprintf(stderr,"Cannot lseek %d bytes in %s.\n",start, filename);
		*rc_return=rc;
		*errno_return=errno;
		return(-1);
	}

	/* test lock file */
	if ((rc=lockf( fd, F_TEST, size )) != 0 )
	{
		if (debug)
			printf("	file locked\n");
		*rc_return=rc;
		*errno_return=errno;
		return(LOCK_GRANTED);
	}
	else
	{
		if (debug)
		printf("	file not locked\n");
		*rc_return=rc;
		*errno_return=errno;
		return(LOCK_DENIED);
	}
}

idl_long_int l_ulock( handle_t h, filename_t filename, idl_long_int start, idl_long_int size, idl_long_int *rc_return, idl_long_int *errno_return)
{
	int	i;
	int	rc;

	if (debug)
		puts("function: l_ulock");
	/* if no file opened, open file - mostly for CLEAR_CHILDREN */
	if (fd_locked==0)
		if ((fd_locked=open( (char *)filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_ISGID)) == 0 )
			{
			if (debug)
				fprintf(stderr,"l_ulock: ");
			fprintf(stderr,"Cannot open %s.\n",filename);
			*rc_return=fd_locked;
			*errno_return=errno;
			return(-1);
		}

	/* move to position start in file */
	if ((rc=lseek( fd_locked, start, SEEK_SET )) < 0 )
	{
		if (debug)
			fprintf(stderr,"l_ulock: ");
		fprintf(stderr,"Cannot lseek %d bytes in %s.\n",start, filename);
		*rc_return=rc;
		*errno_return=errno;
		return(-1);
	}

	/* unlock file */
	if ((rc=lockf( fd_locked, F_ULOCK, size )) != 0 )
	{
		if (debug)
			fprintf(stderr,"l_ulock: ");
		fprintf(stderr,"Cannot unlock %d bytes in %s.\n",size, filename);
		*rc_return=rc;
		*errno_return=errno;
		return(-1);
	}

	/* close file */
	if ((rc=close( fd_locked )) != 0 )
	{
		if (debug)
			fprintf(stderr,"l_ulock: ");
		fprintf(stderr,"Cannot close %s.\n", filename);
		*rc_return=rc;
		*errno_return=errno;
		return(-1);
	}

	*rc_return=rc;
	*errno_return=errno;
	fd_locked=0;
	return(0);
}




idl_long_int writeRange(handle_t h, filename_t filename, idl_long_int start, idl_long_int size, idl_char chartowrite, idl_long_int *rc_return, idl_long_int *errno_return )
{
	int	i,rc;
	int	fd;
	char	charout;

	charout=chartowrite;

	/* open file */
	/* must be root to get S_ISGID to stick */
	if ((fd=open( (char *)filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_ISGID)) == 0 )
	{
		if (debug)
			fprintf(stderr,"writeRange: ");
		fprintf(stderr,"Cannot open %s.\n",filename);
		*rc_return=fd;
		*errno_return=errno;
		return(-1);
	}

	/* move to position start in file */
	if ((rc=lseek( fd, start, SEEK_SET )) < 0 )
	{
		if (debug)
			fprintf(stderr,"writeRange: ");
		fprintf(stderr,"Cannot lseek %d bytes in %s.\n",start, filename);
		*rc_return=rc;
		*errno_return=errno;
		return(-1);
	}

	/* write file */
	for (i=0 ; i < size ; i++)
		if ((rc=write( fd, &charout, 1 )) != 1 )
		{
			if (debug)
				fprintf(stderr,"writeRange: ");
			fprintf(stderr,"Cannot write %c in %s.\n",charout, filename);
			*rc_return=rc;
			*errno_return=errno;
			return(-1);
		}

	close(fd);
	return(0);
}
