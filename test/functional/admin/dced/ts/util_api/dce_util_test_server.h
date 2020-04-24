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
 * $Log: dce_util_test_server.h,v $
 * Revision 1.1.7.3  1996/02/18  23:05:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:30  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:10  root
 * 	Submit
 * 	[1995/12/11  14:33:40  root]
 * 
 * Revision 1.1.2.3  1994/10/27  20:07:16  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:32  rrizzo]
 * 
 * Revision 1.1.2.2  1994/10/19  20:50:02  max
 * 	fix includes for all reference platforms
 * 	[1994/10/19  20:46:41  max]
 * 
 * Revision 1.1.2.1  1994/10/18  20:55:49  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:47:58  max]
 * 
 * $EndLog$
 */

#include <pthread.h>
#include <dce/rgybase.h>
#include <dce/sec_login.h>
#include <dce/secidmap.h>


#ifndef _dce_util_test_server_

/* space for 3 lines */
#define SND_INFOLINE_SZ 240
extern char snd_infoline_buf[SND_INFOLINE_SZ];


extern const char * tet_unresolved_c_str;
extern const char * tet_pass_c_str;
extern const char * tet_fail_c_str;



/* this is the srvrconf/FVT_TEST_dce_util_server attr uuid */
extern char * attr_uuid_g_str;
extern char * binding_obj_uuid_g_str;


/* our conf obj uuid */
extern char * conf_uuid_g_str;
extern char * exec_uuid_g_str;

extern char *principal_g_str; /* this is passed in argv[1] */
extern char *attr_value_0_g_str; /* first attribute value */

extern rpc_if_handle_t if_handle_glob;
extern uuid_t binding_obj_uuid_glob; 
extern uuid_t attr_uuid_glob;
extern uuid_t conf_uuid_glob;


#endif



/* and now all the protos */


int check_our_binding_ep_map (void);
int check_our_if_registered( void);
int do_our_registration ( dce_server_handle_t * server_handle_p);



int prin_uuid_to_name( uuid_t *uuid, sec_rgy_name_t *name );
int net_info_to_principal ( sec_login_net_info_t *net_info, sec_rgy_name_t *prin );




int send_infoline( const char * a);
int send_infoline_nl( const char * a);
int send_infoerror(unsigned32 a);    
int send_infoerror_nl(unsigned32 a);








