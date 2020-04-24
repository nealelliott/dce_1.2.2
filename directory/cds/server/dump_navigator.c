/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dump_navigator.c,v $
 * Revision 1.1.10.3  1996/02/18  19:36:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:46  marty]
 *
 * Revision 1.1.10.2  1995/12/08  15:30:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:19  root]
 * 
 * Revision 1.1.6.1  1994/10/25  16:17:44  proulx
 * 	     fix for moved clearinghouses
 * 	[1994/10/25  15:06:36  proulx]
 * 
 * Revision 1.1.4.3  1994/08/03  19:04:15  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:25:27  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:43:28  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:01  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:11:39  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:43:08  peckham]
 * 
 * Revision 1.1.2.3  1992/12/30  14:21:55  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:22:18  zeliff]
 * 
 * Revision 1.1.2.2  1992/12/07  15:05:28  keegan
 * 	Fixed message log parsing - it forgot to advance over the length
 * 	datum.
 * 	[1992/12/07  15:04:29  keegan]
 * 
 * Revision 1.1  1992/01/19  15:25:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: dump_navigator.c
 * Version: X02.23
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 *  FACILITY:
 *
 *      DECdns Dump Facility
 *
 *  ABSTRACT:
 *
 *      This module provides data structure navigation facilities to the
 *	dump programs.
 */


/*
 *
 *  INCLUDE FILES
 *
 */

#include <server.h>
#include <stdio.h>
#include <dump.h>
#include <dbdef.h>
#include <tlog.h>


/*
 *
 *                         Global Data Structures
 *
 */


/*
 * Dispatch table and associated macros
 *
 * The dispatch table holds pointers to all supported navigation routines
 * indexed by the structure id (for a list of structure id's, see the
 * "dump.h" file).  Also indexed by structure id is the call_type, which
 * tells the navigate how to call the routine (there are a couple of
 * variations).  If call_type is not one of the supported call types
 * (e.g., 0), then the corresponding structure is not supported by the
 * navigator.
 *
 * The table is initialized by the initialize_navigation routine, which uses
 * the REGISTER_STRUCTURE macro.  REGISTER_STRUCTURE is called with a structure
 * id that is to be supported, the type of call the parsing routine accepts,
 * and the address of the routine to call.
 */

static
struct dispatch_struct {
    int  initialized;                      /* TRUE = loaded and ready to go */
    char call_type[MAX_TYPES+1];           /* Call sequence to use */
    char *(*routine[MAX_TYPES+1])(char *); /* Routine to call (parameters may need changing in future)*/
} dispatch_table = { 0 };

#define REGISTER_STRUCTURE(structure_id, call_type_value, routine_name) \
    dispatch_table.call_type[structure_id] = call_type_value;           \
    dispatch_table.routine[structure_id] = routine_name

/*
 * Call types
 *    CT_1 - argument list is: cursor
 *    CT_2 - argument list is: cursor, structure_length
 */

#define CT_1 1
#define CT_2 2



/*
 * Context buffer structure declaration and support macros
 *
 * The context buffer maintains information about the current datum being
 * interpreted.
 *
 * Two support macros are provided which allocate and deallocate the string
 * storage for the interpreted and hexadecimal values of the datum.
 *
 *    PREPARE_CONTEXT is called with the interpreted and hex sizes.
 *
 *    When the routine is done, FREE_CONTEXT should be called.
 */

typedef struct context_buffer {
    int   s_type;         /* structure type id for the current datum */
    int   cardinality;    /* cardinality of the current datum */
    char *address;        /* address of the current datum */
    char *i_value;        /* interpreted value of the current datum */
    char *h_value;        /* hexadecimal value of the current datum */
    int   datum_size;     /* size (in bytes) of the current datum */
    char *base_address;   /* For relative addressing */
} context_buffer_t;

#define PREPARE_CONTEXT(i_space, h_space)        \
    context.i_value = (char *) malloc(i_space);  \
    context.h_value = (char *) malloc(h_space)

#define FREE_CONTEXT       \
    if (context.h_value) free(context.h_value); \
    if (context.i_value) free(context.i_value)



/*
 *                 Miscellaneous declarations
 */

/* Forward declaration */
char *
navigate(
    char      *,
    int       ,
    int        );  



/*
 * ROUTINE - emit
 *
 * The emit routine is used to format information for the human being who
 * is reading the dump.  Theoretically, the emit routine is capable of
 * formatting for a number of different styles although this capability
 * is not being used.
 *
 * All parameters supplied are input, and the result of this routine is
 * to write one or more lines to the output device.
 *
 * The current style supported has a general format of:
 *
 *    Columns 1-8   Address of the data structure
 *            10-80 Variable information
 *    At least 2 lines are emitted.  First the address and interpreted value
 *    of the datum, along with its title.  The length of the interpreted value
 *    may exceed the margin, so additional lines will be emitted.  After the
 *    intepreted value, the hexadecimal equivalent is printed, subject to the
 *    same margin and overflow conditions.
 */

/* The output margin refers to the variable data margin */
#define OUTPUT_MARGIN 63

static void 
emit (int               style,
      char              *base_address,
      char              *cursor,
      char              *title,
      context_buffer_t  *ctx)
{
    char  *i_value = ctx->i_value; /* interpreted value */
    char  *h_value = ctx->h_value; /* raw (hex) value */
    int output_max;         /* work variable for the margin */

    /*
     * Calculate our output margin based on adding in the title for the
     * first line and print it.
     */
    output_max = OUTPUT_MARGIN - (strlen(title) + 2);
    printf("%8p  %s: %0.*s\n", cursor, title, output_max, i_value);

    /*
     * All lines after this are indented 10 spaces.
     *
     * Walk through the interpreted value using the OUTPUT_MARGIN as
     * the increment.  We use printf's capability of setting a maximum
     * size (precision) for string variables.
     *
     * If the length of the string to be printed exceeds the margin size
     * we print a margin's worth of characters and then slide the pointer
     * over a margin's worth... and start over again.
     */

    while (strlen(i_value) > output_max) 
    {
        i_value = i_value + output_max;
        output_max = OUTPUT_MARGIN;
	printf("          %0.*s\n", output_max, i_value);
    }

    /*
     * We follow the same logic here for the hexadecimal interpretation
     * only there's no title line to deal with.
     */

    output_max = OUTPUT_MARGIN;
    printf("          %0.*s\n", output_max, h_value);
    while (strlen(h_value) > output_max) {
        h_value = h_value + output_max;
        output_max = OUTPUT_MARGIN;
	printf("          %0.*s\n", output_max, h_value);
    }
}


/* -------------------------------------------------------------------------- */


/*
 * ROUTINE set_context
 *
 * This routine is used to set up the context for interpreting a
 * datum.  The calls to this routine are always hidden in the macros
 * which are used to walk through a data structure (e.g., DATUM_ADVANCE	).
 *
 */

static void 
set_context (int                    s_type,
             int                    cardinality,
             char                   *cursor,
             struct context_buffer  *context)
{
    /* Record the structure type and cardinality */
    context->s_type = s_type;
    context->cardinality = cardinality;

    /*
     * Call the type interpreter
     *
     * Note that the type interpreter is given all the information needed
     * to interpret the datum.  In return for this information, the interpeter
     * fills in the datum's size, interpreted value, and hex value.
     *
     * Since the interpreted and hex values are strings, these must be
     * preallocated before the call.   This is done by the PREPARE_CONTEXT
     * macro.
     */
    interpret(s_type,
              cardinality,
              cursor,
              &(context->datum_size),
              context->i_value,
              context->h_value
             );
}




/* -------------------------------------------------------------------------- */


/*
 * ROUTINE - advance
 *
 * The advance routine pushes the cursor forward to the next data structure
 * based on the size of the current data structure.  We obtained that
 * bit of information when we called the type interpreter (see the
 * set_context routine above).
 *
 * This routine is always called by one of the macros used to walk through
 * data structures (e.g., DATUM_ADVANCE).
 */

static char 
*advance (char                   *cursor,
          struct context_buffer  *context)
{
    return (cursor + context->datum_size);
}



/* -------------------------------------------------------------------------- */

/*
 * MACRO - DATUM_ADVANCE
 * MACRO - VECTOR_ADVANCE
 * MACRO - DATUM_HOLD
 * MACRO - ADVANCE
 *
 * These macros are used by the navigation (data structure parsing) routines
 * to walk and interpret a data structure.
 *
 * DATUM_ADVANCE will interpret the datum at the cursor based on the
 * the specified structure type and advance the cursor to the next
 * datum. The title argument is the label (or title) you wish to
 * assign to the interpreted value of this datum when it is emitted.
 *
 * VECTOR_ADVANCE will interpret the vector (array) of data at the
 * cursor  based on the specified structure type and advance the
 * cursor to the  datum following the vector. The title argument is
 * the label (or title) you wish to assign to the interpreted value
 * of this datum when it is emitted.
 *
 * DATUM_HOLD interprets the datum at the cursor based on the
 * specified structure type, but does not advance the cursor.  This
 * allows additional  processing of the current datum.  A call to
 * the ADVANCE macro should  follow. The title argument is the label
 * (or title) you wish to assign to the interpreted value of this
 * datum when it is emitted.
 *
 * ADVANCE advances the cursor based on the size of the current datum.
 * Use the ADVANCE macro if the DATUM_HOLD (or other _HOLD) macros are
 * called.
 */

#define DATUM_ADVANCE(s_type, title)         \
  set_context(s_type, 1, cursor, &context);  \
  emit(1, 0, cursor, title, &context);       \
  cursor = advance(cursor, &context)

#define VECTOR_ADVANCE(cardinality, s_type, title)     \
  set_context(s_type, cardinality, cursor, &context);  \
  emit(1, 0, cursor, title, &context);                 \
  cursor = advance(cursor, &context)

#define DATUM_HOLD(s_type, title)              \
  set_context(s_type, 1, cursor, &context);    \
  emit(1, 0, cursor, title, &context)

#define ADVANCE cursor = advance(cursor, &context)



/* -------------------------------------------------------------------------- */

/*
 * ROUTINE walk_off_db_set
 *
 * The walk_off_db_set routine walks the offset data structure used in many
 * places within the DNS database, in particular it is how sets are
 * organized.  The routine expects that the same data structure can be
 * found at each offset.
 *
 * You call the walk_off_db_set routine with the cursor set to the beginning
 * of the offset structure, which is longword just before the vector of
 * offsets themselves.  This longword contains the length of the offset
 * vector.
 *
 * As walk_off_db_set resolves the address of each offset, it calls the
 * structure navigator with the cursor set to the resolved address along
 * with whatever structure type was expected at each of the offsets.
 *
 * See the design documentation for better information on this.  The
 * following diagram is a rough attempt at graphically depicting what
 * we're dealing with here.  LLLL is the length longword, the numbers are
 * the offsets in the offset vector, the letters are the corresponding data
 * for the offsets (e.g., 1111 is the offset to AAA...).
 *
 * LLLL1111222233334444AAAAAAAAAAAABBBBBBBBCCCCCCCCCCC
 *     ^offsets        ^offset_base
 */
static char 
*walk_off_db_set (char  *cursor,
                  int   base_type)
{
    int   number_of_offsets;    /* Number of offsets */
    int   number_of_deletes;    /* Number of delete holders */
    char *offset_base;          /* Base for the offsets */
    char *offset_cursor;        /* Resolved address of a set member */
    long *offsets;              /* Pointer to the current offset */
    long *next_offset;          /* Pointer to the next offset */

    struct context_buffer context;  /* Context buffer */
    int   i;                    /* iteration variable */
    int   structure_length;     /* Calculated length of a set member */

    int distance_to_base = 24;

    /*
     * Here we go!
     */

    /* Offsets are big - allocate lots of space for interpretation */
    PREPARE_CONTEXT(200, 400);

    /*
     * Calculate our base for the offsets to be added against.
     */
    offset_base = cursor - distance_to_base;

    /*
     * First datum is the length of the offset vector.  Print it and
     * store its value.  Calculate how many offsets we have based on the
     * offset length.
     */

    DATUM_HOLD(WORD, "Offset count");
    number_of_offsets = EXT16(cursor);
    ADVANCE;

    DATUM_HOLD(WORD, "Delete holder count");
    number_of_deletes = EXT16(cursor);
    ADVANCE;

    FREE_CONTEXT;
    PREPARE_CONTEXT(200, (1000 + (9 * number_of_offsets)));

    /*
     * We'll be working with the offsets later, but we have to interpret
     * and print them too.  First we squirrel away the address of the
     * offsets (which is at the cursor), then we interpret the vector.
     */
    offsets = (long *)cursor;    /* typecast into a longword pointer */

    VECTOR_ADVANCE(number_of_offsets, LONGWORD, "Offsets");


    /*
     * Display delete holders (if any)
     */

    for (i = 0;  i < number_of_deletes;  i++) {
        DATUM_ADVANCE(CTS, "Entry CTS  ");
        DATUM_ADVANCE(CTS, "Deletion TS");
    }

    /*
     * Walk through the vector of offsets.
     *
     *   Calculate the address of the set member by adding the current
     *   offset value to the offset base.
     *
     *   Calculate the length of the set member by calculating the difference
     *   between the current offset and the next offset.
     *
     *   Call the structure navigator with the set member's address and
     *   length.
     *
     * Note that the last offset in the vector is not used - see below.
     *
     */
    for (i=0; i<number_of_offsets; ++i) {
        offset_cursor = offset_base + EXT32(offsets);
        next_offset = offsets + 1;
        if ((i+1) < number_of_offsets)		 /* OffsetNext - OffsetThis  */
	    structure_length = EXT32(next_offset) - EXT32(offsets);
	else					 /* LenOfSet - OffsetThisLast */
	    structure_length = EXT32(offset_base) - EXT32(offsets);
        cursor = navigate(offset_cursor, base_type, structure_length);
        ++offsets;
    }

    /*
     * We're done, free up string space.
     *
     * The last offset in the vector doesn't point to a set member - it points
     * to the next datum after the last member of the set.  We return this
     * value to calculate the address of the structure beyond the set.
     */

    FREE_CONTEXT;
    return (offset_base + *((unsigned long *)offset_base));
}


/* -------------------------------------------------------------------------- */


/*
 * ROUTINE - walk_off_bucket_set
 *
 * The walk_off_bucket_set routine interprets another, rather rare,
 * form of offset vector.  This offset structure is used by the
 * database code to keep track of the contents of data buckets.
 *
 * The general idea is the same as the walk_off_database_set routine,
 * so see that routine for background information.  The major
 * difference with this version is that the offset vector contains
 * both offsets and lengths and the base for the offsets is
 * different.
 *
 * The offset base is, unfortunately, the beginning of a data bucket.
 * This is assumed to be DISTANCE_TO_BASE (currently 20) bytes before
 * the entry cursor.  I know this is ugly, but anything else would
 * make the call sequences to the navigation routines a nightmare.
 * Since this type of offset structure is not in general use within the
 * server, it seemed like an O.K. tradeoff.
 *
 */
static char 
*walk_off_bucket_set (char  *cursor,
                      int   base_type)
{
    int   number_of_offsets;          /* Stored number of offsets */
    char *offset_base;                /* Base for the offsets */
    char *offset_cursor;              /* Pointer to the current offset's data */
    struct rec_descriptor *offsets;   /* Current offset tuple (descriptor) */

    struct context_buffer context;    /* Context buffer */
    int   i;                          /* iterator variable */

    int distance_to_base = 20;


    PREPARE_CONTEXT(10000, 20000);

    /*
     * Calculate our base for the offsets to be added against.
     */
    offset_base = cursor - distance_to_base;

    DATUM_HOLD(LONGWORD, "Descriptor count");
    number_of_offsets = *((unsigned long *)cursor);
    ADVANCE;

    offsets = (struct rec_descriptor *)cursor;

    /*
     * The number of offsets is the number of offset descriptors, which
     * are offset/length pairs.  SInce this is rather boring information
     * we'll just display it as a longword vector.
     */
    VECTOR_ADVANCE(number_of_offsets*2, LONGWORD, "Offset/length pairs");

    /*
     * Iterate through the offsets.
     */
    for (i=0; i<number_of_offsets; ++i) {
        offset_cursor = offset_base + offsets->rec_offset;
        cursor = navigate(offset_cursor,
                          base_type,
                          offsets->rec_length
                         );
        ++offsets;
    }

    FREE_CONTEXT;
    return (cursor);
}


/* -------------------------------------------------------------------------- */


/*
 * ROUTINE - walk_off_wire_set
 *
 * The walk_off_wire_set routine interprets the sets ase they appear
 * in the DNS architecture (which also specifies how they look on
 * the wire).
 *
 * The general idea is the same as the walk_off_database_set
 * routine, so see that routine for background information.  The
 * basic logic of how we navigate the set is the same, although
 * there are a couple of twists
 *
 * The offset base is, unfortunately, the beginning of a data
 * bucket. This is assumed to be DISTANCE_TO_BASE (currently 1)
 * bytes before the entry cursor.  As this is an architected format,
 * it is unlikely to change.
 *
 */
static char 
*walk_off_wire_set (char  *cursor,
                    int   base_type)
{
    int    offset_length;        /* Length (in bytes) of all the offsets */
    int    number_of_offsets;    /* Calculated number of offsets */
    char  *offset_base;          /* Base for the offsets */
    char  *offset_cursor;        /* Resolved address of a set member */
    short *offsets;              /* Pointer to the current offset */
    short *next_offset;          /* Pointer to the next offset */

    struct context_buffer context;    /* Context buffer */
    int   i;                          /* iterator variable */

    int distance_to_base;
    int structure_length;


    distance_to_base = 1;

    PREPARE_CONTEXT(10000, 20000);

    /*
     * Calculate our base for the offsets to be added against.
     */
    offset_base = cursor - distance_to_base;

    /*
     * First datum is the length of the offset vector.  Print it and
     * store its value.  Calculate how many offsets we have based on the
     * offset length.
     */

    DATUM_HOLD(WORD, "Offset length");
    offset_length = (int)EXT16(cursor);
    number_of_offsets = offset_length / sizeof(short);
    ADVANCE;

    /*
     * Next is the length of the values.
     */

    DATUM_ADVANCE(WORD, "Value length");

    /*
     * We'll be working with the offsets later, but we have to interpret
     * and print them too.  First we squirrel away the address of the
     * offsets (which is at the cursor), then we interpret the vector.
     */
    offsets = (short*)cursor;    /* typecast into a longword pointer */

    VECTOR_ADVANCE(number_of_offsets, WORD, "Offsets");

    /*
     * Walk through the vector of offsets.
     *
     *   Calculate the address of the set member by adding the current
     *   offset value to the offset base.
     *
     *   Calculate the length of the set member by calculating the difference
     *   between the current offset and the next offset.
     *
     *   Call the structure navigator with the set member's address and
     *   length.
     *
     * Note that the last offset in the vector is not used - see below.
     *
     */
    for (i=1; i<number_of_offsets; ++i) {
        offset_cursor = offset_base + EXT32(offsets);
        next_offset = offsets + 1;
        structure_length = EXT32(next_offset) - EXT32(offsets);
        cursor = navigate(offset_cursor, base_type, structure_length);
        ++offsets;
    }

    FREE_CONTEXT;
    return (cursor);
}


/* -------------------------------------------------------------------------- */

/*
 * MACRO   - WALK_OFFSET
 * ROUTINE - walk_offset
 *
 * The WALK_OFFSET macro is a high-level parsing routine used to
 * traverse offset arrays (which are typically used in set representation).
 * The macro only provides a single means of walking offset structures,
 * and exists solely to make the parsing routines a bit more readable
 * at a minor expense in performance (which is not an important goal
 * for this program.)
 */

#define WALK_OFFSET(offset_type, base_type) \
    cursor = walk_offset(offset_type, cursor, base_type)

/* Offset types */

#define DATABASE_SET  1
#define BUCKET_SET    2
#define WIRE_SET      3


static char 
*walk_offset (int   offset_type,
              char  *cursor,
              int   base_type)
{
    switch (offset_type) {
        case DATABASE_SET :
            return (walk_off_db_set(cursor, base_type));
            break;
        case BUCKET_SET :
            return (walk_off_bucket_set(cursor, base_type));
            break;
        case WIRE_SET :
            return (walk_off_wire_set(cursor, base_type));
            break;
        default :
	    /*
	     * Unknown offset_type
	     */
	    dce_svc_printf(CDS_S_DUMP_OFFSET_A_MSG, offset_type);
           return (NULL);
    }
}



/* -------------------------------------------------------------------------- */

/*
 *                ===============================
 *                Data Structure Parsing Routines
 *                ===============================
 *
 *
 * All of the parse_* routines are specified below.  Each of them has
 * the knowledge about how to interpret a particular data structure which
 * is composed of datums (atomic values) or other, nested, data structures.
 *
 * Each of the routines is passed the address of the particular data structure
 * that is to be parsed.  Each routine then walks through the structure
 * calling upon the type interpreters or other navigators to dump the
 * structure.
 *
 * As described in the design document, the navigator is essentially a
 * virtual machine that's tuned for walking around data structures.
 * Keeping that analogy, there is a core instruction set for parsing
 * structures, and a general similarity between each of the parsing
 * routines:
 *
 *     parse_<whatever>(cursor)
 *         declare context buffer (and other variables if necessary)
 *         PREPARE_CONTEXT
 *
 *         one or more of the datum macros or recursive calls to the
 *         structure navigator
 *
 *         FREE_CONTEXT
 *         return the address of the structure after this one
 *
 * If there is a need for additional processing beyond what the macros
 * provide, then regular C code is interspersed.
 *
 * The name of the routine should be parse_ followed by the lowercase
 * version of the structure id.
 */


static char 
*parse_tlog (char *cursor)
{
    struct context_buffer context;
    int transaction_type;
    int opcode;
    int alignment_mask;
    tlog_hdr_t *tlog_p;

    tlog_p = (tlog_hdr_t *)cursor;
    PREPARE_CONTEXT(1000, 2000);

    DATUM_ADVANCE(LONGWORD, "Checksum");
    DATUM_ADVANCE(LONGWORD, "Length");
    DATUM_ADVANCE(LONGWORD, "Sequence number");
    DATUM_HOLD(TLOG_ENTRY_TYPE, "Type");
    transaction_type = (int)EXT8(cursor);
    ADVANCE;
    DATUM_HOLD(OPERATION, "OpCode");
    opcode = (int)EXT8(cursor);
    ADVANCE;
    DATUM_ADVANCE(LONGWORD, "Record");
    DATUM_ADVANCE(LONGWORD, "Block count");
    DATUM_ADVANCE(UID, "Directory UID");
    DATUM_HOLD(WORD, "Alignment mask");
    alignment_mask = *((unsigned short *)cursor);
    ADVANCE;
    cursor = (char *)&tlog_p[1];


    if (transaction_type == TLOG_CREATE) {
        switch (opcode) {
            case OP_CreateDirectory :
                DATUM_ADVANCE(FULL_NAME, "Dir name");
		cursor = (char *)((((unsigned long)cursor)
				+ alignment_mask) & ~alignment_mask);
                cursor = navigate(cursor, DIRECTORY_ENTRY, 0);
                break;
            case OP_CreateObject :
            case OP_CreateLink :
            case OP_CreateChildEntry :
            case OP_DoUpdate :
                DATUM_ADVANCE(FULL_NAME, "Object name");
		cursor = (char *)((((unsigned long)cursor)
				+ alignment_mask) & ~alignment_mask);
                cursor = navigate(cursor, DIRECTORY_ENTRY, 0);
                break;
            default :
		/*
		 * Unsupported navigation of operation
		 */
		dce_fprintf(stdout, cds_s_dump_unknowntransop);
	    }
    } else if (transaction_type == TLOG_UPDATE) {
        cursor = navigate(cursor, UPDATE_PACKET, 0);
    } else {
	/*
	 * Unknown transaction type - ignoring for now...
	 */	
      dce_fprintf(stdout, cds_s_dump_unknowntranstype);
    }

    FREE_CONTEXT;
    return (cursor);
}


/* -------------------------------------------------------------------------- */


static char 
*parse_update_packet (char *cursor)
{
    int x;
    int transaction_count;
    struct context_buffer context;

    PREPARE_CONTEXT(100,200);

    DATUM_ADVANCE(FULL_NAME, "Updating");
    DATUM_ADVANCE(CTS, "Entry CTS");
    DATUM_ADVANCE(ENTITY_TYPE_WIRE, "Type");
    DATUM_HOLD(BYTE, "Count");
    transaction_count = (int)EXT8(cursor);
    ADVANCE;
    for (x = 0; x < transaction_count; x++) {
        cursor = navigate(cursor, UPDATE_STRUCTURE, 0);
    }

    FREE_CONTEXT;
    return(cursor);
}


static char 
*parse_update_structure (char *cursor)
{
    int value_length;
    char value_flag;
    char attr_type;
    struct context_buffer context;

    PREPARE_CONTEXT(20000, 40000);

    DATUM_ADVANCE(BYTE, "Operation");
    DATUM_ADVANCE(CTS, "Timestamp");
    DATUM_HOLD(ATTRIBUTE_TYPE, "Attribute type");
    /* We have to check to see if the AttributeSpecifier is present or null */
    attr_type = EXT8(cursor);
    ADVANCE;
    if (attr_type != 1) {  /* attribute type isn't null */
        DATUM_ADVANCE(SIMPLE_NAME, "Attribute name");
        DATUM_HOLD(BYTE, "Value flag");
        value_flag = EXT8(cursor);
        ADVANCE;
        if (value_flag) {
            DATUM_HOLD(WORD, "Value length");
            value_length = (int) EXT16(cursor);
            ADVANCE;
	    DATUM_ADVANCE(VALUETYPE, "ValueType");
            VECTOR_ADVANCE(value_length, CHARACTER, "Value");
        }
    }

    FREE_CONTEXT;
    return(cursor);
}


/* -------------------------------------------------------------------------- */

static char 
*parse_checkpoint_structure (char *cursor)
{
    struct context_buffer context;
    int entry_type;

    PREPARE_CONTEXT(100, 200);

    DATUM_HOLD(LONGWORD, "Entry type");
    entry_type = (int)EXT32(cursor);
    ADVANCE;
    DATUM_ADVANCE(LONGWORD, "Bucket length (in blocks)");

    if (entry_type == DIRECTORY_BUCKET) {
        cursor = navigate(cursor, DIRECTORY_ENTRY, 0);
    } else if (entry_type == PSEUDO_BUCKET) {
        cursor = navigate(cursor, DIRECTORY_ENTRY, 0);
    } else if (entry_type == DATA_BUCKET) {

        cursor = navigate(cursor, DATA_BUCKET_STRUCTURE, 0);
    } else {
	/*
	 * I don't know how to parse this entry type
	 */	
	dce_fprintf(stdout, cds_s_dump_unknownbucket);
    }

    FREE_CONTEXT;
    return (cursor);
}


/* -------------------------------------------------------------------------- */

static char 
*parse_checkpoint_header (char *cursor)
{
    struct context_buffer context;

    PREPARE_CONTEXT(100, 200);

    DATUM_ADVANCE(LONGWORD, "Version");
    DATUM_ADVANCE(LONGWORD, "Sequence number");
    DATUM_ADVANCE(LONGWORD, "Directory count");
    DATUM_ADVANCE(CTS, "Last CTS");
    DATUM_ADVANCE(ABS_DATE, "Next background schedule time");
    DATUM_ADVANCE(BYTE, "Major version");
    DATUM_ADVANCE(BYTE, "Minor version");
    DATUM_ADVANCE(UID, "NSUID");
    DATUM_ADVANCE(WORD,"Alignment mask");
    DATUM_ADVANCE(LONGWORD,"TLog size threshold (megabytes)");
    DATUM_ADVANCE(LONGWORD,"Checkpoint interval (hours)");

    FREE_CONTEXT;
    return (cursor);
}



/* -------------------------------------------------------------------------- */

static char 
*parse_dir_entry (char *cursor)
{
    struct context_buffer context;

    PREPARE_CONTEXT(1000, 2000);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        "parse_dir_entry:>>>>>>  Directory Entry  <<<<<<"));

    cursor = navigate(cursor, ATTRIBUTE_SET, 0);

    FREE_CONTEXT;
    return (cursor);
}



/* -------------------------------------------------------------------------- */

static char 
*parse_data_bucket (char *cursor)
{
    struct context_buffer context;

    PREPARE_CONTEXT(100,200);

    DATUM_ADVANCE(LONGWORD, "next_p");
    DATUM_ADVANCE(LONGWORD, "prev_p");
    DATUM_ADVANCE(LONGWORD, "free space");
    WALK_OFFSET(BUCKET_SET, ENTRY_DATA_RECORD);

    FREE_CONTEXT;
    return (cursor);
}



/* -------------------------------------------------------------------------- */

static char 
*parse_entry_data_record (char *cursor)
{
    struct context_buffer context;
    char *base = cursor;
    unsigned long keyLength;
    int key_type;
    int name_flag,name_len;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        "parse_entry_data_record: >>>>>>  Entry Data Record  <<<<<<"));

    PREPARE_CONTEXT(100, 200);

    DATUM_HOLD(LONGWORD, "Length");
    keyLength = *((unsigned short *)cursor);
    ADVANCE;
    DATUM_HOLD(BYTE, "Key type");
    key_type = (int)EXT8(cursor);
    ADVANCE;
    switch (key_type) {
    default:
        DATUM_ADVANCE(BYTE, "Key sort");
        DATUM_HOLD(SIMPLE_NAME, "Normalized Entry name");
	name_flag = EXT8(((SimpleName_t *)cursor)->sn_flag);
	name_len = EXT8(((SimpleName_t *)cursor)->sn_length);
	ADVANCE;
        switch (name_flag) {
	case SN_typed:
	    DATUM_ADVANCE(SIMPLE_NAME, "Original Entry name");
	    break;
	case SN_normal:
	case SN_quoted:
	    VECTOR_ADVANCE( name_len, CASEMASK, "Original Entry casemask");
	    break;
	}
        cursor = navigate(base+keyLength, ATTRIBUTE_SET, 0);
	break;
    case 255:
        /* Key Type of 255 indicates is the null high-key holder - no data */
	cursor = base+keyLength;
	break;
    }
    FREE_CONTEXT;
    return (cursor);
}



/* -------------------------------------------------------------------------- */

static char 
*parse_attribute_set (char *cursor)
{
    struct context_buffer context;

    PREPARE_CONTEXT(50, 150);

    DATUM_ADVANCE(LONGWORD, "set_length");
    DATUM_ADVANCE(LONGWORD, "casemask");
    DATUM_ADVANCE(CTS, "Timestamp");
    DATUM_ADVANCE(BYTE, "Flags");
    DATUM_ADVANCE(ATTRIBUTE_TYPE, "Attribute type");
    WALK_OFFSET(DATABASE_SET, ATTRIBUTE);

    FREE_CONTEXT;
    return (cursor);
}




/* -------------------------------------------------------------------------- */

static char 
*parse_attribute (char  *cursor)
{
    struct context_buffer 	context;
    int				name_len;

    PREPARE_CONTEXT(50, 150);

    DATUM_HOLD(SIMPLE_NAME, "attr");
    name_len = EXT8(((SimpleName_t *)cursor)->sn_length);
    ADVANCE;
    DATUM_ADVANCE(LONGWORD, "set_length");
    VECTOR_ADVANCE( name_len, CASEMASK, "casemask");
    DATUM_ADVANCE(CTS, "timestamp");
    DATUM_ADVANCE(BYTE, "flags");
    DATUM_ADVANCE(ATTRIBUTE_TYPE, "type");
    WALK_OFFSET(DATABASE_SET, ATTRIBUTE_VALUE);
    FREE_CONTEXT;

    return (cursor);
}




/* -------------------------------------------------------------------------- */

static char 
*parse_attribute_value (char  *cursor)
{

    struct context_buffer context;
    int   valueLength;

    PREPARE_CONTEXT(10000, 20000);

    DATUM_HOLD(LONGWORD, "ValueLength");
    valueLength = EXT32(cursor);
    ADVANCE;
    DATUM_ADVANCE(CTS, "timestamp");
    DATUM_ADVANCE(VALUE_FLAGS, "Flags");
    DATUM_ADVANCE(VALUETYPE, "ValueType");
    VECTOR_ADVANCE(valueLength, CHARACTER, "Value");

    FREE_CONTEXT;
    return (cursor);
}



/* -------------------------------------------------------------------------- */

static char 
*parse_message_log_record (char *cursor)
{
    struct context_buffer context;
    int message_length;

    PREPARE_CONTEXT(20000, 40000);

    DATUM_ADVANCE(LONGWORD, "Thread ID");
    DATUM_ADVANCE(ABS_DATE, "Timestamp (UTC)");
    DATUM_ADVANCE(LONGWORD, "Message type");
    DATUM_ADVANCE(LONGWORD, "Object number");
    DATUM_HOLD(LONGWORD, "Message length");
    message_length = EXT32(cursor);
    ADVANCE;
    VECTOR_ADVANCE(message_length, CHARACTER, "Data");

    FREE_CONTEXT;
    return (cursor);
}




/* -------------------------------------------------------------------------- */

/*
 * ROUTINE - initialize_navigation
 *
 * This routine initializes the routine dispatch table for the structure
 * navigator.  It is called once by the structure navigator when it notes
 * that the dispatch table has not been initialized.
 *
 * Whenever a new data structure parser is added to this module, you must
 * register it in this routine.
 */
static void 
initialize_navigation (void)
{
    /* Initialize the dispatch structure to zeros */
    memset(&dispatch_table, 0, sizeof(struct dispatch_struct));

    /*
     * Register each structure we support along with its calling
     * format and the routine name to call.
     */

    REGISTER_STRUCTURE(TLOG_STRUCTURE,        CT_1, parse_tlog);
    REGISTER_STRUCTURE(DIRECTORY_ENTRY,       CT_1, parse_dir_entry);
    REGISTER_STRUCTURE(ATTRIBUTE_SET,         CT_1, parse_attribute_set);
    REGISTER_STRUCTURE(ATTRIBUTE,             CT_1, parse_attribute);
    REGISTER_STRUCTURE(ATTRIBUTE_VALUE,       CT_1, parse_attribute_value);
    REGISTER_STRUCTURE(CHECKPOINT_HEADER,     CT_1, parse_checkpoint_header);
    REGISTER_STRUCTURE(CHECKPOINT_STRUCTURE,  CT_1, parse_checkpoint_structure);
    REGISTER_STRUCTURE(DATA_BUCKET_STRUCTURE, CT_1, parse_data_bucket);
    REGISTER_STRUCTURE(ENTRY_DATA_RECORD,     CT_1, parse_entry_data_record);
    REGISTER_STRUCTURE(UPDATE_PACKET,         CT_1, parse_update_packet);
    REGISTER_STRUCTURE(UPDATE_STRUCTURE,      CT_1, parse_update_structure);
    REGISTER_STRUCTURE(MESSAGE_LOG_RECORD,    CT_1, parse_message_log_record);

    /* Log that the table has been initialized */
    dispatch_table.initialized = TRUE;

}


/* -------------------------------------------------------------------------- */

/*
 * ROUTINE - navigate
 *
 * This is the top-level routine for the structure navigator.  The caller
 * passes in the address of a data structure, and a structure_id code that
 * tells the navigator what it is parsing. [The structure length is optional
 * and is only used by a few structures.
 *
 * The valid list of structure id's can be found in "dump.h", but the
 * navigator only supports of subset of these.  See the initialize_navigation
 * routine (above) for the list of registered structure types.
 *
 * The navigate routine returns the address of the data structure that is
 * after the one which has just been dumped.
 */

char 
*navigate (char  *cursor,
           int   structure_id,
           int   structure_length)
{
    /*
     * First we check to see if the dispatch table is usable.  If not, we
     * initialize it.  If this could be done at compile/link-time, then
     * it would trim a microsecond or two off the navigation time.
     */
    if (dispatch_table.initialized == FALSE) {
        initialize_navigation();
    }


    /*
     * Lookup the type of call for the structure id that has been passed
     * in.  If we find a supported type, call the parsing routine with the
     * cursor (and other arguments as needed) - the particular parsing routine
     * we call is in the dispatch table, so we dereference the table entry
     * as part of the call.
     */
    if (dispatch_table.call_type[structure_id] == CT_1) {
        cursor = (*dispatch_table.routine[structure_id])(cursor);
/*
 *   } else if (dispatch_table.call_type[structure_id] == CT_2) {
 *       cursor = (*dispatch_table.routine[structure_id])(cursor, structure_length);
 */
    } else {
	/*
	 * This is serious - uknown structure - abort the dump 
	 */
	dce_svc_printf(CDS_S_DUMP_UNKNOWNSTRUCTURE_A_MSG, structure_id);
    }
    return (cursor);
}
