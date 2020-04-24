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
/************************************************************************

#ifndef lint
static char sccsid[] = "@(#)again.c	1.6 (93/01/25) DTET release 1.0b";
#endif

SCCS:           @(#)again.c    1.6 1/25/93
NAME:           again.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.9 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions relating to the resume and rerun options

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
 *  There is a known bug in this release. This define should be set to the
 *  maximum number of invocable components that will occur in any test case.
 */
#define ICS_MAX 50


static char linein[JNL_LINE_MAX]; /* A line read in from an old journal file */
char  *g_tc_line;                 /* Global tc line - used in resume mode */
int   g_ic_num = -1;              /* Global ic number - used in resume mode */

/* variables for remote systems */
static struct sysid_store *tc_sysid = NULL;
static int num_sysid;

/* variables for parallel processes */
static struct tc_store *rgroup_tc;
static int num_rgroup_tc;
static int rgroup_pipe_fd[2];



/*
 *  This routine combines the functionality described for both the rerun and
 *  resume options.
 *  If a TC is found to rerun within a group, all the TC's and all the IC's
 *  are rerun within that group.
 */
int do_again(rerun, result_list, result_mode)
int   rerun;
int * result_list;
int * result_mode;
{
    int var, ctr, ctr2, saved_ic_num, status, curr_mode = -1;
    int ic_max = 0, *ic_buf, perform_rerun = FALSE, match_rc;
    int *ics = NULL , has_ic_list;
    char *cp, *saved_tc_name = NULL, *tmp_ptr, copyline[JNL_LINE_MAX];
    int count;
    int *repeats;
    /* When inside a repeat we must be carefull that we don't get mixed
     * up between TC's that are being repeated and different TC's.
     * last_group_line and last_group_element are used aswell as last_line
     * and last_element in order to avoid this problem. */
    int last_group_line = -1;
    int last_group_element = -1;
    int last_line = -1;
    int last_element = -1;
    int jnl_line_num = 0;

    int repeat = FALSE;
    int total_repeat = 1;
    int group = FALSE;

    DBUG(DBGFDESC) (stderr,"do_again(%s)\n", (rerun == RERUN ? "rerun" : "resume"));

    /* Invocable component list buffer */
    ic_buf = (int *) TET_MALLOC((size_t) (sizeof(int) * ICS_MAX));
    ics = (int *) TET_MALLOC((size_t) (sizeof(int) * ICS_MAX));
    ics[0] = -2;
    ic_buf[0] = -2;

    /* Look through the old journal file */
    while (fgets(linein,sizeof(linein),old_jnl_fp) != NULL)
    {
	jnl_line_num++;
        if (linein[strlen(linein)-1] == '\n')
            linein[strlen(linein)-1] = '\0';
	
        if ((var = modestart(linein)) > 0) /* A mode start line ? */
        {
	    /* The call to rerun_flush corrupts linein so .. */
	    (void) strcpy(copyline, linein); 

            if( (var <= curr_mode)
                 &&
		((perform_rerun == TRUE) && rerun) /* We have a TC to rerun */
                 &&
		(group == FALSE) /* Not in a group */
		 &&
		/* We haven't just exec/built/cleaned this TC. */
		((last_line!=old_line) || (last_element!=old_element)) )
	    {

		if(abort_requested())
		    total_repeat = 1;

		/* If we are within any repeat loops... repeat the whole TC. */
		for(count=0; count<(total_repeat); count++)
		{
                    rerun_flush(ic_max, saved_tc_name, ic_buf);
		}

		/* Old line and element numbers are then reset to the current
		 * mode line.
		 */
		last_line = old_line;
		last_element = old_element;

                if (abort_requested())
		    return(SUCCESS);

		perform_rerun = FALSE;

            }

	    if( (var <= curr_mode || curr_mode == -1)) /* New test case */
	    {
		TET_FREE((void *) ic_buf);
                ic_buf = (int *) TET_MALLOC((size_t) (sizeof(int) * ICS_MAX));
                ic_buf[0] = -2;

		TET_FREE((void *) ics);
                ics = (int *) TET_MALLOC((size_t) (sizeof(int) * ICS_MAX));
                ic_max = 0;
		ics[0] = -2;
	    }

            curr_mode = var;

            /* Save the TC name */
            cp = strchr(copyline,TC_START_MARK);
            if (cp == NULL)
            {
                (void) sprintf(error_mesg, "Bad TC start line format, old journal:%s\n",copyline);
                ERROR(error_mesg);
                continue;
            }

            tmp_ptr = strtok(cp," \t");

	    /* We need to store all the test cases within a group just in
	     * case one of them failed.
	     */
	    if( rerun && (group == TRUE) )
	    {
		/* If we don;t already have the tc, store it */
		if(!isstored(&rgroup_tc, old_line, old_element,num_rgroup_tc))
		{
		    num_rgroup_tc = store_tc(&rgroup_tc,tmp_ptr,tc_sysid,num_sysid,old_line,old_element, num_rgroup_tc);
		}
	    }
	    else /* Save the testcase name as normal */
	    {
                if (saved_tc_name != NULL)
                    TET_FREE((void *)saved_tc_name);

		saved_tc_name = store_str(tmp_ptr);
	        has_ic_list = 0;
	        cp = strchr(saved_tc_name,IC_START_MARK);
		
	        /* strip off any specified ICs */
	        if (cp != NULL)
	        {
	            has_ic_list = 1;
		    make_ic_list(cp, &ics);
		    *cp = '\0';
		}
	    }
        }
        else if (icstart(linein)) /* Is this line an IC start line ? */
        {
            if (sscanf(linein,"%*d|%*d %d",&saved_ic_num) != 1)
            {
                ERROR("Bad IC start line,  format in old journal file.\n");
            }

	    /* if the TC didn't have an IC list make one as we go along */
	    if(! has_ic_list)
	    {
	        for(ctr = 0; ics[ctr] != -2; ctr++)
	        {
	            if(ctr % ICS_MAX == (ICS_MAX -2))
	            {
		        ics = (int *) TET_REALLOC((void *) ics, (size_t) (sizeof(int) * (ICS_MAX+1)));
		        for(ctr2 = ctr; ctr2 < (ctr + ICS_MAX); ics[ctr2++] = -1);
		        ics[ctr2] = -2;
	            }
	        }
	        ics[ctr] = saved_ic_num;
		ics[++ctr] = -2;
	    }
        }
        else if (icend(linein)) /* Is this line an IC end line ? */
        {
            if (sscanf(linein,"%*d|%*d %d",&var) != 1)
            {
                ERROR("Bad IC start line,  format in old journal file.\n");
            }
	    for(ctr = 0; ics[ctr] != -2; ctr++)
	    {
		if (ics[ctr] == var)
		    ics[ctr] = -1;
	    }

        }
	else if ((var = modeend(linein)) != 0) /* The end of a mode */
	{
            if (sscanf(linein,"%*d|%*d %d", &status) != 1)
            {
                ERROR("Bad TP status line format in old journal file.\n");
            }
	    if (status != 0) /* Didn't run to completion */
	    {
		if ((result_mode[0] == 1 && var == 1) || (result_mode[1] == 1 && var == 2) || (result_mode[2] == 1 && var == 3))
		{
		    if (! rerun)
		    {
			if(group == TRUE)
			{
			    /* We are in a group so we must seach for the group
			     * start and not the current TC.
			     */ 
			    g_tc_line = (char *) TET_MALLOC(strlen(PARALLEL_STR)+2);
		            (void) strcpy(g_tc_line, PARALLEL_STR);
			}
			else
			{
		            g_tc_line = (char *) TET_MALLOC(strlen(saved_tc_name)+2);
		            (void) strcpy(g_tc_line, saved_tc_name);
			}
			
		        TET_FREE((void *) saved_tc_name);

			/* look for unended ICs */
			for(ctr = 0; ics[ctr] != -2; ctr++)
			{
			    if(ics[ctr] != -1)
			    {
		                g_ic_num = ics[ctr];
				break;
			    }
			}

			if (ics != NULL)
			     TET_FREE((void *) ics);
			if (ic_buf != NULL)
			     TET_FREE((void *) ic_buf);

			/* NOTE: If we are in a group, g_tc_line has been
			 * previously set to PARALLEL_STR and will make this
			 * debug statement missleading.
			 */
		        DBUG(DBGFINFO) (stderr,"Incomplete mode (%d), looking for : g_tc_line: %s  g_ic_num: %d  old_line: %d\n", var, g_tc_line, g_ic_num, old_line);

		        return(SUCCESS);
		    }

		    perform_rerun = TRUE;
		    if(var == 2) /* Exec */
		    {
			for(var = 0; ics[var] != -2; var++)
			{
			    /* Check that IC number isn't already in the list */
			    for(ctr = 0; ctr < ic_max; ctr++)
			    {
			        if (ic_buf[ctr] == ics[var])
				    break;
			    }

			    /* Not already in list */
			    if ((ctr == ic_max) && (ics[var] != -1))
			    {
			        /* Run out of space - make some more */
			        if(ic_max % ICS_MAX == (ICS_MAX-2))
				    ic_buf = (int *) TET_REALLOC((void *) ic_buf, (size_t) (sizeof(int) * (ic_max + (ICS_MAX+1))));

			        /* Add the IC to the list for this test case */
			        ic_buf[ic_max++] = ics[var];
			    }
			}
		    }
		}
	    }
	}
        else if (tpend(linein)) /* Is this line a test purpose end line */
        {
            if (sscanf(linein,"%*d|%*d %*d %d", &status) != 1)
            {
                ERROR("Bad TP status line format in old journal file.\n");
            }
            /* 
             *  Look for a matching result code, make sure we 
             *  aren't running the same invocation case, but
             *  for a different test purpose as this causes repitition. 
             */
	    match_rc = 0;
            for (var = 0 ; result_list[var] != -1; var++)
	    {
                if (status == result_list[var])
			match_rc = 1;
	    }
	    if (status != 0)
	    {
		if ((result_mode[0] == 1 && curr_mode == 1) || (result_mode[1] == 1 && curr_mode == 2) || (result_mode[2] == 1 && curr_mode == 3))
		    match_rc = 1;
	    }
            if (match_rc == 1)
            {
		if (! rerun)
		{

      		    if (ics != NULL)
			 TET_FREE((void *) ics);
		    if (ic_buf != NULL)
			 TET_FREE((void *) ic_buf);

		    /* If we are in a group we need to start from the begining
		     * of the group.
		     */
		    if(group == FALSE)
		    {
	                g_tc_line = (char *) TET_MALLOC(strlen(saved_tc_name)+2);
	                (void) strcpy(g_tc_line, saved_tc_name);
	                TET_FREE((void *) saved_tc_name);
	                g_ic_num = saved_ic_num;

		        DBUG(DBGFINFO) (stderr,"Matched TP result, looking for : g_tc_line: %s  g_ic_num: %d  old_line: %d\n", g_tc_line, g_ic_num, old_line);

		    }
		    else
		    {
	                g_tc_line = (char *) TET_MALLOC(strlen(PARALLEL_STR)+2);
	                (void) strcpy(g_tc_line, PARALLEL_STR);
	                TET_FREE((void *) saved_tc_name);
			old_line = group_line;
			old_element = group_element;

			DBUG(DBGFINFO) (stderr,"About to resume from the start of a group at line %d element %d.\n",group_line,group_element);

		    }
	            return(SUCCESS);
		}

		/* Check to see IC number isn't already in the list */
	 	for(ctr = 0; ctr < ic_max; ctr++)
	 	{
		    if (ic_buf[ctr] == saved_ic_num)
		        break;
		}

		if (ctr == ic_max) /* Not already in list */
		{
		    /* Run out of space - make some more */
                    if(ic_max % ICS_MAX == (ICS_MAX-2))
                        ic_buf = (int *) TET_REALLOC((void *) ic_buf, (size_t) (sizeof(int) * (ic_max + (ICS_MAX+1))));

		    /* Add the IC to the list for this test case */
                    ic_buf[ic_max++] = saved_ic_num;
		    perform_rerun = TRUE;
		}
            }
        }
	else if((var=directivestart(linein)) > 0)
	{
	    switch(var)
	    {
		case 1:
		    DBUG(DBGRUNRES)(stderr, "Remote start directive found.\n");
		    if( (tmp_ptr = strrchr(linein,'|')) == NULL)
		    {
			(void) sprintf(error_mesg,"Format error in 'remote start' journal message on line %d.\n", jnl_line_num);
		        FATAL_ERROR(error_mesg);
		    }
		    *tmp_ptr = '\0';
		    if( (tmp_ptr = strchr(linein,'|')) == NULL)
		    {
			
			(void) sprintf(error_mesg,"Format error in 'remote start' journal message on line %d.\n", jnl_line_num);
		        FATAL_ERROR(error_mesg);
		    }
		    tmp_ptr++;
		    DBUG(DBGRUNRES)(stderr, "sysids %s.\n", tmp_ptr);
		    num_sysid = str_to_sysids(&tc_sysid,tmp_ptr,num_sysid);
		    for(count =0;count<num_sysid;count++)
		    {
		        DBUG(DBGRUNRES)(stderr, "Found sysid %d.\n", tc_sysid[count].sysid);
		    }
		
		    break;
		case 2:
		    DBUG(DBGRUNRES)(stderr, "Parallel start directive found.\n");
		    group = TRUE;
		    break;
		case 3:
		    DBUG(DBGRUNRES)(stderr, "Repeat start directive found.\n");
		    if(sscanf(linein,"%*d|%d|", &count) != 1)
		    {
			(void) sprintf(error_mesg,"Old journal file entry has bad format : \"%s\" expected : \"%d|<repeat count>|Repeat Starts, scenario ref <num>-<element>\"\n",linein,TET_JNL_RPT_START);
			FATAL_ERROR(error_mesg);
		    }
		    if(repeat == 0)
		    {
			repeats = (int *)TET_MALLOC(sizeof(int)*20);
		    }
		    else
		    {
		        if(repeat%20 == 0)
			    repeats = (int *)TET_REALLOC(repeats,sizeof(int)*(repeat+20));
		    }
		    repeats[repeat] = count;
		    total_repeat = total_repeat*count;
		    repeat++;
		    break;
		default:
		    break;
	    }
	}
	else if((var=directiveend(linein)) > 0)
	{
	    switch(var)
	    {
		case 1:
		    DBUG(DBGRUNRES)(stderr, "Remote end directive found.\n");
		    clear_sysid_store(&tc_sysid,num_sysid);
		    num_sysid = 0;
		    break;

		case 2:
		    DBUG(DBGRUNRES)(stderr, "Parallel end directive found.\n");

		    if(perform_rerun == TRUE)
		    {
			if(repeat > 0) /* Within a repeat */
			{
			    /* If we haven't just run this group */
			    if((last_group_line != group_line) ||
					(last_group_element != group_element))
			    {
				if(abort_requested())
				    total_repeat = 1;

			        for(count = 0; count < total_repeat; count++)
				{
				    bec_group(rgroup_tc, num_rgroup_tc, rgroup_pipe_fd, PARALLEL);
				}

			        last_group_line = group_line;
			        last_group_element = group_element;
			    }
			}
		        else
			{
			    bec_group(rgroup_tc, num_rgroup_tc, rgroup_pipe_fd, PARALLEL);
			}
                        if (abort_requested())
			{
		            clear_store(&rgroup_tc, num_rgroup_tc);
			    num_rgroup_tc = 0;
    			    if (saved_tc_name != NULL)
			        TET_FREE((void *)saved_tc_name);
   			    if (ic_buf != NULL)
        			TET_FREE((void *)ic_buf);
    			    if (ics != NULL)
				TET_FREE((void *)ics);
		            return(SUCCESS);
			}
			perform_rerun = FALSE;
		    }
		    clear_store(&rgroup_tc, num_rgroup_tc);
		    group = FALSE;
		    break;
		case 3:
		    DBUG(DBGRUNRES)(stderr, "Repeat end directive found.\n");
		    repeat--;
		    total_repeat = total_repeat/repeats[repeat];
		    if(repeat == 0)
		        TET_FREE(repeats);
		    break;
	    }
	}
    }

    if( (perform_rerun == TRUE && rerun)
	 &&
	/* We haven't just exec/built/cleaned this TC. */
	((last_line!=old_line) || (last_element!=old_element)) )
    {
	/* Note that we can't be within a group, or a repeat since we have
	 * reached the end of the file. */
        rerun_flush(ic_max, saved_tc_name, ic_buf);
    }


    if (saved_tc_name != NULL)
        TET_FREE((void *)saved_tc_name);
    if (ic_buf != NULL)
        TET_FREE((void *)ic_buf);
    if (ics != NULL)
	TET_FREE((void *)ics);

    return(SUCCESS);
}


/*
 *  Flush the modes gathered in the do_again(RERUN) routine
 */
void rerun_flush(ic_max, saved_tc_name, ic_buf)
int  ic_max;
char *saved_tc_name;
int  *ic_buf;
{
    int ctr, rc = SUCCESS;
    char scen_line[INPUT_LINE_MAX];

    DBUG(DBGFDESC) (stderr,"rerun_flush(ic_max:%d,%s,ic_buf)\n",ic_max, saved_tc_name);

    /* check to see whether this TC should be processed */
    if(check_line(saved_tc_name) == FAILURE)
    {
        DBUG(DBGFINFO) (stderr,"TC not to be processed.\n");

        return;
    }

    /* flush mode buffers */
    if(ifmode(BUILD))
    {
	rc = proc_tc_dir(saved_tc_name,tc_sysid,num_sysid,BUILD,FALSE,0,FALSE);

	DBUG(DBGFINFO) (stderr,"return from build is %d\n", rc);
    }
    if( ifmode(EXEC) && rc != FAILURE)
    {
	(void) strcpy(scen_line, saved_tc_name);
	if(ic_max != 0) /* Now allowed to pass IC's as part of saved_tc_name.
			 * If this is the case, ic_max must have been set to 0.
			 */
	{
	    /* sort list of ICS */
	    if (ic_max > 1)
	    {
	        qsort((void *) ic_buf, (size_t) ic_max, sizeof(ic_buf[0]), compare_ics);
	    }
            for (ctr = 0; ctr < ic_max; ctr++)
            {
	        if (ctr == 0)
		    (void) strcat(scen_line,"{");

                (void) sprintf(scen_line,"%s%d",scen_line,ic_buf[ctr]);

	        if ((ctr+1) == ic_max)
		    (void) strcat(scen_line,"}");
	        else
	    	    (void) strcat(scen_line,",");
	    }
        }

	if(num_sysid == 0)
	{
	    rc = proc_tc_dir(saved_tc_name,tc_sysid,num_sysid,EXEC,FALSE,0,FALSE);
	}
	else
	{
	    rc = proc_tc_dir(scen_line,tc_sysid,num_sysid,EXEC,FALSE,0,FALSE);
	}

	DBUG(DBGFINFO) (stderr,"return from exec is %d\n", rc);

    }
    if( ifmode(CLEAN) && rc != FAILURE && abort_requested() != TRUE)
    {
	rc = proc_tc_dir(saved_tc_name,tc_sysid,num_sysid,CLEAN,FALSE,0,FALSE);

	DBUG(DBGFINFO) (stderr,"return from clean is %d\n", rc);
    }

    return;
}


/*
 *  Comparison function used for qsort() in rerun_flush()
 */
int compare_ics(ic1, ic2)
#if __STDC__
const void  *ic1, *ic2;
#else
int *ic1, *ic2;
#endif
{
    return(((int) *(int *)ic1) - ((int) *(int *)ic2));
}


/* 
 * Expand an invocable component specification into a list of numbers for use
 * in the rerun routine.
 */
void make_ic_list(str, icsp)
char *str;
int **icsp;
{
    char *ptr, *cp;
    int ctr = 0, last = 0, num1, num2;

    DBUG(DBGFDESC) (stderr,"make_ic_list(%s,icsp)\n", str);

    ptr = str;
    ++ptr; /* advance past opening bracket */

    cp = strchr(ptr,IC_END_MARK);
    *cp = '\0';

    while ( ! last)
    {
        cp = strchr(ptr,IC_SEP_MARK);
        if (cp == NULL)
        {
	    if( ! strcmp(ptr,"all"))
	        break;
	    else
	        last = 1;
        }
        else
	    *cp = '\0';

        if (strchr(ptr,'-') == NULL)
        {
	    (void) sscanf(ptr,"%d", &num1);
	    num2 = num1;
        }
        else
	    (void) sscanf(ptr,"%d-%d", &num1, &num2);

        for ( ; num1 <= num2; num1++)
	{
	    /* Run out of space - make some more */
	    if(ctr % 50 == 48)
		*icsp = (int *) TET_REALLOC((void *) *icsp, (size_t) (sizeof(int) * (ctr + 51)));

	    (*icsp)[ctr++] = num1;
	}

	if ( !last)
		ptr = (cp+1);
    }

    (*icsp)[ctr] = -2;


    DBUG(DBGFINFO) (stderr,"*icsp:");

    if((*icsp)[0] == -2)
    {
        DBUG(DBGFINFO) (stderr,"all");
    }
    else
    {
        DBUG(DBGFINFO) (stderr,"%d", *icsp[0]);
    }

    for(ctr = 1; (*icsp)[ctr] != -2; ctr++)
    {
        DBUG(DBGFINFO) (stderr,",%d", (*icsp)[ctr]);
    }

    DBUG(DBGFINFO) (stderr,"\n");

}
