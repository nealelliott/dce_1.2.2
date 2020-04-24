/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ids_mvr.h,v $
 * Revision 1.1.10.2  1996/02/18  23:34:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:19  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:20  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:50:28  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:15:01  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:36:14  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:48:18  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:14:24  marrek
 * 	Remove Header string.
 * 	[1993/08/03  09:16:00  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/29  12:31:07  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:12:07  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:09:07  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:04:00  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:00:46  melman]
 * 
 * $EndLog$
 */
#ifndef _IDS_MVR_H
#define _IDS_MVR_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char ids_mvr_rcsid[] = "@(#)$RCSfile: ids_mvr.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:34:51 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*
 * Module:
 *	IDS
 * Description:
 *	This file contains:
 *	. data structures directly handled by the operations of the IDS module
 * Reference:
 *	ISO 9594 The Directory
 *		       (Geneva, March 1988)
 *	THORN:  Last years specifications,
 *		Definition of the procedural interface.
 *		Document: INRIA-17, Revision: 3
 */

#include <DUA/std.h>

#define IDS_UNDEFINED	-1

/****************************************************************************
 * general data structures.
 ****************************************************************************/
typedef struct {
        Boolean         psel_defined;
        octet_string    psel;
        Boolean         ssel_defined;
        octet_string    ssel;
        Boolean         tsel_defined;
        octet_string    tsel;
        int             nb_nsap;
        octet_string    * nsap;
} ids_psap;

typedef struct {
	int		nb;
	unsigned char   * contents;
} ids_octet_string;

#define ids_bit_string ids_octet_string   /* for bit strings, nb represents
					   * the number of defined bits     */
#define ids_utc_time 	ids_octet_string  /* the ``contents'' field represents
					   * the Value field of the ASN1
					   * encoding  */
#define ids_att_type 	asn1_oid

typedef asn1_field	ids_att_value;

typedef struct {
	ids_att_type 	type;
	int 		nb_values;
	ids_att_value 	* values;
} ids_att;

typedef struct {
	ids_att_type 	type;
	ids_att_value 	value;
} ids_assertion;

typedef struct {
	int 		nb_assertions;
	ids_assertion 	* assertions;
} ids_rdname;

typedef struct {
	int 		nb_rdnames;
	ids_rdname 	* rdnames;
} ids_dist_name;

typedef ids_dist_name ids_name;

typedef struct {
	int		options;
				/* default: options = 0 */
	int 		priority;
				/* default: priority   = MEDIUM_PRIOR */
	Boolean		time_limit_defined;
	int 		time_limit;
	Boolean 	size_limit_defined;
	int 		size_limit;
	Boolean		scope_ref_defined;
	int 		scope_ref;
} ids_serv_controls;

typedef struct {
	int	ident;
	Boolean critical;
			/* default: critical = FALSE	*/
	asn1_field	item;
} ids_extension;

typedef struct {
	int 	name_res_ph;
	Boolean	next_rdn_defined;
	int	next_rdn;
} ids_op_prog;

typedef struct {
	ids_serv_controls	controls;
		/* default:   controls.options.nb = 0,
			      controls.priority   = MEDIUM_PRIOR,
			      controls.time_limit_defined = FALSE,
			      controls.size_limit_defined = FALSE,
			      controls.scope_ref_defined = FALSE
		   security parameters not defined yet	*/
	Boolean			requestor_defined;
	ids_dist_name 		requestor;
	ids_op_prog		op_prog;
		/* default: op_prog.name_res_ph = NOTSTARTED */
	Boolean			alias_rdn_defined;
	int			alias_rdn;
	Boolean			extensions_defined;
	int			nb_extensions;
	ids_extension		* extensions;
} ids_common_arg;

typedef struct {
	/* security parameters not defined yet	*/
	Boolean			requestor_defined;
	ids_dist_name 		requestor;
	Boolean			alias_deref;
				  /* default: alias_reref = FALSE  */
} ids_common_res;

typedef struct {
	Boolean		is_all_att;
			  /* default: is_all_att = TRUE            */
	int		nb_types;
	ids_att_type	 * types;
			  /* consistent only if is_all_att = FALSE */
	int 		info_type_mode;
			  /* default: info_type_mode = 1           */
} ids_entry_info_sel;

typedef struct {
	Boolean type_only;
	union {
		ids_att_type 	type;
		ids_att 	attribute;
	} contents;
} ids_info;

typedef struct {
	ids_dist_name 	dist_name;
	Boolean 	from_entry;
			  /* default: from_entry = TRUE */
	Boolean		infos_defined;
	int 		nb_info;
	ids_info 	* infos;
} ids_entry_info;

typedef struct {
	int 		mode;
	ids_att_value	value;
} ids_pattern;
	/* mode = 0 : initial,
	 * mode = 1 : any,
	 * mode = 2 : final
	 */

typedef struct {
	ids_att_type 	 type;
	int	    	 nb_pattern;
	ids_pattern	 * patterns;
} ids_substring;

typedef struct {
	int mode;	/* 0: equal,  1: substring, 2: gequal,
			   3: lequal, 4: present,   5: approx_match */
	union {
			/*??? the field names should have been
			 *??? factorized !!!		*/
		ids_assertion   equal;
		ids_substring 	substring;
		ids_assertion   gequal,
				lequal;
		ids_att_type 	present;
		ids_assertion 	approx_match;
	} value;
} ids_filter_item;

typedef struct {
	int			nb_filters;
	struct 	ids_filter   	* filters;
} ids_filter_blk;

typedef struct ids_filter {
	int 	mode;		/* 0: filteritem,
				   1: and,
				   2: or,
				   3: not */
	union {
		ids_filter_item fil_item;
		ids_filter_blk 	fil_and,
		  		fil_or;
		struct ids_filter * fil_not;
	} value;
} ids_filter;

typedef struct {
	ids_name 	name;
	ids_psap     	addr;
} ids_acc_pt;

typedef struct {
	ids_dist_name	ctx_prefix;
	ids_acc_pt	acc_pt;
} ids_cross_ref;

typedef struct {
	ids_name	dsa;
	ids_name	target;
	ids_op_prog	op_prog;
} ids_trace_info;

typedef struct {
	int		nb;
	ids_trace_info	* list;
} ids_trace_infos;

typedef struct {
	ids_name	target;
	Boolean		alias_rdn_defined;
	int		alias_rdn;
	ids_op_prog 	op_prog;
	Boolean		rdn_resolved_defined;
	int		rdn_resolved;
	Boolean		ref_type_defined;
	int		ref_type;
	int		nb_acc_pt;
	ids_acc_pt	* acc_pts;
} ids_cont_reference;

typedef struct {
	ids_dist_name	originator;
	ids_dist_name	target;
	ids_op_prog	op_prog;
		/* default: op_prog.name_res_ph = NOTSTARTED */
	ids_trace_infos	trace_infos;
	Boolean		alias_deref;
		/* default: alias_deref = FALSE */
	int		aliased_rdn;
		/* consistent only when alias_deref is TRUE */
	Boolean		entry_only;
		/* default: entry_only = FALSE */
	Boolean		rt_cross_refs;
		/* default: rt_cross_refs is FALSE */
	int		ref_type;
		/* default: ref_type = SUPERIOR */
	asn1_field	domain_info;
	ids_utc_time	time_limit;
} ids_ch_arg;

typedef struct {
	asn1_field	domain_info;
	int		nb_cross_ref;
	ids_cross_ref	* cross_refs;
} ids_ch_res;

/****************************************************************************
 * data structures associated to the operations of the directory.
 ****************************************************************************/
/*
 * types associated to the operation  "bind"
 */
typedef struct {
	Boolean			time1_defined, time2_defined;
	ids_utc_time		time1, time2;
	Boolean			random1_defined, random2_defined;
	ids_bit_string		random1, random2;
} ids_validity;

typedef struct {
	ids_dist_name 		name;
	Boolean			validity_defined;
	ids_validity		validity;
	Boolean			password_defined;
	ids_octet_string    	password;
} ids_simple_cred;

typedef struct {
	Boolean		cred_defined;
	int 		cred_kind;  /* 0: simple,
				     * 1: strong,
				     * 2: external */
	union {
		ids_simple_cred 	simple;
		asn1_field		external;
		  /* strong credential handled extra */
	} credentials;
	int	version;
		  /* default: version =  V1988_DUA */
} ids_bind_arg;

typedef ids_bind_arg ids_bind_res;

/*
 * types associated to the operation  "read"
 */
typedef struct {
	ids_name 		object;
	ids_entry_info_sel	selection;
		/* default: selection.is_all_att = TRUE
			    selection.info_type_mode = 1	*/
} ids_read_arg;

typedef struct {
	ids_entry_info 	entry_info;
} ids_read_res;

/*
 * types associated to the operation  "compare"
 */
typedef struct {
	ids_name 	object;
	ids_assertion 	purported;
} ids_compare_arg;

typedef struct {
	Boolean		dname_defined;
	ids_dist_name 	dname;
	Boolean 	matched;
	Boolean 	from_entry;
			  /* default: from_entry = TRUE 	*/
} ids_compare_res;

/*
 * types associated to the operation  "list"
 */
typedef struct {
	ids_name 	object;
} ids_list_arg;

typedef struct {
	ids_rdname 	rdname;
	Boolean    	alias_entry;
			  /* default: alias_entry = FALSE */
	Boolean		from_entry;
			  /* default: from_entry = TRUE   */
} ids_subordinate;

typedef struct {
	Boolean			limit_pb_defined;
	int			limit_pb;
	Boolean			unexplored_defined;
	int			nb_unexplored;
	ids_cont_reference	* unexplored;
	Boolean			un_crit_ext;
		/* default: un_crit_ext = FALSE	*/
} ids_part_oq;

typedef struct {
	Boolean			dname_defined;
	ids_dist_name		dname;
	int			nb_subordinate;
	ids_subordinate		* subordinates;
	Boolean			part_oq_defined;
	ids_part_oq		part_oq;
} ids_list_info;

typedef struct ids_list_res_list {
        int                     nb;
        struct ids_list_res    * list;
} ids_list_res_list;

typedef struct ids_list_res {
	Boolean 	uncorrelated;
	union {
		ids_list_res_list uncorr_list_info;
		ids_list_info	list_info;
	} info;
} ids_list_res;

/*
 * types associated to the operation  "search"
 */
typedef struct {
	ids_name 	base_object;
	int 		subset_mode;
		/* default: subset_mode = BASE_OBJECT	 */
	ids_filter 	filter;
		/* default: filter.mode = 1,
			    filer.value.fil_and.nb_filters = 0 */
	Boolean		alias;
		/* default: alias = TRUE		 */
	ids_entry_info_sel 	selection;
		/* default: selection.is_all_att = TRUE
			    selection.info_type_mode = 1	*/
} ids_search_arg;

typedef struct {
	Boolean			dname_defined;
	ids_dist_name		dname;
	int			nb_info;
	ids_entry_info		* infos;
	Boolean			part_oq_defined;
	ids_part_oq		part_oq;
} ids_search_info;

typedef struct ids_search_res_list {
        int                     nb;
        struct ids_search_res  * list;
} ids_search_res_list;

typedef struct ids_search_res {
	Boolean 	uncorrelated;
	union {
		ids_search_res_list	uncorr_search_info;
		ids_search_info		search_info;
	} info;
} ids_search_res;

/*
 * types associated to the operation  "add"
 */
typedef struct {
	ids_dist_name 	object;
	int		nb_att;
	ids_att 	* atts;
} ids_add_arg;

/*
 * types associated to the operation  "remove"
 */
typedef struct {
	ids_dist_name 	object;
} ids_remove_arg;

/*
 * types associated to the operation  "modify"
 */
typedef struct {
	int 	modif_kind;	/* 0: add_att, 1: rm_att, 2: add_val, 3: rm_val */
	union {
		ids_att 	add_att;
		ids_att_type 	rm_att;
		ids_att 	add_val;
		ids_att 	rm_val;
	} modif;
} ids_entry_modif;

typedef struct {
	ids_dist_name 	 object;
	int		 nb_change;
	ids_entry_modif  * changes;
} ids_modify_arg;

typedef struct {
	ids_dist_name 	object;
	ids_rdname 	new_rdn;
	Boolean         delet;
			  /* default: delet = FALSE */
} ids_modify_rdn_arg;

typedef struct {
	int	op_code;
	union {
		ids_read_arg		read;
		ids_compare_arg 	comp;
		int			aban;
		ids_list_arg		list;
		ids_search_arg  	search;
		ids_add_arg		add;
		ids_remove_arg  	remove;
		ids_modify_arg 	 	mod;
		ids_modify_rdn_arg 	mod_rdn;
	} contents;
} ids_arg;

typedef struct {
	int	op_code;
	union {
		ids_read_res	read;
		ids_compare_res comp;
		ids_list_res	list;
		ids_search_res  search;
	} contents;
	ids_common_res		common_res;
} ids_res;

/****************************************************************************
 * data structures associated to the error types.
 ****************************************************************************/
typedef struct {
	int		att_pb;
	ids_att_type	att_type;
	Boolean		att_value_defined;
	ids_att_value	att_value;
} ids_att_pb;

typedef struct {
	ids_name	object;
	int		nb_pb;
	ids_att_pb    * att_pb_list;
} ids_att_error;

typedef struct {
	int		pb;
	ids_name	matched;
} ids_name_error;

typedef struct {
	ids_cont_reference	candidate;
} ids_referral;

typedef struct {
	ids_cont_reference	candidate;
	ids_dist_name		ctx_pref;
	ids_trace_infos		trace_infos;
	Boolean			trace_infos_defined;
} ids_dsa_referral;

typedef struct {
	int pb;
} ids_sec_error;

typedef struct {
	int pb;
} ids_serv_error;

typedef struct {
	int pb;
} ids_upd_error;

typedef struct {
	int	pb;
	int	inv_id;
} ids_aban_failed;

typedef struct {
	int 	version;
	  /* default: version = V1988_DUA */
	Boolean 	is_service_error;
	union {
		ids_serv_error	serv_pb;
		ids_sec_error	sec_pb;
	} pb;
} ids_bind_error;

typedef struct {
	int	error_code;
	union {
		ids_att_error	att;
		ids_name_error	name;
		ids_serv_error	serv;
		ids_referral	ref;
		ids_sec_error	sec;
		ids_aban_failed	aban_failed;
		ids_upd_error	upd;
		ids_bind_error  bind;
	} contents;
} ids_error;

/*
 * Definition of the code of the operations, for the Directory Access Protocol
 * cf. X.519, The Directory - Protocols Specifications (Geneva 1988)
 *     annex A
 */
#define READ_OP 	1
#define COMPARE_OP 	2
#define ABANDON_OP	3
#define LIST_OP		4
#define SEARCH_OP 	5
#define ADD_OP 		6
#define REMOVE_OP	7
#define MODIFY_OP	8
#define MODIFY_RDN_OP 	9

/*
 * Definition of the code of the operations, for the Directory System Protocol
 * cf. X.519, The Directory - Protocols Specifications (Geneva 1988)
 *     annex B
 */
#define CH_READ_OP 		1
#define CH_COMPARE_OP 		2
#define CH_ABANDON_OP		3
#define CH_LIST_OP		4
#define CH_SEARCH_OP 		5
#define CH_ADD_OP 		6
#define CH_REMOVE_OP		7
#define CH_MODIFY_OP		8
#define CH_MODIFY_RDN_OP 	9

/*
 * Definition of the error code, for the Directory Access Protocol.
 * cf. X.519, The Directory - Protocols Specifications (Geneva 1988)
 *     annex A
 */
#define ACC_ATT_ERROR		1
#define ACC_NAME_ERROR		2
#define ACC_SERV_ERROR		3
#define ACC_REFERRAL		4
#define ACC_ABAN_ERROR 		5
#define ACC_SEC_ERROR		6
#define ACC_ABAN_FAILED		7
#define ACC_UPDATE_ERROR	8

/*
 * Definition of the error code, for the Directory Access Protocol.
 * cf. X.519, The Directory - Protocols Specifications (Geneva 1988)
 *     annex B
 */
#define SYS_ATT_ERROR		1
#define SYS_NAME_ERROR		2
#define SYS_SERV_ERROR		3
#define SYS_ABAN_ERROR 		5
#define SYS_SEC_ERROR		6
#define SYS_ABAN_FAILED		7
#define SYS_UPDATE_ERROR	8
#define SYS_DSA_REFERRAL	9

/*
 * Definition of the parameters associated to the error code.
 * cf. X.511, The Directory - Abstract Service Definition (Geneva 1988)
 *     annex A
 */
/* limit problem 	*/
#define TIME_LIMIT_EXC_LM_PB		0
#define SIZE_LIMIT_EXC			1
#define ADM_SIZE_LIMIT_EXC_LM_PB	2

/* the pattern.mode     */
#define PATTERN_INITIAL         0
#define PATTERN_ANY             1
#define PATTERN_FINAL           2

/* the filter_item.mode         */
#define FILTER_ITEM_EQUAL            0
#define FILTER_ITEM_SUBSTRING        1
#define FILTER_ITEM_GREATER_EQUAL    2
#define FILTER_ITEM_LESS_EQUAL       3
#define FILTER_ITEM_PRESENT          4
#define FILTER_ITEM_APPROXIMATE      5

/* the filter.mode      */
#define FILTER_ITEM             0
#define FILTER_AND              1
#define FILTER_OR               2
#define FILTER_NOT              3

/* the credential type          */
#define CREDENTIAL_SIMPLE       0
#define CREDENTIAL_STRONG       1
#define CREDENTIAL_EXTERNAL     2

/* the entry-modify type        */
#define ENTRYMOD_ADD_ATT        0
#define ENTRYMOD_REMOV_ATT      1
#define ENTRYMOD_ADD_VAL        2
#define ENTRYMOD_REMOV_VAL      3

#endif /* _IDS_MVR_H */
