//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_dump_key.cxx,v $
// Revision 1.1.2.1  1996/10/03  20:31:20  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/03  19:30:12  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:30 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:57 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// Module: sec_pkss_dump_key.cxx
//
// Purpose:  This module contains functions from
//	     src/directory/cds/gloria/cds_export_lib.c and
//	     src/directory/cds/gloria/cds_ieu_type_interpreters.c
//	     to dump the contents of asymmetric key pairs because
//	     GLORIA will not be submitted at this time (7/96).
//	     This module should be replaced by the two GLORIA
//	     modules above when GLORIA is submitted.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef EF_INDENT
#define EF_INDENT     "          "
#endif

void 
cds_ieu_emit (const char *field_name_p,
	      const char *i_val,
	      const char *h_val)
#define OUTPUT_MARGIN 63
{

//
// Local variables
//
   int output_max;         // work variable for the margin//

//
// Calculate our output margin based on adding in the title for the
// first line and print it.
//
   output_max = OUTPUT_MARGIN - (strlen(field_name_p) + 2) + strlen (EF_INDENT);
   (void) fprintf (stdout, "%s: %0.*s\n", field_name_p, output_max, i_val);

//
// All lines after this are indented EF_INDENT spaces.
//
// Walk through the interpreted value using the OUTPUT_MARGIN as
// the increment.  We use fprintf's capability of setting a maximum
// size (precision) for string variables.
//
// If the length of the string to be printed exceeds the margin size
// we print a margin's worth of characters and then slide the pointer
// over a margin's worth... and start over again.
//

   while (strlen (i_val) > output_max)
   {
     i_val = i_val + output_max;
     output_max = OUTPUT_MARGIN -
		  (strlen(field_name_p) + 2) +
		  strlen (EF_INDENT);
     (void) fprintf (stdout, "%s: %0.*s\n", field_name_p, output_max, i_val);
   }

//
// We follow the same logic here for the hexadecimal interpretation
// only there's no title line to deal with.
//

   output_max = OUTPUT_MARGIN;
   (void) fprintf (stdout, "%s%0.*s\n", EF_INDENT, output_max, h_val);
   while (strlen(h_val) > output_max)
   {
     h_val = h_val + output_max;
     output_max = OUTPUT_MARGIN;
     (void) fprintf (stdout, "%s%0.*s\n", EF_INDENT, output_max, h_val);
   }

} // End cds_ieu_emit//

//
// ROUTINE - format_hex
//
// The format hex routine accepts the address and length of a section
// of memory and provides a hexadecimal interpretation in the output
// string.
//
// format_hex does not do any output string allocation, it assumes that
// the provided output argument points to an area sufficient to store the
// hex interpretation of the input (which is always 3 times the input
// length).
//

void 
format_hex (char  *output,
            char  *input,
            int   length)
{

   //
   // Iterate through the input value and generate hex strings in the
   // output area.  Each pass of the loop advances the input pointer ahead
   // one byte, and the output pointer 3 bytes.
   //
   // Because our input is a signed character, we have to typecast the
   // the input into an unsigned value before formatting it.
   //

   if (length > 0) {
       while (length > 0) {
           sprintf(output, "%02X ", *((unsigned char*)input));
           output = output + 3;
           input++;
           length--;
       }

       //
       // We're all done, let's get rid of that pesky trailing space...
       //
       output--;
   }

   // Null-terminate the output string//
   *output = '\0';
}


//
// ROUTINE - format_ascii
//
// The format_ascii routine accepts the address and length of a section
// of memory and provides a readable ASCII interpretation in the output
// string.  If the value is considered printable (by ASCII standards)
// then it is passed along unchanged.  Otherwise, the character value is
// replaced with the "." glyph.
//
// format_ascii does not do any output string allocation, it assumes that
// the provided output argument points to an area sufficient to store the
// ascii interpretation of the input (which is always equal to the length
// argument supplied).
//

void 
format_ascii (char  *output,
              char  *input,
              int   length)
{
   //
   // Iterate through the input value and generate readable glyphs in
   // the output area.  Each pass of the loop advances the input pointer
   // and output pointers one byte.
   //
   // The readability (printability) of a character is based on whether or
   // not it's decimal value lies within the range of 32 to 126.
   //

   while (length > 0)
   {
     if ((*input >= 32) && (*input <= 126))
     {
       *output = *input;
     }
       
     else
     {
       *output = '.';
     }
     output++;
     input++;
     length--;
   }

   *output = '\0';
}

//
//    cardinality   IN - Number of elements in the vector
//    cursor   IN  - Pointer to the datum to interpret
//    s_size   OUT - Pointer to the integer to receive the size of the datum
//    i_value  OUT - Pointer to the area where the interpreted value goes
//    h_value  OUT - Pointer to the area where the hex value is placed
//

void 
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

