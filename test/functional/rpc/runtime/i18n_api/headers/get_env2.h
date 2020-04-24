/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: get_env2.h,v $
 * Revision 1.1.4.2  1996/02/18  23:05:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:43  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:48:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:25:14  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:16:31  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:42:36  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:15:03  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/04  21:53:16  mori_m
 * 	CR 9701: RPC runtime I18N extension functional test drop.
 * 	[1994/05/04  21:38:39  mori_m]
 * 
 * $EndLog$
 */
/*
 * This header file is used by a server.  TET journal entries are not created.
 * When error takes place, the error message is written to stdio.
 */

#define GET_SERVER_ENTRY2(entryName) \
{ \
	entryName = getenv("I18N_SERVER_ENTRY"); \
	if (entryName == NULL) \
	{ \
		printf("I18N_SERVER_ENTRY is not set.  Abort the test execution\n"); \
		printf("TET_DCE_FAIL\n");\
		return; \
	} \
}

#define GET_SERVER_LOCALE2(localeName) \
{ \
	localeName = getenv("I18N_SERVER_LOCALE"); \
	if (localeName == NULL) \
	{ \
		printf("I18N_SERVER_ENTRY is not set.  Abort the test execution\n"); \
		printf("TET_DCE_FAIL\n");\
		return; \
	} \
}
