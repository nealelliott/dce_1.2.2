/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: vsys.c,v $
 * Revision 1.1.46.1  1996/10/02  16:57:03  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:27  damon]
 *
 * Revision 1.1.40.4  1994/09/28  21:21:56  sasala
 * 	bos output incorrect -missing output, OT 12280, S12Y
 * 	[1994/09/28  21:20:08  sasala]
 * 
 * Revision 1.1.40.3  1994/08/01  17:52:05  sasala
 * 	Merged with changes from 1.1.40.2
 * 	[1994/08/01  17:51:59  sasala]
 * 
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  15:07:47  sasala]
 * 
 * 	Implement partial S12Y (internationalization) for DFS commands and daemons
 * 	[1994/07/21  14:51:12  sasala]
 * 
 * Revision 1.1.40.2  1994/06/09  13:49:44  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:14  annie]
 * 
 * Revision 1.1.40.1  1994/02/04  20:04:00  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:05:37  devsrc]
 * 
 * Revision 1.1.38.1  1993/12/07  17:11:02  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:40:21  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/18  19:49:22  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:22:35  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  15:36:24  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:03:46  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:36:16  devrcs
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
#include <dcedfs/osi.h>
#include <dcedfs/syscall.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsdsdmac.h>
#include <dfsdsdsvc.h>
#include <dfsdsdmsg.h>
 
dce_svc_handle_t   dsd_svc_handle;

void initialize_svc()
{
  error_status_t st;

  dsd_svc_handle = dce_svc_register(dsd_svc_table,  (idl_char *)"dsd",  &st);
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

  dfs_define_dsd_message_tables();

}

main (argc, argv)
    int argc;
    char **argv; {
    long code;
    int i, counter;
    long parms[6];
    int numberFlag;

    osi_setlocale(LC_ALL, "");

    initialize_svc();

    if (argc < 2) {dce_svc_printf(DSD_S_VSYSUSAGE_MSG); exit(1); }
    numberFlag = 1;
    counter = 0;
    for(i=1;i<argc;i++) {
	if (numberFlag && argv[i][0] == '-') {
	    if (strcmp(argv[i], "-s")==0) numberFlag = 0;
	    else {
		dce_svc_printf(DSD_S_BADSWITCH_MSG, argv[i]);
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
    code = afs_syscall(AFSCALL_CM, parms[0], parms[1], parms[2], parms[3], parms[4]); 
    dce_svc_printf(DSD_S_VSYSCODE_MSG, code);
}
