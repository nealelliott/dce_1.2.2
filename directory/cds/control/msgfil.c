/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: msgfil.c,v $
 * Revision 1.1.6.2  1996/02/18  19:29:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:28  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:10:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:46  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:01:10  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:46:24  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:29  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:26  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:00:21  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:05:00  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:07:57  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:39:36  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Program MSG, Module MSGFIL - VMS binary file operations
 *
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
 *
 * MODULE DESCRIPTION:
 *
 * Program MSG,  Module MSGFIL
 *
 * Perform binary file operations for VMS which are analogous to standard
 * "C" binary file operations.  This is necessary because of the differences 
 * on VMS.
 *
 * Main module.
 *
 *
 */

/*
 * General declarations.
 */

#include <cdscp.h>

#ifdef vms
/*
 * File control.
 */
#define NUMFILES  2
#define FILERR -1

#define TYPECLOSE 0
#define TYPEREAD  1
#define TYPEWRITE 2

static int outtp[NUMFILES] =		/* Type of I/O */
{TYPECLOSE, TYPECLOSE};


static int outfp[NUMFILES] = 		/* VMS block output file pointer */
{FILERR, FILERR};

static char outb1[512];			/* Input/Output buffers */
static char outb2[512];

static char *outbs[NUMFILES] =		/* Buffer starting addresses */
{&outb1[0], &outb2[0]};

static char *outbp[NUMFILES] =		/* Buffer pointer addresses */
{&outb1[0], &outb2[0]};

static int outln[NUMFILES] =		/* Buffer contents length */
{0, 0};

#endif

/*
 * pr_open ***
 *
 * Open file in VMS binary mode.  The inputs/outputs are kept as close to the
 * standard "fopen" call as possible.
 *
 * Inputs:
 *      filename - "char" pointer to file name string.
 *
 *      mode - "char" pointer to mode string.  This may be one of:
 *	        "r" (read only).
 *	        "w" (create and write).
 *
 * Outputs:
 *      "int" value of:
 *		+n if success.  This value must be saved.
 *		-1 if too many files open or illegal mode.
 *		NULL if open error
 */     

#ifdef vms

int
pr_open (
char *filename,
char *mode)
{
    register int index;
    register int outindex;

    /*
     * Find an open file pointer.
     */
    outindex = 0;
    for (index = 0 ; index < NUMFILES ; )
    {
    	if (outfp[index++] == FILERR)
    	{
    	    outindex = index;
    	    break;
    	}
    }

    if (outindex == 0)
    {
    	return (-1);
    }

    /*
     * Determine the mode and open the file.
     */
    switch (*mode)
    {
    	case 'r': outln[outindex-1] = 0;
    		  outfp[outindex-1] = open(filename, 0);
    		  outtp[outindex-1] = TYPEREAD;
    		  break;

    	case 'w': outln[outindex-1] = 0;
    		  outfp[outindex-1] = creat(filename, 0, "mrs = 512", "rfm = fix");
    		  outtp[outindex-1] = TYPEWRITE;
    		  break;

    	default:  return (-1);
    }

    /*
     * Check for open error.
     */
    if (outfp[outindex-1] == FILERR)
    {
    	outtp[outindex-1] = TYPECLOSE;
    	return (NULL);
    }

    /*
     * Return the result
     */
    return (outindex);
}

#endif

/*
 * pr_getc ***
 *
 * Read file opened in binary mode.  The inputs/outputs are kept as close to the
 * standard "fgetc" call as possible.
 *
 * Inputs:
 *      outindex - "int" value, as returned from "pr_open".
 *
 * Outputs:
 *      "int" value of:
 *		+n (0 to 256, character value from the file) if success.
 *		EOF if read error.
 */     

#ifdef vms

int
pr_getc (
register int outindex)
{
    register int retvalue;

    if (outtp[--outindex] != TYPEREAD)
    {
    	return (EOF);
    }

    if (outln[outindex] < 0)
    {
    	return (EOF);
    }

    if (outln[outindex] == 0)
    {
    	outbp[outindex] = outbs[outindex];
    	outln[outindex] = read(outfp[outindex], outbs[outindex], 512);

    	if (outln[outindex] <= 0)
    	{
    	    return (EOF);
    	}
    }

    outln[outindex]--;
    retvalue = *(outbp[outindex]++);
    retvalue &= 0xFF;
    return (retvalue);
}

#endif

/*
 * pr_putc ***
 *
 * Write file opened in binary mode.  The inputs/outputs are kept as close to 
 * the standard "fputc" call as possible.
 *
 * Inputs:
 *      character - "char" value, to be placed in the file.
 *      outindex - "int" value, as returned from "pr_open".
 *
 * Outputs:
 *      "int" value of:
 *		+n (0 to 256, character value placed in the file) if success.
 *		EOF if write error.
 */     

#ifdef vms

int
pr_putc (
register char character,
register int outindex)
{
    register int retvalue;

    if (outtp[--outindex] != TYPEWRITE)
    {
    	return (EOF);
    }

    if (outln[outindex] < 0)
    {
    	return (EOF);
    }

    *(outbp[outindex]++) = character;

    if (++outln[outindex] == 512)
    {
    	outln[outindex] = write(outfp[outindex], outbs[outindex], 512);

    	if (outln[outindex] < 0)
    	{
    	    return (EOF);
    	}

    	outbp[outindex] = outbs[outindex];
    	outln[outindex] = 0;
    }

    retvalue = character;
    retvalue &= 0xFF;
    return (retvalue);
}

#endif

/*
 * pr_close ***
 *
 * Close file opened in VMS binary mode.  The inputs/outputs are kept as close
 * to the standard "fclose" call as possible. 
 *
 * Inputs:
 *      outindex - "int" value, as returned from "pr_open".
 *
 * Outputs:
 *      "int" value of:
 *		0 if file closed.
 *		EOF if error.
 */     

#ifdef vms

int
pr_close (
register int outindex)
{
    register int result;

    if (outtp[--outindex] != TYPEWRITE)
    {
    	outln[outindex] = 0;
    }

    outtp[outindex] = TYPECLOSE;

    if (outln[outindex] > 0)
    {
	while (outln[outindex]++ < 512)
	{
	    *(outbp[outindex]++) = '\0';
	}

    	result = write(outfp[outindex], outbs[outindex], 512);

    	if (result < 0)
    	{
    	    close(outfp[outindex]);
    	    outfp[outindex] = FILERR;
    	    return (EOF);
    	}
    }

    result = close(outfp[outindex]);
    outfp[outindex] = FILERR;
    if (result == -1)
    {
    	return (EOF);
    }

    return (0);
}

#endif
