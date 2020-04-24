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
 * $Log: lbimpl.h,v $
 * Revision 1.1.4.2  1996/02/18  23:32:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:35  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:53:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:49:11  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:15:29  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:14  annie]
 * 
 * Revision 1.1.2.2  1994/05/27  16:31:15  rsalz
 * 	Fix bootstrap phase; cleanup EP and LLB compat code
 * 	[1994/05/27  16:30:25  rsalz]
 * 
 * Revision 1.1.2.1  1994/05/26  19:42:17  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:48  rsalz]
 * 
 * $EndLog$
 */

#include <lb.h>

/*
**  Delay (in seconds) between database cleanups.
*/
#define llb_c_live_long_wait		(60 * 30)

/*
**  Unlock database after this many deletions.
*/
#define llb_live_c_max_deletes		5

/*
**  Number of contexts.  Must fit into an unsigned16.
*/
#define llb_c_num_contexts		100

/*
**  How long to keep a context when we're out of them, and when we're not.
*/
#define llb_c_short_context		60
#define llb_c_long_context		(60 * 60 * 6)


/*
**  An NCS 1.5.1 lb_lookup_handle_t is 32 bits.  NCS 1.5.1 doesn't have
**  context_handle support, so we save lb_lookup_handles in a private
**  array.  The internal representation of the lb_lookup_handle_t contains
**  an offset into the context array and a magic cookie that changes each
**  time we give out a context to a client.  We never use offset 0 since
**  that could conflict with the "non-handle handle", lb_default_lookup_handle.
*/
#define LH_GET_OFFSET(lh)		(((lh) >> 8) & 0xFFFF)
#define LH_GET_COOKIE(lh)		(((lh)     ) & 0xFFFF)
#define LH_SET_COOKIE(lh, c)		(LH_GET_OFFSET(lh) | ((c) & 0xFFFF))
#define LH_SET(o, c)			((((o) & 0xFFFF) << 8) | ((c) & 0xFFFF))
typedef struct llb_context_s_t {
    boolean		free;
    lb_lookup_handle_t	llbh;
    ept_lookup_handle_t	epth;
    time_t		last_used;
} llb_context_t;
