/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: files.h,v $
 * Revision 1.1.10.2  1996/02/18  23:45:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:09  marty]
 *
 * Revision 1.1.10.1  1995/12/07  22:17:30  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:03 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:11:27  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:48:01  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:43:39  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:57:02  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:24:21  bfc]
 * 
 * Revision 1.1.6.2  1993/07/07  19:58:47  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:31:59  ganni]
 * 
 * $EndLog$
 */
/*
**  @OSF_COPYRIGHT@
**
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME:
**
**      files.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Header file for file manipulation routines.
**
**  VERSION: DCE 1.0
**
*/

#ifndef files_incl
#define files_incl

#ifndef S_IFREG
#ifdef vms
#  include <types.h>
#  include <stat.h>
#else
#  include <sys/types.h>
#  include <sys/stat.h>
#endif
#endif

#include <nidl.h>               /* IDL common defs */
#include <nametbl.h>

typedef enum                    /* Filespec kinds: */
{
    file_dir,                   /* Directory */
    file_file,                  /* Regular ol' file */
    file_special                /* Something else */
} FILE_k_t;

extern boolean FILE_open(
#ifdef PROTO
    char *filespec,
    FILE **fid
#endif
);

extern boolean FILE_create(
#ifdef PROTO
    char *filespec,
    FILE **fid
#endif
);

extern boolean FILE_lookup(
#ifdef PROTO
    char        *filespec,
    char        **idir_list,
    struct stat *stat_buf,
    char        *lookup_spec
#endif
);

extern boolean FILE_form_filespec(
#ifdef PROTO
    char *in_filespec,
    char *dir,
    char *type,
    char *rel_filespec,
    char *out_filespec
#endif
);

extern int chdir(
#ifdef PROTO
    const char *
#endif
);

#ifdef VMS
/*
**  Default filespec; only good for one call to FILE_parse.
*/
extern char *FILE_def_filespec;

/* Some common system prototypes */
int SYS$PARSE();
int SYS$SEARCH();
int DECC$TO_VMS();
int LIB$SPAWN();
#endif

extern boolean FILE_parse(
#ifdef PROTO
    char *filespec,
    char *dir,
    char *name,
    char *type
#endif
);

extern boolean FILE_has_dir_info(
#ifdef PROTO
    char *filespec
#endif
);

extern boolean FILE_is_cwd(
#ifdef PROTO
    char *filespec
#endif
);

extern boolean FILE_kind(
#ifdef PROTO
    char        *filespec,
    FILE_k_t    *filekind
#endif
);

extern int FILE_execute_cmd(
#ifdef PROTO
    char        *cmd_string,
    char        *p1,
    char        *p2,
    long        msg_id
#endif
);

extern void FILE_delete(
#ifdef PROTO
    char        *filename
#endif
);

#endif /* files_incl */









