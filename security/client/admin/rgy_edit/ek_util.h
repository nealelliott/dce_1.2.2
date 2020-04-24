/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ek_util.h,v $
 * Revision 1.1.8.2  1996/03/11  13:27:02  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:57  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:00:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:18  root]
 * 
 * Revision 1.1.6.3  1994/09/09  19:12:01  mob
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/09  00:31:44  mob]
 * 
 * Revision 1.1.6.2  1994/05/11  16:48:40  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  20:44:43  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 
 * 	hp_sec_to_osf_2 drop
 * 
 * Revision 1.1.2.2  1992/12/29  12:40:45  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:12:32  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:46:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit kernel utilities - PUBLIC include file
** 
*/


#ifndef ek_util_pub_included
#define ek_util_pub_included

#include <dce/nbase.h> 
#include <dce/id_base.h>
#include <dce/rgybase.h>


#include <form.h>

/********************************************************************************
 ******************************* GENERIC VALIDATION ROUTINES ********************
 ********************************************************************************
 *
 * Generic validation routines encapsulate validation of data that is common  
 * across operations on the data.  E.G., a pgo name always has to begin with a
 * lowercase letter, and contain only lowercase letters, numbers, and '_' 
 * regardless of where it is used (as a key in a pgo operation or a component 
 * of an account name).  Validation routines for a specific operation on a name
 * field can  apply more stringent requirements if necessary.  If not, they can 
 * simply call the generic routine.
 */
 
 
/* FUNCTION eku_chk_name
 * 
 * Given a character string and a string length, checks to see that the string begins
 * with a lower_case letter, contains only lowercase letters, numbers, and underscores,
 * and is within the allowable length for names (as defined by the ek_lim_max_name 
 * constant).  The routine assumes positive, non-zero string lengths.  
 *
 * returns:
 *    ek_err_person_bad_name_chars 
 *    ek_err_group_bad_name_chars 
 *    ek_err_org_bad_name_chars 
 *    ek_err_person_name_too_long 
 *    ek_err_group_name_too_long 
 *    ek_err_org_name_too_long 
 */
error_status_t eku_chk_name (
#ifdef __STDC__
     char              *name,
     sec_rgy_domain_t  domain
#endif
);

/*
 * FUNCTION eku_chk_flag
 * 
 * Checks that the input strings contains either ek_flag_on, or ek_flag_off.
 * Anything else raises an ek_bad_format error.  flag_set is true if the 
 * flag is on, false if it isn't, and undefined if the format is bad.
 */
error_status_t eku_chk_flag (
#ifdef __STDC__
     char      *flag,
     signed32  flaglen,
     boolean32 *flag_set
#endif
);


/* FUNCTION eku_chk_login_name
 *
 * Breaks a login name up into its component pgo names.  Expects the login name
 * to be in the format "person.group.org".  A wildcard character (%) in any of the
 * pgo components is allowed only if the wild_ok parameter is true.  Validates each 
 * of the components of the login name by calling eku_chk_name.
 * 
 * Assumes the pgo name parameters are large enough for legal pgo names.  Does
 * not null-terminate return values.  If any errors are encountered, the return
 * values are undefined.
 *
 * returns:
 *    ek_err_login_name_too_long - if the login name is too long
 *    ek_err_no_wildcards         - if login name contains a wilcard and
 wildcards aren't allowed
 *    ek_err_bad_format          - if the login string isn't in "person.group.org"
 *    ek_err_person_bad_name_chars 
 *    ek_err_group_bad_name_chars 
 *    ek_err_org_bad_name_chars 
 *    ek_err_person_name_too_long 
 *    ek_err_group_name_too_long 
 *    ek_err_org_name_too_long 
 */ 
 
error_status_t eku_chk_login_name ( 
#ifdef __STDC__
    char         lname[],          /* IN - the login name */
    boolean32    wild_ok,          /* IN - are wildcards ok? */
    char         pname[],          /* OUT - the person part */
    char         gname[],          /* OUT */
    char         oname[]           /* OUT */
#endif
);

	    
	    



/*
 * FUNCTION eku_chk_unix_num
 *
 * Given a string representation of an unix number, checks to see if the 
 * string contains a valid unix number (contains only integer digits, and is
 * within the allowable range for unix numbers.  Returns true if the number
 * is valid. false otherwise.  (may need to expand this to a error_status_t 
 * sometime).
 * Returns a converted unix number if the input string is valid
 */
boolean32 eku_chk_unix_num ( 
#ifdef __STDC__
    char     *unum_str,
    signed32 unum_strlen,
    signed32 *unum_ptr
#endif
);


/*
 * FUNCTION eku_chk_uuid
 *
 * Returns true if the uuid string is valid, false otherwise.
 * If the uuid string is valid, uuid_p contains
 * the converted uuid, otherwise its trash.
 */
boolean32 eku_chk_uuid ( 
#ifdef __STDC__
    char     *uuid_str,
    uuid_t   *uuid_p
#endif
);



/* FUNCTION eku_chk_plain_passwd
 *
 *  Checks an unecrypted passwd for validity
 */
error_status_t eku_chk_plain_passwd (
#ifdef __STDC__
    char            *passwd_text,
    signed32        passwd_textlen,
    sec_rgy_plcy_t  policy,
    boolean32       check_policy
#endif
);
 	


/* FUNCTION eku_chk_date
 * 
 * dates have to be in the form [yy]yy/mm/dd (the century is optional).  
 * Legal dates range from 1980/01/01 to 2021/12/31.  If the date
 * string is syntactically legal and within range, it is converted to
 * a sec_timeval_sec_t value and returned in the sec parameter.
 *
 * returns:
 *  ek_err_bad_date          - if the date can't be deciphered.
 *  ek_err_date_out_of_range - if the date is outside the legal range
 */
error_status_t eku_chk_date ( 
#ifdef __STDC__
    char               *date,
    signed32           date_len,
    sec_timeval_sec_t  *sec
#endif
);


/* FUNCTION eku_chk_lifespan
 * 
 * Checks that the lifespan is either ek_lifespan_infinite or an integer in
 * the range ek_min_lifespan <= (lifespan) <= ek_max_lifespan.
 * 
 * returns:
 *  ek_err_bad_lifespan          - if the lifespan isn't a decimal integer.
 *  ek_err_lifespan_out_of_range - if the lifespan is outside the legal range
 */
error_status_t eku_chk_lifespan ( 
#ifdef __STDC__
    char                  *lifespan_str,
    signed32              lifespan_len,
    sec_timeval_period_t  *lifespan_p
#endif
);


/* FUNCTION eku_chk_lifespan_days
 * 
 * Checks that the lifespan is either ek_lifespan_infinite or an integer in
 * the range ek_min_lifespan <= (lifespan) <= ek_max_lifespan.
 * 
 * returns:
 *  ek_err_bad_lifespan          - if the lifespan isn't a decimal integer.
 *  ek_err_lifespan_out_of_range - if the lifespan is outside the legal range
 */
error_status_t eku_chk_lifespan_days (
#ifdef __STDC__
    char                  *lifespan_str,
    signed32              lifespan_len,
    sec_timeval_period_t  *lifespan_p
#endif
);


/* FUNCTION eku_chk_lifespan_hours  (convert hours to clock units)
 * 
 * returns:
 *  ek_err_bad_lifespan          - if the lifespan isn't a decimal integer.
 *  ek_err_lifespan_out_of_range - if the lifespan is outside the legal range
 */
error_status_t eku_chk_lifespan_hours (
#ifdef __STDC__
    char                  *lifespan_str,
    signed32              lifespan_len,
    sec_timeval_period_t  *lifespan_p
#endif
);


/* FUNCTION eku_chk_lifespan_minutes  (convert minutes to clock units)
 * 
 * returns:
 *  ek_err_bad_lifespan          - if the lifespan isn't a decimal integer.
 *  ek_err_lifespan_out_of_range - if the lifespan is outside the legal range
 */
error_status_t eku_chk_lifespan_minutes (
#ifdef __STDC__
    char                  *lifespan_str,
    signed32              lifespan_len,
    sec_timeval_period_t  *lifespan_p
#endif
);


/* FUNCTION eku_chk_integer32
 *
 * checks that the integer string is within the limits for a 32 bit integer and
 * does not contain any non-numeric characters.  Returns converted integer in
 * int_p.
 *
 * returns
 *    ek_err_non_numeric    - if the integer string contains non-numeric chars.
 *    ek_err_int_out_range  - if the integer srtring is too large to be converted
 *                             to a 32 bit integer
 */	
error_status_t eku_chk_integer32 (
#ifdef __STDC__
    char  *int_str,
    signed32  int_len,
    signed32  *int_p
#endif
);

/********************************************************************************
 ******************************* GENERIC CONVERSION ROUTINES ********************
 ********************************************************************************
 *
 * Generic conversion routines are used to convert from strings to rgy_ types, 
 * and vice versa.  
 */



/* FUNCTION eku_dec_int32
 *
 * converts a 32 bit signed integer to a string and stores it in the specified
 * form field.
 */
void eku_dec_int32 (
#ifdef __STDC__
    form_t               *form,
    form_field_offset_t  field,
    signed32             int32
#endif
);


/* FUNCTION eku_dec_lifespan
 *
 * takes sec_timeval_period_t and converts it to lifespan units (#w#d#h#m#s)
 */
void eku_dec_lifespan (
#ifdef __STDC__
    form_t               *form,
    form_field_offset_t  field,
    sec_timeval_period_t clock_units
#endif
);

    
/*  
 *  FUNCTION eku_dec_name
 * 
 * Copies the data from a field into a sec_rgy_name_t parameter. Assumes the
 * validity of the field data has been checked prior to the call, so doesn't
 * perform any bounds checking.  If the data in the field is longer than the
 * name_len parameter, the rgy_name contains a truncated version of the field
 * data.
 */
void eku_dec_name ( 
#ifdef __STDC__
    form_t               *op,
    form_field_offset_t  name_field,
    sec_rgy_name_t       rgy_name,
    signed32             *name_len   /* IN - size of rgy_name buffer
					OUT - length of name in rgy_name */
#endif
);


/*
 * FUNCTION eku_enc_uuid
 *
 * Takes a string representation of a uuid and converts it to uuid_t.
 * The validity of the input string is assumed, as is null-termination.
 */
void eku_enc_uuid ( 
#ifdef __STDC__
    char   *uuid_str,
    uuid_t *uuid_ptr
#endif
);


/*  FUNCTION eku_dec_uuid 
 *
 * converts a uuid to a string and stores the result in the specified field.
*/
void eku_dec_uuid ( 
#ifdef __STDC__
    form_t               *form,
    form_field_offset_t  field_offset,
    uuid_t               uuid
#endif
);


/* 
 *  FUNCTION eku_enc_unix_num
 *
 * Reads a unix number string from the specified field and converts it to a 
 * long integer. Assumes the field has a valid unix number string in it.
 */
void eku_enc_unix_num ( 
#ifdef __STDC__
    form_t               *form, 
    form_field_offset_t  unum_field,
    signed32             *unum_ptr
#endif
);


/* 
 * FUNCTION eku_dec_unix_num
 *
 * Converts a unix number to a string and stores the result in the specified
 * field buffer 
 */
void eku_dec_unix_num ( 
#ifdef __STDC__
    form_t               *form, 
    form_field_offset_t  unum_field,
    signed32             unum
#endif
);


/*
 * FUNCTION eku_dec_pgo_flag
 *
 * converts from the sec_rgy_pgo_flags_t bitset format to the appropriate kernel 
 * flag  value (ek_flag_on or ek_flag_off), and stores the result in the 
 * specified field's buffer. 
 */
void eku_dec_pgo_flag ( 
#ifdef __STDC__
    form_t               *op,
    form_field_offset_t  flag_field,
    sec_rgy_pgo_flags_t      pgo_flags, 
    sec_rgy_pgo_flags_t      mask
#endif
);


/*
 * FUNCTION eku_dec_policy_pwd_flag
 *
 * converts from the sec_rgy_plcy_pwd_flags_t format to the appropriate kernel
 * flag 
 * value (ek_flag_on or ek_flag_off), and stores the result in the specified 
 * field. 
 */
void eku_dec_policy_pwd_flag ( 
#ifdef __STDC__
    form_t                 *form,
    form_field_offset_t    flag_field,
    sec_rgy_plcy_pwd_flags_t policy_pwd_flags, 
    sec_rgy_plcy_pwd_flags_t mask
#endif
);


/*
 * FUNCTION eku_dec_acct_user_flag
 *
 * converts from the sec_rgy_acct_user_flags_t  format to the appropriate kernel
 * flag  value (ek_flag_on or ek_flag_off), and stores the result in the 
 * specified field. 
 */
void eku_dec_acct_user_flag ( 
#ifdef __STDC__
    form_t                 *form,
    form_field_offset_t    flag_field,
    sec_rgy_acct_user_flags_t  acct_user_flags, 
    sec_rgy_acct_user_flags_t  mask
#endif
);


/* 
 * FUNCTION eku_enc_acct_user_flag
 *
 * reads the value stored in the indicated flag field and twiddles the bits of
 * the acct user flag bitset accordingly.  Expects the flag field to have 
 * valid flag string in it.  
*/
void eku_enc_acct_user_flag ( 
#ifdef __STDC__
    form_t                 *form,
    form_field_offset_t    flag_field,
    sec_rgy_acct_user_flags_t  *acct_user_flags, 
    sec_rgy_acct_user_flags_t  mask
#endif
);


/*
 * FUNCTION eku_dec_acct_admin_flag
 *
 * converts from the sec_rgy_acct_admin_flags_t  format to the appropriate kernel
 * flag  value (ek_flag_on or ek_flag_off), and stores the result in the
 * specified field. 
 */
void eku_dec_acct_admin_flag ( 
#ifdef __STDC__
    form_t                  *form,
    form_field_offset_t     flag_field,
    sec_rgy_acct_admin_flags_t  acct_admin_flags, 
    sec_rgy_acct_admin_flags_t  mask
#endif
);


/*
 * FUNCTION eku_enc_acct_admin_flag
 *
 * reads the value stored in the indicated flag field and twiddles the bits of
 * the acct admin flag bitset accordingly.  Expects the flag field to have 
 * valid flag string in it.  
 */
void eku_enc_acct_admin_flag ( 
#ifdef __STDC__
    form_t                  *form,
    form_field_offset_t     flag_field,
    sec_rgy_acct_admin_flags_t  *acct_admin_flags, 
    sec_rgy_acct_admin_flags_t  mask
#endif
);


/*
 * FUNCTION eku_dec_property_flag
 *
 * converts from the sec_rgy_properties_flags_t  format to the appropriate kernel
 * flag  value (ek_flag_on or ek_flag_off), and stores the result in the
 * specified field. 
 */
void eku_dec_property_flag ( 
#ifdef __STDC__
    form_t                  *form,
    form_field_offset_t     flag_field,
    sec_rgy_properties_flags_t  rgy_prop_flags, 
    sec_rgy_properties_flags_t  mask
#endif
);


/* 
 * FUNCTION eku_dec_rgy_sid
 *
 * converts a sec_rgy_sid_t value into a sec_rgy_login_name_t value, and
 * stores the result in the specified field buffer.
 */
void eku_dec_rgy_sid ( 
#ifdef __STDC__
    form_t               *form,
    form_field_offset_t  owner_field,
    sec_rgy_sid_t        sid
#endif 
);



/* 
 * FUNCTION eku_dec_foreign_id
 *
 * converts a sec_id_foreign_t into a compound string and
 * stores the result in the specified field buffer.
 */
void eku_dec_foreign_id ( 
#ifdef __STDC__
    form_t               *form,
    form_field_offset_t  owner_field,
    sec_rgy_foreign_id_t owner
#endif 
);



/* FUNCTION eku_dec_date_time
 * 
 * Converts  a date-time in sec_timeval_sec_t form into a date-time string and writes
    * the results to the specified field.
    */
void eku_dec_date_time ( 
#ifdef __STDC__
    form_t              *form,
    form_field_offset_t dt_field,
/*CLOCKH */ 
    sec_timeval_sec_t       clockh
#endif
);


/* 
 * FUNCTION eku_dec_bool32
 * 
 * Converts  a  form into a date-time string and writes
 * the results to the specified field.
 */
void eku_dec_bool32 ( 
#ifdef __STDC__
    form_t               *form,
    form_field_offset_t  dt_field,
    boolean32             bool32
#endif
);

/* FUNCTION eku_dec_acct_key_parts
 *
 *  converts a sec_rgy_acct_key_t parameter to the appropriate kernel representation
 *  of a key part and stores the results in the specified form field.
 */
void eku_dec_acct_key_parts (
#ifdef __STDC__
    form_t              *form,             
    form_field_offset_t field,
    sec_rgy_acct_key_t      key_parts
#endif
);

void eku_translate_status (
#ifdef __STDC__
    error_status_t  *stp
#endif
);

#endif /* multiple inclusion guard */
