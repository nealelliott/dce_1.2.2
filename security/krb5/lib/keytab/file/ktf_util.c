/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ktf_util.c,v $
 * Revision 1.1.13.2  1996/02/18  00:12:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:46  marty]
 *
 * Revision 1.1.13.1  1995/12/08  17:55:47  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/06/05  21:50 UTC  jrr
 * 	Merge memory leak fix work (CHFts15246).
 * 
 * 	HP revision /main/sommerfeld_CHFts13948/1  1995/05/26  17:40 UTC  sommerfeld
 * 	     Fix memory leak due to incorrect #ifdefing.
 * 	     (this eliminates the success-path leak, but not any of the
 * 	     failure-path leaks, which are probably not severe).
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/31  14:48 UTC  mullan_s
 * 	Merge CHFts15049
 * 
 * 	HP revision /main/mullan_mothra_bin_compat3/2  1995/05/30  18:15 UTC  mullan_s
 * 	Remove int * cast from sec_login_pvt_get_krb5_kt_vno
 * 
 * 	HP revision /main/mullan_mothra_bin_compat3/1  1995/04/14  20:55 UTC  mullan_s
 * 	Binary compatibility: retrieve krb5_kt_vno from dce_cf.db.
 * 	[1995/12/08  16:59:22  root]
 * 
 * Revision 1.1.9.2  1994/09/16  21:51:34  sommerfeld
 * 	[OT12036] Bug in ktfileint_delete_entry: doesn't byte swap length
 * 	enough places.
 * 	[1994/09/16  21:50:10  sommerfeld]
 * 
 * Revision 1.1.9.1  1994/08/26  16:02:15  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:57:34  sommerfeld]
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/keytab/file/ktf_util.c,v $
 * $Author: marty $
 *
 * Copyright (c) Hewlett-Packard Company 1991
 * Released to the Massachusetts Institute of Technology for inclusion
 * in the Kerberos source code distribution.
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * This function contains utilities for the file based implementation of 
 * the keytab.  There are no public functions in this file.
 *
 * This file is the only one that has knowledge of the format of a
 * keytab file.
 *
 * The format is as follows:
 * 
 * <file format vno>
 * <record length>
 * principal timestamp vno key
 * <record length>
 * principal timestamp vno key
 * ....
 *
 * A length field (sizeof(krb5_int32)) exists between entries.  When this
 * length is positive it indicates an active entry, when negative a hole.
 * The length indicates the size of the block in the file (this may be 
 * larger than the size of the next record, since we are using a first
 * fit algorithm for re-using holes and the first fit may be larger than
 * the entry we are writing).  Another (compatible) implementation could
 * break up holes when allocating them to smaller entries to minimize 
 * wasted space.  (Such an implementation should also coalesce adjacent
 * holes to reduce fragmentation).  This implementation does neither.
 *
 * There are no separators between fields of an entry.  
 * A principal is a length-encoded array of length-encoded strings.  The
 * length is a krb5_int16 in each case.  The specific format, then, is 
 * multiple entries concatinated with no separators.  An entry has this 
 * exact format:
 *
 * sizeof(krb5_int16) bytes for number of components in the principal; 
 * then, each component listed in ordser.
 * For each component, sizeof(krb5_int16) bytes for the number of bytes
 * in the component, followed by the component.
 * sizeof(krb5_int32) for the principal type (for KEYTAB V2 and higher)
 * sizeof(krb5_timestamp) bytes for the timestamp
 * sizeof(krb5_kvno) bytes for the key version number
 * sizeof(krb5_keytype) bytes for the keytype
 * sizeof(krb5_int32) bytes for the key length, followed by the key
 *
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_ktf_util_c[] =
"$Id: ktf_util.c,v 1.1.13.2 1996/02/18 00:12:54 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/libos.h>
#include <krb5/los-proto.h>
#include <krb5/osconf.h>
#include <netinet/in.h>
#include <stdio.h>

#include "ktfile.h"

#ifdef OSF_DCE
#include <sec_login_pvt.h>
#endif /* OSF_DCE */

#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#endif

typedef krb5_int16  krb5_kt_vno;

krb5_kt_vno krb5_kt_default_vno = KRB5_KT_DEFAULT_VNO;

#define xfwrite(a, b, c, d) fwrite((char *)a, b, c, d)
#define xfread(a, b, c, d) fread((char *)a, b, c, d)

#ifdef ANSI_STDIO
static char *fopen_mode_rbplus= "rb+";
static char *fopen_mode_rb = "rb";
#else
static char *fopen_mode_rbplus= "r+";
static char *fopen_mode_rb = "r";
#endif

extern int errno;

static krb5_error_code ktf_convert(code)
    krb5_error_code code;
{
    switch(code) 
    {
    case ENOENT:
	return KRB5_KT_NOFILE;
    default:
	return code;
    }
}   

static krb5_error_code
krb5_ktfileint_open(id, mode)
krb5_keytab id;
int mode;
{
    krb5_error_code kerror;
    krb5_kt_vno kt_vno;
    int writevno = 0;

    KTFILEP(id) = fopen(KTFILENAME(id),
			(mode == KRB5_LOCKMODE_EXCLUSIVE) ?
			  fopen_mode_rbplus : fopen_mode_rb);
    if (!KTFILEP(id)) {
	if ((mode == KRB5_LOCKMODE_EXCLUSIVE) && (errno == ENOENT)) {
	    /* try making it first time around */
            krb5_create_secure_file(KTFILENAME(id));
	    KTFILEP(id) = fopen(KTFILENAME(id), fopen_mode_rbplus);
	    if (!KTFILEP(id))
		return ktf_convert(errno);
	    writevno = 1;
	} else				/* some other error */
	    return ktf_convert(errno);
    }
    if (kerror = krb5_lock_file(KTFILEP(id), KTFILENAME(id),
				mode)) {
	(void) fclose(KTFILEP(id));
	KTFILEP(id) = 0;
	return kerror;
    }
    /* assume ANSI or BSD-style stdio */
    setbuf(KTFILEP(id), NULL);

    /* get the vno and verify it */
    if (writevno) {
#ifdef OSF_DCE
	/*
         * Retrieve krb5 keytab file format vno from dce_cf.db
	 */
	sec_login_pvt_get_krb5_kt_vno(&krb5_kt_default_vno);
#endif /* OSF_DCE */
	kt_vno = htons(krb5_kt_default_vno);
	KTVERSION(id) = krb5_kt_default_vno;
	if (!xfwrite(&kt_vno, sizeof(kt_vno), 1, KTFILEP(id))) {
	    kerror = ktf_convert(errno);
	    (void) krb5_unlock_file(KTFILEP(id), KTFILENAME(id));
	    (void) fclose(KTFILEP(id));
	    return kerror;
	}
    } else {
	/* gotta verify it instead... */
	if (!xfread(&kt_vno, sizeof(kt_vno), 1, KTFILEP(id))) {
	    kerror = ktf_convert(errno);
	    (void) krb5_unlock_file(KTFILEP(id), KTFILENAME(id));
	    (void) fclose(KTFILEP(id));
	    return kerror;
	}
	kt_vno = KTVERSION(id) = ntohs(kt_vno);
	if ((kt_vno != KRB5_KT_VNO) &&
	    (kt_vno != KRB5_KT_VNO_1)) {
	    (void) krb5_unlock_file(KTFILEP(id), KTFILENAME(id));
	    (void) fclose(KTFILEP(id));
	    return KRB5_KEYTAB_BADVNO;
	}
    }
    return 0;
}

krb5_error_code
krb5_ktfileint_openr(id)
krb5_keytab id;
{
    return krb5_ktfileint_open(id, KRB5_LOCKMODE_SHARED);
}

krb5_error_code
krb5_ktfileint_openw(id)
krb5_keytab id;
{
    return krb5_ktfileint_open(id, KRB5_LOCKMODE_EXCLUSIVE);
}

krb5_error_code
krb5_ktfileint_close(id)
krb5_keytab id;
{
    krb5_error_code kerror;

    if (!KTFILEP(id))
	return 0;
    kerror = krb5_unlock_file(KTFILEP(id), KTFILENAME(id));
    (void) fclose(KTFILEP(id));
    KTFILEP(id) = 0;
    return kerror;
}

krb5_error_code
krb5_ktfileint_delete_entry(id, delete_point)
krb5_keytab id;
krb5_int32 delete_point;
{
    krb5_int32  size;
    krb5_int32  len;
    char        iobuf[BUFSIZ];

    if (fseek(KTFILEP(id), delete_point, SEEK_SET)) {
        return errno;
    }
    if (!xfread(&size, sizeof(size), 1, KTFILEP(id))) {
        return KRB5_KT_END;
    }
    if (KTVERSION(id) != KRB5_KT_VNO_1)
	size = ntohl(size);

    if (size > 0) {
        krb5_int32 minus_size = -size;
	if (KTVERSION(id) != KRB5_KT_VNO_1)
	    minus_size = htonl(minus_size);

        if (fseek(KTFILEP(id), delete_point, SEEK_SET)) {
            return errno;
        }

        if (!xfwrite(&minus_size, sizeof(minus_size), 1, KTFILEP(id))) {
            return KRB5_KT_IOERR;
        }

        if (size < BUFSIZ) {
            len = size;
        } else {
            len = BUFSIZ;
        }

        memset(iobuf, 0, len);
        while (size > 0) {
            xfwrite(iobuf, 1, len, KTFILEP(id));
            size -= len;
            if (size < len) {
                len = size;
            }
        }

        return krb5_sync_disk_file(KTFILEP(id));
    }

    return 0;
}

krb5_error_code
krb5_ktfileint_internal_read_entry(id, entrypp, delete_point)
krb5_keytab id;
krb5_keytab_entry **entrypp;
krb5_int32 *delete_point;
{
    register krb5_keytab_entry *ret_entry;
    krb5_int16 count;
    krb5_int16 princ_size;
    register int i;
    krb5_int32 size;
    krb5_int32 start_pos;
    krb5_error_code error;
    char	*tmpdata;
    krb5_data	*princ;

    *entrypp = NULL;
    
    if (!(ret_entry = (krb5_keytab_entry *)calloc(1, sizeof(*ret_entry))))
	return ENOMEM;
    
    /* fseek to synchronise buffered I/O on the key table. */

    if (fseek(KTFILEP(id), 0L, SEEK_CUR) < 0)
    {
        return errno;
    }

    do {
        *delete_point = ftell(KTFILEP(id));
        if (!xfread(&size, sizeof(size), 1, KTFILEP(id))) {
            return KRB5_KT_END;
        }
	if (KTVERSION(id) != KRB5_KT_VNO_1)
		size = ntohl(size);

        if (size < 0) {
            if (fseek(KTFILEP(id), -size, SEEK_CUR)) {
                return errno;
            }
        }
    } while (size < 0);

    if (size == 0) {
        return KRB5_KT_END;
    }

    start_pos = ftell(KTFILEP(id));

    /* deal with guts of parsing... */

    /* first, int16 with #princ components */
    if (!xfread(&count, sizeof(count), 1, KTFILEP(id)))
	return KRB5_KT_END;
    if (KTVERSION(id) == KRB5_KT_VNO_1) {
	    count -= 1;		/* V1 includes the realm in the count */
    } else {
	    count = ntohs(count);
    }
    if (!count || (count < 0))
	return KRB5_KT_END;

#ifdef KRB5_BETA1
    count++;
    if (!(ret_entry->principal = (krb5_data **)calloc(count+1, sizeof(krb5_data *))))
	return ENOMEM;
    for (i = 0; i < count; i++) {
	if (!xfread(&princ_size, sizeof(princ_size), 1, KTFILEP(id)))
	    return KRB5_KT_END;
	if (KTVERSION(id) != KRB5_KT_VNO_1)
	    princ_size = ntohs(princ_size);
	if (!princ_size || (princ_size < 0))
	    return KRB5_KT_END;

	if (!(ret_entry->principal[i] = (krb5_data *)malloc(sizeof(krb5_data))))
	    return ENOMEM;
	ret_entry->principal[i]->length = princ_size;
	ret_entry->principal[i]->data = malloc(princ_size);
	if (!ret_entry->principal[i]->data)
	    return ENOMEM;
	if (!xfread(ret_entry->principal[i]->data, sizeof(char), princ_size,
		    KTFILEP(id)))
	    return KRB5_KT_END;
    }
    /* read in the principal type and throw it away, if we can get it */
    if (KTVERSION(id) != KRB5_KT_VNO_1) {
	krb5_int32 dummy;
	if (!xfread(&dummy, sizeof(dummy), 1, KTFILEP(id))) {
	    error = KRB5_KT_END;
	    goto fail;
	}
    }
#else
    ret_entry->principal = (krb5_principal)malloc(sizeof(krb5_principal_data));
    if (!ret_entry->principal)
        return ENOMEM;
    
    ret_entry->principal->length = count;
    ret_entry->principal->data = (krb5_data *)calloc(count, sizeof(krb5_data));
    if (!ret_entry->principal->data) {
	free(ret_entry->principal);    
	return ENOMEM;
    }

    /* Now, get the realm data */
    if (!xfread(&princ_size, sizeof(princ_size), 1, KTFILEP(id))) {
	    error = KRB5_KT_END;
	    goto fail;
    }
    if (KTVERSION(id) != KRB5_KT_VNO_1)
	    princ_size = ntohs(princ_size);
    if (!princ_size || (princ_size < 0)) {
	    error = KRB5_KT_END;
	    goto fail;
    }
    krb5_princ_set_realm_length(ret_entry->principal, princ_size);
    tmpdata = malloc(princ_size+1);
    if (!tmpdata) {
	    error = ENOMEM;
	    goto fail;
    }
    if (fread(tmpdata, 1, princ_size, KTFILEP(id)) != princ_size) {
	    free(tmpdata);
	    error = KRB5_KT_END;
	    goto fail;
    }
    tmpdata[princ_size] = 0;	/* Some things might be expecting null */
				/* termination...  ``Be conservative in */
				/* what you send out'' */
    krb5_princ_set_realm_data(ret_entry->principal, tmpdata);
    
    for (i = 0; i < count; i++) {
	princ = krb5_princ_component(ret_entry->principal, i);
	if (!xfread(&princ_size, sizeof(princ_size), 1, KTFILEP(id))) {
	    error = KRB5_KT_END;
	    goto fail;
        }
	if (KTVERSION(id) != KRB5_KT_VNO_1)
	    princ_size = ntohs(princ_size);
	if (!princ_size || (princ_size < 0)) {
	    error = KRB5_KT_END;
	    goto fail;
        }

	princ->length = princ_size;
	princ->data = malloc(princ_size+1);
	if (!princ->data) {
	    error = ENOMEM;
	    goto fail;
        }
	if (!xfread(princ->data, sizeof(char), princ_size, KTFILEP(id))) {
	    error = KRB5_KT_END;
	    goto fail;
        }
	princ->data[princ_size] = 0; /* Null terminate */
    }

    /* read in the principal type, if we can get it */
    if (KTVERSION(id) != KRB5_KT_VNO_1) {
	    if (!xfread(&ret_entry->principal->type,
			sizeof(ret_entry->principal->type), 1, KTFILEP(id))) {
		    error = KRB5_KT_END;
		    goto fail;
	    }
	    ret_entry->principal->type = ntohl(ret_entry->principal->type);
    }
#endif
    /* read in the timestamp */
    if (!xfread(&ret_entry->timestamp, sizeof(ret_entry->timestamp), 1, KTFILEP(id))) {
	error = KRB5_KT_END;
	goto fail;
    }
    if (KTVERSION(id) != KRB5_KT_VNO_1)
	ret_entry->timestamp = ntohl(ret_entry->timestamp);
    
    /* read in the version number */
    if (!xfread(&ret_entry->vno, sizeof(ret_entry->vno), 1, KTFILEP(id))) {
	error = KRB5_KT_END;
	goto fail;
    }
    
    /* key type */
    if (!xfread(&ret_entry->key.keytype, sizeof(ret_entry->key.keytype), 1,
		KTFILEP(id))) {
	error = KRB5_KT_END;
	goto fail;
    }
    if (KTVERSION(id) != KRB5_KT_VNO_1)
	ret_entry->key.keytype = ntohs(ret_entry->key.keytype);
    
    /* key contents */
    if (!xfread(&count, sizeof(count), 1, KTFILEP(id))) {
	error = KRB5_KT_END;
	goto fail;
    }
    if (KTVERSION(id) != KRB5_KT_VNO_1)
	count = ntohs(count);
    if (!count || (count < 0)) {
	error = KRB5_KT_END;
	goto fail;
    }
    ret_entry->key.length = count;
    
    ret_entry->key.contents = (krb5_octet *)malloc(count);
    if (!ret_entry->key.contents) {
	error = ENOMEM;
	goto fail;
    }		
    if (!xfread(ret_entry->key.contents, sizeof(krb5_octet), count,
		KTFILEP(id))) {
	error = KRB5_KT_END;
	goto fail;
    }

    *entrypp = ret_entry;

    /*
     * Reposition file pointer to the next inter-record length field.
     */
    fseek(KTFILEP(id), start_pos + size, SEEK_SET);
    return 0;
fail:
    
#ifndef KRB5_BETA1
    for (i = 0; i < ret_entry->principal->length; i++) {
	    princ = krb5_princ_component(ret_entry->principal, i);
	    if (princ->data)
		    free(princ->data);
    }
    free(ret_entry->principal->data);
    free(ret_entry->principal);
#else
    for (i = 0; krb5_princ_component(ret_entry->principal, i) != NULL; i++) 
    {
	princ = krb5_princ_component(ret_entry->principal, i);
	if (princ->data)
	    free(princ->data);
	free(princ);
    }
    free(ret_entry->principal);
#endif
    return error;
}

krb5_error_code
krb5_ktfileint_read_entry(id, entrypp)
krb5_keytab id;
krb5_keytab_entry **entrypp;
{
    krb5_int32 delete_point;
    krb5_error_code status;
    
    *entrypp = NULL;
    
    status = krb5_ktfileint_internal_read_entry(id, entrypp, &delete_point);
    if (status != 0) 
    {
	krb5_kt_free_entry (*entrypp);
	free (*entrypp);
	*entrypp = NULL;
    }
    return status;
}

krb5_error_code
krb5_ktfileint_write_entry(id, entry)
krb5_keytab id;
krb5_keytab_entry *entry;
{
    krb5_data *princ;
    krb5_data **princp;
    krb5_int16 count, size, size1, keytype;
    krb5_error_code retval = 0;
    krb5_timestamp timestamp;
    krb5_int32	princ_type;
    krb5_int32  size_needed;
    krb5_int32  commit_point;
    int		i;
    char iobuf[BUFSIZ];

    retval = krb5_ktfileint_size_entry(entry, &size_needed);
    if (retval)
        return retval;
    retval = krb5_ktfileint_find_slot(id, &size_needed, &commit_point);
    if (retval)
        return retval;

    setbuf(KTFILEP(id), iobuf);

    /* fseek to synchronise buffered I/O on the key table. */

    if (fseek(KTFILEP(id), 0L, SEEK_CUR) < 0)
    {
        return errno;
    }

#ifdef KRB5_BETA1
    /* count up principal components */
    for (count = 0, princp = entry->principal; *princp; princp++, count++);

    if (KTVERSION(id) == KRB5_KT_VNO_1)
	;
    else
	count = htons(count-1);
    
    if (!xfwrite(&count, sizeof(count), 1, KTFILEP(id))) {
    abend:
	setbuf(KTFILEP(id), 0);
	return KRB5_KT_IOERR;
    }

    for (princp = entry->principal; *princp; princp++) {
	size1 = size = (*princp)->length;
	if (KTVERSION(id) != KRB5_KT_VNO_1)
	    size1 = ntohs(size1);

	if (!xfwrite(&size1, sizeof(size1), 1, KTFILEP(id))) {
	    goto abend;
	}
	if (!xfwrite((*princp)->data, sizeof(char), size, KTFILEP(id))) {
	    goto abend;
	}
    }

    /*
     * Write out the principal type
     */
    if (KTVERSION(id) != KRB5_KT_VNO_1) {
	    princ_type = htonl(KRB5_NT_UNKNOWN);
	    if (!xfwrite(&princ_type, sizeof(princ_type), 1, KTFILEP(id))) {
		    goto abend;
	    }
    }

#else
    if (KTVERSION(id) == KRB5_KT_VNO_1) {
	    count = entry->principal->length + 1;
    } else {
	    count = htons(entry->principal->length);
    }
    
    if (!xfwrite(&count, sizeof(count), 1, KTFILEP(id))) {
    abend:
	setbuf(KTFILEP(id), 0);
	return KRB5_KT_IOERR;
    }
    size = krb5_princ_realm(entry->principal)->length;
    if (KTVERSION(id) != KRB5_KT_VNO_1)
	    size = htons(size);
    if (!xfwrite(&size, sizeof(size), 1, KTFILEP(id))) {
	    goto abend;
    }
    if (!xfwrite(krb5_princ_realm(entry->principal)->data, sizeof(char),
		 krb5_princ_realm(entry->principal)->length, KTFILEP(id))) {
	    goto abend;
    }

    count = entry->principal->length;
    for (i = 0; i < count; i++) {
	princ = krb5_princ_component(entry->principal, i);
	size = princ->length;
	if (KTVERSION(id) != KRB5_KT_VNO_1)
		size = htons(size);
	if (!xfwrite(&size, sizeof(size), 1, KTFILEP(id))) {
	    goto abend;
	}
	if (!xfwrite(princ->data, sizeof(char), princ->length, KTFILEP(id))) {
	    goto abend;
	}
    }

    /*
     * Write out the principal type
     */
    if (KTVERSION(id) != KRB5_KT_VNO_1) {
	    princ_type = htonl(krb5_princ_type(entry->principal));
	    if (!xfwrite(&princ_type, sizeof(princ_type), 1, KTFILEP(id))) {
		    goto abend;
	    }
    }
#endif
    /*
     * Fill in the time of day the entry was written to the keytab.
     */
    if (krb5_timeofday(&entry->timestamp)) {
        entry->timestamp = 0;
    }
    if (KTVERSION(id) == KRB5_KT_VNO_1)
	    timestamp = entry->timestamp;
    else
	    timestamp = htonl(entry->timestamp);
    if (!xfwrite(&timestamp, sizeof(timestamp), 1, KTFILEP(id))) {
	goto abend;
    }
    
    /* key version number */
    if (!xfwrite(&entry->vno, sizeof(entry->vno), 1, KTFILEP(id))) {
	goto abend;
    }
    /* key type */
    if (KTVERSION(id) == KRB5_KT_VNO_1)
	    keytype = entry->key.keytype;
    else
	    keytype = htons(entry->key.keytype);
    if (!xfwrite(&keytype, sizeof(keytype), 1, KTFILEP(id))) {
	goto abend;
    }
    /* key length */
    if (KTVERSION(id) == KRB5_KT_VNO_1)
	    size = entry->key.length;
    else
	    size = htons(entry->key.length);
    if (!xfwrite(&size, sizeof(size), 1, KTFILEP(id))) {
	goto abend;
    }
    if (!xfwrite(entry->key.contents, sizeof(krb5_octet),
		 entry->key.length, KTFILEP(id))) {
	memset(iobuf, 0, sizeof(iobuf));
	setbuf(KTFILEP(id), 0);
	return KRB5_KT_IOERR;
    }	

    retval = krb5_sync_disk_file(KTFILEP(id));
    (void) memset(iobuf, 0, sizeof(iobuf));
    setbuf(KTFILEP(id), 0);

    if (retval) {
        return retval;
    }

    if (fseek(KTFILEP(id), commit_point, SEEK_SET)) {
        return errno;
    }
    if (KTVERSION(id) != KRB5_KT_VNO_1)
	    size_needed = htonl(size_needed);
    if (!xfwrite(&size_needed, sizeof(size_needed), 1, KTFILEP(id))) {
        goto abend;
    }
    retval = krb5_sync_disk_file(KTFILEP(id));

    return retval;
}

/*
 * Determine the size needed for a file entry for the given
 * keytab entry.
 */
krb5_error_code
krb5_ktfileint_size_entry(entry, size_needed)
krb5_keytab_entry *entry;
krb5_int32 *size_needed;
{
    krb5_int16 count, size;
    krb5_int32 total_size, i;
    krb5_error_code retval = 0;

#ifdef KRB5_BETA1
    krb5_data **princp;
    
    /* count up principal components */
    for (count = 0, princp = entry->principal; *princp; princp++, count++);
    count--;
#else
    count = entry->principal->length;
#endif

    total_size = sizeof(count);
    total_size += krb5_princ_realm(entry->principal)->length + (sizeof(size));
    
    for (i = 0; i < count; i++) {
	    total_size += krb5_princ_component(entry->principal,i)->length
		    + (sizeof(size));
    }

#ifdef KRB5_BETA1
    total_size += sizeof(krb5_int32);
#else
    total_size += sizeof(entry->principal->type);
#endif
    total_size += sizeof(entry->timestamp);
    total_size += sizeof(entry->vno);
    total_size += sizeof(entry->key.keytype);
    total_size += sizeof(size) + entry->key.length;

    *size_needed = total_size;
    return retval;
}

/*
 * Find and reserve a slot in the file for an entry of the needed size.
 * The commit point will be set to the position in the file where the
 * the length (sizeof(krb5_int32) bytes) of this node should be written
 * when commiting the write.  The file position left as a result of this
 * call is the position where the actual data should be written.
 *
 * The size_needed argument may be adjusted if we find a hole that is
 * larger than the size needed.  (Recall that size_needed will be used
 * to commit the write, but that this field must indicate the size of the
 * block in the file rather than the size of the actual entry)  
 */
krb5_error_code
krb5_ktfileint_find_slot(id, size_needed, commit_point)
krb5_keytab id;
krb5_int32 *size_needed;
krb5_int32 *commit_point;
{
    krb5_int32      size;
    krb5_int32      remainder;
    krb5_int32      zero_point;
    krb5_kt_vno     kt_vno;
    krb5_boolean    found = FALSE;
    char            iobuf[BUFSIZ];

    /*
     * Skip over file version number
     */
    if (fseek(KTFILEP(id), 0, SEEK_SET)) {
        return errno;
    }
    if (!xfread(&kt_vno, sizeof(kt_vno), 1, KTFILEP(id))) {
        return KRB5_KT_IOERR;
    }

    while (!found) {
        *commit_point = ftell(KTFILEP(id));
        if (!xfread(&size, sizeof(size), 1, KTFILEP(id))) {
            /*
             * Hit the end of file, reserve this slot.
             */
            setbuf(KTFILEP(id), 0);
            size = 0;

            /* fseek to synchronise buffered I/O on the key table. */

            if (fseek(KTFILEP(id), 0L, SEEK_CUR) < 0)
            {
                return errno;
            }
	    
#ifdef notdef
	    /* We don't have to do this because htonl(0) == 0 */
	    if (KTVERSION(id) != KRB5_KT_VNO_1)
		    size = htonl(size);
#endif
	    
            if (!xfwrite(&size, sizeof(size), 1, KTFILEP(id))) {
            abend:
                return KRB5_KT_IOERR;
            }
            found = TRUE;
        }

	if (KTVERSION(id) != KRB5_KT_VNO_1)
		size = ntohl(size);

        if (size > 0) {
            if (fseek(KTFILEP(id), size, SEEK_CUR)) {
                return errno;
            }
        } else if (!found) {
            size = -size;
            if (size >= *size_needed) {
                *size_needed = size;
                found = TRUE;	
            } else if (size > 0) {
                /*
                 * The current hole is not large enough, so skip it
                 */
                if (fseek(KTFILEP(id), size, SEEK_CUR)) {
                    return errno;
                }
            } else {

                 /* fseek to synchronise buffered I/O on the key table. */

                 if (fseek(KTFILEP(id), 0L, SEEK_CUR) < 0)
                 {
                     return errno;
                 }

                /*
                 * Found the end of the file (marked by a 0 length buffer)
                 * Make sure we zero any trailing data.
                 */
                zero_point = ftell(KTFILEP(id));
                setbuf(KTFILEP(id), iobuf);
                while (size = xfread(iobuf, 1, sizeof(iobuf), KTFILEP(id))) {
                    if (size != sizeof(iobuf)) {
                        remainder = size % sizeof(krb5_int32);
                        if (remainder) {
                            size += sizeof(krb5_int32) - remainder;
                        }
                    }

                    if (fseek(KTFILEP(id), 0L, SEEK_CUR) < 0)
                    {
                        return errno;
                    }

                    memset(iobuf, 0, size);
                    xfwrite(iobuf, 1, size, KTFILEP(id));
                    if (feof(KTFILEP(id))) {
                        break;
                    }

                    if (fseek(KTFILEP(id), 0L, SEEK_CUR) < 0)
                    {
                        return errno;
                    }

                }
                setbuf(KTFILEP(id), 0);
                if (fseek(KTFILEP(id), zero_point, SEEK_SET)) {
                    return errno;
                }
            }
        }
    }

    return 0;
}

