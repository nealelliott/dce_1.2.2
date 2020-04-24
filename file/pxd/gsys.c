/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gsys.c,v $
 * Revision 1.1.387.1  1996/10/02  18:13:48  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:45:45  damon]
 *
 * Revision 1.1.382.4  1994/09/28  21:22:10  sasala
 * 	bos output incorrect -missing output, OT 12280, S12Y
 * 	[1994/09/28  21:20:20  sasala]
 * 
 * Revision 1.1.382.3  1994/08/01  17:57:40  sasala
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  15:08:00  sasala]
 * 
 * 	Implement partial S12Y (internationalization) for DFS commands and daemons
 * 	[1994/07/21  14:51:38  sasala]
 * 
 * Revision 1.1.382.2  1994/06/09  14:17:59  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:30:01  annie]
 * 
 * Revision 1.1.382.1  1994/02/04  20:27:33  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:16:54  devsrc]
 * 
 * Revision 1.1.380.1  1993/12/07  17:31:58  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:19:25  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/21  14:54:39  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:58:51  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  18:25:33  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:17:59  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:43:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <dcedfs/param.h>
#include <dcedfs/cmd.h>
#include <dcedfs/osi.h>
#include <dcedfs/syscall.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsfxdmac.h>
#include <dfsfxdsvc.h>
#include <dfsfxdmsg.h>

dce_svc_handle_t   fxd_svc_handle;

void initialize_svc()
{
  error_status_t st;

  fxd_svc_handle = dce_svc_register(fxd_svc_table,  (idl_char *)"fxd",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dfs_define_fxd_message_tables();

}



main (argc, argv)
    int argc;
    char **argv; {
    long code;
    int i, counter;
    long parms[6];
    int numberFlag;


    initialize_svc();


    if (argc < 2) {dce_svc_printf(FXD_S_USAGE_MSG); exit(1); }
    numberFlag = 1;
    counter = 0;
    for(i=1;i<argc;i++) {
	if (numberFlag && argv[i][0] == '-') {
	    if (strcmp(argv[i], "-s")==0) numberFlag = 0;
	    else {
		dce_svc_printf(FXD_S_BAD_SWITCH_MSG, argv[i]);
		exit(1);
	    }
	}
	else if (numberFlag) {
	    parms[counter++] = atoi(argv[i]);
	    numberFlag = 1;
	}
	else {
	    parms[counter++] = (long) argv[i];
	    numberFlag = 1;
	}
    }
    code = afs_syscall(parms[0], parms[1], parms[2], parms[3], parms[4],
		   parms[5]);
    dce_svc_printf(FXD_S_CODE_MSG, code);
}
