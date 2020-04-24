/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsauth_keytab.c,v $
 * Revision 1.1.707.1  1996/10/02  20:58:51  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:17  damon]
 *
 * $EndLog$
 */
/*
 *	dfsauth_keytab.c -- key table routines for dfsauth package
 *
 * Copyright (C) 1991, 1995 Transarc Corporation
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/icl.h>

#if !defined(KERNEL)
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */
#endif /* !defined(KERNEL) */

#include <krb5/krb5.h>
#include <krb5/keytab.h>
#include <krb5/func-proto.h>

#include <krb5/libos.h>
/*
 * The following is not really a legitimately exported interface, but, since they
 * aren't implementing key remove, we feel justified in doing something desperate.
 */
#include <ktfile.h>

#include <dfsauth_trace.h>
#include <dfsauth_debug.h>

#ifndef KERNEL
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/dfsauth/dfsauth_keytab.c,v 1.1.707.1 1996/10/02 20:58:51 damon Exp $")

IMPORT struct icl_set *dfsauth_iclSetp;
IMPORT krb5_kt_ops krb5_ktf_writable_ops;

char	defaultPasswordFilename[] = "/krb5/passwords";

/*
 * InitKeytab routine based on HP/Apollo keytab.c code
 */
krb5_error_code dfsauth_InitKeytab(filenameP, ktidP)
     char *		filenameP;
     krb5_keytab *	ktidP;
{
  krb5_error_code	rtnVal = 0;
  char  		ktname[MAXPATHLEN + sizeof("WRFILE:") + 1];
  char *		ktp;
  static char		routineName[] = "dfsauth_InitKeytab";

  rtnVal = krb5_kt_register(&krb5_ktf_writable_ops);
  if ((rtnVal == 0) || (rtnVal == KRB5_KT_TYPE_EXISTS)) {
    if (filenameP != (char *)NULL) {
      ktp = ktname;
      bzero(ktp, sizeof(ktname));
      strcpy(ktp, "WRFILE:");
      if ((strlen(filenameP) + sizeof("WRFILE:")) >= sizeof(ktname)) {
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		 ("%s: keyfile name `%s' too long; using `foobar' instead",
		  routineName, filenameP));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_0 , ICL_TYPE_STRING, filenameP);
	strcat(ktp, "foobar");
      } else {
	strcat(ktp, filenameP);
      }
    } else {    
      /* Get default keytab name */    
      /* Anticipate that default name is FILE:/foo/bar.  Play with
       * ptr to ktname so that 'FILE' can be changed to 'WRFILE'.
       */
      ktp = &ktname[2];
      if ( rtnVal = krb5_kt_default_name(ktp, sizeof(ktname) - 2) ) {
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		 ("%s: error getting default keytab name", routineName));
	icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_1 );
      }                                                        
      else if( *ktp == 'F' ) {
	ktname[0] = 'W';
	ktname[1] = 'R';
	ktp = ktname;
      }
    }
    if (rtnVal == 0) {
      rtnVal = krb5_kt_resolve(ktp, ktidP);
      if (rtnVal != 0) {
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		 ("%s: error resolving keytab name", routineName));
	icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_2 );
      }
    }          
  }
  else {
#if !defined(KERNEL)
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	     ("%s: error registering ops: %s", routineName, dfs_dceErrTxt(rtnVal)));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_3 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	     ("%s: error registering ops: %lu", routineName, rtnVal));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_4 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
  }
  
  
  return rtnVal;
}

krb5_error_code dfsauth_Passtab_FindPassword(pwFilenameP, prinNameP, vno, passwdP, length)
     char *		pwFilenameP;
     char *		prinNameP;
     int		vno;
     char *		passwdP;
     int		length;
{
  krb5_error_code	principalFound = KRB5KDC_ERR_S_PRINCIPAL_UNKNOWN;
  char			principalBuffer[BUFSIZ];
  char			passwdBuffer[BUFSIZ];
  int			vnoBuff;
  int			biggestVnoSeen = 0;
  FILE *		passwdFileP;

  passwdFileP = fopen(pwFilenameP, "r+");
  if (passwdFileP != (FILE *)NULL) {
    while((principalFound != 0) &&
	  (fscanf(passwdFileP, "%s %d %s", principalBuffer, &vnoBuff, passwdBuffer)
	   != EOF)) {
      if ((strcmp(prinNameP, principalBuffer) == 0) &&
	  (((vno == 0) && (vnoBuff > biggestVnoSeen)) || (vnoBuff == vno))) {
	strncpy(passwdP, passwdBuffer, length - 1);
	biggestVnoSeen = vnoBuff;
	if (vno != 0) {		/* if we don't have to scan the whole file anyway */
	  principalFound = 0;
	}
      }
    }

    if ((vno == 0) && (biggestVnoSeen > 0)) {
      principalFound = 0;
    }
    
    fclose(passwdFileP);
  }
  else {
    principalFound = KRB5_KT_IOERR;
  }

  return principalFound;
}

long dfsauth_Passtab_AddPassword(pwFilenameP, prinNameP, vno, passwdP)
     char *		pwFilenameP;
     char *		prinNameP;
     int		vno;
     char *		passwdP;
{
  long		rtnVal = 0;
  FILE *	passwdFileP;
  char		passwdBuffer[BUFSIZ];
  
  if (vno > 0) {
    if (dfsauth_Passtab_FindPassword(pwFilenameP, prinNameP, vno,
				     passwdBuffer, sizeof(passwdBuffer)) != 0) {
      passwdFileP = fopen(pwFilenameP, "a+");
      
      fprintf(passwdFileP, "%s %d %s ", prinNameP, vno, passwdP);
      
      fclose(passwdFileP);
    }
  }
  else {
    rtnVal = 1;
  }
  
  return rtnVal;
}

long dfsauth_Passtab_RemovePassword(pwFilenameP, prinNameP, vno)
     char *		pwFilenameP;
     char *		prinNameP;
     int		vno;
{
  long		rtnVal = 0;
  char		tempFilename[BUFSIZ];
  char		principalBuffer[BUFSIZ];
  int		vnoBuff;
  char		passwdBuffer[BUFSIZ];
  FILE *	tempFileP;
  FILE *	passwdFileP;

  if (vno > 0) {
    sprintf(tempFilename, "%s.%d", pwFilenameP, osi_GetPid());
    tempFileP = fopen(tempFilename, "w+");
    if (tempFileP != (FILE *)NULL) {
      passwdFileP = fopen(pwFilenameP, "r+");
      if (passwdFileP != (FILE *)NULL) {
	while (fscanf(passwdFileP, "%s %d %s", principalBuffer, &vnoBuff, passwdBuffer)
	       != EOF) {
	  if ((strcmp(principalBuffer, prinNameP) != 0) || (vnoBuff != vno)) {
	    fprintf(tempFileP, "%s %d %s ", principalBuffer, vnoBuff, passwdBuffer);
	  }
	}
	fclose(passwdFileP);
      }
      fclose(tempFileP);

      if (rtnVal == 0) {
	/* move the temp file over the other one */
	if (rename(tempFilename, pwFilenameP) == 0) {
	  unlink(tempFilename);
	}
	else {
	  rtnVal = KRB5_KT_IOERR;
	}
      }	/* end if we wrote the temp file successfully */
    }
  }
  else {
    rtnVal = 1;
  }
  
  return rtnVal;
}

long dfsauth_Passtab_ChangePassword(pwFilenameP, prinNameP, vno, newPasswdP)
     char *		pwFilenameP;
     char *		prinNameP;
     int		vno;
     char *		newPasswdP;
{
  long		rtnVal = 0;

  if (vno > 0) {
    rtnVal = dfsauth_Passtab_RemovePassword(pwFilenameP, prinNameP, vno);
    if (rtnVal == 0) {
      rtnVal = dfsauth_Passtab_AddPassword(pwFilenameP, prinNameP, vno, newPasswdP);
    }
  }
  else {
    rtnVal = 1;
  }
  
  return rtnVal;
}

/*
 * AddKey routine based on HP/Apollo keytab.c code
 */
krb5_error_code dfsauth_Keytab_AddKey(ktidP, prinNameP, kvno, passwdP)
     krb5_keytab *	ktidP;
     char *		prinNameP;
     int		kvno;
     char *		passwdP;
{
  krb5_error_code			rtnVal = 0;
  krb5_enctype				etype;
  krb5_principal			krbPrincipal;
  krb5_data				krbPasswd;
  krb5_data				salt;
  int					passwdSize;
  krb5_keytab_entry			newEntry;
  krb5_keyblock       			master_keyblock;
  krb5_encrypt_block  			master_encblock;
  register krb5_cryptosystem_entry *	csentry;
  static char				routineName[] = "dfsauth_Keytab_AddKey";
  
  rtnVal = krb5_parse_name(prinNameP, &krbPrincipal);
  if (rtnVal == 0) {
    rtnVal = krb5_kt_get_entry(*ktidP, krbPrincipal, kvno, &newEntry);
    if ((rtnVal == KRB5_KT_NOTFOUND) || (rtnVal == ENOENT)) {
      /* initialize encryption */
      master_keyblock.keytype = KEYTYPE_DES;
      if (valid_keytype(master_keyblock.keytype)) {
	etype = krb5_keytype_array[master_keyblock.keytype]->system->proto_enctype;
	
	if (valid_etype(etype)) {
	  master_encblock.crypto_entry = krb5_csarray[etype]->system;
	  csentry = master_encblock.crypto_entry;
	  
	  /* encrypt given password */
	  krbPasswd.data = passwdP;
	  krbPasswd.length = strlen(passwdP);
#ifdef notdef
	  rtnVal = krb5_string_to_key(&master_encblock,
				      master_keyblock.keytype,
				      &newEntry.key,
				      &krbPasswd,
				      krbPrincipal);
#else /* notdef */
	  rtnVal = krb5_principal2salt(krbPrincipal, &salt);
	  if (rtnVal == 0) {
	    rtnVal = krb5_string_to_key(&master_encblock,
					master_keyblock.keytype,
					&newEntry.key,
					&krbPasswd,
					&salt);
	  }
	  else {
	    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		     ("%s: error converting principal name to salt",
		      routineName));
	    icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_5 );
	  }
#endif /* notdef */
	  bzero(passwdP, strlen(passwdP));
	  if (rtnVal == 0) {
	    newEntry.principal = krbPrincipal;
	    newEntry.vno = kvno;
	    
	    rtnVal = krb5_kt_add_entry(*ktidP, &newEntry);
	    
	    bzero((char *)newEntry.key.contents, newEntry.key.length);
	    free((char *)newEntry.key.contents);	/* presumably, this was malloc'd by krb */
	    krb5_free_principal(krbPrincipal);
	  }
	}
	else {
	  rtnVal = KRB5_PROG_ETYPE_NOSUPP;
	}
      }
      else {
	rtnVal = KRB5_PROG_KEYTYPE_NOSUPP;
      }
    }	/* end if the entry was not already in the key table */
  }	/* end if we could parse the principal name */
  
  return rtnVal;
}

/*
 * local (unbroken) version of broken routine from
 * $(DCETOP)src/security/krb5/lib/keytab/file/ktf_ssget.c
 */
krb5_error_code
local_krb5_ktfile_start_seq_get(id, cursorp)
krb5_keytab id;
krb5_kt_cursor *cursorp;
{
    krb5_error_code retval;
    long *fileoff;

    if (retval = krb5_ktfileint_openr(id))
	return retval;

    if (!(fileoff = (long *)malloc(sizeof(*fileoff)))) {
	krb5_ktfileint_close(id);
	return ENOMEM;
    }
    *fileoff = ftell(KTFILEP(id));
    *cursorp = *((krb5_kt_cursor *)fileoff);	/* <= this is broken in the krb code */

    return 0;
}

krb5_error_code dfsauth_Keytab_ListKeys(ktidP, stream)
     krb5_keytab *	ktidP;
     FILE *		stream;
{
  krb5_error_code	krb5error = 0;
  krb5_keytab_entry	keyEntry;
  krb5_data **		principalPartP;
  krb5_kt_cursor	krbCursor;
  char *		nameBufP;
  
/*
  krb5error = krb5_kt_start_seq_get(*ktidP, &krbCursor);
*/
  krb5error = local_krb5_ktfile_start_seq_get(*ktidP, &krbCursor);
  if (krb5error == 0) {
    while((krb5error = krb5_kt_next_entry(*ktidP, &keyEntry, &krbCursor)) == 0) {
      fprintf(stdout, "found key version number %d for principal: ", keyEntry.vno);
      krb5error = krb5_unparse_name(keyEntry.principal, &nameBufP);
      if (krb5error == 0) {
	fprintf(stream, "%s\n", nameBufP);
      }
      free(nameBufP);
      fflush(stream);
/*
      for (principalPartP = keyEntry.principal;
	   *principalPartP != (krb5_data *)NULL;
	   principalPartP++) {
	bcopy((*principalPartP)->data, nameBufP, (*principalPartP)->length);
	nameBufP[(*principalPartP)->length] = '\0';
	fprintf(stream, "%s ", nameBufP);
      }
*/
    }
    if (krb5error == KRB5_KT_END) {
      krb5error = krb5_kt_end_seq_get(*ktidP, &krbCursor);
    }
  }

  return krb5error;
}

krb5_error_code dfsauth_Keytab_RemoveKey(ktidP, prinNameP, kvno)
     krb5_keytab *	ktidP;
     char *		prinNameP;
     int		kvno;
{
  krb5_error_code	rtnVal = 0;
  krb5_keytab_entry	keyEntry;
  char			originalFilename[BUFSIZ];
  char			tempFilename[BUFSIZ];
  krb5_keytab		tempKtid;
  krb5_principal	krbPrincipal;
  krb5_kt_cursor	krbCursor;

  rtnVal = krb5_parse_name(prinNameP, &krbPrincipal);
  if (rtnVal == 0) {
    /*
      rtnVal = krb5_kt_start_seq_get(*ktidP, &krbCursor);
     */
    rtnVal = local_krb5_ktfile_start_seq_get(*ktidP, &krbCursor);
    if (rtnVal == 0) {
/*
 * the following can be done more efficiently at the ktfile layer using
 * the KTFILENAME macro
 */
      rtnVal = krb5_kt_get_name(*ktidP, originalFilename, sizeof(originalFilename) - 1);
      if (rtnVal == 0) {
	sprintf(tempFilename, "%s.%d", originalFilename, osi_GetPid());
	unlink(tempFilename);		/* make sure there is nothing there */
	rtnVal = dfsauth_InitKeytab(tempFilename, &tempKtid);
	if (rtnVal == 0) {
/*
	  rtnVal = krb5_ktfileint_openw(tempKtid);
*/
	  if (rtnVal == 0) {
	    while (rtnVal == 0) {
/*
	      rtnVal = krb5_ktfileint_read_entry(*ktidP, &keyEntryP);
*/
	      rtnVal = krb5_kt_next_entry(*ktidP, &keyEntry, &krbCursor);
	      if (rtnVal == 0) {
		if ((krb5_principal_compare(keyEntry.principal,
					    krbPrincipal) == FALSE) ||
		    (keyEntry.vno != kvno)) {
/*
		  rtnVal = krb5_ktfileint_write_entry(tempKtid, keyEntryP);
*/
		  rtnVal = krb5_kt_add_entry(tempKtid, &keyEntry);
		}
	      }
	    }	/* end while */
	    if (rtnVal == KRB5_KT_END) {
	      rtnVal = 0;
	    }
/*	    
	    if (rtnVal == 0) {
	      rtnVal = krb5_ktfileint_close(tempKtid);
	    }
*/
	  }	/* end if we opened the temp file successfully */
	}	/* end if we initialized the temp file successfully */
	
	if (rtnVal == 0) {
/*
  	  rtnVal = krb5_ktfileint_close(*ktidP);
*/
	  rtnVal = krb5_kt_end_seq_get(*ktidP, &krbCursor);
	}
      }	/* end if we got the original name successfully */
    }	/* end if we init'd the seq get of the original file successfully */
  }	/* end if we parsed the principal name successfully */
  
  if (rtnVal == 0) {
    /* move the temp file over the other one */
    if (rename(tempFilename, originalFilename) == 0) {
      unlink(tempFilename);
    }
    else {
      rtnVal = KRB5_KT_IOERR;
    }
  }	/* end if we wrote the temp file successfully */

  return rtnVal;
}

krb5_error_code dfsauth_Keytab_FindLatestKey(ktidP, prinNameP, keyEntryP)
     krb5_keytab *		ktidP;
     char *			prinNameP;
     krb5_keytab_entry *	keyEntryP;
{
  krb5_error_code	rtnVal = 0;
  krb5_keytab_entry	keyEntry;
  char			originalFilename[BUFSIZ];
  char			tempFilename[BUFSIZ];
  krb5_keytab		tempKtid;
  krb5_principal	krbPrincipal;
  krb5_kt_cursor	krbCursor;
  unsigned int		biggestVnoSeen = 0;
  
  rtnVal = krb5_parse_name(prinNameP, &krbPrincipal);
  if (rtnVal == 0) {
    /*
      rtnVal = krb5_kt_start_seq_get(*ktidP, &krbCursor);
     */
    rtnVal = local_krb5_ktfile_start_seq_get(*ktidP, &krbCursor);
    if (rtnVal == 0) {
      while (rtnVal == 0) {
/*
	rtnVal = krb5_ktfileint_read_entry(*ktidP, &keyEntryP);
*/
	rtnVal = krb5_kt_next_entry(*ktidP, &keyEntry, &krbCursor);
	if (rtnVal == 0) {
	  if ((krb5_principal_compare(keyEntry.principal,
				      krbPrincipal) == TRUE) &&
	      (keyEntry.vno > biggestVnoSeen)) {
	    *keyEntryP = keyEntry;
	  }
	}
      }	/* end while */
      if (rtnVal == KRB5_KT_END) {
	rtnVal = 0;
      }
      
      if (rtnVal == 0) {
/*
  	rtnVal = krb5_ktfileint_close(*ktidP);
*/
	rtnVal = krb5_kt_end_seq_get(*ktidP, &krbCursor);
      }
    }	/* end if we init'd the seq get successfully */
  }	/* end if we parsed the principal name successfully */
  
  return rtnVal;
}

krb5_error_code dfsauth_Keytab_ChangeKey(ktidP, prinNameP, kvno, passwdP)
     krb5_keytab *	ktidP;
     char *		prinNameP;
     int		kvno;
     char *		passwdP;
{
  krb5_error_code	rtnVal = 0;
  
  /* this routine uses the simple-minded implementation of change */
  rtnVal = dfsauth_Keytab_RemoveKey(ktidP, prinNameP, kvno);
  if (rtnVal == 0) {
    rtnVal = dfsauth_Keytab_AddKey(ktidP, prinNameP, kvno, passwdP);
  }
  
  return rtnVal;
}

EXPORT krb5_error_code dfsauth_Keytab_ShowNthKey(ktidP, index, prinNameP, kvnoP,
						 keyPrintStringP, printStrlen)
     krb5_keytab *	ktidP;
     int		index;
     char *		prinNameP;
     int *		kvnoP;
     char *		keyPrintStringP;
     int		printStrlen;
{
  krb5_error_code	krb5error = 0;
  krb5_kt_cursor	krbCursor;
  krb5_principal	krbPrincipal;
  krb5_keytab_entry	keyEntry;
  char *		copyPtr;
  u_int32		deskey[2];
  int			entryFound = 0;
  int			i = -1;
  
  krb5error = krb5_parse_name(prinNameP, &krbPrincipal);
  if (krb5error == 0) {
    /*
      krb5error = krb5_kt_start_seq_get(*ktidP, &krbCursor);
     */
    krb5error = local_krb5_ktfile_start_seq_get(*ktidP, &krbCursor);
    if (krb5error == 0) {
      while ((krb5error == 0) && (entryFound == 0)) {
	krb5error = krb5_kt_next_entry(*ktidP, &keyEntry, &krbCursor);
	if ((krb5error == 0) &&
	    (krb5_principal_compare(keyEntry.principal, krbPrincipal) == TRUE)) {
	  i++;
	  if (i == index) {
	    entryFound = 1;
	      
	    *kvnoP = keyEntry.vno;

	    /* copy out the key */
	    if (keyEntry.key.keytype == KEYTYPE_NULL) {
	      if ((keyPrintStringP!= (char *)NULL) && (printStrlen >= 1)) {
		keyPrintStringP[0] = '\0';
	      }
	      else {
		krb5error = KRB5_CONFIG_NOTENUFSPACE;
	      }
	    }
	    else if (keyEntry.key.keytype == KEYTYPE_DES) {
	      if ((keyPrintStringP != (char *)NULL) &&
		  (printStrlen >= ((2 * keyEntry.key.length) + 1))) {
		/*
		 * DES keys are 8 bytes long, so we can stuff them into two longs on
		 * most machines
		 */
		if (sizeof(deskey) == keyEntry.key.length) {
		  bcopy((char *)keyEntry.key.contents, (char *)deskey, sizeof(deskey));
		  sprintf(keyPrintStringP, "%02x%02x", deskey[0], deskey[1]);
		}
		else {
		  fprintf(stderr, "(unexpected length of long: %d)\n", sizeof(deskey));
		  copyPtr = keyPrintStringP;
		  for (i = 0; i < keyEntry.key.length; i++) {
		    /* print out some rendition of keyEntry.key.contents[i] */
		    sprintf(copyPtr, "%02x", keyEntry.key.contents[i]);
		    copyPtr += 2;
		  }
		}
	      }
	      else {
		krb5error = KRB5_CONFIG_NOTENUFSPACE;
	      }
	    }
	    else {
	      krb5error = KRB5_PROG_KEYTYPE_NOSUPP;
	    }
	  }	/* end if we have found the entry we want */
	}	/* end if we have found an entry for the requested principal */
      }	/* end while we haven't gotten to the entry we want yet */

      /* if we completed the loop without major error */
      if ((krb5error == 0) || (krb5error == KRB5_KT_END)) {
	(void)krb5_kt_end_seq_get(*ktidP, &krbCursor);

	if (krb5error == KRB5_KT_END) {
	  krb5error = KRB5_KT_NOTFOUND;
	}	/* end if we ran off the end of the table */
      }	/* end if we had a normal completion */
    }	/* end if we could start the sequential get */
  }	/* end if we could parse the name */
  
  return krb5error;
}

/*
 * this version does not try to convert to a printable string
 */
EXPORT krb5_error_code dfsauth_Keytab_ShowNthBinaryKey(ktidP, index, prinNameP, kvnoP,
						       keyBytesPP, numKeyBytesP)
     krb5_keytab *	ktidP;
     int		index;
     char *		prinNameP;
     int *		kvnoP;
     char **		keyBytesPP;
     int *		numKeyBytesP;
{
  krb5_error_code	krb5error = 0;
  krb5_kt_cursor	krbCursor;
  krb5_principal	krbPrincipal;
  krb5_keytab_entry	keyEntry;
  char *		copyPtr;
  int			entryFound = 0;
  int			i = -1;
  
  if (keyBytesPP != (char **)NULL) {
    /* init the output, in case of error */
    *keyBytesPP = (char *)NULL;
    *numKeyBytesP = 0;

    krb5error = krb5_parse_name(prinNameP, &krbPrincipal);
    if (krb5error == 0) {
      /*
	krb5error = krb5_kt_start_seq_get(*ktidP, &krbCursor);
       */
      krb5error = local_krb5_ktfile_start_seq_get(*ktidP, &krbCursor);
      if (krb5error == 0) {
	while ((krb5error == 0) && (entryFound == 0)) {
	  krb5error = krb5_kt_next_entry(*ktidP, &keyEntry, &krbCursor);
	  if ((krb5error == 0) &&
	      (krb5_principal_compare(keyEntry.principal, krbPrincipal) == TRUE)) {
	    i++;
	    if (i == index) {
	      entryFound = 1;
	      
	      *kvnoP = keyEntry.vno;
	      
	      /* copy out the key */
	      if (keyEntry.key.keytype == KEYTYPE_DES) {
		*keyBytesPP = osi_Alloc(keyEntry.key.length);
		if (*keyBytesPP != (char *)NULL) {
		  bcopy((char *)(keyEntry.key.contents), *keyBytesPP, keyEntry.key.length);
		  *numKeyBytesP = keyEntry.key.length;
		}
		else {
		  krb5error = ENOMEM;
		}
	      }
	      else if (keyEntry.key.keytype != KEYTYPE_NULL) {
		krb5error = KRB5_PROG_KEYTYPE_NOSUPP;
	      }

	    }	/* end if we have found the entry we want */
	  }	/* end if we have found an entry for the requested principal */
	}	/* end while we haven't gotten to the entry we want yet */
	
	/* if we completed the loop without major error */
	if ((krb5error == 0) || (krb5error == KRB5_KT_END)) {
	  (void)krb5_kt_end_seq_get(*ktidP, &krbCursor);
	  
	  if (krb5error == KRB5_KT_END) {
	    krb5error = KRB5_KT_NOTFOUND;
	  }	/* end if we ran off the end of the table */
	}	/* end if we had a normal completion */
      }	/* end if we could start the sequential get */
    }	/* end if we could parse the name */
  }
  else {
    krb5error = KRB5_CONFIG_NOTENUFSPACE;
  }
  
  return krb5error;
}

EXPORT long dfsauth_PassKey_Add(prinNameP, kvno, passwordP)
     char *	prinNameP;
     int	kvno;
     char *	passwordP;
{
  long			rtnVal = 0;
  char			principalName[BUFSIZ];
  char			passwordCopy[BUFSIZ];
  krb5_keytab		ktid;
  static char		routineName[] = "dfsauth_PassKey_Add";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s: entered: principal name: %s, kvno: %d",
	    routineName, strlen(prinNameP) ? prinNameP : "local", kvno));
  icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_6,
	     ICL_TYPE_STRING, (strlen(prinNameP) ? prinNameP : "local"),
	     ICL_TYPE_LONG, kvno);
  
  if (strlen(prinNameP)) {
    (void)strncpy(principalName, prinNameP, sizeof(principalName) - 1);
  }
  else {
    rtnVal = dfsauth_PrinName_GetName(prinNameP, (char *)NULL, principalName,
			      sizeof(principalName));
  }

  if (rtnVal == 0) {
    /* we have to make a copy of the password because the keytab function will bzero it */
    strncpy(passwordCopy, passwordP, sizeof(passwordCopy) - 1);
    
    rtnVal = dfsauth_InitKeytab((char *)NULL, &ktid);
    if (rtnVal == 0) {
      rtnVal = dfsauth_Keytab_AddKey(&ktid, prinNameP, kvno, passwordP);
      if (rtnVal == 0) {
#if defined(VALIDATE_USES_PASSWORDS)
	rtnVal = dfsauth_Passtab_AddPassword(defaultPasswordFilename, prinNameP,
				     kvno, passwordCopy);
	if (rtnVal != 0) {
#if !defined(KERNEL)
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		   ("%s: error adding password: %s", routineName, dfs_dceErrTxt(rtnVal)));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_7 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		   ("%s: error adding password: %lu", routineName, rtnVal));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_8 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
	}
#endif /* defined(VALIDATE_USES_PASSWORDS) */
      }
      else {
#if !defined(KERNEL)
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		 ("%s: error adding key: %s", routineName, dfs_dceErrTxt(rtnVal)));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_9 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		 ("%s: error adding key: %lu", routineName, rtnVal));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_10 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
      }
    }
    else {
#if !defined(KERNEL)
      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	       ("%s: error initializing key table: %s",
		routineName, dfs_dceErrTxt(rtnVal)));
      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_11 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	       ("%s: error initializing key table: %lu",
		routineName, rtnVal));
      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_12 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
    }
  }
  else {
#if !defined(KERNEL)
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	     ("%s: error getting principal name: %s", routineName, dfs_dceErrTxt(rtnVal)));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_13 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	     ("%s: error getting principal name: %lu", routineName, rtnVal));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_14 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
  }

#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s: exiting, returning %s", routineName,
	    (rtnVal == 0 ? "SUCCESS" : dfs_dceErrTxt(rtnVal))));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_15 , ICL_TYPE_STRING, (rtnVal==0?"SUCCESS":dfs_dceErrTxt(rtnVal)));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s: exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_16 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */

  return rtnVal;
}

EXPORT long dfsauth_PassKey_Remove(prinNameP, kvno)
     char *	prinNameP;
     int	kvno;
{
  long			rtnVal = 0;
  char			principalName[BUFSIZ];
  krb5_keytab		ktid;
  static char		routineName[] = "dfsauth_PassKey_Remove";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s: entered: principal name: %s, kvno: %d",
	    routineName, strlen(prinNameP) ? prinNameP : "local", kvno));
  icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_17,
	     ICL_TYPE_STRING, (strlen(prinNameP) ? prinNameP : "local"),
	     ICL_TYPE_LONG, kvno);
  
  if (strlen(prinNameP)) {
    (void)strncpy(principalName, prinNameP, sizeof(principalName) - 1);
  }
  else {
    rtnVal = dfsauth_PrinName_GetName(prinNameP, (char *)NULL, principalName,
				      sizeof(principalName));
  }

  if (rtnVal == 0) {
    rtnVal = dfsauth_InitKeytab((char *)NULL, &ktid);
    if (rtnVal == 0) {
      rtnVal = dfsauth_Keytab_RemoveKey(&ktid, prinNameP, kvno);
      if (rtnVal == 0) {
#if defined(VALIDATE_USES_PASSWORDS)
	rtnVal = dfsauth_Passtab_RemovePassword(defaultPasswordFilename, prinNameP, kvno);
	if (rtnVal != 0) {
#if !defined(KERNEL)
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		   ("%s: error removing password: %s",
		    routineName, dfs_dceErrTxt(rtnVal)));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_18 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		   ("%s: error removing password: %lu",
		    routineName, rtnVal));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_19 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
	}
#endif /* defined(VALIDATE_USES_PASSWORDS) */
      }
      else {
#if !defined(KERNEL)
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		 ("%s: error removing key: %s", routineName, dfs_dceErrTxt(rtnVal)));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_20 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		 ("%s: error removing key: %lu", routineName, rtnVal));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_21 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
      }
    }  
    else {
#if !defined(KERNEL)
      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	       ("%s: error initializing key table: %s",
		routineName, dfs_dceErrTxt(rtnVal)));
      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_22 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	       ("%s: error initializing key table: %lu",
		routineName, rtnVal));
      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_23 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
    }
  }
  else {
#if !defined(KERNEL)
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	     ("%s: error getting principal name: %s", routineName, dfs_dceErrTxt(rtnVal)));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_24 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	     ("%s: error getting principal name: %lu", routineName, rtnVal));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_25 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
  }
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s: exiting, returning %s", routineName,
	    (rtnVal == 0 ? "SUCCESS" : dfs_dceErrTxt(rtnVal))));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_26 , ICL_TYPE_STRING, (rtnVal==0?"SUCCESS":dfs_dceErrTxt(rtnVal)));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s: exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_27 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */

  return rtnVal;
}

EXPORT long dfsauth_PassKey_Change(prinNameP, kvno, passwordP)
     char *	prinNameP;
     int	kvno;
     char *	passwordP;
{
  long			rtnVal = 0;
  char			principalName[BUFSIZ];
  char			passwordCopy[BUFSIZ];
  krb5_keytab		ktid;
  static char		routineName[] = "dfsauth_PassKey_Change";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s: entered: principal name: %s, kvno: %d",
	    routineName, strlen(prinNameP) ? prinNameP : "local", kvno));
  icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_28,
	     ICL_TYPE_STRING, (strlen(prinNameP) ? prinNameP : "local"),
	     ICL_TYPE_LONG, kvno);
  
  if (strlen(prinNameP)) {
    (void)strncpy(principalName, prinNameP, sizeof(principalName) - 1);
  }
  else {
    rtnVal = dfsauth_PrinName_GetName(prinNameP, (char *)NULL, principalName,
			      sizeof(principalName));
  }

  if (rtnVal == 0) {
    /* we have to make a copy of the password because the keytab function will bzero it */
    strncpy(passwordCopy, passwordP, sizeof(passwordCopy) - 1);
    
    rtnVal = dfsauth_InitKeytab((char *)NULL, &ktid);
    if (rtnVal == 0) {
      rtnVal = dfsauth_Keytab_ChangeKey(&ktid, prinNameP, kvno, passwordP);
      if (rtnVal == 0) {
#if defined(VALIDATE_USES_PASSWORDS)
	dfsauth_Passtab_ChangePassword(defaultPasswordFilename, prinNameP, kvno,
				       passwordCopy);
	if (rtnVal != 0) {
#if !defined(KERNEL)
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		   ("%s: error changing password: %s",
		    routineName, dfs_dceErrTxt(rtnVal)));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_29 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		   ("%s: error changing password: %lu",
		    routineName, rtnVal));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_30 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
	}
#endif /* defined(VALIDATE_USES_PASSWORDS) */
      }
      else {
#if !defined(KERNEL)
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		 ("%s: error changing key: %s", routineName, dfs_dceErrTxt(rtnVal)));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_31 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
		 ("%s: error changing key: %lu", routineName, rtnVal));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_32 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
      }
    }
    else {
#if !defined(KERNEL)
      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	       ("%s: error initializing key table: %s",
		routineName, dfs_dceErrTxt(rtnVal)));
      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_33 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	       ("%s: error initializing key table: %lu",
		routineName, rtnVal));
      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_34 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
    }
  }
  else {
#if !defined(KERNEL)
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	     ("%s: error getting principal name: %s", routineName, dfs_dceErrTxt(rtnVal)));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_35 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	     ("%s: error getting principal name: %lu", routineName, rtnVal));
    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_36 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
  }
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s: exiting, returning %s", routineName,
	    (rtnVal == 0 ? "SUCCESS" : dfs_dceErrTxt(rtnVal))));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_37 , ICL_TYPE_STRING, (rtnVal==0?"SUCCESS":dfs_dceErrTxt(rtnVal)));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_KEYTAB,
	   ("%s: exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_KEYTAB_KEYTAB_38 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */

  return rtnVal;
}


