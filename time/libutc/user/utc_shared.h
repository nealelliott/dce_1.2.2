/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: utc_shared.h,v $
 * Revision 1.1.8.2  1996/02/18  23:12:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:37:14  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:13:04  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  13:09 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1995/12/08  17:25:19  root]
 * 
 * Revision 1.1.6.1  1994/05/12  15:17:49  rhw
 * 	code cleanup
 * 	[1994/05/09  20:01:14  rhw]
 * 
 * Revision 1.1.5.2  1993/07/08  20:00:16  root
 * 	Initial King Kong branch
 * 	[1993/07/08  19:59:49  root]
 * 
 * Revision 1.1.3.3  1993/06/08  18:43:50  truitt
 * 	Add prototype call for new utc_semcleanup() function.
 * 	[1993/06/08  18:42:56  truitt]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/30  19:22:09  htf]
 * 
 * Revision 1.1.3.2  1993/04/27  14:06:14  truitt
 * 	Changed conditions around how the function prototypes are defined.
 * 	If hpux is defined, then test on _PROTOTYPES instead of __STDC__.
 * 	This will allow c++ to operate correctly.
 * 	[1993/04/27  14:02:21  truitt]
 * 
 * 	Fixed endian problem in user mode implmentation. OT defect 3676.
 * 	[1992/05/20  15:16:21  comuzzi]
 * 
 * Revision 1.1.1.9  1993/04/21  20:51:50  root
 * 	    Loading drop DCE1_0_2B23
 * 
 * Revision 1.1  1992/01/19  15:35:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module utc_shared.h
 *	Version X0.6
 */
/*
 * Copyright (c) 1990 by
 * Digital Equipment Corporation, Maynard, Mass.
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 */

#ifndef _UTC_SHARED_H 
#define _UTC_SHARED_H 1 


#include <dce/dce.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <utctime.h>
#include <dtssdef.h>

#define INIT_KEY	(1)

#define SHARED_MEMORY_VERSION	(1)
#define SHARED_MEMORY_DATAFILE	"/var/adm/time/dts_shared_memory_id"

typedef struct State {
    int	version;
    int shmemid;
    time_t timestamp;
    int writelkid;
    int readlkid;
    Bits64 adjbase;
    Bits64 adjend;
    Bits64 driftbase;
    Bits64 inacc;
    Bits64 leaptime;
    int inaccinfinite;
    int curtdf;
    int nextdf;
    time_t tdftime;
    long currentmdr;
    long resolution;
    long adjrate;
} SharedState;

int utc_sharedmap(int);
void utc_sharedunmap(void);
SharedState *utc_sharedmapid(int , int );
int utc_sharedcreate(void);
void utc_shareddelete(void);
void utc_writelock(void);
void utc_writeunlock(void);
void utc_readlock(void);
void utc_readunlock(void);

#endif /* _UTC_SHARED_H */

void utc_semcleanup(void);

/*  DEC/CMS REPLACEMENT HISTORY, Element UTC_SHARED.H */
/*  *5     8-AUG-1991 16:43:50 COMUZZI "Changed filename per Norbert's doc" */
/*  *4     3-OCT-1990 16:28:21 COMUZZI "Fixed restart problem" */
/*  *3     2-OCT-1990 21:23:34 COMUZZI "added locks" */
/*  *2     1-OCT-1990 12:08:52 COMUZZI "Fixed procedure declaration" */
/*  *1     1-OCT-1990 11:43:17 COMUZZI "Shared memory include data" */
/*  DEC/CMS REPLACEMENT HISTORY, Element UTC_SHARED.H */
