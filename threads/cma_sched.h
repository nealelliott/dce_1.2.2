/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_sched.h $Revision: 1.1.9.1 $ $Date: 1996/05/10 13:40:31 $
 */
/*
 * HISTORY
 * $Log: cma_sched.h,v $
 * Revision 1.1.9.1  1996/05/10  13:40:31  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:44 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:25  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:28  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  17:02  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  13:56:59  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:01  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	redefine "class" and "new" keywords
 * 	[1993/08/03  18:13:23  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:36:46  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:34:26  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:40:46  alan]
 *
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:50:25  sekhar]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:43  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:44:05  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:35:02  mgm  1.1.1.11]
 *
 * Revision 1.1.4.2  1994/06/09  13:38:36  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:25  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:03  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:28  jd]
 * 
 * Revision 1.1.2.4  1992/12/15  22:29:49  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:40:46  alan]
 * 
 * Revision 1.1.2.3  1992/09/29  20:15:22  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:50:25  sekhar]
 * 
 * Revision 1.1.2.2  1992/09/03  14:39:33  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:43  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:44:23  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:44:05  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(ULTRIX/OSF)	%G%
 */
/*
 *  Copyright (c) 1991 by
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
 *	Header file for priority scheduling
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	15 June 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	18 March 1991
 *		Reworked the "update-time" macro to remove floating point ops.
 *		Added general scale-up & scale-down macros.
 *	002	Dave Butenhof	03 April 1991
 *		Move "vp" definitions to cma_vp.h.
 *	003	Dave Butenhof	18 September 1991
 *		Merge Apollo CMA5 reverse drop: Apollo cc doesn't do constant
 *		folding correctly for shifts, so switch to arithmetic.
 *	004	Webb Scales	13 march 1992
 *		Parameterize scheduling policies.
 */


#ifndef CMA_SCHED
#define CMA_SCHED

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  INCLUDE FILES
 */

/*
 * CONSTANTS AND MACROS
 */

#ifdef __cplusplus
#define	class	c_class
#define	new	c_new
#endif

/*
 * Scaling factor for integer priority calculations
 */
#define cma__c_prio_scale   8

#if _CMA_VENDOR_ == _CMA__APOLLO
/*
 * FIX-ME: Apollo cc 6.8 blows contant folded "<<" and ">>"
 */
# define cma__scale_up(exp)  ((exp) * 256)
# define cma__scale_dn(exp)  ((exp) / 256)
#else
# define cma__scale_up(exp)  ((exp) << cma__c_prio_scale)
# define cma__scale_dn(exp)  ((exp) >> cma__c_prio_scale)
#endif


/*
 * Min. num. of ticks between self-adjustments for priority adjusting policies.
 */
#define cma__c_prio_interval	10


/*
 * Number of queues in each class of queues
 */
#define cma__c_prio_n_id    1	    /* Very-low-priority class threads */
#define cma__c_prio_n_bg    8	    /* Background class threads */
#define cma__c_prio_n_0	    1	    /* Very low priority throughput quartile */
#define cma__c_prio_n_1	    2	    /* Low priority throughput quartile */
#define cma__c_prio_n_2	    3	    /* Medium priority throughput quartile */
#define cma__c_prio_n_3	    4	    /* High priority throughput quartile */
#define cma__c_prio_n_rt    1	    /* Real Time priority queues */

/*
 * Number of queues to skip (offset) to get to the queues in this section of LA
 */
#define cma__c_prio_o_id 0
#define cma__c_prio_o_bg cma__c_prio_o_id + cma__c_prio_n_id
#define cma__c_prio_o_0  cma__c_prio_o_bg + cma__c_prio_n_bg
#define cma__c_prio_o_1  cma__c_prio_o_0  + cma__c_prio_n_0
#define cma__c_prio_o_2  cma__c_prio_o_1  + cma__c_prio_n_1
#define cma__c_prio_o_3  cma__c_prio_o_2  + cma__c_prio_n_2
#define cma__c_prio_o_rt cma__c_prio_o_3  + cma__c_prio_n_3

/*
 * Ada_low:  These threads are queued in the background queues, thus there
 * must be enough queues to allow one queue for each Ada priority below the
 * Ada default.
 */  
#define cma__c_prio_o_al cma__c_prio_o_bg

/*
 * Total number of ready queues, for declaration purposes
 */
#define cma__c_prio_n_tot  \
	cma__c_prio_n_id + cma__c_prio_n_bg + cma__c_prio_n_rt \
	+ cma__c_prio_n_0 + cma__c_prio_n_1 + cma__c_prio_n_2 + cma__c_prio_n_3

/*
 * Formulae for determining a thread's priority.  Variable priorities (such
 * as foreground and background) are scaled values.
 */
#define cma__sched_priority(tcb)	\
    ((tcb)->sched.class == cma__c_class_fore  ? cma__sched_prio_fore (tcb)  \
    :((tcb)->sched.class == cma__c_class_back ? cma__sched_prio_back (tcb)  \
    :((tcb)->sched.class == cma__c_class_rt   ? cma__sched_prio_rt (tcb)    \
    :((tcb)->sched.class == cma__c_class_idle ? cma__sched_prio_idle (tcb)  \
    :(cma__bugcheck ("cma__sched_priority: unrecognized class"), 0) ))))

#define cma__sched_prio_fore(tcb)	cma__sched_prio_fore_var (tcb)
#define cma__sched_prio_back(tcb)	((tcb)->sched.fixed_prio	\
	? cma__sched_prio_back_fix (tcb) : cma__sched_prio_back_var (tcb) )
#define cma__sched_prio_rt(tcb)		((tcb)->sched.priority)
#define cma__sched_prio_idle(tcb)	((tcb)->sched.priority)

#define cma__sched_prio_back_fix(tcb)	\
	(cma__g_prio_bg_min + (cma__g_prio_bg_max - cma__g_prio_bg_min) \
	* ((tcb)->sched.priority + cma__c_prio_o_al - cma__c_prio_o_bg) \
	/ cma__c_prio_n_bg)

/*
 * FIX-ME: Enable after modeling (if we like it)
 */
#if 1
# define cma__sched_prio_fore_var(tcb)  \
	((cma__g_prio_fg_max + cma__g_prio_fg_min)/2)
# define cma__sched_prio_back_var(tcb)  \
	((cma__g_prio_bg_max + cma__g_prio_bg_min)/2)
#else
# define cma__sched_prio_back_var(tcb)  cma__sched_prio_fore_var (tcb)

# if 1
/*
 * Re-scale, since the division removes the scale factor.
 * Scale and multiply before dividing to avoid loss of precision.
 */
#  define cma__sched_prio_fore_var(tcb)  \
	((cma__g_vp_count * cma__scale_up((tcb)->sched.tot_time)) \
	/ (tcb)->sched.cpu_time)
# else
/*
 * Re-scale, since the division removes the scale factor.
 * Scale and multiply before dividing to avoid loss of precision.
 * Left shift the numerator to multiply by two.
 */
#  define cma__sched_prio_fore_var(tcb)  \
    (((cma__g_vp_count * cma__scale_up((tcb)->sched.tot_time)  \
    * (tcb)->sched.priority * cma__g_init_frac_sum) << 1)  \
    / ((tcb)->sched.cpu_time * (tcb)->sched.priority * cma__g_init_frac_sum  \
	+ (tcb)->sched.tot_time))
# endif
#endif

/*
 * Update weighted-averaged, scaled tick counters
 */
#define cma__sched_update_time(ave, new) \
    (ave) = (ave) - ((cma__scale_dn((ave)) - (new)) << (cma__c_prio_scale - 4))

#define cma__sched_parameterize(tcb, policy) { \
    switch (policy) { \
	case cma_c_sched_fifo : { \
	    (tcb)->sched.rtb =		cma_c_true; \
	    (tcb)->sched.spp =		cma_c_true; \
	    (tcb)->sched.fixed_prio =	cma_c_true; \
	    (tcb)->sched.class =	cma__c_class_rt; \
	    break; \
	    } \
	case cma_c_sched_rr : { \
	    (tcb)->sched.rtb =		cma_c_false; \
	    (tcb)->sched.spp =		cma_c_true; \
	    (tcb)->sched.fixed_prio =	cma_c_true; \
	    (tcb)->sched.class =	cma__c_class_rt; \
	    break; \
	    } \
	case cma_c_sched_throughput : { \
	    (tcb)->sched.rtb =		cma_c_false; \
	    (tcb)->sched.spp =		cma_c_false; \
	    (tcb)->sched.fixed_prio =	cma_c_false; \
	    (tcb)->sched.class =	cma__c_class_fore; \
	    break; \
	    } \
	case cma_c_sched_background : { \
	    (tcb)->sched.rtb =		cma_c_false; \
	    (tcb)->sched.spp =		cma_c_false; \
	    (tcb)->sched.fixed_prio =	cma_c_false; \
	    (tcb)->sched.class =	cma__c_class_back; \
	    break; \
	    } \
	case cma_c_sched_ada_low : { \
	    (tcb)->sched.rtb =		cma_c_false; \
	    (tcb)->sched.spp =		cma_c_true; \
	    (tcb)->sched.fixed_prio =	cma_c_true; \
	    (tcb)->sched.class =	cma__c_class_back; \
	    break; \
	    } \
	case cma_c_sched_idle : { \
	    (tcb)->sched.rtb =		cma_c_false; \
	    (tcb)->sched.spp =		cma_c_false; \
	    (tcb)->sched.fixed_prio =	cma_c_false; \
	    (tcb)->sched.class =	cma__c_class_idle; \
	    break; \
	    } \
	default : { \
	    cma__bugcheck ("cma__sched_parameterize: bad scheduling Policy"); \
	    break; \
	    } \
	} \
    }

/*
 * TYPEDEFS
 */

/*
 * Scheduling classes
 */
typedef enum CMA__T_SCHED_CLASS {
    cma__c_class_rt,
    cma__c_class_fore,
    cma__c_class_back,
    cma__c_class_idle
    } cma__t_sched_class;

/*
 *  GLOBAL DATA
 */

/*
 * Minimuma and maximum prioirities, for foreground and background threads,
 * as of the last time the scheduler ran.  (Scaled once.)
 */
extern cma_t_integer	cma__g_prio_fg_min;
extern cma_t_integer	cma__g_prio_fg_max;
extern cma_t_integer	cma__g_prio_bg_min;
extern cma_t_integer	cma__g_prio_bg_max;

/*
 * The "m" values are the slopes of the four sections of linear approximation.
 *
 * cma__g_prio_m_I = 4*N(I)/cma__g_prio_range	    (Scaled once.)
 */
extern cma_t_integer	cma__g_prio_m_0,
		    	cma__g_prio_m_1,
		    	cma__g_prio_m_2,
		    	cma__g_prio_m_3;

/* 
 * The "b" values are the intercepts of the four sections of linear approx.
 *  (Not scaled.)
 *
 * cma__g_prio_b_I = -N(I)*(I*prio_max + (4-I)*prio_min)/prio_range + prio_o_I
 */
extern cma_t_integer	cma__g_prio_b_0,
		    	cma__g_prio_b_1,
		    	cma__g_prio_b_2,
		    	cma__g_prio_b_3;

/* 
 * The "p" values are the end points of the four sections of linear approx.
 *
 * cma__g_prio_p_I = cma__g_prio_fg_min + (I/4)*cma__g_prio_range
 *
 * [cma__g_prio_p_0 is not defined since it is not used (also, it is the same
 *  as cma__g_prio_fg_min).]	    (Scaled once.)
 */
extern cma_t_integer	cma__g_prio_p_1,
		    	cma__g_prio_p_2,
		    	cma__g_prio_p_3;

/*
 * Points to the next queue for the dispatcher to check for ready threads.
 */
extern cma_t_integer	cma__g_next_ready_queue;

/*
 * Points to the queues of virtual processors (for preempt victim search)
 */
extern cma__t_queue	cma__g_run_vps;
extern cma__t_queue	cma__g_susp_vps;
extern cma_t_integer	cma__g_vp_count;

/*
 * INTERNAL INTERFACES
 */

#ifdef __cplusplus
#undef	class
#undef	new
#endif

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_SCHED.H */
/*  *8    18-MAR-1992 19:01:29 SCALES "Parameterize scheduling policies" */
/*  *7    24-SEP-1991 16:27:48 BUTENHOF "Merge CMA5 reverse IBM/HP/Apollo drops" */
/*  *6    10-JUN-1991 19:55:24 SCALES "Convert to stream format for ULTRIX build" */
/*  *5    10-JUN-1991 19:21:23 BUTENHOF "Fix the sccs headers" */
/*  *4    10-JUN-1991 18:23:07 SCALES "Add sccs headers for Ultrix" */
/*  *3    12-APR-1991 23:36:32 BUTENHOF "Implement VP layer to support Mach threads" */
/*  *2    28-MAR-1991 17:22:30 SCALES "Improve variable priority dispatch" */
/*  *1    12-DEC-1990 21:51:58 BUTENHOF "Scheduling" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_SCHED.H */
