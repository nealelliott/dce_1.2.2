/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: thradd.h,v $
 * Revision 1.1.6.2  1996/02/18  23:04:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:46  marty]
 *
 * Revision 1.1.6.1  1995/12/08  22:13:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:37  root]
 * 
 * Revision 1.1.4.3  1994/10/05  16:44:29  keutel
 * 	OT 12431
 * 	[1994/10/05  16:37:11  keutel]
 * 
 * Revision 1.1.4.2  1994/06/10  20:19:36  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:36  devsrc]
 * 
 * Revision 1.1.4.1  1994/02/22  16:40:38  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:35:52  marrek]
 * 
 * Revision 1.1.2.1  1993/10/14  19:28:35  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:26:56  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  12:38:12  keutel]
 * 
 * $EndLog$
 */

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1993   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

#ifndef THRADD_H
#define THRADD_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char thradd_rcsid[] = "@(#)$RCSfile: thradd.h,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 23:04:02 $";
#endif

#ifndef TRUE
#define TRUE	(1)
#endif

#ifndef FALSE
#define FALSE	(0)
#endif

#define SUCCESS 0
#define FAILURE 1
#define MAX_LINE_LEN  100 /* max length of line in input file */
#define MAX_AT_LEN    100 /* max length of an attribute value */
#define MAX_THREAD_NO  10 /* max number of threads created    */


/* The application must export the object identfiers it requires.       */

OM_EXPORT (DS_C_AVA)
OM_EXPORT (DS_C_DS_RDN)
OM_EXPORT (DS_C_DS_DN)
OM_EXPORT (DS_C_ATTRIBUTE)
OM_EXPORT (DS_C_ATTRIBUTE_LIST)

OM_EXPORT (DS_A_COUNTRY_NAME)
OM_EXPORT (DS_A_ORG_NAME)
OM_EXPORT (DS_A_ORG_UNIT_NAME)
OM_EXPORT (DS_A_COMMON_NAME)
OM_EXPORT (DS_A_OBJECT_CLASS)
OM_EXPORT (DS_A_PHONE_NBR)
OM_EXPORT (DS_A_SURNAME)

OM_EXPORT (DS_O_TOP)
OM_EXPORT (DS_O_COUNTRY)
OM_EXPORT (DS_O_ORG)
OM_EXPORT (DS_O_ORG_UNIT)
OM_EXPORT (DS_O_PERSON)
OM_EXPORT (DS_O_ORG_PERSON)


/* Build up descriptor lists for the following distinguished names:     */
/*     root                                                             */
/*     /C=it                                                            */
/*     /C=it/O=sni                                                      */
/*     /C=it/O=sni/OU=ap                                                */

static OM_descriptor    ava_it[] = {
    OM_OID_DESC(OM_CLASS, DS_C_AVA),
    OM_OID_DESC(DS_ATTRIBUTE_TYPE, DS_A_COUNTRY_NAME),
    {DS_ATTRIBUTE_VALUES, OM_S_PRINTABLE_STRING, OM_STRING("it")},
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
static OM_descriptor    rdn_it[] = {
    OM_OID_DESC(OM_CLASS, DS_C_DS_RDN),
    {DS_AVAS, OM_S_OBJECT, {0, ava_it}},
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
static OM_descriptor dn_root[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    OM_NULL_DESCRIPTOR
};
static OM_descriptor dn_it[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn_it}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor dn_sni[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn_it}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_sni}},
    OM_NULL_DESCRIPTOR
};
static OM_descriptor dn_ap[] = {
    OM_OID_DESC(OM_CLASS,DS_C_DS_DN),
    {DS_RDNS,OM_S_OBJECT,{0,rdn_it}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_sni}},
    {DS_RDNS,OM_S_OBJECT,{0,rdn_ap}},
    OM_NULL_DESCRIPTOR
};


/* Build up an array of object identifiers for the attributes to be     */
/* added to the directory.                                              */

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

static OM_descriptor    alist_C[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, obj_class_C} },
    OM_NULL_DESCRIPTOR
};
 
static OM_descriptor    alist_O[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, obj_class_O} },
    OM_NULL_DESCRIPTOR
};

static OM_descriptor    alist_OU[] = {
    OM_OID_DESC(OM_CLASS, DS_C_ATTRIBUTE_LIST),
    {DS_ATTRIBUTES, OM_S_OBJECT, {0, obj_class_OU} },
    OM_NULL_DESCRIPTOR
};
 

/* Build up an array of object identifiers for the optional packages    */
/* to be negotiated.                                                    */

static DS_feature features[] = {
    { OM_STRING(OMP_O_DS_BASIC_DIR_CONTENTS_PKG), OM_TRUE },
    { OM_STRING(OMP_O_DSX_GDS_PKG), OM_TRUE },
    { 0 }
};

#endif  /* THRADD_H */
