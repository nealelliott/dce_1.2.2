/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_init.h $Revision: 1.1.11.1 $ $Date: 1996/05/10 13:37:54 $
 */
/*
 * HISTORY
 * $Log: cma_init.h,v $
 * Revision 1.1.11.1  1996/05/10  13:37:54  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:42 UTC  bissen
 * 	Merge from branch jss_single_thread.
 * 	[1994/12/22  19:58 UTC  jss  /main/HPDCE02/2]
 *
 * 	Changes for single threaded client. Added cma__is_multithreaded().
 * 	[1994/12/22  19:24 UTC  jss  /main/HPDCE02/jss_single_thread/2]
 *
 * 	Changes for single threaded client.
 * 	[1994/12/15  15:42 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:03  devsrc]
 * 	 *
 *
 * 	add dce to the include of cma_hosts
 * 	[1994/04/20  15:37:37  ohara]
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:46  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  16:01  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  12:56:34  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:37  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:12:31  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:20:37  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:33:35  hopkins]
 *
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:26:32  hinman]
 *
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:20:21  hinman]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:28:14  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:34:19  mgm  1.1.1.11]
 *
 * 	[raj] New naming + parallelized reentrant library changes.
 * 	[92/12/17  13:41:32  raj  9.5.1.3]
 *
 * 	[raj] Changes for new naming scheme with reentrant libraries.
 * 	[92/11/06  11:10:53  raj  9.5.3.2]
 *
 * 	[hinman] - Fold forward from raj_om
 * 	[92/09/10  16:33:02  hinman  9.5.1.2]
 *
 * 	[raj] merging SVR4 changes into 1.0.2
 * 	[92/09/10  16:02:49  raj  9.5.2.2]
 *
 * 	Update checkin
 * 	[92/09/02  14:03:52  hinman  9.9]
 *
 * Revision 1.1.6.4  1994/06/09  13:38:08  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:03  devsrc]
 * 
 * Revision 1.1.6.3  1994/04/25  19:57:48  ohara
 * 	add dce to the include of cma_hosts
 * 	[1994/04/20  15:37:37  ohara]
 * 
 * Revision 1.1.6.2  1994/04/01  20:16:31  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:46  jd]
 * 
 * Revision 1.1.2.4  1993/02/01  22:18:20  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:26:32  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:20:21  hinman]
 * 
 * Revision 9.5.1.3  92/12/17  13:41:32  raj
 * 	[raj] New naming + parallelized reentrant library changes.
 * 
 * Revision 9.5.3.2  92/11/06  11:10:53  raj
 * 	[raj] Changes for new naming scheme with reentrant libraries.
 * 
 * Revision 9.5.1.2  92/09/10  16:33:02  hinman
 * 	[hinman] - Fold forward from raj_om
 * 
 * Revision 9.5.2.2  92/09/10  16:02:49  raj
 * 	[raj] merging SVR4 changes into 1.0.2
 * 
 * Revision 9.9  92/09/02  14:03:52  hinman
 * 	Update checkin
 * 
 * Revision 1.1.2.2  1992/05/08  23:28:29  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:28:14  bolinger]
 * 
 * $EndLog$
 */
/*
 *	%W%	(DEC OSF/1)	%G%
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
 *	Header file for CMA initialization
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	11 September 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave butenhof	1 June 1990
 *		Add structure and global for environment definition code.
 *	002	Dave Butenhof	28 August 1990
 *		Change cma_shr_init to cma__shr_init, since it's really an
 *		internal interface (even though it's called from the client
 *		image and must be in the transfer vector, no client should
 *		call it directly).
 *	003	Dave Butenhof	22 January 1991
 *		Merge cma_init back into this module (should have been done a
 *		while ago when we removed the requirement that client link
 *		against cma_client.obj).
 *	004	Dave Butenhof	29 March 1991
 *		Change environment initialization stuff
 *	005	Paul Curtin	 7 May 1991
 *		Created cma__int_init macro.
 *	006	Dave Butenhof	19 September 1991
 *		Integrate HPUX CMA5 reverse drop: HP has apparently
 *		integrated DECthreads initialization into crt0.o, and
 *		therefore makes the optimization of defining cma__int_init()
 *		to expand to a null string.
 *	007	Dave Butenhof	14 November 1991
 *		Improve performance of cma__int_init() by doing an
 *		uninterlocked test first; then repeat with interlock if it
 *		doesn't appear to have been set to gain cache coherency.
 *	008	Dave Butenhof	22 November 1991
 *		Remove include of cma_kernel.h, which isn't needed: all it
 *		needs is cma_host.h (which should also avoid a circularity
 *		that seems to be confusing c89). Also, include cma_errors.h
 *		to pick up cma__error().
 *	009	Dave Butenhof	17 April 1992
 *		Define the extern version string.
 */

#ifndef CMA_INIT
#define CMA_INIT

/*
 *  INCLUDE FILES
 */
#include <dce/cma_host.h>
#include <cma_errors.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

#define cma__c_env_maxattr	0
#define cma__c_env_minattr	1
#define cma__c_env_maxcond	2
#define	cma__c_env_mincond	3
#define cma__c_env_maxmutex	4
#define cma__c_env_minmutex	5
#define cma__c_env_maxthread	6
#define cma__c_env_minthread	7
#define cma__c_env_maxcluster	8
#define cma__c_env_mincluster	9
#define cma__c_env_maxvp	10
#define cma__c_env_multiplex	11
#define cma__c_env_trace	12
#define cma__c_env_trace_file	13

#define cma__c_env_count	13

#define cma__is_multithreaded()  (cma__g_init_done2)


/*
 * cma__int_init
 *
 * Initialize the main body of CMA exactly once.
 *
 * We raise an exception if, for some odd reason, there are already threads
 * in the environment (e.g. kernel threads), and one of them is trying to
 * initialize CMA before the  first thread got all the way through the actual
 * initialization. This code maintains the invariants: "after successfully
 * calling CMA_INIT, you can call any CMA function", and  "CMA is actually
 * initialized at most once".
 */

#if (defined(SNI_SVR4) && !defined(CMA_INIT_NEEDED))
# define cma__int_init()
# define cma__int_init2()
#else
# define cma__int_init() { \
    if (!cma__tac_isset(&cma__g_init_started)) { \
	if (!cma__test_and_set (&cma__g_init_started)) { \
	    cma__init_static1 (); \
	    cma__test_and_set (&cma__g_init_done); \
	    } \
	else if (!cma__tac_isset (&cma__g_init_done)) { \
	    cma__error (cma_s_inialrpro); \
    }}}

# define cma__int_init2() { \
    if (!cma__tac_isset(&cma__g_init_started2)) { \
	if (!cma__test_and_set (&cma__g_init_started2)) { \
	    cma__init_static2 (); \
	    cma__test_and_set (&cma__g_init_done2); \
	    } \
	else if (!cma__tac_isset (&cma__g_init_done2)) { \
	    cma__error (cma_s_inialrpro); \
    }}}

#endif
/*
 * TYPEDEFS
 */

typedef enum CMA__T_ENV_TYPE {
    cma__c_env_type_int,
    cma__c_env_type_file
    } cma__t_env_type;

typedef struct CMA__T_ENV {
    char		*name;		/* Name of environment variable */
    cma__t_env_type	type;		/* Type of variable */
    cma_t_integer	value;		/* Numeric value of the variable */
    } cma__t_env;

/*
 *  GLOBAL DATA
 */

extern cma__t_env		cma__g_env[cma__c_env_count];
extern cma__t_atomic_bit	cma__g_init_started;
extern cma__t_atomic_bit	cma__g_init_done;

extern cma__t_atomic_bit	cma__g_init_started2;
extern cma__t_atomic_bit	cma__g_init_done2;

extern char			*cma__g_version;

/*
 * INTERNAL INTERFACES
 */
extern void
cma__init_static1 (void);	/* Initialize static data - single threaded */
extern void
cma__init_static2 (void);	/* Initialize static data - multi threaded */

#if _CMA_OS_ != _CMA__VMS
extern void cma__init_atfork (void);
#endif

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_INIT.H */
/*  *12   17-APR-1992 11:11:33 BUTENHOF "Add version number string" */
/*  *11   22-NOV-1991 11:56:19 BUTENHOF "Don't include cma_kernel.h" */
/*  *10   18-NOV-1991 11:23:51 BUTENHOF "Add non-interlocked test to cma__int_init()" */
/*  *9    24-SEP-1991 16:27:26 BUTENHOF "Merge CMA5 reverse IBM/HP/Apollo drops" */
/*  *8    10-JUN-1991 19:53:36 SCALES "Convert to stream format for ULTRIX build" */
/*  *7    10-JUN-1991 19:20:56 BUTENHOF "Fix the sccs headers" */
/*  *6    10-JUN-1991 18:22:05 SCALES "Add sccs headers for Ultrix" */
/*  *5     7-MAY-1991 10:10:52 CURTIN "created cma__int_init macro" */
/*  *4    12-APR-1991 23:35:56 BUTENHOF "Change type of internal locks" */
/*  *3     1-APR-1991 18:09:00 BUTENHOF "Change environment setup" */
/*  *2    24-JAN-1991 00:34:59 BUTENHOF "Get rid of cma_client module" */
/*  *1    12-DEC-1990 21:46:25 BUTENHOF "CMA initializer" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_INIT.H */
