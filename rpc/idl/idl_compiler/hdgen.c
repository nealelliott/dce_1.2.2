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
 * $Log: hdgen.c,v $
 * Revision 1.1.15.3  1996/02/18  18:51:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:22  marty]
 *
 * Revision 1.1.15.2  1996/02/07  17:48:08  parul
 * 	DCE 1.2.1 final drop from HP
 * 	[1996/02/07  16:13:55  parul]
 * 
 * 	DCE 1.2.1 final drop from HP
 * 
 * Revision 1.1.11.1  1994/05/02  22:43:32  tom
 * 	From DEC: Add binding callout function.
 * 	[1994/05/02  21:06:28  tom]
 * 
 * Revision 1.1.8.2  1993/10/14  12:35:50  hinxman
 * 	CR 8896 improve include file processing
 * 	[1993/10/14  12:35:30  hinxman]
 * 
 * Revision 1.1.8.1  1993/10/01  13:30:46  hinxman
 * 	OT 8874 - change design for spelling of #include generated from ACF include
 * 	statement
 * 	[1993/10/01  13:30:28  hinxman]
 * 
 * Revision 1.1.6.2  1993/07/07  19:59:23  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:32:26  ganni]
 * 
 * $EndLog$
 */
/* 08-May-92 harrow    Suppress IDL_DOUBLE_USED for [local] interfaces */
/*
**
**  Copyright (c) 1990, 1992, 1993 by
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
**      hdgen.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      BE_gen_c_header which emits the C header file for an interface.
**
**  VERSION: DCE 1.0
**
**  MODIFICATION HISTORY:
**
**  08-Jan-92 harrow      Enclose value of integer constants in parens when
**                        to prevent precedence problems when used.
**
*/

#include <nidl.h>
#include <ast.h>
#include <astp.h>
#include <cspell.h>
#include <cspeldcl.h>
#include <files.h>
#include <genpipes.h>
#include <hdgen.h>
#ifdef MIA
#include <command.h>
#include <mtsbacke.h>
#endif

extern int yylineno;


/* forward declarations */

void BE_gen_c_bindings(
#ifdef PROTO
    FILE *fid,
    AST_interface_n_t   *ifp
#endif
    );

void BE_gen_com_bindings(
#ifdef PROTO
    FILE *fid,
    AST_interface_n_t   *ifp
#endif
    );
    
/*
 * mapchar
 *
 * Maps a single character into a string suitable for emission
 */
char *mapchar
#ifdef PROTO
(
    AST_constant_n_t *cp,   /* Constant node with kind == AST_char_const_k */
    boolean warning_flag    /* unused */
)
#else
(cp, warning_flag)
    AST_constant_n_t *cp;   /* Constant node with kind == AST_char_const_k */
    boolean warning_flag;   /* unused */
#endif
{
    char c = cp->value.char_val;
    static char buf[10];

    switch (c)
    {
        case AUDIBLE_BELL: return AUDIBLE_BELL_CSTR;
        case '\b': return "\\b";
        case '\f': return "\\f";
        case '\n': return "\\n";
        case '\r': return "\\r";
        case '\t': return "\\t";
        case '\v': return "\\v";
        case '\\': return "\\\\";
        case '\'': return "\\\'";
        case '\"': return "\\\"";
        default:
            if (c >= ' ' && c <= '~')
                sprintf(buf, "%c", c);
            else
                sprintf(buf, "\\%03o", c);
            return buf;
    }
}

static void CSPELL_constant_def
#ifdef PROTO
(
    FILE *fid,
    AST_constant_n_t *cp,
    char *cast
)
#else
(fid, cp, cast)
    FILE *fid;
    AST_constant_n_t *cp;
    char *cast;
#endif
{
    char *s;

    fprintf (fid, "#define ");
    spell_name (fid, cp->name);
    fprintf (fid, " %s", cast);
    if (cp->defined_as != NULL)
        spell_name (fid, cp->defined_as->name);
    else
        switch (cp->kind) {
            case AST_nil_const_k:
                fprintf (fid, "NULL");
                break;
            case AST_int_const_k:
                fprintf (fid, "(%ld)", cp->value.int_val);
                break;
            case AST_hyper_int_const_k:
                fprintf (fid, "{%ld,%lu}",
                        cp->value.hyper_int_val.high,
                        cp->value.hyper_int_val.low);
                break;
            case AST_char_const_k:
                fprintf (fid, "'%s'", mapchar(cp, TRUE));
                break;
            case AST_string_const_k:
                STRTAB_str_to_string (cp->value.string_val, &s);
                fprintf (fid, "\"%s\"", s);
                break;
            case AST_boolean_const_k:
                fprintf (fid, "%s",
                         cp->value.boolean_val ? "ndr_true" : "ndr_false");
                break;
            default:
               INTERNAL_ERROR( "Unknown const type in CSPELL_constant_def" );
               break;
        }
    fprintf (fid, "\n");
}



static void CSPELL_operation_def
#ifdef PROTO
(
    FILE *fid,
    AST_operation_n_t *op
)
#else
(fid, op)
    FILE *fid;
    AST_operation_n_t *op;
#endif
{
    AST_type_n_t       func_type_node;

    func_type_node = *BE_function_p;
    func_type_node.type_structure.function = op;

    fprintf (fid, "extern ");
    CSPELL_typed_name (fid, &func_type_node, op->name, NULL, false, true,
                      (boolean)(AST_ENCODE_SET(op) || AST_DECODE_SET(op)));
    fprintf (fid, ";\n");
}


void CSPELL_type_def
#ifdef PROTO
(
    FILE *fid,
    AST_type_n_t *tp,
    boolean spell_tag
)
#else
(fid, tp, spell_tag)
    FILE *fid;
    AST_type_n_t *tp;
    boolean spell_tag;
#endif
{
    fprintf (fid, "typedef ");

    CSPELL_typed_name (fid, tp, tp->name, tp, false, spell_tag, false);
    fprintf (fid, ";\n");

    /* declare the "bind" and "unbind" routines as extern's for [handle] types */
    if (AST_HANDLE_SET(tp) && !((tp->kind == AST_handle_k)
                                && (tp->xmit_as_type == NULL)))
    {
        fprintf (fid, "handle_t IDL_ENTRY ");
        spell_name (fid, tp->name);
        fprintf (fid, "_bind(\n#ifdef IDL_PROTOTYPES\n");
        spell_name (fid, tp->name);
        fprintf (fid, " h\n#endif\n);\n");

        fprintf (fid, "void IDL_ENTRY ");
        spell_name (fid, tp->name);
        fprintf (fid, "_unbind(\n#ifdef IDL_PROTOTYPES\n");
        spell_name (fid, tp->name);
        fprintf (fid, " uh,\nhandle_t h\n#endif\n);\n");
    }
}


static void CSPELL_exports
#ifdef PROTO
(
    FILE *fid,
    AST_export_n_t *ep
)
#else
(fid, ep)
    FILE           *fid;
    AST_export_n_t *ep;
#endif
{
    for (; ep; ep = ep->next) {
        switch (ep->kind) {
            case AST_constant_k:
                CSPELL_constant_def (fid, ep->thing_p.exported_constant, "");
                break;
            case AST_operation_k:
                    if ((int)CMD_vals[opt_lang] != opt_lang_cxx)
                        CSPELL_operation_def(fid, ep->thing_p.exported_operation);
                break;
            case AST_type_k:
                CSPELL_type_def (fid, ep->thing_p.exported_type, true);
                break;
            default:
               INTERNAL_ERROR( "Unknown export type in CSPELL_exports" );
               break;
            }
        }
}

static void CSPELL_epv_field
#ifdef PROTO
(
    FILE *fid,
    AST_operation_n_t *op
)
#else
(fid, op)
    FILE   *fid;
    AST_operation_n_t *op;
#endif
{
    AST_type_n_t       type_node_a, type_node_b;
    AST_pointer_n_t    pointer_node;

    type_node_a = *BE_pointer_p;
    type_node_a.type_structure.pointer = &pointer_node;

    type_node_b = *BE_function_p;
    type_node_b.type_structure.function = op;

    pointer_node.fe_info = NULL;
    pointer_node.be_info.other = NULL;
    pointer_node.pointee_type = &type_node_b;

    CSPELL_typed_name (fid, &type_node_a, op->name, NULL, false, true,
                      (boolean)(AST_ENCODE_SET(op) || AST_DECODE_SET(op)));
    fprintf (fid, ";\n");
}


static void CSPELL_epv_type_and_var
#ifdef PROTO
(
    FILE *fid,
    NAMETABLE_id_t if_name,
    unsigned long int if_version,
    AST_export_n_t *ep,
    boolean declare_cepv
)
#else
(fid, if_name, if_version, ep, declare_cepv)
    FILE *fid;
    NAMETABLE_id_t if_name;
    unsigned long int if_version;
    AST_export_n_t *ep;
    boolean declare_cepv;
#endif
{

    /*  emit the declaration of the client/manager EPV type
        and, conditional on declare_cepv, an extern declaration
        for the client EPV
    */
    AST_operation_n_t *op;

    fprintf (fid, "typedef struct ");
    spell_name (fid, if_name);
    fprintf (fid, "_v%d_%d_epv_t {\n", (if_version%65536), (if_version/65536));
    for (; ep; ep = ep->next)
        if ((ep->kind == AST_operation_k)
           ) 
        {
            op = ep->thing_p.exported_operation;
            CSPELL_epv_field (fid, op);
        }
    fprintf (fid, "} ");
    spell_name (fid, if_name);
    fprintf (fid, "_v%d_%d_epv_t;\n", (if_version%65536), (if_version/65536));

    if (declare_cepv) {
        fprintf(fid, "extern ");
        spell_name(fid, if_name);
        fprintf(fid, "_v%d_%d_epv_t ", if_version%65536, if_version/65536);
        spell_name(fid, if_name);
        fprintf(fid, "_v%d_%d_c_epv;\n", if_version%65536, if_version/65536);
    }
}

static void CSPELL_if_spec_refs
#ifdef PROTO
(
    FILE *fid,
    NAMETABLE_id_t if_name,
    unsigned long int if_version
)
#else
(fid, if_name, if_version)
    FILE *fid;
    NAMETABLE_id_t if_name;
    unsigned long int if_version;
#endif
{
    fprintf (fid, "extern rpc_if_handle_t ");
    spell_name (fid, if_name);
    fprintf (fid, "_v%d_%d_c_ifspec;\n",(if_version%65536),(if_version/65536));

    fprintf (fid, "extern rpc_if_handle_t ");
    spell_name (fid, if_name);
    fprintf (fid, "_v%d_%d_s_ifspec;\n",(if_version%65536),(if_version/65536));
}

static void CSPELL_user_prototypes
#ifdef PROTO
(
    FILE *fid,
    AST_interface_n_t *ifp
)
#else
(fid, ifp)
    FILE *fid;
    AST_interface_n_t *ifp;
#endif
{
    AST_export_n_t *ep;
    AST_type_p_n_t *tpp;
    AST_type_n_t *tp;

    /*
     * declare context handle rundown routines
     */
    for (ep = ifp->exports; ep; ep = ep->next)
    {
        if (ep->kind != AST_type_k)
            continue;
        tp = ep->thing_p.exported_type;
        if (!AST_CONTEXT_RD_SET(tp))
            continue;

        fprintf(fid, "void IDL_ENTRY ");
        spell_name(fid, tp->name);
        fprintf(fid, "_rundown(\n#ifdef IDL_PROTOTYPES\n");
        fprintf(fid, "    rpc_ss_context_t context_handle\n#endif\n);\n");
    }

    /*
     * declare the "from_xmit", "to_xmit", "free_xmit", and "free_inst"
     * routines as extern's for types with the [transmit_as()] attribute
     */
    for (ep = ifp->exports; ep; ep = ep->next)
    {
        if (ep->kind != AST_type_k)
            continue;
        tp = ep->thing_p.exported_type;
        if (tp->xmit_as_type == NULL)
            continue;

        fprintf (fid, "void IDL_ENTRY ");
        spell_name (fid, tp->name);
        fprintf (fid, "_from_xmit(\n#ifdef IDL_PROTOTYPES\n");
        CSPELL_type_exp_simple (fid, tp->xmit_as_type);
        fprintf (fid, " *xmit_object,\n");
        spell_name (fid, tp->name);
        fprintf (fid, " *object\n#endif\n);\n");

        fprintf (fid, "void IDL_ENTRY ");
        spell_name (fid, tp->name);
        fprintf (fid, "_to_xmit(\n#ifdef IDL_PROTOTYPES\n");
        spell_name (fid, tp->name);
        fprintf (fid, " *object,\n");
        CSPELL_type_exp_simple (fid, tp->xmit_as_type);
        fprintf (fid, " **xmit_object\n#endif\n);\n");

        fprintf (fid, "void IDL_ENTRY ");
        spell_name (fid, tp->name);
        fprintf (fid, "_free_inst(\n#ifdef IDL_PROTOTYPES\n");
        spell_name (fid, tp->name);
        fprintf (fid, " *object\n#endif\n);\n");

        fprintf (fid, "void IDL_ENTRY ");
        spell_name (fid, tp->name);
        fprintf (fid, "_free_xmit(\n#ifdef IDL_PROTOTYPES\n");
        CSPELL_type_exp_simple (fid, tp->xmit_as_type);
        fprintf (fid, " *xmit_object\n#endif\n);\n");
    }

    /*
     * declare the "from_local", "to_local", "free_local", and
     * "free" routines as extern's for types with the [represent_as()]
     * attribute
     */
    for (tpp = ifp->ra_types; tpp; tpp = tpp->next)
    {
        tp = tpp->type;

        fprintf (fid, "void IDL_ENTRY ");
        spell_name (fid, tp->name);
        fprintf (fid, "_from_local(\n#ifdef IDL_PROTOTYPES\n");
        spell_name (fid, tp->rep_as_type->type_name);
        fprintf (fid, " *local_object,\n");
        spell_name (fid, tp->name);
        fprintf (fid, " **net_object\n#endif\n);\n");

        fprintf (fid, "void IDL_ENTRY ");
        spell_name (fid, tp->name);
        fprintf (fid, "_to_local(\n#ifdef IDL_PROTOTYPES\n");
        spell_name (fid, tp->name);
        fprintf (fid, " *net_object,\n");
        spell_name (fid, tp->rep_as_type->type_name);
        fprintf (fid, " *local_object\n#endif\n);\n");

        fprintf (fid, "void IDL_ENTRY ");
        spell_name (fid, tp->name);
        fprintf (fid, "_free_local(\n#ifdef IDL_PROTOTYPES\n");
        spell_name (fid, tp->rep_as_type->type_name);
        fprintf (fid, " *local_object\n#endif\n);\n");

        fprintf (fid, "void IDL_ENTRY ");
        spell_name (fid, tp->name);
        fprintf (fid, "_free_inst(\n#ifdef IDL_PROTOTYPES\n");
        spell_name (fid, tp->name);
        fprintf (fid, " *net_object\n#endif\n);\n");
    }

    /*
     * Declare the binding handle callout routine specified with the
     * [binding_callout] interface attribute, if any.
     */
    if (ifp->binding_callout_name != NAMETABLE_NIL_ID)
    {
        char *callout_name;
        NAMETABLE_id_to_string(ifp->binding_callout_name, &callout_name);
        /* Don't emit proto for canned routines declared in stubbase.h */
        if (strncmp(callout_name, "rpc_ss_bind_", 12/*prefix len*/) != 0)
        {
            fprintf(fid, "void ");
            spell_name(fid, ifp->binding_callout_name);
            fprintf(fid, "(\n#ifdef IDL_PROTOTYPES\n");
            fprintf(fid, "rpc_binding_handle_t *p_binding,\n");
            fprintf(fid, "rpc_if_handle_t interface_handle,\n");
            fprintf(fid, "error_status_t *p_st\n#endif\n);\n");
        }
    }
}

/*
 *  Spell "extern" statements for user exceptions
 */
void BE_spell_extern_user_excs
#ifdef PROTO
(
    FILE *fid,              /* Handle for emitted C text */
    AST_interface_n_t *ifp /* Ptr to AST interface node */
)
#else
(fid, ifp)
FILE                *fid;
AST_interface_n_t   *ifp;
#endif
{
    AST_exception_n_t *p_exception;

    if (ifp->exceptions == NULL)
    {
        /* There are no user exceptions */
        return;
    }

    /* There is at least one user exception, so drag in some exception
        handling machinery */
#ifdef ultrix
    fprintf (fid, INCLUDE_TEMPLATE, "cma.h");
#endif
    fprintf (fid, INCLUDE_TEMPLATE, "rpcexc.h");

    for (p_exception = ifp->exceptions;
         p_exception != NULL;
         p_exception = p_exception->next)
    {
        fprintf(fid, "extern EXCEPTION ");
        spell_name(fid, p_exception->name);
        fprintf(fid, ";\n");
    }
}

/*
 *  Spell prototypes for I-char machinery
 */
static void BE_spell_ichar_prototypes
#ifdef PROTO
(
    FILE *fid,              /* Handle for emitted C text */
    AST_interface_n_t *ifp  /* Ptr to AST interface node */
)
#else
(fid, ifp)
FILE                *fid;
AST_interface_n_t   *ifp;
#endif
{
    AST_type_p_n_t *cstpp; /* Pointer to chain of [cs_char] types */
    AST_type_n_t *cstp;     /* Pointer to [cs_char] type */
    AST_name_n_t *rnp;      /* Pointer to [cs_tag_rtn] name */

    for (cstpp = ifp->cs_types; cstpp != NULL; cstpp = cstpp->next)
    {
        cstp = cstpp->type;

        fprintf(fid, "void IDL_ENTRY ");
        spell_name(fid, cstp->cs_char_type->type_name);
        fprintf(fid, "_net_size(\n#ifdef IDL_PROTOTYPES\n");
        fprintf(fid, "rpc_binding_handle_t h,\n");
        fprintf(fid, "idl_ulong_int tag,\n");
        fprintf(fid, "idl_ulong_int l_storage_len,\n");
        fprintf(fid, "idl_cs_convert_t *p_convert_type,\n");
        fprintf(fid, "idl_ulong_int *p_w_storage_len,\n");
        fprintf(fid, "error_status_t *p_st\n#endif\n);\n");

        fprintf(fid, "void IDL_ENTRY ");
        spell_name(fid, cstp->cs_char_type->type_name);
        fprintf(fid, "_to_netcs(\n#ifdef IDL_PROTOTYPES\n");
        fprintf(fid, "rpc_binding_handle_t h,\n");
        fprintf(fid, "idl_ulong_int tag,\n");
        spell_name(fid, cstp->cs_char_type->type_name);
        fprintf(fid, " *ldata,\n");
        fprintf(fid, "idl_ulong_int l_data_len,\n");
        spell_name(fid, cstp->name);
        fprintf(fid, " *wdata,\n");
        fprintf(fid, "idl_ulong_int *p_w_data_len,\n");
        fprintf(fid, "error_status_t *p_st\n#endif\n);\n");

        fprintf(fid, "void IDL_ENTRY ");
        spell_name(fid, cstp->cs_char_type->type_name);
        fprintf(fid, "_local_size(\n#ifdef IDL_PROTOTYPES\n");
        fprintf(fid, "rpc_binding_handle_t h,\n");
        fprintf(fid, "idl_ulong_int tag,\n");
        fprintf(fid, "idl_ulong_int w_storage_len,\n");
        fprintf(fid, "idl_cs_convert_t *p_convert_type,\n");
        fprintf(fid, "idl_ulong_int *p_l_storage_len,\n");
        fprintf(fid, "error_status_t *p_st\n#endif\n);\n");

        fprintf(fid, "void IDL_ENTRY ");
        spell_name(fid, cstp->cs_char_type->type_name);
        fprintf(fid, "_from_netcs(\n#ifdef IDL_PROTOTYPES\n");
        fprintf(fid, "rpc_binding_handle_t h,\n");
        fprintf(fid, "idl_ulong_int tag,\n");
        spell_name(fid, cstp->name);
        fprintf(fid, " *wdata,\n");
        fprintf(fid, "idl_ulong_int w_data_len,\n");
        fprintf(fid, "idl_ulong_int l_storage_len,\n");
        spell_name(fid, cstp->cs_char_type->type_name);
        fprintf(fid, " *ldata,\n");
        fprintf(fid, "idl_ulong_int *p_l_data_len,\n");
        fprintf(fid, "error_status_t *p_st\n#endif\n);\n");
    }

    for (rnp = ifp->cs_tag_rtns; rnp != NULL; rnp = rnp->next)
    {
        fprintf(fid, "void IDL_ENTRY ");
        spell_name(fid, rnp->name);
        fprintf(fid, "(\n#ifdef IDL_PROTOTYPES\n");
        fprintf(fid, "rpc_binding_handle_t h,\n");
        fprintf(fid, "idl_boolean server_side,\n");
        fprintf(fid, "idl_ulong_int *p_stag,\n");
        fprintf(fid, "idl_ulong_int *p_drtag,\n");
        fprintf(fid, "idl_ulong_int *p_rtag,\n");
        fprintf(fid, "error_status_t *p_st\n#endif\n);\n");
    }
}

/*
 *  Generate C header file
 */


int BE_is_handle_param(
    AST_parameter_n_t *p
    )
{
    if (p == NULL) return FALSE;
    if (p->type == NULL) return FALSE;
    if (p->type->kind == AST_handle_k) return TRUE;
    if ((p->type->kind == AST_pointer_k) &&
	(p->type->type_structure.pointer->pointee_type->kind == AST_handle_k)) return TRUE;
    return FALSE;
}


void BE_gen_cxx_class(
    FILE *fid,
    AST_interface_n_t   *ifp,
    class_t c    
    )
{    
    AST_export_n_t    *ep;
    AST_parameter_n_t *pp;
    char	      *class_suffix;
    char	      *class_comment;
    char	      class_parent[500];
    char	      *if_name;
    boolean           first;
    int               i;
    char              include_var_name[max_string_len];

    class_parent[0] = '\0';

    switch (c) {
    case base_class:
        class_suffix = "";
        class_comment = "RPC Abstract Interface Class";
            strcpy(class_parent, ": ");
            if (ifp->inherited_interface_name)
	    {
                strcat(class_parent, " public ");
                strcat(class_parent,
                    BE_get_name(ifp->inherited_interface_name));
                strcat(class_parent, ", ");
            }
            strcat(class_parent,"public virtual ");
            strcat(class_parent, "rpc_object_reference");
	break;

    case stub_class:
        class_suffix = "Proxy";
        class_comment = "Proxy Class for RPC Interface Class";
        strcpy(class_parent,": public ");
        strcat(class_parent,BE_get_name(ifp->name));
	break;
    case mgr_class:
        class_suffix = "Mgr";
        class_comment = "Manager Class for RPC Interface Class";
        strcpy(class_parent,": public ");
        strcat(class_parent,BE_get_name(ifp->name));
	break;

    default:
        break;
    };  /* switch */

    fprintf (fid, "\n\n/******* %s *******/\n",class_comment);
    if (c == base_class) {
        fprintf (fid, "const uuid_t %sIID = \n", BE_get_name(ifp->name));
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
        fprintf(fid, "}};\n\n");
    }

    if (c == mgr_class) {
    	sprintf (include_var_name, "%s%s_v%d_%d_included",
		BE_get_name(ifp->name), class_suffix,
		(ifp->version%65536), (ifp->version/65536));
    	fprintf (fid, "#ifndef %s\n#define %s\n",
             include_var_name, include_var_name);
	fprintf (fid, "#include \"%s\"\n\n", (char *) BE_cmd_val[opt_header]);
    }



    fprintf (fid, "class %s%s %s {\n", BE_get_name(ifp->name), class_suffix, class_parent);

    if (c == mgr_class) {
	if (ifp->cxx_delegate_name != NAMETABLE_NIL_ID) {
            fprintf (fid, "private:\n");
            fprintf (fid,"    %s *delegate;\n\n",BE_get_name(ifp->cxx_delegate_name));
	}
    }
    fprintf (fid, "public:\n\n");

      if (c == base_class) {
	/* add the abstract class constructors for inheritance reasons */
        fprintf (fid, "    %s(ObjectRef &oref)\n", BE_get_name(ifp->name));
        if (ifp->inherited_interface_name) {
        	fprintf (fid, ": %s(oref)", 
	            BE_get_name(ifp->inherited_interface_name));
	}
        fprintf (fid, "    {}\n");
       
        fprintf (fid, "    %s() {\n", BE_get_name(ifp->name));
        fprintf (fid, "    rpc_register_interface(this,%sIID);\n",
		BE_get_name(ifp->name));

        fprintf (fid, "  };\n");
      }

    /* generate the proxy class constructor */
    if (c == stub_class) {
        fprintf (fid, "    %s%s (ObjectRef &oref) ",
            BE_get_name(ifp->name), class_suffix);
            fprintf (fid, " : rpc_object_reference(oref), %s(oref)\n",
		BE_get_name(ifp->name));
            fprintf (fid, " {};\n\n");
    }

    /*
    ** generate the proxy creator routine called by the marshalling interpreter
    */
    if (c == stub_class) {
        fprintf (fid, "    static %s *createProxy(ObjectRef &oref) {\n",
            BE_get_name(ifp->name));
        fprintf (fid, "        return new %sProxy(oref);\n    };\n",
            BE_get_name(ifp->name));
    }

    if (c == mgr_class) {
	if (ifp->cxx_delegate_name != NAMETABLE_NIL_ID) 
            fprintf (fid, "%sMgr (%s *d) { delegate = d; }\n\n",
		BE_get_name(ifp->name), BE_get_name(ifp->cxx_delegate_name));
    }

    /* spell a virtual destructor function in the base and manager classes */
        if (c == base_class) {
            fprintf (fid, "     virtual ~%s (void) {};\n\n",
                BE_get_name(ifp->name));
	} else if (c == mgr_class) {
            fprintf (fid, "virtual ~%s%s (void) {};\n\n",
                BE_get_name(ifp->name), class_suffix);
	}

    /* generate the idl specified operations into the class */
    for (ep = ifp->exports; ep; ep = ep->next) {
	if ((ep->kind == AST_operation_k) 
           ) 
	{
	    AST_operation_n_t  *op = ep->thing_p.exported_operation;
	    AST_type_n_t       func_type_node;
	    NAMETABLE_id_t     method_name;
	    AST_parameter_n_t *handle_param;

	    /* Setup a function type for the spellers */
	    func_type_node = *BE_function_p;
	    func_type_node.type_structure.function = op;

	    /* 
	    ** Check for the operation having a constructor/static attribute 
	    */
	    if (IS_CONSTRUCTOR(op) || AST_STATIC_SET(op))
	    {
                /*
                ** Don't generate constructors and static operations
                ** in stub/manager class as they can't be inherited
                */
                if (c == stub_class || c == mgr_class) continue;
                /*
                ** Constructors/Statics are generated as static methods
                */
                fprintf(fid, "    static ");
                CSPELL_typed_name (fid, &func_type_node, op->name, 
                    BE_ulong_int_p, false, true, false);
                fprintf(fid, ";\n");
                continue;
	    }
	    /* Standard member function */
	    else 
		method_name = op->name;

	    /* For constructor/destructor functions, suppress a return type */
            fprintf(fid, "virtual ");

	    /*
	    ** Suppress handle_t parameters for non-constructor 
	    ** or static member functions
	    */
	    handle_param = NULL;
	    if (BE_is_handle_param(op->parameters)) {
                handle_param = op->parameters;
                op->parameters = op->parameters->next;
            }

	    CSPELL_typed_name (fid, &func_type_node, method_name, 
			       BE_ulong_int_p, false, true, false);

	    if (handle_param != NULL) {
                op->parameters = handle_param;
            }
	    
	    /*
	    ** Base class methods are pure virtual 
	    **
	    */
	    if (c == base_class)
                fprintf(fid, " = 0");
	    /*
	    ** Manager class methods have null bodies
	    **
	    */
            if (c == mgr_class) {
                if (op->result->type->kind == AST_void_k) {
                    if (ifp->cxx_delegate_name == NAMETABLE_NIL_ID){
                        fprintf(fid, "{\n    return;\n}");
                    } else {
                        fprintf(fid, "{\ndelegate->%s(",
                            BE_get_name(method_name));
                        first = true;
                        for (pp = op->parameters; pp; pp = pp->next)
                        {
                            if (AST_HIDDEN_SET(pp))
                                continue;
                            if (pp->type->kind != AST_handle_k) {
                                if (first)
                                    first = false;
                                else
                                    fprintf(fid, ",");
                                fprintf(fid, "%s", BE_get_name(pp->name));
                            }
                        }
                        fprintf(fid, ");\nreturn;\n}");
                    }
                } else if (op->result->type->rep_as_type) {
                    fprintf(fid, "{\nreturn %s();\n}",
                        BE_get_name(op->result->type->rep_as_type->type_name));
                } else {
                    if (ifp->cxx_delegate_name == NAMETABLE_NIL_ID){
                        /* cast return type */
                        fprintf(fid, "{\n    return (");
                        CSPELL_typed_name(fid, op->result->type,
                            NULL, NULL, false, false, false);
                        fprintf(fid, ") 0;\n}");
                    } else {
                        fprintf(fid, "{\nreturn delegate->%s(",
                            BE_get_name(method_name));
                        first = true;
                        for (pp = op->parameters; pp; pp = pp->next)
                        {
                            if (AST_HIDDEN_SET(pp))
                                continue;
                            if (pp->type->kind != AST_handle_k) {
                                if (first)
                                    first = false;
                                else
                                    fprintf(fid, ",");
                                fprintf(fid, "%s", BE_get_name(pp->name));
                            }    /* if AST_handle_k */
			}   /* for */   
                        fprintf(fid, ");\n}");
                    }   /* else */
                }   /* else */
            }   /* if mgr_class */
            fprintf(fid, ";\n\n");
        } /* if */
    } /* for */

    /* generate the bind operations into the class */
    if (c == base_class) {
    	    fprintf (fid, "     static %s * bind(unsigned_char_t *name) {\n",
		BE_get_name(ifp->name));
	    fprintf(fid, "        return (%s *) setBindInfo((unsigned char *) name, %sIID);\n",
		    BE_get_name(ifp->name), BE_get_name(ifp->name));
	    fprintf(fid, "   }\n");

    	    fprintf (fid, "     static %s * bind(uuid_t & u) {\n",
		BE_get_name(ifp->name));
	    fprintf(fid, "        return (%s *) setBindInfo(u, %sIID);\n",
		    BE_get_name(ifp->name), BE_get_name(ifp->name));
	    fprintf(fid, "   }\n");

    	    fprintf (fid, "     static %s * bind(rpc_binding_handle_t bh) {\n",
		BE_get_name(ifp->name));
	    fprintf(fid, "        return (%s *) setBindInfo(bh, %sIID);\n",
		    BE_get_name(ifp->name), BE_get_name(ifp->name));
	    fprintf(fid, "   }\n");

    	    fprintf (fid, "     static %s * bind(rpc_object_reference *base) {\n",
		BE_get_name(ifp->name));

	    fprintf(fid, "        if (!base) return 0;\n");
	    fprintf(fid, "        return (%s *) setBindInfo(base, %sIID);\n",
		    BE_get_name(ifp->name), BE_get_name(ifp->name));
	    fprintf(fid, "   }\n");

    	    fprintf (fid, "     void register_named_object(unsigned_char_t *name,\n");
    	    fprintf (fid, "	                          boolean32 registerObject = TRUE) {\n");
    	    fprintf (fid, "	                rpc_object_reference::registerObject(\n");
    	    fprintf (fid, "	                         this,\n");
    	    fprintf (fid, "	                         (unsigned char *) name,\n");
	    NAMETABLE_id_to_string(ifp->name, &if_name);
    	    fprintf (fid, "	                         %sIID,\n", if_name);
    	    fprintf (fid, "	                         registerObject);\n");
    	    fprintf (fid, "     }\n\n");
    }

    fprintf (fid, "\n};\n");

    if (c == mgr_class)
            fprintf (fid, "\n#endif\n");
}

void BE_gen_c_header
#ifdef PROTO
(
    FILE *fid,              /* Handle for emitted C text */
    AST_interface_n_t *ifp, /* Ptr to AST interface node */
    boolean bugs[],         /* List of backward compatibility "bugs" */
    boolean cepv_opt        /* -cepv option present */
)
#else
( fid, ifp, bugs, cepv_opt)
FILE                *fid;
AST_interface_n_t   *ifp;
boolean             bugs[];
boolean             cepv_opt;
#endif
{
    AST_import_n_t    *impp;
    AST_include_n_t   *incp;
    char        include_var_name[max_string_len];
    char        *fn_str, *if_name;
    char	*header_suffix;


    if ((int)CMD_vals[opt_lang] == opt_lang_cxx)
    {
	fprintf(fid,"\n#ifdef __cplusplus\n");
	fprintf(fid, "    class %s;\n    class %s%s;\n",
		BE_get_name(ifp->name), BE_get_name(ifp->name), "Proxy");
	/* changed to binding handle for C binding for now - viv */
	fprintf(fid, "#else\n    typedef struct %s %s;\n#endif\n\n",
		BE_get_name(ifp->name), BE_get_name(ifp->name));

        /* This was fixed to only define IDL_ENTRY if it isn't already.
         * The generated header file includes idlbase.h, which defines
         * it correctly based upon platform.  This fixes the problem 
         * when idlbase.h may have already previously been included then 
         * IDL_ENTRY doesn't get defined to be nothing here!
         */

        fprintf(fid, "#ifndef IDL_ENTRY\n"); 
    	fprintf(fid, "#define IDL_ENTRY\n");
        fprintf(fid, "#endif\n");
    }

    NAMETABLE_id_to_string(ifp->name, &if_name);
    sprintf (include_var_name, "%s_v%d_%d_included", if_name,
                               (ifp->version%65536), (ifp->version/65536));
    fprintf (fid, "#ifndef %s\n#define %s\n",
             include_var_name, include_var_name);

    if (AST_DOUBLE_USED_SET(ifp) && !AST_LOCAL_SET(ifp) && (ifp->op_count > 0))
        fprintf(fid, "#ifndef IDL_DOUBLE_USED\n#define IDL_DOUBLE_USED\n#endif\n");

    if (BE_bug_boolean_def)
        fprintf(fid, "#ifndef NIDL_bug_boolean_def\n#define NIDL_bug_boolean_def\n#endif\n");

#ifdef PERFMON
    /* Include the performance monitoring definitions. */
    fprintf (fid, "#ifdef PERFMON\n");
    fprintf (fid, INCLUDE_TEMPLATE, "idl_log.h");
    fprintf (fid, "#endif\n");
#endif

    fprintf (fid, "#ifndef IDLBASE_H\n");
    fprintf (fid, INCLUDE_TEMPLATE, "idlbase.h");
    fprintf (fid, "#endif\n");

    if (!AST_LOCAL_SET(ifp) && (ifp->op_count > 0))
        fprintf (fid, INCLUDE_TEMPLATE, "rpc.h");

    if (AST_HAS_ENCODE_OPS_SET(ifp))
        fprintf (fid, INCLUDE_TEMPLATE, "idl_es.h");

    for (incp = ifp->includes; incp; incp = incp->next) {
        STRTAB_str_to_string (incp->simple_file_name, &fn_str);
        fprintf (fid, USER_INCLUDE_H_TEMPLATE, fn_str);
    }

    if ((int)CMD_vals[opt_lang] != opt_lang_cxx) {
        fprintf (fid, "\n#ifdef __cplusplus\n    extern \"C\" {\n#endif\n\n");
    }

    for (impp = ifp->imports; impp; impp=impp->next) {
        STRTAB_str_to_string (impp->file_name, &fn_str);
	header_suffix = HEADER_SUFFIX;

        FILE_form_filespec((char *)NULL, (char *)NULL,
			     header_suffix,
                             fn_str, include_var_name);
        if (impp->interface != NULL)
            fprintf (fid, "#ifndef %s_v%d_%d_included\n",
                    BE_get_name(impp->interface->name),
                    (impp->interface->version%65536),
                    (impp->interface->version/65536));
        fprintf (fid, USER_INCLUDE_TEMPLATE, include_var_name);
        if (impp->interface != NULL)
            fprintf (fid, "#endif\n");
    }

    BE_spell_extern_user_excs(fid, ifp);
    if ((int)CMD_vals[opt_lang] == opt_lang_cxx) {
	fprintf(fid,"\n#ifdef __cplusplus\n");
	fprintf(fid,"#include <dce/objref.hxx>\n\n");
	fprintf(fid,"\n#endif\n");
    }

    CSPELL_exports (fid, ifp->exports);
    BE_gen_pipe_routine_decls (fid, ifp);
    CSPELL_user_prototypes (fid, ifp);
    BE_spell_ichar_prototypes(fid, ifp);

    /* emit declarations of implicit handle variable and epv's */
    if (ifp->implicit_handle_name != NAMETABLE_NIL_ID) {
        fprintf (fid, "globalref ");
        if ( ! AST_IMPLICIT_HANDLE_G_SET(ifp) )
        {
            fprintf(fid, "handle_t");
        }
        else
        {
            spell_name (fid, ifp->implicit_handle_type_name);
        }
        fprintf(fid, " ");
        spell_name (fid, ifp->implicit_handle_name);
        fprintf (fid, ";\n");
        }

    if (!AST_LOCAL_SET(ifp) && (ifp->op_count > 0)) {
        CSPELL_epv_type_and_var(fid, ifp->name, ifp->version, ifp->exports,
            cepv_opt);
        CSPELL_if_spec_refs (fid, ifp->name, ifp->version);
    }

    if ((int)CMD_vals[opt_lang] == opt_lang_cxx)
        fprintf(fid,"\n#ifdef __cplusplus\n\n");
    else
        fprintf (fid, "\n#ifdef __cplusplus\n    }\n\n");

    if ((int)CMD_vals[opt_lang] == opt_lang_cxx) {
	BE_gen_cxx_class(fid, ifp, base_class);
    }

    if (!AST_LOCAL_SET(ifp) && (ifp->op_count > 0))
    {
        fprintf (fid, "#else\n");
            if ((int)CMD_vals[opt_lang] == opt_lang_cxx)
                BE_gen_c_bindings(fid, ifp);
    }
    fprintf (fid, "#endif\n");	/* ifdef __cplusplus */
    fprintf (fid, "#endif\n");	/* ifndef <interface>_included */
}


/*
**	This function spells the data structures necessary for 
**      binding an RPC object from a C program. 
**	Macros are created to invoke object member function operations from C.
*/

void
BE_gen_c_bindings(
		FILE *fid,
		AST_interface_n_t   *ifp
    )
{    
    AST_export_n_t    *ep;
    char	      *class_suffix = "Vtbl";
    char	      *class_comment = "C Language Binding";
    char	      c_map_name[500];
    NAMETABLE_id_t    sav_param_id;   

    fprintf (fid, "\n/******* %s *******/\n\n", class_comment);

    /* generate a v-table */
    fprintf (fid, "typedef struct %s%s {\n",
        BE_get_name(ifp->name), class_suffix);
    fprintf (fid, "void (*destroy)(\n");
    fprintf (fid, "#ifdef IDL_PROTOTYPES\n");
    fprintf (fid, "    /* [in] */ %s *this\n", BE_get_name(ifp->name));
    fprintf (fid, "#endif\n");
    fprintf (fid, ");\n");
    for (ep = ifp->exports; ep; ep = ep->next) {
	if ((ep->kind == AST_operation_k)
           ) 
        {
	    AST_operation_n_t  *op = ep->thing_p.exported_operation;
	    AST_parameter_n_t	*handle_param;
	    AST_parameter_n_t	*new_param_p;   /* Ptr to new parameter node */
	    NAMETABLE_id_t      new_param_id;   /* Nametable id of new param */
	    AST_type_n_t	*new_type_p;    /* Ptr to new param type node */
	    AST_type_n_t	*interf_type_p; /* Node bound to inteface name*/
	    AST_pointer_n_t	*new_ptr_p;     /* Ptr to new pointer node */

	    /* bypass constructor/destructor operations */
	    if (IS_CONSTRUCTOR(op) || IS_DESTRUCTOR(op))
		    continue;

	    /* bypass cxx_static operations */
            if (AST_STATIC_SET(op))
	            continue;

	    /* 
	    ** Suppress handle_t parameters for member functions 
	    */
	    handle_param = NULL;
	    if (BE_is_handle_param(op->parameters)) {
		handle_param   = op->parameters;
		op->parameters = op->parameters->next;
	    }

	    /* Add the 'this' parameter */
	    new_param_id = NAMETABLE_add_id("this");
	    new_param_p = AST_parameter_node(new_param_id);

	    interf_type_p = (AST_type_n_t *)NAMETABLE_lookup_binding(ifp->name);

	    new_ptr_p   = AST_pointer_node(interf_type_p);
	    new_type_p  = AST_type_node(AST_pointer_k);
	    new_type_p->type_structure.pointer = new_ptr_p;
	    new_type_p->kind = AST_pointer_k;
	    new_type_p->name = NAMETABLE_NIL_ID;

	    new_param_p->type = new_type_p;
	    new_param_p->name = new_param_id;
	    new_param_p->uplink = op;
	    AST_SET_REF(new_type_p);
	    AST_SET_IN(new_param_p);

	    /* put the 'this' pointer first on the parm list */
	    new_param_p->next = op->parameters;
    	    if (new_param_p->next != NULL) {
		new_param_p->last = new_param_p->next->last;
		new_param_p->next->last = NULL;
	    }
	    op->parameters = new_param_p;

            CSPELL_epv_field (fid, op);
	    if (handle_param != NULL) {
		op->parameters = handle_param;
	    }
	    /*
	    ** note that the variables used to temporarily hold
	    ** the this pointer parameter are not freed since there
	    ** are no routines to do this, but that's considered ok
	    */
	    
	} /* if */
    } /* for */
    fprintf (fid, "\n} %s%s;\n", BE_get_name(ifp->name), class_suffix);

    /* generate the structure for a C binding */
    fprintf(fid, "\nstruct %s {\n", BE_get_name(ifp->name));
    fprintf(fid, "\t%s%s *pVtbl;\n", BE_get_name(ifp->name), class_suffix);
    fprintf(fid, "};\n\n");

    for (ep = ifp->exports; ep; ep = ep->next)
	if ((ep->kind == AST_operation_k))
    {
	/* Setup a function type for the spellers */
	AST_operation_n_t  *op = ep->thing_p.exported_operation;
	AST_type_n_t       func_type_node;
	NAMETABLE_id_t     method_name;
	NAMETABLE_id_t     new_param_id;   /*Nametable id of new parm*/
	AST_parameter_n_t  *handle_param;
	AST_parameter_n_t  *new_param_p;   /* Ptr to new param node */
	AST_type_n_t	   *interf_type_p; /* Node bound to iface name*/
	AST_pointer_n_t	   *new_ptr_p;     /* Ptr to new pointer node */
	AST_type_n_t	   *new_type_p;    /*Ptr to new parm type node*/
	char *             method_string;

	func_type_node = *BE_function_p;
	func_type_node.type_structure.function = op;

	if (IS_CONSTRUCTOR(op) || IS_DESTRUCTOR(op)  || AST_STATIC_SET(op))
	{
	    fprintf(fid, "extern ");
	    sprintf(c_map_name, "%s_%s",
	        BE_get_name(ifp->name),
	    	BE_get_name(op->name));
	    sav_param_id = NAMETABLE_add_id(c_map_name);
	    CSPELL_typed_name (fid, &func_type_node, sav_param_id,
				    BE_ulong_int_p, false, true, false);
            fprintf(fid, ";\n");
	} else {
	    method_name = op->name;
	    method_string = BE_get_name(method_name);
	    fprintf(fid, "extern ");
	    sprintf(c_map_name, "%s_%s", 
	        BE_get_name(ifp->name),
	        BE_get_name(op->name));
	    sav_param_id = NAMETABLE_add_id(c_map_name);

	    handle_param = NULL;
            if (BE_is_handle_param(op->parameters)) {
                handle_param = op->parameters;
                op->parameters = op->parameters->next;
            }
            /* Add the 'this' parameter */
	    new_param_id = NAMETABLE_add_id("this");
	    new_param_p = AST_parameter_node(new_param_id);

	    interf_type_p = (AST_type_n_t *)NAMETABLE_lookup_binding(ifp->name);

	    new_ptr_p   = AST_pointer_node(interf_type_p);
	    new_type_p  = AST_type_node(AST_pointer_k);
	    new_type_p->type_structure.pointer = new_ptr_p;
	    new_type_p->kind = AST_pointer_k;
	    new_type_p->name = NAMETABLE_NIL_ID;

	    new_param_p->type = new_type_p;
	    new_param_p->name = new_param_id;
	    new_param_p->uplink = op;
	    AST_SET_REF(new_type_p);
	    AST_SET_IN(new_param_p);

	    /* put the 'this' pointer first on the parm list */
	    new_param_p->next = op->parameters;
    	    if (new_param_p->next != NULL) {
		new_param_p->last = new_param_p->next->last;
		new_param_p->next->last = NULL;
	    }
	    op->parameters = new_param_p;
	    CSPELL_typed_name (fid, &func_type_node, sav_param_id,
	        BE_ulong_int_p, false, true, false);
	    op->parameters = new_param_p->next;
	    if (handle_param != NULL) {
		op->parameters = handle_param;
	    }
            fprintf(fid, ";\n");
	}
    }
	/* generate mappings to the bind functions */
	fprintf(fid, "\nextern %s *%s_bind_by_name(unsigned_char_t *name);\n",
		BE_get_name(ifp->name), BE_get_name(ifp->name));
	fprintf(fid, "\nextern %s *%s_bind_by_uuid(uuid_t * u);\n",
		BE_get_name(ifp->name), BE_get_name(ifp->name));
	fprintf(fid, "\nextern %s *%s_bind_by_hndl(rpc_binding_handle_t bh);\n",
		BE_get_name(ifp->name), BE_get_name(ifp->name));
	fprintf(fid, "\nextern %s *%s_bind_by_iface(void *iface);\n",
		BE_get_name(ifp->name), BE_get_name(ifp->name));

	fprintf(fid, "\nextern void %s_destroy(%s *obj);\n",
		BE_get_name(ifp->name), BE_get_name(ifp->name));


	fprintf(fid, "\nextern void %s_secure(\n",
		BE_get_name(ifp->name));

	fprintf(fid, "    %s *obj,\n", BE_get_name(ifp->name));
	fprintf(fid, "    unsigned_char_t * principal,\n");
	fprintf(fid, "    unsigned32 protection,\n");
	fprintf(fid, "    unsigned32 authorization,\n");
	fprintf(fid, "    rpc_auth_identity_handle_t authid,\n");
	fprintf(fid, "    unsigned32 authorz_svc);\n");

}
