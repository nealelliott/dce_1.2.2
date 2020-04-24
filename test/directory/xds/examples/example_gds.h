/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: example_gds.h,v $
 * Revision 1.1.6.2  1996/02/18  23:04:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:44  marty]
 *
 * Revision 1.1.6.1  1995/12/08  22:12:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:25  root]
 * 
 * Revision 1.1.4.3  1994/10/05  16:44:23  keutel
 * 	OT 12431
 * 	[1994/10/05  16:36:53  keutel]
 * 
 * Revision 1.1.4.2  1994/07/07  07:02:48  marrek
 * 	Add version string in July 1994 code drop.
 * 	[1994/07/07  06:59:14  marrek]
 * 
 * Revision 1.1.4.1  1994/02/22  16:40:20  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:34:57  marrek]
 * 
 * Revision 1.1.2.2  1993/01/05  20:00:54  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:18:09  htf]
 * 
 * Revision 1.1  1992/01/19  14:30:22  devrcs
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

#ifndef EXAMPLE_GDS_HEADER
#define EXAMPLE_GDS_HEADER

/* The application must export the object identfiers it requires.       */

OM_EXPORT (DS_C_AVA)
OM_EXPORT (DS_C_DS_RDN)
OM_EXPORT (DS_C_DS_DN)
OM_EXPORT (DS_C_ENTRY_INFO_SELECTION)
OM_EXPORT (DS_C_ATTRIBUTE)
OM_EXPORT (DS_C_ATTRIBUTE_LIST)
OM_EXPORT (DS_C_FILTER)
OM_EXPORT (DS_C_FILTER_ITEM)
OM_EXPORT (DSX_C_GDS_SESSION)

OM_EXPORT (DS_A_COUNTRY_NAME)
OM_EXPORT (DS_A_ORG_NAME)
OM_EXPORT (DS_A_ORG_UNIT_NAME)
OM_EXPORT (DS_A_COMMON_NAME)
OM_EXPORT (DS_A_OBJECT_CLASS)
OM_EXPORT (DS_A_USER_PASSWORD)
OM_EXPORT (DS_A_PHONE_NBR)
OM_EXPORT (DS_A_SURNAME)

OM_EXPORT (DS_O_TOP)
OM_EXPORT (DS_O_COUNTRY)
OM_EXPORT (DS_O_ORG)
OM_EXPORT (DS_O_ORG_UNIT)
OM_EXPORT (DS_O_PERSON)
OM_EXPORT (DS_O_ORG_PERSON)


/* Build up a descriptor list for the distinguished names:              */
/*     C=DE                                                             */
/*     C=DE/O=SNI                                                       */
/*     C=DE/O=SNI/OU=AP11                                               */
/*     C=DE/O=SNI/OU=AP11/CN=Guenther                                   */

static OM_descriptor    countryName[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("DE")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    organizationName[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("SNI")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    organizationalUName[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_UNIT_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("AP11")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    commonName[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Guenther")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, countryName}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, organizationName}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, organizationalUName}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, commonName}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor name1[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor name2[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor name3[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn3}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor name4[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn1}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn2}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn3}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn4}},
    OM_NULL_DESCRIPTOR
};


/* Build up an array of object identifiers for the attributes to be     */
/* added to the directory.                                              */

static OM_descriptor    object_class1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_COUNTRY),
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    object_class2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG),
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    object_class3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG_UNIT),
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    object_class4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_PERSON),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG_PERSON),
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    telephone[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("+49 89 636 0")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    password[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("secret")},
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    surname[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Schmit")},
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    attr_list1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, object_class1} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    attr_list2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, object_class2} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    attr_list3[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, object_class3} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    attr_list4[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, object_class4} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, surname} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, telephone} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, password} },
    OM_NULL_DESCRIPTOR
};
 

/* The following descriptor list specifies what to return from the entry*/
/* All attribute types and values are selected.                         */

static OM_descriptor selection[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_TRUE},
    {DS_INFO_TYPE, OM_S_ENUMERATION, DS_TYPES_AND_VALUES},
    OM_NULL_DESCRIPTOR
};

/* The following descriptor list specifies the bind credentials         */

static OM_descriptor credentials[] = {
    {DS_REQUESTOR, OM_S_OBJECT, {0, name4} },
    {DSX_PASSWORD, OM_S_OCTET_STRING, OM_STRING("secret")},
    {DSX_AUTH_MECHANISM, OM_S_ENUMERATION, {DSX_SIMPLE, 0}},
    OM_NULL_DESCRIPTOR
};

/* Build up an array of object identifiers for the optional packages    */
/* to be negotiated.                                                    */

DS_feature features[] = {
    { OM_STRING(OMP_O_DS_BASIC_DIR_CONTENTS_PKG), OM_TRUE },
    { OM_STRING(OMP_O_DSX_GDS_PKG), OM_TRUE },
    { 0 }
};

/* The following descriptor list specifies a filter for search :        */
/*     (Present: objectClass)                                           */

static OM_descriptor filter_item1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_PRESENT, 0} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_NULL_DESCRIPTOR
};

static OM_descriptor filter1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item1} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_AND, 0} },
    OM_NULL_DESCRIPTOR
};


OM_workspace workspace;


#endif  /* EXAMPLE_GDS_HEADER */
