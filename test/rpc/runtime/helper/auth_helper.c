/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: auth_helper.c,v $
 * Revision 1.1.4.2  1996/02/17  23:23:11  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:46  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:13:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:44  root]
 * 
 * Revision 1.1.2.2  1993/01/13  19:52:12  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:19:40  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:43:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  auth_helper.c V=6 11/18/91 //littl/prgy/src/helper
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 * Authentication helper process - dispatching procedures.
 */

#include <stdio.h>
#include <stdlib.h>
#include <krb5/krb5.h>
#include <dce/sec_authn.h>
#include <dce/sec_login.h>
#include <dce/sec_pkl.h>
#include <auth_helper.h>

/* XXX put this in some header file exported from sec_login */
#define SEC_LOGIN_CCACHE_FROM_PAG_FMT "FILE:/tmp/dcecred_%08x"

extern int helper_debug;

#define dprintf if(helper_debug)printf

#define PUT_MESSAGE(ptr, msg) \
     PUT_LONG(ptr, (msg)->length); \
     memcpy(ptr, (msg)->data, (msg)->length); \
     ptr += (msg)->length

#define GET_MESSAGE(ptr, msg) \
   { long temp; \
     GET_LONG(ptr, temp); \
     (msg)->length = temp; \
     (msg)->data = (unsigned char *)ptr; \
     ptr += temp; \
   }

/*
 * The local rep of a context is a pair of integers (pag, euid)
 * 
 * The local rep of a cred is a structure containing a copy of the parameters 
 * used to pick it out.
 */

typedef struct {
    unsigned32 pag;
    unsigned32 euid;
    unsigned   char *server_name;
    unsigned32 authn_level;
    unsigned32 authz_proto;
    unsigned32 expiration;
} sec_krb_cred_rep;

error_status_t cred_from_cred_rep (sec_krb_cred_rep *credrep, sec_krb_cred *cred);

#define GET_CRED(ptr, cred) \
  { sec_krb_cred_rep temp; \
    temp.server_name = 0; \
    GET_LONG(ptr, temp.pag); \
    GET_LONG(ptr, temp.euid); \
    GET_LONG(ptr, temp.authn_level); \
    GET_LONG(ptr, temp.authz_proto); \
    GET_LONG(ptr, temp.expiration); \
    GET_STRING(ptr, temp.server_name); \
    if (temp.authn_level == 0) \
        cred = NULL; \
    else { \
    status = cred_from_cred_rep(&temp, &cred); \
    if (status) goto out; \
    } \
}

typedef struct 
{
    unsigned32 pag;
    unsigned32 euid;
    char refcount;
    char valid;
    char cred_valid;
    sec_login_handle_t context;
    sec_krb_cred_rep credrep;
    sec_krb_cred cred;
} context_cache_t, *context_cache_p_t;

/* XXX should be scaled to "number of active users." */
#define CCSIZE 32

context_cache_t context_cache[CCSIZE];

static void release_context(login_context) 
    sec_login_handle_t *login_context;
{
    int i;
    sec_login_handle_t ctx = *login_context;
    *login_context = NULL;
    if (ctx == NULL)
        return;
    
    for (i=0; i<CCSIZE; i++) {
        if (context_cache[i].context == ctx) {
            if (context_cache[i].refcount < 1) {
                dprintf("refcount negative in context_cache!\n");
                abort();
            }
            context_cache[i].refcount--;
            dprintf("%d: refcount down to %d\n", i, context_cache[i].refcount);
            /* actually gets freed when we reuse cache slot */
            return;
        }
    }
    dprintf("releasing uncached context!!!\n");
    abort();
}    

static int  get_context_from_pag (unsigned32 pag, unsigned32 euid,
                                   sec_login_handle_t *login_context,
                                   error_status_t *st)
{
    unsigned char import_buf[1024];
    int i;
    int slot=-1;

    /* LOCK_CACHE(); */
    
    for (i=0; i<CCSIZE; i++) {
        if (context_cache[i].valid) {
            if (context_cache[i].pag == pag) {
                if (context_cache[i].refcount < 0) {
                    dprintf("bogus refcount in context_cache!\n");
                    abort();
                }
                if (context_cache[i].euid != euid) {
                    *st = rpc_s_helper_wrong_user;
                }
                context_cache[i].refcount++;
                dprintf("%d: pag %d refcount up to %d\n", i, pag, context_cache[i].refcount);
                
                *login_context = context_cache[i].context;
                *st = error_status_ok;
                /* UNLOCK_CACHE() */
                return i;
            }
        } else {
            /* not valid. remember this for future reference */
            if (slot < 0)
                slot = i;
        }
    }
    if (slot < 0) {
        for (i=0; i<CCSIZE; i++) {
            if (context_cache[i].refcount == 0) {
                error_status_t xst;
                dprintf("%d: recycled\n", slot);
                sec_login_release_context (&context_cache[i].context, &xst);
                sec_krb_cred_free(&context_cache[i].cred);
                free(context_cache[i].credrep.server_name);
                context_cache[i].valid = 0;
                context_cache[i].cred_valid = 0;
                slot = i;
                break;
            }
        }
    }
    if (slot < 0) {
        *st = rpc_s_helper_overflow;
        /* UNLOCK_CACHE() */
        return -1;
    }
    sprintf ((char *)import_buf, SEC_LOGIN_CCACHE_FROM_PAG_FMT, pag);
    dprintf ("Importing pag %d, `%s'\n", pag, import_buf);
    sec_login_import_context (strlen(import_buf)+1, import_buf, 
                              login_context, st);
    if (*st == error_status_ok) {
        context_cache[slot].context = *login_context;
        context_cache[slot].pag = pag;
        context_cache[slot].euid = euid;
        context_cache[slot].valid = 1;
        context_cache[slot].refcount = 1;
        dprintf("%d: refcount set to %d\n", slot,
            context_cache[slot].refcount);
    } else {
        dprintf("unable to import pag %x in `%s': status %x\n",
            pag, import_buf, *st);
    }
    /* UNLOCK_CACHE() */
    return slot;
}

#define GET_IDH(ptr, idh) \
  { unsigned32 pag, euid; \
    GET_LONG(ptr, pag); \
    GET_LONG(ptr, euid); \
    get_context_from_pag(pag, euid, &idh, &status); \
    if (status != 0) goto out; \
  }

#define GET_PAC(ptr, pac) \
{ ptr = get_pac (ptr, pac); } 

#define PUT_PAC(ptr, pac) \
{ ptr = put_pac (ptr, pac); }

#define PUT_STRING(ptr, s) \
{ long templen; \
     if (s != NULL) { templen = strlen(s)+1; \
        PUT_LONG(ptr, templen); \
        memcpy(ptr, s, templen); \
        ptr += templen; \
    } else { \
        PUT_LONG(ptr, 1); \
        *ptr++ = 0; \
    } \
}          

#define GET_STRING(ptr, s) \
{ long templen; \
    GET_LONG(ptr, templen); \
    s = (unsigned char *)ptr;  \
    s[templen-1] = 0; \
    ptr += templen; }

#define GET_BLOCK(ptr, b) \
  memcpy(&b, ptr, sizeof(b)); \
  ptr += sizeof(b);

#define PUT_BLOCK(ptr, b) \
  memcpy(ptr, &b, sizeof(b)); \
  ptr += sizeof(b);

#define PUT_NAME(ptr, n) \
 { \
   error_status_t xst; \
   unsigned char *tempname = 0; \
   if (n != 0) { \
       status = sec_krb_unparse_name(n, &tempname); \
       PUT_STRING(ptr, tempname); \
       rpc_string_free(&tempname, &xst); \
   } else { \
       PUT_STRING(ptr, 0); \
   } \
 }   

#define ZERO_MESSAGE(message) \
  { message.data = 0; \
    message.length = 0; \
  }

#define ZERO_PAC(pac) \
{ memset(&pac, 0, sizeof(pac)); \
  pac.groups = 0; \
  pac.foreign_groups = 0; \
}

static char *put_pac (char *ptr, sec_id_pac_t *pac)
{
    sec_id_pickled_pac_t *pickled_pac;
    int length;
    error_status_t status;
    idl_pkl_t *pickle;
    unsigned32 len;

    sec_id_pac_t_pickle(pac, 0, malloc, 8, &pickle, &len, &status);
    if (status != 0) {
        /* XXX should fail cleanly here */
        abort();
        return 0;
    }
    PUT_LONG(ptr, len);
    memcpy (ptr, pickle, len);
    free(pickle);
    return ptr+len;
}

static error_status_t cred_from_cred_rep (sec_krb_cred_rep *credrep, sec_krb_cred *cred)
{
    error_status_t status;
    sec_krb_ccache cc;
    sec_krb_parsed_name server = 0;
    unsigned32 expiration;
    int slot;
    sec_login_handle_t context;
    sec_krb_cred_rep *ccredrep;
    
    slot = get_context_from_pag (credrep->pag, credrep->euid,
        &context, &status);
    if (status)
        return status;

    ccredrep = &context_cache[slot].credrep;
    if (context_cache[slot].cred_valid) {
        if ((credrep->pag == ccredrep->pag) &&
            (credrep->euid == ccredrep->euid) &&
            (credrep->authz_proto == ccredrep->authz_proto) &&
            ((credrep->expiration == 0) ||
             (credrep->expiration == ccredrep->expiration)) &&
            ((credrep->authn_level == rpc_c_authn_level_none) ==
                (ccredrep->authn_level == rpc_c_authn_level_none)) &&
            (strcmp (credrep->server_name, ccredrep->server_name) == 0))
        {
            *cred = context_cache[slot].cred; 
            dprintf("cred cache hit - cred %x\n");
            credrep->expiration = ccredrep->expiration;
            release_context(&context);
            return 0;
        }
    }       
    dprintf("cred cache miss\n");

    cc = context_cache[slot].context;
    
    dprintf("authn_level %d:%d\n", credrep->authn_level, ccredrep->authn_level);
    if (credrep->authn_level == rpc_c_authn_level_none) {
        dprintf("not calling sec_krb_get_cred\n");
        status = error_status_ok;
        *cred = NULL;
    } else {
        status = sec_krb_sec_parse_name(
            sec_login_default_handle, /* XXX */
            rpc_c_authn_level_pkt_integrity, /* XXX */
            credrep->server_name, &server);
        if (status == 0)
        {
            dprintf("calling sec_krb_get_cred\n");
            status = sec_krb_get_cred (cc, server, credrep->authn_level,
                credrep->authz_proto, cred,
                &credrep->expiration);
        }
    }

    if (status == error_status_ok)
    {
        dprintf("cred cache fill slot %d - cred %x\n", slot, *cred);
        ccredrep->pag = credrep->pag;
        ccredrep->euid = credrep->euid;
        ccredrep->authn_level = credrep->authn_level;
        ccredrep->authz_proto = credrep->authz_proto;
        ccredrep->expiration = credrep->expiration;
        ccredrep->server_name = (unsigned char *)
            rpc__stralloc (credrep->server_name);
        context_cache[slot].cred = *cred;
        context_cache[slot].cred_valid = 1;
        dprintf("cred cache fill done\n");
    }
    if (server) sec_krb_parsed_name_free (&server); 

    release_context(&context);
    return status;
}

typedef int (*sec_request_handler_t)
    PROTOTYPE((
        char *, int, char *
    ));

static int handle_error (inptr, inlen, outbuf)
    char *inptr;
    int inlen;
    char *outbuf;
{
    char *outptr = outbuf;
    PUT_LONG(outptr, 0x66660003);    
    return outptr-outbuf;
}

static int handle_sec_krb_get_cred (inptr, inlen, outbuf)
    char *inptr;
    int inlen;
    char *outbuf;
{
    char *outptr = outbuf;
    error_status_t status;
    sec_krb_cred cred = 0;
    sec_krb_cred_rep cred_rep;
    
    /* expanded inline get_cred */
    GET_LONG(inptr, cred_rep.pag);
    GET_LONG(inptr, cred_rep.euid);
    GET_STRING(inptr, cred_rep.server_name);
    GET_LONG(inptr, cred_rep.authn_level);
    GET_LONG(inptr, cred_rep.authz_proto);
    cred_rep.expiration = 0;
    status = cred_from_cred_rep (&cred_rep, &cred);

out:
    PUT_LONG(outptr, status);
    if (!status) {

        PUT_LONG(outptr, cred_rep.pag);
        PUT_LONG(outptr, cred_rep.euid);
        PUT_LONG(outptr, cred_rep.authn_level);
        PUT_LONG(outptr, cred_rep.authz_proto);
        PUT_LONG(outptr, cred_rep.expiration);
        PUT_STRING(outptr, cred_rep.server_name);
        
        PUT_LONG(outptr, cred_rep.expiration);
    }
    return outptr-outbuf;
}

static int handle_sec_krb_dg_build_message(inptr, inlen, outbuf)
    char *inptr;
    int inlen;
    char *outbuf;
{
    char *outptr = outbuf;
    sec_krb_ccache context;
    sec_krb_cred cred;
    sec_des_block challenge, ivec;
    sec_des_key key;
    unsigned long authn_level, authz_proto, ksno;
    error_status_t status;
    sec_krb_message message;

    ZERO_MESSAGE(message);
    GET_IDH(inptr, context);
    GET_CRED(inptr, cred);
    GET_BLOCK(inptr, challenge);
    GET_LONG(inptr, authn_level);
    GET_LONG(inptr, authz_proto);
    GET_LONG(inptr, ksno);
    GET_BLOCK(inptr, key);
    GET_BLOCK(inptr, ivec);

    status = sec_krb_dg_build_message (context, cred, &challenge,
        authn_level, authz_proto,
        ksno, &key, &ivec,
        &message);

    dprintf("(handle_sec_krb_dg_build_message) returning status %x\n", status);
  
 out:
    PUT_LONG (outptr, status);
    if (status == 0) {
        PUT_MESSAGE(outptr, &message);
    }
    release_context(&context);
    sec_krb_message_free(&message);
    return outptr-outbuf;
}

static int handle_sec_krb_dg_decode_message (inptr, inlen, outbuf)
    char *inptr;
    int inlen;
    char *outbuf;
{
    error_status_t xst;
    char *outptr = outbuf;
    sec_krb_message message;
    sec_des_block challenge, ivec;
    unsigned char *client_name;
    sec_krb_parsed_name server_name;
    sec_id_pac_t client_pac;
    unsigned32 authn_level, authz_proto, key_seq, expiration;
    sec_des_key key;
    error_status_t status;
    
    ZERO_PAC(client_pac);

    client_name = 0;

    server_name = 0;
  
    GET_MESSAGE(inptr, &message);
    GET_BLOCK(inptr, challenge);

    status = sec_krb_dg_decode_message (&message, &challenge, 
        &client_name, &client_pac, &server_name,
        &authn_level, &authz_proto, &key_seq,
        &key, &ivec, &expiration);

    PUT_LONG (outptr, status);
    if (status == 0) {
        PUT_STRING(outptr, client_name);
        PUT_PAC(outptr, &client_pac);
#ifdef DEBUG
        if (debug) 
          {
            dprintf("pac from ticket: "); sec_id_pac_print (&client_pac);
          }
#endif
        PUT_NAME(outptr, server_name);
        PUT_LONG(outptr, authn_level);
        PUT_LONG(outptr, authz_proto);
        PUT_LONG(outptr, key_seq);
        PUT_BLOCK(outptr, key);
        PUT_BLOCK(outptr, ivec);
        PUT_LONG(outptr, expiration);
    }
    if (client_name)
        rpc_string_free(&client_name, &xst);
    if (server_name)
        sec_krb_parsed_name_free(&server_name);
    sec_id_pac_util_free(&client_pac);
    return outptr-outbuf;
}

static int handle_sec_krb_register_server (inptr, inlen, outbuf)
    char *inptr;
    int inlen;
    char *outbuf;
{
    char *outptr = outbuf;
    error_status_t status;
    unsigned_char_p_t name, canon_name;

    GET_STRING(inptr, name);
    status = sec_krb_register_server (name, 0, 0);
    (void) sec_krb_get_server(&canon_name);
    dprintf("register_server returns %d\n", status);
    PUT_LONG (outptr, status);
    PUT_STRING(outptr, canon_name);
    return outptr-outbuf;
}

static int handle_sec_krb_gen_key (inptr, inlen, outbuf)
     char *inptr;
     int inlen;
     char *outbuf;
{
    char *outptr = outbuf;
    error_status_t status;
    sec_des_key key;

    status = sec_des_new_random_key (&key);
  
    PUT_LONG(outptr, status);
    if (status == 0) {
        PUT_BLOCK(outptr, key);
    }
    return outptr-outbuf;
}

static int handle_sec_krb_gen_block (inptr, inlen, outbuf)
     char *inptr;
     int inlen;
     char *outbuf;
{
    char *outptr = outbuf;
    error_status_t status;
    sec_des_block block;

    status = sec_des_generate_random_block (&block);
    PUT_LONG(outptr, status);
    if (status == 0) {
        PUT_BLOCK(outptr, block);
    }
    return outptr-outbuf;
}

static sec_request_handler_t handlers[] =
{
    handle_error,               /* 0 */
    handle_error,               /* 1 */
    handle_error,               /* 2 */
    handle_sec_krb_get_cred,    /* 3 */
    handle_error,               /* 4 */
    handle_error,               /* 5 */
    handle_error,               /* 6 */
    handle_sec_krb_register_server, /* 7 */
    handle_sec_krb_gen_key,     /* 8 */
    handle_sec_krb_gen_block,   /* 9 */
    handle_sec_krb_dg_build_message, /* 10 */
    handle_sec_krb_dg_decode_message, /* 11 */
};

static int max_handler = sizeof(handlers)/sizeof(handlers[0]);

void auth_helper_init()
{
    sec_passwd_type_t keytype = sec_passwd_none;
    void *cursor;
    
    error_status_t st;
    
    rpc__init();    
    /*
     * XXX scan through default keytab, registering each unique principal
     * name in keytab, 'just in case'.
     */
    sec_key_mgmt_initialize_cursor (
        rpc_c_authn_dce_private,
        NULL,
        NULL,
        (void *)&keytype,
        &cursor,
        &st);
    
    if (st != error_status_ok) {
        dprintf("Nobody home...\n");
        /*
         * nobody home..
         */
        return;
    }

    dprintf("Scanning keytab\n");
    do {
        unsigned_char_p_t principal;
        unsigned32 key_vno;
        
        sec_key_mgmt_get_next_key (cursor, &principal, &key_vno, NULL, &st);
        if (st == error_status_ok) {
            dprintf("registering %s\n", principal);
            st = sec_krb_register_server (principal, NULL, NULL);
            if (st != error_status_ok) {
                dprintf("register of %s failed: %x\n", principal, st);
            }
            free(principal);
        }
    } while (st == error_status_ok);
    dprintf("done: status = %x\n", st);
    sec_key_mgmt_release_cursor(&cursor, &st);
    dprintf("released cursor\n");
}       

do_auth_request(inptr, inlen, outbuf, outsize)
    char *inptr;
    long inlen;
    char *outbuf;
    long *outsize;
{
    unsigned long opcode;

    GET_LONG(inptr, opcode);
    dprintf("kernel requests opcode %d\n", opcode);
    if (opcode >= max_handler)
        *outsize = handle_error(inptr, inlen, outbuf);
    else
        *outsize = (handlers[opcode])(inptr, inlen, outbuf);
    dprintf("response to opcode %d is 0x%x bytes long (status is %x)\n", opcode, *outsize,
        *(long *)outbuf);
    return 0;
}
