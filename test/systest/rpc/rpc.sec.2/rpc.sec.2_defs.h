/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpc.sec.2_defs.h,v $
 * Revision 1.1.77.2  1996/02/18  23:07:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:46  marty]
 *
 * Revision 1.1.77.1  1995/12/11  22:43:54  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/04  14:58 UTC  bissen
 * 	merge from HPDCE01
 * 	[1995/12/11  22:20:37  root]
 * 
 * Revision 1.1.75.1  1993/12/22  20:48:04  mhickey
 * 	no changes, different backing tree
 * 	[1993/12/22  20:44:30  mhickey]
 * 
 * 	no
 * 
 * 	Added macros to convert authz, authn, and protection level
 * 	numbers to strings, and added the typedef for the data
 * 	structure that identifies a thread.
 * 	[1993/12/22  18:35:20  mhickey]
 * 
 * Revision 1.1.73.1  1993/09/09  19:12:51  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/09  19:11:48  cmckeen]
 * 
 * Revision 1.1.1.5  1993/05/04  22:22:12  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.2.5  1993/03/12  04:07:04  mhickey
 * 	Final checkin for comment fixes before final submit
 * 	[1993/03/12  04:00:10  mhickey]
 * 
 * Revision 1.1.2.4  1993/02/25  20:16:12  mhickey
 * 	Fixing bcs problem
 * 	[1993/02/25  20:02:22  mhickey]
 * 
 * Revision 1.1.2.3  1993/02/05  15:22:33  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:43:28  cjd]
 * 
 * Revision 1.1.4.2  1993/02/04  22:12:16  cjd
 * 	Embedded copyright notice
 * 
 * Revision 1.1.2.2  1992/12/03  19:54:35  mhickey
 * 	Fixed error in logic for TVAL_SETMIN and TVAL _SETMAX,
 * 	added rpcsec2_cnf_other return code.
 * 
 * 	Defect 5887.
 * 	[1992/12/03  17:51:03  mhickey]
 * 
 * 	  Reorged into server defines and client defines, added
 * 	  return values for server auth errors, added keytab
 * 	  defs, moved UUID_BASE_ALLOC def into this file from
 * 	  test_rpc_smain.c, and changed all names to begin
 * 	  with tstrpc, not testrpc.
 * 	[1992/10/16  13:35:02  mhickey]
 * 
 * 	Added definitions for return values for tstrpc_cnf_str,
 * 	and to support the tstrpc_check_ret() function.
 * 	[1992/10/10  03:12:48  mhickey]
 * 
 * 	Initial checkin for base for rpc.security.2
 * 	[1992/10/06  17:29:42  mhickey]
 * 
 * $EndLog$
 */

#include <pthread.h>
#include <err_track.h>    /*---* error tracking data structs *---*/
#include <ctime_nonl.h>   /*---* macro for non-newline terminated ctime *---*/
#include <rdconf.h>       /*---* return values from rdconf *---*/

/*---------------------------------------------------------*
 *---------------------------------------------------------*
 *---*            General defines                      *---*
 *---------------------------------------------------------*
 *---------------------------------------------------------*/
/*---* 
 *---* default configuration file name 
 *---*/
#ifndef RPCSEC2_CONF_FN
#  define RPCSEC2_CONF_FN "rpc.sec.2.conf"
#endif /*---* ifndef RPCSEC2_CONF_FN *---*/

/*---*
 *---* Macros for accessing the elements of the params array. of values 
 *---* returned from rd_conf().
 *---* Note that to use these, the caller code must declare an enum variable 
 *---* named param_i, the elements of which correspond to the values being 
 *---* read from the config file.  For example, if the specifiers for the 
 *---* values being read from the config file are "val1", "val2", "val3",
 *---* the caller code would declare 
 *---*   enum {val1, val2, val3} param_i;
 *---*/
#define PSPEC(parray,index) (parray[(param_i=index)].param_spec)
#define PVAL(parray,index) (parray[(param_i=index)].param_val)
/*---* 
 *---* macro to convert parameter from a string to a integer and return 
 *---* 0 = success in stat, or print a message and return 1 = failure in stat
 *---*/ 
#define int_from_PVAL(var_p,parray,index,desc_str,stat) { \
           if (!isdigit(*(PVAL(parray, index)))) \
           { \
              printf( \
               " Server: non-numeric value (%s) for %s in conf file.\n", \
                                       PVAL(parray, index), desc_str); \
              stat=1; \
           } \
           else \
	   { \
              sscanf(PVAL(parray, index), "%d", var_p); \
              stat=0; \
	   } \
	}



/*----------------------------------------*
 *---*     Data Type Management       *---*
 *----------------------------------------*/
/*---* 
 *---* entry type for array of threads we manage.  The label is a string that
 *---* describes the thread in reports, etc.
 *---*/
typedef struct {
   pthread_t *thd_id;
   char      *thd_label;
} thd_tbl_entry_t, thd_tbl_entry_p_t;

/*---*
 *---* USE_PTHREAD(thd_tbl, thd_arry, thd_num) - assign the thread in position
 *---*                                           thd_num in the thd_arry to 
 *---*                                           the pointer part of the struct
 *---*                                           in position thd_num in the
 *---*                                           thd_tbl.
 *---*/
#define USE_PTHREAD(thd_tbl, thd_arry, thd_num) \
	 thd_tbl[thd_num].thd_id = &(thd_arry[thd_num]);

/*-----------------------------------------------*
 *---* Macros for operations on timeval structs
 *-----------------------------------------------*/
/*---* 
 *---* TVAL_SETMIN(min, new) takes two timeval structs as args, and sets the
 *---*                       first to equal the second if the second is less
 *---*                       than the first.
 *---*/
#define  TVAL_SETMIN(min, new) \
    (min) = ((min).tv_sec || (min).tv_nsec)     ? \
            (((min).tv_sec  > (new).tv_sec )? new :  \
             (((min).tv_sec == (new).tv_sec && (min).tv_nsec > (new).tv_nsec)?\
               new : min)) : new

/*---*
 *---* TVAL_SETMAX (max, new) same as above, but sets first arg equal to second
 *---*                        if second arg is greater than first arg.
 *---*/
#define  TVAL_SETMAX(max, new) \
  (max) = ((max).tv_sec < (new).tv_sec)? new: \
            (((max).tv_sec == (new).tv_sec && (max).tv_nsec < (new).tv_nsec)?\
              new: max)

/*---*
 *---* TVAL_SUB(t1,t2) subtracts timeval struct t2 from timeval struct t1, 
 *---*                 result is in timeval struct t1.
 *---*/
/*---* we define NSEC_PER_SEC  to avoid a call to pow(3m). *---*/
#define  NSECS_PER_SEC 0x3b9aca00
#define  TVAL_SUB(t1, t2) \
    {\
       int ___secadj = 0;\
       (t1).tv_nsec = \
                ((t1).tv_nsec + ((___secadj = ((t1).tv_nsec < (t2).tv_nsec)) ?\
                                   (long)NSECS_PER_SEC: 0)) - (t2).tv_nsec;\
       t1.tv_sec  = \
                ((t1).tv_sec - ___secadj) - (t2).tv_sec;\
    }

/*---* 
 *---* TVAL_ADD(t1, t2) Adds timeval struct t2 to timeval struct t1, result
 *---*                  is returned in timeval struct t1.
 *---*/
#define  TVAL_ADD(t1, t2) \
    {\
       long ___secadj = 0, ___nsum = (t1).tv_nsec + (t2).tv_nsec;\
       (t1).tv_nsec = \
          (___secadj = (___nsum > NSECS_PER_SEC)) ? (___nsum - NSECS_PER_SEC):\
          ___nsum;\
       (t1).tv_sec  =  (t1).tv_sec + (t2).tv_sec + ___secadj;\
    }
  

/*--------------------------------------------*
 *---*    Protection Level Translation    *---*
 *--------------------------------------------*
 *---*
 *---* SET_PROT_LEVEL(prot_str, prot_val, who)
 *---*     
 *---*     Assigns the constant value for the protection level indicated by
 *---*     prot_str to the variable prot_val.  Uses the string who to identify
 *---*     the caller if the prot_str value is not recognized.
 *---*/
#define SET_PROT_LEVEL(prot_str, prot_val, who) \
   if (!strcmp(prot_str, "none")) \
      prot_val = rpc_c_protect_level_none; \
   else if (!strcmp(prot_str, "conn")) \
      prot_val = rpc_c_protect_level_connect; \
   else if (!strcmp(prot_str, "call")) \
      prot_val = rpc_c_protect_level_call; \
   else if (!strcmp(prot_str, "pkt")) \
      prot_val = rpc_c_protect_level_pkt; \
   else if (!strcmp(prot_str, "integ")) \
      prot_val = rpc_c_protect_level_pkt_integ; \
   else if (!strcmp(prot_str, "priv")) \
      prot_val = rpc_c_protect_level_pkt_privacy; \
   else \
   { \
      printf(" %s: protection level %s unknown.\n", who, prot_str); \
      printf(" %s: %s\n", who, usage); \
      exit (1); \
   } 
/*---*
 *---*
 *---* SET_PROT_LEVEL_STR(prot_str, prot_val, who)
 *---*     
 *---*     Assigns the string value for the protection level indicated by
 *---*     prot_val to the variable prot_str.  Uses the string who to identify
 *---*     the caller if the prot_val value is not recognized.
 *---*/
#define SET_PROT_LEVEL_STR(prot_str, prot_val, who) \
   if (prot_val == rpc_c_protect_level_none) \
      prot_str = "none"; \
   else if (prot_val == rpc_c_protect_level_connect) \
      prot_str = "conn"; \
   else if (prot_val == rpc_c_protect_level_call) \
      prot_str = "call"; \
   else if (prot_val == rpc_c_protect_level_pkt) \
      prot_str = "pkt"; \
   else if (prot_val == rpc_c_protect_level_pkt_integ) \
      prot_str = "integ"; \
   else if (prot_val == rpc_c_protect_level_pkt_privacy) \
      prot_str = "priv"; \
   else \
   { \
      printf(" %s: protection level %d unknown.\n", who, prot_val); \
      printf(" %s: %s\n", who, usage); \
      exit (1); \
   } 

/*---*
 *---*
 *---* SET_AUTHN_SVC_STR(athn_str, athn_val, who)
 *---*     
 *---*     Assigns the string value for the authentication service indicated by
 *---*     athn_val to the variable athn_str.  Uses the string who to identify
 *---*     the caller if the athn_val value is not recognized.
 *---*/
#define SET_AUTHN_SVC_STR(athn_str, athn_val, who) \
   if (athn_val == rpc_c_authn_none) \
      athn_str = "none"; \
   else    if (athn_val == rpc_c_authn_dce_secret) \
      athn_str = "dce secret"; \
   else    if (athn_val == rpc_c_authn_dce_public) \
      athn_str = "dce public"; \
   else if (athn_val == rpc_c_authn_default) \
      athn_str = "default"; \
   else \
   { \
      printf(" %s: authentication service %d unknown.\n", who, athn_val); \
      printf(" %s: %s\n", who, usage); \
      exit (1); \
   } 

/*---*
 *---*
 *---* SET_AUTHZ_SVC_STR(athz_str, athz_val, who)
 *---*     
 *---*     Assigns the string value for the authorization service indicated by
 *---*     athz_val to the variable athz_str.  Uses the string who to identify
 *---*     the caller if the athz_val value is not recognized.
 *---*/
#define SET_AUTHZ_SVC_STR(athz_str, athz_val, who) \
   if (athz_val == rpc_c_authz_none) \
      athz_str = "none"; \
   else    if (athz_val == rpc_c_authz_name) \
      athz_str = "name based"; \
   else    if (athz_val == rpc_c_authz_dce) \
      athz_str = "DCE PAC based"; \
   else \
   { \
      printf(" %s: authorization service %d unknown.\n", who, athz_val); \
      printf(" %s: %s\n", who, usage); \
      exit (1); \
   } 


/*---------------------------------------------------------*
 *---------------------------------------------------------*
 *---*            Client Specific Defines              *---*
 *---------------------------------------------------------*
 *---------------------------------------------------------*/
/*---*
 *---* Max number of errors we will track
 *---*/

/*---*
 *---* Operation type defs
 *---*/
#define rpcsec2_op_is_nop 0
#define rpcsec2_op_is_cnf_str 1

/*---------------------------------------------------------*
 *---------------------------------------------------------*
 *---*             Server Specific Defines             *---*
 *---------------------------------------------------------*
 *---------------------------------------------------------*/

/*---------------------------------------------------------*
 *---* Server defines
 *---------------------------------------------------------*/
/*---*
 *---* Server initial UUID arrary size for checking namespace entry
 *---*/
#ifndef RPCSEC2_SRV_UUID_BASE_ALLOC
#   define RPCSEC2_SRV_UUID_BASE_ALLOC 16
#endif  /*---* ifndef RPCSEC2_SRV_UUID_BASE_ALLOC *---*/



/*---------------------------------------------------------*
 *---* Server Return value defines
 *---------------------------------------------------------*/
/*---*
 *---* Definitions for return values from rpcsec2_nop()
 *---*/
#define rpcsec2_nop_ok 0              /*---* no errors        *---*/
#define rpcsec2_nop_err_odd 1         /*---* odd arg passed   *---*/
#define rpcsec2_nop_err_oth 2         /*---* some other error *---*/


/*---*
 *---* Definitions for return values from rpcsec2_cnf_str()
 *---*/
#define rpcsec2_cnf_ok          0       /*---* no errors            *---*/
#define rpcsec2_cnf_null_array  1       /*---* array member is null *---*/
#define rpcsec2_cnf_bad_chksum  2       /*---* checksum error       *---*/
#define rpcsec2_cnf_null_struc  3       /*---* null struct pointer  *---*/
#define rpcsec2_cnf_no_id       4       /*---* no id in struct      *---*/
#define rpcsec2_cnf_no_size     5       /*---* no size in struct    *---*/
#define rpcsec2_cnf_err_other   6       /*---* something else wrong *---*/

/*---*
 *---* Definitions for return values from authorization validation routine
 *---*/
#define rpcsec2_srv_auth_accept          7  /*---* successful a13n *---*/
#define rpcsec2_srv_auth_bad_srv         8  /*---* bad server principle *---*/
#define rpcsec2_srv_auth_unsup_authz     9  /*---* unsupp. authzn srvice *---*/
#define rpcsec2_srv_auth_unsup_authn    10  /*---* unsupp. authnn srvice *---*/
#define rpcsec2_srv_auth_bad_cli        11  /*---* bad client principal  *---*/
#define rpcsec2_srv_auth_unsup_lvl      12  /*---* unsupp protectn level *---*/
#define rpcsec2_srv_auth_cant_get_info  13  /*---* can't get auth info *---*/

