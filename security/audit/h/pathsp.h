/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: pathsp.h,v $
 * Revision 1.1.7.1  1996/07/08  18:27:55  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:25 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  22:57:38  marty  1.1.5.2]
 *
 * Revision 1.1.5.2  1996/02/18  22:57:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:30  marty]
 * 
 * Revision 1.1.5.1  1995/12/08  17:40:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:17:53  root]
 * 
 * Revision 1.1.2.2  1994/01/25  04:59:38  luan
 * 	CR 9745 9746: Added macros for dynamic file-name generation.
 * 	[1994/01/25  04:21:25  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:09  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:22  zeliff]
 * 
 * $EndLog$
 */

#ifndef _PATHSP_H_
#define _PATHSP_H_

#include <dce/dce.h>
#include <esl_filep.h>

extern char * concact_strns(
char **dest,
char *prefix, 
char *str, 
char *suffix
);

#define CONCAT_STRNS(dest, prefix, str, suffix)  (dest ? dest : \
   (char *) concat_strns(&dest, prefix, str, suffix))

#define ESL_UPDATE_FILE_NAME(esl_type) (aud_g_esl_update_full_fname[esl_type] ?\
   aud_g_esl_update_full_fname[esl_type] : \
   CONCAT_STRNS(aud_g_esl_update_full_fname[esl_type], \
	dcelocal_path, aud_g_esl_update_fname[esl_type], ""))

#define ESL_FILE_NAME(esl_type) (aud_g_esl_full_fname[esl_type] ?\
   aud_g_esl_full_fname[esl_type] : \
   CONCAT_STRNS(aud_g_esl_full_fname[esl_type], \
	dcelocal_path, aud_g_esl_fname[esl_type], ""))

extern char * event_class_dir;
extern char * daemon_working_dir;
extern char * client_binding_dir;
extern char * daemon_identity_file;
extern char * daemon_binding_file;

#define EVENT_CLASS_DIR \
   CONCAT_STRNS(event_class_dir, dcelocal_path, "/etc/audit/ec/", "")

#define DAEMON_WORKING_DIR \
   CONCAT_STRNS(daemon_working_dir, dcelocal_path, "/var/audit/adm/", "")

#define CLIENT_BINDING_DIR \
   CONCAT_STRNS(client_binding_dir, dcelocal_path, "/var/audit/client/", "")

#define DAEMON_IDENTITY_FILE \
   CONCAT_STRNS(daemon_identity_file, dcelocal_path, \
	     "/var/audit/daemon_identity", "")

#define DAEMON_BINDING_FILE \
   CONCAT_STRNS(daemon_binding_file, dcelocal_path, \
	     "/var/audit/daemon_binding", "")

#define DAEMON_DEFAULT_ACL_FILE \
   CONCAT_STRNS(daemon_default_acl_file, dcelocal_path, \
	     "/var/audit/acl", "")


#endif /* _PATHSP_H_ */
