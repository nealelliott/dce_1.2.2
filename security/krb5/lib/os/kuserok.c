/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kuserok.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:48  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:37  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:30:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:17:58  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  kuserok.c V=3 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/kuserok.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
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
 * krb5_kuserok()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_kuserok_c [] =
"$Id: kuserok.c,v 1.1.4.2 1996/02/18 00:15:27 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/sysincl.h>
#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>

#include <stdio.h>
#include <pwd.h>

#define MAX_USERNAME 10

/*
 * Given a Kerberos principal "principal", and a local username "luser",
 * determine whether user is authorized to login according to the
 * authorization file ("~luser/.klogin" by default).  Returns TRUE
 * if authorized, FALSE if not authorized.
 *
 * If there is no account for "luser" on the local machine, returns
 * FALSE.  If there is no authorization file, and the given Kerberos
 * name "server" translates to the same name as "luser" (using
 * krb5_aname_to_lname()), returns TRUE.  Otherwise, if the authorization file
 * can't be accessed, returns FALSE.  Otherwise, the file is read for
 * a matching principal name, instance, and realm.  If one is found,
 * returns TRUE, if none is found, returns FALSE.
 *
 * The file entries are in the format produced by krb5_unparse_name(),
 * one entry per line.
 *
 */

krb5_boolean
krb5_kuserok(principal, luser)
krb5_principal principal;
const char *luser;
{
    struct stat sbuf;
    struct passwd *pwd;
    char pbuf[MAXPATHLEN];
    krb5_boolean isok = FALSE;
    FILE *fp;
    char kuser[MAX_USERNAME];
    char *princname;
    char linebuf[BUFSIZ];
    char *newline;
    int gobble;

    /* no account => no access */
    if ((pwd = getpwnam(luser)) == NULL) {
	return(FALSE);
    }
    (void) strcpy(pbuf, pwd->pw_dir);
    (void) strcat(pbuf, "/.k5login");

    if (access(pbuf, F_OK)) {	 /* not accessible */
	/*
	 * if he's trying to log in as himself, and there is no .klogin file,
	 * let him.  To find out, call
	 * krb5_aname_to_localname to convert the principal to a name
	 * which we can string compare. 
	 */
	if (!(krb5_aname_to_localname(principal,
				      sizeof(kuser), kuser))
	    && (strcmp(kuser, luser) == 0)) {
	    return(TRUE);
	}
    }
    if (krb5_unparse_name(principal, &princname))
	return(FALSE);			/* no hope of matching */

    /* open ~/.klogin */
    if ((fp = fopen(pbuf, "r")) == NULL) {
	free(princname);
	return(FALSE);
    }
    /*
     * security:  if the user does not own his own .klogin file,
     * do not grant access
     */
    if (fstat(fileno(fp), &sbuf)) {
	fclose(fp);
	free(princname);
	return(FALSE);
    }
    if (sbuf.st_uid != pwd->pw_uid) {
	fclose(fp);
	free(princname);
	return(FALSE);
    }

    /* check each line */
    while (!isok && (fgets(linebuf, BUFSIZ, fp) != NULL)) {
	/* null-terminate the input string */
	linebuf[BUFSIZ-1] = '\0';
	newline = NULL;
	/* nuke the newline if it exists */
	if (newline = strchr(linebuf, '\n'))
	    *newline = '\0';
	if (!strcmp(linebuf, princname)) {
	    isok = TRUE;
	    continue;
	}
	/* clean up the rest of the line if necessary */
	if (!newline)
	    while (((gobble = getc(fp)) != EOF) && gobble != '\n');
    }
    free(princname);
    fclose(fp);
    return(isok);
}
