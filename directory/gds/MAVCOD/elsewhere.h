/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: elsewhere.h,v $
 * Revision 1.1.10.2  1996/02/18  23:34:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:07  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:08:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:42  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:36:11  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:01:25  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:49:23  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:04:48  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:10:48  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:47  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:20:38  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:21:53  zeliff]
 * 
 * $EndLog$
 */

#ifndef _ELSEWHERE_H
#define _ELSEWHERE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char elsewhere_rcsid[] = "@(#)$RCSfile: elsewhere.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:34:43 $";
#endif

/*
The include file defines some imported data types
*/

typedef int bits1;
typedef int import1;

#define	bits1_cod(z,m,n,x)	asn1_boolean_cod(z,m,n,*x)
#define	import1_cod(z,m,n,x)	asn1_int_cod(z,m,n,*x)
#define	bits1_len(n,x)	asn1_boolean_len(n,*x)
#define	import1_len(n,x)	asn1_int_len(n,*x)
#define	bits1_dec	asn1_boolean_dec
#define	import1_dec	asn1_int_dec
#define	bits1_lwd	asn1_boolean_lwd
#define	import1_lwd	asn1_int_lwd
#define	bits1_lwc	asn1_boolean_lwc
#define	import1_lwc	asn1_int_lwc
#define	bits1_free	asn1_boolean_free
#define	import1_free	asn1_int_free
#define	bits1_in	asn1_boolean_in
#define	import1_in	asn1_int_in
#define	bits1_out(z,x)	asn1_boolean_out(z,*x)
#define	import1_out(z,x)	asn1_int_out(z,*x)
#define	bits1_olen(x)	asn1_boolean_olen(*x)
#define	import1_olen(x)	asn1_int_olen(*x)
#define	bits1_cpy(x,y)	asn1_boolean_cpy(x,*y)
#define	import1_cpy(x,y)	asn1_int_cpy(x,*y)
#define bits1_match(z)	(*(z) == 1)
#define import1_match(z) (*(z) == 2)

#endif /* _ELSEWHERE_H */
