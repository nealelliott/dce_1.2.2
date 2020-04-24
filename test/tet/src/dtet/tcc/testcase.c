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
static char sccsid[] = "@(#)testcase.c	1.10 (93/01/25) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)testcase.c    1.10 1/25/93
NAME:		testcase.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions to handle forking for the parallel directive.

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



/* global system list variables */
int gnumsys = 0;
int *gsyslist;
int gmysysid = -1;

struct tc_store *remote_tc = (struct tc_store *)0;
int num_remote_tc = 0;


/*
 *  Build, Execute and Clean a TC
 */
void bec_tc(testcase,systems,num_systems)
char *testcase;
struct sysid_store *systems;
int  num_systems;
{
    int ret_val = SUCCESS;


    DBUG(DBGFDESC) (stderr,"bec_tc(%s,systems:%ld,num_systems:%d)\n", testcase, (long) systems, num_systems);

    /* check to see whether this TC should be processed */
    if(check_line(testcase) == FAILURE)
    {
        DBUG(DBGFINFO) (stderr,"TC not to be processed.\n");

        return;
    }

    if(ifmode(BUILD))
    {
	/*
	 *  grp = FALSE, grp_pipe_fd = not set, override_eip = FALSE
	 */
	ret_val = proc_tc_dir(testcase,systems,num_systems,BUILD,FALSE,0,FALSE);

        /* Completed another operation - time to note the change of context */
        ++exec_ctr;
    }
    if(ifmode(EXEC) && ret_val != FAILURE)
    {
	/*
	 *  grp = FALSE, grp_pipe_fd = not set, override_eip = FALSE
	 */
	ret_val = proc_tc_dir(testcase,systems,num_systems,EXEC,FALSE,0,FALSE);

        /* Completed another operation - time to note the change of context */
        ++exec_ctr;
    }
    if(ifmode(CLEAN) && ret_val != FAILURE)
    {
	/*
	 *  grp = FALSE, grp_pipe_fd = not set, override_eip = FALSE
	 */
	(void) proc_tc_dir(testcase,systems,num_systems,CLEAN,FALSE,0,FALSE);

        /* Completed another operation - time to note the change of context */
        ++exec_ctr;
    }

    return;
}


/*
 *  process a TC directive and decide what, if any, type of wrapper function
 *  should be called rather than trying to run a TC directly.
 */
int proc_tc_dir(testcase,systems,num_systems,mode,group,group_pipe_fd,override_eip)
char *testcase;
struct sysid_store *systems;
int num_systems;
int mode;
int group;
int *group_pipe_fd;
int override_eip;
{
	int ret_val;


	DBUG(DBGFDESC) (stderr,"proc_tc_dir(%s,systems:%ld,num_systems:%d,mode:%d,group:%d,group_pipe_fd:%ld,override_eip:%d)\n", testcase, (long) systems, num_systems, mode, group, (long) group_pipe_fd, override_eip);


	if(num_systems > 0)
	{
		/* A testcase involving remote systems */

		ret_val = gsys_wrapper(testcase, systems, num_systems, mode, group, group_pipe_fd, override_eip);
	}
	else
	{
		/* remote = FALSE, sysid_value = -1 */

		ret_val = run_tc(testcase, mode, FALSE, -1, group, group_pipe_fd, override_eip);
	}

	return(ret_val);
}


/* 
 *  Make the global systems list needed by ti_tcmputenv() etc.
 */
void make_gsyslist(systems, num_systems)
struct sysid_store *systems;
int num_systems;
{
	int i;


	DBUG(DBGFDESC) (stderr,"make_gsyslist(systems:%ld,num_systems:%d)\n", (long) systems, num_systems);

	gnumsys = num_systems;

        gsyslist = (int *) TET_MALLOC(sizeof(int) * num_systems);

        for(i = 0; i < gnumsys; i++)
	{
                gsyslist[i] = systems[i].sysid;
	}
}


/*
 *  reset the global systems list
 */
void clean_gsyslist()
{
	DBUG(DBGFDESC) (stderr,"clean_gsyslist()\n");

	gnumsys = 0;
	gmysysid = -1;
	TET_FREE((void *) gsyslist);
}


/*
 *  function for running distributed test cases - ie: testcases inside
 *  a remote directive with more than 1 remote system specified.
 */
int rem_grp(testcase, systems, num_systems, mode, group, group_pipe_fd)
char *testcase;
struct sysid_store *systems;
int num_systems;
int mode;
int group;
int *group_pipe_fd;
{
	int i;
	int remote_pipe_fd[2];
	int ret_val = SUCCESS;


	DBUG(DBGFDESC) (stderr,"rem_grp(%s,systems:%ld,num_systems:%d,mode:%d,group:%d,group_pipe_fd:%ld)\n",testcase, (long) systems, num_systems, mode, group, (long) group_pipe_fd);

	/*
	 *  Make a new list of parallel processes, each element of which
	 *  contains just one remote system.
	 */
	num_remote_tc = 0;
	for(i = 0; i < num_systems; i++)
	{
	    num_remote_tc = store_tc(&remote_tc, testcase, &(systems[i]), 1, scenario_line, scenario_element, num_remote_tc);
	
	    /*
	     *  if this remote is part of a parallel directive then make sure
	     *  we inherit any relevant flags.
	     */
	    if(group == TRUE)
	    {
		remote_tc[i].duplicate = systems[i].duplicate;	
		remote_tc[i].override_eip = systems[i].override_eip;	
	    }
	}

	/*
	 *  If we are within a group (ie: for a parallel execution), we need
	 *  to synchronise with the other elements of the group before we
	 *  start forking again.
	 */
	if(group == TRUE)
	{
	    /*
	     *  Signal parent to indicate that we are ready to continue and
	     *  then wait for the parent to signal back.
	     */
	    signal_parent(group_pipe_fd);
	    child_wait();
	}

        proc_tc_grp(remote_tc, num_remote_tc, mode, remote_pipe_fd, REMOTE);

	for(i = 0; i < num_remote_tc; i++)
	{
	    /*
	     *  If a mode fails for the TC on any system then subsequent
	     *  operations for that TC should not occur
	     */
	    if(remote_tc[i].ret_val != SUCCESS)
	    {
		ret_val = FAILURE;
		break;
	    }
	}

	clear_store(&remote_tc, num_remote_tc);

	num_remote_tc = 0;

	return(ret_val);
}


/*
 *  wrapper function for running remote test cases - testcases inside
 *  a remote directive with more than 1 remote system specified.
 */
int gsys_wrapper(testcase,systems,num_systems,mode,group,group_pipe_fd,override_eip)
char *testcase;
struct sysid_store *systems;
int num_systems;
int mode;
int group;
int *group_pipe_fd;
int override_eip;
{
	int ret_val;


	DBUG(DBGFDESC) (stderr,"gsys_wrapper(%s,systems:%ld,num_systems:%d,mode:%d,group:%d,group_pipe_fd:%ld,override_eip:%d)\n",testcase, (long) systems, num_systems, mode, group, (long) group_pipe_fd, override_eip);

	if(xresd_syncd_init(systems, num_systems) == FAILURE)
		return(FAILURE);

	make_gsyslist(systems, num_systems);

	if(num_systems > 1)
	{
		ret_val = rem_grp(testcase, systems, num_systems, mode, group, group_pipe_fd);
	}
	else
	{
		ret_val = rem_tc(testcase, systems, mode, group, group_pipe_fd, override_eip);
	}

	clean_gsyslist();

	return(ret_val);
}


/*
 *  logon to a system - run a TC in a specific mode - logoff a system
 */
int rem_tc(testcase, systems, mode, group, group_pipe_fd, override_eip)
char *testcase;
struct sysid_store *systems;
int mode;
int group;
int *group_pipe_fd;
int override_eip;
{
	int ret_val;
	int sysid_value;
	extern int tc_sysname();
	extern int tc_errno;
	extern char * ptrepcode();


	DBUG(DBGFDESC) (stderr,"rem_tc(%s,systems:%ld,mode:%d,group:%d,group_pipe_fd:%ld,override_eip:%d)\n",testcase, (long) systems, mode, group, (long) group_pipe_fd, override_eip);

	sysid_value = systems[0].sysid;

	/* logon onto the appropriate system */

	if(logonoff_sys(LOGON, sysid_value) != SUCCESS)
	{
		(void) sprintf(error_mesg, "Could not logon to remote system %3.3d.\n", systems[0].sysid);
		BAIL_OUT2(error_mesg);
	}

	gmysysid = sysid_value;

	/* pass system name list to the tccd on remote systems */
	if(sysid_value != MASTER)
	{
		if(tc_sysname(sysid_value, gsyslist, gnumsys) != SUCCESS)
		{
			(void) sprintf(error_mesg, "tc_sysname() failed for system %d, tc_errno = %d (%s)\n", sysid_value, tc_errno, ptrepcode(tc_errno));
			BAIL_OUT2(error_mesg);
		}
	}

	/*
	 *  This is where we actually call a function to run a testcase
	 *  in a given mode.
	 *  remote = TRUE.
	 */
	ret_val = run_tc(testcase, mode, TRUE, sysid_value, group, group_pipe_fd, override_eip);

	/* logoff the approriate system */

	if(logonoff_sys(LOGOFF, sysid_value) != SUCCESS)
	{
		(void)sprintf(error_mesg, "Could not logoff from remote system %3.3d.\n", sysid_value);
		ERROR(error_mesg);
	}

	gmysysid = -1;

	return(ret_val);
}


/*
 *  Run a testcase for a specific mode
 */
int run_tc(testcase, mode, remote, sysid_value, group, group_pipe_fd, override_eip)
char *testcase;
int mode;
int remote;
int sysid_value;
int group;
int *group_pipe_fd;
int override_eip;
{
	int ret_val = SUCCESS;

	DBUG(DBGFDESC) (stderr,"run_tc(%s,mode:%d,remote:%d,sysid_value;%d,group:%d,group_pipe_fd:%ld,override_eip:%d)\n",testcase, mode, remote, sysid_value, group, (long) group_pipe_fd, override_eip);

	switch(mode)
	{
		case BUILD:
			ret_val = bld_cln_tc(testcase, BUILD, remote, sysid_value, group, group_pipe_fd);
			break;

		case EXEC:
			ret_val = exec_tc(testcase, remote, sysid_value, group, group_pipe_fd, override_eip);
			break;

		case CLEAN:
			ret_val = bld_cln_tc(testcase, CLEAN, remote, sysid_value, group, group_pipe_fd);
			break;

		default:
			ret_val = FAILURE;
			(void) sprintf(error_mesg, "unexpected mode: %d\n", mode);
			ERROR(error_mesg);
			break;
	}

	return(ret_val);
}


/*
 *  Logon to system so that a testcase can be built, executed or cleaned
 *  The function is also called to logoff from the remote system
 *
 *  Note: no BAIL_OUTS are allowed in this function.
 */
int logonoff_sys(mode, sysid)
int mode;
int sysid;
{
    static int logged_sysid;
    static int logged_on = NOT_LOGGED_ON;
    static int logged_pid;
    int ret_val = SUCCESS;
    extern int tc_errno;
    extern char * ptrepcode();

    DBUG(DBGFDESC)(stderr, "logonoff_sys(mode:%s,sysid:%d)\n", (mode == LOGON)? "LOGON" : "LOGOFF", sysid);

    if(mode == LOGON)
    {
        if(sysid != MASTER)
	{
	    if(logged_on == LOGGED_ON)
	    {
		DBUG(DBGWARN)(stderr, "Warning: Not logged off from previous system %3.3d.\n", logged_sysid);
	    }
            ret_val = tc_logon(sysid);
	    if(ret_val == SUCCESS)
	    {
		logged_on = LOGGED_ON;
		logged_sysid = sysid;
		logged_pid = getpid();
	    }
	    else
	    {
		DBUG(DBGFINFO)(stderr, "Error in tc_logon. tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
	    }
	}
    }
    else
    {
	if(logged_on == LOGGED_ON)
	{
	    if(sysid == -1)
	    {
		if(logged_pid != getpid())
		{
		    DBUG(DBGWARN)(stderr, "Warning: Requested a logoff of system %3.3d by process %.5d. Logon was by process %.5d. Ignoring request.\n", getpid(), logged_pid);
		}
	        ret_val = tc_logoff(logged_sysid);
	    }
	    else
	    {
		if(sysid != logged_sysid)
		{
		    DBUG(DBGWARN)(stderr, "Warning: Requested a logoff of system %3.3d. Last logon was for system %3.3d.\n", sysid, logged_sysid);
		}
		ret_val = tc_logoff(sysid);
	    }

	    if(ret_val == SUCCESS)
	    {
	        logged_on = NOT_LOGGED_ON;
	    }
	    else
	    {
		DBUG(DBGFINFO)(stderr, "Error in tc_logoff. tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
	    }
	}
    }
    return(ret_val);
}
