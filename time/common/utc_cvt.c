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
 * $Log: utc_cvt.c,v $
 * Revision 1.1.10.2  1996/02/17  23:34:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:45  marty]
 *
 * Revision 1.1.10.1  1995/12/08  18:08:31  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/02/08  13:04 UTC  pare
 * 	Merge changes.
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1995/02/08  13:03 UTC  pare
 * 	Fix problem parsing year value of 1899.
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/26  13:32 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:24:06  root]
 * 
 * Revision 1.1.8.3  1994/08/23  20:32:03  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:56:49  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:52:37  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  14:42 UTC  pare
 * 	Merge changes from Hpdce02
 * 
 * Revision 1.1.8.2  1994/08/08  21:30:09  cbrooks
 * 	CR11149 - utc_mkasctime accepts relative timestamp
 * 	[1994/08/08  19:38:33  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  20:36 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.8.1  1994/05/12  15:17:36  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:35  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:51  rhw]
 * 
 * Revision 1.1.7.5  1993/09/23  17:30:45  pare
 * 	 DCE1_0_3b03 Merge
 * 	[1993/09/23  17:30:13  pare]
 * 
 * Revision 1.1.8.3  1993/09/15  17:46:55  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.4  1993/09/13  14:58:36  truitt
 * 	Re-added obsolete functions utc_mkutcstr and utc_mkrelstr
 * 	so that some cds functional tests can still use these.  In
 * 	the near future, cds functional tests will no longer call
 * 	these obsolete functions, so they can be deleted once again.
 * 	In the mean time, they're baaaack.
 * 	[1993/09/13  14:58:09  truitt]
 * 
 * Revision 1.1.7.3  1993/07/15  13:21:40  truitt
 * 	Removed code for obsolete functions utc_mkutstr and utc_mkrelstr.
 * 	These both core dump when called.  The other two obsolete functions
 * 	still in the code do not core dump, so the code has been saved.
 * 	[1993/07/15  13:20:46  truitt]
 * 
 * Revision 1.1.7.2  1993/07/08  19:44:43  root
 * 	Initial King Kong branch
 * 	[1993/07/08  19:44:10  root]
 * 
 * 	This fixes OT5523 "Create link is successful with invalid Expiration Time"
 * 	by verifying that the individual fields (month, day, etc.) are all within
 * 	bounds.
 * 	[1992/12/30  21:20:24  yankes]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  22:11:00  htf]
 * 
 * 		This change fixes OTs 5494 (parsing routines not accepting some inacc
 * 	strings) and 5270 (carry propagation not done on inacc field).
 * 	[1992/11/03  18:09:34  yankes]
 * 
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:03:14  sekhar]
 * 
 * Revision 1.1.5.2  1993/04/05  17:47:52  truitt
 * 	Recreate GAMERA branch.
 * 	[1993/04/05  17:46:54  truitt]
 * 
 * Revision 1.1.5.2  1993/03/19  14:46:06  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/19  14:44:32  truitt]
 * 
 * Revision 1.1.1.8  1993/03/11  22:32:05  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.2  1992/12/21  23:22:08  smythe
 * 	added OSF OT 5270 fix to do proper carry propagation on inacc field
 * 	added OSF OT 5494 fix to properly accept all legal inacc strings
 * 	[1992/12/19  18:12:43  smythe]
 * 
 * Revision 1.1.2.3  1992/05/14  15:19:58  ko
 * 	Fix ot 2154 - use '--' to mean infinite inaccuracy on input, ----- on output.
 * 	[1992/05/14  15:18:15  ko]
 * 
 * Revision 1.1.2.2  1992/04/09  20:09:03  jim
 * 	Do not redefine string functions for AIX 3.2 compiler.
 * 	[1992/04/09  19:55:03  jim]
 * 
 * Revision 1.1  1992/01/19  15:32:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module UCT_CVT
 *	Version V1.0
 */
/*
 * Copyright (c) 1990, 1991 by
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
 * and  should  not be  construed  as  a commitment by Digital Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:    convert between utc_t and ascii
 *
 * ENVIRONMENT:	VAX/VMS V5.*, ULTRIX 4.*
 *
 * AUTHORS:
 *		Author	K.P. Baryiames
 *		Distributed Systems Services (DSS) 
 *              Based on DECnet/Ultrix NCL parser.
 * 
 * DATE:        July 30, 1990
 *
 */

/* LINTLIBRARY */

#include <stddef.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dce/dce.h>
#include <dtss_config.h>


#include <utc.h>

#include <dtssdef.h>
#include <utc_arith.h>
#include <commondef.h>

#define MAXISTR    80
#define MAXTSTR    80
#define MAXTDFSTR  80
#define SUCCESS    0
#define FAILURE   -1
#define BUFF_OVFL -1
#define IN
#define OUT
#define BOOLEAN short
#define MAXINT 256
#define PLUSMINUS '\261'

#define K_MS_PER_SEC  (K_100NS_PER_SEC/K_100NS_PER_MS)
#define K_MS_PER_MIN  (K_MS_PER_SEC*60)

typedef int INTEGER;

typedef enum {
    Integer32,
    Frac_ns,
    Frac_hour,
    Frac_min
    } CvtType;

/*
 * Local Static Routines
 */
static int do_strtorel( utc_t *pVal, char *cp, int parseForm);
static int FillPeriod( struct tm *tm_time, int period, char *cp, int type, struct tm *tm_record );
static int convert_to_integer( CvtType DType, char *cp, int len, long *pVal);
static int FillDefaults( struct tm *dstPtr, struct tm *defPtr, long *dsttns);

#define UTC_FORMLOCAL 0
#define UTC_FORMGMT   1
#define UTC_FORMREL   0
#define UTC_FORMSREL  1

#define P_DAYPART  0
#define P_YEARPART 1

/* #ifndef _AIX */
/* extern char * strchr(), *strpbrk(); */
/* #endif / * _AIX * / */

#define K_NS_PER_MS (1000000)

int utc_ascsmplreltime( char *, size_t , utc_t *) ;
int utc_mkascsmplreltime ( utc_t *, char *) ;

/*
 * Array used to convert to nanoseconds per second.
 * multipy the integer entered by the array element indexed
 * by size to generate the number of nanoseconds. 
 * ie. .001000, number = 1, len = 3(trailing zeros truncated).
 * nanoseconds = fraccvtarray[3] * number.
 * The maximum size number len is 9.
 */
#define K_MAX_FRAC_LEN 9

unsigned long FracCvtArray[K_MAX_FRAC_LEN + 1] = { 0,
						   100000000,  
						   10000000,
						   1000000,
						   100000,
						   10000,
						   1000,
						   100,
						   10,
						   1};
BOOLEAN fill = TRUE;

/*
 *++
 *  utc_ascgmtime()
 *
 *  Functional Description:
 *
 *  Convert from a utc_t to an ascii binary absolute time string.
 *  The time string is converted into gm text.
 *  
 *  Inputs:
 *   utc - address of a utc_t to convert
 *   strlen - the maximum string length.
 *   format - the format out the output text
 *
 *  Implicit Inputs:
 *   none.
 *
 *  Outputs:
 *   cp - address of a character string in which to place the output.
 *
 *  Implicit Outputs:
 *    none.
 *      
 *  Value Returned:
 *
 *    Success - 0
 *    Syntax Failure - -1
 *
 *  Side Effects:
 *
 *    none.
 *
 *--
 */
int utc_ascgmtime( cp, stringlen, utcTime )
char *cp;
size_t stringlen;
utc_t *utcTime;
{
    struct tm tmTime;
    struct tm tmInacc;
    long tns;
    long ins;
    char inaccstr[MAXISTR];
    char timestr[MAXTSTR];
    int temp_ns;
    int temp_sec;
    
    if (utc_gmtime(&tmTime,
		   &tns,
		   &tmInacc,
		   &ins,
		   utcTime))
	return FAILURE;

    (void) sprintf(timestr, 
		   "%04d-%02d-%02d-%02d:%02d:%02d.%03d",
		   tmTime.tm_year+1900, 
		   ++tmTime.tm_mon, 
		   tmTime.tm_mday, 
		   tmTime.tm_hour,
		   tmTime.tm_min,  
		   tmTime.tm_sec,
		   ((tns + K_NS_PER_MS/2)/K_NS_PER_MS));
    
    /*
     * format the current inaccuracy.
     */
    if (ins != -1) {
        temp_sec = tmInacc.tm_sec;
        temp_ns = (ins + K_NS_PER_MS - 1) / K_NS_PER_MS;
        if (temp_ns >= 1000)
        {
          temp_ns = temp_ns - 1000;
          temp_sec++;
        }
	(void) sprintf(inaccstr, "I%01ld.%03ld",
		       tmInacc.tm_yday * 24 * 60 * 60 +
		       tmInacc.tm_hour * 60 * 60 +
		       tmInacc.tm_min  * 60 +
		       temp_sec,
		       temp_ns);
    }
    else /* infinite inaccuracy */
	(void) sprintf(inaccstr,"%s","I-----");
    
    if ((strlen(inaccstr)+strlen(timestr)) > stringlen)
	return(BUFF_OVFL);
    
    (void) sprintf(cp,"%s%s", timestr, inaccstr);
    
    return SUCCESS;
}


/*
 *++
 *  utc_asclocaltime()
 *
 *  Functional Description:
 *
 *  Convert from a utc_t to an ascii binary absolute time string.
 *  The time string is converted into gm text.
 *  
 *  Inputs:
 *   utc - address of a utc_t to convert
 *   strlen - the maximum string length.
 *   format - the format out the output text
 *
 *  Implicit Inputs:
 *   none.
 *
 *  Outputs:
 *   cp - address of a character string in which to place the output.
 *
 *  Implicit Outputs:
 *    none.
 *      
 *  Value Returned:
 *
 *    Success - 0
 *    Syntax Failure - -1
 *
 *  Side Effects:
 *
 *    none.
 *
 *--
 */
int utc_asclocaltime( cp, stringlen, utcTime)
char *cp;
size_t stringlen;
utc_t *utcTime;
{
    utc_t utcTimeInt1, utcTimeInt2;
    long tdf;
    int status;

    if (NULL == utcTime)
    {
	if (utc_gettime(&utcTimeInt1))
	    return(FAILURE);
    }
    else
	utcTimeInt1 = *utcTime;

    if (utc_localzone ((char *)NULL, (size_t)0, &tdf, (int *)NULL,
                       &utcTimeInt1))
	return(FAILURE);

    if (utc_settdf(&utcTimeInt2, &utcTimeInt1, tdf))
	return(FAILURE);

    status = utc_ascanytime(cp, stringlen, &utcTimeInt2);
    return(status);
        
}

/*
 *++
 *  utc_ascanytime()
 *
 *  Functional Description:
 *
 *  Convert from a utc_t to an ascii binary absolute time string.
 *  The time string is local time in the timestamp's creator's timezone.
 *  
 *  Inputs:
 *   utc - address of a utc_t to convert
 *   strlen - the maximum string length.
 *   format - the format out the output text
 *
 *  Implicit Inputs:
 *   none.
 *
 *  Outputs:
 *   cp - address of a character string in which to place the output.
 *
 *  Implicit Outputs:
 *    none.
 *      
 *  Value Returned:
 *
 *    Success - 0
 *    Syntax Failure - -1
 *
 *  Side Effects:
 *
 *    none.
 *
 *--
 */
int utc_ascanytime( cp, stringlen, utcTime)
char *cp;
size_t stringlen;
utc_t *utcTime;
{
    struct tm tmTime;
    struct tm tmInacc;
    long tns;
    long ins;
    long tdf;
    char inaccstr[MAXISTR];
    char timestr[MAXTSTR];
    int temp_sec;
    int temp_ns;
    
    /*
     * print a local string
     */
    if (utc_anytime(&tmTime,
		    &tns,
		    &tmInacc,
		    &ins,
		    &tdf,
		    utcTime))
	
	return FAILURE;

    (void) sprintf(timestr, 
		   "%04d-%02d-%02d-%02d:%02d:%02d.%03d%s%02d:%02d",
		   tmTime.tm_year+1900, 
		   ++tmTime.tm_mon, 
		   tmTime.tm_mday, 
		   tmTime.tm_hour,
		   tmTime.tm_min,  
		   tmTime.tm_sec,
		   (tns + K_NS_PER_MS/2)/K_NS_PER_MS,
		   ((tdf >= 0) ? "+" : "-"), 

		   labs(tdf)/3600, 
		   (labs(tdf)%3600)/60);
    /*
     * format the current inaccuracy.
     */
    if (ins != -1) {
        temp_sec = tmInacc.tm_sec;
        temp_ns = (ins + K_NS_PER_MS - 1) / K_NS_PER_MS;
        if (temp_ns >= 1000)
        {
          temp_ns = temp_ns - 1000;
          temp_sec++;
        }
	(void) sprintf(inaccstr, "I%01ld.%03ld",
		       tmInacc.tm_yday * 24 * 60 * 60 +
		       tmInacc.tm_hour * 60 * 60 +
		       tmInacc.tm_min  * 60 +
                       temp_sec,
                       temp_ns);
    }
    else /* infinite inaccuracy */
	(void) sprintf(inaccstr,"%s","I-----");
    
    if ((strlen(inaccstr)+strlen(timestr)) > stringlen)
	return(FAILURE);
    
    (void) sprintf(cp,"%s%s", timestr, inaccstr);
    
    return SUCCESS;
}

/*
 *++
 *  utc_ascreltime()
 *
 *  Functional Description:
 *
 *
 *    Convert a utc_t into its ascii binary relative time
 *    string representation.
 *
 *  
 *  Inputs:
 *
 *    utcTime - address of the 128 bit utc to convert
 *    stringlen  - the size of the convert string buffer.
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *    cp  - address of the convert string buffer.
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *    success - 0
 *    syntax failure - -1
 *    string buffer overflow - -2
 *
 *  Side Effects:
 *    none.
 *
 *--
 */
utc_ascreltime( cp, stringlen, utcTime )
char *cp;
size_t stringlen; 
utc_t *utcTime;
{
    struct tm tmtime;
    struct tm tmInacc;
    long tns;
    long ins;
    char sign ;
    char inaccstr[MAXISTR];
    char timestr[MAXTSTR];
    int temp_sec; 
    int temp_ns;
    
    if (utc_reltime( &tmtime, &tns, &tmInacc, &ins, utcTime) != SUCCESS)
	return FAILURE;
    
    /*
     * See if this is a negative relative time and store away the
     * sign.
     */
    if ( (tmtime.tm_sec < 0) || (tmtime.tm_min < 0) ||
	(tmtime.tm_hour < 0) || (tmtime.tm_yday < 0) || (tns < 0) )
	sign = '-' ;
    else
	sign = '+' ;
    
    (void) sprintf( timestr, "%c%ld-%02ld:%02ld:%02ld.%03ld", 
		   sign,
		   abs(tmtime.tm_yday),
		   abs(tmtime.tm_hour), 
		   abs(tmtime.tm_min), 
		   abs(tmtime.tm_sec),

		   (labs(tns) + K_NS_PER_MS/2)/K_NS_PER_MS);

    inaccstr[0] = '\0';
    if ( ins != -1 ) 
    {
        temp_sec = tmInacc.tm_sec;
        temp_ns = (ins + K_NS_PER_MS - 1) / K_NS_PER_MS;
        if (temp_ns >= 1000)
        {
          temp_ns = temp_ns - 1000;
          temp_sec++;
        }
	(void) sprintf( inaccstr, "I%01ld.%03ld",
		       tmInacc.tm_yday * 24 * 60 * 60 +
		       tmInacc.tm_hour * 60 * 60 +
		       tmInacc.tm_min  * 60 +
                       temp_sec,
                       temp_ns);
    }
    
    else /* infinite inaccuracy */
	(void) sprintf( inaccstr, "I-----");

    if (strlen(inaccstr) + strlen(timestr) > stringlen)
        return(BUFF_OVFL);

    (void) sprintf(cp,"%s%s", timestr, inaccstr);
    return SUCCESS;
}




/*
 *++
 *  utc_ascsmplreltime()
 *
 *  Functional Description:
 *
 *
 *    Convert a utc_t into its ascii simple binary relative time
 *    string representation.
 *
 *  
 *  Inputs:
 *
 *    utcTime - address of the 128 bit utc to convert
 *    stringlen  - the size of the convert string buffer.
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *    cp  - address of the convert string buffer.
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *    success - 0
 *    syntax failure - -1
 *    string buffer overflow - -2
 *
 *  Side Effects:
 *    none.
 *
 *--
 */
int utc_ascsmplreltime( char *cp, size_t stringlen, utc_t *utcTime)

{
    struct tm tmtime;
    long tns;
    char sign;
    char timestr[MAXTSTR];
    

    if (utc_reltime (&tmtime, &tns, (struct tm *)NULL, (long *)NULL,
		     utcTime) != SUCCESS)
	return FAILURE;
    
    /*
     * See if this is a negative relative time and store away the
     * sign.
     */
    if ( (tmtime.tm_sec < 0) || (tmtime.tm_min < 0) ||
	(tmtime.tm_hour < 0) || (tmtime.tm_yday < 0) || (tns < 0) )
	sign = '-' ;
    else
	sign = '+' ;
    
    (void) sprintf( timestr, "%c%ld-%02ld:%02ld:%02ld.%03ld", 
		   sign,
		   abs(tmtime.tm_yday),
		   abs(tmtime.tm_hour), 
		   abs(tmtime.tm_min), 
		   abs(tmtime.tm_sec),

		   (labs(tns) + K_NS_PER_MS/2)/K_NS_PER_MS
		   );

    if (strlen(timestr) > stringlen)
        return(BUFF_OVFL);

    (void) strcpy (cp, timestr);
    return SUCCESS;
}




/*
 *++
 * int utc_mkasctime(pVal, cp) 
 *
 *  Functional Description:
 *
 *  Convert an ascii string into a 128 bit utc.
 *  Valid syntax is:
 *
 *  <TIME> ::== <Year-Part> |
 *              <Year-Part> <Partial-Day-Part> | 
 *              <Year-Part> <Partial-Day-Part> <Inacc-Part>  |
 *              <Year-Part> <Inacc-Part>             |
 *              <Partial-Day-Part> <Inacc-Part>              |
 *              <Partial-Day-Part>                           |
 *              <Inacc-Part> 
 *  
 *  <Year-Part> ::== CCYY T | CCYY - <Month-Part>
 *  <Month-Part>::== MM T   | MM - <Day-Part> | epsilon
 *  <Day-Part>  ::== DD T   | DD -  | epsilon
 *  
 *  <Partial-Day-Part> ::==  <Full-Day> | <Defaulted-Day>
 *  <Full-Day> ::==  hh <Partial-Hour> 
 *  <Partial-Hour> ::==  Z  | .ff <TDF> | : mm <Partial-Minute> | <TDF>
 *  <Partial-Minute>::== Z  | .ff <TDF> | : ss <Partial-Second> | <TDF>
 *  <Partial-Second>::== Z  | .ff <TDF> | <TDF>
 *  
 *  <Defaulted-Day> ::== -mm <Partial-Min> | --ss <Partial-Second> |
 *                       --- <Partial-Second>
 *            
 *  <Inacc-Part> ::== I <Inacc-Time-Part> | +/- <Inacc-Time-Part>
 *  <Inacc-Time-Part> ::== <I-seconds> | inf | epsilon
 *  <I-seconds>  ::== s{s} | s{s}.f{f}
 *  
 *  <TDF>   ::== + <TDF-time> | - <TDF-time> | epsilon 
 *  <TDF-time>   ::== hh | hh:mm
 *  
 *  Notes: 1. Quoted strings ("xxx" & 'xxx') are accepted.
 *         2. Spaces are accepted
 *         3. period (.) and comma (,) are interchangable
 *         4. The current time components are used when input fields are
 *            defaulted.
 *  
 *
 *
 *  
 *  Inputs:
 *    cp - the address of a null terminated ascii string which is to
 *         be converted.
 *
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *    pVal - the address of a utc buffer to return the time in.
 *
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *    Success - 0
 *    Failure - -1
 *
 *  Side Effects:
 *    None.
 *
 *--
 */
int utc_mkasctime(pVal, cp)
char *cp; 
utc_t *pVal; 
{
    struct tm tm_time;
    struct tm tm_inacc;
    struct tm tmTime_def;
    struct tm tmInacc_def;
    struct tm verify_tm;
    char *ptr, *p_search;
    int len;
    long ins = 0;
    long tns = -1;
    long ins_def,tns_def;
    long tdf;
    long parseInt;
    BOOLEAN negTDF    = FALSE;
    BOOLEAN infInacc  = TRUE;
    BOOLEAN localTime = FALSE;
    static char TOKeos= '\0';
    char stringCopy[MAXISTR];

    tdf = 0;

    if (NULL == cp)
	return(utc_gettime(pVal));

    if (strlen(cp) == 0)
	return FAILURE;
    
    if (strlen(cp) > (size_t) MAXISTR)
	return FAILURE;
    
    ptr = cp;
    p_search = stringCopy;
    
    /*
     * pack the string down
     */
    while (*ptr) {
	if (isspace(*ptr)) 
	    ptr++;
	else
	    *p_search++ = *ptr++;
    }
    *p_search = '\0';
    
    /*
     * accept a quoted string
     */
    /*
     * accept a quoted string
     */
    if (strlen(stringCopy) == 0)
	return FAILURE;
    
    if (stringCopy[strlen(stringCopy)-1] == '\'' ||
	stringCopy[strlen(stringCopy)-1] == '"')
	stringCopy[strlen(stringCopy)-1] = '\0';
    
    ptr = stringCopy;
    
    /*
     * accept a quoted string
     */
    if (*ptr == '\'' || *ptr == '"')  
	ptr++;
    
    if (strlen(ptr) == 0)
	return FAILURE;
    
    
    /*
     * Initialize the tm struct
     */
    tm_time.tm_year = -1;
    tm_time.tm_mon  = -1;
    tm_time.tm_mday = -1;
    tm_time.tm_hour = -1;
    tm_time.tm_min  = -1;
    tm_time.tm_sec  = -1;

    fill = TRUE;
    
    
    /*
     * Initialize the inaccuracy
     */
    (void)memset((pointer_t)&tm_inacc, 0, sizeof(struct tm));
    
    switch (*ptr) 
    {
    case '-':        /* default symbol, defaulting hour field */
	ptr++;
	goto parse_min;
	
    case 'I':        /* inaccuracy only */
    case PLUSMINUS:
	ptr++;
	goto parse_inacc;
	
    case '+':			/* no absolute time can start with this */
	return FAILURE ;

    default:
	break;
    }
    
    /*
     * get the integer token
     */
    if ((p_search = strpbrk(ptr,"\261ITZ-:.,")) == NULL)  {
	p_search = &TOKeos;                       /* single token */
	len = strlen(ptr);
    }
    else
	len = p_search - ptr;                    
    
    
    if ( len == 0 )                /* do not allow leading T,Z,: */
	return FAILURE;
    
    if (convert_to_integer(Integer32,ptr,len,&parseInt) !=SUCCESS)
	return(FAILURE);
    
    if (*p_search != '\0') 
	ptr = p_search + 1;
    else
	ptr = &TOKeos;
    
    /*
     * treat the Z as an epsilon transition, it must be the last
     * token of the time of day.
     */
    if (*p_search == 'Z') {
	p_search++;
	switch (*p_search) {
	    case 'I':
	    case PLUSMINUS:  
	    ptr = p_search + 1;
	    tm_time.tm_hour = parseInt;
	    goto parse_inacc;
	    
	    case '\0':
	    tm_time.tm_hour = parseInt;
	    goto convert;
	    
	    default:
	    return FAILURE;
	}
    }
    
    
    switch (*p_search) {
	/*
	 * Got the year
	 */
	case 'I':
	case PLUSMINUS:
	tm_time.tm_year = parseInt - 1900;    /* I */
	goto parse_inacc;
	case 'T':
	tm_time.tm_year = parseInt - 1900;    /* 1990T */
	
	if (*ptr == '\0')
	    goto convert;
	if (*ptr == 'I' || *ptr == PLUSMINUS) {
	    ptr++;
	    goto parse_inacc;
        }
	
	goto parse_hourmin;
	case '-': 
	tm_time.tm_year = parseInt - 1900;    /* 1990- */
	fill = FALSE;
	goto parse_month;
	
	case '\0':
	tm_time.tm_year = parseInt - 1900;    /* 1990* */
	goto convert;

	/*
	 *  Default the year,month,day. just got the hour.
	 */
	case ':':
	tm_time.tm_hour = parseInt;          /* 12: */
	goto parse_min;
	
	case '.':
	case ',':
	tm_time.tm_hour = parseInt;
	goto parse_frachour;
	
	default:
	return(FAILURE);
    }
    
    parse_month:
    /*
     * Get the month?
     */
    p_search = strpbrk(ptr,"\261ZIT-:.,");
    
    if (p_search == NULL) {
	len = strlen(ptr);
	p_search = &TOKeos;
    }
    else
	len = p_search - ptr;
    
    if (convert_to_integer(Integer32,ptr,len,&parseInt) !=SUCCESS)
	return(FAILURE);
    
    if (*p_search != '\0')
	ptr = p_search + 1;
    else
	ptr = &TOKeos;
    
    /*
     * treat the Z as an epsilon transition, it must be the last
     * token of the time.
     */
    if (*p_search == 'Z') {
	p_search++;
	switch (*p_search) {
	    case 'I':  
	    case PLUSMINUS:
	    tm_time.tm_hour = parseInt;
	    ptr = p_search + 1;
	    goto parse_inacc;
	    
	    case '\0':
	    tm_time.tm_hour = parseInt;
	    ptr = &TOKeos;
	    goto convert;
	    
	    default:
	    return FAILURE;
	}
	
    }
    
    
    
    switch( *p_search ) {
	/*
	 * got the month
	 */
	case 'T':
	tm_time.tm_mon = --parseInt;   /* 1990-12T */
	if (*ptr == '\0')
	    goto convert;
	if (*ptr == 'I' || *ptr == PLUSMINUS) {
	    ptr++;
	    goto parse_inacc;
        }
	
	goto parse_hourmin;
	
	case 'I':                      /* 1990-12I */
	case PLUSMINUS:
	tm_time.tm_mon = --parseInt;
	goto parse_inacc;
	
	case '-':                       /* 1990-12- */
	tm_time.tm_mon = --parseInt;
	goto parse_day;
	
	case '\0':                      /* 1990-12* */
	tm_time.tm_mon = --parseInt;
	goto convert;
	
	/*
	 * This token was the hour        1990-12:
	 */
	case ':':
	tm_time.tm_hour = parseInt;
	goto parse_min;
	
	case '.':
	case ',':
	tm_time.tm_hour = parseInt;
	goto parse_frachour;
	
	default:
	return(FAILURE);
	
    }        
    
    parse_day:
    /*
     * Get the day field?
     */
    p_search = strpbrk(ptr,"\261ZIT-:.,");
    
    if (p_search == NULL) {
	len = strlen(ptr);
	p_search = &TOKeos;
    }
    else
	len = p_search - ptr;
    
    if (convert_to_integer(Integer32,ptr,len,&parseInt) !=SUCCESS)
	return(FAILURE);
    
    if (*p_search != '\0')
	ptr = p_search + 1;
    else
	ptr = &TOKeos;
    
    /*
     * treat the Z as an epsilon transition, it must be the last
     * token of the time.
     */
    if (*p_search == 'Z') {
	p_search++;
	switch (*p_search) {
	    case 'I':  
	    case PLUSMINUS:
	    tm_time.tm_hour = parseInt;
	    ptr = p_search + 1;
	    goto parse_inacc;
	    
	    case '\0':
	    tm_time.tm_hour = parseInt;
	    ptr = &TOKeos;
	    goto convert;
	    
	    default:
	    return FAILURE;
	}
	
    }
    
    
    switch( *p_search ) {
	
	case 'T':
	tm_time.tm_mday = parseInt;
	if (*ptr == '\0')
	    goto convert;
	if (*ptr == 'I' || *ptr == PLUSMINUS) {
	    ptr++;
	    goto parse_inacc;
	}
	
	goto parse_hourmin;
	
	case '-':
	tm_time.tm_mday = parseInt;    
	goto parse_hourmin;
	
	case 'I':
	case PLUSMINUS:
	tm_time.tm_mday = parseInt;
	goto parse_inacc;
	
	
	case ':':
	tm_time.tm_hour = parseInt;
	goto parse_min;
	
	case '.':
	case ',':
	tm_time.tm_hour = parseInt;
	goto parse_frachour;
	
	case '\0':
	tm_time.tm_mday = parseInt;
	goto convert;
	
	default:
	return(FAILURE);
	
    }   
    
    parse_hourmin:
    /*
     * Get the hour field
     */
    if (*ptr == '-') {
	ptr++;
	goto parse_min;
    }
    
    if (*ptr == 'Z')
	return FAILURE;
    
    p_search = strpbrk(ptr,".,:I\261-+Z");
    if (p_search == NULL) {
	p_search = &TOKeos;
	len = strlen(ptr);
    }
    else
	len = p_search - ptr;
    
    
    if (convert_to_integer(Integer32,ptr,len,&parseInt) !=SUCCESS)
	return(FAILURE);
    
    if (*p_search != '\0') 
	ptr = p_search + 1;
    else
	ptr = &TOKeos;
    
    /*
     * treat the Z as an epsilon transition, it must be the last
     * token of the time.
     */
    if (*p_search == 'Z') {
	p_search++;
	switch (*p_search) {
	    case 'I':  
            case PLUSMINUS:
	    tm_time.tm_hour = parseInt;
	    ptr = p_search + 1;
	    goto parse_inacc;
	    
	    case '\0':
	    tm_time.tm_hour = parseInt;
	    ptr = &TOKeos;
	    goto convert;
	    
	    default:
	    return FAILURE;
	}
	
    }
    
    switch( *p_search ) {
	
	case 'I':
	case PLUSMINUS:
	tm_time.tm_hour = parseInt;
	goto parse_inacc;
	
	case '-':
	negTDF = TRUE;
	case '+':
	tm_time.tm_hour = parseInt;
	goto parse_tdf;
	
	case ':':
	tm_time.tm_hour = parseInt;
	goto parse_min;
	
	case '.':
	case ',':
	tm_time.tm_hour = parseInt;
	goto parse_frachour;
	
	case '\0':
	tm_time.tm_hour = parseInt;
	goto convert;
	
	default:
	return(FAILURE);
	
    }   
    
    parse_frachour:
    
    if (*ptr == '\0')
	return FAILURE;
    
    p_search = strpbrk(ptr,"I\261-+Z");
    
    if (p_search == NULL) {
	len = strlen(ptr);
	p_search = &TOKeos;
    }
    else
	len = p_search - ptr;
    
    
    if (convert_to_integer(Frac_hour,ptr,len,&parseInt) !=SUCCESS)
	return(FAILURE);
    
    tm_time.tm_min = parseInt / K_MS_PER_MIN;
    parseInt = parseInt % K_MS_PER_MIN;		
    tm_time.tm_sec = parseInt / K_MS_PER_SEC;
    parseInt = parseInt % K_MS_PER_SEC;
    tns = parseInt * K_100NS_PER_MS * 100;
    
    if (*p_search != '\0') 
	ptr = p_search + 1;
    else
	ptr = &TOKeos;
    
    if (*p_search == 'Z') {
	p_search++;
	switch (*p_search) {
	    case 'I':  
	    case PLUSMINUS:
	    ptr = p_search + 1;
	    goto parse_inacc;
	    
	    case '\0':
	    goto convert;
	    
	    default:
	    return FAILURE;
	}
    }	    
    
    
    
    switch( *p_search ) {
	
	case 'I':
	case PLUSMINUS:
	goto parse_inacc;
	
	case '-':
	negTDF = TRUE;
	case '+':
	goto parse_tdf;
	
	case '\0':
	goto convert;
	
	default:
	return(FAILURE);
	
    }   
    
    parse_min:
    /*
     * Get the minutes field
     */
    
    if (*ptr == '\0')
	return FAILURE;
    
    if (*ptr == '-') {
	if (tm_time.tm_hour != -1)
	    return FAILURE;
	else {
	    ptr++;
	    goto parse_seconds;
	}
    }
    
    p_search = strpbrk(ptr,":.,I\261-+Z");
    
    if (p_search == NULL) {
	len = strlen(ptr);
	p_search = &TOKeos;
    }
    else
	len = p_search - ptr;
    
    
    if (convert_to_integer(Integer32,ptr,len,&parseInt) !=SUCCESS)
	return(FAILURE);
    
    if (*p_search != '\0') 
	ptr = p_search + 1;
    else
	ptr = &TOKeos;
    
    
    if (*p_search == 'Z') {
	p_search++;
	switch (*p_search) {
	    case 'I':  
	    case PLUSMINUS:
	    ptr = p_search + 1;
	    tm_time.tm_min = parseInt;
	    goto parse_inacc;
	    
	    case '\0':
	    tm_time.tm_min = parseInt;
	    ptr = &TOKeos;
	    goto convert;
	    
	    default:
	    return FAILURE;
	}
    }
    
    
    tm_time.tm_min = parseInt;
    switch( *p_search ) {
	case '.':
	case ',':
	goto parse_fracmin;
	
	case 'I':
	case PLUSMINUS:
	goto parse_inacc;
	
	case '-':
	negTDF = TRUE;
	case '+':
	goto parse_tdf;
	
	case ':':
	goto parse_seconds;
	
	case '\0':
	goto convert;
	
	default:
	return(FAILURE);
	
    }   
    
    parse_fracmin:
    
    if (*ptr == '\0')
	return FAILURE;
    
    p_search = strpbrk(ptr,"I\261-+Z");
    
    if (p_search == NULL) {
	len = strlen(ptr);
	p_search = &TOKeos;
    }
    else
	len = p_search - ptr;
    
    
    if (convert_to_integer(Frac_min,ptr,len,&parseInt) !=SUCCESS)
	return(FAILURE);
    
    tm_time.tm_sec = parseInt / K_MS_PER_SEC;
    parseInt = parseInt % K_MS_PER_SEC;
    tns = parseInt * K_100NS_PER_MS * 100;
    
    if (*p_search != '\0') 
	ptr = p_search + 1;
    else
	ptr = &TOKeos;
    
    
    if (*p_search == 'Z') {
	p_search++;
	switch (*p_search) {
	    case 'I':  
	    case PLUSMINUS:
	    ptr = p_search + 1;
	    goto parse_inacc;
	    
	    case '\0':
	    ptr = &TOKeos;
	    goto convert;
	    
	    default:
	    return FAILURE;
	}
    }	    
    
    switch( *p_search ) {
	
	case 'I':
	case PLUSMINUS:
	goto parse_inacc;
	
	case '-':
	negTDF = TRUE;
	case '+':
	goto parse_tdf;
	
	case '\0':
	goto convert;
	
	default:
	return(FAILURE);
	
    }   
    
    parse_seconds:
    
    if (*ptr == '-') {
	if (tm_time.tm_min != -1)
	    return FAILURE;
	else {
	    ptr++;
	    if (*ptr == '\0') {
		return FAILURE;
	    }
	    /*
	     * if the seconds have been defaulted, there better
	     * be some fractional seconds!
	     */
	    if (*ptr != '.' && *ptr != ',')
		return FAILURE;
	    else 
	    {
		ptr++;
		goto parse_fracsec;
	    }
	}
    }
    
    p_search = strpbrk(ptr,".,I\261-+Z");
    
    if (p_search == NULL) {
	len = strlen(ptr);
	p_search = &TOKeos;
    }
    else
	len = p_search - ptr;
    
    if (convert_to_integer(Integer32,ptr,len,&parseInt) !=SUCCESS)
	return(FAILURE);
    
    if (*p_search != '\0') 
	ptr = p_search + 1;
    else
	ptr = &TOKeos;
    
    if (*p_search == 'Z') {
	p_search++;
	switch (*p_search) {
	    case 'I':  
	    case PLUSMINUS:
	    ptr = p_search + 1;
	    tm_time.tm_sec = parseInt;
	    goto parse_inacc;
	    
	    case '\0':
	    tm_time.tm_sec = parseInt;
	    goto convert;
	    
	    default:
	    return FAILURE;
	}
    }
    
    
    tm_time.tm_sec = parseInt;
    switch( *p_search ) {
	
	case 'I':
	case PLUSMINUS:
	goto parse_inacc;
	
	case '-':
	negTDF = TRUE;
	case '+':
	goto parse_tdf;
	
	case '.':
	case ',':
	goto parse_fracsec;
	
	case '\0':
	goto convert;
	
	default:
	return(FAILURE);
	
    }   
    
    parse_fracsec:
    
    if (*ptr == '\0')
	return FAILURE;
    
    p_search = strpbrk(ptr,"I\261-+Z");
    
    if (p_search == NULL) {
	len = strlen(ptr);
	p_search = &TOKeos;
    }
    else
	len = p_search - ptr;
    
    
    if (convert_to_integer(Frac_ns,ptr,len,&parseInt) !=SUCCESS)
	return(FAILURE);
    
    if (*p_search != '\0') 
	ptr = p_search + 1;
    else
	ptr = &TOKeos;
    
    if (*p_search == 'Z') {
	p_search++;
	switch (*p_search) {
	    case 'I':  
	    case PLUSMINUS:
	    ptr = p_search + 1;
	    tns = parseInt;
	    goto parse_inacc;
	    
	    case '\0':
	    tns = parseInt;
	    ptr = &TOKeos;
	    goto convert;
	    
	    default:
	    return FAILURE;
	}
    }
    
    
    tns = parseInt;
    
    switch( *p_search ) {
	
	case 'I':
	case PLUSMINUS:
	goto parse_inacc;
	
	case '-':
	negTDF = TRUE;
	case '+':
	goto parse_tdf;
	
	case '\0':
	goto convert;
	
	default:
	return(FAILURE);
	
    }   
    
    parse_tdf:
    
    if (*ptr == '\0')
	return FAILURE;
    /*
     * Get the TDF hours
     */
    p_search = strpbrk(ptr,"I\261:");
    
    if (p_search == NULL) {
	len = strlen(ptr);
    }
    else
	len = p_search - ptr;
    
    if (convert_to_integer(Integer32,ptr,len,
			   &parseInt) !=SUCCESS)
	return(FAILURE);
    
    if (p_search == NULL) {
	p_search = &TOKeos;
    }
    else
	ptr = p_search + 1;
    
    localTime = TRUE;
    
    switch( *p_search ) {
	
	case 'I':
	case PLUSMINUS:
	tdf = 3600 * parseInt * (negTDF ? -1 : 1);
	goto parse_inacc;
	
	case ':':
	tdf = 3600 * parseInt;
	break;
	
	case '\0':
	tdf = 3600 * parseInt * (negTDF ? -1 : 1);
	goto convert;
	
	default:
	return(FAILURE);
	
    }   
    
    /*
     * Get the TDF minutes
     */
    p_search = strpbrk(ptr,"I\261");
    
    if (p_search == NULL) {
	len = strlen(ptr);
    }
    else
	len = p_search - ptr;
    
    if (convert_to_integer(Integer32,ptr,len,
			   &parseInt) !=SUCCESS)
	return(FAILURE);
    
    if (p_search == NULL) {
	p_search = &TOKeos;
    }
    else
	ptr = p_search + 1;
    
    tdf += (60 * parseInt);
    tdf *= (negTDF ? -1 : 1);
    switch( *p_search ) {
	
	case 'I':
	case PLUSMINUS:
	goto parse_inacc;
	
	case '\0':
	goto convert;
	
	default:
	return(FAILURE);
	
    }   
    
    parse_inacc:
    /*
     * Get Inaccuracy
     */
    if (*ptr == '\0')
	goto convert;
    
    p_search = strpbrk (ptr, ",.");
    if (p_search == NULL)
    {
	p_search = &TOKeos;
	len = strlen(ptr);   /* just seconds, no fracs */
    }
    else
    {
	len = p_search - ptr;
    }
    
    /*
     * if no seconds digits, check for fractions
     */
    if ( *ptr == '.' || *ptr == ',') {
	ptr++;
	goto parse_fracInacc;
    }
    
    if (strncmp(ptr, "INF", (size_t) len) == 0 ||
        strncmp(ptr, "inf", (size_t) len) == 0 ||
	strncmp(ptr, "-----" , (size_t) len) == 0) 
	goto convert;
    
    if (convert_to_integer (Integer32, ptr, len, 
			    (long *)&tm_inacc.tm_sec) != SUCCESS)
	return FAILURE;
    
    infInacc = FALSE;
    
    
    if (*p_search == '\0')
	goto convert;
    else
	ptr = p_search + 1;
    
    parse_fracInacc:
    /*
     * Get fractional innacuracy
     */
    
    if (*ptr == '\0')
	return FAILURE;;
    
    len = strlen(ptr);
    /*
     * Convert the fractional second
     */
    if (convert_to_integer(Frac_ns, ptr, len,&ins) != SUCCESS)
	return FAILURE;
    
    infInacc = FALSE;
    
    convert:
    
    if (infInacc == TRUE ) {
	tm_inacc.tm_sec = tm_inacc.tm_min = tm_inacc.tm_hour = -1;
	tm_inacc.tm_mon = tm_inacc.tm_mday = tm_inacc.tm_year = -1;
    }
    if (localTime == FALSE) {
	if(utc_gmtime(&tmTime_def,
		      &tns_def,
		      &tmInacc_def,
		      &ins_def,
		      (utc_t *)NULL) != SUCCESS)
	    return FAILURE;
	
	FillDefaults(&tm_time,&tmTime_def,&tns);
	
	if(utc_mkgmtime(
			(utc_t *)pVal,   /* OUT - return BinAbsTime here */
			&tm_time, 	 /* IN - time component of BAT */
			tns, 	         /* IN - fractional seconds of time */
			&tm_inacc, 	 /* IN - inaccuracy component of BAT */
			ins  	         /* IN - fract. seconds of inacc. */
			) != SUCCESS)
	    return FAILURE;
	
    }
    else {
	if(utc_localtime(&tmTime_def,
			 &tns_def,
			 &tmInacc_def,
			 &ins_def,
			 (utc_t *)NULL) != SUCCESS)
	    return FAILURE;
	
	FillDefaults(&tm_time,&tmTime_def,&tns);
	
	if (utc_mkanytime((utc_t *)pVal,
			  &tm_time, 	 /* IN - time component of BAT */
			  tns,           /* IN - fractional seconds of time */
			  &tm_inacc, 	 /* IN - inaccuracy component of BAT */
			  ins,           /* IN - fract. seconds of inacc. */
			  tdf            /* IN - tdf in seconds */
			  ) != SUCCESS )
	    return FAILURE;
    }    

/*  Now convert it back into a tm_struct and compare this structure with
    what was in tm_time.  If they are the same, then it was a valid time.
    If they are not the same, it meant that the user entered something
    that overflowed into the next field (for example, January 40th being
    turned into February 9th) and an error should be flagged. */

    if (utc_anytime(&verify_tm,     /* OUT - tm struct to verify against */
                      NULL,           /* OUT - nanoseconds, don't need */
                      NULL,           /* OUT - inaccuracy, don't need */
                      NULL,           /* OUT - ns inacc, don't need */
                      NULL,           /* OUT - TDF in seconds, don't need */
                      (utc_t *)pVal)  /* In - binary time to cvt back */
                       != SUCCESS)
            return FAILURE;

    if ((verify_tm.tm_year != tm_time.tm_year)  ||
        (verify_tm.tm_mon  != tm_time.tm_mon)   ||
        (verify_tm.tm_mday != tm_time.tm_mday)  ||
        (verify_tm.tm_hour != tm_time.tm_hour)  ||
        (verify_tm.tm_min  != tm_time.tm_min)   ||
        (verify_tm.tm_sec  != tm_time.tm_sec))
            return FAILURE;

    return SUCCESS;
}

/*
 *++
 *  FillDefaults()
 *
 *  Functional Description:
 *
 * fill in the time tm from a string parse with the
 * default values.
 * Rules for defaulting:
 *     1. year,month,day are always defaulted the current values
 *     2. defaults on the rhs of a value are zeroed:
 *           hh -> hh:00:00.00
 *           hh:mm -> hh:mm:00.00
 *           hh:mm:ss -> hh:mm:ss.00
 *     3. defaults on the lhs of a value are the current values,
 *        at the same time, rhs defaults are zeroed:
 *           -mm:ss.ff -> hh:mm:ss.ff
 *           --:ss.ff  -> hh:mm:ss.ff
 *           ---.ff    -> hh:mm:ss.ff
 *           -mm       -> hh:mm:00.00
 *           etc.
 *     4. the tns value is always defaulted to zero:
 *           hh:mm:ss  -> hh:mm:ss.00
 *
 *
 *
 *  
 *  Inputs:
 *    defPtr - default time values tm struct
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *    dstPtr - defaulted time tm struct
 *    dsttns - defaulted tns value.
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *--
 */
static int FillDefaults( struct tm *dstPtr, struct tm *defPtr, long *dsttns)

{                                                
    if (dstPtr->tm_year == (-1) && fill )               
	dstPtr->tm_year = defPtr->tm_year;   
    if (dstPtr->tm_mon == (-1))                
	dstPtr->tm_mon = defPtr->tm_mon;     
    if (dstPtr->tm_mday == (-1))               
	dstPtr->tm_mday = defPtr->tm_mday;   
    
    if (dstPtr->tm_hour == (-1))               
    {                                            
	/*
	 * Case ccyy...T eos
	 */
	if( *dsttns == -1 &&
	   dstPtr->tm_sec == -1 &&
	   dstPtr->tm_min == -1) 
	{
	    dstPtr->tm_sec = 0;
	    dstPtr->tm_min = 0;
	    dstPtr->tm_hour = 0;
	    *dsttns = 0;
	}
	
	else  /* hour defaulted, some rhs value not defaulted */   
	{
	    dstPtr->tm_hour = defPtr->tm_hour;
	    if (dstPtr->tm_min != -1)
	    {
		if (dstPtr->tm_sec == -1) 
		{
		    dstPtr->tm_sec = 0;
		    *dsttns = 0;
		}
		else if (*dsttns == -1) 
		{
		    *dsttns = 0;
		}
	    }
	    else
	    {
		dstPtr->tm_min = defPtr->tm_min;
		if (dstPtr->tm_sec != -1) 
		{
		    if (*dsttns == -1)
		        *dsttns = 0;
		}
		else 
		{
		    dstPtr->tm_sec = defPtr->tm_sec;
		}
	    }
	}
    }
    else    /* hour is not defaulted */          
    {                                            
        if (dstPtr->tm_min == (-1))            
	{                                        
	    dstPtr->tm_min = 0;                
	    dstPtr->tm_sec = 0;                
	}                                        
        else if (dstPtr->tm_sec == (-1))       
	{                                        
            dstPtr->tm_sec = 0;                
	    *dsttns = 0;                          
	}
        else if (*dsttns == -1)
        {
            *dsttns = 0;
	}                                        
    }                                            
}


/*
 *++
 * int utc_mkascreltime(pVal, cp) 
 *
 *  Functional Description:
 *
 *  Convert an ascii binary relative time into its 128 bit representation.
 *
 *
 *  
 *  Inputs:
 *
 *    pVal - address of a 128 utc time
 *    cp   - address of a null terminated string to convert.
 *    
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *    pVal - the converted 128 bit utc.
 *
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *    success - 0
 *    failure - -1
 *
 *  Side Effects:
 *    none.
 *
 *--
 */
int utc_mkascreltime(pVal,cp)
char *cp; 
utc_t *pVal;
{
    return(do_strtorel(pVal, cp, UTC_FORMREL));
}


/*
 *++
 *  utc_mkascsmplreltime()
 *
 *  Functional Description:
 *
 *  Convert an ascii binary simple relative time into its 128 bit 
 *  representation.
 *
 *
 *  
 *  Inputs:
 *
 *    pVal - address of a 128 utc time
 *    cp   - address of a null terminated string to convert.
 *    
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *    pVal - the converted 128 bit utc.
 *
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *    success - 0
 *    failure - -1
 *
 *  Side Effects:
 *    none.
 *
 *--
 */
int utc_mkascsmplreltime ( utc_t *utc, char *string)

{
    return(do_strtorel (utc, string, UTC_FORMSREL));
}


/*
 *++
 *  do_strtorel(pVal, cp) 
 *
 *  Functional Description:
 *
 *  Convert an ascii binary relative time or a 
 *  ascii simple relative time into its 128 bit representation.
 *
 *
 *  
 *  Inputs:
 *
 *    pVal - address of a 128 utc time
 *    cp   - address of a null terminated string to convert.
 *    parseForm - the type of reltime -
 *      0 - binary relatvie time
 *      1 - binary simple relative time
 *    
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *    pVal - the converted 128 bit utc.
 *
 * 
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *    success - 0
 *    failure - -1
 *
 *  Side Effects:
 *    none.
 *
 *--
 */
static int do_strtorel( utc_t *pVal, char *cp, int parseForm)

{
    struct tm tm_time;
    struct tm tm_inacc;
    struct tm tm_record;
    char *ptr, *ptr2, *p_search;
    int len;
    long ifrac_time, ifrac_inacc;
    int ifield;
    int field_count;
    int period_int;
    BOOLEAN negative, more_tokens;
    BOOLEAN day_seen, hms_seen;
    BOOLEAN infinite_inacc;
    char stringCopy[MAXISTR];
    
    if (NULL == cp)
	return FAILURE;

    if (strlen(cp) == 0)
	return FAILURE;
    
    if (strlen(cp) > (size_t) MAXISTR)
	return FAILURE;
    
    ptr = cp;
    p_search = stringCopy;
    
    /*
     * pack the string down
     */
    while (*ptr) {
	if (isspace(*ptr)) 
	    ptr++;
	else
	    *p_search++ = *ptr++;
    }
    *p_search = '\0';
    
    /*
     * accept a quoted string
     */
    if (strlen(stringCopy) == 0)
	return FAILURE;
    
    if (stringCopy[strlen(stringCopy)-1] == '\'' ||
	stringCopy[strlen(stringCopy)-1] == '"')
	stringCopy[strlen(stringCopy)-1] = '\0';
    
    
    ptr = stringCopy;
    
    /*
     * accept a quoted string
     */
    if (*ptr == '\'' || *ptr == '"')
	ptr++;
    
    
    negative = day_seen = hms_seen = FALSE;
    memset((pointer_t)&tm_inacc, 0,sizeof (struct tm));
    memset((pointer_t)&tm_time, 0,sizeof (struct tm));
    ifrac_time = ifrac_inacc = 0;
    field_count = 0;
    infinite_inacc = TRUE;


    /*
     * Is this an ISO Period Time?
     */
    if (*ptr == 'P')
    {
	ptr++;
	goto parse_iso_period;
    }
    
    if (strlen(ptr) == 0)
	return FAILURE;
    
    /*
     * Check for leading - (for negative relative time)
     */
    if (*ptr == '-')
    {
	negative = TRUE;
	ptr++;
    };
    if (*ptr == '+') {
	ptr++;
    }
    
    /*
     * Look for days-hours:min:seconds
     */
    more_tokens = TRUE;
    while (more_tokens) {
	p_search = strpbrk (ptr, "-:,.I\261");
	if (p_search == NULL) {
	    more_tokens = FALSE; /* last token */
	    len = strlen (ptr);
	}
	else len = p_search - ptr;
	field_count++;
	if (field_count == 1 )
	{
	    if (p_search && *p_search == '-')  
		day_seen = TRUE;
	}
	else if (p_search && *p_search == '-')	/* if hyphen seen elsewhere */
	    return FAILURE;
	
	if (convert_to_integer(Integer32, ptr, len,
			       (long *)&ifield) != SUCCESS )
	    return FAILURE;
	if (more_tokens == TRUE)
	    ptr = p_search +1;	/* advance pointer past punctuation mark */
	
	if (negative)
	    ifield = 0 - ifield;
	/* 
	 * the following algorithm is a little unusual, because we don't know
	 * whether we're seeing an hour, minute or second count until we're
	 * done parsing, but we do know the order that they will appear in.
	 */
	tm_time.tm_yday = tm_time.tm_hour;
	tm_time.tm_hour = tm_time.tm_min;
	tm_time.tm_min = tm_time.tm_sec;
	tm_time.tm_sec = ifield;
	hms_seen = TRUE; /* note that we found hour, or min. or sec. */
	if (more_tokens == FALSE || *p_search == '.' || *p_search == ',' 
	    || *p_search == 'I'  || *p_search == PLUSMINUS)
	    break;	/* done parsing whole numbers */
	ptr = p_search +1;	/* advance pointer past punctuation mark */
    }
    
    /* if days specified, then hours and minutes must also be specified */
    if (day_seen && field_count < 3)	
	return FAILURE;
    
    if (p_search == NULL) { /* No more fields */
	if (hms_seen) goto convert; /* at least we found hour, min., or secs */
	else return FAILURE;
    }
    
    /*
     * Get fractional seconds
     */
    if (*p_search == '.' || *p_search == ',')	/* if a fractional number of seconds */
    {
	p_search = strpbrk (ptr, "I\261");
	if (p_search == NULL)
	    len = strlen(ptr);
	else
	    len = p_search - ptr;
    }
    else
	len = 0;
    /*
     * Take trailing 0s off ASCII number
     */
    ptr2 = ptr + len - 1;
    while ((*ptr2 == '0') && (ptr2 != ptr))
    {
	len = len - 1;
	ptr2 = ptr2 - 1;
    }
    if (len <= 0) {
	len = 0;
	ifrac_time = 0;
    }
    else
    {
	if (len > K_MAX_FRAC_LEN) len = K_MAX_FRAC_LEN;
	if (convert_to_integer (Integer32, ptr, len, 
				&ifrac_time) != SUCCESS)
	    return FAILURE;
    }
    /*
     * Convert to nanoseconds per second
     */
    ifrac_time = ifrac_time * FracCvtArray[len];
    
    if (negative)
	ifrac_time = 0 - ifrac_time;
    ptr = p_search +1;
    if (p_search == NULL)
	goto convert;


    /*
     * Get the inaccuracy portion
     */
    goto get_inacc;    


    /*
     * Parsing an ISO period
     */
    parse_iso_period:
    
    (void)memset((pointer_t)&tm_record, 0, sizeof(struct tm));

    if (*ptr == '\0')
	return FAILURE;
	    
    if (*ptr == 'T') 
    {
	ptr++;
	goto parse_iso_daypart;
    }
    
    for(;;)
    {
	p_search = strpbrk(ptr,"TYWDMI");
	if (p_search == NULL) 
	{
	    if (strlen(ptr) == 0)
		goto convert;
	    else
		return FAILURE;
	}

	len = p_search - ptr;
	
	
	switch (*p_search)
	{
	    case 'T':
	    ptr++;
	    goto parse_iso_daypart;
	    
	    case 'I':
	    ptr++;
	    goto get_inacc;
	    
	    default:
	    if (convert_to_integer(Integer32,ptr,len,
				   (long *)&period_int)!= SUCCESS)
		return(FAILURE);
	    if (FillPeriod(&tm_time, period_int, 
			   p_search, P_YEARPART, &tm_record) != SUCCESS)
		return(FAILURE);
	    break;

	}
	ptr = p_search + 1;
    }    

parse_iso_daypart:
    
    if (*ptr == '\0') 
	goto convert;
    
    for(;;)
    {
	p_search = strpbrk(ptr,"HMSI");
	if (p_search == NULL) 
	{
	    if (strlen(ptr) == 0)
		goto convert;
	    else
		return FAILURE;
	}

	len = p_search - ptr;
	
	switch (*p_search)
	{
	    case 'I':
	    ptr++;
	    goto get_inacc;
	    
	    default:
	    if (convert_to_integer(Integer32,ptr,len,
				   (long *)&period_int)!= SUCCESS)
		return(FAILURE);
	    if (FillPeriod(&tm_time, period_int, 
			   p_search, P_DAYPART, &tm_record) != SUCCESS)
		return(FAILURE);
	    
	    break;
	}

	ptr = p_search + 1;
    }    


get_inacc:
    /*
     * Get Inaccuracy
     */
    
    /*
     * if we got here on an 'I' and this is a simple rel time,
     * signal an error.
     */
    if ((*p_search == 'I' || *p_search == PLUSMINUS) && parseForm == UTC_FORMSREL)
	return FAILURE;
    
    p_search = strpbrk(ptr, ".,");
    if (p_search == NULL)
    {
	len = strlen (ptr);
	if (len == 0)
	    goto convert;
    }
    else
    {
	len = p_search - ptr;
    }
    
    /*
     * if no seconds digits, check for fractions
     */
    if ( *ptr == '.' || *ptr == ',') 
	goto fracInacc;
    
    if (strncmp(ptr, "INF", (size_t) len) == 0 ||
        strncmp(ptr, "inf", (size_t) len) == 0 ||
	strncmp(ptr, "-----" , (size_t) len) == 0) 
	goto convert;
    
    if (convert_to_integer (Integer32, ptr, len, 
			    (long *)&tm_inacc.tm_sec) != SUCCESS)
	return FAILURE;
    if (len != 0)
	infinite_inacc = FALSE;
    if (p_search == NULL)
	goto convert;
    
    fracInacc:
    /*
     * Get fractional innacuracy
     */
    ptr = p_search +1;
    len = strlen (ptr);
    if (len == 0)
	goto convert;
    /*
     * Take trailing 0s off ASCII number
     */
    ptr2 = ptr + len - 1;
    while ((*ptr2 == '0') && (ptr2 != ptr))
    {
	len = len - 1;
	ptr2 = ptr2 - 1;
    }
    if (len <= 0) {
	ifrac_inacc = 0;
	len = 0;
    }
    else
    {
	if (len > K_MAX_FRAC_LEN) len = K_MAX_FRAC_LEN;
	if (convert_to_integer (Integer32, ptr, len, 
				&ifrac_inacc) != SUCCESS)
	    return FAILURE;
	infinite_inacc = FALSE;
    }
    /*
     * Convert to nanoseconds per second
     */
    ifrac_inacc = ifrac_inacc * FracCvtArray[len];
    
    
    /*
     * Convert to Binary Relative Time
     */
    
    convert:
    
    if (infinite_inacc == TRUE ) {
	tm_inacc.tm_sec = tm_inacc.tm_min = tm_inacc.tm_hour = -1;
	tm_inacc.tm_mon = tm_inacc.tm_mday = tm_inacc.tm_year = -1;
    }
    if (utc_mkreltime((utc_t *)pVal, 
		      &tm_time, ifrac_time, &tm_inacc, ifrac_inacc) != 0)
	return FAILURE;
    
    return SUCCESS;
}

static int FillPeriod( struct tm *tm_time, int period, char *cp, int type, struct tm *tm_record )

{
    switch (*cp)
    {
	case 'Y':
	if (type == P_DAYPART)
	    return(FAILURE);

	if (tm_record->tm_year) 
	    return(FAILURE);

	tm_record->tm_year = 1;
	tm_time->tm_yday = period * 365;

	break;

	case 'M':
	if (type == P_YEARPART)
	{
	    if (tm_record->tm_mon)
		return(FAILURE);

	    if (period > 12)
		return(FAILURE);

	    tm_time->tm_yday += (30 * period);
	    tm_record->tm_mon = 1;
	}
	else
	{
	    if (tm_record->tm_min)
		return(FAILURE);

	    if (period > 60)
		return(FAILURE);

	    tm_record->tm_min = 1;
	    tm_time->tm_min = period;
	}

	break;

	case 'D':
	if (type == P_DAYPART)
	{
	    return (FAILURE);
	}
	if (tm_record->tm_mday)
	    return(FAILURE);

	if (period > 30)
	    return(FAILURE);

	tm_time->tm_yday += period;
	tm_record->tm_mday = 1;
	
	break;

	case 'W':
	if (type == P_DAYPART)
	    return(FAILURE);

	if (tm_record->tm_wday)
	    return(FAILURE);

	if (period > 51)
	    return(FAILURE);

	tm_time->tm_yday += period*7;
	tm_record->tm_wday = 1;

	break;


	case 'H':
	if (type != P_DAYPART)
	    return(FAILURE);

	if (tm_record->tm_hour)
	    return (FAILURE);

	if (period > 24)
	    return (FAILURE);

	tm_time->tm_hour = period;
	tm_record->tm_hour = 1;
	break;
	
	case 'S':
	if (type != P_DAYPART)
	    return(FAILURE);

	if (tm_record->tm_sec)
	    return(FAILURE);

	if (period > 60)
	    return(FAILURE);

	tm_time->tm_sec = period;
	tm_record->tm_sec = 1;
	break;

	default:
	return FAILURE;
    }

    return SUCCESS;

}
/*
 *                        CONVERT TO INTEGER
 *   Handles also Byte Integer, Word Integer, Long Integer
 *
 * Inputs:
 *     DType = Data type of data represented by ASCII string.
 *     pASCVal = Pointer to ASCII string.
 *     ASCLen = The length of the ASCII string.
 * 
 * Outputs:
 *     pVal = pointer to converted string
 *     pLen = length of the converted string.
 *
 * Returns:
 *     SUCCESS 
 *     FAILURE
 */
static int convert_to_integer( CvtType DType, char *cp, int len, long *pVal)

{
    char istr[MAXINT];
    int  i = 1;
    char *p;
    char *p2;
    long ifrac;
    int  flen;
    double dfrac;
    
    (void)memset( (pointer_t)istr, 0,MAXINT);
    (void)strncpy(istr, cp, (size_t) len);
    p = istr;
    
    /*
     * Check for leading +/-
     */
    if (!(isdigit(*p) || *p == '+' || *p == '-'))
    {
        return FAILURE;
    }
    /*
     * Check the rest of the string
     */
    while (i < len)
    {
	p++;
	i++;
	if (!(isdigit(*p)))
	{  
            return FAILURE;
	}
	if (i == len)
	{
            i++;
            istr[i] = '\0';
	}
    }     
    switch (DType) {
	
	case Frac_ns:
	case Frac_hour:
	case Frac_min:
	
	flen = strlen(istr);
	p2 = istr + len - 1;
	while ((*p2 == '0') && (p2 != istr))
	{
	    flen--;
	    p2--;
	}
	if (flen <= 0) {
	    *pVal = 0;
	    return(SUCCESS);
	}
	if (convert_to_integer (Integer32, istr, flen, &ifrac) != SUCCESS)
	    return FAILURE;
	
	if (DType == Frac_ns) {
	    /*
	     * Convert to nanoseconds per second w/o roundoff
	     */
	    *pVal = ifrac * FracCvtArray[flen];
	    return(SUCCESS);
	}
	else {
	    dfrac = ifrac;	/* convert to float for decimal scaling */
	    /*
	     * Convert number into fractional number
	     */
	    while (flen > 0)
	    {
	        dfrac = dfrac/10;
	        flen--;
	    }
	
	    switch ( DType ) {	    
	        case Frac_hour:
	        ifrac = dfrac * K_MS_PER_MIN * 60;
	        *pVal = ifrac;
	        break;
	    
	        case Frac_min:
	        ifrac = dfrac * K_MS_PER_MIN;
	        *pVal = ifrac;
	        break;
	    
	        default: return FAILURE;
	        }

	    return SUCCESS;

	} /* else */
	
	case Integer32:   
	if (sscanf(istr,"%ld",&i))
	{
	    *pVal = i;
	    return SUCCESS;
	}
	return FAILURE;
	
	default: return FAILURE;
    }
}
#if 0

/*
 *    O B S O L E T E - 
 */
/*
 *++
 *  utc_mkutcstr()
 *
 *  O B S O L E T E
 *
 *--
 */
utc_mkutcstr ( cp, stringlen, utcTime, format )
char *cp;
long *stringlen;
utc_t *utcTime;
int format;
{
    /*
     * print a local string
     */
    if (format == UTC_FORMLOCAL) 
    {
	if (utc_asclocaltime(cp, (size_t)*stringlen, utcTime))
	{
	    *stringlen = -1;
	    return(-1);
	}
	else
	{
	    *stringlen = strlen(cp);
	    return(0);
	}
	
    }
    else if (format == UTC_FORMGMT)
    {
	if (utc_ascgmtime(cp, (size_t)*stringlen, utcTime))
	{
	    *stringlen = -1;
	    return(-1);
	}
	else
	{
	    *stringlen = strlen(cp);
	    return(0);
	}
    }
    else
	return FAILURE;
    
}
utc_mkrelstr( cp, stringlen, utcTime, printForm )
char *cp;
int  *stringlen; 
utc_t *utcTime;
int printForm;
{
    struct tm tmtime;
    struct tm tmInacc;
    long tns;
    long ins;
    char sign ;
    char inaccstr[MAXISTR];
    char timestr[MAXTSTR];
    int length;
    int temp_sec;
    int temp_ns;
    
    if (utc_reltime( &tmtime, &tns, &tmInacc, &ins, utcTime) != SUCCESS)
	return FAILURE;
    
    /*
     * See if this is a negative relative time and store away the
     * sign.
     */
    if ( (tmtime.tm_sec < 0) || (tmtime.tm_min < 0) ||
	(tmtime.tm_hour < 0) || (tmtime.tm_yday < 0) || (tns < 0) )
	sign = '-' ;
    else
	sign = '+' ;
    
    (void) sprintf( timestr, "%c%ld-%02ld:%02ld:%02ld.%03ld", 
		   sign,
		   abs(tmtime.tm_yday),
		   abs(tmtime.tm_hour), 
		   abs(tmtime.tm_min), 
		   abs(tmtime.tm_sec),

		   (labs(tns) + K_NS_PER_MS/2)/K_NS_PER_MS
		   );

    inaccstr[0] = '\0';
    if (printForm == UTC_FORMREL) {
	if ( ins != -1 ) 
        {
            temp_sec = tmInacc.tm_sec;
            temp_ns = (ins + K_NS_PER_MS - 1) / K_NS_PER_MS;
            if (temp_ns >= 1000)
            {
              temp_ns = temp_ns - 1000;
              temp_sec++;
            }
	    (void) sprintf( inaccstr, "I%01ld.%03ld",
			   tmInacc.tm_yday * 24 * 60 * 60 +
			   tmInacc.tm_hour * 60 * 60 +
			   tmInacc.tm_min  * 60 +
			   temp_sec,
			   temp_ns);
        }
	
	else /* infinite inaccuracy */
	    (void) sprintf( inaccstr, "I-----");
    }
    
    length = strlen(inaccstr) + strlen(timestr);
    if (length < *stringlen) {
	(void) sprintf(cp, "%s%s", timestr, inaccstr);
	*stringlen = length;
	return SUCCESS;
    }
    
    *stringlen = -1;
    return FAILURE;
}

/*
 *   - O B S O L E T E - 
 */
/*
 *  utc_strtoutc()
 *
 *  Functional Description:
 *
 *  O B S O L E T E - see utc_mkasctime
 */
int utc_strtoutc(pVal, cp) 
char *cp; 
utc_t *pVal; 
{
    return(utc_mkasctime(pVal,cp));
}

/*
 *  utc_strtorel()
 *
 *  Functional Description:
 *
 *  O B S O L E T E - see mkascreltime
 */
int utc_strtorel(pVal,cp)
char *cp; 
utc_t *pVal;
{
    return(do_strtorel(pVal, cp, UTC_FORMREL));
}
#endif /* #if 0 */
