/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rodpat1.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:17  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:26  root]
 * 
 * Revision 1.1.2.5  1994/08/24  08:35:17  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:33:51  marrek]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:31  keutel
 * 	OT 11620
 * 	[1994/08/19  13:47:03  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:17  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:02  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:17  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:04  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:37  keutel
 * 	creation
 * 	[1994/03/21  13:17:29  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rodpat1.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:25 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c) 1991 by                                                  *
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

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME 	: rodpat1.c					      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-10-31					      */
/*                                                                    */
/* COMPONENT	: RTS dispatcher  (Steuerwerk)			      */
/*                                                                    */
/* PRD#/VERS.	: RTS-V02.00					      */
/*                                                                    */
/* DESCRIPTION	: Central component of RTS			      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  | KZ | CR# FM# */
/* 02.00 K0 | 91-10-31|  original		       | jm |	      */
/*datoff **************************************************************/

/**********************************************************************/
/*                Module + Product Identification                     */
/**********************************************************************/

/** static char *sccsid =    "@(#) rodpat1.c 1.2 93/12/20   RTROS-V03.00"; **/
#ifndef RP0000_MicV0600
/*
static char *sccsid =    "@(#)rodpat1.c" SNI_VERSION " RTROS-V03.00";
*/
#endif
/*********************************************************************/
/*              I N C L U D E S                                      */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>	 /* RTS general definitions		     */
#include <roerval.h>	 /* RTS error values			     */

#include <routil0.h>	 /* RTS utilities interface		     */

#include <rouply0.h>	 /* RTS common upper layer definitons	     */

#include <rodpat0.h>	 /* RTS dispatcher pm interface 	     */
#include <roloif2.h>	 /* RTS lower interface calling interface    */
#include <rotime2.h>	 /* RTS timer calling interface 	     */

#include <roppm00.h>     /* RTS presentation pm interface            */

#include <rodpat1.h>	 /* RTS dispatcher ppm interface	     */
#include <rodpat2.h>	 /* RTS dispatcher external interface	     */
#include <rodpat3.h>	 /* RTS dispatcher configuration interface   */
#include <rodpat4.h>	 /* RTS dispatcher internal definitions      */
#include <rodpat5.h>	 /* RTS dispatcher recovery interface	     */

#include <romoni00.h>    /* service interfce for PDU monitor         */
 
/*********************************************************************/
/*                                                                   */
/*		P R O T O T Y P I N G	 O F   S T A T I C	     */
/*                                                                   */
/*		F U N C T I O N S				     */
/*                                                                   */
/*********************************************************************/
signed short int PASCAL ro0233_getTpmFlowCtrl(unsigned long int ecb);

static
signed short   int PASCAL NEAR ro0200_crtCb PROTO ((unsigned long  int FAR  *,
						    signed   short int FAR  *,
						    rT0255_cb		   **,
						    signed   long  int	     ));
static
signed short   int PASCAL NEAR ro0201_ctRdb PROTO ((unsigned long  int FAR  *,
						    signed   short int FAR  *,
						    rT0255_cb		   **,
						    unsigned long  int	     ,
						    signed   long  int	     ));
static
signed short   int PASCAL NEAR ro0202_crtQueue	PROTO ((void));

static
signed short   int PASCAL NEAR ro0203_clrQueue	PROTO ((unsigned long  int ,
							signed	 short int ));


static
signed short   int PASCAL NEAR ro0204_clrPcb	PROTO ((unsigned long  int));

static
signed short   int PASCAL NEAR ro0205_clrAcb	PROTO ((unsigned long  int));

static
signed	 short int PASCAL NEAR ro0206_lockSysVa PROTO ((unsigned long  int));

static
void		   PASCAL NEAR ro0207_unlockSys PROTO ((void));

static
unsigned long  int PASCAL NEAR ro0208_srNaAcb	PROTO ((unsigned long  int  ,
							char	       FAR *,
							unsigned long  int  ,
							char	       FAR *,
							unsigned long  int  ,
							char	       FAR *));

static
signed	 short int PASCAL NEAR ro0209_dispatch	PROTO ((void));


/*********************************************************************/
/*                                                                   */
/*              L O C A L  V A R I A B L E S                         */
/*                                                                   */
/*********************************************************************/

/* ****************** Systems maintaining variables ***************** */
/* ******************				    ***************** */

static rT0254_scb   NEAR *scbQuPtr   = (rT0254_scb NEAR   *) 0;
static unsigned long int  scbQuHd    = 0;
static signed  short int  scbQuNo    = 0;

/* ****************** System description variables ****************** */
/* ******************                              ****************** */

static rT0250_mcb   NEAR *workEque   = (rT0250_mcb NEAR   *) 0;
static rT0251_acb   NEAR *workAcbs   = (rT0251_acb NEAR   *) 0;
static rT0252_ecb   NEAR *workEcbs   = (rT0252_ecb NEAR   *) 0;
static rT0253_pcb   NEAR *workPcbs   = (rT0253_pcb NEAR   *) 0;
static rT0254_scb   NEAR *workScb    = (rT0254_scb NEAR   *) 0;
static unsigned long int  workScbId  = 0;

/* ****************** Event stack variables ************************* */
/* ******************                       ************************* */

static signed short int   evCout = -1;
static rT0250_mcb         evTabl[RC0250_StackSize];
static rT0250_mcb         evRunn;
static rT0251_acb   NEAR *runAcb = (rT0251_acb NEAR *) 0;

/* ****************** Dispatcher panic state flag ******************* */
/* ******************				  ******************* */

static signed  short int  dpatPanic  = 0;

/* ****************** Logging function name strings ***************** */
/* ******************                               ***************** */

RM0001_STRG (rs0200, "ro0200_crtCb   ")
RM0001_STRG (rs0201, "ro0201_ctRdb   ")
RM0001_STRG (rs0202, "ro0202_crtQueue")
RM0001_STRG (rs0203, "ro0203_clrQueue")
RM0001_STRG (rs0204, "ro0204_clrPcb  ")
RM0001_STRG (rs0205, "ro0205_clrAcb  ")
RM0001_STRG (rs0206, "ro0206_lockSysV")
RM0001_STRG (rs0207, "ro0207_unlockSy")
RM0001_STRG (rs0208, "ro0208_srNaAcb ")
RM0001_STRG (rs0209, "ro0209_dispatch")

RM0001_STRG (rs0210, "ro0210_popInEve")
RM0001_STRG (rs0211, "ro0211_pushOutE")
RM0001_STRG (rs0212, "ro0212_regPmInv")
RM0001_STRG (rs0213, "ro0213_regPmDes")
RM0001_STRG (rs0214, "ro0214_getAcbEc")
RM0001_STRG (rs0215, "ro0215_getEiCxt")
RM0001_STRG (rs0216, "ro0216_getPrBuf")
RM0001_STRG (rs0217, "ro0217_getAddre")
RM0001_STRG (rs0218, "ro0218_conAddre")
RM0001_STRG (rs0219, "ro0219_pushOnAs")
RM0001_STRG (rs0220, "ro0220_pushNewA")
RM0001_STRG (rs0221, "ro0221_pushOnIn")
RM0001_STRG (rs0222, "ro0222_getAddrL")
RM0001_STRG (rs0223, "ro0223_getScb  ")
RM0001_STRG (rs0224, "ro0224_nameAcb ")
RM0001_STRG (rs0225, "ro0225_conAcbs ")
RM0001_STRG (rs0226, "ro0226_changeSt")
RM0001_STRG (rs0227, "ro0227_setSpecF")
RM0001_STRG (rs0228, "ro0228_getTpduF")
RM0001_STRG (rs0229, "ro0229_setLoifI")
RM0001_STRG (rs0230, "ro0230_seaLoifI")
RM0001_STRG (rs0231, "ro0231_getInfor")    /* vinita */
RM0001_STRG (rs0232, "ro0232_getAddre")    /* kc */
RM0001_STRG (rs0233, "ro0233_getTpmFl")    /* kc K02*/

RM0001_STRG (rs0235, "ro0235_regPmRdb")
RM0001_STRG (rs0236, "ro0236_regPmRdb")
RM0001_STRG (rs0237, "ro0237_pmRdbWri")
RM0001_STRG (rs0238, "ro0238_resetExA")

RM0001_STRG (rs0240, "ro0240_rtpmSynt")   /* shiv    */
RM0001_STRG (rs0241, "ro0241_rtpmSynt")   /* shiv    */
RM0001_STRG (rs0242, "ro0242_encodSni")   /* vinita  */
RM0001_STRG (rs0243, "ro0243_decodSni")   /* vinita  */
RM0001_STRG (rs0244, "ro0244_encodX41")   /* vinita  */
RM0001_STRG (rs0245, "ro0245_decodX41")   /* vinita  */
RM0001_STRG (rs0246, "ro0246_clearPdv")
RM0001_STRG (rs0247, "ro0247_encoding")
RM0001_STRG (rs0248, "ro0248_decoding")

RM0001_STRG (rs0250, "ro0250_attachSy")
RM0001_STRG (rs0251, "ro0251_detachSy")
RM0001_STRG (rs0252, "ro0252_clearSys")

RM0001_STRG (rs0260, "ro0260_attach  ")
RM0001_STRG (rs0261, "ro0261_detach  ")
RM0001_STRG (rs0262, "ro0262_provideB")
RM0001_STRG (rs0263, "ro0263_wake    ")
RM0001_STRG (rs0264, "ro0264_getPmInf")
RM0001_STRG (rs0265, "ro0265_getEcbIn")
RM0001_STRG (rs0266, "ro0266_getSysIn")
RM0001_STRG (rs0267, "ro0267_recover ")
RM0001_STRG (rs0268, "ro0268_setRecov")
RM0001_STRG (rs0269, "ro0269_AcbGetEc")

RM0001_STRG (rs0270, "ro0270_send    ")
RM0001_STRG (rs0271, "ro0271_receive ")
RM0001_STRG (rs0273, "ro0273_getPmInf")

RM0001_STRG (rs0274, "ro0274_getPrBfC")
RM0001_STRG (rs0275, "ro0275_ClearAll") /* kc K02 */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0200_crtCb                                        */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed   short int PASCAL NEAR ro0200_crtCb (
       unsigned long  int FAR      *cHd,
       signed   short int FAR      *cNo,
       rT0255_cb		 **cPtr,
       signed   long  int          size)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed	short int	    rc;
unsigned long int	    hd;
char		  NEAR	    *p;
rT0255_cb	  NEAR	   *cb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0200)

rc = 0;

if (! *cNo)  {
      *cHd = ro0030_localalloc (size * RC0251_StoreSize);

     if (*cHd) {
               *cNo  = RC0251_StoreSize;
	       *cPtr = (rT0255_cb NEAR *) ro0031_locallock (*cHd);

               if (! (*cPtr)) rc = -R_LLOCKE;
     } else    {
               rc  = -R_LALLCE;
     }
}

if (! rc) {
   p = (char NEAR *) *cPtr;

   for (rc = 0; rc < *cNo; rc++) {
	cb  = (rT0255_cb NEAR *) p;

	if (! (cb->stateFlg)) break;
	p  += size;
   }

   if (rc == *cNo) {
         if (ro0032_localunlock (*cHd)) rc = -R_LULOKE;

	 if (rc == *cNo) {
		hd = ro0033_localrealloc (*cHd				  ,
					  size * (*cNo) 		  ,
					  size * ((*cNo)+RC0251_StoreSize));
		if (hd) {
                       *cNo += RC0251_StoreSize;
                       *cHd  = hd;
		       *cPtr = (rT0255_cb NEAR *) ro0031_locallock (*cHd);

                       if (! (*cPtr)) rc = -R_LLOCKE;
                } else {
                       rc = -R_LRALLE;
                }
         }
   }
}

if (rc >= 0) rc++;

RM0003_RETURN (rs0200, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0201_ctRdb					      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 92-03-21					      */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed	short int PASCAL NEAR ro0201_ctRdb (
       unsigned long  int FAR      *cHd,
       signed   short int FAR      *cNo,
       rT0255_cb		 **cPtr,
       unsigned long  int	    eNo,
       signed   long  int          size)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed	short int   rc;
unsigned long int   hd;
unsigned long int eNo1;
unsigned long int eNo2;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0201)

rc   = 0;
eNo1 = eNo / RC0251_StoreSize;
eNo2 = eNo % RC0251_StoreSize;
if (eNo2) eNo = (eNo1 * RC0251_StoreSize) + RC0251_StoreSize;

if (! *cNo)  {
     *cHd = ro0030_localalloc (size * eNo);
     if (*cHd) {
	       *cNo  = (signed short int) eNo;  /* skh: Dos warning */
	       *cPtr = (rT0255_cb NEAR *) ro0031_locallock (*cHd);
               if (! (*cPtr)) rc = -R_LLOCKE;
     } else    {
	       rc    = -R_LALLCE;
     }
}

if ((! rc) && (*cNo < (signed short int)eNo)) {   /* skh: Dos warning */
      if (ro0032_localunlock (*cHd)) rc = -R_LULOKE;
      if (! rc) {
	    hd = ro0033_localrealloc (*cHd	   ,
				      size * (*cNo),
				      size *	eNo);
	    if (hd) {
		   *cNo  = (signed short int) eNo;   /* skh: Dos warning */
		   *cHd  =  hd;
		   *cPtr = (rT0255_cb NEAR *) ro0031_locallock (*cHd);

		   if (! (*cPtr)) rc = -R_LLOCKE;
	    } else {
		   rc = -R_LRALLE;
	    }
      }
}

RM0003_RETURN (rs0201, signed short int, rc)
}/*end*/


/*exon ************************************************************** */
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0202_crtQueue				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed short int PASCAL	NEAR ro0202_crtQueue (void)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ************************************************************* */

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int     rc;       /* Variable used to compute the return */
                               /* value of this function.             */

unsigned long int   aHd;       /* Auxiliary Variable used for realloc */
                               /* queue.                              */

rT0250_mcb  NEAR   *apt;       /* Auxiliary Variable used for realloc */
                               /* queue.                              */

signed short int      i;       /* Loop counter.                       */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0202)

rc = 0;

if (! (workScb->uEvntQNo)) {
   workScb->uEvntQhd = ro0030_localalloc (sizeof (rT0250_mcb) *
					  RC0251_StoreSize    );
   if (workScb->uEvntQhd) {
	     workScb->uEvntQNo = RC0251_StoreSize;

	     workEque	       = (rT0250_mcb NEAR *)
				 ro0031_locallock (workScb->uEvntQhd);

	     if (! workEque) rc = -R_LLOCKE;
   } else    {
	     rc = -R_LALLCE;
   }
}
if (rc) RM0003_RETURN (rs0202, signed short int, rc)


if  (workScb->uEvnTail == -1) {
       workScb->uEvnTail = 0;
} else {
       (workScb->uEvnTail)++;
       if (workScb->uEvnTail >= workScb->uEvntQNo) workScb->uEvnTail = 0;

       if (workScb->uEvnTail == workScb->uEvnHead) rc = 1;
}
if (! rc) RM0003_RETURN (rs0202, signed short int, rc)


aHd = ro0030_localalloc (sizeof (rT0250_mcb) *
			 (RC0251_StoreSize + workScb->uEvntQNo));

if (! aHd) RM0003_RETURN (rs0202, signed short int, -R_LALLCE)

apt = (rT0250_mcb NEAR *) ro0031_locallock (aHd);
if (! apt) {
/*    ro0034_localfree (aHd);    ** SKH ** Why required ? */
      RM0003_RETURN (rs0202, signed short int, -R_LLOCKE)
}

i = 0;
do  {
    apt[i] = workEque[workScb->uEvnHead];
    (workScb->uEvnHead)++;
    i++;
    if (workScb->uEvnHead == workScb->uEvntQNo) workScb->uEvnHead = 0;
}
while (workScb->uEvnHead != workScb->uEvnTail);

workScb->uEvnHead  = 0;
workScb->uEvnTail  = workScb->uEvntQNo;
workScb->uEvntQNo += RC0251_StoreSize;
workEque	   = apt;

     if (ro0032_localunlock (workScb->uEvntQhd)) rc = -R_LULOKE;
else if (ro0034_localfree   (workScb->uEvntQhd)) rc = -R_LFREEE;
else						 rc = 0;

workScb->uEvntQhd = aHd;

RM0003_RETURN (rs0202, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0203_clrQueue				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

static signed short int PASCAL NEAR ro0203_clrQueue(	
	  unsigned long  int ecbAcb,
	  signed   short int m)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int	rc;
signed short int      cFlg;
rT0250_mcb   NEAR    *pmcb;
rT0251_acb   NEAR    *pacb;
signed short int	 i;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0203)

rc = 0;

for (i = 0; i < workScb->uEvntQNo; i++) {
    pmcb = workEque + i;

    if (pmcb->event > 0)  {
	   pacb = workAcbs + (pmcb->toAcb) - 1;

	   switch (m) {
	   case 0 : cFlg = (pacb->toEcb == ecbAcb) ? 1 : 0; break;
	   case 1 : cFlg = (pmcb->toAcb == ecbAcb) ? 1 : 0; break;
	   default: cFlg = 1;				    break;
	   }
    } else {
	   cFlg = 0;
    }

    if (cFlg) {
	rc = ro0291_cfClFct (pacb->configNo ,
			     pmcb->autoNo   ,
			     pmcb->event    ,
			     pmcb->params[0],
			     pmcb->params[1],
			     pmcb->params[2],
			     pmcb->params[3]);
	if (! rc) {
		(pacb->evtCount)--;
		pmcb->event = 0;
	} else	{
		break;
	}
    }
}

RM0003_RETURN (rs0203, signed short int, rc)
}/*end*/ 


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0221_pushOnInterface                              */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0221_pushOnInterface (
   signed   short int        m,
   signed   short int        autoNo,
   signed   long  int        event,
   unsigned long  int        p1,
   unsigned long  int        p2,
   unsigned long  int        p3,
   unsigned long  int        p4)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int        rc;    /* Variable used to compute the return */
                               /* value of this function.             */

rT0250_mcb   NEAR    *pmcb;    /* Auxiliary variable used to move the */
                               /* event values into the event stack   */
                               /* table.                              */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0221)

if (m) { rc = (runAcb->stateFlg & RC0259_EndPend) ? (-R_NASSOC) : 0; }
else   { rc = 0; event = -1;					     }

if (! rc) {
      if (m <  0) runAcb->stateFlg |= RC0259_EndPend;
      if (m < -1) rc		    = ro0203_clrQueue (evRunn.toAcb, 1);
}

if ((! rc) && (m < -1) && (! (runAcb->stateFlg & RC0257_ExActive))) {

      rc    = ro0291_cfClFct (runAcb->configNo, autoNo,
			      event,  p1,  p2,	p3, p4);
      event = 0;
}
if (rc || (! event)) RM0003_RETURN (rs0221, signed short int, rc);

rc = ro0202_crtQueue ();

if (! rc)  {
   pmcb = workEque + workScb->uEvnTail;

   (workScb->activScb)++;

   if (event > 0) {
	  (runAcb->evtCount)++;

          pmcb->autoNo    = autoNo;
	  pmcb->toAcb	  = evRunn.toAcb;

          pmcb->event     = event;
          pmcb->params[0] = p1;
          pmcb->params[1] = p2;
          pmcb->params[2] = p3;
          pmcb->params[3] = p4;
   } else {
	  pmcb->event	  = event;
          pmcb->params[2] = p3;

	  pmcb->params[0] =
	  pmcb->params[1] =
	  pmcb->params[3] = 0;
   }
}

RM0003_RETURN (rs0221, signed short int, rc)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0204_clrPcb 				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 92-03-02					      */
/*                                                                    */
/* SYNTAX       :                                                     */

   static signed   short int PASCAL NEAR ro0204_clrPcb (
	  unsigned long  int				ecb)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int rc;
signed short int  i;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0204)

rc = 0;

for (i = 0; i < workScb->prBufNo; i++) {

    if ((workPcbs[i].stateFlg	 ) &&
	(workPcbs[i].toEcb == ecb)) {

	rc = ro0221_pushOnInterface (0, 0, 0, 0, 0, workPcbs[i].stateFlg, 0);

	if (! rc) workPcbs[i].stateFlg = 0;
	else	  break;
    }
}

RM0003_RETURN (rs0204, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0205_clrAcb 				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 92-03-02					      */
/*                                                                    */
/* SYNTAX       :                                                     */

   static signed   short int PASCAL NEAR ro0205_clrAcb (
	  unsigned long  int				ecb)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int	rc;
rT0251_acb   NEAR    *pacb;
signed short int	 i;
signed short int	 j;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0205)

rc = 0;

for (i = 0; i < workScb->assocNo; i++) {
    pacb = workAcbs + i;

    if ((pacb->stateFlg    ) &&
	(pacb->toEcb == ecb) &&
	(pacb->cxtCount > 0)) {

	for (j = 0; j < RC0200_MaxAutomata; j++) {
	     if (pacb->pmcxt[j]) {
		  rc = ro0291_cfClFct (pacb->configNo, j, 0,
				       pacb->pmcxt[j], 0, 0, 0);
		  if (! rc) pacb->pmcxt[j] = 0;
		  else	    break;
	     }

	     if (pacb->pmRdbNo[j] > 0) {
		  rc = ro0288_freeRdb (workScb->hRdbFile, pacb->pmRdbNo[j]);

		  if (! rc) pacb->pmRdbNo[j] = 0;
		  else	    break;
	     }
	}

	if (! rc) pacb->stateFlg = RC0255_Idle;
	else	  break;
    }
}

RM0003_RETURN (rs0205, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0206_lockSysVa				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-02-28					      */
/*                                                                    */
/* SYNTAX       :                                                     */

   static signed   short int PASCAL NEAR ro0206_lockSysVa (
	  unsigned long  int hScb)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int rc;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0206)

if (dpatPanic) RM0003_RETURN (rs0206, signed short int, -R_DPPANC)

workScb = scbQuPtr + hScb - 1;

if ((hScb <		                    1)  ||
    (hScb >	   (unsigned long int)scbQuNo)  ||  /* skh: Dos Warning */
    (! (workScb->stateFlg)                   )) {
	rc	 = -R_NOSCBB;
	workScb  = 0;
} else {
       rc	 = 0;
       workScbId = hScb;

       if (workScb->prBufNo)  {
	   workPcbs = (rT0253_pcb NEAR *)
		       ro0031_locallock (workScb->prBufHd);

	   if (! workPcbs) rc = -R_LLOCKE;
       }

       if (workScb->appliNo)  {
	   workEcbs = (rT0252_ecb NEAR *)
		       ro0031_locallock (workScb->appliHd);

	   if (! workEcbs) rc = -R_LLOCKE;
       }

       if (workScb->assocNo)  {
	   workAcbs = (rT0251_acb NEAR *)
		       ro0031_locallock (workScb->assocHd);

	   if (! workAcbs) rc = -R_LLOCKE;
       }

       if (workScb->uEvntQNo) {
	   workEque = (rT0250_mcb NEAR *)
		       ro0031_locallock (workScb->uEvntQhd);

	   if (! workEque) rc = -R_LLOCKE;
       }

       if (rc) dpatPanic = 1;
}

RM0003_RETURN (rs0206, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0207_unlockSys				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 92-02-28					      */
/*                                                                    */
/* SYNTAX       :                                                     */

static void PASCAL NEAR ro0207_unlockSys (void)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0207)

if (workPcbs)  {
    if (! ro0032_localunlock (workScb->prBufHd))  workPcbs  = 0;
    else					  dpatPanic = 1;
}

if (workEcbs)  {
    if (! ro0032_localunlock (workScb->appliHd))  workEcbs  = 0;
    else					  dpatPanic = 1;
}

if (workAcbs)  {
    if (! ro0032_localunlock (workScb->assocHd))  workAcbs  = 0;
    else					  dpatPanic = 1;
}

if (workEque)  {
    if (! ro0032_localunlock (workScb->uEvntQhd)) workEque  = 0;
    else					  dpatPanic = 1;
}

workScb   = 0;
workScbId = 0;

RM0004_RETURN_VOID (rs0207)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0208_srNaAcb				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-12-05					      */
/*                                                                    */
/* SYNTAX       :                                                     */

   static unsigned long  int PASCAL NEAR ro0208_srNaAcb (
	  unsigned long  int  l1,
	  char		 FAR *p1,
	  unsigned long  int  l2,
	  char		 FAR *p2,
	  unsigned long  int  l3,
	  char		 FAR *p3)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

unsigned long int	rc;
signed	short int	 i;
rT0251_acb   NEAR    *pacb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0208)

rc = 0;

if ((! l1)  &&
    (! l2)  &&
    (! l3)) {

    RM0003_RETURN (rs0208, unsigned long int, rc)
}

for (i = 0; i < workScb->assocNo; i++) {

     pacb = workAcbs + i;

     if ((pacb->stateFlg !=	       RC0255_Idle) &&

	 (pacb->lAcbRef1 ==			l1) &&
	 (pacb->lAcbRef2 ==			l2) &&
	 (pacb->lAcbRef3 ==			l3) &&

	 (! ro0042_memcmp (pacb->cAcbRef1, p1, l1)) &&
	 (! ro0042_memcmp (pacb->cAcbRef2, p2, l2)) &&
	 (! ro0042_memcmp (pacb->cAcbRef3, p3, l3))) {

	 rc = i + 1;
	 break;
	 }
}

RM0003_RETURN (rs0208, unsigned long int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0209_dispatch				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

   static signed short int PASCAL NEAR ro0209_dispatch (void)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed   short int      rc;    /* Variable used to compute the return */
                               /* value of this function.             */
unsigned short int usCheckMoni = NULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0209)

while (evCout > -1) {
    evRunn = evTabl[evCout--];
    runAcb = workAcbs + evRunn.toAcb - 1;

    (runAcb->stkCount)--;
    ro0045_PduLog ( evRunn.autoNo,
                    evRunn.event,
                    evRunn.params[0]);
    usCheckMoni = ro00m1_moniON();

    if(usCheckMoni)
    {
      ro00m0_moniFct ( runAcb->toEcb ,       /*  hEcb     */
                       evRunn.toAcb ,        /*  hAcb     */ 
                       evRunn.autoNo ,       /*  sPmNo    */    
                       evRunn.event ,        /*  ulEvent  */ 
                       &evRunn.params[0] ,   /*  pP1      */
                       &evRunn.params[1] ,   /*  pP2      */
                       &evRunn.params[2] ,   /*  pP3      */ 
                       &evRunn.params[3]     /*  pP4      */
                     );  
    }

    rc = ro0290_cfPmFct (runAcb->configNo	     ,
			 evRunn.autoNo		     ,
			 runAcb->pmcxt[evRunn.autoNo]);

    if ((runAcb->stkCount < 1) &&
	(runAcb->cxtCount < 1)) {

	 runAcb->stateFlg &= (~RC0256_InActive);
    }

    if	(rc) {
	 evCout    = -1;
	 dpatPanic =  1;

	 if (rc > 0) rc = -rc;
    }
}

RM0003_RETURN (rs0209, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0210_popInEvent                                    */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   long int PASCAL  ro0210_popInEvent (
   unsigned long int FAR     *p1,
   unsigned long int FAR     *p2,
   unsigned long int FAR     *p3,
   unsigned long int FAR     *p4)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed long  int     event;    /* Variable used to compute the return */
                               /* value of this function.             */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0210)

if (p1) *p1 = evRunn.params[0];
if (p2) *p2 = evRunn.params[1];
if (p3) *p3 = evRunn.params[2];
if (p4) *p4 = evRunn.params[3];
event	    = evRunn.event;

RM0003_RETURN (rs0210, unsigned long int, event);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0211_pushOutEvent                                 */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0211_pushOutEvent (
   signed   short int        autoNo,
   signed   long  int        event,
   unsigned long  int        p1,
   unsigned long  int        p2,
   unsigned long  int        p3,
   unsigned long  int        p4)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int        rc;    /* Variable used to compute the return */
                               /* value of this function.             */

rT0250_mcb   NEAR    *pmcb;    /* Auxiliary variable used to move the */
                               /* event values into the event stack   */
                               /* table.                              */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0211)

if (evCout < RC0250_StackSize) {

      (runAcb->stkCount)++;

       pmcb            = &(evTabl[++evCout]);
       pmcb->toAcb     = evRunn.toAcb;
       pmcb->autoNo    = autoNo;
       pmcb->event     = event;
       pmcb->params[0] = p1;
       pmcb->params[1] = p2;
       pmcb->params[2] = p3;
       pmcb->params[3] = p4;
       rc              = 0;
} else {
       rc = -R_PUSHER;
}

RM0003_RETURN (rs0211, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0212_regPmInvoc                                                     */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0212_regPmInvoc (
   unsigned long  int        pmcxt)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int        rc;    /* Variable used to compute the return */
                               /* value of this function.             */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0212)

if (!  runAcb->pmcxt[evRunn.autoNo]) {

      (runAcb->cxtCount)++;
       runAcb->pmcxt[evRunn.autoNo]  = pmcxt;
       rc			     =	   0;
} else {
       rc = -R_PMINVE;
}

RM0003_RETURN (rs0212, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0213_regPmDestr                                   */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

   void PASCAL ro0213_regPmDestr (void)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0213)

if (runAcb->pmcxt[evRunn.autoNo])
   {
   runAcb->pmcxt[evRunn.autoNo] = 0;
   (runAcb->cxtCount)--;
   }

RM0004_RETURN_VOID (rs0213);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0214_getAcbEcb                                    */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long int PASCAL ro0214_getAcbEcb (
   unsigned long int FAR   *ecb)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0214)

if (ecb) *ecb = runAcb->toEcb;

RM0003_RETURN (rs0214, unsigned long  int, evRunn.toAcb);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0215_getEiCxt                                     */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0215_getEiCxt (
   signed   short int          lay,
   unsigned long  int FAR   *eiCxt)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int rc;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0215)

if ((-1 < lay) && (lay < RC0254_MaxLayerCxt)) {
       *eiCxt = workEcbs[(runAcb->toEcb) - 1].eiCxt[lay];
} else {
       *eiCxt = 0;
}
rc = (*eiCxt) ? 0 : 1;

RM0003_RETURN (rs0215, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0216_getPrBuffer                                  */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0216_getPrBuffer (
   unsigned long  int FAR   *buf)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int        rc;    /* Variable used to compute the return */
                               /* value of this function.             */

signed short int         i;    /* Auxiliary variable used in for      */
                               /* loop.                               */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0216)

rc = 1;

for (i = 0; i < workScb->prBufNo; i++) {
    if ((workPcbs[i].stateFlg              ) &&
        (workPcbs[i].toEcb == runAcb->toEcb)) {

        rc                   = 0;
	*buf		     = workPcbs[i].stateFlg;
	workPcbs[i].stateFlg = 0;
	break;
    }
}

RM0003_RETURN (rs0216, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0217_getAddress                                   */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0217_getAddress (
   unsigned long  int       adHd,
   signed   short int        lay,
   unsigned short int        len,
   char               FAR   *adr)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short  int	rc;
rT0252_ecb   NEAR    *pecb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0217)

if (! adHd) {
       pecb = workEcbs + (runAcb->toEcb) - 1;
       rc   = ro0306_GetAddress (pecb->myAddres, lay, len, adr);
} else {
       rc   = ro0306_GetAddress (adHd, lay, len, adr);
}

RM0003_RETURN (rs0217, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0218_conAddress                                   */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0218_conAddress (
   unsigned long  int FAR  *adHd,
   signed   short int        lay,
   unsigned short int        len,
   char               FAR   *adr)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/

RM0002_ENTRY  (rs0218)
RM0003_RETURN (rs0218, signed short int, ro0307_PutAddress(adHd,lay,len,adr))

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0215_pushOnAssoc                                  */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0219_pushOnAssoc (
   unsigned long  int        acb,
   signed   short int        autoNo,
   signed   long  int        event,
   unsigned long  int        p1,
   unsigned long  int        p2,
   unsigned long  int        p3,
   unsigned long  int        p4)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int        rc;    /* Variable used to compute the return */
                               /* value of this function.             */

rT0250_mcb   NEAR    *pmcb;    /* Auxiliary variable used to move the */
                               /* event values into the event stack   */
                               /* table.                              */

rT0251_acb   NEAR    *pacb;    /* Auxiliary variable used to          */
                               /* initialize new association control  */
                               /* block.                              */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0219)

pacb = workAcbs + acb - 1;

if (pacb->stateFlg & RC0256_InActive) {
       if (evCout < RC0250_StackSize) {

	     (pacb->stkCount)++;

	      pmcb	      = &(evTabl[++evCout]);
	      pmcb->toAcb     = acb;
	      pmcb->autoNo    = autoNo;
	      pmcb->event     = event;
	      pmcb->params[0] = p1;
	      pmcb->params[1] = p2;
	      pmcb->params[2] = p3;
	      pmcb->params[3] = p4;
	      rc	      = 0;
       } else {
	      rc = -R_PUSHER;
       }
} else {
       rc = -R_NASSOC;
}


RM0003_RETURN (rs0219, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0220_pushNewAssoc                                 */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0220_pushNewAssoc (
   unsigned long  int FAR   *acb,
   unsigned long  int        ecb,
   signed   short int        autoNo,
   signed   long  int        event,
   unsigned long  int        p1,
   unsigned long  int        p2,
   unsigned long  int        p3,
   unsigned long  int        p4)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int        rc;    /* Variable used to compute the return */
                               /* value of this function.             */

rT0250_mcb   NEAR    *pmcb;    /* Auxiliary variable used to move the */
                               /* event values into the event stack   */
                               /* table.                              */

rT0251_acb   NEAR    *pacb;    /* Auxiliary variable used to          */
                               /* initialize new association control  */
                               /* block.                              */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0220)

*acb = 0;

if (evCout <  RC0250_StackSize) {

       rc   = ro0200_crtCb ((unsigned long int  *)&workScb->assocHd       ,
                                                  &workScb->assocNo       ,
                                                  (rT0255_cb **) &workAcbs,
                                                  sizeof (rT0251_acb)     );

       if (rc >= 0) {
              pacb            = workAcbs + rc - 1;
              *acb            = rc;

	      pacb->stateFlg  = RC0256_InActive;
	      pacb->evtCount  = 0;
	      pacb->stkCount  = 1;
	      pacb->cxtCount  = 0;
	      pacb->toEcb     = ecb;
	      pacb->configNo  = workEcbs[ecb - 1].configNo;

	      pacb->lAcbRef1  =
	      pacb->lAcbRef2  =
	      pacb->lAcbRef3  = 0;

              pmcb            = &(evTabl[++evCout]);
              pmcb->toAcb     = *acb;
              pmcb->autoNo    = autoNo;
              pmcb->event     = event;
              pmcb->params[0] = p1;
              pmcb->params[1] = p2;
              pmcb->params[2] = p3;
              pmcb->params[3] = p4;

              rc              = 0;
       }
} else {
       rc = -R_PUSHER;
}

RM0003_RETURN (rs0220, signed short int, rc)
}/*end*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0222_getAddrLength				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   long int PASCAL ro0222_getAddrLength (
   unsigned long int FAR *addrLen,
   unsigned long int	     adHd)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

unsigned long int  rc;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0222)

if (adHd) { rc = ro0308_getAddrLen (addrLen, adHd);  }
else	  { rc = 0; *addrLen = 0;		     }

RM0003_RETURN (rs0222, long int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0223_getScb 				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

   unsigned long int PASCAL ro0223_getScb (void)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0223)
RM0003_RETURN (rs0223, unsigned long int, workScbId);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0224_nameAcb				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0224_nameAcb  (
   unsigned long  int  l1,
   char 	  FAR *p1,
   unsigned long  int  l2,
   char 	  FAR *p2,
   unsigned long  int  l3,
   char 	  FAR *p3)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int  rc;   /* skh: Dos warning */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0224)

if ((l1 > RC0252_MaxRef1u2) ||
    (l2 > RC0252_MaxRef1u2) ||
    (l3 > RC0253_MaxRef3  )) {

    RM0003_RETURN (rs0224, signed short int, -R_NAMLEN)
}

if (ro0208_srNaAcb (l1, p1, l2, p2, l3, p3)) {

       rc = 1;
} else {
       rc = 0;

       runAcb->lAcbRef1 = l1;
       runAcb->lAcbRef2 = l2;
       runAcb->lAcbRef3 = l3;

       if (l1) ro0041_memcpy (runAcb->cAcbRef1, p1, l1);
       if (l2) ro0041_memcpy (runAcb->cAcbRef2, p2, l2);
       if (l3) ro0041_memcpy (runAcb->cAcbRef3, p3, l3);
}

RM0003_RETURN (rs0224, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0225_conAcbs				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0225_conAcbs  (
   unsigned long  int  FAR  *cxt,
   signed   short int	    pmNo,
   unsigned long  int	      l1,
   char 	       FAR   *p1,
   unsigned long  int	      l2,
   char 	       FAR   *p2,
   unsigned long  int	      l3,
   char 	       FAR   *p3)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed	 short int     rc;     /* skh: Dos warning */
signed	 short int	i;
unsigned long  int oldAcb;
rT0251_acb   NEAR   *pacb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0225)

/* ****************** Check whether concatenation is allowed ******** */
/* ****************** or not				     ******** */

if ((runAcb->stateFlg & (~RC0256_InActive)) ||
    (runAcb->stkCount > 		 0) ||
    (runAcb->evtCount > 		 0)) {

       rc = -R_CONERR;
} else {
       oldAcb = ro0208_srNaAcb (l1, p1, l2, p2, l3, p3);
       rc = (oldAcb) ? 0 : RC0200_MaxAutomata;
}

pacb = workAcbs + oldAcb - 1;

for (i = rc; i < RC0200_MaxAutomata; i++) {
    if ((pacb  ->pmcxt[i]   != 0L) &&        /* skh: Dos warning */
	(runAcb->pmcxt[i]   != 0L))          /* skh: Dos warning */
    { rc = RC0200_MaxAutomata; break; }
}

if (rc) {
    *cxt = 0;
    RM0003_RETURN (rs0225, signed short int, rc);
}

/* ****************** Move pm invocations to acb found ************** */
/* ******************				       ************** */

pacb->loifId   = runAcb->loifId;
pacb->cxtCount = 0;

for (i = 0; i < RC0200_MaxAutomata; i++) {
    if (pacb->pmcxt[i] == 0)		 {
	pacb->pmcxt[i] = runAcb->pmcxt[i];
    }
}

for (i = 0; i < RC0200_MaxAutomata; i++) {
    if (pacb->pmcxt[i] != 0)		 {
	pacb->cxtCount++;
    }
}

for (i = 0; i < RC0200_MaxAutomata; i++) {
    if (pacb->pmRdbNo[i] == 0)		 {
	pacb->pmRdbNo[i] = runAcb->pmRdbNo[i];
    }
}

/* ****************** Clean up running acb ************************** */
/* ******************			   ************************** */

runAcb->stateFlg = RC0255_Idle;
runAcb->cxtCount = 0;

for (i = 0; i < RC0200_MaxAutomata; i++) runAcb->pmcxt	[i] = 0;
for (i = 0; i < RC0200_MaxAutomata; i++) runAcb->pmRdbNo[i] = 0;

/* ****************** Return context of pm and chanage ************** */
/* ****************** running acb.		       ************** */

*cxt	     = pacb->pmcxt[pmNo];
runAcb	     = pacb;
evRunn.toAcb = oldAcb;

RM0003_RETURN (rs0225, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0226_changeState				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-12-04					      */
/*                                                                    */
/* SYNTAX       :                                                     */

void PASCAL ro0226_changeState (
   unsigned long int pmcxt)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0226)

if (runAcb->pmcxt[evRunn.autoNo]) {
       if (! pmcxt) (runAcb->cxtCount)--;
} else {
       if (  pmcxt) (runAcb->cxtCount)++;
}

runAcb->pmcxt[evRunn.autoNo] = pmcxt;

RM0004_RETURN_VOID (rs0226)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0227_setSpecFlg				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

void PASCAL ro0227_setSpecFlg (void)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0227)

runAcb->stateFlg |= RC0258_SpecFlag;

RM0004_RETURN_VOID (rs0227)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0228_getTpduFct				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

FARPROC PASCAL ro0228_getTpduFct (void)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
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

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0228)
RM0003_RETURN (rs0228, FARPROC, workScb->tpduFct);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0229_setLoifId				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 92-03-17					      */
/*                                                                    */
/* SYNTAX       :                                                     */

void   PASCAL   ro0229_setLoifId (
   signed long int loifId)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0229)

runAcb->loifId = loifId;

RM0004_RETURN_VOID (rs0229)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0230_seaLoifId				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 92-03-17					      */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned long  int PASCAL ro0230_seaLoifId (
   signed   long  int	     loifId,
   signed   short int	       pmNo)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed	 short int	i;
signed	 long  int    acb;
rT0251_acb     NEAR *pacb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0230)

pacb = workAcbs;
acb  = 0;

for (i = 1; i <= workScb->assocNo; i++)  {
    if ((pacb->stateFlg        ) &&
	(pacb->pmcxt[pmNo]     ) &&
	(pacb->loifId == loifId)) {
	acb = i;
	break;
    }
    pacb++;
}

RM0003_RETURN (rs0230, unsigned long int, acb);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0231_getInform				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE         : 93-02-25                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int  PASCAL ro0231_getInform (
   unsigned long  int  acb,
   signed   short int  pmNo,
   signed   short int  inNo,
   void     FAR       *info)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int     rc;
rT0251_acb  NEAR  *pacb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0231)

if (((signed short int)acb < 1 || ((signed short int)acb > workScb->assocNo))) {
                                             /* skh: Dos warning */
   rc = -R_WASSOC;
}
else {
   pacb = workAcbs + acb - 1;
/* ************************************************************************ 
   This is commented by kc on 26/4/93 . 
   Reason: When AA_ASCind comes & when this fn. is called acb exists &
           ExActive bit is not set. So this fails.


	if (! (pacb->stateFlg & RC0257_ExActive))	rc = -R_WASSOC;
   else if (pacb->stateFlg & RC0259_EndPend)		rc = -R_AUPEN;
   else if ((pmNo < 0) || (pmNo >= RC0200_MaxAutomata)) rc = -R_NPMFCT;
   else 						rc = 0;  
   
   ********* End of comment by kc on 26/4/93  *************************** */
        if (pacb->stateFlg & RC0259_EndPend)		rc = -R_AUPEN;
   else if ((pmNo < 0) || (pmNo >= RC0200_MaxAutomata)) rc = -R_NPMFCT;
   else 						rc = 0;  
}

if (! rc) rc = ro0292_cfIfFct (pacb->configNo	,
			       pmNo		,
			       pacb->pmcxt[pmNo],
			       inNo		,
			       info		);
RM0003_RETURN	 (rs0231, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0232_getAddress2                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-06-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0232_getAddress2(
   unsigned long  int       ecb,
   signed   short int       lay,
   unsigned short int       len,
   char               FAR   *adr)

/*                                                                    */
/* DESCRIPTION  : gets the Address as per the <ecb> supplied.         */
/*                Introduced to get the handle if no Association      */
/*                exists . This is used by Loif (CMX).                */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short  int	rc;
rT0252_ecb   NEAR    *pecb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0232)

       pecb = workEcbs + ecb - 1;
       rc   = ro0306_GetAddress (pecb->myAddres, lay, len, adr);

RM0003_RETURN (rs0232, signed short int, rc);
}/*end*/

/*exon ****************************************************************/  
/*                                                                    */  
/* TYPE         : C-FUNCTION                                          */  
/*                                                                    */  
/* NAME         : ro0233_getTpmFlowCtrl                               */  
/*                                                                    */  
/* AUTHOR       : Chandrasekar.K.                                     */  
/* DATE         : 93-06-15                                            */  
/*                                                                    */  
/* SYNTAX       :                                                     */  
                                                                          
   signed   short int PASCAL ro0233_getTpmFlowCtrl(unsigned long  int ecb)
/*                                                                    */  
/* DESCRIPTION  : Gets the TPM FLOW control flag from Ecb             */  
/*                                                                    */  
/* IN-PARAMETERS:             : none                                  */  
/*                                                                    */  
/* OUT-PARAMETERS             : none                                  */  
/*                                                                    */  
/* RETURN VALUES              :                                       */    
/*                                                                    */    
/* GLOBAL DATA   ( read only ):  none                                 */    
/*                                                                    */    
/* GLOBAL DATA   ( modified ) :  none                                 */    
/*                                                                    */    
/* REMARKS                    :  none                                 */    
/*                                                                    */    
/*inoff ***************************************************************/    
                                                                            
/* ****************** l o c a l    v a r i a b l e s **************** */    
/* ******************                                **************** */    
{/*entr*/                                                                   
                                                                            
signed short  int       rc;                                                 
rT0252_ecb   NEAR    *pecb;                                                 
                                                                            
/* ****************** S T A R T ************************************* */    
/* ******************           ************************************* */    
                                                                            
RM0002_ENTRY (rs0233)                                                       
                                                                             
 pecb = workEcbs + ecb - 1;                                                  
 if ((ecb < (unsigned long int)1 ) || (ecb > (unsigned long int)             
                                                   workScb->appliNo) ||      
      (!pecb->stateFlg)){                                                    
      rc = -R_WAPLID;                                                        
 }else rc = pecb->bTpmFlowCtrl;                                              
                                                                             
                                                                             
RM0003_RETURN (rs0233, signed short int, rc);                                
}/*end*/                                                                     

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0235_regPmRdbInv							*/
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 92-03-05					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0235_regPmRdbInv (
   unsigned long  int        pmcxt)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int    rc;
signed short int rdbNo;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0235)

rdbNo = (workScb->hRdbFile) ? ro0287_giveRdb	(workScb->hRdbFile) :	  0;
rc    = (rdbNo >=	 0) ? ro0212_regPmInvoc (pmcxt) 	    : rdbNo;

if (! rc) runAcb->pmRdbNo[evRunn.autoNo] = rdbNo;

RM0003_RETURN (rs0235, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0236_regPmRdbDes				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 92-03-05					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0236_regPmRdbDes (void)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int    rc;
signed short int rdbNo;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0236)

rdbNo = runAcb->pmRdbNo[evRunn.autoNo];
rc    = (rdbNo < 1) ? 0 : ro0288_freeRdb (workScb->hRdbFile, rdbNo);

if (! rc) {
       runAcb->pmRdbNo[evRunn.autoNo] = 0;
       ro0213_regPmDestr ();
}

RM0003_RETURN (rs0236, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0237_pmRdbWrite				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 92-03-05					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0237_pmRdbWrite (void)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int    rc;
signed short int rdbNo;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0237)

rdbNo = runAcb->pmRdbNo[evRunn.autoNo];
rc    = (rdbNo < 1) ? 0 : ro0286_write2Rdb (workScb->hRdbFile		,
					    rdbNo			,
					    RC0266_PmRecord		,
					    runAcb->toEcb		,
					    evRunn.toAcb		,
					    evRunn.autoNo		,
					    runAcb->pmcxt[evRunn.autoNo]);

RM0003_RETURN (rs0237, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0238_resetExActiveFlag			      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

void PASCAL	ro0238_resetExActiveFlag (void)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0238)

runAcb->stateFlg &= (~RC0257_ExActive);

RM0004_RETURN_VOID (rs0238)
}/*end*/


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro0240_encodSyntaxMatchRtpm                                 */
/*                                                                            */
/* AUTHOR       : Shivkumar, Haran                                            */
/* DATE         : 93-05-26                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro0240_encodSyntaxMatchRtpm (
	unsigned long   int FAR *phRtseUserData,
	unsigned long   int      hsStore)

/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Called by the RTPM to encode the ROSE APDU within an EXTERNAL in the   */
/*     case when it wants to send an OCTET STRING on a RT_TRreq.              */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     hRtseUserData        : The ROSE PDV node that is to be encoded         */
/*                            within an EXTERNAL into the store.              */
/*                                                                            */
/*     hsStore              : Handle to store into which the encoded string   */
/*                            is to be written.                               */
/*                                                                            */
/* OUT-PARAMETERS : None                                                      */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*       0          : OK.                                                     */
/*    != 0          : Error                                                   */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  : None                                             */
/*                                                                            */
/* REMARKS :  None                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro0240_encodSyntaxMatchRtpm*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro0240_encodSyntaxMatchRtpm")
signed   short int       sRetCode   = OK;
unsigned long  int       ulNumb = LNULL;
void               NEAR *pPpmContext;
rT0251_acb         NEAR *pRunAcb = (rT0251_acb NEAR *) 0;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (pFnctName)

pRunAcb    = workAcbs + evRunn.toAcb - 1;
pPpmContext= (void *)ro0031_locallock (pRunAcb->pmcxt[RC0215_Presentation]);
if   (pPpmContext == LNULL) sRetCode = -R_LLOCKE;
else  sRetCode = ro0293_cfPdvEnFct (pRunAcb->configNo                        ,
                                    RC0218_Rtse                              ,
                                    (void FAR *) LNULL                       ,
                                    (void FAR *) LNULL                       ,
                                    hsStore                                  ,
                                    pPpmContext                              ,
                                    phRtseUserData                           ,
                                    (unsigned long int FAR *)&ulNumb         ,
                                    0 /* sAnyNumb is 0 for new Abstr. Syn.*/ );

RM0003_RETURN (rs0247, signed short int, sRetCode)

}/*end-ro0240_encodSyntaxMatchRtpm*/


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro0241_decodSyntaxMatchRtpm                                */
/*                                                                            */
/* AUTHOR       : Shivkumar, Haran                                            */
/* DATE         : 93-05-26                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro0241_decodSyntaxMatchRtpm (
	unsigned long   int FAR *phRtseUserData,
	unsigned long   int      hsStore)

/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Decodes the string in the EXTERNAL from the store into the specified   */
/*     phRtseUserData PDV node of ROSE.                                       */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     hsStore              : Description of store that contains the encoded  */
/*                            string to be decoded.                           */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/*     hRtseUserData        : The ROSE PDV node that is decoded from the      */
/*                            provided STORE.                                 */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*       0          : OK.                                                     */
/*    != 0          : Error                                                   */
/*                    (caller returns immediately with the same value,        */
/*                     mapping of the value within the PPM encoding function).*/
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro0241_decodSyntaxMatchRtpm*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro0241_decodSyntaxMatchRtpm")
signed   short int       sRetCode   = OK;
unsigned long  int       ulNumb     = LNULL;
void               NEAR *pPpmContext;
rT0251_acb         NEAR *pRunAcb = (rT0251_acb NEAR *) 0;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (pFnctName)

pRunAcb  = workAcbs + evRunn.toAcb - 1;
pPpmContext= (void *)ro0031_locallock(pRunAcb->pmcxt [RC0215_Presentation]);
if   (pPpmContext == LNULL) sRetCode = -R_LLOCKE;
else sRetCode = ro0294_cfPdvDeFct (pRunAcb->configNo,
                                   RC0218_Rtse                                 ,
                                   0 /* sAbstrSynId     not used in this case*/,
                                   0 /* lEncodingChoice not used in this case*/,
                                   (void FAR *) LNULL                          ,
                                   (void FAR *) LNULL                          ,
                                   hsStore                                     ,
                                   pPpmContext                                 ,
                                   phRtseUserData                              ,
                                   ulNumb                                      ,
                                   0 /* sAnyNumb is 0 for new Abstr. Syn.*/   );

RM0003_RETURN (pFnctName, signed short int, sRetCode)

}/*end-ro0241_decodSyntaxMatchRtpm*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0242_encodSniP				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-03-30					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0242_encodSniP (
	signed   short int        sPmNo,
	unsigned long  int        hsStore,
	void               NEAR * pContext,
	unsigned long  int FAR  * phNode)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0242)
RM0003_RETURN (rs0242, signed short int, ro0298_cfSniPEnFct (runAcb->configNo,
			                                     sPmNo           ,	
                                 			     hsStore	     ,
							     pContext	     ,
							     phNode	     ))
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0243_decodSniP				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-03-30					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0243_decodSniP (
	signed short int          sPmNo,
	unsigned long  int        hsStore,
	void               NEAR * pContext,
	unsigned long  int FAR  * phNode,
	signed short int   FAR  * psAbsId)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0243)
RM0003_RETURN (rs0243, signed short int, ro0299_cfSniPDeFct (runAcb->configNo,
				                             sPmNo           , 
							     hsStore	     ,
							     pContext	     ,
							     phNode	     ,
                                                             psAbsId         ))
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0244_encodX410				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-02-24					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL  ro0244_encodX410 (
	signed   short int        sPmNo,
	unsigned long  int FAR  * phsStore,
	void               NEAR * pContext,
	unsigned long  int FAR  * phNode)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0244)
RM0003_RETURN (rs0244, signed short int, ro0296_cfX410EnFct (runAcb->configNo,
			                                     sPmNo           ,	
                                 			     phsStore	     ,
							     pContext	     ,
							     phNode	     ))
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0245_decodX410				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-02-24					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL  ro0245_decodX410 (
	signed short   int        sPmNo,
	unsigned       int        uInEvent,
	signed   long  int        lType,
	unsigned long  int        hsStore,
	void 		   NEAR * pContext,
	unsigned long  int FAR  * phNode)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0245)
RM0003_RETURN (rs0245, signed short int, ro0297_cfX410DeFct (runAcb->configNo,
				                             sPmNo           , 
				                             uInEvent        , 
                                	                     lType           ,	
							     hsStore	     ,
							     pContext	     ,
							     phNode	     ))
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0246_clearPdvTree				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-12-01					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0246_clearPdvTree (
   unsigned long  int FAR   *pdvNode)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int rc;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0246)

if (*pdvNode) rc = ro0295_cfPdvClFct (runAcb->configNo, pdvNode);
else	      rc = 0;

RM0003_RETURN (rs0246, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0247_encodingPm				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-12-01					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0247_encodingPm (
	signed   short int	     pmNo,
	rTro76_stDefinedBy FAR  * defBy,
	rTro01_pfvObjectId	     tSyNa,
	unsigned long  int	     store,
	void	              NEAR * pcxt,
	unsigned long  int FAR  * pdvN,
	unsigned long  int FAR  * len,
	signed         int        iAnyNumb)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0247)
RM0003_RETURN (rs0247, signed short int, ro0293_cfPdvEnFct (runAcb->configNo,
							    pmNo	    ,
							    defBy	    ,
							    tSyNa	    ,
							    store	    ,
							    pcxt	    ,
							    pdvN	    ,
							    len 	    ,
                                                            iAnyNumb        ))
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0248_decodingPm				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-12-01					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0248_decodingPm (
	signed   short int	     pmNo,
	signed   short int	     absId,
	signed   long  int	     chTag,
	rTro76_stDefinedBy FAR  * defBy,
	rTro01_pfvObjectId	     tSyNa,
	unsigned long  int	     store,
	void               NEAR * pcxt,
	unsigned long  int FAR  * pdvN,
	unsigned long  int	     len,
	signed         int        iAnyNumb)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0248)
RM0003_RETURN (rs0248, signed short int, ro0294_cfPdvDeFct (runAcb->configNo,
							    pmNo	    ,
							    absId	    ,
							    chTag	    ,
							    defBy	    ,
							    tSyNa	    ,
							    store	    ,
							    pcxt	    ,
							    pdvN	    ,
							    len 	    ,
                                                            iAnyNumb         ))
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0250_attachSys				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0250_attachSys (
	unsigned long  int FAR   *phScb,
	signed   short int	      mode,
	signed   long  int	  systemId,
	FARPROC		   tpduFct,
	unsigned long  int	   lSyName,
	char 	      FAR *pSyName)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int	 rc;
rT0254_scb	 NEAR *pScb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0250)

rc = ro0200_crtCb (&scbQuHd		   ,
		   &scbQuNo		   ,
		   (rT0255_cb **) &scbQuPtr,
		   sizeof (rT0254_scb)	   );
if (rc > 0) {
       *phScb	       =  rc;
       pScb	       =  scbQuPtr + rc - 1;
       pScb->stateFlg  =  1;
       pScb->systemId  =  systemId;
       pScb->tpduFct   =  tpduFct;
       pScb->uEvnHead  =  0;
       pScb->uEvnTail  = -1;
       rc	       = ro0280_openRdb (mode		  ,
					 0		  ,
					 &(pScb->hRdbFile),
					 lSyName	  ,
					 pSyName	  );
}

if (rc) dpatPanic = 1;

RM0003_RETURN (rs0250, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0251_detachSys				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL  ro0251_detachSys (
	unsigned long  int FAR    *phScb)


/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int	 rc;
rT0254_scb	 NEAR *pScb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0251)

rc   = 0;
pScb = scbQuPtr + (*phScb) - 1;

if (((signed short int)(*phScb) < 1      ) ||    /* skh: Dos warning */
    ((signed short int)(*phScb) > scbQuNo) ||    /* skh: Dos warning */
     (! (pScb->stateFlg)))
{
       rc = -R_NOSCBB;
} else {
       if (pScb->activScb < 1)	  {
	      if (pScb->hRdbFile) {
		  rc = ro0281_closeRdb (pScb->hRdbFile);
	      }

	      if (! rc) {
		      *phScb	     = 0;
		      pScb->stateFlg = 0;
		      pScb->systemId = 0;
		      pScb->tpduFct  = 0;
		      pScb->hRdbFile = 0;
		      pScb->activScb = 0;
	       } else {
		      dpatPanic      = 1;
	       }
       }
}

RM0003_RETURN (rs0251, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0252_cleardpat				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0252_cleardpat (void)

/*                                                                    */
/* DESCRIPTION	: This function only close all communication lines    */
/*		  by calling lower interface's detach function.       */
/* It assumes that files ared closed, global and local heap memory is */
/* freed automatically when it returns. The function need access to   */
/* local heap memory when it is performed.			      */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES	      : == 0 : OK			      */
/*				!= 0 : error			      */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS		      :  This function should only be used    */
/*				 by window's WEP routine for DLL's    */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short  int     rc;
signed short  int      r;

unsigned long int    scb;
unsigned long int    ecb;

rT0254_scb   NEAR  *pScb;
rT0252_ecb   NEAR  *pecb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0252)

r = rc = 0;

for (scb = 1; (signed short int)scb <= scbQuNo; scb++)  { /* skh: Dos warning */
     pScb = scbQuPtr + scb - 1;
     ecb  = (pScb->stateFlg) ? 1L : (unsigned long int)(pScb->appliNo + 1);
                                                          /* skh: Dos warning */
     while ((signed short int)ecb <= pScb->appliNo) {     /* skh: Dos warning */
	 pecb = (rT0252_ecb NEAR *) ro0031_locallock (pScb->appliHd);

	 if (pecb) r = ro0301_detach (0, pecb->eiCxt);
	 else	   r = -R_LLOCKE;

	 if (ro0032_localunlock (pScb->appliHd)) r = -R_LULOKE;

	 if (r) rc = r;
	 ecb++;
     }
}

RM0003_RETURN (rs0252, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0260_attach                                       */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0260_attach (
	unsigned long  int	       hScb,
	unsigned long  int FAR      *ecb,
	signed   short int	   configNo,
	signed   short int	    recInPm,
	signed   long  int	   recInEvt,
	unsigned long  int FAR  *hMyAddr,
	unsigned long  int	    lMyAddr,
	unsigned long  int FAR      *hi1,
	unsigned long  int		li1,
	unsigned long  int FAR      *hi2,
	unsigned long  int		li2,
	unsigned long  int FAR      *hi3,
	unsigned long  int		li3)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int	rc;
signed short int	rr;
rT0252_ecb   NEAR    *pecb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0260)

rc = ro0206_lockSysVa (hScb);

if (! rc) rc = ro0200_crtCb ((unsigned long int *)&workScb->appliHd	     ,
			                          &workScb->appliNo	     ,
			                          (rT0255_cb **) &workEcbs   ,
			                          sizeof (rT0252_ecb)        );
if (rc >= 0)  {
       *ecb = rc;
       pecb = workEcbs + rc - 1;

       pecb->ecbRdbNo = (workScb->hRdbFile)
		      ? ro0287_giveRdb (workScb->hRdbFile)
		      : 0;

       if (pecb->ecbRdbNo >= 0) {
/* **skh: To handle File Problem !!!
	      rc = ro0300_attach (*ecb, pecb->eiCxt, workScbId, *hMyAddr);
*/
/* kc K02	 rc= ro0300_attach(*ecb, pecb->eiCxt, workScb->systemId, *hMyAddr); */
             rc= ro0300_attach(*ecb, pecb->eiCxt, workScb->systemId, *hMyAddr,
        (signed short int FAR *)&pecb->bTpmFlowCtrl);            /* K02 */ 
       } else {
	      rc = pecb->ecbRdbNo;
       }

       if (rc >= 0) {
	      (workScb->activScb)++;
	      pecb->stateFlg =	      1;
	      pecb->configNo = configNo;
	      pecb->recInPm  =	recInPm;
	      pecb->recInEvt = recInEvt;
	      pecb->myAddres = *hMyAddr;
	      pecb->eiCxt[1] =	   *hi1;
	      pecb->eiCxt[2] =	   *hi2;
	      pecb->eiCxt[3] =	   *hi3;

	      if (pecb->ecbRdbNo > 0)  {
		  rr = ro0285_write1Rdb (workScb->hRdbFile  ,
					 pecb->ecbRdbNo     ,
					 RC0265_EcbRecord   ,
					 *ecb		    ,
					 0		    ,
					 0		    ,
					 (char FAR *) pecb  ,
					 sizeof (rT0252_ecb),
					 *hMyAddr	    ,
					 lMyAddr	    ,
					 *hi1		    ,
					 li1		    ,
					 *hi2		    ,
					 li2		    ,
					 *hi3		    ,
					 li3		    );

		  if (rr) { dpatPanic = 1; rc = rr; }
	      }
       }
}

if (rc >= 0)
       *hMyAddr = *hi1 = *hi2 = *hi3 =  0;
else   *ecb     = 0;

ro0207_unlockSys ();
RM0003_RETURN (rs0260, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0261_detach                                       */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0261_detach (
   unsigned long  int	    hScb,
   unsigned long  int        ecb,
   unsigned long  int FAR   *myAddr,
   unsigned long  int FAR   *i1,
   unsigned long  int FAR   *i2,
   unsigned long  int FAR   *i3)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int     rc;
signed short int      i;
rT0252_ecb   NEAR *pecb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0261)

rc = ro0206_lockSysVa (hScb);

if (! rc) {
      pecb = workEcbs + ecb - 1;
      if (((signed short int)ecb < 1)                ||   /* skh: Dos warning */
          ((signed short int)ecb > workScb->appliNo) ||   /* skh: Dos warning */
          (! pecb->stateFlg))
      {
	   rc = -R_WAPLID;
      }
}

/* ****************** Clean up and call lower interface's *********** */
/* ****************** detach				  *********** */

if (! rc) {
      rc = ro1103_clearTimerQueue    (ecb);
      if (! rc) rc = ro0203_clrQueue (ecb, 0);
      if (! rc) rc = ro0205_clrAcb   (ecb);
      if (! rc) rc = ro0204_clrPcb   (ecb);
      if (! rc) rc = ro0301_detach   (ecb, pecb->eiCxt);

      if ((! rc) && (pecb->ecbRdbNo > 0)) {
	 rc = ro0288_freeRdb (workScb->hRdbFile, pecb->ecbRdbNo);
      }
      if (rc) dpatPanic = 1;
}

/* ****************** Return attach parameters ********************** */
/* ******************			       ********************** */

if (! rc) {
       *myAddr	      = pecb->myAddres;
       *i1	      = pecb->eiCxt[1];
       *i2	      = pecb->eiCxt[2];
       *i3	      = pecb->eiCxt[3];
       pecb->stateFlg = 0;

       for (i = 0; i < RC0254_MaxLayerCxt; i++) {
	   pecb->eiCxt[i] = 0;
       }
       (workScb->activScb)--;
} else {
       *myAddr = *i1 = *i2 = *i3 = 0;
}

ro0207_unlockSys ();
RM0003_RETURN	 (rs0261, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0262_provideBuffer                                */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0262_provideBuffer (
   unsigned long  int	    hScb,
   unsigned long  int        ecb,
   unsigned long  int FAR   *buf)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int	rc;
rT0252_ecb   NEAR    *pecb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0262)

rc = ro0206_lockSysVa (hScb);

if (rc) {
   RM0003_RETURN (rs0262, signed short int, rc)
}

pecb = workEcbs + ecb - 1;

if (((signed short int)ecb < 1)                ||    /* skh: Dos warning */
    ((signed short int)ecb > workScb->appliNo) ||    /* skh: Dos warning */
    (! (pecb->stateFlg)))
{
       rc = -R_WAPLID;
} else {
       rc = ro0200_crtCb (&workScb->prBufHd	  ,
                          &workScb->prBufNo       ,
                          (rT0255_cb **) &workPcbs,
                          sizeof (rT0253_pcb)     );
       if (rc >= 0) {
              rc--;
	      workPcbs[rc].stateFlg = *buf;
	      *buf		    = 0;
              workPcbs[rc].toEcb    = ecb;
	      rc		    = 0;
       }
}

ro0207_unlockSys ();
RM0003_RETURN (rs0262, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0263_wake                                         */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0263_wake (
   unsigned long  int		 hScb,
   unsigned long  int        systemId)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY  (rs0263)
RM0003_RETURN (rs0263, signed short int, ro0305_wake (systemId))
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0264_getPmInfo				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int  PASCAL ro0264_getPmInfo (
   unsigned long  int  hScb,
   unsigned long  int	acb,
   signed   short int  pmNo,
   signed   short int  inNo,
   void     FAR       *info)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int     rc;
rT0251_acb  NEAR  *pacb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0264)

rc = ro0206_lockSysVa (hScb);

if (rc) {
   RM0003_RETURN (rs0264, signed short int, rc)
}

if (((signed short int)acb < 1) || ((signed short int)acb > workScb->assocNo)) {
                                                      /* skh: Dos warning */
   rc = -R_WASSOC;
}
else {
   pacb = workAcbs + acb - 1;

	if (! (pacb->stateFlg & RC0257_ExActive))	rc = -R_WASSOC;
   else if (pacb->stateFlg & RC0259_EndPend)		rc = -R_AUPEN;
   else if ((pmNo < 0) || (pmNo >= RC0200_MaxAutomata)) rc = -R_NPMFCT;
   else 						rc = 0;
}

if (! rc) rc = ro0292_cfIfFct (pacb->configNo	,
			       pmNo		,
			       pacb->pmcxt[pmNo],
			       inNo		,
			       info		);
ro0207_unlockSys ();
RM0003_RETURN	 (rs0264, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0265_getEcbInfo				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0265_getEcbInfo (
   unsigned long  int	    hScb,
   unsigned long  int        ecb,
   signed   short int FAR   *cnfNo,
   signed   short int FAR   *recPm,
   signed   long  int FAR   *recEvt,
   unsigned long  int FAR   *myAddr,
   unsigned long  int FAR   *i1,
   unsigned long  int FAR   *i2,
   unsigned long  int FAR   *i3)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int	rc;
rT0252_ecb   NEAR    *pecb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0265)

rc = ro0206_lockSysVa (hScb);

if (! rc) {
      pecb = workEcbs + ecb - 1;

      if (((signed short int)ecb < 1               ) ||  /* skh: Dos warning */
          ((signed short int)ecb > workScb->appliNo) ||  /* skh: Dos warning */
          (! pecb->stateFlg))
      {
	  rc	  = -R_WAPLID;
      }
}

if (! rc) {
       *cnfNo  = pecb->configNo;
       *recPm  = pecb->recInPm;
       *recEvt = pecb->recInEvt;
       *myAddr = pecb->myAddres;
       *i1     = pecb->eiCxt[1];
       *i2     = pecb->eiCxt[2];
       *i3     = pecb->eiCxt[3];
} else {
       *myAddr = *i1 = *i2 = *i3 = 0;
}

ro0207_unlockSys ();
RM0003_RETURN	 (rs0265, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0266_getSysInfo				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0266_getSysInfo (
   unsigned long  int	      hScb,
   signed   short int	      mode,
   unsigned	  int FAR   *nEcbs,
   unsigned	  int FAR   *nAcbs,
   unsigned	  int FAR   *nPcbs,
   unsigned	  int	     lEcbs,
   unsigned	  int FAR   *pEcbs,
   unsigned	  int	     lAcbs,
   unsigned	  int FAR   *pAcbs,
   unsigned	  int FAR   *pEcor)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int     rc;
signed short int      i;

rT0251_acb   NEAR *pacb;
rT0252_ecb   NEAR *pecb;
rT0253_pcb   NEAR *ppcb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0266)

*nEcbs = 0;
*nAcbs = 0;
*nPcbs = 0;
rc     = ro0206_lockSysVa (hScb);

if (rc) {
   RM0003_RETURN (rs0266, signed short int, rc)
}

/* ****************** Compute number of application invocations ***** */
/* ****************** and number of associations		***** */

pecb = workEcbs;

for (i = 0; i < workScb->appliNo; i++) {
    if (pecb->stateFlg) { (*nEcbs)++; } pecb++;
}

pacb = workAcbs;

for (i = 0; i < workScb->assocNo; i++) {
    if (pacb->stateFlg & RC0257_ExActive) { (*nAcbs)++; } pacb++;
}

ppcb = workPcbs;

for (i = 0; i < workScb->prBufNo; i++) {
    if (ppcb->stateFlg) { (*nPcbs)++; } ppcb++;
}

/* ****************** Return if handles need not to be ************** */
/* ****************** computed. 		       ************** */

if (! mode) {
      ro0207_unlockSys ();
      RM0003_RETURN    (rs0266, signed short int, rc)
}

/* ****************** Compute ecb handle values ********************* */
/* ******************				********************* */

if (*nEcbs <= lEcbs) {
       pecb = workEcbs;

       for (i = 1; i <= workScb->appliNo; i++)		     {
	   if (pecb->stateFlg) { *pEcbs = i; pEcbs++; } pecb++;
       }
} else {
       rc = -R_IFARYS;
}

/* ****************** Compute acb handle values ********************* */
/* ******************				********************* */

if (*nAcbs <= lAcbs) {
       pacb = workAcbs;
       for (i = 1; i <= workScb->assocNo; i++)	 {
	   if (pacb->stateFlg & RC0257_ExActive) {
	       *pAcbs = (unsigned int)i;
	       *pEcor = (unsigned int)pacb->toEcb;    /* skh: Dos warning */
	       pAcbs++;
	       pEcor++;
	   }
	   pacb++;
       }
} else {
       rc = -R_IFARYS;
}

ro0207_unlockSys ();
RM0003_RETURN (rs0266, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0267_recover				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 92-02-27					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0267_recover (
   unsigned long  int hScb)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed	 short int     rc;
signed	 short int     rr;
unsigned long  int    ecb;
unsigned long  int    acb;
rT0252_ecb    NEAR  *pecb;
rT0251_acb    NEAR  *pacb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0267)

rc = ro0206_lockSysVa (hScb);
if ((rc) || (workScb->hRdbFile == 0)) {
    ro0207_unlockSys ();
    RM0003_RETURN    (rs0267, signed short int, rc)
}

/* ****************** Perform internal attaches. ******************** */
/* ******************				 ******************** */

rr = 0;

for (ecb = 1; (signed short int)ecb <= workScb->appliNo; ecb++) {
                                                      /* skh: Dos warning */
	 pecb = workEcbs + ecb - 1;
	 if (pecb->stateFlg)/*  rc = ro0300_attach (ecb	       ,
						 pecb->eiCxt   ,
						 workScbId     ,
						 pecb->myAddres); K02 */
                                rc = ro0300_attach (ecb           ,          
                    pecb->eiCxt   ,          
                    workScbId     ,          
                    pecb->myAddres,          
                    (signed short int FAR *) 
                     &pecb->bTpmFlowCtrl);    /* K02 */
	 if (rc < 0) break;
	 if (rc > 0) rr = 1;
}

/* ****************** Send recovery event to PM's ******************* */
/* ******************				  ******************* */

if (rc >= 0) {
    acb = 1;
    rc	= 0;
}

while (((signed short int)acb <= workScb->assocNo) && (! rc)) {
                                                     /* skh: Dos warning */
   pacb = workAcbs + acb -1;
   if (pacb->stateFlg) {
       pecb = workEcbs + (pacb->toEcb) - 1;
       rc   = ro0219_pushOnAssoc (acb,pecb->recInPm,pecb->recInEvt,0,0,0,0);
       if (! rc) rc = ro0209_dispatch ();
    }
   acb++;
}

if (! rc) rc	    = rr;
else	  dpatPanic =  1;

ro0207_unlockSys ();
RM0003_RETURN	 (rs0267, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0268_setRecover				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 92-02-27					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int PASCAL ro0268_setRecover (
   unsigned long  int	     hScb)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed	 short int	rc;
signed	 short int	 i;

signed	 short int  sBlkNo;
unsigned short int   sType;
unsigned long  int    ulN1;
unsigned long  int    ulN2;
unsigned long  int    ulN3;
unsigned long  int datalen;

unsigned long  int hMyAddr;
unsigned long  int     hi1;
unsigned long  int     hi2;
unsigned long  int     hi3;

unsigned long  int   pmcxt;

rT0252_ecb   NEAR    *pecb;
rT0251_acb   NEAR    *pacb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0268)

rc = ro0206_lockSysVa (hScb);
if ((rc) || (workScb->hRdbFile == 0)) {
    ro0207_unlockSys ();
    RM0003_RETURN    (rs0268, signed short int, rc)
}

/* ****************** Read Recovery data bassis until *************** */
/* ****************** end of file.		      *************** */

rc = ro0282_readinfoRdb (workScb->hRdbFile, &sBlkNo ,
			 &sType 	  , &ulN1   ,
			 &ulN2		  , &ulN3   ,
			 &datalen		    );
while (! rc) {
      switch (sType) {
	 /* ********* Restore an ecb ******************************** */
	 /* *********		     ******************************** */

	 case RC0265_EcbRecord:
	      if (datalen == sizeof (rT0252_ecb)) {
		     rc = ro0201_ctRdb ((unsigned long int *)&workScb->appliHd,
					                     &workScb->appliNo,
                     				(rT0255_cb **) &workEcbs,
                     					ulN1,
		                     			sizeof (rT0252_ecb));
	      } else {
		     rc = -R_RDBREE;
	      }

	      if (! rc) {
		    pecb = workEcbs + ulN1 - 1;
		    if (pecb->stateFlg) rc = -R_RDBREE;
	      }

	      if (! rc) {
		     rc = ro0283_read1seqRdb (workScb->hRdbFile,
					      (char FAR *) pecb,
					      &hMyAddr	       ,
					      &hi1	       ,
					      &hi2	       ,
					      &hi3	       );
		     if (! rc) {
			   (workScb->activScb)++;
			   pecb->ecbRdbNo =  sBlkNo;
			   pecb->myAddres = hMyAddr;
			   pecb->eiCxt[0] =	  0;
			   pecb->eiCxt[1] =	hi1;
			   pecb->eiCxt[2] =	hi2;
			   pecb->eiCxt[3] =	hi3;
		     }
	      }
	      break;

	 /* ********* Restore an acb and pm context ***************** */
	 /* *********				    ***************** */

	 case RC0266_PmRecord :
	      rc = ro0201_ctRdb ((unsigned long int *)&workScb->assocHd	 ,
				 &workScb->assocNo	 ,
				 (rT0255_cb **) &workAcbs,
				 ulN2			 ,
				 sizeof (rT0251_acb)	 );
	      if (! rc) {
		    pacb = workAcbs + ulN2 - 1;
		    if (pacb->pmcxt[ulN3]) rc = -R_RDBREE;

	      }

	      if (! rc) {
		    rc = ro0284_read2seqRdb (workScb->hRdbFile, &pmcxt);

		    if (! rc) {
			  pacb->stateFlg     =(RC0256_InActive|RC0257_ExActive);
			  pacb->cxtCount    +=	    1;
			  pacb->toEcb	     =	 ulN1;
			  pacb->pmcxt  [ulN3]=	pmcxt;
			  pacb->pmRdbNo[ulN3]= sBlkNo;
		    }
	      }
	      break;

	 /* ********* Unkown record type in recovery **************** */
	 /* ********* data bassis		     **************** */

	 default:
	      rc = -R_RDBUNT;
	      break;
      }

      if (! rc) rc = ro0282_readinfoRdb (workScb->hRdbFile, &sBlkNo ,
					 &sType 	  , &ulN1   ,
					 &ulN2		  , &ulN3   ,
					 &datalen		    );
} /* end of while */

/* ****************** Check ecb references and compute ************** */
/* ****************** config. number in acb's          ************** */

if (rc > 0) {
	rc   = 0;
	i    = 0;
	pacb = workAcbs;
}  else {
	i = workScb->assocNo;
}

while (i < workScb->assocNo) {
    if	(pacb->stateFlg)     {
	 pecb = workEcbs + (pacb->toEcb) - 1;

	 if (((signed short int)pacb->toEcb <= workScb->appliNo) &&
             (pecb->stateFlg))                  /* skh: Dos warning */
         {
		pacb->configNo = pecb->configNo;
	 } else {
		rc = -R_RDBREE; break;
	 }
    }
    i++; pacb++;
}

if (rc) dpatPanic = 1;

ro0207_unlockSys ();
RM0003_RETURN	 (rs0268, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0269_getEcbFromAcb				      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

   signed   short int  PASCAL ro0269_AcbGetEcb (

   unsigned long  int	      hScb,
   unsigned long  int	       acb,
   unsigned long  int  FAR    *ecb)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int     rc;
rT0251_acb  NEAR  *pacb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0269)

rc = ro0206_lockSysVa (hScb);

if (! rc) {
      pacb = workAcbs + acb - 1;

                                                 /* skh: Dos warning */
      if (((signed short int)acb <			      1) ||
	  ((signed short int)acb >	       workScb->assocNo) ||
	  (! (pacb->stateFlg & RC0257_ExActive))) {

	     rc = -R_WASSOC;
      } else {
	     *ecb = pacb->toEcb;
      }
      ro0207_unlockSys ();
}

RM0003_RETURN (rs0269, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0270_send                                         */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int  PASCAL ro0270_send (
	unsigned long  int	      hScb,
	signed   short int         mode,
	unsigned long  int	       ecb,
	unsigned long  int  FAR    *acb,
	signed   short int	    autoNo,
	signed   long  int	     event,
	unsigned long  int  FAR     *p1,
	unsigned long  int  FAR     *p2,
	unsigned long  int  FAR     *p3,
	unsigned long  int  FAR     *p4)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : mode == 0 : new association           */
/*                                      1 : normal                    */
/*				       -1 : normal released	      */
/*                                     -2 : aborted                   */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed   short int        rc;
unsigned long  int   specFlg;    /* skh: Dos warning */
unsigned long  int   pendFlg;    /* skh: Dos warning */

rT0251_acb   NEAR      *pacb;
rT0252_ecb   NEAR      *pecb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0270)

rc = ro0206_lockSysVa (hScb);

if (rc) {
	if (! mode) *acb = 0;
	RM0003_RETURN (rs0270, signed short int, rc);
}

if (! mode) {
       pecb = workEcbs + ecb - 1;

       if (((signed short int)ecb > 0)                 && /* skh: Dos warning */
           ((signed short int)ecb <= workScb->appliNo) && /* skh: Dos warning */
           (pecb->stateFlg))
       {
	      rc = ro0220_pushNewAssoc (acb,  ecb, autoNo, event,
					*p1,  *p2, *p3	 , *p4	);
       } else {
	      rc   = -R_WAPLID;
	      *acb = 0;
       }

       if (! rc) {
	      workAcbs[(*acb) - 1].stateFlg |= RC0257_ExActive;
	      rc  = ro0209_dispatch  ();
	      *p1 = *p2 = *p3 = *p4 = 0L;
       }

ro0207_unlockSys ();
RM0003_RETURN	 (rs0270, signed short int, rc);
}


if(((signed short int)*acb < 1) || ((signed short int)*acb > workScb->assocNo)){
                                                     /* skh: Dos warning */
       rc = -R_WASSOC;
} else {
       pacb = workAcbs + (*acb) - 1;

       if (pacb->stateFlg & RC0257_ExActive) {

	      specFlg = pacb->stateFlg & RC0258_SpecFlag;
	      pendFlg = pacb->stateFlg & RC0259_EndPend;

		   if (specFlg && (mode > -2)) rc = -R_SPLFLG;
	      else if (pendFlg && (mode > -2)) rc = -R_AUPEN;
	      else			       rc = 0;
       } else {
	      rc = -R_WASSOC;
       }
}

if ((! rc) && (! pendFlg)) {
       rc = ro0219_pushOnAssoc (*acb, autoNo, event, *p1, *p2, *p3, *p4);
       if (! rc) rc = ro0209_dispatch ();
       *p1 = *p2 = *p3 = *p4 = 0;
}

if ((! rc) && (mode < 0)) {
    pacb->stateFlg &= RC0256_InActive;
    rc		    = ro0203_clrQueue (*acb, 1);
}

ro0207_unlockSys ();
RM0003_RETURN	 (rs0270, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0271_receive                                      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed   short int  PASCAL ro0271_receive (
   unsigned long  int		hScb,
   signed   short int	   blockTime,
   unsigned long  int  FAR	*ecb,
   unsigned long  int  FAR	*acb,
   signed   long  int  FAR    *event,
   unsigned long  int  FAR	 *p1,
   unsigned long  int  FAR	 *p2,
   unsigned long  int  FAR	 *p3,
   unsigned long  int  FAR	 *p4)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             :                                       */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed	 short int     rc;
signed	 short int fstate;
signed	 short int   pmNo;
signed	 long  int loifId;

rT0250_mcb   NEAR   *pmcb;
rT0251_acb   NEAR   *pacb;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0271)

rc     = ro0206_lockSysVa (hScb);
fstate = (rc) ? 0 : 1;

while (fstate) {
      switch (fstate) {
      case 1:
      case 3: if (workScb->uEvnTail != -1)  {

		     pmcb   = workEque + workScb->uEvnHead;
		     *event = pmcb->event;

		     if (*event) {
			   *p1		= pmcb->params[0];
			   *p2		= pmcb->params[1];
			   *p3		= pmcb->params[2];
			   *p4		= pmcb->params[3];

			   pmcb->event	=
			   fstate	=
			   rc		= 0;
		     }

		     if (*event > 0) {
			    *ecb = (workAcbs + (pmcb->toAcb) - 1)->toEcb;
			    *acb = pmcb->toAcb;
			    pacb = workAcbs + (*acb) - 1;

			    (pacb->evtCount)--;

			    if ((pacb->evtCount <	       1) &&
				(pacb->stateFlg & RC0259_EndPend)) {

				   pacb->stateFlg &= RC0256_InActive;
			    } else {
				   pacb->stateFlg |= RC0257_ExActive;
			    }
		     } else {
			    *ecb = *acb = 0;
		     }

		     (workScb->activScb)--;

		     if (workScb->uEvnHead == workScb->uEvnTail) {
			    workScb->uEvnHead =  0;
			    workScb->uEvnTail = -1;
		     } else {
			    (workScb->uEvnHead)++;
			    if (workScb->uEvnHead >=
				workScb->uEvntQNo  ) workScb->uEvnHead = 0;
		     }
	      } else {
		     if (workScb->activScb)			   {
			    if (fstate == 1) { fstate = 2;	   }
			    else	     { fstate = 0; rc = 2; }
		     } else {
			    fstate = 0;
			    rc	   = 2;
		     }
	      }
	      break;

      case 2: {
              signed long int  systemId = workScb->systemId;

              rc = ro1102_checkTimer (acb, event);

	      if (rc > 0) {
		     ro0207_unlockSys ();
/* **skh: For handling file problem :
		     rc = ro0311_CheckOrWait (ecb      ,
					      &loifId  ,
					      &pmNo    ,
					      event    ,
					      p1       ,
					      p2       ,
					      p3       ,
					      p4       ,
					      blockTime,
					      hScb     );
*/
		     rc = ro0311_CheckOrWait (ecb      ,
					      &loifId  ,
					      &pmNo    ,
					      event    ,
					      p1       ,
					      p2       ,
					      p3       ,
					      p4       ,
					      blockTime,
					      systemId );


		     if (ro0206_lockSysVa (hScb)) {
			    dpatPanic = 1;
			    rc	      = -R_DPPANC;
		     }

		     if (! rc) {
			    *acb = (*ecb) ? 0 : ro0230_seaLoifId (loifId, pmNo);

			    if ((! *ecb) && (! *acb)) {
				  dpatPanic = 1;
				  rc	    = -R_ACBNOF;
			    }
		     }
	      } else {
		     pmNo = RC0201_Timer;
		     *p1  = *p2 = *p3 = *p4 = 0;
	      }

	      if (! rc) {
		 rc = (*acb) ? ro0219_pushOnAssoc  (*acb, pmNo, *event,
						    *p1 , *p2 , *p3,  *p4)

			     : ro0220_pushNewAssoc (acb, *ecb, pmNo, *event,
						    *p1, *p2 , *p3 , *p4   );
	      }

	      if (! rc) rc = ro0209_dispatch ();

	      fstate = (rc) ? 0 : 3;
	      break;
              }
      }
}

if (rc) {
   *event = 0;
   *acb   = *ecb = *p1 = *p2 = *p2 = *p4 = 0;
}

ro0207_unlockSys ();
RM0003_RETURN	 (rs0271, signed short int, rc);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0273_getPmInfo2				      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE         : 93-03-26                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

   signed   short int  PASCAL ro0273_getPmInfo2 (
   unsigned long  int  hScb,
   unsigned long  int	configNo,
   signed   short int  pmNo,
   signed   short int  inNo,
   void     FAR       *info)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

signed short int     rc;
unsigned long int    pmcxt=0;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0273)

rc = ro0206_lockSysVa (hScb);

if (rc) 
   RM0003_RETURN (rs0273, signed short int, rc)
if(workAcbs)
  pmcxt = workAcbs->pmcxt[RC0215_Presentation];

      rc = ro0292_cfIfFct ((signed short int)configNo,  /* skh: Dos warning */
			   pmNo		             ,
			   pmcxt                     ,
			   inNo		             ,
			   info		             );

ro0207_unlockSys ();

RM0003_RETURN	 (rs0273, signed short int, rc)
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0274_getPrBfCount                    	      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-05-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int FAR PASCAL  ro0274_getPrBfCount ( 
unsigned  long  int         hScb  ,
unsigned  long  int         hEcb  ,
signed          int  FAR  * piBfNumb)

/*                                                                    */
/* DESCRIPTION  : Counts the Number of buffers in the Entity.         */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              : Scb Number & Ecb Number               */
/*                                                                    */
/* OUT-PARAMETERS             : Number of Buffers                     */
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
signed   short int      rc = OK;
rT0253_pcb NEAR *       pPcb = NULL;
signed   short int      i    = NULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0274)

rc = ro0206_lockSysVa (hScb);
if (rc == OK ){
    *piBfNumb = NULL; 
    while ( i < workScb->prBufNo ) {
       pPcb = workPcbs + i++;
       if ((pPcb->toEcb == hEcb) && (pPcb->stateFlg) ) 
             (*piBfNumb)++;
    }

ro0207_unlockSys ();
}
RM0003_RETURN (rs0274, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0275_ClearAll                        	      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 16-11-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

void  PASCAL  ro0275_ClearAll     ( void)

/*                                                                    */
/* DESCRIPTION  : Clears all the Ecbs,Acbs,timer queues,Prbufs        */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              : None                                  */
/*                                                                    */
/* OUT-PARAMETERS             : None                                  */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
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
signed short  int     rc;  
signed short  int      r;  
                           
unsigned long int    scb;  
unsigned long int    ecb;  
                           
rT0254_scb   NEAR  *pScb;  
rT0252_ecb   NEAR  *pecb;  
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0275)
r = rc = 0;                                                                     
for (scb = 1; (signed short int)scb <= scbQuNo; scb++)  { 
     rc = ro0206_lockSysVa (scb); 
     if(rc != OK) break;
     pScb = scbQuPtr + scb - 1;                                                 
     ecb  = (pScb->stateFlg) ? 1L : (unsigned long int)(pScb->appliNo + 1);     
                                                         
     while ((signed short int)ecb <= pScb->appliNo) {     
         pecb = (rT0252_ecb NEAR *) ro0031_locallock (pScb->appliHd);           
         if (pecb) {
             rc = ro1103_clearTimerQueue    (ecb);                      
             if (! rc) rc = ro0203_clrQueue (ecb, 0);                   
             if (! rc) rc = ro0205_clrAcb   (ecb);                      
             if (! rc) rc = ro0204_clrPcb   (ecb);                      
/*           if (! rc) rc = ro0301_detach (0, pecb->eiCxt); */
             if ((! rc) && (pecb->ecbRdbNo > 0)) {                      
                rc = ro0288_freeRdb (workScb->hRdbFile, pecb->ecbRdbNo);
             }                                                           
             /*
             pecb->stateFlg = 0;

             for (i = 0; i < RC0254_MaxLayerCxt; i++) {
	      pecb->eiCxt[i] = 0;
             }
             (workScb->activScb)--; */
             if (ro0032_localunlock (pScb->appliHd)) r = -R_LULOKE;     
         }
         else      r = -R_LLOCKE;  
         if (r) rc = r;                                             
         ecb++;                                                     
     }
ro0207_unlockSys ();

}
RM0004_RETURN_VOID (rs0275)                                    
}/*end*/                                                                        

/* EOF */
