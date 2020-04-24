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
static char sccsid[] = "@(#)spawn.c	1.4 (92/12/10) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)spawn.c    1.4 12/10/92
NAME:		spawn.c
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


/*
 *  If EXEC_IN_PLACE is flase then we do a recursive copy of the normal
 *  execution directory to the temp directory
 */
char *do_copy(obj_path, tc_dir, parallel, remote, sysid)
char *obj_path;
char *tc_dir;
int parallel;
int remote;
int sysid;
{
    int rc, stat_loc, tries = 0;
    char *new_dir;
    char *new_tmp_dir;
    char object_path[_POSIX_PATH_MAX];
    int count;
    char *temp_dir;
    char **copy_command;
    struct sigaction sigact;
    int letter;

    DBUG(DBGFDESC) (stderr,"do_copy(object_path:%s, tc_dir:%s, parallel:%d, remote:%d, sysid:%d)\n", obj_path, tc_dir, parallel, remote, sysid);

    /* Make a local copy of object path */
    object_path[0] = '\0';
    (void) strcpy(object_path, obj_path);

    /* Make execution temporary directory */

    temp_dir = get_temp_dir(remote, sysid);

    new_tmp_dir = (char *) TET_MALLOC(sizeof(char) * (strlen(temp_dir) + 11));
    new_tmp_dir[0] = '\0';

    letter = (int) 'a';
    (void) sprintf(new_tmp_dir,"%s/%.5d%c", temp_dir, (int)getpid(), (char) letter);
    do
    {
	rc = tet_mkdir(new_tmp_dir, remote, sysid);

	if(rc == FAILURE)
	{
	    if(letter == (int) 'z' || errno != EEXIST)
		break;

	    ++letter;
	    new_tmp_dir[strlen(new_tmp_dir)-1] = letter;
	}
    }
    while( rc == FAILURE);

    if(rc == FAILURE)
    {
	(void) sprintf(error_mesg,"Unable to make %s\n", new_tmp_dir);
	ERROR(error_mesg);
	TET_FREE((void *) new_tmp_dir);
	return((char *) NULL);
    }

    /* get the relative name of the dir we want to cp */
    new_dir = basename(object_path);

    /* need to obtain a lock for the duration of the copy */
    if(get_alt_exec_dir(remote, sysid) != NULL)
    {
        rc = obtain_lock(SHARED, tc_dir, remote, sysid, TRUE);
    }
    else
    {
        rc = obtain_lock(SHARED, tc_dir, remote, sysid, FALSE);
    }
    if( rc == FAILURE)
    {
        jnl_tc_end(exec_ctr++,TET_ESTAT_LOCK,"TC End");
	TET_FREE(new_tmp_dir);
        return(NULL);
    }

    /* 
     *  Something odd happens when you do a recursive copy of a full
     *  path name - hence the change up directory and relative path
     */ 
    (void) tet_chdir("..", remote, sysid);

    copy_command = (char **) TET_MALLOC(sizeof(char *) * 5);
    copy_command[0] = TET_CP_CMD;
    copy_command[1] = TET_CP_ARG;
    copy_command[2] = new_dir;
    copy_command[3] = new_tmp_dir;
    copy_command[4] = (char *) NULL;

    for(tries = 0; tries < 4; tries++)
    {
        DBUG(DBGFINFO) (stderr,"%s ", copy_command[tries]);
    }
    DBUG(DBGFINFO) (stderr,"\n");

    if( (remote == FALSE) || (sysid == MASTER) )
    {
        /* Give fork several chances to work */
        while ((cpid = fork()) == -1 && tries <= 5)
        {
            tries++;
            (void) sleep(5);
        }

        if (cpid == 0)
        {
	    (void) execvp(TET_CP_CMD, copy_command);

	    exit(EXIT_BAD_CHILD);
	}
    }
    else
    {
	cpid = tc_mexec(sysid, TET_CP_CMD, copy_command, NULL);
    }

    BAIL_OUT_ON(cpid,"fork() failed for exec out of place file copying\n");

    /* Wait for the exec()ed child to complete */
    if( (remote == FALSE) || (sysid == MASTER) )
    {
        (void) waitpid((pid_t)cpid,&stat_loc,0);
    }
    else
    {
	(void) tc_wait(sysid,cpid,-1,&stat_loc);
    }

    cpid = 0;

    /* Check to see that the child completed successfully */
    if ((WIFEXITED(stat_loc) == 0) || (WEXITSTATUS(stat_loc) == EXIT_BAD_CHILD))
    {
        if(WIFEXITED(stat_loc) == 0)
            (void) sprintf(error_mesg,"cp: Exit status = FALSE\n"); 
	else
            (void) sprintf(error_mesg,"cp: Internal failure of exec() function\n"); 
        ERROR(error_mesg);
	TET_FREE(new_tmp_dir);
	new_tmp_dir = NULL;
    }
    else
    {
        /* change dir to the newly created dir */
        rc = tet_chdir(new_tmp_dir, remote, sysid);
        BAIL_OUT_ON(rc,"changing to new_tmp_dir.\n");

        rc = tet_chdir(new_dir, remote, sysid);
        BAIL_OUT_ON(rc,"changing to tmp dir created for EXEC_OUT_OF_PLACE");
    }

    TET_FREE((void *) copy_command);

    /* free exclusive lock */
    release_locks();

    return(new_tmp_dir);
}
