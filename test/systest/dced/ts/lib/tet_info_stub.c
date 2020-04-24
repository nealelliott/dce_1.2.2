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
 * $Log: tet_info_stub.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:22  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:55:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:55:18  root]
 * 
 * Revision 1.1.2.3  1994/06/24  19:54:04  fadden
 * 	Merged with changes from 1.1.2.2
 * 	[1994/06/24  19:53:27  fadden]
 * 
 * 	Write to stdout. Remove "TET_INFOLINE:" leader
 * 	[1994/06/24  18:24:28  fadden]
 * 
 * Revision 1.1.2.2  1994/06/17  18:46:09  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:37:04  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/26  22:44:29  fadden
 * 	Prepare for initial submission
 * 	[1994/05/26  20:11:04  fadden]
 * 
 * $EndLog$
 */

#include <stdio.h>

void tet_infoline(char *msg)
{
  fprintf(stdout,"%s\n",msg);
}
