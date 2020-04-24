/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: example_cds.h,v $
 * Revision 1.1.6.2  1996/02/18  23:04:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:43  marty]
 *
 * Revision 1.1.6.1  1995/12/08  22:12:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:23  root]
 * 
 * Revision 1.1.4.2  1994/07/07  07:02:46  marrek
 * 	Add version string in July 1994 code drop.
 * 	[1994/07/07  06:59:09  marrek]
 * 
 * Revision 1.1.4.1  1994/02/22  16:40:16  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:34:37  marrek]
 * 
 * Revision 1.1.2.2  1993/01/05  20:00:42  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:18:01  htf]
 * 
 * Revision 1.1  1992/01/19  14:30:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

#ifndef EXAMPLE_CDS_HEADER
#define EXAMPLE_CDS_HEADER

/* The application has to export the object identfiers it requires.     */

OM_EXPORT (DS_C_AVA)
OM_EXPORT (DS_C_DS_RDN)
OM_EXPORT (DS_C_DS_DN)
OM_EXPORT (DS_C_ENTRY_INFORMATION_SELECTION)
OM_EXPORT (DS_C_ATTRIBUTE)
OM_EXPORT (DS_C_ATTRIBUTE_LIST)
OM_EXPORT (DSX_TYPELESS_RDN)

OM_EXPORT (DS_A_COUNTRY_NAME)
OM_EXPORT (DS_A_ORGANIZATION_NAME)
OM_EXPORT (DS_A_COMMON_NAME)
OM_EXPORT (DS_A_OBJECT_CLASS)
OM_EXPORT (DS_A_TELEPHONE_NUMBER)
OM_EXPORT (DS_A_USER_PASSWORD)
OM_EXPORT (DS_A_SURNAME)

OM_EXPORT (DS_O_TOP)
OM_EXPORT (DS_O_PERSON)
OM_EXPORT (DS_O_ORGANIZATIONAL_PERSON)

/* Build up a descriptor list for the distinguished names:              */
/*     C=IE/O=SNI                                                       */
/*     C=IE/O=SNI/TL=Brendan                                            */
/*     C=IE/O=SNI/TL=Sinead                                             */

static OM_descriptor    countryName[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("IE")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    orgName[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORGANIZATION_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("SNI")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    commonName[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_TYPELESS_RDN),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Brendan")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    commonName2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_TYPELESS_RDN),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Sinead")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, countryName}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, orgName}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, commonName}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, commonName2}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor name1[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor name2[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn3}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor name3[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn4}},
    OM_NULL_DESCRIPTOR
};

/* The following descriptor list specifies what to return from the entry*/
/* All attribute types and values are selected.                         */

static OM_descriptor selection[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFORMATION_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_TRUE},
    {DS_INFORMATION_TYPE, OM_S_ENUMERATION, DS_TYPES_AND_VALUES},
    OM_NULL_DESCRIPTOR
};

/* Build up an array of object identifiers for the optional packages    */
/* to be negotiated.                                                    */

DS_feature features[] = {
    { OM_STRING(OMP_O_DS_BASIC_DIRECTORY_CONTENTS_PACKAGE), OM_TRUE },
    { 0 }
};

/* Build up an array of object identifiers for the attributes to be     */
/* added to the directory.                                              */

static OM_descriptor    object_class[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_PERSON),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORGANIZATIONAL_PERSON),
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    telephone[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_TELEPHONE_NUMBER),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+49 89 636 0")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    surname[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Moloney")},
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    surname2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Murphy")},
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    password[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("secret")},
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    attr_list1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, object_class} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, telephone} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    attr_list2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, object_class} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname2} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, password} },
    OM_NULL_DESCRIPTOR
};
 

OM_workspace workspace;

#endif  /* EXAMPLE_CDS_HEADER */
