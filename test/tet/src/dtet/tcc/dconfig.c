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
static char sccsid[] = "@(#)dconfig.c	1.4 (92/12/01) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)dconfig.c    1.4 12/1/92
NAME:		dconfig.c
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

#define VERBOSE		2

static struct rem_env *rem_tetenv = (struct rem_env *)0;
static int num_rem_env=0;


/*
 * For each sysid in the system id list 'systems' except 000:
 *		send the complete list of systems involved
 *		if BUILD then exchange build configuration variables.
 *		if EXEC then exchange exec configuration variables.
 *		if CLEAN then exchange build configuration variables.
 */
void config_systems(num_systems, systems)
int  num_systems;
struct sysid_store *systems;
{
	extern void send_config();
	extern char **rec_config();

	int rc;
	char **conf;
	int nconf = 0;
	int sysid;

	DBUG(DBGFDESC) (stderr,"config_systems(%d,systems)\n", num_systems);

	/* Send system IDs of all the systems to each system. */
	send_sysids(num_systems,systems);

	for (rc = 0; rc < num_systems; rc++)
	{
		sysid = systems[rc].sysid;

		if(sysid == MASTER)
			continue;

		if(ifmode(BUILD))
		{
			conf = form_config(bld_env,&nconf);
			if(nconf > 0)
			{
				send_config(sysid, "tetbuild.cfg", conf,nconf);
				free_config(conf,&nconf);
				conf = rec_config(sysid,&nconf);
				make_rem_tetenv(sysid,BUILD,conf,nconf);
				jnl_rem_config(sysid,0,conf,nconf);
				free_config(conf,&nconf);
			}
		}

		if(ifmode(EXEC))
		{
			/* Remote execution configuration data. */
			conf = form_config(exe_env,&nconf);
			if(nconf > 0)
			{
				send_config(sysid, "tetexec.cfg", conf, nconf);
				free_config(conf,&nconf);
				conf = rec_config(sysid,&nconf);
				make_rem_tetenv(sysid,EXEC,conf,nconf);
				jnl_rem_config(sysid,1,conf,nconf);
				free_config(conf,&nconf);
			}
		}

		if(ifmode(CLEAN))
		{
			/* Remote clean configuration data. */
			conf = form_config(cln_env,&nconf);
			if(nconf > 0)
			{
				send_config(sysid, "tetclean.cfg", conf, nconf);
				free_config(conf,&nconf);
				conf = rec_config(sysid,&nconf);
				make_rem_tetenv(sysid,CLEAN,conf,nconf);
				jnl_rem_config(sysid,2,conf,nconf);
				free_config(conf,&nconf);
			}
		}
	}
}

/*
 *  Form a configuration environment of strings "str=value" from a 
 *  configuration environment of the form {string; value;}.
 *  Returns a pointer to the newly constructed environment or NULL if there
 *  is none to construct.
 */
char **form_config(tet_env,nconf)
ENV_T *tet_env;
int *nconf;
{
	char **conf;
	int count;


	DBUG(DBGFINFO) (stderr,"form_config(tet_env, %d)\n", (*nconf));

	for((*nconf) = 0; tet_env[*nconf].name!=NULL; (*nconf)++)
		;
	if((*nconf) == 0)
	    return(NULL);

	conf = (char **) TET_MALLOC(sizeof(char *)*(*nconf));

	for(count=0; count<(*nconf); count++)
	{
		conf[count] = (char *) TET_MALLOC(sizeof(char) * (strlen(tet_env[count].name) + strlen(tet_env[count].value) + 3));
		(void) sprintf(conf[count],"%s=%s",tet_env[count].name,tet_env[count].value);
	}
	return(conf);
}


/*
 *  Free up the contents of an environment that consists of a list of strings
 *  of the form 'string=value'
 */
void free_config(conf, nconf)
char **conf;
int *nconf;
{
	int count;

	DBUG(DBGFINFO) (stderr,"free_config(tet_env, %d)\n", *nconf);


	if((*nconf) == 0)
		return;

	for(count = 0; count < (*nconf); count++)
	{
		TET_FREE(conf[count]);
	}
	TET_FREE(conf);

	(*nconf) = 0;
}


/*
 *  Make an environment of the form {string; value;} from one of the form
 *  string=value.
 */
void make_rem_tetenv(sysid, mode, conf, nconf)
int sysid;
int mode;
char **conf;
int nconf;
{
	ENV_T **tet_env_ad;
	char *name;
	char *value;
	int count;

	DBUG(DBGFDESC) (stderr,"make_rem_tetenv(%d,%d,conf,%d)\n",sysid,mode,nconf);

	/* If a remote environment structure hasn't already been created
	 * due to a previous call to this function for the same system
	 * ID but for a different mode, we must create one.
	 */
	if( (tet_env_ad = get_remenv_ptr(sysid, mode)) == (ENV_T **)0)
	    tet_env_ad = add_remenv_ptr(sysid, mode);

	/* Allocate memory for pointers to new environment variables */
	init_env(tet_env_ad);

	for(count=0; count < nconf; count++)
	{
		name = store_str(conf[count]);
		if((value = strchr(name,CFG_SEP_CHAR)) == NULL)
		{
			(void) sprintf(error_mesg,"Invalid configuration variable \"%s\" received from remote tccd number %d.\n",sysid);
			BAIL_OUT(error_mesg);
		}

		*value = (char)0;
		value++;

		add_tetenv(name,value,CFG_PERM_VAL,tet_env_ad);
		TET_FREE(name);
	}
}


/*
/*  A function to return a pointer to a remote environment, given the remote
 *  system ID and the mode required.
 *  If the particular environment doesn't exist, the routine will create one.
 *  and return a pointer to it.
 */
ENV_T **add_remenv_ptr(sysid,mode)
int sysid;
int mode;
{
	static int remenv_size = 0;
	ENV_T **return_env_ad = (ENV_T **)0;

	DBUG(DBGFDESC) (stderr,"add_remenv_ptr(sysid:%d, mode:%d)\n",sysid,mode);

	if(remenv_size == 0)
	{
		rem_tetenv = (struct rem_env *)TET_MALLOC2(sizeof(struct rem_env) * 20);
		remenv_size = 20;
		num_rem_env = 0;

	}
	else
	{
		/* Add system to environment list */
		if(num_rem_env%20 == 0)
		{
			remenv_size += 20;
			rem_tetenv = (struct rem_env *)TET_REALLOC((void *) rem_tetenv, sizeof(struct rem_env) * remenv_size);
		}
	}

	rem_tetenv[num_rem_env].sysid = sysid;
	rem_tetenv[num_rem_env].bld_env = (ENV_T *)0;
	rem_tetenv[num_rem_env].exe_env = (ENV_T *)0;
	rem_tetenv[num_rem_env].cln_env = (ENV_T *)0;
	return_env_ad = get_remenv_mode(mode, num_rem_env);
	num_rem_env++;
	return(return_env_ad);
}

ENV_T **get_remenv_ptr(sysid, mode)
int sysid;
int mode;
{
	int remenv_num;

	DBUG(DBGFDESC) (stderr,"get_remenv_ptr(sysid:%d,mode:%d)\n",sysid,mode);

	for(remenv_num=0;remenv_num<num_rem_env;remenv_num++)
	{
		if(rem_tetenv[remenv_num].sysid == sysid)
		{
			return(get_remenv_mode(mode, remenv_num));
		}
	}

	DBUG(DBGFINFO) (stderr,"system ID not found in get_remenv_ptr()\n");

	return((ENV_T **)0);
	
}


ENV_T **get_remenv_mode(mode, remenv_num)
int mode;
int remenv_num;
{
	ENV_T **return_env;

	DBUG(DBGFDESC) (stderr,"get_remenv_mode(mode:%d, remenv_num:%d)\n",mode, remenv_num);

	switch(mode)
	{
		case BUILD:
			return_env = &(rem_tetenv[remenv_num].bld_env);
			break;
		case EXEC:
			return_env = &(rem_tetenv[remenv_num].exe_env);
			break;
		case CLEAN:
			return_env = &(rem_tetenv[remenv_num].cln_env);
			break;
		default:

			DBUG(DBGFINFO) (stderr,"Invalid mode in get_remenv_mode()\n");

			return_env = (ENV_T **)0;
			break;
	}
	return(return_env);
}

void send_sysids(num_systems, systems)
int num_systems;
struct sysid_store *systems;
{
	int *sysids;
	int ret_val = SUCCESS;
	int count;
	extern int tc_sysname();
	extern int tc_errno;
	extern char * ptrepcode();


	sysids = (int *)TET_MALLOC(sizeof(int) * num_systems);

	for(count=0; count<num_systems; count++)
	{
		sysids[count] = systems[count].sysid;
	}
	for(count=0;(count<num_systems) && (ret_val == SUCCESS); count++)
	{
		if(systems[count].sysid != MASTER)
		{
			ret_val = tc_sysname(systems[count].sysid, sysids, num_systems);
			if(ret_val != SUCCESS)
			{
				DBUG(DBGFINFO)(stderr, "Error in tc_sysname(). tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
				(void)sprintf(error_mesg,"Could not send system IDs to remote system %3.3d.\n", systems[count].sysid);
				BAIL_OUT2(error_mesg);
			}
		}
	}
}

/* Send configuration variables to remote system. */
void send_config(sysid, filename, conf, nconf)
int  sysid;
char *filename;
char **conf;
int  nconf;
{
	char conf_path[_POSIX_PATH_MAX];
	extern int tc_cfname();
	extern int tc_sndconfv();
	extern int tc_errno;
	extern char * ptrepcode();

	/* Send configuration filename */

	(void) strcpy(conf_path, get_ts_root(TRUE, sysid));
	cat_path(conf_path, filename);

	if(tc_cfname(sysid, conf_path) != SUCCESS)
	{
		(void)sprintf(error_mesg,"Could not send configuration data to remote system %3.3d.\n", sysid);
		DBUG(DBGFINFO)(stderr, "Error in tc_sndconf(). tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
		BAIL_OUT2(error_mesg);
	}

	/* Send configuration data. */
	if(tc_sndconfv(sysid, conf, nconf) != SUCCESS)
	{
		(void)sprintf(error_mesg,"Could not send configuration data to remote system %3.3d.\n", sysid);
		DBUG(DBGFINFO)(stderr, "Error in tc_sndconf(). tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
		BAIL_OUT2(error_mesg);
	}

	return;
}

/* Receive configuration variables from remote system. */
char **rec_config(sysid, nconf_ad)
int sysid;
int  *nconf_ad;
{
	int nlines;
	int done;
	int count;
	char **conf = (char **) 0;
	char **rec_vars;
	extern char ** tc_rcvconfv();
	extern int tc_errno;
	extern char * ptrepcode();

	do
	{
		rec_vars = tc_rcvconfv(sysid, &nlines, &done);
		if(rec_vars == (char **)0)
		{
			DBUG(DBGFINFO)(stderr, "Error in tc_rcvconfv(). tc_errno = %d (%s).\n", tc_errno, ptrepcode(tc_errno));
			(void)sprintf(error_mesg, "Error receiving configuration variables from system %3.3d.\n", sysid);
			BAIL_OUT2(error_mesg);
		}

		for(count=0; count<nlines; count++)
		{
			conf = store_config(conf, nconf_ad,rec_vars[count]);
		}
	}
	while( !done );

	return(conf);
}

/* Routine to temporarily store configuration variables when exchanging
 * betweeen master and remote systems.
 */
char **store_config(conf,nconf_ad,text)
char **conf;
int  *nconf_ad;
char *text;
{
	static int store_size = 0;

	DBUG(DBGFDESC) (stderr,"store_config(conf,%d,%s)\n",(*nconf_ad),text);

	if((*nconf_ad) == 0)
	{
		store_size = 20;
		conf = (char **) TET_MALLOC(sizeof(char *) * store_size);
	}
	else if((*nconf_ad) == store_size-1)
	{
		store_size += 20;
		conf = (char **) TET_REALLOC((void *) conf, sizeof(char *) * (store_size));
	}

	conf[*nconf_ad] = (char *) TET_MALLOC(sizeof(char) * (strlen(text)+1));
	(void) strcpy(conf[*nconf_ad],text);
	(*nconf_ad)++;
	return(conf);
}
