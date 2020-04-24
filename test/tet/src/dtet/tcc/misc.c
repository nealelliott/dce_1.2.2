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
 *
 */

#ifndef lint
static char sccsid[] = "@(#)misc.c	1.3 (92/11/27) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)misc.c    1.3 11/27/92
NAME:		misc.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Miscellaneous functions called by other functions

MODIFICATIONS:
		DTET Development
		David G. Sawyer
		John-Paul Leyland
		UniSoft Ltd.

************************************************************************/

#include <tcc_env.h>
#include <tcc_mac.h>
#include <dtet/tet_jrnl.h>
#include <tcc_prot.h>


/*
 *  Query whether a mode is active or else set the mode active.
 */
int setmode(mode, setvalue)
int mode;
int setvalue;
{
	static int build_mode = FALSE;
	static int exec_mode = FALSE;
	static int clean_mode = FALSE;
	int return_val = FALSE;

	if(setvalue == FALSE)
	{
		switch(mode)
		{
		case BUILD:
			return_val = build_mode;
			break;
		case EXEC:
			return_val = exec_mode;
			break;
		default: /* CLEAN */
			return_val = clean_mode;
			break;
		}
	}
	else
	{
		switch(mode)
		{
		case BUILD:
			build_mode = TRUE;
			break;
		case EXEC:
			exec_mode = TRUE;
			break;
		default: /* CLEAN */
			clean_mode = TRUE;
			break;
		}
	}

	return(return_val);
}


/*
 *  Is the appropriate mode active ? returns TRUE or FALSE
 */
int ifmode(mode)
int mode;
{
	return( setmode(mode, FALSE));
}


/*
 *   If query is FALSE then set the gtimeout.
 *   Returns the value of gtimeout
 */
int set_gtimeout(value, setvalue)
int value;
int setvalue;
{
	static int g_timeout = 0;

	if(setvalue == TRUE)
		g_timeout = value;

	return(g_timeout);
}


/*
 *  What is the value of g_timeout ?
 */
int gtimeout()
{
	return(set_gtimeout(0,FALSE));
}


/*
 *   Whether or not the -p option has been specified
 */
int mode_message(flag)
int flag;
{
	static active = FALSE;

	if(flag == NOQUERY) /* set the value */
		active = TRUE;

	return(active);
}


/*
 *  Have we finished setting everything up ?
 */
int setup_complete(flag)
int flag;
{
	int static active = FALSE;

	if(flag == NOQUERY)
		active = TRUE;

	return(active);
}


int abort_requested()
{
	return(set_abort_req(QUERY));
}


int set_abort_req(flag)
int flag;
{
	static int active = FALSE;

	if(flag == NOQUERY)
		active = TRUE;

	return(active);
}


/*
 *  Establish a unique sync id and xres id for use by the remote directives
 */
int xresd_syncd_init(systems, num_systems)
struct sysid_store *systems;
int num_systems;
{
	int ret_val = SUCCESS;
        int *syslist;
	int count;
	char xresfile[_POSIX_PATH_MAX];
	char *temp_dir;
	extern long sd_snget();
	extern int sd_snsys();

	DBUG(DBGFDESC) (stderr,"xresd_syncd_init(systems:%ld,num_systems:%d)\n",(long) systems,num_systems);

	temp_dir = get_temp_dir(FALSE, MASTER);

	syslist = (int *) TET_MALLOC(sizeof(int) * num_systems);

	for(count = 0; count < num_systems; count++)
		syslist[count] = systems[count].sysid;

	(void) sprintf(xresfile, "%s/xresd%.5d", temp_dir, (int) getpid());

	/* just in case it's already there */
	(void) unlink(xresfile);

	DBUG(DBGFINFO) (stderr,"Calling sd_snget()\n");
	if((snid = sd_snget()) < 0) /* Get a sync id */
	{
	    ERROR("sd_snget() failed.\n");
	    ret_val = FAILURE;
	}
	else /* Let the sync daemon know what systems are involved */
	{
		DBUG(DBGFINFO) (stderr,"Calling sd_snsys(snid:%ld,syslist:%ld,num_systems:%d)\n",snid,syslist,num_systems);
		if(sd_snsys(snid,syslist,num_systems) < 0)
		{
		    ERROR("sd_snsys() failed.\n");
		    ret_val = FAILURE;
		}
	}

	TET_FREE((void *) syslist);

	return(ret_val);
}

