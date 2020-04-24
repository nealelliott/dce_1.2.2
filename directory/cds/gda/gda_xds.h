/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_xds.h,v $
 * Revision 1.1.4.2  1996/02/18  23:33:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:13:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:39  root]
 * 
 * Revision 1.1.2.5  1994/08/16  18:27:46  jd
 * 	drop from janet
 * 	[1994/08/13  16:05:18  jd]
 * 
 * Revision 1.1.2.4  1994/08/03  19:01:45  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:54:01  mccann]
 * 
 * Revision 1.1.2.3  1994/06/23  18:30:11  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:28:20  mccann]
 * 
 * Revision 1.1.2.2  1994/06/09  18:38:14  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:29  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/12  22:01:52  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:10:15  peckham]
 * 
 * $EndLog$
 */
#ifndef _GDA_XDS_H
#define _GDA_XDS_H

/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1993-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 */


/*
 * gda_xds.h
 */

#include <dce/cdsclerk.h>
#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <stdlib.h> /* for EXIT_FAILURE and EXIT_SUCCESS */
#include <dce/x500_name_utils.h>

#define X500_NOERROR       0                 /* Function return code         */
#define X500_ERROR        -1                 /* Function return code         */

#define X500_TOWER_LEN_MAX 500
#define X500_REPLICA_MAX 10

/* Define OIDs for the CDS_CELL and CDS_REPLICAS attributes 
*/
#define OMP_O_DS_A_CDS_CELL        "\053\014\002\210\123\001\003\004\015"
                                   /* 1.3.12.2.1107.1.3.4.13 */
#define OMP_O_DS_A_CDS_REPLICAS    "\053\014\002\210\123\001\003\004\016"
                                   /* 1.3.12.2.1107.1.3.4.14 */
/* OID for dceCellInfo object class for DEC shema */
#define OMP_O_DS_O_CELL_INFO       "\053\030\012\004\020"
                                   /* 1.3.24.10.4.16         */


#ifndef OMX_CLASS_DESC
/* This macro sets an OID class descriptor */
#define OMX_CLASS_DESC(desc,oidstr) { desc.type = OM_CLASS; \
                              desc.syntax = OM_S_OBJECT_IDENTIFIER_STRING; \
                              desc.value.string = oidstr; }
#endif

#ifndef OMX_BOOLEAN_DESC
/* This macro sets a boolean value descriptor for the given type */
#define OMX_BOOLEAN_DESC(desc,type_name,bool) { desc.type = type_name; \
                                     desc.syntax = OM_S_BOOLEAN; \
                                     desc.value.boolean = bool; }
#endif

#ifndef OMX_ENUM_DESC
/* This macro sets an enumerated value descriptor for the given type */
#define OMX_ENUM_DESC(desc,type_name,enum) { desc.type = type_name; \
                                     desc.syntax = OM_S_ENUMERATION; \
                                     desc.value.enumeration = enum; }
#endif

#ifndef OMX_INTEGER_DESC
/* This macro sets an integer value descriptor for the given type */
#define OMX_INTEGER_DESC(desc,type_name,integr) { desc.type = type_name; \
                                     desc.syntax = OM_S_INTEGER; \
                                     desc.value.integer = integr; }
#endif

#ifndef OMX_OBJECT_DESC
/* This macro sets a sub-object descriptor for the given type */
#define OMX_OBJECT_DESC(desc,type_name,obj) { desc.type = type_name; \
                                     desc.syntax = OM_S_OBJECT; \
                                     desc.value.object.object = obj; }
#endif
#ifndef OMX_OM_NULL_DESC
/* This macro sets the null descriptor */
#define OMX_OM_NULL_DESC(desc) { desc.type = OM_NO_MORE_TYPES; \
                           desc.syntax = OM_S_NO_MORE_SYNTAXES; \
			   desc.value.string.length = 0;	\
			   desc.value.string.elements = OM_ELEMENTS_UNSPECIFIED; }
#endif


#ifndef OMX_ATTR_TYPE_DESC
/* This macro sets an attribute type descriptor using the given OID string */
#define OMX_ATTR_TYPE_DESC(desc, ds_type, oidstr) { desc.type = ds_type; \
                             desc.syntax = OM_S_OBJECT_IDENTIFIER_STRING; \
                             desc.value.string = oidstr; }
#endif


#ifndef OMX_ZSTRING_DESC
/* This macro sets a string descriptor given a null (zero) terminated string
   using the given type name */
#define OMX_ZSTRING_DESC(desc,syntx,type_name,str) { desc.type = type_name; \
                                desc.syntax = syntx; \
                                desc.value.string.elements = (void *)str; \
                                desc.value.string.length = strlen(str); }
#endif

#ifndef OMX_STRING_DESC
/* This macro sets a string descriptor given the length and elements pointer
   and using the given type name */
#define OMX_STRING_DESC(desc,syntx,type_name,str,len) { desc.type = type_name;\
                                desc.syntax = syntx; \
                                desc.value.string.elements = (void *)str; \
                                desc.value.string.length = len; }
#endif

/* Example:

Header files:

   #include <xom.h>
   #include <xds.h>
   #include <xdsbdcp.h>

Declarations:

   OM_EXPORT(DS_A_SURNAME)
   OM_EXPORT(DS_A_TITLE)

   OM_descriptor cpub_eis[5];

Assignment:

   OMX_CLASS_DESC(       cpub_eis[0], DS_C_ENTRY_INFO_SELECTION);
   OMX_ATTR_TYPE_DESC(   cpub_eis[1], DS_ATTRIBUTES_SELECTED,DS_A_SURNAME);
   OMX_ATTR_TYPE_DESC(   cpub_eis[2], DS_ATTRIBUTES_SELECTED,DS_A_TITLE);
   OMX_ENUM_DESC(        cpub_eis[3], DS_INFO_TYPE, DS_TYPES_ONLY);
   OMX_OM_NULL_DESC(     cpub_eis[4]);

*/ 


/*
 * declare an error handling function and an error checking macro for DS
 */
void 
report_ds_error(
    char      *, 
    DS_status  );

#define CHECK_DS_CALL(function_call, function)		\
		error = (function);	\
		if (error != DS_SUCCESS) {	\
		    report_ds_error(function_call, error); \
		    return(X500_ERROR); \
		}
/*
 * declare an error handling function and an error checking macro for OM
 */
void 
report_om_error(
     char           *, 
     OM_return_code  );

#define CHECK_OM_CALL(function_call, function)		\
		return_code = (function);	\
		if (return_code != OM_SUCCESS) {	\
                    report_om_error(function_call, return_code); \
                    return (X500_ERROR); \
		}
/*
 * declare the visible routines in x500_utils.c
 */
int 
X500_name_to_object(
    char               *, 
    OM_private_object  *,
    OM_workspace        );

/*
 * declare visible routines in x500_xdsint.c
 */
int
x500_xds_lookup(
    char              *,
    x500_dns_cell_t   *,
    x500_replica_t    [],
    char              **,
    char              *[]);

/*
 * prototype from x500process.c
 */
int
is_X500 (
   cds_FullName_t *);

int
X500_process (
   question_el_t *);

int
is_X500 (
   cds_FullName_t *);

int
init_X500 (
   void);

int
init_X500_process (
   question_el_t   *);

int
destroy_X500_process (
   question_el_t   *);


#endif /* ifndef _GDA_XDS_H */
