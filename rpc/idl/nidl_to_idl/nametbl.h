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
 * Revision 1.1.4.2  1996/02/18  23:46:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:50  marty]
 *
 * Revision 1.1.4.1  1995/12/07  22:36:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:18:15  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:15:00  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:48:07  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:15:50  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:11:17  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:00:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * ==========================================================================
 * Confidential and Proprietary.  Copyright 1987 by Apollo Computer Inc.,
 * Chelmsford, Massachusetts.  Unpublished -- All Rights Reserved Under
 * Copyright Laws Of The United States.
 *
 * Apollo Computer Inc. reserves all rights, title and interest with respect
 * to copying, modification or the distribution of such software programs and
 * associated documentation, except those rights specifically granted by Apollo
 * in a Product Software Program License, Source Code License or Commercial
 * License Agreement (APOLLO NETWORK COMPUTING SYSTEM) between Apollo and
 * Licensee.  Without such license agreements, such software programs may not
 * be used, copied, modified or distributed in source or object code form.
 * Further, the copyright notice must appear on the media, the supporting
 * documentation and packaging as set forth in such agreements.  Such License
 * Agreements do not grant any rights to use Apollo Computer's name or trademarks
 * in advertising or publicity, with respect to the distribution of the software
 * programs without the specific prior written permission of Apollo.  Trademark
 * agreements may be obtained in a separate Trademark License Agreement.
 * ==========================================================================
 */

#ifndef  nametable_incl
#define nametable_incl

#include <idl_base.h>

#define NAMETABLE_id_too_long         1
#define NAMETABLE_no_space            2
#define NAMETABLE_different_casing    3
#define NAMETABLE_string_to_long      4
#define NAMETABLE_bad_id_len          5
#define NAMETABLE_bad_string_len      6

#define max_string_len             256

typedef int  NAMETABLE_id_t  ;
typedef int  STRTAB_str_t ;

#define NAMETABLE_NIL_ID ((NAMETABLE_id_t) 0)

#ifndef __STDC__

NAMETABLE_id_t  NAMETABLE_add_id() ;

void            NAMETABLE_id_to_string() ;

STRTAB_str_t    STRTAB_add_string() ;

void            STRTAB_str_to_string() ;

void            NAMETABLE_dump_tab() ;

void            NAMETABLE_init() ;

boolean         NAMETABLE_add_binding() ;

char*           NAMETABLE_lookup_binding() ;

char*           NAMETABLE_lookup_local() ;

void            NAMETABLE_push_level();

void            NAMETABLE_pop_level();

NAMETABLE_id_t  NAMETABLE_add_derived_name();

NAMETABLE_id_t  NAMETABLE_add_derived_name2() ;

#else
NAMETABLE_id_t NAMETABLE_add_id(char *id, boolean upcase);

void           NAMETABLE_id_to_string(NAMETABLE_id_t NAMETABLE_id, char **str_ptr);

boolean        NAMETABLE_add_binding(NAMETABLE_id_t id, void *binding);

char*          NAMETABLE_lookup_binding(NAMETABLE_id_t identifier);

char*          NAMETABLE_lookup_local(NAMETABLE_id_t identifier);

void           NAMETABLE_push_level(void) ;

void           NAMETABLE_pop_level(void);

STRTAB_str_t   STRTAB_add_string(char *string);

void           STRTAB_str_to_string(STRTAB_str_t str, char **strp) ;

void           NAMETABLE_init(void) ;

void           NAMETABLE_dump_tab(void) ;

void           STRTAB_init(void) ;

NAMETABLE_id_t NAMETABLE_add_derived_name(NAMETABLE_id_t id, char *matrix);

NAMETABLE_id_t NAMETABLE_add_derived_name2(NAMETABLE_id_t id1, NAMETABLE_id_t id2, char *matrix);
#endif
#endif
