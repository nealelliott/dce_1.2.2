/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl.h,v $
 * Revision 1.1.2.2  1996/03/09  20:50:32  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:38  marty]
 *
 * Revision 1.1.2.1  1995/12/11  21:56:12  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/02  20:24 UTC  bissen
 * 	merge HPDCE01
 * 
 * 	HP revision /main/HPDCE01/1  1994/04/01  17:44 UTC  truitt
 * 
 * 	HP revision /main/truitt_junk/1  1994/04/01  16:57 UTC  truitt
 * 	move file to cds directory
 * 	[1995/12/11  20:56:04  root]
 * 
 * 	Loading drop Dce1_0
 * 	[1992/10/01  21:33:57  osfrcs]
 * 
 * 	    Loading drop Dce1_0_1fnl
 * 	[1992/09/21  18:35:51  osfrcs]
 * 
 * Revision 1.1.7.2  1993/07/09  10:54:54  root
 * 	Initial King Kong branch
 * 	[1993/07/09  10:54:25  root]
 * 
 * Revision 1.1.5.2  1993/04/21  19:34:09  giza
 * 	DCE 1.0.1 Release
 * 	[1993/04/21  19:17:32  giza]
 * 
 * $EndLog$
 */
/*
 * (c) Copyright 1990, 1991, 1992 OPEN SOFTWARE FOUNDATION, INC. 
 * ALL RIGHTS RESERVED 
 */
/*
 * OSF DCE Version 1.0, UPDATE 1.0.1
 */
/*
 *
 *   	FILE_NAME: acl.h
 *
 *   	COMPONENT_NAME: dce.systest
 *
 *   	FUNCTIONS: none
 *
 *   	ORIGINS: 27
 *
 *   	(C) COPYRIGHT International Business Machines Corp. 1990
 *   	All Rights Reserved
 *
 *   	FOR_TEST: dcegdacl
 *
 *   	USAGE: n/a
 *
 *   	DESCRIPTION:
 *
 *		This file contains OM descriptor lists used by the test
 *		acl_tester.
 *
 *   	HISTORY:
 *      	11/10/90        Gerald Cantor           Initial design/code
 *		10/20/91	Gerald Cantor		Converted postal
 *							address to correct
 *							form as per Helmut
 *							Volpers.
 */
OM_EXPORT(DS_C_AVA)
OM_EXPORT(DS_C_DS_RDN)
OM_EXPORT(DS_C_ATTRIBUTE)
OM_EXPORT(DS_C_ATTRIBUTE_LIST)
OM_EXPORT(DS_C_ENTRY_MOD)
OM_EXPORT(DS_C_ENTRY_MOD_LIST)
OM_EXPORT(DS_C_ENTRY_INFO)
OM_EXPORT(DS_C_ENTRY_INFO_SELECTION)
OM_EXPORT(DS_C_PRESENTATION_ADDRESS)
OM_EXPORT(DS_C_POSTAL_ADDRESS)
OM_EXPORT(DS_C_DS_DN)
OM_EXPORT(DSX_C_GDS_SESSION)
OM_EXPORT(DSX_A_ACL)
OM_EXPORT(DSX_C_GDS_ACL)
OM_EXPORT(DSX_C_GDS_ACL_ITEM)
OM_EXPORT(DS_A_OBJECT_CLASS)
OM_EXPORT(DS_A_COUNTRY_NAME)
OM_EXPORT(DS_A_ORG_NAME)
OM_EXPORT(DS_A_ORG_UNIT_NAME)
OM_EXPORT(DS_A_COMMON_NAME)
OM_EXPORT(DS_A_SURNAME)
OM_EXPORT(DS_O_ORG_PERSON)
OM_EXPORT(DS_O_PERSON)
OM_EXPORT(DS_A_PHONE_NBR)
OM_EXPORT(DS_A_POSTAL_ADDRESS)
OM_EXPORT(DS_A_USER_PASSWORD)

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

static OM_descriptor credentials[] = {
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR, /* to be filled in later */
        OM_NULL_DESCRIPTOR
};

/*
 * Gerald Cantor is the object we will be adding.
 */
static OM_descriptor common_name[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
	{DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Gerald")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor surname[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
	{DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Cantor")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor rdn1[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
	{ DS_AVAS, OM_S_OBJECT, { 0, country } },
	OM_NULL_DESCRIPTOR
};

static OM_descriptor rdn2[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
	{ DS_AVAS, OM_S_OBJECT, { 0, organization } },
	OM_NULL_DESCRIPTOR
};

static OM_descriptor rdn3[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
	{ DS_AVAS, OM_S_OBJECT, { 0, organizational_unit } },
	OM_NULL_DESCRIPTOR
};

static OM_descriptor rdn4[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
	{DS_AVAS, OM_S_OBJECT, {0, common_name}},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor name[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
	{DS_RDNS, OM_S_OBJECT, {0, rdn1}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn2}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn3}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn4}},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor phone_number[] = {
        OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
        {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("555-1212")},
        OM_NULL_DESCRIPTOR
};

static OM_descriptor postal_address_portion[] = {
	OM_OID_DESC(OM_CLASS, DS_C_POSTAL_ADDRESS),
	{DS_POSTAL_ADDRESS, OM_S_TELETEX_STRING, OM_STRING("2603")},
	OM_NULL_DESCRIPTOR
};
 
static OM_descriptor postal_address[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_POSTAL_ADDRESS),
	{DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, postal_address_portion}},
	OM_NULL_DESCRIPTOR
};


static OM_descriptor password[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
	{ DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("Cantor") },
	OM_NULL_DESCRIPTOR
};

/*
 * This is the attribute of all the users that will logon to the 
 * DSA to access object Gerald.  They all are Organizational Persons
 */
static OM_descriptor common_attribute[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
	OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG_PERSON),
	OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_PERSON),
	OM_NULL_DESCRIPTOR
};
	
/*
 * The following five users will be used to logon to the DSA where 
 * object Gerald exists.
 */	

/*
 * User Danna.  She will have read and modify public rights.
 */
static OM_descriptor user1_comm_name[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
	{DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Danna")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor surname1[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
	{DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Danna")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor user1_rdn[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
	{DS_AVAS, OM_S_OBJECT, {0, user1_comm_name}},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor user1_passwd[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
	{ DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("Danna")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor Danna[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
	{DS_RDNS, OM_S_OBJECT, {0, rdn1}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn2}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn3}},
	{DS_RDNS, OM_S_OBJECT, {0, user1_rdn}},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor Danna_entry_info[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, common_attribute } },
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, user1_passwd } },
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, surname1 } },
	OM_NULL_DESCRIPTOR
};

/* End of user Danna. */

/*
 * User Ken.  He will have read standard rights. 
 */
static OM_descriptor user2_comm_name[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
	{DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Ken")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor surname2[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
	{DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Ken")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor user2_rdn[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
	{DS_AVAS, OM_S_OBJECT, {0, user2_comm_name}},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor Ken[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
	{DS_RDNS, OM_S_OBJECT, {0, rdn1}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn2}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn3}},
	{DS_RDNS, OM_S_OBJECT, {0, user2_rdn}},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor user2_passwd[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
	{ DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("Ken")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor Ken_entry_info[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, common_attribute } },
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, user2_passwd } },
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, surname2 } },
	OM_NULL_DESCRIPTOR
};
/* End of user Ken */

/*
 * User Wayne.  He will have read and write standard rights. 
 */
static OM_descriptor user3_comm_name[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
	{DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Wayne")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor surname3[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
	{DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Wayne")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor user3_rdn[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
	{DS_AVAS, OM_S_OBJECT, {0, user3_comm_name}},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor Wayne[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
	{DS_RDNS, OM_S_OBJECT, {0, rdn1}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn2}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn3}},
	{DS_RDNS, OM_S_OBJECT, {0, user3_rdn}},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor user3_passwd[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
	{ DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("Wayne")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor Wayne_entry_info[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, common_attribute } },
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, user3_passwd } },
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, surname3 } },
	OM_NULL_DESCRIPTOR
};

/*
 * User Jerod.  He will have read sensitive rights. 
 */
static OM_descriptor user4_comm_name[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
	{DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Jerod")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor surname4[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
	{DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Jerod")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor user4_rdn[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
	{DS_AVAS, OM_S_OBJECT, {0, user4_comm_name}},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor Jerod[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
	{DS_RDNS, OM_S_OBJECT, {0, rdn1}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn2}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn3}},
	{DS_RDNS, OM_S_OBJECT, {0, user4_rdn}},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor user4_passwd[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
	{ DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("Jerod")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor Jerod_entry_info[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, common_attribute } },
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, user4_passwd } },
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, surname4 } },
	OM_NULL_DESCRIPTOR
};

/*
 * User Helmut.  He will have read and write sensitive rights. 
 */
static OM_descriptor user5_comm_name[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
	{DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Helmut")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor surname5[] = {
	OM_OID_DESC(OM_CLASS, DS_C_AVA),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
	{DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Helmut")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor user5_rdn[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
	{DS_AVAS, OM_S_OBJECT, {0, user5_comm_name}},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor Helmut[] = {
	OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
	{DS_RDNS, OM_S_OBJECT, {0, rdn1}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn2}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn3}},
	{DS_RDNS, OM_S_OBJECT, {0, user5_rdn}},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor user5_passwd[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
	{ DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("Helmut")},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor Helmut_entry_info[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, common_attribute } },
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, user5_passwd } },
	{ DS_ATTRIBUTES, OM_S_OBJECT, { 0, surname5 } },
	OM_NULL_DESCRIPTOR
};

/*
 * ACL info for object Gerald.
 */
static OM_descriptor subtree[] = {
        OM_OID_DESC(OM_CLASS, DS_C_DS_DN),
	{DS_RDNS, OM_S_OBJECT, {0, rdn1}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn2}},
        {DS_RDNS, OM_S_OBJECT, {0, rdn3}},
        OM_NULL_DESCRIPTOR
};

static OM_descriptor    acl_item1[] = {
    	OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL_ITEM),
    	{DSX_INTERPRETATION, OM_S_ENUMERATION, {DSX_ROOT_OF_SUBTREE, 0}},
	{ DSX_USER, OM_S_OBJECT, { 0, subtree } },
    	OM_NULL_DESCRIPTOR
};

static OM_descriptor    acl_item2[] = {
    	OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL_ITEM),
    	{DSX_INTERPRETATION, OM_S_ENUMERATION, {DSX_SINGLE_OBJECT, 0}},
	{DSX_USER, OM_S_OBJECT, {0, Ken}},
    	OM_NULL_DESCRIPTOR
};


static OM_descriptor    acl_item3[] = {
    	OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL_ITEM),
    	{DSX_INTERPRETATION, OM_S_ENUMERATION, {DSX_SINGLE_OBJECT, 0}},
	{DSX_USER, OM_S_OBJECT, {0, Wayne}},
    	OM_NULL_DESCRIPTOR
};

static OM_descriptor    acl_item4[] = {
    	OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL_ITEM),
    	{DSX_INTERPRETATION, OM_S_ENUMERATION, {DSX_SINGLE_OBJECT, 0}},
	{DSX_USER, OM_S_OBJECT, {0, Jerod}},
    	OM_NULL_DESCRIPTOR
};

static OM_descriptor    acl_item5[] = {
    	OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL_ITEM),
    	{DSX_INTERPRETATION, OM_S_ENUMERATION, {DSX_SINGLE_OBJECT, 0}},
	{DSX_USER, OM_S_OBJECT, {0, Helmut}},
    	OM_NULL_DESCRIPTOR
};

static OM_descriptor    acl1[] = {
    	OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL),
	{DSX_MODIFY_PUBLIC, OM_S_OBJECT, {0, acl_item1}},
	{DSX_READ_STANDARD, OM_S_OBJECT, {0, acl_item2}},
	{DSX_READ_STANDARD, OM_S_OBJECT, {0, acl_item3}},
	{DSX_READ_STANDARD, OM_S_OBJECT, {0, acl_item4}},
	{DSX_READ_STANDARD, OM_S_OBJECT, {0, acl_item5}},
	{DSX_MODIFY_STANDARD, OM_S_OBJECT, {0, acl_item3}},
	{DSX_MODIFY_STANDARD, OM_S_OBJECT, {0, acl_item4}},
	{DSX_MODIFY_STANDARD, OM_S_OBJECT, {0, acl_item5}},
	{DSX_READ_SENSITIVE, OM_S_OBJECT, {0, acl_item4}},
	{DSX_READ_SENSITIVE, OM_S_OBJECT, {0, acl_item5}},
	{DSX_MODIFY_SENSITIVE, OM_S_OBJECT, {0, acl_item5}},
	OM_NULL_DESCRIPTOR
};

static OM_descriptor acl[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_ACL),
	{ DS_ATTRIBUTE_VALUES, OM_S_OBJECT, { 0, acl1 } },
	OM_NULL_DESCRIPTOR
};

/*
 * Now that the ACL info is complete, we can construct the entry.
 * This is the entry that we will pass to ds_add_entry.
 */
static OM_descriptor entry_info[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
	{ DS_ATTRIBUTES, OM_S_OBJECT, {0, common_attribute} },
	{ DS_ATTRIBUTES, OM_S_OBJECT, {0, phone_number} },
	{ DS_ATTRIBUTES, OM_S_OBJECT, {0, postal_address} },
	{ DS_ATTRIBUTES, OM_S_OBJECT, {0, password} },
	{ DS_ATTRIBUTES, OM_S_OBJECT, {0, surname} },
	{ DS_ATTRIBUTES, OM_S_OBJECT, {0, acl} },
	OM_NULL_DESCRIPTOR
};

/*
 * Entry information passed in ds_read call that will get the phone number
 * attribute values.
 */
static OM_descriptor phone_num[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
	{DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
	OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_PHONE_NBR),
	{ DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_AND_VALUES, NULL} },
        OM_NULL_DESCRIPTOR
};

/*
 * Entry information passed in ds_read call that will get the postal address 
 * attribute values.
 */
static OM_descriptor user_post_addr[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
	{DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
	OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_POSTAL_ADDRESS),
	{ DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_AND_VALUES, NULL} },
        OM_NULL_DESCRIPTOR
};

/*
 * Entry information passed in ds_read call that will get the user password
 * attribute values.
 */
static OM_descriptor user_password[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
	{DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
	OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_USER_PASSWORD),
	{ DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_AND_VALUES, NULL} },
	OM_NULL_DESCRIPTOR
};

/*
 * Information passed to ds_mod_entry() that will remove phone number.
 */
static OM_descriptor phone_rm[] = {
	OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
	{DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_ATTRIBUTE, NULL} },
	OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
	OM_NULL_DESCRIPTOR
};

/*
 * Information passed to ds_mod_entry() that will add a new phone number.
 */
static OM_descriptor phone_add[] = {
        OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
        {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
        {DS_ATTRIBUTE_VALUES,OM_S_OCTET_STRING,OM_STRING("777-0909")},
        OM_NULL_DESCRIPTOR
};

/*
 * Information passed to ds_mod_entry() that will remove phone nubmer and
 * add a new phone number.
 */
static OM_descriptor phone_modification[] = {
        OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
        {DS_CHANGES, OM_S_OBJECT, {0, phone_rm} },
        {DS_CHANGES, OM_S_OBJECT, {0, phone_add} },
        OM_NULL_DESCRIPTOR
};

/*
 * Information passed to ds_mod_entry() that will remove postal address.
 */
static OM_descriptor post_addr_rm[] = {
        OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
        {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_ATTRIBUTE, NULL} },
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_POSTAL_ADDRESS),
        OM_NULL_DESCRIPTOR
};

/*
 * Information passed to ds_mod_entry() that will add a new postal address.
 */
static OM_descriptor postal_address_portion_mod[] = {
	OM_OID_DESC(OM_CLASS, DS_C_POSTAL_ADDRESS),
	{DS_POSTAL_ADDRESS, OM_S_TELETEX_STRING, OM_STRING("2402")},
	OM_NULL_DESCRIPTOR
};
 
static OM_descriptor post_addr_add[] = {
        OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
        {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_POSTAL_ADDRESS),
        {DS_ATTRIBUTE_VALUES,OM_S_OBJECT,{0, postal_address_portion_mod}},
        OM_NULL_DESCRIPTOR
};

/*
 * Information passed to ds_mod_entry() that will remove title and
 * add a new title.
 */
static OM_descriptor post_addr_modification[] = {
        OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
        {DS_CHANGES, OM_S_OBJECT, {0, post_addr_rm} },
        {DS_CHANGES, OM_S_OBJECT, {0, post_addr_add} },
        OM_NULL_DESCRIPTOR
};

/*
 * Information passed to ds_mod_entry() that will remove password.
 */
static OM_descriptor pw_rm[] = {
        OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
        {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_ATTRIBUTE, NULL} },
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
        OM_NULL_DESCRIPTOR
};

/*
 * Information passed to ds_mod_entry() that will add a new password.
 */
static OM_descriptor pw_add[] = {
        OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD),
        {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL} },
        OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
        {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("Monroe")},
        OM_NULL_DESCRIPTOR
};

/*
 * Information passed to ds_mod_entry() that will remove password and
 * add a new password.
 */
static OM_descriptor pw_modification[] = {
        OM_OID_DESC(OM_CLASS, DS_C_ENTRY_MOD_LIST),
        {DS_CHANGES, OM_S_OBJECT, {0, pw_rm} },
        {DS_CHANGES, OM_S_OBJECT, {0, pw_add} },
        OM_NULL_DESCRIPTOR
};
