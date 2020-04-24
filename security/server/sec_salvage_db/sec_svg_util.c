/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_svg_util.c,v $
 * Revision 1.1.12.3  1996/02/18  00:22:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:59  marty]
 *
 * Revision 1.1.12.2  1995/12/08  18:01:23  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/26  13:37 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/HPDCE02/hanfei_mothra_bl4/1  1995/05/25  17:06 UTC  hanfei
 * 	modify to use RSDB_PGO and RSDB_DIR macros
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/15  21:12 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:37 UTC  greg  /main/HPDCE02/SEC_migrate_merge/2]
 * 
 * 	fix printing attr_schema rsdb_acl_mgr_type.
 * 	[1995/04/21  19:39 UTC  hanfei  /main/HPDCE02/hanfei_migrate_bl2/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/10/27  15:29 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_svg_bl1/1  1994/10/25  17:26 UTC  hanfei
 * 	Enhancement work for attribute schema and attributes.
 * 	[1995/12/08  17:22:45  root]
 * 
 * Revision 1.1.6.2  1994/06/17  18:43:02  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:20:14  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/25  18:25:03  sekhar
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 	[1994/03/25  18:24:19  sekhar]
 * 
 * Revision 1.1.3.3  1993/03/23  23:02:46  hanfei
 * 	add codes for salvager reconstruct
 * 	[1993/03/23  22:49:43  hanfei]
 * 
 * Revision 1.1.3.2  1993/03/05  18:34:29  frisco
 * 	fix for RIOS port
 * 	[1993/03/03  20:03:44  frisco]
 * 
 * 	initial salvager functionality
 * 	[1993/03/02  21:25:37  frisco]
 * 
 * Revision 1.1.1.2  1992/11/25  21:12:03  frisco
 * 	Initial Revision
 * 
 * $EndLog$
 */

/*  
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
*/

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: sec_svg_util.c,v 1.1.12.3 1996/02/18 00:22:18 marty Exp $";
#endif

/* 
**      Salvager Utilities 
*/

#include <stdio.h> 
#include <sys/time.h>
#include <time.h>
#include <string.h>

#include <dce/dce.h>
#include <macros.h>

#include <sec_svg_util.h>
#include <rsdb_util.h>
#include <dce/rgybase.h>
#include <dce/aclbase.h>
#include <dce/dcesadmsg.h>

#include <rsdb_attr_pvt.h>
#include <sec_attr_base.h>
#include <rs_rep_pvt.h>
#include <rs_attr_base.h>


/*  Table of entry types to match ascii strings with equivalent types
 *  Note: Last entry is NULL, to end the loop that checks a given
 *  string against all known types for a match.
 */

PRIVATE sec_acl_entry_type_table_t entry_types[] = {
    { "mask_obj",       sec_acl_e_type_mask_obj         },
    { "user_obj",       sec_acl_e_type_user_obj         },
    { "group_obj",      sec_acl_e_type_group_obj        },
    { "other_obj",      sec_acl_e_type_other_obj        },
    { "unauthenticated",sec_acl_e_type_unauthenticated  },
    { "user",           sec_acl_e_type_user             },
    { "group",          sec_acl_e_type_group            },
    { "foreign_other",  sec_acl_e_type_foreign_other    },
    { "any_other",      sec_acl_e_type_any_other        },
    { "foreign_user",   sec_acl_e_type_foreign_user     },
    { "foreign_group",  sec_acl_e_type_foreign_group    },
    { "extended",       sec_acl_e_type_extended         },
    { NULL,             sec_acl_e_type_extended         }
};

/* FUNCTION sec_to_dt_string
 *
 *  takes a sec_timeval_sec_t and converts it to a string of the form 
 *  yyyy.mm.dd.hh.mm .  The buffer for the string is provided by
 *  the caller and is assumed to be large enough to hold the converted
 *  null-terminated date-time string.
 */
PUBLIC void sec_to_dt_string
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
    sec_timeval_t      unix_clock;

    if (secs == 0) 
        sprintf(dt_str, "none");
    else {
        unix_clock.sec = secs;
        unix_clock.usec = 0;

        /* convert the timeval to struct tm containing the local time*/
        tmp = localtime(&(unix_clock.sec));

        /* convert the struct tm into the kernel date-time string format */
        sprintf(dt_str, "%04d/%02d/%02d.%02d:%02d", tmp->tm_year + 1900,
            (tmp->tm_mon + 1), tmp->tm_mday, (tmp->tm_hour), tmp->tm_min);
    }
}

/* FUNCTION dt_to_sec
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
PUBLIC error_status_t dt_to_sec
#ifndef __STDC__
    ( date, clockh ) 
    char                *date;
    sec_timeval_sec_t   *clockh;
#else
  (
    char                *date,
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
    long int        date_len;

    static   month_days[2][13] = {
        { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }, /* normal years */
        { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }  /* leap years */
    };

    SET_STATUS(&st, error_status_ok);  

    date_len = strlen(date);
  
        /* catch the "live forever" constant */
    if (strcmp(date,ek_null_date) == 0) {
        *clockh = ek_live_forever;
        return st;
    }

    /* catch none date */
    if (strcmp(date,"none") == 0) {
        *clockh = 0;
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
    /* set isdst to be -1 so the sepcific time can be determined for DST or not */
    /* using localtime isdst information can only reflect if current time is    */
    /* DST or not which causes problem for time pass the the changing date */
    tm.tm_isdst = -1; /*ptm->tm_isdst;*/
    *clockh = mktime (&tm);
    
    return st;
}

/* FUNCTION period_to_dt_string
 *
 *  takes a sec_timeval_period_t and converts it to a string of the form 
 *  ##w##d##h##m##s.  The buffer for the string is provided by
 *  the caller and is assumed to be large enough to hold the converted
 *  null-terminated date-time string.
 */
PUBLIC void period_to_dt_string
#ifndef __STDC__
    (secs, dt_str) 
    sec_timeval_period_t secs;
    char *dt_str;
#else
  (
    sec_timeval_period_t secs,
    char *dt_str
  )
#endif
{
    char units[BUFSIZE];
    int weeks, days, hours, minutes, seconds;

    dt_str[0] = '\0';

    if (secs == 0) 
        sprintf(dt_str, "forever");
    else {
        /* Convert to units */
        weeks = (secs/SECS_IN_A_WEEK);
        secs -= (weeks * SECS_IN_A_WEEK);
    
        days = (secs/SECS_IN_A_DAY);
        secs -= (days * SECS_IN_A_DAY);
    
        hours = (secs/SECS_IN_AN_HOUR);
        secs -= (hours * SECS_IN_AN_HOUR);
    
        minutes = (secs/SECS_IN_A_MINUTE);
        seconds = secs - (minutes * SECS_IN_A_MINUTE);
    
        /* Only print non-zero data */
        if (weeks) {
            sprintf(units, "%dw", weeks);
            strcat(dt_str, units);
        }
        if (days) {
            sprintf(units, "%dd", days);
            strcat(dt_str, (char *)units);
        }
        if (hours) {
            sprintf(units, "%dh", hours);
            strcat(dt_str, (char *)units);
        }
        if (minutes) {
            sprintf(units, "%dm", minutes);
            strcat(dt_str, (char *)units);
        }
        if (seconds) {
            sprintf(units, "%ds", seconds);
            strcat(dt_str, (char *)units);
        }
    }

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

/* FUNCTION dt_to_period
 *
 * Checks that the lifespan is either ek_lifespan_infinite or a string
 * in valid lifespan format (#w#d#h#m#s), then converts it to clock
 * units and returns the result in lifespan_p.
 * 
 * returns:
 *  ek_err_bad_lifespan          - if the lifespan isn't a valid formatted str
 *  ek_err_lifespan_out_of_range - if the lifespan is outside the legal range
 */
PUBLIC error_status_t dt_to_period
#ifndef __STDC__
    ( lifespan_str, lifespan_p ) 
    char                  *lifespan_str;
    sec_timeval_period_t  *lifespan_p;
#else
  (
    char                  *lifespan_str,
    sec_timeval_period_t  *lifespan_p
  )
#endif
{
    int             lifespan, time;
    char            *tmpc;
    error_status_t  st;
    long            lifespan_len;
    
    SET_STATUS(&st, error_status_ok);
    lifespan = 0;

    lifespan_len = strlen(lifespan_str);

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
                    time = time * SECS_IN_A_WEEK;
                    break;
                case 'D':
                case 'd':
                    time = time * SECS_IN_A_DAY;
                    break;
                case 'H':
                case 'h':
                    time = time * SECS_IN_AN_HOUR;
                    break;
                case 'M':
                case 'm':
                    time = time * SECS_IN_A_MINUTE;
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

    
PRIVATE void deif_permset_encode
#ifndef __STDC__
    (perms, string, printstrings, num_strings)
    sec_acl_permset_t   perms;
    char                *string;
    sec_acl_printstring_t printstrings[];
    unsigned32          num_strings;
#else
  (
    sec_acl_permset_t   perms,
    char                *string,
    sec_acl_printstring_t printstrings[],
    unsigned32          num_strings
  )
#endif
{
    int i;
    char tmp_string[BUFSIZE];

    for (i = 0; i < num_strings; i++) {
        if (printstrings[i].permissions & perms)
            sprintf(tmp_string, "%s", printstrings[i].printstring);
        else
            sprintf(tmp_string, "-");
        strcat(string, (char *)tmp_string);
    }
}

PUBLIC void svg_uid_to_foreign_name
#ifndef __STDC__
    (uid, foreign_name, st)
    uuid_t              *uid;
    sec_rgy_name_t      foreign_name;
    error_status_t      *st;
#else
  (
    uuid_t              *uid,
    sec_rgy_name_t      foreign_name,
    error_status_t      *st
  )
#endif
{
    unsigned_char_p_t   name=NULL;

    rsdb_pgo_uid_to_name(sec_rgy_domain_person, uid, foreign_name, st);
    if (BAD_STATUS(st)) {
        CLEAR_STATUS(st);
	uuid_to_string(uid, &name, st);
	if (GOOD_STATUS(st)) {
	    strcpy((char *)foreign_name, (char *)name);
 	    if (name)
	        rpc_string_free(&name, st);
	}
	else
	    strcpy((char *)foreign_name, "<ILLEGAL UUID>");
    }
    else
        rsdb_name_util_convert_to_cell(foreign_name, st); 
}

PUBLIC void svg_foreign_name_to_uid
#ifndef __STDC__
    (uid, foreign_name, st)
    uuid_t              *uid;
    sec_rgy_name_t      foreign_name;
    error_status_t      *st;
#else
  (
    uuid_t              *uid,
    sec_rgy_name_t      foreign_name,
    error_status_t      *st
  )
#endif
{
    sec_rgy_name_t      name;
    boolean32           islocal;

    rsdb_name_util_from_global(foreign_name, name, &islocal, st);
    rsdb_pgo_name_to_uid(sec_rgy_domain_person, name, uid, st);
    if (BAD_STATUS(st)) {
        CLEAR_STATUS(st);
	uuid_from_string((unsigned_char_t *) foreign_name, uid, st);
	if (BAD_STATUS(st))
	    SET_STATUS(st, deif_err_invalid_entry_key);
    }
}

PUBLIC boolean32 rsdb_aclmgr_str_to_type
  (
    char                    *entry_string,
    rsdb_acl_mgr_type_t     *acl_mgr_type
  )
{
    if ( strcmp(entry_string, "policy") == 0 )
        *acl_mgr_type = rsdb_acl_mgr_type_rgy;
    else
    if ( strcmp(entry_string, "principal") == 0 )
        *acl_mgr_type = rsdb_acl_mgr_type_person;
    else
    if ( strcmp(entry_string, "group") == 0 )
        *acl_mgr_type = rsdb_acl_mgr_type_group;
    else
    if ( strcmp(entry_string, "organization") == 0 )
        *acl_mgr_type = rsdb_acl_mgr_type_org;
    else
            return false;
    return true;
}

PUBLIC boolean32 rsdb_aclmgr_type_to_str
  (
    rsdb_acl_mgr_type_t     acl_mgr_type,
    char                    *entry_string
  )
{
    switch(acl_mgr_type) {
        case rsdb_acl_mgr_type_rgy:
            sprintf(entry_string, "policy");
            break;
        case rsdb_acl_mgr_type_person:
            sprintf(entry_string, "principal");
            break;
        case rsdb_acl_mgr_type_group:
            sprintf(entry_string, "group");
            break;
        case rsdb_acl_mgr_type_org:
            sprintf(entry_string, "organization");
            break;
        default:
            return false;
        }
    return true;
}

PUBLIC void rsdb_attr_aclmgr_to_str
  (
    sec_acl_permset_t         perms,
    char                      *perm_string,
    sec_acl_printstring_t     printstrings[],
    unsigned32                num_pstrings
  )
{
    *perm_string = '\0';
    deif_permset_encode(perms, perm_string, printstrings, num_pstrings);
}

PUBLIC void rsdb_acl_entry_to_str
#ifndef __STDC__
    (cell_is_foreign, acl_entry, entry_string, 
          printstrings, num_pstrings)
    boolean32           cell_is_foreign;
    rsdb_acl_entry_t    acl_entry;
    char                *entry_string;
    sec_acl_printstring_t printstrings[];
    unsigned32          num_pstrings;
#else
  (
    boolean32           cell_is_foreign,
    rsdb_acl_entry_t    acl_entry,
    char                *entry_string,
    sec_acl_printstring_t printstrings[],
    unsigned32          num_pstrings
  )
#endif
{
    sec_rgy_name_t      global_name;
    long                name_len;
    sec_rgy_handle_t    context;
    sec_rgy_cursor_t    csr;
    sec_rgy_pgo_item_t  item;
    error_status_t      lstat;
    unsigned_char_p_t   name1 = NULL, name2 = NULL;
    uuid_t              dummy_uuid;
    boolean32           free_name1 = false;
    boolean32           free_name2 = false;
    rsdb_credentials_t  creds;
    char                new_string[BUFSIZE];
    sec_rgy_name_t      foreign_name;
    sec_rgy_name_t      foreign_realm;

    CLEAR_STATUS(&lstat);


    /* Output the entry type, and associated data if necessary */
    switch(acl_entry.entry_type) {

        /* These entry types have only a permset associated with them */
        case sec_acl_e_type_mask_obj:
            sprintf(entry_string, "mask_obj:");
            break;
        case sec_acl_e_type_user_obj:
            sprintf(entry_string, "user_obj:");
            break;
        case sec_acl_e_type_group_obj:
            sprintf(entry_string, "group_obj:");
            break;
        case sec_acl_e_type_other_obj:
            sprintf(entry_string, "other_obj:");
            break;
        case sec_acl_e_type_any_other:
            sprintf(entry_string, "any_other:");
            break;
        case sec_acl_e_type_unauthenticated:
            sprintf(entry_string, "unauthenticated:");
            break;

        /* these entry types have an id_t associated with them */
        case sec_acl_e_type_user:
            rsdb_get_credentials_by_uid(rsdb_person_db, 
                   &acl_entry.entry_info.id.uuid, &creds);
            rsdb_util_path_construct(sec_rgy_domain_person, creds.id, 
                   global_name, &name_len, &lstat);
            if (GOOD_STATUS(&lstat)) {
                name1 = global_name;
            } else {
                uuid_to_string(&acl_entry.entry_info.id.uuid,
                            &name1, &lstat);
                free_name1 = true;
            }
            sprintf(entry_string, "user:%s:", (char *) name1);
            break;

        case sec_acl_e_type_group:
            rsdb_get_credentials_by_uid(rsdb_group_db, 
                   &acl_entry.entry_info.id.uuid, &creds);
            rsdb_util_path_construct(sec_rgy_domain_group, creds.id, 
                   global_name, &name_len, &lstat);
            if (GOOD_STATUS(&lstat)) {
                name1 = global_name;
            } else {
                uuid_to_string(&acl_entry.entry_info.id.uuid,
                            &name1, &lstat);
                free_name1 = true;
            }
            sprintf(entry_string, "group:%s:", (char *) name1);
            break;

        case sec_acl_e_type_foreign_other:
	    svg_uid_to_foreign_name(&acl_entry.entry_info.id.uuid, 
				    foreign_name, &lstat);
            sprintf(entry_string, "foreign_other:%s:", foreign_name);
            break;

        /* these entry types have a foreign_id_t associated with them */
        case sec_acl_e_type_foreign_user:
	    svg_uid_to_foreign_name(&acl_entry.entry_info.foreign_id.id.uuid,
				    foreign_name, &lstat);
	    svg_uid_to_foreign_name(&acl_entry.entry_info.foreign_id.realm.uuid,
				    foreign_realm, &lstat);
            sprintf(entry_string, "foreign_user:%s/%s:", foreign_realm, 
		    foreign_name);
            break;

        case sec_acl_e_type_foreign_group:
	    svg_uid_to_foreign_name(&acl_entry.entry_info.foreign_id.id.uuid,
				    foreign_name, &lstat);
	    svg_uid_to_foreign_name(&acl_entry.entry_info.foreign_id.realm.uuid,
				    foreign_realm, &lstat);
            sprintf(entry_string, "foreign_group:%s/%s:", foreign_realm, 
		    foreign_name);
            break;

        default:
            break;
    } /* switch entry_type */

    /* get the permset */
    new_string[0] = '\0';
    deif_permset_encode(acl_entry.perms, new_string, printstrings,
              num_pstrings);
    strcat(entry_string, (char *) new_string);

    if (free_name1 && name1)
        rpc_string_free(&name1, &lstat);
    if (free_name2 && name2)
        rpc_string_free(&name2, &lstat);
}

PRIVATE void svg_util_parse_foreign_id
#ifndef __STDC__
    (string, string1, string2)
    unsigned char *string;
    unsigned char *string1;
    unsigned char *string2;
#else
  (
    unsigned char *string,
    unsigned char *string1,
    unsigned char *string2
  )
#endif
{
    unsigned char *ptr;

    ptr = (unsigned char *) strrchr((char *)string, '/');
    if (*ptr != NULL) {
        *ptr = '\0';
        strcpy((char *) string1, (char *) string);
        ptr++;
        strcpy((char *) string2, (char *) ptr);
    }
    else
        strcpy((char *) string1, (char *) string);

}

/*
 * m a k e _ l o w e r c a s e
 */
PRIVATE void make_lowercase
#ifndef __STDC__
    (buffer)
    char *buffer;
#else
  (
    char *buffer
  )
#endif
{
    while (*buffer != '\0') {
        if (isupper(*buffer)) *buffer = tolower(*buffer);
    *buffer++;
    }
}

/*
 * d e i f _ g e t _ k e y
 */
PRIVATE error_status_t deif_get_key
#ifndef __STDC__
    (acl_entry_p, entry_type, string)
    rsdb_acl_entry_t        *acl_entry_p;
    sec_acl_entry_type_t    entry_type;
    char                    *string;
#else
  (
    rsdb_acl_entry_t        *acl_entry_p,
    sec_acl_entry_type_t    entry_type,
    char                    *string
  )
#endif
{
    error_status_t      st;
    sec_rgy_name_t      person_string, realm_string;
    sec_rgy_name_t name_in, name_out;
    rsdb_named_item_t item;
    long in_len, out_len;
    rsdb_credentials_t creds;
    sec_rgy_name_t      foreign_name;

    CLEAR_STATUS(&st);

    /* The empty string is not a valid key */
    if (*string == '\0') {
	SET_STATUS(&st, deif_err_invalid_entry_key);
    } else switch(entry_type) {
        case sec_acl_e_type_user:            
            strcpy((char *) name_in, string);
            in_len = strlen((char *) name_in);
            rsdb_util_path_resolve(sec_rgy_domain_person, name_in, &in_len,
                    name_out, &out_len, &item, &st);
            if (GOOD_STATUS(&st)) {
                if (item.type == rsdb_pgo_type) {
                    rsdb_get_credentials_by_unix_id(rsdb_person_db, 
                            (RSDB_PGO(&item)).unix_id, &creds);
                    acl_entry_p->entry_info.id.uuid = creds.uid;
                }
                else 
                    SET_STATUS(&st, deif_err_invalid_entry_key);
            }
            else {
                uuid_from_string((unsigned_char_t *) string, 
                        &acl_entry_p->entry_info.id.uuid, &st);
                if (st != uuid_s_ok)
                    SET_STATUS(&st, deif_err_invalid_entry_key);
            }
            break;

        case sec_acl_e_type_group:           
            strcpy((char *) name_in, string);
            in_len = strlen((char *) name_in);
            rsdb_util_path_resolve(sec_rgy_domain_group, name_in, &in_len,
                    name_out, &out_len, &item, &st);
            if (GOOD_STATUS(&st)) {
                if (item.type == rsdb_pgo_type) {
                    rsdb_get_credentials_by_unix_id(rsdb_group_db, 
                            (RSDB_PGO(&item)).unix_id, &creds);
                    acl_entry_p->entry_info.id.uuid = creds.uid;
                }
                else 
                    SET_STATUS(&st, deif_err_invalid_entry_key);
            }
            else {
                uuid_from_string((unsigned_char_t *) string, 
                        &acl_entry_p->entry_info.id.uuid, &st);
                if (st != uuid_s_ok)
                    SET_STATUS(&st, deif_err_invalid_entry_key);
            }
            break;

        case sec_acl_e_type_foreign_other:   
	    strcpy ((char *)foreign_name, string);
	    svg_foreign_name_to_uid (&acl_entry_p->entry_info.id.uuid,
				     foreign_name, &st);
	    break;

        case sec_acl_e_type_foreign_user:    
        case sec_acl_e_type_foreign_group:
            svg_util_parse_foreign_id((unsigned char *) string, realm_string, 
                                      person_string);

	    strcpy ((char *)foreign_name, (char *)realm_string);
	    svg_foreign_name_to_uid (&acl_entry_p->entry_info.foreign_id.realm.uuid,
				     foreign_name, &st);

	    strcpy ((char *)foreign_name, (char *)person_string);
	    svg_foreign_name_to_uid (&acl_entry_p->entry_info.foreign_id.id.uuid,
				     foreign_name, &st);
            break;

        default:
            SET_STATUS(&st, deif_err_invalid_entry_type);
    }
    return st;
}

PUBLIC void rsdb_attr_str_to_aclmgr
  (
    char                *string,
    sec_acl_permset_t   *permset_p,
    sec_acl_printstring_t printstrings[],
    unsigned32          num_pstrings,
    error_status_t      *status
  )
{
    boolean32       found;
    int             i;

    /*
     *  Translate the permission string into a permset_t.
     *  For each permission character entered, compare against
     *  each possible printstring, for a match.  If found, add
     *  the permission bit for that printstring to the permset.
     */

    *permset_p = 0;

    while((GOOD_STATUS(status)) && (*string != '\0')) {
        found = false;
        for (i = 0; i < num_pstrings && !found; i++)
            if (*string == *printstrings[i].printstring) {
                found = true;
                *permset_p = *permset_p |
                                        printstrings[i].permissions;
            }
        if ((! found) && (*string != '-')) {
            SET_STATUS(status, deif_err_invalid_perm);
        }
        string++;
    }
}



PUBLIC void rsdb_str_to_acl_entry
#ifndef __STDC__
    (string, acl_entry, cell_is_foreign, 
     printstrings, num_pstrings, status)
    char                *string;
    rsdb_acl_entry_t    *acl_entry;
    boolean32           cell_is_foreign;
    sec_acl_printstring_t printstrings[];
    unsigned32          num_pstrings;
    error_status_t      *status;
#else
  (
    char                *string,
    rsdb_acl_entry_t    *acl_entry,
    boolean32           cell_is_foreign,
    sec_acl_printstring_t printstrings[],
    unsigned32          num_pstrings,
    error_status_t      *status
  )
#endif
{
    boolean32       found;
    int             i;
    char            *str1, *str2;

    /* Make sure the string is NULL terminated only (no newline) */
    i = strlen(string);
    if (i > 0)
        if (string[i - 1] == '\n')
            string[i - 1] = '\0';

    acl_entry->perms = 0;
    SET_STATUS(status, error_status_ok);

    /* Parse the entry type */
    str1 = strchr(string, ':');

    /* If no ':' was found, the string cannot be a full sec_acl_entry */
    if (str1 == NULL)
        SET_STATUS(status, deif_err_invalid_entry);
    else {
        if (str1 != NULL)
            if (*str1 == ':') {         /* Get rid of :'s               */
                *str1 = '\0';           /* Terminate the entry_type str */
                str1++;                 /* save the rest of the string  */
            }
        make_lowercase(string);
        found = false;
        for (i = 0; (entry_types[i].name != NULL) && (! found); i++)
            if (strcmp(entry_types[i].name, string) == 0) {
                found = true;
                acl_entry->entry_type = entry_types[i].entry_type;
            }
        if (! found)
            SET_STATUS(status, deif_err_invalid_entry_type);
    }

    /*  Depending on the type, there may be a KEY expected. */
    if (GOOD_STATUS(status)) {
        switch (acl_entry->entry_type) {
            case sec_acl_e_type_mask_obj:           /* No key for these */
            case sec_acl_e_type_unauthenticated: 
            case sec_acl_e_type_user_obj:
            case sec_acl_e_type_other_obj:       
            case sec_acl_e_type_group_obj:       
            case sec_acl_e_type_any_other:       
                break;
            case sec_acl_e_type_user:            
            case sec_acl_e_type_group:           
            case sec_acl_e_type_foreign_other:   
            case sec_acl_e_type_foreign_user:    
            case sec_acl_e_type_foreign_group:   
                /* There should be a key, so look for another colon */
                string = str1;
                str1 = strchr(string, ':');
                if (str1 == NULL)
                    SET_STATUS(status, deif_err_invalid_entry);
                else {              /* Otherwise translate the key */

                    /* if there was a colon, terminate the key */
                    if (str1 != NULL) {
                        *str1 = '\0'; str1++;
                    }
                    *status = deif_get_key(acl_entry, acl_entry->entry_type, string);

                    /* If invalid key, un-truncate the string that was passed
                     * in so the caller can see the bad key.
                     */
                    if (! GOOD_STATUS(status))
                        *(string - 1) = ':';
                }
                break;                         
            case sec_acl_e_type_extended:        
                SET_STATUS(status, deif_err_invalid_entry_type);
                break;                         
            default:
                SET_STATUS(status, deif_err_invalid_entry_type);
        }
    }

    /*
     *  Translate the permission string into a permset_t.
     *  For each permission character entered, compare against
     *  each possible printstring, for a match.  If found, add
     *  the permission bit for that printstring to the permset.
     */

    while((GOOD_STATUS(status)) && (*str1 != '\0')) {
        found = false;
        for (i = 0; i < num_pstrings && !found; i++)
            if (*str1 == *printstrings[i].printstring) {
                found = true;
                acl_entry->perms = acl_entry->perms |
                                        printstrings[i].permissions;
            }
        if ((! found) && (*str1 != '-')) {
            SET_STATUS(status, deif_err_invalid_perm);
        }
        str1++;
    }
}


/*
 * s v g _ r s _ a t t r _ u p d a t e _ n o _ a u t h _ c h e c k
 *
 */
PUBLIC void svg_rs_attr_update_no_auth_check
(
    char			*component_name,	/* [in] */
    unsigned32			num_to_write,	/* [in] */
    sec_attr_t			in_attrs[],	/* [in] */
    signed32			*failure_index,	/* [out] */
    error_status_t		*st_p
)
{
    rsdb_util_rgy_obj_info_t	rgy_obj_info;
    uuid_t			obj_uuid;
    rsdb_attr_sch_list_t        *sch_list = NULL;
    signed32			*xref = NULL;
    signed32			i;

    CLEAR_STATUS(st_p);

    if (num_to_write == 0) {
	return;
    }

    rsdb_util_get_obj_info_by_name(component_name, rsdb_pgo_type,
		sec_acl_type_object, &rgy_obj_info, &obj_uuid, st_p);
    if(STATUS_OK(st_p)) {
	rsdb_attr_sch_get_entries((sec_attr_component_name_t)"",
		num_to_write, in_attrs, &rgy_obj_info, &sch_list, &xref,
		failure_index, st_p);
    }

    /* Perform sematic check of input attributes */
    if(STATUS_OK(st_p)) {
	rsdb_attr_semantic_check(&rgy_obj_info, num_to_write,
		in_attrs, sch_list, xref, failure_index, st_p);
    }
    
    if(STATUS_OK(st_p)) {
	rsdb_attr_update(&rgy_obj_info, num_to_write, in_attrs,
		sch_list, xref, failure_index, st_p);
    } 

    if(sch_list) {
	/* free internal ptrs of each schema_entry allocated */
	for(i = 0; i < sch_list->count; i++) {
	    sec_attr_util_sch_ent_free_ptrs(&(sch_list->entries[i].sch_entry));
	}
	free(sch_list);
    }
    if(xref)
	free(xref);

    /* map non-attr status codes to sec_attr codes */
    rs_attr_util_map_status(st_p);

    return;
}






