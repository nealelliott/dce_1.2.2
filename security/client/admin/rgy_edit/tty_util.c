/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_util.c,v $
 * Revision 1.1.8.2  1996/03/11  13:27:27  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:19  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:02:26  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  19:50  mdf
 * 	merge
 * 	[1995/12/08  16:30:49  root]
 * 
 * Revision 1.1.4.2  1993/09/29  15:28:21  burati
 * 	Needed more space for domain_str in INVALID_OPT_DOMAIN
 * 	[1993/09/29  15:27:47  burati]
 * 
 * Revision 1.1.2.3  1993/09/15  15:36:34  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.6.1  1994/07/15  14:58:32  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:14:22  mdf]
 * 
 * Revision 1.1.2.2  1992/12/29  12:42:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:15  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:46:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit tty interface utilities 
** 
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)tty_util.c	15 - 11/21/91";
#endif

#include <stdio.h>
#include <un_strng.h>

#include <tty_base.h>
#include <macros.h>
#include <rgymacro.h>

#include <ek_types.h>
#include <ek_error.h>

#include <tty_conf.h>
#include <tty_err.h>
#include <tty_io.h>
#include <tty_util.h>

#include <dce/rpc.h>
#include <dce/dce_error.h>

PRIVATE error_status_t Status_okay = { error_status_ok }; 

PUBLIC void INVALID_ARG
#ifndef __STDC__
    ( arg, stp ) 
    char            *arg;
    error_status_t  *stp;
#else
  (
    char            *arg,
    error_status_t  *stp
  )
#endif
{
    tty_error_buf_t error_buf;

    SET_STATUS(stp, tty_invalid_argument);
    sprintf(error_buf, "%c%s%c", DQ, arg, DQ);
    tty_print_status(error_buf, *stp);
}


PUBLIC void INVALID_OPT
#ifndef __STDC__
    ( opt, stp ) 
    char            *opt;
    error_status_t  *stp;
#else
  (
    char            *opt,
    error_status_t  *stp
  )
#endif
{
    tty_error_buf_t error_buf;

    SET_STATUS(stp, tty_invalid_option);
    sprintf(error_buf, "%c%s%c", DQ, opt, DQ);
    tty_print_status(error_buf, *stp);
}


PUBLIC void OPT_REQUIRES_ARG
#ifndef __STDC__
    ( opt, stp ) 
    char            *opt;
    error_status_t  *stp;
#else
  (
    char            *opt,
    error_status_t  *stp
  )
#endif
{
    tty_error_buf_t error_buf;

    SET_STATUS(stp, tty_opt_requires_arg);
    sprintf(error_buf, "%c%s%c", DQ, opt, DQ);
    tty_print_status(error_buf, *stp);
}


PUBLIC void INCOMPATIBLE_OPTS
#ifndef __STDC__
    ( opt1, opt2, stp) 
    char            *opt1;
    char            *opt2;
    error_status_t  *stp;
#else
  (
    char            *opt1,
    char            *opt2,
    error_status_t  *stp
  )
#endif
{
    tty_error_buf_t error_buf;

    SET_STATUS(stp, tty_incompatible_options);
    sprintf(error_buf, "%c%s%c  %c%s%c", DQ, opt1, DQ, DQ, opt2, DQ);
    tty_print_status(error_buf, *stp);
}


PUBLIC void INVALID_OPT_DOMAIN
#ifndef __STDC__
    ( arg, domain, stp ) 
    char             *arg;
    eif_domain_t     domain;
    error_status_t  *stp;
#else
  (
    char             *arg,
    eif_domain_t     domain,
    error_status_t  *stp
  )
#endif
{

    tty_error_buf_t  error_buf;
    char             domain_str[10];

    SET_STATUS(stp, tty_bad_domain);
    switch (domain) {
    case eif_domain_person:
	strcpy(domain_str, "principal");
	break;
    case eif_domain_group:
	strcpy(domain_str, "group");
	break;       
    case eif_domain_org:
	strcpy(domain_str, "org");
	break;
    case eif_domain_acct:
	strcpy(domain_str, "account");
	break;
    }

    sprintf(error_buf, "%c%s%c - Option not valid in %s domain", DQ, arg, DQ, 
	    domain_str);
    tty_print_status(error_buf, Status_okay);
}   


/* tty_get_status_text
 *
 * Attempt to retrieve message text, for the specified status code, from the
 * message catalogs.  Return the hex value of the code in the message string
 * if catalog lookup failed.
 */

PUBLIC void tty_get_status_text
#ifndef __STDC__
    ( status, buff, buffsize ) 
    error_status_t  status;
    char            *buff;
    long            buffsize;
#else
  (
    error_status_t  status,
    char            *buff,
    long            buffsize
  )
#endif
{
    dce_error_string_t  tmp_buff;
    int                 tmp_stat;

    if (GOOD_STATUS(&status))
 	*buff = '\0';
    else {
        dce_error_inq_text(status, tmp_buff, &tmp_stat);
        if (GOOD_STATUS(&tmp_stat)) {
            strcpy(buff, (char *) tmp_buff);
        } else {
            sprintf(buff, "%X", status);
        }
    } 
}


PUBLIC void tty_make_abbrev_lname
#ifndef __STDC__
    ( pname, gname, oname, abbrev, display_name ) 
    char  *pname;
    char  *gname;
    char  *oname;
    char  *abbrev;
    char  *display_name;   /* OUT - the displayable login name */
#else
  (
    char  *pname,
    char  *gname,
    char  *oname,
    char  *abbrev,
    char  *display_name
  )
#endif
{
    if (strcmp(abbrev, ek_acct_key_person) == 0)
        sprintf(display_name, "%s [%s %s]", pname, gname, oname);
    else if (strcmp(abbrev, ek_acct_key_group) == 0)
        sprintf(display_name, "%s %s [%s]", pname, gname, oname);
    else
        sprintf(display_name, "%s %s %s", pname, gname, oname);
}


/*
 * wipe the memory containing password data clean
 */
PUBLIC  void destroy_passwd (
    sec_passwd_rec_t  *passwd
)
{
    unsigned_char_p_t  ucp;

    if (!passwd)
        return;

    switch (PASSWD_TYPE(passwd)) {
    case sec_passwd_plain:
        for (ucp = PLAIN_PASSWD(passwd); *ucp != '\0'; *ucp = '\0', ucp++);
        break;
    case sec_passwd_des:
        memset(DES_PASSWD(passwd), 0, sizeof(sec_passwd_des_key_t));
        break;
    }
}
