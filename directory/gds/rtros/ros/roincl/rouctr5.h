/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rouctr5.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:38  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:41:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:35  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:58  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:39  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:48  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:04:11  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:24  keutel
 * 	creation
 * 	[1994/03/21  13:33:26  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUCTR5_H
#define _ROUCTR5_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rouctr5_rcsid[] = { "@(#) $RCSfile: rouctr5.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:38 $" } ;
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
/* NAME 	: rouctr5.h					      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-04-01					      */
/*                                                                    */
/* COMPONENT	: RORTS upper control calling interface               */
/*                                                                    */
/* PRD#/VERS.	: RORTS-V03.00					      */
/*                                                                    */
/* DESCRIPTION	: Internal Function call interface		      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  = "@(#)rouctr5.h  1.1 93/11/29"                         */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 02.00K0  | 91-12-01|  original		       |vp  |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************** Create Event Data Function ******************** */
/* ******************		  *********************************** */

signed short int PASCAL ro1520_CreateEvData PROTO ((
                        unsigned long int,        
	                unsigned long int  FAR * ));

/* Return value == 0 : successfully	  */
/*              != 0 : error              */
/* Parameter 1       : handle to buffer for converting in EvData      */
/* Parameter 2       : output address to store EvData handle          */

/* ****************** Create Invoke Element ************************* */
/* ******************			   ************************** */

signed short int PASCAL ro1521_CreateIvkEle PROTO ((
                    unsigned long int ,
                    unsigned long int ,
                    rT1530_stAsoContext  FAR   * )) ; 

/* Return value == 0 : successfully		                     */
/*		!= 0 : error			                     */
/* Parameter 1	     : Invoke Identifier                             */
/* Parameter 2	     : handle to buffer for converting in Invoke data*/
/* Parameter 3	     : Pointer to upper control context              */

/* ****************** Event Data Copy to P1 function **************** */
/* ******************			   ************************** */

signed short int PASCAL ro1522_UserDataCp PROTO ((
		unsigned  long  int ,  
		unsigned  long  int ));      

/* Return value == 0: Successfully.				      */
/*		!= 0: Error. (Caller returns immediately with the     */
/*		      same value, mapping of the value within the     */
/*		      PPM encoding function)			      */
/*								      */
/* Parameter1	    : --> handle to User Data (P1)                    */
/*								      */
/* Parameter2	    : --> handle to event Data                        */

/* ****************** Event Data Copy      ************************** */
/* ******************			   ************************** */

signed short int PASCAL ro1523_EventDataCp PROTO ((
		char               FAR *,
		rT1504_stEvData    FAR *,
		rTro04_stUserData  FAR *, 
		unsigned long int  ));

/* Return value == 0: Successfully.				      */
/*		!= 0: Error (caller returns immediately with same     */
/*		      value, mapping of the value within the PPM      */
/*		      encoding function)			      */
/*								      */
/* Parameter1	    : --> character pointer to buffer where user      */
/*			  data is to be copied                        */
/*								      */
/* Parameter2	    : --> pointer to Event Data which  is to be       */
/*			  returned in P1                              */
/*								      */
/* Parameter3	    : --> pointer to userdata                         */
/*								      */
/* Parameter4	    : --> count - for fragment copy                   */

/* ****************** TPDU write function           ***************** */
/* ******************				    ***************** */

signed short int PASCAL ro1524_EventTpduCp PROTO ((
		rT00t3_stTpWrite   FAR *, 
		rTro04_stUserData  FAR *, 
		rT1504_stEvData    FAR *,
		unsigned long int  ));


/* Return value == 0: successfully.				      */
/*		!= 0: error					      */
/*								      */
/* Parameter1	    : -->Pointer to  TpWrite structure                */
/*								      */
/* Parameter2	    : --> pointer to userdata                         */
/*								      */
/* Parameter3	    : --> pointer to Event Data which  is to be       */
/*			  returned in P1                              */
/*								      */
/* Parameter4	    : --> count - for fragment copy                   */

                         
signed short int PASCAL ro1525_SearchIvkEle PROTO (( 
unsigned  long  int          ,
rT1530_stAsoContext  FAR *   ,    
unsigned  long  int  FAR * ));


/* Return value == 0: successfully.				      */
/*		!= 0: error					      */
/*								      */
/* Parameter1	    : --> Invoke Id                                   */
/*								      */
/* Parameter2	    : --> Pointer  to upper control context           */
/*								      */
/* Parameter3	    : --> pointer to Event Data which  is to be       */
/*			  searched for Invoke  Id                     */
/*								      */


signed short int PASCAL ro1526_MovePtrsOnePos PROTO ((
unsigned long int   FAR       *,
unsigned long int   FAR       *, 
rT1531_stInvokeData FAR * FAR *,
rT1531_stInvokeData FAR * FAR *)); 


/* Return value == 0: successfully.				      */
/*		!= 0: error					      */
/*								      */
/* Parameter1	    : --> Pointer to previous Invoke Data  handle     */
/*								      */
/* Parameter2	    : --> Pointer to current Invoke Data  handle      */
/*								      */
/* Parameter3	    : --> Pointer to previous Invoke Data  structure  */
/*								      */
/* Parameter4	    : --> Pointer to current Invoke Data  structure   */
/*								      */

#endif	/* _ROUCTR5_H */
