/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_thdio_defs.h $Revision: 1.1.6.1 $ $Date: 1996/05/10 13:29:20 $
 */
/*
 * HISTORY
 * $Log: cma_thdio_defs.h,v $
 * Revision 1.1.6.1  1996/05/10  13:29:20  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:51 UTC  bissen
 * 	Merge select optimizations from jss_fix_close to HPDCE02.
 * 	[1994/09/26  17:40 UTC  jss  /main/HPDCE02/4]
 *
 * 	Fix for CHFts12442. Serialize close and select notification.
 * 	[1994/09/16  22:12 UTC  jss  /main/HPDCE02/jss_fix_close/3]
 *
 * 	Merge from jss_fix_close to HPDCE02.
 * 	[1994/09/15  19:22 UTC  jss  /main/HPDCE02/3]
 *
 * 	Fix for CHFts12442. Moved serial member of file_obj structure to
 * 	the end of the structure.
 * 	[1994/09/14  16:10 UTC  jss  /main/HPDCE02/jss_fix_close/2]
 *
 * 	Merge from branch jss_close_fix.
 * 	[1994/09/08  19:06 UTC  jss  /main/HPDCE02/2]
 *
 * 	Fix for CHFts12442. Close while I/O pending bug.
 * 	[1994/08/30  14:25 UTC  jss  /main/HPDCE02/jss_fix_close/1]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:48 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  19:09  truitt  /main/HPDCE01/1]
 *
 * 	re-merge kk and dce1_0_3
 * 	[1993/09/23  12:08:12  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:32  root  1.1.11.3]
 *
 * 	replace merged version with straight kk version for now
 * 	[1993/09/22  16:28:13  truitt]
 *
 * 	[hinman@sni] - Fix merge error
 * 	[1993/06/17  18:00:42  hinman]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:14:27  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  19:01:26  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:35:30  hopkins]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:56:39  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:28:53  osfrcs  1.1.1.8]
 *
 * 	DCE 1.0.1 Release
 * 	[1992/09/23  20:39:13  hopkins]
 *
 * Revision 1.1.2.2  1994/06/09  13:34:49  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:40  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/01  20:13:55  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:03:31  jd]
 * 
 * Revision 1.1.7.2  1993/06/24  21:20:02  hinman
 * 	[hinman@sni] - Fix merge error
 * 	[1993/06/17  18:00:42  hinman]
 * 
 * Revision 1.1.4.4  1993/02/01  22:31:31  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:33:11  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:22:50  hinman]
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

#ifdef __cplusplus
extern "C" {
#endif



/*
 * TYPE DEFINITIONS
 */

typedef struct CMA__T_FILE_OBJ {
    cma__t_int_mutex	*mutex;
	cma__t_int_cv   *io_wait[2];
    cma_t_integer	waiters[3];     /* number of outstanding operations */
    cma_t_integer	ref_count;	/* number of descriptors to this obj */
    cma_t_boolean	non_blocking;	/* to indicate if can be set to non_blocking */
    cma_t_boolean	set_non_blocking; /* to indicate if actually set to non_blocking */
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
	cma_t_boolean   fiosnbio;       /* ioctl non-blocking I/O */
	cma_t_boolean	user_non_blocking;  /* Summary, all 4 or'd together */
	} user_fl;
#ifdef _HP_CMA_ASYNC_CLOSE_
    cma_t_integer	serial;	        /* number of descriptors to this obj */
#endif
    }	cma__t_file_obj;

#ifdef  _HP_CMA_ASYNC_CLOSE_
typedef struct CMA__T_FO_ID {
    cma__t_file_obj  *fo;
    cma_t_integer    ser;
    } cma__t_fo_id;
#endif


/*
 * GLOBAL DATA
 */

extern cma__t_file_obj     **cma__g_file;
extern cma__t_file_obj     *cma__g_file_obj;
extern cma_t_integer       cma__g_file_num;
extern cma__t_file_mask    cma__g_wait_fds[3];
extern cma__t_int_mutex    *cma__g_io_data_mutex;
#ifdef  _HP_CMA_ASYNC_CLOSE_
extern cma__t_int_mutex    *cma__g_close_select_mutex;
#endif
extern cma__t_queue        cma__g_select_queue;

#ifdef __cplusplus
}
#endif

#endif






