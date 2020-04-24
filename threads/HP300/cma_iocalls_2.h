/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_iocalls_2.h,v $
 * Revision 1.1.6.2  1996/02/18  23:08:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:13  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:45:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:59:00  root]
 * 
 * Revision 1.1.4.1  1994/06/09  13:34:03  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:13  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:20:18  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:58:52  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef CMA_IOCALLS
#define CMA_IOCALLS

#undef close
#pragma _HP_SECONDARY_DEF _close close
#define cma_close _close
#define close _close_sys

#undef open
#pragma _HP_SECONDARY_DEF _open open
#define cma_open _open
#define open _open_sys

#undef read
#pragma _HP_SECONDARY_DEF _read read
#define cma_read _read
#define read _read_sys

#undef readv
#pragma _HP_SECONDARY_DEF _readv readv
#define cma_readv _readv
#define readv _readv_sys

#undef write
#pragma _HP_SECONDARY_DEF _write write
#define cma_write _write
#define write _write_sys

#undef writev
#pragma _HP_SECONDARY_DEF _writev writev
#define cma_writev _writev
#define writev _writev_sys

#endif
