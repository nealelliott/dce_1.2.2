/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: teldir.c,v $
 * Revision 1.1.13.2  1996/02/18  00:24:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:49  marty]
 *
 * Revision 1.1.13.1  1995/12/08  22:12:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:31  root]
 * 
 * Revision 1.1.11.2  1994/10/05  16:44:26  keutel
 * 	OT 12431
 * 	[1994/10/05  16:37:03  keutel]
 * 
 * Revision 1.1.11.1  1994/02/22  16:40:29  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:35:31  marrek]
 * 
 * Revision 1.1.9.1  1993/10/14  19:28:31  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:26:49  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  12:42:44  keutel]
 * 
 * Revision 1.1.7.2  1993/08/12  12:53:07  marrek
 * 	July 1993 code drop.
 * 	[1993/08/04  11:00:39  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  09:01:00  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  08:15:33  marrek
 * 	Replace OM_S_OBJECT by OM_S_OBJECT_IDENTIFIER_STRING in
 * 	FILL_OMD_XOM_STRING()
 * 	[1993/02/01  14:20:27  marrek]
 * 
 * Revision 1.1.4.3  1993/01/05  20:01:07  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:18:23  htf]
 * 
 * Revision 1.1.4.2  1992/12/01  14:58:32  marrek
 * 	November 1992 code drop
 * 	[1992/12/01  13:17:43  marrek]
 * 
 * Revision 1.1.2.2  1992/06/11  17:43:14  melman
 * 	New GDS test code drop
 * 	[1992/06/11  17:39:45  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: teldir.c,v $ $Revision: 1.1.13.2 $ $Date: 1996/02/18 00:24:27 $";
#endif

/*
 * This sample program behaves like a simple telephone directory.
 * It permits a user to add, read or delete entries in a GDS
 * namespace or to a CDS namespace in any local or remote DCE cell
 * (assuming that permissions are granted by the ACLs).
 *
 * Each entry is of class Organizational-Person and simply contains 
 * a person's surname and their phone number.
 *
 * The addition of an entry is followed by a read to verify that the
 * information was entered properly.
 *
 * All valid names should begin with one of the following symbols:
 *     /...		Fully qualified name (from global root).
 *                      e.g.  /.../C=de/O=sni/OU=ap/CN=klaus
 *
 *     /.:		Partially qualified name (from local cell root).
 *                      e.g.  /.:/brad/sni/com
 *
 * This program demonstrates the following techniques:
 * - Using completely static XDS public objects (pre-defined at the top
 *   of the program and never altered).  See xdsObjectClass,
 *   xdsAttributesToAdd, and xdsAttributeSelection below.
 * - Using partially static XDS public objects (pre-defined at the top
 *   of the program but altered later).  See xdsSurname and xdsPhoneNum
 *   below.  See also the macros whose names begin with "FILL_OMD_".
 * - Using dynamic XDS public objects (created and filled in only as
 *   needed).  See the function stringToXdsName() below.
 * - Parsing DCE-style names and converting them into XDS objects.  See
 *   the function stringToXdsName() below.
 * - Getting the value of an attribute from an object read from the
 *   namespace using ds_read().  See the function extractValue() below.
 * - Getting the numeric value of an error (type DS_status) returned by
 *   one of the XDS calls.  See the function handleDSError() below.
 */

#ifdef THREADSAFE
#	include <pthread.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <xdsgds.h>
#include <xdscds.h>

OM_EXPORT( DS_A_COMMON_NAME )
OM_EXPORT( DS_A_COUNTRY_NAME )
OM_EXPORT( DS_A_LOCALITY_NAME )
OM_EXPORT( DS_A_OBJECT_CLASS )
OM_EXPORT( DS_A_ORG_UNIT_NAME )
OM_EXPORT( DS_A_ORG_NAME )
OM_EXPORT( DS_A_SURNAME )
OM_EXPORT( DS_A_PHONE_NBR )
OM_EXPORT( DS_A_TITLE )
OM_EXPORT( DS_C_ATTRIBUTE )
OM_EXPORT( DS_C_ATTRIBUTE_LIST )
OM_EXPORT( DS_C_AVA )
OM_EXPORT( DS_C_DS_DN )
OM_EXPORT( DS_C_DS_RDN )
OM_EXPORT( DS_C_ENTRY_INFO_SELECTION )
OM_EXPORT( DS_O_ORG_PERSON )
OM_EXPORT( DS_O_PERSON )
OM_EXPORT( DS_O_TOP )
OM_EXPORT( DSX_TYPELESS_RDN )	/* For "typeless" pieces of a name, as  */
                                /* found in cells with bind-style names */
                                /* and in the CDS namespace.            */

#define	MAX_NAME_LEN	1024

/* These values can be found in the Chapter "Directory Class Definitions". */
/* (One byte must be added for the null terminator.)                       */
#define MAX_PHONE_LEN	33
#define MAX_SURNAME_LEN	66

/*****************************************************************************
 * Macros for help filling in static XDS objects.
 *****************************************************************************/
/* Put NULL value (equivalent to OM_NULL_DESCRIPTOR) in object */
#define FILL_OMD_NULL( desc, index ) \
	desc[index].type = OM_NO_MORE_TYPES; \
	desc[index].syntax = OM_S_NO_MORE_SYNTAXES; \
	desc[index].value.object.padding = 0; \
	desc[index].value.object.object = OM_ELEMENTS_UNSPECIFIED;

/* Put C-style (null-terminated) string in object */
#define FILL_OMD_STRING( desc, index, typ, syntx, val ) \
	desc[index].type = typ; \
	desc[index].syntax = syntx; \
	desc[index].value.string.length = (OM_string_length)strlen( val ); \
	desc[index].value.string.elements = val;

/* Put XOM string in object */
#define FILL_OMD_XOM_STRING( desc, index, typ, syntx, val ) \
	desc[index].type = typ; \
	desc[index].syntax = syntx; \
	desc[index].value.string.length = val.length; \
	desc[index].value.string.elements = val.elements;

/* Put other value in object */
#define FILL_OMD_STRUCT( desc, index, typ, syntx, val ) \
	desc[index].type = typ; \
	desc[index].syntax = syntx; \
	desc[index].value.object.padding = 0; \
        desc[index].value.object.object = val;


/*****************************************************************************
 * Static XDS objects.
 *****************************************************************************/
/*
 * To identify which packages we need for this program.  We only need
 * the basic package because we are not doing anything fancy with
 * session parameters, etc.
 */
DS_feature featureList[] = {
        { OM_STRING(OMP_O_DS_BASIC_DIR_CONTENTS_PKG), OM_TRUE },
        { 0 }
};

/*
 * To hold the attributes we want to attach to the name being added.
 * One attribute is the class of the object (DS_O_ORG_PERSON), the
 * rest of the attributes are the surname (required for all objects
 * of class DS_O_ORG_PERSON) and phone number.  In addition, we need
 * an object to hold all this information to pass it into ds_add_entry().
 */
static OM_descriptor xdsObjectClass[] = {

    /* This object is an attribute--an object class. */
    OM_OID_DESC( OM_CLASS,            DS_C_ATTRIBUTE ),
    OM_OID_DESC( DS_ATTRIBUTE_TYPE,   DS_A_OBJECT_CLASS ),

    /* Not only must the class be listed, but also all */
    /* its superclasses.                               */
    OM_OID_DESC( DS_ATTRIBUTE_VALUES, DS_O_TOP ),
    OM_OID_DESC( DS_ATTRIBUTE_VALUES, DS_O_PERSON ),
    OM_OID_DESC( DS_ATTRIBUTE_VALUES, DS_O_ORG_PERSON ),
    
    /* Null terminator */
    OM_NULL_DESCRIPTOR
};

static OM_descriptor xdsSurname[] = {

    /* This object is an attribute--a surname. */
    OM_OID_DESC( OM_CLASS,          DS_C_ATTRIBUTE ),
    OM_OID_DESC( DS_ATTRIBUTE_TYPE, DS_A_SURNAME ),

    /* No default--so we need a place holder for the actual surname. */
    OM_NULL_DESCRIPTOR,

    /* Null terminator */
    OM_NULL_DESCRIPTOR
};

static OM_descriptor xdsPhoneNum[] = {

    /* This object is an attribute--a telephone number. */
    OM_OID_DESC( OM_CLASS,          DS_C_ATTRIBUTE ),
    OM_OID_DESC( DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR ),

    /* By default, phone numbers are unlisted.  If the user specifies */
    /* an actual phone number, it will go into this position.         */
    { DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING( "unlisted" ) },

    /* Null terminator */
    OM_NULL_DESCRIPTOR
};

static OM_descriptor xdsAttributesToAdd[] = {

    /* This object is an attribute list. */
    OM_OID_DESC( OM_CLASS, DS_C_ATTRIBUTE_LIST ),

    /* These are "pointers" to the attributes in the list. */
    { DS_ATTRIBUTES, OM_S_OBJECT, { 0, xdsObjectClass } },
    { DS_ATTRIBUTES, OM_S_OBJECT, { 0, xdsSurname } },
    { DS_ATTRIBUTES, OM_S_OBJECT, { 0, xdsPhoneNum } },

    /* Null terminator */
    OM_NULL_DESCRIPTOR
};

/*
 * To hold the list of attributes we want to read.
 */
static OM_descriptor xdsAttributeSelection[] = {

    /* This is an entry information selection. */
    OM_OID_DESC( OM_CLASS, DS_C_ENTRY_INFO_SELECTION ),

    /* No, we don't want all attributes. */
    { DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE },

    /* These are the ones we want to read. */
    OM_OID_DESC( DS_ATTRIBUTES_SELECTED, DS_A_SURNAME ),
    OM_OID_DESC( DS_ATTRIBUTES_SELECTED, DS_A_PHONE_NBR ),

    /* Give us both the types and their values. */
    { DS_INFO_TYPE, OM_S_ENUMERATION, { DS_TYPES_AND_VALUES, NULL } },

    /* Null terminator */
    OM_NULL_DESCRIPTOR
};


/*****************************************************************************
 * dce_cf_get_cell_name()
 *	Use this dummy function if CDS is not available.
 *****************************************************************************/
void 
dce_cf_get_cell_name(
    char **		cellname,
    unsigned long *	status
)
{
    fprintf( stderr, "CDS unavailable: dce_cf_get_cell_name() error\n" );
    *status = 1;

} /* end dce_cf_get_cell_name() */


/*****************************************************************************
 * showUsage()
 *	Display "usage" information.
 *****************************************************************************/
void 
showUsage(
    char *	cmd     	/* In--Name of command being called */
)
{
    fprintf( stderr, "\nusage:  %s [option]\n\n", cmd );
    fprintf( stderr, "option:  -a : add an entry\n" );
    fprintf( stderr, "         -r : read an entry\n" );
    fprintf( stderr, "         -d : delete an entry\n" );

} /* end showUsage() */


/*****************************************************************************
 * numNamePieces()
 *	Returns the number of pieces in a string name.
 ****************************************************************************/
int
numNamePieces(
    char *	string		/* In--String whose pieces are to be counted */
)
{
    int		count;		/* Number of pieces found */
    char *	currSep;	/* Pointer to separator between pieces */

    if( string == NULL )	/* If nothing there, no pieces */
	return( 0 );
    count = 1;			/* Otherwise, there's at least one */

    /*
     * If the first character is a /, it's not really separating
     * two pieces so we want to ignore it here.
     */
    if( *string == '/' )
	currSep = string + 1;
    else
	currSep = string;

    /* How many pieces are there? */
    while( (currSep = strchr( currSep, '/' )) != NULL ) {
	count++;
	currSep++;	/* Begin at next character */
    }

    return( count );

} /* end numNamePieces() */


/*****************************************************************************
 * splitNamePiece()
 *	Divides a piece of a name (string) into its XDS attribute type
 *	and value.
 ****************************************************************************/
void
splitNamePiece(
    char *	string,	/* In--String to be broken down */
    OM_string *	type,	/* Out--XDS type of this piece of the name */
    char **	value	/* Out--Pointer to beginning of the value part */
)                       /*      of string                              */
{
    char *	equalSign;	/* Location of the = within string */
 
    /*
     * If the string contains an equal sign, this is probably a
     * typed name.  Check for all the attribute types allowed by
     * the default schema.
     */
    if( (equalSign = strchr( string, '=' )) != NULL ) {

	*value = equalSign + 1;
	
	if(( strncmp( string, "C=", 2 ) == 0 ) ||
	   ( strncmp( string, "c=", 2 ) == 0 ))
	    *type = DS_A_COUNTRY_NAME;

	else if(( strncmp( string, "O=", 2 ) == 0 ) ||
                ( strncmp( string, "o=", 2 ) == 0 ))
	    *type = DS_A_ORG_NAME;

	else if(( strncmp( string, "OU=", 3 ) == 0 ) ||
	        ( strncmp( string, "ou=", 3 ) == 0 ))
	    *type = DS_A_ORG_UNIT_NAME;

	else if(( strncmp( string, "LN=", 3 ) == 0 ) ||
	        ( strncmp( string, "ln=", 3 ) == 0 ))
	    *type = DS_A_LOCALITY_NAME;

	else if(( strncmp( string, "CN=", 3 ) == 0 ) ||
	        ( strncmp( string, "cn=", 3 ) == 0 ))
	    *type = DS_A_COMMON_NAME;

	/*
	 * If this did not appear to be a type allowed by the
	 * default schema, consider the whole string as the
	 * value (whose type is "typeless").
	 */
	else {
	    *type = DSX_TYPELESS_RDN;
	    *value = string;
	}
    }

    /*
     * If the string does not contain an equal sign, this is a
     * typeless name.
     */
    else {
	*type = DSX_TYPELESS_RDN;
	*value = string;
    }

} /* end splitNamePiece() */


/*****************************************************************************
 * extractValue()
 *	Pulls the value of a particular attribute from a private object that
 *	was received using ds_read().
 *	Returns:
 *		OM_SUCCESS		If successful.
 *		OM_NO_SUCH_OBJECT	If no values for the attribute
 *					were found.
 *		other			Any value returned by one of the
 *					om_get() calls.
 *****************************************************************************/
OM_return_code
extractValue(
    OM_private_object	object,		/* In--Object to extract from */
    OM_string *		attribute,	/* In--Attribute to extract */
    char *		value		/* Out--Value found */
)
{
    OM_public_object	attrList;
    OM_public_object	attrType;
    OM_public_object	attrValue;
    OM_public_object	entry;
    int			i;
    OM_return_code	omStatus;
    OM_value_position	total;
    OM_value_position	totalAttributes;
    OM_type		xdsIncludedTypes[] = { 0,	/* Place holder */
					       0 };	/* Null terminator */

    /*
     * Get the entry from the object returned by ds_read().
     */
    xdsIncludedTypes[0] = DS_ENTRY;
    omStatus = om_get( object,			/* Object to extract from */
		       OM_EXCLUDE_ALL_BUT_THESE_TYPES+OM_EXCLUDE_SUBOBJECTS,
		                                /* Only want what is in    */
		                                /* xdsIncludedTypes, don't */
		                                /* include subobjects      */
		       xdsIncludedTypes,	/* What to get */
		       OM_FALSE,		/* Currently ignored */
		       OM_ALL_VALUES,		/* Start with first value */
		       OM_ALL_VALUES,		/* End with last value */
		       &entry,			/* Put the entry here */
		       &total );		/* Put number of attribute */
                                                /* descriptors here        */
    if( omStatus != OM_SUCCESS ) {
	fprintf( stderr, "om_get( entry ) returned error %d\n", omStatus );
	return( omStatus );
    }
    if( total <= 0 ) {		/* Make sure something was returned */
	fprintf( stderr,
		 "Number of descriptors returned by om_get( entry ) was %d\n",
		 total );
	return( OM_NO_SUCH_OBJECT );
    }

    /*
     * Get the attribute list from the entry.
     */
    xdsIncludedTypes[0] = DS_ATTRIBUTES;
    omStatus = om_get( entry->value.object.object,
		       OM_EXCLUDE_ALL_BUT_THESE_TYPES+OM_EXCLUDE_SUBOBJECTS,
		       xdsIncludedTypes, OM_FALSE, OM_ALL_VALUES,
		       OM_ALL_VALUES, &attrList, &totalAttributes );
    if( omStatus != OM_SUCCESS ) {
	fprintf( stderr, "om_get( attrList ) returned error %d\n", omStatus );
	return( omStatus );
    }
    if( total <= 0 ) {		/* Make sure something was returned */
	fprintf( stderr,
		 "Number of descriptors returned by om_get( attrList ) was %d\n",
		 total );
	return( OM_NO_SUCH_OBJECT );
    }

    /*
     * Search the list for the attribute with the proper type.
     */
    for( i = 0; i < totalAttributes; i++ ) {
        xdsIncludedTypes[0] = DS_ATTRIBUTE_TYPE;
	omStatus = om_get( (attrList+i)->value.object.object,
			   OM_EXCLUDE_ALL_BUT_THESE_TYPES+OM_EXCLUDE_SUBOBJECTS,
			   xdsIncludedTypes, OM_FALSE, OM_ALL_VALUES,
			   OM_ALL_VALUES, &attrType, &total );
	if( omStatus != OM_SUCCESS ) {
	    fprintf( stderr, "om_get( attrType ) [i = %d] returned error %d\n",
		     i, omStatus );
	    return( omStatus );
	}
	if( total <= 0 ) {		/* Make sure something was returned */
	    fprintf( stderr,
	    "Number of descriptors returned by om_get( attrType ) [i = %d] was %d\n",
		     i, total );
	    return( OM_NO_SUCH_OBJECT );
	}
	if( strncmp( attrType->value.string.elements, attribute->elements,
		     attribute->length ) == 0 )
	    break;		/* If we found a match, quit looking. */
    }
    if( i == totalAttributes ) {	/* Verify that we found a match. */
	fprintf( stderr, 
		 "%s: extractValue() could not find requested attribute\n" );
	return( OM_NOT_PRESENT );
    }

    /*
     * Get the attribute value from the corresponding item in the
     * attribute list.
     */
    xdsIncludedTypes[0] = DS_ATTRIBUTE_VALUES;
    omStatus = om_get( (attrList+i)->value.object.object,
		       OM_EXCLUDE_ALL_BUT_THESE_TYPES+OM_EXCLUDE_SUBOBJECTS,
		       xdsIncludedTypes, OM_FALSE, OM_ALL_VALUES,
		       OM_ALL_VALUES, &attrValue, &total );
    if( omStatus != OM_SUCCESS ) {
	fprintf( stderr, "om_get( attrValue ) returned error %d\n",
		 omStatus );
	return( omStatus );
    }
    if( total <= 0 ) {		/* Make sure something was returned */
	fprintf( stderr,
		 "Number of descriptors returned by om_get( attrValue ) was %d\n",
		 total );
	return( OM_NO_SUCH_OBJECT );
    }

    /*
     * Copy the value into the buffer for return to the caller.
     */
    strncpy( value, attrValue->value.string.elements,
	     attrValue->value.string.length );
    value[attrValue->value.string.length] = '\0';

    /*
     * Free up the resources we don't need any more and return.
     */
    om_delete( attrValue );
    om_delete( attrType );
    om_delete( attrList );
    om_delete( entry );
    return( OM_SUCCESS );

} /* end extractValue() */


/*****************************************************************************
 * stringToXdsName()
 *	Converts a string that is a DCE name to an XDS name object (class 
 *	DS_C_DS_DN).  Returns one of the following:
 *		OM_SUCCESS		If successful.
 *		OM_MEMORY_INSUFFICIENT	If a malloc fails.
 *		OM_PERMANENT_ERROR	If the name is not in a valid format.
 *		OM_SYSTEM_ERROR		If the local cell's name cannot be
 *					determined.
 *
 *	Technically, the space obtained here through malloc() needs
 *	to be returned to the system when it is no longer needed.
 *	If this was a more complex application, this function would
 *	probably malloc all the space it needs at once and require
 *	calling routines to free the space when finished with it.
 ****************************************************************************/
OM_return_code
stringToXdsName(
    char *	origString,	/* In--String name to be converted */
    OM_object *	xdsNameObj	/* Out--Pointer to XDS name object */
)
{
    OM_descriptor *	ava;		/* DS_C_AVA object */
    char *		cellName;       /* Name of this cell */
    OM_object		dsdn;		/* DS_C_DS_DN object */
    char *		end;		/* End of name piece */
    int			index;		/* Index into DS_C_DS_DN object */
    int			numberOfPieces;	/* Number of pieces in the name */
    unsigned long	rc;		/* Return code for some functions */
    OM_descriptor *	rdn;		/* DS_C_RDN object */
    char *		start;		/* Beginning of piece of name */
    char *		string;		/* Copy of origString that we can use */
    OM_string		type;		/* Type of one piece of the name */
    char *		value;		/* Piece of the name */

    /*
     * A DS_C_AVA object only contains pointers to the strings that
     * represent the pieces of the name, not the contents of the strings
     * themselves.  So we'll make a copy of the string passed in to
     * guarantee that these pieces survive in case the programmer alters
     * or reuses the original string.
     *
     * In addition, all valid names should begin with one of the
     * following symbols:
     *     /...		Fully qualified name (from global root).  For
     *			these, we need to ignore the /...
     *     /.:		Partially qualified name (from local cell root).
     *			For these, we must replace the /.: with the name
     *			of the local cell name
     * If we see anything else, we'll return with an error.  (Notice that
     * /: is a valid DCE name, but refers to the file system's namespace.
     * Filenames cannot be accessed through CDS, GDS, or XDS.)
     */
    if( strncmp( origString, "/.../", 5 ) == 0 ) {
	string = (char *)malloc( strlen(origString+5) + 1 );
	if( string == NULL )			/* malloc() failed */
	    return OM_MEMORY_INSUFFICIENT;
	strcpy( string, origString+5 );
    }
    else if( strncmp( origString, "/.:/", 4 ) == 0 ) {
	dce_cf_get_cell_name( &cellName, &rc );
	if( rc != 0 )				/* Could not get cell name */
	    return OM_SYSTEM_ERROR;
	
	/*
	 * The cell name will have /.../ on the front, so we will skip
	 * over it as we add it to the string (by always starting at its
	 * fifth character).
	 */
	string = (char *)malloc( strlen(origString+4) + strlen(cellName+5) + 2 );
	if( string == NULL )			/* malloc() failed */
	    return OM_MEMORY_INSUFFICIENT;
	strcpy( string, cellName+5 );
	strcat( string, "/" );
	strcat( string, origString+4 );
    }
    else					/* Invalid name format */
	return OM_PERMANENT_ERROR;

    /*
     * Count the number of pieces in the name that will have to be dealt with.
     */
    numberOfPieces = numNamePieces( string );

    /*
     * Allocate memory for the DS_C_DS_DN object.  We will need an
     * OM_descriptor for each name piece, one for the class identifier,
     * and one for the null terminator.
     */
    dsdn = (OM_object)malloc( (numberOfPieces + 2) * sizeof(OM_descriptor) );
    if( dsdn == NULL )				/* malloc() failed */
	return OM_MEMORY_INSUFFICIENT;

    /*
     * Initialize it as a DS_C_DS_DN object by placing that class
     * identifier in the first position.
     */
    FILL_OMD_XOM_STRING( dsdn, 0, OM_CLASS, OM_S_OBJECT_IDENTIFIER_STRING, DS_C_DS_DN )

    /*
     * For each piece of string, do the following:
     *     Break off the next piece of the string
     *     Build a DS_C_AVA object to show the type and value
     *         of this piece of the name
     *     Wrap the DS_C_AVA up in a DS_C_RDN object
     *     Add the DS_C_RDN to the DS_C_DS_DN object
     */
    for( start=string, index=1 ; index <= numberOfPieces ; index++, start=end+1 ) {

	/*
	 * Find the next delimiter and replace it with a null byte
	 * so the piece of the name is effectively separated from
	 * the rest of the string.
	 */
	end = strchr( start, '/' );
	if( end != NULL )
	    *end = '\0';
	else		/* If this is the last piece, there won't be */
	                /* a '/' at the end, just a null byte.       */
	    end = strchr( start, '\0' );

	/*
	 * Allocate space for a DS_C_AVA object and fill in its entries:
	 *	DS_C_AVA class identifier
	 *	AVA's type
	 *	AVA's value
	 *	null terminator
	 */
	ava = (OM_descriptor *)malloc( sizeof(OM_descriptor) * 4 );
	if( ava == NULL )			/* malloc() failed */
	    return OM_MEMORY_INSUFFICIENT;
	FILL_OMD_XOM_STRING( ava, 0, OM_CLASS, OM_S_OBJECT_IDENTIFIER_STRING, 
			     DS_C_AVA )
	splitNamePiece( start, &type, &value );
	FILL_OMD_XOM_STRING( ava, 1, DS_ATTRIBUTE_TYPE, OM_S_OBJECT_IDENTIFIER_STRING,
			     type )
	FILL_OMD_STRING( ava, 2, DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING,
			 value )
	FILL_OMD_NULL( ava, 3 )

	/*
	 * Allocate space for a DS_C_RDN object and fill in its entries:
	 *	DS_C_RDN class identifier
	 *	AVA it contains
	 *	null terminator
	 */
	rdn = (OM_descriptor *)malloc( sizeof(OM_descriptor) * 3 );
	if( rdn == NULL )			/* malloc() failed */
	    return OM_MEMORY_INSUFFICIENT;
	FILL_OMD_XOM_STRING( rdn, 0, OM_CLASS, OM_S_OBJECT_IDENTIFIER_STRING, DS_C_DS_RDN )
	FILL_OMD_STRUCT( rdn, 1, DS_AVAS, OM_S_OBJECT, ava )
	FILL_OMD_NULL( rdn, 2 )

	/* Add the DS_C_RDN object to the DS_C_DS_DN object. */
	FILL_OMD_STRUCT( dsdn, index, DS_RDNS, OM_S_OBJECT, rdn )
    }

    /*
     * Null terminate the DS_C_DS_DN, tell the calling routine
     * where to find it, and return.
     */
    FILL_OMD_NULL( dsdn, index )
    *xdsNameObj = dsdn;
    return( OM_SUCCESS );

} /* end stringToXdsName() */


/*****************************************************************************
 * handleDSError()
 *	Extracts the error number from a DS_status return code, prints it
 *	in an error message, then terminates the program.
 *****************************************************************************/
void 
handleDSError(
    char *	header,    	/* In--Name of function whose return code */
                                /*     is being checked                   */
    DS_status	returnCode	/* In--Return code to be checked */
)
{
    OM_type		includeDSProblem[] = { DS_PROBLEM,
					       0 };
    OM_return_code      omStatus;
    OM_public_object	problem;
    OM_value_position	total;

    /*
     * A DS_status return code is an object.  It will be one of the
     * subclasses of the class DS_C_ERROR.  What we want from it is
     * the value of the attribute DS_PROBLEM.
     */
    omStatus = om_get( returnCode,
		       OM_EXCLUDE_ALL_BUT_THESE_TYPES+OM_EXCLUDE_SUBOBJECTS,
		       includeDSProblem,
		       OM_FALSE,
		       OM_ALL_VALUES,
		       OM_ALL_VALUES,
		       &problem,
		       &total );

    /*
     * Make sure we successfully extracted the problem number and print
     * the error message before quitting.
     */
    if( (omStatus == OM_SUCCESS) && (total > 0) )
	printf( "%s returned error %d\n", header, problem->value.enumeration );
    else
	printf( "%s failed for unknown reason\n", header );

    exit( 1 );
}


/*****************************************************************************
 * Main program
 */
void
main(
    int		argc,
    char *	argv[]
)
{
    DS_status		dsStatus;
    OM_sint		invokeID;
    char		newName[MAX_NAME_LEN];
    char		newPhoneNum[MAX_PHONE_LEN];
    char		newSurname[MAX_SURNAME_LEN];
    OM_return_code	omStatus;
    char		phoneNumRead[MAX_PHONE_LEN];
    int			rc = 0;
    OM_private_object	readResult;
    OM_private_object	session;
    char		surnameRead[MAX_SURNAME_LEN];
    OM_object		xdsName;
    OM_workspace	xdsWorkspace;
    int			operation;

    /* Step 1
     *
     * Examine command-line argument.
     */
     operation = getopt( argc, argv, "rad" );
     if ( (operation == '?') || (operation == EOF) ) {
         showUsage( argv[0] );
         exit( 1 );
     }

    /* Step 2
     *
     * Initialize the XDS workspace.
     */
    xdsWorkspace = ds_initialize( );
    if( xdsWorkspace == NULL ) {
	fprintf( stderr, "ds_initialize() failed\n" );
	exit( 1 );
    }

    /* Step 3
     *
     * Pull in the packages that contain the XDS features we need.
     */
    dsStatus = ds_version( featureList, xdsWorkspace );
    if( dsStatus != DS_SUCCESS )
	handleDSError( "ds_version()", dsStatus );


    /* Step 4
     *
     * Find out what name the user wants to use in the namespace and
     * convert it to and XDS object.  We do this conversion dynamically
     * (not using static structures defined at the top of the program)
     * because we don't know how long the name will be.
     */
    switch( operation ) {
    case 'r' :
        printf( "What name do you want to read? " );
        break;
    case 'a' :
        printf( "What name do you want to add? " );
        break;
    case 'd' :
        printf( "What name do you want to delete? " );
        break;
    }
    
    /* Step 5 */
    
    gets( newName );
    omStatus = stringToXdsName( newName, &xdsName );
    if( omStatus != OM_SUCCESS ) {
	fprintf( stderr, "stringToXdsName() failed with OM error %d\n",
		 omStatus );
	exit( 1 );
    }


    if ( operation == 'a' ) {   /* add operation requires additional input */
        /*
         * Get the person's real name from the user and place it in the
         * XDS object already defined at the top of the program (xdsSurname).
         * We are requiring a name, so we will loop until we get one.
         */
        do {
	    printf( "What is this person's surname? " );
	    gets( newSurname );
        } while ( *newSurname == '\0' );
        FILL_OMD_STRING( xdsSurname, 2, DS_ATTRIBUTE_VALUES,
		         OM_S_TELETEX_STRING, newSurname )

        /*
         * Get the person's phone number from the user and place it in the
         * XDS object already defined at the top of the program (xdsPhoneNum).
         * A phone number is not required, so if none is given we will use
         * the default already stored in the structure.
         */
        printf( "What is this person's phone number? " );
        gets( newPhoneNum );
        if( *newPhoneNum != '\0' ) {
	    FILL_OMD_STRING( xdsPhoneNum, 2, DS_ATTRIBUTE_VALUES,
			     OM_S_PRINTABLE_STRING, newPhoneNum )
        }
    }

    /* Step 6
     *
     * Open the session with the namespace:
     *  bind (without credentials) to the default server.
     */
    dsStatus = ds_bind( DS_DEFAULT_SESSION, xdsWorkspace, &session );
    if( dsStatus != DS_SUCCESS )
	handleDSError( "ds_bind()", dsStatus );

    /* Step 7 */

    switch( operation ) {   /* perform the requested operation */

    /*
     * Add entry to the namespace. The xdsSurname and xdsPhoneNum
     * objects are already contained within an attribute list object
     * (xdsAttributesToAdd).
     */
    case 'a' :
        dsStatus = ds_add_entry( session, DS_DEFAULT_CONTEXT, xdsName,
			         xdsAttributesToAdd, &invokeID );
        if( dsStatus != DS_SUCCESS )
	    handleDSError( "ds_add_entry()", dsStatus );

        /* FALL THROUGH */

    /*
     * Read the entry of the name supplied.
     */
    case 'r' :
        dsStatus = ds_read( session, DS_DEFAULT_CONTEXT, xdsName,
		            xdsAttributeSelection, &readResult, &invokeID );
        if( dsStatus != DS_SUCCESS )
	    handleDSError( "ds_read()", dsStatus );
		        
        /*
         * Get each attribute from the object read and print them.
         */
        omStatus = extractValue( readResult, &DS_A_SURNAME, surnameRead );
        if( omStatus != OM_SUCCESS ) {
	    printf( "** Surname could not be read\n" );
	    strcpy( surnameRead, "(unknown)" );
	    rc = 1;
        }
        omStatus = extractValue( readResult, &DS_A_PHONE_NBR, phoneNumRead );
        if( omStatus != OM_SUCCESS ) {
	    printf( "** Phone number could not be read\n" );
    	    strcpy( phoneNumRead, "(unknown)" );
	    rc = 1;
        }
        printf( "The phone number for %s is %s.\n", surnameRead, phoneNumRead );

        break;

    /*
     * delete the entry from the namespace.
     */
    case 'd' :
        dsStatus = ds_remove_entry( session, DS_DEFAULT_CONTEXT, xdsName,
			         &invokeID );
        if( dsStatus != DS_SUCCESS )
	    handleDSError( "ds_remove_entry()", dsStatus );
        else
            printf( "The entry has been deleted.\n" );
        break;
    }


    /*
     * Clean up and exit.
     */
    /* Step 8 */
    dsStatus = ds_unbind( session );
    if( dsStatus != DS_SUCCESS )
	handleDSError( "ds_unbind()", dsStatus );

    /* Step 9 */
    dsStatus = ds_shutdown( xdsWorkspace );
    if( dsStatus != DS_SUCCESS )
	handleDSError( "ds_shutdown()", dsStatus );

    exit( rc );

} /* end main() */
