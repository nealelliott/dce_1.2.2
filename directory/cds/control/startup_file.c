/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: startup_file.c,v $
 * Revision 1.1.8.2  1996/02/18  19:30:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:58  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:12:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:09  root]
 * 
 * Revision 1.1.5.3  1994/08/03  19:01:25  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:43:51  mccann]
 * 
 * Revision 1.1.5.2  1994/06/09  18:37:50  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:53  devsrc]
 * 
 * Revision 1.1.5.1  1994/03/12  22:01:13  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:07:22  peckham]
 * 
 * Revision 1.1.3.3  1992/12/30  13:26:26  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:42:23  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  19:11:28  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:11:35  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:45  devrcs
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
 */

#include <cdscp.h>
#ifdef SNI_SVR4_POSIX
#include <sys/types.h>
#endif
#ifndef vms
#include <fcntl.h>
#endif

#ifndef R_OK
#define R_OK 4
#endif

/* processing the startup file for dnscp */



void
use_startup_file (void)
{
    char *p;				/* ptr to spec from command line */
    int bypass;				/* true to bypass use of init file */
    char *spec;				/* ptr to file spec asciz string */
    static char specbuf[1024+1];	/* max allowed by ultrix + null */
    char *homep;			/* ptr to spec of home dir */
    int homelen;			/* length of home spec */
    static char filep[] = ".cdscpinit";	/* name of default init file */
    int filelen;			/* number of chars in it */

    bypass = 0;				/* normally we use the init file */

/* 
**  For VMS, only support the default init file of sys$login:dnscpinit.  No
**  overrides are allowed on the initial command line.
*/
#ifndef VMS 
    p = startup_file_requested();
    if (p)
	{
	if ((*p=='-')&&(*(p+1)==0))	/* - means do not use an init file */
	    {
	    bypass = 1;			/* bypass use of init file */
	    }
	else
	    {
	    spec = p;			/* use file given on cmd line */
	    };
	}
    else				/* nothing on command line. */
	{				/* use the default init file */
	homep = getenv("HOME");
	homelen = homep ? strlen(homep) : 0;
	filelen = strlen(filep);
	if (homep && (homelen + filelen + 2 < sizeof(specbuf)))
	    {
	    sprintf(specbuf, "%s/%s", homep, filep);
	    spec = specbuf;		/* look in home directory */
	    }
	else
	    {
	    spec = filep;		/* look only in current directory */
	    };
	};				/* end of no init spec on cmd line */

#else /* if VMS */

    spec = filep;

#endif

    if (!bypass)
	{
	int access_status;

	access_status = access(spec, R_OK);
	if (access_status == 0)
	    {
	    struct envblk *const envbp = get_envblk_ptr();

	    (void)exec_docmd(
		    spec,		/* ptr to file spec to process */
		    entitylist_pointer(),	/* ptr to head of entity list */
		    get_attrblk_ptr(),	/* ptr to head of attributes list */
		    envbp,		/* ptr to environment block */
		    envbp->curcmd,	/* ptr to command block */
		    get_op_blk_ptr());	/* ptr to head of output list */		
	    }				/* end of access to file ok */
	}				/* end of not bypass */
    return;				/* no value from void routine */
}
