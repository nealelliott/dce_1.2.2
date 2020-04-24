/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spacpmze.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:16  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:42  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:47:34  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:53  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:46  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:45:38  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:38  keutel
 * 	creation
 * 	[1994/03/21  13:15:13  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spacpmze.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:29 $" } ;
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include "rogenrl.h"  
#include "rouply0.h"   
#include "rostor0.h"   
#include "roacpm1.h"   
#include "asn1st.h"    
#include "roacpmzz.h"  

#include "roacpmze.h"
 
/* SCCS ID inserted with sed                              */ 
 
static char *sccsid =    "@(#) spacpmze.c 1.1 93/12/20   RTS-V2.00"; 




 

int  PASCAL AARQ_apdu_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0641_aarqApdu FAR * x,
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
      (*x).bCalledApTitle = 1;
      sterr=AP_title_dec(hdl,poszm,&(*x).lCalledApTitle,&(*x).uCalledApTitle);
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bCalledApTitle = 0;
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
      (*x).bCalledAeQualifier = 1;
      sterr=AE_qualifier_dec(hdl,poszm,&(*x).lCalledAeQualifier,&(*x).
      uCalledAeQualifier);
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bCalledAeQualifier = 0;
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
      (*x).bCalledApInvocId = 1;
      if (sterr == OK)
	{
	ro0070_stgetc(hdl); /* Skip known tag */
	sterr = asn1st_intdec(hdl,poszm,&(*x).lCalledApInvocId);
	}
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bCalledApInvocId = 0;
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
      (*x).bCalledAeInvocId = 1;
      if (sterr == OK)
	{
	ro0070_stgetc(hdl); /* Skip known tag */
	sterr = asn1st_intdec(hdl,poszm,&(*x).lCalledAeInvocId);
	}
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bCalledAeInvocId = 0;
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
      (*x).bCallingApTitle = 1;
      sterr=AP_title_dec(hdl,poszm,&(*x).lCallingApTitle,&(*x).uCallingApTitle);
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bCallingApTitle = 0;
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
      (*x).bCallingAeQualifier = 1;
      sterr=AE_qualifier_dec(hdl,poszm,&(*x).lCallingAeQualifier,&(*x).
      uCallingAeQualifier);
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bCallingAeQualifier = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==168 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
 
      {
      long posz2;
      long poszms = poszm;
      sterr = asn1st_length(hdl,poszm,&posz2);
      if (posz2)
	poszm = posz2;
      (*x).bCallingApInvocId = 1;
      if (sterr == OK)
	{
	ro0070_stgetc(hdl); /* Skip known tag */
	sterr = asn1st_intdec(hdl,poszm,&(*x).lCallingApInvocId);
	}
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bCallingApInvocId = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==169 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
 
      {
      long posz2;
      long poszms = poszm;
      sterr = asn1st_length(hdl,poszm,&posz2);
      if (posz2)
	poszm = posz2;
      (*x).bCallingAeInvocId = 1;
      if (sterr == OK)
	{
	ro0070_stgetc(hdl); /* Skip known tag */
	sterr = asn1st_intdec(hdl,poszm,&(*x).lCallingAeInvocId);
	}
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bCallingAeInvocId = 0;
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





int  PASCAL AARQ_apdu_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0641_aarqApdu FAR * x,
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
    sterr = AARQ_apdu_dseq(hdl,poszm,x,pContext);
    /* Skipping of not known elements for extensibility  */
    sterr = asn1st_skipclose(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}






int  PASCAL AARQ_apdu_ccod(
STOREHANDLE hdl,
rT0641_aarqApdu FAR * x,
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
  if ((*x).bCalledApTitle != 0)
    {
    ro0067_stputc(hdl,(unsigned char)162);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      AP_title_cod(hdl,-1,0,&(*x).lCalledApTitle,&(*x).uCalledApTitle);
      asn1st_deflencod(hdl, posz0);
      }
    }
  if ((*x).bCalledAeQualifier != 0)
    {
    ro0067_stputc(hdl,(unsigned char)163);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      AE_qualifier_cod(hdl,-1,0,&(*x).lCalledAeQualifier,&(*x).
      uCalledAeQualifier);
      asn1st_deflencod(hdl, posz0);
      }
    }
  if ((*x).bCalledApInvocId != 0)
    {
    ro0067_stputc(hdl,(unsigned char)164);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      ro0067_stputc(hdl,(unsigned char)2);
      asn1st_intcod(hdl,(*x).lCalledApInvocId);
      asn1st_deflencod(hdl, posz0);
      }
    }
  if ((*x).bCalledAeInvocId != 0)
    {
    ro0067_stputc(hdl,(unsigned char)165);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      ro0067_stputc(hdl,(unsigned char)2);
      asn1st_intcod(hdl,(*x).lCalledAeInvocId);
      asn1st_deflencod(hdl, posz0);
      }
    }
  if ((*x).bCallingApTitle != 0)
    {
    ro0067_stputc(hdl,(unsigned char)166);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      AP_title_cod(hdl,-1,0,&(*x).lCallingApTitle,&(*x).uCallingApTitle);
      asn1st_deflencod(hdl, posz0);
      }
    }
  if ((*x).bCallingAeQualifier != 0)
    {
    ro0067_stputc(hdl,(unsigned char)167);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      AE_qualifier_cod(hdl,-1,0,&(*x).lCallingAeQualifier,&(*x).
      uCallingAeQualifier);
      asn1st_deflencod(hdl, posz0);
      }
    }
  if ((*x).bCallingApInvocId != 0)
    {
    ro0067_stputc(hdl,(unsigned char)168);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      ro0067_stputc(hdl,(unsigned char)2);
      asn1st_intcod(hdl,(*x).lCallingApInvocId);
      asn1st_deflencod(hdl, posz0);
      }
    }
  if ((*x).bCallingAeInvocId != 0)
    {
    ro0067_stputc(hdl,(unsigned char)169);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      ro0067_stputc(hdl,(unsigned char)2);
      asn1st_intcod(hdl,(*x).lCallingAeInvocId);
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





int  PASCAL AARQ_apdu_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0641_aarqApdu FAR * x,
asn1_pvoid pContext)
{
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)96);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    AARQ_apdu_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}





int  PASCAL AP_title_dec(
STOREHANDLE hdl,
unsigned long poszm,
long FAR * lChoice,
rT0635_uApTitle FAR * ApTitle)
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
      if(Name_match(matchz))
	{
	sterr=Name_dec(hdl,poszm,&(*ApTitle).stApTitleForm1);
	*lChoice = 1;
	}
      else
	if(((*matchz)|32)==38 )
	  {
	  sterr = asn1st_skiptype(hdl,poszm);
	  sterr = asn1st_oiddec(hdl,poszm,(*ApTitle).vApTitleForm2);
	  *lChoice = 2;
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




int  PASCAL AP_title_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
long FAR * lChoice,
rT0635_uApTitle FAR * ApTitle)
{
  switch (CAST_FOR_SWITCH(*lChoice))
    {
    case 1:
      Name_cod(hdl,-1,0,&(*ApTitle).stApTitleForm1);
      break;
    case 2:
      ro0067_stputc(hdl,(unsigned char)6);
      asn1st_oidcod(hdl,(*ApTitle).vApTitleForm2);
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  return(ro0063_sterror(hdl));
}





int  PASCAL AE_qualifier_dec(
STOREHANDLE hdl,
unsigned long poszm,
long FAR * lChoice,
rT0637_uAeQualifier FAR * AeQual)
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
      if(((*matchz)|32)==49 )
	{
	sterr=RelDisName_dec(hdl,poszm,&(*AeQual).stAeQualifierForm1);
	*lChoice = RCro78_RelDisName;
	}
      else
	if( *matchz==2 )
	  {
	  ro0070_stgetc(hdl); /* Skip known tag */
	  sterr = asn1st_intdec(hdl,poszm,&(*AeQual).lAeQualifierForm2);
	  *lChoice = RCro77_ObjId;
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




int  PASCAL AE_qualifier_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
long FAR * lChoice,
rT0637_uAeQualifier FAR * AeQual)
{
  switch (CAST_FOR_SWITCH(*lChoice))
    {
    case RCro78_RelDisName:
      RelDisName_cod(hdl,0,17,&(*AeQual).stAeQualifierForm1);
      break;
    case RCro77_ObjId:
      ro0067_stputc(hdl,(unsigned char)2);
      asn1st_intcod(hdl,(*AeQual).lAeQualifierForm2);
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  return(ro0063_sterror(hdl));
}






int  PASCAL Name_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0634_stName FAR * x)
{
  int sterr = ro0063_sterror(hdl);  /* sterr 21 */
  unsigned char  matchz[6]; /* 21 */
  /* I_count */
  unsigned long I_1;
 
    {
    int asn1_choice_ok=1;
    if ( sterr != OK )
      asn1_choice_ok = 0;
    else
      {
      /* matching choice, use matchz */
      asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
      if( *matchz==48 )
	{
	ro0070_stgetc(hdl); /* Skip known tag */
 
	  {
	  long posz2;
	  long poszms = poszm;
	  sterr = asn1st_length(hdl,poszm,&posz2);
	  if (posz2)
	    poszm = posz2;
 
	  I_1 = 0;
	  while( /* inside set of */ (sterr  == OK ) &&
	    ((ro0076_stwhere(hdl) + 1) < (signed long) poszm) &&
	    asn1st_getmatchz(hdl,poszm,matchz) )
	    {
	    if ( RCro7f_MaxRDNSequence <= I_1 )
	      {
	      sterr = asn1st_errdecset(hdl,ASN1_ERR_SOFSZ);
	      break;
	      }
	    else
	      {
	      if( ((*matchz)|32)==49 )
		{
		sterr=RelDisName_dec(hdl,poszm,&(*x).uName.stRDNSequence.
		stRDNSequence[I_1]);
		/* Skip unknown components for extensibility */
		}
	      else
		{
		sterr = asn1st_skip(hdl,poszm);
		}
	      }
	    I_1++;
	    }
	  (*x).uName.stRDNSequence.lRDNSequence = I_1;
	  sterr = asn1st_close(hdl,poszm,posz2);
	  poszm = poszms;
	  }
	(*x).lName = 1;
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




int  PASCAL Name_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0634_stName FAR * x)
{
  /* I_count */
  unsigned long I_1;
  switch (CAST_FOR_SWITCH((*x).lName))
    {
    case 1:
      ro0067_stputc(hdl,(unsigned char)48);
 
	{
	long posz0 = ro0076_stwhere(hdl);
	asn1st_deflenpos(hdl);
	for (I_1=0; I_1 < (*x).uName.stRDNSequence.lRDNSequence; I_1++)
	  {
	  RelDisName_cod(hdl,0,17,&(*x).uName.stRDNSequence.stRDNSequence[I_1]);
	  }
	asn1st_deflencod(hdl, posz0);
	}
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  return(ro0063_sterror(hdl));
}





int  PASCAL RelDisName_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0631_stRDN FAR * x)
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
      if ( RCro7g_MaxRDN <= I_0 )
	{
	sterr = asn1st_errdecset(hdl,ASN1_ERR_SOFSZ);
	break;
	}
      else
	{
	if( ((*matchz)|32)==48 )
	  {
	  sterr=AttrValAss_dec(hdl,poszm,&(*x).stRDN[I_0]);
	  /* Skip unknown components for extensibility */
	  }
	else
	  {
	  sterr = asn1st_skip(hdl,poszm);
	  }
	}
      I_0++;
      }
    (*x).lRDN = I_0;
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr) /* return 21 */;
}





int  PASCAL RelDisName_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0631_stRDN FAR * x)
{
  /* I_count */
  unsigned long I_0;
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)49);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    for (I_0=0; I_0 < (*x).lRDN; I_0++)
      {
      AttrValAss_cod(hdl,0,16,&(*x).stRDN[I_0]);
      }
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}





int  PASCAL AttrValAss_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0630_stAttriValueAssertion FAR * x)
{
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==38 )
    {
    sterr = asn1st_skiptype(hdl,poszm);
    sterr = asn1st_oiddec(hdl,poszm,(*x).vAttributeType);
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==52 )
    {
    sterr = asn1st_octmov(hdl,poszm,(char FAR *)(*x).vAttributeValue,(long FAR *
    )&(*x).lAttributeType, (long) RCro7h_MaxAttriValue,(int) 1);
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  return(sterr);
}





int  PASCAL AttrValAss_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0630_stAttriValueAssertion FAR * x)
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
    sterr = AttrValAss_dseq(hdl,poszm,x);
    /* Skipping of not known elements for extensibility  */
    sterr = asn1st_skipclose(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}
int  PASCAL AttrValAss_ccod(
STOREHANDLE hdl,
rT0630_stAttriValueAssertion FAR * x)
{
  ro0067_stputc(hdl,(unsigned char)6);
  asn1st_oidcod(hdl,(*x).vAttributeType);
  ro0067_stputc(hdl,(unsigned char)20);
  asn1st_octets_cod(hdl,(*x).vAttributeValue,(long)(*x).lAttributeType);
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */

int  PASCAL AttrValAss_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0630_stAttriValueAssertion FAR * x)
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
    AttrValAss_ccod(hdl,x);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
