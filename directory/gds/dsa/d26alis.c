/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26alis.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:53  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:42:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:12  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:45:08  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:30:33  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:51:45  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:19:21  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:09:03  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:19:36  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:05:05  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:08:21  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  13:35:28  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:00:57  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:58:48  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:18:07  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:34:09  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:03:20  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:45:12  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:52:58  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26alis.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:46 $";
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
/* NAME         : d26alis.c      [alis]                               */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 27.12.88                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains the CISAM utilities used by   */
/*                  the internal search function, that handle         */
/*                  aliases.                                          */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 28.12.88| birth                          | ek |         */
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

/******** LOCAL  ********/

static signed16 d26_i62_search_al_in_file(D26_target *cmp_target,
    D2_subset subset, D26_dninfo *dninfo, signed16 np_count,
    D2_name_string dname, D26_srt *nam_srt, D26_rdninfo *nam_rdn, D26_at *at,
    signed16 ob_nr, void **target_root, D26_target **act_target,
    byte **message, byte **output, signed32 *output_len, signed32 *remlen,
    D23_rq_info *r_info, D23_ft1_info *f_info, signed16 op_id,
    boolean *cont_sa);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i47_get_al_references()                           */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        28.04.88                                              */
/*                                                                    */
/* DESCRIPTION: This function reads from CISAM-record the DN-List     */
/*              index of the aliased object attribute.                */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      -1          signed32        record doesn't contain alias      */
/*      >= 0                        DN-List-index of aliased object   */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_i47_get_al_references(
    signed16 filenr,    /* IN    -  number of C-ISAM objects file     */
    char     *record)   /* IN    -  record containing object          */

{                               /*  d26_i47_get_al_references         */

  signed16 al_pos;              /* position of alias attribute        */

  al_pos = d26_at[d26_alob_idx].d26a_field_pos[filenr];
  if                    /*  object is alias                           */
    (ldint(record + al_pos) > 0)
  {                     /*  return alias reference                    */
    return(ldlong(record + al_pos + INTSIZE));
  }

  return(-1L);

}                               /*  d26_i47_get_al_references         */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i50_search_aliases                                */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        16.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function determines all object numbers of name   */
/*              structures that describe objects subordinate to the   */
/*              DN dname.    For all such object numbers it determines*/
/*              the C-ISAM file, that holds the aliases with this     */
/*              name structure and calls a function to search them.   */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      cmp_target  D26_target *    target object to compare          */
/*      subset      D2_subset       indicates subset to be searched   */
/*      dname       D26_name_string DN of target object               */
/*      dninfo      D26_dninfo *    information about target object   */
/*      to_nr       Short           target object number              */
/*      target_root Void **         root of target object tree        */
/*      act_target  D26_target **   actual target object              */
/*      message     String **       actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      output_len  Int *           maximum length of result message  */
/*      remlen      Int *           remaining length for result data  */
/*      r_info      D23_rq_info *   information about required attr.  */
/*      f_info      D23_ft1_info *  information about filter attr.    */
/*      op_id       Short           indicates the calling function    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      target_root Void **         root of target object tree        */
/*      act_target  D26_target **   actual target object              */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D26_ERROR   Short           error occurred                    */
/*      D2_NO_ERR                   no error occurred                 */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i50_search_aliases(
    D26_target     *cmp_target,
    D2_subset      subset,
    D2_name_string dname,
    D26_dninfo     *dninfo,
    signed16       to_nr,
    void           **target_root,
    D26_target     **act_target,
    byte          **message,
    byte          **output,
    signed32       *output_len,
    signed32       *remlen,
    D23_rq_info    *r_info,
    D23_ft1_info   *f_info,
    signed16       op_id)

{                               /*  d26_i50_search_aliases            */

  signed16    np_count;         /* count of nameparts in base object  */
  D26_rdninfo *nam_rdn;         /* pointer to last RDN                */
  D26_srt     *nam_srt;         /* pointer to SRT entry of base object*/
  D26_at      *at;              /* pointer to AT entry of attribute to*/
				/* use for index                      */
  boolean     cont_sa;          /* indicates that search alias must   */
				/* continue                           */

  register signed16 i;          /* loop variable                      */
  register D26_srt  *srt;       /* pointer to SRT                     */

DCE_SVC_LOG((GDS_S_ALIAS_SUB_SEARCH_MSG,dname));

np_count = dninfo->d26_rd_anz;
if                                  /*  base object is not root       */
  (np_count > 0)
{                                   /*  assign pointers describing    */
				    /*  last RDN                      */
  nam_rdn = dninfo->d26_rdn + np_count - 1;
  nam_srt = d26_srt + nam_rdn->d26_r_idx;
  at      = d26_at + nam_rdn->d26_r_ava->d26_a_idx;
}                                   


for                     /*  all SRT entries                           */
   (i = 0, srt = d26_srt, cont_sa = TRUE;
    i < d26_nosrt && d26_time_ex == FALSE &&
    d26_dbop_flag != D26_DB_NOOP && cont_sa == TRUE; i++, srt++)
{                       /*  check whether child found                 */
  if                    /*  child or descendant is found              */
    (subset == D2_ONE_LEVEL && srt->d26s_par_nr == to_nr ||
     subset == D2_WHOLE_SUBTREE && d26_u46_descendant(to_nr,i) == TRUE)
  {                     /*  write all matching children               */
    dninfo->d26_filenr = d26_oct[srt->d26s_ocl_arc->d26s_oclass].d26c_filenr;
    d26_u67_trace_name_struct(srt);
    DCE_SVC_LOG((GDS_S_ALIAS_SEARCH_MSG,dninfo->d26_filenr));
    if                  /*  error from searching file                 */
      (d26_i62_search_al_in_file(cmp_target,subset,dninfo,np_count,dname,
       nam_srt,nam_rdn,at,i,target_root,act_target,message,output,output_len,
       remlen,r_info,f_info,op_id,&cont_sa) == D26_ERROR)
    {                   /*  return error                              */
      return(D26_ERROR);
    }                   
  }                     
}                       

return(D2_NO_ERR);

}                       /*  d26_i50_search_aliases                    */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i62_search_al_in_file                             */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        17.07.92                                              */
/*                                                                    */
/* DESCRIPTION: This function searches and dereferences all aliases   */
/*              subordinate to the target object. It calls the use    */
/*              aliases function for all found aliases.               */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      cmp_target  D26_target *    target object to compare          */
/*      subset      D2_subset       indicates subset to be searched   */
/*      dninfo      D26_dninfo *    information about target object   */
/*      np_count    Short           count of nameparts of target      */
/*                                  object                            */
/*      dname       D26_name_string DN of target object               */
/*      nam_srt     D26_srt     *   pointer to SRT entry ...          */
/*      nam_rdn     D26_rdninfo *   pointer to last RDN ...           */
/*                                  ... of target object              */
/*      at          D26_at      *   pointer to AT entry of attribute  */
/*                                  to use for index                  */
/*      ob_nr       Short           object number                     */
/*      target_root Void **         root of target object tree        */
/*      act_target  D26_target **   actual target object              */
/*      message     String **       actual message pointer            */
/*      output      String *        pointer to beginning of message   */
/*      output_len  Int *           maximum length of result message  */
/*      remlen      Int *           remaining length for result data  */
/*      r_info      D23_rq_info *   information about required attr.  */
/*      f_info      D23_ft1_info *  information about filter attr.    */
/*      op_id       Short           indicates the calling function    */
/*      cont_sa     Bool *          indicates whether search_aliases  */
/*                                  should continue                   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      target_root Void **         root of target object tree        */
/*      act_target  D26_target **   actual target object              */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D26_ERROR   Short           error occurred                    */
/*      D2_NO_ERR                   no error occurred                 */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_i62_search_al_in_file(
    D26_target     *cmp_target,
    D2_subset      subset,
    D26_dninfo     *dninfo,
    signed16       np_count,
    D2_name_string dname,
    D26_srt        *nam_srt,
    D26_rdninfo    *nam_rdn,
    D26_at         *at,
    signed16       ob_nr,
    void           **target_root,
    D26_target     **act_target,
    byte           **message,
    byte           **output,
    signed32       *output_len,
    signed32       *remlen,
    D23_rq_info    *r_info,
    D23_ft1_info   *f_info,
    signed16       op_id,
    boolean        *cont_sa)

{                               /*  d26_i62_search_al_in_file        */

			    /* function identifier for logging        */
  static String  function = "i62";

			    /* field containing last RDN including all*/
			    /* trailing EOS's                         */
  char           nam_field[D2_NP_LEN + 1];

  D26_keydesc    key;       /* ISAM key structure                     */
  int            isfd;      /* ISAM file descriptor                   */
  int            ind_len;   /* length of index in bytes               */
  int            mode;      /* ISAM read mode                         */

  signed16       alob_validity; /* validity of aliased object attrib. */

  D2_name_string name_end;  /* end of target object name              */
  D2_name_string tail;      /* pointer end of DN of target object     */

  signed16       filenr;    /* number of accessed file                */
  signed32       rec_nr;    /* number of read record                  */
  boolean        db_access; /* indicates intermediate DB-access       */
  boolean        go_on;     /* indicates that loop must be continued  */
			    /* counts calls of isstart                */
  signed32       is_count = 0;
			    /* counts calls of isread                 */
  signed32       ir_count = 0;
			    /* number of DB accesses in blocks        */
  signed32       nr_dbacc_in_blocks;

  register       D26_ava *ava;  /* pointer to AVAs of RDN             */

if                          /*  no error from open file               */
  ((isfd = d26_i23_open_file(dninfo->d26_filenr,D26_OBJ)) >= 0)
{                           /*  select key                            */
  if                    /*  base object is not root                   */
    (np_count > 0)
  {                     /*  key includes last RDN                     */
    d26_i32_select_key(&key,
	nam_srt->d26s_nam_at->d26s_position[dninfo->d26_filenr],
	at,0,(D2_a_match)D2_EQUAL,dninfo->d26_filenr);
			/*  write object number to record             */
    stint(ob_nr,d26_ob_record + key.k_part[0].kp_start);
			/*  write last namepart to record             */
    d26_u02_fill_field(D2_EOS,nam_rdn->d26_r_ava->d26_a_len,
      (char *)nam_rdn->d26_r_ava->d26_a_val,key.k_part[1].kp_leng,nam_field);
    memcpy(d26_ob_record + key.k_part[1].kp_start,nam_field,
      key.k_part[1].kp_leng);
			/*  write aliased object validity to record   */
    stint(1,d26_ob_record + key.k_part[2].kp_start);
    ind_len = INTSIZE + key.k_part[1].kp_leng + INTSIZE;
  }
  else
  {                     /*  key on aliased object attribute only      */
    d26_i32_select_key(&key,0,(D26_at *)NULL,0,(D2_a_match)D2_EQUAL,
		       dninfo->d26_filenr);
			/*  write object number to record             */
    stint(ob_nr,d26_ob_record + key.k_part[0].kp_start);
			/*  write aliased object validity to record   */
    stint(1,d26_ob_record + key.k_part[1].kp_start);
    ind_len = INTSIZE + INTSIZE;
  }                     
  is_count++;
  d26_dbacc_count++;
  if                    /*  error from start query                    */
    (D26_ISSTART(svc_c_route_nolog,isfd,&key,ind_len,d26_ob_record,ISEQUAL)
     < 0)
  {                     /*  check error number                        */
    if                  /*  serious error                             */
      (iserrno != ENOREC)
    {                   /*  write error log                           */
      dce_svc_printf(DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	svc_c_sev_warning,GDS_S_ISSTART_ERR,iserrno);
    }                   
  }
  else
  {                     /*  read records                              */
    if                  /*  base object is not root                   */
      (np_count > 0)
    {                   /*  set tail to end of DN                     */
      ava  = nam_rdn->d26_r_ava + nam_rdn->d26_r_nava - 1;
      tail = ava->d26_a_val + ava->d26_a_len;
      *tail++ = D2_RDN_SEP;
    }
    else
    {                   /*  set name end to beginning of DN           */
      tail = dname;
    }                   
    mode = ISCURR + ISWAIT;
    d26_ofildes[dninfo->d26_filenr].d26_in_use++;
    nr_dbacc_in_blocks = d26_dbacc_count *
      (d26_dbacc_count / D26_DBACC_BLOCK);
    go_on = TRUE;
    while(go_on == TRUE && *cont_sa == TRUE && d26_abandoned == FALSE &&
	  d26_dbop_flag != D26_DB_NOOP && d26_time_ex == FALSE)
    {                   /*  read ISAM records                         */
      ir_count++;
      d26_dbacc_count++;
      if                /*  error from read isam                      */
	(D26_ISREAD(svc_c_route_nolog,isfd,d26_ob_record,mode) < 0)
      {                 /*  check error                               */
	switch(iserrno)
	{
	  case  EENDFILE:
	    go_on = FALSE;
	    break;
	  default:
	    dce_svc_printf(DCE_SVC(gds_svc_handle,"%d%d"),GDS_S_GENERAL,
	      svc_c_sev_warning,GDS_S_ISREAD_ERR,mode,iserrno);
	    go_on = FALSE;
	    break;
	}   
      }
      else
      {                 /*  compare key filter                        */
	if              /*  record matches key                        */
	  (ldint(d26_ob_record + key.k_part[0].kp_start) == ob_nr &&
	   (np_count > 0 && !memcmp(d26_ob_record + key.k_part[1].kp_start,
	    nam_field,key.k_part[1].kp_leng) &&
	    (alob_validity = ldint(d26_ob_record + key.k_part[2].kp_start))
	     == 1 ||
	    np_count == 0 &&
	    (alob_validity = ldint(d26_ob_record + key.k_part[1].kp_start))
	     == 1))
	{               /*  check other nameparts of base object      */
	  if            /*  all nameparts match                       */
	    (d26_i63_check_name(dninfo,np_count,d26_ob_record) < 0)
	  {             /*  save record number                        */
	    rec_nr    = ldlong(d26_ob_record);
	    filenr    = dninfo->d26_filenr;
	    db_access = FALSE;
	    name_end  = tail;
	    d26_i34_get_dninfo(d26_ob_record,dninfo,&name_end,np_count);
	    DCE_SVC_LOG((GDS_S_ALIAS_FOUND_MSG,dname));
	    if          /*  base object is not root                   */
	      (np_count > 0)
	    {           /*  insert EOS into DN                        */
	      tail[-1] = D2_EOS;
	    }           
	    else
	    {           /*  insert EOS into DN                        */
	      tail[0] = D2_EOS;
	    }           
	    if          /*  alias could not be used                   */
	      (d26_u73_use_alias(cmp_target,subset,filenr,target_root,
	       act_target,&db_access,message,output,output_len,remlen,r_info,
	       f_info,op_id,cont_sa) == D26_ERROR)
	    {           /*  return error                              */
	      d26_ofildes[filenr].d26_in_use--;
	      return(D26_ERROR);
	    }
	    else if     /*  base object is not root                   */
		   (np_count > 0)
	    {           /*  insert RDN separator into DN              */
	      tail[-1] = D2_RDN_SEP;
	    }           
	    if          /*  database was accessed in the meantime     */
	      (*cont_sa == TRUE && db_access == TRUE)
	    {           /*  restore file number and index             */
	      dninfo->d26_filenr = filenr;
	      stint(ob_nr,d26_ob_record + key.k_part[0].kp_start);
	      if        /*  base object is not root                   */
		(np_count > 0)
	      {         /*  key includes last RDN                     */
		memcpy(d26_ob_record + key.k_part[1].kp_start,nam_field,
		       key.k_part[1].kp_leng);
		stint(1,d26_ob_record + key.k_part[2].kp_start);
		stlong(0L,d26_ob_record + key.k_part[3].kp_start);
		stlong(rec_nr,d26_ob_record + key.k_part[4].kp_start);
	      }
	      else
	      {         /*  key on aliased object attribute only      */
		stint(1,d26_ob_record + key.k_part[1].kp_start);
		stlong(0L,d26_ob_record + key.k_part[2].kp_start);
		stlong(rec_nr,d26_ob_record + key.k_part[3].kp_start);
	      }         
			/*  reset key                                 */
	      is_count++;
	      d26_dbacc_count++;
	      if        /*  error from start query                    */
		(D26_ISSTART(svc_c_route_nolog,isfd,&key,0,d26_ob_record,
		 ISGREAT) < 0)
	      {         /*  check error number                        */
		if      /*  serious error                             */
		  (iserrno != ENOREC)
		{       /*  write error log                           */
		  dce_svc_printf(DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
		    svc_c_sev_warning,GDS_S_ISSTART_ERR,iserrno);
		}       
		break;
	      }         
	    }           
	  }             
	  mode = ISNEXT + ISWAIT;
	}
	else
	{               /*  break loop                                */
	  break;
	}               
      }                 
      if                /* another DB access block is full            */
	(d26_dbacc_count > nr_dbacc_in_blocks + D26_DBACC_BLOCK)
      {                 /* read distributed command                   */
	d26_m11_distcmd(SIGUSR1);
	nr_dbacc_in_blocks += D26_DBACC_BLOCK;
      }
    }                   
    d26_ofildes[dninfo->d26_filenr].d26_in_use--;
  }                     
}                       

D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count,ir_count);

return(D2_NO_ERR);

}                       /*  d26_i62_search_al_in_file                 */
