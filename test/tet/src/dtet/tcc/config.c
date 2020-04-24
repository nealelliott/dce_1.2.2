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
static char sccsid[] = "@(#)config.c	1.3 (92/11/27) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)config.c    1.3 11/27/92
NAME:		config.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.9 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions relating to configuration data handling

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


/* The environment pointer */
extern char ** environ;

/*  The table of "pre-initialized" configuration variables */
ENV_T CFG_INIT[4] =
{
    { "TET_OUTPUT_CAPTURE", "False"},
    { "TET_RESCODES_FILE",  "tet_code"},
    { "TET_EXEC_IN_PLACE",  "False"},
    {  NULL,                 NULL    }
};


/*
 *  Initial the configuration variables in the tet env. - removing non-perm
 *  variables and making sure the hard wired ones are there.
 */
void init_config(tet_env_ad)
ENV_T **tet_env_ad;
{
    int ctr = 0;


    DBUG(DBGFDESC) (stderr,"init_config(&");
    if(*tet_env_ad == bld_env)
    {
        DBUG(DBGFDESC) (stderr,"bld)\n");
    }
    else if(*tet_env_ad == exe_env)
    {
        DBUG(DBGFDESC) (stderr,"exe)\n");
    }
    else if(*tet_env_ad == cln_env)
    {
        DBUG(DBGFDESC) (stderr,"cln)\n");
    }


    add_tetenv("TET_VERSION", TET_VERSION, CFG_PERM_VAL,tet_env_ad);

    /* Put the hard wired configuration variables into the tet env. */
    for (ctr = 0; CFG_INIT[ctr].name != NULL; ctr++)
        add_tetenv(CFG_INIT[ctr].name,CFG_INIT[ctr].value,CFG_TEMP_VAL,tet_env_ad);
}


/*
 *  Open the specified configuration file and get the configuration variables
 *  and their values.
 */
void do_config_file(filename, mode, tet_env_ad)
char *filename;
int mode;
ENV_T **tet_env_ad;
{
    FILE *fp;
    char *eq_p, *tmp_ptr = NULL;
    char linein[INPUT_LINE_MAX];
    int linectr = 0;


    DBUG(DBGFDESC) (stderr,"do_config_file(%s, %d)\n", filename, mode);


    if((fp = fopen(filename,"r")) == (FILE *) NULL)
    {
        BAIL_OUT(filename);
    }
    else
    {
	DBUG(DBGFILES)(stderr,"fopen(%ld)\n", (long) fp);
    }

    while (fgets(linein,sizeof(linein),fp) != NULL)
    {
	/* Check for a line that is too big */
	if(strlen(linein) == (INPUT_LINE_MAX - 1))
	{
            (void) sprintf(error_mesg,"File %s line %d. Line too long ! line begins \"%s\"\n", filename,linectr,linein);
            BAIL_OUT(error_mesg);
	}

        /* zap trailing new line */
        if (linein[strlen(linein)-1] == '\n')
            linein[strlen(linein)-1] = '\0'; 

        /* continue on blank or comment line */
        if ( (strlen(linein) == 0) || (linein[0] == CFG_CMNT_CHAR) )
            continue;

        /* if no '=' found, this is a bogus line */
        if ((eq_p = strchr(linein,CFG_SEP_CHAR)) == NULL)
        {
            (void) sprintf(error_mesg,"File %s line %d. No \"%c\", in line \"%s\"\n", filename,linectr,CFG_SEP_CHAR,linein);
            ERROR(error_mesg);
        }
        else
        {
            /* check to see that there is a text value */
            if( (*(eq_p+1) == '\0'))
                continue;
            else
            {
                if(tmp_ptr != NULL)
                    TET_FREE((void *) tmp_ptr);
                tmp_ptr = store_str(eq_p+1);
                if (strtok(tmp_ptr," \t") == NULL)
                    continue;
            }

            /* the line is in a local var.,  no problem with inserting \0
            to separate the var. name from the value */

            *eq_p = '\0';
            add_tetenv(linein,eq_p+1,CFG_TEMP_VAL,tet_env_ad);
        }
        linectr++;
    }

    DBUG(DBGFILES)(stderr,"fclose(%ld)\n", (long) fp);
    (void) fclose(fp);

    /* dump config info to journal file */
    dump_config(filename,mode,*tet_env_ad);

    if(tmp_ptr != NULL)
        TET_FREE((void *) tmp_ptr);

    return;
}


/*
 *  Write the configuration variables and their values into the journal file
 */
void dump_config(filename, mode, tet_env)
char *filename;
int mode;
ENV_T *tet_env;
{
    char msg[INPUT_LINE_MAX];
    int ctr;


    DBUG(DBGFDESC) (stderr,"dump_config(%s, %d, <tet_env>)\n", filename, mode);


    (void) jnl_config_start(filename,mode);

    for (ctr = 0; tet_env[ctr].name != NULL; ctr++)
    {
            (void) sprintf(msg,"%s=%s",tet_env[ctr].name,
                (tet_env[ctr].value == NULL) ? "" : tet_env[ctr].value);

            (void) jnl_config_value(msg);
    }

    (void) jnl_config_end();
}


/*
 * This routine determines the name of the config. file to open.  This
 * is based on the "priority" scheme in the spec.
 *
 * The "priorities" are:
 * 1) user specified file name  - use it (the name was already stored
 *     in *var by the command arg. parser)
 * 2) config. file in alternate exec. dir. - if it exists,  use it
 * 3) config. file in test suite directory
 *
 * This routine just concerns itself with stuffing the name of the file
 * to parse in *var.
 */

void default_cfg_file(var,root,alt_exec_dir,suite,name)
char **var;
char *root;
char *alt_exec_dir;
char *suite;
char *name;
{


    DBUG(DBGFDESC) (stderr, "default_cfg_file(var, %s, %s, %s)\n", root, suite, name);

    if (*var != NULL)  /* if user specified file name */
        return;

    if((alt_exec_dir != NULL) && (*alt_exec_dir != '\0'))
    {
	/* if there is an alt. exec. dir. */
        *var = (char *)TET_MALLOC(strlen(alt_exec_dir)+strlen(name)+4);

	comb_paths(*var, alt_exec_dir, name);

        /* check to see that the file is there */
        if (access(*var,F_OK) == 0)
            return;
        else
            TET_FREE((void *)*var);
    }

    /*
       build up the name of the file in the suite dir, 
       +4 covers the null char. at end,  plus 3 potentially missing
       "dir. sep. char's." (only 2 of which are inserted here)
    */

    *var = (char *)TET_MALLOC2(strlen(root)+strlen(suite)+strlen(name)+4);

    comb_paths(*var, root, suite);
    cat_path(*var, name);

    return;
}

void init_env(tet_env_ad)
ENV_T **tet_env_ad;
{
	int count;

	DBUG(DBGFDESC)(stderr, "init_env(tet_env_ad:%ld)\n", (long)tet_env_ad);

        *tet_env_ad = (ENV_T *)TET_MALLOC2(sizeof(ENV_T) * ENV_BIGGER);

	for(count=0; count<ENV_BIGGER; count++)
		(*tet_env_ad)[count].name = NULL;

	return;
}
