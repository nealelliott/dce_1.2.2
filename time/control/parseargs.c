/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: parseargs.c,v $
 * Revision 1.1.4.2  1996/02/17  23:34:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:09:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:23  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:52:00  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:49  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/21  22:05:41  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:48:14  cbrooks]
 * 
 * $EndLog$
 */
/*
 * parseargs - parse argument list
 * usage: parseargs( int * ac, [in/out] - number of args 
 		     char *** av, [in/out] - argument list 
		     char * hostname)  [out] hostname buffer if applicable
 *
 * parse argument list, searching for "-h hostname" argument
 * if found, copy the value of the option into the hostname 
 * buffer, and remove the option and argument, modifying argc/argv
 * appropriately
 *
 * If the first argument isn't "-h <hostname>", then 
 * just leave well enough alone, and return the values passed in.
 */

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <dtscp.h>

int dts_dtscp_parseargs( int * acp, char *** argvp, char ** hostname)
{
    int argc = *acp;
    char ** argv = *argvp ;
    register char ** ap = 0 ;
    int i = 1, j = 0, k = 0 ;
    register char * s = 0 , *p = 0 ;

    /* begin */

    s = argv[i];		/* first argument  */

    if (!strcmp(s, "-h"))	/* matches */
    {
	if ( ++i >= argc)	/* no value! damn! */
	{
	    errno = EINVAL ;
	    return -1 ;
	}

	p = strdup(argv[i]);
	*hostname = p ;
	ap = (char **)malloc( argc  * sizeof(char *));
	memmove( ap, argv, argc * sizeof(char *)); /* copy the pointers */

	/* fixup argc/argv  */

	for (k = 1, j = 3 ; j < argc ; j++, k++) 
	{
	    ap[k] = argv[j];
	}

	ap[k] = 0 ;		/* null last entry */
	*acp -= 2 ;
	*argvp = ap ;
    }

    return 0;
}    
    
	
    
    
    
