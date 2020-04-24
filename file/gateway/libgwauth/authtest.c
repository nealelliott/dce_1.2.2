/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1995 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * (c) Copyright 1995 HEWLETT-PACKARD COMPANY
 * 
 * To anyone who acknowledges that this file is provided 
 * "AS IS" without any express or implied warranty:
 * permission to use, copy, modify, and distribute this 
 * file for any purpose is hereby granted without fee, 
 * provided that the above copyright notice and this 
 * notice appears in all copies, and that the name of 
 * Hewlett-Packard Company not be used in advertising or 
 * publicity pertaining to distribution of the software 
 * without specific, written prior permission.  Hewlett-
 * Packard Company makes no representations about the 
 * suitability of this software for any purpose.
 *
 */
/*
 * Copyright (c) 1995 Transarc Corp.  All rights reserved.
 */
/*
 * Test auth_at API's in user space for easy debugging
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/user.h>
#include <netinet/in.h>
#include <time.h>
#include <dcedfs/icl.h>

#define _KERNEL
#include <dcedfs/auth_at.h>
#undef _KERNEL

#include "authtest.h"

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/gateway/libgwauth/RCS/authtest.c,v 1.2 1996/04/23 20:23:51 rajesh Exp $")

int suser_uidok = 1;
struct icl_set *dfsgw_iclSetp;
osi_cred_t *u_cred;
osi_cred_t cred;
struct user u, *uptr = &u;
char *cm_sysname = "z80_cpm";

#if __hpux1010p
#ifdef kt_cred
#undef kt_cred
#endif
struct kthread;
typedef struct kthread kthread_t;
osi_cred_t *kt_cred(kthread_t *t)
{
       return(osi_getucred());
}
#endif

int suser()
{
    return(suser_uidok);
}

/*
 * Test the API's
 */
int
dfsgw_size()
{
    int num_entries;

    printf("at_size()\n");
    num_entries = at_size();
    printf("num_entries=%d\n", num_entries);

    return(num_entries);
}

int
dfsgw_add(uid_t uid, long pag, u_long addr, char *sysname, char *hostname, 
	  time_t life)
{
    at_pag_mapping_t me;
    struct sockaddr_in *ip;
    int err;
    
    memset(&me, 0, sizeof(me));

    printf("at_add(NEW)\n");
    me.uid = uid;
    me.pag = pag;
    me.expiration_time = osi_Time() + life;
    ip = (struct sockaddr_in *)&me.addr;
    ip->sin_family = AF_INET;
    ip->sin_port = 0;
    ip->sin_addr.s_addr = addr;
    strcpy(me.sysname, sysname);
    strcpy(me.hostname, hostname);

    if ((err = at_add(&me, AT_ADD_NEW_ENTRY)) != 0) {
	printf("at_add: error (%d) : %s\n", err, strerror(err));
    }
    else
	printf("added - uid=%d pag=%x addr=%s sysname='%s' hostname='%s'\n",
	       me.uid, me.pag, inet_ntoa(ip->sin_addr), me.sysname, me.hostname);
}

int
dfsgw_list()
{
    at_pag_mapping_t *ml;
    struct sockaddr_in *ip;
    int i, n, n2, err;

    n = n2 = at_size();

    ml = osi_Alloc(sizeof(at_pag_mapping_t) * n);
    if (ml == NULL)  {
	printf("at_list: error unable to alloc memory\n");
	return;
    }
    if ((err = at_list(&n, ml)) != 0) {
	printf("at_list: error (%d) : %s\n", err, strerror(err));
	return;
    }
    if (n != n2) 
	printf("at_list: asked for %d entries (as indicated by at_size) but got %d entries only\n", n2, n);

    printf("at_list(%d)\n", n);
    for (i=0; i < n; i++) {
	ip = (struct sockaddr_in *)&ml[i].addr;
	printf("%2.2d: uid=%5.5d pag=0x%8.8x addr=%15.15s sysname='%s' hostname='%s'\n", i,
	       ml[i].uid, ml[i].pag,
	       inet_ntoa(ip->sin_addr), ml[i].sysname, ml[i].hostname);
    }
}

int
dfsgw_query(uid_t uid, u_long addr)
{
    at_pag_mapping_t me;
    struct sockaddr_in *ip;
    int err, code;
    struct in_addr inaddr;
    memset(&me, 0, sizeof(me));
    
    inaddr.s_addr = addr;
    printf("at_query(uid=%d addr=%s)\n", uid, inet_ntoa(inaddr));
    me.uid = uid;
    ip = (struct sockaddr_in *)&me.addr;
    ip->sin_family = AF_INET;
    ip->sin_port = 0;
    ip->sin_addr.s_addr = addr;

    err = at_query(&me);
    switch(err) {
    case 0:
	printf("at_query: found\n");
	code = 1;
	break;

    case -1:
	printf("at_query: failed\n");
	code = 0;
	break;
	
    default:
	printf("at_query: error (%d) : %s\n", err, strerror(err));
	break;
    }
    return(code);
}

int
dfsgw_atname(int type, long pag)
{
    char *np, tname[16], buf[80];

    switch(type) {
    case ATSYS:
	strcpy(tname, "ATSYS");
	break;
    case ATHOST:
	strcpy(tname, "ATHOST");
	break;
    default:
	sprintf(tname, "type=%d", type);
	break;
    }
    
    if (at_atname(type, pag, buf, sizeof(buf)) == 0)
	np = "(null)";
    else
	np = buf;
    
    printf("atname(%#x, %s)='%s'\n", pag, tname, np);

    return(0);
}

int
dfsgw_del(uid_t uid, u_long addr)
{
    at_pag_mapping_t me;
    struct sockaddr_in *ip;
    int err;

    memset(&me, 0, sizeof(me));
    
    printf("at_del()\n");
    me.uid = uid;
    ip = (struct sockaddr_in *)&me.addr;
    ip->sin_family = AF_INET;
    ip->sin_port = 0;
    ip->sin_addr.s_addr = addr;

    if ((err = at_delete(&me)) != 0) {
	printf("at_delete: error (%d) : %s\n", err, strerror(err));
    }
    else
	printf("deleted - uid=%d addr=%s\n",
	       me.uid, inet_ntoa(ip->sin_addr));
}

main(int argc, char *argv[])
{
  int n;
  struct icl_log *logp;
  int code;  

  /*
   * Create ICL log for tracing dfsgw ops
   */
  code = icl_CreateLog("dfsgw", 60*1024, &logp);
  if (code == 0) {
    code = icl_CreateSet("dfsgw", logp, (struct icl_log *)0, &dfsgw_iclSetp);
    if (code) {
      printf("Failed to create ICL set for dfsgw\n");
      dfsgw_iclSetp = NULL;
    }
  }

  osi_setucred(&cred);
   
  printf("should be 0 entries\n");
  dfsgw_size();
    
  dfsgw_add(0, TEST_PAG, 0x12345678, "", "", 5);
  dfsgw_add(0, TEST_PAG, 0x12345678, "", "", 5);

  printf("should be 1 entry\n");
  dfsgw_size();

  dfsgw_add(0, TEST_PAG1, 0x12345678, "", "", 15);

  printf("should be 1 entry\n");
  dfsgw_size();

  dfsgw_add(0, TEST_PAG, 0x12345678, "", "", 15);
  printf("should be 1 entry\n");
  dfsgw_size();

  osi_SetUID(u_cred, 34500);
  dfsgw_add(34500, TEST_PAG1, 0x56781234, "m68k_mac70", "amac", 5);

  osi_SetUID(u_cred, 660);
  dfsgw_add(660, TEST_PAG2, 0x15837490, "i386_msdos", "apc", 5);

  printf("should be 3 entries\n");
  dfsgw_list();
  
  printf("should be found\n");
  dfsgw_query(0, 0x12345678);
  
  printf("calling at_gc()...");
  at_gc();
  printf("done.\n");
  
  printf("should be 3 entries\n");
  dfsgw_list();
  
  dfsgw_atname(ATSYS, TEST_PAG);
  dfsgw_atname(ATHOST, TEST_PAG);
  
  dfsgw_atname(ATSYS, TEST_PAG1);
  dfsgw_atname(ATHOST, TEST_PAG1);
  
  dfsgw_atname(ATSYS, TEST_PAG2);
  dfsgw_atname(ATHOST, TEST_PAG2);
  
  dfsgw_del(660, 0x15837490);
  printf("should be 2 entries\n");
  dfsgw_list();
  
  printf("Sleep 10 seconds\n");
  sleep(10);
  
  printf("calling at_gc()...");
  at_gc();
  printf("done.\n");
  
  printf("should be 1 entry\n");
  dfsgw_list();
  
  printf("Sleep 10 seconds\n");
  sleep(10);
  
  printf("calling at_gc()...");
  at_gc();
  printf("done.\n");
  
  printf("should be 0 entries\n");
  dfsgw_list();
  
  printf("should return (null)\n");
  dfsgw_atname(ATSYS, TEST_PAG);
  dfsgw_atname(ATHOST, TEST_PAG);
  
  dfsgw_atname(ATSYS, TEST_PAG1);
  dfsgw_atname(ATHOST, TEST_PAG1);
  
  dfsgw_atname(ATSYS, TEST_PAG2);
  dfsgw_atname(ATHOST, TEST_PAG2);
  
  osi_SetUID(u_cred, 660);
  dfsgw_add(660, TEST_PAG2, 0x15837490, "i386_msdos", "apc", 5);
  
  printf("should be 1 entry\n");
  dfsgw_list();
  
  dfsgw_del(660, 0x15837490);
  
  printf("should be 0 entries\n");
  dfsgw_list();
  
  printf("Add 512 entries\n");
  for (n = 0; n < 512; n++) {
			      osi_SetUID(u_cred, 100+n);
			      dfsgw_add(100+n, TEST_PAG+n, 1000+n, "m68k_amiga", "test", 5);
			    }
  printf("should be 512 entries\n");
  dfsgw_list();
  
  dfsgw_del(100+255, 1000+255);
  printf("should be 511 entries\n");
  dfsgw_size();
  
  printf("Sleep 10 seconds\n");
  sleep(10);
  
  printf("calling at_gc()...");
  at_gc();
  printf("done.\n");
  
  printf("should be 0 entries\n");
  dfsgw_list();
  
  printf("should fail\n");
  dfsgw_del(660, 0x15837490);
  
  /* Done */
  exit(0);
}
