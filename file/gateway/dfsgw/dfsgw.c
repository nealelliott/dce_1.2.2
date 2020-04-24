/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: dfsgw.c,v $
 * Revision /main/HPDCE02/5  1995/06/22  20:13 UTC  maunsell_c
 * 	mothra C&S fixes
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/6  1995/06/15  18:26 UTC  maunsell_c
 * 	fix UID_MAX build, limits.h says UID_MAX is MAXUID
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/5  1995/06/14  21:12 UTC  brezak
 * 	Remove check_userid() function and inline check in get_id().
 * 	chec_userid() should be able to deal with 32-bit uid's.
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/4  1995/06/09  03:08 UTC  brezak
 * 	Fixes from Transarc
 *
 * Revision /main/HPDCE02/brezak_dfsgw/3  1995/06/07  14:59 UTC  brezak
 * 	Need to use afs_syscall() instead of syscall().
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/2  1995/05/30  19:12 UTC  brezak
 * 	Translate user name to uid.
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/1  1995/05/26  17:25 UTC  brezak
 * 	Zap extra newline on ctime() returns.
 * 	Print @host and @sys values in list and query
 * 	Default @host to hostname in add.
 * 
 * Revision /main/HPDCE02/4  1995/04/06  18:08 UTC  maunsell_c
 * 	CHFts14954
 * 
* Revision /main/HPDCE02/maunsell_mothra1.47/1  1995/04/05  13:56 UTC  maunsell_c
 * 	fix del error display could not add message
 * 
 * Revision /main/HPDCE02/3  1995/02/14  22:42 UTC  maunsell_c
 * 	@sys @host changes for nfs/dfs gateway
 * 
 * Revision /main/HPDCE02/2  1995/01/30  23:00 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:40 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * Revision /main/HPDCE02/1  1994/11/09  14:04 UTC  maunsell_c
 * 	use dfs_version file
 * 
 * Revision /main/maunsell_mothra1.42/1  1994/10/27  21:37 UTC  maunsell_c
 * 	use dfs version file
 * 
 * Revision 1.1.2.4  1994/09/16  19:50:03  maunsell_c
 * 	if tgt exists, use it w/o -dceid prompt
 * 	[1994/09/16  15:27:39  maunsell_c]
 *
 * Revision 1.1.2.3  1994/08/22  16:16:16  maunsell_c
 * 	dfsgw list now displays unix username vs. unix uid
 * 	  on getpwuid fail displays unix userid
 * 	[1994/08/10  16:33:03  maunsell_c]
 * 
 * Revision 1.1.2.2  1994/08/08  18:54:35  mckeen
 * 	Added serviceability
 * 	[1994/08/08  17:46:54  mckeen]
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:03  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:44  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:01:50  mckeen]
 * 
 * $EndLog$
 */
/*
 * dfsgw.c
 */
/*
 * dfsgw.c - Authentication table (AT) control program.
 *
 * This program is used to manipulate the authentication table (AT) database.
 *
 * It is run in several modes:
 *
 * Add mode: Add an entry to the AT.  This sets up a relationship between a
 * networkID:userID pair and dce_login_name:password, given an address family.
 * The actual relationship is bound between networkID:userID pair and a Process
 * Activation Group (PAG).  The PAG is used interally in HP-DFS for authentication
 * of alien access.
 *
 * Usage: dfsgw add -id <networkID>:<userID> [-dceid <login_name>[:<password>]] [-sysname <name>] [-remotehost <name>] [-af <address_family>]
 *
 * The -id argument is required.
 * The -dceid arguments are optional, but
 * if missing will have to be supplied interactivley.  The interactive mode allows
 * secure entry of the dce password. The -sysname argument is optional and represents the 
 * @sys value of the remote nfs client.  The -remotehost argument is optional and
 * represents the @host value of the remote nfs client. The -af argument is optional.
 *
 * Delete mode: Delete an entry from the AT.  This breaks the previous relationship.
 *
 * Usage: dfsgw delete -id <networkID>:<userID>  [ -af <address_family> ]
 * 
 * List mode: List all entries in the AT.  This lists all networkID:userID => PAG
 * relationships.
 *
 * Usage: dfsgw list
 *
 * Query mode: Queries the AT for existence of a relationship.  
 *
 * Usage: dfsgw query -id <networkID>:<userID> [ -af <address_family> ]
 */
#include <dce/dce.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <dcedfs/param.h>

#ifdef AFS_SUNOS5_ENV
#undef MIN
#undef MAX
#endif

#include <dcedfs/osi.h>
#include <dcedfs/syscall.h>
#include <dcedfs/auth_at.h>
#include <dcedfs/cmd.h> 
#include <dcedfs/cmdAux.h>
#include <dcedfs/cm.h>
#include <dcedfs/ioctl.h>
#include <dfsgw_userint.h>
#include <netid.h>
#include <inetid.h>
#include <dcecred.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsgwymac.h>
#include <dfsgwysvc.h>
#include <dfsgwymsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/gateway/dfsgw/RCS/dfsgw.c,v 1.9 1996/04/23 20:23:23 rajesh Exp $")

char *progname;

struct {
        uid_t uid;
        netid_p net_addr;
        char  sysname[CM_MAXSYSNAME];
        char  remotehostname[OSI_MAXHOSTNAMELEN]; 
} id_info = { -2, NULL, {'\0'}, {'\0'} }; 

boolean32 dceid_supplied = NULL;

DCE_SVC_DEFINE_HANDLE(gwy_svc_handle, gwy_svc_table, "gwy")

void initialize_svc()
{
  error_status_t st;

  gwy_svc_handle = dce_svc_register(gwy_svc_table,  (idl_char *)"gwy",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dce_msg_define_msg_table(gwy__table,
        sizeof gwy__table / sizeof gwy__table[0], &st);
  if (st != svc_s_ok)
        fprintf(stderr, 
		"Warning Cannot register gwy incore message table 0x%lx\n", 
		st);

}

/*
 * get_af - get and verify address_family.
*/
get_af(struct cmd_syndesc *as, char **addr_family)
{

    if (cmd_IsParamPresent(as, ADDR_FAMILY_OFFSET)) {
       *addr_family = as->parms[ADDR_FAMILY_OFFSET].items->data;
    } else {
       *addr_family = "inet";
    }
    if (strcmp(*addr_family,"inet") != 0) {
       dce_svc_printf(GWY_S_ADDRFAMILYERR_MSG, progname, *addr_family);
       exit(1);
    }
}

/*
 * splitid - ID parser.
 *
 * Split a pattern <arg0>[:<arg1>], return actual number
 * of arguments returned.
 */
int splitid(char *pattern, char **arg0, char **arg1)
{
        *arg0 = *arg1 = pattern;
        while ((**arg1) && (**arg1 != ':')) {
                ++*arg1;
        }
        if (**arg1) {
                **arg1 = '\0';
                ++*arg1;
                if (**arg1)
                   return(2);
                else {
                   *arg1 = 0;
                   return(1);
                }
                   
        } else {
                *arg1 = 0;
                return(1);
        }
}

/*
 * get_id - get network and userid 
 */
uid_t get_id(struct cmd_syndesc *as, char **networkid, char **userid)
{
    struct passwd *pw;
    u_int uid;
    
    if (splitid(as->parms[NET_ID_OFFSET].items->data, networkid, userid) != 2) {
	dce_svc_printf(GWY_S_IDPAIRERR_MSG, progname);
	exit(1);
    }

    if (pw = getpwnam(*userid))
	return pw->pw_uid;
    
    if (sscanf(*userid, "%d", &uid) != 1) {
	dce_svc_printf(GWY_S_UIDERR_MSG, progname, *userid);
	exit(1);
    }
    
    if (uid > MAXUID) {
	dce_svc_printf(GWY_S_UIDERR_MSG, progname, *userid);
	exit(1);
    }
    return (uid_t)uid;
}


/* actual commands dfsgw performs */
/* dfsgw add  -id <networkID>:<userID> [ -dceid <login_name>[:<password>]]  [ -af <address_family>] */

int dfsgw_AddCmd(struct cmd_syndesc *as, char *aRock)
{
    long code = 0;
    dcecred_p dcred = 0;
    char *addr_family, *username, *password;
    char *networkid, *userid;
    at_pag_mapping_t mapping;
    struct passwd *pw;
    char pwname[32];
    char *pwusername;
    unsigned long pag = 0;
    int existing_cred;

    dceid_supplied = 0;
    existing_cred = 0;

    memset(&mapping, 0, sizeof(mapping));

    /* get -af address_family */
    addr_family = 0;
    get_af(as, &addr_family);

    /* get optional sysname and remotehost */
    if (cmd_IsParamPresent(as, SYSNAME_OFFSET)) {
        strcpy(id_info.sysname, as->parms[SYSNAME_OFFSET].items->data);
    }

    if (cmd_IsParamPresent(as, REMOTEHOST_OFFSET)) {
        strcpy(id_info.remotehostname, as->parms[REMOTEHOST_OFFSET].items->data);
    } 

    /* get -id networkID:userID */
    networkid = userid = 0;
    id_info.uid = get_id(as, &networkid, &userid);
    
    /* do some initializing */
    id_info.net_addr = inetid_init();

    /* call assign id here */
    if (id_info.net_addr->assign_id_str(networkid)) {
       dce_svc_printf(GWY_S_ASSIGNNETIDERR_MSG, progname, networkid);
       exit(1);
    }

    /* Find if mapping already exists for this user */
    mapping.uid = id_info.uid;
    if (id_info.net_addr->get_sockaddr(&mapping.addr)) {
       dce_svc_printf(GWY_S_CONVERTNETIDERR_MSG, progname);
       exit(1);
    }

    /* Find out if a valid ticket already exists */
    if (afs_syscall(AFSCALL_AT, AT_QUERY, (long)&mapping, 
		    NULL, NULL, NULL) == 0) {
	pag = mapping.pag;
	osi_assert(pag != 0);
    }

    /* get optional -dceid login_name:password */
    if (cmd_IsParamPresent(as, DCE_ID_OFFSET)) {
       dceid_supplied = TRUE;
       username = password = 0;
       splitid(as->parms[DCE_ID_OFFSET].items->data, &username, &password);

       /* call initializer here */
       dcred = dcecred_init(username, pag, &existing_cred);

       if (password) {
          if (dcred->validate(0, password) < 0) {
             dce_svc_printf(GWY_S_DCEPASSWDERR_MSG, progname);
             exit(1);
	  }
       }
    } 
    else {
    /*
     * Supplying -dceid is optional, but need to have one
     * for default interactive query.
     */
       dceid_supplied = NULL;
       /* if -dceid not used, pass along unix login id  */
       if ((pw = getpwuid(id_info.uid)) == NULL) {
          dcred = dcecred_init(0, pag, &existing_cred);
       }
       else {
          /* call initializer here */
          dcred = dcecred_init(pw->pw_name, pag, &existing_cred);
       } 
       if (dcred->validate(0, 0) < 0) {
          exit(1);
       }
    }

    mapping.pag = (unsigned long)dcred->pag();
    if (mapping.pag == (unsigned long)-1) {
      dce_svc_printf(GWY_S_DCECREDERR_MSG, progname);
      exit(1);
    }
    mapping.expiration_time = dcred->expiration_time();
    if (mapping.expiration_time == 0) {
       dce_svc_printf(GWY_S_DCECREDTIMEERR_MSG, progname);
       exit(1);
    }

    if (strlen(id_info.remotehostname))
       strcpy(mapping.hostname, id_info.remotehostname);
    else if (! id_info.net_addr->get_str(mapping.hostname, sizeof(mapping.hostname))) {
       dce_svc_printf(GWY_S_CONVERTNETIDERR_MSG, progname);
       exit(1);
    }
    
    if (strlen(id_info.sysname))
	strcpy(mapping.sysname, id_info.sysname);

    /* 
     * If we are creating a cred for the first time for a remote user
     * we need to install both the pag and the expiration time. If
     * refreshing an existing cred, we need to update the expiration
     * time. We use the AT_ADD syscall to do both 
     */

    if (existing_cred) {
	code = afs_syscall(AFSCALL_AT, AT_ADD, (long) &mapping, 
			   AT_ADD_UPDATE_EXPTIME, NULL, NULL);
	if (code < 0) {
	    dce_svc_printf(GWY_S_SYSCALLADDERR_UPDATE_MSG, progname, 
			   strerror(errno));
	    exit(1);
	} else
	    dce_svc_printf(GWY_S_MAPEXISTS_MSG, mapping.pag);
    } else {
	code = afs_syscall(AFSCALL_AT, AT_ADD, (long) &mapping, 
			   AT_ADD_NEW_ENTRY, NULL, NULL);
	if (code < 0) {
	    dce_svc_printf(GWY_S_SYSCALLADDERR_MSG, progname, strerror(errno));
	    exit(1);
	} else 
	    dce_svc_printf(GWY_S_MAPADDED_MSG, mapping.pag);
    }
    return(code);
}

/* dfsgw delete -id <networkID>:<userID> [-af <address_family>] */
int dfsgw_DeleteCmd(struct cmd_syndesc *as, char *aRock)
{
    long code = 0;
    char *addr_family, *username, *password;
    char *networkid, *userid;
    at_pag_mapping_t mapping;

    /* get -af address_family */
    addr_family = 0;
    get_af(as, &addr_family);

    /* get -id networkID:userID */
    id_info.uid = get_id(as, &networkid, &userid);

    /* do some initializing */
    id_info.net_addr = inetid_init();

    /* call assign id here */
    if (id_info.net_addr->assign_id_str(networkid)) {
       dce_svc_printf(GWY_S_ASSIGNNETIDERR_MSG, progname, networkid);
       exit(1);
    }

    mapping.uid = id_info.uid;
    if (id_info.net_addr->get_sockaddr(&mapping.addr)) {
       dce_svc_printf(GWY_S_CONVERTNETIDERR_MSG, progname);
       exit(1);
    }
    if (afs_syscall(AFSCALL_AT, AT_DELETE, (long)&mapping, 
		    NULL, NULL, NULL) == -1) {
       dce_svc_printf(GWY_S_SYSCALLDELERR_MSG, progname);
       exit(1);
    } else {
       dce_svc_printf(GWY_S_MAPDELETED_MSG, progname);
    }
    return(code);
}

/* dfsgw list */
int dfsgw_ListCmd(struct cmd_syndesc *as, char *aRock)
{
    long code = 0;
    int nmappings;
    at_pag_mapping_p temp_mappings,mappings = NULL;
    int i;
    netid_p mnet = NULL;
    char netname[OSI_MAXHOSTNAMELEN];
    struct passwd *pw;
    char *exptime;

    /*
     * First find size of internal table, allocate enough space to hold
     * it, then get table.
     */
    if (afs_syscall(AFSCALL_AT, AT_GETSIZE, (long)&nmappings, 
		    NULL, NULL, NULL) == -1) {
            dce_svc_printf(GWY_S_SYSCALLSIZEERR_MSG, progname);
	    exit(1);
    }
    if (nmappings) {
       if ((mappings = (at_pag_mapping_p)malloc(sizeof(at_pag_mapping_t) * nmappings)) == NULL) {
          dce_svc_printf(GWY_S_MALLOCERR_MSG, progname);  
	  exit(1);
       }
       memset(mappings, 0, sizeof(at_pag_mapping_t) * nmappings);
       if (afs_syscall(AFSCALL_AT, AT_LIST, (long)&nmappings, (long)mappings, 
		       NULL, NULL) == -1) {
          free(mappings);
          dce_svc_printf(GWY_S_SYSCALLOBTAINTBLERR_MSG, progname);
	  exit(1);
       }  
       /*
        * Go down table elements, obtaining information about the network addresses,
        * printing out as much info as possible.
        */
        temp_mappings = mappings;
        for (i = 0; i < nmappings; i++, temp_mappings++) {
           switch (temp_mappings->addr.sa_family) {
              case AF_INET :
                 mnet = inetid_init();
                 mnet->assign_id_addr(&temp_mappings->addr);
                 break;
              default :
                 dce_svc_printf(GWY_S_UNKNOWNADDRFAMILY_MSG, progname,
				(void *)temp_mappings->addr.sa_family);
                 mnet = 0;
                 break;
              }
	   exptime = ctime(&temp_mappings->expiration_time);
	   exptime[strlen(exptime)-1] = '\0';
	   
           if (mnet) {
              if ( mnet->get_str(&netname, sizeof(netname))) {
                 if ((pw = getpwuid(temp_mappings->uid)) == NULL) {
                   dce_svc_printf(GWY_S_MAPIS_USERID_MSG, netname,
                               temp_mappings->uid, temp_mappings->pag,
                               exptime,
			       temp_mappings->hostname,
			       temp_mappings->sysname);			  
                 } else {
                 dce_svc_printf(GWY_S_MAPIS_MSG, netname, 
				pw->pw_name, temp_mappings->pag, 
				exptime,
				temp_mappings->hostname,
				temp_mappings->sysname);		  
                 }
              } else {
                 dce_svc_printf(GWY_S_UNKNOWNADDRFAMILY_MSG, progname,
			    (void *)temp_mappings->addr.sa_family);
              }
           } 
        }
        free(mappings);
    } else {
        dce_svc_printf(GWY_S_NOMAPPINGS_MSG);
    }
    return(code);
}

/*  dfsgw query -id <networkID>:<userID> [-af <address_family>] */
int dfsgw_QueryCmd(struct cmd_syndesc *as, char *aRock)
{
    long code = 0;
    char *addr_family, *username, *password;
    char *networkid, *userid;
    at_pag_mapping_t mapping;
    char *exptime;

    /* get -af address_family */
    addr_family = 0;
    get_af(as, &addr_family);

    /* get -id networkID:userID */
    id_info.uid = get_id(as, &networkid, &userid);

    /* do some initializing */
    id_info.net_addr = inetid_init();

    /* call assign id here */
    if (id_info.net_addr->assign_id_str(networkid)) {
       dce_svc_printf(GWY_S_ASSIGNNETIDERR_MSG, progname, networkid);
       exit(1);
    }

    mapping.uid = id_info.uid;
    if (id_info.net_addr->get_sockaddr(&mapping.addr)) {
      dce_svc_printf(GWY_S_CONVERTNETIDERR_MSG, progname);
      exit(1);
    }
    if (afs_syscall(AFSCALL_AT, AT_QUERY, (long)&mapping, 
		    NULL, NULL, NULL) == -1) {
      dce_svc_printf(GWY_S_SYSCALLNOMAPERR_MSG, progname);
      exit(1);
    } else {
      /* query now provides info for expire time too */
      exptime = ctime(&mapping.expiration_time);
      exptime[strlen(exptime)-1] = '\0';
      dce_svc_printf(GWY_S_MAPPINGFOUND_MSG, mapping.pag, exptime);
    }
    return(code);
}

main(int argc, char **argv)
{
    long code;
   
    initialize_svc();

    progname = *argv;

    SetUpSyntax();
    code = cmd_Dispatch(argc, argv);
    exit (code);
}
