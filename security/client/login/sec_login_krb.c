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
 * $Log: sec_login_krb.c,v $
 * Revision 1.1.2.2  1996/03/11  14:09:27  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:31  marty]
 *
 * Revision 1.1.2.1  1995/12/08  18:04:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:31:28  root]
 * 
 * $EndLog$
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <dce/dce_cf.h>

#include <macros.h>
#include <sec_login_krb.h>

#define DCE_BC_KEY	"krb_ccache_vno"
#define DCE_CONFIG_FILE DCELOCAL_PATH "/dce_cf.db"

void sec_login_krb_get_ccache_version(
    int                 *version,       /* [out] */
    error_status_t      *stp            /* [out] */
)
{
    char *vno = NULL;
    error_status_t st;
    FILE *fp;
    char *config_file = DCE_CONFIG_FILE;

    fp = fopen(config_file, "r");
    if (fp == (FILE *) NULL) {
        SET_STATUS(stp, errno);
        return;
    }
    dce_cf_find_name_by_key(fp, DCE_BC_KEY, &vno, stp);
    if (*stp == dce_cf_st_ok) {
        *version = atoi(vno);
    }

    (void) fclose(fp);
    if (vno)
        free(vno);
}
