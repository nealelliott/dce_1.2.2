/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: test.c,v $
 * Revision 1.1.4.2  1996/02/18  00:08:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:52  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:45  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:56:44  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:51:15  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:54:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  test.c V=2 10/23/91 //littl/prgy/krb5/comerr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
#include <stdio.h>
#include <errno.h>
#include "com_err.h"
#include "test1.h"
#include "test2.h"

extern int sys_nerr, errno;

main()
{
	printf("Before initiating error table:\n\n");
	printf("Table name '%s'\n", error_table_name(KRB_MK_AP_TGTEXP));
	printf("UNIX  name '%s'\n", error_table_name(EPERM));
	printf("Msg TGT-expired is '%s'\n", error_message(KRB_MK_AP_TGTEXP));
	printf("Msg EPERM is '%s'\n", error_message(EPERM));
	printf("Msg FOO_ERR is '%s'\n", error_message(FOO_ERR));
	printf("Msg {sys_nerr-1} is '%s'\n", error_message(sys_nerr-1));
	printf("Msg {sys_nerr} is '%s'\n", error_message(sys_nerr));

	printf("With 0: tgt-expired -> %s\n", error_message(KRB_MK_AP_TGTEXP));

	initialize_krb_error_table();
	initialize_krb_error_table();
	printf("KRB error table initialized:  base %d (%s), name %s\n",
	       ERROR_TABLE_BASE_krb, error_message(ERROR_TABLE_BASE_krb),
	       error_table_name(ERROR_TABLE_BASE_krb));
	printf("With krb: tgt-expired -> %s\n",
	       error_message(KRB_MK_AP_TGTEXP));

	initialize_quux_error_table();
	printf("QUUX error table initialized: base %d (%s), name %s\n",
	       ERROR_TABLE_BASE_quux, error_message(ERROR_TABLE_BASE_quux),
	       error_table_name(ERROR_TABLE_BASE_quux));

	printf("Msg for TGT-expired is '%s'\n",
	       error_message(KRB_MK_AP_TGTEXP));
	printf("Msg {sys_nerr-1} is '%s'\n", error_message(sys_nerr-1));
	printf("Msg FOO_ERR is '%s'\n", error_message(FOO_ERR));
	printf("Msg KRB_SKDC_CANT is '%s'\n",
		    error_message(KRB_SKDC_CANT));
	printf("Msg 1e6 (8B 64) is '%s'\n", error_message(1000000));
	printf("\n\nCOM_ERR tests:\n");
	com_err("whoami", FOO_ERR, (char *)NULL);
	com_err("whoami", FOO_ERR, " -- message goes %s", "here");
	com_err("whoami", 0, (char *)0);
	com_err("whoami", 0, "error number %d\n", 0);
}
