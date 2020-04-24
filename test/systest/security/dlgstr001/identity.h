/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: identity.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:15  marty]
 *
 * Revision 1.1.4.1  1995/12/11  22:45:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:21:39  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:42:54  agd
 * 	expand copyright
 * 	[1994/10/06  14:31:47  agd]
 * 
 * Revision 1.1.2.1  1994/09/19  17:55:13  bhaim
 * 	multidelegate test
 * 	[1994/09/19  17:49:29  bhaim]
 * 
 * $EndLog$
 */
/******************************************************************************
 *		SCCSID(@(#)identity.h	1.2 LCC) Modified: 02/01/94 17:05:04
 *
 *		(C) Copyright 1993, 1994 Locus Computing Corporation
 *			All Rights Reserved
 *
 *  		Header file for establish server's identity
 *
 *****************************************************************************/

#ifndef IDENTITY_H
#define IDENTITY_H


#include <pthread.h>
#include <dce/sec_login.h>
#ifdef TRACING
#include <dce/trace_log.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
**********************************************************************
**
** D E F I N E S
**
**********************************************************************
*/
#if defined(__STDC__) || defined (__cplusplus)
#  ifndef PROTOTYPES
#  define PROTOTYPES
#  endif
#endif

#ifndef MIN
#define MIN(a, b) (((a)>(b)) ? (b) : (a))
#endif

#ifdef FAIL_REFRESH
#define REFRESH_RETRY_SNOOZE_TIME 5
#else
#define REFRESH_RETRY_SNOOZE_TIME 300
#endif
#define REFRESH_MAX_RETRIES 5

/*
**********************************************************************
**
**  T Y P E D E F S
**
**********************************************************************
*/

typedef struct identity_info_t {
    sec_login_handle_t login_context;
    unsigned_char_t    *principal;
    unsigned_char_t    *keytab_file;
    pthread_t          refresh_thread;  /* thread for identity refresh */
    pthread_t          keymgmt_thread;  /* thread for key management */
} identity_info_t, *identity_info_p_t;



/*
**********************************************************************
**
**  G L O B A L S
**
**********************************************************************
*/

extern  identity_info_t       *iden_info;

/*
**********************************************************************
**
**  F O R W A R D    D E C L A R A T I O N S
**
**********************************************************************
*/
#ifdef PROTOTYPES
extern void establish_identity
    (
	/* [in]  */ unsigned32         authn_svc,
	/* [in]  */ unsigned_char_t    *principal_name,
	/* [in]  */ unsigned_char_t    *keytab_file_name,
	/* [out] */ identity_info_t    **identity_info
    );
extern void print_dce_error
    (
	error_status_t status,
	char *msg
    );
#ifdef TRACING
extern void tr_print_dce_error
    (
	tr_handle_t    *tr_h,
	error_status_t status,
	char *msg
    );
#endif
#else
extern void establish_identity();
static void print_dce_error();
extern void tr_print_dce_error();
#endif

#ifdef __cplusplus
}
#endif

/* #endif IDENTITY_H */
/* don't put anything after the #endif */
#endif
