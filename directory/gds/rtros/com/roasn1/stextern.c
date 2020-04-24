/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stextern.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:44  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:01  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:15:26  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:03:26  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:59  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:33  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:08  keutel
 * 	creation
 * 	[1994/03/21  13:16:03  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stextern.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:55 $" } ;
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
/* NAME         : st_extern.c                                               */
/*                                                                          */
/* AUTHOR       : Hubert Trieb                                              */
/* DATE         : 17.01.1992                                                */
/*                                                                          */
/* COMPONENT    : Encode/Decode functions for EXTERNAL and ANY as used      */
/*                in RTS.                                                   */
/*                                                                          */
/* PRD#/VERS.   : ST-MAVROS                                                          */
/*                                                                          */
/* DESCRIPTION  : This modul provides functions which                       */
/*                are the Interface between functions  created              */
/*                by MAVROS and the Encode/Decode Control functions         */
/*                These functions are called when within a pdu the types    */
/*                EXTERNAL or ANY are used.                                 */
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

static char *sccsid = " @(#) stextern.c 1.1 93/11/29  ST_MAVROS";

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
#include <rodpat1.h>
#include <roppm05.h>

#include <asn1st.h>

/***************************************************************************/
/*                                                                         */
/*              P R O V I D E D   G L O B A L   F U N C T I O N S          */
/*                                                                         */
/***************************************************************************/


/***************************************************************************/
/*                                                                         */
/* int ROS_External_dec(hdl,poszm,pNode,pContext)                          */
/*                                                                         */
/* STOREHANDLE hdl;       Handle to store containing ASN1 String           */
/* unsigned long poszm;   End of ASN1 String in store                      */
/* ulong * pNode;         pointer to next node handle                      */
/* void *  pContext;      PPM context(pointer to)                          */
/*                                                                         */
/* Function for decoding an EXTERNAL (used e.g. in ROSE)                   */
/* Function created by MAVROS and changed                                  */
/* This function is called by the RTSE decoding function when it needs     */
/* to decode the ROSE APDU embedded within the external.                   */
/*                                                                         */
/* NOTE: This function differs from the RTS_External_dec function in that  */
/*       it decodes the Tag and Length of the External also and then calls */
/*       the RTS_External_dec function. In this respect, it is more        */
/*       similar to the Association_info_dec function of ACSE (roacpmzz.c).*/
/*                                                                         */
/***************************************************************************/


int PASCAL ROS_External_dec(
STOREHANDLE hdl,
unsigned long poszm,
unsigned long FAR * pNode,
void  NEAR * pContext)
{
	int sterr = ro0063_sterror(hdl);
	unsigned char  matchz[6];

	{
		long posz2;
		long poszms = poszm;

		if ((sterr  == OK ) && 
		    ((ro0076_stwhere(hdl) + 1) < (long) poszm) && 
		   asn1st_getmatchz(hdl,poszm,(unsigned char FAR *)matchz))
		{
		     if( RTS_External_match(matchz))
		     {
			sterr=RTS_External_dec(hdl,poszm,pNode,pContext);
		     } else { 
			sterr = asn1st_errdecset(hdl,ASN1_ERR_WRSET); 
		     }
		}
		sterr = asn1st_close(hdl,poszm,posz2);
		poszm = poszms;
	}
	return(sterr);
}


/***************************************************************************/
/*                                                                         */
/* int RTS_Exernal_dec(hdl,poszm,pNode,pContext)                           */
/*                                                                         */
/* STOREHANDLE hdl;       Handle to store containing ASN1 String           */
/* unsigned long poszm;   End of ASN1 String in store                      */
/* ulong * pNode;         pointer to next node handle                      */
/* void *  pContext;      PPM context(pointer to)                          */
/*                                                                         */
/* Function for decoding an EXTERNAL (used e.g. in ACSE)                   */
/* Function created by MAVROS and changed                                  */
/* This function is called by a MAVROS created function.                   */
/*                                                                         */
/***************************************************************************/


int PASCAL RTS_External_dec(
STOREHANDLE hdl,
unsigned long poszm,
unsigned long FAR * pNode,
void NEAR * pContext)
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
    sterr = RTS_External_dseq(hdl,poszm,pNode,pContext);
    sterr = asn1st_close(hdl,poszm,posz2);
    poszm = poszms;
    }
  return(sterr);
}


/***************************************************************************/
/*                                                                         */
/* int RTS_External_dseq(hdl,poszm,pNode,pContext)                         */
/*                                                                         */
/* STOREHANDLE hdl;       Handle to store containing ASN1 String           */
/* unsigned long poszm;   End of ASN1 String in store                      */
/* ulong * pNode;         pointer to next node handle                      */
/* void * pContext;   PPM context(pointer to)                              */
/*                                                                         */
/* Function for decoding body of an EXTERNAL                               */
/*                                                                         */
/* Function created by MAVROS and changed                                  */
/* This function is called by the function RTS_External_dec.               */
/*                                                                         */
/***************************************************************************/

int PASCAL RTS_External_dseq(
STOREHANDLE hdl,
unsigned long int poszm,
unsigned long FAR * pNode,
void NEAR * pContext)
{
  unsigned char matchz[6]; /* for SEQ */
  long pos_next;

  unsigned long int ulNumb; /* length of presentation-data-values in store */
  unsigned long int ulOffset; /* Offset of presentation-data-values in store */

  rT0411_stExternal localExtern; /* Structure for local values of EXTERNAL */
  signed short int sAbstrSynId;
  signed short int sPmNumb;
  unsigned long int FAR * pTransSynName;
  rTro76_stDefinedBy FAR * pDefinedBy;

  signed short int ed_control_err;
  int sterr = ro0063_sterror(hdl);

  pDefinedBy = 0;
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */

  /* Test if direct-reference was sent */
  if( (sterr == OK) && (*matchz|32)==38 )
    {
	     /* decoding Transfer-syntax-name into local Object Identifier */
    localExtern.oDirectRef[0] = 1;                 /* Boolean for OPTIONAL */
    ro0070_stgetc(hdl);                                  /* Skip known tag */
    sterr = asn1st_oiddec(hdl,poszm,localExtern.oDirectRef);
    }
  else
    {                                      /* No direct-reference was sent */
    localExtern.oDirectRef[0] = 0;
    }


  matchz[0] = (char)ro0077_stgetcp(hdl);                    /* Reading Tag */

				    /* Test if indirect-reference was sent */
  if( (sterr == OK) && *matchz==2 )
    {
    localExtern.bIndirectRef = 1;
    ro0070_stgetc(hdl); /* Skip known tag */
    sterr = asn1st_intdec(hdl,poszm,&localExtern.lIndirectRef);
    }
  else
    {
    localExtern.bIndirectRef = 0;        /* no indirect-reference was sent */
    }

				 /* Test if data-value-descriptor was sent */
  matchz[0] = (char)ro0077_stgetcp(hdl); /* Reading Tag */
  if( (sterr == OK) && (*matchz|32)==39 )
    {
    localExtern.bObjectDescrip = 1;
    sterr = asn1st_octmov(hdl,poszm,localExtern.vObjectDescrip,(long *)&
    localExtern.ulObjectDescrip, (long) RCro0c_MaxLenObjDescExt,(int) 0);
    }
  else
    {
    localExtern.bObjectDescrip = 0; /* no data-value-descriptor was sent */
    }

/* Decoding the "encoding" of EXTERNAL */
    {
    asn1st_getmatchz(hdl,poszm,matchz);                     /* Reading Tag */
			       /* Test if encoding value was sent */
    if( (sterr == OK) &&  (  *matchz==160
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

	  localExtern.lEncodingChoice = RCro0m_ChoiceAny;
	  /* get information for syntax change */
	  ed_control_err = ro0417_decodeExternal (
				  (unsigned long int FAR * FAR *)&pTransSynName,
				  (signed  short int       FAR *)&sAbstrSynId,
				  (signed  short int       FAR *)&sPmNumb,
				  (rT0411_stExternal       FAR *)&localExtern,
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
	  else ulNumb = 0;
/* *************** */
	  /* call to ro248_decodingPm in ANY can not be done at the end    */
	  /* as in OCTET and BIT STRING because ANY is not skipped here    */

	  ed_control_err = ro0248_decodingPm(sPmNumb, sAbstrSynId,
				    localExtern.lEncodingChoice, pDefinedBy,
				    (rTro01_pfvObjectId) pTransSynName,
				    hdl, pContext, pNode, ulNumb,0);
						 /* switch to next E/D PM  */
	 /* Here Any Numbr is NULL  (i.e. Non Rose Data )  by kc on 21/5 */

	  if (ed_control_err) {
		ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
		return ((int)ed_control_err);
	  }
/* *************** */

	  sterr = asn1st_close(hdl,poszm,posz2);   /* Test for correct end */
	  poszm = poszms;                    /* of tag [0], set old endpos */
	  }
	return(sterr);  /* return to RTS_External_dec */
	}
      else
	if(((*matchz)|32)==161 )    /* data-values encoded as OCTET STRING */
	  {
	  sterr = asn1st_octets_dec(hdl,poszm,(long *)&ulOffset,
		  (long *)&ulNumb);               /* Decoding OCTET STRING */
	  localExtern.lEncodingChoice = RCro0n_ChoiceOctetString;
	  }
	else
	  if(((*matchz)|32)==162 )    /* data-values encoded as BIT STRING */
	    {
	    sterr = asn1st_bitst_dec(hdl,poszm,(long *)&ulOffset,
		    (long *)&ulNumb);               /* Decoding BIT STRING */
	    localExtern.lEncodingChoice = RCro0o_ChoiceBitString;
	    }
      }
    else
      {
      sterr = asn1st_errdecset(hdl,ASN1_ERR_MISS);
      }
    }
  if (sterr)
	return(sterr);

  pos_next = ro0076_stwhere(hdl);        /* store position of next element */
  sterr= ro0066_storepos(hdl,ulOffset);  /* set position to begin of
					    presentation-data-value        */

  /* get information for syntax change */
  ed_control_err = ro0417_decodeExternal (
				  (unsigned long int FAR * FAR *)&pTransSynName,
				  (signed short int        FAR *)&sAbstrSynId,
				  (signed short int        FAR *)&sPmNumb,
				  (rT0411_stExternal       FAR *)&localExtern,
				   pContext);
  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }

/* decoding OCTET or BIT STRING to local syntax */
  ed_control_err = ro0248_decodingPm(sPmNumb, sAbstrSynId,
				     localExtern.lEncodingChoice, pDefinedBy,
				     (rTro01_pfvObjectId)pTransSynName,
				     hdl, pContext, pNode, ulNumb,0);
						 /* switch to next E/D PM  */
  /* Any Nuber is NULL (i.e. Non Rose Data by kc on 21/5 */
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
/* int RTS_External_cod(hdl,asn1_m,asn1_n,pNode,pContext)                  */
/*                                                                         */
/* STOREHANDLE hdl;       Handle to store containing ASN1 String           */
/* int asn1_m, asn1_n     Parameter for encoding tag                       */
/* ulong * pNode ;        pointer to next node handle                      */
/* void *  pContext;      PPM context(pointer to)                          */
/*                                                                         */
/* Function for encoding body of an EXTERNAL                               */
/* Function created by MAVROS and changed                                  */
/* This function is called by a MAVROS created function.                   */
/*                                                                         */
/***************************************************************************/


int PASCAL RTS_External_cod(
STOREHANDLE hdl,
int asn1_m,
int asn1_n,
unsigned long FAR * pNode,
void NEAR * pContext)
{
  unsigned char p1;
  
  if (asn1_m == -1)
    {
    ro0067_stputc(hdl,(unsigned char)40);
    }
  else
    asn1st_type_cod(hdl,asn1_m,asn1_n,1);
 
    {
    long posz0 = ro0076_stwhere(hdl);
    p1 = 128;
/*  removed by kc on 14-11-93  for definite length encoding only   ****
    ro0067_stputc(hdl, (unsigned char)p1); ****** */
    asn1st_deflenpos(hdl);  /* added by kc on 14-11-93 */
    RTS_External_ccod(hdl,pNode,pContext);
/* removed by kc on 24-11-93  for definite length encoding only *** 
    asn1st_regular_end(hdl, posz0); **** */
    asn1st_deflencod ( hdl,  posz0);    /* added by kc 24-11-93 */
    }
  return(ro0063_sterror(hdl));
}



/***************************************************************************/
/*                                                                         */
/* int RTS_External_ccod(hdl,pNode,pContext)                               */
/*                                                                         */
/* STOREHANDLE hdl;       Handle to store containing ASN1 String           */
/* ulong * pNode ;        pointer to next node handle                      */
/* void *  pContext;      PPM context(pointer to)                          */
/*                                                                         */
/* Function for encoding body of an EXTERNAL                               */
/* Function created by MAVROS and changed                                  */
/* This function is called by RTS_External_cod.                            */
/*                                                                         */
/***************************************************************************/


int PASCAL RTS_External_ccod(
STOREHANDLE hdl,
unsigned long FAR * pNode,
void NEAR * pContext)
{
  long pos_lenfield, pos_actual;

  unsigned long int ulNumb=NULL; /* length of presentation-data-values in store */ /* initialised to 0 by kc on 25/5 */
  unsigned long int ulByteNumb;
  int UnusedBits;
  unsigned char p3;
  rT0411_stExternal localExtern; /* Structure for local values of EXTERNAL */
  signed short int sPmNumb;
  unsigned long int FAR * pTransSynName;
  rTro76_stDefinedBy FAR * pDefinedBy;

  signed short int ed_control_err;
  int sterr;
  unsigned char p1;

  sterr = ro0063_sterror(hdl);
  if (sterr) return sterr;

  pDefinedBy = 0;
  ed_control_err = ro0412_encodeExternal (
				(rT0411_stExternal        FAR *)&localExtern,
				(unsigned long int  FAR * FAR *)&pTransSynName,
				(signed short int         FAR *)&sPmNumb,
				 pContext,
				*pNode);
			/* get control information for next E/D PM */
  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }

  if (localExtern.oDirectRef[0] != 0)
    {
    ro0067_stputc(hdl,(unsigned char)6);                   /* Encoding tag */
    asn1st_oidcod(hdl,localExtern.oDirectRef); /* Encoding OBJECT IDENTIFIER */
    }
  if (localExtern.bIndirectRef != 0)
    {
    ro0067_stputc(hdl,(unsigned char)2);                   /* Encoding tag */
    asn1st_intcod(hdl,(int)localExtern.lIndirectRef);
    }                                  /* Encoding presentation-contect-id */


  if (localExtern.bObjectDescrip != 0)
  {
     ro0067_stputc(hdl,(unsigned char)7);
 
     if ( RCro0c_MaxLenObjDescExt < localExtern.ulObjectDescrip ){
	 asn1st_octets_cod(hdl,localExtern.vObjectDescrip,
			  (long)RCro0c_MaxLenObjDescExt);
     } else {
	 asn1st_octets_cod(hdl,localExtern.vObjectDescrip,
			  (long)localExtern.ulObjectDescrip);
     }
  }

  sterr = ro0063_sterror(hdl);
  if (sterr) return sterr;
  switch (CAST_FOR_SWITCH(localExtern.lEncodingChoice)) /* Encoding CHOICE */
    {
    case RCro0m_ChoiceAny:
      p1 = 160;
      ro0067_stputc(hdl,(unsigned char)p1);
	{
	long posz0 = ro0076_stwhere(hdl);
/*  removed by kc on 8/6 for definite length encoding only   */
/*      ro0067_stputc(hdl, (unsigned char)128);  */
	asn1st_deflenpos(hdl);  /* added by kc on 8/6 */

	ed_control_err = ro0247_encodingPm(sPmNumb,pDefinedBy,
				    (rTro01_pfvObjectId)pTransSynName,
				    hdl, pContext, pNode, &ulNumb,0);
						 /* switch to next E/D PM  */
  /* Any Nuber is NULL (i.e. Non Rose Data by kc on 21/5 */
	if (ed_control_err) {
	      ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	      return ((int)ed_control_err);
	}
/* removed by kc on 8/6 for definite length encoding only  */
/*      asn1st_regular_end(hdl, posz0);        */
	asn1st_deflencod ( hdl,  posz0);    /* added by kc 8/6 */
	}
      break;


    case RCro0n_ChoiceOctetString:
      /* Encoding presentation-data-value as OCTET STRING */
      p3 = 129; 
      ro0067_stputc(hdl,(unsigned char)p3);               /* Encoding tag */
   /*   ro0067_stputc(hdl, (unsigned char)122);  */
					   /* Length of length 4 bytes */
     /* the above line is commented by kc on 25/5 instead the following
	line is added . 122 -> 132 */
      p3 = 132;
      ro0067_stputc(hdl, (unsigned char)p3);  

      pos_lenfield = ro0076_stwhere(hdl);      /* Position of length field */
      ro0069_stputs(hdl,(long)4);          /* Let 4 bytes free for length  */

      ed_control_err = ro0247_encodingPm(sPmNumb, pDefinedBy,
				  (rTro01_pfvObjectId)pTransSynName,
				   hdl, pContext, pNode, &ulNumb,0);
				     /* switch to next encodeing Function  */
  /* Any Nuber is NULL (i.e. Non Rose Data by kc on 21/5 */

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
/*
      ro0067_stputc(hdl,(unsigned char) ((&ulNumb)[3]));
      ro0067_stputc(hdl,(unsigned char) ((&ulNumb)[2]));
      ro0067_stputc(hdl,(unsigned char) ((&ulNumb)[1]));
      ro0067_stputc(hdl,(unsigned char) ((&ulNumb)[0]));  by kc on 25/5  */

      ro0067_stputc(hdl,(unsigned char) *(((char *)&ulNumb)+3));
      ro0067_stputc(hdl,(unsigned char) *(((char *)&ulNumb)+2));
      ro0067_stputc(hdl,(unsigned char) *(((char *)&ulNumb)+1));
      ro0067_stputc(hdl,(unsigned char) *(((char *)&ulNumb)+0));  
					  /* encode length of OCTET STRING */
      ro0066_storepos(hdl,pos_actual);
      break;

    case RCro0o_ChoiceBitString:
      /* Encoding presentation-data-value as BIT STRING */
      p3 = 130;
      ro0067_stputc(hdl,(unsigned char)p3);   /* Encode tag */
      ro0067_stputc(hdl, (unsigned char)122);  /* Length of length 4 bytes */

      pos_lenfield = ro0076_stwhere(hdl);      /* Position of length field */
      ro0069_stputs(hdl,(long)5);              /* 4 bytes free for length  */
					       /* 1 byte for unused bits   */

      ed_control_err = ro0247_encodingPm(sPmNumb, pDefinedBy,
				    (rTro01_pfvObjectId)pTransSynName,
				    hdl, pContext, pNode, &ulNumb,0);
				     /* switch to next encodeing Function  */
  /* Any Nuber is NULL (i.e. Non Rose Data by kc on 21/5 */
      if (ed_control_err) {
	    ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	    return ((int)ed_control_err);
      }

      pos_actual = ro0076_stwhere(hdl);        /* actual Position of   */
					       /* store after encoding */

      if (ulNumb == 0)
      {  ulNumb = (pos_actual - pos_lenfield - 5) * 8;
	   /* ?! kann ulNumb wirklich 0 sein ? */
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
}




int PASCAL RTS_any_cod(
STOREHANDLE hdl,
int m,
int n,
unsigned long int FAR * pNode,
int sAnyNumb,    /* not used when encoding */
void NEAR * pContext)
{
  int sterr;
  signed short int ed_control_err;

  rTro76_stDefinedBy FAR * pDefinedBy;
  unsigned long int FAR * pTransSynName;
  signed short int sPmNumb;
  unsigned long int ulNumb; /* length of encoded ANY data */

  sterr = ro0063_sterror(hdl);
  if (sterr) return sterr;

  pDefinedBy = 0;
  ed_control_err = ro0413_encodeAny (
				(rTro76_stDefinedBy FAR * FAR *)&pDefinedBy,
				(unsigned long int  FAR * FAR *)&pTransSynName,
				(signed short  int        FAR *)&sPmNumb,
				 pContext,
				*pNode);
			/* get control information for next E/D PM */
  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }
  ed_control_err = ro0247_encodingPm(sPmNumb, pDefinedBy,
				     (rTro01_pfvObjectId)pTransSynName,
				     hdl, pContext, pNode, &ulNumb,0);
				     /* switch to next encodeing Function  */
  /* Any Nuber is NULL (i.e. Non Rose Data by kc on 21/5 */
  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }
  return(ro0063_sterror(hdl));
}



int PASCAL RTS_any_dec(
STOREHANDLE hdl,
unsigned long int   poszm,
unsigned long int FAR * pNode,
int sAnyNumb,
void NEAR * pContext)
{
  int sterr;
  signed short int ed_control_err;

  rTro76_stDefinedBy  FAR * pDefinedBy;
  unsigned long int FAR * pTransSynName;
  signed short int sAbstrSynId;
  signed short int sPmNumb;
  unsigned long int ulNumb; /* maximum length endposition  encoded ANY data */
  long  lEncodingChoice = RCro0m_ChoiceAny;


  pDefinedBy = 0;
  ulNumb = poszm - ro0076_stwhere(hdl);  /* maximum length of ANY data */
  /* comment removed by kc on 19/5 to pass Bin User data */
/*  ulNumb = 0;by kc for bin. data */ /* real length of of ANY data not known */

  sterr = ro0063_sterror(hdl);
  if (sterr)
	return (sterr);

  ed_control_err = ro0418_decodeAny (
				 (rTro76_stDefinedBy FAR * FAR *)&pDefinedBy,
				 (unsigned long int  FAR * FAR *)&pTransSynName,
				 (signed short  int        FAR *)&sAbstrSynId,
				 (signed short  int        FAR *)&sPmNumb,
				 (signed short  )              sAnyNumb,
				  pContext);
			/* get control information for next E/D PM */
  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }

  ed_control_err = ro0248_decodingPm(sPmNumb, sAbstrSynId,
				     lEncodingChoice,pDefinedBy,
				     (rTro01_pfvObjectId)pTransSynName,
				     hdl, pContext, pNode, ulNumb,0);
						 /* switch to next E/D PM  */
  /* Any Nuber is NULL (i.e. Non Rose Data by kc on 21/5 */

  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
  }
  return ((int)ed_control_err);
}
