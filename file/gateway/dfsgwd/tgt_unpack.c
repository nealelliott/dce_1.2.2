/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: tgt_unpack.c,v $
 * Revision 1.1.4.1  1996/10/02  17:50:19  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:29  damon]
 *
 * Revision 1.1.2.1  1994/07/26  19:13:29  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:58:02  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:05:42  mckeen]
 * 
 * $EndLog$
 */
/*
 * tgt_unpack.c - Unpack TGTs
 *
 * We have been passed a TGT (actually a krb5_creds) from dlog to 
 * dfsgwd for eventual installation in the local credential cache.  The only 
 * problem is that the TGT is full of pointers to data which are context-dependent.
 * So we pack/unpack the TGT using the following rules:
 *
 * Typedefs (krb5_octet, etc) are stored using the defined storage class.
 *
 * The TGT packet itself consists of several packlets that start and end at long offsets.
 * Fields within the packlets are aligned as necessary (and noted). 
 *
 * Packlets:
 *
 * krb5_data structures are packed:
 *		unsigned long length
 *		<length> char of data
 *
 * krb5_principal structures are vectors of krb5_data structures:
 *		unsigned long # elements N
 *		N krb5_data packlets
 *
 * krb5_keyblock structures are packed:
 *		krb5_keytype keytype
 *		<round to int boundary>
 *		int length <of contents in bytes> 
 *		<length> of krb5_octet contents
 * 
 * krb5_ticket_times strucures are packed:
 *		krb5_timestamp authtime
 *		krb5_timestamp starttime
 *		krb5_timestamp endtime
 *		krb5_timestamp renew_till
 *
 * krb5_address structures are packed
 *		krb5_addrtype addrtype
 *		<round to int boundary>
 *		int length <of contents in bytes>
 *		<length> of krb5_octet contents
 *
 * Vectors of krb5_address structures are packed:
 *		unsigned long # elements N
 *		N krb5_address packlets
 *
 * krb5_authdata structures are packed:
 *		krb5_authdatatype ad_type
 *		<round to int boundary>
 *		int length <of contents in bytes>
 *		<length> of krb5_octet contents
 *
 * Vectors of krb5_authdata structures are packed:
 *		unsigned long # elements N
 *		N krb5_authdata packlets
 *		
 * The TGT packet consists of:
 *	2 krb5_principal packlets (client and server)
 *	1 krb5_keyblock packlet (keyblock)
 *	1 krb5_ticket_times packlet (times)
 *	1 krb5_boolean (is_skey)
 *	1 krb5_flags (ticket_flags)
 *	A vector of krb5_address packlets (addresses)
 *	2 krb5_data packets (ticket and second_ticket)
 *	A vector of krb5_authdata packlets (authdata)
 *
 * Obviously (and unfortunately), this means that these routines know
 * intimatley about the cred and is going to have to be ported when 
 * we go to new revs.
 *
 * Note that the "other end" of this module is in tgt_pack.c.  They are
 * seperate because they are compiled using different include files (to
 * make up for differences between kerberos versions).
 */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>

#include <dcedfs/stds.h>
RCSID("$Header: /u0/rcs_trees/dce/rcs/file/gateway/dfsgwd/tgt_unpack.c,v 1.1.4.1 1996/10/02 17:50:19 damon Exp $")

/*
 * Local macros and constants.
 */
#define factor	(sizeof(unsigned long) - 1)
#define roundup(n) ((((int)(n)) + factor) & ~factor)

/*
 * unpack_keyblock - Unpack krb5_keyblock structure.
 * 
 * Inputs:
 *	data		: Pointer to destination keyblock.
 * InOuts:
 *	source		: Pointer to pointer of packed data, modified upon return.  
 *			  Storage must be freed later. 
 * Return value:
 *	0 on success, -1 otherwise.
 */
static int unpack_keyblock
#ifdef __STDC__
	(int **source, krb5_keyblock *data)
#else 
	(source, data)
	int **source;
	krb5_keyblock *data;
#endif /* __STDC__ */
{
	data->keytype = *(krb5_keytype *)*source;
	*source = (int *)((int)*source + roundup(sizeof(krb5_keytype)));
	data->length = **source;
	++*source;
	if (data->length) {
		if ((data->contents = (krb5_octet *)malloc(data->length)) == NULL) {
			return(-1);
		}
		memcpy(data->contents, *source, data->length);
		*source = (int *)roundup(((int)*source + data->length));
	} else {
		data->contents = NULL;
	}
	return(0);
}

/*
 * unpack_ticket_times - Unpack krb5_ticket_times structure.
 *
 * Unlike the other unpack routines, this routine unpacks the ticket time
 * packlet and plunks it in a buffer.
 *
 * Inputs:
 *	times		: Pointer to krb5_ticket_times structure.
 * InOuts:
 *	source		: Pointer to pointer of packed data, modified upon return.
 * Return value:
 *	void.
 */
static void unpack_ticket_times
#ifdef __STDC__
	(int **source, krb5_ticket_times *times)
#else 
	(source, times)
	int **source;
	krb5_ticket_times *times;
#endif /* __STDC__ */
{
	times->authtime = *(krb5_timestamp *)(*source); 
	*source += sizeof(times->authtime) >> 2;
	times->starttime = *(krb5_timestamp *)(*source); 
	*source += sizeof(times->starttime) >> 2;
	times->endtime = *(krb5_timestamp *)(*source); 
	*source += sizeof(times->endtime) >> 2;
	times->renew_till = *(krb5_timestamp *)(*source); 
	*source += sizeof(times->renew_till) >> 2;
	*source = (int *)roundup(*source);
}

/*
 * unpack_address - Unpack krb5_address structure.
 *
 * This routine returns a pointer to an allocated krb5_address structure.  It should
 * be freed when it is no longer of use. 
 *
 * InOuts:
 *	source		: Pointer to pointer of packed data, modified upon return.
 * Return value:
 *	Pointer to unpacked krb5_address structure (cast to unsigned long), or NULL on error.
 */
static unsigned long *unpack_address
#ifdef __STDC__
	(int **source)
#else 
	(source)
	int **source;
#endif /* __STDC__ */
{
	krb5_address *data;	/* Pointer to new structure, returned */

	if ((data = (krb5_address *)malloc(sizeof(krb5_address))) == NULL) {
		return(NULL);
	}
	data->addrtype = *(krb5_addrtype *)*source;
	*source = (int *)((int)*source + roundup(sizeof(krb5_addrtype)));
	data->length = **source;
	++*source;
	if (data->length) {
		if ((data->contents = (krb5_octet *)malloc(data->length)) == NULL) {
			return(NULL);
		}
		memcpy(data->contents, *source, data->length);
		*source = (int *)roundup(((int)*source + data->length));
	}
	return((unsigned long *)data);
}

/*
 * unpack_authdata - Unpack krb5_authdata structure.
 *
 * This routine returns a pointer to an allocated krb5_authdata structure.  It should
 * be freed when it is no longer of use. 
 *
 * InOuts:
 *	source		: Pointer to pointer of packed data, modified upon return.
 * Return value:
 *	Pointer to unpacked krb5_authdata structure (cast to unsigned long), or NULL on error.
 */
static unsigned long *unpack_authdata
#ifdef __STDC__
	(int **source)
#else 
	(source)
	int **source;
#endif /* __STDC__ */
{
	krb5_authdata *data;	/* Pointer to new structure, returned */

	if ((data = (krb5_authdata *)malloc(sizeof(krb5_authdata))) == NULL) {
		return(NULL);
	}
	data->ad_type = *(krb5_authdatatype *)*source;
	*source = (int *)((int)*source + roundup(sizeof(krb5_authdatatype)));
	data->length = **source;
	++*source;
	if (data->length) {
		if ((data->contents = (krb5_octet *)malloc(data->length)) == NULL) {
			return(NULL);
		}
		memcpy(data->contents, *source, data->length);
		*source = (int *)roundup(((int)*source + data->length));
	} else {
		data->contents = NULL;
	}
	return((unsigned long *)data);
}


/*
 * unpack_data - Unpack krb5_data structure.
 *
 * This routine returns a pointer to an allocated krb5_data structure.	It should
 * be freed when it is no longer of use. 
 *
 * InOuts:
 *	source		: Pointer to pointer of packed data, modified upon return.
 * Return value:
 *	Pointer to unpacked krb5_data structure (cast to unsigned long), or NULL on error.
 */
static unsigned long *unpack_data
#ifdef __STDC__
	(int **source)
#else 
	(source)
	int **source;
#endif /* __STDC__ */
{
	krb5_data *data;	/* Pointer to new structure, returned */

	if ((data = (krb5_data *)malloc(sizeof(krb5_data))) == NULL) {
		return(NULL);
	}
	data->length = **source;
	++*source;
	if (data->length) {
		if ((data->data = (char *)malloc(data->length)) == NULL) {
			return(NULL);
		}
		memcpy(data->data, *source, data->length);
		*source = (int *)roundup(((int)*source + data->length));
	} else {
		data->data = NULL;
	}
	return((unsigned long *)data);
}


/*
 * unpack_vector - Unpack a vector.
 *
 * This routine returns an assembled, unpacked vector in allocated storage.
 * It must be freed when it is no longer of use.
 *
 * Inputs:
 *	pack_func	: Pointer to element unpack function.
 * InOuts:
 *	source		: Packed data pointer.
 *	vector		: Pointer to destination vector, voided.
 * Return value:
 *	0 on success, -1 otherwise.
 */
static int unpack_vector
#ifdef __STDC__
	(int **source, unsigned long *(*unpack_func)(), void **vector)
#else 
	(source, unpack_func, vector)
	int **source;
	unsigned long *(*unpack_func)();
	void **vector;
#endif /* __STDC__ */
{
	int n;			/* Number of elements to unpack */
	unsigned long **v;	/* New element */
	
	n = **source;
	++*source;
	if (!n) {
		*vector = NULL;
		return(0);
	}
	/*
	 * malloc (n + 1) : terminate vector.
	 */
	if ((v = *vector = (void *)malloc((n + 1) * sizeof(void *))) == NULL) {
		return(-1);
	}
	while (n--) {
		if ((*v = (*unpack_func)(source)) == NULL) {
			return(-1);
		}
		v++;
	}
	*v = NULL;
	return(0);
}


/*
 * tgt_unpack - Unpack a TGT.
 *
 * Unpacking is a little easier than packing.
 *
 * Inputs:
 *	packet		: Pointer to packetized TGT.
 *	cred		: Pointer to krb5_cred structure
 *	swap		: True if byte swapping should take place
 * Return value:
 *	0 on success, -1 otherwise.
 */
int tgt_unpack
#ifdef __STDC__
	(char *packet, krb5_creds *tgt, int swap)
#else
	(packet, tgt, swap)
	char *packet;
	krb5_cred *tgt;
	int swap;
#endif /* __STDC__ */
{
	krb5_data *d;		/* Pointer to unpacked krb5 structures */

	if (unpack_vector((int **)&packet, unpack_data, (void **)&tgt->client) < 0) {
		return(-1);
	}
	if (unpack_vector((int **)&packet, unpack_data, (void **)&tgt->server) < 0) {
		return(-1);
	}
	if (unpack_keyblock((int **)&packet, &tgt->keyblock) < 0) {
		return(-1);
	}
	unpack_ticket_times((int **)&packet, &tgt->times);
	tgt->is_skey = *(krb5_boolean *)packet;
	packet = (char *)roundup(((int)packet + sizeof(tgt->is_skey)));
	tgt->ticket_flags = *(krb5_flags *)packet;
	packet = (char *)roundup(((int)packet + sizeof(tgt->ticket_flags)));
	if (unpack_vector((int **)&packet, unpack_address, (void **)&tgt->addresses) < 0) {
		return(-1);
	}
	if ((d = (krb5_data *)unpack_data((int **)&packet)) == NULL) {
		return(-1);		
	} else {
		memcpy(&tgt->ticket, d, sizeof(krb5_data));
		free(d);
	}
	if ((d = (krb5_data *)unpack_data((int **)&packet)) == NULL) {
		return(-1);		
	} else {
		memcpy(&tgt->second_ticket, d, sizeof(krb5_data));
		free(d);
	}
	if (unpack_vector((int **)&packet, unpack_authdata, (void **)&tgt->authdata) < 0) { 
		return(-1);
	}
	return(0);
} 
