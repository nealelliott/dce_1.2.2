/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl.c,v $
 * Revision 1.1.11.2  1996/02/18  00:24:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:37  marty]
 *
 * Revision 1.1.11.1  1995/12/08  22:12:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:09  root]
 * 
 * Revision 1.1.9.2  1994/10/05  16:44:13  keutel
 * 	OT 12431
 * 	[1994/10/05  16:36:24  keutel]
 * 
 * Revision 1.1.9.1  1994/02/22  16:39:55  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:33:33  marrek]
 * 
 * Revision 1.1.7.2  1993/08/12  12:45:47  marrek
 * 	July 1993 code drop.
 * 	[1993/08/04  10:59:30  marrek]
 * 
 * Revision 1.1.5.2  1993/01/05  20:00:01  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:17:30  htf]
 * 
 * Revision 1.1.2.4  1992/07/28  20:12:43  melman
 * 	GDS drop of 7/17/92
 * 	[1992/07/28  19:46:50  melman]
 * 
 * Revision 1.1.2.3  1992/07/01  03:25:24  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  23:03:59  melman]
 * 
 * Revision 1.1.2.2  1992/06/11  17:42:33  melman
 * 	New GDS test code drop
 * 	[1992/06/11  17:39:12  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: acl.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 00:24:13 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*
 *
 * This sample program displays the access permissions (ACL) on each 
 * entry in the directory for a specific user.  The permissions are 
 * presented in a form similiar to the UNIX file permissions. 
 * In addition, each entry is flagged as either a master
 * or a shadow copy.
 *
 * The distinguished name of the user performing the check is :
 *
 *     /C=de/O=sni/OU=ap/CN=norbert
 *
 * The results are presented in the following format : 
 *
 *     [ABCD] <entry's distinguished name>
 *
 *     A:  'm' master copy
 *         's' shadow copy
 *
 *     B:  'r' read access to public attributes
 *         'w' write access to public attributes
 *         '-' no access to public attributes
 *
 *     C:  'r' read access to standard attributes
 *         'w' write access to standard attributes
 *         '-' no access to standard attributes
 *
 *     D:  'r' read access to sensitive attributes
 *         'w' write access to sensitive attributes
 *         '-' no access to sensitive attributes
 *
 *
 * For example, the following result means that the entry '/C=de/O=sni'
 * is a master copy and that the requesting user
 * (/C=de/O=sni/OU=ap/CN=norbert) has write access to its public 
 * attributes, read access to its standard
 * attributes and no access to its sensitive attributes.
 *
 *      [mwr-] /C=de/O=sni
 * 
 *
 * 
 * The program requires that the specific user perform an authenticated
 * bind to the directory.  In order to achieve this the user's 
 * credentials must already exist in the directory.
 * Therefore the following tree of 6 entries is added to the directory 
 * each time the program runs, and removed again afterwards.
 *
 *
 *
 *              O  C=de
 *              |  (objectClass=Country,
 *              |   ACL=(mod-pub: *
 *              |        read-std:*
 *              |        mod-std: *
 *              |        read-sen:*
 *              |        mod-sen: *))
 *              |
 *              |
 *              O  O=sni
 *              |  (objectClass=Organization,
 *              |   ACL=(mod-pub: /C=de/O=sni/OU=ap/*
 *              |        read-std:/C=de/O=sni/OU=ap/CN=stefanie
 *              |        mod-std: /C=de/O=sni/OU=ap/CN=stefanie
 *              |        read-sen:/C=de/O=sni/OU=ap/CN=stefanie
 *              |        mod-sen: /C=de/O=sni/OU=ap/CN=stefanie))
 *              |
 *              O  OU=ap
 *              |  (objectClass=OrganizationalUnit,
 *              |   ACL=(mod-pub: /C=de/O=sni/OU=ap/*
 *              |        read-std:/C=de/O=sni/OU=ap/CN=stefanie
 *              |        mod-std: /C=de/O=sni/OU=ap/CN=stefanie
 *              |        read-sen:/C=de/O=sni/OU=ap/CN=stefanie
 *              |        mod-sen: /C=de/O=sni/OU=ap/CN=stefanie))
 *              |
 *      +-------+-------+
 *      |       |       |
 *      |       |       O  CN=ingrid
 *      |       |         (objectClass=OrganizationalPerson,
 *      |       |          ACL=(mod-pub: /C=de/O=sni/OU=ap/*
 *      |       |               read-std:/C=de/O=sni/OU=ap/*
 *      |       |               mod-std: /C=de/O=sni/OU=ap/CN=stefanie
 *      |       |               read-sen:/C=de/O=sni/OU=ap/*
 *      |       |               mod-sen: /C=de/O=sni/OU=ap/CN=stefanie),
 *      |       |          surname="Schmid",
 *      |       |          telephone="+49 89 636 0",
 *      |       |          userPassword="secret")
 *      |       |
 *      |       O  CN=norbert
 *      |          (objectClass=OrganizationalPerson,
 *      |           ACL=(mod-pub: /C=de/O=sni/OU=ap/*
 *      |                read-std:/C=de/O=sni/OU=ap/*
 *      |                mod-std: /C=de/O=sni/OU=ap/CN=stefanie
 *      |                read-sen:/C=de/O=sni/OU=ap/*
 *      |                mod-sen: /C=de/O=sni/OU=ap/CN=stefanie),
 *      |           surname="Schmid",
 *      |           telephone="+49 89 636 0",
 *      |           userPassword="secret")
 *      |
 *      O  CN=stefanie
 *         (objectClass=OrganizationalPerson,
 *          ACL=(mod-pub: /C=de/O=sni/OU=ap/*
 *               read-std:/C=de/O=sni/OU=ap/*
 *               mod-std: /C=de/O=sni/OU=ap/CN=stefanie
 *               read-sen:/C=de/O=sni/OU=ap/*
 *               mod-sen: /C=de/O=sni/OU=ap/CN=stefanie),
 *          surname="Schmid",
 *          telephone="+49 89 636 0",
 *          userPassword="secret")
 *
 *
 */

#ifdef THREADSAFE
#	include <pthread.h>
#endif
#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <xdsgds.h>
#include <xdscds.h>
#include "acl.h"           /* static initialization of data structures. */





void
main(
    int  argc,
    char *argv[]
)
{
    OM_workspace       workspace;     /* workspace for objects          */
    OM_private_object  session;       /* Session object.                */
    OM_private_object  bound_session; /* Holds the Session object which */
                                      /* is returned by ds_bind()       */
    OM_private_object  context;       /* Context object.                */
    OM_private_object  result;        /* Holds the search result object.*/
    OM_sint            invoke_id;     /* Integer for the invoke id      */
                                      /* returned by ds_search().       */
                                      /* (this parameter must be present*/
                                      /* even though it is ignored).    */
    OM_type            sinfo_list[] = { DS_SEARCH_INFO, 0 };
    OM_type            entry_list[] = { DS_ENTRIES, 0 };
                                      /* Lists of types to be extracted */
    OM_public_object   sinfo;         /* Search-Info object from result.*/
    OM_public_object   entry;         /* Entry object from search info. */
    OM_value_position  total_num;     /* Number of descriptors returned.*/
    OM_return_code     rc;            /* XOM function return code.      */
    register int       i;
    char               user_name[MAX_DN_LEN];
                                      /* Holds requestor's name.        */
    char               entry_string[MAX_DN_LEN + 7] = "[?r??] ";
                                      /* Holds entry details.           */

    /* Step 3 (see acl.h program code for Steps 1 and 2)
     *
     * Initialize a directory workspace for use by XOM.
     */
    if ((workspace = ds_initialize()) == (OM_workspace)0) {
        printf("ds_initialize() error\n");
	}


    /* Step 4
     *
     * Negotiate the use of the BDCP and GDS packages.
     */
    if (ds_version(features, workspace) != DS_SUCCESS) {
        printf("ds_version() error\n");
	}


    /* Step 5
     *
     * Add a fixed tree of entries to the directory in order to permit 
     * an authenticated bind by:   /C=de/O=sni/OU=ap/CN=norbert
     */
    if (! add_tree(workspace)) {
        printf("add_tree() error\n");
	}


    /* Step 6
     *
     * Create a default session object.
     */
    if ((rc = om_create(DSX_C_GDS_SESSION,OM_TRUE,workspace,&session))
           != OM_SUCCESS) {
        printf("om_create() error %d\n", rc);
	}


    /* Step 7
     *
     * Alter the default session object to include the following
     * credentials: requestor:  /C=de/O=sni/OU=ap/CN=norbert
     *              password:   "secret"
     *              authentication mechanism: simple
     */
    if ((rc = om_put(session, OM_REPLACE_ALL, credentials, 0 ,0, 0))
           != OM_SUCCESS) {
        printf("om_put() error %d\n", rc);
	}


    /* Step 8
     *
     * Bind with credentials to the default GDS server. The returned
     * session object is stored in the private object variable
     * bound_session and is used for all further XDS function calls.
     */
    if (ds_bind(session, workspace, &bound_session) != DS_SUCCESS) {
        printf("ds_bind() error\n");
	}


    /* Step 9
     *
     * Create a default context object.
     */
    if ((rc = om_create(DSX_C_GDS_CONTEXT,OM_TRUE,workspace,&context))
           != OM_SUCCESS) {
        printf("om_create() error %d\n", rc);
	}


    /*
     * Alter the default context object to include 'shadow' entries.
     */
    if ((rc = om_put(context, OM_REPLACE_ALL, use_copy, 0 ,0, 0)) 
           != OM_SUCCESS) {
        printf("om_put() error %d\n", rc);
	}


    /* Step 10
     *
     * Search the whole subtree below root.
     * The filter selects entries with an object-class attribute.
     * The selection extracts the ACL attribute from each selected entry.
     * The results are returned in the private object 'result'.
     *
     * NOTE: Since every entry contains an object-class attribute the
     *       filter performs no function other than to demonstrate how
     *       filters may be used.
     */
    if (ds_search(bound_session, context, dn_root, DS_WHOLE_SUBTREE,
           filter, OM_FALSE, selection_acl, &result, &invoke_id) 
           != DS_SUCCESS) {
        printf("ds_search() error\n");
	}


    /* Step 11
     *
     * Close the connection to the GDS server.
     */
    if (ds_unbind(bound_session) != DS_SUCCESS) {
        printf("ds_unbind() error\n");
	}


    /* Step 12
     *
     * Remove the user's credentials from the Directory.
     */
    if (! remove_tree(workspace, session)) {
        printf("remove_tree() error\n");
	}


    /* Step 13
     *
     * Extract components from the search result by means of om_get().
     */
    if ((rc = om_get(result,
              OM_EXCLUDE_ALL_BUT_THESE_TYPES + OM_EXCLUDE_SUBOBJECTS,
              sinfo_list, OM_FALSE, 0, 0, &sinfo, &total_num))
           != OM_SUCCESS) {
        printf("om_get(Search-Result) error %d\n", rc);
	}


    if ((rc = om_get(sinfo->value.object.object,
              OM_EXCLUDE_ALL_BUT_THESE_TYPES + OM_EXCLUDE_SUBOBJECTS,
              entry_list, OM_FALSE, 0, 0, &entry, &total_num))
           != OM_SUCCESS) {
         printf("om_get(Search-Info) error %d\n", rc);
	}


    /*
     * Convert the requestor's distinguished name to string format.
     */
    if (! xds_name_to_string(dn_norbert, user_name)) {
        printf("xds_name_to_string() error\n");
	}

    printf("User:  %s\nTotal: %d\n", user_name, total_num);

    /* Step 14
     *
     * Examine each entry and print the entry details.
     */
    for (i = 0; i < total_num; i++) {
        if (process_entry_info((entry+i)->value.object.object,
                           entry_string, user_name)) {
            printf("%s\n", entry_string);
		}
    }


    /* Step 15
     *
     * Close the directory workspace.
     */
    if (ds_shutdown(workspace) != DS_SUCCESS) {
        printf("ds_shutdown() error\n");
	}
}





/*
 * Add the tree of entries described above.
 */
int
add_tree(
    OM_workspace workspace
)
{
    OM_private_object session;        /* Holds the Session object which */
                                      /* is returned by ds_bind()       */
    OM_sint           invoke_id;      /* Integer for the invoke id      */
    int               error = 0;


/* Bind (without credentials) to the default GDS server.                */

    if (ds_bind(DS_DEFAULT_SESSION, workspace, &session) != DS_SUCCESS)
	error++;


/* Add entries to the GDS server.                                       */

    ds_add_entry(session, DS_DEFAULT_CONTEXT, dn_de, alist_C,
                  &invoke_id);

    if (ds_add_entry(session, DS_DEFAULT_CONTEXT, dn_sni, alist_O,
                                             &invoke_id) != DS_SUCCESS)
        error++;

    if (ds_add_entry(session, DS_DEFAULT_CONTEXT, dn_ap, alist_OU,
                                             &invoke_id) != DS_SUCCESS)
        error++;

    if (ds_add_entry(session, DS_DEFAULT_CONTEXT, dn_stefanie, alist_OP,
                                             &invoke_id) != DS_SUCCESS)
        error++;

    if (ds_add_entry(session, DS_DEFAULT_CONTEXT, dn_norbert, alist_OP,
                                             &invoke_id) != DS_SUCCESS)
        error++;

    if (ds_add_entry(session, DS_DEFAULT_CONTEXT, dn_ingrid, alist_OP,
                                             &invoke_id) != DS_SUCCESS)
        error++;


/* Close the connection to the GDS server.                              */

    if (ds_unbind(session) != DS_SUCCESS)
        error++;


    return (error?0:1);
}





/*
 * Remove the tree of entries described above.
 */
int
remove_tree(
    OM_workspace workspace,
    OM_private_object  session
)
{
    OM_private_object bound_session;  /* Holds the Session object which */
                                      /* is returned by ds_bind()       */
    OM_sint           invoke_id;      /* Integer for the invoke id      */
    int               error = 0;


/* Bind (with credentials) to the default GDS server.                   */

    if (ds_bind(session, workspace, &bound_session) != DS_SUCCESS)
        error++;


/* Remove entries from the GDS server.                                  */

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, dn_ingrid,
                                            &invoke_id) != DS_SUCCESS)
        error++;

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, dn_stefanie,
                                            &invoke_id) != DS_SUCCESS)
        error++;

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, dn_norbert,
                                            &invoke_id) != DS_SUCCESS)
        error++;

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, dn_ap,
                                            &invoke_id) != DS_SUCCESS)
        error++;

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, dn_sni,
                                            &invoke_id) != DS_SUCCESS)
        error++;

    ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, dn_de,
                     &invoke_id);


/* Close the connection to the GDS server.                              */

    if (ds_unbind(bound_session) != DS_SUCCESS)
        error++;


    return (error?0:1);
}





/*
 * Convert a distinguished name in XDS format (OM_descriptor 
 * lists) to string format.
 */
int
xds_name_to_string(
    OM_public_object  name,           /* Xds distinguished name.        */
    char             *string          /* String distinguished name.     */
)
{
    register OM_object  dn = name;
    register OM_object  rdn;
    register OM_object  ava;
    register char      *sp = string;
             int        error = 0;

    
    while ((dn->type != OM_NO_MORE_TYPES) && (! error)) {
        if ((dn->type == DS_RDNS) &&
            ((dn->syntax & OM_S_SYNTAX) == OM_S_OBJECT)) {
            rdn = dn->value.object.object; 

            while ((rdn->type != OM_NO_MORE_TYPES) && (! error)) {
                if ((rdn->type == DS_AVAS) &&
                    ((rdn->syntax & OM_S_SYNTAX) == OM_S_OBJECT)) {
                    ava = rdn->value.object.object; 

                    while ((ava->type != OM_NO_MORE_TYPES) && 
                             (! error)) {
                        if ((ava->type == DS_ATTRIBUTE_TYPE) &&
                            ((ava->syntax & OM_S_SYNTAX) == 
                                      OM_S_OBJECT_IDENTIFIER_STRING)) {

                            *sp++ = '/';
                            if (strncmp(ava->value.string.elements,
                                        DS_A_COUNTRY_NAME.elements,
                                        ava->value.string.length) == 0)
                                *sp++ = 'C';

                            else if (strncmp(ava->value.string.elements,
                                        DS_A_ORG_NAME.elements,
                                        ava->value.string.length) == 0)
                                *sp++ = 'O';

                            else if (strncmp(ava->value.string.elements,
                                        DS_A_ORG_UNIT_NAME.elements,
                                        ava->value.string.length) == 0)
                                *sp++ = 'O', *sp++ = 'U';

                            else if (strncmp(ava->value.string.elements,
                                        DS_A_COMMON_NAME.elements,
                                        ava->value.string.length) == 0)
                                *sp++ = 'C', *sp++ = 'N';

                            else if (strncmp(ava->value.string.elements,
                                        DS_A_LOCALITY_NAME.elements,
                                        ava->value.string.length) == 0)
                                *sp++ = 'L';

                            else if (strncmp(ava->value.string.elements,
                                        DSX_TYPELESS_RDN.elements,
                                        ava->value.string.length) != 0) {
                                error++;
                                continue;
                            }

                            if (*(sp-1) != '/');  /* no '=' if typeless */
                                *sp++ = '=';
                        }
                        if (ava->type == DS_ATTRIBUTE_VALUES) {
                            switch(ava->syntax & OM_S_SYNTAX) {
                                case OM_S_PRINTABLE_STRING : 
                                case OM_S_TELETEX_STRING : 
                                 strncpy(sp, ava->value.string.elements,
                                         ava->value.string.length);
                                 sp += ava->value.string.length;
                                 break;

                                default:
                                    error++;
                                    continue;
                            }

                        }
                        ava++;
                    }
                }
                rdn++;
            }
        }
        dn++;
    }
    *sp = '\0';

    return (error?0:1);
}





/*
 * Extract information about an entry from the Entry-Info object: 
 * whether the entry is a master-copy, its ACL permissions
 * and its distinguished name.
 * Build up a string based on this information.
 */
int
process_entry_info(
    OM_private_object  entry,
    char              *entry_string,
    char              *user_name
)
{
    OM_return_code     rc;            /* Return code from XOM function. */
    OM_public_object   ei_attrs;      /* Components from Entry-Info.    */
    OM_public_object   attr;          /* Directory attribute.           */
    OM_public_object   acl;           /* ACL attribute value.           */
    OM_public_object   acl_item;      /* ACL item component.            */
    OM_value_position  total_attrs;   /* Number of attributes returned. */
    register int       i;
    register int       interp;
    register int       error = 0;
    register int       found_acl = 0;
    static OM_type     ei_attr_list[] = { DS_FROM_ENTRY,
                                          DS_OBJECT_NAME,
                                          DS_ATTRIBUTES,
                                          0 };
                                      /* Attributes to be extracted.    */


    /*
     * Extract three attributes from each Entry-Info object.
     */
    if ((rc = om_get(entry, OM_EXCLUDE_ALL_BUT_THESE_TYPES,
                     ei_attr_list, OM_FALSE, 0, 0, &ei_attrs, &total_attrs))
                                                                != OM_SUCCESS) {
        error++;
        printf("om_get(Entry-Info) error %d\n", rc);
    }


    for (i = 0; ((i < total_attrs) && (! error)); i++, ei_attrs++) {

        /*
         * Determine if current entry is a master-copy or a shadow-copy.
         */
        if ((ei_attrs->type == DS_FROM_ENTRY) &&
            ((ei_attrs->syntax & OM_S_SYNTAX) == OM_S_BOOLEAN))
            if (ei_attrs->value.boolean == OM_TRUE)
                entry_string[1] = 'm';
            else if (ei_attrs->value.boolean == OM_FALSE)
                     entry_string[1] = 's';
                 else
                     entry_string[1] = '?';


        if ((ei_attrs->type == DS_ATTRIBUTES) &&
            ((ei_attrs->syntax & OM_S_SYNTAX) == OM_S_OBJECT)) {
            attr = ei_attrs->value.object.object; 

            while ((attr->type != OM_NO_MORE_TYPES) && (! error)) {

                /*
                 * Check that the attribute is an ACL attribute.
                 */
                if ((attr->type == DS_ATTRIBUTE_TYPE) &&
                    ((attr->syntax & OM_S_SYNTAX) ==
                                               OM_S_OBJECT_IDENTIFIER_STRING)) {
                    if (strncmp(attr->value.string.elements,
                                DSX_A_ACL.elements,
                                attr->value.string.length) == 0)
                        found_acl++;
                }

                /*
                 * Examine the ACL. Check each permission for
                 * the current user.
                 */
                if ((found_acl) &&
                    (attr->type == DS_ATTRIBUTE_VALUES) &&
                    ((attr->syntax & OM_S_SYNTAX) == OM_S_OBJECT)) {

                    acl = attr->value.object.object;

                    entry_string[2] = 'r';
                    entry_string[3] = '-';
                    entry_string[4] = '-';

                    while (acl->type != OM_NO_MORE_TYPES) {

                        if ((acl->syntax & OM_S_SYNTAX) == OM_S_OBJECT)
                            acl_item = acl->value.object.object;

                        switch (acl->type) {

                        case OM_CLASS:
                            break;

                        case DSX_MODIFY_PUBLIC:
                            if (permitted_access(user_name, acl_item))
                                entry_string[2] = 'w';
                            break;

                        case DSX_READ_STANDARD:
                            if (permitted_access(user_name, acl_item))
                                entry_string[3] = 'r';
                            break;

                        case DSX_MODIFY_STANDARD:
                            if (permitted_access(user_name, acl_item))
                                entry_string[3] = 'w';
                            break;

                        case DSX_READ_SENSITIVE:
                            if (permitted_access(user_name, acl_item))
                                entry_string[4] = 'r';
                            break;

                        case DSX_MODIFY_SENSITIVE:
                            if (permitted_access(user_name, acl_item))
                                entry_string[4] = 'w';
                            break;
                        }
                        acl++;
                    }
                }
                attr++;
            }
        }


        /*
         * Convert the entry's distinguished name to a string format.
         */
        if ((ei_attrs->type == DS_OBJECT_NAME) &&
            ((ei_attrs->syntax & OM_S_SYNTAX) == OM_S_OBJECT))
            if (! xds_name_to_string(ei_attrs->value.object.object,
                                     &entry_string[7])) {
                error++;
                printf("xds_name_to_string() error\n");
            }

    }

    return (error?0:1);
}





/*
 * Check if a user is permitted access based on the ACL supplied.
 *
 */
int
permitted_access(
    char             *user_name,
    OM_public_object  acl_item
)
{
    char  acl_name[MAX_DN_LEN];
    int   interpretation;
    int   acl_present = 0;
    int   access = 0;
    int   acl_name_length;


    while (acl_item->type != OM_NO_MORE_TYPES) {

        switch (acl_item->type) {
        case OM_CLASS:
            break;
        
        case DSX_INTERPRETATION:
            interpretation = acl_item->value.boolean; 
            break;

        case DSX_USER:
            xds_name_to_string(acl_item->value.object.object, acl_name);

            if (interpretation == DSX_SINGLE_OBJECT) {
                if (strcmp(acl_name, user_name) == 0)
                    access = 1;
            }
            else if (interpretation == DSX_ROOT_OF_SUBTREE) {
                     if ((acl_name_length = strlen(acl_name)) == 0)
                         access = 1;
                     else if (strncmp(acl_name,user_name,
                               acl_name_length) == 0)
                         access = 1;
            }
            break;
        }
        acl_item++;
    }

    return (access);
}
