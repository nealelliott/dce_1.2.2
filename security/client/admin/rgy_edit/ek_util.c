 /*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ek_util.c,v $
 * Revision 1.1.10.2  1996/03/11  13:27:00  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:56  marty]
 *
 * Revision 1.1.10.1  1995/12/08  18:00:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:16  root]
 * 
 * Revision 1.1.8.3  1994/09/09  19:12:00  mob
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/09  00:30:16  mob]
 * 
 * Revision 1.1.8.2  1994/08/10  14:09:54  melman
 * 	Add 1 to len of int32_str[] in eku_dec_int32()
 * 	[1994/08/10  14:09:41  melman]
 * 
 * Revision 1.1.8.1  1994/03/02  17:54:23  hanfei
 * 	Initial ERA drop
 * 	[1994/02/24  21:09:04  hanfei]
 * 
 * Revision 1.1.4.2  1992/12/29  12:40:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:35:53  zeliff]
 * 
 * Revision 1.1.2.4  1992/05/18  22:20:43  burati
 * 	CR3614 Don't allow control characters in PGO name.
 * 	CR3310 Fix checking against NULL date.
 * 	[1992/05/18  21:33:41  burati]
 * 
 * Revision 1.1.2.3  1992/04/28  14:24:07  burati
 * 	CR2726 eku_chk_name wasn't checking name[0] for invalid chars too.
 * 	[1992/04/28  04:11:31  burati]
 * 
 * Revision 1.1.2.2  1992/04/06  19:44:38  burati
 * 	Fix date parsing to use mktime instead of hand-constructing
 * 	and depending on kernel timezone value. [sommerfeld]
 * 	[1992/04/06  19:01:08  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  ek_util.c V=30 11/19/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
**      rgy_edit kernel utilities 
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)ek_util.c	30 - 11/19/91";
#endif
 
#include <ctype.h>
#include <stdio.h> 
#include <sys/time.h>
#include <time.h>
#include <string.h>

#include <un_time.h>
#include <un_math.h>
#include <un_strng.h>

#include <macros.h>

#include <dce/rgybase.h>
#include <dce/binding.h>
#include <dce/pgo.h>
#include <dce/rpc.h>
#include <dce/secidmap.h>

#include <ek_util.h>
#include <ek_error.h>
#include <ek_types.h>
#include <ek_glob.h>

/* External context handle set up in ek_site.c */
extern sec_rgy_handle_t    context;

/********************************************************************************
******************************* GENERIC FORMAT ROUTINES *************************
*********************************************************************************
* Generic format routines are used to enforce the kernel's format requirements
* (which really don't amount to much).  Many data fields are just strings and
* require no format checks (e.g. names).  For other fields, the kernel requires
* that the strings contain specific characters, or sets of characters.  Format
* checks are performed whenever  ek_write_field is called.  Format checks are
* "boolean" and are like a type mismatch in a typed language.
*/

/*
 * FUNCTION is_decimal_int
 *
 * checks that all characters in a string are decimal .
 */
PRIVATE boolean32 is_decimal_int
#ifndef __STDC__
    ( int_str, len ) 
    char      *int_str;
    signed32  len;
#else
  (
    char      *int_str,
    signed32  len
  )
#endif
{
    boolean32  status;
    int      cur_digit;

    status = true;
    cur_digit = 0;       /* start with first character in int_str */

   if ((*int_str == '-') || (*int_str == '+'))
        cur_digit++;     /* skip leading sign */

    for (; cur_digit < len && status == true; cur_digit++) 
        if (!isascii(int_str[cur_digit]))
            status = false;
        else if (!isdigit(int_str[cur_digit]))
            status = false;

    return status;
}


/*OBSOLETE - this function is obsolete cause we treat login names as 
 * three separate parameters at the kernel interface.  Just in case we
 * change our mind someday(and decide to use a single string again, we
 * leave the routine here a documentation
 */
#ifdef NOTDEF
/* FUNCTION login_name_format
 * 
 * returns a boolean32 indicating whether or not the input string is in the
 * form "A.B.C" where A, B and C are arbitrary strings of at last length one.
 * Does  not perform any validation of A, B or C other than a check to for
 * non-zero length.  Assumes that the login_name  parameter is 
 * <= ek_lim_max_login_name_len.
 */
PRIVATE boolean32 login_name_format
#ifndef __STDC__
    (login_name) 
    char     *login_name;
#else
  (
    char     *login_name
  )
#endif
{
    char       local_login_name[ek_lim_max_login_name_len + 1];
    char       *component_ptr, *dot_ptr;
    boolean32  valid;
    int        cur_component;   /* for looping through login_name_components */

    if (strlen(login_name) > ek_lim_max_login_name_len)
        return false;

    strcpy (local_login_name, login_name);

    component_ptr = local_login_name;
    valid = true;
    cur_component = 0;
    do {
        dot_ptr = strchr(component_ptr, '.');   /* look for a dot */
        if (!dot_ptr) {
            if (cur_component < 2)             /* less than two dots is bad */
                valid = false;
        } else {
            if (cur_component > 1)            /* more than two dots is bad*/
                valid = false;
            else 
                *dot_ptr = '\0';
        }
        
        if (valid) {
            if (strlen(component_ptr) == 0)
                valid = false;         /* zero length components are no good */
            else {
                component_ptr = dot_ptr + 1;
                cur_component++;
            }
        }
    } while (valid && cur_component <= 2);
    
    return valid;
}
#endif 

/********************************************************************************
 ******************************* GENERIC VALIDATION ROUTINES ********************
 ********************************************************************************
 *
 * Generic validation routines encapsulate validation of data that is common 
 * across  operations on the data.  E.G., a pgo name has to contain only letters,
 * numbers, and '_' regardless of where it is used.  Validation routines for a
 * specific operation on a name field can  apply more stringent requirements if
 * necessary.  If not, they can simply call the generic routine.
 */


PRIVATE  boolean32 is_special_name_char
#ifndef __STDC__
    ( c ) 
    int  c;
#else
  (
    int  c
  )
#endif
{
    static  char illegal_chars[] = {':',   /* colon would screw up passwd file */
                                    '@'
                                   };
    char   *cp;
    int    i;

    for (i = 0, cp = illegal_chars; i < sizeof(illegal_chars); i++, cp++) {
        if ((char)c == *cp) {
            return false;
        }
    }

    return true;
}

 
/*
 * FUNCTION eku_chk_name
 * 
 * Given a character string and a string length, checks to see that the string
 * contains only letters, numbers,and underscores, and is within the allowable
 * length for names (as defined by the ek_lim_max_name  constant). The routine
 * assumes positive, non-zero string lengths.  
 *
 * returns:
 *    ek_err_person_bad_name_chars 
 *    ek_err_group_bad_name_chars 
 *    ek_err_org_bad_name_chars 
 *    ek_err_person_name_too_long 
 *    ek_err_group_name_too_long 
 *    ek_err_org_name_too_long 
 */
error_status_t eku_chk_name
#ifndef __STDC__
    ( name, domain ) 
     char              *name;
     sec_rgy_domain_t  domain;
#else
  (
     char              *name,
     sec_rgy_domain_t  domain
  )
#endif
{
    int             i, namelen;
    error_status_t  st;
    boolean32       bad_chars;

    SET_STATUS(&st, error_status_ok);
    namelen = strlen(name);

    if (strlen(name) < 1) {         
        switch(domain) {

        case sec_rgy_domain_person:
            SET_STATUS(&st, ek_err_person_bad_name_chars);
            break;

        case sec_rgy_domain_group:
            SET_STATUS(&st, ek_err_group_bad_name_chars);
            break;

        case sec_rgy_domain_org:
            SET_STATUS(&st, ek_err_org_bad_name_chars);
            break;

        default:   /* sanity check */
            SET_STATUS(&st, ek_kernel_failure);
            break;
        }

        return st;
    }
        /* names have a maximum length */
    if (namelen > ek_lim_max_name_len) {
        switch(domain) {
            
        case sec_rgy_domain_person:
            SET_STATUS(&st, ek_err_person_name_too_long);
            break;
            
        case sec_rgy_domain_group:
            SET_STATUS(&st, ek_err_group_name_too_long);
            break;
            
        case sec_rgy_domain_org:
            SET_STATUS(&st, ek_err_org_name_too_long);
            break;
            
        default:   /* sanity check */
            SET_STATUS(&st, ek_kernel_failure);
            break;
        }
    }

    if (BAD_STATUS(&st))
        return st;
    
    bad_chars = false;

    for (i = 0; i < namelen && GOOD_STATUS(&st); i++) {
        if (!(isascii(name[i])) || iscntrl(name[i])) {
            bad_chars = true;
        } else {
            if (!( isalpha(name[i]) || isdigit(name[i]) ||
                   is_special_name_char((int) name[i]) ))
                bad_chars = true;
        }
    }

    if (bad_chars) {
        switch(domain) {

        case sec_rgy_domain_person:
            SET_STATUS(&st, ek_err_person_bad_name_chars);
            break;

        case sec_rgy_domain_group:
            SET_STATUS(&st, ek_err_group_bad_name_chars);
            break;

        case sec_rgy_domain_org:
            SET_STATUS(&st, ek_err_org_bad_name_chars);
            break;

        default:   /* sanity check */
            SET_STATUS(&st, ek_kernel_failure);
            break;
        }
    }

    return st;
}
        

/*
 * FUNCTION eku_chk_flag
 * 
 * Checks that the input strings contains either ek_flag_on, or ek_flag_off.
 * Anything else raises an ek_bad_format error.
 */
error_status_t eku_chk_flag
#ifndef __STDC__
    ( flag, flaglen, flag_set ) 
     char       *flag;
     signed32   flaglen;
     boolean32  *flag_set;
#else
  (
     char       *flag,
     signed32   flaglen,
     boolean32  *flag_set
  )
#endif
{
    error_status_t st;
    
    SET_STATUS(&st, error_status_ok);
    
    if ( (strncmp(flag, ek_flag_on, flaglen) == 0) && 
          (strlen(ek_flag_on) == flaglen) ) {
        *flag_set = true;
    } else if ( (strncmp(flag, ek_flag_off, flaglen) == 0) && 
                 (strlen(ek_flag_off) == flaglen) ) {
        *flag_set = false;
    } else
        SET_STATUS(&st, ek_bad_format);
    
    return st;
} 


#ifdef NOTDEF

/*OBSOLETE - since we now accept login names as three distinct fields at
 *           the kernel interface.  But we leave the source code here 
 *           in case we change our minds again somewhere down the
 *           road
 */
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
error_status_t eku_chk_login_name
#ifndef __STDC__
    ( lname, wild_ok, pname, gname, oname ) 
    char         lname[];          /* IN - the login name */
    boolean32    wild_ok;          /* IN - are wildcards ok? */
    char         pname[];          /* OUT - the person part */
    char         gname[];          /* OUT */
    char         oname[];          /* OUT */
#else
  (
    char         lname[],          /* IN - the login name */
    boolean32    wild_ok,          /* IN - are wildcards ok? */
    char         pname[],          /* OUT - the person part */
    char         gname[],          /* OUT */
    char         oname[]
  )
#endif
{
    char              local_lname[ek_lim_max_login_name_len + 1];
    char              *namep, *dot_ptr;
    sec_rgy_domain_t  domain;
    error_status_t    st;
    char              *component_name[sec_rgy_domain_last];
         /* array of name pointers for looping */
    boolean32         done;

    /* if the login name is too long just bail out */
    if (strlen(lname) > ek_lim_max_login_name_len) {
        SET_STATUS(&st, ek_err_login_name_too_long);
        return st;
    }
    
    if (!login_name_format(lname)) {
        SET_STATUS(&st, ek_bad_format);
        return st;
    }

    /* use the pgo name and name length parameters as buffers for what follows */
    component_name[sec_rgy_domain_person]   = pname;
    component_name[sec_rgy_domain_group]    = gname;
    component_name[sec_rgy_domain_org]      = oname;

    SET_STATUS(&st, error_status_ok);
 
        /* make sure the login name is null-terminated */
    strcpy(local_lname, lname);
    
        /* we already know we've got three non-null strings separated by dots, so
           break out the component names and validate them */
    namep = local_lname;
    domain = sec_rgy_domain_person;
    while ( (GOOD_STATUS(&st)) && (domain < sec_rgy_domain_last)) {
        
        dot_ptr = strchr(namep, '.');      /* look for a dot (.) */
        if (dot_ptr) 
            *dot_ptr = '\0';          /* trash the dot with a null */
        
            /* check the validity of the current component name - wildcards are
               only acceptable if wild_ok is true */
        if (IS_WILD(namep)) {
            if (!wild_ok)
                SET_STATUS(&st, ek_err_no_wildcard);
        } else {
            st = eku_chk_name(namep, domain);
        }

        if (STATUS_EQUAL(&st, error_status_ok)) {
                /* if we're here then the current login name component is okay -
                   copy it into the current component buffer */
            strcpy(component_name[domain], namep);

            if (dot_ptr)             /* advance past the dot */
                namep = dot_ptr + 1;    
            else                     /* advance to end the last component */
                namep = namep + (strlen(component_name[domain]) + 1);  

            domain++;
        } 

    } /* while */

    return st;
}
#endif            


/*
 * FUNCTION eku_chk_unix_num
 *
 * Given a string representation of an unix number, checks to see if the 
 * string contains a valid unix number (contains only integer digits, and is
 * within the allowable range for unix numbers.  Returns true if the number
 * is valid. false otherwise.  (may need to expand this to a error_status_t
 * sometime).  Returns a converted unix number if the input string is valid
 */
boolean32 eku_chk_unix_num
#ifndef __STDC__
    ( unum_str, unum_strlen, unum_ptr ) 
    char      *unum_str;
    signed32  unum_strlen;
    signed32  *unum_ptr;
#else
  (
    char      *unum_str,
    signed32  unum_strlen,
    signed32  *unum_ptr
  )
#endif
{
    char       my_unum_str[ek_lim_max_unix_num_len + 1];
        /* big enough for a valid unum string */
    boolean32  valid_num;

    valid_num = true;

        /* maybe unum_str is too short or long? */
    if ((unum_strlen > ek_lim_max_unix_num_len) || (unum_strlen < 1))
        valid_num = false;
    else if (!is_decimal_int(unum_str, unum_strlen))
        /* or it's not an integer string */
        valid_num = false;
    else {       
        strncpy(my_unum_str, unum_str, (unum_strlen + 1));
        sscanf(my_unum_str, "%d", unum_ptr);
        /*  or it's not in the allowable range for unix numbers ? */
        if ((*unum_ptr < ek_min_unix_num) && (*unum_ptr > ek_max_unix_num))
            valid_num = false;
    }
    return valid_num;
}


/*UUID */
/*
 * FUNCTION eku_chk_uuid
 *
 */
boolean32 eku_chk_uuid
#ifndef __STDC__
    (uid_str, uid_p) 
    char      *uid_str;
    uuid_t    *uid_p;
#else
  (
    char      *uid_str,
    uuid_t    *uid_p
  )
#endif
{
    error_status_t  st;

    uuid_from_string((unsigned_char_t *) uid_str, uid_p, &st);
    return(STATUS_EQUAL(&st, error_status_ok));
}



/* FUNCTION eku_chk_plain_passwd
 *
 *  Checks an unecrypted passwd for validity
 */
error_status_t eku_chk_plain_passwd
#ifndef __STDC__
    ( passwd_text, passwd_textlen, policy, check_policy ) 
    char             *passwd_text;
    signed32         passwd_textlen;
    sec_rgy_plcy_t   policy;
    boolean32        check_policy;   /* apply policy checks? */
#else
  (
    char             *passwd_text,
    signed32         passwd_textlen,
    sec_rgy_plcy_t   policy,
    boolean32        check_policy
  )
#endif
{
    error_status_t   st;
    boolean32        all_spaces = true,   /* assume passwd is all spaces */
                     alpha_num  = true;   /* and that it's all alpha_numeric */
    int              i;

    SET_STATUS(&st, error_status_ok);

    if (passwd_textlen == 0) {
        SET_STATUS(&st, ek_err_passwd_too_short);
        return st;
    }

    if (passwd_textlen > ek_lim_max_plain_passwd_len) {
        SET_STATUS(&st, ek_err_passwd_too_long);
        return st;
    }
    
    if (check_policy) {

        if  (passwd_textlen < policy.passwd_min_len) {
            SET_STATUS(&st, ek_err_passwd_too_short);
            return st;
        }
        
        /* step through each character in the passwd_text looking for a non-space
         * or a non-alpha-numeric character and set the value of all_spaces
         * and alpha_num based on what we find
         */
        for (i = 0; i < passwd_textlen && (all_spaces || alpha_num);
             i++, passwd_text++ ) {
            if (!isascii(*passwd_text)) { 
                   /* not ascii ==> not blank or alpha-num */
                alpha_num = false;
                all_spaces = false;
            } else {                 
                if (!isalnum(*passwd_text)) {
                        /* not alpha-num but might be blank */
                    alpha_num = false;
                    if (!(*passwd_text == ' ')) /* nope, not a blank either */ 
                        all_spaces = false;
                } else                    /* is alpha-num ==> not a blank */       
                    all_spaces = false;
            }
        }

        if (((policy.passwd_flags & sec_rgy_plcy_pwd_non_alpha) != 0)
            && (alpha_num))
            SET_STATUS(&st, ek_err_passwd_non_alpha);
        else if (((policy.passwd_flags & sec_rgy_plcy_pwd_no_spaces) != 0) 
                 && (all_spaces))
            SET_STATUS(&st, ek_err_passwd_spaces);
    }
    return st;
}


/* FUNCTION eku_chk_date
 * 
 * dates have to be in the form [yy]yy/mm/dd[.hh:mm] (the century is optional).  
 * Legal dates range from 1980/01/01 to 2021/12/31.  If the date
 * string is syntactically legal and within range, it is converted to
 * a sec_timeval_sec_t value and returned in the clockh parameter.
 *
 * returns:
 *  ek_err_bad_date          - if the date can't be deciphered.
 *  ek_err_date_out_of_range - if the date is outside the legal range
 */
PUBLIC error_status_t eku_chk_date
#ifndef __STDC__
    ( date, date_len, clockh ) 
    char                *date;
    signed32            date_len;
    sec_timeval_sec_t   *clockh;
#else
  (
    char                *date,
    signed32            date_len,
    sec_timeval_sec_t   *clockh
  )
#endif
{
    ek_date_buf_t   ldate;
    int             num_scanned, leap, past_year, past_month;
    unsigned long   days_since_1970, secs_since_1970;
    sec_timeval_t       clock;
    struct timeval  tvp;   /* ditto */
    struct tm       tm, *ptm;
    int             year, month, day, hour, min;
    time_t          tloc;
    error_status_t  st;

    static   month_days[2][13] = {
        { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, /* normal years */
        { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }  /* leap years */
    };

    SET_STATUS(&st, error_status_ok);  
  
        /* catch the "live forever" constant */
    if (strcmp(date,ek_null_date) == 0) {
        *clockh = ek_live_forever;
        return st;
    }

    if (date_len > ek_lim_max_date_len) {
        SET_STATUS(&st, ek_err_bad_date);
        return st;
    }
   
    /* Special case the "now" date */
    if (strcmp(date, "now") == 0) {
        time(&tloc);
	*clockh = tloc;
	return error_status_ok;
    }  else {
        strcpy(ldate, date);  /* strcpy is safe because len has been range-checked */
    }

    hour = 0; min = 0;
    num_scanned = sscanf(ldate, " %d/%d/%d.%d:%d",
                            &year, &month, &day, &hour, &min);
    if ((num_scanned != 3) && (num_scanned != 5)) {
        SET_STATUS(&st, ek_err_bad_date);
        return st;
    }
        /* year can be specifed with or without the century.  years from
         * 0-69 belong to the 2000s.  years from 70-99 belong to the 1900s.
         * if the century is missing, we add it in */
    if ( (year >= 0) && (year <= 69) ) {
        year = year + 2000;
    } else if ( (year >= 70) && (year <= 99) ) {
        year = year + 1900;
    } 

    if ( (year < 1970) || (year > 2021) ) {
        SET_STATUS(&st, ek_err_date_out_of_range);
        return st;
    }

       /* month must be 1-12 */
    if ( (month < 1) || (month > 12) ) {
        SET_STATUS(&st, ek_err_date_out_of_range);
        return st;
    }


       /* hour must be 0-23 */
    if ( (hour < 0) || (hour > 23) ) {
        SET_STATUS(&st, ek_err_date_out_of_range);
        return st;
    }

       /* min must be 0-59 */
    if ( (min < 0) || (min > 59) ) {
        SET_STATUS(&st, ek_err_date_out_of_range);
        return st;
    }

       /* leap is our index into the appropriate dimension of the 
        * month_days vector
        */
    if ( year % 4 == 0)
        leap = 1;  
    else
        leap = 0;

    if ( (day < 1) || (day > month_days[leap][month]) ) {
        SET_STATUS(&st, ek_err_date_out_of_range);
        return st;
    }

    tm.tm_year = year - 1900;
    tm.tm_mon = month-1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = min;
    tm.tm_sec = 0;
    tm.tm_wday = 0;
    tm.tm_yday = 0;
    time (&tloc);
    /* !!! assume that the time of expiration has the same timezone as us */
    ptm = localtime(&tloc);
    tm.tm_isdst = ptm->tm_isdst;
    *clockh = mktime (&tm);
    
    return st;
}  
        

/* FUNCTION IS_LIFESPAN_STRING (private)
 *
 *  Return true, if the specified string has any of the valid format
 *  specifiers found in a lifespan string, false otherwise.
 */
PRIVATE boolean32 IS_LIFESPAN_STRING
#ifndef __STDC__
    (lifespan_str)
    char *lifespan_str;
#else
  (
    char *lifespan_str
  )
#endif
{
    boolean32   result;
    char        *c;

    result = false;

    for (c = lifespan_str; *c != '\0'; c++) {
        switch(*c) {
            case 'w':               /* Valid lifespan unit specifiers */
            case 'd':
            case 'h':
            case 'm':
            case 's':
                result = true;
                break;
            default:                /* Any other alpha char is bogus */
                if (isalpha(*c))
                    return false;
        }
    }
    return result;
}

    
/* FUNCTION eku_chk_lifespan    (converts lifespan to clock units)
 * 
 * Checks that the lifespan is either ek_lifespan_infinite or a string
 * in valid lifespan format (#w#d#h#m#s), then converts it to clock
 * units and returns the result in lifespan_p.
 * 
 * returns:
 *  ek_err_bad_lifespan          - if the lifespan isn't a valid formatted str
 *  ek_err_lifespan_out_of_range - if the lifespan is outside the legal range
 */
PUBLIC error_status_t eku_chk_lifespan
#ifndef __STDC__
    ( lifespan_str, lifespan_len, lifespan_p ) 
    char                  *lifespan_str;
    signed32               lifespan_len;
    sec_timeval_period_t  *lifespan_p;
#else
  (
    char                  *lifespan_str,
    signed32               lifespan_len,
    sec_timeval_period_t  *lifespan_p
  )
#endif
{
    int             lifespan, time;
    char            *tmpc;
    error_status_t  st;
    
    SET_STATUS(&st, error_status_ok);
    lifespan = 0;

    /* Hard to put a limit on this, but anything above BUFSIZ is ridiculous */
    if (lifespan_len > BUFSIZ) {
        SET_STATUS(&st, ek_err_lifespan_out_of_range);
    }

    for (tmpc = lifespan_str; (*tmpc != '\0') && GOOD_STATUS(&st); tmpc++) {
            /* accumulate decimal value */
        for (time = 0; isdigit(*tmpc); tmpc++) {
            time = time * 10 + (*tmpc - '0');
        }
            /* hit non-digit */
        if ((! isalpha(*tmpc)) && (*tmpc != '\0')) {
            SET_STATUS(&st, ek_err_bad_lifespan);
        } else {
            switch(*tmpc) {
                case 'W':
                case 'w':
                    time = time * ek_clock_one_week;
                    break;
                case 'D':
                case 'd':
                    time = time * ek_clock_one_day;
                    break;
                case 'H':
                case 'h':
                    time = time * ek_clock_one_hour;
                    break;
                case 'M':
                case 'm':
                    time = time * ek_clock_one_minute;
                    break;
                case 'S':
                case 's':
                    break;
                case '\0':      /* Leave str terminator there for outer loop */
                    tmpc--;
                    break;
                default:        /* Any other alpha char is invalid */
                    SET_STATUS(&st, ek_err_bad_lifespan);
            }
            if (GOOD_STATUS(&st)) {
                lifespan = lifespan + time;
            }
        }
    }
    *lifespan_p = lifespan;

    return st;
}


/* FUNCTION eku_chk_lifespan_days    (converts days to clock units)
 * 
 * Checks that the lifespan is either ek_lifespan_infinite or an integer in
 * the range ek_min_lifespan <= (lifespan) <= ek_max_lifespan.  If it is,
 * converts it to clock units and returns the result in lifespan_p.
 * 
 * returns:
 *  ek_err_bad_lifespan          - if the lifespan isn't a decimal integer.
 *  ek_err_lifespan_out_of_range - if the lifespan is outside the legal range
 */
PUBLIC error_status_t eku_chk_lifespan_days
#ifndef __STDC__
    ( lifespan_str, lifespan_len, lifespan_p ) 
    char                  *lifespan_str;
    signed32               lifespan_len;
    sec_timeval_period_t  *lifespan_p;
#else
  (
    char                  *lifespan_str,
    signed32               lifespan_len,
    sec_timeval_period_t  *lifespan_p
  )
#endif
{
    int             life_days;
    error_status_t  st;
    
    SET_STATUS(&st, error_status_ok);

    if ( (lifespan_len == strlen(ek_lifespan_infinite)) &&
         (strncmp(lifespan_str, ek_lifespan_infinite, lifespan_len) == 0) ) {
        *lifespan_p = ek_live_forever;
        return st;
    }

        /* Pass down to lifespan converter if necessary */
    if (IS_LIFESPAN_STRING(lifespan_str)) {

        eku_chk_lifespan(lifespan_str, lifespan_len, lifespan_p);

    } else {

        /* Otherwise it must be number of days */
       if (lifespan_len > ek_lim_max_int32_len)
           SET_STATUS(&st, ek_err_lifespan_out_of_range);
       else if (!is_decimal_int(lifespan_str, lifespan_len))
           SET_STATUS(&st, ek_err_bad_lifespan);
       else {
           life_days = atol(lifespan_str);
           if ( (life_days < ek_min_lifespan) || 
                (life_days > ek_max_lifespan) )
               SET_STATUS(&st, ek_err_lifespan_out_of_range);
       }
   
       if (GOOD_STATUS(&st))
           *lifespan_p = (life_days * ek_clock_one_day);
    }

    return st;
}


/* FUNCTION eku_chk_lifespan_hours  (converts hours to clock units)
 * 
 * Checks that the lifespan is either ek_lifespan_infinite or an integer in
 * the range ek_min_lifespan <= (lifespan) <= ek_max_lifespan.  If it is,
 * converts it to clock units and returns the result in lifespan_p.
 * 
 * returns:
 *  ek_err_bad_lifespan          - if the lifespan isn't a decimal integer.
 *  ek_err_lifespan_out_of_range - if the lifespan is outside the legal range
 */
PUBLIC error_status_t eku_chk_lifespan_hours
#ifndef __STDC__
    ( lifespan_str, lifespan_len, lifespan_p ) 
    char                  *lifespan_str;
    signed32              lifespan_len;
    sec_timeval_period_t  *lifespan_p;
#else
  (
    char                  *lifespan_str,
    signed32              lifespan_len,
    sec_timeval_period_t  *lifespan_p
  )
#endif
{
    int             life_hours;
    error_status_t  st;
    
    SET_STATUS(&st, error_status_ok);

    if ( (lifespan_len == strlen(ek_lifespan_infinite)) &&
         (strncmp(lifespan_str, ek_lifespan_infinite, lifespan_len) == 0) ) {
        *lifespan_p = ek_live_forever;
        return st;
    }
    
        /* Pass down to lifespan converter if necessary */
    if (IS_LIFESPAN_STRING(lifespan_str)) {

        eku_chk_lifespan(lifespan_str, lifespan_len, lifespan_p);

    } else {

            /* Otherwise it must be number of days */
        if (lifespan_len > ek_lim_max_int32_len)
            SET_STATUS(&st, ek_err_lifespan_out_of_range);
        else if (!is_decimal_int(lifespan_str, lifespan_len))
            SET_STATUS(&st, ek_err_bad_lifespan);
        else {
            life_hours = atol(lifespan_str);
            if (life_hours < ek_min_lifespan)
                SET_STATUS(&st, ek_err_lifespan_out_of_range);
        }
    
        if (GOOD_STATUS(&st))
            *lifespan_p = (life_hours * ek_clock_one_hour);
    }

    return st;
}


/* FUNCTION eku_chk_lifespan_minutes  (converts minutes to clock units)
 * 
 * Checks that the lifespan is either ek_lifespan_infinite or an integer in
 * the range ek_min_lifespan <= (lifespan) <= ek_max_lifespan.  If it is,
 * converts it to clock units and returns the result in lifespan_p.
 * 
 * returns:
 *  ek_err_bad_lifespan          - if the lifespan isn't a decimal integer.
 *  ek_err_lifespan_out_of_range - if the lifespan is outside the legal range
 */
PUBLIC error_status_t eku_chk_lifespan_minutes
#ifndef __STDC__
    ( lifespan_str, lifespan_len, lifespan_p ) 
    char                  *lifespan_str;
    signed32              lifespan_len;
    sec_timeval_period_t  *lifespan_p;
#else
  (
    char                  *lifespan_str,
    signed32              lifespan_len,
    sec_timeval_period_t  *lifespan_p
  )
#endif
{
    int             life_minutes;
    error_status_t  st;
    
    SET_STATUS(&st, error_status_ok);

    if ( (lifespan_len == strlen(ek_lifespan_infinite)) &&
         (strncmp(lifespan_str, ek_lifespan_infinite, lifespan_len) == 0) ) {
        *lifespan_p = ek_live_forever;
        return st;
    }
    
        /* Pass down to lifespan converter if necessary */
    if (IS_LIFESPAN_STRING(lifespan_str)) {

        eku_chk_lifespan(lifespan_str, lifespan_len, lifespan_p);

    } else {

            /* Otherwise it must be number of days */
        if (lifespan_len > ek_lim_max_int32_len)
            SET_STATUS(&st, ek_err_lifespan_out_of_range);
        else if (!is_decimal_int(lifespan_str, lifespan_len))
            SET_STATUS(&st, ek_err_bad_lifespan);
        else {
            life_minutes = atol(lifespan_str);
            if (life_minutes < ek_min_lifespan)
                SET_STATUS(&st, ek_err_lifespan_out_of_range);
        }
    
        if (GOOD_STATUS(&st))
            *lifespan_p = (life_minutes * ek_clock_one_minute);
    }

    return st;
}


/* FUNCTION eku_chk_integer32
 *
 * checks that the integer string is within the limits for a 32 bit integer and
 * does not contain any non-numeric characters.  returns converted integer in
 * int_p.
 *
 * returns
 *    ek_err_non_numeric    - if the integer string contains non-numeric chars.
 *    ek_err_int_out_range  - if the integer srtring is too large to be converted
 *                             to a 32 bit integer
 */        
PUBLIC error_status_t eku_chk_integer32
#ifndef __STDC__
    ( int_str, int_len, int_p ) 
    char  *int_str;
    signed32  int_len;
    signed32  *int_p;
#else
  (
    char  *int_str,
    signed32  int_len,
    signed32  *int_p
  )
#endif
{
    long                max_int_len;
    error_status_t      st;
    ek_integer32_buf_t  my_int_str;  

    SET_STATUS(&st, error_status_ok);

    if (int_len == 0) {
        SET_STATUS(&st, ek_err_int_out_of_range);
        return st;
    }

    if (!is_decimal_int(int_str, int_len))
        SET_STATUS(&st, ek_err_non_numeric);
    else {
        if (*int_str == '-' || *int_str == '+')
            max_int_len = ek_lim_max_int32_len;
        else
            max_int_len = ek_lim_max_int32_len - 1;
        if (int_len > max_int_len)
            SET_STATUS(&st, ek_err_int_out_of_range); 
    }

    if (GOOD_STATUS(&st)) {
        strncpy(my_int_str, int_str, int_len);
        my_int_str[int_len] = '\0';
        *int_p = atol(my_int_str);
    }

    return st;
}
    

/********************************************************************************
 ******************************* GENERIC CONVERSION ROUTINES ********************
 ********************************************************************************
 *
 * Generic conversion routines are used to convert from strings to rgy_ types, 
 * and vice versa.  
 */


/* FUNCTION integer32_to_string
 *
 * converts a 32 bit signed integer to a string.  The string buffer is passed
 * in by the caller and is assumed to be a least (ek_lim_max_int32_len + 1) 
 * chars long
 */
PRIVATE void integer32_to_string
#ifndef __STDC__
    ( int32, int_str ) 
    signed32  int32;
    char      *int_str;
#else
  (
    signed32  int32,
    char      *int_str
  )
#endif
{
    sprintf(int_str, "%d%", int32);
}


/* FUNCTION eku_dec_int32
 *
 * converts a 32 bit signed integer to a string and stores it in the specified
 * form field.
 */
void eku_dec_int32
#ifndef __STDC__
    (form, field, int32) 
    form_t               *form;
    form_field_offset_t  field;
    signed32             int32;
#else
  (
    form_t               *form,
    form_field_offset_t  field,
    signed32             int32
  )
#endif
{
    char int32_str[ek_lim_max_int32_len + 1];

    integer32_to_string(int32, int32_str);
    form_write(form, field, int32_str, strlen(int32_str), false, &Dummy_st);
}


/* FUNCTION eku_dec_lifespan
 *
 * Convert seconds to ##w##d##h##m##s"
 */
void eku_dec_lifespan
#ifndef __STDC__
    ( form, field, clock_units ) 
    form_t               *form;
    form_field_offset_t  field;
    sec_timeval_period_t clock_units;
#else
  (
    form_t               *form,
    form_field_offset_t  field,
    sec_timeval_period_t clock_units
  )
#endif
{
    char                lifespan[BUFSIZ];
    ek_integer32_buf_t  units;
    int weeks, days, hours, minutes, seconds;

    lifespan[0] = '\0';

    /* Convert to units */
    weeks = (clock_units/ek_clock_one_week);
    clock_units -= (weeks * ek_clock_one_week);

    days = (clock_units/ek_clock_one_day);
    clock_units -= (days * ek_clock_one_day);

    hours = (clock_units/ek_clock_one_hour);
    clock_units -= (hours * ek_clock_one_hour);

    minutes = (clock_units/ek_clock_one_minute);
    seconds = clock_units - (minutes * ek_clock_one_minute);

    /* Only print non-zero data */
    if (weeks) {
        sprintf(units, "%dw", weeks);
        strcat(lifespan, units);
    }
    if (days) {
        sprintf(units, "%dd", days);
        strcat(lifespan, units);
    }
    if (hours) {
        sprintf(units, "%dh", hours);
        strcat(lifespan, units);
    }
    if (minutes) {
        sprintf(units, "%dm", minutes);
        strcat(lifespan, units);
    }
    if (seconds) {
        sprintf(units, "%ds", seconds);
        strcat(lifespan, units);
    }

    form_write(form, field, lifespan, strlen(lifespan), false, &Dummy_st);
}
    
    
/*  
 *  FUNCTION eku_dec_name
 * 
 * Copies the data from a field into a sec_rgy_name_t parameter. Assumes the
 * validity of the field data has been checked prior to the call, so doesn't
 * perform any bounds checking.  If the data in the field is longer than the 
 * name_len parameter,  the rgy_name contains a truncated version of the field
 * data.
 */
void eku_dec_name
#ifndef __STDC__
    ( op, name_field, rgy_name, name_len ) 
    form_t               *op;
    form_field_offset_t  name_field;
    sec_rgy_name_t       rgy_name;
    signed32             *name_len;  /* IN - size of rgy_name buffer,
                                        OUT - length of name in rgy_name */
#else
  (
    form_t               *op,
    form_field_offset_t  name_field,
    sec_rgy_name_t       rgy_name,
    signed32             *name_len
  )
#endif
{
    form_read(op, name_field, rgy_name, *name_len, name_len);
}


/*
 * FUNCTION eku_enc_uuid
 *
 * Takes a string representation of a uuid and converts it to  uuid_t.
 */
void eku_enc_uuid
#ifndef __STDC__
    ( uuid_str, uuid_p) 
    char   *uuid_str;
    uuid_t *uuid_p;
#else
  (
    char   *uuid_str,
    uuid_t *uuid_p
  )
#endif
{
    error_status_t     st;
    unsigned_char_p_t  uuid_str_p;

    uuid_from_string(uuid_str_p, uuid_p, &st); 
    if (GOOD_STATUS(&st)) {
        strcpy(uuid_str, (char *) uuid_str_p);
        rpc_string_free(&uuid_str_p, &st);
    }
}



/* 
 * FUNCTION eku_dec_uuid
 * converts a uid to a string and stores the result in the specified field.
 */
void eku_dec_uuid
#ifndef __STDC__
    (form, field_offset, uid) 
    form_t               *form;
    form_field_offset_t  field_offset;
    uuid_t               uid;
#else
  (
    form_t               *form,
    form_field_offset_t  field_offset,
    uuid_t               uid
  )
#endif
{
    unsigned_char_p_t    uuid_str;
    boolean32            free_uuid_str = true; 
    error_status_t       lst;

    uuid_to_string (&uid, &uuid_str, &lst); 
    if (BAD_STATUS(&lst)) { 
        uuid_str = (unsigned_char_p_t) "<INVALID UUID>"; 
        free_uuid_str = false;
    }
    
    form_write(form, field_offset, (char *)uuid_str, strlen((char *)uuid_str),
               false, &Dummy_st);

    if (free_uuid_str && uuid_str != NULL) {
        rpc_string_free(&uuid_str, &lst);
    }
}


/*
 * FUNCTION unum_to_string
 *
 * converts a unix number to a string.  Assumes the supplied string is
 * long enough to hold the converted unix number. 
 */
PRIVATE void unum_to_string
#ifndef __STDC__
    (unum_str, unum)
    char     *unum_str;
    signed32 unum;
#else
  (
    char     *unum_str,
    signed32 unum
  )
#endif
{
    sprintf (unum_str, "%d", unum);
}


/* 
 * LOCAL FUNCTION eku_dec_unix_num
 *
 * Converts a unix number to a string and stores the result in the specified
 * field buffer 
 */
void eku_dec_unix_num
#ifndef __STDC__
    ( form, unum_field, unum ) 
    form_t               *form; 
    form_field_offset_t  unum_field;
    signed32             unum;
#else
  (
    form_t               *form, 
    form_field_offset_t  unum_field,
    signed32             unum
  )
#endif
{
    char int_str[ek_lim_max_unix_num_len + 1];
    unum_to_string(int_str, unum);
    form_write (form, unum_field, int_str, strlen(int_str),false, &Dummy_st);
}


/*
 * FUNCTION eku_dec_pgo_flag
 *
 * converts from the sec_rgy_pgo_flags_t bitset format to the appropriate kernel 
 * flag  value (ek_flag_on or ek_flag_off), and stores the result in the 
 * specified field's buffer. 
 */
void eku_dec_pgo_flag
#ifndef __STDC__
    ( op, flag_field, pgo_flags, mask ) 
    form_t               *op;
    form_field_offset_t  flag_field;
    sec_rgy_pgo_flags_t  pgo_flags; 
    sec_rgy_pgo_flags_t  mask;
#else
  (
    form_t               *op,
    form_field_offset_t  flag_field,
    sec_rgy_pgo_flags_t  pgo_flags, 
    sec_rgy_pgo_flags_t  mask
  )
#endif
{
    if (!FLAG_SET(pgo_flags, mask))  
        form_write (op, flag_field, ek_flag_off, strlen(ek_flag_off), false, 
                    &Dummy_st);
    else
        form_write (op, flag_field, ek_flag_on, strlen(ek_flag_on), false, &Dummy_st);
}


/*
 * FUNCTION eku_dec_policy_pwd_flag
 *
 * converts from the sec_rgy_plcy_pwd_flags_t  format to the appropriate kernel
 * flag  value (ek_flag_on or ek_flag_off), and stores the result in the
 * specified field. 
 */
void eku_dec_policy_pwd_flag
#ifndef __STDC__
    ( form, flag_field, policy_pwd_flags, mask ) 
    form_t                    *form;
    form_field_offset_t       flag_field;
    sec_rgy_plcy_pwd_flags_t  policy_pwd_flags; 
    sec_rgy_plcy_pwd_flags_t  mask;
#else
  (
    form_t                    *form,
    form_field_offset_t       flag_field,
    sec_rgy_plcy_pwd_flags_t  policy_pwd_flags, 
    sec_rgy_plcy_pwd_flags_t  mask
  )
#endif
{
    if (!FLAG_SET(policy_pwd_flags, mask))  
        form_write (form, flag_field, ek_flag_off, strlen(ek_flag_off), false,
                    &Dummy_st);
    else
        form_write (form, flag_field, ek_flag_on, strlen(ek_flag_on), false,
                    &Dummy_st);
}


/*
 * FUNCTION eku_dec_acct_user_flag
 *
 * converts from the sec_rgy_acct_user_flags_t  format to the appropriate kernel
 * flag  value (ek_flag_on or ek_flag_off), and stores the result in the
 * specified field. 
 */
void eku_dec_acct_user_flag
#ifndef __STDC__
    (form, flag_field, acct_user_flags, mask) 
    form_t                     *form;
    form_field_offset_t        flag_field;
    sec_rgy_acct_user_flags_t  acct_user_flags; 
    sec_rgy_acct_user_flags_t  mask;
#else
  (
    form_t                     *form,
    form_field_offset_t        flag_field,
    sec_rgy_acct_user_flags_t  acct_user_flags, 
    sec_rgy_acct_user_flags_t  mask
  )
#endif
{
    if (!FLAG_SET(acct_user_flags, mask))  
        form_write (form, flag_field, ek_flag_off, strlen(ek_flag_off), false,
                    &Dummy_st);
    else
        form_write (form, flag_field, ek_flag_on, strlen(ek_flag_on), false, 
                    &Dummy_st);
}


/*
 * FUNCTION eku_enc_acct_user_flag
 *
 * reads the value stored in the indicated flag field and twiddled the bits of
 * the acct user flag bitset accordingly.  Expects the flag field to have 
 * valid flag string in it.  
 */
void eku_enc_acct_user_flag
#ifndef __STDC__
    ( form, flag_field, acct_user_flags, mask ) 
    form_t                     *form;
    form_field_offset_t        flag_field;
    sec_rgy_acct_user_flags_t  *acct_user_flags; 
    sec_rgy_acct_user_flags_t  mask;
#else
  (
    form_t                     *form,
    form_field_offset_t        flag_field,
    sec_rgy_acct_user_flags_t  *acct_user_flags, 
    sec_rgy_acct_user_flags_t  mask
  )
#endif
{
    ek_flag_buf_t              flag_buff;
    long int                   flag_len;

    form_read (form, flag_field, flag_buff, sizeof(flag_buff), &flag_len);
    if (CHECK_FLAG(flag_buff, flag_len))
        SET(*acct_user_flags, mask);
    else
        UNSET(*acct_user_flags, mask);
}


/*
 * FUNCTION eku_dec_acct_admin_flag
 *
 * converts from the sec_rgy_acct_admin_flags_t  format to the appropriate kernel
 * flag  value (ek_flag_on or ek_flag_off), and stores the result in the 
 * specified field. 
 */
void eku_dec_acct_admin_flag
#ifndef __STDC__
    ( form, flag_field, acct_admin_flags, mask ) 
    form_t                      *form;
    form_field_offset_t         flag_field;
    sec_rgy_acct_admin_flags_t  acct_admin_flags; 
    sec_rgy_acct_admin_flags_t  mask;
#else
  (
    form_t                      *form,
    form_field_offset_t         flag_field,
    sec_rgy_acct_admin_flags_t  acct_admin_flags, 
    sec_rgy_acct_admin_flags_t  mask
  )
#endif
{
    if (!FLAG_SET(acct_admin_flags, mask))  
        form_write (form, flag_field, ek_flag_off, strlen(ek_flag_off), false,
                     &Dummy_st);
    else
        form_write (form, flag_field, ek_flag_on, strlen(ek_flag_on), false, 
                     &Dummy_st);
}


/*
 * FUNCTION eku_enc_acct_admin_flag
 *
 * reads the value stored in the indicated flag field and twiddled the bits of
 * the acct admin flag bitset accordingly.  Expects the flag field to have 
 * valid flag string in it.  
 */
void eku_enc_acct_admin_flag
#ifndef __STDC__
    ( form, flag_field, acct_admin_flags, mask ) 
    form_t                      *form;
    form_field_offset_t         flag_field;
    sec_rgy_acct_admin_flags_t  *acct_admin_flags; 
    sec_rgy_acct_admin_flags_t  mask;
#else
  (
    form_t                      *form,
    form_field_offset_t         flag_field,
    sec_rgy_acct_admin_flags_t  *acct_admin_flags, 
    sec_rgy_acct_admin_flags_t  mask
  )
#endif
{
    ek_flag_buf_t               flag_buff;
    long int                    flag_len;

    form_read(form, flag_field, flag_buff, sizeof(flag_buff), &flag_len);
    if (CHECK_FLAG(flag_buff, flag_len))
        SET(*acct_admin_flags, mask);
    else
        UNSET(*acct_admin_flags, mask);
}


/*
 * FUNCTION eku_dec_property_flag
 *
 * converts from the sec_rgy_properties_flags_t  format to the appropriate kernel
 * flag value (ek_flag_on or ek_flag_off), and stores the result in the
 * specified field. 
 */
void eku_dec_property_flag
#ifndef __STDC__
    ( form, flag_field, rgy_prop_flags, mask ) 
    form_t                      *form;
    form_field_offset_t         flag_field;
    sec_rgy_properties_flags_t  rgy_prop_flags; 
    sec_rgy_properties_flags_t  mask;
#else
  (
    form_t                      *form,
    form_field_offset_t         flag_field,
    sec_rgy_properties_flags_t  rgy_prop_flags, 
    sec_rgy_properties_flags_t  mask
  )
#endif
{
    if (!FLAG_SET(rgy_prop_flags, mask))  
        form_write (form, flag_field, ek_flag_off, strlen(ek_flag_off), false, 
                    &Dummy_st);
    else
        form_write (form, flag_field, ek_flag_on, strlen(ek_flag_on), false,
                    &Dummy_st);
}


/*
 * FUNCTION sid_to_login_name
 *
 * Converts a sid (uuid triplet) to a login name.  uuid_nil components
 * are converted to a wildcard (%).  If a uuid has no associated name, we copy
 * the uuid (surrounded by angle brackets) to the login name.  Otherwise the 
 * associated name is copied to the login name.
 */
PRIVATE void sid_to_login_name
#ifndef __STDC__
    ( sid, lname ) 
    sec_rgy_sid_t         sid;
    sec_rgy_login_name_t  *lname;
#else
  (
    sec_rgy_sid_t         sid,
    sec_rgy_login_name_t  *lname
  )
#endif
{
    unsigned_char_t       *pgo_name;
    sec_rgy_name_t        tmp_name;
    sec_rgy_domain_t      domain;
    uuid_t                *uuid, uuid_nil;
    unsigned_char_p_t     uuid_str;
    error_status_t        st;
    int i;

#define          wildcard "%"

    uuid_create_nil(&uuid_nil, &st);
    for (domain = sec_rgy_domain_person; domain < sec_rgy_domain_last; domain++) {
        switch (domain) {    /* get the right piece of the uuid triplet */

        case sec_rgy_domain_person:
            uuid = &sid.person;
            pgo_name = lname->pname;
            break;
        case sec_rgy_domain_group:
            uuid = &sid.group;
            pgo_name = lname->gname;
            break;
        case sec_rgy_domain_org:
            uuid = &sid.org;
            pgo_name = lname->oname;
            break;
        default:
            pgo_name = tmp_name;  /* a bit bucket so we don't trash memory */
            break;
        }
        
        if (uuid_equal(uuid, &uuid_nil, &st)) {
             /* it's a wildcard */
            strcpy((char *)pgo_name, wildcard);
        }
        else {
            /* Don't try to convert corrupt uuid to a name */
            uuid_to_string(uuid, (unsigned_char_p_t *) &uuid_str, &st);
            if (GOOD_STATUS(&st)) {
                sec_rgy_pgo_id_to_name(context, domain, uuid, pgo_name, &st);
                if (BAD_STATUS(&st)) {
                    strcpy((char *)pgo_name, "<");
                    strcat((char *)pgo_name, (char *) uuid_str);
                    strcat((char *)pgo_name, ">");                          
                }
                rpc_string_free(&uuid_str, &st);
            }
            else {
                    strcpy((char *)pgo_name, "<UNKNOWN>");
            }
        } 
    }
}


/* 
 * FUNCTION eku_dec_rgy_sid
 *
 * converts a sec_rgy_sid_t value into a sec_rgy_login_name_t value, and stores the
 * result in the specified field buffer.
 */
void eku_dec_rgy_sid
#ifndef __STDC__
    ( form, owner_field, sid ) 
    form_t               *form;
    form_field_offset_t  owner_field;
    sec_rgy_sid_t        sid;
#else
  (
    form_t               *form,
    form_field_offset_t  owner_field,
    sec_rgy_sid_t        sid
  )
#endif
{
    sec_rgy_login_name_t  login_name;
    
    sid_to_login_name(sid, &login_name);
    form_write(form, owner_field, login_name.pname,
                strlen((char *)login_name.pname), false, &Dummy_st);
}


/* 
 * FUNCTION eku_dec_foreign_id
 *
 * converts a sec_id_foreign_t into a compound string and stores the result
 * result in the specified field buffer.
 */
void eku_dec_foreign_id
#ifndef __STDC__
    ( form, owner_field, owner ) 
    form_t               *form;
    form_field_offset_t  owner_field;
    sec_rgy_foreign_id_t owner;
#else
  (
    form_t               *form,
    form_field_offset_t  owner_field,
    sec_rgy_foreign_id_t owner
  )
#endif
{
    /*
     * the global form of a principal name could be twice as long as the
     * the cell-relative form
     */
    char            owner_name[2 * ek_lim_max_name_len + 1];
    sec_rgy_name_t  name;
    error_status_t  st;
    unsigned char   *uuid_string, *namep;
    int             len;

    /* Default is no owner, just in case */
    owner_name[0] = '\0';

    sec_rgy_pgo_id_to_name(context, sec_rgy_domain_person, &owner.cell,
                            name, &st);
    if (GOOD_STATUS(&st)) {
        /* Substitute the architectural name 'krbtgt' with /... */
        if (strncmp("krbtgt", (char *) name, 6) == 0) {
            namep = &(name[2]);
            namep[0] = '/'; namep[1] = '.'; namep[2] = '.'; namep[3] = '.';
        } else
            namep = &(name[0]);
        strncpy(owner_name, (char *)namep, ek_lim_max_name_len + 1);
    } else {
        uuid_to_string(&owner.cell, &uuid_string, &st);
        if (GOOD_STATUS(&st))
            /* we're sure a uuid string will fit in a name buffer */
            strcpy(owner_name, (char *) uuid_string);
    }

    /* If still not good status then no realm, so principal can't be found */
    if (GOOD_STATUS(&st)) {
        if ((len = strlen(owner_name)) < ek_lim_max_name_len) {
            strcat(owner_name, "/");
            len++;
        }
        /* Get the principal name associated with the principal uuid */
        sec_rgy_pgo_id_to_name(context,sec_rgy_domain_person,&owner.principal,
                                name, &st);
        if (GOOD_STATUS(&st))
            strncat(owner_name, (char *) name, (ek_lim_max_name_len - len));
    }
    
    owner_name[2 * ek_lim_max_name_len] = '\0';
    form_write(form, owner_field, owner_name,
                strlen((char *)owner_name), false, &Dummy_st);
}


/* FUNCTION sec_to_dt_string
 *
 *  takes a sec_timeval_sec_t and converts it to a string of the form 
 *  yyyy.mm.dd.hh.mm .  The buffer for the string is provided by
 *  the caller and is assumed to be large enough to hold the converted
 *  null-terminated date-time string.
 */
PRIVATE void sec_to_dt_string
#ifndef __STDC__
    ( secs, dt_str ) 
    sec_timeval_sec_t  secs;     /* IN - date-time  */
    char               *dt_str;  /* OUT - buffer forconverted date-time string */
#else
  (
    sec_timeval_sec_t  secs,     /* IN - date-time  */
    char               *dt_str
  )
#endif
{
    struct tm          *tmp;    /* pointer to a struct tm */
    time_t             unix_time;

    unix_time = (time_t)secs;

        /* convert the timeval to struct tm containing the local time*/
    tmp = localtime(&unix_time);

        /* convert the struct tm into the kernel date-time string format */
    sprintf(dt_str, "%04d/%02d/%02d.%02d:%02d", tmp->tm_year + 1900,
            (tmp->tm_mon + 1), tmp->tm_mday, (tmp->tm_hour), tmp->tm_min);
}


/* 
 * FUNCTION eku_dec_date_time
 *
 * 
 * Converts  a date-time in sec_timeval_sec_t form into a date-time string and writes
 * the results to the specified field.
 */
void eku_dec_date_time
#ifndef __STDC__
    ( form, dt_field, secs ) 
    form_t               *form;
    form_field_offset_t  dt_field;
    sec_timeval_sec_t    secs;           
#else
  (
    form_t               *form,
    form_field_offset_t  dt_field,
    sec_timeval_sec_t    secs
  )
#endif
{
    char dt_string[ek_lim_max_date_time_len + 1];
    
    sec_to_dt_string(secs, dt_string);
    form_write(form, dt_field, dt_string, strlen(dt_string), false, &Dummy_st);
}


/* 
 * FUNCTION eku_dec_bool32
 * 
 * Converts  a  form into a date-time string and writes
 * the results to the specified field.
 */
void eku_dec_bool32
#ifndef __STDC__
    ( form, field, bool32 ) 
    form_t               *form;
    form_field_offset_t  field;
    boolean32            bool32;
#else
  (
    form_t               *form,
    form_field_offset_t  field,
    boolean32            bool32
  )
#endif
{
    if ( bool32 == true)  
        form_write (form, field, ek_flag_on, strlen(ek_flag_on), false,
                    &Dummy_st);
    else
        form_write (form, field, ek_flag_off, strlen(ek_flag_off), false,
                    &Dummy_st);
}


/* FUNCTION eku_dec_acct_key_parts
 *  converts a sec_rgy_acct_key_t parameter to the appropriate kernel representation
 *  of a key part and stores the results in the specified form field.
 */
void eku_dec_acct_key_parts
#ifndef __STDC__
    (form, field, key_parts)
    form_t               *form;             
    form_field_offset_t  field;
    sec_rgy_acct_key_t   key_parts;
#else
  (
    form_t               *form,             
    form_field_offset_t  field,
    sec_rgy_acct_key_t   key_parts
  )
#endif
{
    switch (key_parts) {

    case sec_rgy_acct_key_none:
        form_write(form, field, ek_acct_key_none, strlen(ek_acct_key_none), 
                    false, &Dummy_st);
        break;
    case sec_rgy_acct_key_person:
        form_write(form, field, ek_acct_key_person, strlen(ek_acct_key_person), 
                    false, &Dummy_st);
        break;
    case sec_rgy_acct_key_group:
        form_write(form, field, ek_acct_key_group, strlen(ek_acct_key_group), 
                    false, &Dummy_st);
        break;
    case sec_rgy_acct_key_org:
        form_write(form, field, ek_acct_key_org, strlen(ek_acct_key_org), 
                    false, &Dummy_st);
        break;

    default:   /* just so something that won't dump core gets returned */
        form_write(form, field, "", strlen(""),  false, &Dummy_st);       
    }
}



void eku_translate_status
#ifndef __STDC__
    ( stp ) 
    error_status_t  *stp;
#else
  (
    error_status_t  *stp
  )
#endif
{
    if (STATUS_EQUAL(stp, error_status_ok))
        return;

    switch (*stp) {
    case sec_rgy_not_implemented:
        SET_STATUS(stp, ek_misc_not_implemented);
        break;
    case sec_rgy_bad_domain:
        SET_STATUS(stp, ek_misc_bad_domain);
        break;
    case sec_rgy_name_exists:
        SET_STATUS(stp, ek_err_name_exists);
        break;
    case sec_rgy_object_exists:
        SET_STATUS(stp, ek_err_object_exists);
        break;
    case sec_rgy_unix_id_changed:
        SET_STATUS(stp, ek_err_unix_num_change);
        break;
    case sec_rgy_is_an_alias:
        SET_STATUS(stp, ek_misc_is_an_alias);
        break;
    case sec_rgy_no_more_entries:
        SET_STATUS(stp, ek_misc_no_more_entries);
        break;
    case sec_rgy_object_not_found:
        SET_STATUS(stp, ek_misc_object_not_found);
        break;
    case sec_rgy_not_member_group:
        SET_STATUS(stp, ek_err_not_member_group);
        break;
    case sec_rgy_not_member_org:
        SET_STATUS(stp, ek_err_not_member_org);
        break;
    case sec_rgy_not_member_group_org:
        SET_STATUS(stp, ek_err_not_member_go);
        break;
    case sec_rgy_server_unavailable:
        SET_STATUS(stp, ek_misc_server_unavailable);
        break;
    case sec_rgy_incomplete_login_name:
        SET_STATUS(stp, ek_err_incomplete_login_name);
        break;
    case sec_rgy_passwd_invalid:
        SET_STATUS(stp, ek_err_passwd_invalid);
        break;
    case sec_rgy_not_authorized:
        SET_STATUS(stp, ek_misc_not_authorized);
        break;
    case sec_rgy_read_only:
        SET_STATUS(stp, ek_misc_read_only);
        break;
    case sec_rgy_bad_data:
        SET_STATUS(stp, ek_misc_bad_data);
        break;
    }
}
