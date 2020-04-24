/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_attr_tools.h,v $
 * Revision 1.1.9.3  1996/02/18  23:00:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:15  marty]
 *
 * Revision 1.1.9.2  1995/12/08  17:28:55  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/28  19:51 UTC  mdf
 * 	Resolve CHFts14561
 * 	[1995/04/13  21:34 UTC  mdf  /main/mdf_mothra_defects_3/1]
 * 
 * Revision 1.1.5.1  1994/10/17  18:18:12  cuti
 * 	add schema entry semantic check macros, fix SA_SCH_TRIG_FLAG_IS_NONE
 * 	[1994/10/14  16:15:27  cuti]
 * 
 * Revision 1.1.2.5  1994/08/04  16:12:49  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:09:36  mdf]
 * 
 * Revision 1.1.2.3  1994/06/17  18:42:09  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:45  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/11  19:07:55  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  21:01:29  ahop]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */
/*
 * sec_attr_tools.h: macros and utilities useful for
 *                   consumers of the sec_attr interfaces
 */

#ifndef sec_attr_tools_h__included
#define sec_attr_tools_h__included

#include <dce/rpcbase.h>


/* BINDING INFO field access macros - used by sec_attr_schema_entry_t
 * and sec_attr_t field access macros below.
 *
 * B is a pointer to a sec_attr_bind_info_t.
 */
#define SA_BND_AUTH_INFO(B)		(B)->auth_info
#define SA_BND_AUTH_INFO_TYPE(B)	(SA_BND_AUTH_INFO(B)).info_type

#define SA_BND_AUTH_DCE_INFO(B)		\
	(SA_BND_AUTH_INFO(B)).tagged_union.dce_info

#define SA_BND_AUTH_SVR_PNAME_P(B)	\
	(SA_BND_AUTH_DCE_INFO(B)).svr_princ_name

#define SA_BND_AUTH_PROT_LEVEL(B)	\
	(SA_BND_AUTH_DCE_INFO(B)).protect_level

#define SA_BND_AUTH_AUTHN_SVC(B)	\
	(SA_BND_AUTH_DCE_INFO(B)).authn_svc

#define SA_BND_AUTH_AUTHZ_SVC(B)	\
	(SA_BND_AUTH_DCE_INFO(B)).authz_svc

#define SA_BND_NUM(B)			(B)->num_bindings
#define SA_BND_ARRAY(B,I)		(B)->bindings[I]
#define SA_BND_TYPE(B,I)		(SA_BND_ARRAY(B,I)).bind_type

#define SA_BND_STRING_P(B,I)		\
	(SA_BND_ARRAY(B,I)).tagged_union.string_binding

#define SA_BND_SVRNAME_P(B,I)		\
	(SA_BND_ARRAY(B,I)).tagged_union.svrname

#define SA_BND_SVRNAME_SYNTAX(B,I)	\
	(SA_BND_SVRNAME_P(B,I))->name_syntax

#define SA_BND_SVRNAME_NAME_P(B,I)	\
	(SA_BND_SVRNAME_P(B,I))->name

#define SA_BND_TWRSET_P(B,I)		\
	(SA_BND_ARRAY(B,I)).tagged_union.twr_set

#define SA_BND_TWRSET_COUNT(B,I)	(SA_BND_TWRSET_P(B,I))->count
#define SA_BND_TWR_P(B,I,J)		(SA_BND_TWRSET_P(B,I))->towers[J]
#define SA_BND_TWR_LEN(B,I,J)		(SA_BND_TWR_P(B,I,J))->tower_length

#define SA_BND_TWR_OCTETS(B,I,J)	\
	(SA_BND_TWR_P(B,I,J))->tower_octet_string

/* end BINDING_INFO field access macros
 */


/* 
 * SCHEMA ENTRY - field access macros:
 * S is a pointer to a sec_attr_schema_entry_t.
 * I, J are non-negative integers for array element selection.
 */
#define SA_ACL_MGR_SET_P(S)		(S)->acl_mgr_set
#define SA_ACL_MGR_NUM(S)		(SA_ACL_MGR_SET_P(S))->num_acl_mgrs
#define SA_ACL_MGR_INFO_P(S,I)		(SA_ACL_MGR_SET_P(S))->mgr_info[I]
#define SA_ACL_MGR_TYPE(S,I)		(SA_ACL_MGR_INFO_P(S,I))->acl_mgr_type
#define SA_ACL_MGR_PERMS_QUERY(S,I)	(SA_ACL_MGR_INFO_P(S,I))->query_permset
#define SA_ACL_MGR_PERMS_UPDATE(S,I)	(SA_ACL_MGR_INFO_P(S,I))->update_permset
#define SA_ACL_MGR_PERMS_TEST(S,I)	(SA_ACL_MGR_INFO_P(S,I))->test_permset
#define SA_ACL_MGR_PERMS_DELETE(S,I)	(SA_ACL_MGR_INFO_P(S,I))->delete_permset
#define SA_TRG_BND_INFO_P(S)		(S)->trig_binding

#define SA_TRG_BND_AUTH_INFO(S)		\
	(SA_BND_AUTH_INFO(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_INFO_TYPE(S)	\
	(SA_BND_AUTH_INFO_TYPE(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_DCE_INFO(S)	\
	(SA_BND_AUTH_DCE_INFO(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_SVR_PNAME_P(S)	\
	(SA_BND_AUTH_SVR_PNAME_P(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_PROT_LEVEL(S)	\
	(SA_BND_AUTH_PROT_LEVEL(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_AUTHN_SVC(S)	\
	(SA_BND_AUTH_AUTHN_SVC(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_AUTHZ_SVC(S)	\
	(SA_BND_AUTH_AUTHZ_SVC(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_NUM(S)		\
	(SA_BND_NUM(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_ARRAY(S,I)		\
	(SA_BND_ARRAY((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_TYPE(S,I)		\
	(SA_BND_TYPE((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_STRING_P(S,I)		\
	(SA_BND_STRING_P((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_SVRNAME_P(S,I)		\
	(SA_BND_SVRNAME_P((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_SVRNAME_SYNTAX(S,I)      \
        (SA_BND_SVRNAME_SYNTAX((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_SVRNAME_NAME_P(S,I)      \
        (SA_BND_SVRNAME_NAME_P((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_TWRSET_P(S,I)		\
	(SA_BND_TWRSET_P((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_TWRSET_COUNT(S,I)	\
	(SA_BND_TWRSET_COUNT((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_TWR_P(S,I,J)		\
	(SA_BND_TWR_P((SA_TRG_BND_INFO_P(S)),I,J))

#define SA_TRG_BND_TWR_LEN(S,I,J)	\
	(SA_BND_TWR_LEN((SA_TRG_BND_INFO_P(S)),I,J))

#define SA_TRG_BND_TWR_OCTETS(S,I,J)	\
	(SA_BND_TWR_OCTETS((SA_TRG_BND_INFO_P(S)),I,J))

/* END SCHEMA ENTRY field access macros. */


/* 
 * ATTRIBUTE - field access macros:
 * S is a pointer to a sec_attr_t.
 * I, J are non-negative integers for array element selection.
 */
#define SA_ATTR_ID(S)			(S)->attr_id
#define SA_ATTR_VALUE(S)		(S)->attr_value
#define SA_ATTR_ENCODING(S)		(SA_ATTR_VALUE(S)).attr_encoding

#define SA_ATTR_INTEGER(S)	\
	(SA_ATTR_VALUE(S)).tagged_union.signed_int

#define SA_ATTR_PRINTSTRING_P(S)	\
	(SA_ATTR_VALUE(S)).tagged_union.printstring

#define SA_ATTR_STR_ARRAY_P(S)	\
	(SA_ATTR_VALUE(S)).tagged_union.string_array

#define SA_ATTR_STR_ARRAY_NUM(S)	(SA_ATTR_STR_ARRAY_P(S))->num_strings
#define SA_ATTR_STR_ARRAY_ELT_P(S,I)	(SA_ATTR_STR_ARRAY_P(S))->strings[I]

#define SA_ATTR_BYTES_P(S)	\
	(SA_ATTR_VALUE(S)).tagged_union.bytes

#define SA_ATTR_BYTES_LEN(S)		(SA_ATTR_BYTES_P(S))->length
#define SA_ATTR_BYTES_DATA(S,I)		(SA_ATTR_BYTES_P(S))->data[I]

#define SA_ATTR_IDATA_P(S)	\
	(SA_ATTR_VALUE(S)).tagged_union.idata

#define SA_ATTR_IDATA_CODESET(S)	(SA_ATTR_IDATA_P(S))->codeset
#define SA_ATTR_IDATA_LEN(S)		(SA_ATTR_IDATA_P(S))->length
#define SA_ATTR_IDATA_DATA(S,I)		(SA_ATTR_IDATA_P(S))->data[I]

#define SA_ATTR_UUID(S)	\
	(SA_ATTR_VALUE(S)).tagged_union.uuid

#define SA_ATTR_SET_P(S)	\
	(SA_ATTR_VALUE(S)).tagged_union.attr_set

#define SA_ATTR_SET_NUM(S)		(SA_ATTR_SET_P(S))->num_members
#define SA_ATTR_SET_MEMBERS(S,I)	(SA_ATTR_SET_P(S))->members[I]

#define SA_ATTR_BND_INFO_P(S)	\
	(SA_ATTR_VALUE(S)).tagged_union.binding

#define SA_ATTR_BND_AUTH_INFO(S)	\
	(SA_BND_AUTH_INFO(SA_ATTR_BND_INFO_P(S)))

#define SA_ATTR_BND_AUTH_INFO_TYPE(S)	\
	(SA_BND_AUTH_INFO_TYPE(SA_ATTR_BND_INFO_P(S)))

#define SA_ATTR_BND_AUTH_DCE_INFO(S)	\
	(SA_BND_AUTH_DCE_INFO(SA_ATTR_BND_INFO_P(S)))

#define SA_ATTR_BND_AUTH_SVR_PNAME_P(S)	\
	(SA_BND_AUTH_SVR_PNAME_P(SA_ATTR_BND_INFO_P(S)))

#define SA_ATTR_BND_AUTH_PROT_LEVEL(S)	\
	(SA_BND_AUTH_PROT_LEVEL(SA_ATTR_BND_INFO_P(S)))

#define SA_ATTR_BND_AUTH_AUTHN_SVC(S)	\
	(SA_BND_AUTH_AUTHN_SVC(SA_ATTR_BND_INFO_P(S)))

#define SA_ATTR_BND_AUTH_AUTHZ_SVC(S)	\
	(SA_BND_AUTH_AUTHZ_SVC(SA_ATTR_BND_INFO_P(S)))

#define SA_ATTR_BND_NUM(S)		\
	(SA_BND_NUM(SA_ATTR_BND_INFO_P(S)))

#define SA_ATTR_BND_ARRAY(S,I)		\
	(SA_BND_ARRAY((SA_ATTR_BND_INFO_P(S)),I))

#define SA_ATTR_BND_TYPE(S,I)		\
	(SA_BND_TYPE((SA_ATTR_BND_INFO_P(S)),I))

#define SA_ATTR_BND_STRING_P(S,I)		\
	(SA_BND_STRING_P((SA_ATTR_BND_INFO_P(S)),I))

#define SA_ATTR_BND_SVRNAME_P(S,I)	\
	(SA_BND_SVRNAME_P((SA_ATTR_BND_INFO_P(S)),I))

#define SA_ATTR_BND_SVRNAME_SYNTAX(S,I)      \
        (SA_BND_SVRNAME_SYNTAX((SA_ATTR_BND_INFO_P(S)),I))

#define SA_ATTR_BND_SVRNAME_NAME_P(S,I)      \
        (SA_BND_SVRNAME_NAME_P((SA_ATTR_BND_INFO_P(S)),I))

#define SA_ATTR_BND_TWRSET_P(S,I)		\
	(SA_BND_TWRSET_P((SA_ATTR_BND_INFO_P(S)),I))

#define SA_ATTR_BND_TWRSET_COUNT(S,I)	\
	(SA_BND_TWRSET_COUNT((SA_ATTR_BND_INFO_P(S)),I))

#define SA_ATTR_BND_TWR_P(S,I,J)		\
	(SA_BND_TWR_P((SA_ATTR_BND_INFO_P(S)),I,J))

#define SA_ATTR_BND_TWR_LEN(S,I,J)	\
	(SA_BND_TWR_LEN((SA_ATTR_BND_INFO_P(S)),I,J))

#define SA_ATTR_BND_TWR_OCTETS(S,I,J)	\
	(SA_BND_TWR_OCTETS((SA_ATTR_BND_INFO_P(S)),I,J))

#define SA_ATTR_STATUS(S)	\
	(SA_ATTR_VALUE(S)).tagged_union.status

/* END ATTRIBUTE field access macros. */


/* 
 * BINDING INFO - Data Structure Size Calculation Macros 
 */

/*
 * SA_BND_INFO_SIZE(N) - calculate the size required
 * for a sec_attr_bind_info_t with N bindings.
 */
#define SA_BND_INFO_SIZE(N) ( sizeof(sec_attr_bind_info_t) + \
        (((N) - 1) * sizeof(sec_attr_binding_t)) )

/*
 * SA_TWR_SET_SIZE(N) - calculate the size required
 * for a sec_attr_twr_set_t with N towers.
 */
#define SA_TWR_SET_SIZE(N) ( sizeof(sec_attr_twr_set_t) + \
        (((N) - 1) * sizeof(sec_attr_twr_ref_t)) )

/*
 * SA_TWR_SIZE(N) - calculate the size required
 * for a twr_t with a tower_octet_string of length N.
 */
#define SA_TWR_SIZE(N) ( sizeof(twr_t) + (N) - 1 )

/* END BINDING_INFO - Data Structure Size Calculation Macros */


/* 
 * SCHEMA ENTRY - Data Structure Size Calculation Macros 
 */

/*
 * SA_ACL_MGR_SET_SIZE(N) - calculate the size required
 * for a sec_attr_acl_mgr_info_set_t with N acl_mgrs.
 */
#define SA_ACL_MGR_SET_SIZE(N) ( sizeof(sec_attr_acl_mgr_info_set_t) + \
        (((N) - 1) * sizeof(sec_attr_acl_mgr_info_p_t)) )

/* END SCHEMA ENTRY - Data Structure Size Calculation Macros */


/* 
 * ATTRIBUTE - Data Structure Size Calculation Macros 
 */

/*
 * SA_ATTR_STR_ARRAY_SIZE(N) - calculate the size required
 * for a sec_attr_enc_str_array_t with N sec_attr_enc_printstring_p_t-s.
 */
#define SA_ATTR_STR_ARRAY_SIZE(N) ( sizeof(sec_attr_enc_str_array_t) + \
        (((N) - 1) * sizeof(sec_attr_enc_printstring_p_t)) )

/*
 * SA_ATTR_BYTES_SIZE(N) - calculate the size required
 * for a sec_attr_enc_bytes_t with byte string length of N.
 */
#define SA_ATTR_BYTES_SIZE(N) ( sizeof(sec_attr_enc_bytes_t) + (N) - 1 )

/*
 * SA_ATTR_IDATA_SIZE(N) - calculate the size required
 * for a sec_attr_i18n_data_t with byte string length of N.
 */
#define SA_ATTR_IDATA_SIZE(N) ( sizeof(sec_attr_i18n_data_t) + (N) - 1 )

/*
 * SA_ATTR_SET_SIZE(N) - calculate the size required
 * for a sec_attr_enc_attr_set_t with N members (uuids).
 */
#define SA_ATTR_SET_SIZE(N) ( sizeof(sec_attr_enc_attr_set_t) + \
        (((N) - 1) * sizeof(uuid_t)) )

/* END ATTRIBUTE - Data Structure Size Calculation Macros */


/* 
 * BINDING - Semantic check macros
 */

/*
 * SA_BND_AUTH_INFO_TYPE_VALID(B) - evaluates to TRUE (1)
 * if the binding auth_info type is valid; FALSE (0) otherwise.
 * B is a pointer to a sec_attr_bind_info_t.
 */
#define SA_BND_AUTH_INFO_TYPE_VALID(B)	( \
    (SA_BND_AUTH_INFO_TYPE(B)) == sec_attr_bind_auth_none	|| \
    (SA_BND_AUTH_INFO_TYPE(B)) == sec_attr_bind_auth_dce ? true : false )

/*
 * SA_BND_AUTH_PROT_LEV_VALID(B) - evaluates to TRUE (1)
 * if the binding auth_info protect_level is valid; FALSE (0) otherwise.
 * B is a pointer to a sec_attr_bind_info_t.
 */
#define SA_BND_AUTH_PROT_LEV_VALID(B)	( \
    (SA_BND_AUTH_PROT_LEVEL(B)) == rpc_c_protect_level_default		|| \
    (SA_BND_AUTH_PROT_LEVEL(B)) == rpc_c_protect_level_none		|| \
    (SA_BND_AUTH_PROT_LEVEL(B)) == rpc_c_protect_level_connect		|| \
    (SA_BND_AUTH_PROT_LEVEL(B)) == rpc_c_protect_level_call		|| \
    (SA_BND_AUTH_PROT_LEVEL(B)) == rpc_c_protect_level_pkt		|| \
    (SA_BND_AUTH_PROT_LEVEL(B)) == rpc_c_protect_level_pkt_integ	|| \
    (SA_BND_AUTH_PROT_LEVEL(B)) == rpc_c_protect_level_pkt_privacy ? \
    true : false )

/*
 * SA_BND_AUTH_AUTHN_SVC_VALID(B) - evaluates to TRUE (1)
 * if the binding auth_info authentication service is valid; 
 * FALSE (0) otherwise.
 * B is a pointer to a sec_attr_bind_info_t.
 */
#define SA_BND_AUTH_AUTHN_SVC_VALID(B)	( \
    (SA_BND_AUTH_AUTHN_SVC(B)) == rpc_c_authn_none		|| \
    (SA_BND_AUTH_AUTHN_SVC(B)) == rpc_c_authn_dce_secret	|| \
    (SA_BND_AUTH_AUTHN_SVC(B)) == rpc_c_authn_dce_public	|| \
    (SA_BND_AUTH_AUTHN_SVC(B)) == rpc_c_authn_dce_dummy		|| \
    (SA_BND_AUTH_AUTHN_SVC(B)) == rpc_c_authn_dssa_public	|| \
    (SA_BND_AUTH_AUTHN_SVC(B)) == rpc_c_authn_default ? \
    true : false )

/*
 * SA_BND_AUTH_AUTHZ_SVC_VALID(B) - evaluates to TRUE (1)
 * if the binding auth_info authorization service is valid; 
 * FALSE (0) otherwise.
 * B is a pointer to a sec_attr_bind_info_t.
 */
#define SA_BND_AUTH_AUTHZ_SVC_VALID(B)	( \
    (SA_BND_AUTH_AUTHZ_SVC(B)) == rpc_c_authz_none	|| \
    (SA_BND_AUTH_AUTHZ_SVC(B)) == rpc_c_authz_name	|| \
    (SA_BND_AUTH_AUTHZ_SVC(B)) == rpc_c_authz_dce ? \
    true : false )

/* END BINDING - Semantic check macros */


/* 
 * SCHEMA ENTRY - Semantic check macros 
 * S is a pointer to a sec_attr_schema_entry_t.
 * N is a string pointer
 * E is a sec_attr_encoding_t
 * L is a non-negative integer for max length of a string
 * A is a sec_attr_intercell_action_t
 */

/* Evaluates to true if attr_name (N) is not a NULL pointer, 
 * is not longer than the max length (L), and is not an empty string.
 */
#define SA_SCH_ATTR_NAME_VALID(N,L)	( (N) && \
	strlen((char *)(N)) <= (L) && *(N) != '\0' )

/* The STRING_VALID macro is suitable for checking the schema
 * scope, comment, and binding auth_info server principal name
 * fields which allow NULL or empty string input.  
 * Evaluates to true if string (N) is not a NULL pointer and
 * is not longer than the max length (L) OR if (N) is a NULL ptr.
 */
#define SA_SCH_STRING_VALID(N,L) ( !(N) || strlen((char *)(N)) <= (L) )

/* Evaluates to true if the encoding type (E) is valid for a schema entry.
 * Note: sec_attr_enc_trig_binding is not valid in a schema entry;
 * it's only used in returned attribute instances for triggers.
 */
#define SA_SCH_ENCODING_TYPE_VALID(E)	(	\
    (E) == sec_attr_enc_any			|| \
    (E) == sec_attr_enc_void			|| \
    (E) == sec_attr_enc_integer			|| \
    (E) == sec_attr_enc_printstring		|| \
    (E) == sec_attr_enc_printstring_array	|| \
    (E) == sec_attr_enc_bytes			|| \
    (E) == sec_attr_enc_confidential_bytes	|| \
    (E) == sec_attr_enc_i18n_data		|| \
    (E) == sec_attr_enc_uuid			|| \
    (E) == sec_attr_enc_attr_set		|| \
    (E) == sec_attr_enc_binding			? true : false )

/* Evaluates to true if sec_attr_intercell_action_t (A)
 * is set to a valid value.
 */
#define SA_SCH_INTERCELL_ACTION_VALID(A) ( \
    (A) == sec_attr_intercell_act_accept || \
    (A) == sec_attr_intercell_act_reject || \
    (A) == sec_attr_intercell_act_evaluate ? true : false )


#define SA_TRG_BND_AUTH_INFO_TYPE_VALID(S)	\
	(SA_BND_AUTH_INFO_TYPE_VALID(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_PROT_LEV_VALID(S)	\
	(SA_BND_AUTH_PROT_LEV_VALID(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_AUTHN_SVC_VALID(S)	\
	(SA_BND_AUTH_AUTHN_SVC_VALID(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_AUTHZ_SVC_VALID(S)	\
	(SA_BND_AUTH_AUTHZ_SVC_VALID(SA_TRG_BND_INFO_P(S)))

/* END SCHEMA ENTRY - Semantic check macros */


/* 
 * ATTRIBUTE - Semantic check macros 
 * S is a pointer to a sec_attr_t.
 */

#define SA_ATTR_BND_AUTH_INFO_TYPE_VALID(S)	\
	(SA_BND_AUTH_INFO_TYPE_VALID(SA_ATTR_BND_INFO_P(S)))

#define SA_ATTR_BND_AUTH_PROT_LEV_VALID(S)	\
	(SA_BND_AUTH_PROT_LEV_VALID(SA_ATTR_BND_INFO_P(S)))

#define SA_ATTR_BND_AUTH_AUTHN_SVC_VALID(S)	\
	(SA_BND_AUTH_AUTHN_SVC_VALID(SA_ATTR_BND_INFO_P(S)))

#define SA_ATTR_BND_AUTH_AUTHZ_SVC_VALID(S)	\
	(SA_BND_AUTH_AUTHZ_SVC_VALID(SA_ATTR_BND_INFO_P(S)))

/* END ATTRIBUTE - Semantic check macros */


/* 
 * SCHEMA ENTRY - Flag check macros evaluate
 * to TRUE if the requested flag(s) are set in the
 * schema entry's schema_entry_flags field.
 *
 * S is a pointer to a sec_attr_schema_entry_t.
 * F is a sec_attr_sch_entry_flags_t with the
 * flag bits of interest set.
 */
#define SA_SCH_FLAG_IS_SET(S,F)	\
	(((S)->schema_entry_flags & (F)) == (F))

#define SA_SCH_FLAG_IS_SET_UNIQUE(S)	\
	(SA_SCH_FLAG_IS_SET((S),sec_attr_sch_entry_unique))

#define SA_SCH_FLAG_IS_SET_MULTI_INST(S)	\
	(SA_SCH_FLAG_IS_SET((S),sec_attr_sch_entry_multi_inst))

#define SA_SCH_FLAG_IS_SET_RESERVED(S)	\
	(SA_SCH_FLAG_IS_SET((S),sec_attr_sch_entry_reserved))

#define SA_SCH_FLAG_IS_SET_USE_DEFAULTS(S)	\
	(SA_SCH_FLAG_IS_SET((S),sec_attr_sch_entry_use_defaults))

/*
 * Macro's to set the flags. 
 */

#define SA_SCH_FLAG_SET(S, F) ((S)->schema_entry_flags |= (F))

#define SA_SCH_FLAG_SET_UNIQUE(S)    \
        (SA_SCH_FLAG_SET((S),sec_attr_sch_entry_unique))

#define SA_SCH_FLAG_SET_MULTI_INST(S)        \
        (SA_SCH_FLAG_SET((S),sec_attr_sch_entry_multi_inst))

#define SA_SCH_FLAG_SET_RESERVED(S)  \
        (SA_SCH_FLAG_SET((S),sec_attr_sch_entry_reserved))

#define SA_SCH_FLAG_SET_USE_DEFAULTS(S)      \
        (SA_SCH_FLAG_SET((S),sec_attr_sch_entry_use_defaults))
/*
 * Macro's to unset the flags. 
 */

#define SA_SCH_FLAG_UNSET(S, F) ((S)->schema_entry_flags &= ~(F))

#define SA_SCH_FLAG_UNSET_UNIQUE(S)    \
        (SA_SCH_FLAG_UNSET((S),sec_attr_sch_entry_unique))

#define SA_SCH_FLAG_UNSET_MULTI_INST(S)        \
        (SA_SCH_FLAG_UNSET((S),sec_attr_sch_entry_multi_inst))

#define SA_SCH_FLAG_UNSET_RESERVED(S)  \
        (SA_SCH_FLAG_UNSET((S),sec_attr_sch_entry_reserved))

#define SA_SCH_FLAG_UNSET_USE_DEFAULTS(S)      \
        (SA_SCH_FLAG_UNSET((S),sec_attr_sch_entry_use_defaults))



/* 
 * SCHEMA ENTRY - Trigger flag check macros evaluate
 * to TRUE if the requested flag(s) are set in the
 * schema entry's trig_types field.
 *
 * S is a pointer to a sec_attr_schema_entry_t.
 * F is a sec_attr_trig_type_flags_t with the
 * flag bits of interest set.
 */

#define SA_SCH_TRIG_FLAG_IS_SET(S,F)	\
	(((S)->trig_types & (F)) == (F))

#define SA_SCH_TRIG_FLAG_IS_NONE(S)	\
	((S)->trig_types == sec_attr_trig_type_none)

#define SA_SCH_TRIG_FLAG_IS_QUERY(S)	\
	(SA_SCH_TRIG_FLAG_IS_SET((S),sec_attr_trig_type_query))

#define SA_SCH_TRIG_FLAG_IS_UPDATE(S)	\
	(SA_SCH_TRIG_FLAG_IS_SET((S),sec_attr_trig_type_update))

/* END SCHEMA ENTRY - Flag check macros */

#endif /* sec_attr_tools_h__included */

