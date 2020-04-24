/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: structures.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:22  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:57:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:57:35  root]
 * 
 * Revision 1.1.2.1  1993/09/20  22:40:43  pellis
 * 	Moved from ./systest/directory/gds/*.
 * 	[1993/09/20  22:18:03  pellis]
 * 
 * Revision 1.1.2.2  1993/03/31  21:57:08  mhickey
 * 	Initial checkin for defect 7071.
 * 	[1993/03/31  21:52:44  mhickey]
 * 
 * Revision 1.1.6.2  1993/02/05  15:20:15  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:40:20  cjd]
 * 
 * Revision 1.1.4.2  1993/02/04  22:06:59  cjd
 * 	Embedded copyright notice
 * 
 * Revision 1.1.2.4  1992/07/24  20:07:15  mhickey
 * 	Added dsa_name object for name of dsa to contact, ported to
 * 	latest GDS code.
 * 
 * 	defect 4817.
 * 	[1992/07/24  20:05:24  mhickey]
 * 
 * Revision 1.1.2.3  1992/06/05  22:15:03  gmd
 * 	Shortened naming
 * 	[1992/06/05  21:06:32  gmd]
 * 
 * Revision 1.1.2.2  1992/03/18  01:22:37  akers
 * 	Initial code changes for Directory system tests.
 * 	[1992/03/05  22:39:07  akers]
 * 
 * Revision 1.1  1992/01/19  14:35:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 */
#ifndef STRUCTURES_HEADER
#define STRUCTURES_HEADER
OM_EXPORT(DS_A_COMMON_NAME)
OM_EXPORT(DS_A_COUNTRY_NAME)
OM_EXPORT(DS_A_ORG_NAME)
OM_EXPORT(DS_A_ORG_UNIT_NAME)
OM_EXPORT(DS_A_PHONE_NBR)
OM_EXPORT(DS_C_AVA)
OM_EXPORT(DS_C_DS_DN)
OM_EXPORT(DS_C_DS_RDN)
OM_EXPORT(DS_C_ENTRY_INFO_SELECTION)
OM_EXPORT(DSX_C_GDS_SESSION)

DS_feature feature_list[] = {
        { OM_STRING(OMP_O_DS_BASIC_DIR_CONTENTS_PKG), 0 },
        { OM_STRING(OMP_O_DSX_GDS_PKG), 0 },
        { 0 }
};

static OM_descriptor country[]  = {
        OM_OID_DESC(OM_CLASS, DS_C_AVA),
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR
};

static OM_descriptor organization[] = {
        OM_OID_DESC(OM_CLASS, DS_C_AVA),
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_NAME),
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR
};

static OM_descriptor organizational_unit[] = {
        OM_OID_DESC(OM_CLASS, DS_C_AVA),
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_UNIT_NAME),
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR
};

static OM_descriptor common_name1[] = {
        OM_OID_DESC(OM_CLASS, DS_C_AVA),
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR
};

static OM_descriptor common_name[] = {
        OM_OID_DESC(OM_CLASS, DS_C_AVA),
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR
};

static OM_descriptor rdn1[] = {
        OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR
};

static OM_descriptor rdn2[] = {
        OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR
};

static OM_descriptor rdn3[] = {
        OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR
};

static OM_descriptor rdn4[] = {
        OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR
};

static OM_descriptor rdn5[] = {
        OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR
};

static OM_descriptor name[] = {
        OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR
};

static OM_descriptor dsa_name[] = {
        OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR
};

static OM_descriptor selection[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION), 
	{ DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, { OM_FALSE, NULL} },
	OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_PHONE_NBR),
	{ DS_INFO_TYPE, OM_S_ENUMERATION, { DS_TYPES_AND_VALUES, NULL }},
	OM_NULL_DESCRIPTOR
};


static OM_descriptor dsa_session[] = {
        OM_OID_DESC(OM_CLASS, DSX_C_GDS_SESSION),
        OM_NULL_DESCRIPTOR,  /*---* filled in at runtime *--*/
        OM_NULL_DESCRIPTOR,  /*---* filled in at runtime *--*/
/*---*        {DSX_DIR_ID, OM_S_INTEGER, {2,0}}, *---*/
        OM_NULL_DESCRIPTOR
};


#endif



