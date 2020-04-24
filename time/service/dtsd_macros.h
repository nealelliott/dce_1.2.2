/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtsd_macros.h,v $
 * Revision 1.1.20.2  1996/02/18  23:12:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:37:23  marty]
 *
 * Revision 1.1.20.1  1995/12/08  18:13:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:43  root]
 * 
 * Revision 1.1.18.4  1994/06/10  20:52:31  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:55  devsrc]
 * 
 * Revision 1.1.18.3  1994/04/21  22:05:58  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:59:42  cbrooks]
 * 
 * 	Checkpoint:4.15.94
 * 	[1994/04/15  21:14:27  cbrooks]
 * 
 * 	Checkpoint
 * 	[1994/03/28  20:28:31  cbrooks]
 * 
 * 	DTSD 3-25-94 checkpoint
 * 	[1994/03/25  22:19:32  cbrooks]
 * 
 * Revision 1.1.18.2  1994/01/21  22:08:02  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:28  rsalz]
 * 
 * $EndLog$
 */

#ifdef	DCE_DEBUG
#define SHOW_BINDING(idx, str, handle)					\
    do									\
    if (DCE_SVC_DEBUG_ATLEAST(dts_svc_handle, idx, svc_c_debug1)) {	\
	unsigned char *_p = NULL; unsigned32 _st;			\
	rpc_binding_to_string_binding(handle, &_p, &_st);		\
	DCE_SVC_DEBUG((dts_svc_handle, idx, svc_c_debug1, "%s -> %s\n", str, _p));	\
	rpc_string_free(&_p, &_st);					\
    }									\
    while (0)
#else
#define SHOW_BINDING(idx, str, handle)	/* NULL */
#endif
