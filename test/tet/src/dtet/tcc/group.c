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
static char sccsid[] = "@(#)group.c	1.10 (93/01/25) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)group.c    1.10 1/25/93
NAME:		group.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions to handle forking for the group directive.

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

#define CHILD   0
#define PARENT	1

int  signal_count = 0;

static int process_type;
static int tmp_jnl_fd;
static int num_processes;


/*
 *  build, execute and clean (where appropriate) a group of testcases
 */
void bec_group(group_tc, num_testcases, pipe_fd, directive_type)
struct tc_store *group_tc;
int num_testcases;
int *pipe_fd;
int directive_type;
{
    DBUG(DBGFDESC) (stderr,"bec_group(group_tc:%ld,num_tcs:%d,pipe_fd:%ld,dir_type:%d)\n", (long) group_tc, num_testcases, (long) pipe_fd, directive_type);

    if(ifmode(BUILD))
    {
	proc_tc_grp(group_tc, num_testcases, BUILD, pipe_fd, directive_type);

        /* Completed another operation - time to note the change of context */
        ++exec_ctr;
    }
    if(ifmode(EXEC))
    {
	proc_tc_grp(group_tc, num_testcases, EXEC, pipe_fd, directive_type);

        /* Completed another operation - time to note the change of context */
        ++exec_ctr;
    }
    if(ifmode(CLEAN))
    {
	proc_tc_grp(group_tc, num_testcases, CLEAN, pipe_fd, directive_type);

        /* Completed another operation - time to note the change of context */
        ++exec_ctr;
    }
}


/*
 *  Routine to process a group of testcases in a specific mode
 */
void proc_tc_grp(group_tc, num_testcases, mode, pipe_fd, directive_type)
struct tc_store *group_tc;
int num_testcases;
int mode;
int *pipe_fd;
int directive_type;
{
    int count;
    int ret_val;
    int old_scenario_line;
    int old_scenario_element;
    struct sigaction mysig;
    struct sigaction oldsig;

    DBUG(DBGFDESC) (stderr,"proc_tc_grp(group_tc:%ld,num_testcases:%d,mode:%d,pipe_fd:%ld,dir_type:%d)\n", (long) group_tc, num_testcases, mode, (long) pipe_fd, directive_type);

    /* SAFEGUARD */
    if( num_testcases == 0)
	return;

    old_scenario_element = scenario_element;
    old_scenario_line = scenario_line;

    num_processes = 0;

    if(pipe(pipe_fd) != 0)
    {
        (void) sprintf(error_mesg,"Failed to open pipe for parallel process communication.\n");
        BAIL_OUT2(error_mesg);
    }
    else
    {
	DBUG(DBGFILES)(stderr,"open() via pipe(). pipe[0] = %d, pipe[1] = %d\n", (int) pipe_fd[0], (int) pipe_fd[1]);
    }

    for(count = 0; count < num_testcases; count++)
    {
	/*
	 *  Don't look for duplicate TC's if this has been called from
	 *  rem_grp(). Each element of a remote group will be unique because
	 *  they each relate to different systems.
	 */
        if(directive_type != REMOTE)
        {
	    /*
	     *  this function should only be called once per group - so
	     *  establish which is the first mode of operation and only call
	     *  it for that mode.
	     */
	
	    if(ifmode(BUILD))
	    {
		if(mode == BUILD)
		{
		    /* look to see if TC has already occurred in the list */
		    prev_tc_name(group_tc, count);
		}
	    }
	    else if(ifmode(EXEC))
	    {
		if(mode == EXEC)
		{
		    /* look to see if TC has already occurred in the list */
		    prev_tc_name(group_tc, count);
		}
	    }
	    else if(ifmode(CLEAN))
	    {
		if(mode == CLEAN)
		{
		    /* look to see if TC has already occurred in the list */
		    prev_tc_name(group_tc, count);
		}
	    }
	}

    }

    for(count = 0; count < num_testcases; count++)
    {
	/* 
	 *  When other directives are nested inside the parallel directive
	 *  this fact is noted by setting the tc name equal to the nested
	 *  directive string. This enables us to emit appropriate directive
	 *  start and end messages in the correct place.
	 *  
	 *  At this stage we are looking for test cases to run so we ignore
	 *  any of these markers.
	 */
	if(*group_tc[count].tc_name == ':')
	{
	    DBUG(DBGFINFO) (stderr,"Ignoring \"%s\" - directive marker inside parallel list of TCs\n", group_tc[count].tc_name);

	    continue;
	}

	/* check to see whether this TC should be processed */
	if(group_tc[count].ignore == TRUE)
	{
	    DBUG(DBGFINFO) (stderr,"TC not to be processed.\n");

	    continue;
	}

	if(group_tc[count].ret_val != SUCCESS)
	{
	    /*
	     * A Build or an Execute of the Testcase has already failed, so
	     * we don't want to do anything more with this testcase.
	     */
	    DBUG(DBGFINFO) (stderr,"Ignoring \"%s\" - a previous mode of operation on this TC failed\n", group_tc[count].tc_name);

	    continue;
	}

	if(mode == BUILD || mode == CLEAN)
	{
	    /* only need to build or clean a TC once */
	    if(group_tc[count].duplicate == TRUE) 
	    {
	        DBUG(DBGFINFO) (stderr,"Ignoring duplicate entry for \"%s\"\n", group_tc[count].tc_name);

		continue;
	    }
	}

	/* OK - we are definately going to do something */
	DBUG(DBGFINFO) (stderr,"Processing \"%s\"\n", group_tc[count].tc_name);

	/* Create temp. journal file */
        tmp_jnl_fd = create_tmp_jnl(group_tc,count);

        mysig.sa_handler = signal_handler;
        mysig.sa_flags = 0;
        (void) sigaction(SIGUSR1,&mysig,&oldsig);

	if(group_fork(group_tc) != 0)
	{
	    /* Parent */

	    DBUG(DBGFILES)(stderr,"close(%d)\n", (int) tmp_jnl_fd);
	    (void) close(tmp_jnl_fd);
	}
	else
	{
	    /* Child */

	    DBUG(DBGFILES)(stderr,"close(%d) - pipe_fd[0]\n", (int) pipe_fd[0]);
	    (void) close(pipe_fd[0]);

	    /* Redirect journal output to the temp file */
	    set_group_jnl_fd();

	    /* Set the line and element number to those of the test case
	     * we are about to execute.
             */
	    scenario_line = group_tc[count].line_number;
	    scenario_element = group_tc[count].element_number;

	    /*
	     *  reset appropriate counter to show we are just a single thread.
	     *  these counters are checked inside signal handlers.
	     *  also call the approriate function to run a testcase.
	     */

	    switch(directive_type)
	    {
		case PARALLEL:
			num_parallel_tc = 0;

			/*
			 * call general function which determines whether
			 * we are a DTC, RTC or a normal TC
			 *  group = TRUE
			 */
	    		ret_val = proc_tc_dir(group_tc[count].tc_name, group_tc[count].sysid_store, group_tc[count].num_sysid, mode, TRUE, pipe_fd, group_tc[count].override_eip);

			break;

		case REMOTE:
			num_remote_tc = 0;

			/*
			 *  We are part of a DTC so we can call a more
			 *  specific function directly
			 *  group = TRUE
			 */
	    		ret_val = rem_tc(group_tc[count].tc_name, group_tc[count].sysid_store, mode, TRUE, pipe_fd, group_tc[count].override_eip);

			break;

		default:
			(void) sprintf(error_mesg,"unexpected directive type : %d\n", directive_type);
			ERROR(error_mesg);
			break;
	    }

	    DBUG(DBGFILES)(stderr,"close(%d)\n", (int) tmp_jnl_fd);
	    (void) close(tmp_jnl_fd);

	    exit(ret_val);
	}
    }

    /* Parent closes 'write' side of pipe. */
    DBUG(DBGFILES)(stderr,"close(%d) - pipe_fd[1]\n", (int) pipe_fd[1]);
    (void) close(pipe_fd[1]);

    /* Wait for message from children */
    parent_wait(group_tc, pipe_fd);

    /* Parent closes 'read' side of pipe. */
    DBUG(DBGFILES)(stderr,"close(%d) - pipe_fd[0]\n", (int) pipe_fd[0]);
    (void) close(pipe_fd[0]);

    /* Signal children to continue */
    signal_group(group_tc, num_testcases, SIGUSR1);

    /* Wait for children to exit. */
    wait_on_processes(group_tc, num_testcases);

    /* Merge all the temporary journal files from the child processes */
    merge_group_jnls(group_tc, num_testcases);

    /* Restore old signal action for SIGUSR1 and free memory. */
    (void) sigaction(SIGUSR1,&oldsig,(struct sigaction *)NULL);

    /* Reset scenario line and element numbers. */
    scenario_line = old_scenario_line;
    scenario_element = old_scenario_element;

    return;
}


/*
 *  Routine to send signal 'signal_ID' to all the processes stored in
 *  'group_tc[].process_ID'.
 */
void signal_group(group_tc, num_testcases, signal_ID)
struct tc_store *group_tc;
int num_testcases;
int signal_ID;
{
    int count;

    DBUG(DBGFDESC)(stderr, "signal_group(group_tc:%ld,num_testcases:%d,signal_ID:%d)\n", (long) group_tc, num_testcases, signal_ID);

    /* Send signals to start (or kill) all processes */
  
    for(count=0; count<num_testcases; count++)
    {
	if(group_tc[count].process_ID != 0)
	{

	    DBUG(DBGSIGNALS) (stderr,"Sending signal %d to process ID %d (process of testcase \"%s\" with %d remote designations).\n",signal_ID,group_tc[count].process_ID, group_tc[count].tc_name, group_tc[count].num_sysid);

	    if(kill(group_tc[count].process_ID,signal_ID)!=SUCCESS)
	    {
                (void) sprintf(error_mesg,"Kill of signal %d for process ID %d failed.\
n",signal_ID,group_tc[count].process_ID);
                ERROR(error_mesg);

	    }
	}
    }
}


/*
 *  Used by a child process to write a messgae down a pipe to the parent.
 *  The parent is supposed to wait until it has received a message from
 *  each child before signalling them to continue in parallel.
 */
void signal_parent(pipe_fd)
int *pipe_fd;
{
    char char_message;

    DBUG(DBGFDESC) (stderr,"signal_parent(pipe_fd:%ld)\n",(long) pipe_fd);

    char_message = (char)(num_processes+1);

    if(write(pipe_fd[1],(void *) &char_message,1) != 1)
    {
        (void) sprintf(error_mesg,"Child number %d failed to send message to parent.\n",num_processes+1);
	ERROR(error_mesg);
    }
    else
    {
	DBUG(DBGFINFO) (stderr,"Child send code %d to parent.\n", (int)char_message);
    }

    /* Close write part of pipe */
    DBUG(DBGFILES)(stderr,"close(%d) - pipe_fd[1]\n", (int) pipe_fd[1]);
    (void) close(pipe_fd[1]);
}


/*
 *  A call to this rountine will cause a fork to occur. The parent will return
 *  with the value of the child process ID after storing the value in
 *  group_tc[].process_ID.
 */
pid_t group_fork(group_tc)
struct tc_store *group_tc;
{
    pid_t childpid;

    DBUG(DBGFINFO) (stderr,"group_fork(group_tc:%ld)\n", (long) group_tc);

    signal_count = 0;

    childpid = dofork(); /* Fork */

    if(childpid == 0)
    {
	process_type = CHILD;
	mypid = (int) getpid();
    }
    else /* Parent stores process ID of the child and returns */
    {
	DBUG(DBGFINFO) (stderr,"Parent now has %d parallel processes running.\n",num_processes+1);
	group_tc[num_processes].process_ID = childpid;
	num_processes++;

	process_type = PARENT;
    }
    return(childpid);
}


/*
 *  A child process calls this function to wait for a signal from the
 *  parent. If this was the last child in a group it's possible that
 *  the signal has already been received.
 */
void child_wait()
{
    int count = 0;

    DBUG(DBGFDESC) (stderr,"child_wait()\n");

    DBUG(DBGFINFO) (stderr,"Group child number %d waiting for signal from parent.\n",num_processes+1);

    /* signal_count is set inside the signal handler */
    while( signal_count == 0 )
    {
	    (void) sleep(2);
	    if(++count == 30)
	    {
    		(void) sprintf(error_mesg,"Group child number %d timed out\n",num_processes+1);
		ERROR(error_mesg);
		exit(EXIT_BAD_CHILD);
	    }
    }

    DBUG(DBGFINFO) (stderr,"Group child number %d continuing.\n",num_processes+1);

}


/*
 *  The parent of a group of processes waits in this function until all
 *  the children have reached the same point.
 *  The parent reads a pipe that each of the children is connected to until
 *  each child has indicated it is ready
 */
void parent_wait(group_tc, pipe_fd)
struct tc_store *group_tc;
int *pipe_fd;
{
    char char_message;
    int messages = 0;
    char *messages_received;
    int count;
    int terminated = 0;


    DBUG(DBGFINFO) (stderr,"parent_wait(group_tc:%ld,pipe_fd:%ld)\n", (long) group_tc, (long) pipe_fd);

    messages_received = (char *) TET_MALLOC(sizeof(char)*num_processes);
    for(count = 0; count < num_processes; count++)
	messages_received[count] = (char) 0;

    while( messages < num_processes )
    {
	if(read(pipe_fd[0],(void *) &char_message,1) != 1)
	{
	    /* One or more of the child processes has terminated without
	     * signaling the parent.
	     * There is nothing we can do but continue so that at least the
	     * the journal files will be merged.
	     * Process ID's of terminated processes are reset so that they
	     * are ignored from now onwards.
	     */
	    for(count = 0; count < num_processes; count++)
	    {
		if(messages_received[count] == 0)
		{
		    group_tc[count].process_ID = 0;
		    terminated++;

	            DBUG(DBGFINFO) (stderr,"Child process number %d for testcase \"%s\" terminated before signaling parent.\n",count, group_tc[count].tc_name);

		}
	    }
	    /* Exit main loop */
	    break;
	}
	messages_received[char_message-1] = 1;
	messages++;


        DBUG(DBGFINFO) (stderr,"Group parent received message from child number %d.\n",(int)char_message);

    }

    num_processes -= terminated;
    TET_FREE(messages_received);


    DBUG(DBGFINFO) (stderr,"Parent for group execution continuing.\n");

}

/*
 *  Fork routine. The routine will re-try a failed fork up to five times
 *  if the reason for the fail is due to too many processes running.
 */
pid_t dofork()
{
    int   tries = 0;
    pid_t processid;
    int   err;

    DBUG(DBGFINFO) (stderr,"dofork()\n");

    do
    {
        processid = fork();

	err = errno;
	++tries;

	if(processid == FAILURE)
	{
	    if(errno != EAGAIN)
	    {
		(void) sprintf(error_mesg, "fork() failed. errno : %d\n", err);
		FATAL_ERROR(error_mesg);
	    }

	    if(tries < 5)
	    {
                DBUG(DBGFINFO) (stderr,"fork() failed because no processes are available\n");
        	(void) sleep(5);
                DBUG(DBGFINFO) (stderr,"Trying fork() again\n");
	    }
	}
	else
	    break;
    }
    while(tries < 5);

    return(processid);
}


/*
 *  Wait until all the processes in 'group_tc[].process_ID' have
 *  finished and terminated.
 */
void wait_on_processes(group_tc, num_testcases)
struct tc_store *group_tc;
int num_testcases;
{
    pid_t xpid;
    int count;
    int count2;
    int stat_val;
    int ret_sig;


    DBUG(DBGFDESC) (stderr,"wait_on_processes(group_tc:%ld,num_testcases:%d)\n", (long) group_tc, num_testcases);

    while(num_processes > 0)
    {
	/* wait for a child to terminate */
	xpid = wait(&stat_val);

	for(count = 0; count < num_testcases; count++)
	{
	    if(group_tc[count].process_ID == xpid)
	    {
		num_processes--;
		group_tc[count].process_ID = 0;

		if(WIFEXITED(stat_val))
		{
		    /* hopefully WEXITSTATUS(stat_val) == SUCCESS */

		    group_tc[count].ret_val = WEXITSTATUS(stat_val);
		    DBUG(DBGFINFO) (stderr,"Parallel process %d has exited with a return value of %d.\n",(int)xpid, group_tc[count].ret_val);
		}
		else if(WIFSIGNALED(stat_val))
		{
		    group_tc[count].ret_val = FAILURE;
		    ret_sig = WTERMSIG(stat_val);
		    DBUG(DBGFINFO) (stderr,"Parallel process %d has terminated due to signal %d.\n",(int)xpid, ret_sig);
		}
		else if(WIFSTOPPED(stat_val))
		{
		    group_tc[count].ret_val = FAILURE;
		    DBUG(DBGFINFO) (stderr,"Parallel process %d has stopped due to signal %d.\n",(int)xpid, WSTOPSIG(stat_val));
		}
		else
		{
		    group_tc[count].ret_val = FAILURE;
		    DBUG(DBGFINFO) (stderr,"Parallel process %d has terminated abnormally.\n",(int)xpid);
		}
		for(count2 = 0; count2 < num_testcases; count2++)
		{
		    if(group_tc[count2].process_ID != 0)
		    {
		        DBUG(DBGFINFO) (stderr,"Parallel process %d still running.\n",group_tc[count2].process_ID);
		    }
		}
	        break;
	    }
	}
    }
}


/*
 *  Redirect journal output to a temporary file
 */
void set_group_jnl_fd()
{

    DBUG(DBGFINFO) (stderr,"set_group_jnl_fd()\n");

    jnl_fd = tmp_jnl_fd;
}


/*
 *  Function to create and open a temporary journal file for a member
 *  of a group of testcases.
 */
int create_tmp_jnl(group_tc, tc_num)
struct tc_store *group_tc;
int tc_num;
{
    int file_desc;
    char *temp_dir;
    char letter = 'a';
    char *name_end;


    DBUG(DBGFDESC) (stderr,"create_tmp_jnl(group_tc:%ld,tc_num:%d)\n", (long) group_tc, tc_num);

    temp_dir = get_temp_dir(FALSE, MASTER);

    group_tc[tc_num].tmp_jnl_name = (char *)TET_MALLOC(strlen(temp_dir)+10);    
    (void) sprintf(group_tc[tc_num].tmp_jnl_name,"%s/jnl_%3.3d ",
		       temp_dir,tc_num);

    name_end = group_tc[tc_num].tmp_jnl_name + strlen(group_tc[tc_num].tmp_jnl_name) - 1;


    do
    {
	*name_end = letter++;
        file_desc = open(group_tc[tc_num].tmp_jnl_name,(O_WRONLY|O_CREAT|O_EXCL),(S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH));
    }
    while((file_desc == FAILURE) && (errno == EEXIST) && (letter <= 'z'));

    if(file_desc == FAILURE)
    {
	perror("open");
	(void) sprintf(error_mesg,"Error opening temporary journal file \"%s\".\n",group_tc[tc_num].tmp_jnl_name);
	BAIL_OUT2(error_mesg);
    } 
    else
    {
	DBUG(DBGFILES)(stderr,"open(%d)\n", (int) file_desc);
        DBUG(DBGFINFO) (stderr,"Created temporary journal file \"%s\".\n",group_tc[tc_num].tmp_jnl_name);
    }
    
    return(file_desc);
}
   

/*
 *  Merge all the temporary journal files created by each member of a group
 *  of tetscases.
 */
int merge_group_jnls(group_tc, num_testcases)
struct tc_store *group_tc;
int num_testcases;
{
    char buf[JNL_LINE_MAX + 1];
    int count;
    struct sysid_store *systems;
    FILE *tmp_fdesc;
    int err;
    int line_type;
    int filter;
    int emit_dir_markers = FALSE;


    DBUG(DBGFDESC) (stderr,"merge_group_jnls(group_tc:%ld,num_testcases:%d)\n", (long) group_tc, num_testcases);

    /*
     *  This function is called if we are in a parallel directive or in a
     *  remote directive that involves more than one system
     */

    /*
     *  If this is a distributed test case we only need the journal file
     *  generated for the master system
     */

    /*
     *  Establish whther or not the directive markers should be emitted.
     *  they may not be wanted because of the use of the -y and -n options
     */
    for(count = 0; count < num_testcases; count++) 
    {
        if(group_tc[count].ignore == FALSE)
	{
    	    emit_dir_markers = TRUE;
	}
    }

    for(count = 0; count < num_testcases; count++) 
    {
	filter = FALSE;

	/* 
	 *  When other directives are nested inside the parallel directive
	 *  this fact is noted by setting the tc name equal to the nested
	 *  directive string. This enables us to emit appropriate directive
	 *  start and end messages in the correct place.
	 */
        if(*group_tc[count].tc_name == ':' && emit_dir_markers == TRUE)
        {
	    DBUG(DBGFINFO) (stderr,"printing directive marker \"%s\" inside parallel list of TCs\n", group_tc[count].tc_name);

	    write_directive(&(group_tc[count]));

	    continue;
	}

	if(group_tc[count].tmp_jnl_name != NULL)
	{
	    if(group_tc[count].num_sysid > 1)
	    {
		/*
		 *  Part of a DTC - a remote directive involving 
		 *  more than 1 system
		 */
		systems = group_tc[count].sysid_store;

		/*
		 *  If this is not the master part of a DTC then we only
		 *  want error messages - filter everything else out
		 */
		if( systems[0].sysid != 0)
		{
		    filter = TRUE;
		}
	    }

            if((tmp_fdesc = fopen(group_tc[count].tmp_jnl_name, "r")) == (FILE *) NULL)
            {
	        perror("fopen");
	        (void) sprintf(error_mesg,"Error opening temporary journal file \"%s\" for merging.\n",group_tc[count].tmp_jnl_name);
	        BAIL_OUT2(error_mesg);
	    }
	    else
	    {
		DBUG(DBGFILES)(stderr,"fopen(%ld)\n", (long) tmp_fdesc);
	    }

	    while(fgets(buf, sizeof(buf), tmp_fdesc) != NULL)
	    {
		(void) sscanf(buf,"%d", &line_type);

		if(filter == TRUE)
		{
		    /* Only want error messages from remote sys journals */
		    if(line_type != TET_JNL_TCM_INFO && 
						line_type != TET_JNL_TC_MESSAGE)
		    {
			continue;
		    }
		}

	        (void) write(jnl_fd,(void *)buf, (size_t) strlen(buf));

	    }

	    err = errno;

	    if(feof(tmp_fdesc) == 0)
	    {
	        (void) sprintf(error_mesg,"fgets() failed while merging temp journal file\n",err);
	        ERROR(error_mesg);
	    }

	    DBUG(DBGFILES)(stderr,"fclose(%ld)\n", (long) tmp_fdesc);
	    (void) fclose(tmp_fdesc);

	    (void) unlink(group_tc[count].tmp_jnl_name);
	    TET_FREE(group_tc[count].tmp_jnl_name);
	    group_tc[count].tmp_jnl_name = NULL;
	}
    } 
}


/*
 *  This is the signal handler that a child process, which is a member
 *  of a group of testcases, jumps to when the parent siganls each of the
 *  child processes to continue.
 */
void signal_handler()
{
    DBUG(DBGSIGNALS)(stderr,"signal_handler()\n");

    /* this varaible is checked by the child to see if it's OK to continue */
    signal_count = 1;
}


/*
 *  When you have testcases running in parallel it is only necessary to 
 *  build or clean the TC once. This function checks for duplicate TCs
 *  entries in a list of TCs.
 *
 *  for matches at a group level set group_tc.duplicate
 *  for matches at a system level set group_tc.sysid_store.duplicate
 */
void prev_tc_name(group_tc, num)
struct tc_store *group_tc;
int num;
{
    int count;
    int i, j;
    char *tmp_tc_name;
    char *tc_name;
    char *ptr;

#define	QueriedTC	group_tc[num]
#define	PreviousTC      group_tc[count]

    DBUG(DBGFDESC) (stderr,"prev_tc_name(group_tc:%ld,num:%d) \"%s\"\n", (long) group_tc, num, QueriedTC.tc_name);

    /*
     *  We are only interested in the TC name and path, we
     *  can ignore any IC list.
     */
    tc_name = store_str(QueriedTC.tc_name);

    if((ptr = strchr(tc_name,IC_START_MARK)) != NULL)
	*ptr = '\0';

    /*
     *  Sequentially work through the list of testcases in 'group_tc'
     *  up to the element specified by 'num'.
     *  For each entry check to see if it matches the entry specified by 'num'
     */
    for(count = 0; count < num; count++)
    {
	/* get the next testcase string out of the list */
	tmp_tc_name = store_str(PreviousTC.tc_name);

	/* remove any trailing ICs from the TC string */
	if((ptr = strchr(tmp_tc_name, IC_START_MARK)) != NULL)
	    *ptr = '\0';

	if(strcmp(tmp_tc_name, tc_name) == 0)
	{
	    /* If TC names match */

	    if(QueriedTC.num_sysid > 0) 
	    {
		/* TC being queried is a remote - it uses DAPI */

		if(PreviousTC.num_sysid > 0)
		{
		    /* previous TC that matches is a remote - it uses DAPI */

		    /*
		     *  Now we get really specific and look for matches
		     *  on a per system basis.
		     *
		     *  For each system in the specified PreviousTC check
		     *  to see if there is a matching system entry for the
		     *  QueriedTC.
		     */
		    for(i = 0; i < PreviousTC.num_sysid; i++)
		    {
		        for(j = 0; j < QueriedTC.num_sysid; j++)
			{
			    if((PreviousTC.sysid_store)[i].sysid == (QueriedTC.sysid_store)[j].sysid)
			    {
	        	        DBUG(DBGFINFO) (stderr,"duplicate entry for %s on system %3.3d\n", tc_name, (QueriedTC.sysid_store)[j].sysid);   


			        (QueriedTC.sysid_store)[j].duplicate = TRUE;

			        (PreviousTC.sysid_store)[i].override_eip = TRUE;
			        (QueriedTC.sysid_store)[j].override_eip = TRUE;
			    }
			}
		    }
		}
		else
		{
		    /* previous TC that matches is not a remote - it uses API */

		    /*
		     *  a normal TC on the MASTER system uses the API,
		     *  a DTC on the MASTER system uses the DAPI,
		     *  so if we find a match here it is an error
		     */
		    for(j = 0; j < QueriedTC.num_sysid; j++)
		    {

		        if((QueriedTC.sysid_store)[j].sysid == MASTER)
			{
			    (void) sprintf(error_mesg, "TC \"%s\" specified with & without remote directive\n", tc_name);
			    ERROR(error_mesg);
			    break;
			}
		    }
		}
	    }
	    else
	    {
		/* TC being queried is not a remote - it uses API */

		if(PreviousTC.num_sysid > 0)
		{
		    /* previous TC that matches is a remote - it uses DAPI */

		    /*
		     *  a normal TC on the MASTER system uses the API,
		     *  a DTC on the MASTER system uses the DAPI,
		     *  so if we find a match here it is an error
		     */
		    for(i = 0; i < PreviousTC.num_sysid; i++)
		    {
		        if((PreviousTC.sysid_store)[i].sysid == MASTER)
			{
			    (void) sprintf(error_mesg, "TC \"%s\" specified with & without remote directive\n", tc_name);
			    ERROR(error_mesg);
			    break;
			}
		     }
		}
		else
		{
		    /* previous TC that matches is not a remote - it uses API */

		    /* found a match at the group level */

	            DBUG(DBGFINFO) (stderr,"TC \"%s\" previously found.\n",tc_name);   

		    QueriedTC.duplicate = TRUE;

		    PreviousTC.override_eip = TRUE;
		    QueriedTC.override_eip = TRUE;

	            TET_FREE(tmp_tc_name);
	            TET_FREE(tc_name);
	            return;
		}
	    }
	}

	TET_FREE(tmp_tc_name);
    }

    TET_FREE(tc_name);

#undef	QueriedTC
#undef	PreviousTC

    return;
}


/*
 *  write a directive line whilst merging journal files
 */
void write_directive(tc)
struct tc_store *tc;
{
    char *string;
    int tmp_line;
    int tmp_element;

    DBUG(DBGFDESC)(stderr, "write_directive(tc:%ld)\n", (long) tc);

    /* save scenario line and reference number */
    tmp_line = scenario_line;
    tmp_element = scenario_element;

    /*
     *  Set the line and element number to those of the test case
     *  we are about to execute.
     */
    scenario_line = tc[0].line_number;
    scenario_element = tc[0].element_number;

    /* match directive */
    if(strncmp(tc[0].tc_name, REMOTE_STR, strlen(REMOTE_STR)) == 0)
    {
	string = tc[0].tc_name + strlen(REMOTE_STR);
	jnl_remote_start(string);
    }
    else if(strncmp(tc[0].tc_name, END_REMOTE_STR, strlen(END_REMOTE_STR)) == 0)
    {
        jnl_remote_end();
    }
    else if(strncmp(tc[0].tc_name, PARALLEL_STR, strlen(PARALLEL_STR)) == 0)
    {
        jnl_para_start();
    }
    else if(strncmp(tc[0].tc_name, END_PARALLEL_STR, strlen(END_PARALLEL_STR)) == 0)
    {
        jnl_para_end();
    }
    else
    {
	(void) sprintf(error_mesg, "unexpected directive marker string \"%s\"\n", tc[0].tc_name);
	ERROR(error_mesg);
    }

    /* reset scenario line and reference number */

    scenario_line = tmp_line;
    scenario_element = tmp_element;
}
