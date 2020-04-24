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
static char sccsid[] = "@(#)scenario.c	1.6 (93/01/25) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)scenario.c    1.6 1/25/93
NAME:		scenario.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions relating to the scenario file parsing

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


int  rewound = FALSE;	 /* Have we just done a rewind due to an endrepeat */
FILE *curr_fdesc;	 /* Current file descriptor */

char orig_scen_line[INPUT_LINE_MAX]; /* Copy of scenario line before parsing */

static int remote = FALSE;
static int group = FALSE;
static int parallel_pipe_fd[2];
static int num_sysid = 0;
static struct sysid_store *tc_sysid = NULL;

struct tc_store *parallel_tc = NULL;
int num_parallel_tc;

extern char *g_tc_line;


/* Repeat directive record table */
struct rpt_table
{
	int	ctr;
	FILE *	file;
	long	fptr;
	int	line;
	int	element;
} *repeat_table;



/* 
 *  process an scen from the scenario  file.
 *  the parameter 'scenario_name' is the value given
 *  on the tcc command line.
 */
int perform_scen(resume_status)
int resume_status;
{
    FILE *scen_fp;
    char *str1, *str2, *proc_ptr, *newline;
    int type, ctr;
    bool found_it =  FALSE;
    char linein[INPUT_LINE_MAX];


    DBUG(DBGFDESC) (stderr,"perform_scen() - looking for scenario \"%s\"\n",scenario_name);


    /* Open the scenario file */
    if ((scen_fp = fopen(scen_file_name,"r")) == NULL)
    {
        (void) sprintf(error_mesg,"can't open scenario file : %s\n", scen_file_name);
        perror("fopen");
        BAIL_OUT2(error_mesg);
    }
    else
    {
	DBUG(DBGFILES)(stderr,"fopen(%ld)\n", (long) scen_fp);

	curr_fdesc = scen_fp;
    }

    /* make sure the file descriptor is closed on an exec call  */
    DBUG(DBGFILES)(stderr,"fcntl(%d, F_SETFD, FD_CLOEXEC)\n", (int) fileno(scen_fp));
    (void) fcntl(fileno(scen_fp), F_SETFD, FD_CLOEXEC);

    /* Allocate memory for repeat directive table */
    repeat_table = (struct rpt_table *) TET_MALLOC((size_t)(10*sizeof(struct rpt_table)));
    for(ctr = 0; ctr < 10; ctr++)
	repeat_table[ctr].ctr = -1;

    /* search for the scen name */
    while ( ((fgets(linein,sizeof(linein),curr_fdesc)) != NULL)  && (abort_requested() != TRUE))
    {
	/* Check for a line that is too big */
	if(strlen(linein) == (INPUT_LINE_MAX - 1))
	{
            (void) sprintf(error_mesg,"File %s. Line too long ! line begins \"%s\"\n", scen_file_name,linein);
            BAIL_OUT(error_mesg);
	}

        /* change newline to '\0' for string operations */
        if (linein[strlen(linein)-1] == '\n')
            linein[strlen(linein)-1] = '\0';

	/* Get a copy of the scenario line for error messages etc */
	(void) strcpy(orig_scen_line, linein);

	if (rewound == FALSE)
	{
	    ++scenario_line;
    	    scenario_element = 0;
	}

	proc_ptr = linein;

        if( found_it == TRUE)
        {
	    if (rewound == TRUE) /* After encountering an endrepeat */
	    {
		rewound = FALSE;
	    }
	    else
	    {
                /* Check for end of scenario */
                if( (! isspace(*proc_ptr)) && (*proc_ptr != COMMENT_CHAR))
                    break;
	    }
        }

        if(found_it != TRUE)
        {
            str1 = (char *) TET_MALLOC((size_t)( strlen(linein) + 1));
            (void) strcpy(str1, linein);

            /* get the first token */
            str2 = strtok(str1," \t");
            if( str2 == (char *) NULL)
	    {
		TET_FREE((void *) str1);
                continue;
	    }

            /* is it the scenario of interest */
            if (strcmp(scenario_name,str2) == 0)  
            {
                found_it = TRUE;    /* set the flag */
	    	proc_ptr = &(linein[strlen(str2) + 1]);
	        newline = proc_ptr;
	        do
	        {
	            type = analyse_line(newline, &newline);
                    newline = process_line(newline, type, resume_status);
	        }
    	        while(newline != NULL);
		TET_FREE((void *) str1);
            }
            else
	    {
		TET_FREE((void *) str1);
                continue;
	    }
        }
        else
        {
	    newline = proc_ptr;
	    do
	    {
	        type = analyse_line(newline, &newline);
                newline = process_line(newline, type, resume_status);
	    }
    	    while(newline != NULL);
        }

	if (resume_status == -1) /* Error has occurred */
	    break; /* break out of file scanning loop */
    }

    DBUG(DBGFILES)(stderr,"fclose(%ld)\n", (long) scen_fp);
    (void) fclose(scen_fp);

    if (found_it == FALSE)
    {
        (void) sprintf(error_mesg,"requested scenario \"%s\" not found in file %s\n", scenario_name, scen_file_name);
        BAIL_OUT2(error_mesg);
    }

    TET_FREE((void *) repeat_table);

    if (resume_status != 0)
	return(FAILURE);

    return(SUCCESS);
}


/*
 *  Analyse a line from the 'scenario' file - returns its type.
 */
int analyse_line(aline, newline)
char *  aline;
char ** newline;
{
    int type;

    if (aline[strlen(aline)-1] == '\n')
        aline[strlen(aline)-1]='\0';

    DBUG(DBGFDESC) (stderr,"analyse_line(\"%s\", **newline)\n", aline);


    /* find First Non Blank Character */
    for (;((isspace(*aline)) && (*aline != '\0') ); aline++);

    switch((int) *aline)
    {
    case '\0':
        *newline = NULL;
        type = EMPTY;
        break;

    case MESSAGE_CHAR: /* A message to be included in the journal */
	*newline = aline;
	type = MESSAGE;
        break;

    case COMMENT_CHAR:
	*newline = NULL;
	type = COMMENT;
        break;

    case TC_START_MARK: /* An invocable component */
	*newline = aline;
	type = TESTCASE;
        break;

    case DIRECTIVE_LIMITER: /* An include, repeat, group or remote */
        if (strncmp(INCLUDE_STR,aline,strlen(INCLUDE_STR)) == 0)
        {
            *newline = aline;
	    type = INCLUDE;
        }
        else if (strncmp(REPEAT_STR,aline,strlen(REPEAT_STR)) == 0)
	{
	    *newline = aline;
	    type = REPEAT;
	}
        else if (strncmp(END_REPEAT_STR,aline,strlen(END_REPEAT_STR)) == 0)
	{
	    *newline = aline;
	    type = ENDREPEAT;
	}
	else if (strncmp(PARALLEL_STR,aline,strlen(PARALLEL_STR)) == 0)
	{
	    *newline = aline;
	    type = PARALLEL;
	}
	else if (strncmp(END_PARALLEL_STR,aline,strlen(END_PARALLEL_STR)) == 0)
	{
	    *newline = aline;
	    type = ENDPARALLEL;
	}
	else if (strncmp(REMOTE_STR,aline,strlen(REMOTE_STR)) == 0)
	{
	    *newline = aline;
	    type = REMOTE;
	}
	else if (strncmp(END_REMOTE_STR,aline,strlen(END_REMOTE_STR)) == 0)
	{
	    *newline = aline;
	    type = ENDREMOTE;
	}
	else
	{
	    *newline = NULL;
	    type = FAILURE;
	}
        break;

    default:
        *newline = NULL;
        type = FAILURE;
        break;
    }

    return(type);
}


/*
 * Process a line from the 'scenario' file 
 */
char *process_line(aline,type,resume_status)
char *aline;
int  type;
int  resume_status;
{
    char *newptr, *tmp_ptr, *cp;
    int *ics, ctr, repeat_ctr = 0, num;
    int prev_line, prev_element;
    char runline[INPUT_LINE_MAX], r_char;
    char *string;
  

    DBUG(DBGFDESC) (stderr,"process_line(\"%s\",type: %d,resume_status: %d)\n", aline, type, resume_status);


    ++scenario_element;

    switch(type)
    {
    case EMPTY:
    case COMMENT:

        newptr = NULL;
        break;

    case MESSAGE: /* A message to be included in the journal */

        for( tmp_ptr = (aline+1); *tmp_ptr != MESSAGE_CHAR; tmp_ptr++)
        {
            if( *tmp_ptr == '\0')
            {
                (void) sprintf(error_mesg,"Scenario file comment line has unmatched quote : %s\n", orig_scen_line);
                BAIL_OUT2(error_mesg);
            }
        }
        ++tmp_ptr;
        if(tmp_ptr == '\0')
        {
            (void) jnl_entry_scen(aline);
	    newptr = NULL;
        }
        else
        {
            *tmp_ptr = '\0';
            (void) jnl_entry_scen(aline);
            newptr = ++tmp_ptr;
        }
        break;

    case TESTCASE: /* An invocable component */

        for( tmp_ptr = aline;(*tmp_ptr != '\0')&&(!isspace(*tmp_ptr));
                                   tmp_ptr++);
        if(*tmp_ptr != '\0')
        {
            *tmp_ptr = '\0';
            newptr = ++tmp_ptr;
        }
	else
	    newptr = NULL;

	(void) strcpy(runline, aline);


	if (resume_status == 1) /* resume mode active */
	{
	    if( (scenario_element == old_element)
		&&
		(scenario_line == old_line) )
	    {
		cp = strchr(runline,IC_START_MARK);
		if (cp != NULL)
		    *cp = '\0';

		/* Check TC names match */
		if (strcmp(runline, g_tc_line))
		{
		    (void) sprintf(error_mesg, "Expected \"%s\" to be the %d%s line of the scenario file, found \"%s\" instead.\n", g_tc_line, old_line, number_suffix(old_line), runline);
		    ERROR(error_mesg);
		    resume_status = -1;
		    newptr = tmp_ptr;
		    TET_FREE((void *) g_tc_line);
		    break; 			/* break out of switch */
		}
		else
		    TET_FREE((void *) g_tc_line);

		if(group == TRUE)
		{
		    /* We have found a test case to resume within a group.
		     * We must rerun the whole group and all the IC's of the
		     * test cases.
		     */
		     (void) fprintf(stderr,"Parallel TESTCASE found to resume.\n");
		}
		else /* Find the IC to resume from */
		{
	   	    if(cp != NULL)
		    {
    		        ics = (int *) TET_MALLOC((size_t) (sizeof(int) * 50));
    		        ics[0] = -2;
		        make_ic_list(cp, &ics);
		        for(ctr = 0; ics[ctr] != -2; ctr++)
		        {
		            if (ics[ctr] == g_ic_num)
			        break;
		        }
		        if (ics[ctr] == -2) /* didn't find IC */
		        {
		            (void) sprintf(error_mesg,"Expected to find IC %d on line %d of the scenario file, line begins \"%s\".\n", g_ic_num, old_line, orig_scen_line);
		            ERROR(error_mesg);
		            newptr = tmp_ptr;
		            break; /* break out of switch */
		        }
		        else
		        {
		            /* build up list of remaining ICs in list */

		            /* should be careful about line length, but */
		            (void) sprintf(runline,"%s{%d",runline,ics[ctr]);
		            for (++ctr; ics[ctr] != -2; ctr++)
		                (void) sprintf(runline,"%s,%d", runline, ics[ctr]);
		            (void) sprintf(runline,"%s}",runline);
		        }
		        TET_FREE((void *) ics);
		    }
		    else
		    {
		        if (g_ic_num != -1)
		        {
		            (void)sprintf(runline, "%s{%d}", runline, g_ic_num);
		            exec_all_flag = TRUE;
		        }

		        /* TC line with no ICs specified */
		    }
		}
	    }
	    else
	    {
		newptr = tmp_ptr;
		break;             /* break out of switch */
	    }

	    resume_status = 0; /* Resume option no longer active */
        }

        /*
         * We have found a test case that is to be built and/or
         * cleaned and/or executed; ensure the right environment is being
         * used for each mode.
         */

        newptr = tmp_ptr;

	/*
	 * If we are within a group directive we want to store the test case.
	 * All the stored test cases within the group are only run once the
         * ':endparallel:' has been found.
	 */
	if(group == TRUE)
	{
	    num_parallel_tc = store_tc(&parallel_tc, runline, tc_sysid, num_sysid, scenario_line, scenario_element, num_parallel_tc);
	    break;
	}

	/*
	 * This point is not reached if we are within a parallel execution
	 * directive. In such a case, the testcases are run by the
	 * 'bec_group' rountine.
	 */

	bec_tc(runline, tc_sysid, num_sysid);

        break;

        case INCLUDE: /* An include directive */

	    /* position line pointer just after the directive :include: */
            aline += strlen(INCLUDE_STR);

	    if (*aline != TC_START_MARK)
	    {
		(void) sprintf(error_mesg, "Format error in scenario file line %d, element %d; expected %s/<test case path>/<test case name> found %s\n", scenario_line, scenario_element, INCLUDE_STR, orig_scen_line);
		ERROR(error_mesg);
		tmp_ptr = NULL;
	    }
	    else
	    {
	        /* process the inclusion file */
	        inclusion_file(aline, &tmp_ptr, resume_status);
	    }
	    
            newptr = tmp_ptr;
	    break;

	case REPEAT:

	    /* position line pointer just after the directive :repeat, */
            aline += strlen(REPEAT_STR);

	    /* get the number of repeats */
	    num = sscanf(aline,"%d%c", &repeat_ctr, &r_char);
	    if(num != 2 || repeat_ctr <= 0 || r_char != DIRECTIVE_LIMITER)
	    {
		(void) sprintf(error_mesg, "Format error in scenario file line %d, element %d; expected %s/<num>: found %s\n", scenario_line, scenario_element, REPEAT_STR, orig_scen_line);
		ERROR(error_mesg);
		tmp_ptr = NULL;
	    }
	    else
	    {
		jnl_repeat_start(repeat_ctr);
		aline = strchr(aline, DIRECTIVE_LIMITER);
		++aline;
		tmp_ptr = aline;

	        if (*aline != TC_START_MARK)
	        {
		    for(ctr = 0; repeat_table[ctr].ctr != -1; ctr++);
		    if(ctr%10 == 9)
		    {
			repeat_table = (struct rpt_table *) TET_REALLOC(repeat_table, (size_t) ((ctr+10) * sizeof(struct rpt_table)));
			for(num = ctr+1; num < (num + 10); num++)
			    repeat_table[num].ctr = -1;
		    }

		    repeat_table[ctr].ctr = repeat_ctr;
		    repeat_table[ctr].file = curr_fdesc;

		    repeat_table[ctr].fptr = (long) (ftell(curr_fdesc)-strlen(aline)-1);
		    repeat_table[ctr].line = scenario_line;
		    repeat_table[ctr].element = scenario_element;

		    DBUG(DBGFINFO) (stderr,"repeat table: entry %d; ctr %d, ptr %ld\n", ctr, repeat_table[ctr].ctr,  repeat_table[ctr].fptr);

	        }
	    	else
	    	{
		    /* save out current position */
		    prev_line = scenario_line;
		    prev_element = scenario_element;

	            /* process the inclusion file */
	            while ( repeat_ctr-- != 0)	
		    {

		        DBUG(DBGFINFO) (stderr,"parsing repeat inclusion file. repeat counter = %d\n", repeat_ctr);

			scenario_line = prev_line;
			scenario_element = prev_element;
	                inclusion_file(aline, &tmp_ptr, resume_status);

		        DBUG(DBGFINFO) (stderr,"finished parsing repeat inclusion file.\n");

		    }
		    scenario_line = prev_line;
		    scenario_element = prev_element;
	    	}
	    }
            newptr = tmp_ptr;

	    DBUG(DBGFINFO) (stderr,"finished repeat directive.\n");
	    break;

	case ENDREPEAT:

            aline += strlen(END_REPEAT_STR);

	    /* Position at last entry in table */
	    for(ctr = 0; repeat_table[ctr].ctr != -1; ctr++);
	    --ctr;

	    /* Decrement repeat counter */
	    --(repeat_table[ctr].ctr);

	    DBUG(DBGFINFO) (stderr,"endrepeat encountered %d repeat cycles left\n", repeat_table[ctr].ctr);

	    if(repeat_table[ctr].ctr == 0)
	    {
		repeat_table[ctr].ctr = -1;
	        jnl_repeat_end();
	        tmp_ptr = aline;
	    }
	    else
	    {
		if(curr_fdesc != repeat_table[ctr].file)
		{
		    (void) sprintf(error_mesg,"endrepeat directive does not come in same file as matching repeat directive\n");
		    FATAL_ERROR(error_mesg);
		}
		else
		{
		    curr_fdesc = repeat_table[ctr].file;
		    (void) fseek(curr_fdesc, repeat_table[ctr].fptr, SEEK_SET);
		    scenario_line = repeat_table[ctr].line;
		    scenario_element = repeat_table[ctr].element;

		    DBUG(DBGFINFO) (stderr,"endrepeat directive. Rewinding to: file position %ld, line %d, element %d\n", ftell(curr_fdesc), scenario_line, scenario_element);

		    rewound = TRUE;
		}

		tmp_ptr = NULL;
	    }

            newptr = tmp_ptr;
	    break;

	case REMOTE:

            aline += strlen(REMOTE_STR);
	    for( tmp_ptr = aline;(!isspace(*tmp_ptr))
				  &&
				 (*tmp_ptr != '\0')
				  &&
				 (*tmp_ptr != DIRECTIVE_LIMITER); tmp_ptr++)
		;
	    if(*tmp_ptr != DIRECTIVE_LIMITER)
	    {
		(void) sprintf(error_mesg, "Format error in scenario file line %d, element %d;\n expected %s<remote designations>:/<scenario file path>/<scenario file>\n OR        %s<remote designations:\n found %s\n", scenario_line, scenario_element, REMOTE_STR, orig_scen_line);
		FATAL_ERROR(error_mesg);
	    }
	    else
	    {
		*tmp_ptr = '\0';
		tmp_ptr++;
	    }

	    /* aline is pointing at the remote designations */
	    num_sysid = str_to_sysids(&tc_sysid,aline,num_sysid);

	    if(group == FALSE)
	    {
	        jnl_remote_start(aline);
	    }
	    else
	    {
		string = combine_str(REMOTE_STR, aline);
		
	        num_parallel_tc = store_tc(&parallel_tc, string, tc_sysid, num_sysid, scenario_line, scenario_element, num_parallel_tc);

		TET_FREE((void *)string);
	    }

	    aline = tmp_ptr; /* Character after DIRECTIVE_LIMITER */
	    remote = TRUE;

	    if (*tmp_ptr != TC_START_MARK)
	    {
		if(!isspace(*tmp_ptr) && (*tmp_ptr != '\0'))
		{
		    (void) sprintf(error_mesg, "Format error in scenario file line %d, element %d;  expected %s<remote designations>:/<scenario file path>/<scenario file> OR %s<remote designations>: . Found %s\n", scenario_line, scenario_element, REMOTE_STR,REMOTE_STR, orig_scen_line);
		    FATAL_ERROR(error_mesg);
		    tmp_ptr = NULL;
		}	    
		else
		    newptr = tmp_ptr;
	    }
	    else
	    {
		/* Scenario file follows remote directive   
		   eg. :remote,000,001:/tests/scen_file
		*/

		DBUG(DBGFINFO) (stderr,"Remote file mode.\n");

		/* Include scenario file */
		inclusion_file(aline,&tmp_ptr, resume_status);
	        if(group == FALSE)
	        {
		    /* If not in group we want to clear the store of remote
		     * designations. (If in a group we need to keep a record
		     * of the remote designations until an :endparallel: is
		     * found and the group is run.)
		     */
		     clear_sysid_store(&tc_sysid,num_sysid);
		     num_sysid = 0;
		}
		remote = FALSE; /* Reset remote flag when we come back out
				 * of the included scenario file.
				 */
		newptr = tmp_ptr;
	    }
	    break;

	case ENDREMOTE:

	    aline += strlen(END_REMOTE_STR);
	    if(remote != TRUE)
	    {
		(void) sprintf(error_mesg,"Warning ! 'endremote' directive not required line %d. Ignoring.\n",scenario_line); 
		FATAL_ERROR(error_mesg);
	    }
	    newptr = aline;
	    remote = FALSE; /* No longer in remote */

	    if(group == FALSE)
	    {
	        jnl_remote_end();

		/*
		 * If not in group we want to clear the store of remote
		 * designations. (If in a group we need to keep a record
		 * of the remote designations until an :endparallel: is
		 * found and the group is run.)
		 */
	    
	        clear_sysid_store(&tc_sysid,num_sysid);
	    }
	    else
	    {
	        num_parallel_tc = store_tc(&parallel_tc, END_REMOTE_STR, tc_sysid, num_sysid, scenario_line, scenario_element, num_parallel_tc);
	    }

	    /* no longer within remote directive so reset counter */
	    num_sysid = 0;

	    break;

	case PARALLEL:

	    DBUG(DBGFINFO) (stderr,"Parallel directive found.\n");

	    group = TRUE;

	    /* add parallel directive marker */
	    num_parallel_tc = store_tc(&parallel_tc, PARALLEL_STR, tc_sysid, num_sysid, scenario_line, scenario_element, num_parallel_tc);

	    aline += strlen(PARALLEL_STR);

	    if( (resume_status == 1) /* resume mode active */
                &&
	        (scenario_element == old_element)&&(scenario_line == old_line))
	    {
		if(strcmp(g_tc_line,PARALLEL_STR) != 0)
		{
		    (void) sprintf(error_mesg, "Expected \"%s\" to be the %d%s line of the scenario file, found \"%s\" instead.\n", g_tc_line, old_line, number_suffix(old_line), runline);
		    ERROR(error_mesg);
		    resume_status = -1;
		    newptr = tmp_ptr;
		    TET_FREE((void *) g_tc_line);
		    break; 			/* break out of switch */
		}
                resume_status = 0;
            }

	    if (*aline == TC_START_MARK)
	    {
	        /* process the inclusion file */
	        inclusion_file(aline, &tmp_ptr, resume_status);
	        newptr = tmp_ptr;

		/* add endparallel directive marker */
	    	num_parallel_tc = store_tc(&parallel_tc, END_PARALLEL_STR, tc_sysid, num_sysid, scenario_line, scenario_element, num_parallel_tc);

 		/* End group when we return */
		bec_group(parallel_tc, num_parallel_tc, parallel_pipe_fd, PARALLEL);
	        clear_store(&parallel_tc, num_parallel_tc);
	        num_parallel_tc = 0;
		group = FALSE;
	    }
	    else
	    {
		newptr = aline;
	    }
	    break;

	case ENDPARALLEL:

	    DBUG(DBGFINFO) (stderr,"EndParallel directive found.\n");

	    aline += strlen(END_PARALLEL_STR);
	    /*
	     *  If resume_status is TRUE, none of the TC's within the group
	     *  are to be resumed so we ignore the group.
	     */
	    if(resume_status == FALSE)
	    {
		/* add endparallel directive marker */
	    	num_parallel_tc = store_tc(&parallel_tc, END_PARALLEL_STR, tc_sysid, num_sysid, scenario_line, scenario_element, num_parallel_tc);

	        /*
	         *  Build, Execute, and/or Clean all parallel processes
		 *  within the group.
		 */
		bec_group(parallel_tc, num_parallel_tc, parallel_pipe_fd, PARALLEL);
	    }
	    
	    clear_store(&parallel_tc, num_parallel_tc);
	    num_parallel_tc = 0;
	    group = FALSE;
	    newptr = aline;
	    break;

        default:
            (void) sprintf(error_mesg,"Scenario file : unrecognised element : %s in line %s", aline, orig_scen_line);
            BAIL_OUT2(error_mesg);
            break;
    }

    if(newptr != NULL)
    {
	while((*newptr != '\0') && (isspace(*newptr)))
	    newptr++;

        if(*newptr == '\0')
	    newptr = NULL;
    }

    return(newptr);
}


void inclusion_file(aline, tmp_ptr_ad, resume_status)
char *  aline;
char ** tmp_ptr_ad;
int     resume_status;
{
    char line2[_POSIX_PATH_MAX];
    char *tmp_ptr, *file_name, *newline;
    int type;
    FILE *inc_fp, *prev_fp;
    char *alt_exec_dir;
    char *ts_root;


    DBUG(DBGFDESC) (stderr,"inclusion_file(\"%s\", tmp_ptr_ad)\n", aline);

    alt_exec_dir = get_alt_exec_dir(FALSE, MASTER);
    ts_root = get_ts_root(FALSE, MASTER);

    /* Save current file descriptor */
    prev_fp = curr_fdesc;

    /*  position tmp_ptr after inclusion file */
    for( tmp_ptr = aline;(*tmp_ptr != '\0')&&(!isspace(*tmp_ptr));
                                       tmp_ptr++);
    if(tmp_ptr != '\0')
    {
	*tmp_ptr = '\0';
	++tmp_ptr;
    }

    *tmp_ptr_ad = tmp_ptr;

    /* Position aline just after TC_START_MARK */
    ++aline;

    /* If alt_exec_dir is defined it should be relative to that */
    if( alt_exec_dir != NULL )
    {
	file_name = (char *) TET_MALLOC(strlen(alt_exec_dir) + strlen(aline) + 2);
	(void) sprintf(file_name,"%s/%s", alt_exec_dir, aline);
    }
    else
    {
	/* otherwise it should be relative to the test suite root */
	file_name = (char *) TET_MALLOC(strlen(ts_root) + strlen(aline) + 2);
	(void) sprintf(file_name,"%s/%s", ts_root, aline);
    }

    inc_fp = fopen(file_name,"r");
    if (inc_fp == NULL)
    {
	(void) sprintf(error_mesg,"error opening include file %s\n",file_name);
	perror("fopen");
	BAIL_OUT2(error_mesg);
    }
    else
    {
	DBUG(DBGFILES)(stderr,"fopen(%ld)\n", (long) inc_fp);

	curr_fdesc = inc_fp;
    }

    TET_FREE((void *) file_name);

    /* make sure the file descriptor is closed on an exec call */
    DBUG(DBGFILES)(stderr,"fcntl(%d, F_SETFD, FD_CLOEXEC)\n", (int) fileno(inc_fp));
    (void) fcntl(fileno(inc_fp), F_SETFD, FD_CLOEXEC);

    /*
     * Note the recursive call to process the inclusion file
     */
    while (fgets(line2,sizeof(line2),curr_fdesc) != NULL)
    {
	newline = line2;
	do
	{
	    type = analyse_line(newline, &newline);
	    newline = process_line(newline, type, resume_status);
	}
	while(newline != NULL);
    }

    DBUG(DBGFILES)(stderr,"fclose(%ld)\n", (long) inc_fp);
    (void) fclose(inc_fp);

    /* Reset current file descriptor */
    curr_fdesc = prev_fp;
}

/* Make an scenario file from the command line '-l' argumants */
int make_alt_scenario(scen_path, scen_lines)
char *scen_path;
char **scen_lines;
{
	int count;
	int fid;

	DBUG(DBGFDESC)(stderr, "make_alt_scenario(scen_path:%s, scen_lines:%ld)\n",scen_path, (long)scen_lines);

	fid =  open(scen_path,(O_WRONLY|O_CREAT|O_TRUNC),
		             (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH));
	if(fid == FAILURE)
	{
		return(FAILURE);
	}
	else
	{
	    DBUG(DBGFILES)(stderr,"open(%d)\n", (int) fid);
	}

	(void) write(fid, (void *) "all\n", (size_t) 4);
	for(count=0; scen_lines[count]!=NULL; count++)
	{
		(void) write(fid, (void *) "\t", (size_t) 1);
		(void) write(fid, (void *) scen_lines[count], strlen(scen_lines[count]));
		(void) write(fid, (void *) "\n", (size_t) 1);
	}

	DBUG(DBGFILES)(stderr,"close(%d)\n", (int) fid);
	(void) close(fid);

	return(SUCCESS);
}
