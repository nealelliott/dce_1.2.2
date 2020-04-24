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
static char sccsid[] = "@(#)save.c	1.5 (92/12/10) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)save.c    1.5 12/10/92
NAME:		save.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Miscellaneous fork and exec functions

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


/* Function to make a list of the save files for a particular system
 * environment.
 */
char **get_savefiles(remote, sysid)
int remote;
int sysid;
{
    int ctr;
    char *next_sep;
    char *pres_sep;
    char *save_pat_p;
    char *temp_save;
    char **lsave_files = (char **)0;
    ENV_T *env;

    DBUG(DBGFDESC) (stderr,"get_savefiles(remote:%d, sysid:%d)\n", remote, sysid);

    if( (remote == FALSE) || (sysid == MASTER) )
	env = exe_env;
    else
	env = *get_remenv_ptr(sysid, EXEC);

    /* get the comma seperated list of files that should be saved */
    save_pat_p = get_tetenv("TET_SAVE_FILES",env);

    if(save_pat_p == NULL)
    {
	/* No save files so return */
	return((char **)0);
    }

    temp_save = store_str(save_pat_p);

    lsave_files = (char **) TET_MALLOC((size_t)(sizeof(char *) * 10));

    pres_sep = temp_save;
    ctr = 0;

    do
    {
        if ((next_sep = strchr(pres_sep, NAME_SEP_CHAR)) != NULL)
            *next_sep = '\0';

	lsave_files[ctr++] = store_str(pres_sep);

        if((ctr % 10) == 9)
        {
            lsave_files = (char **) TET_REALLOC((void *)lsave_files, (size_t)(
sizeof(char *) * (ctr + 10)));
        }
        if (next_sep != NULL)
            pres_sep = next_sep + 1;
    }
    while(next_sep != NULL);
    lsave_files[ctr] = NULL;

    TET_FREE(temp_save);
    return(lsave_files);
}

/* Routine to clear the memory used to store the save files */
void clear_savefiles(lsave_files)
char **lsave_files;
{
    int count;

    DBUG(DBGFDESC) (stderr,"clear_savefiles(lsave_files:%ld)\n", (long)lsave_files);

    if(lsave_files == (char **)0)
	return;

    for(count=0; lsave_files[count] != NULL; count++)
	TET_FREE(lsave_files[count]);
    TET_FREE(lsave_files);
}

void savefiles(tc_path, remote, sysid)
char *tc_path;
int remote;
int sysid;
{
	char **lsave_files;
	char *ptr;
	int transfer = TRUE;
	char *savefiles_dir;
	ENV_T *env;

	DBUG(DBGFDESC) (stderr, "savefiles(tc_path:%s, remote:%d, sysid:%d)\n", tc_path, remote, sysid);

        if( (remote == FALSE) || (sysid == MASTER) )
	    env = exe_env;
        else
	    env = *get_remenv_ptr(sysid, EXEC);

	ptr = get_tetenv("TET_TRANSFER_SAVE_FILES", env);
	if( ptr == NULL || *ptr == 'F' || *ptr == 'f' )
	    transfer = FALSE;

	lsave_files = get_savefiles(remote, sysid);
	if(lsave_files != (char **)0)
	{
		savefiles_dir = make_savefiles_dir(tc_path, transfer, remote, sysid);
		if(savefiles_dir == NULL)
		{
			(void) sprintf(error_mesg,"Could not create save files directory for system %03d\n", sysid);
			ERROR(error_mesg);
    		}
		else
		{
			tet_savefiles(savefiles_dir, lsave_files, transfer, remote, sysid);
			/* Try to remove any empty directories */
			remove_savefiles_dir(savefiles_dir, transfer, remote, sysid);
			TET_FREE(savefiles_dir);
		}
		clear_savefiles(lsave_files);
	}
	
	return;
}


/* Routine to make a directory above the results directory in which the
 * save files can be placed.
 */
char *make_savefiles_dir(tc_path, transfer, remote, sysid)
char *tc_path;
int transfer;
int remote;
int sysid;
{
	int tc_name_length;
	char *tc_name;
	char *savefiles_dir;
	char temp_path[_POSIX_PATH_MAX];
	int ret_val;
	char *resdir;

	DBUG(DBGFDESC) (stderr, "make_savefiles_dir(tc_path:%s, transfer:%d, remote:%d, sysid:%d)\n", tc_path, transfer, remote, sysid);

	savefiles_dir = (char *) TET_MALLOC(sizeof(char) * (size_t) _POSIX_PATH_MAX);

	if(transfer == TRUE)
	{
		/* Results are to appear in directories on the master system */
		(void) sprintf(savefiles_dir, "/REMOTE%3.3d", sysid);
    	}
	else
	{
		/* Results are to appear in directories on the remote system. */
		*savefiles_dir = '\0';
	}

	(void) strcat(savefiles_dir, tc_path);

	/* Remove TC name from end of path */
	tc_name = strrchr(savefiles_dir, DIR_SEP_CHAR)+1;
	*(tc_name-1) = '\0';

	/*
	 *  Must shorten a filename to four less than the max
	 *  to allow a unique number to be added if needed.
	 */
	tc_name_length= strlen(tc_name);
	if(tc_name_length > (_POSIX_NAME_MAX -4))
		tc_name[_POSIX_NAME_MAX - 4] = '\0';

	if(sysid == MASTER || transfer == TRUE)
	{
		/* Use results dir on master system */
		resdir = results_dir;
	}
	else if((resdir = rem_results_dir((char *) NULL, sysid, FALSE)) == NULL)
	{
		(void) sprintf(error_mesg, "results dir NULL for system %03d\n", sysid);
		ERROR(error_mesg);
		TET_FREE(savefiles_dir);
		return(NULL);
	}

	(void) strcpy(temp_path, resdir);

	ret_val = make_path( temp_path, savefiles_dir, remote,
			     (transfer == TRUE)? MASTER : sysid);

	if(ret_val == SUCCESS)
	{
		ret_val = make_unique_dir(temp_path,tc_name,DIR_NUMERIC,remote,
				          (transfer == TRUE)? MASTER : sysid);
	}

	if(ret_val == SUCCESS)
	{
		tc_name = strrchr(temp_path, DIR_SEP_CHAR);
		cat_path(savefiles_dir, tc_name);
		return(savefiles_dir);
	}

	TET_FREE(savefiles_dir);
	return(NULL);

}

/* Attempt to remove the directories created by 'make_savefiles_dir'.
 * Directories will only be removed if they are empty.
 */
void remove_savefiles_dir(savefiles_dir, transfer, remote, sysid)
char *savefiles_dir;
int transfer;
int remote;
int sysid;
{
	char *results_path;

	DBUG(DBGFDESC) (stderr, "remove_savefiles_dir(savefiles_dir:%s, transfer:%d, sysid:%d)\n", savefiles_dir, transfer, sysid);

	if(transfer == TRUE)
		results_path = get_results_dir(FALSE, MASTER);
	else
		results_path = get_results_dir(remote, sysid);

	(void) remove_path(results_path, savefiles_dir, remote, sysid);

	return;
}
	


/*
 *  Use a find and cp exec() call to save any files in the comma seperated list,
 *  TET_SAVE_FILES, that are in the execution directory hierarchy; and to do
 *  a recursive copy of them to the results directory
 */
void do_save(savefiles_dir, lsave_files)
char * savefiles_dir;
char **lsave_files;
{
    int rc, stat_loc, cmd_ctr, ctr = 0;
    char **find_command;
    char savefiles_path[_POSIX_PATH_MAX];

    DBUG(DBGFDESC) (stderr,"do_save(savefiles_dir:%s, lsave_files:%ld)\n",savefiles_dir, lsave_files);

    /* if there are no save file patterns */
    if ( lsave_files == (char **)0)
        return;

    (void) strcpy(savefiles_path, get_results_dir(FALSE,MASTER));
    cat_path(savefiles_path, savefiles_dir);

    for (cmd_ctr = 0; lsave_files[cmd_ctr] != NULL; cmd_ctr++)
	;

    /* 3 * the number of names ( -name <name> -o) plus all the other bits */
    find_command = (char **) TET_MALLOC((size_t) (sizeof(char *)*(cmd_ctr*3 + 12)));

    /*
     * We now build up the command we wish to exec(). It will take the form
     * of: `find . (-name expresion -o -name expression ...) -exec 
     *      cp -r {} savefiles_path;`
     */
    cmd_ctr = 0;
    find_command[cmd_ctr++] = "find";
    find_command[cmd_ctr++] = ".";
    find_command[cmd_ctr++] = "(";

    for(ctr=0; lsave_files[ctr] != NULL; ctr++)
    {
        find_command[cmd_ctr++] = "-name";
        find_command[cmd_ctr++] = lsave_files[ctr];
        if( lsave_files[ctr+1] != NULL)
            find_command[cmd_ctr++] = "-o";
    }

    find_command[cmd_ctr++] = ")";
    find_command[cmd_ctr++] = "-exec";
    find_command[cmd_ctr++] = TET_CP_CMD;
    find_command[cmd_ctr++] = TET_CP_ARG;
    find_command[cmd_ctr++] = "{}";
    find_command[cmd_ctr++] = savefiles_path;
    find_command[cmd_ctr++] = ";";
    find_command[cmd_ctr++] = NULL;

    /* This bit is just for debugging */
    {
        int i;
	int old_pid;

        DBUG(DBGFINFO) (stderr,"About to: ");

	/* Don't want child pids to be placed infront of debug messages. */
	old_pid = mypid;
	mypid = 0;

        for( i = 0; find_command[i] != NULL; i++)
        {
            DBUG(DBGFINFO) (stderr,"%s ", find_command[i]);
        }
        DBUG(DBGFINFO) (stderr,"\n");

	mypid = old_pid;
    }


    /* Now we fork and then exec() the find command in the child */
    if ((cpid = fork()) == 0)
    {
        rc = execvp("find",find_command);
        if (rc == FAILURE)
        {
            perror("execvp");
            ERROR("error in find/cp exec\n");
            exit(EXIT_BAD_CHILD);
        }
        exit(EXIT_OK);
    }
    else
    {
        /* Wait to see what happened to the child */
        (void) waitpid((pid_t)cpid,&stat_loc,0);

        cpid = 0;

        /* Check to see that the child exited correctly */
        if ((WIFEXITED(stat_loc) == 0) || (WEXITSTATUS(stat_loc) == EXIT_BAD_CHILD) )
        {
            (void) sprintf(error_mesg,"find IFEXITED %d exit status %d\n",WIFEXITED(stat_loc), WEXITSTATUS(stat_loc)); 
            ERROR(error_mesg);
        }
    }
}
