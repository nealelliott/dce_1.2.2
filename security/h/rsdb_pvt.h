/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_pvt.h,v $
 * Revision 1.1.74.3  1996/10/03  14:51:24  arvind
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:42 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	Define pvtkey_ver field in rsdb_acct_item_t
 * 	[1996/09/06  18:19 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 *
 * 	Merge up from aha_pk8_1
 * 	[1996/08/29  15:41 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 *
 * 	Merge up from aha_pk8
 * 	[1996/08/27  22:25 UTC  aha  /main/DCE_1.2.2/aha_pk8_1/1]
 *
 * 	Add rsdb_acct_register_pvtkey_mechanism() signature
 * 	[1996/08/02  13:02 UTC  aha  /main/DCE_1.2.2/aha_pk8/1]
 *
 * 	Merge Registry migration functionality: database reformat and enable sec_passwd_genprivkey
 * 	[1996/07/31  12:16 UTC  aha  /main/DCE_1.2.2/aha_pk7/2]
 *
 * 	Changes for Public Key Login; redefine size of rsdb_encrypt_key_t to max
 * 	size of an encrypted DES key.
 * 	[1996/07/18  18:45 UTC  aha  /main/DCE_1.2.2/aha_pk7/1]
 *
 * 	Changes for Public Key Login; restore sizes of structures affecting
 * 	only the master key, which is always DES.
 * 	[1996/06/07  19:06 UTC  aha  /main/DCE_1.2.2/aha_pk6/1]
 *
 * 	Changes for Public Key Login: support for Registry storage of KDC private key
 * 	[1996/03/27  17:46 UTC  hanfei  /main/DCE_1.2/2]
 *
 * Revision 1.1.74.2  1996/08/09  12:05:22  arvind
 * 	Merge Registry migration functionality: database reformat and enable sec_passwd_genprivkey
 * 	[1996/07/31  12:16 UTC  aha  /main/DCE_1.2.2/aha_pk7/2]
 * 
 * 	Changes for Public Key Login; redefine size of rsdb_encrypt_key_t to max
 * 	size of an encrypted DES key.
 * 	[1996/07/18  18:45 UTC  aha  /main/DCE_1.2.2/aha_pk7/1]
 * 
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/18  17:32 UTC  aha  /main/DCE_1.2.2/2]
 * 
 * 	Changes for Public Key Login; restore sizes of structures affecting
 * 	only the master key, which is always DES.
 * 	[1996/06/07  19:06 UTC  aha  /main/DCE_1.2.2/aha_pk6/1]
 * 
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:46 UTC  hanfei  /main/DCE_1.2/2]
 * 
 * 	rename rsdb_credentials_foreign_uuid_t to rsdb_credentials_foreign_t
 * 	[1996/02/29  16:21 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/3]
 * 
 * 	name changes of uids ( from uid to uuid ).
 * 	[1996/02/21  15:40 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/2]
 * 
 * 	multi-cell group work: add data structures for foreign_person domain
 * 	and foreign_person database.
 * 	[1996/02/02  18:30 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 * 
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:58  marty]
 * 
 * 	Merged to mothra.
 * 	[1995/12/08  16:53:57  root]
 * 
 * 	Changes for Public Key Login: support for Registry storage of KDC private key
 * 	[1996/03/27  17:46 UTC  hanfei  /main/DCE_1.2/2]
 * 
 * Revision 1.1.74.1  1996/05/10  13:17:48  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:46 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/3  1996/02/29  16:21 UTC  hanfei
 * 	rename rsdb_credentials_foreign_uuid_t to rsdb_credentials_foreign_t
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/21  15:40 UTC  hanfei
 * 	name changes of uids ( from uid to uuid ).
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/02  18:30 UTC  hanfei
 * 	multi-cell group work: add data structures for foreign_person domain
 * 	and foreign_person database.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:11 UTC  psn
 * 	Merged to mothra.
 * 	[1995/05/12  11:42 UTC  sekhar  /main/HPDCE02/1]
 * 
 * Revision 1.1.67.3  1994/07/15  14:59:51  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:17:07  mdf]
 * 
 * Revision 1.1.67.2  1994/04/22  19:13:53  rps
 * 	Named item type changes for rgy polymorphism
 * 	[1994/04/21  00:07:31  rps]
 * 
 * 	Moved location of num_attrs and attr_list_id
 * 	[1994/03/30  16:29:07  rps]
 * 
 * Revision 1.1.67.1  1994/03/02  17:55:05  hanfei
 * 	Initial ERA drop
 * 	[1994/02/18  19:08:58  hanfei]
 * 
 * Revision 1.1.64.1  1993/10/05  22:30:30  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:11:10  mccann]
 * 
 * Revision 1.1.4.6  1993/02/19  17:12:27  ahop
 * 	dbcp: add rsdb_mkey_file_t.
 * 	[1993/02/19  08:41:32  ahop]
 * 
 * Revision 1.1.4.5  1992/12/29  13:26:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:46  zeliff]
 * 
 * Revision 1.1.4.4  1992/11/20  15:51:48  ahop
 * 	Update master_key file definitions
 * 	[1992/11/10  23:06:36  ahop]
 * 
 * Revision 1.1.4.3  1992/10/07  20:28:57  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:13:15  sekhar]
 * 
 * Revision 1.1.5.2  1992/10/06  15:47:11  burati
 * 	 Second replication code drop: bl5
 * 	 Remove master_key globals (now private to rs_mkey.c).
 * 
 * Revision 1.1.4.2  1992/08/31  18:20:27  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:58:27  sekhar]
 * 
 * Revision 1.1.2.3  1992/07/13  20:01:02  ahop
 * 	 3903: define macros for account string_buf access
 * 	[1992/06/30  22:28:55  ahop]
 * 
 * Revision 1.1.2.2  1992/03/30  16:32:34  ahop
 * 	 support locksmith mode
 * 	[1992/03/27  23:44:59  ahop]
 * 
 * Revision 1.1  1992/01/19  14:43:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1994
 * Unpublished work. All Rights Reserved.
 */
/* 
 * Registry Server - private types
 */

#ifndef rsdb_pvt_h__included
#define rsdb_pvt_h__included

#ifdef RSDB_MAIN
#   define EXTERN
#   define INITVAL(X) X
#else
#   define EXTERN extern
#   define INITVAL(X)
#endif

typedef struct rsdb_db *db_handle;

#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rpc.h>

#include <dce/rplbase.h>
#include <dce/id_base.h>
#include <dce/aclbase.h>
#include <krb5/krb5.h>  
#include <krb5/encryption.h>

#include <rs_base.h>
#include <bstring.h>
#include <u_str.h>

#include <initstring.h>
                
/*
 * Define constants for file locations.
 */
#if defined(__STDC__) && !defined(_K_R_PORT_CHECK_)
#   define RGY_FILE_DIRECTORY       DCELOCAL_PATH "/var/security/rgy_data/"
#   define UNIX_FILES_DIRECTORY     RGY_FILE_DIRECTORY "unix"
#   define MKEY_FILE_DIRECTORY      DCELOCAL_PATH "/var/security/"

#else

EXTERN char *rsdb_rgy_file_dir_path INITVAL (= NULL);
#   define RGY_FILE_DIRECTORY \
    STRING_CONCAT(rsdb_rgy_file_dir_path, DCELOCAL_PATH, "/var/security/rgy_data/", "")

EXTERN char *rsdb_unix_file_dir_path INITVAL (= NULL);
#   define UNIX_FILE_DIRECTORY \
    STRING_CONCAT(rsdb_unix_file_dir_path, DCELOCAL_PATH, "/var/security/rgy_data/unix", "")

EXTERN char *rsdb_mkey_file_dir_path INITVAL (= NULL);
#   define MKEY_FILE_DIRECTORY \
    STRING_CONCAT(rsdb_mkey_file_dir_path, DCELOCAL_PATH, "/var/security/", "")

#endif

/*
 *  Macros that reduce (char *) casting in code.
 *  the uxyz macros have been replaced by u_xyz in u_str.h.  These exist
 *  here as a stopgap until all the references in the code have been
 *  updated.
 */
#define ustrcpy(a,b)    u_strcpy(a,b)
#define ustrncpy(a,b,c) u_strncpy(a,b,c)
#define ustrcmp(a,b)    u_strcmp(a,b)
#define ustrncmp(a,b,c) u_strncmp(a,b,c)
#define ustrcat(a,b)    u_strcat(a,b)
#define ustrncat(a,b,c) u_strncat(a,b,c)
#define ustrlen(a)      u_strlen(a)

#define RGY_STATE_FILE   "rgy_state"
#define RGY_FILE         "rgy"
#define ACCT_FILE        "acct"
#define PERSON_FILE      "person"
#define GROUP_FILE       "group"
#define ORG_FILE         "org"
#define REPLICA_FILE     "replicas"
#define LOG_FILE         "update_log"
#define UNIX_PASSWD_FILE "unix/passwd"
#define UNIX_GROUP_FILE  "unix/group"
#define UNIX_ORG_FILE    "unix/org"
#define ACL_FILE         "acl" 
#define MASTER_INFO_FILE "master_info"
#define MASTER_KEY_FILE  ".mkey"
#define ATTR_FILE 	 "attributes"
#define SCHEMA_FILE   	 "attr_schema"
#define LOGIN_FILE       "login_activity"
#define JOURNAL_FILE     "journal_file"
#define FOREIGN_PERSON_FILE "foreign_person"

/*
 * Define a new domain at rsdb level
 *
 * For multi-cell group ( global group ) work, in addition to the
 * existing person, group and org domains, we introduce a fourth domain,
 * and name it foreign_person.  This fourth domain for the moment is 
 * intended to be internal at rsdb level.
 * The purpose to create this new domain is to have a database for 
 * foreign principal.  For multi-cell group work, when a foreign
 * principal is added as a member of local group, there is a data
 * item then store under this domain name after its global name,
 * e.g. a foreign principal /.../cellA/dirA/principalA is stored as 
 * /foregin_principal/cellA/dirA/principalA.
 * There are also new credential block created for this data item.
 * Its indexing method is either via the foreign principal's cell uuid
 * and its principal uuid or its database id ( pvt_id ).
 * ( Presumably this work for creating a fourth domain can be expanded
 * in the future to keep foreign principal unix_id locally.  And this
 * domain will be pulbicized then. )
 * For now, to meet this purpose, we introduce the domain as 
 * sec_rgy_domain_foreign_person (value 3) and replace the usage
 * for sec_rgy_domain_last (value 3) to be sec_rgy_domain_invalid (value 4)
 * at proper code locations.
 */
#define		sec_rgy_domain_foreign_person  (3)
#define		sec_rgy_domain_invalid (4)

/*
 * Global Variables
 */
extern int         rsdb_rgy_fd;
extern db_handle   rsdb_acct_db;
extern db_handle   rsdb_person_db;
extern db_handle   rsdb_group_db;
extern db_handle   rsdb_org_db;
extern db_handle   rsdb_replica_db;
extern db_handle   rsdb_log_db;
extern db_handle   rsdb_acl_db;        
extern db_handle   rsdb_attr_db;        
extern db_handle   rsdb_attr_schema_db;        
extern db_handle   rsdb_login_db;
extern db_handle   rsdb_journal_db;
extern db_handle   rsdb_foreign_person_db;

/* Checkpoint task constants */
#define CHKPT_TASK_STACK_SIZE   64000
#define CHKPT_TASK_PRIORITY     1           /* lowest priority, same as dist. task */

#define CHECKPT_INTERVAL_HRS    2

/* These values used as special case rsdb_pvt_ids in some sequential id lookups. */
#define START_OF_LIST    0
#define END_OF_LIST     -1
#define RSDB_POLICY_OBJ_ID -2
#define RSDB_REPLIST_OBJ_ID -3
#define RSDB_ATTR_SCHEMA_OBJ_ID -4

/*
 *   Used in rsdb_named_item_t type field
 */
typedef enum named_item_type_t {
	rsdb_pgo_type = 0x00000001,
	rsdb_dir_type = 0x00000002
} named_item_type_t;

#define rsdb_auth_pepper_max_len sec_passwd_str_max_len
#define rsdb_auth_pepper_size    sec_passwd_str_t_size  /* size = max_len + 1 */

/* The encrypted key length is algorithmically determined.
 * See krb5_encrypt_size in krb5/include/krb5/encryption.h
 * and usage in rs/rs_mkey.c/rs_mkey_{encrypt,decrypt}_key
 */

/* For DCE 1.0 DES keys, the max_len is 28 bytes:
 *    8 bytes (56 bits) of key
 * + 12 bytes of pad (CRC32_CKSUM_LENGTH + sizeof(mit_des_cblock))
 * rounded up to next multiple of the blocksize, (sizeof(mit_des_cblock))
 * +  4 bytes of key length (sizeof(krb5_encrypted_keyblock.length))
 */
#define rsdb_auth_encrypt_des_key_max_len \
          (krb5_encrypt_size_static(sec_passwd_c_des_key_size, 12, 8) + 4)

/* For DES-encrypted DCE 1.2.2 Public Keys, the max_len is 532 bytes +
 * sizeof(RSA private key structure):
 *   512 bytes (4096 bits) of key
 * + sizeof(RSA private key structure) (both captured in sec_passwd_c_max_pk_key_size)
 * + 12 bytes of pad (CRC32_CKSUM_LENGTH + sizeof(mit_des_cblock))
 * rounded up to next multiple of the blocksize, (sizeof(mit_des_cblock))
 * +  4 bytes of key length (sizeof(krb5_encrypted_keyblock.length))
 */

#define rsdb_auth_encrypt_pk_key_max_len \
          (krb5_encrypt_size_static(sec_passwd_c_max_pk_key_size, 12, 8) + 4)

/* Max encrypt key is max of preceding key max lengths */
#define max(a, b) ((a) > (b) ? (a) : (b))
#define rsdb_auth_encrypt_key_max_len  \
          (max(rsdb_auth_encrypt_pk_key_max_len, rsdb_auth_encrypt_des_key_max_len))

/* rsdb_auth_key_max_len is length of rsdb_auth_key_t buffer; which always
 * holds a master key, which must always be DES (for now)
 */
#define rsdb_auth_key_max_len sec_passwd_c_des_key_size
#define rsdb_auth_key_min_version_number    1
#define rsdb_auth_key_max_version_number    255

#define RSDB_AUTH_DEFAULT_KEY_TYPE  KEYTYPE_DES
#define RSDB_AUTH_DEFAULT_ENCTYPE   ETYPE_DES_CBC_CRC 
/* Master Key principal name */
#define n_master_key    "K/M"

typedef struct rsdb_realm_info {
    krb5_encrypt_block *eblock;
    krb5_pointer rseed;
} rsdb_realm_info;

typedef struct rsdb_locksmith_info_t {
    boolean32               locksmith_mode; /* true if in locksmith mode */
    boolean32               remote_ok;      /* true if remote locksmith client ok */
    uuid_t                  uuid;
} rsdb_locksmith_info_t;

/* rsdb_encrypt_key_t is used ONLY for an encrypted master key, which
 * currently must be a DES key.
 */
typedef struct rsdb_encrypt_key_t {
    krb5_kvno               mkey_version;   /* vno of mkey used for encryption */
    krb5_keytype            keytype;
    long                    length;
    char                    contents[rsdb_auth_encrypt_des_key_max_len];
} rsdb_encrypt_key_t;

/* rsdb_auth_key_t is used only for a master key,
 * which must always be DES (for now)
 */
typedef struct rsdb_auth_key_t {
    krb5_kvno               key_version;   /* vno of key */
    krb5_keytype            keytype;
    long                    length;
    char                    contents[rsdb_auth_key_max_len];
} rsdb_auth_key_t;

/* Master Key File Format */
#define rsdb_c_mkey_file_version   2

typedef struct rsdb_mkey_file_t {
    unsigned32              file_version;
    rsdb_auth_key_t         mkey;
} rsdb_mkey_file_t;

/*
 * Macros
 */
#define IS_SET(FIELD, MASK) ((FIELD) & (MASK)) == (MASK)

#define RSDB_IS_DIR(item)	( (item)->type & rsdb_dir_type )
#define RSDB_IS_PGO(item)	( (item)->type & rsdb_pgo_type )

#define RSDB_DIR(item)		( *( (item)->type == rsdb_dir_type ? \
					&(item)->body.dir : \
					&(item)->body.pgodir.dir ) )

#define RSDB_PGO(item)		( *( (item)->type == rsdb_pgo_type ? \
					&(item)->body.pgo : \
					&(item)->body.pgodir.pgo ) )

#define NAME_LEN(n) ((n)->name_len + sizeof(*(n)) - sizeof((n)->name))
#define PGO_LEN(n) ((n)->fullname_len + sizeof(*(n)) - sizeof((n)->fullname))   
/*
 *  The NAMED_ITEM_LEN define assumes that pgo_item_t is larger than dir_item_t;
 *  for those cases where (n)->type is not defined, use the larger size.
 */
#define NAMED_ITEM_LEN(n) ( \
	(n)->type == rsdb_dir_type ? \
		sizeof(*n) - sizeof((n)->body) + sizeof((n)->body.dir) : \
	(n)->type == rsdb_pgo_type ? \
		sizeof(*n) - sizeof((n)->body) + PGO_LEN(&((n)->body.pgo)) : \
	/* else aggregate */ \
		sizeof(*n) - sizeof((n)->body) + \
			sizeof((n)->body.pgodir.dir) + \
			PGO_LEN(&((n)->body.pgodir.pgo)))

#define RSDB_IS_AN_ALIAS(P) ((P)->flags & sec_rgy_pgo_is_an_alias)
#define RSDB_IS_PROJLIST_OK(P) ((P)->flags & sec_rgy_pgo_projlist_ok)    

#define VALID_DOMAIN(d) ( ((d) < sec_rgy_domain_person || \
                         (d) >= sec_rgy_domain_last) ? false : true )

#define AUTH_SIZE(auth) ( (auth)->pepper_len + (auth)->key_len  \
    + sizeof(*(auth)) - sizeof((auth)->buf) )

#define MULT_KEY_VERS_OK(acct) \
    ((acct)->flags & rsdb_acct_item_mult_versions_ok)

#define PK_AUTHKEY_OK(acct) \
    ((acct)->flags & rsdb_acct_item_privkey_ok)

#define PK_AUTHKEY_EXISTS(acct) \
    ((((acct)->flags) & rsdb_acct_item_privkey_ver) != 0)

#define PK_PRIVKEY_VER_GET(acct) \
    (((acct)->flags & rsdb_acct_item_privkey_ver)>>8)

#define PK_PRIVKEY_VER_SET(acct, ver) \
    ((acct->flags) = ((acct->flags&0xFF) | ((ver&0xFF)<<8)))

#define AUTH_KEY_EXPIRED(auth, time) \
    ((auth)->key_expire_time > time ? false : true)

#define ACCT_SHELL(acct)   (&((acct)->string_buf[(acct)->user_part.homedir_len]))
#define ACCT_GECOS(acct)   (&((acct)->string_buf[ \
            (acct)->user_part.homedir_len + (acct)->user_part.shell_len]))
#define ACCT_PEPPER(acct)   (&((acct)->string_buf[ \
            (acct)->user_part.homedir_len + (acct)->user_part.shell_len + \
            (acct)->user_part.gecos_len]))
#define ACCT_KEY(acct)      (&((acct)->string_buf[ \
            (acct)->user_part.homedir_len + (acct)->user_part.shell_len + \
            (acct)->user_part.gecos_len + (acct)->key_pepper_len]))


/*
 * Types
 */
#define SEQUENTIAL_TYPE     'S'
#define DUMMY_TYPE          'D'
#define NAME_TYPE           'N'
#define UUID_TYPE           'U'
#define UUIDS_TYPE          'u'
#define UNIX_ID_TYPE        '#'
#define MEMBER_LIST_TYPE    'M'
#define POLICY_TYPE         'P'
#define AUTH_TYPE           'A'
#define MAIL_TYPE           'm'
#define COMPAT_PASSWD_TYPE  'C'                         
#define DIRECTORY_UUID_TYPE 'd'   
#define CREDENTIALS_ID_TYPE 'c'
#define ATTR_NAME_TYPE 	    'n'


 /*
  * Note about pvt id's - These are 30 bit quantities, the sign bit is used to
  * denote deleted objects, the next bit is reserved for context specific
  * information.  In most cases, these private id's are defined as "long" in
  * order to avoid masking the special bits which are only used in membership
  * lists and attribute lists (see rsdb_attr_pvt.h as of OSF DCE 1.1). 
  */

typedef struct rsdb_member_id_t {
    unsigned int    deleted:1;
    unsigned int    has_acct:1;
    unsigned int    id:30;
} rsdb_member_id_t;

typedef unsigned long rsdb_pvt_id_t;

typedef struct rsdb_sequential_key_t {
    short           record_type;
    rsdb_pvt_id_t   id;
} rsdb_sequential_key_t;

typedef struct rsdb_membership_list_key_t {
    short           record_type;
    rsdb_pvt_id_t   id;
} rsdb_membership_list_key_t;

typedef struct rsdb_policy_key_t {
    short           record_type;
    rsdb_pvt_id_t   id;
} rsdb_policy_key_t;

typedef struct rsdb_credentials_t {
    uuid_t          uid;
    long            unix_id;
    rsdb_pvt_id_t   id;
    rsdb_pvt_id_t   acl_id;          
} rsdb_credentials_t;

/* <cell_uuid, princ_uuid> as index to foreign_person domain */
typedef struct rsdb_credentials_foreign_t {
    uuid_t	    cell_uuid;
    uuid_t          princ_uuid;
    long            unix_id;	/* = -1 not used for DCE 1.2.2 */
    rsdb_pvt_id_t   id;
    rsdb_pvt_id_t   acl_id;     /* = 0 not used for DCE 1.2.2 */         
} rsdb_credentials_foreign_t;

typedef struct rsdb_foreign_uuids_key_t {
    short           record_type;
    uuid_t          cell_uuid;
    uuid_t          princ_uuid;
} rsdb_foreign_uuids_key_t;

typedef struct rsdb_name_key_t {
    short           record_type;
    rsdb_pvt_id_t   parent_id;
    long            name_len;
    sec_rgy_name_t  name;
} rsdb_name_key_t;

typedef struct rsdb_uid_key_t {
    short           record_type;
    uuid_t          uid;
} rsdb_uid_key_t;

typedef struct rsdb_unix_id_key_t {
    short           record_type;
    long            unix_id;
} rsdb_unix_id_key_t;

typedef struct rsdb_authkey_key_t {
    short                   record_type;        /* AUTH_TYPE */
    krb5_kvno               version;
    krb5_keytype            key_type;
    rsdb_pvt_id_t           acct_id;
} rsdb_authkey_key_t;


typedef struct rsdb_thread_hdr_t {
    rsdb_pvt_id_t  id;
    rsdb_pvt_id_t  prev;
    rsdb_pvt_id_t  next;
    rsdb_pvt_id_t  alias;
} rsdb_thread_hdr_t;

typedef struct rsdb_pgo_item_t {
    rsdb_thread_hdr_t      threads;
    rsdb_pvt_id_t          membership;
    long                   num_members;
    rsdb_pvt_id_t          info;
    long                   unix_id;
    long                   num_foreign_members;
    rsdb_pvt_id_t          foreign_membership;
    short                  fullname_len;
    sec_rgy_pgo_flags_t    flags;
    signed32               quota;
    long                   num_attrs;
    rsdb_pvt_id_t          attr_list_id;
    sec_rgy_name_t         fullname;	/* must be last */
} rsdb_pgo_item_t;


/*
 *   Directory Item 
 */
typedef struct rsdb_dir_item_t {
    rsdb_thread_hdr_t   threads;        /* ID and link list info            */
    uuid_t              uid;            /* this directory's UUID            */
    rsdb_pvt_id_t       acl;            /* ACL for this directory           */
    rsdb_pvt_id_t       def_dir_acl;    /* new directory default ACL        */
    rsdb_pvt_id_t       def_obj_acl;    /* new object default ACL           */
    long		num_attrs;
    rsdb_pvt_id_t	attr_list_id;
    rsdb_pvt_id_t       reserved;       /* reserved for future use          */
} rsdb_dir_item_t;    


/*
 * Aggregate of pgo and directory (so both allocated only when necessary)
 */
typedef struct rsdb_pgodir_item_t {
    rsdb_dir_item_t	dir;
    rsdb_pgo_item_t	pgo;
} rsdb_pgodir_item_t;


 /*
  * RSDB Named Item: Union of Pgo Item and Directory Item   
  *
  * The "threads" member of the "body" union exists for manipulation of the
  * threads header of either the PGO or DIR item when the type is not known. 
  */
typedef struct rsdb_named_item_t {
    int				type;	/* set of named_item_type_t */
    union { rsdb_thread_hdr_t   threads;
            rsdb_pgo_item_t     pgo;
            rsdb_dir_item_t     dir;
	    rsdb_pgodir_item_t	pgodir;
    }                       body;              
} rsdb_named_item_t;

#define RSDB_ITEM_MAX_ACLS		( 3 + 1 )		/* dir + pgo */
#define RSDB_ITEM_NUM_ACLS(item)	( RSDB_IS_DIR(item) ? \
						(RSDB_IS_PGO(item) ? 3+1 : 3) \
						: 1 )

 
/* ACL structures:
 * Store ACLs in the rsdb_acl_db database keyed on acl_id, an
 * rsdb_sequential_key_t.
 */          

typedef struct {
    uuid_t          uuid;
    long            name_len;
} rsdb_id_t;

typedef struct {
    rsdb_id_t       id;
    rsdb_id_t       realm;
} rsdb_id_foreign_t;

/*  r s d b _ a c l _ e n t r y _ t
 *
 *  An rsdb_acl_entry_t is an individual entry contained in an rsdb_acl_t.  Each 
 *  entry contains a type tag (identifying the role for the entry's data).
 *  A sec_acl_entry_t contains a set of permissions, and other optional
 *  information.  The optional information currently divides into
 *  three categories:
 *      1) Keys - identifier for the entry (principal or group name - uuid)
 *      2) Foreign Keys -   same as above with the addition of the
 *                          uuid identifying the foreign realm.
 *      3) extended data -  additional 'pickled' data.  This kind of entry
 *                          cannot be interpreted, but can be used by an
 *                          out-of-date client when copying an acl from 
 *                          one manager to another (assuming that the two
 *                          managers each understand the data).
 */
typedef struct {
    sec_acl_permset_t       perms; 
    sec_acl_entry_type_t    entry_type;
    union {
        /* No additional fields for following entry_types:
         * sec_acl_e_type_user_obj
         * sec_acl_e_type_group_obj
         * sec_acl_e_type_other_obj
         * sec_acl_e_type_unauthenticated
         */

        /* ID for the following entry_types:
         * sec_acl_e_type_user
         * sec_acl_e_type_group
         * sec_acl_e_type_foreign_other
         */             
        rsdb_id_t           id;
           
        /* Foreign_id for the following entry types:
         * sec_acl_e_type_foreign_user
         * sec_acl_e_type_foreign_group
         */
        rsdb_id_foreign_t   foreign_id;
                                          
        /* Extended_info or any other case not supported
         */
    } entry_info;                      
    unsigned_char_t         name_buf[VARYING];
} rsdb_acl_entry_t;                                  
     

/* NOTE: Use rsdb_acl_fixed_t when allocating memory for acl data store.
 * Use rsdb_acl_t for easier access to variable length parts of acl.
 */

/*  r s d b _ a c l _ f i x e d _ t
 * 
 *  An rsdb_acl_fixed_t is a complete acl containing a list of rsdb_acl_entry_t
 *  members.  The default realm identifies the authentication authority 
 *  for simple acl entries (foreign entries already identify the foreign
 *  realms.  The sec_acl_manager_type identifies the manager that is
 *  interpreting this acl. 
 *
 *  NOTE: For storage, a buffer is allocated that is large enough
 *  to hold the fixed part of the acl {sizeof(rsdb_acl_fixed_t)} followed by
 *  the fixed parts for the required number of acl entries
 *  {sizeof(rsdb_acl_entry_t)} followed by the variable length part
 *  (default_realm name) of the acl_fixed_t followed by the variable length
 *  part (id/foreign_id names) of each acl_entry_t in succession.
 *  The variable length data is written starting in the name_buf field
 *  of the last acl_entry_t in the acl.  If acl->num_entries == 0, the
 *  variable length data is written starting in the name_buf field
 *  of a dummy rsdb_acl_entry_t, the only entry in acl->entries.
 */

typedef struct {
    rsdb_thread_hdr_t       threads;
    rsdb_id_t               default_realm;
    uuid_t                  sec_acl_manager_type;
    unsigned32              num_entries;  
    long                    acl_len;
    rsdb_acl_entry_t        entries[VARYING];
} rsdb_acl_fixed_t;
   
                                
/* This max_num_acl_entries is somewhat arbitrary.  It's meant to 
 * be huger than will ever be required.
 */
#define MAX_NUM_ACL_ENTRIES 1024

/* r s d b _ a c l _ t 
 *
 * This structure allows access to more entries than should ever be needed.
 * This structure is intended to be pointed at, not to be allocated
 * automatically or used in a sizeof() call.
*/
typedef struct {
    rsdb_thread_hdr_t       threads;
    rsdb_id_t               default_realm;
    uuid_t                  sec_acl_manager_type;
    unsigned32              num_entries;  
    long                    acl_len;
    rsdb_acl_entry_t        entries[MAX_NUM_ACL_ENTRIES];
} rsdb_acl_t;


typedef struct {
    rsdb_named_item_t   first;
    rsdb_named_item_t   last;
    db_handle           db;
} domain_info_t;			/* Used by domain_info[]. Now defined in rsdb.c */

extern domain_info_t domain_info[];     /* Defined in rsdb.c */


#define MAX_MEMBER_LIST_SIZE 80     /* Must fit in a short */

typedef struct rsdb_membership_list_t {
    rsdb_pvt_id_t      next;
    short              total_count;
    short              count;
    rsdb_member_id_t   member[VARYING];
} rsdb_membership_list_t;

typedef struct rsdb_max_membership_list_t {
    rsdb_pvt_id_t      next;
    short              total_count;
    short              count;
    rsdb_member_id_t   member[MAX_MEMBER_LIST_SIZE];
} rsdb_max_membership_list_t;

typedef struct rsdb_acct_user_item_t {
    sec_rgy_unix_passwd_buf_t  passwd;   /* UNIX encrypted key, null-term */
    sec_timeval_sec_t          passwd_dtm;                             
    short                      homedir_len;
    short                      shell_len;
    short                      gecos_len;
    sec_rgy_acct_user_flags_t  flags;
} rsdb_acct_user_item_t;

typedef unsigned16 rsdb_acct_item_flags_t;

/* multiple auth key versions allowed */
#define rsdb_acct_item_mult_versions_ok     1

/* private key auth key allowed */
#define rsdb_acct_item_privkey_ok          2

/* private key version */
/* private key version uses the 8 most significant bits of acct->flags */
#define rsdb_acct_item_privkey_ver         0xFF00

#define RSDB_ACCT_STRING_BUF_SIZE \
    (3 * sizeof(sec_rgy_pname_t)) + rsdb_auth_pepper_max_len + rsdb_auth_encrypt_des_key_max_len

typedef struct rsdb_acct_item_t {
    rsdb_thread_hdr_t      threads;
    rsdb_pvt_id_t          person_id;
    rsdb_pvt_id_t          group_id;
    rsdb_pvt_id_t          org_id;
    rsdb_pvt_id_t          login_activity_id; /*  replaces long padding1;  */
    sec_rgy_acct_key_t     key_parts;
    sec_rgy_acct_admin_t   admin_part;
    rsdb_acct_user_item_t  user_part;
    sec_rgy_plcy_auth_t    auth_plcy;
    rsdb_acct_item_flags_t flags;              
    krb5_kvno              cur_key_version;    /* always refers to DES keytype */
    krb5_kvno              next_key_version;   /* next most recent version in list */
    short                  default_pepper;     /* true if def pepper alg used */
    short                  key_pepper_len;
    short                  key_len;
    krb5_kvno              mkey_version;        /* mkey version used for encryption */
    char                   string_buf[RSDB_ACCT_STRING_BUF_SIZE];
} rsdb_acct_item_t;

/*  AUTHKEY LOOKUPS
 *
 * NOTE: Num_lookups assumes that the rsdb_acct_item_t of interest 
 * has already been retrieved from the database.  Num_lookups is
 * the number of database retrieves required to assemble the 
 * desired information.
 *
 * To retrieve an authkey, given its version number and keytype:
 * Num_lookups = 1.
 *  1.  Construct an AUTH_TYPE database key (rsdb_authkey_key_t) with
 *      record_type AUTH_KEY, given version number and keytype, and
 *      sequential ID of the related rsdb_acct_item_t.
 *  2.  Use this key to retrieve the rsdb_authkey_item_t, if it exists.
 *
 * To retrieve the current key for keytype DES:
 * Num_lookups = 0.
 *  1.  Retrieve the rsdb_acct_item_t associated with desired key.
 *  2.  rsdb_acct_item_t contains all required info about current key:
 *      cur_key_version, key_pepper, key.
 *
 *  If sec_rgy_acct_admin_mult_key_vers is false (and multiple
 *  keytypes are not supported), there will be no rsdb_authkey_item_t
 *  stored for this account.  The only authkey information available
 *  is the current version in the rsdb_acct_item_t.
 */
typedef struct rsdb_authkey_item_t {
    krb5_kvno               next_version;   /* next most recent version in list */
    short                   default_pepper; /* if true, pepper not stored */
    short                   pepper_len;
    short                   key_len;
    sec_timeval_sec_t       key_expire_time;/* not set while current version */
    char                    buf[rsdb_auth_pepper_max_len + 
                                rsdb_auth_encrypt_key_max_len];
} rsdb_authkey_item_t;



/*
 ******************************************************************************
 **   Multiple Keytype Support
 ******************************************************************************
 * 
 * #define KEYTYPE_TYPE        'K'
 *
 * typedef struct rsdb_keytype_key_t {
 *     short                   record_type;        * KEYTYPE_TYPE *
 *     krb5_keytype            key_type;
 *     rsdb_pvt_id_t           acct_id;
 * } rsdb_keytype_key_t
 * 
 *  
 * typedef struct rsdb_keytype_rec_t {
 *     krb5_keytype            next_key_type;
 *     krb5_kvno               cur_version;
 * } rsdb_keytype_rec_t;  
 * 
 * To retrieve the current key for keytype other than DES (not yet supported):
 * Num_lookups = 2 + n, where n is distance between DES keytype element and
 * desired keytype element.  For example, if keytype XYZ is the next keytype
 * element in the list after DES, n = 1.
 *  1.  Construct a KEYTYPE_TYPE database key (rsdb_keytype_key_t) for
 *      the first keytype (always DES) in the list using record_type KEYTYPE_TYPE,
 *      DES as key_type, and the sequential ID of the related rsdb_acct_item_t.
 *  2.  Use this key to retrieve the rsdb_keytype_rec_t.
 *  3.  Construct another KEYTYPE_TYPE database key using as its keytype
 *      the next_key_type field from the rsdb_keytype_rec_t retrieved in step 2.
 *  4.  Use this key to retrieve the rsdb_keytype_rec_t.
 *      If the key_type field in the rsdb_keytype_key_t is the desired keytype,
 *      then the cur_version field of the retrieved rsdb_keytype_rec_t 
 *      identifies the current version number for the requested key_type;
 *      go to step 5.
 *      If the key_type field in the rsdb_keytype_key_t is NOT the desired keytype,
 *      repeat steps 3 and 4.
 *  5.  Construct an AUTH_KEY database key using desired key_type, cur_version
 *      from the rsdb_keytype_rec_t in the version field, and 
 *      sequential ID of the related rsdb_acct_item_t.
 *  6.  Use this key to retrieve the rsdb_authkey_item_t.
 * 
 * NOTE: Since multiple key_types are not supported, there will be absolutely
 * no use of the rsdb_keytype_key_t and rsdb_keytype_rec_t structures in the
 * current version.
 * These structures are not required because:
 * - only keytype DES is supported; there is no need for a "next_keytype" field,
 * - the current version number for DES is already in the rsdb_acct_item_t
 * 
 */

/*
 * Internal Routines
 */
  

void rsdb_fetch(
#ifdef __STDC__
    db_handle       db,
    Pointer         key,
    int             key_len,
    Pointer         *data,
    int             *data_len,
    error_status_t  *status
#endif
    );

void rsdb_fetch_next(
#ifdef __STDC__
    db_handle       db,
    Pointer         key,
    int             key_len,
    Pointer         *next_key,
    int             *next_key_len,
    Pointer         *data,
    int             *data_len,
    error_status_t  *status
#endif
    );

void rsdb_store(
#ifdef __STDC__
    db_handle       db,
    Pointer         key,
    int             key_len,
    Pointer         data,
    int             data_len,
    error_status_t  *status
#endif
    );

void rsdb_replace(
#ifdef __STDC__
    db_handle       db,
    Pointer         key,
    int             key_len,
    Pointer         data,
    int             data_len,
    error_status_t  *status
#endif
    );

void rsdb_delete(
#ifdef __STDC__
    db_handle       db,
    Pointer         key,
    int             key_len,
    error_status_t  *status
#endif
    );

void rsdb_change_credentials(
#ifdef __STDC__
    db_handle           db,
    rsdb_credentials_t  *old_cr,
    uuid_t              *uid,
    int                 unix_id,
    rsdb_pvt_id_t       id,
    rsdb_pvt_id_t       acl_id
#endif
    );

void rsdb_purge_credentials (
#ifdef __STDC__
    db_handle           db,
    rsdb_credentials_t  *credentials
#endif
);

boolean32 rsdb_get_credentials_by_uid(
#ifdef __STDC__
    db_handle           db,
    uuid_t              *uid,
    rsdb_credentials_t  *credentials
#endif
    );

boolean32 rsdb_get_credentials_by_unix_id(
#ifdef __STDC__
    db_handle           db,
    int                 unix_id,
    rsdb_credentials_t  *credentials
#endif
    );

/*
 * rsdb_policy
 */

void rsdb_sequential_key_get (
#ifdef __STDC__
    rsdb_pvt_id_t  *key
#endif
);

void rsdb_sequential_key_store (
#ifdef __STDC__
    rsdb_pvt_id_t  key
#endif
);

rsdb_pvt_id_t  rsdb_new_sequential_id (
#ifdef __STDC__
    void
#endif
);

rsdb_pvt_id_t  rsdb_new_primary_key (
#ifdef __STDC__
    db_handle   db,
    Pointer     data,
    int         data_len
#endif
);

void rsdb_purge_sequential_key (
#ifdef __STDC__
    db_handle       db,
    rsdb_pvt_id_t   id
#endif
);
    
/* 
 * The following functions are defined in rsdb_policy.c
 */

void rsdb_get_cache_state ( 
#ifdef __STDC__
    sec_timeval_sec_t  *p_dtm,
    sec_timeval_sec_t  *g_dtm,
    sec_timeval_sec_t  *o_dtm
#endif
);

void rsdb_set_domain_cache_state ( 
#ifdef __STDC__
    sec_rgy_domain_t   domain,
    sec_timeval_sec_t  *dtm
#endif
);

void rsdb_get_domain_cache_state ( 
#ifdef __STDC__
    sec_rgy_domain_t   domain,
    sec_timeval_sec_t  *dtm
#endif
);
 
void rsdb_set_init_replicas ( 
#ifdef __STDC__
    boolean32   state
#endif
);

boolean32 rsdb_must_init_replicas ( 
#ifdef __STDC__
    void
#endif
);

void rsdb_init_rgy_info ( 
#ifdef __STDC__
    sec_id_t            *rgy_creator,
    sec_id_t            *rgy_local_cell,
    unsigned_char_p_t   myname
#endif
);

void rsdb_load_rgy_info ( 
#ifdef __STDC__
    int  fd
#endif
);

void rsdb_store_rgy_info ( 
#ifdef __STDC__
    int  fd
#endif
); 

error_status_t rsdb_acct_register_pvtkey_mechanism();

#endif

