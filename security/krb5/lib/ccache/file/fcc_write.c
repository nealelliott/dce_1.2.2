/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_write.c,v $
 * Revision 1.1.8.1  1996/06/04  22:02:22  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:30 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Kerb5Beta5 KRB5_FCC_FVNO_3 merge.
 * 	[1996/01/03  19:14 UTC  psn  /main/DCE_1.2/1]
 *
 * 	CHFts14474: optimize krb credential cache
 * 	[1995/06/30  16:32 UTC  aha  /main/HPDCE02/1]
 *
 * 	CHFts14474: optimize krb cred cache;
 * 	add krb5_fcc_sizeof_cred() function
 * 	[1995/06/14  15:59 UTC  aha  /main/aha_mothra6/1]
 *
 * Revision 1.1.6.2  1996/02/18  00:09:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:30  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:48:02  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/30  16:32 UTC  aha
 * 	CHFts14474: optimize krb credential cache
 * 
 * 	HP revision /main/aha_mothra6/1  1995/06/14  15:59 UTC  aha
 * 	CHFts14474: optimize krb cred cache;
 * 	 add krb5_fcc_sizeof_cred() function
 * 	[1995/12/08  16:58:06  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:56  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:55:15  sommerfeld]
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_write.c,v $
 * $Author: arvind $
 *
 * Copyright 1990,1991,1992,1993,1994 by the Massachusetts Institute of Technology.
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
 * This file contains the source code for krb5_fcc_write_<type>.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_rcsid_fcc_write_c[] =
 "$Id: fcc_write.c,v 1.1.8.1 1996/06/04 22:02:22 arvind Exp $";
#endif /* !lint && !SABER */

#include <errno.h>
#include "fcc.h"

#define CHECK(ret) if (ret != KRB5_OK) return ret;

/*
 * Requires:
 * id is open
 *
 * Effects:
 * Writes len bytes from buf into the file cred cache id.
 *
 * Errors:
 * system errors
 */
krb5_error_code
krb5_fcc_write(id, buf, len)
   krb5_ccache id;
   krb5_pointer buf;
   int len;
{
     int ret;

     ret = write(((krb5_fcc_data *)id->data)->fd, (char *) buf, len);
     if (ret < 0)
	  return krb5_fcc_interpret(errno);
     if (ret != len)
         return KRB5_CC_WRITE;
     return KRB5_OK;
}

/*
 * FOR ALL OF THE FOLLOWING FUNCTIONS:
 * 
 * Requires:
 * ((krb5_fcc_data *) id->data)->fd is open and at the right position.
 * 
 * Effects:
 * Stores an encoded version of the second argument in the
 * cache file.
 *
 * Errors:
 * system errors
 */

krb5_error_code
krb5_fcc_store_principal(id, princ)
   krb5_ccache id;
   krb5_principal princ;
{
    krb5_fcc_data *data = (krb5_fcc_data *)id->data;
    krb5_error_code ret;
    krb5_int32 i, length, tmp, type;

    type = krb5_princ_type(princ);
    tmp = length = krb5_princ_size(princ);

    if (data->version == KRB5_FCC_FVNO_1) {
	/*
	 * DCE-compatible format means that the length count
	 * includes the realm.  (It also doesn't include the
	 * principal type information.)
	 */
	tmp++;
    } else {
	ret = krb5_fcc_store_int32(id, type);
	CHECK(ret);
    }
    
    ret = krb5_fcc_store_int32(id, tmp);
    CHECK(ret);

    ret = krb5_fcc_store_data(id, krb5_princ_realm(princ));
    CHECK(ret);

    for (i=0; i < length; i++) {
	ret = krb5_fcc_store_data(id, krb5_princ_component(princ, i));
	CHECK(ret);
    }

    return KRB5_OK;
}

krb5_error_code
krb5_fcc_store_addrs(id, addrs)
   krb5_ccache id;
   krb5_address ** addrs;
{
     krb5_error_code ret;
     krb5_address **temp;
     krb5_int32 i, length = 0;

     if (addrs == NULL) 
	 length = 0;
     else 
     {
	 /* Count the number of components */
	 temp = addrs;
	 while (*temp++)
	     length += 1;
     }
     ret = krb5_fcc_store_int32(id, length);
     CHECK(ret);
     for (i=0; i < length; i++) {
	  ret = krb5_fcc_store_addr(id, addrs[i]);
	  CHECK(ret);
     }

     return KRB5_OK;
}

krb5_error_code
krb5_fcc_store_keyblock(id, keyblock)
   krb5_ccache id;
   krb5_keyblock *keyblock;
{
     krb5_fcc_data *data = (krb5_fcc_data *)id->data;
     krb5_error_code ret;

     ret = krb5_fcc_store_ui_2(id, keyblock->keytype);
     CHECK(ret);
     if ((data->version != KRB5_FCC_FVNO_1) &&
	 (data->version != KRB5_FCC_FVNO_2)) {
#ifdef OSF_DCE
	 unsigned int etype = 0x1FF; /* ETYPE_UNKNOWN */
	 /*
	  * DCE doesn't support etypes yet. Make a guess at an etype
	  * value based on the keytype.
	  */
	 if (valid_keytype(keyblock->keytype)) {
	     etype = krb5_keytype_array[keyblock->keytype]->system->proto_enctype;
             if (!valid_etype(etype))
                 etype = 0x1FF; /* ETYPE_UNKNOWN */
	 }
	 ret = krb5_fcc_store_ui_2(id, etype);
#else
	 ret = krb5_fcc_store_ui_2(id, keyblock->etype);
#endif
	 CHECK(ret);
     }
     ret = krb5_fcc_store_int32(id, keyblock->length);
     CHECK(ret);
     return krb5_fcc_write(id, (char *) keyblock->contents, keyblock->length);
}

krb5_error_code
krb5_fcc_store_addr(id, addr)
   krb5_ccache id;
   krb5_address *addr;
{
     krb5_error_code ret;

     ret = krb5_fcc_store_ui_2(id, addr->addrtype);
     CHECK(ret);
     ret = krb5_fcc_store_int32(id, addr->length);
     CHECK(ret);
     return krb5_fcc_write(id, (char *) addr->contents, addr->length);
}


krb5_error_code
krb5_fcc_store_data(id, data)
   krb5_ccache id;
   krb5_data *data;
{
     krb5_error_code ret;

     ret = krb5_fcc_store_int32(id, data->length);
     CHECK(ret);
     return krb5_fcc_write(id, data->data, data->length);
}

krb5_error_code
krb5_fcc_store_int32(id, i)
   krb5_ccache id;
   krb5_int32 i;
{
    krb5_fcc_data *data = (krb5_fcc_data *)id->data;
    unsigned char buf[4];

    if ((data->version == KRB5_FCC_FVNO_1) ||
	(data->version == KRB5_FCC_FVNO_2)) 
	return krb5_fcc_write(id, (char *) &i, sizeof(krb5_int32));
    else {
	buf[3] = (unsigned char) (i & 0xFF);
	i >>= 8;
	buf[2] = (unsigned char) (i & 0xFF);
	i >>= 8;
	buf[1] = (unsigned char) (i & 0xFF);
	i >>= 8;
	buf[0] = (unsigned char) (i & 0xFF);
	
	return krb5_fcc_write(id, buf, 4);
    }
}

krb5_error_code
krb5_fcc_store_ui_2(id, i)
    krb5_ccache id;
    krb5_int32 i;
{
    krb5_fcc_data *data = (krb5_fcc_data *)id->data;
    krb5_ui_2 ibuf;
    unsigned char buf[2];
    
    if ((data->version == KRB5_FCC_FVNO_1) ||
	(data->version == KRB5_FCC_FVNO_2)) {
	ibuf = (krb5_ui_2) i;
	return krb5_fcc_write(id, (char *) &ibuf, sizeof(krb5_ui_2));
    } else {
	buf[1] = (unsigned char) (i & 0xFF);
	i >>= 8;
	buf[0] = (unsigned char) (i & 0xFF);
	
	return krb5_fcc_write(id, buf, 2);
    }
}
   
krb5_error_code
krb5_fcc_store_octet(id, i)
    krb5_ccache id;
    krb5_int32 i;
{
    krb5_octet ibuf;

    ibuf = (krb5_octet) i;
    return krb5_fcc_write(id, (char *) &ibuf, 1);
}

krb5_error_code
krb5_fcc_store_times(id, t)
   krb5_ccache id;
   krb5_ticket_times *t;
{
    krb5_fcc_data *data = (krb5_fcc_data *)id->data;
    krb5_error_code retval;

    if ((data->version == KRB5_FCC_FVNO_1) ||
	(data->version == KRB5_FCC_FVNO_2))
	return krb5_fcc_write(id, (char *) t, sizeof(krb5_ticket_times));
    else {
	retval = krb5_fcc_store_int32(id, t->authtime);
	CHECK(retval);
	retval = krb5_fcc_store_int32(id, t->starttime);
	CHECK(retval);
	retval = krb5_fcc_store_int32(id, t->endtime);
	CHECK(retval);
	retval = krb5_fcc_store_int32(id, t->renew_till);
	CHECK(retval);
	return 0;
    }
}

krb5_error_code
krb5_fcc_store_authdata(id, a)
    krb5_ccache id;
    krb5_authdata **a;
{
    krb5_error_code ret;
    krb5_authdata **temp;
    krb5_int32 i, length=0;

    if (a != NULL) {
	for (temp=a; *temp; temp++)
	    length++;
    }

    ret = krb5_fcc_store_int32(id, length);
    CHECK(ret);
    for (i=0; i<length; i++) {
	ret = krb5_fcc_store_authdatum (id, a[i]);
	CHECK(ret);
    }
    return KRB5_OK;
}

krb5_error_code
krb5_fcc_store_authdatum (id, a)
    krb5_ccache id;
    krb5_authdata *a;
{
    krb5_error_code ret;
    ret = krb5_fcc_store_ui_2(id, a->ad_type);
    CHECK(ret);
    ret = krb5_fcc_store_int32(id, a->length);
    CHECK(ret);
    return krb5_fcc_write(id, (krb5_pointer) a->contents, a->length);
}

#ifdef OSF_DCE

/*  Abstraction continues; provide access to the size of a credential
 *  in terms of the number of bytes it would occupy on disk.
 */

#define krb5_fcc_sizeof_addr(ADDR_PTR)                                 \
  ((int) sizeof(krb5_ui_2)	/* address type (...ui_2)    */        \
   + (int) sizeof(int)		/* address length (stored as int) */   \
   + ((ADDR_PTR)->length)*((int) sizeof(krb5_octet))) /* # Octets to bytes */

static int
krb5_fcc_sizeof_addrs(addrs)
   krb5_address **addrs;
{
  int size;
  krb5_error_code ret;
  krb5_address **temp;
  krb5_int32 i, length = 0;

  /* Count the number of components */
  if (addrs == NULL)
    length = 0;
  else {
    temp = addrs;
    while (*temp++)
      length += 1;
  }

  size = (int) sizeof(krb5_int32); /* Would be the component count 'length' */
  for (i=0; i < length; i++)
    size += krb5_fcc_sizeof_addr(addrs[i]);

  return size;
}

#define krb5_fcc_sizeof_authdatum(AUTH_D_PTR)                   \
  ((int) sizeof(krb5_ui_2)	/* auth data type (ui_2)    */  \
   + (int) sizeof(krb5_int32)	/* auth data length (int32) */  \
   + (AUTH_D_PTR)->length)	/* # chars in data          */

static int
krb5_fcc_sizeof_authdata(a)
   krb5_authdata **a;
{
  int size;
  krb5_authdata **temp;
  krb5_int32 i, length=0;

  if (a != NULL) {
    for (temp=a; *temp; temp++)
      length++;
  }

  size = (int) sizeof(krb5_int32);	/* Number of components (length) */
  for (i=0; i<length; i++)
    size += krb5_fcc_sizeof_authdatum(a[i]);

  return size;
}

#define krb5_fcc_sizeof_data(KRB5_DATUM_PTR) \
  ((int) sizeof(krb5_int32)                  \
   + (KRB5_DATUM_PTR)->length)	/* Count of bytes + bytes */

static int
krb5_fcc_sizeof_principal(princ)
   krb5_principal princ;
{
  krb5_principal temp;		/* To walk the princ component list */
  krb5_int32 size, comps = 0, i;

  /* Count the number of components */
  temp = princ;
  while (*temp++)
    comps ++;

  size = (int) sizeof(krb5_int32);	/* for number of components */
  for (i = 0; i < comps; i++)
    size += krb5_fcc_sizeof_data(princ[i]);

  return size;
}

#define krb5_fcc_sizeof_keyblock(KEYB_PTR)                              \
  ((int) sizeof(krb5_keytype)	/* The keyblock's type   */             \
   + (int) sizeof(int)		/* The keyblock's length */             \
   + (KEYB_PTR)->length*((int) sizeof(krb5_octet))) /* # Octets to bytes */

/* Exported routine; the others may be exported in the future should interest
 * arise for them.
 */

int
krb5_fcc_sizeof_cred(cred)
   krb5_creds *cred;
{
  return !cred ? 0 : (krb5_fcc_sizeof_principal(cred->client)
		      + krb5_fcc_sizeof_principal(cred->server)
		      + krb5_fcc_sizeof_keyblock(&cred->keyblock)
		      + (int) sizeof(krb5_ticket_times) /* times        */
		      + (int) sizeof(krb5_boolean)      /* is_skey      */
		      + (int) sizeof(krb5_flags)        /* ticket_flags */
		      + krb5_fcc_sizeof_addrs(cred->addresses)
		      + krb5_fcc_sizeof_authdata(cred->authdata)
		      + krb5_fcc_sizeof_data(&cred->ticket)
		      + krb5_fcc_sizeof_data(&cred->second_ticket));
}
#endif
