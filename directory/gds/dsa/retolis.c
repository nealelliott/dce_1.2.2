/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: retolis.c,v $
 * Revision 1.1.735.2  1996/02/18  18:15:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:38:12  marty]
 *
 * Revision 1.1.735.1  1995/12/08  15:51:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:49  root]
 * 
 * Revision 1.1.733.5  1994/06/21  14:46:20  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:54:46  marrek]
 * 
 * Revision 1.1.733.4  1994/05/10  15:53:17  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:28:40  marrek]
 * 
 * Revision 1.1.733.3  1994/03/23  15:13:57  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:26:11  keutel]
 * 
 * Revision 1.1.733.2  1994/02/22  17:37:39  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:25:42  marrek]
 * 
 * Revision 1.1.733.1  1994/01/13  09:17:52  marrek
 * 	Index with lower level is preferred before dummy index.
 * 	[1994/01/12  11:10:08  marrek]
 * 
 * Revision 1.1.731.2  1993/10/14  17:16:53  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:14:25  keutel]
 * 
 * Revision 1.1.731.1  1993/10/13  17:31:31  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:30:20  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  16:04:07  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:28:24  marrek]
 * 
 * Revision 1.1.4.6  1993/02/02  14:59:31  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.5  1993/02/02  11:35:11  marrek
 * 	Fixes for OT 6587
 * 	[1993/02/01  15:42:42  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:44:27  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:22:46  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  20:35:26  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:25:27  marrek]
 * 
 * Revision 1.1.4.2  1992/09/28  12:33:52  marrek
 * 	Assign new value to al_filenr.
 * 	[1992/09/28  12:31:50  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  02:56:37  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:34:21  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: retolis.c,v $ $Revision: 1.1.735.2 $ $Date: 1996/02/18 18:15:40 $";
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
/* NAME         : retolis.c       [retolis]                           */
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
/* DESCRIPTION  :   The module contains the ISAM utilities used by    */
/*                  the internal search function for subset "one      */
/*                  level" and "subtree"                              */
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

#include <search.h>

/*****  external Includes    *****/

#include <dce/d27util.h>

/*****  internal Includes    *****/

#include <d2shm.h>
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

typedef struct {                    /* Index information structure    */
		    D26_srt  *srt;
		    D26_nam_at *nam_at;
		    D26_at   *at;
		    signed16 length;
		    D2_a_match    rule;
		    byte     *i_value;
		    signed16 ob_id;
	       } D26_index_info;

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

/******** LOCAL  ********/

static byte *d26_i41_search_file(D2_name_string dname, D26_dninfo *dninfo,
    signed16 np_count, signed16 bo_nr, signed16 ob_nr, D23_rq_info *r_info,
    D23_ft1_info *f_info, D26_attr_info *ft_info, byte *message,
    signed32 *output_len, signed32 *remlen, signed16 fct_tag, byte **output);
static boolean d26_i43_use_record(char *record, D26_dninfo *dninfo,
    signed16 np_count, signed16 ob_nr, D26_index_info *i_info,
    signed16 i_pos, byte *i_value, signed16 i_len, signed16 i_val_nr);
static byte *d26_i60_write_children(byte *message, byte **output,
    D2_name_string dname, D26_dninfo *dninfo, signed16 np_count,
    signed16 ob_nr, D26_index_info *i_info, D23_rq_info *r_info,
    D23_ft1_info *f_info, signed32 *remlen, signed32 *output_len,
    signed16 fct_tag);
static byte *d26_i64_search_attr_file(byte *message, byte **output,
    D2_name_string dname, D26_dninfo *dninfo, signed16 np_count,
    signed16 bo_nr, D26_index_info *i_info, D23_rq_info *r_info,
    D23_ft1_info *f_info, signed32 *remlen, signed32 *output_len,
    signed16 fct_tag);
static void d26_i65_insert_keyval(char *record, D26_index_info *i_info,
    struct keypart *k_part, signed16 *len, byte *convert, byte **i_value);
static signed16 d26_i66_use_attr_record(void **rec_nr_root, signed32 rec_nr);
static int d26_i68_cmp_long(const void *rec_nr1,const void *rec_nr2);
static void d26_i69_free_rec_nr(void **rec_nr_root);
static signed16 d26_i88_cmp_index(D2_a_match m_rule1, D26_at *adt1,
    signed16 *oct_idx, D2_a_match m_rule2, boolean only_first_naming,
    D26_at *adt2, byte *value, signed16 length);
static void d26_i97_index_item(D26_dninfo * dninfo, signed16 np_count,
    signed16 bo_nr, signed16 ob_nr, D23_ft1_info **f_ind,
    D26_attr_info **ft_info, D26_index_info *i_info, boolean ignore);
static boolean d26_i98_only_first_naming(signed16 bo_nr, signed16 ob_nr,
    D26_attr_info *ft_info);
static boolean d26_i9c_namepart_repeated(D26_srt *srt, signed16 at_idx,
    signed16 sub_nr);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i41_search_file                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        21.01.91                                              */
/*                                                                    */
/* DESCRIPTION: This function searches one single C-ISAM-object-file  */
/*              for entries with a given object number, fulfilling    */
/*              the given filters. It writes those objects with their */
/*              requested attributes into message.                    */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      dname       D26_name_string DN of base object                 */
/*      dninfo      D26_dninfo *    information about found object    */
/*      np_count    Short           count of nameparts in base object */
/*      bo_nr       Short           number of base object             */
/*      ob_nr       Short           object number for search          */
/*      r_info      D23_rq_info *   information about required attr.  */
/*      f_info      D23_ft1_info *  information about filter attr.    */
/*      ft_info     D26_attr_info * Array containing the attribute    */
/*                                  types of filters                  */
/*      message     String          actual message pointer            */
/*      output_len  Int *           maximum length of result message  */
/*      remlen      Int *           remaining length for message      */
/*      fct_tag     Short           indicates the calling function    */
/*      output      String *        pointer to beginning of message   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*      output      String *        pointer to beginning of message   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      != D2_NULL         String       pointer to byte after         */
/*                                      written message               */
/*         D2_NULL                      error occurred                */
/*                                                                    */
/*exoff ***************************************************************/

static byte *d26_i41_search_file(D2_name_string dname, D26_dninfo *dninfo,
    signed16 np_count, signed16 bo_nr, signed16 ob_nr, D23_rq_info *r_info,
    D23_ft1_info *f_info, D26_attr_info *ft_info, byte *message,
    signed32 *output_len, signed32 *remlen, signed16 fct_tag, byte **output)

{                               /*  d26_i41_search_file               */

			/* structure containing index information     */
  D26_index_info i_info;

			/* pointer to filter information              */
  D23_ft1_info   *f_info1;
			/* pointer for filter attribute information   */
  D26_attr_info  *ft_in_p;

  D26_rdninfo    *i_rdn;/* pointer to RDN to use as index             */
			/* pointer to AVA of naming attribute with    */
			/* highest index level                        */
  D26_ava        *i_ava;
  signed16       ind_level; /* highest index level to use             */

  register D26_srt     *anc;    /* pointer to SRT entry for ancestor  */
  register D26_nam_at  *nam_at; /* pointer to naming attribute of SRT */
				/* entry                              */
  register signed16    i;       /* loop variable                      */
  register D26_rdninfo *rdn;    /* loop pointer to RDN                */
  register D26_ava     *ava;    /* loop pointer to AVA                */

				    /*  initialize pointers           */
f_info1        = f_info;
ft_in_p        = ft_info;

			/*  initialize i_info. If no better index is  */
			/*  found, the index carried by the first     */
			/*  naming attribute of the SRT entry defined */
			/*  by the object number is used.             */
i_info.srt     = d26_srt + ob_nr;
i_info.at      = d26_at + d26_srt[ob_nr].d26s_nam_at[0].d26s_index;
i_info.rule    = D26_IDX_NOT_DEF;
i_info.length  = 0;
i_info.i_value = NULL;
i_info.ob_id   = D26_IDX_NOT_DEF;

			/*  try to find a better index from the filter*/
			/*  information                               */
d26_i97_index_item(dninfo,np_count,bo_nr,ob_nr,(D23_ft1_info **)&f_info1,
		   &ft_in_p,&i_info,FALSE);

			/*  try to find a better index from the       */
			/*  nameparts of the base object              */

for                     /*  all nameparts of base object              */
   (rdn = i_rdn = dninfo->d26_rdn + np_count - 1, i_ava = NULL,
    ind_level = i_info.at->d26a_ind_level; rdn >= dninfo->d26_rdn; rdn--)
{                       /*  scan the AVAs of the RDN                  */
  for                   /*  all AVAs of RDN                           */
     (i = 0, ava = rdn->d26_r_ava; i < rdn->d26_r_nava; i++, ava++)
  {                     /*  check index-level of naming attribute     */
    if                  /*  new index level higher than old one       */
      (d26_at[ava->d26_a_idx].d26a_ind_level >
       (ind_level > D2_MAX_ADM_IND_LEVEL ? ind_level : D2_MAX_ADM_IND_LEVEL))
    {                   /*  assign AT-pointer to use as index         */
      i_rdn     = rdn;
      i_ava     = ava;
      ind_level = d26_at[ava->d26_a_idx].d26a_ind_level;
    }                   
  }                     
}                       

if                                  /*  naming attribute of base      */
				    /*  object was found for index    */
  (i_ava != NULL)
{                                   /*  provide index information     */
  i_info.srt     = d26_srt + i_rdn->d26_r_idx;
  i_info.nam_at  = i_info.srt->d26s_nam_at + (i_ava - i_rdn->d26_r_ava);
  i_info.at      = d26_at + i_ava->d26_a_idx;
  i_info.rule    = D2_EQUAL;
  i_info.length  = i_ava->d26_a_len;
  i_info.i_value = i_ava->d26_a_val;
  DCE_SVC_LOG((GDS_S_IND_BO_TYPE_MSG,i_rdn - dninfo->d26_rdn + 1,
    i_info.at->d26a_type.d2_typ_len,i_info.at->d26a_type.d2_type));
  if                                /*  error from writing children   */
    ((message = d26_i60_write_children(message,output,dname,dninfo,
     np_count,ob_nr,&i_info,r_info,f_info,remlen,output_len,fct_tag))
     == NULL)
  {                                 /*  set return value              */
    return(NULL);
  }                                 
}
else if                             /*  filter gives no index and     */
				    /*  base object has nameparts     */
       (i_info.i_value == NULL && bo_nr != D26_ROOT)
{                                   /*  take lowest namepart of base  */
				    /*  object for index              */
  i_info.srt     = d26_srt + bo_nr;
  i_info.nam_at  = i_info.srt->d26s_nam_at;
  i_info.at      = d26_at + i_rdn->d26_r_ava->d26_a_idx;
  i_info.rule    = D2_EQUAL;
  i_info.length  = i_rdn->d26_r_ava->d26_a_len;
  i_info.i_value = i_rdn->d26_r_ava->d26_a_val;
  DCE_SVC_LOG((GDS_S_IND_BO_TYPE_MSG,i_rdn - dninfo->d26_rdn + 1,
    i_info.at->d26a_type.d2_typ_len,i_info.at->d26a_type.d2_type));
  if                                /*  error from writing children   */
    ((message = d26_i60_write_children(message,output,dname,dninfo,
     np_count,ob_nr,&i_info,r_info,f_info,remlen,output_len,fct_tag))
     == NULL)
  {                                 /*  set return value              */
    return(NULL);
  }                                 
}
else
{                                   /*  use index from filter info.   */
  if                                /*  attribute is in object classes*/
				    /*  but not naming for object cls.*/
    (i_info.srt == NULL)
  {                                 /*  try first as non naming       */
    if                              /*  attribute is stored at object */
      (i_info.at->d26a_filenr < 0)
    {                               /*  search in object file         */
      DCE_SVC_LOG((GDS_S_IND_AT_TYPE_MSG,D26_OBJECTS,
	i_info.at->d26a_type.d2_typ_len,i_info.at->d26a_type.d2_type));
      if                            /*  error from writing children   */
	((message = d26_i60_write_children(message,output,dname,dninfo,
	 np_count,ob_nr,&i_info,r_info,f_info,remlen,output_len,fct_tag))
	 == NULL)
      {                             /*  return error                  */
	return(NULL);
      }                             
    }
    else if                         /*  attribute file not searched   */
	   (d26_ofile_searched[dninfo->d26_filenr] == FALSE)
    {                               /*  search in attribute file      */
      DCE_SVC_LOG((GDS_S_IND_AT_TYPE_MSG,D26_ATTR,
	i_info.at->d26a_type.d2_typ_len,i_info.at->d26a_type.d2_type));
      if                            /*  error from writing children   */
	((message = d26_i64_search_attr_file(message,output,dname,dninfo,
	 np_count,bo_nr,&i_info,r_info,f_info,remlen,output_len,fct_tag))
	 == NULL)
      {                             /*  return error                  */
	return(NULL);
      }                             
      d26_ofile_searched[dninfo->d26_filenr] = TRUE;
    }                               
  }                                 

				    /*  try attribute as naming       */
				    /*  for superior SRT entries      */
  for                               /*  all srt entries               */
     (anc = d26_srt + ob_nr; anc != d26_srt + bo_nr &&
      (d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
       d26_entry_count <= d26_serv_contr->d23_scslm) &&
       d26_time_ex == FALSE; anc = d26_srt + anc->d26s_par_nr)
  {                                 /*  check attribute types         */
    for                             /*  all naming attributes         */
       (nam_at = anc->d26s_nam_at;
	nam_at->d26s_index != -1 &&
	(d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
	d26_entry_count <= d26_serv_contr->d23_scslm) &&
	d26_abandoned == FALSE && d26_dbop_flag != D26_DB_NOOP &&
	d26_time_ex == FALSE;
	nam_at++)
    {                               /*  compare the attribute types   */
      if                            /*  types are equal               */
	(nam_at->d26s_index == i_info.at - d26_at)
      {                             /*  lookup naming attributes      */
	if                          /*  positive index value given    */
				    /*  or naming attribute is not    */
				    /*  repeated                      */
	  (d26_i9c_namepart_repeated(anc,nam_at->d26s_index,ob_nr)
	   == FALSE || i_info.length > 0)
	{                           /*  reset structure rule in index */
				    /*  information                   */
	  i_info.srt    = anc;
	  i_info.nam_at = nam_at;
	  DCE_SVC_LOG((GDS_S_IND_NAM_AT_TYPE_MSG,
	    i_info.at->d26a_type.d2_typ_len,i_info.at->d26a_type.d2_type));
	  if                        /*  error from writing children   */
	    ((message = d26_i60_write_children(message,output,dname,
	     dninfo,np_count,ob_nr,&i_info,r_info,f_info,remlen,output_len,
	     fct_tag)) == NULL)
	  {                         /*  return error                  */
	    return(NULL);
	  }                         
	  break;
	}                           
      }                             
    }                               
  }                                 
}                                   

return(message);

}                       /*  d26_i41_search_file                       */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i43_use_record                                    */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        22.01.91                                              */
/*                                                                    */
/* DESCRIPTION: This function checks, whether all nameparts of the    */
/*              base object match. If so, it checks whether the value */
/*              from index                                            */
/*              information occurs duplicated in the record. If so,   */
/*              the record may not be used for message, FALSE is      */
/*              returned in this case. TRUE is returned otherwise.    */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      record      String          C-Isam-record containing object   */
/*      dninfo      D26_dninfo *    information about base object     */
/*      np_count    Short           count of nameparts in base object */
/*      ob_nr       Short           object number                     */
/*      i_info      D26_index_info * information to be used as index  */
/*      i_pos       Short           position of value used for index  */
/*      i_value     Octet_string    value used for index              */
/*      i_len       Short           length of value used for index    */
/*      i_val_nr    Short           number of attribute value which   */
/*                                  was used for index                */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_TRUE     Short           write object and attributes from  */
/*                                  record into message               */
/*      D2_FALSE                    not first match. Object is in     */
/*                                  message already                   */
/*                                                                    */
/*inoff ***************************************************************/

static boolean d26_i43_use_record(char *record, D26_dninfo *dninfo,
    signed16 np_count, signed16 ob_nr, D26_index_info *i_info,
    signed16 i_pos, byte *i_value, signed16 i_len, signed16 i_val_nr)

{                               /*  d26_i43_use_record                */

  signed16 filenr;          /* number of ISAM file                    */
  signed16 pos;             /* position of attribute in record        */
  signed16 maxval;          /* maximum number of attribute values to  */
			    /* search on                              */
  signed16 syn_factor;      /* count of strings stored for            */
			    /* particular syntax                      */
  signed16 cmp_off;         /* offset of compare value                */
  signed16 rep_len;         /* length of representation field         */
  signed16 phon_len;        /* length of phonetic value               */
  signed16 add_len;         /* length of separator bytes              */

  signed16 bo_nr;           /* object number of base object           */
  signed16 ind_ob_nr;       /* object number of SRT-entry used for    */
			    /* index                                  */

  register signed16   i;    /* loop variables                         */
  register D26_srt    *srt; /* pointer to SRT entry                   */
  register D26_nam_at *nam_at; /* pointer to naming attributes        */

  filenr     = dninfo->d26_filenr;
  syn_factor = i_info->at->d26a_syntax == D2_CIGNORE ||
	       i_info->at->d26a_syntax == D2_IA5 ||
	       i_info->at->d26a_syntax == D2_NUMERIC ||
	       i_info->at->d26a_syntax == D2_TELNR_STX ||
	       i_info->at->d26a_syntax == D2_COUNTRY_STX ||
	       i_info->at->d26a_syntax == D2_CIGN_LIST ||
	       i_info->at->d26a_syntax == D2_POST_AD_STX ||
	       i_info->at->d26a_syntax == D2_MHS_DLSP_STX ||
	       i_info->at->d26a_syntax == D2_MHS_ORADR_STX ||
	       i_info->at->d26a_syntax == D2_MHS_ORNAME_STX ? 2 : 1;
  rep_len    = i_info->at->d26a_rep == D2_T61_PR ? INTSIZE : 0;
  cmp_off    = i_info->rule == D2_APPROX_MATCH &&
	       i_info->at->d26a_phon == TRUE ? 0 : D26_LPHON + rep_len;
  phon_len   = i_info->at->d26a_phon == TRUE ? D26_LPHON : 0;

  if                    /*  names in structure and record differ      */
    (d26_i63_check_name(dninfo,np_count,record) >= 0)
  {                     /*  return don't use                          */
    return(FALSE);
  }
  else if               /*  naming attribute is used for index        */
	 (i_info->srt != NULL)
  {                     /*  set object numbers                        */
    ind_ob_nr = i_info->srt - d26_srt;
    bo_nr     = np_count > 0 ?
		dninfo->d26_rdn[np_count - 1].d26_r_idx : D26_ROOT;
    if                  /*  no namepart of base object was index      */
      (bo_nr == D26_ROOT ||
       ind_ob_nr != bo_nr && d26_u46_descendant(ind_ob_nr,bo_nr) == FALSE)
    {                   /*  check attributes belongance               */
      if                /*  index attribute does not belong to object */
	(ob_nr != ind_ob_nr)
      {                 /*  compare with other name parts of found    */
			/*  object                                    */
	for             /*  all intermediate nameparts                */
	   (srt = d26_srt + d26_srt[ob_nr].d26s_par_nr;
	    srt != i_info->srt; srt = d26_srt + srt ->d26s_par_nr)
	{               /*  scan the naming attributes of SRT-entry   */
	  for           /*  all naming attributes of SRT entry        */
	     (nam_at = srt->d26s_nam_at; nam_at->d26s_index != -1; nam_at++)
	  {             /*  compare attribute types and first values  */
	    if          /*  types and values are equal                */
	      (nam_at->d26s_index == i_info->at - d26_at &&
	       !memcmp(record + nam_at->d26s_position[filenr] + INTSIZE +
	       cmp_off,i_value,i_len))
	    {           /*  return don't use                          */
	      return(FALSE);
	    }
	  }
	}
	srt  = d26_srt + ob_nr;
	for             /*  all naming attributes of SRT entry        */
	   (nam_at = srt->d26s_nam_at; nam_at->d26s_index != -1; nam_at++)
	{               /*  compare attribute types                   */
	  if            /*  types are equal                           */
	    (nam_at->d26s_index == i_info->at - d26_at)
	  {             /*  get position from SRT-entry               */
	    pos = nam_at->d26s_position[filenr];
	    break;
	  }
	}
	if              /*  position not yet set                      */
	  (nam_at->d26s_index == -1)
	{               /*  get position from AT-entry                */
	  pos = i_info->at->d26a_field_pos[filenr];
	}
	maxval = ldint(record + pos);
	pos   += INTSIZE;
	for             /*  all valid values                          */
	   (i = 0; i < maxval; i++)
	{               /*  compare positions of values               */
	  if            /*  compare value is not indexed value        */
	    (pos + cmp_off != i_pos)
	  {             /*  compare values                            */
	    if          /*  values match                              */
	      (!memcmp(record + pos + cmp_off,i_value,i_len))
	    {           /*  return don't use                          */
	      return(FALSE);
	    }
	  }
	  pos += phon_len + rep_len +
	    syn_factor * (i_info->at->d26a_ubound + 1);
	}
      }
      else
      {                 /*  compare with previous values              */
	srt    = d26_srt + ob_nr;
	pos    = i_info->nam_at->d26s_position[filenr] + INTSIZE;
	for             /*  all valid values                          */
	   (i = 0; i < i_val_nr; i++)
	{               /*  compare with indexed value                */
	  if            /*  values match                              */
	    (!memcmp(record + pos + cmp_off,i_value,i_len))
	  {             /*  return don't use                          */
	    return(FALSE);
	  }
	  else
	  {             /*  reset position                            */
	    pos += phon_len + rep_len +
	      syn_factor * (i_info->at->d26a_ubound + 1);
	  }
	}
      }
    }
  }
  else
  {                     /*  compare with previous values              */
    pos = i_info->at->d26a_field_pos[filenr] + INTSIZE;
    for                 /*  all valid values                          */
       (i = 0; i < i_val_nr; i++)
    {                   /*  compare with indexed value                */
      add_len = (i_info->at->d26a_syntax == D2_CIGN_LIST ||
		 i_info->at->d26a_syntax == D2_POST_AD_STX) ?
		 D26_MAXVALPARTS : 1;
      switch(i_info->at->d26a_syntax)
      {
	case D2_CEXACT:         /* <value>'\0'                        */
	case D2_PRINTABLE:
	case D2_TIME:
	case D2_CIGNORE:        /* <norm value>'\0'<exact value>'\0'  */
	case D2_IA5:
	case D2_NUMERIC:
	case D2_TELNR_STX:
	case D2_COUNTRY_STX:
	case D2_CIGN_LIST:      /* <norm elem 1>'\0'..<norm el 10>'\0'*/
	case D2_POST_AD_STX:    /* <exact el 1>'\0'..<exact el 10>'\0'*/
	  if                    /* values match                       */
	    (!memcmp(record + pos + cmp_off,i_value,i_len))
	  {                     /* return don't use                   */
	    return(FALSE);
	  }
	  else
	  {                     /* reset position                     */
	    pos += phon_len + rep_len +
	      syn_factor * (i_info->at->d26a_ubound + add_len);
	    break;
	  }
	case D2_OBJ_IDENT:
	case D2_OCTET:
	case D2_PASSWD_STX:
	case D2_TTXID_STX:
	case D2_FAXNR_STX:
	case D2_MHS_DLSP_STX:
	case D2_MHS_ORADR_STX:
	case D2_MHS_ORNAME_STX:
	  if                    /* values match                       */
	    (!memcmp(record + pos,i_value,i_len))
	  {                     /* return don't use                   */
	    return(FALSE);
	  }
	  else
	  {                     /* reset position                     */
	    pos += syn_factor * (i_info->at->d26a_ubound + INTSIZE);
	  }
	  break;
	case D2_BOOLEAN:        /* <value>                            */
	case D2_INTEGER:
	  if                    /* values match                       */
	    (!memcmp(record + pos,i_value,i_len))
	  {                     /* return don't use                   */
	    return(FALSE);
	  }
	  else
	  {                     /* reset position                     */
	    pos += LONGSIZE;
	  }
	  break;
      }
    }
  }

				/* return use record                  */
  return(TRUE);

}                       /*  d26_i43_use_record                        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i44_write_set                                     */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        22.02.91                                              */
/*                                                                    */
/* DESCRIPTION: This function checks the base object for filters,     */
/*              writes it into message, calculates the object numbers */
/*              and numbers of C-Isam-files and searches them for all */
/*              the children and eventually for referrals. The found  */
/*              children are written into message, the referrals are  */
/*              kept in memory.                                       */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      message     String          actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      subset      D2_subset       indicates subset to be searched   */
/*      dname       D26_name_string normed DN of base object          */
/*      ename       D26_name_string exact DN of base object           */
/*      dninfo      D26_dninfo *    information about found object    */
/*      al_rdns     Short           number of aliased RDNs of base o. */
/*      bo_nr       Short           base object number                */
/*      bob_cl      Long *          object class array of base object */
/*      acl         D26_acl *       acl of base object                */
/*      mk          Long            index of master knowledge in      */
/*                                  DN-list.                          */
/*      record      String          record containing base object     */
/*      r_info      D23_rq_info *   information about required attr.  */
/*      f_info      D23_ft1_info *  information about filter attr.    */
/*      output_len  Int *           maximum length of result message  */
/*      fct_tag     Short           indicates the calling function    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NULL     String          error occurred                    */
/*      != D2_NULL                  pointer after last byte of written*/
/*                                  message                           */
/*                                                                    */
/*exoff ***************************************************************/

byte *d26_i44_write_set(byte *message, byte **output, D2_subset subset,
    D2_name_string dname, D2_name_string ename, D26_dninfo *dninfo,
    signed16 al_rdns, signed16 bo_nr, signed32 *bob_cl, D26_acl *acl,
    signed32 mk, char *record, D23_rq_info *r_info, D23_ft1_info *f_info,
    signed32 *output_len, signed32 *remlen, signed16 fct_tag)

{                               /*  d26_i44_write_set                 */

				    /* alias object classes           */
  static signed32 al_ob_cl[] = {0L,(signed32)D26_EOL};

  signed16      np_count;   /* count of nameparts in base object      */
  D26_rdninfo   *l_rdn;     /* last RDN in distinguished name         */
  D26_ava       *l_ava;     /* last AVA in last RDN                   */
  signed16      filenr;     /* actual file number                     */
  signed16      al_filenr;  /* alias  file number                     */
				    /* object classes of SRT entry    */
  signed32      ob_cl[D26_OCL_VAL_COUNT + 1];

			/* array for filter attribute information     */
  D26_attr_info ft_info[D2_AT_MAX + 1];
				    /* pointer to Filter structure    */
  D23_ft1_info  *f1_info, *f2_info;
  D23_fi1_info  *f_err_item;        /* pointer to erroneous filter    */
				    /* item                           */

			    /* dummy space for write entry to write ..*/
  boolean       recur;      /* ... a recurring flag                   */

  register signed16    i,j;     /* loop variables                     */
  register D26_srt     *srt;    /* pointer to SRT                     */
  register D26_ocl_arc *ocl_arc;    /* loop variable for object class */
				    /* arcs                           */
  register signed16    *aux_cl; /* pointer to auxiliary object classes*/

if                      /*  Subtree or single object is searched and  */
			/*  no limit is exceeded                      */
  (subset != D2_ONE_LEVEL && d26_time_ex == FALSE &&
   (d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
    d26_entry_count <= d26_serv_contr->d23_scslm))
{                       /*  check whether base object is root         */
  if                    /*  base object is not root                   */
    (bo_nr != D26_ROOT)
  {                     /*  calculate filter information              */
    f1_info = f_info;
    if                              /*  filter is ok for the object   */
      (f_info->d23_fp1tag != D23_PATTAG ||
       d26_f09_analyse_filter_object(bo_nr,bob_cl,&f1_info,&f_err_item)
       == D2_NO_ERR)
    {                   /*  calculate filter information              */
      d26_f20_calc_fat_info(bo_nr,bob_cl,f_info,ft_info);
      if                /*  DSA is master or copies are allowed       */
	(mk == D26_OWNNAME || !(d26_serv_contr->d23_scopt & D2_NOCACHE))
      {                 /*  check filter attributes                   */
	if              /*  no error from filter attribute check      */
	  (d26_f19_filter_attr_check(record,dninfo->d26_filenr,bo_nr,dname,
	   ft_info,acl,f_info,&f_err_item,fct_tag,FALSE) == D2_NO_ERR)
	{               /*  write object into result message          */
	  DCE_SVC_LOG((GDS_S_FMATCH_ST_ROOT_MSG,dname));
	  d26_u71_make_attr_list(r_info,d26_attr,dname,acl,bo_nr,bob_cl,
				 fct_tag);
	  d26_entry_count++;
	  if            /*  size limit not yet exceeded               */
	    (d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
	     d26_entry_count <= d26_serv_contr->d23_scslm)
	  {             /*  write the entry into message              */
	    if          /*  error from write entry                    */
	      ((message = d26_u37_write_entry(record,message,output,dname,
	       dninfo,r_info,&recur,d26_attr,mk,bo_nr,remlen,output_len))
	       == NULL)
	    {           /*  return error                              */
	      return(NULL);
	    }           
	  }             
	}               
      }                 
    }                   
  }                     
}                       

np_count = dninfo->d26_rd_anz;

if                      /*  Subtree or One level must be searched     */
  (subset != D2_BASE_OBJECT)
{                       /*  create alias object class array           */
  al_ob_cl[0] = d26_ali_idx;

  for                   /*  all "object file searched" indicators     */
     (i = 0; i < d26_nofile; i++)
  {                     /*  initialize the indicator                  */
    d26_ofile_searched[i] = FALSE;
  }                     

  for                   /*  all SRT entries                           */
     (i = 0, srt = d26_srt; i < d26_nosrt &&
      (d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
       d26_entry_count <= d26_serv_contr->d23_scslm) &&
       d26_abandoned == FALSE && d26_dbop_flag != D26_DB_NOOP &&
       d26_time_ex == FALSE; i++, srt++)
  {                     /*  check whether child found                 */
    if                  /*  child or descendant is found              */
      (subset == D2_ONE_LEVEL && srt->d26s_par_nr == bo_nr ||
       subset == D2_WHOLE_SUBTREE && d26_u46_descendant(bo_nr,i) == TRUE)
    {                   /*  write all matching children               */
      for               /*  all object files                          */
	 (al_filenr = -1, filenr = 0; filenr < d26_nofile &&
	  (d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
	   d26_entry_count <= d26_serv_contr->d23_scslm) &&
	   d26_abandoned == FALSE && d26_dbop_flag != D26_DB_NOOP &&
	   d26_time_ex == FALSE; filenr++)
      {                 /*  check whether object file contains objects*/
			/*  whith this structure rule                 */
	for             /*  all assigned object class arcs            */
	   (ocl_arc = srt->d26s_ocl_arc;
	    ocl_arc->d26s_oclass != -1 &&
	    (d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
	    d26_entry_count <= d26_serv_contr->d23_scslm) &&
	    d26_abandoned == FALSE && d26_dbop_flag != D26_DB_NOOP &&
	    d26_time_ex == FALSE;
	    ocl_arc++)
	{               /*  check whether file contains objects of    */
			/*  this structure                            */
	  if            /*  file contains objects of this structure   */
	    (filenr == d26_oct[ocl_arc->d26s_oclass].d26c_filenr)
	  {             /*  assign object class array                 */
	    ob_cl[0] = (signed32)ocl_arc->d26s_oclass;
	    for         /*  all auxiliary object classes              */
	       (j = 1, aux_cl = d26_oct[ob_cl[0]].d26c_aux_cl; *aux_cl != -1;
		j++, aux_cl++)
	    {           /*  put auxiliary object class into object    */
			/*  class array                               */
	      ob_cl[j] = (signed32)*aux_cl;
	    }           
	    ob_cl[j] = D26_EOL;
	    f2_info = f1_info = f_info;

	    d26_u67_trace_name_struct(srt);
	    dninfo->d26_filenr = filenr;
	    if          /*  Filter is ok for object class array       */
	      (f_info->d23_fp1tag != D23_PATTAG ||
	       d26_f09_analyse_filter_object(i,ob_cl,&f1_info,&f_err_item)
	       == D2_NO_ERR)
	    {           /*  calculate filter information              */
	      DCE_SVC_LOG((GDS_S_OBJ_SEARCH_MSG,filenr));
	      d26_f20_calc_fat_info(i,ob_cl,f_info,ft_info);
	      if        /*  error from searching file                 */
		((message = d26_i41_search_file(dname,dninfo,np_count,bo_nr,
		 i,r_info,f_info,ft_info,message,output_len,remlen,fct_tag,
		 output)) == NULL)
	      {         /*  return error                              */
		return(NULL);
	      }         
	      break;
	    }
	    else if     /*  alias has to be searched in this file     */
	       (al_filenr < 0 &&
		d26_search_arg->d23_Iali == FALSE &&
		d26_f09_analyse_filter_object(i,al_ob_cl,&f2_info,
		&f_err_item) == D2_NO_ERR)
	    {           /*  calculate filter information              */
	      DCE_SVC_LOG((GDS_S_ALIAS_SEARCH_MSG,filenr));
	      d26_f20_calc_fat_info(i,al_ob_cl,f_info,ft_info);
	      if        /*  error from searching file                 */
		((message = d26_i41_search_file(dname,dninfo,np_count,bo_nr,
		 i,r_info,f_info,ft_info,message,output_len,remlen,fct_tag,
		 output)) == NULL)
	      {         /*  return error                              */
		return(NULL);
	      }         
	      break;
	    }
	    else
	    {           /*  log no search                             */
	      DCE_SVC_LOG((GDS_S_FILTER_EXCLUDE_MSG));
	    }           
	    al_filenr = al_filenr < 0 ? filenr : al_filenr;
	  }             
	}               
      }                 
    }                   
  }                     
}                       

if                      /*  referrals are required                    */
  (d26_abandoned == FALSE && d26_dbop_flag != D26_DB_NOOP &&
   d26_time_ex == FALSE &&
   !(d26_serv_contr->d23_scopt & D2_LOCALSCOPE) &&
   (subset != D2_ONE_LEVEL || (d26_serv_contr->d23_scopt & D2_NOCACHE)))
{                       /*  create partial result referrals           */
  dninfo->d26_rd_anz = np_count;
  if                    /*  nameparts are given                       */
    (np_count > 0)
  {                     /*  terminate last namepart by EOS            */
    l_rdn = dninfo->d26_rdn + np_count - 1;
    l_ava = l_rdn->d26_r_ava + l_rdn->d26_r_nava - 1;
    l_ava->d26_a_val[l_ava->d26_a_len] = D2_EOS;
  }
  else
  {                     /*  insert EOS at beginning of name           */
    *dname = D2_EOS;
  }                     
  d26_u72_make_p_res_referrals(dname,ename,dninfo,al_rdns,mk,bo_nr,
    fct_tag == D23_REMOBJT || fct_tag == D23_REPLRDN ?
    D2_BASE_OBJECT : subset);
}                       

return(message);

}                       /*  d26_i44_write_set                         */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i60_write_children                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        16.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function searches the children of the base object*/
/*              of a given object number                              */
/*              and writes the entry informations into message.       */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      message     String          actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      dname       D2_name_string  DN of base object                 */
/*      dninfo      D26_dninfo *    information about base object     */
/*      np_count    Short           count of nameparts in base object */
/*      ob_nr       Short           object number                     */
/*      i_info      D26_index_info * information to be used as index  */
/*      r_info      D23_rq_info *   information about required attr.  */
/*      f_info      String          information about filter attr.    */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*      fct_tag     Short           indicates the calling function    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      output      String *        pointer to beginning of message   */
/*      remlen      Int *           remaining length for message      */
/*      output_len  Int *           maximum length of result message  */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*              String   != D2_NULL         pointer to byte after     */
/*                                          written message           */
/*                          D2_NULL         error occurred            */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d26_i60_write_children(byte *message, byte **output,
    D2_name_string dname, D26_dninfo *dninfo, signed16 np_count,
    signed16 ob_nr, D26_index_info *i_info, D23_rq_info *r_info,
    D23_ft1_info *f_info, signed32 *remlen, signed32 *output_len,
    signed16 fct_tag)

{                               /*  d26_i60_write_children            */

			    /* function identifier for traces         */
  static  String  function = "i60";

  signed16       filenr;    /* number of ISAM file                    */
  signed16       pos;       /* position of attribute in record        */
  signed16       minval;    /* minimum number of attribute values ... */
  signed16       maxval;    /* maximum number of attribute values ... */
			    /* ... search on                          */
			    /* indicates whether master knowledge is  */
			    /* part of the index                      */
  boolean        mk_in_index = FALSE;
  D26_keydesc    key;       /* ISAM key structure                     */
  int            isfd;      /* ISAM file descriptor                   */
  int            s_mode;    /* ISAM start mode                        */
  int            r_mode;    /* ISAM read mode                         */
  boolean        go_on;     /* indicates whether loop must be cont.   */
  byte           *i_value;  /* pointer to index value                 */

  D26_rdninfo    *l_rdn;    /* pointer to last RDN                    */
  D26_ava        *l_ava;    /* pointer to last AVA                    */
  signed16       len;       /* length of value  to use for index      */

  D2_name_string tail;      /* pointer to end of DN of base object    */

  signed16       ao_val;    /* validity for aliased object attribute  */
  signed32       mk_idx;    /* master-knowledge index                 */
  signed32       rec_nr;    /* number of read record                  */
			    /* save key of record for reset           */
  char           save[MAXKEYSIZE];

			    /* converted data for index value         */
  byte           convert[MAXKEYSIZE];

			    /* counts calls of isstart                */
  signed32       is_count = 0;
			    /* counts calls of isread                 */
  signed32       ir_count = 0;
			    /* number of DB accesses in blocks        */
  signed32       nr_dbacc_in_blocks;

  register signed16 i;          /* loop variables                     */
  register D26_srt  *srt;       /* pointer to srt entry               */

filenr  = dninfo->d26_filenr;

if                              /*  error from open file              */
   ((isfd = d26_i23_open_file(filenr,D26_OBJ)) < 0)
{                               /*  return unchanged message          */
  return(message);
}
else
{                       /*  set "in use flag" for file                */
  d26_ofildes[filenr].d26_in_use++;
  maxval = i_info->at->d26a_nr_recur;
  if                    /*  attribute belongs to object classes       */
			/*  but is not first naming for them          */
    (i_info->srt == NULL)
  {                     /*  take position from AT-entry               */
    pos    = i_info->at->d26a_field_pos[filenr];
    for                 /*  all SRT entries                           */
       (i = 0, srt = d26_srt; i < d26_nosrt; i++, srt++)
    {                   /*  check attribute positions                 */
      if                /*  attribute is used as a first naming also  */
	(srt->d26s_nam_at->d26s_position[filenr] == pos)
      {                 /*  set Master knowledge as a part of index   */
	mk_in_index = TRUE;
	break;
      }                 
    }                   
  }
  else
  {                     /*  take position from SRT-entry              */
    pos    = i_info->nam_at->d26s_position[filenr];
    if                  /*  attribute is first naming                 */
      (i_info->srt->d26s_nam_at == i_info->nam_at)
    {                   /*  set master knowledge as part of index     */
      mk_in_index = TRUE;
    }                   
    if                  /*  attribute is naming for superior node     */
      (i_info->srt != d26_srt + ob_nr)
    {                   /*  set maximum number of values to 1         */
      maxval = 1;
    }                   
  }                     

  minval = 0;
  maxval = i_info->length > 0 ? maxval : 1;

  if                    /*  object class attribute is used for index  */
    (i_info->ob_id != D26_IDX_NOT_DEF)
  {                     /*  reset loop boundaries                     */
    if                  /*  structural or alias object class is index */
      (d26_oct[i_info->ob_id].d26c_kind == D2_STRUCTURAL ||
       d26_oct[i_info->ob_id].d26c_kind == D2_ALIAS)
    {                   /*  reset maximum attribute value             */
      maxval = 1;
    }
    else if             /*  auxiliary object class is index           */
	 (d26_oct[i_info->ob_id].d26c_kind == D2_AUXILIARY)
    {                   /*  reset minimum attribute value             */
      minval = 1;
    }                   
  }                     

  for                   /*  all possible attribute values             */
     (i = minval;
      i < maxval && (d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
      d26_entry_count <= d26_serv_contr->d23_scslm) &&
      d26_abandoned == FALSE && d26_dbop_flag != D26_DB_NOOP &&
      d26_time_ex == FALSE;
      i++)
  {                     /*  select C-isam-key                         */
    d26_i32_select_key(&key,pos,i_info->at,i,(D2_a_match)i_info->rule,
		       mk_in_index == FALSE || i > 0 ? -1 : filenr);
    stint(ob_nr,d26_ob_record + key.k_part[0].kp_start);
    d26_i65_insert_keyval(d26_ob_record,i_info,key.k_part + 1,&len,convert,
			  &i_value);
    s_mode = i_info->rule == D2_GTEQ || i_info->rule == D2_LTEQ ?
	    ISGTEQ : ISEQUAL;
    is_count++;
    d26_dbacc_count++;
    if                  /*  error from start query                    */
      (D26_ISSTART(svc_c_route_nolog,isfd,&key,INTSIZE + len,d26_ob_record,
       s_mode) < 0)
    {                   /*  check error number                        */
      if                /*  serious error                             */
	(iserrno != ENOREC)
      {                 /*  write error log                           */
	dce_svc_printf(DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	  svc_c_sev_warning,GDS_S_ISSTART_ERR,iserrno);
	d26_ofildes[filenr].d26_in_use--;
	D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count,
	  ir_count);
	return(message);
      }                 
    }
    else
    {                   /*  set query mode                            */
      r_mode    = ISCURR + ISWAIT;
      go_on     = TRUE;
      d26_reset = FALSE;
      nr_dbacc_in_blocks = d26_dbacc_count *
			   (d26_dbacc_count / D26_DBACC_BLOCK);
      if                /*  base object is not root                   */
	(np_count > 0)
      {                 /*  set tail     to end of DN                 */
	l_rdn = dninfo->d26_rdn + np_count - 1;
	l_ava = l_rdn->d26_r_ava + l_rdn->d26_r_nava - 1;
	tail = l_ava->d26_a_val + l_ava->d26_a_len;
	*tail++ = D2_RDN_SEP;
      }
      else
      {                 /*  set name end to beginning of DN           */
	tail = dname;
      }                 
      while             /*  size and time limit not exceeded          */
	   (go_on == TRUE &&
	    (d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
	    d26_entry_count <= d26_serv_contr->d23_scslm) &&
	    d26_abandoned == FALSE && d26_dbop_flag != D26_DB_NOOP &&
	    d26_time_ex == FALSE)
      {                 /*  read isam                                 */
	ir_count++;
	d26_dbacc_count++;
	if              /*  error from read isam                      */
	  (D26_ISREAD(svc_c_route_nolog,isfd,d26_ob_record,r_mode) < 0)
	{               /*  check error                               */
	  switch(iserrno)
	  {
	    case  EENDFILE:
	      go_on = FALSE;
	      break;
	    default:
	      dce_svc_printf(DCE_SVC(gds_svc_handle,"%d%d"),GDS_S_GENERAL,
		svc_c_sev_warning,GDS_S_ISREAD_ERR,r_mode,iserrno);
	      d26_ofildes[filenr].d26_in_use--;
	      D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,
		is_count,ir_count);
	      return(message);
	  }   
	}
	else
	{               /*  compare key with record                   */
	  if            /*  record matches key                        */
	    (ldint(d26_ob_record + key.k_part[0].kp_start) == ob_nr &&
	     !memcmp(d26_ob_record + key.k_part[1].kp_start,i_value,
	     s_mode == ISGTEQ ? 0 : len))
	  {             /*  check validity of indexed attribute       */
	    if          /*  value is valid                            */
	      (ldint(d26_ob_record + pos) >= i + 1)
	    {           /*  check whether record must be used         */
	      if        /*  record must be used                       */
		(d26_i43_use_record(d26_ob_record,dninfo,np_count,ob_nr,
		 i_info,key.k_part[1].kp_start,i_value,len,i) == TRUE)
	      {         /*  save key from record                      */
		memcpy(save,d26_ob_record + key.k_part[1].kp_start,
		  key.k_part[1].kp_leng);
		if      /*  key includes master knowledge             */
		  (key.k_nparts > 3)
		{       /*  take master knowledge from record         */
		  ao_val = ldint(d26_ob_record + key.k_part[2].kp_start);
		  mk_idx = ldlong(d26_ob_record + key.k_part[3].kp_start);
		}       
		rec_nr = ldlong(d26_ob_record);

		if              /*  error from write subordinate      */
		  ((message = d26_u44_write_child(message,output,dname,
		   dninfo,np_count,tail,ob_nr,r_info,f_info,remlen,
		   output_len,fct_tag)) == NULL)
		{               /*  return error                      */
		  d26_ofildes[filenr].d26_in_use--;
		  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,
		    is_count,ir_count);
		  return(NULL);
		}
		else if       /*  index was changed                 */
		       (d26_reset == TRUE)
		{       /*  reset key                                 */
		  stint(ob_nr,d26_ob_record + key.k_part[0].kp_start);
		  memcpy(d26_ob_record + key.k_part[1].kp_start,save,
		    key.k_part[1].kp_leng);
		  if    /*  key contains master-knowledge             */
		    (key.k_nparts > 3)
		  {     /*  store master-knowledge                    */
		    stint(ao_val,d26_ob_record + key.k_part[2].kp_start);
		    stlong(mk_idx,d26_ob_record + key.k_part[3].kp_start);
		  }     
		  stlong(rec_nr,d26_ob_record);
		  is_count++;
		  d26_dbacc_count++;
		  if    /*  error from start query                    */
		    (D26_ISSTART(svc_c_route_nolog,isfd,&key,0,d26_ob_record,
		     ISGREAT) < 0)
		  {     /*  check error number                        */
		    if  /*  serious error                             */
		      (iserrno != ENOREC)
		    {   /*  write error log                           */
		      dce_svc_printf(DCE_SVC(gds_svc_handle,"%d"),
			GDS_S_GENERAL,svc_c_sev_warning,GDS_S_ISSTART_ERR,
			iserrno);
		    }   
		  }     
		  d26_reset = FALSE;
		}       
	      }         
	    }           
	    r_mode = ISNEXT + ISWAIT;
	  }
	  else
	  {             /*  break loop                                */
	    break;
	  }             
	}               
	if              /*  another DB access block is full           */
	  (d26_dbacc_count > nr_dbacc_in_blocks + D26_DBACC_BLOCK)
	{               /*  read distributed command                  */
	  d26_m11_distcmd(SIGUSR1);
	  nr_dbacc_in_blocks += D26_DBACC_BLOCK;
	}
      }                 
    }                   
  }                     
  d26_ofildes[filenr].d26_in_use--;
}                       

D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count,ir_count);

return(message);

}                       /*  d26_i60_write_children                    */

/**********************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i64_search_attr_file                              */
/*                                                                    */
/* AUTHOR:      Kraemer, AP 11                                        */
/* DATE:        14.05.93                                              */
/*                                                                    */
/* DESCRIPTION: This function searches an attribute value in an       */
/*              attribute file and writes the entry informations of   */
/*              the related entry into message.                       */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      != D2_NULL  byte *          pointer to byte after written     */
/*                                  message                           */
/*         D2_NULL                  error occurred                    */
/*                                                                    */
/**********************************************************************/

static byte *d26_i64_search_attr_file(
    byte *message,      /* IN    -  actual message pointer            */
    byte **output,      /* INOUT -  pointer to beginning of message   */
			/* IN    -  DN of base object                 */
    D2_name_string dname,
    D26_dninfo *dninfo, /* IN    -  information about base object     */
    signed16 np_count,  /* IN    -  count of nameparts in base object */
    signed16 bo_nr,     /* IN    -  object number of base object      */
			/* IN    -  information to be used as index   */
    D26_index_info *i_info,
    D23_rq_info *r_info,/* IN    -  information about required attr.  */
			/* IN    -  information about filter attr.    */
    D23_ft1_info *f_info,
    signed32 *remlen,   /* INOUT -  remaining length for message      */
			/* INOUT -  maximum length of result message  */
    signed32 *output_len,
    signed16 fct_tag)   /* IN    -  indicates the calling function    */

{                               /*  d26_i64_search_attr_file          */

			    /* primary key for objects file           */
  static D26_keydesc o_key = {ISNODUPS,1,{{0,LONGSIZE,LONGTYPE}}};

			    /* function identifier for traces         */
  static char *function = "d26_i64_search_attr_file";

  signed16    a_filenr;     /* number of ISAM file for attributes     */
  signed16    o_filenr;     /* number of ISAM file for objects        */
  D26_keydesc a_key;        /* ISAM key structure for attribute file  */
  int         a_isfd;       /* ISAM file descriptor for attribute file*/
  int         o_isfd;       /* ISAM file descriptor for objects file  */
  int         s_mode;       /* ISAM start mode                        */
  int         r_mode;       /* ISAM read mode                         */
  boolean     go_on;        /* indicates whether loop must be cont.   */
  byte        *i_value;     /* pointer to index value                 */

  D26_rdninfo *l_rdn;       /* pointer to last RDN                    */
  D26_ava     *l_ava;       /* pointer to last AVA                    */
  signed16    len;          /* length of value  to use for index      */

  D2_name_string tail;      /* pointer end of DN of base object       */

  signed32    rec_nr;       /* number of read record                  */
			    /* save key of record for reset           */
  char        save[MAXKEYSIZE];

			    /* converted data for index value         */
  byte        convert[MAXKEYSIZE];

			    /* root of tsearch tree                   */
  void        *tsearch_root = (void *)NULL;

			    /* indicates whether attribute record may */
			    /* be used                                */
  signed16    use_record;

			    /* counts calls of isstart                */
  signed32    is_count = 0;
			    /* counts calls of isread                 */
  signed32    ir_count = 0;
			    /* number of DB accesses in blocks        */
  signed32    nr_dbacc_in_blocks;

  a_filenr  = i_info->at->d26a_filenr;
  o_filenr  = dninfo->d26_filenr;

  if                            /*  error from open attribute file    */
    ((a_isfd = d26_i23_open_file(a_filenr,D26_ATT)) < 0)
  {                             /*  return unchanged message          */
    return(message);
  }
  else
  {                     /*  set "in use flag" for attribute file      */
    d26_afildes[a_filenr].d26_in_use++;
    if                          /*  error from open objects file      */
      ((o_isfd = d26_i23_open_file(o_filenr,D26_OBJ)) < 0)
    {                           /*  return unchanged message          */
      d26_afildes[a_filenr].d26_in_use--;
      return(message);
    }
    else
    {                   /*  set "in use flag" for objects file        */
      d26_ofildes[o_filenr].d26_in_use++;
      d26_i55_sel_rec_key(&a_key,i_info->at,(D2_a_match)i_info->rule,
	D23_SEARCH);
      stint(o_filenr,d26_a_record + a_key.k_part[0].kp_start);
      d26_i65_insert_keyval(d26_a_record,i_info,a_key.k_part + 1,&len,
	convert,&i_value);
      s_mode = i_info->rule == D2_GTEQ || i_info->rule == D2_LTEQ ?
	ISGTEQ : ISEQUAL;
      is_count++;
      d26_dbacc_count++;
      if                /*  error from start query on attribute file  */
	(D26_ISSTART(svc_c_route_nolog,a_isfd,&a_key,INTSIZE + len,
	 d26_a_record,s_mode) < 0)
      {                 /*  check error number                        */
	if              /*  serious error                             */
	  (iserrno != ENOREC)
	{               /*  write error log                           */
	  dce_svc_printf(DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	    svc_c_sev_warning,GDS_S_ISSTART_ERR,iserrno);
	  d26_afildes[a_filenr].d26_in_use--;
	  d26_ofildes[o_filenr].d26_in_use--;
	  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count,
	    ir_count);
	  return(message);
	}
      }
      else if           /*  error from start query on object file     */
	     (D26_ISSTART(svc_c_route_nolog,o_isfd,&o_key,0,d26_ob_record,
	      ISFIRST) < 0)
      {                 /*  check error number                        */
	d26_dbacc_count++;
	is_count++;
	if              /*  serious error                             */
	  (iserrno != ENOREC)
	{               /*  write error log                           */
	  dce_svc_printf(DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	    svc_c_sev_warning,GDS_S_ISSTART_ERR,iserrno);
	  d26_afildes[a_filenr].d26_in_use--;
	  d26_ofildes[o_filenr].d26_in_use--;
	  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count,
	    ir_count);
	  return(message);
	}
      }
      else
      {                 /*  set query mode                            */
	d26_dbacc_count++;
	is_count++;
	r_mode    = ISCURR + ISWAIT;
	go_on     = TRUE;
	d26_reset = FALSE;
	nr_dbacc_in_blocks = d26_dbacc_count *
			     (d26_dbacc_count / D26_DBACC_BLOCK);
	if              /*  base object is not root                   */
	  (np_count > 0)
	{               /*  set tail     to end of DN                 */
	  l_rdn = dninfo->d26_rdn + np_count - 1;
	  l_ava = l_rdn->d26_r_ava + l_rdn->d26_r_nava - 1;
	  tail = l_ava->d26_a_val + l_ava->d26_a_len;
	  *tail++ = D2_RDN_SEP;
	}
	else
	{               /*  set name end to beginning of DN           */
	  tail = dname;
	}
	while           /*  size and time limit not exceeded          */
	     (go_on == TRUE &&
	      (d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
	      d26_entry_count <= d26_serv_contr->d23_scslm) &&
	      d26_abandoned == FALSE && d26_dbop_flag != D26_DB_NOOP &&
	      d26_time_ex == FALSE)
	{               /*  read isam                                 */
	  ir_count++;
	  d26_dbacc_count++;
	  if            /*  error from read isam                      */
	    (D26_ISREAD(svc_c_route_nolog,a_isfd,d26_a_record,r_mode) < 0)
	  {             /*  check error                               */
	    switch(iserrno)
	    {
	      case EENDFILE:
	      case ENOCURR:
		go_on = FALSE;
		break;
	      default:
		dce_svc_printf(DCE_SVC(gds_svc_handle,"%d%d"),GDS_S_GENERAL,
		  svc_c_sev_warning,GDS_S_ISREAD_ERR,r_mode,iserrno);
		d26_afildes[a_filenr].d26_in_use--;
		d26_ofildes[o_filenr].d26_in_use--;
		d26_i69_free_rec_nr(&tsearch_root);
		D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,
		  is_count,ir_count);
		return(message);
	    }
	  }
	  else
	  {             /*  compare key with record                   */
	    if          /*  record matches key                        */
	      (ldint(d26_a_record + a_key.k_part[0].kp_start) == o_filenr &&
	       !memcmp(d26_a_record + a_key.k_part[1].kp_start,i_value,
	       s_mode == ISGTEQ ? 0 : len))
	    {           /*  check whether attribute record may be used*/
	      rec_nr = ldlong(d26_a_record + a_key.k_part[2].kp_start);
	      if        /*  memory error while checking               */
		((use_record = d26_i66_use_attr_record(&tsearch_root,rec_nr))
		 < 0)
	      {         /*  return immediately                        */
		d26_afildes[a_filenr].d26_in_use--;
		d26_ofildes[o_filenr].d26_in_use--;
		d26_i69_free_rec_nr(&tsearch_root);
		D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,
		  is_count,ir_count);
		return(NULL);
	      }
	      else if   /*  record must be used                       */
		     (use_record == TRUE)
	      {         /*  save keypart                              */
		memcpy(save,d26_a_record + a_key.k_part[1].kp_start,
		  a_key.k_part[1].kp_leng);
		stlong(rec_nr,d26_ob_record);
		ir_count++;
		d26_dbacc_count++;
		if      /*  related object record can be read         */
		  (D26_ISREAD(svc_c_sev_warning,o_isfd,d26_ob_record,ISEQUAL)
		   >= 0)
		{       /* check name in record                       */
		  if    /* record contains subordinate of base object */
		    (d26_i63_check_name(dninfo,np_count,d26_ob_record) < 0 &&
		     bo_nr != ldint(d26_ob_record + LONGSIZE))
		  {     /*  write the subordinate into message        */
		    if          /*  error from write subordinate      */
		      ((message = d26_u44_write_child(message,output,dname,
		       dninfo,np_count,tail,ldint(d26_ob_record + LONGSIZE),
		       r_info,f_info,remlen,output_len,fct_tag)) == NULL)
		    {           /*  return error                      */
		      d26_ofildes[o_filenr].d26_in_use--;
		      d26_afildes[a_filenr].d26_in_use--;
		      d26_i69_free_rec_nr(&tsearch_root);
		      D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,
			function,is_count,ir_count);
		      return(NULL);
		    }
		    else if     /*  index was changed                 */
			   (d26_reset == TRUE)
		    {           /* reset key                          */
		      stint(o_filenr,
			d26_a_record + a_key.k_part[0].kp_start);
		      memcpy(d26_a_record + a_key.k_part[1].kp_start,save,
			a_key.k_part[1].kp_leng);
		      stlong(rec_nr,d26_a_record + a_key.k_part[2].kp_start);
		      is_count++;
		      d26_dbacc_count++;
		      if        /* error from start query             */
			(D26_ISSTART(svc_c_route_nolog,a_isfd,&a_key,0,
			 d26_a_record,ISGREAT) < 0)
		      {         /* check error number                 */
			if      /* serious error                      */
			  (iserrno != ENOREC)
			{       /* write error log                    */
			  dce_svc_printf(DCE_SVC(gds_svc_handle,"%d"),
			    GDS_S_GENERAL,svc_c_sev_warning,
			    GDS_S_ISSTART_ERR,iserrno);
			  d26_ofildes[o_filenr].d26_in_use--;
			  d26_afildes[a_filenr].d26_in_use--;
			  d26_i69_free_rec_nr(&tsearch_root);
			  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,
			    function,is_count,ir_count);
			  return(message);
			}
		      }
		      d26_reset = FALSE;
		    }
		  }
		}
	      }
	      r_mode = ISNEXT + ISWAIT;
	    }
	    else
	    {           /*  break loop                                */
	      break;
	    }
	  }
	  if            /*  another DB access block is full           */
	    (d26_dbacc_count > nr_dbacc_in_blocks + D26_DBACC_BLOCK)
	  {             /*  read distributed command                  */
	    d26_m11_distcmd(SIGUSR1);
	    nr_dbacc_in_blocks += D26_DBACC_BLOCK;
	  }
	}
      }
      d26_ofildes[o_filenr].d26_in_use--;
    }
    d26_afildes[a_filenr].d26_in_use--;
  }

  d26_i69_free_rec_nr(&tsearch_root);
  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count,
    ir_count);

  return(message);

}                       /*  d26_i64_search_attr_file                  */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i65_insert_keyval()                               */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        14.05.93                                              */
/*                                                                    */
/* DESCRIPTION: This function inserts the values needed for an index  */
/*              into the record                                       */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      i_info      D26_index_info * information to be used as index  */
/*      k_part      struct keypart * keypart to be used               */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      record      String          C-ISAM record to be filled        */
/*      len         Short *         length of inserted key part       */
/*      convert     Octet_string    memory for converted key value    */
/*      i_value     Octet_string *  ponter to used key value          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_i65_insert_keyval(
    char           *record,
    D26_index_info *i_info,
    struct keypart *k_part,
    signed16       *len,
    byte           *convert,
    byte           **i_value)

{                               /*  d26_i65_insert_keyval             */

if                      /*  phonetic comparison is required           */
  (i_info->rule == D2_APPROX_MATCH)
{                       /*  fill record with phonetic string          */
  *len     = D26_LPHON;
  *i_value = convert;
  d26_soundex(i_info->i_value,i_info->length,convert);
}
else
{                       /*  fill record with index value              */
  switch(i_info->at->d26a_syntax)
  {
    case D2_BOOLEAN:
    case D2_INTEGER:
      *i_value = convert;
      stlong(*(signed32 *)i_info->i_value,(char *)convert);
      *len = LONGSIZE;
      break;
    case D2_MHS_PR_DM_STX:
      for(*len = 0, *i_value = convert;
	  *len < i_info->length && *len < k_part->kp_leng;
	  *len += LONGSIZE, *i_value += LONGSIZE)
      { 
	stlong(*(signed32 *)(i_info->i_value + *len),(char *)*i_value);
      } 
      *i_value = convert;
      break;
    case D2_CEXACT:
    case D2_CIGNORE:
    case D2_COUNTRY_STX:
    case D2_TELNR_STX:
    case D2_NUMERIC:
    case D2_PRINTABLE:
    case D2_IA5:
    case D2_TIME:
    case D2_CIGN_LIST:
    case D2_POST_AD_STX:
      if                /*  record must contain terminating EOS       */
	(i_info->rule == D2_EQUAL && (int)i_info->length < k_part->kp_leng)
      {                 /*  copy index value to private area          */
	*i_value = convert;
	*len     = i_info->length;
	memcpy(convert,i_info->i_value,*len);
	convert[*len] = D2_EOS;
	(*len)++;
	if              /*  list attribute is handled                 */
	  ((i_info->at->d26a_syntax == D2_CIGN_LIST ||
	    i_info->at->d26a_syntax == D2_POST_AD_STX) &&
	   *len < k_part->kp_leng)
	{               /*  insert another EOS                        */
	  convert[*len] = D2_EOS;
	  (*len)++;
	}               
      }
      else
      {                 /*  just use value from message               */
	*i_value = i_info->i_value;
	*len = (int)i_info->length < k_part->kp_leng ?
				     i_info->length : k_part->kp_leng;
      }                 
      break;
    case D2_TTXID_STX:
      *len = d26_is22_st_ttx_term_id((char *)i_info->i_value,
		record + k_part->kp_start,i_info->length);
      *len = *len < k_part->kp_leng ? *len : k_part->kp_leng;
      *i_value = convert;
      memcpy(convert,record + k_part->kp_start,*len);
      break;
    case D2_FAXNR_STX:
      *len = d26_is23_st_fax_number((char *)i_info->i_value,
		record + k_part->kp_start,i_info->length);
      *len = *len < k_part->kp_leng ? *len : k_part->kp_leng;
      *i_value = convert;
      memcpy(convert,record + k_part->kp_start,*len);
      break;
    case D2_MHS_DLSP_STX:
      *len = d26_im23_st_dl_sub_perm((char *)i_info->i_value,
		record + k_part->kp_start,i_info->length);
      *len = *len < k_part->kp_leng ? *len : k_part->kp_leng;
      *i_value = convert;
      memcpy(convert,record + k_part->kp_start,*len);
      break;
    case D2_MHS_ORADR_STX:
      *len = d26_im21_st_or_address((char *)i_info->i_value,
		record + k_part->kp_start,i_info->length);
      *len = *len < k_part->kp_leng ? *len : k_part->kp_leng;
      *i_value = convert;
      memcpy(convert,record + k_part->kp_start,*len);
      break;
    case D2_MHS_ORNAME_STX:
      *len = d26_im22_st_or_name((char *)i_info->i_value,
				 record + k_part->kp_start,i_info->length);
      *len = *len < k_part->kp_leng ? *len : k_part->kp_leng;
      *i_value = convert;
      memcpy(convert,record + k_part->kp_start,*len);
      break;
    case D2_OBJ_IDENT :
      if(i_info->ob_id != D26_IDX_NOT_DEF)
      {   
	*i_value = convert;
	stlong((signed16)i_info->ob_id,(char *)convert);
	*len = LONGSIZE;
	break;
      } 
			    /* go into default case otherwise         */
    default:
      *i_value = i_info->i_value;
      *len = (int)i_info->length < k_part->kp_leng ?
				   i_info->length : k_part->kp_leng;
      break;
  }   
}                       

memcpy(record + k_part->kp_start,*i_value,*len);

}                       /*  d26_i65_insert_keyval                     */

/*inon*****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_i66_use_attr_record                             */
/*                                                                    */
/* AUTHOR       : E. Kraemer, BUBA KP1                                */
/* DATE         : 18.05.93                                            */
/*                                                                    */
/* DESCRIPTION  : The function builds a binary tree of object record  */
/*                numbers. It returns D2_TRUE, if rec_nr is new,      */
/*                D2_FALSE otherwise.                                 */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      rec_nr_root     void **     root of binary tree               */
/*      rec_nr          long        object record number to insert    */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_TRUE         Short       record number must be used        */
/*      D2_FALSE                    record numbers must not be used   */
/*      -1                          error occurred                    */
/*                                                                    */
/**********************************************************************/

static signed16 d26_i66_use_attr_record(void **rec_nr_root, signed32 rec_nr)

{                       /* d26_i66_use_attr_record                    */
  signed32 *rec_nr_tree = NULL; /* memory for record number in tree   */

  void     **tsearch_result;    /* result returned by tsearch         */

if                      /* memory for record number not allocated     */
  (rec_nr_tree == NULL)
{                       /* allocate memory                            */
  if                    /* no memory for record number available      */
    ((rec_nr_tree = (signed32 *)D27_CALLOC(svc_c_sev_warning,1,
     sizeof(signed32))) == NULL)
  {                     /* return error                               */
    return(-1);
  }                     /*                                            */
}                       /*                                            */

			/* assign record number                       */
*rec_nr_tree = rec_nr;
if                      /* no more memory for node in tree            */
  ((tsearch_result = D27_TSEARCH(svc_c_sev_warning,(void *)rec_nr_tree,
   rec_nr_root,d26_i68_cmp_long)) == NULL)
{                       /* return error                               */
  return(-1);
}
else if                 /* record number is in tree already           */
       (*tsearch_result != (void *)rec_nr_tree)
{                       /* return "don't use reecord"                 */
  return(FALSE);
}
else
{                       /* return "use record"                        */
  rec_nr_tree = NULL;
  return(TRUE);
}                       /*                                            */

}                       /* d26_i66_use_attr_record                    */

/*inon*****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_i68_cmp_long                                    */
/*                                                                    */
/* AUTHOR       : E. Kraemer, BUBA KP1                                */
/* DATE         : 18.05.93                                            */
/*                                                                    */
/* DESCRIPTION  : Used by tsearch, to compare two record numbers      */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      rec_nr1         void *      first record number to compare    */
/*      rec_nr2         void *      second record number to compare   */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      -1              int         first record number before second */
/*       0                          record numbers are equal          */
/*      +1                          first record number after second  */
/*                                                                    */
/**********************************************************************/

static int d26_i68_cmp_long(const void *rec_nr1,const void *rec_nr2)

{                       /* d26_i68_cmp_long                           */

if                      /* record numbers are equal                   */
  (*(signed32 *)rec_nr1 == *(signed32 *)rec_nr2)
{                       /* return 'equal'                             */
  return(0);
}
else if                 /* first record number is smaller             */
       (*(signed32 *)rec_nr1 < *(signed32 *)rec_nr2)
{                       /* return 'smaller'                           */
  return(-1);
}
else
{                       /* return 'greater'                           */
  return(1);
}

}                       /* d26_i68_cmp_long                           */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i69_free_rec_nr                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        18.05.93                                              */
/*                                                                    */
/* DESCRIPTION: This function frees all record numbers                */
/*              and deletes them from the tree                        */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      rec_nr_root Void **         root of record number tree        */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      rec_nr_root Void **         root of record number tree        */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_i69_free_rec_nr(void **rec_nr_root)

{                               /*  d26_i69_free_rec_nr               */

  signed32 *act_rec_nr;         /* actual record number to delete     */

for                     /*  all target objects in tree                */
   (; *rec_nr_root != (void *)NULL; )
{                       /*  delete the node from binary tree          */
  act_rec_nr = *(signed32 **)*rec_nr_root;
  tdelete((void *)act_rec_nr,rec_nr_root,d26_i68_cmp_long);
  free(act_rec_nr);
}                       

}                       /*  d26_i69_free_rec_nr                       */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i88_cmp_index                                     */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        17.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function decides, whether adt2 is better to use  */
/*              as index than adt1.                                   */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      m_rule1     D2_a_match      matching rule of first filter item*/
/*      adt1        D26_at *        at entry of first filter item     */
/*      only_first_naming   Bool    indicates whether second filter   */
/*                                  attribute occurs                  */
/*                                  only as a first naming attribute  */
/*      m_rule2     D2_a_match      matching rule of second filter    */
/*      adt2        D26_at *        at entry of second filter item    */
/*      value       Octet_string    value of second filter item       */
/*      length      Ushort          length of value of second filter  */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*      oct_idx     Short *         OCT index for object class filter */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_TRUE     Short           adt2 is better than adt1          */
/*      D2_FALSE                    adt2 is not better than adt1      */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_i88_cmp_index(
    D2_a_match m_rule1,
    D26_at     *adt1,
    signed16   *oct_idx,
    D2_a_match m_rule2,
    boolean    only_first_naming,
    D26_at     *adt2,
    byte       *value,
    signed16   length)

{                               /*  d26_i88_cmp_index                 */

  D2_obj_id ob_id;      /* object identifier structure for object     */
			/* class filters                              */
			/* OCT index of object class filter           */
  signed16  ob_cl = D26_IDX_NOT_DEF;

switch(m_rule2)
{
  case    D2_EQUAL        :
  case    D2_SUBSTRINGS   :
  case    D2_GTEQ         :
  case    D2_LTEQ         :
  case    D2_APPROX_MATCH :
    switch(adt2->d26a_syntax)
    {
      case  D2_OBJ_IDENT:
	if                  /* attribute is object class attribute*/
	  (adt2 == d26_at + d26_obcl_idx)
	{                   /* assign object identifier values    */
	  ob_id.d2_typ_len = (signed32)length;
	  ob_id.d2_type    = value;
	  if                /* object class is not a leaf in tree */
	    (d26_f22_is_ocl_leaf(&ob_id,&ob_cl) == FALSE)
	  {                 /* don't use it for index             */
	    return(FALSE);
	  }                 /*                                    */
	}                   /*                                    */
	/* handle as other cases else */
      case  D2_CEXACT   :
      case  D2_CIGNORE  :
      case  D2_IA5      :
      case  D2_PRINTABLE:
      case  D2_NUMERIC  :
      case  D2_CIGN_LIST:
      case  D2_BOOLEAN  :
      case  D2_INTEGER  :
      case  D2_OCTET    :
      case  D2_TIME     :
      case  D2_TELNR_STX:
      case  D2_POST_AD_STX:
      case  D2_COUNTRY_STX:
      case  D2_PASSWD_STX:
      case  D2_TTXID_STX:
      case  D2_FAXNR_STX:
      case  D2_MHS_DLSP_STX:
      case  D2_MHS_ORADR_STX:
      case  D2_MHS_ORNAME_STX:
      case  D2_MHS_PR_DM_STX:
	if              /*  first index level is smaller than second  */
	  ((adt1->d26a_ind_level <= adt2->d26a_ind_level ||
	    m_rule1 == D26_IDX_NOT_DEF) &&
	   (0 < adt2->d26a_ind_level ||
	   only_first_naming == TRUE && adt2->d26a_nr_recur == 1))
	{               /*  reset the OCT index                       */
	  *oct_idx = ob_cl;
	  return(TRUE);
	}               
			/*  go into default case otherwise            */
      default:
	return(FALSE);
    }   
  default:
    return(FALSE);
}   

}                       /*  d26_i88_cmp_index                         */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i97_index_item                                    */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        09.04.89                                              */
/*                                                                    */
/* DESCRIPTION: This function provides the information to be used for */
/*              the choice of a C-Isam-Index in a query.              */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      dninfo      D26_dninfo *        information about found object*/
/*      np_count    Short               count of nameparts in base    */
/*                                      object                        */
/*      bo_nr       Short               object number of base object  */
/*      ob_nr       Short               object number                 */
/*      f_ind       D23_ft1_info **     information about filter      */
/*                                      attributes                    */
/*      ft_info     D26_attr_info **    Array containing the          */
/*                                      attribute types of filters    */
/*      i_info      D26_index_info *    information to be used for    */
/*                                      index                         */
/*      ignore      Bool                Indicates, whether current    */
/*                                      Filter has to be ignored.     */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      f_ind       D23_ft1_info **     information about filter      */
/*                                      attributes                    */
/*      i_info      D26_index_info *    information to be used for    */
/*                                      index                         */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_i97_index_item(
    D26_dninfo *   dninfo,
    signed16       np_count,
    signed16       bo_nr,
    signed16       ob_nr,
    D23_ft1_info   **f_ind,
    D26_attr_info  **ft_info,
    D26_index_info *i_info,
    boolean        ignore)

{                               /*  d26_i97_index_item                */

  D26_at       *at;         /* pointer to at                          */
  signed16     count;       /* count of filter operators              */
  D23_fi1_info *f_item;     /* pointer to filter item                 */
  D23_at_type  *f_type;     /* pointer to filter type in message      */
  D23_av_value *f_value;    /* pointer to filter value                */
  D23_fs1_info *f_substr;   /* pointer to filter substrings           */
  signed16     length;      /* length of filter value                 */
  D2_a_match   match;       /* matching rule                          */
  byte         *ind_val;    /* filter value to use as index           */
  boolean      only_first_naming;  /* indicates whether a first       */
			    /* naming attribute (with default index)  */
			    /* is used for index                      */

  register signed16 i;      /* loop variable                          */

if                          /*  filter is present                     */
  ((*f_ind)->d23_fp1tag == D23_PATTAG)
{                           /*  check filter operation mode           */
  switch((*f_ind)->d23_fp1mod)
  {
    case D2_AND:            /*  call function recursively for all     */
			    /*  components                            */
      count = (*f_ind)->d23_fp1count;
      (*f_ind) += 1;
      for(i = 0; i < count; i++)
      { 
	d26_i97_index_item(dninfo,np_count,bo_nr,ob_nr,f_ind,ft_info,i_info,
			   ignore);
      } 
      break;
    case D2_OR:             /*  call function recursively for all     */
			    /*  components, but ignore filter         */
      count = (*f_ind)->d23_fp1count;
      (*f_ind) += 1;
      for(i = 0; i < count; i++)
      { 
	d26_i97_index_item(dninfo,np_count,bo_nr,ob_nr,f_ind,ft_info,i_info,
			   TRUE);
      } 
      break;
    case D2_NOT:            /*  call function recursively for the     */
			    /*  single component, but ignore filter   */
      (*f_ind) += 1;
      d26_i97_index_item(dninfo,np_count,bo_nr,ob_nr,f_ind,ft_info,i_info,
			 TRUE);
      break;
    case D2_ITEM:           /*  set the filter item pointer           */
      f_item = (D23_fi1_info *)(*f_ind + 1);
      if                    /*  filter item must be checked           */
	(ignore == FALSE && (*ft_info)->d26_at_idx >= 0)
      {                     /*  initialize match "invalid"            */
	f_type = (D23_at_type *)(f_item + 1);
	at     = d26_at + (*ft_info)->d26_at_idx;
	match  = D26_IDX_NOT_DEF;
			    /*  set the information to be used for an */
			    /*  index into variables                  */
	switch(f_item->d23_fi1match)
	{
	  case D2_EQUAL:
	  case D2_GTEQ:
	  case D2_LTEQ:
	  case D2_APPROX_MATCH:
	    f_value = (D23_av_value *)((String)(f_type + 1) +
		      f_type->d23_atoff);
	    length  = f_value->d23_avlen;
	    ind_val = (byte *)(f_value + 1);
	    if              /*  filter value does not match base obj. */
	      (d26_f23_bo_matches_equal(dninfo,np_count,f_item->d23_fi1match,
	       at,ind_val,length) == FALSE)
	    {               /*  reset match                           */
	      match   = f_item->d23_fi1match;
	    }               
	    break;
	  case D2_SUBSTRINGS:
	    f_substr = (D23_fs1_info *)((String)(f_type + 1) +
		       f_type->d23_atoff);
	    if(f_substr->d23_fs1position == D2_INITIAL)
	    {   
	      if            /*  filter substrings do not match base o.*/
		(d26_f24_bo_matches_substrings(dninfo,np_count,at,f_substr)
		 == FALSE)
	      {             /*  reset match                           */
		match = D2_SUBSTRINGS;
		length  = f_substr->d23_fs1len;
		ind_val = (byte *)(f_substr + 1);
	      }             
	    } 
	    break;
	}   

	only_first_naming = d26_i98_only_first_naming(bo_nr,ob_nr,*ft_info);
	if                  /*  attribute provides a better index     */
	  (d26_i88_cmp_index(i_info->rule,i_info->at,&i_info->ob_id,match,
	   only_first_naming,at,ind_val,length) > 0)
	{                   /*  reset the index information           */
	  i_info->at      = d26_at + (*ft_info)->d26_at_idx;
	  if                /*  attribute is in object classes but not*/
			    /*  as a naming one                       */
	    ((*ft_info)->d26_in_ocls == TRUE &&
	     (*ft_info)->d26_srt_idx != ob_nr)
	  {                 /*  set SRT pointer in index info invalid */
	    i_info->srt = NULL;
	  }
	  else
	  {                 /*  set SRT pointer in index info valid   */
	    i_info->srt    = d26_srt + (*ft_info)->d26_srt_idx;
	  }                 
	  i_info->length  = length;
	  i_info->rule    = match;
	  i_info->i_value = ind_val;
	} 
      } 
      (*ft_info)++;
      *f_ind = (D23_ft1_info *)d26_f07_next_fitem(f_item);
      break;
  }   
}                       

return;

}                       /*  d26_i97_index_item                        */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i98_only_first_naming                             */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        07.04.92                                              */
/*                                                                    */
/* DESCRIPTION: This function checks, whether the attribute described */
/*              by ft_info occurs only as a first naming attribute    */
/*              in the name tree from ob_nr up to bo_nr. Only in this */
/*              case the attribute may be chosen for a C-ISAM index,  */
/*              although the index level is not positive, where the   */
/*              default index may be used instead.                    */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      bo_nr       Short               object number of base object  */
/*      ft_info     D26_attr_info *     Array containing the          */
/*                                      attribute types of filters    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*inoff ***************************************************************/

static boolean d26_i98_only_first_naming(
    signed16       bo_nr,
    signed16       ob_nr,
    D26_attr_info  *ft_info)

{                               /*  d26_i98_only_first_naming         */

  boolean ret_value;        /* return value                           */

  register D26_srt *srt;        /* loop pointer to SRT                */
  register D26_nam_at *nam_at;  /* loop pointer to naming attribute   */
				/* structures                         */

if                      /*  attribute belongs to object class         */
  (ft_info->d26_in_ocls == TRUE)
{                       /*  return "don't use"                        */
  return(FALSE);
}
else
{                       /*  scan the SRT entries of name structure    */
  for                   /*  all SRT-entries below the base object     */
     (ret_value = FALSE, srt = d26_srt + ob_nr; srt != d26_srt + bo_nr;
      srt = d26_srt + srt->d26s_par_nr)
  {                     /*  scan the naming attributes                */
    for                 /*  all naming attributes                     */
       (nam_at = srt->d26s_nam_at; nam_at->d26s_index != -1; nam_at++)
    {                   /*  compare attribute index with filter attr. */
      if                /*  attribute indices match                   */
	(nam_at->d26s_index == ft_info->d26_at_idx)
      {                 /*  check whether naming attribute is the     */
			/*  first one                                 */
	if              /*  naming attribute is the first             */
	  (nam_at == srt->d26s_nam_at)
	{               /*  reset the return value                    */
	  ret_value = TRUE;
	}
	else
	{               /*  return "don't use"                        */
	  return(FALSE);
	}               
      }                 
    }                   
  }                     
}                       

return(ret_value);

}                       /*  d26_i98_only_first_naming                 */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i9c_namepart_repeated()                           */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        16.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function looks up, whether in the sequence of    */
/*              SRT entries between sub_nr and srt the attribute with */
/*              index at_idx occurs as a naming attribute.            */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      srt         D26_srt *       srt-entry of object               */
/*      sub_nr      Short           object number of subordinate      */
/*                                  object.                           */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_TRUE     Short           attribute type occurs             */
/*      D2_FALSE                    attribute type doesn't occur      */
/*                                                                    */
/*inoff ***************************************************************/

static boolean d26_i9c_namepart_repeated(
    D26_srt  *srt,
    signed16 at_idx,
    signed16 sub_nr)

{                               /*  d26_i9c_namepart_repeated         */

  register D26_srt *cmp_srt;        /* pointer to structure rule table*/
  register D26_nam_at *nam_at;      /* pointer to naming attributes   */

for                                 /*  all intermediate srt-entries  */
   (cmp_srt = d26_srt + sub_nr; cmp_srt != srt;
    cmp_srt = d26_srt + cmp_srt->d26s_par_nr)
{                                   /*  check attribute types         */
  for                               /*  all naming attributes         */
     (nam_at = cmp_srt->d26s_nam_at; nam_at->d26s_index != -1; nam_at++)
  {                                 /*  compare the AT indices        */
    if                              /*  types are equal               */
      (nam_at->d26s_index == at_idx)
    {                               /*  reset index attribute         */
      return(TRUE);
    }                               
  }                                 
}                                   

return(FALSE);

}                       /*  d26_i9c_namepart_repeated                 */

