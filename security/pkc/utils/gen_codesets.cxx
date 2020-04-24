/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: gen_codesets.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:42:08  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:03:43  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/11/04  22:27 UTC  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  23:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 * 
 * $EndLog$
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define INVALID_T61_CHAR 0x3f
#define INVALID_IA5_CHAR 0x3f
#define INVALID_ISO646_CHAR 0x3f
#define INVALID_LOCAL_CHAR '?'


// This macro lets you use unsigned notation for character
// constants, regardless of whether the platform treats characters as
// signed or unsigned.  It does assume that the characterset of the
// platform is limited to -128..127 for signed character implementations
#if (CHAR_MIN == 0)
#define c(x) x
#else
#define c(x) ((x >= 0)?x:256+x)
#endif

#include "iso646_mapping.h"
#include "ia5_mapping.h"
#include "t61_mapping.h"



unsigned char local_to_iso646_array[256];

unsigned char local_to_t61_array[256];

unsigned char local_to_ia5_array[256];



char str[8];

char * c_rep(char c) {
    int uc;
    if ((c == '\'') || (c == '\?') || (c == '\\') || (c == '\"')) {
	str[0] = '\'';
	str[1] = '\\';
	str[2] = c;
	str[3] = '\'';
	str[4] = 0;
	return str;
    } else if (isprint(c)) {
	str[0] = '\'';
	str[1] = c;
	str[2] = '\'';
	str[3] = '\0';
	return str;
    } else if (c == '\n') {
	str[0] = '\'';
	str[1] = '\\';
	str[2] = 'n';
	str[3] = '\'';
	str[4] = '\0';
	return str;
    } else if (c == '\t') {
	str[0] = '\'';
	str[1] = '\\';
	str[2] = 't';
	str[3] = '\'';
	str[4] = '\0';
	return str;
    } else if (c == '\v') {
	str[0] = '\'';
	str[1] = '\\';
	str[2] = 'v';
	str[3] = '\'';
	str[4] = '\0';
	return str;
    } else if (c == '\b') {
	str[0] = '\'';
	str[1] = '\\';
	str[2] = 'b';
	str[3] = '\'';
	str[4] = '\0';
	return str;
    } else if (c == '\r') {
	str[0] = '\'';
	str[1] = '\\';
	str[2] = 'r';
	str[3] = '\'';
	str[4] = '\0';
	return str;
    } else if (c == '\f') {
	str[0] = '\'';
	str[1] = '\\';
	str[2] = 'f';
	str[3] = '\'';
	str[4] = '\0';
	return str;
    } else if (c == '\a') {
	str[0] = '\'';
	str[1] = '\\';
	str[2] = 'a';
	str[3] = '\'';
	str[4] = '\0';
	return str;
    };
    if (c < 0) {
       sprintf(str, "%d", c);
    } else {
       sprintf(str, "'\\x%2.2x'", c);
    };
    return str;
}


#define O(x) fprintf(out, x "\n");
#define O1(x, y) fprintf(out, x "\n", y);
#define O2(x, y, z) fprintf(out, x "\n", y, z);
#define O3(x, y, z, a) fprintf(out, x "\n", y, z, a);


int main(int argc, char * argv[] ) {
    FILE * out;
    int i;
    int j;

        
    if (argc != 2) {
	fprintf(stderr, "Usage: gen_codesets <output-file>\n");
	return EXIT_FAILURE;
    };
    out = fopen(argv[1], "w");    
    if (out == NULL) {
	fprintf(stderr, "Error: Can't open %s for writing\n", argv[1]);
	return EXIT_FAILURE;
    };

    for (i=0; i<256; i++) {
        local_to_iso646_array[i] = INVALID_ISO646_CHAR;
        local_to_ia5_array[i] = INVALID_IA5_CHAR;
        local_to_t61_array[i] = INVALID_T61_CHAR;
    };

    for (i=0; i<256; i++) {
    	if (iso646_to_local_array[i] != 0)
            local_to_iso646_array[iso646_to_local_array[i]] = i;
    	if (ia5_to_local_array[i] != 0)
            local_to_ia5_array[ia5_to_local_array[i]] = i;
        if (t61_to_local_array[i] != 0)
            local_to_t61_array[t61_to_local_array[i]] = i;
    };

    for (i=0; i<256; i++) {
    	if (iso646_to_local_array[i] == 0)
            iso646_to_local_array[i] = INVALID_LOCAL_CHAR;
    	if (ia5_to_local_array[i] == 0)
            ia5_to_local_array[i] = INVALID_LOCAL_CHAR;
        if (t61_to_local_array[i] == 0)
            t61_to_local_array[i] = INVALID_LOCAL_CHAR;
    };

// Fixup alternate obsolete T61 conversions.  These are accepted
// for conversions from T61, but should never be generated
    t61_to_local_array[35] = t61_to_local_array[166];  // Alternate hash code
    t61_to_local_array[36] = t61_to_local_array[168];  // Alternate currency code
    t61_to_local_array[201] = t61_to_local_array[198]; // Alternate umlaut code


O("#ifndef PKC_CODESETS_")
O("#define PKC_CODESETS_")
O("")
O("static char t61_to_local_array[256] = {")
O("/* This array maps T61 characters to local 'C' characters.         */")
O("/* Unsupported T61 characters are mapped to the C '?' character.   */")
    for (i=0;i<16;i++) {
        for (j=0;j<15;j++) {
            O3("%s,    /* %d/%d */", c_rep(t61_to_local_array[i*16+j]), i, j);
        };
	if (i<15) {
            O3("%s,    /* %d/%d */", c_rep(t61_to_local_array[i*16+j]), i, j);
            O("");
	} else {
            O3("%s     /* %d/%d */", c_rep(t61_to_local_array[i*16+j]), i, j);
        };
    };
O("};")
O("")


O("static char ia5_to_local_array[256] = {")
O("/* This array maps IA5 characters to local 'C' characters.         */")
O("/* Unsupported IA5 characters are mapped to the C '?' character.   */")
    for (i=0;i<16;i++) {
        for (j=0;j<15;j++) {
            O3("%s,    /* %d/%d */", c_rep(ia5_to_local_array[i*16+j]), i, j);
        };
	if (i<15) {
            O3("%s,    /* %d/%d */", c_rep(ia5_to_local_array[i*16+j]), i, j);
            O("");
	} else {
            O3("%s     /* %d/%d */", c_rep(ia5_to_local_array[i*16+j]), i, j);
        };
    };
O("};")
O("")


O("static char iso646_to_local_array[256] = {")
O("/* This array maps ISO646 characters to local 'C' characters.         */")
O("/* Unsupported ISO646 characters are mapped to the C '?' character.   */")
    for (i=0;i<16;i++) {
        for (j=0;j<15;j++) {
            O3("%s,    /* %d/%d */", c_rep(iso646_to_local_array[i*16+j]), i, j);
        };
	if (i<15) {
            O3("%s,    /* %d/%d */", c_rep(iso646_to_local_array[i*16+j]), i, j);
            O("");
	} else {
            O3("%s     /* %d/%d */", c_rep(iso646_to_local_array[i*16+j]), i, j);
        };
    };
O("};")
O("")



O("static unsigned char local_to_t61_array[256] = {")
O("/* This array maps local 'C' characters to T61 characters.         */")
O("/* Unsupported 'C' characters are mapped to the T61 '?' character. */")
    for (i=0;i<16;i++) {
        for (j=0;j<15;j++) {
            O3("0x%2.2x,    /* %d/%d */", local_to_t61_array[i*16+j], i, j);
        };
	if (i<15) {
            O3("0x%2.2x,    /* %d/%d */", local_to_t61_array[i*16+j], i, j);
            O("");
	} else {
            O3("0x%2.2x     /* %d/%d */", local_to_t61_array[i*16+j], i, j);
        };
    };
O("};")
O("")


O("static unsigned char local_to_ia5_array[256] = {")
O("/* This array maps local 'C' characters to IA5 characters.         */")
O("/* Unsupported 'C' characters are mapped to the IA5 '?' character. */")
    for (i=0;i<16;i++) {
        for (j=0;j<15;j++) {
            O3("0x%2.2x,    /* %d/%d */", local_to_ia5_array[i*16+j], i, j);
        };
	if (i<15) {
            O3("0x%2.2x,    /* %d/%d */", local_to_ia5_array[i*16+j], i, j);
            O("");
	} else {
            O3("0x%2.2x     /* %d/%d */", local_to_ia5_array[i*16+j], i, j);
        };
    };
O("};")
O("")

O("static unsigned char local_to_iso646_array[256] = {")
O("/* This array maps local 'C' characters to ISO646 characters.         */")
O("/* Unsupported 'C' characters are mapped to the ISO646 '?' character. */")
    for (i=0;i<16;i++) {
        for (j=0;j<15;j++) {
            O3("0x%2.2x,    /* %d/%d */", local_to_iso646_array[i*16+j], i, j);
        };
	if (i<15) {
            O3("0x%2.2x,    /* %d/%d */", local_to_iso646_array[i*16+j], i, j);
            O("");
	} else {
            O3("0x%2.2x     /* %d/%d */", local_to_iso646_array[i*16+j], i, j);
        };
    };
O("};")
O("")


O("char t61_to_local(unsigned char t61c) {")
O("   if (t61c > 255) return '?';")
O("   return t61_to_local_array[t61c];")
O("}")
O("")
O("char ia5_to_local(unsigned char ia5c) {")
O("    if (ia5c > 255) return '?';")
O("    return ia5_to_local_array[ia5c];")
O("}")
O("")
O("char iso646_to_local(unsigned char iso646c) {")
O("    if (iso646c > 127) return '?';")
O("    return iso646_to_local_array[iso646c];")
O("}")
O("")
O("unsigned char local_to_t61(char c) {")
O("    int uc;")
O("    if (c > 255) return 0x3f; // '?'")
O("    uc = ((c >= 0) ? c : c + 256);")
O("    return local_to_t61_array[uc];")
O("}")
O("")
O("unsigned char local_to_ia5(char c) {")
O("    int uc;")
O("    if (c > 255) return 0x3f;")
O("    uc = ((c >= 0) ? c : c + 256);")
O("    return local_to_ia5_array[uc];")
O("}")
O("")
O("unsigned char local_to_iso646(char c) {")
O("    int uc;")
O("    if (c > 255) return 0x3f;")
O("    uc = ((c >= 0) ? c : c + 256);")
O("    return local_to_iso646_array[uc];")
O("}")
O("")
O("#endif")

    fclose(out);
    return EXIT_SUCCESS;
}
