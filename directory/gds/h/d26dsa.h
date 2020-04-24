/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26dsa.h,v $
 * Revision 1.1.755.2  1996/02/18  23:35:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:51  marty]
 *
 * Revision 1.1.755.1  1995/12/08  16:00:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:46  root]
 * 
 * Revision 1.1.753.6  1994/08/24  12:14:56  keutel
 * 	OT 11802
 * 	[1994/08/23  15:23:04  keutel]
 * 
 * Revision 1.1.753.5  1994/07/06  15:07:29  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:16:20  marrek]
 * 
 * Revision 1.1.753.4  1994/06/21  14:47:09  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:51:24  marrek]
 * 
 * Revision 1.1.753.3  1994/05/10  15:55:50  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:37:59  marrek]
 * 
 * Revision 1.1.753.2  1994/03/23  15:18:24  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:29:44  keutel]
 * 
 * Revision 1.1.753.1  1994/02/22  18:49:56  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:45:14  marrek]
 * 
 * Revision 1.1.751.3  1993/10/29  15:11:36  keutel
 * 	CR 9261
 * 	[1993/10/29  14:08:40  keutel]
 * 
 * Revision 1.1.751.2  1993/10/14  17:27:47  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:26:48  keutel]
 * 
 * Revision 1.1.751.1  1993/10/13  17:32:36  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:26:31  keutel]
 * 
 * Revision 1.1.6.3  1993/08/11  08:58:54  marrek
 * 	New data version number 50 for D26DV5.
 * 	[1993/08/11  08:57:46  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:39:52  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:40:13  marrek]
 * 
 * Revision 1.1.4.6  1993/02/02  15:02:12  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.5  1993/02/02  11:37:44  marrek
 * 	define D26_NR_COMP_TELEX_NR and D26_UB_POSTAL_LINE
 * 	[1993/02/01  16:00:34  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  20:40:46  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:29:36  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  09:59:56  marrek
 * 	Allow logical unbind after DSP reject
 * 	[1992/12/16  13:07:54  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  19:09:49  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  09:32:19  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  04:04:30  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:42:18  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D26DSA_H
#define _D26DSA_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d26dsa_rcsid[] = "@(#)$RCSfile: d26dsa.h,v $ $Revision: 1.1.755.2 $ $Date: 1996/02/18 23:35:43 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : d26dsa.h                                            */
/*                                                                    */
/* AUTHOR       : W. Schmid, D AP 11                                  */
/* DATE         : 7. 12. 1987                                         */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOK.-NR.     : Directory-Design-Specification                      */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : This Include-File contains "typedefs" and "defines" */
/*                used for directory ISERVICE-interfaces.             */
/*                                                                    */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 87-12-07| Original                       | WS |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>

#include <search.h>

#define D26DV1          10  /* version number of DSA-data             */
#define D26DV2          20  /* version number of DSA-data DCE V1.0    */
#define D26DV3          30  /* version number of DSA-data DCE V1.0.1  */
#define D26DV4          40  /* version number of DSA-data DIR.X V3.0  */
#define D26DV5          50  /* version number of DSA-data DCE V1.0.3  */
#define D26DV6          60  /* version number of DSA-data DCE V1.1    */

			    /* Representations in database            */
#define D26DMIN1    D26DV6
#define D26DMAX1    D26DV6

/****************************** EXIT STATUS ***************************/

#define D26_INIT_SVC_ERR 42 /* serviceability cannot be initialized   */
#define D26_SYS_ERR      43 /* Fatal system error occurred            */
#define D26_IPC_ERR      44 /* IPC error occurred                     */
#define D26_INIT_DSA     45 /* DSA cannot be initialized              */
#define D26_EXECV_ERR    46 /* DSA cannot execute himself             */
#define D26_SHUT_ERR     47 /* DSA cannot shutdown properly           */

/****************************** ERRORS ********************************/

#define D26_ERROR     -1000 /* any error                              */
#define D26_REFERRAL  -1001 /* referrals are supplied                 */
#define D26_NOFILTERMATCH  -1002
			    /* object was found, but filter attribute */
			    /* doesn't match                          */
#define D26_CONCATENATION  -1003    /* indicates that the purported   */
				    /* distinguished name may be an   */
				    /* alias name                     */
				    /* --> the DN is party checked    */
			    /* error came from DSP                    */
#define D26_DSP_ERR        -1004


/*******************Internal    RETURN-VALUES**************************/

#define D26_NO_CHANGE     1 /* dn_list in core has not been changed   */
#define D26_CHANGED       2 /* dn_list was changed                    */

/*******************Definitions for process structure******************/

#define D26_L_LOG           20      /* length of string for putenv    */
#define D26_ADDON_PAR     "-o"  /* parameter for add on DSAs          */
#define D26_IDLE_TIME      120  /* time for idling add on DSAs to live*/

/************** constants for description of schema object      *******/

#define D26_CNL           64    /* length of  Common Name             */
#define D26_TLEN          19    /* length of schema time stamp        */
#define D26_OCL_VAL_COUNT  5    /* maximum count of structural object */
				/* classes                            */

/********** constants for values of configuration parameters **********/

				/* chaining policy indication for ... */
#define D26_SINGLE_HOP  0       /* ... single hop                     */
#define D26_MULTI_HOP   1       /* ... multi hop                      */

/************** constants d26_dbop_flag                         *******/

#define D26_DB_NOOP     0       /* no operations on DB allowed        */
#define D26_DB_RW       1       /* all operations on DB allowed       */
#define D26_DB_RO       2       /* only retrieval operations allowed  */

/************** various constants                               *******/

			    /* maximum length of an attribute value   */
#define D26_MAXVALLEN  32767
#define D26_MAXVALPARTS   10/* maximum count of parts in an attribute */
			    /* value of some 'list'-syntax            */
#define D26_NR_COMP_TELEX_NR 3  /* count of components in telex number*/
#define D26_UB_POSTAL_LINE 6    /* Upper bound for postal address     */

			    /* maximum possible index of dn list in   */
			    /* core                                   */
#define D26_MAX_DNLIDX  32767
#define D26_REF_BLOCKSIZE 100   /* maximum count of referrals         */

#define D26_MAXACP  20      /* maximum count of access points in      */
			    /* referral structure                     */
#define D26_DBACC_BLOCK 100L    /* block of continuous C-ISAM accesses*/

#define D26_ROOT        -1  /* index of "root"                        */
#define D26_LPHON        4  /* length of phonetic attribute values    */

/******* define used by more than one module of DS_MODIFY_ENTRY *******/
			      /* begin of new acl index values in     */
			      /* array                                */
#define D26_C07_NEW_ACL   0
			      /* new masterknowledge index value in   */
			      /* array                                */
#define D26_C09_NEW_MK    2 * D2_ACLIDX_MAX


#define D26_NOTHING     99  /* used for indicating no action in a     */
			    /* 'switch-statement'                     */
			    /* e.g.: d26_i02_upd_file_dnlist          */

			    /* meanings of indices (< 0) "pointing to"*/
			    /* DNL                                    */
#define D26_OWNNAME     -1  /* own name is considered                 */
#define D26_O_SUBTREE   -2  /* subtree level, e. g.                   */
			    /*     -2: own subtree                    */
			    /*     -3: subtree of "father" object     */
			    /*     -4: subtree of "grand father" obj. */
			    /*     etc.                               */
#define D26_TREE        -99 /* whole DIT has rights                   */
#define D26_EOL         -100/* ACL-list is finished (end of list)     */
#define D26_IDX_NOT_DEF (D26_EOL - 1)
			    /* ACL-index is yet unknown               */


#define D26_OBJ            0    /* flag for C-ISAM file - handling    */
				/* for objects                        */
#define D26_ATT            1    /* flag for C-ISAM file - handling    */
				/* for attributes                     */


			    /* access classes                         */
			    /* --> "D26_aclass"                       */
#define D26_PUBLIC       0
#define D26_STANDARD     1
#define D26_SENSITIVE    2


			    /* importances of attribute               */
#define D26_DOANY        0
#define D26_NODELETE     1              /* mandatory                  */
#define D26_NOCHANGE     2
#define D26_NOUPDATE     3

			    /* indicator which right is to be checked */
			    /* in acl-list                            */
			    /* --> "D26_ch_right"                     */
#define D26_READ                0
#define D26_MODIFY              1

#define D26_BLOCK_LEN 16384 /* length of one result block             */

#define D26_ANONYMOUS   " "  /* anonymous requestor                   */
#define D26_NOT_TRUSTED "nt" /* DSA not trusted - check ACL with anon.*/

			    /* requestor types                        */
#define D26_NORMAL_REQ 0    /* indicates normal requestor             */
#define D26_PRIVIL_REQ 1    /* indicates privileged requestor         */

			    /* schema modification flags indicating ..*/
#define D26_NO_SM_PENDING 0 /* ... no pending schema modification     */
#define D26_SM_PENDING 1    /* ... pending schema modification        */
#define D26_SM_COMPLETE 2   /* ... terminated schema modification     */
#define D26_SM_BROKEN  3    /* ... broken schema modification         */
#define D26_SM_UNKNOWN 4    /* ... schema modification unknown        */

#define D26_ATTCMP(x,y)   ((x)->d2_typ_len != (y)->d2_typ_len ||    \
		      memcmp((x)->d2_type, (y)->d2_type, (x)->d2_typ_len))
			       /* compares two attribute types        */
			       /* param: addresses of struct D2_a_type*/

/*******************Internal    RETURN-VALUES**************************/
			    /* error values from collecting results   */
#define D26_ABORTED     -1  /* association is aborted or IPC error    */
#define D26_MEM_ERR     -2  /* malloc error occurred                  */
#define D26_LOG_ERR     -3  /* result is an error                     */
#define D26_REJECTED    -4  /* a request was rejected                 */

/***************** constants used in req. decomposition. **************/
#define D26_RD_RESULT		0	/* result.		*/
#define D26_RD_REFERRAL		1	/* referral result.	*/
#define D26_RD_ERROR		2	/* error result.	*/

/*******************Definitions for data structures *******************/

typedef signed16 D26_ch_right;          /* indicates which right is to*/
					/* be checked in ACL-list     */

typedef signed32 D26_acl;               /* "Long" pointing to DN in   */
					/* Distinguished Name List    */

typedef signed16 D26_aclass;            /* access classes             */

typedef struct
	{
	    char    d26_req_type;       /* indicates requestor type   */
					/* DN of requestor            */
	    char    d26_sm_flag;        /* indicates an active schema */
					/* modification               */
	    signed16 d26_sec_meth;	/* security method	      */
	    signed32 d26_trust_level;   /* trust level of chaining DSA*/
					/* exact requestor name       */
	    char    d26_req_name[D2_DNL_MAX + 1];
					/* normed requestor name      */
	    char    d26_normed_req_name[D2_DNL_MAX + 1];
					/*name for ACL check	      */
	    char    d26_acl_name[D2_DNL_MAX + 1];
	} D26_requestor;                /* Requestor structure        */

					/* length for add_usrcred-calls */
					/* has to be extended with the  */
					/* strlen(d26_req_name) + 1     */
#define D26_REQ_LEN (2 * sizeof(char) + sizeof(signed16) + sizeof(signed32) \
		    + strlen(d26_requestor.d26_req_name) + 1)

typedef struct
	{   D2_dnint        d26_dnint;      /* DN-Interpretation      */
	    D2_name_string  d26_dnname;     /* Distinguished name     */
	    D2_name_string  d26_ename;      /* exact Dist. name       */
	    signed32        d26_occurrency; /* number of references to*/
					    /* distinguished name     */
	} D26_dndesc;


typedef struct
	{   D2_dnint        d26_dint;   /* DN-Interpretation          */
	    D2_name_string  d26_dname;  /* normed DN-name             */
	    char            d26_edname[D2_DNL_MAX]; /* exact DN-name  */
	    signed32        d26_dno;    /* number of referencies      */
					/* > 0: add in DN-ISAM-file   */
					/* < 0: remove from DN-ISAM-  */
					/*      file                  */
	    signed32       *d26_didx;   /* index in DNL-list          */
					/* >= 0: real index           */
					/* = D26_OWNNAME: own name    */
					/* = D26_WH_SUBTREE: whole    */
					/*                   subtree  */
					/* = D26_EOL: end of list     */
					/* = D26_IDX_NOT_DEF: index   */
					/*                    unknown */
	} D26_dns;


typedef struct
	{   signed32       d26_version; /* version number of DN-list  */
					/* changed on every update    */
	    char           d26_reason;  /* changed file (for further  */
					/* use, ignored)              */
	} D26_update_info;

typedef struct
	{   D2_attr_mod    d26_m;       /* modification type           */
	    D2_a_type      d26_mtype;   /* attribute type              */
	    unsigned16     d26_mrep;    /* attribute representation    */
	    D23_av_value   *d26_mold;   /* old attribute (null-ptr.    */
					/* indicates no old val. given)*/
	    D23_av_value   *d26_mnew;   /* new attribute null-ptr.     */
					/* indicates no new val. given)*/
	} D26_md_info;                  /* modification info block     */

typedef struct
	{   signed16       d26_error;   /* error value for filter     */
	    D23_fi1_info   *d26_filter; /* pointer to erroneous filter*/
	} D26_f_err;                    /* filter error information   */

typedef struct
	{
	    D2_name_string d26_dsaname; /* normed DSA name            */
	    D2_name_string d26_dsa_ename;   /* exact DSA name         */
	    D23_av_value   *d26_dsa_ep; /* undecoded part of DSA name */
	    D2_str         d26_psap;    /* PSAP-Address of access pt. */
	    signed32       d26_dnl_idx; /* DN-list index              */
	    boolean        d26_nam_alloc;   /* indicates allocated    */
					/* memory                     */
	    boolean	   d26_auth_m[D2_MAX_AUTH_MECHS];
	    char          *d26_pr_name; /* princ. name (if DCE-Sec.)  */
	} D26_acc_point;                /* access point               */

			    /* referral structure                     */
typedef struct  {
					/* target object              */
		    D2_name_string  d26_target;
					/* last RDN of object to add  */
		    D2_name_string  d26_new_rdn;
					/* undecoded part of target o.*/
		    D23_av_value    *d26_targ_ep;
					/* exact name of target object*/
		    D2_name_string  d26_etarget;
					/* operation progress         */
		    D2_op_progress  d26_op_prog;
					/* aliased RDNs               */
		    signed16        d26_al_rdns;
					/* RDNs resolved              */
		    signed16        d26_rdn_res;
					/* indicates whether only the */
					/* target object must be      */
					/* searched                   */
		    boolean         d26_entry_only;
					/* count of access points     */
		    signed16        d26_acp_count;
					/* array of access points     */
		    signed16        d26_acp[D26_MAXACP];
					/* indicates that referral is */
					/* used for routing only      */
		    boolean         d26_route;
					/* associated bind table entry*/
		    signed16        d26_bt_index;
					/* DN-list index of master DSA*/
					/* of target object           */
		    signed32        d26_master_dsa;
					/* index of referral in array */
		    signed16        d26_ref_index;
					/* index of predecessor in    */
					/* referral tree              */
		    signed16        d26_predecessor;
					/* indicates own shadowing dsa*/
		    boolean         d26_own_sh_dsas;
					/* indicates whether referral */
					/* must be ignored            */
		    boolean         d26_ignore;
		} D26_referral;

typedef struct
	{
	    signed16        d26_a_idx;  /* AT index of attribute      */
	    signed16        d26_a_len;  /* length of attribute value  */
	    byte *          d26_a_val;  /* Attribute value            */
	} D26_ava;                      /* Attribute value assertion  */

typedef struct
	{   signed16        d26_r_idx;  /* index of RDN in SRT        */
	    signed16        d26_r_nava; /* number of AVAs in RDN      */
	    D26_ava         d26_r_ava[D2_NO_AVA];   /* Array of       */
					/* attribute value assertions */
	} D26_rdninfo;                  /* RELATIVE DISTINGUISHED NAME*/
					/* INFO                       */


typedef struct
	{
	    signed16        d26_filenr; /* number of file containing  */
					/* object                     */
	    signed16        d26_rd_anz; /* number of RDNs             */
	    D26_rdninfo     d26_rdn[D2_NP_MAX];
					/* RDN-Info                   */
	} D26_dninfo;                   /* DISTINGUISHED NAME INFO    */

			    /* target object structure                */
typedef struct  {
		    D2_name_string  d26_tdname;
		    D2_name_string  d26_tename;
		    signed16        d26_tbo_nr;
		    D2_name_string  d26_tunresolved;
		    signed16        d26_tal_rdns;
		    signed32        d26_tob_cl[D26_OCL_VAL_COUNT + 1];
		    D26_acl         d26_tacl[D2_ACLIDX_MAX];
		    signed32        d26_tmk;
		    signed16        d26_tfilenr;
		    char            *d26_trecord;
		    D2_subset       d26_tsubset;
		    byte            **d26_tmessage;
		    byte            **d26_toutput;
		    signed32        *d26_toutput_len;
		    signed32        *d26_tremlen;
		    D23_rq_info     *d26_tr_info;
		    D23_ft1_info    *d26_tf_info;
		    signed16        d26_top_id;
		    boolean         d26_tbase;
		} D26_target;

typedef struct
	{   signed16        d26_at_idx; /* Index of attribute in AT   */
	    boolean         d26_in_ocls;/* indicates whether attribute*/
					/* belongs to given object    */
					/* classes                    */
	    signed16        d26_srt_idx;/* Index of SRT-entry         */
	    signed16        d26_nam_idx;/* index of naming attribute  */
					/* in SRT entry               */
	    boolean         d26_read_ok;/* a flag indicating whether  */
					/* the attribute can be read  */
	} D26_attr_info;                /* Attribute Info (in AT)     */

typedef struct
	{
					/* AT-index of naming attrib. */
	    signed16        d26s_index;
					/* Array of positions of      */
					/* naming attributes in C-ISAM*/
					/* files                      */
	    signed16        *d26s_position;
	} D26_nam_at;                   /* naming attribute descript. */

typedef struct
	{                               /* OCT-index of ...           */
	    signed16        d26s_oclass;/* ... object class           */
	    signed16        d26s_supocl;/* ... superior object class  */
	} D26_ocl_arc;                  /* arc of object classes      */

typedef struct
	{
	    signed16        d26s_par_nr;/* Object number of parent    */
					/* object in SRT              */
	    D26_nam_at      *d26s_nam_at;   /* array of naming        */
					/* attribute  descriptions    */
	    D26_ocl_arc     *d26s_ocl_arc;  /* arc of object classes  */
	} D26_srt;                      /* STRUCTURE RULE TABLE       */


typedef struct
	{
	    D2_obj_id       d26c_ob_id; /* Object identifier of the   */
					/* object class               */
	    signed16        d26c_kind;  /* Kind of object class       */
	    signed16        d26c_filenr;/* Number of C_ISAM file with */
					/* objects of this class      */
	    signed16       *d26c_sup_cl;/* array of indices of the    */
					/* superclasses of this class */
	    signed16       *d26c_aux_cl;/* array of indices of the    */
					/* auxiliary object classes   */
					/* of this class              */
	    signed16       *d26c_mandatory;/* Indices of mandatory    */
					/* attributes of this class   */
	    signed16       *d26c_optional;/* Indices of optional      */
					/* attributes of this class   */
	} D26_oct;                      /* OBJECT CLASS TABLE         */


typedef struct
	{
	    D2_a_type    d26a_type;     /* attribute type             */
	    signed16     d26a_lbound;   /* min. length of attribute   */
	    signed16     d26a_ubound;   /* max. length of attribute   */
	    signed16     d26a_nr_recur; /* maximum number of values   */
					/* = 1: single valued         */
					/* > 1: recurring attribute   */
					/*      stored together with  */
					/*      object                */
					/*   0: ("real") recurring    */
					/*      attribute (number     */
					/*      unlimited)            */
	    signed16     d26a_importance; /* indicates whether attrib */
					/* may be changed or deleted. */
	    signed16     d26a_syntax;   /* attribute syntax           */
	    signed16     d26a_rep;      /* ASN.1 representation       */
	    boolean      d26a_phon;     /* indicates whether phonetic */
					/* matching is allowed        */
	    D26_aclass   d26a_acl;      /* Access-Class of attribute  */
	    signed16     d26a_filenr;   /* File no. of file containing*/
					/* attribute values.          */
	    signed16     d26a_ind_level; /* indicates the index level */
					/* of the attribute           */
	    signed16    *d26a_field_pos; /* positions in record       */
      } D26_at;                         /* ATTRIBUTE TABLE            */

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

			/* exported by d26util.c                      */

extern char         d26_logdir[];   /* path name for logfiles         */
extern char         d26_logfile[];  /* Name of logfile                */
				    /* name and file-pointer for ASN1-*/
extern char         asn_file[];     /* logfile                        */
extern FILE         *d27_asn1_fp;
extern char         d26_chdb_name[];/* path name of chdb for call     */

extern char         *d26_chdb_prog; /* basename of chdb               */

extern D26_attr_info *d26_attr;     /* field of attribute informations*/

extern boolean      *d26_ate_used;  /* indicates for each AT entry    */
				    /* whether it is in the message   */
				    /* already                        */

				    /* AT-index of ...                */
extern signed16     d26_obcl_idx;   /* ... object class attribute     */
extern signed16     d26_alob_idx;   /* ... aliased object attribute   */
extern signed16     d26_mk_idx;     /* ... master knowledge attribute */
extern signed16     d26_acl_idx;    /* ... ACL-attribute              */
extern signed16     d26_pwd_idx;    /* ... User-Password-attribute    */
extern signed16     d26_psap_idx;   /* ... PSAP-attribute             */
extern signed16     d26_dnl_idx;    /* ... DN-List-attribute          */
extern signed16     d26_am_idx;     /* ... Auth.-Method-attribute     */
extern signed16     d26_pn_idx;     /* ... Principal-Name-attribute   */

				    /* OCT-index of ...               */
extern signed16     d26_ali_idx;    /* ... object class alias         */
extern signed16     d26_dsa_idx;    /* ... object class DSA           */

				    /* maximum index of DN list in    */
				    /* core                           */
extern signed32     d26_maxidx_dnlist;

				    /* Requestor name of directory    */
extern D26_requestor d26_requestor; /* caller (normed)                */

				    /* name of base object for        */
				    /* internal search                */
extern D2_name_string d26_base_object;
				    /* undecoded part of base object  */
extern D23_av_value *d26_bo_encoded_part;
				    /* base object contained in DAP-  */
				    /* message                        */
extern D2_name_string d26_dap_b_object;
extern D23_av_value *d26_dap_bo_ep; /* undecoded part of DAP base obj.*/

/* global variables for delta-updates */

extern byte         d26_objname[];  /* actual object name (normed)    */
				    /* master knowledge of superior's */
				    /* entry in case of update functs.*/
extern signed32     d26_sup_mk;     /* set by add_object or logg_mesg.*/
				    /* file pointer of SHDLOG file    */
				    /* == D2_NULL: file closed        */
extern FILE         *d26_fpshdlog;  /* != D2_NULL: file opened        */
				    /* indicates permitted database   */
extern signed16     d26_dbop_flag;  /* operations                     */

				    /* indicator whether DSA has to be*/
extern boolean      d26_restart;    /* restarted after saving         */

extern boolean      d26_ma_info;    /* needed for logging, whether    */
				    /* ma-info is changed or not      */

extern D26_update_info d26_upd_info;/* time of last change of DN list */

extern time_t       d26_ftime;      /* time for last access to files  */

extern byte         d26_myname[];   /* own DSA-Name (normed)          */
				    /* own DSA-Name (case sensitive)  */
extern byte         d26_myexact_name[];
				    /* my principal name.             */
extern char         d26_mypr_name[D2_DNL_MAX + 1];
				    /* schema object name (normed)    */
extern byte         d26_schema_name[];

extern signed32     d26_dbacc_count;/* count of database accesses     */
extern signed32     d26_entry_count;/* count of entries written       */
extern boolean      d26_ref_present;/* indicates that referrals are   */
				    /* present                        */
extern void         *d26_ref_root;  /* root of referral tree          */
extern D26_referral d26_referral[]; /* pointer to array of referrals  */
extern signed32     d26_ref_count;  /* count of referrals written     */
				    /* address of partial outcome     */
				    /* qualifier in message           */
extern byte         *d26_start_p_o_q;
				    /* total length of partial outcome*/
				    /* qualifier                      */
extern signed32     d26_p_o_q_length;
				    /* service controls               */
extern D23_serv_ctrl *d26_serv_contr;
extern signed16     d26_al_rdns;    /* aliased RDNs                   */
				    /* operation progress             */
extern D23_op_pr_info *d26_op_progress;
				    /* chaining arguments             */
extern D23_chain_arg  *d26_chain_arg;

extern D23_srcharg  *d26_search_arg;/* search   arguments             */
extern D2_subset    d26_subset;     /* subset used for search         */
extern signed32     d26_time_limit; /* time limit in seconds          */
extern boolean      d26_time_ex;    /* time limit exceeded            */
extern boolean      d26_abandoned;  /* indicates abandoned request    */

extern D26_acc_point *d26_accp_list;/* list of master PSAP-Addresses  */
extern signed32     d26_accp_count; /* count of written access points */
extern signed32     d26_accp_max;   /* maximum number of elements that*/
				    /* access point list can hold     */
extern signed32     d26_bind_count; /* no. of entries in bind table   */

extern signed16     d26_dir_id;     /* own directory id               */
				    /* indicates whether DSA has to   */
extern boolean      d26_upd_cache;  /* update cache                   */
				    /* indicates whether DSA has to   */
extern boolean      d26_ulogging;   /* log update operations          */
				    /* indicates that ISAM index must */
extern boolean      d26_reset;      /* be reset                       */
				    /* indicates chaining policy of   */
extern signed16     d26_chaining_policy;    /* the DSA                */
				    /* indicates routing facility of  */
extern boolean      d26_routing;    /* the DSA                        */

				/* authentication mechanisms supported*/
				/* by the DSA, initialised to FALSE   */
extern boolean      d26_auth_mech[D2_MAX_AUTH_MECHS];

			/* indicators for routing priorities          */
extern signed16    d26_knowledge_prio;
extern signed16    d26_routing_prio;
			/* number of routing DSAs                     */
extern signed16    d26_nr_routing_dsas;
			/* array of routing access points             */
extern D26_acc_point *d26_routing_access_points;
			/* configuration parameter to delay DSP unbind*/
extern time_t      d26_dspunbind_delay;

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

			/* exported by d26dsa.c, d26mkiss.c,          */
			/* d26gendb.c d26trans.c d26mkupd.c           */

extern int main(int argc, char *argv[]);

			/* exported by d26mkiss.c                     */

extern signed16 d26_msi1_write_configuration_data(FILE *fp);

			/* exported by d26sutil.c                     */

extern FILE     *d26_ms1_init(char *filename);
extern signed16 d26_ms2_terminate(FILE *fp, char *filename);

			/* exported by d26bind.c                      */

extern signed16 d26_1_bind(byte *a_in, byte *a_out, signed32 *length,
	unsigned16 cont_id, signed32 usr_id);

			/* exported by d26unbind.c                    */

signed16 d26_2_unbind(byte *a_in, byte *a_out, signed32 *length,
	unsigned16 cont_id, signed32 usr_id);

			/* exported by d26aban.c                      */
signed16 d26_9_abandon(byte *input, byte **output, signed32 *output_len,
	unsigned16 cont_id, signed32 usr_id);

			/* exported by d26read.c                      */

signed16 d26_3_read(signed16 op_id, byte *input, signed32 input_len,
	byte **output, signed32 *output_len, D23_rsrqpb *rsrqpb);

			/* exported by d26compare.c                   */
extern signed16 d26_6_compare(byte *input, signed32 input_len,
	byte **output, signed32 *output_len, D23_rsrqpb *rsrqpb);

			/* exported by d26list.c                      */
extern signed16 d26_4_list(byte *input, signed32 input_len, byte **output,
	signed32 *output_len, D23_rsrqpb *rsrqpb);

			/* exported by d26search.c                    */
extern signed16 d26_5_search(signed16 op_id, byte *input, signed32 input_len,
	byte **output, signed32 *output_len, D23_rsrqpb *rsrqpb);

			/* exported by d26common.c                    */

extern signed16 d26_u02_fill_field(char fill_character, signed16 ln_value,
	char *value, signed16 ln_field, char *field);

			/* exported by d26phon.c                      */

extern D2_obj_id *d26_soundex(byte *str, signed16 length, byte *ph_str);

			/* exported by d26util.c                      */

extern signed16 d26_u03_attr_search(D2_a_type *attr_typ);
extern signed16 d26_u06_get_name(D2_name_string dn, D26_dninfo *dninfo);
extern signed16 d26_u10_syntax_check(D26_at *at, unsigned16 rep, byte *val,
	signed16 length);
extern signed16 d26_u14_get_obj_class_idx(D2_obj_id *obj_class);
extern void     d26_u15_attr_error(byte **start_mess, D2_name_string object,
	signed16 call_nr, signed16 problem, D2_a_type *a_type,
	unsigned16 rep_ai, unsigned16 rep_av, signed16 attr_len,
	byte *attr_val, signed32 *memlen, signed32 *l_err);
extern signed16 d26_u16_cmp_superclasses(D2_obj_id *cmp_ob_id,
	signed16 ob_cl);
extern signed16 d26_u17_check_data_version(char *filename,
	int *data_version);
extern void     d26_u18_scramble_string(byte *input, byte *output,
	signed16 length);
extern signed16 d26_u28_norm_numbers(char *inp, signed16 length, char *out);
extern boolean  d26_u59_attr_in_obj_class(signed32 *ob_cl, signed16 at_idx);

			/* exported by d261util.c                     */

extern signed16 d26_u05_acl_check(D2_name_string entry, D26_ch_right right,
	D26_attr_info *attr_info, D26_acl *acl);
extern signed16 d26_u08_calc_len(D2_name_string name, signed16 level);
extern signed16 d26_u11_check_dn_index(signed32 index,
	D26_dndesc **dnlist_ptr);
extern signed32 d26_u12_get_dnlidx(D2_dnint dnint, D2_name_string dn);
extern signed16 d26_u13_store_del_dns(signed32 dnl_idx, signed32 **del_dns,
	signed16 *no_del_dns, signed16 *no_mem_del_dns,
	boolean *upd_dnl_file, D23_rserror *error, signed32 *l_err);
extern signed32 d26_u79_get_sstr_dnlidx(signed32 begin_idx, D2_dnint dnint,
	D2_name_string dn, signed16 cmp_len);

			/* exported by d26sntx.c                      */

extern signed32 d26_us4_is_PreferDel(byte *val, signed16 len);
extern signed32 d26_us6_is_NumString(byte *val, signed16 len);
extern signed32 d26_us7_is_UTCtime(byte *val);
extern void     d26_us9_norm_utc_time(char *inp, char *out);
extern signed32 d26_usa_is_tlxnr(byte *val, signed16 len);
extern signed32 d26_usb_is_ttxid(byte *val, signed16 len);
extern signed32 d26_usc_is_faxnr(byte *val);
extern signed32 d26_usd_is_OR_addr(byte *val, signed16 len);
extern signed32 d26_use_is_OR_name(byte *val, signed16 len);
extern signed32 d26_usf_is_sub_perm(byte *val);
extern signed32 d26_usg_is_seq_t61_prtbl(byte *val, signed16 len,
	signed16 c_count, signed16 c_maxlen);

extern signed32 d26_ush_is_certificate(byte **val);
extern signed32 d26_usi_is_cert_pair(byte **val);
extern signed32 d26_usj_is_cert_list(byte **val);
extern void     d26_uso_norm_acl(byte *in, byte *out);
extern void     d26_usp_norm_ci_list(byte *in, byte *out, signed16 len);


			/* exported by d26mhs.c                       */

extern signed16 d26_un1_norm_or_address(char *in, signed16 len, char *out);
extern signed16 d26_un2_norm_or_name(char *in, signed16 len, char *out);
extern signed16 d26_un3_norm_submit_perm(char *in, signed16 len, char *out);

			/* exported by d26fil.c                       */

extern signed16 d26_f01_check_filter(D2_name_string b_obj,
	signed16 *no_items, D23_ft1_info **f_info, byte **output,
	D2_subset subset, signed32 *output_len, signed32 *remlen,
	signed16 fct_tag);
extern D23_fi1_info *d26_f07_next_fitem(D23_fi1_info *f_item);
extern signed16 d26_f09_analyse_filter_object(signed16 ob_nr,
	signed32 *ob_cl, D23_ft1_info **f_info, D23_fi1_info **f_error);
extern signed16 d26_f14_multigrep(byte *str, signed16 len,
	D23_fs1_info *f_substr);
extern signed16 d26_f19_filter_attr_check(char *record, signed16 file_nr,
	signed16 ob_nr, D2_name_string dname, D26_attr_info *at_info,
	D26_acl *acl, D23_ft1_info *f_info, D23_fi1_info **f_err_item,
	signed16 fct_tag, boolean acl_only);
extern void     d26_f20_calc_fat_info(signed16 ob_nr, signed32 *ob_cl,
	D23_ft1_info *f_info, D26_attr_info *at_info);
extern boolean  d26_f22_is_ocl_leaf(D2_obj_id *ob_id, signed16 *oct_idx);
extern boolean  d26_f23_bo_matches_equal(D26_dninfo *dninfo,
	signed16 np_count, D2_a_match m_rule, D26_at *at, byte *value,
	signed16 length);
extern boolean  d26_f24_bo_matches_substrings(D26_dninfo *dninfo,
	signed16 np_count, D26_at *at, D23_fs1_info *f_substr);

			/* exported by retreq.c                       */

extern void     d26_u43_no_req_attr_error(byte **message,
	D2_name_string dname, D23_rq_info *r_info, signed32 *output_len,
	signed32 *remlen);
extern byte     *d26_u50_write_attributes(char *record, byte *message,
	byte **output, D2_name_string dname, D2_name_string ename,
	D23_rq_info *r_info, boolean *recur, D26_attr_info *attr,
	signed16 ob_nr, signed16 filenr, signed32 *remlen,
	signed32 *output_len);
extern byte     *d26_u56_write_attribute_type(byte *message, byte **output,
	D2_a_type *a_type, signed16 count, signed16 repr, signed32 *remlen,
	signed32 *output_len);
extern void     d26_u65_get_superclasses(signed32 ob_cl, void **ts_root,
	signed32 **ocl_list);
extern int      d26_u66_longcmp(const void *l1, const void *l2);
extern signed16 d26_u71_make_attr_list(D23_rq_info *r_info,
	D26_attr_info *attr, D2_name_string dname,  D26_acl *acl,
	signed16 ob_nr, signed32 *ob_cl, signed16 fct_tag);

			/* exported by retref.c                       */

extern signed16 d26_u35_find_access_point(signed32 dnl_idx,
	char interpretation, signed16 *acp_idx, D26_at *psap_at,
	boolean *db_access);
extern int      d26_u38_refcmp(const void *referral1, const void *referral2);
extern void     d26_u41_free_referrals(void);
extern byte     *d26_u48_write_referral_set(byte **output, byte *message,
	signed32 *output_len, signed32 *remlen);
extern void     d26_u58_make_subordinate_ref(D26_dninfo *dninfo,
	signed16 np_base, D2_name_string dname, D2_name_string ename,
	boolean *db_access, signed16 acp_idx, D26_at *psap_at);
extern signed16 d26_u63_write_error_referral(byte **output,
	signed32 *output_len, signed32 *remlen, D26_referral *referral);
extern signed32 d26_u64_first_level_mk(void);
extern byte     *d26_u68_write_trace_info(byte **output,
	signed32 *output_len, byte *message, signed32 *remlen,
	D26_referral *referral);
extern signed16 d26_u70_make_error_referrals(D26_dninfo *dninfo,
	D2_name_string dname, signed16 al_rdns, D2_name_string ename,
	D2_name_string unresolved, D2_name_string eunres,
	D23_av_value *undecoded, signed32 mk, boolean entry_only);
extern void     d26_u72_make_p_res_referrals(D2_name_string dname,
	D2_name_string ename, D26_dninfo *dninfo, signed16 al_rdns,
	signed32 mk, signed16 ob_nr, signed16 subset);

			/* exported by d26wrut.c                      */

extern byte     *d26_u30_write_part_out_qual(byte *message, byte **output,
	signed32 *remlen, signed32 *output_len);
extern byte     *d26_u34_write_subordinate(char *record, byte *message,
	byte **output, D26_dninfo *dninfo, signed32 mk, signed16 ob_nr,
	signed32 *remlen, signed32 *output_len);
extern byte     *d26_u37_write_entry(char *record, byte *message,
	byte **output, D2_name_string dname, D26_dninfo *dninfo,
	D23_rq_info *r_info, boolean *recur, D26_attr_info *attr,
	signed32 mk, signed16 ob_nr, signed32 *remlen, signed32 *output_len);
extern byte     *d26_u39_write_eom(byte *message, signed32 *remlen,
	byte **output, signed32 *output_len, signed16 fct_tag);
extern byte     *d26_u40_check_buffer(signed16 len, signed32 *output_len,
	signed32 *remlen, byte **output, byte *message);
extern byte     *d26_u44_write_child(byte *message, byte **output,
	D2_name_string dname, D26_dninfo *dninfo, signed16 np_count,
	D2_name_string tail, signed16 ob_nr, D23_rq_info *r_info,
	D23_ft1_info *f_info, signed32 *remlen, signed32 *output_len,
	signed16 fct_tag);
extern byte     *d26_u45_write_match_flag(byte *message, byte **output,
	signed32 *remlen, signed32 *output_len, signed16 matchflag,
	signed32 mk);
extern byte     *d26_u49_write_dist_name(byte *message, byte **output,
	D2_name_string dname, signed16 dnlen, signed32 *remlen,
	signed32 *output_len, unsigned16 dn_tag);
extern byte     *d26_u54_write_chaining_results(byte *message, byte **output,
	signed32 *remlen, signed32 *output_len);
extern signed16 d26_u84_send_abandoned(signed16 op_id, byte **output,
	signed32 *remlen, signed32 *output_len, D23_rsrqpb *rsrqpb);

			/* exported by d26copy.c                      */

extern void     d26_c07_copy_req_attributes(byte **from, byte **to);
extern void     d26_c08_copy_attribute_type(byte **from, byte **to);
extern void     d26_c09_copy_attribute_value(byte **from, byte **to);

			/* exported by d26anal.c                      */

extern signed16 d26_a01_analyse_search_message(byte *message, byte **output,
	signed32 *output_len, D2_name_string norm_base_obj,
	D23_rq_info **r_info, D23_ft1_info **f_info, signed16 op_id);
extern void     d26_a02_analyse_chaining_args(byte **message);
extern signed16 d26_a03_analyse_common_arguments(byte **message);
extern signed16 d26_a05_analyse_filter(D2_name_string b_obj,
	D23_ft1_info *f_info, byte **output, D2_subset subset,
	signed32 *output_len, signed32 *remlen, signed16 fct_tag);
extern void     d26_a06_write_filter_error(D2_name_string b_obj,
	D26_f_err *f_err, byte **output, signed32 *output_len,
	signed32 *remlen);
extern void     d26_a07_set_time_ex(int);
extern boolean  d26_a08_loop_detected(D2_name_string dsa_name,
	D23_av_value *dsa_ep, D2_name_string base_object,
	D23_av_value *bo_ep, D23_op_pr_info *bo_op);

			/* exported by retolis.c                      */

extern byte     *d26_i44_write_set(byte *message, byte **output,
	D2_subset subset, D2_name_string dname, D2_name_string ename,
	D26_dninfo *dninfo, signed16 al_rdns, signed16 bo_nr,
	signed32 *bob_cl, D26_acl *acl, signed32 mk, char *record,
	D23_rq_info *r_info, D23_ft1_info *f_info, signed32 *output_len,
	signed32 *remlen, signed16 fct_tag);

			/* exported by d26sali.c                      */

extern D26_target *d26_u69_fill_target(D2_name_string dname,
	D2_name_string ename, signed16 to_nr, D2_subset subset,
	void **target_root, D26_target *act_target);
extern signed16 d26_u73_use_alias(D26_target *cmp_target, D2_subset subset,
	signed16 filenr, void **target_root, D26_target **act_target,
	boolean *db_access, byte **message, byte **output,
	signed32 *output_len, signed32 *remlen, D23_rq_info *r_info,
	D23_ft1_info *f_info, signed16 op_id, boolean *cont_sa);
extern void     d26_u75_free_targets(void **target_root);
extern void     d26_u76_target_action(const void *v_target, VISIT visit,
	const int level);

			/* exported by retutil.c                      */

extern signed16 d26_u04_set_name_error_apdu(byte **output,
	signed32 *output_len, signed32 *remlen, signed16 err_value,
	signed32 mk, D26_dninfo *dn_info);
extern D2_name_string d26_u31_find_break_object(D26_dninfo *dninfo,
	D2_name_string assert, D2_name_string deref, char *record,
	boolean lock_mode, signed16 fct_tag);
extern boolean  d26_u46_descendant(signed16 ob_nr, signed16 desc);
extern signed16 d26_u57_find_master_part(D26_dninfo *dninfo, signed32 mk,
	signed16 *changed);
extern void     d26_u67_trace_name_struct(D26_srt *srt);
extern signed16 d26_u00_search(boolean lock_mode, signed16 fct_tag,
	byte *input, signed32 *output_len, byte **output, signed32 *remlen,
	D2_name_string exact_obj, D26_attr_info *attr, signed32 *ob_cl,
	D26_acl *acl, signed32 *mk, boolean *recurring, char **record,
	boolean *al_flag, signed16 *ob_nr);

			/* exported by d26cup.c                       */

extern void     d26_h01_cache_update(byte *result, signed16 op_id,
	signed16 dir_id, D23_serv_ctrl *serv_ctr,
	D2_name_string base_object);

			/* exported by d26logg.c                      */

extern signed32 d26_m12_logg_message(byte *request, signed32 req_len,
	signed16 op_id, boolean dnl_upd, boolean add_mk);
extern signed32 d26_m14_close_shdlog(void);
extern void     d26_m21_logg_dnlist(D2_attr_mod attr_mod,
	D2_name_string old_dn, D2_name_string new_dn);

			/* exported by d26mutil.c                     */

extern signed16 d26_m01_ret_result(signed16 op_id, signed32 err_id,
	byte *result, signed32 result_len, D23_rsrqpb *res_par,
	signed16 res_type);
extern void     d26_m02_free(void);
extern void     d26_m06_hdl_sigdistcmd(int sig);
extern void     d26_m08_set_logging(int *argc, char *argv[]);
extern void     d26_m09_execv(int argc, char *argv[]);
extern void     d26_m11_distcmd(int sig);

			/* exported by d26init.c                      */

extern signed16 d26_u09_init_dsa(signed16 dir_id);

			/* exported by d26dread.c                     */

extern signed16 d26_m15_get_bind_info(byte *msg_in, signed32 in_length,
	byte **msg_out, signed32 *length);

			/* exported by d26dsp.c                       */

extern signed16 d26_d01_chain_request(byte *dap_message, signed32 input_len,
	byte **output, signed32 *output_len, signed32 *remlen,
	signed16 op_id, D23_rsrqpb *rsrqpb);
extern signed16 d26_d02_bind_request(D23_bindpb *bindpb, D23_inrqpb *inrqpb,
	D2_str *psap_addr, D2_name_string dsa_name, boolean *auth_mechs, 
	char *princ_name, boolean *try_again, unsigned32 *sec_ctx_id, 
	signed16 *used_am);
extern signed16 d26_d04_send_request(D23_inrqpb *inrqpb, signed16 meslen,
	byte *request, signed32 request_len);
extern signed16 d26_d05_collect_result(signed16 op_id, D23_rsrqpb *rsrqpb,
	D23_wtrspb *wtrspb, byte **result, signed32 *result_len,
	signed32 *remlen, signed16 *err_id);
extern signed16 d26_d06_build_chained_request(byte **request,
	signed32 *request_len, byte *dap_message, signed32 mes_len,
	signed16 ref_idx,signed16 bind_sec_meth, boolean *auth_mechs);
extern signed16 d26_d10_unbind_request(D23_inrqpb *inrqpb,
	D23_wtrspb *wtrspb);
extern void d26_d11_conf_bind_result(signed16 bind_ret_val,
	unsigned16 cont_id, byte **result, signed32 *result_len,
	signed16 used_am, unsigned32 bind_id);

			/* exported by d26rdec.c                      */

extern signed16 d26_d21_req_decomp(byte *dap_message, signed32 input_len,
	byte **result, signed32 *result_len, signed32 *remlen,
	signed16 op_id, signed16 *result_type, D23_rsrqpb *rsrqpb);
extern signed16 d26_d34_comp_bind_table(void);
extern void     d26_d43_abandon_subrequests(void);

			/* exported by d26mutilis.c                   */

extern signed16 d26_mi1_call_read_scheme(void);
extern void     d26_mi2_free(void);
extern signed16 d26_mi3_open_files(void);
extern signed16 d26_mi4_close_files(void);
extern signed16 d26_mi6_save_schema_obj(void);
extern signed16 d26_mi7_clean_chdb(void);
extern signed16 d26_mi8_remove_saved_schema(void);

			/* exported by d26dnlis.c                     */

extern signed16 d26_i01_upd_core_dnlist(void);
extern signed16 d26_i02_upd_file_dnlist(D2_attr_mod type,
	D2_name_string old_dn, D2_dnint dnint, D2_name_string new_dn,
	signed32 occurrency, boolean new_version, signed32 *dnlist_core_idx);
extern signed16 d26_i05_open_dnlistfile(void);
extern signed16 d26_i06_close_dnlistfile(void);
extern signed16 d26_i07_read_dnlistrec(signed16 key, char *dnlistrec,
	boolean lock);
extern signed16 d26_i08_fill_charfield(char *field, char *contents,
	signed16 ln_field, char fill_character);
extern signed16 d26_i09_fill_dnint(D2_dnint from_dnint, D2_dnint *to_dnint);

			/* exported by d26utilis.c                    */

extern void     d26_i14_unlock(void);

			/* exported by retutilis.c                    */

extern signed16 d26_i31_find_object(D26_dninfo *dninfo, D26_rdninfo **l_rdn,
	char *record);
extern signed16 d26_i33_deref_alias(signed32 al_idx,
	D2_name_string normed_obj, D2_name_string exact_obj,
	D26_dninfo *dninfo);
extern void d26_i34_get_dninfo(char *record, D26_dninfo *dninfo,
	D2_name_string *dname, signed16 np_count);
extern signed16 d26_i35_reread_object(signed16 filenr, boolean lock);
extern void d26_i36_read_ocl_acl_mk(D26_dninfo *dninfo, char *record,
	signed32 *ocl, D26_acl *acl, signed32 *mk);
extern signed16 d26_i39_check_req_attr(char *record, signed16 filenr,
	D26_attr_info *attr);
extern signed16 d26_i48_get_exact_name(char *record, D26_dninfo *dninfo,
	signed16 start, D2_name_string exact_name);

			/* exported by d26filis.c                     */

extern signed16 d26_i79_cmp_sg_item(D23_fi1_info *f_item, signed16 filenr,
	char *record, D2_name_string dname, D26_at *at, signed16 pos);
extern signed16 d26_i81_cmp_rec_item(D23_fi1_info *f_item, signed16 file_nr,
	char *record, D2_name_string dname, D26_at *at, signed16 pos);

			/* exported by retreqis.c                     */

extern void d26_i37_write_dummy_record(char *record, signed16 *pos,
	signed32 idx, signed16 size);
extern byte *d26_i51_write_single_attribute(signed16 filenr, char *record,
	signed16 pos, D2_name_string object, byte *message, byte **output,
	D2_ret_val r_type, D26_at *at, signed32 *remlen,
	signed32 *output_len);
extern byte *d26_i52_write_recur_attribute(char *record, signed16 filenr,
	signed16 pos, D2_name_string object, byte *message, byte **output,
	D2_ret_val r_type, D26_at *at, signed32 *remlen,
	signed32 *output_len);
extern signed16 d26_i56_atdnlen(char *record, signed16 *pos, D26_at *at,
	D2_name_string *dname, boolean exact);
extern signed16 d26_i57_atacllen(D2_name_string object, char *record,
	signed16 *pos, byte *acl, boolean exact);

			/* exported by d26refis.c                     */

extern void     d26_i58_get_accp_info(signed16 filenr, char *record, 
	D2_str *psap_addr, boolean *auth_mechs, char **princ_name);
extern boolean  d26_i59_m_obj_in_file(signed16 ob_nr, signed16 filenr);
extern void     d26_i67_make_st_referrals(D26_at *psap_at, signed16 subset,
	D2_name_string dname, D26_dninfo *dninfo, signed16 ob_nr,
	signed32 mk);

			/* exported by d26alis.c                      */

extern signed32 d26_i47_get_al_references(signed16 filenr, char *record);
extern signed16 d26_i50_search_aliases(D26_target *cmp_target,
    D2_subset subset, D2_name_string dname, D26_dninfo *dninfo,
    signed16 to_nr, void **target_root, D26_target **act_target,
    byte **message, byte **output, signed32 *output_len, signed32 *remlen,
    D23_rq_info *r_info, D23_ft1_info *f_info, signed16 op_id);

			/* exported by remutil.c                      */

extern signed16 d26_u80_check_child (D23_ds_name *ds_name, byte *a_in, 
	byte **a_out, signed32 *length, byte **dap_mess, signed32 *dap_len, 
	boolean attr_req, D26_attr_info *attr_inf, signed32 *oclass, 
	D26_acl *acl, signed32 *mk, boolean *recurring, String *record, 
	boolean *al_flag, signed16 *obj_idx, signed16 op_id, 
	D26_acl *sup_acl_mrdn, signed32 sup_mk_mrdn);

			/* exported by addob.c			      */

extern signed16 d26_7_add_object(signed16, D23_inaddobjt *, signed32, 
	byte **, byte **, signed32 *, boolean *);

			/* exported by addutil.c                      */

extern void  d26_u20_sort_dn(D2_name_string own_name, D26_dns *dn_ptr[],
	signed16 dn_no, D26_dns *dn_sort[], signed16 *dn_act);
extern signed16 d26_u22_count_del(D2_name_string name, signed16 length);
extern signed16 d26_u23_get_acl_names(D2_name_string acl_attr, 
	signed16 acl_length, D26_dns **dnl_ptr, D26_dns *dns, D26_acl *acl_idx);
extern void  d26_u24_sort_acl_idx(D26_dns *dn_ptr[], signed16 dn_no, 
	D26_dns *dn_sort[], signed16 *dn_act);
extern void  d26_u28_copy_dn(char **from, char **to);
extern signed32   d26_u96_attr_len_check(D26_at *, signed16);
extern boolean  d26_u98_attr_val_cmp( byte *val1, signed16 len1, byte *val2, 
	signed16    len2, signed16 syntax);
extern signed16 d26_u99_handle_blanks(D26_at *at, char *val, signed16 len);

			/* exported by addutilis.c                    */

extern signed16 d26_750_create_obj(signed32 *, D26_dninfo *, D23_at_type *,
			D23_at_type *, D26_dns *[], D23_at_type **,
			D26_attr_info *, signed16 *, signed32 *);
extern signed16 d26_751_create_rec_attr(signed16, signed16, signed32, 
			D26_attr_info *, D23_at_type **, signed16 *);
extern signed16 d26_754_create_alias(signed16, D26_dninfo *, D26_attr_info *,
			signed16, signed32, signed32);

			/* exported by moden.c                        */

extern signed16 d26_C_modify_entry (signed16, D23_inmodentr *, signed32, 
	byte **, byte **, signed32 *, signed16, signed32);

			/* exported by modenfil.c                     */

extern signed16 d26_C04_change_main_entry(D2_name_string, signed32, signed16, 
		     boolean, signed16, D26_md_info *, D26_attr_info *, 
		     D26_dns *[], char *, boolean *, signed32 *, signed32 **, 
		     signed16 *, signed16 *, D23_rserror *, signed32 *);
extern void  d26_C09_init_val(char **, D26_at *); 
extern signed16 d26_C10_search_val (char *record, D26_at *p_at_entry,
	D26_oct *p_oct_entry, D26_nam_at *p_nam_at, signed16 length, 
	char *value, signed16 anz_vals, boolean no_value, 
	D2_name_string n_entry, boolean *upd_dnl_file, signed32 **del_dns, 
	signed16 *no_del_dns, signed16 *no_mem_del_dns, signed16 *val_no, 
	char **pos, D23_rserror *error, signed32 *l_err);
extern void d26_C11_copy_vals(char *record, D26_at *p_at_entry, 
	D26_oct *p_oct_entry, D26_nam_at *p_nam_at, signed16 from_val, 
	signed16 upto_val);
extern signed16 d26_C13_change_rec_attrs(signed32, signed32, signed16, 
	D26_md_info *, D26_attr_info *);
extern D2_name_string d26_C17_get_aliased_name(signed32, signed16, signed16, 
	char *, D23_rserror *, signed32 *);
extern boolean d26_u97_search_attr_val (char *, D26_at *, D26_oct *,
	D26_nam_at *, signed16, char *,signed16, D2_name_string, signed16 *, 
	char **);

			/* exported by remob.c                        */

extern signed16 d26_8_remove_object(signed16, byte *,signed32 , byte **, 
	byte **, signed32 *);

			/* exported by remutil.c                      */

extern signed16 d26_u80_check_child (D23_ds_name *ds_name, byte *a_in, 
	byte **a_out, signed32 *length, byte **dap_mess, signed32 *dap_len, 
	boolean attr_req, D26_attr_info *attr_inf, signed32 *oclass, 
	D26_acl *acl, signed32 *mk, boolean *recurring, String *record, 
	boolean *al_flag, signed16 *obj_idx, signed16 op_id, 
	D26_acl *sup_acl_mrdn, signed32 sup_mk_mrdn);

			/* exported by remutilis.c                    */

extern signed16 d26_850_delete_obj(String record, signed32 oclass, 
	boolean al_flag, signed16 obj_idx, byte *a_out, signed32 *a_len, 
	D26_oct **p_oct_entry);
extern signed16 d26_851_del_rec(String record, D26_at *at_entry, 
	signed32 oclass);
extern signed16 d26_852_upd_dnlist(String record, D26_at *at_entry, 
	signed32 oclass, D26_oct *p_oct_entry, boolean *upd_time);

			/* exported by modrdn.c                       */

extern signed16 d26_B_replace_rdn (signed16, D23_in_replrdn *, signed32, 
	byte **, byte **, signed32 * );

			/* exported by modrdnfil.c                    */

extern signed16 d26_B03_change_main_entry(D26_dninfo *dn_info, 
	signed16 last_rdn, boolean del_old_rdn, signed32 obj_class, 
	signed16 obj_idx, boolean alias_flag, char *record, byte **error, 
	signed32 *l_err);

			/* exported by d26stmutil.c                   */

extern signed16 d26_st01_chain_ref(signed16, signed32, signed32, boolean, 
	signed16);

#endif

