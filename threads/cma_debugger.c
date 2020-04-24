/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_debugger.c,v $
 * Revision 1.1.11.2  1996/02/17  23:33:50  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:01  marty]
 *
 * Revision 1.1.11.1  1995/12/07  22:01:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:05:17  root]
 * 
 * Revision 1.1.9.2  1994/06/09  13:37:44  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:41  devsrc]
 * 
 * Revision 1.1.9.1  1994/04/01  20:15:59  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:58  jd]
 * 
 * Revision 1.1.4.3  1992/12/15  22:27:25  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:36  alan]
 * 
 * Revision 1.1.4.2  1992/09/03  14:36:16  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:36  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:21:14  bolinger
 * 	Replace "DECThreads" with "DCE Threads" wherever it might be
 * 	visible to a programmer or application.
 * 	[1992/08/10  15:19:46  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:08:11  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:07:49  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/14  19:58:21  keane
 * 	Changed I/O function calls to use new I/O redirection stuff.
 * 	Slight improvements to formatted output.
 * 	[1992/05/11  20:29:49  keane]
 * 
 * Revision 1.1  1992/01/19  14:40:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *  Copyright (c) 1989, 1992 by
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
 *	Debug functions
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	Halloween 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	1 November 1989
 *		Enhance debugging: make external entry a parser.
 *	002	Webb Scales	3 November 1989
 *		Remove semaphore.lock field references
 *	003 	Bob Conti	4 November 1989
 *		Remove superfluous include of cma_host
 *	004	Webb Scales	19 November 1989
 *		Changed thd->thread_context.SP to thd->static_ctx.sp
 *	005	Dave Butenhof	20 November 1989
 *		Add some commands and switches
 *	006	Dave Butenhof	22 November 1989
 *		Use new "owner" field of mutex to report current owner.
 *	007	Dave Butenhof	1 December 1989
 *		Add scheduling policy to thread output.
 *	008	Dave Butenhof	4 December 1989
 *		Fix unconditional include of (VMS-only) stdlib.h
 *	009	Dave Butenhof	5 December 1989
 *		Remove initial_test attribute.
 *	010	Dave Butenhof	11 December 1989
 *		Change wording of thread display (alert "disabled" to "asynch
 *		disabled").
 *	011	Dave Butenhof	9 February 1990
 *		Enhance thread report to include full stack information.
 *	012	Dave Butenhof	12 February 1990
 *		Fix pointer arithmetic compiler complaint introduced by 011.
 *	013	Dave Butenhof	15 February 1990
 *		Handle default stack as special case (no stack object, so
 *		don't try to look at it).
 *	014	Dave Butenhof	26 February 1990
 *		Add more information for null thread.
 *	015	Dave Butenhof	26 February 1990
 *		Enhance stack information, handle new policy symbols.
 *	016	Dave Butenhof	9 April 1990
 *		Use new known object structures, with locks.
 *	017	Dave Butenhof	14 June 1990
 *		Replace cma__test_and_set by cma__kernel_set, and cma__unset
 *		by cma__kernel_unset, to match redefinitions done to allow
 *		mutex optimizations.
 *	018	Dave Butenhof	18 June 1990
 *		Use macro to test value of name field (doesn't exist for
 *		NDEBUG build).
 *	019	Webb Scales	15 June 1990
 *		Added priority scheduling, rearranged part of the tcb.
 *	020	Dave Butenhof	27 June 1990
 *		Support friendly mutexes.
 *	021	Dave Butenhof	6 July 1990
 *		On machines without interlocked instructions, locking a mutex
 *		involves entering the kernel, but the debugger runs with the
 *		kernel locked to prevent timeslices and such.  Therefore, on
 *		such machines, listing a friendly mutex (which attempts to
 *		lock the mutex's internal mutex) hangs. "Fix" this by reading
 *		the nesting count without locking the internal mutex and
 *		hoping for the best (it's OK on uniprocessors).
 *	022	Webb Scales	26 July 1990
 *		Added policy strings for new scheduling policies.
 *	023	Dave Butenhof	03 August 1990
 *		Fix semaphore stuff
 *	024	Paul Curtin	06 August 1990
 *		Replaced gets, strlen, strncmp, and strtok with  cma__*
 *		versions.  Replace printf with cma__put* functions.
 *	025	Webb Scales	30 August 1990
 *		Removed the put_eol from the debug prompt, and added a prompt
 *		string to _gets.
 *	026	Webb Scales	4 September 1990
 *		Fixed policy to print as a string instead of an int.
 *	027	Dave Butenhof	25 October 1990
 *		Fix up print formatting.
 *	028	Dave Butenhof	14 December 1990
 *		Change cma_attributes.h to cma_attr.h (shorten length)
 *	029	Dave Butenhof	7 February 1991
 *		Changes to alert state bits.
 *	030	Dave Butenhof	12 February 1991
 *		Support nonrecursive mutexes, and change from "friendly" to
 *		"recursive".
 *	031	Dave Butenhof	4 March 1991
 *		Add new command 'set', and new thread selection switches '-h'
 *		for hold, '-n' for "nohold", '-s<exp>' for priority
 *	032	Dave Butenhof	11 March 1991
 *		Add thread -f for brief, and display thread using
 *		cma__deb_show_thread function.
 *	033	Dave Butenhof	12 March 1991
 *		Fix MIPS CC compilation warnings.
 *	034	Webb Scales	14 March 1991
 *		Fix more MIPS CC compilation warnings.
 *	035	Dave Butenhof	18 March 1991
 *		Make several static functions extern so that Ada debug code
 *		can chain on ours. Also add a switch to "set" to act on all
 *		known threads (primarily for hold/nohold).
 *	036	Dave Butenhof	09 April 1991
 *		Don't reference "atomic bits" directly
 *	037	Dave Butenhof	16 May 1991
 *		Add new cma_debug_cmd entry to take command lines as args.
 *	038	Dave Butenhof	21 June 1991
 *		Add casts to cma__puthex calls.
 *	039	Dave Butenhof	01 July 1991
 *		Add mutex type to attribute object display.
 *	040	DECthreads team	    22 July 1991
 *		Changed calls to atol to cma__atol
 *	041	Dave Butenhof	18 September 1991
 *		Modify "thread -i -f" display to show the internal synch.
 *		objects. Also don't try to format "processor" structure
 *		(for "-v") unless it actually exists.
 *	042	Dave Butenhof	19 September 1991
 *		Integrate HPUX CMA5 reverse drop: some additional type casts
 *		make for a happy compiler.
 *	043	Dave Butenhof	04 October 1991
 *		Clean up use of _CMA_UNIPROCESSOR_
 *	044	Dave Butenhof	10 October 1991
 *		Add hooks to print out trace array info (if built) and VM
 *		stats. Change "set" command to "tset" (thread set) since it
 *		hasn't been documented yet, and I'd prefer to save "set" for
 *		more general setup (like to make nonrecursive mutexes the
 *		default, or turn on core-dump-on-synch-term-signal mode).
 *	045	Dave Butenhof	10 December 1991
 *		Fix bug in stack display on "thread -i": for current thread,
 *		it reported value in static_context structure rather than
 *		current live SP.
 *	046	Dave Butenhof	10 February 1992
 *		Add new global cma__g_sbrk_align to memory management report
 *		(number of bytes lost to align sbrk chunks).
 *	047	Dave Butenhof	05 March 1992
 *		Add "show -v" to call cma__vp_dump(). Enhance "thread -i -v"
 *		to show "real" vp state (cma__vp_get_state()).
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_dispatch.h>
#include <cma_tcb_defs.h>
#include <cma_condition.h>
#include <cma_semaphore.h>
#include <cma_mutex.h>
#include <cma_queue.h>
#include <cma_attr.h>
#include <cma_stack.h>
#include <cma_debugger.h>
#include <cma_deb_core.h>
#include <cma_util.h>
#include <cma_vp.h>
#include <cma_assem.h>
#include <cma_vm.h>
#include <stdarg.h>

/*
 * LOCAL MACROS
 */

/*
 * Flags for thread display bitmask
 */
#define DISPLAY_FULL		0x00000001
#define DISPLAY_VP		0x00000002

#define name_string(obj) \
    (cma__obj_null_name(obj) ? user_obj : (obj)->header.name)

#define cma___test_mutex(m) \
    (cma__tac_isset (&(m)->lock) \
	? ((m)->mutex_kind != cma_c_mutex_fast \
	    ? (m)->owner != (cma__t_int_tcb *)cma_c_null_ptr \
	    : cma_c_true) \
	: cma_c_false)

#define cma___intsize(value) ((value)<10?1:((value)<100?2: \
    ((value)<1000?3:((value)<10000?4:((unsigned long)(value)<100000?5:6)))))

/*
 * LOCAL DATA
 */

static long int		active_regs[17] = {0};

static cma__t_string	user_obj = "<USER>";

static cma__t_string	enabled_string = "enabled";
static cma__t_string	disabled_string = "disabled";

typedef enum CMA___T_CMD_TYPE {
    cma___c_cmd_normal,
    cma___c_cmd_modifier,
    cma___c_cmd_hidden,
    cma___c_cmd_exit
    } cma___t_cmd_type;

typedef struct CMA___T_CMD_INFO {
    char		*verb;
    cma___t_cmd_type	type;
    void		(*routine) (int argc, char *argv[]);
    char		*desc;
    } cma___t_cmd_info;

/*
 * Caution: The layout of this must match cma_t_state in cma_tcb_defs.h
 */
static char 	*state[4] = {
    "running",
    "ready",
    "blocked",
    "terminated",
    };

static char	*status[3] = {
    "error",
    "normal",
    "alert"
    };

#define cma___c_num_policies 6
static char	*policy[cma___c_num_policies] = {
    "fifo",
    "rr",
    "throughput",
    "background",
    "ada_low",
    "idle"
    };

#define cma___c_num_pris 3
static char	*pri_names[cma___c_num_pris] = {
    "min",
    "mid",
    "max"
    };

static char	*mutex_type[3] = {
    "fast",
    "recursive",
    "nonrecursive"
    };

static char	*vm_queue[4] = {
    "small",
    "medium",
    "large",
    "other"
    };

#if !_CMA_UNIPROCESSOR_ && (_CMA_OSIMPL_ == _CMA__OS_OSF)
static char	*vp_state[8] = {
    "<unused>",				/* state 0 unused */
    "run",
    "stop",
    "susp",
    "hold",
    "<unknown-state>",
    "<unknown-wait>",
    "<unknown>"
    };

static char	*mach_state[6] = {
    "<unused>",				/* state 0 unused */
    "running",
    "stopped",
    "waiting",
    "uninterruptible",
    "halted"
    };
#endif

static cma_t_priority	pri_range[cma___c_num_pris][cma___c_num_policies] = {
    {cma_c_prio_fifo_min, cma_c_prio_rr_min, cma_c_prio_through_min,
	cma_c_prio_back_min, cma_c_prio_ada_low_min, 0},
    {cma_c_prio_fifo_mid, cma_c_prio_rr_mid, cma_c_prio_through_mid,
	cma_c_prio_back_mid, cma_c_prio_ada_low_mid, 0},
    {cma_c_prio_fifo_max, cma_c_prio_rr_max, cma_c_prio_through_max,
	cma_c_prio_back_max, cma_c_prio_ada_low_max, 0}
    };


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Debugging routine: parse commands and dispatch to execution code.
 *
 *  FORMAL PARAMETERS:
 *
 *	cmd	Command line string (may be a list, ending with 0).
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	Many and varied.
 */
extern void
cma_debug_cmd(
	char	*cmd,
	...)
    {
    cma_t_boolean	lock;
    va_list		ap;
    char		*cmdline;


    va_start (ap, cmd);
    cmdline = cmd;

    lock = cma__debug_trylock ();

    while (cmdline != (char *)cma_c_null_ptr) {
	if (cma__debug_parse (cmdline, lock)) {

	    if (!lock)
		cma__unset_kernel ();

	    return;
	    }

	cmdline = va_arg (ap, char*);
	}

    if (!lock)
	cma__unset_kernel ();

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Main debugging routine: parse commands and dispatch to execution code.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	Many and varied.
 */
extern void
cma_debug(void)
    {
    char			buffer[512];
    char			*ptr;
    cma_t_boolean		locked;


    /*
     * Lock the kernel, so we don't get timesliced out while we're running
     * the debug loop.
     */
    locked = cma__debug_trylock ();

    while (1) {
	ptr = (cma_t_address) cma__gets ("DCE Threads debug> ", &buffer[0]);

	if (ptr == cma_c_null_ptr) {
	    /*
	     * If EOF, then unlock the kernel (if we actually locked it), and
	     * return.
	     */
	    if (!locked)
		cma__unset_kernel ();

	    return;
	    }

	if (cma__strlen (ptr) == 0)
	    continue;

	if (cma__debug_parse (ptr, locked)) {

	    if (!locked)
		cma__unset_kernel ();

	    return;
	    }

	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Format and display a TCB
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb		Address of TCB
 *	display_flags	Bitmask of flags to control display
 *
 *  IMPLICIT INPUTS:
 *
 *	arrays to decode enum types.
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern void
cma__debug_format_thread(
	cma__t_int_tcb	*thd,
	cma_t_integer	display_flags)
    {
    char			output[cma__c_buffer_size];

    output[0] = '\0';
    cma__putstring  (output, "Thread ");
    cma__putstring  (output, name_string(thd));
    cma__putstring  (output, " (");
    cma__puthex	    (output, (cma_t_integer) thd);
    cma__putstring  (output, ") ");
    cma__putint	    (output, thd->header.sequence);
    cma__putstring  (output, " : ");
    cma__putstring  (output, state[(int)thd->state]);
    cma__puteol	    (output);

    if ((display_flags & (DISPLAY_VP | DISPLAY_FULL)) != 0) {

	if (thd->sched.processor == (cma__t_vp *)cma_c_null_ptr) {
	    cma__putstring (output, "  No current vp");
	    cma__puteol (output);
	    }
	else {
#if !_CMA_UNIPROCESSOR_ && (_CMA_OSIMPL_ == _CMA__OS_OSF)
	    cma__t_vp_state	state;
	    cma__t_vp_status	st;


	    cma__putstring (output, "  Current vp is ");
	    cma__putint (output, (cma_t_integer)thd->sched.processor->vp_id->vp);
	    cma__putstring (output, ", port is ");
	    cma__putint (output, (cma_t_integer)thd->sched.processor->vp_id->synch);
	    cma__puteol (output);

	    if ((st = cma__vp_get_state (thd->sched.processor->vp_id, &state))
		    == cma__c_vp_normal) {
		cma__putstring (output, "   VP state is ");
		cma__putstring (output, vp_state[state.run_state]);
		cma__putstring (output, "; ");

		if (state.suspend_count != 0) {
		    cma__putstring (output, "Suspended[");
		    cma__putint (output, state.suspend_count);
		    cma__putstring (output, "], ");
		    }

		cma__putstring (output, "Mach state: ");
		cma__putstring (output, mach_state[state.mach_state]);
		cma__putstring (output, ".");
		cma__puteol (output);
		}
	    else {
#if _CMA_KTHREADS_ == _CMA__MACH
		cma__putstring (output, "   (Can't get VP state: ");
		cma__putstring (output, mach_error_string ((kern_return_t)st));
		cma__putstring (output, ")");
#else
		cma__putstring (output, "   (Can't get VP state)");
#endif
		cma__puteol (output);
		}
#else
	    cma__putstring (output, "  Current vp is ");
	    cma__puthex (output, (cma_t_integer)thd->sched.processor->vp_id);
	    cma__puteol (output);
#endif
	    }

	}

    if ((display_flags & DISPLAY_FULL) != 0) {
	cma__putstring	(output, "  Join: mutex ");
	cma__puthex		(output, (int)thd->mutex);
	cma__putstring	(output, " (");
	cma__putint		(output, ((cma__t_int_mutex *)thd->mutex)->header.sequence);
	cma__putstring	(output, "), cv ");
	cma__puthex		(output, (int)thd->term_cv);
	cma__putstring	(output, " (");
	cma__putint		(output, ((cma__t_int_cv *)thd->term_cv)->header.sequence);
	cma__putstring	(output, ")");
	cma__puteol	(output);
	cma__putstring	(output, "  Sync. wait: mutex ");
	cma__puthex		(output, (int)thd->tswait_mutex);
	cma__putstring	(output, " (");
	cma__putint		(output, ((cma__t_int_mutex *)thd->tswait_mutex)->header.sequence);
	cma__putstring	(output, "), cv ");
	cma__puthex		(output, (int)thd->tswait_cv);
	cma__putstring	(output, " (");
	cma__putint		(output, ((cma__t_int_cv *)thd->tswait_cv)->header.sequence);
	cma__putstring	(output, ")");
	cma__puteol	(output);
	cma__putstring	(output, "  Start function ");
	cma__puthex	(output, (int)thd->start_code);
	cma__putstring	(output, " (");
	cma__puthex	(output, (int)thd->start_arg);
	cma__putstring	(output, ")");
	cma__puteol	(output);
	cma__putstring	(output, "  Thread's last errno was ");
	cma__putint	(output, thd->thd_errno);
	cma__puteol	(output);
	}

    switch ((int)thd->state) {
	case (int)cma__c_state_terminated :
	    {
	    cma__putstring  (output, "  Exit status ");
	    cma__putstring  (output, status[(int)thd->exit_status]);
	    cma__putstring  (output, ", result ");
	    cma__puthex	    (output, (int)thd->return_value);
	    cma__puteol	    (output);
	    cma__putstring  (output, "  ");
	    cma__putint     (output, thd->joiners);
	    cma__putstring  (output, " joiner");
	    cma__putstring  (output, (thd->joiners == 1 ? "." : "s."));
	    cma__puteol	    (output);
	    break;
	    }
	case (int)cma__c_state_blocked :
	    {
	    if (thd->wait_cv != (cma__t_int_cv *)cma_c_null_ptr) {
		cma__putstring	(output, "  Waiting on ");
		cma__putint	(
			output,
			((cma__t_int_cv *)thd->wait_cv)->header.sequence);
		cma__putstring	(output, "  ");
		cma__puthex	(output, (cma_t_integer)thd->wait_cv);
		cma__puteol     (output);
		}
	    else if (thd->wait_mutex
		    != (cma__t_int_mutex *)cma_c_null_ptr ) {
		cma__t_int_mutex	*mutex;

		mutex = thd->wait_mutex;
		cma__putstring	(output, "  Waiting to lock ");
		cma__putint	(output, mutex->header.sequence);
		cma__putstring	(output, "  ");
		cma__puthex	(output, (cma_t_integer)thd->wait_mutex);
		cma__puteol     (output);
		}
	    else {
		cma__putstring	(output, "  Blocked on indeterminate object!");
		cma__puteol	(output);
		}
	    }
	default :
	    {
	    cma__t_int_stack	*stack;
	    cma__t_queue	*sq;
	    cma__t_int_tcb	*self;
	    int			sp, limit;


	    self = cma__get_self_tcb ();
	    cma__putstring  (output, "  Scheduling: priority ");
	    cma__putint	    (output, thd->sched.priority);
	    cma__putstring  (output, ", policy ");
	    cma__putstring  (output, policy[(int)thd->sched.policy]);
	    cma__puteol     (output);

	    if (self == thd)
		sp = (int)cma__fetch_sp ();
	    else
		sp = (int)thd->static_ctx.sp;

	    if (cma__queue_empty (&thd->stack)) {
		cma__putstring	(output, "  Stack: ");
		cma__puthex	(output, sp);
		cma__putstring	(output, " (default stack) ");
		cma__puteol	(output);		
		}
	    else {
		int		select;
		static char	*inrange[2] = {") [<-SP]", ")"};


		sq = cma__queue_next (&thd->stack);
		stack = (cma__t_int_stack *)sq;

#ifdef _CMA_UPSTACK_
		limit = (int)stack->yellow_zone + cma__c_yellow_size;

		if ((sp >= (int)stack->stack_base) && (sp < limit))
		    select = 0;
		else
		    select = 1;
#else
		limit = (int)stack->yellow_zone - cma__c_yellow_size;

		if ((sp <= (int)stack->stack_base) && (sp > limit))
		    select = 0;
		else
		    select = 1;

#endif
		cma__putstring	(output, "  Stack: ");
		cma__puthex	(output, sp);
		cma__putstring	(output, " (base = ");
		cma__puthex	(output, (cma_t_integer)stack->stack_base);
		cma__putstring	(output, ", guard = ");
		cma__puthex	(output, limit);
		cma__putstring	(output, inrange[select]);
		cma__puteol	(output);

		sq = cma__queue_next (sq);

		if (sq != &thd->stack) {
		    cma__putstring  (output, "  Additional stack segments: ");
		    cma__puteol	    (output);

		    while (sq != &thd->stack) {
			stack = (cma__t_int_stack *)sq;
			sq = cma__queue_next (sq);
#ifdef _CMA_UPSTACK_
			limit = (int)stack->yellow_zone + cma__c_yellow_size;

			if ((sp >= (int) stack->stack_base) && (sp < limit))
			    select = 0;
			else
			    select = 1;
#else
			limit = (int)stack->yellow_zone - cma__c_yellow_size;

			if ((sp <= (int) stack->stack_base) && (sp > limit))
			    select = 0;
			else
			    select = 1;
#endif
			cma__putstring (output, "   base = ");
			cma__puthex (
				output,
				(cma_t_integer)stack->stack_base);
			cma__putstring (output, ", guard = ");
			cma__puthex (output, limit);
			cma__putstring (output, inrange[select]);
			cma__puteol (output);
			}

		    }

		}

	    break;
	    }

	}

    if (thd->detached) {
	cma__putstring	(output, "  Detached");
	cma__puteol	(output);
	}

    cma__putstring  (output, "  Alerts: ");

    if (!thd->alert.pending)
	cma__putstring (output, "none ");

    cma__putstring (output, "pending, ");
    cma__putstring (output, "general delivery ");

    if (thd->alert.g_enable)
	cma__putstring (output, enabled_string);
    else
	cma__putstring (output, disabled_string);

    cma__putstring (output, ", asynch delivery ");

    if (thd->alert.a_enable)
	cma__putstring (output, enabled_string);
    else
	cma__putstring (output, disabled_string);

    if (thd->alert.count > 0) {
	cma__putint (output, thd->alert.count);
	cma__putstring (output, " nested scopes");

	if (thd->alert.count != 1)
	    cma__putstring (output, "s");

	}

    cma__puteol (output);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Print help message
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	Print information on each command
 */
extern void
cma__debug_help(
	cma_t_integer	argc,
	char		*argv[])
    {
    char		output[cma__c_buffer_size];
    cma_t_integer	i;
    static char		*debug_help[] = {
	"",
	"	All keywords may be abbreviated: if the abbreviation is ambiguous,",
	"	the first match will be used. Exit from the debug function by entering",
#if _CMA_OS_ == _CMA__VMS
	"	^Z, or a blank line.",
#else
	"	^D, or a blank line.",
#endif
	"",
	" attributes [id]",
	"   Print information about attributes objects.",
	"	id	Attributes object number",
	"",
	" condition [id] -w -q",
	"   Print information about condition variable objects.",
	"	id	Condition variable object number",
	"	-w	List only condition variables with waiting threads",
	"	-q	Show the queue of waiting threads (if any)",
	"",
	" help",
	"   Print this message.",
	"",
	" exit",
	"   Exit from DCE Threads debugger.",
	"",
	" mutex [id] -l -q",
	"   Print information about mutex objects.",
	"	id	Mutex object number",
	"	-l	List only mutexes which are locked",
	"	-q	Show the queue of waiting threads (if any)",
	"",
	" show -m",
	"   Print information about DCE Threads.",
	"	-m	Show internal memory management statistics.",
#ifdef _CMA_TRACE_KERNEL_
	"	-k	Show trace log of kernel lock operations.",
#endif
#ifdef _CMA_TRACE_SEM_
	"	-s	Show trace log of semaphore operations.",
#endif
	"",
	" thread [id] -f -r -b -t -c -d -a -s<sched> -h -n",
	"   Print information about thread objects.",
	"	id	Thread object number",
	"	-f	Display full thread information",
	"	-r	Select only threads that are ready to run",
	"	-b	Select only threads that are blocked",
	"	-t	Select only terminated threads",
	"	-c	Select only currently running threads",
	"	-d	Select only detached threads",
	"	-a	Include \"invisible\" threads owned by DCE Threads",
	"	-s	Select by scheduling information. The syntax of the",
	"		value is \"policy[<op>priority]\". \"policy\" is the name",
	"		of a DCE Threads scheduling policy. <op> is one of the C",
	"		relational operators ==, !=, >, <, >=, or <=. \"priority\"",
	"		may be an integer, or one of the keywords \"min\", \"mid\",",
	"		or \"max\" designating the minimum, midrange, or maximum",
	"		priority of the specified scheduling policy. Threads are",
	"		selected if they have the specified policy and a priority",
	"		which is equal to (==), greater than (>), and so forth, with",
	"		respect to the specified priority",
	"	-h	Select only threads which are on hold",
	"	-n	Select only threads which are not on hold",
	"",
	" tset id -c -s<sched> -h -n -a -v",
	"   Set thread object state. If \"id\" is specified as \"*\", then all",
	"   threads will be modified; only -h, -n, and -s may be used with \"*\".",
	"	id	Mutex object number or \"*\"",
	"	-c	Cancel thread (at next cancellation point)",
	"	-s	Set scheduling information, \"[policy]:priority\". The",
	"		\"policy\" may be any of the DCE Threads scheduling policies,",
	"		fifo, rr, throughput, background, ada_low, or idle. The",
	"		\"priority\" may be an integer, or one of the keywords",
	"		\"min\", \"mid\", or \"max\" designating the minimum,",
	"		midrange, or maximum priority for the specified policy",
	"	-h	Put the thread on hold; the thread cannot run until set",
	"		to -n, \"nohold\"",
	"	-n	Release a thread that was put on hold",
	"	-a	Make the thread active: it will be the next thread to run",
	"	-v	Make the thread visible so that its state may be examined",
	cma_c_null_ptr
	};

    output[0] = '\0';

    for (i = 0; debug_help[i] != cma_c_null_ptr; i++) {
	cma__putstring (output, debug_help[i]);
	cma__puteol (output);
	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Print the status of all attributes objects.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	known attributes object queue.
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	Print information on each known attributes object.
 */
extern void
cma__debug_list_atts(
	cma_t_integer	argc,
	char		*argv[])
    {
    cma__t_queue	*ptr;
    cma_t_integer	select_id;
    char		output[cma__c_buffer_size];


    output[0] = '\0';
    if (argc == 1)			/* If no args, then select all */
	select_id = 0;
    else
	select_id = cma__atol (argv[1]);

    if (cma___test_mutex (cma__g_known_atts.mutex)) {
	cma__putstring (output, ">>Warning: known attributes list is currently locked;");
	cma__puteol    (output);
	cma__putstring (output, ">>state may be inconsistent (continuing anyway).");
	cma__puteol    (output);
	}

    ptr = &cma__g_known_atts.queue;

    while ((ptr = cma__queue_next (ptr)) != &cma__g_known_atts.queue) {
	cma__t_int_attr	*att;

	att = (cma__t_int_attr *)ptr;

	if (select_id == 0 || select_id == att->header.sequence) {
	    cma__putstring  (output, "Attributes object ");
	    cma__putstring  (output, name_string (att));
	    cma__putstring  (output, " (");
	    cma__puthex	    (output, (cma_t_integer) att);
	    cma__putstring  (output, ") ");
	    cma__putint	    (output, att->header.sequence);
	    cma__puteol	    (output);

	    cma__putstring  (output, "  reference count ");
	    cma__putint	    (output, att->refcnt);
	    cma__putstring  (output, (char *) 
		(att->delete_pending ? ", delete pending" : ""));
	    cma__puteol	    (output);

	    cma__putstring  (output, "  scheduling: (priority ");
	    cma__putint	    (output, att->priority);
	    cma__putstring  (output, ", policy ");
	    cma__putstring  (output, policy [(int)att->policy]);
	    cma__putstring  (output, "); ");
	    cma__putstring  (output, (char *) 
		(att->inherit_sched ? "inherit scheduling" : "use default"));
	    cma__puteol	    (output);

	    cma__putstring  (output, "  stack size ");
	    cma__putint	    (output, att->stack_size);
	    cma__putstring  (output, ", guard size ");
	    cma__putint	    (output, att->guard_size);
	    cma__puteol	    (output);

	    cma__putstring  (output, "  mutex type ");
	    cma__putstring  (output, mutex_type [(int)att->mutex_kind]);
	    cma__puteol	    (output); 
	    }

	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Print the status of all condition variables.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	known cv queue.
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	Print information on each known condition variable.
 */
extern void
cma__debug_list_cvs(
	int	argc,
	char	*argv[])
    {
    cma__t_queue	*ptr;
    cma_t_integer	select_id = 0, arg;
    cma_t_boolean	select_all = cma_c_true;
    cma_t_boolean	select_waiters = cma_c_false;
    cma_t_boolean	show_queue = cma_c_false;
    char		output[cma__c_buffer_size];
    cma_t_integer	cols;


    output[0] = '\0';
    for (arg = 1; arg < argc; arg++) {

	if (argv[arg][0] == '-')	/* If a switch */
	    switch (argv[arg][1]) {
		case 'w' : {
		    select_all = cma_c_false;
		    select_waiters = cma_c_true;
		    break;
		    }
		case 'q' : {
		    show_queue = cma_c_true;
		    break;
		    }
		}
	else
	    select_id = cma__atol (argv[arg]);

	}

    if (cma___test_mutex (cma__g_known_cvs.mutex)) {
	cma__putstring (output, ">>Warning: known conditions list is currently locked;");
	cma__puteol(output);
	cma__putstring (output, ">>state may be inconsistent (continuing anyway).");
	cma__puteol(output);
	}

    ptr = &cma__g_known_cvs.queue;

    while ((ptr = cma__queue_next (ptr)) != &cma__g_known_cvs.queue) {
	cma__t_int_cv	*cv;

	cv = (cma__t_int_cv *)ptr;

	if (
		((select_id == 0) || (select_id == cv->header.sequence))
		&& (select_all || (select_waiters
		&& cma__sem_if_waiters (&cv->semaphore)))
		) {
	    cma__t_queue	*que;
	    cma_t_integer	count;


	    cma__putstring (output, "Condition variable ");
	    cols = 19;
	    cma__putstring (output, name_string (cv));
	    cols += cma__strlen (name_string (cv));
	    cma__putstring (output, " (");
	    cma__puthex (output, (cma_t_integer) cv);
	    cma__putstring (output, ") ");
	    cols += 12;
	    cma__putint (output, cv->header.sequence);
	    cols += cma___intsize (cv->header.sequence);

	    if (cma__sem_if_waiters (&cv->semaphore)) {

		for (
			(count = 0, que = cma__queue_next (&cv->semaphore.queue));
			(que != &cv->semaphore.queue);
			(count++, que = cma__queue_next (que))
			);

		cma__putstring (output, ", ");
		cols += 2;
		cma__putint (output, count);
		cols += cma___intsize (count);

		if (count == 1) {
		    cma__putstring (output, " waiter");
		    cols += 7;
		    }
		else {
		    cma__putstring (output, " waiters");
		    cols += 8;
		    }

		if (show_queue) {
		    cma__t_int_tcb	*thd;
		    cma_t_boolean	wol = cma_c_false;


		    thd = (cma__t_int_tcb *)cma__queue_next (&cv->semaphore.queue);
		    cma__putstring (output, ": ");
		    cols += 2;

		    while ((cma__t_queue *)thd != &cv->semaphore.queue) {

			if (wol) {
			    cma__putstring (output, ",");
			    cols++;
			    }

			if (cols >= 76) {
			    cma__puteol (output);
			    cma__putstring (output, "  ");
			    cols = 2;
			    wol = cma_c_false;
			    }

			cma__putint (output, thd->header.sequence);
			cols += cma___intsize (thd->header.sequence);
			wol = cma_c_true;
			thd = (cma__t_int_tcb *)cma__queue_next (
				&thd->header.queue);
			}

		    }

		}

	    if (cols > 2)
		cma__puteol (output);

	    }

	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Print the status of all mutexes
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	known mutex queue.
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	Print information on each known mutex.
 */
extern void
cma__debug_list_mutexes(
	int	argc,
	char	*argv[])
    {
    cma__t_queue	*ptr;
    cma_t_integer	select_id = 0, arg;
    cma_t_boolean	select_all = cma_c_true;
    cma_t_boolean	select_locked = cma_c_false;
    cma_t_boolean	show_queue = cma_c_false;
    char		output[cma__c_buffer_size];
    cma_t_integer	cols = 3;


    output[0] = '\0';
    for (arg = 1; arg < argc; arg++) {

	if (argv[arg][0] == '-')	/* If a switch */
	    switch (argv[arg][1]) {
		case 'l' : {
		    select_all = cma_c_false;
		    select_locked = cma_c_true;
		    break;
		    }
		case 'q' : {
		    show_queue = cma_c_true;
		    break;
		    }
		}
	else
	    select_id = cma__atol (argv[arg]);

	}

    if (cma___test_mutex (cma__g_known_mutexes.mutex)) {
	cma__putstring(output, ">>Warning: known mutex list is currently locked;");
	cma__puteol(output);
	cma__putstring(output, ">>state may be inconsistent (continuing anyway).");
	cma__puteol(output);
	}

    ptr = &cma__g_known_mutexes.queue;

    while ((ptr = cma__queue_next (ptr)) != &cma__g_known_mutexes.queue) {
	cma__t_int_mutex	*mutex;

	mutex = (cma__t_int_mutex *)ptr;

	if (
		((select_id == 0) || (select_id == mutex->header.sequence))
		&&
		(select_all || (select_locked && cma___test_mutex (mutex)))
		) {
	    cma__putstring (output, "Mutex ");
	    cols = 6;
	    cma__putstring (output, name_string(mutex));
	    cols += cma__strlen (name_string (mutex));
	    cma__putstring (output, " (");
	    cma__puthex (output, (cma_t_integer) mutex);
	    cma__putstring (output, ") ");
	    cols += 12;
	    cma__putint (output, mutex->header.sequence);
	    cols += cma___intsize (mutex->header.sequence);
	    cma__putstring (output, ", type ");
	    cols += 7;
	    cma__putstring (output, mutex_type [(int)mutex->mutex_kind]);
	    cols += cma__strlen (mutex_type [(int)mutex->mutex_kind]);

	    if (!cma___test_mutex (mutex)) {
		cma__putstring (output, ", unlocked");
		cols += 10;
		}
	    else {

		if (mutex->owner == (cma__t_int_tcb *)cma_c_null_ptr) {
		    cma__putstring (output, ", locked");
		    cols += 8;
		    }
		else {
		    cma__putstring (output, ", locked by ");
		    cols += 12;
		    cma__putint (output, mutex->owner->header.sequence);
		    cols += cma___intsize (mutex->owner->header.sequence);
		    }

		if (mutex->mutex_kind == cma_c_mutex_recursive) {

		    if (cols >= 67) {
			cma__puteol (output);
			cma__putstring (output, "  ");
			cols = 2;
			cma__putstring (output, "(depth ");
			cols += 7;
			}
		    else {
			cma__putstring (output, " (depth ");
			cols += 8;
			}

		    cma__putint	(output, mutex->nest_count);
		    cols += cma___intsize (mutex->nest_count);
		    cma__putstring (output, ")");
		    cols++;
		    }

		if (cma__sem_if_waiters (&mutex->semaphore)) {
		    cma__t_queue	*q;
		    cma_t_integer	cnt;


		    for (
			    cnt = 0,
				q = cma__queue_next (&mutex->semaphore.queue);
			    q != &mutex->semaphore.queue;
			    cnt++, q = cma__queue_next (q)
			    );

		    if (cols >= 69) {
			cma__putstring (output, ",");
			cma__puteol (output);
			cma__putstring (output, "  ");
			cols = 2;
			}
		    else {
			cma__putstring (output, ", ");
			cols += 2;
			}

		    cma__putint (output, cnt);
		    cols += cma___intsize (cnt);
		    cma__putstring (output, " waiting");
		    cols += 8;

		    if (show_queue) {
			cma_t_boolean	wol = cma_c_false;
			cma__t_int_tcb	*thd;


			thd = (cma__t_int_tcb *)cma__queue_next (
				&mutex->semaphore.queue);
			cma__putstring (output, ": ");
			cols += 2;

			while ((cma__t_queue *)thd != &mutex->semaphore.queue) {

			    if (wol) {
				cma__putstring (output, ",");
				cols++;
				}

			    if (cols >= 76) {
				cma__puteol (output);
				cma__putstring (output, "  ");
				cols = 2;
				wol = cma_c_false;
				}

			    cma__putint (output, thd->header.sequence);
			    cols += cma___intsize (thd->header.sequence);
			    wol = cma_c_true;
			    thd = (cma__t_int_tcb *)cma__queue_next (
				    &thd->header.queue);
			    }

			}

		    }

		}

	    if (cols > 2)
		cma__puteol (output);

	    }

	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Print the status of all threads.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	known thread queue.
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	Print information on each known thread.
 */
extern void
cma__debug_list_threads(
	int	argc,
	char	*argv[])
    {
#define cma___c_cmp_eql 0
#define cma___c_cmp_neq 1
#define cma___c_cmp_gtr 2
#define cma___c_cmp_geq 3
#define cma___c_cmp_lss 4
#define cma___c_cmp_leq 5
    cma__t_queue	*ptr;
    cma_t_boolean	suppress_header = cma_c_false;
    cma_t_integer	select_id = 0, arg;
    cma_t_boolean	select_all = cma_c_true;
    cma_t_boolean	select_blocked = cma_c_false;
    cma_t_boolean	select_terminated = cma_c_false;
    cma_t_boolean	select_current = cma_c_false;
    cma_t_boolean	select_null = cma_c_false;
    cma_t_boolean	select_ready = cma_c_false;
    cma_t_boolean	select_detach = cma_c_false;
    cma_t_boolean	select_hold = cma_c_false;
    cma_t_boolean	select_nohold = cma_c_false;
    cma_t_boolean	select_sched = cma_c_false;
    cma_t_boolean	use_internal_formatter = cma_c_false;
    cma_t_integer	display_flags = 0;
    cma_t_integer	sched_comparison = cma___c_cmp_geq;
    cma_t_boolean	operator_found = cma_c_false;
    cma_t_sched_policy	sched_policy = cma_c_sched_throughput;
    cma_t_priority	sched_priority;
    char		output[cma__c_buffer_size];

    output[0] = '\0';
    for (arg = 1; arg < argc; arg++) {

	if (argv[arg][0] == '-')	/* If a switch */
	    switch (argv[arg][1]) {
		case 'f' : {
		    display_flags |= DISPLAY_FULL;
		    break;
		    }
		case 'r' : {
		    select_all = cma_c_false;
		    select_ready = cma_c_true;
		    break;
		    }
		case 'b' : {
		    select_all = cma_c_false;
		    select_blocked = cma_c_true;
		    break;
		    }
		case 't' : {
		    select_all = cma_c_false;
		    select_terminated = cma_c_true;
		    break;
		    }
		case 'c' : {
		    select_all = cma_c_false;
		    select_current = cma_c_true;
		    break;
		    }
		case 'i' : {
		    use_internal_formatter = cma_c_true;
		    break;
		    }
		case 'd' : {
		    select_all = cma_c_false;
		    select_detach = cma_c_true;
		    break;
		    }
		case 'a' : {
		    select_all = cma_c_false;
		    select_null = cma_c_true;
		    break;
		    }
		case 'h' : {
		    select_all = cma_c_false;
		    select_hold = cma_c_true;
		    break;
		    }
		case 'n' : {
		    select_all = cma_c_false;
		    select_nohold = cma_c_true;
		    break;
		    }
		case 's' : {
		    char		*ch;
		    cma_t_integer	len;


		    /*
		     * Parse a scheduling selector:
		     *
		     *	    <policy><op><priority>
		     *
		     * Where <policy> is "fifo", "rr", "throughput",
		     * "background", "ada_low", or "idle". It may be
		     * abbreviated.
		     *
		     * Where <op> is "==", "!=", "<", ">", "<=", or ">=", and
		     * controls how threads are selected against the priority
		     * (not the policy) specified.
		     *
		     * Where <priority> is an integer, or "min", or "mid", or
		     * "max".  The keywords are interpreted relative to the
		     * specified scheduling policy.
		     */
		    select_all = cma_c_false;
		    select_sched = cma_c_true;
		    ch = &argv[arg][2];
		    len = 0;

		    while (*ch != '\0'
			    && *ch != '>'
			    && *ch != '<'
			    && *ch != '='
			    && *ch != '!') {
			len++;
			ch++;
			}

		    if (len > 0) {
			cma_t_integer	i;
			cma_t_boolean	found = cma_c_false;


			for (i = 0; i < cma___c_num_policies; i++) {

			    if (cma__strncmp (&argv[arg][2], policy[i], len) == 0) {
				sched_policy = (cma_t_sched_policy)i;
				found = cma_c_true;
				break;
				}

			    }

			if (!found) {
			    cma__putstring (output, "%Bad policy: ");
			    cma__putstring (output, argv[arg]);
			    cma__puteol (output);
			    output[0] = '\0';
			    return;
			    }

			}
		    
		    if (*ch == '!' || *ch == '=' || *ch == '>' || *ch == '<') {
			cma_t_boolean	bad_operator = cma_c_false;


			switch (*ch++) {
			    case '!' : {
				if (*ch == '=') {
				    ch++;
				    sched_comparison = cma___c_cmp_neq;
				    }
				else
				    bad_operator = cma_c_true;
				break;
				}
			    case '=' : {
				if (*ch == '=') {
				    ch++;
				    sched_comparison = cma___c_cmp_eql;
				    }
				else
				    bad_operator = cma_c_true;
				break;
				}
			    case '>' : {
				if (*ch == '=') {
				    ch++;
				    sched_comparison = cma___c_cmp_geq;
				    }
				else
				    sched_comparison = cma___c_cmp_gtr;
				break;
				}
			    case '<' : {
				if (*ch == '=') {
				    ch++;
				    sched_comparison = cma___c_cmp_leq;
				    }
				else
				    sched_comparison = cma___c_cmp_lss;
				break;
				}
			    default : {
				bad_operator = cma_c_true;
				break;
				}
				
			    }

			if (bad_operator) {
			    cma__putstring (output, "%Bad operator in ");
			    cma__putstring (output, argv[arg]);
			    cma__puteol (output);
			    return;
			    }
			else
			    operator_found = cma_c_true;

			}
		    else if (*ch != '\0') {	/* Junk at end of switch */
			cma__putstring (output, "%Bad scheduling selector: ");
			cma__putstring (output, argv[arg]);
			cma__puteol (output);
			}

		    /*
		     * Priority may be a keyword: "min" for the minimum value
		     * of the current policy, "mid" for the midrange, or
		     * "max" for the maximum.
		     */
		    if (*ch >= 'a' && *ch <= 'z') {	/* Priority is a keyword */
			cma_t_integer	i;
			cma_t_boolean	found = cma_c_false;
			char		*pri_start;


			len = 0;
			pri_start = ch;

			while (*ch >= 'a' && *ch <= 'z') {
			    ch++;
			    len++;
			    }

			for (i = 0; i < cma___c_num_pris; i++)

			    if (cma__strncmp (pri_start, pri_names[i], len) == 0) {
				sched_priority
				    = pri_range[i][(int)sched_policy];
				found = cma_c_true;
				}

			if (!found) {
			    cma__putstring (output, "%Bad priority: ");
			    cma__putstring (output, pri_start);
			    cma__puteol (output);
			    output[0] = '\0';
			    return;
			    }

			}
		    else if (*ch >= '0' && *ch <= '9') {
			sched_priority = cma__atol (ch);

			if (sched_priority
				    < pri_range[0][(int)sched_policy]
				|| sched_priority
				    > pri_range[2][(int)sched_policy]) {
			    cma__putstring (output, "%Priority ");
			    cma__putint (output, sched_priority);
			    cma__putstring (output, " is not valid for the ");
			    cma__putstring (output, policy[(int)sched_policy]);
			    cma__putstring (output, " policy; range is ");
			    cma__putint (output, pri_range[0][(int)sched_policy]);
			    cma__putstring (output, " to ");
			    cma__putint (output, pri_range[2][(int)sched_policy]);
			    cma__putstring (output, ".");
			    cma__puteol (output);
			    return;
			    }

			}
		    else if (*ch == '\0' && !operator_found)
			/*
			 * The default operator is ">=" and the default
			 * priority is "min"; this combination results in
			 * displaying all threads in the specified policy.
			 */
			sched_priority = pri_range[0][(int)sched_policy];
		    else {
			cma__putstring (output, "%Bad priority in ");
			cma__putstring (output, argv[arg]);
			cma__puteol (output);
			return;
			}

		    break;
		    }

		case 'v' : {
		    display_flags |= DISPLAY_VP;
		    break;
		    }

		default : {
		    cma__putstring (output, "%Unrecognized switch: ");
		    cma__putstring (output, argv[arg]);
		    cma__puteol (output);
		    output[0] = '\0';
		    return;
		    }

		}
	else
	    select_id = cma__atol (argv[arg]);

	}

    if (select_ready) {
	cma_t_integer	i;


        for (i = cma__c_prio_n_tot - 1; i >= 0; i--) {
	    ptr = &cma__g_ready_list[i];	/* Point to ready list */

	    while ((ptr = cma__queue_next (ptr)) != &cma__g_ready_list[i]) {
		if (use_internal_formatter)
 		    cma__debug_format_thread (
			    (cma__t_int_tcb *)ptr,
			    display_flags);
		else {
		    cma__deb_show_thread (
			    (cma__t_int_tcb *)ptr,
			    ((display_flags & DISPLAY_FULL) != 0),
			    suppress_header,
			    active_regs,
			    cma_c_null_ptr,
			    cma_c_null_ptr,
			    cma_c_null_ptr);
		    suppress_header = cma_c_true;
		    }

		}

	    }

	}
    else
	{
	/*
	 * The known thread list is protected by the kernel lock; the debug
	 * main routine already owns the lock if possible, and has given a
	 * warning if it was locked.  There's not much more we can do here.
	 */
	ptr = &cma__g_known_threads.queue;	/* Point to queue header */

	while ((ptr = cma__queue_next (ptr)) != &cma__g_known_threads.queue) {
	    cma__t_int_tcb	*thd;


	    thd = cma__base (ptr, threads, cma__t_int_tcb);	/* Get TCB base */

	    if (thd->kind == cma__c_thkind_null) {

		if (!select_null && select_id != thd->header.sequence)
		    continue;

		}

	    if (select_id != 0 && select_id != thd->header.sequence)
		continue;

	    /*
	     * Reject if switches are set and thread doesn't match (however,
	     * we ignore switches if a thread ID is specified)
	     */
	    if (! select_all && select_id == 0) {

		if (select_ready && thd->state != cma__c_state_ready)
		    continue;
		if (select_blocked && thd->state != cma__c_state_blocked)
		    continue;
		if (select_terminated && thd->state != cma__c_state_terminated)
		    continue;
		if (select_current && thd->state != cma__c_state_running)
		    continue;
		if (select_detach && !thd->detached)
		    continue;
		if (select_hold && !thd->debug.on_hold)
		    continue;
		if (select_nohold && thd->debug.on_hold)
		    continue;

		if (select_sched) {

		    if (thd->sched.policy != sched_policy)
			continue;

		    switch (sched_comparison) {
			case cma___c_cmp_eql : {
			    if (thd->sched.priority != sched_priority)
				continue;
			    break;
			    }
			case cma___c_cmp_neq : {
			    if (thd->sched.priority == sched_priority)
				continue;
			    break;
			    }
			case cma___c_cmp_lss : {
			    if (thd->sched.priority >= sched_priority)
				continue;
			    break;
			    }
			case cma___c_cmp_gtr : {
			    if (thd->sched.priority <= sched_priority)
				continue;
			    break;
			    }
			case cma___c_cmp_leq : {
			    if (thd->sched.priority > sched_priority)
				continue;
			    break;
			    }
			case cma___c_cmp_geq : {
			    if (thd->sched.priority < sched_priority)
				continue;
			    break;
			    }

			}

		    }

		}

	    if (use_internal_formatter)
		cma__debug_format_thread (
			thd,
			display_flags);
	    else {
		cma__deb_show_thread (
			thd,
			((display_flags & DISPLAY_FULL) != 0),
			suppress_header,
			active_regs,
			cma_c_null_ptr,
			cma_c_null_ptr,
			cma_c_null_ptr);
		suppress_header = cma_c_true;
		}

	    }

	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Process a single debug command
 *
 *  FORMAL PARAMETERS:
 *
 *	cmd	The command string
 *	locked	Previous state of kernel lock before cma_debug()
 *
 *  IMPLICIT INPUTS:
 *
 *	lots of stuff
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	cma_c_true if command was "exit"
 *
 *  SIDE EFFECTS:
 *
 *	possibly just about anything
 */
extern cma_t_boolean
cma__debug_parse(
	char		*cmd,
	cma_t_boolean	locked)
    {
#define cma___c_command_count	9
#define cma___c_max_args	10
    static cma___t_cmd_info	cmds[cma___c_command_count] = {
	{"attribute", cma___c_cmd_normal, cma__debug_list_atts,
	    "[id] : list attributes objects"},
	{"condition", cma___c_cmd_normal, cma__debug_list_cvs,
	    "[id] -w -q : list condition variables"},
	{"help", cma___c_cmd_normal, cma__debug_help,
	    ": display help text"},
	{"exit", cma___c_cmd_exit, (void (*)(int argc, char *argv[]))0,
	    ": exit from DCE Threads debugger"},
	{"mutex", cma___c_cmd_normal, cma__debug_list_mutexes,
	    "[id] -l -q : list mutexes"},
	{"show", cma___c_cmd_normal, cma__debug_show,
	    "-m {-k -s} -v"},
	{"thread", cma___c_cmd_normal, cma__debug_list_threads,
	    "[id] -f -r -b -t -c -d -a -s<sched> -h -n -i : list threads"},
	{"tset", cma___c_cmd_modifier, cma__debug_set_thread,
	    "{id | *} -s<sched> -c -h -n -a -v : set state of thread"},
	{"vp", cma___c_cmd_hidden, cma__debug_vp,
	    "-s -r : suspend or resume other VPs"}
	};
    char			*argv[cma___c_max_args];
    char			*ptr, *argptr;
    cma_t_boolean		found;
    cma_t_integer		argc;
    cma_t_integer		i;
    cma_t_integer		length;
    static char			argstr[cma___c_max_args * 30];
    char			output[cma__c_buffer_size];


    ptr = cmd;
    argptr = &argstr[0];
    argc = 0;
    found = cma_c_false;

    for (i = 0; i < cma___c_max_args; i++) argv[i] = cma_c_null_ptr;

    while (argc < cma___c_max_args && *ptr != '\0') {

	/*
	 * Skip any leading whitespace
	 */
	while ((*ptr == ' ' || *ptr == '\t')
		&& (*ptr != '\n') && (*ptr != '\0'))
	    ptr++;

	argv[argc++] = argptr;		/* Set start of next arg */

	/*
	 * Find end of argument string
	 */
	while (*ptr != ' ' && *ptr != '\t' && *ptr != '\n' && *ptr != '\0')
	    *argptr++ = *ptr++;

	*argptr++ = '\0';		/* Terminate the argument */
	}

    if (argc == 0) return cma_c_false;	/* If no args, get new line */

    length = cma__strlen (argv[0]);

    for (i = 0; i < cma___c_command_count; i++) {

	if (cma__strncmp (argv[0], cmds[i].verb, length) == 0) {
	    found = cma_c_true;

	    if (cmds[i].type == cma___c_cmd_exit)
		return cma_c_true;

	    if (cmds[i].type == cma___c_cmd_modifier && locked) {
		output[0] = '\0';
		cma__putstring (output, "%Modifier commands not allowed: ");
		cma__putstring (output, "threads library is active.");
		cma__puteol (output);
		}
	    else
		(cmds[i].routine) (argc, argv);

	    break;
	    }

	}

    if (! found) {
	output[0] = '\0';
	cma__putstring (output, "Verb ");
	cma__putstring (output, argv[0]);
	cma__putstring (output, " not recognized, try one of:");
	cma__puteol (output);

	for (i = 0; i < cma___c_command_count; i++)
	    if (cmds[i].type != cma___c_cmd_hidden) {
		output[0] = '\0';
		cma__putstring(output, "    ");
		cma__putstring(output, cmds[i].verb);
		cma__putstring(output, " ");
		cma__putstring(output, cmds[i].desc);
		cma__puteol (output);
		}

	}

    return cma_c_false;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Set thread attributes
 *
 *  FORMAL PARAMETERS:
 *
 *	argc	Number of arguments to command
 *	argv	Array of argument strings
 *
 *  IMPLICIT INPUTS:
 *
 *	Known thread list
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern void
cma__debug_set_thread(
	cma_t_integer	argc,
	char		*argv[])
    {
    cma_t_integer	thd_num = -1, arg;
    cma__t_int_tcb	*tcb;
    cma__t_queue	*ptr;
    char		output[cma__c_buffer_size];
    cma_t_boolean	set_cancel = cma_c_false;
    cma_t_boolean	set_sched = cma_c_false;
    cma_t_boolean	set_hold = cma_c_false;
    cma_t_boolean	set_nohold = cma_c_false;
    cma_t_boolean	set_active = cma_c_false;
    cma_t_boolean	set_visible = cma_c_false;
    cma_t_boolean	conflicting = cma_c_false;
    cma_t_boolean	boolsetbuf;
    cma_t_sched_policy	sched_policy;
    cma_t_boolean	policy_present = cma_c_false;
    cma_t_priority	sched_priority;
    cma_t_boolean	sched_prio_is_key = cma_c_false;
    cma_t_integer	sched_prio_key;
    cma_t_boolean	do_all_threads = cma_c_false;


    output[0] = '\0';

    if (argc < 2) {
	cma__putstring (output, "%Too few arguments");
	cma__puteol (output);
	return;
	}

    for (arg = 1; arg < argc; arg++) {

	if (argv[arg][0] == '-')	/* If a switch */
	    switch (argv[arg][1]) {
		case 'c' : {
		    set_cancel = cma_c_true;
		    break;
		    }
		case 's' : {
		    char		*ch;
		    cma_t_integer	len;


		    set_sched = cma_c_true;

		    /*
		     * Parse a scheduling specifier:
		     *
		     *	    [<policy>]:<priority>
		     *
		     * Where <policy> is "fifo", "rr", "throughput",
		     * "background", "ada_low", or "idle". It may be
		     * abbreviated.
		     *
		     * Where <priority> is an integer, or "min", or "mid", or
		     * "max".  The keywords are interpreted relative to the
		     * specified scheduling policy.
		     */
		    ch = &argv[arg][2];
		    len = 0;

		    while (*ch != '\0' && *ch != ':') {
			len++;
			ch++;
			}

		    if (len > 0) {
			cma_t_integer	i;


			for (i = 0; i < cma___c_num_policies; i++) {

			    if (cma__strncmp (&argv[arg][2], policy[i], len) == 0) {
				sched_policy = (cma_t_sched_policy)i;
				policy_present = cma_c_true;
				break;
				}

			    }

			}
		    
		    /*
		     * The policy is optional, but the priority is required;
		     * so if there's no colon, complain and quit.
		     */
		    if (*ch++ != ':') {
			cma__putstring (output, "%Priority is required for -s");
			cma__puteol (output);
			return;
			}

		    /*
		     * Priority may be a keyword: "min" for the minimum value
		     * of the current policy, "mid" for the midrange, or
		     * "max" for the maximum.
		     */
		    if (*ch >= 'a' && *ch <= 'z') {	/* Priority is a keyword */
			cma_t_integer	i;
			char		*pri_start;
			cma_t_boolean	found = cma_c_false;


			sched_prio_is_key = cma_c_true;
			len = 0;
			pri_start = ch;

			while (*ch >= 'a' && *ch <= 'z') {
			    ch++;
			    len++;
			    }

			for (i = 0; i < cma___c_num_pris; i++)

			    if (cma__strncmp (pri_start, pri_names[i], len) == 0) {

				if (policy_present)
				    sched_priority
					= pri_range[i][(int)sched_policy];
				else
				    sched_prio_key = i;

				found = cma_c_true;
				}

			if (!found) {
			    cma__putstring (output, "%Bad priority: ");
			    cma__putstring (output, pri_start);
			    cma__puteol (output);
			    return;
			    }

			}
		    else {		/* Priority is an integer */
			sched_priority = cma__atol (ch);

			if (policy_present) {

			    if (sched_priority
					< pri_range[0][(int)sched_policy]
				    || sched_priority
					> pri_range[2][(int)sched_policy]) {
				cma__putstring (output, "%Priority ");
				cma__putint (output, sched_priority);
				cma__putstring (
					output,
					" is not valid for the ");
				cma__putstring (
					output,
					policy[(int)sched_policy]);
				cma__putstring (
					output,
					" policy; range is ");
				cma__putint (
					output,
					pri_range[0][(int)sched_policy]);
				cma__putstring (output, " to ");
				cma__putint (
					output,
					pri_range[2][(int)sched_policy]);
				cma__putstring (output, ".");
				cma__puteol (output);
				return;
				}

			    }

			}

		    break;
		    }
		case 'h' : {
		    if (set_nohold)
			conflicting = cma_c_true;
		    else
			set_hold = cma_c_true;

		    break;
		    }
		case 'n' : {
		    if (set_hold)
			conflicting = cma_c_true;
		    else
			set_nohold = cma_c_true;

		    break;
		    }
		case 'a' : {
		    set_active = cma_c_true;
		    break;
		    }
		case 'v' : {
		    set_visible = cma_c_true;
		    break;
		    }
		}
	else {

	    if (argv[arg][0] == '*' && argv[arg][1] == '\0')
		do_all_threads = cma_c_true;
	    else
		thd_num = cma__atol (argv[arg]);

	    }

	}

    if (thd_num < 0 && !do_all_threads) {
	output[0] = '\0';
	cma__putstring (output, "%Thread number is required");
	cma__puteol (output);
	return;
	}

    if (do_all_threads && (set_active || set_visible || set_cancel))
	conflicting = cma_c_true;

    if (conflicting) {
	cma__putstring (output, "%Conflicting switches were specified:");
	cma__putstring (output, " some have been ignored.");
	cma__puteol (output);
	}

    tcb = (cma__t_int_tcb *)cma_c_null_ptr;
    ptr = &cma__g_known_threads.queue;	/* Point to queue header */

    while ((ptr = cma__queue_next (ptr)) != &cma__g_known_threads.queue) {
	cma__t_int_tcb	*thd = (cma__t_int_tcb *)ptr;


	thd = cma__base (ptr, threads, cma__t_int_tcb);	/* Get TCB base */

	if (do_all_threads && (thd->kind != cma__c_thkind_null)) {

	    if (set_hold) {
		boolsetbuf = cma_c_true;
		cma__deb_set (
			thd,
			cma_c_debset_hold,
			(cma_t_address)&boolsetbuf,
			(cma_t_integer)(sizeof (cma_t_boolean)));
		}

	    if (set_nohold) {
		boolsetbuf = cma_c_false;
		cma__deb_set (
			thd,
			cma_c_debset_hold,
			(cma_t_address)&boolsetbuf,
			(cma_t_integer)(sizeof (cma_t_boolean)));
		}

	    if (set_sched) {

		if (policy_present)
		    cma__deb_set (
			    thd,
			    cma_c_debset_policy,
			    (cma_t_address)&sched_policy,
			    (cma_t_integer)(sizeof (cma_t_sched_policy)));

		cma__deb_set (
			thd,
			cma_c_debset_priority,
			(cma_t_address)&sched_priority,
			(cma_t_integer)(sizeof (cma_t_priority)));
		}
	    
	    }
	else if (thd->header.sequence == thd_num) {
	    tcb = thd;
	    break;
	    }

	}

    /*
     * If we're changing all threads, then we did everything during the
     * traversal.
     */
    if (do_all_threads)
	return;

    if (tcb == (cma__t_int_tcb *)cma_c_null_ptr) {
	output[0] = '\0';
	cma__putstring (output, "%There is no thread with number ");
	cma__putint (output, thd_num);
	cma__puteol (output);
	return;
	}

    if (tcb->kind == cma__c_thkind_null) {
	cma__putstring (output, "%Null thread cannot be modified.");
	cma__puteol (output);
	}

    if (set_cancel)
	(void)cma__deb_set_alert (tcb);

    if (set_hold) {
	boolsetbuf = cma_c_true;
	cma__deb_set (
		tcb,
		cma_c_debset_hold,
		(cma_t_address)&boolsetbuf,
		(cma_t_integer)(sizeof (cma_t_boolean)));
	}

    if (set_nohold) {
	boolsetbuf = cma_c_false;
	cma__deb_set (
		tcb,
		cma_c_debset_hold,
		(cma_t_address)&boolsetbuf,
		(cma_t_integer)(sizeof (cma_t_boolean)));
	}

    if (set_active) {
#if _CMA_THREAD_IS_VP_
	/*
	 * On a one-to-one VP implementation, all threads are concurrently
	 * active, so this switch can't do anything useful.
	 */
#else
	cma__deb_set_next_thread (tcb);
#endif
	}

    if (set_visible) {
#if !_CMA_THREAD_IS_VP_
	cma__putstring (output, "'set visible' not implemented; -v ignored");
	cma__puteol (output);
#else
	cma__vp_interrupt (
		tcb->sched.processor->vp_id,
		(cma__t_vp_handler)cma__do_break,
		cma_c_null_ptr);
#endif
	}

    if (set_sched) {

	if (policy_present)
	    cma__deb_set (
		    tcb,
		    cma_c_debset_policy,
		    (cma_t_address)&sched_policy,
		    (cma_t_integer)(sizeof (cma_t_sched_policy)));
	else {
	    /*
	     * If policy wasn't specified on command line, then we need to do
	     * keyword interpretation or range checking.
	     */
	    sched_policy = tcb->sched.policy;	/* Fetch actual policy */

	    if (sched_prio_is_key)
		sched_priority
		    = pri_range[sched_prio_key][(int)sched_policy];
	    else {

		if (sched_priority < pri_range[0][(int)sched_policy]
			|| sched_priority > pri_range[2][(int)sched_policy]) {
		    cma__putstring (output, "%Priority ");
		    cma__putint (output, sched_priority);
		    cma__putstring (output, " is not valid for the ");
		    cma__putstring (output, policy[(int)sched_policy]);
		    cma__putstring (output, " policy; range is ");
		    cma__putint (output, pri_range[0][(int)sched_policy]);
		    cma__putstring (output, " to ");
		    cma__putint (output, pri_range[2][(int)sched_policy]);
		    cma__putstring (output, ".");
		    cma__puteol (output);
		    return;
		    }

		}

	    }

	cma__deb_set (
		tcb,
		cma_c_debset_priority,
		(cma_t_address)&sched_priority,
		(cma_t_integer)(sizeof (cma_t_priority)));
	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Show DCE Threads information
 *
 *  FORMAL PARAMETERS:
 *
 *	argc	Number of arguments to command
 *	argv	Array of argument strings
 *
 *  IMPLICIT INPUTS:
 *
 *	All sorts of random externs and stuff
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern void
cma__debug_show(
	cma_t_integer	argc,
	char		*argv[])
    {
    cma_t_integer	arg, i;
    cma_t_boolean	show_mem = cma_c_false;
#ifdef _CMA_TRACE_KERNEL_
    cma_t_boolean	show_kernel = cma_c_false;
#endif
#ifdef _CMA_TRACE_SEM_
    cma_t_boolean	show_sem = cma_c_false;
#endif
    cma_t_boolean	show_pool = cma_c_false;
    cma_t_boolean	show_vp = cma_c_false;
    char		buffer[120];


    buffer[0] = '\0';

    if (argc < 2) {
	cma__putstring (buffer, "Too few arguments");
	cma__puteol (buffer);
	return;
	}

    for (arg = 1; arg < argc; arg++) {

	if (argv[arg][0] == '-')	/* If a switch */
	    switch (argv[arg][1]) {
		case 'm' : {
		    show_mem = cma_c_true;
		    break;
		    }
		case 'p' : {
		    show_pool = cma_c_true;
		    break;
		    }
#ifdef _CMA_TRACE_KERNEL_
		case 'k' : {
		    show_kernel = cma_c_true;
		    break;
		    }
#endif
#ifdef _CMA_TRACE_SEM_
		case 's' : {
		    show_sem = cma_c_true;
		    break;
		    }
#endif
		case 'v' : {
		    show_vp = cma_c_true;
		    break;
		    }

		}

	}

    if (show_mem) {
	/*
	 * We're doing this without locking the VM database, since we don't
	 * know what state we're in. Watch it!
	 */
	cma__putstring (buffer, "DCE Threads internal VM manager statistics:");
	cma__puteol (buffer);
	for (i = 0; i < cma__c_pool; i++) {
	    cma__putformat (
		    buffer,
		    " %s (%d bytes): %d allocated, %d free",
		    vm_queue[i],
		    cma__g_memory[i].size,
		    cma__g_memory[i].total,
		    cma__g_memory[i].count);
	    cma__puteol (buffer);
	    }

	cma__putformat (
		buffer,
		" %d thing%s currently on general pool list",
		cma__g_memory[cma__c_pool].count,
		(cma__g_memory[cma__c_pool].count == 1 ? "" : "s"));
	cma__puteol (buffer);
#ifndef NDEBUG
	cma__putformat (
		buffer,
		" %d zeroed allocations (%d bytes)",
		cma__g_pool_stat.zero_allocs,
		cma__g_pool_stat.zero_bytes);
	cma__puteol (buffer);
	cma__putformat (
		buffer,
		" %d syscalls for %d bytes (%d exact size), %d attempts failed",
		cma__g_pool_stat.allocs,
		cma__g_pool_stat.bytes_allocd,
		cma__g_pool_stat.exact_allocs,
		cma__g_pool_stat.failures);
	cma__puteol (buffer);
	cma__putformat (
		buffer,
		" %d pool extractions; %d split from larger packets",
		cma__g_pool_stat.extractions,
		cma__g_pool_stat.breakups);
	cma__puteol (buffer);
	cma__putformat (
		buffer,
		" %d pool returns; %d merged with previous, %d with next",
		cma__g_pool_stat.returns,
		cma__g_pool_stat.merge_befores,
		cma__g_pool_stat.merge_afters);
	cma__puteol (buffer);
	cma__putformat (
		buffer,
		" Lookaside scrounging: %d small, %d medium, %d large",
		cma__g_pool_stat.scrounges[0],
		cma__g_pool_stat.scrounges[1],
		cma__g_pool_stat.scrounges[2]);
	cma__puteol (buffer);
#endif
#if _CMA_OS_ == _CMA__UNIX
	cma__putformat (buffer, " %d sbrk alignments", cma__g_sbrk_align);
	cma__puteol (buffer);
#endif
	}

    if (show_pool) {
	cma__t_queue	*p;
	cma_t_integer	width;


	cma__putstring (buffer, "General pool element list:");
	cma__puteol (buffer);
	p = cma__queue_next (&cma__g_memory[cma__c_pool].queue);
	width = 0;

	while (p != &cma__g_memory[cma__c_pool].queue) {
	    width += 16;

	    if (width >= 80) {
		cma__puteol (buffer);
		width = 16;
		}

	    cma__putstring (buffer, " ");
	    cma__puthex_8 (buffer, (cma_t_integer)p);
	    cma__putstring (buffer, "(");
	    cma__putint_5 (
		    buffer,
		    *(cma_t_natural *)((cma_t_natural)p - cma__c_mem_tag));
	    cma__putstring (buffer, ")");
	    p = cma__queue_next (p);
	    }

	cma__puteol (buffer);
	}

#ifdef _CMA_TRACE_KERNEL_
    if (show_kernel)
	cma__format_karray ();
#endif

#ifdef _CMA_TRACE_SEM_
    if (show_sem)
	cma__sem_format_array ();
#endif

    if (show_vp)
	cma__vp_dump ();

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Lock the kernel (if possible) and do other setup for main debug entry.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	Kernel lock
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	previous state of the kernel lock
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma_t_boolean
cma__debug_trylock(void)
    {
    cma_t_boolean	locked;
    char		output[cma__c_buffer_size];


    locked = cma__tryenter_kernel ();	/* Enter krnl */

    if (locked) {
	/*
	 * If kernel was already locked, print warning, but continue anyway
	 */
	output[0] = '\0';
	cma__putstring (
		output,
		"  WARNING: The cma_debug() function has been called while");
	cma__puteol (output);
	cma__putstring (
		output,
		"  the threads library was active, and the state of thread");
	cma__puteol (output);
	cma__putstring (
		output,
		"  library objects may not be consistent. You may continue");
	cma__puteol (output);
	cma__putstring
		(output,
		"  and examine the current state if you wish, but you may");
	cma__puteol (output);
	cma__putstring
		(output,
		"  not change anything. If possible, please exit from the");
	cma__puteol (output);
	cma__putstring
		(output,
		"  cma_debug() function now, and step (or set a breakpoint)");
	cma__puteol (output);
	cma__putstring
		(output,
		"  outside of the thread library before entering cma_debug().");
	cma__puteol (output);
	cma__puteol (output);
	}

    active_regs[14] = (long int)cma__fetch_sp ();
    return locked;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Suspend or resume all other VPs.
 *
 *  FORMAL PARAMETERS:
 *
 *	argc	Number of arguments to command
 *	argv	Array of argument strings
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern void
cma__debug_vp(
	cma_t_integer	argc,
	char		*argv[])
    {
    cma_t_boolean	suspend_all = cma_c_false;
    cma_t_boolean	resume_all = cma_c_false;
    cma_t_integer	arg;
    char			output[cma__c_buffer_size];


    output[0] = '\0';

    for (arg = 1; arg < argc; arg++)
	if (argv[arg][0] == '-')	/* If a switch */
	    switch (argv[arg][1]) {
		case 'r' : {
		    resume_all = cma_c_true;
		    break;
		    }
		case 's' : {
		    suspend_all = cma_c_true;
		    break;
		    }
		}

    if (suspend_all && resume_all) {
	cma__putstring (output, "%Conflicting switches");
	cma__puteol (output);
	return;
	}

    if (suspend_all)
	(void)cma__vp_suspend_others ();
    else if (resume_all)
	(void)cma__vp_resume_others ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEBUGGER.C */
/*  *28   16-APR-1992 12:40:31 BUTENHOF "Fix access to NOWRT strings" */
/*  *27   24-MAR-1992 13:45:49 BUTENHOF "Put bugcheck output in file" */
/*  *26   17-MAR-1992 09:55:41 BUTENHOF "Fix formatting of a message" */
/*  *25    5-MAR-1992 12:06:23 BUTENHOF "Add to VP debugging" */
/*  *24   19-FEB-1992 07:16:02 BUTENHOF "Fix show -p problem" */
/*  *23   18-FEB-1992 15:28:17 BUTENHOF "Add sbrk() alignment to UNIX memory report" */
/*  *22   13-DEC-1991 09:53:35 BUTENHOF "Fix SP report" */
/*  *21   22-NOV-1991 11:55:13 BUTENHOF "Type a cast" */
/*  *20   14-OCT-1991 13:38:02 BUTENHOF "Refine/fix use of config symbols" */
/*  *19   24-SEP-1991 16:26:37 BUTENHOF "Merge CMA5 reverse IBM/HP/Apollo drops" */
/*  *18   26-JUL-1991 15:53:17 CURTIN "Removed last of CRTL dependence on VMS" */
/*  *17    2-JUL-1991 16:47:03 BUTENHOF "Show mutex type on attributes object" */
/*  *16   21-JUN-1991 11:59:06 BUTENHOF "Add cast to puthex calls" */
/*  *15   10-JUN-1991 18:18:35 SCALES "Add sccs headers for Ultrix" */
/*  *14    5-JUN-1991 17:31:08 BUTENHOF "Fix bug in call to cma__vp_interrupt" */
/*  *13   29-MAY-1991 16:59:13 BUTENHOF "Accept command line as argument" */
/*  *12   14-MAY-1991 13:42:59 BUTENHOF "Integrate changes lost in disk crash" */
/*  *11   12-APR-1991 23:35:04 BUTENHOF "Fix access to atomic bits" */
/*  *10   21-MAR-1991 09:25:59 BUTENHOF "extern some static functions for Ada" */
/*  *9    14-MAR-1991 19:04:44 SCALES "Fix MIPS prototype problem" */
/*  *8    12-MAR-1991 17:24:01 BUTENHOF "Fix some MIPS compilation warnings" */
/*  *7    11-MAR-1991 19:06:56 BUTENHOF "Add new commands" */
/*  *6     8-MAR-1991 18:49:41 BUTENHOF "Add new commands" */
/*  *5    13-FEB-1991 17:54:36 BUTENHOF "Change mutex attribute name symbols" */
/*  *4    12-FEB-1991 23:09:42 BUTENHOF "Recursive/nonrecursive mutexes" */
/*  *3    12-FEB-1991 01:28:49 BUTENHOF "Change to alert control bits" */
/*  *2    14-DEC-1990 00:55:24 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:43:45 BUTENHOF "Debug support" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEBUGGER.C */
