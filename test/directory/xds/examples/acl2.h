/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl2.h,v $
 * Revision 1.1.4.2  1996/02/18  23:03:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:12:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:15  root]
 * 
 * Revision 1.1.2.2  1994/10/05  16:44:17  keutel
 * 	OT 12431
 * 	[1994/10/05  16:36:37  keutel]
 * 
 * Revision 1.1.2.1  1994/09/14  09:46:37  marrek
 * 	Initial version.
 * 	[1994/09/13  12:47:49  marrek]
 * 
 * $EndLog$
 */
#ifndef _ACL2_H
#define _ACL2_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char acl2_rcsid[] = "@(#)$RCSfile: acl2.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:03:57 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

#define MAX_DN_LEN 100  /* max length of a distinguished name in string format*/

/* Step 1 */

/* The application must export the object identfiers it requires.       */

OM_EXPORT (DS_C_DS_DN)
OM_EXPORT (DS_C_ENTRY_INFO_SELECTION)
OM_EXPORT (DS_C_ATTRIBUTE)
OM_EXPORT (DS_C_ATTRIBUTE_LIST)
OM_EXPORT (DS_C_FILTER)
OM_EXPORT (DSX_C_GDS_SESSION)
OM_EXPORT (DSX_C_GDS_CONTEXT)
OM_EXPORT (DSX_A_ACL)



/* 
 * Structure to contain the name and attribute list of a directory entry
 */
struct entry {
    OM_private_object	name;
    OM_object		attr_list;
} Entry;

/* Step 2 */

/* 
 * Names of entries to the directory, in string format
 */
#define DN_ROOT   	"/"
#define DN_DE   	"/C=de"
#define DN_SNI  	"/C=de/O=sni"
#define DN_AP   	"/C=de/O=sni/OU=ap"
#define DN_STEFANIE  	"/C=de/O=sni/OU=ap/CN=stefanie"
#define DN_NORBERT   	"/C=de/O=sni/OU=ap/CN=norbert"
#define DN_INGRID    	"/C=de/O=sni/OU=ap/CN=ingrid"


/* 
 * Attributes, in string format
 */
#define OBJ_CLASS_C	"OCL = TOP; C"
#define OBJ_CLASS_O	"OCL = TOP; ORG"
#define OBJ_CLASS_OU	"OCL = TOP; OU"
#define OBJ_CLASS_OP	"OCL = TOP; PER; ORP"
#define ATT_PHONE_NUM	"TN = '+49 89 636 0' "
#define ATT_PASSWORD	"UP = secret"
#define ATT_SURNAME	"SN = Schmid"
#define ATT_ACL1  "ACL={MPUB = {INT = 1,USR = {/}}, \
		        RSTD = {INT = 0,USR = {/C=de/O=sni/OU=ap/CN=stefanie}},\
		        MSTD = {INT = 0,USR = {/C=de/O=sni/OU=ap/CN=stefanie}},\
		        RSEN = {INT = 0,USR = {/C=de/O=sni/OU=ap/CN=stefanie}},\
		        MSEN = {INT = 0,USR = {/C=de/O=sni/OU=ap/CN=stefanie}}}"
#define ATT_ACL2  "ACL={MPUB = {INT = 1,USR = {/C=de/O=sni/OU=ap}},\
		        RSTD = {INT = 1,USR = {/C=de/O=sni/OU=ap}},\
			MSTD = {INT = 0,USR = {/C=de/O=sni/OU=ap/CN=stefanie}},\
		        RSEN = {INT = 1,USR = {/C=de/O=sni/OU=ap}},\
		        MSEN = {INT = 0,USR = {/C=de/O=sni/OU=ap/CN=stefanie}}}"


/* Other strings 							*/ 
#define PASSWD		"secret"
#define FILTER		"OCL"


#define NO_OF_ENTRIES	6	/* 6 entries to be added		*/
#define NO_C_ATTRS	1	/* 1 attr in Country attribute list	*/
#define NO_O_ATTRS	2	/* 1 attr in Organization attribute list*/
#define NO_OU_ATTRS	1	/* 1 attr in Org-Unit attribute list	*/
#define NO_OP_ATTRS	5	/* 1 attr in Org-Person attribute list	*/


/* Build up an array of object identifiers for the optional packages    */
/* to be negotiated.                                                    */
DS_feature features[] = {
    { OM_STRING(OMP_O_DS_BASIC_DIR_CONTENTS_PKG), OM_TRUE },
    { OM_STRING(OMP_O_DSX_GDS_PKG), OM_TRUE },
    { 0 }
};


#endif /* _ACL2_H */
