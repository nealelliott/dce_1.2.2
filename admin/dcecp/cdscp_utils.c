/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdscp_utils.c,v $
 * Revision 1.1.11.1  1996/10/03  17:48:55  arvind
 * 	Bulletproof the fix for OT13022.
 * 	[1996/08/23  20:32 UTC  jrr  /main/jrr_122_2/1]
 *
 * Revision 1.1.8.3  1996/02/18  19:12:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:45  marty]
 * 
 * Revision 1.1.8.2  1995/12/08  21:20:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/14  1995/10/20  21:21 UTC  jrr
 * 	Change conf_level_pointer to dcp_g_cds_conf_level.
 * 	[1995/10/20  21:10 UTC  jrr  /main/HPDCE02/jrr_dcecp_01/1]
 * 
 * 	HP revision /main/HPDCE02/13  1995/08/22  20:42 UTC  truitt
 * 	Merge tower work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts45/1  1995/08/22  20:40 UTC  truitt
 * 	CHFts16121: The tower output for directory show seems incorrect.
 * 
 * 	HP revision /main/HPDCE02/12  1995/07/28  18:57 UTC  rose_l
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/rose_l_CHFts15801/1  1995/07/28  16:22 UTC  rose_l
 * 	fixing clearinghouse show core dump (CHFts15801)...
 * 
 * 	HP revision /main/HPDCE02/11  1995/07/14  18:24 UTC  truitt
 * 	Merge function name changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts39/1  1995/07/14  18:21 UTC  truitt
 * 	CHFts15697: Rename one of the internal functions so the cdsbrowser
 * 	doesn't run into a conflict with an identically-named one in cdscp.
 * 
 * 	HP revision /main/HPDCE02/10  1995/06/16  18:32 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts35/1  1995/06/16  18:31 UTC  truitt
 * 	CHFts15504: Correct function type of do_sec_login().
 * 
 * 	HP revision /main/HPDCE02/9  1995/05/01  18:54 UTC  truitt
 * 	Merge changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  18:53 UTC  truitt
 * 	CHFts15085: Fix specific memory leaks in code when
 * 	monitor is run using dcecp.
 * 
 * 	HP revision /main/HPDCE02/8  1995/04/07  16:56 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_mothra_chfts25/1  1995/04/07  16:55 UTC  truitt
 * 	CHFts14803: Rename macro.
 * 
 * 	HP revision /main/HPDCE02/7  1995/04/06  14:08 UTC  jrr
 * 	Fix unchecked strcpy() of user input into fixed size buffer (CHFts14888).
 * 	[1995/04/05  21:33 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14888/1]
 * 
 * 	HP revision /main/HPDCE02/6  1995/04/06  13:55 UTC  jrr
 * 	Fix various memory leaks (CHFts14834).
 * 	[1995/04/05  14:31 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14834/1]
 * 
 * 	HP revision /main/HPDCE02/5  1995/03/27  17:27 UTC  jrr
 * 	Check for NULL return from cdsGetHandle() (CHFts14682).
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_cr12550/1  1995/03/27  16:09 UTC  jrr
 * 	Check for NULL return from cdsGetHandle() (CHFts14682).
 * 
 * 	HP revision /main/HPDCE02/4  1995/02/24  15:43 UTC  truitt
 * 	Merge message work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts15/1  1995/02/24  15:42 UTC  truitt
 * 	CHFts14284: Fix bad error message in parse timeout.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts15/1  1995/02/24  15:41 UTC  truitt
 * 	CHFts14284: Fix bad error message in object create.
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/20  19:35 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts03/1  1995/01/20  19:33 UTC  truitt
 * 	CHFts14122: Complete work for using confidence level.
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/18  15:54 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts01/1  1995/01/18  15:53 UTC  truitt
 * 	CHFts14002: CDS_Tower variables are not being displayed
 * 	correctly during an 'object show' command.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/05  17:56 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/12/05  17:55 UTC  truitt
 * 	OT12710: Remove useless macro code.  Also
 * 	fix a compiler warning.
 * 	[1995/12/08  17:54:49  root]
 * 
 * Revision 1.1.4.32  1994/10/06  18:13:57  rousseau
 * 	Fix GDAPointers output (CR 12505).
 * 	[1994/10/06  18:13:37  rousseau]
 * 
 * Revision 1.1.4.31  1994/09/29  21:28:27  kevins
 * 	OT 12415 fixed suspect malloc lengths
 * 	[1994/09/29  21:27:46  kevins]
 * 
 * Revision 1.1.4.30  1994/09/22  23:48:26  zee
 * 	OT 12148: CDS_CellAliases attribute now displays "Primary" and "Alias".
 * 	[1994/09/22  22:55:02  zee]
 * 
 * Revision 1.1.4.29  1994/09/16  14:50:15  kevins
 * 	OT 12194 Modified the output format for  CDS_CellAliases
 * 	[1994/09/16  14:48:23  kevins]
 * 
 * Revision 1.1.4.28  1994/09/12  19:53:56  kevins
 * 	OT 11968 This sets the cdsFlagStat in accordance to the login context.
 * 	[1994/09/12  13:55:09  kevins]
 * 
 * Revision 1.1.4.27  1994/09/08  18:01:43  kevins
 * 	OT 12105 change definition for timebuffer in f_timestamp
 * 	[1994/09/08  18:01:33  kevins]
 * 
 * Revision 1.1.4.26  1994/09/07  15:07:19  kevins
 * 	OT 11814 Changed to use format_ts to convert timestamps.
 * 	[1994/09/07  14:31:29  kevins]
 * 
 * Revision 1.1.4.25  1994/08/23  18:50:09  kevins
 * 	OT 11781 CDS_UpgradeTo attribute type set to cdsSingle
 * 	[1994/08/23  18:49:53  kevins]
 * 
 * Revision 1.1.4.24  1994/08/21  19:45:23  melman
 * 	fix various compiler warnings
 * 	[1994/08/21  19:29:37  melman]
 * 
 * Revision 1.1.4.23  1994/08/10  13:31:41  kevins
 * 	Merged with changes from 1.1.4.22
 * 	[1994/08/10  13:31:32  kevins]
 * 
 * 	OT 11575 untranslatable towers now displayed in binary format
 * 	[1994/08/10  10:36:37  kevins]
 * 
 * Revision 1.1.4.22  1994/08/08  21:04:15  melman
 * 	put include dce_utils.h in dcecp.h
 * 	[1994/08/08  21:01:43  melman]
 * 
 * Revision 1.1.4.21  1994/08/01  14:01:47  kevins
 * 	OT 11501 Added dce_utils.h and fixed castings
 * 	[1994/08/01  13:55:02  kevins]
 * 
 * Revision 1.1.4.20  1994/07/29  20:37:22  kevins
 * 	OT 11451 mad 64 bit changes requested by a_ferris
 * 	[1994/07/29  20:19:18  kevins]
 * 
 * Revision 1.1.4.19  1994/07/14  11:18:50  kevins
 * 	Merged with changes from 1.1.4.18
 * 	[1994/07/14  11:18:37  kevins]
 * 
 * 	CR 11219 CDS_Tower encoding fix.
 * 	[1994/07/14  11:16:36  kevins]
 * 
 * Revision 1.1.4.18  1994/07/12  19:26:25  zee
 * 	     Add enum_set_attr routine (called from cellalias_catalog).
 * 	[1994/07/12  15:56:03  zee]
 * 
 * Revision 1.1.4.18  1994/07/12  19:26:25  zee
 * 	     Add enum_set_attr routine (called from cellalias_catalog).
 * 	[1994/07/12  15:56:03  zee]
 * 
 * Revision 1.1.4.17  1994/06/10  09:39:12  kevins
 * 	Merged with changes from 1.1.4.16
 * 	[1994/06/10  09:38:58  kevins]
 * 
 * 	OT 10917 modify display output to provide extra line between objects
 * 	         in a list.
 * 	OT 10618 Attribute byte type now formats correctly for output.
 * 	[1994/06/10  09:36:50  kevins]
 * 
 * 	OT 10910 modify display generation for attributes
 * 	[1994/06/09  17:37:05  kevins]
 * 
 * Revision 1.1.4.16  1994/06/09  16:01:45  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:09  devsrc]
 * 
 * Revision 1.1.4.15  1994/05/20  22:13:37  kevins
 * 	OT 10714 Modified attribute parsing return values.
 * 	OT 10618 Modified cdsbyte syntax attribute encoding/decoding.
 * 	[1994/05/20  22:12:28  kevins]
 * 
 * Revision 1.1.4.14  1994/05/11  15:11:29  kevins
 * 	OT 10197 added _s(cds) support.
 * 	OT 10555 Fixed core dump on initialization of cds attributes from
 * 	         bad cds_attributes file
 * 	[1994/05/11  15:10:31  kevins]
 * 
 * Revision 1.1.4.13  1994/05/03  13:49:00  kevins
 * 	OT# 10311 10407 10414 10500 10499 10408
 * 	Remove unnecessary cds include files.
 * 	Update modify switches to work correctly according to the RFC.
 * 	Fix inconsistent error messages.
 * 	Display multivalued attributes correctly.
 * 	Set the attribute types appropriately.
 * 	Correct the display format to correctly display tcl lists.
 * 	[1994/05/03  12:58:08  kevins]
 * 
 * Revision 1.1.4.12  1994/04/20  18:18:49  kevins
 * 	CR 10406 encode/decode cds attributes by syntax
 * 	[1994/04/20  18:18:25  kevins]
 * 
 * Revision 1.1.4.11  1994/04/07  10:22:12  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:17:56  kevins]
 * 
 * 	CR 10304 Modifications to help remove cross component
 * 	              dependency build issues
 * 	[1994/04/07  10:10:38  kevins]
 * 
 * Revision 1.1.4.10  1994/03/26  11:56:06  kevins
 * 	CR 10230 removed instances of extern Tcl_Interp *interp from sources
 * 	[1994/03/26  11:55:52  kevins]
 * 
 * Revision 1.1.4.9  1994/03/24  10:21:28  kevins
 * 	CR 10081 modified -timeout on link create
 * 	[1994/03/24  10:21:14  kevins]
 * 
 * Revision 1.1.4.8  1994/03/12  21:54:38  peckham
 * 	Change dns_attlist[] reference to dns_Attribute().
 * 	[1994/03/12  21:52:37  peckham]
 * 
 * Revision 1.1.4.7  1994/03/10  15:49:15  kevins
 * 	CR10098 corrected bad list structure.
 * 	[1994/03/10  15:43:59  kevins]
 * 
 * Revision 1.1.4.6  1994/02/03  18:42:19  kevins
 * 	updated to conform to func spec
 * 	[1994/02/03  18:29:42  kevins]
 * 
 * Revision 1.1.4.5  1994/01/12  13:02:56  kevins
 * 	replaced bzero() function with memset
 * 	[1994/01/12  13:01:47  kevins]
 * 
 * Revision 1.1.4.4  1993/12/29  15:58:10  kevins
 * 	added function to read attr_info from cds_attributes file
 * 	[1993/12/29  15:55:26  kevins]
 * 
 * Revision 1.1.4.3  1993/12/27  15:05:51  kevins
 * 	Remove dce_error function call
 * 	[1993/12/27  15:04:03  kevins]
 * 
 * Revision 1.1.4.2  1993/12/22  18:12:34  kevins
 * 	Message catalog cleanup
 * 	[1993/12/22  18:07:57  kevins]
 * 
 * Revision 1.1.4.1  1993/12/20  20:06:17  kevins
 * 	Fixed to conform to style guide.
 * 	[1993/12/20  18:51:41  kevins]
 * 
 * Revision 1.1.2.3  1993/12/13  14:22:19  kevins
 * 	Updated messages for catalog.
 * 	[1993/12/13  13:48:11  kevins]
 * 
 * Revision 1.1.2.2  1993/10/22  11:16:35  kevins
 * 	updated to tcl7.0
 * 	[1993/10/22  11:15:29  kevins]
 * 
 * Revision 1.1.2.1  1993/10/21  19:39:36  kevins
 * 	completed the name change from dsh to dcecp
 * 	[1993/10/21  19:38:53  kevins]
 * 
 * Revision 1.1.2.9  1993/10/13  19:01:41  kevins
 * 	Added all cds attribute information
 * 	[1993/10/13  18:59:06  kevins]
 * 
 * Revision 1.1.2.8  1993/09/16  18:04:17  kevins
 * 	modified attribute information
 * 	[1993/09/16  18:03:33  kevins]
 * 
 * Revision 1.1.2.7  1993/09/13  14:03:58  kevins
 * 	modifored uuid decoding
 * 	[1993/09/13  14:02:11  kevins]
 * 
 * 	modified uuid decoding
 * 
 * Revision 1.1.2.6  1993/09/08  19:58:19  kevins
 * 	modified attribute decoding
 * 	[1993/09/08  19:51:01  kevins]
 * 
 * Revision 1.1.2.5  1993/08/23  14:27:43  kevins
 * 	Added attribute encoding/decoding information
 * 	[1993/08/23  14:16:26  kevins]
 * 
 * Revision 1.1.2.4  1993/08/03  17:37:24  kevins
 * 	Fix Submit Problem
 * 	[1993/08/03  17:36:17  kevins]
 * 
 * Revision 1.1.2.3  1993/08/03  17:08:10  kevins
 * 	Alter command return values
 * 	[1993/08/03  17:01:03  kevins]
 * 
 * Revision 1.1.2.2  1993/08/03  16:47:49  kevins
 * 	Initial submission for dsh
 * 	[1993/08/03  16:35:26  kevins]
 * 
 * $EndLog$
 */
/* 
 * INCLUDE FILES
 */
#
#include <stdio.h>
#include <locale.h>
#include <setjmp.h>
#include <errno.h>
#include <fcntl.h>
/*#include <limits.h>*/                     /* for UINT_MAX */

/*
 * Don't inherit stubbase.h's use of the exception based pthreads API.
 */
#ifndef NCK
#  define NCK
#endif
#include <dce/rpc.h>
#include <dce/stubbase.h>

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
#include <dce/rpc.h>
#include <dce/utc.h>
#include <dce/sys_time.h>
#include <dcecp.h> 
#include <util_general.h>
#include <util_var.h>
#include <util_login.h>
#include <ctype.h>
#include <dce/cds_clerkserver.h>
#include <util_binding.h>

/* INTERNAL VARIABLES */
typedef struct dcecp_attr_syntax_table_t {
    char *attribute_name;
    int  syntax;
} dcecp_attr_syntax_table_t;

dcecp_attr_syntax_table_t attr_syntax[] = {
    {stringFullName,       cdsFullName},
    {stringbyte,           cdsbyte},
    {stringReplicaPointer, cdsReplicaPointer},
    {stringsmall,          cdssmall},
    {stringTimestamp,      cdsTimestamp},
    {stringuuid,           cdsuuid},
    {stringVersion,        cdsVersion},
    {stringchar,           cdschar},
    {stringgdaPointer,     cdsgdaPointer},
    {stringCHDirectory,    cdsCHDirectory},
    {stringTimeout,        cdsTimeout},
    {stringParentPointer,  cdsParentPointer},
    {stringGroupMember,    cdsGroupMember}
};

unsigned32 dcp_g_cds_conf_level = 1;
unsigned32 dcp_g_cds_conf_level_values[] = {cdsConfLow,
                                            cdsConfMed,
                                            cdsConfHi};
char *dcp_g_cds_conf_level_names[] = {"low",
                                      "medium",
                                      "high"};

/* PROTOTYPES */
int validate_tower(char *, int);
char *format_byte_output(char *,int,boolean32);
char *format_byte_input(char *);
int dcp_format_tower(MemberValue_t *, char **);
int decode_replicapointer_towers(Set_t *, unsigned char *);
int get_cds_binding_string(byte_t *, char *);
AtomicValue_t *find_atomicvalue_in_setmember(SetMember_t *);
void my_uuid_to_string(uuid_t *,unsigned_char_p_t *, unsigned32 *);
int decode_timeout(cds_timeout_t *, unsigned char *);
void format_utc(utc_t *, char *);
void format_extension(utc_t *, char *);
int create_repset(Tcl_Interp *,cds_full_name_t, Set_t **);
cds_attrvalue_t *copy_attr_value(cds_attrvalue_t *);    
void strip_braces(char *buffer);

#define ATTRIBUTE_LIST "attribute_list"

#define APPEND_ATTRLIST_VALUE(buffer)                                        \
    if (buffer) {                                                            \
        char *tmp;                                                           \
        tmp = Tcl_GetVar(attr_interp, ATTRIBUTE_LIST, 0);                    \
        if (tmp)                                                             \
            Tcl_SetVar(attr_interp, ATTRIBUTE_LIST, "\n", TCL_APPEND_VALUE); \
        Tcl_SetVar(attr_interp, ATTRIBUTE_LIST, buffer, TCL_APPEND_VALUE);   \
    }

#define LEN_mbc(p) mblen(p, MB_CUR_MAX)

#define COPY_mbc_and_skip(s,d) { \
    int xxlen = LEN_mbc(s);      \
    if (0 < xxlen)               \
        do                       \
            *d++ = *s++;         \
        while (--xxlen);         \
}


/*
 * FUNCTION: F_small 
 *
 * OVERVIEW:
 * 
 * Converts attribute value of small into a string format. Storing 
 * finished product in a preallocated buffer that is passed in. 
 * 
 * INPUTS:
 *  cds_attrvalue_t *value_p - Attribute structure returned from cds
 *
 * OUTPUTS:
 * unsigned char *buffer - Buffer containing final string. This must 
 * be preallocated. 
 *
 * RETURNS:
 *  void
 */
void F_small(cds_attrvalue_t *value_p, char **buffer)
{
    int  val;

    *buffer = malloc(4);
    val = *((signed char *)&value_p->val_u.val_small);
    sprintf(*buffer, "%1hd", val);
}


/*
 * FUNCTION: F_short
 *
 * OVERVIEW:
 * 
 * INPUTS:
 *
 * OUTPUTS:
 *
 * RETURNS:
 */
void F_short(cds_attrvalue_t *value_p, char **buffer)
{
    int  val;

    val = value_p->val_u.val_short;
    sprintf((char *)buffer, "%1hd", val);
}


/*
 * FUNCTION: F_long 
 *
 * OVERVIEW:
 * 
 * Converts attribute value of long into a string format. Storing 
 * finished product in a preallocated buffer that is passed in. 
 * 
 * INPUTS:
 *  cds_attrvalue_t *value_p - Attribute structure returned from cds
 *
 * OUTPUTS:
 * unsigned char *buffer - Buffer containing final string. This must 
 * be preallocated. 
 *
 * RETURNS:
 *  void
 */
void F_long(cds_attrvalue_t *value_p, char **buffer)
{
    int  val;

    *buffer = malloc(50);
    val = value_p->val_u.val_long;
    sprintf((char *)buffer, "%1ld", val);
}


/*
 * FUNCTION: F_uuid 
 *
 * OVERVIEW:
 * 
 * Converts attribute value of uuid into a string format. Storing 
 * finished product in a preallocated buffer that is passed in. 
 * 
 * INPUTS:
 *  cds_attrvalue_t *value_p - Attribute structure returned from cds
 *
 * OUTPUTS:
 * char *buffer - Buffer containing final string. This must 
 * be preallocated. 
 *
 * RETURNS:
 *  void
 */

void F_uuid(cds_attrvalue_t *value_p,char **buffer)
{
    *buffer = malloc(UUID_STRING_LEN + 5);
    decode_uuid((uuid_t *)&value_p->val_u.val_uuid, *buffer);
}


/*
 * FUNCTION: F_version 
 *
 * OVERVIEW:
 * 
 * Converts attribute value of version into a string format. Storing 
 * finished product in a preallocated buffer that is passed in. 
 * 
 * INPUTS:
 *  cds_attrvalue_t *value_p - Attribute structure returned from cds
 *
 * OUTPUTS:
 * char *buffer - Buffer containing final string. This must 
 * be preallocated. 
 *
 * RETURNS:
 *  void
 */
void F_version(cds_attrvalue_t *value_p, char **buffer)
{
    unsigned int  major, minor;

    major = value_p->val_u.val_version.cdsMajVer;
    minor = value_p->val_u.val_version.cdsMinVer;

    *buffer = malloc(100);
    sprintf(*buffer, "%1d.%1d", major, minor);
}


/*
 * FUNCTION: F_timestamp 
 *
 * OVERVIEW:
 * 
 * Converts attribute value of timestamp into a string format. Storing 
 * finished product in a preallocated buffer that is passed in. 
 * 
 * INPUTS:
 *  cds_attrvalue_t *value_p - Attribute structure returned from cds
 *
 * OUTPUTS:
 * unsigned char *buffer - Buffer containing final string. This must 
 * be preallocated. 
 *
 * RETURNS:
 *  void
 */
void format_ts(Timestamp_t *tsp, unsigned char *);

void F_timestamp(cds_attrvalue_t *value_p,char **buffer)
{
    Timestamp_t *tsp = (Timestamp_t *)&value_p->val_u.val_timestamp;
    unsigned char timebuf[100];

    format_ts(tsp, timebuf);
    *buffer = malloc(strlen((char *)timebuf) + 1);
    strcpy(*buffer, (char *)timebuf);
    return;
}


/*
 * FUNCTION: F_fullname 
 *
 * OVERVIEW:
 * 
 * Converts attribute value of fullname into a string format. Storing 
 * finished product in a preallocated buffer that is passed in. 
 * 
 * INPUTS:
 *  cds_attrvalue_t *value_p - Attribute structure returned from cds
 *
 * OUTPUTS:
 * unsigned char *buffer - Buffer containing final string. This must 
 * be preallocated. 
 *
 * RETURNS:
 *  void
 */
void F_fullname(cds_attrvalue_t *value_p, char **buffer)
{
    int            name_len = CDS_FNAME_MAX;
    unsigned char  temp[CDS_FNAME_MAX];

    cdsCvtFullToStr(value_p->val_u.val_byte.byte_p, temp, &name_len);
    *buffer = malloc(CDS_FNAME_MAX);
    strcpy(*buffer, (char *)temp);
}


/*
 * FUNCTION:  F_timeout()
 *
 * OVERVIEW:
 * 
 * Parses the attribute structure for timeout expiration/extension times.
 * Puts ascii equivalent into buffer to be returned. Buffer should be 
 * preallocated with appropriate size. 
 *
 * INPUTS:
 *  cds_attrvalue_t   *value_p - Attribute structure. 
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  unsigned char     *buffer   - output buffer with attribute data
 *
 * RETURNS:
 *  int dcp_s_ok dcp_s_no_memory
 */
int F_timeout(cds_attrvalue_t *value_p, char **buffer)
{
    char  expire[100];
    char  extend[100];
    char  *expire_svc = NULL;
    char  *extend_svc = NULL;

    format_utc(&value_p->val_u.val_timeout.expire, expire);
    format_extension(&value_p->val_u.val_timeout.extend, extend);

    expire_svc = (char *)dce_sprintf(dcp_t_cds_expire);
    extend_svc = (char *)dce_sprintf(dcp_t_cds_extend);
    *buffer = malloc(strlen(expire_svc) + strlen(extend_svc) + 
                     strlen(expire) + strlen(extend) + 20);
    if (!expire_svc || !extend_svc)
        return dcp_s_no_memory;

    sprintf(*buffer, "{%s %s}\n {%s %s}", expire_svc, expire, extend_svc, extend);
    free(expire_svc);
    free(extend_svc);
    return dcp_s_ok;
}


/*
 * FUNCTION: F_member 
 *
 * OVERVIEW:
 * 
 * Converts attribute value of member into a string format. Storing 
 * finished product in a preallocated buffer that is passed in. 
 * 
 * INPUTS:
 *  cds_attrvalue_t *value_p - Attribute structure returned from cds
 *  int              flag    - whether to display Group/Member or Primary/Alias
 *
 * OUTPUTS:
 * unsigned char *buffer - Buffer containing final string. This must 
 * be preallocated. 
 *
 * RETURNS:
 *  int 
 *  	dcp_s_ok -        function executed correctly.
 *      dcp_s_no_memory - memory could not be allocated
 */
int F_member(cds_attrvalue_t *value_p, char **buffer, int flag)
{
    int            code;
    GroupMember_t  *memp = (GroupMember_t *)value_p->val_u.val_byte.byte_p;
    int            size = CDS_FNAME_MAX;
    int            max, used;
    char           valuebuf[CDS_FNAME_MAX];
    char           *group_svc = NULL;
    char           *member_svc = NULL;

    code = EXT8(memp->gm_isaGroup);	
    
    if (code) {
        if (cdsCvtFullToStr((cds_full_name_t *)memp->gm_member,
                            (unsigned char *)valuebuf,&size) != CDS_SUCCESS) {
            size = LEN_FullName((char *)memp->gm_member);
            max = (size > sizeof(FullName_u) ? sizeof(FullName_u) : size);
            used = (max < sizeof(FullName_overhead)) ?
                sizeof(FullName_overhead) : max;
            format((unsigned char *)memp->gm_member, used, (unsigned char *)valuebuf);
        }
        if (flag == 1)
            group_svc = (char *)dce_sprintf(dcp_t_cds_group);
        else
            group_svc = (char *)dce_sprintf(dcp_t_cds_primary);

        *buffer = malloc(strlen(group_svc) + strlen(valuebuf) + 3);
        if (!group_svc || !*buffer)
            return dcp_s_no_memory;
        sprintf(*buffer, "%s %s", group_svc, valuebuf);
        free(group_svc);
    }
    else {
        if (cdsCvtFullToStr((cds_full_name_t *)memp->gm_member,
                            (unsigned char *)valuebuf,
                            &size) != CDS_SUCCESS) {
            size = LEN_FullName((char *)memp->gm_member);
            max = (size > sizeof(FullName_u) ? sizeof(FullName_u) : size);
            used = (max < sizeof(FullName_overhead)) ?
                sizeof(FullName_overhead) : max;
            format((unsigned char *)memp->gm_member, used, (unsigned char *)valuebuf);
        }
        if (flag == 1)
            member_svc = (char *)dce_sprintf(dcp_t_cds_member);
        else
            member_svc = (char *)dce_sprintf(dcp_t_cds_alias);

        *buffer = malloc(strlen(member_svc) + strlen(valuebuf) + 3);
        if (!member_svc)
            return dcp_s_no_memory;
        sprintf(*buffer, "%s %s", member_svc, valuebuf);
        free(member_svc);
    }

    return dcp_s_ok;
}


/*
 * FUNCTION: F_rp 
 *
 * OVERVIEW:
 *   Converts attribute value of replica pointer into a string format. 
 *   Storing finished product in a preallocated buffer that is passed in. 
 *
 * INPUTS:
 *  cds_attrvalue_t *value_p - Attribute structure returned from cds
 *
 * OUTPUTS:
 * unsigned char *buffer - Buffer containing final string. This must 
 * be preallocated. 
 *
 * RETURNS:
 *  int 
 *  	dcp_s_ok -        function executed correctly.
 *      dcp_s_no_memory - memory could not be allocated
 *
 */
int F_rp(cds_attrvalue_t *value_p, char **buffer)
{
    unsigned char    *addressh;
    unsigned int     replica_type;
    unsigned char    *clearinghouse_name;
    char             uuid_p[UUID_STRING_LEN];
    uuid_t           uuid;
    char             *rep_type;
    unsigned char    tower[CDS_STR_FNAME_MAX];
    cds_full_name_t  clr_house_p;
    int              status = dcp_s_ok;
    char             *temp;
    char             *rep_type_svc = NULL;
    char             *uuid_svc = NULL;
    char             *clearinghouse_name_svc = NULL;

    /* 
     * Aquire the pointers for the various fields. First assign the 
     * byte pointer from the attribute value structure to a structure 
     * of type replicapointer. 
     */
    ReplicaPointer_t *rp_p = (ReplicaPointer_t *)value_p->val_u.val_byte.byte_p;
    temp = (char *)FLD_ReplicaPointer_replicaType(rp_p);
    replica_type = (int )*temp; 
    addressh = (unsigned char *)rp_p->rp_addressHint;
    clearinghouse_name = (unsigned char *)FLD_ReplicaPointer_CHName(rp_p);

    memset((char *)tower, 0, CDS_STR_FNAME_MAX);

    /*
     * Realign the uuid as provided from the rep pointer struct. 
     */
    memcpy(&uuid, rp_p->rp_CHID, sizeof(uuid_t));
    status = decode_uuid(&uuid, uuid_p);
    if ((status = decode_replicatype(replica_type, &rep_type)) != dcp_s_ok)
        return status;

    decode_clearinghouse_name((char *)clearinghouse_name,
                              (char *)clr_house_p.uc);
    status = decode_replicapointer_towers((Set_t *)addressh, tower);
    if (status != dcp_s_ok)
        return status;

    rep_type_svc = (char *)dce_sprintf(dcp_t_cds_reptype);
    uuid_svc = (char *)dce_sprintf(dcp_t_cds_clruuid);
    clearinghouse_name_svc = (char *)dce_sprintf(dcp_t_cds_clrname);
    *buffer = malloc(strlen((char *)uuid_svc) + strlen(uuid_p) +
                     strlen(clearinghouse_name_svc) +
                     strlen((char *)clr_house_p.uc) +
                     strlen((char *)rep_type_svc) + strlen(rep_type) +
                     strlen((char *)tower) + 30);
    if (!rep_type_svc || !uuid_svc || !clearinghouse_name_svc || !*buffer)
        return dcp_s_no_memory;
    sprintf(*buffer, "{%s %s}\n  {%s %s}\n  {%s %s}  %s",
            uuid_svc, (char *)uuid_p,
            clearinghouse_name_svc, clr_house_p.uc,
            rep_type_svc,(char *)rep_type,
            tower);
    free(rep_type_svc);
    free(uuid_svc);
    free(rep_type);
    free(clearinghouse_name_svc);

    return dcp_s_ok;
}


/*
 * FUNCTION: F_pp 
 *
 * OVERVIEW:
 * Converts attribute value of parent pointer into a string format. Storing 
 * finished product in a preallocated buffer that is passed in. 
 * 
 * INPUTS:
 *  cds_attrvalue_t *value_p - Attribute structure returned from cds
 *
 * OUTPUTS:
 * char *buffer - Buffer containing final string. This must 
 * be preallocated. 
 *
 * RETURNS:
 *  *  int 
 *  	dcp_s_ok -        function executed correctly.
 *      dcp_s_no_memory - memory could not be allocated
 */
int F_pp(cds_attrvalue_t *value,char **buffer)
{
    ParentPointer_t  *pp = (ParentPointer_t *)value->val_u.val_byte.byte_p;
    uuid_t           uuid;
    char             uuid_p[UUID_STRING_LEN];
    unsigned32       status = 0; 
    unsigned char    timeoutbuf[1024];
    int              name_len = CDS_SNAME_MAX;
    unsigned char    temp_buffer[CDS_SNAME_MAX];
    char             *parent_svc = NULL;
    char             *myname_svc = NULL;

    memcpy(&uuid,pp->pp_parentID,sizeof(uuid_t));
    status = decode_uuid(&uuid, uuid_p);
    if (status != uuid_s_ok)
        return status;
    status = decode_timeout((cds_timeout_t *)pp->pp_timeout, timeoutbuf);
    if (status != dcp_s_ok)
        return status;
    cdsCvtFullToStr((cds_full_name_t *)pp->pp_myName, temp_buffer, &name_len);
    parent_svc = (char *)dce_sprintf(dcp_t_cds_dir_parent_ptr);
    myname_svc = (char *)dce_sprintf(dcp_t_cds_dir_myname);
    *buffer = malloc(strlen(parent_svc) +  strlen(uuid_p) +
                     strlen((char *)timeoutbuf) +  strlen(myname_svc) +  
                     strlen((char *)temp_buffer) + 20);
    if (!parent_svc || !myname_svc)
        return dcp_s_no_memory;
    sprintf(*buffer,"{%s %s}\n  %s\n  {%s %s}",
            parent_svc, uuid_p, timeoutbuf, myname_svc, temp_buffer);
    free(parent_svc);
    free(myname_svc);
    return dcp_s_ok;
}


/*
 * FUNCTION: F_gp 
 *
 * OVERVIEW:
 * Converts attribute value of gda pointer into a string format. Storing 
 * finished product in a preallocated buffer that is passed in. 
 * 
 * INPUTS:
 *  cds_attrvalue_t *value_p - Attribute structure returned from cds
 *
 * OUTPUTS:
 * char *buffer - Buffer containing final string. This must 
 * be preallocated. 
 *
 * RETURNS:
 *  int
 *       dcp_s_ok - function executed correctly
 *       dcp_s_no_memory - function could not allocate memory. 
 */
int F_gp(cds_attrvalue_t *value_p, char **buffer)
{
    register gdaPointer_t      *gp_p = (gdaPointer_t *)value_p->val_u.val_byte.byte_p;
    register ReplicaPointer_t  *rp_p = (ReplicaPointer_t *)gp_p->gp_replica;

    unsigned char    *addressh;
    unsigned int     replica_type;
    unsigned char    *clearinghouse_name;
    char             uuid_p[UUID_STRING_LEN];
    uuid_t           uuid;
    char             *rep_type;
    unsigned char    tower[CDS_STR_FNAME_MAX];
    cds_full_name_t  clr_house_p;
    int              status = dcp_s_ok;
    char             *temp;
    cds_timeout_t    *timeout;
    unsigned char    timeoutbuf[1024];
    char             *rep_type_svc = NULL;
    char             *uuid_svc = NULL;
    char             *clearinghouse_name_svc = NULL;

    /* 
     * Aquire the pointers for the various fields. First assign the 
     * byte pointer from the attribute value structure to a structure 
     * of type replicapointer. 
     */
    timeout = (cds_timeout_t *)gp_p->gp_timeout;
    temp = (char *)FLD_ReplicaPointer_replicaType(rp_p);
    replica_type = (int)*temp;
    addressh = (unsigned char *)rp_p->rp_addressHint;
    clearinghouse_name = (unsigned char *)FLD_ReplicaPointer_CHName(rp_p);

    memset((char *)tower, 0, CDS_STR_FNAME_MAX);

    status = decode_timeout((cds_timeout_t *)gp_p->gp_timeout, timeoutbuf);
    if (status != dcp_s_ok)
        return status;

    /* 
     * Realign the uuid as provided from the rep pointer struct. 
     */
    memcpy(&uuid,rp_p->rp_CHID, sizeof(uuid_t));
    status = decode_uuid(&uuid, uuid_p);
    if (status != uuid_s_ok)
        return status;
    decode_replicatype(replica_type, &rep_type);
    decode_clearinghouse_name((char *)clearinghouse_name,
                              (char *)clr_house_p.uc);
    status = decode_replicapointer_towers((Set_t *)addressh, tower);
    if (status != dcp_s_ok)
        return status;

    rep_type_svc = (char *)dce_sprintf(dcp_t_cds_reptype);
    uuid_svc = (char *)dce_sprintf(dcp_t_cds_clruuid);
    clearinghouse_name_svc = (char *)dce_sprintf(dcp_t_cds_clrname);
    if (!rep_type_svc || !uuid_svc || !clearinghouse_name_svc)
        return dcp_s_no_memory;
    *buffer = malloc(strlen((char *)timeoutbuf) + strlen(uuid_svc) +
                     strlen((char *)uuid_p) + strlen(clearinghouse_name_svc) + 
                     strlen((char *)clr_house_p.uc) + strlen(rep_type_svc) + 
                     strlen((char *)rep_type)  + strlen((char *)tower) + 50);
    sprintf(*buffer,"%s\n  {%s %s}\n  {%s %s}\n  {%s %s} %s",
            timeoutbuf, uuid_svc, (char *)uuid_p,
            clearinghouse_name_svc, clr_house_p.uc, rep_type_svc, 
            (char *)rep_type, tower);
    free(rep_type);
    free(rep_type_svc);
    free(uuid_svc);
    free(clearinghouse_name_svc);
    return dcp_s_ok;
}


/*
 * FUNCTION: F_chdirectory 
 *
 * OVERVIEW:
 * Converts attribute value of chdirectory into a string format. Storing 
 * finished product in a preallocated buffer that is passed in. 
 * 
 * INPUTS:
 *  cds_attrvalue_t *value_p - Attribute structure returned from cds
 *
 * OUTPUTS:
 * unsigned char *buffer - Buffer containing final string. This must 
 * be preallocated. 
 *
 * RETURNS:
 *  int
 *     dcp_s_ok - function executed correctly 
 *     dcp_s_no_memory - function could not allocate memory
 */
int F_chdirectory(cds_attrvalue_t *value_p,char **buffer)
{
    CHDirectoryPointer_t  *chdir = (CHDirectoryPointer_t *)value_p->val_u.val_byte.byte_p;
    char                  uuid[UUID_STRING_LEN];
    int                   name_len = CDS_FNAME_MAX;
    unsigned              char name[CDS_FNAME_MAX];
    uuid_t                uuid_p;
    unsigned char         *uuid_temp = NULL;
    unsigned char         *name_temp = NULL;

    memset(uuid, 0, sizeof(uuid));
    memcpy(&uuid_p, chdir->cp_dirID, sizeof(uuid_t));
    decode_uuid(&uuid_p, (char *)uuid);
    cdsCvtFullToStr((cds_full_name_t *)chdir->cp_directory, name, &name_len);
    uuid_temp = dce_sprintf(dcp_t_cds_dir_uuid);
    name_temp = dce_sprintf(dcp_t_cds_dir_name);
    if (!uuid_temp || !name_temp)
        return dcp_s_no_memory;
    *buffer = malloc(strlen((char *)uuid_temp) + strlen(uuid) + 
                     strlen((char *)name_temp) + strlen((char *)name) + 20);
    sprintf(*buffer,"{%s %s}\n  {%s %s}",(char *)uuid_temp,uuid,
            (char *)name_temp,(char *)name);
    free(uuid_temp);
    free(name_temp);
    return dcp_s_ok;
}


/*
 * FUNCTION: F_char 
 *
 * OVERVIEW: This function converts a cds_char attribute into an        
 *    ascii string. 
 *
 * INPUTS:
 *  cds_attrvalue_t *value_p the attribute structure. 
 *
 * OUTPUTS:
 *  char **buffer the buffer to hold the resulting string. 
 *
 * RETURNS:
 *  void
 */
void F_char(cds_attrvalue_t *value,char **buffer)
{
    *buffer = malloc(value->val_u.val_char.length + 1);
    strncpy(*buffer, value->val_u.val_char.char_p, value->val_u.val_char.length);
    *(*buffer + value->val_u.val_char.length) = '\0';
}


/*
 * FUNCTION: F_byte 
 *
 * OVERVIEW:
 * Converts attribute value of byte into a string format. Storing 
 * finished product in a preallocated buffer that is passed in. 
 * 
 * INPUTS:
 *  cds_attrvalue_t *value_p - Attribute structure returned from cds
 *
 * OUTPUTS:
 * unsigned char *buffer - Buffer containing final string. This must 
 * be preallocated. 
 *
 * RETURNS:
 *  void
 */
void F_byte(cds_attrvalue_t *value_p, char **buffer)
{
    unsigned char   *ch_p;
    unsigned short  i;
    unsigned char   *thechar;

    *buffer = malloc(4 * value_p->val_u.val_byte.length);
    memset(*buffer, 0, 4 * value_p->val_u.val_byte.length);
    ch_p = (unsigned char *)*buffer;
    thechar = value_p->val_u.val_byte.byte_p;

    /*
     * Convert each byte into hex pairs
     */
    for (i = 0; i < value_p->val_u.val_byte.length; i++) {
        sprintf((char *)ch_p, "%02.2x", *thechar++);
        ch_p += 2;
    }
    *ch_p = '\0';
}


/*
 * FUNCTION: decode_replicatype
 *
 * OVERVIEW:
 *
 * INPUTS:
 * int   type_int - replica type enumeration.
 * char  *type_ptr - replica type string.
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int 
 *      dcp_s_ok - function performed ok
 *      dcp_s_no_memory - unable to allocate memory
 */
int decode_replicatype(int type_int, char **type_ptr)
{
    switch(type_int) {
        case cdsMaster:
            *type_ptr = (char *)dce_sprintf(dcp_t_cds_rep_master);
            break;
        case cdsSecondary:
            *type_ptr = (char *)dce_sprintf(dcp_t_cds_rep_secondary);
            break;
        case cdsReadOnly:
            *type_ptr = (char *)dce_sprintf(dcp_t_cds_rep_readonly);
            break;
        case cdsGda:
            *type_ptr = (char *)dce_sprintf(dcp_t_cds_rep_gda);
            break;
        default:
            *type_ptr = (char *)dce_sprintf(dcp_t_cds_rep_gda);
            break;
    }
    if (!*type_ptr)
        return dcp_s_no_memory;
    return dcp_s_ok;
}


/*
 * FUNCTION: format
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void format(unsigned char *bufptr, int size, unsigned char *x)
{
    int i;				/* loop control */
    int printables;			/* number of printable chars in input*/
    int nonprints;			/* number that will not print */
    int numquotes;			/* number of quote chars in input */
    unsigned char *p;			/* ptr to cur pos in output buffer */
    static unsigned char  hex[] = "0123456789abcdef";	
    unsigned char c;			/* work area for UNSIGNED character */
    
    numquotes = 0;			/* no quotes seen yet */
    printables = nonprints = 0;		/* nothing seen yet */
    for (i = 0; i < size; i++)		/* look at each character */
    {
        if (isprint(*(bufptr+i)))
            printables++;		/* count the printable chars */
        else
            nonprints++;		/* count the non printable chars */
        if (*(bufptr+i) == '"')
            numquotes++;		/* count the quotes */
    };					/* looked at each character */
    
    if ((nonprints == 0) && (numquotes == 0))
    {
        *x = '"';			/* opening quote */
        memcpy(x+1, bufptr, size);	/* the data, problem-free */
        *(x+size+1) = '"';		/* terminal quote */
        *(x+size+2) = 0;		/* terminal null  */
    }
    else if (nonprints == 0)		/* all printable but embedded quotes */
    {
        *x = '"';			/* opening quote */
        p = x + 1;			/* point to current position */
        for (i = 0; i < size; i++)
	{
            if (*(bufptr+i) == '"')	/* is this a quote char */
	    {				/* yes, so copy two of them */
                *p++ = '"';		/* the first one */
                *p++ = '"';		/* the second one */
	    }
            else			/* it is a normal character */
	    {
                *p++ = *(bufptr+i);	/* copy character and bump ptr */
	    };
	};				/* end of for, each character moved */
        *p++ = '"';			/* add the terminal quote mark */
        *p   = '\0';			/* end of string marker */
    }
    else if (9*(nonprints + numquotes) > printables)
    {					/* messy, so use hex */
        p = x;				/* point to output buffer */
        *p++ = '%';			/* 1st char of hex introducer */
        *p++ = 'x';			/* 2nd char of hex introducer */
        for (i = 0; i < size; i++)	/* look at each character */
	{
            c = *(bufptr+i);		/* pick up the char */
            *p++ = hex[c>>4];		/* hex equiv of top 4 bits */
            *p++ = hex[c&15];		/* hex equiv of bottom 4 bits */
	};				/* end of for loop for hex value */
        *p = '\0';			/* mark end of string */
    }					/* end of hex option */
    else				/* nothing else works, so ... */
    {
        p = x;				/* point at output buffer */
        *p++ = '"';			/* opening quote of asciz string */
        for (i = 0; i < size; i++)	/* look at each char in turn */
	{
            c = *(bufptr+i);		/* pick up the character */
            if (isprint(c))		/* printable character ? */
	    {				/* yes */
                *p++ = c;		/* copy the char to output buffer */
	    }
            else			/* nonprintable so use \ form */
	    {				/* remember '\\' means one \ */
                *p++ = '\\';		/* start of c style escape sequence */
                sprintf((char *)p, "%3.3o",c);/* 3 positions of octal value */
                p += 3;			/* point at the null */
	    };
	};
        *p++ = '"';			/* ending quote for string */
        *p = '\0';			/* null terminate the string */
    };
}					/* end of format routine */


/*
 * FUNCTION: decode_clearinghouse_name
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
int decode_clearinghouse_name(char *name_in, char *name_out)
{
    int  size = CDS_STR_FNAME_MAX;
    int  cds_status = CDS_SUCCESS;

    cds_status = cdsCvtFullToStr((cds_full_name_t *)name_in,
                                 (unsigned char *)name_out,
                                 &size);
    return dcp_s_ok;
}


/*
 * FUNCTION: decode_tower
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
int decode_tower(void *tower, 
                 unsigned short length,
                 char **buffer)
{
    int status = CDS_SUCCESS;

    status = dcp_format_tower(tower, buffer);
    return status;
}


/*
 * FUNCTION: validate_tower
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
int validate_tower(char *p, int size)
{
    char  *cp;			/* to scan across the tower */
    int   pieces;		/* count pieces here. 2 per floor */
    int   floors;		/* number of floors in tower */

    cp = p;
    pieces = 0;
    floors = (int)GETL16(cp);		/* save number of floors. advance ptr */
    while (cp < p+size) {
        pieces++;			/* count these counted strings */
        cp += GETL16(cp);		/* now advance the pointer */
    }

    /*
     * Check that we had the right number of pieces and just used up all input.
     */
    return ((size > 1) && (cp == p+size) && (2 * floors == pieces));
}


/*
 * FUNCTION: dcp_format_tower
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 *
 */
int dcp_format_tower(MemberValue_t *tower, char **towerstr)
{
    byte_t                *data_p;
    int	                  count;
    char                  *temp_binding;
    error_status_t        rpc_status;
    char                  string[CDS_FNAME_MAX];
    unsigned char         *string_bind;
    rpc_binding_handle_t  h_p;

    data_p = (byte_t *)tower;
    count = GETL16(data_p);	/* get number of tower floors */

    /*
     * First try to use RPC to convert the complete tower for us.
     */
    rpc_tower_to_binding((unsigned char *)tower, &h_p, &rpc_status);

    if (rpc_status == rpc_s_ok)
        rpc_binding_to_string_binding(h_p, &string_bind, &rpc_status);

    if (rpc_status == rpc_s_ok) {
        *towerstr = malloc(strlen((char *)string_bind) + 10);
        temp_binding = dcp_binding_string_to_tcl((char *)string_bind, &rpc_status);
        if (rpc_status != rpc_s_ok)
            return rpc_status;
        if (temp_binding)
            sprintf(*towerstr, "{%s}", temp_binding );
        rpc_string_free (&string_bind, &rpc_status);
        return CDS_SUCCESS;
    }

    /*
     * If RPC fails because the tower was not an RPC tower, see if
     * it might be our own two-floor protocol tower. If so, show the
     * floors as binding strings, else revert to the "ugly" way of
     * showing the floors individually.
     */
    if (rpc_status == rpc_s_not_rpc_tower) {
        if ((count == 2) && (get_cds_binding_string((byte_t *)tower, string))) {
            *towerstr = malloc(strlen(string) + 4);
            strcpy(*towerstr, string);
            return CDS_SUCCESS;
        }
    }

    /*
     * If we get here, it means we had an rpc error that we
     * cannot recover from.  Return the error status.
     */
    return rpc_status;
}


/*
 * FUNCTION: decode_replicapointer_towers
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 *
 */
int decode_replicapointer_towers(Set_t *addressh, unsigned char *buffer)
{
    MemberValue_t  *memberset;		/* ptr to a member of the set */
    AtomicValue_t  *atomicvalue;	/* ptr to atomic value of member */
    bytes_u        *tower_p;
    int            members, len, i;
    char           *temp;
    char           temp1[CDS_FNAME_MAX];
    char           *tower_svc = NULL;
    int            status;
    int            j;
    unsigned char  *thechar;
    unsigned char  *ch_p;

    memset(temp1, 0, CDS_FNAME_MAX);

    members = (int)NUM_Set(addressh);

    for (i = 0; i < members; i++) {
        memberset = (MemberValue_t *)INDEX_Set(addressh, i);
        atomicvalue = find_atomicvalue_in_setmember((SetMember_t *)memberset);
        tower_p = &(atomicvalue->av_value[0]);
        len = GET16(atomicvalue);
        tower_svc = (char *)dce_sprintf(dcp_t_cds_tower);
        if (!tower_svc)
            return dcp_s_no_memory;
        status = decode_tower((void *)tower_p, len, &temp);
        if (status != CDS_SUCCESS) {
            temp = malloc(4 * len);
            memset(temp, 0, 4 * len);
            thechar = tower_p;
            ch_p = (unsigned char *)temp;
            for (j = 0; j < len; j++) {
                sprintf((char *)ch_p, "%02.2x", *thechar++);
                ch_p += 2;
            }
            *ch_p = '\0';   
            temp = format_byte_output(temp, 3, TRUE);
            sprintf((char *)buffer,"%s\n  {%s\n   {%s}}", temp1, tower_svc, temp);
        }
        else {
            sprintf((char *)buffer,"%s\n  {%s %s}", temp1, tower_svc, temp);
        }
        free(tower_svc);
        free(temp);
        strcpy(temp1,(char *)buffer);
    }
    return dcp_s_ok;
}


/*
 * FUNCTION: find_atomicvalue_in_setmember
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
AtomicValue_t *find_atomicvalue_in_setmember(SetMember_t *smp)
{
    return (AtomicValue_t *)(&(smp->sm_value[0])); /* skip over flag and timestamp */
}


/*
 * FUNCTION: decode_timeout
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
int decode_timeout(cds_timeout_t *timeout, unsigned char *buffer)
{
    cds_timeout_t  align_timeout;
    char           expiration_buf[1024];
    char           extension_buf[1024];
    char           *expire_svc = NULL;
    char           *extend_svc = NULL;
    char           *timeout_svc = NULL;

    memset(expiration_buf, 0, sizeof(expiration_buf));
    memset(extension_buf, 0, sizeof(extension_buf));
    memcpy(&align_timeout, timeout, sizeof(timeout));
    format_utc((utc_t *)&timeout->expire, expiration_buf);
    format_extension((utc_t *)&timeout->extend, extension_buf);
    expire_svc = (char *)dce_sprintf(dcp_t_cds_expire);
    extend_svc = (char *)dce_sprintf(dcp_t_cds_extend);
    timeout_svc =  (char *)dce_sprintf(dcp_t_cds_timeout);
    if (!expire_svc || !extend_svc || !timeout_svc)
        return dcp_s_no_memory;
    sprintf((char *)buffer,"{%s\n   {%s %s}\n   {%s %s}}",
            timeout_svc, expire_svc,expiration_buf, extend_svc, extension_buf);
    free(expire_svc);
    free(extend_svc);
    free(timeout_svc);
    return dcp_s_ok;
}


/*
 * FUNCTION: format_utc
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void format_utc(utc_t *utc, char *buffer)
{
    int     status;
    char    timebuf[99];
    size_t  size = sizeof(timebuf);

    status = utc_asclocaltime(timebuf,	/* ptr to buffer for results */
                              size,	/* size available */
                              utc);	/* ptr to 128bit utc to be formatted */
    if (status == 0) {
        char *p = strchr(timebuf, 'I');		/* find the inaccuracy portion */
        if (p)
            *p = '\0';				/* suppress the inaccuracy; back up */
	p = strrchr(timebuf, '+');		/* If positive timezone is displayed */
	if (p) {
	    *p = '\0';				/* clip it off */
	} else {
	    p = strrchr(timebuf, '-');		/* If timezone is displayed */
            
	    if (p)
		*p = '\0';			 /* clip it off */
	}
	strcpy((char *)buffer, timebuf);	 /* move string to user buffer */
    }
    else
    {
#ifdef SNI_SVR4
        strcpy(timebuf, "<???\?-?\?-?\?-??:??:??.??\?>");
#else
        strcpy(timebuf, "<????-??-??-??:??:??.???>");
#endif
    };
    strcpy((char *)buffer, timebuf);
    return;
}


/*
 * FUNCTION: format_utc
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void format_extension(utc_t *utc, char *buffer)
{
    int     status;			/* result of subroutine */
    size_t  len;			/* size of buffer */
    char    *temp_buf;			/* to scan across the time part of utc*/

    /* zero time is a special case. */
    /* violate abstraction of utc, because approved way is even worse */
    temp_buf = (char *)utc;			/* point at time */
    if (temp_buf[0] || temp_buf[1] || temp_buf[2] || temp_buf[3] || 
        temp_buf[4] || temp_buf[5] || temp_buf[6] || temp_buf[7]) {
        len = 1024;
        status = utc_ascreltime((char *)buffer, len, utc);	
        if (status != 0) {
#ifdef SNI_SVR4
            /*
             * This should probably not be an ifdef, the SNI_SVR4
             * code is generic ANSI.
             */
            strcpy((char *)buffer, "<?\?\?-??:??:??.?\?\?>");
#else
            strcpy((char *)buffer, "<???-??:??:??.???>");
#endif
	};
    }
    else
        strcpy((char *)buffer, "0");	/* special form of null time */
    return;
}


/*
 * FUNCTION: clear_flags
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void clear_flags(struct cdsFlagStat *flags, cdsNameType_t name_type)
{
    memset((char *)flags, 0, sizeof(struct cdsFlagStat));
    flags->fsPartialRes = (cds_full_name_t *)partialRes;
    flags->fsResLength = sizeof(partialRes);
    flags->fsNameType =  name_type; 
    flags->fsCacheTimeout = 0;  
    flags->fsUnAuth = flags->fsTrustAll = !Authenticat;
    flags->fsConf = (int)dcp_g_cds_conf_level_values[dcp_g_cds_conf_level];
    flags->fsDontCache = (dcp_g_cds_conf_level == 2) ? 1 : 0;
}


/*
 * FUNCTION: do_sec_login
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void do_sec_login()
{
    /* Get current context and save it */

    if (dcp_is_logged_in()) {
        Authenticat = 1;
    }
    else {
        Authenticat = 0;
    }
}


/*
 * FUNCTION: convergence
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int
 */
int convergence(cds_attrvalue_t *value,char **buffer)
{
    unsigned32  converg;
    char        *temp = NULL;

    F_small(value, &temp);
    converg = atoi(temp);
    free(temp);

    switch ((int)converg) {
        case cdsConfLow:
            *buffer = (char *)dce_sprintf(dcp_t_cds_low);
            break;
        case cdsConfMed:
            *buffer = (char *)dce_sprintf(dcp_t_cds_medium);
            break;
        case cdsConfHi:
            *buffer = (char *)dce_sprintf(dcp_t_cds_high);
            break;
        default:
            *buffer = (char *)dce_sprintf(dcp_t_cds_medium);
            break;
    }
    if (!*buffer)
        return dcp_s_no_memory;
    return dcp_s_ok;
}


/*
 * FUNCTION: replicatype
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
int replicatype(cds_attrvalue_t *value,char **buffer)
{
    char  *number;
    int   int_number;

    F_small(value, &number);
    int_number = atoi(number);
    free(number);
    return decode_replicatype(int_number, buffer);
}


/*
 * FUNCTION: replicastate
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
int replicastate(cds_attrvalue_t *value,char **buffer)
{
    unsigned32  state = 3;
    char        *temp = NULL;

    F_small(value, &temp);
    state = atoi(temp);
    free(temp);

    switch ((int)state) {
        case RS_newDir:
            *buffer = (char *)dce_sprintf(dcp_t_cds_rs_newdir);
            break;
        case RS_newReplica:
            *buffer = (char *)dce_sprintf(dcp_t_cds_rs_newrep);
            break;
        case RS_on:
            *buffer = (char *)dce_sprintf(dcp_t_cds_rs_on);
            break;
        case RS_dyingDir:
            *buffer = (char *)dce_sprintf(dcp_t_cds_rs_dyingdir);
            break;
        case RS_dyingReplica:
            *buffer = (char *)dce_sprintf(dcp_t_cds_rs_dyingrep);
            break;
        case RS_dead:
            *buffer = (char *)dce_sprintf(dcp_t_cds_rs_dead);
            break;
        default:
            *buffer = (char *)dce_sprintf(dcp_t_cds_clr_unknown_state);
            break;
    }
    if (!*buffer)
        return dcp_s_no_memory;
    return dcp_s_ok;
}


/*
 * FUNCTION: chstate
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
int chstate(cds_attrvalue_t *value,char **buffer)
{
    char        *number;
    unsigned32  state = 2;

    F_small(value, &number);
    state = atoi(number);
    free(number);

    switch ((int)state) {
        case clearinghouse_Off:
            *buffer = (char *)dce_sprintf(dcp_t_cds_clr_off);
            break;
        case clearinghouse_Initial:
            *buffer = (char *)dce_sprintf(dcp_t_cds_clr_initial);
            break;
        case clearinghouse_On:
            *buffer = (char *)dce_sprintf(dcp_t_cds_clr_on);
            break;
        case clearinghouse_Shut:
            *buffer = (char *)dce_sprintf(dcp_t_cds_clr_dying);
            break;
        case clearinghouse_Broken:
            *buffer = (char *)dce_sprintf(dcp_t_cds_clr_broken);
            break;
        default:
            *buffer = (char *)dce_sprintf(dcp_t_cds_clr_unknown_state);
            break;
    }
    if (!*buffer)
        return dcp_s_no_memory;
    return dcp_s_ok;
}


/*
 * FUNCTION: chlastaddress
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
int chlastaddress(cds_attrvalue_t *value,char **buffer)
{
    int              need_bytes, attr_bytes;  
    unsigned_char_t  **string_bindings;
    int              status = CDS_SUCCESS;
    unsigned32       rpc_status;
    int              attr_index = 0;
    int              total_attr_bytes = 0;
    Set_t            *towerset;
    byte_t           *towerset_datap;
    byte_t           *towerset_offsetp;
    int              towerset_offset;
    char             *temp = NULL;
    char             *temp_binding;

    /*
     * Arbitrarily large array of attribute values in the special
     * for of MemberHeader and AtomicValue.
     */
    MemberValue_t   *MemberValues[16];

    /*
     * Allocate some space.
     */
    attr_bytes = value->val_u.val_byte.length;
    need_bytes = sizeof(MemberValue_overhead) + attr_bytes;
    MemberValues[attr_index] = (MemberValue_t *)malloc(need_bytes);

    if (MemberValues[attr_index] == NULL) {
        status = CDS_NONSRESOURCES;
        return status;
    }
    ZERO_bytes(MemberValues[attr_index], need_bytes);

    /*
     * Make a copy of this attribute and bump the total
     * number of attribute bytes that we have accumulated
     * and the number of attributes that we have read
     * successfully.
     */
    SET_FLAG(MemberValues[attr_index]->mv_flag, MV_present);
    INS16(MemberValues[attr_index]->mv_length, (int)attr_bytes);
    INS8(MemberValues[attr_index]->mv_valuetype, dns_byte);
    COPY_bytes (value->val_u.val_byte.byte_p,
                MemberValues[attr_index]->mv_value,
                attr_bytes);
    total_attr_bytes += attr_bytes;

    /*
     * Now that we have accumulated all the attribute values, build
     * a towerset of all the tower members.
     */
    if (status == CDS_SUCCESS) {
        int need_bytes, copy_bytes, value_bytes;

        /*
         * Allocate enough space for the towerset.
         */
        need_bytes = sizeof (Set_overhead) + sizeof(word_u) +
            sizeof (MemberValue_overhead) + total_attr_bytes;
        towerset = (Set_t *)malloc(need_bytes);
        if (towerset == NULL) {
            status = CDS_NONSRESOURCES;
            free(MemberValues[attr_index]);
            return status;
        }

        /*
         * Initialize some towerset overhead.
         */
        SET_FLAG(towerset->st_flag, ST_present);
        INS16(towerset->st_offsetLength, sizeof(word_u));
        value_bytes = sizeof(MemberValue_overhead) + total_attr_bytes;
        INS16(towerset->st_valueLength, value_bytes);

        /*
         * Now add each of the individual attributes to the towerset
         */
        towerset_datap = DATA_Set((Set_u *)towerset);
        towerset_offset = towerset_datap - (byte_t *)towerset;
        towerset_offsetp = OFFSET_Set((Set_u *)towerset, attr_index);
        INS16(towerset_offsetp, towerset_offset);
        copy_bytes = sizeof(MemberValue_overhead) +
            EXT16(MemberValues[attr_index]->mv_length);
        COPY_bytes (MemberValues[attr_index],
                    towerset_datap,
                    copy_bytes);
        towerset_datap  += copy_bytes;
        towerset_offset += copy_bytes;
    }
    status = cdsCvtTowerToStr((char *)towerset, (char **)&string_bindings);
    free (towerset);

    if (status == CDS_SUCCESS) {
        temp = (char *)dce_sprintf(dcp_t_cds_tower);
        if (!temp)
            return dcp_s_no_memory;
        temp_binding = dcp_binding_string_to_tcl((char *)string_bindings[0],
                                                 &rpc_status);
        if (!temp_binding) {
            free(temp);
            return status;
        }
        *buffer = malloc(strlen(temp) + strlen(temp_binding) + 6);
        sprintf(*buffer, "%s {%s}", temp, temp_binding);
        free(temp);
    }
    free (MemberValues[attr_index]);
    return status;
}


/*
 * FUNCTION: parse_attribute
 *
 * OVERVIEW: This function parses and encodes an attribute. The result is       
 *           set into an cds attribute value structure for cds 
 *
 * INPUTS:
 *   cds_attribute_value *value;
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int status code TCL_OK for successfull operation/encoding.
 */
int parse_attribute(Tcl_Interp *interp,
                    cds_attribute_value *attr_value,
                    char *attribute)
{
    extern Tcl_Interp  *attr_interp;
    int                result = TCL_OK;
    char               **argv;
    int                argc = 0;
    int                attr_index = -1;
    int                temp_attr_type;
    cds_attrvalue_t    **value;
    int                size;
    int                i;
    char               *command;
    char               *command_format = "lindex [set [ attr_name %s ]] %d";
    char               *temp_buffer;

    /* 
     * Attribute should come in as a tcl list. If this is not the 
     * case return an error and force a usage message for now. 
     */
    Tcl_SplitList(attr_interp, attribute, &argc, &argv);

    if (argc < 2 && attr_value->action != DeleteAttribute) {
        free((char *)argv);
        DCP_SET_MISC_ERROR(dcp_s_cds_bad_attribute_list);
    }

    /*
     * Allocate the attribute structure(s) for encoding
     */
    if (argc == 1)
        size = sizeof(cds_attrvalue_t *);
    else    
        size = (argc - 1) * sizeof(cds_attrvalue_t *);

    value = (cds_attrvalue_t **)malloc(size);

    /*
     * Set up the command for evaluation by the attribute interpreter. 
     * Make sure there is enough room in the command buffer. This 
     * command returns an integer. 
     */
    command = malloc(strlen(argv[0]) + strlen(command_format) + 10);
    if (command == NULL) {
        free((char *)argv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }
    sprintf(command, command_format, argv[0], SyntaxIndex);
    result = Tcl_Eval(attr_interp, command);
    free(command);
    if (result == TCL_OK)
        attr_index = atoi((const char *)attr_interp->result);

    /*
     * Allocate and save the name of the attribute being encoded.
     * Find the attribute type either cdsSet or cdsSingle.
     */
    command = malloc(strlen(argv[0]) + strlen(command_format) +10);
    if (command == NULL) {
        free((char *)argv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }
    sprintf(command, command_format, argv[0],AttrType);
    if (Tcl_Eval(attr_interp, command) == TCL_OK)
        temp_attr_type = atoi(attr_interp->result);
    if (temp_attr_type != NoAttrType && attr_value->attr_type == cdsSet)
        attr_value->attr_type = temp_attr_type;
    free(command);

    /*
     * Find the complete name for the attribute.
     */
    command = malloc(strlen(argv[0]) + strlen(command_format) + 10);
    if (command == NULL) {
        free((char *)argv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }
    sprintf(command, command_format, argv[0],OidName);
    if (Tcl_Eval(attr_interp, command) == TCL_OK)
        strcpy(attr_value->attr_name, attr_interp->result);
    else
        strcpy(attr_value->attr_name,argv[0]);
    free(command);

    for (i = 1; i < argc ; i++, attr_value->cur_attr++) {
        value[attr_value->cur_attr] = (cds_attrvalue_t *)malloc(sizeof(cds_attrvalue_t)); 
        if (attr_value->action != DeleteAttribute) {
            switch (attr_index) {
                case CDS_Class:
                    if (attr_value->action == ChangeAttribute)
                        attr_value->action = AddAttribute;
                    result = strClass(argv[i], value[attr_value->cur_attr]);
                    break;
                case CDS_Convergence:
                    if (attr_value->action == ChangeAttribute)
                        attr_value->action = AddAttribute;
                    result = CVTstrConvergence(argv[i], value[attr_value->cur_attr]);
                    break;
                case cdsVersion:
                    if (attr_value->action == ChangeAttribute)
                        attr_value->action = AddAttribute;
                    result = strVersion(argv[i], value[attr_value->cur_attr]);
                    break;
                case cdsFullName:
                    if (attr_value->action == ChangeAttribute)
                        attr_value->action = AddAttribute;
                    result = strLinkTarget(argv[i], value[attr_value->cur_attr]);
                    break;
                case CDS_Towers:
                    result = strTower(interp, argv[i], value[attr_value->cur_attr]);
                    break;
                case cdschar:
                    result = strChar(argv[i], value[attr_value->cur_attr]);
                    break;
                case cdsuuid:
                    result = strUuid(argv[i], value[attr_value->cur_attr]);
                    break;
                case cdssmall:
                    result = strSmall(argv[i], value[attr_value->cur_attr]);
                    break;
                case cdsTimeout:
                    if (attr_value->action == ChangeAttribute)
                        attr_value->action = AddAttribute;
                    if (argc < 3) {
                        free((char *)argv);
                        DCP_SET_MISC_ERROR(dcp_s_cds_ill_attr_val);
                    }
                    result = strTimeout(argv, value[attr_value->cur_attr]);
                    break;
                case cdsbyte:
                default:
                    temp_buffer = format_byte_input(argv[i]);
                    result = strByte(temp_buffer, value[attr_value->cur_attr]);
                    free(temp_buffer);
                    break;
            }
            if (result != TCL_OK) {
                free((char *)argv);
                DCP_SET_MISC_ERROR(result);
            }
        }
    }
    if (attr_value->cur_attr == 0)
        attr_value->cur_attr++;
    attr_value->attribute = value;
    free((char *)argv);
    return TCL_OK;
}


/*
 * FUNCTION: CVTstrConvergence
 *
 * OVERVIEW: encode a string representing a cds convergence value into a 
 * cds attribute value structure.
 *
 * INPUTS:
 *  char *buffer - ascii string representing a cds convergence value.
 *
 * INPUT/OUTPUTS:
 *  cds_attrvalue_t **value - encoded cds attribute value structure. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int TCL_OK - operation successful. 
 *
 */
int CVTstrConvergence(char *buffer, cds_attrvalue_t *value)
{
    int                convergence = 2;
    extern Tcl_Interp  *attr_interp;
    char               *command_format = "lindex [set [ attr_name %s ]] 0";
    char               command[CDS_FNAME_MAX];

    memset(command, 0, sizeof(command));
    sprintf(command, command_format, buffer);

    if (Tcl_Eval(attr_interp, command) == TCL_ERROR)
        return dcp_s_cds_ill_attr_val;

    convergence = atoi((const char *)attr_interp->result);

    value->val_syntax = cds_small;
    value->val_u.val_small = (char)convergence;

    return TCL_OK;
}


/*
 * FUNCTION: strVersion
 *
 * OVERVIEW:
 *  Translate a string into a version attribute. 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 *
 */
int strVersion(char *buffer, cds_attrvalue_t *value)
{
    int  minver;
    int  majver;

    sscanf(buffer, "%1d.%1d", &majver, &minver);

    value->val_syntax = cds_Version;
    value->val_u.val_version.cdsMajVer = (unsigned char)majver;
    value->val_u.val_version.cdsMinVer = (unsigned char)minver;
    
    return TCL_OK;
}


/*
 * FUNCTION: struuid
 *
 * OVERVIEW:
 *  Translate a string uuid into a cds_objectuuid attribute. 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
int strUuid(char *buffer, cds_attrvalue_t *value)
{
    unsigned32  result;

    value->val_syntax = cds_uuid;
    uuid_from_string((unsigned char *)buffer,
                     &(value->val_u.val_uuid),
                     (unsigned32 *)&result);
    if (result == uuid_s_ok)
        return TCL_OK;

    return result;
}


/*
 * FUNCTION: strLinkTarget
 *
 * OVERVIEW:
 *  Translate a string into a CDS_LinkTarget attribute type.
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
int strLinkTarget(char *buffer, cds_attrvalue_t *value)
{
    unsigned char  *temp;
    unsigned32     status = 0;
    int            length = CDS_OPQ_FNAME_MAX;

    value->val_syntax = cds_FullName;

    rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce,
                             (unsigned char *)buffer,
                             &temp,&status);
    if (status != rpc_s_ok)
        return status;

    value->val_u.val_byte.byte_p = malloc(length);
    if (value->val_u.val_byte.byte_p == NULL)
        return dcp_s_no_memory;

    cdsCvtFullToOpq((unsigned char *)temp, 
                    value->val_u.val_byte.byte_p,
                    &length);

    value->val_u.val_byte.length = length;
    rpc_string_free(&temp, &status);

    return TCL_OK;
}


/*
 * FUNCTION: strByte
 *
 * OVERVIEW:
 *    Translate a string into a CDS_Class attribute type of cds_byte. 
 * 
 * INPUTS:
 *  char    *buffer - buffer containing the contents of byte style attribute.
 *
 * INPUT/OUTPUTS:
 *  cds_attrvalue_t **value - The attribute structure that will contain the
 *                            byte style attribute.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int TCL_OK for correct translation
 *
 */
int strByte(char *buffer, cds_attrvalue_t *value)
{
    int            len = 0;
    unsigned char  *class;

    value->val_syntax = cds_byte;

    class = (unsigned char *)malloc(strlen(buffer) + 20);
    if (class == NULL)
        return dcp_s_no_memory;

    value->val_u.val_byte.byte_p = class;

    for (;;) {
        char  *maybe_p = (char *)buffer;
        char  nibbles[8], *nibble_p = nibbles, *end_p;
        long  temp_value;

        /*
         * Move temp_value to high and low niblet niblets.
         */
        COPY_mbc_and_skip(maybe_p, nibble_p);
        COPY_mbc_and_skip(maybe_p, nibble_p);
        *nibble_p = '\0';
        temp_value = strtol(nibbles, &end_p, 16);
        if (strlen(end_p))
            return dcp_s_cds_illegal_byte_attr;
        if ((end_p == nibbles) && (temp_value == 0))
            break;
        if ((end_p != nibble_p) || (temp_value < 0) || (255 < temp_value))
            return dcp_s_cds_illegal_byte_attr;

        *class++ = temp_value;
        len++;
        buffer = maybe_p;
    }

    value->val_u.val_byte.length = len;
    return TCL_OK;
}


/*
 * FUNCTION: strClass
 *
 * OVERVIEW:
 *  Translate a string into a CDS_Class attribute type. 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
int strClass(char *buffer, cds_attrvalue_t *value)
{
    unsigned32   status = 0;
    int          length = 1024;
    ClassName_t  *class;

    value->val_syntax = cds_byte;

    class = (ClassName_t *)malloc(strlen(buffer) + 1);
    if (class == NULL)
        return dcp_s_no_memory;

    status = cdsCvtSimpleToOpq((unsigned char *)buffer,
                               0,
                               (cds_simple_name_t *)class,
                               &length);
    if (status != CDS_SUCCESS)
        return status;

    value->val_u.val_byte.byte_p = class;
    value->val_u.val_byte.length = length;
    
    return TCL_OK;
}


/*
 * FUNCTION: strSmall *
 *
 * OVERVIEW:
 *  Turn a character string into a cds_small attribute value.
 *
 * INPUTS:
 *  char *buffer - character string that represents a small attr value. 
 *
 * INPUT/OUTPUTS:
 *  cds_attrvalue_t **value - ultimate attr value structure. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int TCL_OK if conversion occured without error. 
 *
 */
int strSmall(char *buffer, cds_attrvalue_t *value)
{
    value->val_syntax = cds_small;
    value->val_u.val_small = atoi(buffer);
    
    return TCL_OK;
}


/*
 * FUNCTION: strChar
 *
 * OVERVIEW: This function will convert an ascii string into a cds 
 *   char attribute and update the attribute structure.
 *
 * INPUTS:
 *  cds_attrvalue_t *value a cds attribute structure 
 *  char *buffer - the ascii char string.
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 * TCL_OK
 *
 */
int strChar(char *buffer, cds_attrvalue_t *value)
{
    int  length = 1024;

    value->val_syntax = cds_char;
    length = strlen(buffer);
    value->val_u.val_char.char_p =  malloc(length + 1);

    if (value->val_u.val_char.char_p == NULL)
        return dcp_s_no_memory;

    strcpy(value->val_u.val_char.char_p,buffer);
    value->val_u.val_char.length = length;
    
    return TCL_OK;
}


/*
 * FUNCTION: strTimeout
 *
 * OVERVIEW:
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
int strTimeout(char **buffer, cds_attrvalue_t *value)
{
    char               **argv;
    int                argc;
    int                exp_or_ext;
    extern Tcl_Interp  *attr_interp;
    utc_t              timeout;
    int                i;
    char               command[1024];
    char               temp[1025];

    value->val_syntax = cds_Timeout;

    for (i = 1; i < 3; i++) {
        sprintf(temp, "{%s}", buffer[i]);
        Tcl_SplitList(attr_interp, buffer[i], &argc, &argv);

        if (argc < 2) {
            free((char *)argv);
            return dcp_s_cds_bad_attribute_list;
        }

        sprintf(command, "set [ attr_name {%s} ]", argv[0]);
        if (Tcl_Eval(attr_interp, command) != TCL_OK) {
            free((char *)argv);
            return dcp_s_cds_bad_attribute_name;
        }

        exp_or_ext = atoi(attr_interp->result);

        switch(exp_or_ext) {
            case 0:
                if (utc_mkascreltime(&timeout, argv[1]) == -1) {
                    free((char *)argv);
                    return dcp_s_dts_time_conversion_fail;
                }
                memcpy((char *)&(value->val_u.val_timeout.extend),
                       (char *)&timeout,
                       sizeof(utc_t));
                break;
            case 1:
                if (utc_mkasctime(&timeout, argv[1]) == -1) {
                    free((char *)argv);
                    return dcp_s_dts_time_conversion_fail;
                }
                memcpy((char *)&(value->val_u.val_timeout.expire),
                       (char *)&timeout,
                       sizeof(utc_t));
                break;
        }
    }
    free((char *)argv);
    return TCL_OK;
}


/*
 * FUNCTION: free_attr_value
 *
 * OVERVIEW:
 * Clean up the previously allocated attribute value structure. 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void free_attr_value(cds_attribute_list **list)
{
    int  i, j;

    for (i = 0; i < (*list)->cur_value; i++) {
        for (j = 0; j < (*list)->value[i]->cur_attr &&
                        (*list)->value[i]->action != DeleteAttribute &&
                        (*list)->value[i]->action != ShowAttrType; 
                   j++) {
            switch ((*list)->value[i]->attribute[j]->val_syntax) {
                case cds_byte:
                case cds_ReplicaPointer:
                case cds_GroupMember:
                case cds_FullName:
                case cds_ParentPointer:
                case cds_CHDirectory:
                case cds_ASN1:
                case cds_gdaPointer:
                    free((*list)->value[i]->attribute[j]->val_u.val_byte.byte_p);
                    break;
                case cds_char:
                    free((*list)->value[i]->attribute[j]->val_u.val_char.char_p);
                    break;
                default:
                    break;
            }
            free((*list)->value[i]->attribute[j]);
        }

        if ((*list)->value[i]->entered_name)
            free((*list)->value[i]->entered_name);
        free((*list)->value[i]);
    }
    free(*list);
}


/*
 * FUNCTION: strTower
 *
 * OVERVIEW:
 * Convert a string tower to a cds_byte attribute value.
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
int strTower(Tcl_Interp *interp, char *buffer,cds_attrvalue_t *value)
{
    char                    *binding_string[2];
    rpc_binding_handle_t    binding;
    unsigned32              rpc_status; 
    rpc_tower_vector_t      *tower_vector;
    char                    bindingstring[100];
    extern rpc_if_handle_t  cds_ifspec;
    unsigned char           *temp;
    int                     argc;
    char                    **argv;

    Tcl_SplitList(interp, buffer, &argc, &argv);
    if (argc > 1) {
        temp = (unsigned char *)dcp_binding_tcl_to_string(argc, 
                                                          argv, 
                                                          &rpc_status);
        strcpy(bindingstring, (char *)temp);
        rpc_string_free(&temp, &rpc_status);
    }
    else
        strcpy(bindingstring,buffer);    

    free((char *)argv);
    cds_ifspec = cds_clerkserver_v1_0_c_ifspec;
    value->val_syntax = cds_byte;

    binding_string[0] = bindingstring;
    binding_string[1] = NULL;
    
    rpc_binding_from_string_binding((unsigned char *)binding_string[0],
                                    &binding,
                                    &rpc_status);
    if (rpc_status != rpc_s_ok)
        return rpc_status;

    rpc_tower_vector_from_binding(cds_ifspec,
                                  binding,
                                  &tower_vector,
                                  &rpc_status);
    if (rpc_status != rpc_s_ok) 
        return rpc_status;

    value->val_u.val_byte.byte_p = malloc(tower_vector->tower[0]->tower_length);
    value->val_u.val_byte.length = tower_vector->tower[0]->tower_length;
    COPY_bytes(tower_vector->tower[0]->tower_octet_string,
               value->val_u.val_byte.byte_p,
               tower_vector->tower[0]->tower_length);

    rpc_tower_vector_free(&tower_vector, &rpc_status);
    return TCL_OK;
}


/*
 * FUNCTION: create_repset()
 *
 * OVERVIEW: This function will create a replica set of type 
 *           Set_t. This requires passing in a cds directory name. 
 *
 * INPUTS:
 *  cds_full_name_t directory - Directory name to build the set from.
 *
 * INPUT/OUTPUTS:
 *  Noe;
 *
 * OUTPUTS:
 *  Set_t **set - The resultant replica set. 
 *
 * RETURNS:
 *  int cds_success or an error code indicating which error occurred
 *
 */
int create_repset(Tcl_Interp *interp, cds_full_name_t directory, Set_t **set)
{
    int                 status = TCL_OK; 
    ReplicaPointer_t    *replica;
    MemberValue_t       *member;
    cds_attribute_list  *list = NULL;

    status = enum_one_attr(interp,
                           &directory,
                           NULL, 
                           "CDS_Replicas",
                           dnsDir,
                           &list);
    if (status != TCL_OK) {
        return status;
    }

    replica = list->value[0]->attribute[0]->val_u.val_byte.byte_p;

    /*
     * Build a set with one member.
     */
    INS8((*set)->st_flag, ST_present);
    INS16((*set)->st_offsetLength, sizeof(word_u));
    INS16((*set)->st_valueLength, sizeof(SetMember_overhead) + LEN_ReplicaPointer(replica));
    INS16(OFFSET_Set((*set), 0), sizeof(Set_overhead) + sizeof(word_u));
    member = (MemberValue_t *)INDEX_Set((*set), 0);
    INS8(member->mv_flag, MV_present);
    dnsCreateCTS((dns_cts *)member->mv_timestamp);
    INS16(member->mv_length, LEN_ReplicaPointer(replica));
    INS8(member->mv_valuetype, dns_Attribute(DNS_REPLICAS)->vtype);
    COPY_ReplicaPointer(replica, member->mv_value);

    free_attr_value(&list);
    return status;
}


/*
 * FUNCTION: init_cds_attribute_table
 *
 * OVERVIEW: This function will read the cds_attributes file from
 *           /opt/dcelocal/etc to associate known attribute names 
 *           with OID values. 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void init_cds_attribute_table(Tcl_Interp *interp)
{
    FILE           *fp; 
    char           line[1024];
    int            oid;
    char           *oid_ptr;
    char           *oid_tok;
    char           *attr_tok;
    char           *syntax_tok;
    extern         Tcl_Interp  *attr_interp;
    char           command[1024];
    char           *cds_attr_file_path;
    int            syntax_index;
    cdsAttrType_t  attr_type = cdsSet;
    int            i;

    syntax_index =  sizeof attr_syntax / sizeof attr_syntax[0];
    for (i = 0; i < syntax_index; i++) {
        sprintf(command, "set %s %d", attr_syntax[i].attribute_name, attr_syntax[i].syntax);
        Tcl_Eval(attr_interp, command);
    }

    cds_attr_file_path = malloc(strlen(dcelocal_path) + 21);
    sprintf(cds_attr_file_path, "%s/etc/cds_attributes", dcelocal_path);
    
    if ((fp = fopen(cds_attr_file_path, "r")) == NULL) {
        free(cds_attr_file_path);
        return;
    }
    free(cds_attr_file_path);

    while (fgets(line, 1024, fp) != NULL) {
        if (line[0] != '#' && strlen(line) > 1) {
            line[strlen(line) - 1] = '\0';
            oid_tok = strtok(line,"\t ");
            oid_ptr = dce_strdup(oid_tok);
            cvtstrtooid(oid_tok, &oid);
            attr_tok = strtok(NULL,"\t ");
            syntax_tok = strtok(NULL,"\t");

            /*
             * Find out if the syntax index is a predefined function 
             * or if it is a generic syntax such as byte, char, small etc 
             * also set attribute type either cdsSet or cdsSingle.
             */
            sprintf(command, "set %s", syntax_tok);
            if (Tcl_Eval(attr_interp, command) == TCL_ERROR)
                return;
            syntax_index = atoi(attr_interp->result);
            switch (oid) {
                case RPC_ClassVersion:
                    syntax_index = RPC_ClassVersion;
                    break;
                case CDS_ClassVersion:
                case CDS_ObjectUUID:
                case CDS_LinkTarget:
                case CDS_LinkTimeout:
                case CDS_UpgradeTo:
                case CDS_DirectoryVersion:
                case CDS_ReplicaVersion:
                    attr_type = cdsSingle;
                    break;
                case CDS_Class:
                    syntax_index = CDS_Class;
                    attr_type = cdsSingle;
                    break;
                case CDS_Convergence:
                    syntax_index = CDS_Convergence;
                    attr_type = cdsSingle;
                    break;
                case CDS_Towers:
                    syntax_index = CDS_Towers;
                    attr_type = cdsSet;
                    break;
                case CDS_ReplicaState:
                case CDS_InCHName:
                    syntax_index = CDS_ReplicaState;
                    attr_type = cdsSet;
                    break;
                case CDS_ReplicaType:
                    syntax_index = CDS_ReplicaType;
                    break;
                case CDS_CHState:
                    syntax_index = CDS_CHState;
                    attr_type = cdsSingle;
                    break;
                case CDS_CHLastAddress:
                    syntax_index = CDS_CHLastAddress;
                    attr_type = cdsSingle;
                    break;
                case CDS_CellAliases:
                    syntax_index = CDS_CellAliases;
                    attr_type = cdsSet;
                    break;
                default:
                    attr_type = NoAttrType;
                    break;
            }

            /*
             * Set up a variable in the attribute interpreter that 
             * describes the oid and string name and syntax. Several of the 
             * attributes have specific functions for encoding and decoding
             * so we must set them aside for special processing. In order to
             * tag them I have arbitarily decided that the syntax for each 
             * of these attributes will be the compressed oid. That is just 
             * the oid number with no '.' points.      
             * The first command sets up the attribute name as a set with the 
             * values:
             * oid, attr_name, oid_name, syntax_name, syntax_int, attr_type
             * oid - int value made from oid_name
             * syntax_int - int value to determine the syntax 
             * attr_type - cdsSet or cdsSingle or -1 for no known syntax.
             */
            sprintf(command, "set %s {%d %s %s %s %d %d}", attr_tok, oid,
                    attr_tok, oid_ptr, syntax_tok, syntax_index, attr_type);
            Tcl_Eval(attr_interp, command);
            sprintf(command, "set %s {%d %s %s %s %d %d}", oid_ptr, oid,
                    attr_tok, oid_ptr, syntax_tok, syntax_index, attr_type);
            Tcl_Eval(attr_interp, command);
            free(oid_ptr);
        }
    }
    fclose(fp);
}


/*
 * FUNCTION: cvtstrtooid
 *
 * OVERVIEW:
 * 
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void cvtstrtooid(char *str_number, int *oid)
{
    char  temp[1024];
    int   i = 0;

    memset(temp, 0, 1024);
    for (; *str_number != '\0'; str_number++) {
        if (*str_number != '.') {
            temp[i] = *str_number;
            i++;
        }
    }
    *oid = atoi(temp);
}


/*
 * FUNCTION: cds_parse_attributes
 *
 * OVERVIEW: This function takes a tcl list of attributes and parses them
 *           into an array of cds_attrvalue_t structures. This function 
 *           allocates and fills in the array of structures, which includes
 *           the attribute names. and a count of the values.
 *
 * INPUTS:
 *  char *attribute_list - a tcl list of all attributes to be modified. 
 *  Attribute_Action_t action - an enumerated value describing action.
 * 
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  int attrc - the number of entries within the cds_attribute_value structure.
 *  cds_attribute_value **value - the cds attribute structure and name.
 *
 * RETURNS:
 * int - either a success code.
 *
 */
int cds_parse_attributes(Tcl_Interp *interp,
                         char *attribute_list, 
                         Attribute_Action_t action,
                         cdsEntries_t type, 
                         cdsAttrType_t attr_type,
                         cds_attribute_list *list)
{
    int                  result = TCL_OK;
    char                 **argv;
    int                  argc;
    int                  i;
    int                  size = 0;
    cds_attribute_value  **temp_value;

    if (result = get_list_elements(attribute_list, &argc, &argv) != TCL_OK)
        return result;

    size = (list->cur_value + argc) * sizeof(cds_attribute_value *);
    temp_value = (cds_attribute_value **)realloc(list->value,size);
    if (!temp_value) {
        free((char *)argv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    for (i = 0; i < argc; i++, list->cur_value++) {
        temp_value[list->cur_value] = (cds_attribute_value *)
            malloc(sizeof(cds_attribute_value));
        memset((char *)temp_value[list->cur_value], 0, sizeof(cds_attribute_value));
        temp_value[list->cur_value]->cur_attr = 0;
        temp_value[list->cur_value]->attr_type = attr_type;
        temp_value[list->cur_value]->obj_type = type;
        temp_value[list->cur_value]->action = action;
        result = parse_attribute(interp, temp_value[list->cur_value], argv[i]);
        if (result != TCL_OK) {
            free((char *)argv);
            return TCL_ERROR;
        }
    }
    free((char *)argv);
    list->value = temp_value;
    return TCL_OK;
}


/*
 * FUNCTION: get_list_elements
 *
 * OVERVIEW: Return an argv, argc breakdown of a list. 
 *
 * INPUTS:
 *  char *list -  the list to be split. 
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int the success code. 
 */
int get_list_elements(char *list, int *argc, char ***argv)
{
    extern Tcl_Interp  *attr_interp;
    int                islist = FALSE;
    char               **aargv;

    islist = isllist(list);
    switch (islist) {
        case TRUE:
            Tcl_SplitList(attr_interp, list, argc, argv);
            break;
        case FALSE:
            aargv = (char **)malloc(2 * sizeof(char *));
            if (argv == NULL) {
                return dcp_s_no_memory;
            }
            aargv[0] = list;
            aargv[1] = NULL;
            *argc = 1;
            *argv = aargv;
            break;
        default:
            return islist;
    }
    return TCL_OK;
}


/*
 * FUNCTION: parse_timeout
 *
 * OVERVIEW: This function takes an ascii buffer and generates 
 *            a cdstimeout attribute value.
 *
 * INPUTS:
 *  (char *timeout - ascii string describing a timeout. 
 *
 * INPUT/OUTPUTS:
 *  cds_attribute_value ***value- structure with encoded attribute value.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int error_status_ok - on successful completion.
 */
int parse_timeout(char *timeout,cds_attribute_list *list)
{
    extern               Tcl_Interp  *attr_interp;
    char                 **argv;
    int                  argc;
    int                  size;
    cds_attribute_value  **temp_value;

    Tcl_SplitList(attr_interp, timeout, &argc, &argv);

    if (argc < 1 || argc > 2) {
        free((char *)argv);
        return dcp_s_cds_bad_timeout_value;
    }
    size = (list->cur_value + 1) * sizeof(cds_attribute_value);
    temp_value = (cds_attribute_value **) realloc(list->value,size);

    if (!temp_value)
        return dcp_s_no_memory;

    temp_value[list->cur_value] = (cds_attribute_value *)malloc(sizeof(cds_attribute_value));
    if (!temp_value[list->cur_value]) {
        free((char *)argv);
        return dcp_s_no_memory;
    }

    temp_value[list->cur_value]->attribute =  (cds_attrvalue_t **)malloc(sizeof(cds_attrvalue_t));
    temp_value[list->cur_value]->attribute[0] = malloc(sizeof(cds_attrvalue_t));

    (temp_value[list->cur_value]->attribute[0])->val_syntax = cds_Timeout;

    if (utc_mkasctime(&((temp_value[list->cur_value]->attribute[0])->val_u.val_timeout.expire),
                      argv[0]) == -1) {
        free((char *)argv);
        return dcp_s_dts_time_conversion_fail;
    }

    if (argc == 2) {
        if (utc_mkascreltime(&((temp_value[list->cur_value]->attribute[0])->val_u.val_timeout.extend),
                             argv[1]) == -1) {
            free((char *)argv);
            return dcp_s_dts_time_conversion_fail;
        }
    }
    else 
        utc_mkascreltime(&((temp_value[list->cur_value]->attribute[0])->val_u.val_timeout.extend), 0);
    strcpy(temp_value[list->cur_value]->attr_name, "CDS_LinkTimeout");
    list->value = temp_value;
    list->cur_value++;
    return error_status_ok;
}


/*
 * FUNCTION: remove_cds_attr
 *
 * OVERVIEW: This function removes a cds attribute from an object/link/
 *           directory
 *
 * INPUTS:
 *   cds_full_name_t     obj_name;
 *   cds_attr_name_t     attr_name;
 *   cds_attribute_value value
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int CDS_SUCCESS
 */
int remove_cds_attr(cds_full_name_t *fullname, 
                    cds_attr_name_t *attrname,
                    cds_attribute_value *value)
{
    int                 status; 
    struct cdsFlagStat  flags;

    clear_flags(&flags, cdsStrDCE);
    if (value->action != RemoveAttribute) {
        status = cdsRmAttr(fullname, attrname, cdsSingle, value->obj_type, NULL, &flags);
        if (status != CDS_SUCCESS) {
            if (flags.fsLocStat == CDS_WRONGATTRIBUTETYPE)
                status = cdsRmAttr(fullname,
                                   attrname,
                                   cdsSet,
                                   value->obj_type,
                                   NULL,
                                   &flags);
            if (status != CDS_SUCCESS)
                return flags.fsLocStat;
            value->attr_type = cdsSet;
        }
        else
            value->attr_type = cdsSingle;
    }
    return status;
}


/*
 * FUNCTION: modify_cds_attribute
 *
 * OVERVIEW: This function performs modifications on  attributes within 
 *       an object in cds. 
 *
 * INPUTS: Tcl_Interp          *interp;
 *         cds_full_name_t     obj_name;
 *         cds_attr_name_t     attr_name;
 *         cds_attribute_list  *list;
 *  
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success or error code on failure. 
 */
int modify_cds_attribute(Tcl_Interp *interp,
                         cds_full_name_t *name, 
                         cds_attr_name_t *attr_name,
                         cds_attribute_list *list)
{
    int                 i, j;
    int                 status;
    struct cdsFlagStat  flags;

    for (i = 0 ; i < list->cur_value; i++) {
        strcpy((char *)attr_name->uc,list->value[i]->attr_name);
        if (list->value[i]->action == ChangeAttribute) {
            status = remove_cds_attr(name,
                                     attr_name,
                                     list->value[i]);
            if (status != CDS_SUCCESS){
                DCP_SET_MISC_ERROR(status);
            }
        }
        for (j = 0 ; j < list->value[i]->cur_attr; j++) {
            clear_flags(&flags, cdsStrDCE);
            status = CDS_SUCCESS;
            if (list->value[i]->action == DeleteAttribute) {
                status = remove_cds_attr(name,
                                         attr_name,
                                         list->value[i]);
                if (status != CDS_SUCCESS) {
                    DCP_SET_MISC_ERROR(status);
                }
            }
            else if(list->value[i]->action == ChangeAttribute ||
                    list->value[i]->action == AddAttribute){
                status = cdsAddAttrValue(name,
                                         attr_name, 
                                         list->value[i]->attr_type,
                                         list->value[i]->obj_type, 
                                         list->value[i]->attribute[j],
                                         NULL,
                                         &flags);
            }
            else if (list->value[i]->action == RemoveAttribute) {
                if (list->value[i]->attr_type != cdsSet) {
                    DCP_SET_CDS_ERROR_ARG(dcp_s_cds_bad_attribute_type, attr_name->uc);
                }
                status = cdsRmAttrValue(name,
                                        attr_name,
                                        list->value[i]->obj_type, 
                                        list->value[i]->attribute[j],
                                        NULL,
                                        &flags);
            }
            if (status == CDS_ERROR) {
                DCP_SET_MISC_ERROR(flags.fsLocStat);
            }
        }
    }
    return TCL_OK;
}


/*
 * FUNCTION: determine_modify_order
 *
 * OVERVIEW: This function determines the order in which a usr
 *     requested modifcations. That is if a user request that 
 *     -change attr -add attr -remove attr happened in this order
 *     then this function will provide that information. 
 *
 * INPUTS:
 *  char **argv - option list. 
 *  int  argc   - number of options. 
 *  int  order[]  array describeing the order. 
 * 
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void determine_modify_order(int order[], int argc, char **argv)
{
    int   i, j = 0;
    char  *add = "-a";
    char  *remove = "-r";
    char  *change = "-c";

    for (i = 0; i < argc; i++) {
        if (strspn(argv[i], add) == MinimumCmp) {
            order[j] = AddAttribute;
            j++;
        }
        else if (strspn(argv[i],change) == MinimumCmp) {
            order[j] = ChangeAttribute;;
            j++;
        }
        else if (strspn(argv[i],remove) == MinimumCmp) {
            order[j] = RemoveAttribute;;
            j++;
        }
        if (j == ModifyTypes)
            return;
    }
    if (j < ModifyTypes)
        for (; j < ModifyTypes; j++)
            order[j] = -1;
    return;
}


/*
 * FUNCTION: generate_attribute_list
 *
 * OVERVIEW:This function will generate an attribute list for the modify
 * commands. This will generate the list in the order that the list was
 * entered. 
 *
 * INPUTS:
 *  Tcl_Interp *interp - interpreter for recording errors.
 *  char *add - A list of attributes to add to the object.
 *  char *remove - A list of attributes to remove from the object.
 *  char *change - A list of attributes to change in the object.
 *  cdsEntries_t  type - The object type cdsObj, cdsDir, etc. 
 *  int single - a suggestion on whether the attribute should be single/multi
 *  int types - Used on remove to indicate an attribute type to be removed.
 *  int order[] - The order in which they were entered.
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  cds_attribute_list  *list - this is resultant attribute list.
 *
 * RETURNS:
 *  void
 */
int generate_attribute_list(Tcl_Interp *interp,
                            char *add, 
                            char *remove, 
                            char *change,
                            cdsEntries_t type,
                            cds_attribute_list *list,
                            int single,
                            int types,
                            int order[])
{
    cdsAttrType_t       attr_type = cdsSet;
    int                 i = 0;
    char                *attr_list;
    Attribute_Action_t  action = AddAttribute;

    for (i = 0; i < ModifyTypes; i++) {
        switch (order[i]) {
            case AddAttribute:
                if (add) {
                    if (single)
                        attr_type = cdsSingle;
                    attr_list = add;
                }
                break;
            case RemoveAttribute:
                if (remove) {
                    if (types)
                        action = DeleteAttribute;
                    else
                        action = RemoveAttribute;
                    attr_list = remove;
                }
                break;
            case ChangeAttribute:
                if (change) {
                    action = ChangeAttribute;
                    attr_list = change;
                }
                break;
            default:
                return TCL_OK;
        }

        if (cds_parse_attributes(interp, 
                                 attr_list, 
                                 action, 
                                 type, 
                                 attr_type, 
                                 list) != TCL_OK)
            return TCL_ERROR; 
    }
    return TCL_OK;
}


/*
 * FUNCTION: enum_all_attr
 *
 * OVERVIEW: This function takes a cds object, and type and generates 
 *         an attribute list of all attribute entries within the object. 
 *
 * INPUTS: Tcl_Interp         *interp, - interpreter for error processing
 *         cds_full_name_t    *name, - name of object
 *         cds_full_name_t    *clr_name, - clearinghouse if directory object.
 *         cdsEntries_t       type, - object type dir/link/object
 *         Attribute_Action_t action, - action type. 
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *    cds_attribute_list **return_list a pointer to a list 
 *                         containing all attributes.
 *
 * RETURNS:
 *  TCL_OK on successful completion. 
 */
int enum_all_attr(Tcl_Interp *interp,
                  cds_full_name_t *name,
                  cds_full_name_t *clr_name,
                  cdsEntries_t type,
                  Attribute_Action_t action,
                  cds_attribute_list **return_list)
{
    cds_handle_t        context_handle;
    cds_handle_t        attr_handle;
    unsigned char       Attr_Name[CDS_STR_ANAME_MAX + 1];
    cds_attr_name_t     cds_attr_name;
    int                 AttrNameBufLen;
    int                 cds_status = CDS_SUCCESS;
    int                 size = 0;
    char                *temp_clearinghouse;
    unsigned32          j = 0, i = 0;
    cdsAttrType_t       attr_type;
    cds_attrvalue_t     *value;
    cds_attribute_list  *list;
    struct cdsFlagStat  flags; 
    struct cdsFlagStat  attr_flags; 
    boolean32           using_clr_name = FALSE;

    if (!clr_name && (type == cdsDir || type == cdsCH)) {
        temp_clearinghouse = DCP_GENERAL_GET_S_VAR(CLR_NAME);
        if (temp_clearinghouse && strlen(temp_clearinghouse)) {
            clr_name = (cds_full_name_t *)malloc(sizeof(cds_full_name_t));
            strcpy((char *)clr_name->uc, temp_clearinghouse);
            using_clr_name = TRUE;
        }
    }

    /*
     * Aquire the cdshandle needed for the enumerate attribute function.
     * Call the enumerate attribute function until the status returned 
     * indicates that all attributes have been aquired (CDS_EMPTY).
     */
    context_handle = cdsGetHandle();
    if (context_handle == NULL_CDS_HANDLE) {
        if (using_clr_name)
            free((char *)clr_name);
        DCP_SET_RESULT_CODE(dcp_s_cds_no_handle);
        return TCL_ERROR;
    }
    clear_flags(&flags, cdsStrDCE); 
    list = (cds_attribute_list *)malloc(sizeof(cds_attribute_list));
    memset((char *)list, 0, sizeof(cds_attribute_list));
    while (cds_status == CDS_SUCCESS) {
        AttrNameBufLen = CDS_STR_ANAME_MAX;
        cds_status = cdsEnumAttr(name,
                                 context_handle,
                                 type,
                                 &attr_type,
                                 NULL,
                                 Attr_Name,
                                 &AttrNameBufLen,
                                 &flags);
        if (cds_status == CDS_ERROR) {
            if (using_clr_name)
                free((char *)clr_name);
            cdsFreeHandle(context_handle);
            free_attr_value(&list);
            DCP_SET_MISC_ERROR(flags.fsLocStat);
        }
        else if (cds_status == CDS_EMPTY) {
            list->cur_value = j;
            *return_list = list;
            if (using_clr_name)
                free((char *)clr_name);
            cdsFreeHandle(context_handle);
            return TCL_OK;
        }
        size = (j + 1) * sizeof(cds_attribute_value *);
        list->value = (cds_attribute_value **)realloc(list->value, size);
        list->value[j] = (cds_attribute_value *)malloc(sizeof(cds_attribute_value));
        memset((char *)list->value[j], 0, sizeof(cds_attribute_value));
        list->value[j]->action = action;
        list->value[j]->attr_type = attr_type;
        list->value[j]->obj_type = type;
        list->value[j]->cur_attr = 0;
        Attr_Name[AttrNameBufLen + 1] = '\0';
        strcpy(list->value[j]->attr_name, (char *)Attr_Name);
        attr_handle = cdsGetHandle();
        if (attr_handle == NULL_CDS_HANDLE) {
            if (using_clr_name == TRUE) {
                free((char *)clr_name);
            }
            cdsFreeHandle(context_handle);
            free_attr_value(&list);
            DCP_SET_MISC_ERROR(dcp_s_cds_no_handle);
        }
        clear_flags(&attr_flags, cdsStrDCE); 
        strcpy((char *)cds_attr_name.uc, (char *)Attr_Name);
        i = 0;

        /* 
         * If the status is CDS_SUCCESS save the value in the 
         * list, otherwise log the failure. A status of CDS_EMPTY 
         * indicates the attribute was a set type attribute and we 
         * have exhausted all entries. Everything else is a failure. 
         */
        while (cds_status == CDS_SUCCESS && action != ShowAttrType) {
            cds_status = cdsReadAttrValue(name,
                                          attr_handle,
                                          &cds_attr_name,
                                          type,
                                          clr_name,
                                          &value,
                                          NULL,
                                          NULL,
                                          NULL,
                                          &attr_flags);
            if (cds_status == CDS_ERROR) {
                if (using_clr_name == TRUE)
                    free((char *)clr_name);
                free_attr_value(&list);
                cdsFreeHandle(context_handle);
                cdsFreeHandle(attr_handle);
                DCP_SET_MISC_ERROR(attr_flags.fsLocStat);
            }
            if (cds_status == CDS_EMPTY) {
                list->value[j]->cur_attr = i;
                cds_status = CDS_SUCCESS;
                break;
            }
            size = (i + 1) * sizeof(cds_attrvalue_t *);
            list->value[j]->attribute = (cds_attrvalue_t **)
                realloc(list->value[j]->attribute, size);
            list->value[j]->attribute[i] = copy_attr_value(value);
            i++;
        }
        cdsFreeHandle(attr_handle);
        j++;
    }
    cdsFreeHandle(context_handle);
    *return_list = list;
    if (using_clr_name == TRUE)
        free((char *)clr_name);
    return TCL_OK;
}


/*
 * FUNCTION: format_all_attr
 *
 * OVERVIEW: This function takes a list of attributes and formats them for 
 *        printing. 
 *
 * INPUTS:
 *  Tcl_Interp interp - interpreter for error processing. 
 *  cds_attribute_list *list - the list of all attributes. 
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *   TCL_OK if all goes well. 
 */
int format_all_attr(Tcl_Interp *interp, cds_attribute_list *list)
{
    extern Tcl_Interp  *attr_interp; 
    int                i, j;
    char               *attrvalue;
    char               *attrname;
    int                attrsyntax;
    unsigned32         size;
    unsigned32         no_attributes;
    boolean32          multi_line = FALSE;
    boolean32          no_cr = FALSE;
    cds_attrvalue_t    *value;
    char               *buffer;
    int                single;
    int                result;
    char               *temp;
    char               command[1024];

    /* 
     * Reset the temp_attr_value since it may have been used previously
     * in a call to this function. If this call returns a TCL_ERROR that 
     * indicates a first call. 
     */
    Tcl_UnsetVar(attr_interp, ATTRIBUTE_LIST, 0);
    if ((list && list->cur_value == 0) || !list) {
        DCP_SET_MISC_ERROR(dcp_s_cds_no_attr_list_value);
    }

    for (i = 0; i < list->cur_value; i++) {
        attrname = list->value[i]->attr_name;

        if (list->value[i]->cur_attr == 0 && list->value[i]->action != ShowAttrType) {
            attrvalue = malloc(strlen(attrname) + 6);
            sprintf(attrvalue, "{%s {}}", attrname);
            APPEND_ATTRLIST_VALUE(attrvalue);
            free(attrvalue);
        }

        /*
         * Obtain the attribute set information and the attrbute syntax 
         * from a list of attribute variables. Switch on attribute syntax
         * to perform the actual translation. All unknown attributes get 
         * switched to default.
         */
        sprintf(command, "lindex [set [ attr_name %s ]] %d", attrname, SyntaxIndex);
        result = Tcl_Eval(attr_interp, command);
        if (result != TCL_OK)
            attrsyntax = -1;
        else
            attrsyntax = atoi((const char *)attr_interp->result);

        /*
         * Ascertain how the formating should be performed. For instance,
         * if the attribute type is cdsSingle we can decode it normally. 
         * However, if the attribute type is cdsSet we need to find 
         * out if there is more than one value or if each value has 
         * multiple lines. 
         */
        multi_line = is_attribute_multi_line(attrsyntax);
        no_attributes = list->value[i]->cur_attr;
        if (!no_attributes && list->value[i]->action == ShowAttrType)
            no_attributes = 1;
        single = list->value[i]->attr_type;
        if (single == cdsSet && no_attributes > 1) {
            attrvalue = malloc(strlen(attrname) + 4);
            sprintf(attrvalue, "{%s ", attrname);
            APPEND_ATTRLIST_VALUE(attrvalue);
            free(attrvalue);
        }

        for (j = 0; j < no_attributes; j++) {
            result = dcp_s_ok;
            if (list->value[i]->action == ShowAttrType) {
                attrsyntax = cdsAttrType;
                multi_line = FALSE;
                single = cdsSingle;
            }
            else {
                value = list->value[i]->attribute[j];
                /*
                 * If we aren't able to drag up a valid attribute name
                 * try to figure out the type by the val_syntax.
                 */
                if (attrsyntax == -1)
                    attrsyntax = map_val_syntax(value->val_syntax);
            }
            buffer = NULL;

            switch (attrsyntax) {
                case CDS_Class:
                    size = CDS_STR_SNAME_MAX + (value->val_u.val_byte.length * 2);
                    buffer = malloc(size);
                    memset(buffer, 0, size-1);
                    cdsCvtSimpleToStr((cds_simple_name_t *)value->val_u.val_byte.byte_p,
                                      (unsigned char *)buffer,
                                      (int *)&size);
                    break;
                case RPC_ClassVersion:
                    multi_line = FALSE;
                case cdsbyte:
                    F_byte(value, &buffer);
                    buffer = format_byte_output(buffer, 1, multi_line);
                    break;
                case CDS_Towers:
                    result = decode_tower(value->val_u.val_byte.byte_p,
                                          value->val_u.val_byte.length,
                                          &buffer);
                    if (result != CDS_SUCCESS)
                        F_byte(value, &buffer);
                    else
                        strip_braces(buffer);
                    result = dcp_s_ok;
                    break;
                case CDS_CHLastAddress:
                    chlastaddress(value, &buffer);
                    break;
                case cdschar:
                    F_char(value, &buffer);
                    break;
                case cdsCHDirectory:
                    result = F_chdirectory(value, &buffer);
                    break;
                case cdsFullName:
                    F_fullname(value, &buffer);
                    break;
                case cdsgdaPointer:
                    result = F_gp(value, &buffer);
                    break;
                case cdsGroupMember:
                    result = F_member(value, &buffer,1);
                    break;
                case cdsParentPointer:
                    result = F_pp(value, &buffer); 
                    break;
                case cdsReplicaPointer:
                    result = F_rp(value, &buffer);
                    break;
                case CDS_Convergence:
                    result = convergence(value, &buffer);
                    break;
                case CDS_ReplicaState:
                    result = replicastate(value, &buffer);
                    break;
                case CDS_CHState:
                    result = chstate(value, &buffer);
                    break;
                case CDS_ReplicaType:
                    replicatype(value, &buffer);
                    break;
                case CDS_CellAliases:
                    /*
                     * CDS_CellAliases has the same format as a group member,
                     * but we want different text labels.
                     */
                    result = F_member(value, &buffer, 2);
                    if (no_attributes == 1) {
                        multi_line = TRUE;
                        no_cr = TRUE;
                    }
        	    break;
                case cdsTimeout:
                    result = F_timeout(value, &buffer);
                    break;
                case cdsTimestamp:
                    F_timestamp(value, &buffer);
                    break;
                case cdsuuid:
                    F_uuid(value, &buffer);
                    break;
                case cdsVersion:
                    F_version(value, &buffer);
                    break;
                case cdssmall:
                    F_small(value, &buffer);
                    break;
                case cdsAttrType:
                    if (list->value[i]->attr_type == cdsSingle)
                        buffer = (char *)dce_sprintf(dcp_t_cds_single_attribute);
                    else if (list->value[i]->attr_type == cdsSet)
                        buffer = (char *)dce_sprintf(dcp_t_cds_multi_attribute);
                    break;
                default:
                    buffer = (char *)dce_sprintf(dcp_s_cds_unknown_data_type);
                    break;
            }

            if (result != dcp_s_ok) {
                if (buffer != NULL)
                    free(buffer);
                DCP_SET_MISC_ERROR(result);
            }

            attrvalue = NULL;
            if (no_attributes == 1 && !multi_line && buffer) {
                size = strlen(attrname) + strlen(buffer) + 40;
                attrvalue = malloc(size);
                sprintf(attrvalue, "{%s %s}", attrname, buffer);
            }
            else if (no_attributes == 1 && multi_line && buffer && (single == cdsSet)) {
                size = strlen(attrname) + strlen(buffer) + 40;
                attrvalue = malloc(size);
                if (no_cr) {
                    sprintf(attrvalue, "{%s {%s}}", attrname, buffer);
                    no_cr = FALSE;
                }
                else
                    sprintf(attrvalue, "{%s\n {%s}}", attrname, buffer);
            }
            else if ((single == cdsSingle) && multi_line && buffer) {
                size = strlen(attrname) + strlen(buffer) + 40;
                attrvalue = malloc(size);
                sprintf(attrvalue, "{%s\n %s}", attrname, buffer);
            }
            else if ((no_attributes > 1) && buffer) {
                size = strlen(buffer) + 8;
                attrvalue = malloc(size);
                if (buffer) {
                    if (j == (no_attributes - 1))
                        sprintf(attrvalue, " {%s}}", buffer);
                    else 
                        sprintf(attrvalue, " {%s}", buffer);
                }
            }
            if (attrvalue && buffer) {
                APPEND_ATTRLIST_VALUE(attrvalue);
                free(attrvalue);
            }
            if (buffer != NULL)
                free(buffer);
        }
    }

    if (strlen(interp->result)) 
        Tcl_AppendResult(interp, "\n\n", NULL); 
    temp = NULL;
    temp = Tcl_GetVar(attr_interp, ATTRIBUTE_LIST, 0);
    if (temp)
        Tcl_AppendResult(interp, temp, NULL);

  return TCL_OK;
}


/*
 * FUNCTION: map_val_syntax
 *
 * OVERVIEW: This function is used to map cds attr syntax values to
 *	     the complex syntax values used by the dcecp attr format
 *	     routines.
 *
 * INPUTS:
 *  int val_syntax - the attribute syntax from the cds attr
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  
 *
 * RETURNS:
 *  a mapped syntax used by dcecp (or -1)
 *
 */
int map_val_syntax (int val_syntax)
{
    int syntax_map_table [] = {
    	-1,			cdsbyte,	cdsbyte,	    cdssmall,
    	cdsuuid,		cdsTimestamp,	cdsTimeout,	    cdsVersion,
    	cdschar,		cdsbyte,	cdsReplicaPointer,  cdsGroupMember,
    	cdsParentPointer,	cdsFullName,    cdsCHDirectory,	    cdsbyte,
    	-1,			-1,		cdsgdaPointer
    };
    
    /*
     * Pull the dcecp attr syntax from the mapping table
     * based on the cds attr syntax (if it exists)...
     */
    if (val_syntax <= (sizeof(syntax_map_table)/sizeof(int)))
        return (syntax_map_table[val_syntax]);

    return (-1);
}


/*
 * FUNCTION: is_attribute_multi_line 
 *
 * OVERVIEW: This function determines from the attribute syntax 
 *      wheather or not this attribute has multiple lines
 *      int display.
 *
 * INPUTS:
 *  int syntax - the attribute syntax
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  
 *
 * RETURNS:
 *  TRUE  for multiple lines
 *  FALSE for single lines. 
 */
int is_attribute_multi_line(int syntax)
{
    switch(syntax) {
        case cdsParentPointer:
        case cdsReplicaPointer:
        case cdsCHDirectory:
        case cdsTimeout:
        case cdsgdaPointer:
        case cdsbyte:
            return TRUE;
        default:
            return FALSE;
    }
}


/*
 * FUNCTION: copy_attr_value
 *
 * OVERVIEW: This function copies an attribute structure from cds
 *      cds_attrvalue_t. The cdspi returns a pointer to the structure 
 *      but the pointer is only gauranteed until the next use of 
 *      the cds context handle. 
 *
 * INPUTS:
 *  cds_attrvalue_t *value - attribute value structure to be copied. 
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  cds_attrvalue_t - a new pointer to the structure. 
 */
cds_attrvalue_t *copy_attr_value(cds_attrvalue_t *value)
{
    int              size;
    cds_attrvalue_t  *out;

    out = (cds_attrvalue_t *)malloc(CDS_FNAME_MAX + sizeof(cds_syntax_t));

    switch (value->val_syntax) {
        case cds_byte:
        case cds_ReplicaPointer:
        case cds_GroupMember:
        case cds_FullName:
        case cds_ParentPointer:
        case cds_CHDirectory:
        case cds_ASN1:
        case cds_gdaPointer:
            size = value->val_u.val_byte.length;
            out->val_syntax = value->val_syntax;
            out->val_u.val_byte.byte_p = malloc(size);
            memcpy(out->val_u.val_byte.byte_p, value->val_u.val_byte.byte_p, size);
            out->val_u.val_byte.length = value->val_u.val_byte.length;
            return out;
        case cds_small:
            size = sizeof(char);
            break;
        case cds_short:
            size = sizeof(short);
            break;
        case cds_long:
            size = sizeof(long);
            break;
        case cds_char:
            size = value->val_u.val_char.length + sizeof(unsigned short);
            out->val_syntax = value->val_syntax;
            out->val_u.val_char.char_p = malloc(size);
            memcpy(out->val_u.val_char.char_p, value->val_u.val_char.char_p, size);
            out->val_u.val_char.length = value->val_u.val_char.length;
            return out;
        case cds_Timeout:
            size = sizeof(cds_timeout_t);
            break;
        case cds_Version:
            size = sizeof(cdsVersion_t);
            break;
        case cds_uuid:
            size = sizeof(cds_id_t);
            break;
        case cds_Timestamp:
            size = sizeof(cds_cts_t);
            break;
        default:
            break;
    }
  
    out->val_syntax = value->val_syntax;
    memcpy(&out->val_u, &value->val_u, size);
    return out;
}


/*
 * FUNCTION: enum_one_attr
 *
 * OVERVIEW: This function returns the cds_attrvalue_t structure for one 
 *   attribute. 
 *
 * INPUTS: 
 *          Tcl_Interp *interp,  interpretor for the command
 *          cds_full_name_t    *name, full_name of object 
 *          cds_full_name_t    *clr_name, clearinghouse name 
 *          char               *attr_name, attribute name
 *          cdsEntries_t       type, type of object 
 *          cds_attribute_list **return_list attribute list 
 *                              structure to be returned
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success.
 */
int enum_one_attr(Tcl_Interp *interp, 
                  cds_full_name_t *name,
                  cds_full_name_t *clr_name,
                  char *attr_name,
                  cdsEntries_t type,
                  cds_attribute_list **return_list)
{
    cds_handle_t        attr_handle;
    struct cdsFlagStat  flags; 
    cds_attrvalue_t     *value;
    int                 i, j = 0; 
    int                 status;
    int                 size;
    cds_attr_name_t     Attr_Name;
    cds_attribute_list  *list;

    if (*return_list)
        list = *return_list;
    else {
        list = (cds_attribute_list *)malloc(sizeof(cds_attribute_list));
        if (list == NULL) {
            DCP_SET_RESULT_CODE(dcp_s_no_memory);
            return TCL_ERROR;
        }
        memset((char *)list, 0, sizeof(cds_attribute_list));  
    }
    i = list->cur_value;

    DCP_CHECK_CDS_NAME_LEN(attr_name);
    strcpy((char *)Attr_Name.uc, attr_name);
    attr_handle = cdsGetHandle();
    if (attr_handle == NULL_CDS_HANDLE) {
        free(list);
        DCP_SET_RESULT_CODE(dcp_s_cds_no_handle);
        return TCL_ERROR;
    }

    clear_flags(&flags, cdsStrDCE); 
    status = cdsReadAttrValue(name,
                              attr_handle,
                              &Attr_Name,
                              type,
                              clr_name,
                              &value,
                              NULL,
                              NULL,
                              NULL,
                              &flags);
    if (status == CDS_ERROR || status == CDS_EMPTY) {
        DCP_SET_MISC_ERROR(flags.fsLocStat);
    }

    size = (j + 1) * sizeof(cds_attribute_value *);
    list->value = (cds_attribute_value **)realloc(list->value, size);
    list->value[i] = (cds_attribute_value *)malloc(sizeof(cds_attribute_value));
    memset((char *)list->value[i], 0, sizeof(cds_attribute_value));
    list->value[i]->action = ShowAttribute;
    list->value[i]->attr_type = cdsSingle;
    list->value[i]->obj_type = type;
    list->value[i]->cur_attr = 1;
    strcpy(list->value[i]->attr_name, attr_name);
    size = sizeof(cds_attrvalue_t *);
    list->value[i]->attribute = (cds_attrvalue_t **)
        realloc(list->value[i]->attribute,size);
    list->value[i]->attribute[j] = copy_attr_value(value);
    list->cur_value++;
    *return_list = list;
    cdsFreeHandle(attr_handle);
    return TCL_OK;
}


/*
 * FUNCTION: enum_set_attr
 *
 * OVERVIEW: This function returns the cds_attrvalue_t structure for one 
 *   attribute. 
 *
 * INPUTS:
 *          Tcl_Interp         *interp, interpretor for the command
 *          cds_full_name_t    *name, full_name of object 
 *          cds_full_name_t    *clr_name, clearinghouse name 
 *          char               *attr_name, attribute name
 *          cdsEntries_t       type, type of object 
 *          cds_attribute_list **return_list attribute list 
 *                              structure to be returned
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success.
 */
int enum_set_attr(Tcl_Interp *interp, 
                  cds_full_name_t *name,
                  cds_full_name_t *clr_name,
                  char *attr_name,
                  cdsEntries_t type,
                  cds_attribute_list **return_list)
{
    cds_handle_t        attr_handle;
    struct cdsFlagStat  flags; 
    cds_attrvalue_t     *value;
    int                 i, j = 0; 
    int                 status = CDS_SUCCESS;
    int                 size;
    cds_attr_name_t     Attr_Name;
    cds_attribute_list  *list;

    if (*return_list)
        list = *return_list;
    else {
        list = (cds_attribute_list *)malloc(sizeof(cds_attribute_list));
        memset((char *)list, 0, sizeof(cds_attribute_list));  
    }

    size = (j + 1) * sizeof(cds_attribute_value *);
    list->value = (cds_attribute_value **)realloc(list->value,size);
    list->value[j] = (cds_attribute_value *)malloc(sizeof(cds_attribute_value));
    if (list->value[j] == NULL) {
        DCP_SET_RESULT_CODE(dcp_s_no_memory);
        return TCL_ERROR;
    }
    memset((char *)list->value[j], 0, sizeof(cds_attribute_value));
    list->value[j]->action = ShowAttribute;
    list->value[j]->attr_type = cdsSet;
    list->value[j]->obj_type = type;
    list->value[j]->cur_attr = 0;
    DCP_CHECK_CDS_NAME_LEN(attr_name);
    strcpy((char *)Attr_Name.uc, attr_name);
    strcpy(list->value[j]->attr_name, attr_name);
    attr_handle = cdsGetHandle();
    if (attr_handle == NULL_CDS_HANDLE) {
        free(list->value[j]);
        DCP_SET_RESULT_CODE(dcp_s_cds_no_handle);
        return TCL_ERROR;
    }
    clear_flags(&flags, cdsStrDCE); 
    i = 0;
    while (status == CDS_SUCCESS) {
        status = cdsReadAttrValue(name,
                                  attr_handle,
                                  &Attr_Name,
                                  type,
                                  clr_name,
                                  &value,
                                  NULL,
                                  NULL,
                                  NULL,
                                  &flags);
        if (status == CDS_ERROR) {
            if (flags.fsLocStat == CDS_INVALIDNAME) {
                DCP_SET_MISC_ERROR(CDS_EMPTY);
            }
            else {
                DCP_SET_MISC_ERROR(flags.fsLocStat);
            }
        }
        if (status == CDS_EMPTY) {
            list->value[j]->cur_attr = i;
            status = CDS_SUCCESS;
            break;
        }
        size = (i + 1) * sizeof(cds_attrvalue_t *);
        list->value[j]->attribute = (cds_attrvalue_t **)
            realloc(list->value[j]->attribute,size);
        list->value[j]->attribute[i] = copy_attr_value(value);
        i++;
    }
    list->cur_value++;
    cdsFreeHandle(attr_handle);
    *return_list = list;
    return TCL_OK;
}


/*
 * FUNCTION: format_byte_output
 *
 * OVERVIEW: This function will format a cdsbyte syntax attribute 
 *     for output display. The typical byte format is {ab cd 01 01}
 *     for 60 characters then a newline. 
 *
 * INPUTS:
 *  char *buffer - the unformated string value of cdsbyte attribute. 
 *  int   indent - the number of spaces to indent.
 * RETURNS:
 *  char * - the value to output. 
 *
 */
char *format_byte_output(char *buffer,int indent,boolean32 multi_line)
{
    char  *temp_buffer;
    char  *temp_byte_buffer;
    int   buffer_len;
    int   size, i;
    int   number_of_lines;
    char  indent_spaces[80];

    memset(indent_spaces, 0, sizeof(indent_spaces));
    while (indent > -1) {
        indent_spaces[indent] = ' ';
        indent--;
    }
    buffer_len = strlen(buffer);
    number_of_lines = buffer_len/40;
    size = (buffer_len * 2) + (number_of_lines * 3) + 5;
    temp_buffer = malloc(size);
    memset(temp_buffer, 0, size);
    temp_byte_buffer = buffer;
    strcpy(temp_buffer, "");
    if (!multi_line)
        strcat(temp_buffer, "{");
    for (i = 0, size = 0; i < buffer_len/2; size++, i++) {
        strncat(temp_buffer, buffer, 2);
        strcat(temp_buffer, " ");
        if (size == 20) {
            size = -1;
            if (i != (buffer_len/2 - 1)) {
                strcat(temp_buffer, "\n");
                strcat(temp_buffer, indent_spaces);
            }
        }
        buffer += 2;
    }
    *(temp_buffer + strlen(temp_buffer) - 1) = '\0';
    if (!multi_line)
        strcat(temp_buffer, "}");
    free(temp_byte_buffer);
    return temp_buffer;
}


/*
 * FUNCTION: format_byte_input
 *
 * OVERVIEW: This function takes an input cdsbyte syntax attribute
 *      and formats the string for encoding. 
 *
 * INPUTS:
 * char *buffer The attribute value entered by the user. 
 *
 * RETURNS:
 *  char * - the formated input.
 *
 */
char *format_byte_input(char *buffer)
{
    extern Tcl_Interp  *attr_interp;
    char               *command;
    char               *out_buffer;
    char               *join_format = "join [list %s] {}";

    command = malloc(strlen(join_format) + strlen(buffer) + 1);
    sprintf(command, join_format, buffer);
    Tcl_Eval(attr_interp, command);
    out_buffer = malloc(strlen(attr_interp->result) + 1);
    strcpy(out_buffer, attr_interp->result);
    free(command);
    return out_buffer;
}


/*
 * FUNCTION: strip_braces
 *
 * OVERVIEW:
 *
 * INPUTS:
 *
 * RETURNS:
 */
void strip_braces(char *buffer)
{
    extern Tcl_Interp  *attr_interp;
    char               *command;
    char               *set_format = "set foo_bar %s";

    command = malloc(strlen(set_format) + strlen(buffer) + 1);
    sprintf(command, set_format, buffer);
    Tcl_Eval(attr_interp, command);
    strcpy(buffer, attr_interp->result);
    free(command);
}
