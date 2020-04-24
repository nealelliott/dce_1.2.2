/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26util.c,v $
 * Revision 1.1.732.2  1996/02/18  18:15:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:37:39  marty]
 *
 * 	Update OSF copyright years
 *
 * Revision 1.1.732.1  1995/12/08  15:49:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:17  root]
 * 
 * Revision 1.1.730.5  1994/10/05  15:39:21  keutel
 * 	OT 12481: multiple routing
 * 	[1994/10/05  14:37:38  keutel]
 * 
 * Revision 1.1.730.4  1994/06/21  14:46:05  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:54:24  marrek]
 * 
 * Revision 1.1.730.3  1994/05/10  15:52:57  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:26:13  marrek]
 * 
 * Revision 1.1.730.2  1994/03/23  15:13:37  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:23:29  keutel]
 * 
 * Revision 1.1.730.1  1994/02/22  18:40:13  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:21:37  marrek]
 * 
 * Revision 1.1.728.3  1993/10/29  15:11:21  keutel
 * 	CR 9261
 * 	[1993/10/29  14:06:34  keutel]
 * 
 * Revision 1.1.728.2  1993/10/14  17:16:47  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:13:05  keutel]
 * 
 * Revision 1.1.728.1  1993/10/13  17:31:19  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:29:46  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  15:38:50  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:21:52  marrek]
 * 
 * Revision 1.1.4.7  1993/02/02  14:55:26  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.6  1993/02/02  11:31:03  marrek
 * 	Introduce D2_IA5
 * 	[1993/02/01  15:22:13  marrek]
 * 
 * Revision 1.1.4.5  1992/12/31  19:43:04  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:21:39  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/02  10:27:16  marrek
 * 	November 1992 code drop
 * 	Changes for case sensitive printable strings
 * 	[1992/12/02  10:20:24  marrek]
 * 
 * Revision 1.1.4.3  1992/11/27  20:25:34  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:19:41  marrek]
 * 
 * Revision 1.1.4.2  1992/09/23  11:34:13  marrek
 * 	Initialize *dnp = D2_TYPE_VAL_SEP; before return.
 * 	[1992/09/23  11:32:36  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:53:30  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:58:46  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26util.c,v $ $Revision: 1.1.732.2 $ $Date: 1996/02/18 18:15:07 $";
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
/* NAME         : d26util.c <Utility-Functions for DSA-Interfaces>    */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 08.12.87                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    |08.12.87 |  Original                      | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

/*****  internal Includes  *****/

#include <d26dsa.h>

/*****  Operating System - Includes  *****/

#include <errno.h>
#include <string.h>
#include <time.h>

/*****  external Includes  *****/

#include <dce/d27util.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

/* constants for norming numbers                                      */

#define D26_DIGIT    0  /* indicates digit-segment in string          */
#define D26_NONDIGIT 1  /* indicates nondigit-segment in string       */

				/* Upper bound for ...                */
#define D26_UB_POSTAL_STRING 30 /* ... postal address component       */

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

				    /* Installation-Directory         */
char        d26_install[D27_LINST_NAME];
				    /* path name for logfiles         */
char        d26_logdir[D27_LINST_NAME];
				    /* name of chdb for call          */
char        d26_chdb_name[D27_LFILE_NAME];

D26_srt     *d26_srt;               /* Structure rule table           */
D26_nam_at  *d26_nam_at;            /* Naming attribute array         */
D26_ocl_arc *d26_ocl_arc;           /* Object class arcs array        */
D26_oct     *d26_oct;               /* Object Class table             */
D26_at      *d26_at;                /* Attribute-Table                */
signed16    *d26_name_pos;          /* array of positions of naming   */
				    /* attrs in each object file.     */
signed16    *d26_at_pos;            /* array of positions of other    */
				    /* attrs in each object file.     */
byte        *d26_obid_str;          /* string which holds all octal   */
				    /* object ids for classes + attrs.*/
signed16    *d26_oct_idx;           /* String of indices to attrs in  */
				    /* the AT and classes in the OCT. */

signed32    d26_nosrt;              /* count of structure rules       */
signed32    d26_nooct;              /* count of object classes        */
signed32    d26_noat;               /* count of attributes in table   */
signed32    d26_nofile;             /* count of object files          */
signed32    d26_nafile;             /* count of attribute files       */

D26_attr_info *d26_attr;            /* field of attribute informations*/

boolean     *d26_ate_used;          /* indicates for each AT entry    */
				    /* whether it is in the message   */
				    /* already                        */

				    /* AT-index of ...                */
signed16    d26_obcl_idx;           /* ... object class attribute     */
signed16    d26_alob_idx;           /* ... aliased object attribute   */
signed16    d26_mk_idx;             /* ... master knowledge attribute */
signed16    d26_acl_idx;            /* ... ACL-attribute              */
signed16    d26_pwd_idx;            /* ... User-Password-attribute    */
signed16    d26_psap_idx;           /* ... PSAP-attribute             */
signed16    d26_dnl_idx;            /* ... DN-List-attribute          */
signed16    d26_am_idx;             /* ... Auth.-Method-attribute     */
signed16    d26_pn_idx;             /* ... Principal-Name-attribute   */

				    /* OCT-index of ...               */
signed16    d26_ali_idx;            /* ... object class alias         */
signed16    d26_dsa_idx;            /* ... object class DSA           */

				    /* Distinguished-Name-list in core*/
D26_dndesc  *d26_dnlist = NULL;

				    /* maximum index of DN list in    */
				    /* core                           */
signed32    d26_maxidx_dnlist = -1;

				    /* Requestor name of directory    */
				    /* caller (normed)                */
D26_requestor d26_requestor;

				    /* name of base object for        */
				    /* internal search                */
D2_name_string d26_base_object;
				    /* undecoded part of base object  */
D23_av_value *d26_bo_encoded_part;
				    /* base object contained in DAP-  */
				    /* message                        */
D2_name_string d26_dap_b_object;
				    /* undecoded part of DAP base obj.*/
D23_av_value *d26_dap_bo_ep;

/* global variables for delta-updates */
				    /* actual object name (normed)    */
byte        d26_objname[D2_DNL_MAX+1];
				    /* master knowledge of superior's */
				    /* entry in case of update functs.*/
				    /* set by add_object or logg_mesg.*/
signed32    d26_sup_mk = D26_IDX_NOT_DEF;
				    /* file pointer of SHDLOG file    */
				    /* == D2_NULL: file closed        */
				    /* != D2_NULL: file opened        */
FILE        *d26_fpshdlog = (FILE *) NULL;
				    /* indicates permitted database   */
				    /* operations                     */
signed16    d26_dbop_flag = D26_DB_RW;

				    /* indicator whether DSA has to be*/
				    /* restarted after saving         */
boolean     d26_restart = FALSE;

boolean     d26_ma_info;            /* needed for logging, whether    */
				    /* ma-info is changed or not      */
				    /* time of last change of DN list */
D26_update_info d26_upd_info = { 0L, '0' };

time_t      d26_ftime = 0L;         /* time for last access to files  */
				    /* (is set by d26_u09_initdsa)    */


				    /* own DSA-Name (normed)          */
byte        d26_myname[D2_DNL_MAX + 1];
				    /* own DSA-Name (case sensitive)  */
byte        d26_myexact_name[D2_DNL_MAX + 1];
				    /* schema object name (normed)    */
byte        d26_schema_name[D2_DNL_MAX + 1];
				    /* my principal name.             */
char        d26_mypr_name[D2_DNL_MAX + 1];

signed32    d26_dbacc_count;         /* count of DB accesses           */
signed32    d26_entry_count;        /* count of entries written       */
boolean     d26_ref_present;        /* indicates that referrrals are  */
				    /* present                        */
void        *d26_ref_root;          /* root of referral tree          */
				    /* pointer to array of referrals  */
D26_referral d26_referral[D26_REF_BLOCKSIZE];
signed32    d26_ref_count;          /* count of referrals written     */
byte        *d26_start_p_o_q;       /* address of partial outcome     */
				    /* qualifier in message           */
signed32    d26_p_o_q_length;       /* total length of partial outcome*/
				    /* qualifier                      */
D23_serv_ctrl  *d26_serv_contr;     /* service controls               */
signed16    d26_al_rdns;            /* aliased RDNs                   */
D23_op_pr_info *d26_op_progress;    /* operation progress             */
D23_chain_arg  *d26_chain_arg;      /* chaining arguments             */
D23_srcharg *d26_search_arg;        /* search   arguments             */
D2_subset   d26_subset;             /* subset used for search         */
signed32    d26_time_limit;         /* time limit in seconds          */
boolean     d26_time_ex;            /* time limit exceeded            */
boolean     d26_abandoned;          /* indicates abandoned request    */

				    /* list of master PSAP-Addresses  */
D26_acc_point *d26_accp_list = (D26_acc_point *) NULL;
signed32    d26_accp_count = 0L;    /* count of written access points */
signed32    d26_accp_max = 0L;      /* maximum number of elements that*/
				    /* access point list can hold     */

signed32    d26_bind_count = 1;     /* count of bind table entries    */

signed16    d26_dir_id = -1;        /* own directory id               */
				    /* indicates whether DSA has to   */
				    /* update cache                   */
boolean     d26_upd_cache = TRUE;
				    /* indicates whether DSA has to   */
				    /* log update operations          */
boolean     d26_ulogging = TRUE;
				    /* indicates that ISAM index must */
				    /* be reset                       */
boolean     d26_reset = FALSE;

			/* indicates chaining policy of DSA           */
signed16    d26_chaining_policy = D26_MULTI_HOP;
			/* indicates routing facility of the DSA      */
boolean     d26_routing         = FALSE;
			/* indicators for routing priorities          */
signed16    d26_knowledge_prio  = 0;
signed16    d26_routing_prio    = 0;
		 /* indicates DSP unbind delay                 */
time_t      d26_dspunbind_delay = 0L;
		 /* number of routing DSAs                     */
signed16    d26_nr_routing_dsas;
		 /* array of routing access points             */
D26_acc_point *d26_routing_access_points;

boolean     d26_auth_mech[D2_MAX_AUTH_MECHS];
				/* authentication mechanisms supported    */
				/* by the DSA.                            */
				/* automatically initialised to FASLE (0) */

static signed16 d26_u01_rdn_search(D26_rdninfo *, signed16 par_idx);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u01_rdn_search                                  */
/*                                                                    */
/* DESCRIPTION  : This function will look whether the RDN is known in */
/*                SRT.                                                */
/*                The naming attributes in one SRT entry are sorted.  */
/*                (criterion: attribute id)                           */
/*                The srt is sorted in increasing order.              */
/*                (criterion: attribute ids of the RDNs               */
/*                 lexicographically)                                 */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      >= 0        signed16        index of ruling SRT entry         */
/*      D26_CONCATENATION           matching SRT entry exists, but    */
/*                                  not subordinate to par_idx        */
/*      D26_ERROR                   any other error occurred          */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_u01_rdn_search(D26_rdninfo *rdn, signed16 par_idx)

{                                   /*  d26_u01_rdn_search()          */

  signed16 ret_value = D26_ERROR;   /* return value                   */

  register signed16   i;            /* loop variable                  */
  register D26_srt    *srt, *max_srt;   /* loop pointers to SRT       */
  register D26_nam_at *nam_at;      /* loop pointer to naming         */
				    /* attribute structure in SRT     */
  register D26_ava    *ava;         /* loop pointer to naming AVA in  */
				    /* RDN                            */

  for                               /* all entries in srt             */
     (srt = d26_srt, max_srt = srt + d26_nosrt; srt < max_srt; srt++)
  {                                 /* scan the naming attributes     */
    for                             /* all naming attributes of the   */
				    /* SRT entry                      */
       (i = 0, ava = rdn->d26_r_ava, nam_at = srt->d26s_nam_at;
	i < rdn->d26_r_nava; i++, ava++, nam_at++)
    {                               /* check for end of list in SRT   */
				    /* entry                          */
      if                            /* end of list is reached         */
	(nam_at->d26s_index == -1)
      {                             /* SRT "smaller" than RDN,        */
				    /* continue with next SRT entry   */
	break;
      }
      else if                       /* RDN "smaller" than SRT         */
	     (ava->d26_a_idx < nam_at->d26s_index)
      {                             /* return (D26_ERROR)             */
	return(ret_value);
      }
      else if                       /* SRT "smaller" than RDN,        */
	     (ava->d26_a_idx > nam_at->d26s_index)
      {                             /* continue with next SRT entry   */
	break;
      }
    }

    if                              /* SRT entry was found, which     */
				    /* shares the naming attributes   */
      (i == rdn->d26_r_nava)
    {                               /* check whether it has still     */
				    /* more types                     */
      if                            /* still types following in SRT   */
	(nam_at->d26s_index != -1)
      {                             /* RDN "smaller" than SRT entry   */
				    /* return error                   */
	return(ret_value);
      }
      else
      {                             /* reset return value             */
	ret_value = D26_CONCATENATION;
	if                          /* parent index found             */
	   (par_idx == srt->d26s_par_nr)
	{                           /* return (object index)          */
	  return((signed16)(srt - d26_srt));
	}
      }
    }
  }

				    /* return (D26_ERROR)             */
  return(ret_value);

}                                   /*  d26_u01_rdn_search()          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u03_attr_search                                 */
/*                                                                    */
/* DESCRIPTION  : This function will look for the given attribute.    */
/*                The at is sorted in increasing order.               */
/*                (criterion: attribute-id)                           */
/*                A binary search will be used.                       */
/*                                                                    */
/* RETURN VALUE :   >= 0:           index of attribute in AT          */
/*                  D26_ERROR:      attribute not found               */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u03_attr_search(
    D2_a_type *attr_typ)/* IN    -  Attribute type to search for      */

{                                   /*  d26_u03_attr_search()         */

register signed32 cmp;              /* indicates which attribute type */
				    /* is smaller                     */
register signed16 high;             /* right branch                   */
register signed16 low;              /* left branch                    */
register signed16 mid;              /* middle of interval             */
register D26_at   *at;              /* pointer to attribute table     */

                                    /*  determine high and low value  */
				    /*  of interval to be searched    */
low = 0;
high = d26_noat - 1;

while                               /*  interval not empty            */
      (low <= high)
{                                   /*  take middle of interval       */
  mid = (low + high) / 2;
  at = d26_at + mid;
  if                                /*  attribute type < type in AT   */
    ((cmp = d27_303_cmp_obj_id((D2_obj_id *)attr_typ,
     (D2_obj_id *)&at->d26a_type)) < 0)
  {                                 /*  high value = middle - 1       */
    high = mid - 1;
  }
  else if                           /*  attr-Id > actual value        */
	 (cmp > 0)
  {                                 /*  low value = middle + 1        */
    low = mid + 1;
  }
  else
  {                                 /*  return(attr-index)            */
    return(mid);
  }                                 /*                                */
}                                   /*                                */

				    /*  return (D26_ERROR)            */
return (D26_ERROR);

}                                   /*  d26_u03_attr_search()         */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u06_get_name                                    */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function verifies a given distinguished name for validity.*/
/*      This means, all distinguished name parts must be T61-strings, */
/*      all attribute identifiers must be contained in the AT     and */
/*      the distinguished name must meet the form:                    */
/*                                                                    */
/*      <rdn1>\1<rdn2>\1...<rdn<n>>\0                                 */
/*                                                                    */
/*      The RDNs must meet the form:                                  */
/*                                                                    */
/*      <ava1>\2<ava2>\2...<ava<m>>                                   */
/*                                                                    */
/*      The AVAs must meet the form:                                  */
/*                                                                    */
/*      <type>\3<value>                                               */
/*                                                                    */
/*      All attribute types are checked until the sequence of RDNs is */
/*      hurt. In this case D26_CONCATENATION will be returned (because*/
/*      the Distinguished Name may be an alias; the application has to*/
/*      manage the rest of the string).                               */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR           Short       no error occurred             */
/*      D2_NMSYNTAX_ERROR               name syntax error             */
/*      D26_CONCATENATION               DN may be an alias name       */
/*                                      (name is partly checked)      */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u06_get_name(
    D2_name_string dn,  /* IN    -  Distinguished name to prepare     */
    D26_dninfo *dninfo) /* OUT   -  structured information about DN   */

{                                   /*  d26_u06_get_name              */

				    /* memory for converted object    */
				    /* identifiers                    */
static   byte           ob_ids[D2_NO_AVA][D2_OBJ_IDL_MAX];
				    /* memory for naming attr. types  */
static   D2_a_type      nam_type[] =
			    {
			      {0,(Ob_id_string)ob_ids[0]},
			      {0,(Ob_id_string)ob_ids[1]},
			      {0,(Ob_id_string)ob_ids[2]}
			    };

D2_name_string comp;       /* pointer to components of DN    */
D26_ava        *ava;       /* pointer to AVA in RDN          */
char           separator;  /* separator between type and     */
				/* value                          */
signed16       len;        /* length of a component          */
signed16       rep;        /* representation of name part    */

D26_rdninfo    *rdninfo;   /* pointer to RDN                 */
signed16       par_idx;    /* SRT index of parent of RDN     */
D26_at         *at;        /* pointer to AT entry            */

register D2_name_string dnp;        /* loop pointer to DN             */

if                      /*  object is root                            */
  (*dn == D2_EOS)
{                       /*  return no error                           */
  return(D2_NO_ERR);
}                       /*                                            */

par_idx = dninfo->d26_rd_anz == 0 ?
	  D26_ROOT : dninfo->d26_rdn[dninfo->d26_rd_anz - 1].d26_r_idx;

for                     /*  all characters of the name                */
   (dnp = comp = dn, rdninfo = dninfo->d26_rdn + dninfo->d26_rd_anz,
    rdninfo->d26_r_nava = 0, ava = rdninfo->d26_r_ava, ava->d26_a_idx = -1;
    ; dnp++)
{                       /*  check if end of attribute type is reached */
  if                    /*  end of attribute type is reached          */
    (*dnp == D2_T61_REP || *dnp == D2_PRTBL_REP || *dnp == D2_NUM_REP ||
     *dnp == D2_IA5_REP)
  {                     /*  check whether object identifier has       */
			/*  illegal length                            */
    len = dnp - comp;
    if                  /*  length is illegal                         */
      (len == 0 || len > D2_NP_ID_LEN)
    {                   /*  return syntax error                       */
      return(D2_NMSYNTAX_ERROR);
    }
    else
    {                   /*  replace delimiter by EOS                  */
      separator = *dnp;
      *dnp = D2_EOS;
      if                /*  object identifier cannot be converted     */
	(d27_302_str_oid((char *)comp,nam_type + rdninfo->d26_r_nava) < 0)
      {                 /*  return name syntax error                  */
	*dnp = separator;
	return(D2_NMSYNTAX_ERROR);
      }
      else if           /*  attribute type is not in AT               */
	     ((ava->d26_a_idx = d26_u03_attr_search(nam_type +
	      rdninfo->d26_r_nava)) < 0)
      {                 /*  return syntax error                       */
	*dnp = separator;
	return(D2_NMSYNTAX_ERROR);
      }                 /*                                            */
      *dnp = separator;
      comp = dnp + 1;
    }                   /*                                            */
  }                     /*                                            */
  if                    /* end of AVA is reached                      */
    (*dnp == D2_AVA_SEP || *dnp == D2_RDN_SEP || *dnp == D2_EOS)
  {                     /* set length and value in AVA                */
    if                  /*  still attribute type not complete         */
      (ava->d26_a_idx == -1)
    {                   /*  return error                              */
      return(D2_NMSYNTAX_ERROR);
    }                   /*                                            */
    ava->d26_a_len = dnp - comp;
    ava->d26_a_val = comp;
    at  = d26_at + ava->d26_a_idx;
    switch(separator)
    {
      case D2_T61_REP:
	rep = D2_T61;
	break;
      case D2_PRTBL_REP:
	rep = D2_PRINTABLE;
	break;
      case D2_NUM_REP:
	rep = D2_NUMERIC;
	break;
      case D2_IA5_REP:
	rep = D2_IA5;
	break;
    }   
    if                  /*  attribute value has illegal length or     */
			/*  syntax                                    */
      (ava->d26_a_len < at->d26a_lbound ||
       ava->d26_a_len > at->d26a_ubound ||
       d26_u10_syntax_check(at,rep,ava->d26_a_val,ava->d26_a_len)
       !=  D2_NO_ERR)
    {                   /*  return error                              */
      return(D2_NMSYNTAX_ERROR);
    }                   /*                                            */
    rdninfo->d26_r_nava++;
    if                  /*  still AVA following                       */
      (*dnp == D2_AVA_SEP)
    {                   /*  check the number of AVAs                  */
      if                /*  number of AVAs exceeds limit              */
	(rdninfo->d26_r_nava >= D2_NO_AVA)
      {                 /*  return syntax error                       */
	return(D2_NMSYNTAX_ERROR);
      }                 /*  reset the component pointer               */
      ava++;
      ava->d26_a_idx = -1;
    }                   /*                                            */
    if                  /*  end of RDN is reached                     */
      (*dnp == D2_RDN_SEP || *dnp == D2_EOS)
    {                   /*  check the concatenation                   */
      if                /*  no ruling structure rule found            */
	((rdninfo->d26_r_idx = d26_u01_rdn_search(rdninfo,par_idx)) < 0)
      {                 /*  return error                              */
	return(rdninfo->d26_r_idx == D26_CONCATENATION ?
	       D26_CONCATENATION : D2_NMSYNTAX_ERROR);
      }
      else
      {                 /*  reset the parent index                    */
	par_idx = rdninfo->d26_r_idx;
      }                 /*                                            */
      dninfo->d26_rd_anz++;
      if                /*  still RDN following                       */
	(*dnp == D2_RDN_SEP)
      {                 /*  check the number of RDNs                  */
	if              /*  number of RDNs exceeds limit              */
	  (dninfo->d26_rd_anz >= D2_NP_MAX)
	{               /*  return syntax error                       */
	  return(D2_NMSYNTAX_ERROR);
	}               /*  reset the component pointer               */
	rdninfo++;
	ava = rdninfo->d26_r_ava;
	ava->d26_a_idx = -1;
	rdninfo->d26_r_nava = 0;
      }
      else
      {                 /*  end of name is reached, break the loop    */
	break;
      }                 /*                                            */
    }                   /*                                            */
    comp = dnp + 1;
  }                     /*                                            */
}                       /*                                            */

len = dnp - dn;

if                      /*  length of DN is illegal                   */
  (len == 0 || len > D2_DNL_MAX)
{                       /*  return error                              */
  return(D2_NMSYNTAX_ERROR);
}                       /*                                            */

return(D2_NO_ERR);

}                                   /*  d26_u06_get_name              */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_u10_syntax_check                                */
/*                                                                    */
/* DESCRIPTION  : This function will look whether the syntax of the   */
/*                given attribute is correct.                         */
/*                                                                    */
/* RETURN VALUE :   D2_NO_ERR:      attribute syntax is correct       */
/*                  D26_ERROR:      attribute syntax is not correct   */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u10_syntax_check(
    D26_at *at,         /* IN    -  AT entry of attribute value       */
    unsigned16 rep,     /* IN    -  representation of attribute value */
    byte *val,          /* IN    -  attribute value                   */
    signed16 length)    /* IN    -  length of attribute value         */

{                                   /*  d26_u10_syntax_check          */

signed16        i;
D26_dninfo      dn_info;

register byte *max_adr = val + length;

switch(at->d26a_syntax)        /*  syntax is                     */
{
  case D2_DISTNAME:                 /*  Distinguished Name Syntax     */
    if                              /*  length is wrong               */
      (length != strlen((char *)val) + 1)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    dn_info.d26_rd_anz = 0;
    if                          /*  Distinguished Name is not correct */
       (d26_u06_get_name(val,&dn_info))
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_CEXACT:                   /*  Case Exact String             */
  case D2_CIGNORE:                  /*  Case Ignore String            */
    if                              /*  string is claimed as printable*/
      (rep == D2_PRINTABLE)
    {                               /*  check for printable characters*/
      if                            /*  If not Printable String       */
	(d27_605_is_PrtString (val, length) == D2_ERROR)
      {                             /*  return(D26_ERROR)             */
	return(D26_ERROR);
      }                             /*                                */
    }
    else
    {                               /*  check for T61 characters      */
      if                            /*  If not T61String              */
	(d27_601_is_T61String(val,length) == D2_ERROR)
      {                             /*  return(D26_ERROR)             */
	return(D26_ERROR);
      }                             /*                                */
    }                               /*                                */
    break;
  case D2_IA5:                      /*  IA5-String                    */
    if                              /*  If not IA5 String             */
      (d27_606_is_IA5String(val,length) == D2_ERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_TELNR_STX:                /*  Telephone-Number              */
  case D2_PRINTABLE:                /*  Printable String              */
    if                              /*  If not Printable String       */
      (d27_605_is_PrtString (val, length) == D2_ERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_NUMERIC:                  /*  Numeric String                */
    if                              /*  If not Numeric String         */
      (d26_us6_is_NumString (val, length) == D2_ERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_TIME:                     /*  UTC-Time-Syntax               */
    if                              /*  If not Universal Time         */
      (d26_us7_is_UTCtime (val) == D2_ERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_COUNTRY_STX:              /*  Country-Syntax                */
    if                              /*  length != 2                   */
       (length != 2)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }
    else
    {                               /*  check country syntax          */
      if                            /*  If not country-syntax         */
	(d27_608_is_country (val) == D2_ERROR)
      {                             /*  return(D26_ERROR)             */
	return(D26_ERROR);
      }                             /*                                */
    }                                /*                                */
    break;
  case D2_ACCL:                     /*  Access Control List           */
    for                             /*  all ACL-names                 */
       (i = 0 ; i < D2_ACLIDX_MAX && val < max_adr ; i++)
    {                               /*                                */
      if                            /*  ACL-interpretation is correct */
	 (*val == D2_SOBJ  || *val == D2_SUBTREE || *val == D2_GROUP)
      {                             /*                                */
	if                          /*  DN is root of subtree         */
	   (*val == D2_SUBTREE && *(val+1) == D2_EOS)
	{                           /*  take next DN                  */
	  val += 2;
	}
	else
	{                           /* check distinguished name       */
	  val++;
	  dn_info.d26_rd_anz = 0;
	  if                    /*  Distinguished Name is not correct */
	     (d26_u06_get_name(val, &dn_info))
	  {                         /*  return(D26_ERROR)             */
	    return(D26_ERROR);
	  }                         /*                                */
	  val += (strlen((char *)val) + 1);
	}                           /*                                */
      }
      else if                       /*  ACL-Interpretation is IGNORE  */
	     (*val == D2_IGNORE)
      {                             /*  take next ACL entry           */
	val += 2;
      }
      else
      {                             /*  return error                  */
	return(D26_ERROR);
      }                             /*                                */
    }                               /*                                */
    if                              /*  ACL-attribute is correct      */
       (i != D2_ACLIDX_MAX || val != max_adr)
    {                               /*  return error                  */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_CIGN_LIST:                /*  Case-Ignore-List-Syntax       */
    if                              /*  If not Postal address         */
      (d26_usg_is_seq_t61_prtbl(val,length,0,0) == D2_ERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_POST_AD_STX:              /*  Postal-Address-Syntax         */
    if                              /*  If not Postal address         */
      (d26_usg_is_seq_t61_prtbl(val,length,D26_UB_POSTAL_LINE,
       D26_UB_POSTAL_STRING) == D2_ERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_BOOLEAN:                  /*  Boolean Syntax                */
    if                              /*  If not 0, 1  or length != 2   */
      (length != sizeof(Bool) ||
       (*(Bool *)val != TRUE && *(Bool *)val != FALSE))
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_INTEGER  :                /*  Integer Syntax                */
      if                            /*  length not sizeof (Int)       */
	 (length != sizeof(signed32))
      {                             /*  return(D26_ERROR)             */
	return(D26_ERROR);
      }                             /*                                */
    break;
  case D2_GUIDE:                    /*  Search-Guide-Syntax           */
    break;
  case D2_TLXNR_STX:                /*  Telex-Number-Syntax           */
    if                              /*  If not telexnr-syntax         */
      (d26_usa_is_tlxnr(val,length) != D2_NOERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_TTXID_STX:                /*  Teletex-Terminal-ID-Syntax    */
    if                              /*  If not ttxid-syntax           */
      (d26_usb_is_ttxid(val,length) != D2_NOERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_FAXNR_STX:                /*  Fax-Number-Syntax             */
    if                              /*  If not faxnr-syntax           */
      (d26_usc_is_faxnr (val) != D2_NOERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_MHS_PR_DM_STX:
  case D2_PR_DM_STX:                /*  Preferred-Delivery-Method-Stx.*/
    if                              /*  If not Pref. Deliv. sntx      */
      (d26_us4_is_PreferDel (val, length) == D2_ERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_CERTIFICATE:
    if                              /*  If not certificate syntax     */
      (d26_ush_is_certificate(&val) == D2_ERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_CERT_PAIR:
    if                              /*  If not certificate pair syntax*/
      (d26_usi_is_cert_pair(&val) == D2_ERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_CERT_LIST:
    if                              /*  If not certificate list syntax*/
      (d26_usj_is_cert_list(&val) == D2_ERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_ANY_STX:                  /*  Any Syntax                    */
  case D2_OBJ_IDENT:                /*  Object Identifier             */
  case D2_OCTET:                    /*  Octet_string                  */
  case D2_PSAP_STX:                 /*  PSAP-Syntax                   */
  case D2_PASSWD_STX:               /*  Password-Syntax               */
  case D2_ASN1:                     /*  ASN1-syntax                   */
    break;
  case D2_MHS_DLSP_STX:             /*  MHS Submit Permission-Syntax  */
    if                              /*  If not Subm. Perm. sntx       */
      (d26_usf_is_sub_perm(val) == D2_ERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_MHS_ORADR_STX:            /*  MHS OR-Address-Syntax         */
    if                              /*  If not OR-address sntx        */
      (d26_usd_is_OR_addr (val, length) == D2_ERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  case D2_MHS_ORNAME_STX:           /*  MHS OR-Name-Syntax            */
    if                              /*  If not OR-name sntx           */
      (d26_use_is_OR_name(val,length) == D2_ERROR)
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
    break;
  default:                          /*  otherwise                     */
				    /*  return error                  */
    return(D26_ERROR);
}                                   /*                                */

				    /*  return (D2_NO_ERR)            */
return (D2_NO_ERR);

}                                   /*  d26_u10_syntax_check          */

/*exon ****************************************************************/
/*                                                                    */
/* NAME         : d26_u14_get_obj_class_idx                           */
/*                                                                    */
/* DESCRIPTION  : This function will look for the given object class  */
/*                in the OCT.                                         */
/*                The OCT is sorted in increasing order.              */
/*                (criterion: object class identifier)                */
/*                A binary search will be used.                       */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      >= 0        Short           index of object class in OCT      */
/*      D26_ERROR                   object class not found            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u14_get_obj_class_idx(
			/* IN    -  object class identifier to be     */
    D2_obj_id *obj_class)           /* looked for                     */

{                                   /*  d26_u14_get_obj_class_idx     */

register signed16 cmp;              /* indicates which object class   */
				    /* is smaller                     */
register signed16 high;             /* right branch                   */
register signed16 low;              /* left branch                    */
register signed16 mid;              /* middle of interval             */
register D26_oct *oct;              /* pointer to OCT                 */

                                    /*  determine high and low value  */
				    /*  of interval to be searched    */
low = 0;
high = d26_nooct - 1;
while                               /*  interval not empty            */
      (low <= high)
{                                   /*  take middle of interval       */
  mid = (low + high) / 2;
  oct = d26_oct + mid;
  if                                /*  object class < obj. class in OCT */
    ((cmp = d27_303_cmp_obj_id(obj_class,&oct->d26c_ob_id)) < 0)
  {                                 /*  high value = middle - 1       */
    high = mid - 1;
  }
  else if                           /*  object class > obj. class in OCT */
	 (cmp > 0)
  {                                 /*  low value = middle + 1        */
    low = mid + 1;
  }
  else
  {                                 /*  return(obj-class index)       */
    return(mid);
  }                                 /*                                */
}                                   /*                                */

				    /*  return error                  */
return (D26_ERROR);

}                                   /*  d26_u14_get_obj_class_idx     */

/*exon ****************************************************************/
/*                                                                    */
/* NAME         : d26_u15_attr_error                                  */
/*                                                                    */
/* DESCRIPTION  : This function will write an attribute error message.*/
/*                During the first call the header and object name    */
/*                are written, subsequent calls write just one single */
/*                attribute problem. Every call generates an EOM-tag. */
/*                                                                    */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*              char *                      pointer to EOM-tag in     */
/*                                          written message           */
/*exoff ***************************************************************/

void d26_u15_attr_error(
    byte **start_mess,  /* INOUT -  pointer to beginning of message   */
			/* IN    -  DN of resolved object             */
    D2_name_string object,
    signed16 call_nr,   /* IN    -  number of function call           */
    signed16 problem,   /* IN    -  attribute problem                 */
    D2_a_type *a_type,  /* IN    -  attribute type                    */
    unsigned16 rep_ai,  /* IN    -  representation of Av_info         */
    unsigned16 rep_av,  /* IN    -  representation of Av_value        */
    signed16 attr_len,  /* IN    -  attribute length                  */
    byte *attr_val,     /* IN    -  attribute value                   */
    signed32 *memlen,   /* INOUT -  maximum length of error message   */
    signed32 *l_err)    /* INOUT -  length of error message           */

{                                   /*  d26_u15_attr_error()          */

signed16       offset;          /* offset in message                  */
signed16       nam_len;         /* length of erroneous object         */
byte *         message;         /* pointer to message to write        */

message = *start_mess + *l_err;

if                                  /*  first call to function        */
				    /*  set error codes               */
  (call_nr == 1)
{                                   /*  write header and object name  */
  if                                /*  not enough memory available   */
    (*l_err + sizeof(D23_rserror) + sizeof(D23_eom) > *memlen)
  {                                 /*  return error                  */
    return;
  }
  else
  {                                /*  write header into message     */
    ((D23_rserror *)message)->d23_Zretcod = D2_CALL_ERR;
    ((D23_rserror *)message)->d23_Zerrcls = D2_PAR_ERR;
    ((D23_rserror *)message)->d23_Zerrval = D2_ATT_ERROR;
    ((D23_rserror *)message)->d23_Zversion = D23_V1988;

    *l_err  += sizeof(D23_rserror);
    message += sizeof(D23_rserror);
    if                              /*  enough memory available       */
      (*l_err + sizeof(D23_ds_name) + D2_ALIGN(strlen((char *)object) + 1L) +
       sizeof(D23_eom) <= *memlen)
    {                               /*  write distinguished name      */
      ((D23_ds_name *)message)->d23_dstag = D23_NAMTAG;
      ((D23_ds_name *)message)->d23_dscomplete = TRUE;
      nam_len = strlen((char *)object);
      ((D23_ds_name *)message)->d23_dslen = nam_len > 0 ? nam_len + 1 : 0;
      ((D23_ds_name *)message)->d23_dsoff = offset = D2_ALIGN(
				((D23_ds_name *)message)->d23_dslen);
      *l_err  += sizeof(D23_ds_name);
      message += sizeof(D23_ds_name);
      strcpy((char *)message,(char *)object);
      *l_err  += offset;
      message += offset;
    }
    else
    {                               /*  write end of message tag      */
      ((D23_eom *)message)->d23_eomtag    = D23_EOMTAG;
      ((D23_eom *)message)->d23_eom_oqoff = 0L;
      *l_err  += sizeof(D23_eom);
      message += sizeof(D23_eom);
      return;
    }                               /*                                */
  }                                 /*                                */
}
else
{                                   /*  reset message and length      */
  *l_err  -= sizeof(D23_eom);
  message -= sizeof(D23_eom);
}                                   /*                                */

if                                  /*  error message is not too long */
   (*l_err + sizeof(D23_at_prob) +
    sizeof(D23_at_type) + D2_ALIGN(a_type->d2_typ_len) +
    sizeof(D23_eom) <= *memlen)
{                                   /*  specify erroneous object      */
				    /*  specify attribute problem     */
  ((D23_at_prob *)message)->d23_aptag = D23_APTAG;
  ((D23_at_prob *)message)->d23_apval = problem;
  *l_err  += sizeof(D23_at_prob);
  message += sizeof(D23_at_prob);
				    /* specify attribute type         */
  ((D23_at_type *)message)->d23_atttag = D23_ATTTAG;
  ((D23_at_type *)message)->d23_atoff  = offset =
					 D2_ALIGN(a_type->d2_typ_len);
  ((D23_at_type *)message)->d23_atlen  = a_type->d2_typ_len;
  *l_err  += sizeof(D23_at_type);
  message += sizeof(D23_at_type);

  memcpy(message,a_type->d2_type,a_type->d2_typ_len);
  *l_err  += offset;
  message += offset;

  if                                /*  attribute value present and   */
				    /*  not too long                  */
    (attr_len > 0 && *l_err + sizeof(D23_av_info) + sizeof(D23_av_value) +
    D2_ALIGN(attr_len) <= *memlen)
  {                                 /*  specify attribute value info  */
    ((D23_av_info *)message)->d23_avtag  = D23_AVTAG;
    ((D23_av_info *)message)->d23_avnum  = 1;
    ((D23_av_info *)message)->d23_asyntx = rep_ai;
    *l_err  += sizeof(D23_av_info);
    message += sizeof(D23_av_info);
				    /*  specify attribute value       */
    ((D23_av_value *)message)->d23_avlen = attr_len;
    ((D23_av_value *)message)->d23_avoff = offset = D2_ALIGN(attr_len);
    ((D23_av_value *)message)->d23_avrep = rep_av;
    *l_err  += sizeof(D23_av_value);
    message += sizeof(D23_av_value);
    memcpy(message,attr_val,attr_len);
    *l_err  += offset;
    message += offset;
  }                                 /*                                */
}                                   /*                                */

((D23_eom *)message)->d23_eomtag = D23_EOMTAG;
*l_err  += sizeof(D23_eom);
message += sizeof(D23_eom);

return;

}                                   /*  d26_u15_attr_error()          */

/*exon ****************************************************************/
/*                                                                    */
/* NAME         : d26_u16_cmp_superclasses                            */
/*                                                                    */
/* DESCRIPTION  : This function compares the object identifier with   */
/*                the object identifier of the object class with      */
/*                OCT-index ob_cl and all its superclasses.           */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   Short       matching object class found           */
/*      D2_NFOUND               no matching object class found        */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u16_cmp_superclasses(
			/* IN    -  object identifier to compare      */
    D2_obj_id *cmp_ob_id,
			/* IN    -  OCT-index of object class to      */
    signed16 ob_cl)     /*          compare                           */

{                                   /*  d26_u16_cmp_superclasses      */

register signed16   *sup_cl;    /* pointer to superclass array        */

if                                  /*  object identifier matches     */
				    /*  object class                  */
  (D27_ATTCMP(cmp_ob_id,&d26_oct[ob_cl].d26c_ob_id) == 0)
{                                   /*  return match                  */
  return(D2_NO_ERR);
}
else
{                                   /*  scan all superclasses         */
  for                               /*  all superclasses              */
     (sup_cl = d26_oct[ob_cl].d26c_sup_cl; *sup_cl != -1; sup_cl++)
  {                                 /*  call function recursively for */
				    /*  superclass                    */
    if                              /*  object identifier matches     */
				    /*  superclass                    */
      (d26_u16_cmp_superclasses(cmp_ob_id,*sup_cl) == D2_NO_ERR)
    {                               /*  return match                  */
      return(D2_NO_ERR);
    }                               /*                                */
  }                                 /*                                */
}                                   /*                                */

				    /*  return no match               */
return(D2_NFOUND);

}                                   /*  d26_u16_cmp_superclasses      */

/*exon ****************************************************************/
/*                                                                    */
/* NAME         : d26_u17_check_data_version                          */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  : This function compares the content of the data      */
/*                version file with the data version of the DSA.      */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*              D2_NO_ERR       Short           versions are equal    */
/*              D26_ERROR                       versions are not equal*/
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u17_check_data_version(
    char *filename,     /* IN    -  name of data version file         */
    int *data_version)  /* OUT   -  data version number               */

{                                   /*  d26_u17_check_data_version    */

FILE *fp;                           /* File pointer for data version  */
				    /* file                           */
signed16 ret_value = D2_NO_ERR;     /* return value                   */

if                      /*  file cannot be opened                     */
 ((fp = D27_FOPEN(svc_c_route_nolog,filename,"r")) == NULL)
{                       /*  log error                                 */
  *data_version = D26DV1;
}
else
{                       /*  read data version number                  */
  if                    /*  data version number can't be read         */
    (fscanf(fp,"%d",data_version) < 1)
  {                     /*  log error                                 */
    dce_svc_printf(GDS_S_NO_DATAVERSION_MSG,d26_dir_id);
    ret_value = D26_ERROR;
  }                     /*                                            */

  if                    /*  error from closing file                   */
   (D27_FCLOSE(svc_c_sev_fatal,fp))
  {                     /*  log error                                 */
    ret_value = D26_ERROR;
  }                     /*                                            */
}                       /*                                            */

if                      /*  no error occurred                         */
  (ret_value == D2_NO_ERR)
{                       /*  check data versions                       */
  if                    /*  data version not in supported range       */
    (D2_CHECK_MINMAX(D26DMIN1,D26DMAX1,*data_version) == FALSE)
  {                     /*  log error                                 */
    dce_svc_printf(GDS_S_DATAVERSION_ERR_MSG,d26_dir_id,D26DMIN1,D26DMAX1,
		   *data_version);
    ret_value = D26_ERROR;
  }                     /*                                            */
}                       /*                                            */

return(ret_value);


}                                   /*  d26_u17_check_data_version    */

/*exon ****************************************************************/
/*                                                                    */
/* NAME  :      d26_u18_scramble_string                               */
/*                                                                    */
/* DESCRIPTION: This function XORs the first length bytes of input    */
/*              with a hardcoded key and writes it to the output.     */
/*              The function is reversible.                           */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_u18_scramble_string(
    byte *input,        /* IN    -  string to be scrambled            */
    byte *output,       /* OUT   -  scrambled string                  */
    signed16 length)    /* IN    -  length of input string            */

{                               /*  d26_u18_scramble_string           */

			/* hard coded key                             */
static byte key[] = "-PA5/ FSq1y<uFdq32)~?|'ZaR%~HAz";

			/* pointer to key                             */
register byte     *pkey;
register signed16 i;    /* loop variable                              */

for                 /* all characters of input string                 */
   (i = 0, pkey = key; i < length; i++)
{                   /* XOR the character with key character           */
  *output++ = *input++ ^ *pkey++;
  if                /* key length is reached                          */
    (pkey == key + sizeof(key) - 1)
  {                 /* Reset the key pointer                          */
    pkey = key;
  }                 /*                                                */
}                   /*                                                */

}                               /*  d26_u18_scramble_string           */

/*exon ****************************************************************/
/*                                                                    */
/* NAME  :      d26_u28_norm_numbers                                  */
/*                                                                    */
/* DESCRIPTION: This function removes all characters, which are not   */
/*              digits.                                               */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      Ushort                      length of out_string              */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_u28_norm_numbers(
    char *inp,          /* IN    -  numeric string to be normed       */
    signed16 length,    /* IN    -  length of string to be nnormed    */
    char *out)          /* OUT   -  normed numeric string             */

{                               /*  d26_u28_norm_numbers              */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

signed16 mode;
signed16 len;

register signed16 i;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

for                             /*  all characters of input string    */
   (i = 0, len = 0, mode = D26_DIGIT; i < length; i++, inp++)
{                               /*  check mode                        */
  switch(mode)
  {
    case D26_DIGIT:
      if(*inp >= D2_RG9 && *inp <= D2_RG10)
      {
	*out++ = *inp;
	len++;
      }
      else
      {
	mode = D26_NONDIGIT;
      }
      break;
    case D26_NONDIGIT:
      if(*inp >= D2_RG9 && *inp <= D2_RG10)
      {
	mode   = D26_DIGIT;
	*out++ = *inp;
	len++;
      }
      break;
  }   
}                               /*                                    */

return(len);

}                               /*  d26_u28_norm_numbers              */

/*exon ****************************************************************/
/*                                                                    */
/* NAME         : d26_u59_attr_in_obj_class()                         */
/*                                                                    */
/* DESCRIPTION  : This function checks, whether a given attribut is   */
/*                contained as a mandatory or an optional attribute   */
/*                in one of the given object classes or in one of     */
/*                their superclasses.                                 */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_TRUE     boolean         The object classes contain the    */
/*                                  attribute.                        */
/*      D2_FALSE                    The object classes do not contain */
/*                                  the attribute.                    */
/*                                                                    */
/*exoff ***************************************************************/

boolean d26_u59_attr_in_obj_class(
    signed32 *ob_cl,    /* IN    -  array of object class numbers     */
    signed16 at_idx)    /* IN    -  AT-index of attribute to check    */

{                                   /*  d26_u59_attr_in_obj_class     */

register    D26_oct *oct;       /* pointer to OCT                     */
register    signed16 i;         /* loop variable                      */

for                                 /*  all object classes of object  */
   (; *ob_cl != D26_EOL; ob_cl++)
{                                   /*  set the OCT pointer           */
  oct = d26_oct + *ob_cl;

  for                               /*  all mandatory attributes      */
     (i = 0; oct->d26c_mandatory[i] >= 0; i++)
  {                                 /*  compare AT-indices            */
    if                              /*  indices do match              */
      (oct->d26c_mandatory[i] == at_idx)
    {                               /*  return TRUE                   */
      return(TRUE);
    }                               /*                                */
  }                                 /*                                */

  for                               /*  all optional  attributes      */
     (i = 0; oct->d26c_optional[i] >= 0; i++)
  {                                 /*  compare AT-indices            */
    if                              /*  indices do match              */
      (oct->d26c_optional[i] == at_idx)
    {                               /*  return TRUE                   */
      return(TRUE);
    }                               /*                                */
  }                                 /*                                */
}                                   /*                                */

				    /*  return FALSE                  */
return(FALSE);

}                                   /*  d26_u59_attr_in_obj_class     */
