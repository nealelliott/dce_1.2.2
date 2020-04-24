/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsauth_sec_key_mgmt.c,v $
 * Revision 1.1.512.1  1996/10/02  20:59:09  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:29  damon]
 *
 * Revision 1.1.507.3  1994/07/13  22:26:39  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:14:03  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:01:48  mbs]
 * 
 * Revision 1.1.507.2  1994/06/09  14:19:41  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:31  annie]
 * 
 * Revision 1.1.507.1  1994/02/04  20:29:47  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:17:56  devsrc]
 * 
 * Revision 1.1.505.1  1993/12/07  17:33:26  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:56:03  jaffe]
 * 
 * Revision 1.1.4.3  1993/01/21  15:19:33  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:57:59  cjd]
 * 
 * Revision 1.1.4.2  1992/11/24  18:33:02  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:24:31  bolinger]
 * 
 * Revision 1.1.2.3  1992/04/14  03:36:06  mason
 * 	Transarc delta: bab-ot2299-remove-auth-ifdefs 1.1
 * 	  Files modified:
 * 	    bosserver: bossvr_ncs_procs.c
 * 	    security/dacl: dacl_sec_acl.c, dacl_sec_rgy.c
 * 	    security/dfsauth: dfsauth_sec_key_mgmt.c, dfsauth_server.c
 * 	  Selected comments:
 * 	    The backwards compatibility ifdefs for the various auth drops are nolonger
 * 	    needed and only served to confuse things, especially given that the
 * 	    versions of them that were left in the code were not consistently used.
 * 	    Since we longer need to maintain this backwards compatibility, the ifdefs
 * 	    should be replaced with the appropriate code.
 * 	    Replace all occurrences of #if defined(auth..) (& accompanying
 * 	    #else & #endif) with the appropriate code branches.
 * 	[1992/04/13  17:34:13  mason]
 * 
 * Revision 1.1.2.2  1992/04/06  02:22:05  garyf
 * 	fix compilation warnings
 * 	[1992/04/06  02:17:48  garyf]
 * 
 * Revision 1.1  1992/01/19  02:52:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	dfsauth_sec_key_mgmt.c - utility routines for use iwth HP/Apollo
 * sec_key_mgmt routines.
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */
#include <dcedfs/param.h>
#include <dcedfs/stds.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/dfsauth/dfsauth_sec_key_mgmt.c,v 1.1.512.1 1996/10/02 20:59:09 damon Exp $")

#include <dce/keymgmt.h>
#include <dce/acct.h>
#include <dce/secidmap.h>

#include <krb5/krb5.h>

#include <dfsauth.h>

#include <../dfssec_errorStrings.h>

#include <dfsauth_debug.h>
#include <dfsauth_trace.h>
#include "../exception_print.h"		/* get pthread stuff */

#include <dcedfs/icl.h>

IMPORT struct icl_set *dfsauth_iclSetp;

/*
 * The first routine does a little more than cover the sec_key_mgmt routine
 * by doing a little extra work.  We also went ahead and changed the signature.
 */
/*
 * NB: sec_key_mgmt_free_key is used to free an entire array, not just a single key
 */
EXPORT void dfsauth_sec_key_mgmt_free_key(voidKeydataP)
     void *	voidKeydataP;
{     
  int			i;
  sec_passwd_rec_t *	keydataP = (sec_passwd_rec_t *)voidKeydataP;
  error_status_t	secKeyError;
  static char		routineName[] = "dfsauth_sec_key_mgmt_free_key";
  
  /* free out of line data for each key and zero the key/password memory before return */
#ifdef notdef
  /* for now, we have to trust that the free_key routine is doing the right thing */
  for (i = 0; keydataP[i].key.key_type != sec_passwd_none; i++) {
    if (keydataP[i].pepper != (char *)NULL) {
      free(keydataP[i].pepper);
    }
    
    if ((keydataP[i].key.key_type == sec_passwd_plain) &&
	(keydataP[i].key.tagged_union.plain != (char *)NULL)) {
      /*
       * Note assumption that the password is a zero-terminated string.
       * This must be true until/unless a length specifier is ever put into
       * the sec_passwd_rec_t structure.
       */
      bzero(keydataP[i].key.tagged_union.plain,
	    strlen(keydataP[i].key.tagged_union.plain));
      free((char *)(keydataP[i].key.tagged_union.plain));
    }
    else if (keydataP[i].key.key_type == sec_passwd_des) {
      /* re-init the memory, just in case */
      bzero((char *)(keydataP[i].key.tagged_union.des_key), sizeof(sec_passwd_des_key_t));
    }
  }
#endif /* notdef */

  TRY
    sec_key_mgmt_free_key(keydataP, &secKeyError);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "sec_key_mgmt_free_key",
		   __FILE__, __LINE__);
    RERAISE;
  ENDTRY;
  
  if (secKeyError != error_status_ok) {
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
	     ("%s: unexpected error freeing key data storage: %s\n",
	      routineName, RgySec_ErrorString(&secKeyError)));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_MGMT_TRACE_0 , ICL_TYPE_STRING, RgySec_ErrorString(&secKeyError));
  }
}

EXPORT void dfsauth_sec_key_mgmt_keyinfo(voidKeydataP, keyinfoP, principalNameP)
     void *			voidKeydataP;
     dfsauth_keyinfo_t *	keyinfoP;
     char *			principalNameP;
{
  sec_passwd_rec_t *	keydataP = (sec_passwd_rec_t *)voidKeydataP;
  char *		keyBytesP;
  int			realCksumLength;
  krb5_checksum		cksum;

  sec_rgy_cursor_t	unusedAcctCursor;
  sec_rgy_login_name_t	unusedNameResult;
  sec_rgy_sid_t		unusedIdSid;
  sec_rgy_unix_sid_t	unusedUnixSid;
  sec_rgy_acct_key_t	unusedKeyParts;
  sec_rgy_acct_user_t	userPart;
  sec_rgy_acct_admin_t	unusedAdminInfo;
  sec_rgy_login_name_t	loginName;
  sec_rgy_name_t	localPrincipalName;
  
  error_status_t	secError;

  static char		routineName[] = "dfsauth_sec_key_mgmt_keyinfo";
  
  /* first, zero the keyinfo fields */
  bzero((char *)keyinfoP, sizeof(dfsauth_keyinfo_t));

  /* this only makes sense for a DES key */
  if (keydataP->key.key_type == sec_passwd_des) {
    keyBytesP = (char *)keydataP->key.tagged_union.des_key;
    
    /* calculate and copy the checksum */
    /* Joe Pato told us to call the krb5 routine directly, here */
    
    realCksumLength = krb5_cksumarray[CKSUMTYPE_CRC32]->checksum_length;
    cksum.contents = (krb5_octet *)osi_Alloc(realCksumLength);
    if (cksum.contents != (krb5_octet *)NULL) {
      (*(krb5_cksumarray[CKSUMTYPE_CRC32]->sum_func))(
						      (krb5_pointer)keyBytesP,
						      sizeof(sec_passwd_des_key_t),
						      (krb5_pointer)keyBytesP,
						      sizeof(sec_passwd_des_key_t),
						      &cksum);
      
      /* copy out the checksum */
      if (realCksumLength != sizeof(keyinfoP->keyCheckSum)) {
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		 ("%s: Warning: real checksum length (%d) != dfsauth checksum length (%d)\n",
		  routineName, realCksumLength,
		  sizeof(keyinfoP->keyCheckSum)));
	icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_MGMT_TRACE_1 , ICL_TYPE_LONG, realCksumLength, ICL_TYPE_LONG, sizeof(keyinfoP->keyCheckSum));
      }
      /* make sure we only copy what we can handle */
      bcopy((char *)(cksum.contents), (char *)&(keyinfoP->keyCheckSum),
	    ((sizeof(keyinfoP->keyCheckSum) <= realCksumLength) ?
	     sizeof(keyinfoP->keyCheckSum) :
	     realCksumLength));

      /* free the checksum data */
      osi_Free((void *)(cksum.contents), realCksumLength);
    }
  }	  

  /* find out about last key modification */
  /*
   * Currently, we get the last modification from the registry server.  We hope that,
   * someday, we may be able to get this from the local keyfile for the particular key.
   */

  /*
   * sec_rgy_acct_lookup will not take a full principal name, so we have to parse it
   */
  sec_id_parse_name(sec_rgy_default_handle,
		    (unsigned char *)principalNameP,
		    NULL,
		    (uuid_t *)NULL,
		    localPrincipalName,
		    (uuid_t *)NULL,
		    &secError);
  if (secError == error_status_ok) {
    bzero((char *)&unusedAcctCursor, sizeof(sec_rgy_cursor_t));/* just in case it checks */
    bzero((char *)&loginName, sizeof(sec_rgy_login_name_t));
    strncpy((char *)loginName.pname, (char *)localPrincipalName, sizeof(sec_rgy_name_t));
    sec_rgy_acct_lookup (sec_rgy_default_handle,
			 &loginName,
			 &unusedAcctCursor,
			 &unusedNameResult,
			 &unusedIdSid,
			 &unusedUnixSid,
			 &unusedKeyParts,
			 &userPart,
			 &unusedAdminInfo,
			 &secError);
    if (secError == error_status_ok) {
      keyinfoP->mod_sec = userPart.passwd_dtm;
    }
    else {
      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
	       ("%s: sec_rgy_acct_lookup error checking account change date: %s\n",
		routineName, RgySec_ErrorString(&secError)));
      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_MGMT_TRACE_2 , ICL_TYPE_STRING, RgySec_ErrorString(&secError));
    }
  }
  else {
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
	     ("%s: sec_id_parse_name error: %s\n",
	      routineName, RgySec_ErrorString(&secError)));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_MGMT_TRACE_3 , ICL_TYPE_STRING, RgySec_ErrorString(&secError));
  }
  
}
