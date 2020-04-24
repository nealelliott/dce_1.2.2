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
 * $Log: get_env.h,v $
 * Revision 1.1.4.2  1996/02/18  23:05:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:41  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:48:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:25:13  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:16:30  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:42:35  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:15:02  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/04  21:53:15  mori_m
 * 	CR 9701: RPC runtime I18N extension functional test drop.
 * 	[1994/05/04  21:38:25  mori_m]
 * 
 * $EndLog$
 */

#define GET_SERVER_ENTRY(entryName) \
{ \
	entryName = getenv("I18N_SERVER_ENTRY"); \
	if (entryName == NULL) \
	{ \
		pthread_lock_global_np(); \
			mtet_infoline("I18N_SERVER_ENTRY is not set.  Abort the test execution\n"); \
			tet_result(TET_DCE_FAIL);\
		pthread_unlock_global_np();\
		return; \
	} \
}

#define GET_CLIENT_LOCALE(localeName) \
{ \
	localeName = getenv("I18N_CLIENT_LOCALE"); \
	if (localeName == NULL) \
	{ \
		pthread_lock_global_np(); \
			mtet_infoline("I18N_CLIENT_LOCALE is not set.  Abort the test execution\n"); \
			tet_result(TET_DCE_FAIL);\
		pthread_unlock_global_np();\
		return; \
	} \
}
