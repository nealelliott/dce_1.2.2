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
 * $Log: forhdgen.c,v $
 * Revision 1.1.3.2  1996/03/09  20:44:53  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:35  marty]
 *
 * Revision 1.1.3.1  1995/12/07  22:22:53  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:03 UTC  dat
 * 	New file for second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:31:46  root]
 * 
 * 	may 95 idl drop
 * 	[1995/10/23  01:56:17  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/23  01:55:14  bfc]
 * 
 * $EndLog$
 */
/*
**  NAME:
**
**      forhdgen.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  BE_gen_fortran_header which emits the FORTRAN header file for an interface.
**
*/

#include <ctype.h>
#if defined(vax) && !defined(vms)
# include <varargs.h>
#else
# include <stdarg.h>
#endif
#include <nidl.h>
#include <forhdgen.h>
#include <astp.h>
#include <bedeck.h>
#include <checker.h>
#include <command.h>
#include <ddbe.h>
#include <irep.h>
#include <mtsbacke.h>


/* Define pointer size in bytes */
#if defined(__alpha) && defined(__osf__)
#define IDL_PSZ 8
#else
#define IDL_PSZ 4
#endif


/* Static copy of interface node pointer used for error reporting */
static AST_interface_n_t *FSPELL_int_p;

/* Flags for error messages that are output at most once */
static boolean FSPELL_forpointer    = FALSE;
static boolean FSPELL_forsignint    = FALSE;
static boolean FSPELL_langpipesns   = FALSE;

/*
 * Forward static function declarations.
 */
static void FSPELL_constant_def(
#ifdef PROTO
    FILE                *fid,           /* [in] Output file handle */
    AST_constant_n_t    *cp             /* [in] AST constant node */
#endif
);

static boolean FSPELL_typed_name(
#ifdef PROTO
    FILE                *fid,           /* [in] Output file handle */
    AST_type_n_t        *type_p,        /* [in] AST type node */
    AST_instance_n_t    *inst_p,        /* [in] AST instance node (if any) */
    NAMETABLE_id_t      inst_name       /* [in] Instance name (if any) */
#endif
);

static void FSPELL_type_def(
#ifdef PROTO
    FILE                *fid,           /* [in] Output file handle */
    AST_type_n_t        *tp             /* [in] AST type node */
#endif
);



#define LINE_BUFF_SIZE  512             /* Longest internal line length */
#define MAX_LINE_LEN     72             /* Longest output line length */
#define CONT_COLUMN       6             /* FORTRAN source continuation column */
#define CONT_CHAR       '*'             /* FORTRAN continuation character */
#define COMMENT_CHAR    'C'             /* FORTRAN comment character */

static char out_buffer[LINE_BUFF_SIZE]; /* Buffer for outputs */
static int buff_len = 0;                /* Length of current data in buffer */

/*
 *  F S P E L L _ o u t p u t _ l i n e
 *
 *  Replacement routine for fprintf() for use when spelling FORTRAN source
 *  so that output is broken up into multiple lines with continuation
 *  characters if necessary.
 *
 *  Implicit inputs:  out_buffer, buff_len
 *
 *  Assumption: Long output lines will have space (' ') delimiter(s).
 */
int FSPELL_output_line
#ifdef PROTO
(
    FILE * fid,                 /* [in] File handle */
    char *format,               /* [in] Format string */
    ...                         /* [in] 0-N format arguments */
)
#else
(fid, format, va_alist)
    FILE * fid;                 /* [in] File handle */
    char *format;               /* [in] Format string */
    va_dcl                      /* [in] 0-N format arguments */
#endif
{
    va_list args;
    char    *buff;              /* Pointer somewhere into out_buffer */
    int     num_chars;          /* Number chars formatted */
    int     max_len;            /* Maximum length of remainder of output line */
    int     line_len;           /* Length of one line of output */
    boolean comment_line;       /* TRUE => first char of line is comment char */

#if defined(vax) && !defined(vms)
    va_start (args);
#else
    va_start (args, format);
#endif

    num_chars = vsprintf(&out_buffer[buff_len], format, args);
    if (num_chars <= 0)
        return num_chars;

    buff_len += num_chars;

    /*
     * We only do output if the last character is a newline.
     */
    if (out_buffer[buff_len-1] != '\n')
        return num_chars;

    /*
     * Split the line up by continually looking for a space near the end
     * of the output line length limit and changing into a NULL character.
     * Assumes that there will in fact be a space within the limit.
     */
    max_len = MAX_LINE_LEN;
    comment_line = (out_buffer[0] == COMMENT_CHAR);

    for (buff = out_buffer; buff_len > MAX_LINE_LEN; )
    {
        for (line_len = max_len; line_len >= 0; )
            if (buff[--line_len] == ' ')
                break;

        buff[line_len] = '\0';
        fprintf(fid, "%s\n", buff);

        /*
         * There are two forms of splitting lines: if the line is a comment,
         * start the next line with the comment character and some spaces,
         * otherwise start the next line with the continuation character in
         * the appropriate column.
         */
        if (comment_line)
        {
            fprintf(fid, "%c       ", COMMENT_CHAR);
            max_len = MAX_LINE_LEN - 8;
        }
        else
        {
            fprintf(fid, "%*c  ", CONT_COLUMN, CONT_CHAR);
            max_len = MAX_LINE_LEN - CONT_COLUMN - 2;
        }

        /*
         * Account for the space character we overwrote.
         * Adjust buffer pointer and length variables.
         */
        line_len++;
        buff += line_len;
        buff_len -= line_len;
    }

    /*
     * Output the residual part of the line, if any, it ends with a newline.
     */
    if (buff_len > 0)
        fprintf(fid, "%s", buff);

    buff_len = 0;
    return num_chars;
}


/*
 *  F S P E L L _ w a r n _ o n c e
 *
 *  Log a one-time warning message.
 */
void FSPELL_warn_once
#ifdef PROTO
(
    boolean         *once_flag,     /* [io] TRUE => message already logged */
    ASTP_node_t     *node_p,        /* [in] Node to extract source info from */
    unsigned long   message_id,     /* [in] Message identifier */
    unsigned long   message_arg     /* [in] Message argument */
)
#else
(once_flag, node_p, message_id, message_arg)
    boolean         *once_flag;     /* [io] TRUE => message already logged */
    ASTP_node_t     *node_p;        /* [in] Node to extract source info from */
    unsigned long   message_id;     /* [in] Message identifier */
    unsigned long   message_arg;    /* [in] Message argument */
#endif
{
    if (!(*once_flag))
    {
        *once_flag = TRUE;
        /*
         * If no source line to hang the message off, use interface node lineno.
         */
        if (node_p == NULL || node_p->fe_info == NULL
            || node_p->fe_info->file == STRTAB_NULL_STR)
            node_p = (ASTP_node_t *)FSPELL_int_p;
        CHECKER_warning(node_p, message_id, message_arg);
    }
}


/*
 *  F S P E L L _ s t r u c p y
 *
 *  Copies a mixed-case source string to an upper-case destination string.
 *  The address of the upcased string argument is returned as the function
 *  result so that this routine can be used in context requiring a (char *).
 */
char *FSPELL_strucpy
#ifdef PROTO
(
    char    *dst,   /* [io] Destination upper-case string */
    char    *src    /* [in] Source mixed-case string */
)
#else
(dst, src)
    char    *dst;   /* [io] Destination upper-case string */
    char    *src;   /* [in] Source mixed-case string */
#endif
{
    char    uc;     /* Upcased character */
    char    *dp;    /* Ptr to character in destination string */

    for (dp = dst; ; src++)
    {
        uc = toupper(*src);
        *dp++ = uc;
        if (uc == '\0')
            break;
    }

    return dst;
}


/*
 *  F S P E L L _ i d _ t o _ u c _ s t r i n g
 *
 *  Given a nametable ID, returns an upcased copy of the corresponding string.
 *  The address of the upcased string argument is returned as the function
 *  result so that this routine can be used in context requiring a (char *).
 */
char *FSPELL_id_to_uc_string
#ifdef PROTO
(
    NAMETABLE_id_t      id,             /* [in] Nametable ID */
    char                uc_string[]     /* [io] Upcased string */
)
#else
(id, uc_string)
    NAMETABLE_id_t      id;             /* [in] Nametable ID */
    char                uc_string[];    /* [io] Upcased string */
#endif
{
    char    *mc_string;     /* Mixed case string */

    NAMETABLE_id_to_string(id, &mc_string);
    FSPELL_strucpy(uc_string, mc_string);

    return uc_string;       /* Return address of upcased string */
}


/*
 *  F S P E L L _ s t r _ t o _ u c _ s t r i n g
 *
 *  Given a stringtable ID, returns an upcased copy of the corresponding string.
 *  The address of the upcased string argument is returned as the function
 *  result so that this routine can be used in context requiring a (char *).
 */
char *FSPELL_str_to_uc_string
#ifdef PROTO
(
    STRTAB_str_t        str,            /* [in] String handle */
    char                uc_string[]     /* [io] Upcased string */
)
#else
(str, uc_string)
    STRTAB_str_t        str;            /* [in] String handle */
    char                uc_string[];    /* [io] Upcased string */
#endif
{
    char    *mc_string;     /* Mixed case string */

    STRTAB_str_to_string(str, &mc_string);
    FSPELL_strucpy(uc_string, mc_string);

    return uc_string;       /* Return address of upcased string */
}


/*
 *  F S P E L L _ m a p c h a r
 *
 *  Maps a single character into a string suitable for emission.
 *  If the character doesn't translate into something representable in
 *  FORTRAN, a warning is issued and "?" is returned.
 */
char *FSPELL_mapchar
#ifdef PROTO
(
    AST_constant_n_t *const_p,  /* [in] AST constant node used for error case */
    char c,                     /* [in] Character to map to FORTRAN format */
    boolean warning_flag        /* [in] TRUE => emit warning if doesn't map */
)
#else
(const_p, c, warning_flag)
    AST_constant_n_t *const_p;  /* [in] AST constant node used for error case */
    char c;                     /* [in] Character to map to FORTRAN format */
    boolean warning_flag;       /* [in] TRUE => emit warning if doesn't map */
#endif
{
    static char buf[10];

    switch (c)
    {
        case AUDIBLE_BELL:
        case '\b':
        case '\f':
        case '\n':
        case '\r':
        case '\v':
            /* Character doesn't map to target language */
            CHECKER_warning(const_p, NIDL_LANGMAPCHAR);
            return "?";
        case '\t': return "\t";
        case '\\': return "\\";
        case '\'': return "\'\'";
        case '\"': return "\"";
        default:
            if (c >= ' ' && c <= '~')
                sprintf(buf, "%c", c);
            else
            {
                /* Character doesn't map to target language */
                CHECKER_warning(const_p, NIDL_LANGMAPCHAR);
                sprintf(buf, "?");
            }
            return buf;
    }
}


/*
 *  F S P E L L _ m a p o c t a l
 *
 *  Maps an IDL octal text string that represents an "escape sequence"
 *  into its equivalent FORTRAN character, or '?' if the escape sequence
 *  translates into something that is not representable in FORTRAN.
 *  Updates pointers past the escape sequence and mapped translation.
 */
void FSPELL_mapoctal
#ifdef PROTO
(
    AST_constant_n_t *const_p,  /* [in] AST constant node used for error case */
    char    **istring,      /* [io] IDL string */
    char    **fstring,      /* [io] FORTRAN string */
    boolean warning_flag    /* [in] TRUE => emit warning if char doesn't map */
)
#else
(const_p, istring, fstring, warning_flag)
    AST_constant_n_t *const_p;  /* [in] AST constant node used for error case */
    char    **istring;      /* [io] IDL string */
    char    **fstring;      /* [io] FORTRAN string */
    boolean warning_flag;   /* [in] TRUE => emit warning if char doesn't map */
#endif
{
    int     i, value = 0;
    char    *cp;

    for (i = 0, cp = *istring; i < 3; i++, cp++) /* 3 = max digits in esc seq */
    {
        if (*cp >= '0' && *cp <= '7')
            value = value * 8 + (*cp - '0');
        else
            break;
    }
    if (i == 0)
    {
        **fstring = '?';
        *fstring += 1;
        INTERNAL_ERROR("Invalid octal escape sequence in const string");
    }
    else
    {
        sprintf(*fstring, "%s",
                FSPELL_mapchar(const_p, (char)value, warning_flag));
        i = strlen(*fstring);
        *fstring += i;
    }

    *istring = cp;
}


/*
 *  F S P E L L _ m a p h e x
 *
 *  Maps an IDL hexadecimal text string that represents an "escape sequence"
 *  into its equivalent FORTRAN character, or '?' if the escape sequence
 *  translates into something that is not representable in FORTRAN.
 *  Updates pointers past the escape sequence and mapped translation.
 */
void FSPELL_maphex
#ifdef PROTO
(
    AST_constant_n_t *const_p,  /* [in] AST constant node used for error case */
    char    **istring,      /* [io] IDL string */
    char    **fstring,      /* [io] FORTRAN string */
    boolean warning_flag    /* [in] TRUE => emit warning if char doesn't map */
)
#else
(const_p, istring, fstring, warning_flag)
    AST_constant_n_t *const_p;  /* [in] AST constant node used for error case */
    char    **istring;      /* [io] IDL string */
    char    **fstring;      /* [io] FORTRAN string */
    boolean warning_flag;   /* [in] TRUE => emit warning if char doesn't map */
#endif
{
    int     i, value = 0;
    char    *cp;

    for (i = 0, cp = *istring; i < 3; i++, cp++) /* 3 = max digits in esc seq */
    {
        if (*cp >= '0' && *cp <= '9')
            value = value * 16 + (*cp - '0');
        else if (*cp >= 'a' && *cp <= 'f')
            value = value * 16 + (*cp - 'a' + 10);
        else if (*cp >= 'A' && *cp <= 'F')
            value = value * 16 + (*cp - 'A' + 10);
        else
            break;
    }
    if (i == 0)
    {
        **fstring = '?';
        *fstring += 1;
        INTERNAL_ERROR("Invalid hex escape sequence in const string");
    }
    else
    {
        sprintf(*fstring, "%s",
                FSPELL_mapchar(const_p, (char)value, warning_flag));
        i = strlen(*fstring);
        *fstring += i;
    }

    *istring = cp;
}


/*
 *  F S P E L L _ m a p s t r i n g
 *
 *  Maps an IDL string into its FORTRAN form suitable for emission.
 *  Note that the IDL string does not contain any unprintables; it is the IDL
 *  source for the string with backslashes introducing "escape sequences".
 *  If any character doesn't translate into something representable in
 *  FORTRAN, a warning is issued and '?' is generated for that character .
 *  The address of the FORTRAN string argument is returned as the function
 *  result so that this routine can be used in context requiring a (char *).
 */
char *FSPELL_mapstring
#ifdef PROTO
(
    AST_constant_n_t *const_p,  /* [in] AST constant node used for error case */
    char    *istring,       /* [in] IDL string */
    char    *fstring,       /* [out] FORTRAN string */
    boolean warning_flag    /* [in] TRUE => emit warning if char doesn't map */
)
#else
(const_p, istring, fstring, warning_flag)
    AST_constant_n_t *const_p;  /* [in] AST constant node used for error case */
    char    *istring;       /* [in] IDL string */
    char    *fstring;       /* [out] FORTRAN string */
    boolean warning_flag;   /* [in] TRUE => emit warning if char doesn't map */
#endif
{
    static char buf[10];
    char    *cp, *fp;

    for (cp = istring, fp = fstring; *cp != '\0'; )
    {
        switch (*cp)
        {
        case '\'':
            *fp++ = '\'';
            *fp++ = '\'';
            cp++;
            break;

        case '\\':
            cp++;
            switch (*cp++)
            {
            case 'a':
            case 'b':
            case 'f':
            case 'n':
            case 'r':
            case 'v':
                /* Character doesn't map to target language */
                CHECKER_warning(const_p, NIDL_LANGMAPCHAR);
                *fp++ = '?';
                break;
            case 't':
                *fp++ = '\t';
                break;
            case '\\':
                *fp++ = '\\';
                break;
            case '\'':
                *fp++ = '\'';
                *fp++ = '\'';
                break;
            case '\"':
                *fp++ = '"';
                break;
            case 'x':
                FSPELL_maphex(const_p, &cp, &fp, TRUE);
                break;
            default:
                if (*cp >= '0' && *cp <= '7')
                {
                    cp--;
                    FSPELL_mapoctal(const_p, &cp, &fp, TRUE);
                }
                else
                    INTERNAL_ERROR("Invalid escape sequence in const string");
            }
            break;

        default:
            *fp++ = *cp++;
        }
    }

    *fp = '\0';
    return fstring;
}


/*
 *  F S P E L L _ a r r a y _ b o u n d s
 *
 *  Spell array bounds syntax of the form:
 *      ( [lower-bound:] upper-bound [,...] )
 *  where each bound specifier is a constant value, constant name,
 *  parameter name, or an expression combining them.
 */
static void FSPELL_array_bounds
#ifdef PROTO
(
    FILE                *fid,           /* [in] Output file handle */
    AST_array_n_t       *array_p,       /* [in] AST array node */
    AST_instance_n_t    *inst_p         /* [in] AST instance node */
)
#else
(fid, array_p, inst_p)
    FILE                *fid;           /* [in] Output file handle */
    AST_array_n_t       *array_p;       /* [in] AST array node */
    AST_instance_n_t    *inst_p;        /* [in] AST instance node */
#endif
{
    AST_constant_n_t    *lower_p;
    AST_constant_n_t    *upper_p;
    AST_array_index_n_t *index_p;
    AST_field_attr_n_t  *fattr_p;
    int                 dim;
    boolean             param_inst; /* TRUE => parameter instance */
    char ucid[MAX_ID+1];            /* Upper case identifier */

    param_inst = (inst_p != NULL
                  && inst_p->fe_info->node_kind == fe_parameter_n_k);
    index_p = array_p->index_vec;
    fattr_p = (inst_p == NULL) ? NULL : inst_p->field_attrs;

    for (dim = 0; dim < array_p->index_count; dim++)
    {
        lower_p = index_p->lower_bound;
        upper_p = index_p->upper_bound;
        if (dim > 0)
            for_fprintf(fid, ", ");
        /*
         * Process lower bound.  Must check for min_is first since AST
         * array_rep_type for arrayified ptr can have constant lower bound
         * even if an instance of it has a min_is attribute.
         */
        if (fattr_p != NULL
            && fattr_p->min_is_vec != NULL && fattr_p->min_is_vec[dim].valid)
        {
            if (param_inst)
            {
                for_fprintf(fid, "%s:", FSPELL_id_to_uc_string(
                            fattr_p->min_is_vec[dim].ref.p_ref->name, ucid));
            }
            else
            {
                /*
                 * Unsupported field lower conformant bound; print upper bound
                 * if exists or zero.  (Warning message is handled elsewhere).
                 */
                if (upper_p != NULL)
                {
                    if (upper_p->name != NAMETABLE_NIL_ID)
                    {
                        for_fprintf(fid, "%s:",
                                FSPELL_id_to_uc_string(upper_p->name, ucid));
                    }
                    else if (upper_p->value.int_val != 1)
                        for_fprintf(fid, "%ld:", upper_p->value.int_val);
                }
                else
                    for_fprintf(fid, "0:");
            }
        }
        else    /* Constant lower bound */
        {
            /* FORTRAN arrays default to 1-based */
            if (lower_p->name != NAMETABLE_NIL_ID)
            {
                for_fprintf(fid, "%s:",
                            FSPELL_id_to_uc_string(lower_p->name, ucid));
            }
            else if (lower_p->value.int_val != 1)
                for_fprintf(fid, "%ld:", lower_p->value.int_val);
        }

        /*
         * Process upper bound.  Note: AST array_rep_type for arrayified ptr
         * always has a dynamic upper bound by definition of arrayifying, so
         * no need to test for dynamic before fixed bound as with above case.
         */
        if (upper_p != NULL)    /* Constant upper bound */
        {
            if (upper_p->name != NAMETABLE_NIL_ID)
            {
                for_fprintf(fid, "%s",
                            FSPELL_id_to_uc_string(upper_p->name, ucid));
            }
            else
                for_fprintf(fid, "%ld", upper_p->value.int_val);
        }
        else if (param_inst && fattr_p != NULL &&
                fattr_p->max_is_vec != NULL && fattr_p->max_is_vec[dim].valid)
        {
            for_fprintf(fid, "%s", FSPELL_id_to_uc_string(
                        fattr_p->max_is_vec[dim].ref.p_ref->name, ucid));
        }
        else if (param_inst && fattr_p != NULL &&
                fattr_p->size_is_vec != NULL && fattr_p->size_is_vec[dim].valid)
        {
            /* Spell [size_is] parameter reference */
            for_fprintf(fid, "%s", FSPELL_id_to_uc_string(
                        fattr_p->size_is_vec[dim].ref.p_ref->name, ucid));

            /* Now spell the rest of the expression if necessary */
            if (fattr_p->min_is_vec != NULL
                && fattr_p->min_is_vec[dim].valid)
            {
                for_fprintf(fid, "+%s-1", FSPELL_id_to_uc_string(
                            fattr_p->min_is_vec[dim].ref.p_ref->name, ucid));
            }
            else
            {
                /* FORTRAN arrays default to 1-based */
                if (lower_p->name != NAMETABLE_NIL_ID)
                {
                    for_fprintf(fid, "+%s-1",
                                FSPELL_id_to_uc_string(lower_p->name, ucid));
                }
                else if (lower_p->value.int_val != 1)
                {
                    /* Spell rest of upper bound expression: +n-1 or -n-1 */
                    if (lower_p->value.int_val != 0)
                        for_fprintf(fid, "%+ld", lower_p->value.int_val);
                    for_fprintf(fid, "-1");
                }
            }
        }
        else
        {
            /*
             * Unsupported field upper conformant bound - repeat lower bound if
             * exists or zero; warning message is handled elsewhere.  Note that
             * conformant parameters without an explicit upper bound attribute
             * (strings) are handled elsewhere.
             */
            if (param_inst)
                INTERNAL_ERROR("Incorrectly handled conformant parameter");
            if (lower_p != NULL)
            {
                if (lower_p->name != NAMETABLE_NIL_ID)
                {
                    for_fprintf(fid, "%s",
                                FSPELL_id_to_uc_string(lower_p->name, ucid));
                }
                else
                    for_fprintf(fid, "%ld", lower_p->value.int_val);
            }
            else
                for_fprintf(fid, "0");
        }

        index_p++;
    }
}


/*
 *  F S P E L L _ n e s t e d _ a n o n _ t y p e
 *
 *  Spell a nested definition for an anonymous structure or union type.
 */
static void FSPELL_nested_anon_type
#ifdef PROTO
(
    FILE                *fid,           /* [in] Output file handle */
    AST_type_n_t        *type_p,        /* [in] AST type node */
    AST_instance_n_t    *inst_p,        /* [in] AST instance node */
    NAMETABLE_id_t      inst_name,      /* [in] Instance name */
    int                 indent_lvl      /* [in] Indentation level */
)
#else
(fid, type_p, inst_p, inst_name, indent_lvl)
    FILE                *fid;           /* [in] Output file handle */
    AST_type_n_t        *type_p;        /* [in] AST type node */
    AST_instance_n_t    *inst_p;        /* [in] AST instance node (if any) */
    NAMETABLE_id_t      inst_name;      /* [in] Instance name (if any) */
    int                 indent_lvl;     /* [in] Indentation level */
#endif
{
    AST_field_n_t       *field_p;
    AST_arm_n_t         *arm_p;
    AST_disc_union_n_t  *union_p;
    int                 indent;         /* Extra chars of indentation */
    boolean             n_e_union;      /* TRUE => non-encapsulated union */
    char ucid[MAX_ID+1];                /* Upper case identifier */

    if (type_p->kind == AST_structure_k && AST_CONFORMANT_SET(type_p))
    {
        /* Conformant structures not supported in target language */
        CHECKER_warning(type_p, NIDL_LANGCFMTSTRCT);
    }

    indent = indent_lvl * 4;
    for_fprintf(fid, "      %*sSTRUCTURE %s\n", indent, "",
                FSPELL_id_to_uc_string(inst_name, ucid));

    switch (type_p->kind)
    {
    case AST_structure_k:
        /*
         * Recurse to emit the individual fields.
         */
        for (field_p = type_p->type_structure.structure->fields;
             field_p != NULL;
             field_p = field_p->next)
        {
            if (field_p->type->name == NAMETABLE_NIL_ID
                && (field_p->type->kind == AST_structure_k
                    || field_p->type->kind == AST_disc_union_k))
            {
                FSPELL_nested_anon_type(fid, field_p->type,
                    (AST_instance_n_t *)field_p, field_p->name, indent_lvl+1);
            }
            else
            {
                for_fprintf(fid, "    %*s", indent, "");
                FSPELL_typed_name(fid, field_p->type,
                                  (AST_instance_n_t *)field_p, field_p->name);
                for_fprintf(fid, "\n");
            }
        }
        break;

    case AST_disc_union_k:
        /*
         * If encapsulated union, output discriminator field.
         */
        union_p = type_p->type_structure.disc_union;
        n_e_union = (union_p->discrim_name == NAMETABLE_NIL_ID);
        if (!n_e_union)
        {
            for_fprintf(fid, "    %*s", indent, "");
            FSPELL_typed_name(fid, union_p->discrim_type, NULL,
                              union_p->discrim_name);
            for_fprintf(fid, "\n");
        }
            
        /*
         * Recurse to emit the individual arms.
         */
        for_fprintf(fid, "          %*sUNION\n", indent, "");
        for (arm_p = union_p->arms; arm_p != NULL; arm_p = arm_p->next)
        {
            for_fprintf(fid, "              %*sMAP\n", indent, "");
            if (arm_p->type != NULL
                && arm_p->type->name == NAMETABLE_NIL_ID
                && (arm_p->type->kind == AST_structure_k
                    || arm_p->type->kind == AST_disc_union_k))
            {
                FSPELL_nested_anon_type(fid, arm_p->type,
                    (AST_instance_n_t *)arm_p, arm_p->name, indent_lvl+3);
            }
            else if (arm_p->type != NULL)
            {
                for_fprintf(fid, "            %*s", indent, "");
                FSPELL_typed_name(fid, arm_p->type, (AST_instance_n_t *)arm_p,
                                  arm_p->name);
                for_fprintf(fid, "\n");
            }
            for_fprintf(fid, "              %*sEND MAP\n", indent, "");
        }
        for_fprintf(fid, "          %*sEND UNION\n", indent, "");
        break;

    default:
        INTERNAL_ERROR("Invalid type kind in FSPELL_nested_anon_type");
    }

    for_fprintf(fid, "      %*sEND STRUCTURE\n", indent, "");
}


/*
 *  F S P E L L _ t y p e d _ n a m e
 *
 *  Spell a definition or declaration of a type.  A definition is implied iff
 *  (inst_p == NULL && inst_name == NAMETABLE_NIL_ID).  Definitions are only
 *  relevant to structure, union, and enum types.
 *
 *   a) definition - the type name and a complete description of the type is
 *      spelled.  In any situations where a name is required but the IDL type is
 *      anonymous (e.g. a nested struct type) a type name will be constructed.
 *
 *   b) declaration - a reference to the type is spelled followed by the
 *      instance name passed in the 'inst_name' argument.
 */
static boolean FSPELL_typed_name        /* Returns TRUE if anything spelled */
#ifdef PROTO
(
    FILE                *fid,           /* [in] Output file handle */
    AST_type_n_t        *type_p,        /* [in] AST type node */
    AST_instance_n_t    *inst_p,        /* [in] AST instance node (if any) */
    NAMETABLE_id_t      inst_name       /* [in] Instance name (if any) */
)
#else
(fid, type_p, inst_p, inst_name)
    FILE                *fid;           /* [in] Output file handle */
    AST_type_n_t        *type_p;        /* [in] AST type node */
    AST_instance_n_t    *inst_p;        /* [in] AST instance node (if any) */
    NAMETABLE_id_t      inst_name;      /* [in] Instance name (if any) */
#endif
{
    AST_field_n_t       *field_p;
    AST_type_n_t        *btype_p;
    AST_type_n_t        *etype_p;
    AST_arm_n_t         *arm_p;
    AST_disc_union_n_t  *union_p;
    AST_constant_n_t    *const_p;
    AST_array_n_t       *array_p;
    ASTP_node_t         *node_p;        /* For error reporting */
    unsigned long       dims;           /* Dimensions in array */
    unsigned long       strsz;          /* Size of fixed string */
    boolean             in_typedef;     /* TRUE => spelling definition */
    boolean             n_e_union;      /* TRUE => non-encapsulated union */
    boolean             param_inst;     /* TRUE => parameter instance */
    boolean             v1_array;       /* TRUE => [v1_array] array */
    boolean             string;         /* TRUE => [string] array or pointer */
    boolean             cfmt_string;    /* TRUE => conformant [string] array */
    char ucid[MAX_ID+1];                /* Upper case identifier */

    param_inst = (inst_p != NULL
                  && inst_p->fe_info->node_kind == fe_parameter_n_k);
    in_typedef = (inst_p == NULL && inst_name == NAMETABLE_NIL_ID);
    if (inst_name == NAMETABLE_NIL_ID && inst_p != NULL)
        inst_name = inst_p->name;
    node_p = (inst_p != NULL) ? (ASTP_node_t *)inst_p : (ASTP_node_t *)type_p;

    /*
     * Only structures, unions, and enums have FORTRAN definitions.
     */
    if (in_typedef
        && type_p->kind != AST_structure_k
        && type_p->kind != AST_disc_union_k
        && type_p->kind != AST_enum_k)
        return FALSE;

    /*
     * If we are defining a type which was defined as another type, chase to
     * the base type because FORTRAN does not have a mechanism for defining
     * a type as a synonym for another type.
     */
    for (btype_p = type_p;
         btype_p->defined_as != NULL;
         btype_p = btype_p->defined_as)
        ;

    /*
     * If the type has the [represent_as] attribute, emit its local rep name
     * for a declaration; fall into the regular logic for a definition to spell
     * the network type.
     */
    if (!in_typedef && type_p->rep_as_type != NULL)
    {
        AST_type_n_t    *local_type_p;
        /*
         * If local type is IDL-defined, fall into the regular logic but to
         * spell the local type rather than the network type.  Otherwise,
         * spell the local type name, assumed to be a RECORD reference.
         */
        local_type_p = (AST_type_n_t *)NAMETABLE_lookup_binding(
                                        type_p->rep_as_type->type_name);
        if (local_type_p != NULL
            && local_type_p->fe_info->node_kind == fe_type_n_k)
        {
            type_p = local_type_p;
        }
        else
        {
            for_fprintf(fid, "      RECORD /%s/",
                FSPELL_id_to_uc_string(type_p->rep_as_type->type_name, ucid));
            goto spell_instance;
        }
    }

    switch (type_p->kind)
    {
    case AST_boolean_k:
        for_fprintf(fid, "      LOGICAL*1");
        break;
    case AST_byte_k:
        for_fprintf(fid, "      BYTE");
        break;
    case AST_character_k:
        for_fprintf(fid, "      CHARACTER");
        break;
    case AST_small_integer_k:
    case AST_small_unsigned_k:
    case AST_short_integer_k:
        for_fprintf(fid, "      INTEGER*2");
        break;
    case AST_short_unsigned_k:
    case AST_long_integer_k:
        for_fprintf(fid, "      INTEGER*4");
        break;
    case AST_long_unsigned_k:
        /* FORTRAN INTEGER*4 used for IDL unsigned long */
        FSPELL_warn_once(&FSPELL_forsignint, node_p, NIDL_FORSIGNINT, 0);
        for_fprintf(fid, "      INTEGER*4");
        break;
    case AST_hyper_integer_k:
        for_fprintf(fid, "      RECORD /IDL_HYPER_INT/");
        break;
    case AST_hyper_unsigned_k:
        for_fprintf(fid, "      RECORD /IDL_UHYPER_INT/");
        break;
    case AST_short_float_k:
        for_fprintf(fid, "      REAL*4");
        break;
    case AST_long_float_k:
        for_fprintf(fid, "      REAL*8");
        break;

    case AST_handle_k:
        for_fprintf(fid, "      RECORD /HANDLE_T/");
        break;

    case AST_enum_k:
        /*
         * If a reference, simply an INTEGER*4.  If a definition, we must
         * spell the enum literals as INTEGER*4 constants.
         */
        if (!in_typedef)
            for_fprintf(fid, "      INTEGER*4");
        else
        {
            for (const_p = btype_p->type_structure.enumeration->enum_constants;
                 const_p != NULL;
                 const_p = const_p->next)
            {
                FSPELL_constant_def(fid, const_p);
            }
        }
        break;

    case AST_structure_k:
        /*
         * If a named reference, simply a RECORD /type-name/ reference.  If a
         * definition, spell STRUCTURE construct and recurse for each field.
         */
        if (type_p->name == NAMETABLE_NIL_ID)
        {
            /*
             * Probably can never happen - frontend makes sure non-nested
             * structs are not anonymous, and routine FSPELL_anon_type_defs
             * generates names where needed.
             */
            char *inst_id;
            NAMETABLE_id_to_string(inst_name, &inst_id);
            CHECKER_error(type_p, NIDL_ANONTYPE, inst_id);
            return FALSE;
        }
        if (!in_typedef)
        {
            for_fprintf(fid, "      RECORD /%s/",
                        FSPELL_id_to_uc_string(type_p->name, ucid));
        }
        else
        {
            if (AST_CONFORMANT_SET(type_p))
            {
                /* Conformant structures not supported in target language */
                CHECKER_warning(type_p, NIDL_LANGCFMTSTRCT);
            }
            for_fprintf(fid, "      STRUCTURE /%s/\n",
                        FSPELL_id_to_uc_string(type_p->name, ucid));
            /*
             * Recurse to emit the individual fields.
             */
            for (field_p = btype_p->type_structure.structure->fields;
                 field_p != NULL;
                 field_p = field_p->next)
            {
                if (field_p->type->name == NAMETABLE_NIL_ID
                    && (field_p->type->kind == AST_structure_k
                        || field_p->type->kind == AST_disc_union_k))
                {
                    FSPELL_nested_anon_type(fid, field_p->type,
                       (AST_instance_n_t *)field_p, field_p->name, 1);
                }
                else
                {
                    for_fprintf(fid, "    ");
                    FSPELL_typed_name(fid, field_p->type,
                                    (AST_instance_n_t *)field_p, field_p->name);
                    for_fprintf(fid, "\n");
                }
            }
            for_fprintf(fid, "      END STRUCTURE\n");
        }
        break;

    case AST_disc_union_k:
        /*
         * If a named reference, simply a RECORD /type-name/ reference.  If a
         * definition or an anonymous reference, spell STRUCTURE construct and
         * recurse for each arm.
         */
        if (type_p->name == NAMETABLE_NIL_ID)
        {
            /*
             * Probably can never happen - frontend makes sure non-nested
             * unions are not anonymous, and routine FSPELL_anon_type_defs
             * generates names where needed.
             */
            char *inst_id;
            NAMETABLE_id_to_string(inst_name, &inst_id);
            CHECKER_error(type_p, NIDL_ANONTYPE, inst_id);
            return FALSE;
        }
        if (!in_typedef)
        {
            for_fprintf(fid, "      RECORD /%s/",
                        FSPELL_id_to_uc_string(type_p->name, ucid));
        }
        else
        {
            for_fprintf(fid, "      STRUCTURE /%s/\n",
                        FSPELL_id_to_uc_string(type_p->name, ucid));

            /*
             * If encapsulated union, output discriminator field.
             */
            union_p = btype_p->type_structure.disc_union;
            n_e_union = (union_p->discrim_name == NAMETABLE_NIL_ID);
            if (!n_e_union)
            {
                for_fprintf(fid, "    ");
                FSPELL_typed_name(fid, union_p->discrim_type, NULL,
                                  union_p->discrim_name);
                for_fprintf(fid, "\n");
            }
            
            /*
             * Recurse to emit the individual arms.
             */
            for_fprintf(fid, "          UNION\n");
            for (arm_p = union_p->arms; arm_p != NULL; arm_p = arm_p->next)
            {
                for_fprintf(fid, "              MAP\n");
                if (arm_p->type != NULL
                    && arm_p->type->name == NAMETABLE_NIL_ID
                    && (arm_p->type->kind == AST_structure_k
                        || arm_p->type->kind == AST_disc_union_k))
                {
                    FSPELL_nested_anon_type(fid, arm_p->type,
                        (AST_instance_n_t *)arm_p, arm_p->name, 3);
                }
                else if (arm_p->type != NULL)
                {
                    for_fprintf(fid, "            ");
                    FSPELL_typed_name(fid, arm_p->type,
                                      (AST_instance_n_t *)arm_p, arm_p->name);
                    for_fprintf(fid, "\n");
                }
                for_fprintf(fid, "              END MAP\n");
            }
            for_fprintf(fid, "          END UNION\n");
            for_fprintf(fid, "      END STRUCTURE\n");
        }
        break;

    case AST_pipe_k:
        /* Pipes not supported for target language */
        FSPELL_warn_once(&FSPELL_langpipesns, node_p, NIDL_LANGPIPESNS, 0);
        for_fprintf(fid, "      Pipe");
        break;

    case AST_void_k:
        return FALSE;

    case AST_array_k:
        /*
         * Arrays with [ptr] or [unique] are really pointers.
         */
        if (param_inst && (AST_PTR_SET(inst_p) || AST_UNIQUE_SET(inst_p)))
        {
            /* FORTRAN INTEGER*n used for IDL pointer type */
            FSPELL_warn_once(&FSPELL_forpointer, node_p, NIDL_FORPOINTER,
                             IDL_PSZ);
            for_fprintf(fid, "      INTEGER*%d", IDL_PSZ);
            break;
        }

        /*
         * Array of array is maps to a multidimensional array.  String array
         * maps to CHARACTER*n.  Chase to the non-array or string base type.
         */
        dims = 0;
        v1_array = (inst_p != NULL && AST_SMALL_SET(inst_p));
        string   = (inst_p != NULL && AST_STRING_SET(inst_p));
        cfmt_string = (AST_CONFORMANT_SET(type_p)
                       && (string || AST_STRING_SET(type_p)));
        etype_p = type_p;

        while (etype_p->kind == AST_array_k && etype_p->rep_as_type == NULL)
        {
            if (AST_SMALL_SET(etype_p))
                v1_array = TRUE;
            if (AST_STRING_SET(etype_p))
                string = TRUE;
            array_p = etype_p->type_structure.array;
            etype_p = array_p->element_type;
            if (string || v1_array)
                break;

            dims += array_p->index_count;
        }
        if (v1_array)
        {
            ASTP_node_t *node_p;
            ASTP_attr_flag_t attr1 = ASTP_SMALL;

            if (inst_p != NULL)
                node_p = (ASTP_node_t *)inst_p;
            else
                node_p = (ASTP_node_t *)type_p;
            CHECKER_warning(node_p, NIDL_LANGATTRNS,
                KEYWORDS_lookup_text(AST_attribute_to_token(&attr1)));
            return FALSE;
        }
        if (dims > DDBE_FOR_MAX_DIM)
        {
            CHECKER_error(type_p, NIDL_LANGMAXDIM, DDBE_FOR_MAX_DIM);
            return FALSE;
        }

        /*
         * If [array of] string, spell CHARACTER*n*octetsize instance name.
         * Recurse to spell the non-array base type and array instance name.
         */
        if (string && etype_p->rep_as_type == NULL)
        {
            if (cfmt_string)
            {
                if (param_inst)
                    for_fprintf(fid, "      CHARACTER*(*)");
                else
                {
                    /*
                     * Conformant structures not supported in target language.
                     * Warning handled elsewhere; emit single element defn.
                     */
                    for_fprintf(fid, "      CHARACTER*%d", etype_p->ndr_size);
                }
            }
            else
            {
                strsz = array_p->index_vec->upper_bound->value.int_val
                        - array_p->index_vec->lower_bound->value.int_val + 1;
                for_fprintf(fid, "      CHARACTER*%ld", strsz*etype_p->ndr_size);
            }
            for_fprintf(fid, " %s", FSPELL_id_to_uc_string(inst_name, ucid));
            if (dims == 0)
                return TRUE;        /* return unless array of string case */
        }
        else
            FSPELL_typed_name(fid, etype_p, NULL, inst_name);

        /*
         * Now spell the bounds information.
         */
        for_fprintf(fid, "(");
        for (etype_p = type_p;
             etype_p->kind == AST_array_k && !AST_STRING_SET(etype_p)
                && etype_p->rep_as_type == NULL;
             etype_p = etype_p->type_structure.array->element_type)
        {
            array_p = etype_p->type_structure.array;
            /*
             * After toplevel array, null instance ptr since its attributes
             * can only apply to the toplevel array.
             */
            if (etype_p == type_p)
                FSPELL_array_bounds(fid, array_p, inst_p);
            else
            {
                for_fprintf(fid, ", ");
                FSPELL_array_bounds(fid, array_p, NULL);
            }
        }
        for_fprintf(fid, ")");
        return TRUE;

    case AST_pointer_k:
        /*
         * Pointers are valid only for these cases:
         *   1) a (void *), which must be a context handle.
         *   2) a [ref] arrayified pointer parameter instance, in which case
         *      we spell the equivalent array declaration.
         *   3) any other [ref] parameter instance, in which case we spell the
         *      pointee type since everything is passed by reference in FORTRAN.
         */
        if (type_p->type_structure.pointer->pointee_type->kind == AST_void_k)
        {
            for_fprintf(fid, "      INTEGER*4");
            break;
        }

        if (inst_p == NULL || !AST_REF_SET(inst_p)
            || inst_p->fe_info->node_kind != fe_parameter_n_k)
        {
            /* FORTRAN INTEGER*n used for IDL pointer type */
            FSPELL_warn_once(&FSPELL_forpointer, node_p, NIDL_FORPOINTER,
                             IDL_PSZ);
            for_fprintf(fid, "      INTEGER*%d", IDL_PSZ);
            break;
        }

        if (BE_Is_Arrayified(inst_p, type_p))
        {
            FSPELL_typed_name(fid,
                type_p->type_structure.pointer->pointee_type->array_rep_type,
                inst_p, inst_name);
        }
        else
        {
            /*
             * If a parameter [ref] pointer to an array, the pointer is
             * superfluous so pass on the instance pointer, otherwise NULL.
             */
            FSPELL_typed_name(fid, type_p->type_structure.pointer->pointee_type,
                              (param_inst && AST_REF_SET(inst_p) && type_p->
                               type_structure.pointer->pointee_type->kind ==
                               AST_array_k) ? inst_p : NULL,
                              inst_name);
        }
        return TRUE;

    case AST_function_k:
        /* FORTRAN INTEGER*n used for IDL pointer type */
        FSPELL_warn_once(&FSPELL_forpointer, node_p, NIDL_FORPOINTER, IDL_PSZ);
        for_fprintf(fid, "      INTEGER*%d", IDL_PSZ);
        break;

    default:
        INTERNAL_ERROR("Unknown type kind in FSPELL_typed_name");
        return FALSE;
    }

    /*
     * Fall through to here for those cases where we need to spell the
     * instance name if we're processing a declaration.
     */
spell_instance:
    if (inst_name != NAMETABLE_NIL_ID)
        for_fprintf(fid, " %s", FSPELL_id_to_uc_string(inst_name, ucid));

    return TRUE;
}


/*
 *  F S P E L L _ t y p e _ d e f
 *
 *  Spell a type definition.  The last two parameters to FSPELL_typed_name
 *  indicate that this is a definition as opposed to a declaration.
 *  Only some IDL types require a FORTRAN definition, e.g. structures.
 */
static void FSPELL_type_def
#ifdef PROTO
(
    FILE                *fid,           /* [in] Output file handle */
    AST_type_n_t        *tp             /* [in] AST type node */
)
#else
(fid, tp)
    FILE                *fid;           /* [in] Output file handle */
    AST_type_n_t        *tp;            /* [in] AST type node */
#endif
{
    boolean code_spelt;             /* TRUE => code was spelt */

    code_spelt = FSPELL_typed_name(fid, tp, NULL, NAMETABLE_NIL_ID);
    if (code_spelt)
        for_fprintf(fid, "\n");
}


/*
 *  F S P E L L _ a n o n _ i f _ n e c c
 *
 *  If the passed type is an array and the base type is anonymous struct or
 *  union, generate name for and spell definition of the base type.
 */
static void FSPELL_anon_if_necc
#ifdef PROTO
(
    FILE                *fid,           /* [in] Output file handle */
    AST_type_n_t        *passed_type_p  /* [in] AST type node */
)
#else
(fid, passed_type_p)
    FILE                *fid;           /* [in] Output file handle */
    AST_type_n_t        *passed_type_p; /* [in] AST type node */
#endif
{
    AST_type_n_t    *type_p;            /* Base type of array */

    if (passed_type_p == NULL || passed_type_p->kind != AST_array_k)
        return;

    type_p = passed_type_p->type_structure.array->element_type;
    if (type_p->name != NAMETABLE_NIL_ID)
        return;
    if (type_p->kind != AST_structure_k && type_p->kind != AST_disc_union_k)
        return;

    type_p->name = AST_generate_name(FSPELL_int_p, "");

    FSPELL_typed_name(fid, type_p, NULL, NAMETABLE_NIL_ID);
    for_fprintf(fid, "\n");
}


/*
 *  F S P E L L _ a n o n _ t y p e _ d e f s
 *
 *  Generate name for and spell definition of each contained anonymous
 *  struct/union type that requires a name in the FORTRAN source.  This
 *  is limited to fields or arms that contain an array of anonymous
 *  struct or union.  Note that nested anonymous struct or union types
 *  within a struct or union do not require a type name.
 */
static void FSPELL_anon_type_defs
#ifdef PROTO
(
    FILE                *fid,           /* [in] Output file handle */
    AST_type_n_t        *type_p         /* [in] AST type node */
)
#else
(fid, type_p)
    FILE                *fid;           /* [in] Output file handle */
    AST_type_n_t        *type_p;        /* [in] AST type node */
#endif
{
    AST_field_n_t       *field_p;
    AST_arm_n_t         *arm_p;

    switch (type_p->kind)
    {
    case AST_structure_k:
        for (field_p = type_p->type_structure.structure->fields;
             field_p != NULL;
             field_p = field_p->next)
            FSPELL_anon_if_necc(fid, field_p->type);
        break;

    case AST_disc_union_k:
        for (arm_p = type_p->type_structure.disc_union->arms;
             arm_p != NULL;
             arm_p = arm_p->next)
            FSPELL_anon_if_necc(fid, arm_p->type);
        break;

    default:
        break;
    }
}


/*
 *  F S P E L L _ o p e r a t i o n _ d e f
 *
 *  Spell a comment SUBROUTINE or FUNCTION "prototype" for an IDL operation.
 */
static void FSPELL_operation_def
#ifdef PROTO
(
    FILE                *fid,           /* [in] Output file handle */
    AST_operation_n_t   *op             /* [in] AST operation node */
)
#else
(fid, op)
    FILE                *fid;           /* [in] Output file handle */
    AST_operation_n_t   *op;            /* [in] AST operation node */
#endif
{
    AST_parameter_n_t   *param;
    NAMETABLE_id_t      fnres_id;       /* Trailing [out] parameter name */
    char                ucid[MAX_ID+1]; /* Upper case identifier */
    boolean     spelt_paren  = FALSE;   /* TRUE => spelt left parenthesis */
    boolean     res_is_param = FALSE;   /* TRUE => spell function result as */
                                        /*  a trailing [out] parameter      */

    if (AST_NO_CODE_SET(op))
        return;
    res_is_param = DDBE_result_is_param(op, opt_lang_fortran);
    if (res_is_param)
        CHECKER_warning(op, NIDL_LANGUNSFNRS);

    /* Subroutine or function header */
    param = op->result;
    if (res_is_param || param->type->kind == AST_void_k)
        for_fprintf(fid, "C      SUBROUTINE");
    else
    {
        /* Spell type name and dummy instance name "FUNCTION" */
        for_fprintf(fid, "C");
        FSPELL_typed_name(fid, param->type, NULL, NAMETABLE_add_id("FUNCTION"));
    }

    /* Subroutine or function name */
    for_fprintf(fid, " %s", FSPELL_id_to_uc_string(op->name, ucid));

    /* Formal parameter names */
    for (param = op->parameters; param != NULL; param = param->next)
    {
        if ( ! AST_HIDDEN_SET(param) )
        {
            if (!spelt_paren)
            {
                for_fprintf(fid, "(");
                spelt_paren = TRUE;
            }
            else
                for_fprintf(fid, ", ");
            for_fprintf(fid, "%s", FSPELL_id_to_uc_string(param->name, ucid));
        }
    }
    if (res_is_param)
    {
        if (!spelt_paren)
        {
            for_fprintf(fid, "(");
            spelt_paren = TRUE;
        }
        else
            for_fprintf(fid, ", ");
        for_fprintf(fid, "IDL_FUNCTION_RESULT");
        fnres_id = NAMETABLE_add_id("IDL_FUNCTION_RESULT");
    }
    if (spelt_paren)
        for_fprintf(fid, ")");
    for_fprintf(fid, "\n");

    /* Formal parameter list */
    for (param = op->parameters; param != NULL; param = param->next)
    {
        if ( ! AST_HIDDEN_SET(param) )
        {
            for_fprintf(fid, "C");
            FSPELL_typed_name(fid, param->type, (AST_instance_n_t *)param,
                              param->name);
            for_fprintf(fid, "\n");
        }
    }
    if (res_is_param)
    {
        for_fprintf(fid, "C");
        FSPELL_typed_name(fid, op->result->type, (AST_instance_n_t *)op->result,
                          fnres_id);
        for_fprintf(fid, "\n");
    }
    for_fprintf(fid, "\n");
}


/*
 *  F S P E L L _ c o n s t a n t _ d e f
 *
 *  Spell a constant definition:
 *      <type-name> <const-name>
 *      PARAMETER (<const-name>=<const-value>)
 */
static void FSPELL_constant_def
#ifdef PROTO
(
    FILE                *fid,           /* [in] Output file handle */
    AST_constant_n_t    *cp             /* [in] AST constant node */
)
#else
(fid, cp)
    FILE                *fid;           /* [in] Output file handle */
    AST_constant_n_t    *cp;            /* [in] AST constant node */
#endif
{
    char ucid[MAX_ID+1];            /* Upper case identifier */
    char fstr[max_string_len+1];    /* FORTRAN constant string */
    char *s;

    for_fprintf(fid, "      ");
    switch (cp->kind)
    {
        case AST_nil_const_k:
        case AST_int_const_k:
            for_fprintf(fid, "INTEGER*4 ");
            break;
        case AST_hyper_int_const_k:
            INTERNAL_ERROR("hyper constants not supported for FORTRAN");
            break;
        case AST_char_const_k:
            for_fprintf(fid, "CHARACTER ");
            break;
        case AST_string_const_k:
            for_fprintf(fid, "CHARACTER*(*) ");
            break;
        case AST_boolean_const_k:
            for_fprintf(fid, "LOGICAL*1 ");
            break;

	default:
            break;
    }
    FSPELL_id_to_uc_string(cp->name, ucid);
    for_fprintf(fid, "%s\n", ucid);

    for_fprintf(fid, "      PARAMETER (%s=", ucid);
    /*
     * If defined as another constant, emit the constant name.
     * Otherwise, emit the actual value.
     */
    if (cp->defined_as != NULL)
    {
        for_fprintf(fid, "%s",
                    FSPELL_id_to_uc_string(cp->defined_as->name, ucid));
    }
    else
    {
        switch (cp->kind)
        {
            case AST_nil_const_k:
                for_fprintf(fid, "0");
                break;
            case AST_int_const_k:
                for_fprintf(fid, "%ld", cp->value.int_val);
                break;
            case AST_hyper_int_const_k:
                for_fprintf(fid, "unsupported");
                INTERNAL_ERROR("hyper constants not supported for FORTRAN");
                break;
            case AST_char_const_k:
                for_fprintf(fid, "'%s'",
                            FSPELL_mapchar(cp, cp->value.char_val, TRUE));
                break;
            case AST_string_const_k:
                STRTAB_str_to_string(cp->value.string_val, &s);
                for_fprintf(fid, "'%s'",
                            FSPELL_mapstring(cp, s, fstr, TRUE));
                break;
            case AST_boolean_const_k:
                for_fprintf(fid, "%s",
                            cp->value.boolean_val ? ".TRUE." : ".FALSE.");
                break;

	    default:
                break;
        }
    }
    for_fprintf(fid, ")\n\n");
}


/*
 *  F S P E L L _ e x p o r t s
 *
 *  Spell constant, type, and operation definitions into FORTRAN header file.
 */
static void FSPELL_exports
#ifdef PROTO
(
    FILE                *fid,           /* [in] Output file handle */
    AST_export_n_t      *export_p       /* [in] AST export node */
)
#else
(fid, export_p)
    FILE                *fid;           /* [in] Output file handle */
    AST_export_n_t      *export_p;      /* [in] AST export node */
#endif
{
    AST_export_n_t      *ep;            /* [in] AST export node */

    /*
     * First pass: generate named type definitions for anonymous
     * struct/union array element types.
     */
    for (ep = export_p; ep; ep = ep->next)
    {
        switch (ep->kind)
        {
            case AST_constant_k:
                break;
            case AST_operation_k:
                /* IDL does not allow parameters to contain anonymous types */
                break;
            case AST_type_k:
                FSPELL_anon_type_defs(fid, ep->thing_p.exported_type);
                break;
            default:
               INTERNAL_ERROR("Unknown export type in FSPELL_exports");
               break;
        }
    }

    /*
     * Second pass: generate the rest.
     */
    for (ep = export_p; ep; ep = ep->next)
    {
        switch (ep->kind)
        {
            case AST_constant_k:
                FSPELL_constant_def(fid, ep->thing_p.exported_constant);
                break;
            case AST_operation_k:
                FSPELL_operation_def(fid, ep->thing_p.exported_operation);
                break;
            case AST_type_k:
                FSPELL_type_def(fid, ep->thing_p.exported_type);
                break;
            default:
               INTERNAL_ERROR("Unknown export type in FSPELL_exports");
               break;
        }
    }
}


/*
 *  B E _ g e n _ f o r t r a n _ h e a d e r
 *
 *  Emit the FORTRAN header file for an interface.
 */
void BE_gen_fortran_header
#ifdef PROTO
(
    FILE                *fid,           /* [in] Output file handle */
    AST_interface_n_t   *ifp            /* [in] Ptr to AST interface node */
)
#else
(fid, ifp)
    FILE                *fid;           /* [in] Output file handle */
    AST_interface_n_t   *ifp;           /* [in] Ptr to AST interface node */
#endif
{
    AST_import_n_t  *impp;              /* Ptr to AST import node */
    AST_include_n_t *incp;              /* Ptr to AST include node */
    char include_var_name[max_string_len];  /* Include file name */
    char *fn_str;                   /* Imported file name */
    char ucid[MAX_ID+1];            /* Upper case identifier */
    char ucstr[max_string_len+1];   /* Upper case string */

    /* Save static copy of interface node pointer */
    FSPELL_int_p = ifp;

    /*
     * Spell comments for INCLUDEs.
     */
    for_fprintf(fid,
        "C\nC     The following statements must appear in application code\n");

    for (incp = ifp->includes; incp != NULL; incp = incp->next)
    {
        /* Note: ACF parser tacks on correct extension */
        for_fprintf(fid, "C     INCLUDE '%s'\n",
                    FSPELL_str_to_uc_string(incp->file_name, ucid));
    }

    impp = ifp->imports;
    /*
     * Spell auto-import include manually to avoid "dce/" prefix.
     */
    if (impp != NULL)
    {
        for_fprintf(fid, "C     INCLUDE 'NBASE.FOR'\n");
        impp = impp->next;
    }
    for ( ; impp != NULL; impp = impp->next)
    {
        STRTAB_str_to_string(impp->file_name, &fn_str);
        FILE_form_filespec("", (char *)NULL, ".FOR", fn_str, include_var_name);
        for_fprintf(fid, "C     INCLUDE '%s'\n",
                    FSPELL_strucpy(ucstr, include_var_name));
    }
    for_fprintf(fid, "\n");

    FSPELL_exports(fid, ifp->exports);

    /*
     * Spell comment if [implicit_handle] used.
     */
    if (ifp->implicit_handle_name != NAMETABLE_NIL_ID)
    {
        for_fprintf(fid, "\n");
        for_fprintf(fid,
                    "C     Application must supply a COMMON block for the\n");
        for_fprintf(fid,
                    "C     [implicit_handle] variable name %s\n",
                    FSPELL_id_to_uc_string(ifp->implicit_handle_name, ucid));
    }
}
