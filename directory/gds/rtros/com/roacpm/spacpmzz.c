/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spacpmzz.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:22  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:45  root]
 * 
 * Revision 1.1.2.4  1994/06/10  14:14:07  annie
 * 	copyright deleted the file
 * 	[1994/06/10  14:13:50  annie]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:48  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:45:51  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:44  keutel
 * 	creation
 * 	[1994/03/21  13:15:27  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spacpmzz.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:34 $" } ;
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include "rogenrl.h"  
#include "rouply0.h"   
#include "rostor0.h"   
#include "roacpm1.h"   
#include "asn1st.h"    
#include "roacpmze.h"   

#include "roacpmzz.h"
 
/* SCCS ID inserted with sed                              */ 
 
static char *sccsid =    "@(#) spacpmzz.c 1.1 93/12/20   RTS-V2.00"; 



 

int  PASCAL ACSE_apdu_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0646_acseApdus FAR * x,
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
      if(AARQ_apdu_match(matchz))
	{
	sterr=AARQ_apdu_dec(hdl,poszm,&(*x).aarqApdu,pContext);
	(*x).stPdvHeader.lType = RC0665_AARQapdu;
	}
      else
	if(((*matchz)|32)==97 )
	  {
	  sterr=AARE_apdu_dec(hdl,poszm,&(*x).aareApdu,pContext);
	  (*x).stPdvHeader.lType = RC0666_AAREapdu;
	  }
	else
	  if(((*matchz)|32)==98 )
	    {
	    sterr=RLRQ_apdu_dec(hdl,poszm,&(*x).rlrqApdu,pContext);
	    (*x).stPdvHeader.lType = RC0667_RLRQapdu;
	    }
	  else
	    if(((*matchz)|32)==99 )
	      {
	      sterr=RLRE_apdu_dec(hdl,poszm,&(*x).rlreApdu,pContext);
	      (*x).stPdvHeader.lType = RC0668_RLREapdu;
	      }
	    else
	      if(((*matchz)|32)==100 )
		{
		sterr=ABRT_apdu_dec(hdl,poszm,&(*x).abrtApdu,pContext);
		(*x).stPdvHeader.lType = RC0669_ABRTapdu;
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




int  PASCAL ACSE_apdu_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0646_acseApdus FAR * x,
asn1_pvoid pContext)
{
  switch (CAST_FOR_SWITCH((*x).stPdvHeader.lType))
    {
    case RC0665_AARQapdu:
      AARQ_apdu_cod(hdl,-1,0,&(*x).aarqApdu,pContext);
      break;
    case RC0666_AAREapdu:
      AARE_apdu_cod(hdl,1,1,&(*x).aareApdu,pContext);
      break;
    case RC0667_RLRQapdu:
      RLRQ_apdu_cod(hdl,1,2,&(*x).rlrqApdu,pContext);
      break;
    case RC0668_RLREapdu:
      RLRE_apdu_cod(hdl,1,3,&(*x).rlreApdu,pContext);
      break;
    case RC0669_ABRTapdu:
      ABRT_apdu_cod(hdl,1,4,&(*x).abrtApdu,pContext);
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  return(ro0063_sterror(hdl));
}





int  PASCAL AARE_apdu_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0642_aareApdu FAR * x,
asn1_pvoid pContext)
{
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==160 )
    {
    sterr = asn1st_flagdec(hdl,poszm,(unsigned long FAR *)&(*x).ulProtoVersion,(
    long FAR *)&(*x).ulProtoVersionLength);
    }
  else
    {
    asn1_flags_cpy(&(*x).ulProtoVersion,(*x).ulProtoVersionLength,
    RC0640_ProtocolVers1 ,/* copy1 */ (*x).ulProtoVersionLength)/* copy 2 */;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==161 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
 
      {
      long posz2;
      long poszms = poszm;
      sterr = asn1st_length(hdl,poszm,&posz2);
      if (posz2)
	poszm = posz2;
      sterr = asn1st_skiptype(hdl,poszm);
      sterr = asn1st_oiddec(hdl,poszm,(*x).vApliCxtName);
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==162 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
 
      {
      long posz2;
      long poszms = poszm;
      sterr = asn1st_length(hdl,poszm,&posz2);
      if (posz2)
	poszm = posz2;
      if (sterr == OK)
	{
	ro0070_stgetc(hdl); /* Skip known tag */
	sterr = asn1st_intdec(hdl,poszm,&(*x).lResult);
	}
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==163 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
 
      {
      long posz2;
      long poszms = poszm;
      sterr = asn1st_length(hdl,poszm,&posz2);
      if (posz2)
	poszm = posz2;
      sterr=Ass_source_diag_dec(hdl,poszm,&(*x).lResultSourceDiag,&(*x).
      uResultSourceDiag);
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==164 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
 
      {
      long posz2;
      long poszms = poszm;
      sterr = asn1st_length(hdl,poszm,&posz2);
      if (posz2)
	poszm = posz2;
      (*x).bRespondApTitle = 1;
      sterr=AP_title_dec(hdl,poszm,&(*x).lRespondApTitle,&(*x).uRespondApTitle);
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bRespondApTitle = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==165 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
 
      {
      long posz2;
      long poszms = poszm;
      sterr = asn1st_length(hdl,poszm,&posz2);
      if (posz2)
	poszm = posz2;
      (*x).bRespondAeQualifier = 1;
      sterr=AE_qualifier_dec(hdl,poszm,&(*x).lRespondAeQualifier,&(*x).
      uRespondAeQualifier);
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bRespondAeQualifier = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==166 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
 
      {
      long posz2;
      long poszms = poszm;
      sterr = asn1st_length(hdl,poszm,&posz2);
      if (posz2)
	poszm = posz2;
      (*x).bRespondApInvocId = 1;
      if (sterr == OK)
	{
	ro0070_stgetc(hdl); /* Skip known tag */
	sterr = asn1st_intdec(hdl,poszm,&(*x).lRespondApInvocId);
	}
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bRespondApInvocId = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==167 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
 
      {
      long posz2;
      long poszms = poszm;
      sterr = asn1st_length(hdl,poszm,&posz2);
      if (posz2)
	poszm = posz2;
      (*x).bRespondAeInvocId = 1;
      if (sterr == OK)
	{
	ro0070_stgetc(hdl); /* Skip known tag */
	sterr = asn1st_intdec(hdl,poszm,&(*x).lRespondAeInvocId);
	}
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bRespondAeInvocId = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==189 )
    {
    (*x).bImplInfo = 1;
    sterr = asn1st_octmov(hdl,poszm,(char FAR *)(*x).vImplInfo,(long FAR *)&(*x)
    .ulImplInfo, (long) RC0639_MaxImplInfo,(int) 1);
    }
  else
    {
    (*x).bImplInfo = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==190 )
    {
    (*x).bUserInfo = 1;
    sterr=Association_info_dec(hdl,poszm,&(*x).ulUserInfo,(*x).vUserInfo,
    pContext);
    }
  else
    {
    (*x).bUserInfo = 0;
    }
  return(sterr);
}





int  PASCAL AARE_apdu_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0642_aareApdu FAR * x,
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
    sterr = AARE_apdu_dseq(hdl,poszm,x,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}






int  PASCAL AARE_apdu_ccod(
STOREHANDLE hdl,
rT0642_aareApdu FAR * x,
asn1_pvoid pContext)
{
  if (asn1_flags_cmp((*x).ulProtoVersion,(*x).ulProtoVersionLength,
    RC0640_ProtocolVers1 ,/* copy1 */ (*x).ulProtoVersionLength)/* copy 3 */ !=
    0)
    {
    ro0067_stputc(hdl,(unsigned char)128);
    asn1st_flagcod(hdl,(long)(*x).ulProtoVersion,(long)(*x).ulProtoVersionLength
    );
    }
  ro0067_stputc(hdl,(unsigned char)161);
 
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    ro0067_stputc(hdl,(unsigned char)6);
    asn1st_oidcod(hdl,(*x).vApliCxtName);
    asn1st_deflencod(hdl, posz0);
    }
  ro0067_stputc(hdl,(unsigned char)162);
 
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    ro0067_stputc(hdl,(unsigned char)2);
    asn1st_intcod(hdl,(*x).lResult);
    asn1st_deflencod(hdl, posz0);
    }
  ro0067_stputc(hdl,(unsigned char)163);
 
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    Ass_source_diag_cod(hdl,-1,0,&(*x).lResultSourceDiag,&(*x).uResultSourceDiag
    );
    asn1st_deflencod(hdl, posz0);
    }
  if ((*x).bRespondApTitle != 0)
    {
    ro0067_stputc(hdl,(unsigned char)164);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      AP_title_cod(hdl,-1,0,&(*x).lRespondApTitle,&(*x).uRespondApTitle);
      asn1st_deflencod(hdl, posz0);
      }
    }
  if ((*x).bRespondAeQualifier != 0)
    {
    ro0067_stputc(hdl,(unsigned char)165);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      AE_qualifier_cod(hdl,-1,0,&(*x).lRespondAeQualifier,&(*x).
      uRespondAeQualifier);
      asn1st_deflencod(hdl, posz0);
      }
    }
  if ((*x).bRespondApInvocId != 0)
    {
    ro0067_stputc(hdl,(unsigned char)166);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      ro0067_stputc(hdl,(unsigned char)2);
      asn1st_intcod(hdl,(*x).lRespondApInvocId);
      asn1st_deflencod(hdl, posz0);
      }
    }
  if ((*x).bRespondAeInvocId != 0)
    {
    ro0067_stputc(hdl,(unsigned char)167);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      ro0067_stputc(hdl,(unsigned char)2);
      asn1st_intcod(hdl,(*x).lRespondAeInvocId);
      asn1st_deflencod(hdl, posz0);
      }
    }
  if ((*x).bImplInfo != 0)
    {
    ro0067_stputc(hdl,(unsigned char)157);
    asn1st_octets_cod(hdl,(*x).vImplInfo,(long)(*x).ulImplInfo);
    }
  if ((*x).bUserInfo != 0)
    {
    Association_info_cod(hdl,2,30,&(*x).ulUserInfo,(*x).vUserInfo,pContext);
    }
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */






int  PASCAL AARE_apdu_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0642_aareApdu FAR * x,
asn1_pvoid pContext)
{
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)97);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    AARE_apdu_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}




int  PASCAL RLRQ_apdu_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0643_rlrqApdu FAR * x,
asn1_pvoid pContext)
{
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==128 )
    {
    (*x).bReason = 1;
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&(*x).lReason);
    }
  else
    {
    (*x).bReason = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==190 )
    {
    (*x).bUserInfo = 1;
    sterr=Association_info_dec(hdl,poszm,&(*x).ulUserInfo,(*x).vUserInfo,
    pContext);
    }
  else
    {
    (*x).bUserInfo = 0;
    }
  return(sterr);
}





int  PASCAL RLRQ_apdu_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0643_rlrqApdu FAR * x,
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
    sterr = RLRQ_apdu_dseq(hdl,poszm,x,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}






int  PASCAL RLRQ_apdu_ccod(
STOREHANDLE hdl,
rT0643_rlrqApdu FAR * x,
asn1_pvoid pContext)
{
  if ((*x).bReason != 0)
    {
    ro0067_stputc(hdl,(unsigned char)128);
    asn1st_intcod(hdl,(*x).lReason);
    }
  if ((*x).bUserInfo != 0)
    {
    Association_info_cod(hdl,2,30,&(*x).ulUserInfo,(*x).vUserInfo,pContext);
    }
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */





int  PASCAL RLRQ_apdu_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0643_rlrqApdu FAR * x,
asn1_pvoid pContext)
{
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)98);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    RLRQ_apdu_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}





int  PASCAL RLRE_apdu_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0644_rlreApdu FAR * x,
asn1_pvoid pContext)
{
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==128 )
    {
    (*x).bReason = 1;
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&(*x).lReason);
    }
  else
    {
    (*x).bReason = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==190 )
    {
    (*x).bUserInfo = 1;
    sterr=Association_info_dec(hdl,poszm,&(*x).ulUserInfo,(*x).vUserInfo,
    pContext);
    }
  else
    {
    (*x).bUserInfo = 0;
    }
  return(sterr);
}





int  PASCAL RLRE_apdu_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0644_rlreApdu FAR * x,
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
    sterr = RLRE_apdu_dseq(hdl,poszm,x,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}






int  PASCAL RLRE_apdu_ccod(
STOREHANDLE hdl,
rT0644_rlreApdu FAR * x,
asn1_pvoid pContext)
{
  if ((*x).bReason != 0)
    {
    ro0067_stputc(hdl,(unsigned char)128);
    asn1st_intcod(hdl,(*x).lReason);
    }
  if ((*x).bUserInfo != 0)
    {
    Association_info_cod(hdl,2,30,&(*x).ulUserInfo,(*x).vUserInfo,pContext);
    }
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */





int  PASCAL RLRE_apdu_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0644_rlreApdu FAR * x,
asn1_pvoid pContext)
{
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)99);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    RLRE_apdu_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}






int  PASCAL ABRT_apdu_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0645_abrtApdu FAR * x,
asn1_pvoid pContext)
{
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==128 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&(*x).lAbortSource);
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==190 )
    {
    (*x).bUserInfo = 1;
    sterr=Association_info_dec(hdl,poszm,&(*x).ulUserInfo,(*x).vUserInfo,
    pContext);
    }
  else
    {
    (*x).bUserInfo = 0;
    }
  return(sterr);
}





int  PASCAL ABRT_apdu_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0645_abrtApdu FAR * x,
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
    sterr = ABRT_apdu_dseq(hdl,poszm,x,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}






int  PASCAL ABRT_apdu_ccod(
STOREHANDLE hdl,
rT0645_abrtApdu FAR * x,
asn1_pvoid pContext)
{
  ro0067_stputc(hdl,(unsigned char)128);
  asn1st_intcod(hdl,(*x).lAbortSource);
  if ((*x).bUserInfo != 0)
    {
    Association_info_cod(hdl,2,30,&(*x).ulUserInfo,(*x).vUserInfo,pContext);
    }
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */




int  PASCAL ABRT_apdu_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0645_abrtApdu FAR * x,
asn1_pvoid pContext)
{
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)100);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    ABRT_apdu_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}





int  PASCAL Association_info_dec(
STOREHANDLE hdl,
unsigned long poszm,
ulong FAR * NumOfExt,
asn1_pulong pNode,
asn1_pvoid pContext)
{
  int sterr = ro0063_sterror(hdl);  /* sterr 21 */
  unsigned char  matchz[6]; /* 21 */
  /* I_count */
  unsigned long I_0;
 
  if (sterr == OK) /* sterr 2*/
    ro0070_stgetc(hdl); /* Skip known tag */
 
    {
    long posz2;
    long poszms = poszm;
    sterr = asn1st_length(hdl,poszm,&posz2);
    if (posz2)
      poszm = posz2;
 
    I_0 = 0;
    while( /* inside set of */ (sterr  == OK ) &&
      ((ro0076_stwhere(hdl) + 1) < (signed long) poszm) &&
      asn1st_getmatchz(hdl,poszm,matchz) )
      {
      if ( RCro0b_MaxExternal <= I_0 )
	{
	sterr = asn1st_errdecset(hdl,ASN1_ERR_SOFSZ);
	break;
	}
      else
	{
	if( RTS_External_match(matchz))
	  {
	 sterr=RTS_External_dec(hdl,poszm,(unsigned long int FAR *)&pNode[I_0],
                                 (void NEAR *)pContext);
	  }
	else
	  {
	  sterr = asn1st_errdecset(hdl,ASN1_ERR_WRSET);
	  break;
	  }
	}
      I_0++;
      }
    *NumOfExt = I_0;
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr) /* return 21 */;
}

int  PASCAL Association_info_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
ulong FAR * NumOfExt,
asn1_pulong pNode,
asn1_pvoid pContext)
{
  /* I_count */
  unsigned long I_0;
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)48);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    for (I_0=0; I_0 < *NumOfExt; I_0++)
      {
      RTS_External_cod(hdl,-1,0,(unsigned long int FAR *)&pNode[I_0],
                       (void NEAR *)pContext);
      }
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}





int  PASCAL Ass_source_diag_dec(
STOREHANDLE hdl,
unsigned long poszm,
ulong FAR * lChoice,
rT063b_uAssociSourceDiag FAR * uResSourceDiag)
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
      if( *matchz==161 )
	{
	ro0070_stgetc(hdl); /* Skip known tag */
 
	  {
	  long posz2;
	  long poszms = poszm;
	  sterr = asn1st_length(hdl,poszm,&posz2);
	  if (posz2)
	    poszm = posz2;
	  if (sterr == OK)
	    {
	    ro0070_stgetc(hdl); /* Skip known tag */
	    sterr = asn1st_intdec(hdl,poszm,&(*uResSourceDiag).lAcseServiceUser)
	    ;
	    }
	  sterr = asn1st_close(hdl,poszm,posz2);
	  poszm = poszms;
	  }
	*lChoice = RC0644_ResultSourceUser;
	}
      else
	if( *matchz==162 )
	  {
	  ro0070_stgetc(hdl); /* Skip known tag */
 
	    {
	    long posz2;
	    long poszms = poszm;
	    sterr = asn1st_length(hdl,poszm,&posz2);
	    if (posz2)
	      poszm = posz2;
	    if (sterr == OK)
	      {
	      ro0070_stgetc(hdl); /* Skip known tag */
	      sterr = asn1st_intdec(hdl,poszm,&(*uResSourceDiag).
	      lAcseServiceProvider);
	      }
	    sterr = asn1st_close(hdl,poszm,posz2);
	    poszm = poszms;
	    }
	  *lChoice = RC0645_ResultSourceProvid;
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





int  PASCAL Ass_source_diag_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
ulong FAR * lChoice,
rT063b_uAssociSourceDiag FAR * uResSourceDiag)
{
  switch (CAST_FOR_SWITCH(*lChoice))
    {
    case RC0644_ResultSourceUser:
      ro0067_stputc(hdl,(unsigned char)161);
 
	{
	long posz0 = ro0076_stwhere(hdl);
	asn1st_deflenpos(hdl);
	ro0067_stputc(hdl,(unsigned char)2);
	asn1st_intcod(hdl,(*uResSourceDiag).lAcseServiceUser);
	asn1st_deflencod(hdl, posz0);
	}
      break;
    case RC0645_ResultSourceProvid:
      ro0067_stputc(hdl,(unsigned char)162);
 
	{
	long posz0 = ro0076_stwhere(hdl);
	asn1st_deflenpos(hdl);
	ro0067_stputc(hdl,(unsigned char)2);
	asn1st_intcod(hdl,(*uResSourceDiag).lAcseServiceProvider);
	asn1st_deflencod(hdl, posz0);
	}
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  return(ro0063_sterror(hdl));
}
