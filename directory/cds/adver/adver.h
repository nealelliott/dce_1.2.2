/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: adver.h,v $
 * Revision 1.1.4.2  1996/02/18  23:33:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:19  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:09:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:07  root]
 * 
 * Revision 1.1.2.8  1994/09/06  17:36:51  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:28:11  proulx]
 * 
 * 	fix timestamps
 * 
 * 	fix timestamps
 * 
 * 	fix timestamps
 * 
 * Revision 1.1.2.7  1994/08/03  18:58:50  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:01:38  mccann]
 * 
 * Revision 1.1.2.6  1994/06/30  19:14:57  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:34:31  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.2.5  1994/06/09  16:06:43  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:14  devsrc]
 * 
 * Revision 1.1.2.4  1994/05/25  22:22:07  peckham
 * 	Add adver_rpc_rundown() prototype.
 * 	[1994/05/25  22:17:14  peckham]
 * 
 * Revision 1.1.2.3  1994/05/12  21:09:53  peckham
 * 	Remove Uid, BinAbsTime and UTCValue.
 * 	[1994/05/12  19:06:48  peckham]
 * 
 * Revision 1.1.2.2  1994/04/19  20:15:34  jd
 * 	Include select.h for typedef of fd_set on AIX.
 * 	[1994/04/19  19:27:51  jd]
 * 
 * Revision 1.1.2.1  1994/04/19  17:05:02  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:20:43  mccann]
 * 
 * $EndLog$
 */
#ifndef _ADVER_H
#define _ADVER_H
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
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
 */

#include <dce/dce.h>
#include <dce/dcesvcmsg.h>
#include <dce/dce_msg.h>
#include <dce/dcecdsmsg.h>
#include <dce/dce_error.h>
#include <dce/assert.h>
#include <dce/cdsclerk.h>
#include <dcecdssvc.h>
#include <dcecdsmac.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <threads_lib.h>
#include <adver_ipc.h>
#ifdef _AIX
#include <sys/select.h>
#endif


/* Macros for thread that deal with servicablity */
#define CDS_LOCK(mutex) \
 { int thstatus;  if ((thstatus = dthread_lock(&(mutex))) < 0) { \
    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus); }}

#define CDS_END_LOCK(mutex) \
 { int thstatus;  if ((thstatus = dthread_unlock(&(mutex))) < 0) { \
    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG,(long)thstatus); } }

/* Default solicit holddown time is 15 seconds   */
#define DEF_SOLICIT_VAL 15
/* Default interval for flushing cache to disk is 12 hours (in seconds) */
#define DEF_FLUSH_CACHE_VAL 43200

/* V2.0 CDS CLERK entity structure */
typedef struct nsgbl_clerk
    {
  	utc_t		cl_create_time;
    } nsgbl_clerk_t;

/*
 * Prototypes
 */

/*
 * adver_cache.c
 */

void *
adver_cache_new (int);

void
adver_cache_delete (void *);

ClerkStates
adver_cache_get_state (void *);

void
adver_cache_set_state (void *,
                       ClerkStates);

void
adver_cache_add_rep (void *,
                     FullName_t *,
                     ObjUID_t *,
                     Set_t *,
                     int);

void
adver_cache_add_cell (void *,
                      SimpleName_t *,
                      ObjUID_t *,
                      byte_t *);

/*
 * adver_clerk.c
 */

void *
adver_clerk_new (void *);

void
adver_clerk_delete (void *);

void
adver_clerk_signal (void *,
		    int);

/*
 * ADVER_SERVICE_NCL.C
 */

int 
TestAdverUserAccess (
    handle_t           ,
    sec_acl_permset_t  );

/*
 * From ADVER_CDS_EVENTS.C
 */

void *
InitializeRPC (char      *,
               int       );

void
adver_rpc_start (void *,
		 void *);

void
cds_solicit_kill (void *);

void
adver_rpc_rundown (void);

void
TerminateRPC (void *);

error_status_t
cds_SendSolicitServer (
    unsigned_char_t    *,
    cds_Name_t         *);

int 
adver_cds_bind (handle_t         *,
                unsigned_char_t  *,
                error_status_t   *);

void
cds_SendAdvertisements (
			void );
void
cds_SendSolicit (
			void );

void
cds_clear_rpcList (
		   void );

void
L_to_rpcList_AdvertiseMessage_V2 (
    byte_t		*);


/*
 * From ADVER_SERVICE_NCL.C
 */

int 
dnsclerk_create (unsigned long);

/*
 * adver_socket.c
 */
int
adver_open_socket (struct sockaddr_un *, int);

int
adver_accept_socket (int, struct sockaddr_un *, int *);

int
adver_read_socket (int, char *, int);

/*
 * From ADVER_DACL.C
 */

int 
clerk_mgmt_acl_init(
		    void );

/*
 * adver_server.c
 */

void *
adver_server_new (void *,
                  void *);

void
adver_server_delete (void *);

#endif  /* #ifndef _ADVER_H */
