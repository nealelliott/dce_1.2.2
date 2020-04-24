/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: grp_override.c,v $
 * Revision 1.1.4.1  1996/06/04  22:09:27  arvind
 * 	New fix.
 * 	[1995/02/03  18:14 UTC  mdf  /main/HPDCE02/4]
 *
 * 	Something, wasn't quite right, this test failed every other time.
 * 	[1995/02/03  18:11 UTC  mdf  /main/HPDCE02/mdf_MOTHRA_5/1]
 *
 * 	Fix for defect.
 * 	[1995/01/30  20:14 UTC  mdf  /main/HPDCE02/3]
 *
 * 	Fix for CHFts14193
 * 	[1995/01/30  15:34 UTC  mdf  /main/HPDCE02/mdf_MOTHRA_3/1]
 *
 * 	Functional test changes
 * 	[1995/01/27  14:58 UTC  mdf  /main/HPDCE02/2]
 *
 * 	NEED TO FIX SEQ 23
 *
 * 	Its expecting an error code, but is getting back a good status.
 * 	I removed the bad status, so that the tests would work for
 * 	Mary, but I don't belive this is a good fix
 * 	[1995/01/23  16:15 UTC  mdf  /main/HPDCE02/mdf_MOTHRA_2/1]
 *
 * 	Correct the location of GSSAPI functional tests
 * 	[1995/01/20  13:42 UTC  mdf  /main/HPDCE02/1]
 *
 * 	New location
 * 	[1995/01/19  18:39 UTC  mdf  /main/mdf_MOTHRA_1/1]
 *
 * 	fix pw_name cpy in tests 8, 9
 * 	[1996/02/13  16:13 UTC  bartf  /main/HPDCE02/bartf_css/1]
 *
 * Revision 1.1.2.3  1996/02/17  23:11:54  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:25  marty]
 * 
 * Revision 1.1.2.2  1996/02/07  17:48:22  parul
 * 	DCE 1.2.1 final drop from HP
 * 	[1996/02/07  16:14:07  parul]
 * 
 * 	DCE 1.2.1 final drop from HP
 * 
 * Revision 1.1.2.2  1994/07/06  18:14:52  hasbrouc
 * 	"TETize" and get better code coverage
 * 	[1994/07/06  18:12:56  hasbrouc]
 * 
 * Revision 1.1.2.1  1994/05/06  13:50:49  hasbrouc
 * 	not fully TET-ized; cleanup later
 * 	[1994/05/06  13:48:11  hasbrouc]
 * 
 * $EndLog$
 */

/*
* functional test for group_override API
*/
#include <stdio.h>
#include <sys/file.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>
#include <locale.h>
#include <tet_api.h>

#include <dce/binding.h>
#include <dce/acct.h>
#include <dce/override.h>
#include <dce/misc.h>
#include <dce/pgo.h>
#include <dce/rpcbase.h>
#include <dce/dce_error.h>

#define STATUS_OK(stp)            ((stp)==NULL || (*stp) == error_status_ok)
#define GOOD_STATUS(stp)          ((stp)==NULL || (*stp) == error_status_ok)
#define BAD_STATUS(stp)           ((*stp) != error_status_ok)
#define SET_STATUS(stp, val)      (*stp) = val
#define CLEAR_STATUS(stp)         (*stp) = error_status_ok
#define STATUS_EQUAL(stp, st_val) ((*stp) == (st_val))

#define MAX_MEMBERS 50
#define MAX_PWD_BUF 1024
#define MAX_ENTRIES 1000000

/* TET stuff */

void (*tet_startup)() = NULL, (*tet_cleanup)() = NULL;
void tp1 ();
struct tet_testlist tet_testlist[] = { {tp1,1}, {NULL,0}};

typedef char pwd_buf_t[MAX_PWD_BUF];

/**** Global state ****/
sec_rgy_handle_t    context;
static boolean32    verbose = false;

/**** Procedures  *****/

void print_err
#ifndef __STDC__
    (string, status)
    char            *string;
    error_status_t  status;
#else
  (
    char            *string,
    error_status_t  status
  )
#endif
{
    dce_error_string_t  error_text;
    int                 lst;

    dce_error_inq_text(status, error_text, &lst);

    fprintf(stderr, "%s - ", string);
    if (GOOD_STATUS(&lst)) {
        fprintf(stderr, "%s\n", error_text);
    } else {
        fprintf(stderr, "%x\n", status);
    }
}

/*
 * Bind to any query site
 */
error_status_t open_site(
#ifdef __STDC__
void
#endif
)
{
    error_status_t              st;
    unsigned_char_p_t           site_name;
    sec_rgy_bind_auth_info_t    auth_info;

    auth_info.info_type = sec_rgy_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_level = rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;
    CLEAR_STATUS(&st);

    sec_rgy_site_bind_query((unsigned_char_p_t)"/.:", &auth_info,
                            &context, &st);
    if (STATUS_EQUAL(&st, sec_rgy_cant_authenticate)) {
        auth_info.info_type = sec_rgy_bind_auth_none;
        sec_rgy_site_bind((unsigned_char_p_t)"/.:", &auth_info, &context, &st);
        if (GOOD_STATUS(&st)) {
            if (verbose)
                printf("INFO: Binding to registry is unauthenticated\n");
        }
    }
    if (GOOD_STATUS(&st) && verbose) {
        sec_rgy_site_get(context, &site_name, &st);
        if (GOOD_STATUS(&st)) {
            printf("Bound to registry at site: %s\n", site_name);
            free((char *)site_name);
        } else {
            printf("Unable to obtain registry site name\n");
        }
    }
    return st;
}

/*
* check results of function call
*/
void check_result
#ifndef __STDC__
    (feature, seq, result, expect, stat_res, stat_exp)
    char            *feature;
    int             seq;
    char            *result;
    char            *expect;
    int             stat_res;
    int             stat_exp;
#else
  (
    char            *feature,
    int             seq,
    char            *result,
    char            *expect,
    int             stat_res,
    int             stat_exp
  )
#endif
{
 if ((strcmp(result,expect)) || !(stat_exp == stat_res)) {
     printf("FAILED feature %s seq %d result:%s %d   expected:%s %d\n", feature, seq, result, stat_res, expect, stat_exp);
     tet_result(TET_FAIL);
 } else
     {
	 printf("PASSED feature %s seq %d\n", feature, seq);
	 tet_result(TET_PASS);
     }
}

/*
 * Main routine for group_override_tst
 */
void tp1 ()
{
    long        i;
    char        *result = NULL;
    char        *expected = NULL;
    boolean32   overriden;
    error_status_t  st,status;
    sec_rgy_name_t  name, gr_salt;
    sec_rgy_unix_passwd_buf_t pw_name;
    signed32    gr_uid, unix_id;
    sec_rgy_cursor_t    cursor, member_cursor;
    sec_rgy_pgo_item_t      pgo_item;
    signed32    mbrs_supplied, num_members;
    sec_rgy_member_t        member_list[MAX_MEMBERS];
    sec_rgy_unix_passwd_buf_t   gr_passwd;
    sec_override_fields_t       overridemsk;
    char         *grpfnm = "/opt/dcelocal/etc/group_override";
    char         *pwfnm = "/opt/dcelocal/etc/passwd_override";
    char         cmd[80];
    FILE         *fp2;
    boolean32    grpoverexistsalready;
    boolean32    pwoverexistsalready;
    error_status_t s;

    setlocale(LC_ALL, "");

    /* Command Line Args */
    /* Open connection to registry */
    st = open_site();
    if (BAD_STATUS(&st)) {
        print_err("Can't open connection to registry", st);
	tet_result(TET_FAIL);
        exit(1);
    }
/* now test stuff */

    tet_infoline("start of grp_override test");
    result = (char *) malloc(1024);
    expected = (char *) malloc(2048);

/* do /opt/dcelocal/etc/group_override setup stuff */

    printf("testing override_get_by_unix_num\n");
    st = override_get_by_unix_num (sec_rgy_domain_person,3,name,&status); /* should find it */
    sprintf(result, "%d %s",st,name);
    check_result("override_get_by_unix_num", 1, result, "1 bin", 0, 0);

    st = override_get_by_unix_num (sec_rgy_domain_person,1234,name,&status); /* shouldn't find it */
    sprintf(result, "%d",st);
    check_result("override_get_by_unix_num", 2, result, "0", 0, 0);

    st = override_get_by_unix_num (sec_rgy_domain_group,333,name,&status); /* should find it */
    sprintf(result, "%d %s",st,name);
    check_result("override_get_by_unix_num", 3, result, "1 kmem", 0, 0);

    st = override_get_by_unix_num (sec_rgy_domain_group,1234,name,&status); /* shouldn't find it */
    sprintf(result, "%d",st);
    check_result("override_get_by_unix_num", 4, result, "0", 0, 0);
    printf("\n");


    printf("testing override_is_grp_pwd_overridden\n");
    strncpy((char *) name,"system", sec_rgy_name_max_len);
    st = override_is_grp_pwd_overridden (name,&gr_uid, gr_salt, &status);
    sprintf(result, "%d %s",  gr_uid,  gr_salt);
    check_result("override_is_grp_pwd_overidden", 5, result, "0 fo", st, 1);

    strncpy((char *) name,"uucp", sec_rgy_name_max_len);
    st = override_is_grp_pwd_overridden (name,&gr_uid, gr_salt, &status);
    sprintf(result, "%d %s", gr_uid,  gr_salt);
    check_result("override_is_grp_pwd_overidden", 6, result, "2 sn", st, 1);

    strncpy((char *) name,"kmem", sec_rgy_name_max_len);
    st = override_is_grp_pwd_overridden (name,&gr_uid, gr_salt, &status);
    sprintf(result, "");
    check_result("override_is_grp_pwd_overidden", 7, result, "", st, 0);
    printf("\n");

    printf("testing override_check_group_passwd():\n");
    printf("tp 8) test override_check_group_passwd() match condition\n");
    strcpy((char *) name,"system");
    strcpy((char *) pw_name,"foobar");
    overriden = override_check_group_passwd (name, &gr_uid, pw_name, &status);
    sprintf(result, "");
    check_result("override_check_group_passwd", 8, result, "", overriden, 1);

    printf("tp 9) test override_check_group_passwd() no match condition\n");
    strcpy((char *) pw_name,"flipper");
    st = override_check_group_passwd (name, &gr_uid, pw_name, &status);
    sprintf(result, "");
    check_result("override_check_group_passwd", 9, result, "", st, 0);
    printf("\n");

    printf("testing sec_rgy_pgo_get_by_eff_unix_num\n");
    sec_rgy_cursor_reset(&cursor);
    unix_id = 2;
    sec_rgy_pgo_get_by_eff_unix_num(context, sec_rgy_domain_group, (unsigned_char_p_t) "", 
					unix_id, true, &cursor, &pgo_item, name, &overriden, &st);
    sprintf(result, "%s %d %d", name, overriden, pgo_item.unix_num);
    check_result("sec_rgy_pgo_get_by_eff_unix_num", 10, result, "uucp 1 2",st,0);
    unix_id = 3;
    sec_rgy_pgo_get_by_eff_unix_num(context, sec_rgy_domain_group, (unsigned_char_p_t) "", 
					unix_id, true, &cursor, &pgo_item, name, &overriden, &st);
    sprintf(result, "%s %d %d", name, overriden, pgo_item.unix_num);
    check_result("sec_rgy_pgo_get_by_eff_unix_num", 11, result, "bin 0 3", st, 0);
    unix_id = 333;
    sec_rgy_pgo_get_by_eff_unix_num(context, sec_rgy_domain_group, (unsigned_char_p_t) "", 
					unix_id, true, &cursor, &pgo_item, name, &overriden, &st);
    sprintf(result, "%s %d %d", name, overriden, pgo_item.unix_num);
    check_result("sec_rgy_pgo_get_by_eff_unix_num", 12, result, "kmem 1 4", st, 0);
    printf("\n");

    printf("testing override_get_group_info\n");
    strncpy((char *) name,"kmem", sec_rgy_name_max_len);
    sec_rgy_cursor_reset(&member_cursor);
    override_get_group_info (name, MAX_MEMBERS, &gr_uid, 
			 &member_cursor, gr_passwd, member_list,
			 &mbrs_supplied, &num_members, &overridemsk, &status);
    sprintf(result, "%s %d %s %d %d %d",name, gr_uid, gr_passwd, overridemsk, mbrs_supplied, num_members);
    check_result("override_get_group_info", 13, result, "kmem 333  1 0 0", status, 0);
    strncpy((char *) name,"nogroup", sec_rgy_name_max_len);
    sec_rgy_cursor_reset(&member_cursor);
    override_get_group_info (name, MAX_MEMBERS, &gr_uid, 
			 &member_cursor, gr_passwd, member_list,
			 &mbrs_supplied, &num_members, &overridemsk, &status);
    sprintf(result, "%s %d %s %d %d %d %s",name, gr_uid, gr_passwd, overridemsk, mbrs_supplied, num_members, member_list[1]);
    check_result("override_get_group_info", 14, result, "nogroup 555  5 50 100 a1", status, 0);
    strncpy((char *) name,"uucp", sec_rgy_name_max_len);
    sec_rgy_cursor_reset(&member_cursor);
    override_get_group_info (name, MAX_MEMBERS, &gr_uid, 
			 &member_cursor, gr_passwd, member_list,
			 &mbrs_supplied, &num_members, &overridemsk, &status);
    sprintf(result, "%s %d %s %d %d %d",name, gr_uid, gr_passwd, overridemsk, mbrs_supplied, num_members);
    check_result("override_get_group_info", 15, result, "uucp 2 snafu 3 0 0", status, 0);
    strncpy((char *) name,"flubber", sec_rgy_name_max_len);
    sec_rgy_cursor_reset(&member_cursor);
    override_get_group_info (name, MAX_MEMBERS, &gr_uid, 
			 &member_cursor, gr_passwd, member_list,
			 &mbrs_supplied, &num_members, &overridemsk, &status);
    sprintf(result, "%s %d %s %d %d %d",name, gr_uid, gr_passwd, overridemsk, mbrs_supplied, num_members);
    check_result("override_get_group_info", 16, "", "", status, sec_login_s_ovrd_ent_not_found);
    printf("\n");

/*+++ update time of group_override file to cover roverride.c code where we clean out an existing
      group override list and start over from scratch */

    printf("testing modify of /opt/dcelocal/etc/group_override after sec_clientd boot\n");
    sleep(1); /* because stat(group_override) granularity is 1 sec */
    sprintf(cmd,"touch /opt/dcelocal/etc/group_override");
    s = system(cmd);

    /* do a sanity check on it */

    printf("testing override_get_group_info\n");
    strncpy((char *) name,"kmem", sec_rgy_name_max_len);
    sec_rgy_cursor_reset(&member_cursor);
    override_get_group_info (name, MAX_MEMBERS, &gr_uid, 
			 &member_cursor, gr_passwd, member_list,
			 &mbrs_supplied, &num_members, &overridemsk, &status);
    sprintf(result, "%s %d %s %d %d %d",name, gr_uid, gr_passwd, overridemsk, mbrs_supplied, num_members);
    check_result("override_get_group_info", 17, result, "kmem 333  1 0 0", status, 0);
    printf("\n");

/*---*/
/*+++ change protection on /opt/dcelocal/etc/group_override  - root can STILL read it */

    printf("testing modify of /opt/dcelocal/etc/group_override after sec_clientd boot\n");
    sprintf(cmd,"chmod 111 /opt/dcelocal/etc/group_override");
    s = system(cmd);

    /* do a sanity check on it */

    printf("testing override_get_group_info\n");
    strncpy((char *) name,"kmem", sec_rgy_name_max_len);
    sec_rgy_cursor_reset(&member_cursor);
    override_get_group_info (name, MAX_MEMBERS, &gr_uid, 
			 &member_cursor, gr_passwd, member_list,
			 &mbrs_supplied, &num_members, &overridemsk, &status);
    sprintf(result, "%s %d %s %d %d %d",name, gr_uid, gr_passwd, overridemsk, mbrs_supplied, num_members);
    check_result("override_get_group_info", 18, result, "kmem 333  1 0 0", status, 0);
    printf("\n");

/*---*/

/*+++ remove group_override file */
    printf("testing errors when /opt/dcelocal/etc/group_override is removed\n");
    sprintf(cmd,"rm -f /opt/dcelocal/etc/group_override");
    s = system(cmd);
    /* do a sanity check on it */

    printf("testing override_get_group_info\n");
    strncpy((char *) name,"kmem", sec_rgy_name_max_len);
    sec_rgy_cursor_reset(&member_cursor);
    override_get_group_info (name, MAX_MEMBERS, &gr_uid, 
			 &member_cursor, gr_passwd, member_list,
			 &mbrs_supplied, &num_members, &overridemsk, &status);
    check_result("override_get_group_info", 19, "", "", status, sec_login_s_no_override_info);
    printf("\n");

/*---*/

/*+++ reinstate group override file, do a sanity check on it */
    printf("now reinstate /opt/dcelocal/etc/group_override and do a sanity check\n");
    sleep(1); /* because stat(group_override) granularity is 1 sec */
    sprintf(cmd,"cp group_override /opt/dcelocal/etc");
    s = system(cmd);
    sprintf(cmd,"touch /opt/dcelocal/etc/group_override");
    s = system(cmd);
    sprintf(cmd,"sync");
    s = system(cmd);
    printf("testing override_get_group_info\n");
    strncpy((char *) name,"kmem", sec_rgy_name_max_len);
    sec_rgy_cursor_reset(&member_cursor);
    override_get_group_info (name, MAX_MEMBERS, &gr_uid, 
			 &member_cursor, gr_passwd, member_list,
			 &mbrs_supplied, &num_members, &overridemsk, &status);
    sprintf(result, "%s %d %s %d %d %d",name, gr_uid, gr_passwd, overridemsk, mbrs_supplied, num_members);
    check_result("override_get_group_info", 20, result, "kmem 333  1 0 0", status, 0);
    printf("\n");
/*---*/

/*+++ now check all that member cursor stuff */
    printf("testing override_get_group_info member cursor stuff\n");
    strncpy((char *) name,"nogroup", sec_rgy_name_max_len);
    sec_rgy_cursor_reset(&member_cursor);
    override_get_group_info (name, MAX_MEMBERS, &gr_uid, 
			 &member_cursor, gr_passwd, member_list,
			 &mbrs_supplied, &num_members, &overridemsk, &status);
    sprintf(result, "%s %d %s %d %d %d %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",name, gr_uid, gr_passwd, overridemsk, mbrs_supplied, num_members, member_list[0], member_list[1], member_list[2], member_list[3], member_list[4], member_list[5], member_list[6], member_list[7], member_list[8], member_list[9], member_list[10], member_list[11], member_list[12], member_list[13], member_list[14], member_list[15], member_list[16], member_list[17], member_list[18], member_list[19], member_list[20], member_list[21], member_list[22], member_list[23], member_list[24], member_list[25], member_list[26], member_list[27], member_list[28], member_list[29], member_list[30], member_list[31], member_list[32], member_list[33], member_list[34], member_list[35], member_list[36], member_list[37], member_list[38], member_list[39], member_list[40], member_list[41], member_list[42], member_list[43], member_list[44], member_list[45], member_list[46], member_list[47], member_list[48], member_list[49]);
    strcpy(expected, "nogroup 555  5 50 100 a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 a0 a1 a2 a3 a4 a5 a6 a7 a8 a9");
    check_result("override_get_group_info", 21, result,expected , status, 0);
    override_get_group_info (name, MAX_MEMBERS, &gr_uid, 
			 &member_cursor, gr_passwd, member_list,
			 &mbrs_supplied, &num_members, &overridemsk, &status);
    sprintf(result, "%s %d %s %d %d %d %s",name, gr_uid, gr_passwd, overridemsk, mbrs_supplied, num_members, member_list[1]);
    check_result("override_get_group_info", 22, result, "nogroup 555  5 50 100 a1", status, 0);

    override_get_group_info (name, MAX_MEMBERS, &gr_uid, 
			 &member_cursor, gr_passwd, member_list,
			 &mbrs_supplied, &num_members, &overridemsk, &status);
    check_result("override_get_group_info", 23, "", "", status, sec_rgy_no_more_entries );

    strncpy((char *) name,"kmem", sec_rgy_name_max_len);
    sec_rgy_cursor_reset(&member_cursor);
    override_get_group_info (name, MAX_MEMBERS, &gr_uid, 
			 &member_cursor, gr_passwd, member_list,
			 &mbrs_supplied, &num_members, &overridemsk, &status);
    sprintf(result, "%s %d %s %d %d %d",name, gr_uid, gr_passwd, overridemsk, mbrs_supplied, num_members);
    check_result("override_get_group_info", 24, result, "kmem 333  1 0 0", status, 0);

    strncpy((char *) name,"daemon", sec_rgy_name_max_len);
    sec_rgy_cursor_reset(&member_cursor);
    gr_uid = 1;
    override_get_group_info (name, MAX_MEMBERS, &gr_uid, 
			 &member_cursor, gr_passwd, member_list,
			 &mbrs_supplied, &num_members, &overridemsk, &status);
    sprintf(result, "%s %d %s %d %d %d",name, gr_uid, gr_passwd, overridemsk, mbrs_supplied, num_members);
    check_result("override_get_group_info", 25, result, "daemon 1  4 2 2", status, 0);
    printf("\n");

    sec_rgy_site_close(context, &st);

    tet_infoline("end of grp_override test");
}

