/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: example.c,v $
 * Revision 1.1.11.2  1996/02/18  00:24:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:43  marty]
 *
 * Revision 1.1.11.1  1995/12/08  22:12:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:19  root]
 * 
 * Revision 1.1.9.2  1994/10/05  16:44:20  keutel
 * 	OT 12431
 * 	[1994/10/05  16:36:46  keutel]
 * 
 * Revision 1.1.9.1  1994/02/22  16:40:06  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:34:11  marrek]
 * 
 * Revision 1.1.7.2  1993/08/12  12:49:40  marrek
 * 	July 1993 code drop.
 * 	[1993/08/04  11:00:14  marrek]
 * 
 * Revision 1.1.5.3  1993/02/04  08:43:22  marrek
 * 	Changes for bug 7060
 * 	[1993/02/02  17:06:53  marrek]
 * 
 * Revision 1.1.5.2  1993/01/05  20:00:27  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:17:49  htf]
 * 
 * Revision 1.1.2.3  1992/07/28  20:12:48  melman
 * 	GDS drop of 7/17/92
 * 	[1992/07/28  19:47:00  melman]
 * 
 * Revision 1.1.2.2  1992/06/11  19:59:59  melman
 * 	 gds drop
 * 	[1992/06/11  19:22:50  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: example.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 00:24:20 $";
#endif

/*
 *	sample application that uses XDS in synchronous mode
 *
 *	This program reads the telephone number(s) of a given target name.
 */

#ifdef THREADSAFE
#	include <pthread.h>
#endif
#include <stdio.h>

#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>

#include "example.h"		/* possible Error Handling header */


/* Step 1
 *
 * Define necessary Object Identifier constants
 */
OM_EXPORT(DS_A_COMMON_NAME)
OM_EXPORT(DS_A_COUNTRY_NAME)
OM_EXPORT(DS_A_ORG_NAME)
OM_EXPORT(DS_A_ORG_UNIT_NAME)
OM_EXPORT(DS_A_PHONE_NBR)
OM_EXPORT(DS_C_AVA)
OM_EXPORT(DS_C_DS_DN)
OM_EXPORT(DS_C_DS_RDN)
OM_EXPORT(DS_C_ENTRY_INFO_SELECTION)


/* Step 2 */

int main(void)
{
	DS_status          error;	/* return value from DS functions   */
	OM_return_code     return_code;	/* return value from OM functions   */
	OM_workspace       workspace;	/* workspace for objects	    */
	OM_private_object  session;	/* session for directory operations */
	OM_private_object  result;	/* result of read operation	    */
	OM_sint            invoke_id;	/* Invoke-ID of the read operation  */
	OM_value_position  total_num;	/* Number of Attribute Descriptors  */


    static DS_feature	bdcp_package[] = {
	{ OM_STRING(OMP_O_DS_BASIC_DIR_CONTENTS_PKG), OM_TRUE },
	{ { (OM_uint32)0, (void *)0 }, OM_FALSE },
	};

/* Step 3
 *
 * Public Object ("Descriptor List") for Name argument to ds_read().
 * Build the Distinguished-Name of Peter Piper.
 */

    static OM_descriptor	country[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
	{ DS_ATTRIBUTE_VALUES,OM_S_PRINTABLE_STRING,OM_STRING("US") },
	OM_NULL_DESCRIPTOR
	};
    static OM_descriptor	organization[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_NAME),
	{ DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,
	OM_STRING("Acme Pepper Co") },
	OM_NULL_DESCRIPTOR
	};
    static OM_descriptor	organizational_unit[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_UNIT_NAME),
	{ DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Research") },
	OM_NULL_DESCRIPTOR
	};
    static OM_descriptor	common_name[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
	{ DS_ATTRIBUTE_VALUES,OM_S_TELETEX_STRING,OM_STRING("Peter Piper") },
	OM_NULL_DESCRIPTOR
	};

    static OM_descriptor	rdn1[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
	{ DS_AVAS, OM_S_OBJECT, { 0, country } },
	OM_NULL_DESCRIPTOR
	};
    static OM_descriptor	rdn2[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
	{ DS_AVAS, OM_S_OBJECT, { 0, organization } },
	OM_NULL_DESCRIPTOR
	};
    static OM_descriptor	rdn3[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
	{ DS_AVAS, OM_S_OBJECT, { 0, organizational_unit } },
	OM_NULL_DESCRIPTOR
	};
    static OM_descriptor	rdn4[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
	{ DS_AVAS, OM_S_OBJECT, { 0, common_name } },
	OM_NULL_DESCRIPTOR
	};

    OM_descriptor	name[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
	{ DS_RDNS, OM_S_OBJECT, { 0, rdn1 } },
	{ DS_RDNS, OM_S_OBJECT, { 0, rdn2 } },
	{ DS_RDNS, OM_S_OBJECT, { 0, rdn3 } },
	{ DS_RDNS, OM_S_OBJECT, { 0, rdn4 } },
	OM_NULL_DESCRIPTOR
	};


/* Step 4 */

	/*
	 *
	 * Public Object ("Descriptor List") for
	 * for Entry-Information-Selection argument to ds_read().
	 */
    OM_descriptor selection[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
	{ DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, { OM_FALSE, NULL } },
	OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_PHONE_NBR),
	{ DS_INFO_TYPE,OM_S_ENUMERATION, { DS_TYPES_AND_VALUES,NULL } },
	OM_NULL_DESCRIPTOR
	};

/* Step 5 */

	/*
	 * variables to extract the telephone number(s)
	 */
	OM_type			entry_list[]      = { DS_ENTRY, 0 };
	OM_type			attributes_list[] = { DS_ATTRIBUTES, 0 };
	OM_type			telephone_list[]  = { DS_ATTRIBUTE_VALUES, 0 };
	OM_public_object	entry;
	OM_public_object	attributes;
	OM_public_object	telephones;
	OM_descriptor	        *telephone;	/* current phone number  */

	/*
	 * Perform the Directory operations:
	 * (1) Initialize the Directory Service and get an OM workspace
	 * (2) bind a default directory session.
	 * (3) read the telephone number of "name".
	 * (4) terminate the directory session.
	 */

/* Step 6 */
	CHECK_DS_CALL((OM_object) !(workspace=ds_initialize()));

/* Step 7 */
	CHECK_DS_CALL(ds_version(bdcp_package, workspace));

/* Step 8 */
	CHECK_DS_CALL(ds_bind(DS_DEFAULT_SESSION, workspace, &session));

/* Step 9 */
	CHECK_DS_CALL(ds_read(session, DS_DEFAULT_CONTEXT, name, selection,
			      &result, &invoke_id));
	/*
	 * NOTE: should check here for Attribute-Error (no-such-attribute)
	 * in case the "name" doesn't have a telephone.
	 * Then for all other cases call error_handler
	 */

/* Step 10 */
	CHECK_DS_CALL(ds_unbind(session));

/* Step 11 */

	/*
	 * extract the telephone number(s) of "name" from the result
	 *
	 * There are 4 stages:
	 * (1) get the Entry-Information from the Read-Result.
	 * (2) get the Attributes from the Entry-Information.
	 * (3) get the list of phone numbers.
	 * (4) scan the list and print each number.
	 */

	CHECK_OM_CALL(   om_get(result,
				OM_EXCLUDE_ALL_BUT_THESE_TYPES
			      + OM_EXCLUDE_SUBOBJECTS,
				entry_list, OM_FALSE, 0, 0, &entry,
				&total_num));

	CHECK_OM_CALL(   om_get(entry->value.object.object,
				OM_EXCLUDE_ALL_BUT_THESE_TYPES
			      + OM_EXCLUDE_SUBOBJECTS,
				attributes_list, OM_FALSE, 0, 0, 
				&attributes, &total_num));

	CHECK_OM_CALL(   om_get(attributes->value.object.object,
				OM_EXCLUDE_ALL_BUT_THESE_TYPES
			      + OM_EXCLUDE_SUBOBJECTS,
				telephone_list, OM_FALSE, 0, 0,
				&telephones, &total_num));

/* Step 12 */

	/*  We can now safely release all the private objects
	 *  and the public objects we no longer need
	 */
	CHECK_OM_CALL(om_delete(session));
	CHECK_OM_CALL(om_delete(result));
	CHECK_OM_CALL(om_delete(entry));
	CHECK_OM_CALL(om_delete(attributes));
	CHECK_DS_CALL(ds_shutdown(workspace));

/* Step 13 */

	for (telephone = telephones;
	     telephone->type == DS_ATTRIBUTE_VALUES;
	     telephone++)
	{
		if (telephone->type   != DS_ATTRIBUTE_VALUES
		||  (telephone->syntax & OM_S_SYNTAX) != OM_S_PRINTABLE_STRING)
		{
			(void) fprintf(stderr, "malformed telephone number\n");
			exit(EXIT_FAILURE);
		}

		(void) printf("Telephone number: %.*s\n",
			      telephone->value.string.length,
			      telephone->value.string.elements);
	}

/* Step 14 */

	CHECK_OM_CALL(om_delete(telephones));

/* Step 15 */

	/*  more application-specific processing can occur here...
	 */

	/* ... and finally exit. */
	exit(EXIT_SUCCESS);
}
