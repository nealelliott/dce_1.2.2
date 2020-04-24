/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roloif2.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:47  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:42  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:35:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:28  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:56  keutel
 * 	OT 11620
 * 	[1994/08/19  13:48:59  keutel]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:42  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:18  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:19  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:30  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:27  keutel
 * 	creation
 * 	[1994/03/21  13:26:49  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROLOIF2_H
#define _ROLOIF2_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roloif2_rcsid[] = { "@(#) $RCSfile: roloif2.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:47 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1990                     */
/*  * All rights reserved                                             */
/*  *                                                                 */
/*  * This software is furnished under licence and may be used only   */
/*  * in accordance with the terms of that licence and with the       */
/*  * inclusion of the above copyright notice.                        */
/*  * This software may not be provided or otherwise made available   */
/*  * to, or used by, any other person. No title to or ownership of   */
/*  * the software is hereby  transferred.                            */
/*                                                                    */
/* ****************************************************************** */

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : roloif2.h                                           */
/*                                                                    */
/* AUTHOR       : C. Schoenherr / Johann Mueller                      */
/* DATE         : 91-01-01                                            */
/*                                                                    */
/* COMPONENT    : RTS                                                 */
/*                                                                    */
/* PRD#/VERS.   : RTS-V02.00                                          */
/*                                                                    */
/* DESCRIPTION  : Function Prototyping for the Lower Interface        */
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY      : sccsid  =  " @(#)roloif2.h 1.1 93/11/29"                        */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/* 02.00 K0 | 91-01-01|  original                      | sc |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*-------------------------------------------------------------------------*/
signed short int PASCAL ro0300_attach PROTO (( /* ret status               */
        unsigned long int,              /* <-- handle of ECB               */
        unsigned long int FAR *,        /* <-- pointer to applid of loif   */
        unsigned long int,              /* <-- system-id                   */
        unsigned long int,              /* <-- handle of local address     */
        signed   short int FAR *));     /* --> pointer to the Tpmflow ctrl */
                                        /*     bool variable               */

/* Return value == 0 : successful                                          */
/*              == 1 : not first                                           */
/*               < 0 : error                                               */


/*-------------------------------------------------------------------------*/
signed short int PASCAL ro0301_detach PROTO (( /* ret status               */
        unsigned long int,              /* <-- handle of ECB               */
        unsigned long int FAR *));      /* <-- pointer to applid of loif   */

/* Return value == 0 : success                                             */
/*               < 0 : error                                               */

/*-------------------------------------------------------------------------*/

signed short int FAR PASCAL ro0302_informSpm PROTO (( /* ret status        */
        unsigned long  int,             /* <-- handle of context           */ 
        signed   short int,             /* <-- Information Type            */
        void               FAR * ));    /* <--> Pointer to information     */

/* Return value == 0 : success                                             */
/*               < 0 : error                                               */

/*-------------------------------------------------------------------------*/
signed short int PASCAL ro0305_wake PROTO ((   /* ret status               */
        unsigned long int));            /* system-id                       */

/* Return value == 0 : success                                             */

/*-------------------------------------------------------------------------*/
signed short int PASCAL ro0306_GetAddress PROTO ((/*ret Length of address part*/
        unsigned long int,                  /* <-- handle of address buffer*/
        unsigned short int,                 /* <-- Layer number            */
        unsigned short int,                 /* <-- Length of buffer        */
        char FAR *));                       /* <-- Buffer to store address */
                                            /*     part                    */

/* Return value == 0 : success                                             */
/*               < 0 : error                                               */

/*-------------------------------------------------------------------------*/
signed short int PASCAL ro0307_PutAddress PROTO ((   /* ret status         */
        unsigned long int FAR *,              /* <-- pointer to handle of  */
                                              /*     addr.buf              */
        unsigned short int,                   /* <-- Layer number          */
        unsigned short int,                   /* <-- Length of address part*/
        char FAR *));                         /* <-- Buffer to store       */
                                              /*     address part          */

/* Return value == 0 : success                                             */
/*               < 0 : error                                               */

/*-------------------------------------------------------------------------*/
signed short int PASCAL ro0308_getAddrLen PROTO ((   /* ret status	   */
	unsigned long int FAR *,	      /* <-- pointer to where	   */
					      /*     length is stored.	   */
	unsigned long int));		      /* <-- address handle	   */

/* Return value == 0 : success                                             */
/*               < 0 : error                                               */

/*-------------------------------------------------------------------------*/
signed short int PASCAL ro0311_CheckOrWait PROTO (( /* ret status          */
        unsigned long int FAR *,             /* --> pointer to handle of   */
                                             /*     ECB                    */
	signed	 long int FAR *,	     /* --> pointer to handle of   */
					     /*     loif assoc. (conect.)  */
					     /*     identifier		   */

	signed	short int FAR *,	     /* --> pointer to pm number   */

	signed	 long int FAR *,	     /* --> pointer to Event	   */

	unsigned long int FAR *,	     /* --> pointer to p1	   */
	unsigned long int FAR *,	     /* --> pointer to p2	   */
	unsigned long int FAR *,	     /* --> pointer to p3	   */
	unsigned long int FAR *,	     /* --> pointer to p4	   */

	signed	short int      ,	     /* <-- blocking time	   */
	unsigned long int      ));	     /* <-- system-id		   */

/* Return value == 0 : success, event                                      */
/*              == 2 : no event                                            */
/*              == 3 : timeout, no event                                   */
/*               < 0 : error                                               */
/*                                                                         */
/* Blocking time:  == 0 : check only                                       */
/*                 == -1: infinitive waiting                               */
/*          other values: waiting time in secounds                         */

/*-------------------------------------------------------------------------*/
signed short int PASCAL ro0310_SendReceive PROTO (( /* ret status          */
        unsigned long int));                 /* <-- context of loif        */

/* Return value == 0 : success                                             */
/*               < 0 : error                                               */


/*-------------------------------------------------------------------------*/
signed short int PASCAL ro0399_cleanUp PROTO ((/* ret status               */
        signed long int,                /* <-- event                       */
        unsigned long int,              /* <-- Parameter 1                 */
        unsigned long int,              /* <-- Parameter 2                 */
        unsigned long int,              /* <-- Parameter 3                 */
        unsigned long int));            /* <-- Parameter 4                 */

/* Return value == 0 : success                                             */
/*                                                                         */
/* event == 0 : clear context (handle in parameter 1)                      */
/* event != 0 : clear the event (memory blocks passed by handles)          */


/*-------------------------------------------------------------------------*/
/*
int EXPORT FAR PASCAL PostAccept  PROTO ((unsigned long, int));
int EXPORT FAR PASCAL PostConnect PROTO ((unsigned long, int));
int EXPORT FAR PASCAL PostRead    PROTO ((unsigned long, int));
int EXPORT FAR PASCAL PostWrite   PROTO ((unsigned long, int));
*/

#endif	/* _ROLOIF2_H */
