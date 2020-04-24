/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY  
 * $Log: rs_log_acct.c,v $
 * Revision 1.1.77.2  1996/02/18  00:17:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:47  marty]
 *
 * Revision 1.1.77.1  1995/12/08  17:51:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:20:28  root]
 * 
 * Revision 1.1.75.3  1994/08/08  14:23:34  mdf
 * 	CR11018: Put back 64 bit porting changes
 * 	[1994/08/08  12:53:49  mdf]
 * 
 * Revision 1.1.75.2  1994/07/15  15:01:27  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:19:37  mdf]
 * 
 * Revision 1.1.75.1  1994/06/02  21:25:09  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:28  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:55:48  mdf]
 * 
 * 	HP revision /main/GAMERA/KK/ODESSA_2/1  1994/05/04  20:38 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/GAMERA/KK/hanfei_ch_master/1  1994/05/04  20:14 UTC  hanfei
 * 	merge
 * 
 * Revision 1.1.5.2  1993/07/08  12:44:49  root
 * 	Initial King Kong branch
 * 	[1993/07/08  12:44:11  root]
 * 
 * 	CR 7472 - rs_log_acct_acct/replace - deal w. NULL unix passwd.
 * 	[1993/03/24  21:45:47  rps]
 * 
 * 	CR 7472 - rs_log_acct_add/replace use slightly different
 * 	 primitives to decrypt unix passwd.
 * 	[1993/03/19  18:56:45  emartin]
 * 
 * 	CR 7472
 * 	 rs_log_acct_add/replace - take unix_passwd arg and save it to log.
 * 	 rs_log_acct_replay_add/replay - regenerate unix_passwd into
 * 	     user_part.passwd, invoke rsdb_acct_add/replace w.
 * 	     gen_unix_passwd false.
 * 	 rs_log_acct_prop_add/replace - send xmit rep of unix passwd.
 * 	 import/export_acct_item - call import/export_acct_passwd
 * 	 save/restore auth key and unix key.
 * 	[1993/03/17  18:39:22  emartin]
 * 
 * 	bl6e: in prop_replace, if passwd not being modified, make it null pointer
 * 	[1993/01/29  02:37:59  ahop]
 * 
 * 	rs_log_acct_prop_replace - fill passwd.key, checksum
 * 	[1993/01/20  21:33:21  emartin]
 * 
 * 	rs_log_acct_prop_add - fill acct.key.
 * 	[1993/01/18  22:03:38  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  21:23:47  zeliff]
 * 
 * 	5974: fix secd crash on acct_replace - bug in import/export_acct_item
 * 	[1992/11/13  20:07:44  ahop]
 * 
 * 	Bugfix: avoid unaligned reference in log.
 * 	Seems to happen when adding CDS registry entries.
 * 	[1992/10/16  18:42:59  sommerfeld]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:16:38  sekhar]
 * 
 * Revision 1.1.3.2  1993/06/23  21:18:18  ahop
 * 	Always init user_part.passwd to empty string in export_acct_item()
 * 	[1993/06/23  19:57:12  ahop]
 * 
 * 	Second replication drop: bl5
 * 	[1992/10/06  16:59:18  burati]
 * 
 * Revision 1.1.1.12  1993/05/04  20:38:37  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1  1992/01/19  14:44:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/* 
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *     Registry Server - Acct Log operations
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: rs_log_acct.c,v 1.1.77.2 1996/02/18 00:17:53 marty Exp $";
#endif

#include <un_maloc.h>
#include <rs_log.h>

#include <rs_prop_acct.h>
#include <rrs_prop_acct.h>

#include <rs_server.h>
#include <rs_log_acct.h>
#include <rs_log_util.h>
#include <dce/rsbase.h>
#include <rs_pwd.h>

#include <rsdb_acct.h>
#include <rsdb_util.h>

#include <crypt.h>

#include <bstring.h>
#include <un_strng.h>

/*
 * Local types
 */

typedef struct rs_log_acct_item_user_t {
    sec_timeval_sec_t          passwd_dtm;
    unsigned32                 homedir_len;
    unsigned32                 shell_len;
    unsigned32                 gecos_len;
    sec_rgy_acct_user_flags_t  flags;
} rs_log_acct_item_user_t;

typedef struct rs_log_passwd_item_t {
    unsigned32          key_pickle_len;
    unsigned32          key_len;
    unsigned32          checksum_len;
    sec_etype_t         enc_type;
    sec_passwd_type_t   enc_keytype;
    sec_passwd_version_t enc_key_version;
    sec_timeval_sec_t   passwd_dtm;
} rs_log_passwd_item_t;

/*
 * The log_acct_item_t structure contains the fixed portion of the
 * account.  Varying data is appended to the buffer.
 */
typedef struct rs_log_acct_item_t {
    rs_acct_parts_t     data_parts;
    unsigned32          person_name_len;
    unsigned32          group_name_len;
    unsigned32          org_name_len;
} rs_log_acct_item_t;

/* DCE 1.0 log info for backward compatibility */
typedef struct rs_log_acct_add_v1_0_t {
    rs_log_hdr_t            upd_hdr;
    sec_rgy_foreign_id_t    client_ids;
    rs_log_acct_item_t      item;
} rs_log_acct_add_v1_0_t;

/* 
 * At DCE 1.0.2, the data logged for adds are a subset of the
 * data logged for replaces.  In particular, the replace operation
 * requires a timestamp for use in garbage collecting expired 
 * versions of multi-version key. Rather than create a new data type,
 * or add a new fixed -length field that is used only part of the time,
 *  we put the timestamp in the varying portion of the log record.
 */
typedef struct rs_log_acct_add_t {
    rs_log_hdr_t            upd_hdr;
    sec_rgy_foreign_id_t    client_ids;
    sec_passwd_type_t       key_type;
    rs_log_acct_item_t      item;
} rs_log_acct_add_t;

typedef struct rs_log_acct_delete_t {
    rs_log_hdr_t            upd_hdr;
    unsigned32              pname_len;
    unsigned32              gname_len;
    unsigned32              oname_len;
    rs_log_acct_name_buf_t  name_buf;
} rs_log_acct_delete_t;

typedef struct rs_log_acct_rename_t {
    rs_log_hdr_t       upd_hdr;
    unsigned32         old_pname_len;
    unsigned32         old_gname_len;
    unsigned32         old_oname_len;
    unsigned32         new_pname_len;
    unsigned32         new_gname_len;
    unsigned32         new_oname_len;
    char               name_buf[2 * ACCT_NAME_BUFSIZE];
} rs_log_acct_rename_t;


/*
 * PRIVATE local routines
 */

PRIVATE PROP_FUNCTION(rs_log_acct_prop_add);
PRIVATE PROP_FUNCTION(rs_log_acct_prop_delete);
PRIVATE PROP_FUNCTION(rs_log_acct_prop_replace);
PRIVATE PROP_FUNCTION(rs_log_acct_prop_rename);
PRIVATE PROP_FUNCTION(rs_log_acct_prop_key_delete);

PRIVATE REPLAY_FUNCTION(rs_log_acct_replay_add);
PRIVATE REPLAY_FUNCTION(rs_log_acct_replay_delete);
PRIVATE REPLAY_FUNCTION(rs_log_acct_replay_replace);
PRIVATE REPLAY_FUNCTION(rs_log_acct_replay_rename);
PRIVATE REPLAY_FUNCTION(rs_log_acct_replay_key_delete);

/*
 * END OF PRIVATE ROUTINES
 */


/*
 * Define Module opcodes and EPV
 */
#define rs_log_acct_add_op         0
#define rs_log_acct_delete_op      1
#define rs_log_acct_replace_op     2
#define rs_log_acct_rename_op      3
#define rs_log_acct_last           4   /* Must be equal to the number of operations */

static rs_log_ops_t rs_log_acct_epv[rs_log_acct_last] = {
    { GLOBAL_UPDATE, rs_log_acct_prop_add,     rs_log_acct_replay_add, "add" } ,
    { GLOBAL_UPDATE, rs_log_acct_prop_delete,  rs_log_acct_replay_delete, "delete" } ,
    { GLOBAL_UPDATE, rs_log_acct_prop_replace, rs_log_acct_replay_replace, "replace" } ,
    { GLOBAL_UPDATE, rs_log_acct_prop_rename, rs_log_acct_replay_rename, "rename" } ,
};

#ifdef SEC_DEBUG_ACCT_LOGGING
PRIVATE void print_buffer
  (
    char            *header,
    unsigned char   *buf,
    unsigned long   len
  )
{
    int i, j;

    printf("%s\n", header);

    while (len > 0) {
        printf("    ");
        for (i = 0; i<6; i++) {
            printf(" ");
            for (j = 0; j < 4; j++) {
                if (len == 0) break;
                printf("%02.2X", *buf++);
                len--;
            }
        }
        printf("\n");
    }
}
#endif

PUBLIC void rs_log_acct_init
  (
    error_status_t  *st
  )
{
    int             num_ops;

    num_ops = rs_log_acct_last;
    rs_log_init_module(RS_LOG_MODULE_ACCT, num_ops, rs_log_acct_epv, st);
}

PRIVATE void import_acct_passwd
  (
    rs_acct_key_transmit_t  *passwd,
    sec_timeval_sec_t       *passwd_dtm,
    char                    **outbufp
  )
{
    char                    *p;
    rs_log_passwd_item_t    pw;

    p = *outbufp;

    if (passwd != NULL) {
        pw.key_pickle_len      = passwd->key_pickle_len;
        pw.key_len             = passwd->key->enc_pickle_len;
        pw.checksum_len        = passwd->checksum->enc_pickle_len;
        pw.enc_type            = passwd->enc_type;
        pw.enc_keytype         = passwd->enc_keytype;
        pw.enc_key_version     = passwd->enc_key_version;
        pw.passwd_dtm          = *passwd_dtm;
        bcopy( (Pointer) &pw, p, sizeof(rs_log_passwd_item_t));
        p += sizeof(rs_log_passwd_item_t);
        bcopy((Pointer) passwd->key->enc_pickle, p,
                        passwd->key->enc_pickle_len);
        p += passwd->key->enc_pickle_len;
        bcopy((Pointer) passwd->checksum->enc_pickle, p,
                        passwd->checksum->enc_pickle_len);
        p +=  passwd->checksum->enc_pickle_len;
    }
    else {
        /*
         * passwd is NULL.  Set its key length info to 0.
         */
        pw.key_pickle_len      = 0;
        pw.key_len             = 0;
        pw.checksum_len        = 0;
        bcopy( (Pointer) &pw, p, sizeof(rs_log_passwd_item_t));
        p += sizeof(rs_log_passwd_item_t);
    }

    *outbufp = p;
}

/*
 * if time_now is NULL; don't attempt to log it, otherwise store in with the
 * variable length data
 */
PRIVATE void import_acct_item
  (
    rs_log_acct_add_t       **buf,
    rs_acct_parts_t         data_parts,
    sec_rgy_login_name_t    *login_name,
    sec_rgy_acct_user_t     *user_part,
    sec_rgy_acct_admin_t    *admin_part,
    rs_acct_key_transmit_t  *passwd,
    sec_rgy_foreign_id_t    *client_ids,
    sec_passwd_type_t       key_type,
    rs_acct_key_transmit_t  *unix_passwd,
    sec_timeval_sec_t       *time_now,  
    long                    *size
  )
{
    char                        *p;
    rs_log_acct_item_user_t     user_item;
    rs_log_acct_item_t          *item;
    unsigned32                  pname_len;
    unsigned32                  gname_len;
    unsigned32                  oname_len;

    /* Determine size of buffer */

    /* Fixed portion */
    *size = sizeof(rs_log_acct_add_t);

    /* Variable portion */
    pname_len   = strlen( (char *) login_name->pname);
    gname_len   = strlen( (char *) login_name->gname);
    oname_len   = strlen( (char *) login_name->oname);
    *size   += pname_len;
    *size   += gname_len;
    *size   += oname_len;

    if (data_parts & rs_acct_part_admin) {
        *size   += sizeof(sec_rgy_acct_admin_t);
    }
    if (data_parts & rs_acct_part_user) {
        user_item.homedir_len   = strlen( (char *) user_part->homedir);
        user_item.shell_len     = strlen( (char *) user_part->shell);
        user_item.gecos_len     = strlen( (char *) user_part->gecos);
        *size   += sizeof(user_item);
        *size   +=  user_item.homedir_len + user_item.shell_len +
                 user_item.gecos_len;
    }

    if (data_parts & rs_acct_part_passwd) {
        *size   += sizeof(rs_log_passwd_item_t) 
                        + passwd->key->enc_pickle_len
                        + passwd->checksum->enc_pickle_len;
        *size   += sizeof(rs_log_passwd_item_t) 
                        + unix_passwd->key->enc_pickle_len
                        + unix_passwd->checksum->enc_pickle_len;
    }   
 
    if (time_now != NULL) {
        *size += sizeof(*time_now); 
    }

    /*
    ** Allocate space for the buffer and set pointer to beginning
    ** of optional buffer.
    */
    *buf = (rs_log_acct_add_t *) malloc(*size);
    p = (char *) (*buf + 1);
    item = &((*buf)->item);

    /* Fixed portion */
    (*buf)->client_ids      = *client_ids;
    (*buf)->key_type        = key_type;

    item->data_parts        = data_parts;
    item->person_name_len   = pname_len;
    item->group_name_len    = gname_len;
    item->org_name_len      = oname_len;

    /* Variable portion */
    bcopy( (Pointer) login_name->pname, p, item->person_name_len);
    p       += item->person_name_len;

    bcopy( (Pointer) login_name->gname, p, item->group_name_len);
    p       += item->group_name_len;

    bcopy( (Pointer) login_name->oname, p, item->org_name_len);
    p       += item->org_name_len;


    if (data_parts & rs_acct_part_admin) {
        bcopy( (Pointer) admin_part, p, sizeof(sec_rgy_acct_admin_t));
        p       += sizeof(sec_rgy_acct_admin_t);
    }

    if (data_parts & rs_acct_part_user) {
        user_item.flags         = user_part->flags;
        user_item.passwd_dtm    = user_part->passwd_dtm;
        bcopy( (Pointer) &user_item, p, sizeof(user_item));

        p       += sizeof(user_item);

        if (user_item.homedir_len > 0) {
            bcopy( (Pointer) user_part->homedir, p, user_item.homedir_len);
            p += user_item.homedir_len;
        }

        if (user_item.shell_len > 0) {
            bcopy( (Pointer) user_part->shell, p, user_item.shell_len);
            p += user_item.shell_len;
        }

        if (user_item.gecos_len > 0) {
            bcopy( (Pointer) user_part->gecos, p, user_item.gecos_len);
            p += user_item.gecos_len;
        }
    }

    /* Save auth password and unix password
     */
    if (data_parts & rs_acct_part_passwd) {
        import_acct_passwd(passwd, &user_part->passwd_dtm, &p);
        import_acct_passwd(unix_passwd, &user_part->passwd_dtm, &p);
    }

    if (time_now != NULL) {
	memcpy (p, (char *) time_now, sizeof(*time_now));
    }
}

PRIVATE void export_acct_passwd
 (
    char                    **inbufp,
    sec_timeval_sec_t       *passwd_dtm,
    rs_acct_key_transmit_t  *passwd
 )
{
    char                    *p;
    rs_log_passwd_item_t    pw;

    p = *inbufp;

    bcopy(p, (Pointer) &pw, sizeof(pw));

    if (passwd) {
        if (pw.key_len != 0) {
            passwd->key_pickle_len              = pw.key_pickle_len;
            passwd->key->enc_pickle_len         = pw.key_len;
            passwd->checksum->enc_pickle_len    = pw.checksum_len;
            passwd->enc_type                    = pw.enc_type;
            passwd->enc_keytype                 = pw.enc_keytype;
            passwd->enc_key_version             = pw.enc_key_version;
            *passwd_dtm                         = pw.passwd_dtm;
    
            p += sizeof(rs_log_passwd_item_t);
            passwd->key->enc_pickle             = (unsigned char *) p;
            p += pw.key_len;
            passwd->checksum->enc_pickle        = (unsigned char *) p;
            p += pw.checksum_len; 
        }
        else {
            /*
             * no passwd exists.  fill key lengths w. 0.
             */
            passwd->key_pickle_len              = 0;
            passwd->key->enc_pickle_len         = 0;
            passwd->checksum->enc_pickle_len    = 0;

            p += sizeof(rs_log_passwd_item_t);
        }
    }
    else {
        /* password not requested, move past its buffer
         */
        p += sizeof(rs_log_passwd_item_t);
        p += pw.key_len;
        p += pw.checksum_len;
    }

    *inbufp = p;
}

/*
 * if "time_now" is NULL, don't bother looking for it in the logged
 * account data
 */
PRIVATE void export_acct_item
  (
    rs_log_acct_add_t       *buf,
    rs_acct_parts_t         *data_parts,
    sec_rgy_login_name_t    *login_name,
    sec_rgy_acct_user_t     *user_part,
    sec_rgy_acct_admin_t    *admin_part,
    rs_acct_key_transmit_t  *passwd,
    sec_rgy_foreign_id_t    *client_ids,
    sec_passwd_type_t       *key_type,
    rs_acct_key_transmit_t  *unix_passwd,
    sec_timeval_sec_t       *time_now
  )
{
    char                        *p;
    rs_log_acct_item_user_t     user_item;
    rs_log_acct_item_t          *item;

    *client_ids = buf->client_ids;
    *key_type = buf->key_type;
    
    item = &(buf->item);
    p = (char *) (item + 1);

    /* Fixed portion */
    *data_parts         = item->data_parts;

    /* Variable portion */

    bcopy(p, (Pointer) login_name->pname, item->person_name_len);
    login_name->pname[item->person_name_len] = NULL;
    p += item->person_name_len;

    bcopy(p, (Pointer) login_name->gname, item->group_name_len);
    login_name->gname[item->group_name_len] = NULL;
    p += item->group_name_len;

    bcopy(p, (Pointer) login_name->oname, item->org_name_len);
    login_name->oname[item->org_name_len] = NULL;
    p += item->org_name_len;


    if (*data_parts & rs_acct_part_admin) {
        bcopy(p, (Pointer) admin_part, sizeof(sec_rgy_acct_admin_t));
        p += sizeof(sec_rgy_acct_admin_t);
    } else {
        bzero( (Pointer) admin_part, sizeof(sec_rgy_acct_admin_t));
    }

    if (*data_parts & rs_acct_part_user) {
        user_part->passwd[0] = '\0';
        bcopy(p, (Pointer) &user_item, sizeof(user_item));
        user_part->passwd_dtm   = user_item.passwd_dtm;
        user_part->flags        = user_item.flags;

        p += sizeof(user_item);

        if (user_item.homedir_len > 0) {
            bcopy(p, (Pointer) user_part->homedir, user_item.homedir_len);
            p += user_item.homedir_len;
        }
        user_part->homedir[user_item.homedir_len] = '\0';

        if (user_item.shell_len > 0) {
            bcopy(p, (Pointer) user_part->shell, user_item.shell_len);
            p += user_item.shell_len;
        }
        user_part->shell[user_item.shell_len] = '\0';

        if (user_item.gecos_len > 0) {
            bcopy(p, (Pointer) user_part->gecos, user_item.gecos_len);
            p += user_item.gecos_len;
        }
        user_part->gecos[user_item.gecos_len] = '\0';

    } else {
        bzero( (Pointer) user_part, sizeof(sec_rgy_acct_user_t));
    }

    if (*data_parts & rs_acct_part_passwd) {
        export_acct_passwd(&p, &user_part->passwd_dtm, passwd);
        export_acct_passwd(&p, &user_part->passwd_dtm, unix_passwd);
    }

    if (time_now != NULL) {
       memcpy((char *)time_now, p, sizeof (*time_now));
    }
}


PUBLIC void rs_log_acct_add
  (
    sec_rgy_login_name_t        *login_name,
    sec_rgy_acct_user_t         *user_part,
    sec_rgy_acct_admin_t        *admin_part,
    rs_acct_key_transmit_t      *passwd,
    sec_rgy_foreign_id_t        *client_ids,
    sec_passwd_type_t           key_type,
    rs_acct_key_transmit_t      *unix_passwd,
    rs_replica_master_info_t    *master_info
  )
{
    rs_acct_parts_t     data_parts;
    long                size;
    rs_log_acct_add_t   *info;

#ifdef SEC_DEBUG_ACCT_LOGGING
    print_buffer("Log acct, checksum:", passwd->checksum->enc_pickle,
                    passwd->checksum->enc_pickle_len);
    print_buffer("Log acct, key:", passwd->key->enc_pickle,
                    passwd->key->enc_pickle_len);
#endif

    data_parts = rs_acct_part_user | rs_acct_part_admin
                    | rs_acct_part_passwd | rs_acct_part_login_name;
    import_acct_item(&info, data_parts, login_name, user_part, admin_part, 
                     passwd, client_ids, key_type, unix_passwd, NULL, &size);

    rs_log_setup_log_header(&(info->upd_hdr), size, RS_LOG_MODULE_ACCT,
                            rs_log_acct_add_op);

    if (master_info != NULL) {
        rs_log_write((rs_log_hdr_t *) info, 
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write((rs_log_hdr_t *) info, NULL, NULL);
    }

    free(info);
}

PRIVATE void rs_log_acct_prop_add
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
{
    rs_log_acct_add_t           *log_info = (rs_log_acct_add_t *) info;
    rs_prop_acct_add_data_t     acct[1];  /* prop a single acct add */
    rs_acct_parts_t             data_parts;
    rs_acct_key_transmit_t      passwd;
    rs_encrypted_pickle_t       key;
    rs_encrypted_pickle_t       checksum;
    rs_acct_key_transmit_t      unix_passwd;
    rs_encrypted_pickle_t       unix_key;
    rs_encrypted_pickle_t       unix_checksum;

    CLEAR_STATUS(st);

    passwd.key = &key;
    passwd.checksum = &checksum;
    acct[0].key = &passwd; 
    unix_passwd.key = &unix_key;
    unix_passwd.checksum = &unix_checksum;
    acct[0].unix_passwd = &unix_passwd; 

    export_acct_item(log_info, &data_parts, &acct[0].login_name, 
                     &acct[0].user_part, &acct[0].admin_part, acct[0].key, 
                     &acct[0].client, &acct[0].keytype, 
                     acct[0].unix_passwd, NULL);

    if (acct[0].unix_passwd->key->enc_pickle_len == 0) {
        /* no unix passwd available.  let the slave generate 
         * one.
         */
        acct[0].unix_passwd = NULL;
    }

    if (GOOD_STATUS(st)) {
        rrs_prop_acct_add(handle, 1, acct, master_info, propq_only, st);
    }
}

PRIVATE void rs_log_acct_replay_add
  (
    rs_log_rec_t        *info,
    error_status_t      *st
  )
{
    sec_rgy_login_name_t   login_name;
    sec_rgy_acct_key_t     key_parts;
    sec_rgy_acct_user_t    user_part;
    sec_rgy_acct_admin_t   admin_part;
    rs_acct_parts_t        data_parts;
    rs_acct_key_transmit_t passwd;
    rs_encrypted_pickle_t  key;
    rs_encrypted_pickle_t  checksum;
    rs_acct_key_transmit_t unix_passwd;
    rs_encrypted_pickle_t  unix_key;
    rs_encrypted_pickle_t  unix_checksum;
    boolean32              gen_unix_passwd = true;
    sec_passwd_version_t   passwd_version_number;
    sec_rgy_foreign_id_t   client_ids;
    sec_passwd_rec_t       new_key;    
    sec_passwd_rec_t       *princ_key = NULL;    
    sec_passwd_type_t      key_type;
    rs_log_acct_add_t      *log_info = (rs_log_acct_add_t *) info;

    passwd.key = &key;
    passwd.checksum = &checksum;
    unix_passwd.key = &unix_key;
    unix_passwd.checksum = &unix_checksum;

    /* 
     * key parts are obsolete (there is only one account per principal allowed),
     * but still pervasive in the database code. Looks like a v1.1 cleanup task.
     */
    key_parts = sec_rgy_acct_key_person;  

    CLEAR_STATUS(st);
    export_acct_item(log_info, &data_parts, &login_name, &user_part, 
                     &admin_part, &passwd, &client_ids, &key_type, 
                     &unix_passwd, NULL);
                                                                 
#ifdef SEC_DEBUG_ACCT_LOGGING
    print_buffer("Replay acct, checksum:", passwd.checksum->enc_pickle,
                    passwd.checksum->enc_pickle_len);
    print_buffer("Replay acct, key:", passwd.key->enc_pickle,
                    passwd.key->enc_pickle_len);
#endif

    rs_pwd_get_key_by_prnc(&passwd, &client_ids.principal, 
        &new_key, &princ_key, st);

    if (GOOD_STATUS(st)) { 
        if (unix_passwd.key->enc_pickle_len != 0) {
            /*
             * unix passwd was saved.  decrypt it into
             * user_part.passwd and later send it to
             * rsdb_acct_replace.  Tell rsdb_acct_replace
             * not to generate a unix passwd but to take
             * the passwd in user_part.passwd
             */
            rs_pwd_unix_pwd_fr_xmit(
                &unix_passwd, princ_key, user_part.passwd, st);
            gen_unix_passwd = false;
        }
    }

    if (GOOD_STATUS(st)) {
        rsdb_acct_add(&login_name, key_parts, &user_part, &admin_part,
                      &new_key, gen_unix_passwd, &passwd_version_number, st);
    }

    if (GOOD_STATUS(st)) {
        rsdb_util_quota_decrement(&(log_info->client_ids.principal),
                                  &(log_info->client_ids.cell), st);
    }

    if (princ_key != NULL)
        sec_crypt_free_key(&princ_key);
}

PUBLIC void rs_log_acct_delete
  (
    sec_rgy_login_name_t        *login_name,
    rs_replica_master_info_t    *master_info
  )
{
    long                    size = 0;
    char                    *p;
    rs_log_acct_delete_t    info;


    rs_log_import_acct_name(login_name, &(info.pname_len), &(info.gname_len),
                            &(info.oname_len), info.name_buf, &size);

    size = size + sizeof(info) - sizeof(info.name_buf);

    rs_log_setup_log_header(&info.upd_hdr, size, RS_LOG_MODULE_ACCT,
                            rs_log_acct_delete_op);

    if (master_info != NULL) {
        rs_log_write(&info.upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&info.upd_hdr, NULL, NULL);
    }
}

PRIVATE void rs_log_acct_prop_delete
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
{
    rs_log_acct_delete_t   *log_info = (rs_log_acct_delete_t *) info;
    sec_rgy_login_name_t   login_name;

    CLEAR_STATUS(st);
    rs_log_export_acct_name(&login_name, log_info->pname_len,
                            log_info->gname_len, log_info->oname_len,
                            log_info->name_buf);

    rrs_prop_acct_delete(handle, &login_name, master_info, 
                         propq_only, st);

}


PRIVATE void rs_log_acct_replay_delete
  (
    rs_log_rec_t        *info,
    error_status_t      *st
  )
{
    rs_log_acct_delete_t   *log_info = (rs_log_acct_delete_t *) info;
    sec_rgy_login_name_t   login_name;

    rs_log_export_acct_name(&login_name, log_info->pname_len,
                            log_info->gname_len, log_info->oname_len,
                            log_info->name_buf);

    CLEAR_STATUS(st);
    rsdb_acct_delete(&login_name, st);
}


PUBLIC void rs_log_acct_replace 
  (
    sec_rgy_login_name_t        *login_name,
    rs_acct_parts_t             data_parts,
    sec_rgy_acct_user_t         *user_part,
    sec_rgy_acct_admin_t        *admin_part,
    rs_acct_key_transmit_t      *passwd,
    sec_rgy_foreign_id_t        *client_ids,
    sec_passwd_type_t           key_type,
    rs_acct_key_transmit_t      *unix_passwd,
    sec_timeval_sec_t           *time_now,
    rs_replica_master_info_t    *master_info
  ) 
{
    long                size;
    rs_log_acct_add_t   *info;
    sec_rgy_acct_key_t  key_parts;

    /* 
     * key parts are obsolete (there is only one account per principal allowed),
     * but still pervasive in the database code. Looks like a v1.1 cleanup task.
     */
    key_parts = sec_rgy_acct_key_person;  
    import_acct_item(&info, data_parts, login_name, user_part, admin_part,
                     passwd, client_ids, key_type, unix_passwd, time_now, 
                     &size);

    rs_log_setup_log_header(&(info->upd_hdr), size, RS_LOG_MODULE_ACCT,
                            rs_log_acct_replace_op);

    if (master_info != NULL) {
        rs_log_write(&info->upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&info->upd_hdr, NULL, NULL);
    }

    free(info);
}


PRIVATE void rs_log_acct_prop_replace
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
{
    rs_log_acct_add_t           *log_info = (rs_log_acct_add_t *) info;
    sec_rgy_login_name_t        login_name;
    rs_acct_parts_t             modify_parts;
    sec_rgy_acct_user_t         user_part;
    sec_rgy_acct_admin_t        admin_part;
    rs_acct_key_transmit_t      passwd;
    rs_acct_key_transmit_t      *passwd_p;
    rs_encrypted_pickle_t       key;
    rs_encrypted_pickle_t       checksum;
    rs_acct_key_transmit_t      unix_passwd;
    rs_acct_key_transmit_t      *unix_passwd_p;
    rs_encrypted_pickle_t       unix_key;
    rs_encrypted_pickle_t       unix_checksum;
    sec_rgy_foreign_id_t        client;
    sec_passwd_type_t           new_keytype; 
    sec_timeval_sec_t           time_now;
    boolean32                   change_passwd;

    CLEAR_STATUS(st);

    passwd.key = &key;
    passwd.checksum = &checksum;
    passwd_p = &passwd;
    unix_passwd.key = &unix_key;
    unix_passwd.checksum = &unix_checksum;
    unix_passwd_p = &unix_passwd;

    export_acct_item(log_info, &modify_parts, &login_name, &user_part, 
                     &admin_part, passwd_p, &client, &new_keytype, 
                     unix_passwd_p, &time_now);
 
    if(!(change_passwd = FLAG_SET(modify_parts, rs_acct_part_passwd))) {
        /*
         * passwd not being changed
         */
        passwd_p = NULL;
        unix_passwd_p = NULL;
    }
    else {
        if (unix_passwd.key->enc_pickle_len == 0) {
            /* unix passwd not speced.  slave will
             * generate it
             */
            unix_passwd_p = NULL;
        }
    }

    rrs_prop_acct_replace(handle, &login_name, modify_parts, 
                          &user_part, &admin_part, passwd_p, &client, new_keytype, 
                          unix_passwd_p, &time_now, master_info, propq_only, st);
}

PRIVATE void rs_log_acct_replay_replace 
  (
    rs_log_rec_t    *info,
    error_status_t  *st
  )
{
    sec_rgy_login_name_t   login_name;
    sec_rgy_acct_key_t     key_parts;
    sec_rgy_acct_user_t    user_part;
    sec_rgy_acct_admin_t   admin_part;
    rs_acct_parts_t        data_parts;
    rs_acct_key_transmit_t passwd;
    rs_encrypted_pickle_t  key;
    rs_encrypted_pickle_t  checksum;
    rs_acct_key_transmit_t unix_passwd;
    rs_encrypted_pickle_t  unix_key;
    rs_encrypted_pickle_t  unix_checksum;
    boolean32              gen_unix_passwd = true;
    sec_passwd_version_t   passwd_version_number;
    sec_rgy_foreign_id_t   client_ids;
    sec_passwd_rec_t       new_key;
    sec_passwd_rec_t       *princ_key = NULL;    
    sec_passwd_type_t      key_type;     
    sec_timeval_sec_t      time_now;
    rs_log_acct_add_t      *log_info = (rs_log_acct_add_t *) info;

    passwd.key = &key;
    passwd.checksum = &checksum;
    unix_passwd.key = &unix_key;
    unix_passwd.checksum = &unix_checksum;

    /* 
     * key parts are obsolete (there is only one account per principal allowed),
     * but still pervasive in the database code. Looks like a v1.1 cleanup task.
     */
    key_parts = sec_rgy_acct_key_person;  

    CLEAR_STATUS(st);
    export_acct_item(log_info, &data_parts, &login_name, 
                     &user_part, &admin_part, 
                     &passwd, &client_ids, &key_type, 
                     &unix_passwd, &time_now);

    if (data_parts & rs_acct_part_passwd) {
        rs_pwd_get_key_by_prnc(&passwd, &client_ids.principal, 
            &new_key, &princ_key, st);
        if (BAD_STATUS(st)) {
            rs_util_fatal_st(*st,
                "Unable to decipher key on log replay replace");
        } 

        if (unix_passwd.key->enc_pickle_len != 0) {
            /*
             * unix passwd was saved.  decrypt it into
             * user_part.passwd and later send it to
             * rsdb_acct_replace.  Tell rsdb_acct_replace
             * not to generate a unix passwd but to take
             * the passwd in user_part.passwd
             */
            rs_pwd_unix_pwd_fr_xmit(
                &unix_passwd, princ_key, user_part.passwd, st);
            gen_unix_passwd = false;
        }
    }

    rsdb_acct_replace(&login_name, key_parts, data_parts, &user_part,
                      &admin_part, &new_key, gen_unix_passwd, &time_now, 
                      &passwd_version_number, st);

    if (princ_key != NULL)
        sec_crypt_free_key(&princ_key);
}


PUBLIC void rs_log_acct_rename
  (
    sec_rgy_login_name_t        *old_login_name,
    sec_rgy_login_name_t        *new_login_name,
    rs_replica_master_info_t    *master_info
  )
{
    long                    size = 0;
    long                    temp_size;
    char                    *p;
    rs_log_acct_rename_t    info;


    p = &(info.name_buf[0]);

    rs_log_import_acct_name(old_login_name, &(info.old_pname_len),
                     &(info.old_gname_len), &(info.old_oname_len), p,
                     &size);

    p += size;

    rs_log_import_acct_name(new_login_name, &(info.new_pname_len),
                     &(info.new_gname_len), &(info.new_oname_len), p,
                     &temp_size);

    size = size + temp_size + sizeof(info) - sizeof(info.name_buf);

    rs_log_setup_log_header(&info.upd_hdr, size, RS_LOG_MODULE_ACCT,
                            rs_log_acct_rename_op);

    if (master_info != NULL) {
        rs_log_write(&info.upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&info.upd_hdr, NULL, NULL);
    }
}


PRIVATE void rs_log_acct_prop_rename
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
{
    rs_log_acct_rename_t   *log_info = (rs_log_acct_rename_t *) info;
    sec_rgy_login_name_t   old_acct_name;
    sec_rgy_login_name_t   new_acct_name;
    char                   *p;

    CLEAR_STATUS(st);

    p = &(log_info->name_buf[0]);

    rs_log_export_acct_name(&old_acct_name, log_info->old_pname_len,
                     log_info->old_gname_len, log_info->old_oname_len, p);

    p = p + log_info->old_pname_len + log_info->old_gname_len
              + log_info->old_oname_len;

    rs_log_export_acct_name(&new_acct_name, log_info->new_pname_len,
                     log_info->new_gname_len, log_info->new_oname_len, p);

    rrs_prop_acct_rename(handle, &old_acct_name, &new_acct_name, 
                         master_info, propq_only, st);
 }


PRIVATE void rs_log_acct_replay_rename
  (
    rs_log_rec_t    *info,
    error_status_t  *st
  )
{
    rs_log_acct_rename_t   *log_info = (rs_log_acct_rename_t *) info;
    sec_rgy_login_name_t   old_acct_name;
    sec_rgy_login_name_t   new_acct_name;
    sec_rgy_acct_key_t     new_key_parts;
    char                   *p;

    /* 
     * key parts are obsolete (there is only one account per principal allowed),
     * but still pervasive in the database code. Looks like a v1.1 cleanup task.
     */
    new_key_parts = sec_rgy_acct_key_person;  

    p = &(log_info->name_buf[0]);

    rs_log_export_acct_name(&old_acct_name, log_info->old_pname_len,
                     log_info->old_gname_len, log_info->old_oname_len, p);

    p = p + log_info->old_pname_len + log_info->old_gname_len
              + log_info->old_oname_len;

    rs_log_export_acct_name(&new_acct_name, log_info->new_pname_len,
                     log_info->new_gname_len, log_info->new_oname_len, p);

    CLEAR_STATUS(st);
    rsdb_acct_rename(&old_acct_name, &new_acct_name, &new_key_parts, st);
}

