/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndsreg.c,v $
 * Revision 1.1.4.2  1996/02/18  18:20:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:05:38  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	[1995/08/02  17:17 UTC  dtruong  /main/HPDCE02/dtruong_mothra1.1d/1]
 * 	     Remove gnu_regex.c from FSF with HP's regex.c
 * 	HP revision /main/HPDCE02/1  1994/11/17  23:22 UTC  rmm
 * 	merge David Truongs build fixes back to here.
 * 
 * 	HP revision /main/dtruong_mothra1.1b/1  1994/10/10  21:07 UTC  dtruong
 * 	Added hpux1000p for __regcomp and __regexec
 * 	[1995/12/08  15:32:25  root]
 * 
 * Revision 1.1.2.4  1994/09/06  12:25:46  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:17:04  keutel]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:43  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:48  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:06  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:30:16  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:42:13  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:25:37  keutel
 * 	creation
 * 	[1994/03/21  15:53:37  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndsreg.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:20:16 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    module                                                         */
/*                                                                           */
/*   NAME:    ndsreg.c                                                       */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains functions necessary to compile a regular       */
/*       expression (nds_regcomp()) resp. to match a string against a        */
/*       compiled regular expression (nds_regex()). Because compilation      */
/*       of the regular expression is done offline, these functions are      */
/*       highly machine dependent and must be ported to a new platform       */
/*       if required.                                                        */
/*                                                                           */
/*       The following functions are available:                              */
/*                                                                           */
/*          * nds_regcomp   - compile (extented) regular expression          */
/*          * nds_regex     - match string against compiled regular          */
/*                            expression                                     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 9.8.93      */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#if __hpux1000p
#   include <hp_regex.h>
#else
#   include <regex.h>
#endif

/* ******************* DECLARATION OF GLOBAL DATA ************************** */

extern signed32	svcprolog ;

/* ------------------------------------------------------------------------- */

signed32 nds_regcomp (char	*pattern,	/* > regular expression RE */
		      byte	**compexpr,	/* < compiled RE info */
		      signed32	*exprlng) {	/* < size of compiled RE info */

    int		res, regsize ;
    regex_t	preg ;
    static byte	rebuf[1024] ;
    char	errbuf[256] ;


    /* make sure pattern corresponds to a complete line */
    sprintf ((char *) rebuf, "^%s$", pattern) ;

    /* compile (extended) regular expression (RE) */
    if ((res = regcomp (&preg, (char *) rebuf, REG_EXTENDED|REG_NOSUB)) != 0) {
	(void) regerror (res, &preg, errbuf, sizeof (errbuf)) ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_CMX,
	    svc_c_sev_warning | svcprolog, GDS_S_NDS_REGCOMP_ERR, errbuf, res) ;
    } else {
	/* save descriptive information and compiled RE into a static buffer */
	if (sizeof (regex_t) + (regsize = (signed32) (preg.__c_re_end -
						preg.__c_re)) > sizeof (rebuf))
	    return (-1) ;
	*((regex_t *) rebuf) = preg ;
	((regex_t *) rebuf)->__c_re = (unsigned char *) NULL ;
	((regex_t *) rebuf)->__c_re_end = (unsigned char *) regsize ;
	((regex_t *) rebuf)->__c_buf_end =
			    (unsigned char *) (preg.__c_buf_end - preg.__c_re) ;
	(void) memcpy ((void *) ((regex_t *) rebuf + 1), (void *) preg.__c_re,
							     (size_t) regsize) ;
	/* release memory allocated by regcomp */
	regfree (&preg) ;

	/* supply result parameter */
	*compexpr = rebuf ;
	*exprlng = sizeof (regex_t) + regsize ;
    }
    return ((signed32) res) ;
}

/* -------------------------------------------------------------------------- */

signed32 nds_regex (byte	*compexpr,	/* > compiled RE info */
		    char	*string) {	/* > string */

    int	 res ;
    char errbuf[256] ;

    /* update descriptive information */
    if (((regex_t *) compexpr)->__c_re == (unsigned char *) NULL) {
	((regex_t *) compexpr)->__c_re += (int) ((regex_t *) compexpr + 1) ;
	((regex_t *) compexpr)->__c_re_end += (int) ((regex_t *) compexpr)->__c_re ;
	((regex_t *) compexpr)->__c_buf_end += (int) ((regex_t *) compexpr)->__c_re ;
    }

    /* compare string against compiled RE */
    if ((res = regexec ((regex_t *) compexpr, string, (size_t) 0, NULL, 0)) != 0 ) {
	(void) regerror (res, (regex_t *) compexpr, errbuf, sizeof (errbuf)) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_CMX,
			  svc_c_debug6, GDS_S_NDS_REGEX_NOMATCH, errbuf, res)) ;
    }
    return ((signed32) res) ;
}
