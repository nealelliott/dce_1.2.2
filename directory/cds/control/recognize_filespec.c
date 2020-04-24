/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: recognize_filespec.c,v $
 * Revision 1.1.6.2  1996/02/18  19:30:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:52  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:11:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:05  root]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:46  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:48  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:00:59  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:06:53  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:09:34  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:41:38  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* recognize_filespec.c */
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

/* given a ptr to a string, return the number of chars in the file spec */
/* that starts there.  0 means there is no file spec. */

#include <cdscp.h>
#ifdef vms
#include <stdio.h>			/* standard c i/o package, for debug */
#include <descrip.h>			/* vms descriptors */
#include <fscndef.h>			/* defs for filescan sys service */
#endif					/* end of extra vms header files */

#ifdef vms

int
recognize_vms_filespec (
    unsigned char *specp)
{
    int fldflags;			/* filescan puts mask here. ignore it*/
    int status;				/* returned by filescan syssvc */
    struct dsc$descriptor spec;		/* where to look for file spec */
    struct 
	{
	short int length;		/* of file spec found */
	short int code;			/* for what to look for */
	char *addr;			/* where it was found */
	int scan_end;			/* must be zero */
	}   scan_results;		/* this is a vms item list */

    /* fill in the descriptor */
    spec.dsc$w_length = strlen(specp);	/* the rest of the input */
    spec.dsc$a_pointer= (char *)specp;	/* start looking for spec here */
    spec.dsc$b_class  = DSC$K_CLASS_S;	/* static string */
    spec.dsc$b_dtype  = DSC$K_DTYPE_T;	/* of ASCII bytes */

    /* set up the results block */
    scan_results.scan_end = 0;		/* mark the end of the list */
    scan_results.code = FSCN$_FILESPEC;	/* look for a filespec */

    status = SYS$FILESCAN(&spec, &scan_results, &fldflags);
    if (status != 1)
	{				/* accvio or bad item list */
	LIB$SIGNAL(status);		/* admit bug in source program */
	};

    /* if both length and address are 0, filespec was not recognized. */
    /* no file spec means tell caller there was no spec */
    /* only one zero is a bug, but we do not look for it */

    /*
    if (DEBUG.COM_PARSE || DEBUG.OP_LIST)
	{
	int i;
	printf("spec_length = %d.\n", scan_results.length);
	for (i=0; i<scan_results.length; i++) 
	    printf("%c", *(spec.dsc$a_pointer+i));
	printf("\n");
	};
    */

    return(scan_results.length);
}
#endif					/* end of vms version */


#ifdef unix

int
recognize_unix_filespec(
    unsigned char *specp)
{
    /* this table controls the recognition of unix file specs */
    /* each byte of the input is used to index the table. */
    /* a result of 0 means invalid, so end the scan. */
    /* a result of >= '!' is valid, so the scan should be continued. */
    /* other results are invalid now, but reserved for future use. */
    /* they will allow various kinds of quoting, substitution, */
    /* and wild cards. these are all 1 now. */
    static unsigned char unixspectt[256] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,46,47,
	48,49,50,51,52,53,54,55,56,57,1,1,1,1,1,1,
	1,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,90,1,1,1,1,95,
	1,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
	112,113,114,115,116,117,118,119,120,121,122,1,1,1,1,0,

	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	/* 8 bit on */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    unsigned char *p;			/* to scan the file spec */

    for (p=specp; unixspectt[*p]>='!'; p++)	/* find end of spec */
	;				/* no body of for loop */

    return(p - specp);			/* the length of the spec */
}
#endif					/* end of unix version */

/* vms and unix versions have different names on purpose. */
/* this allows us to support cross system specs, later, if we want to. */
/* easy to add an ms-dos or os/2 version, using a similar table */

