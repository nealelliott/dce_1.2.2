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
 * $Log: keywds.c,v $
 * Revision 1.1.8.2  1996/02/18  18:52:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:34  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:19:32  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:04 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:12:12  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:48:19  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:44:01  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:57:52  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:24:45  bfc]
 * 
 * Revision 1.1.4.2  1993/07/07  20:00:29  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:33:04  ganni]
 * 
 * $EndLog$
 */
/*
**  @OSF_COPYRIGHT@
**
**  Copyright (c) 1989 by
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
**  NAME
**
**      KEYWDS.C
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Keyword processing, maintains the keyword hash table and
**      determines if an identifier is really a keyword.
**
**  VERSION: DCE 1.0
**
*/


/*
 * Uncomment the following definition if the hash table should be
 * dumped to stdout to examine the hash chains easily.
 *
 * This causes a main() procedure to be included in the compilation
 * which will build the hash table and dump it to stdout for inspection.
 * Do NOT include this module in the compiler unless the next line is
 * commented out.
 */

/*   #define DUMP_HASH_TABLE     */

#define UINT8 unsigned char

#define HASH(keyword, value)                                            \
        {                                                               \
        int hshval;                                                     \
        char * p;                                                       \
        hshval = 0;                                                     \
        for (p = (keyword); *p != 0; p++) {                             \
            /*   Start of hash algorithm.   */                          \
            hshval = (hshval ^ ((int)(*p))) << 1;                       \
            if (hshval & 0xffffff00)                                    \
                hshval = (hshval & ~0xffffff00) ^ 1;                    \
            };                                                          \
        value = hshval;                                                 \
        }

#include <stdio.h>
#include <astp.h>
#include <y_tab.h>      /* Tokens defined by IDL.Y */

typedef struct {
    long next;
    long res_word;
    char * keyword;
    long token;
    } keyword_attrib_t;

#define HASH_TABLE_SIZE 256

extern boolean search_attributes_table;

static UINT8  keyword_hash_table [HASH_TABLE_SIZE];

static keyword_attrib_t keywords [] = {
/* The zeroth cell of the array cannot be used.*/
{0, 0, 0, 0},

/*
 * List keywords. Order is unimportant for correctness.
 * Order *can* affect efficiency.
 *
 * The first cell is initialized to zero, always. This
 * will be used to form the hash chain during processing.
 *
 * The second cell is a flag indicating whether the keyword
 * is reserved.
 *
 * The third cell is a pointer to the keyword text.
 *
 * The fourth cell is the token value as defined by IDL.Y.
 */

{0, 1, "boolean", BOOLEAN_KW},
{0, 1, "byte", BYTE_KW},
{0, 1, "case", CASE_KW},
{0, 1, "char", CHAR_KW},
{0, 1, "const", CONST_KW},
{0, 1, "default", DEFAULT_KW},
{0, 1, "double", DOUBLE_KW},
{0, 1, "enum", ENUM_KW},
{0, 1, "FALSE", FALSE_KW},
{0, 1, "float", FLOAT_KW},
{0, 1, "handle_t", HANDLE_T_KW},
{0, 1, "hyper", HYPER_KW},
{0, 1, "import", IMPORT_KW},
{0, 1, "int", INT_KW},
{0, 1, "interface", INTERFACE_KW},
{0, 1, "static", STATIC_KW},

/*
 * The "ref" keyword is out of order because it has the same hash
 * value as "long". Placing it before "long" causes it to appear
 * in the hash table *after* "long". Presumably, "long" appears
 * more frequently than "ref"; this will therefore be more efficient.
 */
{0, 0, "ref", REF_KW},

{0, 1, "long", LONG_KW},
{0, 1, "NULL", NULL_KW},
{0, 1, "pipe", PIPE_KW},
{0, 1, "short", SHORT_KW},
{0, 1, "small", SMALL_KW},
{0, 1, "struct", STRUCT_KW},
{0, 1, "switch", SWITCH_KW},
{0, 1, "TRUE", TRUE_KW},
{0, 1, "typedef", TYPEDEF_KW},
{0, 1, "union", UNION_KW},
{0, 1, "unsigned", UNSIGNED_KW},
{0, 1, "void", VOID_KW},

{0, 0, "align", ALIGN_KW},
{0, 0, "broadcast", BROADCAST_KW},
{0, 0, "context_handle", CONTEXT_HANDLE_KW},
{0, 0, "endpoint", ENDPOINT_KW},
{0, 0, "exceptions", EXCEPTIONS_KW},
{0, 0, "first_is", FIRST_IS_KW},
{0, 0, "handle", HANDLE_KW},
{0, 0, "idempotent", IDEMPOTENT_KW},
{0, 0, "ignore", IGNORE_KW},
{0, 0, "in", IN_KW},
{0, 0, "last_is", LAST_IS_KW},
{0, 0, "length_is", LENGTH_IS_KW},
{0, 0, "local", LOCAL_KW},
{0, 0, "max_is", MAX_IS_KW},
{0, 0, "maybe", MAYBE_KW},
{0, 0, "min_is", MIN_IS_KW},
{0, 0, "out", OUT_KW},
{0, 0, "pointer_default", POINTER_DEFAULT_KW},
{0, 0, "ptr", PTR_KW},
{0, 0, "reflect_deletions", REFLECT_DELETIONS_KW},
{0, 0, "shape", SHAPE_KW},
{0, 0, "size_is", SIZE_IS_KW},
{0, 0, "string", STRING_KW},
{0, 0, "switch_is", SWITCH_IS_KW},
{0, 0, "switch_type", SWITCH_TYPE_KW},
{0, 0, "transmit_as", TRANSMIT_AS_KW},
{0, 0, "unique", UNIQUE_KW},
{0, 0, "uuid", UUID_KW},
{0, 0, "version", VERSION_KW},
{0, 0, "v1_array", V1_ARRAY_KW},
{0, 0, "v1_enum", V1_ENUM_KW},
{0, 0, "v1_string", V1_STRING_KW},
{0, 0, "v1_struct", V1_STRUCT_KW},
{0, 0, "cxx_lookup", CXX_LOOKUP_KW},
{0, 0, "client_memory", CLIENT_MEMORY_KW},
{0, 0, 0, 0}
};


void KEYWORDS_init
#ifdef PROTO
(
    void
)
#else
()
#endif
{
int hash_value;
int i;

    /*
     * Initialize the hash table to zeroes.
     */
    for (i = 0; i<HASH_TABLE_SIZE; i++)
        keyword_hash_table [i] = 0;

    /*
     * Build the hash table, and link together the hash hits.
     */
    for (i = 1; keywords [i].keyword != 0; i++) {
        HASH( keywords [i].keyword, hash_value);
        keywords [i].next = keyword_hash_table [hash_value];
        keyword_hash_table [hash_value] = i;
        };
}

int KEYWORDS_screen
#ifdef PROTO
(
    char * identifier,
    NAMETABLE_id_t * id
)
#else
(identifier, id)
    char * identifier;
    NAMETABLE_id_t * id;
#endif
{
    long hash_value;
    long i;

    /*
     * Hash the identifier.
     */
    HASH(identifier, hash_value);

    /*
     * If any keyword hashes to the same value, the value of the
     * hash table will be the keyword table index of the first possible
     * match. Each keyword table entry points to the next possible match.
     */
    if ( keyword_hash_table [hash_value] != 0 ) {
        for (i=keyword_hash_table [hash_value]; i; i=keywords [i].next) {

            /*
             * Test each keyword with the same hash value for a possible
             * match. We have a match if:
             *    1) The strings match, AND either:
             *    2a) The keyword is reserved, OR
             *    2b) We are within attribute brackets, and therefore
             *        must look at the (non-reserved) attribute keywords.
             */
            if ((!strcmp (identifier, keywords [i].keyword)) &&
                (keywords [i].res_word || search_attributes_table))
                    return keywords [i].token;  /* We matched! */
        };
    };

    /*
     * If we get here, we truely have an identifier. Add it to the nametable
     * and return the nametable_id.
     */

     * id = NAMETABLE_add_id(identifier);
     return IDENTIFIER ;

}


/*
 * K E Y W O R D S _ l o o k u p _ t e x t
 * =========================================
 *
 * This function accepts a token number and returns the ASCIZ
 * string representing the keyword.  The string returned is
 * in static storage and need not be freed.
 *
 * Inputs:
 *      token number
 *
 * Returns:
 *      ASCIZ string of token text
 */
char *KEYWORDS_lookup_text
#ifdef PROTO
(
    long    token
)
#else
(token)
    long token;
#endif
{
    long i; /* index into keyword table */

    /*
     * Loop through the table looking for a matching token number.
     */
    for (i = 1; keywords [i].keyword != 0; i++)
        if (keywords[i].token == token) return keywords[i].keyword;

    /*
     * Not, just return question marks
     */
    return "?";
}


#ifdef DUMP_HASH_TABLE
main
#ifdef PROTO
(
    void
)
#else
()
#endif
{
int i, j;

    /*
     * Build the hash table.
     */
    KEYWORDS_init ();

    /*
     * This code dumps the hash table to stdout.
     */
    for (i = 0; i<HASH_TABLE_SIZE; i++) {
        if (keyword_hash_table [i]){
            printf ("Hash value %d: ",i);
            j = keyword_hash_table [i];
            for (j; j; j = keywords [j].next)
                printf ("%s ", keywords [j].keyword);
            printf ("\n");
            };
        };
}
#endif /* DUMP_HASH_TABLE */

