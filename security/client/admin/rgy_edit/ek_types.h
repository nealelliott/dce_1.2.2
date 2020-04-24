/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ek_types.h,v $
 * Revision 1.1.6.2  1996/03/11  13:26:59  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:55  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:00:08  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/10/20  18:50 UTC  burati
 * 	Merge fix for CHFts15386 from KK_FIX
 * 	[1995/12/08  16:30:15  root]
 * 
 * Revision 1.1.4.2  1994/09/09  19:11:59  mob
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/09  00:28:22  mob]
 * 
 * Revision 1.1.4.1  1994/03/02  17:54:21  hanfei
 * 	Initial ERA drop
 * 	[1994/02/24  21:08:46  hanfei]
 * 
 * Revision 1.1.2.2  1992/12/29  12:40:30  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:12:28  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:46:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  ek_types.h
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit kernel - operation type definitions
** 
*/

#ifndef ek_types_included
#define ek_types_included

/* Special Field "Types" */

/* FLAG field definitions */

#define ek_flag_on    "on"
#define ek_flag_off   "off"
#define ek_lim_max_flag_len  3      /* allocate at least this many chars to hold 
                                     * a flag value
                                     */

/* CHECK_FLAG expects a valid flag string and a length.  Expands to true if
 * the flag is on, false otherwise.
 */
#define CHECK_FLAG(flag, flag_len) \
    ( (strncmp((flag), ek_flag_on, (flag_len)) == 0)  && \
      ((flag_len) == strlen(ek_flag_on)) )

/*
 * For instance:
 *     char alias_buff[ekn_flag_max_len];
 *     ek_read_field (get_person_by_name_handle, ekn_pgo_is_an_alias,
 *                     alias_buff, len);
 *     if (CHECK_FLAG(alias_buff) {
 *          ... do whatever you do when the alias flag is on
 *     )
 *
 * if you  attempt an ek_write_field on a flag field with anything but the 
 * ekn_flag_on or ekn_flag_off values as the data parameter, you'll get back 
 * ek_bad_format
 */
  
/* PGO automatic uid generation.
 * For conversion tables and/or validation routines
 */
#define ek_pgo_unknown_uid      "-1"

/* DATE strings 
 *
 *The kernel expects date strings to be in the format [yy]yy/mm/dd
 * An ek_write_field operation on a date-time field that is not in this format
 * results causes an ek_err_bad_date return.  ek_read_field operations return 
 * date-time strings in this format with the hours and minutes added in  hh.mm
 * format.   There are special cases of a date-time string.  For instance, policy
 * data includes a password expiration date.  The kernel accepts a valid 
 * date-time string for this date, but also defines a constant 
 * ekn_policy_null_date that,  although it may not expand to a meaningful
 * date-time format as specified above, is still valid. Interfaces should always 
 * check return values for one of these  symbolic constants.

 * E.G.
 *   To give organization passwords an expiration date that is never in effect.
 *
 *    ek_write_field(policy_op_handle, ekn_policy_org_set, ek_null_date, 
 *                   strlen(ek_null_date));
 *
 * After reading a  pasword expiration date, the interface should always check
 * for the ek_policy_null_date value before attempting to treat the returned
 * string as a meaningful date-time string. 
 */

#define ek_lim_max_date_len       16  /* max length of an input date string */
#define ek_lim_max_date_time_len  16  /* max length of an output date/time
                                       * string
                                       */
#define ek_null_date             "0"  /* use this for assigning/checking-for 
                                       *  expiration dates that never take
                                       * effect
                                       */

/* LIFESPANS
 *
 * Lifespans are are some  number of days.  Use the ekn_policy_live_forever
 *constant below to assign/check-for infinite lifespans.
 */
#define ek_max_lifespan     366   /* max days in a lifespan */
#define ek_min_lifespan     1     /* the other end of the range */

#define ek_lifespan_infinite  "0"  /* use this for specifying infinite
                                    * lifespans (e.g. accounts and passwords) 
                                    */


/* KEY PARTS 
 * An account name may have an abbreviation.  The abbreviation is specified as
 *just the person, person and group, or person, group and org portions of the 
 * the login name.  There are symbolic strings constants defined for these
 * fields as follows:
 */
#define ek_acct_key_none   "n"
#define ek_acct_key_person "p"
#define ek_acct_key_group  "g"
#define ek_acct_key_org    "o"

#define ek_lim_max_key_parts_len 1  /* maximum length of a key parts constant */

/* PASSWORDS
 *
 * passwords may be assigned to groups, organizations, and accounts.  Passwords
 * are written to the kernel in plain text.  Passwords read from the kernel are
 * always encrypted (unless you're reading a plain text passwd field that you
 * already written a value to).  The symbolic constant ek_passwd_none represents
 * no passwd.  
 */

#define ek_passwd_none ""                /* no password */
#define ek_lim_max_plain_passwd_len 512  /* maximum length of plain text passwd */
#define ek_lim_max_enc_passwd_len  14    /* maximum length of a displayable 
                                          * encrypted password
                                          */

/* LIMITS */

#define ek_lim_max_name_len        1024    /* maximum length of a pgo name */
#define ek_lim_max_unix_name_len   8       /* ditto, but with unix restrictions */
#define ek_lim_max_login_name_len  1024    /* maximum length of a login name */
#define ek_lim_max_fullname_len    256     /* maximum length of pgo full names*/
#define ek_lim_max_unix_num_len     10     /* a unix number string contains 10
                                            * decimal digits, max
                                            */

#define ek_lim_max_uuid_len         38      /* a complete UUID string */
#define ek_lim_max_int32_len        11      /* max digits in a long integer
                                             * (including potential minus sign)*/

#define ek_lim_max_pname_len       256

/* These are range constraints - don't use them to allocate buffer space */

#define ek_max_unix_num     4294967295      /* maximum allowable unix num */
#define ek_min_unix_num     0               /* minimum allowable unix num */


/* BUFFER declarations

   Use the following types to declare buffers large enough to hold the largest
   legal null-terminated string for a kernel field.
*/
typedef char   ek_name_buf_t[ek_lim_max_name_len + 1];
typedef char   ek_login_name_buf_t[ek_lim_max_login_name_len + 1];
typedef char   ek_fullname_buf_t[ek_lim_max_fullname_len + 1];
typedef char   ek_enc_passwd_buf_t[ek_lim_max_enc_passwd_len + 1];
typedef char   ek_plain_passwd_buf_t[ek_lim_max_plain_passwd_len + 1];
typedef char   ek_uuid_buf_t[ek_lim_max_uuid_len + 1];
typedef char   ek_unix_num_buf_t[ek_lim_max_unix_num_len + 1];
typedef char   ek_date_buf_t[ek_lim_max_date_time_len + 1];
typedef char   ek_date_time_buf_t[ek_lim_max_date_time_len + 1];
typedef char   ek_integer32_buf_t[ek_lim_max_int32_len + 1];
typedef char   ek_pname_buf_t[ek_lim_max_pname_len + 1];
typedef char   ek_flag_buf_t[ek_lim_max_flag_len + 1];
typedef char   ek_key_parts_buf_t[ek_lim_max_key_parts_len + 1];
typedef char   ek_quota_buf_t[ek_lim_max_int32_len + 1];

#endif
