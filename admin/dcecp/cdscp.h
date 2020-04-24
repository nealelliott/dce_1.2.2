/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdscp.h,v $
 * Revision 1.1.10.1  1996/08/09  11:45:14  arvind
 * 	Merge changes.
 * 	[1996/06/10  14:48 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge changes for bad prototype removal.
 * 	[1996/03/18  15:39 UTC  truitt  /main/HPDCE02/8]
 *
 * 	Merge.
 * 	[1996/03/18  15:38 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	Small code cleanup.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/10  14:20 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.8.2  1996/02/18  23:31:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:35  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  21:19:44  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/7  1995/10/10  15:10 UTC  truitt
 * 	Merge cds changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts49/1  1995/10/05  15:00 UTC  truitt
 * 	CHFts16537: Split 'cdsmisc' command functionality into existing 'directory'
 * 	command and new 'cds' and 'cdsclient' commands.
 * 
 * 	HP revision /main/HPDCE02/6  1995/07/26  19:30 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts43/1  1995/07/26  19:29 UTC  truitt
 * 	CHFts14274: Change 'destroy' to 'discard'.
 * 
 * 	HP revision /main/HPDCE02/5  1995/07/21  13:12 UTC  truitt
 * 	Merge cdscache additions.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts40/1  1995/07/21  13:11 UTC  truitt
 * 	CHFts14274: Add new cdscache command.
 * 
 * 	HP revision /main/HPDCE02/4  1995/06/16  18:34 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts35/1  1995/06/16  18:32 UTC  truitt
 * 	CHFts15504: Correct prototype of do_sec_login().
 * 
 * 	HP revision /main/HPDCE02/3  1995/04/06  14:08 UTC  jrr
 * 	Add DCP_CHECK_CDS_NAME_LEN() macro (CHFts14888).
 * 	[1995/04/05  21:33 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14888/1]
 * 
 * 	HP revision /main/HPDCE02/2  1995/02/14  15:47 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  15:45 UTC  truitt
 * 	CHFts14285: Combine all cds check args macros into one,
 * 	since all six are the same anyway.  Also, make sure correct
 * 	macros are called when processing an error condition.
 * 
 * 	HP revision /main/HPDCE01/1  1995/01/13  18:11 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/truitt_dcecp_chk_07/1  1995/01/13  18:08 UTC  truitt
 * 	Add prototypes for the commands that cover the cdscp
 * 	functionality that is currently not present.
 * 	[1995/12/08  17:54:24  root]
 * 
 * Revision 1.1.5.22  1994/09/26  20:23:23  kevins
 * 	OT 12334 added cdsalias command
 * 	[1994/09/26  20:18:02  kevins]
 * 
 * Revision 1.1.5.21  1994/09/22  23:48:24  zee
 * 	Add argument to F_member prototype.
 * 	[1994/09/22  22:54:40  zee]
 * 
 * Revision 1.1.5.20  1994/09/16  14:50:09  kevins
 * 	OT 11270 Implemented clearinghouse catalog
 * 	[1994/09/16  14:47:33  kevins]
 * 
 * Revision 1.1.5.19  1994/09/07  15:07:17  kevins
 * 	OT 11814 Changed to use format_ts to convert timestamps.
 * 	[1994/09/07  14:31:25  kevins]
 * 
 * Revision 1.1.5.18  1994/09/02  21:03:14  zee
 * 	Add argument to get_clearinghouse_handle prototype.
 * 	[1994/09/02  18:39:09  zee]
 * 
 * Revision 1.1.5.17  1994/08/19  21:09:50  zee
 * 	Add clringhouse_repair prototype.
 * 	[1994/08/19  18:10:11  zee]
 * 
 * Revision 1.1.5.16  1994/08/01  15:07:14  zee
 * 	     Add clringhouse_initiate, clringhouse_verify.
 * 	     Rename cellalias* to cdsalias*.
 * 	[1994/07/29  20:58:41  zee]
 * 
 * Revision 1.1.5.15  1994/07/14  11:18:48  kevins
 * 	CR 11219 CDS_Tower encoding fix.
 * 	[1994/07/14  11:16:33  kevins]
 * 
 * Revision 1.1.5.14  1994/07/12  19:26:20  zee
 * 	     Add cellalias declarations.
 * 	[1994/07/12  15:55:29  zee]
 * 
 * Revision 1.1.5.13  1994/06/09  16:01:38  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:01  devsrc]
 * 
 * Revision 1.1.5.12  1994/05/11  15:11:23  kevins
 * 	OT 10197 Added _s(cds) support
 * 	[1994/05/11  15:08:48  kevins]
 * 
 * Revision 1.1.5.11  1994/05/09  14:53:42  coren
 * 	Merge up.
 * 	[1994/05/06  17:33:16  coren]
 * 
 * 	Removed tabs.
 * 	[1994/05/03  13:57:36  coren]
 * 
 * 	    Added declaration of directory_merge.
 * 	[1994/04/20  18:24:30  coren]
 * 
 * Revision 1.1.5.10  1994/05/03  13:48:53  kevins
 * 	OT# 10499 10407
 * 	Update modify switches to work correctly according to the RFC.
 * 	Set the attribute types appropriately.
 * 	[1994/05/03  12:51:43  kevins]
 * 
 * Revision 1.1.5.9  1994/04/20  18:18:51  kevins
 * 	CR 10406 encode/decode cds attributes by syntax
 * 	[1994/04/20  15:21:56  kevins]
 * 
 * Revision 1.1.5.8  1994/03/26  11:56:04  kevins
 * 	CR 10230 removed instances of extern Tcl_Interp *interp from sources
 * 	[1994/03/26  11:55:50  kevins]
 * 
 * Revision 1.1.5.7  1994/03/24  10:21:30  kevins
 * 	CR 10081 modified -timeout on link create
 * 	[1994/03/24  10:21:15  kevins]
 * 
 * Revision 1.1.5.6  1994/03/14  17:39:34  kevins
 * 	CR9904 Added support for help system
 * 	[1994/03/14  13:44:18  kevins]
 * 
 * Revision 1.1.5.5  1994/03/01  17:29:41  mario
 * 	rebuild corrupted rcs file - includes CDSCP_H to protect from multi inclusions addition
 * 	[1994/03/01  17:29:26  mario]
 * 
 * Revision 1.1.5.4  1994/02/03  18:42:15  kevins
 * 	updated to conform to func spec
 * 	[1994/02/03  18:29:39  kevins]
 * 
 * Revision 1.1.5.3  1993/12/29  15:58:06  kevins
 * 	Added information to support reading attribute information from cds_attributes file
 * 	[1993/12/29  15:57:30  kevins]
 * 
 * Revision 1.1.5.2  1993/12/27  15:05:45  kevins
 * 	Remove dce_error function call
 * 	[1993/12/27  15:04:56  kevins]
 * 
 * Revision 1.1.5.1  1993/12/20  19:53:32  kevins
 * 	Fixed to conform to style guide.
 * 	[1993/12/20  18:51:48  kevins]
 * 
 * Revision 1.1.3.2  1993/11/05  19:52:54  kevins
 * 	added witlcard protos
 * 	[1993/11/05  19:45:55  kevins]
 * 
 * Revision 1.1.3.1  1993/10/21  19:39:25  kevins
 * 	completed the name change from dsh to dcecp
 * 	[1993/10/21  19:38:46  kevins]
 * 
 * 	Defuncted the clearinghouse status command
 * 	[1993/10/14  19:27:44  kevins]
 * 
 * 	added a variety of prototypes
 * 	[1993/10/13  19:00:05  kevins]
 * 
 * 	modified prototypes
 * 	[1993/09/13  14:03:06  kevins]
 * 
 * 	fixed submission error
 * 	[1993/09/08  20:05:34  kevins]
 * 
 * 	Merged with changes from 1.1.2.5
 * 	[1993/09/08  19:58:01  kevins]
 * 
 * 	added function prototypes
 * 	[1993/09/08  19:51:45  kevins]
 * 
 * 	Initial sub for cds-cache commands
 * 	[1993/08/25  14:44:34  kevins]
 * 
 * 	Initial sub for cds-cache commands
 * 	[1993/08/25  14:44:34  kevins]
 * 
 * 	general updating for cdscp commands
 * 	[1993/08/23  14:17:13  kevins]
 * 
 * 	Alter command return values
 * 	[1993/08/03  17:00:37  kevins]
 * 
 * 	Initial submission of dsh
 * 	[1993/07/23  19:34:55  kevins]
 * 
 * Revision 1.1.3.2  1993/08/03  16:42:01  kevins
 * 	Alter command return values
 * 
 * $EndLog$
 */
#ifndef _CDSCP_H
#define _CDSCP_H

#include <dce/cdsclerk.h>
#include <dce/sec_login.h>
#include <dce/tcl.h>
#include <dce/cds_types.h>


/****************************************************************
 * command prototypes
 ****************************************************************/
int directory_create (ClientData, Tcl_Interp *, int, char **);
int directory_delete (ClientData, Tcl_Interp *, int, char **);
int directory_addto (ClientData, Tcl_Interp *, int, char **);
int directory_list (ClientData, Tcl_Interp *, int, char **);
int directory_merge (ClientData, Tcl_Interp *, int, char **);
int directory_modify (ClientData, Tcl_Interp *, int, char **);
int directory_rmfrom (ClientData, Tcl_Interp *, int, char **);
int directory_show (ClientData, Tcl_Interp *, int, char **);
int directory_update (ClientData, Tcl_Interp *, int, char **);
int object_create(ClientData, Tcl_Interp *, int, char **);
int object_delete(ClientData, Tcl_Interp *, int, char **);
int object_modify(ClientData, Tcl_Interp *, int, char **);
int object_show(ClientData, Tcl_Interp *, int, char **);
int link_create(ClientData, Tcl_Interp *, int, char **);
int link_delete(ClientData, Tcl_Interp *, int, char **);
int link_modify(ClientData, Tcl_Interp *, int, char **);
int link_show(ClientData, Tcl_Interp *, int, char **);
int clringhouse_create(ClientData, Tcl_Interp *, int, char **);
int clringhouse_delete(ClientData, Tcl_Interp *, int, char **);
int clringhouse_show(ClientData, Tcl_Interp *, int, char **);
int clringhouse_disable(ClientData, Tcl_Interp *, int, char **);
int clringhouse_initiate(ClientData, Tcl_Interp *, int, char **);
int clringhouse_verify(ClientData, Tcl_Interp *, int, char **);
int clringhouse_repair(ClientData, Tcl_Interp *, int, char **);
int clringhouse_catalog(ClientData, Tcl_Interp *, int, char **);
int cache_create(ClientData, Tcl_Interp *, int, char **);
int cache_delete(ClientData, Tcl_Interp *, int, char **);
int cache_discard(ClientData, Tcl_Interp *, int, char **);
int cache_dump(ClientData, Tcl_Interp *, int, char **);
int cache_show(ClientData, Tcl_Interp *, int, char **);
int cdsalias_create(ClientData, Tcl_Interp *, int, char **);
int cdsalias_connect(ClientData, Tcl_Interp *, int, char **);
int cdsalias_delete(ClientData, Tcl_Interp *, int, char **);
int cdsalias_catalog(ClientData, Tcl_Interp *, int, char **);
int cds_disable(ClientData, Tcl_Interp *, int, char **);
int cds_show(ClientData, Tcl_Interp *, int, char **);
int cdsclient_disable(ClientData, Tcl_Interp *, int, char **);
int cdsclient_show(ClientData, Tcl_Interp *, int, char **);

/* 
 * Prototypes and exported procedures 
 */

void F_small(cds_attrvalue_t *,char **);
void F_short(cds_attrvalue_t *,char **);
void F_long(cds_attrvalue_t *,char **);
void F_uuid(cds_attrvalue_t *,char **);
void F_timestamp(cds_attrvalue_t *,char **);
int F_timeout(cds_attrvalue_t *,char **);
void F_version(cds_attrvalue_t *,char **);
void F_char(cds_attrvalue_t *,char **);
int F_member(cds_attrvalue_t *,char **, int);
int F_rp(cds_attrvalue_t *,char **);
int F_pp(cds_attrvalue_t *,char **);
void F_fullname(cds_attrvalue_t *,char **);
void F_byte(cds_attrvalue_t *, char **);
int F_chdirectory(cds_attrvalue_t *,char **);
int F_gp(cds_attrvalue_t *, char **);
void clear_flags(struct cdsFlagStat *, cdsNameType_t);
void do_sec_login( void );
int read_attr_value(cds_full_name_t *, char *,
                    cds_attrvalue_t **, char **, cdsEntries_t,
                    cds_full_name_t *);

int CVTstrConvergence(char *, cds_attrvalue_t *);
int strVersion(char *, cds_attrvalue_t *);
int strLinkTarget(char *, cds_attrvalue_t *);
int strClass(char *, cds_attrvalue_t *);
int strByte(char *, cds_attrvalue_t *);
int strChar(char *, cds_attrvalue_t *);
int strSmall(char *, cds_attrvalue_t *);
int strTower(Tcl_Interp *, char *, cds_attrvalue_t *);
int strTimeout(char **,cds_attrvalue_t *);
int strUuid(char *,cds_attrvalue_t *);
int decode_replicatype(int, char **);
int decode_clearinghouse_name(char *, char *);
int decode_tower(void *, unsigned short, char **);
void format(unsigned char *, int, unsigned char *);
int create_clearinghouse(cds_full_name_t);
handle_t get_default_handle(int , unsigned32 *);
handle_t get_clearinghouse_handle(int, cds_FullName_t *,
				  uuid_t *, error_status_t *);
int clrnghouse_command(cds_full_name_t, unsigned32);
int cds_cache_command(char *, char *, unsigned32);
int convergence(cds_attrvalue_t *, char **);
int replicatype(cds_attrvalue_t *, char **);
int replicastate(cds_attrvalue_t *, char **);
int clearinghouse_status(cds_full_name_t, unsigned32);
int chstate(cds_attrvalue_t *, char **);
int chlastaddress(cds_attrvalue_t *, char **);
int cdsCvtTowerToStr(char *, char **);
int isllist(char *);
int iswild(unsigned char *);
void cvtstrtooid(char *, int *);
void init_cds_attribute_table(Tcl_Interp *);
int get_list_elements(char *, int *, char ***);
void determine_modify_order(int [], int, char **);
int   is_attribute_multi_line(int);

#define CLR_NAME "cds"
/* Attribute Action */

  typedef enum {
    AddAttribute = 1,
    DeleteAttribute,
    RemoveAttribute,
    ChangeAttribute,
    ShowAttribute,
    ShowAttrType,
    IgnoreAttribute
  } Attribute_Action_t;


/* 
 * data structures and constants
 */
#define REPLICA_SNAME_MAX 10
#define MAXREPTYPE    4
#define NoAttrType   -1
#define Oidindex    0
#define AttrName    1
#define OidName     2
#define SyntaxName  3
#define SyntaxIndex 4
#define AttrType    5 
#define ModifyTypes 3
#define MinimumCmp  2

typedef struct {
    char attr_name[CDS_SNAME_MAX];
    char *entered_name;
    int              cur_attr;
    Attribute_Action_t action;
    cdsEntries_t    obj_type;
    cdsAttrType_t  attr_type;
    cds_attrvalue_t **attribute;
} cds_attribute_value;

typedef struct {
    int cur_value;
    cds_attribute_value **value;
} cds_attribute_list;

int parse_attribute(Tcl_Interp *, cds_attribute_value *, char *);

int cds_parse_attributes(Tcl_Interp *,
                         char *, 
                         Attribute_Action_t , 
                         cdsEntries_t, 
                         cdsAttrType_t,
                         cds_attribute_list *);

int parse_timeout(char *, cds_attribute_list *);
int remove_cds_attr(cds_full_name_t *,
                    cds_attr_name_t *, 
                    cds_attribute_value *);
int modify_cds_attribute(Tcl_Interp *,
                         cds_full_name_t *, 
                         cds_attr_name_t *,
                         cds_attribute_list *);
int generate_attribute_list(Tcl_Interp *,
                            char *, 
                            char *, 
                            char *,
                            cdsEntries_t  ,
                            cds_attribute_list *,
                            int ,
                            int ,
                            int []);


void free_attr_value(cds_attribute_list **);
int format_all_attr(Tcl_Interp  *, cds_attribute_list *);

int enum_all_attr(Tcl_Interp  *,
                  cds_full_name_t *,
                  cds_full_name_t *,
                  cdsEntries_t ,
                  Attribute_Action_t ,
                  cds_attribute_list **);

int enum_one_attr(Tcl_Interp *interp, 
                  cds_full_name_t    *,
                  cds_full_name_t    *,
                  char               *,
                  cdsEntries_t       ,
                  cds_attribute_list **);

int enum_set_attr(Tcl_Interp *interp, 
                  cds_full_name_t    *,
                  cds_full_name_t    *,
                  char               *,
                  cdsEntries_t       ,
                  cds_attribute_list **);

u_char partialRes[1024];
int Authenticat;
sec_login_handle_t login_context_h;
error_status_t error_text[200];

#define stringFullName       "FullName"
#define stringbyte           "byte"
#define stringReplicaPointer "ReplicaPointer"
#define stringsmall          "small"
#define stringTimestamp      "Timestamp"
#define stringuuid           "uuid"
#define stringVersion        "Version"
#define stringchar           "char"
#define stringgdaPointer     "gdaPointer"
#define stringCHDirectory    "CHDirectory"
#define stringTimeout        "Timeout"
#define stringParentPointer  "ParentPointer"
#define stringGroupMember    "GroupMember"

typedef enum {
    cdsFullName,
    cdsbyte,
    cdsReplicaPointer,
    cdssmall,
    cdsTimestamp,
    cdsuuid,
    cdsVersion,
    cdschar,
    cdsgdaPointer,
    cdsCHDirectory,
    cdsTimeout,
    cdsParentPointer,
    cdsGroupMember,
    cdsAttrType
} cds_attribute_syntax_t;

typedef struct {
    cds_attribute_syntax_t syntax;
    int  oid;
    char string_syntax[15];
    char *string_oid;
    char *string_attribute;
}attribute_syntax;

/*
 * That is take the oid "1.3.22.15.1" remove the periods and make it an 
 * int. This is based on the assumption that oid <-> attribute name will
 * remain constant. 
 */
#define CDS_Members 13221310
#define CDS_GroupRevoke 13221311
#define CDS_CTS 13221312 
#define CDS_UTS 13221313
#define CDS_Class  13221315
#define CDS_ClassVersion  13221316
#define CDS_ObjectUUID  13221317
#define CDS_Replicas  13221319
#define CDS_AllUpTo  13221320
#define CDS_Convergence  13221321
#define CDS_InCHName  13221322
#define CDS_ParentPointer  13221323
#define CDS_DirectoryVersion  13221324
#define CDS_UpgradeTo  13221325
#define CDS_LinkTarget  13221327
#define CDS_LinkTimeout  13221328
#define CDS_Towers  13221330
#define CDS_CHName  13221332
#define CDS_CHLastAddress  13221334
#define CDS_CHUpPointers  13221335
#define CDS_CHState  13221336
#define CDS_CHDirectories  13221337
#define CDS_ReplicaState  13221340
#define CDS_ReplicaType  13221341
#define CDS_LastSkulk  13221342
#define CDS_LastUpdate  13221343
#define CDS_RingPointer  13221344
#define CDS_Epoch  13221345
#define CDS_ReplicaVersion  13221346
#define CDS_NSCellname  13221348
#define CDS_GDAPointers  13221352
#define CDS_CellAliases  13221353
#define CDS_ParentCellPointers  13221354
#define RPC_ClassVersion  1322111
#define RPC_ObjectUUIDs  1322112
#define RPC_Group  1322113
#define RPC_Profile  1322114
#define SEC_RepUUID  1322151

/* Generic Management State */
typedef enum {
    clearinghouse_Off = 0,  /* Not available */
    clearinghouse_Initial,  /* In the process of coming up */
    clearinghouse_On,       /* Running and available */
    clearinghouse_Shut,     /* In the process of graceful shutdown */
    clearinghouse_Broken    /* In a fatal error condition */
} Clearinghouse_State_t;


#define DCP_SET_CDS_ERROR_ARG(code, arg) {                              \
    Tcl_SetResult(interp, (char *)dce_sprintf(code,arg), TCL_DYNAMIC);  \
    DCP_GENERAL_SET_E_VAR(code);                                        \
    return TCL_ERROR;                                                   \
}

#define DCP_CHECK_CDS_NO_ARG(type) {                                    \
    if (argc < 2) {                                                     \
        char *temp;                                                     \
        temp = (char *)dce_sprintf(dcp_s_cds_no_ ## type ## _name);     \
        Tcl_SetResult(interp, temp, TCL_DYNAMIC);                       \
        DCP_GENERAL_SET_E_VAR(dcp_s_cds_no_ ## type ## _name);          \
        return TCL_ERROR;                                               \
    }                                                                   \
}

#define DCP_CHECK_CDS_NAME_LEN(string) {                                \
  if (strlen(string) > CDS_FNAME_MAX) {                                 \
    Tcl_SetResult(interp, (char *)dce_sprintf(dcp_s_cds_fname_too_long, string), TCL_DYNAMIC);      \
    DCP_GENERAL_SET_E_VAR(dcp_s_cds_fname_too_long);                    \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#endif
