/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rouctr2.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:18  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:04  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:36:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:22  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:05  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:47  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:41  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:50  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:49  keutel
 * 	creation
 * 	[1994/03/21  13:28:15  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUCTR2_H
#define _ROUCTR2_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rouctr2_rcsid[] = { "@(#) $RCSfile: rouctr2.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:18 $" } ;
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
/* NAME 	: rouctr2.h					      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-04-01					      */
/*                                                                    */
/* COMPONENT	: RORTS upper control calling interface               */
/*                                                                    */
/* PRD#/VERS.	: RORTS-V03.00					      */
/*                                                                    */
/* DESCRIPTION	: Function call interface			      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  = "@(#)rouctr2.h  1.2 93/12/20"                         */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 02.00K0  | 91-12-01|  original		       |vp  |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************** PM function *********************************** */
/* ******************		  *********************************** */

signed short int PASCAL ro1500_upperCtrl  PROTO ((unsigned long int));

/* Return value == 0 : successfully	  */
/*              != 0 : error              */
/* Parameter 1       : pm context handle  */

/* ****************** PM clean up function ************************** */
/* ******************			   ************************** */

signed short int PASCAL ro1501_cleanUp PROTO ((signed	long int,
					       unsigned long int,
					       unsigned long int,
					       unsigned long int,
					       unsigned long int));

/* Return value == 0 : successfully		    */
/*		!= 0 : error			    */
/* Parameter 1	     : pm event | == 0		    */
/* Parameter 2	     : p1	| pm context handle */
/* Parameter 3	     : p2	| not used	    */
/* Parameter 4	     : p3	| not used	    */
/* Parameter 5	     : p4	| not used	    */

/* ****************** PM encoding function ************************** */
/* ******************			   ************************** */

signed short int PASCAL ro1502_encodUpperCtrl PROTO ((
                                                 rTro76_stDefinedBy FAR  *,
						 rTro01_pfvObjectId	  ,
						 unsigned long int	  ,
						 void	            NEAR *,
						 unsigned long int  FAR  *,
						 unsigned long int  FAR  *,
                                                 signed        int       ));

/* Return value == 0: Successfully.				      */
/*		!= 0: Error. (Caller returns immediately with the     */
/*		      same value, mapping of the value within the     */
/*		      PPM encoding function)			      */
/*								      */
/* Parameter1	    : --> ANY DEFINED BY encoding information	      */
/*			  (pointer to). NULL pointer if not used.     */
/*								      */
/* Parameter2	    : --> Transfer syntax object identifier (pointer  */
/*			  to).					      */
/*								      */
/* Parameter3	    : --> Store handle (encoded byte string).	      */
/*								      */
/* Parameter4	    : --> PPM context (pointer to).		      */
/*								      */
/* Parameter5	    : <-> Memory handle to PDV node (pointer to).     */
/*								      */
/* Parameter6	    : <-- Total length of all embedded PDV encodings  */
/*			  (pointer to). 			      */
/*			  - ANY, OCTET STRING : scale "byte".	      */
/*			  - BIT STRING	      : scale "bit"	      */
/*			  (lEncodingChoice from PDV header, see       */
/*			   rouply0.h)				      */
/* Parameter 7      : --> Any Number                                  */

/* ****************** PM decoding function ************************** */
/* ******************			   ************************** */

signed short int PASCAL ro1503_decodUpperCtrl PROTO ((
                                                 signed short int         ,
						 signed long  int	  ,
						 rTro76_stDefinedBy FAR  *,
						 rTro01_pfvObjectId	  ,
						 unsigned long	int	  ,
						 void	            NEAR *,
						 unsigned long	int FAR  *,
						 unsigned long	int	  ,
                                                 signed         int      ));

/* Return value == 0: Successfully.				      */
/*		!= 0: Error (caller returns immediately with same     */
/*		      value, mapping of the value within the PPM      */
/*		      encoding function)			      */
/*								      */
/* Parameter1	    : --> Abstract syntax identifier (describes the   */
/*			  named abstract syntax the node belongs to)  */
/*			  Has to be written into the PDV header.      */
/*			  (Is not necessary for the decoding function */
/*			  itself.)				      */
/*								      */
/* Parameter2	    : --> Encoding choice value of the PDV node.      */
/*			  ANY, OCTET STRING, BIT STRING)	      */
/*			  Has to be written into the PDV header.      */
/*			  (Is not necessary for the decoding function */
/*			  itself.)				      */
/*			  For values and further comments see	      */
/*			  rouply0.h				      */
/*								      */
/* Parameter3	    : --> ANY DEFINED BY decoding information	      */
/*			  (pointer to). NULL pointer if not used.     */
/*								      */
/* Parameter4	    : --> Transfer syntax object identifier (pointer  */
/*			  to).					      */
/*								      */
/* Parameter5	    : --> Store handle (encoded byte string).	      */
/*								      */
/* Parameter6	    : --> PPM context (pointer to).		      */
/*								      */
/* Parameter7	    : <-- Memory handle to PDV node (pointer to).     */
/*								      */
/* Parameter8	    : <-- Total length of all embedded PDV encodings  */
/*			  (Pointer to): 			      */
/*			  - ANY, OCTET STRING: scale "byte"	      */
/*			  - BIT STRING	     : scale "bit".	      */
/*								      */
/*			   NOTE: In the case of ANY length only if    */
/*			   within PDV-list or EXTERNAL and if definite*/
/*			   length encoding. In the other case the     */
/*			   value is equal to NULL.		      */
/* Parameter 9     : --> Any Number                                   */

/* ****************** PM clean up pdv node function ***************** */
/* ******************				    ***************** */

signed short int PASCAL ro1504_clearNodeUpperCtrl PROTO ((unsigned long int FAR *));

/* Return value == 0: successfully.				      */
/*		!= 0: error					      */
/*								      */
/* Parameter1	    : --> Memory handle to PDV node (pointer to)      */

signed short int PASCAL ro1506_encodSniPUpperCtrl PROTO ((
						 unsigned long int	  ,
						 void	            NEAR *,
						 rTro01_pfvObjectId	  ));


signed short int PASCAL ro1507_decodSniPUpperCtrl PROTO ((
                                                 signed   short int FAR  *,
						 unsigned long	int	  ,
						 void               NEAR *,
						 rTro01_pfvObjectId	 ));

#endif	/* _ROUCTR2_H */
