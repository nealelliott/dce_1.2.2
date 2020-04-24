/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: clock_adjust.c,v $
 * Revision 1.1.6.2  1996/02/17  23:35:19  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:33  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:13:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:28  root]
 * 
 * Revision 1.1.4.7  1994/09/23  20:00:11  tom
 * 	Back out change of unsigned/signed64 to typedef hyper
 * 	[1994/09/23  19:26:55  tom]
 * 
 * Revision 1.1.4.5  1994/08/23  20:32:20  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:59  cbrooks]
 * 
 * Revision 1.1.4.4  1994/08/16  18:12:57  cbrooks
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:07  cbrooks]
 * 
 * Revision 1.1.4.3  1994/05/16  18:51:43  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:15  rhw]
 * 
 * Revision 1.1.4.2  1994/04/21  22:05:55  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:59:39  cbrooks]
 * 
 * 	Checkpoint:4.15.94
 * 	[1994/04/15  21:14:17  cbrooks]
 * 
 * 	Checkpoint
 * 	[1994/03/28  20:28:29  cbrooks]
 * 
 * Revision 1.1.4.1  1994/01/25  17:16:31  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/25  17:15:36  rsalz]
 * 
 * Revision 1.1.2.2  1992/12/30  16:37:42  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:20:03  htf]
 * 
 * Revision 1.1  1992/01/19  15:33:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	Module: CLOCK_ADJUST.C
 */
/*
 ****************************************************************************
 *									    *
 *  COPYRIGHT (c) 1988, 1989 BY						    *
 *  DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASSACHUSETTS.		    *
 *  ALL RIGHTS RESERVED.						    *
 * 									    *
 *  THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND COPIED   *
 *  ONLY IN  ACCORDANCE WITH  THE  TERMS  OF  SUCH  LICENSE  AND WITH THE   *
 *  INCLUSION OF THE ABOVE COPYRIGHT NOTICE. THIS SOFTWARE OR  ANY  OTHER   *
 *  COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY   *
 *  OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE IS  HEREBY   *
 *  TRANSFERRED.							    *
 * 									    *
 *  THE INFORMATION IN THIS SOFTWARE IS  SUBJECT TO CHANGE WITHOUT NOTICE   *
 *  AND  SHOULD  NOT  BE  CONSTRUED AS  A COMMITMENT BY DIGITAL EQUIPMENT   *
 *  CORPORATION.							    *
 * 									    *
 *  DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE  OR  RELIABILITY OF ITS   *
 *  SOFTWARE ON EQUIPMENT WHICH IS NOT SUPPLIED BY DIGITAL.		    *
 * 									    *
 *									    *
 ****************************************************************************
 */

/*
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:
 *
 *	This module contains the interfaces to the routines which manipulate
 *	the system's hardware clock.
 *
 * ENVIRONMENT: 	portable
 *
 * AUTHOR:		Carol Frampton
 *
 * CREATION DATE: 	February 1989
 *
 */


#include <limits.h>		/* for max. size of an integer */
#include <time.h>

#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>	/* top-level service header file which includes
				   prototypes for externally visible
				   clock_adjust.c routines */

#include <dcedtssvc.h>
#include <dce/dcedtsmsg.h>


extern ShmBuf *shmPtr;		/* pointer to shared memory which is
				   assumed to be mapped and locked
				   appropriately */

extern dce_svc_handle_t dts_svc_handle;

/*
 *	Statics
 */

/* change the declaration of this structure to be static, since pcc does not
 * currently support static const combination.  This line is commented out.
 *	static const ExtInacc infiniteInacc =
 *   		{ K_INFINITE_INACC_LO, K_INFINITE_INACC_HI };
 */

static ExtInacc infiniteInacc = UTCinit(K_INFINITE_INACC_LO,K_INFINITE_INACC_HI);

static int daysInMonth[12]
			= { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
 * the time that the last leap second was calculated as a UTC
 * value. Originally set to positive infinity.
 */
static UTCValue TlastLS = UTCinit(0xFFFFFFFFUL,0x7FFFFFFFUL);

/*
 * Prototypes for routines in the VMS Executive Shareable Library
 * which should be called only by this module.
 */


/*
 * CalcIntervalEndPoints()-
 *
 * Description:
 *
 *	Given the midpoint and the inaccuracy of a time interval, return
 *	the min. and max. end points of the interval.
 *
 * Inputs:
 *
 *	Tptr		- pointer to the midpoint
 *	Iptr		- pointer to the inaccuracy which may be infinite
 *	minPointPtr	- pointer to the lower bound end point element
 *	maxPointPtr	- pointer to the upper bound end point element
 *
 * Outputs:
 *
 *	the min and max end point elements
 *
 * Return Value:
 *
 *	none
 */

void CalcIntervalEndPoints (
    UTCValue	*Tptr,
    ExtInacc	*Iptr,
    EndPointElement	*minPointPtr,
    EndPointElement	*maxPointPtr
    )
{
    minPointPtr->type = min;
    maxPointPtr->type = max;

    /*
     * If the inaccuracy is infinite, the time interval is infinite.
     */
    if (IsInfiniteInacc (Iptr))
    {
 	minPointPtr->intervalSize = maxPointPtr->intervalSize = infinite;
        minPointPtr->value = maxPointPtr->value = *Tptr;
    }
    else
    {
	minPointPtr->intervalSize = maxPointPtr->intervalSize = non_infinite;

	UTCsub (Tptr, Iptr, &minPointPtr->value);
	UTCadd (Tptr, Iptr, &maxPointPtr->value);
    }
}

/*
 * CalcMidpointAndInacc()-
 *
 * Description:
 *
 *	Calculate the midpoint and inaccuracy of a time interval given the
 * 	min. and max. end points.  If one end point is infinite, they both
 *	should be infinite.
 *
 * Inputs:
 *
 *	minPointPtr	- pointer to the min. end point of the interval which
 *			  may be infinite
 *	maxPointPtr	- pointer to the max. end point of the interval which
 *			  may be infinite
 *	midPointPtr	- pointer to the interval's midpoint
 *	inaccPtr	- pointer to the interval's inaccuaracy
 *
 * Outputs:
 *
 *	the interval's midpoint and inaccuaracy
 *
 * Return Value:
 *
 *	none
 *
 * Side Effects:
 *
 *	If one end point is infinite, they both should be infinite.  BugCheck
 *	if this isn't the case.
 */

void CalcMidpointAndInacc (
    EndPointElement	*minPointPtr,
    EndPointElement	*maxPointPtr,
    UTCValue	*midPointPtr,
    ExtInacc	*inaccPtr
)

{
    UTCValue	halfMin, halfMax, lowBits;

    /*
     * If we are going to lose accuracy because of the divides by 2, save
     * it so we can add it back to the inaccuracy.  Each divide can lose
     * half of a 100ns unit so the most we will lose is 100ns.
     */

    UTCzero (&lowBits);

    if (UTClow(&minPointPtr->value) & 1 || UTClow(&maxPointPtr->value) & 1)
	UTClow(&lowBits) = 1;

    /*
     * Calculate the computed time interval as follows:
     *
     * midPoint = (maxInt + minInt) / 2
     *		= maxInt/2 + minInt/2 to avoid numeric overflow
     * inacc 	= (maxPointPtr->value - minPointPtr->value) / 2 + lowBits
     *	   where lowBits is added to the inaccuracy to cover the truncation
     *	   error which occured because of the two divides
     */
    UTCdivideBy2 (&minPointPtr->value, &halfMin);
    UTCdivideBy2 (&maxPointPtr->value, &halfMax);
    UTCadd (&halfMin, &halfMax, midPointPtr);

    /*
     * Only calculate the inaccuracy if the end points don't have infinite
     * inaccuracy.
     */
    if (minPointPtr->intervalSize == non_infinite)
    {
        if (maxPointPtr->intervalSize != non_infinite)
	    BugCheck();

	UTCsub (&maxPointPtr->value, &minPointPtr->value, inaccPtr);
	UTCdivideBy2 (inaccPtr, inaccPtr);
	UTCadd (inaccPtr, &lowBits, inaccPtr);
    }
    else
    {
	*inaccPtr = infiniteInacc;
    }
}

/*
 * CompareIntervalEndPoints() -
 *
 * Description:
 *
 *	Function which is called by qsort() to sort the end points of the time
 *	intervals.  In the case where two or more end points take on the
 *	same time value, those corresponding to the lower bounds must
 *	preceede those corresponding to the upper bounds.
 *
 * Inputs:
 *
 *	endPoint1	- pointer to the first end point being compared which
 *			  may be infinite
 *	endPoint2	- pointer to the second end point being compared which
 *			  may be infinite
 *
 * Outputs:
 *
 *	none
 *
 * Return Value:
 *
 *	-1	= endPoint1 < endPoint2
 *	0	= endPoint1 = endPoint2
 *	1	= endPoint1 > endPoint2
 *
 *	Note that the return values are dictated by the qsort() routine.
 */

int CompareIntervalEndPoints (
    const VOID		*endPoint1, /* void ptrs required by qsort() */
    const VOID 		*endPoint2
)
{
     EndPointElement	*point1, *point2;

    point1 = (EndPointElement *) endPoint1;
    point2 = (EndPointElement *) endPoint2;

    /*
     * Both end points are finite values so do regular arithmetic
     * comparisions to determine which end point is larger.
     */
    if (point1->intervalSize == non_infinite &&
        point2->intervalSize == non_infinite)
    {
	 /*
	  * end point 1 < end point 2
	  */
	 if ( IsUTClt (&point1->value, &point2->value) )
	    return (-1);

	 /*
	  * end point 1 > end point 2
	  */
	 if (IsUTCgt (&point1->value, &point2->value))
	    return (1);

	/*
	 * Both end points are equal.  If they are both mins or both maxs
	 * then return them as equal. Otherwise make sure the min is
	 * returned as less than the max.
	 */
	if (point1->type == point2->type)
	    return (0);

	if (point1->type == min)
	    return (-1);

	return (1);
    }

    /*
     * At least one end point is infinite.  If it's a min end point and it's
     * infinite, this represents the smallest value.  If it's a max end point
     * and it's infinite, this represents the largest value.
     *
     * Here is a chart of the possible combinations if one of the values is
     * finite.  The smaller of the two values in enclosed in '<>'.
     *
     *		finite value		infinite value
     *		------------		--------------
     *		<min>			max
     *		max			<min>
     *		min			<min>
     *		<max>			max
     */
    else
    {
	/*
	 * end point 1 is finite and end point 2 is infinite.
	 */
	if (point1->intervalSize == non_infinite)
	{
	    if (point2->type == max)
		return (-1);
	    else
		return (1);
	}

	/*
	 * end point 1 is infinite and end point 2 is finite.
	 */
	else if (point2->intervalSize == non_infinite)
        {
	    if (point1->type == min)
		return (-1);
	    else
		return (1);
	}

	/*
	 * end point 1 is infinite and end point 2 is infinite.
	 *
	 * Here is a chart of the possible combinations.  The smaller of the
	 * two values in enclosed in '<>'.
	 *
	 *		infinite value		infinite value
	 *		------------		--------------
	 *		<min>			max
	 *		max			<min>
	 *		min			min   >> use UTC to select
	 *		max			max   >> use UTC to select
	 */
	else
	{
	    if (point1->type == min && point2->type == max)
		return (-1);

	    if (point1->type == max && point2->type == min)
		return (1);

	    /*
	     * Both points are either mins or maxs.  Use the UTC value
	     * to decide.  This is a departure from the architecture because
	     * infinite intervals are not suppose to provide any information.
	     * This departure is helpful at startup when all servers will
	     * orginally have infinite intervals.
	     */
  	    if (IsUTClt (&point1->value, &point2->value))
		return (-1);

  	    if (IsUTCgt (&point1->value, &point2->value))
		return (1);

	    return (0);
	}
    }
}

/*
 * ComputedTimeMaximum() -
 *
 * Description:
 *
 *	Find the maximum end point of the best correct server interval, given
 *	M input time values and assuming up to f faulty servers.  Note that
 *	M - f is always greater than 0, by definition of M and f.
 *
 *	See Algorithm 3.10.2 in the architecture.
 *
 * Inputs:
 *
 *	see comments below on paramters
 *
 * Outputs:
 *
 *	none
 *
 * Return Value:
 *
 *	returns M-f but it may be smaller if there happen to be more than
 *	M faulty servers
 */

int ComputedTimeMaximum (
    int		M,			/* number of time values in the list */
    int		f,			/* max. faulty servers tolerated */
    EndPointElement	endPointArr[], /* end points already sorted */
    EndPointElement	**maxTimePtrPtr /* upper bound of computed interval */
					/* which may be infinite */
   )

{
    int		intersectingIntervals;	/* # intersecting at current point
					   in list */
    int		mostIntersections;	/* most intersections seen so far */
    int		i;

    /*
     * Loop until we either found the end point in M-f intervals or
     * we're done with the list.  We know that [0] is a minimum so don't
     * bother checking it.
     */
    intersectingIntervals = 0;
    mostIntersections = 0;

    for (i = 2*M - 1; (mostIntersections < M-f) && (i > 0); i--)
    {
        /*
         * The number of intersecting intervals is one larger than before
	 * this point.
         */
	if (endPointArr[i].type == max)
	{
	    intersectingIntervals++;

	    /*
	     * If the number of intersections is greater than we have seen
	     * before then make this the new end point the new computed
	     * minimum.
	     */
	    if (intersectingIntervals > mostIntersections)
	    {
		mostIntersections = intersectingIntervals;
		*maxTimePtrPtr = &endPointArr[i];
	    }
	}

	/*
	 * This element is a minimum end point so the number of intersecting
	 * intervals decreases by 1.
	 */
	else
	{
	    intersectingIntervals--;
	}
    }

    return (mostIntersections);
}

/*
 * ComputedTimeMinimum() -
 *
 * Description:
 *
 *	Find the minimum end point of the best correct server interval, given
 *	M input time values and assuming up to f faulty servers.  Note that
 *	M - f is always greater than 0, by definition of M and f.
 *
 *	See Algorithm 3.10.2 in the architecture.
 *
 * Inputs:
 *
 *	see comments below on paramters
 *
 * Outputs:
 *
 *	none
 *
 * Return Value:
 *
 *	returns M-f but it may be smaller if there happen to be more than
 *	M faulty servers
 */

int ComputedTimeMinimum (
int		M,			/* number of time values in the list */
int		f,			/* max. faulty servers tolerated */
EndPointElement	endPointArr[],		/* end points already sorted */
EndPointElement	**minTimePtrPtr	/* lower bound of computed interval */
					/* which may be infinite */
)
{
    int		intersectingIntervals;	/* # intersecting at current point
					   in list */
    int		mostIntersections;	/* most intersections seen so far */
    int		i;

    /*
     * Loop until we either found the end point in M-f intervals or
     * we're done with the list.  Since we know the last element's index is
     * 2*M - 1 and it has to be a maximum don't bother checking it.
     */
    intersectingIntervals = 0;
    mostIntersections = 0;

    for (i = 0; (mostIntersections < M-f) && (i < 2*M - 1); i++)
    {
        /*
         * The number of intersecting intervals is one larger than before
	 * this point.
         */
	if (endPointArr[i].type == min)
	{
	    intersectingIntervals++;

	    /*
	     * If the number of intersections is greater than we have seen
	     * before then make this the new end point the new computed
	     * minimum.
	     */
	    if (intersectingIntervals > mostIntersections)
	    {
		mostIntersections = intersectingIntervals;
		*minTimePtrPtr = &endPointArr[i];
	    }
	}

	/*
	 * This element is a maximum end point so the number of intersecting
	 * intervals decreases by 1.
	 */
	else
	{
	    intersectingIntervals--;
	}
    }

    return (mostIntersections);
}

/*
 * EstimateServerTime()-
 *
 * Description:
 *
 *	Estimate the server's time at the clerk's synchronization instant,
 *	<Tsync,Isync>.  The procedure is described in Section 3.1 of the
 *	architecture and the psuedo-code is in Section 3.10.1 of the
 *	architecture.
 *
 * Inputs:
 *
 *	see comments below on the input paramters
 *
 * Implicit Input:
 *
 *	infiniteInacc - constant equal to infinite inaccuracy
 *
 * Outputs:
 *
 *	TservPtr - pointer to the server's time at the clerk's synch
 *		   instant, Tsync
 *	IservPtr - pointer to the server's inaccuracy at the clerk's synch
 *		   instant which may be infinite
 *
 * Return Value:
 *
 *	DTSS_SUCESS - server time at Tsync was computed
 *	DTSS_ERROR - server time not computed because of arithmetic overflow
 */

unsigned long EstimateServerTime (
    UTCValue	*TsendPtr,	/* Tc(t1) - time request was sent to server */
    UTCValue	*TrespPtr,	/* Ts(t4) - time in response from server */
    ExtInacc	*IrespPtr,	/* Is(t4) - inaccuracy in resp from server */
    UTCValue	*procDelayPtr,	/* w - processing delay incurred by server */
    UTCValue	*TrecPtr,	/* Tc(t8) - time response received back */
    UTCValue	*TsyncPtr,	/* clerk clock time at sync instant */
    ExtInacc	*IsyncPtr,	/* clerk inaccuracy at sync instant */
    UTCValue	*TservPtr,	/* server clock time at sync instant */
    ExtInacc	*IservPtr	/* server inaccuracy at sync instant */
    )
{
    ExtInacc		extIresp;
    long		maxDriftReciprocal;     /* as a ratio,eg 1 per 10000 */
    long                maxDriftFactor;
    long                resolution;             /* in 100NS */
    UTCValue		elapsedTime, procDelayFactor, adjustBase,
			*nextLSptr;

    /*
     * Initialization.
     *
     * convert clockResolution into 100ns by dividing by 100
     * (clockParams.clockResolution is stored in NanoSeconds- PPB.
     * compute a maxdrift reciprocal, clockParams.clockDrift is
     * stored as parts per 10**9(billion), dividing 10**9 by the drift
     * generates the ratio
     */
    resolution = shmPtr->clockParams.clockResolution / 100;
    maxDriftReciprocal = K_ONE_BILLION/shmPtr->clockParams.clockDrift;

    /*
     * Determine the server's time at t1 and then map it to the time at Tsync.
     *
     * Tserv = Tresp -
     *	  	   (Trec + resolution - Tsend)*(1 + maxDrift)/2 + procDelay/2 -
     *	           Tsend + Tsync
     *	     = Tresp -
     *		   (elaspedTime + elapsedTime / maxDriftReciprocal)/2 +
     *		   procDelayFactor - Tsend + Tsync
     *	     = Tresp -
     *		   (elaspedTime + maxDriftFactor)/2 +
     *		   procDelayFactor - Tsend + Tsync
     *	     = Tresp -
     *		   elaspedTime + procDelayFactor - Tsend + Tsync
     *
     * The divide below shouldn't overflow unless someone manually
     * resets the clock.  Tsend and Trec should be within a few minutes of
     * each other at worst.  If it does overflow we have to abort the
     * current synch.
     */
    UTCiAdd (TrecPtr, resolution, &elapsedTime);
    UTCsub (&elapsedTime, TsendPtr, &elapsedTime);

    if ( /* overflow */ ediv(&elapsedTime, maxDriftReciprocal,
		        &maxDriftFactor, 0))
	return (DTSS_ERROR);

    UTCiAdd (&elapsedTime, maxDriftFactor, &elapsedTime);
    UTCdivideBy2 (&elapsedTime, &elapsedTime);

    UTCdivideBy2 (procDelayPtr, &procDelayFactor);

    UTCsub (TrespPtr, &elapsedTime, TservPtr);	/* Tresp - elapsedTime */
    UTCadd (TservPtr, &procDelayFactor, TservPtr);/* + procDelayFactor */
    UTCsub (TservPtr, TsendPtr, TservPtr);	/* - Tsend */
    UTCadd (TservPtr, TsyncPtr, TservPtr);	/* + Tsync = Tserv */

    /*
     * Determine the server's inaccuracy at t1 and then map it to the
     * inaccuracy at Isync.
     *
     * Iserv = Iresp +
     *	  	   (Trec + resolution - Tsend)*(1 + maxDrift)/2 - procDelay/2 +
     *	           (Tsync - Tssend)*maxDrift;
     */

	extIresp = *IrespPtr;

    /*
     * Make sure we aren't starting with an infinite inaccuarcy.  If we are
     * just return Iserv as infinite.
     */
    if (! IsInfiniteInacc (&extIresp))
    {
	UTCadd (&extIresp, &elapsedTime, IservPtr);
	UTCsub (IservPtr, &procDelayFactor, IservPtr);

        UTCsub (TsyncPtr, TsendPtr, &adjustBase);

	(void) ediv(&adjustBase, maxDriftReciprocal,
	            &maxDriftFactor, 0);

	UTCiAdd (IservPtr, maxDriftFactor, IservPtr);

	/*
	 * If a leap second occured between the time the server's response was
	 * received and the synchronization instant, than adjust the inaccuracy
	 * by one second.
	 */
	nextLSptr = &shmPtr->baseTimes.nextLS;
	if ( ! IsGreaterThanOrEqualNextLS (TrespPtr, &extIresp, nextLSptr) &&
	       IsGreaterThanOrEqualNextLS (TsyncPtr, IsyncPtr, nextLSptr))
	    AddLSToInacc (IservPtr);

	/*
	 * Make sure return nothing more than infinite inaccuracy is returned.
	 */
	if (IsInfiniteInacc (IservPtr))
	    *IservPtr = infiniteInacc;
    }

    /*
     * Since Iresp in infinite, Iserv is infinite.
     */
    else
    {
	*IservPtr = infiniteInacc;
    }

    /*
     * To debug the output of this routine.
     */
#ifdef	DCE_DEBUG
    if (DCE_SVC_DEBUG_ATLEAST(dts_svc_handle, dts_s_arith, 1))
	 PrintServerTimes(TsendPtr, TrespPtr, &extIresp, procDelayPtr,
	     TrecPtr, TsyncPtr, IsyncPtr, TservPtr, IservPtr);
#endif

    return (DTSS_SUCCESS);
}

/*
 * IsIntervalIntersection() -
 *
 * Description:
 *
 *	Returns TRUE if the two time intervals, specified as a midpoint and
 *	an inaccuracy, intersect each other.  Either inaccuracy may be
 *	infinite.
 *
 * Inputs:
 *
 *	time1  - pointer to the midpoint of the first time interval
 *	inacc1 - pointer to the inaccuracy of the first time interval
 *	time2  - pointer to the midpoint of the second time interval
 *	inacc2 - pointer to the inaccuracy of the second time interval
 *
 * Outputs:
 *
 *	none
 *
 * Return Value:
 *
 *	TRUE	= the intervals intersect
 *	FALSE	= the intervals don't intersect
 */

int IsIntervalIntersection (
    UTCValue	*time1,
    ExtInacc	*inacc1,
    UTCValue	*time2,
    ExtInacc	*inacc2
)
{
    UTCValue	minTime1, maxTime1, minTime2, maxTime2;

    /*
     * If either interval is infinite, then there is an intersection.
     */
    if (IsInfiniteInacc (inacc1) || IsInfiniteInacc (inacc2))
	return (TRUE);

    /*
     * Find the time interval for time1.
     */
    UTCsub (time1, inacc1, &minTime1);
    UTCadd (time1, inacc1, &maxTime1);

    /*
     * Find the time interval for time2.
     */
    UTCsub (time2, inacc2, &minTime2);
    UTCadd (time2, inacc2, &maxTime2);

    /*
     * Interval interesection if:
     *
     * Case 1:		|----------|			|-----------|
     *		     |-----|			 |------|
     *
     * Case 2:		|----------|			|-----------|
     *			        |-----|				    |-----|
     *
     * Case 3:		|----------|
     *			  |------|
     *
     * Case 4:		|----------|
     *		      |--------------|
     *
     * Case 5:		|----------|
     *		        |----------|
     *
     * There is not an intersection if:
     *
     * Case 1:		|----------| |----------|
     *			    T1		 T2
     *
     * Case 2:		|----------| |----------|
     *			    T2		 T1
     *
     * Determine if there is not an intersection:
     *		maxTime1 < minTime2 || maxTime2 < minTime1
     */
    if (IsUTClt (&maxTime1, &minTime2) || IsUTClt (&maxTime2, &minTime1))
	return (FALSE);
    else
	return (TRUE);
}

/*
 * IsInterval1InInterval2() -
 *
 * Description:
 *
 *	Returns TRUE if interval 1 is contained in interval 2.  Each
 *	interval is specified as a midpoint and an inaacuracy.  Either
 *	inaccuracy may be infinite.
 *
 * Inputs:
 *
 *	time1	- pointer to the midpoint of the first time interval
 *	inacc1	- pointer to the inaccuracy of the first time interval
 *	time2	- pointer to the midpoint of the second time interval
 *	inacc2	- pointer to the inaccuracy of the second time interval
 *
 * Outputs:
 *
 *	none
 *
 * Return Value:
 *
 *	TRUE	= the intervals intersect
 *	FALSE	= the intervals don't intersect
 */

int IsInterval1InInterval2 (
    UTCValue	*time1,
    ExtInacc	*inacc1,
    UTCValue	*time2,
    ExtInacc	*inacc2
)

{
    UTCValue	minTime1, maxTime1, minTime2, maxTime2;

    /*
     * If interval 2 is infinite, then interval 1 is in interval 2 regardless
     * of it's size.  If interval 2 isn't infinite and interval 1 is then
     * interval 1 isn't in interval 2.
     */
    if (IsInfiniteInacc (inacc2))
	return (TRUE);
    else if (IsInfiniteInacc (inacc1))
	return (FALSE);

    /*
     * Find the time interval for time1.
     */
    UTCsub (time1, inacc1, &minTime1);
    UTCadd (time1, inacc1, &maxTime1);

    /*
     * Find the time interval for time2.
     */
    UTCsub (time2, inacc2, &minTime2);
    UTCadd (time2, inacc2, &maxTime2);

    /*
     * Interval 1 in Interval 2 if:
     *
     *		minTime1 >= minTime2 && maxTime1 <= maxTime2
     */
    if ( ! IsUTClt (&minTime1, &minTime2) &&
	 ! IsUTCgt (&maxTime1, &maxTime2) )
    {
	    return (TRUE);
    }

    /*
     * Interval 1 isn't in Interval 2.
     */
    return (FALSE);
}


/*
 *	LEAP SECOND ROUTINES
 */

/*
 * IsGreaterThanOrEqualNextLS () -
 *
 * Description:
 *
 *	Determines whether the current time, T,  is > the time of the next
 *	leap second, nextLS, which is stored in shared memory.
 *
 *	Note: This routine assumes shared memory is locked for read.
 *
 * Inputs:
 *
 *	Tptr 	  - pointer to the current time
 *	Iptr 	  - pointer to the inaccuracy at the current time, which may
 *		    be infinite
 *	nextLSptr - pointer to the time of the next leap second
 *
 * Outputs:
 *
 *	none
 *
 * Return Value:
 *
 *	1 - if LS is expired
 *	0 - if LS is not expired
 *
 */

int IsGreaterThanOrEqualNextLS (
    UTCValue 	*Tptr,
    ExtInacc 	*Iptr,
    UTCValue	*nextLSptr
)
{
    UTCValue 	intervalHi;

    /*
     * If the inaccuracy is infinite, then the upper bound of the time interval
     * is greater than the time of the next leap second.
     */
    if (IsInfiniteInacc (Iptr))
	return (1);

    /*
     * Calculate the upper bounds on the current time interval.
     */
    UTCadd (Tptr, Iptr, &intervalHi);

    /*
     * The leap second is expired if:
     *     T+I > nextLS
     */
    if ( !IsUTClt (&intervalHi, nextLSptr))
	return (1);

    return (0);
}

/*
 * CalcNextLS () -
 *
 * Description:
 *
 *	Calculates the next leap second which will occur after the given
 *	time.  If the nextLS in shared memory still applies, it is returned.
 *
 *	Note:  This routine assumes shared memory is locked for read access.
 *
 * Inputs:
 *
 *	Tptr - pointer to the time
 *	Iptr - pointer to the inaccuracy
 *	nextLSptr - pointer to the next leap second
 *
 * Implicit Inputs:
 *
 *	lastLScalc - the time the last LS was calculated
 *
 * Outputs:
 *
 *	nextLS is the time of the next leap second which will occur after T+I
 *
 * Return Value:
 *
 *	none
 *
 */

void CalcNextLS (
    UTCValue 	*Tptr,
    ExtInacc 	*Iptr,
    UTCValue 	*nextLSptr
)
{
    UTCValue 	interval;
    UTCValue 	*LSptr;
    struct tm	tmtime;
    unsigned int lastDayOfMonth;
    utc_t       utcTime;
    UTCValue    T;			/* current time */

    LSptr = &shmPtr->baseTimes.nextLS;

    /*
     * Do not a new leap second if
     * 1. Ipt is infinite, or
     * 2. the relation: To <= Tptr+Iptr < NextLS
     *	  where To is the sum of Tptr+Iptr when the last
     *	  leap second was calculated, Tptr+Iptr are the current
     *    inputs, and NextLS is the next expected leap second.
     */
    if ( !IsInfiniteInacc( Iptr ) )
    {
	UTCadd( Tptr, Iptr, &T );
        /*
	 * Make sure there were no overflows.  If there were, set
	 * T to positive infinity.
	 */
	if (IsUTCneg( &T ))
	{
	    UTCassign (&T, 0xFFFFFFFFUL, 0x7FFFFFFFUL);
	}

	/*
	 * Tlastls <= T < NextLS
	 */
	if ( IsUTCle(&TlastLS, &T) && IsUTClt(&T,LSptr) )
	{
	    *nextLSptr = *LSptr;
	}
        else
	{    /*
	      * A leap second always occurs at the beginning of
	      * the last second in a month.
	      * Calculate the next ls.  This can
	      * occur during the current month or during the
	      * following month based upon the current time.
	      * If the current time > the last second of the current
	      * month (23:59:59.000) then the next leap second occurs
	      * the following month,
	      * else it occurs during the current month.
	      */

	     interval = *Iptr;

 	     UTCadd( &interval, Tptr, &interval);
	     (void) utc_mkcomptime(&utcTime, &interval, (UTCValue *)NULL, 0L);
	     if (utc_gmtime(&tmtime, (long *)NULL,
			    (struct tm *)NULL, (long *)NULL,
                            &utcTime))
	         BugCheck();

             lastDayOfMonth = ( tmtime.tm_mon == 1 &&
				IsLeapYear( tmtime.tm_year + 1900 )) ?
				    29:daysInMonth[ tmtime.tm_mon ];

	     if (( tmtime.tm_sec >= 59 ) &&
	        ( tmtime.tm_hour == 23 ) &&
                ( tmtime.tm_min  == 59 ) &&
	        ( tmtime.tm_mday == lastDayOfMonth ))
  	     {
	         tmtime.tm_mon++;
	         if ( tmtime.tm_mon > 11 )
	         {
		     tmtime.tm_mon = 0;
		     tmtime.tm_year++;
	         }
                 lastDayOfMonth = ( tmtime.tm_mon == 1 &&
				IsLeapYear( tmtime.tm_year + 1900 )) ?
				   29 : daysInMonth[ tmtime.tm_mon ];
	     }

             /*
              * Leap second to occur beginning of last second of month.
	      */
	     tmtime.tm_sec = 59;
             tmtime.tm_min = 59;
	     tmtime.tm_hour = 23;
             tmtime.tm_mday = lastDayOfMonth;
	     if (utc_mkgmtime(&utcTime, &tmtime, 0L, (struct tm *)NULL, 0L))
	         BugCheck();
	     (void) utc_comptime(nextLSptr, (UTCValue *)NULL, (long *)NULL,
                                 &utcTime);

#ifdef	DCE_DEBUG
	     if (DCE_SVC_DEBUG_ATLEAST(dts_svc_handle, dts_s_msgs, 1)
	      && !IsUTCeq (LSptr, nextLSptr)) {
#if LONG_BIT >= 64
		 DCE_SVC_DEBUG((dts_svc_handle, dts_s_msgs, svc_c_debug1,
		    "Calculated New Leap Second, Time: %u.\n",
		    *Tptr));
		 DCE_SVC_DEBUG((dts_svc_handle, dts_s_msgs, svc_c_debug1,
		    "\tOld LS : %u\t New LS : %u\n",
		    *LSptr,*nextLSptr));
#else
		 DCE_SVC_DEBUG((dts_svc_handle, dts_s_msgs, svc_c_debug1,
		    "Calculated New Leap Second, Time: %u %u.\n",
		    Tptr->hi,Tptr->lo));
		 DCE_SVC_DEBUG((dts_svc_handle, dts_s_msgs, svc_c_debug1,
		    "\tOld LS : %u %u\t New LS : %u %u\n",
		    LSptr->hi,LSptr->lo,nextLSptr->hi,nextLSptr->lo));
#endif	/* LONG_BIT >= 64 */

		 DCE_SVC_DEBUG((dts_svc_handle, dts_s_msgs, svc_c_debug1,
		    "\tNew LS is: %u-%u-%u %u:%u:%u.000\n",
		    tmtime.tm_mday, tmtime.tm_mon + 1,
		    tmtime.tm_year + 1900, tmtime.tm_hour,
		    tmtime.tm_min, tmtime.tm_sec));
	      }
#endif
	     /*
	      *  Save the current time
	      */
	     TlastLS = T;

         }
    }
    else
    {
	*nextLSptr = *LSptr;
    }
}

/***	END LEAP SECOND ROUTINES	***/

#ifdef	DCE_DEBUG
/*
 * PrintServerTimes() -
 *	Print the inputs and the outputs to the EstimateServerTime()
 *	routine.
 */
void PrintServerTimes
(
    UTCValue	*TsendPtr,	/* Tc(t1) - time request was sent to server */
    UTCValue	*TrespPtr,	/* Ts(t4) - time in response from server */
    ExtInacc	*IrespPtr,	/* Is(t4) - inaccuracy in resp from server */
    UTCValue	*procDelayPtr,	/* w - processing delay incurred by server */
    UTCValue	*TrecPtr,	/* Tc(t8) - time response received back */
    UTCValue	*TsyncPtr,	/* clerk clock time at sync instant */
    ExtInacc	*IsyncPtr,	/* clerk inaccuracy at sync instant */
    UTCValue	*TservPtr,	/* server clock time at sync instant */
    ExtInacc	*IservPtr	/* server inaccuracy at sync instant */
    )
{
    char	Tsendbuf[32];
    char	Trespbuf[32];
    char	Irespbuf[32];
    char	procDelaybuf[32];
    char	Trecbuf[32];
    char	Tsyncbuf[32];
    char	Isyncbuf[32];
    char	Tservbuf[32];
    char	Iservbuf[32];

    CvtUTCValueToStr(TsendPtr, Tsendbuf, sizeof Tsendbuf);
    CvtUTCValueToStr(TrespPtr, Trespbuf, sizeof Trespbuf);
    if (IsInfiniteInacc(IrespPtr))
	(void)strcpy(Irespbuf, "infinite");
    else
	CvtUTCValueToStr(IrespPtr, Irespbuf, sizeof Irespbuf);
    CvtUTCValueToStr(procDelayPtr, procDelaybuf, sizeof procDelaybuf);
    CvtUTCValueToStr(TrecPtr, Trecbuf, sizeof Trecbuf);
    CvtUTCValueToStr(TsyncPtr, Tsyncbuf, sizeof Tsyncbuf);
    if (IsInfiniteInacc(IsyncPtr))
	(void)strcpy(Isyncbuf, "infinite");
    else
	CvtUTCValueToStr(IsyncPtr, Isyncbuf, sizeof Isyncbuf);
    CvtUTCValueToStr(TservPtr, Tservbuf, sizeof Tservbuf);
    if (IsInfiniteInacc(IservPtr))
	(void)strcpy(Iservbuf, "infinite");
    else
	CvtUTCValueToStr(IservPtr, Iservbuf, sizeof Iservbuf);

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	"***Estimate Server Time***\n"));
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	 "Tresp: %s\tTsend: %s\n", Trespbuf, Tsendbuf));
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	"Iresp: %s\tprocDelay: %s\n", Irespbuf, procDelaybuf));
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	"Tsync: %s\tTrec:  %s\n", Tsyncbuf, Trecbuf));
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	"Isync: %s\n", Isyncbuf));
    DCE_SVC_DEBUG((dts_svc_handle, dts_s_arith, svc_c_debug1,
	"Tserv: %s\tIserv: %s\n", Tservbuf, Iservbuf));
}
#endif
