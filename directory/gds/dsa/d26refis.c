/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26refis.c,v $
 * Revision 1.1.726.2  1996/02/18  19:45:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:12  marty]
 *
 * Revision 1.1.726.1  1995/12/08  15:47:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:38  root]
 * 
 * Revision 1.1.724.5  1994/07/06  15:07:04  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:13:44  marrek]
 * 
 * Revision 1.1.724.4  1994/06/21  14:45:46  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  11:39:58  marrek]
 * 
 * Revision 1.1.724.3  1994/05/10  15:52:36  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:23:40  marrek]
 * 
 * Revision 1.1.724.2  1994/03/23  15:09:38  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:22:17  keutel]
 * 
 * Revision 1.1.724.1  1994/02/22  17:52:29  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:17:01  marrek]
 * 
 * Revision 1.1.722.3  1993/10/22  17:18:21  keutel
 * 	CR 9237
 * 	[1993/10/22  16:03:41  keutel]
 * 
 * Revision 1.1.722.2  1993/10/14  17:16:37  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:11:10  keutel]
 * 
 * Revision 1.1.722.1  1993/10/13  17:30:59  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:28:28  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  14:32:19  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:15:49  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:41:47  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:20:40  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:18:43  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:15:09  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:51:30  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:57:32  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26refis.c,v $ $Revision: 1.1.726.2 $ $Date: 1996/02/18 19:45:50 $";
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
/* NAME         : d26refis.c      [refis]                             */
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
/*                  referrals.                                        */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 27.12.88| birth                          | ek |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <sys/types.h>
#include <stdlib.h>
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

static void d26_i46_referrals_from_file(D26_dninfo *dninfo,
    signed16 np_count, D2_name_string dname, D26_srt *nam_srt,
    D26_rdninfo *nam_rdn, D26_at *at, D26_at *psap_at, signed16 ob_nr);
static void d26_i61_make_ch_referrals(D26_dninfo *dninfo, signed16 np_count,
    D2_name_string dname, D26_at *psap_at, signed16 ob_nr);

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i46_referrals_from_file                           */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        15.03.91                                              */
/*                                                                    */
/* DESCRIPTION: This function searches for referrals of a given object*/
/*              number in a given file and creates eventually         */
/*              referrals into memory.                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              dninfo      D26_dninfo *    information about base    */
/*                                          object.                   */
/*              np_count    Short           count of nameparts of     */
/*                                          base object               */
/*              dname       D26_name_string DN of base object         */
/*              nam_rdn     D26_rdninfo *   pointer to last RDN ...   */
/*              nam_srt     D26_srt     *   pointer to SRT entry ...  */
/*                                          ... of base object        */
/*              at          D26_at      *   pointer to AT entry of    */
/*                                          attribute to use for index*/
/*              psap_at     D26_at *        pointer to AT-entry of    */
/*                                          PSAP-Address              */
/*              shb_at      D26_at *        pointer to AT-entry of    */
/*                                          Shadowed-by-attribute     */
/*              ob_nr       Short           object number             */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_i46_referrals_from_file(
    D26_dninfo *dninfo,
    signed16 np_count,
    D2_name_string dname,
    D26_srt *nam_srt,
    D26_rdninfo *nam_rdn,
    D26_at *at,
    D26_at *psap_at,
    signed16 ob_nr)

{                                /*  d26_i46_referrals_from_file      */

			    /* function identifier for traces         */
  static      char *function = "d26_i46_referrals_from_file";

			    /* field containing last RDN including all*/
			    /* trailing EOS's                         */
  char        nam_field[D2_NP_LEN + 1];

  D26_keydesc key;          /* ISAM key structure                     */
  int         isfd;         /* ISAM file descriptor                   */
  int         ind_len;      /* length of index in bytes               */
  int         mode;         /* ISAM read mode                         */

  signed32    mk;           /* index into DN list for master knowledge*/
  signed16    acp_idx;      /* index of Access-point in list          */
  D2_name_string name_end;  /* end of target object for referral      */
  D2_name_string tail;      /* pointer end of DN of base object       */
			    /* exact name of target object            */
  byte        exact_obj[D2_DNL_MAX];

  signed16    filenr;       /* number of accessed file                */
  signed32    rec_nr;       /* number of read record                  */
  D26_ava     *ava;         /* pointer to AVAs of RDN                 */
  signed16    mk_pos;       /* position of master knowledge attribute */
  boolean     db_access;    /* indicates intermediate DB-access       */

  signed32    is_count = 0; /* counts calls of isstart                */

  signed32    ir_count = 0; /* counts calls of isread                 */
			    /* number of DB accesses in blocks        */
  signed32    nr_dbacc_in_blocks;

  register    boolean go_on;/* indicates that loop must be continued  */

if                          /*  no error from open file               */
  ((isfd = d26_i23_open_file(dninfo->d26_filenr,D26_OBJ)) >= 0)
{                           /*  select key                            */
  DCE_SVC_LOG((GDS_S_REF_SEARCH_MSG,dninfo->d26_filenr));
  if                    /*  base object is not root                   */
    (np_count > 0)
  {                     /*  key includes last RDN                     */
    d26_i32_select_key(&key,
        nam_srt->d26s_nam_at->d26s_position[dninfo->d26_filenr],
        at,0,(D2_a_match)D2_EQUAL,dninfo->d26_filenr);
			/*  put object number into keyrecord          */
    stint(ob_nr,d26_ob_record + key.k_part[0].kp_start);
			/*  put namepart into keyrecord               */
    d26_u02_fill_field(D2_EOS,nam_rdn->d26_r_ava->d26_a_len,
      (char *)nam_rdn->d26_r_ava->d26_a_val,key.k_part[1].kp_leng,nam_field);
    memcpy(d26_ob_record + key.k_part[1].kp_start,nam_field,
      key.k_part[1].kp_leng);
			/*  put aliased object invalid into keyrecord */
    stint(0,d26_ob_record + key.k_part[2].kp_start);
			/*  put master knowledge into keyrecord       */
    stlong(0L,d26_ob_record + key.k_part[3].kp_start);
    mk_pos  = key.k_part[3].kp_start - INTSIZE;
    ind_len = INTSIZE + key.k_part[1].kp_leng + INTSIZE + LONGSIZE;
  }
  else
  {                     /*  key on master-knowledge attribute only    */
    d26_i32_select_key(&key,0,(D26_at *)NULL,0,(D2_a_match)D2_EQUAL,
		       dninfo->d26_filenr);
			/*  put object number into keyrecord          */
    stint(ob_nr,d26_ob_record + key.k_part[0].kp_start);
			/*  put aliased object invalid into keyrecord */
    stint(0,d26_ob_record + key.k_part[1].kp_start);
			/*  put master knowledge into keyrecord       */
    stlong(0L,d26_ob_record + key.k_part[2].kp_start);
    mk_pos  = key.k_part[2].kp_start - INTSIZE;
    ind_len = INTSIZE + INTSIZE + LONGSIZE;
  }                     
  is_count++;
  d26_dbacc_count++;
  if                    /*  error from start query                    */
    (D26_ISSTART(svc_c_route_nolog,isfd,&key,ind_len,d26_ob_record,ISGTEQ)
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
      ava     = nam_rdn->d26_r_ava + nam_rdn->d26_r_nava - 1;
      tail    = ava->d26_a_val + ava->d26_a_len;
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
    while             /* no limit is exceeded                         */
	 (go_on == TRUE && d26_ref_count + 1 < D26_REF_BLOCKSIZE &&
	  d26_abandoned == FALSE && d26_dbop_flag != D26_DB_NOOP &&
	  d26_time_ex == FALSE)
    {                 /*  read ISAM records                           */
      ir_count++;
      d26_dbacc_count++;
      if              /*  error from read isam                        */
	(D26_ISREAD(svc_c_route_nolog,isfd,d26_ob_record,mode) < 0)
      {               /*  check error                                 */
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
      {               /*  compare key filter                          */
	if            /*  record matches key                          */
	  (ldint(d26_ob_record + key.k_part[0].kp_start) == ob_nr &&
	   (np_count > 0 && !memcmp(d26_ob_record + key.k_part[1].kp_start,
	    nam_field,key.k_part[1].kp_leng) &&
	    ldint(d26_ob_record + key.k_part[2].kp_start) == 0 &&
	    (mk = ldlong(d26_ob_record + key.k_part[3].kp_start)) >= 0 ||
	    np_count == 0 &&
	    ldint(d26_ob_record + key.k_part[1].kp_start) == 0 &&
	    (mk = ldlong(d26_ob_record + key.k_part[2].kp_start)) >= 0))
	{             /*  check other nameparts of base object        */
	  if          /*  all nameparts match and master knowledge is */
		      /*  valid                                       */
	    (d26_i63_check_name(dninfo,np_count,d26_ob_record) < 0 &&
	     ldint(d26_ob_record + mk_pos) > 0)
	  {           /*  save record number                          */
	    rec_nr    = ldlong(d26_ob_record);
	    filenr    = dninfo->d26_filenr;
	    db_access = FALSE;
	    name_end = tail;
	    d26_i34_get_dninfo(d26_ob_record,dninfo,&name_end,np_count);
	    dninfo->d26_rd_anz--;
	    d26_i48_get_exact_name(d26_ob_record,dninfo,0,exact_obj);
	    dninfo->d26_rd_anz++;
	    if          /*  access point found                        */
	      (d26_u35_find_access_point(mk,D2_SOBJ,&acp_idx,psap_at,
	       &db_access) == D2_NO_ERR)
	    {           /*  create referral                           */
	      DCE_SVC_LOG((GDS_S_ACC_POINT_FOUND_MSG,
		d26_accp_list[acp_idx].d26_dsaname));
	      d26_u58_make_subordinate_ref(dninfo,np_count,dname,exact_obj,
		&db_access,acp_idx,psap_at);
	    }           
	    if          /*  database was accessed in the meantime     */
	      (db_access == TRUE)
	    {           /*  restore file number and index             */
	      dninfo->d26_filenr = filenr;
	      stint(ob_nr,d26_ob_record + key.k_part[0].kp_start);
	      if        /*  base object is not root                   */
		(np_count > 0)
	      {         /*  key includes last RDN                     */
		memcpy(d26_ob_record + key.k_part[1].kp_start,nam_field,
		  key.k_part[1].kp_leng);
		stint(0,d26_ob_record + key.k_part[2].kp_start);
		stlong(mk,d26_ob_record + key.k_part[3].kp_start);
		stlong(rec_nr,d26_ob_record + key.k_part[4].kp_start);
	      }
	      else
	      {         /*  key on master-knowledge attribute only    */
		stint(0,d26_ob_record + key.k_part[1].kp_start);
		stlong(mk,d26_ob_record + key.k_part[2].kp_start);
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
      if                /*  another DB access block is full           */
	(d26_dbacc_count > nr_dbacc_in_blocks + D26_DBACC_BLOCK)
      {                 /*  read distributed command                  */
	d26_m11_distcmd(SIGUSR1);
	nr_dbacc_in_blocks += D26_DBACC_BLOCK;
      }
    }                   
    d26_ofildes[dninfo->d26_filenr].d26_in_use--;
  }                     
}                       

D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count, ir_count);

}                       /*  d26_i46_referrals_from_file               */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i58_get_accp_info                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        15.03.88                                              */
/*                                                                    */
/* DESCRIPTION: This function reads from a record		      */
/*		- the PSAP address and its length		      */
/*		- the supported authentication mechanisms	      */
/*		- the principal name of the DSA.		      */
/*		Space for the principal name is allocated if it is    */
/*		present.					      */
/*                                                                    */
/* RETURNVALUE:     None                                              */
/*                                                                    */
/*exoff ***************************************************************/

void d26_i58_get_accp_info(
  signed16 filenr, 		/* IN  - number of object file		*/
  char     *record,		/* IN  - record to read data from	*/
  D2_str   *psap_addr,		/* OUT - PSAP address			*/
  boolean  *auth_mechs,		/* OUT - authentication mechanisms	*/
  char	    **princ_name)	/* OUT - principal name			*/

{                               /*  d26_i58_get_accp_info          */

  static String function = "i58";
  D26_at   *at_ptr;		/* pointer to AT-entry		      */
  signed16 pos;                 /*  position of Attribute in record   */
  char     *r_pos;		/*  pointer to record		      */
  signed16 count;		/*  count of values		      */
  signed16 len;                 /*  length of PSAP address            */
  signed16 i;
  signed32 ob_key;          /* isam key of record                     */
  int      isfd;            /* Isam file descriptor                   */
  int      mode;            /* ISAM read mode                         */
  D26_keydesc key;          /* ISAM key structure                     */

/*** get PSAP address ***/
at_ptr = d26_at + d26_psap_idx;
pos = at_ptr->d26a_field_pos[filenr];

if                              /*  PSAP-Address is valid             */
  (ldint(record + pos) > 0)
{                               /*  read length of attribute          */
  len = ldint(record + pos + INTSIZE + at_ptr->d26a_ubound);
  if                            /*  memory for PSAP-address can be    */
				/*  allocated                         */
    ((psap_addr->d2_value = (byte *)D27_MALLOC(svc_c_sev_warning,len))
     != NULL)
  {                             /*  copy PSAP-Address into memory     */
    psap_addr->d2_size = d26_is14_ld_psap_addr((char *)psap_addr->d2_value,
					       record + pos + INTSIZE,len);
  }
  else
  {                     /*  reset size and return                     */
    psap_addr->d2_size = 0;
    return;
  }                             
}                               

/*** get auth. mechs ***/
at_ptr = d26_at + d26_am_idx;
pos = at_ptr->d26a_field_pos[filenr];

/* initialize array */
for (i = 0; i < D2_MAX_AUTH_MECHS; i++)
  auth_mechs[i] = FALSE;

if                              /*  Auth.-mech is valid               */
  ((count = ldint(record + pos)) > 0)
{ 
  auth_mechs[0] = TRUE;		/* D2_ANONYMOUS is always true 	      */
  for (i=0, r_pos = record + pos + INTSIZE; i < count; i++, r_pos += LONGSIZE)
  {
    auth_mechs[ldlong(r_pos)] = TRUE;
  }
}

/*** get principal name ***/
at_ptr = d26_at + d26_pn_idx;
pos = at_ptr->d26a_field_pos[filenr];

  *princ_name = NULL;

if                              /*  principal name is valid           */
  (ldint(record + pos) > 0)
{                               /*  read length of attribute          */
  if (at_ptr->d26a_filenr < 0)
  {
    len = strlen(record + pos + INTSIZE);
    if                            /*  memory can be allocated           */
     ((*princ_name = (char *) D27_MALLOC(svc_c_sev_warning, len+1)) 
       != NULL)
    {
      strcpy(*princ_name, record + pos + INTSIZE);
    }
  }
  else			/*  read principal name from file	      */
  {
    if                              /*  no error from open file       */
      ((isfd = d26_i23_open_file(at_ptr->d26a_filenr,D26_ATT)) >= 0)
    { 
      d26_i55_sel_rec_key(&key,at_ptr,D2_EQUAL,D23_READ);
			/*  fill key in record                        */
      ob_key = ldlong(record);
      stint(filenr, d26_a_record);
      stlong(ob_key,d26_a_record + INTSIZE);
      if                /*  attribute file is in use already          */
	(d26_afildes[at_ptr->d26a_filenr].d26_in_use > 0)
      {                 /*  set the reset flag                        */
	d26_reset = TRUE;
      }                 /*                                            */
      d26_dbacc_count++;
      if                /*  error from start query                    */
	(D26_ISSTART(svc_c_debug1,isfd,&key,INTSIZE + LONGSIZE,
	 d26_a_record,ISEQUAL) < 0)
      {                 /*  check error number                        */
	D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,1, 0);
      }
      else
      {                 /*  set query mode                            */
	mode = ISCURR + ISWAIT; 
	pos = INTSIZE + LONGSIZE;
	d26_dbacc_count++;
	if            /*  error from read isam                      */
	  (D26_ISREAD(svc_c_debug1,isfd,d26_a_record,mode) < 0)
	{             /*  check error                               */
	  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,1,1);
	  return;
	}
	else if       /*  other object number and key found         */
		 (filenr != ldint(d26_a_record)
		  || ob_key != ldlong(d26_a_record + INTSIZE))
	{             /*  return message                            */
	  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,1,1);
	  return;
	}
	else if       /*  no error from reading attribute value     */
	 ((len = strlen(d26_a_record + pos)) > 0)
	{             
	  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,1,1);
          if          
           ((*princ_name = (char *) D27_MALLOC(svc_c_sev_warning, len+1)) 
             != NULL)
          {
            strcpy(*princ_name, d26_a_record + pos);
          }
	}
      } /* isstart ok */
    }
  }
}

}                               /*  d26_i58_get_accp_info          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i59_m_obj_in_file                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        31.07.91                                              */
/*                                                                    */
/* DESCRIPTION: This function searches a given file for first level   */
/*              objects with a given object number, which are mastered*/
/*              by the current DSA.                                   */
/*                                                                    */
/* INPUT-PARAMETERS: None                                             */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_TRUE     Bool    mastered object exists                    */
/*      D2_FALSE    Bool    mastered object does not exist            */
/*                                                                    */
/*exoff ***************************************************************/

boolean d26_i59_m_obj_in_file(signed16 ob_nr, signed16 filenr)

{                                /*  d26_i59_m_obj_in_file            */

			    /* function identifier for traces         */
  static char *function = "d26_i59_m_obj_in_file";

  D26_keydesc key;          /* ISAM key structure                     */
  int         isfd;         /* ISAM file descriptor                   */

			    /* counts calls of isstart                */
  signed32    is_count = 0;

			    /* return value                           */
  boolean     ret_value = FALSE;

  if                        /* no error from open file                */
    ((isfd = d26_i23_open_file(filenr,D26_OBJ)) >= 0)
  {                         /* select key                             */
    d26_i32_select_key(&key,0,(D26_at *)NULL,0,(D2_a_match)D2_EQUAL,filenr);
			    /* put object number into key record      */
    stint(ob_nr,d26_ob_record + key.k_part[0].kp_start);
			    /* put aliased object invalid into key    */
			    /* record.                                */
    stint(0,d26_ob_record + key.k_part[1].kp_start);
			    /* put Master knowledge "own name" into   */
			    /* key record                             */
    stlong((long)D26_OWNNAME,d26_ob_record + key.k_part[2].kp_start);
    is_count++;
    d26_dbacc_count++;
    if                      /* error from start query                 */
      (D26_ISSTART(svc_c_route_nolog,isfd,&key,INTSIZE + INTSIZE + LONGSIZE,
       d26_ob_record,ISEQUAL) < 0)
    {                       /* check error number                     */
      if                    /* serious error                          */
	(iserrno != ENOREC)
      {                     /* write error log                        */
	dce_svc_printf(DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	  svc_c_sev_warning,GDS_S_ISSTART_ERR,iserrno);
      }
    }
    else
    {                       /* reset return value                     */
      ret_value = TRUE;
    }
  }

  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,is_count, 0);

  return(ret_value);

}                       /*  d26_i59_m_obj_in_file                     */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i61_make_ch_referrals                             */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        18.04.88                                              */
/*                                                                    */
/* DESCRIPTION: This function searches the descendants of the base    */
/*              object of the given object number and creates         */
/*              eventually referrals into memory.                     */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              dninfo      D26_dninfo *    information about base    */
/*                                          object.                   */
/*              np_count    Short           count of nameparts of     */
/*                                          base object               */
/*              dname       D26_name_string DN of base object         */
/*              psap_at     D26_at *        pointer to AT-entry of    */
/*                                          PSAP-Address              */
/*              shb_at      D26_at *        pointer to AT-entry of    */
/*                                          Shadowed-by-attribute     */
/*              ob_nr       Short           object number             */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_i61_make_ch_referrals(
    D26_dninfo *dninfo,
    signed16 np_count,
    D2_name_string dname,
    D26_at *psap_at,
    signed16 ob_nr)

{                               /*  d26_i61_make_ch_referrals         */

  D26_srt     *srt;             /* pointer to SRT entry               */
  D26_rdninfo *nam_rdn;         /* pointer to last RDN                */
  D26_srt     *nam_srt;         /* pointer to SRT entry of base object*/
  D26_at      *at;              /* pointer to AT entry of attribute to*/
				/* use for index                      */

  register    D26_ocl_arc *ocl_arc; /* loop pointer to object class   */
				    /* arcs                           */

if                                  /*  base object is not root       */
  (np_count > 0)
{                                   /*  assign pointers describing    */
				    /*  last RDN                      */
  nam_rdn = dninfo->d26_rdn + np_count - 1;
  nam_srt = d26_srt + nam_rdn->d26_r_idx;
  at      = d26_at + nam_rdn->d26_r_ava->d26_a_idx;
}
else
{                                   /*  set invalid AT pointer        */
  at      = (D26_at *)NULL;
}                                   

for    				    /*  all object files              */
   (srt = d26_srt + ob_nr, dninfo->d26_filenr = 0;
    dninfo->d26_filenr < d26_nofile && d26_abandoned == FALSE &&
    d26_dbop_flag != D26_DB_NOOP && d26_time_ex == FALSE;
    dninfo->d26_filenr++)
{                                   /*  check whether object file     */
				    /*  contains objects whith this   */
				    /*  structure rule                */
  for                               /*  all assigned object class arcs*/
     (ocl_arc = srt->d26s_ocl_arc;
      ocl_arc->d26s_oclass != -1 && d26_abandoned == FALSE &&
      d26_dbop_flag != D26_DB_NOOP && d26_time_ex == FALSE;
      ocl_arc++)
  {                                 /*  check whether file contains   */
				    /*  objects of this structure     */
    if                              /*  file contains objects of this */
				    /*  structure                     */
      (dninfo->d26_filenr == d26_oct[ocl_arc->d26s_oclass].d26c_filenr)
    {                               /*  search for referrals in file  */
      d26_i46_referrals_from_file(dninfo,np_count,dname,nam_srt,nam_rdn,at,
	psap_at,ob_nr);
      break;
    }                     
  }                       
}                         

}                       /*  d26_i61_make_ch_referrals                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i67_make_st_referrals                             */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        18.04.88                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the referrals belonging to the   */
/*              subtree of a base object in one level below the       */
/*              ob_nr. It calls itself recursively then.              */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              psap_at     D26_at *        pointer to AT-entry of    */
/*                                          PSAP-Address              */
/*              shb_at      D26_at *        pointer to AT-entry of    */
/*                                          Shadowed-by-attribute     */
/*              subset      Short           indicates subset to search*/
/*              dname       D26_name_string DN of base object         */
/*              dninfo      D26_dninfo *    information about object  */
/*                                          found                     */
/*              ob_nr       Short           object number for root of */
/*                                          subtree                   */
/*              mk          Long            index for master knowledge*/
/*                                          in DN list for the object */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*exoff ***************************************************************/

void d26_i67_make_st_referrals(
    D26_at *psap_at,
    signed16 subset,
    D2_name_string dname,
    D26_dninfo *dninfo,
    signed16 ob_nr,
    signed32 mk)

{                               /*  d26_i67_make_st_referrals         */

  signed16 np_count;            /* count of nameparts in DN           */

  register signed16 i;          /* loop variable                      */

np_count = dninfo->d26_rd_anz;

for                                 /*  all srt entries               */
   (i = 0;
    i < d26_nosrt && d26_abandoned == FALSE &&
    d26_dbop_flag != D26_DB_NOOP && d26_time_ex == FALSE;
    i++)
{                                   /*  check whether child nr found  */
  if                                /*  child nr found                */
    (d26_srt[i].d26s_par_nr == ob_nr)
  {                                 /*  check whether referrals needed*/
    d26_u67_trace_name_struct(d26_srt + i);
    d26_i61_make_ch_referrals(dninfo,np_count,dname,psap_at,i);
    dninfo->d26_rd_anz = np_count;
    if                  /*  subtree is searched                       */
      (subset == D2_WHOLE_SUBTREE)
    {                   /*  make subtree-referrals for new subtree    */
      d26_i67_make_st_referrals(psap_at,subset,dname,dninfo,i,
	D26_IDX_NOT_DEF);
    }                   
  }                               
}                                 

}                       /*  d26_i67_make_st_referrals                 */
