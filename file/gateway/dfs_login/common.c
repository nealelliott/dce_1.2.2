/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */

/*
 * common.c - Common functions between dfs_login and dfs_logout.
 */

/* 
 * Cannot include dcedfs/osi.h and dcedfs/osi_net.h in place of the
 * following since this file is not built in a DCE/DFS sandbox 
 * but instead is copied over into the kerberos source tree under the
 * clients subdirectory to be built.
 */
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/socket.h>
#if HAVE_UNAME
#include <sys/utsname.h>
#endif
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include <com_err.h>
#include <pwd.h>
#ifdef __hpux
#include <mntent.h>
#elif OSF1
#include <sys/mount.h>
#endif

#include "common.h"
#include "dfsgw.h"

/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/gateway/dfs_login/RCS/common.c,v 1.7 1996/04/23 20:23:02 rajesh Exp $ */

#if defined(i386) || defined(vax) || defined(alpha)
#define host_byte_order         DFSGW_LITTLE_ENDIAN
#else
#define host_byte_order         DFSGW_BIG_ENDIAN
#endif

/*
 * Global data.
 */
extern char *progname;			/* Program name */

/*
 * fatal - Shut down daemon.
 *
 * This routine writes a final message to stderr before
 * terminating the program.
 *
 * Inputs:
 *	msg 		: Optional message.
 * Return value:
 *	DOES NOT RETURN.
 */
fatal
#ifdef __STDC__
(char *msg)
#else
(msg)
char *msg;
#endif /* __STDC__ */
{
  if (msg) {
    com_err(progname, 0, "%s", msg);
  }
  exit(1);
}

/*
 * default_hostname - Get the default hostname.
 *
 * The "host" is defined to be the place where the dfsgwd server
 * is running.  This had also better be the place where the
 * filesystem gateway lives, since otherwise installing the
 * PAG on a system that doesn't get any requests won't do
 * much good.
 *
 * To make things easy, this routine tries to see if the
 * DFS root "/..." is already mounted locally, and if so,
 * it picks up the host from where it was exported.  If it
 * can't determine it from this, it gives up.  I suppose a
 * possible enhancement would be to install the default host
 * in a config file somewhere, but that may just compilcate
 * things if/when it gets stale.
 *
 * This routine is VERY file-system-exporter dependent.  Right
 * now it only works for NFS exporters.  When you add a new file
 * system exporter, this is one place you'll have to make smarter.
 *
 * This routine returns the hostname in static storage - repeated
 * calls will overwrite it.
 *
 * Return value:
 *	Pointer to hostname or NULL on error.
 */
char *default_hostname()
{
#ifdef __hpux
  FILE *mntstream;				/* File pointer into mnttab */
  struct mntent *m;				/* Pointer to mount entry */
  char *hostname;				/* Resulting hostname */
  static char hostname_store[MAXHOSTNAMELEN];	/* For safe keeping */
  char *p;

  if ((mntstream = setmntent("/etc/mnttab", "r")) == NULL) {
    return(NULL);
  }
  hostname = NULL;
  /*
   * We'll scan the entire mount table, if there are multiple
   * possibilities, we'll take the first one and print warnings
   * about the other ones.
   */
  do {
    if ((m = getmntent(mntstream)) && (!strcmp(m->mnt_type, MNTTYPE_NFS))) {
      if ((p = strstr(m->mnt_fsname, ":/...")) &&
	   (!strcmp(m->mnt_dir, "/..."))) {
	/*
	 * We assume this to be a DFS mount point, and that
	 * the node is identified by the string up to "p".
	 */
	*p = '\0';
	if ((hostname) && (strcmp(hostname, m->mnt_fsname))) {
	  com_err(progname, 0, 
		  "warning - multiple DFS mount points (hosts) found - using %s, ignoring %s", 
		  hostname, m->mnt_fsname);
	  com_err(0, 0, "You may want to use the -h <hostname> option");
	} else {
	  strcpy(hostname_store, m->mnt_fsname);
	  hostname = hostname_store;
	}
      }
    }
  } while (m);
  return(hostname);

#elif OSF1

  long mntsize, i;
  struct statfs *mntbuf;
  char *hostname;                                 /* Resulting hostname */
  static char hostname_store[MAXHOSTNAMELEN];     /* For safe keeping */
  char *p;
  
  if ((mntsize = getmntinfo(&mntbuf, MNT_NOWAIT)) == 0) {
    return(NULL);
  }
  hostname = NULL;
  /*
   * We'll scan the entire mount table, if there are multiple
       * possibilities, we'll take the first one and print warnings
       * about the other ones.
       */
  for (i = 0; i < mntsize; i++) {
    if (p = strstr(mntbuf[i].f_mntfromname, ":/...")) {
      /*
       * We assume this to be a DFS mount point, and that
       * the node is identified by the string up to "p".
       */
      *p = '\0';
      if ((hostname) && (strcmp(hostname, mntbuf[i].f_mntfromname))) {
	com_err(progname, 0, 
		"warning - multiple DFS mount points (hosts) found - using %s, ignoring %s", 
		hostname, mntbuf[i].f_mntfromname);
	com_err(0, 0, "You may want to use the -h <hostname> option");
      } else {
	strcpy(hostname_store, mntbuf[i].f_mntfromname);
	hostname = hostname_store;
      }
    }
  }
  return(hostname);

#else    /* for default, do not persume to know how to read mount table */

  return(NULL);
#endif
}

/*
 * default_username - Get the default user login name.
 *
 * This routine gets the user name as a string, looking first
 * in the ticket cache (ala kinit), and if that fails, unix.
 *
 * It's possible for the unix lookup to fail as well, resulting
 * in an unknown user.
 *
 * This routine returns the username in static storage - repeated
 * calls will overwrite it.
 *
 * Return value:
 *	Pointer to username or NULL on error.
 */
char *default_username()
{
  char *username;			/* Result of routine */
  krb5_ccache ccache = NULL;	/* Default ticket cache */
  krb5_principal user;		/* Resulting principal from ticket cache */
  struct passwd *pw;		/* Password for unix */
  
  username = NULL;
  /*
   * First try ticket cache.
   */
  if (!krb5_cc_default(&ccache)) {
    if (!krb5_cc_get_principal(ccache, &user)) {
      if (krb5_unparse_name(user, &username)) {
	username = NULL;
      }
    }
  }
  if (!username) {
    /*
     * All that didn't work.  Talk to unix.
     */
    if (pw = getpwuid(getuid())) {
      username = pw->pw_name;
    }
  }
  return(username);
}

/*
 * default_sysname - Get the default machine sysname.
 *
 * Return value:
 *      Pointer to sysname or NULL on error.
 */
char *default_sysname()
{
  char *sysname;   	/* Result of routine, @sys value for the nfs client */

#if HAVE_UNAME
  /*
   * Construct the SYS_NAME from the utsname info.
   */
  static sysname_store[UTSLEN*2]; /* for safe keeping */
  struct utsname utsname;
  char *mp, *rp;
  
  sysname = (char *)sysname_store;
  uname(&utsname);
  /* Parse "9000/8xx" into "800" (or 700) */
  for (mp = utsname.machine; *mp && *mp != '/'; mp++);
  if (*mp == '/') {
    sprintf(sysname, "hp%c00_ux", *(++mp));
    for (mp = sysname; *mp; mp++);
    /* Parse "B.10.00" into "1000" or B.09.03 into "903" */
    for (rp = utsname.release; *rp && *rp != '.'; rp++);
    if (*rp != '.') goto fail;
    rp++;       /* skip "." */
    if (*rp == '0') rp++; /* skip leading zero */
    while (*rp && *rp != '.')
      *mp++ = *rp++;
    if (*rp != '.') goto fail;
    rp++;       /* skip "." */
    while (*rp)
      *mp++ = *rp++;
    *mp = '\0'; /* null terminate */
  } else
 fail:
#endif
    sysname = NULL;
  return(sysname);
}

/*
 * build_dfsgw_prot_version - build gateway protocol portion of remote authentication packet.
 *
 * Inputs:
 *      version         : dfsgw protocol version packet to build.
 *      type            : dfsgw packet type (auth or rauth)
 *      username        : dce username.
 *      localname       : hostname of client
 *      sysname         : system name of nfs client
 *      packet          : The packed TGT
 *      tgt_len         : size of TGT (0 if this is a logout request)
 *      total           : total size of protocol version pkt
 * Return value:
 *      unsigned long ptr to built packet.
 */
unsigned long *build_dfsgw_prot_version
#ifdef __STDC__
(unsigned char version, unsigned char type, char *username, char *localname, 
 char *sysname, char *packet, int tgt_len, int *total)
#else
(version, type, username, localname, sysname, packet, tgt_len, total)
  unsigned char version;
  unsigned char type;
  char *username;
  char *localname;
  char *sysname;
  char *packet;
  int tgt_len;
  int *total;
#endif /* __STDC__ */
{
  unsigned long *pktptr;                  /* Our data packet */
  unsigned char *cdata;                   /* Character pointer into packet */
  int off;                                /* Offset for building data packet */
  int len;                                /* Lengths */
  struct hostent *hostinfo;               /* client hostent info */

  /*
   * dfsgw protocol version portion of remote authentication packet
   *
   *      unsigned char version
   *      unsigned char DFSGW_AUTH_PACKET or DFSGW_RAUTH_PACKET (host_byte_order encoded in lsb)
   *      (round up to longword boundary)
   *      unsigned long dce_username_len
   *      <dce_username>
   *      (round up to longword boundary)
   *      unsigned long networkAddrLen
   *      <networkAddr>
   *      (round up to longword boundary)
   *      unsigned long unix user ID
   *      (round up to longword boundary)
   *      unsigned long sysname_len       version 2
   *      <sysname>                       version 2
   *      (round up to longword boundary) version 2
   *      unsigned long size of packed TGT
   *      packed TGT
   */


  if ((hostinfo = gethostbyname(localname)) == NULL) {
    fatal("Couldn't get local network addr");
  }
  switch (version) {
  case DFSGW_PROTOCOL_VERSION :
    *total = (sizeof(unsigned long) * 6) + dfsgw_roundup(strlen(sysname)) + dfsgw_roundup(hostinfo->h_length) + dfsgw_roundup(strlen(username)) + tgt_len;
    break;
  default:
    *total = (sizeof(unsigned long) * 5) + dfsgw_roundup(hostinfo->h_length) + dfsgw_roundup(strlen(username)) + tgt_len;
    break;
  }
  /*
   * Make sure our packet is within bounds.
   */
  if (*total > DFSGW_MAXPSIZE) {
    fatal("Authentication packet exceeded protocol size");
  }
  if ((pktptr = (unsigned long *)malloc(*total)) == NULL) {
    fatal("memory allocation failure");
  }
  /*
   * Version, type, etc.
   */
  cdata = (unsigned char *)pktptr;
  *cdata++ = version;
  
  switch(type) {
  case DFSGW_AUTH_PACKET :
    *cdata++ = DFSGW_AUTH_PACKET | host_byte_order;
    break;
  case DFSGW_RAUTH_PACKET :
    *cdata++ = DFSGW_RAUTH_PACKET | host_byte_order;
    break;
  default :
    fatal("unknown packet type cannot process");
    break;
  }

  off = 1;
  /*
   * Username
   */
  len = strlen(username);
  *(pktptr + off++) = len;
  strcpy((char *)(pktptr + off), username);
  off += (dfsgw_roundup(len) / sizeof(unsigned long));    /* off will be longword index */
  /*
   * Network address
   */
  memcpy((pktptr + off++), &hostinfo->h_length, sizeof(unsigned long));
  memcpy((pktptr + off), hostinfo->h_addr, hostinfo->h_length);
  off += (dfsgw_roundup(hostinfo->h_length) / sizeof(unsigned long));
  /*
   * UID
   */
  *(pktptr + off++) = getuid();
  switch (version) {
  case DFSGW_PROTOCOL_VERSION :
    /*
     * Sysname
     */
    len = strlen(sysname);
    *(pktptr + off++) = len;
    strcpy((char *)(pktptr + off), sysname);
    off += (dfsgw_roundup(len) / sizeof(unsigned long));
    break;
  default :
    break;
  }
  /*
   * TGT
   */
  if (tgt_len) {
    *(pktptr + off++) = tgt_len;
    memcpy((pktptr + off), packet, tgt_len);
  }
  return(pktptr);
}
