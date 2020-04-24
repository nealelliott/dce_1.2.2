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
 * $Log: cspeldcl.c,v $
 * Revision 1.1.8.2  1996/02/18  18:51:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:04:56  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:15:19  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:02 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:10:36  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:47:36  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:43:08  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:55:58  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:23:48  bfc]
 * 
 * Revision 1.1.4.2  1993/07/07  19:57:04  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:30:45  ganni]
 * 
 * $EndLog$
 */
/*
**  @OSF_COPYRIGHT@
**
**  Copyright (c) 1989, 1990, 1991, 1992, 1993 by
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
**  NAME:
**
**      cspeldcl.c
**
**  FACILITY:
**
**      IDL Compiler Backend
**
**  ABSTRACT:
**
**  Routines to spell declaration related material to C source
**
**  VERSION: DCE 1.0
*/

#include <nidl.h>
#include <ast.h>
#include <bedeck.h>
#include <cspell.h>
#include <cspeldcl.h>
#include <hdgen.h>
#include <command.h>

/******************************************************************************/
/*                                                                            */
/*    Build text string for constant value                                    */
/*                                                                            */
/******************************************************************************/
void CSPELL_constant_val_to_string
#ifdef PROTO
(
    AST_constant_n_t *cp,
    char *str
)
#else
(cp, str)
    AST_constant_n_t *cp;
    char *str;
#endif
{
    char       *str2;

    switch (cp->kind) {
        case AST_nil_const_k:
            sprintf (str, "NULL");
            break;
        case AST_boolean_const_k:
            if (cp->value.boolean_val)
                sprintf (str, "ndr_true");
            else
                sprintf (str, "ndr_false");
            break;
        case AST_int_const_k:
            sprintf (str, "%ld", cp->value.int_val);
            break;
        case AST_string_const_k:
            STRTAB_str_to_string (cp->value.string_val, &str2);
            sprintf (str, "\"%s\"", str2);
            break;
        case AST_char_const_k:
            sprintf (str, "'%s'", mapchar(cp, FALSE));
            break;
        default:
            INTERNAL_ERROR("Unsupported tag in CSPELL_constant_val_to_string");
            break;
        }
}

/******************************************************************************/
/*                                                                            */
/*    Routine to spell constant to C source                                   */
/*                                                                            */
/******************************************************************************/
void CSPELL_constant_val
#ifdef PROTO
(
    FILE *fid,
    AST_constant_n_t *cp
)
#else
(fid, cp)
    FILE *fid;
    AST_constant_n_t *cp;
#endif
{
    char str[max_string_len];

    CSPELL_constant_val_to_string (cp, str);
    fprintf (fid, "%s", str);
}

/******************************************************************************/
/*                                                                            */
/*    Routine to spell union case label comment to C source                   */
/*                                                                            */
/******************************************************************************/
void CSPELL_labels
#ifdef PROTO
(
    FILE *fid,
    AST_case_label_n_t *clp
)
#else
(fid, clp)
    FILE  *fid;
    AST_case_label_n_t *clp;
#endif
{
    boolean first = true;

    fprintf (fid, "/* case(s): ");
    for (; clp; clp = clp->next) {
        if (first)
            first = false;
        else
            fprintf (fid, ", ");
        if (clp->default_label)
            fprintf (fid, "default");
        else
            CSPELL_constant_val (fid, clp->value);
        };
    fprintf (fid, " */\n");
}

/******************************************************************************/
/*                                                                            */
/*    Routine to spell function parameter list to C source                    */
/*                                                                            */
/******************************************************************************/
void CSPELL_parameter_list
#ifdef PROTO
(
    FILE *fid,
    AST_parameter_n_t *pp,
    boolean encoding_services   /* TRUE => [encode] or [decode] on operation */
)
#else
(fid, pp, encoding_services)
    FILE *fid;
    AST_parameter_n_t *pp;
    boolean encoding_services;
#endif
{
    boolean            first = true;

    if (pp)
    {
        for (; pp; pp = pp->next)
        {
            if (AST_HIDDEN_SET(pp))
            {
                /* Parameter does not appear in signature delivered to user */
                continue;
            }
            if (first)
            {
                first = false;
                if (encoding_services)
                {
                    /* First parameter is a pickling handle */
                    fprintf(fid, "    /* [in] */ idl_es_handle_t ");
                    if (pp->type->kind == AST_pointer_k)
                    {
                        /* Passed by reference */
                        fprintf(fid, "*");
                    }
                    spell_name(fid, pp->name);
                    continue;
                }
            }
            else
                fprintf (fid, ",\n");
            fprintf (fid, "    /* [");
            if (AST_IN_SET(pp))
            {
                fprintf(fid, "in");
                if (AST_OUT_SET(pp)) fprintf (fid, ", out");
            }
            else fprintf  (fid, "out");
            fprintf (fid, "] */ ");
#ifndef MIA
            if (pp->be_info.param)
                CSPELL_typed_name (fid, pp->type,
                    pp->be_info.param->name, NULL, false, true, false);
            else
#endif
                 CSPELL_typed_name (fid, pp->type, pp->name, NULL, false, true,
                                    false);
        }
    }
    else
        fprintf (fid, "    void");
}


/******************************************************************************/
/*                                                                            */
/*    Spell new and old style parameter lists                                 */
/*                                                                            */
/******************************************************************************/
void CSPELL_finish_synopsis
#ifdef PROTO
(
    FILE *fid,
    AST_parameter_n_t *paramlist
)
#else
(fid, paramlist)
    FILE *fid;
    AST_parameter_n_t *paramlist;
#endif
{
    AST_parameter_n_t *pp;

    /* C++ supports function prototypes */
    if ((int) CMD_vals[opt_lang] == opt_lang_cxx)
	return;
    fprintf (fid, "\n#ifndef IDL_PROTOTYPES\n");
    for (pp = paramlist; pp != NULL; pp = pp->next)
    {
        if (AST_HIDDEN_SET(pp))
        {
            /* Parameter does not appear in signature delivered to user */
            continue;
        }
	fprintf(fid,"    ");
            CSPELL_var_decl (fid, pp->type, pp->name);
    }
    fprintf (fid, "#endif\n");
}

