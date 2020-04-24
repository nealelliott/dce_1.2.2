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
static char sccsid[] = "@(#)generic.c	1.6 (92/12/07) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)generic.c    1.6 12/7/92
NAME:		generic.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   May 1992
CONTENTS: 	generic functions called during the modes of the tcc.

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


int server_ok = TRUE; /* set to false when it unexpectantly dies */



/*
 *  Make a source directory pathname. If the remote flag is set make it
 *  for the system specified by sysid.
 */
void tet_srcpath(source_path,tc_dir,remote,sysid)
char *source_path;
char *tc_dir;
int  remote;
int  sysid;
{
	char *ts_root;

	DBUG(DBGFDESC) (stderr,"tet_srcpath(*source_path,%s,remote:%d,sysid:%d)\n",tc_dir, remote, sysid);

	ts_root = get_ts_root(remote, sysid);
	(void) strcpy(source_path, ts_root);
	cat_path(source_path, tc_dir);
}

void tet_exepath(source_path,tc_dir,remote,sysid)
char *source_path;
char *tc_dir;
int  remote;
int  sysid;
{
	char *alt_exec_dir;

	DBUG(DBGFDESC) (stderr,"tet_exepath(*source_path,%s,remote:%d,sysid:%d)\n",tc_dir, remote, sysid);

	alt_exec_dir = get_alt_exec_dir(remote, sysid);
	if(alt_exec_dir != NULL)
	{
		(void) strcpy(source_path, alt_exec_dir);
		cat_path(source_path, tc_dir);
	}
	else
	{
		tet_srcpath(source_path, tc_dir, remote, sysid);
	}
}


/*
 *  Change directory to the given path. If the remote flag is set change
 *  directory on the system given by sysid.
 *
 *  Note: no BAIL_OUTS are allowed in this function.
 */
int tet_chdir(path,remote,sysid)
char *path;
int  remote;
int  sysid;
{
	extern int tc_chdir();
	int ret_val;

	DBUG(DBGFDESC)(stderr,"tet_chdir(%s,remote:%d,sysid:%d)\n",path,remote,sysid);

	if(remote == FALSE || sysid == MASTER)
	{
		ret_val = chdir(path);
	}
	else
	{
		if(server_dead() == TRUE)
			ret_val = FAILURE;
		else
			ret_val = tc_chdir(sysid,path);
	}

	if(ret_val == FAILURE)
	{
	    (void) sprintf(error_mesg,"Change directory to %s failed.\n", path);
	    ERROR(error_mesg);
	}

	return(ret_val);
}

/* Function to make a directory on the local or remote system depending on
 * value of the 'remote' flag
 */
int tet_mkdir(path, remote, sysid)
char *path;
int remote;
int sysid;
{
	int ret_val;
	extern int tc_mkdir();

	DBUG(DBGFDESC)(stderr,"tet_mkdir(%s,remote:%d,sysid:%d)\n",path,remote,sysid);

	if( (remote == FALSE) || (sysid == MASTER) )
	{
		ret_val = mkdir(path, (S_IRWXU|S_IRWXG|S_IRWXO));
	}
	else
	{
		if(server_dead() == TRUE)
			ret_val = FAILURE;
		else
			ret_val = tc_mkdir(sysid, path);
	}

	return(ret_val);
}

/*
 *  Note: no BAIL_OUTS are allowed in this function.
 */
int tet_unlink(path, remote, sysid)
char *path;
int remote;
int sysid;
{
	int ret_val;
	extern int tc_unlink();

	DBUG(DBGFDESC)(stderr,"tet_unlink(%s,remote:%d,sysid:%d)\n",path,remote,sysid);

	if( (remote == FALSE)
	     ||
	    (sysid == MASTER) )
	{
		ret_val = unlink(path);
	}
	else
	{
		if(server_dead() == TRUE)
			ret_val = FAILURE;
		else
			ret_val = tc_unlink(sysid, path);
	}
	return(ret_val);
}

/*
 *  Note: no BAIL_OUTS are allowed in this function.
 */
int tet_rmdir(path, remote, sysid)
char *path;
int remote;
int sysid;
{
	int ret_val;
	extern int tc_rmdir();

	DBUG(DBGFDESC)(stderr,"tet_rmdir(%s,remote:%d,sysid:%d)\n",path,remote,sysid);

	if( (remote == FALSE) || (sysid == MASTER) )
	{
		ret_val = rmdir(path);
	}
	else
	{
		if(server_dead() == TRUE)
			ret_val = FAILURE;
		else
			ret_val = tc_rmdir(sysid, path);
	}
	return(ret_val);
}



/* Export a particular environment variable to a particular system. */
int tet_putenv(envstr, remote, sysid)
char *envstr;
int remote;
int sysid;
{
	int ret_val;
	extern int tc_putenv();

	DBUG(DBGFDESC)(stderr,"tet_putenv(%s,remote:%d,sysid:%d)\n",envstr,remote,sysid);

	if( (remote == FALSE) || (sysid == MASTER) )
	{
		ret_val = put_tcenv(envstr);
	}
	else
	{
		if(server_dead() == TRUE)
			ret_val = FAILURE;
		else
			ret_val = tc_putenv(sysid, envstr);
	}
	return(ret_val);
}

int tet_open(path, remote, sysid)
char *path;
int remote;
int sysid;
{
	int ret_val;
	extern int tc_open();

	DBUG(DBGFDESC)(stderr,"tet_open(%s,remote:%d,sysid:%d)\n",path,remote,sysid);

	if( (remote == FALSE) || (sysid == MASTER) )
	{
    		ret_val = open(path,(O_WRONLY|O_CREAT|O_TRUNC),(S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH));
		DBUG(DBGFILES)(stderr,"open(%d)\n", (int) ret_val);
	}
	else
	{
		/* Do remote 'open' on remote system */
		if(server_dead() == TRUE)
			ret_val = FAILURE;
		else
		{
			ret_val = tc_fopen(sysid, path);
	    		DBUG(DBGFILES)(stderr,"tc_open(%d)\n", (int) ret_val);
		}
	}

	DBUG(DBGFINFO)(stderr,"file no : %d\n", ret_val);

	return(ret_val);
}

/* Close a file on a local or remote system */
int tet_close(fid, remote, sysid)
int fid;
int remote;
int sysid;
{
	int ret_val;
	extern int tc_close();

	DBUG(DBGFDESC)(stderr,"tet_close(fid:%d,remote:%d,sysid:%d)\n",fid,remote,sysid);

	if( (remote == FALSE) || (sysid == MASTER) )
	{
		DBUG(DBGFILES)(stderr,"close(%d)\n", (int) fid);
		ret_val = close(fid);
	}
	else
	{
		if(server_dead() == TRUE)
			ret_val = FAILURE;
		else
		{
	    		DBUG(DBGFILES)(stderr,"tc_close(%d)\n", (int) fid);
			ret_val = tc_fclose(sysid, fid);
		}
	}
	return(ret_val);
}

int tet_puts(fid, text, remote, sysid)
int fid;
char *text;
int remote;
int sysid;
{
	int ret_val = FAILURE;
	extern int tc_puts();

	DBUG(DBGFDESC)(stderr,"tet_puts(fid:%d,text,remote:%d,sysid:%d) %s",fid,remote,sysid,text);

	if( (remote == FALSE) || (sysid == MASTER) )
	{
		if( (write(fid, (void *) text, strlen(text)) != FAILURE)
		     ||
		    ((int) write(fid, (void *) "\n", (size_t) 1) != FAILURE) )
		{
			ret_val = SUCCESS;
		}
	}
	else
	{
		if(server_dead() == TRUE)
			ret_val = FAILURE;
		else
			ret_val = tc_puts(sysid, fid, text);
	}
	return(ret_val);
}

int tet_excllock(lock_name, timeout, remote, sysid)
char *lock_name;
int timeout;
int remote;
int sysid;
{
	int ret_val = FAILURE;
	char *lock_filename = NULL;
	extern int tc_lockfile();

	DBUG(DBGFDESC)(stderr,"tet_excllock(lock_name:%s, timeout:%d, remote:%d, sysid:%d)\n",lock_name,timeout,remote,sysid);

	if( (remote == FALSE) || (sysid == MASTER) )
	{
    		lock_filename = lockfile(lock_name, EXCLUSIVE, timeout);
		if(lock_filename != NULL)
		{
			TET_FREE(lock_filename);
			ret_val = SUCCESS;
		}
	}
	else
	{
		if(server_dead() == TRUE)
			ret_val = FAILURE;
		else
			ret_val = tc_lockfile(sysid, lock_name, timeout);
	}

	return(ret_val);

}


char *tet_sharedlock(lock_name, timeout, remote, sysid)
char *lock_name;
int timeout;
int remote;
int sysid;
{
	char *lock_filename = NULL;
	extern char *tc_sharelock();

	DBUG(DBGFDESC)(stderr,"tet_sharedlock(lock_name:%s, timeout:%d, remote:%d, sysid:%d)\n",lock_name,timeout,remote,sysid);

	if( (remote == FALSE) || (sysid == MASTER) )
	{
    		lock_filename=lockfile(lock_name, SHARED, timeout);
	}
	else
	{
		if(server_dead() != TRUE)
			lock_filename=store_str(tc_sharelock(sysid,lock_name,timeout));
	}

	return(lock_filename);
}

/* Call relevent function to transfer files to be saved. */
void tet_savefiles(path, l_save_files, transfer, remote, sysid)
char *path;
char **l_save_files;
int transfer;
int remote;
int sysid;
{
	int num_files;
	extern int tc_tsmfiles();
	extern int tc_tslfiles();
	extern char *ptrepcode();
	extern int tc_errno;

	DBUG(DBGFDESC)(stderr,"tet_savefiles(%s, l_save_files:%ld, remote:%d, sysid:%d)\n",path, (long)l_save_files, remote, sysid);

	if(l_save_files == (char **)0)
	{
		DBUG(DBGFINFO)(stderr,"tet_savefiles() called will l_save_files=0.\n");
		return;
	}

	if( (remote == FALSE) || (sysid == MASTER) )
	{
		do_save(path, l_save_files);
	}
	else
	{
		if(server_dead() != TRUE)
		{
			for(num_files=0; l_save_files[num_files] != NULL; num_files++) ;

			if(transfer == TRUE)
			{
				DBUG(DBGFINFO)(stderr,"tc_tsmfiles(%03d,%ld,%d,%s)\n", sysid, l_save_files, num_files, path);
				if(tc_tsmfiles(sysid, l_save_files, num_files, path) == FAILURE)
				{
					(void) sprintf(error_mesg, "Error in tc_tsmfiles(). tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
					ERROR(error_mesg);
				}
			}
			else
			{
				DBUG(DBGFINFO)(stderr,"tc_tslfiles(%03d,%ld,%d,%s)\n", sysid, l_save_files, num_files, path);
				if(tc_tslfiles(sysid, l_save_files, num_files, path) == FAILURE)
				{
					(void) sprintf(error_mesg, "Error in tc_tslfiles(). tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
					ERROR(error_mesg);
				}
			}
		}
	}
}


char *tet_mktmpdir(temp_dir_root, remote, sysid)
char *temp_dir_root;
int remote;
int sysid;
{
	char *tetroot;
        char *temp_dir;
	char *temp_dir_leaf;
	char *path = NULL;
	int ret_val = SUCCESS;
	extern char *tc_mktmpdir();

	DBUG(DBGFDESC)(stderr, "tet_mktmpdir(temp_dir_root:%s, remote:%d, sysid:%d)\n", (temp_dir_root==NULL)? "Null":temp_dir_root, remote, sysid);

	temp_dir = (char *)TET_MALLOC(sizeof(char) * (_POSIX_PATH_MAX));

	if( (temp_dir_root == NULL) || (*temp_dir_root == '\0') )
	{
		tetroot = get_tet_root(remote, sysid);
		(void) strcpy(temp_dir, tetroot);
		ret_val = make_path(temp_dir,"tet_tmp_dir", remote, sysid);
	}
	else
	{
		(void) strcpy(temp_dir, temp_dir_root);
	}


	if(ret_val == FAILURE)
	{
		TET_FREE(temp_dir);
		temp_dir = NULL;
	}
	else
	{
		if( (remote == FALSE) || (sysid == MASTER) )
		{
			temp_dir_leaf = (char *)TET_MALLOC(sizeof(char) * 10);
			(void)sprintf(temp_dir_leaf, "%.5d", (int)getpid());
			ret_val = make_unique_dir(temp_dir, temp_dir_leaf, DIR_ALPHA, FALSE, MASTER);
			if(ret_val == FAILURE)
			{
				TET_FREE(temp_dir);
				temp_dir = NULL;
			}
			TET_FREE(temp_dir_leaf);
		}
		else
		{
			if(server_dead() == TRUE)
				temp_dir = NULL;
			else
			{
				path = tc_mktmpdir(sysid, temp_dir);

				TET_FREE(temp_dir);
				temp_dir = NULL;

				temp_dir = store_str(path);
			}
		}
	}
	return(temp_dir);
}


int tet_access(path, mode, remote, sysid)
char *path;
int mode;
int remote;
int sysid;
{
	int ret_val;
	extern int tc_access();

	DBUG(DBGFDESC)(stderr, "tet_access(path:%s, mode:%d, remote:%d, sysid:%d)\n", path, mode, remote, sysid);

	if( (remote == FALSE) || (sysid == MASTER) )
	{
		ret_val = access(path, mode);
	}
	else
	{
		if(server_dead() == TRUE)
			ret_val = FAILURE;
		else
			ret_val = tc_access(sysid, path, mode);
	}

	return(ret_val);
	
}


/*
 *  Is the server active ?
 */
int server_dead()
{
	if(server_ok != TRUE)
	{
		DBUG(DBGFINFO) (stderr,"server dead.\n");
		return(TRUE);
	}
	else
		return(FALSE);
}
