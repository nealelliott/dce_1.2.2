/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nametbl.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:21  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:20:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:12:42  root]
 * 
 * Revision 1.1.4.2  1993/07/07  20:02:18  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:33:52  ganni]
 * 
 * $EndLog$
 */
/*
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
**  NAME
**
**      NAMETBL.H
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Header file for Name Table module, NAMETBL.C
**
**  VERSION: DCE 1.0
**
*/


#ifndef  nametable_incl
#define nametable_incl

/*
** IDL.H needs the definition of STRTAB_str_t, so put it first.
*/

typedef char * NAMETABLE_id_t;
#define NAMETABLE_NIL_ID ((NAMETABLE_id_t) NULL)

#ifdef MSDOS
typedef int  STRTAB_str_t ;
#define STRTAB_NULL_STR  ((STRTAB_str_t) 0)
#else
typedef NAMETABLE_id_t  STRTAB_str_t ;
#define STRTAB_NULL_STR  ((STRTAB_str_t) NULL)
#endif


#include <nidl.h>

#define NAMETABLE_id_too_long         1
#define NAMETABLE_no_space            2
#define NAMETABLE_different_casing    3
#define NAMETABLE_string_to_long      4
#define NAMETABLE_bad_id_len          5
#define NAMETABLE_bad_string_len      6

/*
 * This constant needs to be arbitrarily large since derived names added to
 * the nametable can get arbitrarily large, e.g. with nested structures.
 */
#define max_string_len                2048

NAMETABLE_id_t NAMETABLE_add_id(
#ifdef PROTO
    char *id
#endif
);

NAMETABLE_id_t NAMETABLE_lookup_id(
#ifdef PROTO
    char *id
#endif
);

void NAMETABLE_id_to_string(
#ifdef PROTO
    NAMETABLE_id_t NAMETABLE_id,
    char **str_ptr
#endif
);

boolean NAMETABLE_add_binding(
#ifdef PROTO
    NAMETABLE_id_t id,
    char * binding
#endif
);

char* NAMETABLE_lookup_binding(
#ifdef PROTO
    NAMETABLE_id_t identifier
#endif
);

boolean NAMETABLE_add_tag_binding(
#ifdef PROTO
    NAMETABLE_id_t id,
    char * binding
#endif
);

char* NAMETABLE_lookup_tag_binding(
#ifdef PROTO
    NAMETABLE_id_t identifier
#endif
);

char* NAMETABLE_lookup_local(
#ifdef PROTO
    NAMETABLE_id_t identifier
#endif
);

void  NAMETABLE_push_level(
#ifdef PROTO
    void
#endif
);

void  NAMETABLE_pop_level(
#ifdef PROTO
    void
#endif
);

void  NAMETABLE_set_temp_name_mode (
#ifdef PROTO
    void
#endif
);

void  NAMETABLE_set_perm_name_mode (
#ifdef PROTO
    void
#endif
);

void  NAMETABLE_clear_temp_name_mode (
#ifdef PROTO
    void
#endif
);

STRTAB_str_t   STRTAB_add_string(
#ifdef PROTO
    char *string
#endif
);

void  STRTAB_str_to_string(
#ifdef PROTO
    STRTAB_str_t str,
    char **strp
#endif
);

void  NAMETABLE_init(
#ifdef PROTO
    void
#endif
);

#ifdef DUMPERS
void  NAMETABLE_dump_tab(
#ifdef PROTO
    void
#endif
);

#endif
void  STRTAB_init(
#ifdef PROTO
    void
#endif
);

NAMETABLE_id_t NAMETABLE_add_derived_name(
#ifdef PROTO
    NAMETABLE_id_t id,
    char *matrix
#endif
);

NAMETABLE_id_t NAMETABLE_add_derived_name2(
#ifdef PROTO
    NAMETABLE_id_t id1,
    NAMETABLE_id_t id2,
    char *matrix
#endif
);



#endif
