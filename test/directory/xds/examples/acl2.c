/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl2.c,v $
 * Revision 1.1.4.2  1996/02/18  00:24:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:12:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:13  root]
 * 
 * Revision 1.1.2.2  1994/10/05  16:44:16  keutel
 * 	OT 12431
 * 	[1994/10/05  16:36:35  keutel]
 * 
 * Revision 1.1.2.1  1994/09/14  09:46:35  marrek
 * 	Initial version.
 * 	[1994/09/13  12:47:47  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: acl2.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 00:24:15 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*
 *
 * This sample program displays the access permissions (ACL) on each entry
 * in the directory for a specific user.  The permissions are presented in
 * a form similiar to the UNIX file permissions.  In addition, each entry
 * is flagged as either a master or a shadow copy.
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
 * For example, the following result means that the entry '/C=de/O=sni' is
 * a master copy and that the requesting user (/C=de/O=sni/OU=ap/CN=norbert)
 * has write access to its public attributes, read access to its standard
 * attributes and no access to its sensitive attributes.
 *
 *      [mwr-] /C=de/O=sni
 * 
 *
 * 
 * The program requires that the specific user perform an authenticated bind
 * to the directory.  In order to achieve this the user's credentials must
 * already exist in the directory.  Therefore the following tree of 6 entries
 * is added to the directory each time the program runs, and removed again
 * afterwards.
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
 * In this version of the program, instead of providing client-generated 
 * public objects, the XOM Convenience Functions are used for creating
 * objects. They are also used for extracting information from service
 * generated objects.
 */

#ifdef THREADSAFE
#	include <pthread.h>
#endif
#include <stdio.h>
#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <xdsgds.h>
#include <xdscds.h>
#include <xdsext.h>
#include <xomext.h>
#include "acl2.h"


void
main(
    int  argc,
    char *argv[]
)
{
    OM_workspace       workspace;     /* Workspace for objects          */
    OM_private_object  session;       /* Session object.                */
    OM_private_object  bound_session; /* Holds the Session object which */
                                      /* is returned by ds_bind()       */
    OM_private_object  context;       /* Context object.                */
    OM_private_object  result;        /* Holds the search result object.*/
    OM_sint            invoke_id;     /* Integer for the invoke id      */
                                      /* returned by ds_search().       */
                                      /* (this parameter must be present*/
                                      /* even though it is ignored).    */
    OM_type            navigation_path[] = { DS_SEARCH_INFO, 0 };
				      /* List of OM types to the target */
				      /* object - of the search result  */
    OM_type            entry_list[] = { DS_ENTRIES, 0 };
                                      /* List of types to be extracted  */
    OM_public_object   entry;         /* Entry object from search info. */
    OM_value_position  total_num;     /* Number of descriptors returned.*/
    OM_return_code     rc;            /* XOM function return code.      */
    register int       i;
    char               user_name[MAX_DN_LEN] = DN_NORBERT;
                                      /* Holds requestor's name -	*/
                                      /* "/C=de/O=sni/OU=ap/CN=norbert" */
    char               entry_string[MAX_DN_LEN + 7] = "[?r??] ";
                                      /* Holds entry details.           */
    struct entry       entry_array[6];/* List of entry names and attrs	*/
    OM_object	       credentials;   /* Credentials part of session obj*/
    OM_object	       use_copy;      /* Specifies whether to use shadow*/
				      /* entries, in context object	*/
    OM_object	       filter;        /* Filter - for search operation  */
    OM_object	       dn_root;	      /* Name object for "/"		*/
    OM_object	       selection_acl; /* Entry Information Selection obj*/

    static char	       *name_list[]    = { DN_DE, DN_SNI, DN_AP,
        				   DN_STEFANIE, DN_NORBERT, DN_INGRID };
				      /* Array of names to be added     */

    static char        *C_attr_list[]  = { OBJ_CLASS_C };
    static char        *O_attr_list[]  = { OBJ_CLASS_O, ATT_ACL1 }; 
    static char        *OU_attr_list[] = { OBJ_CLASS_OU };
    static char        *OP_attr_list[] = { OBJ_CLASS_OP, ATT_ACL2, ATT_SURNAME, 
					   ATT_PHONE_NUM, ATT_PASSWORD };
				      /* Attribute lists, in string fmt */

    static char	       *dn_root_str    = DN_ROOT;
    static char	       *filter_str     = FILTER;



    /* Step 3 (see acl2.h program code for Step 1 and 2)
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
     * Build name objects for entries to be added to the directory.
     */
    for (i = 0; i < NO_OF_ENTRIES; i++) {
		if (!build_name_object(workspace,name_list[i],
		&(entry_array[i].name))) {
			printf("build_name_object() error\n");
		}
	}


    /* Step 6
     *
     * Build attribute objects for entries to be added to the directory
     */
    if ((! build_attr_list_object(workspace, NO_C_ATTRS, C_attr_list, 
				 		&entry_array[0].attr_list)) ||
        (! build_attr_list_object(workspace, NO_O_ATTRS, O_attr_list, 
						&entry_array[1].attr_list)) ||
        (! build_attr_list_object(workspace, NO_OU_ATTRS, OU_attr_list, 
						&entry_array[2].attr_list)) ||
        (! build_attr_list_object(workspace, NO_OP_ATTRS, OP_attr_list, 
						&entry_array[3].attr_list))) {
		printf("build_attr_list_object() error\n");
	}

    /*  
     * These entries have the OP attribute list also
     */
    entry_array[4].attr_list = entry_array[3].attr_list;
    entry_array[5].attr_list = entry_array[3].attr_list;


    /* Step 7
     *
     * Add a fixed tree of entries to the directory in order to permit an
     * authenticated bind by:   /C=de/O=sni/OU=ap/CN=norbert
     */
    if (! add_tree(workspace, entry_array, NO_OF_ENTRIES)) {
        printf("add_tree() error\n");
	}


    /* Step 8
     *
     * Create a default session object.
     */
    if ((rc = om_create(DSX_C_GDS_SESSION,OM_TRUE,workspace,&session))
                                                            != OM_SUCCESS){ 
        printf("om_create() error %d\n", rc);
	}


    /* Step 9
     *
     * Build an object with the following credentials:
     *  requestor:  /C=de/O=sni/OU=ap/CN=norbert
     *  password:   "secret"
     *  authentication mechanism: simple
     */
    if (! build_credentials_object(entry_array[4].name,&credentials)) {
        printf("build_credentials_object() error\n");
	}


    /*
     * Alter the default session object to include the credentials
     */
    if ((rc = om_put(session, OM_REPLACE_ALL, credentials, 0 ,0, 0))
                                                 != OM_SUCCESS) {
        printf("om_put() error %d\n", rc);
	}


    /* Step 10
     *
     * Bind with credentials to the default GDS server.
     * The returned session object is stored in the private object variable
     * bound_session and is used for all further XDS function calls.
     */
    if (ds_bind(session, workspace, &bound_session) != DS_SUCCESS) {
        printf("ds_bind() error\n");
	}


    /* Step 11
     *
     * Create a default context object.
     */
    if ((rc = om_create(DSX_C_GDS_CONTEXT,OM_TRUE,workspace,&context))
                                                        != OM_SUCCESS) {
        printf("om_create() error %d\n", rc);
	}


    /* 
     * Build an object specifying that shadow entries should be used
     */
    if (! build_use_copy_object(&use_copy)) {
        printf("build_use_copy_object() error\n");
	}


    /*
     * Alter the default context object to include 'shadow' entries.
     */
    if ((rc = om_put(context, OM_REPLACE_ALL, use_copy,0,0,0)) != OM_SUCCESS) {
        printf("om_put() error %d\n", rc);
	}


    /* Step 12
     *
     * Build a filter object, specifying presence of an object class attr.
     */
    if (! build_filter_object(workspace, filter_str, &filter)) {
        printf("build_filter_object() error\n");
	}


    /* 
     * Build a root name object, name = "/"
     */
    if (! build_name_object(workspace, dn_root_str, &dn_root)) {
        printf("build_name_object() error\n");
	}


    /* 
     * Build an entry information selection object, selecting acl attributes
     */
    if (! build_selection_object(&selection_acl)) {
        printf("build_selection_object() error\n");
	}


    /* Step 13
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
    filter, OM_FALSE, selection_acl, &result, &invoke_id) != DS_SUCCESS) {
        printf("ds_search() error\n");
	}


    /* Step 14
     *
     * Close the connection to the GDS server.
     */
    if (ds_unbind(bound_session) != DS_SUCCESS) {
        printf("ds_unbind() error\n");
	}


    /* Step 15
     *
     * Remove the user's credentials from the directory.
     */
    if (! remove_tree(workspace, session, entry_array, NO_OF_ENTRIES)) {
        printf("remove_tree() error\n");
	}


    /* Step 16
     *
     * Free the name and attribute objects
     * which make up the directory entries.
     */
    if (! free_entry_list(entry_array)) {
		printf("free_entry_list() error\n");
	}


    /*
     * Free public objects which were created.
     */
    free(selection_acl);
    free(use_copy);
    free(credentials);
    if ((om_delete(filter) != OM_SUCCESS)
	|| (om_delete(dn_root) != OM_SUCCESS)) {
		printf("om_delete() error\n");
	}


    /* Step 17
     *
     * Extract components from the search result by means of the XOM
     * Convenience Function, omX_extract()
     */
    if ((rc = omX_extract(result, navigation_path,
	OM_EXCLUDE_ALL_BUT_THESE_TYPES + OM_EXCLUDE_SUBOBJECTS,
	entry_list, OM_FALSE, 0, 0, &entry, &total_num)) != OM_SUCCESS) {
        printf("omX_extract(Search-Result) error %d\n", rc);
	}


    /*
     * Requestor's name = "/C=de/O=sni/OU=ap/CN=norbert"
     */
    printf("User:  %s\nTotal: %d\n", user_name, total_num);


    /* Step 18
     *
     * Examine each entry and print the entry details.
     */
    for (i = 0; i < total_num; i++) {
        if (process_entry_info((entry+i)->value.object.object,
		entry_string, user_name)) {
            printf("%s\n", entry_string);
		}
    }


    /*
     * Now free the entry object (returned from omX_extract() )
     */
    if (om_delete(entry) != OM_SUCCESS) {
		printf("om_delete() error\n");
	}


    /* Step 19
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
    OM_workspace workspace,
    struct entry elist[],
    int		 no_entries
)
{
    OM_private_object  session;       /* Holds the Session object which */
                                      /* is returned by ds_bind()       */
    OM_sint            invoke_id;     /* Integer for the invoke id      */
    int                error = 0;
    int                i;


    /* 
     * Bind (without credentials) to the default GDS server
     */
    if (ds_bind(DS_DEFAULT_SESSION, workspace, &session) != DS_SUCCESS)
        error++;

    /* 
     * Add entries to the GDS server
     */
    for (i = 0; i < no_entries; i++) 
    	if (ds_add_entry(session, DS_DEFAULT_CONTEXT, elist[i].name, 
			elist[i].attr_list, &invoke_id) != DS_SUCCESS) {
	    /* Ignore error if adding country - possibly already there 	*/
	    if (i != 0) error++;
        }

    /*
     * Close the connection to the GDS server
     */
    if (ds_unbind(session) != DS_SUCCESS)
        error++;

    return (error?0:1);
}




/*
 * Remove the tree of entries described above.
 */
int
remove_tree(
    OM_workspace       workspace,
    OM_private_object  session,
    struct entry       elist[],
    int		       no_entries
)
{
    OM_private_object bound_session;  /* Holds the Session object which */
                                      /* is returned by ds_bind()       */
    OM_sint           invoke_id;      /* Integer for the invoke id      */
    int		      i;
    int               error = 0;


    /* 
     * Bind (without credentials) to the default GDS server
     */
    if (ds_bind(session, workspace, &bound_session) != DS_SUCCESS)
        error++;

    /* 
     * Remove entries from the GDS server
     */
    for (i = no_entries-1; i >= 0; i--) 
    	if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, elist[i].name, 
						    &invoke_id) != DS_SUCCESS) {
	    /* Ignore error if removing country - possibly has entries  */
	    /* below it							*/
	    if (i != 0) error++;
	}


    /*
     * Close the connection to the GDS server
     */
    if (ds_unbind(bound_session) != DS_SUCCESS)
        error++;

    return (error?0:1);
}




/*
 * Extract information about an entry from the Entry-Info object: whether
 * the entry is a master-copy, its ACL permissions and its distinguished name.
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
    OM_public_object   acl;           /* ACL attribute parts.           */
    OM_public_object   acl_vals;      /* ACL attribute value.           */
    OM_public_object   acl_item;      /* ACL item component.            */
    OM_value_position  total_attrs;   /* Number of attributes returned. */
    OM_value_position  total_acls;    /* Number of acl values returned. */
    register int       i;
    register int       interp;
    register int       error = 0;
    register int       found_acl = 0;
    static OM_type     ei_attr_list[] = { DS_FROM_ENTRY,
                                          DS_OBJECT_NAME,
                                          0 };
                                      /* Attributes to be extracted.    */
    OM_string	       entry_str;


    /*
     * Extract occurences of DS_FROM_ENTRY, DS_OBJECT_NAME and from each 
     * Entry-Info object.
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

        /*
         * Convert the entry's distinguished name to a string format.
         */
	entry_str.elements = &entry_string[7];
	entry_str.length = MAX_DN_LEN;
        if ((ei_attrs->type == DS_OBJECT_NAME) &&
            ((ei_attrs->syntax & OM_S_SYNTAX) == OM_S_OBJECT)) 
            if ((rc = omX_object_to_string(ei_attrs->value.object.object, 
					OM_FALSE, &entry_str)) != OM_SUCCESS) {
                error++;
                printf("omX_object_to_string() error\n");
            }
    }


    /*
     * Now extract occurences of attributes, where the attribute type is ACL
     * from the Entry-Info object.
     */
    dsX_extract_attr_values(entry, DSX_A_ACL, OM_TRUE, &acl_vals, &total_acls);

    for (i = 0; ((i < total_acls) && (! error)); i++) {
        acl = acl_vals[i].value.object.object;

        /*
         * Examine the ACL. Check each permission for the current user.
         */

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
    /*
     * Now free acl_vals object (returned from dsX_extract_attr_values() )
     */
    if (total_acls > 0) 
    	if ((rc = om_delete(acl_vals)) != OM_SUCCESS) {
	    error++;
	    printf("om_delete() error, rc = %d\n", rc);
    	}
 

    return (error?0:1);
}




/*
 * Check if a user is permitted access based on the ACL supplied.
 */
int
permitted_access(
    char             *user_name,
    OM_public_object  acl_item
)
{
    char  	       acl_name[MAX_DN_LEN];
    OM_string 	       acl_name_str;
    int   	       interpretation;
    int   	       acl_present = 0;
    int   	       access = 0;
    int   	       acl_name_length;
    OM_return_code     rc;

    while (acl_item->type != OM_NO_MORE_TYPES) {

        switch (acl_item->type) {
        case OM_CLASS:
            break;
        
        case DSX_INTERPRETATION:
            interpretation = acl_item->value.boolean; 
            break;

        case DSX_USER:
            acl_name_str.elements = acl_name;
            if ((rc = omX_object_to_string(acl_item->value.object.object, 
				OM_FALSE, &acl_name_str)) == OM_SUCCESS) {
            	if (interpretation == DSX_SINGLE_OBJECT) {
                    if (strcmp(acl_name, user_name) == 0)
                        access = 1;
            	}
            	else if (interpretation == DSX_ROOT_OF_SUBTREE) {
                         if ((acl_name_length = strlen(acl_name)) == 0)
                             access = 1;
                         else if (strncmp(acl_name,user_name,acl_name_length) 
									  == 0)
                             access = 1;
                }
	    }
            break;
        }
        acl_item++;
    }

    return (access);
}




/*
 * Build a name object from a name string using the XOM Convenience Function 
 * omX_string_to_object()
 */
int
build_name_object(
    OM_workspace 	workspace,
    char	 	*name,
    OM_private_object   *name_obj
)
{
    OM_integer        err_pos;
    OM_integer        err_type;
    OM_return_code    rc;
    OM_string	      name_str;
    int               error = 0;

    name_str.length = strlen(name);
    name_str.elements = name;
    if ((rc = omX_string_to_object(workspace, &name_str, DS_C_DS_DN, 
			OM_TRUE, name_obj, &err_pos, &err_type)) != OM_SUCCESS)
	error++;

    return (error?0:1);
}



/*
 * Build an attribute list object given a list of attribute strings
 * Use the XOM Convenience Function omX_string_to_object() to build an 
 * attribute object from an attribute string, and omX_fill() to create the
 * other OM descriptor required
 */
int
build_attr_list_object(
    OM_workspace 	workspace,
    OM_integer	 	no_attrs,
    char	 	*attr_str_array[],
    OM_object	 	*attr_list_obj
)
{
    OM_integer        err_pos;
    OM_integer        err_type;
    OM_object	      attr;
    OM_object	      alist;
    OM_string	      attr_str;
    OM_return_code    rc;
    OM_descriptor     null_desc = OM_NULL_DESCRIPTOR;
    int               error = 0;
    int		      i;

    /* 
     * Allocate space for class descriptor, null descriptor and one descriptor
     * for each attribute 
     */
    if ((alist = (OM_descriptor *)malloc((2+no_attrs) * sizeof(OM_descriptor))) 
									== 0)
	error++;

    if ((rc = omX_fill_oid(OM_CLASS, DS_C_ATTRIBUTE_LIST, &alist[0]))
								!= OM_SUCCESS)
	error++;

    for (i = 1; i <= no_attrs; i++) {

    	attr_str.length = strlen(attr_str_array[i-1]);
    	attr_str.elements = attr_str_array[i-1];
    	if ((rc = omX_string_to_object(workspace, &attr_str, DS_C_ATTRIBUTE,
		    	OM_TRUE, &attr, &err_pos, &err_type)) != OM_SUCCESS)
	    error++;

    	if ((rc = omX_fill(DS_ATTRIBUTES, OM_S_OBJECT, 0, attr, &alist[i]))
								!= OM_SUCCESS)
	error++;
    }

    alist[i] = null_desc;

    *attr_list_obj = alist;
    return (error?0:1);
}




/*
 * Build an entry info selection object using the XOM Convenience Functions
 * omX_fill() and omX_fill_oid() to fill the OM descriptors
 */
int
build_selection_object(
    OM_object	 *selection_obj
)
{
    OM_integer        err_pos;
    OM_integer        err_type;
    OM_object	      desc;
    OM_object	      sel;
    OM_return_code    rc;
    OM_descriptor     null_desc = OM_NULL_DESCRIPTOR;
    int               error = 0;

    /* 
     * Allocate space for class descriptor, null descriptor and one descriptor
     * for each attribute 
     */
    if ((sel = (OM_descriptor *)malloc((5) * sizeof(OM_descriptor))) == 0)
	error++;

    if ((rc = omX_fill_oid(OM_CLASS, DS_C_ENTRY_INFO_SELECTION, &sel[0]))
								 != OM_SUCCESS)
	error++;

    if ((rc = omX_fill(DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE, 0, &sel[1]))
								 != OM_SUCCESS)
	error++;

    if ((rc = omX_fill_oid(DS_ATTRIBUTES_SELECTED, DSX_A_ACL, &sel[2]))
								 != OM_SUCCESS)
	error++;

    if ((rc = omX_fill(DS_INFO_TYPE, OM_S_ENUMERATION, DS_TYPES_AND_VALUES, 0,
						  	&sel[3])) != OM_SUCCESS)
	error++;

    sel[4] = null_desc;

    *selection_obj = sel;
    return (error?0:1);
}





/*
 * Build a credentials object using the XOM Convenience Function omX_fill()
 */
int
build_credentials_object(
    OM_object	 name,
    OM_object	 *credentials_obj
)
{
    OM_integer        err_pos;
    OM_integer        err_type;
    OM_object	      cred;
    OM_return_code    rc;
    OM_descriptor     null_desc = OM_NULL_DESCRIPTOR;
    int               error = 0;

    /* 
     * Just allocate space for a null descriptor and two other descriptors,
     * no class descriptor required
     */
    if ((cred = (OM_descriptor *)malloc((4) * sizeof(OM_descriptor))) == 0)
	error++;

    if ((rc = omX_fill(DS_REQUESTOR, OM_S_OBJECT, 0, name, &cred[0]))
								!= OM_SUCCESS)
	error++;

    if ((rc = omX_fill(DSX_PASSWORD, OM_S_OCTET_STRING, (sizeof(PASSWD)-1), 
					     PASSWD, &cred[1])) != OM_SUCCESS)
	error++;

    if ((rc = omX_fill(DSX_AUTH_MECHANISM, OM_S_ENUMERATION, DSX_SIMPLE,
					 0, &cred[2])) != OM_SUCCESS)
        error++;

    cred[3] = null_desc;

    *credentials_obj = cred;
    return (error?0:1);
}




/*
 * Build an object setting DS_DONT_USE_COPY to FALSE, using the XOM Convenience
 * Function omX_fill()
 */
int
build_use_copy_object(
    OM_object	 *use_copy_obj
)
{
    OM_integer        err_pos;
    OM_integer        err_type;
    OM_object	      desc;
    OM_object	      copy;
    OM_return_code    rc;
    OM_descriptor     null_desc = OM_NULL_DESCRIPTOR;
    int               error = 0;

    /* 
     * Just allocate space for a null descriptor and one other descriptor,
     * no class descriptor required
     */
    if ((copy = (OM_descriptor *)malloc((2) * sizeof(OM_descriptor))) == 0)
	error++;

    if ((rc = omX_fill(DS_DONT_USE_COPY, OM_S_BOOLEAN, OM_FALSE, 0, &copy[0]))
								!= OM_SUCCESS)
	error++;

    copy[1] = null_desc;

    *use_copy_obj = copy;
    return (error?0:1);
}




/*
 * Build a filter object from a filter string using the XOM Convenience
 * Function omX_string_to_object()
 */
int
build_filter_object(
    OM_workspace workspace,
    char 	 *filter,
    OM_object	 *filter_obj
)
{
    OM_integer        err_pos;
    OM_integer        err_type;
    OM_string	      filter_str;
    OM_return_code    rc;
    int               error = 0;


    filter_str.length = strlen(filter);
    filter_str.elements = filter;
    if ((rc = omX_string_to_object(workspace, &filter_str, DS_C_FILTER, 
		OM_TRUE, filter_obj, &err_pos, &err_type)) != OM_SUCCESS)
	error++;

    return (error?0:1);
}




/*
 * Free the name and attribute list objects in the entry list. Objects which
 * have been created using the XOM Convenience Function omX_string_to_object()
 * must be deleted using om_delete()
 */
int
free_entry_list(
    struct entry       entry_array[]
)
{
    OM_object	      attr_list_obj;
    int		      i, j;
    int		      error = 0;

    for (i = 0; i < NO_OF_ENTRIES; i++)  {

	/* 
	 * Delete the service generated public name object 
	 */
	if (om_delete(entry_array[i].name) != OM_SUCCESS)
	    error++;

	/*
	 * The last two attribute lists were the same as the 4th one
	 */
	if (i < NO_OF_ENTRIES-2) {
	    attr_list_obj = entry_array[i].attr_list;
	    for (j = 0; attr_list_obj[j].type != OM_NO_MORE_TYPES; j++) {
	        if (attr_list_obj[j].type == DS_ATTRIBUTES)

		    /* 
		     * Delete the service generated public attribute object
		     */
		    if (om_delete(attr_list_obj[j].value.object.object) 
								!= OM_SUCCESS)
		        error++;
    	    }

	    /* 
	     * Free the whole attribute list object
	     */
    	    free(attr_list_obj);
	}
    }

    return (error?0:1);
}



