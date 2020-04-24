/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsauth_localkey.c,v $
 * Revision 1.1.608.1  1996/10/02  20:58:54  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:18  damon]
 *
 * Revision 1.1.603.3  1994/07/13  22:26:33  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:13:58  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:01:43  mbs]
 * 
 * Revision 1.1.603.2  1994/06/09  14:19:29  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:23  annie]
 * 
 * Revision 1.1.603.1  1994/02/04  20:29:37  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:17:51  devsrc]
 * 
 * Revision 1.1.601.1  1993/12/07  17:33:18  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:54:51  jaffe]
 * 
 * Revision 1.1.5.3  1993/01/21  15:19:12  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:57:13  cjd]
 * 
 * Revision 1.1.5.2  1992/11/24  18:32:08  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:23:49  bolinger]
 * 
 * Revision 1.1.3.3  1992/01/24  03:48:27  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:19:08  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:23:25  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:39:20  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:21:38  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*
 *	dfsauth_localkey.c -- routines for accessing the local keyfile for dfsauth package
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#include <stdio.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/icl.h>

#include <krb5/krb5.h>
#include <krb5/keytab.h>
#include <krb5/func-proto.h>

#include <dfsauth_debug.h>
#include <dfsauth_trace.h>

#include <dfsauth_localkey.h>

#include <../dfssec_errorStrings.h>

#ifndef KERNEL
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/keymgmt.h>

#define DEFAULT_PASSWORD_FILENAME	"/krb5/passwords"

IMPORT struct icl_set *dfsauth_iclSetp;

PRIVATE krb5_error_code dfsauth_Keytab_GetLocalKrb5Key(keyfilenameP, prinNameP, theKeyPP)
     char *			keyfilenameP;
     char *			prinNameP;
     char **			theKeyPP;
{
  krb5_error_code	rtnVal = 0;
  krb5_keytab		ktid;
  krb5_keytab_entry	keyEntry;
/*
  krb5_keyblock *	keyCopyP;
*/
  char *		keyCopyP;
  error_status_t	keyStatus = error_status_ok;
  sec_passwd_rec_t *	passwordRecordP;
  static char		routineName[] = "dfsauth_KeyTab_GetLocalKrb5Key";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_LOCALKEY_KEYTAB_0 );
#ifdef notdef
  rtnVal = dfsauth_InitKeytab(keyfilenameP, &ktid);
  if (rtnVal == 0) {
    rtnVal = dfsauth_Keytab_FindLatestKey(&ktid, prinNameP, &keyEntry);
    if (rtnVal == 0) {
      keyCopyP = (krb5_keyblock *)osi_Alloc(sizeof(krb5_keyblock));
      rtnVal = krb5_copy_keyblock(&(keyEntry.key), keyCopyP);
      if (rtnVal == 0) {
	*theKeyPP = (char *)keyCopyP;
      }
    }
  }
#else /* notdef */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s: about to call sec_key_mgmt_get_key", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_LOCALKEY_KEYTAB_1 );

  sec_key_mgmt_get_key(rpc_c_authn_dce_private,
		       (void *)NULL,
		       (unsigned char *)prinNameP,
		       0 /* => get latest key */,
		       (void **)&passwordRecordP,
		       &keyStatus);

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s: returned from sec_key_mgmt_get_key", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_LOCALKEY_KEYTAB_2 );

  if (keyStatus == error_status_ok) {
    if (passwordRecordP->key.key_type == sec_passwd_des) {
      keyCopyP = (char *)osi_Alloc(sizeof(sec_passwd_des_key_t));
      bcopy((char *)(passwordRecordP->key.tagged_union.des_key), keyCopyP,
	    sizeof(sec_passwd_des_key_t));
      *theKeyPP = keyCopyP;

      sec_key_mgmt_free_key(passwordRecordP, &keyStatus);
      if (keyStatus != error_status_ok) {
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		 ("%s: sec_key_mgmt_free_key returned error: %s",
		  routineName, RgySec_ErrorString(&keyStatus)));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_LOCALKEY_KEYTAB_3 , ICL_TYPE_STRING, RgySec_ErrorString(&keyStatus));
      }
    }
    else {
      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	       ("%s: sec_key_mgmt_get_key returned wrong key type: %u",
		routineName, RgySec_ErrorString(&keyStatus)));
      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_LOCALKEY_KEYTAB_4 , ICL_TYPE_LONG, RgySec_ErrorString(&keyStatus));
    }
  }
  else {
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	     ("%s: sec_key_mgmt_get_key returned error: %s",
	      routineName, RgySec_ErrorString(&keyStatus)));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_LOCALKEY_KEYTAB_5 , ICL_TYPE_STRING, RgySec_ErrorString(&keyStatus));
  }
#endif /* notdef */
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s exiting, returning %#x", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_LOCALKEY_KEYTAB_6 , ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

PRIVATE krb5_error_code dfsauth_Keytab_GetLocalPasswdKey(keyfilenameP, prinNameP, theKeyPP)
     char *			keyfilenameP;
     char *			prinNameP;
     char **			theKeyPP;
{
  krb5_error_code	rtnVal = 0;
  char			passwordFilename[BUFSIZ];
  char			passwdBuffer[BUFSIZ];
  static char		routineName[] = "dfsauth_Keytab_GetLocalPasswdKey";

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_LOCALKEY_KEYTAB_7 );
  
  if (keyfilenameP != (char *)NULL) {
    (void)strncpy(passwordFilename, keyfilenameP, sizeof(passwordFilename) - 1);
  }
  else {
    (void)strncpy(passwordFilename, DEFAULT_PASSWORD_FILENAME,
		  sizeof(passwordFilename) - 1);
  }

  rtnVal = dfsauth_Passtab_FindPassword(passwordFilename, prinNameP, 0,
					passwdBuffer, sizeof(passwdBuffer));

  *theKeyPP = (char *)osi_Alloc(strlen(passwdBuffer) + 1);
  (void)strcpy(*theKeyPP, passwdBuffer);

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s exiting, returning %#x", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_LOCALKEY_KEYTAB_8 , ICL_TYPE_LONG, rtnVal);
  
  return rtnVal;
}

EXPORT krb5_error_code dfsauth_Keytab_GetLocalKey(keyfilenameP, prinNameP, keyTypeP,
						  theKeyPP)
     char *			keyfilenameP;
     char *			prinNameP;
     dfs_krb_key_type_t *	keyTypeP;
     char **			theKeyPP;
{
  krb5_error_code	rtnVal = 0;
  static char		routineName[] = "dfsauth_Keytab_GetLocalKey";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_LOCALKEY_KEYTAB_9 );
  
#if defined(VALIDATE_USES_PASSWORDS)
  *keyTypeP = dfs_krb_passwd;
  rtnVal = dfsauth_Keytab_GetLocalPasswdKey(keyfilenameP, prinNameP, theKeyPP);
#else
  *keyTypeP = dfs_krb_key;
  rtnVal = dfsauth_Keytab_GetLocalKrb5Key(keyfilenameP, prinNameP, theKeyPP);
#endif

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s exiting, returning %#x", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_LOCALKEY_KEYTAB_10 , ICL_TYPE_LONG, rtnVal);
  
  return rtnVal;
}
