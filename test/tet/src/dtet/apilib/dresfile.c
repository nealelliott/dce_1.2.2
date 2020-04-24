/*
 * Copyright 1990 Open Software Foundation (OSF)
 * Copyright 1990 Unix International (UI)
 * Copyright 1990 X/Open Company Limited (X/Open)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of OSF, UI or X/Open not be used in 
 * advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission.  OSF, UI and X/Open make 
 * no representations about the suitability of this software for any purpose.  
 * It is provided "as is" without express or implied warranty.
 *
 * OSF, UI and X/Open DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO 
 * EVENT SHALL OSF, UI or X/Open BE LIABLE FOR ANY SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef lint
static char sccsid[] = "@(#)dresfile.c	1.5 (92/11/23) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)dresfile.c	1.5 92/11/23
NAME:		'C' API results file functions
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Geoff Clare, UniSoft Ltd.
DATE CREATED:	25 July 1990
SYNOPSIS:

	void tet_infoline(char *data);
	void tet_result(int result);
	void tet_setcontext(void);
	void tet_setblock(void);

	void tet_openres(void);
	void tet_error(int errno_val, char *msg);
	long tet_context;
	long tet_block;

DESCRIPTION:

	Tet_infoline() outputs the specified line of data to the
	execution results file, prefixed by the current context, block
	and sequence numbers.  The data should not contain any newline
	characters.

	Tet_result() specifies the result code which is to be entered
	in the execution results file for the current test purpose.
	It stores the result in a temporary file which is later read
	by tet_tpend().

	Tet_setcontext() sets the current context to the current
	process ID and resets the block and sequence numbers to 1.

	Tet_setblock() increments the current block number and
	resets the sequence number to 1.

	Tet_error() is not part of the API.  It is used by API functions
	to report errors to stderr and the results file.

	Tet_openres() is not part of the API: it is used by the TCM to
	open the execution results file.

	Tet_context and tet_block are not part of the API: they are used
	by API functions to access the current context and block numbers.


MODIFICATIONS:
	
	June 1992
	DTET development - this file is derived from TET release 1.10

	Andrew Dingwall, UniSoft Ltd., October 1992
	Moved non-API functions for IC/TP start/end to tcm/ictp.c
	because these are specific to parent TCMs and do different things
	in MTCM and STCMs.

	All vestages of the local execution results file and temporary
	execution results file removed - in the DTET, all the processing
	associated with these files is done by XRESD.

************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include "dtmsg.h"
#include "servlib.h"
#include "dtetlib.h"
#include "dtet/tet_api.h"
#include "dtet/tet_jrnl.h"

#define TET_JNL_LEN	512	/* max length of a journal line */

extern void	(*tet_cleanup)();
extern char *	tet_errname();
extern int	tet_thistest;

int combined_ok = 0; /* Is it OK to try and write to the combined xres file */

static void tet_msgform();
void	tet_error();
long	tet_activity = -1;
long	tet_context = 0;
long	tet_block = 0;
long	tet_sequence = 0;

/*
**	tet_openres() - open the local execution results file
**
**	in the TET, results go to this file
**	in the DTET, results go to XRESD - however, the DTET tcc still
**	expects a TCM to create this file if output capture is set to FALSE
**	(I think - andrew)
*/

void
tet_openres()
{
	int resfd, err;
	mode_t resmode = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;
	char errmsg[512];
	static char resfile[] = "tet_xres";

	(void) unlink(resfile);
	resfd = open(resfile, O_WRONLY|O_CREAT|O_TRUNC|O_APPEND, resmode);
	if (resfd < 0)
	{
		err = errno;
		(void) sprintf(errmsg, "can't open results file \"%s\"",
			resfile);
		tet_error(err, errmsg);
		tet_exit(EXIT_FAILURE);
	}

	(void) close(resfd);
}

/*
**	tet_infoline() - send a TCM information line to the combined
**		results file
*/

void
tet_infoline(data)
char *data;
{
	extern int Mysysid, xd_errno;
	extern long Xrid;
	char header[128];
	char outbuf[TET_JNL_LEN];

	if (tet_context == 0)
		tet_setcontext();

	if (data == NULL)
		data = "(null pointer)";

	/* generate the info line preamble and format the line */
	(void) sprintf(header, "%d|%ld %ld %03d%05ld %ld %ld|", 
		TET_JNL_TC_INFO, tet_activity, tet_thistest, 
		Mysysid, tet_context, tet_block, tet_sequence++);
	tet_msgform(header, data, &outbuf[0]);

	/* Call generic function to enter line in combined xres file */
	if (xd_xres(Xrid, outbuf) < 0) {
		switch (xd_errno) {
		case ER_INVAL:
		case ER_ERR:
			break;
		default:
			combined_ok = 0;
			break;
		}
		tet_error(xd_errno,
			"tet_infoline(): can't send info line to XRESD");
		tet_exit(EXIT_FAILURE);
	}
}

/*
**	tet_result() - send a test purpose result to XRESD
*/

void
tet_result(result)
int result;
{
	extern int xd_errno;
	extern long Xrid;
	char errmsg[128];

	/* Call generic function to tell xresd the result */
	if (xd_result(Xrid, result) < 0)
	{
		switch (xd_errno) {
		case ER_INVAL:
		case ER_SYSID:
		case ER_DONE:
		case ER_ERR:
			break;
		default:
			combined_ok = 0;
			break;
		}
		(void) sprintf(errmsg,
			"tet_result(): can't send result %d to XRESD",
			result);
		tet_error(xd_errno, errmsg);
		tet_exit(EXIT_FAILURE);
	}
}

void
tet_setcontext()
{
	/* Set current context to process ID and reset block & sequence */

	pid_t pid = getpid();

	if (tet_context != pid)
	{
		tet_context = pid;
		tet_block = 1;
		tet_sequence = 1;
	}
}

void
tet_setblock()
{
	/* Increment current block & reset sequence number within block */

	tet_block++;
	tet_sequence = 1;
}

/*
**	tet_error() - print an error message to:
**		stderr
**		combined results file via XRESD
**
**	if the XRESD channel fails, arrangements are made for
**	that channel to be disabled and tet_error() is called recursively
**	to report the failure
**
**	errno_val may be +ve to report a Unix errno value, or -ve to
**	report a DTET server error reply code
*/

void
tet_error(errno_val, msg)
int errno_val;
char *msg;
{
	register char *p;
	char header[128];
	char errbuf[TET_JNL_LEN];
	extern char *tet_pname;
	extern long Xrid;
	extern int Mysysid;

	if(combined_ok != 1)
	{
		/* Can't write to "tet_combined" so write to stderr instead */
		/* write message to stderr */
		(void) fprintf(stderr, "%s: %s", tet_pname, msg);
		if (errno_val > 0)
		{
			(void) fprintf(stderr, ", errno = %d (%s)", 
				errno_val, tet_errname(errno_val));
		}
		else if (errno_val < 0)
		{
			(void) fprintf(stderr, ", reply code = %s",
				ptrepcode(errno_val));
		}
		(void) fprintf(stderr, "\n");
	}
	else
	{

		/* 
		 *  format error message for the results file - put 
		 *  errno first so as to avoid it being lost by truncation
		 */
		p = header;
		(void) sprintf(p, "%d|%ld|system %d", TET_JNL_TCM_INFO,
			tet_activity, Mysysid);
		p += strlen(p);
		if (errno_val > 0)
			(void) sprintf(p, ", errno = %d (%s)",
				errno_val, tet_errname(errno_val));
		else if (errno_val < 0)
			(void) sprintf(p, ", relpy code = %s", 
							ptrepcode(errno_val));
		p += strlen(p);
		(void) sprintf(p, ": ");

		/* Check the message format and write it into errbuf */
		tet_msgform(header, msg, &errbuf[0]);

		/* send error message to XRESD */
		if (xd_xres(Xrid, errbuf) < 0)
		{
			/* No longer OK to write to "tet_combined" file */
			combined_ok = 0;

			tet_error(xd_errno, "tet_error(): Failed to send the following error message to XRESD :");
			tet_error(0,errbuf);

			tet_exit(EXIT_FAILURE);
		}
	}
}

/*
**	tet_msgform() - format a TCM journal line -
**		translate newlines to tabs,
**		make sure that the line does not exceed 512 bytes
**		as required by the spec
**
**	on return, the formatted line is stored in outbuf
*/

static void tet_msgform(header, data, outbuf)
char *header, *data, *outbuf;
{
	register char *p1, *p2;
	static char fmt[] =
		"warning: results file line truncated - prefix: %.*s";
	char errmsg[128];

	p2 = outbuf;

	/* copy over the header preamble */
	for (p1 = header; *p1 && p2 < &outbuf[TET_JNL_LEN - 1]; p1++, p2++)
		*p2 = *p1;

	/* copy over the data, performing translations */
	for (p1 = data; *p1 && p2 < &outbuf[TET_JNL_LEN - 1]; p1++, p2++)
		switch (*p1) {
		case '\n':
			*p2 = '\t';
			break;
		default:
			*p2 = *p1;
			break;
		}

	/* terminate the line, removing trailing while space */
	do {
		*p2-- = '\0';
	} while (isspace(*p2));

	/* see if the line was truncated */
	if (*p1) {
		(void) sprintf(errmsg, fmt, sizeof errmsg - sizeof fmt,
			header);
		tet_error(0, errmsg);
	}
}

