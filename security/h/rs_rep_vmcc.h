/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rep_vmcc.h,v $
 * Revision 1.1.5.2  1996/02/18  22:59:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:06  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:27:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:29  root]
 * 
 * Revision 1.1.3.3  1992/12/29  13:25:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:31  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/07  20:27:26  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:25  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:40:34  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */
/* 
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - virtual memory credential cache
 */

#ifndef rs_rep_vmcc_h__included
#define rs_rep_vmcc_h__included

/* rs_rep_vmcc_get_ccache
 *
 * Acquire a handle on a virtual memory credential cache.  The
 * returned opaque pointer is actually a krb5_ccache handle and can 
 * be used as such in the krb5_cc_ interface.
 *
 * NOTE: This first implementation of the virtual memory credential
 *       cache is a very quick and dirty implementation containing the
 *       bare minimum required for authenticating propagations.  The VM
 *       cache can never contain more than 1 ticket at a time and so, is
 *       useless for any other purpose.
 */
void  * rs_rep_vmcc_get_ccache ( 
#ifdef __STDC__
    error_status_t  *status
#endif
);

#endif  /* #ifndef rs_rep_vmcc_h__included */

