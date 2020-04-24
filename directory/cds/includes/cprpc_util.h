/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cprpc_util.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:08  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:15:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:40  root]
 * 
 * Revision 1.1.4.3  1994/06/09  18:38:46  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:16  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/19  17:06:11  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:37:06  mccann]
 * 
 * Revision 1.1.4.1  1994/03/12  22:02:32  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:12:48  peckham]
 * 
 * Revision 1.1.2.3  1992/12/30  13:28:52  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:46:33  zeliff]
 * 
 * Revision 1.1.2.2  1992/10/15  19:37:31  m_sawyer
 * 	add prototypes for cprpc_cmp_wildfull() and cprpc_string_match()
 * 	[1992/10/15  19:34:40  m_sawyer]
 * 
 * Revision 1.1  1992/01/19  15:16:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CPRPC_UTIL_H
#define _CPRPC_UTIL_H
/*
 * Module: cprpc_util.h
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 *
 * ABSTRACT:
 *
 * CDS Control Program Utilities
 *
 * This module contains prototypes for miscellaneous routines in the
 * control program.
 */
void
mgmt_error(error_status_t );

/*
 *****************************************************************************
 * C P R P C _ E M I T _ S H O W _ H E A D E R
 *
 * Display a header for a show mumble command.
 */
void 
cprpc_emit_show_header
    (
        struct op_blk *,
	unsigned,
        char          *,
        char          *
    );

/*
 *****************************************************************************
 * C P R P C _ R E P O R T _ E R R O R
 *
 * This routine deciphers and reports RPC and CDS errors.
 * It also logs them to syslog, hwt, 16-Aug-1991
 */
void 
cprpc_report_error
    (
        char           *,
        error_status_t 
    );

/*
 *****************************************************************************
 * C P R P C _ G E T _ B I N D I N G _ H A N D L E
 *
 * This routine gets an RPC binding handle.
 */
handle_t 
cprpc_get_binding_handle
    (
        int            ,
        cds_FullName_t *,
        error_status_t *
    );

/*
 *****************************************************************************
 * C P R P C _ N A M E S _ P O I N T L A S T
 *
 * This routine returns a pointer to the last simplename of a given
 * fullname.
 */
SimpleName_t * 
cprpc_names_pointlast
    (
        FullName_t *
    );

/*
 *****************************************************************************
 * C P R P C _ S T R I N G _ M A T C H
 *
 *  This routine compares two text strings and returns boolean TRUE
 *  if the two strings match. The second string (patterh) may be
 *  contain wildcard characters.
 *
 *      ? - matches any single character
 *      * - matches any string of zero or more characters
 *      \ - causes both '?' and '*' to be treated literally
 */
int 
cprpc_string_match
    (
        unsigned char *,
        unsigned char *
    );

/*
 *****************************************************************************
 * C P R P C _ C M P _ W I L D S I M P L E
 *
 * This routine performs a lexicographical comparison of two
 * simplenames much like strcmp() performs a lexicographical
 * comparison of two strings.
 *
 * It returns BOOLEAN TRUE if the two strings match and the second
 * simplename may contain wildcards.
 */
int 
cprpc_cmp_wildsimple
    (
        SimpleName_t *,
        SimpleName_t *
    );

/*
 *****************************************************************************
 * C P R P C _ C M P _ W I L D F U L L
 *
 * This routine performs a lexicographical comparison of two
 * fullnames much like strcmp() performs a lexicographical
 * comparison of two strings.
 *
 * It returns BOOLEAN TRUE if the two strings match and the second
 * fullname may contain wildcards.
 */
int 
cprpc_cmp_wildfull
    (
        FullName_t *,
        FullName_t *
    );
#endif  /* #ifndef _CPRPC_UTIL_H */
