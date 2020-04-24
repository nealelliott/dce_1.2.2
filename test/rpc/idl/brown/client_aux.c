/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.8.2  1996/02/17  23:12:57  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:01  marty]
 *
 * Revision 1.1.8.1  1995/12/11  19:58:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:02  root]
 * 
 * Revision 1.1.6.2  1993/06/15  21:05:12  sommerfeld
 * 	Add include of test_common.h
 * 	[1993/06/14  20:46:59  sommerfeld]
 * 
 * Revision 1.1.2.2  1993/01/07  19:10:00  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:28:44  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:31:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*%a%copyright(Copyright)
    40 1.1 5/8/91
*/

#include <stdlib.h>
#include <stdio.h>
#include <test_common.h>
#include <brown.h>

static ndr_boolean verbose = ndr_false;

#define RPC_SS_HEAP
#ifdef RPC_SS_HEAP
#	define ENABLE() rpc_ss_enable_allocate()
#	define FREE(p) rpc_ss_free(p)
#else
#	define ENABLE()
#	define FREE(p) free(p)
#endif

long client_aux
#ifdef NIDL_PROTOTYPES
(handle_t h,
 int passes)
#else
(h,passes)
handle_t h;
 int passes;
#endif
{
	long            index, result, failures = 0;
	ts_client_t     profile;
	ts_clientList_t clientListP;

    if (verbose)
        printf("In brown_test().\n");

/*	srandom(time(NULL));   */

	profile.clientAuth = random() % 128;

	profile.clientId = (idl_char *)malloc(profile.clientAuth + 1);
	if (NULL == profile.clientId) {
		fprintf(stderr, "In brown_test(): Cannot allocate string.\n");
		exit(1);
	}

	for (index = 0; index < profile.clientAuth; ++index)
		profile.clientId[index] = 'A' + random() % ('z' - 'A');
	profile.clientId[index] ='\0';

	if (verbose) {
		printf("Before tsRPC_ListClient(): profile.\n");
		printf("\tclientId   == \"%s\"\n", profile.clientId);
		printf("\tclientAuth == %d\n", profile.clientAuth);
		printf("\tclientListP == %lx\n", clientListP);
	}

	ENABLE();

	result = tsRPC_ListClient(h, profile, &clientListP);

	if (verbose) {
		printf("After %d == tsRPC_ListClient(): clientListP->:\n", result);
		printf("\tclientListP == %lx\n", clientListP);
	}

	if (result != profile.clientAuth) {
		printf("profile.clientAuth != (%d) tsRPC_ListClient()\n", result);
		++failures;
	}

	if (verbose)
		printf("\tclientCount == %d\n", clientListP->clientCount);

	if (clientListP->clientCount != profile.clientAuth) {
		printf("clientListP->clientCount (%d) ", clientListP->clientCount);
		printf("!= (%d) profile.clientAuth\n", profile.clientAuth);
		++failures;
	}

	for (index = 0; index < clientListP->clientCount; ++index) {
		long len, cmp;

		if (verbose) {
			printf("\tclientP[%d]:\n", index);
			printf("\t\tclientId == \"%s\"\n", clientListP->clientP[index].clientId);
			printf("\t\tclientAuth == %d\n", clientListP->clientP[index].clientAuth);
		}

		len = strlen(clientListP->clientP[index].clientId);

		if (len != clientListP->clientP[index].clientAuth) {
			printf("strlen(clientListP->clientP[%d].clientId) (%d) ", index, len);
			printf("!= (%d) ", clientListP->clientP[index].clientAuth);
			printf("clientListP->clientP[%d].clientAuth\n", index);
			++failures;
		}

		if (cmp = strncmp(clientListP->clientP[index].clientId, profile.clientId, len)) {
			printf("%d == strncmp(", cmp);
			printf("clientListP->clientP[%d].clientId, profile.clientId,", index);
			printf("%d)", len);
			++failures;
		}

		FREE((char *)clientListP->clientP[index].clientId);
	}

	FREE((char *)clientListP);

	free(profile.clientId);

    return failures;
}
