/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dfsdlg_test.h,v $
 * Revision 1.1.8.1  1996/10/17  18:18:51  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:50:17  damon]
 *
 * Revision 1.1.3.2  1994/08/10  19:23:04  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:20  annie]
 * 
 * Revision 1.1.3.1  1994/07/13  22:33:03  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:17:43  devsrc]
 * 
 * 	Delegation Functional Tests - Initial Submit
 * 	[1994/06/08  18:30:23  delgado]
 * 
 * $EndLog$
 */

/*  This file not in bl-5 
#include <dce/macros.h>
*/

#define TARGET_PRINCIPAL_NAME "dfsdlg_target"
#define TARGET_NSENTRY_NAME "/.:/dfsdlg_target"
#define INTEMEDIARY_NSENTRY_NAME "/.:/dfsdlg_int"
#define INTEMEDIARY_PRINCIPAL_NAME "dfsdlg_int"
#define CLIENT_PRINCIPAL_NAME "dfsdlg_client"

/* When we move beyond bl-5 we can just get this stuff in macros.h */

#define STATUS_OK(stp)            ((stp)==NULL || (*stp) == error_status_ok)
#define GOOD_STATUS(stp)          ((stp)==NULL || (*stp) == error_status_ok)
#define BAD_STATUS(stp)           ((*stp) != error_status_ok)
#define SET_STATUS(stp, val)      (*stp) = val
#define CLEAR_STATUS(stp)         (*stp) = error_status_ok
#define STATUS_EQUAL(stp, st_val) ((*stp) == (st_val))

#define EXIT_ON_ERROR(st) \
    if (BAD_STATUS(&st)) { \
        exit(st); \
    }



void log_in(sec_login_handle_t *lcp,
                           unsigned_char_p_t user,
                           unsigned_char_p_t pbuf,
                           error_status_t *stp);

void bind_to_svr(handle_t *h,
                  unsigned_char_p_t import_context,
                  error_status_t *stp);






