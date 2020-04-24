/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dns_cfg.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:16  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:15:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:48  root]
 * 
 * Revision 1.1.4.5  1994/09/06  17:37:02  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:29:36  proulx]
 * 
 * 	fix timestamps
 * 
 * 	fix timestamps
 * 
 * Revision 1.1.4.4  1994/08/03  19:02:12  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:56:03  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:38:51  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:24  devsrc]
 * 
 * Revision 1.1.4.2  1994/05/12  21:11:46  peckham
 * 	Eliminate unnecessary conditionals.
 * 	[1994/05/12  19:16:15  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:02:41  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:13:24  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:29:15  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:47:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _DNS_CFG_H
#define _DNS_CFG_H
/*
 * Module dns_cfg.h
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
 * Module Description
 *
 * These routines allow access to a configuration file
 *
 */
#include <dce/utc.h>
#include <dce/dns_record.h>


void
dns_cfginit (
    char		*,
    char		*);

char *
dns_cfgopen (
    char		*,
    char		*);

int
dns_cfgwrite (
    char		*);

void
dns_cfgclose (
    char		*);

char *
dns_cfgtostr (
    char		*,
    char		*,
    char		*);

int
dns_cfgfromstr (
    char		*,
    char		*,
    char		*);

long
dns_cfgtol (
    char		*,
    char		*,
    long		);

int
dns_cfgfroml (
    char		*,
    char		*,
    long		);

int
dns_cfgfromarg (
    char		*,
    char		*);

void
dns_coredefault(	
    char                *,
    int                  );

int 
dns_cfgtoarg 
    (char  		*,
     char  		*,
     char  		**,
     int   		);

void 
dns_daemon (
     char 		*);

int
dns_CvtCDSFullToOpq (
		     unsigned char    *,
                     dns_full_name_t  *,
                     int              *,
                     char             *);

int
dns_CvtCDSFullToGDS (
		     unsigned char    *,
                     int              ,
                     int              ,
                     dns_full_name_t  *,
                     int              *);

int
dns_CvtCDSFullToStr (
		     dns_full_name_t  *,
                     unsigned char    *,
                     int              *,
                     char             *);

int 
dnsCvtStrToTs (
	       byte_t   *,
               dns_cts  *,
               byte_t   **);

void 
dnsCvtTsToAsc (
	       dns_cts  *,
               byte_t   *);

/*
 * From validate_tower.c
 */
int
validate_tower (
    char 	*,
    int     	 );

#endif  /* #ifndef _DNS_CFG_H */
