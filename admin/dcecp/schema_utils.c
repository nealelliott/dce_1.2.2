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
 * $Log: schema_utils.c,v $
 * Revision 1.1.6.3  1996/11/13  17:54:50  arvind
 * 	Merge changes.
 * 	[1996/10/31  15:58 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk8/1]
 *
 * 	CHFts20224: Check schema acl code for malloc errors.
 * 	[1996/10/09  18:42 UTC  cuti  /main/DCE_1.2.2/3]
 *
 * 	Merge allow aclmgr change permset
 * 	[1996/10/09  18:24 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/1]
 *
 * 	Make aclmgr modify works
 * 	[1996/07/08  16:29 UTC  truitt  /main/DCE_1.2.2/2]
 *
 * 	Merge changes.
 * 	[1996/06/12  15:07 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/18  18:10 UTC  truitt  /main/HPDCE02/9]
 *
 * 	Merge.
 * 	[1996/03/18  18:08 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 * 	[1996/02/15  18:10 UTC  truitt  /main/HPDCE02/8]
 *
 * 	Merge brief changes.
 * 	[1996/02/15  18:07 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 *
 * Revision 1.1.6.2  1996/10/15  20:43:02  arvind
 * 	Merge allow aclmgr change permset
 * 
 * 	Make aclmgr modify works
 * 	[1996/10/09  18:24 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/1]
 * 
 * 	Merge changes.
 * 	[1996/06/12  15:07 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 * 
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/18  18:10 UTC  truitt  /main/HPDCE02/9]
 * 
 * 	Merge.
 * 	[1996/03/18  18:08 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 * 
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 * 	[1996/02/15  18:10 UTC  truitt  /main/HPDCE02/8]
 * 
 * 	Merge brief changes.
 * 	[1996/02/15  18:07 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 * 
 * Revision 1.1.6.1  1996/08/09  11:51:28  arvind
 * 	Merge changes.
 * 	[1996/06/12  15:07 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 * 
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/18  18:10 UTC  truitt  /main/HPDCE02/9]
 * 
 * 	Merge.
 * 	[1996/03/18  18:08 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 * 
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 * 	[1996/02/15  18:10 UTC  truitt  /main/HPDCE02/8]
 * 
 * 	Merge brief changes.
 * 	[1996/02/15  18:07 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 * 
 * 	CHFts17451: Remove include of value.h so duplicate macro
 * 	warning does not appear during HP build.  The include is
 * 	required on other platforms.
 * 
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/12  14:38 UTC  truitt  /main/DCE_1.2.2/1]
 * 
 * Revision 1.1.4.2  1996/02/18  19:15:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:14  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  21:23:21  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/7  1995/06/27  17:05 UTC  truitt
 * 	Merge minor change.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts36/1  1995/06/27  17:04 UTC  truitt
 * 	Use correct variable name in comparison.
 * 
 * 	HP revision /main/HPDCE02/6  1995/05/23  19:10 UTC  truitt
 * 	Merge field header work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts30/1  1995/05/23  19:08 UTC  truitt
 * 	CHFts14679: Move string header info from code to sams file.
 * 
 * 	HP revision /main/HPDCE02/5  1995/05/08  19:23 UTC  truitt
 * 	Merge aclmgr set changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts28/1  1995/05/08  19:21 UTC  truitt
 * 	CHFts14643: Consistency of acl manager set information on input
 * 	and output.  Make multi-valued default to 'no' when creating a
 * 	schema entry.
 * 
 * 	HP revision /main/HPDCE02/4  1995/05/01  19:19 UTC  truitt
 * 	Merge era changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  19:17 UTC  truitt
 * 	CHFts14653: Allow encoding type any to be used.
 * 
 * 	HP revision /main/HPDCE02/3  1995/04/28  20:34 UTC  dnguyen
 * 	Merge schema code fix to use sec_rgy_attr_sch* API
 * 	to get acl manager set info
 * 
 * 	HP revision /main/HPDCE02/dnguyen_mothra/1  1995/04/13  13:52 UTC  dnguyen
 * 	Fix schema code to use sec_rgy_attr_sch* API
 * 	to get acl manager set info.
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/05  19:47 UTC  truitt
 * 	Merge message work.
 * 
 * 	HP revision /main/HPDCE02/truitt_mothra_chfts24/1  1995/04/05  19:46 UTC  truitt
 * 	CHFts14676: The catalog allows slightly improper schema name.
 * 	CHFts14652: Improve 'schema entry not found' error message.
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/10  19:12 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/truitt_dcecp_chfts11/1  1995/02/10  19:10 UTC  truitt
 * 	Some variables were not being initialized or free'd
 * 	correctly.
 * 	[1995/12/08  17:57:25  root]
 * 
 * Revision 1.1.2.13  1994/09/29  17:35:41  ajayb
 * 	Fix bug in abbrev expanding code for schema commands.
 * 	Currently the code would flag an input of 'foo' as
 * 	ambiguous if the list of valid inputs included 'foo1',
 * 	'foo2', and 'foo'. The fix is to ignore other hits if
 * 	we find an *exact* match.
 * 	[1994/09/29  17:19:59  ajayb]
 * 
 * Revision 1.1.2.12  1994/09/28  20:11:51  ajayb
 * 	Merged with changes from 1.1.2.11
 * 	[1994/09/28  20:11:11  ajayb]
 * 
 * 	Move initialization of flag to correct place
 * 	when reading in intercell action values.
 * 	[1994/09/28  20:01:16  ajayb]
 * 
 * 	Accept unique prefixes as valid input
 * 	for schema attr values.
 * 	[1994/09/27  19:58:26  ajayb]
 * 
 * Revision 1.1.2.11  1994/09/28  18:36:01  truitt
 * 	OT12379: Added extra checks for null values in the
 * 	schema modify parsing functions.
 * 	[1994/09/28  18:10:02  truitt]
 * 
 * Revision 1.1.2.10  1994/09/26  19:40:00  truitt
 * 	OT12339: Removed some 'free_ptrs' calls which caused bus errors.
 * 	[1994/09/26  19:38:38  truitt]
 * 
 * Revision 1.1.2.9  1994/09/26  18:41:15  salamone
 * 	CR11409 - Determine security registry group name on the fly.
 * 		  Improved efficiency of error checking in schema_ops.c.
 * 		  Fix 486 compile warnings in schema_utils.c.
 * 	[1994/09/26  18:26:52  salamone]
 * 
 * Revision 1.1.2.8  1994/09/23  18:45:20  ajayb
 * 	Merged with changes from 1.1.2.7
 * 	[1994/09/23  18:45:10  ajayb]
 * 
 * 	Fixed several memory leaks.
 * 	[1994/09/22  20:39:50  ajayb]
 * 
 * Revision 1.1.2.7  1994/09/23  16:27:05  truitt
 * 	OT11182: Oops.  Forgot to change the variable which indicates
 * 	the number of permission characters.
 * 	[1994/09/23  15:29:06  truitt]
 * 
 * 	OT11182: Add the 'x' and 'w' characters to the list of
 * 	hard-coded perm characters so the srvrconf aclmgr will
 * 	work correctly.
 * 	[1994/09/23  14:31:54  truitt]
 * 
 * Revision 1.1.2.6  1994/09/21  17:53:56  salamone
 * 	Merged with changes from 1.1.2.5
 * 	[1994/09/21  17:53:49  salamone]
 * 
 * 	CR11182 - Change ACL mgr names to map to Security ACL mgr names
 * 	[1994/09/21  17:52:11  salamone]
 * 
 * Revision 1.1.2.5  1994/09/21  17:36:12  truitt
 * 	OT12191: Change schema name parsing code to allow
 * 	a '/' character in the schema object name.
 * 	[1994/09/21  16:57:25  truitt]
 * 
 * Revision 1.1.2.4  1994/09/20  20:23:55  truitt
 * 	OT12107: Changed some of the malloc's in the parse schema
 * 	functions.
 * 	[1994/09/20  20:07:22  truitt]
 * 
 * Revision 1.1.2.3  1994/09/07  13:24:28  kevins
 * 	OT 12057 removed duplicate attribute declaration 'uuid'
 * 	[1994/09/07  11:26:32  kevins]
 * 
 * Revision 1.1.2.2  1994/09/02  18:39:34  ajayb
 * 	Handle tower bindings in trigbind value in
 * 	encode_modify_schema_attr(), and
 * 	parse_modify_schema_switch_list().
 * 	[1994/09/02  16:04:42  ajayb]
 * 
 * Revision 1.1.2.1  1994/09/01  19:04:59  truitt
 * 	Put schema commands into their own files.
 * 	[1994/09/01  18:11:04  truitt]
 * 
 * $EndLog$
 */

/* INCLUDES */
#include <stdio.h>
#include <locale.h>
#include <errno.h>
/*
 * Don't inherit stubbase.h's use of the exception based pthreads API.
 */
#ifndef NCK
#  define NCK
#endif
#include <dce/rpc.h>
#include <dce/stubbase.h>

#ifndef SNI_SVR4_POSIX
#include <strings.h>
#endif

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dce/macros.h>
#include <dce/dce_error.h>
#include <sys/types.h>
#include <unistd.h>
#include <dce/dce.h>
#include <dce/binding.h>
#include <dce/pgo.h>
#include <dce/policy.h>
#include <dce/rgynbase.h>
#include <dce/secidmap.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dce_svc.h>
#include <dce/misc.h>
#include <dcecp.h>
#include <dce/tcl.h>
#include <dce/utc.h>
#include <dce/passwd.h>
#include <rgy.h>
#include <wchar.h>
#include <values.h>

#include <util_var.h>
#include <util_general.h>
#include <util_binding.h>
#include <dce/sec_attr_base.h>
#include <dce/sec_attr_util.h>
#include <dce/sec_attr_tools.h>
#include <dce/sec_rgy_attr.h>
#include <dce/sec_rgy_attr_sch.h>
#include <dce/aclbase.h>
#include <dce/acct.h>
#include <ctype.h>
#include <dhd.h>

#include <dce/dce_cf_const.h>
#include <dce/rs_bind.h>

#define MAX_ATTR_SHOW   10
#define MAX_ACL_MGR      8
#define MAX_PERM_TYPES  17
#define MAX_INTERCELL    3
#define MAX_AUTHN       ((sizeof(authn_val)) / (sizeof(unsigned32)))
#define MAX_TRIG         3

/* PROTOTYPES */

void set_attr_schema_binding(dce_attr_component_name_t, error_status_t *);

/* GLOBALS */

extern dce_attr_sch_handle_t  attr_schema_binding_handle;

/* INTERNAL VARIABLES */

typedef struct dcecp_schema_attr_index_table_t {
    char *attribute_name;
    int  index;
} dcecp_schema_attr_index_table_t;

dcecp_schema_attr_index_table_t schema_attr[] = {
    {schema_attr_encoding, schema_encoding},
    {schema_attr_aclmgr, schema_aclmgr},
    {schema_attr_unique, schema_unique},
    {schema_attr_multivalued, schema_multivalued},
    {schema_attr_reserved, schema_reserved},
    {schema_attr_applydefs, schema_applydefs},
    {schema_attr_intercell, schema_intercell},
    {schema_attr_trigtype, schema_trigtype},
    {schema_attr_trigbind, schema_trigbind},
    {schema_attr_scope, schema_scope},
    {schema_attr_annotation, schema_annotation}
};    

static char *prot_lev_name[] = {"default",
                                "none",
                                "connect",
                                "call",
                                "pkt",
                                "pktinteg",
                                "pktprivacy"};

static char *authn_name[] = {"none",
                             "secret",
                             "public",
                             "default"};

static unsigned32 authn_val[] = {rpc_c_authn_none,
                                 rpc_c_authn_dce_secret,
                                 rpc_c_authn_dce_public,
                                 rpc_c_authn_default};

static char *authz_name[] = {"none",
                             "name",
                             "dce"};

static char *intercell_act_name[] = {"accept",
                                     "reject",
                                     "evaluate"};

static char *trig_type_name[] = {"none",
                                 "query",
                                 "update"};

static unsigned32 trig_type_val[] = {sec_attr_trig_type_none,
                                     sec_attr_trig_type_query,
                                     sec_attr_trig_type_update};

unsigned32 num_acl_perm_strings = 0;
sec_acl_printstring_t *acl_perm_strings = NULL;
sec_acl_printstring_t local_acl_perm_strings[] = {
      {"r", "read",            sec_acl_perm_read},
      {"c", "control",         sec_acl_perm_owner},
      {"i", "insert",          sec_acl_perm_insert},
      {"d", "delete",          sec_acl_perm_delete},
      {"t", "test",            sec_acl_perm_test},
      {"w", "write",           sec_acl_perm_write},
      {"x", "execute",         sec_acl_perm_execute},
      {"D", "Delete object",   sec_acl_perm_unused_00000080},  /* sec_acl_perm_Delete_object */
      {"n", "name",            sec_acl_perm_unused_00000100},  /* sec_acl_perm_name */
      {"f", "fullname",        sec_acl_perm_unused_00000200},  /* sec_acl_perm_fullname */
      {"m", "management info", sec_acl_perm_unused_00000400},  /* sec_acl_perm_mgmt_info */
      {"a", "auth info",       sec_acl_perm_unused_00000800},  /* sec_acl_perm_auth_info */
      {"u", "user info",       sec_acl_perm_unused_00001000},  /* sec_acl_perm_user_info */
      {"g", "groups",          sec_acl_perm_unused_00002000},  /* sec_acl_perm_groups */
      {"M", "Member list",     sec_acl_perm_unused_00004000},  /* sec_acl_perm_Member_list */
      {"A", "Administration",  sec_acl_perm_unused_00008000},  /* sec_acl_perm_Admin */
      {"I", "Initialize",      sec_acl_perm_unused_00010000}}; /* sec_acl_perm_Initialize */

/*
 * The four strings which map to the four permission character sets.
 */
static char *local_permset_name_table[] = {"query",
                                           "update",
                                           "test",
                                           "delete"};

/*
 * These ACL manager names map to the ACL manager UUIDs below.
 */
char **acl_mgr_name = NULL;
static char *local_acl_mgr_name[] = {"policy",
                                     "principal",
                                     "group",
                                     "organization",
                                     "secdirectory",
                                     "replist",
                                     "xattrschema",
                                     "srvrconf"};

unsigned32 num_acl_mgr_uuid = 0;
uuid_t *acl_mgr_uuid = NULL;
uuid_t local_acl_mgr_uuid[MAX_ACL_MGR] = {
      {0x06ab8f10,
       0x0191,
       0x11ca,
       0xa9,
       0xe8,
       {0x08, 0x00, 0x1e, 0x03, 0x9d, 0x7d}
      },
      {0x06ab9320,
       0x0191,
       0x11ca,
       0xa9,
       0xe8,
       {0x08, 0x00, 0x1e, 0x03, 0x9d, 0x7d}
      },
      {0x06ab9640,
       0x0191,
       0x11ca,
       0xa9,
       0xe8,
       {0x08, 0x00, 0x1e, 0x03, 0x9d, 0x7d}
      },
      {0x06ab9960,
       0x0191,
       0x11ca,
       0xa9,
       0xe8,
       {0x08, 0x00, 0x1e, 0x03, 0x9d, 0x7d}
      },
      {0x06ab9c80,
       0x0191,
       0x11ca,
       0xa9,
       0xe8,
       {0x08, 0x00, 0x1e, 0x03, 0x9d, 0x7d}
      },
      {0x2ac24970,
       0x60c3,
       0x11cb,
       0xb2,
       0x61,
       {0x08, 0x00, 0x1e, 0x03, 0x9d, 0x7d}
      },
      {0x755cd9ce,
       0xded3,
       0x11cc,
       0x8d,
       0x0a,
       {0x08, 0x00, 0x09, 0x35, 0x35, 0x59}
      },
      {0x004d3731,
       0xc9b5,
       0x1d6c,
       0x9a,
       0xdb,
       {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54}
      }};


/*
 * FUNCTION: schema_attribute
 *
 * OVERVIEW: This function looks up an attribute using a 
 *    Tcl Interpreter where the known attributes are stored. 
 *    All known attributes are assigned an integer value
 *    this value is returned. 
 *
 * INPUTS:
 *  char *attribute ascii representation of the attribute. 
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  The attribute integer value. 
 */
int schema_attribute(char *attribute)
{
   extern Tcl_Interp  *attr_interp;
   char               command[1024];
   char               *command_format = "lindex [set [ attr_name %s ]] 0";
   int                result = TCL_OK;

   sprintf(command, command_format, attribute);
   result = Tcl_Eval(attr_interp, command);
   return atoi((const char *)attr_interp->result);
}


/*
 * FUNCTION: init_schema_attribute_table
 *
 * OVERVIEW: This function initializes the attribute table for the 
 *    xattrschema object. 
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp
 *
 * RETURNS:
 *  void
 */
void init_schema_attribute_table(Tcl_Interp *interp)
{
   int     i, attribute_index;
   char    command[1024];
   extern  Tcl_Interp *attr_interp;

   attribute_index = sizeof schema_attr / sizeof schema_attr[0];
   for (i = 0; i < attribute_index; i++) {
      sprintf(command, "set %s %d", schema_attr[i].attribute_name, schema_attr[i].index);
      Tcl_Eval(attr_interp,command);
   }
}


/*
 * FUNCTION: split_schema_name
 *
 * OVERVIEW:
 *  This function will take as input a fully qualified schema name
 *  and splits that name into the two component parts, the schema
 *  name and the schema attribute name.
 *
 * INPUTS:
 *  char *name - The fully qualified schema name.
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *  char *schema_name - The schema name.
 *  char *schema_attr_name - The schema attribute name.
 *  error_status_t *result - Either ok for a successfull split or
 *                           an error indicating a bad schema name.
 *
 * RETURNS:
 *
 */
void split_schema_name(char *name,
                       char *schema_name,
                       char *schema_attr_name,
                       error_status_t *result)
{
   int  i, j;
   int  sch_total_len;
   char temp_name[12];

   *result = error_status_ok;
   sch_total_len = strlen(name);
   i = 0;
   while (i < sch_total_len) {
      if (name[i] == '/') {
         if ((sch_total_len - i) < 11)
            break;
         for (j = 0; j < 11; j++)
            temp_name[j] = name[i+j+1];
         temp_name[j] = '\0';
         if (strcmp(temp_name, XATTRSCHEMA) == 0) {
            i = i + 12;
            break;
         }
      }
      i++;
   }
   if ((i <= 0) || (i == (sch_total_len - 1))) {
      /*
       * This is not a fully qualified schema name.
       */
      *result = dcp_s_rgy_bad_schema_name;
      return;
   }

   /*
    * We have two separate strings to process.  Assign the
    * first string to the schema name and the second string
    * to the schema attribute name.
    */
   for (j = 0; j < (sch_total_len - i); j++)
      schema_attr_name[j] = name[i+j+1];
   schema_attr_name[j] = '\0';

   for (j = 0; j < i; j++)
      schema_name[j] = name[j];
   schema_name[j] = '\0';
}


/*
 * FUNCTION: free_acl_mgr_tables
 *
 * OVERVIEW:
 *  This function will free the dynamically-allocated acl manager
 *  uuids and names table that are allocated by the function
 *  build_acl_mgr_tables
 *
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *
 * RETURNS:
 *
 */
void free_acl_mgr_tables()
{
   int             i;
   error_status_t  tmp_status;

   if ((acl_mgr_uuid != NULL) && (acl_mgr_uuid != local_acl_mgr_uuid)) {
      free((uuid_t *)acl_mgr_uuid);
      for (i = 0; acl_mgr_name[i] != NULL; i++) {
         rpc_string_free((unsigned_char_t **)(acl_mgr_name+i), &tmp_status);
      }
      num_acl_mgr_uuid = 0;
   }
}


/*
 * FUNCTION: free_acl_perm_strings_table
 *
 * OVERVIEW:
 *  This function will free the acl manager permision strings table
 *  that is allocated by the function build_acl_perm_strings
 *
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *
 * RETURNS:
 *
 */
void free_acl_perm_strings_table()
{
   if ((acl_perm_strings != NULL) && (acl_perm_strings != local_acl_perm_strings)) {
      free((sec_acl_printstring_t *)acl_perm_strings);
      num_acl_perm_strings = 0;
   }
}


/*
 * FUNCTION: build_acl_mgr_tables
 *
 * OVERVIEW:
 *  This function will build table of uuids and names of all the acl
 *  manager that protects a schema object. If the API for getting the acl
 *  manager types of a schema object is not implemented, this function
 *  will default to the local tables local_acl_mgr_uuid and local_acl_mgr_name.
 *
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *  error_status_t *status - Either ok for a successfull build or
 *                           an error indicating a failure.
 *  num_acl_mgr_uuid - number of acl manager types in the table, declared
 *                     globally on top.
 *
 * RETURNS:
 *
 */
void build_acl_mgr_tables(error_status_t *status)
{
   unsigned32      num_acl_mgr_types = 0;
   error_status_t  tmp_status;
   int             i, j;

   if ((acl_mgr_uuid = (uuid_t *)malloc(sizeof(uuid_t) * MAX_ACL_MGR)) == NULL) {
      SET_STATUS(status, dcp_s_no_memory);
      num_acl_mgr_uuid = 0;
      return;
   }
   sec_rgy_attr_sch_get_acl_mgrs(rgy_context,
                                 (sec_attr_component_name_t)NULL,
                                 (unsigned32)MAX_ACL_MGR,
                                 &num_acl_mgr_uuid,
                                 &num_acl_mgr_types,
                                 acl_mgr_uuid,
                                 status);
   if (STATUS_EQUAL(status, sec_attr_not_implemented)) {
      free((uuid_t *)acl_mgr_uuid);
      acl_mgr_uuid = local_acl_mgr_uuid;
      num_acl_mgr_uuid = MAX_ACL_MGR;
   }
   else if (BAD_STATUS(status)) {
      free((uuid_t *)acl_mgr_uuid);
      num_acl_mgr_uuid = 0;
      return;
   }
   else
   {
      /*
       * realloc acl_mgr_uuid if number of types returned is more than
       * size of acl_mgr_uuid and recall sec_rgy_attr_sch_get_acl_mgrs
       */
      if (num_acl_mgr_types > num_acl_mgr_uuid) {
         if ((acl_mgr_uuid = (uuid_t *)realloc((void *)acl_mgr_uuid,
                                               sizeof(uuid_t) *
                                               num_acl_mgr_types)) == NULL) {
            SET_STATUS(status, dcp_s_no_memory);
            num_acl_mgr_uuid = 0;
            return;
         }
         sec_rgy_attr_sch_get_acl_mgrs(rgy_context,
                                       (sec_attr_component_name_t)NULL,
                                       num_acl_mgr_types,
                                       &num_acl_mgr_uuid,
                                       &num_acl_mgr_types,
                                       acl_mgr_uuid,
                                       status);
         if (BAD_STATUS(status)) {
            free((uuid_t *)acl_mgr_uuid);
            num_acl_mgr_uuid = 0;
            return;
         }
      }
   }
   if (acl_mgr_uuid == local_acl_mgr_uuid) {
      acl_mgr_name = local_acl_mgr_name;
      SET_STATUS(status, error_status_ok);
   }
   else {
      if ((acl_mgr_name = (char **)malloc(sizeof(char *) *
                                          (num_acl_mgr_uuid + 1))) == NULL) {
         free((uuid_t *)acl_mgr_uuid);
         num_acl_mgr_uuid = 0;
         SET_STATUS(status, dcp_s_no_memory);
         return;
      }
      for (i = 0; i < num_acl_mgr_uuid; i++) {
         uuid_to_string(&(acl_mgr_uuid[i]), (unsigned_char_t **)acl_mgr_name+i, status);
         if (BAD_STATUS(status)) {
            for (j = 0; j < i - 1; j++) {
               rpc_string_free((unsigned_char_t **)acl_mgr_name+j, &tmp_status);
            }
            free((char **)acl_mgr_name);
            free((uuid_t *)acl_mgr_uuid);
            num_acl_mgr_uuid = 0;
            return;
         }
      }

      /*
       * Null terminated the array; needed for free_acl_mgr_tables().
       */ 
      acl_mgr_name[i] = (char *)NULL;
   }
}


/*
 * FUNCTION: build_acl_perm_strings_table
 *
 * OVERVIEW:
 *  This function will build table of permission printstrings for an acl
 *  manager type. If the API for getting the permission printstrings 
 *  is not implemented, this function will default to the local table
 *  local_acl_perm_strings.
 *
 * INPUTS:
 *  uuid_t *acl_mgr_type - uuid of the acl manager type whose printstrings
 *                         are to be built.
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *  error_status_t *status - Either ok for a successfull build or
 *                           an error indicating a failure.
 *  num_acl_perm_strings - the number of perm. strings in the table, declared
 *                         globally on top.
 *
 * RETURNS:
 *
 */
void build_acl_perm_strings_table(uuid_t *acl_mgr_type,
                                  error_status_t *status)
{
   uuid_t                 acl_mgr_type_chain;
   sec_acl_printstring_t  acl_mgr_info;
   boolean32              tokenize;
   unsigned32             total_num_printstrings = 0;
   error_status_t	  tmp_status;

   free_acl_perm_strings_table();
   if ((acl_perm_strings = (sec_acl_printstring_t *)
                                 malloc(sizeof(sec_acl_printstring_t) *
                                 MAX_PERM_TYPES)) == NULL) {
      SET_STATUS(status, dcp_s_no_memory);
      num_acl_perm_strings = 0;
      return;
   }
   sec_rgy_attr_sch_aclmgr_strings(rgy_context,
                                   (sec_attr_component_name_t)NULL,
                                   acl_mgr_type,
                                   (unsigned32)MAX_PERM_TYPES,
                                   &acl_mgr_type_chain,
                                   &acl_mgr_info,
                                   &tokenize,
                                   &total_num_printstrings,
                                   &num_acl_perm_strings,
                                   acl_perm_strings,
                                   status);
   if (STATUS_EQUAL(status, sec_attr_not_implemented)) {
      free((sec_acl_printstring_t *)acl_perm_strings);
      acl_perm_strings = local_acl_perm_strings;
      num_acl_perm_strings = (unsigned32) MAX_PERM_TYPES;
      SET_STATUS(status, error_status_ok);
   }
   else if (BAD_STATUS(status)) {
      free((sec_acl_printstring_t *)acl_perm_strings);
      num_acl_perm_strings = 0;
   }
   else {
      /*
       * realloc acl_perm_strings if total number of printstrings is more than
       * size of acl_perm_strings and re-call sec_rgy_attr_sch_aclmgr_strings()
       */
      if (total_num_printstrings > num_acl_perm_strings) {
         free((uuid_t *)acl_perm_strings);
         if ((acl_perm_strings = (sec_acl_printstring_t *)
                                  malloc(sizeof(sec_acl_printstring_t) *
                                  total_num_printstrings)) == NULL) {
            SET_STATUS(status, dcp_s_no_memory);
            num_acl_perm_strings = 0;
            return;
         }
         sec_rgy_attr_sch_aclmgr_strings(rgy_context,
                                         (sec_attr_component_name_t)NULL,
                                         acl_mgr_type,
                                         total_num_printstrings,
                                         &acl_mgr_type_chain,
                                         &acl_mgr_info,
                                         &tokenize,
                                         &total_num_printstrings,
                                         &num_acl_perm_strings,
                                         acl_perm_strings,
                                         status);
         if (BAD_STATUS(status)) {
            free((sec_acl_printstring_t *)acl_perm_strings);
            num_acl_perm_strings = 0;
         }
      }
   }
}


/*
 * FUNCTION: show_schema_info
 *
 * OVERVIEW:
 *  This function will take as input the name of one schema attribute and
 *  display all the information associated with that attribute.
 *
 * INPUTS:
 *  char *name - The name of the schema attribute to use.  This is expected
 *               to be the fully qualified schema attribute name, meaning
 *               this function will have to parse the name into it's two
 *               components, the schema name and the schema attribute.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                       will hold the error message (if any) generated from the
 *                       command. This is also the location of any convienience
 *                       variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 */
int show_schema_info(Tcl_Interp *interp, char *name)
{
   int                      result = TCL_OK;
   error_status_t           status;
   error_status_t           tmp_status;
   char                     *id_svc;
   char                     *no_svc;
   char                     *yes_svc;
   char                     *unknown_svc;
   char                     *encoding_svc;
   char                     *acl_set_svc;
   char                     *unique_svc;
   char                     *multi_valued_svc;
   char                     *reserved_svc;
   char                     *defaults_svc;
   char                     *action_svc;
   char                     *trig_types_svc;
   char                     *trig_binding_svc;
   char                     *scope_svc;
   char                     *comment_svc;
   char                     *schema_attr_name = NULL;
   char                     *schema_name = NULL;
   char                     *full_schema_name = NULL;
   char                     *full_sec_group_name = NULL;
   unsigned_char_p_t        rs_ns_sec_group_name = NULL;
   sec_attr_schema_entry_t  schema_entry;
   sec_attr_schema_entry_t  *sch_p;
   int                      i, j, n;
   char                     *tcl_str;
   char                     tmp_str[16];
   unsigned_char_t          *str1, *str2;
   boolean32                perm_found;

   /*
    * After obtaining the registry context of the specified
    * argument, obtain the security group name for the cell.
    */
   if (dcp_schema_read_group_name(interp, &rs_ns_sec_group_name) != TCL_OK) {
      return TCL_ERROR;
   }
   full_sec_group_name = (char *)malloc(sizeof(char) *
      (strlen((char *)rs_ns_sec_group_name) + sizeof(XATTRSCHEMA) + 2));
   if (full_sec_group_name == NULL) {
      DCP_SET_MISC_ERROR(dcp_s_no_memory);
   }
   sprintf(full_sec_group_name,
           "%s/%s",
           (char *)rs_ns_sec_group_name,
           (char *)XATTRSCHEMA);
   rpc_string_free(&rs_ns_sec_group_name, &status);

   /*
    * Parse the fully qualified name into it's two components, the
    * schema name and the schema attribute name.  If after parsing
    * the string length of either or both of these is zero, then
    * the user blew the input.  This is an error.
    */
   schema_name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
   if (schema_name == NULL) {
      free((char *)full_sec_group_name);
      DCP_SET_MISC_ERROR(dcp_s_no_memory);
   }

   schema_attr_name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
   if (schema_attr_name == NULL) {
      free((char *)full_sec_group_name);
      free((char *)schema_name);
      DCP_SET_MISC_ERROR(dcp_s_no_memory);
   }

   split_schema_name(name, schema_name, schema_attr_name, &status);
   if (BAD_STATUS(&status)) {
      free((char *)full_sec_group_name);
      free((char *)schema_name);
      free((char *)schema_attr_name);
      DCP_SET_MISC_ERROR(status);
   }
   full_schema_name = expand_cell_name(schema_name, &status);
   if (BAD_STATUS(&status)) {
      free((char *)full_sec_group_name);
      free((char *)schema_name);
      free((char *)schema_attr_name);
      DCP_SET_MISC_ERROR(status);
   }

   /*
    * Check if the user wants the default registry schema or some
    * other schema.  If the default registry schema is selected,
    * we must use the registry version of the lookup call.  If not,
    * we use the generic version of the lookup call.
    */
   if (strcmp(full_schema_name, full_sec_group_name) == 0) {
      free((char *)full_schema_name);
      free((char *)full_sec_group_name);
      /*
       * Do the schema lookup by name and retrieve the
       * values for all the schema attribute fields.
       */
      sec_rgy_attr_sch_lookup_by_name(rgy_context,
                                      (sec_attr_component_name_t)schema_name,
                                      (idl_char *)schema_attr_name,
                                      &schema_entry,
                                      &status);
      if (BAD_STATUS(&status)) {
         free((char *)schema_name);
         free((char *)schema_attr_name);
         if (status == sec_attr_sch_entry_not_found) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_sch_entry_not_found, (char *)name);
            return TCL_ERROR;
         }
         else {
            DCP_SET_MISC_ERROR(status);
         }
      }
   }
   else {
      free((char *)full_schema_name);
      free((char *)full_sec_group_name);
      set_attr_schema_binding((dce_attr_component_name_t)schema_name, &status);
      DCP_CHECK_SEC_ERROR(status);
      dce_attr_sch_lookup_by_name(attr_schema_binding_handle,
                                  (idl_char *)schema_attr_name,
                                  (dce_attr_schema_entry_t *)&schema_entry,
                                  &status);
      if (BAD_STATUS(&status)) {
         free((char *)schema_name);
         free((char *)schema_attr_name);
         if (status == db_s_key_not_found) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_sch_entry_not_found, (char *)name);
            return TCL_ERROR;
         }
         else {
            DCP_SET_MISC_ERROR(status);
         }
      }
   }

   /*
    * Define a shorthand pointer to the schema entry information.
    */
   sch_p = &schema_entry;

   /*
    * Build the required acl manager table.
    */
   build_acl_mgr_tables(&status);
   if (BAD_STATUS(&status)) {
      free((char *)schema_name);
      free((char *)schema_attr_name);
      DCP_SET_MISC_ERROR(status);
   }

   /*
    * Set up the attribute name fields.  These are defined
    * in the dcecp message file and match, one for one,
    * the fields associated with a schema attribute.
    */
   id_svc = (char *)dce_sprintf(dcp_t_rgy_uuid);
   no_svc = (char *)dce_sprintf(dcp_t_rgy_no);
   yes_svc = (char *)dce_sprintf(dcp_t_rgy_yes);
   unknown_svc = (char *)dce_sprintf(dcp_t_rgy_unknown);
   encoding_svc = (char *)dce_sprintf(dcp_t_rgy_era_encoding);
   acl_set_svc = (char *)dce_sprintf(dcp_t_rgy_era_acl_set);
   unique_svc = (char *)dce_sprintf(dcp_t_rgy_era_unique);
   multi_valued_svc = (char *)dce_sprintf(dcp_t_rgy_era_multi_valued);
   reserved_svc = (char *)dce_sprintf(dcp_t_rgy_era_reserved);
   defaults_svc = (char *)dce_sprintf(dcp_t_rgy_era_defaults);
   action_svc = (char *)dce_sprintf(dcp_t_rgy_era_action);
   trig_types_svc = (char *)dce_sprintf(dcp_t_rgy_era_trig_types);
   trig_binding_svc = (char *)dce_sprintf(dcp_t_rgy_era_trig_binding);
   scope_svc = (char *)dce_sprintf(dcp_t_rgy_era_scope);
   comment_svc = (char *)dce_sprintf(dcp_t_rgy_era_comment);

   /*
    * Check that the field headers were successfully allocated.
    */
   if (!id_svc || !trig_types_svc || !yes_svc || !encoding_svc || !acl_set_svc ||
       !unique_svc || !action_svc || !reserved_svc || !defaults_svc || !unknown_svc ||
       !multi_valued_svc || !no_svc || !trig_binding_svc || !scope_svc || !comment_svc) {
      sec_attr_util_sch_ent_free_ptrs(&schema_entry);
      free((char *)schema_name);
      free((char *)schema_attr_name);
      DCP_SET_MISC_ERROR(dcp_s_no_memory);
   }

   /*
    * Generate the output buffer.  Assign the values from
    * the schema structure returned by the lookup.  Store
    * the formatted information in the interp variable.  The
    * attributes are displayed in lexical order.
    */

   /*
    * Field acl_mgr_set.
    */
   if (SA_ACL_MGR_NUM(sch_p) > 0) {
      Tcl_AppendResult(interp, "{", acl_set_svc, NULL);
      if (SA_ACL_MGR_NUM(sch_p) == 1)
         Tcl_AppendResult(interp, " ", NULL);
      else
         Tcl_AppendResult(interp, "\n {", NULL);
      for (i = 0; i < SA_ACL_MGR_NUM(sch_p); i++) {
         build_acl_perm_strings_table(&(SA_ACL_MGR_TYPE(sch_p, i)), &tmp_status);
         if (BAD_STATUS(&tmp_status)) {
            free((char *)schema_name);
            free((char *)schema_attr_name);
            free(id_svc);
            free(no_svc);
            free(yes_svc);
            free(unknown_svc);
            free(encoding_svc);
            free(acl_set_svc);
            free(unique_svc);
            free(multi_valued_svc);
            free(reserved_svc);
            free(defaults_svc);
            free(action_svc);
            free(trig_types_svc);
            free(trig_binding_svc);
            free(scope_svc);
            free(comment_svc);
            free_acl_mgr_tables();
            DCP_SET_MISC_ERROR(tmp_status);
         }
         uuid_to_string(&(SA_ACL_MGR_TYPE(sch_p, i)), &str1, &tmp_status);
         for (j = 0; j < num_acl_mgr_uuid; j++) {
            uuid_to_string(&acl_mgr_uuid[j], &str2, &tmp_status);
            if (strcmp((char *)str1, (char *)str2) == 0) {
               rpc_string_free(&str2, &tmp_status);
               break;
            }
            rpc_string_free(&str2, &tmp_status);
         }
         if (j < num_acl_mgr_uuid)
            Tcl_AppendResult(interp, "{", acl_mgr_name[j], NULL);
         else
            Tcl_AppendResult(interp, "{", str1, NULL);
         rpc_string_free(&str1, &tmp_status);

         Tcl_AppendResult(interp, " {", local_permset_name_table[0], " ", NULL);
         perm_found = FALSE;
         for (j = 0; j < num_acl_perm_strings; j++) {
            if (acl_perm_strings[j].permissions & SA_ACL_MGR_PERMS_QUERY(sch_p, i)) {
               Tcl_AppendResult(interp, acl_perm_strings[j].printstring, NULL);
               perm_found = TRUE;
            }
         }
         if (perm_found == TRUE)
            Tcl_AppendResult(interp, "}", NULL);
         else
            Tcl_AppendResult(interp, "-}", NULL);

         Tcl_AppendResult(interp, " {", local_permset_name_table[1], " ", NULL);
         perm_found = FALSE;
         for (j = 0; j < num_acl_perm_strings; j++) {
            if (acl_perm_strings[j].permissions & SA_ACL_MGR_PERMS_UPDATE(sch_p, i)) {
               Tcl_AppendResult(interp, acl_perm_strings[j].printstring, NULL);
               perm_found = TRUE;
            }
         }
         if (perm_found == TRUE)
            Tcl_AppendResult(interp, "}", NULL);
         else
            Tcl_AppendResult(interp, "-}", NULL);

         Tcl_AppendResult(interp, " {", local_permset_name_table[2], " ", NULL);
         perm_found = FALSE;
         for (j = 0; j < num_acl_perm_strings; j++) {
            if (acl_perm_strings[j].permissions & SA_ACL_MGR_PERMS_TEST(sch_p, i)) {
               Tcl_AppendResult(interp, acl_perm_strings[j].printstring, NULL);
               perm_found = TRUE;
            }
         }
         if (perm_found == TRUE)
            Tcl_AppendResult(interp, "}", NULL);
         else
            Tcl_AppendResult(interp, "-}", NULL);

         Tcl_AppendResult(interp, " {", local_permset_name_table[3], " ", NULL);
         perm_found = FALSE;
         for (j = 0; j < num_acl_perm_strings; j++) {
            if (acl_perm_strings[j].permissions & SA_ACL_MGR_PERMS_DELETE(sch_p, i)) {
               Tcl_AppendResult(interp, acl_perm_strings[j].printstring, NULL);
               perm_found = TRUE;
            }
         }
         if (perm_found == TRUE)
            Tcl_AppendResult(interp, "}}", NULL);
         else
            Tcl_AppendResult(interp, "-}}", NULL);

         if ((i == (SA_ACL_MGR_NUM(sch_p) - 1)) && (i > 0))
            Tcl_AppendResult(interp, "}}\n", NULL);
         else if (i == (SA_ACL_MGR_NUM(sch_p) - 1))
            Tcl_AppendResult(interp, "}\n", NULL);
         else
            Tcl_AppendResult(interp, "\n  ", NULL);
      }
   }
   else {
      Tcl_AppendResult(interp, "{", acl_set_svc, " {}}\n", NULL);
   }

   free_acl_perm_strings_table();
   free_acl_mgr_tables();

   /*
    * Field comment.
    */
   if (schema_entry.comment == NULL) {
      Tcl_AppendResult(interp,"{", comment_svc, " {}}\n", NULL);
   }
   else {
      Tcl_AppendResult(interp,"{", comment_svc, " {", schema_entry.comment, "}}\n", NULL);
   }

   /*
    * Field apply_defaults.
    */
   if (SA_SCH_FLAG_IS_SET_USE_DEFAULTS(sch_p)) {
      Tcl_AppendResult(interp,"{", defaults_svc, " ", yes_svc, "}\n", NULL);
   }
   else {
      Tcl_AppendResult(interp,"{", defaults_svc, " ", no_svc, "}\n", NULL);
   }

   /*
    * Field attr_encoding.
    */
   Tcl_AppendResult(interp,"{", encoding_svc, " ",
                    enc_type_name[schema_entry.attr_encoding], "}\n", NULL);

   /*
    * Field intercell_action.
    */
   Tcl_AppendResult(interp,"{", action_svc, " ",
                    intercell_act_name[schema_entry.intercell_action], "}\n", NULL);

   /*
    * Field multi_valued.
    */
   if (SA_SCH_FLAG_IS_SET_MULTI_INST(sch_p)) {
      Tcl_AppendResult(interp,"{", multi_valued_svc, " ", yes_svc, "}\n", NULL);
   }
   else {
      Tcl_AppendResult(interp,"{", multi_valued_svc, " ", no_svc, "}\n", NULL);
   }

   /*
    * Field reserved.
    */
   if (SA_SCH_FLAG_IS_SET_RESERVED(sch_p)) {
      Tcl_AppendResult(interp,"{", reserved_svc, " ", yes_svc, "}\n", NULL);
   }
   else {
      Tcl_AppendResult(interp,"{", reserved_svc, " ", no_svc, "}\n", NULL);
   }

   /*
    * Field scope.
    */
   if (schema_entry.scope == NULL) {
      Tcl_AppendResult(interp,"{", scope_svc, " {}}\n", NULL);
   }
   else {
      Tcl_AppendResult(interp,"{", scope_svc, " {", schema_entry.scope, "}}\n", NULL);
   }

   /*
    * Field trig_binding.  A binding comes in two distinct parts,
    * the auth part and the binding part.  Only one auth is allowed
    * per instance, and that can be "none" or "dce".  There can be
    * multiple bindings per instance, so output formatting is
    * going to depend on the number of bindings found.
    */
   if (SA_TRG_BND_INFO_P(sch_p)) {
      if (SA_TRG_BND_AUTH_INFO_TYPE(sch_p) == sec_attr_bind_auth_dce) {
         if (SA_TRG_BND_NUM(sch_p) == 0)
            Tcl_AppendResult(interp, "{", trig_binding_svc, " {{", authz_name[2], " ", NULL);
         else
            Tcl_AppendResult(interp, "{", trig_binding_svc, "\n {{", authz_name[2], " ", NULL);
         Tcl_AppendResult(interp, SA_TRG_BND_AUTH_SVR_PNAME_P(sch_p), " ",
                          prot_lev_name[SA_TRG_BND_AUTH_PROT_LEVEL(sch_p)], NULL);
         for (i = 0; i < MAX_AUTHN; i++) {
            if (SA_TRG_BND_AUTH_AUTHN_SVC(sch_p) == authn_val[i]) {
               Tcl_AppendResult(interp, " ", authn_name[i], NULL);
               break;
            }
         }
         Tcl_AppendResult(interp, " ", authz_name[SA_TRG_BND_AUTH_AUTHZ_SVC(sch_p)], "}", NULL);
      }
      else if (SA_TRG_BND_AUTH_INFO_TYPE(sch_p) == sec_attr_bind_auth_none) {
         if (SA_TRG_BND_NUM(sch_p) == 0)
            Tcl_AppendResult(interp, "{", trig_binding_svc, " {", authz_name[0], NULL);
         else
            Tcl_AppendResult(interp, "{", trig_binding_svc, "\n {", authz_name[0], NULL);
      }
      else {
         if (SA_TRG_BND_NUM(sch_p) == 0)
            Tcl_AppendResult(interp, "{", trig_binding_svc, " {", unknown_svc, NULL);
         else
            Tcl_AppendResult(interp, "{", trig_binding_svc, "\n {", unknown_svc, NULL);
      }
      if (SA_TRG_BND_NUM(sch_p) == 0)
         Tcl_AppendResult(interp, " {}}}\n", NULL);
      else if (SA_TRG_BND_NUM(sch_p) == 1)
         Tcl_AppendResult(interp, "\n  ", NULL);
      else
         Tcl_AppendResult(interp, "\n  {", NULL);
      for (i = 0; i < SA_TRG_BND_NUM(sch_p); i++) {
         switch((int)SA_TRG_BND_TYPE(sch_p, i)) {
            case sec_attr_bind_type_string:
               tcl_str = (char *)dcp_binding_string_to_tcl(
                  (char *)SA_TRG_BND_STRING_P(sch_p, i), &tmp_status);
               Tcl_AppendResult(interp, "{", tcl_str, "}", NULL);
               free(tcl_str);
               break;
            case sec_attr_bind_type_svrname:
               if (SA_TRG_BND_SVRNAME_P(sch_p, i)->name) 
                  Tcl_AppendResult(interp, (SA_TRG_BND_SVRNAME_P(sch_p, i))->name, NULL);
               else
                  Tcl_AppendResult(interp, "NULL", NULL);
               break;
            case sec_attr_bind_type_twrs:
               for (j = 0; j < SA_TRG_BND_TWRSET_COUNT(sch_p, i); j++) {
                  Tcl_AppendResult(interp, "{", NULL);
                  for (n = 0; n < SA_TRG_BND_TWR_LEN(sch_p, i, j); n++) {
                     sprintf(tmp_str, "%.2x", SA_TRG_BND_TWR_OCTETS(sch_p, i, j)[n]);
                     Tcl_AppendResult(interp, tmp_str, NULL);
                     if (n < SA_TRG_BND_TWR_LEN(sch_p, i, j) - 1) {
                        Tcl_AppendResult(interp, " ", NULL);
                     }
                  }
                  Tcl_AppendResult(interp, "}", NULL);
                  if ((SA_TRG_BND_TWRSET_COUNT(sch_p, i) > 1) &&
                      (j < SA_TRG_BND_TWRSET_COUNT(sch_p, i) - 1)) {
                     Tcl_AppendResult(interp, "\n   ", NULL);
                  }
               }
               break;
            default:
               /*
                * If the code gets here, it means that some illegal or
                * unknown binding type was discovered.  Set an error.
                */
               result = dcp_s_rgy_era_unknown_binding_type;
               break;
         }
         if ((i == (SA_TRG_BND_NUM(sch_p) - 1)) && (i > 0))
            Tcl_AppendResult(interp, "}}}\n", NULL);
         else if (i == (SA_TRG_BND_NUM(sch_p) - 1))
            Tcl_AppendResult(interp, "}}\n", NULL);
         else
            Tcl_AppendResult(interp, "\n   ", NULL);
      }
   }
   else {
      Tcl_AppendResult(interp, "{", trig_binding_svc, " {}}\n", NULL);
   }

   /*
    * Field trig_types.
    */
   for (i = 0; i < MAX_TRIG; i++) {
      if (schema_entry.trig_types == trig_type_val[i]) {
         break;
      }
   }
   if (i < MAX_TRIG) {
      Tcl_AppendResult(interp,"{", trig_types_svc, " ", trig_type_name[i], "}\n", NULL);
   }
   else {
      Tcl_AppendResult(interp,"{", trig_types_svc, " {}}\n", NULL);
   }

   /*
    * Field unique.
    */
   if (SA_SCH_FLAG_IS_SET_UNIQUE(sch_p)) {
      Tcl_AppendResult(interp,"{", unique_svc, " ", yes_svc, "}\n", NULL);
   }
   else {
      Tcl_AppendResult(interp,"{", unique_svc, " ", no_svc, "}\n", NULL);
   }

   /*
    * Field attr_id.
    */
   uuid_to_string(&schema_entry.attr_id, &str1, &tmp_status);
   Tcl_AppendResult(interp,"{", id_svc, " ", str1, "}", NULL);
   rpc_string_free(&str1, &tmp_status);

   /*
    * Free the stuff allocated in the schema_entry structure.
    */
   sec_attr_util_sch_ent_free_ptrs(&schema_entry);

   /* 
    * Free allocated memory. 
    */
   free(schema_name);
   free(schema_attr_name);
   free(id_svc);
   free(no_svc);
   free(yes_svc);
   free(unknown_svc);
   free(encoding_svc);
   free(acl_set_svc);
   free(unique_svc);
   free(multi_valued_svc);
   free(reserved_svc);
   free(defaults_svc);
   free(action_svc);
   free(trig_types_svc);
   free(trig_binding_svc);
   free(scope_svc);
   free(comment_svc);

   return result;
}


/*
 * FUNCTION: catalog_schema_info
 *
 * OVERVIEW:
 *  This function will take as input the name of one schema and
 *  display all of the attributes associated with that schema.
 *
 * INPUTS:
 *  char *name - The name of the schema to use.  This is expected
 *               to be the fully qualified schema name.
 *  char *full_schema_name - the fully qualified schema name, including
 *                           expanded cell name, to be used if the
 *                           -simplename option is not selected.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                       will hold the error message (if any) generated from the
 *                       command. This is also the location of any convienience
 *                       variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 */
int catalog_schema_info(Tcl_Interp *interp, char *schema_name, char *full_schema_name)
{
   int                      result = TCL_OK;
   int                      total_read = 0;
   int                      i, sec_length;
   error_status_t           status;
   unsigned32               space_avail = MAX_ATTR_SHOW;
   sec_attr_schema_entry_t  schema_entries[MAX_ATTR_SHOW];
   boolean32                done = FALSE;
   unsigned32               cur_num_entries = 0;
   unsigned32               num_read = 0;
   sec_attr_cursor_t        sch_cursor;
   char                     *full_rgy_schema_name = NULL;
   char                     *full_sec_group_name = NULL;
   unsigned_char_p_t        rs_ns_sec_group_name = NULL;

   /*
    * After obtaining the registry context of the specified
    * argument, obtain the security group name for the cell.
    */
   if (dcp_schema_read_group_name(interp, &rs_ns_sec_group_name) != TCL_OK) {
      return TCL_ERROR;
   }

   full_sec_group_name = (char *)malloc(sizeof(char) *
      (strlen((char *)rs_ns_sec_group_name) + sizeof(XATTRSCHEMA) + 2));
   if (full_sec_group_name == NULL) {
      DCP_SET_MISC_ERROR(dcp_s_no_memory);
   }
   sprintf(full_sec_group_name,
           "%s/%s",
           (char *)rs_ns_sec_group_name,
           (char *)XATTRSCHEMA);
   rpc_string_free(&rs_ns_sec_group_name, &status);

   full_rgy_schema_name = expand_cell_name(schema_name, &status);
   if (BAD_STATUS(&status)) {
       free((char *)full_sec_group_name);
       DCP_SET_MISC_ERROR(status);
   }

   /*
    * Check if the user has specified the default registry schema
    * or some other schema.  If the default registry schema is used,
    * then we use the registry version of the lookup call.  If not,
    * we use the generic version of the lookup call.
    */
   if (strcmp(full_rgy_schema_name, full_sec_group_name) == 0) {
      free((char *)full_rgy_schema_name);
      free((char *)full_sec_group_name);
      /*
       * Initialize the schema attribute cursor.
       */
      sec_rgy_attr_sch_cursor_init(rgy_context,
                                   (sec_attr_component_name_t)schema_name,
                                   &cur_num_entries,
                                   &sch_cursor,
                                   &status);
      DCP_CHECK_SEC_ERROR(status);

      if (cur_num_entries > 0) {
         /*
          * Using the schema cursor, scan through the schema
          * and retrieve all the attribute name information.
          * If there are no schema attributes associated with
          * the specified schema name, just return.  It is
          * not an error.
          */
         while (done == FALSE) {
            /*
             * Keep looping through until we have read
             * all the entries.
             */
            sec_rgy_attr_sch_scan(rgy_context,
                                  (sec_attr_component_name_t)schema_name,
                                  &sch_cursor,
                                  space_avail,
                                  &num_read,
                                  schema_entries,
                                  &status);
            DCP_CHECK_SEC_ERROR(status);
            total_read = total_read + num_read;
            if (total_read >= cur_num_entries) {
               done = TRUE;
            }

            if (num_read > 0) { 
               /*
                * The chunk of schema attribute information is stored
                * in the schema entries structure.  Loop through each
                * entry and display the attribute names.
                */
               for (i = 0; i < num_read; i++) {
                  if (full_schema_name == NULL)
                     Tcl_AppendResult(interp, schema_entries[i].attr_name, NULL);
                  else
                     Tcl_AppendResult(interp, full_schema_name, "/", schema_entries[i].attr_name, NULL);
                  if ((i != (num_read - 1)) || ((i == (num_read - 1)) && (done != TRUE)))
                     Tcl_AppendResult(interp, "\n", NULL);
               }

               /*
                * Free the pointers allocated in the schema entries structure.
                */
               for (i = 0; i < num_read; i++) {
                  sec_attr_util_sch_ent_free_ptrs(&schema_entries[i]);
               }
            }
         }
      }

      /*
       * Release the schema attribute cursor.
       */
      sec_rgy_attr_sch_cursor_release(&sch_cursor, &status);
      DCP_CHECK_SEC_ERROR(status);
   }
   else {
      /*
       * Check that there is no residual information supplied
       * by the user for a legal schema catalog name.
       */
      sec_length = strlen(full_sec_group_name);
      if (strncmp(full_rgy_schema_name, full_sec_group_name, sec_length) == 0) {
         free((char *)full_rgy_schema_name);
         free((char *)full_sec_group_name);
         DCP_SET_MISC_ERROR(dced_s_attr_bad_sch_name);
      }
      free((char *)full_rgy_schema_name);
      free((char *)full_sec_group_name);

      set_attr_schema_binding((dce_attr_component_name_t)schema_name, &status);
      DCP_CHECK_SEC_ERROR(status);
      /*
       * Initialize the schema attribute cursor.
       */
      dce_attr_sch_cursor_init(attr_schema_binding_handle,
                               &cur_num_entries,
                               (dce_attr_cursor_t *)&sch_cursor,
                               &status);
      DCP_CHECK_SEC_ERROR(status);

      if (cur_num_entries > 0) {
         /*
          * Using the schema cursor, scan through the schema
          * and retrieve all the attribute name information.
          * If there are no schema attributes associated with
          * the specified schema name, just return.  It is
          * not an error.
          */
         while (done == FALSE) {
            /*
             * Keep looping through until we have read
             * all the entries.
             */
            dce_attr_sch_scan(attr_schema_binding_handle,
                              (dce_attr_cursor_t *)&sch_cursor,
                              space_avail,
                              &num_read,
                              (dce_attr_schema_entry_t *)schema_entries,
                              &status);
            DCP_CHECK_SEC_ERROR(status);
            total_read = total_read + num_read;
            if (total_read >= cur_num_entries) {
               done = TRUE;
            }

            if (num_read > 0) {
               /*
                * The chunk of schema attribute information is stored
                * in the schema entries structure.  Loop through each
                * entry and display the attribute names.
                */
               for (i = 0; i < num_read; i++) {
                  if (full_schema_name == NULL)
                     Tcl_AppendResult(interp, schema_entries[i].attr_name, NULL);
                  else
                     Tcl_AppendResult(interp, full_schema_name, "/", schema_entries[i].attr_name, NULL);
                  if ((i != (num_read - 1)) || ((i == (num_read - 1)) && (done != TRUE)))
                     Tcl_AppendResult(interp, "\n", NULL);
               }

               /*
                * Free the pointers allocated in the schema entries structure.
                */
               for (i = 0; i < num_read; i++) {
                  sec_attr_util_sch_ent_free_ptrs(&schema_entries[i]);
               }
            }
         }
      }

      /*
       * Release the schema attribute cursor.
       */
      dce_attr_sch_cursor_release((dce_attr_cursor_t *)&sch_cursor, &status);
      DCP_CHECK_SEC_ERROR(status);
   }

   return result;
}


/*
 * FUNCTION: assign_permset
 *
 * OVERVIEW: This function takes a list of permission characters and
 *     after verifying their validity, assigns them to a permset
 *     structure.
 *
 * INPUTS:
 *  char *in_string - the string of permission characters to process.
 *  int  length - the number of permission characters to process.
 *
 * INPUT/OUTPUTS:
 *  sec_acl_permset_t *permset - structure to be assigned permission values.
 *
 * OUTPUTS:
 *   unsigned32 *result - error/success code.
 *
 * RETURNS:
 *  void
 */
void assign_permset(char *in_string, int length, sec_acl_permset_t *permset,
                    unsigned32 *result)
{
   int  i, j;

   if ((length == 1) && (in_string[0] == '-')) {
      *permset = 0;
   }
   else {
      for (i = 0; i < length; i++) {
         for (j = 0; j < num_acl_perm_strings; j++) {
            if (in_string[i] == acl_perm_strings[j].printstring[0]) {
               break;
            }
         }
         if (j == num_acl_perm_strings) {
            *result = dcp_s_rgy_invalid_permset_char;
            return;
         }
         *permset |= acl_perm_strings[j].permissions;
      }
   }
   *result = error_status_ok;
   return;
}


/*
 * FUNCTION: encode_schema_attr
 *
 * OVERVIEW: This decodes ascii schema attribute information and
 *      inserts the data into a sec_attr_schema_entry_t structure.
 *
 * INPUTS:
 *  Tcl_Interp *interp - the tcl interpreter structure for this invocation.
 *  char **attr_info - the attribute list, contains the attribute name in the
 *                     first position, then one or more values for that attribute
 *                     in the succeeding positions.
 *  int num_val - the number of attributes in the above list; note this is
 *                not the number of values, rather the number of values plus
 *                the attribute name.
 *
 * INPUT/OUTPUTS:
 *  sec_attr_schema_entry_t *schema_entry - holds the schema values for any
 *                                          schema attributes specified.
 *
 * OUTPUTS:
 *  unsigned32 *status - sends back to the calling function the status of this
 *                       function.  No problem is returned as sec_rgy_status_ok.
 *                       Other error status codes are possible.
 *
 * RETURNS:
 *  void
 */
void encode_schema_attr(Tcl_Interp *interp,
                        char **attr_info,
                        int *num_val, 
                        sec_attr_schema_entry_t *schema_entry, 
                        unsigned32 *status)
{
   int         i, j, count;
   int         found;
   int         hits;
   int         yes_no;
   int         new_length;
   int         list_length;
   boolean32   is_list = FALSE;
   boolean32   uuid_by_name;
   unsigned32  result;
   char        *attr_name;
   char        **aargv, **bargv;
   uuid_t      temp_uuid;
   int         num_entries = 1;
   sec_attr_t  *attr_p;
   sec_attr_t  attr_after_proc;
   char        **new_set;
   char        **manager_set;
   int         p_length;
   char        **p_set;

   /*
    * The attribute name is the first item in the attr_info structure.
    * The next and succeeding items in the attr structure are
    * values associated with that attribute.  If the value part of the
    * data is missing, then this is a syntax error.
    */
   attr_name = attr_info[0];

   /*
    * Process the attribute values based on the attribute
    * name passed in.  If the attribute name is not one of
    * the currently supported attributes associated with a
    * schema, then it is by definition illegal.  These bad
    * attributes will be processed by the 'default' case.
    */
   switch(schema_attribute(attr_name)) {
      /*
       * Process an attribute id.
       */
      case rgy_pgo_uuid:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         uuid_from_string((unsigned_char_t *)attr_info[1], &(schema_entry->attr_id), status);
         if (*status != error_status_ok)
            return;
         break;
 
      /*
       * Process an encoding type.
       */
      case schema_encoding:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         found = -1;
         hits = 0;
         for (i = 0; i < MAX_ENCODING; i++) {
            if (strncmp(attr_info[1], enc_type_name[i], strlen(attr_info[1])) == 0) {
               if (!strcmp(attr_info[1], enc_type_name[i])) {
                  /*
                   * Exact match means we are done.
                   */
                  hits = 1;
                  found = i;
                  break;
               }
               /*
                * We have a valid prefix - keep looking.
                */
               found = i;
               hits++;
            }
         }
         if (hits > 1) {
           *status = dcp_s_rgy_ambig_encoding_type;
           return;
         }
         if (hits == 0) {
            *status = dcp_s_rgy_unknown_encoding_type;
            return;
         }
         schema_entry->attr_encoding = found;
         break;

      /*
       * Process an acl manager set.
       */
      case schema_aclmgr:
         build_acl_mgr_tables(status);
         if (BAD_STATUS(status)) {
            return;
         }
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         /*
          * Determine if the acl_mgr_set information passed in
          * represents one set (a single list), or represents
          * more than one set (a list of lists).
          */
         is_list = isllist(attr_info[1]);

         if (is_list == TRUE) {
            /*
             * We have multiple acl mgr sets to process.
             */
            aargv = (char **)malloc(sizeof(char *) * 2);
            aargv[0] = attr_info[1];
            aargv[1] = NULL;
            if (Tcl_SplitList(interp, aargv[0], &list_length, &manager_set) != TCL_OK) {
               *status = dcp_s_rgy_invalid_acl_mgr_syntax;
               free((char *)aargv);
               return;
            }
            free((char *)aargv);
         }
         else {
            /*
             * We have but one acl mgr set to process.
             */
            list_length = 1;
            manager_set = (char **)malloc(sizeof(char *) * 2);
            manager_set[0] = attr_info[1];
            manager_set[1] = NULL;
         }

         schema_entry->acl_mgr_set = malloc(sizeof(sec_attr_acl_mgr_info_set_t) +
            (list_length * sizeof(sec_attr_acl_mgr_info_p_t)));
         schema_entry->acl_mgr_set->num_acl_mgrs = list_length;

         for (count = 0; count < list_length; count++) {
            /*
             * Split the element of the list into its component parts.
             */
            if (Tcl_SplitList(interp, manager_set[count], &new_length, &new_set) != TCL_OK) {
               *status = dcp_s_rgy_invalid_acl_mgr_syntax;
               free((char *)manager_set);
               return;
            }

            if (new_length != 5) {
               /*
                * Not the right amount of elements, so the input
                * is not a correctly formatted acl mgr set.
                */
               *status = dcp_s_rgy_invalid_acl_mgr_syntax;
               free((char *)manager_set);
               free((char *)new_set);
               return;
            }

            /*
             * Verify the acl manager set name.  It can either be
             * a uuid or a string which matches known values.  If
             * the uuid check fails, assume it's a string.
             */
            uuid_by_name = FALSE;
            uuid_from_string((unsigned_char_t *)new_set[0], &temp_uuid, &result);
            if (result != error_status_ok) {
               uuid_by_name = TRUE;
               found = -1;
               hits = 0;
               for (i = 0; i < num_acl_mgr_uuid; i++) {
                  if (strncmp(new_set[0], acl_mgr_name[i], strlen(new_set[0])) == 0) {
                     if (!strcmp(new_set[0], acl_mgr_name[i])) {
                        hits = 1;
                        found = i;
                        break;
                     }
                     found = i;
                     hits++;
                  }
               }
               if (hits > 1) {
                  *status = dcp_s_rgy_ambig_acl_mgr_name;
                  free((char *)manager_set);
                  free((char *)new_set);
                  return;
               }
               if (hits == 0) {
                  *status = dcp_s_rgy_invalid_acl_mgr_name;
                  free((char *)manager_set);
                  free((char *)new_set);
                  return;
               }
               i = found;
            }
            schema_entry->acl_mgr_set->mgr_info[count] = malloc(sizeof(sec_attr_acl_mgr_info_t));
            if (uuid_by_name == TRUE)
               schema_entry->acl_mgr_set->mgr_info[count]->acl_mgr_type = acl_mgr_uuid[i];
            else
               schema_entry->acl_mgr_set->mgr_info[count]->acl_mgr_type = temp_uuid;

            build_acl_perm_strings_table(&(SA_ACL_MGR_TYPE(schema_entry, count)), status);
            if (BAD_STATUS(status)) {
               free((char *)manager_set);
               free((char *)new_set);
               return;
            }

            /*
             * There are now four permission strings to process.  They are either
             * a simple permission string, or a {name string} pair.  Loop through
             * the four members of new_set (1 through 4) and process each one.
             * They must be in the order query, update, test, delete.
             */
            for (j = 1; j < new_length; j++) {
               /*
                * Split the parameter.  The length tells which format
                * is being used.
                */
               if (Tcl_SplitList(interp, new_set[j], &p_length, &p_set) != TCL_OK) {
                  *status = dcp_s_rgy_invalid_acl_mgr_syntax;
                  free((char *)manager_set);
                  free((char *)new_set);
                  return;
               }
               if (p_length > 2) {
                  *status = dcp_s_rgy_invalid_acl_mgr_syntax;
                  free((char *)manager_set);
                  free((char *)new_set);
                  free((char *)p_set);
                  return;
               }
               else if (p_length == 2) {
                  /*
                   * Make sure the correct permset name is specified.
                   */
                  if (strncmp(local_permset_name_table[j-1], p_set[0], strlen(p_set[0])) != 0) {
                     *status = dcp_s_rgy_invalid_acl_mgr_syntax;
                     free((char *)manager_set);
                     free((char *)new_set);
                     free((char *)p_set);
                     return;
                  }
               }

               switch (j) {
                  case 1:
                     schema_entry->acl_mgr_set->mgr_info[count]->query_permset = 0;
                     assign_permset(p_set[p_length-1],
                                    strlen(p_set[p_length-1]),
                                    &schema_entry->acl_mgr_set->mgr_info[count]->query_permset,
                                    status);
                     break;
                  case 2:
                     schema_entry->acl_mgr_set->mgr_info[count]->update_permset = 0;
                     assign_permset(p_set[p_length-1],
                                    strlen(p_set[p_length-1]),
                                    &schema_entry->acl_mgr_set->mgr_info[count]->update_permset,
                                    status);
                     break;
                  case 3:
                     schema_entry->acl_mgr_set->mgr_info[count]->test_permset = 0;
                     assign_permset(p_set[p_length-1],
                                    strlen(p_set[p_length-1]),
                                    &schema_entry->acl_mgr_set->mgr_info[count]->test_permset,
                                    status);
                     break;
                  case 4:
                     schema_entry->acl_mgr_set->mgr_info[count]->delete_permset = 0;
                     assign_permset(p_set[p_length-1],
                                    strlen(p_set[p_length-1]),
                                    &schema_entry->acl_mgr_set->mgr_info[count]->delete_permset,
                                    status);
                     break;
               }
               free((char *)p_set);
               if (*status != error_status_ok) {
                  free((char *)manager_set);
                  free((char *)new_set);
                  return;
               }
            }

            /*
             * Free the data storage structure.
             */
            free((char *)new_set);
         }
         free((char *)manager_set);
         free_acl_perm_strings_table();
         free_acl_mgr_tables();
         break;

      /*
       * Process the unique flag.
       */
      case schema_unique:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         yes_no = is_yes(attr_info[1]);
         if (yes_no == TRUE)
            SA_SCH_FLAG_SET_UNIQUE(schema_entry);
         else if (yes_no == FALSE)
            SA_SCH_FLAG_UNSET_UNIQUE(schema_entry);
         else {
            *status = yes_no;
            return;
         }
         break;

      /*
       * Process the multi_valued flag.
       */
      case schema_multivalued:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         yes_no = is_yes(attr_info[1]);
         if (yes_no == TRUE)
            SA_SCH_FLAG_SET_MULTI_INST(schema_entry);
         else if (yes_no == FALSE)
            SA_SCH_FLAG_UNSET_MULTI_INST(schema_entry);
         else {
            *status = yes_no;
            return;
         }
         break;

      /*
       * Process the reserved flag.
       */
      case schema_reserved:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         yes_no = is_yes(attr_info[1]);
         if (yes_no == TRUE)
            SA_SCH_FLAG_SET_RESERVED(schema_entry);
         else if (yes_no == FALSE)
            SA_SCH_FLAG_UNSET_RESERVED(schema_entry);
         else {
            *status = yes_no;
            return;
         }
         break;

      /*
       * Process the apply_defaults flag.
       */
      case schema_applydefs:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         yes_no = is_yes(attr_info[1]);
         if (yes_no == TRUE)
            SA_SCH_FLAG_SET_USE_DEFAULTS(schema_entry);
         else if (yes_no == FALSE)
            SA_SCH_FLAG_UNSET_USE_DEFAULTS(schema_entry);
         else {
            *status = yes_no;
            return;
         }
         break;

      /*
       * Process an intercell_action type.
       */
      case schema_intercell:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         found = -1;
         hits = 0;
         for (i = 0; i < MAX_INTERCELL; i++) {
            if (strncmp(attr_info[1], intercell_act_name[i], strlen(attr_info[1])) == 0) {
               if (!strcmp(attr_info[1], intercell_act_name[i])) {
                  hits = 1;
                  found = i;
                  break;
               }
               found = i;
               hits++;
            }
         }
         if (hits > 1) {
            *status = dcp_s_rgy_ambig_intercell_action;
            return;
         }
         if (hits == 0) {
            *status = dcp_s_rgy_invalid_intercell_action;
            return;
         }
         schema_entry->intercell_action = found;
         break;

      /*
       * Process a trigger type.
       */
      case schema_trigtype:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         found = -1;
         hits = 0;
         for (i = 0; i < MAX_TRIG; i++) {
            if (strncmp(attr_info[1], trig_type_name[i], strlen(attr_info[1])) == 0) {
               if (!strcmp(attr_info[1], trig_type_name[i])) {
                  hits = 1;
                  found = i;
                  break;
               }
               found = i;
               hits++;
            }
         }
         if (hits > 1) {
            *status = dcp_s_rgy_ambig_trigger_type;
            return;
         }
         if (hits == 0) {
            *status = dcp_s_rgy_invalid_trigger_type;
            return;
         }
         schema_entry->trig_types = trig_type_val[found];
         break;

      /*
       * Process trigger bindings.
       */
      case schema_trigbind:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         /*
          * Take advantage of the binding work done in the
          * function that gets attribute values from a list.
          */
         bargv = (char **)malloc(sizeof(char *) * 3);
         bargv[0] = NULL;
         bargv[1] = attr_info[1];
         bargv[2] = NULL;
         uuid_create_nil(&temp_uuid, status);
         if (*status != error_status_ok) {
            free((char *)bargv);
            return;
         }
         *status = get_attr_values(interp,
                                   sec_attr_enc_binding,
                                   temp_uuid,
                                   &attr_after_proc,
                                   &num_entries,
                                   bargv);
         if (*status != sec_rgy_status_ok) {
            free((char *)bargv);
            return;
         }
         free((char *)bargv);

         /*
          * If we get here, it means we have legal trig binding
          * information, stored in the attr_info structure.
          * Allocate enough space in schema_entry to hold all
          * the new information.
          *
          * First define a shorthand method of referencing
          * the stored attribute information.
          */
         attr_p = &attr_after_proc;

         /*
          * Do the auth_info part.
          */
         SA_TRG_BND_INFO_P(schema_entry) = (sec_attr_bind_info_t *) malloc(
            sizeof(sec_attr_bind_info_t) + (SA_ATTR_BND_NUM(attr_p) * sizeof(sec_attr_binding_t)));
         SA_TRG_BND_AUTH_INFO_TYPE(schema_entry) = SA_ATTR_BND_AUTH_INFO_TYPE(attr_p);
         if (SA_TRG_BND_AUTH_INFO_TYPE(schema_entry) == sec_attr_bind_auth_dce) {
            SA_TRG_BND_AUTH_SVR_PNAME_P(schema_entry) = (unsigned_char_p_t) malloc(
               sizeof(unsigned_char_t) * (strlen((char *)SA_ATTR_BND_AUTH_SVR_PNAME_P(attr_p)) + 1));
            strcpy((char *)SA_TRG_BND_AUTH_SVR_PNAME_P(schema_entry),
                   (char *)SA_ATTR_BND_AUTH_SVR_PNAME_P(attr_p));
            SA_TRG_BND_AUTH_PROT_LEVEL(schema_entry) = SA_ATTR_BND_AUTH_PROT_LEVEL(attr_p);
            SA_TRG_BND_AUTH_AUTHN_SVC(schema_entry) = SA_ATTR_BND_AUTH_AUTHN_SVC(attr_p);
            SA_TRG_BND_AUTH_AUTHZ_SVC(schema_entry) = SA_ATTR_BND_AUTH_AUTHZ_SVC(attr_p);
         }

         /*
          * Do the bindings part (including num_bindings).
          */
         SA_TRG_BND_NUM(schema_entry) = SA_ATTR_BND_NUM(attr_p);
         for (i = 0; i < SA_TRG_BND_NUM(schema_entry); i++) {
            /*
             * Do the binding type part.
             */
            SA_TRG_BND_TYPE(schema_entry, i) = SA_ATTR_BND_TYPE(attr_p, i);
            switch((int)SA_TRG_BND_TYPE(schema_entry, i)) {
               case sec_attr_bind_type_string:
                  /*
                   * Do the string binding part.
                   */
                  SA_TRG_BND_STRING_P(schema_entry, i) = (unsigned_char_p_t) malloc(
                     sizeof(unsigned_char_t) * (strlen((char *)SA_ATTR_BND_STRING_P(attr_p, i)) + 1));
                  strcpy((char *)SA_TRG_BND_STRING_P(schema_entry, i),
                         (char *)SA_ATTR_BND_STRING_P(attr_p, i));
                  break;

               case sec_attr_bind_type_svrname:
                  /*
                   * Do the svrname part.
                   */
                  SA_TRG_BND_SVRNAME_P(schema_entry,i) = (sec_attr_bind_svrname *) malloc(
                     sizeof(sec_attr_bind_svrname));

                  /*
                   * Do the name syntax part.
                   */
                  SA_TRG_BND_SVRNAME_SYNTAX(schema_entry, i) = SA_ATTR_BND_SVRNAME_SYNTAX(attr_p, i);

                  /*
                   * Do the name part.
                   */
                  SA_TRG_BND_SVRNAME_NAME_P(schema_entry, i) = (unsigned_char_p_t) malloc(
                     sizeof(char) * (strlen((char *)SA_ATTR_BND_SVRNAME_NAME_P(attr_p, i)) + 1));
                  strcpy((char *)SA_TRG_BND_SVRNAME_NAME_P(schema_entry, i),
                         (char *)SA_ATTR_BND_SVRNAME_NAME_P(attr_p, i));
                  break;

               case sec_attr_bind_type_twrs:
                  SA_TRG_BND_TWRSET_P(schema_entry, i) = 
                     malloc(sizeof(sec_attr_twr_set_t) + 
                        (SA_ATTR_BND_TWRSET_COUNT(attr_p, i) - 1) * sizeof(sec_attr_twr_ref_t));
                  SA_TRG_BND_TWRSET_COUNT(schema_entry, i) = SA_ATTR_BND_TWRSET_COUNT(attr_p, i);
                  for (j = 0; j < SA_TRG_BND_TWRSET_COUNT(schema_entry, i); j++) {
                     SA_TRG_BND_TWR_P(schema_entry, i, j) = 
                        malloc(sizeof(twr_t) + (SA_ATTR_BND_TWR_LEN(attr_p, i, j) - 1) * sizeof(idl_byte));
                     SA_TRG_BND_TWR_LEN(schema_entry, i, j) = SA_ATTR_BND_TWR_LEN(attr_p, i ,j);
                     memcpy(SA_TRG_BND_TWR_OCTETS(schema_entry, i, j), 
                            SA_ATTR_BND_TWR_OCTETS(attr_p, i, j), SA_ATTR_BND_TWR_LEN(attr_p, i, j));
                  }
                  break;

               default:
                  /*
                   * If the code gets here, it means that some illegal or
                   * unknown binding type was discovered.  Set an error.
                   */
                  *status = dcp_s_rgy_era_unknown_binding_type;
                  sec_attr_util_inst_free_ptrs(attr_p);
                  return;
            }
         }
         sec_attr_util_inst_free_ptrs(attr_p);
         break;

      /*
       * Process a scope string.
       */
      case schema_scope:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         schema_entry->scope = malloc(sizeof(char) * (strlen(attr_info[1]) + 1));
         strcpy((char *)schema_entry->scope, (char *)attr_info[1]);
         break;

      /*
       * Process a comment string.
       */
      case schema_annotation:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         schema_entry->comment = malloc(sizeof(char) * (strlen(attr_info[1]) + 1));
         strcpy((char *)schema_entry->comment, (char *)attr_info[1]);
         break;

      /*
       * Process an unknown attribute name.
       */
      default:
         *status = dcp_s_rgy_bad_attribute_name;
         return;
   }

   *status = sec_rgy_status_ok;
   return;
}


/*
 * FUNCTION: parse_schema_attr_list
 *
 * OVERVIEW: This encodes ascii values into a schema structure according
 *     to the list of data.  That is, the input string is parsed into
 *     is component parts and processed individually.
 *
 * INPUTS:
 *  char *attr_string - The character string to parse.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp, - interpreter to store information
 *  sec_attr_schema_entry_t *schema_entry - structure to be filled with encoded values.
 *
 * OUTPUTS:
 *   unsigned32 *result - error/success code.
 *
 * RETURNS:
 *  void
 */
void parse_schema_attr_list(Tcl_Interp *interp, char *attr_string,
                            sec_attr_schema_entry_t *schema_entry,
                            unsigned32 *result)
{
   int        i;
   boolean32  is_list = FALSE;
   char       **aargv, **pargv;
   int        aargc = 0;
   int        pargc = 0;

   if (attr_string != NULL) {
      /*
       * Check if the input string is represents a
       * single attribute-value pair, or is a list
       * of attribute-value pairs.
       */
      is_list = isllist(attr_string);
      if (is_list) {
         /*
          * The input is a list of attribute-value pairs.
          */
         if (Tcl_SplitList(interp, attr_string, &pargc, &pargv) != TCL_OK) {
            *result = TCL_ERROR;
            return;
         }
      }
      else {
         /*
          * The input is a single attribute-value pair.
          */
         pargc = 1;
         pargv = (char **)malloc(2*sizeof(char *));
         if (pargv == NULL) {
            *result = dcp_s_no_memory;
            return;
         }
         pargv[0] = attr_string;
         pargv[1] = NULL;
      }

      /*
       * Process each attribute-value pair separately.
       */
      for (i = 0; i < pargc; i++) {
         if (Tcl_SplitList(interp, pargv[i], &aargc, &aargv) != TCL_OK) {
            *result = TCL_ERROR;
            free((char *)pargv);
            return;
         }

         /*
          * Take the aargv structure and pass it along to the
          * encoding function, which will do the work.
          */
         encode_schema_attr(interp, aargv, &aargc, schema_entry, result);
         if (*result != error_status_ok) {
            free((char *)pargv);
            free((char *)aargv);
            return;
         }

         free((char *)aargv);
      }
      free((char *)pargv);
   }
   *result = TCL_OK;
   return;
}


/*
 * FUNCTION: parse_schema_switch_list
 *
 * OVERVIEW: This encodes ascii values into a schema structure according
 *     to the variable name.  That is, attr_id is encoded into the schema
 *     structure as the field attr_id.
 *
 * INPUTS:
 *  char *attr_id - ascii uuid of the schema.
 *  char *attr_encoding - the encoding type, one of the legal list.
 *  char *acl_mgr_set - the acl_mgr_set value for the schema.
 *  char *unique - ascii yes/no whether or not the schema is unique. 
 *  char *multi_valued - ascii yes/no whether or not the schema allows multiple values. 
 *  char *reserved - ascii yes/no whether or not the schema is reserved. 
 *  char *apply_defaults - ascii yes/no whether or not the schema should apply defaults. 
 *  char *intercell_action - one of a set of legal intercell action values.
 *  char *trig_types - one of a set of legal trig type values.
 *  char *trig_binding - The trig binding for the schema.
 *  char *scope - ascii string describing the scope of the schema.
 *  char *comment - ascii string describing any comments for the schema.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp, - interpreter to store information
 *  sec_attr_schema_entry_t *schema_entry - structure to be filled with encoded values.
 *
 * OUTPUTS:
 *   unsigned32 *result - error/success code.
 *
 * RETURNS:
 *  void
 */
void parse_schema_switch_list(Tcl_Interp *interp, char *attr_id, char *attr_encoding,
                              char *acl_mgr_set, char *unique, char *multi_valued,
                              char *reserved, char *apply_defaults, char *intercell_action,
                              char *trig_types, char *trig_binding, char *scope,
                              char *comment, sec_attr_schema_entry_t *schema_entry,
                              unsigned32 *result)
{
   int         i, j, count;
   int         found;
   int         hits;
   int         yes_no;
   int         new_length;
   int         list_length;
   boolean32   is_list = FALSE;
   boolean32   uuid_by_name;
   char        **aargv, **bargv;
   uuid_t      temp_uuid;
   int         num_entries = 1;
   sec_attr_t  *attr_p;
   sec_attr_t  attr_info;
   char        **new_set;
   char        **manager_set;
   int         p_length;
   char        **p_set;

   if (attr_id == NULL) {
      uuid_create_nil(&(schema_entry->attr_id), result);
      if (*result != error_status_ok)
         return;
   }
   else {
      uuid_from_string((unsigned_char_t *)attr_id, &(schema_entry->attr_id), result);
      if (*result != error_status_ok)
         return;
   }

   if (attr_encoding == NULL) {
      schema_entry->attr_encoding = -1;
   }
   else {
      found = -1;
      hits = 0;
      for (i = 0; i < MAX_ENCODING; i++) {
         if (strncmp(attr_encoding, enc_type_name[i], strlen(attr_encoding)) == 0) {
            if (!strcmp(attr_encoding, enc_type_name[i])) {
               hits = 1;
               found = i;
               break;
            }
            found = i;
            hits++;
         }
      }
      if (hits > 1) {
         *result = dcp_s_rgy_ambig_encoding_type;
         return;
      }
      if (hits == 0) {
         /*
          * The string passed for the encoding type did not match
          * one of the legal shorthand strings.  This is an error.
          */
         *result = dcp_s_rgy_unknown_encoding_type;
         return;
      }
      schema_entry->attr_encoding = found;
   }

   if (acl_mgr_set == NULL) {
      schema_entry->acl_mgr_set = malloc(SA_ACL_MGR_SET_SIZE(1));
      schema_entry->acl_mgr_set->num_acl_mgrs = 0;
   }
   else {
      build_acl_mgr_tables(result);
      if (BAD_STATUS(result)) {
         return;
      }

      /*
       * Determine if the acl_mgr_set information passed in
       * represents one set (a single list), or represents
       * more than one set (a list of lists).
       */
      is_list = isllist(acl_mgr_set);

      if (is_list == TRUE) {
         /*
          * We have multiple acl mgr sets to process.
          */
         aargv = (char **)malloc(sizeof(char *) * 2);
         aargv[0] = acl_mgr_set;
         aargv[1] = NULL;
         if (Tcl_SplitList(interp, aargv[0], &list_length, &manager_set) != TCL_OK) {
            *result = dcp_s_rgy_invalid_acl_mgr_syntax;
            free((char *)aargv);
            return;
         }
      }
      else {
         /*
          * We have but one acl mgr set to process.
          */
         list_length = 1;
         manager_set = (char **)malloc(sizeof(char *) * 2);
         manager_set[0] = acl_mgr_set;
         manager_set[1] = NULL;
      }

      schema_entry->acl_mgr_set = malloc(sizeof(sec_attr_acl_mgr_info_set_t) +
         (list_length * sizeof(sec_attr_acl_mgr_info_p_t)));
      schema_entry->acl_mgr_set->num_acl_mgrs = list_length;

      for (count = 0; count < list_length; count++) {
         /*
          * Split the element of the list into its component parts.
          */
         if (Tcl_SplitList(interp, manager_set[count], &new_length, &new_set) != TCL_OK) {
            *result = dcp_s_rgy_invalid_acl_mgr_syntax;
            free((char *)manager_set);
            return;
         }

         if (new_length != 5) {
            /*
             * Not the right amount of elements, so the input
             * is not a correctly formatted acl mgr set.
             */
            *result = dcp_s_rgy_invalid_acl_mgr_syntax;
            free((char *)manager_set);
            free((char *)new_set);
            return;
         }

         /*
          * Verify the acl manager set name.  It can either be
          * a uuid or a string which matches known values.  If
          * the uuid check fails, assume it's a string.
          */
         uuid_by_name = FALSE;
         uuid_from_string((unsigned_char_t *)new_set[0], &temp_uuid, result);
         if (*result != error_status_ok) {
            uuid_by_name = TRUE;
            found = -1;
            hits = 0;
            for (i = 0; i < num_acl_mgr_uuid; i++) {
               if (strncmp(new_set[0], acl_mgr_name[i], strlen(new_set[0])) == 0) {
                  if (!strcmp(new_set[0], acl_mgr_name[i])) {
                     hits = 1;
                     found = i;
                     break;
                  }
                  found = i;
                  hits++;
               }
            }
            if (hits > 1) { 
               *result = dcp_s_rgy_ambig_acl_mgr_name;
               free((char *)manager_set);
               free((char *)new_set);
               return;
            }
            if (hits == 0) {
               *result = dcp_s_rgy_invalid_acl_mgr_name;
               free((char *)manager_set);
               free((char *)new_set);
               return;
            }
            i = found;
         }
         schema_entry->acl_mgr_set->mgr_info[count] = malloc(sizeof(sec_attr_acl_mgr_info_t));
         if (uuid_by_name == TRUE)
            schema_entry->acl_mgr_set->mgr_info[count]->acl_mgr_type = acl_mgr_uuid[i];
         else
            schema_entry->acl_mgr_set->mgr_info[count]->acl_mgr_type = temp_uuid;

         build_acl_perm_strings_table(&(SA_ACL_MGR_TYPE(schema_entry, count)), result);
         if (BAD_STATUS(result))
         {
            free((char *)manager_set);
            free((char *)new_set);
            return;
         }

         /*
          * There are now four permission strings to process.  They are either
          * a simple permission string, or a {name string} pair.  Loop through
          * the four members of new_set (1 through 4) and process each one.
          * They must be in the order query, update, test, delete.
          */
         for (j = 1; j < new_length; j++) {
            /*
             * Split the parameter.  The length tells which format
             * is being used.
             */
            if (Tcl_SplitList(interp, new_set[j], &p_length, &p_set) != TCL_OK) {
               *result = dcp_s_rgy_invalid_acl_mgr_syntax;
               free((char *)manager_set);
               free((char *)new_set);
               return;
            }
            if (p_length > 2) {
               *result = dcp_s_rgy_invalid_acl_mgr_syntax;
               free((char *)manager_set);
               free((char *)new_set);
               free((char *)p_set);
               return;
            }
            else if (p_length == 2) {
               /*
                * Make sure the correct permset name is specified.
                */
               if (strncmp(local_permset_name_table[j-1], p_set[0], strlen(p_set[0])) != 0) {
                  *result = dcp_s_rgy_invalid_acl_mgr_syntax;
                  free((char *)manager_set);
                  free((char *)new_set);
                  free((char *)p_set);
                  return;
               }
            }

            switch (j) {
               case 1:
                  schema_entry->acl_mgr_set->mgr_info[count]->query_permset = 0;
                  assign_permset(p_set[p_length-1],
                                 strlen(p_set[p_length-1]),
                                 &schema_entry->acl_mgr_set->mgr_info[count]->query_permset,
                                 result);
                  break;
               case 2:
                  schema_entry->acl_mgr_set->mgr_info[count]->update_permset = 0;
                  assign_permset(p_set[p_length-1],
                                 strlen(p_set[p_length-1]),
                                 &schema_entry->acl_mgr_set->mgr_info[count]->update_permset,
                                 result);
                  break;
               case 3:
                  schema_entry->acl_mgr_set->mgr_info[count]->test_permset = 0;
                  assign_permset(p_set[p_length-1],
                                 strlen(p_set[p_length-1]),
                                 &schema_entry->acl_mgr_set->mgr_info[count]->test_permset,
                                 result);
                  break;
               case 4:
                  schema_entry->acl_mgr_set->mgr_info[count]->delete_permset = 0;
                  assign_permset(p_set[p_length-1],
                                 strlen(p_set[p_length-1]),
                                 &schema_entry->acl_mgr_set->mgr_info[count]->delete_permset,
                                 result);
                  break;
            }
            free((char *)p_set);
            if (*result != error_status_ok) {
               free((char *)manager_set);
               free((char *)new_set);
               return;
            }
         }

         /*
          * Free the data storage structure.
          */
         free((char *)new_set);
      }
      free((char *)manager_set);
      free_acl_perm_strings_table();
      free_acl_mgr_tables();
   }

   if (unique == NULL) {
      SA_SCH_FLAG_UNSET_UNIQUE(schema_entry);
   }
   else {
      yes_no = is_yes(unique);
      if (yes_no == TRUE)
         SA_SCH_FLAG_SET_UNIQUE(schema_entry);
      else if (yes_no == FALSE)
         SA_SCH_FLAG_UNSET_UNIQUE(schema_entry);
      else {
         *result = yes_no;
         return;
      }
   }

   if (multi_valued == NULL) {
      SA_SCH_FLAG_UNSET_MULTI_INST(schema_entry);
   }
   else {
      yes_no = is_yes(multi_valued);
      if (yes_no == TRUE)
         SA_SCH_FLAG_SET_MULTI_INST(schema_entry);
      else if (yes_no == FALSE)
         SA_SCH_FLAG_UNSET_MULTI_INST(schema_entry);
      else {
         *result = yes_no;
         return;
      }
   }

   if (reserved == NULL) {
      SA_SCH_FLAG_UNSET_RESERVED(schema_entry);
   }
   else {
      yes_no = is_yes(reserved);
      if (yes_no == TRUE)
         SA_SCH_FLAG_SET_RESERVED(schema_entry);
      else if (yes_no == FALSE)
         SA_SCH_FLAG_UNSET_RESERVED(schema_entry);
      else {
         *result = yes_no;
         return;
      }
   }

   if (apply_defaults == NULL) {
      SA_SCH_FLAG_UNSET_USE_DEFAULTS(schema_entry);
   }
   else {
      yes_no = is_yes(apply_defaults);
      if (yes_no == TRUE)
         SA_SCH_FLAG_SET_USE_DEFAULTS(schema_entry);
      else if (yes_no == FALSE)
         SA_SCH_FLAG_UNSET_USE_DEFAULTS(schema_entry);
      else {
         *result = yes_no;
         return;
      }
   }

   if (intercell_action == NULL) {
      schema_entry->intercell_action = sec_attr_intercell_act_reject;
   }
   else {
      found=-1;
      hits=0;
      for (i = 0; i < MAX_INTERCELL; i++) {
         if (strncmp(intercell_action, intercell_act_name[i], strlen(intercell_action)) == 0) {
            if (!strcmp(intercell_action, intercell_act_name[i])) {
               hits = 1;
               found = i;
               break;
            }
            found = i;
            hits++;
         }
      }
      if (hits > 1) {
         *result = dcp_s_rgy_ambig_intercell_action;
         return;
      }
      if (hits == 0) {
         /*
          * The value passed in for the intercell action field did not
          * match one of the legal values.  This is an error.
          */
         *result = dcp_s_rgy_invalid_intercell_action;
         return;
      }
      schema_entry->intercell_action = found;
   }

   if (trig_types == NULL) {
      schema_entry->trig_types = sec_attr_trig_type_none;
   }
   else {
      found = -1;
      hits = 0;
      for (i = 0; i < MAX_TRIG; i++) {
         if (strncmp(trig_types, trig_type_name[i], strlen(trig_types)) == 0) {
            if (!strcmp(trig_types, trig_type_name[i])) {
               hits = 1;
               found = i;
               break;
            }
            found = i;
            hits++;
         }
      }
      if (hits > 1) {
         *result = dcp_s_rgy_ambig_trigger_type;
         return;
      }
      if (hits == 0) {
         /*
          * The value passed in for the trigger type field did not
          * match one of the legal values.  This is an error.
          */
         *result = dcp_s_rgy_invalid_trigger_type;
         return;
      }
      schema_entry->trig_types = trig_type_val[found];
   }

   if (trig_binding == NULL) {
      schema_entry->trig_binding = NULL;
   }
   else {
      /*
       * Take advantage of the binding work done in the
       * function that gets attribute values from a list.
       * To do this, we have to send the trig binding string
       * via the bargv structure.
       */
      bargv = (char **)malloc(sizeof(char *) * 3);
      bargv[0] = NULL;
      bargv[1] = trig_binding;
      bargv[2] = NULL;
      uuid_create_nil(&temp_uuid, result);
      *result = get_attr_values(interp,
                                sec_attr_enc_binding,
                                temp_uuid,
                                &attr_info,
                                &num_entries,
                                bargv);
      if (*result != sec_rgy_status_ok) {
         free((char *)bargv);
         return;
      }
      free((char *)bargv);

      /*
       * If we get here, it means we have legal trig binding
       * information, stored in the attr_info structure.
       * Allocate enough space in schema_entry to hold all
       * the new information.
       *
       * First define a shorthand method of referencing
       * the stored attribute information.
       */
      attr_p = &attr_info;

      /*
       * Do the auth_info part.
       */
      SA_TRG_BND_INFO_P(schema_entry) = (sec_attr_bind_info_t *) malloc(
         sizeof(sec_attr_bind_info_t) + (SA_ATTR_BND_NUM(attr_p) * sizeof(sec_attr_binding_t)));
      SA_TRG_BND_AUTH_INFO_TYPE(schema_entry) = SA_ATTR_BND_AUTH_INFO_TYPE(attr_p);
      if (SA_TRG_BND_AUTH_INFO_TYPE(schema_entry) == sec_attr_bind_auth_dce) {
         SA_TRG_BND_AUTH_SVR_PNAME_P(schema_entry) = (unsigned_char_p_t) malloc(
            sizeof(unsigned_char_t) * (strlen((char *)SA_ATTR_BND_AUTH_SVR_PNAME_P(attr_p)) + 1));
         strcpy((char *)SA_TRG_BND_AUTH_SVR_PNAME_P(schema_entry),
                (char *)SA_ATTR_BND_AUTH_SVR_PNAME_P(attr_p));
         SA_TRG_BND_AUTH_PROT_LEVEL(schema_entry) = SA_ATTR_BND_AUTH_PROT_LEVEL(attr_p);
         SA_TRG_BND_AUTH_AUTHN_SVC(schema_entry) = SA_ATTR_BND_AUTH_AUTHN_SVC(attr_p);
         SA_TRG_BND_AUTH_AUTHZ_SVC(schema_entry) = SA_ATTR_BND_AUTH_AUTHZ_SVC(attr_p);
      }

      /*
       * Do the bindings part (including num_bindings).
       */
      SA_TRG_BND_NUM(schema_entry) = SA_ATTR_BND_NUM(attr_p);
      for (i = 0; i < SA_TRG_BND_NUM(schema_entry); i++) {
         /*
          * Do the binding type part.
          */
         SA_TRG_BND_TYPE(schema_entry, i) = SA_ATTR_BND_TYPE(attr_p, i);
         switch((int)SA_TRG_BND_TYPE(schema_entry, i)) {
            case sec_attr_bind_type_string:
               /*
                * Do the string binding part.
                */
               SA_TRG_BND_STRING_P(schema_entry, i) = (unsigned_char_p_t) malloc(
                  sizeof(unsigned_char_t) * (strlen((char *)SA_ATTR_BND_STRING_P(attr_p, i)) + 1));
               strcpy((char *)SA_TRG_BND_STRING_P(schema_entry, i),
                      (char *)SA_ATTR_BND_STRING_P(attr_p, i));
               break;

            case sec_attr_bind_type_svrname:
               /*
                * Do the svrname part.
                */
               SA_TRG_BND_SVRNAME_P(schema_entry,i) = (sec_attr_bind_svrname *) malloc(
                  sizeof(sec_attr_bind_svrname));

               /*
                * Do the name syntax part.
                */
               SA_TRG_BND_SVRNAME_SYNTAX(schema_entry, i) = SA_ATTR_BND_SVRNAME_SYNTAX(attr_p, i);

               /*
                * Do the name part.
                */
               SA_TRG_BND_SVRNAME_NAME_P(schema_entry, i) = (unsigned_char_p_t) malloc(
                  sizeof(char) * (strlen((char *)SA_ATTR_BND_SVRNAME_NAME_P(attr_p, i)) + 1));
               strcpy((char *)SA_TRG_BND_SVRNAME_NAME_P(schema_entry, i),
                      (char *)SA_ATTR_BND_SVRNAME_NAME_P(attr_p, i));
               break;

            case sec_attr_bind_type_twrs:
               SA_TRG_BND_TWRSET_P(schema_entry, i) = malloc(
                  sizeof(sec_attr_twr_set_t) +
                  (SA_ATTR_BND_TWRSET_COUNT(attr_p, i) - 1) * sizeof(sec_attr_twr_ref_t));
               SA_TRG_BND_TWRSET_COUNT(schema_entry, i) = SA_ATTR_BND_TWRSET_COUNT(attr_p, i);
               for (j = 0; j < SA_TRG_BND_TWRSET_COUNT(schema_entry, i); j++) {
                  SA_TRG_BND_TWR_P(schema_entry,i,j) =
                     malloc(sizeof(twr_t) + (SA_ATTR_BND_TWR_LEN(attr_p, i, j) - 1) * sizeof(idl_byte));
                  SA_TRG_BND_TWR_LEN(schema_entry, i, j) = SA_ATTR_BND_TWR_LEN(attr_p, i, j);
                  memcpy(SA_TRG_BND_TWR_OCTETS(schema_entry, i, j),
                         SA_ATTR_BND_TWR_OCTETS(attr_p, i, j), SA_ATTR_BND_TWR_LEN(attr_p, i, j));
               }
               break;

            default:
               /*
                * If the code gets here, it means that some illegal or
                * unknown binding type was discovered.  Set an error.
                */
               *result = dcp_s_rgy_era_unknown_binding_type;
               sec_attr_util_inst_free_ptrs(attr_p);
               return;
         }
      }
      sec_attr_util_inst_free_ptrs(attr_p);
   }

   if (scope == NULL) {
      schema_entry->scope = NULL;
   }
   else {
      schema_entry->scope = malloc(sizeof(char) * (strlen(scope) + 1));
      strcpy((char *)schema_entry->scope, (char *)scope);
   }

   if (comment == NULL) {
      schema_entry->comment = NULL;
   }
   else {
      schema_entry->comment = malloc(sizeof(char) * (strlen(comment) + 1));
      strcpy((char *)schema_entry->comment, (char *)comment);
   }
}

/*
 * FUNCTION: parse_modify_schema_switch_list
 *
 * OVERVIEW: This encodes ascii values into a schema structure according
 *     to the variable name.
 *
 * INPUTS:
 *  char *acl_mgr_set - the acl_mgr_set value for the schema.
 *  char *reserved - ascii yes/no whether or not the schema is reserved.
 *  char *apply_defaults - ascii yes/no whether or not the schema should apply defaults.
 *  char *intercell_action - one of a set of legal intercell action values.
 *  char *trig_binding - The trig binding for the schema.
 *  char *comment - ascii string describing any comments for the schema.
 *
 * INPUT/OUTPUTS:
 *  sec_attr_schema_entry_t *schema_entry - structure to be filled with encoded values.
 *
 * OUTPUTS:
 *  sec_attr_schema_entry_parts_t *modify_parts - flag that indicates which fields 
 *                                                are to be modified.
 *  unsigned32 *result - error/success code.
 *
 * RETURNS:
 *  void
 */
void parse_modify_schema_switch_list(Tcl_Interp *interp,
                                     char *acl_mgr_set,
                                     char *reserved,
                                     char *apply_defaults,
                                     char *intercell_action,
                                     char *trig_binding,
                                     char *comment,
                                     sec_attr_schema_entry_t *schema_entry,
                                     sec_attr_schema_entry_parts_t *modify_parts,
                                     unsigned32 *result)
{
   int         i, j;
   int         found;
   int         hits;
   int         count;
   int         yes_no;
   int         list_length;
   int         new_length;
   boolean32   is_list = FALSE;
   boolean32   uuid_by_name;
   char        **aargv, **bargv;
   uuid_t      temp_uuid;
   int         num_entries = 1;
   sec_attr_t  *attr_p;
   sec_attr_t  attr_info;
   char        **new_set;
   char        **manager_set;
   int         p_length;
   char        **p_set;

   *modify_parts = 0;

   if (acl_mgr_set == NULL) {
      schema_entry->acl_mgr_set = malloc(SA_ACL_MGR_SET_SIZE(1));
      schema_entry->acl_mgr_set->num_acl_mgrs = 0;
   }
   else {
      build_acl_mgr_tables(result);
      if (BAD_STATUS(result)) {
         return;
      }

      /*
       * Determine if the acl_mgr_set information passed in
       * represents one set (a single list), or represents
       * more than one set (a list of lists).
       */
      is_list = isllist(acl_mgr_set);

      if (is_list == TRUE) {
         /*
          * We have multiple acl mgr sets to process.
          */
         aargv = (char **)malloc(sizeof(char *) * 2);
         aargv[0] = acl_mgr_set;
         aargv[1] = NULL;
         if (Tcl_SplitList(interp, aargv[0], &list_length, &manager_set) != TCL_OK) {
            *result = dcp_s_rgy_invalid_acl_mgr_syntax;
            free((char *)aargv);
            return;
         }
         free((char *)aargv);
      }
      else {
         /*
          * We have but one acl mgr set to process.
          */
         list_length = 1;
         manager_set = (char **)malloc(sizeof(char *) * 2);
         manager_set[0] = acl_mgr_set;
         manager_set[1] = NULL;
      }

      schema_entry->acl_mgr_set = malloc(sizeof(sec_attr_acl_mgr_info_set_t) +
         (list_length * sizeof(sec_attr_acl_mgr_info_p_t)));
      schema_entry->acl_mgr_set->num_acl_mgrs = list_length;

      for (count = 0; count < list_length; count++) {
         /*
          * Split the element of the list into its component parts.
          */
         if (Tcl_SplitList(interp, manager_set[count], &new_length, &new_set) != TCL_OK) {
            *result = dcp_s_rgy_invalid_acl_mgr_syntax;
            free((char *)manager_set);
            return;
         }

         if (new_length != 5) {
            /*
             * Not the right amount of elements, so the input
             * is not a correctly formatted acl mgr set.
             */
            *result = dcp_s_rgy_invalid_acl_mgr_syntax;
            free((char *)manager_set);
            free((char *)new_set);
            return;
         }

         /*
          * Verify the acl manager set name.  It can either be
          * a uuid or a string which matches known values.  If
          * the uuid check fails, assume it's a string.
          */
         uuid_by_name = FALSE;
         uuid_from_string((unsigned_char_t *)new_set[0], &temp_uuid, result);
         if (*result != error_status_ok) {
            uuid_by_name = TRUE;
            found = -1;
            hits = 0;
            for (i = 0; i < num_acl_mgr_uuid; i++) {
               if (strncmp(new_set[0], acl_mgr_name[i], strlen(new_set[0])) == 0) {
                  if (!strcmp(new_set[0], acl_mgr_name[i])) { 
                     hits = 1;
                     found = i;
                     break;
                  }
                  found = i;
                  hits++;
               }
            }
            if (hits > 1) {
               *result = dcp_s_rgy_ambig_acl_mgr_name;
               free((char *)manager_set);
               free((char *)new_set);
               return;
            }
            if (hits == 0) {
               *result = dcp_s_rgy_invalid_acl_mgr_name;
               free((char *)manager_set);
               free((char *)new_set);
               return;
            }
            i = found;
         }
         schema_entry->acl_mgr_set->mgr_info[count] = malloc(sizeof(sec_attr_acl_mgr_info_t));
         if (uuid_by_name == TRUE)
            schema_entry->acl_mgr_set->mgr_info[count]->acl_mgr_type = acl_mgr_uuid[i];
         else
            schema_entry->acl_mgr_set->mgr_info[count]->acl_mgr_type = temp_uuid;

         build_acl_perm_strings_table(&(SA_ACL_MGR_TYPE(schema_entry, count)), result);
         if (BAD_STATUS(result))
         {
            free((char *)manager_set);
            free((char *)new_set);
            return;
         }

         /*
          * There are now four permission strings to process.  They are either
          * a simple permission string, or a {name string} pair.  Loop through
          * the four members of new_set (1 through 4) and process each one.
          * They must be in the order query, update, test, delete.
          */
         for (j = 1; j < new_length; j++) {
            /*
             * Split the parameter.  The length tells which format
             * is being used.
             */
            if (Tcl_SplitList(interp, new_set[j], &p_length, &p_set) != TCL_OK) {
               *result = dcp_s_rgy_invalid_acl_mgr_syntax;
               free((char *)manager_set);
               free((char *)new_set);
               return;
            }
            if (p_length > 2) {
               *result = dcp_s_rgy_invalid_acl_mgr_syntax;
               free((char *)manager_set);
               free((char *)new_set);
               free((char *)p_set);
               return;
            }
            else if (p_length == 2) {
               /*
                * Make sure the correct permset name is specified.
                */
               if (strncmp(local_permset_name_table[j-1], p_set[0], strlen(p_set[0])) != 0) {
                  *result = dcp_s_rgy_invalid_acl_mgr_syntax;
                  free((char *)manager_set);
                  free((char *)new_set);
                  free((char *)p_set);
                  return;
               }
            }

            switch (j) {
               case 1:
                  schema_entry->acl_mgr_set->mgr_info[count]->query_permset = 0;
                  assign_permset(p_set[p_length-1],
                                 strlen(p_set[p_length-1]),
                                 &schema_entry->acl_mgr_set->mgr_info[count]->query_permset,
                                 result);
                  break;
               case 2:
                  schema_entry->acl_mgr_set->mgr_info[count]->update_permset = 0;
                  assign_permset(p_set[p_length-1],
                                 strlen(p_set[p_length-1]),
                                 &schema_entry->acl_mgr_set->mgr_info[count]->update_permset,
                                 result);
                  break;
               case 3:
                  schema_entry->acl_mgr_set->mgr_info[count]->test_permset = 0;
                  assign_permset(p_set[p_length-1],
                                 strlen(p_set[p_length-1]),
                                 &schema_entry->acl_mgr_set->mgr_info[count]->test_permset,
                                 result);
                  break;
               case 4:
                  schema_entry->acl_mgr_set->mgr_info[count]->delete_permset = 0;
                  assign_permset(p_set[p_length-1],
                                 strlen(p_set[p_length-1]),
                                 &schema_entry->acl_mgr_set->mgr_info[count]->delete_permset,
                                 result);
                  break;
            }
            free((char *)p_set);
            if (*result != error_status_ok) {
               free((char *)manager_set);
               free((char *)new_set);
               return;
            }
         }

         /*
          * Free the data storage structure.
          */
         free((char *)new_set);
      }
      free((char*)manager_set);
      free_acl_perm_strings_table();
      free_acl_mgr_tables();
      *modify_parts = (*modify_parts) | sec_attr_schema_part_acl_mgrs;
   }

   if (reserved == NULL) {
      SA_SCH_FLAG_UNSET_RESERVED(schema_entry);
   }
   else {
      yes_no = is_yes(reserved);
      if (yes_no == TRUE)
         SA_SCH_FLAG_SET_RESERVED(schema_entry);
      else if (yes_no == FALSE)
         SA_SCH_FLAG_UNSET_RESERVED(schema_entry);
      else {
         *result = yes_no;
         return;
      }
      *modify_parts = (*modify_parts) | sec_attr_schema_part_reserved;
   }

   if (apply_defaults == NULL) {
      SA_SCH_FLAG_UNSET_USE_DEFAULTS(schema_entry);
   }
   else {
      yes_no = is_yes(apply_defaults);
      if (yes_no == TRUE)
         SA_SCH_FLAG_SET_USE_DEFAULTS(schema_entry);
      else if (yes_no == FALSE)
         SA_SCH_FLAG_UNSET_USE_DEFAULTS(schema_entry);
      else {
         *result = yes_no;
         return;
      }
      *modify_parts = (*modify_parts) | sec_attr_schema_part_defaults;
   }

   if (intercell_action == NULL) {
      schema_entry->intercell_action = sec_attr_intercell_act_reject;
   }
   else {
      found = -1;
      hits = 0;
      for (i = 0; i < MAX_INTERCELL; i++) {
         if (strncmp(intercell_action, intercell_act_name[i], strlen(intercell_action)) == 0) {
            if (!strcmp(intercell_action, intercell_act_name[i])) {
               hits = 1;
               found = i;
               break;
            }
            found = i;
            hits++;
         }
      }
      if (hits > 1) {
         *result = dcp_s_rgy_ambig_intercell_action;
         return;
      }
      if (hits == 0) {
         /*
          * The value passed in for the intercell action field did not
          * match one of the legal values.  This is an error.
          */
         *result = dcp_s_rgy_invalid_intercell_action;
         return;
      }
      schema_entry->intercell_action = found;
      *modify_parts = (*modify_parts) | sec_attr_schema_part_intercell;
   }

   if (trig_binding == NULL) {
      schema_entry->trig_binding = NULL;
   }
   else {
      /*
       * Take advantage of the binding work done in the
       * function that gets attribute values from a list.
       * To do this, we have to send the trig binding string
       * via the bargv structure.
       */
      bargv = (char **)malloc(sizeof(char *) * 3);
      bargv[0] = NULL;
      bargv[1] = trig_binding;
      bargv[2] = NULL;
      uuid_create_nil(&temp_uuid, result);
      *result = get_attr_values(interp,
                                sec_attr_enc_binding,
                                temp_uuid,
                                &attr_info,
                                &num_entries,
                                bargv);
      if (*result != sec_rgy_status_ok) {
         free((char *)bargv);
         return;
      }
      free((char *)bargv);

      /*
       * If we get here, it means we have legal trig binding
       * information, stored in the attr_info structure.
       * Allocate enough space in schema_entry to hold all
       * the new information.
       *
       * First define a shorthand method of referencing
       * the stored attribute information.
       */
      attr_p = &attr_info;

      /*
       * Do the auth_info part.
       */
      SA_TRG_BND_INFO_P(schema_entry) = (sec_attr_bind_info_t *) malloc(
         sizeof(sec_attr_bind_info_t) + (SA_ATTR_BND_NUM(attr_p) * sizeof(sec_attr_binding_t)));
      SA_TRG_BND_AUTH_INFO_TYPE(schema_entry) = SA_ATTR_BND_AUTH_INFO_TYPE(attr_p);
      if (SA_TRG_BND_AUTH_INFO_TYPE(schema_entry) == sec_attr_bind_auth_dce) {
         SA_TRG_BND_AUTH_SVR_PNAME_P(schema_entry) = (unsigned_char_p_t) malloc(
            sizeof(unsigned_char_t) * (strlen((char *)SA_ATTR_BND_AUTH_SVR_PNAME_P(attr_p)) + 1));
         strcpy((char *)SA_TRG_BND_AUTH_SVR_PNAME_P(schema_entry),
                (char *)SA_ATTR_BND_AUTH_SVR_PNAME_P(attr_p));
         SA_TRG_BND_AUTH_PROT_LEVEL(schema_entry) = SA_ATTR_BND_AUTH_PROT_LEVEL(attr_p);
         SA_TRG_BND_AUTH_AUTHN_SVC(schema_entry) = SA_ATTR_BND_AUTH_AUTHN_SVC(attr_p);
         SA_TRG_BND_AUTH_AUTHZ_SVC(schema_entry) = SA_ATTR_BND_AUTH_AUTHZ_SVC(attr_p);
      }

      /*
       * Do the bindings part (including num_bindings).
       */
      SA_TRG_BND_NUM(schema_entry) = SA_ATTR_BND_NUM(attr_p);
      for (i = 0; i < SA_TRG_BND_NUM(schema_entry); i++) {
         /*
          * Do the binding type part.
          */
         SA_TRG_BND_TYPE(schema_entry, i) = SA_ATTR_BND_TYPE(attr_p, i);
         switch((int)SA_TRG_BND_TYPE(schema_entry, i)) {
            case sec_attr_bind_type_string:
               /*
                * Do the string binding part.
                */
               SA_TRG_BND_STRING_P(schema_entry, i) = (unsigned_char_p_t) malloc(
                  sizeof(unsigned_char_t) * (strlen((char *)SA_ATTR_BND_STRING_P(attr_p, i)) + 1));
               strcpy((char *)SA_TRG_BND_STRING_P(schema_entry, i),
                      (char *)SA_ATTR_BND_STRING_P(attr_p, i));
               break;

            case sec_attr_bind_type_svrname:
               /*
                * Do the svrname part.
                */
               SA_TRG_BND_SVRNAME_P(schema_entry,i) = (sec_attr_bind_svrname *) malloc(
                  sizeof(sec_attr_bind_svrname));

               /*
                * Do the name syntax part.
                */
               SA_TRG_BND_SVRNAME_SYNTAX(schema_entry, i) = SA_ATTR_BND_SVRNAME_SYNTAX(attr_p, i);

               /*
                * Do the name part.
                */
               SA_TRG_BND_SVRNAME_NAME_P(schema_entry, i) = (unsigned_char_p_t) malloc(
                  sizeof(char) * (strlen((char *)SA_ATTR_BND_SVRNAME_NAME_P(attr_p, i)) + 1));
               strcpy((char *)SA_TRG_BND_SVRNAME_NAME_P(schema_entry, i),
                      (char *)SA_ATTR_BND_SVRNAME_NAME_P(attr_p, i));
               break;

            case sec_attr_bind_type_twrs:
               SA_TRG_BND_TWRSET_P(schema_entry, i) = 
                  malloc(sizeof(sec_attr_twr_set_t) + 
                         (SA_ATTR_BND_TWRSET_COUNT(attr_p, i) - 1) * sizeof(sec_attr_twr_ref_t));
               SA_TRG_BND_TWRSET_COUNT(schema_entry, i) = SA_ATTR_BND_TWRSET_COUNT(attr_p, i);
               for (j = 0; j < SA_TRG_BND_TWRSET_COUNT(schema_entry, i); j++) {
                  SA_TRG_BND_TWR_P(schema_entry, i, j) = 
                     malloc(sizeof(twr_t) + (SA_ATTR_BND_TWR_LEN(attr_p, i, j) - 1) * sizeof(idl_byte));
                  SA_TRG_BND_TWR_LEN(schema_entry, i, j) = SA_ATTR_BND_TWR_LEN(attr_p, i, j);
                  memcpy(SA_TRG_BND_TWR_OCTETS(schema_entry, i, j), 
                         SA_ATTR_BND_TWR_OCTETS(attr_p, i, j), SA_ATTR_BND_TWR_LEN(attr_p, i, j));
               }
               break;

            default:
               /*
                * If the code gets here, it means that some illegal or
                * unknown binding type was discovered.  Set an error.
                */
               *result = dcp_s_rgy_era_unknown_binding_type;
               sec_attr_util_inst_free_ptrs(attr_p);
               return;
         }
      }
      sec_attr_util_inst_free_ptrs(attr_p);
      *modify_parts = (*modify_parts) | sec_attr_schema_part_trig_bind;
   }

   if (comment == NULL) {
      schema_entry->comment = NULL;
   }
   else {
      schema_entry->comment = malloc(sizeof(char) * (strlen(comment) + 1));
      strcpy((char *)schema_entry->comment, (char *)comment);
      *modify_parts = (*modify_parts) | sec_attr_schema_part_comment;
   }
}

/*
 * FUNCTION: build_proper_aclmgr_set
 *
 * OVERVIEW:
 *
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *
 * RETURNS:
 */
int build_proper_aclmgr_set(Tcl_Interp *interp, 
                            char *schema_name, 
                            char *schema_attr_name, 
                            char *full_sec_group_name, 
                            sec_attr_schema_entry_t *schema_entry)
{
   error_status_t               status;
   sec_attr_schema_entry_t      schema_entry_orig;
   int                          i, j;
   int                          orig_num_acl_mgr;
   int                          specified_final_num_acl_mgr;
   int                          specified_num_acl_mgr;
   int                          total_num_aclmgrs;
   boolean32                    *new_acl_mgr;
   sec_attr_acl_mgr_info_set_t  *new_acl_mgr_set;
   sec_attr_acl_mgr_info_set_t  *orig_acl_mgr_set;
   sec_attr_acl_mgr_info_set_t  *specified_acl_mgr_set;

   if (strcmp((char *)schema_name, full_sec_group_name) == 0) {
      /*
       * Do the schema lookup by name and retrieve the
       * values for all the schema attribute fields.
       */
      sec_rgy_attr_sch_lookup_by_name(rgy_context,
                                      (sec_attr_component_name_t)schema_name,
                                      (idl_char *)schema_attr_name,
                                      &schema_entry_orig,
                                      &status);
      if (BAD_STATUS(&status)) {
         if (status == sec_attr_sch_entry_not_found) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_sch_entry_not_found, (char *)schema_attr_name);
            return TCL_ERROR;
         }
         else {
            DCP_SET_MISC_ERROR(status);
         }
      }
   }
   else {
      set_attr_schema_binding((dce_attr_component_name_t) schema_name, &status);
      DCP_CHECK_SEC_ERROR(status);
      dce_attr_sch_lookup_by_name(attr_schema_binding_handle,
                                  (idl_char *)schema_attr_name,
                                  (dce_attr_schema_entry_t *) &schema_entry_orig,
                                  &status);
      if (BAD_STATUS(&status)) {
         if (status == db_s_key_not_found) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_sch_entry_not_found, (char *)schema_attr_name);
            return TCL_ERROR;
         }
         else {
            DCP_SET_MISC_ERROR(status);
         }
      }
   }

   orig_num_acl_mgr = SA_ACL_MGR_NUM(&schema_entry_orig);
   specified_num_acl_mgr = SA_ACL_MGR_NUM(schema_entry);
   specified_final_num_acl_mgr = specified_num_acl_mgr;

   new_acl_mgr = (boolean32 *)malloc(sizeof(boolean32) * specified_num_acl_mgr); 
   if (new_acl_mgr == NULL) {
      SET_STATUS(&status, dcp_s_no_memory);
      return status;
   }

   for (i = 0; i < orig_num_acl_mgr; i++) {
     for (j = 0; j < specified_num_acl_mgr; j++) {
       if (!uuid_compare(&(SA_ACL_MGR_TYPE(&schema_entry_orig, i)),
			 &(SA_ACL_MGR_TYPE(schema_entry, j)), &status)) {
	 /* duplicate acl_mgr_type */
	 new_acl_mgr[j] = false;
	 specified_final_num_acl_mgr--;
	 /* replace old mgr_info with the new one */
	 *(schema_entry_orig.acl_mgr_set->mgr_info[i]) = *(schema_entry->acl_mgr_set->mgr_info[j]);
       }
       else {
	 new_acl_mgr[j] = true;
       }
     }
   }
   
   total_num_aclmgrs = orig_num_acl_mgr + specified_final_num_acl_mgr;
   orig_acl_mgr_set = schema_entry_orig.acl_mgr_set;
   specified_acl_mgr_set = schema_entry->acl_mgr_set;
   new_acl_mgr_set = malloc(SA_ACL_MGR_SET_SIZE(total_num_aclmgrs));
   if (new_acl_mgr_set == NULL) {
      SET_STATUS(&status, dcp_s_no_memory);
      return status;
   }

   new_acl_mgr_set->num_acl_mgrs = total_num_aclmgrs;
   for (i = 0; i < orig_num_acl_mgr; i++) {
      if ((new_acl_mgr_set->mgr_info[i] = 
	   malloc(sizeof(sec_attr_acl_mgr_info_t))) == NULL) {
	SET_STATUS(&status, dcp_s_no_memory);
	return status;
      }
      *(new_acl_mgr_set->mgr_info[i]) = *(orig_acl_mgr_set->mgr_info[i]);
   }

   for (i = 0; i < specified_num_acl_mgr; i++) {
     if (new_acl_mgr[i]) {
       if ((new_acl_mgr_set->mgr_info[i+orig_num_acl_mgr] 
	   = malloc(sizeof(sec_attr_acl_mgr_info_t))) == NULL) {
	SET_STATUS(&status, dcp_s_no_memory);
	return status;
      }
       *(new_acl_mgr_set->mgr_info[i+orig_num_acl_mgr]) = *(specified_acl_mgr_set->mgr_info[i]);
     }
   }
   free(new_acl_mgr);
   schema_entry->acl_mgr_set = new_acl_mgr_set;
   sec_attr_util_sch_ent_free_ptrs(&schema_entry_orig);
   return TCL_OK;
}

/*
 * FUNCTION: encode_modify_schema_attr
 *
 * OVERVIEW: This decodes ascii schema attribute information and
 *      inserts the data into a sec_attr_schema_entry_t structure.
 *
 * INPUTS:
 *  Tcl_Interp *interp - the tcl interpreter structure for this invocation.
 *  char **attr_info - the attribute list, contains the attribute name in the
 *                     first position, then one or more values for that attribute
 *                     in the succeeding positions.
 *  int num_val - the number of attributes in the above list; note this is
 *                not the number of values, rather the number of values plus
 *                the attribute name.
 *
 * INPUT/OUTPUTS:
 *  sec_attr_schema_entry_t *schema_entry - holds the schema values for any
 *                                          schema attributes specified.
 *
 * OUTPUTS:
 *  unsigned32 *status - sends back to the calling function the status of this
 *                       function.  No problem is returned as sec_rgy_status_ok.
 *                       Other error status codes are possible.
 *  sec_attr_schema_entry_parts_t *modify_parts - flag that indicates which fields
 *                                                are to be modified.
 *
 * RETURNS:
 *  void
 */
void encode_modify_schema_attr(Tcl_Interp *interp,
                               char **attr_info,
                               int *num_val, 
                               sec_attr_schema_entry_t *schema_entry, 
                               sec_attr_schema_entry_parts_t *modify_parts,
                               unsigned32 *status)
{
   int         i, count;
   int         j;
   int         found;
   int         hits;
   int         yes_no;
   int         new_length;
   int         list_length;
   boolean32   is_list = FALSE;
   boolean32   uuid_by_name;
   unsigned32  result;
   char        *attr_name;
   char        **aargv, **bargv;
   uuid_t      temp_uuid;
   int         num_entries = 1;
   sec_attr_t  *attr_p;
   sec_attr_t  attr_after_proc;
   char        **new_set;
   char        **manager_set;
   int         p_length;
   char        **p_set;

   /*
    * The attribute name is the first item in the attr_info structure.
    * The next and succeeding items in the attr structure are
    * values associated with that attribute.
    */
   attr_name = attr_info[0];

   /*
    * Process the attribute values based on the attribute
    * name passed in.  If the attribute name is not one of
    * the currently supported attributes associated with a
    * schema, then it is by definition illegal.  These bad
    * attributes will be processed by the 'default' case.
    */
   switch(schema_attribute(attr_name)) {
      /*
       * Process an acl manager set.
       */
      case schema_aclmgr:
         build_acl_mgr_tables(status);
         if (BAD_STATUS(status)) {
            return;
         }

         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         /*
          * Determine if the acl_mgr_set information passed in
          * represents one set (a single list), or represents
          * more than one set (a list of lists).
          */
         is_list = isllist(attr_info[1]);

         if (is_list == TRUE) {
            /*
             * We have multiple acl mgr sets to process.
             */
            aargv = (char **)malloc(sizeof(char *) * 2);
            aargv[0] = attr_info[1];
            aargv[1] = NULL;
            if (Tcl_SplitList(interp, aargv[0], &list_length, &manager_set) != TCL_OK) {
               *status = dcp_s_rgy_invalid_acl_mgr_syntax;
               free((char *)aargv);
               return;
            }
            free((char *)aargv);
         }
         else {
            /*
             * We have but one acl mgr set to process.
             */
            list_length = 1;
            manager_set = (char **)malloc(sizeof(char *) * 2);
            manager_set[0] = attr_info[1];
            manager_set[1] = NULL;
         }

         schema_entry->acl_mgr_set = malloc(sizeof(sec_attr_acl_mgr_info_set_t) +
            (list_length * sizeof(sec_attr_acl_mgr_info_p_t)));
         schema_entry->acl_mgr_set->num_acl_mgrs = list_length;

         for (count = 0; count < list_length; count++) {
            /*
             * Split the element of the list into its component parts.
             */
            if (Tcl_SplitList(interp, manager_set[count], &new_length, &new_set) != TCL_OK) {
               *status = dcp_s_rgy_invalid_acl_mgr_syntax;
               free((char *)manager_set);
               return;
            }

            if (new_length != 5) {
               /*
                * Not the right amount of elements, so the input
                * is not a correctly formatted acl mgr set.
                */
               *status = dcp_s_rgy_invalid_acl_mgr_syntax;
               free((char *)manager_set);
               free((char *)new_set);
               return;
            }

            /*
             * Verify the acl manager set name.  It can either be
             * a uuid or a string which matches known values.  If
             * the uuid check fails, assume it's a string.
             */
            uuid_by_name = FALSE;
            uuid_from_string((unsigned_char_t *)new_set[0], &temp_uuid, &result);
            if (result != error_status_ok) {
               uuid_by_name = TRUE;
               found = -1;
               hits = 0;
               for (i = 0; i < num_acl_mgr_uuid; i++) {
                  if (strncmp(new_set[0], acl_mgr_name[i], strlen(new_set[0])) == 0) {
                     if (!strcmp(new_set[0], acl_mgr_name[i])) {
                        hits = 1;
                        found = i;
                        break;
                     }
                     found = i;
                     hits++;
                  }
               }
               if (hits > 1) { 
                  *status = dcp_s_rgy_ambig_acl_mgr_name;
                  free((char *)manager_set);
                  free((char *)new_set);
                  return;
               }
               if (hits == 0) {
                  *status = dcp_s_rgy_invalid_acl_mgr_name;
                  free((char *)manager_set);
                  free((char *)new_set);
                  return;
               }
               i = found;
            }
            schema_entry->acl_mgr_set->mgr_info[count] = malloc(sizeof(sec_attr_acl_mgr_info_t));
            if (uuid_by_name == TRUE)
               schema_entry->acl_mgr_set->mgr_info[count]->acl_mgr_type = acl_mgr_uuid[i];
            else
               schema_entry->acl_mgr_set->mgr_info[count]->acl_mgr_type = temp_uuid;

            build_acl_perm_strings_table(&(SA_ACL_MGR_TYPE(schema_entry, count)), status);
            if (BAD_STATUS(status)) {
               free((char *)manager_set);
               free((char *)new_set);
               return;
            }

            /*
             * There are now four permission strings to process.  They are either
             * a simple permission string, or a {name string} pair.  Loop through
             * the four members of new_set (1 through 4) and process each one.
             * They must be in the order query, update, test, delete.
             */
            for (j = 1; j < new_length; j++) {
               /*
                * Split the parameter.  The length tells which format
                * is being used.
                */
               if (Tcl_SplitList(interp, new_set[j], &p_length, &p_set) != TCL_OK) {
                  *status = dcp_s_rgy_invalid_acl_mgr_syntax;
                  free((char *)manager_set);
                  free((char *)new_set);
                  return;
               }
               if (p_length > 2) {
                  *status = dcp_s_rgy_invalid_acl_mgr_syntax;
                  free((char *)manager_set);
                  free((char *)new_set);
                  free((char *)p_set);
                  return;
               }
               else if (p_length == 2) {
                  /*
                   * Make sure the correct permset name is specified.
                   */
                  if (strncmp(local_permset_name_table[j-1], p_set[0], strlen(p_set[0])) != 0) {
                     *status = dcp_s_rgy_invalid_acl_mgr_syntax;
                     free((char *)manager_set);
                     free((char *)new_set);
                     free((char *)p_set);
                     return;
                  }
               }

               switch (j) {
                  case 1:
                     schema_entry->acl_mgr_set->mgr_info[count]->query_permset = 0;
                     assign_permset(p_set[p_length-1],
                                    strlen(p_set[p_length-1]),
                                    &schema_entry->acl_mgr_set->mgr_info[count]->query_permset,
                                    status);
                     break;
                  case 2:
                     schema_entry->acl_mgr_set->mgr_info[count]->update_permset = 0;
                     assign_permset(p_set[p_length-1],
                                    strlen(p_set[p_length-1]),
                                    &schema_entry->acl_mgr_set->mgr_info[count]->update_permset,
                                    status);
                     break;
                  case 3:
                     schema_entry->acl_mgr_set->mgr_info[count]->test_permset = 0;
                     assign_permset(p_set[p_length-1],
                                    strlen(p_set[p_length-1]),
                                    &schema_entry->acl_mgr_set->mgr_info[count]->test_permset,
                                    status);
                     break;
                  case 4:
                     schema_entry->acl_mgr_set->mgr_info[count]->delete_permset = 0;
                     assign_permset(p_set[p_length-1],
                                    strlen(p_set[p_length-1]),
                                    &schema_entry->acl_mgr_set->mgr_info[count]->delete_permset,
                                    status);
                     break;
               }
               free((char *)p_set);
               if (*status != error_status_ok) {
                  free((char *)manager_set);
                  free((char *)new_set);
                  return;
               }
            }

            /*
             * Free the data storage structure.
             */
            free((char *)new_set);
         }
         free((char *)manager_set);
         free_acl_perm_strings_table();
         free_acl_mgr_tables();
         *modify_parts = (*modify_parts) | sec_attr_schema_part_acl_mgrs;
         break;

      /*
       * Process the reserved flag.
       */
      case schema_reserved:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         yes_no = is_yes(attr_info[1]);
         if (yes_no == TRUE)
            SA_SCH_FLAG_SET_RESERVED(schema_entry);
         else if (yes_no == FALSE)
            SA_SCH_FLAG_UNSET_RESERVED(schema_entry);
         else {
            *status = yes_no;
            return;
         }
         *modify_parts = (*modify_parts) | sec_attr_schema_part_reserved;
         break;

      /*
       * Process the apply_defaults flag.
       */
      case schema_applydefs:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         yes_no = is_yes(attr_info[1]);
         if (yes_no == TRUE)
            SA_SCH_FLAG_SET_USE_DEFAULTS(schema_entry);
         else if (yes_no == FALSE)
            SA_SCH_FLAG_UNSET_USE_DEFAULTS(schema_entry);
         else {
            *status = yes_no;
            return;
         }
         *modify_parts = (*modify_parts) |sec_attr_schema_part_defaults;
         break;

      /*
       * Process an intercell_action type.
       */
      case schema_intercell:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         found = -1;
         hits = 0;
         for (i = 0; i < MAX_INTERCELL; i++) {
            if (strncmp(attr_info[1], intercell_act_name[i], strlen(attr_info[1])) == 0) {
               if (!strcmp(attr_info[1], intercell_act_name[i])) {
                  hits = 1;
                  found = i;
                  break;
               }
               found = i;
               hits++;
            }
         }
         if (hits > 1) {
            *status = dcp_s_rgy_ambig_intercell_action;
            return;
         }
         if (hits == 0) {
            *status = dcp_s_rgy_invalid_intercell_action;
            return;
         }
         schema_entry->intercell_action = found;
         *modify_parts = (*modify_parts) | sec_attr_schema_part_intercell;
         break;

      /*
       * Process trigger bindings.
       */
      case schema_trigbind:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         /*
          * Take advantage of the binding work done in the
          * function that gets attribute values from a list.
          */
         bargv = (char **)malloc(sizeof(char *) * 3);
         bargv[0] = NULL;
         bargv[1] = attr_info[1];
         bargv[2] = NULL;
         uuid_create_nil(&temp_uuid, status);
         if (*status != error_status_ok) {
            free((char *)bargv);
            return;
         }
         *status = get_attr_values(interp,
                                   sec_attr_enc_binding,
                                   temp_uuid,
                                   &attr_after_proc,
                                   &num_entries,
                                   bargv);
         if (*status != sec_rgy_status_ok) {
            free((char *)bargv);
            return;
         }
         free((char *)bargv);

         /*
          * If we get here, it means we have legal trig binding
          * information, stored in the attr_info structure.
          * Allocate enough space in schema_entry to hold all
          * the new information.
          *
          * First define a shorthand method of referencing
          * the stored attribute information.
          */
         attr_p = &attr_after_proc;

         /*
          * Do the auth_info part.
          */
         SA_TRG_BND_INFO_P(schema_entry) = (sec_attr_bind_info_t *) malloc(
            sizeof(sec_attr_bind_info_t) + (SA_ATTR_BND_NUM(attr_p) * sizeof(sec_attr_binding_t)));
         SA_TRG_BND_AUTH_INFO_TYPE(schema_entry) = SA_ATTR_BND_AUTH_INFO_TYPE(attr_p);
         if (SA_TRG_BND_AUTH_INFO_TYPE(schema_entry) == sec_attr_bind_auth_dce) {
            SA_TRG_BND_AUTH_SVR_PNAME_P(schema_entry) = (unsigned_char_p_t) malloc(
               sizeof(unsigned_char_t) * (strlen((char *)SA_ATTR_BND_AUTH_SVR_PNAME_P(attr_p)) + 1));
            strcpy((char *)SA_TRG_BND_AUTH_SVR_PNAME_P(schema_entry),
                   (char *)SA_ATTR_BND_AUTH_SVR_PNAME_P(attr_p));
            SA_TRG_BND_AUTH_PROT_LEVEL(schema_entry) = SA_ATTR_BND_AUTH_PROT_LEVEL(attr_p);
            SA_TRG_BND_AUTH_AUTHN_SVC(schema_entry) = SA_ATTR_BND_AUTH_AUTHN_SVC(attr_p);
            SA_TRG_BND_AUTH_AUTHZ_SVC(schema_entry) = SA_ATTR_BND_AUTH_AUTHZ_SVC(attr_p);
         }

         /*
          * Do the bindings part (including num_bindings).
          */
         SA_TRG_BND_NUM(schema_entry) = SA_ATTR_BND_NUM(attr_p);
         for (i = 0; i < SA_TRG_BND_NUM(schema_entry); i++) {
            /*
             * Do the binding type part.
             */
            SA_TRG_BND_TYPE(schema_entry, i) = SA_ATTR_BND_TYPE(attr_p, i);
            switch((int)SA_TRG_BND_TYPE(schema_entry, i)) {
               case sec_attr_bind_type_string:
                  /*
                   * Do the string binding part.
                   */
                  SA_TRG_BND_STRING_P(schema_entry, i) = (unsigned_char_p_t) malloc(
                     sizeof(unsigned_char_t) * (strlen((char *)SA_ATTR_BND_STRING_P(attr_p, i)) + 1));
                  strcpy((char *)SA_TRG_BND_STRING_P(schema_entry, i),
                         (char *)SA_ATTR_BND_STRING_P(attr_p, i));
                  break;

               case sec_attr_bind_type_svrname:
                  /*
                   * Do the svrname part.
                   */
                  SA_TRG_BND_SVRNAME_P(schema_entry,i) = (sec_attr_bind_svrname *) malloc(
                     sizeof(sec_attr_bind_svrname));

                  /*
                   * Do the name syntax part.
                   */
                  SA_TRG_BND_SVRNAME_SYNTAX(schema_entry, i) = SA_ATTR_BND_SVRNAME_SYNTAX(attr_p, i);

                  /*
                   * Do the name part.
                   */
                  SA_TRG_BND_SVRNAME_NAME_P(schema_entry, i) = (unsigned_char_p_t) malloc(
                     sizeof(char) * (strlen((char *)SA_ATTR_BND_SVRNAME_NAME_P(attr_p, i)) + 1));
                  strcpy((char *)SA_TRG_BND_SVRNAME_NAME_P(schema_entry, i),
                         (char *)SA_ATTR_BND_SVRNAME_NAME_P(attr_p, i));
                  break;

               case sec_attr_bind_type_twrs:
                  SA_TRG_BND_TWRSET_P(schema_entry, i) = 
                     malloc(sizeof(sec_attr_twr_set_t) + 
                            (SA_ATTR_BND_TWRSET_COUNT(attr_p, i) - 1) * sizeof(sec_attr_twr_ref_t));
                  SA_TRG_BND_TWRSET_COUNT(schema_entry, i) = SA_ATTR_BND_TWRSET_COUNT(attr_p, i);
                  for (j = 0; j < SA_TRG_BND_TWRSET_COUNT(schema_entry, i); j++) {
                     SA_TRG_BND_TWR_P(schema_entry, i, j) = 
                        malloc(sizeof(twr_t) + (SA_ATTR_BND_TWR_LEN(attr_p, i, j) - 1) * sizeof(idl_byte));
                     SA_TRG_BND_TWR_LEN(schema_entry, i, j) = SA_ATTR_BND_TWR_LEN(attr_p, i, j);
                     memcpy(SA_TRG_BND_TWR_OCTETS(schema_entry, i, j), 
                            SA_ATTR_BND_TWR_OCTETS(attr_p, i, j), SA_ATTR_BND_TWR_LEN(attr_p, i, j));
                  }
                  break;

               default:
                  /*
                   * If the code gets here, it means that some illegal or
                   * unknown binding type was discovered.  Set an error.
                   */
                  *status = dcp_s_rgy_era_unknown_binding_type;
                  sec_attr_util_inst_free_ptrs(attr_p);
                  return;
            }
         }

         *modify_parts = (*modify_parts) | sec_attr_schema_part_trig_bind;
         /*
          * Free the stuff allocated in the sec_attr_t structure.
          */
         sec_attr_util_inst_free_ptrs(attr_p);
         break;

      /*
       * Process a comment string.
       */
      case schema_annotation:
         if (attr_info[1] == NULL) {
            *status = dcp_s_rgy_bad_attribute_list;
            return;
         }
         schema_entry->comment = malloc(sizeof(char) * (strlen(attr_info[1]) + 1));
         strcpy((char *)schema_entry->comment, (char *)attr_info[1]);
         *modify_parts = (*modify_parts) | sec_attr_schema_part_comment;
         break;

      /*
       * Process an unknown attribute name.
       */
      default:
         *status = dcp_s_rgy_bad_attribute_name;
         return;
   }

   *status = sec_rgy_status_ok;
   return;
}

/*
 * FUNCTION: parse_modify_schema_attr_list
 *
 * OVERVIEW: This encodes ascii values into a schema structure according
 *     to the list of data.  That is, the input string is parsed into
 *     is component parts and processed individually.
 *
 * INPUTS:
 *  char *attr_string - The character string to parse.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp, - interpreter to store information
 *  sec_attr_schema_entry_t *schema_entry - structure to be filled with encoded values.
 *
 * OUTPUTS:
 *   unsigned32 *result - error/success code.
 *   sec_attr_schema_entry_parts_t *modify_parts - flag that indicates which fields
 *                                                 are to be modified.
 *
 * RETURNS:
 *  void
 */
void parse_modify_schema_attr_list(Tcl_Interp *interp, char *attr_string,
                                   sec_attr_schema_entry_t *schema_entry, 
                                   sec_attr_schema_entry_parts_t *modify_parts, 
                                   unsigned32 *result)
{
   int        i;
   boolean32  is_list = FALSE;
   char       **aargv, **pargv;
   int        aargc = 0;
   int        pargc = 0;

   if (attr_string != NULL) {
      /*
       * Check if the input string represents a
       * single attribute-value pair, or is a list
       * of attribute-value pairs.
       */
      is_list = isllist(attr_string);
      if (is_list) {
         /*
          * The input is a list of attribute-value pairs.
          */
         if (Tcl_SplitList(interp, attr_string, &pargc, &pargv) != TCL_OK) {
            *result = TCL_ERROR;
            return;
         }
      }
      else {
         /*
          * The input is a single attribute-value pair.
          */
         pargc = 1;
         pargv = (char **)malloc(2*sizeof(char *));
         if (pargv == NULL) {
            *result = dcp_s_no_memory;
            return;
         }
         pargv[0] = attr_string;
         pargv[1] = NULL;
      }

      /*
       * Process each attribute-value pair separately.
       */
      for (i = 0; i < pargc; i++) {
         if (Tcl_SplitList(interp, pargv[i], &aargc, &aargv) != TCL_OK) {
            *result = TCL_ERROR;
            free((char *)pargv);
            return;
         }
         /*
          * Take the aargv structure and pass it along to the
          * encoding function, which will do the work.
          */
         encode_modify_schema_attr(interp, aargv, &aargc, schema_entry, modify_parts, result);
         if (*result != error_status_ok) {
            free((char *)pargv);
            free((char *)aargv);
            return;
         }
         free((char *)aargv);
      }
      free((char *)pargv);
   }
   *result = TCL_OK;
   return;
}

/*
 * FUNCTION:
 * dcp_schema_read_group_name()
 *
 * OVERVIEW:
 * This routine returns the cell profile's security group name. This
 * will allow xattrschema operations on the local cell as well as a
 * remote cell. In addition, we can't assume a security group name
 * of /.:/sec.
 * The caller is responsible for calling the rpc_string_free() routine
 * to free the allocated memory for the security group name.
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command.  This
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * rs_ns_sec_group_name
 *           Cell profile security group name.
 *           For example: /<cell name>/sec
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_schema_read_group_name(Tcl_Interp *interp,
                           unsigned_char_p_t *rs_ns_sec_group_name)
{
   unsigned_char_p_t    rs_ns_sec_profile_name = NULL;
   rpc_if_id_t          rs_bind_if_id;
   rpc_ns_inq_handle_t  profile_context;
   unsigned32           priority;
   error_status_t       status;
   error_status_t       tmp_status;

   CLEAR_STATUS(&status);
   CLEAR_STATUS(&tmp_status);

   rs_ns_sec_profile_name = (unsigned_char_t *)malloc(
       sizeof(char) * (strlen((char *)site_cache[realtime_cache].cell_name)
       + CELL_PROFILE_LEN + 2));
   if (rs_ns_sec_profile_name == NULL) {
      DCP_SET_MISC_ERROR(dcp_s_no_memory);
   }
   sprintf((char *)rs_ns_sec_profile_name, 
           "%s/%s", 
           site_cache[realtime_cache].cell_name, 
           (char *)CELL_PROFILE_NAME);

   /*
    * Try to get the security group name associated with the
    * rs_bind interface for a client.  
    */
   rpc_if_inq_id(rs_bind_v2_0_c_ifspec,
                 &rs_bind_if_id, 
                 &status);
   if (BAD_STATUS(&status)) {
      free((char *)rs_ns_sec_profile_name);
      DCP_SET_MISC_ERROR(status);
   }

   /* 
    * Open the Cell Profile and attempt to retrieve the catalog point
    * (general security service name) by matching the interface uuid.
    * The catalog point for the security service must have been added
    * to the Cell Profile prior to this point (ie: at Big Bang).
    */
   rpc_ns_profile_elt_inq_begin(rpc_c_ns_syntax_dce,
                                rs_ns_sec_profile_name,
                                rpc_c_profile_match_by_if,
                                &rs_bind_if_id,
                                rpc_c_vers_exact,
                                rpc_c_ns_syntax_dce,
                                NULL,
                                &profile_context,
                                &status);

   if (GOOD_STATUS(&status)) {
      rpc_ns_profile_elt_inq_next(profile_context,
                                  &rs_bind_if_id,
                                  rs_ns_sec_group_name,
                                  &priority,
                                  NULL,
                                  &status);
      rpc_ns_profile_elt_inq_done(&profile_context, &tmp_status);
   }

   /*
    * Release allocated memory
    */
   free((char *)rs_ns_sec_profile_name);

   DCP_CHECK_SEC_ERROR(status);
   return TCL_OK;
}
