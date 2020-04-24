/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dacl_anode.c,v $
 * Revision 1.1.435.1  1996/10/02  18:15:15  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:26  damon]
 *
 * Revision 1.1.430.3  1994/07/13  22:26:03  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:10:58  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:01:07  mbs]
 * 
 * Revision 1.1.430.2  1994/06/09  14:18:30  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:30:33  annie]
 * 
 * Revision 1.1.430.1  1994/02/04  20:28:19  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:17:18  devsrc]
 * 
 * Revision 1.1.428.1  1993/12/07  17:32:32  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:43:59  jaffe]
 * 
 * Revision 1.1.3.3  1993/01/21  15:16:44  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:52:54  cjd]
 * 
 * Revision 1.1.3.2  1992/11/24  18:27:36  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:20:04  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:52:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	dacl_anode.c -- routines to be used in the Episode version of the ACL manager
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/episode/logbuf.h>
#include <dcedfs/episode/file.h>

#include <dcedfs/dacl.h>
#include <dcedfs/icl.h>
/*
#include <dacl_debug.h>
*/
#include <dacl_trace.h>

extern struct icl_set *dacl_iclSetp;

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/dacl/dacl_anode.c,v 1.1.435.1 1996/10/02 18:15:15 damon Exp $")

/* values for which ACL to be obtained from same place as vnode interface gets them ?? */
typedef struct dacl_anode_disk_data {
  epi_anode_t	fileAnodeP;
  int		typeOfAcl;
  u_int32	bytesInAcl;
} dacl_anode_disk_data;

typedef union dacl_disk_data {
  char *		filename;
  dacl_epi_disk_data	anodeAclData;
} dacl_disk_data_t;

EXPORT long dacl_ReadAcl(aclP, mgrUuidP, readDataP)
     dacl_t *		aclP;
     epi_uuid_t *	mgrUuidP;
     dacl_disk_data_t *	readDataP;
{
  long		rtnVal = 0;
  static char	routineName[] = "dacl_ReadAcl";

  if (mgrUuidP != (epi_uuid_t *)NULL) {
    if (bcmp((char *)mgrUuidP, (char *)(&episodeAclMgmtUuid), sizeof(epi_uuid_t)) == 0) {
      dacl_ReadFromAnode(aclP, &(readDataP->anodeAclData), mgrUuidP);
    }
    else if (bcmp((char*)mgrUuidP, (char *)(&bosserverAclMgmtUuid),
		  sizeof(epi_uuid_t)) == 0) {
      dacl_ReadFromDisk(aclP, readDataP->filename, mgrUuidP);
    }
    else {
      rtnVal = DACL_ERROR_UNRECOGNIZED_MGR_TYPE;
    }
  }
  else {
    dacl_LogParamError(routineName, "mgrUuidP", dacl_debug_flag, __FILE__, __LINE__);
    rtnVal = DACL_ERROR_PARAMETER_ERROR;
  }
  
  return rtnVal;
}

EXPORT long dacl_ReadFromAnode(aclP, anodeDataP, mgrUuidP)
     dacl_t *			aclP;
     dacl_anode_disk_data_t *	anodeDataP;
     epi_uuid_t *		mgrUuidP;
{
  long		rtnVal = 0;
  char *	bufferP;
  static char	routineName[] = "dacl_ReadFromAnode";
  
  bufferP = osi_Alloc(anodeDataP->bytesInAcl);
  if (bufferP != (char *)NULL) {
    /* any way to spec which type of ACL? */
    rtnVal = epif_ReadAux(anodeDataP->fileAnodeP,
			  EPIF_AUX_IS_ACL,
			  (u_long)0,		/* read from start */
			  anodeDataP->bytesInAcl,
			  bufferP);
    if (rtnVal == 0) {
      rtnVal = dacl_ParseAcl(bufferP, anodeDataP->bytesInAcl, aclP, mgrUuidP);
    }
    else {
      dprintf(dacl_debug_flag,
	      ("%s: Episode aux data read error: %ld", routineName, rtnVal));
      icl_Trace1(dacl_iclSetp, DACL_ICL_ANODE_NONE_0, ICL_TYPE_LONG, rtnVal);
    }
    osi_Free(bufferP, anodeDataP->bytesInAcl);
  }
  else {
    dprintf(dacl_debug_flag,
	    ("%s: unable to allocate %d bytes of memory", routineName,
	     anodeDataP->bytesInAcl));
    icl_Trace1(dacl_iclSetp, DACL_ICL_ANODE_NONE_1, ICL_TYPE_LONG, anodeDataP->bytesInAcl);
    rtnVal = DACL_ERROR_BUFFER_ALLOCATION;
  }
  
  return rtnVal;
}

EXPORT long dacl_WriteToAnode(aclP, anodeDataP, mgrUuidP)
     dacl_t *			aclP;
     dacl_anode_disk_data_t *	anodeDataP;
     epi_uuid_t *		mgrUuidP;
{
  long		rtnVal = 0;
  int		numBytes;
  char *	bufferP = (char *)NULL;
  struct uio	uio;			      /* I/O descriptor */
  struct iovec	iov;			      /* I/O vector */
  static char	routineName[] = "dacl_WriteToAnode";

  rtnVal = FlattenAcl(aclP, &bufferP, &numBytes);
  if (rtnVal == DACL_SUCCESS) {
    /*
     * The following code is taken from the old eacl code.  I have no idea how this
     * does what it's supposed to do.
     */

    /* set up to do the write */
    epia_InitUio(&uio,		/* I/O command block */
		 &iov,		/* I/O vector */
		 0,		/* anode offset */
		 numBytes,
		 bufferP);

    WITH_TRANS("WriteAcl", anodeDataP->fileAnodeP->device,
	       {
		 rtnVal = epia_Write(trans, anodeDataP->fileAnodeP, 0, &uio);
	       });
	       
    

    /*
     * end of old eacl code
     */

    /* if the flatten call, fails, it returns any allocated memory */
    osi_Free((void *)bufferP, numBytes);
  }

  return rtnVal;
}
