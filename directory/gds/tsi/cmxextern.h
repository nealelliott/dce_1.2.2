/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxextern.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:12  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:47:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:08  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:08:26  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:21:37  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:11  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:31  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:14  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:50:37  marrek]
 * 
 * $EndLog$
 */
#ifndef _CMXEXTERN_H
#define _CMXEXTERN_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cmxextern_rcsid[] = "@(#)$RCSfile: cmxextern.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:12 $";
#endif

/* #ident " @(#)cmxextern.h	311.10 93/08/24 CMX (NC1)" */
/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG  All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1993
 *               All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

/****************************************************************************/
/*                                                                          */
/*                  HEADER FILE FOR ALL GLOBAL VARIABLES                    */
/*                  ====================================                    */
/*                                                                          */
/* This header file contains all the references to global variables         */
/* in the CMX library. These variables are defined in the module CMX2K.C.   */
/*                                                                          */
/*                                                                          */
/* Author: kam                                            Date:   30.03.92  */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

/**************************************************************************/    
/*      D E C L A R A T I O N    O F    G L O B A L    D A T A            */    
/**************************************************************************/    

/*
 * DEFINES
 */

#ifndef FALSE                                                                   
#define FALSE 0                                                                 
#define TRUE  1                                                                 
#endif                                                                          

#ifndef SIGIO                                                                   
#define SIGIO 22                                                                
#endif                                                                          

#define	T_RETRY	255	/* internal retry of t_event because of local FC */
/*
 * DATA
 * Variables defined in CMX2K
 */
extern 	int     t_ncpy(register char *, register char *, register int);
extern  int     t_pid;		/* Process Id of the CMX application process */
extern  int     _t_error;	/* ICMX(L) error variable */
extern 	fd_set  t_fdset, t_wfds, t_rfds, t_xfds;
extern 	int     t_sfds; 
extern  int     t_cmxsig;       /* >0: CMX signalling with this signal-# */
extern  int     t_sigio;        /* TRUE: CMX handler active for SIGIO */
extern struct sigaction t_prevsigio; /* Store SIGIO handler */
extern  int     t_sigoth;       /* TRUE: CMX handler active for SIGOTH */
extern struct sigaction t_prevsigoth; /* Store old signal handler */
extern	void    t_signal(int);     /* Signal handler */
extern  int     cmx_fd;         /* File descriptor for the CMX automaton */
extern  int     open_max;       /* Determine OPEN_MAX via sysconf */
extern  int     t_tidusize;	/* TIDU-Size for CMX on top of TCP/IP and TLI/XTI */


/*
 * FUNCTIONS
 */
extern  char    *t_mapname(char *);
extern  union t_address *t_mapaddr(union t_address *, union t_address *);
#endif /* _CMXEXTERN_H */
