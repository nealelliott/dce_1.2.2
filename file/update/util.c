/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi_port_mach.h>
#include <dcedfs/compat.h>		/* dfs_copyDceErrTxt */
#include <update.h>
#include <upcommon.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsudtmac.h>
#include <dfsudtsvc.h>
#include <dfsudtmsg.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

/* stripTrailing
 *	strips trailing slashes from a pathname
 */

stripTrailingSlash(ptr)
     char *ptr;
{
    char *startPtr;

    if ( (ptr == 0) || (strcmp(ptr,"/") == 0) )
	return;

    startPtr = ptr;
    ptr += strlen(ptr) - 1;
    while ( (*ptr == '/') && (ptr != startPtr) )
	ptr--;

    ptr++;
    *ptr = 0;
}

/* compatibleObjects
 *	compatibility check for adding an object to the upserver/upclient's
 *	list of objects.
 * exit:
 *	1 - yes. Objects are distinct
 *	0 - no. one object is contained in the other's tree.
 */

compatibleObjects(obj1P, obj2P)
     char *obj1P, *obj2P;
{
    long obj1length, obj2length;
    long minlength;
    char *maxPtr, *minPtr;

    if ( strcmp(obj1P, obj2P) == 0 )		/* identical */
	return(0);

    obj1length = strlen(obj1P);
    obj2length = strlen(obj2P);
    if ( obj1length < obj2length )
    {
	minlength = obj1length;
	maxPtr = obj2P;
	minPtr = obj1P;
    }
    else
    {
	minlength = obj2length;
	maxPtr = obj1P;
	minPtr = obj2P;
    }

    /* special case for root */
    if ( strcmp(minPtr, "/") == 0 )
	return(0);

    if ( strncmp(obj1P, obj2P, minlength) == 0 )
    {
	/* identical to a point */
	maxPtr += minlength;
	if ( *maxPtr == '/' )		/* subpath */
	    return(0);
    }
    return(1);
}


static int day = 0;

/* Log
 *	If logging is enabled, log a message to the log file, otherwise
 *	ignore the call.
 */

/*VARARGS*/
Log(stP,a,b,c,d,e,f,g,h,i,j)
      unsigned32 stP;
{
    struct timeval tp;
    struct tm *timePtr;
    extern upLogP logP;

    if ( logP == 0 )			/* no logging */
	return;

    osi_gettimeofday(&tp, (struct timezone *)0);
    timePtr = localtime((time_t *)&tp.tv_sec);

    /* if crossed a day boundary, or first time, print out a complete date
     * as a marker in the log
     */
    if ( day != timePtr->tm_mday ) {
	fprintf(logP->stream,"%s",ctime((time_t *)&(tp.tv_sec)));
	day = timePtr->tm_mday;
    }

    fprintf(logP->stream, "%02d:%02d:%02d ",
            timePtr->tm_hour, timePtr->tm_min, timePtr->tm_sec);
    dce_fprintf(logP->stream,stP,a,b,c,d,e,f,g,h,i,j);
    fflush(logP->stream);
}

/* LogError
 *	log both a message string and the results of decoding an error
 *	value.
 */

/*VARARGS*/
LogError(messageP, st, a,b,c,d,e,f,g,h,i,j)
     unsigned32 messageP;
     unsigned32 st;
{
    char errorMsg[NCSCOMPAT_ERR_BUF_SIZE];	/* error description string */
    unsigned32 callStatus;
    struct timeval tp;
    struct tm *timePtr;
    extern upLogP logP;

    if ( logP == 0 )			/* no logging */
	return;

    /* decipher the error */
    dfs_copyDceErrTxt(st, errorMsg, sizeof(errorMsg));

    osi_gettimeofday(&tp, (struct timezone *)0);
    timePtr = localtime((time_t *)&tp.tv_sec);

    /* if crossed a day boundary, or first time, print out a complete date
     * as a marker in the log
     */
    if ( day != timePtr->tm_mday ) {
	fprintf(logP->stream,"%s",ctime((time_t *)&(tp.tv_sec)));
	day = timePtr->tm_mday;
    }

    fprintf(logP->stream, "%02d:%02d:%02d ",
            timePtr->tm_hour, timePtr->tm_min, timePtr->tm_sec);
    dce_fprintf(logP->stream,messageP,a,b,c,d,e,f,g,h,i,j);
    fprintf(logP->stream, "  - %s\n", errorMsg);
    fflush(logP->stream);

}
