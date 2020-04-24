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
static char sccsid[] = "@(#)lock.c	1.3 (92/11/27) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)lock.c    1.3 11/27/92
NAME:		lock.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions relating to the locking scheme employed by the tcc

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

static char *where_lock = NULL;     /* The dir to be in, to free any locks */
static char *where_alt_lock = NULL; /* The dir to be in, to free any locks */
static char *sharefile = NULL;      /* Filename of the lock */
int lock_remote;
int lock_sysid;


/*  
 *  Obtain an exclusive lock in the current directory. If at first you don't
 *  succeed keep trying every WAIT_INTERVAL seconds until you time out.
 */
int obtain_lock(l_lock_type, tc_dir, remote, sysid, alt_lock)
int l_lock_type;
char *tc_dir;
int remote;
int sysid;
int alt_lock;
{
	char *exe_root;
	int ret_val;

	DBUG(DBGFDESC) (stderr,"obtain_lock(l_lock_type:%s, tc_dir:%s, remote:%d, sysid:%d, alt_lock:%d)\n",(l_lock_type == EXCLUSIVE)? "EXCLUSIVE":"SHARED", tc_dir, remote, sysid, alt_lock);

	lock_remote = remote;
	lock_sysid = sysid;

	ret_val = lock(LOCK_FILENAME, l_lock_type, gtimeout(), remote, sysid);

	if(alt_lock == FALSE)
		exe_root = get_ts_root(remote, sysid);
	else
		exe_root = get_alt_exec_dir(remote, sysid);

	if(ret_val == FAILURE)
	{
		(void) sprintf(error_mesg,"Could not obtain %s lock in %s%s\n",
			(l_lock_type == EXCLUSIVE ? "exclusive" : "shared"),
			exe_root, tc_dir);
		ERROR(error_mesg);
	}
	else
	{
		if(alt_lock == FALSE)
		{
			where_lock = (char *)TET_MALLOC(sizeof(char) * _POSIX_PATH_MAX);
			comb_paths(where_lock, exe_root, tc_dir);
		}
		else
		{
			where_alt_lock = (char *)TET_MALLOC(sizeof(char) * _POSIX_PATH_MAX);
			comb_paths(where_alt_lock, exe_root, tc_dir);
		}
	}

	return(ret_val);
}


int lock(lock_name, l_lock_type, timeout, remote, sysid)
char *lock_name;
int l_lock_type;
int timeout;
int remote;
int sysid;
{
	int ret_val = FAILURE;

	DBUG(DBGFDESC)(stderr, "lock(lock_name:%s, l_lock_type:%s, timeout:%d, remote:%d, sysid:%d)\n", lock_name, (l_lock_type == SHARED)? "SHARED":"EXCLUSIVE", timeout, remote, sysid);

	if(l_lock_type == EXCLUSIVE)
	{
		ret_val = tet_excllock(lock_name, timeout, remote, sysid);
	}
	else
	{
		sharefile = tet_sharedlock(lock_name, timeout, remote, sysid);

		if(sharefile != NULL)
			ret_val = SUCCESS;
	}

	return(ret_val);
}



int obtain_alt_lock(tc_dir, remote, sysid)
char *tc_dir;
int remote;
int sysid;
{
	char *alt_exec_lock;
	char *alt_exec_dir;
	int ret_val;

	DBUG(DBGFDESC) (stderr,"obtain_alt_lock(tc_dir:%s, remote:%d, sysid:%d)\n", tc_dir, remote, sysid);

	lock_remote = remote;
	lock_sysid = sysid;

	where_alt_lock = (char *)TET_MALLOC(sizeof(char) * _POSIX_PATH_MAX);
	alt_exec_dir = get_alt_exec_dir(remote, sysid);
	comb_paths(where_alt_lock, alt_exec_dir, tc_dir);

	alt_exec_lock = (char *)TET_MALLOC(sizeof(char) * (strlen(where_alt_lock) + strlen(LOCK_FILENAME) + 2));
	comb_paths(alt_exec_lock, where_alt_lock, LOCK_FILENAME);

	ret_val = tet_excllock(alt_exec_lock, gtimeout(), remote, sysid);
	if(ret_val == FAILURE)
	{
       		ERROR("Could not obtain lock for alternate execution directory.\n");
       		TET_FREE((void *)where_alt_lock);
		where_alt_lock = NULL;
	}
	TET_FREE(alt_exec_lock);

	return(ret_val);
}


char *lockfile(lock_name, l_lock_type, timeout)
char *lock_name;
int l_lock_type;
int timeout;
{
    int time_left;
    char *lock_filename;

    DBUG(DBGFDESC) (stderr,"lockfile(lock_name:%s, l_lock_type:%s, timeout:%d)\n", lock_name, (l_lock_type == SHARED)? "SHARED":"EXCLUSIVE" ,timeout);
    
    time_left = timeout;
    time_left += WAIT_INTERVAL; /* the first try is free of charge */

    while(time_left >= WAIT_INTERVAL)
    {
	lock_filename = place_lock(lock_name, l_lock_type);
        if (lock_filename == NULL)
        {
            if (errno == EEXIST || errno == EISDIR)
            {
                if((time_left == timeout+WAIT_INTERVAL) && (timeout != 0))
		{
                    DBUG(DBGFINFO)(stderr,"Lock encountered, retrying until timeout.. \n");
		}
                if(timeout != 0)
		{
                    (void) sleep(WAIT_INTERVAL);
		}
		else
		{
		    return(NULL);
		}
                time_left -= WAIT_INTERVAL;
            }
            else
            {
                return(NULL);
            }
        }
        else 
        {
            if(time_left != timeout + WAIT_INTERVAL) /* ie been waiting */
                (void) fprintf(stderr, "Lock released,  continuing.\n");
            break;
        }
    }
    if(time_left < WAIT_INTERVAL)
    {
        if( timeout != 0)
            ERROR("Timeout on obtaining lock.\n");
        return(NULL);
    }
    return(lock_filename);
}


/*
 *  Tries to create a unique file within the 'lock_subdir'. The name
 *  of the file is made up from the pid number and a letter, if needed.
 */
char *place_lock(lock_name, l_lock_type)
char *lock_name;
int l_lock_type;
{
	int ret_val = FAILURE;
	char letter = 'a';
	char *ret_filename = NULL;

	DBUG(DBGFDESC)(stderr, "place_lock(lock_name:%s, l_lock_type:%s)\n", lock_name, (l_lock_type == SHARED)? "SHARED":"EXCLUSIVE");

	if(l_lock_type == EXCLUSIVE)
	{
       	 	ret_val = open(lock_name, (O_CREAT|O_EXCL), (S_IRUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH));
		if(ret_val != FAILURE)
		{
	    		DBUG(DBGFILES)(stderr,"open(%d)\n", (int) ret_val);

			DBUG(DBGFILES)(stderr,"close(%d)\n", (int) ret_val);
			(void) close(ret_val);
			ret_filename = store_str(lock_name);
		}
	}
	else
	{
		ret_val = do_mkdir(lock_name);
		if(ret_val != SUCCESS)
		{
			(void) sprintf(error_mesg, "Could not create directory %s errno = %d\n",lock_name, errno);
			ERROR(error_mesg);
		}
		else
		{
			ret_filename = (char *)TET_MALLOC(sizeof(char)*(strlen(lock_name) + 11));
			(void) sprintf(ret_filename,"%s/%.5d%c", lock_name, (int)getpid(), (char) letter);
			do
			{
				ret_val = open(ret_filename, (O_CREAT|O_EXCL), (S_IRUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH));
				if(ret_val != FAILURE)
				{
	    				DBUG(DBGFILES)(stderr,"open(%d)\n", (int) ret_val);
					DBUG(DBGFILES)(stderr,"close(%d)\n", (int) ret_val);
					(void) close(ret_val);
				}
				else
				{
					if(errno != EEXIST || letter == (int) 'z')
						break;

					++letter;
					ret_filename[strlen(ret_filename)-1] = letter;
				}
			}
			while(ret_val == FAILURE);

			if(ret_val == FAILURE)
			{
				(void) sprintf(error_mesg,"Could not create %s\n", ret_filename);
				ERROR(error_mesg);
				TET_FREE((void *) ret_filename);
				ret_filename = NULL;
			}
		}
	}

	return(ret_filename);
}


/*
 *  Release the locks. If there is an alternate exe dir the one in there is
 *  freed first. Then the lock in source directory is freed 
 *  NOTE: No BAIL_OUTS are permitted in this function !
 */
void release_locks()
{
	int ret_val;
	char *lock_loc = NULL;

	DBUG(DBGFDESC) (stderr,"release_locks()\n");
	DBUG(DBGFINFO) (stderr,"where_lock=%s\n", (where_lock == NULL ? "NULL" : where_lock));
	DBUG(DBGFINFO) (stderr,"sharefile=%s\n", (sharefile == NULL ? "NULL" : sharefile));
	DBUG(DBGFINFO) (stderr,"where_alt_lock=%s\n", (where_alt_lock == NULL ? "NULL" : where_alt_lock));

	while(where_lock != NULL || where_alt_lock != NULL)
	{
		if(where_lock != NULL)
			lock_loc = where_lock;
		else
			lock_loc = where_alt_lock;

		if(sharefile == NULL)
		{
			ret_val = unlock(LOCK_FILENAME, lock_loc, EXCLUSIVE, lock_remote, lock_sysid);
			if(ret_val != SUCCESS)
			{
				ERROR("Could not release exclusive lock file.\n");
			}
		}
		else
		{
			ret_val = unlock(sharefile, lock_loc, SHARED, lock_remote, lock_sysid);
			if(ret_val != SUCCESS)
			{
				ERROR("Could not release share lock file.\n");
			}
			TET_FREE(sharefile);
			sharefile = NULL;
		}

		if(lock_loc == where_lock)
		{
			TET_FREE(where_lock);
			where_lock = NULL;
		}
		else
		{
			TET_FREE(where_alt_lock);
			where_alt_lock = NULL;
		}
	}
}


/*
 *  Note: no BAIL_OUTS are allowed in this function.
 */
int unlock(lock_name, lock_dir, l_lock_type, remote, sysid)
char *lock_name;
char *lock_dir;
int l_lock_type;
int remote;
int sysid;
{
	int ret_val;
	char lock_path[_POSIX_PATH_MAX];
	char *ptr;

	DBUG(DBGFDESC)(stderr, "unlock(lock_name:%s, lock_dir:%s, l_lock_type:%s, remote:%d, sysid:%d)\n", lock_name, lock_dir, (l_lock_type == SHARED)? "SHARED":"EXCLUSIVE", remote, sysid);

	comb_paths(lock_path, lock_dir, lock_name);

	ret_val = tet_unlink(lock_path, remote, sysid);

	if(l_lock_type == SHARED)
	{
		/*
		 *  Attempt to remove the lock directory.
		 *  EEXIST = unique file still present = OK
		 *  ENOENT = race condition to remove dir = OK.
		 */
		ptr = strrchr(lock_path, DIR_SEP_CHAR);
		*ptr = '\0';
		if ((tet_rmdir(lock_path, remote, sysid) == -1) && (errno != EEXIST) && (errno != ENOENT))
		{
			(void) sprintf(error_mesg,"rmdir of %s failed. errno = %d\n", lock_path, errno);
			ERROR(error_mesg);
		}
	}

	return(ret_val);
	
}
