/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: import1.h,v $
 * Revision 1.1.10.2  1996/02/18  23:03:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:31  marty]
 *
 * Revision 1.1.10.1  1995/12/08  22:11:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:44:50  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:12:46  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  11:55:23  marrek]
 * 
 * Revision 1.1.6.2  1993/08/12  11:29:08  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:14:30  marrek]
 * 
 * Revision 1.1.4.2  1993/01/04  17:02:43  htf
 * 	Embedded copyright notice
 * 	[1993/01/04  16:15:38  htf]
 * 
 * Revision 1.1.2.2  1992/06/11  19:51:55  melman
 * 	 gds drop
 * 	[1992/06/11  19:18:05  melman]
 * 
 * $EndLog$
 */
#ifndef _IMPORT1_H
#define _IMPORT1_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char import1_rcsid[] = "@(#)$RCSfile: import1.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:03:53 $";
#endif

/*
The following definitions are used for the
user encoded type ``import1''
*/
#define import1_len(n,f)	(17)
#define import1_match(z) 	((z)[0] == 67)
#define import1_free(f)		(0)
#define import1_err(f)		(0)

asn1 import1_dec(), import1_lwd(), import1_cod(), import1_lwc();
char * import1_in(), * import1_out();

#endif /* _IMPORT1_H */
