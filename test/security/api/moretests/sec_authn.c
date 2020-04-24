/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_authn.c,v $
 * Revision 1.1.6.1  1996/06/04  22:13:09  arvind
 * 	merge u2u changes
 * 	[1996/05/06  22:07 UTC  burati  /main/DCE_1.2/1]
 *
 * 	u2u changes (new arguments to sec_krb_get_cred())
 * 	[1996/05/06  15:21 UTC  burati  /main/mb_u2u/1]
 *
 * Revision 1.1.4.2  1996/02/17  23:25:49  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:53  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  21:50:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:02  root]
 * 
 * Revision 1.1.2.3  1994/09/23  20:36:35  desai
 * 	CR 12310 Fix. Change sec_krb_cn_build_message() invocation to match current
 * 	modifications made to this function.
 * 	[1994/09/23  16:22:27  desai]
 * 
 * Revision 1.1.2.2  1994/06/17  18:45:53  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:35:37  devsrc]
 * 
 * Revision 1.1.2.1  1994/06/02  21:14:48  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:21:45  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <stdio.h>
#include <stdarg.h>
#include <dce/sec_authn.h>

/* 
 * test some of the sec_authn code without going through RPC.
 * 
 * pass:
 * 	dg/cn
 * 	server principal name.
 * 	[server key table or server key]
 * 	key_seq (dg only)
 * 	authz_svc
 * 	authz_fmt
 * 	authn_level
 * 	
 */

static int failures = 0;

static void check_status (error_status_t st, char *message) 
{
    char buf[1024];

    if (st) 
    {
	dce_error_inq_text (st, buf, 0);
	printf("%s: %s\n", message, buf);
	failures++;
    }
}
static void usage ()
{
    fprintf(stderr, "Usage: sec_authn dg serverprinc authn_level authz_proto authz_fmt key_seq\n");
    fprintf(stderr, "or   : sec_authn cn serverprinc authn_level authz_proto authz_fmt\n");
    exit(1);
    
}

/* THIS IS AN OUTLINE */
int main(int argc, char **argv)
{

    int dg=0, cn=0;
    unsigned32 cusec;
    unsigned char *server_name;
    sec_krb_parsed_name xserv, xserv1;
    sec_krb_ccache ccache;
    unsigned char *client_name;
    
    unsigned32 authn_level, authz_proto, authz_fmt, key_seq;
    unsigned32 authn_level1, authz_proto1, key_seq1;
    unsigned32 exp, exp1;
    sec_id_pac_t client_pac;
    rpc_authz_cred_handle_t  client_creds;
    sec_krb_cred cred;
    sec_des_block challenge, ivec;
    sec_des_block ivec1;
    sec_des_key key, key1;
    error_status_t st;
    char *server_keytab = NULL;
    sec_krb_message message;
    sec_krb_timestamp cltime;
    
    if (argc < 2) usage();

    dg = !strcmp(argv[1], "dg");
    cn = !strcmp(argv[1], "cn");
    
    if (!dg && !cn) usage();
    if (dg && (argc < 6)) usage();
    if (cn && (argc < 5)) usage();

    server_name = (unsigned char *)argv[2];
    authn_level = atoi(argv[3]);
    authz_proto = atoi(argv[4]);
    authz_fmt = atoi(argv[5]);
    if (dg) key_seq = atoi(argv[5]);

    
    rpc_server_register_auth_info (server_name, rpc_c_authn_default, NULL, server_keytab, &st);
    check_status(st, "sec_krb_register_server");
    
    st = sec_krb_get_cc(NULL, &ccache);
    check_status(st, "sec_krb_get_cc");
    st = sec_krb_sec_parse_name (NULL, authn_level, server_name, &xserv);
    check_status(st, "sec_krb_parse_name");
    st = sec_krb_get_cred(ccache, xserv, authn_level, authz_proto, 0, NULL,
        &cred, &exp);
    check_status(st, "get_cred");
    if (dg)  
    {
	sec_des_generate_random_block(&challenge);
	sec_des_generate_random_block(&ivec);
	sec_des_new_random_key(&key);
	st = sec_krb_dg_build_message (ccache, cred, &challenge,
				  authn_level, authz_proto, key_seq, &key,
				  &ivec, authz_fmt, &message);
	check_status(st, "sec_krb_dg_build_message");
	st = sec_krb_dg_decode_message(&message, &challenge, authz_fmt, 
				       &client_name, &client_pac, 
				       &client_creds, &xserv1,
				       &authn_level1, &authz_proto1, 
				       &key_seq1, &key1, &ivec1, &exp1);
	check_status(st, "sec_krb_dg_decode_message");

	/* verify: output args match input args!! */
    } else if (cn) 
    {
	st = sec_krb_cn_build_message (ccache, cred, authn_level, authz_proto, authz_fmt, &key, &cltime, &cusec, &message);
	check_status(st, "sec_krb_cn_build_message");	
	st = sec_krb_cn_decode_message (&message, authz_fmt, &client_name, &client_pac, &client_creds, &xserv1, &authz_proto1, &key1, &exp1, &cltime, &cusec);
	check_status(st, "sec_krb_cn_decode_message");		
    }
    
    return failures;
}
