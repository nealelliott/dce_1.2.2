/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: time_actions.c,v $
 * Revision 1.1.8.2  1996/02/18  19:30:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:59  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:12:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:10  root]
 * 
 * Revision 1.1.6.3  1994/08/03  19:01:26  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:43:49  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:37:51  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:55  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:01:19  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:07:29  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  13:26:35  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:42:46  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/10  15:39:02  mfox
 * 	Replace usage of obsolete utc functions
 * 	[1992/12/10  15:36:37  mfox]
 * 
 * Revision 1.1.2.2  1992/06/09  16:17:37  m_sawyer
 * 	Add code to handle TDF's to allow users to input local times and
 * 	have them converted to UTC.
 * 	[1992/06/09  16:16:43  m_sawyer]
 * 
 * Revision 1.1  1992/01/19  15:22:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
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
 *
 */

/* time_actions.c contains parser action routines that deal with time */

#include <cdscp.h>
#ifdef unix
#include <sys/time.h>
#endif
#include <dce/sys_time.h>
#include <prsdef.h>
#include <dce/utc.h>

/* there is only one place for the parser to leave an absolute time */
/* there is only one place for the parser to leave a  delta    time */
/* there is only one place for the parser to leave a  timestamp     */
/* other parser action routines must move the times away soon enough */

/* the only way to access the parsed times is through these access routines. */

static utc_t abstime;

utc_t *
abstime_pointer (void)
{
    return(&abstime);
}

static utc_t deltatime;

utc_t *
deltatime_pointer (void)
{
    return(&deltatime);
}

/* the real work of parsing a time string is done by the called routines. */
/* these routines just hook them to the parser. */
/* unfortunately, the called parser insists on a null terminated string. */
/* so first we parse the input to isolate the string for the parser. */
/* the parser delivers the time as a 128 bit utc. */

/*
 * get_tdf - get the local tdf
 */
static pthread_once_t get_tdf_once;

static char tdf_string[7] = {'\0'};    /* eg. '+4:00' */

static void
get_tdf (void)
{
    char local_zone[4] = {'\0'};     /* eg. 'EST' */
    long local_tdf = 0;
    long local_tdf_abs = 0;
    int status;

    status = utc_localzone (&local_zone[0],
                            sizeof(local_zone),
                            &local_tdf,
                            NULL,
                            NULL);
    if (status != 0) return;

    /*
     * convert seconds to hours and perform a sanity check on the
     * resultant tdf
     */
    local_tdf /= 60 * 60;
    local_tdf_abs = abs (local_tdf);
    if (local_tdf_abs > 23)
    {
        local_tdf_abs = local_tdf = 0;
    }
    tdf_string[0] = local_tdf < 0 ? '-' : '+';

    sprintf (&tdf_string[1], "%d:00", local_tdf_abs);
}


int
parse_abstime (
    struct prs *pcb)			/* ptr to parser context block */
{
    int return_code;			/* summary status for parser */
    int status;				/* returned by subroutines */
    char buf[99];			/* big enough scratch area */
    static char abschars[] = "0123456789-:.";
    int len;				/* number of chars in string abs time */

    pthread_once (&get_tdf_once, get_tdf);

    return_code = ACT_ERROR;		/* assume failure until success */
    len = strspn(pcb->cur, abschars);	/* eg 1990-12-25-13:01:02.1234567 */
    if (len)				/* 0 means none of the chars seen */
	{				/* a time was found */
	strncpy(buf, pcb->cur, len);	/* extract the needed portion */
	buf[len] = 0;			/* null terminate it */
        strcat (buf, tdf_string);       /* tack on the tdf */
	status =			/* try to parse it */
	    utc_mkasctime(		/* result is gregorian based */
		abstime_pointer(),
		buf);
	if (status == 0)
	    {				/* we got an absolute time */
	    pcb->nxt = 
		pcb->cur + len;		/* tell parser where to resume */
	    return_code = 
		ACT_SUCCESS;		/* tell parser of our success */
	    }				/* end of real parse worked */
	}				/* end of preparse worked */
    return(return_code);		/* tell parser what happened */
}					/* end of routine int parse_abstime */

int
parse_deltatime (
    struct prs *pcb)			/* ptr to parser context block */
{
    int return_code;			/* summary status for parser */
    int status;				/* returned by subroutines */
    char buf[99];			/* big enough scratch area */
    static char relchars[] = "0123456789-:+.";
    int len;				/* number of chars in string rel time */

    return_code = ACT_ERROR;		/* assume failure until success */
    len = strspn(pcb->cur, relchars);	/* eg 25-13:01:02.1234567 */
    if (len)				/* 0 means none of the chars seen */
	{				/* a delta time was found */
	strncpy(buf, pcb->cur, len);	/* extract the needed portion */
	buf[len] = 0;			/* null terminate it */
	status =			/* try to parse it */
	    utc_mkascreltime(deltatime_pointer(), buf);
	if (status == 0)
	    {				/* we got a delta time */
	    pcb->nxt = 
		pcb->cur + len;		/* tell parser where to resume */
	    return_code = 
		ACT_SUCCESS;		/* tell parser of our success */
	    }				/* end of real parse worked */
	}				/* end of preparse worked */
    return(return_code);		/* tell parser what happened */
}					/* end of parse_deltatime */
