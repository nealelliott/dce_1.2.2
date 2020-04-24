/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_read.c,v $
 * Revision 1.1.10.1  1996/06/04  22:01:53  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:30 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Kerb5Beta5 KRB5_FCC_FVNO_3 merge.
 * 	[1996/01/03  19:14 UTC  psn  /main/DCE_1.2/1]
 *
 * 	CHFts14474: optimize krb credential cache
 * 	[1995/06/30  16:32 UTC  aha  /main/HPDCE02/1]
 *
 * 	CHFts14474: optimize krb cache;
 * 	add krb5_fcc_read_cred() function for sharing between
 * 	krb5_fcc_store and krb5_fcc_next_cred.
 * 	[1995/06/29  13:42 UTC  aha  /main/aha_mothra6/1]
 *
 * Revision 1.1.8.2  1996/02/18  00:09:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:10  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  17:47:19  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/30  16:32 UTC  aha
 * 	CHFts14474: optimize krb credential cache
 * 
 * 	HP revision /main/aha_mothra6/1  1995/06/29  13:42 UTC  aha
 * 	CHFts14474: optimize krb cache;
 * 	 add krb5_fcc_read_cred() function for sharing between
 * 	 krb5_fcc_store and krb5_fcc_next_cred.
 * 	[1995/12/08  16:57:59  root]
 * 
 * Revision 1.1.6.1  1994/08/26  16:01:41  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:53:57  sommerfeld]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_read.c,v $
 * $Author: arvind $
 *
 * Copyright 1990,1991,1992,1993,1994 by the Massachusetts Institute 
 * of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * This file contains the source code for reading variables from a
 * credentials cache.  These are not library-exported functions.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fcc_read_c[] =
"$Id: fcc_read.c,v 1.1.10.1 1996/06/04 22:01:53 arvind Exp $";
#endif /* !lint && !SABER */

#include <errno.h>
#include "fcc.h"

#define CHECK(ret) if (ret != KRB5_OK) goto errout;
     
/*
 * Effects:
 * Reads len bytes from the cache id, storing them in buf.
 *
 * Errors:
 * KRB5_CC_END - there were not len bytes available
 * system errors (read)
 */
krb5_error_code
krb5_fcc_read(id, buf, len)
   krb5_ccache id;
   krb5_pointer buf;
   int len;
{
     int ret;

     ret = read(((krb5_fcc_data *) id->data)->fd, (char *) buf, len);
     if (ret == -1)
	  return krb5_fcc_interpret(errno);
     else if (ret != len)
	  return KRB5_CC_END;
     else
	  return KRB5_OK;
}

/*
 * FOR ALL OF THE FOLLOWING FUNCTIONS:
 *
 * Requires:
 * id is open and set to read at the appropriate place in the file
 *
 * Effects:
 * Fills in the second argument with data of the appropriate type from
 * the file.  In some cases, the functions have to allocate space for
 * variable length fields; therefore, krb5_destroy_<type> must be
 * called for each filled in structure.
 *
 * Errors:
 * system errors (read errors)
 * KRB5_CC_NOMEM
 */

krb5_error_code
krb5_fcc_read_principal(id, princ)
   krb5_ccache id;
   krb5_principal *princ;
{
    krb5_fcc_data *data = (krb5_fcc_data *)id->data;
    krb5_error_code kret;
    register krb5_principal tmpprinc;
    krb5_int32 length, type, msize;
    int i;

    if (data->version == KRB5_FCC_FVNO_1) {
	type = KRB5_NT_UNKNOWN;
    } else {
        /* Read principal type */
        kret = krb5_fcc_read_int32(id, &type);
        if (kret != KRB5_OK)
	    return kret;
    }

    /* Read the number of components */
    kret = krb5_fcc_read_int32(id, &length);
    if (kret != KRB5_OK)
	return kret;

    /*
     * DCE includes the principal's realm in the count; the new format
     * does not.
     */
    if (data->version == KRB5_FCC_FVNO_1)
	length--;

#ifdef KRB5_BETA1
     /* Make tmpprinc able to hold length pointers to krb5_data structs
      * Add two extra for a null-terminated list and for the realm.
      */
     tmpprinc = (krb5_principal) calloc(length+2, sizeof(krb5_data *));
     if (tmpprinc == NULL)
	  return KRB5_CC_NOMEM;

     for (i=0; i <= length; i++) {
	  (tmpprinc)[i] = (krb5_data *) malloc(sizeof(krb5_data));
	  if ((tmpprinc)[i] == NULL) {
	      krb5_free_principal(tmpprinc);
	      return KRB5_CC_NOMEM;
          }	  
	  kret = krb5_fcc_read_data(id, (tmpprinc)[i]);
	  CHECK(kret);
     }
    (tmpprinc)[length+1] = NULL;
#else
    tmpprinc = (krb5_principal) malloc(sizeof(krb5_principal_data));
    if (tmpprinc == NULL)
	return KRB5_CC_NOMEM;
    if (length) {
            tmpprinc->data = 0;
            msize = length * sizeof(krb5_data);
            if ((msize & VALID_UINT_BITS) == msize)  /* Not overflow size_t */
                    tmpprinc->data = (krb5_data *) malloc((size_t) msize);
            if (tmpprinc->data == (krb5_data *) 0) {
		    free((char *)tmpprinc);
		    return KRB5_CC_NOMEM;
	    }
    } else
	    tmpprinc->data = 0;
    tmpprinc->length = length;
    tmpprinc->type = type;
    kret = krb5_fcc_read_data(id, krb5_princ_realm(tmpprinc));

    i = 0;
    CHECK(kret);

    for (i=0; i < length; i++) {
	kret = krb5_fcc_read_data(id, krb5_princ_component(tmpprinc, i));
	CHECK(kret);
    }
#endif
    *princ = tmpprinc;
    return KRB5_OK;

 errout:
    while(--i >= 0)
	free(krb5_princ_component(tmpprinc, i)->data);
#ifndef KRB5_BETA1
    free((char *)tmpprinc->data);
#endif
    free((char *)tmpprinc);
    return kret;
}

krb5_error_code
krb5_fcc_read_addrs(id, addrs)
   krb5_ccache id;
   krb5_address ***addrs;
{
     krb5_error_code kret;
     krb5_int32 length, msize;
     int i;

     *addrs = 0;

     /* Read the number of components */
     kret = krb5_fcc_read_int32(id, &length);
     CHECK(kret);

     /* Make *addrs able to hold length pointers to krb5_address structs
      * Add one extra for a null-terminated list
      */
     msize = length+1;
     if ((msize & VALID_UINT_BITS) != msize)    /* Overflow size_t??? */
          return KRB5_CC_NOMEM;
     *addrs = (krb5_address **) calloc((size_t) msize, sizeof(krb5_address *));
     if (*addrs == NULL)
	  return KRB5_CC_NOMEM;

     for (i=0; i < length; i++) {
	  (*addrs)[i] = (krb5_address *) malloc(sizeof(krb5_address));
	  if ((*addrs)[i] == NULL) {
	      krb5_free_addresses(*addrs);
	      return KRB5_CC_NOMEM;
	  }	  
	  kret = krb5_fcc_read_addr(id, (*addrs)[i]);
	  CHECK(kret);
     }

     return KRB5_OK;
 errout:
     if (*addrs)
	 krb5_free_addresses(*addrs);
     return kret;
}

krb5_error_code
krb5_fcc_read_keyblock(id, keyblock)
   krb5_ccache id;
   krb5_keyblock *keyblock;
{
     krb5_fcc_data *data = (krb5_fcc_data *)id->data;
     krb5_error_code kret;
     krb5_ui_2 ui2;
     krb5_int32 int32;

     keyblock->contents = 0;

     kret = krb5_fcc_read_ui_2(id, &ui2);
     keyblock->keytype = ui2;
     CHECK(kret);
     if ((data->version == KRB5_FCC_FVNO_1) ||
         (data->version == KRB5_FCC_FVNO_2))
#ifdef OSF_DCE
     {
	/*
	 * Do nothing. We don't support etypes yet.
	 */
     }
#else
         keyblock->etype = ETYPE_UNKNOWN;
#endif
     else {
             kret = krb5_fcc_read_ui_2(id, &ui2);
#ifdef OSF_DCE
       	     /*
              * Do nothing. We don't support etypes yet.
              */
#else
             keyblock->etype = ui2;
#endif
             CHECK(kret);
     }

     kret = krb5_fcc_read_int32(id, &int32);
     CHECK(kret);
     if ((int32 & VALID_INT_BITS) != int32)     /* Overflow size_t??? */
          return KRB5_CC_NOMEM;
     keyblock->length = (int) int32;
     if ( keyblock->length == 0 )
	     return KRB5_OK;
     keyblock->contents = (unsigned char *) malloc(keyblock->length*
						   sizeof(krb5_octet));
     if (keyblock->contents == NULL)
	  return KRB5_CC_NOMEM;
     
     kret = krb5_fcc_read(id, keyblock->contents, keyblock->length);
     if (kret)
         goto errout;

     return KRB5_OK;
 errout:
     if (keyblock->contents)
	 krb5_xfree(keyblock->contents);
     return kret;
}

krb5_error_code
krb5_fcc_read_data(id, data)
   krb5_ccache id;
   krb5_data *data;
{
     krb5_error_code kret;
     krb5_int32 len;

     data->data = 0;

     kret = krb5_fcc_read_int32(id, &len);
     CHECK(kret);
     if ((len & VALID_INT_BITS) != len)
        return KRB5_CC_NOMEM;
     data->length = (int) len;

     if (data->length == 0) {
	data->data = 0;
	return KRB5_OK;
     }

     data->data = (char *) malloc(data->length+1);
     if (data->data == NULL)
	  return KRB5_CC_NOMEM;

     kret = krb5_fcc_read(id, data->data, data->length);
     CHECK(kret);

     data->data[data->length] = 0; /* Null terminate, just in case.... */
     return KRB5_OK;
 errout:
     if (data->data)
	 krb5_xfree(data->data);
     return kret;
}

krb5_error_code
krb5_fcc_read_addr(id, addr)
   krb5_ccache id;
   krb5_address *addr;
{
     krb5_error_code kret;
     krb5_ui_2 ui2;
     krb5_int32 int32;

     addr->contents = 0;

     kret = krb5_fcc_read_ui_2(id, &ui2);
     CHECK(kret);
     addr->addrtype = ui2;

     kret = krb5_fcc_read_int32(id, &int32);
     CHECK(kret);
     if ((int32 & VALID_INT_BITS) != int32)     /* Overflow int??? */
          return KRB5_CC_NOMEM;
     addr->length = (int) int32;

     if (addr->length == 0)
	     return KRB5_OK;

     addr->contents = (krb5_octet *) malloc(addr->length);
     if (addr->contents == NULL)
	  return KRB5_CC_NOMEM;

     kret = krb5_fcc_read(id, addr->contents, addr->length);
     CHECK(kret);
 
     return KRB5_OK;
 errout:
     if (addr->contents)
	 krb5_xfree(addr->contents);
     return kret;
}

krb5_error_code
krb5_fcc_read_int32(id, i)
   krb5_ccache id;
   krb5_int32 *i;
{
    krb5_fcc_data *data = (krb5_fcc_data *)id->data;
    krb5_error_code retval;
    unsigned char buf[4];
    krb5_int32 val;

    if ((data->version == KRB5_FCC_FVNO_1) ||
        (data->version == KRB5_FCC_FVNO_2)) 
        return krb5_fcc_read(id, (krb5_pointer) i, sizeof(krb5_int32));
    else {
        retval = krb5_fcc_read(id, buf, 4);
        if (retval)
            return retval;
        val = buf[0];
        val = (val << 8) | buf[1];
        val = (val << 8) | buf[2];
        val = (val << 8) | buf[3];
        *i = val;
        return 0;
    }
}

krb5_error_code
krb5_fcc_read_ui_2(id, i)
   krb5_ccache id;
   krb5_ui_2 *i;
{
    krb5_fcc_data *data = (krb5_fcc_data *)id->data;
    krb5_error_code retval;
    unsigned char buf[2];
    
    if ((data->version == KRB5_FCC_FVNO_1) ||
        (data->version == KRB5_FCC_FVNO_2))
        return krb5_fcc_read(id, (krb5_pointer) i, sizeof(krb5_ui_2));
    else {
        retval = krb5_fcc_read(id, buf, 2);
        if (retval)
            return retval;
        *i = (buf[0] << 8) + buf[1];
        return 0;
    }
}

krb5_error_code
krb5_fcc_read_octet(id, i)
   krb5_ccache id;
   krb5_octet *i;
{
    return krb5_fcc_read(id, (krb5_pointer) i, 1);
}

krb5_error_code
krb5_fcc_read_times(id, t)
   krb5_ccache id;
   krb5_ticket_times *t;
{
    krb5_fcc_data *data = (krb5_fcc_data *)id->data;
    krb5_error_code retval;
    krb5_int32 i;
    
    if ((data->version == KRB5_FCC_FVNO_1) ||
        (data->version == KRB5_FCC_FVNO_2))
        return krb5_fcc_read(id, (krb5_pointer) t, sizeof(krb5_ticket_times));
    else {
        retval = krb5_fcc_read_int32(id, &i);
        CHECK(retval);
        t->authtime = i;
        
        retval = krb5_fcc_read_int32(id, &i);
        CHECK(retval);
        t->starttime = i;

        retval = krb5_fcc_read_int32(id, &i);
        CHECK(retval);
        t->endtime = i;

        retval = krb5_fcc_read_int32(id, &i);
        CHECK(retval);
        t->renew_till = i;
    }
    return 0;
errout:
    return retval;
}

krb5_error_code
krb5_fcc_read_authdata(id, a)
    krb5_ccache id;
    krb5_authdata ***a;
{
     krb5_error_code kret;
     krb5_int32 length, msize;
     int i;

     *a = 0;

     /* Read the number of components */
     kret = krb5_fcc_read_int32(id, &length);
     CHECK(kret);

     if (length == 0)
	 return KRB5_OK;

     /* Make *a able to hold length pointers to krb5_authdata structs
      * Add one extra for a null-terminated list
      */
     msize = length+1;
     if ((msize & VALID_UINT_BITS) != msize)    /* Overflow size_t??? */
          return KRB5_CC_NOMEM;
     *a = (krb5_authdata **) calloc((size_t) msize, sizeof(krb5_authdata *));
     if (*a == NULL)
	  return KRB5_CC_NOMEM;

     for (i=0; i < length; i++) {
	  (*a)[i] = (krb5_authdata *) malloc(sizeof(krb5_authdata));
	  if ((*a)[i] == NULL) {
	      krb5_free_authdata(*a);
	      return KRB5_CC_NOMEM;
	  }	  
	  kret = krb5_fcc_read_authdatum(id, (*a)[i]);
	  CHECK(kret);
     }

     return KRB5_OK;
 errout:
     if (*a)
	 krb5_free_authdata(*a);
     return kret;
}

krb5_error_code
krb5_fcc_read_authdatum(id, a)
    krb5_ccache id;
    krb5_authdata *a;
{
    krb5_error_code kret;
    krb5_int32 int32;
    krb5_ui_2 ui2;
    
    a->contents = NULL;

    kret = krb5_fcc_read_ui_2(id, &ui2);
    CHECK(kret);
    a->ad_type = (krb5_authdatatype)ui2;
    kret = krb5_fcc_read_int32(id, &int32);
    CHECK(kret);
    if ((int32 & VALID_INT_BITS) != int32)     /* Overflow int??? */
          return KRB5_CC_NOMEM;
    a->length = (int) int32;
    
    if (a->length == 0 )
	    return KRB5_OK;

    a->contents = (krb5_octet *) malloc(a->length);
    if (a->contents == NULL)
	return KRB5_CC_NOMEM;

    kret = krb5_fcc_read(id, a->contents, a->length);
    CHECK(kret);

    return KRB5_OK;
 errout:
     if (a->contents)
	 krb5_xfree(a->contents);
     return kret;
    
}

#ifdef OSF_DCE
/*
 * krb5_fcc_read_cred
 *
 * assumes: cache file is already positioned at beginning of
 * potential credential (may also be at end of file)
 *
 * returns: error code returned by credential field read
 * function.  Returns following first field with error, if
 * any.  Otherwise, returns KRB5_OK.
 *
 */
krb5_error_code
krb5_fcc_read_cred(id, creds)
     krb5_ccache     id;
     krb5_creds      *creds;
{
     krb5_error_code kret;
     krb5_int32 int32;
     krb5_octet octet;

     if (kret = krb5_fcc_read_principal(id, &creds->client))
         return(kret);
     if (kret = krb5_fcc_read_principal(id, &creds->server))
         return(kret);
     if (kret = krb5_fcc_read_keyblock(id, &creds->keyblock))
         return(kret);
     if (kret = krb5_fcc_read_times(id, &creds->times))
         return(kret);
     if (kret = krb5_fcc_read_octet(id, &octet))
         return(kret);
     creds->is_skey = octet;
     if (kret = krb5_fcc_read_int32(id, &int32))
         return(kret);
     creds->ticket_flags = int32;
     if (kret = krb5_fcc_read_addrs(id, &creds->addresses))
         return(kret);
     if (kret = krb5_fcc_read_authdata(id, &creds->authdata))
         return(kret);
     if (kret = krb5_fcc_read_data(id, &creds->ticket))
         return(kret);
     return (krb5_fcc_read_data(id, &creds->second_ticket));
}
#endif

