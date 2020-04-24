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
static char sccsid[] = "@(#)tcc.c	1.8 (92/11/27) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)tcc.c    1.8 11/27/92
NAME:		tcc.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	tcc()

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

int debug_flag = 0;           /* flag used for debugging messages etc */
int mypid = 0;                /* The process id of this process (default 0) */
int cpid = 0;                 /* Child process id */
int jnl_fd = -1;              /* The journal file descriptor */
int interrupted = 0;          /* Has the user generated an interrupt */
int eip_set = FALSE;          /* Is exec in place set */
char error_mesg[BIG_BUFFER];  /* Error message buffer */
long snid = 0;                /* sync id */

int oc_set = FALSE;           /* Has Ouput Capture been set ? */
int scenario_line = 0;        /* line number, used in mode start messages */
int old_line = 0;             /* the scenario line number to resume from */
int scenario_element = 0;     /* element number, used in mode start messages */
int old_element = 0;          /* the scenario element number to resume from */
int group_line = 0;	      /* the line number of the current group direct. */
int group_element = 0;	      /* the element ---------------"---------------- */

char *result_pat = NULL;        /* result code pattern, for -m,-r options */
char *bld_cfg_file_name = NULL; /* build configuration file */
char *cln_cfg_file_name = NULL; /* clean configuration file */
char *exe_cfg_file_name = NULL; /* execution configuration file */
char *dis_cfg_file_name = NULL; /* distributed configuration file */
char *scen_file_name = NULL;    /* scenario file name */
char *suite_name = NULL;        /* actual test suite name */
char *scenario_name = NULL;     /* the scene to look for */
char **yes_list = NULL;         /* Text to match before exeing a scen line */
char **no_list = NULL;          /* Text to match to not exec a scen line */

MEMCHECK *memtable = NULL;      /* Memory allocation checking table */

ENV_T * bld_env = NULL;         /* the build mode environment */
ENV_T * exe_env = NULL;         /* the exe mode environment */
ENV_T * cln_env = NULL;         /* the clean environment */
ENV_T * dis_env = NULL;         /* the distributed environment */
ENV_T * com_env = NULL;         /* the command line environment variables */

FILE * old_jnl_fp;              /* old journal file pointer */

char start_dir[_POSIX_PATH_MAX];          /* The directory we start in */
char results_dir[_POSIX_PATH_MAX];        /* results directory */

/* Ensure a sensible default value */
#ifndef TET_SIG_IGNORE
#define TET_SIG_IGNORE 0
#endif

/* Ensure a sensible default value */
#ifndef TET_SIG_LEAVE
#define TET_SIG_LEAVE 0
#endif

int main(argc,argv)
int argc;
char *argv[];
{
    int exit_val;

    exit_val = tcc(argc, argv);

    DBUG(DBGFINFO) (stderr,"TCC successfully exiting with exit value of %d\n", exit_val);

    exit(exit_val);

    /* NOTREACHED */
}


/*
 *  TCC main routine, a "-?" flag option gives you the correct synopsis
 */
int tcc(argc,argv)
int argc;
char *argv[];
{
    int ch, ctr, num, rc, sig_set;
    int exit_val = EXIT_OK, max_res = 0, user_results_dir = FALSE;
    char msg[256], tmp_dir[_POSIX_PATH_MAX];
    char *line_p, *eq_p, *jnl_dir, *tmp_ptr, *cp;
    bool resume_opt = FALSE, rerun_opt = FALSE;
    int resume_status = 0;
    int * result_list = NULL;
    static int    sig_ignore[] = { 0, TET_SIG_IGNORE};
    static int    sig_leave[]  = { 0, TET_SIG_LEAVE};
    static char *optstring = "a:bcd:ef:g:hi:j:l:m:n:pr:s:T:t:v:x:y:?";
    static int result_mode[3] = {FALSE,FALSE,FALSE};
    char *jnl_file_name = NULL;
    char **scen_lines = NULL;
    struct sigaction sig;
    char old_journal[_POSIX_PATH_MAX];
    extern int optind;
    extern char *optarg;
    char *tetroot;
    char *ts_root;
    char *temp_dir;
    char *alt_exec_dir;
    int  com_scen = FALSE; /* Command line scenario lines */
    char *tmp_scen_file_name = NULL;
    char *tmp_scen_name;
    char *tmp_scen_file;
    extern int remote_tmp_dirs;

    /* 
     *  Initialise the environments.
     */
    init_env(&bld_env);
    init_env(&exe_env);
    init_env(&cln_env);
    init_env(&dis_env);
    init_env(&com_env);

    tetroot = (char *)TET_MALLOC(sizeof(char) * (_POSIX_PATH_MAX));
     
    /*
     *  Initialise tetroot[0] so that we can check later to 
     *  see if tetroot has been set .
     */
    tetroot[0] = '\0';

    ts_root = (char *)TET_MALLOC(sizeof(char) * (_POSIX_PATH_MAX));
    temp_dir = (char *)TET_MALLOC(sizeof(char) * (_POSIX_PATH_MAX));
    alt_exec_dir = (char *)TET_MALLOC(sizeof(char) * (_POSIX_PATH_MAX));

    /* check to see any options were supplied */
    if (argc == 1)
    {
        show_synopsis();
        tet_shutdown();
    }

    /* Sort out the signal handling */
    for( num = 1; num < NSIG; num++)
    {
        sig_set = 0;
        if (num == SIGKILL || num == SIGSTOP || num == SIGCHLD)
            continue;

        for(ctr = 0; (sizeof(sig_leave[0])*ctr) <= sizeof(sig_leave); ctr++)
            if(sig_leave[ctr] == num)
            {
                sig_set = 1;
                break;
            }

        if(sig_set)
            continue;

        for(ctr = 0; (sizeof(sig_ignore[0])*ctr) <= sizeof(sig_ignore); ctr++)
            if(sig_ignore[ctr] == num)
            {
                sig.sa_handler = SIG_IGN;
                sig_set = 1;
                break;
            }


        if( ! sig_set)
	{
	    if (num == SIGHUP || num == SIGINT || num == SIGQUIT)
	    {
		struct sigaction osig;
		if (sigaction(num, (struct sigaction *)NULL, &osig) != -1)
		{
		    if (osig.sa_handler == SIG_IGN)
		    {
			sig.sa_handler = SIG_IGN;
			sig_set = 1;
		    }
		}
	    }
	    if( ! sig_set)
		if (num == SIGINT)
		    sig.sa_handler = siginthdlr;
		else
		    sig.sa_handler = sighdlr;
	}

        sig.sa_flags = 0;
        (void) sigaction(num, &sig, (struct sigaction *)NULL);
    }

    
    /* so that the umask does not effect the creation of files etc */
    (void) umask(000); 

    /* sort out the dir we start off in */
    if( getcwd(start_dir, sizeof(start_dir)) == (char *) NULL)
    {
        perror("getcwd");
        tet_shutdown();
    }

#ifdef TET_ROOT
    /* initialize 'tetroot' from defined value */
    (void) strcpy(tetroot, TET_ROOT);
#endif

    /* 
     *  initialize 'tetroot' from environment var $TET_ROOT
     *  (this takes precedence over any compile-time value)
     */
    if ((line_p = getenv("TET_ROOT")) != NULL)
	(void) strcpy(tetroot, line_p);

    /*
     * Stop if tetroot has not been defined from either source
     */
    if (tetroot[0] == '\0')
    {
        (void) fprintf(stderr, "No TET_ROOT directory defined\n"); 
        tet_shutdown();
    }

    /*
     * Check the accessability of the tetroot...
     */
    if (chdir(tetroot) == -1)
    {
        BAIL_OUT("tetroot check failed ");
    }
    else
    {
        /* get back to where we were */
        BAIL_OUT_ON(chdir(start_dir),"changing back to starting directory.\n");
    }

    /* initialize 'temp_dir' from environment var $TET_TMP_DIR */
    if ((line_p = getenv("TET_TMP_DIR")) != NULL)
	(void) strcpy(temp_dir, line_p);

    /* initialize 'alt_exec_dir' from environment var $TET_EXECUTE */
    if( (line_p = getenv("TET_EXECUTE")) != NULL)
	(void) strcpy(alt_exec_dir, line_p);

    /*
     *  process all command line options.
     *  options used to date:
     *                        a,b,c,d,e,f,g,h,i,j,l,m,n,p,r,s,t,v,x,y,?
     */
    while (( ch = optget(argc,argv,optstring) ) != EOF)
    {
        switch ((char)ch)
        {
            /* The alternate execution directory */
        case 'a':
            /*
             * When the alt_exec_dir is specified by this option we 
             * must make sure it is stored as a full path.
             */
            if(optarg[0] != DIR_SEP_CHAR)
                (void) sprintf(alt_exec_dir,"%s/%s",start_dir,optarg);
            else
                (void) strcpy(alt_exec_dir,optarg);
            break;

            /* Build mode */
        case 'b':
	    (void) setmode(BUILD, TRUE);
            break;

            /* Clean mode */
        case 'c':
	    (void) setmode(CLEAN, TRUE);
            break;

            /* Enable debugging break points */
        case 'd':
	    if(*optarg == 'T')
	    {
		/* debug is for -T option so overwrite 'd' with 'T' */
		(void) strcpy(optarg-1, optarg);
	    }
	    else
	    {
	        debug_flag = atoi(optarg);
	    }
            break;

            /* Exec mode */
        case 'e':
	    (void) setmode(EXEC, TRUE);
            break;

            /* Specific clean config file */
        case 'f':
            cln_cfg_file_name = store_str(optarg);
            break;

            /* Specific build config file */
        case 'g':
            bld_cfg_file_name = store_str(optarg);
            break;

            /* Intermediate Results Dir. (working dir.) */
        case 'i':
            (void) strcpy(results_dir,optarg);
            user_results_dir = TRUE;
            break;

            /* journal file name */
        case 'j':
            jnl_file_name = store_str(optarg);
            break;

            /* use this specific scenario line */
        case 'l':
            if( scen_lines == NULL)
            {
                scen_lines = (char **) TET_MALLOC((size_t)(sizeof(char *)*10));
                for(num = 0; num < 10; num++)
                    scen_lines[num] = NULL;
            }
            for(num = 0; scen_lines[num] != NULL; num++)
		;
            if((num % 10) == 9)
            {
                scen_lines = (char **) TET_REALLOC((void *)scen_lines, (size_t)(sizeof(char *)*(num+11)));
                for(ctr = num; ctr < (num + 11); ctr++)
                    scen_lines[ctr] = NULL;
            }
            scen_lines[num] = store_str(optarg);
	    com_scen = TRUE;
            break;

            /*  Resume option, record the result codes to look for and
             *  note that we need to look for a specific test case before
             *  processing the rest of the scenario file.
             */
        case 'm':
            resume_opt = TRUE;
            resume_status = 1; 					/* active */
            result_pat = store_str(optarg);
            break;

            /* If this text occurs in a scenario line don't exec it */
        case 'n':
            if( no_list == NULL)
            {
                no_list = (char **) TET_MALLOC((size_t)(sizeof(char *)*10));
                for(num = 0; num < 10; num++)
                    no_list[num] = NULL;
            }
            for(num = 0; no_list[num] != NULL; num++);
            if((num % 10) == 9)
            {
                no_list = (char **) TET_REALLOC((void *)no_list, (size_t)(sizeof(char *)*(num+11)));
                for(ctr = num; ctr < (num + 11); ctr++)
                    no_list[ctr] = NULL;
            }
            no_list[num] = store_str(optarg);
            break;

            /* Produce minimal output to give the user something to look at */
        case 'p':
            (void) mode_message(SETVALUE);
            break;

            /* Rerun option, record the result codes to look for */
        case 'r':
            rerun_opt = TRUE;
            result_pat = store_str(optarg);
            break;

            /* scenario file name */
        case 's':
            scen_file_name = store_str(optarg);
            break;

        case 'T':
            /* Used by distributed tools - ignored by the tcc */
            break;

            /* timeout, in seconds */
        case 't':
            (void) set_gtimeout(atoi(optarg), TRUE);
            break;

            /* Define a configuration variable on the command line */
        case 'v':
            line_p = store_str(optarg);
            eq_p = strchr(line_p,CFG_SEP_CHAR);
            if (eq_p == NULL)
            {
                (void) fprintf(stderr,"-v option requires %c\n",CFG_SEP_CHAR);
                TET_FREE((void *)line_p);
                continue;
            }
            /* delimit variable name */
            *eq_p = '\0'; 
            if( *(eq_p+1) != '\0')
            {
                /* put the variable into each of the environments */
                add_tetenv(line_p,eq_p+1,CFG_PERM_VAL,&bld_env);
                add_tetenv(line_p,eq_p+1,CFG_PERM_VAL,&exe_env);
                add_tetenv(line_p,eq_p+1,CFG_PERM_VAL,&cln_env);
		add_tetenv(line_p,eq_p+1,CFG_PERM_VAL,&com_env);
            }
            TET_FREE((void *)line_p);
            break;

            /* Specific execution config file */
        case 'x':
            exe_cfg_file_name = store_str(optarg);
            break;

            /* Only exec a scenario line if this text occurs in it */
        case 'y':
            if( yes_list == NULL)
            {
                yes_list = (char **) TET_MALLOC((size_t)(sizeof(char *)*10));
                for(num = 0; num < 10; num++)
                    yes_list[num] = NULL;
            }
            for(num = 0; yes_list[num] != NULL; num++);
            if((num % 10) == 9)
            {
                yes_list = (char **) TET_REALLOC((void *)yes_list, (size_t)(sizeof(char *)*(num+11)));
                for(ctr = num; ctr < (num + 11); ctr++)
                    yes_list[ctr] = NULL;
            }
            yes_list[num] = store_str(optarg);
            break;

            /* print a synopsis for the TCC */
        case '?':
	case 'h':
        default:
            show_synopsis();
            tet_shutdown();
        }
    }

    /* Must specify at least one mode */
    if( !ifmode(BUILD) && !ifmode(EXEC) && !ifmode(CLEAN))
    {
        (void) fprintf(stderr,"Must specify at least one mode of operation.\n");
        tet_shutdown();
    }

    /* Not allowed to have both resume and rerun enabled */
    if ( (resume_opt == TRUE) && (rerun_opt == TRUE) )
    {
        (void) fprintf(stderr,"Can't have both rerun and resume options set\n");
        tet_shutdown();
    }

    /* If we have rerun or resume option we need to know the old_jnl name */
    if ( (resume_opt == TRUE) || (rerun_opt == TRUE) )
    {
        if ( (argc - optind) < 1 )
        {
            (void) fprintf(stderr,"old journal file is a required argument.\n");
            tet_shutdown();
        }

        (void) strcpy(old_journal,argv[optind]);

        old_jnl_fp = fopen(old_journal,"r");
        if (old_jnl_fp == NULL)
        {
            (void) fprintf(stderr,"Failed to open old journal %s.\n",old_journal);
            BAIL_OUT("fopen of old journal");
        }
	else
	{
	    DBUG(DBGFILES)(stderr,"fopen(%ld)\n", (long) old_jnl_fp);
	}

        /* make sure the file descriptor is closed on an exec call */
        DBUG(DBGFILES)(stderr,"fcntl(%d, F_SETFD, FD_CLOEXEC)\n", (int) fileno(old_jnl_fp));
        (void) fcntl(fileno(old_jnl_fp), F_SETFD, FD_CLOEXEC);
        
        optind++;
    }

    DBUG(DBGFINFO) (stderr,"tetroot = %s\n", tetroot);
    DBUG(DBGFINFO) (stderr,"start_dir = %s\n", start_dir);

    /*
     *  Determine whether the suite_name and the scene name have been given
     *  and if not try and calculate their defaults.
     */
    if ((argc - optind) < 2)
    {
        /* If -l is null or scen_file_name has been specified */
        if( (com_scen == FALSE) || (scen_file_name != NULL))
        {
            /* default scene name */
            scenario_name = store_str("all");
        }

        if((argc - optind) < 1)
        {
            /* Don't want to mess with start_dir */
            (void) strcpy(tmp_dir, start_dir);

            /* try to establish suite_name, as per spec. */
            if(strncmp(tetroot,tmp_dir,strlen(tetroot)) == 0)
            {
                if((tmp_ptr = strchr(&tmp_dir[strlen(tetroot)+1],DIR_SEP_CHAR))
                            != NULL)
                {
                    *tmp_ptr = '\0';
		    suite_name = store_str(&tmp_dir[strlen(tetroot)+1]);
                }
                else
                {
                    if((tmp_dir[strlen(tetroot)+1] != '\0') && 
                        (! isspace(tmp_dir[strlen(tetroot)+1])))
                    {
		    	suite_name = store_str(&tmp_dir[strlen(tetroot)+1]);
                    }
                    else
                    {
                        (void) fprintf(stderr,"No test suite name supplied, and unable to establish default.\n");
                        tet_shutdown();
                    }
                }

                /* build up the test suite root */
		comb_paths(ts_root, tetroot, suite_name);
            }
            else
            {
                (void) fprintf(stderr, "No test suite name supplied, and unable to establish default.\n");
                tet_shutdown();
            }

        }
        else
        {
            /* set the test suite name */
            suite_name = store_str(argv[optind]);
	    comb_paths(ts_root, tetroot, suite_name);
        }
    }
    else
    {
        /* set the test suite name */
        suite_name = store_str(argv[optind]);
	comb_paths(ts_root, tetroot, suite_name);

        optind++;
	scenario_name = store_str(argv[optind]);
    }


    DBUG(DBGFINFO) (stderr,"ts_root = %s\n", ts_root);

    if(com_scen == TRUE)
    {
        if((scenario_name != NULL) && (scen_file_name == NULL))
        {
            (void) fprintf(stderr,"Need to set the scenario file name if you wish to use a \nscenario with specific scenario lines.\n");
            tet_shutdown();
        }
        /* Not allowed resume or rerun enabled with specific scen lines */
        if ( (resume_opt == TRUE) || (rerun_opt == TRUE) )
        {
            (void) fprintf(stderr,"Can't have rerun or resume options set, with specific scenario lines.\n");
            tet_shutdown();
        }
    }

    /*
     *  Build the path to the 'results' dir,  adding "/"
     *  chars between tetroot, suite_name, & "results" 
     */
    if (results_dir[0] == '\0')
        (void) sprintf(results_dir,"%s/results",ts_root);

    /* create results dir. if necessary */
    if (do_mkdir(results_dir) == FAILURE)
    {
        (void) fprintf(stderr,"Cannot make results directory %s\n",results_dir);
        tet_shutdown();
    }

    /*
     * If the user did not supply a results directory
     * on the command line then we need to sort out the sub-directory
     * beneath the "results" directory.
     */
    if( user_results_dir == FALSE)
    {
        /* get_jnl_dir() uses results_dir */
        jnl_dir = get_jnl_dir();
        if (jnl_dir == (char *) NULL)
        {
            (void) fprintf(stderr, "Failed to obtain dir for journal file\n");
            tet_shutdown();
        }

        (void) strcpy(results_dir, jnl_dir);
    }

    DBUG(DBGFINFO) (stderr,"results_dir = %s\n", results_dir);

    /* determine journal file name, if none given on command line */
    if (jnl_file_name == NULL)
    {
        jnl_file_name = (char *) TET_MALLOC2(strlen(results_dir) + 3 + strlen("journal"));
	comb_paths(jnl_file_name, results_dir, "journal");
    }

    /* Create the journal */
    if ((jnl_fd = open(jnl_file_name,O_WRONLY|O_CREAT|O_EXCL,
               (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH))) == FAILURE)
    {
        (void) fprintf(stderr,"Error opening journal file %s\n",jnl_file_name);
        perror("open");
        tet_shutdown();
    }
    else
    {
	DBUG(DBGFILES)(stderr,"open(%d)\n", (int) jnl_fd);

        /* display the journal name as per Spec. - well sort of... */
        (void) printf("journal file name is: %s\n", jnl_file_name);
        (void) fflush(stdout);

        /* make sure the file descriptor is closed on an exec call */
        DBUG(DBGFILES)(stderr,"fcntl(%d, F_SETFD, FD_CLOEXEC)\n", (int) jnl_fd);
        (void) fcntl(jnl_fd, F_SETFD, FD_CLOEXEC);
    }

    /*
     * Put a copy of the command line into the "TCC Start" journal message
     */
    (void) strcpy(msg,"TCC Start, Command Line: ");
    ctr = 0;
    while ((ctr<argc) && ((int) (strlen(msg) + strlen(argv[ctr] + 1)) < 256))
    {
        (void) strcat(msg,argv[ctr++]); 
        (void) strcat(msg," ");
    }

    /* write the TCC start message to the journal file */
    (void) jnl_entry_tcc_start(msg);

    /*
     *  Now we sort out all the configuration file names.
     */

    /* default build cfg file */
    default_cfg_file(&bld_cfg_file_name,tetroot,alt_exec_dir,suite_name,"tetbuild.cfg");

    /* default exec cfg file */
    default_cfg_file(&exe_cfg_file_name,tetroot,alt_exec_dir,suite_name,"tetexec.cfg");

    /* default clean cfg file */
    default_cfg_file(&cln_cfg_file_name,tetroot,alt_exec_dir,suite_name,"tetclean.cfg");

    /* default distributed cfg file */
    default_cfg_file(&dis_cfg_file_name,tetroot,alt_exec_dir,suite_name,"tetdist.cfg");

    if((com_scen == FALSE) || (com_scen == TRUE && scen_file_name != NULL))
    {
        /* default scenario file */
        default_cfg_file(&scen_file_name,tetroot,alt_exec_dir,suite_name,"tet_scen");
    }

    /* Write the tetroot, ts_root and alt_exec_dir variables to the dis_env.
     * The environment is used by routines in order to find out the values
     * of these variables for the MASTER and any remote systems.
     */
    add_tetenv("TET_REM000_TET_ROOT", tetroot, CFG_PERM_VAL, &dis_env);
    add_tetenv("TET_REM000_TET_TSROOT", ts_root, CFG_PERM_VAL, &dis_env);
    add_tetenv("TET_REM000_TET_EXECUTE", alt_exec_dir, CFG_PERM_VAL, &dis_env);

    /* Make unique temporary directory for the MASTER system */
    if((tmp_ptr = tet_mktmpdir(temp_dir, FALSE, MASTER)) != NULL)
    {
        add_tetenv("TET_REM000_TET_TMP_DIR", tmp_ptr, CFG_PERM_VAL, &dis_env);
        TET_FREE(tmp_ptr);
    }
    else
    {
	BAIL_OUT2("Could not create temporary directory for the master system.\n");
    }

    /* Call the presyntax checker */
    /* The presyntax check produces a list of all the remote designations
     * used within the scenario file as well as checking the syntax of the file.
     * ('all_sysids' stores the designation numbers, 'total_sysid' is the total
     * number of remote designations found.)
     */
    if(scen_file_name != NULL)
    {
        if(check_scenario(scen_file_name, scenario_name) == FAILURE)
	{
		(void) sprintf(error_mesg, "Syntax check of scenario '%s' in file '%s' failed", scenario_name, scen_file_name);
		BAIL_OUT2(error_mesg);
	}
    }
    if(com_scen == TRUE)
    {
        tmp_scen_file_name = store_paths(get_temp_dir(FALSE, MASTER), "com_line_scen");

	if(make_alt_scenario(tmp_scen_file_name, scen_lines) == FAILURE)
	{
		(void)sprintf(error_mesg, "Could not make temporary scenario file: \"$s\"\n", tmp_scen_file_name);
		BAIL_OUT2(error_mesg);
	}

        for(num = 0; scen_lines[num] != NULL; num++)
	    TET_FREE((void *)scen_lines[num]);

	/* Check syntax */
	if( check_scenario(tmp_scen_file_name, "all") == FAILURE)
	{
		BAIL_OUT2("Syntax check of command line specified scenario directives failed");
	}
    }

    /* 
     *  Load in the environment for each of the modes selected, this is also
     *  the point where the configuration variables get written to the jnl.
     */
    if (ifmode(BUILD))
    {
        init_config(&bld_env);
        do_config_file(bld_cfg_file_name,MODE_BLD,&bld_env);
    }

    if (ifmode(EXEC))
    {
        init_config(&exe_env);
        do_config_file(exe_cfg_file_name,MODE_EXEC,&exe_env);
    }

    if (ifmode(CLEAN))
    {
        init_config(&cln_env);
        do_config_file(cln_cfg_file_name,MODE_CLN,&cln_env);
    }

    if(total_sysid > 0)
    {
        do_config_file(dis_cfg_file_name,MODE_DIST,&dis_env);
    }

    TET_FREE(temp_dir);
    TET_FREE(tetroot);
    TET_FREE(ts_root);
    TET_FREE(alt_exec_dir);

    /* Check that all the paths required by the tcc have been defined */
    if(validate_pathnames(FALSE, MASTER) == FAILURE)
	tet_shutdown();

    /* Check that all the paths required by the remote systems have been
     * defined.
     */

    for(ctr=0; ctr<total_sysid; ctr++)
    {
        if(all_sysids[ctr].sysid == MASTER)
                continue;

        if(validate_pathnames(TRUE, all_sysids[ctr].sysid) == FAILURE)
		tet_shutdown();
    }


    /* Get the result codes and their actions from the appropiate file */
    if(get_rescode_file() == FAILURE)
    {
        (void) fprintf(stderr, "Failure to successfully process TET result code file\n");
        tet_shutdown();
    }

    /* 
     *  Now that we have the result codes, convert the result pattern names
     *  into numbers.
     */
    if( result_pat != NULL)
    {
        result_list = (int *) TET_MALLOC2((size_t)(sizeof(int)*10));
        for(ctr = 0; ctr < 10; ctr++)
	    result_list[ctr] = -1;

        /* Get an array of results codes from the comma seperated list */
        for( cp = result_pat; cp != NULL; )
        {
	    ch = 0;
            /* The results code name is stored internally with quotes round
             * it so we add our own to the word we're checking.
             */
            tmp_ptr = (char *) TET_MALLOC( strlen(cp) + 5);
            tmp_ptr[0] = '"';
            (void) strcpy(&tmp_ptr[1], cp);

            if((cp = strchr(cp,',')) != NULL)
            {
                tmp_ptr[strlen(tmp_ptr) - strlen(cp)] = '\0';
                /* advance the pointer to the next position after the comma */
                cp++;
            }
            (void) strcat(tmp_ptr, "\"");

	    /* check for mode specifiers in the list */
	    if(strlen(tmp_ptr) == 3) /* ie a single letter in quotes */
	    {
		switch((int) tmp_ptr[1])
		{
			case 'b':
				result_mode[0] = TRUE;
				ch = 1;
				break;
			case 'e':
				result_mode[1] = TRUE;
				ch = 1;
				break;
			case 'c':
				result_mode[2] = TRUE;
				ch = 1;
				break;
			default:
				break;
		}
		if( ch == 1)
		{
			TET_FREE((void *) tmp_ptr);
			continue;
		}
	    }

            rc = rescode_name_to_num(tmp_ptr);
            if(rc == FAILURE)
            {
                (void) fprintf(stderr,"Invalid result code name: %s\n",tmp_ptr);
                tet_shutdown();
            }
            else
            {
                if((max_res % 10) == 9)
                {
                    result_list = (int *) TET_REALLOC((void *)result_list, (size_t)(sizeof(int)*(max_res+11)));
                    for(ctr = max_res; ctr < (max_res + 11); ctr++)
                        result_list[ctr] = -1;
                }
                result_list[max_res] = rc;
                ++max_res;
            }

            TET_FREE((void *) tmp_ptr);
        }
        if(max_res == 0 && result_mode[0] == FALSE && result_mode[1] == FALSE &&
		result_mode[2] == FALSE)
        {
            (void) fprintf(stderr,"Couldn't get any result codes from command line.\n");
            tet_shutdown();
        }
        else
            result_list[max_res] = -1;
    }

    /* Distributed bit: Logon to remote systems, send and receive configuration
     * data and set up directories. */
    if(dtcc(total_sysid, all_sysids, argc, argv) == FAILURE)
	tet_shutdown();
    else
	remote_tmp_dirs = TRUE;
	

    /* Check that all the paths required by the tcc have the neccassary
     * permisions.
     */
    if(check_access(FALSE, MASTER) == FAILURE)
	tet_shutdown();

    /* Check that all the paths required by the remote systems have the
     * neccassary permisions.
     */

    for(ctr=0; ctr<total_sysid; ctr++)
    {
        if(all_sysids[ctr].sysid == MASTER)
                continue;

        if(check_access(TRUE, all_sysids[ctr].sysid) == FAILURE)
		tet_shutdown();
    }

    /* Write the command line environment variables to a file
     * "COM_VAR" and the distributed environment to a file "DIST" on
     * the master and remote systems and export variables "TET_COM_VAR"
     * and "TET_DIST" equated to the full path names of the files to the
     * system. 
     */
    write_env_file(com_env, "COM_VAR", "TET_COM_VAR", FALSE, MASTER);
    write_env_file(dis_env, "DIST", "TET_DIST", FALSE, MASTER);

    for(ctr=0; ctr<total_sysid; ctr++)
    {
        /* Ignore if MASTER system. */
        if(all_sysids[ctr].sysid == MASTER)
                continue;

    	write_env_file(com_env, "COM_VAR", "TET_COM_VAR", TRUE, all_sysids[ctr].sysid);
	write_env_file(dis_env, "DIST", "TET_DIST", TRUE, all_sysids[ctr].sysid);
    }

    /* logoff all remote systems now that we have done variable exchange etc. */
    end_tccd(total_sysid, all_sysids);

    /* Everything is now set up ! */
    (void) setup_complete(SETVALUE);

    /*
     *  Depending on what modes and options were selected we now call
     *  some heavy duty functions.
     */
    if (rerun_opt == TRUE)
    {
        /*
         *  do_again(RERUN) looks in the old journal file and reruns any 
         *  invocation cases that match the right results code, mode, etc..
         */
        if(do_again(RERUN, result_list, result_mode) == FAILURE)
            exit_val = EXIT_BAD_MISC;
    }
    else
    {
        if (resume_opt == TRUE)
        {
            /*
             *  do_gain(RESUME) discovers the test case line (tc_line) and
             *  the invocable component number (ic_num) from which to
             *  continue processing from in the scenario file.
             */
            if(do_again(RESUME, result_list, result_mode) == FAILURE)
                exit_val = EXIT_BAD_MISC;
            else
            {
                /* Process the scenario file */
                if(perform_scen(resume_status) == FAILURE)
                    exit_val = EXIT_BAD_MISC;
            }
        }
        else 
        {
            if( com_scen == TRUE)
            {
		/* Run temporary scenario file */
		tmp_scen_file = scen_file_name;
		tmp_scen_name = scenario_name;
		
		scen_file_name = tmp_scen_file_name;
		scenario_name = store_str("all");

                if(perform_scen(resume_status) == FAILURE)
                    exit_val = EXIT_BAD_MISC;

		TET_FREE((void *)scenario_name);
		scen_file_name = tmp_scen_file;
		scenario_name = tmp_scen_name;
            }

            /* Process the scenario file */
	    if(abort_requested() != TRUE)
	    {
                if(scenario_name != NULL)
		{
                    if(perform_scen(resume_status) == FAILURE)
                        exit_val = EXIT_BAD_MISC;
		}
	    }
        }
    }

    /* write the TCC end message to the journal file */
    (void) strcpy(msg,"TCC End");
    (void) jnl_entry_tcc_end(msg);

    DBUG(DBGFILES)(stderr,"close(%d)\n", (int) jnl_fd);
    (void) close(jnl_fd);
    jnl_fd = -1;

    tet_cleanup();

    if(debug_flag & DBGMEM)
    {
	if(debug_flag & DBGMEM2)
	    memory_table(1);
	else
	    memory_table(0);
    }

    return(exit_val);
}


/*
 * Pretty obvious from the name I hope ....
 */
void show_synopsis()
{

    DBUG(DBGFDESC) (stderr, "show_synopsis()\n");


    (void) fprintf(stderr, "\n%s\n%s\n%s\n%s\n%s\n\n",
"Usage: tcc [ -b ] [ -e ] [ -c ] [ -a Alt_exec_dir ] [ -f Clean_cfg_file ]",
"    [ -g Build_cfg_file ] [ -i Int_results_dir ] [ -j Journal_file]",
"    [ -l Scenario_line ] [ -p ] [ -n Search_string ] [ -s Scenario_file]",
"    [ -t Timeout ] [ -v Variable=value ] [ -x Exec_cfg_file ]",
"    [ -y Search_string ] [ Test_suite [ Scenario ] ]");

    (void) fprintf(stderr, "%s\n\n%s\n",
"OR: tcc -m Code_list [ options ] Old_journal_file [ Test_suite [ Scenario ] ]",
"OR: tcc -r Code_list [ options ] Old_journal_file [ Test_suite [ Scenario ] ]"
	);
}
