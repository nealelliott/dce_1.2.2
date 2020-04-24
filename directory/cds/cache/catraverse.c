/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: catraverse.c,v $
 * Revision 1.1.12.2  1996/02/18  19:24:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:25  marty]
 *
 * Revision 1.1.12.1  1995/12/08  00:11:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:45  root]
 * 
 * Revision 1.1.9.8  1994/08/03  18:59:04  mccann
 * 	fix assert
 * 	[1994/07/21  16:50:37  mccann]
 * 
 * Revision 1.1.9.7  1994/06/30  19:15:13  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:46:44  mccann]
 * 
 * 	sams cleanup drop2
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.9.6  1994/06/23  18:28:36  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:46:40  mccann]
 * 
 * Revision 1.1.9.5  1994/06/09  16:07:20  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:54  devsrc]
 * 
 * Revision 1.1.9.4  1994/06/08  20:17:50  peckham
 * 	Initialize serviceability and message catalogs.
 * 	[1994/05/31  18:56:02  peckham]
 * 
 * Revision 1.1.9.3  1994/05/12  21:10:44  peckham
 * 	Remove direct NBPG reference.
 * 	[1994/05/12  19:13:09  peckham]
 * 
 * Revision 1.1.9.2  1994/04/14  14:38:10  peckham
 * 	Remove VMS and UNIX conditionals.
 * 	[1994/04/11  18:09:48  peckham]
 * 
 * Revision 1.1.9.1  1994/03/12  21:56:06  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:49:05  peckham]
 * 
 * Revision 1.1.6.5  1992/12/30  12:37:39  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:21:13  zeliff]
 * 
 * Revision 1.1.6.4  1992/12/18  00:32:55  jd
 * 	Alan Peckham, 17-Dec-1992
 * 	Have a cookie for being so good.
 * 	With the new casys.c, use CASysFindMem() to get the shmid
 * 	when -m or -n is not supplied.
 * 	[1992/12/18  00:25:41  jd]
 * 
 * Revision 1.1.6.3  1992/12/15  15:54:31  tom
 * 	Bug 6409 - add setlocale call for i18n.
 * 	[1992/12/15  15:16:46  tom]
 * 
 * Revision 1.1.6.2  1992/09/29  18:28:48  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:09:28  weisman]
 * 
 * Revision 1.1.2.3  1992/04/24  17:41:35  keegan
 * 	<Allow catraverse to build without BLD_DEBUG set
 * 	[1992/04/24  15:54:17  keegan]
 * 
 * Revision 1.1.3.2  1992/04/24  15:16:47  keegan
 * 	Allow 'catraverse' to build withoug BLD_DEBUG set.
 * 
 * Revision 1.1.2.2  1992/03/22  21:16:25  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:06:29  weisman]
 * 
 * Revision 1.1  1992/01/19  15:24:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module catraverse.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 */
#include <cache.h>
#include <dce/assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <uc_clerk.h>
#include <threads_lib.h>
#include <locale.h>

/*
 * prototype declarations of procedures in this module
 */
int
main (
    int              ,
    char            **);

/* Pointer to start of cache */
extern struct CacheFixed *CAfixed_p;

/*
 * Use shadow flag vector to mark info on cache blocks
 */
static unsigned char *shadow_p;
#define SH_VLD	(1<<7)		/* valid CacheHeader address */
#define SH_BRKN	(1<<6)		/* invalid CacheHeader contents */
#define SH_LNK	(1<<5)		/* inconsistent links */
#define SH_BLK	(1<<4)		/* consistent from ca_free thru size */
#define SH_REF	(1<<3)		/* block checked at some time */
#define SH_GLRU	(1<<2)		/* block on ca_Glru */
#define SH_CLRU	(1<<1)		/* block on ca_Clru */
#define SH_FREE	(1<<0)		/* block on ca_free */
#define SH_LIST	(SH_FREE|SH_CLRU|SH_GLRU)	/* block on some list */

#define valid_offset(v) ((0 <= (v)) && ((v) < CAfixed_p->ca_size))

static void
display_cache (void);

static int
valid_header (
    CacheHeader_t *,
    int		   );

/* return values */
#define VAL_OK		0
#define VAL_ALGN	1	/* address not on CacheHeader boundry */
#define VAL_LOOP	2	/* entry in list with cycle */
#define VAL_BRKN	3	/* entry contains bad data */

static int
walk_list (
    CacheHeader_t *,
    int		   );

static void
dump_entry (
    CacheHeader_t *,
    int		   );

static void
dump_header (
    CacheHeader_t *);

static void
dump_all (void);

static void
follow_freelist (void);

static void
follow_ClientLRU (void);

static void
follow_GlobalLRU (void);

static int
ReadDump (
    char *);

static void
shadow_reset (
    int		,
    int		);

/* extern int getopt(); */
 extern char *optarg;
 extern int optind;

/*
 * Check cache lists.
 */
int 
main (register int   argc,
      register char  *argv[])
{
  dnsFlagStat_t dnsstatus;
  char svcname[128];
  char *myname_p;
  error_status_t st;
  long value = -1L;
  int option;

  /*
   * Get our filename, stripped of directories
   */
  if (myname_p = strrchr(argv[0], '/'))
    myname_p++;
  else
    myname_p = argv[0];

  setlocale(LC_ALL, "");

  (void)sprintf(svcname, "%s(%ld)", myname_p, (long)getpid());
  dce_svc_set_progname(svcname, &st);
  dce_assert(cds__svc_handle, (st == svc_s_ok));

  dce_msg_define_msg_table(cds_msg_table,
                           sizeof(cds_msg_table) / sizeof(cds_msg_table[0]),
                           &st);
  dce_assert(cds__svc_handle, (st == msg_s_ok));

  dthread_init();

  /* Map shared memory */
  option = getopt(argc, argv, "m:n:");
  switch (option) {
  default:
    /* Try shmid file */
    if (0 <= (value = CASysFindMem()))
      goto mapit;
    /* Get current cache id from child */
    if (cdsGetInfo(&value, &dnsstatus) == DNS_SUCCESS)
      goto mapit;

    (void)fprintf(stderr, "usage: catraverse; or: catraverse -m Cacheid; or: catraverse -n filename\n");
    exit(0);
  case 'm':
    value = atoi(optarg);
  mapit:
    if (CASysMap(value) != DNS_SUCCESS) {
      (void)fprintf(stderr, "Cannot map %d\n", value);
      exit(0);
    }
    /* Make sure it is really the clerk cache */
    if (!CheckCacheVersion(CAfixed_p)) {
      CASysUnmap();
      (void)fprintf(stderr, 
	      "Shared memory is not a DnsClerkCache version %1d.%1d\n",
	      MAJOR, MINOR);
      exit(0);
    }
    break;
  case 'n':
    if (!ReadDump(optarg)) {
      (void)fprintf(stderr, "%s not a valid cache file\n", optarg);
      exit(0);
    }
    break;
  }

  shadow_p = (unsigned char *)malloc(CAfixed_p->ca_size);
  if (shadow_p) {
    (void)memset(shadow_p, '\0', CAfixed_p->ca_size);
    shadow_reset(0, SH_VLD);
  }
  /* Go through each hash table and dump it contents to a file */
  if (option == 'm') {
    CAWriteLockCache();
    display_cache();
    CAWriteUnlockCache();
  } else
    display_cache();

  if (shadow_p) free(shadow_p);

  exit(0);
  /*NOTREACHED*/
}
static void 
display_cache (void)
{
  (void)walk_list(&CAfixed_p->ca_free, SH_FREE);
  (void)walk_list(&CAfixed_p->ca_Clru, SH_CLRU);
  (void)walk_list(&CAfixed_p->ca_Glru, SH_GLRU);
  dump_all();

  shadow_reset(SH_LIST, 0);

  follow_freelist();
  follow_ClientLRU();
  follow_GlobalLRU();
}

/*
 * Validate entry
 */
static int 
valid_header (CacheHeader_t  *cache_p,
              int            flag)
{
  const int offset = CA_GET_OFFSET(cache_p);

  shadow_p[offset] |= SH_REF;

  if (shadow_p) {
    if (!(shadow_p[offset] & SH_VLD))
      return(VAL_ALGN);

    if (shadow_p[offset] & flag)
      return(VAL_LOOP);

    shadow_p[offset] |= flag;
  }

  if (cache_p->hdr.pattern != PATTERN) {
    shadow_p[offset] |= SH_BRKN;
    return(VAL_BRKN);
  }
  switch (cache_p->hdr.inuse) {
  case 1:
    if (!(flag & SH_FREE))
      break;
    shadow_p[offset] |= SH_BRKN;
    return(VAL_BRKN);
  case 0:
    if (!(flag & (SH_CLRU|SH_GLRU)))
      return(VAL_OK);
  default:
    shadow_p[offset] |= SH_BRKN;
    return(VAL_BRKN);
  }
  if (!valid_offset(cache_p->hdr.list.ll_flink)) {
    shadow_p[offset] |= SH_BRKN;
    return(VAL_BRKN);
  } else if (cache_p->hdr.list.ll_flink
	     && (CA_GET_CAPOINTER(cache_p->hdr.list.ll_flink)->hdr.list.ll_blink != offset))
    shadow_p[offset] |= SH_LNK;

  if (!valid_offset(cache_p->hdr.list.ll_blink)) {
    shadow_p[offset] |= SH_BRKN;
    return(VAL_BRKN);
  }  else if (cache_p->hdr.list.ll_blink
	      && (CA_GET_CAPOINTER(cache_p->hdr.list.ll_blink)->hdr.list.ll_flink != offset))
    shadow_p[offset] |= SH_LNK;

  switch (cache_p->hdr.type) {
  case Othertype:
  case DHEntry:
  case CHEntry:
  case LHEntry:
  case NHEntry:
  case AHEntry:
  case VHEntry:
  case UHEntry:
  case UAEntry:
  case UGEntry:
    break;
  default:
    shadow_p[offset] |= SH_BRKN;
    return(VAL_BRKN);
  }
  return(VAL_OK);
}

/*
 * Follow list pointers
 */
static int 
walk_list (CacheHeader_t  *header_p,
           int            flag)
{
  register CacheHeader_t *cache_p;
  register int last, broken = FALSE;

  for (last = 0, cache_p = CA_GET_CAPOINTER(header_p->hdr.list.ll_flink);
       cache_p != header_p;
       cache_p = CA_GET_CAPOINTER(cache_p->hdr.list.ll_flink))
    if (valid_header(cache_p, flag) == VAL_OK) {
      if (last && (cache_p->hdr.list.ll_blink != last))
        (void)walk_list(CA_GET_CAPOINTER(cache_p->hdr.list.ll_blink), flag);
      last = CA_GET_OFFSET(cache_p);
    } else {
      broken = TRUE;
      break;
    }

  for (last = 0, cache_p = CA_GET_CAPOINTER(header_p->hdr.list.ll_blink);
       cache_p != header_p;
       cache_p = CA_GET_CAPOINTER(cache_p->hdr.list.ll_blink))
    if (valid_header(cache_p, flag) == VAL_OK) {
      if (last && (cache_p->hdr.list.ll_flink != last))
        (void)walk_list(CA_GET_CAPOINTER(cache_p->hdr.list.ll_flink), flag);
      last = CA_GET_OFFSET(cache_p);
    } else {
      broken = TRUE;
      break;
    }

  return(broken);
}

/*
 * Dump type and in/use flag
 */
static void 
dump_entry (CacheHeader_t  *cache_p,
            int            flags)
{
  LOG_ASCII_BUF_AttributeName(attrBuf)
  LOG_ASCII_BUF_FullName(fullBuf)
  LOG_ASCII_BUF_token(tokenBuf)
  LOG_ASCII_BUF_UserEntry(userBuf)

  if (!cache_p)
    (void)printf("       Addr   flink  blink   Size  Refs\n");
  else {
    const int offset = CA_GET_OFFSET(cache_p);
    const int data = offset + sizeof(CacheHeader_t);

    (void)printf("%c%c%c%c%c%6x  %6x %6x  %5x  %4d ",
      ((cache_p->hdr.inuse) ? 'a' : 'f'),
      ((flags & SH_FREE) ? 'f' : ' '),
      ((flags & SH_GLRU) ? 'g' : ' '),
      ((flags & SH_CLRU) ? 'c' : ' '),
      ((flags & SH_BLK) ? ' ' : 'x'),
      offset,
      cache_p->hdr.list.ll_flink,
      cache_p->hdr.list.ll_blink,
      cache_p->hdr.size,
      cache_p->hdr.refs);

    if (cache_p->hdr.inuse)
      switch (cache_p->hdr.type) {
      case DHEntry:
	{ DirEntry_t *const dh_p = (DirEntry_t *)&cache_p[1];
	(void)printf("dh %s\n", deb_ascii_FullName(fullBuf,dh_p->dh_name)); }
	break;
      case CHEntry:
	{ CleEntry_t *const ch_p = (CleEntry_t *)&cache_p[1];
	(void)printf("ch %s\n", deb_ascii_FullName(fullBuf,ch_p->ch_name)); }
	break;
      case LHEntry:
	{ LinkEntry_t *const lh_p = (LinkEntry_t *)&cache_p[1];
	(void)printf("lh %s\n", deb_ascii_FullName(fullBuf,lh_p->lh_name)); }
	break;
      case NHEntry:
	{ NameEntry_t *const nh_p = (NameEntry_t *)&cache_p[1];
	(void)printf("nh %s\n", deb_ascii_FullName(fullBuf,nh_p->nh_name)); }
	break;
      case AHEntry:
	{ AttrEntry_t *const ah_p = (AttrEntry_t *)&cache_p[1];
	(void)printf("ah %s\n", deb_ascii_AttributeName(attrBuf,ah_p->ah_name));
	}
	break;
      case VHEntry:
	{ ValueEntry_t *const av_p = (ValueEntry_t *)&cache_p[1], 
		       *dummy;
	/* 
	 * The dummy variable is here to get rid of a compiler warning,
	 * remove it when deb_ascii_token is redefined.
	 */
	dummy = av_p;
	(void)printf("av %s\n", deb_ascii_token(tokenBuf,av_p->av_token)); }
	break;
      case UHEntry:
	{ UserEntry_t *const uh_p = (UserEntry_t *)&cache_p[1];
	(void)printf("uh %s\n", deb_ascii_UserEntry(userBuf,uh_p)); }
	break;
      case UAEntry:
	{ UserAccessEntry_t *const ua_p = (UserAccessEntry_t *)&cache_p[1],
	  		    *dummy;
	/* 
	 * The dummy variable is here to get rid of a compiler warning,
	 * remove it when deb_ascii_token is redefined.
	 */
	dummy = ua_p;
	(void)printf("ua %s\n", deb_ascii_token(tokenBuf,ua_p->ua_token)); }
	break;
      case UGEntry:
	{ UserGroupEntry_t *const ug_p = (UserGroupEntry_t *)&cache_p[1];
	(void)printf("ug %s\n", deb_ascii_FullName(fullBuf,ug_p->ug_name)); }
	break;
      case Othertype:
	if (data == CAfixed_p->ca_dir)
	  { (void)printf("dh hash\n"); break; }
	else if (data == CAfixed_p->ca_link)
	  { (void)printf("lh hash\n"); break; }
	else if (data == CAfixed_p->ca_cle)
	  { (void)printf("ch hash\n"); break; }
	else if (data == CAfixed_p->ca_user)
	  { (void)printf("uh hash\n"); break; }
	else if (data == CAfixed_p->ca_name)
	  { (void)printf("nh hash\n"); break; }
	else if (data == CAfixed_p->ca_attr)
	  { (void)printf("av hash\n"); break; }
      default:
	(void)printf("(%d)\n", cache_p->hdr.type);
      }
    else
      (void)printf("\n");

    if (flags & SH_BRKN)
      dump_header(cache_p);
  }
}

static void 
dump_header (CacheHeader_t *cache_p)
{
  register unsigned int *p = (unsigned int *)cache_p;
  const int offset = CA_GET_OFFSET(cache_p);

  (void)printf("  %6x  %08x %08x %08x %08x\n",
    offset, p[0], p[1], p[2], p[3]);
  (void)printf("  %6x  %08x %08x %08x %08x\n",
    offset+16, p[4], p[5], p[6], p[7]);
}

/*
 * walk entire cache, dump type and in/use flag 
 */
static void 
dump_all (void)
{
  register CacheHeader_t *cache_p;
  register int offset;
  const int size = CAfixed_p->ca_size - sizeof(*CAfixed_p);

  for (cache_p = &CAfixed_p->ca_free + 1;
       0 < cache_p->hdr.size;
       cache_p = cache_p + (1 + cache_p->hdr.size))
    if (valid_header(cache_p, SH_BLK) != VAL_OK)
      break;

  for (cache_p = &CAfixed_p->ca_free + (size/sizeof(CacheHeader_t));
       0 < cache_p->hdr.prevsize;
       cache_p = cache_p - (1 + cache_p->hdr.prevsize))
    if (valid_header(cache_p, SH_BLK) != VAL_OK)
      break;

  dump_entry(NULL, 0);

  for (offset = CA_GET_OFFSET(&CAfixed_p->ca_free + 1);
       offset < CAfixed_p->ca_size;
       offset++)
    if (shadow_p[offset] & SH_REF)
      dump_entry(CA_GET_CAPOINTER(offset), shadow_p[offset]);

}

/*
 * Follow freelist pointers and make sure space adds up.
 */
static void 
follow_freelist (void)
{
  register CacheHeader_t *cache_p;
  register int total = 0, last = 0;
  
  (void)printf("\nFreeList sizes ***********************\n");
  
  /* Locate first free block that will satisfy this request */
  for (cache_p = CA_GET_CAPOINTER(CAfixed_p->ca_free.hdr.list.ll_flink);
       cache_p != &CAfixed_p->ca_free;
       cache_p = CA_GET_CAPOINTER(cache_p->hdr.list.ll_flink)) {
    const int offset = CA_GET_OFFSET(cache_p);

    if (last && (cache_p->hdr.list.ll_blink != last)) {
      (void)printf("  %6x blink %6x != %6x\n",
		offset, cache_p->hdr.list.ll_blink, last);
    }
    last = offset;

    switch (valid_header(cache_p, SH_FREE)) {
    case VAL_OK:
      (void)printf("%6x = %d\n", offset, cache_p->hdr.size);

      /* Include header in total */
      total += 1 + cache_p->hdr.size;
      continue;
    case VAL_BRKN:
      (void)printf("%6x corrupted , quitting\n", offset);
      break;
    case VAL_ALGN:
      (void)printf("  %6x INVALID!!! aborting\n", offset);
      break;
    case VAL_LOOP:
      (void)printf("%6x LOOP!!!!!\n", offset);
      break;
    }
    dump_header(cache_p);
    break;
  }

  /* Last block of cache is preallocted, account for it here */
  total = CAfixed_p->ca_total - CAfixed_p->ca_Gused - CAfixed_p->ca_Cused - 
          total; 

  if (total) 
    (void)printf("MISSING %d OF FREESPACE \n", total);
  else
    (void)printf("ALL FREESPACE ACCOUNTED FOR\n");
   
}

/*
 * Follow client LRU list and make sure space adds up.
 */
static void 
follow_ClientLRU (void)
{
  register CacheHeader_t *cache_p;
  register int total = 0, last = 0;
  
  (void)printf("\nClient LRU sizes ***********************\n");

  /* Locate first free block that will satisfy this request */
  for (cache_p = CA_GET_CAPOINTER(CAfixed_p->ca_Clru.hdr.list.ll_flink);
       cache_p != &CAfixed_p->ca_Clru;
       cache_p = CA_GET_CAPOINTER(cache_p->hdr.list.ll_flink)) {
    const int offset = CA_GET_OFFSET(cache_p);

    if (last && (cache_p->hdr.list.ll_blink != last)) {
      (void)printf("  %6x blink %6x != %6x\n",
		offset, cache_p->hdr.list.ll_blink, last);
    }
    last = offset;

    switch (valid_header(cache_p, SH_CLRU)) {
    case VAL_OK:
      (void)printf("%6x = size %d,  type %d\n",
		offset, cache_p->hdr.size, cache_p->hdr.type);
      /* Include header in total */
      total += 1 + cache_p->hdr.size;
      continue;
    case VAL_ALGN:
      (void)printf("  %6x INVALID!!! aborting\n", offset);
      break;
    case VAL_LOOP:
      (void)printf("%6x LOOP!!!!!\n", offset);
      break;
    case VAL_BRKN:
      (void)printf("%6x corrupted , quitting\n", offset);
      break;
    }
    dump_header(cache_p);
    break;
  }

  /* Last block of cache is preallocted, account for it here */
  total = CAfixed_p->ca_Cused - total;

  if (total)
    (void)printf("MISSING %d FOR CLIENT LRU LIST\n", total);
  else
    (void)printf("ALL CLIENT SPACE ACCOUNTED FOR\n");
   
}

/*
 * Follow global LRU list and make sure space adds up.
 */
static void 
follow_GlobalLRU (void)
{
  register CacheHeader_t *cache_p;
  register int total = 0, last = 0;
  
  (void)printf("\nGlobal LRU sizes ***********************\n");

  /* Locate first free block that will satisfy this request */
  for (cache_p = CA_GET_CAPOINTER(CAfixed_p->ca_Glru.hdr.list.ll_flink);
       cache_p != &CAfixed_p->ca_Glru;
       cache_p = CA_GET_CAPOINTER(cache_p->hdr.list.ll_flink)) {
    const int offset = CA_GET_OFFSET(cache_p);

    if (last && (cache_p->hdr.list.ll_blink != last)) {
      (void)printf("  %6x blink %6x != %6x\n",
		offset, cache_p->hdr.list.ll_blink, last);
    }
    last = offset;

    switch (valid_header(cache_p, SH_GLRU)) {
    case VAL_OK:
      (void)printf("%6x = size %4d,  type %d\n",
		offset, cache_p->hdr.size, cache_p->hdr.type);
      /* Include header in total */
      total += 1 + cache_p->hdr.size;
      continue;
    case VAL_ALGN:
      (void)printf("  %6x INVALID!!! aborting\n", offset);
      break;
    case VAL_LOOP:
      (void)printf("%6x LOOP!!!!!\n", offset);
      break;
    case VAL_BRKN:
      (void)printf("%6x corrupted , quitting\n", offset);
      break;
    }
    dump_header(cache_p);
    break;
  }

  /* Last block of cache is preallocted, account for it here */
  total = CAfixed_p->ca_Gused - total;

  if (total)
    (void)printf("MISSING %d FOR GLOBAL LRU LIST\n", total);
  else
    (void)printf("ALL GLOBAL SPACE ACCOUNTED FOR\n");
   
}
static int 
ReadDump (char *name_p)
{
  int status;
  int size;
  int cachefile;
  struct CacheFixed cache;
  char *buf_p;

  /* Open cache file */
  cachefile = open(name_p, O_RDONLY, 0); 
  if (cachefile <= 0) return(0);
  
  /* File was found, read fixed portion to get cache size */
  status = read(cachefile, (char *)&cache, sizeof(struct CacheFixed));  
  if ((status <= 0) || (status < sizeof(struct CacheFixed))) {
    (void)close(cachefile);
    return(0);
  }

  /* Make sure it is the current version.  If not, ignore file */
  if (!CheckCacheVersion(&cache)) {
    (void)close(cachefile);
    return(0);
  }

  if (!(CAfixed_p = (struct CacheFixed *)malloc(cache.ca_size))) {
    (void)close(cachefile);
    return(0);
  }
  
  /* Move fixed portion into shared memory */
  *CAfixed_p = cache;
  
  /* Read the rest of cache into memory */
  size = CAfixed_p->ca_size - sizeof(struct CacheFixed);
  buf_p = (char *)(CAfixed_p + 1);
  while (size > 0) {
    if (size > _CDS_PAGE_SIZE_) 
      status = read(cachefile, buf_p, _CDS_PAGE_SIZE_);
    else 
      status = read(cachefile, buf_p, size);
    
    /* If what we were expecting wasn't there, return an error */  
    if (status <= 0) {
      (void)close(cachefile);
      return(0);
    }
    
    size -= status;
    buf_p += status;
  }
  (void)close(cachefile);
  return(1);
}

static void 
shadow_reset (int  off,
              int  on)
{
  if (shadow_p) {
    register int i;

    for (i = CA_GET_OFFSET(&CAfixed_p->ca_free + 1);
	 i < CAfixed_p->ca_size;
	 i += sizeof(CacheHeader_t))
      shadow_p[i] = (shadow_p[i] & ~off) | on;
  }
}
