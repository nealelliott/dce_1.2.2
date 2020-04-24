/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rodpat3.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:36  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:33  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:34:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:10  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:34  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:09  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:09  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:56:58  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:14  keutel
 * 	creation
 * 	[1994/03/21  13:26:24  keutel]
 * 
 * $EndLog$
 */
#ifndef _RODPAT3_H
#define _RODPAT3_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rodpat3_rcsid[] = { "@(#) $RCSfile: rodpat3.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:36 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1991 BY SIEMENS NIXDORF INFORMATIONS AG         */
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
/* NAME         : rodpat3.h                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller,  Vinita                             */
/* DATE         : 91-10-31                                            */
/*                                                                    */
/* COMPONENT    : RTROS dispatcher                                    */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V03.00                                        */
/*                                                                    */
/* DESCRIPTION  : Dispatcher configuration modul interface            */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  = "@(#)rodpat3.h  1.2  93/12/20"                        */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 2.00 K0  | 91-10-31|  original                      |jm  |         */
/* 3.00 K0  | 93-04-09| Changes for X410 & SniP Cf I/fs|vp  |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************** Configuration PM function ********************* */
/* ******************                           ********************* */

signed short int PASCAL ro0290_cfPmFct PROTO ((signed   short int,
					       signed   short int,
					       unsigned long  int));

/* ****************** Configuration PM clear function *************** */
/* ******************                                 *************** */

signed short int PASCAL ro0291_cfClFct PROTO ((signed   short int,
					       signed   short int,
					       signed   long  int,
					       unsigned long  int,
					       unsigned long  int,
					       unsigned long  int,
					       unsigned long  int));

/* ****************** Configuration PM information function ********* */
/* ******************                                       ********* */

signed short int PASCAL ro0292_cfIfFct PROTO ((signed   short int,
					       signed   short int,
					       unsigned long  int,
					       signed   short int,
					       void     FAR     *));

/* ****************** Configuration PM pdv node encode function ***** */
/* ******************                                           ***** */

signed short int PASCAL ro0293_cfPdvEnFct PROTO ((signed   short int       ,
						  signed   short int       ,
						  rTro76_stDefinedBy FAR  *,
						  rTro01_pfvObjectId	   ,
						  unsigned long  int       ,
						  void               NEAR *,
						  unsigned long  int FAR  *,
						  unsigned long  int FAR  *,
						  signed         int));

/* ****************** Configuration PM pdv node decode function ***** */
/* ******************                                           ***** */

signed short int PASCAL ro0294_cfPdvDeFct PROTO ((signed   short int       ,
						  signed   short int       ,
						  signed   short int       ,
						  signed   long  int       ,
						  rTro76_stDefinedBy FAR  *,
						  rTro01_pfvObjectId       ,
						  unsigned long  int       ,
						  void               NEAR *,
						  unsigned long  int FAR  *,
						  unsigned long  int       ,
						  signed         int));

/* ****************** Configuration PM pdv node clear function ****** */
/* ******************                                          ****** */

signed short int PASCAL ro0295_cfPdvClFct PROTO ((signed   short int      ,
						  unsigned long  int FAR *));


/* ******************** Configuration PM X410  encode function ****** */
/* ******************                                           ***** */

signed short int PASCAL ro0296_cfX410EnFct PROTO ((signed   short int      ,
						  signed   short int       ,
						  unsigned long  int FAR  *,
						  void               NEAR *,
						  unsigned long  int FAR  * ));


/* ******************** Configuration PM X410  decode function ****** */
/* ******************                                           ***** */

signed short int PASCAL ro0297_cfX410DeFct PROTO ((signed   short int      ,
						  signed   short int       ,
						  unsigned       int       ,
						  signed   long  int       ,
						  unsigned long  int       ,
						  void               NEAR *,
						  unsigned long  int FAR  * ));

/* ******************** Configuration PM SNIP  encode function ****** */
/* ******************                                           ***** */

signed short int PASCAL ro0298_cfSniPEnFct PROTO ((signed   short int      ,
						   signed   short int      ,
						   unsigned long  int      ,
						   void               NEAR *,
						   unsigned long  int FAR  * ));


/* ******************** Configuration PM SNIP  decode function ****** */
/* ******************                                           ***** */

signed short int PASCAL ro0299_cfSniPDeFct PROTO ((signed   short int       ,
						   signed   short int       ,
						   unsigned long  int       ,
						   void               NEAR *,
						   unsigned long  int FAR  *,
                                                   signed   short int FAR  * ));

#endif	/* _RODPAT3_H */
