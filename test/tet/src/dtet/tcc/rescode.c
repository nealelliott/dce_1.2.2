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
static char sccsid[] = "@(#)rescode.c	1.5 (92/11/27) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)rescode.c    1.5 11/27/92
NAME:		rescode.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions relating to result code handling

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

/* Initialise the results code table */
RESCODE res_tbl[MAX_RESCODES] = { 
    { 999,   "ENDENDEND",       "Abort"       } 
};

/* Default result codes and their corresponding actions */
RESCODE DEF_RES_CODE[] = {
    { 0,     "\"PASS\"",        "Continue"    },
    { 1,     "\"FAIL\"",        "Continue"    },
    { 2,     "\"UNRESOLVED\"",  "Continue"    },
    { 3,     "\"NOTINUSE\"",    "Continue"    },
    { 4,     "\"UNSUPPORTED\"", "Continue"    },
    { 5,     "\"UNTESTED\"",    "Continue"    },
    { 6,     "\"UNINITIATED\"", "Continue"    },
    { 7,     "\"NORESULT\"",    "Continue"    },
    { 999,   "ENDENDEND",       "Abort"       }
};

int g_max_rescode = 0;  /* The current number of defined result codes */


/*
 * Set the default result codes & actions, then override/supplement
 * them by any user-supplied result codes files that exist - firstly
 * $TET_ROOT/$TET_RESCODE_FILE, then $TET_ROOT/suite-name/$TET_RESCODE_FILE.
 * No arguments; returns SUCCESS
 */
int get_rescode_file()
{
    char *rcf_name, *rcfn_p;
    FILE *rc_fp;
    int ctr;
    char * tetroot;


    DBUG(DBGFDESC) (stderr,"get_rescode_file()\n");

    tetroot = get_tet_root(FALSE, MASTER);

    /*
     *  Find an env. that has the value of TET_RESCODES_FILE in it.
     *  It doesn't matter which we use.
     */
    rcfn_p = get_tetenv("TET_RESCODES_FILE", bld_env);
    if( (rcfn_p == NULL) || (*rcfn_p == '\0') )
        rcfn_p = get_tetenv("TET_RESCODES_FILE", exe_env);
    if( (rcfn_p == NULL) || (*rcfn_p == '\0') )
        rcfn_p = get_tetenv("TET_RESCODES_FILE", cln_env);
    if( (rcfn_p == NULL) || (*rcfn_p == '\0') )
    {
	BAIL_OUT2("TET_RESCODES_FILE has not been set.");
	return(FAILURE);
    }

    /* initialize with default values */
    for (ctr = 0 ; (strcmp(DEF_RES_CODE[ctr].name,"ENDENDEND") != 0); ctr++)
    {
        res_tbl[ctr].num = DEF_RES_CODE[ctr].num;
        res_tbl[ctr].name = (char *) TET_MALLOC2((size_t) (strlen(DEF_RES_CODE[ctr].name) + 1));
        (void) strcpy(res_tbl[ctr].name,DEF_RES_CODE[ctr].name);
        (void) strcpy(res_tbl[ctr].action,DEF_RES_CODE[ctr].action);
    }

    g_max_rescode = ctr;

    /* pick up values from TET_ROOT file */
    rcf_name = (char *)TET_MALLOC(strlen(tetroot)+strlen(rcfn_p)+3);

    (void) strcpy(rcf_name,tetroot);
    cat_path(rcf_name, rcfn_p);

    rc_fp = fopen(rcf_name,"r");

    if (rc_fp != NULL)
    {
	DBUG(DBGFILES)(stderr,"fopen(%ld)\n", (long) rc_fp);

        if(do_rescode_file(rc_fp) == FAILURE)
        {
            BAIL_OUT2("Error in results code file.");
        }

	DBUG(DBGFILES)(stderr,"fclose(%ld)\n", (long) rc_fp);
        (void) fclose(rc_fp);
    }

    TET_FREE((void *)rcf_name);

    /* pick up values from suite specific file */
    rcf_name = (char *)TET_MALLOC(strlen(tetroot) + strlen(suite_name) + strlen(rcfn_p) + 4);

    /*
     *  Build up the file name... tetroot + suite_name + file_name
     */
    (void) strcpy(rcf_name,tetroot);
    if ((rcf_name)[strlen(rcf_name)-1] != DIR_SEP_CHAR)
        (void) strcat(rcf_name,DIR_SEP_STR);

    (void) strcat(rcf_name,suite_name);
    if ((rcf_name)[strlen(rcf_name)-1] != DIR_SEP_CHAR)
        (void) strcat(rcf_name,DIR_SEP_STR);

    (void) strcat(rcf_name,rcfn_p);

    rc_fp = fopen(rcf_name,"r");

    if (rc_fp != NULL)
    {
	DBUG(DBGFILES)(stderr,"fopen(%ld)\n", (long) rc_fp);

        if(do_rescode_file(rc_fp) == FAILURE)
        {
            BAIL_OUT2("Error in results code file.");
        }

	DBUG(DBGFILES)(stderr,"fclose(%ld)\n", (long) rc_fp);
        (void) fclose(rc_fp);
    }

    TET_FREE((void *)rcf_name);

    return(SUCCESS);
}


/*
 * Read a result codes file, and store its contents in the table.
 * Arguments:
 * file pointer to opened result codes file.
 * Returns SUCCESS (fatal error -> exit() call being made!)
 * The format of this file is checked quite carefully ! - I hope...
 */
int do_rescode_file(rc_fp)
FILE *rc_fp;
{
    int idx, num, i, rc = SUCCESS;
    char linein[INPUT_LINE_MAX];
    char *name = NULL, action[RESCODE_AC_LEN];
    char *myline = NULL;
    char *num_str = NULL, *name_str = NULL, *action_str = NULL;


    DBUG(DBGFDESC) (stderr,"do_rescode_file(rc_fp)\n");

        
    /*
     * Keep getting lines from the results file until you can't get any more
     * or until the maximum number of results codes is reached
     */
    while ( (fgets(linein,sizeof(linein),rc_fp) != NULL) &&
        (!feof(rc_fp)) && (g_max_rescode < MAX_RESCODES) )
    {
        if (linein[0] == CFG_CMNT_CHAR)
            continue;
        linein[strlen(linein) - 1] = '\0';

        /* Ignore blank lines */
        for(i = 0; linein[i] != '\0'; i++)
        {
            if(linein[i] != '\t' && linein[i] != ' ' && linein[i] != '\n')
                break;
        }
        if(linein[i] == '\0')
            continue;

        if(name_str != NULL)
            TET_FREE((void *) name_str);
        name_str = (char *) TET_MALLOC(strlen(linein) + 5);

        if(num_str != NULL)
            TET_FREE((void *) num_str);
        num_str = (char *) TET_MALLOC(strlen(linein) + 5);

        if(action_str != NULL)
            TET_FREE((void *) action_str);
        action_str = (char *) TET_MALLOC(strlen(linein) + 5);

        if(myline != NULL)
            TET_FREE((void *) myline);
        myline = (char *) TET_MALLOC(strlen(linein) + 5);

        action[0] = '\0';

        /* put a copy of linein into myline so that we can play with it */
        if (linein != (char *) NULL)
            (void) strcpy(myline, linein);

        /* Look for the first field, which should be a number */
        /* This should cope with any leading white spaces in the line */
        if(strpbrk(myline,"0123456789") != (char *) NULL)
            (void) strcpy(myline,strpbrk(myline,"0123456789"));
        else
        {
            ERROR("Bad format line in tet_code file (no first field)\n");
            rc = FAILURE;
            break;
        }

        (void) strcpy(num_str, myline);

        /* Look for the beginning of the second field - shown by a quote */
        if( strchr(myline,'"') != (char *) NULL)
            (void) strcpy(name_str, strchr(myline,'"'));
        else
        {
            ERROR("Bad format line in tet_code file (no first quote)\n");
            rc = FAILURE;
            break;
        }
        num_str[strlen(num_str) - strlen(name_str) +1] = '\0';
        /* num_str should now contain a number followed by some white space */

        /* Get the number - and make sure that we got one */
        num = atoi(num_str);
        if(num == 0 && num_str[0] != '0')
        {
            ERROR("Bad format line in tet_code file (can't get first field num)\n");
            rc = FAILURE;
            break;
        }

        /* Get the end of the second field */
        if(strrchr(name_str+1,'"') != (char *) NULL)
            (void) strcpy(action_str, strrchr(name_str+1,'"'));
        else
        {
            ERROR("Bad format line in tet_code file (no second quote)\n");
            rc = FAILURE;
            break;
        }
        name_str[strlen(name_str)-strlen(action_str) +1] = '\0';

        /*
         *  Look for the third field; the action to take. If it is set
         *  to Abort or Continue then use that value otherwise use the
         *  default action - Continue
         */
        if(strpbrk(action_str,"CA") != (char *) NULL)
            (void) strcpy(action_str,strpbrk(action_str,"CA"));

        if( name != NULL)
            TET_FREE((void *) name);

        name = (char *) TET_MALLOC((size_t)(strlen(name_str)+1));
        (void) strcpy(name, name_str);

        if(action_str == (char *) NULL)
        {
            action[0] = '\0'; /* default action will be assumed */
        }
        else
        {
            (void) sscanf(action_str,"%s", action);
            if(action == (char *) NULL)
            {
                action[0] = '\0'; /* default action will be assumed */
            }
            else if(strcmp(action,"Abort") && strcmp(action,"Continue"))
            {
                action[0] = '\0'; /* default action will be assumed */
            }
        }

        /* Check to see if the number is already in the table */
        if ((idx = rescode_num_to_index(num)) != FAILURE)
        {
	    /* Change the name */
            res_tbl[idx].name = (char *) TET_REALLOC2((void *)res_tbl[idx].name, (size_t) (strlen(name)+1));
            (void) strcpy(res_tbl[idx].name,name);

	    /* Change the action */
            if (action[0] == '\0')
                (void) strcpy(res_tbl[idx].action, DEFAULT_ACTION_NAME);
            else
                (void) strcpy(res_tbl[idx].action, action);
        }
        else
        {
            /* Add an entry to the results codes table */
            if (g_max_rescode >= MAX_RESCODES)
            {
                BAIL_OUT2("rescodes table full\n");
            }
            res_tbl[g_max_rescode].name = (char *) TET_MALLOC2((size_t) (strlen(name)+1));
            (void) strcpy(res_tbl[g_max_rescode].name,name);
            if (action[0] == '\0')
                (void) strcpy(res_tbl[g_max_rescode].action, DEFAULT_ACTION_NAME);
            else
                (void) strcpy(res_tbl[g_max_rescode].action, action);
            res_tbl[g_max_rescode].num = num;
            g_max_rescode++;
        }
    }

    TET_FREE((void *)name_str);
    TET_FREE((void *)num_str);
    TET_FREE((void *)action_str);
    TET_FREE((void *)myline);
    TET_FREE((void *)name);


    for(i = 0; res_tbl[i].name != NULL; i++)
    {
        DBUG(DBGFINFO) (stderr, "%-5d %-15s %s\n", res_tbl[i].num, res_tbl[i].name, res_tbl[i].action);
    }


    return(rc);
}


/*
 *  Pass it a number and it will return the corresponding action for that number
 *  in the results code table if one exists otherwise return an empty string
 */
char *rescode_num_to_action(num)
int num;
{
    int  ctr;
    char *rptr = (char *) NULL;

    for (ctr = 0; (ctr < MAX_RESCODES); ctr++)
    {
        if (res_tbl[ctr].num == num)
	{
	    rptr = res_tbl[ctr].action;
	    break;
	}
    }

    if( rptr == (char *) NULL)
        return("");
    else
	return(rptr);
}


/*
 *  Pass it a name and it will return the corresponding number for that name
 *  in the results code table if one exists otherwise return FAILURE.
 */
int rescode_name_to_num(name)
char *name;
{
    int ctr;

    for (ctr = 0 ; (ctr < MAX_RESCODES) ; ctr++)
    {
        if (! strcmp(res_tbl[ctr].name, name))
            return(res_tbl[ctr].num);
    }
    return(FAILURE);
}


/* 
 *  Pass it a number and it will look for it in the results code table and
 *  and return its index number if it exists or else FAILURE.
 */
int rescode_num_to_index(num)
int num;
{
    int ctr;

    for (ctr = 0 ; (ctr < MAX_RESCODES) ; ctr++)
        if (num == res_tbl[ctr].num)
            return(ctr);
    return(FAILURE);
}


char *rescode_num_to_name(num)
int num;
{
    int ctr;
    static char no_quotes[128];

    for (ctr = 0 ; (ctr < MAX_RESCODES) ; ctr++)
        if (res_tbl[ctr].num == num)
        {
            (void) strcpy(no_quotes, res_tbl[ctr].name);
            *(strrchr(no_quotes,'"')) = '\0';
            return(&(no_quotes[1]));
        }
    return((char *) NULL);
}
