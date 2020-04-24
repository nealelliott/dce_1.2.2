/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stdeflen.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:37  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:55  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:15:21  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:03:23  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:56  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:17  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:00  keutel
 * 	creation
 * 	[1994/03/21  13:15:50  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stdeflen.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:47 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1992                     */
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
/* NAME         : stdeflen.c                                          */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-06-08                                            */
/*                                                                    */
/* COMPONENT    : ENCODING length only in long model                  */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : T - 84 XXXX    After the TAG the LENGTH is encoded  */
/*               as a LONG of four bytes.                             */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)stdeflen.c	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-06-08|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/
static char *sccsid = " @(#) stdeflen.c 1.1 93/11/29  ST_MAVROS";  

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>
#include <routil0.h>                   
#include <asn1st.h> 

#define BDL 5
/* length of intermediat buffer */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: asn1st_deflencod				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-06-08                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

int PASCAL asn1st_deflencod (
STOREHANDLE hdl, 
long  poszm)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              : Handle to store, Length               */
/*                                                                    */
/* OUT-PARAMETERS             : None                                  */
/*                                                                    */
/* RETURN VALUES              :   >= 0  Success                       */
/*                                <  0  Error                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  The Store r/w position is at the     */
/*                               end of User data whose length has    */
/*                               to be coded                          */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
 signed short int rc = OK;
 signed long  int lIniPos = NULL;
 signed long  int lLen    = NULL; 
 int n;                                                                  
 unsigned char interbuf[BDL];

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
 lIniPos= ro0076_stwhere(hdl);
 lLen = lIniPos - (poszm + sizeof(long)  + 1);
 rc =ro0066_storepos (hdl,poszm);
 if (rc == 0  ){
        for(n=4;n > 0 ;) {                                                     
            interbuf[n--] = (unsigned char) (lLen % 256);  
            lLen = lLen / 256 ;                                             
        }                                                               
        interbuf[0] = 128 +BDL-1;
        ro0068_stputm(hdl,(char FAR *)&interbuf[0],BDL); 
        rc = ro0066_storepos(hdl,lIniPos);                          
  }
 return(ro0063_sterror(hdl));                                            
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: asn1st_deflenpos  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-06-08                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

int PASCAL asn1st_deflenpos(
STOREHANDLE hdl)
/*                                                                    */
/* DESCRIPTION  :     allocate space for def length encoding &        */
/*                    positions the store at the end of  length.      */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   >= 0  Success                       */
/*                                <  0  Error                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
char FAR * cBuf = NULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
   ro0067_stputc(hdl,128);
   cBuf =  ro0069_stputs(hdl,sizeof(long));
   /*
   if (cBuf) {
      ro0043_memset (cBuf , 0 , sizeof(long));
      ro0066_storepos (hdl , (poszm+sizeof(long)+1));
   } */
  return(ro0063_sterror(hdl));                                            
      
}/*end*/                                                                        
