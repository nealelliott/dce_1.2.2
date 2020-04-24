/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26spatis.c,v $
 * Revision 1.1.129.2  1996/02/18  19:46:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:29  marty]
 *
 * Revision 1.1.129.1  1995/12/08  15:47:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:55  root]
 * 
 * Revision 1.1.127.2  1994/06/09  18:44:59  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:46  devsrc]
 * 
 * Revision 1.1.127.1  1994/02/22  17:52:50  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:18:43  marrek]
 * 
 * Revision 1.1.125.2  1993/10/14  17:16:42  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:12:09  keutel]
 * 
 * Revision 1.1.125.1  1993/10/13  17:31:10  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:46:40  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26spatis.c,v $ $Revision: 1.1.129.2 $ $Date: 1996/02/18 19:46:06 $";
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
/* NAME         : d26spatis                                           */
/*                                                                    */
/* AUTHOR       : E. Kraemer, AP 11                                   */
/* DATE         : 21.09.93                                            */
/*                                                                    */
/* COMPONENT    : D2                                                  */
/*                                                                    */
/* DOC.-NR.     : D2-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*                                                                    */
/* DESCRIPTION  : The module contains functions, that load the        */
/*                structured X.500 attributes from C-ISAM records     */
/*                and store them.                                     */
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
#include <dce/cmx_addr.h>

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

static void      d26_is15_ld_criteria(char **criteria, char **record);
static void      d26_is16_ld_item(char **item, char **record);
static void      d26_is25_st_criteria(char **criteria, char **record);
static void      d26_is26_st_item(char **item, char **record);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is11_ld_search_guide                              */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        22.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the Search guide attribute value */
/*              in APDU format from C-ISAM format.                    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_is11_ld_search_guide(
			/* OUT   -  where the Search guide value has  */
    char *search_guide, /*          to be written to                  */
    char *record)       /* IN    -  Record to read data from.         */

{                               /* d26_is11_ld_search_guide           */

  signed16 coff;                /* offset of search guide component   */
  signed16 clen;                /* length of search guide component   */
  char     *sg_begin;           /* start of search guide              */

  sg_begin = search_guide;

				/* load object identifier             */
  clen = ldint(record);
  coff = D2_ALIGN(clen);
  ((d21_obj_len *)search_guide)->d21_length = clen;
  ((d21_obj_len *)search_guide)->d21_offset = coff;
  record       += INTSIZE;
  search_guide += sizeof(d21_obj_len);
  memcpy(search_guide,record,clen);
  record       += clen;
  search_guide += clen;
  memset(search_guide,D2_EOS,coff - clen);
  search_guide += coff - clen;
  d26_is15_ld_criteria(&search_guide,&record);

  return(search_guide - sg_begin);

}                               /* d26_is11_ld_search_guide           */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is12_ld_ttx_term_id                               */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        22.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the TTX-Id attribute value in    */
/*              APDU format from C-ISAM format.                       */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        Short           length of written data            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_is12_ld_ttx_term_id(
			/* OUT   -  where the TTX-ID value has to be  */
    char     *ttx_id,   /*          written to                        */
    char     *record,   /* IN    -  Record to read data from.         */
    signed16 len)       /* IN    -  Length of data in record.         */

{                                   /* d26_is12_ld_ttx_term_id        */

  d21_ttxid *comp;                 /* pointer to ttx-id component     */
  signed16  clen;                  /* length of ttx-id component      */
  signed16  coff;                  /* offset of ttx-id component      */
  char      *r_begin;              /* start of data to load           */
  char      *ttx_begin;            /* start of TTX-Id to write        */

  r_begin   = record;
  ttx_begin = ttx_id;

				    /* load parts of TTX-ID           */
  while                             /* still data to load             */
       (record - r_begin < len)
  {                                 /* load type                      */
    comp     = (d21_ttxid *)ttx_id;
    comp->d2_str_type = ldint(record);
    comp->d2_filler   = 0;
    record  += INTSIZE;
    clen     = ldint(record);
    coff     = D2_ALIGN(clen);
    record  += INTSIZE;
    comp->d2_str_len = clen;
    comp->d2_offset  = coff;
    ttx_id  += sizeof(d21_ttxid);
    memcpy(ttx_id,record,clen);
    record  += clen;
    ttx_id  += clen;
    memset(ttx_id,D2_EOS,coff - clen);
    ttx_id  += coff - clen;
  }

  return(ttx_id - ttx_begin);

}                                   /* d26_is12_ld_ttx_term_id        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is13_ld_fax_number                                */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        22.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the FAX number                   */
/*              attribute value in APDU format from C-ISAM format.    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_is13_ld_fax_number(
			/* OUT    - where the fax number attribute    */
    char     *fax_number,           /* value has to be written to     */
    char     *record,   /* IN     - Record to read data from.         */
    signed16 len)       /* IN     - Length of data in record.         */

{                                   /* d26_is13_ld_fax_number         */

  signed16 clen;                    /* length of telephone number     */
  signed16 coff;                    /* offset of telephone number     */
  char     *fax_begin;              /* begin of fax number            */

  fax_begin = fax_number;

  ((d21_faxnr *)fax_number)->d2_str_len = clen = ldint(record);
  record += INTSIZE;
  ((d21_faxnr *)fax_number)->d2_offset  = coff = ldint(record);
  record += INTSIZE;
  fax_number += sizeof(d21_faxnr);
  memcpy(fax_number,record,clen);
  record += clen;
  fax_number += clen;
  memset(fax_number,D2_EOS,D2_ALIGN(clen) - clen);
  fax_number += D2_ALIGN(clen) - clen;

  if                            /* G3-non basic parameters following  */
    (coff >= 0)
  {                             /* load them                          */
    *(signed32 *)fax_number = ldlong(record);
    record     += LONGSIZE;
    fax_number += sizeof(signed32);
  }

  return(fax_number - fax_begin);

}                               /* d26_is13_ld_fax_number             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is14_ld_psap_address                              */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        23.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the PSAP address                 */
/*              attribute value in APDU format from C-ISAM format.    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_is14_ld_psap_addr(
			/* OUT   -  where the PSAP address attribute  */
    char     *psap_addr,/*          value has to be written to        */
    char     *record,   /* IN    -  Record to read data from.         */
    signed16 len)       /* IN    -  Length of data in record.         */

{                               /* d26_is14_ld_psap_address           */

  signed16 coff;                /* offset of PSAP component           */
  char     *r_begin;            /* begin of data in record            */
  char     *psap_begin;         /* begin of PSAP address              */

  psap_begin = psap_addr;
  r_begin    = record;

  while                         /* still data to load                 */
       (record - r_begin < len)
  {                             /* load component                     */
    ((T_adrdesc *)psap_addr)->t_infotype = *record++;
    ((T_adrdesc *)psap_addr)->t_infovers = *record++;
    ((T_adrdesc *)psap_addr)->t_translng = ldint(record);
    record += INTSIZE;
    ((T_adrdesc *)psap_addr)->t_infolng = ldint(record);
    record += INTSIZE;
    ((T_adrdesc *)psap_addr)->t_nextoff = coff = ldint(record);
    record += INTSIZE;
    psap_addr += sizeof(T_adrdesc);
    coff      -= sizeof(T_adrdesc);
    memcpy(psap_addr,record,coff);
    record += coff;
    psap_addr += coff;
  }

  return(psap_addr - psap_begin);

}                               /* d26_is14_ld_psap_addr              */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is15_ld_criteria                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        22.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function reads the components of                 */
/*              search guide criteria from C-ISAM format and converts */
/*              to APDU.                                              */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is15_ld_criteria(
			/* OUT   -  where the criteria component      */
    char **criteria,    /*          has to be written to              */
    char **record)      /* IN    -  Record to read data from.         */

{                               /* d26_is15_ld_criteria               */

  signed16 choice;              /* choice of criteria                 */
  signed16 count;               /* count of subcomponents             */

  register signed16 i;          /* loop variable                      */

				/* check choice of criteria           */
  choice   = ldint(*record);
  *record += INTSIZE;
  ((d21_crdesc *)*criteria)->d21_choice = choice;
  switch(choice)
  {
    case D2_AND:
    case D2_OR:
      count    = ldint(*record);
      *record += INTSIZE;
      ((d21_crdesc *)*criteria)->d21_count = count;
      *criteria += sizeof(d21_crdesc);
      for                         /* all subcriteria                    */
	 (i = 0; i < count; i++)
      {                           /* load subcriteria                   */
	d26_is15_ld_criteria(criteria,record);
      }
      break;
    case D2_NOT:
      ((d21_crdesc *)*criteria)->d21_count = 0;
      *criteria += sizeof(d21_crdesc);
      d26_is15_ld_criteria(criteria,record);
      break;
    case D2_ITEM:
      ((d21_crdesc *)*criteria)->d21_count = 0;
      *criteria += sizeof(d21_crdesc);
      d26_is16_ld_item(criteria,record);
      break;
  }

}                               /* d26_is15_ld_criteria               */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is16_ld_item                                      */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        22.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function reads the search guide item from C-ISAM */
/*              format and converts to APDU.                          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is16_ld_item(
			/* OUT   -  where the criteria item has to be */
    char   **item,      /*          written to                        */
    char   **record)    /* IN    -  Record to read data from.         */

{                               /* d26_is16_ld_item                   */

  signed16 len;                 /* length of attribute type           */
  signed16 off;                 /* offset of attribute type           */

				/* check choice of criteria           */
  ((d21_itdesc *)*item)->d21_match  = ldint(*record);
  ((d21_itdesc *)*item)->d21_filler = 0;
  *record += INTSIZE;
  len      = ldint(*record);
  off      = D2_ALIGN(len);
  *record += INTSIZE;
  ((d21_itdesc *)*item)->d21_length = len;
  ((d21_itdesc *)*item)->d21_offset = off;
  *item   += sizeof(d21_itdesc);
  memcpy(*item,*record,len);
  *record += len;
  *item   += len;
  memset(*item,D2_EOS,off - len);
  *item   += off - len;

}                               /* d26_is16_ld_item                   */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is21_st_search_guide                              */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        22.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function stores the search guide attribute value */
/*              in C-ISAM format from APDU format.                    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_is21_st_search_guide(
    char *search_guide, /* IN    -  search guide value to be written  */
    char *record)       /* OUT   -  Record to write data to.          */

{                               /* d26_is21_st_search_guide           */

  signed16 clen;                /* length of OR address component     */
  signed16 coff;                /* offset of OR address component     */
  char     *r_begin;            /* begin of data in record            */

  r_begin = record;
				/* store object identifier            */
  clen = ((d21_obj_len *)search_guide)->d21_length;
  coff = ((d21_obj_len *)search_guide)->d21_offset;
  stint(clen,record);
  record       += INTSIZE;
  search_guide += sizeof(d21_obj_len);
  memcpy(record,search_guide,clen);
  record       += clen;
  search_guide += coff;
  d26_is25_st_criteria(&search_guide,&record);

  return(record - r_begin);

}                               /* d26_is21_st_search_guide           */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is22_st_ttx_term_id                               */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        22.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the TTX-terminal-Id attribute    */
/*              value in C-ISAM format from APDU format.              */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_is22_st_ttx_term_id(
    char *ttx_id,       /* IN    -  ttx-Id value to be written        */
    char *record,       /* OUT   -  Record to write data to           */
    signed16 len)       /* IN    -  Length of data to write           */

{                               /* d26_is22_st_ttx_term_id            */

  d21_ttxid *comp;              /* pointer to ttx-id component        */
  signed16  clen;               /* length of OR name component        */
  signed16  coff;               /* offset of OR name component        */
  char      *ttx_begin;         /* start of TTX-Id to write           */
  char      *r_begin;           /* start of record                    */

  ttx_begin = ttx_id;
  r_begin   = record;

				/* store parts of TTX-ID              */
  while                         /* still data to store                */
       (ttx_id - ttx_begin < len)
  {                             /* store type                         */
    comp = (d21_ttxid *)ttx_id;
    stint(comp->d2_str_type,record);
    record += INTSIZE;
    clen = comp->d2_str_len;
    coff = comp->d2_offset;
    stint(clen,record);
    record += INTSIZE;
    ttx_id += sizeof(d21_ttxid);
    memcpy(record,ttx_id,clen);
    record  += clen;
    ttx_id  += coff;
  }

  return(record - r_begin);

}                               /* d26_is22_st_ttx_term_id            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is23_st_fax_number                                */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        22.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the FAX number                   */
/*              attribute value in C-ISAM format from APDU format.    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        signed16        length of written data            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_is23_st_fax_number(
			/* IN    -  FAX number to be written          */
    char     *fax_number,
    char     *record,   /* OUT   -  Record to write data to.          */
    signed16 len)       /* IN    -  Length of data to write.          */

{                               /* d26_im23_fax_number                */

  signed16 clen;                /* length of ...                      */
  signed16 coff;                /* offset of ...                      */
				/* ... FAX telephone number component */
  char     *r_begin;            /* start of data in record            */

  r_begin = record;

  clen = ((d21_faxnr *)fax_number)->d2_str_len;
  stint(clen,record);
  record += INTSIZE;
  coff = ((d21_faxnr *)fax_number)->d2_offset;
  stint(coff,record);
  record += INTSIZE;
  fax_number += sizeof(d21_faxnr);
  memcpy(record,fax_number,clen);
  record += clen;
  fax_number += D2_ALIGN(clen);

  if                            /* G3-non basic parameters following  */
    (coff >= 0)
  {                             /* load them                          */
    stlong(*(signed32 *)fax_number,record);
    record     += LONGSIZE;
    fax_number += sizeof(signed32);
  }

  return(record - r_begin);

}                               /* d26_is23_st_fax_number             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is24_st_psap_addr                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        23.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the FAX number                   */
/*              attribute value in C-ISAM format from APDU format.    */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      psap_addr   String          PSAP address to be written        */
/*      len         Short           Length of data to write.          */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      record      String          Record to write data to.          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0        Short           length of written data            */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*      stint()                                                       */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_is24_st_psap_addr(
    char     *psap_addr,/* IN    -  PSAP address to be written        */
    char     *record,   /* OUT   -  Record to write data to.          */
    signed16 len)       /* IN    -  Length of data to write.          */

{                               /* d26_is24_st_psap_addr              */

  signed16 coff;                /* offset of PSAP component           */
  char     *r_begin;            /* begin of data in record            */
  char     *psap_begin;         /* begin of PSAP address              */

  psap_begin = psap_addr;
  r_begin    = record;

  while                         /* still data to store                */
       (psap_addr - psap_begin < len)
  {                             /* store component                    */
    *record++ = ((T_adrdesc *)psap_addr)->t_infotype;
    *record++ = ((T_adrdesc *)psap_addr)->t_infovers;
    stint(((T_adrdesc *)psap_addr)->t_translng,record);
    record += INTSIZE;
    stint(((T_adrdesc *)psap_addr)->t_infolng,record);
    record += INTSIZE;
    coff = ((T_adrdesc *)psap_addr)->t_nextoff;
    stint(coff,record);
    record += INTSIZE;
    psap_addr += sizeof(T_adrdesc);
    coff   -= sizeof(T_adrdesc);
    memcpy(record,psap_addr,coff);
    record += coff;
    psap_addr += coff;
  }

  return(record - r_begin);

}                               /* d26_is24_st_psap_addr              */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is25_st_criteria                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        22.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function reads the criteria component of a       */
/*              search guide from APDU format and converts to C-ISAM. */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is25_st_criteria(
    char **criteria,    /* IN    -  criteria component to write       */
    char **record)      /* OUT   -  Record to write data to.          */

{                               /* d26_is25_st_criteria               */

  signed16 choice;              /* choice of criteria                 */
  signed16 count;               /* count of subcomponents             */

  register signed16 i;          /* loop variable                      */

				/* check choice of criteria           */
  choice = ((d21_crdesc *)*criteria)->d21_choice;
  stint(choice,*record);
  *record += INTSIZE;

  switch(choice)
  {
    case D2_AND:
    case D2_OR:
      count = ((d21_crdesc *)*criteria)->d21_count;
      stint(count,*record);
      *record   += INTSIZE;
      *criteria += sizeof(d21_crdesc);
      for                       /* all subcriteria                    */
	 (i = 0; i < count; i++)
      {                         /* store subcriteria                  */
	d26_is25_st_criteria(criteria,record);
      }
      break;
    case D2_NOT:
      *criteria += sizeof(d21_crdesc);
      d26_is25_st_criteria(criteria,record);
      break;
    case D2_ITEM:
      *criteria += sizeof(d21_crdesc);
      d26_is26_st_item(criteria,record);
      break;
  }

}                               /* d26_is25_st_criteria               */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is26_st_item                                      */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        22.09.93                                              */
/*                                                                    */
/* DESCRIPTION: This function reads the search guide item from APDU   */
/*              format and converts to C-ISAM format.                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is26_st_item(
    char **item,        /* IN    -  criteria item to write            */
    char **record)      /* OUT   -  Record to write data to.          */

{                               /* d26_is26_st_item                   */

  unsigned16 match;             /* matching rule of item              */
  signed16   len;               /* length of attribute type           */
  signed16   off;               /* offset of attribute type           */

				/* check choice of criteria           */
  match = ((d21_itdesc *)*item)->d21_match;
  len   = ((d21_itdesc *)*item)->d21_length;
  off   = ((d21_itdesc *)*item)->d21_offset;
  stint(match,*record);
  *record += INTSIZE;
  stint(len,*record);
  *record += INTSIZE;
  *item   += sizeof(d21_itdesc);
  memcpy(*record,*item,len);
  *record += len;
  *item   += off;

}                               /* d26_is26_st_item                   */

