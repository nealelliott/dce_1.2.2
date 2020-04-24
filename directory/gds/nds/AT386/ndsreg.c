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
 * Revision 1.1.4.2  1996/02/18  18:20:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:48  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:04:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:59  root]
 * 
 * Revision 1.1.2.5  1994/09/06  12:25:44  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:16:59  keutel]
 * 
 * Revision 1.1.2.4  1994/07/06  15:07:49  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:17:41  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:35  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:42  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:01  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:30:04  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:41:57  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:19:16  keutel
 * 	creation
 * 	[1994/03/21  15:53:22  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ndsreg.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:20:06 $";
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
/*       of the regular expression is done offline (by the NDS-compiler),    */
/*       whereas the comparison is done at runtime by the NDS-library,      */
/*       there must be some additional code around the ordinary regular      */
/*       expression functions 'regcomp()' and 'regexec ()'.                  */
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
#include <ndscfg.h>
#include <gnu_regex.h>

/* ******************* DECLARATION OF GLOBAL DATA ************************** */

extern signed32	svcprolog ;

/* ------------------------------------------------------------------------- */

signed32 nds_regcomp (char	*pattern,	/* > regular expression RE */
		      byte	**compexpr,	/* < compiled RE info */
		      signed32	*exprlng) {	/* < size of compiled RE info */

    int		 res ;
    regex_t	 preg ;
    static byte rebuf[1024] ;
    char	errbuf[256] ;


    /* make sure pattern corresponds to a complete line */
    sprintf ((char *) rebuf, "^%s$", pattern) ;

    /* compile (extended) regular expression (RE) */
    if ((res = __regcomp (&preg, (char *) rebuf, REG_EXTENDED|REG_NOSUB)) != 0) {
	(void) __regerror (res, &preg, errbuf, sizeof (errbuf)) ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_CMX,
	    svc_c_sev_warning | svcprolog, GDS_S_NDS_REGCOMP_ERR, errbuf, res) ;
    } else {
	/* save descriptive information and compiled RE into a static buffer */
	if (sizeof (regex_t) + preg.used > sizeof (rebuf))
	    return (-1) ;
	*((regex_t *) rebuf) = preg ;
	((regex_t *) rebuf)->buffer = (unsigned char *) NULL ;
	((regex_t *) rebuf)->allocated = ((regex_t *) rebuf)->used ;
	(void) memcpy ((void *) ((regex_t *) rebuf + 1), (void *) preg.buffer,
							   (size_t) preg.used) ;
	/* release memory allocated by regcomp */
	__regfree (&preg) ;

	/* supply result parameter */
	*compexpr = rebuf ;
	*exprlng = sizeof (regex_t) + ((regex_t *) rebuf)->used ;
    }
    return ((signed32) res) ;
}

/* -------------------------------------------------------------------------- */

signed32 nds_regex (byte	*compexpr,	/* > compiled RE info */
		    char	*string) {	/* > matching string */

    int	 res ;
    char errbuf[256] ;

    /* update descriptive information */
    if (((regex_t *) compexpr)->buffer == (byte *) NULL)
	((regex_t *) compexpr)->buffer = (byte *) ((regex_t *) compexpr + 1) ;

    /* compare string against compiled RE */
    if ((res = __regexec ((regex_t *) compexpr, string, (size_t) 0, NULL,
								    0)) != 0 ) {
	(void) __regerror (res, (regex_t *) compexpr, errbuf, sizeof (errbuf)) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s%d"), GDS_S_CMX,
			  svc_c_debug6, GDS_S_NDS_REGEX_NOMATCH, errbuf, res)) ;
    }
    return ((signed32) res) ;
}
