/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdsiapl.h,v $
 * Revision 1.1.10.2  1996/02/18  23:37:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:43  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:36:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:24  root]
 * 
 * Revision 1.1.8.8  1994/09/19  08:50:14  marrek
 * 	Fixes for OT 12198.
 * 	[1994/09/19  08:49:40  marrek]
 * 
 * Revision 1.1.8.7  1994/08/10  08:25:52  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:16:25  marrek]
 * 
 * Revision 1.1.8.6  1994/07/06  15:09:18  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:25:31  marrek]
 * 
 * Revision 1.1.8.5  1994/06/21  14:48:58  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:49:07  marrek]
 * 
 * Revision 1.1.8.4  1994/05/10  16:04:10  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:15:45  marrek]
 * 
 * Revision 1.1.8.3  1994/03/23  15:49:08  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:07  keutel]
 * 
 * Revision 1.1.8.2  1994/02/24  15:03:33  marrek
 * 	Change rcs string format.
 * 	[1994/02/24  12:13:54  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:59:48  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:20  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:02:56  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:10:56  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:20:29  marrek]
 * 
 * Revision 1.1.4.2  1992/12/30  23:30:33  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:20:35  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:16:27  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:43:29  melman]
 * 
 * $EndLog$
 */
#ifndef _XDSIAPL_H
#define _XDSIAPL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdsiapl_rcsid[] = "@(#)$RCSfile: xdsiapl.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:37:57 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xdsiapl.h                                           */
/*                                                                    */
/* AUTHOR       : V. Ryan                                             */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    : SNI XDS/XOM Design Specification November 1991      */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : XDS interface to IAPL                               */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      :                                                     */ 
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   1.0    | 90-10-01| Original                       | VR |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/



/* IAPL version */
#define IAPL_VERSION    D21V022

/* Definition of possible object-types at XDS-interface            */
/* these definitions are used by xds_private_object structure to   */
/* identify the structures to which it points                      */

#define XDS_ABANDON_FAILED              701
#define XDS_ACCESS_POINT                702
#define XDS_ADDRESS                     703
#define XDS_ATTRIBUTE                   704
#define XDS_ATTRIBUTE_ERROR             705
#define XDS_ATTRIBUTE_LIST              706
#define XDS_ATTRIBUTE_PROBLEM           707
#define XDS_AVA                         708
#define XDS_COMMON_RESULTS              709
#define XDS_COMMUNICATIONS_ERROR        710
#define XDS_COMPARE_RESULT              711
#define XDS_CONTEXT                     712
#define XDS_CONTINUATION_REF            713
#define XDS_DS_DN                       714
#define XDS_DS_RDN                      715
#define XDS_ENTRY_INFO                  716
#define XDS_ENTRY_INFO_SELECTION        717
#define XDS_ENTRY_MOD                   718
#define XDS_ENTRY_MOD_LIST              719
#define XDS_ERROR                       720
#define XDS_EXT                         721
#define XDS_FILTER                      722
#define XDS_FILTER_ITEM                 723
#define XDS_LIBRARY_ERROR               724
#define XDS_LIST_INFO                   725
#define XDS_LIST_INFO_ITEM              726
#define XDS_LIST_RESULT                 727
#define XDS_NAME                        728
#define XDS_NAME_ERROR                  729
#define XDS_OPERATION_PROGRESS          730
#define XDS_PARTIAL_OUTCOME_QUAL        731
#define XDS_PRESENTATION_ADDRESS        732
#define XDS_READ_RESULT                 733
#define XDS_REFERRAL                    734
#define XDS_RELATIVE_NAME               735
#define XDS_SEARCH_INFO                 736
#define XDS_SEARCH_RESULT               737
#define XDS_SECURITY_ERROR              738
#define XDS_SERVICE_ERROR               739
#define XDS_SESSION                     740
#define XDS_SYSTEM_ERROR                741
#define XDS_UPDATE_ERROR                742

#define XDS_FACSIMILE_PHONE_NBR         801
#define XDS_POSTAL_ADDRESS              802
#define XDS_SEARCH_CRITERION            803
#define XDS_SEARCH_GUIDE                804
#define XDS_TELETEX_TERM_IDENT          805
#define XDS_TELEX_NBR                   806

#define XDS_DL_SUBMIT_PERMS             801

#define XDS_ALGORITHM_IDENT		        821
#define XDS_CERTIFICATE                 822
#define XDS_CERTIFICATE_LIST            823
#define XDS_CERTIFICATE_PAIR            824
#define XDS_CERTIFICATE_SUBLIST         825
#define XDS_SIGNATURE                   826

#define XDS_GDS_SESSION                 0
#define XDS_GDS_CONTEXT                 1
#define XDS_GDS_ACL                     2
#define XDS_GDS_ACL_ITEM                3


typedef D2_name_string  xds_dist_name;
typedef xds_dist_name   xds_rdn;

typedef Psap_string     xds_psap;

typedef D2_acc_point    xds_acc_point;  /* QUEUE OF ACCESS POINTS */
       
typedef D2_a_info       xds_attribute;  /* Only one instance of D2_a_info    */
                                        /* Next pointer of D2_a_info is NULL */

typedef D2_a_info       xds_attr_list;  /* queue of D2_a_info structure */

typedef D2_c_res        xds_common_res;

typedef D2_ava          xds_ava;

typedef struct {
     xds_dist_name     dist_name;       /* d21_6dist_name of d21_6  */
     Bool              match;           /* d21_6match of d21_6      */
     Bool              from_entry;      /* d21_6from_entry of d21_6 */
     xds_common_res  * com_res;         /* d21_6cres of d21_p6      */
}  xds_compare_result;

typedef D2_extension        xds_extension;

typedef D2_op_progress      xds_op_progress;

typedef D2_c_arg            xds_context;

typedef D2_ref_inf          xds_cont_ref;       /* Continuation Reference */
typedef xds_cont_ref        xds_referral;

typedef D2_ei_info          xds_entry_info;     /* D2_ei_info in results    */
                                                /* will be generated by DUA */

typedef D2_req_info         xds_entry_info_sel;

typedef D2_a_modinf         xds_entry_mod;          /* D2_m_next is NULL */

typedef D2_a_modinf         xds_entry_mod_list;

typedef struct xds_fi_item {
     struct xds_fi_item  *f_item_next;
     OM_sint	          match_type;  /* EQ GE LE PRESENT SUBSTR APPROX */
     xds_attribute        f_item_att;
     D2_a_value           f_ini_sstr;
     D2_a_value           f_lst_sstr;
} xds_fi_item;

typedef struct xds_filter {
    xds_fi_item        *fil_item;       /* NULL if no Filter-Item     */
    struct xds_filter  *filter_first;   /* NULL if no further Filters */
    struct xds_filter  *filter_next;    /* NULL if no further Filters */
    OM_sint            fi_type;        /* AND OR NOT                 */
} xds_filter;

typedef struct  {
    xds_cont_ref      * cont_ref;      /* d21_4rf of d21_p4           */
    OM_sint            limit_prob;    /* d21_4lim_prob of d21_p4     */
    Bool                unav_crit_ext; /* d21_4uav_crit_ext of d21_p4 */
} xds_part_oq;

typedef D2_l_info             xds_list_item;

typedef struct {
    xds_dist_name       t_object;       /* d21_4dist_name of d21_p4 */
    xds_part_oq       * l_part_oq;      /* NULL if not-defined      */
    xds_list_item     * subordinates;   /* NULL => no subordinates  */
    xds_common_res    * com_res;        /* d21_4cres of d21_p4      */
} xds_list_info;

typedef xds_list_info       xds_list_res; /* uncorrelated_list_info  */
                                          /* is not supported        */

typedef struct {
    xds_entry_info    *  read_info;         /* d21_3res in d21_p3  */
    xds_common_res    *  com_res;           /* d21_3cres in d21_p3 */
} xds_read_result;

typedef  struct {
    xds_entry_info    * search_entries;   /* d21_5res of d21_p5               */
    xds_dist_name       b_object;         /* b_object.no_rdn=0 if not-defined */
                                          /* d21_5dist_name of d21_p5         */
    xds_part_oq       * s_part_oq;        /* NULL if not-defined              */
                                          /* d21_5rf of d21_p5                */
                                          /* d21_5lim_prob of d21_p5          */
                                          /* d21_5uav_crit_ext of d21_p5      */
    xds_common_res    * com_res;          /* d21_5cres of d21_p5              */
} xds_search_info;

typedef xds_search_info       xds_search_result;/* uncorrelated_search_info  */
                                                /* is not supported          */

typedef struct {
    xds_psap              dsa_address;  /* d21_1addr_dsa of d21_p1          */
    xds_dist_name         dsa_name;     /* d21_1name_dsa of d21_p1          */
    xds_dist_name         requestor;    /* part of d21_1u_credent of d21_p1 */
    D2_password           password;     /* Private extension part of        */
                                        /* d21_1u_credent of d21_p1         */
    OM_sint               dir_id;       /* Private extension (1 is default) */
                                        /* d21_1dir_id of d21_p1            */
    OM_sint               auth_mech;    /* authentication mech. (default 0) */
                                        /* d21_1auth_mech of d21_p1         */
    D2_str                auth_info;    /* authentication info (future use) */
} xds_session;


/*
   In case of a directory-error, xds_error structure is allocated and
   private object for error (i.e. OM_descriptor pointing to xds_error) is
   created. Pointer to this OM_descriptor is assigned to DS_status
   and returned at XDS.
*/

typedef union {
    /* Kind of problems (i.e. serv_prob, sec_prob, upd_prob, att_prob, */
    /* referral, name_prob, sys_prob, lib_prob, com_prob) are          */
    /* identified from the obj_type of the xds_private_object          */

        OM_sint        val;            /* D2_pbhead has to be interpreted to */
                                        /* map it to XDS-errors for each      */
                                        /* class of problem                   */

        D2_error     *  ptr;            /* returned in case of att-,name-, */
                                        /* referral-error                  */
}  xds_error;


typedef Octet_string    xds_acl;
typedef Octet_string    xds_acl_item;

typedef struct {
        OM_sint         ai_len;         /* access info length (with interp) */
        OM_sint         ai_inter;       /* interpretation */
        char            *ai_p;          /* pointer to dname string */
} Aci_info;

typedef struct {
        OM_sint         ar_val_nb;      /* number of significant values */
        OM_sint         ar_len;         /* access right string length */
        char            *ar_p;          /* pointer to first acl_item char */
        Aci_info        ar_info[D2_NOIDX_ACL];
} Acr_info;

typedef struct {
        Acr_info        al_p[D2_NOACCESS_RIGHTS];        /* Acr array */
} Acl_info;


typedef D2_guide        xds_search_guide;
typedef D2_criteria     xds_search_criterion;
typedef D2_telex_number xds_telex_number;
typedef D2_faxnumber    xds_faxnumber;
typedef D2_str_list     xds_str_list;
typedef xds_str_list    xds_postal_address;
typedef D2_ttxid        xds_ttxid;

typedef D2_OR_address   xds_OR_address;
typedef D2_OR_name      xds_OR_name;
typedef D2_dl_submit_perm xds_dl_submit_perm;


typedef Alg_id	xds_algorithm_ident;
typedef Certificate xds_certificate;
typedef Cert_pair xds_certificate_pair;
typedef Cert_list xds_certificate_list;
typedef Rev_certs xds_certificate_sublist;
typedef Rev_cert_body xds_certificate_subsublist;

typedef struct xds_UTC_time_string{
    struct xds_UTC_time_string		*next;
	UTC_time_string					value;
} xds_UTC_time_string;

#endif /* _XDSIAPL_H */
