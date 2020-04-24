/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropzz.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:05  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:44  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:22  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:22:15  keutel
 * 	OT 11620
 * 	[1994/08/19  13:51:32  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:42  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:09  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:22  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:06:47  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:40:01  keutel
 * 	creation
 * 	[1994/03/21  13:35:33  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roropzz.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:05 $" } ;
#endif


#ifdef THREADSAFE
#include <pthread.h>
#endif
#include "rogenrl.h"   /* K02 */
#include "rouply0.h" 
#include "rouply1.h" 
#include "roppm01.h" 
#include "roppm05.h" 

#include "roropzz.h"
 
/* SCCS ID inserted with sed                              */ 
 
static char *sccsid =    "@(#) roropzz.c 1.1 93/11/29   RTS-V2.00"; 



 

int  PASCAL ROSEapdus_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0715_unRoseApdus FAR * x,
asn1_pvoid pContext)
  {
  int sterr = ro0063_sterror(hdl);  /* sterr 21 */
  unsigned char  matchz[6]; /* 21 */
 
    {
    int asn1_choice_ok=1;
    if ( sterr != OK )
      asn1_choice_ok = 0;
    else
      {
      /* matching choice, use matchz */
      asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
      if(((*matchz)|32)==161 )
	{
	sterr=ROIVapdu_dec(hdl,poszm,&(*x).stROIVapdu,pContext);
	(*x).stPdvHeader.lType = RC071P_ROIVapdu;
	}
      else
	if(((*matchz)|32)==162 )
	  {
	  sterr=RORSapdu_dec(hdl,poszm,&(*x).stRORSapdu,pContext);
	  (*x).stPdvHeader.lType = RC071Q_RORSapdu;
	  }
	else
	  if(((*matchz)|32)==163 )
	    {
	    sterr=ROERapdu_dec(hdl,poszm,&(*x).stROERapdu,pContext);
	    (*x).stPdvHeader.lType = RC071R_ROERapdu;
	    }
	  else
	    if(((*matchz)|32)==164 )
	      {
	      sterr=RORJapdu_dec(hdl,poszm,&(*x).stRORJapdu,pContext);
	      (*x).stPdvHeader.lType = RC071S_RORJapdu;
	      }
	    else
	      asn1_choice_ok = 0;
      }
    if (asn1_choice_ok == 0)
      {
      sterr = asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      }
    }
  return(sterr) /* return 21 */;
  }






int  PASCAL ROSEapdus_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0715_unRoseApdus FAR * x,
asn1_pvoid pContext)
  {
  switch (CAST_FOR_SWITCH((*x).stPdvHeader.lType))
    {
    case RC071P_ROIVapdu:
      ROIVapdu_cod(hdl,2,1,&(*x).stROIVapdu,pContext);
      break;
    case RC071Q_RORSapdu:
      RORSapdu_cod(hdl,2,2,&(*x).stRORSapdu,pContext);
      break;
    case RC071R_ROERapdu:
      ROERapdu_cod(hdl,2,3,&(*x).stROERapdu,pContext);
      break;
    case RC071S_RORJapdu:
      RORJapdu_cod(hdl,2,4,&(*x).stRORJapdu,pContext);
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  return(ro0063_sterror(hdl));
  }






int  PASCAL ROIVapdu_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0710_stROIVapdu FAR * x,
asn1_pvoid pContext)
  {
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==2 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&(*x).lInvokeID);
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==128 )
    {
    (*x).bLinkedID = 1;
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&(*x).lLinkedID);
    }
  else
    {
    (*x).bLinkedID = 0;
    }
 
    {/* Tag more than one byte or special type */
    asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
    if( (sterr == OK) && *matchz && OpType_match(matchz))
      {
      sterr=OpType_dec(hdl,poszm,&(*x).stOperationValue);
      }
    else
      {
      sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
      }
 
    }/* End use matchz */
 
    {/* Tag more than one byte or special type */
    asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
/*****                                                                          
        Baiju / kc Modified to accept binary data in BER negotiated             
        Abs Syn . Check on matahz == 0 is removed to allow binary data.         
                                                Mon Apr 25 15:56:53 MDT 1994    
    if( (sterr == OK) && *matchz && RTS_anydef_match(matchz) )                  
*****/                                                                          
    if( (sterr == OK) && RTS_anydef_match(matchz) &&                            
        (poszm !=       /* **skh */                                             
  (unsigned long int)( ro0076_stwhere(hdl) + ro042b_indefDecget(pContext) * 2) )
      )                                                                         
      {
      (*x).bArgument = 1;
      sterr=RTS_anydef_dec(hdl,poszm,&(*x).hArgument,&(*x).stOperationValue,3,
      pContext);
      }
    else
      {
      (*x).bArgument = 0;
      }
 
    }/* End use matchz */
  return(sterr);
  }






int  PASCAL ROIVapdu_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0710_stROIVapdu FAR * x,
asn1_pvoid pContext)
  {
  int sterr = ro0063_sterror(hdl); /* sterr 8 */
 
  if (sterr == OK) /* sterr 2*/
    sterr = asn1st_skiptype(hdl,poszm);
 
    {
    long posz2;
    long poszms = poszm;
    sterr = asn1st_length(hdl,poszm,&posz2);
    if (posz2)
      poszm = posz2;
    else     /* Added by Baiju / Amit on 6-4-94 */ 
      ro042c_indefDecinc(pContext);                
    sterr = ROIVapdu_dseq(hdl,poszm,x,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
  }







int  PASCAL ROIVapdu_ccod(
STOREHANDLE hdl,
rT0710_stROIVapdu FAR * x,
asn1_pvoid pContext)
  {
  ro0067_stputc(hdl,(unsigned char)2);
  asn1st_intcod(hdl,(*x).lInvokeID);
  if ((*x).bLinkedID != 0)
    {
    ro0067_stputc(hdl,(unsigned char)128);
    asn1st_intcod(hdl,(*x).lLinkedID);
    }
  OpType_cod(hdl,-1,0,&(*x).stOperationValue);
  if ((*x).bArgument != 0)
    {
    RTS_anydef_cod(hdl,-1,0,&(*x).hArgument,&(*x).stOperationValue,1,pContext);
    }
  return(ro0063_sterror(hdl));
  }/* sterr ret 5 */





int  PASCAL ROIVapdu_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0710_stROIVapdu FAR * x,
asn1_pvoid pContext)
  {
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)48);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    ROIVapdu_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
  }






int  PASCAL RORSapdu_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0712_stRORSapdu FAR * x,
asn1_pvoid pContext)
  {
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==2 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&(*x).lInvokeID);
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==48 )
    {
    (*x).bOperationResult = 1;
    sterr=RORSapdu_stOperationResult_dec(hdl,poszm,&(*x).stOperationResult,
    pContext);
    }
  else
    {
    (*x).bOperationResult = 0;
    }
  return(sterr);
  }






int  PASCAL RORSapdu_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0712_stRORSapdu FAR * x,
asn1_pvoid pContext)
  {
  int sterr = ro0063_sterror(hdl); /* sterr 8 */
 
  if (sterr == OK) /* sterr 2*/
    sterr = asn1st_skiptype(hdl,poszm);
 
    {
    long posz2;
    long poszms = poszm;
    sterr = asn1st_length(hdl,poszm,&posz2);
    if (posz2)
      poszm = posz2;
    else     /* Added by Baiju / Amit on 6-4-94 */
      ro042c_indefDecinc(pContext);               
    sterr = RORSapdu_dseq(hdl,poszm,x,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
  }







int  PASCAL RORSapdu_ccod(
STOREHANDLE hdl,
rT0712_stRORSapdu FAR * x,
asn1_pvoid pContext)
  {
  ro0067_stputc(hdl,(unsigned char)2);
  asn1st_intcod(hdl,(*x).lInvokeID);
  if ((*x).bOperationResult != 0)
    {
    RORSapdu_stOperationResult_cod(hdl,0,16,&(*x).stOperationResult,pContext);
    }
  return(ro0063_sterror(hdl));
  }/* sterr ret 5 */







int  PASCAL RORSapdu_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0712_stRORSapdu FAR * x,
asn1_pvoid pContext)
  {
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)48);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    RORSapdu_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
  }








int  PASCAL RORSapdu_stOperationResult_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0711_stOperationResult FAR * x,
asn1_pvoid pContext)
  {
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
 
    {/* Tag more than one byte or special type */
    asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
    if( (sterr == OK) && *matchz && OpType_match(matchz))
      {
      sterr=OpType_dec(hdl,poszm,&(*x).stOperationValue);
      }
    else
      {
      sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
      }
 
    }/* End use matchz */
 
    {/* Tag more than one byte or special type */
    asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
/*****                                                                          
        Baiju / kc Modified to accept binary data in BER negotiated             
        Abs Syn . Check on matahz == 0 is removed to allow binary data.         
                                                Mon Apr 25 15:56:53 MDT 1994    
    if( (sterr == OK) && *matchz && RTS_anydef_match(matchz) )                  
*****/                                                                          
    if( (sterr == OK) && RTS_anydef_match(matchz) &&                            
        (poszm !=   /* **skh */                                                 
    (unsigned long int)( ro0076_stwhere(hdl) + ro042b_indefDecget(pContext) * 2)
 )                                                                              
      )                                                                         
      {
      sterr=RTS_anydef_dec(hdl,poszm,&(*x).hResult,&(*x).stOperationValue,4,
      pContext);
      }
    else
      {
      sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
      }
 
    }/* End use matchz */
  return(sterr);
  }









int  PASCAL RORSapdu_stOperationResult_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0711_stOperationResult FAR * x,
asn1_pvoid pContext)
  {
  int sterr = ro0063_sterror(hdl); /* sterr 8 */
 
  if (sterr == OK) /* sterr 2*/
    sterr = asn1st_skiptype(hdl,poszm);
 
    {
    long posz2;
    long poszms = poszm;
    sterr = asn1st_length(hdl,poszm,&posz2);
    if (posz2)
      poszm = posz2;
    else     /* Added by Baiju / Amit on 6-4-94 */  
      ro042c_indefDecinc(pContext);                 
    sterr = RORSapdu_stOperationResult_dseq(hdl,poszm,x,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
  }







int  PASCAL RORSapdu_stOperationResult_ccod(
STOREHANDLE hdl,
rT0711_stOperationResult FAR * x,
asn1_pvoid pContext)
  {
  OpType_cod(hdl,-1,0,&(*x).stOperationValue);
  RTS_anydef_cod(hdl,-1,0,&(*x).hResult,&(*x).stOperationValue,2,pContext);
  return(ro0063_sterror(hdl));
  }/* sterr ret 5 */







int  PASCAL RORSapdu_stOperationResult_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0711_stOperationResult FAR * x,
asn1_pvoid pContext)
  {
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)48);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    RORSapdu_stOperationResult_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
  }









int  PASCAL ROERapdu_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0713_stROERapdu FAR * x,
asn1_pvoid pContext)
  {
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==2 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&(*x).lInvokeID);
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
 
    {/* Tag more than one byte or special type */
    asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
    if( (sterr == OK) && *matchz && ErrType_match(matchz))
      {
      sterr=ErrType_dec(hdl,poszm,&(*x).stErrorValue);
      }
    else
      {
      sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
      }
 
    }/* End use matchz */
 
    {/* Tag more than one byte or special type */
    asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
/*****                                                                          
        Baiju / kc Modified to accept binary data in BER negotiated             
        Abs Syn . Check on matahz == 0 is removed to allow binary data.         
                                                Mon Apr 25 15:56:53 MDT 1994    
    if( (sterr == OK) && *matchz && RTS_anydef_match(matchz) )                  
*****/                                                                          
    if( (sterr == OK) && RTS_anydef_match(matchz) &&                            
        (poszm !=    /* ** skh */                                               
  (unsigned long int)( ro0076_stwhere(hdl) + ro042b_indefDecget(pContext) * 2) )
      )                                                                         
      {
      (*x).bParameter = 1;
      sterr=RTS_anydef_dec(hdl,poszm,&(*x).hParameter,&(*x).stErrorValue,5,
      pContext);
      }
    else
      {
      (*x).bParameter = 0;
      }
 
    }/* End use matchz */
  return(sterr);
  }






int  PASCAL ROERapdu_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0713_stROERapdu FAR * x,
asn1_pvoid pContext)
  {
  int sterr = ro0063_sterror(hdl); /* sterr 8 */
 
  if (sterr == OK) /* sterr 2*/
    sterr = asn1st_skiptype(hdl,poszm);
 
    {
    long posz2;
    long poszms = poszm;
    sterr = asn1st_length(hdl,poszm,&posz2);
    if (posz2)
      poszm = posz2;
    else     /* Added by Baiju / Amit on 6-4-94 */
      ro042c_indefDecinc(pContext);               
    sterr = ROERapdu_dseq(hdl,poszm,x,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
  }






int  PASCAL ROERapdu_ccod(
STOREHANDLE hdl,
rT0713_stROERapdu FAR * x,
asn1_pvoid pContext)
  {
  ro0067_stputc(hdl,(unsigned char)2);
  asn1st_intcod(hdl,(*x).lInvokeID);
  ErrType_cod(hdl,-1,0,&(*x).stErrorValue);
  if ((*x).bParameter != 0)
    {
    RTS_anydef_cod(hdl,-1,0,&(*x).hParameter,&(*x).stErrorValue,3,pContext);
    }
  return(ro0063_sterror(hdl));
  }/* sterr ret 5 */






int  PASCAL ROERapdu_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0713_stROERapdu FAR * x,
asn1_pvoid pContext)
  {
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)48);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    ROERapdu_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
  }








int  PASCAL RORJapdu_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0714_stRORJapdu FAR * x,
asn1_pvoid pContext)
  {
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
 
    {/* Tag more than one byte or special type */
    asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
    if( (sterr == OK) && *matchz &&  (  *matchz==2
      ||  *matchz==5  ) )
      {
      /* matching choice, use matchz */
      asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
      if( *matchz==2 )
	{
	ro0070_stgetc(hdl); /* Skip known tag */
	sterr = asn1st_intdec(hdl,poszm,&(*x).lInvokeID);
	(*x).lInvokeIDChoiceNum = RC071P_InvIdPrest;
	}
      else
	if( *matchz==5 )
	  {
	  ro0070_stgetc(hdl); /* Skip known tag */
	  if ( (ro0076_stwhere(hdl)) >=(signed long) poszm || ro0070_stgetc(hdl)
	     != 0 )
	    {
	    sterr = asn1st_errdecset(hdl,ASN1_ERR_NULL);
	    }
	  (*x).lInvokeIDChoiceNum = RC071Q_NoInvId;
	  }
	else
	  {
	  sterr = asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
	  }
      }
    else
      {
      sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
      }
 
    }/* End use matchz */
 
    {/* Tag more than one byte or special type */
    asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
    if( (sterr == OK) && *matchz &&  (  *matchz==128
      ||  *matchz==129
      ||  *matchz==130
      ||  *matchz==131  ) )
      {
      /* matching choice, use matchz */
      asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
      if( *matchz==128 )
	{
	ro0070_stgetc(hdl); /* Skip known tag */
	sterr = asn1st_intdec(hdl,poszm,&(*x).lProblem);
	(*x).lProblemChoiceNum = RC0712_GeneralProblem;
	}
      else
	if( *matchz==129 )
	  {
	  ro0070_stgetc(hdl); /* Skip known tag */
	  sterr = asn1st_intdec(hdl,poszm,&(*x).lProblem);
	  (*x).lProblemChoiceNum = RC0713_InvokeProblem;
	  }
	else
	  if( *matchz==130 )
	    {
	    ro0070_stgetc(hdl); /* Skip known tag */
	    sterr = asn1st_intdec(hdl,poszm,&(*x).lProblem);
	    (*x).lProblemChoiceNum = RC0714_ReturnResultProblem;
	    }
	  else
	    if( *matchz==131 )
	      {
	      ro0070_stgetc(hdl); /* Skip known tag */
	      sterr = asn1st_intdec(hdl,poszm,&(*x).lProblem);
	      (*x).lProblemChoiceNum = RC0715_ReturnErrorProblem;
	      }
	    else
	      {
	      sterr = asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
	      }
      }
    else
      {
      sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
      }
 
    }/* End use matchz */
  return(sterr);
  }







int  PASCAL RORJapdu_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0714_stRORJapdu FAR * x,
asn1_pvoid pContext)
  {
  int sterr = ro0063_sterror(hdl); /* sterr 8 */
 
  if (sterr == OK) /* sterr 2*/
    sterr = asn1st_skiptype(hdl,poszm);
 
    {
    long posz2;
    long poszms = poszm;
    sterr = asn1st_length(hdl,poszm,&posz2);
    if (posz2)
      poszm = posz2;
    sterr = RORJapdu_dseq(hdl,poszm,x,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
  }





int  PASCAL RORJapdu_ccod(
STOREHANDLE hdl,
rT0714_stRORJapdu FAR * x,
asn1_pvoid pContext)
  {
  switch (CAST_FOR_SWITCH((*x).lInvokeIDChoiceNum))
    {
    case RC071P_InvIdPrest:
      ro0067_stputc(hdl,(unsigned char)2);
      asn1st_intcod(hdl,(*x).lInvokeID);
      break;
    case RC071Q_NoInvId:
      ro0067_stputc(hdl,(unsigned char)5);
      ro0067_stputc(hdl,(unsigned char)0);
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  switch (CAST_FOR_SWITCH((*x).lProblemChoiceNum))
    {
    case RC0712_GeneralProblem:
      ro0067_stputc(hdl,(unsigned char)128);
      asn1st_intcod(hdl,(*x).lProblem);
      break;
    case RC0713_InvokeProblem:
      ro0067_stputc(hdl,(unsigned char)129);
      asn1st_intcod(hdl,(*x).lProblem);
      break;
    case RC0714_ReturnResultProblem:
      ro0067_stputc(hdl,(unsigned char)130);
      asn1st_intcod(hdl,(*x).lProblem);
      break;
    case RC0715_ReturnErrorProblem:
      ro0067_stputc(hdl,(unsigned char)131);
      asn1st_intcod(hdl,(*x).lProblem);
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  return(ro0063_sterror(hdl));
  }/* sterr ret 5 */






int  PASCAL RORJapdu_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0714_stRORJapdu FAR * x,
asn1_pvoid pContext)
  {
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)48);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    RORJapdu_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
  }






int  PASCAL OpType_dec(
STOREHANDLE hdl,
unsigned long poszm,
rTro76_stDefinedBy FAR * x)
  {
  int sterr = ro0063_sterror(hdl);  /* sterr 21 */
  unsigned char  matchz[6]; /* 21 */
 
    {
    int asn1_choice_ok=1;
    if ( sterr != OK )
      asn1_choice_ok = 0;
    else
      {
      /* matching choice, use matchz */
      asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
      if( *matchz==2 )
	{
	ro0070_stgetc(hdl); /* Skip known tag */
	sterr = asn1st_intdec(hdl,poszm,&(*x).unValue.lInteger);
	(*x).lType = RC047a_DefinedByInteger;
	}
      else
	if(((*matchz)|32)==38 )
	  {
	  sterr = asn1st_skiptype(hdl,poszm);
	  sterr = asn1st_oiddec(hdl,poszm,(*x).unValue.oObjectId);
	  (*x).lType = RC047b_DefinedByObjectId;
	  }
	else
	  asn1_choice_ok = 0;
      }
    if (asn1_choice_ok == 0)
      {
      sterr = asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      }
    }
  return(sterr) /* return 21 */;
  }






int  PASCAL OpType_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rTro76_stDefinedBy FAR * x)
  {
  switch (CAST_FOR_SWITCH((*x).lType))
    {
    case RC047a_DefinedByInteger:
      ro0067_stputc(hdl,(unsigned char)2);
      asn1st_intcod(hdl,(*x).unValue.lInteger);
      break;
    case RC047b_DefinedByObjectId:
      ro0067_stputc(hdl,(unsigned char)6);
      asn1st_oidcod(hdl,(*x).unValue.oObjectId);
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  return(ro0063_sterror(hdl));
  }







int  PASCAL ErrType_dec(
STOREHANDLE hdl,
unsigned long poszm,
rTro76_stDefinedBy FAR * x)
  {
  int sterr = ro0063_sterror(hdl);  /* sterr 21 */
  unsigned char  matchz[6]; /* 21 */
 
    {
    int asn1_choice_ok=1;
    if ( sterr != OK )
      asn1_choice_ok = 0;
    else
      {
      /* matching choice, use matchz */
      asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
      if( *matchz==2 )
	{
	ro0070_stgetc(hdl); /* Skip known tag */
	sterr = asn1st_intdec(hdl,poszm,&(*x).unValue.lInteger);
	(*x).lType = RC047a_DefinedByInteger;
	}
      else
	if(((*matchz)|32)==38 )
	  {
	  sterr = asn1st_skiptype(hdl,poszm);
	  sterr = asn1st_oiddec(hdl,poszm,(*x).unValue.oObjectId);
	  (*x).lType = RC047b_DefinedByObjectId;
	  }
	else
	  asn1_choice_ok = 0;
      }
    if (asn1_choice_ok == 0)
      {
      sterr = asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      }
    }
  return(sterr) /* return 21 */;
  }







int  PASCAL ErrType_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rTro76_stDefinedBy FAR * x)
  {
  switch (CAST_FOR_SWITCH((*x).lType))
    {
    case RC047a_DefinedByInteger:
      ro0067_stputc(hdl,(unsigned char)2);
      asn1st_intcod(hdl,(*x).unValue.lInteger);
      break;
    case RC047b_DefinedByObjectId:
      ro0067_stputc(hdl,(unsigned char)6);
      asn1st_oidcod(hdl,(*x).unValue.oObjectId);
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  return(ro0063_sterror(hdl));
  }
