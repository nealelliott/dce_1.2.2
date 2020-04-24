/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * HISTORY
 * $Log: d261utilis.c,v $
 * Revision 1.1.717.2  1996/02/18  19:44:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:46  marty]
 *
 * Revision 1.1.717.1  1995/12/08  15:42:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:03  root]
 * 
 * Revision 1.1.715.4  1994/06/21  14:45:04  marrek
 * 	June 1994 code submission.
 * 	[1994/06/20  13:04:23  marrek]
 * 
 * Revision 1.1.715.3  1994/05/10  15:51:41  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:19:06  marrek]
 * 
 * Revision 1.1.715.2  1994/03/23  15:09:00  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:19:28  keutel]
 * 
 * Revision 1.1.715.1  1994/02/22  16:04:58  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:07:42  marrek]
 * 
 * Revision 1.1.713.3  1993/10/29  15:11:03  keutel
 * 	CR 9261
 * 	[1993/10/29  14:07:06  keutel]
 * 
 * Revision 1.1.713.2  1993/10/14  17:16:15  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:07:49  keutel]
 * 
 * Revision 1.1.713.1  1993/10/13  17:30:20  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:25:21  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  13:29:54  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  12:59:35  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  14:44:21  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  11:16:11  marrek
 * 	Interworking with paradise
 * 	[1993/02/01  15:56:29  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:58:28  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:17:46  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:33:17  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:02:36  marrek]
 * 
 * Revision 1.1.2.4  1992/07/06  17:21:11  melman
 * 	CR 4327: unaligned read/writes on shadow update
 * 	[1992/07/06  17:20:14  melman]
 * 
 * Revision 1.1.2.3  1992/06/30  20:55:38  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:34:49  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:44:27  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:52:31  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d261utilis.c,v $ $Revision: 1.1.717.2 $ $Date: 1996/02/18 19:44:41 $";
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
/* NAME         : d261utilis.c                                        */
/*                                                                    */
/* AUTHOR       : Schreck, D AP 11                                    */
/* DATE         : 07.01.88                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*                                                                    */
/* DESCRIPTION  : This module contains functions working with C-ISAM  */
/*                needed by all DSA functions.                        */
/*                                                                    */
/* SYSTEM DEPENDENCIES: SINIX C-ISAM                                  */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*   0.1    |07.01.88 | Original                       | as |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

/*****  external Includes    *****/

/*****  internal Includes    *****/

#include <dce/d27util.h>
#include <dce/osiaddr.h>

#include <d26schema.h>
#include <d26isam.h>
#include <d26svcis.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

			  /* Constants for function d26_i23_open_file */
#define D26_I18_FILE_CLOSED     -1
#define D26_I19_NO_FILETABLES    2  /* number of filetables           */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/******** IMPORT ********/

extern int  d26_sch_fd[];   /* fd's in /chdb-directory                */

/******** LOCAL  ********/

			    /* format string for building file names  */
static char *file_form  = "%s%s%d";

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

static signed16 d26_i70_cmp_rec_val(signed16 len, char *val,
	signed32 dnl_idx, char *rec_pos, D26_at *p_at_entry, signed16 rule,
	boolean  *read_next_rec, signed16 *rec_found);
static signed16 d26_i71_read_dn_sub(char dn_interpret, signed16 dn_len,
	D2_name_string dn_sub, signed32 key, signed16 obj_filenr,
	D26_at *p_at_entry, int isfd, struct keydesc *isam_key,
	char *val_pos, char *record, signed32 *start_dnl_idx);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i15_get_record_pos                                */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        01.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function calculates the position of the attribute*/
/*              value in 'record'. The calculated position is returned*/
/*              as 'return value' (that is the pointer to the value   */
/*              not the position to the number of values !).          */
/*                                                                    */
/*              When 'pos_no' == D2_TRUE the position of number of    */
/*              values in the main record is returned.                */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Char *       pointer to attribute value                        */
/*                                                                    */
/*exoff ***************************************************************/

char *d26_i15_get_record_pos(

    char       *record, /* IN    -  pointer to record of data         */
			/* IN    -  number of value for which the     */
    signed16   val_no,  /*          position should be calculated     */
    D26_at     *p_at,   /* IN    -  pointer to AT entry of attribute  */
    D26_oct    *p_oct,  /* IN    -  pointer to OCT entry of object    */
			/* IN    -  pointer to naming attribute in SRT*/
    D26_nam_at *p_nam_at,           /* entry of object                */
			/* IN    -  indicates whether position of     */
			/*          number of values should be        */
    boolean     pos_no) /*          returned                          */

{                       /* d26_i15_get_record_pos                     */

  char     *ret_pos;    /* return value: pointer to attribute value   */
  signed32 addsize = 0;
  signed16 mult;

  register signed16 val;/* number of value                            */

  val = val_no;         /* set value number                           */

  if (pos_no == TRUE)
  {                 /* get position of number of values in main record*/
    val = 1;        /* ignore given value number (val_no)             */
    if (p_nam_at != (D26_nam_at *) NULL)
    {               /* attribute is naming attribute                  */
      ret_pos = record + p_nam_at->d26s_position[p_oct->d26c_filenr];
    }
    else
    {               /* attribute is non-naming attribute              */
      ret_pos = record + p_at->d26a_field_pos[p_oct->d26c_filenr];
    }
  }
  else
  {                 /* get position of value                          */
		    /* get position of first value                    */
    if (p_nam_at != (D26_nam_at *) NULL)
    {               /* attribute is naming attribute                  */
      ret_pos = record + p_nam_at->d26s_position[p_oct->d26c_filenr] +
	INTSIZE;
    }
    else
    {               /* attribute is non-naming attribute              */
      if            /* attribute stored at the main entry             */
	 (p_at->d26a_filenr < 0)
      {             /* get pointer to first value                     */
	ret_pos = record + p_at->d26a_field_pos[p_oct->d26c_filenr] +
	  INTSIZE;
      }
      else
      {             /* attribute is not stored at the main entry      */
		    /* get pointer to value                           */
	ret_pos = record + INTSIZE + LONGSIZE;
	val = 1;
      }
    }
  }

  mult = val - 1;

  if                /* not first value                                */
    (mult > 0)
  {                 /* calculate offset to begin of first value       */
    addsize = d26_i59_get_valmemsize(p_at);
		    /* calculate pointer to attribute value           */
    ret_pos = ret_pos + (mult * addsize);
  }

  return(ret_pos);

}                       /* d26_i15_get_record_pos                     */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_i16_insert_val                                  */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 02.02.88                                            */
/*                                                                    */
/* DESCRIPTION  : This function will put the given attribute value    */
/*                (normed value, exact value, length ...) at the      */
/*                position "*pos" in the C-ISAM record.               */
/*                The actual position in the record will be returned. */
/*                (If no (real) recurring attributes are inserted     */
/*                it's the position of the valid sign of the 'next'   */
/*                attribute else it's the position nof the next value)*/
/*                                                                    */
/* RETURN VALUE :   D2_NO_ERR:      no error occured                  */
/*                  D26_ERROR:      any error occurred                */
/*                                                                    */
/*inoff ***************************************************************/

signed16 d26_i16_insert_val(
    D26_at     *at,     /* IN    -  pointer to AT entry of attribute  */
    unsigned16 rep,     /* IN    -  representation of attribute value */
    char       *val,    /* IN    -  attribute value                   */
    signed16   length,  /* IN    -  length of attribute value         */
			/* IN    -  pointer list for ACL-attribute    */
    D26_dns    *acl_val[],
			/* IN    -  pointer to master knowledge value */
    D26_dns    *mk_val[],
			/* INOUT -  position of attribute in C-ISAM   */
    char       **pos)   /*          record                            */

{                                   /*  d26_i16_insert_val            */

  signed32 rec_nr = D26_IDX_NOT_DEF;/* record number in DNL           */
  signed16 i;
  char     *n_pos,                  /* pointer to normed value        */
	   *e_pos;                  /* pointer to exact value         */
  char     dn_interpret;            /* interpretation for dn-syntax   */
  signed16 rlen;                    /* length of value in record      */
  signed16 nlen;                    /* length of normed value in APDU */
  signed16 add_len;     /* length of internal added characters        */
			/* memory for normed attribute value          */
  signed32 normed_val[D26_MAXVALLEN / sizeof(signed32)];

  add_len  = at->d26a_rep == D2_T61_PR_LIST ? 2 * D26_MAXVALPARTS :
    (at->d26a_rep == D2_TLXNR_STX ? D26_MAXVALPARTS : 1);

  switch(at->d26a_syntax)           /*  attribute syntax              */
  {
    case D2_CIGNORE:
    case D2_COUNTRY_STX:
    case D2_IA5:
    case D2_CIGN_LIST:
    case D2_POST_AD_STX:
      if                            /*  phonetic string must be       */
				    /*  inserted                      */
	 (at->d26a_phon == TRUE)
      {                             /*  put "phonetic string" into    */
				    /*  record                        */
	d26_soundex((byte *)val,length,(byte *)*pos);
	*pos += D26_LPHON;
      }
      if                            /*  representation is a choice    */
	 (at->d26a_rep == D2_T61_PR)
      {                             /*  put representation into record*/
	stint(rep,*pos);
	*pos += INTSIZE;
      }
      n_pos = *pos;
      e_pos = n_pos + at->d26a_ubound + add_len;
      *pos = e_pos + at->d26a_ubound + add_len;
				    /*  put "exact" value in record   */
      d26_u02_fill_field(D2_EOS,length,val,at->d26a_ubound + add_len,e_pos);
				    /*  put "normed" value in record  */
      switch(at->d26a_syntax)       /* check attribute syntax         */
      {
	case D2_CIGNORE:
	  d27_651_norm_T61String((byte *)val,(byte *)n_pos,length);
	  break;
	case D2_COUNTRY_STX:
	case D2_IA5:
	  d27_031_norm_pr_string((byte *)val,(byte *)n_pos,length);
	  break;
	case D2_CIGN_LIST:
	case D2_POST_AD_STX:
	  d26_usp_norm_ci_list((byte *)val,(byte *)n_pos,length);
	  break;
      }
      d26_u02_fill_field(D2_EOS,0,(char *)NULL,
	at->d26a_ubound + add_len - length,n_pos + length);
      break;
    case D2_NUMERIC:
    case D2_TELNR_STX:
      n_pos = *pos;
      *pos = e_pos = n_pos + at->d26a_ubound + 1;
      *pos = e_pos + at->d26a_ubound + 1;
				    /*  put "exact" value in record   */
      d26_u02_fill_field(D2_EOS,length,val,at->d26a_ubound + 1,e_pos);
				    /*  put "normed" value in record  */
      length = d26_u28_norm_numbers(val,length,n_pos);
      d26_u02_fill_field(D2_EOS,0,(char *)NULL,
	at->d26a_ubound + 1 - length,n_pos + length);
      break;
    case D2_CEXACT:
    case D2_PRINTABLE:
    case D2_TLXNR_STX:
      if                            /*  phonetic string must be       */
				    /*  inserted                      */
	 (at->d26a_phon == TRUE)
      {                             /*  put "phonetic string" into    */
				    /*  record                        */
	d26_soundex((byte *)val,length,(byte *)*pos);
	*pos += D26_LPHON;
      }
      if                            /*  representation is a choice    */
	 (at->d26a_rep == D2_T61_PR)
      {                             /*  put representation into record*/
	stint(rep,*pos);
	*pos += INTSIZE;
      }
				    /*  put "exact" value in record   */
      d26_u02_fill_field(D2_EOS,length,val,at->d26a_ubound + add_len,*pos);
      *pos += (at->d26a_ubound + add_len);
      break;
    case D2_DISTNAME:               /*  DISTINGUISHED-NAME            */
      if                            /*  attribute is MASTER-KNOWLEDGE */
	(D2_ATTCMP(&(at->d26a_type),D2_MASTER_KNOWLEDGE) == 0)
      {                             /*  insert MK-index in record     */
	stlong(*((*mk_val)->d26_didx),*pos);
	*pos += LONGSIZE;
      }
      else
      {                             /* put attribute in dnlist        */
				    /* special handling for DNLIST    */
				    /* attribute                      */
	dn_interpret = (D2_ATTCMP(&(at->d26a_type),D2_DNLIST) == 0) ?
			D2_COPY : D2_SOBJ ;
	if                          /*  operation is successful       */
	  (!d26_i02_upd_file_dnlist(D2_ADD_AV,(D2_name_string)NULL,
	   dn_interpret,(D2_name_string)val,1L,TRUE,&rec_nr))
	{                           /*  store index                   */
	  stlong(rec_nr,*pos);
	  *pos += LONGSIZE;
	}
	else
	{                           /*  return(D26_ERROR)             */
	  return(D26_ERROR);
	}
      }
      break;
    case D2_OBJ_IDENT:
      if                            /*  attribute is OBJECT-CLASS     */
	(D2_ATTCMP(&at->d26a_type,D2_OBJ_CLASS) == 0)
      {
	stlong(*(signed32 *)val,*pos);
	*pos += LONGSIZE;
	break;
      }
				    /*  goto D2_OCTET otherwise       */
    case D2_OCTET:
    case D2_ASN1:
    case D2_ANY_STX:
    case D2_PASSWD_STX:
      d26_u02_fill_field(D2_EOS,length,val,at->d26a_ubound,*pos);
      if                            /*  attribute has password syntax */
	(at->d26a_syntax == D2_PASSWD_STX)
      {                             /*  scramble it                   */
	d26_u18_scramble_string((byte *)*pos,(byte *)*pos,length);
      }
      *pos += at->d26a_ubound;
				    /*  put attribute length in record */
      stint(length,*pos);
      *pos += INTSIZE;
      break;
    case D2_TTXID_STX:
      rlen = d26_is22_st_ttx_term_id(val,*pos,length);
      d26_u02_fill_field(D2_EOS,0,(char *)NULL,at->d26a_ubound - rlen,
	*pos + rlen);
      *pos += at->d26a_ubound;
      stint(rlen,*pos);
      *pos += INTSIZE;
      break;
    case D2_FAXNR_STX:
      rlen = d26_is23_st_fax_number(val,*pos,length);
      d26_u02_fill_field(D2_EOS,0,(char *)NULL,at->d26a_ubound - rlen,
	*pos + rlen);
      *pos += at->d26a_ubound;
      stint(rlen,*pos);
      *pos += INTSIZE;
      break;
    case D2_GUIDE:
      rlen = d26_is21_st_search_guide(val,*pos);
      d26_u02_fill_field(D2_EOS,0,(char *)NULL,at->d26a_ubound - rlen,
	*pos + rlen);
      *pos += at->d26a_ubound;
      stint(rlen,*pos);
      *pos += INTSIZE;
      break;
    case D2_PSAP_STX:
      rlen = d26_is24_st_psap_addr(val,*pos,length);
      d26_u02_fill_field(D2_EOS,0,(char *)NULL,at->d26a_ubound - rlen,
	*pos + rlen);
      *pos += at->d26a_ubound;
      stint(rlen,*pos);
      *pos += INTSIZE;
      break;
    case D2_MHS_DLSP_STX:
      nlen = d26_un3_norm_submit_perm(val,length,(char *)normed_val);
      rlen = d26_im23_st_dl_sub_perm((char *)normed_val,*pos,nlen);
      d26_u02_fill_field(D2_EOS,0,(char *)NULL,at->d26a_ubound - rlen,
	*pos + rlen);
      *pos += at->d26a_ubound;
      stint(rlen,*pos);
      *pos += INTSIZE;
      rlen = d26_im23_st_dl_sub_perm(val,*pos,length);
      d26_u02_fill_field(D2_EOS,0,(char *)NULL,at->d26a_ubound - rlen,
	*pos + rlen);
      *pos += at->d26a_ubound;
      stint(rlen,*pos);
      *pos += INTSIZE;
      break;
    case D2_MHS_ORADR_STX:
      nlen = d26_un1_norm_or_address(val,length,(char *)normed_val);
      rlen = d26_im21_st_or_address((char *)normed_val,*pos,nlen);
      d26_u02_fill_field(D2_EOS,0,(char *)NULL,at->d26a_ubound - rlen,
	*pos + rlen);
      *pos += at->d26a_ubound;
      stint(rlen,*pos);
      *pos += INTSIZE;
      rlen = d26_im21_st_or_address(val,*pos,length);
      d26_u02_fill_field(D2_EOS,0,(char *)NULL,at->d26a_ubound - rlen,
	*pos + rlen);
      *pos += at->d26a_ubound;
      stint(rlen,*pos);
      *pos += INTSIZE;
      break;
    case D2_MHS_ORNAME_STX:
      nlen = d26_un2_norm_or_name(val,length,(char *)normed_val);
      rlen = d26_im22_st_or_name((char *)normed_val,*pos,nlen);
      d26_u02_fill_field(D2_EOS,0,(char *)NULL,at->d26a_ubound - rlen,
	*pos + rlen);
      *pos += at->d26a_ubound;
      stint(rlen,*pos);
      *pos += INTSIZE;
      rlen = d26_im22_st_or_name(val,*pos,length);
      d26_u02_fill_field(D2_EOS,0,(char *)NULL,at->d26a_ubound - rlen,
	*pos + rlen);
      *pos += at->d26a_ubound;
      stint(rlen,*pos);
      *pos += INTSIZE;
      break;
    case D2_ACCL:
      for (i = 0; i < D2_ACLIDX_MAX; i++, *pos += LONGSIZE)
      {
	stlong(*(acl_val[i]->d26_didx),*pos);
      }
      break;
    case D2_INTEGER:
      stlong(*(signed32 *)val,*pos);
      *pos += LONGSIZE;
      break;
    case D2_BOOLEAN:
      stlong((signed32)*(Bool *)val,*pos);
      *pos += LONGSIZE;
      break;
    case D2_PR_DM_STX:
    case D2_MHS_PR_DM_STX:
      for (i = 0; i < at->d26a_ubound; i++, *pos += LONGSIZE)
      {
	if                          /*  still part of value available */
	  (i * sizeof(signed32) < length)
	{                           /*  insert part of value          */
	  stlong(*(signed32 *)(val + i * sizeof(signed32)),*pos);
	}
	else
	{                           /*  store invalid part            */
	  stlong(-1L,*pos);
	}                           /*                                */
      }
      break;
    case D2_TIME:
      d26_us9_norm_utc_time(val,*pos);
      rlen = strlen(*pos);
      *pos += rlen;
      d26_u02_fill_field(D2_EOS,0,"",at->d26a_ubound + 1 - rlen,*pos);
      *pos += at->d26a_ubound + 1 - rlen;
      break;
    case D2_CERTIFICATE:
    case D2_CERT_PAIR:
    case D2_CERT_LIST:
      switch(at->d26a_syntax)
      {
	case D2_CERTIFICATE:
	  d26_is51_st_certificate(&val,pos);
	  break;
	case D2_CERT_PAIR:
	  d26_is52_st_cert_pair(&val,pos);
	  break;
	case D2_CERT_LIST:
	  d26_is53_st_cert_list(&val,pos);
	  break;
      }
      d26_u02_fill_field(D2_EOS,0,"",at->d26a_ubound - length,*pos);
      *pos += at->d26a_ubound - length;
      stint(length,*pos);
      *pos += INTSIZE;
      break;
    default:                        /*  otherwise                     */
      dce_svc_printf(GDS_S_ILL_AT_SYNTAX_MSG,at->d26a_syntax);
      break;
  }

				    /*  return (D2_NO_ERR)            */
  return(D2_NO_ERR);

}                                   /*  d26_i16_insert_val            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i17_del_all_rec_vals                              */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        04.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function reads all values of a special real      */
/*              recurring attribute from file and deletes the records.*/
/*              When necessary the dn list in file is updated.        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      signed16 D2_NO_ERR       record found and deleted             */
/*                                                                    */
/*               D26_ERROR       any error occurred (record not found */
/*                               or C-ISAM error occurred)            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i17_del_all_rec_vals(
			/* IN    -  pointer to AT entry of attribute  */
    D26_at    *p_at_entry,
    signed32  key,      /* IN    -  C-ISAM key of changed main entry  */
			/* IN    -  object file number                */
    signed16  obj_filenr,
    int       isfd,     /* IN    -  file descriptor of attribute file */
    char      *record)  /* IN    -  pointer to record buffer          */

{                               /*  d26_i17_del_all_rec_vals          */

  signed16 return_value = D2_NO_ERR;
  signed16 ret_val;
  char     *val_pos;

			/*  read first record                         */
  ret_val = d26_i18_read_rec_val(0,(char *)NULL,key,obj_filenr,
    p_at_entry,TRUE,TRUE,ISEQUAL,isfd,D2_EQUAL,record);

  val_pos = d26_i15_get_record_pos(record,1,p_at_entry,(D26_oct *)NULL,
    (D26_nam_at *)NULL,FALSE);

  while                 /*  records found                             */
       (ret_val == D2_NO_ERR)
  {                     /*  delete record from file                   */
    if (d26_i19_del_rec_val(ret_val,p_at_entry,isfd,val_pos) != D2_NO_ERR)
    {
      return_value = D26_ERROR;
    }

			/*  read next record                          */
    ret_val = d26_i18_read_rec_val(0,(char *)NULL,key,obj_filenr,
      p_at_entry,TRUE,TRUE,ISNEXT,isfd,D2_EQUAL,record);
  }

  if (ret_val == D26_ERROR)
  {                     /* error from read record                     */
    return_value = D26_ERROR;
  }

  return (return_value);

}                               /*  d26_i17_del_all_rec_vals          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i18_read_rec_val                                  */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        04.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function reads one record from a file of a real  */
/*              recurring attribute.                                  */
/*                                                                    */
/*              Since this is an internal interface there are no      */
/*              checks whether the parameter values are given         */
/*              correctly !                                           */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      signed16 D2_NO_ERR       record found                         */
/*                                                                    */
/*               D2_NFOUND       record doesn't exist                 */
/*                                                                    */
/*               D26_ERROR       any error occurred (C-ISAM error     */
/*                               occurred)                            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i18_read_rec_val(
    signed16 len,       /* IN    -  length of attribute value         */
    char     *val,      /* IN    -  attribute value                   */
    signed32 key,       /* IN    -  C-ISAM key of main entry          */
    signed16 obj_filenr,/* IN    -  object file number                */
			/* IN    -  pointer to AT entry of attribute  */
    D26_at   *p_at_entry,
    boolean  no_old_val,/* IN    -  indicates whether a value is given*/
			/* IN    -  indicates whether the read record */
    boolean  lock,      /*          should be locked                  */
    int      mode,      /* IN    _  mode value for 'isread' function  */
			/* IN    -  C-ISAM filedescriptor of attribute*/
    int      isfd,      /*          file                              */
    signed16 rule,      /* IN    -  indicates matching rule for value */
    char     *record)   /* INOUT -  record containing attribute value */

{                               /*  d26_i18_read_rec_val              */

  static String   function = "i18";
  static signed32 last_dnl_idx = 0; /* last used index for D2_DISTNAME*/
				    /* for rule D2_SUBSTRINGS         */

  signed16       return_value = D2_NO_ERR;
  signed32       dnl_idx;
  boolean        read_rec;
  signed16       compare;
  int            s_mode, read_mode, key_len, rcode;
  signed16       max_key_len;
  struct keydesc isam_key;
  char           *val_pos;
  char           *pos;
  char           dn_interpret;      /* interpretation for dn-syntax   */

  signed32       is_count = 0;     /* counts calls of isstart        */
  signed32       ir_count = 0;     /* counts calls of isread         */

/************************/
/* initialize variables */
/************************/

  max_key_len = MAXKEYSIZE - (INTSIZE + LONGSIZE);
  val_pos = d26_i15_get_record_pos(record,1,p_at_entry,(D26_oct *)NULL,
    (D26_nam_at *)NULL,FALSE);
  compare = d26_i55_sel_rec_key(&isam_key,p_at_entry,rule,D23_READ);
  if (no_old_val == FALSE && p_at_entry->d26a_syntax == D2_DISTNAME)
  {                                 /* get dnl_idx of dn list in core */
    if (rule == D2_SUBSTRINGS)
    {                   /* substring at the begin of dn is given      */
      if (mode == ISEQUAL)
      {
	last_dnl_idx = 0;
      }
      else
      {
	last_dnl_idx++;
      }

      dn_interpret = (D2_ATTCMP(&p_at_entry->d26a_type,D2_DNLIST) != 0) ?
	D2_SOBJ : D2_COPY;
      return_value = d26_i71_read_dn_sub(dn_interpret,len,
	(D2_name_string)val,key,obj_filenr,p_at_entry,isfd,&isam_key,val_pos,
	record,&last_dnl_idx);
      return(return_value);
    }
    else
    {
			/* special handling for DNLIST attribute      */
      dn_interpret = (D2_ATTCMP(&p_at_entry->d26a_type,D2_DNLIST) != 0) ?
	D2_SOBJ : D2_COPY;

      if ((dnl_idx = d26_u12_get_dnlidx (dn_interpret,(D2_name_string)val))
	  < 0)
      {
	return(D2_NFOUND);
      }
    }
  }

/***************************************/
/* set key and fill in searched values */
/***************************************/
  switch(mode)          /*  given mode for 'isread'                   */
  {
    case ISGTEQ:
    case ISEQUAL:
			/* set isam key via 'isstart' and fill in     */
			/* record values                              */
      key_len = 0;
      if (no_old_val == TRUE)
      {
	key_len = INTSIZE + LONGSIZE;
      }
      else
      {
	if (rule == D2_SUBSTRINGS && len < max_key_len &&
	    p_at_entry->d26a_syntax != D2_DISTNAME)
	{
	  key_len = len + INTSIZE + LONGSIZE;
	}
      }

			/* fill in record values                      */
      stint (obj_filenr,record);
      stlong(key,record + INTSIZE);
      if (no_old_val == FALSE)
      {
	if (p_at_entry->d26a_syntax == D2_DISTNAME)
	{
	  stlong(dnl_idx, val_pos);
	}
	else
	{
	  pos = val_pos;
	  d26_i16_insert_val(p_at_entry,D2_T61,val,len,(D26_dns **)NULL,
	    (D26_dns **)NULL,&pos);
	}
      }

				    /* call isstart                   */
      if                /*  attribute file is in use already          */
	(d26_afildes[p_at_entry->d26a_filenr].d26_in_use > 0)
      {                 /*  set the reset flag                        */
	d26_reset = TRUE;
      }

      is_count++;
      d26_dbacc_count++;
      if (D26_ISSTART(svc_c_route_nolog,isfd,&isam_key,key_len,record,
	  mode) < 0)
      {
	if (iserrno == ENOREC)
	{               /* record doesn't exist                       */
	  return_value = D2_NFOUND;
	}
	else
	{
	  return_value = D26_ERROR;
	}
	D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count,
	  ir_count);
	return (return_value);
      }
      s_mode = ISCURR;
      break;
    case ISNEXT:
      s_mode = ISNEXT;
      break;
    default:
      return (D26_ERROR);
  }

/**********************/
/* read wished record */
/**********************/
  read_rec  = TRUE;
  read_mode = s_mode;
  if (lock == TRUE)
  {
    read_mode = read_mode + ISLOCK;
  }

  while                 /*  correct record isn't found                */
       (read_rec == TRUE)
  {                     /*  read record using isread                  */
   /***************/
   /* read record */
   /***************/
    ir_count++;
    d26_dbacc_count++;
    rcode = D26_ISREAD(svc_c_route_nolog,isfd,record,read_mode);
    while               /*  record is locked                          */
	 (rcode < 0 && iserrno == ELOCKED)
    {                   /*  sleep 1 second                            */
      sleep (1);
			/*  try to read record again                  */
      ir_count++;
      d26_dbacc_count++;
      rcode = D26_ISREAD(svc_c_route_nolog,isfd,record,read_mode);
    }

    if (rcode < 0)      /*  isread error occurred                     */
    {                   /*  MA_LOG isam error                         */
      read_rec = FALSE;

			/* set return_value                           */
      switch(iserrno)   /* errorcode from isread                      */
      {
	case EENDFILE:  /* end of file                                */
	case ENOCURR:   /* no current record                          */
	case ENOREC:    /* no such record                             */
	  return_value = D2_NFOUND;
	  break;
	default:        /* other C isam error occured                 */
	  return_value = D26_ERROR;
	  break;
      }
      break;            /* leave loop !                               */
    }

   /******************************************/
   /* check whether correct record was found */
   /******************************************/
			/*  check whether the wished record was found */
    if                  /* 'key', 'obj_filenr' and 'val' must be      */
			/* compared                                   */
      (compare == D26_CMP && (rule != D2_SUBSTRINGS && len >= max_key_len ||
       len > max_key_len) || mode == ISNEXT)
    {                   /* compare 'key' and 'obj_filenr'             */
      if                /* key or obj_filenr doesn't match            */
	(obj_filenr != ldint(record) || key != ldlong(record + INTSIZE))
      {                 /* searched record doesn't exist              */
	return_value = D2_NFOUND;
	read_rec = FALSE;
      }
      else
      {                 /* key and obj_filenr matches                 */
	if (no_old_val == FALSE && rule != D2_GTEQ && rule != D2_LTEQ)
	{               /* value must be compared also                */
	  d26_i70_cmp_rec_val(len,val,dnl_idx,val_pos,p_at_entry,rule,
	    &read_rec,&return_value);
	}
	else
	{               /* searched record was found                  */
	  read_rec = FALSE;
	  break;
	}
      }

      if                /* not the correct record was read and the    */
			/* read record is locked                      */
	((read_rec == TRUE || return_value == D2_NFOUND) && lock == TRUE)
      {                 /* unlock record                              */
	if ((D26_ISRELEASE(svc_c_sev_warning,isfd)) < 0)
	{
	  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count,
	    ir_count);
	  return (D26_ERROR);
	}
      }

      if (read_rec == TRUE)
      {                 /* the correct record wasn't found            */
	if (mode == ISNEXT)
	{               /* searched record doesn't exist              */
	  read_rec = FALSE;
	  return_value = D2_NFOUND;
	  break;        /* don't read next record, leave loop !!!!    */
	}
	else
	{               /* set read_mode for next 'isread'            */
	  read_mode = ISNEXT;
	  if (lock == TRUE)
	  {
	    read_mode = read_mode + ISLOCK;
	  }
	}
      }
    }
    else
    {                   /* searched record was found                  */
      read_rec = FALSE;
      break;
    }
  }

  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count,
    ir_count);

  return (return_value);

}                               /*  d26_i18_read_rec_val              */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i19_del_rec_val                                   */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        03.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function checks whether a record could be read   */
/*              from real recurring value file. When a record was     */
/*              read the record is deleted and if necessary the       */
/*              dn list in file is updated.                           */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      signed16 D2_NO_ERR       record found and deleted             */
/*                                                                    */
/*               D26_ERROR       any error occurred (record not found */
/*                               or C-ISAM error occurred)            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i19_del_rec_val(
    signed16 rec_found, /* IN    -  indicates whether record was read */
		    /* D2_NO_ERR   record was read ---> delete record */
		    /* D2_NFOUND   record not found                   */
		    /* D26_ERROR   C-ISAM error occurred during read  */
			/* IN    -  pointer to AT entry of attribute  */
    D26_at   *p_at_entry,
			/* IN    -  C-ISAM filedescriptor of attribute*/
    int      isfd,      /*          file                              */
    char     *val_pos)  /* IN    -  attribute value in record         */

{                                   /*  d26_i19_del_rec_val           */

  signed32 dnl_idx;                 /* index value to dn list in core */

  if                         /* record found                          */
    (rec_found == D2_NO_ERR)
  {                          /* delete record                         */
    d26_i20_append_dnlidx(p_at_entry,val_pos,(boolean *)NULL,
      (signed32 **)NULL,(signed16 *)NULL,(signed16 *)NULL,&dnl_idx,
      (D23_rserror *)NULL,(signed32 *)NULL);
    if                       /* operation fails                       */
      (D26_ISDELCURR(svc_c_sev_warning,isfd) < 0)
    {                        /* MA2_002_ERROR                         */
      D26_ISRELEASE(svc_c_sev_warning,isfd);
      return(D26_ERROR);
    }
    else
    {                        /* rewrite successfull                   */
      D26_ISRELEASE(svc_c_sev_warning,isfd);
      if (dnl_idx >= 0)
      {
	d26_i02_upd_file_dnlist(D2_DEL_AV,(D2_name_string)NULL,D2_IGNORE,
	  (D2_name_string)NULL,-1L,TRUE,&dnl_idx);
      }
    }
  }
  else
  {                          /* error: record not found               */
    dce_svc_printf(GDS_S_DB_INCONSISTENT_MSG);
    return(D26_ERROR);
  }

  return (D2_NO_ERR);

}                               /*  d26_i19_del_rec_val               */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i20_append_dnlidx                                 */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        01.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function loads the index to dn list from 'pos'   */
/*              when the syntax of the attribute is distinguished     */
/*              name syntax and the attribute isn't the master        */
/*              knowledge attribute.                                  */
/*                                                                    */
/*              When the attribute is a real recurring one only the   */
/*              index to dn list is returned (or val < 0). In this    */
/*              case an error doesn't never occur.                    */
/*                                                                    */
/*              When the attribute is stored in the main entry the    */
/*              index value is appended to 'del_dns' (when the index  */
/*              is >= 0). In this case errors can occur and the error */
/*              apdu is returned.                                     */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*       signed16 D2_NO_ERR no error occurred (everytime for real     */
/*                          recurring attributes)                     */
/*                                                                    */
/*                D26_ERROR error occurred (not enough memory         */
/*                          for index values available)               */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i20_append_dnlidx(
			/* IN    -  pointer to AT entry of the        */
    D26_at      *p_at_entry,        /* attribute                      */
    char        *pos,   /* IN    -  pointer to attribute value        */
			/* INOUT -  indicates whether distinguished   */
    boolean     *upd_dnl_file,      /* name list must be updated      */
			/* INOUT -  DN list indices whose occurrencies*/
    signed32    **del_dns,          /*  must be decreased             */
			/* INOUT -  number of elements in del_dns     */
    signed16    *no_del_dns,
			/* INOUT -  available memory in del_dns       */
    signed16    *no_mem_del_dns,
			/* OUT   -  DN-list index of attribute value  */
    signed32    *dnl_idx,
    D23_rserror *error, /* OUT   -  error message                     */
    signed32    *l_err) /* OUT   -  length of error message           */


{                            /*  d26_i20_append_dnlidx                */

  signed16 ret_val;          /* return value of a called function     */

			     /*  initialize variables                 */
  *dnl_idx = D26_EOL;

  if                         /*  attr. syntax == dist.name syntax and */
			     /*  attr. type != master knowledge       */
    (p_at_entry->d26a_syntax == D2_DISTNAME &&
     D2_ATTCMP(&p_at_entry->d26a_type,D2_MASTER_KNOWLEDGE) != 0)
  {                          /*  load index value                     */
    *dnl_idx = ldlong (pos);
    if                  /* attribute isn't non real recurring attrib. */
			/* and index value >= 0                       */
      (p_at_entry->d26a_filenr < 0 && *dnl_idx >= 0)
    {                   /* append dnl_idx to list                     */
      ret_val = d26_u13_store_del_dns(*dnl_idx,del_dns,no_del_dns,
	no_mem_del_dns,upd_dnl_file,error,l_err);
      if (ret_val != D2_NO_ERR)
      {
	return (D26_ERROR);
      }
    }
  }

  return (D2_NO_ERR);
}                               /*  d26_i20_append_dnlidx             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_i23_open_file                                   */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 02.02.88                                            */
/*                                                                    */
/* DESCRIPTION  : This function will return the appropriate C-ISAM    */
/*                file descriptor.                                    */
/*                                                                    */
/*                First the function checks whether the file is       */
/*                already open.                                       */
/*                                                                    */
/*                When the file is open then the time of last         */
/*                use is set using the value of the global variable   */
/*                d26_ftime.                                          */
/*                                                                    */
/*                When the file isn't open the file which was Last    */
/*                Recently Used, is closed (when the times of more    */
/*                files are equal the file with the lowest file number*/
/*                (from AT  == index in file table) is closed) and the*/
/*                wished file is opened and time is set.              */
/*                (First the attribute files are examined. When a     */
/*                attribute file was found the object files aren't    */
/*                examined and the attribute file is closed).         */
/*                                                                    */
/* RETURN VALUE :  >= 0:        C-ISAM file descriptor                */
/*                 D26_ERROR:   invalid file descriptor               */
/*                                                                    */
/*exoff ***************************************************************/

int d26_i23_open_file(
    signed16 file_nr,   /* IN    -  C-ISAM file number                */
    signed16 oa_flag)   /* IN    -  indicates object or attribute file*/

{                                   /*  d26_i23_open_file             */

				    /* C-ISAM File-Name               */
  char       filename[D27_LFILE_NAME];
  int        fd;                    /* file descriptor                */
  D26_fildes *p_fd;                 /* Pointer to file description of */
				    /* of opened file                 */



  register signed32   i, j;         /* counter                            */
  register D26_fildes *p_act,       /* ptr to actual entry in filetable   */
		      *p_close,     /* ptr to entry of file to close in   */
				    /* filetable                          */
		      *p_search;    /* ptr to begin of searched filetable */
  register signed32   no_files;     /* number of files in act. filetable  */

/* if schema modification is pending and scheme files shall be opened */
/* then files in the subdirectory /chdb have to be opened             */
  if (d26_requestor.d26_sm_flag == D26_SM_PENDING &&
      ((oa_flag == D26_OBJ && file_nr < 1) ||
       (oa_flag == D26_ATT && file_nr < 3)))
  {
    if (oa_flag == D26_OBJ)
    {
      sprintf(filename,"%s%s%s%d",d26_install,D26_CHDB_DIR,D26_OBJECTS,
	file_nr);
    }
    else
    {
      sprintf(filename,"%s%s%s%d",d26_install,D26_CHDB_DIR,D26_ATTR,file_nr);
    }
    i = oa_flag == D26_OBJ ? 0 : 1 + file_nr;
    if (d26_sch_fd[i] < 0)
    {
      if                            /* isopen fails                   */
	((fd = D26_ISOPEN(svc_c_sev_warning,filename,ISINOUT + ISMANULOCK))
	 < 0)
      {
	 return (D26_ERROR);
      }
      d26_sch_fd[i] = fd;
    }
    else
    {
      fd = d26_sch_fd[i];
    }
    return(fd);
  }

				    /*  get pointer to filedescription*/
				    /*  of file to open               */
  p_fd = (oa_flag == D26_OBJ) ? d26_ofildes : d26_afildes;
  p_fd = p_fd + file_nr;


				    /*  fetch file descriptor         */
  if                                /*  file descriptor is invalid    */
    ((fd = p_fd->d26_isfd) < 0)
  {                                 /*  search file to close          */

    /************************/
    /* search file to close */
    /************************/
    p_close = (D26_fildes *) NULL;

    for                             /*  filetables to examine and     */
				    /*  no file to close found        */
       (j = 1; j <= D26_I19_NO_FILETABLES && p_close == (D26_fildes *)NULL;
	j++)
    {                               /*  get filetable to examine      */
      switch (j)                    /* filetable to search            */
      {
	case 1:                     /* first filetable to search      */
				    /* table of attribute files       */
	  p_search = d26_afildes;
	  no_files = d26_nafile;
	  break;
	case 2:                     /* second filetable to search     */
				    /* table of object files          */
	  p_search = d26_ofildes;
	  no_files = d26_nofile;
	  break;
	default:                    /* never occurs                   */
	  break;
      }

				    /*  find file to close:           */
      for                           /*  all files in filetable        */
	  (i = 0; i < no_files; i++)
      {
	p_act = p_search + i;       /*  get actual filetable entry    */
	if                          /*  file not in use but open      */
	   ((p_act->d26_in_use == FALSE) && (p_act->d26_isfd >= 0))
	{
	  if                        /*  a file to close not yet found */
	     (p_close == (D26_fildes *) NULL)
	  {                         /*  set p_close = p_act           */
	    p_close = p_act;
	  }
	  else
	  {                         /*  compare p_close and p_act and */
				    /*  set p_close                   */
	    if (p_act->d26_last_use < p_close->d26_last_use)
	    {                       /* p_close used after p_act       */
	      p_close = p_act;
	    }
	  }
	}
      }
    }

    /***************************************/
    /* close unused file and open new file */
    /***************************************/
    if                              /*  file to close found           */
       (p_close != (D26_fildes *) NULL)
    {                               /*  close unused file             */
      if                            /* isclose fails                  */
	(D26_ISCLOSE(svc_c_sev_warning,p_close->d26_isfd) < 0)
      {
				    /* error trace and return         */
	if (p_close >= d26_afildes && p_close <= d26_afildes + d26_nafile)
	{                           /* attribute file                 */
	  sprintf(filename,file_form,d26_install,D26_ATTR,
	    p_close - d26_afildes);
	}
	else
	{                           /* object file                    */
	  sprintf(filename,file_form,d26_install,D26_OBJECTS,
	    p_close - d26_ofildes);
	}

	return (D26_ERROR);
      }

				    /* set filedescriptor of closed   */
				    /* file                           */
      p_close->d26_isfd = D26_I18_FILE_CLOSED;

				    /* build filename of file to open */
      if                            /* object file                    */
	(oa_flag == D26_OBJ)
      {
	sprintf(filename,file_form,d26_install,D26_OBJECTS,file_nr);
      }
      else
      {                             /* attribute file                 */
	sprintf(filename, file_form, d26_install, D26_ATTR, file_nr);
      }

				    /* open new file                  */
      if                            /* isopen fails                   */
	((fd = D26_ISOPEN(svc_c_sev_warning,filename,ISINOUT + ISMANULOCK))
	 < 0)
      {                             /* error trace and return         */
	return(D26_ERROR);
      }

      p_fd->d26_isfd = fd;          /* set filedescriptor in filetable*/
      D27_022_ENTER_TIME_TRACE(GDS_S_IS_OPENED,filename,0,0);
    }
    else
    {                               /*  error: no file to close found */
      dce_svc_printf(GDS_S_POOL_OPEN_OVFLW_MSG);
      return(D26_ERROR);
    }
  }

/**************************************************/
/* set time of last use and return filedescritpor */
/**************************************************/
  p_fd->d26_last_use = d26_ftime;   /*  set time of last use          */
  return(fd);                       /*  return(C-ISAM file descriptor)*/

}                                   /*  d26_i23_open_file             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_i38_cmp_equal                                   */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 18.02.88                                            */
/*                                                                    */
/* DESCRIPTION  : This function will compare the filter and attribute */
/*                values from message and record using equality rule. */
/*                                                                    */
/* RETURN VALUE :   D2_NO_ERR:              object matches filter     */
/*                  D2_NFOUND               object doesn't match      */
/*                                          filter                    */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i38_cmp_equal(
    signed16       len, /* IN    -  length of filter value            */
			/* IN    -  filter value                      */
    char           *value,
			/* IN    -  number of object file             */
    signed16       ofilenr,
			/* IN    -  record containing base object     */
    char           *record,
			/* IN    -  distinguished name of object found*/
    D2_name_string dname,
    signed16       *pos,/* INOUT -  position of value in record       */
    D26_at         *at) /* IN    -  AT entry of filter attribute      */

{                                   /*  d26_i38_cmp_equal             */

  signed16  atlen;              /* length of attribute value in record*/
  signed16  add_len;            /* length of internal added characters*/
  signed16  phon_len;           /* length of phonetical value         */
  signed16  rep_len;            /* length of representation           */
  signed16  field_factor;       /* occurrency of fields               */
				/* memory for normed attribute value  */
  signed32  normed_val[D26_MAXVALLEN / sizeof(signed32)];
  char      *comp_val;
  D2_obj_id cmp_ob_id;          /* object identifier to compare       */
  signed16  ocl;                /* object class                       */
  signed16  acl_pos;

  register signed16 p_len;      /* length of part of attribute value  */

  phon_len = at->d26a_phon == TRUE ? D26_LPHON : 0;
  rep_len  = at->d26a_rep == D2_T61_PR ? INTSIZE : 0;

  switch(at->d26a_syntax)
  {
    case  D2_CEXACT   :
    case  D2_PRINTABLE:
      if(strlen(record + *pos + phon_len + rep_len) != len)
      {
	*pos += phon_len + rep_len + at->d26a_ubound + 1;
	return(D2_NFOUND);
      }
      else if(memcmp(record + *pos + phon_len + rep_len,value,len))
      {
	*pos += phon_len + rep_len + at->d26a_ubound + 1;
	return(D2_NFOUND);
      }
      break;
    case  D2_TIME     :
      d26_us9_norm_utc_time(value,(char *)normed_val);
      if(strcmp(record + *pos,(char *)normed_val))
      {
	*pos += at->d26a_ubound + 1;
	return(D2_NFOUND);
      }
      break;
    case  D2_CIGNORE  :
    case  D2_IA5      :
    case  D2_CIGN_LIST:
    case  D2_NUMERIC  :
    case  D2_TELNR_STX:
    case  D2_POST_AD_STX:
    case  D2_TLXNR_STX:
    case  D2_COUNTRY_STX:
      field_factor = 2;
      if                /* attribute is a sequence of choice T61-prtbl*/
	(at->d26a_rep == D2_T61_PR_LIST)
      {                 /* set additional length                      */
	add_len = 2 * D26_MAXVALPARTS;
      }
      else if           /* attribute is telex number                  */
	     (at->d26a_syntax == D2_TLXNR_STX)
      {                 /* set additional length                      */
	add_len = D26_MAXVALPARTS;
	field_factor = 1;
      }
      else if           /* attribute is common name of schema object  */
	     (ofilenr == 0)
      {                 /* set additional length                      */
	add_len = D26_CNL - at->d26a_ubound + 1;
	phon_len = D26_LPHON;
      }
      else
      {                 /* set additional length to 1                 */
	add_len = 1;
      }
      comp_val = record + *pos + phon_len + rep_len;
      for(atlen = 0; atlen < at->d26a_ubound + add_len;
	  atlen += p_len + 1, comp_val += p_len + 1)
      {
	if((p_len = strlen(comp_val)) == 0)
	{
	  break;
	}
      }
      atlen--;
      if(atlen != len)
      {
	*pos += phon_len + rep_len + field_factor *
		(at->d26a_ubound + add_len);
	return(D2_NFOUND);
      }
      else
      {
	if (memcmp(record + *pos + phon_len + rep_len,value,len))
	{
	  *pos += phon_len + rep_len + field_factor *
		  (at->d26a_ubound + add_len);
	  return(D2_NFOUND);
	}
      }
      break;
    case  D2_PSAP_STX :
      d26_is14_ld_psap_addr((char *)normed_val,record + *pos,
			    ldint(record + *pos + at->d26a_ubound));
      if(d2p_psap_addr_match((byte *)value,(byte *)normed_val) == FALSE)
      {
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      break;
    case  D2_OBJ_IDENT:
      if(!D2_ATTCMP(&at->d26a_type,D2_OBJ_CLASS))
      {
	cmp_ob_id.d2_typ_len = (signed32)len;
	cmp_ob_id.d2_type    = (Ob_id_string)value;
	ocl = (signed16)ldlong(record + *pos);
	if                  /*  object class or superclass does not   */
			    /*  match                                 */
	  (d26_u16_cmp_superclasses(&cmp_ob_id,ocl) == D2_NFOUND)
	{                   /*  return no match                       */
	  *pos += LONGSIZE;
	  return(D2_NFOUND);
	}
	break;
      }
	  /* handle as octet string otherwise     */
    case  D2_OCTET    :
    case  D2_PASSWD_STX:
      if(ldint(record + *pos + at->d26a_ubound) != len)
      {
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      else if(memcmp(record + *pos,value,len))
      {
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      break;
    case  D2_FAXNR_STX:
      if((atlen = d26_is13_ld_fax_number((char *)normed_val,record + *pos,
	 ldint(record + *pos + at->d26a_ubound))) != len)
      {
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      else if(memcmp((char *)normed_val,value,len))
      {
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      break;
    case  D2_TTXID_STX:
      if((atlen = d26_is12_ld_ttx_term_id((char *)normed_val,record + *pos,
	 ldint(record + *pos + at->d26a_ubound))) != len)
      {
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      else if(memcmp((char *)normed_val,value,len))
      {
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      break;
    case  D2_DISTNAME :
      comp_val = (char *)normed_val;
      atlen = d26_i56_atdnlen(record,pos,at,(D2_name_string *)&comp_val,
	FALSE);
      if(comp_val == (String)NULL)
      {
	return(D2_NFOUND);
      }
      else
      {
	if(strcmp(comp_val,value) == 0)
	{
	  *pos -= LONGSIZE;
	}
	else
	{
	  return(D2_NFOUND);
	}
      }
      break;
    case  D2_ACCL     :
      acl_pos = *pos;
      if((atlen = d26_i57_atacllen(dname,record,&acl_pos,(byte *)normed_val,
	 FALSE)) != len)
      {
	*pos += D2_ACLIDX_MAX * LONGSIZE;
	return(D2_NFOUND);
      }
      else if(memcmp((char *)normed_val,value,len))
      {
	*pos += D2_ACLIDX_MAX * LONGSIZE;
	return(D2_NFOUND);
      }
      break;
    case  D2_BOOLEAN  :
      if((Bool)ldlong(record + *pos) != *(Bool *)value)
      {
	*pos += LONGSIZE;
	return(D2_NFOUND);
      }
      break;
    case  D2_INTEGER  :
      if(ldlong(record + *pos) != *(signed32 *)value)
      {
	*pos += LONGSIZE;
	return(D2_NFOUND);
      }
      break;
    case  D2_MHS_PR_DM_STX:
      for(atlen = 0, comp_val = record + *pos; atlen < len;
	  atlen += LONGSIZE, comp_val += LONGSIZE, value += LONGSIZE)
      {
	if(ldlong(comp_val) != *(signed32 *)value)
	{
	  break;
	}
      }
      if(atlen < len || ldlong(comp_val) >= 0)
      {
	*pos += at->d26a_ubound * LONGSIZE;
	return(D2_NFOUND);
      }
      break;
    case  D2_MHS_DLSP_STX:
      if((atlen = d26_im13_ld_dl_sub_perm((char *)normed_val,record + *pos,
	  ldint(record + *pos + at->d26a_ubound))) != len)
      {
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      else if(memcmp((char *)normed_val,value,len))
      {
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      break;
    case  D2_MHS_ORADR_STX:
      if((atlen = d26_im11_ld_or_address((char *)normed_val,record + *pos,
	  ldint(record + *pos + at->d26a_ubound))) != len)
      {
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      else if(memcmp((char *)normed_val,value,len))
      {
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      break;
    case  D2_MHS_ORNAME_STX:
      if((atlen = d26_im12_ld_or_name((char *)normed_val,record + *pos,
	  ldint(record + *pos + at->d26a_ubound))) != len)
      {
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      else if(memcmp((char *)normed_val,value,len))
      {
	*pos += at->d26a_ubound + INTSIZE;
	return(D2_NFOUND);
      }
      break;
    default           :
				    /* can never happen               */
      return(D2_NFOUND);
  }

				    /*  return no error               */
  return(D2_NO_ERR);

}                                   /*  d26_i38_cmp_equal             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i59_get_valmemsize                                */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        16.04.91                                              */
/*                                                                    */
/* DESCRIPTION: This function calculates the size of memory used by an*/
/*              value in a record. The calculated length is returned  */
/*              as 'return value'. The length includes memory for     */
/*              phonetical value and normed value.                    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     signed32     >0      size of memory for value in bytes         */
/*                 ==0      error (syntax not supported)              */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_i59_get_valmemsize(
    D26_at *p_at)       /* IN    -  AT entry of attribute             */

{                           /*  d26_i59_get_valmemsize                */

  signed32 ret_size = 0;
  signed32 phon_len;
  signed32 add_len;
  signed32 rep_len;             /* length of representation           */

  phon_len = (p_at->d26a_phon == TRUE) ? D26_LPHON : 0;
  rep_len  = (p_at->d26a_rep == D2_T61_PR) ? INTSIZE : 0;
  add_len  = p_at->d26a_rep == D2_T61_PR_LIST ? 2 * D26_MAXVALPARTS :
    (p_at->d26a_rep == D2_TLXNR_STX ? D26_MAXVALPARTS : 1);

  switch (p_at->d26a_syntax)        /*  attribute syntax              */
  {
    case D2_COUNTRY_STX:            /*  Country-Syntax                */
    case D2_CIGNORE:                /*  case-IGNORE-STRING            */
    case D2_IA5:                    /*  case-IGNORE-IA5-String        */
    case D2_POST_AD_STX:
    case D2_CIGN_LIST:
      ret_size = phon_len + rep_len + (2 * (p_at->d26a_ubound + add_len));
      break;
    case D2_NUMERIC:                /*  case-NUMERIC-STRING           */
    case D2_TELNR_STX:
      ret_size = 2 * (p_at->d26a_ubound + add_len);
      break;
    case D2_CEXACT:                 /*  case-EXACT-STRING             */
    case D2_PRINTABLE:              /*  case-PRINTABLE-STRING         */
    case D2_TLXNR_STX:
      ret_size = phon_len + rep_len + p_at->d26a_ubound + add_len;
      break;
    case D2_DISTNAME:               /*  DISTINGUISHED-NAME            */
      ret_size = LONGSIZE;
      break;
    case D2_OBJ_IDENT:              /*  OBJECT-IDENTIFIER             */
      if                            /*  attribute is OBJECT-CLASS     */
	 (D2_ATTCMP(&(p_at->d26a_type), D2_OBJ_CLASS) == 0)
      {
	ret_size = LONGSIZE;
	break;
      }
	 /* goto D2_OCTET */
    case D2_ANY_STX:
    case D2_OCTET:                  /*  OCTET_STRING                  */
    case D2_GUIDE:
    case D2_TTXID_STX:
    case D2_FAXNR_STX:
    case D2_PSAP_STX:
    case D2_PASSWD_STX:
    case D2_CERTIFICATE:
    case D2_CERT_PAIR:
    case D2_CERT_LIST:
      ret_size = p_at->d26a_ubound + INTSIZE;
      break;
    case D2_MHS_DLSP_STX:
    case D2_MHS_ORADR_STX:
    case D2_MHS_ORNAME_STX:
      ret_size = 2 * (p_at->d26a_ubound + INTSIZE);
      break;
    case D2_ACCL:                   /*  ACL-SYNTAX                    */
      ret_size = D2_ACLIDX_MAX * LONGSIZE;
      break;

    case D2_INTEGER:                /*  INTEGER-SYNTAX                */
    case D2_BOOLEAN:                /*  BOOLEAN-SYNTAX                */
      ret_size = LONGSIZE;
      break;
    case D2_TIME:                   /*  TIME-SYNTAX                   */
      ret_size = p_at->d26a_ubound + add_len;
      break;
    case D2_PR_DM_STX:
    case D2_MHS_PR_DM_STX:
      ret_size = p_at->d26a_ubound * LONGSIZE;
      break;
    default:                        /* otherwise                     */
      break;
  }


  return (ret_size);

}                                   /*  d26_i59_get_valmemsize        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i70_cmp_rec_val                                   */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        04.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function compares the given value (of a real     */
/*              recurring attribute) with the value of a read record. */
/*              (The function supplies especially the requirements    */
/*              of the function d26_i18_read_rec_val).                */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     signed16 D2_NO_ERR     given value matches with value of read  */
/*                            record                                  */
/*                                                                    */
/*              D2_NFOUND     given value doesn't match with value    */
/*                            of read record                          */
/*                                                                    */
/*              D26_ERROR     any error occurred (e.g. wrong or not   */
/*                            supplied syntax, phonetic match not     */
/*                            allowed but required)                   */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_i70_cmp_rec_val(
    signed16 len,       /* IN    -  length of searched value          */
    char     *val,      /* IN    -  searched value                    */
    signed32 dnl_idx,   /* IN    -  DN-list index                     */
    char     *rec_pos,  /* IN    -  pointer to value of record        */
			/* IN    -  AT entry of attribute             */
    D26_at   *p_at_entry,
    signed16 rule,      /* IN    -  matching rule for value           */
			/* D2_EQUAL        value matches for equality */
			/* D2_APPROX_MATCH phonetical match           */
			/* D2_SUBSTRINGS   substring match            */
			/* INOUT -  indicator whether next record     */
    boolean  *read_next_rec,        /* should be read                 */
			/* INOUT -  indicator whether searched record */
    signed16 *rec_found)/*          was read                          */

{                                   /* d26_i70_cmp_rec_val            */

  signed16 retval = D2_NO_ERR;
  char     *pos;
  signed16 max_key_len = MAXKEYSIZE - (INTSIZE + LONGSIZE);
  boolean  cmp;
  signed16 rec_val_len;
  char     phon_code[D26_LPHON];
  signed16 match;
  signed16 phon_len;
  signed16 add_len;
  signed16 part_len;
  char     *part_val;
  signed16 addsize;
  signed16 rc;

  pos = rec_pos;

  if                         /* comparision of phonetic code is wished*/
    (rule == D2_APPROX_MATCH)
  {                          /*  compare phonetic code                */
    if (p_at_entry->d26a_phon == TRUE)
    {                        /* phonetic comparision is allowed       */
      d26_soundex ((byte *)val,len,(byte *)phon_code);
      if (memcmp(pos, phon_code, D26_LPHON) != 0)
      {                      /* record doesn't exist                  */
	*rec_found = retval = D2_NFOUND;
			     /* don't read next record                */
      }
      *read_next_rec = FALSE;
      return(retval);
    }
  }

/* get position and length of value in record */
/* and compare non string values              */
  phon_len = (p_at_entry->d26a_phon == TRUE) ? D26_LPHON : 0;
  add_len  = p_at_entry->d26a_rep == D2_T61_PR_LIST ? 2 * D26_MAXVALPARTS :
    (p_at_entry->d26a_rep == D2_TLXNR_STX ? D26_MAXVALPARTS : 1);
  cmp = FALSE;
  switch (p_at_entry->d26a_syntax)  /*  attribute syntax              */
  {
    case D2_COUNTRY_STX:
    case D2_TELNR_STX:
    case D2_NUMERIC:
    case D2_CIGNORE:
    case D2_IA5:
    case D2_TLXNR_STX:
    case D2_PRINTABLE:
    case D2_CEXACT:
	/* calculate position of normed or exact record value         */
      pos += phon_len + (p_at_entry->d26a_rep == D2_T61_PR ? INTSIZE : 0);
      rec_val_len = strlen (pos);    /*  length of record value        */
      cmp = TRUE;                 /*  set indicator for comparision */
      break;

    case D2_CIGN_LIST:
    case D2_POST_AD_STX:
	/* calculate position first list value */
      pos += phon_len;
	/* calculate total length of all list values */
      for(rec_val_len = 0, part_val = pos;
	  rec_val_len < p_at_entry->d26a_ubound + add_len;
	  rec_val_len += part_len + 1, part_val += part_len + 1)
      {
	if((part_len = strlen(part_val)) == 0)
	{
	  break;
	}
      }
      rec_val_len--;
      /* set indicator for comparision */
      cmp = TRUE;
      break;
    case D2_OBJ_IDENT:
    case D2_PASSWD_STX:
    case D2_OCTET:
      /* get length of record value    */
       rec_val_len = ldint (pos + p_at_entry->d26a_ubound);
       cmp = TRUE;                 /*  set indicator for comparision */
       break;
    case D2_PSAP_STX:
    case D2_TTXID_STX:
    case D2_FAXNR_STX:
    case D2_MHS_DLSP_STX:
    case D2_MHS_ORADR_STX:
    case D2_MHS_ORNAME_STX:
    case D2_TIME:
    case D2_MHS_PR_DM_STX:
       /* comapre value for equality (substring never occurs) */
      addsize = 0;
      rc = d26_i38_cmp_equal(len,val,D26_IDX_NOT_DEF,pos,
	(D2_name_string)NULL,&addsize,p_at_entry);
      if                /* value does not match                       */
	(rc != D2_NO_ERR)
      {
	*rec_found = retval = D2_NFOUND;    /* record doesn't exist   */
      }
      else
      {                                     /* don't read next record */
	*read_next_rec = FALSE;
      }
      break;
    case D2_INTEGER:
      /* comapre value for equality (substring never occurs) */
      if                            /*  value doesn't match           */
	 ((*(signed32 *) val) != ldlong (pos))
      {    /* record doesn't exist           */
	   *rec_found = retval = D2_NFOUND;
	   *read_next_rec = FALSE; /* don't read next record  */
      }
      break;
    case D2_BOOLEAN:
      /* comapre value for equality (substring never occurs) */
      if                            /*  value doesn't match           */
	 ((*(Bool *) val) != (Bool) ldlong (pos))
      {                             /* record doesn't exist           */
	*rec_found = retval = D2_NFOUND;
	*read_next_rec = FALSE; /* don't read next record */
      }  /* value doesn't match */
      break;
    case D2_DISTNAME:
      /* comapre value for equality                  */
      /* (substring handled by d26_i71_read_dn_sub ) */
      if                            /*  value doesn't match           */
	(dnl_idx != ldlong(pos))
      {    /* record doesn't exist           */
	*rec_found = retval = D2_NFOUND;
	*read_next_rec = FALSE; /* don't read next record */
      }
      break;
    case D2_ACCL:
      *read_next_rec = FALSE;
      *rec_found = retval = D26_ERROR;
      break;
    default:                        /* otherwise                      */
      *read_next_rec = FALSE;
      *rec_found = retval = D26_ERROR;
      break;

  }

/* compare string values */
  if                    /*  indicator for comparision is set          */
    (cmp == TRUE)
  {                     /*  compare value                             */
    if (len < max_key_len)
    {                   /* don't read next record                     */
      *read_next_rec = FALSE;
      match = memcmp (val,pos,len);
      if                /* (value doesn't match for given len for     */
			/*  substring search) and                     */
			/* ((len of value != length of record value)  */
			/*  or (vaule doesn't match record value))    */
	(((rule == D2_SUBSTRINGS) && (match != 0)) &&
	 ((len != rec_val_len) || (match != 0)))
      {                 /* record doesn't exist                       */
	*rec_found = retval = D2_NFOUND;
      }
    }
    else
    {                  /* len >= max_key_len                         */
      if               /* value doesn't match upto max_key_len       */
	(memcmp(val,pos,max_key_len) != 0)
      {                /* record doesn't exist                       */
	*rec_found = retval = D2_NFOUND;
		       /* don't read next record                     */
	*read_next_rec = FALSE;
      }
      else
      {                /* compare rest of value                      */
	match = memcmp(&val[max_key_len],&pos[max_key_len],
	  len - max_key_len);
	if             /* (value matches upto value length for       */
		       /*  substring search) or                      */
		       /* ((value length == record value length) and */
		       /*  (value matches upto value length))        */
	  (((rule == D2_SUBSTRINGS) && (match == 0))
	   || ((len == rec_val_len) && (match == 0)))
	{              /* searched record was found                  */
		       /* don't read next record                     */
	  *read_next_rec = FALSE;
	}
	else
	{              /* searched rec. wasn't found, read next rec. */
	  retval = D2_NFOUND;
	}
      }
    }
  }

  return (retval);

}                                   /* d26_i70_cmp_rec_val            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i71_read_dn_sub                                   */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        14.04.88                                              */
/*                                                                    */
/* DESCRIPTION: This function reads one record from a file of a real  */
/*              recurring attribute when a substring at the begin     */
/*              of a distinguished name is given. The record isn't    */
/*              locked, the mode is ISEQUAL and isam key is set.      */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      signed16 D2_NO_ERR       record found                         */
/*                                                                    */
/*              D2_NFOUND        record doesn't exist                 */
/*                                                                    */
/*              D26_ERROR        any error occurred (C-ISAM error     */
/*                               occurred)                            */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_i71_read_dn_sub(
			/* IN    -  interpretation of given           */
  char           dn_interpret,      /* distinguished name             */
  signed16       dn_len,/* IN    -  length of substring               */
			/* IN    -  given substring (the value must be*/
  D2_name_string dn_sub,            /* normed)                        */
  signed32       key,   /* IN    -  C-ISAM key of main entry          */
			/* IN    -  object file number                */
  signed16       obj_filenr,
			/* IN    -  AT entry of attribute             */
  D26_at         *p_at_entry,
  int            isfd,  /* IN    -  filedescriptor of attribute file  */
			/* IN    -  selected isam key                 */
  struct keydesc *isam_key,
			/* IN    -  record position of attribute value*/
  char           *val_pos,
			/* INOUT -  record containing attribute value */
  char           *record,
			/* INOUT -  DN-list index                     */
  signed32       *start_dnl_idx)

{                               /*  d26_i71_read_dn_sub               */

  static char *      function = "i71";

  signed16         return_value = D2_NO_ERR;
  int              key_len;
			    /* counter for isreads                    */
  signed32         ir_count = 0;

  register signed32 dnl_idx;


			    /*  store obj_filenr and key in record    */
  stint (obj_filenr, record);
  stlong (key, record + INTSIZE);


/****************/
/* call isstart */
/****************/
  if                         /*  dn_len == 0                          */
    (dn_len <= 0)
  {                          /*  Error: invalid value for dn_len      */
    *start_dnl_idx = 0L;
    return (D26_ERROR);
  }
  else
  {                          /*  set key via isstart                  */
    key_len = 0;
			     /* call isstart                          */
    d26_dbacc_count++;
    if (D26_ISSTART(svc_c_sev_warning,isfd,isam_key,key_len,record,ISFIRST)
	 < 0)
    {
      *start_dnl_idx = 0L;
      return (D26_ERROR);
    }
  }

/*************************************/
/* search first record which matches */
/*************************************/
  dnl_idx = *start_dnl_idx;

  while                 /*  dn in dn list matches                     */
       ((dnl_idx = d26_u79_get_sstr_dnlidx(dnl_idx,dn_interpret,dn_sub,
	dn_len)) >= 0)
  {                     /*  read record                               */
			/* store searched value in record             */
    stlong(dnl_idx, val_pos);
			/* read record                                */
    ir_count++;
    d26_dbacc_count++;
    if                  /*  no record found                           */
      (D26_ISREAD(svc_c_route_nolog,isfd,record,ISEQUAL) < 0)
    {                   /*  get next dnl_idx which matches (d26_u79)  */
      dnl_idx++;
      switch (iserrno)  /* errorcode from isread                      */
      {
	case ENOREC:    /* no such record                             */
	  return_value = D2_NFOUND;
	  break;
	default:        /* other C isam error occured                 */
	  dce_svc_printf(GDS_S_ISREAD_ERR_MSG,ISEQUAL,iserrno);
	  return_value = D26_ERROR;
	  break;
      }                 /* errorcode from isread                      */
    }
    else
    {                   /*  searched record found                     */
      *start_dnl_idx = dnl_idx;
      D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,1,ir_count);
      return (D2_NO_ERR);
    }
  }
			/* record not found                           */
  *start_dnl_idx = 0L;
  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,1,ir_count);
  return (return_value);
}                                   /* d26_i71_read_dn_sub            */
