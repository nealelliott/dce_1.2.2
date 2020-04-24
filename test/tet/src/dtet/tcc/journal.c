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
static char sccsid[] = "@(#)journal.c	1.7 (93/01/24) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)journal.c    1.7 1/24/93
NAME:		journal.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions relating to the journal file

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
#include <pwd.h>
#if __STDC__
#include <stdarg.h>
#endif
#include <dirent.h>


#define  MAXRETRY  10    	 /* Number of retries in race for filename */

extern char zeetime[];           /* Time array */
time_t time_val;          	 /* Variable to hold the time stucture */


/*
 * Determines & creates the directory for the journal file name
 * (TET_ROOT/results/n, where 'n' is one greater than the highest
 * existing numbered directory).  To cope with more than one tcc competing
 * with each other, we try again up to MAXRETRY times if the directory we
 * try to create already exists). 
 * The full dir name includes the modes of invocation, this dir is created
 * after we have our unique sequential number, the number only dir is then
 * removed.
 * Returns a pointer to the determined directory, NULL in case of failure.
 */
char *get_jnl_dir()
{
    DIR *dir_pointer;
    struct dirent *dp;
    char tmp_name[_POSIX_PATH_MAX], full_name[_POSIX_PATH_MAX];
    int num, max, rc;


    DBUG(DBGFDESC) (stderr,"get_jnl_dir()");

    if ((dir_pointer = opendir(results_dir)) == NULL)
    {
        BAIL_OUT("cannot open results directory\n");
    }

    /*
     * Find highest-numbered directory in existence
     * (atoi() will return 0 for dirs like "." & "..")
     */
    max = 0;
    while ((dp = readdir(dir_pointer)) != (struct dirent *) NULL)
    {
        /* atoi() ignores the letters after the number portion of the name */
        num = atoi(dp->d_name);
        if (num > max)
            max = num;
    }
    (void) closedir(dir_pointer);
    /*
     * Create new directory numbered one greater than current max;
     * if beaten to it, retry MAXRETRY times
     */
    for (num = 0; num < MAXRETRY; num++)
    {
        (void) sprintf(tmp_name, "%s/%4.4d", results_dir, ++max);
        errno = 0;
        rc = mkdir(tmp_name, (S_IRWXU|S_IRWXG|S_IRWXO));
        if (rc == FAILURE && errno == EEXIST)
            continue;    /* Try again if file already exists */
        else
            break;    /* Succeeded, or failed for another reason */
    }

    if (rc == FAILURE)
    {
        DBUG(DBGFDESC) (stderr,"  returns NULL\n");
        return((char *) NULL);
    }
    else
    {
        /* 
         * We now have our exclusively numbered dir, however we want to
         * include the modes of invocation of the program in the dir
         * name so we create the dir we intend to use, ie 0001bec and then
         * remove the uniquely numbered dir, ie 0001
         */
         
        (void) strcpy(full_name, tmp_name);
        if(ifmode(BUILD))
            (void) strcat(full_name,"b");
        if(ifmode(EXEC))
            (void) strcat(full_name,"e");
        if(ifmode(CLEAN))
            (void) strcat(full_name,"c");
        rc = mkdir(full_name, (S_IRWXU|S_IRWXG|S_IRWXO));
        (void) rmdir(tmp_name);
        if (rc == FAILURE)
            return((char *) NULL);
    }

    DBUG(DBGFDESC) (stderr,"  returns %s\n", full_name);

    return(full_name);
}


/*
 *  Write a line of text to the journal.
 *  NOTE: No BAIL_OUTS are permitted in this function !
 */
void jnl_entry(linep)
char *linep;
{
    if((int) strlen(linep) > JNL_LINE_MAX)
    {
        ERROR("warning: journal file line truncated\n");

        linep[JNL_LINE_MAX-1] = '\n';
        linep[JNL_LINE_MAX] = '\0';
    }

    if( write(jnl_fd,(void *)linep,strlen(linep)) == -1)
        (void) fprintf(stderr,"write() to journal failed:\n%s", linep);
}


/*
 *  Write the tool start entry to the journal file.
 */
void jnl_tool_start(ctr,tcname,text, what_mode, systemid)
int ctr;
char *tcname;
char *text;
int what_mode;
int systemid;
{
    char sysidbuf[13];
    char lineout[BIG_BUFFER];

    (void) get_time();

    if(systemid != -1)
	(void) sprintf(sysidbuf,"sysid %.3d, ", systemid);

    systemid = -1;
    (void) sprintf(lineout, "%d|%d %s %s|%s, %s%s %d-%d\n", 
	(what_mode == BUILD ? TET_JNL_BUILD_START : TET_JNL_CLEAN_START),
	ctr, tcname,zeetime,text,
	(systemid == -1 ? "" : sysidbuf),
	"scenario ref",
	(scenario_line == 0 ? old_line : scenario_line),
	(scenario_element == 0 ? old_element : scenario_element));

    (void) jnl_entry(lineout);
}


/*
 *  Write the tool finish entry to the journal file.
 */
void jnl_tool_end(ctr,status,text,what_mode)
int ctr;
int status;
char *text;
int what_mode;
{
    char lineout[BIG_BUFFER];

    (void) get_time();
    (void) sprintf(lineout,((text == NULL) ? "%d|%d %d %s|\n" :
            "%d|%d %d %s|%s\n"),((what_mode == BUILD) ? TET_JNL_BUILD_END :
            TET_JNL_CLEAN_END),ctr, status,zeetime,text);
    
    (void) jnl_entry(lineout);
}


/*
 *  Write a configuration variable and value to the journal file.
 */
void jnl_config_value(text)
char *text;
{
    char lineout[BIG_BUFFER];

    (void) sprintf(lineout,((text == NULL) ? "%d||\n" : "%d||%s\n"),
        TET_JNL_CFG_VALUE,text);

    (void) jnl_entry(lineout);
}

void jnl_rem_config(rem_id, mode, conf, nconf)
int rem_id;
int mode;
char **conf;
int nconf;
{
    int count;
    char lineout[BIG_BUFFER];

    (void) sprintf(lineout,"%d|remote_%3.3d %d|Config Start\n",TET_JNL_CFG_START,rem_id,mode);
    jnl_entry(lineout);

    for(count=0; count<nconf; count++)
	jnl_config_value(conf[count]);

    (void) sprintf(lineout,"%d||Config End\n",TET_JNL_CFG_END);
    jnl_entry(lineout);
}
	


/*
 *  Write a message contained in the scenario file to the journal
 */
void jnl_entry_scen(text)
char *text;
{
    char lineout[BIG_BUFFER];

    (void) sprintf(lineout,"%d||%s\n",TET_JNL_SCEN_OUT,text);
    (void) jnl_entry(lineout);
}


/*
 *  Write the wonderous TCC startup message to the journal, text
 *  represents the tcc command line.
 */
void jnl_entry_tcc_start(text)
char *text;
{
    int userid;
    char *username;
    struct tm *tp;
    char lineout[BIG_BUFFER];

    (void) time(&time_val);
    /*
     * For START message, include date in YYYYMMDD format
     */
    tp = localtime(&time_val);
    (void)sprintf(zeetime, "%02d:%02d:%02d %4d%02d%02d",
	tp->tm_hour, tp->tm_min, tp->tm_sec, tp->tm_year+1900,
	tp->tm_mon+1, tp->tm_mday);
    userid = (int) getuid();
    username = getlogin();
    if (username == (char *) NULL)
    {
        struct passwd *pw;

        pw = getpwuid((uid_t) userid);
        if (pw == (struct passwd *) NULL || pw->pw_name == (char *) NULL)
            username = "unknown";
        else
            username = pw->pw_name;
    }

    (void) sprintf(lineout,((text == NULL) ?
            "%d|%s %s|User: %s (%d)\n" :
            "%d|%s %s|User: %s (%d) %s\n"), TET_JNL_TCC_START, TET_VERSION,
            zeetime,username,userid,text);
    (void) jnl_entry(lineout);
}


/*
 *  Write the TCC end startup message to the journal
 */
void jnl_entry_tcc_end(text)
char *text;
{
    char lineout[BIG_BUFFER];

    (void) get_time();
    (void) sprintf(lineout,((text == NULL) ? "%d|%s|\n":"%d|%s|%s\n"),
        TET_JNL_TCC_END,zeetime,text);
    (void) jnl_entry(lineout);
}


/*
 *  Write the Invocable Test Case line to the journal
 */
void jnl_entry_invoke_tc(ctr,name,ictext,systemid)
int ctr;
char *name;
char *ictext;
int systemid;
{
    char sysidbuf[13];
    char lineout[BIG_BUFFER];

    (void) get_time();

    if(systemid != -1)
	(void) sprintf(sysidbuf,"sysid %.3d, ", systemid);

    systemid = -1;

    (void) sprintf(lineout,((*ictext == '\0') ? 
	"%d|%d %s %s|TC Start, %s%s %d-%d\n" :
        "%d|%d %s %s|TC Start, %s%s %d-%d%s\n"), 
		TET_JNL_INVOKE_TC,
		ctr, name,zeetime,
		(systemid == -1 ? "" : sysidbuf),
		"scenario ref", 
		(scenario_line == 0 ? old_line : scenario_line),
		(scenario_element == 0 ? old_element : scenario_element), 
		ictext);
    (void) jnl_entry(lineout);
}


/*
 *  Output from a tool or TC during output_capture is written to the journal
 */
void jnl_entry_captured(ctr, text)
int ctr;
char *text;
{
    char lineout[BIG_BUFFER];

    /*
     *  This "special case" is due to the use of newline as a record 
     *  separator in the journal file (and the aesthetic problem people 
     *  have with seeing two newlines in the journal after captured output). 
     *  The use of a byte count, would permit transparent data in the 
     *  journal.
     */

    if ((text != NULL) && (text[strlen(text)-1] == '\n'))
        (void) sprintf(lineout,"%d|%d|%s",TET_JNL_CAPTURED_OUTPUT,ctr,text);
    else
        (void) sprintf(lineout,((text == NULL) ? "%d|%d|\n" : "%d|%d|%s\n"),
            TET_JNL_CAPTURED_OUTPUT,ctr,text);

    (void) jnl_entry(lineout);
}


/*
 *  Write the Test Case finish line to the journal
 */
void jnl_tc_end(ctr,status,text)
int ctr;
int status;
char *text;
{
    char lineout[BIG_BUFFER];

    (void) get_time();
    (void) sprintf(lineout,((text == NULL) ? "%d|%d %d %s|\n" :
            "%d|%d %d %s|%s\n"), TET_JNL_TC_END,ctr,status,
            zeetime,text);
    (void) jnl_entry(lineout);
}


/*
 *  Write the Test Purpose result line to the journal
 */
void jnl_tp_result(ctr,tpnum,status,text)
int ctr;
int tpnum;
int status;
char *text;
{
    char lineout[BIG_BUFFER];

    (void) get_time();
    (void) sprintf(lineout,((text == NULL) ? "%d|%d %d %d %s|\n" :
            "%d|%d %d %d %s|%s\n"), TET_JNL_TP_RESULT,ctr,
            tpnum,status,zeetime,text);

    (void) jnl_entry(lineout);
}


/*
 *  Write the configuration start message, including the config being used,
 *  to the journal.
 */
void jnl_config_start(path,mode)
char *path;
int mode;
{
    char lineout[BIG_BUFFER];

    (void) sprintf(lineout,"%d|%s %d|Config Start\n",TET_JNL_CFG_START, path,mode);
    (void) jnl_entry(lineout);
}


/*
 *  Write the configuration end message to the journal
 */
void jnl_config_end()
{
    char lineout[BIG_BUFFER];

    (void) sprintf(lineout,"%d||Config End\n",TET_JNL_CFG_END);
    (void) jnl_entry(lineout);
}


/*
 *  If the TCC encounters an error it writes a message to the journal.
 *  NOTE: No BAIL_OUTS are permitted in this function !
 */
void jnl_tc_message(text)
char *text;
{
    char lineout[BIG_BUFFER];

    if(jnl_fd > 0)
    {
        (void) sprintf(lineout,"%d||%s", TET_JNL_TC_MESSAGE,
	                        (text == NULL ? "<NULL pointer>\n" : text));
    }
    else
    {
        (void) sprintf(lineout,"ERROR: %s", 
	                        (text == NULL ? "<NULL pointer>\n" : text));
    }

    if(jnl_fd > 0)
    {
        (void) jnl_entry(lineout);
    }
    else
    {
	(void) fprintf(stderr,"%s", lineout);
    }
}


/*
 *  The user has generated a SIGINT and aborted the TC - write a message
 *  to the journal file.
 */
void jnl_user_abort(text)
char *text;
{
    char lineout[BIG_BUFFER];

    (void) get_time();
    (void) sprintf(lineout,((text == NULL) ? "%d|%s|\n" : "%d|%s|%s\n"),
        TET_USER_ABORT, zeetime, text);
    (void) jnl_entry(lineout);
}


/*
 *  Write the parallel start message to the journal.
 */
void jnl_para_start()
{
    char lineout[BIG_BUFFER];

    (void) get_time();
    (void) sprintf(lineout, "%d||Parallel Start, scenario ref %d-%d\n", 
	TET_JNL_PRL_START, (scenario_line == 0 ? group_line : scenario_line),
	(scenario_element == 0 ? group_element : scenario_element));
    (void) jnl_entry(lineout);
}


/*
 *  Write the group end message to the journal
 */
void jnl_para_end()
{
    char lineout[BIG_BUFFER];

    (void) sprintf(lineout,"%d||Parallel End\n",TET_JNL_PRL_END);
    (void) jnl_entry(lineout);
}


/*
 *  Write the repeat start message to the journal.
 */
void jnl_repeat_start(repeat_num)
int repeat_num;
{
    char lineout[BIG_BUFFER];

    (void) get_time();
    (void) sprintf(lineout, "%d|%d|Repeat Start, scenario ref %d-%d\n", 
	TET_JNL_RPT_START, repeat_num, scenario_line, scenario_element);
    (void) jnl_entry(lineout);
}


/*
 *  Write the repeat end message to the journal
 */
void jnl_repeat_end()
{
    char lineout[BIG_BUFFER];

    (void) sprintf(lineout,"%d||Repeat End\n",TET_JNL_RPT_END);
    (void) jnl_entry(lineout);
}


/*
 *  Write the remote start message to the journal.
 */
void jnl_remote_start(rem_directives)
char *rem_directives;
{
    char lineout[BIG_BUFFER];

    (void) sprintf(lineout, "%d|%s|Remote Start, scenario ref %d-%d\n", 
	TET_JNL_RMT_START, rem_directives, scenario_line, scenario_element);
    (void) jnl_entry(lineout);
}


/*
 *  Write the remote end message to the journal
 */
void jnl_remote_end()
{
    char lineout[BIG_BUFFER];

    (void) sprintf(lineout,"%d||Remote End\n",TET_JNL_RMT_END);
    (void) jnl_entry(lineout);
}


/*
 *  Does the line buf correspond to a TP start line ?
 */
int tpstart(buf)
char *buf;
{
    int f, n;

    n = sscanf(buf,"%d",&f);

    if (n != 1)
    {
        (void) sprintf(error_mesg,"results file entry has bad format: %s\n", buf);
        ERROR(error_mesg);
        return(FAILURE);
    }

    return (f == TET_JNL_TP_START);
}


/*
 *  Does the line buf correspond to a Invocable component start line ?
 */
int icstart(buf)
char *buf;
{
    int f, n;

    n = sscanf(buf,"%d",&f);

    if (n != 1)
    {
        (void) sprintf(error_mesg,"results file entry has bad format: %s\n", buf);
        ERROR(error_mesg);
        return(FAILURE);
    }

    return (f == TET_JNL_IC_START);
}


/*
 *  Does the line buf correspond to a Invocable component start line ?
 */
int icend(buf)
char *buf;
{
    int f, n;

    n = sscanf(buf,"%d",&f);

    if (n != 1)
    {
        (void) sprintf(error_mesg,"results file entry has bad format: %s\n", buf);
        ERROR(error_mesg);
        return(FAILURE);
    }

    return (f == TET_JNL_IC_END);
}



/*
 *  Does the line buf correspond to a Test Purpose end line ?
 */
int tpend(buf)
char *buf;
{
    int f, n;

    n = sscanf(buf,"%d",&f);

    if (n != 1)
    {
        (void) sprintf(error_mesg,"results file entry has bad format: %s\n", buf);
        ERROR(error_mesg);
        return(FAILURE);
    }

    return (f == TET_JNL_TP_RESULT);
}


/*
 * Does the line buf correspond to a start line, and if so what type of mode
 * does the start line represent, returns the context.
 */
int modestart(buf)
char *buf;
{
    int f, f2, n, rc;

    n = sscanf(buf,"%d",&f);

    if (n != 1)
    {
        (void) sprintf(error_mesg,"Old journal file entry has bad format: %s\n", buf);
        FATAL_ERROR(error_mesg);
    }
    else
    {
        /* determine the type of mode and return a corresponding value */
        switch(f)
        {
        case TET_JNL_BUILD_START:
	    rc = 1;
	    break;
        case TET_JNL_INVOKE_TC:
	    rc = 2;
	    break;
        case TET_JNL_CLEAN_START:
	    rc = 3;
	    break;
        default:
	    rc = 0;
	    break;
        }
    }

    if(rc >= 1 && rc <= 3)
    {
	/* "mode_num|context tc_name time|mode_name Start, scenario line X" */
        n = sscanf(buf,"%*d|%*d %*[^|]|%*s %*[^,],%*s %*s %d-%d",&f, &f2);

        if (n != 2)
        {
            (void) sprintf(error_mesg,"Old journal file entry has bad format: \"%s\" expected : \"%s\"\n", buf, "<mode_start_num>|<activity> <tc_name> <time>|<mode_name> Start, scenario ref <num>-<element>");
            FATAL_ERROR(error_mesg);
        }
	else
	{
	    old_line = f;
	    old_element = f2;
	}
    }

    return(rc);
}


/*
 * Does the line buf correspond to an end line, and if so what type of mode
 * does the start line represent returns the context which is used in rerun.
 */
int modeend(buf)
char *buf;
{
    int f, n;

    n = sscanf(buf,"%d",&f);

    if (n != 1)
    {
        (void) sprintf(error_mesg,"results file entry has bad format: %s\n", buf);
        ERROR(error_mesg);
        return(FAILURE);
    }

    /* determine the type of mode and return a corresponding value */
    switch(f)
    {
        case TET_JNL_BUILD_END:
            return(1);
        case TET_JNL_TC_END:
            return(2);
        case TET_JNL_CLEAN_END:
	    return(3);
    default:
        return(0);
    }
}

int directivestart(buf)
char *buf;
{
    int f, f2, n;

    n = sscanf(buf,"%d",&f);

    if (n != 1)
    {
        (void) sprintf(error_mesg,"results file entry has bad format: %s\n", buf);
        ERROR(error_mesg);
        return(FAILURE);
    }
    switch(f)
    {
	case TET_JNL_RMT_START:
	    return(1);
	case TET_JNL_PRL_START:
	    /* "mode_num||Group Start, scenario line X" */
            n = sscanf(buf,"%*d||%*s %*[^,],%*s %*s %d-%d",&f, &f2);
            if (n != 2)
            {
                (void) sprintf(error_mesg,"Old journal file entry has bad format: \"%s\" expected : \"%s\"\n", buf, "<para_start_num>||Parallel Start, scenario ref <num>-<element>");
                FATAL_ERROR(error_mesg);
            }
	    else
	    {
	        group_line = f;
	        group_element = f2;
	    }
	    return(2);
	case TET_JNL_RPT_START:
	    return(3);
        default:
            return(0);
    }
}

int directiveend(buf)
char *buf;
{
    int f, n;

    n = sscanf(buf,"%d",&f);

    if (n != 1)
    {
        (void) sprintf(error_mesg,"results file entry has bad format: %s\n", buf);
        ERROR(error_mesg);
        return(FAILURE);
    }
    switch(f)
    {
	case TET_JNL_RMT_END:
	    return(1);
	case TET_JNL_PRL_END:
	    return(2);
	case TET_JNL_RPT_END:
	    return(3);
        default:
            return(0);
    }
}


/*
 *  Checks a line to see if its too long - and sorts it out if it is
 */
void truncate_line(file_ptr, buf, max_size)
FILE *file_ptr;
char *buf;
int  max_size;
{
	char local_buf[JNL_LINE_MAX + 1];
	char *ret;

	if(strlen(buf) < (size_t) max_size)
		return;

	if(strlen(buf) == max_size)
	{
		if(buf[max_size-1] == '\n')
			return;
	}

	/* time to do some truncating */

	DBUG(DBGFINFO)(stderr,"truncating results file line that begins: \"%30s\"\n", buf);
	/* truncate line */
	buf[max_size-1] = '\n';
	buf[max_size] = '\0';

	/* advance file pointer to newline or EOF */
	while(feof(file_ptr) == 0)
	{
		ret = fgets(local_buf,sizeof(local_buf),file_ptr);

		if(ret == NULL)
			break;

		if(strlen(local_buf) < (size_t) JNL_LINE_MAX)
			break;

		if(strlen(local_buf) == JNL_LINE_MAX)
			if(local_buf[JNL_LINE_MAX] == '\n')
				break;
	}

	return;
}
