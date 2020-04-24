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
static char sccsid[] = "@(#)syntax.c	1.3 (92/11/27) DTET release 1.0b";
#endif

/************************************************************************

SCCS:           @(#)syntax.c    1.3 11/27/92
NAME:		syntax.c
PRODUCT:        DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:         This source file is based on Release 1.10 of the TET
DATE CREATED:   February 1992
CONTENTS: 	Functions to check the syntax of the scenario file.

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

/* defines used in scenario syntax checking */
#define NONE        0

#define AFILE       12
#define ENDAFILE    13
#define SCENARIO    14
#define ENDSCENARIO 15

#define END_OF_FILE	-1
#define SCEN_START	1
#define SCEN_ELEMENT	0
#define BAD_MESSAGE	-2

struct sysid_store *all_sysids = NULL;
int total_sysid = 0;

static int syntax_err = FALSE;
static char *dbug_line;


int check_scenario(scen_file, scen_name)
char *scen_file;
char *scen_name;
{
    int store_flag;

    DBUG(DBGFDESC) (stderr,"check_scenario(scen_file:%s, scen_name:%s)\n", scen_file, scen_name);

    dbug_line = (char *)TET_MALLOC(sizeof(char)*INPUT_LINE_MAX);

    store_flag = debug_flag;

    if((debug_flag & DBGSYNTAX) == 0)
        debug_flag = 0;
    
    /* scan_scen(file name, repeat flag, group flag, remote flag, main_scen) */
    if(!scan_scen(scen_file, scen_name, 0, 0, 0, TRUE))
    {
        (void) sprintf(error_mesg,"Can't open scenario file : \"%s\"\n", scen_file);
        BAIL_OUT2(error_mesg);
    }

    debug_flag = store_flag;

    TET_FREE(dbug_line);

    if(syntax_err == TRUE)
	return(FAILURE);
    else
	return(SUCCESS);
}


/* Function to open a scenario file and check the syntax of the directives
 * within it.
 * The function is indirectly called recursively via the 'match' function 
 */
int scan_scen(file_name, scen_name, repeat, group, remote, main_scen)
char *file_name;
char *scen_name;
int repeat;
int group;
int remote;
int main_scen;
{
    FILE *scen_fp;
    char buf[INPUT_LINE_MAX];
    char *line_ptr;
    int line_number=0;
    /* 'new_file_name' is used by 'match' to store a new file name to re-call
     * 'scan_scen' with.
     * The variable is only indirectly used in the function.
     * Using a local variable in this way saves calling malloc from within
     * 'match' and therfore this method simplifies the program.
     */
    char new_file_name[_POSIX_PATH_MAX];

    DBUG(DBGSYNTAX) (stderr,"scan_scen(file_name:%s, scen_name:%s, repeat:%d, group:%d, remote:%d, main_scen:%d)\n",file_name, scen_name, repeat,group,remote,main_scen);

    *new_file_name = '\0';
    line_ptr=buf;

    /* Open the scenario file */
    if((scen_fp = fopen(file_name,"r")) == NULL)
    {
	return(FALSE);
    }
    else
    {
	DBUG(DBGFILES)(stderr,"fopen(%ld)\n", (long) scen_fp);
    }

    /* make sure the file descriptor is closed on an exec call  */
    DBUG(DBGFILES)(stderr,"fcntl(%d, F_SETFD, FD_CLOEXEC)\n", (int) fileno(scen_fp));
    (void) fcntl(fileno(scen_fp), F_SETFD, FD_CLOEXEC);

    if(main_scen)
    {
	/* If this is the main scenario file we must find the correct 
         * scenario before we proceed.
         */

        *buf = '\0';
	if((line_number = find_scen(scen_fp,buf,&line_ptr,scen_name)) == FALSE)
	{
	    syntax_error("Scenario name not found",file_name, NULL,0,TRUE);
	}

	/* Match the end of the scenario */
        if(!match(file_name,scen_name,new_file_name,scen_fp,buf,&line_ptr,&line_number,SCENARIO,repeat,group,remote))
	{
	    syntax_error("End of scenario not reached",file_name,NULL,0,TRUE);
	}
    }
    else
    {
	/* Match the end of the file */
        if(!match(file_name,scen_name,new_file_name,scen_fp,buf,&line_ptr,&line_number,AFILE,repeat,group,remote))
	{
	    syntax_error("Did not reach end of file due to previous error",file_name,NULL,0,TRUE);
	}
    }
    return(TRUE);
}


/*
 *  Function to match the start and end of directives, files and scenarios.
 *  This is called from 'scan_scen' and may also call itself.
 */
int match(file_name, scen_name, new_file_name,scen_fp,buf,line_ptr_ad,line_number_ad,match_type,repeat,group,remote)
char *file_name;
char *scen_name;
char *new_file_name;
FILE *scen_fp;
char *buf;
char **line_ptr_ad;
int *line_number_ad;
int match_type;
int repeat;
int group;
int remote;
{
    int ret_val;
    int type;
    char *line_ptr;  /* Local line pointer only needed if return point is
                      * required to trace back to the directive for which a 
                      * match was not found. */ 
    char *tmp_ptr;
    char *tmp_ptr2;
    int line_number; /* Local line number (doesn't change if a directive match
                      * isn't found).
                      */
    int matched = FALSE; /* Found end to the directive 
                          * (eg :endremote: to :remote, directives)
                          */

    DBUG(DBGSYNTAX) (stderr,"match(file_name:%s, scen_name:%s,<ptr(file)>,%s,<ptr(line)>,<ptr(l_num)>,%d,%d,%d,%d)\n",file_name,scen_name,buf,match_type,repeat,group,remote);

    line_number = *line_number_ad;
    line_ptr = *line_ptr_ad;

    while( !matched && syntax_err == FALSE )
    {
            ret_val=get_scenario_item(scen_fp,buf, &line_ptr, &line_number);
	    switch(ret_val)
	    {
		case END_OF_FILE: /* End of file */
		    type = ENDAFILE;
		    break;
		case SCEN_START: /* New scenario found */
		    type = ENDSCENARIO;
		    break;
		case SCEN_ELEMENT: /* Normal directive or testcase */
	            type = analyse_line(line_ptr,&line_ptr);
	            if(line_ptr == NULL) /* Analyse line sets the pointer to 
                                          * NULL under certain circumstances
					  */
	            {
		        line_ptr = buf;
		        *line_ptr = '\0';
		    }
		    break;
		case BAD_MESSAGE:
		    type = EMPTY; /* Ignore this line */	
		    line_ptr = buf;
		    *line_ptr = '\0';
		    syntax_error("End message quote missing",file_name,dbug_line,line_number,FALSE);
		    break;
	    }    
	    switch(type)
	    {
	        case EMPTY:
		    DBUG(DBGSYNTAX) (stderr,"Empty line.\n");
		    break;
		case TESTCASE:
		    DBUG(DBGSYNTAX) (stderr,"Testcase.\n");
		    break;
		case MESSAGE:
		    DBUG(DBGSYNTAX) (stderr,"Message.\n");
		    break;
		case FAILURE:
			    syntax_error("Invalid directive found",file_name,dbug_line,line_number,FALSE);
			    break;
			case REPEAT:
			    if(group > 0)
			    {
				syntax_error("':repeat,' directive found within a group directive",file_name, dbug_line,line_number,TRUE);
			    }
			    if(remote > 0)
			    {
				syntax_error("':repeat,' directive found within a remote directive",file_name, dbug_line,line_number,TRUE);
			    }
			    line_ptr += strlen(REPEAT_STR);

			    parse_param(TRUE,new_file_name,&line_ptr);
			    if(*new_file_name != '\0') /* if we have a file name */
			    {
				if(!scan_scen(new_file_name, scen_name, repeat+1,group,remote,FALSE))
				{
				    syntax_error("Scenario file for ':repeat,' directive not found",file_name,dbug_line,line_number, FALSE); 
				}
			    }
			    else
			    {
				if(!match(file_name,scen_name,new_file_name,scen_fp,buf,&line_ptr,&line_number,REPEAT,repeat+1,group,remote))
				{
				    go_to_line(scen_fp,buf,line_number);
				    syntax_error("Parser expected matching ':endrepeat:' directive for",file_name,dbug_line,line_number,TRUE);
				}
			    }
			    break;
			case ENDREPEAT:
			    line_ptr += strlen(END_REPEAT_STR);
			    if(match_type != REPEAT)
			    {
				syntax_error("Unexpected ':endrepeat:' found",file_name,dbug_line,line_number,FALSE);
			    }
			    else
				matched = TRUE;
			    break;
			case PARALLEL:
			    if(group > 0)
			    {
				syntax_error("':group:' directive found within a group directive",file_name, dbug_line,line_number,TRUE);
			    }
			    if(remote > 0)
			    {
				syntax_error("':group:' directive found within a remote directive",file_name, dbug_line,line_number,TRUE);
			    }
			    line_ptr += strlen(PARALLEL_STR);
			    parse_param(FALSE,new_file_name,&line_ptr);
			    if(*new_file_name != '\0') /* if we have a file name */
			    {
				(void) fprintf(stderr,"Filename is '%s'.\n",new_file_name);
				if(!scan_scen(new_file_name,scen_name,repeat,group+1,remote,FALSE))
				{
				    syntax_error("Scenario file for ':group:' directive not found",file_name, dbug_line,line_number,FALSE); 
				}
			    }
			    else
			    {
				if(!match(file_name,scen_name,new_file_name,scen_fp,buf,&line_ptr,&line_number,PARALLEL,repeat,group+1,remote))
				{
				    go_to_line(scen_fp,buf,line_number);
				    syntax_error("Parser expected ':endrepeat:' directive for",file_name,dbug_line,line_number,TRUE);
				}
			    }
			    break;
			case ENDPARALLEL:
			    if(match_type != PARALLEL)
			    {
				syntax_error("Unexpected ':endgroup:' found",file_name,dbug_line,line_number,FALSE);
			    }
			    else
				matched = TRUE;
			    line_ptr += strlen(END_PARALLEL_STR);
			    break;
			case REMOTE:
			    if(remote > 0)
			    {
				syntax_error("':remote,' directive found within a remote directive",file_name, dbug_line, line_number,TRUE);
			    }
			    line_ptr += strlen(REMOTE_STR);
			    tmp_ptr = line_ptr;
			    while( !isspace(*tmp_ptr)
				    &&
				   (*tmp_ptr != DIRECTIVE_LIMITER)
				    &&
				   (*tmp_ptr != '\0') )
			    {
				tmp_ptr++;
			    }
			    if(*tmp_ptr != DIRECTIVE_LIMITER)
			    {
				syntax_error("Remote designations missing from remote directive",file_name, dbug_line, line_number, TRUE);
			    }
			    tmp_ptr = store_str(line_ptr);
			    tmp_ptr2 = strchr(tmp_ptr,DIRECTIVE_LIMITER);
			    *tmp_ptr2 = '\0';
			    if((total_sysid=str_to_sysids(&all_sysids,tmp_ptr,total_sysid))==FAILURE)
			    {
				syntax_error("Format error for remote designations in remote directive",file_name, dbug_line, line_number,TRUE);
			    }
			    TET_FREE(tmp_ptr);
			    line_ptr = strchr(line_ptr,DIRECTIVE_LIMITER);
			    line_ptr++; /* Move beyond DIRECTIVE_LIMITER */
			    parse_param(FALSE,new_file_name,&line_ptr);
			    if(*new_file_name != '\0') /* if we have a file name */
			    {
				if(!scan_scen(new_file_name,scen_name,repeat,group,remote+1,FALSE))
				{
				    syntax_error("Scenario file for ':remote,' directive not found",file_name, dbug_line,line_number,TRUE); 
				}
			    }
			    else
			    {
				if(!match(file_name,scen_name,new_file_name,scen_fp,buf,&line_ptr,&line_number,REMOTE,repeat,group,remote+1))
				{
				    go_to_line(scen_fp,buf,line_number);
				    syntax_error("Parser expected matching ':endremote:' directive for",file_name,dbug_line,line_number,TRUE);
				}
			    }
			    break;
			case ENDREMOTE:
			    if(match_type != REMOTE)
			    {
				syntax_error("Unexpected ':endremote:' directive found",file_name,dbug_line,line_number,FALSE);
			    }
			    else
				matched = TRUE;
			    line_ptr += strlen(END_REMOTE_STR);
			    break;
			case INCLUDE:
			    line_ptr += strlen(INCLUDE_STR);
			    parse_param(FALSE,new_file_name,&line_ptr);
			    if(*new_file_name != '\0')
			    {
				if( !scan_scen(new_file_name,scen_name,repeat,group,remote,FALSE))
				{
				    syntax_error("Scenario file for ':include:' directive not found",file_name, dbug_line,line_number,TRUE); 
				}
			    }
			    else
			    {
				syntax_error("Scenario file to include missing",file_name,dbug_line,line_number,FALSE);
			    }
			    break;
			case ENDAFILE:
			    if( (match_type != AFILE) && (match_type != SCENARIO) )
			    {
				/* This is pretty fatal */
				syntax_error("Unexpected end of file found",file_name,NULL,0,FALSE);
			    }
			    else
				matched = TRUE;
			    break;
			case ENDSCENARIO:
			    if( match_type != SCENARIO )
			    {
				syntax_error("Unexpected start of new scenario found",file_name,dbug_line,line_number,FALSE);
		    }
		    else
			matched = TRUE;
		    break;
	    }
    }

    *line_ptr_ad = line_ptr;
    *line_number_ad = line_number;
    
    if(syntax_err == TRUE)
	return(FALSE);
    else
	return(TRUE);
}


int find_scen(scen_fp,buf,line_ptr_ad,scen_name)
FILE *scen_fp;
char *buf;
char **line_ptr_ad;
char *scen_name;
{
    int scen_line=0;
    int ret_val; 


    DBUG(DBGSYNTAX) (stderr,"find_scen(<ptr(file)>,%s,<ptr(line)>,%s)\n",buf,scen_name);


    *line_ptr_ad = buf;
    while( (ret_val=get_scenario_item(scen_fp,buf,line_ptr_ad,&scen_line)) >= 0)
    {
	if( ret_val == SCEN_START)
	{
	    if(strncmp(buf,scen_name,strlen(scen_name)) == 0)
		return(scen_line);
	}
        *line_ptr_ad = buf;
	**line_ptr_ad = '\0'; /* Force 'get_scenario_item' to get next line */
    }
    return(FALSE);
}


void go_to_line(file_pointer,buf,line_number)
FILE *file_pointer;
char *buf;
int line_number;
{
    int current_line=0;

    rewind(file_pointer);
    for(current_line=0;current_line<line_number;current_line++)
    {
        if(fgets(buf,INPUT_LINE_MAX,file_pointer) == NULL)
            return; /* End of file */
    }
    if(buf[strlen(buf)-1] == '\n')
	buf[strlen(buf)-1] = '\0';

    return;
}


/* Function to get a file name if it exists after a directive */
void parse_param(ints_expected,file_name,line_ptr_ad)
int ints_expected;
char *file_name;
char **line_ptr_ad;
{
    char *alt_exec_dir;
    char *ts_root;

    DBUG(DBGSYNTAX) (stderr,"parse_param(%d,%s,<ptr(line:'%s')>)\n",ints_expected,file_name,*line_ptr_ad);

    ts_root = get_ts_root(FALSE, MASTER);
    alt_exec_dir = get_alt_exec_dir(FALSE, MASTER);

    if(ints_expected) /* If integer parameters are expected. eg. remote
                       * designations after a remote directive.
                       */
    {
	while( (**line_ptr_ad != ':') && (**line_ptr_ad != '\0') )
	    (*line_ptr_ad)++;

	if(**line_ptr_ad == '\0')
	    return;

	(*line_ptr_ad)++;
    }

    /* If a filename is present after the directive, copy it. */
    if(**line_ptr_ad == TC_START_MARK)
    {
	if(alt_exec_dir != NULL) /* If there is an alternative path defined */
	{
	    (void) strcpy(file_name,alt_exec_dir);
	}
	else
	{
	    (void) strcpy(file_name,ts_root);
	}
	file_name += strlen(file_name);
        while(!isspace(**line_ptr_ad) && (**line_ptr_ad != '\0') )
	    *(file_name++) = *((*line_ptr_ad)++);
    }
    *file_name = '\0';
    return;
}


void syntax_error(error_message,pathname,buf,line_number,bail_out)
char *error_message;
char *pathname;
char *buf;
int line_number;
int bail_out;
{
    char *file_name;

    file_name = basename(pathname);

    (void) strcpy(error_mesg, error_message);

    if(buf != NULL)
    {
        (void) sprintf(error_mesg,"%s '%s'",error_mesg, buf);
    }

    (void) sprintf(error_mesg, "%s in file %s", error_mesg, file_name);

    if(line_number != 0)
    {
        (void) sprintf(error_mesg,"%s line %d", error_mesg, line_number);

    }

    (void) strcat(error_mesg,"\n");

    if(bail_out)
    {
        FATAL_ERROR(error_mesg);
    }
    else
    {
	ERROR(error_mesg);
	syntax_err = TRUE;
    }
}

/* Function to find the position of the first scenario directive on a line */
/* If the function is passed a blank line it will read a new line from the
 * file specified by 'file_ptr'
 */
/* 'line_ptr_ad' should be the address of a pointer to the current character
 * of 'line'. 
 */

int get_scenario_item(file_pointer, buf, line_ptr_ad, line_number_ad)
FILE *file_pointer;
char *buf;
char **line_ptr_ad;
int *line_number_ad;
{
    int ret_val;

    DBUG(DBGSYNTAX) (stderr,"get_scenario_item(<ptr(file)>,%s,<ptr(line:'%s')>)\n",buf,*line_ptr_ad);

    /* If we are not pointing at a space, we must be still on the last 
     * directive so move the pointer until we find a space
     */
    while(!isspace(**line_ptr_ad) && **line_ptr_ad!='\0')
        (*line_ptr_ad)++;

    if( (ret_val = find_item(line_ptr_ad)) != SUCCESS )
    {
        DBUG(DBGSYNTAX) (stderr,"get_scenario_item returning ERROR: %d : '%s'\n",ret_val, *line_ptr_ad);
	return(ret_val);
    }

    if(**line_ptr_ad == '\0')
    {
	/* End of line has been reached */

        if(fgets(buf,INPUT_LINE_MAX,file_pointer) == NULL)
	{

            DBUG(DBGSYNTAX) (stderr,"get_scenario_item returning END_OF_FILE : '%s'\n",*line_ptr_ad);

            return(END_OF_FILE); /* End of file */
	}

	if(buf[strlen(buf)-1] == '\n') /* remove return char if it exists */
	    buf[strlen(buf)-1]='\0';

	(void) strcpy(dbug_line, buf);

	*line_ptr_ad=buf; /* Reset line pointer */

	(*line_number_ad)++; /* Increment line number */

	if( !isspace(*buf) && (*buf != COMMENT_CHAR) && (*buf != '\0'))
        {

            DBUG(DBGSYNTAX) (stderr,"get_scenario_item returning SCEN_START : '%s'\n",*line_ptr_ad);

	    return(SCEN_START); /* This is the beginning of new scenario */
        }

        if( (ret_val = find_item(line_ptr_ad)) != SUCCESS )
	{
            DBUG(DBGSYNTAX) (stderr,"get_scenario_item returning ERROR: %d : '%s'\n",ret_val, *line_ptr_ad);
	    return(ret_val);
	}
    }

    DBUG(DBGSYNTAX) (stderr,"get_scenario_item returning SCEN_ELEMENT : '%s'\n",*line_ptr_ad);

    return(SCEN_ELEMENT); 
}

int find_item(line_ptr_ad)
char **line_ptr_ad;
{
    int ret_val = SUCCESS;

    DBUG(DBGSYNTAX) (stderr,"find_item(<ptr(line:'%s')>)\n", *line_ptr_ad);

    /* Search until we find something other than a space */
    while(isspace(**line_ptr_ad) && **line_ptr_ad!='\0')
        (*line_ptr_ad)++;

    if( **line_ptr_ad == MESSAGE_CHAR)
    {
	do
	{
            (*line_ptr_ad)++;
	}
        while( (**line_ptr_ad != MESSAGE_CHAR) && (**line_ptr_ad != '\0') );

	if( **line_ptr_ad == '\0' )
	    ret_val = BAD_MESSAGE;
    }
    return(ret_val);
}
