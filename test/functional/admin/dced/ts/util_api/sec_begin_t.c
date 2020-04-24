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
 * $Log: sec_begin_t.c,v $
 * Revision 1.1.7.3  1996/02/18  00:31:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:39  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:37  root
 * 	Submit
 * 	[1995/12/11  14:33:56  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:28  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:48  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/18  20:56:10  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:48:12  max]
 * 
 * $EndLog$
 */


#include <dtapi_impl.h>



#define SND_INFOLINE_SZ 240
char rcv_infoline_buf[SND_INFOLINE_SZ];


void setup(void);
void cleanup(void);
void dce_server_sec_begin_001(void);





void (*tet_startup)(void) = setup;	/* Load the start routine into TET */
void (*tet_cleanup)(void) = cleanup;	/* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dce_server_sec_begin_001,  1 },
    { NULL, 0 }
};




void
setup()
{
    common_setup();
}

void
cleanup()
{
    common_cleanup();
}





void dce_server_sec_begin_001(void)
{

    /* out assertion */
	tet_infoline("dce_server_sec_begin_001, Assertion 01(A): ");
	tet_infoline(
	"When a server object launched by dced calls ");
	tet_infoline(
	"the dce_server_sec_begin API, with the (dce_server_c_manage_key | dce_server_c_login) ");
	tet_infoline(
	"flags, a login is made using the configuration keytab, ");
	tet_infoline(
	" a key management thread is launched and status is set to error_status_ok.");

	common_server_func ("dce_server_sec_begin_001");
}









