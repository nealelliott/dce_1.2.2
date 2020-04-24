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
 * $Log: namtbpvt.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:22  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:20:57  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:04 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:12:45  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:48:30  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:44:14  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:58:20  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:24:59  bfc]
 * 
 * Revision 1.1.4.2  1993/07/07  20:02:26  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:33:56  ganni]
 * 
 * $EndLog$
 */
/*
**  @OSF_COPYRIGHT@
**
**  Copyright (c) 1990 by
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
**      namtbpvt.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  This header file contains the private definitions necessary for the
**  nametable modules.
**
**  VERSION: DCE 1.0
**
*/

#ifndef NAMTBPVT_H
#define NAMTBPVT_H



/********************************************************************/
/*                                                                  */
/*   NAMTBPVT.H                                                     */
/*                                                                  */
/*              Data types private to the nametable routines.       */
/*                                                                  */
/********************************************************************/



/*
 *  We have a different (non-opaque) view of a NAMETABLE_id_t.
 */
typedef struct NAMETABLE_n_t * NAMETABLE_n_t_p;

typedef struct NAMETABLE_binding_n_t {
        int                              bindingLevel;
        char                            *theBinding;
        struct NAMETABLE_binding_n_t    *nextBindingThisLevel;
        struct NAMETABLE_binding_n_t    *oldBinding;
        NAMETABLE_n_t_p                  boundBy;
}
NAMETABLE_binding_n_t;


typedef struct NAMETABLE_n_t {
        struct NAMETABLE_n_t    *left;  /* Subtree with names less          */
        struct NAMETABLE_n_t    *right; /* Subtree with names greater       */
        struct NAMETABLE_n_t    *parent;/* Parent in the tree               */
                                        /* NULL if this is the root         */
        char                    *id;    /* The identifier string            */
        NAMETABLE_binding_n_t   *bindings;      /* The list of bindings known       */
                                                /* by this name at this time.       */
        NAMETABLE_binding_n_t   *tagBinding;    /* The structure known by this tag. */
}
NAMETABLE_n_t;

typedef struct NAMETABLE_temp_name_t {
        struct NAMETABLE_temp_name_t * next;  /* Next temp name chain block */
        NAMETABLE_n_t_p node;                 /* The temp name tree node    */
}
NAMETABLE_temp_name_t;


#endif /* NAMTBPVT_H */
