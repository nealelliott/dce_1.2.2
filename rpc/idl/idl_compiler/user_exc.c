/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: user_exc.c,v $
 * Revision 1.1.6.2  1996/02/18  18:52:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:53  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:22:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:13:16  root]
 * 
 * Revision 1.1.2.2  1993/07/07  20:04:09  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:35:02  ganni]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) Digital Equipment Corporation, 1993
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
**  NAME:
**
**      user_exc.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Code generation for user exceptions
**
**
*/

#include <ast.h>
#include <be_pvt.h>
#include <cspell.h>
#include <user_exc.h>

/******************************************************************************/
/*                                                                            */
/*  Declare user exceptions                                                   */
/*                                                                            */
/******************************************************************************/
static void DDBE_list_exceptions
#ifdef PROTO
(
    FILE *fid,                      /* [in] Handle for emitted C text */
    AST_interface_n_t *p_interface, /* [in] Pointer to AST interface node */
    int *p_num_declared_exceptions, /* [out] Number of declared exceptions */
    int *p_num_extern_exceptions    /* [out] Number of external exceptions */
)
#else
(fid, p_interface, p_num_declared_exceptions, p_num_extern_exceptions)
    FILE *fid;
    AST_interface_n_t *p_interface;
    int *p_num_declared_exceptions;
    int *p_num_extern_exceptions;
#endif
{
    AST_exception_n_t *p_exception;

    *p_num_declared_exceptions = 0;
    *p_num_extern_exceptions = 0;

    for (p_exception = p_interface->exceptions;
         p_exception != NULL;
         p_exception = p_exception->next)
    {
        if (AST_EXTERN_SET(p_exception))
        {
            fprintf(fid, "extern ");
            (*p_num_extern_exceptions)++;
        }
        else
            (*p_num_declared_exceptions)++;
        fprintf(fid, "EXCEPTION ");
        spell_name(fid, p_exception->name);
        fprintf(fid, ";\n");
    }
}

/******************************************************************************/
/*                                                                            */
/*  Spell code to initialize declared exceptions                              */
/*                                                                            */
/******************************************************************************/
static void DDBE_init_exceptions
#ifdef PROTO
(
    FILE *fid,                      /* [in] Handle for emitted C text */
    AST_interface_n_t *p_interface  /* [in] Pointer to AST interface node */
)
#else
(fid, p_interface)
    FILE *fid;
    AST_interface_n_t *p_interface;
#endif
{
    AST_exception_n_t *p_exception;

    fprintf(fid, "static void IDL_exceptions_init()\n{\n");
    for (p_exception = p_interface->exceptions;
         p_exception != NULL;
         p_exception = p_exception->next)
    {
        if ( ! AST_EXTERN_SET(p_exception))
        {
            fprintf(fid, "EXCEPTION_INIT(");
            spell_name(fid, p_exception->name);
            fprintf(fid, ");\n");
        }
    }
    fprintf(fid, "}\n");
    fprintf( fid,
"static RPC_SS_THREADS_ONCE_T IDL_exception_once = RPC_SS_THREADS_ONCE_INIT;\n"
             );
}

/******************************************************************************/
/*                                                                            */
/*  Spell an array of pointers to the user exceptions                         */
/*                                                                            */
/******************************************************************************/
static void DDBE_ref_exception_array
#ifdef PROTO
(
    FILE *fid,                      /* [in] Handle for emitted C text */
    AST_interface_n_t *p_interface  /* [in] Pointer to AST interface node */
)
#else
(fid, p_interface)
    FILE *fid;
    AST_interface_n_t *p_interface;
#endif
{
    AST_exception_n_t *p_exception;
    boolean first = true;

    fprintf(fid, "static EXCEPTION *IDL_exception_addresses[] = {\n");
    for (p_exception = p_interface->exceptions;
         p_exception != NULL;
         p_exception = p_exception->next)
    {
        if (first)
            first = false;
        else
            fprintf(fid, ",\n");
        fprintf(fid, "&");
        spell_name(fid, p_exception->name);
    }
    fprintf(fid, "};\n");
}

/******************************************************************************/
/*                                                                            */
/*  Declare user exception machinery at start of stub                         */
/*                                                                            */
/******************************************************************************/
void DDBE_user_exceptions
#ifdef PROTO
(
    FILE *fid,                      /* [in] Handle for emitted C text */
    AST_interface_n_t *p_interface, /* [in] Pointer to AST interface node */
    int *p_num_declared_exceptions, /* [out] Number of declared exceptions */
    int *p_num_extern_exceptions    /* [out] Number of external exceptions */
)
#else
(fid, p_interface, p_num_declared_exceptions, p_num_extern_exceptions)
    FILE *fid;
    AST_interface_n_t *p_interface;
    int *p_num_declared_exceptions;
    int *p_num_extern_exceptions;
#endif
{
    DDBE_list_exceptions(fid, p_interface, p_num_declared_exceptions,
                         p_num_extern_exceptions);
    if (*p_num_declared_exceptions != 0)
        DDBE_init_exceptions(fid, p_interface);
    else if (*p_num_extern_exceptions == 0)
    {
        /* No exception machinery to set up */
        return;
    }
    DDBE_ref_exception_array(fid, p_interface);
}
