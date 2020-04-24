/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rodpat32.c,v $
 * Revision 1.1.4.2  1996/03/11  13:21:48  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:33  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:38:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:15  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:08:46  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:51:47  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:04  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:00:23  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:33  keutel
 * 	creation
 * 	[1994/03/21  13:29:29  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rodpat32.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:48 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1991 BY SIEMENS NIXDORF INFORMATIONS AG	      */
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
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : rodpat32.c                                          */
/*                                                                    */
/* AUTHOR       : Hubert Trieb                                        */
/* DATE         : 92-02-20                                            */
/*                                                                    */
/* COMPONENT    : RTROS dispatcher                                    */
/*                                                                    */
/* PRD#/VERS.	: RTROS-V03.00					      */
/*                                                                    */
/* DESCRIPTION  : Management of Recovery Data Base (Rdb);             */
/*                interface functions                                 */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/* V02.00 K0| 92-02-20|  original                      | ht |         */
/* V03.00 K0| 93-05-31|  ifndef for ROS product        | ma |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) rodpat32.c 1.1 93/11/29   RTS-V2.00";

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>	 /* RTS general definitions		     */
#include <roerval.h>	 /* RTS error values			     */
#include <routil0.h>	 /* RTS utilities interface		     */
#include <rouply0.h>     /* RTS common upper layer definitions       */

#include <rodpat0.h>     /* RTS dispatcher structures                */
#include <rodpat5.h>     /* RTS recovery interface                   */

/*********************************************************************/
/*                                                                   */
/*              L O C A L  V A R I A B L E S                         */
/*                                                                   */
/*********************************************************************/

/* ****************** Logging function name strings ***************** */
/* ******************                               ***************** */

RM0001_STRG (rs0280, "ro0280_openRdb ")
RM0001_STRG (rs0281, "ro0281_closeRdb ")
RM0001_STRG (rs0282, "ro0282_readinfoRdb ")
RM0001_STRG (rs0283, "ro0283_read1Rdb ")
RM0001_STRG (rs0284, "ro0284_read2Rdb ")
RM0001_STRG (rs0285, "ro0285_write1Rdb ")
RM0001_STRG (rs0286, "ro0286_write2Rdb ")
RM0001_STRG (rs0287, "ro0287_giveRdb ")
RM0001_STRG (rs0288, "ro0288_freeRdb ")


#ifndef RC0280_FILERECOVER                              

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0280_openRdb                                      */
/*                                                                    */
/* AUTHOR       : Hubert Trieb                                        */
/* DATE         : 92-02-20                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/* signed short int PASCAL ro0280_openRDB (mode, lock_now, hRdbRef,   */
/*                                         lSyName, pSyName)          */
/*                                                                    */
/* signed short int         mode;       open mode parameter           */
/* signed short int         lock_now;   Rdb file lock parameter       */
/* unsigned long  int FAR * hRdbRef;    handle to rT0282_stRdbRef     */
/*                                      initialised structure is      */
/*                                      returned to caller;           */
/* unsigned long  int       lSyName;    length of RdbFile name        */
/* char               FAR * pSyName;    name of RdbFile               */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  : This function initializes the structure of type     */
/*                rT0282_stRdbRef which contains all information      */
/*                about the status of the recovery.                   */
/*                The initialisation is depending on the open "mode " */
/*                parameter :                                         */
/*                  0 : no recovery                                   */
/*                      an empty RdbFile is created or an existing    */
/*                      file is deleted. *hRdbRef is set to 0.        */
/*                > 0 : delete and recover                            */
/*                      an empty RdbFile is created and hRdbRef is    */
/*                      inisialised with the default values.          */
/*                < 0 : recover                                       */
/*                      the RdbFile(pSyName) is opened and hRdbRef is */
/*                      initialized with the content of the rdbFile.  */
/*                      In this mode the read functions can be used   */
/*                      for getting information of the recovery.      */
/*                                                                    */
/*                parameter "lock_now"  indicates if the RdbFile      */
/*                should be locked in ro0280_openRdb.                 */
/*                   0 : The RdbFile is not locked. This is the normal*/
/*                       use of the parameter because locking is done */
/*                       in ro028x_writexRdb.                         */
/*                != 0 : Used in status function                      */
/*                       The RdbFile is locked when opening the file. */
/*                       Only efficient if mode is < 0.               */
/*                       The file will be locked until ro00fb_funlock */
/*                       is called.                                   */
/*                       If ro028x_writexRdb is called the file is    */
/*                       also unlocked, because in ro028x_writeRdb    */
/*                       the file is locked first and then unlocked.  */
/*                                                                    */
/*inoff ***************************************************************/

signed short int PASCAL ro0280_openRdb (
signed short int         mode,
signed short int         lock_now,
unsigned long  int FAR * hRdbRef,
unsigned long  int       lSyName,
char               FAR * pSyName)

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
RM0002_ENTRY (rs0280)

    *hRdbRef = 0;
    RM0003_RETURN (rs0280, signed short int, 0)

}

/* ****************************************************************** */

signed short int PASCAL ro0281_closeRdb (
unsigned long  int     hRdbRef)

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

    return (-R_RDBINT);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0282_readinfo                                     */
/*                                                                    */
/* AUTHOR       : Hubert Trieb                                        */
/* DATE         : 92-02-20                                            */
/*                                                                    */
/* DESCRIPTION  : reads the dp information sBlkno,sType,ulN1,ulN2,    */
/*                ulN3 and the length of the main datablock.          */
/*                After returning, the file pointer stands at the     */
/*                beginning of the main datablock, the next action    */
/*                should be a read1seqRdb() or read2seqRdb            */
/*                                                                    */
/*inoff ***************************************************************/

signed short int PASCAL ro0282_readinfoRdb ( 

unsigned long  int        hRdbRef,
signed   short int FAR *  sBlkno,
unsigned short int FAR *  sType,
unsigned long  int FAR *  ulN1,
unsigned long  int FAR *  ulN2,
unsigned long  int FAR *  ulN3,
unsigned long  int FAR *  datalen)

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

    return (-R_RDBINT);
}

/*  ***************************************************************** */

signed short int PASCAL ro0283_read1seqRdb ( 

unsigned long  int        hRdbRef,
char               FAR *  mainblock,
unsigned long  int FAR *  hdl1,
unsigned long  int FAR *  hdl2,
unsigned long  int FAR *  hdl3,
unsigned long  int FAR *  hdl4)

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/
    return (-R_RDBINT);
}

/*  ***************************************************************** */

signed short int PASCAL ro0284_read2seqRdb ( 

unsigned long  int        hRdbRef,
unsigned long  int FAR *  hmainblock)

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

    return (-R_RDBINT);

}

/*  ***************************************************************** */


signed short int PASCAL ro0285_write1Rdb  ( 
unsigned long  int   hRdbRef,
signed   short int   sBlkno ,
unsigned short int   sType  ,
unsigned long  int   ulN1   ,
unsigned long  int   ulN2   ,
unsigned long  int   ulN3   ,
char           FAR * mainblock,
unsigned long  int   mainlen,
unsigned long  int   hdl1   ,
unsigned long  int   len1   ,
unsigned long  int   hdl2   ,
unsigned long  int   len2   ,
unsigned long  int   hdl3   ,
unsigned long  int   len3   ,
unsigned long  int   hdl4   ,
unsigned long  int   len4   )

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

    return (-R_RDBINT);

}

/*  ***************************************************************** */

signed short int PASCAL ro0286_write2Rdb  ( 
unsigned long  int   hRdbRef,
signed   short int   sBlkno ,
unsigned short int   sType  ,
unsigned long  int   ulN1   ,
unsigned long  int   ulN2   ,
unsigned long  int   ulN3   ,
unsigned long  int   hmainblock)

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

    return (-R_RDBINT);

}

/*  ***************************************************************** */


signed short int PASCAL ro0287_giveRdb (
unsigned long  int  hRdbRef)

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

    return (-R_RDBINT);
}

/*  ***************************************************************** */

signed short int PASCAL ro0288_freeRdb ( 
unsigned long  int hRdbRef,
signed   short int sBlkno)

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

    return (-R_RDBINT);
}

/*  ***************************************************************** */

#endif
