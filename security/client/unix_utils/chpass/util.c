/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util.c,v $
 * Revision 1.1.6.2  1996/02/18  00:07:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:05  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:24:12  root
 * 	Submit
 * 	[1995/12/11  15:14:01  root]
 * 
 * Revision 1.1.4.2  1992/12/29  13:05:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:42:49  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/17  19:01:45  burati
 * 	Integration with DCE
 * 	[1992/07/17  18:50:26  burati]
 * 
 * 	Initial version from BSD4.4
 * 	[1992/07/13  18:54:53  burati]
 * 
 * $EndLog$
 */

/*-
 * Copyright (c) 1988 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted provided
 * that: (1) source distributions retain this entire copyright notice and
 * comment, and (2) distributions including binaries display the following
 * acknowledgement:  ``This product includes software developed by the
 * University of California, Berkeley and its contributors'' in the
 * documentation or other materials provided with the distribution and in
 * all advertising materials mentioning features or use of this software.
 * Neither the name of the University nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
static char sccsid[] = "@(#)util.c	5.13 (Berkeley) 6/29/90";
#endif /* not lint */

#include <sys/types.h>
#include <sys/time.h>
#include <tzfile.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dce/binding.h>
#include <dce/acct.h>
#include <dce/misc.h>
#include <dce/policy.h>
#include <dce/rpc.h>
#include <chpass.h>
#include <pathnames.h>

static int dmsize[] =
	{ -1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static char *months[] =
	{ "January", "February", "March", "April", "May", "June",
	  "July", "August", "September", "October", "November",
	  "December", NULL };

char *ttoa
#ifndef __STDC__
    (tval)
	time_t tval;
#else
  (
	time_t tval
  )
#endif
{
	struct tm *tp;
	static char tbuf[50];

	if (tval) {
		tp = localtime(&tval);
		(void)sprintf(tbuf, "%s %d, 19%d", months[tp->tm_mon],
		    tp->tm_mday, tp->tm_year);
	}
	else
		*tbuf = '\0';
	return(tbuf);
} 

int atot
#ifndef __STDC__
    (p, store)
	char *p;
	time_t *store;
#else
  (
	char *p,
	time_t *store
  )
#endif
{
	register char *t, **mp;
	static struct tm *lt;
	time_t tval, time();
	int day, month, year;

	if (!*p) {
		*store = 0;
		return(0);
	}
	if (!lt) {
		unsetenv("TZ");
		(void)time(&tval);
		lt = localtime(&tval);
	}
	if (!(t = strtok(p, " \t")))
		goto bad;
	for (mp = months;; ++mp) {
		if (!*mp)
			goto bad;
		if (!strncasecmp(*mp, t, 3)) {
			month = mp - months + 1;
			break;
		}
	}
	if (!(t = strtok((char *)NULL, " \t,")) || !isdigit(*t))
		goto bad;
	day = atoi(t);
	if (!(t = strtok((char *)NULL, " \t,")) || !isdigit(*t))
		goto bad;
	year = atoi(t);
	if (day < 1 || day > 31 || month < 1 || month > 12 || !year)
		goto bad;
	if (year < 100)
		year += TM_YEAR_BASE;
	if (year <= EPOCH_YEAR)
bad:		return(1);
	tval = isleap(year) && month > 2;
	for (--year; year >= EPOCH_YEAR; --year)
		tval += isleap(year) ?
		    DAYSPERLYEAR : DAYSPERNYEAR;
	while (--month)
		tval += dmsize[month];
	tval += day;
	tval = tval * HOURSPERDAY * MINSPERHOUR * SECSPERMIN;
	tval -= 14400;
/* lt->tm_gmtoff; */
	*store = tval;
	return(0);
}

/*
 * print --
 *	print out the file for the user to edit; strange side-effect:
 *	return if the user is allowed to modify their shell.
 */
int print
#ifndef __STDC__
    (fp, pw)
	FILE *fp;
	struct passwd *pw;
#else
  (
	FILE *fp,
	struct passwd *pw
  )
#endif
{
	register char *p;
	int shellval;
	char *bp;
	char *getusershell(), *ok_shell(), *ttoa();

	shellval = 1;
	(void)fprintf(fp, "#Changing user database information for %s.\n",
	    pw->pw_name);
	if (!uid) {
		(void)fprintf(fp, "Login: %s\n", pw->pw_name);
		(void)fprintf(fp, "Password: %s\n", pw->pw_passwd);
		(void)fprintf(fp, "Uid [#]: %d\n", pw->pw_uid);
		(void)fprintf(fp, "Gid [# or name]: %d\n", pw->pw_gid);
		(void)fprintf(fp, "Change [month day year]: %s\n",
		    ttoa(pw->pw_change));
		(void)fprintf(fp, "Expire [month day year]: %s\n",
		    ttoa(pw->pw_expire));
		(void)fprintf(fp, "Home directory: %s\n", pw->pw_dir);
		(void)fprintf(fp, "Shell: %s\n",
		    *pw->pw_shell ? pw->pw_shell : _PATH_BSHELL);
	}
	/* only admin can change "restricted" shells */
	else if (ok_shell(pw->pw_shell))
		(void)fprintf(fp, "Shell: %s\n",
		    *pw->pw_shell ? pw->pw_shell : _PATH_BSHELL);
	else
		shellval = 0;
	bp = pw->pw_gecos;
	p = strsep(&bp, ",");
	(void)fprintf(fp, "Full Name: %s\n", p ? p : "");
	p = strsep(&bp, ",");
	(void)fprintf(fp, "Location: %s\n", p ? p : "");
	p = strsep(&bp, ",");
	(void)fprintf(fp, "Office Phone: %s\n", p ? p : "");
	p = strsep(&bp, ",");
	(void)fprintf(fp, "Home Phone: %s\n", p ? p : "");
	return(shellval);
}

char *ok_shell
#ifndef __STDC__
    (name)
	register char *name;
#else
  (
	register char *name
  )
#endif
{
	register char *p, *sh;
	char *getusershell();

	setusershell();
	while (sh = getusershell()) {
		if (!strcmp(name, sh))
			return(name);
		/* allow just shell name, but use "real" path */
		if ((p = rindex(sh, '/')) && !strcmp(name, p + 1))
			return(sh);
	}
	return(NULL);
}

/* Cache registry account data so it doesn't have
 * to be obtained in multiple places.
 */
static sec_rgy_login_name_t account_name;
static sec_rgy_handle_t     rgy_h;
static sec_rgy_acct_user_t  user_part;
static sec_rgy_acct_admin_t admin_part;
static sec_rgy_acct_key_t   key_parts;

void get_account_data
#ifndef __STDC__
    (name, get_pwent, pwent_p, st_p)
    unsigned_char_p_t   name;
    boolean32           get_pwent;
    struct passwd       *pwent_p;
    error_status_t      *st_p;
#else
  (
    unsigned_char_p_t   name,
    boolean32           get_pwent,
    struct passwd       *pwent_p,
    error_status_t      *st_p
  )
#endif
{
    sec_rgy_cursor_t    cursor;
    sec_rgy_sid_t       sid;
    sec_rgy_unix_sid_t  unix_sid;
    sec_rgy_plcy_t      plcy;
    sec_timeval_sec_t   tmp_exp;
    sec_rgy_name_t      tmp_name;
    sec_rgy_login_name_t    name_key;
    sec_rgy_cursor_reset(&cursor);
    CLEAR_STATUS(st_p);

    /* In DCE1.* there is only one account per principal,
     * so we can just do a wildcard acct lookup by pname.
     */
    strncpy((char *)name_key.pname, (char *)name,
            sec_rgy_name_max_len);
    name_key.pname[sec_rgy_name_max_len] = '\0';
    name_key.gname[0] = '\0';
    name_key.oname[0] = '\0';
    if (name_key.pname[0] == '/') {
        sec_rgy_site_open(name_key.pname, &rgy_h, st_p);
    } else {
        strcpy((char *)tmp_name, "/.:");
        sec_rgy_site_open(tmp_name, &rgy_h, st_p);
    }
    if (GOOD_STATUS(st_p)) {
        sec_rgy_acct_lookup(rgy_h, &name_key, &cursor,
                            &account_name, &sid, &unix_sid,
                            &key_parts, &user_part, &admin_part, st_p);
        if (GOOD_STATUS(st_p)) {
            pwent_p->pw_passwd  = (char *)user_part.passwd;
            if (get_pwent) {
                /* Fill in the passwd entry structure for the caller */
                pwent_p->pw_uid     = unix_sid.person;
                pwent_p->pw_gid     = unix_sid.group;
                pwent_p->pw_class   = "";
                pwent_p->pw_gecos   = (char *)user_part.gecos;
                pwent_p->pw_dir     = (char *)user_part.homedir;
                pwent_p->pw_shell   = (char *)user_part.shell;
                pwent_p->pw_expire  = admin_part.expiration_date;

                sec_rgy_plcy_get_effective(rgy_h, account_name.oname,
                                            &plcy, st_p);
                if (GOOD_STATUS(st_p)) {
                    if (plcy.passwd_lifetime == 0) {
                        /* Use exp date, unless passwd's been changed since then */
                        pwent_p->pw_change =
                            (plcy.passwd_exp_date > user_part.passwd_dtm) ?
                                                    plcy.passwd_exp_date : 0;
                    } else if (plcy.passwd_exp_date == 0) {
                        pwent_p->pw_change = user_part.passwd_dtm +
                                                    plcy.passwd_lifetime;
                    } else {
                        /* Exp date, unless passwd's been changed since then */
                        tmp_exp = (plcy.passwd_exp_date > user_part.passwd_dtm) ?
                                                    plcy.passwd_exp_date : 0;
                        if (tmp_exp == 0) {
                            pwent_p->pw_change = user_part.passwd_dtm +
                                                    plcy.passwd_lifetime;
                        } else {
                            pwent_p->pw_change = MIN(tmp_exp,
                                    (user_part.passwd_dtm + plcy.passwd_lifetime));
                        }
                    }
                }
            }
        }
    }
}


void save_entry(pwent_p, st_p)
    struct passwd   *pwent_p;
    error_status_t  *st_p;
{
    sec_passwd_rec_t        ckey, nkey;
    char                    *caller_passwd;
    sec_passwd_version_t    ver;
    sec_passwd_type_t       ktype = sec_passwd_des;
    boolean32               setkey = false;

    /* Replace the user part */
    strncpy((char *)user_part.gecos, pwent_p->pw_gecos, sec_rgy_pname_max_len);
    user_part.gecos[sec_rgy_pname_max_len] = '\0';
    strncpy((char *)user_part.homedir, pwent_p->pw_dir, sec_rgy_pname_max_len);
    user_part.homedir[sec_rgy_pname_max_len] = '\0';
    strncpy((char *)user_part.shell, pwent_p->pw_shell, sec_rgy_pname_max_len);
    user_part.shell[sec_rgy_pname_max_len] = '\0';

    if (strcmp(pwent_p->pw_passwd, (char *)user_part.passwd) != 0) {
        setkey = true;
        nkey.pepper = ckey.pepper = NULL;
        nkey.version_number = ckey.version_number = sec_passwd_c_version_none;
        nkey.key.key_type = ckey.key.key_type = sec_passwd_plain;
        nkey.key.tagged_union.plain = (unsigned_char_p_t) pwent_p->pw_passwd;

        /* The call to getpass needs to be replaced with a call
         * to an equivalent function that allows passwords to be
         * more than 8 characters long.
         */
        caller_passwd = getpass("Enter your password for verification: ");
        ckey.key.tagged_union.plain = (unsigned_char_p_t) caller_passwd;
    }

    /* Replace all if necessary */
    if (admin_part.expiration_date != pwent_p->pw_expire) {

        admin_part.expiration_date = pwent_p->pw_expire;
        sec_rgy_acct_replace_all(rgy_h, &account_name, &key_parts,
                                    &user_part, &admin_part, setkey,
                                    &ckey, &nkey, ktype, &ver, st_p);
    } else {
        sec_rgy_acct_user_replace(rgy_h, &account_name, &user_part,
                                    setkey, &ckey, &nkey, ktype, &ver, st_p);
    }
}

