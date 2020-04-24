/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: asn1glue.h,v $
 * Revision 1.1.2.1  1996/06/05  20:36:02  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:39:02  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/asn.1/asn1glue.h
 *
 * Copyright 1989,1990 by the Massachusetts Institute of Technology.
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
 * Header file for some glue functions (macros, mostly)
 */


#ifndef __KRB5_GLUE_H__
#define __KRB5_GLUE_H__

#define krb5_data2qbuf(val) str2qb((val)->data, (val)->length, 1)

#define krb5_kdcoptions2KRB5_KDCOptions(val, err) (struct type_KRB5_KDCOptions *)krb5_flags2KRB5_TicketFlags(val, err)
#define KRB5_KDCOptions2krb5_kdcoptions(val, err) KRB5_TicketFlags2krb5_flags((struct type_KRB5_TicketFlags *) (val), err)
#define krb5_apoptions2KRB5_APOptions(val, err) (struct type_KRB5_APOptions *)krb5_flags2KRB5_TicketFlags(val, err)
#define KRB5_APOptions2krb5_apoptions(val, err) KRB5_TicketFlags2krb5_flags((struct type_KRB5_APOptions *) (val), err)

/* to keep lint happy */
#define xbcopy(src,dst,size) memcpy((char *)(dst), (char *)(src), size)
#define xmalloc(n) malloc((unsigned) (n))
#define xcalloc(n,s) calloc((unsigned)(n), (unsigned)(s))

#endif /* __KRB5_GLUE_H__ */
