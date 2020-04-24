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
 * $Log: rgy_pgo_utils.c,v $
 * Revision 1.1.6.1  1996/08/09  11:49:36  arvind
 * 	Merge changes.
 * 	[1996/06/11  17:04 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/2]
 *
 * 	Minor changes.
 * 	[1996/06/11  15:58 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/02/15  18:22 UTC  truitt  /main/HPDCE02/6]
 *
 * 	Merge changes.
 * 	[1996/02/15  18:20 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 *
 * 	CHFts17451: Remove include of value.h so duplicate macro
 * 	warning does not appear during HP build.  The include is
 * 	required on other platforms.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/11  15:40 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.4.2  1996/02/18  19:14:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:44  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  21:22:23  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/07/12  13:21 UTC  truitt
 * 	Merge i18n changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts38/1  1995/07/12  13:20 UTC  truitt
 * 	CHFts15275: Allow proper parsing of input and output for attributes
 * 	with encoding type i18n_data.
 * 
 * 	HP revision /main/HPDCE02/4  1995/05/01  19:07 UTC  truitt
 * 	Merge era work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  19:06 UTC  truitt
 * 	CHFts14301: Allocate memory for the sec_attr_t structures
 * 	dynamically.
 * 
 * 	HP revision /main/HPDCE02/3  1995/04/05  19:50 UTC  truitt
 * 	Merge schema message work.
 * 
 * 	HP revision /main/HPDCE02/truitt_mothra_chfts24/1  1995/04/05  19:49 UTC  truitt
 * 	CHFts14652: Improve 'schema entry not found' error message.
 * 
 * 	HP revision /main/HPDCE02/2  1995/03/09  14:30 UTC  truitt
 * 	Handle a bogus -attr value.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts18/1  1995/03/09  14:29 UTC  truitt
 * 	CHFts14579: Have the parse attribute function check
 * 	early if it is playing with an empty attribute name.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/15  19:56 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_chk_01/1  1994/12/15  19:53 UTC  truitt
 * 	Change the way encoding types are checked during the
 * 	show, print, and modify routines.  Instead of using
 * 	the encoding type from the attribute structure, we must
 * 	use the encoding type from the schema structure.
 * 	[1995/12/08  17:56:47  root]
 * 
 * Revision 1.1.2.8  1994/10/05  18:43:03  truitt
 * 	OT12492: In the encode_rgy_pgo_item_attr() function, the
 * 	value part of the attribute structure needs to be initialized
 * 	correctly when special case is true.
 * 	[1994/10/05  18:39:33  truitt]
 * 
 * Revision 1.1.2.7  1994/09/29  18:45:18  truitt
 * 	OT10967: Allow proper display of multiple values per attribute.
 * 	Also, cleaned up some of the 'free' code.
 * 	[1994/09/29  17:34:07  truitt]
 * 
 * Revision 1.1.2.6  1994/09/28  20:11:49  ajayb
 * 	Merged with changes from 1.1.2.5
 * 	[1994/09/28  20:10:48  ajayb]
 * 
 * 	Make is_yes() accept valid prefixes of
 * 	"yes" and "no" as valid inputs.
 * 	[1994/09/27  19:56:52  ajayb]
 * 
 * Revision 1.1.2.5  1994/09/28  13:27:49  kevins
 * 	OT 11597 modified uid/gid/orgid error messages
 * 	[1994/09/28  13:24:17  kevins]
 * 
 * Revision 1.1.2.4  1994/09/16  16:50:43  truitt
 * 	Forgot one of the encoding types.
 * 	[1994/09/16  16:50:16  truitt]
 * 
 * Revision 1.1.2.3  1994/09/16  16:02:48  truitt
 * 	When doing a modify -remove by type, some encoding
 * 	types require the value part of the structure to
 * 	be set explicitly to NULL.
 * 	[1994/09/16  16:02:17  truitt]
 * 
 * Revision 1.1.2.2  1994/09/13  19:12:39  kevins
 * 	OT 11641 list syntax not supported in catalog commands.
 * 	[1994/09/13  18:46:41  kevins]
 * 
 * Revision 1.1.2.1  1994/09/06  16:19:56  kevins
 * 	11694 - Broke up rgy_utils.c into:
 * 	                        rgy_pgo_utils.c
 * 	                        rgy_utils.c
 * 	[1994/09/06  16:11:21  kevins]
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

#define RGY_CHECK_RESULT(result)                    \
    if(result != sec_rgy_status_ok){                \
        *status = result;                           \
        return;                                     \
    }                                               \


/*
 * FUNCTION:  mod_rgy_quota
 *
 * OVERVIEW:
 *  update the quota field in the pgo item. 
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
int mod_rgy_quota(char *quota, sec_rgy_pgo_item_t *pgo_item)
{
    char  *unlimited_svc;
    int   quota_int = -1;
    char  *end;

    unlimited_svc = (char *)dce_sprintf(dcp_t_rgy_unlimited);
    if (unlimited_svc == NULL)
        return dcp_s_no_memory;
    if (!strcmp((char *)quota, (char *)unlimited_svc)) {
        pgo_item->quota = -1;
    }
    else {
        quota_int = is_number(quota);
        if (quota_int == dcp_rgy_not_int) {
            free(unlimited_svc);
            return dcp_s_rgy_quota_not_int;
        }
        else if (quota_int == dcp_rgy_int_out_of_range) {
            free(unlimited_svc);
            return dcp_s_rgy_quota_out_of_range;
        }
        quota_int = strtol(quota, &end, 10);
        if (quota_int < -1) {
            free(unlimited_svc);
            return dcp_s_rgy_quota_is_negative;
        }
        pgo_item->quota = quota_int;
    }
    free(unlimited_svc);
    return sec_rgy_status_ok;
}


/*
 * FUNCTION: change_rgy_name
 *
 * OVERVIEW:
 * 
 *  update the org name
 *
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int change_rgy_name(sec_rgy_name_t from_name,
                    char *to_name,
                    sec_rgy_domain_t domain)
{
    sec_rgy_name_t new_name;
    error_status_t result;

    result = error_status_ok;
    strcpy((char *)new_name, to_name);
    sec_rgy_pgo_rename(rgy_context, domain, from_name, new_name, &result);
    return result;
}


/*
 * FUNCTION: add_uid
 *
 * OVERVIEW:
 * 
 * Add the unix number to the pgo item
 * INPUTS:
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 */
int add_uid(char *uid,
            sec_rgy_pgo_item_t *pgo_item)
{
    int   uid_no = 0;
    char  *end;

    uid_no = is_number(uid);

    if (uid_no == dcp_rgy_not_int)
        return dcp_s_rgy_uid_is_wrong;
    else if (uid_no == dcp_rgy_int_out_of_range)
        return dcp_s_rgy_uid_out_of_range;
    uid_no = 0;
    uid_no = strtol(uid,&end,10);

    if (uid_no < 0) 
        return dcp_s_rgy_uid_is_wrong;
    else
        pgo_item->unix_num = uid_no;

    return sec_rgy_status_ok;
}


/*
 * FUNCTION: add_acctlife
 *
 * OVERVIEW: This function takes an ascii string that 
 *      represents the account lifespan and translates it into 
 *      a policy attribute. 
 * 
 *
 * INPUTS:
 *  char *acct_life - ascii string representing account lifespan
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_plcy_t *policy - pointer to policy structure.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  sec_rgy_status_ok on success.
 */
int add_acctlife(char *acct_life,
                 sec_rgy_plcy_t *policy)
{
    sec_timeval_sec_t  attlife_in_sec = 0;
    char               *unlimited_svc;

    unlimited_svc = (char *)dce_sprintf(dcp_t_rgy_unlimited);
    if (unlimited_svc == NULL)
        return dcp_s_no_memory;

    if (!strcmp((char *)acct_life, (char *)unlimited_svc)) {
        free(unlimited_svc);
        policy->acct_lifespan = 0;
        return sec_rgy_status_ok;
    }
    free(unlimited_svc);

    reldate_to_seconds(&attlife_in_sec, acct_life);
    if (attlife_in_sec == 0)
        return dcp_s_rgy_acct_is_bad;
    policy->acct_lifespan = attlife_in_sec;
    return sec_rgy_status_ok;
}


/*
 * FUNCTION: add_pwdlife
 *
 * OVERVIEW: This function takes an ascii string representing the
 *    password life and converts it into a policy attribute.
 * 
 * INPUTS:
 *  char *pwd_life - ascii string representing passwd life.
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_plcy_t *policy - pointer to a policy structure. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  sec_rgy_status_ok on successful completion
 */
int add_pwdlife(char *pwd_life, sec_rgy_plcy_t *policy)
{
    sec_timeval_sec_t  pwdlife_in_sec = 0;
    char               *unlimited_svc;

    unlimited_svc = (char *)dce_sprintf(dcp_t_rgy_unlimited);
    if (unlimited_svc == NULL)
        return dcp_s_no_memory;

    if (!strcmp((char *)pwd_life, (char *)unlimited_svc)) {
        free(unlimited_svc);
        policy->passwd_lifetime = 0;
        return sec_rgy_status_ok;
    }
    free(unlimited_svc);

    reldate_to_seconds(&pwdlife_in_sec, pwd_life);
    if (!pwdlife_in_sec)
        return dcp_s_rgy_pwdlife_is_bad;
    policy->passwd_lifetime = pwdlife_in_sec;

    return sec_rgy_status_ok;
}


/*
 * FUNCTION: is_yes
 *
 * OVERVIEW: This function takes an ascii string and determines 
 *     whether it is a "yes" or a "no". Returns TRUE for yes
 *     and FALSE for no
 *
 * INPUTS:
 *    char *yes_no - ascii string representing yes or no.
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TRUE for yes FALSE for no.  
 */
int is_yes(char *yes_no)
{
    char  *yes = NULL;
    char  *no = NULL;
    int   result = -1;

    yes = (char *)dce_sprintf(dcp_t_rgy_yes);
    no = (char *)dce_sprintf(dcp_t_rgy_no);
    if (no == NULL || yes == NULL)
        return dcp_s_no_memory;
    if (!strncmp(yes, yes_no, strlen(yes_no)))
        result = TRUE;
    else if (!strncmp(no, yes_no, strlen(yes_no)))
        result = FALSE;
    else
        result = dcp_s_rgy_invalid_pnvalue;

    free(no);
    free(yes);
    return result;
}


/*
 * FUNCTION: init_pgo_item
 *
 * OVERVIEW: This function initializes a pgo item for modification. 
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_pgo_item_t *pgo_item 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void init_pgo_item(sec_rgy_pgo_item_t *pgo_item)
{
    memset((char *)pgo_item, 0, sizeof(sec_rgy_pgo_item_t));
    pgo_item->unix_num = -1;
    pgo_item->quota = -2;
    pgo_item->flags = 0;
}


/*
 * FUNCTION: init_policy()
 *
 * OVERVIEW: This function initialzes a policy for modification;
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_plcy_t * The policy to be initialized.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void init_policy(sec_rgy_plcy_t *policy)
{
    memset((char *)policy, 0, sizeof(sec_rgy_plcy_t));
    policy->passwd_min_len = -1;
    policy->passwd_exp_date = -1;
    policy->passwd_lifetime = -1;
    policy->passwd_flags = 0;
    policy->acct_lifespan = -1;
}


/*
 * FUNCTION: copy_policy_with_restrict
 *
 * OVERVIEW: This copies policy information from one policy struct to another. 
 *      The copy is doen with restrictions. That is if a field in the from 
 *      policy has been modified it will be copied to the to policy. The from 
 *      policy should have been initialized properly for the restrictions to 
 *      be enforced. 
 *
 * INPUTS:
 *  sec_rgy_plcy_t *from_policy 
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_plcy_t *to_policy;
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void copy_policy_with_restrict(sec_rgy_plcy_t *to_policy, 
                               sec_rgy_plcy_t *from_policy)

{
    if (from_policy->passwd_min_len != -1)
        to_policy->passwd_min_len = from_policy->passwd_min_len;
    if (from_policy->passwd_lifetime != -1)
        to_policy->passwd_lifetime  = from_policy->passwd_lifetime;
    if (from_policy->passwd_exp_date != -1)
        to_policy->passwd_exp_date = from_policy->passwd_exp_date;
    if (from_policy->acct_lifespan != -1)
        to_policy->acct_lifespan = from_policy->acct_lifespan;
    if (from_policy->passwd_flags & rgy_plcy_alpha_found) {
        if (from_policy->passwd_flags & sec_rgy_plcy_pwd_non_alpha)
            to_policy->passwd_flags|=sec_rgy_plcy_pwd_non_alpha;
        else
            to_policy->passwd_flags&=~sec_rgy_plcy_pwd_non_alpha;
    }
    if (from_policy->passwd_flags & rgy_plcy_spaces_found) {
        if (from_policy->passwd_flags & sec_rgy_plcy_pwd_no_spaces)
            to_policy->passwd_flags|=sec_rgy_plcy_pwd_no_spaces;
        else
            to_policy->passwd_flags&=~sec_rgy_plcy_pwd_no_spaces;
    }
}


/*
 * FUNCTION: copy_pgo_item_with_restrict
 *
 * OVERVIEW: This copies pgo_item information from one pgo_item struct 
 *      to another. The copy is doen with restrictions. That is if a 
 *      field in the from pgo_item has been modified it will be copied 
 *      to the to pgo_item. The from pgo_item should have been initialized 
 *      properly for the restrictions to be enforced. 
 *
 * INPUTS:
 * sec_rgy_pgo_item_t  *from_pgo
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_pgo_item_t *to_pgo
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  void
 */
void copy_pgo_item_with_restrict(sec_rgy_pgo_item_t *to_pgo, 
                                 sec_rgy_pgo_item_t *from_pgo,
                                 rgy_attribute_info_t *attr_info)
{
    if (from_pgo->unix_num != -1)
        to_pgo->unix_num = from_pgo->unix_num ;
    if (from_pgo->quota != -2)
        to_pgo->quota = from_pgo->quota;
    if (strlen((char *)from_pgo->fullname) || attr_info->fullname)
        strcpy((char *)to_pgo->fullname, (char *)from_pgo->fullname);
    if (from_pgo->flags & rgy_pgo_alias_found) {
        if (from_pgo->flags & sec_rgy_pgo_is_an_alias)
            to_pgo->flags|=sec_rgy_pgo_is_an_alias;
        else
            to_pgo->flags&=~sec_rgy_pgo_is_an_alias;
    }
    if (from_pgo->flags & rgy_pgo_inprojlist_found) {
        if (from_pgo->flags & sec_rgy_pgo_projlist_ok)
            to_pgo->flags|=sec_rgy_pgo_projlist_ok;
        else
            to_pgo->flags&=~sec_rgy_pgo_projlist_ok;
    }
}


/*
 * FUNCTION: add_pgo_flag
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
int add_pgo_flag(char *value,
                 sec_rgy_pgo_item_t *pgo_item,
                 int flag)
{
    int  yes_no = -1;

    if (pgo_item->flags == (unsigned32)-1)
        pgo_item->flags = 0;

    yes_no = is_yes(value);
    if (yes_no == TRUE) {
        pgo_item->flags|=flag;
    } else if(yes_no == FALSE) {
        pgo_item->flags = pgo_item->flags & ~flag;
    } else
        return yes_no;

    return sec_rgy_status_ok;
}


/*
 * FUNCTION: add_policy_flag
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
int add_policy_flag(char *value,
                    sec_rgy_plcy_t *policy,
                    int flag)
{
    int  yes_no = -1;

    yes_no = is_yes(value);
    if (yes_no == TRUE) 
        policy->passwd_flags&=~flag;
    else if (yes_no == FALSE)
        policy->passwd_flags|=flag;
    else
        return yes_no;

    return sec_rgy_status_ok;
}


/*
 * FUNCTION: add_pwdminlen
 *
 * OVERVIEW: This adds pawwsord min length to the rgy policy item passed in.
 * 
 *
 * INPUTS:
 *  char *length_ptr
 *
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_plcy_t *policy
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  dcp_s_rgy_pwdlen_out_of_range - if the integer is out of range.
 * 
 */
int add_pwdminlen(char *length_ptr,
                  sec_rgy_plcy_t *policy)
{
    int   length;
    int   status;
    char  *end;

    status = is_number(length_ptr);
    if (status == dcp_rgy_not_int)
        return dcp_s_rgy_pwdminlen_not_int;
    else if (status == dcp_rgy_int_out_of_range)
        return dcp_s_rgy_pwdminlen_out_of_range;

    length = strtol(length_ptr,&end,10);

    if (length < 0) 
        return dcp_s_rgy_pwdminlen_is_negative;
    else
        policy->passwd_min_len = length;
    
    return sec_rgy_status_ok;
}


/*
 * FUNCTION: add_pwdexpdate
 *
 * OVERVIEW: add passwd expiration date into supplied policy structure.
 * 
 *
 * INPUTS:
 *  char *date_ptr - ascii string describing the date to me added
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_plcy_t *policy - Policy structure to add date into.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int success/failure code. 
 */
int add_pwdexpdate(char *date_ptr,
                   sec_rgy_plcy_t *policy)
{
    char               *now_svc;
    char               *none_svc;
    sec_timeval_sec_t  seconds = 0;
    error_status_t     status = error_status_ok;
    time_t             tloc;

    none_svc = (char *)dce_sprintf(dcp_t_rgy_none);
    if (!none_svc)
        return dcp_s_no_memory;

    /*
     * Catch the "live forever" constant
     */
    if ((strcmp(date_ptr,rgy_plcy_null_date) == 0) || (strcmp(date_ptr, none_svc) == 0)) {
        policy->passwd_exp_date = org_pol_live_forever;
        free(none_svc);
        return status;
    }
    free(none_svc);

    now_svc = (char *)dce_sprintf(dcp_t_rgy_date_now);
    if (!now_svc)
        return dcp_s_no_memory;

    /*
     * Special case the "now" date.
     */
    if (strcmp(date_ptr, now_svc) == 0) {
        time(&tloc);
        policy->passwd_exp_date = tloc;
        free(now_svc);
        return error_status_ok;
    }  
    free(now_svc);

    date_to_seconds(&seconds, date_ptr);
    if (!seconds)
        return dcp_s_rgy_bad_expdate;
    policy->passwd_exp_date = seconds;

    return status;
}


/*
 * FUNCTION: encode_rgy_pgo_item_attr
 *
 * OVERVIEW: This decodes ascii pgo_items and inserts them into
 *      a sec_rgy_pgo_item_t structure.
 *
 * INPUTS:
 *  Tcl_Interp *interp - the tcl interpreter structure for this invocation
 *  char **attr - the attribute list, contains the attribute name in the
 *                first position, then one or more values for that attribute
 *                in the succeeding positions.
 *  int num_val - the number of attributes in the above list; note this is
 *                not the number of values, rather the number of values plus
 *                the attribute name.
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_pgo_item_t *pgo_item    - holds pgo item information
 *  rgy_attribute_info_t *attr_info - holds attribute information, including
 *                                    the count of extended attributes specified.
 *
 * OUTPUTS:
 *  sec_attr_t *attrs  - holds the attribute values for any era's specified.
 *  int *bad_value     - an integer representing the position in the attribute
 *                       list of the first unknown attribute name found.  Only
 *                       used when a list of attribute names is being processed
 *                       for removal by type.
 *  unsigned32 *status - sends back to the calling function the status of this
 *                       function.  No problem is returned as sec_rgy_status_ok.
 *                       Other error status codes are possible.
 *
 * RETURNS:
 *  void
 */
void encode_rgy_pgo_item_attr(Tcl_Interp *interp,
                              char **attr,
                              int *num_val, 
                              sec_rgy_pgo_item_t *pgo_item, 
                              rgy_attribute_info_t *attr_info, 
                              sec_attr_t *attrs,
                              int *bad_value,
                              unsigned32 *status)
{
   unsigned32                 result = sec_rgy_status_ok;
   sec_attr_component_name_t  schema_name = NULL;
   sec_attr_schema_entry_t    schema_entry;
   char                       *attr_name;
   boolean32                  special_case = FALSE;
   int                        i, j;
   unsigned32                 tmp_status;
   int                        encptr = -1;
   int                        hits;
   int                        p_count;
   boolean32                  any_case = FALSE;

   /*
    * The attribute name is the first item in the attr structure.
    * The next and succeeding items in the attr structure are
    * values associated with that attribute.
    */
   attr_name = attr[0];
   if (strlen(attr_name) <= 0) {
      *status = dcp_s_rgy_bad_attribute_list;
      return;
   }

   /*
    * Process the attribute values based on the type of attribute
    * passed in.  If the attribute type is not one of the
    * hard-coded attribute types, it is then by definition
    * either a legal extended attribute or an unknown attribute.
    * These will get processed by the 'default' case.
    */
   switch(rgy_attribute(attr_name)) {
      case rgy_pgo_quota:
         if (*num_val != 2) {
            *status = dcp_s_rgy_bad_quota_attr_val;
            return;
         }
         if (attr_info->domain != sec_rgy_domain_person) {
            *status = dcp_s_rgy_invalid_attribute;
            return;
         }
         if (attr_info->modify_type == modify_type_add ||
             attr_info->modify_type == modify_type_remove) {
            *status = dcp_s_rgy_era_illegal_attribute_list;
            return;
         }
         result = mod_rgy_quota(attr[1],pgo_item);
         if (result != sec_rgy_status_ok) {
            *status = result;
            return; 
         }
         attr_info->pgo = TRUE;
         break;

      case rgy_pgo_is_an_alias:
         if (attr_info->domain == sec_rgy_domain_org) {
            *status = dcp_s_rgy_invalid_attribute;
            return;
         }
         if (attr_info->modify_type == modify_type_add ||
             attr_info->modify_type == modify_type_remove) {
            *status = dcp_s_rgy_era_illegal_attribute_list;
            return;
         }
         if (*num_val != 2) {
            *status = dcp_s_rgy_bad_alias_attr_val;
            return;
         }
         result = add_pgo_flag(attr[1], pgo_item, sec_rgy_pgo_is_an_alias);
         if (result != sec_rgy_status_ok) {
            *status = result;
            return;
         }
         pgo_item->flags|=rgy_pgo_alias_found;
         attr_info->pgo = TRUE;
         break;

      case rgy_pgo_inprojlist:
         if (attr_info->domain != sec_rgy_domain_group) {
            *status = dcp_s_rgy_invalid_attribute;
            return;
         }
         if (attr_info->modify_type == modify_type_add ||
             attr_info->modify_type == modify_type_remove) {
            *status = dcp_s_rgy_era_illegal_attribute_list;
            return;
         }
         if (*num_val != 2) {
            *status = dcp_s_rgy_bad_inprojlist_attr_val;
            return;
         }
         result = add_pgo_flag(attr[1], pgo_item, sec_rgy_pgo_projlist_ok);
         pgo_item->flags|=rgy_pgo_inprojlist_found;
         attr_info->pgo = TRUE;
         break;

      case rgy_pgo_fullname:
         if (attr_info->modify_type == modify_type_add ||
             attr_info->modify_type == modify_type_remove) {
            *status = dcp_s_rgy_era_illegal_attribute_list;
            return;
         }
         if (*num_val != 2) {
            *status = dcp_s_rgy_bad_fullname_attr_val;
            return;
         }
         strcpy((char *)pgo_item->fullname, attr[1]);
         attr_info->fullname = dce_strdup((char *)attr);
         attr_info->pgo = TRUE;
         break;

      case rgy_pgo_unix_num:
        if (*num_val > 2) {
            *status = dcp_s_rgy_bad_attribute_value;
            return;
        }
        if (attr_info->flags == pgo_modify) {
            *status = dcp_s_rgy_invalid_attribute;
            return;
        }
        result = sec_rgy_status_ok;
        switch ((int)attr_info->domain) {
          case sec_rgy_domain_person:
            if (*num_val != 2) {
                *status = dcp_s_rgy_bad_uid_attr_val;
                return;
            }
            result = check_uid_validity(sec_rgy_domain_person, 
                                        dcp_t_rgy_uid,
                                        attr_name);
            RGY_CHECK_RESULT(result);
            result = add_uid(attr[1],pgo_item);
            RGY_CHECK_RESULT(result);
            break;
          case sec_rgy_domain_group:
            if (*num_val != 2) {
                *status = dcp_s_rgy_bad_gid_attr_val;
                return;
            }
            result = check_uid_validity(sec_rgy_domain_group, 
                                        dcp_t_rgy_gid,
                                        attr_name);
            RGY_CHECK_RESULT(result);
            result = add_uid(attr[1],pgo_item);
            if (result == dcp_s_rgy_uid_is_wrong)
                result = dcp_s_rgy_gid_is_wrong;
            else if (result == dcp_s_rgy_uid_is_wrong) 
                result = dcp_s_rgy_gid_is_wrong;
            RGY_CHECK_RESULT(result);
            break;
          case sec_rgy_domain_org:
            if (*num_val != 2) {
                *status = dcp_s_rgy_bad_orgid_attr_val;
                return;
            }
            result = check_uid_validity(sec_rgy_domain_org, 
                                        dcp_t_rgy_orgid,
                                        attr_name);
            RGY_CHECK_RESULT(result);
            result = add_uid(attr[1],pgo_item);
            if (result == dcp_s_rgy_uid_is_wrong)
                result = dcp_s_rgy_orgid_is_wrong;
            else if (result == dcp_s_rgy_uid_is_wrong)
                result = dcp_s_rgy_orgid_is_wrong;
            RGY_CHECK_RESULT(result);
            break;
        }
        attr_info->pgo = TRUE;
        break;

      case rgy_pgo_uuid:
         if (attr_info->flags == pgo_modify) {
            *status = dcp_s_rgy_invalid_attribute;
            return;
         }
         if (*num_val != 2) {
            *status = dcp_s_rgy_bad_uuid_attr_val;
            return;
         }
         uuid_from_string((unsigned_char_t *)attr[1],&(pgo_item->id),
                          (unsigned32 *)&result);
         RGY_CHECK_RESULT(result);
         attr_info->pgo = TRUE;
         break;

      case rgy_plcy_acctlife:
      case rgy_plcy_pwdalpha:
      case rgy_plcy_pwdspaces:
      case rgy_plcy_pwdexpdate:
      case rgy_plcy_pwdminlen:
      case rgy_plcy_pwdlife:
         if (attr_info->domain != sec_rgy_domain_org) {
            *status = dcp_s_rgy_illegal_attr;
            return;
         }
         if (attr_info->modify_type == modify_type_add ||
             attr_info->modify_type == modify_type_remove) {
            *status = dcp_s_rgy_era_illegal_attribute_list;
            return;
         }
         break;

      default:
         /*
          * One of the usual, hard-coded registry attribute names
          * was not specified, so we might have an extended attribute name.
          * Check the schema and see if the attribute name is a
          * legal attribute name.
          */
         sec_rgy_attr_sch_lookup_by_name(rgy_context,
                                         schema_name,
                                         (idl_char *)attr_name,
                                         &schema_entry,
                                         &result);
         if (result != sec_rgy_status_ok) {
            /*
             * The attribute name is unknown, so tell the user and exit.
             */
            if ((result == rpc_s_unknown_if) || (result == ept_s_not_registered) ||
                (result == rpc_s_comm_failure))
               *status = dcp_s_rgy_illegal_attr;
            else
               *status = result;
            return;
         }

         /*
          * Since we got here, we must have a legal extended attribute name.
          * If the number of values passed in is zero or less, then there
          * is a problem.
          */
         if (*num_val <= 0) {
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
            *status = dcp_s_rgy_era_wrong_num_values;
            return;
         }

         /*
          * Regardless of how many values are passed, if the encoding type is void,
          * and the modify type is change, then this is an error.  There is no way
          * to change an attribute of type void.  If we are doing a create, or
          * a modify/add or a modify/remove, then we must check through the attrs
          * structure for any voids of the same name that are already marked for
          * processing.  The security api code will allow multiple void attributes
          * to exist in the database, but our treatment of voids implies only one.
          * Therefore, trap any extra voids of the same name now.
          */
         if (schema_entry.attr_encoding == sec_attr_enc_void) {
            if (attr_info->modify_type == modify_type_change) {
               sec_attr_util_sch_ent_free_ptrs(&schema_entry);
               *status = dcp_s_rgy_era_illegal_change_command;
               return;
            }
            else {
               if (*num_val > 2) {
                  sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                  *status = dcp_s_rgy_era_too_many_voids;
                  return;
               }
               for (i = 0; i < attr_info->era_count; i++) {
                  if (uuid_equal(&attrs[i].attr_id, &schema_entry.attr_id, &tmp_status)) {
                     sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                     *status = dcp_s_rgy_era_too_many_voids;
                     return;
                  }
               }
            }
         }

         /*
          * If the number of values passed is one (meaning only the attribute
          * name is here and we don't have an attribute-value pair), then we
          * must check to see we are doing a 'modify -remove'.  Only on this
          * specific command can we process an attribute with no value.
          */
         if ((*num_val == 1) && (attr_info->modify_type != modify_type_remove)) {
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
            *status = dcp_s_rgy_era_wrong_num_values;
            return;
         }

         /*
          * Check for any special processing required for attribute of encoding
          * type 'any'.  The input for this case must contain an encoding type
          * to use during the processing (unless we are removing by type).  Check
          * that the proper number of parameters were passed in.  Set the any flag.
          */
         if (schema_entry.attr_encoding == sec_attr_enc_any) {
            if (*num_val < 3) {
               if (attr_info->modify_type != modify_type_remove) {
                  sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                  *status = dcp_s_rgy_era_type_and_value;
                  return;
               }
               else {
                  if ((attr_info->types == FALSE) && (*num_val != 1)) {
                     sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                     *status = dcp_s_rgy_era_any_illegal_remove;
                     return;
                  }
               }
            }
            if (*num_val > 1) {
               hits = 0;
               for (i = 0; i < MAX_ENCODING; i++) {
                  if (strncmp(attr[1], enc_type_name[i], strlen(attr[1])) == 0) {
                     if (!strcmp(attr[1], enc_type_name[i])) {
                        hits = 1;
                        encptr = i;
                        break;
                     }
                     encptr = i;
                     hits++;
                  }
               }
               if (attr_info->types == FALSE) {
                  if (hits == 0) {
                     sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                     *status = dcp_s_rgy_era_any_unknown_enc_type;
                     return;
                  }
                  if (hits > 1) {
                     sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                     *status = dcp_s_rgy_era_any_ambig_enc_type;
                     return;
                  }
                  if (encptr == 0) {
                     sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                     *status = dcp_s_rgy_era_any_illegal_enc_type;
                     return;
                  }

                  /*
                   * Check if this 'any' attribute is being set up as an encoding
                   * type void.  If so, make sure only one instance is being passed.
                   */
                  if (encptr == 1) {
                     if (attr_info->modify_type == modify_type_change) {
                        sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                        *status = dcp_s_rgy_era_illegal_change_command;
                        return;
                     }
                     else {
                        if (*num_val > 3) {
                           sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                           *status = dcp_s_rgy_era_too_many_voids;
                           return;
                        }
                     }
                  }
               }
               else if ((attr_info->types == TRUE) && (encptr > -1) && (*num_val > 2)) {
                  sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                  *status = dcp_s_rgy_era_any_illegal_remove;
                  return;
               }
            }
            any_case = TRUE;
         }

         /*
          * If we have values passed, and we are doing a remove, and the types
          * flag is set, set the special case variable.  If the encoding type
          * of the attribute is any, just set the types flag instead.
          */
         if ((*num_val >= 2) && (attr_info->modify_type == modify_type_remove) &&
             (attr_info->types == TRUE)) {
            if ((any_case == TRUE) && (encptr != -1))
               attr_info->types = TRUE;
            else
               special_case = TRUE;
         }

         /*
          * If we are doing a remove with no values passed in, then set
          * the remove_by_type flag to TRUE, since that will be a signal
          * to remove all instances, regardless of value.  It might already
          * be set, but we'll set it anyway.
          */
         if ((*num_val == 1) && (attr_info->modify_type == modify_type_remove)) {
            attr_info->types = TRUE;
         }

         if (special_case == TRUE) {
            /*
             * We are trying to delete a list of attributes by type only,
             * with no values.  The num_val variable reflects the number
             * of attributes to delete, and the attr variable holds the
             * different names.  Each should be looked up and processed
             * one at a time.  First check for multiple voids.
             */
            for (i = 0; i < *num_val; i++) {
               sec_attr_util_sch_ent_free_ptrs(&schema_entry);
               sec_rgy_attr_sch_lookup_by_name(rgy_context,
                                               schema_name,
                                               (idl_char *)attr[i],
                                               &schema_entry,
                                               &result);
               if (result != sec_rgy_status_ok) {
                  *bad_value = i;
                  *status = result;
                  return;
               }

               if (schema_entry.attr_encoding == sec_attr_enc_void) {
                  for (j = 0; j < attr_info->era_count; j++) {
                     if (uuid_equal(&attrs[j].attr_id, &schema_entry.attr_id, &tmp_status)) {
                        sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                        *status = dcp_s_rgy_era_too_many_voids;
                        return;
                     }
                  }
               }

               attrs[attr_info->era_count].attr_id = schema_entry.attr_id;
               attrs[attr_info->era_count].attr_value.attr_encoding = schema_entry.attr_encoding;
               switch (schema_entry.attr_encoding) {
                  case sec_attr_enc_bytes:
                  case sec_attr_enc_confidential_bytes:
                     attrs[attr_info->era_count].attr_value.tagged_union.bytes = NULL;
                     break;
                  case sec_attr_enc_printstring:
                     attrs[attr_info->era_count].attr_value.tagged_union.printstring = NULL;
                     break;
                  case sec_attr_enc_printstring_array:
                     attrs[attr_info->era_count].attr_value.tagged_union.string_array = NULL;
                     break;
                  case sec_attr_enc_i18n_data:
                     attrs[attr_info->era_count].attr_value.tagged_union.idata = NULL;
                     break;
                  case sec_attr_enc_attr_set:
                     attrs[attr_info->era_count].attr_value.tagged_union.attr_set = NULL;
                     break;
                  case sec_attr_enc_binding:
                     attrs[attr_info->era_count].attr_value.tagged_union.binding = NULL;
                     break;
               }
               attr_info->era_count++;
            }
         }
         else if (attr_info->types == TRUE) {
            /*
             * We are trying to delete an instance by type and not by
             * value, so we have nothing to process.  Set up an entry
             * in the attrs structure that reflects the id and encoding
             * type of the attribute, but with no value.  Some encoding
             * types require the value structure to be NULL to ensure
             * the database lookup works correctly.
             */
            attrs[attr_info->era_count].attr_id = schema_entry.attr_id;
            if (encptr == -1) {
               attrs[attr_info->era_count].attr_value.attr_encoding = schema_entry.attr_encoding;
            }
            else {
               attrs[attr_info->era_count].attr_value.attr_encoding = encptr;
            }
            switch (attrs[attr_info->era_count].attr_value.attr_encoding) {
               case sec_attr_enc_bytes:
               case sec_attr_enc_confidential_bytes:
                  attrs[attr_info->era_count].attr_value.tagged_union.bytes = NULL;
                  break;
               case sec_attr_enc_printstring:
                  attrs[attr_info->era_count].attr_value.tagged_union.printstring = NULL;
                  break;
               case sec_attr_enc_printstring_array:
                  attrs[attr_info->era_count].attr_value.tagged_union.string_array = NULL;
                  break;
               case sec_attr_enc_i18n_data:
                  attrs[attr_info->era_count].attr_value.tagged_union.idata = NULL;
                  break;
               case sec_attr_enc_attr_set:
                  attrs[attr_info->era_count].attr_value.tagged_union.attr_set = NULL;
                  break;
               case sec_attr_enc_binding:
                  attrs[attr_info->era_count].attr_value.tagged_union.binding = NULL;
                  break;
            }
            attr_info->era_count++;
         }
         else if ((*num_val >= 2) && ((schema_entry.attr_encoding == sec_attr_enc_attr_set) ||
                  (schema_entry.attr_encoding == sec_attr_enc_printstring_array))) {
            /*
             * This type of attribute can have multiple values stored in
             * the same instance, so all values are processed together.
             * Store the attribute value information in the correct structure.
             */
            result = get_attr_values(interp,
                                     schema_entry.attr_encoding,
                                     schema_entry.attr_id,
                                     &attrs[attr_info->era_count],
                                     num_val,
                                     attr);
            if (result != sec_rgy_status_ok) {
               sec_attr_util_sch_ent_free_ptrs(&schema_entry);
               *status = result;
               return;
            }
            attr_info->era_count++;
         }
         else if ((*num_val >= 2) && ((encptr == 4) || (encptr == 9)) && (any_case == TRUE)) {
            /*
             * We are processing the type of attribute that can have multiple
             * values stored in the same instance, and we are doing it under
             * the guise of an encoding type 'any'.  Assign the attribute
             * encoding type and the number of values to process correctly.
             */
            for (i = 1; i < *num_val-1; i++) {
               attr[i] = attr[i+1];
            }
            p_count = *num_val - 1;
            result = get_attr_values(interp,
                                     (sec_attr_encoding_t)encptr,
                                     schema_entry.attr_id,
                                     &attrs[attr_info->era_count],
                                     &p_count,
                                     attr);
            if (result != sec_rgy_status_ok) {
               sec_attr_util_sch_ent_free_ptrs(&schema_entry);
               *status = result;
               return;
            }
            attr_info->era_count++;
         }
         else if ((*num_val >= 2) && (any_case == TRUE)) {
            /*
             * We are trying to process the values passed in to an attribute
             * of encoding type 'any'.  The next value in the list is the
             * encoding type to use when processing the given values.
             */
            for (i = 2; i < *num_val; i++) {
               attr[1] = attr[i];
               result = get_attr_values(interp,
                                        (sec_attr_encoding_t)encptr,
                                        schema_entry.attr_id,
                                        &attrs[attr_info->era_count],
                                        num_val,
                                        attr);
               if (result != sec_rgy_status_ok) {
                  sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                  *status = result;
                  return;
               }
               attr_info->era_count++;
            }
         }
         else if (*num_val >= 2) {
            /*
             * The rest of the attribute types (not one of those mentioned above)
             * must have their multiple values, if any, stored in separate instances.
             * Store each attribute-value pair in a different entry in the structure.
             */
            for (i = 1; i < *num_val; i++) {
               attr[1] = attr[i];
               result = get_attr_values(interp,
                                        schema_entry.attr_encoding,
                                        schema_entry.attr_id,
                                        &attrs[attr_info->era_count],
                                        num_val,
                                        attr);
               if (result != sec_rgy_status_ok) {
                  sec_attr_util_sch_ent_free_ptrs(&schema_entry);
                  *status = result;
                  return;
               }
               attr_info->era_count++;
            }
         }
         else {
            /*
             * We have some case here which we did not anticipate.
             * This is an internal error.
             */
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
            *status = dcp_s_rgy_era_internal_error;
            return;
         }

         /*
          * If we get to here, that means we successfully stored one or
          * more extended attributes in the proper structure.  The count
          * of extended attributes has already been updated.  Free the
          * pointers allocated for the schema_entry structure.
          */
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         break;
    }
    *status = sec_rgy_status_ok;
    return;
}


/*
 * FUNCTION:  parse_attribute_list
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
 *  int
 */

int parse_attribute_list(Tcl_Interp *interp,
                         char *attr_list, 
                         sec_rgy_pgo_item_t *bad_pgo_item,
                         sec_rgy_plcy_t *policy,
                         rgy_attribute_info_t *attr_info,
                         sec_attr_t *attrs)
{
    int                 i, j;
    int                 bad_value = -1;
    boolean32           list_list = FALSE;
    char                **pargv;
    int                 pargc = 0;
    char                **aargv;
    int                 aargc = 0;
    sec_rgy_pgo_item_t  *pgo_item = attr_info->pgo_item;
    error_status_t      tmp_status;
    unsigned32          result = error_status_ok;

    if (attr_list != NULL) {
        list_list = isllist(attr_list);
        if (list_list) {
            if (Tcl_SplitList(interp, attr_list, &pargc, &pargv) != TCL_OK)
                return TCL_ERROR;
        }
        else {
            pargc = 1;
            pargv = (char **)malloc(2 * sizeof(char *));
            if (pargv == NULL) {
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }
            pargv[0] = attr_list;
            pargv[1] = NULL;
        }
        for (i = 0; i < pargc; i++) {
            if (Tcl_SplitList(interp, pargv[i], &aargc, &aargv) != TCL_OK) {
                free((char *)pargv);
                return TCL_ERROR;
            }
            if (pgo_item != NULL) {
                encode_rgy_pgo_item_attr(interp,
                                         aargv,
                                         &aargc,
                                         pgo_item,
                                         attr_info,
                                         attrs,
                                         &bad_value,
                                         &result);
            }
            if (policy != NULL) {
                if (attr_info->domain == sec_rgy_domain_org && result == error_status_ok)
                    encode_rgy_plcy_attr(aargv[0],
                                         aargv[1],
                                         policy,
                                         attr_info, 
                                         &result);
            }
            if (result != error_status_ok) {
                free((char *)pargv);
                if (result == dcp_s_rgy_illegal_attr) {
                    DCP_SET_SEC_ERROR(result, aargv[0]);
                }
                else if (result == dcp_s_rgy_quota_out_of_range ||
                         result == dcp_s_rgy_uid_out_of_range ||
                         result == dcp_s_rgy_pwdminlen_out_of_range ||
                         result == dcp_s_rgy_era_out_of_range ||
                         result == dcp_s_minorgid_out_of_range ||
                         result == dcp_s_maxuid_out_of_range ||
                         result == dcp_s_minuid_out_of_range ||
                         result == dcp_s_mingid_out_of_range){
                    free((char *)aargv);
                    DCP_SET_SEC_ERROR(result, MAXINT);
                }
                else if (result == sec_attr_sch_entry_not_found) {
                    if (bad_value == -1) {
                        DCP_SET_SEC_ERROR(dcp_s_rgy_sch_entry_not_found, aargv[0]);
                    }
                    else {
                        DCP_SET_SEC_ERROR(dcp_s_rgy_sch_entry_not_found, aargv[bad_value]);
                    }
                }
                free((char *)aargv);
                DCP_SET_MISC_ERROR(result);
            }
            free((char *)aargv);
        }
        free((char *)pargv);

        /*
         * If there are era records to process, check for multiple instances
         * of the 'any' attribute being sent in as an encoding type void.  Also
         * check for multiple instances (in different records) of stringarray's
         * and attrset's.
         */
        if (attr_info->era_count > 1) {
            for (i = 0; i < attr_info->era_count; i++) {
                if (attrs[i].attr_value.attr_encoding == sec_attr_enc_void) {
                    for (j = i+1; j < attr_info->era_count; j++) {
                        if ((uuid_equal(&attrs[i].attr_id, &attrs[j].attr_id, &tmp_status)) &&
                            (attrs[j].attr_value.attr_encoding == sec_attr_enc_void)) {
                            DCP_SET_MISC_ERROR(dcp_s_rgy_era_too_many_voids);
                        }
                    }
                }
                if ((attrs[i].attr_value.attr_encoding == sec_attr_enc_printstring_array) ||
                    (attrs[i].attr_value.attr_encoding == sec_attr_enc_attr_set)) {
                    for (j = i+1; j < attr_info->era_count; j++) {
                        if ((uuid_equal(&attrs[i].attr_id, &attrs[j].attr_id, &tmp_status)) &&
                            (attrs[j].attr_value.attr_encoding == attrs[i].attr_value.attr_encoding)) {
                            DCP_SET_MISC_ERROR(dcp_s_rgy_era_illegal_attribute_list);
                        }
                    }
                }
            }
        }
    }
    return TCL_OK;
}


/*
 * FUNCTION: parse_item_switch_list
 *
 * OVERVIEW: This encodes ascii values into a pgo_item  structure according 
 *            to the variable name. That is fullname is encoded into 
 *            the policy structure as the pgo fullname. 
 *
 * INPUTS:
 *  char *quota - object creation quota integer/"unlimited"
 *  char *unix_num - pgo uid value (unix_num) integer
 *  char *fullname - fullname value ascii.
 *  char *uuid - object uuid ascii uuid value. 
 *  char *alias - Ascii yes/no wether or not pgo is an alias
 *  char *inprojlist - Ascii yes/no wether or not project list allowed,
 *  sec_rgy_domain_t domain - domain of pgo_item.
 * 
 * INPUT/OUTPUTS:
 *  sec_rgy_pgo_item_t *pgo_item - pgo item structure to be filed with encode.
 *  
 * OUTPUTS:
 *   unsigned32 *result - error/success code.
 *
 * RETURNS:
 *  void
 */
void parse_item_switch_list(char *quota,
                            char *fullname,
                            char *uuid, 
                            char *unix_num,
                            char *alias,
                            char *inprojlist,
                            sec_rgy_pgo_item_t *bad_pgo_item, 
                            sec_rgy_domain_t domain,
                            rgy_attribute_info_t *attr_info,
                            unsigned32 *result)
{
    sec_rgy_pgo_item_t  *pgo_item = attr_info->pgo_item;

    /* 
     * Add the fields to the pgo_item. 
     */
    if (quota != NULL) {
        if (domain != sec_rgy_domain_person) {
            *result = dcp_s_rgy_invalid_attribute;
            return;
        }
        *result = mod_rgy_quota(quota, pgo_item);
        if (*result != error_status_ok)
            return;
    }
    if (fullname != NULL) {
        strcpy((char *)pgo_item->fullname, (char *)fullname);
        attr_info->fullname = dce_strdup((char *)fullname);
    }
    if (uuid == NULL) {
        uuid_create_nil(&(pgo_item->id), result);
        if (*result != error_status_ok)
            return;
    }
    else {
        uuid_from_string((unsigned_char_t *)uuid, &(pgo_item->id), result);
        if (*result != error_status_ok)
            return;
    }
    if (unix_num != NULL) {
        *result = add_uid(unix_num,pgo_item);
        if (*result == dcp_s_rgy_uid_is_wrong) {
            switch ((int)domain) {
              case sec_rgy_domain_group:
                *result = dcp_s_rgy_gid_is_wrong;
                break;
              case sec_rgy_domain_org:
                *result = dcp_s_rgy_orgid_is_wrong;
                break;
            }
        } else if (*result == dcp_s_rgy_uid_is_wrong) {
            switch ((int)domain) {
              case sec_rgy_domain_group:
                *result = dcp_s_rgy_gid_is_wrong;
                break;
              case sec_rgy_domain_org:
                *result = dcp_s_rgy_orgid_is_wrong;
                break;
            }
        }
        if (*result != error_status_ok)
            return;
    }
    if (alias != NULL) {
        if (domain == sec_rgy_domain_org) {
            *result = dcp_s_rgy_invalid_attribute;
            return;
        }
        *result = add_pgo_flag(alias, pgo_item, sec_rgy_pgo_is_an_alias);
        if (*result != error_status_ok)
            return;
        pgo_item->flags|=rgy_pgo_alias_found;
    }
    if (inprojlist != NULL) {
        if (domain != sec_rgy_domain_group) {
            *result = dcp_s_rgy_invalid_attribute;
            return;
        }
        *result = add_pgo_flag(inprojlist, pgo_item, sec_rgy_pgo_projlist_ok);
        if (*result != error_status_ok)
            return;
        pgo_item->flags |= rgy_pgo_inprojlist_found;
    }
}


/*
 * FUNCTION: encode_rgy_plcy_attr
 *
 * OVERVIEW: This encode the policy structure given an ascii tcl list entry.
 *
 * INPUTS:
 *  char *attr_name - the name of the attribute to be encoded.
 *  char *attr - the ascii value of the attribute. 
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_plcy_t *policy - policy structure to hold the encoded entry. 
 *      
 * OUTPUTS:
 *  boolean32 *flag - TRUE/FALSE value whether or not an attribute was encoded.
 *  unsigned32 *status - error/success code. 
 * RETURNS:
 *  void
 */
void encode_rgy_plcy_attr(char *attr_name,
                          char *attr,
                          sec_rgy_plcy_t *policy, 
                          rgy_attribute_info_t *attr_info,
                          unsigned32 *status)
{
    unsigned32                 result = sec_rgy_status_ok;
    sec_attr_component_name_t  schema_name = NULL;
    sec_attr_schema_entry_t    schema_entry;

    switch (rgy_attribute(attr_name)) {
      case rgy_plcy_pwdlife:
        if (!attr) {
            *status = dcp_s_rgy_bad_pwdlife_attr_val;
            return;
        }
        attr_info->policy = TRUE;
        result = add_pwdlife( attr, policy);
        if (result != error_status_ok) {
            *status = result;
            return;
        }
        break;
      case rgy_plcy_pwdminlen:
        if (!attr) {
            *status = dcp_s_rgy_bad_pwdminlen_attr_val;
            return;
        }
        attr_info->policy = TRUE;
        result = add_pwdminlen( attr, policy);
        if (result != error_status_ok) {
            *status = result;
            return;
        }
        break;
      case rgy_plcy_pwdexpdate:
        if (!attr) {
            *status = dcp_s_rgy_bad_pwdexpdate_attr_val;
            return;
        }
        result = add_pwdexpdate(attr, policy);
        if (result != error_status_ok) {
            *status = result;
            return;
        }
        attr_info->policy = TRUE;
        break;
      case rgy_plcy_pwdspaces:
        if (!attr) {
            *status = dcp_s_rgy_bad_pwdspaces_attr_val;
            return;
        }
        result = add_policy_flag(attr, policy, sec_rgy_plcy_pwd_no_spaces);
        if (result != error_status_ok) {
            *status = result;
            return;
        }
        policy->passwd_flags|=rgy_plcy_spaces_found;
        attr_info->policy = TRUE;
        break;
      case rgy_plcy_pwdalpha:
        if (!attr) {
            *status = dcp_s_rgy_bad_pwdalpha_attr_val;
            return;
        }
        result = add_policy_flag(attr, policy, sec_rgy_plcy_pwd_non_alpha);
        if (result != error_status_ok) {
            *status = result;
            return;
        }
        policy->passwd_flags |= rgy_plcy_alpha_found;
        attr_info->policy = TRUE;
        break; 
      case rgy_plcy_acctlife: 
        if (!attr) {
            *status = dcp_s_rgy_bad_acctlife_attr_val;
            return;
        }
        result = add_acctlife(attr, policy);
        if (result != error_status_ok) {
            *status = result;
            return;
        }
        attr_info->policy = TRUE;
        break;
      case rgy_pgo_uuid:
      case rgy_pgo_unix_num:
      case rgy_pgo_fullname:
        if (attr_info->domain != sec_rgy_domain_org) {
            *status = dcp_s_rgy_illegal_attr;
            return;
        }
        break;
      default:
         /*
          * One of the usual hard-coded policy attribute names was
          * not specified, nor was one of the legal org attribute
          * names.  If the specified name is a legal extended
          * attribute name, then it should have been already
          * processed, so we can just ignore it here.  If it is
          * not a legal extended attribute, then we should report
          * back an illegal attribute message.
          */
         sec_rgy_attr_sch_lookup_by_name(rgy_context,
                                         schema_name,
                                         (idl_char *)attr_name,
                                         &schema_entry,
                                         &result);
         if (result != sec_rgy_status_ok) {
            /*
             * The attribute name is unknown, so tell the user and exit.
             */
            *status = result;
            return;
         }
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         break;
    }
    *status = sec_rgy_status_ok;
    return;
}


/*
 * FUNCTION: parse_policy_switch_list()
 *
 * OVERVIEW: The encodes ascii values into a policy structure according 
 *            to the variable name. That is pwdminlen is encoded into 
 *            the policy structure as the passwd minimum length. 
 *
 * INPUTS:
 *  acctlife -  Ascii equivalent of account lifespan. integer/"unlimited"
 *  pwdalpha -  Ascii yes/no wether or not passwds can be alphanumeric.
 *  pwdexpdate -Ascii date representing passwd expiration 
 *  pwdlife -   Ascii lifespan of passwd integer/"unlimited"
 *  pwdminlen - Ascii minimum length of passwd integer
 *  pwdspaces - Ascii Yes/no wether or not passwds can contain spaces. 
 *
 * INPUT/OUTPUTS:
 *  sec_rgy_plcy_t *policy - The policy structure that hold the encoded results.
 *
 * OUTPUTS:
 *  unsigned32 *status - the resulting error or success code. 
 *
 * RETURNS:
 *  void
 */
void parse_policy_switch_list(char *acctlife,
                              char *pwdalpha,
                              char *pwdexpdate,
                              char *pwdlife,
                              char *pwdminlen,
                              char *pwdspaces, 
                              sec_rgy_plcy_t *policy,
                              unsigned32 *status)
{
    unsigned32  result = error_status_ok;

    if (pwdalpha != NULL) {
        result = add_policy_flag(pwdalpha, policy, sec_rgy_plcy_pwd_non_alpha);
        if (result != error_status_ok) {
            *status = result;
            return;
        }
        policy->passwd_flags |= rgy_plcy_alpha_found;
    }
    if (pwdminlen != NULL) {
        result = add_pwdminlen(pwdminlen , policy);
        if (result != error_status_ok) {
            *status = result;
            return;
        }
    }
    if (pwdspaces != NULL) {
        result = add_policy_flag(pwdspaces, policy, sec_rgy_plcy_pwd_no_spaces);
        if (result != error_status_ok) {
            *status = result;
            return;
        }
        policy->passwd_flags |= rgy_plcy_spaces_found;
    }
    /* modify passwd expiration date in org policy */
    if (pwdexpdate != NULL) {
        result = add_pwdexpdate(pwdexpdate, policy);
        if (result != error_status_ok) {
            *status = result;
            return;
        }
    }
    /* modify account lifespan in org policy */
    if (acctlife != NULL) {
        result = add_acctlife(acctlife, policy);
        if (result != error_status_ok) {
            *status = result;
            return;
        }
    }
    if (pwdlife != NULL) {
       result = add_pwdlife(pwdlife, policy);
        if (result != error_status_ok) {
            *status = result;
            return;
        }
    }
}


/*
 * FUNCTION: check_uid_validity()
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
int check_uid_validity(sec_rgy_domain_t valid_domain,
                       unsigned32 code,
                       char *name)
{
    char  *key_uid_svc;
    char  *uid_svc;
    char  *gid_svc;
    char  *orgid_svc;
    int   result = sec_rgy_status_ok;

    key_uid_svc = (char *)dce_sprintf(code);
    uid_svc = (char *)dce_sprintf(dcp_t_rgy_uid);
    gid_svc = (char *)dce_sprintf(dcp_t_rgy_gid);
    orgid_svc = (char *)dce_sprintf(dcp_t_rgy_orgid);

    if (!uid_svc || !key_uid_svc || !gid_svc || !orgid_svc)
        return dcp_s_no_memory;

    if (strcmp(key_uid_svc, name)) {
        switch ((int)code) {
          case dcp_t_rgy_uid:
            if (!strcmp(name, gid_svc))
                result = dcp_s_gid_not_allowed;
            else if (!strcmp(name,orgid_svc))
                result = dcp_s_orgid_not_allowed;
            break;
          case dcp_t_rgy_gid:
            if (!strcmp(name, uid_svc))
                result = dcp_s_uid_not_allowed;
            else if (!strcmp(name, orgid_svc))
                result = dcp_s_orgid_not_allowed;
            break;
          case dcp_t_rgy_orgid:
            if (!strcmp(name, uid_svc))
                result = dcp_s_uid_not_allowed;
            else if (!strcmp(name, gid_svc))
                result = dcp_s_gid_not_allowed;
            break;
        }
    }
    free(gid_svc);
    free(orgid_svc);
    free(uid_svc);
    free(key_uid_svc); 
    return result;
}
