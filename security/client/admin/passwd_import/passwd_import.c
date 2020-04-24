/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: passwd_import.c,v $
 * Revision 1.1.12.2  1996/03/11  13:26:21  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:22  marty]
 *
 * Revision 1.1.12.1  1995/12/08  17:56:44  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/10/30  21:29 UTC  bartf
 * 	merge fix for uninit fullname, verified group looping works
 * 
 * 	fix uninitialized pgo_entry.fullname
 * 	[1995/10/30  21:21 UTC  bartf  /main/HPDCE02/bartf_sec_90/2]
 * 
 * 	fix looping on group name bug
 * 	[1995/10/26  19:36 UTC  bartf  /main/HPDCE02/bartf_sec_90/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/30  23:19 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:33 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/20  13:59 UTC  mdf
 * 	Fix defect
 * 
 * 	HP revision /main/HPDCE02/mdf_MOTHRA_1/1  1995/01/20  13:11 UTC  mdf
 * 	Changes for CHFts14053
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:21 UTC  mullan_s
 * 	Merge.
 * 	[1995/12/08  17:25:55  root]
 * 
 * Revision 1.1.9.3  1994/08/04  16:12:28  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/28  11:41:09  mdf]
 * 
 * Revision 1.1.9.2  1994/04/01  20:08:16  jd
 * 	Undefine _NO_PROTO  so the cma prototypes don;t complain. Tis presumably
 * 	won't need to be undef'd when the security cleanup happens.
 * 	[1994/03/28  20:25:59  jd]
 * 
 * Revision 1.1.9.1  1994/03/25  17:29:30  sekhar
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 	[1994/03/25  17:28:10  sekhar]
 * 
 * Revision 1.1.6.1  1993/10/05  22:29:45  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:03:05  mccann]
 * 
 * 	HP revision /main/GAMERA/KK/ODESSA_2/hanfei_svc_bl1/2  1994/07/12  18:25 UTC  hanfei
 * 	change isascii() to isdcepcs().
 * 
 * 	HP revision /main/GAMERA/KK/ODESSA_2/hanfei_svc_bl1/1  1994/06/30  17:43 UTC  hanfei
 * 	servicibility conversion.
 * 
 * 	HP revision /main/GAMERA/KK/ODESSA_2/1  1994/06/02  19:40 UTC  sekhar
 * 	Merge SVC changes back from OSF.
 * 
 * 	HP revision /main/GAMERA/KK/sekhar_svc/1  1994/05/13  19:44 UTC  sekhar
 * 	SVC changes. Merge from OSF DCE1.1 source tree.
 * 
 * Revision 1.1.5.2  1993/07/08  10:51:44  root
 * 	Initial King Kong branch
 * 	[1993/07/08  10:51:07  root]
 * 
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:00:48  tom]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:33:56  zeliff]
 * 
 * Revision 1.1.3.2  1993/05/25  16:31:36  rps
 * 	standardize version string
 * 	[1993/05/25  16:21:48  rps]
 * 
 * 	Finish -p and -o functionality, clean up verbose output.
 * 	[1992/05/07  19:14:48  burati]
 * 
 * 	Added copyright header
 * 	[1992/04/23  01:49:17  burati]
 * 
 * 	Initial version.
 * 	[1992/04/23  01:22:40  burati]
 * 
 * Revision 1.1.1.10  1993/05/04  20:22:37  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
*/

#include <dce/dce.h>
#undef _NO_PROTO
#include <dce/rpcbase.h>
#include <dce/binding.h>
#include <dce/uuid.h>
#include <dce/passwd.h>
#include <dce/acct.h>
#include <dce/misc.h>
#include <dce/pgo.h>
#include <dce/policy.h>
#include <dce/dce_error.h>
#include <dce/keymgmt.h>
#include <dce/sec_login.h>

#include <com_err.h>
#include <krb5/krb5.h>
#include <krb5/los-proto.h>
#include <macros.h>
#include <args.h>
#include <dce/dce_msg.h>
#include <dce/dcesadmsg.h>
#include <un_io.h>
#include <sec_def_flags.h>
#include <passwd_import.h>
#include <passwd_import_subs.h>

#include <sys/time.h>
#include <pi_pwd.h>
#include <pi_grp.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

/* Private state information, used in so many places
 * that they were made global for efficiency reasons.
 */
static  boolean32   verbose = false;
static  int         max_unix_num = 65535;
static  uuid_t      uuid_nil;

/*** Forward Declarations ***/

PRIVATE void err_exit(
    error_status_t  status
);

PRIVATE boolean32 setup_etc(
    char        *dir
);

PRIVATE boolean32 setup_rgy(
    sec_rgy_name_t      u_name,
    char                *pwd,
    sec_rgy_handle_t    *context_p
);

PRIVATE boolean32 is_number(
    char    *int_str,
    long     len
);

PRIVATE  boolean32 is_special_name_char(
    int  c
);

PRIVATE void chk_pgo_name(
    sec_rgy_domain_t    domain,
    char                *name,
    error_status_t      *st
);

PRIVATE void trim(
    char *str
);

boolean32 get_string(
    char        *buff,
    long         buffsize,
    boolean32   *overflow
);

boolean32 get_arg(
    char        *buff,
    long        buffsize,
    boolean32   *overflow
);

PRIVATE void read_line(
    char  *prompt,
    char  *buff,
    long  buffsize
);

void get_pgo_name(
    sec_rgy_domain_t    domain,
    char                *prompt,
    sec_rgy_name_t      new_name
);

PRIVATE make_lower_case(
   char *buf
);

PRIVATE boolean32 ask_y_n(
    unsigned32  question_msgid
);

PRIVATE void check_org(
    char                *org,
    sec_rgy_handle_t    context,
    error_status_t      *st_p
);

PRIVATE void do_name_conflict(
    sec_rgy_domain_t    domain,
    sec_rgy_name_t      name,
    signed32            unix_num,
    signed32            exist_num,
    boolean32           *done_p,
    boolean32           identical,
    boolean32           check_only
);

PRIVATE boolean32 chk_num(
    signed32  num
);

PRIVATE void do_num_conflict(
    sec_rgy_domain_t    domain,
    sec_rgy_pgo_item_t  *item_p,
    boolean32           *alias_p,
    boolean32           *done_p,
    boolean32           check_only
);

PRIVATE boolean32 add_pgo(
    char                *name,
    sec_rgy_pgo_item_t  *item_p,
    sec_rgy_domain_t    domain,
    sec_rgy_handle_t    context,
    boolean32           identical,
    boolean32           check_only
);

PRIVATE error_status_t process_groups(
    boolean32           ign_ident,
    boolean32           check_only,
    sec_rgy_handle_t    context,
    int                 *num_groups_p
);

PRIVATE void close_etc(
    void
);

PRIVATE void close_rgy(
    sec_rgy_handle_t    *context_p
);

PRIVATE boolean32 add_org_member(
    sec_rgy_handle_t    context,
    unsigned_char_p_t   p_name,
    unsigned_char_p_t   org,
    error_status_t      *st_p
);

PRIVATE void add_acct(
    sec_rgy_handle_t        context,
    sec_rgy_name_t          p_name,
    sec_rgy_name_t          g_name,
    sec_rgy_name_t          o_name,
    struct passwd           *pwent_p,
    sec_rgy_acct_admin_t    *admin_part,
    sec_rgy_acct_user_t     *user_part,
    sec_passwd_rec_t        *caller_key,
    sec_passwd_rec_t        *new_key
);

PRIVATE boolean32 cobble_up_group_ent(
    sec_rgy_handle_t    context,
    signed32            gid,
    sec_rgy_name_t      g_name,
    boolean32           ignore_ident,
    boolean32           check_only
);

PRIVATE boolean32 add_group_member(
    sec_rgy_handle_t    context,
    sec_rgy_name_t      p_name,
    sec_rgy_name_t      g_name,
    signed32            gid,
    boolean32           check_only,
    boolean32           ignore_ident,
    error_status_t      *st_p
);

PRIVATE void add_members(
    sec_rgy_handle_t    context,
    boolean32           check_only,
    boolean32           ignore_ident
);

PRIVATE void process_principals_accounts(
    boolean32           ign_ident,
    boolean32           check_only,
    sec_rgy_handle_t    context,
    char                *org,
    char                *caller_pkey,
    int                 *num_princs_p,
    error_status_t      *st_p
);
#define VERBOSE_MESSAGE(msg) \
    if (verbose) { \
        print_verbose_message(msg); \
    }
 
/*
 * print_verbose_message
 */
PRIVATE void print_verbose_message
  (
    error_status_t  msg_code
  )
{
    dce_error_string_t  error_text;
    int                 lst;

    dce_error_inq_text(msg_code, error_text, &lst);

    if (GOOD_STATUS(&lst)) {
        fprintf(stderr, "%s\n", error_text);
    } else {
        fprintf(stderr, "Unknown informational message: %x\n", msg_code);
    }
}


/*
 * err_exit - display error message, exit
 */
PRIVATE void err_exit
  (
    error_status_t  status
  )
{
    dce_pgm_fprintf(stderr, status);
    dce_pgm_fprintf(stderr, pwd_imp_err_fatal);
/*    show_tally;
    imp_cleanup;           /* release allocated storage */

    exit(1);
}


/*
 * setup_etc - Open files to import
 */
PRIVATE boolean32 setup_etc
  (
    char        *dir
  )
{ 
    char            pw_name[MAXPATHLEN], gr_name[MAXPATHLEN];
    struct stat     buff;
    error_status_t  st;

        /* first construct the full pathnames for the files */
    VERBOSE_MESSAGE(pwd_imp_inf_prepare_import);

        /* Don't even try to import from a pathname the OS can't handle */
    if ( (strlen(dir)+pw_leaf_len) > MAXPATHLEN ) {
        dce_pgm_fprintf(stderr, pwd_imp_err_path_too_long, dir);
        return false;
    }

    strcpy(pw_name, dir);                                   /* passwd file */
    strcat(pw_name, pw_leaf);
    if (stat(pw_name, &buff) < 0) {
        dce_pgm_fprintf(stderr, pwd_imp_err_no_pwd_access, pw_name, 
			strerror(errno));
        return false;
    }
    pi_setpwfile(pw_name);

    strcpy((char *)gr_name, dir);                           /* group file */
    strcat(gr_name, gr_leaf);
    if (stat(gr_name, &buff) < 0) {
        pi_endpwent();
        dce_pgm_fprintf(stderr, pwd_imp_err_no_grp_access, gr_name, 
			strerror(errno));
        return false;
    }
    pi_setgrfile(gr_name);
    pi_setgrent();
    return true;
} /* setup_etc */


/*
 * setup_rgy -  Initialize registry information
 *              Stop and return false if any auth step fails.
 */
PRIVATE boolean32 setup_rgy
  (
    sec_rgy_name_t      u_name,
    char                *pwd,
    sec_rgy_handle_t    *context_p
  )
{
    error_status_t          st;
    char                    tmp_pwd[sec_passwd_str_t_size];
    sec_rgy_properties_t    properties;
    sec_passwd_rec_t        passwd;
    sec_login_auth_src_t    auth_src;
    sec_login_handle_t      login_context;
    boolean32               reset;
    VERBOSE_MESSAGE(pwd_imp_inf_prepare_rgy);

    strcpy(tmp_pwd, pwd);   /* Copy it since validate zeros it */
    passwd.version_number = sec_passwd_c_version_none;
    passwd.pepper = (unsigned_char_p_t)0;
    passwd.key.key_type = sec_passwd_plain;
    passwd.key.tagged_union.plain = (unsigned_char_p_t)tmp_pwd;

        /* Don't proceed any further if any auth step fails */
    if (! sec_login_setup_identity(u_name, sec_login_no_flags,
                                    &login_context, &st)) {
	dce_pgm_fprintf(stderr, st);
	dce_pgm_fprintf(stderr, pwd_imp_err_auth);
        return false;
    }
    if (! sec_login_validate_identity(login_context, &passwd,
                                        &reset, &auth_src, &st)) {
	dce_pgm_fprintf(stderr, st);
	dce_pgm_fprintf(stderr, pwd_imp_err_auth);
        return false;
    }
    sec_login_set_context(login_context, &st);
    if (BAD_STATUS(&st)) {
	dce_pgm_fprintf(stderr, st);
	dce_pgm_fprintf(stderr, pwd_imp_err_auth);
        return false;
    }

    sec_rgy_site_open_update((unsigned_char_p_t)"/.:", context_p, &st);
    if (BAD_STATUS(&st)) {
	dce_pgm_fprintf(stderr, st);
        return false;
    }

    sec_rgy_properties_get_info(*context_p, &properties, &st);
    if (GOOD_STATUS(&st)) {
        max_unix_num = properties.max_unix_id;
    }

    return true;
} /* setup_rgy */


/*
** FUNCTION is_number
**
** checks that all characters in a string are digits .
**/
PRIVATE boolean32 is_number
  (
    char    *int_str,
    long    len
  )
{
    boolean32   all_digits;
    int         cur_digit;

    all_digits = true;
    cur_digit = 0;       /* start with first character in int_str */

   if ((*int_str == '-') || (*int_str == '+'))
        cur_digit++;     /* skip leading sign */

    for (; cur_digit < len && all_digits; cur_digit++) 
        if (!isdcepcs(int_str[cur_digit]))
            all_digits = false;
        else if (!isdigit(int_str[cur_digit]))
            all_digits = false;

    return all_digits;
}


/*
 * is_special_name_char
 */
PRIVATE  boolean32 is_special_name_char
  (
    int  c
  )
{
    static  char illegal_chars[] = {':',   /* colon would screw up passwd file */
                                    '@'
                                   };
    char   *cp;
    int    i;

    for (i = 0, cp = illegal_chars; i < sizeof(illegal_chars); i++, cp++) {
        if ((char)c == *cp) {
            return false;
        }
    }
    return true;
}


/*
 * chk_pgo_name - routine to check pgo_name for validity
 */
PRIVATE void chk_pgo_name
  (
    sec_rgy_domain_t    domain,
    char                *name,
    error_status_t      *st
  )
{
    int i, len;
    CLEAR_STATUS(st);

    if ((len = strlen(name)) > sec_rgy_name_max_len) {
        switch(domain) {
            case sec_rgy_domain_person:
                SET_STATUS(st, pwd_imp_err_pname_too_long);
                break;
            case sec_rgy_domain_group:
                SET_STATUS(st, pwd_imp_err_gname_too_long);
                break;
            case sec_rgy_domain_org:
                SET_STATUS(st, pwd_imp_err_oname_too_long);
                break;
            default:
                SET_STATUS(st, pwd_imp_err_internal_error);
                break;
        }
    }

    if (GOOD_STATUS(st)) {
        for (i = 1; i < len && GOOD_STATUS(st); i++) {
            if ( !(isdcepcs(name[i])) ||
                 (!( isalpha(name[i]) || isdigit(name[i]) ||
                        is_special_name_char((int) name[i]) )) ) {
                switch(domain) {
                    case sec_rgy_domain_person:
                        SET_STATUS(st, pwd_imp_err_pname_too_long);
                        break;
                    case sec_rgy_domain_group:
                        SET_STATUS(st, pwd_imp_err_gname_too_long);
                        break;
                    case sec_rgy_domain_org:
                        SET_STATUS(st, pwd_imp_err_oname_too_long);
                        break;
                    default:
                        SET_STATUS(st, pwd_imp_err_internal_error);
                        break;
                }   /* switch */
            }       /* if  */
        }           /* for */
    }               /* if  */
    return;
}  /* chk_pgo_name */


/* 
 * trim - Trims leading and trailing whitespace from a string
 */
PRIVATE void trim
  (
    char *str
  )
{
    char *nonwhite;

    if (*str == '\0') 
        return;

    nonwhite = str;
        /* skip to first non-white character */
    while (isspace(*nonwhite))
        nonwhite++;
        
    if (nonwhite != str) {              /* got some leading whitespace */
        if (*nonwhite == '\0') {
            *str = '\0';
            return;                    /* maybe the whole string was whitespace? */
        }
        while (*nonwhite != '\0') 
            *str++ = *nonwhite++;
        *str = '\0';
    } else {                           /* advance to end of str */
        str += strlen(str);
    }
    
        /* walk backwards through str trashing whitespace chars with a null */
    str--;
    while (isspace(*str)) 
        *str-- = '\0';
}

    
/*
 * get_string
 *
 * Just reads characters from standard input into a buffer, ignoring any that
 * would exceed the buffer size.  The buffer always returns null terminated, so 
 * pass in something at least 1 character longer than the maximum sensible response
 * you're willing to accept.  Returns false if the input is an immediate newline or
 * EOF,  true otherwise.   
 */
boolean32 get_string
  (
    char        *buff,
    long        buffsize,
    boolean32   *overflow
  )
{
    long     chars_read = 0;
    char     cur_char;
    char     *buff_ptr = buff;
    boolean32 ret_val;

    if (buffsize < 1)  return false;

    *overflow = false;

    do {
        cur_char = getchar();
        if (!((cur_char == EOF) || (cur_char == '\n'))) {
            if (chars_read < buffsize - 1) {
                *buff_ptr++ = cur_char;
            } else
                *overflow = true;
            chars_read++;
        }
    } while (!((cur_char == EOF) || (cur_char == '\n')));

    if (*buff == EOF || *buff == '\n')    /* check for immediate EOF or \n */
        ret_val = false;
    else
        ret_val = true;

    *buff_ptr = '\0';                     /* before we trash it with a null */

    return ret_val;
}


/* get_arg
 *  Like get_string, but can deal with quoted strings.  Returns false on
 * on any unquoted whitespace string, true otherwise.  If input data exceeds
 * the input buffer size, the overflow parameter returns true, otherwise it
 * returns false.
 */
boolean32 get_arg
  (
    char        *buff,
    long        buffsize,
    boolean32   *overflow
  )
{
    boolean32   ret_val;
    char       **argv;                  /* vector for multiple option args */
    int        argc;                    /* count of multiple option arguments */
    error_status_t  st;
    char       *arg_ptr;
    
    ret_val = get_string(buff, buffsize, overflow);
    
    if (ret_val && !(*overflow)) {
        /* treat non-quoted whitespace the same as  an immediate newline */
        trim(buff);
        if (*buff == '\0') return false;
        
        args_get(buff, &argc, &argv);
        if (argc == 0)
            *buff = '\0';
        else {
            arg_ptr = argv[0];
            while (*arg_ptr != '\0')
                *buff++ = *arg_ptr++;
            *buff = '\0';
        }
    }
    return ret_val;
}


/* read_line
 *  Get a line of input from stdin.  Keeps prompting if the user enters nothing
 * (nothing being defined as any unquoted stream of whitespace, including a nl),
 * or something that exceeds the length of the input buffer.
 */
PRIVATE void read_line
  (
    char  *prompt,
    char  *buff,
    long  buffsize
  )
{
    boolean32 null_line,
              overflow;
    do {
        dce_printf(pwd_imp_prompt_print, prompt);
        null_line = !get_arg(buff, buffsize, &overflow);
        if (!null_line && overflow) 
            dce_fprintf(stderr, pwd_imp_err_input_too_long);
    } while (null_line || overflow);

    return;
} /* read_line */


/* get_pgo_name
 *      Prompts for, accept, checks a new pgo name.  Does not
 *  return until an acceptable name is entered.
 */
void get_pgo_name
  (
    sec_rgy_domain_t    domain,
    char                *prompt,
    sec_rgy_name_t      new_name
  )
{
    boolean32       gotit = false;
    error_status_t  st;
    sec_rgy_name_t  buff;

    do {                            /* keep going until we get a good name */
        read_line(prompt, (char *)buff, sizeof(buff));
        chk_pgo_name(domain, (char *)buff, &st);    /* is name legal? */
        gotit = (GOOD_STATUS(&st));
        if (!gotit) { 
            dce_fprintf(stderr, st);
	    dce_fprintf(stderr, pwd_imp_err_bad_name, buff);
        }
    } while  (!gotit);

    strcpy((char *)new_name, (char *)buff);
    
    return;
} /* get_pgo_name */


/* ask_y_n
 * repeatedly asks the input question until y[es] or n[o] is
 * entered.  Returns true for yes, false for no.
 */
PRIVATE boolean32 ask_y_n
  (
    unsigned32  question_msgid
  )
{
    char        buff[128];
    int         gotit ;
    boolean32   answer;
    char        *question_str=NULL;

    question_str = (char *) dce_sprintf(question_msgid);
    gotit = false;

    do {
        read_line(question_str, buff, sizeof(buff));
	gotit = dce_rpmatch(buff);
	switch (gotit) {
	    case 1: 
		if ( question_str != NULL ) {
		    free(question_str);
		}
		return true;
                break;
	    case 0:
		if ( question_str != NULL ) {
		    free(question_str);
		}
		return false;
	        break;
	    default:
            	dce_pgm_fprintf(stderr, pwd_imp_err_bad_ask_y_n, buff);
		continue;
	}
    } while (gotit<0);

} /* ask_y_n */ 


/*
 * check_org - Make sure specified organization exists.
 */
PRIVATE void check_org
  (
    char                *org,
    sec_rgy_handle_t    context,
    error_status_t      *st_p
  )
{
    sec_rgy_pgo_item_t  tmp_item;
    sec_rgy_cursor_t    cursor;

    VERBOSE_MESSAGE(pwd_imp_inf_check_org);
    sec_rgy_cursor_reset(&cursor);
    sec_rgy_pgo_get_by_name(context, sec_rgy_domain_org,
                            (unsigned_char_p_t) org, &cursor,
                            &tmp_item, st_p);
}


/*
 * do_name_conflict -
 *      handle/resolve name conflict in specified domain.
 */
PRIVATE void do_name_conflict
  (
    sec_rgy_domain_t    domain,
    sec_rgy_name_t      name,
    signed32            unix_num,
    signed32            exist_num,
    boolean32           *done_p,
    boolean32           identical,
    boolean32           check_only
  )
{
    int                     lst;
    char		    *msg;
    char                    dom_name[10];
    boolean32               gotit;

    switch(domain) {
        case sec_rgy_domain_person:
            strcpy(dom_name, "Principal");
            break;
        case sec_rgy_domain_group:
            strcpy(dom_name, "Group");
            break;
        case sec_rgy_domain_org:
            strcpy(dom_name, "Org");
            break;
    } 

        /* Don't go any further if -c (check_only) mode specified */
    if (check_only) { 
        dce_fprintf(stderr, pwd_imp_inf_need_new_name);
        *done_p = true;
        return; 
    }
                
        /* Don't resolve name unless they want to */
    if (! ask_y_n(pwd_imp_qus_enter_conflict_action)) {
        *done_p = true;
        return;
    }
        /* Resolve conflict if we got this far. */
    msg = (char *) dce_sprintf(pwd_imp_msg_enter_new_name, dom_name, name, unix_num);
    get_pgo_name(domain, (char *)msg, name);
    free(msg);
    return;
} /* do_name_conflict */


/* chk_num
 *  Decode ascii unix_num & check for validity. Returns true if valid,
 * displays error message & returns false if invalid.
 */
PRIVATE boolean32 chk_num
  (
    signed32  num
  )
{
    boolean32 valid_num;

    if ((num > max_unix_num) || (num < 0)) {
        valid_num = false;
    } else 
        valid_num = true;

    return valid_num;
} /* chk_num */


/*
 * do_num_conflict -
 *      Handle/resolve UNIX num conflict in specified domain.
 */
PRIVATE void do_num_conflict
  (
    sec_rgy_domain_t    domain,
    sec_rgy_pgo_item_t  *item_p,
    boolean32           *alias_p,
    boolean32           *done_p,
    boolean32           check_only
  )
{
    int                     lst;
    signed32                new_num;
    char                    dom_name[10], token[10];
    char                    *msg_str=NULL;
    boolean32               gotit, choice;
    
    switch(domain) {
        case sec_rgy_domain_person:
            strcpy(dom_name, "Principal");
            break;
        case sec_rgy_domain_group:
            strcpy(dom_name, "Group");
            break;
        case sec_rgy_domain_org:
            strcpy(dom_name, "Org");
            break;
    } 
    
        /* Just inform them of conflict if in check mode */
    if (check_only) { 
        dce_fprintf(stderr, pwd_imp_inf_need_new_uid);
        return; 
    }

        /* Don't resolve name unless they want to */
    if (! ask_y_n(pwd_imp_qus_enter_conflict_action)) {
        *done_p = true;
        return;
    }

        /* Resolve conflict if we got this far. */
        /* keep going until we get a good answer */
    gotit = false;
    do {
            /* Give them a choice on how to resolve the conflict */
        choice = false;
        do {	
	    msg_str = (char *)dce_sprintf(pwd_imp_prompt_select_conflict_action);
            read_line(msg_str, token, sizeof(token));
	    if ( msg_str != NULL ) {
		free(msg_str);
	    }

            if (match_command("alias", token, 1)) {
                SET(item_p->flags, sec_rgy_pgo_is_an_alias);
                *alias_p = true;
                gotit = true; choice = true;

            } else if (match_command("generate", token, 1)) {
                item_p->unix_num = sec_rgy_uxid_unknown;
                gotit = true; choice = true;

            } else if (match_command("skip", token, 1)) {
                *done_p = true;
                gotit = true; choice = true;

            } else if (match_command("help", token, 1)) {
                dce_fprintf(stderr, pwd_imp_inf_choices);

            } else if (match_command("enter", token, 1)) {
                choice = true;
            } else {
                dce_pgm_fprintf(stderr, pwd_imp_err_invalid_option, token);
            }
        } while (!choice);

            /* Only prompt for a new UNIX id if don't have alternative */
        if (!gotit) {
	    msg_str = (char *)dce_sprintf(pwd_imp_prompt_enter_unix_id);
            read_line(msg_str, token, sizeof(token));
	    if ( msg_str != NULL ) {
		free(msg_str);
	    }
            if (!is_number(token, strlen(token))) {
                dce_fprintf(stderr, pwd_imp_err_invalid_uid);
                continue;
            }
            if (!(sscanf(token, "%u", &new_num) == 1)) {
                dce_fprintf(stderr,pwd_imp_err_invalid_uid);
                continue;
            }                  
                /* make sure the number is acceptable */
            if (!chk_num(new_num)) {
                dce_fprintf(stderr,pwd_imp_err_invalid_uid);
                continue; 
            }

                /* looks like we got a good one */ 
            gotit = true;
            item_p->unix_num = new_num;
        }
    } while (!gotit);
} /* do_num_conflict */


/*
 * add_pgo - Add the pgo entry for the specified domain.
 *           Check for and resolve any conflicts.
 */
PRIVATE boolean32 add_pgo
  (
    char                *name,
    sec_rgy_pgo_item_t  *item_p,
    sec_rgy_domain_t    domain,
    sec_rgy_handle_t    context,
    boolean32           identical,
    boolean32           check_only
  )
{
    boolean32           an_alias = false, add_to_cache = false;
    boolean32           alias_ok, done;
    sec_rgy_name_t      pgo_name, tmp_name;
    char                msg[sec_rgy_name_t_size+16];    /* name + uid */
    sec_rgy_pgo_item_t  tmp_item;
    imp_tbl_ent_t       t_ent, new_t_ent;
    sec_rgy_cursor_t    cursor;
    error_status_t      st;
    char                *domain_str;

    alias_ok = ((domain == sec_rgy_domain_person) ||
                (domain == sec_rgy_domain_group));
    CLEAR_STATUS(&st);
    switch(domain) {
        case sec_rgy_domain_person:
            domain_str = "Principal";
            break;
        case sec_rgy_domain_group:
            domain_str = "Group";
            break;
        case sec_rgy_domain_org:
            domain_str = "Org";
            break;
        default:
            SET_STATUS(&st, sec_rgy_bad_domain);
            err_exit(st);
    }

        /* Save the data passed in for the cache */
    strncpy((char *)new_t_ent.orig_name, name, sec_rgy_name_max_len);
    new_t_ent.orig_name[sec_rgy_name_max_len] = '\0';
    new_t_ent.orig_num = item_p->unix_num;

    /* See if this is an alias in the import file i.e. same unix id.
     * If so and it's supported, we want to preserve the alias-ness
     */
    imp_lookup_num(domain, item_p->unix_num, &t_ent, &st);
    if (GOOD_STATUS(&st)) {
        if (alias_ok) {         /* it's a supported alias */
            item_p->unix_num = t_ent.new_num;
            SET(item_p->flags, sec_rgy_pgo_is_an_alias);
            an_alias = true;
        }
    } else {
        CLEAR_STATUS(&st);
    }

    strncpy((char *)pgo_name, name, sec_rgy_name_max_len);
    pgo_name[sec_rgy_name_max_len] = '\0';
    done = false;

    do {
        /* Check for name conflict (i.e. same name already in */
        /* registry).  If so, resolve the conflict            */
        sec_rgy_cursor_reset(&cursor);
        sec_rgy_pgo_get_by_name(context, domain, pgo_name, &cursor,
                                &tmp_item, &st); 
        if (GOOD_STATUS(&st)) {
            switch(domain) {
                case sec_rgy_domain_person:
                    if (item_p->unix_num == tmp_item.unix_num) {
                        SET_STATUS(&st, pwd_imp_err_pname_exist_match);
                    } else {
                        SET_STATUS(&st, pwd_imp_err_pname_exist_nmatch);
                    }
                    break;
                case sec_rgy_domain_group:
                    if (item_p->unix_num == tmp_item.unix_num) {
                        SET_STATUS(&st, pwd_imp_err_gname_exist_match);
                    } else {
                        SET_STATUS(&st, pwd_imp_err_gname_exist_nmatch);
                    }
                    break;
                case sec_rgy_domain_org:
                default:
                    SET_STATUS(&st, pwd_imp_err_internal_error);
                    err_exit(st);
            }
	    dce_fprintf(stderr, pwd_imp_msg_conflict, name, item_p->unix_num);
	    dce_fprintf(stderr, st);

                /* If -i specified, then we ignore identical names */
            if (identical) {
                dce_fprintf(stderr, pwd_imp_msg_import, domain_str);
                SET_STATUS(&st, pwd_imp_inf_ignore_conflict);
                dce_fprintf(stderr, pwd_imp_inf_ignore_conflict);
                add_to_cache = true;
                done = true;
            } else {
                do_name_conflict(domain, pgo_name, item_p->unix_num,
                                    tmp_item.unix_num, &done, identical,
                                    check_only);
                if (done) {
                    /* Must have specified 'skip' this entry */
                    return false;
                } else {
                    /* No longer a name conflict */
                    CLEAR_STATUS(&st);
                }
            }
        } else {
            /* Bad status is good, because we wanted lookup to fail */
            CLEAR_STATUS(&st);
        }

        if ((GOOD_STATUS(&st)) && (!an_alias)) {
            /* check for num conflict, and if so, resolve the conflict.  */
            sec_rgy_cursor_reset(&cursor);
            sec_rgy_pgo_get_by_unix_num(context, domain, (unsigned_char_p_t)"",
                                        item_p->unix_num, true, &cursor,
                                        &tmp_item, tmp_name, &st);
            if (GOOD_STATUS(&st)) {
                switch(domain) {
                    case sec_rgy_domain_person:
                        SET_STATUS(&st, pwd_imp_err_puid_exists);
                        break;
                    case sec_rgy_domain_group:
                        SET_STATUS(&st, pwd_imp_err_guid_exists);
                        break;
                    case sec_rgy_domain_org:
                    default:
                        SET_STATUS(&st, pwd_imp_err_internal_error);
                        err_exit(st);
                }
		dce_fprintf(stderr, pwd_imp_msg_conflict, pgo_name, 
			item_p->unix_num);
		dce_fprintf(stderr, st);
                dce_fprintf(stderr, pwd_imp_msg_conflict_entry,
			(char *)tmp_name, tmp_item.unix_num);
                do_num_conflict(domain, item_p, &an_alias, &done, check_only);
                if (done)
                    /* Must have specified 'skip' this entry */
                    return false;
            } else {
                /* Bad status is good, because we wanted lookup to fail */
                CLEAR_STATUS(&st);
            }
        }

        if ((GOOD_STATUS(&st)) && (!done) && (!check_only)) {
            if (verbose) {
                if (item_p->unix_num == sec_rgy_uxid_unknown) {
                    dce_fprintf(stderr, pwd_imp_msg_add_entry_gen_unix_id, 
                            domain_str, pgo_name, item_p->unix_num);
                } else {
                    dce_fprintf(stderr, pwd_imp_msg_add_entry, domain_str,
                            pgo_name, item_p->unix_num);
                }
            }
            sec_rgy_pgo_add(context, domain, pgo_name, item_p, &st);
            if (BAD_STATUS(&st)) {
		dce_fprintf(stderr, pwd_imp_err_add_pgo, 
			pgo_name, dce_sprintf(st));
                return false;
            } else {
                add_to_cache = true;
                done = true;
            }
        } else if (check_only) {
            /* In check mode, don't do anything else */
            done = true;
        }
    } while (!done);

    if (add_to_cache) {
            /* Save the data (w/changes if any) in the cache */
        strcpy((char *)new_t_ent.new_name, (char *) pgo_name);
        new_t_ent.new_num = item_p->unix_num; 
        imp_add(domain, &new_t_ent, &an_alias, &st);
        if (BAD_STATUS(&st)) {
	    dce_fprintf(stderr, pwd_imp_err_internal_cache);
            dce_fprintf(stderr, st);
            return false;
        }
    }

    return true;
} /* add_pgo */ 


/*
 * process_groups -
 *      Create group entries from /etc/group or equivalent file.
 */
PRIVATE error_status_t process_groups
  (
    boolean32           ign_ident,
    boolean32           check_only,
    sec_rgy_handle_t    context,
    int                 *num_groups_p
  )
{
    error_status_t      st;
    struct group        *grent_p;
    sec_rgy_pgo_item_t  g_item;

    VERBOSE_MESSAGE(pwd_imp_inf_create_groups);
    g_item.fullname[0] = '\0';
    g_item.quota = 0;
    *num_groups_p = 0;

    do {            /* keep going until no more entries */
        grent_p = pi_getgrent();    /* get the first/next entry */
        if (grent_p != NULL) {
            CLEAR_STATUS(&st);
            chk_pgo_name(sec_rgy_domain_group, grent_p->gr_name, &st);
            if (BAD_STATUS(&st)) {
                dce_fprintf(stderr, st);
		dce_fprintf(stderr, pwd_imp_err_bad_name, grent_p->gr_name);
                continue;
            }
            g_item.unix_num = grent_p->gr_gid;          /* unix id   */
            g_item.flags = sec_rgy_pgo_projlist_ok;
            g_item.id = uuid_nil;

            /* attempt to add the group, resolve conflicts if (necessary) */
            if (add_pgo(grent_p->gr_name, &g_item, sec_rgy_domain_group,
                            context, ign_ident, check_only)) {
                *num_groups_p += 1;
            }
        } else if ((errno != 0) && (*num_groups_p == 0)) {
            perror("Can't import group entry");
        }
    } while (!(grent_p == NULL));

    return;
} /* process_groups */


/*
 * close_etc - done with imported files
 */
PRIVATE void close_etc(
void
)
{
    VERBOSE_MESSAGE(pwd_imp_inf_close_import);
    pi_endpwent();
    pi_endgrent();
} /* close_etc */ 


/*
 * close_rgy - cleanup registry information
 */ 
PRIVATE void close_rgy
  (
    sec_rgy_handle_t    *context_p
  )
{
    error_status_t  st;
    VERBOSE_MESSAGE(pwd_imp_inf_close_rgy);
    if (context_p) {
        sec_rgy_site_close(*context_p, &st);
        *context_p = NULL;
    }

    return;
} /* close_rgy */


/*
 * add_org_member -
 *  Add the specified principal as a  member of the specified org.
 *  The org must already exist.
 */
PRIVATE boolean32 add_org_member
  (
    sec_rgy_handle_t    context,
    unsigned_char_p_t   p_name,
    unsigned_char_p_t   org,
    error_status_t      *st_p
  )
{ 
    CLEAR_STATUS(st_p);

    sec_rgy_pgo_add_member(context, sec_rgy_domain_org, org,
                             p_name, st_p);
    return(GOOD_STATUS(st_p));

} /* add_org_member */


/*
 * add_acct - Build and add an account from the specified data.
 */
PRIVATE void add_acct
  (
    sec_rgy_handle_t        context,
    sec_rgy_name_t          p_name,
    sec_rgy_name_t          g_name,
    sec_rgy_name_t          o_name,
    struct passwd           *pwent_p,
    sec_rgy_acct_admin_t    *admin_part,
    sec_rgy_acct_user_t     *user_part,
    sec_passwd_rec_t        *caller_key,
    sec_passwd_rec_t        *new_key
  )
{ 
    error_status_t          st;
    sec_rgy_login_name_t    login_name;
    sec_passwd_version_t    version;
    sec_rgy_acct_key_t      keyparts = sec_rgy_acct_key_person;
    sec_passwd_type_t       keytype = sec_passwd_des;

    strncpy((char *)login_name.pname, (char *)p_name, sec_rgy_name_max_len);
    login_name.pname[sec_rgy_name_max_len] = '\0';
    strncpy((char *)login_name.gname, (char *)g_name, sec_rgy_name_max_len);
    login_name.gname[sec_rgy_name_max_len] = '\0';
    strncpy((char *)login_name.oname, (char *)o_name, sec_rgy_name_max_len);
    login_name.oname[sec_rgy_name_max_len] = '\0';

    if (strlen(pwent_p->pw_dir) > sizeof(user_part->homedir)) {
	dce_fprintf(stderr, pwd_imp_err_data_too_long, pwent_p->pw_dir);
        return;
    }
    if (strlen(pwent_p->pw_shell) > sizeof(user_part->shell)) {
	dce_fprintf(stderr, pwd_imp_err_data_too_long, pwent_p->pw_shell);
        return;
    }
    if (strlen(pwent_p->pw_gecos) > sizeof(user_part->gecos)) {
	dce_fprintf(stderr, pwd_imp_err_data_too_long, pwent_p->pw_gecos);
        return;
    }
    
    strcpy((char *) user_part->homedir, pwent_p->pw_dir);
    strcpy((char *) user_part->shell, pwent_p->pw_shell);
    strcpy((char *) user_part->gecos, pwent_p->pw_gecos);
 
        /* add the account */
    if (verbose) {
        printf(">> Adding account for %s %s %s.\n", (char *)p_name,
                (char *)g_name, (char *)o_name);
    }
    sec_rgy_acct_add(context, &login_name, &keyparts,
                        user_part, admin_part,
                        caller_key, new_key, keytype,
                        &version, &st);
    if (BAD_STATUS(&st)) {
	dce_fprintf(stderr, pwd_imp_msg_add_acct_fail, (char *)login_name.pname,
                (char *)login_name.gname, (char *)login_name.oname);
	dce_fprintf(stderr, pwd_imp_msg_conflict_rgy_st, dce_sprintf(st));
    }
    return;

} /* add_acct */


/*
 * cobble_up_group_ent - Create & add a group entry for an unnamed group.
 */
PRIVATE boolean32 cobble_up_group_ent
  (
    sec_rgy_handle_t    context,
    signed32            gid,
    sec_rgy_name_t      g_name,
    boolean32           ignore_ident,
    boolean32           check_only
  )
{
    sec_rgy_cursor_t    cursor;
    char                *msg;
    sec_rgy_pgo_item_t  pgo_entry;
    boolean32           got_gid, is_alias = false;
    boolean32           required;
    imp_tbl_ent_t       entry;
    error_status_t      lst;
    
    required = false;
    sec_rgy_cursor_reset(&cursor);
    sec_rgy_pgo_get_by_unix_num(context, sec_rgy_domain_group,
                                (unsigned_char_p_t)"", gid, true,
                                &cursor, &pgo_entry, g_name, &lst);
    got_gid = (GOOD_STATUS(&lst));
    
    if (got_gid) {
        required = FLAG_SET(pgo_entry.flags, sec_rgy_pgo_is_required);
        if (ignore_ident && (!required)) {
            if (verbose) {
                printf(">>>> No group was added for UNIX id: %d, but one exists in the registry.\n", gid);
                printf(">>>> Since -i was specified, will assume identical names were ignored and,\n");
                printf(">>>> the group name: ( %s ) from the registry will be used.\n", g_name);
            }
        }
        return true;
    }

    if (check_only) {                /* generate a name for the group */
	g_name = dce_sprintf(pwd_imp_msg_temp_group_name, gid);
        return true;
    }
    if (!chk_num(gid)) {
	dce_fprintf(stderr, pwd_imp_err_invalid_uid);
	return false;
    }

    dce_fprintf(stderr, pwd_imp_err_unknown_group);

        /* If we got this far, there's no choice but make them
         * add a group for this id.
         */
    pgo_entry.unix_num = gid;
    pgo_entry.id = uuid_nil;
    pgo_entry.flags = sec_rgy_pgo_projlist_ok;
    pgo_entry.fullname[0] = '\0';

    msg = (char *) dce_sprintf(pwd_imp_msg_enter_group_name, gid);

    do {    /* keep going until we get a good new name */
        get_pgo_name(sec_rgy_domain_group, msg, g_name);
    } while (! add_pgo((char *)g_name, &pgo_entry, sec_rgy_domain_group,
                        context, false, false));

    if ( msg != NULL ) {
	free(msg);
    }
    return true;

} /* cobble_up_group_ent */


/*
 * add_group_member - Add the principal as a member of the indicated group.
 */
PRIVATE boolean32 add_group_member
  (
    sec_rgy_handle_t    context,
    sec_rgy_name_t      p_name,
    sec_rgy_name_t      g_name,
    signed32            gid,
    boolean32           check_only,
    boolean32           ignore_ident,
    error_status_t      *st_p
  )
{
    imp_tbl_ent_t   t_ent;
    CLEAR_STATUS(st_p);

        /* make sure the group is defined */
    imp_lookup_num(sec_rgy_domain_group, gid, &t_ent, st_p);
    if (GOOD_STATUS(st_p)) {
            /* watch out for name changes */
        strcpy((char *)g_name, (char *)t_ent.new_name);
    } else if (STATUS_EQUAL(st_p, pwd_imp_err_cache_e_not_found)) { 
            /* this is an unnamed group in the import file */ 
        if (!cobble_up_group_ent(context, gid, g_name, ignore_ident, check_only)) {
            SET_STATUS(st_p, pwd_imp_err_unknown_group);
            return false;
        }
    } else {
        SET_STATUS(st_p, pwd_imp_err_unknown_group);
        return false;
    } 

        /* Can only assume principal and group was added if
         * running in check mode, since those operations weren't
         * really executed in this mode...
         */
    if (check_only) {
        return  true;
    }
    sec_rgy_pgo_add_member(context, sec_rgy_domain_group, g_name,
                            p_name, st_p);
    
    return(GOOD_STATUS(st_p));

} /* add_group_member */


/*
 * add_members - add memberships from etc/group
 */
PRIVATE void add_members
  (
    sec_rgy_handle_t    context,
    boolean32           check_only,
    boolean32           ignore_ident
  )
{ 
    void            *imp_cursor;
    int             i;
    imp_tbl_ent_t   t_ent;
    error_status_t  st;
    struct group    *grent_ptr;
    boolean32       member_printed;
    sec_rgy_name_t  tmp_name;

    CLEAR_STATUS(&st);
    VERBOSE_MESSAGE(pwd_imp_inf_add_members);

    /* step thru group table/getgrgid rather than getgrent, so
     * that we don"t have to re-check namelength, etc.
     * group table SHOULD only contain entries for good groups.
     * but watch for unnamed-groups from passwd file
     */

    imp_cursor = NULL;  /* start at the beginning of the list */
    imp_get_next(sec_rgy_domain_group, &imp_cursor, &t_ent, &st);

    while (GOOD_STATUS(&st)) {              /* cycle through group entries   */
        pi_setgrent();                      /* always start at the beginning */
        grent_ptr = pi_getgrgid(t_ent.orig_num);    /* get group entry by gid */
        if (grent_ptr != NULL) {
            i = 0;
            /* cycle through membership list */
            while ( (grent_ptr->gr_mem[i] != NULL) &&
                    (*(grent_ptr->gr_mem[i]) != '\0') ) {
                member_printed = false; 
                if ((verbose) && (!member_printed)) {
                    printf(">> Add %s as member of group with UNIX id: %d\n",
                            grent_ptr->gr_mem[i], t_ent.orig_num);
                    member_printed = true;
                }
                if (!add_group_member(context,
                                        (unsigned_char_p_t)grent_ptr->gr_mem[i],
                                        tmp_name,
                                        t_ent.orig_num, check_only,
                                        ignore_ident, &st)) {
		    dce_fprintf(stderr, pwd_imp_err_add_group_member,
                            grent_ptr->gr_mem[i], t_ent.orig_num,
                    	    dce_sprintf(st));
                }
                i++;
            } /* while - i.e. end of member list for this group */
        }  /* if (grent_ptr != NULL) */
        imp_get_next(sec_rgy_domain_group, &imp_cursor, &t_ent, &st);
    } /* while - i.e. end of group file */

    return;
} /* add_members */


/*
 * process_principals_accounts -
 *      Create principal entries from imported passwd file.
 *      Also, add them as members of account group & org.
 *      Finally, add the account itself.
 */
PRIVATE void process_principals_accounts
  (
    boolean32           ign_ident,
    boolean32           check_only,
    sec_rgy_handle_t    context,
    char                *org,
    char                *caller_pkey,
    int                 *num_princs_p,
    error_status_t      *st_p
  )
{
    struct passwd           *pwent_p;
    sec_rgy_pgo_item_t      item;
    sec_rgy_name_t          p_name, g_name;
    error_status_t          lst;
    sec_rgy_acct_user_t     acct_user_part;
    sec_rgy_acct_admin_t    acct_admin_part;
    sec_passwd_rec_t        ckey, *nkey;
    sec_passwd_str_t        cpkey;
    sec_passwd_type_t       keytype = sec_passwd_des;
    imp_tbl_ent_t   t_ent;
    time_t                  tloc;
    char                    error_message[sec_rgy_name_t_size+10]; /*name+uid */

    VERBOSE_MESSAGE(pwd_imp_inf_create_accounts);
    *num_princs_p = 0;

        /* Only initialize these once for all accounts */
    acct_user_part.flags =  SEC_DEF_FLGS_ACCT_USER;
    acct_admin_part.expiration_date = 0;
    acct_admin_part.flags = SEC_DEF_FLGS_ACCT_ADMIN;
    time(&tloc);
    acct_admin_part.good_since_date = (sec_timeval_sec_t)tloc;
    acct_admin_part.authentication_flags = SEC_DEF_FLGS_ACCT_AUTH;

        /* Set up caller key fields */
    ckey.version_number = sec_passwd_c_version_none;
    ckey.pepper = NULL;
    ckey.key.key_type = sec_passwd_plain;
    ckey.key.tagged_union.plain = &(cpkey[0]);

        /* The following PGO fields are the same for all imported principals */
    item.quota = 0;
    item.fullname[0] = '\0';

    pi_setpwent();
    do {       /* keep going until no more entries */
        pwent_p = pi_getpwent();    /* get the first/next entry from passwd */
        if (pwent_p != NULL) { 
            CLEAR_STATUS(st_p);
            if (strlen(pwent_p->pw_name) > sec_rgy_name_max_len) {
                SET_STATUS(&lst, pwd_imp_err_pname_too_long);
            } else {
                chk_pgo_name(sec_rgy_domain_person, pwent_p->pw_name, &lst);
            }
            if (BAD_STATUS(&lst)) {
                dce_fprintf(stderr, lst);
		dce_fprintf(stderr, pwd_imp_err_bad_name, pwent_p->pw_name);
                continue;
            }
            item.id = uuid_nil;
            item.unix_num = pwent_p->pw_uid;    /* unix id */
            item.flags = sec_rgy_pgo_projlist_ok;

                /* add the person, resolve conflicts if (necessary */              
            if (!add_pgo(pwent_p->pw_name, &item, sec_rgy_domain_person,
                            context, ign_ident, check_only)) {
                continue;
            } else {
                *num_princs_p += 1;
            }

                /* Can only assume principal and group was added if
                 * running in check mode, since those operations weren't
                 * really executed in this mode...
                 */
            if (check_only)
                continue;

            imp_lookup_name(sec_rgy_domain_person, pwent_p->pw_name, &t_ent, &lst); 
            if (GOOD_STATUS(&lst)) {
                    /* watch out for name changes */
                strcpy((char *)p_name, (char *)t_ent.new_name);
            } else {
                strncpy((char *)p_name, pwent_p->pw_name, sec_rgy_name_max_len);
                p_name[sec_rgy_name_max_len] = 0;
            }

                /* add memberships needed for this account */
            if (!add_group_member(context, p_name,
                                    g_name, pwent_p->pw_gid, check_only,
                                    ign_ident, &lst)){
		dce_fprintf(stderr, pwd_imp_err_add_princ_group,  
                        pwent_p->pw_name, pwent_p->pw_gid,
			dce_sprintf(lst));
                continue;
            }
            if (!add_org_member(context, p_name,
                                (unsigned_char_p_t) org, &lst)) {
                dce_fprintf(stderr, pwd_imp_err_add_org_member,
		    pwent_p->pw_name, dce_sprintf(lst));
                continue;
            }
        
                /* Reset the caller key each time in case it's zero'd */
            strncpy((char *)cpkey, caller_pkey, sec_passwd_str_max_len);
            cpkey[sec_passwd_str_max_len] = '\0';
            sec_key_mgmt_gen_rand_key(rpc_c_authn_none, (void *)NULL,
                                        p_name,
                                        (void *)&keytype,
                                        (unsigned32)sec_passwd_c_version_none,
                                        (void **)&nkey, &lst);

                /* build & add the account */
            add_acct(context, p_name,
                        g_name, (unsigned_char_p_t) org, pwent_p,
                        &acct_admin_part, &acct_user_part, &ckey, nkey);
            sec_key_mgmt_free_key(nkey, &lst);
        } else if ((errno != 0) && (*num_princs_p == 0)) {
            perror("Error while reading passwd entries");
        }
     } while (!(pwent_p == NULL));

    return;
} /* process_principals_accounts */
    

/*
 * passwd_import - main body of the utility.
 *      Set up import files.
 *      Set up registry context.
 *      Make sure the specified (or default) organization exists.
 *      Add groups from group import file.
 *
 *  If any major step fails, cleanup is performed and the utility
 *  exits with a fatal status, because it is unknown what problems
 *  further processing might cause after a fatal error.
 */
main
  (
    int  argc,
    char **argv
  )
{ 
    error_status_t      st;
    boolean32           iflag   = false;
    boolean32           pflag   = false;
    boolean32           uflag   = false;
    boolean32           check   = false;
    boolean32           err     = false;
    sec_rgy_handle_t    context;
    sec_rgy_name_t      u_name;
    char                *org    = DEFAULT_IMPORT_ORG;
    char                *dir    = DEFAULT_IMPORT_DIR;
    char                cky[sec_passwd_str_t_size];
    krb5_error_code     retval;
    int                 ch, num_groups, num_princs;
    int                 size = sec_passwd_str_max_len;
    char                *msg_str=NULL;
    extern              char *optarg;
    uuid_create_nil(&uuid_nil, &st);

    setlocale(LC_ALL, "");

    dce_svc_set_progname("passwd_import", &st);
    dce_msg_define_msg_table(sad__table,
        sizeof sad__table / sizeof sad__table[0], &st);
    
        /* Unless told otherwise, POSIX mask_obj calculation */

        /* Parse arguments */
    while (!err && ((ch = getopt(argc, argv, "hicvo:p:d:u:")) != EOF)) {
        switch (ch) {
            case 'h':
                dce_fprintf(stderr, pwd_imp_cl_usage, argv[0]);
                exit(0);
                break;
            case 'i':
                iflag = true;
                break;
            case 'c':
                check = true;
                break;
            case 'v':
                verbose = true;
                break;
            case 'o':
                org = optarg;
                break;
            case 'd':
                dir = optarg;
                break;
            case 'u':
                strncpy((char *)u_name, optarg, sec_rgy_name_max_len);
                u_name[sec_rgy_name_max_len] = '\0';
                uflag = true;
                break;
            case 'p':
                strncpy(cky, optarg, sec_passwd_str_max_len);
                cky[sec_passwd_str_max_len] = '\0';
                memset(optarg, 0, strlen(cky));
                pflag = true;
                break;
            case '?':
            default:
                err = true;
                break;
        }
    }
    if (err) {
	dce_fprintf(stderr, pwd_imp_cl_usage, argv[0]);
        exit(1);
    }
    if (!uflag) {
	msg_str = (char *) dce_sprintf(pwd_imp_prompt_enter_user_name);
        get_pgo_name(sec_rgy_domain_person, msg_str, u_name);
	if ( msg_str != NULL ) {
	    free(msg_str);
	}
    }
    if (!pflag) {
        /* Use the krb5 utility routine for obtaining passwords */
        if (retval = krb5_read_password("Enter your password: ", NULL,
                                        (char *)cky, &size)) {
            com_err(argv[0], retval, "Error in input password.");
            exit(1);
        } else {
            /* Make sure a nl gets output */
	    fputc('\n', stdout);
        }
    }

        /* setup files being imported */
    if (! setup_etc(dir)) {
        dce_pgm_fprintf(stderr, pwd_imp_err_fatal);
        exit(1);
    }
        /* open & initialize registry */
    if (! setup_rgy(u_name, cky, &context)) {
        close_etc();
        dce_pgm_fprintf(stderr, pwd_imp_err_fatal);
        exit(1);
    }
        /* Initialize the cache with estimated num of entries*/
    imp_init();

        /* Make sure the Org exists before doing any real work */
    check_org(org, context, &st);
    if (BAD_STATUS(&st)) {
	dce_pgm_fprintf(stderr, st);
        dce_pgm_fprintf(stderr, pwd_imp_rgy_bad_org, org);
        dce_pgm_fprintf(stderr, pwd_imp_err_fatal);
        imp_cleanup();
        close_etc();
        close_rgy(&context);
        exit(1);
    }

        /* create group entries from <dir>/group */
    process_groups(iflag, check, context, &num_groups);

        /* create principal entries, memberships, accounts
         * from etc/passwd
         */
    process_principals_accounts(iflag, check, context, org,
                                cky, &num_princs, &st);

        /* add members from etc/group to groups */
    add_members(context, check, iflag);

        /* done with the cache */
    imp_cleanup();
        /* done with import files */ 
    close_etc();
        /* done with registry */
    close_rgy(&context);
}

