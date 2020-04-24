/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_svc.h,v $
 * Revision 1.1.4.2  1996/02/18  23:01:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:41  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:29:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:25  root]
 * 
 * Revision 1.1.2.2  1994/08/25  18:07:43  sekhar
 * 	Add SEC_SVC_PRINT_MSG_ST macro .
 * 	[1994/08/25  18:06:57  sekhar]
 * 
 * Revision 1.1.2.1  1994/08/04  16:12:51  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:09:38  mdf]
 * 
 * 	HP revision /main/ODESSA_2/1  1994/07/18  19:11 UTC  sekhar
 * 	merge servicibility work.
 * 
 * 	HP revision /main/sekhar_svc_bl2/2  1994/06/30  01:05 UTC  sekhar
 * 	Include header files from export tree.
 * 
 * 	HP revision /main/sekhar_svc_bl2/1  1994/06/29  14:19 UTC  sekhar
 * 	SVC header file for "sec" component.
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */

#if !defined(_SEC_SVC_H)
#define _SEC_SVC_H

#include <dce/dce_svc.h>
#include <dce/dcesecsvc.h>
#include <dce/dcesecmac.h>

#define SEC_SVC_PRINT_MSG_ST(SVC_MSG, SVC_ST)                \
{                                                            \
    unsigned char *svc_msgp=NULL;                            \
                                                             \
    dce_svc_printf(SVC_MSG, svc_msgp=dce_sprintf(SVC_ST));   \
    if (svc_msgp)                                            \
        free(svc_msgp);                                      \
}

            
#endif                                  /* _SEC_SVC_H */
