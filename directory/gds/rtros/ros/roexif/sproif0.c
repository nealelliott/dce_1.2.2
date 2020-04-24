/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sproif0.c,v $
 * Revision 1.1.4.2  1996/03/11  13:22:07  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:51  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:39:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:47  root]
 * 
 * Revision 1.1.2.5  1994/08/22  15:22:10  keutel
 * 	OT 11620
 * 	[1994/08/19  13:51:13  keutel]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:07  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:19:37  marrek]
 * 
 * Revision 1.1.2.3  1994/06/10  21:10:01  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:08  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:20  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:02:45  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:42  keutel
 * 	creation
 * 	[1994/03/21  13:31:42  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: sproif0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:07 $" } ;
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
/* NAME 	: sproif0.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K.	         		      */
/* DATE 	: 93-04-21					      */
/*                                                                    */
/* COMPONENT    : RTROS  Evaluation Program                           */
/* PRD#/VERS.   : RTROS-V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : In this module the trace record types are evaluated */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 03.00  K0 | 93-04-21| original		       | ks |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) sproif0.c 1.2 93/12/20   RTS-V2.00";

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
#include <ross.h>

#include <routil0.h>	  /* Function prototyping		      */
#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <sproif0.h>	  /* Function prototyping		      */

#include <sproif10.h>   
        
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
/* NAME 	: sp1300_traceExif()				      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K.	        		      */
/* DATE 	: 93-04-21					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int sp1300_traceExif (

long int	               lType,
long int	               lTime,
unsigned short int             argn,
char               FAR * FAR * argv,
unsigned long int        FAR * argl,
unsigned short int             nerr)

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
      case (RL1301_EnEVENT & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(entry EVENT)",
			    lType, lTime, nerr);
	 if (argn == 2) sp1301_enEvent (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL1302_ReEVENT & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(RETURN EVENT)",
			    lType, lTime, nerr);
	 if (argn == 2) sp1302_reEvent (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL1303_EnATTACH & RLro20_TypeField):
        if ( argl[0] == sizeof (RO_attapb)) 
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(ENTRY ro_atach)",
			    lType, lTime, nerr);
	 sp1303_enAttach (argv, argl);

	 break;

      case (RL1304_ReATTACH & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(RETURN ATTACH)",
			    lType, lTime, nerr);
	 if (argn == 2) sp1304_reAttach (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL1305_EnDETACH & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(ENTRY ro_dtach)",
			    lType, lTime, nerr);
	 if (argn == 1) sp1305_enDetach (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL1306_ReDETACH & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(RETURN DETACH)",
			    lType, lTime, nerr);
	 break;

      case (RL1307_EnBINDreq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(ENTRY ro_binrq)",
			    lType, lTime, nerr);
	 if (argn == 4) sp1307_enBindReq (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL1308_ReBINDreq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(RETURN BIND req)",
			    lType, lTime, nerr);
	 if (argn == 2) sp1308_reBindReq (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL1309_EnBINDrsp & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(ENTRY ro_binrs)",
			    lType, lTime, nerr);
	 if (argn == 3) sp1309_enBindRsp (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL1310_ReBINDrsp & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(RETURN BIND rsp)",
			    lType, lTime, nerr);
	 break;

      case (RL1311_ReBINDind & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(RETURN BIND ind)",
			    lType, lTime, nerr);
	 if (argn == 4) sp1311_reBindInd (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL1312_ReBINDcnf & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(RETURN BIND cnf)",
			    lType, lTime, nerr);
	 if (argn == 3) sp1312_reBindCnf (argv, argl);
	 else rc = ERROR;
	 break;




      case (RL1313_EnINVreq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(ENTRY ro_invrq)",
			    lType, lTime, nerr);
	 if (argn == 6) sp1313_enInvReq (argv, argl);
	 else rc = ERROR;
	 break;


      case (RL1314_ReINVind & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"R_INTERFACE","(RETURN INVOKE ind)",
			    lType, lTime, nerr);
	 if (argn == 5) sp1314_reInvInd (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL1315_EnRESreq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE","(ENTRY ro_resrq)",
			    lType, lTime, nerr);
	 if (argn == 4) sp1315_enResReq (argv, argl);
	 else rc = ERROR;
	 break;
      case (RL1316_ReRESind & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE","(RETURN RESULT ind)",
			    lType, lTime, nerr);
	 if (argn == 4) sp1316_reResInd (argv, argl);
	 else rc = ERROR;
	 break;


      case (RL1317_EnERRreq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(ENTRY ro_errrq)",
			    lType, lTime, nerr);
	 if (argn == 4) sp1317_enErrReq (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL1318_ReERRind & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(RETURN ERROR ind)",
			    lType, lTime, nerr);
	 if (argn == 4) sp1318_reErrInd   (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL1319_EnUREJreq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE", "(ENTRY ro_urej)",
			    lType, lTime, nerr);
	 if (argn == 1) sp1319_enURjReq (argv , argl );
	 else rc = ERROR;
	 break;

      case (RL131a_ReUREJind & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE","(RETURN REJECT ind)",
			    lType, lTime, nerr);
	 if (argn == 1) sp1320_reURjInd (argv, argl);
	 else rc = ERROR;
	 break;


      case (RL131b_RePREJind & RLro20_TypeField):
	 sp1251_recordHead(RC1200_OFFSET0,"R_INTERFACE","(RETURN REJECT-P ind)",
			    lType, lTime, nerr);
	 if (argn == 2) sp1321_rePRjInd    (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL131c_EnUBINDreq & RLro20_TypeField):
	 sp1251_recordHead(RC1200_OFFSET0,"R_INTERFACE","(ENTRY ro_ubnrq)",
			    lType, lTime, nerr);
	 if (argn == 3) sp1322_enUnBindReq(argv,argl);
	 else rc = ERROR;
	 break;

      case (RL131d_ReUBINDreq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"R_INTERFACE","(RETURN UNBIND req)",
			    lType, lTime, nerr);
	 if (argn == 2) sp1323_reUnBindReq (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL131e_EnUBINDrsp & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"R_INTERFACE","(ENTRY ro_ubnrs)",
			    lType, lTime, nerr);
	 if (argn == 2) sp1324_enUnBindRsp (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL131f_ReUBINDrsp & RLro20_TypeField):
	 break;

      case (RL131g_ReUBINDind & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"R_INTERFACE","(RETURN UNBIND ind)",
			    lType, lTime, nerr);
	 if (argn == 2)sp1326_reUnBindInd(argv,argl); 
	 else rc = ERROR;
	 break;

      case (RL131h_ReUBINDcnf & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0, "R_INTERFACE","(RETURN UNBIND cnf)",
			    lType, lTime, nerr);
	 if (argn == 2) sp1327_reUnBindCnf (argv, argl);
	 else rc = ERROR;
	 break;

      case (RL131i_EnUABORTreq & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"R_INTERFACE","(ENTRY ro_aborq)",
			    lType, lTime, nerr);
	 if (argn == 2) sp1328_enUAbortReq(argv,argl);
	 else rc = ERROR;
	 break;

      case (RL131j_ReUABORTreq & RLro20_TypeField):
	 break;

      case (RL131k_ReUABORTind & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"R_INTERFACE","(RETURN U-ABORT ind)",
			    lType, lTime, nerr);
	 if (argn == 2) 
           sp132a_reUAbortInd(argv,argl); 
	 else rc = ERROR;
	 break;

      case (RL131l_RePABORTind & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"R_INTERFACE","(RETURN P-ABORT ind)",
			    lType, lTime, nerr);
	  if(argn == 2)sp132b_rePAbortInd(argv,argl);
          else rc = ERROR;
	 break;
      case (RL131m_EnPROVBUFER & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"R_INTERFACE","(ENTRY ro_prbuf)",
			    lType, lTime, nerr);
         if (argn == 2) sp132c_enProvBuf(argv,argl);
         break; 
      case (RL131n_RePROVBUFER & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"R_INTERFACE","(RETURN r_rbinev)",
			    lType, lTime, nerr);
         if (argn == 2) sp132f_rnProvBuf(argv,argl);
         break; 
      
      case ( RL131t_InTraCwait & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"R_CWAITTRACE","(ENTRY ro_cwait)",
			    lType, lTime, nerr);
         sp132d_enCwait(argv,argl);
         break;
       
      case ( RL131u_RnInform & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"R_INTERFACE","(ENTRY ro_infrm)",
			    lType, lTime, nerr);
         if (argn == 2)
               sp132e_enInform(argv,argl); 
         
         break;
      case (RL131v_ReExit & RLro20_TypeField):
	 sp1251_recordHead (RC1200_OFFSET0,"R_INTERFACE",
                           "(RETURN of Rtros Call)",
			    lType, lTime, nerr);
          
         sp132g_rnrtrosCl(argv,argl);
         break;
      default:
	 rc = ERROR;
	 break;
   }

   if (rc == ERROR)
      sp1264_write (RC1202_OFFSET2, "ERROR IN LOGGING FILE\n\n", NULL, NULL,
		    NULL, NULL);


   return (OK);
}

        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	:                 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1301_enEvent (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
}/*end*/
 
       
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1302_reEvent  				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1302_reEvent (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
}/*end*/

                                                         
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1303_enAttach 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1303_enAttach (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
   /*-----------------------*/
   /* p  : RO_attapb  FAR * */
   /*-----------------------*/
   /*-------------------------------------*/
   /* len: (signed short int) sizeof (*p) */
   /* val: (char FAR	   *) p 	  */
   /*-------------------------------------*/
if ( argl[0] == sizeof (RO_attapb)) {
   if (argl[0] > 0) {
      RO_attapb         FAR  *pp;
      pp = (RO_attapb FAR *)argv[0];
/*    sp1252_blockBegin ("RO_attapb");*/

      sp1330_enHeader ((RO_fncthd FAR *)&(pp->r_hdinfo));

      sp1255_wrSgLong(RC1202_OFFSET2, "ATTACH.r_pid           : ",
                      (signed long FAR *)&(pp->r_pid),RC1206_DEC_NL);
      sp1256_wrUnInt(RC1202_OFFSET2, "ATTACH.r_apclst.r_apcnum:",
                       (unsigned int FAR *)&(pp->r_apclst.r_apcnum),RC1206_DEC_NL);
      sp1256_wrUnInt(RC1202_OFFSET2, "ATTACH.r_apclst.r_absnum:",
                       (unsigned int FAR *)&(pp->r_apclst.r_absnum),RC1206_DEC_NL);
      sp1256_wrUnInt(RC1202_OFFSET2, "ATTACH.r_apclst.r_trsnum:",
                       (unsigned int FAR *)&(pp->r_apclst.r_trsnum),RC1206_DEC_NL);

       
      /*sp1253_blockEnd ("RO_attapb");*/
   }

   /*-----------------------------------------------*/
   /* len: (signed short int) p->r_padd.r_paddln    */
   /* val: (char FAR	   *) p->r_padd.r_paddrf    */
   /*-----------------------------------------------*/
   if (argl[1] > 0) {
      
      sp1255_wrSgLong(RC1202_OFFSET2, "ATTACH.r_paddln           : ",
                      (signed long FAR *)&argl[1],RC1206_DEC_NL);
      sp1263_hexDump (RC1202_OFFSET2, "ATTACH.Pradd   : ",
			     argv[1], argl[1] );
   }

 }else if (argl[0] == sizeof(RO_apcary)){
     if(argl[1] > 0 )
         sp1274_wrUnShrt(RC1202_OFFSET2,"ATTACH.Appl.Cxt.Id     : ",
            (unsigned short int FAR *)argv[1], RC1206_DEC_NL);
     if(argl[2] > 0) {
      sp1282_WrObjId (RC1202_OFFSET2,  (char FAR *)"ATTACH.Appl.Cxt.Name   : ",
        argv[2],argl[2]);
     }
  
     if(argl[0] > 0){
      RO_apcary         FAR  *pp;
      pp = (RO_apcary FAR *) argv[0];
      sp1255_wrSgLong(RC1202_OFFSET2, "ATTACH.r_applcxt.r_pmode: ",
                      (signed long int FAR *)&(pp->r_pmode),RC1206_DEC_NL);
      sp1256_wrUnInt(RC1202_OFFSET2, "ATTACH.r_apclst.r_asecif: ",
                       (unsigned int FAR *)&(pp->r_asecif),RC1206_DEC_NL);
      sp1256_wrUnInt(RC1202_OFFSET2, "ATTACH.r_apclst.r_idsnum: ",
                       (unsigned int FAR *)&(pp->r_idsnum),RC1206_DEC_NL);
      if (argl[3] > 0){
        unsigned int i =0;
        sp1264_write(RC1202_OFFSET2,"%s","ATTACH.AplCxt.AbsSynIds : ",
               NULL,NULL,NULL);
        for (i=0; i < pp->r_idsnum;i++){
           sp1254_wrSgInt (RC1200_OFFSET0,(char FAR *)"",
                      (signed int FAR *)argv[3]+i ,RC1205_DEC);
           sp1265_indent(2);
        }   
        sp1271_wrLine();
      }
        sp1271_wrLine();

     }
 }else if ( argl[0] == sizeof(RO_absary)){
     if (argl[1] > 0)
      sp1274_wrUnShrt (RC1202_OFFSET2,"ATTACH.AbstractSynId :",
                           (unsigned short int FAR *)argv[1], RC1206_DEC_NL);
     if(argl[2] > 0){
      sp1282_WrObjId (RC1202_OFFSET2, (char FAR *)"ATTACH.AbsSyn Name   :",argv[2],argl[2]);
     }
     if (argl[0] > 0){
      RO_absary         FAR  *pp;
      pp = (RO_absary FAR *) argv[0]; 
      sp1254_wrSgInt (RC1203_OFFSET3, (char FAR *)"ATTACH.AbsSyn.Trsnum   : ",
                      (signed int FAR *)&(pp->r_idtnum) ,RC1206_DEC_NL);
      if (argl[3] > 0) {
        unsigned int i =0;
        sp1264_write(RC1203_OFFSET3,"%s","ATTACH.AbsSyn.TrSynIDs : ",
               NULL,NULL,NULL);
        for (i=0; i < pp->r_idtnum;i++){
           sp1254_wrSgInt (RC1200_OFFSET0,(char FAR *)"",
                      (signed int FAR *)argv[3]+i ,RC1205_DEC);
           sp1265_indent(2);
        }
        sp1271_wrLine();
      }
        sp1271_wrLine();
             
    }
      
 }else if (argl[0] == sizeof(RO_trsary)){
   if (argl[1] > 0 )
     sp1274_wrUnShrt( RC1202_OFFSET2,"ATTACH.TransferSynId :",
                           (unsigned short int FAR *)argv[1], RC1206_DEC_NL);
   if (argl[2] > 0){
      sp1282_WrObjId (RC1202_OFFSET2,(char FAR *)"ATTACH.TransSyn Name :",argv[2],argl[2]);
    } 
        sp1271_wrLine();
 }else if (argl[0] == sizeof(RO_apname)){
    RO_apname FAR * p = (RO_apname FAR *) argv[0];
    char vStr[32]="WRONG AET";
 
    switch (p->r_aptype){
      case (RO_AETF1 | RO_AETCK):
         ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_AETF1 | RO_AETCK");
         break;
      case (RO_AETF1 | RO_AETNC):
         ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_AETF1 | RO_AETNC");
         break;
      case (RO_AETF2 | RO_AETCK):
         ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_AETF2 | RO_AETCK");
         break;
      case (RO_AETF2 | RO_AETNC):
         ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_AETF2 | RO_AETNC");
         break;
      default:
         sp1255_wrSgLong(RC1202_OFFSET2, "AE-Title.r_aptype: ",
                      (signed long int FAR *)&(p->r_aptype),RC1206_DEC_NL);
         break;
    }
    sp1264_write(RC1202_OFFSET2,"%s%s\n","AE-Title.r_aptype:",vStr,
               NULL,NULL);    
    if ((p->r_aptype & ~RO_AETNC) == RO_AETF1){
     if(argl[1] > 0){
       RO_aetfm1 FAR * pAeTitl1 = (RO_aetfm1 FAR *)p->r_aetval;

       sp1255_wrSgLong(RC1202_OFFSET2, "AE-Title.r_aetitl1.r_aptnum: ",
                    (signed long int FAR *)&(pAeTitl1->r_aptnum),RC1206_DEC_NL);
       sp1255_wrSgLong(RC1202_OFFSET2, "AE-Title.r_aetitl1.r_aeqnum: ",
                    (signed long int FAR *)&(pAeTitl1->r_aeqnum),RC1206_DEC_NL);
     }
      
    }
    else if((p->r_aptype & ~RO_AETNC) == RO_AETF2){
       if(argl[1] > 0 )  {
         sp1282_WrObjId (RC1202_OFFSET2,(char FAR *)"AE-Title.aptitl  : ",argv[1],argl[1]);
         sp1271_wrLine();
       }
       if(argl[2] > 0) {
         sp1255_wrSgLong(RC1202_OFFSET2, "AE-Title.r_aequal           : ",
                      (signed long int FAR *)argv[2],RC1206_DEC_NL);
       } 
    }else 
        sp1264_write(RC1203_OFFSET3,"%s","AE-Title.r_aetitl.r_aptype is WRONG ",
               NULL,NULL,NULL);
      
       
 }else if (argl[0] == sizeof(RO_rdname)){
         RO_rdname FAR * p = (RO_rdname FAR *) argv[0];
         sp1255_wrSgLong(RC1202_OFFSET2, "AE-Title.r_aetitl1.r_rdnnum : ",
                      (signed long int FAR *)p->r_rdnnum,RC1206_DEC_NL);
       
 }else if (argl[0] == sizeof(RO_avastr)){
         RO_avastr FAR * p = (RO_avastr FAR *) argv[0];
        
         sp1255_wrSgLong(RC1202_OFFSET2, "AE-Title.r_aetitl1.r_avastr : ",
                      (signed long int FAR *)p->r_avanum,RC1206_DEC_NL);
         if(argl[1] > 0) {
           sp1282_WrObjId (RC1202_OFFSET2,(char FAR *)"AE-Title.aptitl1.r_avatyp  : ",
                   argv[1],argl[1]);
           sp1271_wrLine();
         }
         if(argl[2] > 0)
             sp1263_hexDump (RC1202_OFFSET2, "AE-Title.Aetitl1.AVAval: ",
			     argv[2], argl[2] );
  } 
   

}/*end*/
        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1304_reAttach 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1304_reAttach (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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

}/*end*/
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1305_enDetach 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1305_enDetach (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_detapb          FAR *pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*-----------------------------------------------------*/
   /* len: (signed short int) sizeof (p->pbhead->version) */
   /* val: (char FAR	   *) &(p->pbhead->version)	  */
   /*-----------------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_detapb FAR *) argv[0];
      
      /*sp1252_blockBegin ("RO_detapb");*/
      sp1330_enHeader((RO_fncthd FAR *)&pp->r_hdinfo);

      sp1257_wrUnLong(RC1202_OFFSET2, "DETACH.APPLID         : ",
                      (unsigned long int FAR *)&(pp->r_aplid) ,RC1206_DEC_NL);
      /*sp1253_blockEnd ("RO_detapb");*/
   }

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1307_enBindReq				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1307_enBindReq (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_bnrqpb          FAR *pp  = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
   if (argl[0] > 0) {
      unsigned long int i =0;
      pp = (RO_bnrqpb FAR *) argv[0];
      
      /*sp1252_blockBegin ("RO_bnrqpb");*/
      sp1330_enHeader((RO_fncthd FAR *)&(pp->r_hdinfo));
      
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDreq.APPLID        : ",
                      (unsigned long int FAR *)&(pp->r_aplid),RC1206_DEC_NL);
      sp1333_writeAclass(RC1202_OFFSET2,(char FAR *)"BINDreq.r_aclass       : ",
                         pp->r_aclass);

      sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"BINDreq.r_apcon        : ",
                      (signed int FAR *)&(pp->r_apcon) ,RC1206_DEC_NL);
      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"BINDreq.SNFrag.r_mtype : ",
                         pp->r_sndfra.r_mtype);

      sp1258_wrAddr (RC1202_OFFSET2, "BINDreq.SNFrag.r_fraref : ",
                      (char  FAR *)(pp->r_sndfra.r_fraref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDreq.SNFrag.r_fralen: ",
                      (unsigned long int FAR *)&(pp->r_sndfra.r_fralen),
                       RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDreq.SNFrag.r_fracnt: ",
                      (unsigned long int FAR *)&(pp->r_sndfra.r_fracnt),
                       RC1206_DEC_NL);
      if ( (pp->r_sndfra.r_fracnt < 6) && (pp->r_sndfra.r_fraref))
         for ( i=0;i< pp->r_sndfra.r_fracnt;i++){
           sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"BINDreq.SNFrag.r_absid : ",
                      (signed int FAR *)&(pp->r_sndfra.r_fraary[i].r_absid) ,
                        RC1206_DEC_NL);
           sp1257_wrUnLong(RC1202_OFFSET2, "BINDreq.SNFrag.r_sublen: ",
                      (unsigned long int FAR *)&(pp->r_sndfra.r_fraary[i].r_sublen),
                       RC1206_DEC_NL);
    }
      sp1271_wrLine(); 
                    
      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"BINDreq.EVFrag.r_mtype : ",
                         pp->r_buffer.r_mtype);
      sp1258_wrAddr (RC1202_OFFSET2, "BINDreq.EVFrag.r_bufref : ",
                      (char   FAR *)(pp->r_buffer.r_bufref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDreq.EVFrag.r_buflen: ",
                      (unsigned long int FAR *)&(pp->r_buffer.r_buflen),
                       RC1206_DEC_NL);
      /*sp1253_blockEnd ("RO_bnrqpb");*/
   }

   /*-------------------------------------*/
   /* len: (signed short int) FileLen     */
   /* val: (char FAR	   *) FileName    */
   /*-------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "BINDreq.SndBuffer :",
                            argv[1],argl[1]);

   }

   /*----------------------------------------------------*/
   /* len: (signed short int) File Name Length        	 */
   /* val: (char FAR	   *) File Name     		 */
   /*----------------------------------------------------*/

   if (argl[2] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "BINDreq.BufBuffer :",
                            argv[2],argl[2]);
   }

   /*----------------------------------------------*/
   /* len: (signed short int) p->r_pradd.r_paddln */
   /* val: (char FAR	   *) p->r_pradd.r_paddrf */
   /*----------------------------------------------*/

   if (argl[3] > 0) {
      sp1255_wrSgLong(RC1202_OFFSET2, "BINDreq.r_paradd.r_paddln : ",
                      (signed long int FAR *)&argl[3],RC1206_DEC_NL);
      sp1263_hexDump (RC1202_OFFSET2, "BINDreq.r_paradd.r_paddrf: ",
			     argv[3], argl[3]);
   }

}/*end*/
        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1308_reBindReq				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1308_reBindReq (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1309_enBindRsp				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1309_enBindRsp (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_bnrspb   FAR *pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*-----------------------------------------------------*/
   /* len: (signed short int) sizeof (RO_bnrspb)          */
   /* val: (char FAR	   *) RO_bnrspb            	  */
   /*-----------------------------------------------------*/

   if (argl[0] > 0) {
      unsigned long int i;
      pp = (RO_bnrspb FAR *)argv[0];
      /*sp1252_blockBegin ("RO_bnrspb");*/
      sp1330_enHeader((RO_fncthd FAR *)&(pp->r_hdinfo));
      
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDrsp.ASSOCID        : ",
                      (unsigned long int FAR *)&(pp->r_assid),RC1206_DEC_NL);
/*      sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"BINDrsp.r_aclass       : ",
                      (signed int FAR *)&(pp->r_aclass) ,RC1206_DEC_NL); */
      sp1333_writeAclass(RC1202_OFFSET2,(char FAR *)"BINDrsp.r_aclass       : ",
                         pp->r_aclass);
      sp1336_writeResult(RC1202_OFFSET2, (char FAR *)"BINDrsp.r_result        : ",
                      pp->r_result);

      sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"BINDrsp.r_apcon        : ",
                      (signed int FAR *)&(pp->r_apcon) ,RC1206_DEC_NL);
          
      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"BINDrsp.SNFrag.r_mtype : ",
                         pp->r_sndfra.r_mtype);
      sp1258_wrAddr (RC1202_OFFSET2, "BINDrsp.SNFrag.r_fraref : ",
                      (char  FAR *)(pp->r_sndfra.r_fraref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDrsp.SNFrag.r_fralen: ",
                      (unsigned long int FAR *)&(pp->r_sndfra.r_fralen),
                       RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDrsp.SNFrag.r_fracnt: ",
                      (unsigned long int FAR *)&(pp->r_sndfra.r_fracnt),
                       RC1206_DEC_NL);
      if ( (pp->r_sndfra.r_fracnt < 6) && (pp->r_sndfra.r_fraref))
         for ( i=0;i< pp->r_sndfra.r_fracnt;i++){
           sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"BINDrsp.SNFrag.r_absid : ",
                      (signed int FAR *)&(pp->r_sndfra.r_fraary[i].r_absid) ,
                        RC1206_DEC_NL);
           sp1257_wrUnLong(RC1202_OFFSET2, "BINDrsp.SNFrag.r_sublen: ",
                      (unsigned long int FAR *)&(pp->r_sndfra.r_fraary[i].r_sublen),
                       RC1206_DEC_NL);
    }
      sp1271_wrLine(); 
                    
      /*sp1253_blockEnd ("RO_bnrspb");*/
   }

   /*-------------------------------------*/
   /* len: (signed short int) FileLen     */
   /* val: (char FAR	   *) FileName    */
   /*-------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "BINDrsp.SndBuffer :",
                            argv[1],argl[1]);

   }

   /*-------------------------------------------------*/
   /* len: (signed short int) ApTitleLength           */
   /* val: ApTitle                                    */
   /*-------------------------------------------------*/



   /*----------------------------------------------*/
   /* len: (signed short int) p->r_pradd.r_paddln */
   /* val: (char FAR	   *) p->r_pradd.r_paddrf */
   /*----------------------------------------------*/

   if (argl[2] > 0) {
      sp1255_wrSgLong(RC1202_OFFSET2, "BINDrsp.r_paradd.r_paddln : ",
                      (signed long int FAR *)&argl[2],RC1206_DEC_NL);
      sp1263_hexDump (RC1202_OFFSET2, "BINDrsp.r_paradd.r_paddrf: ",
			     argv[2], argl[2]);
   }

}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1311_reBindInd				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1311_reBindInd (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_bninev FAR * pp;
unsigned long int i;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_bninev FAR *)argv[0];
      /*sp1252_blockBegin ("RO_bninev");*/
      sp1331_reEvHead ((RO_evnthd FAR *)&(pp->r_evnthd));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDind.APPLID         : ",
                      (unsigned long int FAR *)&(pp->r_aplid) , RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDind.ASSOCID         : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1255_wrSgLong(RC1202_OFFSET2, "BINDind.r_pid          : ",
                      (signed long int FAR *)&(pp->r_pid),RC1206_DEC_NL);
      sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"BINDind.r_ptcon        : ",
                      (signed int FAR *)&(pp->r_ptcon) ,RC1206_DEC_NL);


      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"BINDind.EVFrag.r_mtype : ",
                         pp->r_evfrag.r_mtype);
      sp1280_writeBool(RC1202_OFFSET2, "BINDind.EvFrag.r_specfl : ",
                       pp->r_evfrag.r_specfl);

      sp1258_wrAddr (RC1202_OFFSET2, "BINDind.EvFrag.r_bufref : ",
                      (char  FAR *)(pp->r_evfrag.r_bufref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDind.EvFrag.r_buflen: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_buflen),
                       RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDind.EvFrag.r_fracnt: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_fracnt),
                       RC1206_DEC_NL);
      if ( (pp->r_evfrag.r_fracnt < 6) && (pp->r_evfrag.r_bufref))
         for ( i=0;i< pp->r_evfrag.r_fracnt;i++){
           sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"BINDind.EvFrag.r_absid : ",
                      (signed int FAR *)&(pp->r_evfrag.r_fraary[i].r_absid) ,
                        RC1206_DEC_NL);
           sp1257_wrUnLong(RC1202_OFFSET2, "BINDind.EvFrag.r_sublen: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_fraary[i].r_sublen),
                       RC1206_DEC_NL);
    }
      sp1271_wrLine(); 
      /*sp1253_blockEnd ("RO_bninev");*/
   }

   /*-------------------------------------*/
   /* len: (signed short int) FileLen     */
   /* val: (char FAR	   *) FileName    */
   /*-------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "BINDind.EvtBuffer :",
                            argv[1],argl[1]);

   }
   /*-------------------------------------------------*/
   /* len: (signed short int) ApTitleLength           */
   /* val: ApTitle                                    */
   /*-------------------------------------------------*/


   /*----------------------------------------------*/
   /* len: (signed short int) p->r_pradd.r_paddln */
   /* val: (char FAR	   *) p->r_pradd.r_paddrf */
   /*----------------------------------------------*/

   if (argl[2] > 0) {
      sp1255_wrSgLong(RC1202_OFFSET2, "BINDind.r_paradd.r_paddln : ",
                      (signed long int FAR *)&argl[2],RC1206_DEC_NL);
      sp1263_hexDump (RC1202_OFFSET2, "BINDind.r_paradd.r_paddrf: ",
			     argv[2], argl[2]);
   }
   /*----------------------------------------------*/
   /* len: (signed short int) p->r_cldpad.r_paddln */
   /* val: (char FAR	   *) p->cldpad.r_paddrf */
   /*----------------------------------------------*/

   if (argl[3] > 0) {
      sp1255_wrSgLong(RC1202_OFFSET2, "BINDind.r_cldpad.r_paddln : ",
                      (signed long int FAR *)&argl[3],RC1206_DEC_NL);
      sp1263_hexDump (RC1202_OFFSET2, "BINDind.r_cldpad.r_paddrf: ",
			     argv[3], argl[3]);
   }


      sp1271_wrLine();
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1312_reBindCnf				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1312_reBindCnf (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_bncnev   FAR * pp;
unsigned long int i;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_bncnev FAR *)argv[0];
      /*sp1252_blockBegin ("RO_bninev");*/
      sp1331_reEvHead ((RO_evnthd FAR *)&(pp->r_evnthd));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDcnf.ASSOCID         : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1336_writeResult(RC1202_OFFSET2, (char FAR *)"BINDcnf.r_result          : ",
                      pp->r_result);
      sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"BINDcnf.r_ptcon        : ",
                      (signed int FAR *)&(pp->r_ptcon) ,RC1206_DEC_NL);


      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"BINDcnf.EVFrag.r_mtype : ",
                         pp->r_evfrag.r_mtype);
      sp1280_writeBool(RC1202_OFFSET2, "BINDcnf.EvFrag.r_specfl : ",
                       pp->r_evfrag.r_specfl);

      sp1258_wrAddr (RC1202_OFFSET2, "BINDcnf.EvFrag.r_bufref : ",
                      (char  FAR *)(pp->r_evfrag.r_bufref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDcnf.EvFrag.r_buflen: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_buflen),
                       RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2, "BINDcnf.EvFrag.r_fracnt: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_fracnt),
                       RC1206_DEC_NL);
      if ( (pp->r_evfrag.r_fracnt < 6) && (pp->r_evfrag.r_bufref))
         for ( i=0;i< pp->r_evfrag.r_fracnt;i++){
           sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"BINDcnf.EvFrag.r_absid : ",
                      (signed int FAR *)&(pp->r_evfrag.r_fraary[i].r_absid) ,
                        RC1206_DEC_NL);
           sp1257_wrUnLong(RC1202_OFFSET2, "BINDcnf.EvFrag.r_sublen: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_fraary[i].r_sublen),
                       RC1206_DEC_NL);
    }
      sp1271_wrLine(); 
      /*sp1253_blockEnd ("RO_bncnev");*/
   }


   /*-------------------------------------*/
   /* len: (signed short int) FileLen     */
   /* val: (char FAR	   *) FileName    */
   /*-------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "BINDcnf.EvtBuffer :",
                            argv[1],argl[1]);

   }


   /*----------------------------------------------*/
   /* len: (signed short int) p->r_pradd.r_paddln */
   /* val: (char FAR	   *) p->r_pradd.r_paddrf */
   /*----------------------------------------------*/

   if (argl[2] > 0) {
      sp1255_wrSgLong(RC1202_OFFSET2, "BINDcnf.r_paradd.r_paddln : ",
                      (signed long int FAR *)&argl[2],RC1206_DEC_NL);
      sp1263_hexDump (RC1202_OFFSET2, "BINDcnf.r_paradd.r_paddrf: ",
			     argv[2], argl[2]);
   }

      sp1271_wrLine();
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1313_enInvReq 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1313_enInvReq (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_ivrqpb   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_ivrqpb FAR *)argv[0];
      /*sp1252_blockBegin ("RO_ivrqpb");*/
      sp1330_enHeader ((RO_fncthd FAR *)&(pp->r_hdinfo));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "INVreq.ASSOCID          : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1255_wrSgLong(RC1202_OFFSET2, "INVreq.r_invid           : ",
                      (signed long int FAR *)&(pp->r_invid),RC1206_DEC_NL);
      sp1334_writeOpclass(RC1202_OFFSET2,(char FAR *)"INVreq.r_oclass        : ",
                           pp->r_oclass);
      sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"INVreq.r_absid        : ",
                      (signed int FAR *)&(pp->r_absid) ,RC1206_DEC_NL);
      

      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"INVreq.SnFrag.r_mtype : ",
                         pp->r_sndata.r_mtype);
      sp1258_wrAddr (RC1202_OFFSET2, "INVreq.SnData.r_fraref : ",
                      (char   FAR *)(pp->r_sndata.r_fraref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "INVreq.SnData.r_fralen: ",
                      (unsigned long int FAR *)&(pp->r_sndata.r_fralen),
                       RC1206_DEC_NL);
      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"INVreq.EvData.r_mtype : ",
                         pp->r_buffer.r_mtype);
      sp1258_wrAddr (RC1202_OFFSET2, "INVreq.EvData.r_bufref : ",
                      (char   FAR *)(pp->r_buffer.r_bufref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "INVreq.EvData.r_buflen: ",
                      (unsigned long int FAR *)&(pp->r_buffer.r_buflen),
                       RC1206_DEC_NL);
      /*sp1253_blockEnd ("RO_ivrqpb");*/
   }

   /*----------------------------------------*/
   /* len: (signed short int) SndFileLen     */
   /* val: (char FAR	   *) SndFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "INVreq.SndFile :",
                            argv[1],argl[1]);

   }
   /*----------------------------------------*/
   /* len: (signed short int) BufFileLen     */
   /* val: (char FAR	   *) BufFileName    */
   /*----------------------------------------*/

   if (argl[2] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "INVreq.BufFile   :",
                            argv[2],argl[2]);

   }
   /*----------------------------------------*/
   /* len: (signed short int) LinkIdLen      */
   /* val: (char FAR	   *) LinkId         */
   /*----------------------------------------*/

   if (argl[3] > 0) {
      sp1255_wrSgLong(RC1202_OFFSET2, "INVreq.r_linkid        : ",
                      (signed long int FAR *)argv[3],RC1206_DEC_NL);

   }
   if (argl[4] > 0) {
      sp1255_wrSgLong(RC1202_OFFSET2, "INVreq.r_opval.r_oploc   : ",
                      (signed long int FAR *)argv[4] ,RC1206_DEC_NL); 
   }
   if (argl[5] > 0 ){
      sp1282_WrObjId (RC1202_OFFSET2,(char FAR *)"INVreq.r_opval.r_opglb: ",argv[5],argl[5]);  /*
      sp1262_asciiOrHexDump(RC1202_OFFSET2, (char FAR *)"INVreq.r_opval.r_opglb: ",
                            argv[5],argl[5],FALSE ); */
   }

}/*end*/
        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1314_reInvInd 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1314_reInvInd (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_ivinev   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_ivinev FAR *)argv[0];
      /*sp1252_blockBegin ("RO_ivinev");*/
      sp1331_reEvHead ((RO_evnthd FAR *)&(pp->r_evnthd));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "INVind.ASSOCID          : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1255_wrSgLong(RC1202_OFFSET2, "INVind.r_invid           : ",
                      (signed long int  FAR *)&(pp->r_invid),RC1206_DEC_NL);

      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"INVind.EvData.r_mtype : ",
                         pp->r_evdata.r_mtype);
      sp1258_wrAddr (RC1202_OFFSET2, "INVind.EvData.r_bufref : ",
                      (char   FAR *)(pp->r_evdata.r_bufref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "INVind.EvData.r_buflen: ",
                      (unsigned long int FAR *)&(pp->r_evdata.r_buflen),
                       RC1206_DEC_NL);
      /*sp1253_blockEnd ("RO_ivinev");*/
   }

   /*----------------------------------------*/
   /* len: (signed short int) EvtFile        */
   /* val: (char FAR	   *) EvtFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "INVind.EvtFile :",
                            argv[1],argl[1]);

   }
   /*----------------------------------------*/
   /* len: (signed short int) LinkIdLen      */
   /* val: (char FAR	   *) LinkId         */
   /*----------------------------------------*/

   if (argl[2] > 0) {
      sp1255_wrSgLong(RC1202_OFFSET2, "INVind.r_linkid        : ",
                      (signed long int FAR *)argv[2],RC1206_DEC_NL);

   }
   if (argl[3] > 0) {
      sp1255_wrSgLong(RC1202_OFFSET2, "INVind.r_opval.r_oploc   : ",
                      (signed long int FAR *)argv[3] ,RC1206_DEC_NL); 
   }
   if (argl[4] > 0 ){
      sp1282_WrObjId (RC1202_OFFSET2,(char FAR *)"INVind.r_opval.r_opglb: ",argv[4],argl[4]);
   }

}/*end*/
        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1315_enResReq 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1315_enResReq (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_rsrqpb   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_rsrqpb FAR *)argv[0];
      /*sp1252_blockBegin ("RO_rsrqpb");*/
      sp1330_enHeader ((RO_fncthd FAR *)&(pp->r_hdinfo));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "RESreq.ASSOCID          : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1255_wrSgLong(RC1202_OFFSET2, "RESreq.r_invid           : ",
                      (signed long int FAR *)&(pp->r_invid),RC1206_DEC_NL);
      sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"RESreq.r_absid        : ",
                      (signed int  FAR *)&(pp->r_absid) ,RC1206_DEC_NL);

      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"RESreq.SnData.r_mtype : ",
                         pp->r_sndata.r_mtype);
      sp1258_wrAddr (RC1202_OFFSET2, "RESreq.SnData.r_fraref : ",
                      (char   FAR *)(pp->r_sndata.r_fraref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "RESreq.SnData.r_fralen: ",
                      (unsigned long int FAR *)&(pp->r_sndata.r_fralen),
                       RC1206_DEC_NL);
      /*sp1253_blockEnd ("RO_rsrqpb");*/
   }

   /*----------------------------------------*/
   /* len: (signed short int) SndFileLen     */
   /* val: (char FAR	   *) SndFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "RESreq.SndFile :",
                            argv[1],argl[1]);

   }
   if (argl[2] > 0) {
      sp1255_wrSgLong(RC1202_OFFSET2, "RESreq.r_opval.r_oploc   : ",
                      (signed long int FAR *)argv[2] ,RC1206_DEC_NL); 
   }
   if (argl[3] > 0 ){
      sp1282_WrObjId (RC1202_OFFSET2,(char FAR *)"RESreq.r_opval.r_opglb: ",argv[3],argl[3]);
   }

}/*end*/
        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1316_reResInd 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1316_reResInd (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_rsinev   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_rsinev FAR *)argv[0];
      /*sp1252_blockBegin ("RO_rsinev");*/
      sp1331_reEvHead ((RO_evnthd FAR *)&(pp->r_evnthd));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "RESind.ASSOCID          : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1255_wrSgLong(RC1202_OFFSET2, "RESind.r_invid           : ",
                      (signed long int FAR *)&(pp->r_invid),RC1206_DEC_NL);

      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"RESind.EvData.r_mtype : ",
                         pp->r_evdata.r_mtype);
      sp1258_wrAddr (RC1202_OFFSET2, "RESind.EvData.r_bufref : ",
                      (char   FAR *)(pp->r_evdata.r_bufref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "RESind.EvData.r_buflen: ",
                      (unsigned long int FAR *)&(pp->r_evdata.r_buflen),
                       RC1206_DEC_NL);
      /*sp1253_blockEnd ("RO_rsinev");*/
   }

   /*----------------------------------------*/
   /* len: (signed short int) EvtFileLen     */
   /* val: (char FAR	   *) EvtFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "RESind.EvtFile :",
                            argv[1],argl[1]);

   }
   if (argl[2] > 0) {
      sp1255_wrSgLong(RC1202_OFFSET2, "RESind.r_opval.r_oploc   : ",
                      (signed long int FAR *)argv[2],RC1206_DEC_NL ); 
   }
   if (argl[3] > 0 ){
      sp1282_WrObjId (RC1202_OFFSET2,(char FAR *)"RESind.r_opval.r_opglb: ",argv[3],argl[3]);
   }

}/*end*/
        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1317_enErrReq 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1317_enErrReq (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_errqpb   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_errqpb FAR *)argv[0];
      /*sp1252_blockBegin ("RO_errqpb");*/
      sp1330_enHeader ((RO_fncthd FAR *)&(pp->r_hdinfo));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "ERRreq.ASSOCID          : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1255_wrSgLong(RC1202_OFFSET2, "ERRreq.r_invid           : ",
                      (signed long int FAR *)&(pp->r_invid),RC1206_DEC_NL);
      sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"ERRreq.r_absid        : ",
                      (signed int FAR *)&(pp->r_absid) ,RC1206_DEC_NL);

      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"ERRreq.SnData.r_mtype : ",
                         pp->r_sndata.r_mtype);
      sp1258_wrAddr (RC1202_OFFSET2, (char FAR *)"ERRreq.SnData.r_fraref : ",
                      (char   FAR *)(pp->r_sndata.r_fraref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "ERRreq.SnData.r_fralen: ",
                      (unsigned long int FAR *)&(pp->r_sndata.r_fralen),
                       RC1206_DEC_NL);
      /*sp1253_blockEnd ("RO_errqpb");*/
   }

   /*----------------------------------------*/
   /* len: (signed short int) SndFileLen     */
   /* val: (char FAR	   *) SndFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "ERRreq.SndFile :",
                            argv[1],argl[1]);

   }
   if (argl[2] > 0) {
      sp1255_wrSgLong(RC1202_OFFSET2, "ERRreq.r_erval.r_oploc   : ",
                      (signed long int FAR *)argv[2] ,RC1206_DEC_NL); 
   }
   if (argl[3] > 0 ){
      sp1282_WrObjId (RC1202_OFFSET2,(char FAR *)"ERRreq.r_erval.r_opglb: ",argv[3],argl[3]);
   }

}/*end*/
        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1318_reErrInd 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1318_reErrInd (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_erinev   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_erinev FAR *)argv[0];
      /*sp1252_blockBegin ("RO_erinev");*/
      sp1331_reEvHead ((RO_evnthd FAR *)&(pp->r_evnthd));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "ERRind.ASSOCID          : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1255_wrSgLong(RC1202_OFFSET2, "ERRind.r_invid           : ",
                      (signed long int FAR *)&(pp->r_invid),RC1206_DEC_NL);

      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"ERRind.EvData.r_mtype : ",
                         pp->r_evdata.r_mtype);
      sp1258_wrAddr (RC1202_OFFSET2, "ERRind.EvData.r_bufref : ",
                      (char   FAR *)(pp->r_evdata.r_bufref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "ERRind.EvData.r_buflen: ",
                      (unsigned long int FAR *)&(pp->r_evdata.r_buflen),
                       RC1206_DEC_NL);
      /*sp1253_blockEnd ("RO_erinev");*/
   }

   /*----------------------------------------*/
   /* len: (signed short int) EvtFileLen     */
   /* val: (char FAR	   *) EvtFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "ERRind.EvtFile :",
                            argv[1],argl[1]);

   }
   if (argl[2] > 0) {
      sp1255_wrSgLong(RC1202_OFFSET2, "ERRind.r_erval.r_oploc   : ",
                      (signed long int FAR *)argv[2] ,RC1206_DEC_NL); 
   }
   if (argl[3] > 0 ){
      sp1282_WrObjId (RC1202_OFFSET2,(char FAR *)"ERRind.r_erval.r_opglb: ",argv[3],argl[3]);
   }

}/*end*/
        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1319_enURjReq 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1319_enURjReq (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_rurqpb   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_rurqpb FAR *)argv[0];
      /*sp1252_blockBegin ("RO_rurqpb");*/
      sp1330_enHeader ((RO_fncthd FAR *)&(pp->r_hdinfo));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2,
                      (char FAR *)"RJUreq.ASSOCID          : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1255_wrSgLong(RC1202_OFFSET2,
                      (char FAR *)"RJUreq.r_invid          : ",
                      (signed long int FAR *)&(pp->r_invid),RC1206_DEC_NL);
      sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"RJUreq.r_absid        : ",
                      (signed int  FAR *)&(pp->r_absid) ,RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2,             
                      (char FAR *)"RJUreq.r_reason         : ",
                      (unsigned long int FAR *)&(pp->r_reason) , RC1208_HEX_NL);

      /*sp1253_blockEnd ("RO_rurqpb");*/
   }


}/*end*/
        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1320_reURjInd 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1320_reURjInd (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_ruinev   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_ruinev FAR *)argv[0];
      /*sp1252_blockBegin ("RO_ruinev");*/
      sp1331_reEvHead ((RO_evnthd FAR *)&(pp->r_evnthd));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "RJUreq.ASSOCID          : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1255_wrSgLong(RC1202_OFFSET2, "RJUreq.r_invid           : ",
                      (signed long int FAR *)&(pp->r_invid),RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2, "RJUreq.r_reason          : ",
                      (unsigned long int FAR *)&(pp->r_reason) , RC1206_DEC_NL);

      /*sp1253_blockEnd ("RO_ruinev");*/
   }

}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1321_rePRjInd 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1321_rePRjInd (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_rpinev   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_rpinev FAR *)argv[0];
      /*sp1252_blockBegin ("RO_rpinev");*/
      sp1331_reEvHead ((RO_evnthd FAR *)&(pp->r_evnthd));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "RJPind.ASSOCID          : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2, "RJPind.r_reason          : ",
                      (unsigned long int FAR *)&(pp->r_reason) , RC1206_DEC_NL);

      /*sp1253_blockEnd ("RO_rpinev");*/
   }
   if (argl[1] > 0 ){
      sp1255_wrSgLong (RC1202_OFFSET2 , "RJPind.r_invid          : ",
		       (signed long int FAR *)argv[1], RC1206_DEC_NL);
    
   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1322_enUnBindReq                		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1322_enUnBindReq (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_ubrqpb   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_ubrqpb FAR *)argv[0];
      /*sp1252_blockBegin ("RO_ubrqpb");*/
      sp1330_enHeader ((RO_fncthd FAR *)&(pp->r_hdinfo));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "UNBINDreq.ASSOCID       : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);

      /*sp1253_blockEnd ("RO_ubrqpb");*/
   }
   /*----------------------------------------*/
   /* len: (signed short int) SndFileLen     */
   /* val: (char FAR	   *) SndFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "UNBINDreq.SndFile: ",
                            argv[1],argl[1]);

   }
   /*----------------------------------------*/
   /* len: (signed short int) BufFileLen     */
   /* val: (char FAR	   *) BufFileName    */
   /*----------------------------------------*/

   if (argl[2] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "UNBINDreq.BufFile   :",
                            argv[2],argl[2]);

   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1323_reUnBindReq                		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1323_reUnBindReq (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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

}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1324_enUnBindRsp                		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1324_enUnBindRsp (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_ubrspb   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_ubrspb FAR *)argv[0];
      /*sp1252_blockBegin ("RO_ubrspb");*/
      sp1330_enHeader ((RO_fncthd FAR *)&(pp->r_hdinfo));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "UNBINDrsp.ASSOCID       : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1337_writeUreasn (RC1202_OFFSET2, (char FAR *)"UNBINDrsp.r_ureasn       : ",
                          pp->r_ureasn);

      /*sp1253_blockEnd ("RO_ubrspb");*/
   }
   /*----------------------------------------*/
   /* len: (signed short int) SndFileLen     */
   /* val: (char FAR	   *) SndFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "UNBINDrsp.SndFile: ",
                            argv[1],argl[1]);

   }
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1325_reUnBindReq                		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1325_reUnBindReq (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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

}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1326_reUnBindInd                		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1326_reUnBindInd (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_ubinev   FAR * pp = LNULL;
unsigned long int i;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_ubinev FAR *)argv[0];
      /*sp1252_blockBegin ("RO_ubinev");*/
      sp1331_reEvHead ((RO_evnthd FAR *)&(pp->r_evnthd));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "UNBINDind.ASSOCID       : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);

      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"UNBINDind.EvFrag.r_mtype : ",
                         pp->r_evfrag.r_mtype);
      sp1280_writeBool(RC1202_OFFSET2, "UNBINDind.EvFrag.r_specfl : ",
                       pp->r_evfrag.r_specfl);

      sp1258_wrAddr (RC1202_OFFSET2, "UNBINDind.EvFrag.r_bufref : ",
                      (char  FAR *)(pp->r_evfrag.r_bufref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "UNBINDind.EvFrag.r_buflen: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_buflen),
                       RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2, "UNBINDind.EvFrag.r_fracnt: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_fracnt),
                       RC1206_DEC_NL);
      if ( (pp->r_evfrag.r_fracnt < 6) && (pp->r_evfrag.r_bufref))
         for ( i=0;i< pp->r_evfrag.r_fracnt;i++){
           sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"UNBINDind.EvFrag.r_absid : ",
                      (signed int FAR *)&(pp->r_evfrag.r_fraary[i].r_absid) ,
                        RC1206_DEC_NL);
           sp1257_wrUnLong(RC1202_OFFSET2, "UNBINDind.EvFrag.r_sublen: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_fraary[i].r_sublen),
                       RC1206_DEC_NL);
    }
      sp1271_wrLine(); 
      /*sp1253_blockEnd ("RO_ubinev");*/
   }
   /*----------------------------------------*/
   /* len: (signed short int) EvtFileLen     */
   /* val: (char FAR	   *) EvtFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "UNBINDind.EvtFile: ",
                            argv[1],argl[1]);

   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1327_reUnBindCnf                		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1327_reUnBindCnf (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_ubcnev   FAR * pp = LNULL;
unsigned long int i;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_ubcnev FAR *)argv[0];
      /*sp1252_blockBegin ("RO_ubcnev");*/
      sp1331_reEvHead ((RO_evnthd FAR *)&(pp->r_evnthd));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "UNBINDcnf.ASSOCID       : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1337_writeUreasn(RC1202_OFFSET2, (char FAR *)"UNBINDcnf.r_ureasn       : ",
                      pp->r_ureasn);

      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"UNBINDcnf.EvFrag.r_mtype : ",
                         pp->r_evfrag.r_mtype);
      sp1280_writeBool(RC1202_OFFSET2, "UNBINDcnf.EvFrag.r_specfl : ",
                       pp->r_evfrag.r_specfl);

      sp1258_wrAddr (RC1202_OFFSET2, "UNBINDcnf.EvFrag.r_bufref : ",
                      (char  FAR *)(pp->r_evfrag.r_bufref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "UNBINDcnf.EvFrag.r_buflen: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_buflen),
                       RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2, "UNBINDcnf.EvFrag.r_fracnt: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_fracnt),
                       RC1206_DEC_NL);
      if ( (pp->r_evfrag.r_fracnt < 6) && (pp->r_evfrag.r_bufref))
         for ( i=0;i< pp->r_evfrag.r_fracnt;i++){
           sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"UNBINDcnf.EvFrag.r_absid : ",
                      (signed int FAR *)&(pp->r_evfrag.r_fraary[i].r_absid) ,
                        RC1206_DEC_NL);
           sp1257_wrUnLong(RC1202_OFFSET2, "UNBINDcnf.EvFrag.r_sublen: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_fraary[i].r_sublen),
                       RC1206_DEC_NL);
    }
      sp1271_wrLine(); 
      /*sp1253_blockEnd ("RO_ubcnev");*/
   }
   /*----------------------------------------*/
   /* len: (signed short int) EvtFileLen     */
   /* val: (char FAR	   *) EvtFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "UNBINDcnf.EvtFile: ",
                            argv[1],argl[1]);

   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1328_enUAbortReq                		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1328_enUAbortReq (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_abrqpb   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_abrqpb FAR *)argv[0];
      /*sp1252_blockBegin ("RO_abrqpb");*/
      sp1330_enHeader ((RO_fncthd FAR *)&(pp->r_hdinfo));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "UABORTreq.ASSOCID       : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);

      /*sp1253_blockEnd ("RO_abrqpb");*/
   }
   /*----------------------------------------*/
   /* len: (signed short int) SndFileLen     */
   /* val: (char FAR	   *) SndFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "UABORTreq.SndFile: ",
                            argv[1],argl[1]);

   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1329_reUAbortReq                		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1329_reUAbortReq (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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

}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp132a_reUAbortInd                		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp132a_reUAbortInd (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_abinev   FAR * pp = LNULL;
unsigned long int i;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_abinev FAR *)argv[0];
      /*sp1252_blockBegin ("RO_abinev");*/
      sp1331_reEvHead ((RO_evnthd FAR *)&(pp->r_evnthd));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "UABORTind.ASSOCID       : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2, "UABORTind.r_result       : ",
                      (unsigned long int FAR *)&(pp->r_result) , RC1208_HEX_NL);
      sp1290_writeAbortReason(pp->r_result); 
      sp1257_wrUnLong(RC1202_OFFSET2, "UABORTind.r_adinfo       : ",
                      (unsigned long int FAR *)&(pp->r_adinfo) , RC1206_DEC_NL);

      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"UABORTind.EvFrag.r_mtype : ",
                         pp->r_evfrag.r_mtype);
      sp1280_writeBool(RC1202_OFFSET2, "UABORTind.EvFrag.r_specfl : ",
                       pp->r_evfrag.r_specfl);

      sp1258_wrAddr (RC1202_OFFSET2, "UABORTind.EvFrag.r_bufref : ",
                      (char  FAR *)(pp->r_evfrag.r_bufref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "UABORTind.EvFrag.r_buflen: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_buflen),
                       RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2, "UABORTind.EvFrag.r_fracnt: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_fracnt),
                       RC1206_DEC_NL);
      if ( (pp->r_evfrag.r_fracnt < 6) && (pp->r_evfrag.r_bufref))
         for ( i=0;i< pp->r_evfrag.r_fracnt;i++){
           sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"UABORTind.EvFrag.r_absid : ",
                      (signed int FAR *)&(pp->r_evfrag.r_fraary[i].r_absid) ,
                        RC1206_DEC_NL);
           sp1257_wrUnLong(RC1202_OFFSET2, "UABORTind.EvFrag.r_sublen: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_fraary[i].r_sublen),
                       RC1206_DEC_NL);
    }
      sp1271_wrLine(); 
      /*sp1253_blockEnd ("RO_abinev");*/
   }
   /*----------------------------------------*/
   /* len: (signed short int) EvtFileLen     */
   /* val: (char FAR	   *) EvtFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "UABORTind.EvtFile: ",
                            argv[1],argl[1]);

   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp132b_rePAbortInd                		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp132b_rePAbortInd (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_abinev   FAR * pp = LNULL;
unsigned long int i;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_abinev FAR *)argv[0];
      /*sp1252_blockBegin ("RO_abinev");*/
      sp1331_reEvHead ((RO_evnthd FAR *)&(pp->r_evnthd));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "PABORTind.ASSOCID       : ",
                      (unsigned long int FAR *)&(pp->r_assid) , RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2, "PABORTind.r_result       : ",
                      (unsigned long int FAR *)&(pp->r_result) , RC1208_HEX_NL);
      sp1290_writeAbortReason(pp->r_result);
      sp1257_wrUnLong(RC1202_OFFSET2, "PABORTind.r_adinfo       : ",
                      (unsigned long int FAR *)&(pp->r_adinfo) , RC1206_DEC_NL);

      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"PABORTind.EvFrag.r_mtype : ",
                         pp->r_evfrag.r_mtype);
      sp1280_writeBool(RC1202_OFFSET2, "PABORTind.EvFrag.r_specfl : ",
                       pp->r_evfrag.r_specfl);

      sp1258_wrAddr (RC1202_OFFSET2, "PABORTind.EvFrag.r_bufref : ",
                      (char  FAR *)(pp->r_evfrag.r_bufref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "PABORTind.EvFrag.r_buflen: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_buflen),
                       RC1206_DEC_NL);
      sp1257_wrUnLong(RC1202_OFFSET2, "PABORTind.EvFrag.r_fracnt: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_fracnt),
                       RC1206_DEC_NL);
      if ( (pp->r_evfrag.r_fracnt < 6) && (pp->r_evfrag.r_bufref))
         for ( i=0;i< pp->r_evfrag.r_fracnt;i++){
           sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"PABORTind.EvFrag.r_absid : ",
                      (signed int FAR *)&(pp->r_evfrag.r_fraary[i].r_absid) ,
                        RC1206_DEC_NL);
           sp1257_wrUnLong(RC1202_OFFSET2, "PABORTind.EvFrag.r_sublen: ",
                      (unsigned long int FAR *)&(pp->r_evfrag.r_fraary[i].r_sublen),
                       RC1206_DEC_NL);
    }
      sp1271_wrLine(); 
      /*sp1253_blockEnd ("RO_abinev");*/
   }
   /*----------------------------------------*/
   /* len: (signed short int) EvtFileLen     */
   /* val: (char FAR	   *) EvtFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "PABORTind.EvtFile: ",
                            argv[1],argl[1]);

   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp132c_enProvBuf                  		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-09-27                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp132c_enProvBuf   (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_pvbfpb   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_pvbfpb FAR *)argv[0];
      /*sp1252_blockBegin ("RO_pvbfpb");*/
      sp1330_enHeader ((RO_fncthd FAR *)&(pp->r_hdinfo));
      sp1271_wrLine();
      sp1257_wrUnLong(RC1202_OFFSET2, "ProvBuf.APPLID         : ",
                      (unsigned long int FAR *)&(pp->r_aplid) , RC1206_DEC_NL);
      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"ProvBuf.Buffer.r_mtype : ",
                         pp->r_buffer.r_mtype);
      sp1258_wrAddr (RC1202_OFFSET2, "ProvBuf.Buffer.r_bufref : ",
                      (char   FAR *)(pp->r_buffer.r_bufref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "ProvBuf.Buffer.r_buflen: ",
                      (unsigned long int FAR *)&(pp->r_buffer.r_buflen),
                      RC1206_DEC_NL);
      /*sp1253_blockEnd ("RO_pvbfpb");*/
   }
   /*----------------------------------------*/
   /* len: (signed short int) SndFileLen     */
   /* val: (char FAR	   *) SndFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "ProvBuf.BufFile: ",
                            argv[1],argl[1]);

   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp132d_enCwait                                      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-09-27                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp132d_enCwait     (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_chwtpb   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_chwtpb FAR *)argv[0];
      /*sp1252_blockBegin ("RO_cwait");*/
      sp1330_enHeader ((RO_fncthd FAR *)&(pp->r_hdinfo));
      sp1271_wrLine();
      sp1258_wrAddr (RC1202_OFFSET2, "CWait.r_evntrf : ",
                      (char   FAR *)(pp->r_evntrf) );
      sp1257_wrUnLong(RC1202_OFFSET2, "CWait.r_wtime: ",
                      (unsigned long int FAR *)&(pp->r_wtime),
                      RC1206_DEC_NL);
      /*sp1253_blockEnd ("RO_cwait");*/
   }
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp132e_enInform                                     */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-09-27                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp132e_enInform    (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_inform   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_inform FAR *)argv[0];
      /*sp1252_blockBegin ("RO_inform");*/
      sp1330_enHeader ((RO_fncthd FAR *)&(pp->r_hdinfo));
      sp1271_wrLine();
      sp1258_wrAddr (RC1202_OFFSET2, "Inform.r_infval : ",
                      (char   FAR *)(pp->r_infval) );
      sp1254_wrSgInt(RC1202_OFFSET2, (char FAR *)"Inform.r_inftyp: ",
                      (signed int FAR *)&(pp->r_inftyp),
                      RC1208_HEX_NL);
      /*sp1253_blockEnd ("RO_inform");*/
   } 
   if ( argl[1] > 0 ) {
      if (argl[1] == sizeof(RO_gtpbif)){
        RO_gtpbif FAR * pp1 = (RO_gtpbif FAR *)argv[1]; 
        sp1254_wrSgInt(RC1202_OFFSET2, (char FAR *)"Inform.r_gtpbif.r_number: ",
                      (signed int FAR *)&(pp1->r_number),
                      RC1206_DEC_NL);
         
      } else if (argl[1] == sizeof(RO_gtdcif)) {
        RO_gtdcif FAR * pp1 = (RO_gtdcif FAR *)argv[1]; 
        int i=0;
      sp1257_wrUnLong(RC1202_OFFSET2, "Inform.GetDCS.assid: ",
                      (unsigned long int FAR *)&(pp1->r_assid),
                      RC1206_DEC_NL);
      sp1254_wrSgInt(RC1202_OFFSET2, (char FAR *)"Inform.GetDCS.r_pcnum: ",
                      (signed int FAR *)&(pp1->r_pcnum),
                      RC1206_DEC_NL);
      for (i=0;i< pp1->r_pcnum;i++) {
         sp1255_wrSgLong (RC1202_OFFSET2 , "Inform.GetDCS.r_pctid: ",
		       (signed long int FAR *)&(pp1->r_prcxt[i].r_pctid), 
                       RC1206_DEC_NL);
         sp1254_wrSgInt(RC1202_OFFSET2, (char FAR *)"Inform.GetDCS.r_absid: ",
                      (signed  int FAR *)&(pp1->r_prcxt[i].r_absid),
                      RC1206_DEC_NL);
         sp1254_wrSgInt(RC1202_OFFSET2, (char FAR *)"Inform.GetDCS.r_trsid: ",
                      (signed  int FAR *)&(pp1->r_prcxt[i].r_trsid),
                      RC1206_DEC_NL);
         sp1271_wrLine();
      }
    }
  }
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp132f_rnProvBuf                  		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-09-27                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp132f_rnProvBuf   (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_rbinev   FAR * pp = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_rbinev FAR *)argv[0];
      /*sp1252_blockBegin ("RO_rbinev");*/
      sp1331_reEvHead ((RO_evnthd FAR *)&(pp->r_evnthd));
      sp1271_wrLine();
      sp1335_writeMtype(RC1202_OFFSET2,(char FAR *)"Buffind.Buffer.r_mtype : ",
                         pp->r_buffer.r_mtype);
      sp1258_wrAddr (RC1202_OFFSET2, "Buffind.Buffer.r_bufref : ",
                      (char   FAR *)(pp->r_buffer.r_bufref) );
      sp1257_wrUnLong(RC1202_OFFSET2, "Buffind.Buffer.r_buflen: ",
                      (unsigned long int FAR *)&(pp->r_buffer.r_buflen),
                      RC1206_DEC_NL);
      /*sp1253_blockEnd ("RO_rbinev");*/
   }
   /*----------------------------------------*/
   /* len: (signed short int) SndFileLen     */
   /* val: (char FAR	   *) SndFileName    */
   /*----------------------------------------*/

   if (argl[1] > 0) {
      sp1263_hexDump(RC1202_OFFSET2, "Buffind.BufFile: ",
                            argv[1],argl[1]);

   }
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp132g_rnrtrosCl                  		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-10-07                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp132g_rnrtrosCl   (

char FAR * FAR *    argv,
unsigned long int  FAR *argl)

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
RO_fncthd   FAR * pp = LNULL;
char vStr[16]="UNKNOWN";
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

   /*---------------------------------------------*/
   /* len: (signed short int) (*p)                */
   /* val: (char FAR	   *) p         	  */
   /*---------------------------------------------*/

   if (argl[0] > 0) {
      pp = (RO_fncthd FAR *)argv[0];
      sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"Return.Fncthd.r_errval  : ",
                      (signed int FAR *)&(pp->r_errval) ,RC1206_DEC_NL);
      switch(pp->r_erclas){
        case RO_SUCES:
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_SUCES");
           break;       
        case RO_ILVERS:
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_ILVERS");
           break;       
        case RO_NOSPEC:
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_NOSPEC");
           break;       
        case RO_PARERR:
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_PARERR");
           break;       
        case RO_SYSERR:
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_SYSERR");
           break;       
        case RO_OSSERR:
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_OSSERR");
           break;       
        case RO_SYSOSS:
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_SYSOSS");
           break;       
        case RO_CMXERR:
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_CMXERR");
           break;       
        case RO_CHCKEY:
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_CHCKEY");
           break;       
        default:
           sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"Return.Fncthd.r_erclas  : ",
                      (signed int FAR *)&(pp->r_erclas) ,RC1206_DEC_NL);
           break;
      }
      sp1264_write(RC1202_OFFSET2,"%s%s\n","Return.Fncthd.r_erclas  :",vStr,
               NULL,NULL);    
      switch(pp->r_retcod){
        case RO_SUCES: 
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_SUCES");
           break;       
        case RO_NOT1ST: 
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_NOT1ST");
           break;       
        case RO_WARNIG: 
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_WARNIG");
           break;       
        case RO_NOEVNT: 
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_NOEVNT");
           break;       
        case RO_TIMOUT: 
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_TIMOUT");
           break;       
        case RO_CALERR: 
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_CALERR");
           break;       
        case RO_PRMERR: 
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_PRMERR");
           break;       
        case RO_TMPERR: 
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_TMPERR");
           break;       
        default:
           ro0050_strcpy((char FAR *)vStr,(char FAR *)"UNKNOWN");
           sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"Return.Fncthd.r_retcod  : ",
                      (signed int FAR *)&(pp->r_retcod) ,RC1206_DEC_NL);
           break;
      }
      sp1264_write(RC1202_OFFSET2,"%s%s\n","Return.Fncthd.r_retcod  :",vStr,
               NULL,NULL);    

     switch (pp->r_fnctnb){
       case RO_ATACH:
         {
          RO_attapb FAR * p = (RO_attapb FAR *) pp;
          sp1264_write (RC1202_OFFSET2, "Return of RO_ATACH \n\n", NULL, NULL,
		    NULL, NULL);
           
          sp1257_wrUnLong(RC1202_OFFSET2, "Attach.APPLID         : ",
                      (unsigned long int FAR *)&(p->r_aplid),
                      RC1206_DEC_NL);
         }
           break; 
       case RO_DTACH:
         sp1264_write (RC1202_OFFSET2, "Return of RO_DTACH \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_CWAIT:
         sp1264_write (RC1202_OFFSET2, "Return of RO_CWAIT \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_PRBUF:
         sp1264_write (RC1202_OFFSET2, "Return of RO_PRBUF \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_REDRQ:
         sp1264_write (RC1202_OFFSET2, "Return of RO_REDRQ \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_WAKE:
         sp1264_write (RC1202_OFFSET2, "Return of RO_WAKE  \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_INFRM:
         sp1264_write (RC1202_OFFSET2, "Return of RO_INFRM \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_LOGON:
         sp1264_write (RC1202_OFFSET2, "Return of RO_LOGON \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_LOGOF:
         sp1264_write (RC1202_OFFSET2, "Return of RO_LOGOF \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_LOGWR:
         sp1264_write (RC1202_OFFSET2, "Return of RO_LOGWR \n\n", NULL, NULL,
		    NULL, NULL);
           break; 

       case RO_BINRQ:
         sp1264_write (RC1202_OFFSET2, "Return of RO_BINRQ \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_BINRS:
         sp1264_write (RC1202_OFFSET2, "Return of RO_BINRS \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_UBNRQ:
         sp1264_write (RC1202_OFFSET2, "Return of RO_UBNRQ \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_UBNRS:
         sp1264_write (RC1202_OFFSET2, "Return of RO_UBNRS \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_ABORQ:
         sp1264_write (RC1202_OFFSET2, "Return of RO_ABORQ \n\n", NULL, NULL,
		    NULL, NULL);
           break; 

       case RO_INVRQ:
         sp1264_write (RC1202_OFFSET2, "Return of RO_INVRQ \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_RESRQ:
         sp1264_write (RC1202_OFFSET2, "Return of RO_RESRQ \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_ERRRQ:
         sp1264_write (RC1202_OFFSET2, "Return of RO_ERRRQ \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       case RO_RJURQ:
         sp1264_write (RC1202_OFFSET2, "Return of RO_RJURQ \n\n", NULL, NULL,
		    NULL, NULL);
           break; 
       default: 
      sp1264_write (RC1202_OFFSET2, "ERROR IN LOGGING FILE\n\n", NULL, NULL,
		    NULL, NULL);
          break;

     }
      
   }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1330_enHeader   				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

void sp1330_enHeader( 
RO_fncthd  FAR * p)

/*                                                                    */
/* DESCRIPTION  : This prints th eHeader information for the          */
/*                entry functions.                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
 char vStr[16]="UNKNOWM";
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
   
 sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"Version Number        : ",
                      (signed int  FAR *)&(p->r_versnb) ,RC1206_DEC_NL);
     
 switch(p->r_fnctnb){
    case RO_ATACH:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_ATACH");
       break;       
    case RO_DTACH:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_DTACH");
       break;       
    case RO_CWAIT:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_CWAIT");
       break;       
    case RO_PRBUF:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_PRBUF");
       break;       
    case RO_REDRQ:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_REDRQ");
       break;       
    case RO_WAKE:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_WAKE");
       break;       
    case RO_INFRM:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_INFRM");
       break;       
    case RO_LOGON:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_LOGON");
       break;       
    case RO_LOGOF:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_LOGOF");
       break;       
    case RO_LOGWR:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_LOGWR");
       break;       
    case RO_BINRQ:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_BINRQ");
       break;       
    case RO_BINRS:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_BINRS");
       break;       
    case RO_UBNRQ:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_UBNRQ");
       break;       
    case RO_UBNRS:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_UBNRS");
       break;       
    case RO_ABORQ:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_ABORQ");
       break;       
    case RO_INVRQ:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_INVRQ");
       break;       
    case RO_RESRQ:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_RESRQ");
       break;       
    case RO_ERRRQ:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_ERRRQ");
       break;       
    case RO_RJURQ:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_RJURQ");
       break;       
    default: 
      sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"Function Number       : ",
                      (signed int  FAR *)&(p->r_fnctnb) ,RC1206_DEC_NL);
      break;
 }
 sp1264_write(RC1202_OFFSET2,"%s%s\n","Function              : ",vStr,
               NULL,NULL);    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1331_reEvHead 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

void sp1331_reEvHead( 
RO_evnthd FAR * p)

/*                                                                    */
/* DESCRIPTION  : This prints th eHeader information for the          */
/*                entry functions.                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
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
char vStr[16]="UNKNOWN";
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
 switch(p->r_evtype){
    case RO_NOEVET:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_NOEVET");
       break;       
    case RO_TIMEUT:
       ro0050_strcpy((char FAR *)vStr,"RO_TIMEUT");
       break;       
    case RO_RBINEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_RBINEV");
       break;       
    case RO_BNINEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_BNINEV");
       break;       
    case RO_BNCNEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_BNCNEV");
       break;       
    case RO_UBINEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_UBINEV");
       break;       
    case RO_UBCNEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_UBCNEV");
       break;       
    case RO_ABINEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_ABINEV");
       break;       
    case RO_IVINEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_IVINEV");
       break;       
    case RO_RSINEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_RSINEV");
       break;       
    case RO_ERINEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_ERINEV");
       break;       
    case RO_RUINEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_RUINEV");
       break;       
    case RO_RPINEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_RPINEV");
       break;       
    case RO_IVRQEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_IVRQEV");
       break;       
    case RO_RSRQEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_RSRQEV");
       break;       
    case RO_ERRQEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_ERRQEV");
       break;       
    case RO_RURQEV:
       ro0050_strcpy((char FAR *)vStr,(char FAR *)"RO_RURQEV");
       break;       
    default: 
      sp1254_wrSgInt (RC1202_OFFSET2, (char FAR *)"Event Numb            : ",
                      (signed int FAR *)&(p->r_evtype) ,RC1206_DEC_NL);
      break;
 }
 sp1264_write(RC1202_OFFSET2,"%s%s\n","Event                 : ",vStr,
               NULL,NULL);    
}/*end*/               

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1335_writeMtype                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1335_writeMtype   (

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
    case RO_MEMORY:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_MEMORY",NULL,NULL);
      break;
    case RO_FILE:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_FILE",NULL,NULL);
      break;
#ifdef RP0000_MicV0600
    case RO_GLOHAN:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_GLOHAN",NULL,NULL);
      break;
#endif
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)"",(signed int FAR *)&iValue,RC1206_DEC_NL);
      break; 
      
  }

}/*end*/
 

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1333_writeAclass                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1333_writeAclass(

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
    case RO_ACL1:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_ACL1",NULL,NULL);
      break;
    case RO_ACL2:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_ACL2",NULL,NULL);
      break;
    case RO_ACL3:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_ACL3",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)"",(signed int FAR *)&iValue,RC1206_DEC_NL);
      break; 
  }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1334_writeOpclass                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1334_writeOpclass(

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
    case RO_OPC1:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_OPC1",NULL,NULL);
      break;
    case RO_OPC2:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_OPC2",NULL,NULL);
      break;
    case RO_OPC3:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_OPC3",NULL,NULL);
      break;
    case RO_OPC4:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_OPC4",NULL,NULL);
      break;
    case RO_OPC5:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_OPC5",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)"",(signed int FAR *)&iValue,RC1206_DEC_NL);
      break; 
  }
}/*end*/
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1336_writeResult                                 */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1336_writeResult(

int                      iOffset,
char                FAR *pStr,
unsigned long  int       iValue)
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
    case RO_ACCEPT:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_ACCEPT",NULL,NULL);
      break;
    case RO_UREJCT:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_UREJCT | ReasonNotSpecfied",NULL,NULL);
      break;
    case (RO_UREJCT | RO_NoReason):
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_UREJCT | RO_NoReason",NULL,NULL);
      break;
    case (RO_UREJCT | RO_UnsupApplCtxt):
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_UREJCT | RO_UnsupApplCtxt",NULL,NULL);
      break;
    case (RO_UREJCT | RO_UnrecClgApTitle):
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_UREJCT | RO_UnrecClgApTitle",NULL,NULL);
      break;
    case (RO_UREJCT | RO_UnrecClgApInvocId):
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_UREJCT | RO_UnrecClgApInvocId",NULL,NULL);
      break;
    case (RO_UREJCT | RO_UnrecClgAeQual):
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_UREJCT | RO_UnrecClgAeQual",NULL,NULL);
      break;
    case (RO_UREJCT | RO_UnrecClgAeInvocId):
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_UREJCT | RO_UnrecClgAeInvocId",NULL,NULL);
      break;
    case (RO_UREJCT | RO_UnrecCldApTitle):
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_UREJCT | RO_UnrecCldApTitle",NULL,NULL);
      break;
    case (RO_UREJCT | RO_UnrecCldApInvocId):
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_UREJCT | RO_UnrecCldApInvocId",NULL,NULL);
      break;
    case (RO_UREJCT | RO_UnrecCldAeQual):
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_UREJCT | RO_UnrecCldAeQual",NULL,NULL);
      break;
    case (RO_UREJCT | RO_UnrecCldAeInvocId):
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_UREJCT | RO_UnrecCldAeInvocId",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)"",(signed int FAR *)&iValue,RC1206_DEC_NL);
      break; 
  }
}/*end*/
 


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1337_writeUreasn                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1337_writeUreasn (

int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue)
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
  switch(ulValue){
    case RO_NotDefined:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_NotDefined",NULL,NULL);
      break;
    case RO_NotFinished:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_NotFinished",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)"",(signed int FAR *)&ulValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/
 
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1338_writeReason                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

void sp1338_writeReason (
int                        iOffset,
char                  FAR *pStr,
unsigned long int          ulValue)

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
  switch(ulValue){
    case RO_RINDU:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RINDU",NULL,NULL);
      break;
    case RO_RINUO:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RINUO",NULL,NULL);
      break;
    case RO_RINMA:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RINMA",NULL,NULL);
      break;
    case RO_RINRL:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RINRL",NULL,NULL);
      break;
    case RO_RINIR:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RINIR",NULL,NULL);
      break;
    case RO_RINUL:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RINUL",NULL,NULL);
      break;
    case RO_RINLR:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RINLR",NULL,NULL);
      break;
    case RO_RINUC:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RINUC",NULL,NULL);
      break;
    case RO_RREUI:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RREUI",NULL,NULL);
      break;
    case RO_RRERU:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RRERU",NULL,NULL);
      break;
    case RO_RREMR:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RREMR",NULL,NULL);
      break;
    case RO_RERUI:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RERUI",NULL,NULL);
      break;
    case RO_RERRU:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RERRU",NULL,NULL);
      break;
    case RO_RERCE:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RERCE",NULL,NULL);
      break;
    case RO_RERXE:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RERXE",NULL,NULL);
      break;
    case RO_RERMP:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RERMP",NULL,NULL);
      break;
    case RO_RPOUA:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RPOUA",NULL,NULL);
      break;
    case RO_RPOMA:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RPOMA",NULL,NULL);
      break;
    case RO_RPOBA:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_RPOBA",NULL,NULL);
      break;
    case RO_NTRAN:
      sp1264_write(iOffset,"%s%s\n",pStr,"RO_NTRAN",NULL,NULL);
      break;
    default:
      sp1264_write(iOffset,"%s%s",pStr,"UNKNOWN/",NULL,NULL);
      sp1254_wrSgInt(RC1200_OFFSET0,(char FAR *)"",(signed int FAR *)&ulValue,RC1208_HEX_NL);
      break; 
  }
}/*end*/
 

/*EOF*/
