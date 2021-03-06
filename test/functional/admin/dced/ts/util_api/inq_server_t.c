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
 * $Log: inq_server_t.c,v $
 * Revision 1.1.7.3  1996/02/18  00:31:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:30  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:26  root
 * 	Submit
 * 	[1995/12/11  14:33:47  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:23  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:41  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/18  20:56:01  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:48:06  max]
 * 
 * $EndLog$
 */


#include <dtapi_impl.h>



#define SND_INFOLINE_SZ 240
char rcv_infoline_buf[SND_INFOLINE_SZ];


void setup(void);
void cleanup(void);
void dce_server_inq_server_001(void);





void (*tet_startup)(void) = setup;	/* Load the start routine into TET */
void (*tet_cleanup)(void) = cleanup;	/* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dce_server_inq_server_001,  1 },
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





void dce_server_inq_server_001(void)
{

    /* out assertion */
	tet_infoline("dce_server_inq_server_001, Assertion 01(A): ");
	tet_infoline(
	"When a server object launched by dced calls ");
	tet_infoline(
	"the dce_server_inq_server API, a pointer to a valid ");
	tet_infoline(
	"server_t structure is returned and status error_status_ok.");

	common_server_func ("dce_server_inq_server_001");
}







