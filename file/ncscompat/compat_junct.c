/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: compat_junct.c,v $
 * Revision 1.1.64.1  1996/10/02  17:54:30  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:12  damon]
 *
 * Revision 1.1.59.2  1994/06/09  14:13:26  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:26:33  annie]
 * 
 * Revision 1.1.59.1  1994/02/04  20:22:46  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:55  devsrc]
 * 
 * Revision 1.1.57.1  1993/12/07  17:28:07  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:49:48  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/19  16:08:29  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  15:12:03  cjd]
 * 
 * Revision 1.1.4.3  1992/11/24  17:55:14  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:10:11  bolinger]
 * 
 * Revision 1.1.4.2  1992/08/31  20:07:29  jaffe
 * 	Transarc delta: cfe-ot4389-fix-dfsjunct-proc-interface 1.1
 * 	  Selected comments:
 * 	    Clean up the dfs_GetJunctionName procedure interface not to use a global
 * 	    variable as an input parameter.  This should allow exporting of the
 * 	    procedure's code to libdce.a without losing face.
 * 	    Move to new interface.
 * 	Transarc delta: jaffe-ot4300-fix-compilation-warnings 1.2
 * 	  Selected comments:
 * 	    Fix compilation warnings
 * 	    pass first argument to uuid_from_string as an (unsigned_char_t *)
 * 	    cast pointer assignments to profileName as (unsigned_char_t *)
 * 	Transarc delta: vijay-ot3973-ncsubik-improved-error-handling 1.4
 * 	  Selected comments:
 * 	    Error handling in ncsubik was erroneous in most parts. Ubik errors are
 * 	    generated for different error conditions and these are propagated to the
 * 	    callers. Also, upon hitting an error condition, control returns with an error
 * 	    instead of proceeding. Also resource cleanup is handled in the right way. Some
 * 	    miscellaneous bugs found in the code review process were also fixed.
 * 	    This delta is a checkpoint to upgrade, not ready for release yet.
 * 	    code cleanup, see above
 * 	    fix a bug in handling rpc_s_no_more_members
 * 	    Add a help feature to utst_client
 * 	    Fix some compilation problems in backup
 * 	[1992/08/30  03:01:52  jaffe]
 * 
 * Revision 4.1  1992/03/09  14:09:42  mason
 * 	Upgrade source to build 5.
 * 	[from revision 4.0 by delta osf-b5, revision 1.1]
 * 
 * Revision 1.1  1992/01/19  02:47:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* Copyright 1991 (C) Transarc Corporation -- All Rights Reserved */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/common_data.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dce/rpc.h>
#include <stdio.h>	/* need before dce_cf.h */
#include <dce/dce_cf.h>
#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <errno.h>

#define HIDDEN_ANNON "none"
#define CELL_PROFILE "/cell-profile"

/* Get the name of the junction going from the named cell to DFS. */
void dfs_GetJunctionName(celln, allowHide, junctp, sizejunctp, stp)
char *celln;
int allowHide;
char *junctp;
int sizejunctp;
unsigned long *stp;
{
    unsigned_char_t *profileName, *memberName, *annon;
    unsigned32 count, prio;
    rpc_ns_inq_handle_t context;
    rpc_if_id_t ifId;
    uuid_t fsUuid;
    error_status_t unusedSt;

    *stp=0;
    if (sizejunctp <= 0 || junctp == (char *)NULL) {
      *stp = rpc_s_invalid_arg;
      return;
    }

    profileName = (unsigned_char_t *)malloc(strlen(celln)+strlen(CELL_PROFILE)+1);
    if (profileName == (unsigned_char_t *)NULL) {
	*stp = rpc_s_no_memory;
	return;
    }
    strcpy ((char *)profileName, celln);
    strcat ((char *)profileName, CELL_PROFILE);

    uuid_from_string ((unsigned_char_t *)"4d37f2dd-ed43-0000-02c0-37cf2e000001",
		      &fsUuid, stp);

    ifId.uuid = fsUuid;
    ifId.vers_major = 4;
    ifId.vers_minor = 0;

    rpc_ns_profile_elt_inq_begin (rpc_c_ns_syntax_default,
				  profileName,
				  rpc_c_profile_match_by_if,
				  &ifId,
				  rpc_c_vers_compatible,
				  rpc_c_ns_syntax_default,
				  (unsigned_char_t *)NULL,
				  &context,
				  stp);
    if (*stp != rpc_s_ok) {
      free(profileName);
      goto exit;
    }

    rpc_ns_profile_elt_inq_next (context,
				 &ifId,
				 &memberName,
				 &prio,
				 &annon,
				 stp);
    if (*stp != rpc_s_ok) {
      free(profileName);
      rpc_ns_profile_elt_inq_done(&context, &unusedSt);
      goto exit;
    }
    
    rpc_ns_profile_elt_inq_done (&context, stp);
    if (*stp != rpc_s_ok) {
      free(profileName);
      rpc_string_free(&memberName, &unusedSt);
      rpc_string_free(&annon, &unusedSt);
      goto exit;
    }

    if (allowHide && strcmp((char *)annon, HIDDEN_ANNON) != 0) {
	bzero(junctp, sizejunctp);
	*stp=0;
	free(profileName);
	rpc_string_free(&memberName, &unusedSt);
	rpc_string_free(&annon, &unusedSt);
	return;
    }

    if (strncmp((char *)memberName, "/.:", 3) == 0) {
	if (sizejunctp < (int)(strlen((char *)memberName))-3) {
	    *stp = ERANGE;
	    free(profileName);
	    rpc_string_free(&memberName, &unusedSt);
	    rpc_string_free(&annon, &unusedSt);
	    return;
	}
	strcpy (junctp, (char *)memberName+3);
    }
    else if (strncmp((char *)memberName, celln, strlen(celln)) == 0){
	if (sizejunctp < (int)(strlen((char *)memberName)-strlen(celln))) {
	    *stp = ERANGE;
	    free(profileName);
	    rpc_string_free(&memberName, &unusedSt);
	    rpc_string_free(&annon, &unusedSt);
	    return;
	}
	strcpy(junctp, (char *)memberName+strlen(celln));
    }
    else {
	*stp = rpc_s_entry_not_found;
	free(profileName);
	rpc_string_free(&memberName, &unusedSt);
	rpc_string_free(&annon, &unusedSt);
	return;
    }

    free(profileName);
    rpc_string_free (&memberName, &unusedSt);
    rpc_string_free (&annon, &unusedSt);
exit:
    return;
}

EXPORT void dfs_GetBakJunctionName(celln, junctp, sizejunctp, stp)
     IN 	char *celln;
     INOUT 	char *junctp;
     IN 	int sizejunctp;
     OUT 	unsigned long *stp;
{
    unsigned_char_t *profileName, *memberName, *annon;
    unsigned32 count, prio;
    rpc_ns_inq_handle_t context;
    rpc_if_id_t ifId;
    uuid_t fsUuid;
    error_status_t unusedSt;

    *stp=0;
    if (sizejunctp <= 0 || junctp == (char *)NULL) {
      *stp = rpc_s_invalid_arg;
      return;
    }

    profileName = (unsigned_char_t *)malloc(strlen(celln)+strlen(CELL_PROFILE)+1);
    if (profileName == (unsigned_char_t *)NULL) {
	*stp = rpc_s_no_memory;
	return;
    }
    strcpy ((char *)profileName, celln);
    strcat ((char *)profileName, CELL_PROFILE);

    uuid_from_string ((unsigned_char_t *)"eb814e2a-0099-11ca-8678-02608c2ea96e",
		      &fsUuid, stp);
    if (*stp != uuid_s_ok) {
      free(profileName);
      goto exit;
    }
    ifId.uuid = fsUuid;
    ifId.vers_major = 4;
    ifId.vers_minor = 0;

    rpc_ns_profile_elt_inq_begin (rpc_c_ns_syntax_default,
				  profileName,
				  rpc_c_profile_match_by_if,
				  &ifId,
				  rpc_c_vers_compatible,
				  rpc_c_ns_syntax_default,
				  (unsigned_char_t *)NULL,
				  &context,
				  stp);
    if (*stp != rpc_s_ok) {
      free(profileName);
      goto exit;
    }

    rpc_ns_profile_elt_inq_next (context,
				 &ifId,
				 &memberName,
				 &prio,
				 &annon,
				 stp);
    if (*stp != rpc_s_ok) {
      free(profileName);
      rpc_ns_profile_elt_inq_done(&context, &unusedSt);
      goto exit;
    }
    
    rpc_ns_profile_elt_inq_done (&context, stp);
    if (*stp != rpc_s_ok) {
      free(profileName);
      rpc_string_free(&memberName, &unusedSt);
      rpc_string_free(&annon, &unusedSt);
      goto exit;
    }

    if (strncmp((char *)memberName, "/.:", 3) == 0) {
	if (sizejunctp < (int)strlen((char *)memberName)-3) {
	    *stp = ERANGE;
	    free(profileName);
	    rpc_string_free(&memberName, &unusedSt);
	    rpc_string_free(&annon, &unusedSt);
	    return;
	}
	strcpy (junctp, (char *)memberName+3);
    }
    else if (strncmp((char *)memberName, celln, strlen(celln)) == 0){
	if (sizejunctp < (int)(strlen((char *)memberName)-strlen(celln))) {
	    *stp = ERANGE;
	    free(profileName);
	    rpc_string_free(&memberName, &unusedSt);
	    rpc_string_free(&annon, &unusedSt);
	    return;
	}
	strcpy(junctp, (char *)memberName+strlen(celln));
    }
    else {
	*stp = rpc_s_entry_not_found;
	free(profileName);
	rpc_string_free(&memberName, &unusedSt);
	rpc_string_free(&annon, &unusedSt);
	return;
    }

    free(profileName);
    rpc_string_free (&memberName, &unusedSt);
    rpc_string_free (&annon, &unusedSt);

exit:
    return;
}

