/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: hdimpl.h,v $
 * Revision 1.1.2.2  1996/02/18  23:32:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:22  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:18  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/19  15:30 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 	[1995/12/08  20:48:55  root]
 * 
 * Revision 1.1.2.4  1994/08/28  00:58:37  rsalz
 * 	hd_post_proc passes in the item name through dced_fork (OT CR 11895).
 * 	[1994/08/26  19:14:05  rsalz]
 * 
 * Revision 1.1.2.3  1994/04/09  21:22:50  bowe
 * 	Added include <sys/stat.h>.
 * 	[1994/04/09  21:20:58  bowe]
 * 
 * Revision 1.1.2.2  1994/03/11  15:00:06  bowe
 * 	Make #include's consistent. Fix some lint warnings
 * 	[1994/03/11  14:56:00  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:48  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:19  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#ifndef _HDIMPL_H
#define _HDIMPL_H

#include <fcntl.h>
#include <sys/stat.h>

/* hostdata data structures */

typedef struct hd_postproc_s_t {
    uuid_t		uuid;
    char		*cmd;
} hd_postproc_t;

/* hostdata function prototypes */

void	hd_post_process(dced_entry_t *, error_status_t *);

#endif
