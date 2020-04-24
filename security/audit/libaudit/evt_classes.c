/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: evt_classes.c,v $
 * Revision 1.1.6.2  1996/10/15  20:47:46  arvind
 * 	fix memory leak during startup: close directory when we're done with
 * 	it.
 * 	[1996/10/11  00:09 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:26 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:22  marty  1.1.4.2]
 *
 * Revision 1.1.6.1  1996/07/08  18:31:26  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:26 UTC  arvind  /main/arvind_audit122/1]
 * 
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:22  marty  1.1.4.2]
 * 
 * Revision 1.1.4.2  1996/02/18  00:06:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:45  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:41:41  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/14  18:42 UTC  jrr
 * 	Change dce_printf() to dce_svc_printf().
 * 	[1995/07/14  18:37 UTC  jrr  /main/jrr_mothra_pure/1]
 * 
 * Revision 1.1.2.5  1994/04/28  13:50:33  weisz
 * 	CR 10469: initialize aud_g_cli_num_of_ecs to 0, in input_evt_table
 * 	[1994/04/28  13:47:03  weisz]
 * 
 * Revision 1.1.2.4  1994/03/11  22:48:40  luan
 * 	CR 10115: Added functions for maintaining an event class table.
 * 	CR 9775 9776: Changed debug messages to use dce_svc_printf() or DCE_SVC_DEBUG.
 * 	[1994/03/11  22:27:04  luan]
 * 
 * Revision 1.1.2.3  1994/02/22  16:47:32  luan
 * 	CR 9775 9776 9860:  Defined DCE_AUD_DEBUG in audit.h.  Removed
 * 	debug_mode and PRE_BETA_DEBUG or replaced it with DCE_DEBUG.
 * 	[1994/02/22  16:41:52  luan]
 * 
 * Revision 1.1.2.2  1994/01/25  04:59:41  luan
 * 	CR 9745 9746: File path names consolidated.
 * 	Hard-coded path names replaced with macros
 * 	which generate names dynamically.
 * 	[1994/01/25  04:13:47  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:29  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:40  zeliff]
 * 
 * $EndLog$
 */

/*
** evt_classes.c
** 
** When used by audit clients:
** Parses event class files to initialize the in-core event table.
**
** When used by the audit control program:
** Parses event class files to intiialize the in-core event-class table.
*/

#include <evtp.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>

#include <dce/audit.h>
#include <dce/dceaudmsg.h>
#include <dce/dceaudmac.h>
#include <dce/dceaudsvc.h>
#include <sec_aud_evts.h>
#include <dmn_aud_evtsp.h>

#define LINE_BUF_SIZE	2048	/* for both input and output lines */
#define NUM_BUF_SIZE	32	/* for numbers                     */
#define FILENAME_SIZE	512	/* for event class dir and files   */

enum ec_file_parse_mode { evt_table_input, ec_table_input };
enum ec_file_parse_mode parse_mode;

evt_t		*evt_table;
ec_t		*ec_table = NULL;
unsigned32	 aud_g_cli_num_of_ecs = 0;
unsigned32	*aud_g_cli_evt_classes = NULL;

static 	int	 lineno;	/* line number in current ec file */
static  FILE 	*file;		/* current ec file                */
static	char	*line;		/* buffer for reading a line      */
static	int	 line_len;	/* current length of a line       */

static	unsigned32	evt_class_no;
static	boolean32	ecn_assigned = FALSE;
static	boolean32	ecn_added = FALSE;
static	char		fname[FILENAME_SIZE];


typedef enum {
    Ecn,	    /* Event Class Number */
    Sep   	    /* Servers' Events Prefixes */
} Parse_Operation_t;


/*
 * The parseKeywords table is searched when deciding if any operation is 
 * needed for a line that is being parsed.
 */
static struct {
    char    	  *name;    	/* Name of keyword */
    int	    	  op;	    	/* Operation */
} parseKeywords[] = {
{ "ECN",	 Ecn},
{ "SEP",	 Sep}
};

static void
init_evt_table ()
{
    int i;

    evt_table = (evt_t *) malloc(aud_g_cli_num_of_evts * sizeof(evt_t));
    if (evt_table == NULL) {
        dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, aud_g_cli_num_of_evts * sizeof(evt_t));
        return;
    }
    for (i = 0 ; i < aud_g_cli_num_of_evts; i++) {
	evt_table[i].evt_number = aud_g_cli_first_evt_number + i;
	evt_table[i].evt_class_cnt = 0;
	evt_table[i].evt_classes = NULL;
    }
    return;
}

void
free_evt_table ()
{
    int i;

    for (i = 0 ; i < aud_g_cli_num_of_evts; i++) {
	if (evt_table[i].evt_classes != NULL) {
	    free(evt_table[i].evt_classes);
	}
    }
    free(evt_table);
    return;
}

void
free_ec_table()
{
    ec_t     * tmp_table_p;
    ec_evt_t * tmp_evt_p;

    while (ec_table != NULL) {
	tmp_table_p = ec_table->next;
	while (ec_table->evts != NULL) {
	    tmp_evt_p = ec_table->evts->next;
	    free(ec_table->evts);
	    ec_table->evts = tmp_evt_p;
	}
	free(ec_table);
	ec_table = tmp_table_p;
    }
    return;
}

static void
add_ec_to_ec_table()
{
    ec_t * tmp_table_p;

    if (ec_table == NULL) {
	ec_table = (ec_t *) malloc(sizeof(ec_t));
        if (ec_table == NULL) {
            dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, sizeof(ec_t));
            return;
        }
	ec_table->next = NULL;
    } else {
	tmp_table_p = (ec_t *) malloc(sizeof(ec_t));
        if (tmp_table_p == NULL) {
            dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, sizeof(ec_t));
            return;
        }
	tmp_table_p->next = ec_table;
	ec_table = tmp_table_p;
    }
    ec_table->ec_name = (char *) malloc( strlen(fname + strlen(EVENT_CLASS_DIR)) + 1);
    if (ec_table->ec_name == NULL) {
        dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, strlen(fname+strlen(EVENT_CLASS_DIR))+1);
        return;
    }
    strcpy(ec_table->ec_name, fname + strlen(EVENT_CLASS_DIR));
    ec_table->ec_number = evt_class_no;
    ec_table->evts = NULL;
    return;
}

void
add_evt_to_ec_table
(
  unsigned32	evt_number
)
{
    ec_evt_t *new_evt_p, *evt_p1, *evt_p2;

    new_evt_p = (ec_evt_t *) malloc( sizeof(ec_evt_t) );
    if (new_evt_p == NULL) {
        dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, sizeof(ec_evt_t));
        return;
    }
    new_evt_p->evt_number = evt_number;

    if (ec_table->evts == NULL) { /* empty list, insert in the beginning */
	ec_table->evts = new_evt_p;
        new_evt_p->next = NULL;
	return;
    } 

    if (evt_number <= ec_table->evts->evt_number) {
	if (evt_number != ec_table->evts->evt_number) {
	    new_evt_p->next = ec_table->evts;
	    ec_table->evts = new_evt_p;
	}
	return;
    }
    
    evt_p1 = ec_table->evts;
    evt_p2 = evt_p1->next;

    while (1) {  /* looking for the place to insert the new evt number  */
	if (evt_p2 == NULL) {  /* hit end of the list */
	    new_evt_p->next = NULL;
	    evt_p1->next = new_evt_p;
	    return;
	} 
	if (evt_p2->evt_number == evt_number) return;  /* the number exists */
	if (evt_p2->evt_number > evt_number) { /* this is the place to insert */
	    evt_p1->next = new_evt_p;
	    new_evt_p->next = evt_p2;
	    return;
	}
	evt_p1 = evt_p2;
	evt_p2 = evt_p2->next;
    }
    return;
}

void
add_evt_to_evt_table
(
  unsigned32	evt_number
)
{
    int         num_ec;
    unsigned32 *old_ec_ptr;
    unsigned32 *new_ec_ptr;
    int         i;

    if ( evt_number >= aud_g_cli_first_evt_number && 
	 evt_number < aud_g_cli_first_evt_number+aud_g_cli_num_of_evts) {
#ifdef DCE_AUD_DEBUG
        DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
    	    "add_evt_to_evt_table: event %lx belongs to this server\n", evt_number));
#endif
	i = evt_number - aud_g_cli_first_evt_number;
	if ((num_ec = evt_table[i].evt_class_cnt) > 0) {
	    old_ec_ptr = evt_table[i].evt_classes;
	    new_ec_ptr = (unsigned32 *) malloc(sizeof(unsigned32) * (num_ec+1));
            if (new_ec_ptr == NULL) {
                dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, sizeof(unsigned32)*(num_ec+1));
                return;
            }
	    memcpy((char *) new_ec_ptr, (char *) old_ec_ptr, sizeof(unsigned32) * num_ec); 
	    *(new_ec_ptr + num_ec) = evt_class_no;
	    evt_table[i].evt_classes = new_ec_ptr;
	    evt_table[i].evt_class_cnt++;
	    free(old_ec_ptr);
	} else { 
	    new_ec_ptr = (unsigned32 *) malloc(sizeof(unsigned32));
            if (new_ec_ptr == NULL) {
                dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, sizeof(unsigned32));
                return;
            }
	    *new_ec_ptr = evt_class_no;
	    evt_table[i].evt_class_cnt = 1;
	    evt_table[i].evt_classes = new_ec_ptr;
	}
	if (!ecn_added) {
	    if (aud_g_cli_num_of_ecs == 0) {
		aud_g_cli_evt_classes = (unsigned32 *) malloc(sizeof(unsigned32));
                if (aud_g_cli_evt_classes == NULL) {
                    dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, sizeof(unsigned32));
                    return;
                }
		*aud_g_cli_evt_classes = evt_class_no;
	    } else {
	        old_ec_ptr = aud_g_cli_evt_classes;
	        new_ec_ptr = (unsigned32 *) 
		    malloc(sizeof(unsigned32) * (aud_g_cli_num_of_ecs + 1));
                if (new_ec_ptr == NULL) {
                    dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, sizeof(unsigned32)*(aud_g_cli_num_of_ecs+1));
                    return;
                }
	    	memcpy((char *) new_ec_ptr, (char *) old_ec_ptr, 
		   	sizeof(unsigned32) * aud_g_cli_num_of_ecs); 
	    	*(new_ec_ptr + aud_g_cli_num_of_ecs) = evt_class_no;
	        aud_g_cli_evt_classes = new_ec_ptr;
	        free(old_ec_ptr);
	    }
            aud_g_cli_num_of_ecs++;
	    ecn_added = TRUE;
	}
    }
    return;
}

void
get_event_number_from_name
(
  char * evt_name,
  unsigned32 evt_number
)
{
    evt_number = 0;
    /* Events for Security Server */
    if (strcmp(evt_name, "AS_Request") == 0)
        evt_number = AS_Request;
    if (strcmp(evt_name, "TGS_TicketReq") == 0)
        evt_number = TGS_TicketReq;
    if (strcmp(evt_name, "TGS_RenewReq") == 0)
        evt_number = TGS_RenewReq;
    if (strcmp(evt_name, "TGS_ValidateReq") == 0)
        evt_number = TGS_ValidateReq;
    if (strcmp(evt_name, "ACL_Lookup") == 0)
        evt_number = ACL_Lookup;
    if (strcmp(evt_name, "ACL_Replace") == 0)
        evt_number = ACL_Replace;
    if (strcmp(evt_name, "ACL_GetAccess") == 0)
        evt_number = ACL_GetAccess;
    if (strcmp(evt_name, "ACL_TestAccess") == 0)
        evt_number = ACL_TestAccess;
    if (strcmp(evt_name, "ACL_TestOnBeHalf") == 0)
        evt_number = ACL_TestOnBehalf;
    if (strcmp(evt_name, "ACL_GetMgrTypes") == 0)
        evt_number = ACL_GetMgrTypes;
    if (strcmp(evt_name, "ACL_GetReferral") == 0)
        evt_number = ACL_GetReferral;
    if (strcmp(evt_name, "PRIV_GetPtgt") == 0)
        evt_number = PRIV_GetPtgt;
    if (strcmp(evt_name, "ACCT_Add") == 0)
        evt_number = ACCT_Add;
    if (strcmp(evt_name, "ACCT_Delete") == 0)
        evt_number = ACCT_Delete;
    if (strcmp(evt_name, "ACCT_Rename") == 0)
        evt_number = ACCT_Rename;
    if (strcmp(evt_name, "ACCT_Lookup") == 0)
        evt_number = ACCT_Lookup;
    if (strcmp(evt_name, "ACCT_Replace") == 0)
        evt_number = ACCT_Replace;
    if (strcmp(evt_name, "ACCT_GetProjlist") == 0)
        evt_number = ACCT_GetProjlist;
    if (strcmp(evt_name, "LOGIN_GetInfo") == 0)
        evt_number = LOGIN_GetInfo;
    if (strcmp(evt_name, "PGO_Add") == 0)
        evt_number = PGO_Add;
    if (strcmp(evt_name, "PGO_Delete") == 0)
        evt_number = PGO_Delete;
    if (strcmp(evt_name, "PGO_Replace") == 0)
        evt_number = PGO_Replace;
    if (strcmp(evt_name, "PGO_Rename") == 0)
        evt_number = PGO_Rename;
    if (strcmp(evt_name, "PGO_Get") == 0)
        evt_number = PGO_Get;
    if (strcmp(evt_name, "PGO_KeyTransfer") == 0)
        evt_number = PGO_KeyTransfer;
    if (strcmp(evt_name, "PGO_AddMember") == 0)
        evt_number = PGO_AddMember;
    if (strcmp(evt_name, "PGO_DeleteMember") == 0)
        evt_number = PGO_DeleteMember;
    if (strcmp(evt_name, "PGO_IsMember") == 0)
        evt_number = PGO_IsMember;
    if (strcmp(evt_name, "PGO_GetMembers") == 0)
        evt_number = PGO_GetMembers;
    if (strcmp(evt_name, "PROP_GetInfo") == 0)
        evt_number = PROP_GetInfo;
    if (strcmp(evt_name, "PROP_SetInfo") == 0)
        evt_number = PROP_SetInfo;
    if (strcmp(evt_name, "POLICY_GetInfo") == 0)
        evt_number = POLICY_GetInfo;
    if (strcmp(evt_name, "POLICY_SetInfo") == 0)
        evt_number = POLICY_SetInfo;
    if (strcmp(evt_name, "AUTHPOLICY_GetInfo") == 0)
        evt_number = AUTHPOLICY_GetInfo;
    if (strcmp(evt_name, "AUTHPOLICY_SetInfo") == 0)
        evt_number = AUTHPOLICY_SetInfo;
    if (strcmp(evt_name, "REPADMIN_Stop") == 0)
        evt_number = REPADMIN_Stop;
    if (strcmp(evt_name, "REPADMIN_Maint") == 0)
        evt_number = REPADMIN_Maint;
    if (strcmp(evt_name, "REPADMIN_Mkey") == 0)
        evt_number = REPADMIN_Mkey;
    if (strcmp(evt_name, "REPADMIN_Destroy") == 0)
        evt_number = REPADMIN_Destroy;
    if (strcmp(evt_name, "REPADMIN_Init") == 0)
        evt_number = REPADMIN_Init;
    if (strcmp(evt_name, "REPADMIN_SetSwRev") == 0)
        evt_number = REPADMIN_SetSwRev;
    if (strcmp(evt_name, "SECID_ParseName") == 0)
        evt_number = SECID_ParseName;
    if (strcmp(evt_name, "SECID_GenName") == 0)
        evt_number = SECID_GenName;
    if (strcmp(evt_name, "ERA_Update") == 0)
        evt_number = ERA_Update;
    if (strcmp(evt_name, "ERA_Delete") == 0)
        evt_number = ERA_Delete;
    if (strcmp(evt_name, "ERA_TestUpdate") == 0)
        evt_number = ERA_TestUpdate;
    if (strcmp(evt_name, "ERA_LookupById") == 0)
        evt_number = ERA_LookupById;
    if (strcmp(evt_name, "ERA_LookupNoExpand") == 0)
        evt_number = ERA_LookupNoExpand;
    if (strcmp(evt_name, "ERA_LookupByName") == 0)
        evt_number = ERA_LookupByName;
    if (strcmp(evt_name, "ERA_SchemaCreate") == 0)
        evt_number = ERA_SchemaCreate;
    if (strcmp(evt_name, "ERA_SchemaDelete") == 0)
        evt_number = ERA_SchemaDelete;
    if (strcmp(evt_name, "ERA_SchemaUpdate") == 0)
        evt_number = ERA_SchemaUpdate;
    if (strcmp(evt_name, "ERA_SchemaLookupId") == 0)
        evt_number = ERA_SchemaLookupId;
    if (strcmp(evt_name, "ERA_SchemaLookupName") == 0)
        evt_number = ERA_SchemaLookupName;
    if (strcmp(evt_name, "PRIV_GetEptgt") == 0)
        evt_number = PRIV_GetEptgt;
    if (strcmp(evt_name, "PRIV_BecomeDelegate") == 0)
        evt_number = PRIV_BecomeDelegate;
    if (strcmp(evt_name, "PRIV_BecomeImpersonator") == 0)
        evt_number = PRIV_BecomeImpersonator;
    /* Events for Audit Server */
    if (strcmp(evt_name, "EVT_DELETE_FILTER") == 0)
        evt_number = EVT_DELETE_FILTER;
    if (strcmp(evt_name, "EVT_SHOW_FILTER") == 0)
        evt_number = EVT_SHOW_FILTER;
    if (strcmp(evt_name, "EVT_LIST_FILTER") == 0)
        evt_number = EVT_LIST_FILTER;
    if (strcmp(evt_name, "EVT_ADD_FILTER") == 0)
        evt_number = EVT_ADD_FILTER;
    if (strcmp(evt_name, "EVT_REMOVE_FILTER") == 0)
        evt_number = EVT_REMOVE_FILTER;
    if (strcmp(evt_name, "EVT_MODIFY_SSTRATEGY") == 0)
        evt_number = EVT_MODIFY_SSTRATEGY;
    if (strcmp(evt_name, "EVT_MODIFY_STATE") == 0)
        evt_number = EVT_MODIFY_STATE;
    if (strcmp(evt_name, "EVT_REWIND") == 0)
        evt_number = EVT_REWIND;
    if (strcmp(evt_name, "EVT_STOP") == 0)
        evt_number = EVT_STOP;
    if (strcmp(evt_name, "EVT_SHOW_SSTRATEGY") == 0)
        evt_number = EVT_SHOW_SSTRATEGY;
    if (strcmp(evt_name, "EVT_SHOW_STATE") == 0)
        evt_number = EVT_SHOW_STATE;
    return;
}

void
get_event_name_from_number
(
  unsigned32 evt_number,
  char * evt_name
)
{
    switch(evt_number) {
      /* Events for Security Server */
      case AS_Request:
        strcpy(evt_name, "AS_Request");
        break;
      case TGS_TicketReq:
        strcpy(evt_name, "TGS_TicketReq");
        break;
      case TGS_RenewReq:
        strcpy(evt_name, "TGS_RenewReq");
        break;
      case TGS_ValidateReq:
        strcpy(evt_name, "TGS_ValidateReq");
        break;
      case ACL_Lookup:
        strcpy(evt_name, "ACL_Lookup");
        break;
      case ACL_Replace:
        strcpy(evt_name, "ACL_Replace");
        break;
      case ACL_GetAccess:
        strcpy(evt_name, "ACL_GetAccess");
        break;
      case ACL_TestAccess:
        strcpy(evt_name, "ACL_TestAccess");
        break;
      case ACL_TestOnBehalf:
        strcpy(evt_name, "ACL_TestOnBeHalf");
        break;
      case ACL_GetMgrTypes:
        strcpy(evt_name, "ACL_GetMgrTypes");
        break;
      case ACL_GetReferral:
        strcpy(evt_name, "ACLGet_Referral");
        break;
      case PRIV_GetPtgt:
        strcpy(evt_name, "PRIV_GetPtgt");
        break;
      case ACCT_Add:
        strcpy(evt_name, "ACCT_Add");
        break;
      case ACCT_Delete:
        strcpy(evt_name, "ACCT_delete");
        break;
      case ACCT_Rename:
        strcpy(evt_name, "ACCT_Rename");
        break;
      case ACCT_Lookup:
        strcpy(evt_name, "ACCT_Lookup");
        break;
      case ACCT_Replace:
        strcpy(evt_name, "ACCT_Replace");
        break;
      case ACCT_GetProjlist:
        strcpy(evt_name, "ACCT_GetProjlist");
        break;
      case LOGIN_GetInfo:
        strcpy(evt_name, "LOGIN_GetInfo");
        break;
      case PGO_Add:
        strcpy(evt_name, "PGO_Add");
        break;
      case PGO_Delete:
        strcpy(evt_name, "PGO_Delete");
        break;
      case PGO_Replace:
        strcpy(evt_name, "PGO_Replace");
        break;
      case PGO_Rename:
        strcpy(evt_name, "PGO_Rename");
        break;
      case PGO_Get:
        strcpy(evt_name, "PGO_Get");
        break;
      case PGO_KeyTransfer:
        strcpy(evt_name, "PGO_KeyTransfer");
        break;
      case PGO_AddMember:
        strcpy(evt_name, "PGO_AddMember");
        break;
      case PGO_DeleteMember:
        strcpy(evt_name, "PGO_DeleteMember");
        break;
      case PGO_IsMember:
        strcpy(evt_name, "PGO_IsMember");
        break;
      case PGO_GetMembers:
        strcpy(evt_name, "PGO_GetMembers");
        break;
      case PROP_GetInfo:
        strcpy(evt_name, "PROP_GetInfo");
        break;
      case PROP_SetInfo:
        strcpy(evt_name, "PROP_SetInfo");
        break;
      case POLICY_GetInfo:
        strcpy(evt_name, "POLICY_GetInfo");
        break;
      case POLICY_SetInfo:
        strcpy(evt_name, "POLICY_SetInfo");
        break;
      case AUTHPOLICY_GetInfo:
        strcpy(evt_name, "AUTHPOLICY_GetInfo");
        break;
      case AUTHPOLICY_SetInfo:
        strcpy(evt_name, "AUTHPOLICY_SetInfo");
        break;
      case REPADMIN_Stop:
        strcpy(evt_name, "REPADMIN_Stop");
        break;
      case REPADMIN_Maint:
        strcpy(evt_name, "REPADMIN_Maint");
        break;
      case REPADMIN_Mkey:
        strcpy(evt_name, "REPADMIN_Mkey");
        break;
      case REPADMIN_Destroy:
        strcpy(evt_name, "REPADMIN_Destroy");
        break;
      case REPADMIN_Init:
        strcpy(evt_name, "REPADMIN_Init");
        break;
      case REPADMIN_SetSwRev:
        strcpy(evt_name, "REPADMIN_SetSwRev");
        break;
      case SECID_ParseName:
        strcpy(evt_name, "SECID_ParseName");
        break;
      case SECID_GenName:
        strcpy(evt_name, "SECID_GenName");
        break;
      case ERA_Update:
        strcpy(evt_name, "ERA_Update");
        break;
      case ERA_Delete:
        strcpy(evt_name, "ERA_Delete");
        break;
      case ERA_TestUpdate:
        strcpy(evt_name, "ERA_TestUpdate");
        break;
      case ERA_LookupById:
        strcpy(evt_name, "ERA_LookupById");
        break;
      case ERA_LookupNoExpand:
        strcpy(evt_name, "ERA_LoopupNoExpand");
        break;
      case ERA_LookupByName:
        strcpy(evt_name, "ERA_LookupByName");
        break;
      case ERA_SchemaCreate:
        strcpy(evt_name, "ERA_SchemaCreate");
        break;
      case ERA_SchemaDelete:
        strcpy(evt_name, "ERA_SchemaDelete");
        break;
      case ERA_SchemaUpdate:
        strcpy(evt_name, "ERA_SchemaUpdate");
        break;
      case ERA_SchemaLookupId:
        strcpy(evt_name, "ERA_SchemaLookupId");
        break;
      case ERA_SchemaLookupName:
        strcpy(evt_name, "ERA_SchemaLookupName");
        break;
      case PRIV_GetEptgt:
        strcpy(evt_name, "PRIV_GetEptgt");
        break;
      case PRIV_BecomeDelegate:
        strcpy(evt_name, "PRIV_BecomeDelegate");
        break;
      case PRIV_BecomeImpersonator:
        strcpy(evt_name, "PRIV_BecomeImpersonator");
        break;
      /* Events for Audit Server */
      case EVT_DELETE_FILTER:
        strcpy(evt_name, "EVT_DELETE_FILTER");
        break;
      case EVT_SHOW_FILTER:
        strcpy(evt_name, "EVT_SHOW_FILTER");
        break;
      case EVT_LIST_FILTER:
        strcpy(evt_name, "EVT_LIST_FILTER");
        break;
      case EVT_ADD_FILTER:
        strcpy(evt_name, "EVT_ADD_FILTER");
        break;
      case EVT_REMOVE_FILTER:
        strcpy(evt_name, "EVT_REMOVE_FILTER");
        break;
      case EVT_MODIFY_SSTRATEGY:
        strcpy(evt_name, "EVT_MODIFY_SSTRATEGY");
        break;
      case EVT_MODIFY_STATE:
        strcpy(evt_name, "EVT_MODIFY_STATE");
        break;
      case EVT_REWIND:
        strcpy(evt_name, "EVT_REWIND");
        break;
      case EVT_STOP:
        strcpy(evt_name, "EVT_STOP");
        break;
      case EVT_SHOW_SSTRATEGY:
        strcpy(evt_name, "EVT_SHOW_SSTRATEGY");
        break;
      case EVT_SHOW_STATE:
        strcpy(evt_name, "EVT_SHOW_STATE");
        break;
      default :
        strcpy(evt_name, " ");
        break;
    }
    return;
}

boolean32
get_ec_number_from_name
(
  char * evt_class_name,
  unsigned32 * evt_class_number
)
{
    ec_t * ec_p = ec_table;

    while (ec_p != NULL) {
	if ( strcmp(ec_p->ec_name, evt_class_name) == 0 ) {
	    *evt_class_number = ec_p->ec_number;
	    return(TRUE);
	}
	ec_p = ec_p->next;
    }
    return(FALSE);
}

boolean32
get_ec_name_from_number
(
  unsigned32 evt_class_number,
  char * evt_class_name
)
{
    ec_t * ec_p = ec_table;

    while (ec_p != NULL) {
	if ( ec_p->ec_number == evt_class_number) {
	    strcpy(evt_class_name, ec_p->ec_name);
	    return(TRUE);
	}
	ec_p = ec_p->next;
    }
    return(FALSE);
}

boolean32
get_ec_events
(
  char * ec_name,               /* input event class name */
  unsigned32 * ec_number,       /* output event class number */
  int  * evt_cnt,               /* number of events returned */
  unsigned32 ** evts            /* returned event number array */
)
{
    ec_t     * ec_p = ec_table;
    ec_evt_t * evt_p;
    int        i, j;

    while (ec_p != NULL) {
	if ( strcmp (ec_p->ec_name, ec_name) == 0 ) {
	    evt_p = ec_p->evts;
	    for ( i = 0; evt_p != NULL; i++) {
		evt_p = evt_p->next;
	    }
	    if (i != 0) {
		*evts = (unsigned32 *) malloc (i * sizeof(unsigned32));
		if (evts == NULL) {
		    dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, i * sizeof(unsigned32));
		    return(FALSE);
		}
	        evt_p = ec_p->evts;
		for ( j=0; j < i; j++) {
		    (*evts)[j] = evt_p->evt_number;
                    evt_p = evt_p->next;
            	}
	    }
	    *ec_number = ec_p->ec_number;
	    *evt_cnt = i;
	    return(TRUE);
	}
	ec_p = ec_p->next;
    }
    dce_svc_printf(AUD_S_UNKNOWN_EVENT_CLASS_MSG, ec_name);
    return(FALSE);
}

boolean32
get_ec_names
(
  int  * ec_cnt,                /* output the number of evt classes defined */ 
  char *** ec_names             /* output a pointer to an array of name ptrs */
)
{
    ec_t * ec_p = ec_table;
    int i = 0;
    int j = 0;

    while (ec_p != NULL) {
	i++;
	ec_p = ec_p->next;
    }
    if (i != 0) {
	*ec_names = (char **) malloc ( i * sizeof(char *));
 	if (*ec_names == NULL) {
	    dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, i*sizeof(char *));
	    return(FALSE);
	}
	ec_p = ec_table;
	for ( j=0; j < i; j++) {
	    (*ec_names)[i-j-1] = (char *) malloc (strlen(ec_p->ec_name) + 1);
 	    if (((*ec_names)[i-j-1]) == NULL) {
	        dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, strlen(ec_p->ec_name) + 1);
	        return(FALSE);
	    }
	    strcpy((*ec_names)[i-j-1], ec_p->ec_name);
	    ec_p = ec_p->next;
	}
    } else {
	*ec_names = NULL;
    }
    *ec_cnt = i;
    return(TRUE);
}

/* 
 * Frees the memory allocated by get_ec_names.
 */

void
free_ec_names
(
  int  ec_cnt,                /* number of evt classes names to be freed */ 
  char ** ec_names            /* an array of name ptrs */
)
{
    int i = ec_cnt;

    for (; i-- > 0; ) {
	free(ec_names[i]);
    }
    if (ec_names != NULL) {
	free(ec_names);
    }
    return;
}

void 
print_ec_table
(
   boolean32 with_events
)
{
    ec_t     * ec_p = ec_table;
    ec_evt_t * evt_p;

    while (ec_p != NULL) {
	printf("Event Class %s", ec_p->ec_name);
	if (with_events) {
	    printf(":\n");
	    evt_p = ec_p->evts;
	    while (evt_p != NULL) {
	        printf("%lx\n", evt_p->evt_number);
	        evt_p = evt_p->next;
	    }
        }
	printf("\n");
	ec_p = ec_p->next;
    }
    return;
}
	
static void
DoKeyword
(
  int op,
  char * str
)
{
    char *end;
    int   i;

#ifdef DCE_AUD_DEBUG
    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	"DoKeyWord: Find keyword %s with residual string %s\n", 
	parseKeywords[op].name, str));
#endif

    switch (op) {
	case Ecn:
	    for (i=0; i < LINE_BUF_SIZE; i++) {
		if ( !isspace( *(str+i) ) ) break;
	    }
	    if (i < LINE_BUF_SIZE) {
	        evt_class_no = strtoul(str, &end, 0);
		if (isspace(*end) || *end == '\0') {
		    if (parse_mode == ec_table_input) {
			add_ec_to_ec_table();
		    } 
	    	    ecn_assigned = TRUE;
		    return;
		} 
	    }
            break;
	default:
	    break;
    }
    return;
}

static void
DoEventNumber
(
  char *str
)
{
    unsigned32 evt_number;
    char *end;

    evt_number = strtoul(str, &end, 0);
    if ( !isspace(*end) && *end != '\0' ) {
	dce_svc_printf(AUD_S_INVALID_LINE_IN_CLASS_FILE_MSG, str, fname);
	return;
    }

    if (parse_mode == evt_table_input) {
	add_evt_to_evt_table(evt_number);
    } else {	/* parse_mode == ec_table_input */
	add_evt_to_ec_table(evt_number);
    }
    return;
}

#ifdef DCE_AUD_DEBUG
static void
print_relevant_ecs()
{
    unsigned32	*ec;
    int 	i;
    char	num_buf[NUM_BUF_SIZE];
    char 	line_buf[LINE_BUF_SIZE];
    boolean32	buffer_overrun = FALSE;

    ec = aud_g_cli_evt_classes;
    line_buf[0] = '\0';
    for (i = aud_g_cli_num_of_ecs; i > 0; i--) {
	sprintf(num_buf, " %lx", *ec++);
	if (strlen(line_buf) + strlen(num_buf) < LINE_BUF_SIZE) {
	    strcat(line_buf, num_buf);
	} else {
	    buffer_overrun = TRUE;
	    break;
	}
    }
    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	"print_relevant_ecs: relevant event classes = %s\n", line_buf));
    if (buffer_overrun) {
        DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1, 
	"print_relevant_ecs: debug message incomplete -- buffer overrun\n"));
    }
    return;
}
#endif	/* DCE_AUD_DEBUG */

#ifdef DCE_AUD_DEBUG	
static void print_evt_table()
{
    int i, j;
    char	num_buf[NUM_BUF_SIZE];
    char 	line_buf[LINE_BUF_SIZE];
    boolean32	buffer_overrun = FALSE;
 
    for (i = 0; i < aud_g_cli_num_of_evts; i++) {
	if (evt_table[i].evt_class_cnt > 0) {
	    line_buf[0] = '\0';
	    for (j = 0; j < evt_table[i].evt_class_cnt; j++) {
	        sprintf(num_buf, " %lx", (evt_table[i].evt_classes)[j]); 
		if (strlen(line_buf) + strlen(num_buf) < LINE_BUF_SIZE) {
	    	    strcat(line_buf, num_buf);
		} else {
		    buffer_overrun = TRUE;
	    	    break;
		}
	    }
            DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1, 
	        "print_evt_table: event %lx belongs to class(es): %s\n", 
	        evt_table[i].evt_number, line_buf));
	    if (buffer_overrun) {
                DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1, 
	        "print_evt_table: debug message incomplete -- buffer overrun\n"));
	    }
	}
    }
    return;
}
#endif  /* DCE_AUD_DEBUG */

static int
Line_AddChar
(
  char c
)
{
    if (line_len >= LINE_BUF_SIZE) {
	dce_svc_printf(AUD_S_LINE_TOO_LONG_IN_CLASS_FILE_MSG, fname);
	return(-1);
    }
    line[line_len++] = c;
    return(0);
}
	
static int
ParseFindKeyword (str, rtn_str)
    char	    *str;		/* String to find */
    char	    **rtn_str;		/* String to return */
{
    register int    end,
		    cur;
    register int    diff;
    register int    i;
    
    end = (sizeof(parseKeywords)/sizeof(parseKeywords[0])) - 1;

    for (cur = 0; cur <=end; cur++) {
	diff = strncmp (str, parseKeywords[cur].name, 
			     strlen(parseKeywords[cur].name));

	if (diff == 0) {
	    for (i = strlen(parseKeywords[cur].name); i < LINE_BUF_SIZE;
			i++) {
		if (*(str+i) == '=') {
	    	    *rtn_str = str + i + 1;
	     	    return (cur);
		} else if ( !isspace( *(str+i) ) ) 
		    break;
	    }
	    return (-1);
	}
    } 
    return (-1);
}

/*-
 *---------------------------------------------------------------------
 * ParseReadLine --
 *	Read an entire line from the input file. Called only by ParseFile.
 *	To facilitate escaped newlines and what have you, a character is
 *	buffered in 'lastc', which is '\0' when no characters have been
 *	read. When we break out of the loop, c holds the terminating
 *	character and lastc holds a character that should be added to
 *	the line (unless we don't read anything but a terminator).
 *
 * Results:
 *	A line w/o its newline
 *
 * Side Effects:
 *	Only those associated with reading a character
 *---------------------------------------------------------------------
 */
static int
ParseReadLine ()
{
    register int  c;	      	/* the current character */
    register int  lastc;    	/* The most-recent character */
    char    	  *line;    	/* Result */
    int	    	  lineLength;	/* Length of result */

    line_len = 0;

    while(1) {
	c = getc(file);
	if (c == '\n') {
	    lineno++;
	} else if (c == '#') {
		ungetc(c, file); 
		break;
	} else {
	    /*
	     * Anything else breaks out without doing anything
	     */
	    break;
	}
    }
	
    if (c != EOF) {
	lastc = c;
	
	while (((c = getc(file)) != '\n' || (lastc == '\\')) &&
	       (c != EOF))
	{
test_char:
	    switch(c) {
	    case '\n':
		/*
		 * Escaped newline: read characters until a non-space or an
		 * unescaped newline and replace them all by a single space.
		 * This is done by storing the space over the backslash and
		 * dropping through with the next nonspace. 
		 */
		lineno++;
		lastc = ' ';
		while ((c = getc(file)) == ' ' || c == '\t') {
		    continue;
		}
		if (c == EOF || c == '\n') {
		    lineno++;
		    goto line_read;
		} else {
		    goto test_char;
		}
		break;
	    case '#':
		do {
		    c = getc(file);
		} while ((c != '\n') && (c != EOF));
		lineno++;
		goto line_read;
		break;
	    default:
		break;
	    }
	    /*
	     * Copy in the previous character and save this one in lastc.
	     */
	    if (Line_AddChar (lastc) == -1) return(0);
	    lastc = c;
	}
	lineno++;
    line_read:
	
	if (lastc != '\0') {
	    if (Line_AddChar (lastc) == -1) return(0);
	}
	if (Line_AddChar ('\0') == -1) return(0);
	return (1);
    } else {
	/*
	 * Hit end-of-file, so return a NULL line to indicate this.
	 */
	return(0);
    }
}


/*-
 *---------------------------------------------------------------------
 * ParseFile --
 *	Parse an event class configuration file, incorporating it into the
 *	server's event table.  Only event classes which contains events 
 * 	of the server are included in the event table.	
 *
 *---------------------------------------------------------------------
 */
void
ParseFile()
{
    register char *cp;		/* pointer into the line */
    int            op;
    char          *residual;

    lineno = 0;

    /* Reset the following two variables for each ec file read */
    ecn_assigned = FALSE;	/* assigned at the first non-comment line */
    ecn_added = FALSE;		/* added once find an event in this class */

nextLine:
    while (ParseReadLine ()) {
	if (*line == '#') {
	/* a comment line */
	    goto nextLine;
	}
	    
	if ((op = ParseFindKeyword(line, &residual)) != -1) {
	    DoKeyword(op, residual);
	} else {
	    if (ecn_assigned == FALSE) {
		dce_svc_printf(AUD_S_INVALID_EVENT_CLASS_FILE_MSG, fname);
		return;
	    } 
	    DoEventNumber(line);
	}
    }
}

void
ParseFiles()
{
    struct dirent * dir_entry;
    DIR * dir;
    int i;

    line = malloc(LINE_BUF_SIZE);
    if (line == NULL) {
        dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, LINE_BUF_SIZE);
        return;
    }
	
    dir = opendir(EVENT_CLASS_DIR);
    if (dir == NULL) {
	dce_svc_printf(AUD_S_CANNOT_OPEN_EVENT_CLASS_DIR_MSG, EVENT_CLASS_DIR);
	return;
    }

    while ((dir_entry = readdir(dir)) != NULL) {
	if (strcmp(".", dir_entry->d_name) && strcmp("..", dir_entry->d_name)) {
	    strcpy(fname, EVENT_CLASS_DIR);
	    strcat(fname, dir_entry->d_name);
	    file = fopen(fname, "r");
	    if (file != NULL) {
#ifdef DCE_AUD_DEBUG
	        DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		    "input_evt_table: File %s opened\n", fname));
#endif
	        ParseFile();
	        fclose(file);
#ifdef DCE_AUD_DEBUG
	        if (ecn_assigned) {
	            DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		        "input_evt_table: File %s define event class number %lx\n", 
		        fname, evt_class_no));
	        }
#endif
	    } else {
	        dce_svc_printf(AUD_S_CANNOT_OPEN_EVENT_CLASS_FILE_MSG, fname);
	    } 
        }
    }
    closedir(dir);
    free(line);
    return;
}


void input_evt_table ()
{
    parse_mode = evt_table_input;  
    init_evt_table();
    aud_g_cli_evt_classes = NULL;
    aud_g_cli_num_of_ecs = 0;

    ParseFiles();

#ifdef DCE_AUD_DEBUG
    print_evt_table();
    print_relevant_ecs();
#endif
    return;
}

void input_ec_table ()
{

    parse_mode = ec_table_input;
    ParseFiles();
    return;
}

