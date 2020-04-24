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
 * $Log: enable_service_t.c,v $
 * Revision 1.1.7.3  1996/02/18  00:31:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:25  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:19  root
 * 	Submit
 * 	[1995/12/11  14:33:44  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:19  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:37  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/18  20:55:57  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:48:02  max]
 * 
 * $EndLog$
 */


#include <dtapi_impl.h>



#define SND_INFOLINE_SZ 240
char rcv_infoline_buf[SND_INFOLINE_SZ];


void setup(void);
void cleanup(void);
void dce_server_enable_service_001(void);





void (*tet_startup)(void) = setup;	/* Load the start routine into TET */
void (*tet_cleanup)(void) = cleanup;	/* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dce_server_enable_service_001,  1 },
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





void dce_server_enable_service_001(void)
{


    /* our assertion */
	tet_infoline("dce_server_enable_service_001, Assertion 01(A): ");
	tet_infoline(
	"When a server object launched by dced calls after successfully");
	tet_infoline(
	"calling dce_server_register, calls dce_server_enable_service, ");
	tet_infoline(
	"the specified service is disabled."); 

	common_server_func ("dce_server_enable_service_001");

}










