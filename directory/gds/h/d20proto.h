/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20proto.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:23  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:59:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:22  root]
 * 
 * Revision 1.1.2.7  1994/08/08  13:21:00  marrek
 * 	Fix for OT11045.
 * 	[1994/08/08  13:19:01  marrek]
 * 
 * Revision 1.1.2.6  1994/08/01  15:31:54  marrek
 * 	Bug fixes for OT 11295, 11456, 11458, 11471.
 * 	[1994/08/01  15:30:58  marrek]
 * 
 * Revision 1.1.2.5  1994/06/21  14:47:03  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:40:37  marrek]
 * 
 * Revision 1.1.2.4  1994/06/09  18:46:09  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:11  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  15:55:40  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:37:15  marrek]
 * 
 * Revision 1.1.2.2  1994/03/23  15:18:17  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:31:22  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:09  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:11:47  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:53:03  marrek]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char d20proto_rcsid[] = "@(#)$RCSfile: d20proto.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:27 $";
#endif

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1994 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Component   : DCE 1.1 GDS Administration                        */
/*                                                                 */
/* File Name   : d20proto.h                                        */
/* Description : This file contains administration function        */
/*               prototypes                                        */
/* Date        : Fri Jan 14 11:00:44 MET 1994                      */
/* Author      : Jean-Marc Bonnaudet, SNI BU BA NM 123             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _D20PROTO_H
#define _D20PROTO_H

#include <gds.h>
#include <nl_types.h>
#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d2spec_msk.h>
#include <d21iapl.h>
#include <d21dua.h>
#include <d2hdlacl.h>
#include <d2dump.h>
#include <d2delupd.h>

/* in admin/d20logon.c, usradmin/d20lngset.c, usradmin/d20admin.c */
/* in usradmin/d20util.c, usradmin/d20distcmd.c */
int main(int argc, char *argv[]);

/* in admin/d20util.c */
void ds0_512_object_tree(D20_omask *d20_6mskptr, char tree[],
			Ds_obj_entry *obj_entry, signed16 *no_np,
			signed16 mask);
signed16 ds0_513_hexa(String h_flds, signed16 j);
void ds4_004_octncpy(Octet_string to, Octet_string from, signed16 n);
signed16 d20_strlen(char *str);
signed16 d20_octncmp(Octet_string str1, Octet_string str2, signed16 n);
d2_ret_val d20_gen_dnintf (String dn);
Bool d20_cmp_objid (D2_a_type *objid_ptr1, signed32 objid_len2,
			Ob_id_string objid_str2);
void d20_error(D2_pbhead *pbhead_ptr, signed16 retcode, signed16 errclass,
			signed16 errvalue);
void d20_errcode(Errcode *errcode_ptr, char retcode, char errclass,
			signed16 errvalue);
d2_ret_val d20_198_distname_syntax (char *distname, signed32 rulenr);
Bool d20_cmp_structure (char *dn, char *tree);
signed16 d20_dsacmp( char *dsa1, char *dsa2);

/* in admin/d20trans.c */
signed32 ds0_520_handle_spec_attr(signed16 operation, Ds_property *prty);

/* in admin/d20tnsx.c */
signed32 hex_asc (char *out, char *in, signed16 isize);
d2_ret_val asc_form (char *in1, signed16 isize);

/* in admin/d20spec_msk.c */
signed32 d20_ttxid(signed32 op, char *attr_name, D2_ttxid *ttxid);
signed32 d20_telex(signed32 op, char *attr_name, D2_telex_number *telex);
signed32 d20_fax(signed32 op, char *attr_name, D2_faxnumber *fax);
signed32 d20_t61(signed32 op, char *attr_name, D2_T61_seq *t61);
signed32 d20_postal(signed32 op, char *attr_name, D2_str_list *str_list);
signed32 d20_dnscell(signed32 op, char *attr_name, Octet_string cell_str,
			signed16 *c_len);
signed32 d20_dnsrep(signed32 op, char *attr_name, Octet_string rep_str,
			signed16 *r_len);
signed32 d20_cert(signed32 object_counter, signed32 op, char *attr_name,
			Certificate *rep_str, signed16 *r_len);
signed32 d20_certp(signed32 object_counter, signed32 op, char *attr_name,
			Cert_pair *rep_str, signed16 *r_len);
signed32 d20_certl(signed32 object_counter, signed32 op, char *attr_name,
			Cert_list *rep_str, signed16 *r_len);
signed32 d20_rcert(signed32 object_counter, signed32 op, char *attr_name,
			Rev_certs **rep_str, signed16 *r_len);
signed32 d20_dme_disp(signed32 object_counter, signed32 op, char *attr_name,
			Octet_string *rep_str, signed16 *r_len);

/* in admin/d20schem.c */
d2_ret_val d20_204_read_scheme(char *env);

/* in admin/d20objname.c */
signed16 ds0_501_object_name(Bool scheme_disp, signed16 type, char operation,
			signed32 rule_nr, char tree[], Bool init_flds,
			signed16  *count, D20_oct **oct_ent);

/* in admin/d20acl.c */
signed16 ds0_507_handle_acl(char operation, Bool init_flds, signed16 acl_len,
			char *acl_val);
signed16 ds0_508_check_name(char *dn);

/* in admin/d20admin.c */
signed16 d20_1_admin(D20_pE *parblk);

/* in admin/d20admobj.c */
signed16 ds0_500_objects(Bool scheme_disp, signed16 type, signed32 *rule_nr,
			char tree[]);
Bool ds0_check_chgstruct(Octet_string attr_type, signed32 len_attr,
			D20_srt *srt);

/* in admin/d20displst.c */
void ds0_504_display_list(String tree);
void ds0_530_display_errlist(D2_name_string *old_objects, signed16 head_no);
void d20_550_disp_def_dsa(void);
signed32 d20_handle_attr_list(signed32 operation, D20_srt *srt_ent,
			D20_oct *oct_ent, D20_at  ***attr_ptr_space,
			signed32 *no_selected_attrs);
signed32 d20_handle_schema_list(signed32 operation,
			signed32 *schema_index_space,
			signed32 *no_selected_entries);
signed32 d20_call_disp_aux(D20_oct *oct_ent, Bool bool_array[]);

/* in admin/d20gdnname.c */
d2_ret_val d20_gen_distname(signed32 rule_nr, char  **ifields[], char *ofield);

/* in admin/d20getat.c */
signed32 ds0_502_get_attr(char operation, signed32 rule_nr, D20_oct *oct_ent,
			signed16 *count, Bool init_flds);

signed32 ds0_malloc_attr(signed32 c_val, 
			char attr[][(D20_MAX_LEN * D20_T61_CONV_FACT) + 1],
			D2_str d2_str[], D2_ttxid *ttx_id, D2_faxnumber *fax,
			D2_telex_number *telex, D2_T61_seq *t61,
			D2_obj_id obj_id[]);
void ds0_assign_space_to_post_add( void );
void ds0_assign_space_to_oradd( void );
void ds0_assign_space_to_orname( void );

/* in admin/d20getsh.c */
d2_ret_val d20_lget_scheme(signed32 *no_attrs, D20_at **atptr,
			signed32 *no_oclass, D20_oct **octptr,
			signed32 *no_rules, D20_srt  **srtptr);
caddr_t d20_search_obj(signed16 dsa_adm, signed32 no_oct, caddr_t oct,
			Abbrv obj_abbrv);
D20_at *d20_search_adm_attr(signed32 no_at, D20_at *at, char attr_abbrv[]);
D20_srt *d20_search_adm_srule(signed32 no_srt, D20_srt *srt, signed32 rule_nr);

/* in admin/d20gstruct.c */
void d20_gen_name_structure(signed32 rule_nr, D20_omask **maskptr,
			signed32 flen1, signed32 flen2);

/* in admin/d20hdlacl.c */
signed16 d20_5_handle_acl(D20_p5 *parblk);

/* in admin/d20hdlmsk.c */
signed32 d20_handlemask(D20_omask *parptr);
d2_ret_val d20_save_mask(D20_omask *orig_mask, D20_omask *sav_mask);
d2_ret_val d20_restore_mask(D20_omask *orig_mask, D20_omask *sav_mask);

/* in admin/d20logon.c */
signed32 d20_adm_read_mask_file(void);
signed16 d20_232_spec_exit(void);
signed32 d20_read_params(int argc, char *argv[], D21_p1 *plbind);
signed32 d20_disp_error(char *message); 
d2_ret_val d20_param_error(signed32 msg_set, signed32 msg_no);
Bool d20_check_bind_dsa(char *bind_dsa, char *path, signed16 dir_id);

/* in admin/d20mhs_msk.c */
signed32 d20_oradd(signed32 op, char *attr_name, D2_OR_address *oradd,
		D2_OR_address *new_oradd, signed16 *mod_type);
signed32 d20_oradd_disp(signed32 object_counter, char *attr_name,
		D2_OR_address *oradd);
signed32 d20_orname(signed32 object_counter, signed32 op, char *attr_name,
		D2_OR_name *orname, D2_OR_name *new_orname, signed16 *mod_type);
signed32 d20_dlsp(signed32 object_counter, signed32 op, char *attr_name,
		D2_dl_submit_perm *dlsp, D2_dl_submit_perm *new_dlsp,
		D2_OR_name *orname, char *mog_space_ptr, signed16 *mod_type);

/* in admin/d20modattr.c */
void ds0_disp_old_val(signed16 len, Octet_string val, signed16 syntax);
signed16 ds0_503_mod_attr(D20_srt *srt_ent, D20_oct *oct_ent,
			Ds_at_entry *attr_entry);


/* in adm_dump/d20dpfile.c */
signed16 d20_116_read_dump_file(String file, char **names, D20_offset **pdn,
			D20_offset **dn, signed32 *dn_no, signed32 *attr_len,
			D20_delupd_info *del_info);
signed16 d20_118_write_dump_file(String file, D20_offset **pdn, signed32 dn_no,
			char *names, signed32 l_names, signed32 attr_len,
			D20_delupd_info *del_info);
signed16 d20_120_read_delupd_info(String file, signed32 *val,
			D20_delupd_info *del_info);

/* in adm_dump/d20gen.c */
d2_ret_val d20_106_get_dn_offsets(Bool ignore, int fd, D20_dns *ext_dn,
			D20_dns *int_dn, D20_dn_off *dn, D20_dn_off *mk,
			signed32 *no_bytes, signed32 *attr_len);
void d20_111_free(char *ptr1, char *ptr2, char *ptr3, char *ptr4, char *ptr5,
			char *ptr6, char *ptr7, char *ptr8, char *ptr9,
			char *ptr10);
void d20_norm(char *dn, unsigned16 len);

/* in adm_dump/d20gendn.c */
d2_ret_val d20_105_gen_dn(Bool ignore, String file, int *fd, char **names,
			signed32 *act_anz, D20_offset **pdn_off,
			D20_offset **dn_off, signed32 *dn_no, 
			signed32 *attr_len);

/* in adm_dump/d20getdns.c */
d2_ret_val d20_112_get_more_dns(D20_offset **pdn_off, signed16 dn_no,
			D20_offset **new_off, signed16 new_no, Bool *dn_exist);
d2_ret_val d20_113_search_dn(signed16 no, signed16 max_no, signed16 *rdn_pos,
			char *dn, Bool *obj_found);
void d20_114_get_rdns(char *dn, signed16 *no, signed16 *pos);
void d20_115_own_list(D20_offset **first, D20_offset **max, signed16 *no,
			signed16 max_no, signed16 *rdn_pos, char *dn);

/* in adm_dump/d20getdsa.c */
signed16 d20_180_get_dsa(char *dsa_disname, char *p_env);

/* in adm_dump/d20hdlsub.c */
signed16 d20_B_handle_subtrees(D20_pB *parblk, D2_name_string dst_dsa_name);

/* in adm_dump/d20rdwr.c */
signed16 d20_107_read(int fd, char *p_area, signed32 n);
signed16 d20_117_write(int fd, char *p_area, signed32 n, Bool out);
signed16 d20_119_read_attr(int fd, signed32 off, char *dn, signed32 attr_len,
			char *area, signed32 *min, signed32 *max, char **p_dn,
			char **p_attr);

/* in adm_dump/d20restore.c */
signed16 d20_100_restore(String file1, int fd1, char *name1, D20_offset **pdn1,
			D20_offset **dn1, signed16 dn_no1, signed32 attr1_len,
			String file2, Name_string dst_dsa_name);
signed32 d20_101_compare_dn(D20_offset **dn1, D20_offset **dn2);

/* in adm_dump/d20search.c */
signed16 d20_165_call_search(signed16 b_id, Bool single_ob, D2_c_arg *com_arg,
			D2_name_string subtree, char *filename, Bool second,
			int *fd1, char **name1, signed32 *l_name1,
			D20_offset **pdn1, D20_offset **dn1, signed32 *dn_no1,
			signed32 *attr1_len);


/* in adm_sch/d20admsch.c */
signed16 d20_2_admin_scheme(D20_pS *parblk);

/* in adm_sch/d20disp.c */
d2_ret_val d20_6_display_scheme(D20_p6 *parblk);

/* in adm_sch/d20dpat.c */
signed32  d20_013_display_at(signed16 nattr, At *at);

/* in adm_sch/d20dpoct.c */
signed32 d20_003_display_oct(signed16 nobj, Oct *oct);
void d20_004_fill_oct_mask(Oct *oct_entry);

/* in adm_sch/d20dpsrt.c */
signed32 d20_021_display_srt(signed16 nsrule, Srt *srt);
void d20_022_fill_srt_mask(Srt *srt_entry);

/* in adm_sch/d20gattr.c */
d2_ret_val d20_016_get_new_attr(Bool init, signed32 op_mode, At *attr);
d2_ret_val d20_015_get_attr(Bool init, char *abbrv);

/* in adm_sch/d20gobj.c */
d2_ret_val d20_005_get_obj(Bool init, char *abbrv);
d2_ret_val d20_006_get_new_obj(Bool init, signed32 op_mode, Oct *obj);
d2_ret_val d20_extract_acronyms(signed32 field_cnt, char *fields[],
			signed32 acr_limit, signed32 *no_acr, Abbrv *acr_arr,
			char *max_fld, signed32 *ret_code);

/* in adm_sch/d20gsrule.c */
d2_ret_val d20_025_get_srule(Bool init, signed32 *rule_nr);
d2_ret_val d20_026_get_new_srule(Bool init, signed32 op_mode, Srt *srule);

/* in adm_sch/d20hdlat.c */
signed32 d20_010_check_attr(Errcode *ret_code, S_upd_info *upd_info);
signed32 d20_011_add_attr(Errcode *ret_code, At *at_entry, signed32 *no_at,
			At **at);
signed32 d20_012_del_attr(char *abbrv, signed32 *no_at, At *at);
signed32 d20_013_mod_attr(At *at_entry, signed32 no_at, At *at);
signed32 d20_014_search_at_abbrv(char *abbrv, signed32 no_at, char *at,
			signed32 schema);
d2_ret_val d20_015_check_string(char *str);
d2_ret_val d20_016_check_obj_id(char *str);

/* in adm_sch/d20hdloct.c */
signed32 d20_020_check_obj(Errcode *ret_code, S_upd_info *upd_info);
signed32 d20_021_add_obj(Errcode *ret_code, Oct *oct_entry, signed32 *no_oct,
			Oct **oct);
signed32 d20_022_del_obj(char *abbrv, signed32 *no_oct, Oct *oct);
signed32 d20_023_mod_obj(Oct *oct_entry, signed32 no_oct, Oct *oct);
signed32 d20_024_search_obj_abbrv(char *abbrv, signed32 no_oct, char *oct,
			signed32 schema);

/* in adm_sch/d20hdlsrt.c */
signed32 d20_030_check_srule(Errcode *ret_code, S_upd_info *upd_info);
signed32 d20_031_add_srule(Errcode *ret_code, Srt *srt_entry, signed32 *no_srt,
			Srt **srt);
signed32 d20_032_del_srule(signed32 rule_nr, signed32 *no_srt, Srt *srt);
signed32 d20_033_mod_srule(Srt *srt_entry, signed32 no_srt, Srt *srt);
signed32 d20_034_search_srule(signed32 rule_nr,signed32 no_srt,Srt *srt);
signed32 d20_035_search_na_oclass(Abbrv nam_abbrv, Abbrv oclass,
			signed32 no_oct, Oct *oct);

/* in adm_sch/d20ldsch.c */
d2_ret_val d20_027_load(char *file, signed32 *nrsrt, Srt **psrt,
			signed32 *nroct, Oct **poct, signed32 *nrat, At **pat);

/* in adm_sch/d20ldsh.c */
signed32 gds_ldsh(Errcode *return_code, signed16 version,char *file_name,
			signed32 *no_srt, Srt **srt,signed32 *no_oct, Oct **oct,
			signed32 *no_at, At **at);

/* in adm_sch/d20read.c */
d2_ret_val d21_21_rdsh(signed16 bind_id,Errcode *ret_code,signed16 version,
			signed32 *no_srt, Srt **srt, signed32 *no_oct,Oct **oct,
			signed32 *no_at, At **at);

/* in adm_sch/d20sadmsch.c */
d2_ret_val d20_store_admsch(FILE *fid, signed32 no_admat, D20_at *admat,
			signed32 no_admoct, D20_oct *admoct, signed32 no_admsrt,
			D20_srt *admsrt);

/* in adm_sch/d20store.c */
signed32 d21_22_stsh(signed16 bind_id,Errcode *ret_code,signed16 version,
			char *file_name, signed32 no_srt, Srt *srt,
			signed32 no_oct, Oct *oct, signed32 no_at, At *at);

/* in adm_sch/d20transf.c */
d2_ret_val d20_sch_trans(signed32 no_at, At *at, signed32 no_oct, Oct *oct,
			signed32 no_srt, Srt *srt, signed32 *no_admat,
			D20_at **admat, signed32 *no_admoct, D20_oct **admoct,
			signed32 *no_admsrt, D20_srt **admsrt);

/* in adm_sch/d20update.c */
signed32 gds_upsh(Errcode *ret_code, signed16 version, S_upd_info *upd_info);

/* in adm_sch/d20updsch.c */
d2_ret_val d20_032_update(S_upd_info *upd_info);


/* from delupd/d20create.c */
d2_ret_val d20_C00_create_shadow ( D20_pE * , signed32 );

/* from delupd/d20cupd.c */
signed16 d20_D_cache_update ( D20_pE * );

/* from delupd/d20delupd.c */
signed16 d20_C_delta_update ( D20_pE * );

/* from delupd/d20delutil.c */
d2_ret_val d20_C05_read_obj ( D20_pE * , Bool , Name_string * , signed32 * );
d2_ret_val d20_C06_read_dsa ( char * , char * );
d2_ret_val d20_C07_get_time ( Upd_time * , Bool );

/* from delupd/d20display.c */
void d20_C03_display_jobs ( signed16 , signed16 , Bool * );
void d20_C08_copy_name ( char * , char * , char * );
signed32 d20_C10_display_job_mask ( Bool );
void d20_C11_fill_job_mask ( signed32 , Upd_time * );

/* from delupd/d20errfile.c */
d2_ret_val d20_C04_show_error_file ( char * , signed16 , signed16 );

/* from delupd/d20remove.c */
d2_ret_val d20_C01_remove_shadow ( D20_pE * , Bool , signed32 );
signed16 d20_099_set_install_alias ( signed16 , D20_offset ** , signed32 ,
			char * , char * , signed32 );

/* from delupd/d20update.c */
d2_ret_val d20_C02_update_job ( D20_pE * , Bool );

/* from delupd/d20updconfig.c */
d2_ret_val d20_turn_shadowing_job( signed16, d2_upd_status );
d2_ret_val d20_check_update( signed16, char * );

/* from delupd/d20updutil.c */
signed32 d20d050_update_dsa ( D2_pbhead * , Bool , signed16 , D2_name_string ,
			signed32 , char * , signed32 );

/* from delupd/d21cache.c */
d2_ret_val d21_025_update ( signed32 , Errcode * , signed16 , signed32 ,
			Upd_time * );
d2_ret_val d21d027_getjb ( char * , Errcode * , signed32 , signed32 ,
			Shd_job * );

/* from delupd/d21dpsub.c */
d2_ret_val d21_16_dump ( signed16 , Errcode * , signed16 , Name_string ,
			Name_string , signed32 , C_arg * , String );

/* from delupd/d21dutil.c */
d2_ret_val d21d002_check_job ( signed16 ,char * , signed32 , Shd_job * , Bool , 
			Errcode * , Subtree ** , Bool * , signed32 * );
d2_ret_val d21d004_add_job ( signed16 , Shd_job * , Bool , char * , signed32 ,
			signed32 , char * , char * , Errcode * );
d2_ret_val d21d005_upd_job_file ( signed16 , signed32 , Shd_job * , char * , 
			signed32 , Errcode * );

/* from delupd/d21getjob.c */
d2_ret_val d21_20_getjb ( signed16 , Errcode * , signed16 , signed32 ,
			Shd_job * );

/* from delupd/d21gnshd.c */
d2_ret_val d21_17_gnshd ( signed16 , Errcode * , signed16 , signed32 ,
			Name_string , signed32 , String , Name_string , Bool , 
			Subtree ** );

/* from delupd/d21lock.c */
d2_ret_val d21d008_open_and_lock ( char * , signed32 , Bool , Errcode * );

/* from delupd/d21mabind.c */
d2_ret_val d21d019_master_bind ( signed16 , Errcode * , char * ,
			D2_name_string , signed16 , signed16 * , char * );
d2_ret_val d21d022_unbind ( signed16 );

/* from delupd/d21param.c */
d2_ret_val d21d006_param_check ( signed16 , Errcode * , signed16 , char ** ,
			signed32 * );

/* from delupd/d21remove.c */
d2_ret_val d21_18_rmshd ( signed16 , Errcode * , signed16 , Name_string ,
			signed32 , Name_string , signed32 );

/* from delupd/d21time.c */
d2_ret_val d21d001_check_time ( Upd_time * );
void d21d012_add_upd_time ( Upd_time * );
void d21d013_del_upd_time ( Upd_time * );
void d21d017_store_old_times(void);
d2_ret_val d21d016_gen_cron_entry ( char * , signed32 );

/* from delupd/d21trace.c */
void d21d010_trace_job ( Name_string , Name_string , signed32 );
void d21d011_trace_upd_mode_and_times ( signed32 , Upd_time * );

/* from delupd/d21update.c */
d2_ret_val d21_19_updat ( signed16 , Errcode * , signed16 , signed32 , 
			Name_string , signed32 , Name_string , Upd_time * );


/* in adm_iapl/d20bind.c */
d2_ret_val d20__bind(D2_name_string dsa_name, signed16 *bind_id);

/* in adm_iapl/d20calld.c */
signed16 map_errnum(signed16 errnum);
signed16 ds4_b11_call_directory (signed16 operation);

/* in adm_iapl/d20dnget.c */
d2_ret_val d20_dnget (signed16 mode, D2_name_string *vptr);

/* in adm_iapl/d20fattr.c */
void ds4_b02_fill_attr (signed16 optype, Ds_at_entry *attr_ent, signed16 no_attr);
signed32 d20_copy_val(Ds_at_entry *attr_ent);
signed32 d20_copy_hex(unsigned16 len, Octet_string val, Bool hex_use);

/* in adm_iapl/d20fentry.c */
void ds4_b01_fill_entry (signed16 optype, Ds_obj_entry *obj_entry,
			signed16 no_np, char tree[]);

/* in adm_iapl/d20fentryr.c */
void ds4_b03_fill_obj_fields (D2_name_string name, char *ifields[]);

/* in adm_iapl/d20ffilt.c */
void d20_ffilter (signed16 optype, Ds_at_entry *attr_ent, signed16 no_attr);

/* in adm_iapl/d20fmodify.c */
void d20_fill_modify (signed16 optype, Ds_at_entry *attr_ent, signed16 no_attr);

/* in adm_iapl/d20getattr.c */
d2_ret_val d20_get_attribute (signed16 mode, D2_a_type *type,
			D2_a_value **a_val);

/* in adm_iapl/d20gfattr.c */
d2_ret_val d20_attr_from_message(signed16 mode, Bool check, char *area,
			signed32 max_len, D2_a_type *type, signed16 **len,
			unsigned16 **rep, char **vptr, char **p_mess,
			char **p_max, D2_a_type *at_type);

/* in adm_iapl/d20iapl.c */
d2_ret_val d20_call_iapl (signed16 operation);
d2_ret_val d20_a0_read (D2_pbhead *pbhead);
d2_ret_val d20_a1_addobj (D2_pbhead *pbhead);
d2_ret_val d20_a2_delobj (D2_pbhead *pbhead);
d2_ret_val d20_a3_search (D2_pbhead *pbhead);
d2_ret_val d20_a6_modattr (D2_pbhead *pbhead);
d2_ret_val d20_a7_addalias (D2_pbhead *pbhead);

/* in adm_iapl/d20init.c */
d2_ret_val d20_init (void);

/* in adm_iapl/d20interf.c */
d2_ret_val d20_a4_addattr (D2_pbhead *pbhead);
d2_ret_val d20_a5_delattr (D2_pbhead *pbhead);
d2_ret_val d20_a11_modrdn (D2_pbhead *pbhead);

/* in adm_iapl/d20salias.c */
void d20_ins_alias (char *aliased_obj);
void d20_res_alias (void);

/* in adm_iapl/d20sattr.c */
d2_ret_val d20_insert_attr (signed16 optype, D2_a_type *type, signed16 len,
			unsigned16 rep, char *val, char **at_val_area);

/* in adm_iapl/d20setinfo.c */
void d20_160_set_info(signed16 b_id, D2_c_arg *com_arg, char *file);
void d20_161_reset_info(void);
void d20_162_set_name(String dn);
void d20_164_get_subset(D2_subset *subset);
void d20_166_set_all_attr(void);
void d20_167_set_file_mode(void);
void d20_168_set_serv_cntrls(void);
void d20_169_reset_serv_cntrls(void);
void d20_170_set_req_attr(signed16 no, D2_a_type *types);
void d20_172_set_master_serv_cntrls(void);
void d20_173_init_serv_cntrls(signed32 s_cntrls);

/* in adm_iapl/d20smodify.c */
d2_ret_val d20_insert_modify (signed16 optype, signed16 opmode,
			D2_a_type *type1, signed16 len1, unsigned16 rep1,
			Octet_string val1, D2_a_type *type2, signed16 len2,
			unsigned16 rep2, Octet_string val2, char **at_val_area);

/* in adm_iapl/d20specdir.c */
signed16 d20_spec_call_dir(char *p_env, signed16 dsa_typ);
signed16 d20_delete_default_dsa(void);

/* in adm_iapl/d20subset.c */
void d20_set_subset (D2_subset subset);

/* in adm_iapl/d20unbind.c */
d2_ret_val d20__unbind(signed16 bind_id);

/* in adm_iapl/mess.c */
float mess(int p, char *s, float toverbr, int count);


/* in adm_tree/d20append.c */
signed32 d20_E01_append(D20_pE *parblk);

/* in adm_tree/d20chgmas.c */
signed32 d20_E05_chgmas(D20_pE *parblk);

/* in adm_tree/d20chgnam.c */
signed32 d20_E03_chgnam(D20_pE *parblk);

/* in adm_tree/d20chngall.c */
void ds0_505_change_all_attr(signed32 subset);
signed32 d20_E06_modify(D20_pE  *parblk);

/* in adm_tree/d20copy.c */
signed32 d20_E02_copy(D20_pE *parblk);

/* in adm_tree/d20delete.c */
signed32 d20_E04_delete(D20_pE *parblk);

/* in adm_tree/d20save.c */
signed32 d20_E00_save(D20_pE *parblk);

/* in adm_tree/d20tree.c */
signed16 d20_E_tree_handling(D20_pE *parblk);

/* in adm_tree/d21addap.c */
signed16 d21t003_add_appnd(int fd, D20_offset *dn, char *at_mem,
			signed32 *min_pos, signed32 *max_pos, signed32 inherit,
			Name_string parent_obj, Name_string subtree,
			signed32  new_parent, Name_string bind_dsa,
			signed16 lenacl, char *pacl, char *at_val_area,
   			signed16 use_flag);
signed32 d20_match_rdn_type(D2_a_type *type, signed32 no_nam_attr,
			D2_a_type nam_type[]);
signed32 d20_121_get_nam_attr(D2_name_string p_dn, D2_a_type type[]);

/* in adm_tree/d21apsub.c */
d2_ret_val d21_12_appnd(signed16 bind_id, Errcode *return_code,
			signed16 version, String file_name, 
			Name_string parent_name, signed32 inherit,
			Bool overwrite, Name_string **old_objects);

/* in adm_tree/d21chdnl.c */
d2_ret_val d21t065_change_dnlist(char *old_dn, char *new_dn, signed16 bind_id,
			Errcode *return_code);
d2_ret_val d21t066_chdnl(char *old_dn, char *new_dn, signed16 bind_id,
			Errcode *return_code);

/* in adm_tree/d21checkp.c */
d2_ret_val d21t009_param_check(signed16 bind_id, Errcode *return_code,
			signed16 version, signed16 *dir_id);

/* in adm_tree/d21chmas.c */
signed32 d21_13_chmas(signed16 bind_id, Errcode *return_code, signed16 version,
			Name_string subtree_root, Name_string old_master,
			Name_string new_master, Name_string **not_changed);
signed16 d21t050_change_master(char *filename, Errcode *return_code, D21_p5 *pb,
			D21_pC *pbmod, Name_string **not_changed,
			Name_string old_master, Name_string new_master,
			char *bind_dsa, signed16 dir_id, signed16 bind_id,
			signed16 *new_bind_id);
Bool d21t051_is_parent(char *parent, char *son);
 
/* in adm_tree/d21chnam.c */
signed32 d21_15_chnam(signed16  bind_id, Errcode *return_code, signed16 version,
			Name_string old_dname, Name_string new_dname,
			Bool overwrite, Name_string **conflicts);
D20_offset **d21t040_search_dn(D20_offset **first, signed16 num_dn, char *beg2,
			Name_string object);
signed16 d21t042_filter_obj(char *obj, char *subtree);
signed16 d21t044_get_memory(signed32 dn_no, D20_rdn_tbl **num_rdn,
			D20_rdn_tbl **prdn, signed32 attr_len, char **at_mem,
			signed32 attr_len2, char **at_mem2, char **area_aux);
void  d21t045_build_dn(char *old_dn, char *new_dn, char *name, char *n_name);
signed16 d21t046_dsa_table(D20_dsa_tbl num_dsa[], char *look_dsa);
signed16 d21t047_bind_dsa(char *curr_dsa, char *new_dsa, signed16 *new_bind,
			signed16 dir_id, D2_c_arg *c_arg);
void d21t048_ins_rdn(D20_rdn_tbl **first_prdn, D20_rdn_tbl *first_num_rdn,
			signed32 *n_elem, char *object, signed16 index,
			D20_offset **pdn1, signed32 dn_no1, D20_offset **pdn2,
			signed32 dn_no2, char *old_dn, char *new_dn);
signed16 d21t049_hdl_obj(int fd1, D20_offset **first_dn1, char *name1,
			signed32 dn_no1, char *at_mem1, signed32 *min_pos1,
			signed32 *max_pos1, int fd2, D20_offset **first_dn2,
			char *name2, signed32 dn_no2, char *at_mem2,
			signed32 *min_pos2, signed32 *max_pos2,
			char *at_val_area, Name_string old_dn,
			Name_string new_dn, Name_string object,
			Name_string bind_dsa);

/* in adm_tree/d21confl.c */
d2_ret_val d21t068_conflicts(signed32 dn_no, D20_offset **pdn,
			Name_string **conflicts, Bool *dn_name_col,
			char *old_dn, char *new_dn);

/* in adm_tree/d21cpsub.c */
d2_ret_val d21_11_copy(signed16 source_bind_id, Errcode *return_code,
			signed16 version, Name_string object, signed32 subset,
			C_arg *c_arg, signed16 sink_bind_id,
			Name_string parent_name, signed32 inherit,
			Bool overwrite, Name_string **old_objects);

/* in adm_tree/d21delst.c */
signed32 d21_10_delst(signed16 bind_id, Errcode *return_code, signed16 version,
			Name_string subtree_root, signed32 serv_cntrls,
			Name_string **not_deleted);
signed32 d21t030_rem_objects(char *filename, Errcode *return_code, D21_p5 *pb,
			D21_p8 *pbdel, Name_string **not_deleted, Bool nocache,
			char *bind_dsa);
signed32 d21t031_handle_dns(int fd, D20_offset **dn, signed32 dn_no, D21_p5 *pb,
			D21_p8 *pbdel, Bool *b_notdel, Bool nocache,
			Errcode *return_code, char *bind_dsa, char *d20_beg);
signed32 d21t032_get_search_file(int *fd, char *filename, Errcode *return_code,
			D20_dn_off *dn, D20_dn_off *mk, D20_dns *ext_dn,
			D20_dns *int_dn, signed32 *attr_len);
Bool d21t033_parent_object_name(char *obj, char *par_obj);
signed32 d21t034_not_deleted(signed32 dn_no, D20_offset **pdn,
			Name_string **not_deleted, Bool *dn_name_col);

/* in adm_tree/d21hdldn.c */
signed32 d21t005_compare_dn_appnd(char *dn1, D20_offset **dn2, char *beg2);
signed16 d21t001_hdldns_appnd(int fd1, D20_offset **dn1, signed32 *dn_no1,
			char *at1_mem, int fd2, D20_offset **dn2,
			signed32 dn_no2, char *at2_mem, signed32 inherit,
			Bool overwrite, char *pacl, signed16 lenacl,
			signed32 new_parent, Name_string parent_obj,
			Name_string subtree, Name_string bind_dsa,
			Bool *name_col, char *at_val_area);
d2_ret_val d21t010_change_dn(Name_string old_name, Name_string new_par,
			Name_string old_par, char *new_dn);

/* in adm_tree/d21modap.c */
signed16 d21t004_modify_appnd(int fd1, D20_offset *dn1, char *at1_mem,
			signed32 *min1, signed32 *max1, int fd2,
			D20_offset *dn2, char *at2_mem, signed32 *min2,
			signed32 *max2, signed32 inherit,
			Name_string parent_obj, Name_string subtree,
			signed32 new_parent, char *pacl, signed16 lenacl,
			Name_string bind_dsa, Bool overwrite, Bool *name_col,
			char *at_val_area, signed16 use_flag);

/* in adm_tree/d21oldobj.c */
d2_ret_val d21t002_old_objects(signed16 dn_no, D20_offset **pdn,
			Name_string **old_objects, Bool *dn_name_col,
			Name_string old_par, Name_string new_par);

/* in adm_tree/d21restor.c */
signed16 d21t000_restore_appnd(int fd1, D20_offset **pdn1, D20_offset **dn1,
			signed32 dn_no1, char *name1, signed32 attr1_len,
			String file2, signed32 inherit, Bool overwrite,
			signed32 new_parent, Name_string **old_objects,
			Name_string parent_obj, Name_string subtree,
			signed16 bind_id);
signed32 max_comp_attr(void);
signed32 d21t006_new_parent(char *parent_obj, char *subtree);
signed32 d21t007_set_status_flag(char *str1, char *str2);
d2_ret_val d21t008_get_attr(int fd, D20_offset *dn, char *at_mem,
			signed32 *min_pos, signed32 *max_pos,
			D2_a_type attr_type, char **attr_val,
			signed16 *attr_len, char *beg);

/* in adm_tree/d21svsub.c */
d2_ret_val d21_14_save(signed16 bind_id, Errcode *return_code, signed16 version,
			Name_string object, signed32 subset,
			signed32 serv_cntrls, String file_name);


/* in usradmin/d20m_init.c */
d2_ret_val d20_mask_init(nl_catd nl_fd, signed32 msg_set, char **str,
   			D20_omask  *m_ptr);
d2_ret_val d20_read_str(nl_catd nl_fd, signed32 msg_set, signed32 max_no,
   			char **str, char **str_array);

/* in usradmin/d20param.c */
Bool d20_displ_mask(signed32 act_params, D20_omask *pmask, signed32 sel_cmd,
			signed32  *fkt_key);
signed32 d20_get_str(char *str, char *name, signed32 max_length, char param);
signed32 d20_get_val(char *str, signed32 min, signed32 max, signed32 *val);
signed32 d20_get_params(int argc, char *argv[], char *dir, signed32 *cmd,
			signed32 *act_params, Bool *all_params);
void d20_disp_clhelp(void);

/* in usradmin/d20admin.c */
d2_ret_val is_allowed(char *name);

#endif		/* _D20PROTO_H */
