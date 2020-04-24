/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: write_msg.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:10  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:45  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:31:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:19:11  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  write_msg.c V=1 06/06/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/write_msg.c,v $
 * $Author: marty $
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
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
 * convenience sendauth/recvauth functions
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_write_msg_c [] =
"$Id: write_msg.c,v 1.1.4.2 1996/02/18 00:15:55 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/los-proto.h>
#include <com_err.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

krb5_error_code
krb5_write_message(fdp, outbuf)
	krb5_pointer	fdp;
	krb5_data	*outbuf;
{
	krb5_int32	len;
	int		fd = *( (int *) fdp);

	len = htonl(outbuf->length);
	if (krb5_net_write(fd, (char *)&len, 4) < 0) {
		return(errno);
	}
	if (len && (krb5_net_write(fd, outbuf->data, outbuf->length) < 0)) {
		return(errno);
	}
	return(0);
}




