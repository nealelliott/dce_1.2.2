/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spcmx00.c,v $
 * Revision 1.1.4.2  1996/03/11  13:22:51  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:32  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:58  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:21  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:54  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:08  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:05:21  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:40  keutel
 * 	creation
 * 	[1994/03/21  13:34:16  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spcmx00.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:51 $" } ;
#endif

/**********************************************************************/
/*                                                                    */
/*    COPYRIGHT  (C)						      */
/*    Siemens Nixdorf Informationssysteme AG 1993		      */
/*    All rights reserved					      */
/*								      */
/*    This software is furnished under licence and may be used only   */
/*    in accordance with the terms of that licence and with the       */
/*    inclusion of the above copyright notice.			      */
/*    This software may not be provided or otherwise made available   */
/*    to, or used by, any other person. No title to or ownership of   */
/*    the software is hereby transferred.			      */
/*                                                                    */
/**********************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME 	: spcmx00.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K.	         		      */
/* DATE 	: 27-10-93					      */
/*                                                                    */
/* COMPONENT    : CMX    Evaluation Program                           */
/* PRD#/VERS.   : RTROS-V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : In this module the trace record types are evaluated */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 03.00  K0 | 27-10-93| original		       | kc |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spcmx00.c 1.2 94/01/19   RTS-V2.00";

/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>	  /* General RTS definitons		      */
#include <rolgval.h>	  /* General RTS definitons		      */
#include <cmx.h>
#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <spcmx01.h>      /* Local function proto types               */
#include <spcmx10.h>      /* Local function proto types               */
        
/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0300_traceCmx()				      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K.	        		      */
/* DATE 	: 27-10-93					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int sp0300_traceCmx  (

long int	     lType,
long int	     lTime,
unsigned short int   argn,
char FAR * FAR *     argv,
unsigned long int   *argl,
unsigned short int   nerr)

/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/* IN-PARAMETERS: lType, lTime, argn, argl, argv		      */
/*                                                                    */
/* OUT-PARAMETERS:   none                                             */
/*                                                                    */
/* RETURN VALUES :   none                                             */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  Information is printed on stdout     */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/

/* ****************************************************************** */
/*              Local Variables                                       */
/* ****************************************************************** */

   signed   short int	rc   = OK;

/* ****************************************************************** */
/*              Start                                                 */
/* ****************************************************************** */

   lType &= RLro20_TypeField;

   switch (lType) {
      case (RL2001_EnTAttach & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(ENTRY t_attach)",
			    lType, lTime, nerr);
         if(argn == 2) sp0301_enTatach(argv,argl);
         else rc=ERROR;
	 break;

      case (RL2002_ReTAttach & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(RETURN t_attach)",
			    lType, lTime, nerr);
         if(argn == 2) sp0302_reTattach(argv,argl);
         else rc=ERROR;

	 break;

      case (RL2003_EnTDetach & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(ENTRY t_detach)",
			    lType, lTime, nerr);
         sp0303_enTdetach(argv,argl);
	 break;

      case (RL2004_ReTDetach & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(RETURN t_detach)",
			    lType, lTime, nerr);
         sp0304_reTdetach(argv,argl);
	 break;

      case (RL2005_TERROR & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(ENTRY t_error)",
			    lType, lTime, nerr);
         if(argl[0] > 0) 
             sp03a3_writeErrorVal(
                  RC1202_OFFSET2,"t_error.Value:",(int) *argv[0]);
         else rc=ERROR;
	 break;

      case (RL2006_EnTEvent & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(ENTRY t_event)",
			    lType, lTime, nerr);
         if(argn == 2) sp0306_enTevent(argv,argl);
         else rc=ERROR;
	 break;

      case (RL2007_ReTEvent  & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(RETURN t_event)",
			    lType, lTime, nerr);
         if(argn == 3) sp0307_reTevent(argv,argl);
         else rc=ERROR;
	 break;

      case (RL2008_EnTdisrq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(ENTRY t_disrq)",
			    lType, lTime, nerr);
         if(argn == 2) sp0308_enTdisrq(argv,argl);
         else rc=ERROR;
	 break;

      case (RL2009_ReTdisrq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(RETURN t_disrq)",
			    lType, lTime, nerr);
         sp0309_reTdisrq(argv,argl);
	 break;

      case (RL200a_EnTconrq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(ENTRY t_conrq)",
			    lType, lTime, nerr);
         if(argn == 3) sp030a_enTconrq(argv,argl);
         else rc=ERROR;
	 break;

      case (RL200b_ReTconrq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(RETURN t_conrq)",
			    lType, lTime, nerr);
         if(argn == 3) sp030b_reTconrq(argv,argl);
         else rc=ERROR;
	 break;

      case (RL200c_EnTredrq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(ENTRY t_redrq)",
			    lType, lTime, nerr);
         if(argn == 3) sp030c_enTredrq(argv,argl);
         else rc=ERROR;
	 break;

      case (RL200d_ReTredrq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"CMX_INTERFACE","(RETURN t_redrq)",
			    lType, lTime, nerr);
         sp030d_reTredrq(argv,argl);
	 break;

      case (RL200e_EnTconrs & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE","(ENTRY t_conrs)",
			    lType, lTime, nerr);
         if(argn == 2) sp030e_enTconrs(argv,argl);
         else rc=ERROR;
	 break;
      case (RL200f_ReTconrs & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE","(RETURN t_conrs)",
			    lType, lTime, nerr);
         sp030f_reTconrs(argv,argl);
	 break;


      case (RL2010_EnTinfo & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(ENTRY t_info)",
			    lType, lTime, nerr);
         if(argn == 2) sp0310_enTinfo(argv,argl);
         else rc=ERROR;
	 break;

      case (RL2011_ReTinfo & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(RETURN t_info)",
			    lType, lTime, nerr);
         if(argn == 2) sp0311_reTinfo(argv,argl);
         else rc=ERROR;
	 break;

      case (RL2012_EnTdatarq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE", "(ENTRY t_datarq)",
			    lType, lTime, nerr);
         if(argn == 4) sp0312_enTdatarq(argv,argl);
         else rc=ERROR;
	 break;

      case (RL2013_ReTdatarq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE","(RETURN t_datarq)",
			    lType, lTime, nerr);
         sp0313_reTdatarq(argv,argl);
	 break;


      case (RL2014_EnTconin & RLro20_TypeField):
	 sp1251_recordHead(RC1200_OFFSET0,"CMX_INTERFACE","(ENTRY t_conin)",
			    lType, lTime, nerr);
         if(argn == 4) sp0314_enTconin(argv,argl);
         else rc=ERROR;
	 break;

      case (RL2015_ReTconin & RLro20_TypeField):
	 sp1251_recordHead(RC1200_OFFSET0,"CMX_INTERFACE","(RETURN t_conin)",
			    lType, lTime, nerr);
         if(argn == 5) sp0315_reTconin(argv,argl);
         else rc=ERROR;
	 break;

      case (RL2016_EnTredin & RLro20_TypeField):
	 sp1251_recordHead(RC1200_OFFSET0,"CMX_INTERFACE","(ENTRY t_redin)",
			    lType, lTime, nerr);
         if(argn == 3) sp0316_enTredin(argv,argl);
         else rc=ERROR;
	 break;

      case (RL2017_ReTredin & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"CMX_INTERFACE","(RETURN t_redin)",
			    lType, lTime, nerr);
         if(argn == 4) sp0317_reTredin(argv,argl);
         else rc=ERROR;
	 break;

      case (RL2018_EnTconcf & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"CMX_INTERFACE","(ENTRY t_concf)",
			    lType, lTime, nerr);
         if(argn == 2) sp0318_enTconcf(argv,argl);
         else rc=ERROR;
	 break;

      case (RL2019_ReTconcf & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"CMX_INTERFACE","(RETURN t_concf)",
			    lType, lTime, nerr);
         if(argn == 2) sp0319_reTconcf(argv,argl);
         else rc=ERROR;
	 break;

      case (RL201a_EnTdisin & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"CMX_INTERFACE","(ENTRY t_disin)",
			    lType, lTime, nerr);
         if(argn == 2) sp031a_enTdisin(argv,argl);
         else rc=ERROR;
	 break;

      case (RL201b_ReTdisin & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE","(RETURN t_disin)",
			    lType, lTime, nerr);
         if(argn == 2) sp031b_reTdisin(argv,argl);
         else rc=ERROR;
	 break;

      case (RL201c_EnTdatain & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"CMX_INTERFACE","(ENTRY t_datain)",
			    lType, lTime, nerr);
         if(argn == 3) sp031c_enTdatain(argv,argl);
         else rc=ERROR;
	 break;

      case (RL201d_ReTdatain & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "CMX_INTERFACE","(RETURN t_datain)",
			    lType, lTime, nerr);
         if(argn == 3) sp031d_reTdatain(argv,argl);
         else rc=ERROR;
	 break;

      default:
	 rc = ERROR;
	 break;
   }

   if (rc == ERROR)
      sp1264_write (RC1202_OFFSET2, "CMX ERROR IN LOGGING FILE\n\n", NULL, NULL,
		    NULL, NULL);


   return (OK);
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0301_enTatach 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0301_enTatach (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
 if (argl[0] > 0){
   sp03a9_writeAddress ((union t_address FAR *)argv[0], 0);                    
 } 
 if (argl[1] > 0){
      struct t_opta2 * pp = (struct t_opta2 *) argv[1];
      
      sp03a0_writeAOptionNr(RC1202_OFFSET2,"t_attach.t_optnr :",pp->t_optnr);
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_attach.t_apmode:",    
                (signed int  *)&(pp->t_apmode) ,RC1206_DEC_NL); 
      if(pp->t_conlim == -1 )
       sp1264_write(RC1202_OFFSET2,"%s%s\n","t_attach.t_conlim:", "T_NOLIMIT",
              NULL, NULL); 
      else                                          
       sp1254_wrSgInt (RC1202_OFFSET2,      "t_attach.t_conlim:",    
                (signed int  *)&(pp->t_conlim) ,RC1206_DEC_NL); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_attach.t_uattid:",    
                (signed int  *)&(pp->t_uattid) ,RC1206_DEC_NL); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_attach.t_ccbits:",    
                (signed int  *)&(pp->t_ccbits) ,RC1208_HEX_NL); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_attach.t_sptypes:",    
                (signed int  *)&(pp->t_sptypes) ,RC1208_HEX_NL); 
 }
}/*end*/
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0302_reTattach 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0302_reTattach (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a2_writeFnRetVal(RC1202_OFFSET2,"t_attach.retValue:",(int) *argv[0]);
   }
   if (argl[1] > 0){
      struct t_opta2 * pp = (struct t_opta2 *) argv[1];
      
      sp03a0_writeAOptionNr(RC1202_OFFSET2,"t_attach.t_optnr :",pp->t_optnr);
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_attach.t_apmode:",    
                (signed int  *)&(pp->t_apmode) ,RC1206_DEC_NL); 
      if(pp->t_conlim == -1 )
       sp1264_write(RC1202_OFFSET2,"%s%s\n","t_attach.t_conlim:", "T_NOLIMIT",
              NULL, NULL); 
      else                                          
       sp1254_wrSgInt (RC1202_OFFSET2,      "t_attach.t_conlim:",    
                (signed int  *)&(pp->t_conlim) ,RC1206_DEC_NL); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_attach.t_uattid:",    
                (signed int  *)&(pp->t_uattid) ,RC1206_DEC_NL); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_attach.t_ccbits:",    
                (signed int  *)&(pp->t_ccbits) ,RC1208_HEX_NL); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_attach.t_sptypes:",    
                (signed int  *)&(pp->t_sptypes) ,RC1208_HEX_NL); 
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0303_enTdetach 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0303_enTdetach (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
   sp03a9_writeAddress ((union t_address FAR *)argv[0], 0);                    
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0304_reTdetach 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0304_reTdetach (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

     sp03a2_writeFnRetVal(RC1202_OFFSET2,"t_detach.retValue:",(int) *argv[0]);

}/*end*/

        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0306_enTevent 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0306_enTevent (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a4_writeWaitMode(RC1202_OFFSET2,"t_event.Cmode:",(int) *argv[0]);
   }
   if (argl[1] > 0){
      struct t_opte1 * pp = (struct t_opte1 *) argv[1];
     
      if(pp->t_optnr == T_OPTE1 ) 
        sp1264_write(RC1202_OFFSET2,"%s%s\n","t_event.t_optnr:",
                        "T_OPTE1",NULL,NULL);
      else {
        sp1264_write(RC1202_OFFSET2,"%s%s\n","t_event.t_optnr:",
                       "UNKNOWN",NULL,NULL);
        sp1254_wrSgInt (RC1202_OFFSET2,      "t_event.t_optnr:",    
                (signed int  *)&(pp->t_optnr) ,RC1206_DEC_NL); 
      }
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_event.t_attid:",    
                (signed int  *)&(pp->t_attid) ,RC1206_DEC_NL); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_event.t_uattid:",    
                (signed int  *)&(pp->t_uattid) ,RC1206_DEC_NL); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_event.t_ucepid:",    
                (signed int  *)&(pp->t_ucepid) ,RC1206_DEC_NL); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_event.t_timeout:",    
                (signed int  *)&(pp->t_timeout) ,RC1206_DEC_NL); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_event.t_evdat:",    
                (signed int  *)&(pp->t_evdat) ,RC1206_DEC_NL); 
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0307_reTevent  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0307_reTevent (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
   if (argl[0] > 0) {
     sp1257_wrUnLong (RC1202_OFFSET2, "t_event.TransConId: ",               
                (unsigned long int *)argv[0], RC1206_DEC_NL);
     sp1271_wrLine();
   } 
   if (argl[1] > 0) {
     sp03a7_writeTeventVal(RC1202_OFFSET2,"t_event.retValue:",(int) *argv[1]);
   }
   if (argl[2] > 0){
      struct t_opte1 * pp = (struct t_opte1 *) argv[2];
      
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_event.t_evdat:",    
                (signed int  *)&(pp->t_evdat) ,RC1206_DEC_NL); 
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0308_enTdisrq  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0308_enTdisrq (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_disrq.TransConId:",    
                (signed int  *)argv[0] ,RC1206_DEC_NL); 
   }
   if (argl[1] > 0){
      struct t_optc2 * pp = (struct t_optc2 *) argv[1];
      
      sp03a1_writeCOptionNr(RC1202_OFFSET2,"t_disrq.t_optnr :",pp->t_optnr);
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_disrq.t_udatal:",    
                (signed int  *)&(pp->t_udatal) ,RC1206_DEC_NL); 
      sp1258_wrAddr (RC1202_OFFSET2,      "t_disrq.t_udatap:",    
                (char FAR  *)pp->t_udatap ); 
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0309_reTdisrq  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0309_reTdisrq (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a2_writeFnRetVal(RC1202_OFFSET2,"t_disrq.retValue:",(int) *argv[0]);
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp030a_enTconrq  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp030a_enTconrq (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a9_writeAddress ((union t_address FAR *)argv[0], 1);                    
   }
   if (argl[1] > 0){
     sp03a9_writeAddress ((union t_address FAR *)argv[1], 0);                    
   }
   if (argl[2] > 0){
      struct t_optc3 * pp = (struct t_optc3 *) argv[2];
      
      sp03a1_writeCOptionNr(RC1202_OFFSET2,"t_conrq.t_optnr :",pp->t_optnr);
      sp1258_wrAddr (RC1202_OFFSET2,       "t_conrq.t_udatap:",
          (char FAR  *)pp->t_udatap );                  
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_conrq.t_udatal:",    
                (signed int  *)&(pp->t_udatal) ,RC1206_DEC_NL); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_conrq.t_xdata:",    
                pp->t_xdata ); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_conrq.t_timeout:",    
                pp->t_timeout ); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_conrq.t_ucepid:",    
                (signed int  *)&(pp->t_ucepid) ,RC1208_HEX_NL); 
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp030b_reTconrq  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp030b_reTconrq (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a2_writeFnRetVal(RC1202_OFFSET2,"t_conrq.retValue:",(int) *argv[0]);
   }
   if (argl[1] > 0){
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_conrq.TransConId:",    
                (signed int  *)argv[1] ,RC1206_DEC_NL); 
   }
     
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp030c_enTredrq  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp030c_enTredrq (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_redrq.TransConId:",    
                (signed int  *)argv[0] ,RC1206_DEC_NL); 
   }
   if (argl[1] > 0){
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_redrq.Pid:",    
                (signed int  *)argv[1] ,RC1206_DEC_NL); 
   }
   if (argl[2] > 0){
      struct t_optc3 * pp = (struct t_optc3 *) argv[2];
      
      sp03a1_writeCOptionNr(RC1202_OFFSET2,"t_redrq.t_optnr :",pp->t_optnr);
      sp1258_wrAddr (RC1202_OFFSET2,       "t_redrq.t_udatap:",
          (char FAR  *)pp->t_udatap );                  
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_redrq.t_udatal:",    
                (signed int  *)&(pp->t_udatal) ,RC1206_DEC_NL); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_redrq.t_xdata:",    
                pp->t_xdata ); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_redrq.t_timeout:",    
                pp->t_timeout ); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_redrq.t_ucepid:",    
                (signed int  *)&(pp->t_ucepid) ,RC1208_HEX_NL); 
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp030d_reTredrq  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp030d_reTredrq (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a2_writeFnRetVal(RC1202_OFFSET2,"t_redrq.retValue:",(int) *argv[0]);
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp030e_enTconrs  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp030e_enTconrs (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_conrs.TransConId:",    
                (signed int  *)argv[0] ,RC1206_DEC_NL); 
   }
   if (argl[1] > 0){
      struct t_optc3 * pp = (struct t_optc3 *) argv[1];
      
      sp03a1_writeCOptionNr(RC1202_OFFSET2,"t_conrs.t_optnr :",pp->t_optnr);
      sp1258_wrAddr (RC1202_OFFSET2,       "t_conrs.t_udatap:",
          (char FAR  *)pp->t_udatap );                  
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_conrs.t_udatal:",    
                (signed int  *)&(pp->t_udatal) ,RC1206_DEC_NL); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_conrs.t_xdata:",    
                pp->t_xdata ); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_conrs.t_timeout:",    
                pp->t_timeout ); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_conrs.t_ucepid:",    
                (signed int  *)&(pp->t_ucepid) ,RC1208_HEX_NL); 

   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp030f_reTconrs  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp030f_reTconrs (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a2_writeFnRetVal(RC1202_OFFSET2,"t_conrs.retValue:",(int) *argv[0]);
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0310_enTinfo  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0310_enTinfo (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_info.TransConId:",    
                (signed int  *)argv[0] ,RC1206_DEC_NL); 
   }
   if (argl[1] > 0){
      struct t_opti1 * pp = (struct t_opti1 *) argv[1];
     
      switch(pp->t_optnr){
          case T_OPTI1:
             sp1264_write(RC1202_OFFSET2,"%s%s\n","t_info.t_optnr",
                                "T_OPTI1",NULL,NULL);
             break;
          default:
            sp1264_write(RC1202_OFFSET2,"%s%s\n","t_info.t_optnr",
                          "UNKNOWN",NULL,NULL);
        sp1254_wrSgInt (RC1202_OFFSET2,      "/val",    
                (signed int  *)&(pp->t_optnr) ,RC1206_DEC_NL); 
            break;
      } 
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0311_reTinfo  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0311_reTinfo (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a2_writeFnRetVal(RC1202_OFFSET2,"t_onfo.retValue:",(int) *argv[0]);
   }
   if (argl[1] > 0){
      struct t_opti1 * pp = (struct t_opti1 *) argv[1];
      
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_info.t_maxl:",    
                (signed int  *)&(pp->t_maxl) ,RC1206_DEC_NL); 
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0312_enTdatarq  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0312_enTdatarq (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_datarq.TransConId:",    
                (signed int  *)argv[0],RC1206_DEC_NL); 
   }
   if (argl[1] > 0){
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_datarq.Length:",    
                (signed int  *)argv[1],RC1206_DEC_NL); 
   }
   if (argl[2] > 0)
       sp03a6_writeChainVal(RC1202_OFFSET2,"t_datarq.Chain :",(int)*argv[3]);
   if(argl[3] > 0)
     sp1263_hexDump (RC1202_OFFSET2, "t_datarq.data          : ", 
                       argv[3], argl[3]);                    
     

   

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0313_reTdatarq  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0313_reTdatarq (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a2_writeFnRetVal(RC1202_OFFSET2,"t_datarq.retValue:",(int) *argv[0]);
   }
   

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0314_enTconin  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0314_enTconin (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_conin.tref:",    
                (signed int  *)argv[0],RC1206_DEC_NL); 
   }
   if (argl[1] > 0){
     sp03a9_writeAddress ((union t_address FAR *)argv[1],0);                    
   }
   if (argl[2] > 0){
     sp03a9_writeAddress ((union t_address FAR *)argv[2],1);                    
   }
   if (argl[3] > 0){
      struct t_optc1 * pp = (struct t_optc1 *) argv[3];
      
      sp03a1_writeCOptionNr(RC1202_OFFSET2,"t_conin.t_optnr :",pp->t_optnr);
      sp1258_wrAddr (RC1202_OFFSET2,       "t_conin.t_udatap:",
          (char FAR  *)pp->t_udatap );                  
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_conin.t_udatal:",    
                (signed int  *)&(pp->t_udatal) ,RC1206_DEC_NL); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_conin.t_xdata:",    
                pp->t_xdata ); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_conin.t_timeout:",    
                pp->t_timeout ); 

   }

}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0315_reTconin  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0315_reTconin (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a2_writeFnRetVal(RC1202_OFFSET2,"t_conin.retValue:",(int) *argv[0]);
   }
   if (argl[1] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_conin.tref:",    
                (signed int  *)argv[1],RC1206_DEC_NL); 
   }
   if (argl[2] > 0){
     sp03a9_writeAddress ((union t_address FAR *)argv[2],0);                    
   }
   if (argl[3] > 0){
     sp03a9_writeAddress ((union t_address FAR *)argv[3],1);                    
   }
   if (argl[4] > 0){
      struct t_optc1 * pp = (struct t_optc1 *) argv[4];
      
      sp03a1_writeCOptionNr(RC1202_OFFSET2,"t_conin.t_optnr :",pp->t_optnr);
      sp1258_wrAddr (RC1202_OFFSET2,       "t_conin.t_udatap:",
          (char FAR  *)pp->t_udatap );                  
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_conin.t_udatal:",    
                (signed int  *)&(pp->t_udatal) ,RC1206_DEC_NL); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_conin.t_xdata:",    
                pp->t_xdata ); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_conin.t_timeout:",    
                pp->t_timeout ); 

   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0316_enTredin  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0316_enTredin (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_redin.tref:",    
                (signed int  *)argv[0] ,RC1206_DEC_NL); 
   }
   if (argl[1] > 0){
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_redin.Pid:",    
                (signed int  *)argv[1] ,RC1206_DEC_NL); 
   }
      
   
   if (argl[2] > 0){
      struct t_optc3 * pp = (struct t_optc3 *) argv[2];
      
      sp03a1_writeCOptionNr(RC1202_OFFSET2,"t_redin.t_optnr :",pp->t_optnr);

      sp1258_wrAddr (RC1202_OFFSET2,       "t_redin.t_udatap:",
          (char FAR  *)pp->t_udatap );                  
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_redin.t_udatal:",    
                (signed int  *)&(pp->t_udatal) ,RC1206_DEC_NL); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_redin.t_xdata:",    
                pp->t_xdata ); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_redin.t_timeout:",    
                pp->t_timeout ); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_redin.ucepid:",    
                (signed int  *)&(pp->t_ucepid) ,RC1208_HEX_NL); 
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0317_reTredin  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0317_reTredin (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a2_writeFnRetVal(RC1202_OFFSET2,"t_redin.retValue:",(int) *argv[0]);
   }
   if (argl[1] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_redin.tref:",    
                (signed int  *)argv[1] ,RC1206_DEC_NL); 
   }
   if (argl[2] > 0){
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_redin.Pid:",    
                (signed int  *)argv[2] ,RC1206_DEC_NL); 
   }
      
   
   if (argl[3] > 0){
      struct t_optc3 * pp = (struct t_optc3 *) argv[3];
      
      sp03a1_writeCOptionNr(RC1202_OFFSET2,"t_redin.t_optnr :",pp->t_optnr);

      sp1258_wrAddr (RC1202_OFFSET2,       "t_redin.t_udatap:",
          (char FAR  *)pp->t_udatap );                  
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_redin.t_udatal:",    
                (signed int  *)&(pp->t_udatal) ,RC1206_DEC_NL); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_redin.t_xdata:",    
                pp->t_xdata ); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_redin.t_timeout:",    
                pp->t_timeout ); 
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_redin.ucepid:",    
                (signed int  *)&(pp->t_ucepid) ,RC1208_HEX_NL); 
   }

}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0318_enTconcf  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0318_enTconcf (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_concf.TransConId:",    
                (signed int  *)argv[0],RC1206_DEC_NL); 
   }
   if (argl[1] > 0){
      struct t_optc1 * pp = (struct t_optc1 *) argv[1];
      
      sp03a1_writeCOptionNr(RC1202_OFFSET2,"t_concf.t_optnr :",pp->t_optnr);
      sp1258_wrAddr (RC1202_OFFSET2,       "t_concf.t_udatap:",
          (char FAR  *)pp->t_udatap );                  
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_concf.t_udatal:",    
                (signed int  *)&(pp->t_udatal) ,RC1206_DEC_NL); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_concf.t_xdata:",    
                pp->t_xdata ); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_concf.t_timeout:",    
                pp->t_timeout ); 

   }

}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0319_reTconcf  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp0319_reTconcf (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a2_writeFnRetVal(RC1202_OFFSET2,"t_concf.retValue:",(int) *argv[0]);
   }
   if (argl[1] > 0){
      struct t_optc1 * pp = (struct t_optc1 *) argv[1];
      
      sp03a1_writeCOptionNr(RC1202_OFFSET2,"t_concf.t_optnr :",pp->t_optnr);
      sp1258_wrAddr (RC1202_OFFSET2,       "t_concf.t_udatap:",
          (char FAR  *)pp->t_udatap );                  
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_concf.t_udatal:",    
                (signed int  *)&(pp->t_udatal) ,RC1206_DEC_NL); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_concf.t_xdata:",    
                pp->t_xdata ); 
      sp03a5_writeCmxOptions (RC1202_OFFSET2,      "t_concf.t_timeout:",    
                pp->t_timeout ); 

   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp031a_enTdisin  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp031a_enTdisin (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_disin.TransConId:",    
                (signed int  *)argv[0] ,RC1206_DEC_NL); 
   }
   if (argl[1] > 0){
      struct t_optc2 * pp = (struct t_optc2 *) argv[1];
      
      sp03a1_writeCOptionNr(RC1202_OFFSET2,"t_disin.t_optnr :",pp->t_optnr);
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_disin.t_udatal:",    
                (signed int  *)&(pp->t_udatal) ,RC1206_DEC_NL); 
      sp1258_wrAddr (RC1202_OFFSET2,      "t_disin.t_udatap:",    
                (char FAR  *)pp->t_udatap ); 
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp031b_reTdisin  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp031b_reTdisin (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a2_writeFnRetVal(RC1202_OFFSET2,"t_disin.retValue:",(int) *argv[0]);
   }
   if (argl[1] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_disin.AbortReason:",    
                (signed int  *)argv[1] ,RC1205_DEC); 
      sp03a8_writeAbortReason (RC1200_OFFSET0, " -->",(int)*(int *)argv[1] ); 
   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp031c_enTdatain  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp031c_enTdatain (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_datain.TransConId:",    
                (signed int  *)argv[0] ,RC1206_DEC_NL); 
   }
   if (argl[1] > 0){
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_datain.Length:",    
                (signed int  *)argv[1],RC1206_DEC_NL); 
   }
   if (argl[2] > 0){
       sp03a6_writeChainVal(RC1202_OFFSET2,"t_datain.Chain :",(int)*argv[3]);

   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp031d_reTdatain  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp031d_reTdatain (

char FAR * FAR *    argv,
unsigned long int  *argl)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   if (argl[0] > 0) {
     sp03a2_writeFnRetVal(RC1202_OFFSET2,"t_datain.retValue:",(int) *argv[0]);
   }
   if(argl[1] > 0)
       sp03a6_writeChainVal(RC1202_OFFSET2,"t_datain.Chain :",(int)*argv[1]);
   if(argl[2] > 0) {
      sp1254_wrSgInt (RC1202_OFFSET2,      "t_datain.Length:",    
                (signed int  *)&argl[2],RC1206_DEC_NL); 
      sp1263_hexDump (RC1202_OFFSET2, "t_datain.data          : ", 
                       argv[2], argl[2]);                    
   }

}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp03a0_writeAOptionNr                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp03a0_writeAOptionNr (

int          iOffset,
char    FAR *pStr,
int          iValue)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  switch(iValue){
    case T_OPTA1:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_OPTA1",NULL,NULL);
      break;
    case T_OPTA2:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_OPTA2",NULL,NULL);
      break;
    case T_OPTA4:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_OPTA4",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s\n",pStr,"UNKNOWN",NULL,NULL);
        sp1254_wrSgInt (RC1202_OFFSET2,      "Val:",    
                (signed int  *)&(iValue     ) ,RC1206_DEC_NL); 
      break;
      
  }

}/*end*/
 

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp03a1_writeCOptionNr                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp03a1_writeCOptionNr (

int          iOffset,
char    FAR *pStr,
int          iValue)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  switch(iValue){
    case T_OPTC1:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_OPTC1",NULL,NULL);
      break;
    case T_OPTC2:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_OPTC2",NULL,NULL);
      break;
    case T_OPTC3:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_OPTC3",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s\n",pStr,"UNKNOWN",NULL,NULL);
        sp1254_wrSgInt (RC1202_OFFSET2,      "Val:",    
                (signed int  *)&(iValue) ,RC1206_DEC_NL); 
      break;
  }

}/*end*/
 

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp03a2_writeFnRetVal                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp03a2_writeFnRetVal (

int          iOffset,
char    FAR *pStr,
int          iValue)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  switch(iValue){
    case T_NOTFIRST:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_NOTFIRST",NULL,NULL);
      break;
    case T_OK:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_OK",NULL,NULL);
      break;
    case T_ERROR:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_ERROR",NULL,NULL);
      break;
    case T_DATASTOP:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_DATASTOP",NULL,NULL);
      break;
    case T_XDATSTOP:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_XDATSTOP",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s\n",pStr,"UNKNOWN",NULL,NULL);
        sp1254_wrSgInt (RC1202_OFFSET2,      "Val:",    
                (signed int  *)&(iValue) ,RC1206_DEC_NL); 
      break;
  }

}/*end*/
 

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp03a3_writeErrorVal                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp03a3_writeErrorVal (

int          iOffset,
char    FAR *pStr,
int          iValue)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  switch(iValue){
    case T_NOERROR:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_NOERROR",NULL,NULL);
      break;
    case T_ENOENT:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_ENOENT",NULL,NULL);
      break;
    case T_EINTR:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_EINTR",NULL,NULL);
      break;
    case T_EIO:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_EIO",NULL,NULL);
      break;
    case T_EBADF:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_EBADF",NULL,NULL);
      break;
    case T_ENOMEM:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_ENOMEM",NULL,NULL);
      break;
    case T_EFAULT:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_EFAULT",NULL,NULL);
      break;
    case T_EBUSY:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_EBUSY",NULL,NULL);
      break;
    case T_EINVAL:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_EINVAL",NULL,NULL);
      break;
    case T_ENOBUF:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_ENOBUF",NULL,NULL);
      break;
    case T_UNSPECIFIED:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_UNSPECIFIED",NULL,NULL);
      break;
    case T_WSEQUENCE:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_WSEQUENCE",NULL,NULL);
      break;
    case T_WREQUEST:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_WREQUEST",NULL,NULL);
      break;
    case T_WPARAMETER:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_WPARAMETER",NULL,NULL);
      break;
    case T_WAPPLICATION:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_WAPPLICATION",NULL,NULL);
      break;
    case T_WAPP_LIMIT:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_WAPP_LIMIT",NULL,NULL);
      break;
    case T_WCONN_LIMIT:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_WCONN_LIMIT",NULL,NULL);
      break;
    case T_WTREF:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_WTREF",NULL,NULL);
      break;
    case T_WTUI_MSG:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_WTUI_MSG",NULL,NULL);
      break;
    case T_COLLISION:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_COLLISION",NULL,NULL);
      break;
    case T_WPROC_LIMIT:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_WPROC_LIMIT",NULL,NULL);
      break;
    case T_NOCCP:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_NOCCP",NULL,NULL);
      break;
    case T_ETIMEOUT:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_ETIMEOUT",NULL,NULL);
      break;
    case T_WROUTINFO:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_WROUTINFO",NULL,NULL);
      break;
    case T_CCP_END:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_CCP_END",NULL,NULL);
      break;
    case T_WRED_LIMIT:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_WRED_LIMIT",NULL,NULL);
      break;
    case T_WLIBVERSION:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_WLIBVERSION",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s\n",pStr,"UNKNOWN",NULL,NULL);
        sp1254_wrSgInt (RC1202_OFFSET2,      "Val:",    
                (signed int  *)&(iValue) ,RC1206_DEC_NL); 
      break;
  }

}/*end*/
 

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp03a4_writeWaitMode                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp03a4_writeWaitMode (

int          iOffset,
char    FAR *pStr,
int          iValue)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  switch(iValue){
    case T_WAIT:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_WAIT",NULL,NULL);
      break;
    case T_CHECK:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_CHECK",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s\n",pStr,"UNKNOWN",NULL,NULL);
        sp1254_wrSgInt (RC1202_OFFSET2,      "Val:",    
                (signed int  *)&(iValue) ,RC1206_DEC_NL); 
      break;
  }

}/*end*/
 

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp03a5_writeCmxOptions                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp03a5_writeCmxOptions (

int          iOffset,
char    FAR *pStr,
int          iValue)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :  Void                                 */
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
  switch(iValue){
    case T_YES:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_YES",NULL,NULL);
      break;
    case T_NO:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_NO",NULL,NULL);
      break;
    case T_NOLIMIT:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_NOLIMIT",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s\n",pStr,"UNKNOWN",NULL,NULL);
        sp1254_wrSgInt (RC1202_OFFSET2,      "Val:",    
                (signed int  *)&(iValue) ,RC1206_DEC_NL); 
      break;


  }

}/*end*/
 

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp03a6_writeChainVal                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp03a6_writeChainVal (

int          iOffset,
char    FAR *pStr,
int          iValue)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  switch(iValue){
    case T_END:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_END",NULL,NULL);
      break;
    case T_MORE:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_MORE",NULL,NULL);
      break;
    case T_VCNT:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_VCNT",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s\n",pStr,"UNKNOWN",NULL,NULL);
        sp1254_wrSgInt (RC1202_OFFSET2,      "Val:",    
                (signed int  *)&(iValue) ,RC1206_DEC_NL); 
      break;
  }

}/*end*/
 

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp03a7_writeTeventVal                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 27-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp03a7_writeTeventVal (

int          iOffset,
char    FAR *pStr,
int          iValue)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  switch(iValue){
    case T_NOEVENT:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_NOEVENT",NULL,NULL);
      break;
    case T_DATAIN:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_DATAIN",NULL,NULL);
      break;
    case T_DATAGO:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_DATAGO",NULL,NULL);
      break;
    case T_XDATIN:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_XDATIN",NULL,NULL);
      break;
    case T_XDATGO:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_XDATGO",NULL,NULL);
      break;
    case T_CONIN:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_CONIN",NULL,NULL);
      break;
    case T_CONCF:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_CONCF",NULL,NULL);
      break;
    case T_DISIN:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_DISIN",NULL,NULL);
      break;
    case T_REDIN:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_REDIN",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s\n",pStr,"UNKNOWN",NULL,NULL);
        sp1254_wrSgInt (RC1202_OFFSET2,      "Val:",    
                (signed int  *)&(iValue) ,RC1206_DEC_NL); 
      break;
  }

}/*end*/
 

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp03a8_writeAbortReason                             */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 02-11-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp03a8_writeAbortReason(

int          iOffset,
char    FAR *pStr,
int          iValue)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  switch(iValue){
    case T_RUNKNOWN:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RUNKNOWN",NULL,NULL);
      break;
    case T_RSAPCONGEST:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RSAPCONGEST",NULL,NULL);
      break;
    case T_RSAPNOTATT:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RSAPNOTATT",NULL,NULL);
      break;
    case T_RUNSAP:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RUNSAP",NULL,NULL);
      break;
    case T_RPERMLOST:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RPERMLOST",NULL,NULL);
      break;
    case T_RSYSERR:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RSYSERR",NULL,NULL);
      break;
    case T_RCONGEST:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RCONGEST",NULL,NULL);
      break;
    case T_RCONNFAIL:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RCONNFAIL",NULL,NULL);
      break;
    case T_RDUPREF:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RDUPREF",NULL,NULL);
      break;
    case T_RMISREF:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RMISREF",NULL,NULL);
      break;
    case T_RPROTERR:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RPROTERR",NULL,NULL);
      break;
    case T_RREFOFLOW:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RREFOFLOW",NULL,NULL);
      break;
    case T_RNOCONN:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RNOCONN",NULL,NULL);
      break;
    case T_RINLNG:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RINLNG",NULL,NULL);
      break;
    case T_RLCONGEST:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLCONGEST",NULL,NULL);
      break;
    case T_RLNOQOS:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLNOQOS",NULL,NULL);
      break;
    case T_RILLPWD:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RILLPWD",NULL,NULL);
      break;
    case T_RNETACC:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RNETACC",NULL,NULL);
      break;
    case T_RLPROTERR:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLPROTERR",NULL,NULL);
      break;
    case T_RLINTIDU:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLINTIDU",NULL,NULL);
      break;
    case T_RLNORMFLOW:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLNORMFLOW",NULL,NULL);
      break;
    case T_RLEXFLOW:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLEXFLOW",NULL,NULL);
      break;
    case T_RLINSAPID:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLINSAPID",NULL,NULL);
      break;
    case T_RLINCEPID:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLINCEPID",NULL,NULL);
      break;
    case T_RLINPAR:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLINPAR",NULL,NULL);
      break;
    case T_RLNOPERM:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLNOPERM",NULL,NULL);
      break;
    case T_RLPERMLOST:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLPERMLOST",NULL,NULL);
      break;
    case T_RLNOCONN:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLNOCONN",NULL,NULL);
      break;
    case T_RLCONNLOST:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLCONNLOST",NULL,NULL);
      break;
    case T_RLNORESP:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLNORESP",NULL,NULL);
      break;
    case T_RLIDLETRAF:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLIDLETRAF",NULL,NULL);
      break;
    case T_RLRESYNC:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLRESYNC",NULL,NULL);
      break;
    case T_RLEXLOST:
      sp1264_write(iOffset,"%s%s\n",pStr,"T_RLEXLOST",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,NULL,&iValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp03a9_writeAddress                             */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 28-12-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp03a9_writeAddress(
union t_address FAR *   pAddr,
int                     mode)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
 switch(mode){
   case 0:
      {
        struct t_myname FAR * pMyname;
 
        pMyname = (struct t_myname FAR *) pAddr;
        sp1264_write(RC1202_OFFSET2,"%s\n","OWN ADDRESS:",NULL,NULL,NULL); 
        sp1264_write(RC1203_OFFSET3,"%s%s\n","t_mnmode:","T_MNMODE",NULL,NULL);
        sp1273_wrSgShrt (RC1203_OFFSET3,      "t_mnlng:",
          (signed short int  *)&pMyname->t_mnlng ,RC1206_DEC_NL);  
        sp1263_hexDump (RC1202_OFFSET2, "t_myaddr:", 
                       (char FAR *)pMyname->t_mn,pMyname->t_mnlng);                    
              
      }break;
  case 1:
      {
        struct t_partaddr FAR * pPartAddr;
        
        pPartAddr = (struct t_partaddr FAR *) pAddr;
        sp1264_write(RC1202_OFFSET2,"%s\n","PARTNER ADDRESS:",NULL,NULL,NULL); 
        sp1264_write(RC1203_OFFSET3,"%s%s\n","t_pamode:","T_PAMODE",NULL,NULL);
        sp1273_wrSgShrt (RC1203_OFFSET3,      "t_palng:",
          (signed short int  *)&pPartAddr->t_palng ,RC1206_DEC_NL);  
        sp1263_hexDump (RC1202_OFFSET2, "t_paaddr:", 
                       (char FAR *)pPartAddr->t_pa,pPartAddr->t_palng);                    
              
      }break;       
}
}/* end*/

/* EOF */
        
