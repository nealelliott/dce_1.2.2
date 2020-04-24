/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_login_file_loc.h,v $
 * Revision 1.1.13.1  1996/10/03  14:44:57  arvind
 * 	Suppress CPP warning for duplicate define.
 * 	[1996/09/16  20:05 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.11.2  1996/03/11  13:27:52  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:40  marty]
 * 
 * Revision 1.1.11.1  1995/12/08  18:04:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:31:26  root]
 * 
 * Revision 1.1.8.5  1994/08/15  20:15:01  bowe
 * 	New macro, SEC_LOGIN_CCACHE_FROM_PAG_FMT_LOCAL.  Reworked a bit
 * 	so you can get relative paths to .../var/security/...  [CR 11656]
 * 	[1994/08/15  20:14:23  bowe]
 * 
 * Revision 1.1.8.4  1994/06/02  21:57:27  mdf
 * 	Merged with changes from 1.1.8.3
 * 	[1994/06/02  21:55:28  mdf]
 * 
 * 	force ANSI dependence so RIOS will build!
 * 	[1994/06/01  18:02:53  mdf]
 * 
 * Revision 1.1.8.3  1994/06/02  20:20:56  mdf
 * 	force ANSI dependence so RIOS will build!
 * 	[1994/06/01  18:02:53  mdf]
 * 
 * Revision 1.1.8.1  1994/05/11  16:49:36  ahop
 * 	move PWD_STRENGTHD_LOG here from pwd_strength.c
 * 	[1994/05/06  20:52:05  ahop]
 * 
 * 	add non-ANSI support for new CCACHE #defines
 * 	[1994/05/05  22:11:04  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  20:45:24  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 
 * 	hp_sec_to_osf_2 drop
 * 
 * Revision 1.1.4.2  1993/12/06  23:20:22  rps
 * 	*** empty log message ***
 * 	[1993/12/04  01:20:31  rps]
 * 
 * 	Added SEC_LOGIN_CCACHE_TKT_LEAF for kdestroy (ANSI only; post-1.0.3)
 * 	[1993/12/04  00:02:19  rps]
 * 
 * Revision 1.1.2.3  1993/09/15  15:37:02  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.5.4  1993/02/01  20:35:59  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  16:14:20  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:11:48  hinman]
 * 
 * Revision 9.5.1.4  93/01/06  10:05:43  meg
 * 	[meg] incorporated osf rev 1.1.5.3
 * 
 * Revision 9.5.4.2  93/01/06  10:04:12  meg
 * 	[meg] incorporated osf rev 1.1.5.3
 * 
 * Revision 9.13.1.1  92/12/30  09:56:47  root
 * 	Branch holding OSF rev
 * 	Revision 9.5.1.3  92/10/01  16:16:26  chen
 * 	fixed definitions that would have broken on systems with
 * 	non-ANSI C preprocessors -- rcc
 * 
 * Revision 9.13  92/12/30  09:46:15  root
 * 	Update checkin
 * 	Revision 9.5.3.2  92/10/01  16:07:24  chen
 * 	fixed definitions that would have broken on systems with
 * 	non-ANSI C preprocessors -- rcc
 * 
 * Revision 1.1.5.3  1992/12/29  12:43:44  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:39:37  zeliff]
 * 
 * Revision 9.12.1.2  92/09/30  10:05:16  hinman
 * 	Auto checkin of OSF rev 1.1.5.2
 * 
 * Revision 1.1.5.2  1992/09/29  21:15:30  devsrc
 * 	[OT 5373]  SNI/SVR4 merge UPDATE: fixed macros to call STRING_CONCAT with the
 * 	           correct number of arguments.
 * 	[1992/09/23  21:08:02  sekhar]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:19:26  sekhar]
 * 
 * Revision 1.1.2.2  1992/06/03  19:00:01  ahop
 * 	 2143: make SEC_LOGIN_PAG_NAME_LENGTH equal to MAXPATHLEN
 * 	[1992/06/03  00:12:05  ahop]
 * 
 * 	 2143: put '/' in front of dcecred
 * 	[1992/05/28  19:53:36  ahop]
 * 
 * 	 CR2143: fix DCE_LOCAL_PATH typo
 * 	[1992/05/27  14:30:31  ahop]
 * 
 * 	 Fix 2143: define global strings for credential cache file locations
 * 	[1992/05/22  19:39:47  ahop]
 * 
 * $EndLog$
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

#ifndef sec_login_file_loc_h__included
#define sec_login_file_loc_h__included

#if !defined(EXTERN)		/* also defined in rca_pvt.h... */
#ifdef _sec_login_ALLOCATE_GLOBALS_
#   define EXTERN
#   define INITVAL(X) X
#else
#   define EXTERN extern
#   define INITVAL(X)
#endif
#endif

#include <initstring.h>
#include <sys/param.h>

/*
** Global Definitions
*/

/*
 * format that auth. helper can use to convert a numeric PAG into
 * something suitable for use with import_context.
 *
 * both of these two defines are necessary, and must be kept in sync.
 */

#ifdef SNI_SVR4
	/* SVR4 silently truncates filenames longer than 14 chars on s5
	 * filesystems so CREDFNAME has to be shortened to ensure that
	 * the names of all files containing credentials stay < 13 chars
	 * long (1 for safety so you can see if something screws up).
	 * the #define's for CREDFNAME, SEC_LOGIN_ENV_NAME_LENGTH, and
	 * DATAFILE_SUFFIX all have to kept in sync with each other -- rcc
	 */
#define CREDLNAME	"dc_"
#define CREDFNAME	"/dc_"
#else
#define CREDLNAME	"dcecred_"
#define CREDFNAME	"/dcecred_"
#endif

#define SEC_LOGIN_CCACHE_PREFIX		"FILE:"
#define SEC_LOGIN_CCACHE_TKT_LEAF		CREDLNAME "%08x"
#define SEC_LOGIN_CREDS_REL_DIR              "/var/security/creds"
#define SEC_LOGIN_CREDS_DIR                  DCELOCAL_PATH SEC_LOGIN_CREDS_REL_DIR
#define SEC_LOGIN_CCACHE_FROM_PAG_FMT        SEC_LOGIN_CCACHE_PREFIX SEC_LOGIN_CCACHE_FILE_FROM_PAG_FMT
#define SEC_LOGIN_CCACHE_FILE_FROM_PAG_FMT   SEC_LOGIN_CREDS_DIR CREDFNAME "%08x"
#define SEC_LOGIN_CCACHE_TKT_ROOT            SEC_LOGIN_CREDS_DIR CREDFNAME
#define PWD_STRENGTHD_REL_LOG                "/var/security/pwd_strengthd.log"
#define PWD_STRENGTHD_LOG                    DCELOCAL_PATH PWD_STRENGTHD_REL_LOG

/* This has a "%s" in place of DCELOCAL_PATH, since in 1.1 programs should
 * use the global variable dcelocal_path.  Use by:
 * sprintf(buf, SEC_LOGIN_CCACHE_FROM_PAG_FMT_LOCAL, dcelocal_path, NNN);
 */
#define SEC_LOGIN_CCACHE_FROM_PAG_FMT_LOCAL  \
  SEC_LOGIN_CCACHE_PREFIX "%s" SEC_LOGIN_CREDS_REL_DIR CREDFNAME "%08x"
  
#define SEC_LOGIN_TKT_NAME_MAX_LENGTH   MAXPATHLEN
#define SEC_LOGIN_PAG_NAME_LENGTH       MAXPATHLEN
#define SEC_LOGIN_NO_PAG                -1

                            
#endif /* sec_login_file_loc_h__included */
