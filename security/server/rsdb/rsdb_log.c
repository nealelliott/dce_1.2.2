/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_log.c,v $
 * Revision 1.1.9.2  1996/02/18  00:21:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:24  marty]
 *
 * Revision 1.1.9.1  1995/12/08  17:59:46  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/09  10:44 UTC  sekhar
 * 	Merge fixes to mothra.
 * 
 * 	HP revision /main/sekhar_mothra_fix/1  1995/06/01  12:32 UTC  sekhar
 * 	[ CHFts13106 ] Replaced SVC code sec_sys_errno_text with sec_sys_errno_file_text.
 * 	[1995/12/08  17:22:22  root]
 * 
 * Revision 1.1.7.2  1994/08/25  21:36:15  sekhar
 * 	[ OT 11759 ] Fix missing parameter in dce_svc_printf() call.
 * 	[1994/08/25  21:18:16  sekhar]
 * 
 * Revision 1.1.7.1  1994/08/04  16:14:48  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:13:53  mdf]
 * 
 * Revision 1.1.3.5  1992/12/29  16:38:55  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:28:35  zeliff]
 * 
 * Revision 1.1.3.4  1992/12/18  22:13:14  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:54:10  sekhar]
 * 
 * Revision 1.1.5.2  1992/12/11  22:40:40  emartin
 * 	Fix harmless mistyped parameter.
 * 
 * Revision 1.1.3.3  1992/10/07  20:38:12  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:20:29  sekhar]
 * 
 * Revision 1.1.4.2  1992/10/06  16:04:14  burati
 * 	Second replication code drop
 * 
 * Revision 1.1.3.2  1992/09/29  20:14:12  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:40:36  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  rsdb_log.c V=14 04/29/91 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *      Registry Server Stable Storage Log - primitives
 *
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rsdb_log.c	14 - 04/29/91";
#endif

#include <sys/types.h>
#include <sys/file.h>

#include <dce/dce.h>
#include <dce/rgynbase.h>

#include <sys/types.h>
#include <stdio.h>

#include <rs_base.h>
#include <rs_util.h>
#include <rs_pvt.h>
#include <rs_log.h>
#include <rsdb_sysdep.h>
#include <rsdb_pvt.h>
#include <rsdb_log.h>
#include <rsdb_file.h>

#include <un_io.h>
#include <bstring.h>
#include <sec_svc.h>

#define FILE_CREATION_PROTECTION 0600

/*
 * Global Variables
 */

PUBLIC int  rs_log_fd = -1;

/*
 * r s d b _ l o g _ c r e a t e
 *
 * Create a new log file.
 */
PUBLIC void rsdb_log_create
#ifndef __STDC__
    ( lbp, blen, st )
    Pointer             lbp;
    unsigned32          blen;
    error_status_t      *st;
#else
  (
    Pointer             lbp,
    unsigned32          blen,
    error_status_t      *st
  )
#endif
{
    if (rs_log_fd != -1) {
	dce_svc_printf(SEC_RSDB_LOG_FILE_OPEN_MSG);
        return;
    }

    rs_log_fd = rsdb_log_open_logfile(LOG_FILE, O_RDWR | O_CREAT);
    if (rs_log_fd < 0) {
        /* SVC : error already logged by rsdb_log_open_logfile */
        SET_STATUS(st, sec_rgy_rep_cannot_create_db);
        return;
    }

    if (write(rs_log_fd, (char *) lbp, blen) != blen) {
	dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
		       sec_s_rs_rsdb,
		       svc_c_sev_error,
		       sec_sys_file_write_error,
		       rs_log_fd);
        SET_STATUS(st, sec_rgy_rep_cannot_create_db);
        return;
    }

    rsdb_file_fsync(rs_log_fd);

    CLEAR_STATUS(st);
}

/*
 * r s d b _ l o g _ o p e n _ l o g f i l e
 *
 * Open an existing log file.
 */
PUBLIC int rsdb_log_open_logfile
#ifndef __STDC__
    ( fname, mode )
    char        *fname;
    int         mode;
#else
  (
    char        *fname,
    int         mode
  )
#endif
{
    int     fd;

    fd = rsdb_file_open(fname, mode, FILE_CREATION_PROTECTION);

    /* SVC OPEN :
     *
     *   Following depends on rsdb_file_open() preserving the errorno ;
     *   this is already assumed elsewhere in the code. So it is not a
     *   new requirement.
     *
     *   However rsdb_file_open()  may not be preserving the errno.
     *   and so should be checked.
     */
    if ( fd < 0 )
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
		       sec_s_rs_rsdb,
                       svc_c_sev_error,
                       sec_sys_errno_file_text,
		       fname,
		       strerror(errno));
    return fd;
}

/*
 * r s d b _ l o g _ o p e n
 *
 * Open an existing log file - and abort if it fails
 */
PUBLIC void rsdb_log_open (
#ifdef __STDC__
    void
#endif
)
{
    if (rs_log_fd != -1) {
        dce_svc_printf(SEC_RSDB_LOG_FILE_OPEN_MSG);
        return;
    }

    rs_log_fd = rsdb_log_open_logfile(LOG_FILE, O_RDWR);

    if (rs_log_fd < 0) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_errno_file_text, 
		       LOG_FILE,
                       strerror(errno));
    }
}

/*
 * r s d b _ l o g _ c l o s e
 *
 * Close the log file.
 */
PUBLIC void rsdb_log_close (
#ifdef __STDC__
    void
#endif
)
{
    if (rs_log_fd == -1) return;

    rsdb_file_close(rs_log_fd);

    rs_log_fd = -1;
}
