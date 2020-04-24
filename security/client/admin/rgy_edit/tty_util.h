/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_util.h,v $
 * Revision 1.1.6.2  1996/03/11  13:27:28  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:19  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:02:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:50  root]
 * 
 * Revision 1.1.4.1  1994/07/15  14:58:34  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:14:25  mdf]
 * 
 * Revision 1.1.2.2  1992/12/29  12:42:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:18  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:46:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  tty_util.h V=7 11/22/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit tty interface utilities - public include file
** 
*/


#ifndef tty_util__h_included
#define tty_util__h_included

#include <dce/nbase.h>

#include <eif.h>

void tty_get_status_text ( 
#ifdef __STDC__
    error_status_t  status,
    char       *buff,
    long       buffsize
#endif
);


/* tty_make_abbrev_lname
 *
 * takes a login name and an abbreviation string, and encloses the optional
 * part of the login name in square brackets.  Assumes the login name is
 * properly formed "name.name.name"
 *
 * E.G.
 *    login name =             "greg dds r_d"
 *    abbreviation =           ekn_acct_key_person
 *    displayable login name = "greg [dds r_d]"
 */
void tty_make_abbrev_lname (
#ifdef __STDC__
    char  *pname, 
    char  *gname,
    char  *oname,
    char  *abbrev,
    char  *display_name
#endif
);

 
/* 
** routines for reporting standard kinds of command line errors
*/
void INVALID_ARG (
#ifdef __STDC__
    char *arg,
    error_status_t  *stp   /* [out] - set to the appropriate status */
#endif
);

void INVALID_OPT (
#ifdef __STDC__
    char *opt,
    error_status_t  *stp   /* [out] - set to the appropriate status */
#endif
);

void OPT_REQUIRES_ARG (
#ifdef __STDC__
    char *opt,
    error_status_t  *stp   /* [out] - set to the appropriate status */
#endif
);

void INCOMPATIBLE_OPTS (
#ifdef __STDC__
    char *opt1,
    char *opt2,
    error_status_t  *stp   /* [out] - set to the appropriate status */
#endif
);

void INVALID_OPT_DOMAIN (
#ifdef __STDC__
    char           *arg,
    eif_domain_t  domain,
    error_status_t  *stp   /* [out] - set to the appropriate status */
#endif
); 

void INVALID_OPT_DOMAIN (
#ifdef __STDC__
    char           *arg,
    eif_domain_t  domain,
    error_status_t  *stp   /* [out] - set to the appropriate status */
#endif
);

void destroy_passwd (
    sec_passwd_rec_t *passwd
);


#endif /*tty_util_included */
