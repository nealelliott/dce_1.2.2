/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * @DEC_COPYRIGHT@
 * 
 */
/*
 * HISTORY
 * $Log: irepdmp.c,v $
 * Revision 1.1.7.2  1996/03/09  20:44:55  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:37  marty]
 *
 * Revision 1.1.7.1  1995/12/07  22:23:09  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:04 UTC  dat
 * 	New file for second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:32:43  root]
 * 
 * Revision 1.1.2.1  1995/10/23  03:21:23  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  01:58:36  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/23  01:57:58  bfc]
 * 
 * $EndLog$
 */
/*
**  NAME:
**
**      irepdmp.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Dumps the Intermediate Representation of the IDL interface.
**
**  %a%private_begin
**
**  MODIFICATION HISTORY: 
**  
**  09-Oct-91 D.Annicchiarico   Original version.
**
**  %a%private_end  
*/

#include <nidl.h>       /* Standard IDL defs */
#include <ast.h>        /* Abstract Syntax Tree defs */
#include <irep.h>       /* Intermediate Rep defs */
#include <astp.h>       /* AST processing defs */

/* Necessary forward function declarations */

static void IR_dump_tuples(
#ifdef PROTO
    IR_scope_ctx_t      *ctx_p,     /* [in] Scope context */
    IR_tup_n_t          *tup_p      /* [in] Ptr to list of IREP tuples */
#endif
);


/*
 * Dump a tuple's flags.
 */
#define MAX_FLAG_TEXT 80
static void IR_dump_flags
#ifdef PROTO
(
    IR_flags_t          flags       /* [in] tuple flags */
)
#else
(flags)
    IR_flags_t          flags;      /* [in] tuple flags */
#endif
{
    char    flagtext[MAX_FLAG_TEXT];

    if (flags == 0)
        return;

    strcpy(flagtext, "flags=");
    if (flags & IR_ENCAPSULATED)    strcat(flagtext, "ENCAPSULATED|");
    if (flags & IR_CONFORMANT)      strcat(flagtext, "CONFORMANT|");
    if (flags & IR_ARRAYIFIED_PTR)  strcat(flagtext, "ARRAYIFIED_PTR|");
    if (flags & IR_STRING)          strcat(flagtext, "STRING|");
    if (flags & IR_VOID)            strcat(flagtext, "VOID|");
    if (flags & IR_BOOLEAN)         strcat(flagtext, "BOOLEAN|");
    if (flags & IR_REP_AS)          strcat(flagtext, "REP_AS|");
    if (flags & IR_CS_CHAR)         strcat(flagtext, "CS_CHAR|");
    if (flags & IR_IN_ONLY)         strcat(flagtext, "IN_ONLY|");
    if (flags & IR_OUT_ONLY)        strcat(flagtext, "OUT_ONLY|");

    flagtext[strlen(flagtext)-1] = '\0';    /* overwrite last '|' */
    strcat(flagtext, ", ");
    printf(flagtext);
}


/*
 * Dump the name of a scalar type.
 */
static void IR_dump_scalar_type
#ifdef PROTO
(
    AST_type_n_t        *type_p     /* [in] Ptr to AST type node */
)
#else
(type_p)
    AST_type_n_t        *type_p;    /* [in] Ptr to AST type node */
#endif
{
    switch(type_p->kind)
    {
    case AST_boolean_k:
        printf("AST_boolean_k");
        break;
    case AST_byte_k:
        printf("AST_byte_k");
        break;
    case AST_character_k:
        printf("AST_character_k");
        break;
    case AST_small_integer_k:
        printf("AST_small_integer_k");
        break;
    case AST_short_integer_k:
        printf("AST_short_integer_k");
        break;
    case AST_long_integer_k:
        printf("AST_long_integer_k");
        break;
    case AST_hyper_integer_k:
        printf("AST_hyper_integer_k");
        break;
    case AST_small_unsigned_k:
        printf("AST_small_unsigned_k");
        break;
    case AST_short_unsigned_k:
        printf("AST_short_unsigned_k");
        break;
    case AST_long_unsigned_k:
        printf("AST_long_unsigned_k");
        break;
    case AST_hyper_unsigned_k:
        printf("AST_hyper_unsigned_k");
        break;
    case AST_short_float_k:
        printf("AST_short_float_k");
        break;
    case AST_long_float_k:
        printf("AST_long_float_k");
        break;
    case AST_enum_k:
        printf("AST_enum_k");
        break;
    default:
        INTERNAL_ERROR("Invalid scalar type");
    }
}


/*
 * Dump a type name or its address.
 */
static void IR_dump_type
#ifdef PROTO
(
    AST_type_n_t        *type_p     /* [in] Ptr to AST type node */
)
#else
(type_p)
    AST_type_n_t        *type_p;    /* [in] Ptr to AST type node */
#endif
{
    if (type_p->name == NAMETABLE_NIL_ID)
        printf("type=0x%08lx", (long)type_p);
    else
    {
        char *name;
        NAMETABLE_id_to_string(type_p->name, &name);
        printf("type=%s", name);
    }
}


/*
 * Dump an instance name.
 */
static void IR_dump_instance
#ifdef PROTO
(
    AST_instance_n_t    *inst_p     /* [in] Ptr to AST instance node */
)
#else
(inst_p)
    AST_instance_n_t    *inst_p;    /* [in] Ptr to AST instance node */
#endif
{
    char *name;

    if (inst_p == NULL)
        name = "NULL";
    else
        NAMETABLE_id_to_string(inst_p->name, &name);

    printf("instance=%s", name);
}


/*
 * Dump a name.
 */
static void IR_dump_name
#ifdef PROTO
(
    NAMETABLE_id_t      name_id     /* [in] Name ID */
)
#else
(name_id)
    NAMETABLE_id_t      name_id;    /* [in] Name ID */
#endif
{
    char *name;
    NAMETABLE_id_to_string(name_id, &name);
    printf("%s", name);
}


/*
 * Dump information for an array bound.
 */
static void IR_dump_bound
#ifdef PROTO
(
    IR_tup_n_t          *tup_p      /* [in] Ptr to list of IREP tuples */
)
#else
(tup_p)
    IR_tup_n_t          *tup_p;     /* [in] Ptr to list of IREP tuples */
#endif
{
    AST_type_n_t        *deref_type_p;  /* Dereferenced (if necc.) type */
    AST_instance_n_t    *inst_p;
    char                *bound_text;
    char                *ref_name;
    int                 i;
    IR_bound_k_t        bound_kind;

    bound_kind = tup_p->arg[IR_ARG_BOUND].bound_k;

    switch (bound_kind)
    {
    case IR_bnd_fixed_k:
        printf("%ld", tup_p->arg[IR_ARG_INT].int_val);
        return;
        
    case IR_bnd_string_k:
        if (tup_p->flags & IR_REP_AS)
        {
            NAMETABLE_id_to_string(
                tup_p->arg[IR_ARG_TYPE2].type->rep_as_type->type_name,
                &ref_name);
            printf("string(local_type=%s", ref_name);
        }
        else
            printf("string(octetsize=%ld", tup_p->arg[IR_ARG_INT].int_val);
        if (tup_p->arg[IR_ARG_PFNUM].int_val != 0)
            printf(",field_num=%ld", tup_p->arg[IR_ARG_PFNUM].int_val);
        printf(")");
        return;

    case IR_bnd_min_is_k:
        bound_text = "min_is";
        break;

    case IR_bnd_max_is_k:
        bound_text = "max_is";
        break;

    case IR_bnd_size_is_k:
        bound_text = "size_is";
        break;

    default:
        INTERNAL_ERROR("Invalid bound kind");
        return;
    }

    printf("%s(", bound_text);

    inst_p = tup_p->arg[IR_ARG_INST].inst;
    NAMETABLE_id_to_string(inst_p->name, &ref_name);

    deref_type_p = ASTP_chase_ptr_to_type(inst_p->type);
    for (i = 0; i < deref_type_p->fe_info->pointer_count; i++)
        printf("*");

    printf("%s)", ref_name);
}


/*
 * Dump information for an array data limit.
 */
static void IR_dump_limit
#ifdef PROTO
(
    IR_tup_n_t          *tup_p      /* [in] Ptr to list of IREP tuples */
)
#else
(tup_p)
    IR_tup_n_t          *tup_p;     /* [in] Ptr to list of IREP tuples */
#endif
{
    AST_type_n_t        *deref_type_p;  /* Dereferenced (if necc.) type */
    AST_instance_n_t    *inst_p;
    char                *limit_text;
    char                *ref_name;
    int                 i;
    IR_limit_k_t        limit_kind;

    limit_kind = tup_p->arg[IR_ARG_LIMIT].limit_k;

    switch (limit_kind)
    {
    case IR_lim_fixed_k:
        printf("%ld", tup_p->arg[IR_ARG_INT].int_val);
        return;
        
    case IR_lim_string_k:
        if (tup_p->flags & IR_REP_AS)
        {
            NAMETABLE_id_to_string(
                tup_p->arg[IR_ARG_TYPE2].type->rep_as_type->type_name,
                &ref_name);
            printf("string(local_type=%s)", ref_name);
        }
        else
            printf("string(octetsize=%ld)", tup_p->arg[IR_ARG_INT].int_val);
        return;

    case IR_lim_upper_conf_k:
        printf("upper_conf");
        return;
        
    case IR_lim_first_is_k:
        limit_text = "first_is";
        break;

    case IR_lim_last_is_k:
        limit_text = "last_is";
        break;

    case IR_lim_length_is_k:
        limit_text = "length_is";
        break;

    default:
        INTERNAL_ERROR("Invalid data limit kind");
        return;
    }

    printf("%s(", limit_text);

    inst_p = tup_p->arg[IR_ARG_INST].inst;
    NAMETABLE_id_to_string(inst_p->name, &ref_name);

    deref_type_p = ASTP_chase_ptr_to_type(inst_p->type);
    for (i = 0; i < deref_type_p->fe_info->pointer_count; i++)
        printf("*");

    printf("%s)", ref_name);
}


/*
 * Dump a case label constant.
 */
static void IR_dump_case_label
#ifdef PROTO
(
    AST_case_label_n_t  *label_p    /* [in] Ptr to AST case label node */
)
#else
(label_p)
    AST_case_label_n_t  *label_p;   /* [in] Ptr to AST case label node */
#endif
{
    AST_constant_n_t    *const_p;   /* Ptr to AST constant node */

    const_p = label_p->value;

    switch (const_p->kind)
    {
    case AST_int_const_k:
        printf("%ld", const_p->value.int_val);
        break;

    case AST_char_const_k:
        printf("'%c'", const_p->value.char_val);
        break;

    case AST_boolean_const_k:
        printf("%s", (const_p->value.boolean_val) ? "TRUE" : "FALSE");
        break;

    default:
        INTERNAL_ERROR("Invalid constant kind");
    }
}


/*
 * Dumps a single Intermediate Representation tuple.
 */
static void IR_dump_tuple
#ifdef PROTO
(
    IR_scope_ctx_t      *ctx_p,     /* [in] Scope context */
    IR_tup_n_t          *tup_p      /* [in] Ptr to IREP tuple */
)
#else
(ctx_p, tup_p)
    IR_scope_ctx_t      *ctx_p;     /* [in] Scope context */
    IR_tup_n_t          *tup_p;     /* [in] Ptr to IREP tuple */
#endif
{
    AST_type_n_t    *type_p;        /* Type node pointer */
    char            *name;

    /* Pick up type pointer, which is valid for most tuples. */
    type_p = tup_p->arg[IR_ARG_TYPE].type;

    switch (tup_p->opcode)
    {
    case IR_op_align_k:
        printf("IR_op_align_k(%d)\n", tup_p->arg[IR_ARG_INT].int_val);
        break;

    case IR_op_allocate_k:
        printf("IR_op_allocate_k(TBD)\n");
        break;

    case IR_op_array_bounds_k:
        printf("IR_op_array_bounds_k(");
        IR_dump_type(type_p);
        printf(", dim=%d)\n", tup_p->arg[IR_ARG_INT].int_val);
        break;

    case IR_op_array_end_k:
        printf("IR_op_array_end_k(");
        IR_dump_type(type_p);
        printf(")\n");
        break;

    case IR_op_bound_k:
        printf("IR_op_bound_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_bound(tup_p);
        printf(")\n");
        break;

    case IR_op_call_k:
        printf("IR_op_call_k(TBD)\n");
        break;

    case IR_op_call_param_k:
        printf("IR_op_call_param_k(TBD)\n");
        break;

    case IR_op_case_k:
        printf("IR_op_case_k(");
        IR_dump_flags(tup_p->flags);
        printf("%lu)\n", tup_p->arg[IR_ARG_INT].int_val);
        break;

    case IR_op_codeset_shadow_k:
        printf("IR_op_codeset_shadow_k(");
        IR_dump_flags(tup_p->flags);
        printf("num_elt=%lu)\n", tup_p->arg[IR_ARG_INT].int_val);
        break;

    case IR_op_conformant_array_k:
        printf("IR_op_conformant_array_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_type(type_p);
        printf(", ");
        IR_dump_instance(tup_p->arg[IR_ARG_INST].inst);
        printf(")\n");
        break;

    case IR_op_conformant_info_k:
        printf("IR_op_conformant_info_k(");
        IR_dump_tuple(ctx_p, tup_p->arg[IR_ARG_TUP].tup);
        printf(")\n");
        break;

    case IR_op_context_handle_k:
        printf ("IR_op_context_handle_k(");
        IR_dump_type(type_p);
        printf(", ");
        IR_dump_instance(tup_p->arg[IR_ARG_INST].inst);
        printf(")\n");
        break;

    case IR_op_cs_char_k:
        printf("IR_op_cs_char_k(network_");
        IR_dump_type(type_p);
        NAMETABLE_id_to_string(tup_p->arg[IR_ARG_CS_CHAR].cs_char->type_name,
                               &name);
        printf(", local_type=%s)\n", name);
        break;

    case IR_op_cs_char_end_k:
        printf("IR_op_cs_char_end_k(network_");
        IR_dump_type(type_p);
        NAMETABLE_id_to_string(tup_p->arg[IR_ARG_CS_CHAR].cs_char->type_name,
                               &name);
        printf(", local_type=%s)\n", name);
        break;

    case IR_op_declare_k:
        printf("IR_op_declare_k(TBD)\n");
        break;

    case IR_op_default_k:
        printf("IR_op_default_k(");
        IR_dump_flags(tup_p->flags);
        if (type_p == NULL)
            printf("<none>");
        printf(")\n");
        break;

    case IR_op_disc_union_begin_k:
        printf("IR_op_disc_union_begin_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_type(type_p);
        printf(")\n");
        break;

    case IR_op_disc_union_end_k:
        printf("IR_op_disc_union_end_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_type(type_p);
        printf(")\n");
        break;

    case IR_op_fixed_array_k:
        printf("IR_op_fixed_array_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_type(type_p);
        printf(", ");
        IR_dump_instance(tup_p->arg[IR_ARG_INST].inst);
        printf(")\n");
        break;

    case IR_op_flat_array_k:
        printf("IR_op_flat_array_k(addr=0x%08lx, end=0x%08lx)\n",
               (long)tup_p, (long)tup_p->arg[IR_ARG_TUP].tup);
        break;

    case IR_op_free_k:
        printf("IR_op_free_k(TBD)\n");
        break;

    case IR_op_full_array_end_k:
        printf("IR_op_full_array_end_k(addr=0x%08lx)\n", (long)tup_p);
        break;

    case IR_op_full_array_k:
        printf("IR_op_full_array_k(flat=0x%08lx)\n",
               (long)tup_p->arg[IR_ARG_TUP].tup);
        break;

    case IR_op_full_ptr_k:
        printf("IR_op_full_ptr_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_type(type_p);
        printf(", ");
        IR_dump_instance(tup_p->arg[IR_ARG_INST].inst);
        printf(")\n");
        break;

    case IR_op_interface_k:
        printf("IR_op_interface_k(");
        IR_dump_name(tup_p->arg[IR_ARG_INTFC].intfc->name);
        printf(")\n");
        break;

    case IR_op_limit_k:
        printf("IR_op_limit_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_limit(tup_p);
        printf(")\n");
        break;

    case IR_op_marshall_k:
        printf("IR_op_marshall_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_scalar_type(type_p);
        printf(", ");
        IR_dump_instance(tup_p->arg[IR_ARG_INST].inst);
        printf(")\n");
        break;

    case IR_op_noop_k:
        printf("IR_op_noop_k()\n");
        break;

    case IR_op_open_array_k:
        printf("IR_op_open_array_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_type(type_p);
        printf(", ");
        IR_dump_instance(tup_p->arg[IR_ARG_INST].inst);
        printf(")\n");
        break;

    case IR_op_passed_by_ref_k:
        printf("IR_op_passed_by_ref_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_type(type_p);
        printf(")\n");
        break;

    case IR_op_pipe_begin_k:
        printf("IR_op_pipe_begin_k(");
        IR_dump_type(type_p);
        printf(", base_");
        IR_dump_type(tup_p->arg[IR_ARG_TYPE2].type);
        printf(")\n");
        break;

    case IR_op_pipe_end_k:
        printf("IR_op_pipe_end_k(");
        IR_dump_type(type_p);
        printf(", base_");
        IR_dump_type(tup_p->arg[IR_ARG_TYPE2].type);
        printf(")\n");
        break;

    case IR_op_pointee_end_k:
        printf("IR_op_pointee_end_k(");
        IR_dump_type(type_p);
        printf(")\n");
        break;

    case IR_op_ref_ptr_k:
        printf("IR_op_ref_ptr_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_type(type_p);
        printf(", ");
        IR_dump_instance(tup_p->arg[IR_ARG_INST].inst);
        printf(")\n");
        break;

    case IR_op_release_shadow_k:
        printf("IR_op_release_shadow_k(");
        IR_dump_flags(tup_p->flags);
        printf(")\n");
        break;

    case IR_op_represent_as_k:
        printf("IR_op_represent_as_k(network_");
        IR_dump_type(type_p);
        NAMETABLE_id_to_string(tup_p->arg[IR_ARG_REP_AS].rep_as->type_name,
                               &name);
        printf(", local_type=%s)\n", name);
        break;

    case IR_op_represent_end_k:
        printf("IR_op_represent_end_k(network_");
        IR_dump_type(type_p);
        NAMETABLE_id_to_string(tup_p->arg[IR_ARG_REP_AS].rep_as->type_name,
                               &name);
        printf(", local_type=%s)\n", name);
        break;

    case IR_op_struct_begin_k:
        printf("IR_op_struct_begin_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_type(type_p);
        if (tup_p->arg[IR_ARG_INST].inst != NULL)
        {
            printf(", ");
            IR_dump_instance(tup_p->arg[IR_ARG_INST].inst);
        }
        printf(")\n");
        break;

    case IR_op_struct_end_k:
        printf("IR_op_struct_end_k(");
        IR_dump_type(type_p);
        if (tup_p->arg[IR_ARG_INST].inst != NULL)
        {
            printf(", ");
            IR_dump_instance(tup_p->arg[IR_ARG_INST].inst);
        }
        printf(")\n");
        break;

    case IR_op_switch_enc_k:
        printf("IR_op_switch_enc_k(");
        IR_dump_name(tup_p->arg[IR_ARG_NAME].name);
        printf(", ");
        IR_dump_scalar_type(type_p);
        printf(")\n");
        break;

    case IR_op_switch_n_e_k:
        printf("IR_op_switch_n_e_k(");
        IR_dump_instance(tup_p->arg[IR_ARG_INST].inst);
        printf(", instance_num=%ld)\n", tup_p->arg[IR_ARG_PFNUM].int_val);
        break;

    case IR_op_transmit_as_k:
        printf("IR_op_transmit_as_k(xmit_");
        IR_dump_type(type_p);
        printf(", pres_");
        IR_dump_type(tup_p->arg[IR_ARG_TYPE2].type);
        printf(")\n");
        break;

    case IR_op_transmit_end_k:
        printf("IR_op_transmit_end_k(xmit_");
        IR_dump_type(type_p);
        printf(", pres_");
        IR_dump_type(tup_p->arg[IR_ARG_TYPE2].type);
        printf(")\n");
        break;

    case IR_op_type_indirect_k:
        printf("IR_op_type_indirect_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_type(type_p);
        printf(", ");
        IR_dump_instance(tup_p->arg[IR_ARG_INST].inst);
        printf(")\n");
        /*
         * Recurse to dump type's IREP tuples unless tagged reference to
         * self-pointing type.
         */
        if (!IR_under_type(ctx_p, type_p))
        {
            IR_process_tup(ctx_p, tup_p);
            /*
             * If the REP_AS flag is set the tuples hang off the
             * rep_as node instead of the type node.
             * Similar situation for CS_CHAR flag.
             */
            if (tup_p->flags & IR_REP_AS)
                IR_dump_tuples(ctx_p, type_p->rep_as_type->data_tups);
            else if (tup_p->flags & IR_CS_CHAR)
                IR_dump_tuples(ctx_p, type_p->cs_char_type->data_tups);
            else
                IR_dump_tuples(ctx_p, type_p->data_tups);
        }
        break;

    case IR_op_unique_ptr_k:
        printf("IR_op_unique_ptr_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_type(type_p);
        printf(", ");
        IR_dump_instance(tup_p->arg[IR_ARG_INST].inst);
        printf(")\n");
        break;

    case IR_op_varying_array_k:
        printf("IR_op_varying_array_k(");
        IR_dump_flags(tup_p->flags);
        IR_dump_type(type_p);
        printf(", ");
        IR_dump_instance(tup_p->arg[IR_ARG_INST].inst);
        printf(")\n");
        break;

    default:
        printf("** Unknown tuple: opcode = %d **\n", (int)tup_p->opcode);
    }
}


/*
 * Dumps a list of Intermediate Representation tuples.
 */
static void IR_dump_tuples
#ifdef PROTO
(
    IR_scope_ctx_t      *ctx_p,     /* [in] Scope context */
    IR_tup_n_t          *tup_p      /* [in] Ptr to list of IREP tuples */
)
#else
(ctx_p, tup_p)
    IR_scope_ctx_t      *ctx_p;     /* [in] Scope context */
    IR_tup_n_t          *tup_p;     /* [in] Ptr to list of IREP tuples */
#endif
{
    while (tup_p != NULL)
    {
        IR_dump_tuple(ctx_p, tup_p);
        tup_p = tup_p->next;
    }
}


/*
 * Dumps the Intermediate Representation of an interface.
 */
void IR_dump_irep
#ifdef PROTO
(
    AST_interface_n_t   *int_p      /* [in] Ptr to AST interface node */
)
#else
(int_p)
    AST_interface_n_t   *int_p;     /* [in] Ptr to AST interface node */
#endif
{
    AST_export_n_t      *export_p;
    AST_operation_n_t   *oper_p;
    AST_parameter_n_t   *param_p;
    IR_scope_ctx_t      *ctx_p;     /* Needed to track self-pointing data */

    printf("\nDump of Intermediate Representation\n");
    printf(  "-----------------------------------\n\n");

    for (export_p = int_p->exports; export_p != NULL; export_p = export_p->next)
    {
        if ((export_p->kind == AST_operation_k)
           )
        {
            oper_p = export_p->thing_p.exported_operation;
            printf("**** Operation ");
            IR_dump_name(oper_p->name);
            printf(" ****\n\n");

            for (param_p = oper_p->parameters;
                 param_p != NULL;
                 param_p = param_p->next)
            {
                printf("---- Parameter ");
                IR_dump_name(param_p->name);
                printf(" ----\n");
                ctx_p = IR_init_scope((struct AST_parameter_n_t *)param_p);
                IR_dump_tuples(ctx_p, param_p->data_tups);
                IR_finish_scope(ctx_p);
            }

            printf("---- ");
            IR_dump_name(oper_p->name);
            printf(" function result ----\n");
            ctx_p = IR_init_scope((struct AST_parameter_n_t *)oper_p->result);
            IR_dump_tuples(ctx_p, oper_p->result->data_tups);
            IR_finish_scope(ctx_p);
        }
    }
}
