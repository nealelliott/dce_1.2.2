/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: prsrea.c,v $
 * Revision 1.1.6.2  1996/02/18  19:30:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:50  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:11:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:04  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:01:20  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:44:02  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:45  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:47  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:00:55  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:06:47  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:09:24  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:41:29  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:22:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Library PRS, Module PRSREA - Read a parse table into memory.
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
 * Library PRS,  Module PRSREA
 *
 * Get an entire parse table file into memory.
 *
 *
 * Networks & Communications Software Engineering
 *
 * EDIT HISTORY:
 *
 * 1.00  25-May-85
 *       Networks and Communications
 *
 * 1.01	20-Aug-85
 *	Put in system specific binary file I/O controls.
 *
 * END EDIT HISTORY
 */

/*
 * General declarations.
 */

#include <cdscp.h>
#include <prspre.h>
#include <prsdef.h>

#ifdef __osf__
#undef FGETCB
#define FGETCB(f)	getc(f)		/* Hack for TIN port */
#endif
/*
 * File control if non-VMS.
 */
#ifndef vms
static FILE *file_p1;			/* File pointer for non-VMS */
#endif

/*
 * File control if VMS.
 */
#ifdef vms
static int file_p1;			/* File result for VMS */
static int file_s1;			/* File system context for VMS */
#define FILERR -1			/* VMS open error value */
static char outb1[512];			/* Input/Output buffer */
static char *outbp;			/* Buffer pointer address */
static int outln;			/* Buffer contents length */
#endif

static int pr_rd1(struct prs *);

/*
 * pr_read ***
 *
 * Get a an entire parse table file into caller's buffer.
 *
 * Inputs:
 *	context - Address of argument structure.
 *
 *		The following entries must be set prior to the call.
 *
 *		tbl -	"char" pointer to parse table buffer.
 *		tbs -	"unsigned int" size of parse table buffer.
 *
 *	file_name - Char pointer to file name string.
 *
 * Outputs:
 *	int	RET_SUCCESS	Parse table file has been read in.
 *		RET_OPENERROR	Parse table file open error.
 *		RET_IOERROR	Parse file I/O error.
 *		RET_VERSION	Parse file version error.
 *		RET_FILE	Parse file had illegal contents.
 *		RET_SIZE	Parse table buffer too small.
 *
 *	Argument structure entries:
 *
 *		The following entries are valid as output.
 *
 *		tbl -	parse table buffer now filled in with contents of file.
 *		tbu -	"unsigned int" amount of buffer actually used.
 *
 *		The following entries are unmodified.
 *
 *		tbl -	"char" pointer to parse table buffer.
 *		tbs -	"unsigned int" size of parse table buffer.
 *		cmd -	"char" pointer to command buffer.
 *		cms -	"int" size of command buffer.
 *		act -	"struct" pointer to action routine array.
 *
 *		All other entries are invalid.
 */

int
pr_read(
struct prs *context,
char *file_name)
{
    int result;

    /*
     * Open the file, for READ, in BINARY mode.
     */
    if ((file_p1 = FOPENB(file_name, FINP)) == (int)NULL)
    {
    	return (RET_OPENERROR);
    }

    /*
     * Read in the file.
     */
    result = pr_rd1(context);

    /*
     * Close the file.
     */
    FCLOSB(file_p1);

    /*
     * Finished.
     */
    return (result);
}

/*
 * pr_rd1 ***
 *
 * Perform the actual file read operation.  This is done in a subroutine
 * in order to save having to close the file multiple places.
 *
 * Inputs:
 *	context - Address of argument structure, from "pr_read".
 *
 * Outputs:
 *	int, same as "pr_read".
 */

static int
pr_rd1(
struct prs *context)
{
    /*
     * Local data.
     */
    int file_char;			/* Integer character from file */
    register char *file_buffer;		/* Local pointer to buffer */
    unsigned int file_size;		/* File size */
    char local_buffer[4];		/* In case buffer much too small */

    /*
     * Must be at least 4 bytes in order to read in the version and size.
     */
    file_buffer = context->tbl;

    if (context->tbs < 4)
    {
    	file_buffer = &local_buffer[0];
    }

    /*
     * Check the version/edit.
     */
    if ((file_char = FGETCB(file_p1)) == EOF)
    {
    	return (RET_IOERROR);
    }

    *file_buffer = file_char;
    if (*file_buffer++ != PRS_VERSION)
    {
	return (RET_VERSION);
    }

    if ((file_char = FGETCB(file_p1)) == EOF)
    {
    	return (RET_IOERROR);
    }

    *file_buffer = file_char;
    if (*file_buffer++ > PRS_EDIT)
    {
	return (RET_VERSION);
    }

    /*
     * Check the file size
     */
    if ((file_size = FGETCB(file_p1)) == EOF)
    {
    	return (RET_IOERROR);
    }

    if ((file_char = FGETCB(file_p1)) == EOF)
    {
    	return (RET_IOERROR);
    }

    *file_buffer++ = file_size;
    *file_buffer++ = file_char;

    file_size |= file_char << 8;
    context->tbu = file_size;

    if (context->tbs < file_size)
    {
	return (RET_SIZE);
    }

    /*
     * Read the file into memory.
     */
    file_size -= 4;
    while (file_size-- > 0)
    {
    	if ((file_char = FGETCB(file_p1)) == EOF)
    	{
    	    return (RET_IOERROR);
    	}

    	*file_buffer++ = file_char;
    }

    /*
     * File has been read in.
     */
    return (RET_SUCCESS);
}

/*
 * pr_open ***
 *
 * Open file in VMS binary mode.  The inputs/outputs are kept as close to the
 * standard "fopen" call as possible.
 *
 * Inputs:
 *      filename - "char" pointer to file name string.
 *
 *	mode - "char" pointer to open mode, must point to "r".
 *
 * Outputs:
 *      "int" value of:
 *		+n if success.  This value must be saved.
 *		-1 if too many files open or illegal mode.
 *		NULL if open error
 */     

#ifdef vms

static int
pr_open(
char *filename,
char *mode)
{
    /*
     * Open the file.
     */
    if (*mode != 'r')
    {
	return (-1);
    }

    outln = 0;    

    if ((file_s1 = open(filename, 0)) == FILERR)
    {
    	return ((int)NULL);
    }

    return (1);
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

static int
pr_getc(
register int outindex)
{
    register int retvalue;

    if (outindex != 1)
    {
    	return (EOF);
    }

    if (outln < 0)
    {
    	return (EOF);
    }

    if (outln == 0)
    {
    	outbp = &outb1[0];
    	outln = read(file_s1, &outb1[0], 512);

    	if (outln <= 0)
    	{
    	    return (EOF);
    	}
    }

    outln--;
    retvalue = *(outbp++);
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

static int
pr_close(
register int outindex)
{
    if (outindex != 1)
    {
    	return (EOF);
    }

    close(file_s1);
    return (0);
}

#endif
