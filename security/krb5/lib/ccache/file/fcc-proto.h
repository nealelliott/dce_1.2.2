/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc-proto.h,v $
 * Revision 1.1.8.1  1996/06/04  22:01:01  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:30 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Kerb5Beta5 KRB5_FCC_FVNO_3 merge.
 * 	[1996/01/03  19:13 UTC  psn  /main/DCE_1.2/1]
 *
 * 	CHFts14474: optimize krb credential cache
 * 	[1995/06/30  16:31 UTC  aha  /main/HPDCE02/1]
 *
 * 	CHFts14474: optimize krb cache;
 * 	add prototypes for krb5_fcc_read_cred(), krb5_fcc_debug_cred(), aut,
 * 	authdata_match().
 * 	[1995/06/29  13:50 UTC  aha  /main/aha_mothra6/2]
 *
 * 	CHFts14474: optimize krb cred cache;
 * 	export two new functions: krb5_fcc_next_unexpired_cred() and krb5_fcc_sizeof_cred()
 * 	[1995/06/14  15:47 UTC  aha  /main/aha_mothra6/1]
 *
 * Revision 1.1.6.2  1996/02/18  23:03:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:50  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:46:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/30  16:31 UTC  aha
 * 	CHFts14474: optimize krb credential cache
 * 
 * 	HP revision /main/aha_mothra6/2  1995/06/29  13:50 UTC  aha
 * 	CHFts14474: optimize krb cache;
 * 	 add prototypes for krb5_fcc_read_cred(), krb5_fcc_debug_cred(), aut,
 * 	 authdata_match().
 * 
 * 	HP revision /main/aha_mothra6/1  1995/06/14  15:47 UTC  aha
 * 	CHFts14474: optimize krb cred cache;
 * 	export two new functions: krb5_fcc_next_unexpired_cred() and krb5_fcc_sizeof_cred()
 * 	[1995/12/08  16:57:47  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:19  sommerfeld
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:51:19  sommerfeld]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc-proto.h,v $
 * $Author: arvind $
 * $Id: fcc-proto.h,v 1.1.8.1 1996/06/04 22:01:01 arvind Exp $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * Prototypes for File-based credentials cache
 */


#ifndef KRB5_FCC_PROTO__
#define KRB5_FCC_PROTO__

/* fcc_close.c */
krb5_error_code krb5_fcc_close PROTOTYPE((krb5_ccache id ));

/* fcc_defnam.c */
char *krb5_fcc_default_name PROTOTYPE((void ));

/* fcc_destry.c */
krb5_error_code krb5_fcc_destroy PROTOTYPE((krb5_ccache id ));

/* fcc_eseq.c */
krb5_error_code krb5_fcc_end_seq_get PROTOTYPE((krb5_ccache id , krb5_cc_cursor *cursor ));

/* fcc_gennew.c */
krb5_error_code krb5_fcc_generate_new PROTOTYPE((krb5_ccache *id ));

/* fcc_getnam.c */
char *krb5_fcc_get_name PROTOTYPE((krb5_ccache id ));

/* fcc_gprin.c */
krb5_error_code krb5_fcc_get_principal PROTOTYPE((krb5_ccache id , krb5_principal *princ ));

/* fcc_init.c */
krb5_error_code krb5_fcc_initialize PROTOTYPE((krb5_ccache id , krb5_principal princ ));

/* fcc_nseq.c */
krb5_error_code krb5_fcc_next_cred PROTOTYPE((krb5_ccache id , krb5_cc_cursor *cursor , krb5_creds *creds ));
#ifdef OSF_DCE
krb5_error_code krb5_fcc_next_unexpired_cred PROTOTYPE((krb5_ccache id , krb5_cc_cursor *cursor , krb5_creds *creds ));
#endif

/* fcc_read.c */
krb5_error_code krb5_fcc_read_principal PROTOTYPE((krb5_ccache id , krb5_principal *princ ));
krb5_error_code krb5_fcc_read_keyblock PROTOTYPE((krb5_ccache id , krb5_keyblock *keyblock ));
krb5_error_code krb5_fcc_read_data PROTOTYPE((krb5_ccache id , krb5_data *data ));
krb5_error_code krb5_fcc_read_int32 PROTOTYPE((krb5_ccache id , krb5_int32 *i ));
krb5_error_code krb5_fcc_read_ui_2 PROTOTYPE((krb5_ccache id , krb5_ui_2 *i ));
krb5_error_code krb5_fcc_read_octet PROTOTYPE((krb5_ccache id , krb5_octet *i ));
krb5_error_code krb5_fcc_read_times PROTOTYPE((krb5_ccache id , krb5_ticket_times *t ));
krb5_error_code krb5_fcc_read_addrs PROTOTYPE((krb5_ccache, krb5_address ***));
krb5_error_code krb5_fcc_read_addr PROTOTYPE((krb5_ccache, krb5_address *));
krb5_error_code krb5_fcc_read_authdata PROTOTYPE((krb5_ccache , krb5_authdata ***));
krb5_error_code krb5_fcc_read_authdatum PROTOTYPE((krb5_ccache , krb5_authdata *));
#ifdef OSF_DCE
krb5_error_code krb5_fcc_read_cred PROTOTYPE((krb5_ccache id , krb5_creds *creds ));
#endif

/* fcc_reslv.c */
krb5_error_code krb5_fcc_resolve PROTOTYPE((krb5_ccache *id , char *residual ));

/* fcc_retrv.c */
krb5_error_code krb5_fcc_retrieve PROTOTYPE((krb5_ccache id , krb5_flags whichfields , krb5_creds *mcreds , krb5_creds *creds ));
#ifdef OSF_DCE
krb5_boolean authdata_match
    PROTOTYPE ((krb5_authdata * const *, krb5_authdata * const *));
void krb5_fcc_debug_cred PROTOTYPE((char *msg,
                                    krb5_creds *creds,
                                    int offset,
                                    krb5_error_code error,
                                    krb5_flags matchfields ));
#endif

/* fcc_sseq.c */
krb5_error_code krb5_fcc_start_seq_get PROTOTYPE((krb5_ccache id , krb5_cc_cursor *cursor ));

/* fcc_store.c */
krb5_error_code krb5_fcc_store PROTOTYPE((krb5_ccache id , krb5_creds *creds ));

/* fcc_skip.c */
krb5_error_code krb5_fcc_skip_principal PROTOTYPE((krb5_ccache id ));

/* fcc_sflags.c */
krb5_error_code krb5_fcc_set_flags PROTOTYPE((krb5_ccache id , krb5_flags flags ));

/* fcc_ops.c */
extern krb5_cc_ops krb5_cc_file_ops;

/* fcc_write.c */
krb5_error_code krb5_fcc_write PROTOTYPE((krb5_ccache id , krb5_pointer buf , int len ));
krb5_error_code krb5_fcc_store_principal PROTOTYPE((krb5_ccache id , krb5_principal princ ));
krb5_error_code krb5_fcc_store_keyblock PROTOTYPE((krb5_ccache id , krb5_keyblock *keyblock ));
krb5_error_code krb5_fcc_store_data PROTOTYPE((krb5_ccache id , krb5_data *data ));
krb5_error_code krb5_fcc_store_int32 PROTOTYPE((krb5_ccache id , krb5_int32 i ));
krb5_error_code krb5_fcc_store_ui_2 PROTOTYPE((krb5_ccache id , krb5_int32 i ));
krb5_error_code krb5_fcc_store_octet PROTOTYPE((krb5_ccache id , krb5_int32 i ));
krb5_error_code krb5_fcc_store_times PROTOTYPE((krb5_ccache id , krb5_ticket_times *t ));
krb5_error_code krb5_fcc_store_addrs PROTOTYPE((krb5_ccache , krb5_address ** ));
krb5_error_code krb5_fcc_store_addr PROTOTYPE((krb5_ccache , krb5_address * ));
krb5_error_code krb5_fcc_store_authdata PROTOTYPE((krb5_ccache , krb5_authdata **));
krb5_error_code krb5_fcc_store_authdatum PROTOTYPE((krb5_ccache , krb5_authdata *));
#ifdef OSF_DCE
int krb5_fcc_sizeof_cred PROTOTYPE((krb5_creds *cred));
#endif

/* fcc_errs.c */
krb5_error_code krb5_fcc_interpret PROTOTYPE((int ));

/* fcc_maybe.c */
krb5_error_code krb5_fcc_close_file PROTOTYPE((krb5_ccache));
krb5_error_code krb5_fcc_open_file PROTOTYPE((krb5_ccache, int));

#endif /* KRB5_FCC_PROTO__ */
