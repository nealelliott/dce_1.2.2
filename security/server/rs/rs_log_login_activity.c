/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_login_activity.c,v $
 * Revision 1.1.5.2  1996/02/18  00:18:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:54  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:51:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:20:34  root]
 * 
 * Revision 1.1.2.3  1994/08/29  15:51:44  hondo
 * 	OT 11919 --login activity idl change to support different address types
 * 	[1994/08/28  21:45:31  hondo]
 * 
 * Revision 1.1.2.2  1994/08/10  19:22:06  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:24  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  15:01:30  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  18:00:17  mdf]
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
typedef struct rs_log_login_item_t {
       unsigned32             last_seqno;
       sec_timeval_sec_t      dis_time;
       unsigned32             cur_inv_attempts;
       sec_timeval_sec_t      last_bad_time;
       sec_timeval_sec_t      last_good_time;
       unsigned32 	      good_addrtype;
       unsigned32 	      good_length;
       unsigned32 	      bad_addrtype;
       unsigned32 	      bad_length;
       unsigned32	      pname_len;
       unsigned32	      gname_len;
       unsigned32	      oname_len;
       char		      buf[VARYING];
} rs_log_login_item_t;

typedef struct rs_log_data_t {
    uuid_t                          uuid_of_src;
} rs_log_data_t;

typedef struct rs_log_login_act_update_t {
    rs_log_hdr_t		    upd_hdr;
    rs_log_data_t	            upd_data;
    rs_log_login_item_t	            login_part;
} rs_log_login_act_update_t;



PRIVATE REPLAY_FUNCTION(rs_log_login_act_replay_update);

/*
 * Define Module opcodes and EPV
 */
#define rs_log_login_act_update_op	0
#define rs_log_login_act_last		1 /* The number of operations */

static rs_log_ops_t rs_log_login_act_epv[rs_log_login_act_last] = {

    {	LOCAL_UPDATE,
	NULL,
	rs_log_login_act_replay_update,
	"update" },
};

PUBLIC void
rs_log_login_act_init(error_status_t  *st)
{
    int num_ops = rs_log_login_act_last;
    rs_log_init_module(RS_LOG_MODULE_LOGIN, num_ops, rs_log_login_act_epv, st);
}

PRIVATE boolean32 import_login_activity(
    sec_rgy_login_name_t        *login_name,       /* [in] */
    sec_rgy_login_activity_t  *login_part,       /* [in] */
    sec_timeval_sec_t           *time_now,         /* [in] */
    unsigned32                  *size,
    rs_log_login_act_update_t    **data		   /* [out] */
)
{
    char                        *p;
    unsigned32                  pname_len;
    unsigned32                  gname_len;
    unsigned32                  oname_len;
    unsigned32                  g_o_name_len;
    unsigned32                  b_o_name_len;
    unsigned32                  fixed_len;
    rs_log_login_item_t         *item;

    /* Fixed portion */
    fixed_len=(sizeof(rs_log_login_item_t) + sizeof(rs_log_hdr_t) 
	+ sizeof( rs_log_data_t));
    *size=fixed_len;

    /* Variable portion */
    pname_len   = strlen( (char *) login_name->pname);
    gname_len   = strlen( (char *) login_name->gname);
    oname_len   = strlen( (char *) login_name->oname);
    b_o_name_len   = login_part->bad_origin.length;
    g_o_name_len   = login_part->good_origin.length;

    *size   += pname_len;
    *size   += gname_len;
    *size   += oname_len;
    *size   += b_o_name_len;
    *size   += g_o_name_len;



    if (time_now != NULL) {
        *size += sizeof(*time_now);
    }

    if ((*data = (rs_log_login_act_update_t *) malloc(*size)) ==NULL)
	return(1);

    item = &((*data)->login_part);
    memset((char *)&((*data)->upd_data.uuid_of_src),0,sizeof(uuid_t));
    p = (char *) (((unsigned32) *data) + fixed_len);


    /* Fixed portion */
    item->last_seqno          = 0;
    item->dis_time            = login_part->dis_time;
    item->cur_inv_attempts    = login_part->cur_inv_attempts;
    item->last_bad_time       = login_part->last_bad_time;
    item->last_good_time      = login_part->last_good_time;
    item->good_addrtype       = login_part->good_origin.data.addrtype;
    item->bad_addrtype        = login_part->bad_origin.data.addrtype;

    item->good_length         = g_o_name_len;
    item->bad_length          = b_o_name_len;
    item->pname_len           = pname_len;
    item->gname_len           = gname_len;
    item->oname_len           = oname_len;


    switch (item->good_addrtype)
    {
	case sec_ip_addr:
	    bcopy((Pointer)
		login_part->good_origin.data.tagged_union.o_ip_addr,
		p, login_part->good_origin.length);
    	    p += item->good_length;
	    break;
	case sec_dce_string:
	    bcopy((Pointer)
		login_part->good_origin.data.tagged_union.o_string_binding,
		p, login_part->good_origin.length);
    	    p += item->good_length;
	    break;
	default:
	    break;
    }
    switch (item->bad_addrtype)
    {
	case sec_ip_addr:
	    bcopy((Pointer)
		login_part->bad_origin.data.tagged_union.o_ip_addr,
		p, login_part->bad_origin.length);
    	    p += item->bad_length;
	    break;
	case sec_dce_string:
	    bcopy((Pointer)
		login_part->bad_origin.data.tagged_union.o_string_binding,
		p, login_part->bad_origin.length);
    	    p += item->bad_length;
	    break;
	default:
	    break;
    }

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

PRIVATE void export_login_activity(
    rs_log_login_act_update_t   **buf,		   /* [in] */
    sec_rgy_login_name_t        *login_name,       /* [out] */
    sec_rgy_login_activity_t  *login_part,       /* [out] */
    uuid_t                      *client,	   /* [out] */
    sec_timeval_sec_t           *time_now          /* [out] */
)
{
    char                        *p;
    rs_log_login_item_t         *item;
    unsigned32			fixed_len;

   fixed_len=sizeof(rs_log_login_item_t);
   item = &((*buf)->login_part);
   p = (char *) (((unsigned32) item) + fixed_len);
  
   login_part->dis_time            = item->dis_time;
   login_part->cur_inv_attempts    = item->cur_inv_attempts;
   login_part->last_bad_time       = item->last_bad_time;
   login_part->last_good_time      = item->last_good_time;
   login_part->good_origin.length  = item->good_length;
   login_part->bad_origin.length  = item->bad_length;

    switch (item->good_addrtype)
    {
	case sec_ip_addr:
   	    login_part->good_origin.data.addrtype=sec_ip_addr;
	    bcopy(p,(Pointer)
		login_part->good_origin.data.tagged_union.o_ip_addr,
		item->good_length);
	    break;
	case sec_dce_string:
   	    login_part->good_origin.data.addrtype=sec_dce_string;
	    bcopy(p,(Pointer)
		login_part->good_origin.data.tagged_union.o_string_binding,
		item->good_length);
	    break;
	default:
	    break;
    }
    switch (item->bad_addrtype)
    {
	case sec_ip_addr:
   	    login_part->bad_origin.data.addrtype=sec_ip_addr;
	    bcopy(p,(Pointer)
		login_part->bad_origin.data.tagged_union.o_ip_addr,
		item->bad_length);
	    break;
	case sec_dce_string:
   	    login_part->bad_origin.data.addrtype=sec_dce_string;
	    bcopy(p,(Pointer)
		login_part->bad_origin.data.tagged_union.o_string_binding,
		item->bad_length);
	    break;
	default:
	    break;
    }

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
rs_log_login_activity_update
(
    sec_rgy_login_name_t        *login_name,       /* [in] */
    sec_rgy_login_activity_t    *login_part,       /* [in] */
    sec_timeval_sec_t           *time_now,
    rs_replica_master_info_t    *master_info,
    uuid_t        		*uuid_of_src,
    error_status_t		*st_p		/* [out] */
)
{
    rs_log_login_act_update_t	*info;
    unsigned32			size;
    unsigned32			retval;

    retval=import_login_activity(login_name,login_part,time_now,&size,&info);
    if (retval){
	SET_STATUS(st_p, sec_rgy_log_entry_out_of_range);
	return;
    }
    rs_log_setup_log_header(&(info->upd_hdr), size, RS_LOG_MODULE_LOGIN,
                            rs_log_login_act_update_op);

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
rs_log_login_act_replay_update
(
    rs_log_rec_t *info,
    error_status_t *st_p
)
{
    rs_log_login_act_update_t   *log_info= (rs_log_login_act_update_t *) info;
    sec_rgy_login_name_t        login_name;
    sec_rgy_login_activity_t    login_part;
    sec_timeval_sec_t           time_now;
    uuid_t        		client;

   CLEAR_STATUS(st_p);
   export_login_activity(&log_info, &login_name, &login_part, &client, &time_now);
   rsdb_acct_replace_login_activity(&login_name, &login_part, st_p);
}
