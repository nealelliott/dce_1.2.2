/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util.c,v $
 * Revision 1.1.6.2  1996/02/17  23:59:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:09  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:37:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:18:42  root]
 * 
 * Revision 1.1.4.3  1993/01/03  22:34:08  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:48:51  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  19:34:07  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:14:30  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/04  03:29:50  harrow
 * 	Fix translations of enums (defect 2125)
 * 	[1992/03/04  03:27:32  harrow]
 * 
 * Revision 1.1  1992/01/19  03:00:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * util.c -- Random utility routines
 */

#include <stdio.h>
#include <astp.h>
#include <ast.h>
#include <nametbl.h>
#include <socket.h>
#include <utils.h>
#include <emit.h>
#include <util.h>

#define Is_subrange_k(kind) ((kind) == small_subrange_k || (kind) == short_subrange_k || \
                             (kind) == long_subrange_k)

#ifdef __STDC__
void set_indent(void);
void indent_more(void);
void indent_less(void);
#else
void set_indent();
void indent_more();
void indent_less();
#endif


#ifdef __STDC__
static void spell_type(type_t *type, char *name);
#else
static void spell_type();
#endif

#ifdef __STDC__
static int len(char *s)
#else
static int len(s)
char *s;
#endif
{
    int count = 0;

    while(*s++) count++;

    return count;
}



#ifdef __STDC__
void print_comments(comment_t *c)
#else
void print_comments(c)
comment_t *c;
#endif
{
    comment_t *cp = c;
    char *new_line;

    if (c == NULL) return;

    /* Special case single line comments */
    if ((c->next == NULL) &&
        ((new_line = (char *)strchr(c->comment_text,'\n')) ==
        (c->comment_text + strlen(c->comment_text)-1)))
    {
        *new_line = '\0';
        fprintf (F, "/* %s */\n",c->comment_text);
        set_indent();
        free (c->comment_text);
        free (c);
        return;
    }

    /*
     * Print out multi line comments
     */
    if (*cp->comment_text != '\n') fprintf(F,"\n/*\n");
    else fprintf(F,"\n/*");

    while (cp != NULL) {
        fprintf(F, "%s", cp->comment_text);
        if (cp->comment_text != NULL)
            free(cp->comment_text);
        cp->comment_text = NULL;
        cp = cp->next;
        }

    fprintf(F,"*/\n");
    set_indent();

    for (cp = c; cp;)
    {
        c = cp;
        cp = cp->next;
        free(c);
    }
}

#ifdef __STDC__
char *get_family(unsigned long family)
#else
char *get_family(family)
unsigned long family;
#endif
/* returns the family name given its integer representation */
{
    switch(family)
    {
        case socket_$unspec:
            return "unspec";
        case socket_$internet:
            return "ip";
        case socket_$dds:
            return "dds";
        default:
            warn("Unsupported socket address family encountered, substituting 'ip'");
            return "ip";
    }
}

#ifdef __STDC__
char *get_name(NAMETABLE_id_t name)
#else
char *get_name(name)
NAMETABLE_id_t name;
#endif

{
    char *str;

    if (name == NAMETABLE_NIL_ID) return "";
    NAMETABLE_id_to_string (name, &str);

    /* Translate those symbols possible to their new represenatations */
    if (strcmp (str, "uuid_$t") == 0) return "uuid_t";
    if (strcmp (str, "status_$ok") == 0) return "error_status_ok";
    if (strcmp (str, "status_$t") == 0) return "error_status_t";
    if (strcmp (str, "pinteger") == 0) return "unsigned16";
    if (strcmp (str, "binteger") == 0) return "unsigned8";
    if (strcmp (str, "linteger") == 0) return "unsigned32";

    return str;
}

#ifdef __STDC__
char *get_string(STRTAB_str_t string)
#else
char *get_string(string)
STRTAB_str_t string;
#endif
{
    char *str;

    STRTAB_str_to_string(string, &str);
    return str;
}

#ifdef __STDC__
static void spell_indices(array_index_t *indices)
#else
static void spell_indices(indices)
array_index_t *indices;
#endif
/* emits an index string for an array */
{
    type_t *type;
    int adjust_upper = 0; /* set to true if array is 1-based and upper-bound must be decremented */
    constant_t *index;

    while (indices)
    {
        /* since we're not emitting subrange definitions,
           we explicitly spell out all subranges in array indices */

        type = indices->type;
        while (type->kind == named_k)
            type = type->type_structure.named.resolution;

        if (language_style == c_style)
        {
            switch (type->type_structure.subrange.upper_bound->kind)
            {
                case integer_k:
                    fprintf(F, "[%d]", type->type_structure.subrange.upper_bound->value.int_val);
                    break;
                case named_const_k:
                    fprintf(F, "[%s]",
                        get_name(type->type_structure.subrange.upper_bound->value.named_val.name));
                    break;
            }
        }
        else
        {
            switch (type->type_structure.subrange.lower_bound->kind)
            {
                case integer_k:
                    /*
                     * If the original language was pascal and the lower bound was 1, then this
                     * is the same as a C array with a lower bound of 0.
                     */
                    if (type->type_structure.subrange.lower_bound->value.int_val == 1)
                    {
                        fprintf(F, "[0..");
                        adjust_upper = 1; /* Upper bound should be decremented */
                    }
                    else
                        fprintf(F, "[%d..", type->type_structure.subrange.lower_bound->value.int_val);
                    break;
                case named_const_k:
                    {
                    char msg_buf[SMALL_BUFFER];

                    index = type->type_structure.subrange.lower_bound->value.named_val.resolution;
                    if (index->kind == integer_k)
                    {
                        if (index->value.int_val == 1) adjust_upper = 1;
                        if (index->value.int_val != 0)
                        {
                            sprintf(msg_buf, "Identifier '%s': named lower bound not adjusted (C arrays are 0 based)",
                                   get_name(type->type_structure.subrange.lower_bound->value.named_val.name));
                            warn(msg_buf);
                        }
                    }
                    fprintf(F, "[%s..",
                        get_name(type->type_structure.subrange.lower_bound->value.named_val.name));
                    break;
                    }
            }
            switch (type->type_structure.subrange.upper_bound->kind)
            {
                case integer_k:
                    if (adjust_upper)
                        fprintf(F, "%d]", type->type_structure.subrange.upper_bound->value.int_val-1);
                    else
                        fprintf(F, "%d]", type->type_structure.subrange.upper_bound->value.int_val);
                    break;
                case named_const_k:
                    {
                    char msg_buf[SMALL_BUFFER];
                    if (adjust_upper)
                    {
                        sprintf(msg_buf, "Identifier '%s': named upper bound not adjusted (C arrays are 0 based)",
                            get_name(type->type_structure.subrange.upper_bound->value.named_val.name));
                        warn(msg_buf);
                    }
                    fprintf(F, "%s]",
                        get_name(type->type_structure.subrange.upper_bound->value.named_val.name));
                    break;
                    }
            }
        }
        indices = indices->next;
    }
}

#ifdef __STDC__
static void spell_enum(enumeration_t *enumb, char *suffix)
#else
static void spell_enum(enumb, suffix)
enumeration_t *enumb;
char *suffix;
#endif
/* emits an enumeration declaration and set of constants */
{
    binding_t *next_enum;

    next_enum = enumb->enum_constants;
    indent_more();
    fprintf(F, "{\n");

    while (next_enum)
    {
        set_indent();
        OUTPUT_COMMENTS(next_enum->comment_text);
        OUTPUT_COMMENTS(next_enum->binding->constant.comment_text);

        fprintf(F, "%s%s",get_name(next_enum->name), suffix);
        next_enum = next_enum->next;
        if (next_enum) {
            fprintf(F,",\n");
            }
    }

    indent_less();
    fprintf(F, "} ");
}


#ifdef __STDC__
static void spell_bitset(enumeration_t *enumb, char *suffix)
#else
static void spell_bitset(enumb, suffix)
enumeration_t *enumb;
char *suffix;
#endif
/* emits an set of bitset constant declarations */
{
    binding_t *next_enum;
    int value = 1;

    next_enum = enumb->enum_constants;
    indent_more();

    while (next_enum)
    {
        set_indent();
        OUTPUT_COMMENTS(next_enum->comment_text);
        OUTPUT_COMMENTS(next_enum->binding->constant.comment_text);
        fprintf(F, "const long int %s%s = 0x%x",
          get_name(next_enum->name), suffix, value);
        value = value << 1;
        next_enum = next_enum->next;
        if (next_enum) fprintf(F,";\n");
    }

    indent_less();
}

#ifdef __STDC__
static void spell_field_attrs(field_t *field)
#else
static void spell_field_attrs(field)
field_t *field;
#endif
/* emits field attributes */
{
    OUTPUT_COMMENTS(field->comment_text);
    fprintf(F, "[v1_array");
    if (field->last_is_ref)
    {
        /*
         * Since Pascal arrays are 1-based, then use length_is() instead last_is()
         * and size_is() instead of max_is().  If the pascal has a lower bound other
         * than one the IDL compiler will complain.
         */
        if (language_style != c_style)
        {
            fprintf(F, ", length_is(%s)", get_name(field->last_is_ref->name));
            if (field->type->kind == open_array_k && !field->max_is_ref)
                fprintf(F, ", size_is(%s)", get_name(field->last_is_ref->name));
            else if (field->type->kind == named_k && ! field->max_is_ref &&
                (field->type->type_structure.named.resolution->kind == open_array_k))
                fprintf(F, ", size_is(%s)", get_name(field->last_is_ref->name));
        }
        else {
            fprintf(F, ", last_is(%s)", get_name(field->last_is_ref->name));
            if (field->type->kind == open_array_k && !field->max_is_ref)
                fprintf(F, ", max_is(%s)", get_name(field->last_is_ref->name));
            else if (field->type->kind == named_k && ! field->max_is_ref &&
                (field->type->type_structure.named.resolution->kind == open_array_k))
                fprintf(F, ", max_is(%s)", get_name(field->last_is_ref->name));
        }
    }

    if (field->max_is_ref)
        fprintf(F, ", max_is(%s)", get_name(field->max_is_ref->name));

    if (field->type->kind == fixed_string_zero_k)
        fprintf(F, ", v1_string");
    if (field->type->kind == record_k)
        fprintf(F, ", v1_struct");
    if (field->type->kind == long_enumeration_k)
        fprintf(F, ", v1_enum");
    fprintf(F, "] ");
}

#ifdef __STDC__
static void spell_variant(variant_t *variant)
#else
static void spell_variant(variant)
variant_t *variant;
#endif
/* emits a union switch declaration */
{
    tag_t *next_tag;
    field_t *next_field;
    binding_t *next_enum;
    component_t *next_component;
    int multiple_fields;
    long ordinal_mapping;

    indent_more();

    fprintf(F, "union switch (");
    spell_typed_name(variant->tag_type, variant->tag_id);
    fprintf(F, ") %s\n", get_name(variant->label));
    set_indent();
    fprintf(F, "{\n");
    indent_more();
    next_component = variant->components;

    while (next_component)
    {
        next_tag = next_component->tags;
        if (next_tag == NULL)
        {
            set_indent();
            fprintf(F, "default:");
        }
        else while (next_tag)
        {
            set_indent();
            switch (next_tag->tag_value.kind)
            {
                case integer_k:
                    fprintf(F, "case %d:\n", next_tag->tag_value.value.int_val);
                    break;
                case named_const_k:
                    fprintf(F, "case %s:\n", get_name(next_tag->tag_value.value.named_val.name));
                    break;
                case boolean_const_k:
                    fprintf(F, "case %s:\n", next_tag->tag_value.value.boolean_value?"true":"false");
                    break;
                case enum_k:
                    ordinal_mapping = next_tag->tag_value.value.enum_val.ordinal_mapping;
                    next_enum =
                    next_tag->tag_value.value.enum_val.base_type->type_structure.enumeration.enum_constants;
                    while (ordinal_mapping)
                    {
                        Assert(next_enum);
                        next_enum = next_enum->next;
                        ordinal_mapping--;
                    }
                    Assert(next_enum);
                    fprintf(F, "case %s:", get_name(next_enum->name));
                    break;
                default:
                    fatal_error("Invalid union case type encountered");
            }
            next_tag = next_tag->next_tag;
        }

        next_field = next_component->fields;

        if (!next_field)
        {
            fprintf(F, ";\n");
            multiple_fields = 0;
        }
        else if (multiple_fields = next_field->next_field ? 1 : 0)
            fprintf(F, "struct\n{\n");

        while (next_field)
        {
            set_indent();
            if (next_field->type->kind == fixed_array_k || next_field->type->kind == open_array_k || next_field->type->kind == fixed_string_zero_k)
                spell_field_attrs(next_field);
            else if (next_field->type->kind == record_k)
                fprintf(F, "[v1_struct]");
            else if (next_field->type->kind == long_enumeration_k)
                fprintf(F, "[v1_enum]");

            spell_typed_name(next_field->type, next_field->name);
            fprintf(F, ";\n");
            next_field = next_field->next_field;
        }

        if (multiple_fields)
        {
            set_indent();
            fprintf(F, "} %s;\n", get_name(next_component->label));
        }

        next_component = next_component->next_component;
    }

    indent_less();
    set_indent();
    fprintf(F, "}");
    indent_less();
}

#ifdef __STDC__
static void spell_struct(record_t *structb)
#else
static void spell_struct(structb)
record_t *structb;
#endif
/* emits and a structure declaration or invokes
   spell_variant to return a union switch declaration */
{
    field_t *next_field;

    OUTPUT_COMMENTS(structb->comment_text);
    if (structb->variant)
    {
        if (structb->fields)
            fatal_error("Variant records embedded in structures are unsupported");
        else spell_variant(structb->variant);
        return;
    }

    fprintf(F, "struct %s\n", get_name(structb->tag_name));
    set_indent();
    fprintf(F, "{\n");
    indent_more();

    next_field = structb->fields;
    while (next_field)
    {
        set_indent();
        if (next_field->type->kind == fixed_array_k || next_field->type->kind == open_array_k)
            spell_field_attrs(next_field);
        spell_typed_name(next_field->type, next_field->name);
        fprintf(F, ";\n");
        next_field = next_field->next_field;
    }
    indent_less();
    set_indent();
    fprintf(F, "}");
}

#ifdef __STDC__
static void spell_type(type_t *type, char *name)
#else
static void spell_type(type, name)
type_t *type;
char *name;
#endif
{
    char *buf;

    OUTPUT_COMMENTS(type->comment_text);

    switch(type->kind)
    {
        case drep_k:
            fprintf(F, "rpc_drep_t %s", name);
            break;
        case void_k:
            fprintf(F, "void %s", name);
            break;
        case boolean_k:
            fprintf(F, "boolean %s", name);
            break;
        case byte_k:
            fprintf(F, "byte %s", name);
            break;
        case character_k:
            fprintf(F, "char %s", name);
            break;
        case small_integer_k:
            fprintf(F, "small %s", name);
            break;
        case short_integer_k:
            fprintf(F, "short %s", name);
            break;
        case long_integer_k:
            fprintf(F, "long %s", name);
            break;
        case hyper_integer_k:
            fprintf(F, "hyper %s", name);
            break;
        case small_unsigned_k:
            fprintf(F, "unsigned small %s", name);
            break;
        case short_unsigned_k:
            fprintf(F, "unsigned short %s", name);
            break;
        case long_unsigned_k:
            fprintf(F, "unsigned long %s", name);
            break;
        case hyper_unsigned_k:
            fprintf(F, "unsigned hyper %s", name);
            break;
        case short_real_k:
            fprintf(F, "float %s", name);
            break;
        case long_real_k:
            fprintf(F, "double %s", name);
            break;
        case handle_k:
            fprintf(F, "handle_t %s", name);
            break;
        case small_bitset_k:
        case short_bitset_k:
        case long_bitset_k:
            if (type->kind == small_bitset_k) fprintf(F, "small ");
            if (type->kind == short_bitset_k) fprintf(F, "short ");
            if (type->kind == long_bitset_k) fprintf(F, "long ");
            /* we must explicitly spell out named (enum) basetypes of bitsets */
            type = type->type_structure.bitset.base_type;
            while (type->kind == named_k) type = type->type_structure.named.resolution;
            OUTPUT_COMMENTS(type->comment_text);
            fprintf(F, "int %s;\n",name);
            spell_bitset(&type->type_structure.enumeration,"_mask");
            break;
        case small_enumeration_k:
        case short_enumeration_k:
        case long_enumeration_k:
            fprintf(F, "enum ");
            spell_enum(&type->type_structure.enumeration,"");
            fprintf(F, " %s", name);
            break;
        case fixed_string_zero_k:
            fprintf(F, "char %s", name);
            /* coerce the lower bound of the index of pascal strings to 1 */
            if (language_style != c_style)
                type->type_structure.fixed_string_zero.index->type->type_structure.subrange.lower_bound->value.int_val = 1;
            spell_indices(type->type_structure.fixed_string_zero.index);
            break;
        case fixed_array_k:
            spell_type(type->type_structure.fixed_array.elements, name);
            spell_indices(type->type_structure.fixed_array.indices);
            break;
        case open_array_k:
            spell_type(type->type_structure.open_array.elements, name);
            fprintf(F, "[]");
            spell_indices(type->type_structure.open_array.indices);
            break;
        case record_k:
            spell_struct(&type->type_structure.record);
            fprintf(F, " %s", name);
            break;
        case named_k:
            {
            if ((type->type_structure.named.resolution->kind == record_k) &&
                (type->type_structure.named.resolution->type_structure.record.tag_name != NAMETABLE_NIL_ID))
                    fprintf(F, "struct %s %s", get_name(type->type_structure.named.resolution->type_structure.record.tag_name), name);
            else if ((type->type_structure.named.resolution->kind == user_marshalled_k) &&
                (type->type_structure.named.resolution->type_structure.user_marshalled.user_type->type_structure.record.tag_name != NAMETABLE_NIL_ID))
                    fprintf(F, "struct %s %s", get_name(type->type_structure.named.resolution->type_structure.user_marshalled.user_type->type_structure.record.tag_name), name);
            else
                fprintf(F, "%s %s", get_name(type->type_structure.named.name), name);
            break;
            }
        case reference_k:
        case pointer_k:
            buf = alloc(2+len(name));
            sprintf(buf, "*%s", name);
            spell_type(type->type_structure.pointer.pointee, buf);
            free(buf);
            break;
        case user_marshalled_k:
            spell_type(type->type_structure.user_marshalled.user_type, name);
            break;
        case routine_ptr_k:
            buf = alloc(4+len(name));
            sprintf(buf, "(*%s)", name);
            spell_type(type->type_structure.routine_ptr.result->type, buf);
            fprintf(F, "(\n");
            emit_parameters(type->type_structure.routine_ptr.parameters,
                            NAMETABLE_add_id(name, false));
            fprintf(F, "\n");
            set_indent();
            fprintf(F, ")\n");
            break;

        case small_subrange_k:
        case short_subrange_k:
        case long_subrange_k:
            {
            char msg_buf[SMALL_BUFFER];
            sprintf(msg_buf, "Identifier '%s': subrange definition omitted, translated as int", name);
            warn(msg_buf);

            if (type->kind == small_subrange_k) fprintf(F, "small ");
            if (type->kind == short_subrange_k) fprintf(F, "short ");
            if (type->kind == long_subrange_k) fprintf(F, "long ");
            fprintf(F, "int %s", name);
            break;
            }

        case open_string_zero_k:
        case univ_ptr_k:
        default:
            unimp_type_exit(type->kind, "spell_type");
    }
}

#ifdef __STDC__
void spell_typed_name(type_t *type, NAMETABLE_id_t name)
#else
void spell_typed_name(type, name)
type_t *type;
NAMETABLE_id_t name;
#endif
{
    spell_type(type, get_name(name));
}

#ifdef __STDC__
static type_t *gen_type_node(type_kind kind)
#else
static type_t *gen_type_node(kind)
type_kind kind;
#endif
{
    structure_t *sp;
    sp = AST_type_node (kind);
    return &sp->type;
}

#ifdef __STDC__
static type_t *pointerize_type(type_t *tp)
#else
static type_t *pointerize_type(tp)
type_t *tp;
#endif
{
    type_t    *new_tp;
    type_kind focus_kind;


    /*
     * Determine what to pointerize (if it is a named type, we must go down a level)
     */
    switch (tp->kind) {
        case named_k:
            focus_kind = tp->type_structure.named.resolution->kind;
            break;
        default:
            focus_kind = tp->kind;
            break;
        }

    /*
     * Now, determine what to do to make this into a pointer
     */
    switch (focus_kind) {

        case fixed_array_k:
        case open_array_k:
        case fixed_string_zero_k:
        case open_string_zero_k:
            /* These are already passed by reference, don't pointerize */
            return tp;

        /*
         * To turn these into a pointer, just add a pointer node.
         */
        case pointer_k:
        default:
            /* create new pointer type node */
            new_tp = gen_type_node (pointer_k);
            new_tp->type_structure.pointer.pointee = tp;

            /* propagate pointee's attributes */
            new_tp->xmit_type   = tp->xmit_type;

            return new_tp;
        }
    /*lint -unreachable*/
}

#ifdef __STDC__
void pointerize_routine (routine_t *rp)
#else
void pointerize_routine (rp)
routine_t *rp;
#endif
{
    parameter_t *pp;

    if (!rp->pointerized) {
        rp->pointerized = true;
        for (pp = rp->parameters; pp; pp = pp->next_param)
            /*
             * If it is an out parameter or passed by reference, we must make
             * the parameter a pointer such that the * is materialized explicity
             * in the routine signiture for V2.
             */
            if (pp->out || pp->ref) {
                pp->type = pointerize_type (pp->type);
            }
        }
}
