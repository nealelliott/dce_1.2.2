# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.3  1996/11/13  17:57:50  arvind
# 	CHFts20476: etype failures (add des_md5)
# 	[1996/10/24  21:24 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_4/1]
#
# 	Do not build t_walk_rtree.o or t_kerb.o; they are tests and not part
# 	of libmitkrb5.a.
# 	[1996/05/29  20:32 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
#
# 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
# 	[1996/05/20  19:51 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/3]
#
# 	Look in DCE directories for DES_OFILES - do not use those
# 	provided by MIT because of export issues.
# 	[1996/05/03  20:00 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2]
#
# 	Remove t_std_conf.o from libmitkrb5.a; it is a test module.
# 	[1996/05/03  20:00 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2]
#
# Revision 1.1.2.2  1996/10/03  14:39:59  arvind
# 	Add -DDES_NEUTER when building USE_DES=0
# 	[1996/09/11  15:26 UTC  jrr  /main/DCE_1.2.2/jrr_122_4/1]
# 
# 	Do not build t_walk_rtree.o or t_kerb.o; they are tests and not part
# 	of libmitkrb5.a.
# 	[1996/05/29  20:32 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
# 
# 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
# 	[1996/05/20  19:51 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/3]
# 
# 	Look in DCE directories for DES_OFILES - do not use those
# 	provided by MIT because of export issues.
# 	[1996/05/03  20:00 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2]
# 
# 	Remove t_std_conf.o from libmitkrb5.a; it is a test module.
# 	[1996/05/03  20:00 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2]
# 
# Revision 1.1.2.1  1996/06/04  21:50:51  arvind
# 	Merge krb5 work to DCE1.2.2 mainline
# 	[1996/05/30  21:16 UTC  mullan_s  /main/DCE_1.2.2/2]
# 
# 	Do not build t_walk_rtree.o or t_kerb.o; they are tests and not part
# 	of libmitkrb5.a.
# 	[1996/05/29  20:32 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
# 
# 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
# 	[1996/05/20  19:51 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/3]
# 
# 	Look in DCE directories for DES_OFILES - do not use those
# 	provided by MIT because of export issues.
# 	[1996/05/03  20:00 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2]
# 
# 	Remove t_std_conf.o from libmitkrb5.a; it is a test module.
# 	[1996/05/03  20:00 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2]
# 
# $EndLog$
#
# Copyright (c) Hewlett-Packard Company 1996
# Unpublished work. All Rights Reserved.
#
#
# MIT krb5
# mit_krb5/lib/krb5 Makefile

# See VPATH for location of DES objects - get them from DCE to avoid export issues
DES_OFILES =  cksum.o cbc_cksum.o \
        cbc_encrypt.o des_tables.o ecb_encrypt.o   \
        make_sched.o pcbc_encrypt.o desglue.o des_md5.o new_rn_key.o      \
        process_ky.o init_rkey.o random_key.o string2key.o      \
        finish_key.o fin_rndkey.o key_parity.o weak_key.o

libmitkrb5.a_OFILES = asn1_decode.o asn1_encode.o asn1_get.o asn1_k_decode.o \
	asn1_k_encode.o asn1_make.o asn1_misc.o asn1buf.o glue2.o krb5_decode.o \
	krb5_encode.o fcc_close.o fcc_defops.o fcc_destry.o fcc_errs.o fcc_eseq.o \
	fcc_gennew.o fcc_getnam.o fcc_gprin.o fcc_init.o fcc_maybe.o fcc_nseq.o \
	fcc_ops.o fcc_read.o fcc_reslv.o fcc_retrv.o fcc_sflags.o fcc_skip.o \
	fcc_sseq.o fcc_store.o fcc_write.o scc_close.o scc_defops.o \
	scc_destry.o scc_errs.o scc_eseq.o scc_gennew.o scc_getnam.o scc_gprin.o \
	scc_init.o scc_maybe.o scc_nseq.o scc_ops.o scc_read.o scc_reslv.o \
	scc_retrv.o scc_sflags.o scc_skip.o scc_sseq.o scc_store.o \
	scc_write.o ccbase.o ccdefault.o ccdefops.o init_ets.o f_addr.o \
	f_address.o f_ap_rep.o f_ap_req.o f_arep_enc.o f_auth_cnt.o f_authdata.o \
	f_authent.o f_chksum.o f_cred.o f_cred_cnt.o f_cred_enc.o f_creds.o \
	f_enc_kdc.o f_enc_tkt.o f_error.o f_kdc_rp.o f_kdc_rq.o f_keyblock.o \
	f_last_req.o f_padata.o f_princ.o f_priv.o f_priv_enc.o f_pwd_data.o \
	f_pwd_seq.o f_safe.o f_tckt.o f_tckts.o f_tgt_cred.o f_tkt_auth.o \
	ktf_add.o ktf_close.o ktf_defops.o ktf_endget.o ktf_g_ent.o ktf_g_name.o \
	ktf_next.o ktf_ops.o ktf_remove.o ktf_resolv.o ktf_ssget.o ktf_util.o \
	ktf_wops.o ktf_wreslv.o ktadd.o ktbase.o ktdefault.o ktfr_entry.o \
	ktremove.o read_servi.o addr_comp.o addr_order.o addr_srch.o \
	auth_con.o bld_pr_ext.o bld_princ.o chk_trans.o compat_recv.o \
	conv_princ.o copy_addrs.o copy_athctr.o copy_auth.o \
	copy_cksum.o copy_creds.o copy_data.o copy_key.o copy_princ.o \
	copy_tick.o cp_key_cnt.o decode_kdc.o decrypt_tk.o encode_kdc.o \
	encrypt_tk.o faddr_ordr.o free_rtree.o gc_frm_kdc.o gc_via_tkt.o \
	gen_seqnum.o gen_subkey.o get_creds.o get_in_tkt.o in_tkt_ktb.o \
	in_tkt_pwd.o in_tkt_sky.o init_ctx.o kdc_rep_dc.o krbconfig.o \
	mk_cred.o mk_error.o mk_priv.o mk_rep.o mk_req.o mk_req_ext.o \
	mk_safe.o parse.o pr_to_salt.o preauth.o princ_comp.o rd_cred.o \
	rd_error.o rd_priv.o rd_rep.o rd_req.o rd_req_dec.o rd_safe.o \
	recvauth.o send_tgs.o sendauth.o srv_rcache.o \
	tgtname.o unparse.o walk_rtree.o DNR.o an_to_ln.o ccdefname.o def_realm.o \
	free_hstrl.o free_krbhs.o full_ipadr.o gen_port.o gen_rname.o genaddrs.o \
	get_krbhst.o gmt_mktime.o hst_realm.o init_os_ctx.o krbfileio.o \
	ktdefname.o kuserok.o localaddr.o locate_kdc.o lock_file.o macsock.o \
	mk_faddr.o net_read.o net_write.o osconfig.o port2ip.o read_msg.o \
	read_pwd.o realm_dom.o sendto_kdc.o sn2princ.o timeofday.o \
	unlck_file.o ustime.o write_msg.o rc_base.o rc_conv.o rc_dfl.o rc_io.o \
	rcdef.o asn1_err.o kdb5_err.o krb5_err.o kv5m_err.o adm_err.o \
	prof_tree.o prof_file.o prof_parse.o prof_err.o prof_init.o \
	decrypt_key.o encrypt_key.o fetch_mkey.o kdb_dbm.o setup_mkey.o \
	store_mkey.o verify_mky.o md4.o md4crypto.o md4glue.o md5.o md5crypto.o \
	md5glue.o rnd_confoun.o crc.o ${DES_OFILES} com_err.o error_message.o \
	et_name.o cryptoconf.o

.if defined (USE_DES)
DES_DIR = domestic
.else
DES_DIR = international
SECURITY_CFLAGS += -DDES_NEUTER
.endif

VPATH	= ./asn.1:./ccache/file:./ccache/stdio:./ccache:./error_tables:./free:./keytab/file:./keytab:./krb:./os:./rcache:./../crypto/crc32:./../crypto/md4:./../crypto/md5:./../crypto/os:./../crypto:./../kdb:./../../util/profile:./../../util/et:./../../../security/krb5/lib/crypto/common:./../../../security/krb5/lib/crypto/${DES_DIR}

INCFLAGS = -I${EXPORTBASE}/usr/include/mitkrb5 -I${EXPORTBASE}/usr/include/mitkrb5/krb5

CFLAGS = -DOSF_DCE -DDCE_COMPAT ${${TARGET_MACHINE}_CFLAGS} ${SECURITY_CFLAGS}

LIBRARIES = libmitkrb5.a
EXPLIB_TARGETS	= export_libmitkrb5.a

# Directory in which to place exported files
EXPDIR = /usr/lib/mitkrb5/

.include <${RULES_MK}>

ccdefops.o_INCFLAGS = ${INCFLAGS} -I./ccache/file
cryptoconf.o_INCFLAGS = ${INCFLAGS} -I./../crypto/md4 -I./../crypto/crc32

rnd_confoun.o_CFLAGS = ${CFLAGS} -DHAVE_SRAND48=1 -DHAVE_SRAND=1 -DHAVE_GETPID=1
error_message.o_CFLAGS = ${CFLAGS} -DNO_YYLINENO=1 -DNEED_SYS_ERRLIST=1 -DHAS_STRERROR=1 -DHAVE_STDARG_H=1 -DHAVE_STDLIB_H=1
kdb_dbm.o_CFLAGS = ${CFLAGS} -DHAVE_UNISTD_H=1
gmt_mktime.o_CFLAGS = ${CFLAGS} -DTIME_WITH_SYS_TIME=1 -DHAVE_SYS_TIME_H=1
lock_file.o_CFLAGS = ${CFLAGS} -DHAVE_UNISTD_H=1
sendto_kdc.o_CFLAGS = ${CFLAGS} -DHAVE_SYS_TIME_H=1
timeofday.o_CFLAGS = ${CFLAGS} -DPOSIX_TYPES=1
fcc_maybe.o_CFLAGS = ${CFLAGS} -DHAVE_UNISTD_H=1
com_err.o_CFLAGS = ${CFLAGS} -DNEED_SYS_ERRLIST=1 -DHAVE_STDARG_H=1 -DHAVE_STDLIB_H=1
et_name.o_CFLAGS = ${CFLAGS} -DNEED_SYS_ERRLIST=1 -DHAVE_STDLIB_H=1
init_et.o_CFLAGS = ${CFLAGS} -DHAVE_STDLIB_H=1
def_realm.o_CFLAGS = ${CFLAGS} -DOLD_CONFIG_FILES=1
get_krbhst.o_CFLAGS = ${CFLAGS} -DOLD_CONFIG_FILES=1
hst_realm.o_CFLAGS = ${CFLAGS} -DOLD_CONFIG_FILES=1
init_os_ctx.o_CFLAGS = ${CFLAGS} -DOLD_CONFIG_FILES=1
osconfig.o_CFLAGS = ${CFLAGS} -DOLD_CONFIG_FILES=1
realm_dom.o_CFLAGS = ${CFLAGS} -DOLD_CONFIG_FILES=1

# These are DCE DES source files which need the MIT_KRB5 define
# to build with the MIT krb5 include files.
new_rn_key.o_CFLAGS = -DMIT_KRB5 ${CFLAGS}
desglue.o_CFLAGS = -DMIT_KRB5 ${CFLAGS}
random_key.o_CFLAGS = -DMIT_KRB5 ${CFLAGS}
string2key.o_CFLAGS = -DMIT_KRB5 ${CFLAGS}
