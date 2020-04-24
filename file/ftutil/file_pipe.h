/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: file_pipe.h,v $
 * Revision 1.1.18.1  1996/10/02  17:48:24  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:21  damon]
 *
 * Revision 1.1.13.2  1994/06/09  14:09:06  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:23:46  annie]
 * 
 * Revision 1.1.13.1  1994/02/04  20:19:30  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:31  devsrc]
 * 
 * Revision 1.1.11.2  1994/01/20  18:43:06  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:21  annie]
 * 
 * Revision 1.1.11.1  1993/12/07  17:25:28  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:04:52  jaffe]
 * 
 * $EndLog$
 */
/*
*/
/*
 * file_pipe.h
 *
 * (C) Copyright 1993 Transarc Corporation
 * All Rights Reserved.
 *
 */

#ifndef _TRANSARC_FILE_PIPE_H
#define _TRANSARC_FILE_PIPE_H

#include <stdio.h>
#include <dcedfs/pipe.h>


IMPORT int CreateFilePipe _TAKES((
    IN FILE*		fileStream,
    OUT afsPipe_t*	pipeP,
    IN unsigned		flags
));

#endif /* _TRANSARC_FILE_PIPE_H */
