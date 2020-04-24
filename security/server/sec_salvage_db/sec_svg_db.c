/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_svg_db.c,v $
 * Revision 1.1.14.3  1996/02/18  00:22:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:49  marty]
 *
 * Revision 1.1.14.2  1995/12/08  18:00:59  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/6  1995/07/11  18:20 UTC  jrr
 * 	Fix call to rsdb_file_stat().
 * 	[1995/07/06  18:05 UTC  jrr  /main/HPDCE02/hanfei_mothra_bl4/2]
 * 
 * 	build acl separately from pgo to save memory
 * 	[1995/06/23  13:18 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/1]
 * 
 * 	HP revision /main/HPDCE02/5  1995/05/15  21:11 UTC  greg
 * 	merge
 * 	[1995/05/01  19:17 UTC  hanfei  /main/HPDCE02/SEC_migrate_merge/2]
 * 
 * 	add checking database version when doing reconstruct
 * 	[1995/05/01  19:03 UTC  hanfei  /main/HPDCE02/SEC_migrate/hanfei_migrate_bl2/1]
 * 
 * 	Create SEC_migrate_merge mainline by merging SEC_migrate and HPDCE02.
 * 	[1995/04/20  20:48 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	HP revision /main/HPDCE02/4  1995/01/30  22:44 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/3  1994/11/17  18:19 UTC  mullan_s
 * 	Merge unintegrated
 * 	[1995/12/08  17:22:37  root]
 * 
 * Revision 1.1.10.1  1994/10/26  15:17:30  hanfei
 * 	ERA enhancement work.
 * 
 * 	enhancement for ERA.
 * 	Revision 1.1.7.8  1994/09/20  19:18:43  hanfei
 * 	HP revision /main/HPDCE02/2  1994/11/15  21:23 UTC  mullan_s
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  20:04  mdf
 * 	merge
 * 
 * Revision 1.1.5.2  1993/07/08  12:59:22  root
 * 	Initial King Kong branch
 * 	[1993/07/08  12:58:54  root]
 * 
 * Revision 1.1.3.4  1993/06/17  21:27:07  rps
 * 	Included domestic vs. export in libdce rev_ctl.
 * 	[1993/06/17  17:03:12  rps]
 * 
 * Revision 1.1.3.3  1993/06/11  13:00:31  hanfei
 * 	disable few options for -reconstruct.
 * 	[1993/06/11  12:59:25  hanfei]
 * 
 * Revision 1.1.3.2  1993/05/25  16:33:02  rps
 * 	standardize version string
 * 	[1993/05/25  16:22:53  rps]
 * 
 * Revision 1.1.1.6  1993/05/04  20:41:23  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * 	HP revision /main/HPDCE02/1  1994/10/27  15:29 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_svg_bl1/3  1994/10/25  17:25 UTC  hanfei
 * 	Enhancement work for attribute schema and attributes.
 * 
 * 	HP revision /main/hanfei_svg_bl1/2  1994/09/16  17:02 UTC  hanfei
 * 	add codes to put DCE 1.1 stuff in info.prt.
 * 
 * 	HP revision /main/hanfei_svg_bl1/1  1994/09/14  19:23 UTC  hanfei
 * 	codes for backward conversion from DCE 1.1 to DCE 1.0.3.
 * 
 * Revision 1.1.7.7  1994/06/17  18:42:57  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:20:06  devsrc]
 * 
 * Revision 1.1.7.6  1994/04/04  17:53:18  desai
 * 	Fix for CR 10277. Remove * from aud_status declaration and move it to the
 * 	beginning of the function.
 * 	[1994/04/04  17:52:36  desai]
 * 
 * Revision 1.1.7.5  1994/04/01  21:24:55  kline_s
 * 	Merged with changes from 1.1.7.4
 * 	[1994/04/01  21:24:40  kline_s]
 * 
 * 	[OT 10264] First audit code point drop for security.
 * 	[1994/04/01  21:17:42  kline_s]
 * 
 * Revision 1.1.7.4  1994/03/28  21:35:32  sekhar
 * 	Fix merge errors from changes for OT 101064 .
 * 	[1994/03/28  18:54:53  sekhar]
 * 
 * Revision 1.1.7.3  1994/03/25  18:23:07  sekhar
 * 	Merged with changes from 1.1.7.2
 * 	[1994/03/25  18:22:41  sekhar]
 * 
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 	              Also call setlocale().
 * 	[1994/03/25  18:20:10  sekhar]
 * 
 * Revision 1.1.7.4  1994/03/28  21:35:32  sekhar
 * 	Fix merge errors from changes for OT 101064 .
 * 	[1994/03/28  18:54:53  sekhar]
 * 
 * Revision 1.1.7.3  1994/03/25  18:23:07  sekhar
 * 	Merged with changes from 1.1.7.2
 * 	[1994/03/25  18:22:41  sekhar]
 * 
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 	              Also call setlocale().
 * 	[1994/03/25  18:20:10  sekhar]
 * 
 * Revision 1.1.7.2  1994/03/07  15:06:30  hanfei
 * 	[OT 8067] verify required print files existed for reconstruction and
 * 	obtain user confirmation if reconstruct on default database.
 * 	[1994/03/04  19:23:22  hanfei]
 * 
 * Revision 1.1.7.1  1994/02/04  21:23:28  hanfei
 * 	add -sort option for -print.  default now for pring pgo's is not to sort.
 * 
 * 	add -sort option for -print.  default now is not to sort.
 * 	Revision 1.1.5.2  1993/06/24  20:20:57  hinman
 * 	[hinman] - Make include of sys/types.h available to all
 * 	[1993/06/22  11:45:53  hinman]
 * 
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:39:16  hinman]
 * 
 * Revision 1.1.3.6  1993/04/02  18:38:58  hanfei
 * 	fixed a bad parameter name for #ifdef _STDC_
 * 	[1993/04/02  18:38:01  hanfei]
 * 
 * Revision 1.1.3.5  1993/04/01  20:20:52  hanfei
 * 	when directory for -prtpath and -dbpath is not existed, create it.
 * 	[1993/04/01  20:19:35  hanfei]
 * 
 * 	correct error of error reporting for -prtpath and -dbpath pathname
 * 	if they don't exist.
 * 	[1993/03/31  20:28:24  hanfei]
 * 
 * Revision 1.1.3.4  1993/03/30  22:10:16  hanfei
 * 	authentication keys' default pepper values are printed/reconstructed.
 * 	[1993/03/30  22:06:14  hanfei]
 * 
 * Revision 1.1.3.3  1993/03/23  23:02:24  hanfei
 * 	add codes for salvager reconstruct
 * 	[1993/03/23  22:47:25  hanfei]
 * 
 * Revision 1.1.3.2  1993/03/05  18:32:11  frisco
 * 	initial salvager functionality
 * 	[1993/03/02  21:21:47  frisco]
 * 
 * Revision 1.1.1.2  1992/11/25  21:11:07  frisco
 * 	Initial Revision
 * 
 * $EndLog$
 */

/*  
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
*/

/* 
**      Rgy Salvager Tool
*/

#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <locale.h>
#include <dce/dce.h>
#include <macros.h>
#include <stdio.h>
#include <rsdb.h>  
#include <rsdb_create.h>  
#include <rsdb_util.h>  
#include <rsdb_acct.h>  
#include <rsdb_acl.h>  
#include <rsdb_file.h>  
#include <rsdb_auth.h>  
#include <rsdb_policy.h>  
#include <sec_svg.h>
#include <sec_svg_prt.h>
#include <sec_svg_util.h>

#include <dce/dce_msg.h>
#include <dce/dcesadmsg.h>

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>

/* Define audit trail location */
static char *_sec_audit_path = NULL;
#define _SEC_AUDIT_TRAIL_PATH \
    STRING_CONCAT(_sec_audit_path, DCELOCAL_PATH, "/var/security/sec_audit_trail", "")

dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */

#define		NO_ACL   	FALSE
#define		ONLY_ACL	TRUE

PRIVATE boolean32 print = FALSE;
PRIVATE boolean32 reconstruct = FALSE;
PRIVATE boolean32 check = FALSE;
PRIVATE boolean32 fix = FALSE;
 
PRIVATE boolean32 do_principals = TRUE;
PRIVATE boolean32 do_groups = TRUE;
PRIVATE boolean32 do_orgs = TRUE;
PRIVATE boolean32 do_accts = TRUE;
PRIVATE boolean32 do_acls = TRUE;
PRIVATE boolean32 do_policies = TRUE;
PRIVATE boolean32 do_state = TRUE;
PRIVATE boolean32 do_replicas = TRUE;
PRIVATE boolean32 do_mkey = TRUE;
PRIVATE boolean32 do_pgo = TRUE;
PRIVATE boolean32 do_attr_schema = TRUE;

PUBLIC boolean32 verbose = FALSE;
PUBLIC boolean32 sort = FALSE;
PUBLIC boolean32 sec_svg_dce103 = FALSE;
/* get rid of bypass_auth when gone from OSF source */
PUBLIC boolean32 rs_bypass_auth = FALSE;
/*rsdb_name_key_t  root_name_keys[sec_rgy_domain_last];*/
PUBLIC char salvager[] = "salvager";


PRIVATE void normalize_path
#ifndef __STDC__
    (path)
    char *path;
#else
  (
    char *path
  )
#endif
{
    int len;

    len = strlen(path);
    if (path[len - 1] != '/')
        strcat(path, "/");

}

PRIVATE void domain_name_init
#ifndef __STDC__
    ()
#else
  (
  )
#endif
{
    sec_rgy_domain_t    domain;
    long            name_key_len = sizeof(rsdb_name_key_t);

    /* keep name of top-level directory for principal/org/group */
    for(domain = sec_rgy_domain_person; domain < sec_rgy_domain_last;
        (domain)++) {
        rsdb_util_get_key_of_id(domain, START_OF_LIST,
			    (Pointer)&root_name_keys[domain], &name_key_len);
    }
}

PRIVATE void db_init
#ifndef __STDC__
    (status)
    error_status_t *status;
#else
  (
    error_status_t *status
  )
#endif
{

    struct stat *buf;

    if (rsdb_file_stat((char *) db_path, buf) == 0) {
        fprintf(stderr,
"Unable to open registry data.\n\
Discovered directory '%s',\n\
this is an indication that a previous run failed.  Remove the '.bak'\n\
extension from the directory name and rerun.\n", db_path);
        return;
    }

    rs_state_init(status);
    if (BAD_STATUS(status)) return;

    rs_log_initialize(status);
    if (BAD_STATUS(status)) return;

    rsdb_load_init(rs_state.rep_state, status);
    if (BAD_STATUS(status)) return;

}

PRIVATE void svg_mkdir
#ifndef __STDC__
    (dirname)
    char *dirname;
#else
  (
     char *dirname
  )
#endif
{
    char *p;
    struct stat dstat;

    p = dirname;
    if (*p == '/' ) /* skip root */
        p++;
    while ( true )
    { /* for each directory level, do ... */
        p= strpbrk(p, "/");
	if ( p != NULL ) 
	    *p = '\0';	
	if (stat((char *)dirname, &dstat) == -1 ) {
	    if ( mkdir(dirname, S_IRUSR+S_IWUSR+S_IXUSR) == -1 )
	        perror(dirname);
	}
	if ( p != NULL)
	    *p = '/';
	else /* last level */
	    break;
	p++;
    }
}

PRIVATE void falsify_db_opts
#ifndef __STDC__
    ()
#else
  (void)
#endif
{
    do_principals = FALSE;
    do_groups = FALSE;
    do_orgs = FALSE;
    do_accts = FALSE;
    do_acls = FALSE;
    do_policies = FALSE;
    do_state = FALSE;
    do_replicas = FALSE;
    do_mkey = FALSE;
    do_attr_schema = FALSE;
}

PRIVATE void do_print
#ifndef __STDC__
    ()
#else
  (void)
#endif
{
    error_status_t status;

    CLEAR_STATUS(&status);

    db_init(&status);
    if (BAD_STATUS(&status)) {
        error_st(status, "Cannot initialize rgy database");
	return;
    }

    if (status == error_status_ok) {

        print_init(&status);
	if (BAD_STATUS(&status)) {
	    error_st(status, "Cannot initialize for print");
	    return;
	}

        if ( sec_svg_dce103 == TRUE ) {
	    print_open_info_file();
        }

	domain_name_init();

        if (do_principals) {
            print_pgo_domain(sec_rgy_domain_person, &status);
	    if (BAD_STATUS(&status)) {
	        error_st(status, "Cannot print rgy principal files");
		return;
	    }
	}
        if (do_groups) {
            print_pgo_domain(sec_rgy_domain_group, &status);
	    if (BAD_STATUS(&status)) {
	        error_st(status, "Cannot print rgy group files");
		return;
	    }
	}
        if (do_orgs) {
            print_pgo_domain(sec_rgy_domain_org, &status);
	    if (BAD_STATUS(&status)) {
	        error_st(status, "Cannot print rgy org files");
		return;
	    }
	}
        if (do_attr_schema) {
            print_attr_schema(&status);
	    if (BAD_STATUS(&status)) {
	        error_st(status, "Cannot print attribute schema files");
		return;
	    }
	}
        if (do_policies) {
            print_policies(&status);
	    if (BAD_STATUS(&status)) {
	        error_st(status, "Cannot print rgy policy files");
		return;
	    }
	}
        if (do_state) {
            print_rgy_state(&status);
	    if (BAD_STATUS(&status)) {
	        error_st(status, "Cannot print rgy state files");
		return;
	    }
	}
        if (do_replicas) {
            print_replicas(&status);
	    if (BAD_STATUS(&status)) {
	        error_st(status, "Cannot print rgy replicas files");
		return;
	    }
	}
        if (do_principals) {
            print_mkey(&status);
	    if (BAD_STATUS(&status)) {
	        error_st(status, "Cannot print rgy mkey files");
		return;
	    }
	}

        if ( sec_svg_dce103 == TRUE ) {
	    print_close_info_file();
    	}
    
    }
    else {
        fprintf(stderr, "Unable to load database in directory %s\n", db_path);
        return;
    }

}

PRIVATE void do_reconstruct
#ifndef __STDC__
    ()
#else
  (void)
#endif
{
    error_status_t status;

    construct_version_check(&status);
    if (BAD_STATUS(&status)) {
	error_st(status, "Cannot reconstruct");
	return;
    }

    construct_init(&status);
    if (BAD_STATUS(&status)) {
        error_st(status, "Cannot initialize for reconstruct");
	return;
    }

    domain_name_init();

    if (do_attr_schema) {
        construct_attr_schema(NO_ACL, &status);
	if (BAD_STATUS(&status)) {
	    error_st(status, "Cannot reconstruct attribute schema");
	    return;
	}
    }

    /* has to construct policy before construct master key */
    /* so rgy_info fields for master keys are initialized */
    construct_policies(NO_ACL, &status);
    if (BAD_STATUS(&status)) {
        error_st(status, "Cannot reconstruct policies");
	return;
    }

    if (do_state) {
        construct_rgy_state(&status);
	if (BAD_STATUS(&status)) {
	    error_st(status, "Cannot reconstruct state");
	    return;
	}
    }
    if (do_pgo) {
        construct_mkey(&status);
	if (BAD_STATUS(&status)) {
	    error_st(status, "Cannot reconstruct mkey");
	    return;
	}
    }
    construct_replicas(NO_ACL, &status);
    if (BAD_STATUS(&status)) {
        error_st(status, "Cannot reconstruct replicas");
	return;
    }

    if (do_orgs) {
        construct_pgo_domain(sec_rgy_domain_org, NO_ACL, &status);
	if (BAD_STATUS(&status)) {
	    error_st(status, "Cannot reconstruct org");
	    return;
	}
    }
    if (do_groups) {
        construct_pgo_domain(sec_rgy_domain_group, NO_ACL, &status);
	if (BAD_STATUS(&status)) {
	    error_st(status, "Cannot reconstruct group");
	    return;
	}
    }
    if (do_principals) {
        construct_pgo_domain(sec_rgy_domain_person, NO_ACL, &status);
	if (BAD_STATUS(&status)) {
	    error_st(status, "Cannot reconstruct principal");
	    return;
	}
    }

    construct_member(&status);
    if (BAD_STATUS(&status)) {
        error_st(status, "Cannot reconstruct member");
	return;
    }

    if (do_attr_schema) {
        construct_attr_schema(ONLY_ACL, &status);
	if (BAD_STATUS(&status)) {
	    error_st(status, "Cannot reconstruct attribute schema acl");
	    return;
	}
    }

    construct_policies(ONLY_ACL, &status);
    if (BAD_STATUS(&status)) {
        error_st(status, "Cannot reconstruct policies acl");
	return;
    }

    construct_replicas(ONLY_ACL, &status);
    if (BAD_STATUS(&status)) {
        error_st(status, "Cannot reconstruct replicas acl");
        return;
    }

    if (do_orgs) {
        construct_pgo_domain(sec_rgy_domain_org, ONLY_ACL, &status);
	if (BAD_STATUS(&status)) {
	    error_st(status, "Cannot reconstruct org acl");
	    return;
	}
    }
    if (do_groups) {
        construct_pgo_domain(sec_rgy_domain_group, ONLY_ACL, &status);
	if (BAD_STATUS(&status)) {
	    error_st(status, "Cannot reconstruct group acl");
	    return;
	}
    }
    if (do_principals) {
        construct_pgo_domain(sec_rgy_domain_person, ONLY_ACL, &status);
	if (BAD_STATUS(&status)) {
	    error_st(status, "Cannot reconstruct principal acl");
	    return;
	}
    }

    clean_princ_list();

    construct_save(&status);
    if (BAD_STATUS(&status)) {
        error_st(status, "Cannot do reconstruct save");
	return;
    }

}

PRIVATE void do_check
#ifndef __STDC__
    ()
#else
  (void)
#endif
{

    fprintf(stderr, "Check not yet implemented\n");

/*  NOT YET IMPLEMENTED
    if (do_principals)
        check_principals(&status);
    if (do_groups)
        check_groups(&status);
    if (do_orgs)
        check_orgs(&status);
    if (do_accts)
        check_accts(&status);
    if (do_acls)
        check_acls(&status);
    if (do_policies)
        check_policies(&status);
    if (do_state)
        check_rgy_state(&status);
    if (do_replicas)
        check_replicas(&status);
*/

}

PRIVATE void do_fix
#ifndef __STDC__
    ()
#else
  (void)
#endif
{

    fprintf(stderr, "Fix not yet implemented\n");

/*  NOT YET IMPLEMENTED
    if (do_principals)
        fix_principals(&status);
    if (do_groups)
        fix_groups(&status);
    if (do_orgs)
        fix_orgs(&status);
    if (do_accts)
        fix_accts(&status);
    if (do_acls)
        fix_acls(&status);
    if (do_policies)
        fix_policies(&status);
    if (do_state)
        fix_rgy_state(&status);
    if (do_replicas)
        fix_replicas(&status);
*/

}

PRIVATE void process_args
#ifndef __STDC__
    (argc, argv) 
    int     argc;
    char   *argv[];
#else
  (
    int     argc,
    char   *argv[]
  )
#endif
{
    boolean32 db_opt = FALSE;
    int i;
    boolean32 err = false;
    boolean32 dbpath_at_default = TRUE;
    char   response[80];
    struct stat dstat;

    for (i = 1; i < argc; i++) { 
        if (strcmp(argv[i], "-dbpath") == 0) {
            i++;
            strcpy((char *) db_base_path, argv[i]);
	    if (stat((char *)db_base_path, &dstat) == -1 ) {
	        svg_mkdir((char *)db_base_path);
	    }
            normalize_path((char *) db_base_path);
            strcpy((char *) db_path, (char *) db_base_path);
            strcat((char *) db_path, DB_DIRECTORY);
            rsdb_file_set_base_path((char *) db_path);
            rsdb_file_mkey_set_base_path((char *) db_base_path);
	    dbpath_at_default = FALSE;
        }
        else if(strcmp(argv[i], "-prtpath") == 0) {
            i++;
            strcpy((char *) prt_base_path, argv[i]);
	    if (stat((char *)prt_base_path, &dstat) == -1 ) {
	        svg_mkdir((char *)prt_base_path);
	    }
            normalize_path((char *) prt_base_path);
            strcpy((char *) prt_path, (char *) prt_base_path);
            strcat((char *) prt_path, PRT_DIRECTORY);
        }
        else if(strcmp(argv[i], "-verbose") == 0) {
            verbose = TRUE;
        }
        else if(strcmp(argv[i], "-sort") == 0) {
            sort = TRUE;
        }
        else if(strcmp(argv[i], "-dce1.0.3") == 0) {
            sec_svg_dce103 = TRUE;
        }
        else if(strcmp(argv[i], "-print") == 0) {
            print = TRUE;
        }
        else if(strcmp(argv[i], "-reconstruct") == 0) {
            reconstruct = TRUE;
        }
        else if(strcmp(argv[i], "-check") == 0) {
            check = TRUE;
        }
        else if(strcmp(argv[i], "-fix") == 0) {
            fix = TRUE;
        }
        else if(strcmp(argv[i], "-princ") == 0) {
            if (!db_opt) {
                db_opt = TRUE;
                falsify_db_opts();
            }
            do_principals = TRUE;
        }
        else if(strcmp(argv[i], "-group") == 0) {
            if (!db_opt) {
                db_opt = TRUE;
                falsify_db_opts();
            }
            do_groups = TRUE;
        }
        else if(strcmp(argv[i], "-org") == 0) {
            if (!db_opt) {
                db_opt = TRUE;
                falsify_db_opts();
            }
            do_orgs = TRUE;
        }
        else if(strcmp(argv[i], "-attr_schema") == 0) {
            if (!db_opt) {
                db_opt = TRUE;
                falsify_db_opts();
            }
            do_attr_schema = TRUE;
        }
        else if(strcmp(argv[i], "-acct") == 0) {
            if (!db_opt) {
                db_opt = TRUE;
                falsify_db_opts();
            }
            do_accts = TRUE;
        }
        else if(strcmp(argv[i], "-acl") == 0) {
            if (!db_opt) {
                db_opt = TRUE;
                falsify_db_opts();
            }
            do_acls = TRUE;
        }
        else if(strcmp(argv[i], "-policy") == 0) {
            if (!db_opt) {
                db_opt = TRUE;
                falsify_db_opts();
            }
            do_policies = TRUE;
        }
        else if(strcmp(argv[i], "-state") == 0) {
            if (!db_opt) {
                db_opt = TRUE;
                falsify_db_opts();
            }
            do_state = TRUE;
        }
        else if(strcmp(argv[i], "-replicas") == 0) {
            if (!db_opt) {
                db_opt = TRUE;
                falsify_db_opts();
            }
            do_replicas = TRUE;
        }
        else if(strcmp(argv[i], "-mkey") == 0) {
            if (!db_opt) {
                db_opt = TRUE;
                falsify_db_opts();
            }
            do_mkey = TRUE;
        }
        else {
            fprintf(stderr, "Bad Option: %s\n", argv[i]);
            err = TRUE;
            break;
        }

    }
    if (reconstruct==TRUE) {
	if ( (do_policies!=TRUE) || 
	(do_state!=TRUE) || (do_replicas!=TRUE) ||
	(do_principals!=TRUE) || (do_groups!=TRUE) || 
	(do_attr_schema!=TRUE) || 
	(do_orgs!=TRUE) || (do_accts!=TRUE) ||
	(do_acls!=TRUE) || (do_mkey!=TRUE) ) 
	/* can only reconstruct for all, because of dependency */
	    err = TRUE;
	else { 
	/* verify all print files existed */
	    if (sec_svg_exist_prtfiles() != TRUE)
		exit(1);
        }
    }
    if (!print && !reconstruct && !check && !fix)
        err = TRUE;
    if (err) {
        fprintf(stderr, "\nUsage:\n");
        fprintf(stderr, "\n%s -check [-dbpath db_pathname]", argv[0]);
        fprintf(stderr, " [db_options] [-verbose]\n");
        fprintf(stderr, "\n%s -fix [-dbpath db_pathname]", argv[0]);
        fprintf(stderr, "[db_options] [-query] [-verbose]\n");
        fprintf(stderr, "\n%s -print [-dbpath db_pathname]", argv[0]);
        fprintf(stderr, "[-prtpath print_pathname] [db_options] [-verbose] [-sort] [-dce1.0.3]\n");
        fprintf(stderr, "\n%s -reconstruct [-dbpath db_pathname]", argv[0]);
        fprintf(stderr, "[-prtpath print_pathname] [-verbose]\n");
        exit(1);
    }

    /* no error, need to verify with user if they want to overwrite */
    /* default rgy_data files */
    if (reconstruct && dbpath_at_default) {
	fprintf(stdout, "Will overwrite data at %s,\n", db_path);
        fprintf(stdout,
          "do you wish to continue (y[es]) or abort %s (n[o])? ", argv[0]);
        fflush(stdout);
	fscanf(stdin, "%s", &response); 
	if ((strcmp(response,"yes") == 0) || (strcmp(response,"y")==0))
	    ;
	else if ((strcmp(response,"no") == 0) || (strcmp(response,"n")==0)) 
	         exit(1);
	     else {
	         fprintf(stdout, "Invalid answer: %s, should be either y[es] or n[o]\n", response);
		 exit(1);
	     }
    }

}

PRIVATE void salvage_init
#ifndef __STDC__
    () 
#else
    (void)
#endif
{

    /* set default database pathname */
    strcpy((char *) db_base_path, DB_BASE_DIRECTORY);
    strcpy((char *) db_path, (char *) db_base_path);
    strcat((char *) db_path, DB_DIRECTORY);
    rsdb_file_set_base_path((char *) db_path);
    rsdb_file_mkey_set_base_path((char *) db_base_path);

    /* set default print pathname */
    prt_base_path[0] = '\0';
    strcpy((char *) prt_path, PRT_DIRECTORY);


}


/*
 *  MAIN
 */
int main
#ifndef __STDC__
    (argc, argv) 
    int     argc;
    char   *argv[];
#else
  (
    int     argc,
    char   *argv[]
  )
#endif
{
    int unix_id = 0;
    error_status_t              st;

#if defined(DCE_AUD_POINTS)
       error_status_t              aud_status;
#endif

    setlocale(LC_ALL, "");
    dce_svc_set_progname("sec_salvage_db", &st);
    dce_msg_define_msg_table(sad__table,
        sizeof sad__table / sizeof sad__table[0], &st);

#if defined(DCE_AUD_POINTS)
    dce_aud_open(aud_c_trl_open_write, (char *)_SEC_AUDIT_TRAIL_PATH,
        SEC_AUD_FIRST_EVT_NUMBER, SEC_AUD_NUM_OF_EVTS, 
        &sec_aud_trail, &aud_status);
     if (BAD_STATUS(&aud_status))
        error_st(aud_status, "Cannot open audit trail");
#endif	/* defined(DCE_AUD_POINTS) */
    
    rs_verbose = false; 
    salvage_init();
    process_args(argc, argv);

#ifndef DEBUG
    unix_id = getuid();
#endif

    /* must be root to run sec_salvage_db */

    if (unix_id == 0) {
        if (print)
            do_print();
        if (reconstruct)
            do_reconstruct();
        if (check)
            do_check();
        if (fix)
            do_fix();
    }
    else
        fprintf(stderr, "Must be root to run sec_salvage_db\n");

}
