/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dce_utils.h,v $
 * Revision 1.1.4.2  1996/02/18  23:32:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:04  root]
 * 
 * Revision 1.1.2.6  1994/08/28  14:40:27  rsalz
 * 	Add dce_gettimeofday (OT CR 11931).
 * 	[1994/08/28  14:20:08  rsalz]
 * 
 * Revision 1.1.2.5  1994/07/27  19:19:55  rsalz
 * 	Add bvfile (OT CR 11466).
 * 	[1994/07/27  19:18:46  rsalz]
 * 
 * Revision 1.1.2.4  1994/07/27  12:27:53  rsalz
 * 	Silence SVC compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:33:17  rsalz]
 * 
 * Revision 1.1.2.3  1994/07/18  14:50:29  rsalz
 * 	Add dce_strdup (OT CR 11341).
 * 	[1994/07/18  14:49:04  rsalz]
 * 
 * Revision 1.1.2.2  1994/06/29  13:15:05  melman
 * 	style suggestions from John Bowe
 * 	[1994/06/29  12:14:43  melman]
 * 
 * Revision 1.1.2.1  1994/06/22  17:51:40  bowe
 * 	initial checkin
 * 	[1994/06/22  16:48:21  bowe]
 * 
 * $EndLog$
 */

/*
 *  Miscellaneous DCE utilities - #defines, data types, and function prototyes.
 *  This is included by both the library .c files and applications that
 *  use them.
 */

#include <dce/dcelibmsg.h>

/* For dce_get_802_addr() [on some platforms] */
#ifndef IEEE_802_FILE
#define IEEE_802_FILE   "/etc/ieee_802_addr"
#endif

#define PCS_CHARS " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

typedef struct dce_802_addr_s_t {
    unsigned_char_t	eaddr[6];
} dce_802_addr_t;

void dce_get_802_addr(dce_802_addr_t*, error_status_t*);
int isdcepcs(int);
char *dce_strdup(char*);
int dce_rpmatch(char*);
typedef struct timeval *dce_timeval_t;
int dce_gettimeofday(dce_timeval_t);

#if	defined(EOF)
void dce_bindings_to_file(FILE*, rpc_binding_vector_t*, error_status_t*);
void dce_bindings_from_file(FILE*, rpc_binding_vector_t**, error_status_t*);
#endif	/* defined(EOF) */
