/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: passwd_export.c,v $
 * Revision 1.1.13.2  1996/03/11  13:26:16  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:18  marty]
 *
 * Revision 1.1.13.1  1995/12/08  17:56:12  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:41 UTC  jrr
 * 	Conditionailize the use of sigvec() / sigvector() depending on platform.
 * 
 * 	HP revision /main/HPDCE02/6  1995/10/17  21:38 UTC  bartf
 * 	 * merge group_override fix for members printed twice
 * 	 * [1995/10/05  20:42 UTC  bartf  /main/HPDCE02/5]
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_wc.10/7  1995/10/13  19:20 UTC  bartf
 * 	add more write checking
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_wc.10/6  1995/10/12  12:44 UTC  bartf
 * 	also remove temp files if file errror
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_wc.10/5  1995/10/10  21:46 UTC  bartf
 * 	add file checking conditions
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_wc.10/4  1995/10/09  14:14 UTC  bartf
 * 	fix for manay members corruption(CHFts16418)
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_wc.10/3  1995/10/04  16:35 UTC  bartf
 * 	fix multi group members loop bug
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_wc.10/2  1995/10/04  15:12 UTC  bartf
 * 	fix multi group members loop bug
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_wc.10/1  1995/10/02  20:35 UTC  bartf
 * 	fix multi group members bug
 * 
 * 	HP revision /main/HPDCE02/4  1995/06/26  14:14 UTC  mullan_s
 * 	Merge fix for CHFts15566.
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_chfts15566/1  1995/06/26  14:09 UTC  mullan_s
 * 	Lock /etc/ptmp before binding to registry.
 * 
 * 	HP revision /main/HPDCE02/3  1995/06/09  10:15 UTC  sekhar
 * 	Merge to mothra.
 * 
 * 	HP revision /main/HPDCE02/sekhar_mothra_fix/1  1995/05/31  11:54 UTC  sekhar
 * 	[ CHFts15248 ] Log absolute pathnames for files in error messages.
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/30  23:20 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:33 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:21 UTC  mullan_s
 * 	Merge.
 * 	[1995/12/08  16:28:49  root]
 * 
 * Revision 1.1.10.4  1994/08/04  16:12:25  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/28  11:40:58  mdf]
 * 
 * Revision 1.1.10.3  1994/05/05  20:44:28  hasbrouc
 * 	add group override support (RFC 55)
 * 	[1994/05/05  20:11:11  hasbrouc]
 * 	HP revision /main/GAMERA/KK/ODESSA_2/hanfei_svc_bl1/3  1994/07/12  18:20 UTC  hanfei
 * 	change two dce_printf() calls to dce_fprintf().
 * 
 * 	HP revision /main/GAMERA/KK/ODESSA_2/hanfei_svc_bl1/2  1994/06/23  13:51 UTC  hanfei
 * 	change some dce_fprintf to be dce_pgm_frpintf.
 * 
 * 	HP revision /main/GAMERA/KK/ODESSA_2/hanfei_svc_bl1/1  1994/06/22  20:34 UTC
 * 	put in servicibility handles for error messages.
 * 
 * 	HP revision /main/GAMERA/KK/sekhar_svc/1  1994/05/13  19:38 UTC  sekhar
 * 	SVC changes. Merge from OSF DCE1.1 source tree.
 * 
 * 	HP revision /main/GAMERA/KK/ODESSA_2/1  1994/05/04  20:33 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/GAMERA/KK/hanfei_ch_master/1  1994/05/04  20:10 UTC  hanfei
 * 	merge
 * 
 * Revision 1.1.11.5  1993/12/28  16:01:50  hanfei
 * 	fix loop condition for getting group members.
 * 	[1993/12/28  15:55:33  hanfei]
 * 
 * Revision 1.1.11.4  1993/12/22  21:15:28  hanfei
 * 	add lock to ensure single passwd_export access
 * 	[1993/12/22  21:05:55  hanfei]
 * 
 * Revision 1.1.11.3  1993/12/01  21:58:04  hanfei
 * 	[DTS 9590] with override, if override infomation cannot be
 * 	obtained because server is not available, bail out.
 * 	[1993/12/01  21:57:30  hanfei]
 * 
 * Revision 1.1.11.2  1993/07/08  10:48:55  root
 * 	Initial King Kong branch
 * 	[1993/07/08  10:48:26  root]
 * 
 * Revision 1.1.8.3  1993/05/25  16:31:26  rps
 * 	standardize version string
 * 	[1993/05/25  16:21:39  rps]
 * 
 * Revision 1.1.8.2  1993/03/30  20:21:49  ahop
 * 	Merge in HP changes: prime_group.
 * 	[1993/03/30  20:16:30  ahop]
 * 
 * Revision 1.1.1.8  1993/03/11  19:41:45  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.10.2  1994/04/01  20:08:12  jd
 * 	Undefine _NO_PROTO  so the cma prototypes don;t complain. Tis presumably
 * 	won't need to be undef'd when the security cleanup happens.
 * 	[1994/03/28  20:20:34  jd]
 * 
 * Revision 1.1.10.1  1994/03/25  17:21:56  sekhar
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 	[1994/03/25  17:17:40  sekhar]
 * 
 * Revision 1.1.6.1  1993/10/05  22:29:52  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:03:49  mccann]
 * 
 * Revision 1.1.4.5  1993/02/05  21:13:04  burati
 * 	CR7139 Fix the length of passwds for strncpy in make_files()
 * 	[1993/02/05  20:51:27  burati]
 * 
 * Revision 1.1.4.4  1993/01/08  22:17:00  tom
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:00:42  tom]
 * 
 * Revision 1.1.4.3  1992/12/29  12:37:34  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:33:33  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/01  15:49:27  burati
 * 	CR6241 Usage typo, and off by 1 error on entry limit.
 * 	[1992/11/30  23:41:19  burati]
 * 
 * Revision 1.1.2.3  1992/06/26  14:58:50  burati
 * 	CR4469 Make sure it checks omit flag before omitting entries
 * 	[1992/06/25  23:19:44  burati]
 * 
 * Revision 1.1.2.2  1992/04/03  22:21:25  burati
 * 	CR2538 Fix error handling, change getwd to getcwd
 * 	[1992/04/03  21:30:26  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 *
 * Copyright (c) Hewlett-Packard Company 1991, 1993
 * Unpublished work. All Rights Reserved.
 *
*/
/*
**      Create passwd and group files from registry account information
*/

#include <dce/dce.h>
#undef _NO_PROTO
#include <un_maloc.h>
#include <passwd_export.h>
#include <stdio.h>
#include <sys/file.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>
#include <macros.h>
#include <un_io.h>
#include <locale.h>
#include <signal.h>

#include <dce/binding.h>
#include <dce/acct.h>
#include <dce/override.h>
#include <dce/misc.h>
#include <dce/pgo.h>
#include <dce/rpcbase.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>

#include <dce/dce_msg.h>
#include <dce/dcesadmsg.h>
#include <sys/stat.h>


typedef char pwd_buf_t[MAX_PWD_BUF];
char    g_cur_dir[MAXPATHLEN+1];  /* logged by SVC messages */

#define CVT_ABSPATH(path) strcat(strcat(g_cur_dir, "/"), path)
#ifdef HPUX
#define SIGVEC sigvector
#else
#define SIGVEC sigvec
#endif

/**** Global state ****/
sec_rgy_handle_t    context;
static boolean32    verbose = false;

/**** Procedures  *****/

PRIVATE void asyn_sig_handler()
{
    /* clean up the temp lock file */
    unlink(TEMP_PASSWD_FILE);
    exit(1);
}

PRIVATE void setup_sig_handler()
{
    struct sigvec nvec, ovec;
    long   omask;
    long   oflag;

    /* get current setting */
    SIGVEC(SIGHUP, 0, &ovec); 
    omask = ovec.sv_mask;
    oflag = ovec.sv_flags;

    /* setup SIGHUP, SIGINT and SIGTERM for asyn_sig_handler */
    /* and SIGWINCH to be ignored */
    nvec.sv_handler = asyn_sig_handler;
    nvec.sv_mask = omask;
    nvec.sv_flags = oflag;

    SIGVEC(SIGHUP, &nvec, &ovec); 

    nvec.sv_handler = asyn_sig_handler;
    nvec.sv_mask = omask;
    nvec.sv_flags = oflag;

    SIGVEC(SIGINT, &nvec, &ovec); 

    nvec.sv_handler = asyn_sig_handler;
    nvec.sv_mask = omask;
    nvec.sv_flags = oflag;

    SIGVEC(SIGTERM, &nvec, &ovec); 
    
    nvec.sv_handler = SIG_IGN;
    nvec.sv_mask = omask;
    nvec.sv_flags = oflag;

    SIGVEC(SIGWINCH, &nvec, &ovec); 
}

/*
 * Bind to any query site
 */
error_status_t open_site(
void
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


/* Qsort passes in *pointers* to the elements in the array, hence
 *  we take pointers to pointers to passwd structs as parameters
 */
int compare_uids
  (
    const void *   ptr1,
    const void *   ptr2
  )
{
    char    *tmpc;
    int     uid1, uid2;
    /* If we can't find the Unix IDs, somethings wrong with the input string */
    if (sscanf(ptr1, "%*[^:]:%*[^:]:%d", &uid1)) {
        if (sscanf(ptr2, "%*[^:]:%*[^:]:%d", &uid2)) {
            if (uid1 < uid2)
                return(-1);
            if (uid1 > uid2)
                return(1);

            /* Unix ids must be the same if we are here
             * so now compare login names.
             */
            return(strcmp(ptr1, ptr2));
        }
    }
    return(0);
}


/*
 * Prime the group file with SYS_GROUP_FILE
 */
void prime_group
  (
    FILE *fp
  )
{
    int         tfd;
    FILE        *tfp;
    pwd_buf_t   tmp_buf;

    tfp = fopen(SYS_GROUP_FILE, "r");
    if (tfp != NULL ) {
        if (verbose) {
            printf("Using %s to prime the group file\n", SYS_GROUP_FILE);
        }
        while (!feof(tfp)) {
            if (fgets(tmp_buf, sizeof(tmp_buf), tfp)) {
                fputs(tmp_buf, fp);
            }
	    if ( ferror(fp) != 0 ) {
		fclose(fp);
	   	dce_pgm_fprintf(stderr, pwd_exp_no_space, strerror(errno));
	        fclose(tfp);
		exit(1);
	    }
        }
        fclose(tfp);
    } else if (errno != ENOENT) {
        fflush(stdout);
	dce_pgm_fprintf(stderr, pwd_exp_f_cannot_open_prime_group, 
		    SYS_GROUP_FILE, strerror(errno) );
    }
}

boolean32 check_file_ok(char * f) {
    FILE * cfp;
    struct stat *sbuf;
    pwd_buf_t   tmp_pwdbuf;

    if ( stat( f, sbuf ) < 0 ) {
	dce_pgm_fprintf(stderr, pwd_exp_no_space, strerror(errno));
	return false;
    }
    else if ( sbuf->st_size <= 0 ) {
	dce_pgm_fprintf(stderr, pwd_exp_no_space, strerror(errno));
	return false;
    } else {
	/* check if root exists in file */
	cfp = fopen( f, "r" );
	if ( ferror(cfp) != 0 ) {
	   dce_pgm_fprintf(stderr, pwd_exp_no_space, strerror(errno));
           fclose(cfp);
	   return false;
        }
        while (!feof(cfp)) {
	   if (fgets(tmp_pwdbuf, sizeof(tmp_pwdbuf), cfp)) {
              if ( strncmp("root:", tmp_pwdbuf, 5) == 0 ) {
                 fclose(cfp);
            	 return true;
              }
	   }
        }
        fclose(cfp);
	dce_pgm_fprintf(stderr, pwd_exp_no_space, strerror(errno));
	fprintf(stderr, " Entry for root does not exist in %s.\n", f);
    }
    return false;
}


void make_files
  (
    long      entry_limit,
    boolean32 omit_flag,
    boolean32 sort_flag,
    boolean32 no_ovrd_flag
  )
{
    int                 fd;
    FILE                *fp;
    long                i;
    signed32            mbrs_supplied, num_members, mbrs_len;
    struct passwd       *pwent;
    struct group        *grent;
    error_status_t      status, lstat, grp_ovr_stat;
    boolean32           write_error, buffer_ok, line_too_long, root_exists;
    int                 entry_count, mbrs_saved;
    sec_rgy_cursor_t    cursor, cursor2, member_cursor;
    sec_rgy_login_name_t    acct_name, null_name;
    sec_rgy_sid_t           id_sid;
    sec_rgy_unix_sid_t      unix_sid;
    sec_rgy_acct_key_t      key_parts;
    sec_rgy_acct_user_t     user_part;
    sec_rgy_acct_admin_t    admin_part;
    pwd_buf_t               passwd_buf;
    pwd_buf_t               *sort_buf;
    sec_rgy_pgo_item_t      pgo_item;
    sec_rgy_name_t          group;
    sec_rgy_member_t        member_list[MAX_MEMBERS];

    /* Override info */
    sec_rgy_unix_passwd_buf_t   pw_passwd;
    sec_rgy_name_t              pw_gecos;
    sec_rgy_name_t              pw_dir;
    sec_rgy_name_t              pw_shell;
    sec_override_fields_t       overridden;
    sec_rgy_unix_passwd_buf_t   gr_passwd;


    null_name.pname[0] = '\0';
    null_name.gname[0] = '\0';
    null_name.oname[0] = '\0';

    fd = open(TEMP_PASSWD_FILE, O_RDWR|O_CREAT|O_TRUNC|O_EXCL, 0644);
    if (fd == -1) {
        dce_pgm_fprintf(stderr, pwd_exp_f_cannot_open_temp_passwd,
                "open", CVT_ABSPATH(TEMP_PASSWD_FILE), strerror(errno));
	exit(1);
    }
    fp = fdopen(fd, "w+");
    if ( fp == NULL ) {
        dce_pgm_fprintf(stderr, pwd_exp_f_cannot_open_temp_passwd,
                "fdopen", CVT_ABSPATH(TEMP_PASSWD_FILE), strerror(errno));
	exit(1);
    }

    /* Open connection to registry */
    status = open_site();
    if (BAD_STATUS(&status)) {
	fclose(fp);
        unlink(TEMP_PASSWD_FILE);
	dce_pgm_fprintf(stderr, status);
	dce_pgm_fprintf(stderr, pwd_exp_rgy_cannot_open);
	exit(1);
    }

    /* Reset the account lookup cursor */
    sec_rgy_cursor_reset(&cursor);

    if (verbose)
        printf("Writing temporary passwd file ...\n");
    fflush(stdout);
    entry_count = 0;
    write_error = false;
    CLEAR_STATUS(&status);

    while (GOOD_STATUS(&status) && (entry_count < entry_limit)) {

        sec_rgy_acct_lookup(context, &null_name, &cursor, &acct_name, &id_sid,
                            &unix_sid, &key_parts, &user_part, &admin_part,
                            &status);
        if (GOOD_STATUS(&status)) {
            buffer_ok = true;

            if (! no_ovrd_flag) {
                /* Check for override info for this acct */
                override_get_login_info(acct_name.pname, &unix_sid.person,
                                    &unix_sid.group, pw_passwd, pw_gecos,
                                    pw_dir, pw_shell, &overridden, &status);
		/* when override fails because server (sec_clientd) is */
		/* not available, bail out.                            */
		if ( STATUS_EQUAL(&status, rpc_s_connect_rejected) ) {
		    fclose(fp);
                    unlink(TEMP_PASSWD_FILE);
		    dce_pgm_fprintf(stderr, pwd_exp_rgy_connect_rejected);
		    exit(1);
		}
                if ((GOOD_STATUS(&status)) && overridden) {  /* Overridden */

                    if (overridden & sec_override_pw_passwd) {
                        if (omit_flag && strcmp((char *)pw_passwd, "OMIT") == 0)
                            buffer_ok = false;
                        else {
                            strncpy((char *)user_part.passwd,
                                    (char *)pw_passwd,
                                    sec_rgy_max_unix_passwd_len);
                            user_part.passwd[sec_rgy_max_unix_passwd_len-1]='\0';
			}

                    }
                    if (overridden & sec_override_pw_gecos) {
                        strncpy((char *)user_part.gecos, (char *)pw_gecos,
                                sec_rgy_pname_max_len);
                    }
                    if (overridden & sec_override_pw_dir) {
                        strncpy((char *)user_part.homedir, (char *)pw_dir,
                                sec_rgy_pname_max_len);
                    }
                    if (overridden & sec_override_pw_shell) {
                        strncpy((char *)user_part.shell, (char *)pw_shell,
                                sec_rgy_pname_max_len);
                    }
                } else {
                    CLEAR_STATUS(&status);
                }
            }

            if (buffer_ok) {
                ++entry_count;
		  sprintf(passwd_buf, "%s:%s:%d:%d:%s:%s:%s\n",
                        acct_name.pname, user_part.passwd, (signed32)unix_sid.person,
                        (signed32)unix_sid.group, user_part.gecos, user_part.homedir,
                        user_part.shell);
                fputs(passwd_buf, fp);
		if ( ferror(fp) != 0 ) {
			fclose(fp);
			unlink(TEMP_PASSWD_FILE);
	   		dce_pgm_fprintf(stderr, pwd_exp_no_space, strerror(errno));
			exit(1);
		}
            }
        }
    }
    if (STATUS_EQUAL(&status, sec_rgy_no_more_entries)) {
        CLEAR_STATUS(&status);
    }

    if (GOOD_STATUS(&status) && sort_flag) {
        /* read the passwd buffers into a single array for sorting */
        sort_buf = (pwd_buf_t *) malloc(entry_count * sizeof(pwd_buf_t));
	if (sort_buf == NULL) {
	    fclose(fp);
            unlink(TEMP_PASSWD_FILE);
	    dce_pgm_fprintf(stderr, pwd_exp_err_no_memory);
	    exit(1);
	}
        fseek(fp, 0, SEEK_SET);
        for (i = 0; i < entry_count; i++) {
            fgets(sort_buf[i], MAX_PWD_BUF, fp);
        }

        if (verbose)
            printf("Sorting passwd entries.\n");
        fflush(stdout);
        qsort(sort_buf, entry_count, sizeof(pwd_buf_t), compare_uids);

        if (verbose)
            printf("Done sorting.\n");

        /* Truncate the original temporary file so we can rewrite it
         * with the sorted data.  Make sure file pointer starts at 0.
         */
        ftruncate(fd, 0);
        fseek(fp, 0, SEEK_SET);
        for (i = 0; i < entry_count; i++) {
            fputs(sort_buf[i], fp);
        }
	if ( ferror(fp) != 0 ) {
		fclose(fp);
		unlink(TEMP_PASSWD_FILE);
   		dce_pgm_fprintf(stderr, pwd_exp_no_space, strerror(errno));
		exit(1);
	}
        fclose(fp);
    } else {
        fclose(fp);
    }
        /* Don't do group file if passwd file failed */
    if (BAD_STATUS(&status)) {
        unlink(TEMP_PASSWD_FILE);
	dce_pgm_fprintf(stderr, status);
	dce_pgm_fprintf(stderr, pwd_exp_rgy_error_passwd);
	exit(1);
    }

    fd = open(TEMP_GROUP_FILE, O_RDWR|O_CREAT|O_TRUNC, 0644);
    if (fd == -1) {
	dce_pgm_fprintf(stderr, pwd_exp_f_cannot_open_temp_group,
		    CVT_ABSPATH(TEMP_GROUP_FILE), strerror(errno)); 
        unlink(TEMP_PASSWD_FILE);
    }
    fp = fdopen(fd, "w+");
    if (fp == NULL) {
	dce_pgm_fprintf(stderr, pwd_exp_f_cannot_open_temp_group,
		    CVT_ABSPATH(TEMP_GROUP_FILE), strerror(errno)); 
        unlink(TEMP_PASSWD_FILE);
        exit(1);
    }

    /* Reset the group lookup cursor */
    sec_rgy_cursor_reset(&cursor);

    if (verbose)
        printf("Writing group file ... \n");
    fflush(stdout);

    /* Prime group file with SYS_GROUP_FILE */
    prime_group(fp);

    entry_count = 0;
    write_error = false;
    CLEAR_STATUS(&status);

    while (GOOD_STATUS(&status) && (entry_count < entry_limit)) {
        sec_rgy_pgo_get_next(context, sec_rgy_domain_group,
                                (unsigned_char_p_t) "", &cursor, &pgo_item,
                                group, &status);
        if (GOOD_STATUS(&status)) {
            ++entry_count;
	    buffer_ok = true;

	    /* do a check here in the group_override file */

            sec_rgy_cursor_reset(&member_cursor);

	    gr_passwd[0] = '\0';

	    overridden = false;

	    if (! no_ovrd_flag) {
		override_get_group_info (group, MAX_MEMBERS, &pgo_item.unix_num, 
					 &member_cursor, gr_passwd, member_list,
					 &mbrs_supplied, &num_members, &overridden, &grp_ovr_stat);
		if ((GOOD_STATUS(&grp_ovr_stat)) && overridden) {
		    if (omit_flag && (strcmp((char *)gr_passwd, "OMIT")==0) )
			buffer_ok = false;
		    else {
			strncpy((char *)user_part.passwd, (char *)gr_passwd, sec_rgy_max_unix_passwd_len);
			user_part.passwd[sec_rgy_max_unix_passwd_len-1]= '\0';
		    }
		}
	    }

	    if (buffer_ok) {
		sprintf(passwd_buf, "%s:%s:%d:", group, gr_passwd, pgo_item.unix_num);
		fputs(passwd_buf, fp);
		if ( ferror(fp) != 0 ) {
			fclose(fp);
			unlink(TEMP_GROUP_FILE);
	   		dce_pgm_fprintf(stderr, pwd_exp_no_space, strerror(errno));
			exit(1);
		}

		/* Get all members */

		if ( (!overridden) || (! FLAG_SET(overridden, sec_group_override_member)) ) {
		    CLEAR_STATUS(&lstat);
		    mbrs_saved = 0; num_members = -1;
		    sec_rgy_cursor_reset(&cursor2);
            	    do {
			sec_rgy_pgo_get_members(context, sec_rgy_domain_group, group,
						&cursor2, MAX_MEMBERS, member_list,
						&mbrs_supplied, &num_members, &lstat);
			if (GOOD_STATUS(&lstat)) {
			    mbrs_saved += mbrs_supplied;
			    for (i = 0; i < mbrs_supplied; i++) {
				if ((i > 0) || (mbrs_saved > MAX_MEMBERS))
				    fputc(',', fp);
				fputs((char *)member_list[i], fp);
			    }
			    if ( ferror(fp) != 0 ) {
				fclose(fp);
				unlink(TEMP_GROUP_FILE);
	   			dce_pgm_fprintf(stderr, pwd_exp_no_space, strerror(errno));
				exit(1);
			    }
			}
	            } while ((GOOD_STATUS(&lstat)) && (mbrs_saved < num_members));
		} else {  
		    if (num_members > 0) {
			mbrs_saved = mbrs_supplied;
			/* get lenth of first 3 fields */
			line_too_long = false;
			mbrs_len = strlen( (char*)group) + sizeof(pgo_item.unix_num)
                                           + strlen((char*)gr_passwd) + 3; 
			for (i = 0; (i < mbrs_supplied) && (line_too_long == false); i++) { 
			    /* make sure it is less than the line lenth */
			    mbrs_len += strlen( (char *)member_list[i] );
                            if (mbrs_len < (LINE_MAX - 1)) {
				if ((i > 0) || (mbrs_saved > MAX_MEMBERS)) {
					fputc(',', fp);
					mbrs_len++;
				}
			    	fputs((char *)member_list[i], fp);
				if ( ferror(fp) != 0 ) {
					fclose(fp);
					unlink(TEMP_GROUP_FILE);
	   				dce_pgm_fprintf(stderr, pwd_exp_no_space, strerror(errno));
					exit(1);
			    	}
			    }
			    else line_too_long = true;
			}

			while ((GOOD_STATUS(&grp_ovr_stat)) && (mbrs_saved < num_members)){
			    override_get_group_info (group, MAX_MEMBERS, &pgo_item.unix_num, 
						     &member_cursor, gr_passwd, member_list,
						     &mbrs_supplied, &num_members, &overridden, &grp_ovr_stat);
			    if (GOOD_STATUS(&grp_ovr_stat)){
				mbrs_saved += mbrs_supplied;
				for (i = 0; (i < mbrs_supplied) && (line_too_long == false); i++) {
			            /* make sure it is less than the line lenth */
			            mbrs_len += strlen( (char *)member_list[i] );
	                            if (mbrs_len < (LINE_MAX - 1)) {
					if ((i > 0) || (mbrs_saved > MAX_MEMBERS)) {
						fputc(',', fp);
						mbrs_len++;
					}
					fputs((char *)member_list[i], fp);
					if ( ferror(fp) != 0 ) {
					   fclose(fp);
					   unlink(TEMP_GROUP_FILE);
	   				   dce_pgm_fprintf(stderr, pwd_exp_no_space, strerror(errno));
					   exit(1);
			    		}
				    }
				    else line_too_long = true;
				}
			    }
			}
		    }
		}
	    fputc('\n', fp);
	    } /* if (buffer_ok) */
	} /* if ((GOOD_STATUS */ 
    } /* End while */

    fclose(fp);
    if (STATUS_EQUAL(&status, sec_rgy_no_more_entries)) {
        CLEAR_STATUS(&status);
    }  

        /* Don't finish group file if lookups failed */
    if (BAD_STATUS(&status)) {
        unlink(TEMP_PASSWD_FILE);
	dce_pgm_fprintf(stderr, status);
	dce_pgm_fprintf(stderr, pwd_exp_rgy_error_group);
	exit(1);
    }

    /* Make .bak file */
    unlink(GROUP_BAK);
    link(DEFAULT_GROUP_FILE, GROUP_BAK);

    /* check that new group file is okay before replacing */
    if ( check_file_ok(TEMP_GROUP_FILE ) ) {
	/* Rename temp file to correct name */
	rename(TEMP_GROUP_FILE, DEFAULT_GROUP_FILE);

	/* Change ownership to root.staff */
	chown(DEFAULT_GROUP_FILE, 0, 10);

	/* Make file read-only */
	chmod( DEFAULT_GROUP_FILE, 0444);

	if (verbose)
        	printf("Group file written.\n");
    } else unlink( TEMP_GROUP_FILE );

    /* Make passwd.bak file */
    unlink(PASSWD_BAK);
    link(DEFAULT_PASSWD_FILE, PASSWD_BAK);

    /* check that new passwd file is okay before replacing */
    if ( check_file_ok(TEMP_PASSWD_FILE ) ) {
	/* Rename temp file to correct name */
	rename(TEMP_PASSWD_FILE, DEFAULT_PASSWD_FILE);

	/* Change ownership to root.staff */
	chown(DEFAULT_PASSWD_FILE, 0, 10);

	/* Make file read-only */
	chmod(DEFAULT_PASSWD_FILE, 0444);

	if (verbose)
        	printf("Passwd file written.\n");
    } else unlink( TEMP_PASSWD_FILE );

}


/*
 * Main routine for passwd_export
 */
main
  (
   int argc,
   char *argv[]
  )
{
    char        current_wd[MAXPATHLEN+1];
    long        i;
    int         result;
    char        *dir = NULL;
    boolean32   force_write;
    long        entry_limit = MAX_ENTRIES;
    boolean32   omit_flag = false;
    boolean32   sort_flag = false;
    boolean32   no_ovrd_flag = false;
    error_status_t  st;

    setup_sig_handler();

    setlocale(LC_ALL, "");

    dce_svc_set_progname("passwd_export", &st);
    dce_msg_define_msg_table(sad__table,
        sizeof sad__table / sizeof sad__table[0], &st);
    
    /* Command Line Args */
    for (i=1; i < argc; ++i) {
        if (strcmp(argv[i], LIMIT_ENTRIES) == 0) {
            if ((i+1) == argc) {
                dce_fprintf(stderr, pwd_exp_cl_bad_opt_m, argv[0]);
                exit(1);
            } else {
                entry_limit = atoi(argv[++i]);
                if ( entry_limit > MAX_ENTRIES || entry_limit < 0 ) {
                    dce_fprintf(stderr, pwd_exp_cl_bad_entries, argv[0], MAX_ENTRIES);
                    entry_limit = MAX_ENTRIES;
        	    fflush(stderr);
                }
            }
        } else if (strcmp(argv[i], OMIT_FLAG) == 0) {
            omit_flag = true;
        } else if (strcmp(argv[i], SORT_FLAG) == 0) {
            sort_flag = true;
        } else if (strcmp(argv[i], NO_OVRD_FLAG) == 0) {
            no_ovrd_flag = true;
        } else if (strcmp(argv[i], VERBOSE_FLAG) == 0) {
            verbose = true;
        } else if (strcmp(argv[i], OPT_DIR) == 0) {
            if ((i+1) == argc) {
                dce_fprintf(stderr, pwd_exp_cl_bad_opt_d, argv[0]);
		exit(1);
            } else {
                dir = argv[++i];
            }
        } else if (strncmp(argv[i], GIVE_HELP, 2) == 0) {
	    dce_fprintf(stderr, pwd_exp_cl_usage, argv[0]);
            exit(0);
        } else {
            dce_fprintf(stderr, pwd_exp_cl_bad_arg, argv[0], argv[i]);
	    dce_fprintf(stderr, pwd_exp_cl_usage, argv[0]);
            exit(1);
        }
    }

    /* Changed to the desired directory and get it's  full pathname */
    if (dir == NULL)
        dir = DEFAULT_DIRECTORY;

    result = chdir(dir);
    if (result != 0 ) {
	dce_pgm_fprintf(stderr, pwd_exp_env_fail_change_dir, dir, strerror(errno));
        fflush(stderr);
    } else {
        if (verbose)
            printf("All output files will be saved in the %s directory.\n",
                    dir);
    }

    if ((dir = getcwd(g_cur_dir, sizeof(g_cur_dir))) == (char *)NULL) {
	dce_pgm_fprintf(stderr, pwd_exp_env_fail_getcwd, strerror(errno));
	exit(1);
    }
    make_files(entry_limit, omit_flag, sort_flag, no_ovrd_flag);

    sec_rgy_site_close(context, &st);
    if (BAD_STATUS(&st)) {
	dce_pgm_fprintf(stderr, st);
	dce_pgm_fprintf(stderr, pwd_exp_rgy_cannot_close);
	exit(1);
    }
    /* Make sure the command returns a 0 on success for testing */
    exit(0);
}

