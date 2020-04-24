/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dacl_read.c,v $
 * Revision 1.1.449.1  1996/10/02  20:56:08  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:46  damon]
 *
 * Revision 1.1.444.4  1994/08/23  18:59:05  rsarbo
 * 	delegation
 * 	[1994/08/23  16:34:10  rsarbo]
 * 
 * Revision 1.1.444.3  1994/07/13  22:25:54  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:07:49  devsrc]
 * 
 * 	Merged with changes from 1.1.447.3
 * 	[1994/06/07  19:30:29  delgado]
 * 
 * 	delegation ACL support
 * 	[1994/06/07  14:05:08  delgado]
 * 
 * 	Fixed the problem with the merge of the RCSID string.
 * 	[1994/04/29  19:07:36  mbs]
 * 
 * 	Merged with changes from 1.1.447.1
 * 	[1994/04/28  16:29:23  mbs]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:01:24  mbs]
 * 
 * 	remove redundant dprintfs and dmprintfs
 * 	[1994/04/21  14:03:48  delgado]
 * 
 * Revision 1.1.444.2  1994/06/09  14:18:52  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:30:53  annie]
 * 
 * Revision 1.1.444.1  1994/02/04  20:28:51  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:17:30  devsrc]
 * 
 * Revision 1.1.442.1  1993/12/07  17:32:50  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:48:06  jaffe]
 * 
 * Revision 1.1.3.4  1993/01/21  15:17:42  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:54:42  cjd]
 * 
 * Revision 1.1.3.3  1992/11/24  18:29:36  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:21:45  bolinger]
 * 
 * Revision 1.1.3.2  1992/08/31  21:02:38  jaffe
 * 	Replace missing RCS ids
 * 	[1992/08/31  15:45:33  jaffe]
 * 
 * 	Transarc delta: bab-ot4447-security-dacl-remove-ifdef-notdef 1.1
 * 	  Selected comments:
 * 	    Removed #ifdef notdefs from code.
 * 	    ot 4447
 * 	    Removed code covered by #ifdef notdef.
 * 	[1992/08/30  12:19:24  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:52:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	dacl_read.c -- routines for reading ACLs
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/icl.h>

#include <dcedfs/osi.h>

#include <dce/nbase.h>

#include <epi_id.h>
#include <dcedfs/fshs_deleg.h>
#include <dacl.h>

#include <dacl_debug.h>
#include <dacl_trace.h>


#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

extern struct icl_set *dacl_iclSetp;
  
#define CREATION_MODE	0644

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/dacl/dacl_read.c,v 1.1.449.1 1996/10/02 20:56:08 damon Exp $")

IMPORT char * RgySec_ErrorString _TAKES((error_status_t * secErrorP));

EXPORT long dacl_ReadFromDisk(aclP, filenameP, mgrUuidP)
     dacl_t *		aclP;
     char *		filenameP;
     epi_uuid_t *	mgrUuidP;
{
  long		rtnVal = 0;
  int		aclfd;
  char *	bufferP;
  struct stat	statBuf;
  int		savedErrno;
  static char	routineName[] = "dacl_ReadFromDisk";
  
  icl_Trace1(dacl_iclSetp, DACL_ICL_READ_PARSE_0,
	     ICL_TYPE_STRING, ((filenameP!=(char*)NULL)?filenameP:"NULL"));
  
  aclfd = open(filenameP, O_RDONLY, 0);
  if (aclfd >= 0) {
    if (fstat(aclfd, &statBuf) == 0) {
      bufferP = (char *)osi_Alloc(statBuf.st_size);
      if (bufferP != (char *)NULL) {
	/* read the whole file into the buffer (it shouldn't be too awful big) */
	if (read(aclfd, bufferP, statBuf.st_size) == statBuf.st_size) {
	  rtnVal = dacl_ParseAcl(bufferP, statBuf.st_size, aclP, mgrUuidP);
	}
	else {
	  icl_Trace2(dacl_iclSetp, DACL_ICL_READ_FS_1,
		     ICL_TYPE_STRING, filenameP,
		     ICL_TYPE_STRING, SysErrString(errno));
	  rtnVal = DACL_ERROR_FS_READ;
	}
	osi_Free((opaque)bufferP, (long)(statBuf.st_size));
      }
      else {
	icl_Trace1(dacl_iclSetp, DACL_ICL_READ_NONE_0 , ICL_TYPE_LONG, statBuf.st_size);
	rtnVal = DACL_ERROR_BUFFER_ALLOCATION;
      }
    }
    else {
      icl_Trace2(dacl_iclSetp, DACL_ICL_READ_FS_2,
		 ICL_TYPE_STRING, filenameP,
		 ICL_TYPE_STRING, SysErrString(errno));
      rtnVal = DACL_ERROR_FS_STAT;
    }

    if (close(aclfd) != 0) {
      icl_Trace2(dacl_iclSetp, DACL_ICL_READ_FS_3,
		 ICL_TYPE_STRING, filenameP,
		 ICL_TYPE_STRING, SysErrString(errno));
      rtnVal = DACL_ERROR_FS_CLOSE;
    }
  }
  else {
    savedErrno = errno;
    icl_Trace2(dacl_iclSetp, DACL_ICL_READ_FS_4, 
	       ICL_TYPE_STRING, filenameP,	
	       ICL_TYPE_STRING, SysErrString(savedErrno));
    if (savedErrno == ENOENT) {
      rtnVal = DACL_ERROR_ACLFILE_NOT_FOUND;
    }
    else {
      rtnVal = DACL_ERROR_FS_OPEN;
    }
  }
  
  icl_Trace1(dacl_iclSetp, DACL_ICL_READ_PARSE_5 , ICL_TYPE_LONG, rtnVal);
  return rtnVal;
}

