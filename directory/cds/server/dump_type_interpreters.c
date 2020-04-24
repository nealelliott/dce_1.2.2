/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dump_type_interpreters.c,v $
 * Revision 1.1.8.2  1996/02/18  19:36:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:48  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:30:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:21  root]
 * 
 * Revision 1.1.6.3  1994/08/03  19:04:16  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:25:34  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:43:29  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:02  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:11:42  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:43:18  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:22:04  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:22:26  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/20  20:47:46  mfox
 * 	Replace OP_CARDINALITY with NUM_OPCODES to avoid redefinition of constant
 * 	defined in dbdef.h
 * 	[1992/07/20  20:28:56  mfox]
 * 
 * Revision 1.1  1992/01/19  15:25:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: dump_type_interpreters.c
 * Version: X02.29
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
 */

/*
**++
**  FACILITY:
**
**      DECdns Dump Facility
**
**  ABSTRACT:
**
**      This module provides data interpretation routines for the
**	dump programs.
**
**  AUTHORS:
**
**      Dave Griffin, NaC/DPE
**
**  CREATION DATE:     12-Aug-89
**
**  MODIFICATION HISTORY:
**
**	X02.15	Alan Peckham		29-Jan-1991
**		Support for X.500 typed names in interpret_sname().
**
**	        Tamar R. Wexler		Mon Jun 10 14:00:48 1991
**		Added cast in interpret_sname to get module to compile on vms.
**
**	Gen 24	Alan Peckham		2-Jul-1991
**		Support ET_dirOrObj in readable_entity_type()
**
**	Gen 29	Alan Peckham		29-Aug-1991
**		Support VT_gdaPointer in interpret_valuetype()
**		
**		Tamar R. Wexler		Thu Oct  3 13:45:26 1991
**		Added support for casemask interpretation.
**--
**/


/*
 *
 *  INCLUDE FILES
 *
 */

#include <server.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <dce/sys_time.h>
#include <dump.h>
#include <dbdef.h>
#include <tlog.h>

#  include <dce/rpc.h>

/*
 *
 *                         Global Data Structures
 *
 */


/*
 * The op structure holds readable interpretation of name service
 * operation codes.
 */

#define NUM_OPS 51
static struct
    {
    char name[30];
    unsigned long value;
    }  op[NUM_OPS] =
       {
           { "ENUMERATE_ATT", OP_EnumerateAttributes },
           { "READ_ATT", OP_ReadAttribute },
           { "MODIFY_ATT", OP_ModifyAttribute },
           { "TEST_ATT", OP_TestAttribute },
           { "CREATE_OBJ", OP_CreateObject },
           { "ENUMERATE_OBJ", OP_EnumerateObjects },
           { "DELETE_OBJ", OP_DeleteObject },
           { "CREATE_DIR", OP_CreateDirectory },
           { "DELETE_DIR", OP_DeleteDirectory },
           { "ADD_REPLICA", OP_AddReplica },
           { "REMOVE_REPLICA", OP_RemoveReplica },
           { "ENUMERATE_CHILD", OP_EnumerateChildren },
           { "SKULK", OP_Skulk },
           { "CREATE_LINK", OP_CreateLink },
           { "DELETE_LINK", OP_DeleteLink },
           { "RESOLVE_NAME",OP_ResolveName },
           { "TEST_GROUP", OP_TestGroup },
           { "CREATE_CHILD", OP_CreateChildEntry },
           { "ENUMERATE_LINK", OP_EnumerateLinks },
           { "DELETE_CHILD", OP_DeleteChildEntry },
           { "READ_REPLICA", OP_ReadReplica },
           { "MODIFY_REPLICA", OP_ModifyReplica },
           { "LINK_REPLICA", OP_LinkReplica },
           { "COMBINE", OP_Combine },
           { "DOUPDATE", OP_DoUpdate }
       };


/*
 * ROUTINE - format_hex
 *
 * The format hex routine accepts the address and length of a section
 * of memory and provides a hexadecimal interpretation in the output
 * string.
 *
 * format_hex does not do any output string allocation, it assumes that
 * the provided output argument points to an area sufficient to store the
 * hex interpretation of the input (which is always 3 times the input
 * length).
 */

static void 
format_hex (char  *output,
            char  *input,
            int   length)
{

   /*
    * Iterate through the input value and generate hex strings in the
    * output area.  Each pass of the loop advances the input pointer ahead
    * one byte, and the output pointer 3 bytes.
    *
    * Because our input is a signed character, we have to typecast the
    * the input into an unsigned value before formatting it.
    */

   if (length > 0) {
       while (length > 0) {
           sprintf(output, "%02X ", *((unsigned char*)input));
           output = output + 3;
           input++;
           length--;
       }

       /*
        * We're all done, let's get rid of that pesky trailing space...
        */
       output--;
   }

   /* Null-terminate the output string */
   *output = '\0';
}



/*
 * ROUTINE - format_ascii
 *
 * The format_ascii routine accepts the address and length of a section
 * of memory and provides a readable ASCII interpretation in the output
 * string.  If the value is considered printable (by ASCII standards)
 * then it is passed along unchanged.  Otherwise, the character value is
 * replaced with the "." glyph.
 *
 * format_ascii does not do any output string allocation, it assumes that
 * the provided output argument points to an area sufficient to store the
 * ascii interpretation of the input (which is always equal to the length
 * argument supplied).
 */

static void 
format_ascii (char  *output,
              char  *input,
              int   length)
{

   /*
    * Iterate through the input value and generate readable glyphs in
    * the output area.  Each pass of the loop advances the input pointer
    * and output pointers one byte.
    *
    * The readability (printability) of a character is based on whether or
    * not it's decimal value lies within the range of 32 to 126.
    */

   while (length > 0) {
       if ((*input >= 32) && (*input <= 126)) {
           *output = *input;
       } else {
           *output = '.';
       }
       output++;
       input++;
       length--;
   }

   *output = '\0';
}




/*
 *
 *              ============================
 *              Type Interpretation Routines
 *              ============================
 *
 *
 *
 * The following set of routines are the individual data type interpreters.
 *
 * Type interpreters have 2 calling formats, one for single values, and
 * and a variation for vectors.
 *
 * The standard arguments are:
 *
 *    cursor   IN  - Pointer to the datum to interpret
 *    s_size   OUT - Pointer to the integer to receive the size of the datum
 *    i_value  OUT - Pointer to the area where the interpreted value goes
 *    h_value  OUT - Pointer to the area where the hex value is placed
 *
 * Vector routines have a single additional argument (preceding the standard
 * ones):
 *
 *    cardinality   IN - Number of elements in the vector
 *
 * Not all types have vector and non-vector variations implemented.
 *
 */


static void 
interpret_longword (char  *cursor,
                    int   *s_size,
                    char  *i_value,
                    char  *h_value)
{
    unsigned long *value;        /* We cast the input here - code readability */

    value = (unsigned long *) cursor;    /* Cast value into a longword */

    sprintf(i_value,"%u", EXT32(value));
    sprintf(h_value,"%08X", EXT32(value));
    *s_size = sizeof(unsigned long);
}

static void 
interpret_casemask_vector (int   cardinality,
                           char  *cursor,
                           int   *s_size,
                           char  *i_value,
                           char  *h_value)
{
    int		bits_per_byte = 8,		 /* # of bits in a byte  */
		max_casemask_size = sizeof(long);
    int		ndx,				 /* index into casemask  */
		casemask = 0;			 /* casemask */
    

    if ((cardinality+(bits_per_byte-1))/bits_per_byte > max_casemask_size)
    {					   	/* If casemask no longer  */
	dce_fprintf(stdout, cds_s_dump_casemask);
	casemask = EXT32(cursor);
    }
    else					 /* Get value of casemask  */
	{
	for (ndx = 0; ndx < cardinality; ndx++)
	    {					 /* Only get bits that are */
	    casemask |=	 	 		 /* part of the casemask.  */
		(1<<ndx & EXT32(cursor));
	    }
	}
    sprintf(i_value,"%u", casemask);
    sprintf(h_value,"%08X",casemask);		 /* Get casemask  */

    *s_size = max_casemask_size;		 /* Skip space left for mask */
}


static void 
interpret_longword_vector (int   cardinality,
                           char  *cursor,
                           int   *s_size,
                           char  *i_value,
                           char  *h_value)
{
    int x;
    unsigned long *value;

    value = (unsigned long *) cursor;    /* Cast value into a longword */

    strcpy(i_value, "");                 /* No attempt to interpret this */

    for (x=0; x<cardinality; x++)
         {
         sprintf(h_value, "%08X ", EXT32(value));
	 ++value;
         h_value = h_value + 9;
         }
    *h_value = '\0';

    *s_size = sizeof(unsigned long) * cardinality;
}




static void 
interpret_word (char  *cursor,
                int   *s_size,
                char  *i_value,
                char  *h_value)
{
    unsigned short *value;        /* We cast the input here - code readability */

    value = (unsigned short *) cursor;    /* Cast value into a longword */

    sprintf(i_value,"%u", EXT16(value));
    sprintf(h_value,"%04X", EXT16(value));
    *s_size = sizeof(unsigned short);
}



static void 
interpret_word_vector (int   cardinality,
                       char  *cursor,
                       int   *s_size,
                       char  *i_value,
                       char  *h_value)
{
    int x;
    unsigned short *value;

    value = (unsigned short *) cursor;    /* Cast value into a longword */

    strcpy(i_value, "");                 /* No attempt to interpret this */

    for (x=0; x<cardinality; x++)
         {
         sprintf(h_value, "%04X ", EXT16(value));
	 ++value;
         h_value = h_value + 5;
         }
    *h_value = '\0';

    *s_size = sizeof(unsigned short) * cardinality;
}



static void 
interpret_byte (char  *cursor,
                int   *s_size,
                char  *i_value,
                char  *h_value)
{
  sprintf(i_value,"%d", EXT8(cursor) );
  sprintf(h_value,"%02X", EXT8(cursor) );
  *s_size = sizeof(char);
}



static void 
interpret_character (char  *cursor,
                     int   *s_size,
                     char  *i_value,
                     char  *h_value)
{
    format_ascii(i_value, cursor, 1);
    sprintf(h_value,"%02X", EXT8(cursor) );
    *s_size = sizeof(char);
}



static void 
interpret_character_vector (int   cardinality,
                            char  *cursor,
                            int   *s_size,
                            char  *i_value,
                            char  *h_value)
{
    format_ascii(i_value, cursor, cardinality);
    format_hex(h_value, cursor, cardinality);
    *s_size = sizeof(char) * cardinality;
}



static void 
interpret_date (char  *cursor,
                int   *s_size,
                char  *i_value,
                char  *h_value)
{
   time_quad_t time;

   format_hex(h_value, cursor, sizeof(Time_u));
   COPY_Time((time_quad_t *)cursor, &time);
   sys_utc_to_string(&time, i_value, 80);
   *s_size = sizeof(Time_u);
}



static void 
interpret_CTS (char  *cursor,
               int   *s_size,
               char  *i_value,
               char  *h_value)
{
   Timestamp_t *ts_in = (Timestamp_t *)cursor;
   time_quad_t time;

   format_hex(h_value, cursor, sizeof(Timestamp_u));
   COPY_Time(ts_in->ts_time, &time);
   sys_utc_to_string(&time, i_value, 80);
   i_value += strlen(i_value);
   *i_value++ = '/';
   sprintf(i_value, "%02X-%02X-%02X-%02X-%02X-%02X",
	 ts_in->ts_node[0],
	 ts_in->ts_node[1],
	 ts_in->ts_node[2],
	 ts_in->ts_node[3],
	 ts_in->ts_node[4],
	 ts_in->ts_node[5]
	);
   *s_size = sizeof(Timestamp_u);
}
static void 
interpret_UID (char  *cursor,
               int   *s_size,
               char  *i_value,
               char  *h_value)
{

  unsigned32 status;
  unsigned_char_t *value;
  uuid_t id;
  
  format_hex(h_value, cursor, sizeof(ObjUID_u));
  
  memcpy(&id, cursor, sizeof(uuid_t));
  
  uuid_to_string(&id, &value, &status);
  strcpy(i_value, (char *)value);
  rpc_string_free(&value, &status);
  
  *s_size = sizeof(ObjUID_u);
}



static void 
interpret_fname (char  *cursor,
                 int   *s_size,
                 char  *i_value,
                 char  *h_value)
{
    FullName_t *name;                    /* support the typecast below */

    name = (FullName_t *) cursor;        /* typecast for readability */

    *s_size = LEN_FullName(name);

    format_ascii(i_value, cursor, *s_size);
    format_hex(h_value, cursor, *s_size);
}



static void 
interpret_sname (char  *cursor,
                 int   *s_size,
                 char  *i_value,
                 char  *h_value)
{
    SimpleName_t *name;
    char flag_interpretation[10];
    unsigned char name_interpretation[DNS_STR_SNAME_MAX];
    unsigned char dottystr[DNS_STR_SNAME_MAX];
    int dottylen = sizeof(dottystr);
    int name_len = sizeof(name_interpretation);

    name = (SimpleName_t *) cursor;

    switch (EXT8(name->sn_flag))
        {
        case SN_null :
            strcpy(flag_interpretation, "end");
            break;
        case SN_normal :
            strcpy(flag_interpretation, "asc");
            break;
        case SN_quoted :
            strcpy(flag_interpretation, "quo");
            break;
        case SN_binary :
            strcpy(flag_interpretation, "bin");
            break;
        case SN_wildcard :
            strcpy(flag_interpretation, "wild");
            break;
        case SN_ellipsis :
            strcpy(flag_interpretation, "elip");
            break;
	case SN_typed :
            strcpy(flag_interpretation, "type");
            break;
	case SN_objectid :
            strcpy(flag_interpretation, "objid");
            break;
	case SN_cds :
            strcpy(flag_interpretation, "cdswild");
            break;
	case SN_cdswildcard :
            strcpy(flag_interpretation, "cdswild");
            break;
        default :
            strcpy(flag_interpretation, "*ERROR*");
            break;
        }

    if (EXT8(name->sn_flag) == SN_objectid) {
	dnsCvtObjIDToStr((dns_attr_name_t *)name, NULL,
		name_interpretation, &name_len, dottystr, &dottylen);
	if (name_len == 0)
	  strcpy((char *)name_interpretation,  (char *)dottystr);
    } else
      format_ascii((char *)name_interpretation, (char *)name->sn_name, EXT8(name->sn_length));

    sprintf(i_value,
            "(%s/%d) %s",
            flag_interpretation, EXT8(name->sn_length), name_interpretation
           );

    *s_size = LEN_SimpleName(name);

    format_hex(h_value, cursor, *s_size);

}



static void 
interpret_tlog_entry_type (char  *cursor,
                           int   *s_size,
                           char  *i_value,
                           char  *h_value)
{
    unsigned long *value;

    value = (unsigned long *) cursor;    /* Cast value into a longword */

    switch (EXT32(value))
        {
        case TLOG_CREATE :
            strcpy(i_value, "CREATE");
            break;
        case TLOG_UPDATE :
            strcpy(i_value, "UPDATE");
            break;
        default :
            strcpy(i_value, "**UNKNOWN**");
            break;
        }
    sprintf(h_value, "%08X", EXT32(cursor) );
    *s_size = sizeof(unsigned long);
}



static void 
interpret_operation (char  *cursor,
                     int   *s_size,
                     char  *i_value,
                     char  *h_value)
{
    unsigned long *value;
    int i;

    value = (unsigned long *) cursor;    /* Cast value into a longword */

    strcpy(i_value, "**UNKNOWN**");
    for (i = 0; i < NUM_OPS; i++)
        {
            if (EXT32(value) == op[i].value)
                {
                strcpy(i_value, op[i].name);
                break;
                }
        }
    sprintf(h_value, "%08X", EXT32(cursor) );
    *s_size = sizeof(unsigned long);
}



static void 
interpret_value_flags (char  *cursor,
                       int   *s_size,
                       char  *i_value,
                       char  *h_value)
{
    struct value_flags_vector {
        unsigned char present_flag[1];
        unsigned char value_type[1];
        unsigned char valid_flag[1];
        unsigned char data_present_flag[1];
    } *value;

    char present_interpretation[30];
    char type_interpretation[30];
    char valid_interpretation[30];
    char data_present_interpretation[30];

    value = (struct value_flags_vector *) cursor;    /* Cast value */

    if (EXT8(value->present_flag) == DM_present) {
        strcpy(present_interpretation, "Present");
    } else if (EXT8(value->present_flag) == DM_absent) {
        strcpy(present_interpretation, "Absent");
    } else {
        strcpy(present_interpretation, "**Illegal presence**");
    }

    if (EXT8(value->value_type) == AT_null) {
        strcpy(type_interpretation, "Null");
    } else if (EXT8(value->value_type) == AT_single) {
        strcpy(type_interpretation, "Single");
    } else if (EXT8(value->value_type) == AT_set) {
        strcpy(type_interpretation, "Set");
    } else {
        strcpy(type_interpretation, "**Illegal type**");
    }

    if (EXT8(value->valid_flag) == TRUE) {
        strcpy(valid_interpretation, "Valid");
    } else if (EXT8(value->valid_flag) == FALSE) {
        strcpy(valid_interpretation, "Not valid");
    } else {
        strcpy(valid_interpretation, "**Illegal validity**");
    }

    if (EXT8(value->data_present_flag) == TRUE) {
        strcpy(data_present_interpretation, "Data present");
    } else if (EXT8(value->data_present_flag) == FALSE) {
        strcpy(data_present_interpretation, "Data absent");
    } else {
        strcpy(data_present_interpretation, "**Illegal data presence**");
    }

    sprintf(i_value, "%s, %s, %s, %s",
            present_interpretation,
            type_interpretation,
            valid_interpretation,
            data_present_interpretation
           );

    format_hex(h_value, cursor, sizeof(struct value_flags_vector));

    *s_size = sizeof(struct value_flags_vector);
}



static void 
interpret_attribute_type (char  *cursor,
                          int   *s_size,
                          char  *i_value,
                          char  *h_value)
{
    unsigned char *value;

    value = (unsigned char *) cursor;    /* Cast value into a byte */

    switch ((int)*value) {
        case AT_null :
            strcpy(i_value, "Null");
            break;
        case AT_single :
            strcpy(i_value, "Single");
            break;
        case AT_set :
            strcpy(i_value, "Set");
            break;
        default :
            strcpy(i_value, "**UNKNOWN**");
    }

    sprintf(h_value, "%02X", EXT8(cursor) );
    *s_size = sizeof(unsigned char);
}

static void 
interpret_valuetype (char  *cursor,
                     int   *s_size,
                     char  *i_value,
                     char  *h_value)
{
    unsigned char *value;

    value = (unsigned char *) cursor;    /* Cast value into a byte */

    switch ((int) *value) {
        case VT_none :
            strcpy(i_value, "Null");
            break;
        case VT_long:
            strcpy(i_value, "Long");
            break;
        case VT_short:
            strcpy(i_value, "Short");
            break;
        case VT_small:
            strcpy(i_value, "Small");
            break;
        case VT_uuid:
            strcpy(i_value, "Uuid");
            break;
	case VT_ReplicaPointer:
            strcpy(i_value, "ReplicaPointer");
            break;
	case VT_GroupMember:
            strcpy(i_value, "GroupMember");
            break;
        case VT_ParentPointer:
            strcpy(i_value, "ParentPointer");
            break;
        case VT_gdaPointer:
            strcpy(i_value, "gdaPointer");
            break;
        case VT_char:
            strcpy(i_value, "Char");
            break;
        case VT_byte:
            strcpy(i_value, "Byte");
            break;
        case VT_FullName:
            strcpy(i_value, "FullName");
            break;
        case VT_Timeout:
            strcpy(i_value, "Timeout");
            break;
        case VT_Timestamp:
            strcpy(i_value, "Timestamp");
            break;
        case VT_CHDirectory:
            strcpy(i_value, "CHDirectory");
            break;
        case VT_Version:
            strcpy(i_value, "Verison");
            break;
        case VT_ASN1:
            strcpy(i_value, "ASN1");
            break;
	case VT_DACL:
	    strcpy(i_value, "ACL");
	    break;
        default :
            strcpy(i_value, "**UNKNOWN**");
    }

    sprintf(h_value, "%02X", EXT8(cursor) );
    *s_size = sizeof(unsigned char);
}


static void 
readable_entity_type (int   value,
                      char  *i_value)
{
    switch (value) {
        case ET_directory:
            strcpy(i_value, "Directory");
            break;
        case ET_object:
            strcpy(i_value, "Object");
            break;
       case ET_childPointer:
            strcpy(i_value, "Child pointer");
            break;
        case ET_softlink:
            strcpy(i_value, "Softlink");
            break;
        case ET_clearinghouse:
            strcpy(i_value, "Clearinghouse");
            break;
        case ET_anyDirectName:
            strcpy(i_value, "Any");
            break;
        case ET_dirOrObj:
            strcpy(i_value, "Directory or Object");
            break;
        default :
            strcpy(i_value, "**UNKNOWN**");
    }

}


static void 
interpret_entity_type_wire (char  *cursor,
                            int   *s_size,
                            char  *i_value,
                            char  *h_value)
{
    unsigned char *value;

    value = (unsigned char *) cursor;    /* Cast value into a byte */
    readable_entity_type((int) *value, i_value);
    sprintf(h_value, "%02X", *((unsigned char *) cursor) );
    *s_size = sizeof(unsigned char);
}


static void 
interpret_entity_type_db (char  *cursor,
                          int   *s_size,
                          char  *i_value,
                          char  *h_value)
{
    unsigned long *value;

    value = (unsigned long *) cursor;    /* Cast value into a longword */
    readable_entity_type(EXT32(value), i_value);
    sprintf(h_value, "%08X", EXT32(cursor) );
    *s_size = sizeof(unsigned long);
}




/*
 * ROUTINE - interpret
 *
 * The interpret routine dispatches one of the type interpreters
 * to transform the input data into an ascii and hexadecimal format
 * based on the desired semantics of the data.
 */

int 
interpret (int   s_type,
           int   cardinality,
           char  *cursor,
           int   *s_size,
           char  *i_value,
           char  *h_value)
{
    switch (s_type)
    {
        case LONGWORD :
            if (cardinality == 1)
                {
                interpret_longword(cursor, s_size, i_value, h_value);
                }
            else
                {
                interpret_longword_vector(cardinality, cursor, s_size, i_value, h_value);
                }
            break;
        case WORD :
            if (cardinality == 1)
                {
                interpret_word(cursor, s_size, i_value, h_value);
                }
            else
                {
                interpret_word_vector(cardinality, cursor, s_size, i_value, h_value);
                }
            break;
        case BYTE :
            interpret_byte(cursor, s_size, i_value, h_value);
            break;
        case ABS_DATE :
            interpret_date(cursor, s_size, i_value, h_value);
            break;
        case CTS :
            interpret_CTS(cursor, s_size, i_value, h_value);
            break;
        case UID :
            interpret_UID(cursor, s_size, i_value, h_value);
            break;
        case FULL_NAME :
            interpret_fname(cursor, s_size, i_value, h_value);
            break;
        case SIMPLE_NAME :
            interpret_sname(cursor, s_size, i_value, h_value);
            break;
        case OPERATION :
            interpret_operation(cursor, s_size, i_value, h_value);
            break;
        case VALUETYPE :
            interpret_valuetype(cursor, s_size, i_value, h_value);
            break;
        case TLOG_ENTRY_TYPE :
            interpret_tlog_entry_type(cursor, s_size, i_value, h_value);
            break;
        case CHARACTER :
            if (cardinality == 1)
                {
                interpret_character(cursor, s_size, i_value, h_value);
                }
            else
                {
                interpret_character_vector(cardinality, cursor, s_size, i_value, h_value);
                }
            break;
        case VALUE_FLAGS :
            interpret_value_flags(cursor, s_size, i_value, h_value);
            break;
        case ATTRIBUTE_TYPE :
            interpret_attribute_type(cursor, s_size, i_value, h_value);
            break;
        case ENTITY_TYPE_WIRE :
            interpret_entity_type_wire(cursor, s_size, i_value, h_value);
            break;
        case ENTITY_TYPE_DB :
            interpret_entity_type_db(cursor, s_size, i_value, h_value);
            break;
	case CASEMASK :
	    interpret_casemask_vector( 
		cardinality, cursor, s_size, i_value, h_value);
	    break;
         default :
	     /*
	      * Unsupported interpreter type --> ABORT !!
	      */
	     dce_svc_printf(CDS_S_DUMP_TYPE_A_MSG);
    }
    return(1);
}
