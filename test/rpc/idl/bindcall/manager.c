/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/17  23:12:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:51  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:58:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:47  root]
 * 
 * Revision 1.1.2.1  1994/05/02  22:43:46  tom
 * 	Initial checkin.
 * 	[1994/05/02  21:06:40  tom]
 * 
 * $EndLog$
 */
/*#include "test_com.h" */
#include "bindcall.h"

static void mgr_add(a, b, c)
idl_long_int a, b, *c;
{
    *c = a + b;
}

bindcall_v0_0_epv_t bindcall_v0_0_m_epv = {mgr_add};
