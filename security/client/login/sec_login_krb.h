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
 * $Log: sec_login_krb.h,v $
 * Revision 1.1.2.2  1996/03/11  14:09:28  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:32  marty]
 *
 * Revision 1.1.2.1  1995/12/08  18:04:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:31:29  root]
 * 
 * $EndLog$
 */
#include <dce/nbase.h>

void sec_login_krb_get_ccache_version(
    int                 *version,       /* [out] */
    error_status_t      *stp            /* [out] */
);
