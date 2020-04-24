/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: epi_id.c,v $
 * Revision 1.1.856.1  1996/10/02  20:56:21  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:59  damon]
 *
 * Revision 1.1.851.3  1994/07/13  22:26:19  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:11:08  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:01:32  mbs]
 * 
 * Revision 1.1.851.2  1994/06/09  14:19:02  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:03  annie]
 * 
 * Revision 1.1.851.1  1994/02/04  20:29:08  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:17:38  devsrc]
 * 
 * Revision 1.1.849.1  1993/12/07  17:33:00  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:50:23  jaffe]
 * 
 * Revision 1.1.5.3  1993/01/21  15:18:05  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:55:20  cjd]
 * 
 * Revision 1.1.5.2  1992/11/24  18:30:22  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:22:26  bolinger]
 * 
 * Revision 1.1.3.2  1992/04/21  14:33:48  mason
 * 	Transarc delta: cburnett-ot2713-security_changes_for_aix32 1.1
 * 	  Files modified:
 * 	    security/dacl: dacl_flatten.c, dacl_parse.c, epi_id.c
 * 	  Selected comments:
 * 	    changes to build on AIX 3.2
 * 	    include osi_net.h instead of netinet.h
 * 	[1992/04/20  22:59:41  mason]
 * 
 * Revision 1.1  1992/01/19  02:53:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	epi_id.c -- code to manipulate Episode uuids and principal uuids
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>

#include <epi_id.h>
#include <dacl.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/dacl/epi_id.c,v 1.1.856.1 1996/10/02 20:56:21 damon Exp $")

int shortUuidSize = 4; /* 32 bits */
int zeroStringInit = 0;
char* zeroString;

EXPORT int isFullUuid(epiUuidP)
    epi_uuid_t *       epiUuidP;
{
  char* uuidP;
  int retVal;

  if ( !zeroStringInit ) {
      /* 
       * We do not know when or where is the right place to free this memory
       * as acl_edit can call this multiple times in user space in one
       * invocation of acl_edit itself. Anyway this is allocated only once per 
       * acl_edit invocation in user space, which will be freed up when the 
       * acl_edit process exits and also only once if this function 
       * is ever called in kernel space.
       */
      zeroString=dacl_Alloc(sizeof(epi_uuid_t) - shortUuidSize);
      bzero(zeroString, sizeof(epi_uuid_t) - shortUuidSize);
      zeroStringInit++;
  }
  uuidP = (char *)epiUuidP;
  uuidP += shortUuidSize;
  retVal = bcmp(uuidP, zeroString, sizeof(epi_uuid_t) - shortUuidSize) != 0;

  return retVal;
}

EXPORT void hton_epi_uuid(epiUuidP)
     epi_uuid_t *	epiUuidP;
{
  epiUuidP->longField1 = htonl(epiUuidP->longField1);
  epiUuidP->shortField1 = htons(epiUuidP->shortField1);
  epiUuidP->shortField2 = htons(epiUuidP->shortField2);
}

EXPORT void ntoh_epi_uuid(epiUuidP)
     epi_uuid_t *	epiUuidP;
{
  epiUuidP->longField1 = ntohl(epiUuidP->longField1);
  epiUuidP->shortField1 = ntohs(epiUuidP->shortField1);
  epiUuidP->shortField2 = ntohs(epiUuidP->shortField2);
}

EXPORT void ntoh_epi_principal_id(epiPrinIdP)
     epi_principal_id_t *	epiPrinIdP;
{
#if defined(EPI_USE_FULL_ID)
  ntoh_epi_uuid(epiPrinIdP);
#else /* defined(EPI_USE_FULL_ID) */
  *epiPrinIdP = ntohl(*epiPrinIdP);
#endif /* defined(EPI_USE_FULL_ID) */
}

EXPORT void hton_epi_principal_id(epiPrinIdP)
     epi_principal_id_t *	epiPrinIdP;
{
#if defined(EPI_USE_FULL_ID)
  hton_epi_uuid(epiPrinIdP);
#else /* defined(EPI_USE_FULL_ID) */
  *epiPrinIdP = htonl(*epiPrinIdP);
#endif /* defined(EPI_USE_FULL_ID) */
}
