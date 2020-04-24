/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_login_reset.c,v $
 * Revision 1.1.7.3  1996/02/18  00:18:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:56  marty]
 *
 * Revision 1.1.7.2  1995/12/08  17:51:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:20:35  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:11  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:26  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/26  20:01:00  hondo
 * 	changes to support propagation of login activity.
 * 	[1994/10/25  14:58:37  hondo]
 * 
 * 	changes for replication of login activity.
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */

/*
 *     Registry Server - Login Activity Log operations
 */

#include <rs_log.h>
#include <rsdb_attr_pvt.h>     
#include <dce/rs_login_base.h>   
#include <dce/rgynbase.h>              
#include <rsdb_login.h>         

#include <sec_attr_tools.h> 

/*
 * Local types 
 */
typedef struct rs_log_login_r_t {
       unsigned32             last_seqno;
       sec_timeval_sec_t      dis_time;
       unsigned32             cur_inv_attempts;
       sec_timeval_sec_t      last_bad_time;
       sec_timeval_sec_t      last_good_time;
       unsigned32	      pname_len;
       unsigned32	      gname_len;
       unsigned32	      oname_len;
} rs_log_login_r_t;


typedef struct rs_log_login_reset_update_t {
    rs_log_hdr_t		    upd_hdr;
    rs_log_login_r_t	            login_part;
} rs_log_login_reset_update_t;



PRIVATE PROP_FUNCTION(rs_log_login_reset_prop_update);
PRIVATE REPLAY_FUNCTION(rs_log_login_reset_replay_update);

/*
 * Define Module opcodes and EPV
 */
#define rs_log_login_reset_update_op	0
#define rs_log_login_reset_last		1 /* The number of operations */

static rs_log_ops_t rs_log_login_reset_epv[rs_log_login_reset_last] = {

    {	GLOBAL_UPDATE,
	rs_log_login_reset_prop_update,
	rs_log_login_reset_replay_update,
	"update" },
};

PUBLIC void
rs_log_login_reset_init(error_status_t  *st)
{
    int num_ops = rs_log_login_reset_last;
    rs_log_init_module(RS_LOG_MODULE_LOGINR, num_ops, rs_log_login_reset_epv, st);
}

PRIVATE boolean32 import_login_reset(
    sec_rgy_login_name_t        *login_name,       /* [in] */
    sec_rgy_login_activity_t  *login_part,       /* [in] */
    sec_timeval_sec_t           *time_now,         /* [in] */
    unsigned32                  *size,
    rs_log_login_reset_update_t    **data		   /* [out] */
)
{
    char                        *p;
    unsigned32                  pname_len;
    unsigned32                  gname_len;
    unsigned32                  oname_len;
    unsigned32                  fixed_len;
    rs_log_login_r_t         *item;

    /* Fixed portion */
    fixed_len=(sizeof(rs_log_login_r_t) + sizeof(rs_log_hdr_t));
    *size=fixed_len;

    /* Variable portion */
    pname_len   = strlen( (char *) login_name->pname);
    gname_len   = strlen( (char *) login_name->gname);
    oname_len   = strlen( (char *) login_name->oname);

    *size   += pname_len;
    *size   += gname_len;
    *size   += oname_len;



    if (time_now != NULL) {
        *size += sizeof(*time_now);
    }

    if ((*data = (rs_log_login_reset_update_t *) malloc(*size)) ==NULL)
	return(1);

    item = &((*data)->login_part);
    p = (char *) (((unsigned32) *data) + fixed_len);


    /* Fixed portion */
    item->last_seqno          = 0;
    item->dis_time            = login_part->dis_time;
    item->cur_inv_attempts    = login_part->cur_inv_attempts;
    item->last_bad_time       = login_part->last_bad_time;
    item->last_good_time      = login_part->last_good_time;

    item->pname_len           = pname_len;
    item->gname_len           = gname_len;
    item->oname_len           = oname_len;


    bcopy( (Pointer) login_name->pname, p, item->pname_len);
    p       += item->pname_len;

    bcopy( (Pointer) login_name->gname, p, item->gname_len);
    p       += item->gname_len;

    bcopy( (Pointer) login_name->oname, p, item->oname_len);
    p       += item->oname_len;

    if (time_now != NULL) {
        memcpy (p, (char *) time_now, sizeof(*time_now));
    }

    return(0);
	
}

PRIVATE void export_login_reset(
    rs_log_login_reset_update_t   **buf,		   /* [in] */
    sec_rgy_login_name_t        *login_name,       /* [out] */
    sec_rgy_login_activity_t  *login_part,       /* [out] */
    uuid_t                      *client,	   /* [out] */
    sec_timeval_sec_t           *time_now          /* [out] */
)
{
    char                        *p;
    rs_log_login_r_t            *item;
    unsigned32			fixed_len;

   fixed_len=(sizeof(rs_log_login_r_t) + sizeof(rs_log_hdr_t));
   item = &((*buf)->login_part);
   p = (char *) (((unsigned32) item) + sizeof(rs_log_login_r_t));
  
   login_part->dis_time            = item->dis_time;
   login_part->cur_inv_attempts    = item->cur_inv_attempts;
   login_part->last_bad_time       = item->last_bad_time;
   login_part->last_good_time      = item->last_good_time;

    bcopy(p, (Pointer) login_name->pname, item->pname_len);
    login_name->pname[item->pname_len] = NULL;
    p += item->pname_len;

    bcopy(p, (Pointer) login_name->gname, item->gname_len);
    login_name->gname[item->gname_len] = NULL;
    p += item->gname_len;

    bcopy(p, (Pointer) login_name->oname, item->oname_len);
    login_name->oname[item->oname_len] = NULL;
    p += item->oname_len;

    if (time_now != NULL) {
       memcpy((char *)time_now, p, sizeof (*time_now));
    }

}

PUBLIC void
rs_log_login_reset_update
(
    sec_rgy_login_name_t        *login_name,       /* [in] */
    sec_rgy_login_activity_t    *login_part,       /* [in] */
    sec_timeval_sec_t           *time_now,
    rs_replica_master_info_t    *master_info,
    uuid_t        		*uuid_of_src,
    error_status_t		*st_p		/* [out] */
)
{
    rs_log_login_reset_update_t	*info=NULL;
    unsigned32			size=0;
    unsigned32			retval=0;

    retval=import_login_reset(login_name,login_part,time_now,&size,&info);
    if (retval){
	SET_STATUS(st_p, sec_rgy_log_entry_out_of_range);
	return;
    }
    rs_log_setup_log_header(&(info->upd_hdr), size, RS_LOG_MODULE_LOGINR,
                            rs_log_login_reset_update_op);

    if (master_info != NULL) {
        rs_log_write(&(info->upd_hdr),
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&(info->upd_hdr), NULL, NULL);
    }

    if (info)
    	free(info);

}

PRIVATE void
rs_log_login_reset_replay_update
(
    rs_log_rec_t *info,
    error_status_t *st_p
)
{
    rs_log_login_reset_update_t *log_info=(rs_log_login_reset_update_t *)info;
    sec_rgy_login_name_t        login_name;
    sec_rgy_login_activity_t    login_part;
    sec_timeval_sec_t           time_now;
    uuid_t        		client;

   CLEAR_STATUS(st_p);
   memset(&login_part,0,sizeof(sec_rgy_login_activity_t));
   memset(&login_name,0,sizeof(sec_rgy_login_name_t));
   export_login_reset(&log_info, &login_name, &login_part, &client, &time_now);
   rsdb_acct_reset_login_activity(&login_name, &login_part, st_p);
}

PRIVATE void
rs_log_login_reset_prop_update
(
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st_p
)
{
    rs_log_login_reset_update_t *log_info=(rs_log_login_reset_update_t *) info;
    sec_rgy_login_name_t        login_name;
    sec_rgy_login_activity_t    login_part;
    sec_timeval_sec_t           time_now;
    uuid_t                      client;

   CLEAR_STATUS(st_p);
   memset(&login_part,0,sizeof(sec_rgy_login_activity_t));
   memset(&login_name,0,sizeof(sec_rgy_login_name_t));
   export_login_reset(&log_info, &login_name, &login_part, &client,
        &time_now);
   rrs_prop_login_reset(handle, &login_name, &login_part, &client, &time_now,
              master_info, propq_only, st_p);

}
