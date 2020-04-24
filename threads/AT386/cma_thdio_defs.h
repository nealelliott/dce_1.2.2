/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_thdio_defs.h,v $
 * Revision 1.1.5.2  1996/02/18  23:08:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:01  marty]
 *
 * Revision 1.1.5.1  1995/12/07  21:44:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:58:40  root]
 * 
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:33:11  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:22:50  hinman]
 * 
 * Revision 1.1.2.2  1994/06/09  13:33:45  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:24:58  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/01  20:13:18  jd
 * 	Moved from src/threads to here.
 * 	[1994/04/01  16:23:42  jd]
 * 
 * Revision 1.1.7.2  1993/06/24  21:20:02  hinman
 * 	[hinman@sni] - Fix merge error
 * 	[1993/06/17  18:00:42  hinman]
 * 
 * Revision 9.5.1.3  92/11/12  16:51:17  raj
 * 	<<<replace with log message for ./threads/cma_thdio_defs.h>>>
 * 
 * Revision 9.5.3.2  92/11/12  15:05:57  raj
 * 	<<<replace with log message for ./threads/cma_thdio_defs.h>>>
 * 
 * Revision 9.5.1.2  92/09/10  16:36:18  hinman
 * 	[hinman] - Fold forward from raj_om
 * 
 * Revision 9.5.2.2  92/09/10  16:10:59  raj
 * 	[raj] merging SVR4 changes into 1.0.2
 * 
 * Revision 9.9  92/09/02  14:09:39  hinman
 * 	Update checkin
 * 
 * Revision 1.1.2.3  1992/08/14  20:23:10  bolinger
 * 	Bring forward 1.0.1 revision 1.1.2.2 (fix for OT defect 2540).
 * 	[1992/08/10  16:31:49  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:04:48  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:04:34  bolinger]
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and  copied
 *  only  in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any  other
 *  copies  thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is  hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without  notice
 *  and  should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	CMA services
 *
 *  ABSTRACT:
 *
 *	Header file for thread synchrounous I/O
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 *
 *	1 May 1991
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	2 May 1991
 *		Removed unneeded includes.
 *	002	Dave Butenhof	19 May 1991
 *		Include fcntl.h to get O_NONBLOCK and O_NDELAY bits needed
 *		for structure definition.
 */


#ifndef CMA_THDIO_DEFS
#define CMA_THDIO_DEFS

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_int_errno.h>
#include <cma_thread_io.h>
#include <cma_queue.h>
#include <cma_mutex.h>
#include <cma_condition.h>
#include <fcntl.h>



/*
 * TYPE DEFINITIONS
 */

typedef struct CMA__T_FILE_OBJ {
    cma__t_int_mutex	*mutex;
	cma__t_int_cv   *io_wait[2];
    cma_t_integer	waiters[3];/* number of outstanding operations */
    cma_t_integer	ref_count;	/* number of descriptors to this obj */
    cma_t_boolean	non_blocking;	/* to indicate if can be set to non_blocking */
    cma_t_boolean	set_non_blocking; /* to indicate if actually set to non_blocking */
    cma_t_boolean	is_char_dev;	/* indicates if is char special file */
    struct {				/* The settings the user supplied... */
#ifdef O_NDELAY
	cma_t_boolean	o_ndelay;	/* ...for the "non-blocking" mode... */
#endif
#ifdef _CMA_FNDELAY_
	cma_t_boolean	fndelay;	/* ...file bits.  All four may not... */
#endif
#ifdef O_NONBLOCK
	cma_t_boolean	o_nonblock;	/* ...be distinct or even exist on... */
#endif
#ifdef _CMA_FNBLOCK_
	cma_t_boolean	fnblock;	/* ...all platforms. */
#endif
	cma_t_boolean	user_non_blocking;  /* Summary, all 4 or'd together */
	} user_fl;
    }	cma__t_file_obj;



/*
 * GLOBAL DATA
 */

extern cma__t_file_obj     **cma__g_file;
extern cma__t_file_obj     *cma__g_file_obj;
extern cma_t_integer       cma__g_file_num;
extern cma__t_file_mask    cma__g_wait_fds[3];
extern cma__t_int_mutex    *cma__g_io_data_mutex;
extern cma__t_queue        cma__g_select_queue;

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THDIO_DEFS.H */
/*  *7    17-SEP-1991 13:21:37 BUTENHOF "Handle variable # of FDs more efficiently" */
/*  *6    19-JUN-1991 14:08:12 BUTENHOF "Include fcntl.h for O_NONBLOCK & O_FNDELAY" */
/*  *5    11-JUN-1991 10:37:20 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *4    10-JUN-1991 19:22:03 BUTENHOF "Fix the sccs headers" */
/*  *3    10-JUN-1991 18:24:28 SCALES "Add sccs headers for Ultrix" */
/*  *2     2-MAY-1991 22:28:57 SCALES "Remove unneeded includes" */
/*  *1     2-MAY-1991 16:27:32 CURTIN "Thread synchronous I/O cont" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THDIO_DEFS.H */






