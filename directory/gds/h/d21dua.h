/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21dua.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:28  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:59:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:27  root]
 * 
 * Revision 1.1.2.5  1994/06/21  14:47:06  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:40:45  marrek]
 * 
 * Revision 1.1.2.4  1994/06/09  18:46:11  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:14  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  15:55:42  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:37:22  marrek]
 * 
 * Revision 1.1.2.2  1994/03/23  15:18:19  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:17  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:11  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:12:27  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:53:50  marrek]
 * 
 * $EndLog$
 */
#ifndef _D21DUA_H
#define _D21DUA_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d21dua_rcsid[] = "@(#)$RCSfile: d21dua.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:30 $";
#endif

/* includes */

#include <time.h>
#include <dce/dce.h>
#include <dce/d2dir.h>
#include <dce/d21.h>
#include <dce/d21iapl.h>
#include <dce/d27util.h>
#include <dce/osiaddr.h>
#include <dce/cmx_addr.h>

#include <d23apdu.h>
#include <d21spat.h>
#include <d23ipc.h>
#include <malgcfe.h>
#include <D00_dua_protect.h>
#include <D00_switch_protect.h>
#include <D00_gds_sec_protect.h>

#include <d21svc.h>

/* constants */

#define D21_F_CREA_MODE 04600

/* prototyping */

/* from d21binda.c */

d2_ret_val d21_b15_read_named_dsa ( D21_p1 * );
d2_ret_val d21_b17_read_def_dsa ( D2_a_info ** , D21_p1 * );

/* from d21bindh.c */

void d21_bh0_init_bind_handler ( void );
d2_ret_val d21_bh1_exec_bind ( D21_p1 * , Bool );
void d21_bh2_exec_unbind ( D21_p2 * );
d2_ret_val d21_bh8_get_bind_info ( signed16 , d21_bind_info * , Bool * ,
					D2_pbhead * , Bool );
d2_ret_val d21_bh4_i_bind ( signed16 , d21_ipc_info ** , D2_pbhead * );
void d21_bh5_i_unbind ( signed16 );
d2_ret_val d21_att_cl ( D2_pbhead * );
d2_ret_val d21_88_cds_bind ( D21_p1 * );
void d21_ipc_clear ( signed16 , Bool * , d21_bind_info * );
void d21_detach( void );
signed32 d21_ca_calc_credlen ( D2_credent * );
void d21_ca8_cred_iput ( D2_credent * , char * );

/* from d21readc.c */

d2_ret_val d21_b_read_cache ( D2_pbhead * , D2_name_string , signed16 ,
								D2_a_info ** );
d2_ret_val d21_rd04_cache_read ( D21_p3 * , char * , signed32 ,
							char ** , signed32 * );
d2_ret_val d21_rd05_move_ei_info_to_mem ( D21_p3 * , char * );
void d21_rd03_build_req ( D21_p3 * , char * , signed16 , signed16 );
void d21_rat_iput ( D2_req_info * , char * , char ** , Bool );
d2_ret_val d21_b18_read_psap ( D2_name_string , signed16 , D2_a_value ** ,
								D2_pbhead * );

/* from d21readh.c */

d2_ret_val d21_rd01_check_params ( D21_p3 * );
signed32 d21_rd06_dsa_read ( Bool , D21_p3 * , char * , signed32 ,
				char ** , signed32 * , d21_bind_info * );
d2_ret_val d21_rd07_check_attr ( D21_p3 * , char * , signed16 );

/* from d21lsh.c */

d2_ret_val d21_ls01_check_params ( D21_p4 * );
signed32 d21_ca_len_inlist ( D21_p4 * );
void d21_ls03_build_req ( D21_p4 * , char * , signed16 , signed16 );
d2_ret_val d21_ls05_move_names_to_mem (D21_p4 *,char *,signed16,char **,
					D23_rf_info **,char **,signed16 *);
signed32 d21_ls07_dsa_list ( D21_p4 * , char * , signed32 , char ** ,
						signed32 * , d21_bind_info * );
d2_ret_val d21_ls08_handle_ref( D21_lb_info * , signed16 * , D21_p4 * ,
				char * , signed32 , char ** , signed32 * ,
				time_t , signed16 , char ** , D23_rf_info ** ,
				char ** , signed16 * , d21_bind_info * );

/* from d21sch.c */

d2_ret_val d21_sc01_check_params ( D21_p5 * );
signed32 d21_sc02_len_insearch ( D21_p5 * );
d2_ret_val d21_sc03_build_req ( D21_p5 * , char * , signed16 , signed16 );
d2_ret_val d21_sc04_cache_search ( D21_p5 * , char * , signed32 ,
						 char ** , signed32 * );
d2_ret_val d21_sc05_move_ei_info_to_mem (  D21_p5 * , char * , signed32 ,
			char ** , D23_rf_info ** , char ** , signed16 * );
signed32 d21_sc06_dsa_only_search ( Bool , D21_p5 * , char * , signed32 ,
				char ** , signed32 * , d21_bind_info * );
d2_ret_val d21_sc08_handle_ref( D21_lb_info * , signed16 * , Bool , D21_p5 * ,
				char * , signed32 , char ** , signed32 * ,
				time_t , signed16 , char ** , D23_rf_info ** ,
				char ** , signed16 * , d21_bind_info * );

/* from d21cph.c */

d2_ret_val d21_cp01_check_params ( D21_p6 * );
d2_ret_val d21_cp03_build_req ( D21_p6 * , char * , signed16 , signed16 );
d2_ret_val d21_cp04_cache_compare ( D21_p6 * , char * , signed32 ,
						char ** , signed32 * );
d2_ret_val d21_cp05_set_result ( D21_p6 * , char * );
signed32 d21_cp06_dsa_compare ( D21_p6 * , char * , signed32 , char ** ,
						signed32 * , d21_bind_info * );

/* from d21adh.c */

signed16 d21_ad_01_check_m_k ( D21_p7 * , d21_bind_info * , D2_a_info ** );
signed16 d21_ad_02_add_to_dsa ( signed16 , D21_p7 * , d21_bind_info * ,
				char * , signed32 , char ** , signed32 * );
signed16 d21_ad_05_sup_master_bind ( D21_p7 * , d21_bind_info * , signed16 * );
void d21_ad_07_unbind ( D21_p7 * , signed16 );
d2_ret_val d21_ad_08_bind ( D21_p7 * , d21_bind_info * , char * , signed16 * );
d2_ret_val d21_ad_09_read_mk ( D2_name_string , signed16 , D21_p7 * ,
				D21_p3 * , Bool * , Bool * );

/* from d21remh.c */

d2_ret_val d21_rm_02_rem_from_dsa ( signed16 , D21_p8 * , d21_bind_info * ,
				char * , signed32 , char ** , signed32 * );

/* from d21rrh.c */

d2_ret_val d21_rr1_check_params ( D21_pB * );
void d21_rr3_build_req ( D21_pB * , char * , d21_bind_info * );
signed32 d21_ca_len_inreplrdn ( D21_pB * );

/* from d21mah.c */

d2_ret_val d21_m01_check_params ( D21_pC * );
signed16 d21_m02_bind_to_master_dsa ( D2_pbhead * , signed32 , signed16 ,
			D2_name_string , D2_name_string , d21_bind_info * ,
			D2_name_string , D2_name_string , signed16 * ,
			D2_error ** , Bool * );
void d21_m03_unbind ( signed16 , signed16 );
void d21_m04_map_rc ( signed16 , D2_pbhead * , D2_pbhead * , D2_error * );
signed16 d21_m06_build_req ( D21_pC * , char * , d21_bind_info * );
signed16 d21_m07_exec ( signed16 , D2_pbhead * , char * , signed32 ,
					signed16 , char ** , signed32 * ,
						const d21_bind_info * );
void d21_m08_proc_epdu ( signed16 , signed32 , D2_pbhead * , char * ,
				signed32 , D2_error ** );
signed32 d21_ca_len_inmodify ( D21_pC * );

/* from d21com.c */

void d21_c30_get_sup ( D2_name_string , D2_name_string );
signed32 d21_ca4_attr_ilen_get ( char ** );
signed32 d21_ca_len_ei_info ( char * );
signed32 d21_ca_len_l_info ( char * );
signed32 d21_ca_len_ref_inf ( char ** );
signed32 d21_ca_len_error ( char * , signed16 * );
signed32 d21_ca_calc_av_info ( D2_a_value * , signed32 );
signed32 d21_ca_calc_carglen ( D2_c_arg * );
signed32 d21_ca_calc_req_len ( D2_req_info * , signed32 );
signed32 d21_ca_calc_atinfo_len ( D2_a_info * );
void d21_ca9_eom_iput ( char * , char ** );
signed32 d21_ca_len_inread ( D21_p3 * );
signed32 d21_ca_len_incompare ( D21_p6 * );
signed32 d21_ca_len_inremove ( D21_p8 * );

/* from d21common.c */
void d21_cm_cpyobj ( D2_a_type * , char ** );
void d21_cm01_serv_ctrl ( signed32 , signed16 * );
void d21_check_comarg ( D2_c_arg * , signed16 * , const char * , signed16 );
void d21_cm02_build_hdr ( D23_header * , d21_bind_info * );
void d21_cm03_build_comarg ( D2_c_arg * , char ** );
void d21_cab_dsname_iput ( D2_name_string , char * , char ** );
boolean d21_ca_aval_iput ( D2_a_value * , char ** , char ** );
void d21_ca2_attr_iget ( char ** , D2_a_info * , char ** );
d2_ret_val d21_copy_referral ( D23_rf_info ** , D2_ref_inf ** , D2_pbhead * );
d2_ret_val d21_cac_copy_error ( char * , D2_pbhead * , D2_error ** );
signed16 d21_cac_copy_referral ( char ** , D2_ref_inf * );
void d21_skip_referral ( char ** );

/* from d21_c20.c */

signed16 d21_c22_attr_iput ( Bool , D2_a_info * , char * , char ** );
d2_ret_val d21_ca5_attr_val_iput (D2_a_value *,char *,char **,unsigned16);
signed16 d21_ca4_check_sc ( D2_c_arg * );

/* from d21_c70.c */

signed32 d21_send_invoke ( D21_req_info * , signed32 , D2_pbhead * );
signed32 d21_recv_result ( D21_req_info * , signed32 , D2_pbhead * );
signed32 d21_recv_bind_res ( D21_req_info * , D2_pbhead * , unsigned16 * );

/* from d21_c80.c */
Bool d21_c82_dsname_cmp ( D2_name_string , D2_name_string );

/* from d21log.c */
void d21_L01_init (void);
void d21_L02_trace_bind_input (D21_p1 * );
void d21_L03_trace_unbind_input (D21_p2 * );
void d21_L04_trace_iapl_input( D2_name_string , D2_c_arg * );

/* from d21ref.c */
d2_ret_val d21_b03_ref_bind ( D2_pbhead * , D2_name_string , D2_a_value * ,
					d21_bind_info * , signed16 * );
d2_ret_val d21_handle_ref ( D21_lb_info * , signed16 * , D23_rf_info ** ,
				D2_pbhead * , time_t , signed16 , signed16 ,
				char ** , signed32 * , char * , signed32 ,
				d21_bind_info * );

/* from d21refh.c */

d2_ret_val d21_cr1_add_lb ( D21_lb_info * , signed16 * , D2_name_string ,
				D2_op_progress * , D2_name_string );
Bool d21_cr3_check_lb ( D21_lb_info * , signed16 * , D2_name_string ,
				D2_op_progress * , D2_name_string );
void d21_cr4_purge_lb_tab ( D21_lb_info * , signed16 * );

/* from d21filter.c */

d2_ret_val d21_ca_calc_fi_len ( D2_Fi_filter * , signed32 * );
d2_ret_val d21_fi_iput ( D2_Fi_filter * , char * , char ** );

/* from d21file.c */

d2_ret_val d21_sc10_write_output_to_file ( D2_pbhead * , d21_bind_info * ,
				char * , signed32 , D23_rf_info ** ,
				signed16 * , signed16 , signed16 * ,
				int , Bool );
d2_ret_val d21_sc13_handle_file_ref ( D21_lb_info * , signed16 * , D2_pbhead * ,
				char * , signed32 , time_t , D23_rf_info * ,
				signed16 * , signed16 , signed16 * ,
				d21_bind_info * , int );

/* from d21mhsfm.c */

void d21_221_or_addr_fr_ipc ( char ** , char ** , Bool );
void d21_222_or_name_fr_ipc ( char ** , char ** , Bool );
void d21_223_sub_perm_fr_ipc ( char ** , char ** , Bool );

/* from d21mhslm.c */

signed32 d21_211_or_addr_msglen ( D2_OR_address * );
signed32 d21_212_or_name_msglen ( D2_OR_name * );
signed32 d21_213_sub_perm_msglen ( D2_dl_submit_perm * );

/* from d21mhsls.c */

signed32 d21_231_or_addr_struct_len ( char * );
signed32 d21_232_or_name_struct_len ( char * );
signed32 d21_233_sub_perm_struct_len ( char * );

/* from d21mhstm.c */

d2_ret_val d21_201_or_addr_to_ipc ( D2_OR_address * , char ** );
d2_ret_val d21_202_or_name_to_ipc ( D2_OR_name * , char ** );
d2_ret_val d21_203_sub_perm_to_ipc ( D2_dl_submit_perm * , char ** );

/* from d21sap.c */

void d21_500_certificate_from_apdu (char ** , char ** );
void d21_501_cert_pair_from_apdu (char ** , char ** );
void d21_502_cert_list_from_apdu (char ** , char ** );

void d21_510_certificate_to_apdu ( Certificate * , char ** );
void d21_511_cert_pair_to_apdu ( Cert_pair * , char ** );
void d21_512_cert_list_to_apdu ( Cert_list * , char ** );

signed32 d21_540_certificate_struct_len( char * );
signed32 d21_541_cert_pair_struct_len( char * );
signed32 d21_542_cert_list_struct_len( char * );

signed32 d21_550_certificate_msg_len( Certificate * );
signed32 d21_551_cert_pair_msg_len( Cert_pair * );
signed32 d21_552_cert_list_msg_len( Cert_list * );

/* from d21sec.c */
d2_ret_val dua_gen_simp_anon_sec_ctx ( signed16 , D2_credent * , D2_str * );
void dua_conf_simp_anon_sec_ctx ( signed16 , D2_str * , D2_name_string * );

/* from d21ipcev.c */

d2_ret_val d21_create_event_dispatcher ( D2_pbhead * );
d2_ret_val d21_remove_event_dispatcher ( D2_pbhead * );
signed32 d21_ipcinvoke ( D23_inrqpb * );
signed32 d21_ipcreceive ( D21_abandon_info * , D23_wtrspb * , signed16 );

/* from d21abandon.c */

#ifdef THREADSAFE
pthread_startroutine_t d21_abandon_handler ( pthread_addr_t );
#endif

/* from d21signal.c */
void d21_disable_signals ( void );
void d21_enable_signals ( void );


/* useful macros */

#define D21_MEMCPY(x,y,l)			\
	if (l != 0)				\
	  {					\
	  memcpy(x,y,l);			\
	  }					\
	else					\
	  {					\
	  x = NULL;				\
	  }

#endif /* _D21DUA_H */
