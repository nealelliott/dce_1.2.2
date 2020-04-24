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
 * $Log: tgt_pack.c,v $
 * Revision 1.1.4.1  1996/10/02  17:49:43  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:09  damon]
 *
 * Revision 1.1.2.2  1994/08/08  18:54:32  mckeen
 * 	Check for NULL before referencing vector ptr
 * 	[1994/08/08  17:46:45  mckeen]
 * 
 * Revision 1.1.2.1  1994/07/26  19:12:54  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:39  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:08:08  mckeen]
 * 
 * $EndLog$
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfs_login/tgt_pack.c,v 1.1.4.1 1996/10/02 17:49:43 damon Exp $ */

/*
 * tgt_pack.c - Pack TGTs
 *
 * We are passing over the TGT (actually a krb5_creds) from dlog
 * to dfsgwd for eventual installation in the local credential
 * cache.  The only problem is that the TGT is full of pointers to
 * data which are context-dependent.  So we pack/unpack the TGT
 * using the following rules: Typedefs (krb5_octet, etc) are
 * stored using the defined storage class.  The TGT packet itself
 * consists of several packlets that start and end at long
 * offsets.  Fields within the packlets are aligned as necessary
 * (and noted).
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
 *              <round to int boundary>
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
 *              <round to int boundary>
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
 * Note that the "other end" of this module is in tgt_unpack.c.  They are
 * seperate because they are compiled using different include files (to
 * make up for differences between kerberos versions). */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>

/*
 * Local macros and constants.
 */
#define factor	(sizeof(unsigned long) - 1)
#define roundup(n) ((((int)(n)) + factor) & ~factor)
#define sum_keyblock(d) (roundup(roundup(sizeof((d)->keytype)) + (d)->length + sizeof((d)->length)))
#define sum_ticket_times(d) (roundup(sizeof((d)->authtime) +  sizeof((d)->starttime) + sizeof((d)->endtime) +  sizeof((d)->renew_till)))	

/*
 * pack_keyblock - Pack krb5_keyblock structure.
 *
 * Inputs:
 *	data		: krb5_keyblock structure pointer.
 * InOuts:
 *	buf		: Pointer to pointer of buffer, modifed during routine.
 * Return value:
 *	void.
 */
static void pack_keyblock
#ifdef __STDC__
	(krb5_keyblock *data, char **buf)
#else 
	(data, buf)
	krb5_keyblock *data;
	char **buf;
#endif /* __STDC__ */
{
	*(krb5_keytype *)(*buf) = data->keytype;
	*buf += roundup(sizeof(data->keytype));
	*(int *)(*buf) = data->length;
	*buf += sizeof(data->length);
	memcpy(*buf, data->contents, data->length);
	*buf += data->length;
	*buf = (char *)roundup(*buf);
}

/*
 * pack_ticket_times - Pack krb5_ticket_times structure.
 *
 * Inputs:
 *	data		: krb5_ticket_times structure pointer.
 * InOuts:
 *	buf		: Pointer to pointer of buffer, modifed during routine.
 * Return value:
 *	void.
 */
static void pack_ticket_times
#ifdef __STDC__
	(krb5_ticket_times *data, char **buf)
#else 
	(data, buf)
	krb5_ticket_times *data;
	char **buf;
#endif /* __STDC__ */
{
	*(krb5_timestamp *)(*buf) = data->authtime;
	*buf += sizeof(data->authtime);
	*(krb5_timestamp *)(*buf) = data->starttime;
	*buf += sizeof(data->starttime);
	*(krb5_timestamp *)(*buf) = data->endtime;
	*buf += sizeof(data->endtime);
	*(krb5_timestamp *)(*buf) = data->renew_till;
	*buf += sizeof(data->renew_till);
	*buf = (char *)roundup(*buf);
}

/*
 * sum_address - Return size of packed krb5_address structrure.
 *
 * Inputs:
 *	data		: krb5_address structure pointer.
 * Return value:
 *	Size of packetized krb5_address structure.
 */
static int sum_address
#ifdef __STDC__
	(krb5_address *data)
#else 
	(data)
	krb5_address *data;
#endif /* __STDC__ */
{
	return(roundup(roundup(sizeof(data->addrtype)) + data->length + sizeof(data->length)));
}

/*
 * pack_address - Pack krb5_address structure.
 *
 * Inputs:
 *	data		: krb5_address structure pointer.
 * InOuts:
 *	buf		: Pointer to pointer of buffer, modifed during routine.
 * Return value:
 *	void.
 */
static void pack_address
#ifdef __STDC__
	(krb5_address *data, char **buf)
#else 
	(data, buf)
	krb5_address *data;
	char **buf;
#endif /* __STDC__ */
{
	*(krb5_addrtype *)(*buf) = data->addrtype;
	*buf += roundup(sizeof(data->addrtype));
	*(int *)(*buf) = data->length;
	*buf += sizeof(data->length);
	memcpy(*buf, data->contents, data->length);
	*buf += data->length;
	*buf = (char *)roundup(*buf);
}

/*
 * sum_authdata - Return size of packed krb5_authdata structrure.
 *
 * Inputs:
 *	data		: krb5_authdata structure pointer.
 * Return value:
 *	Size of packetized krb5_authdata structure.
 */
static int sum_authdata
#ifdef __STDC__
	(krb5_authdata *data)
#else 
	(data)
	krb5_authdata *data;
#endif /* __STDC__ */
{
	return(roundup(roundup(sizeof(data->ad_type)) + data->length + sizeof(data->length)));
}

/*
 * pack_authdata - Pack krb5_authdata structure.
 *
 * Inputs:
 *	data		: krb5_authdata structure pointer.
 * InOuts:
 *	buf		: Pointer to pointer of buffer, modifed during routine.
 * Return value:
 *	void.
 */
static void pack_authdata
#ifdef __STDC__
	(krb5_authdata *data, char **buf)
#else 
	(data, buf)
	krb5_authdata *data;
	char **buf;
#endif /* __STDC__ */
{
	*(krb5_authdatatype *)(*buf) = data->ad_type;
	*buf += roundup(sizeof(data->ad_type));
	*(int *)(*buf) = data->length;
	*buf += sizeof(data->length);
	memcpy(*buf, data->contents, data->length);
	*buf += data->length;
	*buf = (char *)roundup(*buf);
}

/*
 * sum_data - Return size of packed krb5_data structrure.
 *
 * Inputs:
 *	data		: krb5_data structure pointer.
 * Return value:
 *	Size of packetized krb5_data structure.
 */
static int sum_data
#ifdef __STDC__
	(krb5_data *data)
#else 
	(data)
	krb5_data *data;
#endif /* __STDC__ */
{
	return(roundup(data->length + sizeof(data->length)));
}

/*
 * pack_data - Pack krb5_data structure.
 *
 * Inputs:
 *	data		: krb5_data structure pointer.
 * InOuts:
 *	buf		: Pointer to pointer of buffer, modifed during routine.
 * Return value:
 *	void.
 */
static void pack_data
#ifdef __STDC__
	(krb5_data *data, char **buf)
#else 
	(data, buf)
	krb5_data *data;
	char **buf;
#endif /* __STDC__ */
{
	*(unsigned long *)(*buf) = data->length;
	*buf += sizeof(data->length);
	memcpy(*buf, data->data, data->length);
	*buf += data->length;
	*buf = (char *)roundup(*buf);
}

/*
 * sum_vector - Compute size of vector.
 *
 * Returns size of vector (when packletized).
 *
 * Inputs:
 *	vector		: Pointer to vector, voided.
 *	size_func	: Pointer to element size function.
 * Return value:
 *	Size of packed vector.
 */
static int sum_vector
#ifdef __STDC__
	(void **vector, int (*size_func)())
#else
	(vector, size_func)
	void **vector;
	int (*size_func)();
#endif /* __STDC__ */
{
	int total = sizeof(unsigned long);	/* Total storage used (first longword is # elements) */

	if (vector != NULL)
	  while (*vector) {
	    total += (*size_func)(*vector);
	    ++vector;
	  }
	return(total); 
}

/*
 * pack_vector - Pack a vector.
 *
 * Inputs:
 *	vector		: Pointer to vector, voided.
 *	pack_func	: Pointer to element pack function.
 * InOuts:
 *	buf		: Pointer to pointer of buffer, modifed during routine.
 * Return value:
 *	void.
 */
static void pack_vector
#ifdef __STDC__
	(void **vector, void (*pack_func)(), char **buf)
#else 
	(vector, pack_func, buf)
	void **vector;
	void (*pack_func)();
	char **buf;
#endif /* __STDC__ */
{
	int *p;		/* Pointer into number of elements slot */
	int n = 0;	/* Number of elements packed */
  	
	p = (int *)(*buf);
	*buf += sizeof(int *);
	if (vector != NULL)
	  while (*vector) {
	    (*pack_func)(*vector, buf);
	    ++n;
	    ++vector;
	  }
	*p = n;
}

/*
 * sum_princ_vector - Return size of a packed v5b2 principal vector.
 *
 * The V5 BETA2 kerberos code has a different style krb5_principal vector
 * than the V5 stuff that DCE has.  We pack a v5b2-style one independently
 * here.
 *
 * Inputs:
 *	princ		: krb5_principal
 * Return value:
 *	Size of packetized vector.
 */
static int sum_princ_vector
#ifdef __STDC__
	(krb5_principal princ)
#else 
	(princ)
	krb5_principal princ;
#endif /* __STDC__ */
{
	int total = sizeof(unsigned long);	/* Total storage used (first longword is # elements) */
	krb5_data *d;				/* Pointer to individual elements */
	int i;

	total += sum_data(&princ->realm);
	for (d = princ->data, i = princ->length; i; i--, d++) {
		total += sum_data(d);
	}
	return(total);
}

/*
 * pack__princ_vector - Pack a v5b2 principal vector.
 *
 * The V5 BETA2 kerberos code has a different style krb5_principal vector
 * than the V5 stuff that DCE has.  We pack a v5b2-style one independently
 * here.
 *
 * Inputs:
 *	princ		: krb5_principal
 *	pack_func	: Pointer to element pack function (pack_data).
 * InOuts:
 *	buf		: Pointer to pointer of buffer, modifed during routine.
 * Return value:
 *	void.
 */
static void pack_princ_vector
#ifdef __STDC__
	(krb5_principal princ, void (*pack_func)(), char **buf)
#else 
	(princ, pack_func, buf)
	krb5_principal princ;
	void (*pack_func)();
	char **buf;
#endif /* __STDC__ */
{
	int *p;		/* Pointer into number of elements slot */
	int i;		/* Counter of elements processed */
	krb5_data *d;   /* Pointer to individual elements */

	p = (int *)(*buf);
	*p = 1 + princ->length;
	*buf += sizeof(int *);
	(*pack_func)(&princ->realm, buf);
	for (d = princ->data, i = princ->length; i; i--, d++) {
		(*pack_func)(d, buf);
	}
}

/*
 * tgt_pack - Pack a TGT 
 *
 * Inputs:
 *	tgt		: krb5_cred structure of local tgt
 * Outputs:
 *	packet		: Reference to pointer.  Filled in with pointer to packed tgt. 
 *	len		: Length of entire packet
 * Return value:
 *	0 on success, -1 otherwise.
 *
 * This routine allocates storage for the packet that must be freed when
 * it is of no longer of use.
 */
int tgt_pack
#ifdef __STDC__
	(krb5_creds *cred, char **packet, int *len)
#else 
	(cred, packet, len)
	krb5_creds *cred;
	char **packet;
	int *len;
#endif /* __STDC__ */
{
	int length;		/* Length of whole mess */
	char *p;		/* Pointer into packet */		

	/*
	 * One pass to compute how much storage we'll need.
	 */
	length = sum_princ_vector(cred->client) + sum_princ_vector(cred->server) 
	  	+ sum_keyblock(&cred->keyblock) + sum_ticket_times(&cred->times) +
		 roundup(sizeof(cred->is_skey)) + roundup(sizeof(cred->ticket_flags)) + sum_vector((void **)cred->addresses, sum_address) + 
		 sum_data(&cred->ticket) + sum_data(&cred->second_ticket) + sum_vector((void **)cred->authdata, sum_authdata); 

	if ((p = (char *)malloc(length)) == NULL) {
		return(-1);
	}
	*packet = p;
	*len = length; 
	/*
	 * Assemble packet and return.  
	 */
	pack_princ_vector(cred->client, pack_data, &p);
	pack_princ_vector(cred->server, pack_data, &p);
	pack_keyblock(&cred->keyblock, &p);
	pack_ticket_times(&cred->times, &p);
	*(krb5_boolean *)p = cred->is_skey;
	p = (char *)roundup(((int)(p + sizeof(cred->is_skey))));
	*(krb5_flags *)p = cred->ticket_flags;
	p = (char *)roundup(((int)(p + sizeof(cred->ticket_flags))));
	pack_vector((void **)cred->addresses, pack_address, &p);
	pack_data(&cred->ticket, &p);
	pack_data(&cred->second_ticket, &p);
	pack_vector((void **)cred->authdata, pack_authdata, &p);
	return(0);
}
