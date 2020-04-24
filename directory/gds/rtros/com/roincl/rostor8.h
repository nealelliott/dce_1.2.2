/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rostor8.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:10  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:12:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:15  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:10  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:41  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:46  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:49  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:04  keutel
 * 	creation
 * 	[1994/03/21  13:19:38  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROSTOR8_H
#define _ROSTOR8_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rostor8_rcsid[] = { "@(#) $RCSfile: rostor8.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:01 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c) 1992 by                                                  *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG                                 *
*  * All rights reserved                                                    *
*  *                                                                        *
*  * This software is furnished under licence and may be used only in       *
*  * accordance with the terms of that licence and with the inclusion of    *
*  * the above copyright notice. This software may not be provided or       *
*  * otherwise made available to, or used by, any other person. No title    *
*  * to or ownership of the software is hereby transferred.                 *
*                                                                           *
\***************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE		: INCLUDE					      */
/*								      */
/* NAME         : rostor8.h                                           */
/*								      */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : FEB-6th-1992                                        */
/*								      */
/* COMPONENT    : Store common utilities                              */
/*								      */
/* PRD#/VERS.   : RTS-V2.0 / MAVROS-V2.2                              */
/*								      */
/* DESCRIPTION  : Store common utilities                              */
/*								      */
/*		  - Read/Write operation on a buffered Store          */
/*								      */
/* SYSTEM DEPENDENCE :                                                */
/*								      */
/* HISTORY	: sccsid  =  @(#)rostor8.h	1.1 94/01/26				      */
/*								      */
/* Vers.Nr. | Date    |	 changes		       |mark| CR# EM# */
/*  1.00    | 91/11/01|  original                      | gj |         */
/*  1.01    | 91/11/02|  new error codes introduced    | gj |         */
/*  1.02    | 91/11/13|  introduced: stname[]          | gj |         */
/*  1.03    | 92/02/06|  MAXBULEN changeable for tests | gj |         */
/*  1.04    | 92/02/25|  L_PREFIX changed              | gj |         */
/*  1.05    | 92/03/10|  new struct-element lockhandle | gj |         */
/*  1.06    | 92/04/22|  removed all items concerning  | gj |         */
/*          |         |  Store naming conventions - the|    |         */
/*          |         |  Storefile is now temporary    |    |         */
/*  2.00    | 93/05/05|  Boolean variable for keeping  |sss |         */
/*          |         |  track of named file and file  |    |         */
/*          |         |  name a single dim.  array     |    |         */
/*          |         |  added in STORE structure      |    |         */
/*datoff **************************************************************/
/*datoff **************************************************************/

/* ********************************************************************/
/* **********              Store facility                  ********** */
/* **********        internal definitions for rostor0.c    ********** */
/* ********************************************************************/
#ifdef STORETEST
#define MAXBULEN 4*1024L               /* For test purpose only           */
#else
#define MAXBULEN 10*1024L          /* Store buffer limit (can't be exceeded)*/
#endif

#define OK     0                   /* used for 'no error' statement         */
#define MAXTMPNAM 25               /* used for tmpnam function              */


typedef struct {
  unsigned long int lockhandle;    /* casted address from globallock        */
				   /* needed as input when a storefct is    */
				   /* called within a storefct              */
  unsigned long int handle;        /* handle returned from globalalloc      */
				   /* needed for all further memory fct     */
				   /* like unlock,lock,free,realloc...      */
  long              stlength;      /* how many bytes the Store contains     */
  long              blength;       /* length of buffer                      */
  long              offset;        /* store position of the first byte      */
				   /* in the buffer                         */
  unsigned char FAR *        base; /* start adress of the buffer            */
				   /* if base = NULL -> no buffer activated */
  unsigned long int bufhandle;     /* handle connected to the buffer        */
  unsigned char FAR *        bptr; /* read/write pointer within buffer      */
  long              brest;         /* number of free bytes in buffer        */
  signed short int  error;         /* current error type                    */
  int               modify;        /* set if buffer was changed by a        */
				   /* write access (stputc,stputm,stputs)   */
  /* FILE           fp;         */ /* ordinary C-file descriptor            */
  unsigned long int   fp;          /* Handle of C-file descriptor           */
  int               links;         /* counts number of users linked to      */
				   /* this STORE                            */
  char       vnamed[MAXTMPNAM];    /* current file's name if it is not Temp.*/  
  signed short int  bnamed;        /* current file is named or temp.        */
 } STORE;

#endif	/* _ROSTOR8_H */
