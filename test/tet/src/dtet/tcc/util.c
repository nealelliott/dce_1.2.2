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
static char sccsid[] = "@(#)util.c	1.7 (92/12/07) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)util.c    1.7 12/7/92
NAME:		util.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Tcc implementaions of miscellaneous functions

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

char zeetime[18];          /* Big enough to contain "HH:MM:YY YYYYMMDD" */
time_t time_val;           /* Variable to hold the time stucture */

extern char ** environ;

/*
 *  Create a directory with permissions = (S_IRWXU|S_IRWXG|S_IRWXO)
 *  Only generate an error if it doesn't already exist - howvever if
 *  it does exists errno will still be set to EEXIST.
 */
int do_mkdir(path_p)
char *path_p;
{
    char *pn;
    int rc;


    DBUG(DBGUTIL) (stderr,"do_mkdir(%s)\n", path_p);

    /* Errno will only change value if there is an error. */
    errno = 0;

    pn = store_str(path_p);

    if (path_p[strlen(path_p)-1] == DIR_SEP_CHAR)
        pn[strlen(path_p)-1] = '\0';

    rc = mkdir(pn,(S_IRWXU|S_IRWXG|S_IRWXO));
    if((rc == FAILURE) && (errno != EEXIST))
    {
        COMPLAIN("mkdir failed");
        return(FAILURE);
    }
    TET_FREE((void *)pn);

    return(SUCCESS);
}


char *number_suffix( num)
int num;
{
    static char suf[3];


    DBUG(DBGUTIL) (stderr,"number_suffix(%d)\n", num);

    switch (num%10)
    {
	case 1:
	    (void) strcpy(suf, "st");
	    break;
	case 2:
	    (void) strcpy(suf, "nd");
	    break;
	case 3:
	    (void) strcpy(suf, "rd");
	    break;
	default:
	    (void) strcpy(suf, "th");
	    break;
    }

    return(suf);
}



/*
 * Looks for a string inside another one and returns a pointer to its
 * occurance or else a NULL.
 */
char *strinstr(line_str, string)
char *line_str, *string;
{
    char *cp;

    for (cp = line_str ; (*cp != '\0') ; cp++)
    {
        cp = strchr(cp,*string); /* find the first char. */
        if (cp == NULL)
            return(NULL);
        else
            if (strncmp(cp,string,strlen(string)) == 0)
                return(cp);
    }

    return(NULL);
}


/*
 * A function to return the basename from a pathname, being careful
 * never to return a null pointer.
 * Arguments:
 * pointer to pathname string
 * Returns:
 * pointer to 'leafname' of the string
 */
char *basename(path)
char *path;
{
    char *sp;

    if ((sp = strrchr(path, DIR_SEP_CHAR)) != (char *) NULL)
    {
        if (*(sp + 1) != '\0')
            return(++sp);
        else
            return(sp);
    }
    else
        return(path);
}


/*
char *getpath(path)
char *path;
{
    char *newpath;     
    char *ptr;

    newpath = store_str(path);

    ptr = strrchr(newpath, DIR_SEP_CHAR);
    if( (ptr != NULL)
         &&
	(*(ptr+1) != '\0') )
    {
	*ptr = '\0';
    }
    else
    {
	TET_FREE(newpath);
	newpath = NULL;
    }
    return(newpath);
}
*/



/*
 * A routine that allows more than one go at mallocing something
 */
void * tet_malloc(size_wanted, file_name, line_no, flag)
size_t size_wanted;
char * file_name;
int    line_no;
int    flag;
{
    char *mall_ptr;
    int tries = 0;

    DBUG(DBGMEM3) (stderr, "tet_malloc(%d, %s, %d, %d)", (int) size_wanted, file_name, line_no, flag);

    if(debug_flag & DBGMEM)
        mem_setup();

    while(tries < 5)
    {
        mall_ptr = (char *) malloc((size_t) size_wanted);
        if(mall_ptr != NULL)
            break;
        tries++;
        (void) sleep(2);
    }
    if(tries >= 5)
    {
        (void) fprintf(stderr,"\nMalloc failed. source file %s line %d\n", file_name, line_no);
        tet_shutdown();
    }

    /* This bit is for memory tracing */
    if(debug_flag & DBGMEM)
    {
        int memctr;

        for(memctr=0; memtable[memctr].line != -1; memctr++)
        {
            if (memtable[memctr].line == 0)
            {
                memtable[memctr].ptr = (long) mall_ptr;
                memtable[memctr].vol = (int) size_wanted;
                memtable[memctr].line = line_no;
		if (flag)
		{
                     (void) strcpy(memtable[memctr].file, "* ");
                     (void) strcat(memtable[memctr].file, file_name);
		}
		else
                     (void) strcpy(memtable[memctr].file, file_name);
                break;
            }
        }
    }

    DBUG(DBGMEM3) (stderr, "    returns %ld\n", (long) mall_ptr);

    return((void *)mall_ptr);
}


/* ARGSUSED */
void tet_free( ptr, file_name, line_no)
#if __STDC__
void *ptr;
#else
char *ptr;
#endif
char *file_name;
int line_no;
{
    int memctr;

    DBUG(DBGMEM3) (stderr, "tet_free(%ld, %s, %d)\n", (long) ptr, file_name, line_no);

    if(debug_flag & DBGMEM)
    {
        for(memctr=0; memtable[memctr].line != -1; memctr++)
        {
            if (memtable[memctr].ptr == (long) ptr)
            {
		if(debug_flag & DBGMEM3)
    	            (void) fprintf(stderr, "%-11s(%12ld %8d %20s %15d)\n", "tet_free",memtable[memctr].ptr, memtable[memctr].vol, memtable[memctr].file, memtable[memctr].line);
                memtable[memctr].line = 0;
                memtable[memctr].ptr = 0;
                break;
            }
        }
        if (memtable[memctr].line == -1)
            (void) fprintf(stderr,"Attempt to free pointer in source file %s line %d\nthat is not in memory table\n", file_name, line_no);
        else
            free((void *) ptr);
    }
    else
        free((void *) ptr);
}


/*
 * A routine that allows more than one go at reallocing something
 */
void * tet_realloc(ptr, size_wanted, file_name, line_no, flag)
#if __STDC__
void *ptr;
#else
char *ptr;
#endif
size_t size_wanted;
char * file_name;
int    line_no;
int    flag;
{
    char *mall_ptr;
    int tries = 0;

    DBUG(DBGMEM3) (stderr, "tet_realloc(%ld, %d, %s, %d, %d)", (long) ptr, (int) size_wanted, file_name, line_no, flag);

    if(debug_flag & DBGMEM)
        mem_setup();

    while(tries < 5)
    {
        mall_ptr = (char *) realloc((void *) ptr, (size_t) size_wanted);
        if(mall_ptr != NULL)
            break;
        tries++;
        (void) sleep(2);
    }
    if(tries >= 5)
    {
        (void) fprintf(stderr,"\nRealloc failed. source file %s line %d\n", file_name, line_no);
        tet_shutdown();
    }

    /* This bit for memory tracing */
    if(debug_flag & DBGMEM)
    {
        int memctr;

        for(memctr=0; memtable[memctr].line != -1; memctr++)
        {
            if (memtable[memctr].ptr == (long) ptr)
                break;
        }

        if (memtable[memctr].line == -1)
        {
            for(memctr=0; memtable[memctr].line != -1; memctr++)
            {
                if (memtable[memctr].line == 0)
                    break;
            }
        }
        memtable[memctr].ptr = (long) mall_ptr;
        memtable[memctr].vol = (int) size_wanted;
        memtable[memctr].line = line_no;
	if (flag)
	{
            (void) strcpy(memtable[memctr].file, "* ");
            (void) strcat(memtable[memctr].file, file_name);
	}
	else
            (void) strcpy(memtable[memctr].file, file_name);
    }

    DBUG(DBGMEM3) (stderr, "    returns %ld\n", (long) mall_ptr);

    return((void *) mall_ptr);
}


void memory_table( flag)
int flag;
{
    int memctr, found_one = 0;

    for(memctr = 0; memtable[memctr].line != -1; memctr++)
    {
        if (memtable[memctr].line != 0)
        {
	    if ((flag == 0 && *(memtable[memctr].file) != '*') || (flag == 1))
	    {
                if (! found_one++)
                    (void) fprintf(stderr, "\n%40s\n%40s\n\n%12s %8s %20s %15s\n", "Memory Table Contents", "~~~~~~~~~~~~~~~~~~~~~", "Pointer", "Size", "Source File", "Line Number");
                (void) fprintf(stderr, "%12ld %8d %20s %15d\n", memtable[memctr].ptr, memtable[memctr].vol, memtable[memctr].file, memtable[memctr].line);
	    }
        }
    }
}


void mem_setup()
{
    int memctr, newctr;

    if (memtable == NULL)
    {
        if ((memtable = (MEMCHECK *) malloc((size_t) (sizeof(MEMCHECK) * 50))) == NULL)
        {
            (void) fprintf(stderr,"Malloc failed. source file %s line %d\n", __FILE__, __LINE__);
            tet_shutdown();
        }
        for(memctr = 0; memctr < 49; memctr++)
            memtable[memctr].line = 0;
        memtable[memctr].line = -1;
    }
    else
    {
        for(memctr = 0; (memtable[memctr].line != 0) && (memtable[memctr].line != -1); memctr++);
        if (memtable[memctr].line == -1) /* full */
        {
            if ((memtable = (MEMCHECK *) realloc((void *) memtable, (size_t) (sizeof(MEMCHECK) * (50+memctr+1)))) == NULL)
            {
                 (void) fprintf(stderr,"Realloc failed. source file %s line %d\n", __FILE__, __LINE__);
                 tet_shutdown();
            }
            for(newctr = memctr; newctr < (memctr+49); newctr++)
                memtable[newctr].line = 0;
            memtable[newctr].line = -1;
        }
    }
}


/* Special non Posix functions used in tcc */

int    opterr = 1;
int    optind = 1;
char   *optarg;

int optget(argc, argv, opts)
int    argc;
char   **argv, *opts;
{
    static int sp = 1;
    int c;
    char *cp;

    /* Are we just starting to process this particular parameter */
    if (sp == 1)
    {
        if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0')
	{
            return EOF;
	}
        else if(strcmp(argv[optind], "--") == 0)
        {
            optind++;
            return EOF;
        }
    }

    /* set 'c' equal to the paramter letter */
    c = argv[optind][sp];

    /* if 'c' is not the legal parameter list or if 'c' is ':' */
    if (c == ':' || (cp = strchr(opts, c)) == NULL)
    {
        if (opterr)
            (void) fprintf(stderr, "%s: illegal option -- %c\n", argv[0], c);

        if (argv[optind][++sp] == '\0')
        {
            optind++;
            sp = 1;
        }
        return '?';
    }

    /* If this parameter letter is supposed to take a value */
    if (*++cp == ':')
    {
        if (argv[optind][sp+1] != '\0')
        {
            optarg = &argv[optind++][sp+1];
        }
        else if(++optind >= argc)
        {
            if (opterr)
                (void) fprintf(stderr,
                    "%s: option requires an argument -- %c\n", argv[0], c);
            sp = 1;
            return '?';
        } 
        else
            optarg = argv[optind++];
        sp = 1;
    }
    else
    {
        if (argv[optind][++sp] == '\0')
        {
            sp = 1;
            optind++;
        }
        optarg = NULL;
    }
    return c;
}


/*
 * Get the time & format it into zeetime for inclusion in a journal entry.
 */
void get_time()
{
    struct tm *tp;

    (void) time(&time_val);
    tp = localtime(&time_val);
    (void)sprintf(zeetime, "%02d:%02d:%02d",
        tp->tm_hour, tp->tm_min, tp->tm_sec);
}


/*
 * This routine mimics putenv(), and is provided purely
 * because putenv() is not in POSIX.1
 */
int put_tcenv(envstr)
char *envstr;
{
    char **newenv, **cur, *envname;
    int n, count = 0;
    static char **allocp = NULL;

    DBUG(DBGUTIL) (stderr, "put_tcenv(envstr:%s)\n", envstr);

    if (environ == NULL)
    {
        newenv = (char **)TET_MALLOC((size_t)(2*sizeof(char *)));

        newenv[0] = (char *) TET_MALLOC2((size_t) (strlen(envstr)+1));
        (void) strcpy(newenv[0] ,envstr);
        newenv[1] = NULL;
        environ = newenv;
        allocp = newenv;
        return(SUCCESS);
    }

    cur = environ;

    /* Look to substitute it in the current environment */
    while (*cur != NULL)
    {
        count++;
        envname = *cur;
        n = strcspn(envstr, "=");
        if (strncmp(envname, envstr, (size_t) n) || envname[n] != '=')
            cur++;
        else
        {
            *cur = (char *) TET_MALLOC2((size_t) (strlen(envstr)+1));
            (void) strcpy(*cur, envstr);
            return(SUCCESS);
        }
    }
    
    /*
     * If we previously allocated this environment enlarge it using
     * realloc(), otherwise allocate a new one and copy it over.
     * Note that only the last malloc()/realloc() pointer is saved, so
     * if environ has since been changed the old space will be wasted.
     */

    if (environ == allocp)
        newenv = (char **) TET_REALLOC2((void *) environ, 
                (size_t) ((count+2)*sizeof(char *)));
    else
        newenv = (char **) TET_MALLOC2((size_t) ((count+2)*sizeof(char *)));

    if (environ != allocp)
    {
        for (n = 0; environ[n] != NULL; n++)
            newenv[n] = environ[n];
        allocp = newenv;
    }
    newenv[count] = (char *) TET_MALLOC2((size_t) (strlen(envstr)+1));
    (void) strcpy(newenv[count], envstr);
    newenv[count+1] = NULL;
    environ = newenv;

    return(SUCCESS);
}

char *get_ts_root(remote, sysid)
int remote;
int sysid;
{
    char env_var[23];
    char *ts_root;

    DBUG(DBGUTIL) (stderr, "get_ts_root(remote:%d, sysid:%d)\n", remote, sysid);

    if(remote == FALSE)
	sysid = MASTER;

    (void) sprintf(env_var,"TET_REM%3.3d_TET_TSROOT",sysid);
    ts_root = get_tetenv(env_var, dis_env);

    if(ts_root != NULL)
    {
	if(*ts_root == '\0')
	    ts_root = NULL;
    }

    if(ts_root == NULL)
    {
	DBUG(DBGWARN)(stderr, "Warning: get_ts_root() returning NULL.\n");
    }

    return(ts_root);
}


/*
 *  Note: no BAIL_OUTS are allowed in this function.
 */
char *get_tet_root(remote, sysid)
int remote;
int sysid;
{
    char env_var[21];
    char *tetroot;

    DBUG(DBGUTIL) (stderr, "get_tet_root(remote:%d, sysid:%d)\n", remote, sysid);

    if(remote == FALSE)
	sysid = MASTER;

    (void) sprintf(env_var,"TET_REM%3.3d_TET_ROOT",sysid);
    tetroot = get_tetenv(env_var, dis_env);

    if(tetroot != NULL)
    {
	if(*tetroot == '\0')
	    tetroot = NULL;
    }

    if(tetroot == NULL)
    {
	DBUG(DBGWARN)(stderr, "Warning: get_tet_root() returning NULL.\n");
    }

    return(tetroot);
}


char *get_alt_exec_dir(remote, sysid)
int remote;
int sysid;
{
    char env_var[25];
    char *alt_exec_dir;


    DBUG(DBGUTIL) (stderr, "get_alt_exec_dir(remote:%d, sysid:%d)\n", remote, sysid);

    if(remote == FALSE)
	sysid = MASTER;

    (void) sprintf(env_var,"TET_REM%3.3d_TET_EXECUTE",sysid);
    alt_exec_dir = get_tetenv(env_var, dis_env);

    if(alt_exec_dir != NULL)
    {
	if(*alt_exec_dir == '\0')
	    alt_exec_dir = NULL;
    }

    return(alt_exec_dir);
}

/*
 *  Note: no BAIL_OUTS are allowed in this function.
 */
char *get_temp_dir(remote, sysid)
int remote;
int sysid;
{
    char env_var[25];
    char *temp_dir;

    DBUG(DBGUTIL) (stderr, "get_temp_dir(remote:%d, sysid:%d)\n", remote, sysid);

    if(remote == FALSE)
	sysid = MASTER;

    (void) sprintf(env_var,"TET_REM%3.3d_TET_TMP_DIR",sysid);
    temp_dir = get_tetenv(env_var, dis_env);

    if(temp_dir != NULL)
    {
	if(*temp_dir == '\0')
	    temp_dir = NULL;
    }

    return(temp_dir);
}


/*
 * Routine to combine two path names
 * The memory pointed to by 'path1' must be large enough to store both
 * 'path1' and 'path2' plus a DIR_SEP_CHAR and a '\0'.
 *
 *  Note: no BAIL_OUTS are allowed in this function.
 */
void cat_path(path1, path2)
char *path1;
char *path2;
{
    DBUG(DBGUTIL) (stderr, "cat_path(path1:%s, path2:%s)\n", path1, path2);

    if( (path1 == NULL) || (path2 == NULL) )
    {
	DBUG(DBGWARN) (stderr, "Warning: Null argumant #%d passed to 'cat_path()'.\n", (path1 == NULL)? 1 : 2);
	return;
    }

 
    if(*path1 == '\0')
    {
	(void)strcpy(path1, path2);
    }
    else
    {
        if(*path2 == DIR_SEP_CHAR)
            path2++;
        if(path1[strlen(path1)-1] != DIR_SEP_CHAR)
	    (void) strcat(path1, DIR_SEP_STR);
        (void) strcat(path1, path2);
    }


    DBUG(DBGUTIL) (stderr, "cat_path() path1 now \"%s\"\n", path1);

    return;
}

/*
 * Combine two path names.
 *
 *  Note: no BAIL_OUTS are allowed in this function.
 */
void comb_paths(comb_path, path1, path2)
char *comb_path;
char *path1;
char *path2;
{
	DBUG(DBGUTIL)(stderr, "comb_paths(comb_path, path1:%s, path2:%s)\n", (path1 == NULL)? "Null":path1, (path2 == NULL)? "Null":path2);

	if(comb_path == NULL)
	{
		DBUG(DBGWARN)(stderr, "Warning: Null 'comb_path' pointer passed to 'comb_paths()'.\n");
		return;
	}
	if(path1 != NULL)
		(void)strcpy(comb_path, path1);
	else
		*comb_path = '\0';

	cat_path(comb_path, path2);

	return;
}

char *store_paths(path1, path2)
char *path1;
char *path2;
{
	char *new_path;

	DBUG(DBGUTIL)(stderr, "store_paths(path1:%s, path2:%s)\n", (path1 == NULL)? "Null":path1, (path2 == NULL)? "Null":path2); 

	if((path1 == NULL) || (path2 == NULL))
	{
		DBUG(DBGWARN)(stderr, "Warning: Null parameter passed to 'store_paths()'.\n");
		return(NULL);
	}

	new_path = (char *)TET_MALLOC(sizeof(char) * (strlen(path1)+strlen(path2)+2));
	comb_paths(new_path, path1, path2);

	return(new_path);
}


/*
 *  Routine to create the directories which make up 'create_path' above
 *  the path 'path'.
 *  'path' is overwritten with the new path. Care must be taken therefore
 *  that the space allocated for 'path' is large enough for 'create_path'
 *  to be added.
 */
int make_path(path, create_path, remote, sysid)
char *path;
char *create_path;
int remote;
int sysid;
{
	char *dir;
	char *new_dirs;
	int ret_val = SUCCESS;

	DBUG(DBGUTIL) (stderr, "make_path(path:%s, create_path:%s, remote:%d, sysid:%d)\n", path, create_path, remote, sysid);

	new_dirs = store_str(create_path);

	dir = strtok(new_dirs, DIR_SEP_STR);
	
	while(dir != NULL)
	{
		cat_path(path, dir);

		if((ret_val = tet_mkdir(path, remote, sysid)) == FAILURE)
		{
			if(errno == EEXIST)
			{
				/* If the directory already exists */
				ret_val = SUCCESS;
			}
			else
			{
				(void) sprintf(error_mesg,"failed to make directory %s\n", path);
				ERROR(error_mesg);
				ret_val = FAILURE;
				break;
			}
		}

		/* get next token */
		dir = strtok(NULL, DIR_SEP_STR);
	}

	TET_FREE(new_dirs);
	return(ret_val);
}


/* Routine to make a unique directory. The directories which preceed the
 * directory to create must already exist.
 * The function will return the SUCCESS if the directory was created, otherwise
 * the error value of the mkdir will be returned and errno will be equal to
 * the specific error which caused it to fail.
 */
int make_unique_dir(path, create_dir, type, remote, sysid)
char *path;
char *create_dir;
int type;
int remote;
int sysid;
{
	int path_end;
	int ret_val;
	int count;

	DBUG(DBGUTIL) (stderr, "make_unique_dir(path:%s, type:%s, remote:%d, sysid:%d)\n", path, (type == DIR_ALPHA)? "ALPHA" : "NUMERIC", remote, sysid);

	cat_path(path, create_dir);

	path_end = strlen(path);

	if(type == DIR_ALPHA)
	{
		/* Add letter to the end of the path */
		path[path_end+1] = '\0';
		for(count=(int)'a'; count<=(int)'z'; count++)
		{
			path[path_end] = (char)count;
			ret_val = tet_mkdir(path, remote, sysid);
			if( (ret_val == SUCCESS) || (errno != EEXIST) )
				break;
		}
	}
	else
	{
		/* Add a three digit number to the end of the path */
		for(count=0; count<1000; count++)
		{
			(void)sprintf(&path[path_end], "%3.3d", count);
			ret_val = tet_mkdir(path, remote, sysid);
			if( (ret_val == SUCCESS) || (errno != EEXIST) )
				break;
		}
	}
	return(ret_val);
}


/* Remove all the directories which make up 'delete_path' above the path 'path'.
 * Directories must be empty, otherwise the function will fail.
 */
int remove_path(path, delete_path, remote, sysid)
char *path;
char *delete_path;
int remote;
int sysid;
{
	char rm_path[_POSIX_PATH_MAX];
	char *rm_dirs;
	char *last_dir;
	int ret_val;

	DBUG(DBGUTIL) (stderr, "remove_path(path:%s, delete_path:%s, remote:%d, sysid:%d)\n", path, delete_path, remote, sysid);

	rm_dirs = (char *) TET_MALLOC(sizeof(char) * (strlen(delete_path)+1));

        do
        {
                (void) strcpy(rm_path, path);
                cat_path(rm_path, rm_dirs);

                DBUG(DBGUTIL) (stderr,"About to try and remove %s\n", rm_path);
                ret_val = tet_rmdir(rm_path, remote, sysid);

                last_dir = strrchr(rm_dirs, DIR_SEP_CHAR);
                if(last_dir != NULL)
                        *last_dir = '\0';
        }
        while((last_dir != NULL) && (ret_val == SUCCESS));

	TET_FREE(rm_dirs);

	return(ret_val);
}
	

char *get_results_dir(remote, sysid)
int remote;
int sysid;
{
	char *ret_dir;

	DBUG(DBGUTIL) (stderr,"get_results_dir(sysid:%d)\n",sysid);

	if( (remote == FALSE) || (sysid == MASTER) )
	{
		ret_dir = results_dir;
	}
	else
	{
		ret_dir = rem_results_dir(NULL, sysid, FALSE);
	}

	return(ret_dir);
}

/* Check that specific system has a tet_root, tet_ts_root */
int validate_pathnames(remote, sysid)
int remote;
int sysid;
{
	char *path;
	char sysid_str[20];
	int ret_val = SUCCESS;

	DBUG(DBGFDESC)(stderr, "validate_paths(remote:%d, sysid:%d)\n", remote, sysid);

	if(remote == TRUE)
		(void) sprintf(sysid_str, " for system %3.3d", sysid);
	else
		sysid_str[0] = '\0';

	/* Check that system has a TET_TSROOT */
	path = get_ts_root(remote, sysid);
	if(path == NULL)
	{
		(void) sprintf(error_mesg, "No TET_TSROOT defined%s.\n",sysid_str);
		ERROR(error_mesg);
		ret_val = FAILURE;
	}

	/* Check that system has a TET_ROOT */
	path = get_tet_root(remote, sysid);
	if(path == NULL)
	{
		(void) sprintf(error_mesg, "No TET_ROOT defined.\n", sysid_str);
		ERROR(error_mesg);
		ret_val = FAILURE;
	}

	return(ret_val);
}


/* Check that access on all the paths required by the tcc allows read/write and
 * execution.
 */
int check_access(remote, sysid)
int remote;
int sysid;
{
	char *path;
	char sysid_str[20];
	int ret_val = SUCCESS;

	DBUG(DBGFDESC)(stderr, "validate_paths(remote:%d, sysid:%d)\n", remote, sysid);

	if(remote == TRUE)
		(void) sprintf(sysid_str, " for system %3.3d", sysid);
	else
		sysid_str[0] = '\0';

	/* Check access for TET_TSROOT */
	path = get_ts_root(remote, sysid);
	if(tet_access(path, (R_OK|W_OK|X_OK), remote, sysid) == FAILURE)
	{
		(void) sprintf(error_mesg, "Access failed for TET_TSROOT:\"%s\"%s.\n", path, sysid_str);
		ERROR(error_mesg);
		ret_val = FAILURE;
	}


	/* Check that system has a TET_ROOT */
	path = get_tet_root(remote, sysid);
	if(tet_access(path, (R_OK|W_OK|X_OK), remote, sysid) == FAILURE)
	{
		(void) sprintf(error_mesg, "Access failed for TET_ROOT:\"%s\"%s.\n", path, sysid_str);
		ERROR(error_mesg);
		ret_val = FAILURE;
	}

	if(get_alt_exec_dir(remote, sysid) != NULL)
	{
		if(tet_access(path, (R_OK|W_OK|X_OK), remote, sysid) == FAILURE)
		{
			(void) sprintf(error_mesg, "Access failed for TET_ALT_EXEC_DIR:\"%s\"%s.\n", path, sysid_str);
			ERROR(error_mesg);
			ret_val = FAILURE;
		}
	}

	path = get_results_dir(remote, sysid);
	if(tet_access(path, (R_OK|W_OK|X_OK), remote, sysid) == FAILURE)
	{
		(void) sprintf(error_mesg, "Access failed for results dir:\"%s\"%s.\n", path, sysid_str);
		ERROR(error_mesg);
		ret_val = FAILURE;
	}

	return(ret_val);
}



/* Store a string in memory. */
char *store_str(string)
char *string;
{
	char *str_ptr = NULL;

	DBUG(DBGUTIL)(stderr, "store_str(string:%s)\n", (string == NULL)? "Null" : string);

	if(string != NULL)
	{
		str_ptr = (char *)TET_MALLOC(sizeof(char *) * (strlen(string) + 1));
		(void)strcpy(str_ptr, string);
	}

	return(str_ptr);
}

char *combine_str(string1, string2)
char *string1;
char *string2;
{
	char *str_ptr;

	DBUG(DBGUTIL)(stderr, "combine_str(string1:%s, string2:%s)\n", (string1 == NULL)? "NULL" : string1, (string2 == NULL)? "NULL" : string2);

	if( (string1 != NULL) && (string2 != NULL) )
	{
		str_ptr = (char *)TET_MALLOC(sizeof(char *) * (strlen(string1) + strlen(string2) + 1));
		(void)strcpy(str_ptr, string1);
		(void)strcat(str_ptr, string2);
	}
	return(str_ptr);
}

/*
 *  Do a recursive remove of a directory, and then attempt to remove the
 *  directory.
 *
 *  NOTE: No BAIL_OUTS are permitted in this function !
 */
int remove_dir(dir_to_rm, remote, sysid)
char *dir_to_rm;
int remote;
int sysid;
{
    int ret_val;
    char *argv[4];
    char *tetroot;
    int stat_loc;


    DBUG(DBGFDESC) (stderr,"remove_dir(dir_to_rm:%s, remote:%d, sysid:%d)\n",dir_to_rm, remote, sysid);

    /* Make sure we are not in a directory that we are about to remove */
    tetroot = get_tet_root(remote, sysid);
    ret_val = tet_chdir(tetroot, remote, sysid);

    if(ret_val != FAILURE)
    {
	argv[0] = store_str("rm");
	argv[1] = store_str("-rf");
	argv[2] = store_str(dir_to_rm);
	argv[3] = NULL;

	if( (remote == FALSE) || (sysid == MASTER) )
	{
            if ((cpid = fork()) == 0)
            {
		execvp("rm", argv);

               	exit(EXIT_BAD_CHILD);
            }
	}
	else
	{
		if(server_dead() == TRUE)
			cpid = FAILURE;
		else
			cpid = tc_mexec(sysid, "rm", argv, NULL);
	}

	TET_FREE(argv[0]);
	TET_FREE(argv[1]);
	TET_FREE(argv[2]);

	if(cpid != -1)
	{
		if( (remote == FALSE) || (sysid == MASTER) )
		{
        	    (void) waitpid((pid_t)cpid,&stat_loc,0);
		}
		else
		{
		    (void) tc_wait(sysid, (long) cpid, -1, &stat_loc);
		}

        	if ( (WIFEXITED(stat_loc) == FALSE) || 
				(WEXITSTATUS(stat_loc) == EXIT_BAD_CHILD) )
		{
                	(void) sprintf(error_mesg,"rm IFEXITED %d exit status %d\n", WIFEXITED(stat_loc), WEXITSTATUS(stat_loc)); 
                	ERROR(error_mesg);
			ret_val = FAILURE;
            	}
	}
	else
	{
		ret_val = FAILURE;
	}
    }

    if(ret_val == FAILURE)
    {
	(void)sprintf(error_mesg, "Removal of directory %s failed\n", dir_to_rm);
	ERROR(error_mesg);
    }

    cpid = 0;

    return(ret_val);
}
