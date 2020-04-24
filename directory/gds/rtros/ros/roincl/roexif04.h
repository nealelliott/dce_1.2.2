/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif04.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:13  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:39:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:54  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:10:12  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:14  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:24  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:02:57  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:48  keutel
 * 	creation
 * 	[1994/03/21  13:32:27  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROEXIF04_H
#define _ROEXIF04_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roexif04_rcsid[] = { "@(#) $RCSfile: roexif04.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:13 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1993                     */
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
/* NAME         : roexif4.h                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* COMPONENT    : Remote Opeartions Service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Function and data definition for roexif01.c         */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roexif04.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-02-05|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/
/* *********************************************************************** */   
/*                                                                         */   
/*                    Defines                                              */   
/*                                                                         */   
/* *********************************************************************** */   
                                                                                
#define RC133a_Free      0    /* frees the given handle into the function  */   
#define RC133b_NotFree   1    /* don't frees the given handle              */   


/* ****************** Function prototyping ************************** */
/* ******************			   ************************** */

signed short int FAR PASCAL ro1310_roexcom PROTO ((                             
unsigned long  int  FAR    *pScbHd,
signed   long  int          lTaskId,
FARPROC                     tpduFct,
RO_excopb           FAR    *pParlst));

char FAR * NEAR PASCAL ro131q_setParameter PROTO((
	char		          FAR *,
	char		    FAR * FAR *,
	signed	 long  int        FAR *,
	char		          FAR *,
	unsigned long  int	       ,
	signed   short int	       ));

signed short int NEAR PASCAL  ro131a_setHeader PROTO((
			   RO_fncthd  FAR    *,
			   signed short  int  ));



signed short int  NEAR PASCAL ro131b_checkApplCxt PROTO(( 
                      RO_apclst           FAR   *));
                      

signed short int NEAR PASCAL ro131e_copyAplTitle PROTO(( 
       unsigned long int  FAR    * ,
       RO_apname          FAR    * ));
                                                                 
signed short int  NEAR PASCAL ro131c_storeApInv PROTO(( 
       RO_apname         FAR   *,                                     
       RO_apclst         FAR   *,
       RO_invcid         FAR   *,                                     
       unsigned long int FAR   *));
                                     
signed short int NEAR PASCAL ro131d_storeAddress PROTO((
		       char               FAR    *,
		       unsigned long int          ,
		       unsigned long int  FAR    *));



signed short int NEAR PASCAL ro131f_storePresInv PROTO(( 
	 RO_apclst          FAR  * ,
	 unsigned long  int FAR  * ));

signed short int   NEAR  PASCAL ro131g_storeUserData PROTO((
      RO_snfrag            FAR * ,
      signed    long  int        ,
      signed    short int        ,
      unsigned  long  int  FAR * ,
      unsigned  long  int        ,
      unsigned  long  int        ,
      unsigned  long  int  FAR * ));

signed short int NEAR PASCAL ro131h_GetAplCxtNameId PROTO ((
              unsigned  long  int             hScb,
              unsigned  long  int             ulAcb,
              rT06z0_stInfrmApplCxtName FAR * pStIfApl,
              signed short  int               sMode));

signed short int   NEAR  PASCAL ro131i_freeUserData PROTO(( 
      unsigned  long  int     ));

signed short int   NEAR  PASCAL ro131j_storeBuffer PROTO((
      RO_buffer            FAR  * ,
      unsigned  long  int  FAR  * ));

signed short int   NEAR  PASCAL ro131k_CpyOpValue  PROTO((
        RO_opval           FAR * ,
        rTro76_stDefinedBy FAR * ,
        signed short  int     ));

signed short int NEAR PASCAL ro131n_retrieveEvFrag PROTO(( 
      RO_evtpb                 FAR *  ,     
      unsigned long  int              ,
      char               FAR * FAR *  ,     
      signed   long  int       FAR *  ,    
      void                     FAR *  ,
      signed   short int              ));

signed	short int NEAR PASCAL ro131l_issueRosEvent PROTO((
       unsigned long  int	    ,
       unsigned long  int	    ,
       signed	long  int	    ,
       unsigned long  int	    ,
       unsigned long  int	    ,
       unsigned long  int	    ,
       unsigned long  int	    ,
       RO_evtpb		  FAR	*   ));


signed short int NEAR PASCAL ro131m_retrieveAddress PROTO ((
      RO_evtpb                         FAR *  punEventBuf      ,
      unsigned  long    int                   hHandle          ,
      char                       FAR * FAR *  pUnUsedBufRef    ,
      signed  long      int            FAR *  pUnUsedLen       ,
      RO_pradd                         FAR *  pPrAddr          ));


signed  short  int  NEAR PASCAL ro131o_retrieveOpValue PROTO((
      RO_evtpb                         FAR  *  ,     
      rTro76_stDefinedBy               FAR  *  ,
      char                       FAR * FAR  *  ,     
      signed    long    int            FAR  *  ,    
      RO_opval                         FAR  *  ));   

signed short int NEAR PASCAL ro131p_retrieveAeTitle PROTO((
      RO_evtpb                    FAR *  ,     
      unsigned  long    int              ,     
      char                  FAR * FAR *  ,     
      signed    long    int       FAR *  ,    
      RO_apname                   FAR *  ));    



signed short int NEAR PASCAL ro131r_GetAscCf PROTO(( 
	 RO_apclst          FAR  * ,
         unsigned long int  FAR  * ,
         signed        int  FAR  * ));

signed short int NEAR PASCAL ro131s_CheckLegalOcur PROTO((
     unsigned   long  int         ,
     unsigned   long  int         ,
       signed   long  int         ,
       signed   short int   FAR * ));
 

signed short int NEAR PASCAL ro131t_GetAbsSynRslt PROTO ((
          unsigned  long  int          ,
          unsigned  long  int          ,
          rTro65_pfvAbstrSynResult     ));

signed short int NEAR PASCAL ro131u_findAbsId PROTO ((
          unsigned  long  int          ,
          signed    long  int          ,
          signed    short int         ));

signed short int NEAR PASCAL ro131v_getMode PROTO (( 
unsigned long int      hScb,
unsigned long int      hAcb,
signed        int FAR *pMode));

signed short int NEAR PASCAL ro131w_ChkObjId PROTO ((
unsigned long int  FAR   * pObjId)); 

signed short int NEAR PASCAL ro131x_ChkAEtitl PROTO ((
unsigned long int      hScb,                                              
signed   long int      lAplId,                                            
RO_apname         FAR *pApName)); 

signed short int NEAR PASCAL ro131y_ChkInAEtitl  PROTO (( 
RO_apname         FAR *pApName));  
                              

signed short int NEAR PASCAL ro1312_detach PROTO((
       unsigned long  int  	        ,
       RO_detapb 	   FAR	       *));

signed short int NEAR PASCAL ro131z_CpyRDN  PROTO (( 
RO_rdname         FAR * pRDName,                                    
rTro69_stRDN      FAR * pstRDN));                                     

signed short int NEAR PASCAL ro132a_CpyAVA  PROTO ((
RO_avastr         FAR * pAVAval,                                       
rTro68_stAVA      FAR * pstAVA));                                        

signed short int NEAR PASCAL ro132b_retrieveRDN    PROTO (( 
      RO_rdname                        FAR  *  pRDName,                   
      rTro69_stRDN                     FAR  *  pstRDN,                    
      RO_evtpb                         FAR  *  punEventBuf      ,         
      char                       FAR * FAR  *  pUnUsedBufRef    ,         
      signed  long    int              FAR  *  pUnUsedLen       ));         
                                                                          
signed short int NEAR PASCAL ro132c_retrieveAVA  PROTO (( 
      RO_avastr                        FAR  *  pAVAval,                
      rTro68_stAVA                     FAR  *  pstAVA,                 
      RO_evtpb                         FAR  *  punEventBuf      ,      
      char                       FAR * FAR  *  pUnUsedBufRef    ,      
      signed  long    int              FAR  *  pUnUsedLen       ));      

                                                                       
signed short int NEAR PASCAL ro132d_reterieveInvocId   PROTO (( 
RO_invcid                        FAR * pInvocIds,
rTro74_stInvoc                   FAR * pstInvoc,
RO_evtpb                         FAR  *  punEventBuf      ,                     
char                       FAR * FAR  *  pUnUsedBufRef    ,                     
signed  long    int              FAR  *  pUnUsedLen       ));                     
signed short int NEAR PASCAL ro132e_ChkSetofAVAs PROTO ((
                     signed long  int       lAVA1,         
                     RO_avastr        FAR * vAVA1,         
                     signed long  int       lAVA2,         
                     rTro68_stAVA     FAR * vAVA2 )) ;       
                                                           
#endif	/* _ROEXIF04_H */
