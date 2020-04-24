/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * @HP_DCE_VERSION@
 * @(#)Module: utc.h $Revision: 1.2.10.2 $ $Date: 1996/02/18 23:11:59 $
 */
/*
 * HISTORY
 * $Log: utc.h,v $
 * Revision 1.2.10.2  1996/02/18  23:11:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:53  marty]
 *
 * Revision 1.2.10.1  1995/12/08  18:08:20  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/06/05  15:18 UTC  mullan_s
 * 	Merge CHFts15134
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat5/1  1995/06/01  18:23 UTC  mullan_s
 * 	Merge utc_gettime_noshm.
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/2  1995/05/31  17:24 UTC  pare
 * 	Add function utc_gettime_noshm
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1994/11/15  19:08 UTC  pare
 * 	Add utc_getTCPtime() prototype.
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/26  13:25 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:24:04  root]
 * 
 * Revision 1.2.8.4  1994/09/15  17:46:12  tom
 * 	Fix timespec to aling with later POSIX, now delcare tv_sec as time_t.
 * 	Also change reltimespec tv_sec to time_t. (OT 12186)
 * 	[1994/09/15  16:25:27  tom]
 * 
 * Revision 1.2.8.3  1994/08/16  18:12:51  cbrooks
 * 	CR 11494
 * 	[1994/08/09  19:00:00  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  20:28 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.7.4  1993/12/07  18:53:46  hopkins
 * 	Make timespec stuff work for both 700 and 800,
 * 	for 9.0 and 10.0 releases ...
 * 	[1993/11/03  19:44:08  hopkins]
 * 
 * Revision 1.1.7.3  1993/09/23  17:28:50  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/23  17:28:12  pare]
 * 
 * Revision 1.1.8.3  1993/09/15  17:46:47  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.2  1993/07/08  19:43:09  root
 * 	Initial King Kong branch
 * 	[1993/07/08  19:42:41  root]
 * 
 * Revision 1.1.5.7  1993/06/03  17:41:54  ahop
 * 	add module name to rev control
 * 	[1993/06/03  17:41:21  ahop]
 * 
 * 	add rev control
 * 	[1993/06/03  17:40:27  ahop]
 * 
 * Revision 1.1.5.6  1993/06/03  17:15:07  tatsu_s
 * 	Do not include <sys/timers.h> unless __hp9000s700 is defined.
 * 	The series 800 is not AES compliant.
 * 	[1993/06/03  17:10:57  tatsu_s]
 * 
 * Revision 1.1.5.5  1993/05/18  20:55:05  tatsu_s
 * 	Allow the inclusion of both cma_px.h and utc.h .
 * 	[1993/05/18  16:40:29  tatsu_s]
 * 
 * 	Need to define "struct timespec", #if defined(__hpux) &&
 * 	!defined(_INCLUDE_AES_SOURCE).
 * 	[1993/05/17  14:47:01  tatsu_s]
 * 
 * 	Do the timespec thing right.
 * 	[1993/05/14  20:25:14  tatsu_s]
 * 
 * Revision 1.1.5.4  1993/05/05  15:40:10  truitt
 * 	Nevermind.  Leave the timers.h include as it was.
 * 	[1993/05/05  15:39:49  truitt]
 * 
 * Revision 1.1.5.3  1993/05/05  15:17:24  truitt
 * 	Ensure timers.h gets sourced if we are on an hpux node.
 * 	[1993/05/05  15:17:00  truitt]
 * 
 * Revision 1.1.5.2  1993/04/05  17:45:24  truitt
 * 	Recreate GAMERA branch.
 * 	[1993/04/05  17:44:47  truitt]
 * 
 * Revision 1.1.5.2  1993/03/19  14:41:25  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/19  14:38:51  truitt]
 * 
 * Revision 1.1.1.8  1993/03/11  22:31:52  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.2  1993/03/03  15:27:47  truitt
 * 	Changed the conditions around how the function prototypes are
 * 	defined.  For C++ applications, ifdef __STDC__ is not correct.
 * 	By changing it to ifdef _PROTOTYPES instead, we continue to
 * 	support C applications, as well as making C++ applications work.
 * 	This had to be hpux specific, however, resulting in a fairly
 * 	long ifdef statement.
 * 	[1993/03/03  15:26:02  truitt]
 * 
 * Revision 1.2.8.2  1994/05/12  15:17:34  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:32  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:49  rhw]
 * 
 * Revision 1.2.8.1  1994/04/21  22:05:16  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:05:21  cbrooks]
 * 
 * Revision 1.2.4.5  1993/02/03  16:11:00  tom
 * 	7068 - Check AES_SOURCE not __OSF__ to see if we can include
 * 	sys/timers.h for the timespec struct.
 * 	[1993/02/02  15:44:52  tom]
 * 
 * Revision 1.2.4.4  1993/01/19  16:10:12  jd
 * 	Back out the change untill we know all of the dependencies. It's
 * 	causing no end of problems with the nightly builds.
 * 	[1993/01/19  14:44:15  jd]
 * 
 * Revision 1.2.4.2  1992/12/30  20:40:12  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:08:45  htf]
 * 
 * Revision 1.2.2.3  1992/05/11  13:34:41  comuzzi
 * 	Fix conditional so that it compiles on DEC's OSF/1. Fixes defect 3348.
 * 	[1992/05/08  16:05:55  comuzzi]
 * 
 * Revision 1.2.2.2  1992/01/22  22:34:01  jim
 * 	 Changes to build on the RIOS platform.
 * 	[1992/01/22  21:24:09  jim]
 * 
 * Revision 1.2  1992/01/19  22:14:51  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
 *  OSF DCE Version 1.0 
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED. 
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 */

#ifndef _UTC_H
#define _UTC_H 1

#include <time.h>
#include <stddef.h>		/* For size_t */

#ifndef _UTC_T_
#define _UTC_T_
#include <dce/utctypes.h>
#endif /* _UTC_T_ */

enum utc_cmptype {
    utc_lessThan,
    utc_equalTo,
    utc_greaterThan,
    utc_indeterminate
};

/*  
 *  We need a definition of struct timespec, the OSF/1 AES defines it in
 *  sys/timers.h.  If using an AES compliant system include that file, 
 *  else use the definition below.  This is so utc.h and sys/timers.h 
 *  can be included in any order.
 */

#ifdef __hpux

#include <dce/hpdce_platform.h>

#if !defined(_STRUCT_TIMESPEC)
# if __hpux1000p || defined(hp9000s700) || defined(__hp9000s700)
#  include <sys/timers.h>
#  if !defined(_STRUCT_TIMESPEC)
#   define _STRUCT_TIMESPEC
#  endif
# else
#  define _STRUCT_TIMESPEC
   struct timespec {
       unsigned long	tv_sec;		/* seconds */
       long		tv_nsec;	/* and nanoseconds */
   };
# endif
#endif

#if !defined(_TIMESPEC_T_)
# define _TIMESPEC_T_
  typedef struct timespec timespec_t;
#endif

#else	/* !__hpux */

#if defined(_AES_SOURCE)
#include <sys/timers.h>
#else
#ifndef _TIMESPEC_T_
#define _TIMESPEC_T_
typedef struct timespec {
    time_t tv_sec;
    long   tv_nsec;
} timespec_t;
#endif	/* _TIMESPEC_T_ */
#endif  /* _AES_SOURCE */

#if !defined(_TIMESPEC_T_) && !defined(_TIMESPEC)
#define _TIMESPEC_T_
typedef struct timespec timespec_t;
#endif	/* _TIMESPEC_T_ */

#endif	/* __hpux */

#ifndef _RELTIMESPEC_T_
#define _RELTIMESPEC_T_
typedef struct reltimespec
{
    time_t	tv_sec;
    long 	tv_nsec;
} reltimespec_t;
#endif  /* _RELTIMESPEC_T_ */

#define UTC_MAX_STR_LEN	50	/* Max length of string used by conversion
				   routines (utc_asc...) */

int utc_gettime(utc_t *);
int utc_bintime(timespec_t *, timespec_t *, long *, utc_t *);
int utc_mkbintime(utc_t *, timespec_t *,  timespec_t *, long );
int utc_binreltime(reltimespec_t *, timespec_t *, utc_t *);
int utc_mkbinreltime(utc_t *, reltimespec_t *, timespec_t *);
int utc_localtime(struct tm *, long *, struct tm *, long *, utc_t *);
int utc_mklocaltime(utc_t *, struct tm *, long , struct tm *, long );
int utc_localzone(char *, size_t , long *, int *, utc_t *);
int utc_anyzone(char *, size_t , long *, int *, utc_t *);
int utc_gmtzone(char *, size_t , long *, int *, utc_t *);
int utc_gmtime(struct tm *, long *, struct tm *, long *, utc_t *);
int utc_mkgmtime(utc_t *, struct tm *, long , struct tm *, long );
int utc_anytime(struct tm *, long *, struct tm *, long *, long *, utc_t *);
int utc_mkanytime(utc_t *, struct tm *, long , struct tm *, long , long );
int utc_reltime(struct tm *, long *, struct tm *, long *, utc_t *);
int utc_mkreltime(utc_t *, struct tm *, long , struct tm *, long );
int utc_abstime(utc_t *, utc_t *);
int utc_addtime(utc_t *, utc_t *, utc_t *);
int utc_subtime(utc_t *, utc_t *, utc_t *);
int utc_multime(utc_t *, utc_t *, long );
int utc_mulftime(utc_t *, utc_t *, double );
int utc_cmpmidtime(enum utc_cmptype *, utc_t *, utc_t *);
int utc_cmpintervaltime(enum utc_cmptype *, utc_t *, utc_t *);
int utc_boundtime(utc_t *, utc_t *, utc_t *);
int utc_spantime(utc_t *, utc_t *, utc_t *);
int utc_pointtime(utc_t *, utc_t *, utc_t *, utc_t *);
int utc_getusertime(utc_t *);
int utc_ascgmtime(char *, size_t , utc_t *);
int utc_asclocaltime(char *, size_t , utc_t *);
int utc_ascanytime(char *, size_t , utc_t *);
int utc_ascreltime(char *, size_t , utc_t *);
int utc_mkasctime(utc_t *, char *);
int utc_mkascreltime(utc_t *, char *);
int utc_getTCPtime(char *, utc_t *);
int utc_gettime_noshm(utc_t *);
                                                             
#endif /* _UTC_H */
