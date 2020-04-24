/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26mhsis.c,v $
 * Revision 1.1.125.2  1996/02/18  19:45:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:51  marty]
 *
 * Revision 1.1.125.1  1995/12/08  15:46:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:15  root]
 * 
 * Revision 1.1.123.2  1994/06/09  18:44:55  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:42  devsrc]
 * 
 * Revision 1.1.123.1  1994/02/22  16:06:07  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:14:50  marrek]
 * 
 * Revision 1.1.121.2  1993/10/14  17:16:35  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:10:57  keutel]
 * 
 * Revision 1.1.121.1  1993/10/13  17:30:56  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:46:26  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26mhsis.c,v $ $Revision: 1.1.125.2 $ $Date: 1996/02/18 19:45:34 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : d26mhsis                                            */
/*                                                                    */
/* AUTHOR       : E. Kraemer, AP 11                                   */
/* DATE         : 16.09.93                                            */
/*                                                                    */
/* COMPONENT    : D2                                                  */
/*                                                                    */
/* DOC.-NR.     : D2-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*                                                                    */
/* DESCRIPTION  : The module contains functions, that load the MHS    */
/*                attributes from C-ISAM records and store them.      */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*      1.0 | 16.09.93| Birth                          | ek |         */
/*datoff **************************************************************/

/*--------------------------------------------------------------------*/
/*                        I N C L U D E S                             */
/*--------------------------------------------------------------------*/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

/*****  external Includes    *****/

#include <dce/d27util.h>

/*****  internal Includes    *****/

#include <d21spat.h>
#include <d26isam.h>

/*--------------------------------------------------------------------*/
/*                        D E F I N E S                               */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                 T Y P E -  D E F I N I T I O N S                   */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                 D E C L A R A T I O N S        DATA                */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*--------------------------------------------------------------------*/

static signed16 d26_im15_ld_str_list(char *str_list, char *record,
	signed16 len);
static signed16 d26_im25_st_str_list(char *str_list, char *record,
	signed16 len);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_im11_ld_or_address                                */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        16.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the OR address attribute value   */
/*              in APDU format from C-ISAM format.                    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_im11_ld_or_address(
			/* OUT   -  where the OR-address value has    */
    char     *or_address,           /* to be written to               */
    char     *record,   /* IN    -  Record to read data from.         */
    signed16 len)       /* IN    -  Length of data in record.         */

{                               /* d26_im11_ld_or_address             */

  D21_OR_address_part *comp;    /* pointer to OR-address component    */
  unsigned16 tag;               /* tag of OR address component        */
  signed16   clen;              /* length of OR address component     */
  signed16   rlen;              /* length of data in record           */
  char       *r_begin;          /* start of data in record            */
  char       *oa_begin;         /* start of OR-address                */

  r_begin  = record;
  oa_begin = or_address;

				/* load address parts                 */
  while                         /* still data to load                 */
       (record - r_begin < len)
  {                             /* load tag                           */
    comp     = (D21_OR_address_part *)or_address;
    tag      = ldint(record);
    record  += INTSIZE;
    rlen     = ldint(record);
    record  += INTSIZE;
    comp->d21_ora_tag = tag;
    comp->d21_ora_fil = 0;
    or_address += sizeof(D21_OR_address_part);
    switch(tag)
    {
      case D21_ORA_ADMD_N:
      case D21_ORA_COM_N:
      case D21_ORA_CTRY_N:
      case D21_ORA_DOM_T1:
      case D21_ORA_DOM_T2:
      case D21_ORA_DOM_T3:
      case D21_ORA_DOM_T4:
      case D21_ORA_DOM_V1:
      case D21_ORA_DOM_V2:
      case D21_ORA_DOM_V3:
      case D21_ORA_DOM_V4:
      case D21_ORA_GEN:
      case D21_ORA_GIV_N:
      case D21_ORA_INIT:
      case D21_ORA_ORG_N:
      case D21_ORA_ORGU_N1:
      case D21_ORA_ORGU_N2:
      case D21_ORA_ORGU_N3:
      case D21_ORA_ORGU_N4:
      case D21_ORA_P_A_D:
      case D21_ORA_P_CODE:
      case D21_ORA_P_CTRY:
      case D21_ORA_P_D_P_N:
      case D21_ORA_P_G_D_A:
      case D21_ORA_P_LOC:
      case D21_ORA_P_O_B_N:
      case D21_ORA_P_O_N:
      case D21_ORA_P_O_NUM:
      case D21_ORA_P_ORG_N:
      case D21_ORA_P_P_D:
      case D21_ORA_P_P_N:
      case D21_ORA_P_S_A:
      case D21_ORA_PRMD_N:
      case D21_ORA_SURNAME:
	      /*---------------------------------------------------*/
	      /* string lists                                      */
	      /*---------------------------------------------------*/
	clen = d26_im15_ld_str_list(or_address,record,rlen);
	comp->d21_ora_len = clen;
	comp->d21_ora_off = D2_ALIGN(comp->d21_ora_len);
	or_address       += comp->d21_ora_off;
	record += rlen;
	break;

      case D21_ORA_ISDN_N:
      case D21_ORA_ISDN_S:
      case D21_ORA_NUM_ID:
      case D21_ORA_X121_A:
	      /*---------------------------------------------------*/
	      /* single numeric string                             */
	      /*---------------------------------------------------*/
      case D21_ORA_P_A_F:
	      /*---------------------------------------------------*/
	      /* single T61_string                                 */
	      /*---------------------------------------------------*/
      case D21_ORA_P_D_S_N:
      case D21_ORA_T_ID:
      case D21_ORA_P_A_L:
	      /*---------------------------------------------------*/
	      /* single printable string                           */
	      /*---------------------------------------------------*/
	memcpy(or_address,record,rlen);
	comp->d21_ora_len = rlen;
	comp->d21_ora_off = D2_ALIGN(comp->d21_ora_len);
	or_address       += rlen;
	memset(or_address,D2_EOS,comp->d21_ora_off - rlen);
	or_address       += comp->d21_ora_off - rlen;
	record += rlen;
	break;
      case D21_ORA_PRES_A:
	      /*---------------------------------------------------*/
	      /* d2_presentation_addr (Psap_string)                */
	      /*---------------------------------------------------*/
	clen = d26_is14_ld_psap_addr(or_address,record,rlen);
	comp->d21_ora_len = clen;
	comp->d21_ora_off = D2_ALIGN(comp->d21_ora_len);
	or_address       += clen;
	memset(or_address,D2_EOS,comp->d21_ora_off - clen);
	or_address       += comp->d21_ora_off - clen;
	record += rlen;
	break;
      case D21_ORA_T_T:
	      /*---------------------------------------------------*/
	      /* d2_terminal_type (Int)                            */
	      /*---------------------------------------------------*/
	*(signed32 *)or_address = ldlong(record);
	comp->d21_ora_len  = sizeof(signed32);
	comp->d21_ora_off  = D2_ALIGN(comp->d21_ora_len);
	or_address        += comp->d21_ora_off;
	record            += LONGSIZE;
	break;
      default:
	break;
    }
  }

  return(or_address - oa_begin);

}                               /* d26_im11_ld_or_address             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_im12_ld_or_name                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        17.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the OR name attribute value      */
/*              in APDU format from C-ISAM format.                    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_im12_ld_or_name(
			/* OUT   -  where the OR-name value has to be */
    char     *or_name,              /* written to                     */
    char     *record,   /* IN    -  Record to read data from.         */
    signed16 len)       /* IN    -  Length of data in record.         */

{                               /* d26_im12_ld_or_name                */

  D21_OR_name *comp;            /* pointer to OR-name component       */
  unsigned16  tag;              /* tag of OR name component           */
  signed16    clen;             /* length of OR name component        */
  signed16    rlen;             /* length of data in record           */
  char        *r_begin;         /* start of data to load              */
  char        *on_begin;        /* start of OR-name to write          */

  r_begin  = record;
  on_begin = or_name;

				/* load parts of OR-name              */
  while                         /* still data to load                 */
       (record - r_begin < len)
  {                             /* load tag                           */
    comp     = (D21_OR_name *)or_name;
    tag      = ldint(record);
    record  += INTSIZE;
    rlen     = ldint(record);
    record  += INTSIZE;
    comp->d21_orn_tag = tag;
    comp->d21_orn_fil = 0;
    or_name += sizeof(D21_OR_name);
    switch(tag)
    {
      case D21_ORN_DN:
	      /*---------------------------------------------------*/
	      /* Distinguished name                                */
	      /*---------------------------------------------------*/
	memcpy(or_name,record,rlen);
	record += rlen;
	comp->d21_orn_len = rlen;
	comp->d21_orn_off = D2_ALIGN(rlen);
	or_name += rlen;
	memset(or_name,D2_EOS,comp->d21_orn_off - rlen);
	or_name += comp->d21_orn_off - rlen;
	break;
      case D21_ORN_ORA:
	      /*---------------------------------------------------*/
	      /* OR address                                        */
	      /*---------------------------------------------------*/
	clen = d26_im11_ld_or_address(or_name,record,rlen);
	comp->d21_orn_len = clen;
	comp->d21_orn_off = D2_ALIGN(clen);
	record  += rlen;
	or_name += clen;
	memset(or_name,D2_EOS,comp->d21_orn_off - clen);
	or_name += comp->d21_orn_off - clen;
	break;
    }
  }

  return(or_name - on_begin);

}                               /* d26_im12_ld_or_name                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_im13_ld_dl_sub_perm                               */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        17.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the DL-submit permission         */
/*              attribute value in APDU format from C-ISAM format.    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_im13_ld_dl_sub_perm(
			/* OUT   -  where the DL-submit permission has*/
    char     *dl_sub_perm,          /* to be written to               */
    char     *record,   /* IN    -  Record to read data from.         */
    signed16 len)       /* IN    -  Length of data in record.         */

{                               /*  d26_im13_ld_dl_sub_perm           */

  D21_DL_sub_perm *comp;        /* pointer to DL-submit permission    */
				/* component                          */
  unsigned16 tag;               /* tag of OR name component           */
  signed16   rlen;              /* length of OR name component in     */
				/* record                             */
  signed16   clen;              /* length of OR name component        */
  char       *dlsp_begin;       /* begin of DL-submit permission      */

  dlsp_begin = dl_sub_perm;

  comp     = (D21_DL_sub_perm *)dl_sub_perm;
  tag      = ldint(record);
  record  += INTSIZE;
  rlen     = ldint(record);
  record  += INTSIZE;
  comp->d21_dlsp_tag = tag;
  comp->d21_dlsp_fil = 0;
  dl_sub_perm += sizeof(D21_DL_sub_perm);

  switch(tag)
  {
    case D21_DLSP_INDIV:
    case D21_DLSP_PATT:
    case D21_DLSP_MEM_DL:
	    /*---------------------------------------------------*/
	    /* OR-names                                          */
	    /*---------------------------------------------------*/
      clen = d26_im12_ld_or_name(dl_sub_perm,record,rlen);
      comp->d21_dlsp_len = clen;
      comp->d21_dlsp_off = D2_ALIGN(clen);
      record += rlen;
      dl_sub_perm += clen;
      memset(dl_sub_perm,D2_EOS,comp->d21_dlsp_off - clen);
      dl_sub_perm += comp->d21_dlsp_off - clen;
      break;
    case D21_DLSP_MEM_G:
	    /*---------------------------------------------------*/
	    /* Distinguished name                                */
	    /*---------------------------------------------------*/
      memcpy(dl_sub_perm,record,rlen);
      record += rlen;
      comp->d21_dlsp_len = rlen;
      comp->d21_dlsp_off = D2_ALIGN(rlen);
      dl_sub_perm       += rlen;
      memset(dl_sub_perm,D2_EOS,comp->d21_dlsp_off - rlen);
      dl_sub_perm       += comp->d21_dlsp_off - rlen;
      break;
  }

  return(dl_sub_perm - dlsp_begin);

}                               /* d26_im13_ld_dl_sub_perm            */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_im15_ld_str_list                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        16.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function reads the string list components of     */
/*              an OR-address from C-ISAM format and converts to APDU.*/
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_im15_ld_str_list(
			/* OUT   -  where the stringlist component has*/
    char     *str_list,             /* to be written to               */
    char     *record,   /* IN    -  Record to read data from.         */
    signed16 len)       /* IN    -  Length of data in record.         */

{                               /* d26_im15_ld_str_list               */

  signed16 slen;                /* length of single string            */
  signed16 soff;                /* offset of single string            */
  char     *r_begin;            /* start of data to load              */
  char     *sl_begin;           /* start of string list               */

  r_begin  = record;
  sl_begin = str_list;

				/* load address parts                 */
  while                         /* still data to load                 */
       (record - r_begin < len)
  {                             /* load length and representation     */
    ((D21_str_list *)str_list)->d21_strl_len = slen = ldint(record);
    record    += INTSIZE;
    soff       = D2_ALIGN(slen);
    ((D21_str_list *)str_list)->d21_strl_off = soff;
    ((D21_str_list *)str_list)->d21_strl_rep = ldint(record);
    ((D21_str_list *)str_list)->d21_strl_fil = 0;
    record    += INTSIZE;
    str_list  += sizeof(D21_str_list);
    memcpy(str_list,record,slen);
    str_list  += slen;
    memset(str_list,D2_EOS,soff - slen);
    str_list  += soff - slen;
    record    += slen;
  }

  return(str_list - sl_begin);

}                               /* d26_im15_ld_str_list               */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_im21_st_or_address                                */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        17.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function stores the OR address attribute value   */
/*              in C-ISAM format from APDU format.                    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_im21_st_or_address(
			/* OUT   -  OR-address value to be written    */
    char     *or_address,
    char     *record,   /* IN    -  Record to write data to.          */
    signed16 len)       /* IN    -  Length of data to write.          */

{                               /* d26_im21_st_or_address             */

  char       *plen;             /* pointer to place of length in      */
				/* record                             */
  unsigned16 tag;               /* tag of OR address component        */
  signed16   rlen;              /* length of data in record           */
  signed16   clen;              /* length of OR address component     */
  signed16   coff;              /* offset of OR address component     */
  char       *oa_begin;         /* start of data to store             */
  char       *r_begin;          /* start of data in record            */

  oa_begin = or_address;
  r_begin  = record;

				/* store address parts                */
  while                         /* still data to store                */
       (or_address - oa_begin < len)
  {                             /* store tag                          */
    tag  = ((D21_OR_address_part *)or_address)->d21_ora_tag;
    clen = ((D21_OR_address_part *)or_address)->d21_ora_len;
    coff = ((D21_OR_address_part *)or_address)->d21_ora_off;
    stint(tag,record);
    record += INTSIZE;
    plen    = record;
    record += INTSIZE;
    or_address += sizeof(D21_OR_address_part);
    switch(tag)
    {
      case D21_ORA_ADMD_N:
      case D21_ORA_COM_N:
      case D21_ORA_CTRY_N:
      case D21_ORA_DOM_T1:
      case D21_ORA_DOM_T2:
      case D21_ORA_DOM_T3:
      case D21_ORA_DOM_T4:
      case D21_ORA_DOM_V1:
      case D21_ORA_DOM_V2:
      case D21_ORA_DOM_V3:
      case D21_ORA_DOM_V4:
      case D21_ORA_GEN:
      case D21_ORA_GIV_N:
      case D21_ORA_INIT:
      case D21_ORA_ORG_N:
      case D21_ORA_ORGU_N1:
      case D21_ORA_ORGU_N2:
      case D21_ORA_ORGU_N3:
      case D21_ORA_ORGU_N4:
      case D21_ORA_P_A_D:
      case D21_ORA_P_CODE:
      case D21_ORA_P_CTRY:
      case D21_ORA_P_D_P_N:
      case D21_ORA_P_G_D_A:
      case D21_ORA_P_LOC:
      case D21_ORA_P_O_B_N:
      case D21_ORA_P_O_N:
      case D21_ORA_P_O_NUM:
      case D21_ORA_P_ORG_N:
      case D21_ORA_P_P_D:
      case D21_ORA_P_P_N:
      case D21_ORA_P_S_A:
      case D21_ORA_PRMD_N:
      case D21_ORA_SURNAME:
	      /*---------------------------------------------------*/
	      /* string lists                                      */
	      /*---------------------------------------------------*/
	rlen = d26_im25_st_str_list(or_address,record,clen);
	stint(rlen,plen);
	or_address += coff;
	record     += rlen;
	break;

      case D21_ORA_ISDN_N:
      case D21_ORA_ISDN_S:
      case D21_ORA_NUM_ID:
      case D21_ORA_X121_A:
	      /*---------------------------------------------------*/
	      /* single numeric string                             */
	      /*---------------------------------------------------*/
      case D21_ORA_P_A_F:
	      /*---------------------------------------------------*/
	      /* single T61_string                                 */
	      /*---------------------------------------------------*/
      case D21_ORA_P_D_S_N:
      case D21_ORA_T_ID:
      case D21_ORA_P_A_L:
	      /*---------------------------------------------------*/
	      /* single printable string                           */
	      /*---------------------------------------------------*/
	memcpy(record,or_address,clen);
	stint(clen,plen);
	or_address += coff;
	record     += clen;
	break;
      case D21_ORA_PRES_A:
	      /*---------------------------------------------------*/
	      /* d2_presentation_addr (Psap_string)                */
	      /*---------------------------------------------------*/
	rlen = d26_is24_st_psap_addr(or_address,record,clen);
	stint(rlen,plen);
	or_address += coff;
	record     += rlen;
	break;
      case D21_ORA_T_T:
	      /*---------------------------------------------------*/
	      /* d2_terminal_type (Int)                            */
	      /*---------------------------------------------------*/
	stlong(*(signed32 *)or_address,record);
	stint(LONGSIZE,plen);
	or_address += coff;
	record     += LONGSIZE;
	break;
    }
  }

  return(record - r_begin);

}                               /* d26_im21_st_or_address             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_im22_st_or_name                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        17.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the OR name attribute value      */
/*              in C-ISAM format from APDU format.                    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_im22_st_or_name(
    char     *or_name,  /* OUT   -  OR-name value to be written       */
    char     *record,   /* IN    -  Record to write data to.          */
    signed16 len)       /* IN    -  Length of data to write.          */

{                               /* d26_im22_st_or_name                */

  char       *plen;             /* pointer to place of length in      */
				/* record                             */
  unsigned16 tag;               /* tag of OR name component           */
  signed16   rlen;              /* length of data in record           */
  signed16   clen;              /* length of OR name component        */
  signed16   coff;              /* offset of OR name component        */
  char       *or_begin;         /* start of data to store             */
  char       *r_begin;          /* start of record                    */

  or_begin = or_name;
  r_begin  = record;

				/* store address parts                */
  while                         /* still data to store                */
       (or_name - or_begin < len)
  {                             /* store tag                          */
    tag     = ((D21_OR_name *)or_name)->d21_orn_tag;
    clen    = ((D21_OR_name *)or_name)->d21_orn_len;
    coff    = ((D21_OR_name *)or_name)->d21_orn_off;
    stint(tag,record);
    record += INTSIZE;
    plen    = record;
    record += INTSIZE;
    or_name += sizeof(D21_OR_name);
    switch(tag)
    {
      case D21_ORN_DN:
	      /*---------------------------------------------------*/
	      /* Distinguished name                                */
	      /*---------------------------------------------------*/
	memcpy(record,or_name,clen);
	record += clen;
	stint(clen,plen);
	or_name += coff;
	break;
      case D21_ORN_ORA:
	      /*---------------------------------------------------*/
	      /* OR address                                        */
	      /*---------------------------------------------------*/
	rlen = d26_im21_st_or_address(or_name,record,clen);
	stint(rlen,plen);
	record  += rlen;
	or_name += coff;
	break;
    }
  }

  return(record - r_begin);

}                               /* d26_im22_st_or_name                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_im23_st_dl_sub_perm                               */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        17.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the DL-submit permission         */
/*              attribute value in C-ISAM format from APDU format.    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_im23_st_dl_sub_perm(
			/* OUT   -  DL-submit permission to be written*/
    char     *dl_sub_perm,
    char     *record,   /* IN    -  Record to write data to.          */
    signed16 len)       /* IN    -  Length of data to write.          */

{                               /* d26_im23_st_dl_sub_perm            */

  char       *plen;             /* pointer to place of length in      */
				/* record                             */
  unsigned16 tag;               /* tag of ...                         */
  signed16   rlen;              /* length of data in record           */
  signed16   clen;              /* length of ...                      */
  signed16   coff;              /* offset of ...                      */
				/* ... DL-submit permission component */
  char       *r_begin;          /* start of data in record            */

  r_begin = record;

  tag     = ((D21_DL_sub_perm *)dl_sub_perm)->d21_dlsp_tag;
  clen    = ((D21_DL_sub_perm *)dl_sub_perm)->d21_dlsp_len;
  coff    = ((D21_DL_sub_perm *)dl_sub_perm)->d21_dlsp_off;
  stint(tag,record);
  record += INTSIZE;
  plen    = record;
  record += INTSIZE;
  dl_sub_perm += sizeof(D21_DL_sub_perm);
  switch(tag)
  {
    case D21_DLSP_INDIV:
    case D21_DLSP_PATT:
    case D21_DLSP_MEM_DL:
	    /*---------------------------------------------------*/
	    /* OR-names                                          */
	    /*---------------------------------------------------*/
      rlen = d26_im22_st_or_name(dl_sub_perm,record,clen);
      stint(rlen,plen);
      record += rlen;
      dl_sub_perm += coff;
      break;
    case D21_DLSP_MEM_G:
	    /*---------------------------------------------------*/
	    /* Distinguished name                                */
	    /*---------------------------------------------------*/
      memcpy(record,dl_sub_perm,clen);
      record      += clen;
      dl_sub_perm += coff;
      stint(clen,plen);
      break;
  }

  return(record - r_begin);

}                               /* d26_im23_st_dl_sub_perm            */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_im25_st_str_list                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        17.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function reads the string list components of     */
/*              an OR-address from APDU format and converts to C-ISAM.*/
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_im25_st_str_list(
    char     *str_list, /* OUT   -  string list component to write    */
    char     *record,   /* IN    -  Record to write data to.          */
    signed16 len)       /* IN    -  Length of data to write.          */

{                               /* d26_im25_st_str_list               */

  signed16 slen;                /* length of single string            */
  signed16 soff;                /* offset of single string            */
  char     *sl_begin;           /* start of data to store             */
  char     *r_begin;            /* start of data in record            */

  sl_begin = str_list;
  r_begin  = record;

				/* store string lists                 */
  while                         /* still data to store                */
       (str_list - sl_begin < len)
  {                             /* store length and re presentation   */
    slen = ((D21_str_list *)str_list)->d21_strl_len;
    soff = ((D21_str_list *)str_list)->d21_strl_off;
    stint(slen,record);
    record += INTSIZE;
    stint(((D21_str_list *)str_list)->d21_strl_rep,record);
    record   += INTSIZE;
    str_list += sizeof(D21_str_list);
    memcpy(record,str_list,slen);
    str_list += soff;
    record   += slen;
  }

  return(record - r_begin);

}                               /* d26_im25_st_str_list               */
