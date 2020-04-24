/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/17  23:12:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:01  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:58:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:03  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:10:04  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:28:47  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:31:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*%a%copyright(Copyright)
    41 1.1 5/8/91
*/

#include <stdio.h>
#include <brown.h>

static ndr_boolean verbose=ndr_false;

static idl_long_int
mgr_op0
#ifdef NIDL_PROTOTYPES
(
	/* in  */ handle_t        h,
	/* in  */ ts_client_t     profile,
	/* out */ ts_clientList_t *clientListP
)
#else
(h, profile, clientListP)
	/* in  */ handle_t        h;
	/* in  */ ts_client_t     profile;
	/* out */ ts_clientList_t *clientListP;
#endif
{
	long index;

    if (verbose) {
        printf("In mgr_op0().\n");
		printf("\tclientId   == \"%s\"\n", profile.clientId);
		printf("\tclientAuth == %d\n", profile.clientAuth);
	}

	*clientListP = (ts_clientList_t)rpc_ss_allocate(
		sizeof(struct ts_clientList) + (profile.clientAuth - 1) * sizeof(ts_client_t)
	);

	if (NULL == *clientListP) {
		fprintf(stderr, "In mgr_op0(): Cannot allocate struct ts_clientList.\n");
		exit(1);
	}

	for (index = 0; index < profile.clientAuth; ++index) {
		(*clientListP)->clientP[index].clientAuth = profile.clientAuth - index;
		(*clientListP)->clientP[index].clientId = (idl_char *)rpc_ss_allocate(
			(*clientListP)->clientP[index].clientAuth + 1
		);
		if (NULL == (*clientListP)->clientP[index].clientId) {
			fprintf(stderr, "In mgr_op0(): Cannot allocate string.\n");
			exit(1);
		}
		strncpy(
			(*clientListP)->clientP[index].clientId,
			profile.clientId, (*clientListP)->clientP[index].clientAuth
		);
                (*clientListP)->clientP[index].clientId[(*clientListP)->clientP[index].clientAuth] ='\0';
	        
	}
	(*clientListP)->clientCount = index;

	if (verbose) {
		printf("In mgr_op0(): (*clientListP)->\n");
		printf("\tclientCount == %d\n", (*clientListP)->clientCount);
		for (index = 0; index < profile.clientAuth; ++index) {
			printf("\tclientP[%d] ==\n", index);
			printf("\t\tclientAuth == %d\n", (*clientListP)->clientP[index].clientAuth);
			printf("\t\tclientId == \"%s\"\n", (*clientListP)->clientP[index].clientId);
		}
		printf("\n");
	}

	return index;
}

globaldef brown_v0_0_epv_t brown_v0_0_m_epv = {mgr_op0};
