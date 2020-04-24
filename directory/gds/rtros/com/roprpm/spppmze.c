/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spppmze.c,v $
 * Revision 1.1.4.2  1996/02/18  18:23:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:01  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:14:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:16  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:06:40  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:23  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:19  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:52:41  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:59  keutel
 * 	creation
 * 	[1994/03/21  13:21:52  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spppmze.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:23:05 $" } ;
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
#include "roppmzz.h"   

#include "roppmze.h"
 
/* SCCS ID inserted with sed                              */ 
 
static char *sccsid =    "@(#) spppmze.c 1.1 93/12/20   RTS-V2.00"; 

int  PASCAL CP_type_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT041A_stCp FAR * x,
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
	    sterr=Components_of_set_dec(hdl,poszm,
                     (unsigned long FAR *)&(*x).hX410RtpmNode,
	    RC104u_RTORQapdu,(void NEAR *)pContext);
	    }
	  else
	    if(((*matchz)|32)==162 )
	      {
	      asn1set[asn1setc - 1] += 1;
	      (*x).bNormalSEQUENCE = 1;
	      sterr=CP_type_seq_dec(hdl,poszm,x,pContext);
	      }/* skip unknown components for extensibility */
	    else
	      sterr = asn1st_skip(hdl,poszm);
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
int  PASCAL CP_type_ccod(
STOREHANDLE hdl,
rT041A_stCp FAR * x,
asn1_pvoid pContext)
{
  Mode_selector_cod(hdl,2,0,&(*x).lModeSelector);
  if ((*x).hX410RtpmNode != 0)
    {
    Components_of_set_cod(hdl,2,1,
        (unsigned long FAR *)&(*x).hX410RtpmNode,RC104u_RTORQapdu,
        (void NEAR *)pContext)
    ;
    }
  if ((*x).bNormalSEQUENCE != 0)
    {
    CP_type_seq_cod(hdl,2,2,x,pContext);
    }
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */
int  PASCAL CP_type_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT041A_stCp FAR * x,
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
    CP_type_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL Context_list_dec(
STOREHANDLE hdl,
unsigned long poszm,
ulong FAR * ulDefListNumb,
asn1_prT0412_stPresContDef vDefList)
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
	  sterr=Context_list_seq_dec(hdl,poszm,&vDefList[I_0]);
	  /* Skip unknown components for extensibility */
	  }
	else
	  {
	  sterr = asn1st_skip(hdl,poszm);
	  }
	}
      I_0++;
      }
    *ulDefListNumb = I_0;
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr) /* return 21 */;
}
int  PASCAL Context_list_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
ulong FAR * ulDefListNumb,
asn1_prT0412_stPresContDef vDefList)
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
    for (I_0=0; I_0 < *ulDefListNumb; I_0++)
      {
      Context_list_seq_cod(hdl,0,16,&vDefList[I_0]);
      }
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL Context_list_seq_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT0412_stPresContDef FAR * vDefList)
{
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  /* I_count */
  unsigned long I_1;
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==2 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&(*vDefList).lPresContId);
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==38 )
    {
    sterr = asn1st_skiptype(hdl,poszm);
    sterr = asn1st_oiddec(hdl,poszm,(*vDefList).oAbstrSynName);
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && *matchz==48 )
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
	if ( RCro03_MaxTransPerAbstr <= I_1 )
	  {
	  sterr = asn1st_errdecset(hdl,ASN1_ERR_SOFSZ);
	  break;
	  }
	else
	  {
	  if( ((*matchz)|32)==38 )
	    {
	    sterr = asn1st_skiptype(hdl,poszm);
	    sterr = asn1st_oiddec(hdl,poszm,(*vDefList).vTransSynName[I_1]);
	    /* Skip unknown components for extensibility */
	    }
	  else
	    {
	    sterr = asn1st_skip(hdl,poszm);
	    }
	  }
	I_1++;
	}
      (*vDefList).ulTransSynNumber = I_1;
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  return(sterr);
}
int  PASCAL Context_list_seq_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0412_stPresContDef FAR * vDefList)
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
    sterr = Context_list_seq_dseq(hdl,poszm,vDefList);
    /* Skipping of not known elements for extensibility  */
    sterr = asn1st_skipclose(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}
int  PASCAL Context_list_seq_ccod(
STOREHANDLE hdl,
rT0412_stPresContDef FAR * vDefList)
{
  /* I_count */
  unsigned long I_1;
  ro0067_stputc(hdl,(unsigned char)2);
  asn1st_intcod(hdl,(*vDefList).lPresContId);
  ro0067_stputc(hdl,(unsigned char)6);
  asn1st_oidcod(hdl,(*vDefList).oAbstrSynName);
  ro0067_stputc(hdl,(unsigned char)48);
 
 
    {
    long posz0 = ro0076_stwhere(hdl);
    asn1st_deflenpos(hdl);
    for (I_1=0; I_1 < (*vDefList).ulTransSynNumber; I_1++)
      {
      ro0067_stputc(hdl,(unsigned char)6);
      asn1st_oidcod(hdl,(*vDefList).vTransSynName[I_1]);
      }
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */
int  PASCAL Context_list_seq_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0412_stPresContDef FAR * vDefList)
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
    Context_list_seq_ccod(hdl,vDefList);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL Default_context_name_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
asn1_prTro00_vObjectId Abstract_syntax,
asn1_prTro00_vObjectId Transfer_syntax)
{
  int sterr = ro0063_sterror(hdl); /* sterr 3 */
  unsigned char matchz[6]; /* for SEQ */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==160 )
    {
    sterr = asn1st_skiptype(hdl,poszm);
    sterr = asn1st_oiddec(hdl,poszm,Abstract_syntax);
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==161 )
    {
    sterr = asn1st_skiptype(hdl,poszm);
    sterr = asn1st_oiddec(hdl,poszm,Transfer_syntax);
    }
  else
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  return(sterr);
}
int  PASCAL Default_context_name_dec(
STOREHANDLE hdl,
unsigned long poszm,
asn1_prTro00_vObjectId Abstract_syntax,
asn1_prTro00_vObjectId Transfer_syntax)
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
    sterr = Default_context_name_dseq(hdl,poszm,Abstract_syntax,Transfer_syntax)
    ;
    /* Skipping of not known elements for extensibility  */
    sterr = asn1st_skipclose(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}
int  PASCAL Default_context_name_ccod(
STOREHANDLE hdl,
asn1_prTro00_vObjectId Abstract_syntax,
asn1_prTro00_vObjectId Transfer_syntax)
{
  ro0067_stputc(hdl,(unsigned char)128);
  asn1st_oidcod(hdl,Abstract_syntax);
  ro0067_stputc(hdl,(unsigned char)129);
  asn1st_oidcod(hdl,Transfer_syntax);
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */
int  PASCAL Default_context_name_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
asn1_prTro00_vObjectId Abstract_syntax,
asn1_prTro00_vObjectId Transfer_syntax)
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
    Default_context_name_ccod(hdl,Abstract_syntax,Transfer_syntax);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL Mode_selector_dec(
STOREHANDLE hdl,
unsigned long poszm,
long FAR * ModeSelector)
{
  int sterr = ro0063_sterror(hdl);  /* sterr 21 */
  unsigned char  matchz[6]; /* 21 */
  int asn1set[1];
  int asn1setc=0;
  ASN1_ZERO(&asn1set[asn1setc],1 * sizeof(int));
  asn1setc += 1;
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
	if( *matchz==128 )
	  {
	  asn1set[asn1setc - 1] += 1;
	  ro0070_stgetc(hdl); /* Skip known tag */
	  sterr = asn1st_intdec(hdl,poszm,ModeSelector);
	  }/* skip unknown components for extensibility */
	else
	  sterr = asn1st_skip(hdl,poszm);
	}
      sterr = asn1st_close(hdl,poszm,posz2);
      poszm = poszms;
      }
    }
  asn1setc -= 1;
  if (asn1set[asn1setc + 0] == 0)
    {
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }
  sterr = asn1st_checksetnum(hdl,1,asn1set);
  return(sterr) /* return 21 */;
}
int  PASCAL Mode_selector_ccod(
STOREHANDLE hdl,
long FAR * ModeSelector)
{
  ro0067_stputc(hdl,(unsigned char)128);
  asn1st_intcod(hdl,*ModeSelector);
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */
int  PASCAL Mode_selector_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
long FAR * ModeSelector)
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
    Mode_selector_ccod(hdl,ModeSelector);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL Fully_encoded_data_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT0415_stFully FAR * FullyData,
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
    if (sterr) sterr = OK ; /* by kc on 10-12-93 for limited log */ 
    I_0 = 0;
    while( /* inside set of */ (sterr  == OK ) &&
      ((ro0076_stwhere(hdl) + 1) < (signed long) poszm) &&
      asn1st_getmatchz(hdl,poszm,matchz)   )
      {
      if ( RCro0a_MaxPdvList <= I_0 )
	{
	sterr = asn1st_errdecset(hdl,ASN1_ERR_SOFSZ);
	break;
	}
      else
	{
	if( PDV_list_match(matchz))
	  {
	  sterr=PDV_list_dec(hdl,poszm,
                  (unsigned long FAR *)&(*FullyData).vPdvList[I_0],
                  (void NEAR *)pContext);
	  /* Skip unknown components for extensibility */
	  }
	else
	  {
	  sterr = asn1st_skip(hdl,poszm);
	  }
	}
      I_0++;
      }
    (*FullyData).ulPdvList = I_0;
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr) /* return 21 */;
}
int  PASCAL Fully_encoded_data_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT0415_stFully FAR * FullyData,
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
    for (I_0=0; I_0 < (*FullyData).ulPdvList; I_0++)
      {
      PDV_list_cod(hdl,-1,0,(unsigned long FAR *)&(*FullyData).vPdvList[I_0],
                   (void NEAR *)pContext);
      }
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
int  PASCAL CP_type_seq_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
rT041A_stCp FAR * x,
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
  if( (sterr == OK) && *matchz && (*matchz|32)==161 )
    {
    (*x).bCallingSelector = 1;
    sterr = asn1st_octmov(hdl,poszm,(char FAR *)(*x).vCallingSelector,(long FAR
    *)&(*x).ulCallingSelector, (long) RCro1a_MaxLenPSelector,(int) 1);
    }
  else
    {
    (*x).bCallingSelector = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==162 )
    {
    (*x).bCalledSelector = 1;
    sterr = asn1st_octmov(hdl,poszm,(char FAR *)(*x).vCalledSelector,(long FAR *
    )&(*x).ulCalledSelector, (long) RCro1a_MaxLenPSelector,(int) 1);
    }
  else
    {
    (*x).bCalledSelector = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==164 )
    {
    (*x).bPresContDef = 1;
    sterr=Context_list_dec(hdl,poszm,&(*x).ulPresContDef,(*x).vPresContDef);
    }
  else
    {
    (*x).bPresContDef = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==166 )
    {
    (*x).bDefaultContext = 1;
    sterr=Default_context_name_dec(hdl,poszm,(*x).oDefaultAbstrSyn,(*x).
    oDefaultTransSyn);
    }
  else
    {
    (*x).bDefaultContext = 0;
    }
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && *matchz && (*matchz|32)==168 )
    {
    (*x).bPresRequirements = 1;
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
    (*x).bSessRequirements = 1;
    sterr = asn1st_flagdec(hdl,poszm,(unsigned long FAR *)&(*x).
    bsSessRequirements,(long FAR *)&(*x).ulSessRequirements);
    }
  else
    {
    (*x).bSessRequirements = 0;
    }
 
    {/* Tag more than one byte or special type */
    asn1st_getmatchz(hdl,poszm,matchz); /* Reading Tag */
    if( (sterr == OK) && *matchz && User_data_match(matchz))
      {
      (*x).bUserData = 1;
    sterr = (int)ro0420_syntaxCp (pContext,(rT041G_unPpdu FAR *) x); 
      if (sterr)
	ro0080_seterror(hdl, (short) sterr);
      sterr=User_data_dec(hdl,poszm,&(*x).stUserData,pContext);
      }
    else
      {
      (*x).bUserData = 0;
    sterr = (int)ro0420_syntaxCp (pContext,(rT041G_unPpdu FAR *) x); 
			if (sterr) ro0080_seterror(hdl, (short) sterr);/* sed 1*/
      }
 
    }/* End use matchz */
  return(sterr);
}
int  PASCAL CP_type_seq_dec(
STOREHANDLE hdl,
unsigned long poszm,
rT041A_stCp FAR * x,
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
    sterr = CP_type_seq_dseq(hdl,poszm,x,pContext);
    /* Skipping of not known elements for extensibility  */
    sterr = asn1st_skipclose(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}
int  PASCAL CP_type_seq_ccod(
STOREHANDLE hdl,
rT041A_stCp FAR * x,
asn1_pvoid pContext)
{
  if (asn1_flags_cmp((*x).bsProtVersion,(*x).ulProtVersion,RC0419_ProtVersion1 ,
    /* copy1 */ (*x).ulProtVersion)/* copy 3 */ != 0)
    {
    ro0067_stputc(hdl,(unsigned char)128);
    asn1st_flagcod(hdl,(long)(*x).bsProtVersion,(long)(*x).ulProtVersion);
    }
  if ((*x).bCallingSelector != 0)
    {
    ro0067_stputc(hdl,(unsigned char)129);
    asn1st_octets_cod(hdl,(*x).vCallingSelector,(long)(*x).ulCallingSelector);
    }
  if ((*x).bCalledSelector != 0)
    {
    ro0067_stputc(hdl,(unsigned char)130);
    asn1st_octets_cod(hdl,(*x).vCalledSelector,(long)(*x).ulCalledSelector);
    }
  if ((*x).bPresContDef != 0)
    {
    Context_list_cod(hdl,2,4,&(*x).ulPresContDef,(*x).vPresContDef);
    }
  if ((*x).bDefaultContext != 0)
    {
    Default_context_name_cod(hdl,2,6,(*x).oDefaultAbstrSyn,(*x).oDefaultTransSyn
    );
    }
  if ((*x).bPresRequirements != 0)
    {
    ro0067_stputc(hdl,(unsigned char)136);
    asn1st_flagcod(hdl,(long)(*x).bsPresRequirements,(long)(*x).
    ulPresRequirements);
    }
  if ((*x).bSessRequirements != 0)
    {
    ro0067_stputc(hdl,(unsigned char)137);
    asn1st_flagcod(hdl,(long)(*x).bsSessRequirements,(long)(*x).
    ulSessRequirements);
    }
  if ((*x).bUserData != 0)
    {
    User_data_cod(hdl,-1,0,&(*x).stUserData,pContext);
    }
  return(ro0063_sterror(hdl));
}/* sterr ret 5 */
int  PASCAL CP_type_seq_cod(
STOREHANDLE hdl,
int asn1_m,
long asn1_n,
rT041A_stCp FAR * x,
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
    CP_type_seq_ccod(hdl,x,pContext);
    asn1st_deflencod(hdl, posz0);
    }
  return(ro0063_sterror(hdl));
}
