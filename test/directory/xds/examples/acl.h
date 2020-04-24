/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl.h,v $
 * Revision 1.1.10.2  1996/02/18  23:03:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:34  marty]
 *
 * Revision 1.1.10.1  1995/12/08  22:12:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:11  root]
 * 
 * Revision 1.1.8.2  1994/10/05  16:44:15  keutel
 * 	OT 12431
 * 	[1994/10/05  16:36:28  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:39:58  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:33:43  marrek]
 * 
 * Revision 1.1.6.2  1993/08/12  12:46:58  marrek
 * 	July 1993 code drop.
 * 	[1993/08/04  10:59:45  marrek]
 * 
 * Revision 1.1.4.2  1993/01/05  20:00:09  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:17:36  htf]
 * 
 * Revision 1.1.2.3  1992/07/01  03:25:28  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  23:04:05  melman]
 * 
 * Revision 1.1.2.2  1992/06/11  17:42:43  melman
 * 	New GDS test code drop
 * 	[1992/06/11  17:39:19  melman]
 * 
 * $EndLog$
 */
#ifndef _ACL_HEADER
#define _ACL_HEADER

#if !defined(lint) && defined(GDS_RCS_ID)
static char acl_rcsid[] = "@(#)$RCSfile: acl.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:03:55 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

#define MAX_DN_LEN 100
/* max length of a distinguished name in string format */


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
OM_EXPORT (DSX_C_GDS_CONTEXT)
OM_EXPORT (DSX_C_GDS_ACL)
OM_EXPORT (DSX_C_GDS_ACL_ITEM)

OM_EXPORT (DS_A_COUNTRY_NAME)
OM_EXPORT (DS_A_ORG_NAME)
OM_EXPORT (DS_A_ORG_UNIT_NAME)
OM_EXPORT (DS_A_COMMON_NAME)
OM_EXPORT (DS_A_LOCALITY_NAME)
OM_EXPORT (DS_A_OBJECT_CLASS)
OM_EXPORT (DS_A_USER_PASSWORD)
OM_EXPORT (DS_A_PHONE_NBR)
OM_EXPORT (DS_A_SURNAME)
OM_EXPORT (DSX_A_ACL)
OM_EXPORT (DSX_TYPELESS_RDN)

OM_EXPORT (DS_O_TOP)
OM_EXPORT (DS_O_COUNTRY)
OM_EXPORT (DS_O_ORG)
OM_EXPORT (DS_O_ORG_UNIT)
OM_EXPORT (DS_O_PERSON)
OM_EXPORT (DS_O_ORG_PERSON)


/* Build up descriptor lists for the following distinguished names:     */
/*     root                                                             */
/*     /C=de                                                            */
/*     /C=de/O=sni                                                      */
/*     /C=de/O=sni/OU=ap                                                */
/*     /C=de/O=sni/OU=ap/CN=stefanie                                    */
/*     /C=de/O=sni/OU=ap/CN=norbert                                     */
/*     /C=de/O=sni/OU=ap/CN=ingrid                                      */

static OM_descriptor    ava_de[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("de")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    ava_sni[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("sni")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    ava_ap[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_ORG_UNIT_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("ap")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    ava_stefanie[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("stefanie")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    ava_norbert[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("norbert")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    ava_ingrid[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("ingrid")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn_de[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, ava_de}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn_sni[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, ava_sni}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn_ap[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, ava_ap}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn_stefanie[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, ava_stefanie}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn_norbert[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, ava_norbert}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn_ingrid[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, ava_ingrid}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor dn_root[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    OM_NULL_DESCRIPTOR
};
static OM_descriptor dn_de[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn_de}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor dn_sni[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn_de}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_sni}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor dn_ap[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn_de}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_sni}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_ap}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor dn_stefanie[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn_de}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_sni}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_ap}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_stefanie}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor dn_norbert[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn_de}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_sni}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_ap}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_norbert}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor dn_ingrid[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn_de}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_sni}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_ap}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_ingrid}},
    OM_NULL_DESCRIPTOR
};


/* Build up an array of object identifiers for the attributes to be */
/* added to the directory.                                          */

static OM_descriptor    obj_class_C[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_COUNTRY),
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    obj_class_O[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG),
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    obj_class_OU[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG_UNIT),
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    obj_class_OP[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_TOP),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_PERSON),
    OM_OID_DESC(DS_ATTRIBUTE_VALUES, DS_O_ORG_PERSON),
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    att_phone_num[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_PHONE_NBR),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING,
      OM_STRING("+49 89 636 0")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    att_password[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_USER_PASSWORD),
    {DS_ATTRIBUTE_VALUES, OM_S_OCTET_STRING, OM_STRING("secret")},
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    att_surname[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Schmid")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    acl_item_root[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL_ITEM),
    {DSX_INTERPRETATION, OM_S_ENUMERATION, {DSX_ROOT_OF_SUBTREE, 0}},
    {DSX_USER, OM_S_OBJECT, {0, dn_root}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    acl_item_ap[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL_ITEM),
    {DSX_INTERPRETATION, OM_S_ENUMERATION, {DSX_ROOT_OF_SUBTREE, 0}},
    {DSX_USER, OM_S_OBJECT, {0, dn_ap}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    acl_item_stefanie[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL_ITEM),
    {DSX_INTERPRETATION, OM_S_ENUMERATION, {DSX_SINGLE_OBJECT, 0}},
    {DSX_USER, OM_S_OBJECT, {0, dn_stefanie}},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    acl1[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL),
    {DSX_MODIFY_PUBLIC, OM_S_OBJECT, {0, acl_item_root}},
    {DSX_READ_STANDARD, OM_S_OBJECT, {0, acl_item_stefanie}},
    {DSX_MODIFY_STANDARD, OM_S_OBJECT, {0, acl_item_stefanie}},
    {DSX_READ_SENSITIVE, OM_S_OBJECT, {0, acl_item_stefanie}},
    {DSX_MODIFY_SENSITIVE, OM_S_OBJECT, {0, acl_item_stefanie}},
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    acl2[] = {
    OM_OID_DESC(OM_CLASS, DSX_C_GDS_ACL),
    {DSX_MODIFY_PUBLIC, OM_S_OBJECT, {0, acl_item_ap}},
    {DSX_READ_STANDARD, OM_S_OBJECT, {0, acl_item_ap}},
    {DSX_MODIFY_STANDARD, OM_S_OBJECT, {0, acl_item_stefanie}},
    {DSX_READ_SENSITIVE, OM_S_OBJECT, {0, acl_item_ap}},
    {DSX_MODIFY_SENSITIVE, OM_S_OBJECT, {0, acl_item_stefanie}},
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    att_acl1[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_ACL),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, acl1} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    att_acl2[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DSX_A_ACL),
    {DS_ATTRIBUTE_VALUES, OM_S_OBJECT, {0, acl2} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    alist_C[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, obj_class_C} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    alist_O[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, obj_class_O} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att_acl1} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    alist_OU[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, obj_class_OU} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    alist_OP[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, obj_class_OP} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att_acl2} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att_surname} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att_phone_num} },
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, att_password} },
    OM_NULL_DESCRIPTOR
};
 

/* The following descriptor list specifies what to return from the*/
/* entry. The ACL attribute's types and values are selected.      */

static OM_descriptor selection_acl[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
    {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
    OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DSX_A_ACL),
    {DS_INFO_TYPE, OM_S_ENUMERATION, DS_TYPES_AND_VALUES},
    OM_NULL_DESCRIPTOR
};

/* The following descriptor list specifies the bind credentials   */

static OM_descriptor credentials[] = {
    {DS_REQUESTOR, OM_S_OBJECT, {0, dn_norbert} },
    {DSX_PASSWORD, OM_S_OCTET_STRING, OM_STRING("secret")},
    {DSX_AUTH_MECHANISM, OM_S_ENUMERATION, {DSX_SIMPLE, 0}},
    OM_NULL_DESCRIPTOR
};

/* The following descriptor list specifies part of the context    */

static OM_descriptor use_copy[] = {
    {DS_DONT_USE_COPY, OM_S_BOOLEAN, {OM_FALSE, 0} },
    OM_NULL_DESCRIPTOR
};

/* Build up an array of object identifiers for the optional       */
/* packages to be negotiated.                                     */

DS_feature features[] = {
    { OM_STRING(OMP_O_DS_BASIC_DIR_CONTENTS_PKG), OM_TRUE },
    { OM_STRING(OMP_O_DSX_GDS_PKG), OM_TRUE },
    { 0 }
};

/* The following descriptor list specifies a filter for search :  */
/*     (Present: objectClass)                                     */

static OM_descriptor filter_item[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER_ITEM),
    {DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, {DS_PRESENT, 0} },
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_OBJECT_CLASS),
    OM_NULL_DESCRIPTOR
};

static OM_descriptor filter[] = {
    OM_OID_DESC(OM_CLASS, DS_C_FILTER),
    {DS_FILTER_ITEMS, OM_S_OBJECT, {0, filter_item} },
    {DS_FILTER_TYPE, OM_S_ENUMERATION, {DS_AND, 0} },
    OM_NULL_DESCRIPTOR
};


#endif /* _ACL_HEADER */
