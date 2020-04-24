/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: retutilis.c,v $
 * Revision 1.1.739.2  1996/02/18  18:16:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:37  marty]
 *
 * Revision 1.1.739.1  1995/12/08  15:51:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:09  root]
 * 
 * Revision 1.1.737.4  1994/06/21  14:46:29  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:55:17  marrek]
 * 
 * Revision 1.1.737.3  1994/05/10  15:53:30  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:29:58  marrek]
 * 
 * Revision 1.1.737.2  1994/03/23  15:14:08  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:27:07  keutel]
 * 
 * Revision 1.1.737.1  1994/02/22  17:37:56  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:27:44  marrek]
 * 
 * Revision 1.1.735.2  1993/10/14  17:27:22  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:25:02  keutel]
 * 
 * Revision 1.1.735.1  1993/10/13  17:31:48  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:31:52  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  16:19:19  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:32:39  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  15:01:39  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  11:37:14  marrek
 * 	Interworking with paradise
 * 	[1993/02/01  15:49:48  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:45:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:23:22  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:41:02  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:29:33  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:57:21  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:36:22  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:04:29  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:35:12  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: retutilis.c,v $ $Revision: 1.1.739.2 $ $Date: 1996/02/18 18:16:03 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : retutilis.c     [retutilis]                         */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 10.05.88                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains ISAM utilities used by the    */
/*                  internal search function.                         */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 10.05.88| birth                          | ek |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*                                                                    */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

/*****  external Includes    *****/

#include <dce/d27util.h>

/*****  internal Includes    *****/

#include <d26schema.h>
#include <d26isam.h>
#include <d26svcis.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

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

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i31_find_object                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        21.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function searches the DIB for a special object   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   signed16    no error occurred                     */
/*      D2_NFOUND               Object not found                      */
/*      D26_ERROR               C-Isam error occurred                 */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i31_find_object(
    D26_dninfo *dninfo, /* INOUT -  information about object to find  */
    D26_rdninfo **l_rdn,/* OUT   -  pointer to last RDN               */
    char *record)       /* OUT   -  record containing object          */

{                               /*  d26_i31_find_object               */

			    /* function identifier for traces         */
  static char *function = "d26_i31_find_object";

  signed16    ob_nr;        /* object number                          */
  D26_srt     *srt;         /* pointer to srt                         */
  D26_ocl_arc *ocl_arc;     /* pointer to object class arc            */
  D26_at      *i_at;        /* pointer to at entry to use for index   */
  D26_rdninfo *i_rdn;       /* pointer to RDN to use for index        */
  D26_ava     *i_ava;       /* pointer to AVA to use for index        */
  signed16    ind_level;    /* highest index level of nameparts       */
  signed16    pos;          /* position for indexed attribute         */
  D26_keydesc key;          /* ISAM key structure                     */
  signed16    len;          /* length of key                          */
  int         isfd;         /* ISAM file descriptor                   */
  int         mode;         /* ISAM read mode                         */
  char        *np;          /* pointer to name part in record         */
  signed16    ret_value;    /* return value                           */

			    /* counter for isstart calls              */
  signed32    is_count = 0;
			    /* counter for isread calls               */
  signed32    ir_count = 0;

  register D26_rdninfo *rdn;/* loop pointer to RDN                    */
  register D26_at      *at; /* pointer to at entry to use for index   */

  *l_rdn  = dninfo->d26_rdn + dninfo->d26_rd_anz - 1;
  ob_nr   = (*l_rdn)->d26_r_idx;
  srt     = d26_srt + ob_nr;

  for                   /*  all nameparts                             */
     (rdn = *l_rdn, ind_level = -1; rdn >= dninfo->d26_rdn; rdn--)
  {                     /*  check index-level of naming attribute     */
    at = d26_at + rdn->d26_r_ava[0].d26_a_idx;
    if                  /*  new index level higher than old one       */
      (at->d26a_ind_level > ind_level)
    {                   /*  assign at-pointer to use as index         */
      if                /*  index level greater than maximum allowed  */
	(at->d26a_ind_level > D2_MAX_ADM_IND_LEVEL)
      {                 /*  reset index level and pointers            */
	ind_level = at->d26a_ind_level;
	i_at  = at;
	i_rdn = rdn;
      }
      else if           /*  index level is not yet set                */
	     (ind_level == -1)
      {                 /*  reset index level and pointers            */
	ind_level = 0;
	i_at  = at;
	i_rdn = rdn;
      }
    }
  }

  for                   /*  all assigned OCT-entries                  */
     (ocl_arc = srt->d26s_ocl_arc, dninfo->d26_filenr = -1,
      ret_value = D2_NFOUND; ocl_arc->d26s_oclass >= 0; ocl_arc++)
  {                     /*  compare filenumbers                       */
    if                  /*  file was not yet searched                 */
      (d26_oct[ocl_arc->d26s_oclass].d26c_filenr > dninfo->d26_filenr)
    {                   /*  reset the filenumber                      */
      dninfo->d26_filenr = d26_oct[ocl_arc->d26s_oclass].d26c_filenr;
      if                /*  file could not be opened                  */
	((isfd = d26_i23_open_file(dninfo->d26_filenr,D26_OBJ)) < 0)
      {                 /*  reset return value                        */
	ret_value = D26_ERROR;
      }
      else
      {                 /*  select key                                */
	pos = d26_srt[i_rdn->d26_r_idx].d26s_nam_at[0].
					d26s_position[dninfo->d26_filenr];
	d26_i32_select_key(&key,pos,i_at,0,(D2_a_match)D2_EQUAL,
			   dninfo->d26_filenr);
			/*  fill key in record                        */
	stint(ob_nr,record + key.k_part[0].kp_start);
	i_ava = i_rdn->d26_r_ava;
	len = i_ava->d26_a_len > key.k_part[1].kp_leng - 1 ?
	      key.k_part[1].kp_leng - 1 : i_ava->d26_a_len;
	sprintf(record + key.k_part[1].kp_start,"%*.*s",i_ava->d26_a_len,
		i_ava->d26_a_len,i_ava->d26_a_val);
	is_count++;
	d26_dbacc_count++;
	if              /*  error from start query                    */
	  (D26_ISSTART(svc_c_route_nolog,isfd,&key,INTSIZE + len + 1,record,
	   ISEQUAL) < 0)
	{               /*  check error number                        */
	  if            /*  hard error occurred                       */
	    (iserrno != ENOREC)
	  {             /*  log error                                 */
	    dce_svc_printf(DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	      svc_c_sev_warning,GDS_S_ISSTART_ERR,iserrno);
	    ret_value = D26_ERROR;
	  }
	}
	else
	{               /*  set query mode                            */
	  mode = ISCURR;
	  for(D2_EVER)
	  {             /*  read isam                                 */
	    ir_count++;
	    d26_dbacc_count++;
	    if          /*  error from read isam                      */
	      (D26_ISREAD(svc_c_route_nolog,isfd,record,mode) < 0)
	    {           /*  check error                               */
	      switch(iserrno)
	      {
		case  ELOCKED :
		  sleep(1);
		  continue;
		case  EENDFILE:
		  break;
		default       :
		  dce_svc_printf(DCE_SVC(gds_svc_handle,"%d%d"),
		    GDS_S_GENERAL,svc_c_sev_warning,GDS_S_ISREAD_ERR,mode,
		    iserrno);
		  ret_value = D26_ERROR;
	      }
	      break;
	    }
	    else
	    {           /*  set name pointer                          */
	      np = record + key.k_part[1].kp_start;
	      if        /*  record matches key                        */
		(ldint(record + key.k_part[0].kp_start) == ob_nr &&
		 !strncmp(np,(char *)i_ava->d26_a_val,len) &&
		 (INTSIZE + len + 1 + LONGSIZE < MAXKEYSIZE &&
		  strlen(np) == i_ava->d26_a_len) ||
		  INTSIZE + len + 1 + LONGSIZE == MAXKEYSIZE)
	      {         /*  check all RDNs                            */
		if      /*  all RDNs match                            */
		  (d26_i63_check_name(dninfo,dninfo->d26_rd_anz,record) < 0)
		{       /*  return ok                                 */
		  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,
		    is_count,ir_count);
		  return(D2_NO_ERR);
		}
		else
		{       /*  reset the mode                            */
		  mode = ISNEXT;
		}
	      }
	      else
	      {         /*  break loop and query next file            */
		break;
	      }
	    }
	  }
	}
      }
    }
  }

  dninfo->d26_filenr = -1;
  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count,
    ir_count);
  return(ret_value);

}                       /*  d26_i31_find_object                       */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i32_select_key                                    */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        21.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function builds an  ISAM key assigned to a speci-*/
/*              fied attribute.                                       */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              pos         Short           position in record to use */
/*                                          for index                 */
/*              at          D26_at *        attribute description     */
/*              av_nr       Short           number of attribute value */
/*                                          carrying the index        */
/*              rule        D2_a_match      indicates matching rule   */
/*                                          to be chosen for index.   */
/*              filenr      Short           number of C-Isam-file     */
/*                                          present only if master-   */
/*                                          knowledge attribute  must */
/*                                          be part of the index      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              key         D26_keydesc *   pointer to ISAM-key       */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*exoff ***************************************************************/

void d26_i32_select_key(D26_keydesc *key, signed16 pos, D26_at *at,
    signed16 av_nr, D2_a_match rule, signed16 filenr)

{                               /*  d26_i32_select_key                */

  signed16 phon_len;        /* length of phonetical part of attribute */
  signed16 rep_len;         /* length of representation part of       */
			    /* attribute value                        */
  signed16 count_part;      /* count of keypart                       */
  signed16 reduce_len;      /* length to reduce keypart 1             */

key->k_flags  = ISDUPS + COMPRESS;

				/*  set keypart object number         */
key->k_part[0].kp_start = LONGSIZE;
key->k_part[0].kp_leng  = INTSIZE;
key->k_part[0].kp_type  = INTTYPE;
reduce_len = INTSIZE;

count_part = 1;
if                              /*  attribute value for key given     */
  (pos > 0 && at != (D26_at *)NULL)
{                               /*  set keypart attribute value       */
  key->k_part[count_part].kp_start = pos + INTSIZE;
  phon_len = at->d26a_phon == TRUE || filenr == 0 ? D26_LPHON : 0;
  rep_len  = at->d26a_rep == D2_T61_PR ? INTSIZE : 0;
  switch(at->d26a_syntax)
  {
    case D2_COUNTRY_STX:
    case D2_CIGNORE   :
    case D2_IA5       :
    case D2_NUMERIC   :
    case D2_TELNR_STX :
      key->k_part[count_part].kp_start += av_nr *
	   (phon_len + rep_len + 2 * (at->d26a_ubound + 1)) +
	   (rule != D2_APPROX_MATCH) * (phon_len + rep_len);
      key->k_part[count_part].kp_leng =
	   (rule == D2_APPROX_MATCH) ? D26_LPHON :
	   ((filenr == 0 ? D26_CNL : at->d26a_ubound) + 1);
      key->k_part[count_part].kp_type  = CHARTYPE;
      break;
    case D2_CIGN_LIST :
    case D2_POST_AD_STX:
      key->k_part[count_part].kp_start += av_nr *
			      (2 * (at->d26a_ubound + 2 * D26_MAXVALPARTS));
      key->k_part[count_part].kp_leng = at->d26a_ubound + 2 *D26_MAXVALPARTS;
      key->k_part[count_part].kp_type = CHARTYPE;
      break;
    case D2_TLXNR_STX :
      key->k_part[count_part].kp_start += av_nr *
			      (at->d26a_ubound + D26_MAXVALPARTS);
      key->k_part[count_part].kp_leng = at->d26a_ubound + D26_MAXVALPARTS;
      key->k_part[count_part].kp_type = CHARTYPE;
      break;
    case D2_TIME      :
    case D2_PRINTABLE :
    case D2_CEXACT    :
      key->k_part[count_part].kp_start += av_nr *
	   (phon_len + rep_len + at->d26a_ubound + 1) +
	   (rule != D2_APPROX_MATCH) * (phon_len + rep_len);
      key->k_part[count_part].kp_leng =
	   (rule == D2_APPROX_MATCH) ? D26_LPHON : at->d26a_ubound + 1;
      key->k_part[count_part].kp_type = CHARTYPE +
					(rule == D2_LTEQ ? ISDESC : 0);
      break;
    case D2_OBJ_IDENT :
      if(!D2_ATTCMP(&at->d26a_type,D2_OBJ_CLASS))
      {   
	key->k_part[count_part].kp_start += av_nr * LONGSIZE;
	key->k_part[count_part].kp_leng = LONGSIZE;
	key->k_part[count_part].kp_type  = LONGTYPE;
	break;
      } 
			/* handle like octet string otherwise         */
    case D2_PSAP_STX  :
    case D2_PASSWD_STX:
    case D2_OCTET     :
    case D2_TTXID_STX:
    case D2_FAXNR_STX:
    case D2_MHS_DLSP_STX:
    case D2_MHS_ORADR_STX:
    case D2_MHS_ORNAME_STX:
      key->k_part[count_part].kp_start += av_nr *
	   (at->d26a_ubound + INTSIZE);
      key->k_part[count_part].kp_leng = at->d26a_ubound;
      key->k_part[count_part].kp_type = CHARTYPE +
	   (rule == D2_LTEQ ? ISDESC : 0);
      break;
    case D2_MHS_PR_DM_STX:
      key->k_part[count_part].kp_start += av_nr * at->d26a_ubound * LONGSIZE;
      key->k_part[count_part].kp_leng = at->d26a_ubound * LONGSIZE;
      key->k_part[count_part].kp_type = CHARTYPE;
      break;
    case D2_BOOLEAN:
    case D2_INTEGER:
    case D2_DISTNAME  :
      key->k_part[count_part].kp_start += av_nr * LONGSIZE;
      key->k_part[count_part].kp_leng = LONGSIZE;
      key->k_part[count_part].kp_type  = LONGTYPE +
	   (rule == D2_LTEQ ? ISDESC : 0);
      break;
    case D2_ACCL      :
      key->k_part[count_part].kp_leng = D2_ACLIDX_MAX * LONGSIZE;
      key->k_part[count_part].kp_type  = CHARTYPE;
      break;
			/* Not supported until now                    */
    default           :
      key->k_part[count_part].kp_leng = 0;
      key->k_part[count_part].kp_type  = CHARTYPE;
      break;
  }   
  count_part++;
}                       

if                      /*  master knowledge is part of index         */
  (filenr >= 0 && rule != D2_APPROX_MATCH)
{                       /*  set aliased object and master knowledge   */
			/*  parts                                     */
  key->k_part[count_part].kp_start =
       d26_at[d26_alob_idx].d26a_field_pos[filenr];
  key->k_part[count_part].kp_leng  = INTSIZE;
  key->k_part[count_part].kp_type  = INTTYPE;
  reduce_len += INTSIZE;
  count_part++;

  key->k_part[count_part].kp_start =
       d26_at[d26_mk_idx].d26a_field_pos[filenr] + INTSIZE;
  key->k_part[count_part].kp_leng  = LONGSIZE;
  key->k_part[count_part].kp_type  = LONGTYPE;
  reduce_len += LONGSIZE;
  count_part++;
}                       

key->k_part[count_part].kp_start = 0;
key->k_part[count_part].kp_leng  = LONGSIZE;
key->k_part[count_part].kp_type  = LONGTYPE;
reduce_len += LONGSIZE;
count_part++;

key->k_part[1].kp_leng = key->k_part[1].kp_leng + reduce_len <= MAXKEYSIZE ?
			 key->k_part[1].kp_leng : MAXKEYSIZE - reduce_len;
key->k_nparts = count_part;

}                       /*  d26_i32_select_key                        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i33_deref_alias                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        02.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function returns the normed and the exact DN     */
/*              of the aliased object referenced by al_idx.           */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      al_idx      Long            DN-List-index of aliased object   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      normed_obj  D2_name_string  normed DN of aliased object       */
/*      exact_obj   D2_name_string  exact DN of aliased object        */
/*      dninfo      D26_dninfo *    pointer to information about      */
/*                                  aliased object                    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Short          -1        alias could not be dereferenced       */
/*                   >=0        number of RDNs in aliased object      */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i33_deref_alias(signed32 al_idx, D2_name_string normed_obj,
    D2_name_string exact_obj, D26_dninfo *dninfo)

{                               /*  d26_i33_deref_alias               */

  D26_dndesc  *dndesc;      /* pointer to DN description              */

d26_u11_check_dn_index(al_idx,&dndesc);
if                              /*  No such entry in DN-List          */
  (dndesc == NULL)
{                               /*  return "Not found"                */
  return(-1);
}
else if                         /*  DN-List-entry is invalid          */
       (dndesc->d26_occurrency == 0 || dndesc->d26_dnint != D2_SOBJ)
{                               /*  return "Not found"                */
  return(-1);
}
else
{                               /*  copy the distinguished names      */
  strcpy((char *)normed_obj,(char *)dndesc->d26_dnname);
  strcpy((char *)exact_obj,(char *)dndesc->d26_ename);
  dninfo->d26_rd_anz = 0;
  d26_u06_get_name(normed_obj,dninfo);
} 

return(dninfo->d26_rd_anz);

}                       /*  d26_i33_deref_alias                       */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i34_get_dninfo()                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        02.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function fills the structure dninfo from record  */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              record      String          ISAM record containing    */
/*                                          the object.               */
/*              np_count    Short           count of nameparts in root*/
/*                                          object.                   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              dninfo      D26_dninfo *    pointer to DN information */
/*              dname       D2_name_string *pointer where DN has to   */
/*                                          be written to             */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*exoff ***************************************************************/

void d26_i34_get_dninfo(char *record, D26_dninfo *dninfo,
    D2_name_string *dname, signed16 np_count)

{                               /*  d26_i34_get_dninfo                */

  signed16    root;             /* object number of root object       */
  signed16    ob_nr;            /* object number of read object       */
  signed16    par_nr;           /* parent number of read object       */
  D26_srt     *srt;             /* pointer to SRT                     */
  D26_at      *at;              /* pointer to AT                      */
  D26_rdninfo *rdn;             /* pointer to RDN                     */
  signed16    pos;              /* position in record                 */
  char        separator;        /* separator character between type   */
				/* and value                          */

  register signed16 i;          /* loop variable                      */
  register D26_nam_at *nam_at;  /* loop pointer to naming attributes  */
  register D26_ava    *ava;     /* loop pointer to AVAs               */

par_nr = ob_nr = ldint(record + LONGSIZE);

root = np_count > 0 ? dninfo->d26_rdn[np_count - 1].d26_r_idx : D26_ROOT;

for                     /*  all ancestors                             */
   (i = 0, par_nr = ob_nr;
    (par_nr = d26_srt[par_nr].d26s_par_nr) != root; i++)
{                       /*  count                                     */
}                       

dninfo->d26_rd_anz = np_count + i + 1;

for                     /*  all ancestors                             */
   (par_nr = ob_nr, rdn = dninfo->d26_rdn + np_count + i,
    srt = d26_srt + ob_nr; i >= 0; i--, srt = d26_srt + par_nr, rdn--)
{                       /*  assign srt-indices                        */
  rdn->d26_r_idx  = par_nr;
  for			/*  all naming attributes of SRT entry	      */
     (rdn->d26_r_nava = 0, ava = rdn->d26_r_ava, nam_at = srt->d26s_nam_at;
      nam_at->d26s_index != -1; rdn->d26_r_nava++, ava++, nam_at++)
  { 			/*  assign attribute index and type           */
    ava->d26_a_idx  = nam_at->d26s_index;
    par_nr          = srt->d26s_par_nr;
  }                     
}                       

for                     /*  all RDN's                                 */
   (i = 0, rdn = dninfo->d26_rdn + np_count;
    np_count + i < dninfo->d26_rd_anz; i++, rdn++)
{                       /*  copy values from record                   */
  srt = d26_srt + rdn->d26_r_idx;
  for			/*  all naming attributes of SRT entry	      */
     (ava = rdn->d26_r_ava, nam_at = srt->d26s_nam_at;
      nam_at->d26s_index != -1; ava++, nam_at++)
  { 			/*  convert object identifier 		      */
    at = d26_at + nam_at->d26s_index;
    d27_301_oid_str(&at->d26a_type,(char *)*dname);
    *dname += strlen((char *)*dname);
    pos = nam_at->d26s_position[dninfo->d26_filenr] + INTSIZE +
				(at->d26a_phon == TRUE ? D26_LPHON : 0);
    switch(at->d26a_rep)
    {
      case D2_T61_PR:
	pos += INTSIZE;
	/* FALLTHROUGH */
      case D2_PRINTABLE:         /* analog to d27_007_norm_name */
	separator = D2_T61_REP;
	break;
      case D2_NUMERIC:
	separator = D2_NUM_REP;
	break;
      case D2_IA5:
	separator = D2_IA5_REP;
	break;
    }   
    sprintf((char *)*dname,"%c%s%c",separator,record + pos,D2_AVA_SEP);
    ava->d26_a_val = ++*dname;
    ava->d26_a_len = strlen((char *)*dname) - 1;
    *dname += ava->d26_a_len + 1;
  }                     
  *(*dname - 1) = D2_RDN_SEP;
}                       
*(*dname - 1) = D2_EOS;

}                       /*  d26_i34_get_dninfo                        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME:        d26_i35_reread_object                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        21.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function rereads and eventually locks the current*/
/*              record                                                */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   signed16    no error occurred                     */
/*      D2_NFOUND               no such record                        */
/*      D26_ERROR               C-isam error occurred                 */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i35_reread_object(
    signed16 filenr,    /* IN    -  number of object file             */
			/* IN    -  indicates whether record must be  */
    boolean lock)       /*          locked                            */

{                               /*  d26_i35_reread_object             */

			    /* primary key for objects file           */
  static D26_keydesc key = {ISNODUPS,1,{{0,LONGSIZE,LONGTYPE}}};

			    /* function identifier for traces         */
  static char *function = "d26_i35_reread_object";

  int  isfd;                /* ISAM file descriptor                   */
  int  mode;                /* ISAM read mode                         */

  if                                /*  error from open file          */
    ((isfd = d26_i23_open_file(filenr,D26_OBJ)) < 0)
  {                                 /*  return error                  */
    return(D26_ERROR);
  }
  else if               /*  error from starting query                 */
	 (D26_ISSTART(svc_c_route_nolog,isfd,&key,0,d26_ob_record,ISEQUAL)
	  < 0)
  {                     /* check isam error                           */
    d26_dbacc_count++;
    D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,1,0);
    switch(iserrno)
    {
      case  ENOREC:
	return(D2_NFOUND);
      default:
	dce_svc_printf(DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	  svc_c_sev_warning,GDS_S_ISSTART_ERR,iserrno);
	return(D26_ERROR);
    }
  }
  else
  {                     /* read isam record with lock                 */
    d26_dbacc_count += 2;
    mode = ISCURR + ISWAIT + (lock == TRUE ? ISLOCK : 0);
    if                  /* error from read isam                       */
      (D26_ISREAD(svc_c_route_nolog,isfd,d26_ob_record,mode) < 0)
    {                   /*  check isam error                          */
      D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,1,1);
      switch(iserrno)
      {
	case ENOCURR:
	  return(D2_NFOUND);
	default:
	  dce_svc_printf(DCE_SVC(gds_svc_handle,"%d%d"),GDS_S_GENERAL,
	    svc_c_sev_warning,GDS_S_ISREAD_ERR,mode,iserrno);
	  return(D26_ERROR);
      }
    }
    D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,1,1);
  }

			/*  set in use flag for file                  */
  d26_ofildes[filenr].d26_in_use = TRUE;

			/*  return no error                           */
  return(D2_NO_ERR);

}                               /*  d26_i35_reread_object             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i36_read_ocl_acl_mk                               */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        10.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function reads the object-class, acl and master  */
/*              knowledge of an object from record. In case of an     */
/*              alias the superior node is read into a second record  */
/*              and this record is used.                              */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      dninfo      D26_dninfo *    information about found object    */
/*      record      String          record containing object          */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      ocl         Long *          object class of object            */
/*      acl         D26_acl *       acl of object                     */
/*      mk          Long *          index of master DSA in DN-list    */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*exoff ***************************************************************/

void d26_i36_read_ocl_acl_mk(D26_dninfo *dninfo, char *record, signed32 *ocl,
    D26_acl *acl, signed32 *mk)

{                               /*  d26_i36_read_ocl_acl_mk           */

  D26_dninfo  al_dninfo;    /* DN-information for reading for alias   */
  D26_rdninfo *l_rdn;       /* pointer to last namepart               */
  signed16    filenr;       /* number of objects file                 */
  signed16    pos;          /* position of attribute in record        */
  signed16    count;        /* count of attribute values              */

  register signed16 i;      /* loop variable                          */

filenr = dninfo->d26_filenr;

pos   = d26_at[d26_obcl_idx].d26a_field_pos[filenr];
count = ldint(record + pos);

for                     /*  all object class values                   */
   (i = 0; i < count; i++)
{                       /*  read in object class                      */
  ocl[i]  = ldlong(record + pos + INTSIZE + i * LONGSIZE);
}                       
ocl[i] = D26_EOL;

*mk = D26_IDX_NOT_DEF;

if                      /*  object is alias                           */
  (*ocl == d26_ali_idx)
{                       /*  look for superior node                    */
  if                    /*  alias has superior node                   */
    (dninfo->d26_rd_anz > 1)
  {                     /*  copy DN-info for superior node            */
    al_dninfo = *dninfo;
    al_dninfo.d26_rd_anz--;
  }
  else
  {                     /*  get DN-Info from schema                   */
    al_dninfo.d26_rd_anz = 0;
    d26_u06_get_name(d26_schema_name,&al_dninfo);
    *mk = (signed32)D26_OWNNAME;
  }                     
  d26_reset = TRUE;
  switch(d26_i31_find_object(&al_dninfo,&l_rdn,d26_sn_record))
  {
    case D2_NO_ERR:     /*  assign record and new file number         */
      record = d26_sn_record;
      filenr = al_dninfo.d26_filenr;
      break;
    case D2_NFOUND:     /*  assign default master knowledge and acl   */
      *mk = (signed32)D26_OWNNAME;
      for               /*  all acl indices                           */
	 (i = 0; i < D2_ACLIDX_MAX; i++)
      {                 /*  set acl subtree                           */
	*acl++ = D26_TREE;
      }                 
      return;
    case D26_ERROR:     /*  assign invalid master knowledge and acl   */
      *mk = D26_IDX_NOT_DEF;
      for               /*  all acl indices                           */
	 (i = 0; i < D2_ACLIDX_MAX; i++)
      {                 /*  set acl subtree                           */
	*acl++ = D26_IDX_NOT_DEF;
      }                 
      return;
  }                     
}                       

if                      /*  master-knowledge not yet defined          */
  (*mk == D26_IDX_NOT_DEF)
{                       /*  get it from the read record               */
  pos = d26_at[d26_mk_idx].d26a_field_pos[filenr];
  *mk = ldlong(record + pos + INTSIZE);
}                       

pos = d26_at[d26_acl_idx].d26a_field_pos[filenr];
record += pos;

for                     /*  all acl indices                           */
   (i = 0, record += INTSIZE; i < D2_ACLIDX_MAX; i++, record += LONGSIZE)
{                       /*  read acl index                            */
  *acl++ = ldlong(record);
}                       

}                       /*  d26_i36_read_ocl_acl_mk                   */

/*exon*****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i39_check_req_attr                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        30.10.90                                              */
/*                                                                    */
/* DESCRIPTION: This decides, whether any of the requested attributes,*/
/*              which are specified by the user by their types, is    */
/*              present and may be accessed.                          */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      record      String          record containing base object     */
/*      filenr      Short           number of object file             */
/*      attr        D26_attr_info * array of indices (and a flag for  */
/*                                  read permission for each requested*/
/*                                  attribute in the AT (the last one */
/*                                  will be "-1")                     */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR               at least one attribute may be returned*/
/*      D2_ATTRIBUTE_MISSING    none of the listed attributes is      */
/*                              present                               */
/*      D2_ACCESS_RIGHTS_INSUFFICIENT   none of the listed attributes */
/*                              may be accessed                       */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i39_check_req_attr(char *record, signed16 filenr,
    D26_attr_info *attr)

{                               /*  d26_i39_check_req_attr            */

			    /* return value                           */
  signed16 ret_value = D2_ATTRIBUTE_MISSING;
  D26_at   *at;             /* pointer to AT-entry                    */

for                     /*  all listed attributes                     */
   (; attr->d26_at_idx != D26_EOL; attr++)
{                       /*  check whether attribute is present        */
  if                    /*  attribute is a naming one                 */
    (attr->d26_srt_idx != D26_IDX_NOT_DEF)
  {                     /*  check attributes presence in record       */
    if                  /*  attribute is present in record            */
      (ldint(record + d26_srt[attr->d26_srt_idx].
       d26s_nam_at[attr->d26_nam_idx].d26s_position[filenr]) > 0)
    {                   /*  check access right                        */
      if                /*  access is given                           */
	(attr->d26_read_ok == TRUE)
      {                 /*  return no error                           */
	return(D2_NO_ERR);
      }
      else
      {                 /*  reset return value                        */
	ret_value = D2_ACCESS_RIGHTS_INSUFFICIENT;
      }                 
    }                   
  }                     
  if                    /*  Attribute is not the pseudo attribute     */
    (attr->d26_at_idx != D26_IDX_NOT_DEF)
  {                     /*  set AT pointer                            */
    at = d26_at + attr->d26_at_idx;
    if                  /*  attribute present in file                 */
      (at->d26a_field_pos[filenr] > 0)
    {                   /*  check attributes presence in record       */
      if                /*  attribute is present in record            */
	(ldint(record + at->d26a_field_pos[filenr]) > 0)
      {                 /*  check access right                        */
	if              /*  access is given                           */
	  (attr->d26_read_ok == TRUE)
	{               /*  return no error                           */
	  return(D2_NO_ERR);
	}
	else
	{               /*  reset return value                        */
	  ret_value = D2_ACCESS_RIGHTS_INSUFFICIENT;
	}               
      }                 
    }                   
  }                     
}                       

return(ret_value);

}                       /*  d26_i39_check_req_attr                    */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_i42_cmp_phon()                                  */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 11.03.88                                            */
/*                                                                    */
/* DESCRIPTION  : This function will compare the filter and attribute */
/*                values from message and record using approximate    */
/*                matching  rule.                                     */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   signed16        object matches filter             */
/*      D2_NFOUND                   object doesn't match filter       */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i42_cmp_phon(
    signed16 len,       /* IN    -  length of filter value            */
    byte *value,        /* IN    -  filter or attr value              */
    char *record,       /* IN    -  record containing base object     */
    signed16 *pos,      /* IN    -  position of value in record       */
    D26_at *at)         /* IN    -  description of filter attribute   */

{                                   /*  d26_i42_cmp_phon              */

  char     *comp_val;           /* attribute value to compare         */
  byte     phon[D26_LPHON];     /* space to write the phonetic        */
				/* shortcut of a filter attribute     */
  signed16 add_len;             /* length of internal added characters*/
  signed16 field_factor;        /* occurrency of fields               */

  comp_val = record + *pos;
  d26_soundex(value,(signed16)len,phon);

  if                        /*  phonetical values are different       */
    (memcmp(comp_val,phon,D26_LPHON))
  {                         /*  reset position and return 'no match'  */
    add_len  = at->d26a_rep == D2_T61_PR_LIST ? 2 * D26_MAXVALPARTS :
      (at->d26a_rep == D2_TLXNR_STX ? D26_MAXVALPARTS : 1);
    field_factor = at->d26a_syntax == D2_CIGNORE ||
      at->d26a_syntax == D2_IA5 || at->d26a_syntax == D2_CIGN_LIST ||
      at->d26a_syntax == D2_POST_AD_STX ||
      at->d26a_syntax == D2_COUNTRY_STX ? 2 : 1;
    *pos += D26_LPHON + (at->d26a_rep == D2_T61_PR ? INTSIZE : 0) +
      field_factor * (at->d26a_ubound + add_len);
    return(D2_NFOUND);
  }
			    /*  return no error                       */
  return(D2_NO_ERR);

}                                   /*  d26_i42_cmp_phon              */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i48_get_exact_name                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        22.03.92                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the exact name from record into  */
/*              exact_name beginning with the RDN start.              */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      record      String          contains whole entry              */
/*      dninfo      D26_dninfo *    information about object found    */
/*      start       Short           indicates first namepart to be    */
/*                                  written                           */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      exact_name  String          memory to write the exact name to */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      >= 0	    Int  	    length of the exact name          */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i48_get_exact_name(char *record, D26_dninfo *dninfo,
    signed16 start, D2_name_string exact_name)

{                               /*  d26_i48_get_exact_name            */

  D26_srt  *srt;            /* pointer to SRT-entry                   */
  D26_at   *at;             /* pointer to AT-entry                    */
  signed16 phon_len;        /* length of phonetical value of attribut */
  signed16 pos;             /* position in record                     */
  char     separator;       /* separator character between type and   */
			    /* value                                  */

  register signed16    i;       /* loop variable                      */
  register D26_rdninfo *rdn;    /* loop variable (relative dist. name)*/
  register D26_nam_at  *nam_at; /* loop variable for naming attribute */
  register char        *np;     /* loop pointer to namepart           */

for                     /*  all RDNs                                  */
   (i = start, np = (char *)exact_name, rdn = dninfo->d26_rdn + start;
      i < dninfo->d26_rd_anz; i++, np += strlen(np), rdn++)
{                       /*  scan the AVAs                             */
  srt = d26_srt + rdn->d26_r_idx;
  for			/*  all naming attributes of SRT entry	      */
     (nam_at = srt->d26s_nam_at; nam_at->d26s_index != -1; nam_at++,
      np += strlen(np))
  { 			/*  write AVA into exact name		      */
    at = d26_at + nam_at->d26s_index;
    phon_len = at->d26a_phon == TRUE ? D26_LPHON : 0;
    d27_301_oid_str((D2_obj_id *)&at->d26a_type,np);
    np += strlen(np);
    pos = nam_at->d26s_position[dninfo->d26_filenr] + INTSIZE + phon_len;
    switch(at->d26a_rep)
    {
      case D2_T61_PR:
	switch(ldint(record + pos))
	{
	  case D2_PRINTABLE:
	    separator = D2_PRTBL_REP;
	    break;
	  default:
	    separator = D2_T61_REP;
	    break;
	}   
	pos += INTSIZE;
	break;
      case D2_PRINTABLE:
	separator = D2_PRTBL_REP;
	break;
      case D2_NUMERIC:
	separator = D2_NUM_REP;
	break;
      case D2_IA5:
	separator = D2_IA5_REP;
	break;
    }   
    if (at->d26a_syntax != D2_CEXACT && at->d26a_syntax != D2_PRINTABLE)
    {
       pos += (dninfo->d26_filenr == 0 ? D26_CNL : at->d26a_ubound) + 1;
    }
    sprintf(np,"%c%s%c",separator,record + pos,D2_AVA_SEP);
  }                     
  *(np - 1) = D2_RDN_SEP;
} 			

if                      /*  RDNs were written                         */
  (np > (char *)exact_name)
{                       /*  reduce namepart pointer                   */
  *(np - 1) = D2_EOS;
  return(np - (char *)exact_name);
}
else
{			/*  return 0				      */
  *np = D2_EOS;
  return(0);
}                       

}  			/*  d26_i48_get_exact_name		      */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i63_check_name                                    */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        22.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function compares the name described in dninfo   */
/*              with the name contained in a record. It returns the   */
/*              highest number of non matching nameparts              */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      dninfo      D26_dninfo *    information about object to be    */
/*                                  compared                          */
/*      np_count    Short           count of nameparts to be compared */
/*      record      String          record to compare                 */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     >= 0         Short           highest number of non matching    */
/*                                  nameparts                         */
/*      -1                          all nameparts match               */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i63_check_name(D26_dninfo *dninfo, signed16 np_count,
    char *record)

{                               /*  d26_i63_check_name                */

  D26_at   *at;             /* pointer to AT                          */
  char     *np;             /* pointer to name part in record         */
  signed16 pos;             /* position of namepart in record         */

  register signed16    i,j; /* loop variables                         */
  register D26_rdninfo *rdn;/* loop pointer to RDN                    */
  register D26_ava     *ava;/* loop pointer to AVA                    */

for                   /*  all nameparts of object to compare          */
   (i = np_count - 1, rdn = dninfo->d26_rdn + i; i >= 0; i--, rdn--)
{                     /*  scan the AVAs                               */
  for                 /*  all AVAs of RDN                             */
     (j = 0, ava = rdn->d26_r_ava; j < rdn->d26_r_nava; j++, ava++)
  {                   /*  set name-pointer to record                  */
    pos = d26_srt[rdn->d26_r_idx].d26s_nam_at[j].
				  d26s_position[dninfo->d26_filenr];
    at = d26_at + ava->d26_a_idx;
    np = record + pos + INTSIZE + (at->d26a_phon == TRUE ? D26_LPHON : 0)+
				  (at->d26a_rep == D2_T61_PR ? INTSIZE : 0);
    if                /*  namepart does not match                   */
      (strncmp(np,(char *)ava->d26_a_val,ava->d26_a_len) ||
       strlen(np) != ava->d26_a_len)
    {                 /*  break loop                                */
      break;
    }                 
  }                   
  if                  /*  RDN does not match                        */
    (j < rdn->d26_r_nava)
  {                   /*  break loop                                */
    break;
  }                   
}                     

return(i);

}                       /*  d26_i63_check_name                        */
