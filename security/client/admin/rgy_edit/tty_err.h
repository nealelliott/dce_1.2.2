/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_err.h,v $
 * Revision 1.1.4.2  1996/03/11  13:27:16  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:01:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:35  root]
 * 
 * Revision 1.1.2.2  1992/12/29  12:41:46  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:12:58  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:46:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  tty_err.h V=10 11/22/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit tty interface utilities - public include file
** 
*/

#ifndef tty_error__h_included
#define tty_error__h_included

#include <dce/nbase.h>
  
#include <tty_util.h>
#include <un_io.h>

/* error message buffer type for passing error text around between internal
 * routines
 */
#define TTY_MAX_ERROR_LEN 256
typedef char tty_error_buf_t[TTY_MAX_ERROR_LEN];
                   
extern boolean32 tty_exit_on_error;   /* if true causes an exit whenever an error 
                                     * occurs.  initially false. 
				     */

extern error_status_t tty_exit_status;  /* records error status for exit */
                                     

#define SET_EXIT_STATUS(stp)  { \
            if ((*stp) != error_status_ok) \
                tty_exit_status = (*stp); \
	    }
/* 
** exit on bad status if tty_exit_on_error is true
*/
#define EXIT_ON_ERROR(stp) { \
            if ( (*stp) != error_status_ok) { \
                if (tty_exit_on_error) { \
                    fprintf(stderr, "FAULT at line %d of file %s", \
                            __LINE__, __FILE__); \
                    exit(*stp); \
                } \
	    } \
	}

#define SET_AND_EXIT_ON_ERROR(stp) { \
            if ( (*stp) != error_status_ok) { \
                if (tty_exit_on_error) { \
                    fprintf(stderr, "FAULT at line %d of file %s", \
                            __LINE__, __FILE__); \
                    exit(*stp); \
                } else { \
                    tty_exit_status = (*stp); \
                } \
            } \
        }

#define RETURN_OR_EXIT_ON_ERROR(stp) { \
            if ( (*stp) != error_status_ok) { \
                if (tty_exit_on_error) { \
                    fprintf(stderr, "FAULT at line %d of file %s", \
                            __LINE__, __FILE__); \
                    exit (*stp); \
                } else { \
                    return *(stp); \
                } \
           } \
        }
 
#define CONTINUE_OR_EXIT_ON_ERROR(stp) { \
            if ( (*stp) != error_status_ok) { \
                if (tty_exit_on_error) { \
                    fprintf(stderr, "FAULT at line %d of file %s", \
                            __LINE__, __FILE__); \
                    exit(*stp); \
                } else { \
                    continue; \
                } \
           } \
        }

#define DATA_FORMAT_ERROR(data, stp) { \
        *stp = tty_bad_data_format; \
        tty_status_pre_echo((data), "", (*stp)); \
        tty_exit_status = (*stp); \
    }
           
#define TTY_FATAL(preamble, stp) { \
            tty_print_status((preamble), (*stp)); \
            exit(*stp); \
	}

/* tty_error_get_text
 *
 * returns true and passes back the status text if the status is a tty module
 * status
 * code, false otherwise.
 */
boolean32 tty_error_get_text (
#ifdef __STDC__
    error_status_t  st,
    char            *buff,
    long            buff_size
#endif
);  

#endif /*tty_error_included */
