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
 * $Log: rgy_pk_N.c,v $
 * Revision 1.1.2.2  1996/10/03  18:47:43  arvind
 * 	Add OSF copyright
 * 	[1996/09/30  15:45 UTC  arvind  /main/DCE_1.2.2/4]
 *
 * Revision 1.1.2.1  1996/08/09  20:16:47  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  20:03:07  arvind]
 * 
 * $EndLog$
 */
/* rgy_pk_N.c
 *
 * Test fail conditions for sec_passwd_pubkey and sec_passwd_genprivkey
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <u_str.h>
#include <dce/dce.h>
#include <dce/dce_error.h>
#include <dce/passwd.h>
#include <dce/binding.h>
#include <dce/rgybase.h>
#include <dce/pkl.h>
#include <dce/sec_pk.h>
#include <dce/sec_pubkey.h>
#include <dce/dce_msg.h>
#include <dce/dcesadmsg.h>
#include <dce/dcepsmmsg.h>
#include <dce/sec_pk_base.h>
#include <sec_encode.h>
#include <macros.h>
#include <tet_api.h>
#include <ts-common.h>
#include "../common.h"

#undef PK_USER
#define PK_USER "test_sec_rgy_pk_user"

char * localcellname;
char krbtgt_princ[256];


static void tp1(void), tp2(void), tp3(void);
static void pk_startup(void), pk_cleanup(void);


unsigned char unexpected[dce_c_error_string_len];
unsigned char expected[dce_c_error_string_len];
sec_login_handle_t login_h;
sec_rgy_name_t     user;
sec_passwd_str_t   passwd;
int                dce_status;
sec_login_handle_t ilh = NULL;
rpc_binding_handle_t  dced_h;
char *     cellname;
sec_rgy_handle_t     context;         /* registry context handle */
unsigned32 pk_size = 1024;


/* List of tests to run that TET will be looking for */
struct tet_testlist tet_testlist[] = {
    {tp1, 1},
    {tp2, 2},
    {tp3, 3},
    {NULL, 0}
};

void (*tet_startup)() = pk_startup;
void (*tet_cleanup)() = pk_cleanup;


/* Initialization of data needed for each of the test cases */
static void pk_startup(void)
{
    error_status_t     st;
    char              *cp;
     
    mtet_infoline("rgy_pk test startup.\n");
    CLEAR_STATUS(&st);
    strcpy((char *)user, PK_USER);
    strcpy((char *)passwd, PK_PASSWD);

    dce_cf_get_cell_name( & cellname, & st );

    localcellname = cellname + 4;
    sprintf(krbtgt_princ, "krbtgt%s", localcellname );

    cp = getenv ("PK_SIZE");
    if (cp) {
	pk_size = atoi(cp);
	if (pk_size < 256)
	    pk_size = 256;
    }


    /* add  pk_user */

    if (system("./setup.ksh") != 0) {
        int test_no;
        mtet_infoline("FAILED: system(./setup.ksh)\n" );
        for (test_no=0; tet_testlist[test_no++].testfunc != NULL;)
            tet_delete(test_no, "startup() has failed");
        return;
    }

    /* Bind to registry */
    sec_rgy_site_open_update(NULL, &context, &st);
    CHECK_STARTUP_RESULT(true, "N. RGY_PK: sec_rgy_site_open_update startup", &st, error_status_ok);

    return;
}


/* Cleanup of data needed for each of the test cases */
static void pk_cleanup(void)
{
    error_status_t          st;

    system("./cleanup.ksh");

    sec_rgy_site_close(context, &st);
    CHECK_RESULT(true, "N. RGY_PK: sec_rgy_site_close cleanup", &st, error_status_ok);

    return;
}

/*       TEST_NAME   PRINCIPAL   PASSWD_TYPE            PASSWD_VERSION
"test1_N",   cell_admin, sec_passwd_pubkey,    1);
exp: key type not supported
*/

static void tp1()
{
    boolean32            set_password=TRUE;
    sec_passwd_rec_t     caller_key;      /* cell_admin key */
    sec_passwd_rec_t     new_key;         /* new key */
    sec_passwd_type_t    new_keytype;     /* new keytype */
    sec_passwd_version_t new_key_version; /* new key version */
    char                 *mypwd="-dce-";
    unsigned32           pvtkey_len=6;
    char                 *pvtkey="pvtkey";
    char                 *plainkey=(char *)passwd;
    sec_rgy_cursor_t     cursor;          /* registry cursor */
    sec_rgy_login_name_t login_name;      /* login name of principal being modified */
    sec_rgy_login_name_t name_result;     /* returned full login name */
    sec_rgy_acct_user_t  user_part;
    error_status_t       st,lst;          /* global and local status */
    sec_passwd_type_t    pwdtype;
    sec_passwd_version_t version;

    mtet_infoline("(tp1) N1. sec_rgy_acct_user_replace(): check Unsuccessful call, with user, sec_passwd_pubkey, version=1.\n" );

    /* Set up new key */
    pwdtype=sec_passwd_pubkey;
    version=1;

    new_keytype=pwdtype;
    new_key.key.key_type=pwdtype;
    new_key.version_number=version;
    new_key.pepper=NULL;

    new_key.key.tagged_union.pub_key.len=pvtkey_len;
    new_key.key.tagged_union.pub_key.data=(byte *)pvtkey;

    /* Initialize principal name */
    login_name.pname[0]='\0';
    login_name.gname[0]='\0';
    login_name.oname[0]='\0';
    u_strncpy(login_name.pname, user, sec_rgy_name_max_len);

    /* Initialize rgy cursor */
    sec_rgy_cursor_reset(&cursor);

    /* Lookup account */
    sec_rgy_acct_lookup(context,
                        &login_name,
                        &cursor,
                        &name_result,
                        NULL,
                        NULL,
                        NULL,
                        &user_part,
                        NULL,
                        &st);

    CHECK_RESULT(true, "(tp1) N1. RGY_PK: sec_rgy_acct_lookup", &st, error_status_ok);

    /* Initialize caller_key */
    caller_key.key.key_type=sec_passwd_plain;
    caller_key.version_number=sec_passwd_c_version_none;
    caller_key.pepper=NULL;
    caller_key.key.tagged_union.plain=(byte *)mypwd;

    /* Update account */
    sec_rgy_acct_user_replace(context,
                              &login_name,
                              &user_part,
                              set_password,
                              &caller_key,
                              &new_key,
                              new_keytype,
                              &new_key_version,
                              &st);

    CHECK_RESULT_DONT_RETURN(true, "(tp1) N1. RGY_PK: sec_rgy_acct_user_replace", &st, sec_rgy_key_bad_type );

    /*
    "test2_N",   user, sec_passwd_genprivkey, 1
    exp: Only DES keys allowed for this account
    */

    mtet_infoline("(tp1) N2. sec_rgy_acct_user_replace(): check Unsuccessful call, with user, sec_passwd_genprivkey, version=1.\n" );

    /* Set up new key */
    pwdtype=sec_passwd_genprivkey;
    version=1;

    new_keytype=pwdtype;
    new_key.key.key_type=pwdtype;
    new_key.version_number=version;
    new_key.pepper=NULL;

    new_key.key.tagged_union.modulus_size=pk_size;

    /* Update account */
    sec_rgy_acct_user_replace(context,
                              &login_name,
                              &user_part,
                              set_password,
                              &caller_key,
                              &new_key,
                              new_keytype,
                              &new_key_version,
                              &st);

    CHECK_RESULT_DONT_RETURN(true, "(tp1) N2. RGY_PK: sec_rgy_acct_user_replace", &st, sec_rsdb_acct_bad_keytype );

    tet_result(TET_PASS);

}


/*
"test3_N",   krbtgt,     sec_passwd_pubkey,    1
"test7_N",   krbtgt,     sec_passwd_genprivkey, 2
*/
static void tp2()
{
    boolean32            set_password=TRUE;
    sec_passwd_rec_t     caller_key;      /* cell_admin key */
    sec_passwd_rec_t     new_key;         /* new key */
    sec_passwd_type_t    new_keytype;     /* new keytype */
    sec_passwd_version_t new_key_version; /* new key version */
    char                 *mypwd="-dce-";
    unsigned32           pvtkey_len=6;
    char                 *pvtkey="pvtkey";
    char                 *plainkey=(char *)passwd;
    sec_rgy_cursor_t     cursor;          /* registry cursor */
    sec_rgy_login_name_t login_name;      /* login name of principal being modified */
    sec_rgy_login_name_t name_result;     /* returned full login name */
    sec_rgy_acct_user_t  user_part;
    error_status_t       st,lst;          /* global and local status */
    sec_passwd_type_t    pwdtype;
    sec_passwd_version_t version;

    mtet_infoline("(tp2) N1. sec_rgy_acct_user_replace(): check Unsuccessful call, with krbtgt_princ, sec_passwd_pubkey, version=1.\n" );

    /* Initialize caller_key for cell_admin */
    caller_key.key.key_type=sec_passwd_plain;
    caller_key.version_number=sec_passwd_c_version_none;
    caller_key.pepper=NULL;
    caller_key.key.tagged_union.plain=(byte *)mypwd;

    /* Set up new key */
    pwdtype=sec_passwd_pubkey;
    version=1;

    new_keytype=pwdtype;
    new_key.key.key_type=pwdtype;
    new_key.version_number=version;
    new_key.pepper=NULL;

    new_key.key.tagged_union.pub_key.len=pvtkey_len;
    new_key.key.tagged_union.pub_key.data=(byte *)pvtkey;


    /* Initialize principal name */
    login_name.pname[0]='\0';
    login_name.gname[0]='\0';
    login_name.oname[0]='\0';
    u_strncpy(login_name.pname, krbtgt_princ, sec_rgy_name_max_len);

    /* Initialize rgy cursor */
    sec_rgy_cursor_reset(&cursor);

    /* Lookup account */
    sec_rgy_acct_lookup(context,
                        &login_name,
                        &cursor,
                        &name_result,
                        NULL,
                        NULL,
                        NULL,
                        &user_part,
                        NULL,
                        &st);

    CHECK_RESULT(true, "(tp2) N1. RGY_PK: sec_rgy_acct_lookup", &st, error_status_ok);


    /* Update account */
    sec_rgy_acct_user_replace(context,
                              &login_name,
                              &user_part,
                              set_password,
                              &caller_key,
                              &new_key,
                              new_keytype,
                              &new_key_version,
                              &st);

    CHECK_RESULT_DONT_RETURN(true, "(tp2) N1. RGY_PK: sec_rgy_acct_user_replace", &st, sec_rgy_key_bad_type );


    mtet_infoline("(tp2) N2. sec_rgy_acct_user_replace(): check Unsuccessful call, with krbtgt, sec_passwd_genprivkey, version=256.\n" );

    /* Set up new key */
    pwdtype=sec_passwd_genprivkey;
    version=256;

    new_keytype=pwdtype;
    new_key.key.key_type=pwdtype;
    new_key.version_number=version;
    new_key.pepper=NULL;

    new_key.key.tagged_union.modulus_size=pk_size;

    /* Update account */
    sec_rgy_acct_user_replace(context,
                              &login_name,
                              &user_part,
                              set_password,
                              &caller_key,
                              &new_key,
                              new_keytype,
                              &new_key_version,
                              &st);

    CHECK_RESULT_DONT_RETURN(true, "(tp2) N2. RGY_PK: sec_rgy_acct_user_replace", &st, sec_pk_e_key_vers_unsupported );


    mtet_infoline("(tp2) N3. sec_rgy_acct_user_replace(): check Unsuccessful call, with bogus princ, sec_passwd_pubkey, version=0.\n" );
    /* Initialize principal name */
    login_name.pname[0]='\0';
    login_name.gname[0]='\0';
    login_name.oname[0]='\0';
    u_strncpy(login_name.pname,"pri_that_doesnt_exist", sec_rgy_name_max_len);

    /* Set up new key, good values */
    pwdtype=sec_passwd_genprivkey;
    version=0;

    new_keytype=pwdtype;
    new_key.key.key_type=pwdtype;
    new_key.version_number=version;
    new_key.pepper=NULL;

    sec_rgy_acct_user_replace(context,
                              &login_name,
                              &user_part,
                              set_password,
                              &caller_key,
                              &new_key,
                              new_keytype,
                              &new_key_version,
                              &st);

    CHECK_RESULT_DONT_RETURN(true, "(tp2) N3. RGY_PK: sec_rgy_acct_user_replace", &st, sec_rgy_object_not_found);

    tet_result(TET_PASS);

}


static void tp3()
{
    boolean32            set_password=TRUE;
    sec_passwd_rec_t     caller_key;      /* cell_admin key */
    sec_passwd_rec_t     new_key;         /* new key */
    sec_passwd_type_t    new_keytype;     /* new keytype */
    sec_passwd_version_t new_key_version; /* new key version */
    char                 *mypwd="-dce-";
    unsigned32           pvtkey_len=6;
    char                 *pvtkey="pvtkey";
    char                 *plainkey=(char *)passwd;
    sec_rgy_cursor_t     cursor;          /* registry cursor */
    sec_rgy_login_name_t login_name;      /* login name of principal being modified */
    sec_rgy_login_name_t name_result;     /* returned full login name */
    sec_rgy_acct_user_t  user_part;
    error_status_t       st;          /* global and local status */
    sec_passwd_type_t    pwdtype;
    sec_passwd_version_t version;


    mtet_infoline("(tp3) N1. sec_rgy_acct_replace(): check Unsuccessful call, with krbtgt, modulus size 1, too small.\n" );

    /* Initialize principal name */
    login_name.pname[0]='\0';
    login_name.gname[0]='\0';
    login_name.oname[0]='\0';
    u_strncpy(login_name.pname, krbtgt_princ, sec_rgy_name_max_len);

    /* Initialize rgy cursor */
    sec_rgy_cursor_reset(&cursor);

    /* Lookup account */
    sec_rgy_acct_lookup(context,
                        &login_name,
                        &cursor,
                        &name_result,
                        NULL,
                        NULL,
                        NULL,
                        &user_part,
                        NULL,
                        &st);

    CHECK_RESULT(true, "(tp3) N. RGY_PK: sec_rgy_acct_lookup", &st, error_status_ok);

    /* Initialize caller_key */
    caller_key.key.key_type=sec_passwd_plain;
    caller_key.version_number=sec_passwd_c_version_none;
    caller_key.pepper=NULL;
    caller_key.key.tagged_union.plain=(byte *)mypwd;

    /* Set up new key */
    pwdtype=sec_passwd_genprivkey;
    version=1;

    new_keytype=pwdtype;
    new_key.key.key_type=pwdtype;
    new_key.version_number=version;
    new_key.pepper=NULL;

    new_key.key.tagged_union.modulus_size = 1;

    /* Update account */
    sec_rgy_acct_user_replace(context,
                              &login_name,
                              &user_part,
                              set_password,
                              &caller_key,
                              &new_key,
                              new_keytype,
                              &new_key_version,
                              &st);

    CHECK_RESULT(true, "(tp3) N1. RGY_PK: sec_rgy_acct_user_replace", &st, sec_s_bsafe_mod_size_out_of_range);

    tet_result(TET_PASS);
}



