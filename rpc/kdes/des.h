/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: des.h,v $
 * Revision 1.1.8.2  1996/02/18  23:46:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:01  marty]
 *
 * Revision 1.1.8.1  1995/12/08  00:13:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:20  root]
 * 
 * Revision 1.1.6.1  1994/01/21  22:30:47  cbrooks
 * 	Code Cleanup
 * 	[1994/01/21  20:20:50  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  22:34:32  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:49:34  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  19:35:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:15:11  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  17:36:10  rsalz
 * 	"Changed pas part of rpc6 drop."
 * 	[1992/05/01  17:33:51  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:16:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  des.h V=1 11/07/90 //crown/prgy/krb5/lib/destoo
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
**    ** OSF DCE Confidential - DCE Security Component BL 6   **
**    ** OSF DCE AUTH 3 - prerelease                          **
**
*/
/* [eichin:19901017.0001EST] */

#ifndef _DES_H_
#define _DES_H_ 

#include <dce/dce.h>

typedef unsigned char des_cblock[8];
typedef unsigned long des_key_schedule[32];

int des_cbc_encrypt _DCE_PROTOTYPE_ (
(des_cblock *, des_cblock*, signed32, des_key_schedule, des_cblock, int)
) ;

int des_ecb_encrypt _DCE_PROTOTYPE_ (
(des_cblock *, des_cblock *, des_key_schedule, int)
);

int sec_des_is_weak_key _DCE_PROTOTYPE_ (
(des_cblock)
);

int make_key_sched _DCE_PROTOTYPE_ (
(des_cblock *, des_key_schedule)
);

int des_key_sched _DCE_PROTOTYPE_ (
(des_cblock *, des_key_schedule)
);    

unsigned long 
des_cbc_cksum _DCE_PROTOTYPE_ (
(des_cblock *, des_cblock *, register long, des_key_schedule, des_cblock)
) ;

int sec_des_is_weak_key _DCE_PROTOTYPE_ (
(des_cblock)
);

#endif				/* _DES_H_ */

