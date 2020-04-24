/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dce_error.h,v $
 * Revision 1.1.6.1  1996/09/09  20:58:27  bfc
 * 	Insert C++ wrapper
 * 	[1996/09/09  20:52:12  bfc]
 *
 * Revision 1.1.4.2  1996/02/18  23:33:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:06  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  21:37:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:23  root]
 * 
 * Revision 1.1.2.4  1994/09/07  21:04:59  bowe
 * 	Add dce_cma_error_inq_text() prototype. [CR 11885]
 * 	[1994/09/07  21:03:46  bowe]
 * 
 * Revision 1.1.2.3  1994/06/09  16:05:51  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:27  devsrc]
 * 
 * Revision 1.1.2.2  1994/01/05  14:27:43  rsalz
 * 	Remove dependency on nbase.idl for now.
 * 	[1994/01/05  14:27:31  rsalz]
 * 
 * Revision 1.1.2.1  1994/01/03  19:35:05  rsalz
 * 	#include <dce/nbase.h> for error_status_t.
 * 	[1994/01/03  18:06:51  rsalz]
 * 
 * 	Created from rpc/sys_idl version.
 * 	[1994/01/03  15:33:12  rsalz]
 * 
 * $EndLog$
 */
#if	!defined(DCE_ERROR_H)
#define DCE_ERROR_H
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**      Contains public DCE error facility definitions. 
*/

#ifdef  __cplusplus
	extern "C" {
#endif

/*
**  Maximum length of returned string.
*/
#define dce_c_error_string_len	160


/*
**  An error message buffer.
*/
typedef unsigned char		dce_error_string_t[dce_c_error_string_len];


/*
**  Function to map status to text.  Return status is 0 for success, -1
**  for failure.  It's deliberately not a DCE error code, to discourage
**  recursion.  Note also that status_to_convert is an IDL long, 32 bits.
*/
extern void dce_error_inq_text(
    unsigned long	/* status_to_convert */,
    unsigned char*	/* buffer */,
    int*		/* status */
);

/* same as above, but for use within the cma/threads code */
extern void dce_cma_error_inq_text(
    unsigned long	/* status_to_convert */,
    unsigned char*	/* buffer */,
    int*		/* status */
);

#ifdef  __cplusplus
	}
#endif

#endif	/* !defined(DCE_ERROR_H) */
