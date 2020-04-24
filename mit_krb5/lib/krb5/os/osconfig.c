/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: osconfig.c,v $
 * Revision 1.1.2.1  1996/06/05  21:18:54  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:09:10  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/os/osconfig.c
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
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
 * Definition of default configuration parameters.
 */

#ifdef OLD_CONFIG_FILES
#define NEED_WINDOWS
#endif
#include "k5-int.h"

#ifdef OLD_CONFIG_FILES
char *krb5_config_file = DEFAULT_CONFIG_FILENAME;
char *krb5_trans_file = DEFAULT_TRANS_FILENAME;
#endif
char *krb5_defkeyname  = DEFAULT_KEYTAB_NAME;
#ifdef USE_DBM_LNAME
char *krb5_lname_file = DEFAULT_LNAME_FILENAME;
#endif

int krb5_max_dgram_size = MAX_DGRAM_SIZE;
int krb5_max_skdc_timeout = MAX_SKDC_TIMEOUT;
int krb5_skdc_timeout_shift = SKDC_TIMEOUT_SHIFT;
int krb5_skdc_timeout_1 = SKDC_TIMEOUT_1;

char *krb5_default_pwd_prompt1 = DEFAULT_PWD_STRING1;
char *krb5_default_pwd_prompt2 = DEFAULT_PWD_STRING2;

#ifdef OLD_CONFIG_FILES
/*
 * On Windows, we want to let the user specify in the kerberos.ini file
 * where the config and realms files, krb.con and krb.rea, reside. If they
 * aren't specified then we fall back to having them in the windows
 * directory. We use the same format as the K4 version for compatability.
 *
 * Note: these values can change asynchronously so we can't cache the values.
 */
krb5_error_code
krb5_find_config_files ()
{
#ifdef _WINDOWS
    static char cnfname[160];                   /* For krb.con */
    static char realmsname[160];                /* For krb.rea */
    char defname[160];                          /* Default value */

    /* First locate krb.con file */
    GetWindowsDirectory(defname, sizeof(defname));
    strcat (defname, "\\");
    strcat (defname, DEFAULT_CONFIG_FILENAME);
	GetPrivateProfileString(INI_FILES, INI_KRB_CONF, defname,
    	cnfname, sizeof(cnfname), KERBEROS_INI);
    
    /* Now locate krb.rea file */
    GetWindowsDirectory(defname, sizeof(defname));
    strcat (defname, "\\");
    strcat (defname, DEFAULT_TRANS_FILENAME);
	GetPrivateProfileString(INI_FILES, INI_KRB_REALMS, defname,
    	realmsname, sizeof(realmsname), KERBEROS_INI);

    krb5_config_file = cnfname;
    krb5_trans_file = realmsname;

#endif /* _WINDOWS */

    return 0;
}
#endif /* OLD_CONFIG_FILES */

