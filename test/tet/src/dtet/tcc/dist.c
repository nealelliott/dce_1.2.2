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
static char sccsid[] = "@(#)dist.c	1.6 (93/01/24) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)dist.c    1.6 1/24/93
NAME:		dist.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 

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


char tet_root[_POSIX_PATH_MAX];
static int xresd = NOT_LOGGED_ON;
static int syncd = NOT_LOGGED_ON;


int dtcc(num_systems, systems, argc, argv)
int num_systems;
struct sysid_store *systems;
int argc;
char **argv;
{
	DBUG(DBGFDESC) (stderr,"dtcc(num_systems:%d, systems:%ld, argc:%d, argv:%ld)\n", num_systems, (long)systems, argc, (long)argv);

	if(num_systems == 0)
		return(SUCCESS);

	(void) strcpy(tet_root, get_tet_root(FALSE, MASTER));

        if(start_dtcc(argc, argv) != SUCCESS)
		return(FAILURE);

	if(start_syncd() != SUCCESS)
		return(FAILURE);

        if(start_xresd() != SUCCESS)
		return(FAILURE);

        if(start_tccd(num_systems,systems) != SUCCESS)
		return(FAILURE);

        /* Send and receive configuration variables to and from the remote
         * system.
         */
        config_systems(num_systems,systems);

        /* Instruct the remote systems to make a results directory and make
         * a list of all the remote results directory names.
         */
	if(make_rem_results(num_systems, systems) != SUCCESS)
		return(FAILURE);

        /* Make a temporary directory on each remote system. The directory
         * name is added to the 'dis_env'.
         */
        if(make_rem_tmpdir(num_systems, systems) != SUCCESS)
		return(FAILURE);

	return(SUCCESS);
}

int start_dtcc(argc, argv)
int  argc;
char **argv;
{
	extern int tccinit();
	extern int tc_errno;
	extern char * ptrepcode();

	DBUG(DBGFDESC) (stderr,"start_dtcc()\n");

	if (tccinit(argc, argv) != SUCCESS)
	{
		ERROR("Could not initialise transport for remote systems.\n");
		DBUG(DBGFINFO)(stderr, "Error in tccinit(). tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
		return(FAILURE);
	}

	return(SUCCESS);
}


int start_syncd()
{
	extern int sd_start();
	extern int sd_logon();
	extern int tc_errno;
	extern char * ptrepcode();

	DBUG(DBGFDESC) (stderr,"start_syncd()\n");

	if(sd_start() != SUCCESS)
	{
		ERROR("Could not initialise with SYNCD.\n");
		DBUG(DBGFINFO)(stderr, "Error in sd_start. tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
		return(FAILURE);
	}

	if(sd_logon() != SUCCESS)
	{
		ERROR("Could not logon to SYNCD.\n");
		DBUG(DBGFINFO)(stderr, "Error in sd_logon. tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
		return(FAILURE);
	}

        syncd = LOGGED_ON;
	return(SUCCESS);
}


int start_xresd()
{
	char exe_cfg_name[_POSIX_PATH_MAX];
	char dis_cfg_name[_POSIX_PATH_MAX];
	char com_cfg_name[_POSIX_PATH_MAX]; 

	extern int xd_start();
	extern int xd_logon();
	extern int tc_errno;
	extern char * ptrepcode();

	DBUG(DBGFDESC) (stderr,"start_xresd()\n");

	if(xd_start(get_results_dir(FALSE, MASTER)) != SUCCESS)
	{
		ERROR("Could not initialise with XRESD.\n");
		DBUG(DBGFINFO)(stderr, "Error in xd_start. tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
		return(FAILURE);
	}

	if (xd_logon() != SUCCESS)
	{
		ERROR("Could not logon to XRESD.\n");
		DBUG(DBGFINFO)(stderr, "Error in xd_logon. tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
		return(FAILURE);
	}

	(void) strcpy(exe_cfg_name, get_ts_root(FALSE, MASTER));	
	cat_path(exe_cfg_name, "tetexec.cfg");
	(void) strcpy(dis_cfg_name, get_temp_dir(FALSE, MASTER));	
	cat_path(dis_cfg_name, "DIST");
	(void) strcpy(com_cfg_name, get_temp_dir(FALSE, MASTER));	
	cat_path(com_cfg_name, "COM_VAR");

	if (xd_cfname(exe_cfg_name, dis_cfg_name, com_cfg_name) != SUCCESS)
	{
		ERROR("Could not send configuration filenames to XRESD.\n");
		DBUG(DBGFINFO)(stderr, "Error in xd_cfname. tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
		return(FAILURE);
	}

	xresd = LOGGED_ON;
	return(SUCCESS);
}


/*
 *  Note: no BAIL_OUTS are allowed in this function.
 */
int start_tccd(num_systems, systems)
int  num_systems;
struct sysid_store *systems;
{
	extern int tc_logon();
	int rc;
	int ret_val = SUCCESS;

	DBUG(DBGFDESC) (stderr,"start_tccd(%d,systems)\n", num_systems);

	for (rc = 0; rc < num_systems; rc++)
	{
		/* Don't try and do a log on for MASTER system */
		if(systems[rc].sysid != MASTER)
		{
			DBUG(DBGFINFO) (stderr,"Calling tc_logon(%d)\n", systems[rc].sysid);
			if(systems[rc].logged_on == TRUE)
			{
				DBUG(DBGWARN)(stderr, "Warning: flag indicates that we are already logged on to system %3.3d.\n", systems[rc].sysid);
			}
			else if(tc_logon(systems[rc].sysid) == SUCCESS)
			{

				systems[rc].logged_on = TRUE;
				DBUG(DBGFINFO) (stderr,"tc_logon for %3.3d succeeded\n", systems[rc].sysid);
			}
			else
			{
				(void) sprintf(error_mesg,"Could not logon to system %3.3d.\n", systems[rc].sysid);
				ERROR(error_mesg);
				ret_val = FAILURE;
				break;
			}
		}
	}

	return(ret_val);
}


/*
 *  Note: no BAIL_OUTS are allowed in this function.
 */
void end_tccd(num_systems, systems)
int  num_systems;
struct sysid_store *systems;
{
	extern int tc_logoff();
	int rc;

	DBUG(DBGFDESC) (stderr,"end_tccd()\n");

	for (rc = 0; rc < num_systems; rc++)
	{
		if (systems[rc].logged_on != TRUE)
			continue;

		if (tc_logoff(systems[rc].sysid) == SUCCESS)
		{
			DBUG(DBGFINFO) (stderr,"tc_logoff for system %3.3d succeeded\n", systems[rc].sysid);
		}
		else
		{
			DBUG(DBGWARN)(stderr, "Warning: Could not log off system %3.3d.\n", systems[rc].sysid);
		}

		systems[rc].logged_on = FALSE;
	}
}

/*
 *  Logoff XRESD, SYNCD and each remote system we have connected to 
 *
 *  Note: no BAIL_OUTS are allowed in this function.
 */
void end_dtcc(num_systems, systems)
int num_systems;
struct sysid_store *systems;
{
	extern int sd_logoff();
	extern xd_logoff();

	DBUG(DBGFDESC)(stderr, "end_dtcc(num_systems:%d, systems:%ld)\n", num_systems, (long)systems);

	if(num_systems == 0)
		return;

	end_tccd(num_systems, systems);

	if(xresd == LOGGED_ON)
	{
		xd_logoff();
		xresd = NOT_LOGGED_ON;
	}

	if(syncd == LOGGED_ON)
	{
		sd_logoff(0);
		syncd = NOT_LOGGED_ON;
	}

	return;
}


/* Routine to instruct each remote system to make a results directory.
 * The routine calls rem_results_dir to store the returned path from each
 * system for future reference.
 */
int  make_rem_results(num_systems, systems)
int num_systems;
struct sysid_store *systems;
{
	int count;
	char suffix[4];
	char *results_path;
	char *results_base_dir;
	char *tmp_ptr;
	char *ts_root;
	int ret_val = SUCCESS;
	extern char *tc_mksdir();
	extern int tc_errno;
	extern char * ptrepcode();

	DBUG(DBGFDESC) (stderr,"make_rem_results(num_systems:%d, systems:%ld)\n", num_systems, (long)systems);

	*suffix = '\0';
	if(setmode(BUILD, FALSE))
		(void) strcat(suffix, "b");
	if(setmode(EXEC, FALSE))
		(void) strcat(suffix, "e");
	if(setmode(CLEAN, FALSE))
		(void) strcat(suffix, "c");

	for(count=0; count<num_systems; count++)
	{
		if(systems[count].sysid == MASTER)
			continue;

		ts_root = get_ts_root(TRUE, systems[count].sysid);
		results_base_dir = store_paths(ts_root, "results");

		ret_val = tet_mkdir(results_base_dir,TRUE,systems[count].sysid);
		if(ret_val == FAILURE)
		{
			if(errno == EEXIST)
			{
				ret_val = SUCCESS;
			}
			else
			{
				TET_FREE((void *)results_base_dir);
				break;
			}
		}
		tmp_ptr = tc_mksdir(systems[count].sysid, results_base_dir, suffix);
		TET_FREE((void *)results_base_dir);
		if(tmp_ptr == NULL)
		{
			DBUG(DBGFINFO)(stderr, "Error in tc_mksdir(). tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
			ret_val = FAILURE;
			break;
		}
		else
		{
			results_path = store_str(tmp_ptr);
			(void)rem_results_dir(results_path, systems[count].sysid, TRUE );
		}
	}
	if(ret_val == FAILURE)
	{
		(void)sprintf(error_mesg,"Couldn't make results directory for system %3.3d.\n", systems[count].sysid);
		ERROR(error_mesg);
	}
	return(ret_val);
}

/* Routine to set or retrieve the results directory of a remote system. */
char *rem_results_dir(results_path, sysid, set )
char *results_path;
int sysid;
int set;
{
	struct results_dir 
	{
		int sysid;
		char *results_path;
	};

	static int store_size = 0;
	static int num_dirs = 0;
	static struct results_dir *rem_results;
	char *ret_dir = NULL;
	int count;

	DBUG(DBGFDESC)(stderr, "rem_results_dir(results_path:%s, sysid:%3.3d, set:%s)\n", (results_path == NULL ? "NULL" : results_path), sysid, (set == TRUE)? "TRUE" : "FALSE");

	if(set == TRUE)
	{
		if(results_path != NULL)
		{
			if(store_size == 0)
			{
				store_size = 10;
				rem_results = (struct results_dir *)TET_MALLOC(sizeof(struct results_dir) * (store_size));
			}
			else
			{
				if(num_dirs%10 == 0)
				{
					rem_results = (struct results_dir *)TET_REALLOC((void *)rem_results, sizeof(struct results_dir) * (store_size));
				}
			}
			rem_results[num_dirs].sysid = sysid;
			rem_results[num_dirs].results_path = results_path;
			num_dirs++;
		}
	}
	else
	{
		for(count=0; count<num_dirs; count++)
		{
			if(rem_results[count].sysid == sysid)
			{
				ret_dir = rem_results[count].results_path;
				break;
			}
		}
		
	}
	return(ret_dir);
}
	

/* Make a temporary directory on each of the systems in 'systems'. */
int make_rem_tmpdir(num_systems, systems)
int num_systems;
struct sysid_store *systems;
{
	char env_var[25];
	char *temp_dir;
	char *dir_name;
	int count;
	int ret_val = SUCCESS;

	DBUG(DBGFDESC)(stderr, "make_rem_tmpdir(num_systems:%d, systems:%ld)\n", num_systems, (long)systems);

        for(count=0; (count<num_systems) && (ret_val == SUCCESS); count++)
        {
		/* Ignore if MASTER system. It is dealt with separately. */
        	if(systems[count].sysid == MASTER)
			continue;

		/* Make a temp. directory for each remote system */
		temp_dir = get_temp_dir(TRUE, systems[count].sysid);

		dir_name = tet_mktmpdir(temp_dir, TRUE, systems[count].sysid);
		if(dir_name != NULL)
		{
			(void) sprintf(env_var,"TET_REM%3.3d_TET_TMP_DIR", systems[count].sysid);
			add_tetenv(env_var, dir_name, CFG_PERM_VAL, &dis_env);
			TET_FREE(dir_name);
		}
		else
		{
			(void) sprintf(error_mesg, "Could not create temporary directory for system %3.3d.\n", systems[count].sysid);
			ERROR(error_mesg);
			ret_val = FAILURE;
		}
	}
	return(ret_val);
}
