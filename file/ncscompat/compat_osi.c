/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (c) 1995 Transarc Corporation
 * All rights reserved
 */
/*
 * HISTORY
 * $Log: compat_osi.c,v $
 * Revision 1.1.11.1  1996/10/02  17:54:32  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:17  damon]
 *
 * $EndLog$
 */
#include <pthread.h>
#include <compat.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsncsmac.h>
#include <dfsncssvc.h>
#include <dfsncsmsg.h>

extern int  compat_svc_must_init;
extern void compat_svc_init();

#define INIT_SVC if (compat_svc_must_init) compat_svc_init()

void afsos_panic(a) 
char *a;
{
    INIT_SVC;
    dce_svc_printf(NCS_S_CO_PANIC_MSG, a);
    exit(1);
}
