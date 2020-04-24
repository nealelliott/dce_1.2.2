/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_vp_defs.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:44:54 $
 */
/*
 * HISTORY
 * $Log: cma_vp_defs.h,v $
 * Revision 1.1.8.1  1996/05/10  13:44:54  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:47 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:46 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:59  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:40  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  15:20  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  15:44:37  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:45:04  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:15:19  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  19:20:37  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:36:24  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:44:30  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:08:15  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:17:01  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:36:32  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:39:13  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:59  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:56  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:40  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:33:04  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:44:30  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:42:46  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:08:15  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:17:16  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:17:01  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(DEC OSF/1)	%G%
 */
/*
 *  Copyright (c) 1991, 1992 by
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
 *  and  should  not  be  construed  as  a commitment by Digital Equipment
 *  Corporation.
 *
 *  Digital assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by Digital.
 */

/*
 *  FACILITY:
 *
 *	DECthreads services
 *
 *  ABSTRACT:
 *
 *	Header file for virtual processor services
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	03 April 1991
 *
 *  MODIFICATION HISTORY:
 * 
 *	001	Dave Butenhof	24 May 1991
 *		Change "history" value into flags word for flexibility.
 *	002	Dave Butenhof	02 June 1991
 *		Add new return status for trying to delete a VP that's not
 *		suspended.
 *	003	Dave Butenhof	11 September 1991
 *		Add support for caching VPs to improve performance.
 *	004	Dave Butenhof	08 February 1992
 *		Rework state flags for better debugging.
 *	005	Webb Scales	13 March 1992
 *		Move VP structure here from cma_sched.h
 */

#ifndef CMA_VP_DEFS
#define CMA_VP_DEFS

/*
 *  INCLUDE FILES
 */

#if _CMA_KTHREADS_ == _CMA__MACH
# include <mach.h>
# include <mach/message.h>
#endif

#include <cma_queue.h>
#include <cma_tcb_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * Define return status values
 */
#if _CMA_KTHREADS_ == _CMA__MACH
# define cma__c_vp_normal	KERN_SUCCESS
# define cma__c_vp_err_resource	KERN_RESOURCE_SHORTAGE
# define cma__c_vp_err_inval	KERN_INVALID_ARGUMENT
# define cma__c_vp_err_timeout	RCV_TIMED_OUT
#else
# define cma__c_vp_normal	0	/* Completed OK */
# define cma__c_vp_err_resource	1	/* Resource shortage (create) */
# define cma__c_vp_err_inval	2	/* Invalid parameter */
# define cma__c_vp_err_timeout	3	/* Timeout */
#endif

#define cma__c_vp_err_badstate	-500

#if _CMA_KTHREADS_ == _CMA__MACH
/*
 * Code for synchronization message code (VP suspend/resume)
 */
# define cma__c_vp_msg_resume	0xACEFACE
# define cma__c_vp_msg_kernel	0xBEEFED
#endif

#if _CMA_THREAD_IS_VP_
# define cma__count_vp_blocked() (cma__g_vp_running--)
# define cma__count_vp_running() (cma__g_vp_running++)
# define cma__try_run(tcb)		/* Don't waste time doing the call */
#else
# define cma__count_vp_blocked()
# define cma__count_vp_running()
#endif

/*
 * TYPEDEFS
 */

typedef int	cma__t_vp_status;
 typedef cma_t_address 	(*cma__t_vp_handler)  (cma_t_address);
 typedef cma_t_address 	(*cma__t_vp_startroutine)  (cma_t_address);

#if _CMA_KTHREADS_ == _CMA__MACH
# define cma__c_vp_new		0x00000001
# define cma__c_vp_running	0x00000002
# define cma__c_vp_susp		0x00000004
# define cma__c_vp_cached	0x00000008
# define cma__c_vp_default	0x00010000
# define cma__c_vp_hold		0x00020000

typedef struct CMA__T_VPID {
    cma__t_queue	queue;		/* Queue of VPs */
    thread_t		vp;		/* Actual Mach VP id */
    port_t		synch;		/* Synch port */
    cma_t_integer	flags;		/* What's it doing */
    } cma__t_vstruct, *cma__t_vpid;
#else
typedef cma_t_address 	cma__t_vpid;
#endif

#define cma__c_vp_st_run	1
#define cma__c_vp_st_stop	2
#define cma__c_vp_st_susp	3
#define cma__c_vp_st_hold	4
#define cma__c_vp_st_unks	5
#define cma__c_vp_st_unkw	6
#define cma__c_vp_st_unk	7

typedef struct CMA__T_VP_STATE {
    int		policy;
    int		priority;
    int		tcb;
    int		start_routine;
    int		start_arg;
    int		stack;
    int		run_state;
#if _CMA_KTHREADS_ == _CMA__MACH
    int		mach_state;
#endif
    int		suspend_count;
    } cma__t_vp_state;

/*
 * List of known virtual processors' per-processor data structures
 */
typedef struct CMA__T_VP {
    cma__t_queue	queue;
    cma__t_queue	interrupts;
    cma__t_int_tcb	*current_thread;
    cma__t_vpid		vp_id;
    } cma__t_vp;

/*
 *  GLOBAL DATA
 */

extern cma__t_queue	cma__g_run_vps;		/* Currently running processors */
extern cma__t_queue	cma__g_susp_vps;	/* Currently suspended processors */
extern cma_t_integer	cma__g_vp_count;	/* Current number of processors */
extern cma_t_integer	cma__g_vp_running;	/* Number of running processors */

/*
 * INTERNAL INTERFACES
 */

#ifdef __cplusplus
}
#endif

#endif
