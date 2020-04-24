/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: emit.c,v $
 * Revision 1.1.6.2  1996/02/17  23:59:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:02  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:34:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:17:42  root]
 * 
 * Revision 1.1.4.3  1993/01/03  22:14:31  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:47:14  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  19:14:37  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:10:27  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/04  03:29:26  harrow
 * 	Add V1_* type attributes for anonymous types (defect 2127)
 * 	[1992/03/04  03:26:48  harrow]
 * 
 * Revision 1.1  1992/01/19  02:59:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * emit.c -- The Interface Definition Emitter
 */

#include <ast.h>
#include <stdio.h>
#include <uuid.h>
#include <socket.h>
#include <nametbl.h>
#include <util.h>
#include <emit.h>

FILE *F,    *ACF;        /* the output files */
binding_t   *ACF_interface;
char        *ACF_suffix;
static int          indent = 0;

#ifdef __STDC__
FILE * create_file (STRTAB_str_t source_name, char *suffix);
#else
FILE * create_file ();
#endif


void set_indent
#ifdef __STDC__
 (void)
#else
()
#endif
{
    int i;
    for (i = indent; i > 0; i --) fprintf(F," ");
}

void indent_more
#ifdef __STDC__
 (void)
#else
()
#endif
{
    indent += 4;
}

void indent_less
#ifdef __STDC__
 (void)
#else
()
#endif
{
    indent -= 4;
}


void uuid_$encode_new
#ifdef __STDC__
 (
  /* [in] */uuid_$t *uuid,
  /* [out] */uuid_$string_t s)
#else
(uuid, s)
    uuid_$t *uuid;
    uuid_$string_t s;
#endif
{
    typedef struct  {
        unsigned int time_low;
        unsigned short time_mid;
        unsigned short time_hi_and_version;
        unsigned char clock_seq_hi_and_reserved;
        unsigned char clock_seq_low;
        unsigned char node[6];
    } new_uuid_t;
    new_uuid_t  new_uuid;

    uuid->reserved = 0;     /* Make sure old UUID zero in reserved field */
    *(uuid_$t*)&new_uuid = *uuid;

    sprintf(s, "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
              new_uuid.time_low, new_uuid.time_mid, new_uuid.time_hi_and_version,
              new_uuid.clock_seq_hi_and_reserved, new_uuid.clock_seq_low,
              new_uuid.node[0], new_uuid.node[1], new_uuid.node[2],
              new_uuid.node[3], new_uuid.node[4], new_uuid.node[5]);
}

void uuid_$encode_old
#ifdef __STDC__
 (
  /* [in] */uuid_$t *uuid,
  /* [out] */uuid_$string_t s)
#else
(uuid, s)
    uuid_$t *uuid;
    uuid_$string_t s;
#endif
{
    sprintf(s, "%08lx%04x.%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x",
        uuid->time_high, uuid->time_low, uuid->family,
        uuid->host[0], uuid->host[1], uuid->host[2], uuid->host[3],
        uuid->host[4], uuid->host[5], uuid->host[6]);
}


#ifdef __STDC__
static void emit_if_header(interface_t *ifb, NAMETABLE_id_t if_name)
#else
static void emit_if_header(ifb, if_name)
interface_t *ifb;
NAMETABLE_id_t if_name;
#endif
{
    unsigned long family, ports = 0;
    uuid_$string_t uuid_buf;
    char buf[SMALL_BUFFER];
    Assert(ifb);


    OUTPUT_COMMENTS(ifb->comment_text);

    /* emit interface attributes */
    if (ifb->local_only)
        fprintf(F, "[local]\n");
    else
    {
        uuid_$encode_old(&ifb->interface_uuid, uuid_buf);
        fprintf(F, "[\n /* V1 format UUID: %s */\n", uuid_buf);
        uuid_$encode_new(&ifb->interface_uuid, uuid_buf);
        fprintf(F, " uuid(%s)", uuid_buf);

        if (ifb->interface_version >= 65535)
        {
            sprintf(buf, "Version number '%d' is too large, must be changed", ifb->interface_version);
            warn(buf);
        }
        fprintf(F, ",\n version(%d)", ifb->interface_version);

        for (family = 0; family < socket_$num_families; family++)
            if (ifb->well_known_ports[family] != socket_$unspec_port) ports++;

        if (ports)
        {
            fprintf(F, ",\n endpoint(");
            for (family = 0; ports && family < socket_$num_families; family++)
                if (ifb->well_known_ports[family] != socket_$unspec_port)
                {
                    fprintf(F, "\"%s:[%d]\"", get_family(family), ifb->well_known_ports[family]);
                    if (--ports) fprintf(F, ", ");
                }
            fprintf(F, ")");
        }
        fprintf(F, "]\n");
    }
    fprintf(F, "interface %s\n{\n", get_name(if_name));
}

#ifdef __STDC__
static void emit_acf_header()
#else
static void emit_acf_header()
#endif
{
    interface_t *ifb = (interface_t *)ACF_interface->binding;
    NAMETABLE_id_t if_name = ACF_interface->name;
    Assert(ifb);

    if (ACF) return;

    /* Open the ACF file */
    ACF = create_file(ifb->source_file, ACF_suffix);

    /* emit acf interface attributes */
    if (ifb->local_only)
    {
    }
    else
    {
        if (ifb->implicit_handle_var != NAMETABLE_NIL_ID)
        {
            FILE *save_F = F;
            fprintf(ACF, "[implicit_handle(");
            F = ACF;
            spell_typed_name(ifb->implicit_handle_type, ifb->implicit_handle_var);
            F = save_F;
            fprintf(ACF, ")");
            fprintf(ACF, "]\n");
        }

    }
    fprintf(ACF, "interface %s\n{\n", get_name(if_name));
}

#ifdef __STDC__
void emit_imports(import_t *imports)
#else
void emit_imports(imports)
import_t *imports;
#endif
{
    char *filename;

    while (imports)
    {
        if (imports->comment_text) fprintf(F, "\n");
        OUTPUT_COMMENTS(imports->comment_text);
        if ((strcmp(get_string(imports->file_name),"rpc.idl") != 0) &&
            (strcmp(get_string(imports->file_name),"nbase.idl") != 0))
            fprintf(F, "import \042%s\042;\n", get_string(imports->file_name));
        imports = imports->next_import;
    }
}

#ifdef __STDC__
void emit_constant(constant_t *constant, NAMETABLE_id_t name)
#else
void emit_constant(constant, name)
constant_t *constant;
NAMETABLE_id_t name;
#endif
{
    char *stringval;
    char mesg[SMALL_BUFFER];

    if (constant->comment_text) fprintf(F, "\n");
    OUTPUT_COMMENTS(constant->comment_text);
    indent = 4;
    switch (constant->kind)
    {
        case integer_k:
            fprintf(F, "    const long %s = %d;\n", get_name(name), constant->value.int_val);
            break;
        case string_k:
            fprintf(F, "    const char *%s = \042%s\042;\n", get_name(name),
                get_string(constant->value.string_val));
            break;
        case boolean_const_k:
            {
            char *boolean_const;
            if (constant->value.boolean_value) boolean_const = "TRUE";
            else boolean_const = "FALSE";

            fprintf(F, "    const boolean %s = %s;\n", get_name(name), boolean_const);
            break;
            }
        case named_const_k:
            {
                char *type_name = "????";
                if (constant->value.named_val.resolution->kind == integer_k)
                    type_name = "long ";
                else if (constant->value.named_val.resolution->kind == string_k)
                    type_name = "char *";
                else {
                    sprintf(mesg, "Unsupported constant declaration ignored for identifier '%s'\n",
                        get_name(name));
                    warn(mesg);
                    }

                fprintf(F, "    const %s%s = %s;\n", type_name,
                    get_name(name), get_name(constant->value.named_val.name));
                break;
            }
        default:
            sprintf(mesg, "Unsupported constant declaration ignored for identifier '%s'\n",
                get_name(name));
            warn(mesg);
            fprintf(F, "    const ????? %s = ????;\n", get_name(name));
            break;
    }
}

#ifdef __STDC__
void emit_type(type_t *type, NAMETABLE_id_t name)
#else
void emit_type(type, name)
type_t *type;
NAMETABLE_id_t name;
#endif
{
    type_t *user_type = type;
    char buf[SMALL_BUFFER];
    boolean prev;

    if (type->comment_text) fprintf(F, "\n");
    OUTPUT_COMMENTS(type->comment_text);
    indent = 4;
    fprintf(F, "    typedef ");
    if (Has_type_attrs(type) || (type->kind == record_k))
    {
        prev = false;
        fprintf(F, "[");
        if (type->is_handle)
        {
            fprintf(F, "handle");
            prev = true;
        }
        if (type->kind == user_marshalled_k)
        {
            if (prev) fprintf(F, ", ");
            fprintf(F, "transmit_as(%s)",
                get_name(type->type_structure.user_marshalled.xmit_type->type_structure.named.name));
            prev = true;
            user_type = type->type_structure.user_marshalled.user_type;
        }

        if (user_type->kind == fixed_array_k || user_type->kind == open_array_k ||
            user_type->kind == fixed_string_zero_k)
        {
            if (prev) fprintf(F, ", ");
            fprintf(F, "v1_array");
            prev = true;
        }

        if (user_type->kind == fixed_string_zero_k)
        {
            if (prev) fprintf(F, ", ");
            fprintf(F, "v1_string");
            prev = true;
        }

        if (user_type->kind == record_k)
        {
            if (prev) fprintf(F, ", ");
            fprintf(F, "v1_struct");
            prev = true;
        }

        if (user_type->kind == long_enumeration_k)
        {
            if (prev) fprintf(F, ", ");
            fprintf(F, "v1_enum");
            prev = true;
        }

        fprintf(F, "] ");
    }

    spell_typed_name(type, name);
    fprintf(F, ";\n");

    /*
     * For bitset's, output a const def with the definition of typename_mask_none
     * But only do it once.
     */
    switch(type->kind)
    {
     case small_bitset_k:
     case short_bitset_k:
     case long_bitset_k:
     {  struct type_t *enum_type;
        enum_type = type->type_structure.bitset.base_type;
        while (enum_type->kind == named_k)
            enum_type = enum_type->type_structure.named.resolution;

        /*
        ** If we haven't previously output a typename_mask_none for this bitset
        ** output it now, if we have a name to use.
        */
        if ((!enum_type->bitset_mask_none_done) &&
            (type->type_structure.bitset.base_type->kind == named_k))
        {
            enum_type->bitset_mask_none_done = true;
            set_indent();
            fprintf(F, "const ");
            if (type->kind == small_bitset_k) fprintf(F, "small ");
            if (type->kind == short_bitset_k) fprintf(F, "short ");
            if (type->kind == long_bitset_k) fprintf(F, "long ");
            fprintf(F, " %s_mask_none = 0;\n", get_name(
                type->type_structure.bitset.base_type->type_structure.named.name));
            break;
        }
      }
    }
}

#ifdef __STDC__
void emit_parameters(parameter_t *parameter, NAMETABLE_id_t op_name)
#else
void emit_parameters(parameter, op_name)
parameter_t *parameter;
NAMETABLE_id_t op_name;
#endif
{
    char buf[SMALL_BUFFER];

    indent += 4;
    while (parameter)
    {
        set_indent();
        OUTPUT_COMMENTS(parameter->comment_text);
        OUTPUT_COMMENTS(parameter->type->comment_text);
        if (parameter->comm_status)
        {
            emit_acf_header();
            fprintf(ACF, "     %s ([comm_status] %s);\n", get_name(op_name), get_name(parameter->name));
            sprintf(buf, "Parameter '%s' in operation '%s': [comm_status] requires .acf file",
                get_name(parameter->name), get_name(op_name));
            warn(buf);
        }
        /* we assume that all parameters have at least an [in] or [out] attribute */
        fprintf(F, "[");
        if (parameter->in)
            fprintf(F, "in");
        if (parameter->out)
        {
            if (parameter->in) fprintf(F, ", ");
                fprintf(F, "out");
        }
        if (parameter->type->kind == fixed_array_k || parameter->type->kind == open_array_k || parameter->type->kind == fixed_string_zero_k)
            fprintf(F, ", v1_array");

        if (parameter->last_is_ref)
        {
            fprintf(F, ", last_is(");
            if (parameter->last_is_ref->out) fprintf(F, "*");
            fprintf(F, "%s)", get_name(parameter->last_is_ref->name));

            if (parameter->type->kind == open_array_k && !parameter->max_is_ref)
                fprintf(F, ", max_is(%s)", get_name(parameter->last_is_ref->name));
            else if (parameter->type->kind == named_k && !parameter->max_is_ref &&
                (parameter->type->type_structure.named.resolution->kind == open_array_k))
                fprintf(F, ", max_is(%s)", get_name(parameter->last_is_ref->name));
        }
        if (parameter->max_is_ref)
        {
            fprintf(F, ", max_is(");
            if (parameter->max_is_ref->out) fprintf(F, "*");
            fprintf(F, "%s)", get_name(parameter->max_is_ref->name));
        }
        if (parameter->type->kind == fixed_string_zero_k)
            fprintf(F, ", v1_string");
        if (parameter->type->kind == record_k)
            fprintf(F, ", v1_struct");
        if (parameter->type->kind == long_enumeration_k)
            fprintf(F, ", v1_enum");
        fprintf(F, "] ");

        spell_typed_name(parameter->type, parameter->name);

        parameter = parameter->next_param;
        if (parameter)  fprintf(F, ",\n");
    }

    indent -= 4;
}

#ifdef __STDC__
void emit_routine(routine_t *routine, NAMETABLE_id_t name)
#else
void emit_routine(routine, name)
routine_t *routine;
NAMETABLE_id_t name;
#endif
{
    int prev = 0;
    char buf[SMALL_BUFFER];

    if (routine->comment_text) fprintf(F, "\n");
    OUTPUT_COMMENTS(routine->comment_text);
    pointerize_routine(routine);
    indent = 4;

    set_indent();
    if (routine->comm_status)
    {
        emit_acf_header();
        fprintf(ACF, "     [comm_status] %s();\n", get_name(name));
        sprintf(buf, "Operation '%s': [comm_status] requires .acf file", get_name(name));
        warn(buf);
    }

    if (Has_routine_attrs(routine))
    {
        fprintf(F, "[");
        if (routine->idempotent)
        {
            if (prev) fprintf(F, ", ");
            fprintf(F, "idempotent");
            prev = 1;
        }
        if (routine->broadcast)
        {
            if (prev) fprintf(F, ", ");
            fprintf(F, "broadcast");
            prev = 1;
        }
        if (routine->maybe)
        {
            if (prev) fprintf(F, ", ");
            fprintf(F, "maybe");
        }
        fprintf(F, "] ");
    }

    spell_typed_name(routine->result->type, name);
    fprintf(F, "\n");
    set_indent();
    fprintf(F, "(\n");
    emit_parameters(routine->parameters, name);
    fprintf(F, "\n");
    set_indent();
    fprintf(F, ");\n");
}

#ifdef __STDC__
void emit(char *idl_suffix, char *acf_suffix, binding_t *ast)
#else
void emit(idl_suffix, acf_suffix, ast)
char *idl_suffix, *acf_suffix;
binding_t *ast;
#endif
{
    int i;
    binding_t *p = ast;

    F = create_file(p->binding->interface.source_file, idl_suffix);
    ACF = NULL;
    ACF_interface = p;
    ACF_suffix = acf_suffix;

    if (p->binding->interface.implicit_handle_var != NAMETABLE_NIL_ID)
    {
        char buf[SMALL_BUFFER];
        sprintf(buf, "[implicit_handle] requires .acf file");
        warn(buf);
        emit_acf_header();
    }

    Assert(p->kind == (int)interface_k);
    emit_if_header(&p->binding->interface, p->name);
    emit_imports(p->binding->interface.imports);
    p = p->binding->interface.exports;
    while (p)
    {
        indent = 0;
        fprintf(F, "\n");
        OUTPUT_COMMENTS(p->comment_text);
        switch (p->kind)
        {
            case constant_k:
                emit_constant(&p->binding->constant, p->name);
                break;
            case type_k:
                emit_type(&p->binding->type, p->name);
                break;
            case routine_k:
                emit_routine(&p->binding->routine, p->name);
                break;
            default:
                Assert(0);
        }
        p = p->next;
    }

    fprintf(F, "}\n");

    /* Add closing brace on ACF if there is one */
    if (ACF) {
        fprintf(ACF,"}\n");
    }

}
