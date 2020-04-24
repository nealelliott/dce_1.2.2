/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_krb_util.c,v $
 * Revision 1.1.13.2  1996/10/03  15:15:06  arvind
 * 	Silence compiler warnings.
 * 	Eliminate sec_krb_name_parsed_cmp.
 * 	[1996/09/16  23:32 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:43 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	Change sec_passwd_privkey -> sec_passwd_pubkey
 * 	[1996/09/10  21:08 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 *
 * 	Merge Registry support for KDC private key storage
 * 	[1996/06/18  20:56 UTC  aha  /main/aha_pk6/1]
 *
 * 	Changes for Public Key Login; support Registry storage of KDC private key.
 * 	[1996/02/18  00:23:02  marty  1.1.11.3]
 *
 * Revision 1.1.13.1  1996/08/09  12:12:48  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/06/18  20:56 UTC  aha  /main/aha_pk6/1]
 * 
 * 	Changes for Public Key Login; support Registry storage of KDC private key.
 * 	[1996/02/18  00:23:02  marty  1.1.11.3]
 * 
 * Revision 1.1.11.3  1996/02/18  00:23:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:36  marty]
 * 
 * Revision 1.1.11.2  1995/12/08  18:03:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:13  root]
 * 
 * Revision 1.1.7.1  1994/10/25  17:18:36  mob
 * 	Fix for OT 12545
 * 	[1994/10/24  20:37:52  mob]
 * 
 * Revision 1.1.5.1  1994/07/15  15:03:18  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:24:31  mdf]
 * 
 * Revision 1.1.2.3  1993/02/03  22:10:06  ahop
 * 	bl6e: realm should be freed after call to krb5_get_default_realm
 * 	[1993/01/22  22:44:54  ahop]
 * 
 * Revision 1.1.2.2  1992/12/29  16:41:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:32:21  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:41:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1993
 * Unpublished work. All Rights Reserved.
 */
/*   
 * DCE security/kerberos name conversion routines
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)sec_krb_util.c	7 - 12/10/91";
#endif

#include <un_maloc.h>
#include <u_str.h>
#include <rgymacro.h>

#include <dce/sec_base.h>
#include <dce/rgynbase.h>

#include <sec_krb_util.h>
#include <crypt.h>
#include <krb5/kdb.h>
#include <krb5/los-proto.h>

PUBLIC char *ptgs_princ_name = SEC_PRIV_SERVER_NAME;

/* d c e _ c e l l _ t o _ k r b _ r e a l m    
 *
 * A kerberos realm name is just the DCE cell name
 * without the DCE global root. 
 * Caller must free returned pointer.
 */
PRIVATE  char *  dce_cell_to_krb_realm
#ifndef __STDC__
    ( cell, stp ) 
    char            cell[];
    error_status_t  *stp;
#else
  (
    char            cell[],
    error_status_t  *stp
  )
#endif
{
    char            *rp = (char *)0;
    signed32        len;

    if (cell == NULL || *cell == '\0') {
        SET_STATUS(stp, sec_rgy_bad_name);
        return rp;
    }

    /*
     * If a global name (/.../ relative) strip off the prefix
     */
    if (u_strncmp(cell, GLOBAL_DIR_ROOT, GLOBAL_DIR_ROOT_LEN) == 0) {
        len = u_strlen(cell) - GLOBAL_DIR_ROOT_LEN;
        if (!(rp = malloc(len + 1))) {
            SET_STATUS(stp, sec_rgy_cant_allocate_memory);
            return rp;
        }
        u_strcpy(rp, cell + GLOBAL_DIR_ROOT_LEN);
    } else if (u_strncmp(cell, CELL_DIR_ROOT, CELL_DIR_ROOT_LEN-1) == 0
              && (cell[CELL_DIR_ROOT_LEN-1] == '\0' 
                   || cell[CELL_DIR_ROOT_LEN-1] == PATH_COMPONENT_SEPARATOR)) {
        /*
         * If we name the local cell (/.:) then return the local realm.
         */
        (void) krb5_get_default_realm(&rp);
    }

    return rp;
}

/* s e c _ k r b _ s t r i n g _ f r o m _ d c e _ p a i r
 *
 * Generate a primitive principal name <princ@realm> from a DCE
 * cell and relative principal name.  The krb princ name will be
 * allocated for the caller and is to be freed by the caller.
 */
PUBLIC void sec_krb_string_from_dce_pair
#ifndef __STDC__
    ( cell_name, princ_name, krb_princ, stp )  
    char            *cell_name;
    char            *princ_name;
    char            **krb_princ;
    error_status_t  *stp;
#else
  (
    char            *cell_name,
    char            *princ_name,
    char            **krb_princ,
    error_status_t  *stp
  )
#endif
{
    char             *unparsed;
    char             *realm = (char *)0;
    signed32         realm_len = 0;
    
    CLEAR_STATUS(stp);
    *krb_princ = NULL;

    /* can't do anything with a NULL principal  or cell name */
    if (princ_name == NULL || *princ_name == '\0') {
        SET_STATUS(stp, sec_rgy_bad_name);
	return;
    }

    if (cell_name && *cell_name) {
        realm = dce_cell_to_krb_realm(cell_name, stp);
        if (BAD_STATUS(stp)) {
            return;
        }
        realm_len = u_strlen(realm);
    }
    
    unparsed = malloc(u_strlen(princ_name) + realm_len + 2);
    if (unparsed != NULL) {

	u_strcpy(unparsed, princ_name);
        
	if (realm_len) {
	    u_strcat(unparsed, "@");
            u_strcat(unparsed, realm);
	}

	*krb_princ = unparsed;
    } else {
       SET_STATUS(stp, sec_rgy_cant_allocate_memory);
    }
    if(realm) {
        free(realm);
    }
}

/* s e c _ k r b _ p r i n c _ a n d _ c e l l_ f r o m _ d c e
 *
 * Generate a primitive principal name <princ@realm> from a DCE
 * cell and principal name.  The krb princ name will be
 * allocated for the caller and is to be freed by the caller.
 */
PUBLIC void sec_krb_tgtprinc_from_dce_cell_pair
#ifndef __STDC__
    ( cell_name, princ_name, krb_princ, stp )  
    char            *cell_name;
    char            *princ_name;
    char            **krb_princ;
    error_status_t  *stp;
#else
  (
    char            *cell_name,
    char            *princ_name,
    char            **krb_princ,
    error_status_t  *stp
  )
#endif
{
    char             *unparsed;
    char             *realm = (char *)0;
    char             *cl_princ_name = (char *)0;
    signed32         cl_princ_name_len = 0;
    signed32         realm_len = 0;
    
    CLEAR_STATUS(stp);
    *krb_princ = NULL;

    /* can't do anything with a NULL principal  or cell name */
    if (princ_name == NULL || *princ_name == '\0') {
        SET_STATUS(stp, sec_rgy_bad_name);
	return;
    }
    else{
    	cl_princ_name = dce_cell_to_krb_realm(princ_name, stp);
    	if (BAD_STATUS(stp)) {
        	return;
	}
        cl_princ_name_len = u_strlen(cl_princ_name);
    }

    if (cell_name && *cell_name) {
        realm = dce_cell_to_krb_realm(cell_name, stp);
        if (BAD_STATUS(stp)) {
            return;
        }
        realm_len = u_strlen(realm);
    }
    
    unparsed = malloc(cl_princ_name_len + realm_len + 2);
    if (unparsed != NULL) {

	u_strcpy(unparsed, cl_princ_name);
        
	if (realm_len) {
	    u_strcat(unparsed, "@");
            u_strcat(unparsed, realm);
	}

	*krb_princ = unparsed;
    } else {
       SET_STATUS(stp, sec_rgy_cant_allocate_memory);
    }
    if(realm) {
        free(realm);
    }
}

/* s e c _ k r b _ p r i n c _ f r o m _ d c e _ p a i r
 *
 * convert a dce authentication cell/principal name pair into
 * a krb5_principal.  The global root prefix is stripped from
 * DCE cell name to produce the kerberos realm name.  
 */
PUBLIC  void  sec_krb_princ_from_dce_pair
#ifndef __STDC__
    ( cell_name, princ_name, krb_princ, stp )  
    char            *cell_name;
    char            *princ_name;
    krb5_principal  *krb_princ;
    error_status_t  *stp;
#else
  (
    char            *cell_name,
    char            *princ_name,
    krb5_principal  *krb_princ,
    error_status_t  *stp
  )
#endif
{
    char             *unparsed;
    
    CLEAR_STATUS(stp);
    *krb_princ = NULL;

    /* can't do anything with a NULL principal name */
    if (princ_name == NULL || *princ_name == '\0') {
	return;
    }

    sec_krb_string_from_dce_pair(cell_name, princ_name, &unparsed, stp);
    
    if (GOOD_STATUS(stp)) {

	*stp = krb5_parse_name(unparsed, krb_princ);
	if (BAD_STATUS(stp)) {
	    *krb_princ = NULL; 
        }
		       
	free( (char *) unparsed);
    }
}

/*
 * Caller must free *cell_name and *princ_name on successful return.
 */
PUBLIC  void  sec_dce_pair_from_krb_princ
#ifndef __STDC__
    ( krb_princ, cell_name, princ_name, stp )  
    krb5_principal  krb_princ;
    char            **cell_name;
    char            **princ_name;
    error_status_t  *stp;
#else
  (
    krb5_principal  krb_princ,
    char            **cell_name,
    char            **princ_name,
    error_status_t  *stp
  )
#endif
{
    char  *unparsed, *rp;  
    int   i, j, len;

    CLEAR_STATUS(stp);
    
    *stp = krb5_unparse_name(krb_princ, &unparsed);
    if (BAD_STATUS(stp)) {
       return;
    }

    /* convert "\/" to '/' */
    len = u_strlen(unparsed);
    for (i = 0; i < len; i++)
    {
        if ((unparsed[i] == '\\') &&
            (unparsed[i+1] == '/'))
        {
            for (j = i; j < len; j++) {
                unparsed[j] = unparsed[j+1];
            }
            len--;
        }
    }
    unparsed[len] = '\0';

    rp = u_strchr(unparsed, '@');
    *rp++ = 0;

    if (cell_name != NULL) {
    
        *cell_name = sec_krb_realm_to_cell(rp);
        if (*cell_name == NULL) {
            SET_STATUS(stp, sec_rgy_cant_allocate_memory);
        }
    }
          
    if (BAD_STATUS(stp)) {
        free( (char *) unparsed);
        return;
    }

    if (princ_name != NULL) {
         *princ_name = malloc(u_strlen(unparsed) +1);
        if (*princ_name != NULL) {
            u_strcpy(*princ_name, unparsed);
	} else {
            SET_STATUS(stp, sec_rgy_cant_allocate_memory);
        }
    }

    free( (char *) unparsed);
}

PUBLIC void sec_krb_tgs_parsed_name_from_dce_pair (
    char            *client_cell_name,
    char            *server_cell_name,
    krb5_principal  *tgs_parsed_name,
    error_status_t  *stp
)
{
    krb5_data          *tgs[4];
    krb5_data          client_realm;
    krb5_data          server_realm;
    static krb5_data   tgsname = { sizeof(TGTNAME)-1, TGTNAME };
    char               *unparsed;

    CLEAR_STATUS(stp);
    *tgs_parsed_name = NULL;

    client_realm.data = dce_cell_to_krb_realm(client_cell_name, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    server_realm.data = dce_cell_to_krb_realm(server_cell_name, stp);
    if (BAD_STATUS(stp)) {
        free(client_realm.data);
        return;
    }

    client_realm.length = u_strlen(client_realm.data);  /* we know its null-terminated */
    server_realm.length = u_strlen(server_realm.data);

    tgs[1] = &tgsname;
    tgs[0] = &client_realm;
    tgs[2] = &server_realm;
    tgs[3] = NULL;

    /*
     * unparse the krb5_princ stack variable, and reparse the
     * results into the return parameter.  Inefficient, but simple.
     */
    *stp = krb5_unparse_name(tgs, &unparsed);
    free(client_realm.data);
    free(server_realm.data);
    if (BAD_STATUS(stp)) {
        return;
    }

    *stp = krb5_parse_name(unparsed, tgs_parsed_name);
    free( (char *) unparsed);
    if (BAD_STATUS(stp)) {
        *tgs_parsed_name = NULL;
    }
}

PUBLIC  void  sec_krb_tgs_parsed_name
#ifndef __STDC__
    ( cell_name, tgs_parsed_name, stp ) 
    char            *cell_name;
    krb5_principal  *tgs_parsed_name;
    error_status_t  *stp;
#else
  (
    char            *cell_name,
    krb5_principal  *tgs_parsed_name,
    error_status_t  *stp
  )
#endif
{
    krb5_data          *tgs[4];
    krb5_data          realm;
    static krb5_data   tgsname = { sizeof(TGTNAME)-1, TGTNAME };
    char               *unparsed;
    
    
    CLEAR_STATUS(stp);
    *tgs_parsed_name = NULL;
    
    if (cell_name && *cell_name) {
        realm.data =  dce_cell_to_krb_realm(cell_name, stp);  
    } else {
	*stp = krb5_get_default_realm(&realm.data);
    }
    if (BAD_STATUS(stp)) {
        return;
    }
    
    realm.length = u_strlen(realm.data);  /* we know its null-terminated */
    
    tgs[1] = &tgsname;
    tgs[0] = tgs[2] = &realm;
    tgs[3] = NULL;
    
    /*
     * unparse the krb5_princ stack variable, and reparse the 
     * results into the return parameter.  Inefficient, but simple.
     */
    *stp = krb5_unparse_name(tgs, &unparsed);
    free(realm.data);
    if (BAD_STATUS(stp)) {
	return;
    }
    
    *stp = krb5_parse_name(unparsed, tgs_parsed_name);
    free( (char *) unparsed);
    if (BAD_STATUS(stp)) {
	*tgs_parsed_name = NULL;
    }
}
       
/*
 * Caller must free returned pointer on successful call.
 */
PUBLIC  char *  sec_krb_realm_from_dce_cell
#ifndef __STDC__
    ( cell ) 
    char    *cell;
#else
  (
    char    *cell
  )
#endif
{
    error_status_t  status;

    return dce_cell_to_krb_realm(cell, &status);
}

/*
 * Caller must free returned pointer on successful call.
 */
PUBLIC char * sec_krb_realm_to_cell_len
#ifndef __STDC__
    ( len, realm ) 
    unsigned32  len;
    char        *realm;
#else
  (
    unsigned32  len,
    char        *realm
  )
#endif
{
    char    *retval;

    retval = (char *) malloc(len + GLOBAL_DIR_ROOT_LEN + 1);
    if (retval) {
        if (len == 0) {
            u_strcpy(retval, CELL_DIR_ROOT);
            /*
             * Drop trailing "/"
             */
            retval[CELL_DIR_ROOT_LEN-1] = '\0';
        } else {
            u_strcpy(retval, GLOBAL_DIR_ROOT);
            u_strncpy(&retval[GLOBAL_DIR_ROOT_LEN], realm, len);
            retval[GLOBAL_DIR_ROOT_LEN + len] = '\0';
        }
    }

    return retval;
}     

/*
 * Caller must free returned pointer on successful call.
 */
PUBLIC char * sec_krb_realm_to_cell
#ifndef __STDC__
    ( realm ) 
    char    *realm;
#else
  (
    char    *realm
  )
#endif
{
    unsigned32  len;

    len = u_strlen(realm);
    return sec_krb_realm_to_cell_len(len, realm);
}     

PUBLIC  boolean32  sec_krb_name_parsed_cmp_princ (
    krb5_principal  princ1,
    krb5_principal  princ2)
{
    char             *unparsed1, *unparsed2, *rp1 = NULL, *rp2 = NULL;
    krb5_error_code  kst;
    boolean32        result = false;

    kst = krb5_unparse_name(princ1, &unparsed1);
    if (kst != 0) {
	return result;
    }
    rp1 = u_strchr(unparsed1, '@');
    *rp1++ = 0;

    kst = krb5_unparse_name(princ2, &unparsed2);
    if (kst != 0) {
	free(unparsed1);
	return result;
    }
    rp2 = u_strchr(unparsed2, '@');
    *rp2++ = 0;

    result = u_strcmp(unparsed1, unparsed2) == 0;

    free(unparsed1);
    free(unparsed2);

    return result;
}


PUBLIC  krb5_keytype  sec_to_krb_keytype
#ifndef __STDC__
    ( sec_keytype, stp ) 
    sec_passwd_type_t sec_keytype;
    error_status_t    *stp;
#else
  (
    sec_passwd_type_t sec_keytype,
    error_status_t    *stp
  )
#endif
{
    krb5_keytype     krb_keytype = KEYTYPE_NULL;
    
    CLEAR_STATUS(stp);

    switch(sec_keytype) {

    case sec_passwd_des:
	krb_keytype = KEYTYPE_DES;
	break;

    case sec_passwd_pubkey:
        krb_keytype = KEYTYPE_ENCRYPT_RSA_PRIV;
        break;

    default:
	SET_STATUS(stp, sec_rgy_key_bad_type);
	break;
    }

    return krb_keytype;
}


PUBLIC  sec_passwd_type_t  krb_to_sec_keytype
#ifndef __STDC__
    ( krb_keytype, stp )  
    long int /* krb5_keytype */     krb_keytype;
    error_status_t                  *stp;
#else
  (
    long int /* krb5_keytype */     krb_keytype,
    error_status_t                  *stp
  )
#endif
{
    sec_passwd_type_t     sec_keytype = sec_passwd_none;
    
    CLEAR_STATUS(stp);

    switch(krb_keytype) {

    case KEYTYPE_DES:
	sec_keytype = sec_passwd_des;
	break;

    case KEYTYPE_ENCRYPT_RSA_PRIV:
        sec_keytype = sec_passwd_pubkey;
        break;

    default:
	SET_STATUS(stp, sec_rgy_key_bad_type);
	break;
    }

    return sec_keytype;
}

/*
 * Caller must free returned keyblock 
 */
PUBLIC  krb5_keyblock *  sec_to_krb_key
#ifndef __STDC__
    ( sec_key, stp ) 
    sec_passwd_rec_t  *sec_key;
    error_status_t    *stp;
#else
  (
    sec_passwd_rec_t  *sec_key,
    error_status_t    *stp
  )
#endif
{
    krb5_keyblock     *krb_key;
    
    krb_key = (krb5_keyblock *) malloc(sizeof(krb5_keyblock));
    if (krb_key == NULL) {
	SET_STATUS(stp, sec_rgy_cant_allocate_memory);
	return krb_key;
    }
    memset(krb_key, 0, sizeof(*krb_key));

    CLEAR_STATUS(stp);

    switch(PASSWD_TYPE(sec_key)) {

    case sec_passwd_des:
	krb_key->keytype = KEYTYPE_DES;
	krb_key->contents = malloc(sec_passwd_c_des_key_size);
	if (krb_key->contents == NULL) {
	    SET_STATUS(stp, sec_rgy_cant_allocate_memory);
	} else {
	    krb_key->length = sec_passwd_c_des_key_size;
	    memcpy(krb_key->contents, DES_PASSWD(sec_key), krb_key->length);
	}
	break;

    case sec_passwd_pubkey:
        krb_key->keytype = KEYTYPE_ENCRYPT_RSA_PRIV;
        krb_key->contents = malloc(PUBKEY_PASSWD(sec_key).len);
        if (krb_key->contents == NULL) {
            SET_STATUS(stp, sec_rgy_cant_allocate_memory);
        } else {
            krb_key->length = PUBKEY_PASSWD(sec_key).len;
            memcpy(krb_key->contents, PUBKEY_PASSWD(sec_key).data, krb_key->length);
        }
        break;

    default:
	SET_STATUS(stp, sec_rgy_key_bad_type);
    }

    if (BAD_STATUS(stp)) {
	krb5_free_keyblock(krb_key);
	krb_key = NULL;
    }
    return krb_key;
}


/* 
 * Caller must free returned sec_passwd_rec_t
 */
PUBLIC  sec_passwd_rec_t *  krb_to_sec_key
#ifndef __STDC__
    ( krb_key, stp ) 
    krb5_keyblock     *krb_key;
    error_status_t    *stp;
#else
  (
    krb5_keyblock     *krb_key,
    error_status_t    *stp
  )
#endif
{
    sec_passwd_rec_t  *sec_key;
    
    sec_key = (sec_passwd_rec_t *) malloc(sizeof(sec_passwd_rec_t));
    if (sec_key == NULL) {
	SET_STATUS(stp, sec_rgy_cant_allocate_memory);
	return sec_key;
    }
    memset(sec_key, 0, sizeof(*sec_key));

    CLEAR_STATUS(stp);

    PASSWD_TYPE(sec_key) = krb_to_sec_keytype(krb_key->keytype, stp);
    
    if (GOOD_STATUS(stp)) {

	switch(PASSWD_TYPE(sec_key)) {
	    
	case sec_passwd_des:
	    memcpy(DES_PASSWD(sec_key), krb_key->contents, sec_passwd_c_des_key_size);
	    break;

        case sec_passwd_pubkey:
            PUBKEY_PASSWD(sec_key).len = krb_key->length;
            memcpy(PUBKEY_PASSWD(sec_key).data, krb_key->contents, krb_key->length);
            break;
	    
	default:
	    SET_STATUS(stp, sec_rgy_key_bad_type);
	}
    }

    if (BAD_STATUS(stp)) {
	sec_crypt_free_key(&sec_key);
    }

    return sec_key;
}
    

PUBLIC  krb5_enctype  sec_to_krb_etype
#ifndef __STDC__
    ( sec_etype, stp ) 
    sec_etype_t     sec_etype;
    error_status_t  *stp;
#else
  (
    sec_etype_t     sec_etype,
    error_status_t  *stp
  )
#endif
{
    krb5_enctype  krb_etype = ETYPE_NULL;

    CLEAR_STATUS(stp);

    switch (sec_etype) {
    case sec_etype_des_cbc_crc:
	krb_etype = ETYPE_DES_CBC_CRC;
	break;
    default:
	SET_STATUS(stp, sec_rgy_crypt_bad_type);
	break;
    }
    
    if (GOOD_STATUS(stp)) {
	if (!valid_etype(krb_etype)) {
	    SET_STATUS(stp, sec_rgy_crypt_bad_type);
	}
    }

    return krb_etype;
}

#ifndef CKSUMTYPE_RSA_MD4_DES
#   define CKSUMTYPE_RSA_MD4_DES 0x0003
#endif

PUBLIC  krb5_enctype  sec_to_krb_chksum_type
#ifndef __STDC__
    ( sec_chksum_type, stp ) 
    sec_chksum_type_t  sec_chksum_type;
    error_status_t     *stp;
#else
  (
    sec_chksum_type_t  sec_chksum_type,
    error_status_t     *stp
  )
#endif
{
    krb5_cksumtype  krb_sum_type = 0;

    CLEAR_STATUS(stp);

    switch (sec_chksum_type) {
    case sec_chksum_crc32:
	krb_sum_type = 	CKSUMTYPE_CRC32;
	break;
    case sec_chksum_des_cbc:
	krb_sum_type = 	CKSUMTYPE_DESCBC;
	break;
    case sec_chksum_rsa_md4:
	krb_sum_type = CKSUMTYPE_RSA_MD4;
	break;
    case sec_chksum_rsa_md4_des:
	krb_sum_type = CKSUMTYPE_RSA_MD4_DES;
	break;

    default:
	SET_STATUS(stp, sec_rgy_bad_chksum_type);
	break;
    }

    if (GOOD_STATUS(stp)) {
	if (!valid_cksumtype(krb_sum_type)) {
	    SET_STATUS(stp, sec_rgy_bad_chksum_type);
	}
    }
    
    return krb_sum_type;
}

