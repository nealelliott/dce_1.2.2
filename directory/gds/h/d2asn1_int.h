/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2asn1_int.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:16  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:01:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:05  root]
 * 
 * Revision 1.1.2.5  1994/06/21  14:47:16  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:41:30  marrek]
 * 
 * Revision 1.1.2.4  1994/06/09  18:46:20  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:24  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  15:55:57  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:38:29  marrek]
 * 
 * Revision 1.1.2.2  1994/03/23  15:18:29  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:14  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:17  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:11:39  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:52:55  marrek]
 * 
 * $EndLog$
 */
#ifndef _D2ASN1_INT_H
#define _D2ASN1_INT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d2asn1_int_rcsid[] = "@(#)$RCSfile: d2asn1_int.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:59 $";
#endif

/* includes */

#include <stdio.h>

#include <dce/dce.h>
#include <dce/asn1.h>
#include <dce/dce.h>
#include <dce/asn1.h>
#include <dce/d2dir.h>
#include <dce/d21.h>
#include <dce/d27util.h>
#include <dce/osiaddr.h>
#include <dce/cmx_addr.h>

#include <d27svc.h>
#include <d23apdu.h>
#include <d21spat.h>
#include <DUA/std.h>
#include <DUA/dua.h>
#include <IDS/ids.h>
#include <IDS/credext.h>
#include <DUA/dua_dua.h>
#include <DUA/dua_ids.h>

/* defines */

#define D2_ASN1_TRACE_ENTER_ENCODING(x,y) \
			DCE_SVC_LOG((GDS_S_ASN1_ENTER_ENCODING_MSG, \
				x, y));

#define D2_ASN1_TRACE_EXIT_ENCODING(x,y,z) \
			DCE_SVC_LOG((GDS_S_ASN1_EXIT_ENCODING_MSG, \
				x, y, z));

#define D2_ASN1_TRACE_ENTER_ERROR_ENCODING(x,y) \
			DCE_SVC_LOG((GDS_S_ASN1_ENTER_ERROR_ENCODING_MSG, \
				x, y));

#define D2_ASN1_TRACE_EXIT_ERROR_ENCODING(x,y,z) \
			DCE_SVC_LOG((GDS_S_ASN1_EXIT_ERROR_ENCODING_MSG, \
				x, y, z));

#define D2_ASN1_TRACE_ENTER_DECODING(x,y) \
			DCE_SVC_LOG((GDS_S_ASN1_ENTER_DECODING_MSG, \
				x, y));

#define D2_ASN1_TRACE_EXIT_DECODING(x,y,z) \
			DCE_SVC_LOG((GDS_S_ASN1_EXIT_DECODING_MSG, \
				x, y, z));

#define D2_ASN1_TRACE_ENTER_ERROR_DECODING(x,y) \
			DCE_SVC_LOG((GDS_S_ASN1_ENTER_ERROR_DECODING_MSG, \
				x, y));

#define D2_ASN1_TRACE_EXIT_ERROR_DECODING(x,y,z) \
			DCE_SVC_LOG((GDS_S_ASN1_EXIT_ERROR_DECODING_MSG, \
				x, y, z));

#define D2_ASN1_TRACE_ENCODED_MESSAGE(x) \
			DCE_SVC_LOG((GDS_S_ASN1_ENCODED_MESSAGE_MSG, \
				x->l, x->v));

#define D2_ASN1_TRACE_TAG_MISMATCH(x) \
			DCE_SVC_LOG((GDS_S_ASN1_TAG_MISMATCH_MSG, \
				function_name, x));
	
#define D2_ASN1_TRACE_DEC_ERROR(x) \
			DCE_SVC_LOG((GDS_S_ASN1_DEC_ERROR_MSG, \
				function_name, x));

#define D2_ASN1_ERR_RETURN_WITH_TRACE \
			DCE_SVC_LOG((GDS_S_ASN1_ERR_RETURN_MSG, \
				function_name)); \
			return(D2_ERROR);

#define  D2A_APDU_MEM           4096   /* size of apdu to be allocated */
#define  D2A_ERR_APDU_SIZE      20     /* size of error apdu */

#define D2A_MORE_MEM    16384       /* memory that is additionally    */
				    /* allocated in case of           */
				    /* reallocation                   */

/****************************/
/* function prototypes      */
/* (additional to d2asn1.h) */
/****************************/

/* from d24aadd.c */

signed32 d2a075_apdu_add_arg_ids ( byte * , signed32 * , ids_add_arg * ,
						ids_common_arg * , byte * );

/* from d24abind.c */

signed32 d2a002_apdu_bind_arg_ids(byte *,signed32 *,ids_bind_arg *,byte *);
signed32 d2a004_ids_bind_res_apdu(byte **,signed32 *,ids_bind_res *,byte *);
signed32 d2a012_ids_bind_error_apdu(byte **,signed32 *,ids_bind_error *,byte *);
d2_ret_val d2a600_ext_cred_to_asn1 (byte *,signed32 *,asn1_field *,byte *);
d2_ret_val d2a601_ext_cred_from_asn1(byte **,signed32 *,asn1_field *,byte *);

/* from d24acharg.c */

signed32 d2a150_apdu_charg_ids ( byte ** , signed32 * , ids_ch_arg * , byte *);
signed32 d2a151_ids_chres_apdu (byte **,byte **,signed32 *,ids_ch_res *,byte *);
signed32 d2a152_tr_info_ids (byte **,signed32 *,signed32,ids_trace_infos *,byte *);

/* from d24acomp.c */

signed32 d2a082_apdu_compare_arg_ids ( byte * , signed32 * , ids_compare_arg *,
						ids_common_arg * , byte * );
signed32 d2a084_ids_compare_res_apdu ( byte ** , byte ** , signed32 * ,
			ids_compare_res * , ids_common_res * , byte *);

/* from d24aerror.c */

signed32 d2a014_error_dec_apdu ( byte ** , signed32 * , ids_error * , byte *);

/* from d24alist.c */

signed32 d2a100_apdu_list_arg_ids ( byte * , signed32 * , ids_list_arg * ,
					ids_common_arg * , byte * );
signed32 d2a103_ids_list_res_apdu ( byte ** , byte ** , signed32 * ,
				ids_list_res * , ids_common_res * , byte * );

/* from d24amod.c */

signed32 d2a104_apdu_modify_arg_ids ( byte * , signed32 * , ids_modify_arg * ,
					ids_common_arg * , byte * );

/* from d24amodrdn.c */

signed32 d2a127_modrdn_arg_ids ( byte * , signed32 * , ids_modify_rdn_arg * ,
					ids_common_arg * , byte * );

/* from d24aread.c */

signed32 d2a006_apdu_read_arg_ids ( byte * , signed32 * , ids_read_arg * ,
					ids_common_arg * , byte * );
signed32 d2a008_ids_read_res_apdu ( byte ** , byte ** , signed32 * ,
				ids_read_res * , ids_common_res * , byte * );
signed32 d2a019_req_to_einfo_sel ( byte ** , ids_entry_info_sel * ,
					Octet_string , signed32 * );

/* from d24arem.c */

signed32 d2a067_apdu_rem_arg_ids ( byte * , signed32 * , ids_remove_arg * ,
					ids_common_arg * , byte * );

/* from d24asearch.c */

signed32 d2a069_apdu_search_arg_ids ( byte * , signed32 * , ids_search_arg * ,
					ids_common_arg * , byte * );
signed32 d2a071_ids_search_res_apdu (byte **,byte **,signed32 *,ids_search_res *,
					ids_common_res *,byte *);

/* from d24asn1.c */

#ifdef  DSA /* in d24asn1n.c */

signed32 d2a020_apdu_bind_arg_asn (byte *,signed32 *,asn1_field *,byte *);
signed32 d2a022_apdu_read_arg_asn (byte *,signed32 *,asn1_field *,byte *);
signed32 d2a065_apdu_rem_arg_asn (byte *,signed32 *,asn1_field *,byte *);
signed32 d2a073_apdu_add_arg_asn (byte *,signed32 *,asn1_field *,byte *);
signed32 d2a086_apdu_mod_arg_asn (byte *,signed32 *,asn1_field *,byte *);
signed32 d2a087_apdu_list_arg_asn (byte *,signed32 *,asn1_field *,byte *);
signed32 d2a089_apdu_search_arg_asn (byte *,signed32 *,asn1_field *,byte *);
signed32 d2a091_apdu_comp_arg_asn (byte *,signed32 *,asn1_field *,byte *);
signed32 d2a128_apdu_modrdn_asn (byte *,signed32 *,asn1_field *,byte *);
signed32 d2a999_apdu_abandon_asn (byte *,signed32 *,asn1_field *,byte *);

signed32 d2a041_asn_bind_res_apdu (byte **,signed32 *,asn1_field *,byte *);
signed32 d2a043_asn_read_res_apdu (byte **,byte **,signed32 *, asn1_field *,byte *);
signed32 d2a095_asn_list_res_apdu (byte **,byte **,signed32 *, asn1_field *,byte *);
signed32 d2a097_asn_srch_res_apdu (byte **,byte **,signed32 *, asn1_field *,byte *);
signed32 d2a099_asn_comp_res_apdu (byte **,byte **,signed32 *, asn1_field *,byte *);
signed32 d2a046_asn_bind_error_apdu (byte **,signed32 *,asn1_field *,byte *);

#else /* if not defined DSA */ /* in d24asn1n.c */

#endif

/* from d25aadd.c */

signed32 d2a076_ids_add_arg_apdu (byte **,byte **,signed32 *,ids_add_arg *,
						ids_common_arg *,byte *);

/* from d25abind.c */

signed32 d2a003_ids_bind_arg_apdu (byte **,signed32 *,ids_bind_arg *);
signed32 d2a005_apdu_bind_res_ids ( byte * , ids_bind_res * );
signed32 d2a013_apdu_bind_error_ids (byte *,signed32,ids_bind_error *);

/* from d25acharg.c */

signed32 d2a160_ids_charg_apdu ( byte ** , byte ** , signed32 * ,
                                        ids_ch_arg * , byte * );
signed32 d2a161_apdu_chres_ids ( byte ** , signed32 * , ids_ch_res * , byte * );
signed32 d2a162_tr_info_apdu ( byte ** , ids_trace_infos * , byte ** ,
                                        signed32 * , byte * , signed32 * );
signed32 d2a164_apdu_dsa_ref_ids (byte *,signed32 *,ids_dsa_referral *,byte *);

/* from d25acomp.c */

signed32 d2a083_ids_compare_arg_apdu (byte **,byte **,signed32 *,
                                ids_compare_arg *, ids_common_arg *,byte *);
signed32 d2a085_apdu_compare_res_ids ( byte * , signed32 , ids_compare_res * ,
                                                ids_common_res * );

/* from d25aerror.c */

signed32 d2a015_error_cod_ids ( byte * , signed32 , signed32 * , ids_error * );

/* from d25alist.c */

signed32 d2a102_apdu_list_res_ids ( byte * , signed32 , ids_list_res * ,
                                                ids_common_res * );
signed32 d2a101_ids_list_arg_apdu (byte **,byte **,signed32 *,ids_list_arg *,
                                        ids_common_arg *,byte *);
signed32 d2a108_mk_ids_part_oq (byte **,ids_part_oq *,byte *,signed32 *);

/* from d25amod.c */

signed32 d2a105_ids_modify_arg_apdu (byte **,byte **,signed32 *,ids_modify_arg *,
                                                ids_common_arg *,byte *);

/* from d25amodrdn.c */

signed32 d2a126_ids_modrdn_arg_apdu (byte **,byte **,signed32 *,
                        ids_modify_rdn_arg *,ids_common_arg *,byte *);

/* from d25aread.c */

signed32 d2a007_ids_read_arg_apdu (byte **,byte **,signed32 *,ids_read_arg *,
                                        ids_common_arg *,byte *);
signed32 d2a009_apdu_read_res_ids (byte *,signed32,ids_read_res *,
                                        ids_common_res *);
signed32 d2a028_einfo_sel_to_req_attr (ids_entry_info_sel *,byte **,
                                        byte **,signed32 *,byte *,signed32 *);

/* from d25arem.c */

signed32 d2a068_ids_rem_arg_apdu (byte **,byte **,signed32 *,ids_remove_arg *,
                                                ids_common_arg *,byte *);

/* from d25asearch.c */

signed32 d2a070_ids_search_arg_apdu (byte **,byte **,signed32 *,ids_search_arg *,
                                        ids_common_arg *,byte *);
signed32 d2a072_apdu_search_res_ids (byte *,signed32,ids_search_res *,
                                                ids_common_res *);

/* from d2aattr.c */

signed32 d2a029_entry_info_to_at_info (signed32 ,ids_info * ,byte ** ,byte **,
                                        signed32 * , byte * , signed32 * );
signed32 d2a030_at_info_to_entry_info (byte **,signed32 *,ids_info **,
                                                Octet_string,signed32 *);
signed32 d2a060_mk_att_apdu ( ids_att * , byte ** , byte ** , signed32 *,
                                        byte * , signed32 * );
signed32 d2a062_mk_apdu_att (byte **,ids_att *,signed32 *,Octet_string ,signed32 * );
signed32 d2a170_t61_pr_seq_len ( byte * );
signed32 d2a171_t61_pr_seq_fr_ipc ( byte ** , t61_pr_seq * );
signed32 d2a172_t61_pr_seq_to_ipc ( t61_pr_seq * , byte ** );

/* from d2acommon.c */

signed32 d2a031_cp_attr ( signed32 , signed16 , byte * , dua_att_value * ,
                                                byte * , signed32 * );
signed32 d2a033_check_memsize ( signed32 , byte ** , byte ** , signed32 * ,
                                        D23_rserror * , signed32 *);
void d2a080_mk_ids_comarg (byte ** , ids_common_arg * );
void d2a081_mk_ids_comres ( byte ** , ids_common_res * );
signed32 d2a106_ids_atval_to_atval (ids_att_value *,signed32,signed16 *,
				signed16 *,unsigned16 *,byte **,byte **,
						signed32 *,byte *,signed32 *);
signed32 d2a120_copy_att_type (byte **,ids_att_type *,byte *,signed32 *);
signed32 d2a123_mk_apdu_cmarg ( byte ** , ids_common_arg * , byte ** ,
                                        signed32 * , byte * , signed32 * );
signed32 d2a124_copy_attyp_apdu (byte **,ids_att_type *,byte **,signed32 *,
                                        byte *,signed32 *);
void d2a130_mk_apdu_comres ( ids_common_res * , byte **);

/* from d2ads_dn.c */

signed32 d2a_016_str_to_dn (char *,dua_dist_name *,byte *,signed32 *);
signed32 d2a017_dn_to_str (unsigned16,dua_dist_name *,byte **,byte **,
                                        signed32 *,byte *,signed32 *);
signed32 d2a121_dn_ids ( byte ** , ids_dist_name * , byte * , signed32 * );
signed32 d2a122_dn_apdu ( unsigned16 , byte ** , ids_dist_name * , byte ** ,
                                signed32 * , byte * , signed32 * );

/* from d2amhs.c */

signed32 d2a139_dlsp_to_ipc (DLSubmitPermission *,byte **,byte **,signed32 *,byte *);
signed32 d2a140_oradr_to_ipc (ORAddress *,byte **,byte **,signed32 *,byte *);
signed32 d2a141_orname_to_ipc (ORName *,byte **,byte **,signed32 *,byte *);

/* from d2amhs1.c */

signed32 d2a136_dlsp_fr_ipc (signed16,byte *,DLSubmitPermission *,byte *,
								signed32 *);
signed32 d2a137_oradr_fr_ipc (signed16,byte *,ORAddress *,byte *,signed32 *);
signed32 d2a138_orname_fr_ipc (signed16,byte *,ORName *,byte *,signed32 * );

/* from d2aref.c */

signed32 d2a039_put_referral (ids_cont_reference *,byte **,byte *,signed32 *);
signed32 d2a049_put_psap ( byte * , dua_psap * , byte * , signed32 * );
signed32 d2a053_put_ref ( ids_cont_reference * , byte ** , byte ** ,
                                signed32 * , byte * , signed32 * );
signed32 d2a061_get_psap ( dua_psap * , boolean , byte ** , byte ** ,
                                signed32 * , byte * , signed32 * );

/* from d2asap.c */

d2_ret_val d2a_200_certificate_from_apdu ( char ** , duaCertificate * );
d2_ret_val d2a_201_cert_pair_from_apdu ( char ** , duaCertPair * );
d2_ret_val d2a_202_cert_list_from_apdu ( char ** , duaCertList * );

d2_ret_val d2a_210_certificate_to_apdu ( duaCertificate * , char ** );
d2_ret_val d2a_211_cert_pair_to_apdu ( duaCertPair * , char ** );
d2_ret_val d2a_212_cert_list_to_apdu ( duaCertList * , char ** );

size_t d2a_250_certificate_msg_len( duaCertificate * );
size_t d2a_251_cert_pair_msg_len( duaCertPair * );
size_t d2a_252_cert_list_msg_len( duaCertList * );

void d2a_260_certificate_free( duaCertificate * );
void d2a_261_cert_pair_free ( duaCertPair * );
void d2a_262_cert_list_free ( duaCertList * );

/* from dua_dua.c */

signed32 dua_bit_string_dua ( ids_bit_string * , dua_bit_string * );
signed32 dua_utc_time_dua ( ids_utc_time * , dua_utc_time * );
signed32 dua_att_value_dua ( ids_att_value * , dua_att_value1 * , signed32 );
signed32 dua_dist_name_dua ( ids_dist_name * , dua_dist_name * );
signed32 dua_dist_name_syntax_dua ( ids_att_value * , dua_dist_name ** );
signed32 dua_rdname_dua ( ids_rdname * , dua_rdname * );

/* from dua_free.c */

void dua_att_value1_free ( dua_att_value1 * );
void dua_att_value_free ( dua_att_value * );
void d_name_free (dua_name * );
void dua_name_free( dua_dist_name * );

/* from dua_ids.c */

signed32 dua_bit_string_ids ( dua_bit_string * , ids_bit_string * );
signed32 dua_utc_time_ids ( dua_utc_time * , ids_utc_time * );
signed32 dua_att_value_ids ( dua_att_value * , ids_att_value * );
signed32 dua_dist_name_ids ( dua_dist_name * , ids_dist_name * );
signed32 dua_rdname_ids( dua_rdname * , ids_rdname * );

/* from ids_free.c */

void i_dist_name_free ( ids_dist_name * );
void i_cont_reference_free ( ids_cont_reference * );
void i_bind_arg_free ( ids_bind_arg * );
void i_bind_res_free ( ids_bind_res * );
void i_ch_arg_free ( ids_ch_arg * , asn1_field );
void i_ch_res_free ( ids_ch_res * , asn1_field );
void i_ch_null_res_free ( ids_ch_res * );
void i_read_arg_free ( ids_read_arg * , ids_common_arg * );
void i_read_res_free ( ids_read_res * , ids_common_res * );
void i_compare_arg_free ( ids_compare_arg * , ids_common_arg * );
void i_compare_res_free ( ids_compare_res * , ids_common_res * );
void i_abandon_arg_free ( signed32 );
void i_list_arg_free ( ids_list_arg * , ids_common_arg * );
void i_list_res_free ( ids_list_res * , ids_common_res * );
void i_search_arg_free ( ids_search_arg * , ids_common_arg * );
void i_search_res_free ( ids_search_res * , ids_common_res * );
void i_add_arg_free ( ids_add_arg * , ids_common_arg * );
void i_remove_arg_free ( ids_remove_arg * , ids_common_arg * );
void i_modify_arg_free ( ids_modify_arg * , ids_common_arg * );
void i_modify_rdn_arg_free ( ids_modify_rdn_arg * , ids_common_arg * );
void i_att_error_free ( ids_att_error * );
void i_name_error_free( ids_name_error * );
void i_dsa_referral_free ( ids_dsa_referral * );
void i_bind_error_free ( ids_bind_error * );

/* from std.c */

d2_ret_val copy_octet_string ( octet_string *, octet_string * );

#endif /* _D2ASN1_INT_H */
