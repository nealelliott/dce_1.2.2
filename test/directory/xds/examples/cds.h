/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cds.h,v $
 * Revision 1.1.10.2  1996/02/18  23:03:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:38  marty]
 *
 * Revision 1.1.10.1  1995/12/08  22:12:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:18  root]
 * 
 * Revision 1.1.8.2  1994/10/05  16:44:19  keutel
 * 	OT 12431
 * 	[1994/10/05  16:36:43  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:40:04  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:34:01  marrek]
 * 
 * Revision 1.1.6.2  1993/08/12  12:48:48  marrek
 * 	July 1993 code drop.
 * 	[1993/08/04  11:00:04  marrek]
 * 
 * Revision 1.1.4.2  1993/01/05  20:00:22  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:17:46  htf]
 * 
 * Revision 1.1.2.2  1992/06/11  17:43:00  melman
 * 	New GDS test code drop
 * 	[1992/06/11  17:39:34  melman]
 * 
 * $EndLog$
 */
#ifndef _CDS_H
#define _CDS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cds_rcsid[] = "@(#)$RCSfile: cds.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:03:58 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/* The application must export the object identfiers it requires.       */

OM_EXPORT (DS_C_AVA)
OM_EXPORT (DS_C_DS_RDN)
OM_EXPORT (DS_C_DS_DN)
OM_EXPORT (DS_C_ENTRY_INFO_SELECTION)
OM_EXPORT (DS_C_ATTRIBUTE)
OM_EXPORT (DS_C_ATTRIBUTE_LIST)

OM_EXPORT (DS_A_COUNTRY_NAME)
OM_EXPORT (DS_A_ORG_NAME)
OM_EXPORT (DS_A_ORG_UNIT_NAME)
OM_EXPORT (DS_A_COMMON_NAME)
OM_EXPORT (DS_A_OBJECT_CLASS)
OM_EXPORT (DS_A_PHONE_NBR)
OM_EXPORT (DS_A_USER_PASSWORD)
OM_EXPORT (DS_A_SURNAME)

OM_EXPORT (DS_O_TOP)
OM_EXPORT (DS_O_PERSON)
OM_EXPORT (DS_O_ORG_PERSON)

/* Build up descriptor lists for the following distinguished names:     */
/*     C=ie/O=sni                                                       */
/*     C=ie/O=sni/TL=brendan                                            */
/*     C=ie/O=sni/TL=sinead                                             */

static OM_descriptor    ava_ie[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("ie")},
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
static OM_descriptor    ava_brendan[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("brendan")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    ava_sinead[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COMMON_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("sinead")},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn_ie[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, ava_ie}},
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
static OM_descriptor    rdn_brendan[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, ava_brendan}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor    rdn_sinead[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, ava_sinead}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor dn_ap[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn_ie}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_sni}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_ap}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor dn_brendan[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn_ie}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_sni}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_ap}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_brendan}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor dn_sinead[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn_ie}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_sni}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_ap}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_sinead}},
    OM_NULL_DESCRIPTOR
};


/* Build up an array of object identifiers for the optional packages    */
/* to be negotiated.                                                    */

DS_feature features[] = {
    { OM_STRING(OMP_O_DS_BASIC_DIR_CONTENTS_PKG), OM_TRUE },
    { 0 }
};

/* Build up an array of object identifiers for the attributes to be     */
/* added to the directory.                                              */

static OM_descriptor    object_class[] = {
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
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING,
    OM_STRING("+49 89 636 0")},
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    surname[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_SURNAME),
    {DS_ATTRIBUTE_VALUES, OM_S_TELETEX_STRING, OM_STRING("Moloney")},
    OM_NULL_DESCRIPTOR
};

static OM_descriptor surname2[] = {
    OM_OID_DESC(OM_CLASS,DS_C_AVA),
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
 

#endif /* _CDS_H */
