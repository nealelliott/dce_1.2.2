/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdscp.c,v $
 * Revision 1.1.18.1  1996/08/09  11:44:58  arvind
 * 	OT13485: Have 'directory list' authenticate correctly.
 * 	[1996/02/18  19:12:15  marty  1.1.16.3]
 *
 * Revision 1.1.16.3  1996/02/18  19:12:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:28  marty]
 * 
 * Revision 1.1.16.2  1995/12/08  21:19:39  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/15  1995/10/10  15:15 UTC  truitt
 * 	Merge cds changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts49/1  1995/10/05  14:58 UTC  truitt
 * 	CHFts16537: Split 'cdsmisc' command functionality into existing 'directory'
 * 	command and new 'cds' and 'cdsclient' commands.
 * 
 * 	HP revision /main/HPDCE02/14  1995/06/16  18:36 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts35/1  1995/06/16  18:35 UTC  truitt
 * 	CHFts15504: Correct build warnings in enq() and free_list().
 * 
 * 	HP revision /main/HPDCE02/13  1995/04/07  16:58 UTC  truitt
 * 	Merge cdsmisc memory work.
 * 
 * 	HP revision /main/HPDCE02/truitt_mothra_chfts25/1  1995/04/07  16:57 UTC  truitt
 * 	CHFts14803: cdsmisc memory leaks
 * 
 * 	HP revision /main/HPDCE02/12  1995/04/06  14:08 UTC  jrr
 * 	Check CDS names for valid size on input (CHFts14888).
 * 	[1995/04/05  21:33 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14888/1]
 * 
 * 	HP revision /main/HPDCE02/11  1995/04/06  13:54 UTC  jrr
 * 	Fix various memory leaks (CHFts14834).
 * 	[1995/04/05  14:30 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14834/1]
 * 
 * 	HP revision /main/HPDCE02/10  1995/03/27  17:26 UTC  jrr
 * 	Check for NULL return from cdsGetHandle() (CHFts14682).
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_cr12550/1  1995/03/27  16:07 UTC  jrr
 * 	Check for NULL return from cdsGetHandle().
 * 
 * 	HP revision /main/HPDCE02/9  1995/03/24  20:07 UTC  truitt
 * 	Merge dir list work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts21/1  1995/03/24  20:06 UTC  truitt
 * 	CHFts14345: When dcecp is built debug, the call to cdsEnumObj()
 * 	is causing a bus error when control returned back to Tcl_Eval().
 * 	The call was changed to use opaque mode instead of string mode,
 * 	and the proper parameters were used.
 * 
 * 	HP revision /main/HPDCE02/8  1995/02/24  15:37 UTC  truitt
 * 	Merge directory work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts15/1  1995/02/24  15:36 UTC  truitt
 * 	CHFts14284: Allow the directory delete command the
 * 	ability to delete a directory even if it has
 * 	replicas.
 * 
 * 	HP revision /main/HPDCE02/7  1995/02/14  15:45 UTC  truitt
 * 	 Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  15:44 UTC  truitt
 * 	CHFts14285: Combine all cds check args macros into one,
 * 	since all six are the same anyway.  Also, make sure correct
 * 	macros are called when processing an error condition.
 * 
 * 	HP revision /main/HPDCE02/6  1995/01/20  19:38 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts03/1  1995/01/20  19:36 UTC  truitt
 * 	CHFts14122: Complete work for using confidence level.
 * 
 * 	HP revision /main/HPDCE02/5  1995/01/19  20:34 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts02/1  1995/01/19  20:33 UTC  truitt
 * 	CHFts14121: Finish the cdsmisc set directory command.
 * 
 * 	HP revision /main/HPDCE02/4  1995/01/13  18:18 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chk_07/1  1995/01/13  18:15 UTC  truitt
 * 	Add new commands to cover some cdscp functionality
 * 	that is currently not present.
 * 
 * 	HP revision /main/HPDCE02/3  1994/12/05  17:47 UTC  truitt
 * 	OT12710: Remove useless macro code.
 * 
 * 	HP revision /main/HPDCE02/2  1994/11/03  19:49 UTC  truitt
 * 	Fixed directory merge command switch.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_01/1  1994/11/03  19:44 UTC  truitt
 * 	Fix directory merge command switches.
 * 
 * 	HP revision /main/HPDCE02/1  1994/10/20  17:49 UTC  truitt
 * 	Initial merge.
 * 
 * 	HP revision /main/truitt_era_dcecp/1  1994/10/20  17:31 UTC  truitt
 * 	Clean up directory_list() function.
 * 	[1995/12/08  17:54:18  root]
 * 
 * Revision 1.1.5.23  1994/09/30  18:46:16  coren
 * 	Changed order of messages, and removed extra newlines, when adding
 * 	an attribute fails on "driectory create" (CR 11539).
 * 	[1994/09/30  16:37:07  coren]
 * 
 * Revision 1.1.5.22  1994/09/02  13:36:45  kevins
 * 	OT 12002 modified dir add argument checking
 * 	[1994/09/02  13:35:48  kevins]
 * 
 * Revision 1.1.5.21  1994/08/19  21:16:03  coren
 * 	Try to delete directory if adding attribute fails in "dir create -attr"
 * 	(CR 11539).
 * 	[1994/08/19  19:54:56  coren]
 * 
 * Revision 1.1.5.20  1994/07/18  20:15:50  kevins
 * 	CR 10541 dir add/remove now accept tcl lists as arguments.
 * 	CR 11330 dir show -member now implemented.
 * 	[1994/07/18  20:08:28  kevins]
 * 
 * Revision 1.1.5.19  1994/06/09  16:01:36  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:46:59  devsrc]
 * 
 * Revision 1.1.5.18  1994/05/20  22:13:30  kevins
 * 	OT 10714 Modified attribute parsing return values.
 * 	[1994/05/20  22:11:17  kevins]
 * 
 * Revision 1.1.5.17  1994/05/12  19:32:07  kevins
 * 	OT 10625 fix the initialized _s(cds) value
 * 	[1994/05/12  19:30:56  kevins]
 * 
 * Revision 1.1.5.16  1994/05/11  19:27:02  kevins
 * 	OT 10097 Modified argument checknig routines
 * 	[1994/05/11  19:25:21  kevins]
 * 
 * 	OT 10097 modified arg checking
 * 	[1994/05/11  16:12:16  kevins]
 * 
 * Revision 1.1.5.15  1994/05/11  15:11:21  kevins
 * 	OT 10197 Added _s(cds) support
 * 	[1994/05/11  15:08:45  kevins]
 * 
 * Revision 1.1.5.14  1994/05/09  14:53:39  coren
 * 	Make directory_merge expand cell names before calling script.
 * 	[1994/05/06  19:51:53  coren]
 * 
 * 	Added use of dcp_s_bad_arg_list.
 * 	[1994/05/03  13:57:32  coren]
 * 
 * 	     Added directory_merge function.
 * 	[1994/04/20  15:35:01  coren]
 * 
 * Revision 1.1.5.13  1994/05/06  12:29:08  kevins
 * 	OT 10195 correct path representation of dir list output
 * 	[1994/05/06  12:28:57  kevins]
 * 
 * Revision 1.1.5.12  1994/05/03  13:48:52  kevins
 * 	OT# 10311 10360 10407 10414 10500 10499
 * 	Remove unnecessary cds include files.
 * 	Update modify switches to work correctly according to the RFC.
 * 	Fix inconsistent error messages.
 * 	Set the attribute types appropriately.
 * 	Correct the display format to correctly display tcl lists.
 * 	[1994/05/03  12:49:33  kevins]
 * 
 * Revision 1.1.5.11  1994/04/11  18:58:33  kevins
 * 	CR 10325 Mod dir sync command to accept tcl list arguments
 * 	[1994/04/11  18:58:11  kevins]
 * 
 * Revision 1.1.5.10  1994/04/07  10:22:07  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	CR 10304 Modifications to help remove cross
 * 	                 component dependency build issues
 * 	     [1994/04/07  10:17:48  kevins]
 * 
 * Revision 1.1.5.9  1994/04/05  20:48:21  coren
 * 	Adjusted error reporting from "delete -tree".
 * 	[1994/04/05  19:02:08  coren]
 * 
 * Revision 1.1.7.4  1994/04/01  20:50:26  coren
 * 	Added code for "dir delete -tree".
 * 
 * Revision 1.1.7.3  1994/03/22  20:05:33  coren
 * 	Merging yet again.
 * 
 * Revision 1.1.7.2  1994/03/14  20:15:24  coren
 * 	Merging up.
 * 
 * Revision 1.1.5.8  1994/03/18  14:31:18  kevins
 * 	CR10169 modified switch names for dir list command.
 * 	[1994/03/18  14:31:08  kevins]
 * 
 * Revision 1.1.5.7  1994/03/14  17:39:32  kevins
 * 	CR9904 Added support for help system
 * 	[1994/03/14  13:44:16  kevins]
 * 
 * Revision 1.1.5.6  1994/03/10  15:50:19  kevins
 * 	CR 10030 removed extra newlines
 * 	[1994/03/10  15:42:12  kevins]
 * 
 * Revision 1.1.5.5  1994/02/03  18:42:13  kevins
 * 	updated to conform to func spec
 * 	[1994/02/03  18:29:37  kevins]
 * 
 * Revision 1.1.5.4  1993/12/29  15:58:04  kevins
 * 	removed cdscp_attr.h from includes
 * 	[1993/12/29  15:39:28  kevins]
 * 
 * Revision 1.1.5.3  1993/12/27  15:05:43  kevins
 * 	Remove dce_error function call
 * 	[1993/12/27  15:03:23  kevins]
 * 
 * Revision 1.1.5.2  1993/12/22  18:12:27  kevins
 * 	Message catalog cleanup
 * 	[1993/12/22  12:05:13  kevins]
 * 
 * Revision 1.1.5.1  1993/12/20  19:52:29  kevins
 * 	Fixed to conform to style guide.
 * 	[1993/12/20  18:51:47  kevins]
 * 
 * Revision 1.1.2.3  1993/12/13  14:22:21  kevins
 * 	Updated messages for catalog.
 * 	[1993/12/13  13:48:14  kevins]
 * 
 * Revision 1.1.2.2  1993/11/05  19:52:52  kevins
 * 	fixed  list execution for dir list
 * 	[1993/11/05  19:41:38  kevins]
 * 
 * Revision 1.1.2.1  1993/10/21  19:39:23  kevins
 * 	completed the name change from dsh to dcecp
 * 	[1993/10/21  19:38:45  kevins]
 * 
 * Revision 1.1.2.12  1993/10/14  15:30:38  kevins
 * 	Altered full_names passed to cdsDelChild and cdsCreateChild
 * 	to be opaque
 * 	[1993/10/14  15:30:24  kevins]
 * 
 * Revision 1.1.2.11  1993/10/13  19:01:31  kevins
 * 	Added directory rmfrom and addto command.
 * 	Completed directory modify commands.
 * 	removed attribute manipulation. Added these to cdscp_utils.c
 * 	[1993/10/13  18:55:44  kevins]
 * 
 * Revision 1.1.2.10  1993/09/16  18:04:19  kevins
 * 	modified attribute information
 * 	[1993/09/16  18:03:35  kevins]
 * 
 * Revision 1.1.2.9  1993/09/15  12:56:25  kevins
 * 	Fix in show dir. No longer munging return value.
 * 	[1993/09/15  12:55:54  kevins]
 * 
 * Revision 1.1.2.8  1993/09/13  14:03:56  kevins
 * 	modified uuid decoding
 * 	[1993/09/13  14:02:19  kevins]
 * 
 * Revision 1.1.2.7  1993/09/08  19:58:16  kevins
 * 	modified attribute decoding
 * 	[1993/09/08  19:50:29  kevins]
 * 
 * 	Alter command return values
 * 	[1993/08/03  16:41:52  kevins]
 * 
 * Revision 1.1.2.6  1993/08/25  20:11:22  kevins
 * 	updated attribute reading parameters
 * 	[1993/08/25  20:10:17  kevins]
 * 
 * Revision 1.1.2.5  1993/08/23  14:27:24  kevins
 * 	Updated attribute encoding/decoding
 * 	[1993/08/23  14:15:02  kevins]
 * 
 * Revision 1.1.2.4  1993/08/03  17:07:51  kevins
 * 	Alter command return values
 * 	[1993/08/03  17:00:25  kevins]
 * 
 * Revision 1.1.2.3  1993/07/26  15:27:20  kevins
 * 	removed tk.h and replaced with dsh.h
 * 	[1993/07/26  15:18:55  kevins]
 * 
 * Revision 1.1.2.2  1993/07/23  20:01:56  kevins
 * 	Initial submission of dsh
 * 	[1993/07/23  19:30:53  kevins]
 * 
 * $EndLog$
 */
/* INCLUDES */
#include <stdio.h>
#include <locale.h>
#include <setjmp.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>                     /* for UINT_MAX */

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

#include <stdlib.h>
#include <string.h>
#include <dce/dce_error.h>
#include <sys/types.h>
#include <unistd.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dce_svc.h>
#include <dce/tcl.h>
#include <dce/cdsclerk.h>
#include <dce/sec_login.h>
#include <cprpc_clerk.h>
#include <cprpc_server.h>
#include <cdscp.h>
#include <dce/dnsmessage.h>
#include <dce/attributes.h>
#include <dce/dnsclerk.h>
#include <dce/dnsmanpar.h>
#include <dce/sys_time.h>
#include <dcecp.h>
#include <util_general.h>
#include <util_var.h>
#include <util_help.h>

/* GLOBAL DEFINITIONS */

#define OLD 0
#define NEW 1

#define dnsMasterRep 9
#define dnsSecondRep 10
#define dnsReadRep 11
#define dnsExcludeRep 12

#define NUM_CLERK_ATTRS 8
#define NUM_SERVER_ATTRS 11

#define CDS_CLERK_CREATE_TIME 0
#define CDS_CLERK_PROTOCOL_ERROR_COUNT 1
#define CDS_CLERK_AUTHENTICATION_COUNT 2
#define CDS_CLERK_READ_OPS_COUNT 3
#define CDS_CLERK_CACHE_HITS_COUNT 4
#define CDS_CLERK_CACHE_BYPASS_COUNT 5
#define CDS_CLERK_WRITE_OPS_COUNT 6
#define CDS_CLERK_MISC_OPS_COUNT 7

#define DNS_CREATE_TIME 0
#define DNS_FUTURE_SKEW 6
#define DNS_READ_ACCESS_COUNT 11
#define DNS_WRITE_ACCESS_COUNT 12
#define DNS_SKULKS_INITIATED_COUNT 13
#define DNS_SKULKS_COMPLETED_COUNT 14
#define DNS_TREE_BROKEN_COUNT 15
#define DNS_CRUCIAL_REPLICA_COUNT 17
#define DNS_CHILD_POINTER_COUNT 18
#define DNS_SECURITY_COUNT 19
#define DNS_KNOWN_CLEARINGHOUSES 21

/* PROTOTYPES */

int cdsDelChild(cds_full_name_t *, struct cdsFlagStat *);
int create_repset(Tcl_Interp *,cds_full_name_t directory, Set_t **set);
int cdsCreateChild(cds_full_name_t *,sec_id_foreign_t *,uuid_t *,
                   Set_t *,int,uuid_t *,struct dnsFlagStat *);
int split_combine(Tcl_Interp *, char *, char *, int *, char ***);
void translate_counter64(cprpc_attr_t *, char *);
static int enq(int, unsigned char *, int);
static int free_list(void);
handle_t cds_bind_to_server(Tcl_Interp *, char *, unsigned32 *);

/* INTERNAL VARIABLES */

typedef struct ep {
    struct ep      *link;
    int            r_type;
    dns_opq_fname  ch_name;
} epochDesc;

epochDesc *old_epoch_head = NULL;
epochDesc *old_epoch_tail = (epochDesc *)&old_epoch_head;
epochDesc *new_epoch_head = NULL;
epochDesc *new_epoch_tail = (epochDesc *)&new_epoch_head;


/*
 * FUNCTION: directory_create
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */

int directory_create(ClientData clientData, 
                     Tcl_Interp *interp,
                     int argc, 
                     char **argv)
{
    
    cds_id_t            cts;
    int                 cds_status = 0;
    int                 dir_number = 1; 
    struct cdsFlagStat  flags;
    cds_full_name_t     dir_name;
    char                *clearinghouse;
    cds_full_name_t     cds_clearinghouse;
    enum cdsReplicaType ReplicaType = cdsReadOnly;
    boolean32           replica = FALSE;
    char                **cargv;
    int                 cargc;
    cds_attribute_list  *list;
    char                *attribute;
    cdsEntries_t        type = cdsDir;
    cdsAttrType_t       attr_type = cdsSet;
    Attribute_Action_t  action = AddAttribute;
    cds_attr_name_t     attribute_name;
    int                 j, i;
    int                 single;
    
   
    
    dcp_ArgvInfo  arg_table[] = {
        {"-attribute",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_cds_dir_create_attr_help},
        {"-clearinghouse",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_cds_dir_create_clh_help},
        {"-replica",DCP_ARGV_CONSTANT,(char *)1,NULL,
             dcp_t_cds_dir_create_replica_help},
        {"-single",DCP_ARGV_CONSTANT,(char *)1,NULL,
             dcp_t_cds_dir_single_help}, 
        {NULL,DCP_ARGV_END,NULL,NULL,0} 
    };
   /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();
    /*
     *  Initialize the parse table with appropriate pointers 
     */
    clearinghouse = NULL;
    attribute = NULL;
    single = 0;
    arg_table[0].dst = (char *)&attribute;
    arg_table[1].dst = (char *)&clearinghouse;
    arg_table[2].dst = (char *)&replica;
    arg_table[3].dst = (char *)&single;    

    if(dcp_ParseArgv(interp,&argc,argv,arg_table,0) != TCL_OK)
        return TCL_ERROR;
    /*
     * If the parser comes back with less than two arguments the 
     *  user typed something wrong. terminate command. 
     */
    DCP_CHECK_CDS_NO_ARG(dir);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if(single && !attribute){
        DCP_SET_MISC_ERROR(dcp_s_cds_single_no_attr);
    }
    if(single)
        attr_type = cdsSingle;

    /* setup replica, clearinghouse & attribute information if specified */
    if (replica)
        if (!clearinghouse){
            DCP_SET_MISC_ERROR(dcp_s_cds_no_clearinghouse_option);
        }
    if(!clearinghouse){
        clearinghouse = DCP_GENERAL_GET_S_VAR(CLR_NAME);
        if(!strlen(clearinghouse))
            clearinghouse = NULL;
    }

    if (clearinghouse) {
        DCP_CHECK_CDS_NAME_LEN(clearinghouse);
        strcpy((char *)cds_clearinghouse.uc, clearinghouse);
    }

    if (attribute) {
        list = (cds_attribute_list *)malloc(sizeof(cds_attribute_list));
        memset((char *)list,0,sizeof(cds_attribute_list));
        if(!list){
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        if(cds_parse_attributes( interp, 
                                attribute, 
                                action, 
                                type,
                                attr_type, 
                                list)  != TCL_OK)
            return TCL_ERROR;
    }    
    /* 
     *  retrieve the currently established network context. 
     */
    do_sec_login();
    /* cycle through all of the remaining entries within argument list. */
    /* ignore the entry for the original command  */
    Tcl_SplitList(interp,argv[1],&cargc,&cargv);
    for (dir_number = 0; dir_number < cargc; dir_number++) {
        DCP_CHECK_CDS_NAME_LEN(cargv[dir_number]);
        strcpy((char *)dir_name.uc, cargv[dir_number]);
        /* clear the flags each time as previous calls to cds may 
           manipulate them inappropriately */
        clear_flags(&flags,cdsStrDCE);
        /* 
         * if the user requested create a replica in the named clearinghouse
         * the dir_name must already exist in some other clearinghouse. 
         */
        if (replica)
            cds_status = cdsAddReplica(&dir_name,&cds_clearinghouse,
                                       ReplicaType,&flags);
        else {
            /* 
             * Create a directory object in a clearinghouse if provided 
             * otherwise create the directory object in the default 
             * clearinghouse. 
             */
            if (clearinghouse)
                cds_status = cdsCreateDir(&dir_name, 
                                          &cds_clearinghouse,&cts,&flags);
            else
                cds_status = cdsCreateDir(&dir_name,NULL,&cts,&flags);
        }
        if (cds_status != CDS_SUCCESS){
            free((char *)cargv);
            DCP_SET_MISC_ERROR(flags.fsLocStat);
        }
        if (attribute){
            for(j = 0; j < list->cur_value ; j++){
                for(i = 0; i < list->value[j]->cur_attr; i++){
                    DCP_CHECK_CDS_NAME_LEN(list->value[j]->attr_name);
                    strcpy((char *)attribute_name.uc,list->value[j]->attr_name);
                    cds_status = cdsAddAttrValue(&dir_name,&attribute_name,
                                                 list->value[j]->attr_type,
                                                 list->value[j]->obj_type,
                                                 list->value[j]->attribute[i],
                                                 NULL, &flags);
                    if(cds_status != CDS_SUCCESS){
			char *temp_str;
                        temp_str = (char *)dce_sprintf(flags.fsLocStat);
			/* try to delete directory, and report result */
			if (replica) {
			    cds_status = cdsRmReplica(&dir_name,&cds_clearinghouse,&flags);
			} /* end if replica */ 
			else {
			    cds_status = cdsDelDir(&dir_name,&flags);
			}
			if (cds_status != CDS_SUCCESS){
			    DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_cds_dir_created,
							dir_name.uc);
			}
			else {
			    DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_cds_dir_not_created,
							dir_name.uc);
			}
			Tcl_AppendResult(interp, " ", temp_str, (char *)NULL);
                        free((char *)cargv);
			return TCL_ERROR;
		    }
                } /* end for all entries within an attribute */
            } /* end for all attributes */
        } /* end if attribute */
    } /* end for() splitlist */
    /*
     * Free the pointer returned from SplitList. 
     */
    free((char *)cargv);
    return TCL_OK;
}

/*
 * FUNCTION: directory_delete
 *
 * OVERVIEW:
 * The directory delete command will delete one or more directories. 
 * The user must have the appropriate acl rights in the replica and 
 * the parent replica.
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *
 */
int directory_delete(ClientData clientData, 
                     Tcl_Interp *interp,
                     int argc, 
                     char **argv)
{
    int                 cds_status = 0;
    struct cdsFlagStat  flags;
    cds_full_name_t     dir_name;
    char                *clearinghouse = NULL;
    cds_full_name_t     cds_clearinghouse;
    int                 replica = 0;
    int                 tree = 0;
    int                 force = 0;
    int                 dir_number = 1; 
    char                **dargv;
    int                 dargc;
    unsigned32          result = error_status_ok;
    int                 script_status;
    char                command[1024];
    extern              Tcl_Interp *dir_interp;
    cds_attribute_list  *list;
    cds_full_name_t     *clearinghouse_value;
    cdsEntries_t        type = cdsDir;
    int                 action = ShowAttribute;
    int                 i, j;
    cds_attrvalue_t     *value;
    unsigned int        replica_type;
    char                *temp;
    ReplicaPointer_t    *rp_p;
    unsigned char       *clr_name;

    dcp_ArgvInfo  arg_table[] = {
        {"-clearinghouse",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_cds_dir_delete_clh_help},
        {"-force",DCP_ARGV_CONSTANT,(char *)1,NULL,
             dcp_t_cds_dir_delete_force_help},
        {"-replica",DCP_ARGV_CONSTANT,(char *)1,NULL,
             dcp_t_cds_dir_delete_rep_help},    
        {"-tree",DCP_ARGV_CONSTANT,(char *)1,NULL,
             dcp_t_cds_dir_delete_tree_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Initialize the destination values for parsing.
     */
    arg_table[0].dst = (char *)&clearinghouse;
    arg_table[1].dst = (char *)&force;
    arg_table[2].dst = (char *)&replica;
    arg_table[3].dst = (char *)&tree;
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    DCP_CHECK_CDS_NO_ARG(dir);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * If trying to force a delete, neither the -replica nor
     * the -clearinghouse options may be used.
     */
    if (force && (replica || clearinghouse)) {
        DCP_SET_MISC_ERROR(dcp_s_cds_force_replica_option);
    }

    /*
     * If trying to delete a directory using the -tree option,
     * neither the -replica nor -clearinghouse options can be
     * used.
     */
    if (tree && (replica || clearinghouse)) {
        DCP_SET_MISC_ERROR(dcp_s_cds_tree_replica_option);
    }

    /*
     * If trying to delete a directory, not a replica, the
     * -clearinghouse option is meaningless.
     */
    if (clearinghouse && !replica) {
        DCP_SET_MISC_ERROR(dcp_s_cds_clh_replica_option);
    }

    /*
     * If trying to delete a replica, the clearinghouse
     * name is required.
     */
    if (replica) {
        if (!clearinghouse) {
            DCP_SET_MISC_ERROR(dcp_s_cds_no_clearinghouse_option);
        }
        DCP_CHECK_CDS_NAME_LEN(clearinghouse);
        strcpy((char *)cds_clearinghouse.uc, clearinghouse);
    }

    /*
     * Attempt to set the current login context.
     */
    do_sec_login();

    /*
     * If the command argument is a list, parse it.
     */
    if (Tcl_SplitList(interp, argv[1], &dargc, &dargv) == TCL_ERROR)
        return TCL_ERROR;

    for (dir_number = 0; dir_number < dargc; dir_number++) {
        /* 
         * Cycle through all entries in the list.
         */
        clear_flags(&flags,cdsStrDCE);
        DCP_CHECK_CDS_NAME_LEN(dargv[dir_number]);
        strcpy((char *)dir_name.uc, (char *)dargv[dir_number]);

        if (tree) {
            /*
             * Invoke the TCL script that recurses down the tree
             * (using the given directory name as the root) and
             * deletes everything.
             */
            sprintf(command, "dir_delete_tree %d %s", force, dargv[dir_number]);
            if ((result = Tcl_Eval(dir_interp, command)) != TCL_OK) {
                /*
                 * An error occured in the script during the delete.
                 * Copy result from the command into main interp.
                 */
                sscanf(dir_interp->result, "%d", &script_status);
                DCP_SET_RESULT_CODE(script_status);
                return TCL_ERROR;
            }
        }
        else if (replica) {
            /*
             * Delete the specified replica.
             */
            cds_status = cdsRmReplica(&dir_name, &cds_clearinghouse, &flags);
            if (cds_status != CDS_SUCCESS){
                DCP_SET_MISC_ERROR(flags.fsLocStat);
            }
        }
        else {
            /*
             * Delete the specified directory.
             */
            cds_status = cdsDelDir(&dir_name, &flags);
            if (cds_status != CDS_SUCCESS) {
                /*
                 * Something went wrong.
                 */
                if (force && (flags.fsLocStat == CDS_MORETHANONEREPLICA)) {
                    /*
                     * The directory delete failed because this directory is
                     * replicated.  In order to delete the directory, the
                     * replicas must be deleted first.  If the -force flag
                     * is set, we have permission to do this.
                     */
                    clearinghouse_value = NULL;
                    if (enum_all_attr(interp, &dir_name, clearinghouse_value,
                                      type, action, &list) != TCL_OK) {
                        return TCL_ERROR;
                    }

                    /*
                     * See if we have a list structure filled with
                     * information about this directory.  If not,
                     * report an error.
                     */
                    if (((list != NULL) && (list->cur_value == 0)) || (list == NULL)) {
                        /*
                         * For some reason, we are unable to get information
                         * about the directory.
                         */
                        DCP_SET_MISC_ERROR(dcp_s_cds_no_attr_list_value);
                    }

                    /*
                     * We have cds information.  Find the replica data.
                     */
                    for (i = 0; i < list->cur_value; i++) {
                        if (strcmp(list->value[i]->attr_name, "CDS_Replicas") == 0) {
                            /*
                             * We found the replica pointer structure.
                             */
                            for (j = 0; j < list->value[i]->cur_attr; j++) {
                                /*
                                 * For each replica, figure out the replica type.
                                 */
                                value = list->value[i]->attribute[j];
                                rp_p = (ReplicaPointer_t *)value->val_u.val_byte.byte_p;
                                temp = (char *)FLD_ReplicaPointer_replicaType(rp_p);
                                replica_type = (int)*temp;
                                if ((replica_type == cdsSecondary) ||
                                    (replica_type == cdsReadOnly)) {
                                    /*
                                     * This is a replica, not a master.  Find the
                                     * clearinghouse name.
                                     */
                                    clr_name = (unsigned char *)FLD_ReplicaPointer_CHName(rp_p);
                                    decode_clearinghouse_name((char *)clr_name,
                                                              (char *)cds_clearinghouse.uc);
                                    /*
                                     * Using the directory name and the clearinghouse
                                     * name, delete the replica.
                                     */
                                    cds_status = cdsRmReplica(&dir_name,
                                                              &cds_clearinghouse,
                                                              &flags);
                                    if (cds_status != CDS_SUCCESS) {
                                        DCP_SET_MISC_ERROR(flags.fsLocStat);
                                    }
                                }
                            }
                            /*
                             * If we found any replicas, they have been deleted.  We
                             * can stop looking through the directory information now.
                             */
                            break;
                        }
                    }
                    /*
                     * All the directory information has been searched.  All the
                     * replicas for this directory have been removed.  Therefore,
                     * we can try to delete the directory again.  Any error at
                     * this point is serious, so we'll report it and leave.
                     */
                    cds_status = cdsDelDir(&dir_name,&flags);
                    if (cds_status != CDS_SUCCESS){
                        DCP_SET_MISC_ERROR(flags.fsLocStat);
                    }   
                }
                else {
                   /*
                    * Either we do not have -force permission, or the error
                    * is something that cannot be handled.  Regardless,
                    * report the error and leave.
                    */
                    DCP_SET_MISC_ERROR(flags.fsLocStat);
                }
            }
        }
    }
    free((char *)dargv);
    return TCL_OK;
}


/*****************************************************************
 * 
 * 
 *                  This command accepts no switches. 
 *
 ****************************************************************/
/*
 * FUNCTION: directory_update
 *
 * OVERVIEW: directory update performs a skulk on the specified directory 
 *            This command accepts no switches.
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */

int
    directory_update(ClientData clientData, 
                     Tcl_Interp *interp,
                     int argc, 
                     char **argv)
{
    unsigned            result = 0;
    struct cdsFlagStat  flags;
    cds_full_name_t     dir_name;
    char                **sargv;
    int                 sargc;
    int                 i;

    

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();
    DCP_CHECK_CDS_NO_ARG(dir);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    do_sec_login();
    /* clear and initialize the status of the cds flags */
    /* check to see if command argument is a list. If so parse it */
    if (Tcl_SplitList(interp,argv[1],
                      &sargc,&sargv) == TCL_ERROR)
        return TCL_ERROR;
    
    for (i = 0;i < sargc; i++){
        clear_flags(&flags,cdsStrDCE);
        DCP_CHECK_CDS_NAME_LEN(sargv[i]);
        strcpy((char *)dir_name.uc,sargv[i]);
        result = cdsSkulkDir(&dir_name,&flags);
    /* check for a failure. */
        if (result != CDS_SUCCESS){
            free((char *)sargv);
            DCP_SET_MISC_ERROR(flags.fsLocStat);
        }
    }
    free((char *)sargv);
    return TCL_OK;
}

/*
 * FUNCTION: directory_rmfrom
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */

int
    directory_rmfrom(ClientData clientData, Tcl_Interp *interp,int argc, char **argv)
{
    int                 status = 0;
    struct cdsFlagStat  flags;
    cds_full_name_t     child_name;
    char                *member = NULL;
    char                full_name[CDS_SNAME_MAX];
    int                 length = CDS_SNAME_MAX;
    char                **aargv;
    int                 aargc;
    char                **bargv;
    int                 i = 0, j;
    int                 bargc;
     
    
    dcp_ArgvInfo  arg_table[] = {
        {"-member",DCP_ARGV_STRING,NULL,NULL,dcp_t_cds_member_del_help},    
        {NULL,DCP_ARGV_END,NULL,NULL,0} 
    };
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();
    /* initialize the dst value for parsing. */
    member = NULL;
    arg_table[0].dst = (char *)&member;
    if(dcp_ParseArgv(interp,&argc,argv,arg_table,0) == TCL_ERROR)
        return TCL_ERROR;

    DCP_CHECK_CDS_NO_ARG(dir);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if(member == NULL){
        DCP_SET_MISC_ERROR(dcp_s_cds_no_member);
    }

    if(Tcl_SplitList(interp,member,&bargc,&bargv) == TCL_ERROR)
        return TCL_ERROR;

    if(Tcl_SplitList(interp,argv[1],&aargc,&aargv) == TCL_ERROR)
        return TCL_ERROR;
     
    do_sec_login();

    for(;i < aargc; i++) {
        for(j = 0; j < bargc; j++) {
            sprintf(full_name,"%s/%s",aargv[i],bargv[j]);
            length = CDS_SNAME_MAX;
            cdsCvtFullToOpq( (unsigned char *)full_name,
                            &child_name, &length);
            
            /* clear and initialize the status of the cds flags */
            clear_flags(&flags,cdsOpqDNA);
            status = cdsDelChild(&child_name,&flags);
            /* check for a failure. */
            if (status != CDS_SUCCESS){
                DCP_SET_MISC_ERROR(flags.fsLocStat);
            }
        }
    }
    return TCL_OK;
}

/*
 * FUNCTION: directory_addto
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */

int
    directory_addto(ClientData clientData, Tcl_Interp *interp,int argc, char **argv)
{
    int                 status = 0;
    struct cdsFlagStat  flags;
    cds_full_name_t     dir_name;
    cds_full_name_t     clearinghouse_name;
    cds_full_name_t     child_name;
    cds_full_name_t     temp_child_name;
    char                *member;
    char                *clearinghouse;
    Set_t               *set;
    char                **aargv;
    int                 aargc;
    char                **bargv;
    int                 i = 0, j;
    int                 bargc;
    int                 length = CDS_SNAME_MAX;
    cds_handle_t        cdshandle;
    cds_attribute_list  *list = NULL;

    dcp_ArgvInfo  arg_table[] = {
        {"-clearinghouse",DCP_ARGV_STRING,NULL,NULL,dcp_t_cds_child_add_help},
        {"-member",DCP_ARGV_STRING,NULL,NULL,dcp_t_cds_member_add_help},    
        {NULL,DCP_ARGV_END,NULL,NULL,0} 
    };
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();
    /* initialize the dst value for parsing. */
    
    clearinghouse = NULL;
    member = NULL;
    arg_table[0].dst = (char *)&clearinghouse;
    arg_table[1].dst = (char *)&member;
    
    if(dcp_ParseArgv(interp,&argc,argv,arg_table,0) == TCL_ERROR)
        return TCL_ERROR;
    
    DCP_CHECK_CDS_NO_ARG(dir);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);
    
    if (!member){
        DCP_SET_MISC_ERROR(dcp_s_cds_no_member);
    }
    if (!clearinghouse){
        DCP_SET_MISC_ERROR(dcp_s_cds_no_clearinghouse_option);
    }
    DCP_CHECK_CDS_NAME_LEN(clearinghouse);
    strcpy((char *)clearinghouse_name.uc,clearinghouse);

    do_sec_login();
    if(Tcl_SplitList(interp,member,&bargc,&bargv) == TCL_ERROR)
        return TCL_ERROR;

    if(Tcl_SplitList(interp,argv[1],&aargc,&aargv) == TCL_ERROR)
        return TCL_ERROR;

    for(;i < aargc; i++) {
        DCP_CHECK_CDS_NAME_LEN(aargv[i]);
        strcpy((char *)dir_name.uc,aargv[i]);
        for(j = 0; j < bargc; j++) {
            DCP_CHECK_CDS_NAME_LEN(bargv[j]);
            sprintf((char *)temp_child_name.uc,"%s/%s",dir_name.uc,bargv[j]);
            /* allocate a pointer to contain the replicapointer */
            set = (Set_t *)malloc(sizeof(Set_u) + sizeof(ReplicaPointer_u) +
                                  sizeof(SetMember_u));
            if (set == NULL) {
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }

            cdshandle = cdsGetHandle();
            if (cdshandle == NULL_CDS_HANDLE) {
                DCP_SET_RESULT_CODE(dcp_s_cds_no_handle);
                free(aargv);
                free(bargv);
                free(set);
                return TCL_ERROR;
            }
                
            length = CDS_SNAME_MAX;
            cdsCvtFullToOpq( (unsigned char *)temp_child_name.uc,
                            &child_name, &length);
        
            /* Obtain the uuid of the parent pointer. */
            status = enum_one_attr(interp,&temp_child_name,&clearinghouse_name, 
                                   "CDS_ObjectUUID",dnsDir,&list);
            if(status != TCL_OK){   
                return TCL_ERROR;
            }
            /* create a replica set of the base dir */
            if(create_repset(interp,temp_child_name,&set) != TCL_OK)
                return TCL_ERROR;
            /* clear and initialize the status of the cds flags */
            clear_flags(&flags,cdsOpqDNA);
        
            status = cdsCreateChild(&child_name,NULL,
                                    &(list->value[0]->attribute[0]->val_u.val_uuid),
                                    set,LEN_Set(set),NULL,&flags);
        
            cdsFreeHandle(cdshandle);
        
            /* check for a failure. */
            if (status != CDS_SUCCESS){
                DCP_SET_MISC_ERROR(flags.fsLocStat);
            }
            free(set);
            free_attr_value(&list);
            list = NULL;
        }
    }
    free(aargv);
    free(bargv);
    return TCL_OK;
}

/*
 * FUNCTION: directory_list
 *
 * OVERVIEW: List the contents of a directory. Provide the capability to 
 *           list all of the decendents of the specified dir. 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */

int directory_list(ClientData clientData,
                   Tcl_Interp *interp,
                   int argc,
                   char **argv)
{
    cds_full_name_t     dir_name;
    int                 cds_status = CDS_SUCCESS;
    int                 tmp_status = CDS_SUCCESS;
    unsigned32          status = rpc_s_ok;
    int                 dir_number; 
    struct cdsFlagStat  flags;
    cds_handle_t        cds_context_handle;
    int                 cds_str_sname_len;
    unsigned char       cds_str_sname[CDS_STR_SNAME_MAX];
    char                **largv;
    int                 largc;
    char                *temp_name = NULL;
    boolean32           directory = FALSE;
    boolean32           fullname = FALSE;
    boolean32           link = FALSE;
    boolean32           object = FALSE;
    boolean32           simplename = FALSE;
    boolean32           list_all = TRUE;
    char                root_name[CDS_SNAME_MAX];
    char                root_name_temp[CDS_SNAME_MAX];
    cds_simple_name_t   sname;
    int                 sname_size;
    cds_simple_name_t   *eachp;
    cds_full_name_t     directory_name;
    int                 dir_size;

    dcp_ArgvInfo  arg_table[] = {
        {"-directories", DCP_ARGV_CONSTANT, (char *)1, NULL,
             dcp_t_cds_dir_list_dir_help},
        {"-fullname", DCP_ARGV_CONSTANT, (char *)1, NULL,
             dcp_t_cds_dir_list_full_help},
        {"-links", DCP_ARGV_CONSTANT, (char *)1, NULL,
             dcp_t_cds_dir_list_link_help},
        {"-objects", DCP_ARGV_CONSTANT, (char *)1, NULL,
             dcp_t_cds_dir_list_obj_help},
        {"-simplename", DCP_ARGV_CONSTANT, (char *)1, NULL,
             dcp_t_cds_dir_list_smpl_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&directory;
    arg_table[1].dst = (char *)&fullname;
    arg_table[2].dst = (char *)&link;
    arg_table[3].dst = (char *)&object;
    arg_table[4].dst = (char *)&simplename;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    DCP_CHECK_CDS_NO_ARG(dir);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Specifying both full and simplename format is not allowed.
     */
    if ((fullname == TRUE) && (simplename == TRUE)) {
        DCP_SET_RESULT_CODE(dcp_s_mut_excl_opts);
        return TCL_ERROR;
    }

    if (directory || object || link)
        list_all = FALSE;

    if (simplename == FALSE)
        fullname = TRUE;

    /*
     * Set up the sname parameter if objects
     * are being requested.
     */
    if (list_all || object) {
        sname_size = sizeof(sname);
        cdsCvtSimpleToOpq((unsigned char *)"*", 1, &sname, &sname_size);
    }

    Tcl_ResetResult(interp);

    /*
     * Attempt to set the current login context.
     */
    do_sec_login();

    /*
     * Cycle through all of the directories to be listed.
     */
    if (Tcl_SplitList(interp, argv[1], &largc, &largv) != TCL_OK)
        return TCL_ERROR;

    for (dir_number = 0; dir_number < largc; dir_number++) {
        DCP_CHECK_CDS_NAME_LEN(largv[dir_number]);
        strcpy((char *)dir_name.uc, (char *)largv[dir_number]);

        /*
         * Save a copy of the root pathname for full/simple translation.
         */
        temp_name = expand_cell_name(largv[dir_number], &status);
        DCP_CHECK_RPC_ERROR(status);
        strcpy(root_name, temp_name);
        free(temp_name);

        /*
         * Enumerate each directory/child first.  The enum function will 
         * return a cds_empty message when the last entry has been aquired
         */
        if (directory || list_all) {
            clear_flags(&flags, cdsStrDCE);   
            cds_status = CDS_SUCCESS;
            cds_context_handle = cdsGetHandle();
            if (cds_context_handle == NULL_CDS_HANDLE) {
                free((char *)largv);
                DCP_SET_RESULT_CODE(dcp_s_cds_no_handle);
                return TCL_ERROR;
            }

            while (cds_status != CDS_EMPTY) {
                /*
                 * Set the name length each loop. This must be set
                 * to the maximum avail length of a cds simple name.
                 */ 
                cds_str_sname_len = CDS_STR_SNAME_MAX; 
                cds_status = cdsEnumChild(&dir_name,
                                          cds_context_handle,
                                          NULL,
                                          NULL,
                                          cds_str_sname,
                                          &cds_str_sname_len,
                                          &flags);

                if (cds_status == CDS_SUCCESS) {
                    if (fullname)
                        sprintf(root_name_temp, "%s/%s",
                                root_name, (char *)cds_str_sname);
                    else
                        strcpy(root_name_temp, (char *)cds_str_sname);
                    Tcl_AppendElement(interp, root_name_temp);
                }
                else if (cds_status == CDS_ERROR) {
                    cdsFreeHandle(cds_context_handle);
                    DCP_SET_MISC_ERROR(flags.fsLocStat);
                }
            }
            cdsFreeHandle(cds_context_handle);
        }

        /*
         * Add objects to the directory list being generated.
         */
        if (object || list_all) {
            clear_flags(&flags, cdsOpqDNA);   
            cds_status = CDS_SUCCESS;
            cds_context_handle = cdsGetHandle();
            if (cds_context_handle == NULL_CDS_HANDLE) {
                free((char *)largv);
                DCP_SET_RESULT_CODE(dcp_s_cds_no_handle);
                return TCL_ERROR;
            }

            dir_size = CDS_SNAME_MAX;
            tmp_status = cdsCvtFullToOpq((unsigned char *)dir_name.uc,
                                         &directory_name,
                                         &dir_size);
            DCP_CHECK_CDS_ERROR(tmp_status);

            while (cds_status != CDS_EMPTY) {
                cds_status = cdsEnumObj(&directory_name,
                                        cds_context_handle,
                                        NULL,
                                        (cds_simple_name_t *)&sname,
                                        (int *)NULL,
                                        &eachp,
                                        (unsigned char *)NULL,
                                        (int *)NULL,
                                        (cds_simple_name_t **)NULL,
                                        (unsigned char *)NULL,
                                        (int *)NULL,
                                        &flags);
                if (cds_status == CDS_SUCCESS) {
                    cds_str_sname_len = CDS_STR_SNAME_MAX; 
                    tmp_status = cdsCvtSimpleToStr(eachp,
                                                   (unsigned char *)cds_str_sname,
                                                   &cds_str_sname_len);
                    DCP_CHECK_CDS_ERROR(tmp_status);

                    if (fullname)
                        sprintf(root_name_temp, "%s/%s",
                                root_name, (char *)cds_str_sname);
                    else
                        strcpy(root_name_temp, (char *)cds_str_sname);
                    Tcl_AppendElement(interp, root_name_temp);
                }
                else if (cds_status == CDS_ERROR) {
                    DCP_SET_MISC_ERROR(flags.fsLocStat);
                }
            }
            cdsFreeHandle(cds_context_handle);
        }

        /*
         * Enumerate all of the softlinks.
         */
        if (link || list_all) {
            clear_flags(&flags, cdsStrDCE);
            cds_status = CDS_SUCCESS;
            cds_context_handle = cdsGetHandle();
            if (cds_context_handle == NULL_CDS_HANDLE) {
                free((char *)largv);
                DCP_SET_RESULT_CODE(dcp_s_cds_no_handle);
                return TCL_ERROR;
            }

            while (cds_status != CDS_EMPTY) {
                /*
                 * Set the name length each loop. This must be set
                 * to the maximum avail length of a cds simple name.
                 */
                cds_str_sname_len = CDS_STR_SNAME_MAX;
                cds_status = cdsEnumSLink(&dir_name,
                                          cds_context_handle,
                                          NULL,
                                          NULL,
                                          cds_str_sname,
                                          &cds_str_sname_len,
                                          &flags);
                if (cds_status == CDS_SUCCESS) {
                    if (fullname)
                        sprintf(root_name_temp, "%s/%s",
                                root_name, (char *)cds_str_sname);
                    else
                        strcpy(root_name_temp, (char *)cds_str_sname);
                    Tcl_AppendElement(interp, root_name_temp);
                }
                else if (cds_status == CDS_ERROR) {
                    DCP_SET_MISC_ERROR(flags.fsLocStat);
                }
            }
            cdsFreeHandle(cds_context_handle);
        }
    }
    free((char *)largv);
    return TCL_OK;
}


/*
 * FUNCTION: directory_merge
 *
 * OVERVIEW: This will copy the contents of one directory into another
 *  directory. 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguments.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This
 *                      will hold the result of the command, which is
 *                      an empty string in case of success, or a list
 *                      of (name, error message) pairs otherwise.
 *                      This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int directory_merge(ClientData clientData,
                    Tcl_Interp *interp,
                    int argc,
                    char **argv)
{
    char        *clearinghouse = NULL;
    char        *into = NULL;
    int         tree = 0;
    int         nocheck = 0;
    char        **dargv;
    int         dargc;
    unsigned32  result = error_status_ok;
    char        *temp;
    char        source_name[CDS_SNAME_MAX];
    char        target_name[CDS_SNAME_MAX];
    char        command[1024];
    extern      Tcl_Interp *dir_interp;

    dcp_ArgvInfo  arg_table[] = {
        {"-clearinghouse", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_cds_dir_merge_clh_help},
        {"-into", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_cds_dir_merge_into_help},    
        {"-nocheck", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_dir_merge_nocheck_help},
        {"-tree", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_dir_merge_tree_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Initialize the destination value for parsing.
     */
    arg_table[0].dst = (char *)&clearinghouse;
    arg_table[1].dst = (char *)&into;
    arg_table[2].dst = (char *)&nocheck;
    arg_table[3].dst = (char *)&tree;
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    DCP_CHECK_CDS_NO_ARG(dir);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (!into) {
        DCP_SET_MISC_ERROR(dcp_s_cds_merge_no_target);
    }

    /*
     * Attempt to set the current login context.
     */
    do_sec_login();

    /* 
     * Command argument should not be a list -- if it has more than
     * member, it's an error
     */
    if (Tcl_SplitList(interp,argv[1], &dargc, &dargv) == TCL_ERROR) {
        return TCL_ERROR;
    }
    free((char *)dargv);

    if (dargc > 1) {
        DCP_SET_MISC_ERROR(dcp_s_bad_arg_list);
    }

    /*
     * We need to expand the directory names so that they'll match what 
     * "directory list" returns
     */
    temp = expand_cell_name(argv[1], &result);
    DCP_CHECK_RPC_ERROR(result);
    strcpy(source_name, temp);
    free(temp);
    temp = expand_cell_name(into, &result);
    DCP_CHECK_RPC_ERROR(result);
    strcpy(target_name, temp);
    free(temp);

    sprintf(command, "dir_merge %s %s %d %d %s", source_name, target_name,
            nocheck, tree, clearinghouse ? clearinghouse : "0");

    if ((result = Tcl_Eval(dir_interp, command)) != TCL_OK) {
        /*
         * Copy result from above command into main interp
         */
        DCP_SET_RESULT(dir_interp->result);
        return TCL_ERROR;
    }
    return TCL_OK;
}


/*
 * FUNCTION: directory_modify
 *
 * OVERVIEW: This will add, remove or change an attribute in the 
 *           named directory.  Will also change the epoch of a
 *           directory as does the cdscp 'set directory to new
 *           epoch' command.
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */

int directory_modify(ClientData clientData,
                     Tcl_Interp *interp,
                     int argc,
                     char **argv)
{
    char                *add = NULL;
    char                *remove = NULL;
    char                *change = NULL;
    char                *exclude = NULL;
    char                *master = NULL;
    char                *readonly = NULL;
    cds_full_name_t     dir_name;
    cds_attr_name_t     attr_name;
    int                 result = 0;
    int                 dir_number;
    cdsEntries_t        type = cdsDir;
    int                 order[ModifyTypes];
    cds_attribute_list  *list;
    int                 single = 0;
    int                 types = 0;
    int                 i, j;
    int                 dargc;
    char                **dargv;
    int                 eargc;
    char                **eargv;
    int                 margc;
    char                **margv;
    int                 rargc;
    char                **rargv;
    struct cdsFlagStat  flags;
    cds_handle_t        hp;
    cds_attrvalue_t     *value;
    dns_status_t        status = CDS_SUCCESS;
    unsigned char       opq_name[CDS_OPQ_FNAME_MAX];
    int                 namesize = CDS_OPQ_FNAME_MAX;
    unsigned char       opq_dirname[CDS_OPQ_FNAME_MAX];
    int                 dirnamesize = CDS_OPQ_FNAME_MAX;
    ReplicaPointer_t    *rp_p;
    unsigned char       *rtype_p;
    unsigned char       *name_p;
    boolean32           found;
    epochDesc           *op, *np;
    char                old_name[DNS_STR_FNAME_MAX];
    int                 oldlen = DNS_STR_FNAME_MAX;
    int                 ch_count;
    unsigned char       *master_ch;
    unsigned char       **secondary_ch;
    unsigned char       **secP;
    unsigned char       **readonly_ch;
    unsigned char       **ronlyP;

    dcp_ArgvInfo arg_table[] = {
        {"-add", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_cds_dir_mod_add_help},
        {"-change", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_cds_dir_mod_change_help},
        {"-exclude", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_cds_dir_mod_exclude_help},
        {"-master", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_cds_dir_mod_master_help},
        {"-readonly", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_cds_dir_mod_readonly_help},
        {"-remove", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_cds_dir_mod_rem_help},
        {"-single", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_dir_single_help},    
        {"-types", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_cds_mod_types_help},    
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&add;
    arg_table[1].dst = (char *)&change;
    arg_table[2].dst = (char *)&exclude;
    arg_table[3].dst = (char *)&master;
    arg_table[4].dst = (char *)&readonly;
    arg_table[5].dst = (char *)&remove;
    arg_table[6].dst = (char *)&single;
    arg_table[7].dst = (char *)&types;

    determine_modify_order(order, argc, argv);    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_CDS_NO_ARG(dir);
    DCP_CHECK_EXTRA_ARGS();

    /*
     * Set convenience variable.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Check the interaction of the various standard modify options.
     */
    if (!add && !remove && !change && !master) {
        DCP_SET_MISC_ERROR(dcp_s_cds_no_dir_mod_operation);
    }
    if (single && !add && !master) {
        DCP_SET_MISC_ERROR(dcp_s_cds_single_no_add);
    }
    if (types && !remove && !master) {
        DCP_SET_MISC_ERROR(dcp_s_cds_types_no_remove);
    }

    /*
     * Check the interaction of the 'new epoch' options.
     */
    if ((add || remove || change || single || types) && (master || exclude || readonly)) {
        DCP_SET_MISC_ERROR(dcp_s_cds_wrong_options);
    }

    /*
     * Check whether we are modifying an attribute or changing the epoch
     * of the given directory.  If the '-master' option has been selected,
     * then we are changing the epoch.  Otherwise, we are modifying an
     * attribute.
     */
    if (!master) {
        /*
         * Modifying an attribute of the directory.
         */
        list = (cds_attribute_list *)malloc(sizeof(cds_attribute_list));
        memset((char *)list, 0, sizeof(cds_attribute_list));

        result = generate_attribute_list(interp,
                                         add,
                                         remove,
                                         change,
                                         type,
                                         list,
                                         single,
                                         types,
                                         order);
        if (result != TCL_OK)
            return TCL_ERROR;
        do_sec_login();

        /*
         * Split the entry list and cycle through each entry in the list.
         */
        Tcl_SplitList(interp, argv[1], &dargc, &dargv);

        for (dir_number = 0; dir_number < dargc; dir_number++) {
            DCP_CHECK_CDS_NAME_LEN(dargv[dir_number]);
            strcpy((char *)dir_name.uc, dargv[dir_number]);
            result = modify_cds_attribute(interp, &dir_name, &attr_name, list);
            if (result != TCL_OK) {
                free((char *)dargv);
                return result;
            }
        }
    }
    else {
        /*
         * Changing the epoch of the directory.  Only one
         * directory can be processed at a time.
         */
        if (Tcl_SplitList(interp, argv[1], &dargc, &dargv) == TCL_ERROR) {
            return TCL_ERROR;
        }
        if (dargc != 1) {
            free((char *)dargv);
            DCP_SET_RESULT_CODE(dcp_s_dir_mod_bad_num_dirs);
            return TCL_ERROR;
        }

        /*
         * Check that only one master was specified.
         */
        if (Tcl_SplitList(interp, master, &margc, &margv) == TCL_ERROR) {
            free((char *)dargv);
            return TCL_ERROR;
        }
        if (margc != 1) {
            free((char *)dargv);
            free((char *)margv);
            DCP_SET_RESULT_CODE(dcp_s_dir_mod_bad_num_masters);
            return TCL_ERROR;
        }

        /*
         * Get the rest of the names if required.
         */
        if (exclude != NULL) {
            if (Tcl_SplitList(interp, exclude, &eargc, &eargv) == TCL_ERROR) {
                free((char *)dargv);
                free((char *)margv);
                return TCL_ERROR;
            }
        }
        else {
            eargc = 0;
            eargv = NULL;
        }
        if (readonly != NULL) {
            if (Tcl_SplitList(interp, readonly, &rargc, &rargv) == TCL_ERROR) {
                free((char *)dargv);
                free((char *)margv);
                if (eargv != NULL)
                    free((char *)eargv);
                return TCL_ERROR;
            }
        }
        else {
            rargc = 0;
            rargv = NULL;
        }

        /*
         * Go through and make sure there are no duplicate
         * clearinghouse names in the structures.  If so, it
         * is an error.  At this point, we must have only one
         * 'master', but can also have any number specified
         * in the 'exclude' and 'readonly' arrays.
         */
        for (i = 0; i < eargc; i++) {
            for (j = 0; j < rargc; j++) {
                if (strcmp(eargv[i], rargv[j]) == 0) {
                    free((char *)dargv);
                    free((char *)margv);
                    free((char *)eargv);
                    free((char *)rargv);
                    DCP_SET_RESULT_CODE(dcp_s_dir_mod_duplicate_found);
                    return TCL_ERROR;
                }
            }
            if (strcmp(eargv[i], margv[0]) == 0) {
                free((char *)dargv);
                free((char *)margv);
                free((char *)eargv);
                if (rargv != NULL)
                    free((char *)rargv);
                DCP_SET_RESULT_CODE(dcp_s_dir_mod_duplicate_found);
                return TCL_ERROR;
            }
        }
        for (j = 0; j < rargc; j++) {
            if (strcmp(rargv[j], margv[0]) == 0) {
                free((char *)dargv);
                free((char *)margv);
                free((char *)rargv);
                if (eargv != NULL)
                    free((char *)eargv);
                DCP_SET_RESULT_CODE(dcp_s_dir_mod_duplicate_found);
                return TCL_ERROR;
            }
        }

        /*
         * Put the new configuration into a linked list.
         */
        status = cdsCvtFullToOpq((unsigned char *)margv[0],
                                 (dns_opq_fname *)opq_name,
                                 &namesize);
        DCP_CHECK_CDS_ERROR(status);
        namesize = CDS_OPQ_FNAME_MAX;
        enq(NEW, opq_name, (int)dnsMasterRep);
        for (i = 0; i < eargc; i++) {
            status = cdsCvtFullToOpq((unsigned char *)eargv[i],
                                     (dns_opq_fname *)opq_name,
                                     &namesize);
            if (status != CDS_SUCCESS) {
                free((char *)dargv);
                free((char *)margv);
                if (eargv != NULL)
                    free((char *)eargv);
                if (rargv != NULL)
                    free((char *)rargv);
                free_list();
                DCP_SET_MISC_ERROR(status);
            }
            namesize = CDS_OPQ_FNAME_MAX;
            enq(NEW, opq_name, (int)dnsExcludeRep);
        }
        for (i = 0; i < rargc; i++) {
            status = cdsCvtFullToOpq((unsigned char *)rargv[i],
                                     (dns_opq_fname *)opq_name,
                                     &namesize);
            if (status != CDS_SUCCESS) {
                free((char *)dargv);
                free((char *)margv);
                if (eargv != NULL)
                    free((char *)eargv);
                if (rargv != NULL)
                    free((char *)rargv);
                free_list();
                DCP_SET_MISC_ERROR(status);
            }
            namesize = CDS_OPQ_FNAME_MAX;
            enq(NEW, opq_name, (int)dnsReadRep);
        }

        /*
         * Gather the old epoch information into a linked list.
         */
        status = cdsCvtFullToOpq((unsigned char *)dargv[0],
                                 (dns_opq_fname *)opq_dirname,
                                 &dirnamesize);
        if (status != CDS_SUCCESS) {
            free((char *)dargv);
            free((char *)margv);
            if (eargv != NULL)
                free((char *)eargv);
            if (rargv != NULL)
                free((char *)rargv);
            free_list();
            DCP_SET_MISC_ERROR(status);
        }

        hp = cdsGetHandle();
        if (hp == NULL_CDS_HANDLE) {
            free((char *)dargv);
            free((char *)margv);
            if (eargv != NULL)
                free((char *)eargv);
            if (rargv != NULL)
                free((char *)rargv);
            free_list();
            DCP_SET_RESULT_CODE(dcp_s_cds_no_handle);
            return TCL_ERROR;
        }

        clear_flags(&flags, cdsOpqDNA);
        while (status == CDS_SUCCESS) {
            status = cdsReadAttrValue((dns_opq_fname *)opq_dirname,
                                      hp,
                                      (cds_attr_name_t *)dns_AttributeName(DNS_REPLICAS),
                                      cdsDir,
                                      (dns_opq_fname *)NULL,
                                      &value,
                                      (dns_cts **)NULL,
                                      (unsigned char *)NULL,
                                      (int *)NULL,
                                      &flags);
            if (status != CDS_SUCCESS) {
                if (status == CDS_EMPTY) {
                    break;
                }
                free((char *)dargv);
                free((char *)margv);
                if (eargv != NULL)
                    free((char *)eargv);
                if (rargv != NULL)
                    free((char *)rargv);
                free_list();
                cdsFreeHandle(hp);
                if (status == CDS_ERROR) {
                    DCP_SET_MISC_ERROR(flags.fsLocStat);
                }
                else {
                    return status;
                }
            }

            if (value->val_syntax == cds_ReplicaPointer) {
                rp_p = (ReplicaPointer_t *)value->val_u.val_byte.byte_p;
                rtype_p = (unsigned char *)FLD_ReplicaPointer_replicaType(rp_p);
                name_p = (unsigned char *)FLD_ReplicaPointer_CHName(rp_p);
                enq(OLD, name_p, (int)(*rtype_p));
            }
        }
        cdsFreeHandle(hp);

        /*
         * For each entry in the old epoch list, make sure there
         * is a corresponding entry somewhere in the new lists.
         */
        for (op = old_epoch_head; op != NULL; op = op->link) {
            found = FALSE;
            for (np = new_epoch_head; np != NULL; np = np->link) {
                if (dnsCmpFull(&op->ch_name, &np->ch_name) == 0) {
                    found = TRUE;
                    break;
                }
            }
            if (found == FALSE) {
                dnsCvtCDSFullToStr((dns_opq_fname *)&op->ch_name,
                                   (unsigned char *)old_name,
                                   &oldlen);
                free((char *)dargv);
                free((char *)margv);
                if (eargv != NULL)
                    free((char *)eargv);
                if (rargv != NULL)
                    free((char *)rargv);
                free_list();
                DCP_SET_CDS_ERROR_ARG(dcp_s_dir_mod_missing_new_entry, old_name);
            }
        }

        /*
         * For each entry in the new epoch list, make sure it
         * exists in the old epoch list.
         */
        for (np = new_epoch_head; np != NULL; np = np->link) {
            found = FALSE;
            for (op = old_epoch_head; op != NULL; op = op->link) {
                if (dnsCmpFull(&np->ch_name, &op->ch_name) == 0) {
                    found = TRUE;
                    break;
                }
            }
            if (found == FALSE) {
                dnsCvtCDSFullToStr((dns_opq_fname *)&np->ch_name,
                                   (unsigned char *)old_name,
                                   &oldlen);
                free((char *)dargv);
                free((char *)margv);
                if (eargv != NULL)
                    free((char *)eargv);
                if (rargv != NULL)
                    free((char *)rargv);
                free_list();
                DCP_SET_CDS_ERROR_ARG(dcp_s_dir_mod_bad_new_entry, old_name);
            }
        }

        /*
         * Get a count of the number of existing
         * clearinghouses we are dealing with.
         */
        for (ch_count = 0, op = old_epoch_head; op != NULL; ++ch_count, op = op->link) {}

        /*
         * Allocate plenty of space.
         */
        secondary_ch = (unsigned char **)malloc((ch_count + 1) * sizeof(char *));
        readonly_ch = (unsigned char **)malloc((ch_count + 1) * sizeof(char *));
        if ((secondary_ch == NULL) || (readonly_ch == NULL)) {
            free((char *)dargv);
            free((char *)margv);
            if (eargv != NULL)
                free((char *)eargv);
            if (rargv != NULL)
                free((char *)rargv);
            free_list();
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }

        /*
         * Set up the structures.
         */
        for (np = new_epoch_head, secP = secondary_ch, ronlyP = readonly_ch;
             np != NULL; np = np->link) {
            switch (np->r_type) {
                case (dnsMasterRep):
                    master_ch = (unsigned char *)&np->ch_name;
                    break;
                case (dnsSecondRep):
                    *secP++ = (unsigned char *)&np->ch_name;
                    break;
                case (dnsReadRep):
                    *ronlyP++ = (unsigned char *)&np->ch_name;
                    break;
            }
        }
        *secP = *ronlyP = NULL;

        /*
         * Make the call.
         */
        do_sec_login();
        clear_flags(&flags, cdsOpqDNA);
        status = dnsNewEpoch((dns_opq_fname *)opq_dirname,
                             (dns_opq_fname *)master_ch,
                             (dns_opq_fname **)secondary_ch,
                             (dns_opq_fname **)readonly_ch,
                             &flags);
        if (status != CDS_SUCCESS) {
            free((char *)dargv);
            free((char *)margv);
            if (eargv != NULL)
                free((char *)eargv);
            if (rargv != NULL)
                free((char *)rargv);
            free_list();
            free((char *)secondary_ch);
            free((char *)readonly_ch);
            if (status == CDS_ERROR) {
                DCP_SET_MISC_ERROR(flags.fsLocStat);
            }
            else {
                return status;
            }
        }

        free((char *)dargv);
        free((char *)margv);
        if (eargv != NULL)
            free((char *)eargv);
        if (rargv != NULL)
            free((char *)rargv);
        free_list();
        free((char *)secondary_ch);
        free((char *)readonly_ch);
    }

    return TCL_OK;
}


/*
 * FUNCTION: directory_show
 *
 * OVERVIEW:
 *
 *   directory_show - This returns a list of attributes in the 
 *   directory as an argument. This will return the directory 
 *   specific and replica specific attributes for the replica. 
 *   This takes the -replica and -clearinghouse switches to specify that 
 *   the contents of the replica are to be returned. The 
 *   clearinghouse the replica is in is specified by the -clearinghouse 
 *   switch. 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */

int
directory_show(ClientData clientData, 
               Tcl_Interp *interp,
               int argc, 
               char **argv)
{
    cds_full_name_t     dir_name;
    cds_full_name_t     clr_name;
    cds_full_name_t     *clearinghouse_value;
    cdsEntries_t        type = cdsDir;
    int                 i = 0; 
    int                 replica = 0; 
    char                *clearinghouse;
    char                *member;
    int                 sargc;
    char                **sargv;
    char                *temp;
    int                 schema;
    int                 action = ShowAttribute;
    cds_attribute_list  *list;
    
    dcp_ArgvInfo  arg_table[] = {
        {"-clearinghouse",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_cds_dir_show_clh_help},
        {"-member",DCP_ARGV_STRING,NULL,NULL,
             dcp_t_cds_dir_show_member_help}, 
        {"-replica",DCP_ARGV_CONSTANT,(char *)1,NULL,
             dcp_t_cds_dir_show_rep_help}, 
        {"-schema",DCP_ARGV_CONSTANT,(char *)1,NULL,
             dcp_t_cds_schema_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0} 
    };

    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* 
     * Initialize parsing parameters. Parser returns clearinghouse information
     * if provided on command line. 
     */
    schema = 0;
    clearinghouse = NULL;
    member = NULL;
    arg_table[0].dst = (char *)&clearinghouse;
    arg_table[1].dst = (char *)&member;
    arg_table[2].dst = (char *)&replica;
    arg_table[3].dst = (char *)&schema;

    clearinghouse_value = NULL;
    temp = NULL;

    if(dcp_ParseArgv(interp,&argc,argv,arg_table,0) != TCL_OK)
        return TCL_ERROR;

    DCP_CHECK_CDS_NO_ARG(dir);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if(schema)
        action = ShowAttrType;

    /* check for request for replica information */
    /* setup replica & clearinghouse information if specified */
    if (replica){
        if (!clearinghouse){
            DCP_SET_MISC_ERROR(dcp_s_cds_no_clearinghouse_option);
        }
    }
    if ((replica || clearinghouse) && member){
        DCP_SET_MISC_ERROR(dcp_s_cds_no_clearinghouse_option);
    }

    if (clearinghouse){
        DCP_CHECK_CDS_NAME_LEN(clearinghouse);
        strcpy((char *)clr_name.uc, clearinghouse); 
        clearinghouse_value = &clr_name;
    } 
    else if (member)
        type = cdsChildPtr;
    

    do_sec_login();
    if(member){
        if(split_combine(interp,argv[1],member,&sargc,&sargv) == TCL_ERROR)
            return TCL_ERROR;
    }
    else if (Tcl_SplitList(interp,argv[1],&sargc,&sargv) == TCL_ERROR)
        return TCL_ERROR;
    for (;sargc;sargc--,i++){
        DCP_CHECK_CDS_NAME_LEN(sargv[i]);
        strcpy((char *)dir_name.uc,sargv[i]);
        if (enum_all_attr(interp, &dir_name, 
                         clearinghouse_value, 
                         type, 
                         action, 
                         &list) != TCL_OK) {
            if(member) {
                for(i = 0; i < sargc; i++)
                    free(argv[i]);
            } 
            else {
                free((char *)sargv);
            }
            return TCL_ERROR;
        }
        if (format_all_attr(interp, list) != TCL_OK) {
            if(member) {
                for(i = 0; i < sargc; i++)
                    free(argv[i]);
            } 
            else {
                free((char *)sargv);
            }
            free_attr_value(&list);
            return TCL_ERROR;
        }
        free_attr_value(&list);
    }
    if(member) {
        for(i = 0; i < sargc; i++)
            free(argv[i]);
    } 
    else {
        free((char *)sargv);
    }
    return TCL_OK;
}


/*
 * FUNCTION: split_combine 
 *
 * OVERVIEW: This takes two tcl list entries and combines them into 
 *           one argc argv combination. The base entry is combined with 
 *           the extend entry into a path name. Therefore if each tcl list
 *           on input has three entries the argv argc combination will have
 *           nine entries.
 *
 * INPUTS:
 *   char *base - tcl list representing base of pathname
 *   char *extend - tcl list representing tail of pathname
 *
 * INPUT/OUTPUTS:
 *    Tcl_Interp *interp - interpreter to do the work.
 *
 * OUTPUTS:
 *   char *argc - number of entries generated.
 *   char ***argv - entries generated.
 *
 * RETURNS:
 *   TCL_OK - on success
 */

int split_combine(Tcl_Interp *interp,
                  char *base,
                  char *extend,
                  int *argc,
                  char ***argv)
{

    int    aargc;
    char   **aargv;
    int    bargc;
    char   **bargv;
    char   **rargv;
    int    i = 0, j, k = 0;

    if(Tcl_SplitList(interp,base,&aargc,&aargv) == TCL_ERROR)
        return TCL_ERROR;

    if(Tcl_SplitList(interp,extend,&bargc,&bargv) == TCL_ERROR)
        return TCL_ERROR;
    rargv = (char **)malloc((aargc * bargc) * sizeof(char *));

    for(;i < aargc; i++){
        for(j = 0; j < bargc; j++,k++){
            rargv[k] = malloc(strlen(aargv[i]) + strlen(bargv[j]) + 2);
            sprintf(rargv[k],"%s/%s",aargv[i],bargv[j]);
        }
    }
    *argc = k;
    *argv = rargv;
    return TCL_OK;
}


/*
 * FUNCTION: cds_disable
 *
 * OVERVIEW: Disable the specified CDS server.  This command provides
 *           compatibility with the cdscp 'disable server' command.
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 */

int cds_disable(ClientData clientData,
                Tcl_Interp *interp,
                int argc,
                char **argv)
{
    rpc_binding_handle_t  rpc_binding;
    char                  **largv;
    int                   largc;
    unsigned32            euid;
    unsigned32            rpc_status;
    unsigned32            tmp_status;
    command_status_t      status = CDS_SUCCESS;

    dcp_ArgvInfo arg_table[] = {
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_cds_no_cds_server);
    }

    /*
     * Set convenience variable.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split argument list properly.
     */
    if (Tcl_SplitList(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Only one CDS server can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_cds_only_one_server)
    }

    /*
     * Disable the specified cds server.
     */
    euid = (long)geteuid();
    rpc_binding = cds_bind_to_server(interp, largv[0], &rpc_status);
    DCP_CHECK_RPC_ERROR(rpc_status);

    /*
     * Make the call to cds.
     */
    rpc_status = dnscp_server(rpc_binding,
                              DISSRV,
                              euid,
                              NULL,
                              NULL,
                              &status);
    if (rpc_status != rpc_s_ok) {
        rpc_binding_free(&rpc_binding, &tmp_status);
        free((char *)largv);
        DCP_SET_MISC_ERROR(rpc_status);
    }
    if (status != CDS_SUCCESS) {
        rpc_binding_free(&rpc_binding, &tmp_status);
        free((char *)largv);
        DCP_SET_MISC_ERROR(status);
    }

    rpc_binding_free(&rpc_binding, &rpc_status);
    free((char *)largv);
    DCP_CHECK_RPC_ERROR(rpc_status);

    return TCL_OK;
}


/*
 * FUNCTION: cdsclient_disable
 *
 * OVERVIEW: Disable the specified CDS client.  This command provides
 *           compatibility with the cdscp 'disable clerk' command.
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 */

int cdsclient_disable(ClientData clientData,
                      Tcl_Interp *interp,
                      int argc,
                      char **argv)
{
    rpc_binding_handle_t  rpc_binding;
    char                  **largv;
    int                   largc;
    unsigned32            euid;
    unsigned32            rpc_status;
    unsigned32            tmp_status;
    command_status_t      status = CDS_SUCCESS;

    dcp_ArgvInfo arg_table[] = {
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_cds_no_cds_client);
    }

    /*
     * Set convenience variable.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split argument list properly.
     */
    if (Tcl_SplitList(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Only one CDS client can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_cds_only_one_client)
    }

    /*
     * Disable the specified cds client.
     */
    euid = (long)geteuid();
    do_sec_login();
    rpc_binding = cds_bind_to_server(interp, largv[0], &rpc_status);
    DCP_CHECK_RPC_ERROR(rpc_status);

    /*
     * Make the call to cds.
     */
    rpc_status = dnscp_clerk(rpc_binding,
                             DISCLK,
                             euid,
                             NULL,
                             NULL,
                             NULL,
                             &status);
    if (rpc_status != rpc_s_ok) {
        rpc_binding_free(&rpc_binding, &tmp_status);
        free((char *)largv);
        DCP_SET_MISC_ERROR(rpc_status);
    }
    if (status != CDS_SUCCESS) {
        rpc_binding_free(&rpc_binding, &tmp_status);
        free((char *)largv);
        DCP_SET_MISC_ERROR(status);
    }

    rpc_binding_free(&rpc_binding, &rpc_status);
    free((char *)largv);
    DCP_CHECK_RPC_ERROR(rpc_status);

    return TCL_OK;
}


/*
 * FUNCTION: cds_show
 *
 * OVERVIEW: Show information about the specified CDS server.  This
 *           provides compatibility with the cdscp 'show server'
 *           command.
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 */

int cds_show(ClientData clientData,
             Tcl_Interp *interp,
             int argc,
             char **argv)
{
    rpc_binding_handle_t  rpc_binding;
    unsigned32            euid;
    int                   i, j, result;
    char                  **largv;
    int                   largc;
    char                  tmpstr[UTC_MAX_STR_LEN];
    int                   tmpstr_len;
    char                  bufstr[CDS_STR_SNAME_MAX];
    int                   bufstr_len;
    char                  *create_time_svc;
    char                  *read_ops_svc;
    char                  *write_ops_svc;
    char                  *future_skew_svc;
    char                  *skulks_init_svc;
    char                  *skulks_comp_svc;
    char                  *times_lookup_svc;
    char                  *crucial_replicas_svc;
    char                  *child_updates_svc;
    char                  *security_failures_svc;
    char                  *known_clearinghouses_svc;
    unsigned32            rpc_status;
    unsigned32            tmp_status;
    command_status_t      status = CDS_SUCCESS;
    cprpc_attr_list_t     *cprpc_attr_list_p;
    cprpc_attr_t          *attrp;
    cprpc_attr_t          cpsrv_attrs[NUM_SERVER_ATTRS];
    FullName_t            Lname;
    int                   clh_count;
    clh_info_list_t       clh_list;
    clh_info_list_t       *clh_list_p;
    static int            ticks_per_second;
    int                   seconds, nsecs;

    dcp_ArgvInfo  arg_table[] = {
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_cds_no_cds_server);
    }

    /*
     * Set convenience variable.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split argument list properly.
     */
    if (Tcl_SplitList(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Only one CDS server can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_cds_only_one_server)
    }

    /*
     * Retrieve information about the specified CDS server.
     */
    euid = (long)geteuid();
    rpc_binding = cds_bind_to_server(interp, largv[0], &rpc_status);
    DCP_CHECK_RPC_ERROR(rpc_status);

    /*
     * Set up the clearinghouse output structure.
     */
    ZERO_bytes(&clh_list, sizeof(clh_list));
    clh_list_p = NULL;

    /*
     * Set the cds attributes to retrieve.
     */
    memset((char *)cpsrv_attrs, 0, sizeof(cpsrv_attrs));
    cpsrv_attrs[0].attr_id = DNS_CREATE_TIME;
    cpsrv_attrs[1].attr_id = DNS_FUTURE_SKEW;
    cpsrv_attrs[2].attr_id = DNS_READ_ACCESS_COUNT;
    cpsrv_attrs[3].attr_id = DNS_WRITE_ACCESS_COUNT;
    cpsrv_attrs[4].attr_id = DNS_SKULKS_INITIATED_COUNT;
    cpsrv_attrs[5].attr_id = DNS_SKULKS_COMPLETED_COUNT;
    cpsrv_attrs[6].attr_id = DNS_TREE_BROKEN_COUNT;
    cpsrv_attrs[7].attr_id = DNS_CRUCIAL_REPLICA_COUNT;
    cpsrv_attrs[8].attr_id = DNS_CHILD_POINTER_COUNT;
    cpsrv_attrs[9].attr_id = DNS_SECURITY_COUNT;
    cpsrv_attrs[10].attr_id = DNS_KNOWN_CLEARINGHOUSES;

    cprpc_attr_list_p = (cprpc_attr_list_t *)malloc(sizeof(cprpc_attr_list_t) +
                        sizeof(cprpc_attr_list_p->attr_members[0]));
    if (cprpc_attr_list_p == NULL) {
        rpc_binding_free(&rpc_binding, &tmp_status);
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    /*
     * Set up the output field headers.
     */
    create_time_svc = (char *)dce_sprintf(dcp_t_cdsclient_create_time);
    future_skew_svc = (char *)dce_sprintf(dcp_t_cdsclient_future_skew);
    read_ops_svc = (char *)dce_sprintf(dcp_t_cdsclient_read_operations);
    write_ops_svc = (char *)dce_sprintf(dcp_t_cdsclient_write_operations);
    skulks_init_svc = (char *)dce_sprintf(dcp_t_cdsclient_skulks_init);
    skulks_comp_svc = (char *)dce_sprintf(dcp_t_cdsclient_skulks_comp);
    times_lookup_svc = (char *)dce_sprintf(dcp_t_cdsclient_times_lookup);
    crucial_replicas_svc = (char *)dce_sprintf(dcp_t_cdsclient_crucial_replicas);
    child_updates_svc = (char *)dce_sprintf(dcp_t_cdsclient_child_updates);
    security_failures_svc = (char *)dce_sprintf(dcp_t_cdsclient_security_failures);
    known_clearinghouses_svc = (char *)dce_sprintf(dcp_t_cdsclient_known_clearinghouses);
    if ((create_time_svc == NULL) || (future_skew_svc == NULL) ||
        (read_ops_svc == NULL) || (write_ops_svc == NULL) ||
        (skulks_init_svc == NULL) || (skulks_comp_svc == NULL) ||
        (times_lookup_svc == NULL) || (crucial_replicas_svc == NULL) ||
        (child_updates_svc == NULL) || (security_failures_svc == NULL) ||
        (known_clearinghouses_svc == NULL)) {
        free(cprpc_attr_list_p);
        rpc_binding_free(&rpc_binding, &tmp_status);
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    /*
     * For each attribute to retrieve, set up the proper
     * structure, make the api call, and format the output.
     */
    for (i = 0; i < NUM_SERVER_ATTRS; i++) {
        cprpc_attr_list_p->attr_len = 1;
        cprpc_attr_list_p->attr_members[0] = &cpsrv_attrs[i];

        /*
         * Make the call to cds.
         */
        rpc_status = dnscp_server(rpc_binding,
                                  SHOWSRV,
                                  euid,
                                  cprpc_attr_list_p,
                                  &clh_list_p,
                                  &status);
        if ((rpc_status != rpc_s_ok) || (status != CDS_SUCCESS)) {
            free(create_time_svc);
            free(future_skew_svc);
            free(read_ops_svc);
            free(write_ops_svc);
            free(skulks_init_svc);
            free(skulks_comp_svc);
            free(times_lookup_svc);
            free(crucial_replicas_svc);
            free(child_updates_svc);
            free(security_failures_svc);
            free(known_clearinghouses_svc);
            free(cprpc_attr_list_p);
            rpc_binding_free(&rpc_binding, &tmp_status);
            free((char *)largv);
            if (rpc_status != rpc_s_ok) {
                DCP_SET_MISC_ERROR(rpc_status);
            }
            else {
                DCP_SET_MISC_ERROR(status);
            }
        }

        attrp = cprpc_attr_list_p->attr_members[0];
        switch (i) {
            case 0:
                result = utc_asclocaltime(tmpstr,
                                          UTC_MAX_STR_LEN,
                                          (utc_t *)&attrp->value.av_val.av_utctime);
                if (result != 0) {
                    free(create_time_svc);
                    free(future_skew_svc);
                    free(read_ops_svc);
                    free(write_ops_svc);
                    free(skulks_init_svc);
                    free(skulks_comp_svc);
                    free(times_lookup_svc);
                    free(crucial_replicas_svc);
                    free(child_updates_svc);
                    free(security_failures_svc);
                    free(known_clearinghouses_svc);
                    free(cprpc_attr_list_p);
                    rpc_binding_free(&rpc_binding, &tmp_status);
                    free((char *)largv);
                    DCP_SET_MISC_ERROR(dcp_s_clock_get_local_time_fail);
                }
                Tcl_AppendResult(interp, "{", create_time_svc, " ", tmpstr, "}\n", NULL);
                break;
            case 1:
                ticks_per_second = 10 * 1000 * 1000;
                dns_ediv(&ticks_per_second,
                         (time_quad_t *)&attrp->value.av_val.av_time_quad,
                         &seconds,
                         &nsecs);
                sprintf(tmpstr, "%d", seconds);
                Tcl_AppendResult(interp, "{", future_skew_svc," ", tmpstr, "}\n", NULL);
                break;
            case 2:
                translate_counter64(attrp, tmpstr);
                Tcl_AppendResult(interp, "{", read_ops_svc, " ", tmpstr, "}\n", NULL);
                break;
            case 3:
                translate_counter64(attrp, tmpstr);
                Tcl_AppendResult(interp, "{", write_ops_svc, " ", tmpstr, "}\n", NULL);
                break;
            case 4:
                translate_counter64(attrp, tmpstr);
                Tcl_AppendResult(interp, "{", skulks_init_svc, " ", tmpstr, "}\n", NULL);
                break;
            case 5:
                translate_counter64(attrp, tmpstr);
                Tcl_AppendResult(interp, "{", skulks_comp_svc, " ", tmpstr, "}\n", NULL);
                break;
            case 6:
                translate_counter64(attrp, tmpstr);
                Tcl_AppendResult(interp, "{", times_lookup_svc, " ", tmpstr, "}\n", NULL);
                break;
            case 7:
                translate_counter64(attrp, tmpstr);
                Tcl_AppendResult(interp, "{", crucial_replicas_svc, " ", tmpstr, "}\n", NULL);
                break;
            case 8:
                translate_counter64(attrp, tmpstr);
                Tcl_AppendResult(interp, "{", child_updates_svc, " ", tmpstr, "}\n", NULL);
                break;
            case 9:
                translate_counter64(attrp, tmpstr);
                Tcl_AppendResult(interp, "{", security_failures_svc, " ", tmpstr, "}\n", NULL);
                break;
            case 10:
                Tcl_AppendResult(interp, "{", known_clearinghouses_svc, NULL);
                clh_count = clh_list_p->clh_count;
                if (clh_count == 0) {
                    Tcl_AppendResult(interp, " }", NULL);
                }
                else {
                    if (clh_count == 1) {
                        Tcl_AppendResult(interp, " ", NULL);
                    }
                    else if (clh_count > 1) {
                        Tcl_AppendResult(interp, "\n {", NULL);
                    }
                    for (j = 0; j < clh_count; j++) {
                        cdsImport_FullName(NULL,
                                           clh_list_p->clh_members[j]->clh_name_p,
                                           &Lname,
                                           0);
                        bufstr_len = sizeof(bufstr);
                        status = dnsCvtCDSFullToStr((dns_full_name_t *)&Lname,
                                                    (unsigned char *)bufstr,
                                                    &bufstr_len);
                        if (status != CDS_SUCCESS) {
                            free(create_time_svc);
                            free(future_skew_svc);
                            free(read_ops_svc);
                            free(write_ops_svc);
                            free(skulks_init_svc);
                            free(skulks_comp_svc);
                            free(times_lookup_svc);
                            free(crucial_replicas_svc);
                            free(child_updates_svc);
                            free(security_failures_svc);
                            free(known_clearinghouses_svc);
                            free(cprpc_attr_list_p);
                            rpc_binding_free(&rpc_binding, &tmp_status);
                            free((char *)largv);
                            DCP_SET_MISC_ERROR(status);
                        }
                        Tcl_AppendResult(interp, bufstr, NULL);
                        if ((clh_count > 1) && (j != (clh_count - 1))) {
                            Tcl_AppendResult(interp, "\n  ", NULL);
                        }
                        else if ((clh_count > 1) && (j == (clh_count -1))) {
                            Tcl_AppendResult(interp, "}", NULL);
                        }
                    }
                    Tcl_AppendResult(interp, "}", NULL);
                }
                break;
            default:
                /*
                 * Should not be here, so it's some kind of internal problem.
                 */
                free(create_time_svc);
                free(future_skew_svc);
                free(read_ops_svc);
                free(write_ops_svc);
                free(skulks_init_svc);
                free(skulks_comp_svc);
                free(times_lookup_svc);
                free(crucial_replicas_svc);
                free(child_updates_svc);
                free(security_failures_svc);
                free(known_clearinghouses_svc);
                free(cprpc_attr_list_p);
                rpc_binding_free(&rpc_binding, &tmp_status);
                free((char *)largv);
                DCP_SET_MISC_ERROR(dcp_s_cds_internal_error);
        }
    }

    free(create_time_svc);
    free(future_skew_svc);
    free(read_ops_svc);
    free(write_ops_svc);
    free(skulks_init_svc);
    free(skulks_comp_svc);
    free(times_lookup_svc);
    free(crucial_replicas_svc);
    free(child_updates_svc);
    free(security_failures_svc);
    free(known_clearinghouses_svc);
    free(cprpc_attr_list_p);
    rpc_binding_free(&rpc_binding, &rpc_status);
    free((char *)largv);
    DCP_CHECK_RPC_ERROR(rpc_status);

    return TCL_OK;
}


/*
 * FUNCTION: cdsclient_show
 *
 * OVERVIEW: Show information about the specified CDS client.  This
 *           provides compatibility with the cdscp 'show clerk'
 *           command.
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 */

int cdsclient_show(ClientData clientData,
                   Tcl_Interp *interp,
                   int argc,
                   char **argv)
{
    rpc_binding_handle_t  rpc_binding;
    unsigned32            euid;
    int                   i, result;
    char                  **largv;
    int                   largc;
    char                  tmpstr[UTC_MAX_STR_LEN];
    int                   tmpstr_len;
    char                  *create_time_svc;
    char                  *protocol_errors_svc;
    char                  *auth_count_svc;
    char                  *read_ops_svc;
    char                  *cache_hits_svc;
    char                  *cache_bypass_svc;
    char                  *write_ops_svc;
    char                  *misc_ops_svc;
    unsigned32            rpc_status = 0;
    unsigned32            tmp_status = 0;
    command_status_t      status = CDS_SUCCESS;
    cprpc_attr_list_t     *cprpc_attr_list_p;
    cprpc_attr_t          *attrp;
    cprpc_attr_t          cprpc_attrs[NUM_CLERK_ATTRS];

    dcp_ArgvInfo arg_table[] = {
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_cds_no_cds_client);
    }

    /*
     * Set convenience variable.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split argument list properly.
     */
    if (Tcl_SplitList(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Only one CDS client can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_cds_only_one_client)
    }

    /*
     * Retrieve information about the specified CDS client.
     */
    euid = (long)geteuid();
    do_sec_login();
    rpc_binding = cds_bind_to_server(interp, largv[0], &rpc_status);
    DCP_CHECK_RPC_ERROR(rpc_status);

    /*
     * Set the cds attributes to retrieve.
     */
    memset((char *)cprpc_attrs, 0, sizeof(cprpc_attrs));
    cprpc_attrs[0].attr_id = CDS_CLERK_CREATE_TIME;
    cprpc_attrs[1].attr_id = CDS_CLERK_PROTOCOL_ERROR_COUNT;
    cprpc_attrs[2].attr_id = CDS_CLERK_AUTHENTICATION_COUNT;
    cprpc_attrs[3].attr_id = CDS_CLERK_READ_OPS_COUNT;
    cprpc_attrs[4].attr_id = CDS_CLERK_CACHE_HITS_COUNT;
    cprpc_attrs[5].attr_id = CDS_CLERK_CACHE_BYPASS_COUNT;
    cprpc_attrs[6].attr_id = CDS_CLERK_WRITE_OPS_COUNT;
    cprpc_attrs[7].attr_id = CDS_CLERK_MISC_OPS_COUNT;

    cprpc_attr_list_p = (cprpc_attr_list_t *)malloc(sizeof(cprpc_attr_list_t) +
                        sizeof(cprpc_attr_list_p->attr_members[0]));
    if (cprpc_attr_list_p == NULL) {
        rpc_binding_free(&rpc_binding, &tmp_status);
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    /*
     * Set up the output field headers.
     */
    create_time_svc = (char *)dce_sprintf(dcp_t_cdsclient_create_time);
    protocol_errors_svc = (char *)dce_sprintf(dcp_t_cdsclient_protocol_errors);
    auth_count_svc = (char *)dce_sprintf(dcp_t_cdsclient_authentication_count);
    read_ops_svc = (char *)dce_sprintf(dcp_t_cdsclient_read_operations);
    cache_hits_svc = (char *)dce_sprintf(dcp_t_cdsclient_cache_hits);
    cache_bypass_svc = (char *)dce_sprintf(dcp_t_cdsclient_cache_bypass);
    write_ops_svc = (char *)dce_sprintf(dcp_t_cdsclient_write_operations);
    misc_ops_svc = (char *)dce_sprintf(dcp_t_cdsclient_misc_operations);
    if ((create_time_svc == NULL) || (protocol_errors_svc == NULL) ||
        (auth_count_svc == NULL) || (read_ops_svc == NULL) ||
        (cache_hits_svc == NULL) || (cache_bypass_svc == NULL) ||
        (write_ops_svc == NULL) || (misc_ops_svc == NULL)) {
        free(cprpc_attr_list_p);
        rpc_binding_free(&rpc_binding, &tmp_status);
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    /*
     * For each attribute to retrieve, set up the proper
     * structure, make the api call, and format the output.
     */
    for (i = 0; i < NUM_CLERK_ATTRS; i++) {
        cprpc_attr_list_p->attr_len = 1;
        cprpc_attr_list_p->attr_members[0] = &cprpc_attrs[i];

        /*
         * Make the call to cds.
         */
        rpc_status = dnscp_clerk(rpc_binding,
                                 SHOWCLK,
                                 euid,
                                 cprpc_attr_list_p,
                                 NULL,
                                 NULL,
                                 &status);
        if ((rpc_status != rpc_s_ok) || (status != CDS_SUCCESS)) {
            free(create_time_svc);
            free(protocol_errors_svc);
            free(auth_count_svc);
            free(read_ops_svc);
            free(cache_hits_svc);
            free(cache_bypass_svc);
            free(write_ops_svc);
            free(misc_ops_svc);
            free(cprpc_attr_list_p);
            rpc_binding_free(&rpc_binding, &tmp_status);
            free((char *)largv);
            if (rpc_status != rpc_s_ok) {
                DCP_SET_MISC_ERROR(rpc_status);
            }
            else {
                DCP_SET_MISC_ERROR(status);
            }
        }

        attrp = cprpc_attr_list_p->attr_members[0];
        if (i == 0) {
            result = utc_asclocaltime(tmpstr,
                                      UTC_MAX_STR_LEN,
                                      (utc_t *)&attrp->value.av_val.av_utctime);
            if (result != 0) {
                free(create_time_svc);
                free(protocol_errors_svc);
                free(auth_count_svc);
                free(read_ops_svc);
                free(cache_hits_svc);
                free(cache_bypass_svc);
                free(write_ops_svc);
                free(misc_ops_svc);
                free(cprpc_attr_list_p);
                rpc_binding_free(&rpc_binding, &tmp_status);
                free((char *)largv);
                DCP_SET_MISC_ERROR(dcp_s_clock_get_local_time_fail);
            }
        }
        else {
            sprintf(tmpstr, "%d", attrp->value.av_val.av_counter);
        }
        switch (i) {
            case 0:
                Tcl_AppendResult(interp, "{", create_time_svc, " ", NULL);
                break;
            case 1:
                Tcl_AppendResult(interp, "{", protocol_errors_svc," ", NULL);
                break;
            case 2:
                Tcl_AppendResult(interp, "{", auth_count_svc, " ", NULL);
                break;
            case 3:
                Tcl_AppendResult(interp, "{", read_ops_svc, " ", NULL);
                break;
            case 4:
                Tcl_AppendResult(interp, "{", cache_hits_svc, " ", NULL);
                break;
            case 5:
                Tcl_AppendResult(interp, "{", cache_bypass_svc, " ", NULL);
                break;
            case 6:
                Tcl_AppendResult(interp, "{", write_ops_svc, " ", NULL);
                break;
            case 7:
                Tcl_AppendResult(interp, "{", misc_ops_svc, " ", NULL);
                break;
            default:
                /*
                 * Should not be here, so it's some kind of internal problem.
                 */
                free(create_time_svc);
                free(protocol_errors_svc);
                free(auth_count_svc);
                free(read_ops_svc);
                free(cache_hits_svc);
                free(cache_bypass_svc);
                free(write_ops_svc);
                free(misc_ops_svc);
                free(cprpc_attr_list_p);
                rpc_binding_free(&rpc_binding, &tmp_status);
                free((char *)largv);
                DCP_SET_MISC_ERROR(dcp_s_cds_internal_error);
        }
        Tcl_AppendResult(interp, tmpstr, "}", NULL);
        if (i != (NUM_CLERK_ATTRS - 1)) {
            Tcl_AppendResult(interp, "\n", NULL);
        }
    }

    free(create_time_svc);
    free(protocol_errors_svc);
    free(auth_count_svc);
    free(read_ops_svc);
    free(cache_hits_svc);
    free(cache_bypass_svc);
    free(write_ops_svc);
    free(misc_ops_svc);
    free(cprpc_attr_list_p);
    rpc_binding_free(&rpc_binding, &rpc_status);
    free((char *)largv);
    DCP_CHECK_RPC_ERROR(rpc_status);

    return TCL_OK;
}


/*
 * FUNCTION: enq
 *
 * OVERVIEW: Put a name/type record on one of the epoch queues.
 *
 */

static int enq(int which_queue,
               unsigned char *name,
               int type)
{
    epochDesc  *blah;

    blah = (epochDesc *)malloc(sizeof(epochDesc));
    blah->r_type = type;
    blah->link = NULL;
    COPY_FullName(name, (unsigned char *)&blah->ch_name);
    if (which_queue == OLD) {
        old_epoch_tail->link = blah;
        old_epoch_tail = blah;
    }
    else {
        new_epoch_tail->link = blah;
        new_epoch_tail = blah;
    }
    return TCL_OK;
}


/*
 * FUNCTION: free_list
 *
 * OVERVIEW: Free the link lists created by the enq calls.
 *
 */

static int free_list(void)
{
    epochDesc  *p, *nextp;

    if (old_epoch_head) {
        for (p = old_epoch_head, nextp = p->link; p != NULL; p = nextp) {
            nextp = p->link;
            free((char *)p);
        }
    }
    old_epoch_head = NULL;
    old_epoch_tail = (epochDesc *)&old_epoch_head;

    if (new_epoch_head) {
        for (p = new_epoch_head, nextp = p->link; p != NULL; p = nextp) {
            nextp = p->link;
            free((char *)p);
        }
    }
    new_epoch_head = NULL;
    new_epoch_tail = (epochDesc *)&new_epoch_head;
    return TCL_OK;
}


/*
 * FUNCTION: cds_bind_to_server
 *
 * OVERVIEW: Get a binding to the specified cds entity,
 *           be it a server or a client.
 * INPUTS:
 *    char *user_site: the namespace entry of the cds server
 *                     for binding.
 *
 */

handle_t cds_bind_to_server(Tcl_Interp *interp,
                            char *user_site,
                            unsigned32 *status)
{
    char                  *site = NULL;
    char                  *tmp = NULL;
    rpc_binding_handle_t  handle = NULL;
    rpc_ns_handle_t       import_context;
    error_status_t        rpc_status = 0;

    /*
     * Expand the name to include the complete cell name.
     */
    tmp = expand_cell_name(user_site, &rpc_status);
    if ((tmp != NULL) && (rpc_status == rpc_s_ok)) {
        site = tmp;
    }
    else {
        site = dce_strdup(user_site);
    }

    /*
     * Use the expanded namespace entry to get an import context.
     */
    rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce,
                                (unsigned_char_t *)site,
                                NULL,
                                NULL,
                                &import_context,
                                &rpc_status);
    if (rpc_status != rpc_s_ok) {
        free(site);
        *status = rpc_status;
        return NULL;
    }

    rpc_ns_binding_import_next(import_context, 
                               &handle, 
                               &rpc_status);
    if (rpc_status != rpc_s_ok) {
        free(site);
        *status = rpc_status;
        return NULL;
    }

    rpc_ns_binding_import_done(&import_context, &rpc_status);
    if (rpc_status != rpc_s_ok) {
        free(site);
        *status = rpc_status;
        return NULL;
    }

    rpc_binding_set_auth_info(handle,
                              NULL,
                              rpc_c_authn_level_default,
                              rpc_c_authn_dce_secret,
                              (rpc_auth_identity_handle_t)login_context_h,
                              rpc_c_authz_dce,
                              &rpc_status);
    if (rpc_status != rpc_s_ok) {
        free(site);
        *status = rpc_status;
        return NULL;
    }

    free(site);
    *status = rpc_status;
    return((rpc_binding_handle_t)handle);
}
