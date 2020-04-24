/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ifspemts.c,v $
 * Revision 1.1.8.2  1996/02/18  18:52:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:27  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:18:54  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:04 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:11:57  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:48:13  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:43:55  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:57:36  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:24:37  bfc]
 * 
 * Revision 1.1.4.1  1994/09/15  18:05:44  tom
 * 	Bug 12207 - Change some constants to be explicitly unsigned.
 * 	[1994/09/15  18:04:10  tom]
 * 
 * Revision 1.1.2.2  1993/07/07  19:59:48  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:32:45  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1989, 1990, 1991 by
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
**      ifspemts.c
**
**  FACILITY:
**
**      IDL Compiler Backend
**
**  ABSTRACT:
**
**  Routines for the emission of ifspecs, including MIA transfer syntaxes
**
**
*/

#include <nidl.h>
#include <ast.h>
#include <cspell.h>
#include <command.h>
#include <ifspec.h>
#include <bedeck.h>
#include <dutils.h>
#include <mtsbacke.h>

/******************************************************************************/
/*                                                                            */
/*    Return a pointer to the name of the if_spec                             */
/*                                                                            */
/******************************************************************************/
static char *BE_ifspec_name
#ifdef PROTO
(
    AST_interface_n_t *ifp,
    BE_output_k_t kind
)
#else
(ifp, kind)
    AST_interface_n_t *ifp;
    BE_output_k_t kind;
#endif
{
    static char retval[100];

    sprintf(retval, "%s_v%d_%d_%c_ifspec", BE_get_name(ifp->name),
            (ifp->version%65536), (ifp->version/65536),
            kind == BE_client_stub_k ? 'c' : 's');

    return retval;
}

/******************************************************************************/
/*                                                                            */
/*    Spell the manager epv to the output stream                              */
/*                                                                            */
/******************************************************************************/
void CSPELL_manager_epv
#ifdef PROTO
(
    FILE *fid,
    AST_interface_n_t *ifp
)
#else
( fid, ifp )
    FILE *fid;
    AST_interface_n_t *ifp;
#endif
{
    AST_export_n_t *p_export;
    boolean first_op = true;


    fprintf( fid, "\nstatic %s_v%d_%d_epv_t IDL_manager_epv = {\n",
           BE_get_name(ifp->name), (ifp->version%65536), (ifp->version/65536) );

    for( p_export = ifp->exports; p_export != NULL; p_export = p_export->next )
    {
        if ((p_export->kind == AST_operation_k)
           )
        {
            if ( ! first_op ) fprintf( fid, "," );
            if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx)
            {
                /*
                 * mepv not used by C++ support, but still convenient to have
                 * one, initialized to all zeroes.
                 */
                fprintf(fid, "0");
            }
            else
                spell_name( fid, p_export->thing_p.exported_operation->name );
            fprintf( fid, "\n" );
            first_op = false;
        }
    }

    fprintf( fid,"};\n" );
}


/******************************************************************************/
/*                                                                            */
/*    Spell an interface definition and related material to the output stream */
/*                                                                            */
/******************************************************************************/
void CSPELL_interface_def
#ifdef PROTO
(
    FILE *fid,
    AST_interface_n_t *ifp,
    BE_output_k_t kind,
    boolean generate_mepv
)
#else
(fid, ifp, kind, generate_mepv)
    FILE *fid;
    AST_interface_n_t *ifp;
    BE_output_k_t kind;
    boolean generate_mepv;
#endif
{
    boolean     first;
    long        i, endpoints;
    char *protseq, *portspec;

    if ((endpoints = ifp->number_of_ports) != 0)
    {
        first = true;
        fprintf(fid,
                "static rpc_endpoint_vector_elt_t IDL_endpoints[%d] = \n{",
                endpoints);
        for (i = 0; i < endpoints; i++)
        {
            STRTAB_str_to_string(ifp->protocol[i], &protseq);
            STRTAB_str_to_string(ifp->endpoints[i], &portspec);
            if (!first) fprintf(fid, ",\n");
            fprintf(fid,
               "{(unsigned_char_p_t)\"%s\", (unsigned_char_p_t)\"%s\"}",
                   protseq, portspec);
            first = false;
        }
        fprintf(fid, "};\n");
    }

    /* Transfer syntaxes */
    fprintf( fid, "\nstatic rpc_syntax_id_t IDL_transfer_syntaxes[%d] = {\n{\n",
                                                                 1 );
        fprintf(fid,
"{0x8a885d04u, 0x1ceb, 0x11c9, 0x9f, 0xe8, {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}},");
        fprintf(fid, "\n2}");
    fprintf(fid, "};\n");

    fprintf(fid, "\nstatic rpc_if_rep_t IDL_ifspec = {\n");
    fprintf(fid, "  1, /* ifspec rep version */\n");
    fprintf(fid, "  %d, /* op count */\n", ifp->op_count);
    fprintf(fid, "  %d, /* if version */\n", ifp->version);
    fprintf(fid, "  {");
#ifdef _VMS
    fprintf(fid, "0x%08.8xu, ", ifp->uuid.time_low);
#elif (defined(__alpha) && defined(__osf__))
    fprintf(fid, "0x%08.8xu, ", ifp->uuid.time_low);
#else
    fprintf(fid, "0x%08.8lxu, ", ifp->uuid.time_low);
#endif
    fprintf(fid, "0x%04.4x, ", ifp->uuid.time_mid);
    fprintf(fid, "0x%04.4x, ", ifp->uuid.time_hi_and_version);
    fprintf(fid, "0x%02.2x, ", ifp->uuid.clock_seq_hi_and_reserved);
    fprintf(fid, "0x%02.2x, ", ifp->uuid.clock_seq_low);
    fprintf(fid, "{");
    first = true;
    for (i = 0; i < 6; i++)
    {
        if (first)
            first = false;
        else
            fprintf (fid, ", ");
        fprintf (fid, "0x%x", ifp->uuid.node[i]);
    }
    fprintf(fid, "}},\n");
    fprintf(fid, "  2, /* stub/rt if version */\n");
    fprintf(fid, "  {%d, %s}, /* endpoint vector */\n", endpoints,
                 endpoints ? "IDL_endpoints" : "NULL");
    fprintf(fid, "  {%d, IDL_transfer_syntaxes} /* syntax vector */\n",
                                                                 1 );

    if (kind == BE_server_stub_k)
    {
        fprintf(fid, ",IDL_epva /* server_epv */\n");
        if (generate_mepv)
        {
            fprintf(fid,",(rpc_mgr_epv_t)&IDL_manager_epv /* manager epv */\n");
        }
        else
        {
            fprintf(fid,",NULL /* manager epv */\n");
        }
    }
    fprintf(fid, "};\n");
    fprintf(fid,
            "/* global */ rpc_if_handle_t %s = (rpc_if_handle_t)&IDL_ifspec;\n",
            BE_ifspec_name(ifp, kind));
}
