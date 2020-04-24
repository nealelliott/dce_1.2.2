/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppmzz.c,v $
 * Revision 1.1.4.2  1996/02/18  18:23:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:14:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:12  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:06:38  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:22  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:18  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:52:35  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:56  keutel
 * 	creation
 * 	[1994/03/21  13:21:47  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roppmzz.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:23:03 $" } ;
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include "rogenrl.h"   
#include "rouply0.h"   
#include "rostor0.h"   
#include "roppm01.h"   
#include "rortpm1.h"   
#include "roppm13.h"   
#include "asn1st.h"    
#include "roppmze.h"   

#include "roppmzz.h"
 
/* SCCS ID inserted with sed                              */ 
 
static char *sccsid =    "@(#) roppmzz.c 1.1 93/11/29   RTS-V2.00"; 

int  PASCAL CPA_PPDU_dec(
	STOREHANDLE hdl,
	unsigned long poszm,
	rT041B_stCpa FAR * x,
	asn1_pvoid pContext)
{
  int sterr = ro0063_sterror(hdl);  /* sterr 21 */
  unsigned char  matchz[6]; /* 21 */
  int asn1set[3];
  int asn1setc=0;
  ASN1_ZERO(&asn1set[asn1setc],3 * sizeof(int));
  asn1setc += 3;
  if (sterr == OK)
    {
    ro0070_stgetc(hdl); /* Skip known tag */
 
      {
      long posz2;
      long poszms = poszm;
      sterr = asn1st_length(hdl,poszm,&posz2);
      if (posz2)
	poszm = posz2;
      while(/* inside set */ (sterr  == OK ) &&
	((ro0076_stwhere(hdl) + 1) < (signed long) poszm) &&
	asn1st_getmatchz(hdl,poszm,matchz))
	{
	/* first = 0 ? <}> ? */
	if(((*matchz)|32)==160 )
	  {
	  asn1set[asn1setc - 3] += 1;
	  sterr=Mode_selector_dec(hdl,poszm,&(*x).lModeSelector);
	  }
	else
	  if(((*matchz)|32)==161 )
	    {
	    asn1set[asn1setc - 2] += 1;
	    sterr=Components_of_set_dec(hdl,poszm,&(*x).hX410RtpmNode,
	    RC104v_RTOACapdu,pContext);
	    }
	  else
	    if(((*matchz)|32)==162 )
	      {
	      asn1set[asn1setc - 1] += 1;
	      (*x).bNormalSEQUENCE = 1;
	      sterr=CPA_PPDU_seq_dec(hdl,poszm,x,pContext);
	      }
	    else
	      {
	      asn1st_errdecset(hdl,ASN1_ERR_WRSET);
	      break;
	      }
	/* in MAVROS V2.2 */
	/* else sterr = asn1st_skip(hdl,poszm); */
	/* necessary if SET is used in COMPONENTS OF */
	}
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  asn1setc -= 3;
  if (asn1set[asn1setc + 0] == 0)
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  if (asn1set[asn1setc + 1] == 0)
    {
    (*x).hX410RtpmNode = 0;
    }
  if (asn1set[asn1setc + 2] == 0)
    {
    (*x).bNormalSEQUENCE = 0;
    }
  sterr = asn1st_checksetnum(hdl,3,asn1set);
  return(sterr) /* return 21 */;
}
int  PASCAL CPA_PPDU_ccod(
	STOREHANDLE hdl,
	rT041B_stCpa FAR * x,
	asn1_pvoid pContext)
{
  Mode_selector_cod(hdl,2,0,&(*x).lModeSelector);
  if ((*x).hX410RtpmNode != 0)
    {
    Components_of_set_cod(hdl,2,1,&(*x).hX410RtpmNode,RC104v_RTOACapdu,pContext)
    ;
    }
  if ((*x).bNormalSEQUENCE != 0)
    {
    CPA_PPDU_seq_cod(hdl,2,2,x,pContext);
    }
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */

int  PASCAL CPA_PPDU_cod(
	STOREHANDLE hdl,
	int asn1_m,
	long asn1_n,
	rT041B_stCpa FAR * x,
	asn1_pvoid pContext)
{
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)49);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    CPA_PPDU_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}

int  PASCAL CPR_PPDU_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT041C_stCpr FAR * x,
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
      if(Components_of_set_match(matchz))
	{
	sterr=Components_of_set_dec(hdl,poszm,&(*x).hX410RtpmNode,
	RC104w_RTORJapdu,pContext);
	(*x).lX410orNormalChoice = RC041a_X410Mode;
	}
      else
	if(((*matchz)|32)==48 )
	  {
	  sterr=CPR_PPDU_seq_dec(hdl,poszm,x,pContext);
	  (*x).lX410orNormalChoice = RC041b_NormalMode;
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

int  PASCAL CPR_PPDU_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT041C_stCpr FAR * x,
asn1_pvoid pContext)
{
  switch (CAST_FOR_SWITCH((*x).lX410orNormalChoice))
    {
    case RC041a_X410Mode:
      Components_of_set_cod(hdl,-1,0,&(*x).hX410RtpmNode,RC104w_RTORJapdu,
      pContext);
      break;
    case RC041b_NormalMode:
      CPR_PPDU_seq_cod(hdl,0,16,x,pContext);
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  return(ro0063_sterror(hdl));
}




int  PASCAL Abort_type_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT041G_unPpdu FAR * x,
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
      if(ARU_PPDU_match(matchz))
	{
	sterr=ARU_PPDU_dec(hdl,poszm,&(*x).stAru,pContext);
	(*x).stPdvHeader.lType = RC041D_Aru;
	}
      else
	if(((*matchz)|32)==48 )
	  {
	  sterr=ARP_PPDU_dec(hdl,poszm,&(*x).stArp);
	  (*x).stPdvHeader.lType = RC041E_Arp;
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

int  PASCAL Abort_type_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT041G_unPpdu FAR * x,
asn1_pvoid pContext)
{
  switch (CAST_FOR_SWITCH((*x).stPdvHeader.lType))
    {
    case RC041D_Aru:
      ARU_PPDU_cod(hdl,-1,0,&(*x).stAru,pContext);
      break;
    case RC041E_Arp:
      ARP_PPDU_cod(hdl,0,16,&(*x).stArp);
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL ARU_PPDU_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT041D_stAru FAR * x,
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
      if(Components_of_set_match(matchz))
	{
	sterr=Components_of_set_dec(hdl,poszm,&(*x).hX410RtpmNode,
	RC104z_RTABapdu,pContext);
	(*x).lX410orNormalChoice = RC041a_X410Mode;
	}
      else
	if(((*matchz)|32)==160 )
	  {
	  sterr=ARU_PPDU_seq_dec(hdl,poszm,x,pContext);
	  (*x).lX410orNormalChoice = RC041b_NormalMode;
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
int  PASCAL ARU_PPDU_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT041D_stAru FAR * x,
asn1_pvoid pContext)
{
  switch (CAST_FOR_SWITCH((*x).lX410orNormalChoice))
    {
    case RC041a_X410Mode:
      Components_of_set_cod(hdl,-1,0,&(*x).hX410RtpmNode,RC104z_RTABapdu,
      pContext);
      break;
    case RC041b_NormalMode:
      ARU_PPDU_seq_cod(hdl,2,0,x,pContext);
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL ARP_PPDU_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT041E_stArp FAR * x)
{
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==128 )
    {
    (*x).bAbortReason = 1;
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&(*x).lAbortReason);
    }
  else
    {
    (*x).bAbortReason = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==129 )
    {
    (*x).bEventId = 1;
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&(*x).lEventId);
    }
  else
    {
    (*x).bEventId = 0;
    }
  return(sterr);
}
int  PASCAL ARP_PPDU_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT041E_stArp FAR * x)
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
    sterr = ARP_PPDU_dseq(hdl,poszm,x);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}
int  PASCAL ARP_PPDU_ccod(
STOREHANDLE hdl,
rT041E_stArp FAR * x)
{
  if ((*x).bAbortReason != 0)
    {
    ro0067_stputc(hdl,(unsigned char)128);
    asn1st_intcod(hdl,(*x).lAbortReason);
    }
  if ((*x).bEventId != 0)
    {
    ro0067_stputc(hdl,(unsigned char)129);
    asn1st_intcod(hdl,(*x).lEventId);
    }
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */
int  PASCAL ARP_PPDU_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT041E_stArp FAR * x)
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
    ARP_PPDU_ccod(hdl,x);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL Pres_cont_id_list_dec(
STOREHANDLE hdl,
unsigned long poszm,
ulong FAR * ContIdNumb,
asn1_prT0414_stPresContId ContIdList)
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
      if ( RCro04_MaxPresCont <= I_0 )
	{
	sterr = asn1st_errdecset(hdl,ASN1_ERR_SOFSZ);
	break;
	}
      else
	{
	if( ((*matchz)|32)==48 )
	  {
	  sterr=Pres_cont_id_list_seq_dec(hdl,poszm,&ContIdList[I_0]);
	  }
	else
	  {
	  sterr = asn1st_errdecset(hdl,ASN1_ERR_WRSET);
	  break;
	  }
	}
      I_0++;
      }
    *ContIdNumb = I_0;
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr) /* return 21 */;
}
int  PASCAL Pres_cont_id_list_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
ulong FAR * ContIdNumb,
asn1_prT0414_stPresContId ContIdList)
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
    for (I_0=0; I_0 < *ContIdNumb; I_0++)
      {
      Pres_cont_id_list_seq_cod(hdl,0,16,&ContIdList[I_0]);
      }
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL Pres_cont_id_list_seq_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0414_stPresContId FAR * ContIdList)
{
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==2 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&(*ContIdList).lPresContId);
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==38 )
    {
    sterr = asn1st_skiptype(hdl,poszm);
    sterr = asn1st_oiddec(hdl,poszm,(*ContIdList).oTransSynName);
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  return(sterr);
}
int  PASCAL Pres_cont_id_list_seq_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0414_stPresContId FAR * ContIdList)
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
    sterr = Pres_cont_id_list_seq_dseq(hdl,poszm,ContIdList);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}
int  PASCAL Pres_cont_id_list_seq_ccod(
STOREHANDLE hdl,
rT0414_stPresContId FAR * ContIdList)
{
  ro0067_stputc(hdl,(unsigned char)2);
  asn1st_intcod(hdl,(*ContIdList).lPresContId);
  ro0067_stputc(hdl,(unsigned char)6);
  asn1st_oidcod(hdl,(*ContIdList).oTransSynName);
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */
int  PASCAL Pres_cont_id_list_seq_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0414_stPresContId FAR * ContIdList)
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
    Pres_cont_id_list_seq_ccod(hdl,ContIdList);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL Result_list_dec(
STOREHANDLE hdl,
unsigned long poszm,
ulong FAR * DefResNumb,
asn1_prT0413_stPresContDefRes DefResList)
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
      if ( RCro04_MaxPresCont <= I_0 )
	{
	sterr = asn1st_errdecset(hdl,ASN1_ERR_SOFSZ);
	break;
	}
      else
	{
	if( ((*matchz)|32)==48 )
	  {
	  sterr=Result_list_seq_dec(hdl,poszm,&DefResList[I_0]);
	  }
	else
	  {
	  sterr = asn1st_errdecset(hdl,ASN1_ERR_WRSET);
	  break;
	  }
	}
      I_0++;
      }
    *DefResNumb = I_0;
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr) /* return 21 */;
}
int  PASCAL Result_list_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
ulong FAR * DefResNumb,
asn1_prT0413_stPresContDefRes DefResList)
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
    for (I_0=0; I_0 < *DefResNumb; I_0++)
      {
      Result_list_seq_cod(hdl,0,16,&DefResList[I_0]);
      }
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL Result_list_seq_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0413_stPresContDefRes FAR * DefResList)
{
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==128 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&(*DefResList).lResult);
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==161 )
    {
    sterr = asn1st_skiptype(hdl,poszm);
    sterr = asn1st_oiddec(hdl,poszm,(*DefResList).oTransSynName);
    }
  else
    {
    (*DefResList).oTransSynName[0] = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==130 )
    {
    (*DefResList).bProviderReason = 1;
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&(*DefResList).lProviderReason);
    }
  else
    {
    (*DefResList).bProviderReason = 0;
    }
  return(sterr);
}
int  PASCAL Result_list_seq_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0413_stPresContDefRes FAR * DefResList)
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
    sterr = Result_list_seq_dseq(hdl,poszm,DefResList);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}
int  PASCAL Result_list_seq_ccod(
STOREHANDLE hdl,
rT0413_stPresContDefRes FAR * DefResList)
{
  ro0067_stputc(hdl,(unsigned char)128);
  asn1st_intcod(hdl,(*DefResList).lResult);
  if ((*DefResList).oTransSynName[0] != 0)
    {
    ro0067_stputc(hdl,(unsigned char)129);
    asn1st_oidcod(hdl,(*DefResList).oTransSynName);
    }
  if ((*DefResList).bProviderReason != 0)
    {
    ro0067_stputc(hdl,(unsigned char)130);
    asn1st_intcod(hdl,(*DefResList).lProviderReason);
    }
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */
int  PASCAL Result_list_seq_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0413_stPresContDefRes FAR * DefResList)
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
    Result_list_seq_ccod(hdl,DefResList);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL User_data_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0417_stUserData FAR * UserData,
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
      if(((*matchz)|32)==96 )
	{
	sterr=Simply_encoded_data_dec(hdl,poszm,&(*UserData).unUserData.hSimply,
	pContext);
	(*UserData).lSimplyFullyChoice = RC0417_SimplyEncoded;
	}
      else
	if(((*matchz)|32)==97 )
	  {
	  sterr=Fully_encoded_data_dec(hdl,poszm,&(*UserData).unUserData.stFully
	  ,pContext);
	  (*UserData).lSimplyFullyChoice = RC0418_FullyEncoded;
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
int  PASCAL User_data_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0417_stUserData FAR * UserData,
asn1_pvoid pContext)
{
  switch (CAST_FOR_SWITCH((*UserData).lSimplyFullyChoice))
    {
    case RC0417_SimplyEncoded:
      Simply_encoded_data_cod(hdl,1,0,&(*UserData).unUserData.hSimply,pContext);
      break;
    case RC0418_FullyEncoded:
      Fully_encoded_data_cod(hdl,1,1,&(*UserData).unUserData.stFully,pContext);
      break;
    default:
      asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
      break;
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL CPA_PPDU_seq_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT041B_stCpa FAR * x,
asn1_pvoid pContext)
{
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==160 )
    {
    sterr = asn1st_flagdec(hdl,poszm,(unsigned long FAR *)&(*x).bsProtVersion,(
    long FAR *)&(*x).ulProtVersion);
    }
  else
    {
    asn1_flags_cpy(&(*x).bsProtVersion,(*x).ulProtVersion,RC0419_ProtVersion1 ,
    /* copy1 */ (*x).ulProtVersion)/* copy 2 */;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==163 )
    {
    (*x).bRespondingSelector = 1;
    sterr = asn1st_octmov(hdl,poszm,(char FAR *)(*x).vRespondingSelector,(long
    FAR *)&(*x).ulRespondingSelector, (long) RCro1a_MaxLenPSelector,(int) 1);
    }
  else
    {
    (*x).bRespondingSelector = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==165 )
    {
    (*x).bPresContDefRes = 1;
    sterr=Result_list_dec(hdl,poszm,&(*x).ulPresContDefRes,(*x).vPresContDefRes)
    ;
    }
  else
    {
    (*x).bPresContDefRes = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==168 )
    {
    sterr = asn1st_flagdec(hdl,poszm,(unsigned long FAR *)&(*x).
    bsPresRequirements,(long FAR *)&(*x).ulPresRequirements);
    }
  else
    {
    (*x).bPresRequirements = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==169 )
    {
    sterr = asn1st_flagdec(hdl,poszm,(unsigned long FAR *)&(*x).
    bsPresRequirements,(long FAR *)&(*x).ulPresRequirements);
    }
  else
    {
    (*x).bPresRequirements = 0;
    }
 
    {/* Tag more than one byte or special type */
    asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
    if( (sterr == OK) && *matchz && User_data_match(matchz))
      {
      (*x).bUserData = 1;
      sterr = (int)ro0421_syntaxCpa(pContext,(rT041G_unPpdu FAR *) x);
      if (sterr)
	ro0080_seterror(hdl, (short) sterr);
      sterr=User_data_dec(hdl,poszm,&(*x).stUserData,pContext);
      }
    else
      {
      (*x).bUserData = 0;
      sterr = (int)ro0421_syntaxCpa(pContext,(rT041G_unPpdu FAR *) x);
			if (sterr) ro0080_seterror(hdl, (short) sterr);/* sed 1*/
      }
 
    }/* End use matchz */
  return(sterr);
}
int  PASCAL CPA_PPDU_seq_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT041B_stCpa FAR * x,
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
    sterr = CPA_PPDU_seq_dseq(hdl,poszm,x,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}
int  PASCAL CPA_PPDU_seq_ccod(
STOREHANDLE hdl,
rT041B_stCpa FAR * x,
asn1_pvoid pContext)
{
  if (asn1_flags_cmp((*x).bsProtVersion,(*x).ulProtVersion,RC0419_ProtVersion1 ,
    /* copy1 */ (*x).ulProtVersion)/* copy 3 */ != 0)
    {
    ro0067_stputc(hdl,(unsigned char)128);
    asn1st_flagcod(hdl,(long)(*x).bsProtVersion,(long)(*x).ulProtVersion);
    }
  if ((*x).bRespondingSelector != 0)
    {
    ro0067_stputc(hdl,(unsigned char)131);
    asn1st_octets_cod(hdl,(*x).vRespondingSelector,(long)(*x).
    ulRespondingSelector);
    }
  if ((*x).bPresContDefRes != 0)
    {
    Result_list_cod(hdl,2,5,&(*x).ulPresContDefRes,(*x).vPresContDefRes);
    }
  if ((*x).bPresRequirements != 0)
    {
    ro0067_stputc(hdl,(unsigned char)136);
    asn1st_flagcod(hdl,(long)(*x).bsPresRequirements,(long)(*x).
    ulPresRequirements);
    }
  if ((*x).bPresRequirements != 0)
    {
    ro0067_stputc(hdl,(unsigned char)137);
    asn1st_flagcod(hdl,(long)(*x).bsPresRequirements,(long)(*x).
    ulPresRequirements);
    }
  if ((*x).bUserData != 0)
    {
    User_data_cod(hdl,-1,0,&(*x).stUserData,pContext);
    }
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */
int  PASCAL CPA_PPDU_seq_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT041B_stCpa FAR * x,
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
    CPA_PPDU_seq_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL CPR_PPDU_seq_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT041C_stCpr FAR * x,
asn1_pvoid pContext)
{
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==160 )
    {
    sterr = asn1st_flagdec(hdl,poszm,(unsigned long FAR *)&(*x).bsProtVersion,(
    long FAR *)&(*x).ulProtVersion);
    }
  else
    {
    asn1_flags_cpy(&(*x).bsProtVersion,(*x).ulProtVersion,RC0419_ProtVersion1 ,
    /* copy1 */ (*x).ulProtVersion)/* copy 2 */;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==163 )
    {
    (*x).bRespondingSelector = 1;
    sterr = asn1st_octmov(hdl,poszm,(char FAR *)(*x).vRespondingSelector,(long
    FAR *)&(*x).ulRespondingSelector, (long) RCro1a_MaxLenPSelector,(int) 1);
    }
  else
    {
    (*x).bRespondingSelector = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==165 )
    {
    (*x).bPresContDefRes = 1;
    sterr=Result_list_dec(hdl,poszm,&(*x).ulPresContDefRes,(*x).vPresContDefRes)
    ;
    }
  else
    {
    (*x).bPresContDefRes = 0;
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
      (*x).bDefaultContextResult = 1;
      if (sterr == OK)
	{
	ro0070_stgetc(hdl); /* Skip known tag */
	sterr = asn1st_intdec(hdl,poszm,&(*x).lDefaultContextResult);
	}
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bDefaultContextResult = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==170 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
 
      {
      long posz2;
      long poszms = poszm;
      sterr = asn1st_length(hdl,poszm,&posz2);
      if (posz2)
	poszm = posz2;
      (*x).bProviderReason = 1;
      if (sterr == OK)
	{
	ro0070_stgetc(hdl); /* Skip known tag */
	sterr = asn1st_intdec(hdl,poszm,&(*x).lProviderReason);
	}
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    (*x).bProviderReason = 0;
    }
 
    {/* Tag more than one byte or special type */
    asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
    if( (sterr == OK) && *matchz && User_data_match(matchz))
      {
      (*x).bUserData = 1;
      sterr = (int)ro0422_syntaxCpr(pContext,(rT041G_unPpdu FAR *) x);
      if (sterr)
	ro0080_seterror(hdl, (short) sterr);
      sterr=User_data_dec(hdl,poszm,&(*x).stUserData,pContext);
      }
    else
      {
      (*x).bUserData = 0;
      sterr = (int)ro0422_syntaxCpr(pContext,(rT041G_unPpdu FAR *) x);
			if (sterr) ro0080_seterror(hdl, (short) sterr);/* sed 1*/
      }
 
    }/* End use matchz */
  return(sterr);
}
int  PASCAL CPR_PPDU_seq_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT041C_stCpr FAR * x,
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
    sterr = CPR_PPDU_seq_dseq(hdl,poszm,x,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}
int  PASCAL CPR_PPDU_seq_ccod(
STOREHANDLE hdl,
rT041C_stCpr FAR * x,
asn1_pvoid pContext)
{
  if (asn1_flags_cmp((*x).bsProtVersion,(*x).ulProtVersion,RC0419_ProtVersion1 ,
    /* copy1 */ (*x).ulProtVersion)/* copy 3 */ != 0)
    {
    ro0067_stputc(hdl,(unsigned char)128);
    asn1st_flagcod(hdl,(long)(*x).bsProtVersion,(long)(*x).ulProtVersion);
    }
  if ((*x).bRespondingSelector != 0)
    {
    ro0067_stputc(hdl,(unsigned char)131);
    asn1st_octets_cod(hdl,(*x).vRespondingSelector,(long)(*x).
    ulRespondingSelector);
    }
  if ((*x).bPresContDefRes != 0)
    {
    Result_list_cod(hdl,2,5,&(*x).ulPresContDefRes,(*x).vPresContDefRes);
    }
  if ((*x).bDefaultContextResult != 0)
    {
    ro0067_stputc(hdl,(unsigned char)167);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      ro0067_stputc(hdl,(unsigned char)2);
      asn1st_intcod(hdl,(*x).lDefaultContextResult);
      asn1st_deflencod(hdl, posz0);
      }
    }
  if ((*x).bProviderReason != 0)
    {
    ro0067_stputc(hdl,(unsigned char)170);
 
 
      {
      long posz0 = ro0076_stwhere(hdl);
      asn1st_deflenpos(hdl);
      ro0067_stputc(hdl,(unsigned char)2);
      asn1st_intcod(hdl,(*x).lProviderReason);
      asn1st_deflencod(hdl, posz0);
      }
    }
  if ((*x).bUserData != 0)
    {
    User_data_cod(hdl,-1,0,&(*x).stUserData,pContext);
    }
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */
int  PASCAL CPR_PPDU_seq_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT041C_stCpr FAR * x,
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
    CPR_PPDU_seq_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL ARU_PPDU_seq_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT041D_stAru FAR * x,
asn1_pvoid pContext)
{
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==160 )
    {
    (*x).bPresContId = 1;
    sterr=Pres_cont_id_list_dec(hdl,poszm,&(*x).ulPresContId,(*x).vPresContId);
    }
  else
    {
    (*x).bPresContId = 0;
    }
 
    {/* Tag more than one byte or special type */
    asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
    if( (sterr == OK) && *matchz && User_data_match(matchz))
      {
      (*x).bUserData = 1;
      sterr = (int)ro0423_syntaxAru(pContext,(rT041G_unPpdu FAR *) x);
      if (sterr)
	ro0080_seterror(hdl, (short) sterr);
      sterr=User_data_dec(hdl,poszm,&(*x).stUserData,pContext);
      }
    else
      {
      (*x).bUserData = 0;
      sterr = (int)ro0423_syntaxAru(pContext,(rT041G_unPpdu FAR *) x);
			if (sterr) ro0080_seterror(hdl, (short) sterr);/* sed 1*/
      }
 
    }/* End use matchz */
  return(sterr);
}
int  PASCAL ARU_PPDU_seq_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT041D_stAru FAR * x,
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
    sterr = ARU_PPDU_seq_dseq(hdl,poszm,x,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}
int  PASCAL ARU_PPDU_seq_ccod(
STOREHANDLE hdl,
rT041D_stAru FAR * x,
asn1_pvoid pContext)
{
  if ((*x).bPresContId != 0)
    {
    Pres_cont_id_list_cod(hdl,2,0,&(*x).ulPresContId,(*x).vPresContId);
    }
  if ((*x).bUserData != 0)
    {
    User_data_cod(hdl,-1,0,&(*x).stUserData,pContext);
    }
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */
int  PASCAL ARU_PPDU_seq_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT041D_stAru FAR * x,
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
    ARU_PPDU_seq_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
