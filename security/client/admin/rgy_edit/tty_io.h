/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_io.h,v $
 * Revision 1.1.4.2  1996/03/11  13:27:18  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:11  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:01:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:38  root]
 * 
 * Revision 1.1.2.2  1992/12/29  12:41:59  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:02  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:45:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  tty_io.h V=8 05/21/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit tty interface utilities - public include file
**
** 
*/


#ifndef tty_io__h_included
#define tty_io__h_included

#include <dce/nbase.h>

#include <ek_db.h> 

#define DQ '"'    /* symbolic definition of a double quote character. 
		   * we use it to quote output, as in printf("%c%s%c", DQ, 
		   * string, DQ) 
		   */

typedef enum {tty_verify_no,
	      tty_verify_yes,
	      tty_verify_go,
	      tty_verify_quit
} tty_verify_response_t;


typedef enum {tty_response_no,
	      tty_response_yes,
	      tty_response_none
} tty_y_n_response_t;


/*  conversion table structure for converting kernel representations to tty 
 * interface representations.   For instance, the only acceptable values for 
 * kernel flags are defined by the kernel constants ek_flag_on and ek_flag_off.
 * These  representations probably don't make sense to a user interacting with
 * the  tty interface, so we define the conversion table structure (tables are 
 * defined and initialized at compile time by the specific interface module) to
 * allow easy conversion to/from tty format.
 */
typedef struct tty_io_map_t {
    char     *tty_val;
    char     *kernel_val;
} tty_io_map_t;
    
typedef struct tty_io_conversion_table_t {
    tty_io_map_t  *table;
    long          num_entries;     /* number of entries in the io map. */
    boolean32     one_to_one;     /* is there a one-to-one mapping between tty 
				     and kernel values? */
} tty_io_conversion_table_t;


/* tty_get_kernel_io_val
 *
 * Looks up an interface i/o string in a conversion table.  If the tty value
 * exists, the  corresponding kernel value from the table is returned and the
 * match parameter returns true.  If the tty value does not exist, the result
 * depends on  whether there is a one-to-one mapping between kernel and tty 
 * values. If the mapping is NOT one-to-one, the function simply returns a
 * duplicate of tty_key parameter and the match parameter returns true.   If 
 * the mapping IS  one-to-one the match parameter returns false. 
 * 
 * The return value is a pointer to static storage that is overwritten by each 
 * call.
 */
char  *tty_get_kernel_io_val (
#ifdef __STDC__
    char                       *tty_key,
    tty_io_conversion_table_t  *table,
    boolean32                  *match
#endif
);

/* tty_get_tty_io_val
 *
 * Like eif_get_kernel_io_val, but in the other direction
*/

char  *tty_get_if_io_val (
#ifdef __STDC__
    char                       *kernel_key,
    tty_io_conversion_table_t  *table,
    boolean32                  *match
#endif
);
 
/* a null conversion table for i/o strings that can pass between the tty
 *interface and the kernel "as is"
 */
extern tty_io_conversion_table_t tty_null_conv_table;

extern tty_io_conversion_table_t tty_uid_conv_table;

extern tty_io_conversion_table_t tty_flag_conv_table;

/* this conversion table is for those funny flag situations where  a "yes"
 * response turns the flag off
 */
extern tty_io_conversion_table_t tty_flag_yes_off_conv_table;

extern tty_io_conversion_table_t tty_lifespan_conv_table;

extern tty_io_conversion_table_t tty_date_conv_table;

extern tty_io_conversion_table_t tty_abbrev_conv_table;
 
/* 
** tty io routines 
*/

/* tty_show_prompt
 *
 * if standard output is a tty, dump "prompt" to it.
 */
void tty_show_prompt (
#ifdef __STDC__
    char  *prompt
#endif
);

/* tty_end_prompt
 *
 * if standard output is a tty, dump "prompt" to it followed by a newline.
*/
void tty_end_prompt (
#ifdef __STDC__
    char  *prompt
#endif
);
 
void tty_show_default (
#ifdef __STDC__
    char  *default_str
#endif
);


/* tty_verify
 *
 * if the input is a tty, echoes the prompt string and asks the user for a 
 * [y/n/q/g] reponse.  If input is not from a tty, just returns tty_verify_go
 * (this will allow calling routines that are iterating to behave as though
 * the user had entered "g" when input is not a tty).
 * 
 * returns:
 *   tty_verify_no,
 *   tty_verify_yes,
 *   tty_verify_go,
 *   tty_verify_quit 
 */
tty_verify_response_t tty_verify (
#ifdef __STDC__
    char  *prompt_string
#endif
);


tty_y_n_response_t tty_ask_y_n (
#ifdef __STDC__
    char                *prompt_string,
    tty_y_n_response_t  def_response
#endif
);



/* tty_get_string
 *
 * just reads characters from standard input into a buffer, ignoring any that
 * would exceed the buffer size.  The buffer always returns null terminated, so 
 * pass in something at least 1 character longer than the maximum sensible
 * response you're willing to accept.  Returns false if the input is an 
 * immediate newline or EOF,  true otherwise.    If input data exceeds the
 * input buffer size, the overflow parameter returns true, otherwise it returns
 * false.
 */
boolean32 tty_get_string (
#ifdef __STDC__
    char     *buff,
    long     buffsize,
    boolean32  *overflow
#endif
);

 
/* tty_get_arg
 * 
 * Like tty_get_string, but can deal with quoted strings.  returns false on
 * on any unquoted whitespace string, true otherwise.  
 */

boolean32 tty_get_arg (
#ifdef __STDC__
    char            *buff,
    long            buffsize,
    error_status_t  *stp
#endif
);
  
     
error_status_t tty_get_field_arg ( 
#ifdef __STDC__
     ek_op_handle_t             op_handle,
     ek_field_id_t              field,
     tty_io_conversion_table_t  *io_table,
     boolean32                  show_default,
     char                       *default_str,
     char                       *io_buff,
     long                       io_buff_size,
     char                       *prompt,
     boolean32                  *input
#endif
);  

void tty_print_status (
#ifdef __STDC__
   char            *mmsg,     /* error message string */
   error_status_t  status
#endif
);


void tty_status_pre_echo (
#ifdef __STDC__
    char            *input,
    char            *msg,
    error_status_t  status
#endif
);

void tty_status_post_echo (
#ifdef __STDC__
    char            *input,
    char            *msg,
    error_status_t  status
#endif
);


#endif /*tty_io_included */
