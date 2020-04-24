/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: sec_psm_N.c,v $
 * Revision 1.1.3.3  1996/11/14  16:14:33  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/14  14:29:37  arvind]
 *
 * Revision /main/DCE_1.2.2/13  1996/10/28  18:52 UTC  bartf
 * 	merge tp5 fix
 * 
 * Revision /main/DCE_1.2.2/bartf_pk_fix/2  1996/10/28  18:50 UTC  bartf
 * 	get cuti's changes
 * 
 * 	fix tp5() N1 domain
 * 	[1996/10/28  18:44 UTC  bartf  /main/DCE_1.2.2/bartf_pk_fix/1]
 * 	Revision /main/DCE_1.2.2/12  1996/10/24  12:31 UTC  cuti
 * 	typo
 * 
 * Revision /main/DCE_1.2.2/11  1996/10/23  18:21 UTC  cuti
 * 	Merge of cuti_pk_fix
 * 
 * 	Add OSF copyright
 * 	[1996/09/30  15:44 UTC  arvind  /main/DCE_1.2.2/10]
 * 	Revision /main/DCE_1.2.2/cuti_pk_fix/2  1996/10/22  04:00 UTC  cuti
 * 	more uninit kvno
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_fix/1  1996/10/16  19:16 UTC  cuti
 * 	kvno should initialzied to 0
 * 
 * $EndLog$
 */
/* sec_psm_N.c  Error Condition testcases
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <errno.h>
#include <macros.h>
#include <tet_api.h>
#include <ts-common.h>

#include <dce/binding.h>
#include <dce/dce_error.h>

#include <dce/rgybase.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>

#include <sec_def_flags.h>

#include <dce/sec_pk.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pvtkey.h>
#include <dce/sec_psm.h>
#include <dce/sec_psm_base.h>
#include <sec_bsafe.h>
#include "../common.h"


#undef PK_USER
#define PK_USER "test_psm_pk_user"

#define PVTKEY_FILE_PREFIX "/opt/dcelocal/var/security/pk_file/"

static void tp1(void), tp2(void), tp3(void), tp4(void), tp5(void), tp6(void);
static void pk_startup(void), pk_cleanup(void);

unsigned32 pk_size = 1024;

sec_rgy_name_t     user;
sec_passwd_str_t   passwd;
sec_passwd_str_t   new_passwd;
int                dce_status;
sec_login_handle_t ilh = NULL;
sec_rgy_handle_t   write_context;
char rm_pk_file_com[512];

unsigned char unexpected[dce_c_error_string_len];
unsigned char expected[dce_c_error_string_len];

sec_pk_data_t tobesigned_data = {
    50, "The is a test about psm and pvtkey and bsafe stuff" };

sec_pk_data_t tobeencrypted_data = {
    60, "The is a test about psm and pvtkey and bsafe stuff0123456789" };
                                  

/* List of tests to run that TET will be looking for */
struct tet_testlist tet_testlist[] = {
    {tp1, 1},  
    {tp2, 2},  
    {tp3, 3},  
    {tp4, 4},  
    {tp5, 5},  
    {tp6, 6},  
    {NULL, 0}
};

void (*tet_startup)() = pk_startup;
void (*tet_cleanup)() = pk_cleanup;


/* Initialization of data needed for each of the test cases */
static void pk_startup(void)
{
    error_status_t          st;
    char *cp;
    sec_rgy_bind_auth_info_t auth_info;
    sec_rgy_handle_t     rgy_handle;
    uuid_t               princ_id;
    unsigned_char_t      *princ_idstr;
    sec_rgy_pname_t      pvtkey_file;

    mtet_infoline("sec_psm test startup.\n");
    
    cp = getenv ("PK_SIZE");
    if (cp) {
	pk_size = atoi(cp);
	if (pk_size < 512)
	    pk_size = 512;
    }

    CLEAR_STATUS(&st);
    strcpy((char *)user, PK_USER);
    strcpy((char *)passwd, PK_PASSWD);
    strcpy((char *)new_passwd, NEW_PK_PASSWD);


    /* just in case cleanup didn't work before... */
    system("./cleanup.ksh > /dev/null 2>&1");

    /* add a generic pk_user */

    if (system("./setup.ksh > /dev/null 2>&1") != 0) {
        int test_no;
        mtet_infoline("FAILED: system(./setup.ksh)\n");
        for (test_no=0; tet_testlist[test_no++].testfunc != NULL;)
            tet_delete(test_no, "startup() has failed");
        return;
    }


    /* Need to form pvtkey file name by appending to PVTKEY_PREFIX
       the user id */

    auth_info.info_type = sec_rgy_bind_auth_none;
    auth_info.tagged_union.dce_info.authn_level= rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;
 
    sec_rgy_site_bind_query(NULL, &auth_info, &rgy_handle, &st);

    sec_rgy_pgo_name_to_id(rgy_handle, sec_rgy_domain_person, user, &princ_id, &st);

    uuid_to_string(&princ_id, &princ_idstr, &st);

    sec_rgy_site_close(rgy_handle, &st);

    strcpy(pvtkey_file, PVTKEY_FILE_PREFIX);
    strcat(pvtkey_file, princ_idstr);

    strcpy(rm_pk_file_com, "rm ");
    strcat(rm_pk_file_com, pvtkey_file);
    
    rpc_string_free(&princ_idstr, &st);

    mtet_infoline("sec_psm test startup complete.\n");
    return;

}


/* Cleanup of data needed for each of the test cases, deleting pk_user
   account  */
static void pk_cleanup(void)
{
    mtet_infoline("sec_psm test cleanup.\n");

    if (system("./cleanup.ksh") != 0) {
        mtet_infoline("FAILED: system(./cleanup.ksh)\n");
    }

    /* cleanup pk_file */
    system( rm_pk_file_com );

    return;
}



/* TP1: N1. sec_psm:  check Unsuccessful call, with bad psm handles */

static void tp1()
{

    error_status_t   st;
    sec_psm_handle_t psm_h;
    sec_psm_handle_t bad_psm_h;
    sec_pk_data_t    pvtkey;
    sec_pk_data_t    pubkey;
    sec_pk_usage_flags_t   usages = sec_pk_usage_digitalSignature;
    sec_pk_algorithm_id_t * alg_id;
    sec_pk_signed_t     signature;
    sec_pk_encrypted_t  encrypted_data;
    sec_pk_data_t       wasdecrypted_data;
    unsigned32          kvno;



    mtet_infoline("(tp1) N1. sec_psm_sign_data(): check Unsuccessful call, using bogus handle.\n" );
  
    /* start fresh  */
    system( rm_pk_file_com );

    st = sec_psm_open( user,
                       passwd,
                       & sec_pk_domain_dce_general,
                       & psm_h);
    CHECK_RESULT(true, "N. SEC_PSM: sec_psm_open", &st, error_status_ok);

    bad_psm_h = psm_h;

    st = sec_psm_close( psm_h );
    CHECK_RESULT(true, "N. SEC_PSM: sec_psm_close for bad_psm_h", &st, error_status_ok);


    mtet_infoline("(tp1) N1. sec_psm_put_pub_key(): check Unsuccessful call, using bogus handle.\n" );
     st = sec_psm_put_pub_key( bad_psm_h, 
                               passwd, 
			       sec_pk_usage_digitalSignature, 
			       &pubkey, 
		   	       &pvtkey );

    CHECK_RESULT(true, "N1. SEC_PSM: sec_psm_put_pub_key", &st, sec_psm_invalid_handle);


    mtet_infoline("(tp1) N2. sec_psm_update_pub_key(): check Unsuccessful call, using bogus handle.\n" );
    st = sec_psm_update_pub_key( bad_psm_h, 
			         passwd, 
				 NULL,
				 sec_pk_usage_digitalSignature, 
				 &pubkey,
				 &pvtkey);

    CHECK_RESULT(true, "N2. SEC_PSM: sec_psm_update_pub_key", &st, sec_psm_invalid_handle);


    mtet_infoline("(tp1) N3. sec_psm_sign_data(): check Unsuccessful call, using bogus handle.\n" );
    alg_id = MD5RSA;
    kvno = 0;
    st = sec_psm_sign_data( bad_psm_h,
			    alg_id, 
		            sec_pk_usage_digitalSignature, 
			    & tobesigned_data,
                            & kvno,
			    & signature);
                            
    CHECK_RESULT(true, "N3. SEC_PSM: sec_psm_sign_data", &st, sec_psm_invalid_handle);

    mtet_infoline("(tp1) N4. sec_psm_verify_data(): check Unsuccessful call, using bogus handle.\n" );
    st = sec_psm_verify_data( bad_psm_h,
                              &sec_pk_domain_dce_general,
                              user, 
                              kvno,
			      alg_id, 
			      sec_pk_usage_digitalSignature, 
			      &tobesigned_data,
			      &signature);
    CHECK_RESULT(true, "N4. SEC_PSM: sec_psm_verify_data", &st, sec_psm_invalid_handle);


    mtet_infoline("(tp1) N5. sec_psm_encrypt_data(): check Unsuccessful call, using bogus handle.\n" );
    alg_id = RSAENC;
    st = sec_psm_encrypt_data( bad_psm_h,
                               &sec_pk_domain_dce_general,
                               user, 
                               &kvno,
	                       alg_id, 
			       usages, 
			       & tobeencrypted_data,
			       & encrypted_data );

    CHECK_RESULT(true, "N5. SEC_PSM: sec_psm_encrypt_data", &st, sec_psm_invalid_handle);


    mtet_infoline("(tp1) N6. sec_psm_decrypt_data(): check Unsuccessful call, using bogus handle.\n" );
    /* kvno=0; */
    st = sec_psm_decrypt_data( bad_psm_h, 
                               kvno,
	                       alg_id, 
			       usages, 
			       &encrypted_data,
			       & wasdecrypted_data );

    CHECK_RESULT(true, "N6. SEC_PSM: sec_psm_decrypt_data", &st, sec_psm_invalid_handle);

    mtet_infoline("(tp1) N7. sec_psm_close(): check Unsuccessful call, using bogus handle.\n" );
    st = sec_psm_close( bad_psm_h );
    CHECK_RESULT(true, "N7. SEC_PSM: sec_psm_close", &st, sec_psm_invalid_handle);

   /* cleanup pk_file/ */
   system( rm_pk_file_com );

   tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/


/* TP2: N1. sec_psm: check Unsuccessful call, with uninitiated handle */

static void tp2()
{

    error_status_t   st;
#if 0
    sec_psm_handle_t psm_h;
#endif
    sec_pk_data_t    pvtkey;
    sec_pk_data_t    pubkey;
#if 0
    boolean32        pvtkey_open = false;
    unsigned32       kno;
    int              key_usage = 0;
#endif
    sec_pk_usage_flags_t   usages = sec_pk_usage_digitalSignature;
    sec_pk_algorithm_id_t * alg_id;
    sec_pk_signed_t     signature;
    sec_pk_encrypted_t  encrypted_data;
    sec_pk_data_t       wasdecrypted_data;
    unsigned32          kvno;

    sec_pk_data_init(&pvtkey);
    sec_pk_data_init(&pubkey);

    mtet_infoline("(tp2) N1. sec_psm_put_pub_key(): check Unsuccessful call, using uninitiated handle.\n" );
    /* start fresh  */
    system( rm_pk_file_com );

    st = sec_psm_put_pub_key( NULL, 
                              passwd, 
  		              sec_pk_usage_digitalSignature, 
			      &pubkey, 
		   	      &pvtkey );

    CHECK_RESULT(true, "N1. SEC_PSM: sec_psm_put_pub_key", &st, sec_psm_not_init);


    mtet_infoline("(tp2) N2. sec_psm_update_pub_key(): check Unsuccessful call, using uninitiated handle.\n" );
    st = sec_psm_update_pub_key( NULL, 
			         passwd, 
				 NULL,
				 sec_pk_usage_digitalSignature, 
				 &pubkey,
				 &pvtkey);

    CHECK_RESULT(true, "N2. SEC_PSM: sec_psm_update_pub_key", &st, sec_psm_not_init);


    mtet_infoline("(tp2) N3. sec_psm_sign_data(): check Unsuccessful call, using uninitiated handle.\n" );
    alg_id = MD5RSA;
    st = sec_psm_sign_data( NULL,
			    alg_id, 
		            sec_pk_usage_digitalSignature, 
			    & tobesigned_data,
                            & kvno,
			    & signature);
                            
    CHECK_RESULT(true, "N3. SEC_PSM: sec_psm_sign_data", &st, sec_psm_not_init);

    mtet_infoline("(tp2) N4. sec_psm_verify_data(): check Unsuccessful call, using uninitiated handle.\n" );
    st = sec_psm_verify_data( NULL,
                              &sec_pk_domain_dce_general,
                              user, 
                              kvno,
			      alg_id, 
			      sec_pk_usage_digitalSignature, 
			      & tobesigned_data,
			      & signature);
    CHECK_RESULT(true, "N4. SEC_PSM: sec_psm_verify_data", &st, sec_psm_not_init);


    mtet_infoline("(tp2) N5. sec_psm_encrypt_data(): check Unsuccessful call, using uninitiated handle.\n" );
    alg_id = RSAENC;
    st = sec_psm_encrypt_data( NULL,
                               &sec_pk_domain_dce_general,
                               user, 
                               &kvno,
	                       alg_id, 
			       usages, 
			       & tobeencrypted_data,
			       & encrypted_data );

    CHECK_RESULT(true, "N5. SEC_PSM: sec_psm_encrypt_data", &st, sec_psm_not_init);


    mtet_infoline("(tp2) N6. sec_psm_decrypt_data(): check Unsuccessful call, using uninitiated handle.\n" );
    /* kvno=0; */
    st = sec_psm_decrypt_data( NULL, 
                               kvno,
	                       alg_id, 
			       usages, 
			       & encrypted_data,
			       & wasdecrypted_data );

    CHECK_RESULT(true, "N6. SEC_PSM: sec_psm_decrypt_data", &st, sec_psm_not_init);

   tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/


/* TP3: N1. sec_psm:  check Unsuccessful call, with wrong key usage */

static void tp3()
{

    error_status_t   st;
    sec_psm_handle_t psm_h;
    sec_pk_data_t    pvtkey;
    sec_pk_data_t    pubkey;
    sec_pk_usage_flags_t   usages;
    sec_pk_algorithm_id_t * alg_id;
    sec_pk_signed_t     signature;
    sec_pk_encrypted_t  encrypted_data;
    sec_pk_data_t       wasdecrypted_data;
    unsigned32          kvno;


    mtet_infoline("(tp3) N1. sec_psm_put_pub_key(): check Unsuccessful call, using (wrong) sec_pk_usage_nonRepudiation.\n" );

    /* start fresh  */
    system( rm_pk_file_com );

    mtet_infoline("(tp3) N. sec_psm_open setup for wrong key usage.\n");
    st = sec_psm_open( user,
                       passwd,
                       & sec_pk_domain_dce_general,
                       & psm_h);
    CHECK_RESULT(true, "N. SEC_PSM: sec_psm_open setup for wrong key usage", &st, error_status_ok);

    mtet_infoline("(tp3) N. sec_psm_gen_pub_key setup for wrong key usage.\n");
    mtet_infoline("          (generating public key; be patient...)\n");
	
    alg_id = RSA_PKCS;
    /* generate keys */
    st = sec_psm_gen_pub_key( alg_id,
                              pk_size, /* unsigned32  key_length */
                              NULL,  /* *seed sec_pk_gen_data_t */
                              &pubkey,
                              &pvtkey );

    CHECK_RESULT(true, "N. SEC_PSM: sec_psm_gen_pub_key setup for wrong key usage ", &st, error_status_ok);
    usages = sec_pk_usage_nonRepudiation;
    st = sec_psm_put_pub_key( psm_h, 
                              passwd, 
	 	              usages, 
			      &pubkey, 
		   	      &pvtkey );

    CHECK_RESULT_DONT_RETURN(true, "N1. SEC_PSM: sec_psm_put_pub_key", &st, sec_pk_e_usage_unsupported);


    mtet_infoline("(tp3) N2. sec_psm_update_pub_key(): check Unsuccessful call, using (wrong) sec_pk_usage_nonRepudiation.\n" );
    st = sec_psm_update_pub_key( psm_h, 
			         passwd, 
				 NULL,
				 usages, 
				 &pubkey,
				 &pvtkey);

    CHECK_RESULT_DONT_RETURN(true, "N2. SEC_PSM: sec_psm_update_pub_key", &st, sec_pk_e_usage_unsupported);


    mtet_infoline("(tp3) N3. sec_psm_sign_data(): check Unsuccessful call, using (wrong) sec_pk_usage_nonRepudiation.\n" );

    /* first do _put_pub_key and _sign_data for valid key */
     st = sec_psm_put_pub_key( psm_h, 
                               passwd, 
			       sec_pk_usage_digitalSignature, 
			       &pubkey, 
		   	       &pvtkey );

    CHECK_RESULT(true, "N3. SEC_PSM: sec_psm_put_pub_key", &st, error_status_ok);

    alg_id = MD5RSA;
    kvno = 0;
    st = sec_psm_sign_data( psm_h,
			    alg_id, 
		            sec_pk_usage_digitalSignature, 
			    & tobesigned_data,
                            & kvno,
			    & signature);
                            
    CHECK_RESULT(true, "N3. SEC_PSM: sec_psm_sign_data", &st, error_status_ok);


    mtet_infoline("(tp3) N4. sec_psm_verify_data(): check Unsuccessful call, using (wrong) sec_pk_usage_nonRepudiation.\n" );
    usages = sec_pk_usage_nonRepudiation;
    st = sec_psm_verify_data( psm_h,
                              &sec_pk_domain_dce_general,
                              user, 
                              kvno,
			      alg_id, 
			      usages, 
			      & tobesigned_data,
			      & signature);
    CHECK_RESULT_DONT_RETURN(true, "N4. SEC_PSM: sec_psm_verify_data", &st, sec_pk_e_usage_unsupported);


    mtet_infoline("(tp3) N5. sec_psm_encrypt_data(): check Unsuccessful call, using unsupported key type.\n" );
    alg_id = RSAENC;
    usages = sec_pk_usage_nonRepudiation;
    st = sec_psm_encrypt_data( psm_h,
                               &sec_pk_domain_dce_general,
                               user, 
                               &kvno,
	                       alg_id, 
			       usages, 
			       & tobeencrypted_data,
			       & encrypted_data );

    CHECK_RESULT_DONT_RETURN(true, "(tp3) N5. SEC_PSM: sec_psm_encrypt_data", &st, sec_pk_e_usage_unsupported );


    st = sec_psm_close( psm_h );
    CHECK_RESULT(true, "N7. SEC_PSM: sec_psm_close", &st, error_status_ok);

    sec_pk_data_free(& pubkey);
    sec_pk_data_zero_and_free(& pvtkey);
    sec_pk_data_free(& signature);
    sec_pk_data_free(& encrypted_data);

    tet_result(TET_PASS);
}
/*-----------------------------------------------------------------------*/


/* TP4: N1. sec_psm:  check Unsuccessful call, with illegal sign id */

static void tp4()
{

    error_status_t   st;
    sec_psm_handle_t psm_h;
    sec_pk_data_t    pvtkey;
    sec_pk_data_t    pubkey;
    sec_pk_usage_flags_t   usages = sec_pk_usage_digitalSignature;
    sec_pk_algorithm_id_t * alg_id;
    sec_pk_signed_t     signature;
    sec_pk_encrypted_t  encrypted_data;
    sec_pk_data_t       wasdecrypted_data;
    unsigned32          kvno;


    mtet_infoline("(tp4) N1. sec_psm_gen_pub_key(): check Unsuccessful call, using illegal sign id.\n" );
    mtet_infoline("          (generating public key; be patient...)\n");

    /* start fresh  */
    system( rm_pk_file_com );

    alg_id = MD5RSA;  /* invalid choice for this op */
    st = sec_psm_gen_pub_key( alg_id,
                              pk_size, /* unsigned32  key_length */
                              NULL,  /* *seed sec_pk_gen_data_t */
                              &pubkey,
                              &pvtkey );

    CHECK_RESULT(true, "N1. SEC_PSM: sec_psm_gen_pub_key", &st, sec_psm_wrong_pub_key_type);


    st = sec_psm_open( user,
                       passwd,
                       & sec_pk_domain_dce_general,
                       & psm_h);
    CHECK_RESULT(true, "N. SEC_PSM: sec_psm_open for illegal sign id", &st, error_status_ok);


    mtet_infoline("(tp4) N. sec_psm_gen_pub_key setup for illegal sign id.\n");
    mtet_infoline("          (generating public key; be patient...)\n");
	
    alg_id = RSA_PKCS;
    /* generate keys */
    st = sec_psm_gen_pub_key( alg_id,
                              pk_size, /* unsigned32  key_length */
                              NULL,  /* *seed sec_pk_gen_data_t */
                              &pubkey,
                              &pvtkey );

    CHECK_RESULT(true, "N. SEC_PSM: sec_psm_gen_pub_key setup for illegal sign id.", &st, error_status_ok);

    /* first do _put_  for valid key */
    mtet_infoline("(tp4) N. sec_psm_put_pub_key setup for illegal sign id.\n");
    st = sec_psm_put_pub_key( psm_h, 
                              passwd, 
  		              usages, 
			      &pubkey, 
		   	      &pvtkey );

    CHECK_RESULT(true, "N. SEC_PSM: sec_psm_put_pub_key", &st, error_status_ok);

    mtet_infoline("(tp4) N. sec_psm_sign_data(): check Unsuccessful call, using illegal sign id.\n");
    alg_id = RSA_PKCS;  /* invalid choice for this op */
    kvno = 0;
    st = sec_psm_sign_data( psm_h,
			    alg_id, 
		            usages, 
			    & tobesigned_data,
                            & kvno,
			    & signature);
    mtet_infoline("Current pvtkey kvno = %d\n", kvno);
                            
    CHECK_RESULT_DONT_RETURN(true, "(tp4) N2. SEC_PSM: sec_psm_sign_data", &st, sec_psm_unsupported_algorithm_id );


    mtet_infoline("(tp4) N3. sec_psm_verify_data(): check Unsuccessful call, using illegal sign id.\n" );
    st = sec_psm_verify_data( psm_h,
                              &sec_pk_domain_dce_general,
                              user, 
                              kvno,
			      alg_id, 
			      usages, 
			      & tobesigned_data,
			      & signature);
    CHECK_RESULT_DONT_RETURN(true, "N3. SEC_PSM: sec_psm_verify_data", &st, sec_psm_unsupported_algorithm_id);


    mtet_infoline("(tp4) N4. sec_psm_encrypt_data(): check Unsuccessful call, using illegal sign id.\n" );
    alg_id = RSA_PKCS;   /* invalid choice for this op */
    usages = sec_pk_usage_digitalSignature;
    kvno = 0;
    st = sec_psm_encrypt_data( psm_h,
                               &sec_pk_domain_dce_general,
                               user, 
                               &kvno,
	                       alg_id, 
			       usages, 
			       & tobeencrypted_data,
			       & encrypted_data );

    CHECK_RESULT_DONT_RETURN(true, "N4. SEC_PSM: sec_psm_encrypt_data", &st, sec_psm_unsupported_algorithm_id );


    mtet_infoline("(tp4) N5. sec_psm_decrypt_data(): check Unsuccessful call, using illegal sign id.\n" );
    st = sec_psm_decrypt_data( psm_h, 
                               kvno,
	                       alg_id, 
			       usages, 
			       & encrypted_data,
			       & wasdecrypted_data );

    CHECK_RESULT_DONT_RETURN(true, "N5. SEC_PSM: sec_psm_decrypt_data", &st, sec_psm_unsupported_algorithm_id );

    st = sec_psm_close( psm_h );
    CHECK_RESULT(true, "N. SEC_PSM: sec_psm_close for illegal sign id", &st, error_status_ok);

    sec_pk_data_free(& pubkey);
    sec_pk_data_zero_and_free(& pvtkey);

    tet_result(TET_PASS);
}
/*-----------------------------------------------------------------------*/


/* TP5: N1. sec_psm:  check Unsuccessful call, with unsupported domain */

static void tp5()
{

    error_status_t   st;
    sec_psm_handle_t psm_h;
    sec_pk_data_t    pvtkey;
    sec_pk_data_t    pubkey;
    sec_pk_usage_flags_t   usages = sec_pk_usage_digitalSignature;
    uuid_t sec_pk_domain_is_unsupported = {
      /* 8357bc38-30ce-11d0-92e5-080009c87598 */
      0x8357bc38,
      0x30ce,
      0x11d0,
      0x92,
      0xe5,
      {0x08, 0x00, 0x09, 0xc8, 0x75, 0x98}
    };

    /* start fresh  */
    system( rm_pk_file_com );


    mtet_infoline("(tp5) N1. sec_psm_open(): check successful call, using bogus domain.\n" );

    /* because of design change, this call now gets the
       sec_pk_e_domain_unsupported status. 
    */
    st = sec_psm_open( user,
                       passwd,
                       & sec_pk_domain_is_unsupported,
                       & psm_h);
    CHECK_RESULT(true, "N. SEC_PSM: sec_psm_open", &st, sec_pk_e_domain_unsupported);

    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/


/* TP6: N1. sec_psm:  check Unsuccessful call, with unknown data */

static void tp6()
{

    error_status_t   st;
    sec_psm_handle_t psm_h;
    sec_pk_usage_flags_t   usages = sec_pk_usage_digitalSignature;
    sec_pk_algorithm_id_t * alg_id;
    sec_pk_signed_t     bad_signature;
    sec_pk_signed_t     signature;
    sec_pk_data_t bad_signed_data = { 1, " " };
    sec_pk_signed_t     signature1, signature2;
    sec_pk_data_t tobesigned_data1 = { 25, "The is tobesigned_data  1" };
    sec_pk_data_t tobesigned_data2 = { 25, "The is tobesigned_data  2" };
    sec_pk_data_t    pvtkey;
    sec_pk_data_t    pubkey;
    unsigned32       kvno;


    mtet_infoline("(tp6) N1. sec_psm_verify_data(): check Unsuccessful call, using unknown signed data.\n" );

    /* start fresh  */
    system( rm_pk_file_com );


    st = sec_psm_open( user,
                       passwd,
                       & sec_pk_domain_dce_general,
                       & psm_h);
    CHECK_RESULT(true, "N. SEC_PSM: sec_psm_open for unknown signed data", &st, error_status_ok);

    /* generate keys, store the keys */
    alg_id = RSA_PKCS;
    st = sec_psm_gen_pub_key( alg_id,
                              pk_size, /* unsigned32  key_length */
                              NULL,  /* *seed sec_pk_gen_data_t */
                              &pubkey,
                              &pvtkey );

    CHECK_RESULT(true, "N1.1. SEC_PSM: sec_psm_gen_pub_key", &st, error_status_ok);

    /* then store keys */
    st = sec_psm_put_pub_key( psm_h, 
			          passwd, 
				  usages, 
			          &pubkey, 
		   	          &pvtkey );

    CHECK_RESULT(true, "N1.2. SEC_PSM: sec_psm_put_pub_key", &st, error_status_ok);


    if (pk_size < 1024) {
	    tobesigned_data1.len = (pk_size/8)-1;
	    tobesigned_data2.len = (pk_size/8)-1;
    }

    alg_id =  MD5RSA;
    kvno = 0;
    /* sign 2 different datums, then verify using the wrong one */	
    st = sec_psm_sign_data( psm_h, 
			    alg_id, 
			    usages, 
			    & tobesigned_data1,
                            & kvno,
			    & signature1);

    mtet_infoline("current pvtkey kvno = %d\n", kvno);

     CHECK_RESULT_DONT_RETURN(true, "N1.3. SEC_PSM: sec_psm_sign_data", &st, error_status_ok);
     st = sec_psm_sign_data( psm_h, 
                             alg_id, 
			     usages,
			     & tobesigned_data2,
                             & kvno,
			     & signature2);

    CHECK_RESULT_DONT_RETURN(true, "N1.4. SEC_PSM: sec_psm_sign_data", &st, error_status_ok);
    
    st = sec_psm_verify_data( psm_h,
                              &sec_pk_domain_dce_general,
                              user, 
                              kvno,
			      alg_id, 
			      usages,
			      & tobesigned_data1,
			      & signature2);

    CHECK_RESULT_DONT_RETURN(true, "N1.5 SEC_PSM: sec_psm_verify_data", &st, sec_s_bsafe_signature);


    mtet_infoline("(tp6) N2. sec_psm_verify_data(): check Unsuccessful call, using bad signature.\n" );

    bad_signature.len = 0;
    bad_signature.data = NULL;

    st = sec_psm_verify_data( psm_h,
                              &sec_pk_domain_dce_general,
                              user, 
                              kvno,
			      alg_id, 
			      usages, 
			      & tobesigned_data1,
			      & bad_signature);   /* bad  */
    CHECK_RESULT_DONT_RETURN(true, "N2. SEC_PSM: sec_psm_verify_data", &st, sec_s_bsafe_decryption_failure);


    st = sec_psm_close( psm_h );
    CHECK_RESULT(true, "N. SEC_PSM: sec_psm_close for unknown", &st, error_status_ok);

    sec_pk_data_free(& pubkey);
    sec_pk_data_zero_and_free(& pvtkey);
    sec_pk_data_free(& signature1);
    sec_pk_data_free(& signature2);

    tet_result(TET_PASS);

}
/*-----------------------------------------------------------------------*/


