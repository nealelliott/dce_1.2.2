/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krb5-manual-glue.c,v $
 * Revision 1.1.13.1  1996/10/03  15:05:54  arvind
 * 	detect malloc and other failures and return an error.
 * 	clean up
 * 	constify.
 * 	include MAVROS-dependent guck directly.
 * 	[1996/09/16  21:28 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.11.2  1996/02/18  00:15:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:31  marty]
 * 
 * Revision 1.1.11.1  1995/12/08  18:02:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:26  root]
 * 
 * Revision 1.1.8.1  1994/08/26  16:18:50  sommerfeld
 * 	Fix compiler warnings.
 * 	Fix bitfield routines
 * 	[1994/08/26  15:58:45  sommerfeld]
 * 
 * Revision 1.1.6.4  1992/12/29  16:00:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:16:52  zeliff]
 * 
 * Revision 1.1.6.3  1992/09/29  21:19:47  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:33:59  sekhar]
 * 
 * Revision 1.1.6.2  1992/09/03  14:30:03  bolinger
 * 	Fix to compile with new threads source base (see OT defect 5300):
 * 	because threads headers no longer cause timeb.h to be included,
 * 	it has to be included here unconditionally.
 * 	[1992/09/02  13:42:35  bolinger]
 * 
 * Revision 1.1.2.2  1992/04/14  12:49:37  mishkin
 * 	Try to work around CMA dependency
 * 	[1992/04/10  19:38:58  mishkin]
 * 
 * Revision 1.1  1992/01/19  14:49:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  krb5-manual-glue.c V=13 12/12/91 //littl/prgy/krb5/lib/mvr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* krb5-manual-glue.c
 * 
 * Copyright Hewlett-Packard Company 1990, 1991
 *  Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
 *
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

#include <krb5/krb5.h>
#include <krb5/asn1.h>
#include <dce/asn1.h>
#include <krb5/mvr/KRB5_mvr.h>
#include <krb5/mvr/KRB5.h>
#include <krb5/mvr/krb5-manual-glue.h>
#include <krb5/ext-proto.h>

#include <sys/timeb.h>

#define CHECK_ASN_MALLOC(t,ty,s) {size_t l_ = (s)*sizeof(ty); ty *t1=(ty*)asn1_malloc(l_?l_:1); (t)=t1; if (t1 == NULL) return ENOMEM; memset(t1,0,l_);}

#define CHECK_MALLOC(t,ty,s) {size_t l_ = (s)*sizeof(ty); ty *t1=(ty*)malloc(l_?l_:1); (t)=t1; if (t1 == NULL) return ENOMEM; memset(t1,0,l_);}

#define CHECK_MALLOC1(t,ty) {ty *t1=(ty*)malloc(sizeof(ty)); (t)=t1; if (t1 == NULL) return ENOMEM; memset(t1,0,sizeof(ty));}

#define ERRCHECK() {if (errcode) return errcode;}


krb5_error_code asn1_field2krb5_flags(m, k)
    const asn1_field *m;
    krb5_flags *k;
{
    krb5_flags val;

    val  = (m->v[3] & 0xff);
    val |= (m->v[2] & 0xff) <<  8;
    val |= (m->v[1] & 0xff) << 16;
    val |= (m->v[0] & 0xff) << 24;
    *k = val;
    return 0;
}

krb5_error_code krb5_flags2asn1_field(k, m)
    const krb5_flags *k;
    asn1_field *m;
{
    krb5_flags val = *k;
    
    m->l = 32;                   /* 32 bits */
    CHECK_ASN_MALLOC(m->v, asn1_octet, 4);
    m->v[3] = ( val        & 0xff);
    m->v[2] = ((val >>  8) & 0xff);
    m->v[1] = ((val >> 16) & 0xff);
    m->v[0] = ((val >> 24) & 0xff);
    return 0;
}

krb5_error_code asn1_field2krb5_data(m, k)
    const asn1_field *m;
    krb5_data *k;
{
    int len = k->length = m->l;
    CHECK_MALLOC(k->data, char, len);
    memcpy (k->data, m->v, len);
    return 0;
}

krb5_error_code krb5_data2asn1_field(k, m)
    const krb5_data *k;
    asn1_field *m;
{
    int len = k->length;
    m->l = 0;
    CHECK_ASN_MALLOC(m->v, asn1_octet, len);
    m->l = len;
    memcpy (m->v, k->data, len);
    return 0;
}

krb5_error_code asn1_field2krb5_octets(m, l, c)
    const asn1_field *m;
    int *l;
    krb5_octet **c;
{
    int len = *l = m->l;
    CHECK_MALLOC(*c, krb5_octet, len);
    memcpy (*c, m->v, len);
    return 0;
}

krb5_error_code krb5_octets2asn1_field(l, c, m)
    const int *l;
    krb5_octet * const* c;
    asn1_field *m;
{
    int len = m->l = *l;
    CHECK_ASN_MALLOC(m->v, asn1_octet, len);
    memcpy (m->v, *c, len);
    return 0;
}
    
krb5_error_code krb5_timestamp2asn1_field(k, m)
    const krb5_timestamp *k;
    asn1_field *m;
{
    struct timeb t;
    
    t.time = *k;
    t.millitm = 0;
    t.timezone = 0;
    t.dstflag = 0;

    CHECK_ASN_MALLOC(m->v, asn1_octet, 20);
    m->l = asn1_gen_time ((char *)m->v, &t);
    return 0;
}

krb5_error_code asn1_field2krb5_timestamp(m, k)
    const asn1_field *m;
    krb5_timestamp *k;
{
    struct timeb t;

    asn1_time_gen ((char *)m->v, m->l, &t);

    *k = t.time;
    return 0;
}

krb5_error_code PrincipalRealm2krb5_principal(n, r, k)
    const PrincipalName *n;
    const Realm *r;
    krb5_principal *k;
{
    int i, size;
    krb5_principal ret;
    krb5_error_code errcode;
    
    size = n->name_string.n+1;

    CHECK_MALLOC(ret, krb5_data*, size+1);
    for (i=0; i<size; i++) {
	CHECK_MALLOC1(ret[i], krb5_data);
    }
    errcode = asn1_field2krb5_data (r, ret[0]);
    ERRCHECK();

    for (i=0; i<size-1; i++) {
        errcode = asn1_field2krb5_data (&(n->name_string.v[i]), ret[i+1]);
	ERRCHECK();
    }
    ret[size] = 0;
    (*k) = ret;
    return 0;
}


krb5_error_code krb5_principal2PrincipalRealm(k, n, r)
    const krb5_principal *k;
    PrincipalName *n;
    Realm *r;
{
    int i, size;
    krb5_error_code errcode;
    
    if (r) {
        errcode = krb5_data2asn1_field ((*k)[0], r);
	ERRCHECK();
    }
    

    /*
     * In early V5 implementations, only NT-PRINCIPAL is used.
     */
    n->name_type = KRB5_NT_PRINCIPAL;
    for (i=1; (*k)[i] != 0; i++) ;
    size = i-1;

    n->name_string.n = 0;
    CHECK_ASN_MALLOC(n->name_string.v, asn1_field, size);
    n->name_string.n = size;

    for (i=0; i < size; i++) {
        errcode = krb5_data2asn1_field((*k)[i+1], &n->name_string.v[i]);
	ERRCHECK();
    }
    return 0;
}


krb5_error_code OPrincipalRealm2krb5_principal(n, r, k)
    PrincipalName * const* n;
    const Realm *r;
    krb5_principal *k;
{
    krb5_error_code errcode;

    if (*n) {
        errcode = PrincipalRealm2krb5_principal (*n, r, k);
	ERRCHECK();
    } else {
        *k = 0;
    }
    return 0;
}

krb5_error_code krb5_principal2OPrincipalRealm(k, n, r)
    const krb5_principal *k;
    PrincipalName **n;
    Realm *r;
{
    krb5_error_code errcode;

    if (*k) {
	CHECK_ASN_MALLOC(*n, PrincipalName, 1);
        errcode = krb5_principal2PrincipalRealm (k, *n, r);
	ERRCHECK();
    } else {
        *n = 0;
        if (r)
            r->l = 0;
    }
    return 0;
}

krb5_error_code OPrincipalORealm2krb5_principal (n, r, k)
    PrincipalName * const* n;
    Realm * const* r;
    krb5_principal *k;
{
    krb5_error_code errcode;

    if (*r) {
        errcode = OPrincipalRealm2krb5_principal (n, *r, k);
	ERRCHECK();
    } else {
        *k = 0;
    }
    return 0;
}

krb5_error_code krb5_principal2OPrincipalORealm (k, n, r)
    const krb5_principal *k;
    PrincipalName **n;
    Realm **r;
{
    krb5_error_code errcode;

    if (*k) {
        if (r) {
            CHECK_ASN_MALLOC(*r,Realm,1);
	}
        errcode = krb5_principal2OPrincipalRealm (k, n, *r);
	ERRCHECK();
    } else {
        if (r)
            *r = 0;
        *n = 0;
    }
    return 0;
}

krb5_error_code krb5_enctype_vec2INTSEQ (k, n, m)
    krb5_enctype * const* k;
    const int *n;
    INTSEQ *m;
{
    int len = m->n = *n;
    int i;

    CHECK_ASN_MALLOC(m->v, int, len);
    for (i=0; i<len; i++)
        m->v[i] = (*k)[i];
    return 0;
}

krb5_error_code INTSEQ2krb5_enctype_vec (m, k, n)
    const INTSEQ *m;
    krb5_enctype **k;
    int *n;
{
    int i;
    int len = m->n;
    krb5_enctype *r;

    CHECK_MALLOC(r, krb5_enctype, len);

    for (i=0; i<len; i++) 
        r[i] = m->v[i];
    *k = r;
    *n = len;
    return 0;
}    

krb5_error_code opt_krb5_octets2asn1_field (int const *l, krb5_octet * const* c, asn1_field **m)
{
    krb5_error_code errcode;

    if (c==NULL)
	*m = NULL;
    else {
	asn1_field *r;
	CHECK_MALLOC1(r, asn1_field);
	errcode = krb5_octets2asn1_field(l, c, r);
	ERRCHECK();
	*m = r;
    }
    return 0;
}

krb5_error_code opt_asn1_field2krb5_octets (asn1_field const* m, int *l, krb5_octet **c) 
{
    krb5_error_code errcode;

    if (m == NULL) {
	*c = NULL;
	*l = 0;
    } else {
	errcode = asn1_field2krb5_octets (m, l, c);
	ERRCHECK();
    }
    return 0;
}
