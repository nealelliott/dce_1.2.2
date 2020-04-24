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
 * $Log: glue2.c,v $
 * Revision 1.1.2.1  1996/06/05  20:36:12  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:39:11  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
#include "krb5.h"

krb5_data string_list[3] = {
{0, 14, "ATHENA.MIT.EDU"},
{0, 6, "jtkohl"},
{0, 4, "root"},
};

krb5_data *princ[] = {&string_list[0], &string_list[1], &string_list[2], 0};

krb5_data string_list2[3] = {
{0, 14, "ATHENA.MIT.EDU"},
{0, 6, "krbtgt"},
{0, 14, "ATHENA.MIT.EDU"},
};

krb5_data *princ2[] = {&string_list2[0], &string_list2[1], &string_list2[2], 0};
				   
krb5_last_req_entry lrentries[] = { {32000, 1}, {0, 3}, {10, 2} };
krb5_last_req_entry *lrfoo1[] = {&lrentries[0], &lrentries[1], &lrentries[2], 0};

krb5_authdata adarr1[3] = { {0, 3, 7, "authdat"}, {0,2,4,"foob"}, {0,257,9,"jtkohlxxx"}};
krb5_authdata *authdats[] = {&adarr1[0],&adarr1[1],&adarr1[2],0};

krb5_pa_data authdarr1[] = { {0, 3, 7, "authdat"}, {0,2,4,"foob"}, {0,257,9,"jtkohlxxx"}};
krb5_pa_data *padats[] = {&authdarr1[0],&authdarr1[1],&authdarr1[2],0};

krb5_address adrarr1[] = { {0,ADDRTYPE_INET,4,"abcd"},
			   {0,ADDRTYPE_ISO,10,"6176432831"},
			   {0,ADDRTYPE_INET,4,"efgh"} };
krb5_address *addrs[] = {&adrarr1[0],&adrarr1[1],&adrarr1[2],0};
