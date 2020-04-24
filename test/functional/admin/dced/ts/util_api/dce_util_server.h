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
 * $Log: dce_util_server.h,v $
 * Revision 1.1.7.3  1996/02/18  23:05:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:29  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:05  root
 * 	Submit
 * 	[1995/12/11  14:33:37  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:15  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:31  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/18  20:55:44  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:47:56  max]
 * 
 * $EndLog$
 */




#ifdef _dce_util_test_server_
#define EXTERN_GLOB
#else
#define EXTERN_GLOB extern
#endif


/* space for 3 lines */
#define SND_INFOLINE_SZ 240
char snd_infoline_buf[SND_INFOLINE_SZ];


int send_infoline( char * a);
int send_infoline_nl( char * a);
int send_infoerr(unsigned32 a);    
int send_infoerr_nl(unsigned32 a);


EXTERN_GLOB const char * tet_unresolved_c_str = "TET_UNRESOLVED";
EXTERN_GLOB const char * tet_pass_c_str       = "TET_PASS";
EXTERN_GLOB const char * test_fail_c_str      = "TET_FAIL";



/* this is the srvrconf/FVT_TEST_dce_util_server attr uuid */
EXTERN_GLOB const char * attr_uuid_c_str = "008b47dd-6ec9-1d6a-9ac7-0000c09ce054";
EXTERN_GLOB const char * binding_obj_uuid_c_str = "2f407b08-dd56-11cd-a646-000c00b4b407";


/* our conf obj uuid */
EXTERN_GLOB const char * conf_uuid_c_str = "9e5fd7dc-dbd4-11cd-9bc0-000c00b4b407";
EXTERN_GLOB const char * exec_uuid_c_str;

EXTERN_GLOB char *principal_glob_str; /* this is passed in argv[1] */
EXTERN_GLOB rpc_if_handle_t if_handle_glob;
EXTERN_GLOB uuid_t binding_obj_uuid_glob; 
EXTERN_GLOB uuid_t attr_uuid_glob;
EXTERN_GLOB uuid_t conf_uuid_glob;


