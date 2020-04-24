/*
 ** @OSF_COPYRIGHT@
 ** COPYRIGHT NOTICE
 ** Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 ** ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 ** the full copyright text.
*/
/*
 * HISTORY
 * $Log: px_config.c,v $
 * Revision 1.1.12.1  1996/10/02  16:56:45  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:18  damon]
 *
 * Revision 1.1.6.1  1994/06/09  13:49:34  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:03  annie]
 * 
 * Revision 1.1.4.3  1993/01/18  19:48:32  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:21:42  cjd]
 * 
 * Revision 1.1.4.2  1992/11/24  15:15:16  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:03:23  bolinger]
 * 
 * 	Merging dfs5.11 to dfs6.3
 * 
 * Revision 1.1.2.2  1992/01/24  01:47:31  devsrc
 * 	Bring in dfs6.3 sources
 * 	[1992/01/22  19:28:42  devsrc]
 * 
 * Revision 1.1.2.2  91/11/20  17:38:33  garyf
 * 	initial coding
 * 	[91/11/20  17:36:58  garyf]
 * 
 * $EndLog$
*/

#include <dce_afs.h>
#include <dcedfs/param.h>		/* Should be always first */
#include <dcedfs/syscall.h>
#include <sys/sysconfig.h>

extern int afscall_exporter();
extern int afs_nosys();

fx_configure(op, indata, indatalen, outdata, outdatalen)
  sysconfig_op_t    op;
  void              *indata;
  size_t            indatalen;
  void              *outdata;  
  size_t            outdatalen;
{
#define afson (op == SYSCONFIG_CONFIGURE)
	
	AfsCall(AFSCALL_PX, afson ? afscall_exporter : afs_nosys);
	
	return 0;
}

