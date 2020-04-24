/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rodpat5.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:37  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:34:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:11  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:34  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:10  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:09  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:02  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:15  keutel
 * 	creation
 * 	[1994/03/21  13:26:27  keutel]
 * 
 * $EndLog$
 */
#ifndef _RODPAT5_H
#define _RODPAT5_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rodpat5_rcsid[] = { "@(#) $RCSfile: rodpat5.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:37 $" } ;
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
/* NAME         : rodpat5.h                                           */
/*                                                                    */
/* AUTHOR       : Hubert Trieb                                        */
/* DATE         : 92-02-18                                            */
/*                                                                    */
/* COMPONENT    : RTS dispatcher                                      */
/*                                                                    */
/* PRD#/VERS.	: RTS-V02.00					      */
/*                                                                    */
/* DESCRIPTION  : Dispatcher - recovery interface                     */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY	: sccsid  = "@(#)rodpat5.h  1.1  93/11/29"			      */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 2.00 K0  | 92-03-04|  original                      | ht |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************** Initialisaton of recovery ********************* */
/* ******************				********************* */

signed short int PASCAL ro0280_openRdb PROTO ((
    signed short int          mode,
    signed short int          lock_now,
    unsigned long  int FAR *  hRdbRef,
    unsigned long  int        lSyName,
    char               FAR *  pSyName));

/* Return value == 0: OK.
                != 0: Error

   - mode           : --> Mode for recovery.
			  0: RTNORECOVER, >0: RTDELANDRECOVER, <0: RTRECOVER
   - lock_now       : --> Rdb file lock parameter
			  0   : normal use in RTS;
			  !=0 : used in status function; The file is locked
				because it may not be changed while reading
				the status.
   - hRdbRef        : <-- reference to recovery managementblock.
   - lSyName        : --> Length of recovery file name.
   - pSyName        : --> Name of recovery file (pointer to).
*/


/* ****************** closing recovery information ****************** */
/* ******************                              ****************** */


signed short int PASCAL ro0281_closeRdb PROTO ((
    unsigned long  int       hRdbRef));

/* Return value == 0: OK.
                != 0: Error

   - hRdbRef        : --> reference to recovery managementblock.
*/


/* ****************** reading recovery information ****************** */
/* ******************                              ****************** */

/* ****************** reading information of recovery data ********** */

signed short int PASCAL ro0282_readinfoRdb PROTO ((
    unsigned long  int        hRdbRef,
    signed   short int FAR *  sBlkno ,
    unsigned short int FAR *  sType  ,
    unsigned long  int FAR *  ulN1   ,
    unsigned long  int FAR *  ulN2   ,
    unsigned long  int FAR *  ulN3   ,
    unsigned long  int FAR *  datalen));

/* Return value == 0: OK.
		>  0: returned when the end of file is reached and no
		      further Rdb data are available;
		      The returned values are undefined;
		<  0: Error

   - hRdbRef        : <-> reference to recovery managementblock.
   - sBlkno         : <-- Number of virtual block in recovery data base.
   - sType          : <-- Type of datablock ( > 0).
   - ulN1           : <-- Parameter 1 of dispatcher.
   - ulN2           : <-- Parameter 2 of dispatcher.
   - ulN3           : <-- Parameter 3 of dispatcher.
   - datalen        : <-- Length of main datablock.
*/

/* ****************** function 1, reading of recovery data ********** */
/* ****************** reading function 1 of recovery data ********** */

signed short int PASCAL ro0283_read1seqRdb PROTO ((
    unsigned long  int        hRdbRef,
    char               FAR *  mainblock,
    unsigned long  int FAR *  hdl1   ,
    unsigned long  int FAR *  hdl2   ,
    unsigned long  int FAR *  hdl3   ,
    unsigned long  int FAR *  hdl4  ));

/* Return value == 0: OK.
                != 0: Error

   - hRdbRef    : <-> reference to recovery managementblock.
   - mainblock  : <-> main data block, area allocate by caller.
   - hdl1       : <-- handle to data block allocated by read function.
   - hdl2       : <-- handle to data block allocated by read function.
   - hdl3       : <-- handle to data block allocated by read function.
   - hdl4       : <-- handle to data block allocated by read function.
*/

/* ********** function 2, reading of recovery data into one block ******* */

signed short int PASCAL ro0284_read2seqRdb PROTO ((
    unsigned long  int        hRdbRef,
    unsigned long  int FAR *  hmainblock));

/* Return value == 0: OK.
                != 0: Error

   - hRdbRef        : <-> reference to recovery managementblock.
   - hmainblock     : <-- main data block of PM (pointer to handle),
			  header with handle and length fields at begin.
*/


/* ****************** Writing recovery information ******************** */
/* ******************                              ******************** */

/* ****************** Function 1 for writing       ******************** */

signed short int PASCAL ro0285_write1Rdb PROTO ((
    unsigned long  int   hRdbRef,
    signed   short int   sBlkno ,
    unsigned short int   sType  ,
    unsigned long  int   ulN1   ,
    unsigned long  int   ulN2   ,
    unsigned long  int   ulN3   ,
    char           FAR * mainblock,
    unsigned long  int   mainlen  ,
    unsigned long  int   hdl1   ,
    unsigned long  int   len1   ,
    unsigned long  int   hdl2   ,
    unsigned long  int   len2   ,
    unsigned long  int   hdl3   ,
    unsigned long  int   len3   ,
    unsigned long  int   hdl4   ,
    unsigned long  int   len4 ));


/* Return value == 0: OK.
                != 0: Error

   - hRdbRef    : <-> reference to recovery managementblock.
   - sBlkno     : --> Number of virtual block in recovery data base.
   - sType      : --> Type of datablock.
   - ulN1       : --> Parameter 1 of dispatcher.
   - ulN2       : --> Parameter 2 of dispatcher.
   - ulN3       : --> Parameter 3 of dispatcher.
   - mainblock  : --> main datablock
   - mainlen    : --> length of main datablock
   - hdl1       : --> handle to data block1.
   - len1       : --> length of data block1.
   - hdl2       : --> handle to data block2.
   - len2       : --> length of data block2.
   - hdl3       : --> handle to data block3.
   - len3       : --> length of data block3.
   - hdl4       : --> handle to data block4.
   - len4       : --> length of data block4.

   if (lenx == 0)  no information is writen;
*/


/* ****************** Function 2 for writing         ******************** */
signed short int PASCAL ro0286_write2Rdb PROTO ((
    unsigned long  int       hRdbRef,
    signed   short int       sBlkno ,
    unsigned short int       sType  ,
    unsigned long  int       ulN1   ,
    unsigned long  int       ulN2   ,
    unsigned long  int       ulN3   ,
    unsigned long  int       hmainblock));

/* Return value == 0: OK.
                != 0: Error

   - hRdbRef        : <-> reference to recovery managementblock.
   - sBlkno         : --> Number of virtual block in recovery data base.
   - sType          : --> Type of datablock.
   - ulN1           : --> Parameter 1 of dispatcher.
   - ulN2           : --> Parameter 2 of dispatcher.
   - ulN3           : --> Parameter 3 of dispatcher.
   - hmainblock     : --> main datablock of PM; (handle to rT0200_rdbHeader)
			  header with handle and length fields at begin.
*/


/* ****************** Reserve new block number ********************** */
/* ******************                          ********************** */

signed short int PASCAL ro0287_giveRdb PROTO ((
    unsigned long  int  hRdbRef));


/* Return value  < 0: Error
		 > 0: number of virtual block in recovery database.

   - hRdbRef        : <-> reference to recovery managementblock.

*/

/* ****************** Delete and free recoveryblock ******************* */
/* ******************                               ******************* */

signed short int PASCAL ro0288_freeRdb  PROTO ((
    unsigned long  int hRdbRef,
    signed   short int sBlkno ));

/* Return value == 0: OK.
                != 0: Error

   - hRdbRef        : <-> reference to recovery managementblock.
   - sBlkno         : --> Number of virtual block in recovery data base.
*/

/* ****************** Read Rdb and assign to Acb/Ecb ****************** */
/* ******************                                ****************** */

signed short int PASCAL ro0289_readRdbRtpm PROTO ((
			char               FAR *pSyName,
			unsigned       int      lSyName,
			unsigned long  int FAR *hEcbField,
			signed   short int FAR *lenEcb,
			unsigned long  int FAR *hAcbField,
			signed   short int FAR *lenAcb));

#endif	/* _RODPAT5_H */
