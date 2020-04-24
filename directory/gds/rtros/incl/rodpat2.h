/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rodpat2.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:35  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:32  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:34:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:08  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:33  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:08  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:08  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:56:55  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:13  keutel
 * 	creation
 * 	[1994/03/21  13:26:22  keutel]
 * 
 * $EndLog$
 */
#ifndef _RODPAT2_H
#define _RODPAT2_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rodpat2_rcsid[] = { "@(#) $RCSfile: rodpat2.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:35 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONS AG         */
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
/* NAME         : rodpat2.h                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* COMPONENT    : RTS dispatcher                                      */
/*                                                                    */
/* PRD#/VERS.   : RTS-V02.00                                          */
/*                                                                    */
/* DESCRIPTION  : Interface between dispatcher and external interface */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  = "@(#) @(#)rodpat2.h1.193/11/29"                       */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 02.00 K0 | 90-12-31|  original                      |jm  |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************** System control functions ********************** */
/* ******************                          ********************** */

signed short int PASCAL ro0250_attachSys PROTO ((unsigned long	int FAR *,
						 signed   short int	 ,
						 signed   long	int	 ,
						 FARPROC		 ,
						 unsigned long	int	 ,
						 char		    FAR *));

signed short int PASCAL ro0251_detachSys PROTO ((unsigned long	int FAR *));


signed short int PASCAL ro0252_cleardpat PROTO ((void));
void             PASCAL ro0275_ClearAll  PROTO ((void));


/* ****************** Non peer communication functions ************** */
/* ******************                                  ************** */

signed short int PASCAL ro0260_attach PROTO ((unsigned long  int      ,
					      unsigned long  int FAR *,
                                              signed   short int      ,
					      signed   short int      ,
					      signed   long  int      ,
					      unsigned long  int FAR *,
                                              unsigned long  int      ,
					      unsigned long  int FAR *,
					      unsigned long  int      ,
					      unsigned long  int FAR *,
                                              unsigned long  int      ,
					      unsigned long  int FAR *,
                                              unsigned long  int      ));

signed short int PASCAL ro0261_detach PROTO ((unsigned long  int      ,
					      unsigned long  int      ,
                                              unsigned long  int FAR *,
                                              unsigned long  int FAR *,
                                              unsigned long  int FAR *,
                                              unsigned long  int FAR *));

signed short int PASCAL ro0262_provideBuffer PROTO ((unsigned long int	    ,
						     unsigned long int	    ,
						     unsigned long int FAR *));

signed short int PASCAL ro0263_wake	  PROTO ((unsigned long  int,
						  unsigned long  int));

signed short int PASCAL ro0264_getPmInfo  PROTO ((unsigned long  int,
						  unsigned long  int,
                                                  signed   short int,
                                                  signed   short int,
						  void	   FAR	   *));

signed short int PASCAL ro0265_getEcbInfo PROTO ((unsigned long  int	  ,
						  unsigned long  int	  ,
						  signed   short int FAR *,
						  signed   short int FAR *,
						  signed   long  int FAR *,
						  unsigned long  int FAR *,
						  unsigned long  int FAR *,
						  unsigned long  int FAR *,
						  unsigned long  int FAR *));

signed short int PASCAL ro0266_getSysInfo PROTO ((unsigned long  int	  ,
						  signed   short int	  ,
						  unsigned	 int FAR *,
						  unsigned	 int FAR *,
						  unsigned	 int FAR *,
						  unsigned	 int	  ,
						  unsigned	 int FAR *,
						  unsigned	 int	  ,
						  unsigned	 int FAR *,
						  unsigned	 int FAR *));

signed short int PASCAL ro0267_recover	  PROTO ((unsigned long  int));

signed short int PASCAL ro0268_setRecover PROTO ((unsigned long  int));

signed short int PASCAL ro0269_AcbGetEcb  PROTO ((unsigned long  int	  ,
						  unsigned long  int	  ,
						  unsigned long  int FAR *));

/* ****************** Peer communication functions ****************** */
/* ******************                              ****************** */

signed short int PASCAL ro0270_send PROTO ((unsigned long  int	     ,
					    signed   short int	     ,
                                            unsigned long  int       ,
                                            unsigned long  int   FAR*,
                                            signed   short int       ,
                                            signed   long  int       ,
					    unsigned long  int	 FAR*,
					    unsigned long  int	 FAR*,
					    unsigned long  int	 FAR*,
					    unsigned long  int	 FAR*));

signed short int PASCAL ro0271_receive PROTO ((unsigned long  int      ,
					       signed	short int      ,
                                               unsigned long  int FAR* ,
                                               unsigned long  int FAR* ,
                                               signed   long  int FAR* ,
                                               unsigned long  int FAR* ,
                                               unsigned long  int FAR* ,
                                               unsigned long  int FAR* ,
                                               unsigned long  int FAR* ));

/* Return value == 0 : success, event                                      */
/*              == 2 : no event                                            */
/*              == 3 : timeout, no event                                   */
/*               < 0 : error                                               */
/*                                                                         */

signed short int PASCAL ro0273_getPmInfo2 PROTO ((unsigned long  int,
						  unsigned long  int,
                                                  signed   short int,
                                                  signed   short int,
						  void	   FAR	   *));

signed short int PASCAL ro0274_getPrBfCount PROTO (( unsigned long int  ,
                                                     unsigned long int  ,
                                                     signed        int FAR *));

#endif	/* _RODPAT2_H */
