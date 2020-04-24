/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * HISTORY
 * $Log: cma_libc_redefs.h,v $
 * Revision 1.1.6.2  1996/02/18  23:09:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:33:13  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:55:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:13  root]
 * 
 * Revision 1.1.4.1  1994/06/09  13:36:52  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:59  devsrc]
 * 
 * Revision 1.1.2.2  1993/02/01  20:14:28  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  20:02:30  hinman]
 * 
 * $EndLog$
 */
#ifndef R_LIBC_REDEFS
#define R_LIBC_REDEFS

/*
 * the following symbols are used during cma initialization or during
 * dispatching of threads; so we can not use the reentrant versions 
 * when building libcma. __nr_ is the original non-reentrant interface.
 */
# define abort     __nr_abort
# define catopen   __nr_catopen
# define catgets   __nr_catgets
# define catclose  __nr_catclose
# define exit      __nr_exit
# define fprintf   __nr_fprintf
# define getenv    __nr_getenv
# define gets      __nr_gets
# define perror    __nr_perror
# define printf    __nr_printf
# define sbrk      __nr_sbrk
# define setitimer __nr_setitimer
# define sigaddset __nr_sigaddset
# define sigdelset __nr_sigdelset
# define sigfillset __nr_sigfillset
# define sprintf   __nr_sprintf
# define vsprintf  __nr_vsprintf


#endif /* R_LIBC_REDEFS */
