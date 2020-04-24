/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_defs.h $Revision: 1.1.15.1 $ $Date: 1996/05/10 13:35:56 $
 */
/*
 * HISTORY
 * $Log: cma_defs.h,v $
 * Revision 1.1.15.1  1996/05/10  13:35:56  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/08  21:40 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  15:18  truitt  /main/HPDCE01/1]
 *
 * 	Change polarity of ifdef for including cma performance fixes
 * 	[1993/10/29  18:37:51  ajayb]
 * 	 *
 *
 * 	Add proper ifdefs to include DEPT changes.
 * 	[1993/10/19  19:36:50  ajayb]
 *
 * 	Merging DEPT changes
 * 	[1993/09/30  15:10:47  prepetit]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  12:01:28  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:21  root  1.1.10.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:12:03  hopkins]
 *
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code)
 * 	[1993/06/17  14:43:25  hinman]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:06:54  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:33:06  hopkins]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:54:17  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:26:46  osfrcs  1.1.1.8]
 *
 * 	DCE 1.0.1 Release
 * 	[1992/09/23  20:37:36  hopkins]
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  18:49 UTC  psn
 * 	Remove AIX_CORE_FILE ifdef to avoid problem described in OSF OT CR 13204
 * 	[1995/11/16  21:07 UTC  psn  /main/DCE1_1_WP/jrr_1.2_mothra/1]
 *
 * 	Changes to allow valid AIX core dumps.
 * 	[1995/04/26  16:56 UTC  jrr  /main/DCE1_1_WP/1]
 *
 * Revision 1.1.6.4  1994/06/09  13:37:50  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:49  devsrc]
 * 
 * Revision 1.1.6.3  1994/04/25  19:57:42  ohara
 * 	code cleanup
 * 	[1994/04/25  18:17:21  ohara]
 * 
 * 	svr4 code cleanup
 * 
 * Revision 1.1.6.2  1994/04/01  20:16:09  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:12  jd]
 * 
 * Revision 1.1.6.1  1994/01/20  22:05:40  ohara
 * 	add dce to include
 * 	[1994/01/20  22:04:51  ohara]
 * 
 * Revision 1.1.4.2  1993/06/24  20:51:44  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:43:25  hinman]
 * 
 * Revision 1.1.2.5  1992/12/15  22:27:48  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:38:01  alan]
 * 
 * Revision 1.1.2.4  1992/12/07  13:21:16  zeliff
 * 	Removed duplicate OSF copyright markers
 * 	[1992/12/06  00:45:17  zeliff]
 * 
 * Revision 1.1.2.3  1992/09/29  20:14:51  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:41:13  sekhar]
 * 
 * 	Fix typo introduced in previous revision.
 * 	[1992/08/28  18:30:08  bolinger]
 * 
 * 	Bring forward IBM-specific #ifdef's from 1.0.
 * 	[1992/08/10  15:20:59  bolinger]
 * 
 * Revision 1.1.2.2  1992/09/03  14:36:41  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:53  bolinger]
 * 
 * Revision 1.1.6.2  1992/08/28  20:45:08  bolinger
 * 	Move latest 1.0.1-based revision into 1.0.2.
 * 	[1992/08/28  20:41:41  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:13:48  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:13:32  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *      %W%     (DEC OSF/1)    %G%
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
 *	Concert Multithread Architecture (CMA) services
 *
 *  ABSTRACT:
 *
 *	General CMA facility definitions
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	24 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	15 August 1989
 *		Add cma__t_short type, and common object header
 *	002	Dave Butenhof	16 August 1989
 *		Remove boolean true/false and null pointer constants to
 *		CMA.h, since they're externally useful, too.
 *	003	Dave Butenhof	24 August 1989
 *		Modify object header to include generic queue element.
 *	004	Dave Butenhof	15 September 1989
 *		Add "attr_revision" count to object header for queue
 *		validation.
 *	005	Dave Butenhof	22 September 1989
 *		Remove erroneous semicolons in #defines (hmmm...)
 *	006	Bob Conti 	6 October 1989
 *		Add assertions interfaces to make them easy to use.
 *	007	Dave Butenhof	19 October 1989
 *		Add new macro cma__structure_base for backing off from
 *		embedded queue elements to structure head.
 *	008	Bob Conti 	4 November 1989
 *		Make cma_host.h an implicit include file for all modules.
 *	009	Webb Scales	19 November 1989
 *		Move cma__zero from cma_thread_io_low.h to here.
 *	010	Webb Scales	11 January 1990
 *		Add cma__copy macro.
 *	011	Dave Butenhof	10 April 1990
 *		Add object name field to object header, and macro to set it.
 *	012	Dave Butenhof	18 June 1990
 *		Add macros for clearing and testing header.name (clean up
 *		build for NDEBUG).
 *	013	Paul Curtin	3 July 1990
 *		Removed stack __c_ constants to cma_stack_int.h
 *	014	Paul Curtin	24 July 1990
 *		Adding generic cma__roundup macro.
 *	015	Paul Curtin	06 August 1990
 *		Replaced memcpy w/ cma__memcpy. Replaced memset w/
 *		cma__memset
 *	016	Paul Curtin	22 August 1990
 *		Added cma__max macro: return greater of two inputs
 *	017	Paul Curtin	11 September 1990
 *		Added cma__min macro: return lesser of two inputs
 *	018	Bob Conti	19 September 1990
 *		Add VAX Debug support.
 *	019	Webb Scales	10 October 1990
 *		Moved "idle" scheduling class constants here from cma.h
 *	020	Webb Scales	 6 December 1990
 *		Added HP page size constant.
 *	021	Dave Butenhof	14 December 1990
 *		Change cma_assertions.h to cma_assert.h (shorten length)
 *	022	Dave Butenhof	24 April 1991
 *		Fix cma__trace macro, and add some documentation for it.
 *	023	Dave Butenhof	03 May 1991
 *		Add new trace class, and allow this module to be included
 *		from assembly code (for trace class defs).
 *	024	Dave Butenhof	13 May 1991
 *		Add new cma__trace_kernel() to be used for tracing when
 *		inside the kernel (it will exit the kernel to prevent a
 *		possible deadlock if the thread needs to block).
 *	025	Paul Curtin	 5 June 1991
 *		Added new constants for reinit routines.
 *	026	Webb Scales	23 July 1991
 *		Added VMS versions of the printf routines.
 *	027	Dave Butenhof	25 July 1991
 *		Add vsprintf and vfprintf versions to 024, and replace by
 *		#defines to the "real" *printf functions on non-VMS systems.
 *	028	Dave Butenhof	24 September 1991
 *		When the object name field was first added, it evaporated in
 *		NDEBUG builds; therefore, names weren't set for NDEBUG
 *		builds. Since the name field long since became unconditional
 *		to solve alignment problems, we could have made the names
 *		unconditional, but didn't. Rectify that oversight now.
 *	029	Dave Butenhof	14 October 1991
 *		Fix comments about cma__t_header structure that incorrectly
 *		state it "can't be changed": the restriction is only for VMS
 *		systems, where VMS Debugger integration relies on fixed
 *		offsets.
 *	030	Dave Butenhof	30 January 1992
 *		Remove cma__trace_kernel, which isn't used much and is really
 *		too dangerous to leave around.
 *	031	Dave Butenhof	12 March 1992
 *		Add trace class to cover fork wrapper.
 */

#ifndef CMA_DEFS
#define CMA_DEFS

/*
 *  INCLUDE FILES
 */

#include <dce/cma_queue.h>
#include <dce/cma_host.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * Enumerated type for objects (only defined as constants since ENUMS can't
 * be stored in a word).
 */
#define cma__c_obj_attr 	1
#define cma__c_obj_cv		2
#define cma__c_obj_mutex	3
#define cma__c_obj_tcb		4
#define cma__c_obj_stack	5
/*
 * Define the number of object classes.  Note that it's actually one HIGHER
 * than the maximum object number... this allows an array to be indexed by
 * the actual type constants (which start at 1).  Too bad C doesn't allow
 * non-0-based array indices.
 */
#define cma__c_obj_num		6


/*
 * Private scheduling policy
 */
#define cma_c_prio_idle_min     0
#define cma_c_prio_idle_mid     0
#define cma_c_prio_idle_max     0
#define cma_c_sched_idle	((cma_t_sched_policy) 5)

/*
 * This macro computes the base of a structure, given a pointer to some field
 * within the structure, the name of the field, and the type of the
 * structure.  For example, it can be used to find the beginning of a TCB
 * structure when a TCB is removed from a timer queue.
 */
#define cma__base(pointer, field, type) \
    (type  *)((int)(pointer) - (int)(&(((type *)0)->field)))

/*
 * Copy N bytes of memory from one address to another
 */
#define cma__copy(src, dst, cnt) cma__memcpy ((char *)(dst), (char *)(src), (cnt))

/*
 * Zero a given data structure
 */
#define cma__zero(p)	cma__memset ((char *)(p), 0, sizeof (*(p)))

#ifndef _HP_CMA_PERF_
#define cma__memset(s,value,size)  				\
    {								\
    char		*ptr__ = s;				\
    cma_t_integer	i__;					\
    for (i__ = 0; i__ < size; i__++, ptr__++)		\
	*ptr__ = value;					\
    }
#endif /* _HP_CMA_PERF_ */
/*
 * Set the name of an object
 */
#define cma__obj_set_name(o,string) (((cma__t_object *)(o))->name = (string))

/*
 * Clear the name of an object
 */
#define cma__obj_clear_name(o) \
    (((cma__t_object *)(o))->name = (cma__t_string)cma_c_null_ptr)

/*
 * Test whether name is null
 */
#define cma__obj_null_name(o) \
    (((cma__t_object *)(o))->name == (cma__t_string)cma_c_null_ptr)

/*
 * Round first value (a) upto a value fully divisible by second value (b).
 */
#define cma__roundup(a,b)   ((((a) + ((b) - 1))/(b)) * (b))

/*
 * Return the greater of (a) and (b).
 */
#define cma__max(a,b) (((a) > (b)) ? (a) : (b))

/*
 * Return the lesser of (a) and (b).
 */
#define cma__min(a,b) (((a) < (b)) ? (a) : (b))

/*
 * Define simple spinlock based on test-and-set from cma_host.h
 */
#define cma__spinlock(lock)	while (cma__test_and_set (lock));
#define cma__spinunlock(lock)	cma__tac_clear (lock)

/*
 * Print trace message (if CMA_TRACE environment variable is defined to a
 * filepath, and if DECthreads is built without NDEBUG symbol).
 *
 * By convention, begin the trace message with the function name (minus the
 * "cma__" prefix) in parens. For example,
 *
 * 	cma__trace ((cma__c_trc_vp, "(vp_create) created vp %d", vpid->vp));
 */
#define cma__c_trc_assem	0x00000001	/* Assembly code trace */
#define cma__c_trc_attr		0x00000002	/* Attributes changes */
#define cma__c_trc_bug		0x00000004	/* More info before bugcheck */
#define cma__c_trc_cache	0x00000008	/* Object cache changes */
#define cma__c_trc_deb		0x00000010	/* Debug operations */
#define cma__c_trc_disp		0x00000020	/* Dispatcher operation */
#define cma__c_trc_io		0x00000040	/* I/O wrappers */
#define cma__c_trc_init		0x00000080	/* Initialization */
#define cma__c_trc_null		0x00000100	/* Null thread */
#define cma__c_trc_obj		0x00000200	/* Object creation/deletion */
#define cma__c_trc_sched	0x00000400	/* Scheduling changes */
#define cma__c_trc_stack	0x00000800	/* Trace stacks */
#define cma__c_trc_sync		0x00001000	/* Synchronization functions */
#define cma__c_trc_time		0x00002000	/* Timer operations */
#define cma__c_trc_tmp		0x00004000	/* Temporary debug traces */
#define cma__c_trc_vm		0x00008000	/* Virtual memory */
#define cma__c_trc_vp		0x00010000	/* Virtual Processors */
#define cma__c_trc_wrp		0x00020000	/* Assorted wrappers */

#if !defined(__LANGUAGE_ASSEMBLY__) && !defined(LANGUAGE_ASSEMBLY)

# ifndef NDEBUG
#  define cma__trace(args)		cma__trace_print args
# else
#  define cma__trace(args)
# endif

/*
 * reinit routine constants (flags).
 */
# define cma__c_reinit_prefork_lock	0
# define cma__c_reinit_postfork_unlock	1
# define cma__c_reinit_postfork_clear	2

/*
 * TYPEDEFS
 */

typedef char *		cma__t_string;
typedef short int	cma__t_short;

/*
 * Common header for all CMA objects
 *
 * NOTE for VMS support: This structure affects the positioning of the
 * standard prolog at the front of all TCBs. The VMS Debugger integration
 * requires that certain information be present at 32 byte offset in the
 * thread control structure; if changes are made here, compensating changes
 * must be made in cma__t_tcb_pad in file cma_tcb_def.h, and the maximum size
 * this structure can ever become is 32 bytes. This restriction affects VMS
 * only.
 */
typedef struct CMA__T_OBJECT {
    cma__t_queue	queue;		/* Queue element MUST BE FIRST */
    cma__t_short	sequence;	/* Sequence number */
    cma__t_short	type;		/* Type of object */
    cma_t_natural	revision;	/* Revision count of attr. obj */
    cma__t_string	name;		/* Name of object for debugging */
    } cma__t_object;

/*
 * This is included at the end because it's just an "implicit include" for
 * modules including cma_defs.h, and it depends on definitions that are
 * made here.p
 */

#ifdef __cplusplus
}
#endif

# include <cma_assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * INTERNAL INTERFACES
 */

# ifndef NDEBUG
extern void cma__trace_print  (int,char *,...);
# endif

# if (_CMA_UNIX_TYPE != _CMA__SVR4 )
#  define cma__int_output 	cma__int_fprintf
#  define cma__int_output_open  cma__int_fopen
# endif

# if _CMA_OS_ == _CMA__VMS
extern cma_t_integer cma__int_fprintf  (cma_t_address,char *,...);

extern cma_t_integer cma__int_printf  (char *,...);

extern cma_t_integer cma__int_sprintf  (char *,char *,...);

extern cma_t_integer cma__int_vfprintf  (cma_t_address,char *,cma_t_address);

extern cma_t_integer cma__int_vprintf  (char *,cma_t_address);

extern cma_t_integer cma__int_vsprintf  (char *,char *,cma_t_address);

# elif ( _CMA_UNIX_TYPE == _CMA__SVR4 )

#  include <stdio.h>
#   include <stdarg.h>

extern cma_t_integer cma__int_fprintf  (FILE *,char *,...);

extern cma_t_integer cma__int_printf  (char *,...);

extern cma_t_integer cma__int_sprintf  (char *,char *,	...);

extern cma_t_integer cma__int_vfprintf  (FILE *,char *,va_list);

extern cma_t_integer cma__int_vprintf  (char *,va_list);

extern cma_t_integer cma__int_vsprintf  (char *,char *,va_list ap);
# else
# ifdef _HP_LIBC_R
extern cma_t_integer cma__int_fprintf (cma_t_address, char *,...);

extern cma_t_integer cma__int_printf  (char *,...);

extern cma_t_integer cma__int_vfprintf  (cma_t_address,char *,__va_list);

extern cma_t_integer cma__int_vprintf  (char *,__va_list);

#  define cma__int_sprintf	sprintf
#  define cma__int_vsprintf	vsprintf


# else
#  define cma__int_fprintf	fprintf
#  define cma__int_printf	printf
#  define cma__int_sprintf	sprintf
#  define cma__int_vfprintf	vfprintf
#  define cma__int_vprintf	vprintf
#  define cma__int_vsprintf	vsprintf
# endif
# endif

#endif					/* __LANGUAGE_ASSEMBLY__ */

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEFS.H */
/*  *20   13-MAR-1992 14:08:35 BUTENHOF "Add tracing" */
/*  *19   30-JAN-1992 11:55:43 BUTENHOF "Get rid of trace_kernel" */
/*  *18   14-OCT-1991 13:38:21 BUTENHOF "Fix some misleading comments" */
/*  *17   24-SEP-1991 16:26:50 BUTENHOF "Unconditionalize object names" */
/*  *16   25-JUL-1991 13:59:44 BUTENHOF "Remove debug line in previous change" */
/*  *15   25-JUL-1991 13:53:32 BUTENHOF "Use cma__int_*printf functions" */
/*  *14   23-JUL-1991 20:19:58 SCALES "Add cma-printf routine protos" */
/*  *13    1-JUL-1991 16:58:09 SCALES "Put back SCCS header" */
/*  *12   17-JUN-1991 15:45:53 BUTENHOF "Make cma__trace_kernel a block" */
/*  *11   11-JUN-1991 17:16:37 BUTENHOF "cma__trace_kernel should do unset, not exit" */
/*  *10   10-JUN-1991 19:52:40 SCALES "Convert to stream format for ULTRIX build" */
/*  *9    10-JUN-1991 19:20:35 BUTENHOF "Fix the sccs headers" */
/*  *8    10-JUN-1991 18:21:26 SCALES "Add sccs headers for Ultrix" */
/*  *7     5-JUN-1991 16:12:51 CURTIN "fork work" */
/*  *6    14-MAY-1991 13:43:14 BUTENHOF "Integrate changes lost in disk crash" */
/*  *5    10-MAY-1991 17:15:53 BUTENHOF "Add ""assem"" trace class, allow as include" */
/*  *4     2-MAY-1991 13:58:07 BUTENHOF "Fix cma__trace macro" */
/*  *3    12-APR-1991 23:35:26 BUTENHOF "Add simple spinlock macro" */
/*  *2    14-DEC-1990 00:55:30 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:44:33 BUTENHOF "General definitions" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEFS.H */
