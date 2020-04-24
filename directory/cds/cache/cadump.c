/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cadump.c,v $
 * Revision 1.1.11.2  1996/02/18  19:24:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:01  marty]
 *
 * Revision 1.1.11.1  1995/12/08  00:10:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:30  root]
 * 
 * Revision 1.1.8.6  1994/08/16  17:27:11  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Replace CALockGlobal() by CALockData(),
 * 	CAUnlockGlobal() by CAUnlockData(),
 * 	CALockClient() by CALockData(),
 * 	CAUnlockClient() by CAUnlockData().
 * 	[1994/08/16  15:29:59  peckham]
 * 
 * Revision 1.1.8.5  1994/06/23  18:28:20  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:35:53  mccann]
 * 
 * Revision 1.1.8.4  1994/06/09  16:07:04  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:39  devsrc]
 * 
 * Revision 1.1.8.3  1994/05/12  21:10:38  peckham
 * 	Remove direct NBPG reference.
 * 	Remove unnecessary conditionals.
 * 	[1994/05/12  19:12:49  peckham]
 * 
 * Revision 1.1.8.2  1994/04/14  14:37:55  peckham
 * 	Remove VMS conditional code.
 * 	[1994/04/11  17:35:01  peckham]
 * 
 * Revision 1.1.8.1  1994/03/12  21:55:36  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:47:14  peckham]
 * 
 * Revision 1.1.5.6  1992/12/30  12:36:34  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:19:36  zeliff]
 * 
 * Revision 1.1.5.5  1992/12/18  00:32:51  jd
 * 	Alan Peckham, 17-Dec-1992
 * 	Have a cookie for being so good.
 * 	With the new casys.c, use CASysFindMem() to get the shmid
 * 	if -m or -n is not supplied.
 * 	[1992/12/18  00:23:47  jd]
 * 
 * Revision 1.1.5.4  1992/12/10  15:38:41  mfox
 * 	Replace usage of obsolete utc functions
 * 	[1992/12/10  15:35:09  mfox]
 * 
 * Revision 1.1.5.3  1992/10/15  19:22:12  m_sawyer
 * 	add display of clearinghouse management tag.
 * 	[1992/10/15  19:19:56  m_sawyer]
 * 
 * Revision 1.1.5.2  1992/09/29  18:27:58  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:09:06  weisman]
 * 
 * Revision 1.1.2.2  1992/03/22  21:11:53  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:04:38  weisman]
 * 
 * Revision 1.1  1992/01/19  15:24:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: cadump.c
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
#include <fcntl.h>
#include <dce/utc.h>
#include <dce/rpc.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <unistd.h>
#include <uc_child.h>
#include <uc_clerk.h>
#include <tower.h>

int hex_print = 0; /* h option sets this to 1 to force hex print of towers */

/* Pointer to start of cache */

typedef struct {
    void	*ll_flink;
    void	*ll_blink;
} dumpLinkedList_t;

#define ATTHASHSIZE 53
dumpLinkedList_t AttHash[ATTHASHSIZE];

struct ahentry {
  dumpLinkedList_t ah_collision;
  AttrEntry_t *ah_ptr;
};

#define NAMEHASHSIZE 101
dumpLinkedList_t NameHash[NAMEHASHSIZE];

struct nhentry {
  dumpLinkedList_t nh_collision;
  NameEntry_t *nh_ptr;
};

#define LINKHASHSIZE 101
dumpLinkedList_t LinkHash[LINKHASHSIZE];

struct lhentry {
  dumpLinkedList_t lh_collision;
  LinkEntry_t *lh_ptr;
};

/* Bit definitions to control which caches are displayed */
#define ACACHE  1<<0 /* Attribute values cache */
#define DCACHE  1<<1 /* directory cache */
#define CCACHE  1<<2 /* Clearinghouse cache */
#define UCACHE  1<<3 /* user cache */

static void 
buildattributehash (void);

static void
dumpname (void);

static void
dumpnames (
    UserEntry_t *);

static void
dumpgroups (
    UserEntry_t *);

static void
dumpuser (void);

static void
dumpdir (void);

static void
dumpcle (void);

static void
print_towers (
    Set_t   *);

static void 
buildnamehash (void);

static int
ReadDump (
    char *);

static void
buildlinkhash (void);

int 
cdsGetInfo (
    long          *,
    dnsFlagStat_t *);

static char *usage_msg = 
    "usage: cadump [-acdhu]; or: cadump [-macdhu] Cachid; or: cadump [-nacdhu] filename";


/*
 * Dump the caches to a file
 */
dns_status_t 
cadump (int   argc,
        char  *argv[],
        int   usage_flag)
{
  dnsFlagStat_t dnsstatus;
  long value = -1;
  int option;
  int caches = 0;
  int mapped = 0;
  int readin = 0;
  
  /* Map shared memory */
  while  ((option = getopt(argc, argv, "m:n:acdhu")) != EOF)
    switch (option) 
    {
      case 'm':
	if (mapped || readin) break;
	value = atoi(optarg);
        mapit:
  	if (CASysMap(value) != DNS_SUCCESS) 
	{
	    (void)fprintf(stderr,
		"Cannot map %d\n - check id and protection\n", value);
	    return(DNS_ERROR);
 	}
	/* Make sure it is really the clerk cache */
	if (!CheckCacheVersion(CAfixed_p)) 
	{
	    CASysUnmap();
	    (void)fprintf(stderr, 
		    "Shared memory is not ClerkCache version %1d.%1d\n",
		    MAJOR, MINOR);
	    return(DNS_ERROR);
	}
	mapped = 1;
	break;
    case 'n':
	if (mapped || readin) break;
	if (!ReadDump(optarg)) 
	{
	    (void)fprintf(stderr,
		"%s not a valid cache file or protection error\n", optarg);
	    return(DNS_ERROR);
	}
	readin = 1;
	break;
    case 'a':
	caches |= ACACHE;
	break;
    case 'u':
	caches |= UCACHE;
	break;
    case 'd':
	caches |= DCACHE;
	break;
    case 'c':
	caches |= CCACHE;
	break;
    case 'h':
	hex_print = 1;
	break;
    default:
	(void)printf("%s\n", usage_msg);
	return(DNS_ERROR);
	
    } /* End switch */

  if (!mapped && !readin) 
  {
      int get_status;
      /* Get cache id from shmid file */
      if (0 <= (value = CASysFindMem()))
          goto mapit;
      /* Get current cache id from child */
      get_status = cdsGetInfo(&value, &dnsstatus);
      if (get_status == DNS_SUCCESS)
          goto mapit;
      if (usage_flag)
          (void)printf("%s\n", usage_msg);
      return(get_status);
  }

  if (caches == 0) caches = 0xffff;

/* Since VMS copies the cache to a user-mode area, it doesn't need to lock */
  CALockData();

  /* Display the default clerk timeout */
  (void)printf("Default timeout = %d seconds\n",
	CAfixed_p->ca_ClerkTimeout);

  /* Go through each hash table and dump it contents to a file */

  if (caches & DCACHE) 
    dumpdir();

  if (caches & CCACHE) 
    dumpcle();

  /* 
   * Build a hash table that will map the attribute time to the attribute
   * name so we can display the namehash with real attribute names.
   */
  if (caches & ACACHE) 
  {
      buildattributehash();
      dumpname();
  }

  /* Build 2 hash tables that will map the name token to the real name
   * for display
   */
  if (caches & UCACHE) 
  {
      buildnamehash();
      buildlinkhash();
      dumpuser();
  }

  CAUnlockData();

  if (mapped)
    CASysUnmap();

  
  return(DNS_SUCCESS);
}

/*
 * Dump tower in hex 
 */
static void 
print_towers (Set_t *set_p)
{
  int len,i;
  MemberValue_t *member_p;
  unsigned char **str_array;


  if (( !hex_print ) && 
	(dnsCvtTowerToStr(set_p, (byte_t **)&str_array ) == DNS_SUCCESS)) 
  {
    for ( i=0; str_array[i] != NULL; i++ ) 
    {
      (void)fprintf(stdout, "        ");
      (void)fprintf(stdout, "%s\n", str_array[i]);
    }
    free ((char *)str_array);
  }
  else

  for (i=0; i<NUM_Set(set_p); ++i) 
  {
    member_p = (MemberValue_t *)INDEX_Set(set_p, i);

    (void)fprintf(stdout, "        ");

    for (len=0; (len+1) < EXT16(member_p->mv_length); ++len)
      (void)fprintf(stdout, "%02x-", member_p->mv_value[len]);
    
    /* Last value without - */
    (void)fprintf(stdout, "%02x\n", member_p->mv_value[len]);
  }
  
  
}


/*
 * Dump cached directory names 
 */
static void 
dumpdir (void)
{
  int i,j;
  DirEntry_t *dir_p;
  CacheLinkedList_t *ll_p;
  unsigned char name[DNS_STR_FNAME_MAX];
  Set_t *set_p;
  int namelen;
  unsigned_char_t *string_p;
  unsigned32 status;

  
  (void)fprintf(stdout,
	  "\nCACHED DIRECTORIES*****************************************************\n");

  ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_dir);

  for (i=0; i<CAfixed_p->ca_dir_hash_len; ++i) 
  {
    dir_p = CA_GET_DHPOINTER((ll_p + i)->ll_flink);
    while ((CacheLinkedList_t *)dir_p != (ll_p + i)) 
    {
      namelen = sizeof(name);
      name[0] = 0;
      (void)dnsCvtCDSFullToStr((dns_opq_fname *)dir_p->dh_name, name, &namelen);
      (void)fprintf(stdout, "%.*s\n", namelen, name);
      /*
       * Display clearinghouse names for replicas 
       */
      
      set_p = (Set_t *)SKIP_FullName(dir_p->dh_name);
      for (j=0; j<NUM_Set(set_p); ++j) 
      {
	uuid_t id;
	MemberValue_t *const mv_p = (MemberValue_t *)INDEX_Set(set_p, j);
	ReplicaPointer_t *const rp_p = (ReplicaPointer_t *)mv_p->mv_value;
	FullName_t *const chname_p = (FullName_t *)
	    SKIP_BOOLEAN(SKIP_Set(rp_p->rp_addressHint));

	switch (EXT8(SKIP_Set(rp_p->rp_addressHint)))
	  {
	  case RT_master:
	    (void)fprintf(stdout, "    Master at ");
	    break;
	  case RT_readOnly:
	    (void)fprintf(stdout, "    ReadOnly at ");
	    break;
	  case RT_secondary:
	    (void)fprintf(stdout, "    Secondary at ");
	    break;
	  case RT_gda:
	    (void)fprintf(stdout, "    GDA at ");
	    break;
	  }

	/* Print clearinghouse name for this replica */
	namelen = sizeof(name);
	name[0] = 0;
	(void)dnsCvtCDSFullToStr((dns_opq_fname *)chname_p,  name, &namelen);
     	(void)fprintf(stdout, "%.*s\n", namelen, name);

	/* Print clearinhouse cts */
	COPY_bytes(rp_p->rp_CHID, &id, sizeof(uuid_t));
	uuid_to_string(&id, &string_p, &status);
	(void)fprintf(stdout, "    Clearinghouse UID: %s\n", string_p);
	rpc_string_free(&string_p, &status);

	/* Print out tower */
	(void)fprintf(stdout, "    Clearinghouse Address: \n");
	print_towers((Set_t *)rp_p->rp_addressHint);
	(void)fprintf(stdout, "\n");
      }
      dir_p = CA_GET_DHPOINTER(dir_p->dh_collision.ll_flink);
    }
  }
  
}

/*
 * Dump cached clearinhouses 
 */
static void 
dumpcle (void)
{
  CleEntry_t *cle_p;
  CacheLinkedList_t *ll_p;
  unsigned_char_t *string_p;
  Set_t *set_p;
  unsigned32 status;
  int i;

  
  (void)fprintf(stdout,
	  "\nCACHED CLEARINGHOUSES**************************************************\n");

  ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_cle);

  for (i=0; i<CAfixed_p->ca_cle_hash_len; ++i) 
  {
    cle_p = CA_GET_CHPOINTER((ll_p + i)->ll_flink);
    while ((CacheLinkedList_t *)cle_p != (ll_p + i)) 
    {
      unsigned char name[CDS_STR_SNAME_MAX];
      uuid_t id;
      SimpleName_t *mgmt_name_p;
      int namelen = sizeof(name);

      name[0] = 0;
      (void)dnsCvtCDSFullToStr((dns_opq_fname *)cle_p->ch_name, name, &namelen);

      (void)fprintf(stdout, "%.*s\n", namelen, name);

      /* Print clearinhouse cts */
      COPY_bytes(cle_p->ch_id, &id, sizeof(uuid_t));
      uuid_to_string(&id, &string_p, &status);
      (void)fprintf(stdout, "    Clearinghouse UID: %s\n", string_p);
      rpc_string_free(&string_p, &status);

      (void)fprintf(stdout,"  Clearinghouse address: \n");
      set_p = (Set_t *)SKIP_FullName(cle_p->ch_name);
      print_towers(set_p);

      namelen = sizeof(name);
      name[0] = '\0';

      mgmt_name_p = (SimpleName_t *)SKIP_Set(set_p);
      (void)dnsCvtCDSSimpleToStr((dns_opq_sname *)mgmt_name_p,
                                 name, &namelen);
      if (!strlen((char *)name))
        {
          strcpy((char *)name, "<none>");
        }
      (void)fprintf(stdout,"  Clearinghouse management name: %s\n", name);

      /* Print if used or onlan */
      if (cle_p->ch_flags.status.flags.onLAN == 1)
	(void)fprintf(stdout, "  OnLan");
      if (cle_p->ch_flags.status.flags.addressused == 1)
	(void)fprintf(stdout, "  AddressUsed");
      if (cle_p->ch_flags.status.flags.sticky == 1)
	(void)fprintf(stdout, "  Sticky");
      if (cle_p->ch_flags.status.flags.ok == 1)
	(void)fprintf(stdout, "  Ok");
      (void)fprintf(stdout, "\n\n");
      cle_p = CA_GET_CHPOINTER(cle_p->ch_collision.ll_flink);
    }
  }

  
}

/*
 * Dump cached users blocks and their contents
 */
static void 
dumpuser (void)
{
  int i;
  UserEntry_t *ue_p;
  CacheLinkedList_t *ll_p;
#if defined(DCE_SEC)
  byte_t * profile_p;
  unsigned char name[256]; 
#else
  unsigned char name[DNS_STR_FNAME_MAX];
#endif
  

  (void)fprintf(stdout, 
	  "\nCACHED USERS***********************************************************\n");

  ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_user);

  for (i=0; i<CAfixed_p->ca_user_hash_len; ++i) 
  {
    ue_p = CA_GET_UHPOINTER((ll_p + i)->ll_flink);
    while ((CacheLinkedList_t *)ue_p != (ll_p + i)) 
    {
      int namelen = sizeof(name);

      (void)strcpy((char *)name, "*bad SimpleName*");
#if defined(DCE_SEC)
      profile_p = ue_p->uh_profile;
      namelen = GET16(profile_p);
      (void)strcpy((char *)name, (char *)profile_p);
#else
      dnsCvtCDSSimpleToStr((dns_opq_sname *)ue_p->uh_sname, name, &namelen);
#endif
      (void)fprintf(stdout, "%.*s\n", namelen, name);
      dumpgroups(ue_p);
      dumpnames(ue_p);
      ue_p = CA_GET_UHPOINTER(ue_p->uh_collision.ll_flink);
    }
  }
}

/*
 * Dump cached group/members for a user
 */
static void 
dumpgroups (UserEntry_t *ue_p)
{
  UserGroupEntry_t *group_p;
  int i;
  unsigned char name[DNS_STR_FNAME_MAX];
  char utcstring[100];
  int namelen;
  size_t stringL;
  CacheLinkedList_t *ll_p;
  
  (void)fprintf(stdout, "  Group Name, Member Name, Timeout \n");

  ll_p = CA_GET_LLPOINTER(ue_p->uh_group);

 if (ue_p->uh_group != 0)
  for (i=0; i<USERGROUP_HASH_LEN; ++i) 
  {
    group_p = CA_GET_UGPOINTER((ll_p + i)->ll_flink);
    while ((CacheLinkedList_t *)group_p != (ll_p + i)) 
    {
      namelen = sizeof(name);
      name[0] = 0;
      (void)dnsCvtCDSFullToStr((dns_opq_fname *)group_p->ug_name,
		name, &namelen);
      (void)fprintf(stdout, "    %.*s, ", namelen, name);
      namelen = sizeof(name);
      name[0] = 0;
      (void)dnsCvtCDSFullToStr(
		(dns_opq_fname *)SKIP_FullName(group_p->ug_name), 
		name, &namelen);
      (void)fprintf(stdout, "%.*s\n", namelen, name);
      stringL = sizeof(utcstring);
      (void)utc_asclocaltime(utcstring, stringL, 
		   (utc_t *)((Timeout_t *)group_p->ug_timeout)->to_expiration);
      (void)printf("%s ", utcstring);

      stringL = sizeof(utcstring);
      (void)utc_ascreltime(utcstring, stringL,
		   (utc_t *)((Timeout_t *)group_p->ug_timeout)->to_extension);
      (void)printf("%s\n", utcstring);
      group_p = CA_GET_UGPOINTER(group_p->ug_collision.ll_flink);
    }
  }
}

/*
 * Dump cached names this user has access to
 */
static void 
dumpnames (UserEntry_t *ue_p)
{
  UserAccessEntry_t *access_p;
  int i;
  unsigned char name[DNS_STR_FNAME_MAX];
  int namelen;
  CacheLinkedList_t *ll_p;
  struct nhentry *name_p;
  struct lhentry *link_p;
  int idx,found;
  
  (void)fprintf(stdout, "  Cached Entries \n");

  ll_p = CA_GET_LLPOINTER(ue_p->uh_access);

  if (ue_p->uh_access != 0)
    for (i=0; i<USERACCESS_HASH_LEN; ++i) 
    {
      access_p = CA_GET_UAPOINTER((ll_p + i)->ll_flink);
      while ((CacheLinkedList_t *)access_p != (ll_p + i)) 
      {
	/* Hash name token to get entry name */
	idx = EXT32(access_p->ua_token) % NAMEHASHSIZE;
	
	/* Search collision list for a match */
	name_p = NameHash[idx].ll_flink;
	found = 0;
	while (name_p != (struct nhentry *)&NameHash[idx]) 
	{
	  if (EQ_bytes(name_p->nh_ptr->nh_token, access_p->ua_token, 
		       sizeof(Time_u))) 
	  {
	    char type[12], *type_p;
	    namelen = sizeof(name);
	    (void)dnsCvtCDSFullToStr((dns_opq_fname *)name_p->nh_ptr->nh_name,
			       name, &namelen);
	    switch (EXT8(name_p->nh_ptr->nh_type)) 
	    {
	    case ET_directory:
	      type_p = "directory";
	      break;
	    case ET_object:
	      type_p = "object";
	      break;
	    case ET_childPointer:
	      type_p = "childPointer";
	      break;
	    case ET_softlink:
	      type_p = "softlink";
	      break;
	    case ET_clearinghouse:
	      type_p = "clearinghouse";
	      break;
	    case ET_anyDirectName:
	      type_p = "*anyDirectName*";
	      break;
	    case ET_dirOrObj:
	      type_p = "*dirOrObj*";
	      break;
	    default:
	      (void)sprintf(type_p = type, "*%d*", EXT8(name_p->nh_ptr->nh_type));
	      break;
	    }
	    (void)fprintf(stdout, "      %.*s (%s)\n", namelen, name, type_p);
	    ++found;
	    break;
	  }
	  /* Keep searching for the name token */
	  name_p = name_p->nh_collision.ll_flink;
	}
	/* Check softlinks */
	if (!found) 
	{
	  /* Hash name token to get link name */
	  idx = EXT32(access_p->ua_token) % LINKHASHSIZE;
	
	  /* Search collision list for a match */
	  link_p = LinkHash[idx].ll_flink;
	  while (link_p != (struct lhentry *)&LinkHash[idx]) 
	  {
	    if (EQ_bytes(link_p->lh_ptr->lh_token, access_p->ua_token, 
		       sizeof(Time_u))) 
	    {
	      namelen = sizeof(name);
	      (void)dnsCvtCDSFullToStr((dns_opq_fname *)link_p->lh_ptr->lh_name,
				 name, &namelen);
	      (void)fprintf(stdout, "(SL)  %.*s", namelen, name);
	      namelen = sizeof(name);
	      (void)dnsCvtCDSFullToStr(
		 (dns_opq_fname *)SKIP_FullName(link_p->lh_ptr->lh_name), 
				 name, &namelen);
	      (void)fprintf(stdout, " -> %.*s\n", namelen, name);
	      break;
	    }
	    /* Keep searching for the name token */
	    link_p = link_p->lh_collision.ll_flink;
	  } /* End searching link cache */
	  
	} /* End not found */
	access_p = CA_GET_UAPOINTER(access_p->ua_collision.ll_flink);
      } /* End list */
    } /* End user access table */
  }

/* 
 * Print out fullnames hashed 
 */
static void 
dumpname (void)
{
  int i,j;
  NameEntry_t *name_p;
  CacheLinkedList_t *ll_p;
  unsigned char name[DNS_STR_FNAME_MAX];
  int len,namelen;
  ValueEntry_t *val_p;
  struct ahentry *att_p;
  int idx;
  byte_t *buf_p;

  (void)fprintf(stdout,
	  "\nCACHED ATTRIBUTE VALUES************************************************\n");

  ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_name);

  for (i=0; i<CAfixed_p->ca_name_hash_len; ++i) 
  {
    name_p = CA_GET_NHPOINTER((ll_p + i)->ll_flink);
    while ((CacheLinkedList_t *)name_p != (ll_p + i)) 
    {
      char type[12], *type_p;

      namelen = sizeof(name);
      name[0] = 0;
      (void)dnsCvtCDSFullToStr((dns_opq_fname *)name_p->nh_name,
			name, &namelen);
      switch (EXT8(name_p->nh_type)) 
      {
      case ET_directory:
	type_p = "directory";
	break;
      case ET_object:
	type_p = "object";
	break;
      case ET_childPointer:
	type_p = "childPointer";
	break;
      case ET_softlink:
	type_p = "softlink";
	break;
      case ET_clearinghouse:
	type_p = "clearinghouse";
	break;
      case ET_anyDirectName:
	type_p = "*anyDirectName*";
	break;
      case ET_dirOrObj:
	type_p = "*dirOrObj*";
	break;
      default:
	(void)sprintf(type_p = type, "*%d*", EXT8(name_p->nh_type));
	break;
      }
      (void)fprintf(stdout, "%.*s (%s)\n", namelen, name, type_p);

      /* Dump cached attribute/values */
      val_p = CA_GET_AVPOINTER(name_p->nh_attributes.ll_flink);
      while (val_p != (ValueEntry_t*)&name_p->nh_attributes) 
      {
	/* Hash attribute token to get attribute name */
	idx = EXT32(val_p->av_token) % ATTHASHSIZE;

	/* Search collision list for a match */
	att_p = AttHash[idx].ll_flink;
	while (att_p != (struct ahentry *)&AttHash[idx]) 
	{
	  if (EQ_bytes(att_p->ah_ptr->ah_token, val_p->av_token, 
		       sizeof(Time_u))) 
	  {
	    unsigned char dotty[DNS_STR_ANAME_MAX];
	    int dottylen = sizeof(dotty);

	    dotty[0] = 0;
	    namelen = sizeof(name);
	    name[0] = 0;
	    (void)dnsCvtObjIDToStr((dns_attr_name_t *)att_p->ah_ptr->ah_name,
				(byte_t *)0, name, &namelen, dotty, &dottylen);
	    if (strlen((char *)name) == 0)
	      (void)fprintf(stdout, "  %.*s", dottylen, dotty);
	    else
	      (void)fprintf(stdout, "  %.*s", namelen, name);

	    /* Print attribute type */
	    if (EXT8(((AttributeContents_t *)val_p->av_value)->ac_type) == 
		AT_set) 
	    {
	      (void)fprintf(stdout, " - Is a Set");
	      /* Whole entry cached ? */
	      if (val_p->av_wholeset == TRUE)
		(void)fprintf(stdout, " - Whole Set Cached\n");
	      else
		(void)fprintf(stdout, " - Partial Set Cached\n");
	    }
	    else
	      (void)fprintf(stdout, " - Is a Single\n");

	    /* Dump attribute Value in Hex */
	    len = LEN_AttributeContents(val_p->av_value) - sizeof(byte_u);
	    buf_p = ((AttributeContents_t *)val_p->av_value)->ac_set;
	    for (; 0 < len; ) 
	    {
	      (void)fprintf(stdout, "    ");
	      for (j = 0; (j < 25) && (0 < len); j++, --len) 
		(void)fprintf(stdout, "%02x ", *buf_p++);
	    (void)fprintf(stdout, "\n");
	    }
	    break;
	  }
	  /* Keep searching for the attribute token */
	  att_p = att_p->ah_collision.ll_flink;
	}
	/* Display next attribute */
	val_p = CA_GET_AVPOINTER(val_p->av_collision.ll_flink);
      }
      /* Display next cached name */
      name_p = CA_GET_NHPOINTER(name_p->nh_collision.ll_flink);
    }
  }
}

/*
 * Build table reverse mapping the attributes hashed so that the
 * token is the key to the name. 
 */
static void 
buildattributehash (void)
{
  int i;
  AttrEntry_t *att_p;
  struct ahentry *newatt_p;
  CacheLinkedList_t *ll_p;
  int idx;
  
 /* THE NEW TABLE WILL CONTAIN REAL ADDRESSES NOT OFFSETS */
  for (i=0; i<ATTHASHSIZE; ++i) 
  {
    AttHash[i].ll_flink = &AttHash[i];
    AttHash[i].ll_blink = &AttHash[i];
  }

  ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_attr);

  for (i=0; i<CAfixed_p->ca_attr_hash_len; ++i) 
  {
    att_p = CA_GET_AHPOINTER((ll_p + i)->ll_flink);
    while ((CacheLinkedList_t *)att_p != (ll_p + i)) 
    {
      idx = EXT32(att_p->ah_token) % ATTHASHSIZE;
      newatt_p = (struct ahentry *)malloc(sizeof(struct ahentry));
      newatt_p->ah_ptr = att_p;

      /* Insert onto front of collision list */
      newatt_p->ah_collision.ll_blink = &AttHash[idx];
      newatt_p->ah_collision.ll_flink = AttHash[idx].ll_flink;
  
      ((dumpLinkedList_t *)newatt_p->ah_collision.ll_flink)->ll_blink = newatt_p;
      AttHash[idx].ll_flink = newatt_p;

      att_p = CA_GET_AHPOINTER(att_p->ah_collision.ll_flink);
    }
  }
}

/*
 * Build table reverse mapping the name hash so that the
 * token is the key to the name. 
 */
static void 
buildnamehash (void)
{
  int i;
  NameEntry_t *name_p;
  struct nhentry *newname_p;
  CacheLinkedList_t *ll_p;
  int idx;
  
 /* THE NEW TABLE WILL CONTAIN REAL ADDRESSES NOT OFFSETS */
  for (i=0; i<NAMEHASHSIZE; ++i) 
  {
    NameHash[i].ll_flink = &NameHash[i];
    NameHash[i].ll_blink = &NameHash[i];
  }

  ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_name);

  for (i=0; i<CAfixed_p->ca_name_hash_len; ++i) 
  {
    name_p = CA_GET_NHPOINTER((ll_p + i)->ll_flink);
    while ((CacheLinkedList_t *)name_p != (ll_p + i)) 
    {
      idx = EXT32(name_p->nh_token) % NAMEHASHSIZE;
      newname_p = (struct nhentry *)malloc(sizeof(struct nhentry));
      newname_p->nh_ptr = name_p;

      /* Insert onto front of collision list */
      newname_p->nh_collision.ll_blink = &NameHash[idx];
      newname_p->nh_collision.ll_flink = NameHash[idx].ll_flink;
  
      ((dumpLinkedList_t *)newname_p->nh_collision.ll_flink)->ll_blink = newname_p;
      NameHash[idx].ll_flink = newname_p;

      name_p = CA_GET_NHPOINTER(name_p->nh_collision.ll_flink);
    }
  }
}

/*
 * Build table reverse mapping the link hash so that the
 * token is the key to the link. 
 */
static void 
buildlinkhash (void)
{
  int i;
  LinkEntry_t *link_p;
  struct lhentry *newlink_p;
  CacheLinkedList_t *ll_p;
  int idx;
  
 /* THE NEW TABLE WILL CONTAIN REAL ADDRESSES NOT OFFSETS */
  for (i=0; i<LINKHASHSIZE; ++i) 
  {
    LinkHash[i].ll_flink = &LinkHash[i];
    LinkHash[i].ll_blink = &LinkHash[i];
  }

  ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_link);

  for (i=0; i<CAfixed_p->ca_link_hash_len; ++i) 
  {
    link_p = CA_GET_LHPOINTER((ll_p + i)->ll_flink);
    while ((CacheLinkedList_t *)link_p != (ll_p + i)) 
    {
      idx = EXT32(link_p->lh_token) % LINKHASHSIZE;
      newlink_p = (struct lhentry *)malloc(sizeof(struct lhentry));
      newlink_p->lh_ptr = link_p;

      /* Insert onto front of collision list */
      newlink_p->lh_collision.ll_blink = &LinkHash[idx];
      newlink_p->lh_collision.ll_flink = LinkHash[idx].ll_flink;
  
      ((dumpLinkedList_t *)newlink_p->lh_collision.ll_flink)->ll_blink = newlink_p;
      LinkHash[idx].ll_flink = newlink_p;

      link_p = CA_GET_LHPOINTER(link_p->lh_collision.ll_flink);
    }
  }
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
  if ((status <= 0) || (status < sizeof(struct CacheFixed))) 
  {
    (void)close(cachefile);
    return(0);
  }

  /* Make sure it is the current version.  If not, ignore file */
  if (!CheckCacheVersion(&cache)) 
  {
    (void)close(cachefile);
    return(0);
  }

  if (!(CAfixed_p = (struct CacheFixed *)malloc(cache.ca_size))) 
  {
    (void)close(cachefile);
    return(0);
  }
  /* Move fixed portion into shared memory */
  *CAfixed_p = cache;
  
  /* Read the rest of cache into memory */
  size = CAfixed_p->ca_size - sizeof(struct CacheFixed);
  buf_p = (char *)(CAfixed_p + 1);
  while (size > 0) 
  {
    if (size > _CDS_PAGE_SIZE_)
    { 
      status = read(cachefile, buf_p, _CDS_PAGE_SIZE_);
    }
    else
    { 
      status = read(cachefile, buf_p, size);
    }
    /* If what we were expecting wasn't there, return an error */  
    if (status <= 0) 
    {
      (void)close(cachefile);
      return(0);
    }
    
    size -= status;
    buf_p += status;
  }
  (void)close(cachefile);
  return(1);
}
