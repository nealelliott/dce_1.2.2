/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_journal_activity.c,v $
 * Revision 1.1.4.3  1996/02/18  00:18:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:53  marty]
 *
 * Revision 1.1.4.2  1995/12/08  17:51:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:20:33  root]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */

/*
 *     Registry Server - Journal Activity Log operations
 */

#include <rs_log.h>
#include <rsdb_attr_pvt.h>     
#include <dce/rs_login_base.h>   
#include <dce/rgynbase.h>              
#include <rsdb_login.h>         
#include <rsdb_jrnl.h>         

#include <sec_attr_tools.h> 

/*
 * Local types 
 */

typedef struct rs_log_data_t {
    uuid_t                          uuid_of_src;
    rsdb_pvt_id_t                   id;  
    unsigned32                      len;
} rs_log_data_t;

typedef struct rs_log_login_act_update_t {
    rs_log_hdr_t		    upd_hdr;
    rs_log_data_t	            upd_data;
    rsdb_jrnl_rec_t	            jrnl_part;
} rs_log_jrnl_act_update_t;



PRIVATE REPLAY_FUNCTION(rs_log_jrnl_act_replay_update);

/*
 * Define Module opcodes and EPV
 */
#define rs_log_jrnl_act_update_op	0
#define rs_log_jrnl_act_last		1 /* The number of operations */

static rs_log_ops_t rs_log_jrnl_act_epv[rs_log_jrnl_act_last] = {

    {	LOCAL_UPDATE,
	NULL,
	rs_log_jrnl_act_replay_update,
	"update" },
};

PUBLIC void
rs_log_jrnl_act_init(error_status_t  *st)
{
    int num_ops = rs_log_jrnl_act_last;
    rs_log_init_module(RS_LOG_MODULE_JOURNAL, num_ops, rs_log_jrnl_act_epv, st);
}


PUBLIC void
rs_log_journal_activity_update
(
    rsdb_pvt_id_t               id,          /* [in] */
    rsdb_jrnl_rec_t             *data,       /* [in] */
    unsigned32                  data_len,    /* [in] */
    sec_timeval_sec_t           *time_now,
    rs_replica_master_info_t    *master_info,
    uuid_t        		*uuid_of_src,
    error_status_t		*st_p	     /* [out] */
)
{
    rs_log_jrnl_act_update_t   *info;
    int				size;


    size=(sizeof(rs_log_hdr_t) + sizeof(rs_log_data_t) + data_len);
    if ((info = (rs_log_jrnl_act_update_t *) malloc(size)) == NULL)
        return;
    rs_log_setup_log_header(&(info->upd_hdr), size, RS_LOG_MODULE_JOURNAL,
                            rs_log_jrnl_act_update_op);
    info->upd_data.id=id;
    info->upd_data.uuid_of_src=(*uuid_of_src);
    info->upd_data.len=data_len;
    bcopy(data,&(info->jrnl_part),data_len);

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
rs_log_jrnl_act_replay_update
(
    rs_log_rec_t   *info,
    error_status_t *st_p
)
{
    rsdb_sequential_key_t  sequential_key;
    rsdb_jrnl_rec_t        *data=NULL;
    rs_log_jrnl_act_update_t   *jrnl_info= (rs_log_jrnl_act_update_t *) info;
    int			   data_len=0;


    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    CLEAR_STATUS(st_p);
    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id =jrnl_info->upd_data.id;

    data=&(jrnl_info->jrnl_part);
    data_len=jrnl_info->upd_data.len;
 
    rsdb_replace(rsdb_journal_db, (Pointer) &sequential_key,
           sizeof(sequential_key), (Pointer) data, data_len, st_p);
}
