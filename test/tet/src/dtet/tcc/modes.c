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
static char sccsid[] = "@(#)modes.c	1.8 (92/12/01) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)modes.c    1.8 12/1/92
NAME:		modes.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions common to all three modes of the tcc

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


extern char zeetime[];     /* Time array */

sigjmp_buf jmpbuf;         /* this is the allocated jmpbuf */

extern RESCODE res_tbl[];



/*
 *  Export the TET environment. There are several stages:
 *
 *  First, each of the following are written to a file of appropriate name on
 *  the appropriate system. Then an environment variable is set to the full
 *  path of the file and exported to that system.
 *     - The current environment (build, execute or clean).
 *     - The environment variables which were present on the command line
 *     - The result codes.
 *  
 *  Secondly, TET_ACTIVITY is set to the current context and exported.
 *
 *  If an alternate execution directory is defined, set TET_EXECUTE equal to
 *  it and then export that to the environment as well.
 *
 *  Finally, the tet root is exported to the environment via the
 *  variable TET_ROOT
 */
int export(environment, context, remote, sysid)
ENV_T *environment;
int   context;
int   remote;
int   sysid;
{
    int len;
    char *env_line;
    char *alt_exec_dir;
    char *tetroot;

    DBUG(DBGFDESC) (stderr,"export(environment, context:%d, remote:%d, sysid:%d)\n",context, remote, sysid);


    /*
     *  Write the current environment (bld_env/exe_env/cln_env) to a file
     *  "CONFIG" and export a variable "TET_CONFIG" equated to the full
     *  path name of the file.
     */
    write_env_file(environment, "CONFIG", "TET_CONFIG", remote, sysid);



    /* 
     *  Write the result codes to a file "tet_code" and export a variable
     *  "TET_CODE" equated to the full path name of the file.
     *  If this is a remote and this is the master system let xresd know
     *  the value of TET_CODE aswell.
     */
    write_res_file("tet_code", "TET_CODE", remote, sysid);



    /* 
     *  Set the env var TET_ACTIVITY equal to the current context 
     */
    env_line = (char *)TET_MALLOC(strlen("TET_ACTIVITY")+3+40);
    (void) sprintf(env_line,"%s=%d","TET_ACTIVITY",context);
    (void) tet_putenv(env_line, remote, sysid);
    DBUG(DBGFINFO) (stderr,"%s\n", env_line);
    TET_FREE((void *)env_line);



    /*
     *  If alt_exec_dir is defined set TET_EXECUTE equal to it otherwise NULL 
     */
    alt_exec_dir = get_alt_exec_dir(remote, sysid);
    len = ((alt_exec_dir != NULL)? strlen(alt_exec_dir) : 1);
    len += strlen("TET_EXECUTE")+3;
    env_line = (char *)TET_MALLOC((size_t)len);
    (void) sprintf(env_line,"%s=%s","TET_EXECUTE",((alt_exec_dir != NULL)? alt_exec_dir : ""));
    (void) tet_putenv(env_line, remote, sysid);
    DBUG(DBGFINFO) (stderr,"%s\n", env_line);
    TET_FREE((void *)env_line);



    /*
     *  Set the env var TET_ROOT equal to the tet root directory 
     */
    tetroot = get_tet_root(remote, sysid);
    env_line = (char *)TET_MALLOC(strlen("TET_ROOT")+3+strlen(tetroot));
    (void) sprintf(env_line, "%s=%s", "TET_ROOT", (tetroot == NULL) ? "" : tetroot);
    (void) tet_putenv(env_line, remote, sysid);
    DBUG(DBGFINFO) (stderr,"%s\n", env_line);
    TET_FREE((void *)env_line);



    /* 
     *  Set the TET_TIARGS and TET_TSARGS environment variables for the 
     *  master system
     */
    if(remote && (sysid == MASTER))
    {
	    if(ti_tcmputenv(sysid, snid, (long) -1, gsyslist, gnumsys) == FAILURE)
	    {
		ERROR("Failed to successfully place TET_TIARGS in the environment\n");
		return(FAILURE);
	    }
	    else
	    {
		DBUG(DBGFINFO)(stderr, "TET_TIARGS=%s\n", getenv("TET_TIARGS"));
	    }

	    if(ts_tcmputenv() == FAILURE)
	    {
		ERROR("Failed to successfully place TET_TSARGS in the environment\n");
		return(FAILURE);
	    }
	    else
	    {
		DBUG(DBGFINFO)(stderr, "TET_TSARGS=%s\n", getenv("TET_TSARGS"));
	    }
    }
  

    return(SUCCESS);
}


/*
 * Take a scenario line that is due for processing and checking to see
 * whether it matches any of the yes-exec and/or no-exec texts.
 * If there are yes texts then a piece of their text must appear in the
 * line otherwise FAILURE is returned. Any line passing this point is
 * checked for a match against the no texts. A match gives a FAILURE
 * return. Otherwise SUCCESS is returned.
 */
int check_line(query_str)
char *query_str;
{
    int num;
    char *ok_str = NULL;

    DBUG(DBGFDESC) (stderr,"check_line(%s)\n", query_str);

    /* If there is any yes-exec text */
    if(yes_list != NULL)
    {
        for(num = 0; yes_list[num] != NULL; num++)
        {
            if((ok_str = strinstr(query_str, yes_list[num])) != NULL)
                break;
        }
        if(ok_str == NULL)
            return(FAILURE);
    }
    ok_str = NULL;

    /* If there is any no-exec text */
    if(no_list != NULL)
    {
        for(num = 0; no_list[num] != NULL; num++)
        {
            if((ok_str = strinstr(query_str, no_list[num])) != NULL)
                break;
        }
        if(ok_str != NULL)
            return(FAILURE);
    }
    return(SUCCESS);
}


/*
 *  Look to see if the variable is already in the env. If so check if it has
 *  permanent status and if so issue a warning for trying to change it. If
 *  its not permanent change its value. If is not there then add it to the
 *  enviroment.
 */
void add_tetenv(name_p, value_p, perm, tet_env_ad)
char *name_p, *value_p;
int perm;
ENV_T **tet_env_ad;
{
    int env_ctr = 0, tmp_ctr;

    DBUG(DBGFDESC)(stderr,"add_tetenv(name_p:%s, value_p:%s, perm:%s, tet_env_ad:%ld)\n",name_p, value_p, (perm == 1 ? "PERM" : "TEMP"), (long)tet_env_ad);

    if(*tet_env_ad == (ENV_T *)0)
    {
	/* Environment doesn't exist yet. */
	init_env(tet_env_ad);
    }

    /* Look for it in the env. */
    for (env_ctr = 0; !( (*tet_env_ad == NULL) || 
                         ((*tet_env_ad)[env_ctr].name == NULL)); env_ctr++)
    {
        /* replace the value of an existing entry */
        if (strcmp((*tet_env_ad)[env_ctr].name,name_p) == 0)
        {
            if ((*tet_env_ad)[env_ctr].perm == CFG_PERM_VAL)
            {
                /* Naughty - tried to change a permanent value... */
                return;
            }
            else
            {
                (*tet_env_ad)[env_ctr].perm = perm;
                if ((*tet_env_ad)[env_ctr].value != NULL)
                    TET_FREE((void *)(*tet_env_ad)[env_ctr].value);

                if (value_p == NULL)
                    (*tet_env_ad)[env_ctr].value = NULL;
                else
                {
                    (*tet_env_ad)[env_ctr].value = (char *)TET_MALLOC2(strlen(value_p)+1);
                    (void) strcpy((*tet_env_ad)[env_ctr].value,value_p);
                }
                return;
            }
        }
    }

    /* 
     *  check to see if the enviroment is nearly full.
     *  ... or if its exists at all.
     *  This method relies on the ENV_BIGGER constant.
     */
    if(((*tet_env_ad) == NULL) || ((env_ctr % ENV_BIGGER) == (ENV_BIGGER - 1)))
    {

        DBUG(DBGFINFO) (stderr,"About to expand the environment, new size = %d\n",env_ctr + ENV_BIGGER + 1);

        (*tet_env_ad) = (ENV_T *) TET_REALLOC((void *)(*tet_env_ad),
                        (size_t)((env_ctr+ENV_BIGGER+1)*sizeof(ENV_T)));
        tmp_ctr = env_ctr;
        for( ++tmp_ctr; tmp_ctr <= (env_ctr + ENV_BIGGER); tmp_ctr++)
            (*tet_env_ad)[tmp_ctr].name = NULL;
    }

    /* add a new entry to the table */

    (*tet_env_ad)[env_ctr].perm = perm;

    (*tet_env_ad)[env_ctr].name  = (char *)TET_MALLOC2(strlen(name_p)+1);
    (void) strcpy((*tet_env_ad)[env_ctr].name,name_p);

    if (value_p == NULL)
        (*tet_env_ad)[env_ctr].value = NULL;
    else
    {
        (*tet_env_ad)[env_ctr].value = (char *)TET_MALLOC2(strlen(value_p)+1);
        (void) strcpy((*tet_env_ad)[env_ctr].value,value_p);
    }

    return;
}


/*
 *  Look for a variable in the env. If its there, return its value otherwise
 *  return NULL.
 *
 *  Note: no BAIL_OUTS are allowed in this function.
 */
char *get_tetenv(name_p, tet_env)
char *name_p;
ENV_T *tet_env;
{
    char *val = (char *) NULL;
    int env_ctr;

    for( env_ctr = 0; tet_env[env_ctr].name != NULL; env_ctr++)
    {
        if (strcmp(name_p,tet_env[env_ctr].name) == 0)
        {
            val = tet_env[env_ctr].value;
	    break;
        }
    }

    DBUG(DBGFDESC) (stderr,"get_tetenv(%s) value is %s\n", name_p, (val == NULL ? "NULL" : val));

    return(val);
}

ENV_T *get_env_ptr(mode, remote, sysid)
int mode;
int remote;
int sysid;
{
    ENV_T *ret_env;

    DBUG(DBGFDESC) (stderr,"get_env_ptr(mode:%d, remote:%d, sysid:%d)\n", mode, remote, sysid);

    if( (remote == FALSE)
	 ||
	(sysid == MASTER) )
    {
	switch(mode)
	{
	    case BUILD:
		ret_env = bld_env;
		break;
	    case EXEC:
		ret_env = exe_env;
		break;
	    case CLEAN:
		ret_env = cln_env;
		break;
	    default:
		{
		    DBUG(DBGFINFO) (stderr, "Invalid mode in get_env_ptr()\n");
		}
	}
    }
    else
    {
	ret_env = *get_remenv_ptr(sysid, mode);
    }
    return(ret_env);
}


/*
 *  Routine to create an output capture file and redirect stderr and stdout
 *  to that file.
 *
 *  Note that this for the MASTER system only.
 */
void create_oc_file()
{
    int oc_file;

    DBUG(DBGFDESC)(stderr,"create_oc_file()\n");

    /* Create output capture file */
    if((oc_file = open(OC_FILENAME,O_WRONLY|O_CREAT|O_TRUNC,
                       S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)) == FAILURE)
    {
        ERROR("open of output capture file for write failed\n");
    }
    else
    {
	DBUG(DBGFILES)(stderr,"open(%d)\n", (int) oc_file);

        /*
         *  Redirect the stdout and stderr of the tool so that go into
         *  a temporary file.
         */
        ERROR_ON(dup2(oc_file,FILENO_STDOUT), "dup2 oc_file->stdout\n");

	DBUG(DBGFILES)(stderr,"close(%d) via dup2()\n", (int) FILENO_STDOUT);
	DBUG(DBGFILES)(stderr,"open(%d) via dup2() duped fp refers to %d\n", (int) FILENO_STDOUT, (int) oc_file);

        ERROR_ON(dup2(oc_file,FILENO_STDERR), "dup2 oc_file->stderr\n");

	DBUG(DBGFILES)(stderr,"close(%d) via dup2()\n", (int) FILENO_STDERR);
	DBUG(DBGFILES)(stderr,"open(%d) via dup2() duped fp refers to %d\n", (int) FILENO_STDERR, (int) oc_file);

	DBUG(DBGFILES)(stderr,"close(%d)\n", (int) oc_file);
        ERROR_ON(close(oc_file), "close up output capture orig. fd.\n");
    }
}

/* Routine to read the output capture file and write the contents to the
 * journal.
 */
void tool_oc_jnl(remote, sysid)
int remote;
{
    char linein[JNL_LINE_MAX+2];
    FILE *oc_fd;
    char *oc_filename;

    DBUG(DBGFDESC)(stderr,"tool_oc_jnl(remote:%d)\n",remote);

    oc_filename = get_output_file(OC_FILENAME, remote, sysid);

    if(oc_filename == NULL)
	return;

    if( (oc_fd = fopen(oc_filename,"r")) == NULL )
    {
        (void)sprintf(error_mesg,"Could not open output capture file \"%s\" for reading.\n", oc_filename);
        ERROR(error_mesg);
    }
    else
    {
	DBUG(DBGFILES)(stderr,"fopen(%ld)\n", (long) oc_fd);

        while(fgets(linein,sizeof(linein),oc_fd) != NULL)
	{
	    /* Sort out the line if it's too long */
            truncate_line(oc_fd, linein, (JNL_LINE_MAX+1));
	    /*
	     *  If the line was too long we leave 1 byte longer than
	     *  the journal line max so that a warning is generated
	     *  when it is written to the journal file.
	     */

	    jnl_entry_captured(exec_ctr,linein);
	}

	DBUG(DBGFILES)(stderr,"fclose(%ld)\n", (long) oc_fd);
        (void) fclose(oc_fd);
	clear_output_file(oc_filename, remote, sysid);
    }
    return;
}

void exec_oc_jnl(remote, sysid)
int remote;
int sysid;
{
    FILE *oc_fd;
    char linein[JNL_LINE_MAX+2];
    char lineout[BIG_BUFFER];
    int n;
    int sequence;
    char *oc_filename;

    DBUG(DBGFDESC)(stderr,"exec_oc_jnl(remote:%d, sysid:%d)\n",remote, sysid);

    oc_filename = get_output_file(OC_FILENAME, remote, sysid);
    if(oc_filename == NULL)
	return;

    if( (oc_fd = fopen(oc_filename,"r")) == NULL )
    {
        (void)sprintf(error_mesg,"Could not open output capture file \"%s\" for reading.\n", oc_filename);
        ERROR(error_mesg);
    }
    else
    {
	DBUG(DBGFILES)(stderr,"fopen(%ld)\n", (long) oc_fd);

        (void) get_time();

        (void) sprintf(lineout,"%d|%d 0 1 %s|IC Start\n",TET_JNL_IC_START, exec_ctr, zeetime);
        (void) jnl_entry(lineout);
        (void) sprintf(lineout,"%d|%d 0 %s|TP Start\n",TET_JNL_TP_START, exec_ctr, zeetime);
        (void) jnl_entry(lineout);
        sequence = 0;

        while(fgets(linein,sizeof(linein),oc_fd) != NULL)
        {

	    /* Sort out the line if it's too long */
            truncate_line(oc_fd, linein, (JNL_LINE_MAX+1));
	    /*
	     *  If the line was too long we leave 1 byte longer than
	     *  the journal line max so that a warning is generated
	     *  when it is written to the journal file.
	     */

	    n = strlen(linein);
	    if(linein[n-1] == '\n')
		linein[n-1] = '\0';
	    (void) sprintf(lineout,"%d|%d 0 0 0 %d|%s\n",TET_JNL_TC_INFO, exec_ctr, ++sequence, linein);
            (void) jnl_entry(lineout);
	}
	DBUG(DBGFILES)(stderr,"fclose(%ld)\n", (long) oc_fd);
	(void) fclose(oc_fd);
	clear_output_file(oc_filename, remote, sysid);
    }
    return;
}


/* Returns the path of a file. If the file is on the remote system, the file
 * is copied to the master system and the full path of the file on the
 * master system is returned otherwise, the filename is expected to be in
 * the current directory and so just the filename is returned.
 */
char *get_output_file(filename, remote, sysid)
char *filename;
int remote;
int sysid;
{
    char *ret_file_path = NULL;
    char *res_dir;
    char *path;
    char sysid_str[4];
    int ret_val;
    extern int tc_rxfile();

    DBUG(DBGFDESC) (stderr, "get_output_file(filename:%s, remote:%d, sysid:%d)\n", filename, remote, sysid);

    if( (remote == TRUE) && (sysid != MASTER) )
    {
	(void) sprintf(sysid_str, "%3.3d", sysid);

	res_dir = get_results_dir(FALSE, MASTER);

	ret_file_path = (char *)TET_MALLOC(sizeof(char)*(strlen(res_dir)+strlen(sysid_str)+strlen(filename)+5));
	(void) strcpy(ret_file_path, res_dir);

	/* Make unique dir on master system to tranfer file to */
	ret_val = make_unique_dir(ret_file_path, sysid_str, DIR_ALPHA, FALSE, MASTER);

	if(ret_val != FAILURE)
	{
	    cat_path(ret_file_path, filename);
	    path = ret_file_path+strlen(res_dir);

	    /* Transfer OC file from remote system */
	    if ((server_dead() == TRUE) || (tc_rxfile(sysid, filename, path) != SUCCESS))
	    {
	        ERROR("Could not transfer remote file to master system.\n");
		TET_FREE((void *)ret_file_path);
		ret_file_path = NULL;
	    }
	}
	else
	{
	    (void)sprintf(error_mesg, "Failed to make unique directory.\n");
	    ERROR(error_mesg);
	    TET_FREE((void *)ret_file_path);
	    ret_file_path = NULL;
	}
    }
    else
    {
	/* Master system so OC file should be in the current directory */
	ret_file_path = store_str(filename);
    }

    return(ret_file_path);
}


/*
 *  Unlink the temporary results file and then remove the temporary unique 
 *  directory.
 *  Note: This is only done when a remote file has been copied to the master.
 */
void clear_output_file(output_file, remote, sysid)
char *output_file;
int remote;
int sysid;
{
    char *tmp_ptr;

    if((remote == TRUE) && (sysid != MASTER))
    {
        (void)unlink(output_file);
        tmp_ptr = strrchr(output_file, DIR_SEP_CHAR);
        *tmp_ptr = '\0';
        (void)rmdir(output_file);
    }
    TET_FREE(output_file);
}

/* Writes the contents of the environment to a file 'filename' and exports
 * a variable 'env_var', which has been equated with the filename, to the
 * appropriate system.
 */
void write_env_file(environment, filename, var_name, remote, sysid)
ENV_T *environment;
char *filename;
char *var_name;
int remote;
int sysid;
{
    char *path;
    char tmpstr[INPUT_LINE_MAX];
    int fid;
    int ctr;
    char *env_line;
    char *temp_dir;

    DBUG(DBGFDESC)(stderr, "write_env_file(environment, filename:%s, var_name:%s, remote:%d, sysid:%d)\n", filename, var_name, remote, sysid);

    temp_dir = get_temp_dir(remote, sysid);

    /*
     * The file that the configuration variables are to be written to resides
     * under the the "temp_dir".
     */
    path = store_paths(temp_dir, filename);

    fid = tet_open(path, remote, sysid);
    if (fid == FAILURE)
    {
        (void) sprintf(error_mesg,"open config file for export : %s\n", path);
        FATAL_ERROR(error_mesg);
    }

    /* Write all the config variables to the file */
    for (ctr = 0 ; environment[ctr].name != NULL; ctr++)
    {
        if (environment[ctr].value == NULL)
	{
            (void) sprintf(tmpstr,"%s=\n",environment[ctr].name);
	    (void) tet_puts(fid, tmpstr, remote, sysid);
	}
        else
	{
            (void) sprintf(tmpstr,"%s=%s\n",environment[ctr].name,environment[ctr].value);
	    (void) tet_puts(fid, tmpstr, remote, sysid);
	}
    }

    (void) tet_close(fid, remote, sysid);

    /* set the environment var 'var_name' equal to the full config file name */
    env_line = (char *)TET_MALLOC(strlen(var_name)+strlen(path)+2);
    (void) sprintf(env_line,"%s=%s",var_name,(path == NULL) ? "" : path);

    (void) tet_putenv(env_line, remote, sysid);

    DBUG(DBGFINFO) (stderr,"%s\n", env_line);

    TET_FREE((void *)env_line);

    TET_FREE((void *)path);

    return;
}



void write_res_file(filename, var_name, remote, sysid)
char *filename;
char *var_name;
int remote;
int sysid;
{
    char *path;
    char *env_line;
    char tmpstr[INPUT_LINE_MAX];
    int fid;
    int ctr;
    char *temp_dir;
    extern int xd_codesfile();
    static int xresd_done = FALSE;

    DBUG(DBGFDESC)(stderr, "write_res_file(file:%s,var_name:%s,rem:%d,sysid:%d) %d\n", filename, var_name, remote, sysid, g_max_rescode);

    temp_dir = get_temp_dir(remote, sysid);

    /* The file containing the results codes will reside under "temp_dir" */
    path = store_paths(temp_dir, filename);

    fid = tet_open(path, remote, sysid);
    if (fid == FAILURE)
    {
        (void) sprintf(error_mesg,"Failed to open rescodes file for export : %s\n", path);
        FATAL_ERROR(error_mesg);
    }

    /* Write results codes and actions into the file */
    for (ctr = 0 ; (ctr < g_max_rescode) ; ctr++)
    {
        (void) sprintf(tmpstr,"%d\t%s\t%s\n",res_tbl[ctr].num,res_tbl[ctr].name, res_tbl[ctr].action);
	(void) tet_puts(fid, tmpstr, remote, sysid);
    }
    (void) tet_close(fid, remote, sysid);

    /* set the env var TET_CODE equal to the full pathname of the file */
    env_line = (char *)TET_MALLOC(strlen(temp_dir)+3+strlen(var_name)+strlen("/tet_code"));
    (void) sprintf(env_line,"%s=%s%s",var_name,temp_dir,"/tet_code");
    (void) tet_putenv(env_line, remote, sysid);

    DBUG(DBGFINFO) (stderr,"%s\n", env_line);

    /*
     *  If this is a remote and this is the master system then we let the
     *  xresd know what the result codes file name is.
     */
    if(remote && (sysid == MASTER))
    {
	/* Only need to do this once */
	if(xresd_done == FALSE)
	{
	    xresd_done = TRUE;
	    (void) xd_codesfile(path);
	}
    }

    TET_FREE((void *)env_line);
    TET_FREE((void *)path);

    return;
}
