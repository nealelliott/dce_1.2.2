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
static char sccsid[] = "@(#)resfile.c	1.4 (92/11/27) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)resfile.c    1.4 11/27/92
NAME:		resfile.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions relating to the tet_xres file parsing.

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


sigjmp_buf jmpbuf;         /* this is the allocated jmpbuf */
TPRS *rootp;               /* The root pointer to the linked list of TPRS's */


/*
 * This function performs the first stage in organising the results file
 * contents and copying them into the journal file. The order of the lines
 * in the results file can get jumbled by concurrently executing processes,
 * and so special care has to taken to ensure that they appear in the
 * journal file in a coherent order.
 */
int copy_results_file(res_fp, what_mode)
FILE *res_fp;
int  what_mode;
{
    int n = 0, rc = 0;
    char buf[JNL_LINE_MAX + 2];
    char *fg_val;
    int return_val = 0; /* will be set to tp end status */



    DBUG(DBGFDESC) (stderr,"copy_results_file(res_fp,mode : %d)\n", what_mode);


    if(what_mode == BUILD)
    {
        /*
         * We want to find out the tp end status so that it can be checked for
         * a build tool failure - any failures in formatting will be picked up
         * in the full analysis of the file below.
         */

        do
        {
            if ((( fg_val = fgets(buf,sizeof(buf),res_fp)) == NULL) &&
                        (feof(res_fp) == 0))
                    break;
            if (fg_val != NULL)
            {
		/* Sort out the line if it's too long */
		truncate_line(res_fp, buf, (JNL_LINE_MAX+1));
		/*
		 *  If the line was too long we leave 1 byte longer than
		 *  the journal line max so that a warning is generated
		 *  when it is written to the journal file.
		 */

		/* Look for the TET_JNL_TP_END - don't check for aborts */
                rc = tpend(buf); 
                if(rc == FAILURE)
	        {
		    n = 1;
                    break;
	        }
                if(rc != 0) /* this line is the tp end result */
	        {
                    if(sscanf(buf,"%*d | %*d %*d %d",&return_val) != 1)
		    {
        		(void) sprintf(error_mesg,"results file entry has bad format: %s\n", buf);
        		ERROR(error_mesg);
		    }
		    else
		        n = 1;
	        }
            }
        }
        while ( (!feof(res_fp)) && return_val == 0 );

        /*
         *  If n still equals zero we didn't find a TP end line, so assume a
         *  result of NORESULT (7).
         */

        if( n == 0)
	    return_val = 7; 

        /* Rewind to the beginning of the file to do the proper analysis */
        rewind(res_fp);
    }


    /* Now we do the proper analysis */

    while (!feof(res_fp)) /* for all test purposes */
    {
        /* While we don't encounter a TP start write the lines to the jnl */
        do
        {
            if ((( fg_val = fgets(buf,sizeof(buf),res_fp)) == NULL) &&
                    (feof(res_fp) == 0))
            {
                BAIL_OUT("fgets from results file\n"); 
            }

	    /* Sort out the line if it's too long */
	    truncate_line(res_fp, buf, (JNL_LINE_MAX+1));
	    /*
	     *  If the line was too long we leave 1 byte longer than
	     *  the journal line max so that a warning is generated
	     *  when it is written to the journal file.
	     */

            if (fg_val != NULL)
            {
                rc = tpstart(buf); /* Look for the TET_JNL_TP_START */
                if(rc == FAILURE)
                {
                    
                    (void) sprintf(error_mesg,"Bad format line in results file: %s\n",buf);
                    BAIL_OUT2(error_mesg);
                }
                else if (rc == 0)
                    BAIL_OUT_ON(write(jnl_fd,(void *)buf,strlen(buf)), "write results to jnl file");
            }
        } 
        while ( (!feof(res_fp)) && rc == 0 );

        if ((!feof(res_fp)) && (fg_val != NULL))
        {
            /* We should have just encountered a TP start, now sort the rest */
            parse_tprs_list( make_tprs_list(res_fp,buf));
        }
    }
    return(return_val); /* The tp end status or the default of 0 (PASS) */
}


/* 
 * This function processes the results file from and including the TP start
 * line. It generates a doubly linked list of the lines that may need
 * sorting into order, and if need be passes them onto a collate and sort
 * routine. The sorted lines eventually end up in the journal.
 * The Spec details the way in which the lines should be sorted.
 */
int make_tprs_list(res_fp,inbuf)
FILE *res_fp;
char *inbuf;
{
    int buf_len, rc = 0, count = 0;
    char buf[JNL_LINE_MAX + 2], *data_p;
    TPRS *node;

    /* Just so we can a nice neat debug statement */
    {
    char *neat_inbuf;
    neat_inbuf = store_str(inbuf);
    neat_inbuf[strlen(neat_inbuf) - 1] = '\0';
    DBUG(DBGFDESC) (stderr,"make_tprs_list(<FILE *>, %s)\n", neat_inbuf);
    TET_FREE((void *) neat_inbuf);
    }


    /* 
     * At this point,  we've copied out up to (not incl.) TP start,  
     * and we have the TP start in inbuf.
     */

    buf_len = strlen(inbuf);

    /* rootp is the root pointer in a doubly linked list */
    rootp = (TPRS *)(TET_MALLOC(sizeof(TPRS)));
    count++;

    rootp->prev = rootp->next = NULL;

    rootp->data = (char *)TET_MALLOC((size_t)buf_len+1);

    /* node is the pointer to the current node in the doubly linked list */
    node = rootp;

    /* copy the contents of inbuf into the data field of rootp */
    (void) strncpy(rootp->data,inbuf,(size_t)buf_len+1);

    /*
     * Identify the current jnl entry and set the appropiate fields of
     * the structure pointed to by rootp.
     * This is a bit silly coz the entry should be the TP start - see comment
     * above ...
     */
    rc = analyze_jnl_entry(rootp->data,&(rootp->tpnum),
            &(rootp->block), &(rootp->num),&(rootp->context));
    if(rc == FAILURE)
    {
        /* Failed to sucessfull classify a line - free() everything & exit */
        (void) sprintf(error_mesg,"Bad format line in results file: %s\n",rootp->data);
        TET_FREE((void *) rootp->data);
        TET_FREE((void *) rootp);
        BAIL_OUT2(error_mesg);
    }
    else
        rootp->type = rc;

    /* As long as we can get lines from the file add them to the list */
    do
    {
        if ((data_p = fgets(buf,sizeof(buf),res_fp)) != NULL)
        {
            if (feof(res_fp))
                break;

	    /* Sort out the line if it's too long */
	    truncate_line(res_fp, buf, (JNL_LINE_MAX+1));
	    /*
	     *  If the line was too long we leave 1 byte longer than
	     *  the journal line max so that a warning is generated
	     *  when it is written to the journal file.
	     */

            buf_len = strlen(buf);

            /*
             * Create a new node. Ensure the "next" pointer of the current
             * node points to the new node. Ensure the "prev" pointer of 
             * the new node points to the current node. Then advance the
             * current node pointer to this new node.
             */

            node->next = (TPRS *)(TET_MALLOC(sizeof(TPRS)));
            (node->next)->prev = node;
            node = node->next;
            count++;

            /*
             * Identify the current jnl entry and set the appropiate fields of
             * the structure pointed to by node 
             */
            rc = analyze_jnl_entry(data_p,&(node->tpnum),&(node->block),&(node->num), &(node->context));
            if(rc == FAILURE)
            {
                /*
                 * Failed to sucessfull classify a line
                 * free all the allocated nodes and exit.
                 */

		/* This the new node just allocated - no data associated yet */
	        node = node->prev;
                TET_FREE((void *) node->next);

                while(node->prev != NULL)
                {
                    TET_FREE((void *) node->data);
                    node = node->prev;
                    TET_FREE((void *) node->next);
                }

		/* node should be pointing to the rootp at this stage */
                TET_FREE((void *) node->data);
                TET_FREE((void *) node);

                (void) sprintf(error_mesg,"Bad format line in results file: %s\n",data_p);
                BAIL_OUT2(error_mesg);
            }
            else
                node->type = rc;

            node->data = (char *)TET_MALLOC((size_t)buf_len+1);
            (void) strncpy(node->data,buf,(size_t)buf_len+1);
            node->next = NULL;
        }
    } 
    while ( (data_p != NULL) && (feof(res_fp) == 0) );

    return(count);
}


/*
 * Take the list generated by make_tprs_list() and wade through it, sorting
 * and making entries into the journal as we go along
 */
void parse_tprs_list( count)
int count;
{
    int found_tc_info, this_tpnum = -1;
    TPRS *node;


    DBUG(DBGFDESC) (stderr, "parse_tprs_list(%d)\n", count);


    /*
     * OK - At this point we have a doubly linked list of all the
     * remaining entries in the tmp results file from and including
     * TET_JNL_TP_START 
     */

    while( rootp != NULL)
    {
        node = rootp;
        found_tc_info = 0;

        /*
         * Look for a Test Case Info line - these are the ones that might need
         * sorting. 
         */
	for( ; node->next != NULL; node = node->next)
        {
            if(node->type == TET_JNL_TC_INFO)
            {
                found_tc_info = 1;
                break;
            }

	    if(node->type == TET_JNL_TP_RESULT)
	        this_tpnum = -1;
	    else if(node->type == TET_JNL_TP_START)
	    {
	        if (this_tpnum == -1)
	            this_tpnum = node->type;
	        else                  /* ie. missing a TP End Line */
		    break;
	    }
	    else if(node->type == TET_JNL_IC_END)
	    {
	        if (this_tpnum != -1) /* ie. missing a TP End Line */
		    break;
	    }
        }

        if(! found_tc_info)
        {
	    /* Missing a TP End Line */
	    if ( node->next == NULL)
            {
		/* End of list */
                pr_list_to_jnl(rootp);
		rootp = NULL;
            }
	    else /* TET_JNL_TP_START || TET_JNL_IC_END */
            {
                (node->prev)->next = NULL;
                node->prev = NULL;
                /* Print the list and append a TP result line */
                pr_list_to_jnl(rootp);
	        jnl_tp_result(exec_ctr,this_tpnum,7,"NORESULT, Added by TCC");
		this_tpnum = -1;
                rootp = node;
            }
        }
        else
        {
	    if (node != rootp) /* ie for interleaved info lines */
	    {
	        /* Flush the accumalated lines to the journal */
                (node->prev)->next = NULL;
                node->prev = NULL;
                pr_list_to_jnl(rootp);
	    }

            /* Collect more lines: same context and block - then sort them */
            node = collate_and_sort(node,count+1);
            if(node != (TPRS *) NULL) 
            {
                /* change of Test Purpose Num - Not good */
		if(node == rootp)
		{
        	    jnl_tp_result(exec_ctr,this_tpnum,7,"NORESULT, Added by TCC");
		    this_tpnum = -1;
		}
            }
        }
    }
}


/*
 * Take a doubly linked list and print it to the journal file, if the flag
 * is set and the last entry isn't a TET_JNL_TP_RESULT then add one of our
 * own. 
 */
void pr_list_to_jnl(node)
TPRS *node;
{
    TPRS *newnode;


    DBUG(DBGFDESC) (stderr,"pr_list_to_jnl(TPRS *)\n");

    /* put in the journal,and free */
    while(node != NULL)
    {
        jnl_entry(node->data);

	DBUG(DBGFINFO) (stderr,"\t%s", node->data);

        TET_FREE((void *)node->data);
        newnode = node->next;
        TET_FREE((void *)node);
	node = newnode;
    }
}


/*
 *  Analyze a line for the journal, setting values for the current node in
 *  the TPRS structure as appropiate. If the wrong number of field is read
 *  then a FAILURE is indicated
 */
int analyze_jnl_entry(data_p,tpnum_p,block_p,num_p,context_p)
char *data_p;
int *tpnum_p;
int *block_p;
int *num_p;
int *context_p;
{
    int type, rc, rescode;

    *tpnum_p = *block_p = *num_p = *context_p = -1;
    rc = sscanf(data_p,"%d|",&type);
    if (rc < 1)
        return(FAILURE);

    switch (type)
    {
    case TET_JNL_TP_START: /* 200 */
        if(sscanf(data_p,"%*d|%*d %d",tpnum_p) != 1)
            return(FAILURE);
        break;
    case TET_JNL_TC_INFO: /* 520 */
        if(sscanf(data_p,"%*d|%*d %d %d %d %d",tpnum_p,context_p,block_p,num_p) != 4)
            return(FAILURE);
        break;
    case TET_JNL_TP_RESULT: /* 220 */
        if(sscanf(data_p,"%*d|%*d %d %d",tpnum_p, &rescode) != 2)
            return(FAILURE);

	/* check to see if result code maps to an abort */
	if (abort_requested() != TRUE)
            if (*rescode_num_to_action(rescode) == 'A')
	    {
		/* Set the Abort Flag */
                (void) set_abort_req(SETVALUE);

		DBUG(DBGFINFO) (stderr,"Found ABORT result code !\n");

	    }
        break;
    default:
        break;
    }

    return(type);
}


/*
 * group together an array of entries with the same context
 * and block numbers. As each entry is found it is removed from the the
 * origianl list and added to a new one. The new list is then sorted and
 * then printed to the journal file 
 */
TPRS *collate_and_sort(node, count)
TPRS *node;
int count;
{
    TPRS **sortlist;
    int this_context, this_block;
    int list_pos = 0, rootpset = 0;


    DBUG(DBGFDESC) (stderr,"collate_and_sort(<TPRS *>, %d)\n", count);


    this_context = node->context;
    this_block = node->block;


    sortlist = (TPRS **) TET_MALLOC(sizeof(TPRS) * count);

    while(node != NULL && node->type != TET_JNL_TP_RESULT)
    {
        /* Missing TP Result Line */
	if(node->type == TET_JNL_TP_START || node->type == TET_JNL_IC_END)
	{
	    break;
	}
        else if(node->context == this_context && node->block == this_block)
        {
            /*
             * this bit removes the entry from the original linked list
             * and adds the entry to the end of the sort list 
             */
            if(node->prev != NULL)
                (node->prev)->next = node->next;
            if(node->next != NULL)
                (node->next)->prev = node->prev;

            sortlist[list_pos++] = node;
        }
        else if( ! rootpset)
	{
	    rootp = node;
	    rootpset = 1;
	}
        node = node->next;
    }

    /* now we sort the list we have gathered according to sequence number */
    qsort((void *)sortlist,(size_t)list_pos,sizeof(sortlist[0]),
        compare_seq_num);

    /* Print the sorted list to the journal */
    pr_array_to_jnl(sortlist, list_pos);

    TET_FREE((void *) sortlist);

    if(! rootpset)
        rootp = node;

    if(node == NULL || node->type == TET_JNL_TP_RESULT)
    {
        /* This indicates full success */
        return((TPRS *) NULL);
    }
    else
    {
        /* Missing TP End Line */
        /* Return a pointer to the node that begins the change of TP num */
        return(node);
    }
}


/*
 *  Comparison function used for qsort()
 */
int compare_seq_num(node1, node2)
#if __STDC__
const void  *node1, *node2;
#else
TPRS **node1, **node2;
#endif
{
    return( (*( (TPRS **)node1 ))->num - (*( (TPRS **)node2 ))->num);
}


/*
 * Print an array of journal type lines to the journal.
 */
void pr_array_to_jnl(list, list_len)
TPRS **list;
int list_len;
{
    int i = 0;

    DBUG(DBGFDESC) (stderr,"pr_array_to_jnl(TPRS *list, %d)\n", list_len);


    /* put in the journal */
    while(i < list_len)
    {
        jnl_entry(list[i]->data);

	DBUG(DBGFINFO) (stderr,"\t%s", list[i]->data);

        TET_FREE((void *)list[i]->data);
        TET_FREE((void *)list[i]);
        i++;
    }
}
