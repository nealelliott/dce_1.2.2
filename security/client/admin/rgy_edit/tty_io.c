/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_io.c,v $
 * Revision 1.1.8.2  1996/03/11  13:27:17  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:10  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:01:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:36  root]
 * 
 * Revision 1.1.6.3  1993/10/22  19:49:42  desai
 * 	CR 9239 fix. Backed-out current changes associated with \ not escaping in
 * 	command line problem.
 * 	[1993/10/22  19:49:08  desai]
 * 
 * Revision 1.1.4.2  1992/12/29  12:41:54  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:36:31  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/08  18:20:45  burati
 * 	Don't restrict names to 256 chars
 * 	[1992/05/07  23:12:02  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  tty_io.c V=19 11/13/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit tty interface i/o utilities 
**
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)tty_io.c	19 - 11/13/91";
#endif

#include <stdio.h>
#include <ctype.h>
#include <un_strng.h>
#include <un_io.h>

#include <macros.h>

#include <ek_types.h>
#include <tty_base.h>
#include <tty_err.h>
#include <tty_util.h>
#include <tty_io.h>
#include <tty_conf.h>

#include <commands.h>
#include <args.h>

/* a null conversion table for i/o strings that can pass between the tty 
 * interface and the kernel "as is"
 */
PRIVATE tty_io_map_t tty_null_map[] = { 0, 0 };
tty_io_conversion_table_t tty_null_conv_table = { tty_null_map, 0, false };

PRIVATE tty_io_map_t tty_flag_map[] = {
    { tty_y,             ek_flag_on  },
    { tty_n,             ek_flag_off },
    { tty_yes,           ek_flag_on  },
    { tty_no,            ek_flag_off }
};
PUBLIC tty_io_conversion_table_t tty_flag_conv_table = { tty_flag_map, 4, true };


/* For those funny situations where a yes response turns a flag off */
PRIVATE tty_io_map_t tty_flag_yes_off_map[] = {
    { tty_yes,           ek_flag_off },
    { tty_no,            ek_flag_on  },
    { tty_y,             ek_flag_off },
    { tty_n,             ek_flag_on  }
};
PUBLIC tty_io_conversion_table_t tty_flag_yes_off_conv_table = 
     { tty_flag_yes_off_map, 4, true};


PRIVATE tty_io_map_t tty_lifespan_map[] = {
    { tty_lifespan_infinite,  ek_lifespan_infinite }
};
PUBLIC tty_io_conversion_table_t tty_lifespan_conv_table = 
       { tty_lifespan_map, 1, false };


PRIVATE tty_io_map_t tty_date_map[] = {
    { tty_date_none,    ek_null_date   }
};
PUBLIC tty_io_conversion_table_t tty_date_conv_table = 
    { tty_date_map, 1, false };


PRIVATE tty_io_map_t tty_acct_abbrev_map[] = {
    { tty_acct_abbrev_person,  ek_acct_key_person },
    { tty_acct_abbrev_group,   ek_acct_key_group  },
    { tty_acct_abbrev_org,     ek_acct_key_org    }
};
PUBLIC tty_io_conversion_table_t tty_abbrev_conv_table = 
    { tty_acct_abbrev_map, 3, true }; 

PRIVATE tty_io_map_t tty_uid_map[] = {
    { tty_uid_unknown,  ek_pgo_unknown_uid }
};
PUBLIC tty_io_conversion_table_t  tty_uid_conv_table =
    { tty_uid_map, 1, false };


/* 
** Private routines
*/


PRIVATE void make_lower
#ifndef __STDC__
    ( buf ) 
   char  *buf;
#else
  (
   char  *buf
  )
#endif
{
    while (*buf != '\0') {
        if (isupper(*buf)) *buf = tolower(*buf);
        buf++;
    }
}

PRIVATE char *dostring
#ifndef __STDC__
    ( buf ) 
    char  *buf;
#else
  (
    char  *buf
  )
#endif
{
    char *start = buf;

    if (*buf++ != '\0') {
	return buf;
    }
    
    while (*buf != '"') {
	if (*buf == '\\') buf++;
	if (*buf == '\0') break;
	buf++;
    }
    /* get rid of opening quote - by sliding string over */
    if (*buf == '"') {
	*buf = '\0';
	strcpy(start, start+1);
	strcpy(buf-1, buf+1);
	buf -= 2;
   } else {
       strcpy(start, start+1);
   }
    
    return buf;
}


/* tty_trim
 *
 * trims leading and trailing whitespace from a string
 */
PRIVATE void tty_trim
#ifndef __STDC__
    ( str ) 
     char  *str;
#else
  (
     char  *str
  )
#endif
{
    
    char *nonwhite;

    if (*str == '\0') 
	return;

    nonwhite = str;
        /* skip to first non-white character */
    while (isspace(*nonwhite))
	nonwhite++;
        
    if (nonwhite != str) {           /* got some leading whitespace */
        if (*nonwhite == '\0') {
	    *str = '\0';
	    return;                  /* maybe the whole string was whitespace? */
	}
	while (*nonwhite != '\0') 
	    *str++ = *nonwhite++;
	*str = '\0';
    } else {                           /* advance to end of str */
	str += strlen(str);
    }
    
        /* walk backwards through str trashing whitespace chars with a null */
    str--;
    while (isspace(*str)) 
	*str-- = '\0';
}


/* 
** Public routines
*/


#define TTY_MAX_IO_BUFF_SIZE  ek_lim_max_pname_len

    /* static storage for i/o conversion return values */
PRIVATE char tty_io_map_buff[TTY_MAX_IO_BUFF_SIZE];
PRIVATE char kernel_io_map_buff[TTY_MAX_IO_BUFF_SIZE];

char  *tty_get_kernel_io_val
#ifndef __STDC__
    ( tty_key, table, match  ) 
    char                       *tty_key;
    tty_io_conversion_table_t  *table;
    boolean32                  *match;
#else
  (
    char                       *tty_key,
    tty_io_conversion_table_t  *table,
    boolean32                  *match
  )
#endif
{
    tty_io_map_t               *table_index;
    boolean32                  found;
    char                       *ret_val = kernel_io_map_buff;
    int                        i;

    /* loop through table until we find a match for tty_key, or the end of
     * the table is reached */
    found = false;
    for (i = 0, table_index = table->table; 
	 i < table->num_entries && !found; 
	 i++, table_index++) {
	if (strcmp(table_index->tty_val, tty_key) == 0) {
	    found = true;
	    strncpy(kernel_io_map_buff, table_index->kernel_val, 
		    TTY_MAX_IO_BUFF_SIZE);
	    kernel_io_map_buff[TTY_MAX_IO_BUFF_SIZE -1] = '\0'; 
	}
    }
    
    if (!found) {
	if (table->one_to_one) {
	    ret_val = NULL;
	    *match = false;
	} else {
	    strncpy(kernel_io_map_buff, tty_key, TTY_MAX_IO_BUFF_SIZE);
	    kernel_io_map_buff[TTY_MAX_IO_BUFF_SIZE -1] = '\0'; 
            *match = true;
	}
    } else
	*match = true;

    return ret_val;
}
	    

char *tty_get_if_io_val
#ifndef __STDC__
    ( kernel_key, table, match ) 
    char                       *kernel_key;
    tty_io_conversion_table_t  *table;
    boolean32                  *match;
#else
  (
    char                       *kernel_key,
    tty_io_conversion_table_t  *table,
    boolean32                  *match
  )
#endif
{
    tty_io_map_t               *table_index;
    boolean32                  found;
    char                       *ret_val = tty_io_map_buff;
    int                        i;

    /* loop through table until we find a match for kernel_key, or the end of 
     * the table is reached
     */
    found = false;
    for (i = 0, table_index = table->table;
	 i < table->num_entries && !found; 
	 i++, table_index++) 
	if (strcmp(table_index->kernel_val, kernel_key) == 0) {
	    strncpy(tty_io_map_buff, table_index->tty_val, TTY_MAX_IO_BUFF_SIZE);
	    tty_io_map_buff[TTY_MAX_IO_BUFF_SIZE -1] = '\0'; 
	    found = true;
	}

    if (!found) {
	if (table->one_to_one) {
	    ret_val = NULL;
	    *match = false;
	} else {
	    strncpy(tty_io_map_buff, kernel_key,  TTY_MAX_IO_BUFF_SIZE);
	    tty_io_map_buff[TTY_MAX_IO_BUFF_SIZE -1] = '\0'; 
            *match = true;
	}
    } else
	*match = true;

    return ret_val;
}

   

PUBLIC void tty_show_prompt
#ifndef __STDC__
    ( prompt ) 
    char  *prompt;
#else
  (
    char  *prompt
  )
#endif
{
    if (isatty(0)) {
	printf("%s", prompt); 
        fflush(stdout);
    }
}

PUBLIC void tty_end_prompt
#ifndef __STDC__
    ( prompt ) 
    char  *prompt;
#else
  (
    char  *prompt
  )
#endif
{
    if (isatty(0))
	printf("%s\n", prompt);
}

PUBLIC void tty_show_default
#ifndef __STDC__
    ( default_str ) 
    char  *default_str;
#else
  (
    char  *default_str
  )
#endif
{
    if (isatty(0))
	printf("(%s) ", default_str);
}

    
/* tty_get_string
 *
 * just reads characters from standard input into a buffer, ignoring any that
 * would exceed the buffer size.  The buffer always returns null terminated, so 
 * pass in something at least 1 character longer than the maximum sensible
 * response you're willing to accept.  Returns false if the input is an
 * immediate newline or EOF,  true otherwise.   
 *
 */
boolean32 tty_get_string
#ifndef __STDC__
    ( buff, buffsize, overflow)  
    char      *buff;
    long      buffsize;
    boolean32 *overflow;
#else
  (
    char      *buff,
    long      buffsize,
    boolean32 *overflow
  )
#endif
{
    long      chars_read = 0;
    char      cur_char;
    char      *buff_ptr = buff;
    boolean32 ret_val;
    boolean32 done;

    if (buffsize < 1)  return false;

    *overflow = false;
    done = false;

    do {
	cur_char = getchar();
	done = (cur_char == EOF || cur_char == '\n');

	if (!done) {
	    if (chars_read < buffsize - 1) {
		*buff_ptr++ = cur_char;
	    } else
		*overflow = true;
	    chars_read++;
	} else if (cur_char == EOF && !feof(stdin)) {
	    done = false;  /* hack to handle spurious EOF due to cma use of EINTR */
	}

    } while (!done);

    if (*buff == EOF || *buff == '\n')    /* check for immediate EOF or \n */
        ret_val = false;
    else
	ret_val = true;

    *buff_ptr = '\0';                     /* before we trash it with a null */

    return ret_val;
}


/* tty_get_arg
 * 
 * Like tty_get_string, but can deal with quoted strings.  returns false on
 * on any unquoted whitespace string, true otherwise.  Reports buffer overflow
 * to standard error.
 */
boolean32 tty_get_arg
#ifndef __STDC__
    ( buff, buffsize, stp ) 
    char            *buff;
    long            buffsize;
    error_status_t  *stp;
#else
  (
    char            *buff,
    long            buffsize,
    error_status_t  *stp
  )
#endif
{
    boolean32       ret_val,
                    overflow;

    SET_STATUS(stp, error_status_ok);
    ret_val = tty_get_string(buff, buffsize, &overflow);
    if (!ret_val) return ret_val;
    
    if (overflow) {
	SET_STATUS(stp, tty_err_input_too_long);
	tty_status_pre_echo(buff, tty_input_too_long, *stp);
	EXIT_ON_ERROR(stp);
	return ret_val;
    }

        /* treat non-quoted whitespace the same as  an immediate newline */
    tty_trim(buff);
    if (*buff == '\0') return false;

    dostring(buff);

    return ret_val;
}


tty_y_n_response_t tty_ask_y_n
#ifndef __STDC__
    ( prompt_string, def_response ) 
    char                *prompt_string;
    tty_y_n_response_t  def_response;
#else
  (
    char                *prompt_string,
    tty_y_n_response_t  def_response
  )
#endif
{
    tty_y_n_response_t  ret_val;
    char                response[sec_rgy_name_t_size],
                        response_lower[sec_rgy_name_t_size];
    boolean32           done = false;
    boolean32           got_one;
    char                local_prompt[256];
    error_status_t      st;

    sprintf(local_prompt, "%s %s ", prompt_string, "[y/n]?");
    SET_STATUS(&st, error_status_ok);

    while (!done) {
	tty_show_prompt(local_prompt);
	switch (def_response) {
	case tty_response_yes:
	    tty_show_default(tty_y);
	    break;
	case tty_response_no:
	    tty_show_default(tty_n);
	    break;
	default:
	    break;
	}

	done = true;
	got_one = tty_get_arg(response, sizeof(response), &st);
	if (!got_one) {   
	    ret_val = def_response;
	    done = true;
	} else if (BAD_STATUS(&st)) {
	    done = false;
	}  else {
	    strcpy(response_lower, response);
	    make_lower(response_lower);
	    if (match_command(tty_yes, response_lower, 1)) {
		ret_val = tty_response_yes;
	    } else if (match_command(tty_no, response_lower, 1)) {
		ret_val = tty_response_no;
	    } else {
		SET_STATUS(&st, tty_input_out_of_range);
		tty_status_post_echo(response, tty_dont_understand, st);
	    }
	}

	EXIT_ON_ERROR(&st);
    }
	    
    return ret_val;
}
     

/* tty_verify
 *
 * if the input is a tty, echoes the prompt string and asks the user for a 
 * [y/n/q/g] reponse.  If input is not from a tty, and the input is not
 * valid, calls eif_fatal to terminate the session.  If the input is from
 * a terminal, keeps looping until the user enters something valid.
 * 
 * returns:
 *   tty_verify_no,
 *   tty_verify_yes,
 *   tty_verify_go,
 *   tty_verify_quit 
 */
tty_verify_response_t tty_verify
#ifndef __STDC__
    ( prompt_string ) 
    char  *prompt_string;
#else
  (
    char  *prompt_string
  )
#endif
{
    tty_verify_response_t  ret_val;
    char                   response[sec_rgy_name_t_size],
                           response_lower[sec_rgy_name_t_size];
    boolean32              done = false;
    boolean32              got_something;
    char                   local_prompt[256];
    error_status_t         st;

    SET_STATUS(&st, error_status_ok);

    while (!done) {
	sprintf(local_prompt, "%s %s ", prompt_string, "[y/n/g/q]?");
	tty_show_prompt(local_prompt);
	got_something = tty_get_arg(response, sizeof(response), &st);
	done = true;     /* assume the user gave a valid answer */
	if (got_something && GOOD_STATUS(&st)) {
	    strcpy(response_lower, response);
	    make_lower(response_lower);
	    if (match_command(tty_yes, response_lower, 1)) {
		ret_val = tty_verify_yes;
	    } else if (match_command(tty_no, response_lower, 1)) {
		ret_val = tty_verify_no;
	    } else if (match_command(tty_go, response_lower, 1)) { 
		ret_val = tty_verify_go;
	    } else if (match_command(tty_quit, response_lower, 1)) { 
		ret_val = tty_verify_quit;
	    } else
		done = false;
	} else 
	    done = false;

	if (!done) {
	    if (GOOD_STATUS(&st)) {
		SET_STATUS(&st, tty_input_out_of_range);
	    }
	    tty_status_post_echo(response, tty_dont_understand, st);
	}

	EXIT_ON_ERROR(&st);
    }
		
    return ret_val;
}


/* tty_get_field_arg
 *
 * This is a complicated function. It's designed to prompt for field input and
 * supply default values for both add and change operations.  If the field
 * contains a value, that value is used as the default.  If the field does not
 * contain a value, and the show_default parameter is true, the value in the
 * default_str parameter is used as the default.  The input parameter returns
 * true if the user enters  something other than the default value in the field,
 * or the user enters  nothing and the default value is not in the field
 * (basically, input=true means the caller needs to write the contents of the 
 * io_buff into the field.)
 */
PUBLIC error_status_t tty_get_field_arg
#ifndef __STDC__
    ( op_handle, field, io_table, show_default, default_str, io_buff,
        io_buff_size, prompt, input )
     ek_op_handle_t             op_handle;
     ek_field_id_t              field;
     tty_io_conversion_table_t  *io_table;
     boolean32                  show_default;
     char                       *default_str;
     char                       *io_buff;
     long                       io_buff_size;
     char                       *prompt;
     boolean32                  *input;
#else
  (
     ek_op_handle_t             op_handle,
     ek_field_id_t              field,
     tty_io_conversion_table_t  *io_table,
     boolean32                  show_default,
     char                       *default_str,
     char                       *io_buff,
     long                       io_buff_size,
     char                       *prompt,
     boolean32                  *input
  )
#endif
{
    long                        default_len;
    char                        *default_buff,
                                *conv_buff;
    boolean32                   io_map,
                                use_field;
    error_status_t              st;

    SET_STATUS(&st, error_status_ok);

    use_field = ek_field_has_data(op_handle, field);
    if (use_field) {
	ek_read_field(op_handle, field, io_buff, io_buff_size, &default_len);
	default_buff = tty_get_if_io_val(io_buff, io_table, &io_map);	    
	show_default = true;    /* always use field value as the default */
    } else if (show_default)
	default_buff = tty_get_if_io_val(default_str, io_table, &io_map);

    if (show_default && !io_map) {
	SET_STATUS(&st, tty_input_out_of_range);
	TTY_FATAL("io mapping error in tty_get_field_arg", &st);	
    }

    tty_show_prompt(prompt);
    if (show_default)    
	tty_show_default(default_buff);
    
    *input = tty_get_arg(io_buff, io_buff_size, &st);
    
    if (*input) {
	if (GOOD_STATUS(&st)) {
	    conv_buff = tty_get_kernel_io_val(io_buff, io_table, &io_map);
	    if (!io_map) {
		SET_STATUS(&st, tty_input_out_of_range);
		tty_status_post_echo(io_buff, tty_dont_understand, st);
	    } else {
		/* the usr entered something that didn't blow away
		 * the buffer and passed the io conversion test.  If what the
		 * user entered is the same as what was already in the field,
		 * then return input=false, otherwise return input=true */
		if (show_default) {
		    if ((use_field && (strcmp(io_buff, default_buff) == 0) && 
			 strlen(io_buff) == strlen(default_buff)))
			*input = false; 
		    else
			strcpy(io_buff, conv_buff);	
		} else
		    strcpy(io_buff, conv_buff);	
	    }  
	}
	

    } else {      /* the user didn't enter anything */
	if (show_default) {
	    if (!use_field) {
		/* if the default value wasn't in the field, then return 
		 * true in the input parameter, so the caller will know they
		 * have to write something to the field.  We make it easy by
		 * by placing the converted default in the io_buff. 
		 */
		*input = true;
		conv_buff = tty_get_kernel_io_val(default_buff, io_table,
						  &io_map);
		strcpy(io_buff, conv_buff);
	    }
	}
    }
    EXIT_ON_ERROR(&st);

    return st;
}    

  
/* FUNCTION tty_print_status
 * Check status against local internal codes in the kernel and tty modules before
 * going to the nck error library for the message text
 */
PUBLIC void tty_print_status
#ifndef __STDC__
    ( mmsg, status ) 
   char             *mmsg;     /* the internal error message string */
   error_status_t   status;
#else
  (
   char             *mmsg,     /* the internal error message string */
   error_status_t   status
  )
#endif
{   
    tty_error_buf_t msg;
 
        /* return if nothing to print */
    if ((GOOD_STATUS(&status)) && (strlen(mmsg) == 0))
        return;

    if (GOOD_STATUS(&status)) {             /* print internal message only */
        msg[0] = '\0';                      /* status msg is null */
	printf("?(rgy_edit) %s\n", mmsg);
    }
    else {                  /* print internal message and converted status */
	tty_get_status_text(status, msg, sizeof(msg));
	printf("?(rgy_edit) %s - %s\n", mmsg, msg);
    }
}


/* FUNCTION tty_status_pre_echo
 *
 * Encloses an input argument in double quotes.  Echos the input BEFORE printing
 * the associated error message string and status.  
 */
PUBLIC void tty_status_pre_echo
#ifndef __STDC__
    ( input, msg, status ) 
    char            *input;
    char            *msg;
    error_status_t  status;
#else
  (
    char            *input,
    char            *msg,
    error_status_t  status
  )
#endif
{
    tty_error_buf_t  status_text;

    tty_get_status_text(status, status_text, sizeof(status_text));
    printf("?(rgy_edit) %c%s%c %s", DQ, input, DQ, msg);
    if (strlen(status_text) != 0)
	printf(" - %s", status_text);
    printf("\n");
}


/* FUNCTION tty_status_post_echo
 *
 * Encloses an input argument in double quotes.  Echos the input AFTER printing
 * the associated error message string but BEFORE printing the status.
 */
PUBLIC void tty_status_post_echo
#ifndef __STDC__
    ( input, msg, status ) 
    char            *input;
    char            *msg;
    error_status_t  status;
#else
  (
    char            *input,
    char            *msg,
    error_status_t  status
  )
#endif
{
    tty_error_buf_t  status_text;

    tty_get_status_text(status, status_text, sizeof(status_text));
    printf("?(rgy_edit) %s %c%s%c", msg, DQ, input, DQ);
    if (strlen(status_text) != 0)
	printf(" - %s", status_text);
    printf("\n");
}
