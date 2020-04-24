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
 * $Log: fcc_test.c,v $
 * Revision 1.1.2.1  1996/06/05  20:42:04  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:43:26  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/ccache/file/fcc_test.c
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 */


#include "fcc.h"

krb5_data client1 = {
#define DATA "client1-comp1"
     sizeof(DATA),
     DATA,
#undef DATA
};

krb5_data client2 = {
#define DATA "client1-comp2"
     sizeof(DATA),
     DATA,
#undef DATA
};

krb5_data server1 = {
#define DATA "server1-comp1"
     sizeof(DATA),
     DATA,
#undef DATA
};

krb5_data server2 = {
#define DATA "server1-comp2"
     sizeof(DATA),
     DATA,
#undef DATA
};

krb5_creds test_creds = {
     NULL,
     NULL,
     {
	  1,
	  1,
	  (unsigned char *) "1"
     },
     {
	  1111,
	  2222,
	  3333,
	  4444
     },
     1,
     5555,
     {
#define TICKET "This is ticket 1"
     sizeof(TICKET),
     TICKET,
#undef TICKET
     },
     {
#define TICKET "This is ticket 2"
     sizeof(TICKET),
     TICKET,
#undef TICKET
     },
};

void init_test_cred()
{
     test_creds.client = (krb5_principal) malloc(sizeof(krb5_data *)*3);
     test_creds.client[0] = &client1;
     test_creds.client[1] = &client2;
     test_creds.client[2] = NULL;

     test_creds.server = (krb5_principal) malloc(sizeof(krb5_data *)*3);
     test_creds.server[0] = &server1;
     test_creds.server[1] = &server2;
     test_creds.server[2] = NULL;
}

#define CHECK(kret,msg) \
     if (kret != KRB5_OK) {\
	  printf("%s returned %d\n", msg, kret);\
     };
						   
void fcc_test()
{
     krb5_ccache id;
     krb5_creds creds;
     krb5_error_code kret;
     krb5_cc_cursor cursor;

     init_test_cred();

     kret = krb5_fcc_resolve(context, &id, "/tmp/tkt_test");
     CHECK(kret, "resolve");
     kret = krb5_fcc_initialize(context, id, test_creds.client);
     CHECK(kret, "initialize");
     kret = krb5_fcc_store(context, id, &test_creds);
     CHECK(kret, "store");

     kret = krb5_fcc_start_seq_get(context, id, &cursor);
     CHECK(kret, "start_seq_get");
     kret = 0;
     while (kret != KRB5_CC_END) {
	  printf("Calling next_cred\n");
	  kret = krb5_fcc_next_cred(context, id, &cursor, &creds);
	  CHECK(kret, "next_cred");
     }
     kret = krb5_fcc_end_seq_get(context, id, &cursor);
     CHECK(kret, "end_seq_get");

     kret = krb5_fcc_destroy(context, id);
     CHECK(kret, "destroy");
     kret = krb5_fcc_close(context, id);
     CHECK(kret, "close");
}

