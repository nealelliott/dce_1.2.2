/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pkl.c,v $
 * Revision 1.1.7.2  1996/02/18  00:22:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:17  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:02:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:56  root]
 * 
 * Revision 1.1.5.2  1992/12/29  16:40:40  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:31:57  zeliff]
 * 
 * Revision 1.1.2.3  1992/07/15  20:52:25  sekhar
 * 	[CR4080] : Drop the fix as indicated below.
 * 	[1992/07/15  20:48:56  sekhar]
 * 
 * Revision 1.1.3.3  1992/07/15  17:31:43  burati
 * 	OT 4080 Fixed more syntax errors that showed up on little endian machines
 * 
 * Revision 1.1.3.2  1992/07/15  16:01:43  burati
 * 	OT 4080 Also need stubbase.h for ndr defs (which needs idlbase,rpc.h)
 * 
 * Revision 1.1.2.2  1992/07/10  19:31:54  dineen
 * 	OT 4080 fix: swap bytes in idl_pkl_get_type_uuid only when needed
 * 	[1992/07/08  20:09:43  dineen]
 * 
 * Revision 1.1  1992/01/19  14:41:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  pkl.c V=3 12/05/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca.
**
**   Copyright (c) Hewlett-Packard Company 1991
**   Unpublished work. All Rights Reserved.
**  
**
**  NAME:
**
**      pkl.c
**
**  FACILITY:
**
**      Pickling support
**
**  ABSTRACT:
**
**      routines for manipulating pickles.
**
**  
**
*/

#include <dce/idlbase.h>
#include <dce/rpc.h>
#include <dce/stubbase.h>
#include <dce/pkl.h>


#define swap_unsigned16(src, dst)\
    dst[0]=src[1];\
    dst[1]=src[0]

#define swap_unsigned32(src, dst)\
    dst[0]=src[3];\
    dst[1]=src[2];\
    dst[2]=src[1];\
    dst[3]=src[0]

#define swap_uuid(src, dst)\
    swap_unsigned32(src, dst);\
    swap_unsigned16((&src[4]),(&dst[4]));\
    swap_unsigned16((&src[6]),(&dst[6]));\
    {int i; for (i=8; i<16; i++) dst[i]=src[i];}


/*
 * i d l _ p k l _ g e t _ t y p e _ u u i d
 *
 * Extract a type uuid from (the header part of) an
 * idl_pkl_t.
 */
void idl_pkl_get_type_uuid
#ifndef __STDC__
    (pickle, type) 
    idl_pkl_t pickle;
    uuid_t *type;
#else
  (
    idl_pkl_t pickle,
    uuid_t *type
  )
#endif
{
#if NDR_LOCAL_INT_REP == ndr_c_int_big_endian
    *type = *(uuid_t *)&pickle[24];
#else
    swap_uuid((pickle+24), ((idl_byte *)type));
#endif
}

/*
 * i d l _ p k l _ g e t _ h e a d e r
 *
 * Extract a header structure (in local byte order) from (the header
 * part of) an idl_pkl_t.
 */
void idl_pkl_get_header
#ifndef __STDC__
    (pickle, version, length, syntax, type) 
    idl_pkl_t pickle;
    unsigned8 *version;
    unsigned32 *length;
    rpc_syntax_id_t *syntax;
    uuid_t *type;
#else
  (
    idl_pkl_t pickle,
    unsigned8 *version,
    unsigned32 *length,
    rpc_syntax_id_t *syntax,
    uuid_t *type
  )
#endif
{
#if NDR_LOCAL_INT_REP == ndr_c_int_big_endian
    *version = *(unsigned8 *)&pickle[0];
    *length = (*(unsigned32 *)&pickle[0]) & 0x00ffffff;
    *syntax = *(rpc_syntax_id_t *)&pickle[4];
    *type = *(uuid_t *)&pickle[24];
#else
    *version = *(unsigned8 *)&pickle[0];
    swap_unsigned32((pickle+0), ((idl_byte *)length));
    *length &= 0x00ffffff;
    swap_uuid((pickle+4), ((idl_byte *)&(syntax->id)));
    swap_unsigned32((pickle+20), ((idl_byte *)&(syntax->version)));
    swap_uuid((pickle+4), ((idl_byte *)&(syntax->id)));
    swap_uuid((pickle+24), ((idl_byte *)type));
#endif
}

/*
 * i d l _ p k l _ s e t _ h e a d e r
 *
 * Inject a header structure (in local byte order) into (the header
 * part of) an idl_pkl_t.
 */
void idl_pkl_set_header
#ifndef __STDC__
    (pickle, version, length, syntax, type) 
    idl_pkl_t pickle;
    unsigned32 version;
    unsigned32 length;
    rpc_syntax_id_t *syntax;
    uuid_t *type;
#else
  (
    idl_pkl_t pickle,
    unsigned32 version,
    unsigned32 length,
    rpc_syntax_id_t *syntax,
    uuid_t *type
  )
#endif
{
    version = version & 255;  /* mask to fit in 1 byte */

#if NDR_LOCAL_INT_REP == ndr_c_int_big_endian
    *(unsigned32 *)&pickle[0] = length;
    *(unsigned8 *)&pickle[0] = version;
    *(rpc_syntax_id_t *)&pickle[4] = *syntax;
    *(uuid_t *)&pickle[24] = *type;
#else
    swap_unsigned32(((idl_byte *)&length), (pickle+0));
    pickle[0] = version;
    swap_uuid(((idl_byte *)&syntax->id), (pickle+4));
    swap_unsigned32(((idl_byte *)&syntax->version), (pickle+20));
    swap_uuid(((idl_byte *)type), (pickle+24));
#endif
}

