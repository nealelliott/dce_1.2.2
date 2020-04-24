/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stpres.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:03  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:09:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:17  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:28  keutel
 * 	OT 11620
 * 	[1994/08/19  14:22:28  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:13  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:47:55  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:10  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:47:09  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:24  keutel
 * 	creation
 * 	[1994/03/21  13:16:39  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stpres.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:15 $" } ;
#endif

/****************************************************************************\
*                                                                            *
*  * Copyright (c) 1991 by                                                   *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG                                  *
*  * All rights reserved                                                     *
*  *                                                                         *
*  * This software is furnished under licence and may be used only in        *
*  * accordance with the terms of that licence and with the inclusion of     *
*  * the above copyright notice. This software may not be provided or        *
*  * otherwise made available to, or used by, any other person. No title     *
*  * to or ownership of the software is hereby transferred.                  *
*                                                                            *
\****************************************************************************/

/*daton *********************************************************************/
/*                                                                          */
/* TYPE         : MODULE                                                    */
/*                                                                          */
/* NAME         : st_pres.c                                                 */
/*                                                                          */
/* AUTHOR       : Hubert Trieb                                              */
/* DATE         : 17.01.1992                                                */
/*                                                                          */
/* COMPONENT    : Encode/Decode functions                                   */
/*                                                                          */
/* PRD#/VERS.   : ST_MAVROS                                                 */
/*                                                                          */
/* DESCRIPTION  : This modul provides functions which                       */
/*                are the Interface between functions  created              */
/*                by MAVROS and the Encode/Decode Control functions         */
/*                of Presentation.                                          */
/*                These functions are called when within a pdu the types    */
/*                PDV-list, Simpli-encoded-data or X410-mode (RTS-PDUs)     */
/*                are used.                                                 */
/*                Most parts of the functions have been created by MAVROS.  */
/*                                                                          */
/*                                                                          */
/* SYSTEM DEPENDENCE :  SINIX V5.4                                          */
/*                                                                          */
/* HISTORY      :                                                           */
/*                                                                          */
/* Vers.Nr. | Date     |         Modification              |mark| CR# EM#   */
/* 1.00 K0  | 92-01-17 |  original                         | ht |           */
/*          |          |                                   |    |           */
/*datoff ********************************************************************/

/****************************************************************************/
/*                                                                          */
/*                     Module Identification                                */
/*                                                                          */
/****************************************************************************/

static char *sccsid = " @(#) stpres.c 1.2 93/12/20  ST_MAVROS";

/****************************************************************************/
/*                                                                          */
/*                    S T D - I N C L U D E S                               */
/*                                                                          */
/****************************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************/
/*                                                                           */
/*        D E F I N E S,  T Y P E D E F S  &   P R O T O T Y P E S           */
/*                                                                           */
/*****************************************************************************/

#include <rogenrl.h>                             /* general defines for RTS */
#include <rostor0.h>                                    /* Store Prototypes */
#include <rouply0.h>
#include <roppm01.h>
#include <rodpat0.h>
#include <rodpat1.h>
#include <roppm05.h>
/*
#include <rortpm2.h>
*/

#include <asn1st.h>


/***************************************************************************/
/*                                                                         */
/*              P R O V I D E D   G L O B A L   F U N C T I O N S          */
/*                                                                         */
/***************************************************************************/



/***************************************************************************/
/*                                                                         */
/* int PDV_list_dec(hdl,poszm,pNode,pContext)                              */
/*                                                                         */
/* STOREHANDLE hdl;       Handle to store containing ASN1 String           */
/* unsigned long poszm;   End of ASN1 String in store                      */
/* ulong * pNode;         pointer to next node handle                      */
/* void *  pContext;      PPM context(pointer to)                          */
/*                                                                         */
/* Function for decoding Pdv-list element                                  */
/* Function created by MAVROS and changed                                  */
/* This function is called by a MAVROS created function.                   */
/*                                                                         */
/***************************************************************************/


int PASCAL PDV_list_dec(hdl,poszm,pNode,pContext)
STOREHANDLE hdl;
unsigned long poszm;
unsigned long FAR * pNode;
void NEAR * pContext;
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
    else     /* Added by Baiju / Amit on 5-4-94  K02 */ 
      ro042c_indefDecinc(pContext);                
    sterr = PDV_list_dseq(hdl,poszm,pNode,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
  }


/***************************************************************************/
/*                                                                         */
/* int PDV_list_dseq(hdl,poszm,pNode,pContext)                             */
/*                                                                         */
/* STOREHANDLE hdl;       Handle to store containing ASN1 String           */
/* unsigned long poszm;   End of ASN1 String in store                      */
/* ulong * pNode;         pointer to next node handle                      */
/* void * pContext;   PPM context(pointer to)                              */
/*                                                                         */
/* Function for decoding body of Pdv-list element                          */
/*                                                                         */
/* Function created by MAVROS and changed                                  */
/* This function is called by the function PDV_list_dec.                   */
/*                                                                         */
/***************************************************************************/

int PASCAL PDV_list_dseq(hdl,poszm,pNode,pContext)
STOREHANDLE hdl;
unsigned long int poszm;
unsigned long FAR * pNode;
void NEAR * pContext;
  {
  int sterr;
  unsigned char matchz[6]; /* for SEQ */
  long pos_next;

  unsigned long int ulNumb=0; /* length of presentation-data-values in store */
  unsigned long int ulOffset; /* Offset of presentation-data-values in store */

  rT0410_stPdvList localPdvList;   /* Structure for local values of PDV-list */
  signed short int sAbstrSynId;
  signed short int sPmNumb;
  unsigned long int FAR * pTransSynName;
  rTro76_stDefinedBy FAR * pDefinedBy = 0;

  signed short int ed_control_err;


  sterr = ro0063_sterror(hdl);
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */

  /* Test if Transfer-syntax-name was sent */
  if( (sterr == OK) && (*matchz|32)==38 )
    {
	     /* decoding Transfer-syntax-name into local Object Identifier */
    localPdvList.oTransSynName[0] = 1;             /* Boolean for OPTIONAL */
    ro0070_stgetc(hdl);                                  /* Skip known tag */
    sterr = asn1st_oiddec(hdl,poszm,localPdvList.oTransSynName);
    }
  else
    {                                  /* No Transfer-syntax-name was sent */
    localPdvList.oTransSynName[0] = 0;
    }

  matchz[0] = (char)ro0077_stgetcp(hdl);                    /* Reading Tag */

		       /* Test if Presentation-context-identifier was sent */
  if( (sterr == OK) && *matchz==2 )
    {
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&localPdvList.lPresContId);
    }
  else
    {
	 /* Set error, because no Presentation-context-identifier was sent */
    sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
    }


/* Decoding Presentation-data-value */
    {
    asn1st_getmatchz(hdl,poszm,matchz);                     /* Reading Tag */
			       /* Test if Presentation-data-value was sent */
    if( (sterr == OK) && *matchz &&  (  *matchz==160
      || ((*matchz)|32)==161
      || ((*matchz)|32)==162  ) )

      {
      if( *matchz==160 )        /* data-values encoded as simply-ASN1-type */
	{
	ro0070_stgetc(hdl);                              /* Skip known tag */
	  {
	  long posz2;
	  long poszms = poszm;
	  sterr = asn1st_length(hdl,poszm,&posz2);

	  localPdvList.lEncodingChoice = RCro0m_ChoiceAny;
	  ed_control_err = ro0416_decodePdvList (
				  (unsigned long int FAR * FAR *)&pTransSynName,
				  (signed short  int       FAR *)&sAbstrSynId,
				  (signed short  int       FAR *)&sPmNumb,
				  (rT0410_stPdvList        FAR *)&localPdvList,
				   pContext);
	  if (ed_control_err) {
		ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
		return ((int)ed_control_err);
	  }
	  ulOffset = ro0076_stwhere(hdl);
						/* Offset where ANY starts */
	  if (posz2) {
	       poszm = posz2;
	       ulNumb = (posz2 - ulOffset);
			     /* Length of ANY (if defined length encoding) */
	  }
/*	  else ulNumb = 0; */
          else     /* Added by Baiju / Amit on 5-4-94 K02*/  
               ro042c_indefDecinc(pContext);               
/* *************** */
	  /* call to ro248_decodingPm in ANY can not be done at the end    */
	  /* as in OCTET and BIT STRING because ANY is not skipped here    */

	  ed_control_err = ro0248_decodingPm((signed short int)sPmNumb, 
                   (signed short int)sAbstrSynId,
		   (signed long int)localPdvList.lEncodingChoice, 
                   pDefinedBy,
		   (rTro01_pfvObjectId) pTransSynName, 
                   (STOREHANDLE)hdl, 
                   (void NEAR *)pContext,
		   (unsigned long FAR *)pNode, 
                   (unsigned long int)ulNumb,
                   (signed int)1);            /* switch to next E/D PM  */
    /* Any Number is 1 (i.e .,  ROSE Data may be present   by kc on 21/5  */

	  if (ed_control_err) {
		ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
		return ((int)ed_control_err);
	  }
/* *************** */

	  sterr = asn1st_close(hdl,poszm,posz2);   /* Test for correct end */
	  poszm = poszms;                    /* of tag [0], set old endpos */
	  }
			/* end of decoding [0] ANY  */
	return(sterr);  /* return to PDV_list_dec */
	}
      else
	if(((*matchz)|32)==161 )    /* data-values encoded as OCTET STRING */
	  {
	  sterr = asn1st_octets_dec(hdl,poszm,(long *)&ulOffset,
		  (long *)&ulNumb);               /* Decoding OCTET STRING */
	  localPdvList.lEncodingChoice = RCro0n_ChoiceOctetString;
	  }
	else
	  if(((*matchz)|32)==162 )    /* data-values encoded as BIT STRING */
	    {
	    sterr = asn1st_bitst_dec(hdl,poszm,(long *)&ulOffset,
		    (long *)&ulNumb);               /* Decoding BIT STRING */
	    localPdvList.lEncodingChoice = RCro0o_ChoiceBitString;
	    }
      }
    else
      {
      sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
      }
    }
  if (sterr)
	return(sterr);

  ed_control_err = ro0416_decodePdvList (
				  (unsigned long int FAR * FAR *)&pTransSynName,
				  (signed short  int       FAR *)&sAbstrSynId,
				  (signed short  int       FAR *)&sPmNumb,
				  (rT0410_stPdvList        FAR *)&localPdvList,
				   pContext);
  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }
  pos_next = ro0076_stwhere(hdl);        /* store position of next element */
  sterr= ro0066_storepos(hdl,ulOffset);  /* set position to begin of
					    presentation-data-value        */

/* decoding OCTET or BIT STRING to local syntax */
  ed_control_err = ro0248_decodingPm((signed short int)sPmNumb, 
                   (signed short int)sAbstrSynId,
		   (signed long int)localPdvList.lEncodingChoice, 
                   (rTro76_stDefinedBy FAR *) pDefinedBy,
		   (rTro01_pfvObjectId) pTransSynName, hdl, pContext,
		   pNode, ulNumb,1);               /* switch to next E/D PM  */
    /* Any Number is 1 (i.e .,  ROSE Data may be present   by kc on 21/5  */

  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }
  sterr = ro0066_storepos(hdl,pos_next);   /* set position to next element */
  return(sterr);
}


/***************************************************************************/
/*                                                                         */
/* int PDV_list_cod(hdl,asn1_m,asn1_n,pNode,pContext)                              */
/*                                                                         */
/* STOREHANDLE hdl;       Handle to store containing ASN1 String           */
/* int asn1_m;            Flag for coding class of tag; If asn1_m == -1    */
/*                        predefined tag is coded.                         */
/* int asn1_n;            Number for tag                                   */
/* ulong * pNode ;        pointer to next node handle                      */
/* void *  pContext;      PPM context(pointer to)                          */
/*                                                                         */
/* Function for encoding a Pdv-list element                                */
/* Function created by MAVROS and changed                                  */
/* This function is called by a MAVROS created function.                   */
/*                                                                         */
/***************************************************************************/


int PASCAL PDV_list_cod(hdl,asn1_m,asn1_n,pNode,pContext)
STOREHANDLE hdl;
int asn1_m,asn1_n;
unsigned long FAR * pNode;
void NEAR * pContext;
  {
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)48);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
/*****  for osf warning : unused variable Baiju Chandru
    unsigned char t1 = 128;
*****/
/*  removed by kc on 14-11-93  for definite length encoding only   **** 
    ro0067_stputc(hdl,(unsigned char)t1); ***** */
    asn1st_deflenpos(hdl);  /* added by kc on 14-11-93 */
    PDV_list_ccod(hdl,pNode,pContext);
    /* removed by kc on 24-11-93  for definite length encoding only ***
    asn1st_regular_end(hdl, posz0); *** */
    asn1st_deflencod ( hdl,  posz0);    /* added by kc 24-11-93 */ 
    }
  return(ro0063_sterror(hdl));
}



/***************************************************************************/
/*                                                                         */
/* int PDV_list_ccod(hdl,pNode,pContext)                                   */
/*                                                                         */
/* STOREHANDLE hdl;       Handle to store containing ASN1 String           */
/* ulong * pNode ;        pointer to next node handle                      */
/* void *  pContext;      PPM context(pointer to)                          */
/*                                                                         */
/* Function for encoding body of Pdv-list element                          */
/* Function created by MAVROS and changed                                  */
/* This function is called by PDV_list_cod.                                */
/*                                                                         */
/***************************************************************************/


int PASCAL PDV_list_ccod(hdl, pNode, pContext)
STOREHANDLE hdl;
unsigned long int FAR * pNode;
void NEAR * pContext;
  {
  long pos_lenfield, pos_actual;
  int sterr;

  unsigned long int ulNumb; /* length of presentation-data-values in store */
  unsigned long int ulByteNumb;
  int UnusedBits;

  rT0410_stPdvList localPdvList;   /* Structure for local values of PDV-list */
  signed short int sPmNumb;
  unsigned long int FAR * pTransSynName;
  rTro76_stDefinedBy FAR * pDefinedBy = 0;

  signed short int ed_control_err;
  unsigned char p;
  sterr = ro0063_sterror(hdl);
  if (sterr) return sterr;

  ed_control_err = ro0411_encodePdvList (
				 (rT0410_stPdvList         FAR *)&localPdvList,
				 (unsigned long int  FAR * FAR *)&pTransSynName,
				 (signed short  int        FAR *)&sPmNumb,
				 pContext, *pNode);
			/* get control information for next E/D PM */
  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }

  if (localPdvList.oTransSynName[0] != 0)
    {
    ro0067_stputc(hdl,(unsigned char)6);                   /* Encoding tag */
    asn1st_oidcod(hdl,localPdvList.oTransSynName); /* Encoding OBJECT IDENTIFIER */
    }
  ro0067_stputc(hdl,(unsigned char)2);                     /* Encoding tag */
  asn1st_intcod(hdl,(int)localPdvList.lPresContId); /* Encoding presentation-contect-id */

  sterr = ro0063_sterror(hdl);
  if (sterr) return sterr;

  switch (CAST_FOR_SWITCH(localPdvList.lEncodingChoice)) /* Encoding CHOICE */
    {
    case RCro0m_ChoiceAny:
      p = 160; ro0067_stputc(hdl,(unsigned char)p);
	{
	long posz0 = ro0076_stwhere(hdl);
/*****  for osf warning : unused variable Baiju Chandru
	unsigned char t2 = 128; 
*****/
        /*  removed by kc on 14-11-93  for definite length encoding only   ****
	ro0067_stputc(hdl,(unsigned char)t2); **** */
        asn1st_deflenpos(hdl);  /* added by kc on 14-11-93 */ 

	ed_control_err = ro0247_encodingPm(sPmNumb,pDefinedBy,
					   (rTro01_pfvObjectId) pTransSynName,
					   hdl, pContext, pNode, &ulNumb,1);
						 /* switch to next E/D PM  */
    /* Any Number is 1 (i.e .,  ROSE Data may be present   by kc on 21/5  */
	  if (ed_control_err) {
	      ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	      return ((int)ed_control_err);
	}
        /* removed by kc on 24-11-93  for definite length encoding only *** 
	asn1st_regular_end(hdl, posz0); **** */
        asn1st_deflencod ( hdl,  posz0);    /* added by kc 24-11-93 */ 
	}
      break;


    case RCro0n_ChoiceOctetString:
      /* Encoding presentation-data-value as OCTET STRING */
      p = 129; ro0067_stputc(hdl,(unsigned char)p);           /* Encoding tag */
      ro0067_stputc(hdl,(unsigned char)132);  /* Length of length 4 bytes */

      pos_lenfield = ro0076_stwhere(hdl);      /* Position of length field */
      ro0069_stputs(hdl,(long)4);              /* 4 bytes free for length  */

      ed_control_err = ro0247_encodingPm(sPmNumb,pDefinedBy,
					 (rTro01_pfvObjectId) pTransSynName,
					 hdl, pContext, pNode, &ulNumb,1);
						 /* switch to next E/D PM  */
    /* Any Number is 1 (i.e .,  ROSE Data may be present   by kc on 21/5  */
      if (ed_control_err) {
	    ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	    return ((int)ed_control_err);
      }
      pos_actual = ro0076_stwhere(hdl);        /* actual Position of   */
					       /* store after encoding */
      if (ulNumb == 0)
      {  ulNumb = pos_actual - pos_lenfield - 4;
	   /* ?! kann ulNumb wirklich 0 sein ? */
      }
      ro0066_storepos(hdl,pos_lenfield);
      ro0067_stputc(hdl,(unsigned char) ((&ulNumb)[3]));
      ro0067_stputc(hdl,(unsigned char) ((&ulNumb)[2]));
      ro0067_stputc(hdl,(unsigned char) ((&ulNumb)[1]));
      ro0067_stputc(hdl,(unsigned char) ((&ulNumb)[0]));
					  /* encode length of OCTET STRING */
      ro0066_storepos(hdl,pos_actual);
      break;


    case RCro0o_ChoiceBitString:
      /* Encoding presentation-data-value as BIT STRING */
      p = 130; ro0067_stputc(hdl,(unsigned char)p);   /* Encode tag */
      ro0067_stputc(hdl, (unsigned char)132);  /* Length of length 4 bytes */

      pos_lenfield = ro0076_stwhere(hdl);      /* Position of length field */
      ro0069_stputs(hdl,(long)5);              /* 4 bytes free for length  */
					       /* 1 byte for unused bits   */

      ed_control_err = ro0247_encodingPm(sPmNumb,pDefinedBy,
					 (rTro01_pfvObjectId) pTransSynName,
					 hdl, pContext, pNode, &ulNumb,1);
						 /* switch to next E/D PM  */
    /* Any Number is 1 (i.e .,  ROSE Data may be present   by kc on 21/5  */
      if (ed_control_err) {
	    ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	    return ((int)ed_control_err);
      }

      pos_actual = ro0076_stwhere(hdl);        /* actual Position of   */
					       /* store after encoding */
      if (ulNumb == 0)
      {  ulNumb = (pos_actual - pos_lenfield - 5) * 8;
	 /* ?! kann ulNumb wirklich 0 sein ? dann muesste Berechnung auch 0 sein */
      }
      ulByteNumb = (ulNumb >> 3) + 1;
				   /* Length one byte more for unused bits */
      UnusedBits = (int)((ulByteNumb << 3) - ulNumb);

      ro0066_storepos(hdl,pos_lenfield);
      ro0067_stputc(hdl,(unsigned char) ((&ulByteNumb)[3]));
      ro0067_stputc(hdl,(unsigned char) ((&ulByteNumb)[2]));
      ro0067_stputc(hdl,(unsigned char) ((&ulByteNumb)[1]));
      ro0067_stputc(hdl,(unsigned char) ((&ulByteNumb)[0]));
					  /* encode length of OCTET STRING */
      ro0067_stputc(hdl,(unsigned char) UnusedBits);
      ro0066_storepos(hdl,pos_actual);
      break;
    default:
	 asn1st_errdecset(hdl,ASN1_ERR_CHOICE);
    }
  return(ro0063_sterror(hdl));
  }/* sterr ret 5 */



int PASCAL Simply_encoded_data_dec(hdl,poszm,pNode,pContext)
STOREHANDLE hdl;
unsigned long poszm;
unsigned long FAR * pNode;
void NEAR * pContext;
  {
  unsigned long int ulNumb; /* length of presentation-data-values in store */
  unsigned long int ulOffset; /* Offset of presentation-data-values in store */
  long pos_next;

  rT0411_stExternal localExtern; /* Structure for local values of EXTERNAL */
  signed short int sAbstrSynId;
  signed short int sPmNumb;
  unsigned long int FAR * pTransSynName;
  rTro76_stDefinedBy FAR * pDefinedBy = 0;

  signed short int ed_control_err;
  int sterr = ro0063_sterror(hdl);

  if (sterr) return sterr;

  /* get information for syntax change */

  ed_control_err = ro0415_decodeSimply (
				(unsigned long int FAR * FAR *)&pTransSynName,
				(signed  short int       FAR *)&sAbstrSynId,
				(signed  short int       FAR *)&sPmNumb,
				 pContext);
  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }

  sterr = asn1st_octets_dec(hdl,poszm,(long *)&ulOffset,
	      (long *)&ulNumb);               /* Decoding OCTET STRING */

  pos_next = ro0076_stwhere(hdl);        /* store position of next element */
  sterr= ro0066_storepos(hdl,ulOffset);  /* set position to begin of
					    presentation-data-value        */
  if (sterr) return(sterr);
/* decoding OCTET to local syntax */
  ed_control_err = ro0248_decodingPm(sPmNumb, sAbstrSynId,
				     localExtern.lEncodingChoice, pDefinedBy,
				     (rTro01_pfvObjectId)pTransSynName,
				     hdl, pContext, pNode, ulNumb,1);
						 /* switch to next E/D PM  */
    /* Any Number is 1 (i.e .,  ROSE Data may be present   by kc on 21/5  */

  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }

  sterr = ro0066_storepos(hdl,pos_next);   /* set position to next element */
  return(sterr);
  }


int PASCAL Simply_encoded_data_cod(hdl,asn1_m,asn1_n,pNode,pContext)
STOREHANDLE hdl;
int asn1_m,asn1_n;
unsigned long FAR * pNode;
void NEAR * pContext;
  {

  unsigned long int ulNumb; /* length of presentation-data-values in store */
  long pos_lenfield, pos_actual;
  signed short int sPmNumb;
  unsigned long int FAR * pTransSynName;
  rTro76_stDefinedBy  FAR * pDefinedBy = 0;

  signed short int ed_control_err;
  int sterr;

  sterr = ro0063_sterror(hdl);
  if (sterr) return sterr;

  ed_control_err = ro0410_encodeSimply (
				  (unsigned long int FAR * FAR *)&pTransSynName,
				  (signed  short int       FAR *)&sPmNumb,
				   pContext,
				  *pNode);
				/* get control information for next E/D PM */
  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }

  if (asn1_m == -1)  /* encoding tag */
    {
    ro0067_stputc(hdl,(unsigned char)4); /* predefined tag 4 */
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,0); /* new tag if implicit */


   /* Encoding OCTET STRING */
   ro0067_stputc(hdl, (unsigned char)132);  /* Length of length 4 bytes */

   pos_lenfield = ro0076_stwhere(hdl);      /* Position of length field */
   ro0069_stputs(hdl,(long)4);          /* Let 4 bytes free for length  */

   ed_control_err = ro0247_encodingPm(sPmNumb,pDefinedBy,
				      (rTro01_pfvObjectId) pTransSynName, hdl,
				      pContext, pNode, &ulNumb,1);
				     /* switch to next encoding Function  */
    /* Any Number is 1 (i.e .,  ROSE Data may be present   by kc on 21/5  */

   if (ed_control_err) {
	 ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	 return ((int)ed_control_err);
   }
   pos_actual = ro0076_stwhere(hdl);        /* actual Position of   */
					       /* store after encoding */
   if (ulNumb == 0)
   {  ulNumb = pos_actual - pos_lenfield - 4;
	/* ?! kann ulNumb wirklich 0 sein ? */
   }

   ro0066_storepos(hdl,pos_lenfield);
   ro0067_stputc(hdl,(unsigned char) ((&ulNumb)[3]));
   ro0067_stputc(hdl,(unsigned char) ((&ulNumb)[2]));
   ro0067_stputc(hdl,(unsigned char) ((&ulNumb)[1]));
   ro0067_stputc(hdl,(unsigned char) ((&ulNumb)[0]));
					  /* encode length of OCTET STRING */
   ro0066_storepos(hdl,pos_actual);

  return(ro0063_sterror(hdl));
  }


/***************************************************************************/
/*                                                                         */
/*  Interface presentation E/D functions to RTS E/D functions              */
/*  in X410 mode.                                                          */
/*  SET { COMPONENTS OF ... }  calls Components_of_set coding function     */
/*                                                                         */
/***************************************************************************/


int PASCAL Components_of_set_dec(
STOREHANDLE hdl,
unsigned long poszm,
unsigned long int FAR * pNode,
long lRtsPmNum,
void NEAR * pContext)
{
  int sterr = ro0063_sterror(hdl);
  if (!sterr) {
      sterr = ro0245_decodX410(RC0218_Rtse,0,lRtsPmNum,hdl,pContext,pNode);
  }
  return(sterr);
}




int PASCAL Components_of_set_cod(
STOREHANDLE hdl,
int asn1_m,
int asn1_n,
unsigned long int FAR * pNode,
long lRtsPmNum,
void NEAR * pContext)
{
  int sterr = ro0063_sterror(hdl);
  if (!sterr) {
      sterr = (int)ro0244_encodX410(RC0218_Rtse,(unsigned  long int FAR *)&hdl,
				     pContext,pNode);
  }
  return(sterr);
}
