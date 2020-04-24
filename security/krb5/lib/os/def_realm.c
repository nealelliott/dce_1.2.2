/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: def_realm.c,v $
 * Revision 1.1.10.1  1996/10/03  15:06:21  arvind
 * 	Correct compiler warnings.
 * 	[1996/09/16  21:31 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.8.3  1996/02/18  00:15:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:34  marty]
 * 
 * Revision 1.1.8.2  1995/12/08  18:03:13  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/11/08  20:45 UTC  bartf
 * 	merge MIT krb func fix
 * 
 * 	add comment about DCEvsKRB
 * 	[1995/11/07  19:11 UTC  bartf  /main/HPDCE02/bartf_sec_kinit.10/2]
 * 
 * 	add default krb behavior
 * 	[1995/11/02  20:39 UTC  bartf  /main/HPDCE02/bartf_sec_kinit.10/1]
 * 	HP revision /main/HPDCE02/1  1995/04/15  00:04 UTC  rps
 * 	copyright
 * 	[1995/04/05  17:20 UTC  rps  /main/MOTHRA_panic/3]
 * 
 * 	remove #if 0'ed code..
 * 	[1995/04/03  23:50 UTC  sommerfeld  /main/MOTHRA_panic/2]
 * 
 * 	Restructure for "cleaner" cell-to-realm conversions.
 * 	[1995/02/11  00:08 UTC  sommerfeld  /main/sommerfeld_krb5_alias_1/1]
 * 
 * Revision 1.1.4.1  1993/10/14  17:45:39  mccann
 * 	CR8651 64bit port
 * 	[1993/10/14  17:44:04  mccann]
 * 
 * Revision 1.1.2.2  1992/12/29  16:01:07  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:17:21  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  def_realm.c V=2 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/def_realm.c,v $
 * $Author: arvind $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * krb5_get_default_realm() function.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_def_realm_c[] =
"$Id: def_realm.c,v 1.1.10.1 1996/10/03 15:06:21 arvind Exp $";
#endif  /* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#include <stdio.h>

#ifdef OSF_DCE
#   include <dce/dce_cf.h>
#   include <dce/dce_cf_const.h>
#   include <dce/dcesecmsg.h>
#endif

/*
 * Retrieves the default realm to be used if no user-specified realm is
 *  available.  [e.g. to interpret a user-typed principal name with the
 *  realm omitted for convenience]
 * 
 *  returns system errors, NOT_ENOUGH_SPACE
*/

/*
 * Implementation:  the default realm is stored in a configuration file,
 * named by krb5_config_file;  the first token in this file is taken as
 * the default local realm name.
 *
 * OSF_DCE NOTE: In the OSF DCE the default realm is obtained by taking
 * the default cell and removing the global directory root prefix.  The
 * conventional MIT configuration file is not used.
 * We back off and use the MIT mechanism if the DCE mechanism fails?
 * (should we give preference to the MIT mechanism?,
 *  currently DCE takes precedence)
*/

#ifdef OSF_DCE
/* This function should be moved elsewhere */

error_status_t dce_cell_to_realm (unsigned char *cell, char **realm)
{
    char *r, *p;
    int c;
    
    *realm = NULL;
    
    if ((cell[0] != '/') &&
	(cell[1] != '.') &&
	(cell[2] != '.') &&
	(cell[3] != '.') &&
	(cell[4] != '/'))
	return sec_authn_bad_cell_name;

    r = strdup((char *)&cell[5]);
    if (!r)
	return ENOMEM;
    *realm = r;
    
    return error_status_ok;
}

#endif


extern char *krb5_config_file;          /* extern so can be set at
                                           load/runtime */
krb5_error_code
krb5_get_default_realm(lrealm)
char **lrealm;
{
#ifdef OSF_DCE
    error_status_t       st;
    char *new_name;
    int             len;
    krb5_error_code ret = 0;

    dce_cf_get_cell_name(&new_name, &st);
    if (st == dce_cf_st_ok) {
	ret = dce_cell_to_realm((unsigned char *)new_name, lrealm);
        free(new_name);
    } else {
        ret = st;
    }
    if ( ret == 0 )
	return ret;

#endif   /* OSF_DCE, so must be in MIT mode */
{
    FILE *config_file;
    char realmbuf[BUFSIZ];
    char *cp;

    if (!(config_file = fopen(krb5_config_file, "r")))
        /* can't open */
        return KRB5_CONFIG_CANTOPEN;

    if (fscanf(config_file, "%s", realmbuf) != 1) {
        fclose(config_file);
        return( KRB5_CONFIG_BADFORMAT);
    }
    fclose(config_file);
    if (!(*lrealm = cp = malloc((unsigned int) strlen(realmbuf) + 1)))
            return ENOMEM;
    strcpy(cp, realmbuf);
    return(0);
}
}
