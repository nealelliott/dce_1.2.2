/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: private.h,v $
 * Revision 1.1.11.2  1996/02/18  23:11:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:37  marty]
 *
 * Revision 1.1.11.1  1995/12/08  18:07:28  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/26  13:10 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:23:55  root]
 * 
 * Revision 1.1.9.3  1994/08/23  20:31:56  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:29  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  20:11 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  21:25  pare
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.8.2  1993/07/08  19:40:04  root
 * 	Initial King Kong branch
 * 	[1993/07/08  19:39:34  root]
 * 	Revision 1.1.9.2  1994/05/13  19:27:27  cbrooks
 * 	CR 10631 - Zic compiler
 * 	[1994/05/13  19:03:53  cbrooks]
 * 
 * Revision 1.1.6.3  1993/04/06  17:34:21  truitt
 * 	Remerge with dce1.0.2 due to failure of RCS tree.
 * 	[1993/04/06  17:33:49  truitt]
 * 
 * Revision 1.1.6.2  1993/03/30  15:25:19  truitt
 * 	Remove re-define of FILENAME_MAX because 1) it was causing annoying
 * 	compiler warnings and 2) it wasn't doing anything.  The modules that
 * 	use this file (private.h) don't use FILENAME_MAX anyway, so the
 * 	redefinition is not necessary.
 * 
 * Revision 1.1.1.8  1993/03/11  22:31:00  osfrcs
 * 	Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.6.4  1993/02/01  22:33:06  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:40:22  hinman]
 * 
 * Revision 1.1.9.1  1994/05/12  15:17:16  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:27  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:33  rhw]
 * 
 * Revision 1.1.6.4  1993/02/01  22:33:06  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:40:22  hinman]
 * 
 * Revision 1.1.6.3  1992/12/30  20:39:36  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:08:14  htf]
 * 	Revision 1.1.7.2  1993/01/11  16:24:47  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.6.2  1992/09/29  20:44:43  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:02:48  sekhar]
 * 
 * Revision 1.1.2.3  1992/06/24  16:13:25  grober
 * 	Changes necessary to build on hpux.
 * 	[1992/06/22  15:52:40  grober]
 * 
 * Revision 1.1.3.3  1992/06/18  20:58:02  smythe
 * 	redefine FILENAME_MAX as MAXPATHLEN in cleaner form
 * 	[1992/06/18  15:36:40  smythe]
 * 
 * Revision 1.1.2.2  1992/04/14  20:31:06  melman
 * 	Did the ugly thing for a broken OSF/1 definition of unlink.
 * 	This should be removed when OSF/1 is fixed.
 * 
 * 	Add the following: (following comments are altered for this log)
 * 	 #ifdef __OSF__
 * 	 extern int	unlink P((char * filename)); / * fixes broken OSF/1 1.0.3 * /
 * 	 #else
 * 	 extern int	unlink P((const char * filename));
 * 	 #endif
 * 	[1992/04/14  20:30:17  melman]
 * 
 * Revision 1.1  1992/01/19  15:31:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef _PRIVATE_H

#define _PRIVATE_H

#ifdef SNI_SVR4
#define USG
#endif /* SNI_SVR4 */

/*
** This header is for use ONLY with the time conversion code.
** There is no guarantee that it will remain unchanged,
** or that it will remain at all.
** Do NOT copy it to any system include directory.
** Thank you!
*/

#include <sys/types.h>	/* for time_t */
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <unistd.h>

#ifndef TRUE
#define TRUE	1
#endif /* !defined TRUE */

#ifndef FALSE
#define FALSE	0
#endif /* !defined FALSE */


#endif /* !defined _PRIVATE_H */
