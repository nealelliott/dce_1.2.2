/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtssmacros.h,v $
 * Revision 1.1.38.2  1996/02/18  23:11:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:32  marty]
 *
 * Revision 1.1.38.1  1995/12/08  18:07:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:53  root]
 * 
 * Revision 1.1.36.7  1994/09/23  20:00:00  tom
 * 	Back out change of unsigned/signed64 to typedef hyper
 * 	[1994/09/23  19:26:48  tom]
 * 
 * Revision 1.1.36.5  1994/08/23  20:31:55  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:56:46  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:52:28  cbrooks]
 * 
 * Revision 1.1.36.4  1994/08/16  18:12:49  cbrooks
 * 	CR 11494
 * 	[1994/08/09  18:59:57  cbrooks]
 * 
 * Revision 1.1.36.3  1994/05/12  15:17:14  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:31  rhw]
 * 
 * Revision 1.1.36.2  1994/01/21  21:40:17  rsalz
 * 	Merged with changes from 1.1.36.1
 * 	[1994/01/21  21:40:00  rsalz]
 * 
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:25  rsalz]
 * 
 * Revision 1.1.36.1  1994/01/21  21:08:24  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:25  rsalz]
 * 
 * Revision 1.1.2.2  1992/12/30  20:39:30  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:08:04  htf]
 * 
 * Revision 1.1  1992/01/19  15:31:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * Copyright (c) 1989 by
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
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:	DTSS macros
 *
 * ENVIRONMENT:	portable
 *
 * AUTHORS:
 *		Carol Frampton
 *		Distributed Systems Services (DSS)
 *
 * DATE:	December 28, 1988
 *
 */

#ifndef DTSSMACROS_H
#define DTSSMACROS_H

/*
 * Check statusBlk for error status.
 */
#define IsError(statusBlk) 				\
    ((statusBlk.status) != DTSS_SUCCESS )

/*
 * Check statusBlk for successful status.
 */
#define IsSuccess(statusBlk) 				\
    (((statusBlk).status) == DTSS_SUCCESS )

/*
 * Given a OS specific status, fill in a DTSS status block and return.
 */
#define ReturnSysStatus(OSstatus, statusBlk) 		\
    { 							\
	if (OSstatus & 1)				\
	    statusBlk.status = DTSS_SUCCESS; 		\
	else 						\
	    statusBlk.status = DTSS_ERROR; 		\
	statusBlk.sysStatus = OSstatus; 		\
	return(statusBlk);				\
    }

/*
 * Given a DTSS specific status, fill in the DTSS status block and return.
 */
#define ReturnDTSSstatus(DTSStatus, statusBlk)		\
    {							\
	statusBlk.status = DTSStatus;			\
	return(statusBlk);				\
    }

/*
 * If the status block contains an error, return it.
 */
#define ReturnIfError(statusBlk)  			\
    {							\
	if (IsError(statusBlk))				\
	    return(statusBlk);				\
    }

/*
 * Check for TimeMaintainer configuration.
 * We assume that the caller has already determined that
 * ISSERVER is false.
 * Relies on global variable shmPtr
 */
#define IsMaintainer()					\
    (shmPtr->mgtParams.hasProvider == FALSE)

/*
 * Check for TimeProvider configuration.
 * We assume that the caller has already determined that
 * ISSERVER is true.
 * Relies on global variable shmPtr.
 */
#define IsProvider()					\
    (shmPtr->mgtParams.hasProvider == TRUE)

/*
 * Check for Server configuration.
 * Relies on global variable shmPtr.
 */
#define IsServer()			      		\
    (shmPtr->mgtParams.variant == VTserver)

/*
 * Check for Clerk configuration.
 * Relies on global variable shmPtr.
 */
#define IsClerk()					\
    (shmPtr->mgtParams.variant == VTclerk)

/*
 * Check for Courier configuration.
 * myCourierRole is updated from mgtParams.courierRole each time
 * the local set cache is updated, in order to keep my backup courier
 * status up to date.
 * Relies on global variable shmPtr.
 */
#define IsCourier()					\
    (shmPtr->mgtParams.myCourierRole == K_BACKUP_ROLE_COURIER)

/*
 * To determine wheter the status value is the current status value
 * of the dtss service.
 */
#define IsState( statusValue )				\
    (shmPtr->mgtParams.TSstate == statusValue )

/*
 * To determine whether the value of an extended inaccuracy represents
 * infinity - lower 48 bits are all 1's.
 * We need to cast it since pcc treats the Hex constant where all the 
 * bits are set, as -1.
 */
#if LONG_BIT >= 64
#define IsInfiniteInacc(extInaccPtr)                                    \
    ( *(extInaccPtr) >= (((unsigned long)(K_INFINITE_INACC_HI)<<32)| \
                                K_INFINITE_INACC_LO)  )
#else	/* LONG_BIT */
#define IsInfiniteInacc(extInaccPtr)					\
    (((extInaccPtr)->lo == (unsigned long)K_INFINITE_INACC_LO &&	\
      (extInaccPtr)->hi >= (unsigned long)K_INFINITE_INACC_HI )  ||	\
      (extInaccPtr)->hi >  (unsigned long)K_INFINITE_INACC_HI )
#endif	/* LONG_BIT */


/*
 * Leap Year
 *
 * Assumes the year >= 1582
 */
#define IsLeapYear(year)						  \
    (                                                                     \
       ( ((year) % 4 == 0) && ((year) % 100 != 0 ) ) ||                   \
       ((year) % 400 == 0) 				      		  \
     )									  

/*
 * BaseTimes update
 */
#define UpdateBaseTimes(TbasePtr,IbasePtr,NextLSPtr)           	\
    shmPtr->baseTimes.Tbase = *(UTCValue *)TbasePtr; 		\
    shmPtr->baseTimes.Ibase = *(UTCValue *)IbasePtr; 		\
    shmPtr->baseTimes.nextLS = *(UTCValue *)NextLSPtr;

/*
 * Memory management
 */
#define DTSSmallocStruct(mstr) (mstr *) DTSSmalloc (sizeof (mstr))



/* Trace()
 *
 * Currently always on.  We'll later have a symbol, for
 * instance which is passed on when building a KIT,
 * which we'll #ifdef to comment out the TRACED statement.
 */
#define Trace(code)					\
{							\
    code						\
}

/* TraceIf(flag, code)
 *
 * Same as the Trace macro, except that it checks one of several
 * flags in shared memory before executing {code}.
 *
 * TraceIf assumes that shmPtr is declared (usually extern) in
 * the calling module. The flag masks are defined in SHMEM.H.
 */
#define TraceIf(flag, code)				\
{							\
    if (shmPtr->mgtParams.traceFlags & (flag))		\
    {							\
        code						\
    }       						\
}

/*
 * BugCheck()
 * bugcheck is now system dependent - ultrix has bug; abort causes accvio
 */


#define BugCheck() 					\
    { 				                        \
        char msg[255+1];		        	\
							\
	(void) sprintf(msg,                             \
                       "\nFatal error at line %d in file %s\n", \
               __LINE__, 				\
               __FILE__);                               \
  	(void) fprintf(stderr, "%s", msg);		\
        (void) PrintMsgToOperator( msg );               \
	exit(-1);					\
    }

#define BugCheckOSstatus(OSstatus) 			\
    { 							\
        char OSmsg[K_MAX_OSMSG+1];			\
        char msg[K_MAX_OSMSG+80+1];                     \
	void abort _DCE_PROTOTYPE_( (void) );		\
                                                        \
        GetOsMsg((unsigned long)(OSstatus), OSmsg,	\
		 sizeof(OSmsg));    			\
	(void) sprintf(msg,                             \
                       "\nFatal error at line %d in file %s\n%s\n", \
		       __LINE__, 			\
		       __FILE__,			\
		       OSmsg);	                        \
  	(void) fprintf(stderr, "%s", msg);		\
	(void) PrintMsgToOperator(msg);			\
        (void) abort();					\
    }


#endif  /* #ifndef DTSSMACROS_H */
