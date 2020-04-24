/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dhd_util.c,v $
 * Revision 1.1.4.2  1996/02/18  19:13:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:11  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:21:06  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/6  1995/09/29  20:47 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/23  16:49 UTC  tatsu_s
 * 	Added dhd__schema_ent_free_ptrs() kluge.
 * 
 * 	HP revision /main/HPDCE02/5  1995/06/27  17:09 UTC  truitt
 * 	Merge binding changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts36/1  1995/06/27  17:06 UTC  truitt
 * 	CHFts15407: Allow string binding to be specified as a legal hostdata name.
 * 
 * 	HP revision /main/HPDCE02/4  1995/06/05  18:44 UTC  truitt
 * 	Merge name parsing changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts33/1  1995/06/05  18:41 UTC  truitt
 * 	CHFts15397: Improve the name parsing function.
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/24  16:30 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts31/1  1995/05/24  16:29 UTC  truitt
 * 	CHFts14679: Change the way field header strings are defined.
 * 
 * 	HP revision /main/HPDCE02/2  1995/02/02  15:27 UTC  truitt
 * 	Merge to fix hostdata show spacing.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts07/1  1995/02/02  15:25 UTC  truitt
 * 	CHFts14261: Correct spacing in hostdata output.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/15  19:45 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_chk_01/1  1994/12/15  19:43 UTC  truitt
 * 	The call to print_attrs() now requires the
 * 	encoding type of the attribute to be passed
 * 	in as a parameter.
 * 	[1995/12/08  17:55:52  root]
 * 
 * Revision 1.1.2.23  1994/10/03  21:02:07  pwang
 * 	Called Tcl_Concat insted of Tcl_Merge for binary data [OT#12468]
 * 	Shown the sc_uuid instead of instance in srvrexec_data_t [OT#12442]
 * 	[1994/10/03  21:01:39  pwang]
 * 
 * Revision 1.1.2.22  1994/09/30  00:49:18  pwang
 * 	Used tcl syntax for des key value on input and output
 * 	[OT#12424]
 * 
 * Revision 1.1.2.21  1994/09/29  19:11:18  truitt
 * 	Change one more line associated with the print_attrs() fix.
 * 	[1994/09/29  19:10:35  truitt]
 * 
 * Revision 1.1.2.20  1994/09/29  18:45:05  truitt
 * 	OT10967: Had to change the way print_attrs() was called
 * 	in order to match the changes to that function.  The
 * 	function which calls print_attrs() needs to display the
 * 	attribute name on it's own now.
 * 	[1994/09/29  18:30:03  truitt]
 * 
 * Revision 1.1.2.19  1994/09/23  20:53:21  pwang
 * 	Added "xattrschema" as aprth of schema name [OT#12319]
 * 	[1994/09/23  20:53:01  pwang]
 * 
 * Revision 1.1.2.18  1994/09/21  21:53:15  pwang
 * 	Accepted full qualified hostname [OT#12188]
 * 	[1994/09/21  21:52:53  pwang]
 * 
 * Revision 1.1.2.17  1994/09/01  20:09:56  pwang
 * 	Expanded the dhd__parse_sec_attr() to handle the remove_by_type
 * 	stuff [OT#11970]
 * 	[1994/09/01  20:05:46  pwang]
 * 
 * Revision 1.1.2.16  1994/08/30  18:05:01  pwang
 * 	Added logic to check and convert a tcl binding to
 * 	string binding [O#11928]
 * 	[1994/08/30  17:49:28  pwang]
 * 
 * Revision 1.1.2.15  1994/08/26  14:54:07  pwang
 * 	Removed extra spaces and adjusted the outlook
 * 	in dhd__show_object() [#11902]
 * 	[1994/08/26  14:53:33  pwang]
 * 
 * Revision 1.1.2.14  1994/08/24  21:01:56  pwang
 * 	Added new server attr entryname [OT#11398]
 * 	Showed byte format for untranslatable tower data [OT#11575]
 * 	[1994/08/24  20:58:18  pwang]
 * 
 * Revision 1.1.2.13  1994/08/22  19:11:52  pwang
 * 	dhd__parse_sec_attr() generates era_info_t array if being
 * 	requested [OT#11424]
 * 	[1994/08/22  18:50:56  pwang]
 * 
 * Revision 1.1.2.12  1994/08/08  18:51:48  melman
 * 	change strdup to dce_strdup
 * 	[1994/08/08  17:52:18  melman]
 * 
 * Revision 1.1.2.11  1994/07/22  21:40:55  pwang
 * 	Replaced intermediate attr schema with dce_attr
 * 	schema interface [OT#10262]
 * 	[1994/07/22  19:43:37  pwang]
 * 
 * Revision 1.1.2.10  1994/07/07  20:53:40  coren
 * 	Changed call to get_attr_values for new signature.
 * 	[1994/07/05  21:43:25  coren]
 * 
 * Revision 1.1.2.9  1994/07/05  16:30:11  pwang
 * 	Removed unused routines, added missed options of server object,
 * 	properly released binding of attr_schema and corrected mis-useed
 * 	attributes [OT#11072, #11058, #11020]
 * 	[1994/07/05  16:26:16  pwang]
 * 
 * Revision 1.1.2.8  1994/06/22  16:08:28  pwang
 * 	Implement utility routines to access dced attr_schema IDL
 * 	interface and used Era routines of rgy_util.c [OT #10464]
 * 	[1994/06/22  16:04:39  pwang]
 * 
 * Revision 1.1.2.7  1994/06/09  16:02:06  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:37  devsrc]
 * 
 * Revision 1.1.2.6  1994/06/07  20:04:31  pwang
 * 	Adjusted the server object output
 * 	[1994/06/07  20:03:13  pwang]
 * 
 * Revision 1.1.2.5  1994/05/19  18:23:43  pwang
 * 	Removed extra newlines in outputs.
 * 	[1994/05/19  18:03:34  pwang]
 * 
 * Revision 1.1.2.4  1994/05/18  21:57:38  pwang
 * 	Updated to changes in RFC and Added dhd__parse_bindata()
 * 	to support binary data and adjusted outputs (attribute list,
 * 	ERA stuff).
 * 	[1994/05/18  21:39:59  pwang]
 * 
 * Revision 1.1.2.3  1994/05/05  14:57:01  pwang
 * 	Made the corresponding changes due to the changes in dced.
 * 	[1994/05/05  14:55:12  pwang]
 * 
 * Revision 1.1.2.2  1994/04/26  20:03:56  pwang
 * 	Used sr as server's abbreviated name instead of sv
 * 	[1994/04/26  19:41:54  pwang]
 * 
 * Revision 1.1.2.1  1994/04/22  19:59:26  pwang
 * 	Initial Implementation
 * 	[1994/04/22  19:50:45  pwang]
 * 
 * $EndLog$
 */

/*
 * MODULE: dhd_util.c
 *
 * DESCRIPTION:
 * This module contains internal functions used to implement the
 * DCED operations in dcecp.
 */

#include <dhd.h>
#include <util_binding.h>
#include <ctype.h>

/*
 * Don't inherit stubbase.h's use of the exception based pthreads API.
 */
#ifndef NCK
#  define NCK
#endif
#include <dce/rpc.h>
#include <dce/stubbase.h>
#include <dce/sec_attr_base.h>
#include <dce/sec_attr_util.h>
#include <dce/sec_attr_tools.h>

#define UCHAR(c) ((unsigned char) (c))

#define	dhd_c_util_leading_space	" "
#define	dhd_c_util_leading_space1	"  "
#define	dhd_c_util_leading_space2	"   "
#define dhd_c_dced_cell_token1		"/.:/"
#define dhd_c_dced_cell_token2		"/.../"
#define dhd_c_dced_cell_token1_len	(sizeof dhd_c_dced_cell_token1 - 1)
#define dhd_c_dced_cell_token2_len	(sizeof dhd_c_dced_cell_token2 - 1)

/*
 * Field headers.
 */
#define DCP_T_DHD_UUID_HEADER           "uuid"
#define DCP_T_DHD_ANNOTATION_HEADER     "annotation"
#define DCP_T_DHD_STORAGE_HEADER        "storage"
#define DCP_T_DHD_PROGRAM_HEADER        "program"
#define DCP_T_DHD_ARGUMENTS_HEADER      "arguments"
#define DCP_T_DHD_PREREQUISITES_HEADER  "prerequisites"
#define DCP_T_DHD_KEYTABS_HEADER        "keytabs"
#define DCP_T_DHD_ENTRYNAME_HEADER      "entryname"
#define DCP_T_DHD_SERVICES_HEADER       "services"
#define DCP_T_DHD_IFNAME_HEADER         "ifname"
#define DCP_T_DHD_INTERFACE_HEADER      "interface"
#define DCP_T_DHD_BINDINGS_HEADER       "bindings"
#define DCP_T_DHD_OBJECTS_HEADER        "objects"
#define DCP_T_DHD_FLAGS_HEADER          "flags"
#define DCP_T_DHD_PRINCIPALS_HEADER     "principals"
#define DCP_T_DHD_STARTON_HEADER        "starton"
#define DCP_T_DHD_UID_HEADER            "uid"
#define DCP_T_DHD_GID_HEADER            "gid"
#define DCP_T_DHD_DIR_HEADER            "dir"
#define DCP_T_DHD_EXECUTING_HEADER      "executing"
#define DCP_T_DHD_PID_HEADER            "pid"
#define DCP_T_DHD_GENERIC_HEADER        "hostdata/data"

/*
 * Miscellaneous values.
 */
#define DCP_T_DHD_NONE_PW          "none"
#define DCP_T_DHD_PLAIN_PW         "plain"
#define DCP_T_DHD_DES_PW           "des"
#define DCP_T_DHD_UNKNOWN_PW       "unknown"
#define DCP_T_DHD_DISABLED_FLAG    "disabled"
#define DCP_T_DHD_BOOT_POLICY      "boot"
#define DCP_T_DHD_AUTO_POLICY      "auto"
#define DCP_T_DHD_EXPLICIT_POLICY  "explicit"
#define DCP_T_DHD_FAILURE_POLICY   "failure"
#define DCP_T_DHD_SCHEDULE_POLICY  "schedule"

static	dced_entry_t		s_null_entry;
static	server_t		s_null_server;
static	sec_passwd_rec_t	s_null_passwd;

/* The NDR transfer syntax id */
static rpc_syntax_id_t NIDL_ndr_id = 
{
	{0x8a885d04U, 0x1ceb, 0x11c9, 0x9f, 0xe8, 
	{0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}}, 2
};


/*
 * FUNCTION: dhd__parse_name
 *
 * OVERVIEW:
 *
 * This routine will parse an input name into entry name and object
 * name to access dced service. 
 *
 * INPUTS:
 * Tcl_Interp           *interp
 * char                 *name -  input name 
 * char			*service -  the service name
 * int			service_len - the length of service name
 * char			*hostname - hostname
 *
 * OUTPUTS:
 * char      		**entryname - entry name 
 * char			**objname - object name
 * char			**fullname - full entry name
 *
 * RETURNS:
 * TCL_OK on success, TCL_ERROR on failure  
 */
int
dhd__parse_name(Tcl_Interp *interp,
                char       *name,
                char       *service,
                int        service_len,	
                char       *hostname,
                char       **entryname,
                char       **objname,
                char       **fullname)
{
    char                   *srv_ptr, *srv_ptr2;
    char                   *obj_ptr = NULL;
    char                   *entry_name, *object_name;
    char                   *full_name, *name_ptr, *root_ptr, *tmp_ptr;
    int                    entryname_len, firstpart_len;
    int                    srv_name_len, fullname_len, cell_name_len;
    error_status_t         status;
    char                   *binding_string;
    handle_t               rpc_binding;
    dced_binding_handle_t  dced_binding;
    char                   *hname = NULL;
    uuid_t                 entry_uuid, attr_uuid;
    sec_attr_t             *dced_data;

    /*
     * Initialize the entryname, objname and fullname in case it's 
     * failed in the middle
     */
    if (entryname) {
        *entryname = NULL;
    }
    if (objname) {
        *objname = NULL;
    }
    if (fullname) {
        *fullname = NULL;
    }

    /*
     * Parse name:
     * If there is "/config/", the name is dced entryname + object name.
     * - The part after "/config/" is service name. If the service name
     *   is "server", it will be replace with $service (input parameter)
     *   If the service name is not "server" and $service, an error will 
     *   be reported back. 
     * - The part after the last '/' is object name. If it's missing,
     *   an error will be returned.
     * If there is not "/config/", the name is a simple object name and
     * the hostname will be taken into account (i.e service@hostname or
     * service).
     */
    if ((name) && (srv_ptr = strstr(name, dhd_c_dced_entry_token))) {
    	/*
    	 * /.:/hosts/hostname/config/service/name
    	 * or
    	 * /.../cellname/hosts/hostname/config/service/name
    	 */
    	srv_ptr += dhd_c_dced_entry_token_len;
    	obj_ptr = strrchr(name, '/') + 1;
    	if (srv_ptr == obj_ptr) {
            /*
             * /.:/hosts/hostname/config/service
             * or
             * /.../cellname/hosts/hostname/config/service
             */
            DCP_SET_MISC_ERROR(dcp_s_dhd_miss_objname);
    	}
    	srv_name_len = (obj_ptr - srv_ptr) - 1;
    	if ((srv_name_len == dhd_c_server_token_len) &&
            !(strncasecmp(srv_ptr, dhd_c_server_token, dhd_c_server_token_len)) &&
            (((service_len == dced_c_service_srvrconf_len) &&
            !(strncasecmp(service, dced_c_service_srvrconf, service_len))) ||
            ((service_len == dced_c_service_srvrexec_len) &&
            !(strncasecmp(service, dced_c_service_srvrexec, service_len))))) {
            /*
             * /.:/hosts/hostname/config/server/name
             * or
             * /.../cellname/hosts/hostname/config/server/name
             */
            firstpart_len = srv_ptr - name; 	
            entryname_len = firstpart_len + service_len + 1;	
            DCP_DHD_MALLOC_RESET_RET(entry_name, char, entryname_len);
            strncpy(entry_name, name, firstpart_len);
            strncpy(entry_name+firstpart_len, service, service_len);
            entry_name[entryname_len-1] = '\0';
            object_name = dce_strdup(obj_ptr);
    	}
    	else if (((srv_name_len == service_len) &&
                 !(strncasecmp(srv_ptr, service, service_len))) ||
                 ((srv_name_len == dced_c_service_srvrexec_len) &&
                 !(strncasecmp(srv_ptr, dced_c_service_srvrexec, srv_name_len)) &&
                 (service_len == dced_c_service_srvrconf_len) &&
                 !(strncasecmp(service, dced_c_service_srvrconf, service_len)))) {
            /*
             * There is a legal service here somewhere.
             */
            entryname_len =  obj_ptr - name;
            DCP_DHD_MALLOC_RESET_RET(entry_name, char, entryname_len);
            strncpy(entry_name, name, entryname_len-1);
            entry_name[entryname_len-1] = '\0';
            object_name = dce_strdup(obj_ptr);
        }
        else {
            /*
             * This is an illegal service.
             */
            DCP_SET_MISC_ERROR(dcp_s_dhd_wrong_service_req);
        }
    }
    else if ((name) && ((srv_ptr = strstr(name, dhd_c_dced_cell_token1)) ||
             (srv_ptr2 = strstr(name, dhd_c_dced_cell_token2)))) {
        /*
         * The name contains the identifying characters of a
         * cellname, but does not contain the string 'config'.
         * This syntax is not valid when we are talking about
         * srvrconf objects.
         */
        DCP_SET_MISC_ERROR(dcp_s_dhd_miss_objname);
    }
    else {
        /*
         * The entryname will be service@hostname or service
         * and object name will be name. 
         */
        if (hostname) {
            /*
             * It is possible here to have a string binding instead of
             * a host name.  If we can obtain a legal rpc binding using
             * the hostname variable, then we have a string binding instead
             * of a hostname, and we have to process it a little differently.
             */
            binding_string = dce_strdup(hostname);
            rpc_binding_from_string_binding((unsigned_char_t *)binding_string,
                                            &rpc_binding,
                                            &status);
            if (status == rpc_s_ok) {
                /*
                 * Processing a string binding.
                 */
                entryname_len = strlen(hostname) + 1;
                DCP_DHD_MALLOC_RESET_RET(entry_name, char, entryname_len);
                strcpy(entry_name, hostname);

                /*
                 * While we are here, if the fullname is requested, see if we
                 * can replace the string binding with the 'hosts/name' string.
                 */
                if (fullname) {
                    dced_binding_from_rpc_binding(dced_e_service_type_hostdata,
                                                  rpc_binding,
                                                  &dced_binding,
                                                  &status);
                    if (status == rpc_s_ok) {
                        /*
                         * Try to get the host information from the
                         * dced hostname object.
                         */
                        entry_uuid = dced_g_uuid_host_name;
                        attr_uuid = dced_g_uuid_fileattr;
                        dced_hostdata_read(dced_binding,
                                           &entry_uuid,
                                           &attr_uuid,
                                           &dced_data,
                                           &status);
                        if (status == rpc_s_ok) {
                            /*
                             * Read the data structure to get the hostname.
                             */
                            hname = dce_strdup((char *)SA_ATTR_STR_ARRAY_ELT_P(dced_data, 0));
                        }
                        dced_binding_free(dced_binding, &status);
                    }
                    else {
                        rpc_binding_free(&rpc_binding, &status);
                    }
                }
                else {
                    rpc_binding_free(&rpc_binding, &status);
                }
                free(binding_string);
            }
            else {
                /*
                 * Processing a host name.
                 */
                free(binding_string);
                entryname_len = service_len + 1 + strlen(hostname) + 1;
                DCP_DHD_MALLOC_RESET_RET(entry_name, char, entryname_len);
                strncpy(entry_name, service, service_len);
                entry_name[service_len] = '@';
                strcpy(entry_name + service_len + 1, hostname);
            }
        }
        else {
            entry_name = dce_strdup(service);
        }

        if (name == NULL) {
            object_name = NULL;
        }
        else {
            object_name = dce_strdup(name);
        }
    }

    /*
     * Return a fullname of the entry if it's requested:
     * /.:/hosts/host-simple-name/config/service/'\0'
     */
    if (fullname) {
        if (hname != NULL) {
            dce_cf_dced_entry_from_host(hname, &name_ptr, &status);
            free(hname);
        }
        else {
            dce_cf_dced_entry_from_host(hostname, &name_ptr, &status);
        }
        if (!DCP_DHD_GOOD_STATUS(status)) {
            free(entry_name);
            free(object_name);
            DCP_SET_MISC_ERROR(status);
        }
        fullname_len = strlen(name_ptr) + 1 + service_len + 1 + 1;

        /*
         * Strip out the CELL_DIR_ROOT if there is one since
         * we need GLOBAL_DIR_ROOT and cell_name 
         */
        tmp_ptr = name_ptr;
        if (!strncmp(name_ptr, CELL_DIR_ROOT, CELL_DIR_ROOT_LEN)) {
            fullname_len -=CELL_DIR_ROOT_LEN;
            tmp_ptr += CELL_DIR_ROOT_LEN;
        }

        /*
         * There are two ways to get the cell name if there is no cellname yet:
         *  - /.../cellname is part of given entryname
         *  - local cell
         */
        if (strncmp(tmp_ptr, GLOBAL_DIR_ROOT, GLOBAL_DIR_ROOT_LEN)) {
            if (!strncmp((char *)entry_name, GLOBAL_DIR_ROOT, GLOBAL_DIR_ROOT_LEN)) {
                root_ptr = strchr(entry_name + GLOBAL_DIR_ROOT_LEN, '/');
                cell_name_len = (root_ptr - (char *)entry_name) + 1;
                fullname_len += cell_name_len;
                DCP_DHD_MALLOC_RESET(full_name, char, fullname_len, status);
                if (!DCP_DHD_GOOD_STATUS(status)) {
                    free(entry_name);
                    free(object_name);
                    free(name_ptr);
                    DCP_SET_MISC_ERROR(status);
                }
                strncpy(full_name, entry_name, cell_name_len);
            }
            else {
                dce_cf_get_cell_name(&root_ptr, &status);
                if (!DCP_DHD_GOOD_STATUS(status)) {
                    free(entry_name);
                    free(object_name);
                    free(name_ptr);
                    DCP_SET_MISC_ERROR(status);
                }
                fullname_len += strlen(root_ptr) + 1;
                DCP_DHD_MALLOC_RESET(full_name, char, fullname_len, status);
                if (!DCP_DHD_GOOD_STATUS(status)) {
                    free(entry_name);
                    free(object_name);
                    free(name_ptr);
                    free(root_ptr);
                    DCP_SET_MISC_ERROR(status);
                }
                strcpy(full_name, root_ptr);
                strcat(full_name, "/");
                free(root_ptr);
            }
        }
        else {
            DCP_DHD_MALLOC_RESET(full_name, char, fullname_len, status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                free(entry_name);
                free(object_name);
                free(name_ptr);
                free(root_ptr);
                DCP_SET_MISC_ERROR(status);
            }
        }
        strcat(full_name, tmp_ptr);
        strcat(full_name, "/");
        strcat(full_name, service);
        strcat(full_name, "/");
        free(name_ptr);
    }

    /*
     * Return the entryname, objectname and fullname only if they're 
     * required.
     */
    if (entryname) {
        *entryname = entry_name;
    }
    else {
        free(entry_name);
    }
    if (objname) {
        *objname = object_name;
    }
    else {
        free(object_name);
    }
    if (fullname) {
        *fullname = full_name;
    }
    return TCL_OK;
}


/*
 * FUNCTION: dhd__parse_interface
 *
 * OVERVIEW:
 *
 * This routine will convert a input interface string into a rpc_if_id_t.
 *
 * INPUTS:
 * Tcl_Interp           *interp
 * char                 *interface_str -  input interface 
 *
 * OUTPUTS:
 * rpc_if_id_t		*rpc_if - Converted  rpc interface id 
 *
 * RETURNS:
 * TCL_OK on success, TCL_ERROR on failure  
 */
int
dhd__parse_interface(Tcl_Interp		*interp,
                     char		*interface_str,
                     rpc_if_id_t	*rpc_if)
{
    int			argc1;
    char		**argv1;
    char		interface_uuid[dhd_c_uuid_string_max];
    error_status_t	status;

    DCP_DHD_CLEAR_STATUS(status);
    DCP_DHD_CLEAR_ARGV(argc1, argv1); 

    if (strchr(interface_str, ',')) {
    	/*
    	 * uuid,major.minor
    	 */
    	sscanf(interface_str, "%[^,],%hd.%hd", interface_uuid,
    	       &rpc_if->vers_major, &rpc_if->vers_minor);
    }
    else {
    	/*
    	 * uuid major.minor
    	 */
    	if (Tcl_SplitList(interp, interface_str, &argc1, &argv1) != TCL_OK) {
    		return TCL_ERROR;
    	}
	if (argc1 != dhd_c_def_if_elements) {
		DCP_DHD_ARGV_FREE(argc1, argv1);
		DCP_SET_MISC_ERROR(dcp_s_sr_bad_if_attr);
	}
    	strcpy(interface_uuid, argv1[0]);
    	sscanf(argv1[1], "%hd.%hd", 
    	       &rpc_if->vers_major, &rpc_if->vers_minor);
    	DCP_DHD_ARGV_FREE(argc1, argv1); 
    }
    uuid_from_string((unsigned char *) interface_uuid,
    		     &rpc_if->uuid,
    		     &status);
    DCP_DHD_CHECK_STATUS_RET(status);

    return TCL_OK;
} /* End of dhd__parse_interface */


/*
 * FUNCTION: dhd__parse_interface_list
 *
 * OVERVIEW:
 *
 * This routine will convert a list of input interface string into a 
 * list of rpc_if_id_t.
 *
 * INPUTS:
 * Tcl_Interp           *interp
 * char                 *interface_list -  input interface 
 *
 * OUTPUTS:
 * int			*count
 * rpc_if_id_t		**rpc_if_list - Converted to a list of rpc interface id 
 *
 * RETURNS:
 * TCL_OK on success, TCL_ERROR on failure  
 */
int
dhd__parse_interface_list(Tcl_Interp	*interp,
                     	  char		*interface_list,
			  int		*count,
                     	  rpc_if_id_t	**rpc_if_list)
{
    int			argc1;
    char		**argv1;
    rpc_if_id_t		*if_list_ptr;
    int			i;
    error_status_t	status;

    DCP_DHD_CLEAR_STATUS(status);
    DCP_DHD_CLEAR_ARGV(argc1, argv1); 

    /*
     * Inintialize interface_list in case it's failed in the
     * middle
     */
     *count = 0;
     *rpc_if_list = NULL;

    /*
     * Split the interface list
     */
    if ((isllist(interface_list)) || (strchr(interface_list, ','))) {
	/*
	 * {uuid major.minor} ... {uuid major.minor}
	 * or
	 * uuid,major.minor ... uuid,major.minor
	 * or
	 * uuid,major.minor
	 */
	if (Tcl_SplitList(interp, interface_list, &argc1, &argv1) != TCL_OK) {
		return TCL_ERROR;
	}
    }	
    else {
	/*
	 * uuid major.minor
	 */
	argc1 = 1;
	DCP_DHD_ARGV_ALLOC(argc1, argv1);
	argv1[0] = interface_list;
    }
    DCP_DHD_MALLOC_RESET(if_list_ptr, rpc_if_id_t, argc1, status);
    if (!DCP_DHD_GOOD_STATUS(status)) {
	DCP_DHD_ARGV_FREE(argc1, argv1);
	return TCL_ERROR;
    }

    for (i=0; i<argc1; i++) {
	if (dhd__parse_interface(interp, argv1[i], &if_list_ptr[i]) 
							!= TCL_OK) {
		free(if_list_ptr);
		DCP_DHD_ARGV_FREE(argc1, argv1);
		return TCL_ERROR;
	}
    }

    *count = argc1;
    *rpc_if_list = if_list_ptr;
    DCP_DHD_ARGV_FREE(argc1, argv1);
    return TCL_OK;
} /* End of dhd__parse_interface_list */



/*
 * FUNCTION: dhd__parse_towers
 *
 * OVERVIEW:
 *
 * This routine will set the towers in service_t
 *
 * INPUTS:
 * Tcl_Interp		   *interp
 * rpc_if_handle_t         if_handle 
 * int			   inargc
 * char		           **inargv 
 *
 * OUTPUTS:
 * rpc_tower_list_list_t   *tower_list
 *
 * RETURNS:
 * TCL_OK on success, TCL_ERROR on failure
 */
int
dhd__parse_towers(Tcl_Interp			*interp,
		  rpc_if_handle_t		if_handle, 
		  int				inargc,
		  char				**inargv,
		  dced_tower_list_t		*tower_list)
{
    int				i;
    int                         argc1;
    char                        **argv1;
    rpc_binding_handle_t	binding;
    char                        *string_binding;
    error_status_t		status, status1;

    DCP_DHD_CLEAR_STATUS(status);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    if (strcasecmp(dhd_c_serv_bindings_token, inargv[0])) {
		DCP_DHD_SET_RESULT(dcp_s_dhd_invalid_args, inargv[0]);
		return TCL_ERROR;
    }

    if (inargc <= 1) {
	tower_list->count = 0;
	tower_list->list = NULL;
	return TCL_OK;
    }

    DCP_DHD_MALLOC_LIST_RESET(tower_list, sec_attr_twr_set_p_t, 
			      (inargc-1), status);
    DCP_DHD_CHECK_STATUS_RET(status);

    for (i=1; i<inargc; i++) {
        if (strchr(inargv[i], ' ')) {
            /*
             * It's a Tcl syntax binding
             */
            if (Tcl_SplitList(interp, inargv[i], &argc1, &argv1) != TCL_OK) {
                dced_tower_list_free_ptrs(tower_list);
                return TCL_ERROR;
            }
            string_binding = dcp_binding_tcl_to_string(argc1, argv1, &status);
            DCP_DHD_ARGV_FREE(argc1, argv1);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                dced_tower_list_free_ptrs(tower_list);
                DCP_SET_MISC_ERROR(status);
            }

    	    rpc_binding_from_string_binding((unsigned char *) string_binding,
    					    &binding, &status);
            rpc_string_free((unsigned_char_t **) &string_binding, &status1);
        } 
        else {
            /*
             * It's a string binding
             */ 
    	    rpc_binding_from_string_binding((unsigned char *) inargv[i],
    					    &binding, &status);
        }
    	if (!DCP_DHD_GOOD_STATUS(status)) {
    		dced_tower_list_free_ptrs(tower_list);
    		DCP_SET_MISC_ERROR(status);
    	}
    	rpc_tower_vector_from_binding(
				if_handle,
				binding,
				(rpc_tower_vector_p_t *) &tower_list->list[i-1],
				&status);
    	if (!DCP_DHD_GOOD_STATUS(status)) {
    		dced_tower_list_free_ptrs(tower_list);
    		DCP_SET_MISC_ERROR(status);
    	}
    	rpc_binding_free(&binding, &status);
    	if (!DCP_DHD_GOOD_STATUS(status)) {
    		dced_tower_list_free_ptrs(tower_list);
    		DCP_SET_MISC_ERROR(status);
    	}
    }

    return TCL_OK;
} /* End of dhd__parse_towers */


/*
 * FUNCTION: dhd__parse_sec_attr
 *
 * OVERVIEW:
 *
 * This routine will parse the tcl list of sec_attr and assign it to 
 * sec_attr
 *
 * INPUTS:
 * Tcl_Interp	*interp
 * char *list - the tcl list for sec_attr 
 *
 * OUTPUTS:
 * dced_attr_list_t	*attribute
 * OPTIONAL:
 * era_info_t           *era_info
 * boolean32            type
 *
 * RETURNS:
 * TCL_OK on success, TCL_ERROR on failure
 */
int
dhd__parse_sec_attr(Tcl_Interp  	*interp,
               	    char         	*list,
               	    dced_attr_list_t	*attribute,
                    era_info_t          **era_info,
                    boolean32           type)
{
    int             			argc1, argc2;
    char            			**argv1, **argv2;
    int             			i;
    extern dce_attr_sch_handle_t	attr_schema_binding;
    dce_attr_schema_entry_t		schema_entry;
    era_info_t                          *era_info_p;
    error_status_t			status;

    DCP_DHD_CLEAR_STATUS(status);
    DCP_DHD_CLEAR_ARGV_2(argc1, argv1, argc2, argv2);

    /*
     * Initialize the attribute list in case it's failed
     * in the middle
     */
     attribute->count = 0;
     attribute->list = NULL;

    /*
     * Check to see if the list is a list of attribute or
     * just one
     */
    if ((type) || (isllist(list))) {
    	if (Tcl_SplitList(interp, list, &argc1, &argv1) != TCL_OK) {
   	     return TCL_ERROR;
    	}
    }
    else {
	argc1 = 1;
	DCP_DHD_ARGV_ALLOC(argc1, argv1);
	argv1[0] = list;
    }

    /*
     * Allocation the memory for the sec_attr list 
     */
    DCP_DHD_MALLOC_LIST_RESET(attribute, sec_attr_t, argc1, status);
    if (!DCP_DHD_GOOD_STATUS(status)) {
	DCP_DHD_ARGV_FREE(argc1, argv1);
	return TCL_ERROR;
    }
    if (era_info != NULL) {
        DCP_DHD_MALLOC_RESET(era_info_p, era_info_t, argc1, status);
        if (!DCP_DHD_GOOD_STATUS(status)) {
	    DCP_DHD_ARGV_FREE(argc1, argv1);	
	    return TCL_ERROR;
        }
    }

    for (i=0; i<argc1; i++) {

        if (Tcl_SplitList(interp, argv1[i], &argc2, &argv2) != TCL_OK) {
	    DCP_DHD_ARGV_FREE(argc1, argv1);	
	    dced_attr_list_free_ptrs(attribute);
            return TCL_ERROR;
        }

        /*
	 * Lookup by attr name
         */
	dce_attr_sch_lookup_by_name(attr_schema_binding, 
				    (unsigned_char_t *)argv2[0],
				    &schema_entry, &status); 
	if (!DCP_DHD_GOOD_STATUS(status)) {
	    DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);	
	    dced_attr_list_free_ptrs(attribute);
	    DCP_SET_MISC_ERROR(status);
        }
 

        if (era_info) {
            era_info_p[i].attr_id = schema_entry.attr_id;
            era_info_p[i].schema_entry_flags = schema_entry.schema_entry_flags;
        }
 
	status = get_attr_values(interp,
                                 schema_entry.attr_encoding, 
				 schema_entry.attr_id,
			    	 &attribute->list[i], 
				 &argc2, 
				 argv2);
#ifndef	HPDCE_DCED_LOCAL
	dhd__schema_ent_free_ptrs(&schema_entry);
#else
	sec_attr_util_sch_ent_free_ptrs(&schema_entry);
#endif	/* HPDCE_DCED_LOCAL */
	DCP_DHD_ARGV_FREE(argc2, argv2);
	if (!DCP_DHD_GOOD_STATUS(status)) {
	    	DCP_DHD_ARGV_FREE(argc1, argv1);	
	    	dced_attr_list_free_ptrs(attribute);
            	DCP_SET_MISC_ERROR(dcp_s_dhd_bad_attribute_list);
	}

    } /* End of for (i=0; i<argc1; i++) */ 

    if (era_info) {
        *era_info = era_info_p;
    }
    DCP_DHD_ARGV_FREE(argc1, argv1);
    return TCL_OK;
} /* End of dhd__parse_sec_attr */


/*
 * FUNCTION: dhd__parse_services
 *
 * OVERVIEW:
 *
 * This routine will parse the service list and assign it to service_t
 *
 * INPUTS:
 * Tcl_Interp *interp
 * char *list - the tcl list for service
 *
 * OUTPUTS:
 * service_list_t	*service_list
 *
 * RETURNS:
 * TCL_OK on success, TCL_ERROR on failure
 */
int
dhd__parse_services(Tcl_Interp		*interp,
               	    char       		*list,
               	    service_list_t	*service_list)
{
    int             argc1, argc2, argc3;
    char            **argv1, **argv2, **argv3;
    char	    *bindings = NULL;
    boolean32	    ifspec_parsed = FALSE, ifspec_wanted = FALSE;
    service_t	    *service;
    int             i, j, k;
    error_status_t  status;

    rpc_if_rep_t    interface_spec = {
	0,                  /* ifspec rep version                           */
	0,                  /* operation count (not needed)                 */
	0,                  /* interface version (to be filled in later)    */
	{ 0 },              /* interface uuid (to be filled in later)       */
	2,                  /* stub/runtime interface version number        */
	{ 0, NULL },        /* endpoint vector (not needed)                 */
	{ 1, &NIDL_ndr_id}, /* NDR transfer syntax (the only one for now)   */
	NULL,               /* server stub epv (not needed)                 */
	NULL                /* manager stub epv (not needed)                */
    };

    DCP_DHD_CLEAR_STATUS(status);
    DCP_DHD_CLEAR_ARGV_3(argc1, argv1, argc2, argv2, argc3, argv3);

    if (Tcl_SplitList(interp, list, &argc1, &argv1) != TCL_OK) {
	return TCL_ERROR;
    }

    /*
     * Check to see if the "list" is a list of service tcl list or
     * just one service tcl list
     */
    if (!isllist(argv1[0])) {
	/*
	 * The list is only a service tcl list
	 */
	DCP_DHD_ARGV_FREE(argc1, argv1);
	argc1 = 1;
	DCP_DHD_ARGV_ALLOC(argc1, argv1);
	argv1[0] = list;
    }

    /*
     * Allocate memory for the serivce list
     */
    DCP_DHD_MALLOC_LIST_RESET(service_list, service_t, argc1, status);
    if (!DCP_DHD_GOOD_STATUS(status)) {
        DCP_DHD_ARGV_FREE(argc1, argv1);
        return TCL_ERROR;
    }

    /*
     * Loop thr each service tcl list
     */
    for (i=0; i<argc1; i++) {

	/*
	 * Check to see if the service tcl list has more than one
	 * attribute lists
	 */ 
	if (isllist(argv1[i])) {
		if (Tcl_SplitList(interp, argv1[i], &argc2, &argv2) != TCL_OK) {
			dced_service_list_free_ptrs(service_list);
			DCP_DHD_ARGV_FREE(argc1, argv1);
			return TCL_ERROR;
		}
	}
	else {
        	argc2 = 1;
        	DCP_DHD_ARGV_ALLOC(argc2, argv2);
        	argv2[0] = argv1[i];
    	}


    	if (argc2 > dhd_c_max_serv_attr_size) {
	    	dced_service_list_free_ptrs(service_list);
	     	DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
	     	DCP_SET_MISC_ERROR(dcp_s_too_many_list_elements);
    	}

    	/*
     	 * For each component of a service tcl list
     	 */
	service = &service_list->list[i];
    	for (j=0; j<argc2; j++) {

        	if (Tcl_SplitList(interp, argv2[j], &argc3, &argv3) != TCL_OK) {
	    		dced_service_list_free_ptrs(service_list);
			DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
            		return TCL_ERROR;
        	} /* End of if (Tcl_SplitList(interp, argv2[j], &argc3, &argv3)
								   != TCL_OK) */
		/*
		 * Get the annotation text 
	 	 */
       		if (!strcasecmp(dhd_c_attr_annotation_token, argv3[0])) {
			if (argc3 != dhd_c_def_attr_elements) {
	    		       dced_service_list_free_ptrs(service_list);
	    		       DCP_DHD_ARGV_FREE_3(argc1, argv1, 
						   argc2, argv2, 
						   argc3, argv3);
            		       DCP_SET_MISC_ERROR(dcp_s_dhd_bad_attribute_list);
			}
			DCP_DHD_STRING_FREE(service->annotation);
			service->annotation = 
                                           (dced_string_t) dce_strdup(argv3[1]);
		} /* End of if (!strcasecmp(dhd_c_attr_annotation_token,
								argv3[0])) */

		/*
	 	 * Get the bindings 
	 	 */
        	else if ((!strcasecmp(dhd_c_serv_bindings_token, argv3[0])) &&
			 (argc3 > 1)) {
			/*
			 * If ifspec_parsed is on here, the ifsepc has been 
			 * parsed, otherwise not. 
			 */
			if (ifspec_parsed) { 
    				/*
    				 * If the tower list has been parsed, 
				 * the previous one will be overwritten. 
				 */
    				if (!DCP_DHD_EMPTY_LIST(&service->towers)) {
    					dced_tower_list_free_ptrs(
							&service->towers);
    				}
				if (dhd__parse_towers(
					      interp,
					      (rpc_if_handle_t)&interface_spec,
					      argc3,
					      argv3,
					      &service->towers) != TCL_OK) {
	    				dced_service_list_free_ptrs(
								service_list);
	    				DCP_DHD_ARGV_FREE_3(argc1, argv1, 
							    argc2, argv2,
							    argc3, argv3);
					return TCL_ERROR;
				}
			}
			else {
				bindings = Tcl_Merge(argc3, argv3);
				ifspec_wanted = TRUE;
			}
		} /* End of if (!strcasecmp(dhd_c_serv_bindings_token, 
								argv3[0])) */

		/*
	 	 * Get this entry name
	 	 */
		else if (!strcasecmp(dhd_c_serv_entryname_token, argv3[0])) {
			if (argc3 != dhd_c_def_attr_elements) {
	    		       dced_service_list_free_ptrs(service_list);
	    		       DCP_DHD_ARGV_FREE_3(argc1, argv1, 
						   argc2, argv2, 
						   argc3, argv3);
            		       DCP_SET_MISC_ERROR(dcp_s_dhd_bad_attribute_list);
			}
			DCP_DHD_STRING_FREE(service->entryname);
			service->entryname = 
                                           (dced_string_t) dce_strdup(argv3[1]);
		} /* End of if (!strcasecmp(dhd_c_serv_entryname_token, 
								argv3[0])) */ 

		/*
	         * Get the flag for this service
	 	 */
		else if (!strcasecmp(dhd_c_serv_flag_token, argv3[0])) {
			if (argc3 != dhd_c_def_attr_elements) {
	    		       dced_service_list_free_ptrs(service_list);
	    		       DCP_DHD_ARGV_FREE_3(argc1, argv1, 
						   argc2, argv2, 
						   argc3, argv3);
            		       DCP_SET_MISC_ERROR(dcp_s_dhd_bad_attribute_list);
			}
			if (!strcasecmp(dhd_c_serv_disabled_token, argv3[1])) {
				service->flags = service_c_disabled;
			}
			else {
	    		       dced_service_list_free_ptrs(service_list);
	    		       DCP_DHD_ARGV_FREE_3(argc1, argv1, 
						   argc2, argv2, 
						   argc3, argv3);
            		       DCP_SET_MISC_ERROR(dcp_s_sr_unknown_serv_flag);
			}		
		} /* End of if (!strcasecmp(dhd_c_serv_flag_token, argv3[0])) */

		/*
	 	 * Get the interface name
	 	 */
        	else if (!strcasecmp(dhd_c_serv_ifname_token, argv3[0])) {
			if (argc3 != dhd_c_def_attr_elements) {
	    		       dced_service_list_free_ptrs(service_list);
	    		       DCP_DHD_ARGV_FREE_3(argc1, argv1, 
						   argc2, argv2, 
						   argc3, argv3);
            		       DCP_SET_MISC_ERROR(dcp_s_dhd_bad_attribute_list);
			}
			DCP_DHD_STRING_FREE(service->ifname);
			service->ifname = (dced_string_t) dce_strdup(argv3[1]);
		} /* End of if (!strcasecmp(dhd_c_serv_ifname_token,
								argv3[0])) */

		/*
	 	 * Get the interface spec
	 	 */
        	else if (!strcasecmp(dhd_c_serv_ifspec_token, argv3[0])) {
			if (argc3 != dhd_c_def_attr_elements) {
	    		       dced_service_list_free_ptrs(service_list);
	    		       DCP_DHD_ARGV_FREE_3(argc1, argv1, 
						   argc2, argv2, 
						   argc3, argv3);
            		       DCP_SET_MISC_ERROR(dcp_s_dhd_bad_attribute_list);
			}
			if (dhd__parse_interface(interp, argv3[1], 
					 	 &service->ifspec) != TCL_OK) {
	    			dced_service_list_free_ptrs(service_list);
	    		        DCP_DHD_ARGV_FREE_3(argc1, argv1, 
			 			    argc2, argv2, 
						    argc3, argv3);
				return TCL_ERROR;
			}
			interface_spec.vers = (service->ifspec.vers_minor << 6)
					      | service->ifspec.vers_major;
			interface_spec.id = service->ifspec.uuid;
			/*
			 * If ifspec_wanted is on here, the binding has been 
			 * parsed before a ifspec is parsed, otherwise not. 
			 */
			if ((ifspec_wanted) && (bindings)) {
    				/*
    				 * If the tower list has been parsed, 
				 * the previous one will be overwritten. 
				 */
    				if (!DCP_DHD_EMPTY_LIST(&service->towers)) {
    					dced_tower_list_free_ptrs(
							&service->towers);
    				}
	    			DCP_DHD_ARGV_FREE(argc3, argv3);
        			if (Tcl_SplitList(interp, bindings, 
						  &argc3, &argv3) != TCL_OK) {
	    				dced_service_list_free_ptrs(
								service_list);
	    		        	DCP_DHD_ARGV_FREE_2(argc1, argv1, 
			 			    	    argc2, argv2); 
            				return TCL_ERROR;
				}
				free(bindings);
				if (dhd__parse_towers(
					      interp,
					      (rpc_if_handle_t)&interface_spec,
					      argc3,
					      argv3,
					      &service->towers) != TCL_OK) {
	    				dced_service_list_free_ptrs(
								service_list);
	    		        	DCP_DHD_ARGV_FREE_3(argc1, argv1, 
			 			    	    argc2, argv2, 
						    	    argc3, argv3);
					return TCL_ERROR;
				}
				ifspec_wanted = FALSE;
			}
			else {
				ifspec_parsed = TRUE;
			}
		} /* End of if (!strcasecmp(dhd_c_serv_ifspec_token, 
								argv3[0])) */
	
		/*
	 	 * Get object list
	 	 */
	 	else if ((!strcasecmp(dhd_c_serv_object_token, argv3[0])) &&
			 (argc3 > 1)) {

			if (!DCP_DHD_EMPTY_LIST(&service->objects)) {
				dced_uuid_list_free_ptrs(&service->objects);
			}
			DCP_DHD_MALLOC_LIST_RESET(&service->objects, uuid_t,
					     	  (argc3-1), status);
			if (!DCP_DHD_GOOD_STATUS(status)) {
				dced_service_list_free_ptrs(service_list);
				DCP_DHD_ARGV_FREE_3(argc1, argv1,
						    argc2, argv2, 
						    argc3, argv3);
				return TCL_ERROR;
			}
			for (k=1; k<argc3; k++) {
				uuid_from_string((unsigned char *) argv3[k],
						 &service->objects.list[k-1],
						 &status);
				if (!DCP_DHD_GOOD_STATUS(status)) {
					dced_service_list_free_ptrs(
								service_list);
					DCP_DHD_ARGV_FREE_3(argc1, argv1,
							    argc2, argv2, 
							    argc3, argv3);
					DCP_SET_MISC_ERROR(status);
				}
			}
		} /* End of if (!strcasecmp(dhd_c_serv_object_token, 
								argv3[0])) */

		/*
	 	 * Get operation number list
	 	 */
		else if ((!strcasecmp(dhd_c_serv_opnum_token, argv3[0])) &&
			 (argc3 > 1)) {

			if (!DCP_DHD_EMPTY_LIST(&service->operations)) {
				dced_opnum_list_free_ptrs(&service->operations);
			}
			DCP_DHD_MALLOC_LIST_RESET(&service->operations,
					     	  unsigned32,
				     	     	  (argc3-1), 
					     	  status);
			if (!DCP_DHD_GOOD_STATUS(status)) {
				dced_service_list_free_ptrs(service_list);
				DCP_DHD_ARGV_FREE_3(argc1, argv1,
						    argc2, argv2, 
						    argc3, argv3);
				return TCL_ERROR;
			}
			for (k=1; k<argc3; k++) {
				service->operations.list[k-1] = atol(argv3[k]);
			}
		} /* End of if (!strcasecmp(dhd_c_serv_opnum_token,argv3[0])) */

		else {
			dced_service_list_free_ptrs(service_list);
			DCP_DHD_SET_RESULT(dcp_s_bad_attribute, argv3[0]);
			DCP_DHD_ARGV_FREE_3(argc1, argv1,
					    argc2, argv2, 
					    argc3, argv3);
			return TCL_ERROR;
		} /* End of else */

		DCP_DHD_ARGV_FREE(argc3, argv3);
    	} /* End of for (j=0; j<argc2; j++) */ 

    	DCP_DHD_ARGV_FREE(argc2, argv2);
   } /* End of for (i=0; i<argc1; i++) */ 

   DCP_DHD_ARGV_FREE(argc1, argv1);
   return TCL_OK;
} /* End of dhd__parse_service */


/*
 * FUNCTION: dhd__parse_attr
 *
 * OVERVIEW:
 *
 * This routine will parse the dced configuratuion entry 
 * attribute (tcl) list  (i.e. uuid, annotation, storage and data)
 *
 * INPUTS:
 * Tcl_Interp *interp
 * char *list - the tcl list for attribute  
 *
 * OUTPUTS:
 * char **dataptr
 *
 * RETURNS:
 * TCL_OK on success, TCL_ERROR on failure
 */
int
dhd__parse_attr(Tcl_Interp  *interp,
               char         *list,
               dced_entry_t *entry,
               char	    **dataptr)
{
    int             argc1, argc2;
    char            **argv1, **argv2;
    int             i;
    error_status_t  status;

    DCP_DHD_CLEAR_STATUS(status);
    DCP_DHD_CLEAR_ARGV_2(argc1, argv1, argc2, argv2);

    /*
     * Check to see if the list is a list of attribute or
     * just one
     */
    if (isllist(list)) {
    	if (Tcl_SplitList(interp, list, &argc1, &argv1) != TCL_OK) {
   	     return TCL_ERROR;
    	}
    	if (argc1 > dhd_c_max_entry_attr_size) {
	     DCP_DHD_ARGV_FREE(argc1, argv1);
	     DCP_SET_MISC_ERROR(dcp_s_too_many_list_elements);
    	}
    }
    else {
	argc1 = 1;
	DCP_DHD_ARGV_ALLOC(argc1, argv1);
	argv1[0] = list;
    }

    /*
     * Initialize the entry and data in case it's failed in
     * middle
     */
    *entry = s_null_entry;
    *dataptr = NULL;

    for (i=0; i<argc1; i++) {
        if (Tcl_SplitList(interp, argv1[i], &argc2, &argv2) != TCL_OK) {
	    DCP_DHD_ARGV_FREE(argc1, argv1);
	    dced_entry_free_ptrs(entry);
            return TCL_ERROR;
        } /* End of if (Tcl_SplitList(interp, argv1[i], &argc2, &argv2) 
							  != TCL_OK) */

        /* 
	 * All attributes here except data attribute are single value
	 * attribute, i.e it's composed of attribute-type and attribute-value,
         * {type value}, where value could be single string or a tcl list.
         */

        if (!strcasecmp(dhd_c_attr_id_token, argv2[0])) {
	    if (argc2 != dhd_c_def_attr_elements) {
		DCP_DHD_SET_STATUS(status, dcp_s_dhd_bad_attribute_list);
	    }
	    else {
           	 uuid_from_string((unsigned char *) argv2[1], 
				  &entry->id, 
                                  &status);  
	    }
	    if (!DCP_DHD_GOOD_STATUS(status)) {
	    	DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
	    	dced_entry_free_ptrs(entry);
                DCP_SET_MISC_ERROR(status);
            }
        } /* End of if (!strcasecmp(dhd_c_attr_id_token, argv2[0])) */

        else if (!strcasecmp(dhd_c_attr_annotation_token, argv2[0])) {
	    if (argc2 != dhd_c_def_attr_elements) {
	    	DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
	    	dced_entry_free_ptrs(entry);
                DCP_SET_MISC_ERROR(dcp_s_dhd_bad_attribute_list);
	    }
	    DCP_DHD_STRING_FREE(entry->description);
            entry->description = (dced_string_t) dce_strdup(argv2[1]);
        } /* End of if (!strcasecmp(dhd_c_attr_annotation_token, argv2[0])) */ 

        else if (!strcasecmp(dhd_c_entry_attr_storage_token, argv2[0])) {
	    if (argc2 != dhd_c_def_attr_elements) {
	    	DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
	    	dced_entry_free_ptrs(entry);
                DCP_SET_MISC_ERROR(dcp_s_dhd_bad_attribute_list);
	    }
	    DCP_DHD_STRING_FREE(entry->storage_tag);
            entry->storage_tag = (dced_string_t) dce_strdup(argv2[1]);
        } /* End of if (!strcasecmp(dhd_c_entry_attr_storage_token, 
								argv2[0])) */ 
        else if (!strcasecmp(dhd_c_entry_attr_data_token, argv2[0])) {
	    DCP_DHD_STRING_FREE(*dataptr);
	    *dataptr = Tcl_Concat(argc2-1, &argv2[1]);
        } /* End of if ((!strcasecmp(dbh_c_entry_attr_data_token, argv2[0])) */

	else {
		dced_entry_free_ptrs(entry);
		DCP_DHD_SET_RESULT(dcp_s_bad_attribute, argv2[0]);
		DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
		return TCL_ERROR;
	} /* End of else */

	DCP_DHD_ARGV_FREE(argc2, argv2);
    } /* End of for (i=0; i<argc1; i++) */ 

    DCP_DHD_ARGV_FREE(argc1, argv1);
    return TCL_OK;
} /* End of dhd__parse_attr */


/*
 * FUNCTION: dhd__parse_server
 *
 * OVERVIEW:
 *
 * This routine will parse the server attribute and assign it to server_t
 *
 * INPUTS:
 * Tcl_Interp *interp
 * char *list - the tcl list for attribute  
 *
 * OUTPUTS:
 * server_t *server
 * char	    **var_attr
 *
 * RETURNS:
 * TCL_OK on success, TCL_ERROR on failure
 */
int
dhd__parse_server(Tcl_Interp  *interp,
               	  char        *list,
               	  server_t    *server,
		  char	      **var_attr)
{
    int             argc1, argc2;
    char            **argv1, **argv2;
    char	    *list_ptr;
    int             i, j;
    error_status_t  status;

    DCP_DHD_CLEAR_STATUS(status);
    DCP_DHD_CLEAR_ARGV_2(argc1, argv1, argc2, argv2);

    /*
     * Check to see if the list is a list of attribute or
     * just one
     */
    if (isllist(list)) {
    	if (Tcl_SplitList(interp, list, &argc1, &argv1) != TCL_OK) {
   	     return TCL_ERROR;
    	}
    	if (argc1 > dhd_c_max_srvr_attr_size) {
	     DCP_DHD_ARGV_FREE(argc1, argv1);
	     DCP_SET_MISC_ERROR(dcp_s_too_many_list_elements);
    	}
    }
    else {
	argc1 = 1;
	DCP_DHD_ARGV_ALLOC(argc1, argv1);
	argv1[0] = list;
    }

    /*
     * Initialize the object 
     */
    *server = s_null_server;
    *var_attr = NULL;

    for (i=0; i<argc1; i++) {

        if (Tcl_SplitList(interp, argv1[i], &argc2, &argv2) != TCL_OK) {
	    DCP_DHD_ARGV_FREE(argc1, argv1);
	    dced_server_free_ptrs(server);
            return TCL_ERROR;
        } /* End of if (Tcl_SplitList(interp, argv1[i], &argc2, &argv2) 
							!= TCL_OK) */
	/*
	 * - arguments
	 */
	if ((!strcasecmp(dhd_c_srvr_argument_token, argv2[0])) && 
	    (argc2 > 1)) {
		/*
		 * If the arguments has been parsed before, this one will
		 * overwrite the previous one
		 */
		if (!DCP_DHD_EMPTY_LIST(&server->fixed.arguments)) {
			dced_string_list_free_ptrs(&server->fixed.arguments);
		}
		DCP_DHD_MALLOC_LIST_RESET(&server->fixed.arguments, 
					  dced_string_t, 
					  (argc2-1), 
					  status);
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
			dced_server_free_ptrs(server);
			return TCL_ERROR;
		}

		for (j=1; j<argc2; j++) {
			server->fixed.arguments.list[j-1] 
					= (dced_string_t) dce_strdup(argv2[j]);
		}
	} /* End of if (!strcasecmp(dhd_c_srvr_argument_token, argv2[0])) */

	/*
	 * - attributes
	 */
	else if ((!strcasecmp(dhd_c_srvr_var_attr_token, argv2[0])) &&
		 (argc2 > 1)) {
		*var_attr = Tcl_Merge(argc2-1, &argv2[1]);
	}

	/*
	 * - directory 
	 */
	else if (!strcasecmp(dhd_c_srvr_directory_token, argv2[0])) {
        	if (argc2 != dhd_c_def_attr_elements) {
	    		DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
	    		dced_server_free_ptrs(server);
            		DCP_SET_MISC_ERROR(dcp_s_dhd_bad_attribute_list);
        	}  
		/*
		 * If the directory has been parsed before, this one will
		 * overwrite the previous one
		 */
		DCP_DHD_STRING_FREE(server->fixed.posix_dir);
		server->fixed.posix_dir = (dced_string_t) dce_strdup(argv2[1]);
	} /* End of if (!strcasecmp(dhd_c_srvr_directory_token, argv2[0])) */ 

	/*
	 * - entryname 
	 */
	else if (!strcasecmp(dhd_c_srvr_entryname_token, argv2[0])) {
        	if (argc2 != dhd_c_def_attr_elements) {
	    		DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
	    		dced_server_free_ptrs(server);
            		DCP_SET_MISC_ERROR(dcp_s_dhd_bad_attribute_list);
        	}  
		/*
		 * If the entryname has been parsed before, this one will
		 * overwrite the previous one
		 */
		DCP_DHD_STRING_FREE(server->entryname);
		server->entryname = (dced_string_t) dce_strdup(argv2[1]);
	} /* End of if (!strcasecmp(dhd_c_srvr_entryname_token, argv2[0])) */ 

	/*
	 * - gid 
	 */
	else if (!strcasecmp(dhd_c_srvr_gid_token, argv2[0])) {
        	if (argc2 != dhd_c_def_attr_elements) {
	    		DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
	    		dced_server_free_ptrs(server);
            		DCP_SET_MISC_ERROR(dcp_s_dhd_bad_attribute_list);
        	}  
		server->fixed.posix_gid = atol(argv2[1]);
	} /* End of if (!strcasecmp(dhd_c_srvr_gid_token, argv2[0])) */ 

	/*
	 * - keytables 
	 */
	else if ((!strcasecmp(dhd_c_srvr_keytab_token, argv2[0])) &&
		 (argc2 > 1)) {
		/*
		 * If the keytables has been parsed before, this one will
		 * overwrite the previous one
		 */
		if (!DCP_DHD_EMPTY_LIST(&server->fixed.keytables)) {
			dced_uuid_list_free_ptrs(&server->fixed.keytables);
		}
		DCP_DHD_MALLOC_LIST_RESET(&server->fixed.keytables, 
					  uuid_t,
					  (argc2-1),
					  status);
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
			dced_server_free_ptrs(server);
			return TCL_ERROR;
		}
		for (j=1; j<argc2; j++) {
			uuid_from_string((unsigned char *) argv2[j],
					 &server->fixed.keytables.list[j-1],
					 &status);
			if (!DCP_DHD_GOOD_STATUS(status)) {
				DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
				dced_server_free_ptrs(server);
				DCP_SET_MISC_ERROR(status);
			}
		}
	} /* End of if (!strcasecmp(dhd_c_srvr_keytab_token, argv2[0])) */

	/*
	 * - prerequisites
	 */
	else if ((!strcasecmp(dhd_c_srvr_prerequisite_token, argv2[0])) &&
		 (argc2 > 1)) {

		/*
		 * If the prerequisites has been parsed before, this one will
		 * overwrite the previous one
		 */
		if (!DCP_DHD_EMPTY_LIST(&server->fixed.prerequisites)) {
			dced_uuid_list_free_ptrs(&server->fixed.prerequisites);
		}
		DCP_DHD_MALLOC_LIST_RESET(&server->fixed.prerequisites, 
					  uuid_t,
					  (argc2-1),
					  status);
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
			dced_server_free_ptrs(server);
			return TCL_ERROR;
		}
		for (j=1; j<argc2; j++) {
			uuid_from_string((unsigned char *) argv2[j],
					 &server->fixed.prerequisites.list[j-1],
					 &status);
			if (!DCP_DHD_GOOD_STATUS(status)) {
				DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
				dced_server_free_ptrs(server);
				DCP_SET_MISC_ERROR(status);
			}
		}
	} /* End of if (!strcasecmp(dhd_c_srvr_prerequisite_token, argv2[0])) */

	/*
	 * Get prin_names 
	 */
	else if ((!strcasecmp(dhd_c_srvr_prin_name_token, argv2[0])) &&
		 (argc2 > 1)) {

		/*
		 * If the prin_names has been parsed before, this one will
		 * overwrite the previous one
		 */
		if (!DCP_DHD_EMPTY_LIST(&server->prin_names)) {
			dced_string_list_free_ptrs(&server->prin_names);
		}
		DCP_DHD_MALLOC_LIST_RESET(&server->prin_names, 
					  dced_string_t,
					  (argc2-1),
					  status);
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
			dced_server_free_ptrs(server);
			return TCL_ERROR;
		}
		for (j=1; j<argc2; j++) {
			server->prin_names.list[j-1] 
					= (dced_string_t) dce_strdup(argv2[j]);
		}
	} /* End of if (!strcasecmp(dhd_c_srvr_prin_name_token, argv2[0])) */

	/*
	 * - program
	 */
	else if (!strcasecmp(dhd_c_srvr_program_token, argv2[0])) {
        	if (argc2 != dhd_c_def_attr_elements) {
	    		DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
	    		dced_server_free_ptrs(server);
            		DCP_SET_MISC_ERROR(dcp_s_dhd_bad_attribute_list);
        	}  
		/*
		 * If the program has been parsed before, this one will
		 * overwrite the previous one
		 */
		DCP_DHD_STRING_FREE(server->fixed.program);
		server->fixed.program = (dced_string_t) dce_strdup(argv2[1]);
	} /* End of if (!strcasecmp(dhd_c_srvr_program_token, argv2[0])) */ 

	/*
	 * Get the server services 
	 */
        else if ((!strcasecmp(dhd_c_srvr_service_token, argv2[0])) &&
		 (argc2 > 1)) {

		/*
		 * If the service list has been parsed before, this one
		 * will overwrite the previous one completely
		 */
		if (!DCP_DHD_EMPTY_LIST(&server->services)) {
			dced_service_list_free_ptrs(&server->services);
		}

		list_ptr = Tcl_Merge(argc2-1, &argv2[1]);
		if (dhd__parse_services(interp, list_ptr, &server->services)
								   != TCL_OK) {
			DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
			dced_server_free_ptrs(server);
			free(list_ptr);
            		return TCL_ERROR;
        	} 
		free(list_ptr);

        } /* End of if (!strcasecmp(dhd_c_srvr_service_token, argv2[0])) */
	
	/*
	 * Get server_fixedattr_t
	 *  - startupflag
	 */
	else if ((!strcasecmp(dhd_c_srvr_starton_token, argv2[0])) &&
		 (argc2 > 1)) {

		for (j=1; j<argc2; j++) {
		    if (!strcasecmp(dhd_c_starton_boot_token, argv2[j])) {
			server->fixed.startupflags |= 
						server_c_startup_at_boot;
		    }
		    if (!strcasecmp(dhd_c_starton_auto_token, argv2[j])) {
			server->fixed.startupflags |= 
						server_c_startup_auto;
		    }
		    if (!strcasecmp(dhd_c_starton_explicit_token, argv2[j])) {
			server->fixed.startupflags |= 
						server_c_startup_explicit;
		    }
		    if (!strcasecmp(dhd_c_starton_failure_token, argv2[j])) {
			server->fixed.startupflags |= 
						server_c_startup_on_failure;
		    }
		    if (!strcasecmp(dhd_c_starton_schedule_token, argv2[j])) {
			server->fixed.startupflags |= 
						server_c_startup_on_schedule;
		    }
		}
	} /* if (!strcasecmp(dhd_c_srvr_starton_token,argv2[0])) */

	/*
	 * Get the uid
	 */
        else if (!strcasecmp(dhd_c_srvr_uid_token, argv2[0])) {
	    if (argc2 != dhd_c_def_attr_elements) {
	    	DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
	    	dced_server_free_ptrs(server);
                DCP_SET_MISC_ERROR(dcp_s_dhd_bad_attribute_list);
	    }
	    server->fixed.posix_uid = atol(argv2[1]);
        } /* End of if (!strcasecmp(dhd_c_srvr_uid_token, argv2[0])) */

	/*
	 * Get the uuid for the object
	 */
        else if (!strcasecmp(dhd_c_attr_id_token, argv2[0])) {
	    if (argc2 != dhd_c_def_attr_elements) {
		DCP_DHD_SET_STATUS(status, dcp_s_dhd_bad_attribute_list);
	    }
	    else {
	    	uuid_from_string((unsigned char *) argv2[1], 
               	                 &server->id, 
                                 &status);  
	    }
	    if (!DCP_DHD_GOOD_STATUS(status)) {
	    	DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
	    	dced_server_free_ptrs(server);
                DCP_SET_MISC_ERROR(status);
            }
        } /* End of if (!strcasecmp(dhd_c_attr_id_token, argv2[0])) */

	else {
		dced_server_free_ptrs(server);
		DCP_DHD_SET_RESULT(dcp_s_bad_attribute, argv2[0]);
		DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
		return TCL_ERROR;
	} /* End of else */

    	DCP_DHD_ARGV_FREE(argc2, argv2);
    } /* End of for (i=0; i<argc1; i++) */ 

    DCP_DHD_ARGV_FREE(argc1, argv1);
    return TCL_OK;
} /* End of dhd__parse_server */



/*
 * FUNCTION: dhd__parse_keys
 *
 * OVERVIEW:
 *
 * This routine will parse the key data and assign it to a list
 * of dced_key_t.
 *
 * INPUTS:
 * Tcl_Interp	*interp
 * char 	*list - the tcl list for key data  
 * boolean32	random_gen - if generate a random des key
 *
 * OUTPUTS:
 * dced_key_list_t *keys
 *
 * RETURNS:
 * TCL_OK on success, TCL_ERROR on failure
 */
int
dhd__parse_keys(Tcl_Interp  	*interp,
                char         	*list,
		boolean32	random_gen,
	        dced_key_list_t	*keys)
{
    int			argc1, argc2, argc3;
    char		**argv1, **argv2, **argv3;
    int			i, j;
    error_status_t	status;

    DCP_DHD_CLEAR_STATUS(status);
    DCP_DHD_CLEAR_ARGV_3(argc1, argv1, argc2, argv2, argc3, argv3);

    /*
     * Check to see if the list is a list of key data or
     * just one
     */
    if (isllist(list)) {
	if (Tcl_SplitList(interp, list, &argc1, &argv1) != TCL_OK) {
		return TCL_ERROR;
	}
    }
    else {
	argc1 = 1;
	DCP_DHD_ARGV_ALLOC(argc1, argv1);
	argv1[0] = list;
    }

    /*
     * Allocate the dced key list 
     */
    DCP_DHD_MALLOC_LIST_RESET(keys, dced_key_t, argc1, status);
    if (!DCP_DHD_GOOD_STATUS(status)) {
	DCP_DHD_ARGV_FREE(argc1, argv1);
	return TCL_ERROR;
    }
    /*
     * Parse thr each tcl list  for a key
     */
    for (i=0; i<argc1; i++) {
        if (Tcl_SplitList(interp, argv1[i], &argc2, &argv2) != TCL_OK) {
		DCP_DHD_ARGV_FREE(argc1, argv1);
		dced_key_list_free_ptrs(keys);
		return TCL_ERROR;
        }

	if ((argc2 < dhd_c_min_key_data_size) ||
	    (argc2 > dhd_c_max_key_data_size)) {
		DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
		dced_key_list_free_ptrs(keys);
		DCP_SET_MISC_ERROR(dcp_s_dhd_bad_key_data);
	}
	/*
	 * Copy each tcl key list to dced key structure
	 *
     	 * The Each key data must have a principal name and key type,
     	 * and rest (version number and key value) may be optional.
    	 * In case of no version number specified, a next available
         * version number will be used.
    	 * In case of no key value specified, a randomly generated 
    	 * key will be used if random_gen is true.
    	 */
	DCP_DHD_MALLOC_RESET(keys->list[i].passwd, sec_passwd_rec_t, 
			     1, status);
	if (!DCP_DHD_GOOD_STATUS(status)) {
		DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
		dced_key_list_free_ptrs(keys);
		return TCL_ERROR;	
	}
	*keys->list[i].passwd = s_null_passwd;
    	keys->list[i].principal = (dced_string_t) dce_strdup(argv2[0]);
    	if (!strcasecmp(argv2[1], dhd_c_key_type_des_token)) {
		keys->list[i].passwd->key.key_type = sec_passwd_des;
		/*
	 	 * For des key, both version number and key value may be 
		 * optional
		 */
		if (argc2 == 3) {
		   /*
		    * Only one of version number and des key has been 
		    * supplied.
		    */
                   if (strchr(argv2[2], ' ') == NULL) {
		       if (!random_gen) {
		           DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
			   dced_key_list_free_ptrs(keys);
			   DCP_SET_MISC_ERROR(dcp_s_kt_key_value_missing);
		       }
		       keys->list[i].passwd->version_number = atoi(argv2[2]); 
                   }
                   else {
                       if (Tcl_SplitList(interp, argv2[2], &argc3, &argv3) 
                                                              != TCL_OK) {
		           DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
		           dced_key_list_free_ptrs(keys);
		           return TCL_ERROR;
                       }
                       if (argc3 != sec_passwd_c_des_key_size) { 
			      dced_key_list_free_ptrs(keys);
			      DCP_DHD_SET_RESULT(dcp_s_kt_bad_key_value,
					         argv2[2]);
			      DCP_DHD_ARGV_FREE_3(argc1, argv1, argc2, argv2,
                                                  argc3, argv3);
			      return TCL_ERROR;	
                        }
			for (j=0; j<sec_passwd_c_des_key_size; j++) {
			      keys->list[i].passwd->key.tagged_union.des_key[j]
					    = (byte)strtoul(argv3[j], NULL, 16);
			}
			DCP_DHD_ARGV_FREE(argc3, argv3);
                   }
		} /* End of if (argc2 == 3) */

		else if (argc2 == 4) {
		    /*
		     * Both version number and des key has been supplied.
		     */
		     keys->list[i].passwd->version_number = atoi(argv2[2]);
                     if (strchr(argv2[3], ' ') == NULL) {
			 DCP_DHD_SET_RESULT(dcp_s_kt_bad_key_value, argv2[3]);
		         DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
			 dced_key_list_free_ptrs(keys);
			 return TCL_ERROR;	
		     }
                     if (Tcl_SplitList(interp, argv2[3], &argc3, &argv3) 
                                                            != TCL_OK) {
		         DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
		         dced_key_list_free_ptrs(keys);
		         return TCL_ERROR;
                     }
                     if (argc3 != sec_passwd_c_des_key_size) { 
			 dced_key_list_free_ptrs(keys);
			 DCP_DHD_SET_RESULT(dcp_s_kt_bad_key_value, argv2[3]);
			 DCP_DHD_ARGV_FREE_3(argc1, argv1, argc2, argv2,
                                             argc3, argv3);
			 return TCL_ERROR;	
                     }
		     for (j=0; j<sec_passwd_c_des_key_size; j++) {
		         keys->list[i].passwd->key.tagged_union.des_key[j]
					    = (byte)strtoul(argv3[j], NULL, 16);
		     }
		     DCP_DHD_ARGV_FREE(argc3, argv3);
		} /* End of if (argc2 == 4) */
		else {
			/*
			 * Neither of version number and des key has been 
			 * supplied.
			 */
		        if (!random_gen) {
			      DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
			      dced_key_list_free_ptrs(keys);
			      DCP_SET_MISC_ERROR(dcp_s_kt_key_value_missing);
		        }
		} /* End of else of if (argc2 == 4) */

    	} /* End of if (!strcasecmp(argv2[1], dhd_c_key_type_des_token)) */

    	else if (!strcasecmp(argv2[1], dhd_c_key_type_plain_token)) {
		keys->list[i].passwd->key.key_type = sec_passwd_plain;
		/*
	 	 * For plain key, Only version number is optional. 
		 * The key value has to be presented, otherwise it's
		 * error.
		 */
		if (argc2 == 3) {
			/*
			 * Only the plain key has been supplied.
			 */
			keys->list[i].passwd->key.tagged_union.plain 
		   		= (unsigned char *) dce_strdup(argv2[2]);
		} /* End of if (argc2 == 3) */

		else if (argc2 == 4) {
			/*
			 * Both version number and the plain key has been 
			 * supplied.
			 */
			keys->list[i].passwd->version_number = atoi(argv2[2]);
			keys->list[i].passwd->key.tagged_union.plain 
		   		= (unsigned char *) dce_strdup(argv2[3]);
		} /* End of else if (argc2 == 4) */

		else {
			/*
			 * Neither of version number and the plain key has 
			 * been supplied -): a error
			 */
			DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
			dced_key_list_free_ptrs(keys);
			DCP_SET_MISC_ERROR(dcp_s_kt_key_value_missing);
		} /* End of else of if (argc2 == 4) */

    	} /* End of if (!strcasecmp(argv2[1], dhd_c_key_type_plain_token)) */
        
	else {
		DCP_DHD_ARGV_FREE_2(argc1, argv1, argc2, argv2);
		dced_key_list_free_ptrs(keys);
		DCP_SET_MISC_ERROR(dcp_s_kt_bad_key_type);
	} /* End of else */

    	/*
     	 * Initialize version and authn_service
     	 */
	keys->list[i].version = keys->list[i].passwd->version_number;
    	keys->list[i].authn_service = dhd_c_key_authn_service;

	DCP_DHD_ARGV_FREE(argc2, argv2);
    } /* End of for (i=0; i<argc1; i++) */

    /*
     * Free memory and return
     */
    DCP_DHD_ARGV_FREE(argc1, argv1);
    return TCL_OK;
} /* End of dhd__parse_key */


/*
 * FUNCTION: dhd__show_entry
 *
 * OVERVIEW:
 *
 * This routine will show the entry attributes
 *
 * INPUTS:
 * Tcl_Interp       *interp
 * dced_entry_t     entry - the entry
 * char		    *ns_entry
 * dhd_show_flag_t  flag - which attribute is shown
 *
 * RETURNS:
 * TCL_OK on success, TCL_ERROR on failure  
 */
int
dhd__show_entry(Tcl_Interp      *interp,
                dced_entry_t    *entry,
                char            *ns_entry,
                dhd_show_flag_t flag)
{
    unsigned char   *string_uuid;
    char            *show_buf;
    int             show_buf_len;
    char            buf[1024];
    error_status_t  status;

    DCP_DHD_CLEAR_STATUS(status);

    switch (flag) {
        case dhd_e_show_name:
            if (ns_entry) {
                show_buf_len = strlen(ns_entry) + strlen((char *)entry->name) + 1;
                DCP_DHD_MALLOC_RESET_RET(show_buf, char, show_buf_len);
                strcpy(show_buf, ns_entry);
                strcat(show_buf, (char *)entry->name);
                DCP_APPEND_RESULT((unsigned char *)show_buf);
                free(show_buf);
            }
            else {
                DCP_APPEND_RESULT(entry->name);
            }
            break;

        case dhd_e_show_all:
            uuid_to_string(&entry->id, &string_uuid, &status);
            DCP_DHD_CHECK_STATUS_RET(status);
            if (strcmp(interp->result, "") != 0) {
                Tcl_AppendResult(interp, "\n", (char *)NULL);
            }
            sprintf(buf, "{%s %s}", DCP_T_DHD_UUID_HEADER, string_uuid);
            Tcl_AppendResult(interp, buf, "\n", (char *)NULL);
            rpc_string_free(&string_uuid, &status);
            DCP_DHD_CHECK_STATUS_RET(status);

            /*
             * Don't need to show entry name, because it's on cmd line
             */
            Tcl_AppendResult(interp, "{", DCP_T_DHD_ANNOTATION_HEADER, (char *)NULL);
            Tcl_AppendElement(interp, (char *) entry->description);
            Tcl_AppendResult(interp, "}\n", (char *)NULL);
            sprintf(buf, "{%s %s}", DCP_T_DHD_STORAGE_HEADER, entry->storage_tag);
            Tcl_AppendResult(interp, buf, (char *)NULL);
            break;

        default:
            break;
    }
    return TCL_OK;
}


/*
 * FUNCTION: dhd__show_object
 *
 * OVERVIEW:
 *
 * This routine will show the object attributes
 *
 * INPUTS:
 * Tcl_Interp           *interp
 * void                 *object - the object 
 * dced_service_type_t  service - which dced service this object belongs to
 * dhd_show_flag_t      flag - which attribute is shown
 *
 * RETURNS:
 * TCL_OK on success, TCL_ERROR on failure  
 */
int
dhd__show_object(Tcl_Interp          *interp,
                 void                *object,
                 dced_service_type_t service,
                 dhd_show_flag_t     flag)
{
    sec_attr_t                    *hd_data;
    dced_key_list_t               *kt_data;
    server_t                      *server;
    int                           i, j, k, bindings_serv;
    rpc_tower_vector_t            *twr_ptr;
    rpc_binding_handle_t          twr_handle;
    char                          buf[1024];
    char                          *tmp_buf;
    unsigned char                 *string_uuid, *string_binding;
    extern dce_attr_sch_handle_t  attr_schema_binding;
    dce_attr_schema_entry_t       schema_entry;
    error_status_t                status, status1;

    DCP_DHD_CLEAR_STATUS_2(status, status1);

    if ((!object) || ((service == dced_e_service_type_keytab) && 
        (DCP_DHD_EMPTY_LIST((dced_key_list_t *) object)))) {
        return TCL_OK;
    }

    switch (service) {
        case dced_e_service_type_hostdata:
            hd_data = (sec_attr_t *)object;

            if (attr_schema_binding != NULL) {
                /*
                 * Use the attribute binding handle to look up the
                 * attribute by id.  The attribute name and encoding
                 * type will be used and displayed.
                 */
                dce_attr_sch_lookup_by_id(attr_schema_binding,
                                          &hd_data->attr_id,
                                          &schema_entry,
                                          &status);
                DCP_DHD_CHECK_STATUS_RET(status);

                if (flag == dhd_e_show_name) {
                    DCP_APPEND_RESULT(schema_entry.attr_name);
                }
                else if (flag == dhd_e_show_all) {
                    Tcl_AppendResult(interp, "\n{", schema_entry.attr_name, NULL);
                    if (print_attrs(interp, hd_data, schema_entry.attr_encoding) != TCL_OK) { 
#ifndef	HPDCE_DCED_LOCAL
			dhd__schema_ent_free_ptrs(&schema_entry);
#else
                        sec_attr_util_sch_ent_free_ptrs(&schema_entry);
#endif	/* HPDCE_DCED_LOCAL */
                        return TCL_ERROR;
                    }
                    Tcl_AppendResult(interp, "}", NULL);
                }
#ifndef	HPDCE_DCED_LOCAL
		dhd__schema_ent_free_ptrs(&schema_entry);
#else
                sec_attr_util_sch_ent_free_ptrs(&schema_entry);
#endif	/* HPDCE_DCED_LOCAL */
            }
            else {
                /*
                 * We don't have a binding to the attribute schema, so fake it.
                 */
                if (flag == dhd_e_show_name) {
                    DCP_APPEND_RESULT(DCP_T_DHD_GENERIC_HEADER);
                }
                else if (flag == dhd_e_show_all) {
                    Tcl_AppendResult(interp, "\n{", DCP_T_DHD_GENERIC_HEADER, NULL);
                    if (print_attrs(interp, hd_data, sec_attr_enc_printstring_array) != TCL_OK) { 
                        return TCL_ERROR;
                    }
                    Tcl_AppendResult(interp, "}", NULL);
                }
            }
            break;

        case dced_e_service_type_keytab:
            kt_data = (dced_key_list_t *)object;

            for (i = 0; i < kt_data->count; i++) {
                if (flag == dhd_e_show_name) {
                    DCP_APPEND_RESULT(kt_data->list[i].principal);
                }
                else {
                    sprintf(buf, "{%s ", kt_data->list[i].principal);
                    DCP_APPEND_RESULT(buf);
                    switch (kt_data->list[i].passwd->key.key_type) {
                        case sec_passwd_none:
                            Tcl_AppendResult(interp, DCP_T_DHD_NONE_PW, (char *)NULL);
                            break;
                        case sec_passwd_plain:
                            Tcl_AppendResult(interp, DCP_T_DHD_PLAIN_PW, (char *)NULL);
                            break;
                        case sec_passwd_des:
                            Tcl_AppendResult(interp, DCP_T_DHD_DES_PW, (char *)NULL);
                            break;
                        default:
                            Tcl_AppendResult(interp, DCP_T_DHD_UNKNOWN_PW, (char *)NULL);
                            break;
                    }
                    sprintf(buf, " %d", kt_data->list[i].passwd->version_number);
                    Tcl_AppendResult(interp, buf, (char *)NULL);

                    if (flag == dhd_e_show_all) {
                        switch (kt_data->list[i].passwd->key.key_type) {
                            case sec_passwd_plain:
                                sprintf(buf, "%s",
                                        kt_data->list[i].passwd->key.tagged_union.plain);
                                Tcl_AppendResult(interp, " ", buf, (char *)NULL);
                                break;
                            case sec_passwd_des:
                                Tcl_AppendResult(interp, " ", "{", (char *)NULL);
                                for (j = 0; j < sec_passwd_c_des_key_size; j++) {
                                    sprintf(buf, "%2.2x",
                                            kt_data->list[i].passwd->key.tagged_union.des_key[j]);
                                    if (j < (sec_passwd_c_des_key_size - 1)) {
                                        Tcl_AppendResult(interp, buf, " ", (char *)NULL);
                                    }
                                }
                                Tcl_AppendResult(interp, buf, "}", (char *)NULL);
                                break;
                            default:
                                break;
                        }
                    }
                    Tcl_AppendResult(interp, "}", (char *)NULL);    
                }
            }
            break;

        case dced_e_service_type_srvrconf:
        case dced_e_service_type_srvrexec:
            server = (server_t *)object;

            if (flag == dhd_e_show_name) {
                DCP_APPEND_RESULT((char *)server->name);
            }
            else if (flag == dhd_e_show_all) {
                /*
                 * Show object uuid
                 */
                 uuid_to_string(&server->id, &string_uuid, &status);
                 DCP_DHD_CHECK_STATUS_RET(status);
                 sprintf(buf, "{%s %s}", DCP_T_DHD_UUID_HEADER, string_uuid);
                 DCP_APPEND_RESULT(buf);
                 rpc_string_free(&string_uuid, &status);
                 DCP_DHD_CHECK_STATUS_RET(status);

                /*
                 * - program
                 */
                if (server->fixed.program) {
                    sprintf(buf, "{%s %s}", DCP_T_DHD_PROGRAM_HEADER, server->fixed.program);
                    DCP_APPEND_RESULT(buf);
                }
                else {
                    sprintf(buf, "{%s {}}", DCP_T_DHD_PROGRAM_HEADER);
                    DCP_APPEND_RESULT(buf);
                }

                /*
                 * - arguments
                 */
                sprintf(buf, "{%s", DCP_T_DHD_ARGUMENTS_HEADER);
                DCP_APPEND_RESULT(buf);
                switch ((int)server->fixed.arguments.count) {
                    case 0:
                        Tcl_AppendResult(interp, " {}", (char *) NULL);
                        break;
                    case 1:
                        Tcl_AppendResult(interp, " ",
                                         server->fixed.arguments.list[0], 
                                         (char *) NULL); 
                        break;
                    default:
                        for (i=0; i<server->fixed.arguments.count; i++) {
                            sprintf(buf, "{%s}", server->fixed.arguments.list[i]);
                            Tcl_AppendResult(interp, "\n", dhd_c_util_leading_space,
                                             buf, (char *)NULL);
                        }
                        break;
                }
                Tcl_AppendResult(interp, "}", (char *)NULL);

                /*
                 * - Prerequisites
                 */
                sprintf(buf, "{%s", DCP_T_DHD_PREREQUISITES_HEADER);
                DCP_APPEND_RESULT(buf);
                switch ((int) server->fixed.prerequisites.count) {
                    case 0:
                        Tcl_AppendResult(interp, " {}", (char *) NULL);
                        break;
                    case 1:
                        uuid_to_string(&server->fixed.prerequisites.list[0],
                                       &string_uuid,
                                       &status);
                        DCP_DHD_CHECK_STATUS_RET(status);
                        Tcl_AppendResult(interp, " ", string_uuid, (char *)NULL);
                        rpc_string_free(&string_uuid, &status);
                        DCP_DHD_CHECK_STATUS_RET(status);
                        break;
                    default:
                        for (i=0; i<server->fixed.prerequisites.count; i++) {
                            uuid_to_string(&server->fixed.prerequisites.list[i],
                                           &string_uuid,
                                           &status);
                            DCP_DHD_CHECK_STATUS_RET(status);
                            sprintf(buf, "{%s}", string_uuid);
                            Tcl_AppendResult(interp, "\n", dhd_c_util_leading_space,
                                             buf, (char *)NULL);
                            rpc_string_free(&string_uuid, &status);
                            DCP_DHD_CHECK_STATUS_RET(status);
                        }
                        break;
                }
                Tcl_AppendResult(interp, "}", (char *)NULL);

                /*
                 * - keytables 
                 */
                sprintf(buf, "{%s", DCP_T_DHD_KEYTABS_HEADER);
                DCP_APPEND_RESULT(buf);
                switch ((int) server->fixed.keytables.count) {
                    case 0:
                        Tcl_AppendResult(interp, " {}", (char *)NULL);
                        break;
                    case 1:
                        uuid_to_string(&server->fixed.keytables.list[0],
                                       &string_uuid,
                                       &status);
                        DCP_DHD_CHECK_STATUS_RET(status);
                        Tcl_AppendResult(interp, " ", string_uuid, (char *)NULL);
                        rpc_string_free(&string_uuid, &status);
                        DCP_DHD_CHECK_STATUS_RET(status);
                        break;
                    default:
                        for (i=0; i<server->fixed.keytables.count; i++) {
                            uuid_to_string(&server->fixed.keytables.list[i],
                                           &string_uuid,
                                           &status);
                            DCP_DHD_CHECK_STATUS_RET(status);
                            sprintf(buf, "{%s}", string_uuid);
                            Tcl_AppendResult(interp, "\n", dhd_c_util_leading_space,
                                             buf, (char *)NULL);
                            rpc_string_free(&string_uuid, &status);
                            DCP_CHECK_RPC_ERROR(status);
                        }
                        break;
                }
                Tcl_AppendResult(interp, "}", (char *) NULL);

                /*
                 * - entryname 
                 */
                if (server->entryname) {
                    sprintf(buf, "{%s %s}", DCP_T_DHD_ENTRYNAME_HEADER, server->entryname);
                    DCP_APPEND_RESULT(buf);
                }
                else {
                    sprintf(buf, "{%s {}}", DCP_T_DHD_ENTRYNAME_HEADER);
                    DCP_APPEND_RESULT(buf);
                }

                /*
                 * Show the service list
                 */
                sprintf(buf, "{%s", DCP_T_DHD_SERVICES_HEADER);
                DCP_APPEND_RESULT(buf);
                switch ((int) server->services.count) {
                    case 0:
                        Tcl_AppendResult(interp, " {}", (char *) NULL);
                        break;
                    default:
                        for (i=0; i<server->services.count; i++) {
                            /*
                             * Show ifname
                             */
                            Tcl_AppendResult(interp, "\n", dhd_c_util_leading_space,
                                             "{{", DCP_T_DHD_IFNAME_HEADER, (char *) NULL);
                            /*
                             * Note Tcl_AppendElement will add a separator space
                             * before the converted list element unless the current 
                             * result is empty, contains the single character "{", 
                             * or ends in " {
                             */
                            Tcl_AppendElement(interp, 
                                              (char *) server->services.list[i].ifname);
                            Tcl_AppendResult(interp, "}", (char *) NULL);

                            /*
                             * Show annotation
                             */
                            Tcl_AppendResult(interp, "\n", dhd_c_util_leading_space1,
                                             "{", DCP_T_DHD_ANNOTATION_HEADER, (char *) NULL);
                            /*
                             * Note Tcl_AppendElement will add a separator space
                             * before the converted list element unless the current 
                             * result is empty, contains the single character "{", 
                             * or ends in " {
                             */
                            Tcl_AppendElement(interp, 
                                              (char *) server->services.list[i].annotation);
                            Tcl_AppendResult(interp, "}", (char *) NULL);

                            /*
                             * show rpc_if_id_t
                             */
                            if (!uuid_is_nil(&server->services.list[i].ifspec.uuid, &status1)) {
                                uuid_to_string(&server->services.list[i].ifspec.uuid,
                                               &string_uuid,
                                               &status);
                                DCP_DHD_CHECK_STATUS_RET(status);
                                sprintf(buf, "{%s {%s %d.%d}}",
                                        DCP_T_DHD_INTERFACE_HEADER, string_uuid,
                                        server->services.list[i].ifspec.vers_major,
                                        server->services.list[i].ifspec.vers_minor);
                                Tcl_AppendResult(interp, "\n", dhd_c_util_leading_space1,
                                                 buf, (char *) NULL);
                                rpc_string_free(&string_uuid, &status);
                                DCP_DHD_CHECK_STATUS_RET(status);
                            }
                            else {
                                Tcl_AppendResult(interp, "\n", dhd_c_util_leading_space1,
                                                 "{", DCP_T_DHD_INTERFACE_HEADER,
                                                 " {}}", (char *) NULL);
                            }

                            /*
                             * Show towers - bindings
                             */
                            Tcl_AppendResult(interp, "\n", dhd_c_util_leading_space1,
                                             "{", DCP_T_DHD_BINDINGS_HEADER, (char *) NULL);

                            bindings_serv = server->services.list[i].towers.count;
                            for (j=0; j<server->services.list[i].towers.count; j++) {
                                twr_ptr = (rpc_tower_vector_t *) 
                                          server->services.list[i].towers.list[j];
                                bindings_serv *= twr_ptr->count;           
                                for (k=0; k<twr_ptr->count; k++) {
                                    rpc_tower_to_binding(
                                               twr_ptr->tower[k]->tower_octet_string,
                                               &twr_handle, &status);
                                    if (DCP_DHD_GOOD_STATUS(status)) {
                                        rpc_binding_to_string_binding(twr_handle,
                                                                      &string_binding,
                                                                      &status);
                                        rpc_binding_free(&twr_handle, &status1);
                                    }

                                    /*
                                     * if the twr is untansferable, show it in bytes
                                     */
                                    if (!DCP_DHD_GOOD_STATUS(status)) {
                                        tmp_buf = dcp_binding_to_bytes(
                                                           twr_ptr->tower[k],
                                                           dhd_c_util_leading_space2,
                                                           &status); 
                                        DCP_DHD_CHECK_STATUS_RET(status);
                                        Tcl_AppendResult(interp, "\n",
                                                         tmp_buf, (char *) NULL);
                                    }
                                    else {
                                        tmp_buf = dcp_binding_string_to_tcl(
                                                           (char *) string_binding,
                                                           &status);
                                        rpc_string_free(&string_binding, &status1);
                                        DCP_DHD_CHECK_STATUS_RET(status);
                                        sprintf(buf, "{%s}", tmp_buf);
                                        if (bindings_serv == 1) { 
                                            Tcl_AppendResult(interp, " ",
                                                             buf, (char *) NULL);
                                        }
                                        else {
                                            Tcl_AppendResult(interp, "\n",
                                                         dhd_c_util_leading_space2,
                                                         buf, (char *) NULL);
                                        }
                                    }
                                    free(tmp_buf);
                                    DCP_CHECK_RPC_ERROR(status);
                                }
                            }
                            if (!bindings_serv) {
                                Tcl_AppendResult(interp, " {}", (char *) NULL);
                            }
                            Tcl_AppendResult(interp, "}", (char *) NULL);

                            /*
                             * Show object uuids
                             */
                            Tcl_AppendResult(interp, "\n", dhd_c_util_leading_space1,
                                             "{", DCP_T_DHD_OBJECTS_HEADER, (char *) NULL);
                            switch ((int)server->services.list[i].objects.count) {
                                case 0:
                                    Tcl_AppendResult(interp, " {}", (char *) NULL);
                                    break;
                                case 1:
                                    uuid_to_string(
                                              &server->services.list[i].objects.list[0],
                                              &string_uuid, &status);
                                    DCP_DHD_CHECK_STATUS_RET(status);
                                    Tcl_AppendResult(interp, " ", string_uuid, (char *)NULL);
                                    rpc_string_free(&string_uuid, &status);
                                    DCP_DHD_CHECK_STATUS_RET(status);
                                    break;
                                default:
                                    for (j=0; j<server->services.list[i].objects.count; j++) {
                                        uuid_to_string(
                                               &server->services.list[i].objects.list[j],
                                               &string_uuid, &status);
                                        DCP_DHD_CHECK_STATUS_RET(status);
                                        sprintf(buf, "{%s}", string_uuid);
                                        Tcl_AppendResult(interp, "\n",
                                                         dhd_c_util_leading_space2,
                                                         buf, (char *) NULL);
                                        rpc_string_free(&string_uuid, &status);
                                        DCP_DHD_CHECK_STATUS_RET(status);
                                    }
                                    break;
                            }
                            Tcl_AppendResult(interp, "}", (char *) NULL);

                            /*
                             * Show the disabled flag
                             */
                            if (server->services.list[i].flags == service_c_disabled) {
                                Tcl_AppendResult(interp, "\n",
                                                 dhd_c_util_leading_space1,
                                                 "{", DCP_T_DHD_FLAGS_HEADER, " ",
                                                 DCP_T_DHD_DISABLED_FLAG, "}",
                                                 (char *) NULL);
                            }
                            else {
                                Tcl_AppendResult(interp, "\n",
                                                 dhd_c_util_leading_space1,
                                                 "{", DCP_T_DHD_FLAGS_HEADER, " {}}",
                                                 (char *) NULL);
                            }

                            /*
                             * Show the entry name for the service
                             */
                            if (server->services.list[i].entryname) {
                                sprintf(buf, "{%s %s}}", DCP_T_DHD_ENTRYNAME_HEADER,
                                        server->services.list[i].entryname);
                                Tcl_AppendResult(interp, "\n", 
                                                 dhd_c_util_leading_space1,
                                                 buf, (char *) NULL);
                            }
                            else {
                                Tcl_AppendResult(interp, "\n",
                                                 dhd_c_util_leading_space1,
                                                 "{", DCP_T_DHD_ENTRYNAME_HEADER,
                                                 " {}}}", (char *) NULL);
                            }
                        }
                        break;
                }
                Tcl_AppendResult(interp, "}", (char *) NULL);

                /*
                 * - principal names
                 */
                sprintf(buf, "{%s", DCP_T_DHD_PRINCIPALS_HEADER);
                DCP_APPEND_RESULT(buf);
                switch ((int) server->prin_names.count) {
                    case 0:
                        Tcl_AppendResult(interp, " {}", (char *) NULL);
                        break;
                    case 1:
                       Tcl_AppendResult(interp, " ", server->prin_names.list[0], (char *)NULL); 
                       break;
                    default:
                        for (i=0; i<server->prin_names.count; i++) {
                            sprintf(buf, "{%s}", server->prin_names.list[i]);
                            Tcl_AppendResult(interp, "\n", dhd_c_util_leading_space, buf, NULL);
                        }
                        break;
                }
                Tcl_AppendResult(interp, "}", (char *) NULL);

                /*
                 * Show server_fixedattr_t
                 * - startup flags
                 */
                sprintf(buf, "{%s", DCP_T_DHD_STARTON_HEADER);
                DCP_APPEND_RESULT(buf);
                if (server->fixed.startupflags & server_c_startup_at_boot) {
                    Tcl_AppendResult(interp, " ", DCP_T_DHD_BOOT_POLICY, (char *) NULL);
                }
                if (server->fixed.startupflags & server_c_startup_auto) {
                    Tcl_AppendResult(interp, " ", DCP_T_DHD_AUTO_POLICY, (char *) NULL);
                }
                if (server->fixed.startupflags & server_c_startup_explicit) {
                    Tcl_AppendResult(interp, " ", DCP_T_DHD_EXPLICIT_POLICY, (char *) NULL);
                }
                if (server->fixed.startupflags & server_c_startup_on_failure) {
                    Tcl_AppendResult(interp, " ", DCP_T_DHD_FAILURE_POLICY, (char *) NULL);
                }
                if (server->fixed.startupflags & server_c_startup_on_schedule) {
                    Tcl_AppendResult(interp, " ", DCP_T_DHD_SCHEDULE_POLICY, (char *) NULL);
                }
                if (!server->fixed.startupflags) {
                    Tcl_AppendResult(interp, " {}", (char *) NULL);
                }
                Tcl_AppendResult(interp, "}", (char *) NULL);

                /*
                 * - posix_uid
                 */
                sprintf(buf, "{%s %d}", DCP_T_DHD_UID_HEADER, server->fixed.posix_uid);
                DCP_APPEND_RESULT(buf);

                /*
                 * - posix_gid
                 */
                sprintf(buf, "{%s %d}", DCP_T_DHD_GID_HEADER, server->fixed.posix_gid);
                DCP_APPEND_RESULT(buf);
 
                /*
                 * - posix_dir
                 */
                if (server->fixed.posix_dir) {
                    sprintf(buf, "{%s %s}", DCP_T_DHD_DIR_HEADER, server->fixed.posix_dir);
                    DCP_APPEND_RESULT(buf);
                }
                else {
                    sprintf(buf, "{%s {}}", DCP_T_DHD_DIR_HEADER);
                    DCP_APPEND_RESULT(buf);
                }
 
                /*
                 * Show variable attribute value
                 */
                for (i=0; i<server->attributes.count; i++) {
                    /*
                     * Lookup by attr_id
                     */
                    dce_attr_sch_lookup_by_id(attr_schema_binding, 
                                              &server->attributes.list[i].attr_id,
                                              &schema_entry,
                                              &status);
                    DCP_DHD_CHECK_STATUS_RET(status);

                    Tcl_AppendResult(interp, "\n{", schema_entry.attr_name, NULL);
                    if (print_attrs(interp,
                                    &server->attributes.list[i],
                                    schema_entry.attr_encoding) != TCL_OK) {
#ifndef	HPDCE_DCED_LOCAL
			dhd__schema_ent_free_ptrs(&schema_entry);
#else
                        sec_attr_util_sch_ent_free_ptrs(&schema_entry);
#endif	/* HPDCE_DCED_LOCAL */
                        return TCL_ERROR;
                    }
                    Tcl_AppendResult(interp, "}", NULL);
#ifndef	HPDCE_DCED_LOCAL
		    dhd__schema_ent_free_ptrs(&schema_entry);
#else
                    sec_attr_util_sch_ent_free_ptrs(&schema_entry);
#endif	/* HPDCE_DCED_LOCAL */
                }
 
                /*
                 * Show the server running data (srvrexec_data_t)
                 */
                if (service == dced_e_service_type_srvrexec) {
                    /*
                     * Show srvrexecdata_t
                     */
                    switch ((int)server->exec_data.execstate) {
                        case server_c_exec_notrunning:
                            break;
                        case server_c_exec_running:
                            uuid_to_string(
                                  &server->exec_data.tagged_union.running_data.sc_uuid,
                                  &string_uuid, &status);
                            DCP_DHD_CHECK_STATUS_RET(status);
                            sprintf(buf, "{%s", DCP_T_DHD_EXECUTING_HEADER);
                            DCP_APPEND_RESULT(buf);
                            sprintf(buf, "{%s %s}", DCP_T_DHD_UUID_HEADER, string_uuid);
                            Tcl_AppendResult(interp, "\n", dhd_c_util_leading_space,
                                             buf, (char *) NULL);
                            rpc_string_free(&string_uuid, &status);
                            DCP_DHD_CHECK_STATUS_RET(status);

                            sprintf(buf, "{%s %d}}", DCP_T_DHD_PID_HEADER,
                                 server->exec_data.tagged_union.running_data.posix_pid);
                            Tcl_AppendResult(interp, "\n", dhd_c_util_leading_space,
                                             buf, (char *) NULL);
                            break; 
                        default: 
                            break;
                    }
                }
            }
            break;

        case dced_e_service_type_null:
        case dced_e_service_type_ep:
        case dced_e_service_type_secval:
        default:
            break;
    }
    return TCL_OK;
}


/*
 * FUNCTION: dhd__schema_name_from_dced_name
 *
 * OVERVIEW:
 *
 * This routine will genernate a dce_attr_component name from 
 * a dced name 
 *
 * INPUTS:
 * Tcl_Interp			*interp
 * char                 	*dced_name 
 *
 * OUTPUTS:
 * dce_attr_component_name_t	*schema_name
 *
 * RETURNS:
 * TCL_OK on success, TCL_ERROR on failure  
 */
int
dhd__schema_name_from_dced_name(
	Tcl_Interp			*interp,
	char				*dced_name,
	dce_attr_component_name_t	*schema_name
)
{
	
    char			*cp, *cpp, *serv_entryname;
    int				len, len1;
    error_status_t		status;

    DCP_DHD_CLEAR_STATUS(status);

    /*
     * There could be the following formats of input parameter
     * dced_name,
     *      - /.../cellname/host/config/service
     *      - /.:/host/config/service
     *      - service@host
     *      - service
     *
     * The host should have the format as "hosts/name" with or 
     * without cell name, where the name is the return of posix 
     * command "uname -n", for example foster.  In the last 
     * case the local host and local cell will be assumed.
     *
     * From the dced_name, we need to generate the name of 
     * desired dced entry
     */
    len = dhd_c_attr_schema_token_len + 1;
    if (cp = strrchr(dced_name, '/')) {
		len1 = cp - dced_name;
                len += len1;
		DCP_DHD_MALLOC_RESET_RET(serv_entryname, char, (len+1));
		strncpy(serv_entryname, dced_name, len1);
    }
    else {
		cp = strchr(dced_name, '@');
		dce_cf_dced_entry_from_host((cp) ? cp+1 : NULL, &cpp, &status);
		DCP_DHD_CHECK_STATUS_RET(status);
                len += strlen(cpp) + 1;
                if (strncmp(cpp, GLOBAL_DIR_ROOT, GLOBAL_DIR_ROOT_LEN) &&
                    strncmp(cpp, CELL_DIR_ROOT, CELL_DIR_ROOT_LEN)) {
		    len += CELL_DIR_ROOT_LEN;
		    DCP_DHD_MALLOC_RESET(serv_entryname, char, len, status);
		    if (!DCP_DHD_GOOD_STATUS(status)) {
			free(cpp);
			DCP_SET_MISC_ERROR(status);
		    }	
		    strcpy(serv_entryname, CELL_DIR_ROOT);
                }
                else {
		    DCP_DHD_MALLOC_RESET(serv_entryname, char, len, status);
		    if (!DCP_DHD_GOOD_STATUS(status)) {
			free(cpp);
			DCP_SET_MISC_ERROR(status);
		    }	
                }
		strcat(serv_entryname, cpp); 
		free(cpp);
    }
    strcat(serv_entryname, "/");
    strcat(serv_entryname, dhd_c_attr_schema_token);
    *schema_name = (dce_attr_component_name_t) serv_entryname;
	
    return TCL_OK;
}

#ifndef	HPDCE_DCED_LOCAL
/* Copy of sec_attr_util_sch_ent_free_ptrs(),
 * but calls rpc_ss_client_free().

/* 
 * SCHEMA ENTRY - field access macros: Stolen from sec_attr_tools.h.
 * S is a pointer to a sec_attr_schema_entry_t.
 * I, J are non-negative integers for array element selection.
 */
#define SA_ACL_MGR_SET_P(S)		(S)->acl_mgr_set
#define SA_ACL_MGR_NUM(S)		(SA_ACL_MGR_SET_P(S))->num_acl_mgrs
#define SA_ACL_MGR_INFO_P(S,I)		(SA_ACL_MGR_SET_P(S))->mgr_info[I]
#define SA_ACL_MGR_TYPE(S,I)		(SA_ACL_MGR_INFO_P(S,I))->acl_mgr_type
#define SA_ACL_MGR_PERMS_QUERY(S,I)	(SA_ACL_MGR_INFO_P(S,I))->query_permset
#define SA_ACL_MGR_PERMS_UPDATE(S,I)	(SA_ACL_MGR_INFO_P(S,I))->update_permset
#define SA_ACL_MGR_PERMS_TEST(S,I)	(SA_ACL_MGR_INFO_P(S,I))->test_permset
#define SA_ACL_MGR_PERMS_DELETE(S,I)	(SA_ACL_MGR_INFO_P(S,I))->delete_permset
#define SA_TRG_BND_INFO_P(S)		(S)->trig_binding

#define SA_TRG_BND_AUTH_INFO(S)		\
	(SA_BND_AUTH_INFO(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_INFO_TYPE(S)	\
	(SA_BND_AUTH_INFO_TYPE(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_DCE_INFO(S)	\
	(SA_BND_AUTH_DCE_INFO(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_SVR_PNAME_P(S)	\
	(SA_BND_AUTH_SVR_PNAME_P(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_PROT_LEVEL(S)	\
	(SA_BND_AUTH_PROT_LEVEL(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_AUTHN_SVC(S)	\
	(SA_BND_AUTH_AUTHN_SVC(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_AUTH_AUTHZ_SVC(S)	\
	(SA_BND_AUTH_AUTHZ_SVC(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_NUM(S)		\
	(SA_BND_NUM(SA_TRG_BND_INFO_P(S)))

#define SA_TRG_BND_ARRAY(S,I)		\
	(SA_BND_ARRAY((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_TYPE(S,I)		\
	(SA_BND_TYPE((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_STRING_P(S,I)		\
	(SA_BND_STRING_P((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_SVRNAME_P(S,I)		\
	(SA_BND_SVRNAME_P((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_SVRNAME_SYNTAX(S,I)      \
        (SA_BND_SVRNAME_SYNTAX((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_SVRNAME_NAME_P(S,I)      \
        (SA_BND_SVRNAME_NAME_P((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_TWRSET_P(S,I)		\
	(SA_BND_TWRSET_P((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_TWRSET_COUNT(S,I)	\
	(SA_BND_TWRSET_COUNT((SA_TRG_BND_INFO_P(S)),I))

#define SA_TRG_BND_TWR_P(S,I,J)		\
	(SA_BND_TWR_P((SA_TRG_BND_INFO_P(S)),I,J))

#define SA_TRG_BND_TWR_LEN(S,I,J)	\
	(SA_BND_TWR_LEN((SA_TRG_BND_INFO_P(S)),I,J))

#define SA_TRG_BND_TWR_OCTETS(S,I,J)	\
	(SA_BND_TWR_OCTETS((SA_TRG_BND_INFO_P(S)),I,J))

/* END SCHEMA ENTRY field access macros. */

void
dhd__schema_ent_free_ptrs(
	sec_attr_schema_entry_t		*sec_sch_entry_p
)
{
    long		i, j;

    if(!sec_sch_entry_p) {
	return;
    }

    if(sec_sch_entry_p->attr_name) {
	rpc_ss_client_free(sec_sch_entry_p->attr_name);
	sec_sch_entry_p->attr_name = NULL;
    }

    /* sec_attr_util_sch_free_acl_mgrs(sec_sch_entry_p); */
    if(sec_sch_entry_p->acl_mgr_set) {
	for(i = 0; i < SA_ACL_MGR_NUM(sec_sch_entry_p); i++) {
	    if(SA_ACL_MGR_INFO_P(sec_sch_entry_p, i)) {
		rpc_ss_client_free(SA_ACL_MGR_INFO_P(sec_sch_entry_p, i));
	    }
	}
	rpc_ss_client_free(sec_sch_entry_p->acl_mgr_set);
	sec_sch_entry_p->acl_mgr_set = NULL;
    }

    /* sec_attr_util_sch_free_binding(sec_sch_entry_p); */
    if(sec_sch_entry_p->trig_binding) {
	if(SA_TRG_BND_AUTH_INFO_TYPE(sec_sch_entry_p) == 
	    sec_attr_bind_auth_dce) {
	    if(SA_TRG_BND_AUTH_SVR_PNAME_P(sec_sch_entry_p)) {
		rpc_ss_client_free(SA_TRG_BND_AUTH_SVR_PNAME_P(sec_sch_entry_p));
	    }
	}
	for(i = 0; i < SA_TRG_BND_NUM(sec_sch_entry_p); i++) {
	    switch(SA_TRG_BND_TYPE(sec_sch_entry_p, i)) {
	    case sec_attr_bind_type_string:
		if(SA_TRG_BND_STRING_P(sec_sch_entry_p, i)) {
		    rpc_ss_client_free(SA_TRG_BND_STRING_P(sec_sch_entry_p, i));
		}
		break;
	    case sec_attr_bind_type_twrs:
		if(SA_TRG_BND_TWRSET_P(sec_sch_entry_p, i)) {
		    for(j = 0; 
			j < SA_TRG_BND_TWRSET_COUNT(sec_sch_entry_p, i);
			j++) {
			if(SA_TRG_BND_TWR_P(sec_sch_entry_p, i, j)) {
			    rpc_ss_client_free(SA_TRG_BND_TWR_P(sec_sch_entry_p, i, j));
			}
		    }
		    rpc_ss_client_free(SA_TRG_BND_TWRSET_P(sec_sch_entry_p, i));
		}
		break;
	    case sec_attr_bind_type_svrname:
		if(SA_TRG_BND_SVRNAME_P(sec_sch_entry_p, i)) {
		    if(SA_TRG_BND_SVRNAME_NAME_P(sec_sch_entry_p, i)) {
		        rpc_ss_client_free(SA_TRG_BND_SVRNAME_NAME_P(sec_sch_entry_p, i));
		    }
		    rpc_ss_client_free(SA_TRG_BND_SVRNAME_P(sec_sch_entry_p, i));
		}
		break;
	    default:
		break;
	    } /* end switch */
	}
	rpc_ss_client_free(sec_sch_entry_p->trig_binding);
	sec_sch_entry_p->trig_binding = NULL;
    }

    if(sec_sch_entry_p->scope) {
	rpc_ss_client_free(sec_sch_entry_p->scope);
	sec_sch_entry_p->scope = NULL;
    }

    if(sec_sch_entry_p->comment) {
	rpc_ss_client_free(sec_sch_entry_p->comment);
	sec_sch_entry_p->comment = NULL;
    }
}
#endif	/* HPDCE_DCED_LOCAL */
